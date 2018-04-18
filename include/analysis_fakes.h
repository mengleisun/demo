double etaRatesEB[]={0.0494428, 0.0318789, 0.025132, 0.024415, 0.0226846, 0.0222701, 0.0213539, 0.0205112, 0.0209977, 0.0211939, 0.0192827, 0.0195855, 0.0184657, 0.0245522, 0.0243049, 0.021924, 0.0204956, 0.0195934, 0.0199442, 0.0192691, 0.017597, 0.0182474, 0.0291908, 0.0223044, 0.0188447, 0.0186427, 0.0204014, 0.0237803, 0.0299409}; 
double etaRatesEE[]={0.0307332,0.0299524,0.0280145,0.02547,0.0240278,0.021802,0.0225558,0.0226875,0.0226886,0.0215866,0.0236088,0.020717,0.0216991,0.0226854,0.0261154,0.0234785,0.0237738,0.0238696,0.0231526,0.0241064,0.0236423,0.0225728,0.0225309,0.0215295,0.0229856,0.0243029,0.0229342,0.0232938,0.0247321,0.023548,0.022498,0.0218185,0.0234682,0.0254467,0.0246834,0.0229259,0.0231025,0.0216125,0.0210904,0.0221214,0.0202627,0.0191955,0.0160481,0.0167744,0.0160238,0.0159168,0.014636,0.0140576,0.0140319,0.0145108,0.0134792,0.0133057,0.0126636,0.0134775,0.013568,0.0148307,0.0137881,0.0142709,0.0140171,0.0141443,0.0132946,0.0153951,0.0128556,0.0131305,0.0117914,0.0133917,0.0128375,0.0132793,0.0119076,0.0128915,0.0121654,0.0120543,0.0132253,0.0130123,0.0126019,0.0131891,0.0144337,0.0154444,0.0201884,0.023823,0.0273998,0.0323177,0.0384226,0.0467818,0.0621622,0.077865,0.101108,0.12769,0.155996,0.190138,0.234992,0.292042,0.303861,0.183474};

double MCetaRatesEB[]={0.030, 0.019, 0.015, 0.015, 0.014, 0.013, 0.013, 0.012, 0.012, 0.011, 0.011, 0.011, 0.010, 0.012, 0.011, 0.011, 0.010, 0.010, 0.011, 0.011, 0.009, 0.010, 0.011, 0.010, 0.010, 0.011, 0.012, 0.012, 0.015};
double MCetaRatesEE[]={};

Double_t fakerate_func(Double_t *x, Double_t *par)
{
	double weight_pt(0.02);
	double weight_nvtx(0.02);
	double weight_eta(0.02);

	double normfactor = par[0];
  double slope = par[1];
  double constant = par[2];
  double index = par[3];
  double coeff = 1.0; 
	double vtx_constant = par[5];
	double vtx_slope = par[6];

  double pt = TMath::Max(x[0],0.000001);
	double nvtx=x[1];
	double eta= x[2];

	double arg = 0;
	arg = slope*pt + constant; 
	double fitval = pow(arg, index)*coeff; 
	weight_pt = fitval;

	weight_nvtx = vtx_constant + vtx_slope*nvtx;

	if(eta >= 0 && eta < 1.4442){
		for(int ieta(0); ieta < 29; ieta++)
			if(eta > ieta*0.05 && eta <= (ieta+1)*0.05)weight_eta = etaRatesEB[ieta];
	}
	else if(eta > 1.56 && eta <= 2.5){
		for(int ieta(0); ieta < 94; ieta++)
			if(eta > 1.56 + ieta*0.01 && eta <= 1.56+(ieta+1)*0.01)weight_eta = etaRatesEE[ieta];
	}
	else weight_eta = 0;

	double totalfakerate= normfactor*weight_pt*weight_nvtx*weight_eta;
	return totalfakerate;
}

Double_t mcfakerate_func(Double_t *x, Double_t *par)
{
	double weight_pt(0.02);
	double weight_nvtx(0.02);
	double weight_eta(0.02);

	double normfactor = par[0];
  double slope = par[1];
  double constant = par[2];
  double index = par[3];
  double coeff = 1.0; 
	double vtx_constant = par[5];
	double vtx_slope = par[6];

  double pt = TMath::Max(x[0],0.000001);
	double nvtx=x[1];
	double eta= x[2];

	double arg = 0;
	arg = slope*pt + constant; 
	double fitval = pow(arg, index)*coeff; 
	weight_pt = fitval;

	weight_nvtx = vtx_constant + vtx_slope*nvtx;

	if(eta >= 0 && eta < 1.4442){
		for(int ieta(0); ieta < 29; ieta++)
			if(eta > ieta*0.05 && eta <= (ieta+1)*0.05)weight_eta = MCetaRatesEB[ieta];
	}
	else if(eta > 1.56 && eta <= 2.5){
		for(int ieta(0); ieta < 94; ieta++)
			if(eta > 1.56 + ieta*0.01 && eta <= 1.56+(ieta+1)*0.01)weight_eta = MCetaRatesEE[ieta];
	}
	else weight_eta = 0;

	double totalfakerate= normfactor*weight_pt*weight_nvtx*weight_eta;
	return totalfakerate;
}


//Double_t jetfake_func(Double_t *x, Double_t *par)
//{
//	double pt_low = x[0] - 1.0/2.0;
//	double pt_high = x[0]+ 1.0/2.0;
//
//	double c1 = par[0];
//	double c2 = par[1];
//	double lamda1 = par[2];
//	double lamda2 = par[3];
//
//	double jetfakes_lowedge = c1*exp(lamda1*pt_low)/lamda1 + c2*exp(lamda2*pt_low)/lamda2;
//	double jetfakes_highedge =  c1*exp(lamda1*pt_high)/lamda1 + c2*exp(lamda2*pt_high)/lamda2;
//	//return (jetfakes_highedge + jetfakes_lowedge)/2.0*REBINSIZE;
//	return (jetfakes_highedge - jetfakes_lowedge);
//}

Double_t jetfake_func(Double_t *x, Double_t *par)
{
	double pt_low = x[0] - 1.0/2.0;
	double pt_high = x[0]+ 1.0/2.0;

	double c1 = par[0];
	double c2 = par[1];
	double lamda1 = par[2];
	double lamda2 = par[3];

	double jetfakes_lowedge = c1*exp(lamda1*pt_low)/lamda1 + c2*exp(lamda2*pt_low)/lamda2;
	double jetfakes_highedge =  c1*exp(lamda1*pt_high)/lamda1 + c2*exp(lamda2*pt_high)/lamda2;
	//return (jetfakes_highedge + jetfakes_lowedge)/2.0*REBINSIZE;
	return (jetfakes_highedge - jetfakes_lowedge);
}

