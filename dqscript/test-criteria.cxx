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

string KAname = "436-EHV-0-1003-0002";


int getCriteria(char* runNumber, string configName){

	KTreeFormat* fFormat = new KInifileTreeFormat();
	stringstream inputstream;

        string ininame = KAname+".ini";
	std::ifstream file(ininame);
        if (file)
	{
		inputstream << file.rdbuf();
        	file.close();
    	}
        std::cout << inputstream.str() << std::endl;
	
	ofstream configfile(configName);
	KTree outputTree;
	
	    istringstream ss(inputstream.str());
	    KTree tree;
	    fFormat->Read(tree, ss);
	    vector<KTree*> valuelist = tree.ChildNodeList();

	    KTree& currentNode = outputTree.AppendNode("QualityAnalysis");
            currentNode["Name"] = KAname; 
	    for(unsigned i(0); i < valuelist.size(); i++){
		if(valuelist[i]->NodeName() == "QualityCriteria")continue;
		KTree& tmptree = currentNode["TimeSeriesProcessingChain"].AppendNode("Processor");
		tmptree["Type"] = valuelist[i]->NodeName(); 
		tmptree["Parameter"] = *(valuelist[i]);	
	    }
	KKtfTreeSerializer(configfile).Serialize(outputTree["/"]);
		
	return 1;
}	

int main(int argc, char** argv)
{
	char* runId;
	runId = argv[1];
	
	string qafilename = "result/qa-config.ktf";
	if(!getCriteria(runId, qafilename)){
		cout << "Failed to get Criteria from database" << endl;
		return -1;
	}
	KDBeans beans;

	(beans

		.Append((new KDLocalFileQualityEvaluation())
		->SetFile(qafilename)
		->StoreReadoutTimeSeries()
		->SetCutThreshold(999)
		)

		.Append(new KDTimeSeriesDraw())
		.Append(new KDQualityEvaluationDrawDecoration())

		.Append(new KDDraw())
		.SetOutput("result/DQM.root")
	);
 
	beans.Build(argc, argv).Start();

	const KTree& qareport = beans.GetReport()["Run[0]/QualityEvaluation"];
	ofstream of3("result/QualityEvaluation.ktf");
	of3 << setformat("ktf") << qareport << endl;   

	return 0;
}
