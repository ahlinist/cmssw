import FWCore.ParameterSet.Config as cms
import sys
import glob
import copy

"""
This is the config file for Tau trigger efficiency analysis using electrons faking taus.
Author: gfball

This originally used sys.stdin to read options from the command line, using the style "echo opt1,opt2... | cmsRun ETTEff...py".
However, I realise this doesn't exactly meet any criteria of user friendliness and just confuses people, so the variables I did read from stdin are now written just below this point.
From CMSSW 3 (or 2.2x with some tags) there is a VarParsing package (see bottom of twiki:SWGuideAboutPythonConfigFile) which will allow a more sensible command line input. (Note, you cannot use sys.argv because the config file is exec'd from within cmsRun and so sys.argv is empty).

By default, files are read using a slightly messy parser that allows either datasets, wildcard strings of local files or DBS python output to be read. If you want to run this remotely (grid or batch) you may wish to replace this with just a static list of files - in which case uncomment the readfiles variable at the top. Otherwise, the following syntax works:

data = "/full/dbs/dataset/name/GEN-SIM-RAW/RECO" - this dataset will be looked up in DBS and the full list of files used.
data = "glob:/tmp/data/*.root" - all files named '*.root' in /tmp/data will be used.
data = "file:my_file_list_cfi.py" - file containing a source and readfile list (as DBS produces) is executed and the contents used

"""

#Important variables

#data to use OR list of files
#you either need to change this, or add something to the readfiles list
#if there are filenames in readfile they will be used instead of anything in data
data = "glob:/localscratch/2110ZEE/*.root"
readFiles = cms.untracked.vstring()

#Use tighter electron requirements - minpt>0.5, isolation=0 in cone 0.7
highpurity = False

#Use full tracks instead of pixel tracks for L25 (only works with dohlt=True)
nopixel = False

#Electron ID quality to use. Should be one of (loose,robust,tight)
eidqual = 'robust'

#Prefix for output filename
prefix = 'etteff'

#Use L1Calosim
docalosim = True

#Use HLTTauV1 table
dohlt = True



filename = "%s_%s" % (prefix,eidqual)
if highpurity:
  filename+="_highpurity"
if nopixel:
  filename+="_nopixel"
filename += ".root"


#Start defining the process
process = cms.Process("TEST")

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Geometry.CaloEventSetup.CaloTopology_cfi")
process.load("FWCore/MessageService/MessageLogger_cfi")

"""
File load section.
As above, if the "readFiles" list already contains a list of files we'll use that.
Alternatively, we will either:
  execute a python file list file (syntax 'file:my_filelist_cfi.py')
  expand a local list of root files (syntax 'glob:/path/*.root')
  look up files from DBS (syntax '/full/dataset/name')
"""
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))
from ElectroWeakAnalysis.TauTriggerEfficiency.DBSFetch import DBSFetch
if not len(readFiles):
  if data.startswith('file:'):
    data = data.split(':')[1]
    x={}
    print "Reading python filelist:",data
    execfile(data,globals(),x)
    readFiles.extend(x['readFiles'])
  elif data.startswith('glob:'):
    data = data.split(':')[1]
    print "Expanding file glob:",data
    files = glob.glob(data)
    files = ["file:%s"%(f) for f in files]
    readFiles.extend(files)
  else:
    print "Performing DBS lookup:",data
    readFiles.extend(DBSFetch.getFiles(data))
else:
  print "Using supplied filelist"
process.source = cms.Source ("PoolSource",fileNames = readFiles)


#Load the electron producer
process.load("ElectroWeakAnalysis.TauTriggerEfficiency.ElectronProducer_cff")
process.eidCutBasedExt.electronQuality = eidqual


#Define the main analysis process.

process.ETTEffAnalysis = cms.EDAnalyzer("TTEffAnalyzer",
        PFTauCollection         = cms.InputTag("ElectronProducer"),
        PFTauIsoCollection      = cms.InputTag("None"),
        L1extraTauJetSource     = cms.InputTag("hltL1extraParticles:Tau"),
        L1extraCentralJetSource = cms.InputTag("hltL1extraParticles:Central"),
        L1bitInfoSource         = cms.InputTag("l1CaloSim", "L1BitInfos"),
        L1GtReadoutRecord       = cms.InputTag("hltGtDigis"),
        L1GtObjectMapRecord     = cms.InputTag("hltL1GtObjectMap"),
        L1TauTriggerSource      = cms.InputTag("tteffL1GTSeed"),
        HltResults              = cms.InputTag("TriggerResults::HLT"),
        L1JetMatchingCone       = cms.double(0.5),
        L2AssociationCollection = cms.InputTag("hltL2TauIsolationProducer","L2TauIsolationInfoAssociator"),
        L2matchingDeltaR        = cms.double(0.3),
        outputFileName          = cms.string(filename),
        l25JetSource            = cms.InputTag("hltL25TauPixelTracksConeIsolation"),
        l25PtCutSource          = cms.InputTag("hltL25TauPixelTracksLeadingTrackPtCutSelector"),
        l25IsoSource            = cms.InputTag("hltL25TauPixelTracksIsolationSelector"),
        l25MatchingCone         = cms.double(0.3),
        l1JetFilters            = cms.VInputTag(),
        caloTauFilters          = cms.VInputTag()
                                        
)

"""
If we're using one of the new trigger types, we need to re-run the HLT. This section conditionally loads the necessary includes for this. Re-runs both the base HLT 2E30 trigger table and then an additional tau one.
"""

if docalosim:
    print "Using L1calosim"
    process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")
    process.load("Geometry.CaloEventSetup.CaloGeometry_cff")
    process.load("Geometry.CaloEventSetup.CaloTopology_cfi")
    process.load("Geometry.CaloEventSetup.CaloTowerConstituents_cfi")
    process.load("RecoTauTag.L1CaloSim.l1calosim_cfi")
    process.l1CaloSim.AlgorithmSource = "RecHits" 
    process.l1CaloSim.EmInputs = cms.VInputTag(cms.InputTag("ecalRecHit","EcalRecHitsEB"), cms.InputTag("ecalRecHit","EcalRecHitsEE"))
    process.l1CaloSim.DoBitInfo = cms.bool(True)
    process.o = cms.Path(process.l1CaloSim)  

if dohlt:
    print "Using new HLT"
    process.load("Configuration.StandardSequences.GeometryPilot2_cff")
    process.load("Configuration.StandardSequences.MagneticField_cff")
    process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
    process.GlobalTag.globaltag = 'IDEAL_V9::All'
    process.load("Configuration.StandardSequences.L1Emulator_cff")
    process.load("Configuration.StandardSequences.L1TriggerDefaultMenu_cff")
    process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
    process.load("HLTrigger.Configuration.HLT_2E30_cff")
    #process.load("DQMOffline.Trigger.Tau.HLTTau_V_Fixed_cff")
    process.load("ElectroWeakAnalysis.TauTriggerEfficiency.HLT_Tau_V1_cff")
    
    process.load("L1Trigger/Configuration/L1Config_cff")
    process.load("Configuration/StandardSequences/L1TriggerDefaultMenu_cff")
    #process.load("L1TriggerConfig/L1GtConfigProducers/Luminosity/lumi1030/L1Menu_2008MC_2E30_Unprescaled_cff")
    process.load("HLTrigger/HLTfilters/hltLevel1GTSeed_cfi")
    process.tteffL1GTSeed = copy.deepcopy(process.hltLevel1GTSeed)
    process.tteffL1GTSeed.L1TechTriggerSeeding = cms.bool(False)
    process.tteffL1GTSeed.L1SeedsLogicalExpression = cms.string("L1_SingleTauJet30")
    #process.tteffL1GTSeed.L1SeedsLogicalExpression = cms.string("L1_SingleTauJet80")
    #process.tteffL1GTSeed.L1SeedsLogicalExpression = cms.string("L1_SingleTauJet60 OR L1_SingleJet100")
    process.tteffL1GTSeed.L1GtReadoutRecordTag = cms.InputTag("hltGtDigis")
    process.tteffL1GTSeed.L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap")
    process.tteffL1GTSeed.L1CollectionsTag = cms.InputTag("hltL1extraParticles")
    process.tteffL1GTSeed.L1MuonCollectionTag = cms.InputTag("hltL1extraParticles")

if nopixel:
    print "Using full tracks"
    process.hltL25TauJetPixelTracksAssociator.tracks=cms.InputTag("generalTracks")
    process.hltL25TauPixelTracksConeIsolation.vertexSrc=cms.InputTag("offlinePrimaryVertices")

if highpurity:
    print "Using High Purity Mode"
    process.ElectronProducer.ptMinTrack=cms.double(0.5)
    process.ElectronProducer.MaxIsoVar=cms.double(0.0)
    process.ElectronProducer.OuterConeDR=cms.double(0.7)

process.p = cms.Path(process.tteffL1GTSeed*process.ElectronProductionSequence*process.ETTEffAnalysis)
if docalosim:
    process.p = cms.Path(process.ElectronProductionSequence*process.l1CaloSim*process.ETTEffAnalysis)

