#!/bin/bash
source ~/CoreCode/mainkasper/install/bin/kasperenv.sh
do
#	ktf-treedump --format=json QualityEvaluation-${irun}.ktf > tmp.json
#        python appendRun.py
	./test-criteria ${irun}
        mv QualityEvaluation.ktf  QualityEvaluation-${irun}.ktf  
done 
        #python checkQA.py >> badlist.txt
