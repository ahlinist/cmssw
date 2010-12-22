#!/usr/bin/env python

import sys
import subprocess
import shlex
import time

print("<exportAHtoXYplots>:")

samples = {
  'TaNCloose' : {
     'input'  : [
       '/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/plotsAHtoMuTau_TaNCloose_wSys_skimmed_part01.root',
       '/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/plotsAHtoMuTau_TaNCloose_wSys_skimmed_part02.root',
       '/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/plotsAHtoMuTau_TaNCloose_wSys_skimmed_part03.root'
     ],
     'output' : "/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/export_AHtoMuTau_TaNCloose_wSys"
  },

  'TaNCmedium' : {
     'input'  : [
       '/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/plotsAHtoMuTau_TaNCmedium_wSys_skimmed_part01.root',
       '/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/plotsAHtoMuTau_TaNCmedium_wSys_skimmed_part02.root',
       '/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/plotsAHtoMuTau_TaNCmedium_wSys_skimmed_part03.root'
     ],
     'output' : "/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/export_AHtoMuTau_TaNCmedium_wSys"
  },

  'TaNCtight' : {
     'input'  : [
       '/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/plotsAHtoMuTau_TaNCtight_wSys_skimmed_part01.root',
       '/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/plotsAHtoMuTau_TaNCtight_wSys_skimmed_part02.root',
       '/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/plotsAHtoMuTau_TaNCtight_wSys_skimmed_part03.root'
     ],
     'output' : "/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/export_AHtoMuTau_TaNCtight_wSys"
  }
}

cfgFileName = "exportAnalysisResults_cfg.py"

channelsToExport = """
    'AHtoMuTau_woBtag',
    'AHtoMuTau_wBtag'
"""

for sampleName, sampleConfig in samples.items():
   print("submitting export job for sample = %s:" % sampleName)

   cfgFile = open(cfgFileName, "r")
   cfg = cfgFile.read()
   cfgFile.close()

   inputFileNames = sampleConfig['input']
   outputFilePath = sampleConfig['output']

   cfgFileName_modified = cfgFileName.replace("_cfg.py", "_%s_cfg.py" % sampleName)
   print("--> creating config file = %s" % cfgFileName_modified)
   cfgFile_modified = open(cfgFileName_modified, "w")
   cfgFile_modified.write(cfg)
   cfgFile_modified.write("\n")
   cfgFile_modified.write("process.loadAnalysisResults.all.inputFileNames = cms.vstring(%s)\n" % inputFileNames)
   cfgFile_modified.write("process.exportAnalysisResults = " \
     + "configureDQMExportAnalysisResults(channels, [%s], processes, processesToExport, higgsMassPoints, '%s')\n" 
      % (channelsToExport, outputFilePath))
   cfgFile_modified.write("process.exportAnalysisResults_inclusive = " \
     + "configureDQMExportAnalysisResults(channels, 'AHtoMuTau_inclusive', processes, processesToExport, higgsMassPoints, '%s')\n" 
      % outputFilePath.replace("_wSys", "_wSys_inclusive"))
   cfgFile_modified.write("print process.dumpPython()")
   cfgFile_modified.close()

   # need to wait until config file has finished writing...
   time.sleep(1)

   print("--> starting cmsRun...")
   #subprocess.call("cmsRun %s &" % cfgFileName_modified, shell = True)

