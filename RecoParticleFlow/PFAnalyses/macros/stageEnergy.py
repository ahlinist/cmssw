#!/usr/bin/env python

import commands
import sys
print 'Staging energy script with args: '
print sys.argv
from optparse import OptionParser

parser = OptionParser()
parser.add_option("-e", "--energy", type="int", dest="energy", help="Energy to consider")
parser.add_option("-s", action="store_true", dest="nostage", help="Don't stage files")
parser.add_option("-c", "--cuts", dest="cuts", metavar="FILE", help="Write testbeam cut macro to FILE")
parser.add_option("-p", "--python", dest="python", metavar="FILE", help="Write CMSSW python config to FILE")
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
    
print 'Available files...'
for file in files:
    print file

if options.cuts:
    print 'Testbeam cut processing...'
    for file in files:
        tokens = file.split('.')
        print 'process.processEvents(\"rfio://' + file + '\", ' + str(int(tokens[2])) + ");" 
    
if options.python:
    print 'Config file...'

    for file in files:
        print '\"rfio://' + file + '\", '
    
    
    
    
