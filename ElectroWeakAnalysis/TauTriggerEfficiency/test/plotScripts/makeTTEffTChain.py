#!/usr/bin/env python
import sys
import re
import os

def main(argv):

        if len(sys.argv) == 1:
            print "\n"
            print "### Usage:   makeTTEffTChain.py <castor data path>\n"
            print "\n"
            sys.exit()

	path_re = re.compile("(?P<castorpath>[^/]*$)")

	path = sys.argv[1:len(sys.argv)]

	thePath = path[0]
	thePath = thePath.rstrip('/')

	pathAfterLastSlash = path_re.search(thePath)
	if pathAfterLastSlash:
	    TChainName = re.sub('-','_',pathAfterLastSlash.group("castorpath"))
	    TChainName = re.sub('.root','',TChainName)
	    TChainName = re.sub('_._','_',TChainName)
	    fOUT = TChainName + ".cxx"
	else:
	    sys.exit()

	tmpfiles = []
        for file in path:
            if re.search("castor",file):
                tmpfiles[len(tmpfiles):len(tmpfiles)] = execute("nsls %s"%file)
            else:
                tmpfiles[len(tmpfiles):len(tmpfiles)] = execute("ls %s"%file)

#	loop_re = re.compile("(tteffAnalysis-)(.*?)_")
	loop_re = re.compile("(tteffAnalysis)(-|_)(.*?)(_|\.)")
	loopnames = []

	for file in tmpfiles:
            match = loop_re.search(file)
            if match:
		loopname = match.group(3)
		if loopnames.count(loopname) == 0:
		    loopnames.append(loopname)

	tchainFile = open(fOUT,'w')

	for loopname in loopnames:
	    tmploopfiles = []
	    for file in tmpfiles:
		match = re.search(loopname,file)
		if match:
		    tmploopfiles.append(file)

#	    loopname = re.sub('-pftau','',loopname)
	    loopname = re.sub('-','_',loopname)
	    writeTChain(tchainFile,thePath,TChainName,loopname,tmploopfiles)

	print fOUT

###

def execute(cmd):
    f = os.popen(cmd)
    ret=[]
    for line in f:
        ret.append(line.replace("\n", ""))
    f.close()
    return ret

###

def writeTChain(tchainFile,path,chainName,loopName,files):

    tchainFile.write('TChain* '+ chainName + '_' + loopName + '() {\n')
    tchainFile.write('\n')
    tchainFile.write('        TChain* chain = new TChain("TTEffTree");\n')
    tchainFile.write('\n')
    tchainFile.write('        TString path = "' + str(path) + '/";\n')

    root_re = re.compile("(?P<rootfile>((.*)\.root))")
    for file in files:
	match = root_re.search(file)
	if match:
	    fileName = match.group("rootfile")
	    tchainFile.write('        chain->AddFile(path + "' + str(fileName) + '");\n')
    tchainFile.write('        return chain;\n')
    tchainFile.write('}\n')
    tchainFile.write('\n')


main(sys.argv[1:])
