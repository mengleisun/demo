#ifndef ANALYSIS_ELE
#define ANALYSIS_ELE

#ifndef ANALYSIS_RAWDATA
#include "analysis_rawData.h"
#endif

#ifndef ROOT_TLorentzVector
#include "TLorentzVector.h"
#endif

#include<iostream>


  class recoEle{
  public:
  recoEle(rawData& raw, int iele){
    runtype_ = raw.runtype_;

    p4_.SetPtEtaPhiE((*raw.elePt)[iele],(*raw.eleSCEta)[iele],(*raw.elePhi)[iele],(*raw.eleEn)[iele]);
    Calibp4_.SetPtEtaPhiE((*raw.eleCalibPt)[iele],(*raw.eleSCEta)[iele],(*raw.elePhi)[iele],(*raw.eleCalibEn)[iele]);

    eleCharge_ = (*raw.eleCharge)[iele];
    eleD0_ = (*raw.eleD0)[iele];
    eleDz_ = (*raw.eleDz)[iele];
    eleR9_ = (*raw.eleR9)[iele];
    eleHoverE_ = (*raw.eleHoverE)[iele];
    eleEoverPInv_ = (*raw.eleEoverPInv)[iele];
    eleSigmaIEtaIEta_ = (*raw.eleSigmaIEtaIEta)[iele];
    eleConvVeto_ = (*raw.eleConvVeto)[iele];
    eleMissHits_ = (*raw.eleMissHits)[iele];
    elePFChIso_ = (*raw.elePFChIso)[iele];
    elePFPhoIso_ = (*raw.elePFPhoIso)[iele];
    elePFNeuIso_ = (*raw.elePFNeuIso)[iele];
    elePFPUIso_ = (*raw.elePFPUIso)[iele];
    elePFMiniIso_ = (*raw.elePFMiniIso)[iele];

    float eta = fabs((*raw.eleSCEta)[iele]);
    float rho = raw.rho;
    if(eta<=1.0){
      eleCombIso_ = (elePFChIso_ + std::max(0.0, elePFNeuIso_+elePFPhoIso_-rho*0.1703))/(*raw.elePt)[iele];
      eleCalibCombIso_ = (elePFChIso_ + std::max(0.0, elePFNeuIso_+elePFPhoIso_-rho*0.1703))/(*raw.eleCalibPt)[iele];
    }
    else if(eta>1.0 && eta<= 1.479){
      eleCombIso_ = (elePFChIso_ + std::max(0.0, elePFNeuIso_+elePFPhoIso_-rho*0.1715))/(*raw.elePt)[iele];
      eleCalibCombIso_ = (elePFChIso_ + std::max(0.0, elePFNeuIso_+elePFPhoIso_-rho*0.1715))/(*raw.eleCalibPt)[iele];
    }
    else if(eta>1.479 && eta<=2.0){
      eleCombIso_ = (elePFChIso_ + std::max(0.0, elePFNeuIso_+elePFPhoIso_-rho*0.1213))/(*raw.elePt)[iele];
      eleCalibCombIso_ = (elePFChIso_ + std::max(0.0, elePFNeuIso_+elePFPhoIso_-rho*0.1213))/(*raw.eleCalibPt)[iele];
    }
    else if(eta>2.0 && eta<=2.2){
      eleCombIso_ = (elePFChIso_ + std::max(0.0, elePFNeuIso_+elePFPhoIso_-rho*0.1230))/(*raw.elePt)[iele];
      eleCalibCombIso_ = (elePFChIso_ + std::max(0.0, elePFNeuIso_+elePFPhoIso_-rho*0.1230))/(*raw.eleCalibPt)[iele];
    } 
    else if(eta>2.2 && eta<=2.3){
      eleCombIso_ = (elePFChIso_ + std::max(0.0, elePFNeuIso_+elePFPhoIso_-rho*0.1635))/(*raw.elePt)[iele];
      eleCalibCombIso_ = (elePFChIso_ + std::max(0.0, elePFNeuIso_+elePFPhoIso_-rho*0.1635))/(*raw.eleCalibPt)[iele];
    } 
    else if(eta>2.3 && eta<=2.4){
      eleCombIso_ = (elePFChIso_ + std::max(0.0, elePFNeuIso_+elePFPhoIso_-rho*0.1937))/(*raw.elePt)[iele];
      eleCalibCombIso_ = (elePFChIso_ + std::max(0.0, elePFNeuIso_+elePFPhoIso_-rho*0.1937))/(*raw.eleCalibPt)[iele];
    } 
    else if(eta>2.4 && eta<=2.5){
      eleCombIso_ = (elePFChIso_ + std::max(0.0, elePFNeuIso_+elePFPhoIso_-rho*0.2393))/(*raw.elePt)[iele];
      eleCalibCombIso_ = (elePFChIso_ + std::max(0.0, elePFNeuIso_+elePFPhoIso_-rho*0.2393))/(*raw.eleCalibPt)[iele];
    } 
    else{
      eleCombIso_ = (elePFChIso_ + elePFNeuIso_+elePFPhoIso_)/(*raw.elePt)[iele];
      eleCalibCombIso_ = (elePFChIso_ + elePFNeuIso_+elePFPhoIso_)/(*raw.eleCalibPt)[iele];
    } 

    eledEtaAtVtx_ = (*raw.eledEtaAtVtx)[iele];
    eledPhiAtVtx_ = (*raw.eledPhiAtVtx)[iele];
    eleTrkdxy_ = (*raw.eleTrkdxy)[iele];
	eleFiredSingleTrgs_ = (*raw.eleFiredSingleTrgs)[iele];
	eleFiredDoubleTrgs_ = (*raw.eleFiredDoubleTrgs)[iele];
	eleFiredL1Trgs_ = (*raw.eleFiredL1Trgs)[iele];
    eleIDbit_ = (*raw.eleIDbit)[iele];

 }

  ~recoEle(){
  };
   
inline float getE() { return p4_.E();}
inline double getEt() { return p4_.Et();}
inline double getCalibEt() { return Calibp4_.Et();}
inline float getEta() { return p4_.Eta();}
inline float getPhi() {return p4_.Phi();}
inline double getPt() {return p4_.Pt();}
inline double getCalibPt() {return Calibp4_.Pt();}
inline TLorentzVector getP4() {
         TLorentzVector p = p4_;
          return p;
}
inline TLorentzVector getCalibP4() {
         TLorentzVector p = Calibp4_;
          return p;
}
inline float getChIso() {return elePFChIso_;}
inline float getNeuIso() {return elePFNeuIso_;}
inline float getPhoIso() {return elePFPhoIso_;}
inline float getPFPUIso(){return elePFPUIso_;}
inline float getMiniIso(){return elePFMiniIso_;}
inline float getRelIso() {return eleCombIso_;}
inline float getCalibRelIso() {return eleCalibCombIso_;}
inline bool fireTrgs(int itrg) {
            if(((eleFiredSingleTrgs_ >> itrg)&1) == 1)return true;
            else{ return false;}
}


inline int getCharge() {return eleCharge_;} 
inline float getD0() {return eleD0_;}
inline float getDz() {return eleDz_;}
inline float getR9() {return eleR9_;}
inline float getHoverE() {return eleHoverE_;}
inline float getEoverPInv() {return eleEoverPInv_;}
inline float getSigma() {return eleSigmaIEtaIEta_;}
inline int  getConvVeto() {return eleConvVeto_;}
inline int  getMissHits() {return eleMissHits_;}
inline float getdEtaIn() {return eledEtaAtVtx_;}
inline float getdPhiIn() {return eledPhiAtVtx_;}

inline bool isLoose() {
   if((( eleIDbit_ >> 1)&1)==1)return true;
   else return false;
}
inline bool isMedium() {
   if((( eleIDbit_ >> 2)&1)==1)return true;
   else return false;
}

inline bool isTight() {
   if((( eleIDbit_ >> 3)&1)==1)return true;
   else return false;
}

inline bool isEB(){
     if(fabs(p4_.Eta())<1.4442)return true;
     else return false;
}

inline bool isEE(){
     if(fabs(p4_.Eta())>1.566 && fabs(p4_.Eta()) < 2.5)return true;
     else return false;
}

bool passSignalSelection();
bool passHLTSelection();
bool isMiniMedium();
bool isMiniLoose();
bool passBasicID();
bool isFakeProxy();
bool isLooseFakeProxy();

private:
    RunType runtype_;

    TLorentzVector p4_;
    TLorentzVector Calibp4_;
    int    eleCharge_;
    float  eleEn_;
    float  eleD0_;
    float  eleDz_;
    float  elePt_;
    float  eleCalibPt_;
    float  eleCalibEn_;
    float  eleEta_;
    float  elePhi_;
    float  eleR9_;
    float  eleHoverE_;
    float  eleEoverPInv_;
    float  eleSigmaIEtaIEta_;
    int    eleConvVeto_;
    int    eleMissHits_;
    float  elePFChIso_;
    float  elePFPhoIso_;
    float  elePFNeuIso_;
    float  elePFPUIso_;
    float  elePFMiniIso_;
    float  eleCombIso_;
    float  eleCalibCombIso_;
    float  eledEtaAtVtx_;
    float  eledPhiAtVtx_;
    float  eleTrkdxy_;
    Int_t  eleFiredSingleTrgs_;
    Int_t  eleFiredDoubleTrgs_;
    Int_t  eleFiredL1Trgs_;

    UShort_t eleIDbit_;
};
  

#endif 
