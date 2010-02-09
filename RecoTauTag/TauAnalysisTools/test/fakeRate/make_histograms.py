from RecoTauTag.TauAnalysisTools.fakeRate.makeHistograms import *
from RecoTauTag.TauAnalysisTools.TriggerReport import TriggerReport

# Get our configuration
from RecoTauTag.TauAnalysisTools.fakeRate.histogramConfiguration import makeCuts

import sys
import os
import glob
import copy

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

def getNEvents(logFileGlob, pathName):
    " Get total visited events for pathName in outputPath log files"
    trigReport = TriggerReport(name="report")
    for file in glob.glob(logFileGlob):
        trigReport.addLogFile(file)
    return trigReport.paths[pathName].visited

# Define our standard binning
standard_bins = {
    'x_bins' : [0, 10, 20, 30, 50, 80, 120], # pt
    'y_bins' : [0, 0.5, 1.0, 1.2, 1.5, 2.0, 2.5], # eta
    'z_bins' : [0, 0.01, 0.02, 0.03, 0.04] # jet width
}

denominator = "$ByLeadTrackPt && abs($eta) < 2.1 && $pt > 20"

build_list = ['dijet_first', 'dijet_second', 'ppmux', 'ztt', 'wjets']
#build_list = ['wjets']

sources = {}
# Define our sources
sources['ztt'] = {
    # Source defintion (crab_output, xSection (pb), filter eff)
    'sources': {'Ztautau/res': (1944, 1.0)},
    'output_file': "ztt_histograms.root",
    'ntuple_name': "shrinkingConePFTau",
    'ntuple_setup': lambda ntuple: ntuple.set_collection("matched"),
}
sources['ztt'].update(standard_bins)

sources['ppmux'] = {
    'sources': {'InclusiveMu15/res': ((0.5091*1e9), 2.881e-4)}, # 0.5091 mb 
    'output_file': 'ppmux_histograms.root',
    'ntuple_name': 'shrinking',
    'ntuple_setup': lambda ntuple: ntuple.set_collection("unmatched"),
}
sources['ppmux'].update(standard_bins)

# Case for highest pt jet probe
sources['dijet_first'] = {
    'sources': {
        'Dijet20_30/res': (400982000., 1.0), # in pb
        'Dijet30_50/res': (94702500., 1.0),
        'Dijet50_80/res': (12195900., 1.0),
        'Dijet80_120/res': (1617240., 1.0),
        'Dijet120_170/res': (255987., 1.0) },
    'output_file': 'dijet_highpt_histograms.root',
    'ntuple_name': 'highestPtJet',
    'ntuple_setup': lambda ntuple: ntuple.set_collection('all'),
}
sources['dijet_first'].update(standard_bins)

# Case for second pt jet probe
sources['dijet_second'] = {
    'sources': {
        'Dijet20_30/res': (400982000., 1.0), # in pb
        'Dijet30_50/res': (94702500., 1.0),
        'Dijet50_80/res': (12195900., 1.0),
        'Dijet80_120/res': (1617240., 1.0),
        'Dijet120_170/res': (255987., 1.0) },
    'output_file': 'dijet_secondpt_histograms.root',
    'ntuple_name': 'secondPtJet',
    'ntuple_setup': lambda ntuple: ntuple.set_collection('all'),
}
sources['dijet_second'].update(standard_bins)

sources['wjets'] = {
    'sources': {'WJets/res': (30000., 0.36)},
    'output_file': 'wjets_histograms.root',
    'ntuple_name': 'shrinking',
    'ntuple_setup': lambda ntuple: ntuple.set_collection("unmatched"),
}
sources['wjets'].update(standard_bins)

# Figure out the normalization
to_build = [
    (source, source_info) for source, source_info in sources.iteritems() 
    if source in build_list ]

for source, source_info in to_build:
    for location in list(source_info['sources'].keys()):
        print "Parsing %s logs" % location
        xSec, filterEff = source_info['sources'][location]
        nEvents = getNEvents("%s/*.stderr" % location, "path")
        weight = normalizeToOneInverseNanoBarn(
            xSec, nEvents, filterEff)
        # Update the info
        source_info['sources'][location] = (
            xSec, filterEff, nEvents, weight)


for source, source_info in to_build:
    print '==========================='
    print source 
    for loc, (xSec, filterEff, nEvents, weight) in source_info['sources'].iteritems():
        print "Loc: %s, nEvents: %i, weight %f" % (loc, nEvents, weight)

for source, source_info in to_build:
    # Build file list
    files_and_weights = []
    for loc, (xSec, filterEff, nEvents, weight) in source_info['sources'].iteritems():
        for file in glob.glob("%s/*.root" % loc):
            files_and_weights.append((file, weight))
    make_plots(files_and_weights, selections=makeCuts(denominator), **source_info)

