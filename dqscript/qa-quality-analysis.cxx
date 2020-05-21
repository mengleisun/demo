// qa-quality-analysis.cxx //
// Author: Sanshiro Enomoto <sanshiro@uw.edu> //

#include <fstream>
#include <KDBeans.h>
#include <KDQualityFilter.h>

#include <KLDataManager.h>

using namespace std;
using namespace katrin;


int main(int argc, char** argv)
{
    KDBeans beans;

    //beans.AddInput("fpd009789");  // good run
    //beans.AddInput("fpd009790");  // 2013 Christmas-Toast run
    //KLDataManager::GetInstance().SetUrl("http://kdb-orig.kaas.kit.edu/kdb-kali.fcgi");

    (beans
        .Append((new KDBasicQualityAnalysis())
            ->SetName("FpdTemperatureQuality")
            ->SetKatrinNumber("527-RTP-6-1285-0001")
            ->SetContextLength(300)
            ->SetMovingAverageLength(100)
            ->SetHysteresis(0)
            ->AddValueLimits(/*AlertLevel*/20, /*Lower*/-55, /*Upper*/-50)
            ->AddValueLimits(/*AlertLevel*/50, /*Lower*/-57, /*Upper*/-48)
            ->AddGapLengthLimit(/*AlertLevel*/10, /*Length*/30)
            ->SetAlertTimeExtensionLength(60)
            ->StoreIntermediateTimeSeries()
        )
        .Append(new KDTimeSeriesListDump())
        .Append((new KDTimeSeriesDraw())
            ->AddInputNameExclusionRegex("(.*Alert.*)|(FpdTemperatureQuality)")
            ->AddYAxisLineMarking(-55)
            ->AddYAxisLineMarking(-50)
        )
        .Append((new KDTimeSeriesDraw())
            ->AddInputNameRegex("(.*Alert.*)|(FpdTemperatureQuality)")
        )
    );
    
    beans.Build(argc, argv).Start();

    KTree criteriaTree = beans.Find<KDQualityAnalysis>()->GetCriteriaAsTree();
    ofstream of("QualityCriteria.ktf");
    of << setformat("json") << criteriaTree << endl;

    return 0;
}
