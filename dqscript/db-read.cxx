#include <string>
#include <iostream>
#include <fstream>
#include <sstream> 
#include <KTree.h>
#include <KTreeFormat.h>
#include <KTreeFile.h>
#include <KLogger.h>

#include <KLDataManager.h>
#include <KLSensor.h>
#include <KLQualityFilter.h>
#include <KLQualityCriteria.h>
#include <KLRunId.h>
#include <KLRun.h>

#include <KTree.h>
#include <KLogger.h>
#include "KInifileTreeFormat.h"

#include <KDBeans.h>
#include <KDQualityFilter.h>

#include "TCollection.h"
#include "TKey.h"
#include "TGraph.h"
#include "TStyle.h"
#include "TLine.h"

KLOGGER("kali.applications");
#define THRESHOLD 500

using namespace std;
using namespace katrin;

const int N_SUBSYSTEM = 6;
std::map<string,string> KATRINNAME = {
	{"default","UndefinedSystem/"},
	{"1XX", "REAR_SYSTEM/"},
	{"2XX", "WGTS/"},
	{"3XX", "Transport_System/"},
 		{"31X","Transport_System/DPS2-F/"},
		{"32X","Transport_System/CPS/"},
		{"322","Transport_System/CPS/FBM/"},
	{"4XX", "Spectrometer/"},
		{"41X","Spectrometer/PS/"},
		{"43X","Spectrometer/MS/"},
		{"44X","Spectrometer/MonitorSpectrometer/"},
	{"5XX", "FPD/"},
	{"6XX", "LOOP/"},
};

string getDirName(string katrin_number){
        if(katrin_number.length() < 3)
		return KATRINNAME["default"];
	string level_one_kn = katrin_number.substr(0,1);
	level_one_kn.append("XX");
        string level_two_kn = katrin_number.substr(1,1);
	level_two_kn.append("X");
	string level_three_kn = katrin_number.substr(2,1);
	if(KATRINNAME.find(level_three_kn) != KATRINNAME.end() ) { 
		return KATRINNAME[level_three_kn];
	}
	else if(KATRINNAME.find(level_two_kn) != KATRINNAME.end() ) {
		return KATRINNAME[level_two_kn];
	}
	else if(KATRINNAME.find(level_one_kn) != KATRINNAME.end() ) {
		return KATRINNAME[level_one_kn];
	}
	else{
		return KATRINNAME["default"];
	}
}

int getCriteria(string runNumber, string configName){

	KTreeFormat* fFormat = new KInifileTreeFormat();
	stringstream inputstream;
	
	// Create your instance of a DataManager
	KLDataManager dataManager;
	
	vector<string> knumberlist;
	knumberlist.push_back("521-RPY-0-1240-0001");
	
	vector<KLKatrinNumber> sensorList;
	for(const string entry: knumberlist){
		KLKatrinNumber sensor(entry);
		if (!sensor.IsValid()) {
			KERROR("Invalid Katrin Number: " << entry ); 
			return -1;
		}
	
		KLKatrinNumber sensorEntry = dataManager.GetKatrinNumber(sensor);	
		if(sensorEntry.IsNull()){
			KERROR("Unknown Katrin Number: " << entry );  
			return -1;
		}
		sensorList.push_back(sensor);
	} 	
	
	// Define our time interval
	KLTimeStamp now = KLTimeStamp::Now();
	KLTimeStamp minTime = KLTimeStamp::Max();
	KLTimeStamp maxTime = KLTimeStamp::Min();
	
	KLRunId runIDentifier(runNumber);
	if (runIDentifier.IsValid()) {
		KLRun* run = dataManager.GetRun(runIDentifier);
		KLTimeStamp runStart = run->GetTimeInterval().GetStart();
		KLTimeStamp runEnd = run->GetTimeInterval().GetEnd();
	
		if ((minTime == now) || (runStart < minTime)){
			minTime = runStart;
		}
		if ((maxTime == now) || (runEnd > maxTime)){
			maxTime = runEnd;
		}
	}
	if (minTime >= maxTime){
		maxTime = minTime = now;
	}
	KLTimeInterval timeInterval( minTime, maxTime );

	// Create a filter for our database query:
	KLDatabaseFilter dbFilter;
	dbFilter.SetKatrinNumberList( sensorList );
	dbFilter.SetOnlyMostRecentEntry(true);
	dbFilter.SetKatrinNumbersConsiderSubType(false);
	dbFilter.SetValidityInterval( timeInterval );
	
	// Request database items
	KLKatrinNumberList knData = dataManager.GetKatrinNumberList(dbFilter);
	
	if (knData.size() == 0) {
		KERROR("Found 0 entry in the database.");
	}
	else {
		KINFO("Found " << knData.size() << " matching Katrin numbers:");
	}
	
	// Create a filter for our quality criteria query:
	KLQualityFilter qcFilter(dbFilter);
	
	// Request quality criteria data
	KLQualityCriteriaList qcData = dataManager.GetQualityCriteriaList( qcFilter );
	
	if (qcData.size() == 0) {
		KERROR("Failed to look up QualityCriteria entries!");
		return -1;
	}
	
	ofstream configfile;
	configfile.open(configName);
	if(configfile.is_open()){
		KTree outputTree;
		for (const KLQualityCriteria* entry : qcData ) {
			inputstream.str("");
			inputstream << entry->GetCriteria() << endl;
			stringstream criteriaName;
			criteriaName.str("");
			criteriaName << entry->GetKatrinNumber();
			if(!(entry->GetCriteriaName()).empty())criteriaName << "-" << entry->GetCriteriaName();

			istringstream ss(inputstream.str());
			KTree tree;
			fFormat->Read(tree, ss);
			vector<KTree*> valuelist = tree.ChildNodeList();
	
			KTree& currentNode = outputTree.AppendNode("QualityAnalysis");
			currentNode["Name"] = criteriaName.str();
			for(unsigned i(0); i < valuelist.size(); i++){
				KTree& tmptree = currentNode["TimeSeriesProcessingChain"].AppendNode("Processor");
				tmptree["Type"] = valuelist[i]->NodeName(); 
				tmptree["Parameter"] = *(valuelist[i]);	
			}
		}
		KKtfTreeSerializer(configfile).Serialize(outputTree["/"]);
		
		return 1;
	}
	else{
		std::cout << "Error opening file";
		return -1;
	}
	
	return 0;
}	

int main(int argc, char** argv)
{
	KArgumentList args(argc, argv);
	string criteriaFilePath = args["--criteria-file-path"].Or("tmp/Data");	
	string outputFilePath = args["--output-file-path"].Or(".");
	string runID = args["--runID"].Or(args[0]);

	ostringstream qasum;
	qasum.str("");
	ostringstream qaplot;
	qaplot.str("");
	qasum  << outputFilePath + "/QualityEvaluation-" << runID << ".json";
	qaplot << outputFilePath + "/DQM_" << runID << ".root";
    
	string qafilename = criteriaFilePath + "/QualityCriteria_KNM2.ktf";
	if(!getCriteria(runID, qafilename)){
		cout << "Failed to get Criteria from database" << endl;
		return -1;
	}

	KDBeans beans;

	(beans
		.Append((new KDLocalFileQualityEvaluation())
			->SetFile(qafilename)
			->StoreReadoutTimeSeries()
			->SetCutThreshold(THRESHOLD)	
		)

	);
 
	beans.Build(argc, argv).Start();

	const KTree& qareport = beans.GetReport()["Run[0]/QualityEvaluation"];
	ofstream of3(qasum.str().c_str());
	of3 << setformat("json") << qareport << endl;   

	gStyle->SetOptStat(0);
	TFile *file = new TFile(qaplot.str().c_str(), "recreate");
	for(std::map<string,string>::iterator it = KATRINNAME.begin(); it != KATRINNAME.end(); it++){
		file->mkdir((it->second).c_str());
	} 
	std::vector<string> histnamelist = beans.GetTimeSeriesNameList();

	float run_length = beans.GetReport()["Run[0]/QualityEvaluation/RunLength"];
	std::vector< pair<float,float> > badperiods[N_SUBSYSTEM];
	float sensor_freq[N_SUBSYSTEM];
	for(unsigned i(0); i < N_SUBSYSTEM; i++){
		badperiods[i].clear();
		sensor_freq[i] = 1E6;
	}

	for(auto& name:histnamelist){
		auto* ts = beans.FindTimeSeries(name);
		if(name.find("Input") != std::string::npos)continue;
		bool isReadoutGraph = (name.find("Readout") != std::string::npos);
		string dirname = getDirName(name);
		int system_index = name[0] - '0' - 1;
		float run_end   = ts->end;
		if(system_index >= 0 && system_index < N_SUBSYSTEM){
			if(run_end/(*ts).x.size() < sensor_freq[system_index])
				sensor_freq[system_index] = run_end/(*ts).x.size();
		} 
		file->cd(dirname.c_str());
		TGraph* g = new TGraph();
		g->SetName(name.c_str());
		g->SetTitle(name.c_str());
		float last_quality = 0;
		float start_badtime = 0;
		float end_badtime = 0;
		for (unsigned i = 0; i < (*ts).x.size(); i++) {
			g->SetPoint(i, (*ts).t[i], (*ts).x[i]);
			if(isReadoutGraph || system_index < 0 || system_index >= N_SUBSYSTEM)continue;
			if(last_quality < THRESHOLD && (*ts).x[i] > THRESHOLD){
				start_badtime = (*ts).t[i];
				last_quality = (*ts).x[i];
			}
			else if((last_quality > THRESHOLD && (*ts).x[i] < THRESHOLD) || (last_quality > THRESHOLD && i == (*ts).x.size()-1)){
				end_badtime = (*ts).t[i];
				badperiods[system_index].push_back(make_pair(start_badtime, end_badtime));        
				last_quality = (*ts).x[i]; 
			}
		}
		g->Write();
	}
	file->Write();
	file->Close();


	TCanvas *can = new TCanvas("Sub_system Quality","Sub_system Quality",1200,1800);
	can->Divide(2,3);
	TH2F *dummy[N_SUBSYSTEM];
	TGraph *summary_good[N_SUBSYSTEM];
	TGraph *summary_bad[N_SUBSYSTEM];
	for(unsigned isys(0); isys < N_SUBSYSTEM; isys++){
		int np = 1;
		if(sensor_freq[isys] < run_length){
			np = int(run_length/sensor_freq[isys]);
		}
		string tmpname = to_string(isys+1) + "XX"; 
		string name = getDirName(tmpname) + "Summary";
		dummy[isys] = new TH2F(name.c_str(), (name + ";time(s);").c_str(), np, 0, run_length, 2,-0.5,1.5);
		summary_good[isys] = new TGraph();
		summary_bad[isys] = new TGraph();
		for(unsigned ip(0); ip < np; ip++){
			float x_value = ip*sensor_freq[isys];
			int   y_value = 0;
			for(unsigned it(0); it < badperiods[isys].size(); it++){
				if(x_value > badperiods[isys][it].first && x_value < badperiods[isys][it].second){
					y_value = 1;
					break;
				}
			}
			if(y_value < THRESHOLD)summary_good[isys]->SetPoint(summary_good[isys]->GetN(), x_value, y_value);
			else if(y_value > THRESHOLD)summary_bad[isys]->SetPoint(summary_bad[isys]->GetN(), x_value, y_value);
		}
		can->cd(isys+1);
		dummy[isys]->GetYaxis()->SetBinLabel(1,"GOOD");
		dummy[isys]->GetYaxis()->SetBinLabel(2,"Bad");
		dummy[isys]->Draw();
		if(np > 1){
			summary_good[isys]->SetMarkerColor(kGreen);
			summary_bad[isys]->SetMarkerColor(kRed);
			summary_good[isys]->SetMarkerStyle(20);
			summary_bad[isys]->SetMarkerStyle(20);
			if(summary_good[isys]->GetN() > 0)summary_good[isys]->Draw("P same");
			if(summary_bad[isys]->GetN() > 0)summary_bad[isys]->Draw("P same");
		} 
	} 

	can->SaveAs((outputFilePath + "/" + "QualityEvaluation-SubSystem" + "-" + runID + ".png").c_str());

	TCanvas *can_all = new TCanvas("Quality Summary","Quality Summary",600,600);
	can_all->cd();
	TH2F *dummy_all = new TH2F("Quality Summary","Quality Summary", 10, 0, run_length, 2,-0.5,1.5);
	TGraph *g_all = new TGraph();
	float last_timestamp(0);
	for(unsigned i(0); i < qareport["BadPeriod"].Length(); i++){
		auto& qa = qareport["BadPeriod"][i];
		float start_badtime = qa["Start"];
		float end_badtime = qa["End"];
		g_all->SetPoint(g_all->GetN(), last_timestamp, 0);
		g_all->SetPoint(g_all->GetN(), start_badtime, 0);
		g_all->SetPoint(g_all->GetN(), start_badtime, 1);
		g_all->SetPoint(g_all->GetN(), end_badtime, 1);
		last_timestamp = end_badtime; 
	}
	if(last_timestamp < run_length){
		g_all->SetPoint(g_all->GetN(), last_timestamp, 0);
		g_all->SetPoint(g_all->GetN(), run_length, 0); 
	}
	dummy_all->GetYaxis()->SetBinLabel(1,"GOOD");
	dummy_all->GetYaxis()->SetBinLabel(2,"Bad");
	dummy_all->Draw();
	g_all->SetLineWidth(2);
	g_all->Draw("same");
	can_all->SaveAs((outputFilePath + "/" + "QualityEvaluation-Summary" + "-" + runID + ".png").c_str());
	
	return 0;
}
