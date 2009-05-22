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
def ConfigHandler(correction,cross_section,dataset,njobs,uiworkingdir,remotedir,sewhitelist,generator):
    pset_py = """
import FWCore.ParameterSet.Config as cms

process = cms.Process("SDDijetsAna")

process.load("FWCore.MessageService.MessageLogger_cfi")
# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.categories.append('SDDijetsAna')
process.MessageLogger.cerr.INFO = cms.untracked.PSet(
    default          = cms.untracked.PSet( limit = cms.untracked.int32(0)  ),
    Demo = cms.untracked.PSet( limit = cms.untracked.int32(1000) )
)
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(2000) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        #'rfio:/castor/cern.ch/user/f/fsilva/POMWIG_SingleDiffractiveDijetsMinus_10TeV_Pt_30_cff_py_GEN_FASTSIM.root'
        'rfio:/castor/cern.ch/user/f/fsilva/sample_summer08.root'
        #'file:/tmp/fsilva/sample_summer08.root'
       #'dcap://se-dcache.hepgrid.uerj.br/////pnfs/hepgrid.uerj.br/data/cms/store/mc/Summer08/PomWigDiJetsMinusPt30/GEN-SIM-RECO/IDEAL_V9_v1/0000/007A81C2-3FEB-DD11-B5FB-00163E1124E5.root',
       #'dcap://se-dcache.hepgrid.uerj.br/////pnfs/hepgrid.uerj.br/data/cms/store/mc/Summer08/PomWigDiJetsMinusPt30/GEN-SIM-RECO/IDEAL_V9_v1/0003/F2223124-C587-DD11-927D-00163E1124E4.root',
       #'dcap://se-dcache.hepgrid.uerj.br/////pnfs/hepgrid.uerj.br/data/cms/store/mc/Summer08/PomWigDiJetsMinusPt30/GEN-SIM-RECO/IDEAL_V9_v1/0003/F2A44EC1-C487-DD11-BE1C-00163E1124EC.root'
        #'/store/relval/CMSSW_2_1_9/RelValSingleMuPt100/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/IDEAL_V9_v2/0000/2E29E000-A985-DD11-8C04-000423D98950.root'
        #'file:/home/felipe/CMSSW_2_2_3/src/sample_2_2_3.root'
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


### Necessary fixes to run 2.2.X on 2.1.X data
#from PhysicsTools.PatAlgos.tools.cmsswVersionTools import run22XonSummer08AODSIM
#run22XonSummer08AODSIM(process)


#process.source.inputCommands = cms.untracked.vstring(
#          'keep *',
#          'drop *_particleFlowBlock_*_*' )
#



##Trigger Filter
import copy
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
process.SDDijetsHLTFilter = copy.deepcopy(hltHighLevel)
process.SDDijetsHLTFilter.TriggerResultsTag = cms.InputTag("TriggerResults","","HLT")
process.SDDijetsHLTFilter.HLTPaths = ['HLT_DiJetAve30']

process.prescale = cms.EDFilter("Prescaler",
    prescaleFactor = cms.int32(50)
)




#process.TFileService = cms.Service("TFileService",
#                                   fileName = cms.string("histos.root")
# )

#############   Include the jet corrections ##########
process.load("JetMETCorrections.Configuration.@@CORRECTION@@")
#process.load("JetMETCorrections.Configuration.L2L3Corrections_Winter09_cff")
# set the record's IOV. Must be defined once. Choose ANY correction service. #
process.prefer("L2L3JetCorrectorSC5Calo")


#### Castor FastSimulation
process.load('FastSimulation.ForwardDetectors.CastorTowerProducer_cfi')
#### Castor Reconstruction
process.load('RecoLocalCalo.Castor.Castor_cfi')

###genEventKTValue
process.load("PhysicsTools.HepMCCandAlgos.genEventKTValue_cfi")

process.sdDijets = cms.EDProducer('SingleDiffractiveDijetsAnalysis',
    weightFromCFG = cms.double(@@CROSS_SECTION@@),
    CastorGenEnergyCut = cms.double(10.0),
    #generator = cms.string("pythia")
    generator = cms.string("@@GENERATOR@@")
)







process.out = cms.OutputModule("PoolOutputModule",
#process.out = cms.OutputModule("AsciiOutputModule",
    outputCommands = cms.untracked.vstring('drop *',
    'keep  *_TriggerResults_*_HLT',
    'keep  *_towerMaker_*_*',
    'keep  recoGenParticles_genParticles_*_HLT',
    'keep  *_sisCone5GenJets_*_*',
    'keep  recoTracks_generalTracks_*_*',
    'keep  *_genEventScale_*_HLT',
    'keep  *_*_*_SDDijetsAna',
    'drop  *_TriggerResults_*_SDDijetsAna',
    'drop  *_CastorFullReco_*_SDDijetsAna',
    'drop  *_CastorFullReco_*_SDDijetsAna',
    'drop  *_CastorEgammas_*_SDDijetsAna'),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('p')
    ),
    fileName = cms.untracked.string('SDDijetsAnaNtuple.root')
)


#process.out = cms.OutputModule("PoolOutputModule",
##process.out = cms.OutputModule("AsciiOutputModule",
#    outputCommands = cms.untracked.vstring('drop *',
#    'keep  *_TriggerResults_*_HLT',
#    #'keep  *_towerMaker_*_*',
#    'keep  *_genParticles_*_*',
#    'keep  *_sisCone5GenJets_*_*',
#    'keep  *_generalTracks_*_*',
#    'keep  *_genEventScale_*_*',
#    'keep  *_*_*_SDDijetsAna',
#    #'keep *_hltTriggerSummaryAOD_*_*',
#    #'keep *_offlineBeamSpot_*_*',
#    #'keep *_offlinePrimaryVertices_*_*',
#    #'drop  *_*_genParticlesAllStable_SDDijetsAna',
#    'drop  *_TriggerResults_*_SDDijetsAna',
#    #'drop  *_L2L3CorJetSC5Calo_*_SDDijetsAna',
#    'drop  *_CastorTowerReco_*_SDDijetsAna',
#    'drop  *_CastorFullReco_*_SDDijetsAna',
#    'drop  *_CastorEgammas_*_SDDijetsAna'),
##    SelectEvents = cms.untracked.PSet(
##        SelectEvents = cms.vstring('p')
##    ),
#    fileName = cms.untracked.string('SDDijetsAnaNtuple.root')
#)


process.dump=cms.EDAnalyzer('EventContentAnalyzer')
#process.p = cms.Path(process.L2L3CorJetSC5Calo)
#process.p = cms.Path(process.SDDijetsHLTFilter * process.CastorTowerReco * process.CastorFullReco * process.L2L3CorJetSC5Calo)
#process.p = cms.Path(process.genEventKTValue * process.CastorTowerReco * process.CastorFullReco * process.L2L3CorJetSC5Calo * process.sdDijets)
process.p = cms.Path(process.SDDijetsHLTFilter * process.genEventKTValue * process.CastorTowerReco * process.CastorFullReco * process.L2L3CorJetSC5Calo )
#process.p = cms.Path(process.SDDijetsHLTFilter * process.genEventKTValue * process.CastorTowerReco * process.CastorFullReco * process.L2L3CorJetSC5Calo )
#process.p = cms.Path(process.CastorTowerReco * process.CastorFullReco * process.L2L3CorJetSC5Calo * process.sdDijets)
#process.p = cms.Path(process.dump)


#process.p = cms.Path(process.myProducerLabel)

process.e = cms.EndPath(process.out)


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
# dbs_url = http://cmsdoc.cern.ch/cms/test/aprom/DBS/CGIServer/prodquery

### The name of ParameterSet to be used
pset=pset.py
skip_TFileService_output=1

#total_number_of_events=100
#number_of_jobs=10

total_number_of_events=-1
#events_per_job = @@NJOBS@@
number_of_jobs = @@NJOBS@@

### The output files produced by your application (comma separated list)
output_file = SDDijetsAnaNtuple.root

[USER]

#eMail = your_email_address
#thresholdLevel = 100

ui_working_dir = @@UIWORKINGDIR@@

return_data = 0

copy_data = 1

storage_element = T2_BR_UERJ
user_remote_dir = @@REMOTEDIR@@

#storage_path=/srm/managerv2?SFN=/castor/cern.ch/
#user_remote_dir = name_directory_you_want

publish_data=1
publish_data_name = @@REMOTEDIR@@
dbs_url_for_publication = https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_02_writer/servlet/DBSServlet

#additional_input_files = file1, file2, /full/path/file3

#script_exe=file_exe

#dontCheckSpaceLeft = 1


[EDG]
proxy_server = myproxy.cern.ch

#se_black_list = T0,T1
se_black_list = wisc
@@USESEWHITELIST@@se_white_list = @@SEWHITELIST@@

ce_black_list = gpn-husker.unl.edu
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
    pset_py = pset_py.replace('@@CORRECTION@@',correction,-1)
    pset_py = pset_py.replace('@@CROSS_SECTION@@',cross_section,-1)
    pset_py = pset_py.replace('@@GENERATOR@@',generator,-1)
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
    crab_cfg = crab_cfg.replace('@@REMOTEDIR@@',remotedir,-1)
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
#    cmsRunCall()
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
##signal
## https://cmsweb.cern.ch/dbs_discovery/aSearch?caseSensitive=on&userMode=user&sortOrder=desc&sortName=&grid=0&method=dbsapi&dbsInst=cms_dbs_prod_global&userInput=find+dataset+where+dataset+like+%2Fpomwigdijets*winter09*reco

njobs = '50'

##PomWigDiJetsMinusPt80
correction = 'L2L3Corrections_Winter09_cff'
cross_section = '4000'
dataset = '/PomWigDiJetsMinusPt80/Winter09_IDEAL_V12_FastSim_v2/GEN-SIM-DIGI-RECO'
#njobs = '2000'
uiworkingdir = 'PomWigDiJetsMinusPt80_FastSim_UIDIR'
remotedir = 'PomWigDiJetsMinusPt80_Skim_FastSim_v2'
##use '' for no se_white_list
sewhitelist = ''
generator = 'pomwig'
#ConfigHandler(correction,cross_section,dataset,njobs,uiworkingdir,remotedir,sewhitelist,generator)

##PomWigDiJetsPlusPt80
correction = 'L2L3Corrections_Winter09_cff'
cross_section = '4000'
dataset = '/PomWigDiJetsPlusPt80/Winter09_IDEAL_V12_FastSim_v2/GEN-SIM-DIGI-RECO'
#njobs = '2000'
uiworkingdir = 'PomWigDiJetsPlusPt80_FastSim_UIDIR'
remotedir = 'PomWigDiJetsPlusPt80_Skim_FastSim_v2'
##use '' for no se_white_list
sewhitelist = ''
generator = 'pomwig'
#ConfigHandler(correction,cross_section,dataset,njobs,uiworkingdir,remotedir,sewhitelist,generator)

##PomWigDiJetsMinusPt30
correction = 'L2L3Corrections_Winter09_cff'
cross_section = '300000'
dataset = '/PomWigDiJetsMinusPt30/Winter09_IDEAL_V12_FastSim_v2/GEN-SIM-DIGI-RECO'
#njobs = '2000'
uiworkingdir = 'PomWigDiJetsMinusPt30_FastSim_UIDIR'
remotedir = 'PomWigDiJetsMinusPt30_Skim_FastSim_v2'
##use '' for no se_white_list
sewhitelist = ''
generator = 'pomwig'
#ConfigHandler(correction,cross_section,dataset,njobs,uiworkingdir,remotedir,sewhitelist,generator)

##PomWigDiJetsPlusPt30
correction = 'L2L3Corrections_Winter09_cff'
cross_section = '300000'
dataset = '/PomWigDiJetsPlusPt30/Winter09_IDEAL_V12_FastSim_v2/GEN-SIM-DIGI-RECO'
#njobs = '2000'
uiworkingdir = 'PomWigDiJetsPlusPt30_FastSim_UIDIR'
remotedir = 'PomWigDiJetsPlusPt30_Skim_FastSim_v2'
##options: uerj,kfki,pic
##use '' for no se_white_list
sewhitelist = ''
generator = 'pomwig'
#ConfigHandler(correction,cross_section,dataset,njobs,uiworkingdir,remotedir,sewhitelist,generator)

##############################################################################################################################
##BACKGROUND
## https://cmsweb.cern.ch/dbs_discovery/aSearch?dbsInst=cms_dbs_prod_global&userMode=user&fromRow=0&_idx=0&pagerStep=0&userInput=find%20dataset%20where%20dataset%20like%20/QCDpt*/Winter09_IDEAL_V1*_FastSim_v*/GEN-SIM-DIGI-RECO%20&sortName=PATH&sortOrder=desc&caseSensitive=on&grid=0

njobs = '50'
#njobs = '50000'

##QCDpt15
correction = 'L2L3Corrections_Winter09_cff'
cross_section = '1457159248'
dataset = '/QCDpt15/Winter09_IDEAL_V11_FastSim_v1/GEN-SIM-DIGI-RECO'
#njobs = '2000'
uiworkingdir = 'QCDpt15_FastSim_UIDIR'
remotedir = 'QCDpt15_Skim_FastSim_v2'
##use '' for no se_white_list
sewhitelist = ''
generator = 'pythia'

njobs = '100'

ConfigHandler(correction,cross_section,dataset,njobs,uiworkingdir,remotedir,sewhitelist,generator)

njobs = '50'

##QCDpt30
correction = 'L2L3Corrections_Winter09_cff'
cross_section = '109057220.4'
dataset = '/QCDpt30/Winter09_IDEAL_V11_FastSim_v1/GEN-SIM-DIGI-RECO'
#njobs = '2000'
uiworkingdir = 'QCDpt30_FastSim_UIDIR'
remotedir = 'QCDpt30_Skim_FastSim_v2'
##use '' for no se_white_list
sewhitelist = ''
generator = 'pythia'
ConfigHandler(correction,cross_section,dataset,njobs,uiworkingdir,remotedir,sewhitelist,generator)


##QCDpt80
correction = 'L2L3Corrections_Winter09_cff'
cross_section = '1934639.567'
dataset = '/QCDpt80/Winter09_IDEAL_V11_FastSim_v1/GEN-SIM-DIGI-RECO'
#njobs = '2000'
uiworkingdir = 'QCDpt80_FastSim_UIDIR'
remotedir = 'QCDpt80_Skim_FastSim_v2'
##use '' for no se_white_list
sewhitelist = ''
generator = 'pythia'
ConfigHandler(correction,cross_section,dataset,njobs,uiworkingdir,remotedir,sewhitelist,generator)


##QCDpt170
correction = 'L2L3Corrections_Winter09_cff'
cross_section = '62562.87713'
dataset = '/QCDpt170/Winter09_IDEAL_V11_FastSim_v1/GEN-SIM-DIGI-RECO'
#njobs = '2000'
uiworkingdir = 'QCDpt170_FastSim_UIDIR'
remotedir = 'QCDpt170_Skim_FastSim_v2'
##use '' for no se_white_list
sewhitelist = ''
generator = 'pythia'
ConfigHandler(correction,cross_section,dataset,njobs,uiworkingdir,remotedir,sewhitelist,generator)


##QCDpt300
correction = 'L2L3Corrections_Winter09_cff'
cross_section = '3664.608301'
dataset = '/QCDpt300/Winter09_IDEAL_V11_FastSim_v1/GEN-SIM-DIGI-RECO'
#njobs = '2000'
uiworkingdir = 'QCDpt300_FastSim_UIDIR'
remotedir = 'QCDpt300_Skim_FastSim_v2'
##use '' for no se_white_list
sewhitelist = ''
generator = 'pythia'
ConfigHandler(correction,cross_section,dataset,njobs,uiworkingdir,remotedir,sewhitelist,generator)


##QCDpt470
correction = 'L2L3Corrections_Winter09_cff'
cross_section = '315.5131272'
dataset = '/QCDpt470/Winter09_IDEAL_V11_FastSim_v1/GEN-SIM-DIGI-RECO'
#njobs = '2000'
uiworkingdir = 'QCDpt470_FastSim_UIDIR'
remotedir = 'QCDpt470_Skim_FastSim_v2'
##use '' for no se_white_list
sewhitelist = ''
generator = 'pythia'
ConfigHandler(correction,cross_section,dataset,njobs,uiworkingdir,remotedir,sewhitelist,generator)


##QCDpt800
correction = 'L2L3Corrections_Winter09_cff'
cross_section = '11.9419745'
dataset = '/QCDpt800/Winter09_IDEAL_V11_FastSim_v1/GEN-SIM-DIGI-RECO'
#njobs = '2000'
uiworkingdir = 'QCDpt800_FastSim_UIDIR'
remotedir = 'QCDpt800_Skim_FastSim_v2'
##use '' for no se_white_list
sewhitelist = ''
generator = 'pythia'
ConfigHandler(correction,cross_section,dataset,njobs,uiworkingdir,remotedir,sewhitelist,generator)


##QCDpt1400
correction = 'L2L3Corrections_Winter09_cff'
cross_section = '0.1720187189'
dataset = '/QCDpt1400/Winter09_IDEAL_V11_FastSim_v1/GEN-SIM-DIGI-RECO'
#njobs = '2000'
uiworkingdir = 'QCDpt1400_FastSim_UIDIR'
remotedir = 'QCDpt1400_Skim_FastSim_v2'
##use '' for no se_white_list
sewhitelist = ''
generator = 'pythia'
ConfigHandler(correction,cross_section,dataset,njobs,uiworkingdir,remotedir,sewhitelist,generator)


##QCDpt2200
correction = 'L2L3Corrections_Winter09_cff'
cross_section = '0.0014207778'
dataset = '/QCDpt2200/Winter09_IDEAL_V11_FastSim_v1/GEN-SIM-DIGI-RECO'
#njobs = '2000'
uiworkingdir = 'QCDpt2200_FastSim_UIDIR'
remotedir = 'QCDpt2200_Skim_FastSim_v2'
##use '' for no se_white_list
sewhitelist = ''
generator = 'pythia'
ConfigHandler(correction,cross_section,dataset,njobs,uiworkingdir,remotedir,sewhitelist,generator)

##############################################################################################################################
##############################################################################################################################
##############################################################################################################################

print ''
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'
print 'Finishing submission...'
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'
print ''


