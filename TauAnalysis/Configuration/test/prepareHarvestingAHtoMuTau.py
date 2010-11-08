#!/usr/bin/env python

import TauAnalysis.Configuration.tools.crab as crab
from TauAnalysis.Configuration.tools.harvestingMakefile import buildMakefile
from TauAnalysis.Configuration.recoSampleDefinitionsAHtoMuTau_7TeV_grid_cfi \
        import SAMPLES_TO_ANALYZE
import os

CHANNEL = 'AHtoMuTau'
ID = 'Run11'
CASTOR_DIRECTORY = os.path.join(os.environ['CASTOR_HOME'],'AHtoMuTau')

PLOT_OUTPUT_DIRECTORY = '/data1/friis/Run11'
if not os.path.exists(PLOT_OUTPUT_DIRECTORY):
    os.mkdir(PLOT_OUTPUT_DIRECTORY)

# Where to store temporary files
WORKING_DIRECTORY = '/tmp/friis/Run11'
if not os.path.exists(WORKING_DIRECTORY):
    os.mkdir(WORKING_DIRECTORY)

# Map samples to their crab directories
crab_dir_map = {}

for sample in SAMPLES_TO_ANALYZE:
    crab_dir_map[sample] = os.path.join(
        'crab', 'crabdir_runAHtoMuTau_AHtoMuTau_%s_%s' % (sample, ID))

harvest_jobs = []

for sample in SAMPLES_TO_ANALYZE:
    print "Finding input files for", sample
    output_file = "harvested_%s_%s_%s.root" % (CHANNEL, sample, ID)
    output_path = os.path.join(PLOT_OUTPUT_DIRECTORY, output_file)
    files_to_merge = list( 'rfio:%s' % file for file in
                          crab.map_lfns_to_castor(
                              crab.lfns(crab_dir_map[sample])))
    harvest_jobs.append( (sample, output_path, files_to_merge) )

makefile_name = 'Makefile2.harvest_%s_%s' % (CHANNEL, ID)
buildMakefile(harvest_jobs, WORKING_DIRECTORY, makefile_name, merge_per_job=7,
              ana_defs="TauAnalysis.Configuration.recoSampleDefinitionsAHtoMuTau_7TeV_grid_cfi",
              # Defintion of the plots to make
              plot_defs="TauAnalysis.Configuration.plotAHtoMuTau_cff",
              # Plotter modules
              plotters=['plotAHtoMuTau_woBtag', 'plotAHtoMuTau_wBtag']
             )
