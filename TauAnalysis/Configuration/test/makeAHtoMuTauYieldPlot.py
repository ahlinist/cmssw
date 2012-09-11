#!/usr/bin/env python

from TauAnalysis.Configuration.makeYieldPlot import makeYieldPlot
import TauAnalysis.Configuration.userRegistry as reg
import os

##reg.overrideJobId('AHtoMuTau', '2010Dec23_lxbatch')
##
##inputFileName = os.path.join(reg.getHarvestingFilePath('AHtoMuTau'), "plotsAHtoMuTau_all.root")
##
##dqmDirectory = '/harvested/data/ahMuTauAnalyzerOS_woBtag/FilterStatistics/' \
##              + 'evtSelDiTauCandidateForAHtoMuTauZeroCharge/events_passed_cumulative/'

makeYieldPlot(selEventsFileName = "/afs/cern.ch/user/v/veelken/scratch0/CMSSW_5_2_3_patch3/src/TauAnalysis/Configuration/test/selEvents_VBF_muTau_2012_svFitMassGt200.txt",
              jsonFileName = "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-195947_8TeV_PromptReco_Collisions12_JSON.txt",
              ##inputFileName = inputFileName, dqmDirectory = dqmDirectory,
              ##outputFileName = "plotAHtoMuTau_woBtag_yield.png"
              outputFileName = "plotAHtoMuTau_VBFcategory_2012_yield.png")
