#ifndef ANALYSIS_RAWDATA
#define ANALYSIS_RAWDATA

#ifndef ROOT_TTree
#include "TTree.h"
#include "TString.h"
#endif

using namespace std;

enum RunType{
  MC = 0,
  DoubleEG2015 = 1,
  MuonEG2015 = 2,
  SingleElectron2015 = 3,
  SingleMuon2015 = 4,
  DoubleMuon2015 = 5,
  DoubleEG2016 = 6,
  MuonEG2016 = 7,
  SingleElectron2016 = 8,
  SingleMuon2016 = 9,
  DoubleMuon2016 = 10,
  MET2016 = 11,
	MCDoubleEG = 12,
	MCMuonEG = 13,
	MCSingleElectron = 14,
	MCSingleMuon = 15,
	MCDoubleMuon = 16,
	MCMET = 17,
  nRunType = 18
};



  class rawData{
  public:
    rawData(TTree *tree, RunType type): runtype_(type)
  {    
	  if(type == MC || type == MCDoubleEG || type == MCMuonEG||  type == MCSingleElectron || type == MCSingleMuon||  type == MCDoubleMuon || type == MCMET ){
		tree->SetBranchAddress("EventTag",     &EventTag);
		
		tree->SetBranchAddress("nMC",          &nMC);
		tree->SetBranchAddress("mcPID",        &mcPID);
		tree->SetBranchAddress("mcPt",         &mcPt);
		tree->SetBranchAddress("mcMass",       &mcMass);
		tree->SetBranchAddress("mcEta",        &mcEta);
		tree->SetBranchAddress("mcPhi",        &mcPhi);
		tree->SetBranchAddress("mcE",          &mcE);
		tree->SetBranchAddress("mcEt",         &mcEt);
		tree->SetBranchAddress("mcStatusFlag", &mcStatusFlag);
		tree->SetBranchAddress("mcStatus",     &mcStatus);
		tree->SetBranchAddress("mcGMomPID",    &mcGMomPID);
		tree->SetBranchAddress("mcMomPID",     &mcMomPID);
		tree->SetBranchAddress("mcMomPt",      &mcMomPt);
		tree->SetBranchAddress("mcMomMass",    &mcMomMass);
		tree->SetBranchAddress("mcMomEta",     &mcMomEta);
		tree->SetBranchAddress("mcMomPhi",     &mcMomPhi);
		tree->SetBranchAddress("genMET",    			&genMET);
		tree->SetBranchAddress("genMETPhi",    		&genMETPhi);
	//	tree->SetBranchAddress("pdfWeight",       &pdfWeight);
	//	tree->SetBranchAddress("pdfSystWeight",   &pdfSystWeight);
	//	tree->SetBranchAddress("genScaleSystWeights", &genScaleSystWeights);
    }
	  tree->SetBranchAddress("run",			        &run);
	  tree->SetBranchAddress("event",			    &event);
	  tree->SetBranchAddress("lumis",			    &lumis);
      tree->SetBranchAddress("nVtx",			    &nVtx);
      tree->SetBranchAddress("nGoodVtx",		    &nGoodVtx);
      tree->SetBranchAddress("isPVGood",		    &isPVGood);
      tree->SetBranchAddress("rho",                 &rho);
      tree->SetBranchAddress("HLTEleMuX",           &HLTEleMuX);
      tree->SetBranchAddress("HLTPho",              &HLTPho);
      tree->SetBranchAddress("HLTJet",              &HLTJet);
      tree->SetBranchAddress("HLTEleMuXIsPrescaled",&HLTEleMuXIsPrescaled);
      tree->SetBranchAddress("HLTPhoIsPrescaled",   &HLTPhoIsPrescaled);
      tree->SetBranchAddress("pfMET",    			&pfMET);
      tree->SetBranchAddress("pfMETPhi",    		&pfMETPhi);
			tree->SetBranchAddress("pfMET_T1JERUp",			&pfMET_T1JERUp);
			tree->SetBranchAddress("pfMET_T1JERDo",     &pfMET_T1JERDo);
			tree->SetBranchAddress("pfMET_T1JESUp",     &pfMET_T1JESUp);
			tree->SetBranchAddress("pfMET_T1JESDo",     &pfMET_T1JESDo);
			tree->SetBranchAddress("pfMETPhi_T1JESUp",  &pfMETPhi_T1JESUp);
			tree->SetBranchAddress("pfMETPhi_T1JESDo",  &pfMETPhi_T1JESDo);
			tree->SetBranchAddress("pfMETPhi_T1UESUp",  &pfMETPhi_T1UESUp);
			tree->SetBranchAddress("pfMETPhi_T1UESDo",  &pfMETPhi_T1UESDo);
      tree->SetBranchAddress("metFilters",   		&metFilters);

      tree->SetBranchAddress("nPho",                    &nPho);
      tree->SetBranchAddress("phoE",                    &phoE);
      tree->SetBranchAddress("phoEt",                   &phoEt);
      tree->SetBranchAddress("phoCalibEt",              &phoCalibEt);
      tree->SetBranchAddress("phoEta",                  &phoEta);
      tree->SetBranchAddress("phoPhi",                  &phoPhi);
      tree->SetBranchAddress("phoSCEta",                &phoSCEta);
      tree->SetBranchAddress("phoSCPhi",                &phoSCPhi);
      tree->SetBranchAddress("phohasPixelSeed",         &phohasPixelSeed);
      tree->SetBranchAddress("phoEleVeto",              &phoEleVeto);
      tree->SetBranchAddress("phoR9Full5x5",                   &phoR9);
      tree->SetBranchAddress("phoHoverE",               &phoHoverE);
      tree->SetBranchAddress("phoSigmaIEtaIEtaFull5x5", &phoSigmaIEtaIEtaFull5x5);
      tree->SetBranchAddress("phoPFChIso",              &phoPFChIso);
      tree->SetBranchAddress("phoPFPhoIso",             &phoPFPhoIso);
      tree->SetBranchAddress("phoPFNeuIso",             &phoPFNeuIso);
      tree->SetBranchAddress("phoFiredSingleTrgs",         &phoFiredSingleTrgs);
      tree->SetBranchAddress("phoFiredDoubleTrgs",         &phoFiredDoubleTrgs);
      tree->SetBranchAddress("phoFiredL1Trgs",             &phoFiredL1Trgs);
      tree->SetBranchAddress("phoIDbit",      &phoIDbit);

      tree->SetBranchAddress("nMu",           &nMu);
      tree->SetBranchAddress("muPt",          &muPt);
      tree->SetBranchAddress("muEn",          &muEn);
      tree->SetBranchAddress("muEta",         &muEta);
      tree->SetBranchAddress("muPhi",         &muPhi);
      tree->SetBranchAddress("muCharge",      &muCharge);
      tree->SetBranchAddress("muType",        &muType);
      tree->SetBranchAddress("muIDbit",       &muIDbit); 
			tree->SetBranchAddress("muD0",          &muD0);
			tree->SetBranchAddress("muDz",          &muDz);
      tree->SetBranchAddress("muPFMiniIso",   &muPFMiniIso);
      tree->SetBranchAddress("muPFChIso",     &muPFChIso);
      tree->SetBranchAddress("muPFPhoIso",    &muPFPhoIso);
      tree->SetBranchAddress("muPFNeuIso",    &muPFNeuIso);
      tree->SetBranchAddress("muPFPUIso",     &muPFPUIso);
      tree->SetBranchAddress("muFiredTrgs",   &muFiredTrgs);
      tree->SetBranchAddress("muFiredL1Trgs", &muFiredL1Trgs);

      tree->SetBranchAddress("nEle",                    &nEle);
      tree->SetBranchAddress("eleCharge",               &eleCharge);
      tree->SetBranchAddress("eleEn",                   &eleEn);
      tree->SetBranchAddress("eleD0",                   &eleD0);
      tree->SetBranchAddress("eleDz",                   &eleDz);
      tree->SetBranchAddress("elePt",                   &elePt);
      tree->SetBranchAddress("eleCalibPt",              &eleCalibPt);
      tree->SetBranchAddress("eleCalibEn",              &eleCalibEn);
      tree->SetBranchAddress("eleEta",                  &eleEta);
      tree->SetBranchAddress("eleSCEta",                &eleSCEta);
      tree->SetBranchAddress("elePhi",                  &elePhi);
      tree->SetBranchAddress("eleR9",                   &eleR9);
      tree->SetBranchAddress("eleHoverE",               &eleHoverE);
      tree->SetBranchAddress("eleEoverPInv",            &eleEoverPInv);
      tree->SetBranchAddress("eleSigmaIEtaIEtaFull5x5", &eleSigmaIEtaIEta);
      tree->SetBranchAddress("eleConvVeto",             &eleConvVeto);
      tree->SetBranchAddress("eleMissHits",             &eleMissHits);
      tree->SetBranchAddress("elePFChIso",              &elePFChIso);
      tree->SetBranchAddress("elePFPhoIso",             &elePFPhoIso);
      tree->SetBranchAddress("elePFNeuIso",             &elePFNeuIso);
      tree->SetBranchAddress("elePFPUIso",              &elePFPUIso);
      tree->SetBranchAddress("elePFMiniIso",            &elePFMiniIso);
      tree->SetBranchAddress("eleTrkdxy",               &eleTrkdxy);
      tree->SetBranchAddress("eledEtaAtVtx",            &eledEtaAtVtx);
      tree->SetBranchAddress("eledPhiAtVtx",            &eledPhiAtVtx);
	  tree->SetBranchAddress("eleFiredSingleTrgs",      &eleFiredSingleTrgs);
	  tree->SetBranchAddress("eleFiredDoubleTrgs",      &eleFiredDoubleTrgs);
	  tree->SetBranchAddress("eleFiredL1Trgs",          &eleFiredL1Trgs);
      tree->SetBranchAddress("eleIDbit",  &eleIDbit);

      tree->SetBranchAddress("nJet",      &nJet);
      tree->SetBranchAddress("jetPt",     &jetPt);
      tree->SetBranchAddress("jetEn",     &jetEn);
      tree->SetBranchAddress("jetEta",    &jetEta);
      tree->SetBranchAddress("jetPhi",    &jetPhi);
      //tree->SetBranchAddress("jetID",     &jetID);
			tree->SetBranchAddress("jetArea",   &jetArea);
			tree->SetBranchAddress("jetJECUnc", &jetJECUnc);
			tree->SetBranchAddress("jetCSV2BJetTags", &jetCSV2BJetTags);


    }
   
    ~rawData(){
    };

    void GetData(TTree *tree, Int_t iEvt){
      tree->GetEntry(iEvt);
    }

    bool passHLT();
		bool passMETFilter(int filter);
		int  failFilterStep(int filter);
    RunType runtype_;

		TString   *EventTag=0;	
    Int_t     run=0;
    Long64_t  event=0;
    Int_t     lumis=0;
    Int_t     nVtx = 0;
    Int_t     nGoodVtx = 0;
    Bool_t    isPVGood = 0;
    float     rho =0;
    ULong64_t HLTEleMuX =0;
    ULong64_t HLTPho =0;
		ULong64_t HLTJet =0;
    ULong64_t HLTEleMuXIsPrescaled =0;
    ULong64_t HLTPhoIsPrescaled =0;
    float     pfMET = 0;
    float     pfMETPhi = 0;
    float     genMET = 0;
    float     genMETPhi = 0;
		float 		pfMET_T1JERUp = 0;
		float 		pfMET_T1JERDo = 0;
		float 		pfMET_T1JESUp = 0;
		float 		pfMET_T1JESDo = 0;	
		float			pfMETPhi_T1JESUp = 0;
		float			pfMETPhi_T1JESDo = 0;
		float			pfMETPhi_T1UESUp = 0;
		float			pfMETPhi_T1UESDo = 0;
    Int_t     metFilters=0;

    Int_t                 nMC =0;
    std::vector<int>      *mcPID =0;
    std::vector<float>    *mcPt =0;
    std::vector<float>    *mcMass =0;
    std::vector<float>    *mcEta =0;
    std::vector<float>    *mcPhi =0;
    std::vector<float>    *mcE =0;
    std::vector<float>    *mcEt =0;
		vector<UShort_t>      *mcStatusFlag=0;
		vector<int>      			*mcStatus=0;
    std::vector<int>      *mcGMomPID =0;
    std::vector<int>      *mcMomPID =0;
    std::vector<float>    *mcMomPt =0;
    std::vector<float>    *mcMomMass =0;
    std::vector<float>    *mcMomEta =0;
    std::vector<float>    *mcMomPhi =0;
		float                 pdfWeight = 0;
		std::vector<float>    *pdfSystWeight = 0;
		std::vector<float>    *genScaleSystWeights = 0;

    Int_t          nPho =0;
    vector<float>  *phoE = 0;
    vector<float>  *phoEt = 0;
    vector<float>  *phoCalibEt = 0;
    vector<float>  *phoEta = 0;
    vector<float>  *phoPhi = 0;
    vector<float>  *phoSCEta =0;
    vector<float>  *phoSCPhi =0;
    vector<int>    *phohasPixelSeed =0;
    vector<int>    *phoEleVeto =0;
    vector<float>  *phoR9 =0;
    vector<float>  *phoHoverE =0;
    vector<float>  *phoSigmaIEtaIEta =0;
    vector<float>  *phoSigmaIEtaIEtaFull5x5 = 0;
    vector<float>  *phoPFChIso =0;
    vector<float>  *phoPFPhoIso =0;
    vector<float>  *phoPFNeuIso =0;
    vector<Int_t>  *phoFiredSingleTrgs =0;
    vector<Int_t>  *phoFiredDoubleTrgs =0;
    vector<Int_t>  *phoFiredL1Trgs =0;
    vector<UShort_t> *phoIDbit =0;


    Int_t          nMu =0;
    vector<float>  *muPt =0;
    vector<float>  *muEn =0;
    vector<float>  *muEta =0;
    vector<float>  *muPhi =0;
    vector<int>    *muCharge =0;
    vector<int>    *muType =0;
    vector<UShort_t> *muIDbit =0;
		vector<float>  *muD0 = 0;
		vector<float>  *muDz = 0;
    vector<float>  *muPFMiniIso=0;
    vector<float>  *muPFChIso =0;
    vector<float>  *muPFPhoIso =0;
    vector<float>  *muPFNeuIso =0;
    vector<float>  *muPFPUIso =0;
    vector<UInt_t> *muFiredTrgs =0;
    vector<UInt_t> *muFiredL1Trgs = 0;

    Int_t          nEle =0;
    vector<int>    *eleCharge =0;
    vector<float>  *eleEn =0;
    vector<float>  *eleD0 =0;
    vector<float>  *eleDz =0;
    vector<float>  *elePt =0;
    vector<float>  *eleCalibPt=0;
    vector<float>  *eleCalibEn=0;
    vector<float>  *eleEta =0;
    vector<float>  *eleSCEta =0;
    vector<float>  *elePhi =0;
    vector<float>  *eleR9 =0;
    vector<float>  *eleHoverE =0;
    vector<float>  *eleEoverPInv =0;
    vector<float>  *eleSigmaIEtaIEta =0;
    vector<int>    *eleConvVeto =0;
    vector<int>    *eleMissHits =0;
    vector<float>  *elePFChIso =0;
    vector<float>  *elePFPhoIso =0;
    vector<float>  *elePFNeuIso =0;
    vector<float>  *elePFPUIso =0;
    vector<float>  *elePFMiniIso =0;
    vector<float>  *eleTrkdxy =0;
    vector<float>  *eledEtaAtVtx=0;
    vector<float>  *eledPhiAtVtx=0;
    vector<UInt_t>  *eleFiredSingleTrgs=0;
	vector<UInt_t>  *eleFiredDoubleTrgs=0;
	vector<UInt_t>  *eleFiredL1Trgs=0;
    vector<UShort_t> *eleIDbit =0;

    Int_t          nJet = 0;
    std::vector<float>  *jetPt = 0;
    std::vector<float>  *jetEn = 0;
    std::vector<float>  *jetEta = 0;
    std::vector<float>  *jetPhi = 0;
		std::vector<int>    *jetID  = 0;
		std::vector<float>  *jetArea = 0;
		std::vector<float>  *jetJECUnc = 0;
		std::vector<float>  *jetCSV2BJetTags = 0;
  };

#endif
