#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
#include<algorithm>

#include "TFile.h"
#include "TTree.h"
#include "TF1.h"
#include "TF3.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TString.h"
#include "TChain.h"
#include "TSystem.h"
#include "TMath.h"
#include "TLegend.h"
#include "TLine.h"
#include "TLatex.h"
#include "TProfile.h"
#include "TLorentzVector.h"
#include "TRandom3.h"
#include "TGraphErrors.h"
#include "TVector2.h"

#include "../include/analysis_rawData.h"
#include "../include/analysis_photon.h"
#include "../include/analysis_muon.h"
#include "../include/analysis_ele.h"
#include "../include/analysis_jet.h"
#include "../include/analysis_mcData.h"
#include "../include/analysis_tools.h"
#include "../include/analysis_fakes.h"
#include "../include/analysis_binning.h"
#include "../include/analysis_scalefactor.h"
#include "../include/tdrstyle.C"

int ichannel(1);
int anatype(-1);
int lowMt(0);
int highMt(-1);
int lowMET(0);
int highMET(-1);
int lowPt(25);
int highPt(-1);
int lepIso(4);

int   NBIN(0);
float METbin1(200), METbin2(400);
float HTbin1(100),  HTbin2(400);
float PHOETbin(200);

double factor_egQCD(0.237);
double factorerror_egQCD(0.0474);
double factor_mgQCD(0.62);
double factorerror_mgQCD(0.124);
double factor_egVGamma(1.17);
double factorerror_egVGamma(0.234);
double factor_mgVGamma(1.33);
double factorerror_mgVGamma(0.266);

bool SetRunConfig(){
	
	std::ifstream configfile("/uscms/homes/m/mengleis/work/SUSY2016/SUSYAnalysis/test/Background/BkgPredConfig.txt");
	std::string conftype;
	double confvalue;
	if(configfile.is_open()){
  	for(int i(0); i<9; i++){ 
			configfile >> conftype >> confvalue; 
			if(conftype.find("ichannel")!=std::string::npos)ichannel = confvalue;
			if(conftype.find("anatype")!=std::string::npos)anatype = confvalue;
			if(conftype.find("lowMt")!=std::string::npos)lowMt = confvalue;
			if(conftype.find("highMt")!=std::string::npos)highMt = confvalue;
			if(conftype.find("lowMET")!=std::string::npos)lowMET = confvalue;
			if(conftype.find("highMET")!=std::string::npos)highMET = confvalue;
			if(conftype.find("lowPt")!=std::string::npos)lowPt = confvalue;
			if(conftype.find("highPt")!=std::string::npos)highPt = confvalue;
			if(conftype.find("lepIso")!=std::string::npos)lepIso = confvalue;
	  }
	}
	configfile.close();
	
	if(anatype >= 0)return true;
	else return false;
}

bool SetSignalConfig(){
	
	std::ifstream configfile("/uscms/homes/m/mengleis/work/SUSY2016/SUSYAnalysis/test/Result/SigConfig.txt");
	std::string conftype;
	double confvalue;
	if(configfile.is_open()){
  	for(int i(0); i<9; i++){ 
			configfile >> conftype >> confvalue; 
			if(conftype.find("ichannel")!=std::string::npos)ichannel = confvalue;
			if(conftype.find("anatype")!=std::string::npos)anatype = confvalue;
			if(conftype.find("lowMt")!=std::string::npos)lowMt = confvalue;
			if(conftype.find("highMt")!=std::string::npos)highMt = confvalue;
			if(conftype.find("lowMET")!=std::string::npos)lowMET = confvalue;
			if(conftype.find("highMET")!=std::string::npos)highMET = confvalue;
			if(conftype.find("lowPt")!=std::string::npos)lowPt = confvalue;
			if(conftype.find("highPt")!=std::string::npos)highPt = confvalue;
			if(conftype.find("lepIso")!=std::string::npos)lepIso = confvalue;
	  }
	}
	configfile.close();

	std::ifstream binfile("/uscms/homes/m/mengleis/work/SUSY2016/SUSYAnalysis/test/script/binConfig.txt");
	if(binfile.is_open()){
		for(int i(0); i<6; i++){
			binfile >> conftype >> confvalue;
			if(conftype.find("NBIN")!=std::string::npos)NBIN = int(confvalue);
			if(conftype.find("METbin1")!=std::string::npos)METbin1= confvalue;
			if(conftype.find("METbin2")!=std::string::npos)METbin2= confvalue;
			if(conftype.find("HTbin1")!=std::string::npos)HTbin1= confvalue;
			if(conftype.find("HTbin2")!=std::string::npos)HTbin2= confvalue;
			if(conftype.find("PHOETbin")!=std::string::npos)PHOETbin= confvalue;
		}
	}
	
	if(anatype == 3)return true;
	else return false;
}

double calcToyError(vector<double> & vtoy, bool useGauss){
	
		double syserr(0);
		double normvalue = vtoy[0];
		std::sort(vtoy.begin(), vtoy.end());
		TH1D *temphist = new TH1D("temphist","",500,0.5*normvalue,1.5*normvalue);
		for(unsigned it(0); it < vtoy.size(); it++){
			temphist->Fill(vtoy[it]);
		}
		if(useGauss){
			temphist->Fit("gaus");
			syserr = temphist->GetFunction("gaus")->GetParameter(2);
		}
		else syserr = std::max( fabs(vtoy.front() - normvalue), fabs(vtoy.back() - normvalue));
		delete temphist;

		return syserr;
}
