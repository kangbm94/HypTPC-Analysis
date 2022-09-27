// -*- C++ -*-

#ifndef TPC_PAD_HELPER_HH
#define TPC_PAD_HELPER_HH

#include "DetectorID.hh"
static const int max_padid = 5768;
namespace tpc
{
	enum EPadParameter
	{
		kLayerID,
		kNumOfPad,
		kRadius,
		kNumOfDivision,
		kDummy,
		kLength,
		NPadParameter
	};

	//_____________________________________________________________________________
	//#OfPad #division #radius padLength
	static const Double_t padParameter[NumOfLayersTPC][NPadParameter] =
	{{0, 48,    14.75, 48, 0,  9.},
		{1, 48,    24.25, 48, 0,  9.},
		{2, 72,    33.75, 72, 0,  9.},
		{3, 96,    43.25, 96, 0,  9.},
		{4, 120,    52.75,120,0,   9.},
		{5, 144,    62.25,144,0,   9.},
		{6, 168,    71.75,168,0,   9.},
		{7, 192,    81.25,192,0,   9.},
		{8, 216,    90.75,216,0,   9.},
		{9, 240,    100.25,240,0,  9.},
		{10,208,    111.5,241, 0,  12.5},
		{11,218,    124.5,271, 0,  12.5},
		{12,230,    137.5,300, 0,  12.5},
		{13,214,    150.5,330, 0,  12.5},
		{14,212,    163.5,360, 0,  12.5},
		{15,214,    176.5,390, 0,  12.5},
		{16,220,    189.5,420, 0,  12.5},
		{17,224,    202.5,449, 0,  12.5},
		{18,232,    215.5,479, 0,  12.5},
		{19,238,    228.5,509, 0,  12.5},
		{20,244,    241.5,539, 0,  12.5},
		{21,232,    254.5,569, 0,  12.5},
		{22,218,    267.5,599, 0,  12.5},
		{23,210,    280.5,628, 0,  12.5},
		{24,206,    293.5,658, 0,  12.5},
		{25,202,    306.5,688, 0,  12.5},
		{26,200,    319.5,718, 0,  12.5},
		{27,196,    332.5,748, 0,  12.5},
		{28,178,    345.5,777, 0,  12.5},
		{29,130,    358.5,807, 0,  12.5},
		{30,108,    371.5,837, 0,  12.5},
		{31,90,     384.5,867, 0, 12.5}};

	//_____________________________________________________________________________
	inline Int_t GetCoBoId(Int_t layer, Int_t row)
	{
		switch(layer){
			case 4:
				if(60<=row && row<=99)
					return 1;
				else
					return 0;
			case 5:
				if(72<=row && row<=119)
					return 1;
				else
					return 0;
			default:
				return layer/4;
		}
	}

	//_____________________________________________________________________________
	inline Int_t GetPadId(Int_t layerID, Int_t rowID)
	{
		//Original
		//Int_t padID=0;
		// Check!!!!!!!
		Int_t padID=1;
		for(int layi = 0 ; layi<layerID; layi++) padID += padParameter[layi][1];
		padID+=rowID;
		return padID;

	}

	inline Int_t getLayerID(Int_t padID)
	{
		padID-=1;
		int layer;
		int sum = 0;

		for (layer = 0; layer <= 30 && sum + padParameter[layer][1] <= padID; layer++)
		{
			sum += padParameter[layer][1];
		}
		return layer;
	}

	inline Int_t getRowID(Int_t padID)
	{
		padID-=1;
		int layer, row;
		int sum = 0;

		for (layer = 0; layer <= 30 && sum + padParameter[layer][1] <= padID; layer++)
		{
			sum += padParameter[layer][1];
		}
		row = padID - sum;
		return row;
	}
	/*
		 Double_t getTheta(Int_t layerID, Int_t rowID)
		 {
		 Double_t sTheta = 180.-(360./padParameter[layerID][3])*padParameter[layerID][1]/2.;
		 Double_t theta = sTheta+(rowID+0.5)*(360.-2*sTheta)/padParameter[layerID][1];
		 return theta;
		 }
		 */

	inline Double_t getTheta(Int_t padID)
	{
		padID-=1;
		int layer, row;
		int sum = 0;

		for (layer = 0; layer <= 30 && sum + padParameter[layer][1] <= padID; layer++)
		{
			sum += padParameter[layer][1];
		}
		row = padID - sum;
		//std::cout<<"layer="<<layer<<", row="<<row<<std::endl;
		Double_t sTheta = 180.-(360./padParameter[layer][3])*padParameter[layer][1]/2.;
		//Double_t theta = sTheta+(row+0.5)*(360.-2*sTheta)/padParameter[layer][1];
		Double_t theta = sTheta+(row+0.5)*360./padParameter[layer][3]-180;
		//std::cout<<"theta="<<theta<<std::endl;

		return theta;
	}

	inline Double_t getTheta(Int_t layer, Double_t m_row)
	{
		Double_t sTheta = 180.-(360./padParameter[layer][3])*padParameter[layer][1]/2.;
		//Double_t theta = sTheta+(row+0.5)*(360.-2*sTheta)/padParameter[layer][1];
		Double_t theta = sTheta+(m_row+0.5)*360./padParameter[layer][3]-180;
		//std::cout<<"theta="<<theta<<std::endl;
		return theta;
	}


	inline Double_t getR(Int_t padID)
	{
		padID-=1;
		int layer;
		int sum = 0;

		for (layer = 0; layer <= 30 && sum + padParameter[layer][1] <= padID; layer++)
		{
			sum += padParameter[layer][1];
		}
		Double_t R = padParameter[layer][2];
		return R;
	}

	inline TVector3 getPosition(int padID)
	{
		padID-=1;
		int layer, row;
		int sum = 0;

		for (layer = 0; layer <= 30 && sum + padParameter[layer][1] <= padID; layer++)
		{
			sum += padParameter[layer][1];
		}
		row = padID - sum;

		TVector3 result;
		if (row > padParameter[layer][1]){ // out of range
			result.SetX(0);
			result.SetY(-1);
			result.SetZ(0);
		}
		else{
			double x, z;
			//Double_t sTheta = 180.-(360./padParameter[layer][3])*padParameter[layer][1]/2.;

			//    x = padParameter[layer][2] * -sin((360./padParameter[layer][3])*TMath::Pi()/180. * (row + 0.5) + sTheta*TMath::Pi()/180.);
			//    z = padParameter[layer][2] * -cos((360./padParameter[layer][3])*TMath::Pi()/180. * (row + 0.5) + sTheta*TMath::Pi()/180.) - 143.0;

			// x = padParameter[layer][2] * sin(getTheta(padID+1)*TMath::Pi()/180.);
			// z = padParameter[layer][2] * cos(getTheta(padID+1)*TMath::Pi()/180.) - 143.0;
			//std::cout<<"layer="<<layer<<", row"<<row<<std::endl;
			x = padParameter[layer][2] * sin(getTheta(layer,row)*TMath::Pi()/180.);
			z = padParameter[layer][2] * cos(getTheta(layer,row)*TMath::Pi()/180.) - 143.0;

			// Double_t sTheta = 180.-(360./padParameter[layer][3])*padParameter[layer][1]/2.;
			// double x_ = padParameter[layer][2] * -sin((360./padParameter[layer][3])*TMath::Pi()/180. * (row + 0.5) + sTheta*TMath::Pi()/180.);
			// double z_ = padParameter[layer][2] * -cos((360./padParameter[layer][3])*TMath::Pi()/180. * (row + 0.5) + sTheta*TMath::Pi()/180.) - 143.0;
			//std::cout<<"x="<<x<<", z="<<z<<", x_="<<x_<<", z_="<<z_<<std::endl;
			result.SetX(x);
			result.SetY(0);
			result.SetZ(z);
		}
		return result;
	}

	inline TVector3 getPosition(int layer, double m_row)
	{
		TVector3 result;
		if (m_row > padParameter[layer][1]){ // out of range
			result.SetX(0);
			result.SetY(-1);
			result.SetZ(0);
		}
		else{
			double x, z;

			x = padParameter[layer][2] * sin(getTheta(layer, m_row)*TMath::Pi()/180.);
			z = padParameter[layer][2] * cos(getTheta(layer, m_row)*TMath::Pi()/180.) - 143.0;
			result.SetX(x);
			result.SetY(0);
			result.SetZ(z);
			//std::cout<<"x="<<x<<", z="<<z<<std::endl;
		}
		return result;
	}



	inline int findPadID(double z, double x)
	{
		z += 143;
		double radius = sqrt(x*x + z*z);
		double angle;
		if (z == 0)
		{
			if (x > 0)   angle = 1.5*TMath::Pi();
			else if (x < 0)   angle = 0.5*TMath::Pi();
			else return -1000; // no padID if (0,0)
		}
		//  else
		//  {
		//    if (z < 0 && x < 0) angle = atan(x / z);
		//    else if (z > 0 && x < 0) angle = TMath::Pi() - atan(-x / z);
		//    else if (z > 0 && x > 0) angle = TMath::Pi() + atan(x / z);
		//    else if (z < 0 && x > 0) angle = 2*TMath::Pi() - atan(-x / z);
		//  }
		else if (z > 0) angle = TMath::Pi()+atan(x / z);
		else if( z < 0&&x<0) angle = atan(x / z);
		else angle = 2*TMath::Pi()+ atan(x / z);
		//cout << " angle: " << angle*180/TMath::Pi() << endl;
		if(z<0&&x>0){
			//		cout<<Form("Angle: %f,  (%f , %f ) ",angle,z,x)<<endl;
		}
		int layer, row;
		// find layer_num.
		for (layer = 0; !(padParameter[layer][2]+padParameter[layer][5]*0.5 >= radius
					&& padParameter[layer][2]-padParameter[layer][5]*0.5 <= radius); layer++)
		{
			if (layer >= 32) return -1000;
			if (layer != 0)
			{
				if (padParameter[layer][2] - padParameter[layer][5] * 0.5 >= radius &&
						padParameter[layer - 1][2] + padParameter[layer - 1][5] * 0.5 <= radius) return -layer;
			}
		}

		//cout << " layer: " << layer << endl;

		Double_t sTheta = 180.-(360./padParameter[layer][3])*padParameter[layer][1]/2.;

		// find row_num
		if (angle - (sTheta*TMath::Pi()/180.) < 0)
		{
//			cout<<Form("Angle: %f,  (%f , %f ) ",angle,z,x)<<endl;
			return -1000;
		}

		//double a, b, c;
		row = (int)((angle-(sTheta*TMath::Pi()/180.))/(360./padParameter[layer][3]*TMath::Pi()/180.));
		if (row > padParameter[layer][1]) return -1000;

		//cout << " row: " << row << endl;
		//This is original one
		//return GetPadId(layer, row)+1;
		//Please check
		return GetPadId(layer, row);
	}

	//_____________________________________________________________________________
	inline
		TH2Poly* InitializeHistograms()
		{
			/*
				 std::vector<TH2Poly*> target;
				 TList* list = gDirectory->GetList();
				 TIter itr(list);
				 while(itr.Next()){
				 const TString& name((*itr)->GetName());
				 const TString& cname((*itr)->ClassName());
			//hddaq::cout << " " << std::setw(8) << std::left << name
			std::cout << " " << std::setw(8) << std::left << name
			<< "(" << cname << ")" << std::endl;
			if(cname.EqualTo("TH2Poly")){
			target.push_back(dynamic_cast<TH2Poly*>(*itr));
			}
			}
			*/
			Double_t X[5];
			Double_t Y[5];
			TH2Poly* h = new TH2Poly();
			for(Int_t layer=0; layer<NumOfLayersTPC; ++layer){
				Double_t pLength = tpc::padParameter[layer][5];
				Double_t st = 180.-(360./tpc::padParameter[layer][3])
					* tpc::padParameter[layer][1]/2.;
				Double_t sTheta  = (-1+st/180.)*TMath::Pi();
				Double_t dTheta  = (360./tpc::padParameter[layer][3])/180.*TMath::Pi();
				Double_t cRad    = tpc::padParameter[layer][2];
				Int_t    nPad    = tpc::padParameter[layer][1];
				for(Int_t j=0; j<nPad; ++j){
					X[1] = (cRad+(pLength/2.))*TMath::Cos(j*dTheta+sTheta);
					X[2] = (cRad+(pLength/2.))*TMath::Cos((j+1)*dTheta+sTheta);
					X[3] = (cRad-(pLength/2.))*TMath::Cos((j+1)*dTheta+sTheta);
					X[4] = (cRad-(pLength/2.))*TMath::Cos(j*dTheta+sTheta);
					X[0] = X[4];
					Y[1] = (cRad+(pLength/2.))*TMath::Sin(j*dTheta+sTheta);
					Y[2] = (cRad+(pLength/2.))*TMath::Sin((j+1)*dTheta+sTheta);
					Y[3] = (cRad-(pLength/2.))*TMath::Sin((j+1)*dTheta+sTheta);
					Y[4] = (cRad-(pLength/2.))*TMath::Sin(j*dTheta+sTheta);
					Y[0] = Y[4];
					for(Int_t ii=0; ii<5; ++ii) X[ii] -=143;
					//      for(auto& h: target) h->AddBin(5, X, Y);
					h->AddBin(5, X, Y);
				}
			}
			return h;
		}
}
void GetLayerRow(int padId, int &layer, int &row){
	layer=tpc::getLayerID(padId);
	row=tpc::getRowID(padId);
};
#endif
