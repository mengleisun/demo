import sys
from argparse import ArgumentParser
import pandas as pd
import numpy as np
import math
from collections import defaultdict
import re
import csv

alert_level = {
	'GREEN':1,
	'YELLOW':2,
}

pattern_cuttype = {
	'KATRIN':['cut type'],
	'RunType':['run type'],
	'ValueAlert':['up and low','min and max','valuealert'],
	'MovingAverage':['trend','mov aver'],
	'Dependence':['dependence'],
	'GapAlert':['gap'],
	'InvalidValueRemoval':['valid']
}	

pattern_parameter = {
	'KATRIN NUMBER':['katrin number','katrin #'],
	'DESCRIPTION':['description','describe'],
	'CRITERIANAME':['criteria'],
	'INCLUDERUN':['run include'],
	'EXCLUDERUN':['run exclude'],
	'GREEN_Lower':['green min','green low'],
	'GREEN_Upper' :['green max','green up','green high'],
	'YELLOW_Lower':['yellow min','yellow " low"'],
	'YELLOW_Upper':['yellow max','yellow up','yellow high'],
	'Hysteresis':['hysteresis'],
	'DEPENDENCE':['depend'],
	'WindowLength':['averag time','averag length','mov averag'],
	'GREEN_Length':['gap green'],
	'YELLOW_Length':['gap yellow'],
	'VALID_Lower':['valid min','valid reading min','valid low'],
	'VALID_Upper':['valid max','valid up']
}

map_cuttype_parameter = {
	'KATRIN':['KATRIN NUMBER','DESCRIPTION','CRITERIANAME'],
	'Dependence':['DEPENDENCE'],
	'RunType':['INCLUDERUN','EXCLUDERUN'],
	'InvalidValueRemoval':['VALID_Lower','VALID_Upper'],
	'MovingAverage':['WindowLength'],
	'ValueAlert':['GREEN_Lower','GREEN_Upper','YELLOW_Lower','YELLOW_Upper','Hysteresis'],
	'GapAlert':['GREEN_Length','YELLOW_Length']
}

def isvalidkanumber(kanumber):
	if re.match("^[0-9]{3}-[A-Z]{3}-[0-9]-[0-9]{4}-[0-9]{4}$",kanumber):
		return True
	else:
		return False


def writeHeader(outputfile, header):
	outputfile.write("#QualityAnalysis:\n")
	outputfile.write("#  Name: %s-Quality\n"%(header))
	outputfile.write("#  TimeSeriesProcessingChain:\n") 
	outputfile.write("#    Processor:\n") 
	outputfile.write("#      Type: SlowControlReadout\n")
	outputfile.write("#      Parameter:\n")
	outputfile.write("#        KatrinNumber: %s\n"%(header))

def writeValueLimit(outputfile, cuts):
	for _color in alert_level.keys():
		lower_key =  _color+'_LOW'
		upper_key =  _color+'_UP'
		if lower_key in cuts.keys() or upper_key in cuts.keys():
			alert_value = alert_level[_color]
			outputfile.write("#        Limit:\n")
			outputfile.write("#          AlertLevel:%d\n"%(alert_value))
			if lower_key in cuts.keys():
				outputfile.write("#          Lower: %s\n"%(cuts[lower_key]))
			else:
				outputfile.write("#          Lower: nan\n")

			if upper_key in cuts.keys():
				outputfile.write("#          Upper: %s\n"%(cuts[upper_key]))
			else:
				outputfile.write("#          Upper: nan\n")

			if 'Hysteresis' in cuts.keys():
				outputfile.write("#          Hysteresis: %f\n"%(cuts['Hysteresis']))
			else:
				outputfile.write("#          Hysteresis: 0\n")
		else:
			outputfile.write("#        Limit:\n")
			outputfile.write("#          AlertLevel:%d\n"%(-1))
			outputfile.write("#          Lower: nan\n")
			outputfile.write("#          Upper: nan\n")
			outputfile.write("#          Hysteresis: nan\n")
			 
def writeValidRange(outputfile, cuts):
		lower_key =  'VALID_Lower'
		upper_key =  'VALID_Upper'
		outputfile.write("#        InvalidRange:\n")
		if lower_key in cuts.keys():
			outputfile.write("#          Lower: %s\n"%(cuts[lower_key]))
		else:
			outputfile.write("#          Lower: nan\n")

		if upper_key in cuts.keys():
			outputfile.write("#          Upper: %s\n"%(cuts[upper_key]))
		else:
			outputfile.write("#          Upper: nan\n")

def writeGapLimit(outputfile, cuts):
	if len(cuts) > 0:
		outputfile.write("#    Processor:\n")
		outputfile.write("#      Type: GapInMiddleAlert\n")
		outputfile.write("#      Parameter:\n")
	for _color in alert_level.keys():
		_key =  'GAP_' + _color
		if _key in cuts.keys():
			alert_value = alert_level[_color]
			outputfile.write("#        Limit:\n")
			outputfile.write("#          AlertLevel:%d\n"%(alert_value))
			outputfile.write("#          Length: %s\n"%(cuts[_key]))
	if len(cuts) > 0:
		outputfile.write("#    Processor:\n")
		outputfile.write("#      Type: GapAtBoundaryAlert\n")
		outputfile.write("#      Parameter:\n")
	for _color in alert_level.keys():
		_key =  'GAP_' + _color
		if _key in cuts.keys():
			alert_value = alert_level[_color]
			outputfile.write("#        Limit:\n")
			outputfile.write("#          AlertLevel:%d\n"%(alert_value))
			outputfile.write("#          Length: %s\n"%(cuts[_key]))

def writeProcessor(outputfile, criteria):
	for processor in map_cuttype_parameter:
		if processor == "KATRIN":
			continue
		if processor in criteria.keys():
			outputfile.write("#    Processor:\n")
			outputfile.write("#      Type: %s\n"%(processor))
			outputfile.write("#      Parameter:\n")
		if processor == "ValueAlert":
			writeValueLimit(outputfile, criteria[processor])
		elif processor == "InvalidValueRemoval":
			continue
			writeValidRange(outputfile, criteria[processor])
		elif processor == "GapAlert": 
			writeGapLimit(outputfile, criteria[processor])
		else:		
			for i in criteria[processor]:
				outputfile.write("#        %s: %s\n"%(i, criteria[processor][i]))

def writeCSV(csvwriter, criteria):
	datarow = {}
	for processor in map_cuttype_parameter:
		for cut in map_cuttype_parameter[processor]:
			if processor in criteria.keys() and cut in criteria[processor].keys():
				datarow[cut] = criteria[processor][cut]
			else:
				datarow[cut] = ""	
	csvwriter.writerow(datarow)
				

def to_standard_colname(lst, pattern_map):
	stand_lst = []
	for item in lst:
		is_standard = False
		for _key,_patterns in pattern_map.items():
			for _kwords in _patterns:
				if all(_word in item.lower() for _word in _kwords.split()):
					stand_lst.append(_key)
					is_standard = True
					break
		if not is_standard:
			print("Warning: Unknown type:",item)
			i_unknown = sum('UNKNOWN' in _word for _word in stand_lst)	
			stand_lst.append('UNKNOWN'+str(i_unknown))
	return stand_lst

def set_cuttype(para_lst, type_lst):
	stand_type_lst = []
	map_para_cut = {}
	#switch the key and values of cuttype_parameter map
	for _key,_values in map_cuttype_parameter.items():
		for i in _values:
			map_para_cut[i] = _key
	
	for i in range(0,len(para_lst)):
		if para_lst[i] in map_para_cut:
			if len(type_lst) < i+1:
				stand_type_lst.append(map_para_cut[para_lst[i]])
			elif type_lst[i] != map_para_cut[para_lst[i]]:
				print("Warning: the processor type of the parameter %s is wrong. Set to %s"%(para_lst[i], map_para_cut[para_lst[i]]))
				stand_type_lst.append(map_para_cut[para_lst[i]])
			else:
				stand_type_lst.append(type_lst[i])
		else:
			i_unknown = sum('UNKNOWN' in _word for _word in stand_type_lst)	
			stand_type_lst.append('UNKNOWN'+str(i_unknown))
	return stand_type_lst	
			
 
if __name__ == '__main__':

	parser = ArgumentParser(description='Read quality criteria from files and fill into database.')
	parser.add_argument('-f','--file',type=str,help='path and name of the input file')
	parser.add_argument('--writeCSV',action='store_true', help='Create a .csv file as output')
	parser.add_argument('--writeKTF',action='store_true', help='Create a .ktf file as output')
	parser.add_argument('--dry_run',action='store_true', help='Do not fill into database')
	args = parser.parse_args()

	if not args.file:
		print('No input file specified. \nExit the script.')
		sys.exit() 

	dqfile = pd.read_excel(args.file)

	#Read validity date and entry date
	#valid_date = 
	#entry_date = 
	
	#Skip rows marked as comments
	indexComment = dqfile[dqfile.iloc[:,0].str.contains('^#') == True].index
	dqfile.drop(indexComment , inplace=True)
	dqfile.reset_index(drop=True, inplace=True)

	#Read the processor types	
	cuttype_rowid = dqfile.apply(lambda x:x.str.contains("cut type",case=False)).any(axis=1).idxmax()
	cuttype_names = []
	if cuttype_rowid:
		cuttype_names = to_standard_colname(dqfile.iloc[cuttype_rowid,:].fillna(axis=0, method='ffill').tolist(), pattern_cuttype)

	#Read the names of parameters	
	kanumber_rowid = dqfile.apply(lambda x:x.str.contains("katrin number",case=False)).any(axis=1).idxmax()
	if not kanumber_rowid:
		sys.exit("Input error: No row containing column names is found. Check your xlsx file. The row should start with 'KATRIN number'.")
	paratype_names = to_standard_colname(dqfile.iloc[kanumber_rowid,:].tolist(), pattern_parameter)
	#Check if the processor types are correctly assigned.
	cuttype_names = set_cuttype(paratype_names, cuttype_names) 
	
	#Group the processor type and parameters
	arrays = [cuttype_names, paratype_names]
	tuples = list(zip(*arrays))
	multi_index = pd.MultiIndex.from_tuples(tuples)

	#Read the data and convert the dataframe into a list of dictionaries
	data_startrow = kanumber_rowid + 1	
	criteria_block = dqfile.iloc[data_startrow:len(dqfile),:].copy()
	criteria_block.columns = multi_index 
	data_entries = criteria_block.to_dict(orient='record')
	
	nrow = len(data_entries)
	quality_criteria = [defaultdict(dict) for _ in range(nrow)]
	for i,entry in enumerate(data_entries):
		for (key1,key2) in entry.keys():
			if not pd.isnull(entry[(key1,key2)]):
				quality_criteria[i][key1][key2] = entry[(key1,key2)]

	if not args.dry_run:
		if args.writeCSV:
			csvoutput = open("QualityCriteria.csv","w")
			csv_columns = []
			for processor in map_cuttype_parameter:
				for cut in map_cuttype_parameter[processor]:
					csv_columns.append(cut)	
			csvwriter = csv.DictWriter(csvoutput, fieldnames=csv_columns)
			csvwriter.writeheader()
		
			for sensor in quality_criteria:
				k_number = sensor['KATRIN']['KATRIN NUMBER']
				if isvalidkanumber(k_number):
					writeCSV(csvwriter, sensor)
				else:
					print("The KATRIN number " + k_number + " is not valid. Skip this sensor.")
				
		if args.writeKTF:	
			for sensor in quality_criteria:
				k_number = sensor['KATRIN']['KATRIN NUMBER']
				if isvalidkanumber(k_number):
					if 'Dependence' not in sensor.keys():
						output = open("QA-%s.ktf"%(sensor['KATRIN']['KATRIN NUMBER']),"w")
						writeHeader(output, sensor['KATRIN']['KATRIN NUMBER'])
						writeProcessor(output,sensor)
				else:
					print("The KATRIN number " + k_number + " is not valid.")
						
	else:
		for sensor in quality_criteria:
			k_number = sensor['KATRIN']['KATRIN NUMBER']
			writeHeader(sys.stdout, sensor['KATRIN']['KATRIN NUMBER'])
			writeProcessor(sys.stdout, sensor)
