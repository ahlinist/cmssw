import sys, os, FWCore.ParameterSet.Config as cms

process = cms.Process('SKIM')
process.source = cms.Source('PoolSource', fileNames = cms.untracked.vstring('/store/mc/Summer11/TKCosmics_p100_PEAK/GEN-SIM-RAW/COSMC_42_PEAK-v1/0023/F2A549E7-C105-E111-BB76-001A9227D20D.root'))
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(50))
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.GeometryIdeal_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'COSMC_42_PEAK::All'
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.ReconstructionCosmics_cff')
process.load('FWCore.MessageLogger.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 5000

process.raw2reco = cms.Sequence(process.RawToDigi * process.reconstructionCosmics)

pointing = cms.EDFilter('HLTMuonPointingFilter',
                        SALabel = cms.string(''),
                        PropagatorName = cms.string('SteppingHelixPropagatorAny'),
                        radius = cms.double(10.0),
                        maxZ = cms.double(50.0)
                        )

tracks = [
    'cosmictrackfinderP5',
    'rsWithMaterialTracksP5',
    'ctfWithMaterialTracksP5',
    'cosmicMuonsBarrelOnly',
    'cosmicMuonsEndCapsOnly',
    'cosmicMuons',
    'cosmicMuons1Leg',
    'globalCosmicMuonsBarrelOnly',
    'globalCosmicMuonsEndCapsOnly',
    'globalCosmicMuons',
    'globalCosmicMuons1Leg',
    ]

paths = [tk + 'Path' for tk in tracks]

for tk in tracks:
    filter = pointing.clone(SALabel = cms.string(tk))
    path = cms.Path(process.raw2reco * filter)
    setattr(process, tk + 'Filter', filter)
    setattr(process, tk + 'Path', path)

process.out = cms.OutputModule('PoolOutputModule',
                               outputCommands = cms.untracked.vstring('keep *', 'drop *_MEtoEDMConverter_*_*', 'drop *_*_*_SKIM'),
                               SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring(*paths)),
                               fileName = cms.untracked.string('pointing.root')
                               )
process.endp = cms.EndPath(process.out)

if __name__ == '__main__' and 'submit' in sys.argv:
    crab_cfg = '''
[CRAB]
jobtype = cmssw
scheduler = condor

[CMSSW]
datasetpath = %(dataset)s
pset = spskim.py
total_number_of_events = -1
events_per_job = 10000
get_edm_output = 1

[USER]
ui_working_dir = crab/crab_spskim_%(name)s
copy_data = 1
storage_element = T3_US_FNALLPC
check_user_remote_dir = 0
publish_data = 1
publish_data_name = spskim_%(name)s
dbs_url_for_publication = https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_02_writer/servlet/DBSServlet
'''

    just_testing = 'testing' in sys.argv

    datasets = [
        ('P10Peak',  '/TKCosmics_p10_PEAK/Summer11-COSMC_42_PEAK-v1/GEN-SIM-RAW'),
        ('P100Peak', '/TKCosmics_p100_PEAK/Summer11-COSMC_42_PEAK-v1/GEN-SIM-RAW'),
        ('P500Peak', '/TKCosmics_p100_PEAK/Summer11-COSMC_42_PEAK-v1/GEN-SIM-RAW'),
        ]

    for name, dataset in datasets:
        open('crab.cfg', 'wt').write(crab_cfg % locals())
        if not just_testing:
            os.system('crab -create -submit all')
            os.system('rm crab.cfg')
