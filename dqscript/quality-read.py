import sys
from argparse import ArgumentParser
import pandas as pd
import numpy as np
import math
import re
from datetime import datetime
import json
import subprocess

period_name = "KNM1"

alert_level = {
	'GREEN':1,
	'YELLOW':2,
}

pattern_parameter = {
	'Katrin Number':{'pattern':['katrin number','katrin #'],'path':['QualityCriteria/KatrinNumber','SlowControlReadout/KatrinNumber']},
	'Description':{'pattern':['description','describe'],'path':['QualityCriteria/Description']},
	'CriteriaName':{'pattern':['criteria'],'path':['CriteriaName']},
	'IncludeRun':{'pattern':['run include'],'path':['IncludeRun']},
	'ExcludeRun':{'pattern':['run exclude'],'path':['ExcludeRun']},
	'GREEN_Lower':{'pattern':['green min','green low'],'path':['ValueAlert/Limit[GREEN]/Lower']},
	'GREEN_Upper' :{'pattern':['green max','green up','green high'],'path':['ValueAlert/Limit[GREEN]/Upper']},
	'YELLOW_Lower':{'pattern':['yellow min','yellow " low"'],'path':['ValueAlert/Limit[YELLOW]/Lower']},
	'YELLOW_Upper':{'pattern':['yellow max','yellow up','yellow high'],'path':['ValueAlert/Limit[YELLOW]/Upper']},
	'Hysteresis':{'pattern':['hysteresis'],'path':["ValueAlert/Limit[GREEN]/Hysteresis","ValueAlert/Limit[YELLOW]/Hysteresis"]},
	'Dependence':{'pattern':['depend'],'path':['Dependence']},
	'WindowLength':{'pattern':['averag time','averag length','mov averag'],'path':['MovingAverage/Length']},
	'GREEN_Length':{'pattern':['gap green'],'path':['GapAlert']},
	'YELLOW_Length':{'pattern':['gap yellow'],'path':['GapAlert']},
	'VALID_Lower':{'pattern':['valid min','valid reading min','valid low'],'path':['InvalidValueRemoval/Lower']},
	'VALID_Upper':{'pattern':['valid max','valid up'],'path':['InvalidValueRemoval/Upper']},
}

def isvalidkanumber(kanumber):
	if re.match("^[0-9]{3}-[A-Z]{3}-[0-9]-[0-9]{4}-[0-9]{4}$",kanumber):
		return True
	else:
		return False


def createProcessor(criteria, validdate):
	quality_criteria = {}
	#quality_criteria[i]["Name"] = period_name
	quality_criteria["QualityCriteria"] = {}
	quality_criteria["QualityCriteria"]["ValidFrom"] = validdate
	for parname in criteria:
		if pd.isnull(ientry[parname]):
			continue 
		if parname not in pattern_parameter:
			continue

		for ipath in pattern_parameter[parname]["path"]:
			parpath = ipath.split("/")
			currentlayer = quality_criteria;
			for key_name in parpath[:-1]:
				if "[" in key_name and "]" in key_name:
					color_name = re.search('\[([^]]+)',key_name).group(1)
					key_name = key_name.split("[")[0] 
					if key_name not in currentlayer:
						currentlayer[key_name] = []
					currentlayer = currentlayer[key_name]

					if color_name not in alert_level:
						continue 
					color_value = alert_level[color_name] 
					for ilimit in currentlayer:
						if ilimit["AlertLevel"] == color_value:
							currentlayer = ilimit
					if not (isinstance(currentlayer, dict)): 
						currentlayer.append({"AlertLevel": color_value})
						currentlayer = currentlayer[-1]
				else:
					if key_name not in currentlayer:
						currentlayer[key_name] = {}
					currentlayer = currentlayer[key_name]
			if(isinstance(currentlayer, dict)):			
				currentlayer[parpath[-1]] = ientry[parname]
	return quality_criteria
			 
def to_standard_colname(lst, pattern_map):
	stand_lst = []
	for item in lst:
		is_standard = False
		for _key,_patterns in pattern_map.items():
			for _kwords in _patterns['pattern']:
				if all(_word in item.lower() for _word in _kwords.split()):
					stand_lst.append(_key)
					is_standard = True
					break
		if not is_standard:
			print("Warning: Unknown type:",item)
			i_unknown = sum('UNKNOWN' in _word for _word in stand_lst)	
			stand_lst.append('UNKNOWN'+str(i_unknown))
	return stand_lst
 
if __name__ == '__main__':

	parser = ArgumentParser(description='Read quality criteria from files and fill into database.')
	parser.add_argument('-f','--file',type=str,help='path and name of the input file')
	parser.add_argument('--dry_run',action='store_true', help='Do not fill into database')
	args = parser.parse_args()

	if not args.file:
		print('No input file specified. \nExit the script.')
		sys.exit() 

	dqfile = pd.read_excel(args.file)
	
	#Skip rows marked as comments
	indexComment = dqfile[dqfile.iloc[:,0].str.contains('^#') == True].index
	dqfile.drop(indexComment , inplace=True)
	dqfile.reset_index(drop=True, inplace=True)

	#Read validity date and entry date
	date_rowid = dqfile.apply(lambda x:x.str.contains("date",case=False)).any(axis=1).idxmax()
	valid_dates = [date for date in  dqfile.iloc[date_rowid,:] if isinstance(date, datetime)]
	valid_date = (valid_dates[0] if len(valid_dates) > 0 else datetime.utcnow()).strftime("%Y-%m-%d") 
	print(valid_date)
	#entry_date = 

	#Read the names of parameters	
	kanumber_rowid = dqfile.apply(lambda x:x.str.contains("katrin number",case=False)).any(axis=1).idxmax()
	if not kanumber_rowid:
		sys.exit("Input error: No row containing column names is found. Check your xlsx file. The row should start with 'KATRIN number'.")
	paratype_names = to_standard_colname(dqfile.iloc[kanumber_rowid,:].tolist(), pattern_parameter)

	#Read the data and convert the dataframe into a list of dictionaries
	data_startrow = kanumber_rowid + 1	
	criteria_block = dqfile.iloc[data_startrow:len(dqfile),:].copy()
	criteria_block.columns = paratype_names 
	data_entries = criteria_block.to_dict(orient='record')

	for ientry in data_entries:
		k_number = ientry['Katrin Number']
		if isvalidkanumber(k_number):
			quality_criteria = createProcessor(ientry, valid_date)
			if not args.dry_run:
				outputfile = open("QualityCriteria.json","w")
				outputfile.write(json.dumps(quality_criteria))
				outputfile.close()
				inifile = open(k_number+".ini", "w")
				subprocess.run(["ktf-treedump", "--format=ini", outputfile.name], stdout=inifile)
				CMD = "awk '{if (/=/) print $0\";\"; else print $0}' " + inifile.name + " > temp.ini; mv temp.ini " + inifile.name
				subprocess.run(CMD, shell=True) 
			else:
				print(json.dumps(quality_criteria))
		else:
			print("The KATRIN number " + k_number + " is not valid.")
