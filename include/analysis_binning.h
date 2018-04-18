#ifndef ANALYSIS_BINNING
#define ANALYSIS_BINNING

#ifndef ROOT_TFile
#include "TFile.h"
#endif
#include<iostream>

  class binning{
  public:
    binning(int NBIN, float METbin1, float METbin2, float HTbin1, float HTbin2, float PHOETbin):
			NBIN_(NBIN),
			METbin1_(METbin1), 
			METbin2_(METbin2), 
			HTbin1_(HTbin1),
			HTbin2_(HTbin2),
			PHOETbin_(PHOETbin)
  	{
		}

	~binning(){
	};

	inline int findSignalBin(float MET, float HT, float Et){

		int SigBinIndex(-1);
		int halfbin = NBIN_/2;
		if(NBIN_ > 20)halfbin = NBIN_/3;
		if(HTbin1_ < HTbin2_-10){

			if(MET > 120 && MET <= METbin1_){
				if(HT  < HTbin1_)SigBinIndex = 0;
				else if(HT  > HTbin1_ && HT < HTbin2_)SigBinIndex = 1;
				else if(HT >= HTbin2_)SigBinIndex = 2; 
			}
			else if(MET > METbin1_ && MET <= METbin2_){
				if(HT  < HTbin1_)SigBinIndex = 3;
				else if(HT  > HTbin1_ && HT < HTbin2_)SigBinIndex = 4; 
				else if(HT >= HTbin2_)SigBinIndex = 5;
			}
			else if(MET > METbin2_){ 
				if(HT  < HTbin1_)SigBinIndex = 6;
				else if(HT  > HTbin1_ && HT < HTbin2_)SigBinIndex = 7; 
				else if(HT >= HTbin2_)SigBinIndex = 8;
			}
		}
		else{

			if(MET > 120 && MET <= METbin1_){
				if(HT  < HTbin2_)SigBinIndex = 0;
				else if(HT >= HTbin2_)SigBinIndex = 1; 
			}
			else if(MET > METbin1_ && MET <= METbin2_){
				if(HT  < HTbin2_)SigBinIndex = 2;
				else if(HT >= HTbin2_)SigBinIndex = 3;
			}
			else if(MET > METbin2_){ 
				if(HT  < HTbin2_)SigBinIndex = 4;
				else if(HT >= HTbin2_)SigBinIndex = 5;
			}
		}
		if(SigBinIndex >= 0 && PHOETbin_ > 10){
			if(Et > PHOETbin_)SigBinIndex += halfbin;
		}
		return SigBinIndex;
	}
	
//	inline int findSignalBin(float MET, float HT, float Et){
//
//		int SigBinIndex(-1);
//			if(MET > 120 && MET <= METbin1_){
//				if(HT  < HTbin1_)SigBinIndex = 0;
//				else if(HT  > HTbin1_ && HT < HTbin2_)SigBinIndex = 1;
//				else if(HT >= HTbin2_)SigBinIndex = 2; 
//			}
//			else if(MET > METbin1_ && MET <= METbin2_){
//				if(HT  < HTbin1_)SigBinIndex = 3;
//				else if(HT  > HTbin1_ && HT < HTbin2_)SigBinIndex = 4; 
//				else if(HT >= HTbin2_)SigBinIndex = 5;
//			}
//			else if(MET > METbin2_ && MET < 600){ 
//				if(HT  < HTbin1_)SigBinIndex = 6;
//				else if(HT  > HTbin1_ && HT < HTbin2_)SigBinIndex = 7; 
//				else if(HT >= HTbin2_)SigBinIndex = 8;
//			}
//			else if(MET >= 600){
//				if(HT  < HTbin2_)SigBinIndex = 9;
//				else if(HT >= HTbin2_)SigBinIndex = 10;	
//			}
//		return SigBinIndex;
//	}
	private:
		int	NBIN_;
		float	METbin1_;
		float	METbin2_;
		float	HTbin1_;
		float	HTbin2_;
		float	PHOETbin_;
	};

#endif

