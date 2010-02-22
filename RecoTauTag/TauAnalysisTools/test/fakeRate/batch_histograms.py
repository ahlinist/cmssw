#!/usr/bin/env python

from RecoTauTag.TauAnalysisTools.fakeRate.makeHistograms import make_plots
from RecoTauTag.TauAnalysisTools.TriggerReport import TriggerReport
# Get our configuration
from RecoTauTag.TauAnalysisTools.fakeRate.histogramConfiguration import makeCuts
# Get the configuration parameters
from sources import denominator, sources
import glob

from optparse import OptionParser

parser = OptionParser()

parser.add_option('-n', '--njobs', help="Specify how many jobs to split the input into", type="int")
parser.add_option('-j', '--job', help="Specify the job index to run", type="int")
parser.add_option('-s', '--source', help="Name of the ntuple source to use.  Currently configured: %s" % sources.keys())

(options, args) = parser.parse_args()     

def normalizeToOneInverseNanoBarn(xSection, nEvents, filterEff):
    ''' Return a normalization factor to one nb-1 

    xSection should be given in picobarns

    L = nEvents/(xSection*filterEff)
    '''
    # in pb-1
    uncorrectedLumi = float(nEvents)/(xSection*filterEff)
    # pb-1 = 1e3 * nb-1
    nbLumi = 1e-3*uncorrectedLumi
    weight = 1.0/nbLumi
    return weight

def nthOfEveryMItems(n, m, theList):
    ''' Yield the nth item of sub groups of M items from the list '''
    for iter, item in enumerate(theList):
        if iter % m == n:
            yield item

def getNEvents(logFileGlob, pathName):
    " Get total visited events for pathName in outputPath log files"
    trigReport = TriggerReport(name="report")
    for file in glob.glob(logFileGlob):
        trigReport.addLogFile(file)
    return trigReport.paths[pathName].visited

# Determine which sources we want to build
build_list = [options.source]

print build_list

# Figure out the normalization
to_build = [
    (source, source_info) for source, source_info in sources.iteritems() 
    if source in build_list ]

for source, source_info in to_build:
    for location in list(source_info['sources'].keys()):
        print "Parsing %s logs" % location
        xSec, filterEff = source_info['sources'][location]
        nEvents = getNEvents("OldCrabDirs/%s/*.stderr" % location, "path")
        weight = normalizeToOneInverseNanoBarn(
            xSec, nEvents, filterEff)
        # Update the info
        source_info['sources'][location] = (
            xSec, filterEff, nEvents, weight)
        # Change the output filename to reflect the batch job
        source_info['output_file'] = source_info['output_file'].replace(
            '.root', '_%i.root' % options.job)

for source, source_info in to_build:
    # Print out some info
    print '==========================='
    print source 
    for loc, (xSec, filterEff, nEvents, weight) in source_info['sources'].iteritems():
        print "Loc: %s, nEvents: %i, weight %f" % (loc, nEvents, weight)

    # Build file list
    files_and_weights = []
    for loc, (xSec, filterEff, nEvents, weight) in source_info['sources'].iteritems():
        # Loop over the files to use for this run
        for file in nthOfEveryMItems(options.job, options.njobs, 
                                     glob.glob("OldCrabDirs/%s/*.root" % loc)):
            files_and_weights.append((file, weight))
    make_plots(files_and_weights, selections=makeCuts(denominator), **source_info)

