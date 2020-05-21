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

int getCriteria(string katrinnumber){

	KTreeFormat* fFormat = new KInifileTreeFormat();
	stringstream inputstream;
	
	// Create your instance of a DataManager
	KLDataManager dataManager;
	
	vector<string> knumberlist;
	knumberlist.push_back(katrinnumber);
	
	vector<KLKatrinNumber> sensorList;
	for(const string entry: knumberlist){
		KLKatrinNumber sensor(entry);
		if (!sensor.IsValid()) {
			KERROR("Invalid Katrin Number: " << entry ); 
			return 0;
		}
	
		KLKatrinNumber sensorEntry = dataManager.GetKatrinNumber(sensor);	
		if(sensorEntry.IsNull()){
			KERROR("Unknown Katrin Number: " << entry );  
			return 0;
		}
		sensorList.push_back(sensor);
	} 	
	
	// Define our time interval
	KLTimeStamp now = KLTimeStamp::Now();
	KLTimeStamp minTime = KLTimeStamp::Min();
	KLTimeStamp maxTime = KLTimeStamp::Max();
	
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
		return 0;
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
		return 0;
	}
	
	
	return 1;
}	

int main(int argc, char** argv)
{
	ifstream infile("sensorlist.txt");
	string line;
	if(infile.is_open()){
		while ( getline(infile,line) )
    		{
			if(!getCriteria(line)){
				cout << "Failed to get " << line << " Criteria from database" << endl;
			}
		}
	}
    	infile.close();

}
