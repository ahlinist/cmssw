#!/usr/bin/env python
import os
import random
import time

def crabCall(uiworkingdir):
    print ''
    print '##########################################'
    print 'Creating job...'
    print '##########################################'
    os.system('crab -create')
#    print ''
#    print '##########################################'
#    print 'Hacking job...'
#    print '##########################################'
#    os.system('rm job_'+ process +'_'+ jetalgo +'/job/CMSSW.sh_back')
#    g = open('job_'+ process +'_'+ jetalgo +'/job/CMSSW.sh','r')
#    BatchFile_ = g.read()
#    print ''
#    print 'Old CMSSW.sh: '
#    os.system('cat job_'+ process +'_'+ jetalgo +'/job/CMSSW.sh | grep filesToCheck=')
#    BatchFile_ = BatchFile_.replace('filesToCheck="AnaDiffJet_$NJob.root crab_fjr_$NJob.xml CMSSW_$NJob.stdout CMSSW_$NJob.stderr"','filesToCheck="AnaDiffJet_$NJob.root crab_fjr_$NJob.xml"',-1)
#    g.close()
#    h = open('job_'+ process +'_'+ jetalgo +'/job/CMSSW.sh_back','a+')
#    h.write(BatchFile_)
#    h.close()
#    os.system('mv job_'+ process +'_'+ jetalgo +'/job/CMSSW.sh_back job_'+ process +'_'+ jetalgo +'/job/CMSSW.sh')
#    print ''
#    print 'New CMSSW.sh: '
#    os.system('cat job_'+ process +'_'+ jetalgo +'/job/CMSSW.sh | grep filesToCheck=')
#    print ''
    print '##########################################'
    print 'Submiting job...'
    print '##########################################'
    os.system('crab -submit -c '+ uiworkingdir)

def cmsRunCall():
    print ''
    print '##########################################'
    print 'Starting CMSSW...'
    print '##########################################'
    os.system('cmsRun pset.py')

#define a funcao que manipula os arquivos de config do CMSSW
def ConfigHandler(generator, lumi, rgsp, eff, parametersList, dataset, njobs, uiworkingdir, sewhitelist):
    pset_py1 = """

import FWCore.ParameterSet.Config as cms

process = cms.Process("SDDijetsPlotter")

process.load("FWCore.MessageService.MessageLogger_cfi")
# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.categories.append('SDDijetsPlotter')
process.MessageLogger.cerr.INFO = cms.untracked.PSet(
    default          = cms.untracked.PSet( limit = cms.untracked.int32(0)  ),
    Demo = cms.untracked.PSet( limit = cms.untracked.int32(1000) )
)
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
       '/store/user/fsilva/PomWigDiJetsMinusPt30/PomWigDiJetsMinusPt30_Skim_v38/264ca8f6f534226335efe858355695c5/SDDijetsAnaNtuple_40.root',
       '/store/user/fsilva/PomWigDiJetsMinusPt30/PomWigDiJetsMinusPt30_Skim_v38/264ca8f6f534226335efe858355695c5/SDDijetsAnaNtuple_41.root',
       '/store/user/fsilva/PomWigDiJetsMinusPt30/PomWigDiJetsMinusPt30_Skim_v38/264ca8f6f534226335efe858355695c5/SDDijetsAnaNtuple_42.root',
       '/store/user/fsilva/PomWigDiJetsMinusPt30/PomWigDiJetsMinusPt30_Skim_v38/264ca8f6f534226335efe858355695c5/SDDijetsAnaNtuple_43.root',
       '/store/user/fsilva/PomWigDiJetsMinusPt30/PomWigDiJetsMinusPt30_Skim_v38/264ca8f6f534226335efe858355695c5/SDDijetsAnaNtuple_44.root',
       '/store/user/fsilva/PomWigDiJetsMinusPt30/PomWigDiJetsMinusPt30_Skim_v38/264ca8f6f534226335efe858355695c5/SDDijetsAnaNtuple_45.root',
       '/store/user/fsilva/PomWigDiJetsMinusPt30/PomWigDiJetsMinusPt30_Skim_v38/264ca8f6f534226335efe858355695c5/SDDijetsAnaNtuple_46.root',
       '/store/user/fsilva/PomWigDiJetsMinusPt30/PomWigDiJetsMinusPt30_Skim_v38/264ca8f6f534226335efe858355695c5/SDDijetsAnaNtuple_47.root',
       '/store/user/fsilva/PomWigDiJetsMinusPt30/PomWigDiJetsMinusPt30_Skim_v38/264ca8f6f534226335efe858355695c5/SDDijetsAnaNtuple_48.root',
       '/store/user/fsilva/PomWigDiJetsMinusPt30/PomWigDiJetsMinusPt30_Skim_v38/264ca8f6f534226335efe858355695c5/SDDijetsAnaNtuple_49.root'
           ),
    inputCommands = cms.untracked.vstring(
          'keep *',
          'drop *_particleFlowBlock_*_*',
          'drop *_particleFlow_*_*',
          'drop *_particleFlowClusterECAL_*_*',
          'drop *_particleFlowClusterHCAL_*_*',
          'drop *_particleFlowClusterPS_*_*'
           ),
    skipBadFiles = cms.untracked.bool(True)
)

##Trigger Filter
import copy
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
process.SDDijetsHLTFilter = copy.deepcopy(hltHighLevel)
process.SDDijetsHLTFilter.TriggerResultsTag = cms.InputTag("TriggerResults","","HLT")
process.SDDijetsHLTFilter.HLTPaths = ['HLT_DiJetAve30']

##genEventKTValue
process.load("PhysicsTools.HepMCCandAlgos.genEventKTValue_cfi")


process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("SDDijetsHistos.root")
)

process.TrackMultiplicity = cms.EDProducer('TrackMultiplicity',
    TrackPtCut = cms.double(0.9)
)


process.GenScaleFilter = cms.EDFilter('GenScaleFilter',
    generator = cms.string("@@GENERATOR@@")
)

process.WeightProducer = cms.EDProducer('WeightProducer',
    generator = cms.string("@@GENERATOR@@"),
    Lumi = cms.double(@@LUMI@@),
    RGSP = cms.double(@@RGSP@@),
    Eff = cms.double(@@EFF@@)

)

process.RecoKinPlots = cms.EDAnalyzer('KinPlotter',
)

process.HLTKinPlots = cms.EDAnalyzer('KinPlotter',
)


"""

    pset_py2 = """
process.GapSideProducer@@NAME@@ = cms.EDProducer('GapSideProducer',
    HFNoise = cms.double(@@HFNOISE@@),
    RandomGapSide = cms.bool(False)
)

process.DijetsFilter@@NAME@@ = cms.EDFilter('DijetsFilter',
    GapSideSource = cms.string("GapSideProducer@@NAME@@")
)

process.OfflineKinPlots@@NAME@@ = cms.EDAnalyzer('KinPlotter',
)

process.sdDijetsPlotterInf@@NAME@@ = cms.EDAnalyzer('SdDijetsPlotter',
    GapSideSource = cms.string("GapSideProducer@@NAME@@"),
    HFNoise = cms.double(@@HFNOISE@@),
    CASTORNoise = cms.double(@@CASTORNOISE@@),
    ntrackCut = cms.int32(999999)
)

process.sdDijetsPlotter5@@NAME@@ = cms.EDAnalyzer('SdDijetsPlotter',
    GapSideSource = cms.string("GapSideProducer@@NAME@@"),
    HFNoise = cms.double(@@HFNOISE@@),
    CASTORNoise = cms.double(@@CASTORNOISE@@),
    ntrackCut = cms.int32(5)
)

process.sdDijetsPlotter1@@NAME@@ = cms.EDAnalyzer('SdDijetsPlotter',
    GapSideSource = cms.string("GapSideProducer@@NAME@@"),
    HFNoise = cms.double(@@HFNOISE@@),
    CASTORNoise = cms.double(@@CASTORNOISE@@),
    ntrackCut = cms.int32(1)
)

##RANDOM
process.GapSideProducerRandom@@NAME@@ = cms.EDProducer('GapSideProducer',
    HFNoise = cms.double(@@HFNOISE@@),
    RandomGapSide = cms.bool(True)
)

process.DijetsFilterRandom@@NAME@@ = cms.EDFilter('DijetsFilter',
    GapSideSource = cms.string("GapSideProducerRandom@@NAME@@")
)

process.OfflineKinPlotsRandom@@NAME@@ = cms.EDAnalyzer('KinPlotter',
)

process.sdDijetsPlotterInfRandom@@NAME@@ = cms.EDAnalyzer('SdDijetsPlotter',
    GapSideSource = cms.string("GapSideProducerRandom@@NAME@@"),
    HFNoise = cms.double(@@HFNOISE@@),
    CASTORNoise = cms.double(@@CASTORNOISE@@),
    ntrackCut = cms.int32(999999)
)

process.sdDijetsPlotter5Random@@NAME@@ = cms.EDAnalyzer('SdDijetsPlotter',
    GapSideSource = cms.string("GapSideProducerRandom@@NAME@@"),
    HFNoise = cms.double(@@HFNOISE@@),
    CASTORNoise = cms.double(@@CASTORNOISE@@),
    ntrackCut = cms.int32(5)
)

process.sdDijetsPlotter1Random@@NAME@@ = cms.EDAnalyzer('SdDijetsPlotter',
    GapSideSource = cms.string("GapSideProducerRandom@@NAME@@"),
    HFNoise = cms.double(@@HFNOISE@@),
    CASTORNoise = cms.double(@@CASTORNOISE@@),
    ntrackCut = cms.int32(1)
)##/RANDOM

#process.dump=cms.EDAnalyzer('EventContentAnalyzer')
#process.p = cms.Path(process.L2L3CorJetSC5Calo)

process.p@@NAME@@ = cms.Path(process.genEventKTValue * process.GenScaleFilter * process.WeightProducer * process.TrackMultiplicity * process.RecoKinPlots * process.SDDijetsHLTFilter * process.HLTKinPlots * process.GapSideProducer@@NAME@@ * process.DijetsFilter@@NAME@@ * process.OfflineKinPlots@@NAME@@ * (process.sdDijetsPlotterInf@@NAME@@ + process.sdDijetsPlotter5@@NAME@@ + process.sdDijetsPlotter1@@NAME@@))
process.pRandom@@NAME@@ = cms.Path(process.genEventKTValue * process.GenScaleFilter * process.WeightProducer * process.TrackMultiplicity * process.RecoKinPlots * process.SDDijetsHLTFilter * process.HLTKinPlots * process.GapSideProducerRandom@@NAME@@ * process.DijetsFilterRandom@@NAME@@ * process.OfflineKinPlotsRandom@@NAME@@  * (process.sdDijetsPlotterInfRandom@@NAME@@ + process.sdDijetsPlotter5Random@@NAME@@ + process.sdDijetsPlotter1Random@@NAME@@))

#process.p = cms.Path(process.genEventKTValue * process.CastorTowerReco * process.CastorFullReco * process.L2L3CorJetSC5Calo * process.sdDijets)
#process.p = cms.Path(process.CastorTowerReco * process.CastorFullReco * process.L2L3CorJetSC5Calo * process.sdDijets)
#process.p = cms.Path(process.dump)


#process.p = cms.Path(process.myProducerLabel)

#process.e = cms.EndPath(process.out)



"""

    crab_cfg = """
[CRAB]

jobtype = cmssw

scheduler = glite

#server_name = cnaf

[CMSSW]
#datasetpath=/PomWigDiJetsPlusPt30/Summer08_IDEAL_V9_v3/GEN-SIM-RECO
datasetpath=@@DATASET@@

# get_edm_output = 1

### To use a local DBS istance specify the related URL here.
dbs_url = https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_02_writer/servlet/DBSServlet

### The name of ParameterSet to be used
pset=pset.py


#total_number_of_events=1
#number_of_jobs=1
total_number_of_events=-1
#events_per_job = @@NJOBS@@
number_of_jobs = @@NJOBS@@

### The output files produced by your application (comma separated list)
#output_file = SDDijetsAnaNtuple.root

[USER]

#eMail = your_email_address
#thresholdLevel = 100

ui_working_dir = @@UIWORKINGDIR@@

return_data = 1

copy_data = 0

#storage_element = T2_BR_UERJ
#user_remote_dir = @@REMOTEDIR@@

#storage_path=/srm/managerv2?SFN=/castor/cern.ch/
#user_remote_dir = name_directory_you_want

publish_data=0
#publish_data_name = @@REMOTEDIR@@
#dbs_url_for_publication = https://phedex.hepgrid.uerj.br:8443/DBS/servlet/DBSServlet

#additional_input_files = file1, file2, /full/path/file3

#script_exe=file_exe

#dontCheckSpaceLeft = 1


[EDG]
proxy_server = myproxy.cern.ch

#se_black_list = T0,T1
#se_white_list = kfki
@@USESEWHITELIST@@se_white_list = @@SEWHITELIST@@

#ce_black_list = infn
#ce_white_list = infn

virtual_organization = cms

"""
    print ''
    print '##########################################'
    print 'Removing old stuff...'
    print '##########################################'
    os.system('rm pset.py')
    os.system('rm crab.cfg')
    print ''
    print '##########################################'
    print 'Creating PSet file...'
    print '##########################################'
    pset_py1 = pset_py1.replace('@@LUMI@@',lumi,-1)
    pset_py1 = pset_py1.replace('@@RGSP@@',rgsp,-1)
    pset_py1 = pset_py1.replace('@@EFF@@',eff,-1)
    pset_py1 = pset_py1.replace('@@GENERATOR@@',generator,-1)

    pset_py = pset_py1
    for param in parametersList:
        pset_py2Bias = pset_py2
        pset_py2Bias = pset_py2Bias.replace('@@HFNOISE@@',param["hfnoise"],-1)
        pset_py2Bias = pset_py2Bias.replace('@@CASTORNOISE@@',param["castornoise"],-1)
        pset_py2Bias = pset_py2Bias.replace('@@NAME@@',param["name"],-1)
        pset_py = pset_py + pset_py2Bias


    pset_pyFile = open('pset.py','a+')
    pset_pyFile.write(pset_py)
    pset_pyFile.close()
    print ''
    print '##########################################'
    print 'Creating CRAB Config file...'
    print '##########################################'
    crab_cfg = crab_cfg.replace('@@DATASET@@',dataset,-1)
    crab_cfg = crab_cfg.replace('@@NJOBS@@',njobs,-1)
    crab_cfg = crab_cfg.replace('@@UIWORKINGDIR@@',uiworkingdir,-1)
    crab_cfg = crab_cfg.replace('@@SEWHITELIST@@',sewhitelist,-1)
    #use or not se_white_list
    if (sewhitelist == ''):
        crab_cfg = crab_cfg.replace('@@USESEWHITELIST@@','#',-1)
    else:
        crab_cfg = crab_cfg.replace('@@USESEWHITELIST@@','',-1)
    crab_cfgFile = open('crab.cfg','a+')
    crab_cfgFile.write(crab_cfg)
    crab_cfgFile.close()
    ##crab or cmsRun
    crabCall(uiworkingdir)
    #cmsRunCall()
    #clear
    print ''
    print '##########################################'
    print 'Backuping crab.cfg and pset.py...'
    print '##########################################'
    os.system('mkdir -p '+uiworkingdir+'/crabCallerBackup')
    os.system('cp crab.cfg '+uiworkingdir+'/crabCallerBackup/.')
    os.system('cp pset.py '+uiworkingdir+'/crabCallerBackup/.')
    print ''
    print 'Backups saved: '+uiworkingdir+'/crabCallerBackup/'
    print ''
    print ''
    print '##########################################'
    print 'Cleaning...'
    print '##########################################'
    os.system('rm pset.py')
    os.system('rm crab.cfg')

################################################################################################################################
################################################################################################################################
################################################################################################################################

print ''
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'
print 'Starting submission...'
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'

################################################################################################################################
################################################################################################################################
################################################################################################################################

lumi = '10.0'
rgsp = '1.0'
eff = '0.02'

parametersList = []
##name pattern: HfHFNOISECastorCASTORNOISE
Hf00Castor00 = {"name":"Hf00Castor00","hfnoise":"0.0","castornoise":"0.0"}
parametersList.append(Hf00Castor00)

Hf20Castor50 = {"name":"Hf20Castor50","hfnoise":"2.0","castornoise":"5.0"}
parametersList.append(Hf20Castor50)

Hf45Castor50 = {"name":"Hf45Castor50","hfnoise":"4.5","castornoise":"5.0"}
parametersList.append(Hf45Castor50)

Hf20Castor100 = {"name":"Hf20Castor100","hfnoise":"2.0","castornoise":"10.0"}
parametersList.append(Hf20Castor100)


Hf21Castor100 = {"name":"Hf21Castor100","hfnoise":"2.1","castornoise":"10.0"}
parametersList.append(Hf21Castor100)


Hf22Castor100 = {"name":"Hf22Castor100","hfnoise":"2.2","castornoise":"10.0"}
parametersList.append(Hf22Castor100)


Hf23Castor100 = {"name":"Hf23Castor100","hfnoise":"2.3","castornoise":"10.0"}
parametersList.append(Hf23Castor100)


Hf24Castor100 = {"name":"Hf24Castor100","hfnoise":"2.4","castornoise":"10.0"}
parametersList.append(Hf24Castor100)


Hf25Castor100 = {"name":"Hf25Castor100","hfnoise":"2.5","castornoise":"10.0"}
parametersList.append(Hf25Castor100)


Hf26Castor100 = {"name":"Hf26Castor100","hfnoise":"2.6","castornoise":"10.0"}
parametersList.append(Hf26Castor100)


Hf27Castor100 = {"name":"Hf27Castor100","hfnoise":"2.7","castornoise":"10.0"}
parametersList.append(Hf27Castor100)


Hf28Castor100 = {"name":"Hf28Castor100","hfnoise":"2.8","castornoise":"10.0"}
parametersList.append(Hf28Castor100)


Hf29Castor100 = {"name":"Hf29Castor100","hfnoise":"2.9","castornoise":"10.0"}
parametersList.append(Hf29Castor100)


Hf30Castor100 = {"name":"Hf30Castor100","hfnoise":"3.0","castornoise":"10.0"}
parametersList.append(Hf30Castor100)


Hf31Castor100 = {"name":"Hf31Castor100","hfnoise":"3.1","castornoise":"10.0"}
parametersList.append(Hf31Castor100)


Hf32Castor100 = {"name":"Hf32Castor100","hfnoise":"3.2","castornoise":"10.0"}
parametersList.append(Hf32Castor100)


Hf33Castor100 = {"name":"Hf33Castor100","hfnoise":"3.3","castornoise":"10.0"}
parametersList.append(Hf33Castor100)


Hf34Castor100 = {"name":"Hf34Castor100","hfnoise":"3.4","castornoise":"10.0"}
parametersList.append(Hf34Castor100)


Hf35Castor100 = {"name":"Hf35Castor100","hfnoise":"3.5","castornoise":"10.0"}
parametersList.append(Hf35Castor100)


Hf36Castor100 = {"name":"Hf36Castor100","hfnoise":"3.6","castornoise":"10.0"}
parametersList.append(Hf36Castor100)


Hf37Castor100 = {"name":"Hf37Castor100","hfnoise":"3.7","castornoise":"10.0"}
parametersList.append(Hf37Castor100)


Hf38Castor100 = {"name":"Hf38Castor100","hfnoise":"3.8","castornoise":"10.0"}
parametersList.append(Hf38Castor100)


Hf39Castor100 = {"name":"Hf39Castor100","hfnoise":"3.9","castornoise":"10.0"}
parametersList.append(Hf39Castor100)


Hf40Castor100 = {"name":"Hf40Castor100","hfnoise":"4.0","castornoise":"10.0"}
parametersList.append(Hf40Castor100)


Hf41Castor100 = {"name":"Hf41Castor100","hfnoise":"4.1","castornoise":"10.0"}
parametersList.append(Hf41Castor100)


Hf42Castor100 = {"name":"Hf42Castor100","hfnoise":"4.2","castornoise":"10.0"}
parametersList.append(Hf42Castor100)


Hf43Castor100 = {"name":"Hf43Castor100","hfnoise":"4.3","castornoise":"10.0"}
parametersList.append(Hf43Castor100)


Hf44Castor100 = {"name":"Hf44Castor100","hfnoise":"4.4","castornoise":"10.0"}
parametersList.append(Hf44Castor100)


Hf45Castor100 = {"name":"Hf45Castor100","hfnoise":"4.5","castornoise":"10.0"}
parametersList.append(Hf45Castor100)


Hf46Castor100 = {"name":"Hf46Castor100","hfnoise":"4.6","castornoise":"10.0"}
parametersList.append(Hf46Castor100)


Hf47Castor100 = {"name":"Hf47Castor100","hfnoise":"4.7","castornoise":"10.0"}
parametersList.append(Hf47Castor100)


Hf48Castor100 = {"name":"Hf48Castor100","hfnoise":"4.8","castornoise":"10.0"}
parametersList.append(Hf48Castor100)


Hf49Castor100 = {"name":"Hf49Castor100","hfnoise":"4.9","castornoise":"10.0"}
parametersList.append(Hf49Castor100)


Hf50Castor100 = {"name":"Hf50Castor100","hfnoise":"5.0","castornoise":"10.0"}
parametersList.append(Hf50Castor100)


Hf51Castor100 = {"name":"Hf51Castor100","hfnoise":"5.1","castornoise":"10.0"}
parametersList.append(Hf51Castor100)


Hf52Castor100 = {"name":"Hf52Castor100","hfnoise":"5.2","castornoise":"10.0"}
parametersList.append(Hf52Castor100)


Hf53Castor100 = {"name":"Hf53Castor100","hfnoise":"5.3","castornoise":"10.0"}
parametersList.append(Hf53Castor100)


Hf54Castor100 = {"name":"Hf54Castor100","hfnoise":"5.4","castornoise":"10.0"}
parametersList.append(Hf54Castor100)


Hf55Castor100 = {"name":"Hf55Castor100","hfnoise":"5.5","castornoise":"10.0"}
parametersList.append(Hf55Castor100)


Hf56Castor100 = {"name":"Hf56Castor100","hfnoise":"5.6","castornoise":"10.0"}
parametersList.append(Hf56Castor100)


Hf57Castor100 = {"name":"Hf57Castor100","hfnoise":"5.7","castornoise":"10.0"}
parametersList.append(Hf57Castor100)


Hf58Castor100 = {"name":"Hf58Castor100","hfnoise":"5.8","castornoise":"10.0"}
parametersList.append(Hf58Castor100)


Hf59Castor100 = {"name":"Hf59Castor100","hfnoise":"5.9","castornoise":"10.0"}
parametersList.append(Hf59Castor100)


Hf60Castor100 = {"name":"Hf60Castor100","hfnoise":"6.0","castornoise":"10.0"}
parametersList.append(Hf60Castor100)


Hf61Castor100 = {"name":"Hf61Castor100","hfnoise":"6.1","castornoise":"10.0"}
parametersList.append(Hf61Castor100)


Hf62Castor100 = {"name":"Hf62Castor100","hfnoise":"6.2","castornoise":"10.0"}
parametersList.append(Hf62Castor100)


Hf63Castor100 = {"name":"Hf63Castor100","hfnoise":"6.3","castornoise":"10.0"}
parametersList.append(Hf63Castor100)


Hf64Castor100 = {"name":"Hf64Castor100","hfnoise":"6.4","castornoise":"10.0"}
parametersList.append(Hf64Castor100)


Hf65Castor100 = {"name":"Hf65Castor100","hfnoise":"6.5","castornoise":"10.0"}
parametersList.append(Hf65Castor100)


Hf66Castor100 = {"name":"Hf66Castor100","hfnoise":"6.6","castornoise":"10.0"}
parametersList.append(Hf66Castor100)


Hf67Castor100 = {"name":"Hf67Castor100","hfnoise":"6.7","castornoise":"10.0"}
parametersList.append(Hf67Castor100)


Hf68Castor100 = {"name":"Hf68Castor100","hfnoise":"6.8","castornoise":"10.0"}
parametersList.append(Hf68Castor100)


Hf69Castor100 = {"name":"Hf69Castor100","hfnoise":"6.9","castornoise":"10.0"}
parametersList.append(Hf69Castor100)


Hf70Castor100 = {"name":"Hf70Castor100","hfnoise":"7.0","castornoise":"10.0"}
parametersList.append(Hf70Castor100)


Hf71Castor100 = {"name":"Hf71Castor100","hfnoise":"7.1","castornoise":"10.0"}
parametersList.append(Hf71Castor100)


Hf72Castor100 = {"name":"Hf72Castor100","hfnoise":"7.2","castornoise":"10.0"}
parametersList.append(Hf72Castor100)


Hf73Castor100 = {"name":"Hf73Castor100","hfnoise":"7.3","castornoise":"10.0"}
parametersList.append(Hf73Castor100)


Hf74Castor100 = {"name":"Hf74Castor100","hfnoise":"7.4","castornoise":"10.0"}
parametersList.append(Hf74Castor100)


Hf75Castor100 = {"name":"Hf75Castor100","hfnoise":"7.5","castornoise":"10.0"}
parametersList.append(Hf75Castor100)


Hf76Castor100 = {"name":"Hf76Castor100","hfnoise":"7.6","castornoise":"10.0"}
parametersList.append(Hf76Castor100)


Hf77Castor100 = {"name":"Hf77Castor100","hfnoise":"7.7","castornoise":"10.0"}
parametersList.append(Hf77Castor100)


Hf78Castor100 = {"name":"Hf78Castor100","hfnoise":"7.8","castornoise":"10.0"}
parametersList.append(Hf78Castor100)


Hf79Castor100 = {"name":"Hf79Castor100","hfnoise":"7.9","castornoise":"10.0"}
parametersList.append(Hf79Castor100)


Hf80Castor100 = {"name":"Hf80Castor100","hfnoise":"8.0","castornoise":"10.0"}
parametersList.append(Hf80Castor100)


Hf81Castor100 = {"name":"Hf81Castor100","hfnoise":"8.1","castornoise":"10.0"}
parametersList.append(Hf81Castor100)


Hf82Castor100 = {"name":"Hf82Castor100","hfnoise":"8.2","castornoise":"10.0"}
parametersList.append(Hf82Castor100)


Hf83Castor100 = {"name":"Hf83Castor100","hfnoise":"8.3","castornoise":"10.0"}
parametersList.append(Hf83Castor100)


Hf84Castor100 = {"name":"Hf84Castor100","hfnoise":"8.4","castornoise":"10.0"}
parametersList.append(Hf84Castor100)


Hf85Castor100 = {"name":"Hf85Castor100","hfnoise":"8.5","castornoise":"10.0"}
parametersList.append(Hf85Castor100)


Hf86Castor100 = {"name":"Hf86Castor100","hfnoise":"8.6","castornoise":"10.0"}
parametersList.append(Hf86Castor100)


Hf87Castor100 = {"name":"Hf87Castor100","hfnoise":"8.7","castornoise":"10.0"}
parametersList.append(Hf87Castor100)


Hf88Castor100 = {"name":"Hf88Castor100","hfnoise":"8.8","castornoise":"10.0"}
parametersList.append(Hf88Castor100)


Hf89Castor100 = {"name":"Hf89Castor100","hfnoise":"8.9","castornoise":"10.0"}
parametersList.append(Hf89Castor100)


Hf90Castor100 = {"name":"Hf90Castor100","hfnoise":"9.0","castornoise":"10.0"}
parametersList.append(Hf90Castor100)


Hf91Castor100 = {"name":"Hf91Castor100","hfnoise":"9.1","castornoise":"10.0"}
parametersList.append(Hf91Castor100)


Hf92Castor100 = {"name":"Hf92Castor100","hfnoise":"9.2","castornoise":"10.0"}
parametersList.append(Hf92Castor100)


Hf93Castor100 = {"name":"Hf93Castor100","hfnoise":"9.3","castornoise":"10.0"}
parametersList.append(Hf93Castor100)


Hf94Castor100 = {"name":"Hf94Castor100","hfnoise":"9.4","castornoise":"10.0"}
parametersList.append(Hf94Castor100)


Hf95Castor100 = {"name":"Hf95Castor100","hfnoise":"9.5","castornoise":"10.0"}
parametersList.append(Hf95Castor100)


Hf96Castor100 = {"name":"Hf96Castor100","hfnoise":"9.6","castornoise":"10.0"}
parametersList.append(Hf96Castor100)


Hf97Castor100 = {"name":"Hf97Castor100","hfnoise":"9.7","castornoise":"10.0"}
parametersList.append(Hf97Castor100)


Hf98Castor100 = {"name":"Hf98Castor100","hfnoise":"9.8","castornoise":"10.0"}
parametersList.append(Hf98Castor100)


Hf99Castor100 = {"name":"Hf99Castor100","hfnoise":"9.9","castornoise":"10.0"}
parametersList.append(Hf99Castor100)


Hf100Castor100 = {"name":"Hf100Castor100","hfnoise":"10.0","castornoise":"10.0"}
parametersList.append(Hf100Castor100)



#print parametersList
################################################################################################################################
################################################################################################################################
################################################################################################################################

##signal
## https://cmsweb.cern.ch/dbs_discovery/aSearch?caseSensitive=on&userMode=user&sortOrder=desc&sortName=&grid=0&method=dbsapi&dbsInst=cms_dbs_prod_global&userInput=find+dataset+where+dataset+like+%2FPomWigDiJets*Pt*%2FSummer08_IDEAL_V9_v*%2FGEN-SIM-RECO+and+dataset+not+like+%2FPomWigDiJetsPlusPt30%2FSummer08_IDEAL_V9_v1%2FGEN-SIM-RECO*

njobs = '25'

#PomWigDiJetsMinusPt80
generator = 'pomwig'
dataset = '/PomWigDiJetsMinusPt80/fsilva-PomWigDiJetsMinusPt80_Skim_v38-bc8ec1d2a1fbbf297b1b2d0f0e2b8862/USER'
#njobs = '2000'
uiworkingdir = 'PomWigDiJetsMinusPt80_Ana_UIDIR'
#options: uerj,kfki,pic
#use '' for no se_white_list
sewhitelist = 'uerj'
ConfigHandler(generator, lumi, rgsp, eff, parametersList, dataset, njobs, uiworkingdir, sewhitelist)

#PomWigDiJetsPlusPt80
generator = 'pomwig'
dataset = '/PomWigDiJetsPlusPt80/fsilva-PomWigDiJetsPlusPt80_Skim_v38-bc8ec1d2a1fbbf297b1b2d0f0e2b8862/USER'
#njobs = '2000'
uiworkingdir = 'PomWigDiJetsPlusPt80_Ana_UIDIR'
#options: uerj,kfki,pic
#use '' for no se_white_list
sewhitelist = 'uerj'
ConfigHandler(generator, lumi, rgsp, eff, parametersList, dataset, njobs, uiworkingdir, sewhitelist)

#PomWigDiJetsMinusPt30
generator = 'pomwig'
dataset = '/PomWigDiJetsMinusPt30/fsilva-PomWigDiJetsMinusPt30_Skim_v38-264ca8f6f534226335efe858355695c5/USER'
#njobs = '2000'
uiworkingdir = 'PomWigDiJetsMinusPt30_Ana_UIDIR'
#options: uerj,kfki,pic
#use '' for no se_white_list
sewhitelist = 'uerj'
ConfigHandler(generator, lumi, rgsp, eff, parametersList, dataset, njobs, uiworkingdir, sewhitelist)

#PomWigDiJetsPlusPt30
generator = 'pomwig'
dataset = '/PomWigDiJetsPlusPt30/fsilva-PomWigDiJetsPlusPt30_Skim_v38-264ca8f6f534226335efe858355695c5/USER'
#njobs = '2000'
uiworkingdir = 'PomWigDiJetsPlusPt30_Ana_UIDIR'
#options: uerj,kfki,pic
#use '' for no se_white_list
sewhitelist = 'uerj'
ConfigHandler(generator, lumi, rgsp, eff, parametersList, dataset, njobs, uiworkingdir, sewhitelist)

##############################################################################################################################
##BACKGROUND
## https://cmsweb.cern.ch/dbs_discovery/aSearch?dbsInst=cms_dbs_prod_global&userMode=user&fromRow=0&_idx=0&pagerStep=0&userInput=find%20dataset%20where%20dataset%20=%20/QCDpt170/Summer08_IDEAL_V9_v3/GEN-SIM-RECO%20OR%20dataset%20=%20/QCDpt30/Summer08_IDEAL_V9_v4/GEN-SIM-RECO%20OR%20dataset%20=%20/QCDpt80/Summer08_IDEAL_V9_v2/GEN-SIM-RECO%20OR%20dataset%20=%20/QCDpt170/Summer08_IDEAL_V9_v3/GEN-SIM-RECO%20OR%20dataset%20=%20/QCDpt300/Summer08_IDEAL_V9_v1/GEN-SIM-RECO%20OR%20dataset%20=%20/QCDpt470/Summer08_IDEAL_V9_v1/GEN-SIM-RECO%20OR%20dataset%20=%20/QCDpt800/Summer08_IDEAL_V9_reco-v4/GEN-SIM-RECO%20OR%20dataset%20=%20/QCDpt1400/Summer08_IDEAL_V9_reco-v1/GEN-SIM-RECO%20OR%20dataset%20=%20/QCDpt2200/Summer08_IDEAL_V9_reco-v1/GEN-SIM-RECO%20OR%20dataset%20=%20/QCDpt3000/Summer08_IDEAL_V9_reco-v2/GEN-SIM-RECO%20OR%20dataset%20=%20/QCDpt15/Summer08_IDEAL_V9_v3/GEN-SIM-RECO%20&sortName=PATH&sortOrder=desc&caseSensitive=on&grid=0

njobs = '50'
#njobs = '50000'

##QCDpt15
generator = 'pythia'
dataset = '/QCDpt15/fsilva-QCDpt15_Skim_v38-038992d5f7c346931b7b52cd62f8eaa7/USER'
#njobs = '2000'
uiworkingdir = 'QCDpt15_Ana_UIDIR'
##options: T2_IT_Pisa*,maite.iihe.ac.be*,T1_FR_CCIN2P3*,T2_FR_IPHC*,T2_FR_GRIF_LLR*
##use '' for no se_white_list
sewhitelist = 'uerj'
ConfigHandler(generator, lumi, rgsp, eff, parametersList, dataset, njobs, uiworkingdir, sewhitelist)


##QCDpt30
generator = 'pythia'
dataset = '/QCDpt30/fsilva-QCDpt30_Skim_v38-7ae8f8e09ff57ea325845c0928fc987c/USER'
#njobs = '2000'
uiworkingdir = 'QCDpt30_Ana_UIDIR'
##options: ,T2_IT_Pisa,T2_US_Wisconsin,T1_FR_CCIN2P3,T2_FR_IPHC,T2_CN_Beijing
##use '' for no se_white_list
sewhitelist = 'uerj'
ConfigHandler(generator, lumi, rgsp, eff, parametersList, dataset, njobs, uiworkingdir, sewhitelist)


##QCDpt80
generator = 'pythia'
dataset = '/QCDpt80/fsilva-QCDpt80_Skim_v38-b2cc5d52897cf0306f05a3593a23e96b/USER'
#njobs = '2000'
uiworkingdir = 'QCDpt80_Ana_UIDIR'
##options: T2_IT_Pisa,T1_FR_CCIN2P3
##use '' for no se_white_list
sewhitelist = 'uerj'
ConfigHandler(generator, lumi, rgsp, eff, parametersList, dataset, njobs, uiworkingdir, sewhitelist)


##QCDpt170
generator = 'pythia'
dataset = '/QCDpt170/fsilva-QCDpt170_Skim_v38-bf9168895291c75ea910f25a08806c90/USER'
#njobs = '2000'
uiworkingdir = 'QCDpt170_Ana_UIDIR'
##options: T2_IT_Pisa,T1_FR_CCIN2P3
##use '' for no se_white_list
sewhitelist = 'uerj'
ConfigHandler(generator, lumi, rgsp, eff, parametersList, dataset, njobs, uiworkingdir, sewhitelist)


##QCDpt300
generator = 'pythia'
dataset = '/QCDpt300/fsilva-QCDpt300_Skim_v38-a61eac8d3164a709cd9db55b3cd1f193/USER'
#njobs = '2000'
uiworkingdir = 'QCDpt300_Ana_UIDIR'
##options: T2_IT_Pisa,T1_FR_CCIN2P3
##use '' for no se_white_list
sewhitelist = 'uerj'
ConfigHandler(generator, lumi, rgsp, eff, parametersList, dataset, njobs, uiworkingdir, sewhitelist)


##QCDpt470
generator = 'pythia'
dataset = '/QCDpt470/fsilva-QCDpt470_Skim_v38-d44df2a3b3d981a6d30a1813cfc01e42/USER'
#njobs = '2000'
uiworkingdir = 'QCDpt470_Ana_UIDIR'
##options: T2_IT_Pisa,T1_UK_RAL
##use '' for no se_white_list
sewhitelist = 'uerj'
ConfigHandler(generator, lumi, rgsp, eff, parametersList, dataset, njobs, uiworkingdir, sewhitelist)


##QCDpt800
generator = 'pythia'
dataset = '/QCDpt800/fsilva-QCDpt800_Skim_v38-f7730813d8a1551994ad35715ee2c590/USER'
#njobs = '2000'
uiworkingdir = 'QCDpt800_Ana_UIDIR'
##options: T1_UK_RAL*,se2.ppgrid1.rhul.ac.uk*,T2_UK_SGrid_RALPP*,T2_EE_Estonia*
##use '' for no se_white_list
sewhitelist = 'uerj'
ConfigHandler(generator, lumi, rgsp, eff, parametersList, dataset, njobs, uiworkingdir, sewhitelist)


##QCDpt1400
generator = 'pythia'
dataset = '/QCDpt1400/fsilva-QCDpt1400_Skim_v38-219d9b583015489183fad76b45ef7848/USER'
#njobs = '2000'
uiworkingdir = 'QCDpt1400_Ana_UIDIR'
##options: T1_UK_RAL*,T2_EE_Estonia*
##use '' for no se_white_list
sewhitelist = 'uerj'
ConfigHandler(generator, lumi, rgsp, eff, parametersList, dataset, njobs, uiworkingdir, sewhitelist)


##QCDpt2200
generator = 'pythia'
dataset = '/QCDpt2200/fsilva-QCDpt2200_Skim_v38-f1a2315ae0c434fd0383c314e4f061f6/USER'
#njobs = '2000'
uiworkingdir = 'QCDpt2200_Ana_UIDIR'
##options: T2_EE_Estonia
##use '' for no se_white_list
sewhitelist = 'uerj'
ConfigHandler(generator, lumi, rgsp, eff, parametersList, dataset, njobs, uiworkingdir, sewhitelist)

##############################################################################################################################
##############################################################################################################################
##############################################################################################################################

print ''
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'
print 'Finishing submission...'
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'



