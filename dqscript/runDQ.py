import csv
import subprocess

last_run = 57126

with open('BrewRunTable-KNM2-191210-143451.csv') as csv_file:
	csv_reader = csv.reader(csv_file, delimiter=',')
	line_count = 0
	for row in csv_reader:
		if line_count == 0:
			line_count += 1
			continue
		else:
			runNumber = int(row[0])
			if(runNumber >= last_run):
				continue
			subprocess.run(["./test-criteria", str(runNumber)])
			subprocess.run(["mv", "result/QualityEvaluation.ktf", "result/QualityEvaluation-"+str(runNumber)+".ktf"]) 
			line_count += 1

