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
def ConfigHandler(dataset,njobs,uiworkingdir,sewhitelist,generator):
    pset_py = """
import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(2000) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
       '/store/user/fsilva/PomWigDiJetsMinusPt30/PomWigDiJetsMinusPt30_Skim_v27/7a413468bb74a0436c27225230c9b23c/SDDijetsAnaNtuple_10.root',
       '/store/user/fsilva/PomWigDiJetsMinusPt30/PomWigDiJetsMinusPt30_Skim_v27/7a413468bb74a0436c27225230c9b23c/SDDijetsAnaNtuple_51.root',
       '/store/user/fsilva/PomWigDiJetsMinusPt30/PomWigDiJetsMinusPt30_Skim_v27/7a413468bb74a0436c27225230c9b23c/SDDijetsAnaNtuple_98.root'
    )
)

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("NevtOnBins.root")
)

process.NetvOnBins = cms.EDAnalyzer('NetvOnBins',
    generator = cms.string("@@GENERATOR@@")

)


process.p = cms.Path(process.NetvOnBins)

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
##signal
## https://cmsweb.cern.ch/dbs_discovery/aSearch?caseSensitive=on&userMode=user&sortOrder=desc&sortName=&grid=0&method=dbsapi&dbsInst=cms_dbs_prod_global&userInput=find+dataset+where+dataset+like+%2FPomWigDiJets*Pt*%2FSummer08_IDEAL_V9_v*%2FGEN-SIM-RECO+and+dataset+not+like+%2FPomWigDiJetsPlusPt30%2FSummer08_IDEAL_V9_v1%2FGEN-SIM-RECO*

njobs = '25'

##PomWigDiJetsMinusPt80
generator = 'pomwig'
dataset = '/PomWigDiJetsMinusPt80/fsilva-PomWigDiJetsMinusPt80_Skim_FastSim_v1-d9106ce61bbaa279a796fd9486526c79/USER'
#njobs = '2000'
uiworkingdir = 'PomWigDiJetsMinusPt80_NevtsOnBins_FAST_UIDIR'
##options: uerj,kfki,pic
##use '' for no se_white_list
sewhitelist = 'uerj'
ConfigHandler(dataset,njobs,uiworkingdir,sewhitelist,generator)

##PomWigDiJetsPlusPt80
generator = 'pomwig'
dataset = '/PomWigDiJetsPlusPt80/fsilva-PomWigDiJetsPlusPt80_Skim_FastSim_v1-d9106ce61bbaa279a796fd9486526c79/USER'
#njobs = '2000'
uiworkingdir = 'PomWigDiJetsPlusPt80_NevtsOnBins_FAST_UIDIR'
##options: uerj,kfki,pic
##use '' for no se_white_list
sewhitelist = 'uerj'
ConfigHandler(dataset,njobs,uiworkingdir,sewhitelist,generator)

##PomWigDiJetsMinusPt30
generator = 'pomwig'
dataset = '/PomWigDiJetsMinusPt30/fsilva-PomWigDiJetsMinusPt30_Skim_FastSim_v1-cc0dc234f49eeaa02840aa412879f3e7/USER'
#njobs = '2000'
uiworkingdir = 'PomWigDiJetsMinusPt30_NevtsOnBins_FAST_UIDIR'
##options: uerj,kfki,pic
##use '' for no se_white_list
sewhitelist = 'uerj'
ConfigHandler(dataset,njobs,uiworkingdir,sewhitelist,generator)

##PomWigDiJetsPlusPt30
generator = 'pomwig'
dataset = '/PomWigDiJetsPlusPt30/fsilva-PomWigDiJetsPlusPt30_Skim_FastSim_v1-cc0dc234f49eeaa02840aa412879f3e7/USER'
#njobs = '2000'
uiworkingdir = 'PomWigDiJetsPlusPt30_NevtsOnBins_FAST_UIDIR'
##options: uerj,kfki,pic
##use '' for no se_white_list
sewhitelist = 'uerj'
ConfigHandler(dataset,njobs,uiworkingdir,sewhitelist,generator)

##############################################################################################################################
##BACKGROUND
## https://cmsweb.cern.ch/dbs_discovery/aSearch?dbsInst=cms_dbs_prod_global&userMode=user&fromRow=0&_idx=0&pagerStep=0&userInput=find%20dataset%20where%20dataset%20=%20/QCDpt170/Summer08_IDEAL_V9_v3/GEN-SIM-RECO%20OR%20dataset%20=%20/QCDpt30/Summer08_IDEAL_V9_v4/GEN-SIM-RECO%20OR%20dataset%20=%20/QCDpt80/Summer08_IDEAL_V9_v2/GEN-SIM-RECO%20OR%20dataset%20=%20/QCDpt170/Summer08_IDEAL_V9_v3/GEN-SIM-RECO%20OR%20dataset%20=%20/QCDpt300/Summer08_IDEAL_V9_v1/GEN-SIM-RECO%20OR%20dataset%20=%20/QCDpt470/Summer08_IDEAL_V9_v1/GEN-SIM-RECO%20OR%20dataset%20=%20/QCDpt800/Summer08_IDEAL_V9_reco-v4/GEN-SIM-RECO%20OR%20dataset%20=%20/QCDpt1400/Summer08_IDEAL_V9_reco-v1/GEN-SIM-RECO%20OR%20dataset%20=%20/QCDpt2200/Summer08_IDEAL_V9_reco-v1/GEN-SIM-RECO%20OR%20dataset%20=%20/QCDpt3000/Summer08_IDEAL_V9_reco-v2/GEN-SIM-RECO%20OR%20dataset%20=%20/QCDpt15/Summer08_IDEAL_V9_v3/GEN-SIM-RECO%20&sortName=PATH&sortOrder=desc&caseSensitive=on&grid=0

njobs = '25'

##QCDpt15
generator = 'pythia'
dataset = '/QCDpt15/fsilva-QCDpt15_Skim_FastSim_v1-9ac412157f75e519d59071381c3a27b1/USER'
#njobs = '2000'
uiworkingdir = 'QCDpt15_NevtsOnBins_FAST_UIDIR'
##options: T2_IT_Pisa*,maite.iihe.ac.be*,T1_FR_CCIN2P3*,T2_FR_IPHC*,T2_FR_GRIF_LLR*
##use '' for no se_white_list
sewhitelist = 'uerj'
ConfigHandler(dataset,njobs,uiworkingdir,sewhitelist,generator)


##QCDpt30
generator = 'pythia'
dataset = '/QCDpt30/fsilva-QCDpt30_Skim_FastSim_v1-07acece1e9f1bb4977d2a9cfd0038a6f/USER'
#njobs = '2000'
uiworkingdir = 'QCDpt30_NevtsOnBins_FAST_UIDIR'
##options: ,T2_IT_Pisa,T2_US_Wisconsin,T1_FR_CCIN2P3,T2_FR_IPHC,T2_CN_Beijing
##use '' for no se_white_list
sewhitelist = 'uerj'
ConfigHandler(dataset,njobs,uiworkingdir,sewhitelist,generator)


##QCDpt80
generator = 'pythia'
dataset = '/QCDpt80/fsilva-QCDpt80_Skim_FastSim_v1-faf8d374708b3097a06fa0097fbf10bb/USER'
#njobs = '2000'
uiworkingdir = 'QCDpt80_NevtsOnBins_FAST_UIDIR'
##options: T2_IT_Pisa,T1_FR_CCIN2P3
##use '' for no se_white_list
sewhitelist = 'uerj'
ConfigHandler(dataset,njobs,uiworkingdir,sewhitelist,generator)


##QCDpt170
generator = 'pythia'
dataset = '/QCDpt170/fsilva-QCDpt170_Skim_FastSim_v1-f364d31cbfccb0fc1c0849728025ea46/USER'
#njobs = '2000'
uiworkingdir = 'QCDpt170_NevtsOnBins_FAST_UIDIR'
##options: T2_IT_Pisa,T1_FR_CCIN2P3
##use '' for no se_white_list
sewhitelist = 'uerj'
ConfigHandler(dataset,njobs,uiworkingdir,sewhitelist,generator)


##QCDpt300
generator = 'pythia'
dataset = '/QCDpt300/fsilva-QCDpt300_Skim_FastSim_v1-d667605768154e8cfb79664a0f9fa85a/USER'
#njobs = '2000'
uiworkingdir = 'QCDpt300_NevtsOnBins_FAST_UIDIR'
##options: T2_IT_Pisa,T1_FR_CCIN2P3
##use '' for no se_white_list
sewhitelist = 'uerj'
ConfigHandler(dataset,njobs,uiworkingdir,sewhitelist,generator)


##QCDpt470
generator = 'pythia'
dataset = '/QCDpt470/fsilva-QCDpt470_Skim_FastSim_v1-62464dd9477fb4b452d1dc190319e60c/USER'
#njobs = '2000'
uiworkingdir = 'QCDpt470_NevtsOnBins_FAST_UIDIR'
##options: T2_IT_Pisa,T1_UK_RAL
##use '' for no se_white_list
sewhitelist = 'uerj'
ConfigHandler(dataset,njobs,uiworkingdir,sewhitelist,generator)


##QCDpt800
generator = 'pythia'
dataset = '/QCDpt800/fsilva-QCDpt800_Skim_FastSim_v1-7c1785c9139c4f03a44c32dfa79078d1/USER'
#njobs = '2000'
uiworkingdir = 'QCDpt800_NevtsOnBins_FAST_UIDIR'
##options: T1_UK_RAL*,se2.ppgrid1.rhul.ac.uk*,T2_UK_SGrid_RALPP*,T2_EE_Estonia*
##use '' for no se_white_list
sewhitelist = 'uerj'
ConfigHandler(dataset,njobs,uiworkingdir,sewhitelist,generator)


##QCDpt1400
generator = 'pythia'
dataset = '/QCDpt1400/fsilva-QCDpt1400_Skim_FastSim_v1-922398a0a127c37eabc78b3267a2617f/USER'
#njobs = '2000'
uiworkingdir = 'QCDpt1400_NevtsOnBins_FAST_UIDIR'
##options: T1_UK_RAL*,T2_EE_Estonia*
##use '' for no se_white_list
sewhitelist = 'uerj'
ConfigHandler(dataset,njobs,uiworkingdir,sewhitelist,generator)


##QCDpt2200
generator = 'pythia'
dataset = '/QCDpt2200/fsilva-QCDpt2200_Skim_FastSim_v1-fe0f9347d7f31239499da34e43e23eca/USER'
#njobs = '2000'
uiworkingdir = 'QCDpt2200_NevtsOnBins_FAST_UIDIR'
##options: T2_EE_Estonia
##use '' for no se_white_list
sewhitelist = 'uerj'
ConfigHandler(dataset,njobs,uiworkingdir,sewhitelist,generator)

##############################################################################################################################
##############################################################################################################################
##############################################################################################################################

print ''
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'
print 'Finishing submission...'
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'



