import sys
from argparse import ArgumentParser
import pandas as pd
import numpy as np
import math
import re
from datetime import datetime
import json
import subprocess

period_name = "KNM2"

alert_level = {
	'GREEN':1000,
	'YELLOW':2000,
}

keyorder = ['Katrin Number', 'Description', 'CriteriaName', 'IncludeRun', 'ExcludeRun', 'Dependence', 'VALID_Lower', 'VALID_Upper', 'WindowLength', 'GREEN_Lower', 'GREEN_Upper', 'YELLOW_Lower', 'YELLOW_Upper', 'Hysteresis', 'GREEN_Grace', 'GREEN_Length', 'YELLOW_Length','UNKNOWN0']

pattern_parameter = {
	'Katrin Number':{'pattern':['katrin number','katrin #'],'path':['QualityCriteria/KatrinNumber','SlowControlReadout/KatrinNumber']},
	'Description':{'pattern':['description','describe'],'path':['QualityCriteria/Description']},
	'CriteriaName':{'pattern':['criteria'],'path':['QualityCriteria/CriteriaName']},
	'IncludeRun':{'pattern':['run include'],'path':['QualityCriteria/IncludeRun']},
	'ExcludeRun':{'pattern':['run exclude'],'path':['QualityCriteria/ExcludeRun']},
	'VALID_Lower':{'pattern':['valid min','valid reading min','valid low'],'path':['ValidValueSelection/ValidRange/Lower']},
	'VALID_Upper':{'pattern':['valid max','valid reading max','valid up'],'path':['ValidValueSelection/ValidRange/Upper']},
	'GREEN_Lower':{'pattern':['green min','green low'],'path':['ValueAlert/Limit<GREEN>/Lower']},
	'GREEN_Upper' :{'pattern':['green max','green up','green high'],'path':['ValueAlert/Limit<GREEN>/Upper']},
	'YELLOW_Lower':{'pattern':['yellow min','yellow " low"'],'path':['ValueAlert/Limit<YELLOW>/Lower']},
	'YELLOW_Upper':{'pattern':['yellow max','yellow up','yellow high'],'path':['ValueAlert/Limit<YELLOW>/Upper']},
	'Hysteresis':{'pattern':['hysteresis'],'path':["ValueAlert/Limit<GREEN>/Hysteresis","ValueAlert/Limit<YELLOW>/Hysteresis"]},
        'GREEN_Grace':{'pattern':['grace'], 'path':['GracePeriod/GraceLength', 'GracePeriod/GraceLevel']},
	'Dependence':{'pattern':['depend'],'path':['Dependence']},
	'WindowLength':{'pattern':['averag time','averag length','mov averag'],'path':['MovingAverage/WindowLength']},
	'GREEN_Length':{'pattern':['gap green'],'path':['GapAtBoundaryAlert/Limit<GREEN>/Length', 'GapInMiddleAlert/Limit<GREEN>/Length']},
	'YELLOW_Length':{'pattern':['gap yellow'],'path':['GapAtBoundaryAlert/Limit<YELLOW>/Length', 'GapInMiddleAlert/Limit<YELLOW>/Length']},
}

def isvalidknumber(knumber):
	if re.match("^[0-9]{3}-[A-Z0-9]{3}-[0-9]-[0-9]{4}-[0-9]{4}$",knumber):
		return True
	else:
		return False

def skip_comment(dqfile):
	rowid_comment = dqfile[dqfile.iloc[:,0].str.contains('^#') == True].index
	dqfile.drop(rowid_comment , inplace=True)
	dqfile.reset_index(drop=True, inplace=True)

def get_quality_criteria(dqfile, rowindex, column_name):
	startrow = rowindex + 1	
	criteria_block = dqfile.iloc[startrow:len(dqfile),:].copy()
	criteria_block.columns = column_name
	return criteria_block.to_dict(orient='record')

def createProcessor(rawcriteria, validdate):
	criteria = {key:value for key,value in sorted(rawcriteria.items(), key=lambda i:keyorder.index(i[0]))}
	quality_criteria = {}
	quality_criteria["QualityCriteria"] = {}
	quality_criteria["QualityCriteria"]["ValidFrom"] = validdate
	quality_criteria["QualityCriteria"]["Name"] = period_name
	for parname in criteria:
		if pd.isnull(ientry[parname]):
			continue 
		if parname not in pattern_parameter:
			continue

		for ipath in pattern_parameter[parname]["path"]:
			parpath = ipath.split("/")
			currentlayer = quality_criteria;
			for key_name in parpath[:-1]:
				if "<" in key_name and ">" in key_name:
					color_name = re.search('<([^>]+)',key_name).group(1)
					key_name = key_name.split("<")[0] 
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
				if "GraceLevel" in ipath:
					currentlayer[parpath[-1]] = 0
				if "IncludeRun" in ipath or "ExcludeRun" in ipath:
					run_list = ''.join('"[' +x+ ']"' for x in ientry[parname].split(','))
					currentlayer[parpath[-1]] = run_list
					print(parpath[-1], run_list) 
				else:
					currentlayer[parpath[-1]] = ientry[parname]
	if "ValueAlert" in quality_criteria and len(quality_criteria["ValueAlert"]["Limit"]) == 1:
		quality_criteria["ValueAlert"]["Limit"][0]["AlertLevel"] = alert_level["YELLOW"]
	if "GapAtBoundaryAlert" in quality_criteria and len(quality_criteria["GapAtBoundaryAlert"]["Limit"]) == 1:
		quality_criteria["GapAtBoundaryAlert"]["Limit"][0]["AlertLevel"] = alert_level["YELLOW"]
		quality_criteria["GapInMiddleAlert"]["Limit"][0]["AlertLevel"] = alert_level["YELLOW"]   
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
	skip_comment(dqfile)

	#Read validity date
	rowid_date = dqfile.apply(lambda x:x.str.contains("date",case=False)).any(axis=1).idxmax()
	listofdates = [date for date in  dqfile.iloc[rowid_date,:] if isinstance(date, datetime)]
	valid_date = (listofdates[0] if len(listofdates) > 0 else datetime.utcnow()).strftime("%Y-%m-%d") 
	print(valid_date)

	#Read the names of parameters	
	rowid_knumber = dqfile.apply(lambda x:x.str.contains("katrin number",case=False)).any(axis=1).idxmax()
	if not rowid_knumber:
		sys.exit("Input error: No row containing column names is found. Check your xlsx file. The row should start with 'KATRIN number'.")
	para_types = to_standard_colname(dqfile.iloc[rowid_knumber,:].tolist(), pattern_parameter)

	#Read the data and convert the dataframe into a list of dictionaries
	data_entries = get_quality_criteria(dqfile, rowid_knumber, para_types)	

	for ientry in data_entries:
		k_number = ientry['Katrin Number']
		if isvalidknumber(k_number):
			quality_criteria = createProcessor(ientry, valid_date)
			if not args.dry_run:
				outputfile = open("temp-QualityCriteria.json","w")
				outputfile.write(json.dumps(quality_criteria))
				outputfile.close()
				inifile = open(k_number+".ini", "w")
				subprocess.run(["ktf-treedump", "--format=ini", outputfile.name], stdout=inifile)
		#		CMD = "awk '{if (/=/) print $0\";\"; else print $0}' " + inifile.name + " > temp.ini; mv temp.ini " + inifile.name
			#		subprocess.run(CMD, shell=True) 
			else:
				#json_criteria = json.dumps(quality_criteria)
				#print(json_criteria)
				print(quality_criteria["QualityCriteria"]["KatrinNumber"])
		else:
			print("The KATRIN number " + k_number + " is not valid.")
