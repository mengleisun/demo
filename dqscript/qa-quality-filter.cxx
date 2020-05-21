// qa-quality-filter.cxx //
// Author: Sanshiro Enomoto <sanshiro@uw.edu> //

#include <KDBeans.h>
#include <KDQualityFilter.h>
#include <fstream>

#include <KLDataManager.h>

using namespace std;
using namespace katrin;


int main(int argc, char** argv)
{
    KDBeans beans;

    (beans
        .Append((new KDLocalFileQualityEvaluation())
            ->SetFile("QA_KNM1.ktf")
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
