#include "TLorentzVector.h"


  class recoJet{
  public:
  recoJet(rawData& raw, int ij){
    p4_.SetPtEtaPhiE((*raw.jetPt)[ij],(*raw.jetEta)[ij],(*raw.jetPhi)[ij],(*raw.jetEn)[ij]);
		//jetID_ = (*raw.jetID)[ij];
    jetArea_ = (*raw.jetArea)[ij];
		jetJECUnc_ = (*raw.jetJECUnc)[ij];
		jetCSV2BJetTags_ = (*raw.jetCSV2BJetTags)[ij];
  }

  ~recoJet(){
  };
   
inline float getE() { return p4_.E();}
inline float getEt() { return p4_.Et();}
inline float getPt() { return p4_.Pt();}
inline float getEta() { return p4_.Eta();}
inline float getPhi() {return p4_.Phi();}
inline TLorentzVector getP4() {
         TLorentzVector p = p4_;
          return p;
}
inline float getArea() { return jetArea_;}
inline float getPtUnc(){ return jetJECUnc_;}
inline bool  isBJet()  {
	if(jetCSV2BJetTags_ > 0.8484)return true;
	else return false;
}
inline bool isLoose() { 
	if(jetID_ > 0)return true;
	else return false;
}

inline bool passSignalSelection(){
	bool pass(true);
	if(getPt() < 30.0)pass = false;
	if(fabs(getEta()) > 2.5)pass = false;
	return pass;
}

private:
    TLorentzVector p4_;
		int   jetID_;
    float jetArea_;
		float jetJECUnc_;
		float jetCSV2BJetTags_;
};
   
