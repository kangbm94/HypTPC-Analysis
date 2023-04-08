#ifndef KinFit_h
#define KinFit_h
namespace{
	double gpar[15]={0};
	void fcn(int& npar, double* grad, double& fval, double* par, int flag){
		//Kinematic fitting of two particles, L ->  P + Q with invariant mass of InvM.

		double lambda = par[0];//Lagrange Multiplier
		double px = par[1];// Corrected X momentum of P
		double py = par[2];
		double pz = par[3];
		double qx = par[4];// Corrected X momemtum of Q
		double qy = par[5];
		double qz = par[6];

		double px0 = gpar[0];// Measured X momentum of P 
		double py0 = gpar[1];
		double pz0 = gpar[2];
		double qx0 = gpar[3];
		double qy0 = gpar[4];
		double qz0 = gpar[5];

		double psx = gpar[6];//sigma of px
		double psy = gpar[7];
		double psz = gpar[8];
		double qsx = gpar[9];
		double qsy = gpar[10];
		double qsz = gpar[11];

		double MP = gpar[12];
		double MQ = gpar[13];
		double InvM = gpar[14];
		double P_E = sqrt(MP*MP+px*px+py*py+pz*pz); 
		double Q_E = sqrt(MQ*MQ+qx*qx+qy*qy+qz*qz); 
		double L_E = P_E+Q_E;
		double L_M = sqrt(L_E*L_E -((px+qx)*(px+qx)+(py+qy)*(py+qy)+(pz+qz)*(pz+qz)));

		double eq_px = px0 - px - psx*psx*lambda*(-qx+Q_E/P_E*px)/InvM; 
		double eq_py = py0 - py - psy*psy*lambda*(-qy+Q_E/P_E*py)/InvM; 
		double eq_pz = pz0 - pz - psz*psz*lambda*(-qz+Q_E/P_E*pz)/InvM; 

		double eq_qx = qx0 - qx - qsx*qsx*lambda*(-px+P_E/Q_E*qx)/InvM; 
		double eq_qy = qy0 - qy - qsy*qsy*lambda*(-py+P_E/Q_E*qy)/InvM; 
		double eq_qz = qz0 - qz - qsz*qsz*lambda*(-pz+P_E/Q_E*qz)/InvM; 

		double eq_IM = (L_M - InvM);

		fval = eq_px*eq_px + eq_py*eq_py + eq_pz*eq_pz
			+ eq_qx*eq_qx + eq_qy*eq_qy + eq_qz*eq_qz
			+ eq_IM*eq_IM;
	}
}
class KinematicFitter{
	private:
		double InvMass;
		
		TLorentzVector P;
		TVector3 Pres;
		TLorentzVector PCor;

		TLorentzVector Q;
		TVector3 Qres;
		TLorentzVector QCor;

//		TMinuitMinimizer minimizer;
		TMinuit* minimizer = new TMinuit(7);
	public:
		KinematicFitter(){}
		KinematicFitter(double Mass){
			InvMass = Mass;
		}
		void AssignLorentzVector(TLorentzVector P_,TLorentzVector Q_);
		void SetResolution(TVector3 Pres_,TVector3 Qres_);
		void DoKinematicFit();
		vector<TLorentzVector> GetFittedLV();
};
#endif
