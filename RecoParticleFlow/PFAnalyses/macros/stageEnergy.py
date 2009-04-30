#!/usr/bin/env python

import commands
import sys
print 'Staging energy script with args: '
print sys.argv
from optparse import OptionParser

#Edit this to write output files anywhere other then the current directory
dest = ''
home = '/afs/cern.ch/user/b/ballin/scratch0/cmssw/src'

parser = OptionParser()
parser.add_option("-e", "--energy", type="int", dest="energy", help="Energy to consider")
parser.add_option("-s", action="store_true", dest="nostage", help="Don't stage files")
parser.add_option("-c", "--cuts", dest="cuts", metavar="FILE", help="Write testbeam cut macro to FILE")
parser.add_option("-p", "--python", dest="python", metavar="FILE", help="Write CMSSW python config to FILE")
parser.add_option("-b", "--batch", dest="batchCmd", metavar="FILE", help="Write batchfile too!")
parser.add_option("-t", action="store_true", dest="temp", help="Copy files to /tmp")
(options, args) = parser.parse_args()

from RecoParticleFlow.PFAnalyses.RunDict import *

specifiedE = energies[options.energy]

result = []
if not options.nostage:
    print "Staging files..."
    result = map(lambda x : commands.getoutput('stager_get -M /castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.000' + str(x) + '.combined.OutServ_0.0-cmsswreco.root'), specifiedE)
else:
    print "Querying files..."
    result = map(lambda x : commands.getoutput('stager_qry -M /castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.000' + str(x) + '.combined.OutServ_0.0-cmsswreco.root'), specifiedE)

files = []

for line in result:
    file = line.split('\n')
    item = file[1]
    staged = True
    filename = item.split(' ')
    if options.nostage:
        if filename[2] == 'STAGED':
            files.append(filename[0])
    else:
        files.append(filename[0])
    
print 'Currently staged files...'
for file in files:
    print file
    
if options.temp:
    print 'Copying staged files to /tmp'
    for file in files:
        commands.getoutput('rfcp ' + file + ' /tmp')

if options.cuts:
    print 'Testbeam cut processing...'
    for file in files:
        tokens = file.split('.')
        print 'process.processEvents(\"rfio://' + file + '\", ' + str(int(tokens[2])) + ");" 
    
if options.python:
    print 'Writing python config file...'
    output = open(options.python, 'w')
    template = open('cmsRunTemplate.txt')
    for line in template:
        if str(line) == '<FILENAMES>\n':
            output.write('runs = cms.untracked.vstring(')
            for file in files:
                output.write('\"rfio://' + file + '\", \n')
            output.write(')\n')
        elif str(line) == '<OUTPUTTREE>\n':
            output.write('process.TFileService.fileName = cms.string(\"' + dest + 'outputtree_' + str(options.energy) + 'GeV.root\")\n')
        elif str(line) == '<EVENT>\n':
            output.write('process.finishup.fileName = cms.untracked.string(\"' + dest + 'reprocessed_' + str(options.energy) + 'GeV.root\")\n')
	elif str(line) == '<LOG>\n':
	    output.write('log = \"' + dest + 'log_' + str(options.energy) + 'GeV.txt\"\n')
        else:
            output.write(line)
            
    
print 'Done.'
    
    
