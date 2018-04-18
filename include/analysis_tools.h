enum MCType{
  NOMC = 0,
  WGJetInclusive = 1,
  WGJet40 = 2,
  WGJet130 = 3,
	ZGInclusive = 4,
	DYLL50 = 5,
	TTG = 6,
	WWG = 7,
	WZG = 8,
	DYLL10 = 9,
	ZG130 = 10,
	TT = 11,
	WW = 12,
	WZ = 13,
  W  = 14,
	QCDEM30 = 15,
	QCDEM40 = 16,
	QCDMU = 17,
	GJet = 18,
	generalMC = 19,
  nMCType = 20
};

double MC_XS[20] = {1, 489.0/*WGJetInclusive*/, 17.018/*WGJet40*/, 0.87971/*WGJet130*/, 117.8/*ZGInclusive*/, 5670/*DY*/, 3.697/*TTG*/, 0.21/*WWG*/,0.04/*WZG*/, 18610, 0.143, 750/*TTBar*/,63.21/*WW*/,22.82/*WZ*/, 61526/*W*/, 108000000/*QCDEM30*/, 54120000/*QCDEM40*/, 1/*MU*/, 16792/*GJet*/,1};

//0.143 // LO ZG130
//0.1404 // NLO ZG130

float DeltaPhi(float phi1, float phi2){
  float deltaPhi = phi1 - phi2;
  if(fabs(deltaPhi) > TMath::Pi()){
    if(deltaPhi > 0)deltaPhi = -1.0*(TMath::TwoPi() - fabs(deltaPhi));
    else deltaPhi = TMath::TwoPi() - fabs(deltaPhi);
  }
  return deltaPhi;
}

float DeltaR(float eta1,float phi1,float eta2,float phi2)
{
	float deltaPhi = TMath::Abs(phi1-phi2);
	float deltaEta = eta1-eta2;
	if(deltaPhi > TMath::Pi())
	deltaPhi = TMath::TwoPi() - deltaPhi;
		return TMath::Sqrt(deltaEta*deltaEta + deltaPhi*deltaPhi);
}

bool isHad(int PID, int momID){
   bool isFakePho;
   if(fabs(PID) == 22 || fabs(PID) == 11){
       switch(momID){
         case 1: isFakePho = false; break;
         case 2: isFakePho = false; break;
         case 3: isFakePho = false; break;
         case 4: isFakePho = false; break;
         case 5: isFakePho = false; break;
         case 6: isFakePho = false; break;
         case 999: isFakePho = false; break;
         case 11: isFakePho = false; break;
         case 13: isFakePho = false; break;
         case 15: isFakePho = false; break;
         case 22: isFakePho = false; break;
         case 24: isFakePho = false; break;
         case 21: isFakePho = false; break;
         default: isFakePho = true; break;
       }
  }
  else isFakePho = true;

  return isFakePho;
}

template<class recoParticle>
bool Veto(recoParticle reco, std::vector<recoPhoton>::iterator itpho, float dRcut){
  bool passVeto(true);
  typename recoParticle::iterator ire;
  for(ire = reco.begin(); ire != reco.end(); ire++)
    if(ire->getEt() > 2.0 && DeltaR(itpho->getEta(), itpho->getPhi(), ire->getEta(), ire->getPhi()) < dRcut)passVeto = false;
  return passVeto;
}


Double_t mybw(Double_t* x, Double_t* par)
{
  Double_t arg1 = 14.0/22.0; // 2 over pi
  Double_t arg2 = par[1]*par[1]*par[2]*par[2]; //Gamma=par[1]  M=par[2]
  Double_t arg3 = ((x[0]*x[0]) - (par[2]*par[2]))*((x[0]*x[0]) - (par[2]*par[2]));
  Double_t arg4 = x[0]*x[0]*x[0]*x[0]*((par[1]*par[1])/(par[2]*par[2]));
  Double_t arg5 = exp(par[3]+par[4]*x[0]);
  return par[0]*arg1*arg2/(arg3 + arg4)+ arg5;
}


Double_t mybwpol(Double_t* x, Double_t* par)
{
  Double_t arg1 = 14.0/22.0; // 2 over pi
  Double_t arg2 = par[1]*par[1]*par[2]*par[2]; //Gamma=par[1]  M=par[2]
  Double_t arg3 = ((x[0]*x[0]) - (par[2]*par[2]))*((x[0]*x[0]) - (par[2]*par[2]));
  Double_t arg4 = x[0]*x[0]*x[0]*x[0]*((par[1]*par[1])/(par[2]*par[2]));
  Double_t arg5 = par[3]*x[0]*x[0] + par[4]*x[0]+par[5]+par[6];
  return par[0]*arg1*arg2/(arg3 + arg4)+ arg5;
}

void DrawHisto(TObjArray *obj_list,TCanvas *canvas, int iPad, bool setLog){
  std::ostringstream name;
  int ihist(0);
  TObjArrayIter itr(obj_list);
  TH1F* hist(0);

  canvas->cd(iPad);
  if(setLog)gPad->SetLogy();
  TLegend *leg =  new TLegend(0.5,0.6,0.96,0.75);
  while( (hist = static_cast<TH1F*>(itr.Next())) ){
     ihist += 1;
     hist->SetLineColor(ihist+1);
     hist->SetLineWidth(2);
     hist->SetMarkerStyle(8);
     hist->SetMarkerColor(ihist+1);
     name.str("");
     name << hist->GetName();

     if(name.str().find("loose")!= std::string::npos){hist->SetLineColor(1);hist->SetMarkerColor(1);}
     else if(name.str().find("medium")!= std::string::npos){hist->SetLineColor(8);hist->SetMarkerColor(8);}
     else if(name.str().find("tight")!= std::string::npos){hist->SetLineColor(4);hist->SetMarkerColor(4);}
     leg->AddEntry(hist,name.str().c_str());
     if(ihist==1)hist->Draw();
     else hist->Draw("same");
  }
  leg->Draw("same");
}

unsigned findIndex(float* array, float kinevar,unsigned len){
  unsigned Index(0);
  for(unsigned i(0); i< len; i++){
    if(i != len-1){
      if(kinevar >= *(array+i) && kinevar < *(array+i+1))Index = i;}
    else{
      if(kinevar >= *(array+i))Index = i;
    } 
  }
  return Index;
}

float getPUESF(int nvertex){
	
	float pileupweights[100];
	pileupweights[0] = 0.00000;  
	pileupweights[1] = 7.04304;
	pileupweights[2] = 1.57478;
	pileupweights[3] = 1.25661;
	pileupweights[4] = 1.35018;
	pileupweights[5] = 1.30695;
	pileupweights[6] = 1.41195;
	pileupweights[7] = 1.45244;
	pileupweights[8] = 1.41624;
	pileupweights[9] = 1.43834;
	pileupweights[10]= 1.42312;
	pileupweights[11]= 1.39738;
	pileupweights[12]= 1.35533;
	pileupweights[13]= 1.32917;
	pileupweights[14]= 1.28631;
	pileupweights[15]= 1.24885;
	pileupweights[16]= 1.19631;
	pileupweights[17]= 1.15745;
	pileupweights[18]= 1.11156;
	pileupweights[19]= 1.06394;
	pileupweights[20]= 1.02461;
	pileupweights[21]= 0.97780;
	pileupweights[22]= 0.93597;
	pileupweights[23]= 0.89860;
	pileupweights[24]= 0.85123;
	pileupweights[25]= 0.82087;
	pileupweights[26]= 0.77079;
	pileupweights[27]= 0.72831;
	pileupweights[28]= 0.68178;
	pileupweights[29]= 0.63569;
	pileupweights[30]= 0.58593;
	pileupweights[31]= 0.55364;
	pileupweights[32]= 0.50999;
	pileupweights[33]= 0.46912;
	pileupweights[34]= 0.42884;
	pileupweights[35]= 0.40110;
	pileupweights[36]= 0.37670;
	pileupweights[37]= 0.34328;
	pileupweights[38]= 0.31827;
	pileupweights[39]= 0.30448;
	pileupweights[40]= 0.27736;
	pileupweights[41]= 0.25714;
	pileupweights[42]= 0.24828;
	pileupweights[43]= 0.25165;
	pileupweights[44]= 0.22350;
	pileupweights[45]= 0.21263;
	pileupweights[46]= 0.20924;
	pileupweights[47]= 0.20377;
	pileupweights[48]= 0.21069;
	pileupweights[49]= 0.19633;
	pileupweights[50]= 0.18781;
	pileupweights[51]= 0.18492;
	pileupweights[52]= 0.18520;
	pileupweights[53]= 0.18875;
	pileupweights[54]= 0.17958;
	pileupweights[55]= 0.16748;
	pileupweights[56]= 0.16586;
	pileupweights[57]= 0.19078;
	pileupweights[58]= 0.17135;
	pileupweights[59]= 0.16249;
	pileupweights[60]= 0.15503;
	pileupweights[61]= 0.12345;
	pileupweights[62]= 0.14708;
	pileupweights[63]= 0.13309;
	pileupweights[64]= 0.14519;
	pileupweights[65]= 0.11982;
	pileupweights[66]= 0.11879;
	pileupweights[67]= 0.11448;
	pileupweights[68]= 0.14388;
	pileupweights[69]= 0.10006;
	pileupweights[70]= 0.08019;
	pileupweights[71]= 0.12361;
	pileupweights[72]= 0.12655;
	pileupweights[73]= 0.09417;
	pileupweights[74]= 0.07416;
	pileupweights[75]= 0.06769;
	pileupweights[76]= 0.05976;
	pileupweights[77]= 0.06849;
	pileupweights[78]= 0.14126;
	pileupweights[79]= 0.10595;
	pileupweights[80]= 0.09888;
	pileupweights[81]= 0.08476;
	pileupweights[82]= 0.04512;
	pileupweights[83]= 0.04316;
	pileupweights[84]= 0.04120;
	pileupweights[85]= 0.02354;
	pileupweights[86]= 0.07063;
	pileupweights[87]= 0.02354;
	pileupweights[88]= 0.02354;
	pileupweights[89]= 0.10595;
	pileupweights[90]= 0;
	pileupweights[91]= 0.14126;
	pileupweights[92]= 0.02354;
	pileupweights[93]= 0;
	pileupweights[94]= 0.03531;
	pileupweights[95]= 0;
	pileupweights[96]= 0;
	pileupweights[97]= 0;
	pileupweights[98]= 0;
	pileupweights[99]= 0;

	if(nvertex > 99)return 0;
	else return pileupweights[nvertex]; 
}


//Double_t bkgEtBins[]={35,40,45,50,55,60,65,70,75,80, 85,90,95,100,105,110,115,120,125,130, 135,140,146,152,158,164,170,177,184,192, 200,208,216,224,232,240,250,260,275,290, 305,325,345,370,400,500,800};
Double_t bkgEtBins[]={35,40,50,60,70,80,90,100,110,120,130,140,150,160,170,185,200,215,230,250,275,290, 305,325,345,370,400,500,800};
int nBkgEtBins= sizeof(bkgEtBins)/sizeof(bkgEtBins[0]) -1;
//Double_t bkgPtBins[]={25,30,35,40,45,50,55,60,65,70,75,80, 85,90,95,100,105,110,115,120,125,130, 135,140,146,152,158,164,170,177,184,192, 200,208,216,224,232,240,250,260,275,290, 305,325,345,370,400,500,800};
Double_t bkgPtBins[]={25,50,75,100,125,150,200,400,800};
int nBkgPtBins= sizeof(bkgPtBins)/sizeof(bkgPtBins[0])-1;
Double_t bkgMETBins[]={0,40,60,80,100,120,140,160,180,210,240,280,320,400,600,1000};
int nBkgMETBins= sizeof(bkgMETBins)/sizeof(bkgMETBins[0]) -1;
Double_t bkgMtBins[]={0,20,40,60,80,100,120,140,160,180,200,300,400,500,1000};
int nBkgMtBins= sizeof(bkgMtBins)/sizeof(bkgMtBins[0]) -1;
Double_t bkgHTBins[]={0,40,60,80,100,120,140,160,180,200,225,250,275,300,340,380,420,500,600,1000};
int nBkgHTBins= sizeof(bkgHTBins)/sizeof(bkgHTBins[0]) -1;


//Double_t sigEtBins[]={35,50,100,150,200,300,500,800};
//int nSigEtBins= sizeof(sigEtBins)/sizeof(sigEtBins[0]) -1;
//Double_t sigPtBins[]={25,50,100,150,200,300,500,800};
//int nSigPtBins= sizeof(sigPtBins)/sizeof(sigPtBins[0])-1;
//Double_t sigMETBins[]={120,200,300,400,550,1000};
//int nSigMETBins= sizeof(sigMETBins)/sizeof(sigMETBins[0]) -1;
//Double_t sigMtBins[]={100,200,300,400,600,1000};
//int nSigMtBins= sizeof(sigMtBins)/sizeof(sigMtBins[0]) -1;
//Double_t sigHTBins[]={0,400,800,1500,2000};
//int nSigHTBins= sizeof(sigHTBins)/sizeof(sigHTBins[0]) -1;
//
Double_t sigEtBins[]={35,50,100,150,200,300,500,800};
int nSigEtBins= sizeof(sigEtBins)/sizeof(sigEtBins[0]) -1;
Double_t sigPtBins[]={25,50,100,150,200,300,500,800};
int nSigPtBins= sizeof(sigPtBins)/sizeof(sigPtBins[0])-1;
Double_t sigMETBins[]={0,20,40,60,80,100,120,150,200,250,300,350,400,600};
int nSigMETBins= sizeof(sigMETBins)/sizeof(sigMETBins[0]) -1;
Double_t sigMtBins[]={100,200,300,400,600,1000};
int nSigMtBins= sizeof(sigMtBins)/sizeof(sigMtBins[0]) -1;
Double_t sigHTBins[]={0,100,200,300,400,800,1500,2000};
int nSigHTBins= sizeof(sigHTBins)/sizeof(sigHTBins[0]) -1;
