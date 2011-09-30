
#--------------------------------------------------------------------------------
# Run generateEDF.py tool developed by Charles Plager
# to make a plot of the number of selected events as function of integrated luminosity
# of the analyzed dataset
#
# See http://home.fnal.gov/~cplager/log/1009/log.html
# for details about the tool
#--------------------------------------------------------------------------------

import subprocess
import shlex

cfg = \
"""
import FWCore.ParameterSet.Config as cms

process = cms.Process('dumpRunLumiSectionEventNumbers')

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(0)
)

process.source = cms.Source("EmptySource")

process.loadFilterStatistics = cms.EDAnalyzer("DQMFileLoader",
    all = cms.PSet(
        inputFileNames = cms.vstring(),
        dqmDirectory_store = cms.string('')
    )
)

process.dumpRunLumiSectionEventNumbers = cms.EDAnalyzer("DQMDumpRunLumiSectionEventNumbers",
    dqmDirectories = cms.vstring(),
    runLumiSectionEventNumberFileName = cms.string(''),
    separator = cms.string(':')
)

process.p = cms.Path(
    process.loadFilterStatistics
   + process.dumpRunLumiSectionEventNumbers
)
"""

def _runCommand(commandLine):

    print(commandLine)
    args = shlex.split(commandLine)
    retval = subprocess.Popen(args, stdout = subprocess.PIPE)
    retval.wait()

def makeYieldPlot(selEventsFileName = None, jsonFileName = None, outputFileName = None,
                  inputFileName = None, dqmDirectory = None):

    # check that either selEventFileName or inputFileName plus dqmDirectory are defined and non-empty
    if selEventsFileName is None and (inputFileName is None or dqmDirectory is None):
        raise ValueError("Must defined either 'selEventFileName' or 'inputFileName' plus 'dqmDirectory' parameters !!")
    # check that outputFileName parameter is defined and non-empty
    if outputFileName is None:
        raise ValueError("Undefined outputFileName Parameter !!")

    tmpFiles = []

    if selEventsFileName is None: 
        selEventsFileName = "selEvents_tmp.txt"
        #tmpFiles.append(selEventsFileName)

        cfgFileName = "makeYieldPlot_cfg.py"
        cfgFile = open(cfgFileName, "w")
        cfgFile.write(cfg)
        cfgFile.write("process.loadFilterStatistics.all.inputFileNames = cms.vstring('%s')\n" % inputFileName)
        cfgFile.write("process.dumpRunLumiSectionEventNumbers.dqmDirectories = cms.vstring('%s')\n" % dqmDirectory)
        cfgFile.write("process.dumpRunLumiSectionEventNumbers.runLumiSectionEventNumberFileName = cms.string('%s')\n" % selEventsFileName)
        cfgFile.close()
        tmpFiles.append(cfgFileName)

        commandLine = 'cmsRun %s' % cfgFileName
        _runCommand(commandLine)

    lumiCalcFileName = "lumiCalc_goodrunlist.csv"
    #tmpFiles.append(lumiCalcFileName)

    #commandLine = 'lumiCalc.py -i %s -o %s lumibylsXing' % (jsonFileName, lumiCalcFileName)
    commandLine = 'lumiCalc.py -i %s -o %s lumibyls' % (jsonFileName, lumiCalcFileName)
    #_runCommand(commandLine)

    httpAddress = "http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?revision=1.9"
    commandLine = 'wget "%s" -O generateEDF.py' % httpAddress
    _runCommand(commandLine)
    commandLine = 'chmod +x generateEDF.py'
    _runCommand(commandLine)

    commandLine = './generateEDF.py %s %s %s --ignoreNoLumiEvents' % (lumiCalcFileName, selEventsFileName, outputFileName)
    _runCommand(commandLine)

    for tmpFileName in tmpFiles:
        subprocess.call("rm %s" % tmpFileName, shell = True)
