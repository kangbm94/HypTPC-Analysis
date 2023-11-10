
double ResP = 0.04;
double ResPi1 = 0.03;
double ResPi2 = 0.03;
double ResTh = 0.01; 
double ResPh = 0.01; 
double ResThV = 0.05; 
double ResPhV = 0.05; 

double PLd,ThLd,PhLd; 
double PP,ThP,PhP; 
double PPi1,ThPi1,PhPi1; 
double PPi2,ThPi2,PhPi2; 
double PLdMeas,ThLdMeas,PhLdMeas; 
double PPMeas,ThPMeas,PhPMeas; 
double PPi1Meas,ThPi1Meas,PhPi1Meas; 
double PPi2Meas,ThPi2Meas,PhPi2Meas; 
double PLdCor,ThLdCor,PhLdCor; 
double PPCor,ThPCor,PhPCor; 
double PPi1Cor,ThPi1Cor,PhPi1Cor; 
double PPi2Cor,ThPi2Cor,PhPi2Cor; 
double InvMLd,InvMXi,InvMLdCor,InvMXiCor;
double Chi2;
int NStep,BestStep;
int Step[200];
double StepChi2[200];
double StepMassDiff[200];
int iev;
void SetBranches(TTree* tree){
	tree->Branch("iev",&iev);
	tree->Branch("PLd",&PLd);
	tree->Branch("ThLd",&ThLd);
	tree->Branch("PhLd",&PhLd);
	tree->Branch("PP",&PP);
	tree->Branch("ThP",&ThP);
	tree->Branch("PhP",&PhP);
	tree->Branch("PPi1",&PPi1);
	tree->Branch("ThPi1",&ThPi1);
	tree->Branch("PhPi1",&PhPi1);
	tree->Branch("PPi2",&PPi2);
	tree->Branch("ThPi2",&ThPi2);
	tree->Branch("PhPi2",&PhPi2);
	
	tree->Branch("InvMLd",&InvMLd);	
	tree->Branch("InvMXi",&InvMXi);	
	tree->Branch("PLdMeas",&PLdMeas);
	tree->Branch("ThLdMeas",&ThLdMeas);
	tree->Branch("PhLdMeas",&PhLdMeas);
	tree->Branch("PPMeas",&PPMeas);
	tree->Branch("ThPMeas",&ThPMeas);
	tree->Branch("PhPMeas",&PhPMeas);
	tree->Branch("PPi1Meas",&PPi1Meas);
	tree->Branch("ThPi1Meas",&ThPi1Meas);
	tree->Branch("PhPi1Meas",&PhPi1Meas);
	tree->Branch("PPi2Meas",&PPi2Meas);
	tree->Branch("ThPi2Meas",&ThPi2Meas);
	tree->Branch("PhPi2Meas",&PhPi2Meas);
	
	tree->Branch("InvMLdCor",&InvMLdCor);	
	tree->Branch("InvMXiCor",&InvMXiCor);	
	tree->Branch("PLdCor",&PLdCor);
	tree->Branch("ThLdCor",&ThLdCor);
	tree->Branch("PhLdCor",&PhLdCor);
	tree->Branch("PPCor",&PPCor);
	tree->Branch("ThPCor",&ThPCor);
	tree->Branch("PhPCor",&PhPCor);
	tree->Branch("PPi1Cor",&PPi1Cor);
	tree->Branch("ThPi1Cor",&ThPi1Cor);
	tree->Branch("PhPi1Cor",&PhPi1Cor);
	tree->Branch("PPi2Cor",&PPi2Cor);
	tree->Branch("ThPi2Cor",&ThPi2Cor);
	tree->Branch("PhPi2Cor",&PhPi2Cor);

	tree->Branch("NStep",&NStep);
	tree->Branch("BestStep",&BestStep);
	tree->Branch("Chi2",&Chi2);
	tree->Branch("Step",Step,"Step[NStep]/I");
	tree->Branch("StepChi2",StepChi2,"StepChi2[NStep]/D");
	tree->Branch("StepMassDiff",StepMassDiff,"StepMassDiff[NStep]/D");

}
