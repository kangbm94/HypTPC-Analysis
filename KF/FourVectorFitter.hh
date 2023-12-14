#ifndef FourVectorFitter_h
#define FourVectorFitter_h
#include "KinFit2.hh"
class FourVectorFitter: virtual public KinematicFitter{
	//R -> P + Q;
	protected:	
		TLorentzVector P;
		TVector3 Pres;
		TLorentzVector PCor;
		double mP;

		TLorentzVector Q;
		TVector3 Qres;
		TLorentzVector QCor;
		double mQ;

		TLorentzVector R;
		TVector3 Rres;
		TLorentzVector RCor;
		vector<double> MassDiffs;
		double mR;
		bool MeasDir = false;
	public:
		FourVectorFitter(){}
		FourVectorFitter(TLorentzVector P_,TLorentzVector Q_,TLorentzVector R_);
		void SetInvMass(double IM){
			mR = IM;
		}
		vector<TLorentzVector> GetFittedLV(){
			vector<TLorentzVector> ret = {PCor,QCor,RCor};
			return ret;
		}
		void UseVertex(bool status,TVector3 Vert1,TVector3 Vert2);
		void ToDecayPlane();
	protected:
		virtual void Initialize();
		virtual void SampleStepPoint(int steps);
		virtual void SetConstraints();
		virtual void Rotate();
		
};
#endif
