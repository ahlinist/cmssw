
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

def makeYieldPlot(inputFileName = None, dqmDirectory = None, outputFileName = None):

    # check that inputFileName, dqmDirectory and outputFileName parameters are defined and non-empty
    if inputFileName is None:
        raise ValueError("Undefined inputFileName Parameter !!")
    if dqmDirectory is None:
        raise ValueError("Undefined dqmDirectory Parameter !!")
    if outputFileName is None:
        raise ValueError("Undefined outputFileName Parameter !!")

    selEventsFileName = "selEvents_tmp.txt"
    
    cfgFileName = "makeYieldPlot_cfg.py"
    cfgFile = open(cfgFileName, "w")
    cfgFile.write(cfg)
    cfgFile.write("process.loadFilterStatistics.all.inputFileNames = cms.vstring('%s')\n" % inputFileName)
    cfgFile.write("process.dumpRunLumiSectionEventNumbers.dqmDirectories = cms.vstring('%s')\n" % dqmDirectory)
    cfgFile.write("process.dumpRunLumiSectionEventNumbers.runLumiSectionEventNumberFileName = cms.string('%s')\n" % selEventsFileName)
    cfgFile.close()

    commandLine = 'cmsRun %s' % cfgFileName
    _runCommand(commandLine)

    jsonFileName = "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions10/7TeV/StreamExpress/goodrunlist_json.txt"
    lumiCalcFileName = "lumiCalc_goodrunlist.csv"

    #commandLine = 'lumiCalc.py -i %s -o %s lumibylsXing' % (jsonFileName, lumiCalcFileName)
    commandLine = 'lumiCalc.py -i %s -o %s lumibyls' % (jsonFileName, lumiCalcFileName)
    _runCommand(commandLine)

    httpAddress = "http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?revision=1.9"
    commandLine = 'wget "%s" -O generateEDF.py' % httpAddress
    _runCommand(commandLine)

    commandLine = './generateEDF.py %s %s %s' % (lumiCalcFileName, selEventsFileName, outputFileName)
    _runCommand(commandLine)

    #subprocess.call("rm %s" % lumiCalcFileName, shell = True)
    #subprocess.call("rm %s" % selEventsFileName, shell = True)
