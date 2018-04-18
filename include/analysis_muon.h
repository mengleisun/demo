#ifndef ANALYSIS_MUON
#define ANALYSIS_MUON

#ifndef ANALYSIS_RAWDATA
#include "analysis_rawData.h"
#endif

#ifndef ROOT_TLorentzVector
#include "TLorentzVector.h"
#endif

class recoMuon{
  public:
  recoMuon(rawData& raw, int imu){
    runtype_ = raw.runtype_;

    p4_.SetPtEtaPhiE((*raw.muPt)[imu],(*raw.muEta)[imu],(*raw.muPhi)[imu],(*raw.muEn)[imu]);

    muCharge_ = (*raw.muCharge)[imu]; 
    muType_ = (*raw.muType)[imu];
    muIDbit_ = (*raw.muIDbit)[imu];
    muPFMiniIso_ = (*raw.muPFMiniIso)[imu];
    muPFChIso_ = (*raw.muPFChIso)[imu];
    muPFPhoIso_ = (*raw.muPFPhoIso)[imu];
    muPFNeuIso_ = (*raw.muPFNeuIso)[imu];
    muPFPUIso_ = (*raw.muPFPUIso)[imu];
		muD0_ = (*raw.muD0)[imu];
		muDz_ = (*raw.muDz)[imu];
    muFiredTrgs_ = (*raw.muFiredTrgs)[imu];
    muFiredL1Trgs_ = (*raw.muFiredL1Trgs)[imu]; 
    muCombIso_ = (muPFChIso_ + std::max(0.0, muPFNeuIso_+muPFPhoIso_- 0.5*muPFPUIso_))/(*raw.muPt)[imu];
 }

  ~recoMuon(){
  };
   
inline float getE() { return p4_.E();}
inline float getEt() { return p4_.Et();}
inline double getPt() {return p4_.Pt();}
inline float getEta() { return p4_.Eta();}
inline float getPhi() {return p4_.Phi();}
inline TLorentzVector getP4() {
         TLorentzVector p = p4_;
          return p;
}
inline float getMiniIso() {return muPFMiniIso_;}
inline float getRelIso(){return muCombIso_;}
inline float getD0() {return muD0_;}
inline float getDz() {return muDz_;}

inline bool fireSingleTrg(int itrg) {
            if(((muFiredTrgs_ >> itrg)&1) == 1)return true;
            else{ return false;}
}

inline bool fireL1Trg(int itrg) {
            if(((muFiredL1Trgs_ >> itrg)&1) == 1)return true;
            else{ return false;}
}

inline bool isPosi(){
  return (muCharge_ > 0);
}

inline bool isLoose() {
   if(((muIDbit_ >> 0)&1) == 1)return true;
   else return false;
}
inline bool isMedium() {
   if(((muIDbit_ >> 1)&1) == 1)return true;
   else return false;
}

inline bool isTight() {
   if(((muIDbit_ >> 2)&1) == 1)return true;
   else return false;
}

bool passSignalSelection();
bool passHLTSelection();
bool isFakeProxy();

private:
    RunType runtype_;

    TLorentzVector p4_;
    int    muCharge_; 
    int    muType_;
    UShort_t muIDbit_;
    float  muPFMiniIso_;
    float  muPFChIso_;
    float  muPFPhoIso_;
    float  muPFNeuIso_;
    float  muPFPUIso_;
    float  muCombIso_;
    int    muFiredTrgs_;
    int    muFiredL1Trgs_;
		float  muD0_;
		float  muDz_;
};
  
#endif 
