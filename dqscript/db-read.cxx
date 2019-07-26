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

KLOGGER("kali.applications");

using namespace std;
using namespace katrin;

int getCriteria(char* runNumber, string configName){

	KTreeFormat* fFormat = new KInifileTreeFormat();
	stringstream inputstream;

	// Create your instance of a DataManager
	KLDataManager dataManager;
	
	vector<string> knumberlist;
	knumberlist.push_back("322-MCH-0-3110-0001");
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

	KLRunId runIdentifier(runNumber);
	if (runIdentifier.IsValid()) {
		KLRun* run = dataManager.GetRun(runIdentifier);
		KLTimeStamp runStart = run->GetTimeInterval().GetStart();
		KLTimeStamp runEnd = run->GetTimeInterval().GetEnd();
	
		if ((minTime == now) || (runStart < minTime))
		minTime = runStart;
		if ((maxTime == now) || (runEnd > maxTime))
		maxTime = runEnd;
	}
	
	if (minTime >= maxTime)
	    maxTime = minTime = now;
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
	
	ofstream configfile(configName);
	KTree outputTree;
	for (const KLQualityCriteria* entry : qcData ) {
	    inputstream.str("");
	    inputstream << entry->GetCriteria() << endl;
	
	    istringstream ss(inputstream.str());
	    KTree tree;
	    fFormat->Read(tree, ss);
	    vector<KTree*> valuelist = tree.ChildNodeList();

	    KTree& currentNode = outputTree.AppendNode("QualityAnalysis");
	    for(unsigned i(0); i < valuelist.size(); i++){
		KTree& tmptree = currentNode["TimeSeriesProcessingChain"].AppendNode("Processor");
		tmptree["Type"] = valuelist[i]->NodeName(); 
		tmptree["Parameter"] = *(valuelist[i]);	
	    }
	}
	KKtfTreeSerializer(configfile).Serialize(outputTree["/"]);
		
	return 1;
}	

int main(int argc, char** argv)
{
	char* runId;
	runId = argv[1];
	
	string qafilename = "qa-config.ktf";
	if(!getCriteria(runId, qafilename)){
		cout << "Failed to get Criteria from database" << endl;
		return -1;
	}
	KDBeans beans;

	(beans
		.Append((new KDLocalFileQualityEvaluation())
		->SetFile(qafilename)
		->StoreReadoutTimeSeries()
		->SetCutThreshold(0.5)
		)

		.Append(new KDTimeSeriesDraw())
		.Append(new KDQualityEvaluationDrawDecoration())

		.Append(new KDDraw())
		.SetOutput("DQM.root")
	);
 
	beans.Build(argc, argv).Start();

	const KTree& qareport = beans.GetReport()["Run[0]/QualityEvaluation"];
	ofstream of3("QualityEvaluation.ktf");
	of3 << setformat("ktf") << qareport << endl;   

	return 0;
}
