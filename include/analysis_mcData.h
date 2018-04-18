#include "TLorentzVector.h"


  class mcData{
  public:
  mcData(rawData& raw, int imc){
    p4_.SetPtEtaPhiM((*raw.mcPt)[imc],(*raw.mcEta)[imc],(*raw.mcPhi)[imc],(*raw.mcMass)[imc]);
    momp4_.SetPtEtaPhiM((*raw.mcMomPt)[imc],(*raw.mcMomEta)[imc],(*raw.mcMomPhi)[imc],(*raw.mcMomMass)[imc]);
    mcPID_ = (*raw.mcPID)[imc];
    mcGMomPID_ = (*raw.mcGMomPID)[imc];
    mcMomPID_ = (*raw.mcMomPID)[imc];
    mcStatusFlag_ = (*raw.mcStatusFlag)[imc];
    mcStatus_ = (*raw.mcStatus)[imc];
  }

  ~mcData(){
  };
   
inline float getE() { return p4_.E();}
inline float getEt() { return p4_.Et();}
inline float getPt() { return p4_.Pt();}
inline float getEta() { return p4_.Eta();}
inline float getPhi() {return p4_.Phi();}
inline float getMass() {return p4_.M();}
inline TLorentzVector getP4() {
         TLorentzVector p = p4_;
          return p;
}
inline int getStatus(){return mcStatus_;}

inline float getmomE() { return momp4_.E();}
inline float getmomEt() { return momp4_.Et();}
inline float getmomPt() { return momp4_.Pt();}
inline float getmomEta() { return momp4_.Eta();}
inline float getmomPhi() {return momp4_.Phi();}
inline float getmomMass() {return momp4_.M();}
inline TLorentzVector getmomP4() {
         TLorentzVector p = momp4_;
          return p;
}

inline bool isPhoton(){
    if(mcPID_==22)return true;
    else{ return false;}
}

inline bool isElectron(){
    if(abs(mcPID_)==11)return true;
    else{return false;}
}

inline bool isMuon(){
    if(abs(mcPID_)==13)return true;
    else{return false;}
}

inline bool decayFromNeu(){
    if(mcMomPID_ == 1000022 || mcMomPID_==1000023)return true;
    else{return false;}
}

inline bool decayFromGlu(){
    if(mcMomPID_ == 1000021)return true;
    else{return false;}
}

inline int getMomPID(){ return mcMomPID_;}
inline int getGMomPID() {return mcGMomPID_;}
inline int getPID() {return mcPID_;}

private:
    TLorentzVector p4_;
    TLorentzVector momp4_;
    int      mcPID_;
    int      mcGMomPID_;
    int      mcMomPID_;
    int      mcStatusFlag_;
    int      mcStatus_;
};
   
