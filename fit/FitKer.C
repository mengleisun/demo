#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
#include<algorithm>
#include "TH1F.h"
#include "TH2F.h"
#include "TH2D.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TProfile.h"
#include "TLegend.h"
#include "TAxis.h"
#include "TLine.h"
#include "TLatex.h"
#include "TF1.h"
#include "TMath.h"
#include "TSystem.h"
#include "TChain.h"
#include "TTree.h"

#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooAddPdf.h"
#include "RooAbsReal.h"
#include "RooKeysPdf.h"
#include "RooConstVar.h"
#include "RooDataHist.h"
#include "RooPlot.h"
#include "RooNumIntConfig.h"
#include "RooFFTConvPdf.h"
#include "RooBreitWigner.h"
#include "RooCBShape.h"
#include "RooHistPdf.h"
#include "RooMCStudy.h"
#include "RooChi2MCSModule.h"
#include "RooFitResult.h"
#include "RooExponential.h"
#include "../include/RooDCBShape.h"
#include "../include/tdrstyle.C"

void FitKer(){//main  

  gSystem->Load("/uscms/home/mengleis/work/SUSY2016/SUSYAnalysis/lib/libRooFitClasses.so");
	setTDRStyle();

//************** Process Z->ee Tree ********************************************************//   
  TH1F*  p_invmass = new TH1F("p_invmass", "p_invmass",60,60,120);

  TChain *etree = new TChain("FakeRateTree");																									
  etree->Add("/uscms_data/d3/mengleis/FullStatusOct/plot_elefakepho-FullEcalTnP.root");

  float invmass=0; 
  float probePt=0; 
  bool  vetovalue=0;
	bool  FSRveto=0;
  etree->SetBranchAddress("invmass",   &invmass); 
  etree->SetBranchAddress("probePt",   &probePt);
  etree->SetBranchAddress("vetovalue", &vetovalue);
	etree->SetBranchAddress("FSRveto",   &FSRveto);

  for(unsigned iEvt(0); iEvt < etree->GetEntries(); iEvt++){
    etree->GetEntry(iEvt);
		if(vetovalue== true && FSRveto == true)
			p_invmass->Fill(invmass); 
  }

//************** Process mg Bkg Tree *******************************************//
  TH1F* bg_pt = new TH1F("bg", "", 95, 55,150);
  TTree *newbgtree = new TTree("newbgtree","newbgtree");
  double invmass_fordataset(0);
  newbgtree->Branch("invmass", &invmass_fordataset);

  TChain *bgtree = new TChain("BGTree");
  bgtree->Add("/uscms_data/d3/mengleis/FullStatusOct/plot_bgtemplate_FullEcal.root");
  float invmass_bg=0; 
  float probePt_bg=0; 
  bool  vetovalue_bg=0;
  bool  FSRveto_bg=0;	
  bgtree->SetBranchAddress("invmass",   &invmass_bg); 
  bgtree->SetBranchAddress("probePt",   &probePt_bg);
  bgtree->SetBranchAddress("vetovalue", &vetovalue_bg);
  bgtree->SetBranchAddress("FSRveto",   &FSRveto_bg);

  for(unsigned iEvt(0); iEvt < bgtree->GetEntries(); iEvt++){
		bgtree->GetEntry(iEvt);
		if(vetovalue_bg== true && FSRveto_bg==true){
			bg_pt->Fill(invmass_bg); 
			invmass_fordataset=invmass_bg; 
			newbgtree->Fill();
		}
  }

//************* Construct RooFit Models *********************************************************//
  RooRealVar mass_axis("invmass","M_{tag-probe}",60,120);
  RooDataSet BkgDataSet("BkgDataSet","BkgDataSet",RooArgSet(mass_axis),RooFit::Import(*newbgtree));
  RooKeysPdf BkgKer("BkgKer","BkgKer",mass_axis,BkgDataSet,RooKeysPdf::MirrorBoth, 2);
/*********************** Plot Kernel Background ********************************/ 
  TCanvas *c_ker = new TCanvas("ker","",600,600);														 
  c_ker->cd();
  RooPlot* kerFrame = mass_axis.frame(RooFit::Title("kernel"));
  BkgDataSet.plotOn(kerFrame);
  BkgKer.plotOn(kerFrame);
  kerFrame->Draw();
  c_ker->SaveAs("Kernel.pdf");
/******************************************************************************/

  TCanvas *c_fitMass = new TCanvas("c_fitMass", "", 600, 600);
	c_fitMass->SetLeftMargin(0.15);
	c_fitMass->SetBottomMargin(0.15);
  RooDataHist datahist_data("both", "", mass_axis, p_invmass);

  c_fitMass->cd();

  RooRealVar m0( "m0", "m0", 91.188, 80,100);
  RooRealVar width( "width", "width", 2.495, 0, 15);
  //RooRealVar m0( "m0", "m0", 91.188);
  //RooRealVar width( "width", "width", 2.495);
  RooRealVar mean("mean", "" ,0.,-1,1);
  RooRealVar sigma("sigma", "",2.4 , 0.0, 15.0);
  //RooRealVar sigma("sigma", "", 1.0, 0, 2);
  RooRealVar alpha("alpha", "", 1.0, 0.0, 20.0);
  RooRealVar n("n","", 1.0, 0.0, 20.0);
  RooRealVar alpha2("2ndalpha","", 1.0, 0.0, 20.0);
  RooRealVar n2("2ndn", "", 1.0, 0.0, 20.0);
  RooBreitWigner bw("bw", "", mass_axis, m0, width);
  RooDCBShape *cb = new RooDCBShape("cb","cb", mass_axis, mean, sigma, alpha, n, alpha2, n2);
  RooGaussian gauss("gs", "gs", mass_axis, mean, sigma);
  RooFFTConvPdf *signalRes = new RooFFTConvPdf("pdf", "pdf",mass_axis, bw, *cb);
  double iniSig = p_invmass->Integral(20,40);
  double iniBkg = p_invmass->Integral(1,20) + p_invmass->Integral(40,60);
  RooRealVar nSig("nSig", "", iniSig, 0, p_invmass->GetEntries()*1.2);
  RooRealVar nBkg("nBkg", "", iniBkg, 0, p_invmass->GetEntries());
  RooAddPdf *model = new RooAddPdf("model", "", RooArgList(BkgKer, *signalRes),RooArgList(nBkg, nSig));   

  RooPlot* mass_Frame = mass_axis.frame(RooFit::Title("demo"),RooFit::Bins(60));
  model->fitTo(datahist_data);
  model->plotOn(mass_Frame, RooFit::Components(BkgKer),
				 RooFit::LineStyle(kDashed),
				 RooFit::Normalization(1.0, RooAbsReal::RelativeExpected));
  model->plotOn(mass_Frame,
				 RooFit::Components(RooArgSet(BkgKer, *signalRes)),
				 RooFit::LineStyle(kSolid),
				 RooFit::Normalization(1.0, RooAbsReal::RelativeExpected));
  datahist_data.plotOn(mass_Frame);
  mass_Frame->Draw();

  double chi2 = mass_Frame->chiSquare();
  TLatex* latex = new TLatex();
  char chi2str[50];
  sprintf (chi2str, "chi2/ndf = %.02f", chi2);
  latex->DrawLatex(95,0.7*p_invmass->GetBinContent(31),chi2str);
  c_fitMass->SaveAs("demo.pdf");

  mass_axis.setRange("signal",80,101);
  RooAbsReal* igx_sig;
	igx_sig = signalRes->createIntegral(mass_axis,RooFit::NormSet(mass_axis),RooFit::Range("signal"));
	double norminalmean = igx_sig->getVal()*(nSig.getVal());
	double norminalrms  = igx_sig->getVal()*(nSig.getError());
		
	std::cout << norminalmean << " " << norminalrms << std::endl;

}
