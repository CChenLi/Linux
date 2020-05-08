
import sys
import random
import argparse
import string

class shuf:
	def __init__(self, headcount, repeat, inputrange, args):
		if inputrange != None:
			inputrange=inputrange.split('-')
			if len(args) != 0:	
				sys.exit(sys.argv[0] + 
": input-range not compitable when argument is given")
			if not sys.stdin.isatty():
				sys.exit(sys.argv[0] + 
": input-range not compitable when input stream exist")
			if len(inputrange) != 2:
				sys,exit(sys.argv[0] + ": --input-range lo-hi,\
where 0 <= lo <= hi")
			try:
				lo=int(inputrange[0])
				hi=int(inputrange[1])
			except:
				sys.exit(sys.argv[0] + ": lo and hi should be\
int type")
	
			if (lo < 0) or (lo > hi):
				sys.exit(sys.argv[0] + ": lo should be smaller\
than hi and positive")
	
			resovior = list(range(lo, hi+1))

		if len(args) > 1:
			sys.exit(sys.argv[0] + ": Too many operands, '"\
				+ args[1] + "'")
	
		if inputrange == None and (len(args) == 0 or\
			(len(args) == 1 and args[0] == '-')):
			usrinput = sys.stdin.readlines()
			resovior = [i.strip('\n') for i in usrinput]
	
		if inputrange == None and (len(args) == 1 and\
			args[0] != '-'):
			try:
				ifs = open(args[0], 'r')
				usrinput = ifs.readlines()
				resovior = [i.strip('\n') for i in usrinput]
			except:
				sys.exit(sys.argv[0] + ": open file failed")
	
	#finish resovior set up------------------------------

		if headcount != None:
			try:
				headcount = int(headcount)
			except:
				sys.exit(sys.argv[0] + ": invalid head-count")
			if headcount < 0:
				sys.exit(sys.argv[0] + ": negative head-count")
			if repeat:
				for i in range(headcount):
					output = random.choices(resovior,
						k=headcount)
			if not repeat:
					output = random.sample(resovior,
						headcount)
		if headcount == None:
			if repeat:
				while True:
					print(random.choice(resovior))
			if not repeat:
				output = resovior
				random.shuffle(output)

		self.output=output

	def printshuf(self):
		for i in self.output:
			print(i)

def main():
    # Use argparse to parse the arguments
	parser = argparse.ArgumentParser()
	parser.add_argument("-n", "--head-count", dest="headcount",
		help="output at most COUNT lines\n")
	parser.add_argument("-r", "--repeat", action='store_true',
		dest="repeat", default=False,
		help="output lines can be repeated\n")
	parser.add_argument("-i", "--input-range", dest="inputrange",
		help="treat each number LO through HI as an input line\n")
	options, args = parser.parse_known_args()
	#repeat=options.repeat
	
	S = shuf(options.headcount, options.repeat, options.inputrange, args)
	S.printshuf()

	return 0


if __name__=="__main__":
	main()