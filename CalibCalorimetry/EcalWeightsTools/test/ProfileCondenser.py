#ProfileCondenser.py v1.0
# A program to combine the output of EcalProfileBuilder.cc into one file
#NB This will not as of this version append results of the condensation to an already existent profile amalgam, but instead overwrites the output file. If you wish to append to an existent file, include this as an input and then output to a dummy file. This can be renamed.
#Usage : python ProfileCondenser.py profile_XXXXX.txt profile_YYYYY.txt profile_MERGED.txt
#David Wardrope, 19th January 2007

import fileinput, sys
#take 1st argument and search in files for it
print "Processing files %s" % (sys.argv[1:-1])
outputfile = (sys.argv[-1])
output = open(outputfile, "w")
Xtals = {}
for line in fileinput.input(sys.argv[1:-1]):
	words = line.split()
	if len(words) == 1: xid = words[0]
	if len(words) == 3 and words[1] == "1": #need to ensure nxtals and xid lines don't cause segfaults
		print "Maximum for crystal %s is on line %d." % (xid, fileinput.filelineno())
		L = [words[2], fileinput.filename()]
		if Xtals.has_key(xid):
			if Xtal[xid][1] > words[2]: Xtals[xid] = L
		else: Xtals[xid] = L

#Doesn't check for duplicates, yet...
output.write(str(len(Xtals))+"\n")
for line in fileinput.input(sys.argv[1:-1]):
	nxstals = 0
	#words = line.split()
	if fileinput.filelineno() == 1:
		nxstals = nxstals+int(line)
		print 1+int(nxstals)*251
	#elif len(words) == 1: xid = words[0]
	elif len(line.split()) == 1:
		xid = line.split()[0]
		output.write(line)
	elif Xtals[xid][1] == fileinput.filename(): output.write(line)

	#elif Xtals[xid][1] == fileinput.filename() and fileinput.filelineno(): output.write(line)

