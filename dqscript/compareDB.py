import sys
from argparse import ArgumentParser
import json
import subprocess

if __name__ == '__main__':
	parser = ArgumentParser(description='Read quality criteria from files and fill into database.')
	parser.add_argument('-k','--KATRIN_number',type=str,help='KATRIN number')
	args = parser.parse_args()

	katrin_number = args.KATRIN_number

	dbfile = open("DB-" + katrin_number +".ini", "w")
	subprocess.run(["kali-quality-reader", katrin_number, "--output="+ dbfile.name],stdout=subprocess.PIPE)
	dbjson = open("DB-" + katrin_number +".json", "w")
	subprocess.run(["ktf-treedump", "--format=json", dbfile.name], stdout=dbjson)

	inifile = open(katrin_number+".ini", "r")
	inijson = open(katrin_number+".json", "w")
	subprocess.run(["ktf-treedump", "--format=json", inifile.name], stdout=inijson)

	with open(inijson.name, 'r') as data_file:
		data = json.load(data_file)
		data.pop('QualityCriteria', None)

	with open(inijson.name, 'w') as data_file:
		data = json.dump(data, data_file, indent=2)

	subprocess.run(["diff", "--unified", dbjson.name, inijson.name])

	subprocess.run(["rm", dbfile.name, dbjson.name, inijson.name])
