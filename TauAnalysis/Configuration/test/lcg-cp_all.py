#!/usr/bin/env python

import os
import re
import sys
import subprocess
import shlex

#--------------------------------------------------------------------------------
# Copy all files stored in a given DPM directory to given directory on local disk
#
# Author: Christian Veelken (UC Davis)
# 
# Examples:
# 
# ./lcg-cp_all.py "srm://se.polgrid.pl:8446/srm/managerv2?SFN=/dpm/polgrid.pl/home/cms/phedex/store/store/user/fruboes/20120307_TMVA/20120307_TMVA/4460588ca06ab2d76f02c184859e4fb1//histo_9_1_4hj.root" /tmp/veelken
#
#--------------------------------------------------------------------------------

print("<lcg-cp_all>:")

if not len(sys.argv) == 3:
    raise ValueError("Usage: lcg-cp_all.py input_directory output_directory")

inputFilePath = sys.argv[1]
print "inputFilePath = %s" % inputFilePath
outputFilePath = sys.argv[2]
print "outputFilePath = %s" % outputFilePath

executable_lcg_cp = 'lcg-cp'
executable_lcg_ls = 'lcg-ls'

inputFileName_regex = r'[a-zA-Z0-9_:/\.\?=]+/(?P<fileName>[a-zA-Z0-9_\.]+)'
inputFileName_matcher = re.compile(inputFileName_regex)

def runCommand(commandLine):
    print(commandLine)
    args = shlex.split(commandLine)
    retVal = subprocess.Popen(args, stdout = subprocess.PIPE)
    #retVal.wait()
    return retVal.stdout.readlines()

which_lcg_cp = runCommand('which %s' % executable_lcg_cp)
print "which lcg_cp = %s" % which_lcg_cp
if len(which_lcg_cp) == 0:
    raise ValueError("Failed to find executable '%s' !!" % executable_lcg_cp)
which_lcg_ls = runCommand('which %s' % executable_lcg_ls)
if len(which_lcg_ls) == 0:
    raise ValueError("Failed to find executable '%s' !!" % executable_lcg_ls)
print "which lcg_ls = %s" % which_lcg_ls

inputFiles = runCommand('%s "%s"' % (executable_lcg_ls, inputFilePath))
for inputFile in inputFiles:
    print "inputFile = %s" % inputFile
    inputFileName_match = inputFileName_matcher.match(inputFile)
    if inputFileName_match:
        inputFileName_base = inputFileName_match.group('fileName')
        inputFileName = os.path.join(inputFilePath, inputFileName_base)
        outputFileName = os.path.join(outputFilePath, inputFileName_base)
        runCommand('%s "%s" %s' % (executable_lcg_cp, inputFileName, outputFileName))
    else:
        raise ValueError("Invalid input file name = %s !!" % inputFile)
