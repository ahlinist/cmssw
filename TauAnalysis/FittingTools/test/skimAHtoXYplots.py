#!/usr/bin/env python

import sys
import subprocess
import shlex
import time

print("<skimAHtoXYplots>:")

samples = {
  'TaNCloose_part01' : {
     'input'  : '/data2/friis/Run35SYSloose/plotsAHtoMuTau_all.sm.root',
     'output' : '/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/plotsAHtoMuTau_TaNCloose_wSys_skimmed_part01.root'
  },
  'TaNCloose_part02' : {
     'input'  : '/data2/friis/Run35SYSloose/plotsAHtoMuTau_all.bsmgg.root',
     'output' : '/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/plotsAHtoMuTau_TaNCloose_wSys_skimmed_part02.root'
  },
  'TaNCloose_part03' : {
     'input'  : '/data2/friis/Run35SYSloose/plotsAHtoMuTau_all.bsmbb.root',
     'output' : '/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/plotsAHtoMuTau_TaNCloose_wSys_skimmed_part03.root'
  },

  'TaNCmedium_part01' : {
     'input'  : '/data2/friis/Run35SYS/plotsAHtoMuTau_all.sm.root',
     'output' : '/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/plotsAHtoMuTau_TaNCmedium_wSys_skimmed_part01.root'
  },
  'TaNCmedium_part02' : {
     'input'  : '/data2/friis/Run35SYS/plotsAHtoMuTau_all.bsmgg.root',
     'output' : '/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/plotsAHtoMuTau_TaNCmedium_wSys_skimmed_part02.root'
  },
  'TaNCmedium_part03' : {
     'input'  : '/data2/friis/Run35SYS/plotsAHtoMuTau_all.bsmbb.root',
     'output' : '/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/plotsAHtoMuTau_TaNCmedium_wSys_skimmed_part03.root'
  },

  'TaNCtight_part01' : {
     'input'  : '/data2/friis/Run35SYStight/plotsAHtoMuTau_all.sm.root',
     'output' : '/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/plotsAHtoMuTau_TaNCtight_wSys_skimmed_part01.root'
  },
  'TaNCtight_part02' : {
     'input'  : '/data2/friis/Run35SYStight/plotsAHtoMuTau_all.bsmgg.root',
     'output' : '/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/plotsAHtoMuTau_TaNCtight_wSys_skimmed_part02.root'
  },
  'TaNCtight_part03' : {
     'input'  : '/data2/friis/Run35SYStight/plotsAHtoMuTau_all.bsmbb.root',
     'output' : '/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/plotsAHtoMuTau_TaNCtight_wSys_skimmed_part03.root'
  }
}

cfgFileName = "skimAHtoMuTauPlots_cfg.py"

outputCommands = """
  'drop harvested/*',
  'keep harvested/*/ahMuTauAnalyzerOS_woBtag/afterEvtSelDiTauCandidateForAHtoMuTauZeroCharge/*',
  'keep harvested/*/ahMuTauAnalyzerOS_woBtag/FilterStatistics/*',
  'keep harvested/*/ahMuTauAnalyzerOS_wBtag/afterEvtSelDiTauCandidateForAHtoMuTauZeroCharge/*',
  'keep harvested/*/ahMuTauAnalyzerOS_wBtag/FilterStatistics/*'
"""

for sampleName, sampleConfig in samples.items():
   print("submitting skimming job for sample = %s:" % sampleName)

   cfgFile = open(cfgFileName, "r")
   cfg = cfgFile.read()
   cfgFile.close()

   inputFileName = sampleConfig['input']
   outputFileName = sampleConfig['output']

   cfgFileName_modified = cfgFileName.replace("_cfg.py", "_%s_cfg.py" % sampleName)
   print("--> creating config file = %s" % cfgFileName_modified)
   cfgFile_modified = open(cfgFileName_modified, "w")
   cfgFile_modified.write(cfg)
   cfgFile_modified.write("\n")
   cfgFile_modified.write("process.loadAnalysisResults.all.inputFileNames = cms.vstring('%s')\n" % inputFileName)
   cfgFile_modified.write("process.saveAnalysisResults.outputFileName = cms.string('%s')\n" % outputFileName)
   cfgFile_modified.write("process.saveAnalysisResults.outputCommands = cms.vstring(%s)\n" % outputCommands)
   cfgFile_modified.write("print process.dumpPython()")
   cfgFile_modified.close()

   # need to wait until config file has finished writing...
   time.sleep(1)

   print("--> starting cmsRun...")
   #subprocess.call("cmsRun %s &" % cfgFileName_modified, shell = True)
