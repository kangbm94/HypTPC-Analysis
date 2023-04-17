#include "../include/ReconTools.hh"
#ifndef ReconTools_C
#define ReconTools_C
#include "KinFit.cc"
namespace{
	TVector3 Mom(double x,double y,double u,double v,double p){
		double pt = p/sqrt(1+u*u+v*v);
		return TVector3(pt*u,pt*v,pt);
	}
	TVector3 Pos(double x,double y,double z){
		return TVector3(x,y,z);
	}
	bool InTarget(TVector3 pos,double cd){
		if(abs(pos.X())<20 
				and abs(pos.Y())<25
				and abs(pos.Z()+30)<100
				and cd < 100) return true;
		else return false;
	}
}
Recon::Recon(vector<TLorentzVector> D,TVector3 vertex,double clos_dist,int id1,int id2, int charge_){
	charge = charge_;
	LV.SetXYZM(0,0,0,0);
	Daughters = D;
	Vert=vertex;
	close_dist=clos_dist;
	for(auto lv:D) LV+=lv;
	CombID = pow(2,id1)+pow(2,id2);
	exist = true;
	auto V_t = GlobalToTarget(Vert);
	auto mom = LV.Vect();
	auto dir_t = GlobalToTargetMom(mom);
	dir_t = dir_t* (1/dir_t.Y());
	auto u = dir_t.X();
	auto v = dir_t.Z();
	par[0]=V_t.X()-V_t.Z()*u,par[1]=V_t.Z()-V_t.Y()*v,par[2]= u,par[3]=v;
	if(charge){
		GetHelixParameter(Vert,mom,charge,par);
	}
	trid1=id1;trid2=id2;
}

bool Vertex::AddTrack(Track p){
	int np = NTrack();
	int nt = p.GetID();
	if(Counted(p)) return false;
	if(np==1){
		auto par1 = Tracks[0].GetPar();
		auto par2 = p.GetPar();
		double cd,t1,t2;
		auto pos = VertexPointHelix(par1,par2,cd,t1,t2);
		if(cd<cdcut){
			Tracks.push_back(p);
			verts.push_back(pos);
			Vert_id+=pow(2,nt);
			SetVert();
			return true;
		}
	}
	else{
		double t = GetTcal(p.GetPar(),vert);
		auto point = HelixPos(p.GetPar(),t);
		double cd = (vert-point).Mag();
		double ct,t1,t2;
		if(cd<cdcut*1.5){
			vector<TVector3> cand;
			for(auto pt:Tracks){
				cand.push_back(VertexPointHelix(pt.GetPar(),p.GetPar(),ct,t1,t2));
			}
			TVector3 vec(0,0,0);
			for(auto v:cand){vec+=v*(1./cand.size());	
				Tracks.push_back(p);Vert_id+=pow(2,nt);verts.push_back(vec); SetVert();
			}
			return true;
		}
	}
	return false;
}
void Vertex::SearchLdCombination(){
	int np = NTrack();
	vector<Track>PCand;PCand.clear();
	vector<Track>PiCand;PiCand.clear();
	for(auto p:Tracks){
		if(p.IsP())PCand.push_back(p);
		if(p.IsPi() and p.IsNegative())PiCand.push_back(p);
	}
	double mom_cut = 0.9;//1.2  
	double mom_cutMin = 0.2;//0.3  
	for(auto p:PCand){
		for(auto pi:PiCand){
			double cd_,t1_,t2_;
			if(p.GetID() == pi.GetID()) continue;
			auto ppivert = VertexPointHelix(p.GetPar(),pi.GetPar(),cd_,t1_,t2_); 
			if(cd_>cdcut) continue;
			auto p1 = CalcHelixMom(p.GetPar(),ppivert.y());
			auto p2 = CalcHelixMom(pi.GetPar(),ppivert.y());
			auto pLV = TLorentzVector(p1,sqrt(mp*mp+p1.Mag2()));
			auto piLV = TLorentzVector(p2,sqrt(mpi*mpi+p2.Mag2()));
//			vector<TLorentzVector> lv1 = {pLV,piLV};
//			auto ldlv1 = pLV+piLV;
//			if(ldlv1.Vect().Mag()<mom_cut and ldlv1.Vect().Mag()>mom_cutMin)LdCand.push_back(Recon(lv1,ppivert,cd_,p.GetID(),pi.GetID()));
			auto piLVInv = TLorentzVector(-p2,sqrt(mpi*mpi+p2.Mag2()));
			vector<TLorentzVector> lv2 = {pLV,piLVInv};
			auto ldlv2 = pLV+piLVInv;
			if(ldlv2.Vect().Mag()<mom_cut and ldlv2.Vect().Mag()>mom_cutMin)LdCand.push_back(Recon(lv2,ppivert,cd_,p.GetID(),pi.GetID()));
		}
	}
}

bool VertexLH::AddTrack(Track p){
	int np = NTrack();
	int nt = p.GetID();
	if(Recons[0].Counted(p)) return false;
	if(!(Recons[0].Exist())) return false;
	if(np==1 or np!=1){
		auto par1 = p.GetPar();
		auto par2 = Recons[0].GetPar();
		double cd,t1,t2;
		auto vv = Recons[0].Vertex();
		auto pos = VertexPointHelixLinear(par1,par2,cd,t1,t2);
		if(cd<cdcut){
				auto prop = vv-pos;
//				cout<<Form("Cd=%f, PropDist(%f,%f,%f)",cd,prop.X(),prop.Y(),prop.Z())<<endl;
				//	cout<<Form("LdVert(%f,%f,%f)",vv.X(),vv.Y(),vv.Z())<<endl;
		//	cout<<Form("HLVert(%f,%f,%f)",pos.X(),pos.Y(),pos.Z())<<endl;
			Tracks.push_back(p);
			verts.push_back(pos);Vert_id+=pow(2,nt);SetVert();
			return true;
		}
	}
	else{
		double t = GetTcal(p.GetPar(),vert);
		auto point = HelixPos(p.GetPar(),t);
		double cd = (vert-point).Mag();
		double ct,t1,t2;
		if(cd<cdcut){
			vector<TVector3> cand;
			for(auto pt:Tracks){
				cand.push_back(VertexPointHelix(pt.GetPar(),p.GetPar(),ct,t1,t2));
			}
			TVector3 vec(0,0,0);
			for(auto v:cand){
				vec+=v*(1./cand.size());	
				Tracks.push_back(p);Vert_id+=pow(2,nt);verts.push_back(vec); SetVert();
			}
			return true;
		}
	}
	return false;
}
void VertexLH::SearchXiCombination(){
	int np = NTrack();
	LdCand.clear();
	vector<Track>PiCand;PiCand.clear();
	for(auto p:Tracks){
		if(p.IsPi() and p.IsNegative())PiCand.push_back(p);
	}
	double mom_cut = 0.9;//Xi->0.9 
	double mom_cutMin = 0.4;//Xi-> 0.4 
	for(auto ld:Recons) LdCand.push_back(ld);
	for(auto ld:LdCand){
		for(auto pi:PiCand){
			double cd_,t1_,t2_;
			if(ld.Counted(pi)) continue;
			auto ldpivert = VertexPointHelixLinear(pi.GetPar(),ld.GetPar(),cd_,t1_,t2_); 
			TVector3 ldmomdir = ld.Momentum() * (1./ld.Momentum().Mag());
			TVector3 lddir = ld.Vertex() - ldpivert;
			lddir = lddir * (1./lddir.Mag());
			if(ldmomdir * lddir < 0) continue;
			auto p2 = CalcHelixMom(pi.GetPar(),ldpivert.y());
			std::bitset<8>ldb(ld.GetID());
//			cout<<"cd = "<<cd_<<" PiID,LdId = ("<<pi.GetID()<<" , "<<ldb<<" )"<<endl;
			auto ldLV = ld.GetLV();

			auto piLV = TLorentzVector(p2,sqrt(mpi*mpi+p2.Mag2()));
//			vector<TLorentzVector> lv1 = {ldLV,piLV};
//			auto xilv1 = ldLV+piLV;
//			if(xilv1.Vect().Mag()<mom_cut and xilv1.Vect().Mag()>mom_cutMin)XiCand.push_back(Recon(lv1,ldpivert,cd_,ld.GetID(),pi.GetID(),-1));
			auto piLVInv = TLorentzVector(-p2,sqrt(mpi*mpi+p2.Mag2()));
			vector<TLorentzVector> lv2 = {ldLV,piLVInv};
			auto xilv2 = ldLV+piLVInv;
			if(xilv2.Vect().Mag()<mom_cut and xilv2.Vect().Mag()>mom_cutMin)XiCand.push_back(Recon(lv2,ldpivert,cd_,ld.GetID(),pi.GetID(),-1));
		
			
/*
			auto ldPLV = ld.GetDaughter(0);
			auto ldPP = ldPLV.Vect(); 
			auto ldPPres = ldPP * (0.03);
			
			auto ldpiLV = ld.GetDaughter(1);
			auto ldpiP = ldpiLV.Vect(); 
			auto ldpiPres = ldpiP * (0.03);
			Fitter.AssignLorentzVector(ldPLV,ldpiLV);
			Fitter.SetResolution(ldPPres,ldpiPres);
			
			Fitter.DoKinematicFit();
			auto ldPCor = Fitter.GetFittedLV().at(0);
			auto ldpiCor = Fitter.GetFittedLV().at(1);
			auto ldLVCor = ldPCor+ldpiCor;

			auto xiCorlv1 = ldLVCor+piLV;
			vector<TLorentzVector> lv1Cor = {ldLVCor,piLV};
			if(xiCorlv1.Vect().Mag()<mom_cut and xiCorlv1.Vect().Mag()>mom_cutMin)XiCorCand.push_back(Recon(lv1Cor,ldpivert,cd_,ld.GetID(),pi.GetID()));
			auto xiCorlv2 = ldLVCor+piLVInv;
			vector<TLorentzVector> lv2Cor = {ldLVCor,piLVInv};
			if(xiCorlv2.Vect().Mag()<mom_cut and xiCorlv2.Vect().Mag()>mom_cutMin)XiCorCand.push_back(Recon(lv2Cor,ldpivert,cd_,ld.GetID(),pi.GetID()));
		*/
		}
	}
}

void VertexLH::AddKuramaTrack(Track p){
	KuramaFlag = true;
	KuramaTracks.push_back(p);
}
void
XiStarRecon::Construct(vector<TVector3>KMX,vector<TVector3>KMP,vector<TVector3>KPX,vector<TVector3>KPP){
	int ntK18 = KMX.size();
	int ntKurama = KPX.size();
	KMPos=KMX;
	KMmom=KMP;
	KPPos=KPX;
	KPmom=KPP;
	charge = -1;
	vector<double>close_dists;
	vector<double>MMs;
	vector<TLorentzVector>LVs;
	vector<TVector3> verts;
	vector<int>kpids;
	vector<int>kmids;
	bool Correction = true;
	if(!K18Track.GetQ() and !KuramaTrack.GetQ()) Correction = true;
	Correction = false;
	for(int ikm = 0;ikm<ntK18;++ikm){
		auto xkm = KMX.at(ikm);
		auto pkm = KMP.at(ikm);
		
		for(int ikp = 0;ikp<ntKurama;++ikp){
			auto xkp = KPX.at(ikp);
			auto pkp = KPP.at(ikp);
			auto kkvert = VertexPoint(xkm, xkp, pkm, pkp);
//			cout<<Form("Xi0Vertex (%f,%f,%f)",kkvert.x(),kkvert.y(),kkvert.z())<<endl;
			double cd = CloseDist(xkm,xkp,pkm,pkp); 
//			cout<<cd<<endl;
			if(!InTarget(kkvert,cd))continue;
			TLorentzVector LVKM(pkm,sqrt(mk*mk+pkm*pkm));
			TLorentzVector LVKP(pkp,sqrt(mk*mk+pkp*pkp));
			TLorentzVector LVTarget(0,0,0,mp);
			TLorentzVector LVMM = LVKM+LVTarget-LVKP;
			double MM = LVMM.Mag();
			verts.push_back(kkvert);
			close_dists.push_back(cd);
			LVs.push_back(LVMM);
			MMs.push_back(MM);
			kmids.push_back(ikm);
			kpids.push_back(ikp);
		}
	}
	double dif = 1e9;
	int id = -1;
	double mxistar = 0.;
	for(int im=0;im<MMs.size();++im){
		if(dif >abs( MMs.at(im) - mXiStar)){
			dif = abs(MMs.at(im)-mXiStar);
			id = im;
			mxistar = MMs.at(im);
			kmid=kmids.at(im);
			kpid=kpids.at(im);
			cout<<"XiStar "<<mxistar<<"GeV"<<endl;
		}
	}
	if(id>-1){
		exist = true;
		double cd,t1,t2;
		if(Correction){
			auto p1 = K18Track.GetPar();
			auto p2 = KuramaTrack.GetPar();
			/*
			double xt = verts.at(id).x();
			double yt = verts.at(id).y();
			double zt = verts.at(id).z();
			double dx = xt-( -p1[0]);//cx_helix = -cx_real
			double dz = xt-( p1[1]+ZTarget);//cy_helix = cz_real+ZTarget
			double t = atan2(dx,dz);
			double vkm = p1[4];
			double dirz = -sin(t)/sqrt(1+vkm*vkm);
			double dirx = cos(t)/sqrt(1+vkm*vkm);
			TVector3 KMDir(dirx,vkm,dirz); 
			if(dirz<0)KMDir = -KMDir;
			dx = xt-( -p2[0]);//cx_helix = -cx_real
			dz = xt-( p2[1]+ZTarget);//cy_helix = cz_real+ZTarget
			t=atan2(dx,dz);
			double vkp = p2[4];
			dirz = -sin(t)/sqrt(1+vkp*vkp);
			dirx = cos(t)/sqrt(1+vkp*vkp);
			TVector3 KPDir(dirx,vkp,dirz); 
			if(dirz<0)KPDir = -KPDir;
			TVector3 PK18 = KMDir*(KMP.at(kmid)).Mag();
			TVector3 PKurama = KPDir*(KPP.at(kpid)).Mag();
			Vert = TVector3(xt,yt,ZTarget);
			*/
						Vert = VertexPointHelix(K18Track.GetPar(),KuramaTrack.GetPar(), cd,t1,t2);
			
//			cout<<Form("Vertex  = (%f,%f,%f)",Vert.X(),Vert.Y(),Vert.Z())<<endl;
//			Vert.SetZ(ZTarget);	
//			cout<<Form("Vertex  = (%f,%f,%f)",Vert.X(),Vert.Y(),Vert.Z())<<endl;
			auto PK18 = KMP.at(kmid);//= CalcHelixMom(K18Track.GetPar(),Vert.y());
			if(PK18.z()<0) PK18 = -PK18;
//			auto PKurama = 1.*CalcHelixMom(KuramaTrack.GetPar(),Vert.y());
			PK18 = PK18 *( 1./(PK18.Mag()) )* KMP.at(kmid).Mag();
			auto PKurama = KPP.at(kpid);
			//			PKurama = PKurama * (1./(PKurama.Mag()) )* KPP.at(kpid).Mag();

	
			//			cout<<"KPMom = "<<KPP.at(kpid).Mag()<<endl;
//			if(PK18.z()<0) PK18 = -PK18;
//			if(PKurama.z()<0) PKurama = -PKurama;
			cout<<Form("KM Mom = (%f,%f,%f)",PK18.X(),PK18.Y(),PK18.Z())<<endl;
			cout<<Form("KP Mom = (%f,%f,%f)",PKurama.X(),PKurama.Y(),PKurama.Z())<<endl;

			TLorentzVector LVKM(PK18,sqrt(mk*mk+PK18.Mag2()));
			TLorentzVector LVKP(PKurama,sqrt(mk*mk+PKurama.Mag2()));
			TLorentzVector LVTarget(0,0,0,mp);
			TLorentzVector LV = LVKM+LVTarget-LVKP;
			double MMraw = LV.Mag();
			double angle = acos( PK18*PKurama /	PK18.Mag()/PKurama.Mag());
			double diff = - sin(angle)*PK18.Mag()*PKurama.Mag()/sqrt(MMraw);
			double dtheta = (MMraw-mXiStar)/diff; //diff*dtheta = MXi +dM =MM
			cout<<"MMraw = "<<MMraw<<endl;
			cout<<"CorAngle = "<<dtheta<<endl;
			PKurama.RotateY(dtheta);
			LVKP= TLorentzVector(PKurama,sqrt(mk*mk+PKurama.Mag2()));
			LV = LVKM+LVTarget-LVKP;
			double MM = LV.Mag();
				
			IniMom = LV.Vect();
			cout<<"KMKP Recon Mass = "<<LV.Mag()<<endl;
			cout<<Form("KMKP Mom = (%f,%f,%f)",IniMom.X(),IniMom.Y(),IniMom.Z())<<endl;
			GetHelixParameter(Vert,IniMom,charge,par);
		}
		else{
			auto vert_ = verts.at(id);
			double x = vert_.X();	
			double y = vert_.Y();	
			double z = vert_.Z();	
			z=ZTarget;
			Vert= TVector3(x,y,z);
			auto LV = LVs.at(id);	
			IniMom = LV.Vect();
			GetHelixParameter(Vert,IniMom,charge,par);
			//		cout<<Form("Mom = (%f,%f,%f)",IniMom.X(),IniMom.Y(),IniMom.Z())<<endl;
		}
		auto momcal = CalcHelixMom(par,Vert.y());
		cout<<Form("MomRecal = (%f,%f,%f)",momcal.X(),momcal.Y(),momcal.Z())<<endl;
	}
}
Recon::Recon(Recon P,Recon Q,double m1,double m2){
	// Q + L = P -> L = P - Q;
	auto PPar =	P.GetPar();
	auto QPar =	Q.GetPar();
	double cd,t1,t2;
	auto vp = P.Vertex();
	auto pp = CalcHelixMom(PPar,vp.y());
	auto vq = Q.Vertex();
	auto pq = CalcHelixMom(QPar,vq.y());
		
//	cout<<Form("XiStarVertex (%f,%f,%f)",vp.x(),vp.y(),vp.z())<<endl;
//	cout<<Form("MomRecal = (%f,%f,%f),%f",pp.X(),pp.Y(),pp.Z(),pp.Mag())<<endl;

//	Vert  = VertexPointHelix(PPar,QPar,cd,t1,t2);
//	Vert = (vp+vq)*0.5;
		Vert = vp;
//	Vert = VertexPoint(vp,vq,vp,vq);
//	cout<<"Pi0CD = "<<(Vert-P.Vertex()).Mag()<<endl;
	cout<<"Pi0CD = "<<cd<<endl;
	auto Pp = P.GetCharge()*CalcHelixMom(PPar,Vert.y());
	auto Qp = Q.GetCharge()*CalcHelixMom(QPar,Vert.y());
	cout<<Form("MomP = (%f,%f,%f),%f, MomQ =(%f,%f,%f),%f,cos = %f,dif = %f",Pp.X(),Pp.Y(),Pp.Z(),Pp.Mag(),Qp.X(),Qp.Y(),Qp.Z(),Qp.Mag(),Pp*Qp/(Pp.Mag()*Qp.Mag()),(Pp-Qp).Mag())<<endl;
//	TLorentzVector LVP(Pp, sqrt(Pp.Mag2()+m1*m1));	
	auto LVP = Pp.GetLV();
	TLorentzVector LVQ(Qp, sqrt(Qp.Mag2()+m2*m2));	
	cout<<Form("EP = %f, EQ = %f, PPi0= %f GeV/c",LVP.T(),LVQ.T(),sqrt((LVP.T()-LVQ.T())*(LVP.T()-LVQ.T())-mpi0*mpi0))<<endl;
	LV = LVP - LVQ;
	double PM = Pp.Mag(),QM= Qp.Mag();
	double angle_raw = acos( Pp*Qp/PM/QM )*180./acos(-1);
	double cos = (mpi0*mpi0-LV.T()*LV.T()+PM*PM+QM*QM)   / (2*PM*QM);
	double angle = acos(cos)*180./acos(-1);
	cout<<Form("Angle raw , real = (%f,%f),cos = %f",angle_raw,angle,cos)<<endl;

	auto Mom = LV.Vect();
	cout<<Form("Pi0Vert = (%f,%f,%f)",Vert.x(),Vert.y(),Vert.z())<<endl;
	cout<<Form("Pi0Mom = (%f,%f,%f)",Mom.x(),Mom.y(),Mom.z())<<endl;
	cout<<Form("Pi0Mass = %f",LV.Mag())<<endl;
	exist = true;
	auto V_t = GlobalToTarget(Vert);
	auto mom = LV.Vect();
	auto dir_t = GlobalToTargetMom(mom);
	dir_t = dir_t* (1/dir_t.Y());
	auto u = dir_t.X();
	auto v = dir_t.Z();
	par[0]=V_t.X()-V_t.Z()*u,par[1]=V_t.Z()-V_t.Y()*v,par[2]= u,par[3]=v;
	if(charge){
		GetHelixParameter(Vert,mom,charge,par);
	}
}
#endif
