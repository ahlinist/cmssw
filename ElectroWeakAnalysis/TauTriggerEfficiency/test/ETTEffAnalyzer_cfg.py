import FWCore.ParameterSet.Config as cms
import sys

"""
This is the config file for Tau trigger efficiency analysis using electrons faking taus.
Author: gfball

Some options are intended for future work to enable the reading of triggers with additional steps and will be irrelevant here.

This first section is used to read command line options from stdin, since cmsRun seems to strip the sys.argv parameter.
To use this you would do something like "echo electronTau,robust,219 | cmsRun ETTEffAnalyzer.py"

If you do not like this style of input, then comment out the following section...

"""

paths = ("DoubleIsoTau","IsoElecIsoTau","NewSingleTauMET","SingleTau","DoubleTau","SingleTauMET","ElectronTau","MuonTau")
eidquals = ("robust","loose","tight")
datas = (212,216,217,219)

input = sys.stdin.read()
path,eidqual,data = input.split(",")
data=int(data)
if not path in paths:
    print "Invalid path"
    sys.exit(-1)
if not eidqual in eidquals:
    print "Invalid eIDqual"
    sys.exit(-1)
if not data in datas:
    print "Invalid data"
    sys.exit(-1)

"""
...down to here and then manually declare variables:
path: the name of an tau trigger path. Note the actual names are irrelevant, this name will be used in the following dictionaries to determine the parameters for the ETTEffAnalyzer. Feel free to change the parameters if for instance you use a different trigger table. This was intended to be a conveience for multiple running by just having to change the stdin parameters.
eidqual: E/Gamma electron id quality. Typically one of (robust, tight, loose)
data: One of the relval datasets defined in the ZEEData dictionary. Alternatively if you're using an alternative data import, set this to anything useful you'd like appended to the output filename.

"""


print "Using path=%s and eIDqual=%s and data=%s" % (path,eidqual,data)

UseNewHLTDict={
    "DoubleIsoTau":True,
    "IsoElecIsoTau":True,
    "NewSingleTauMET":True,
    "SingleTau":False,
    "DoubleTau":False,
    "SingleTauMET":False,
    "ElectronTau":False,
    "MuonTau":False
    }

L2AssocDict={
    "DoubleIsoTau":cms.InputTag("hltL2TauIsolationProducer","L2TauIsolationInfoAssociator"),
    "IsoElecIsoTau":cms.InputTag("hltL2TauIsolationProducer","L2TauIsolationInfoAssociator"),
    "NewSingleTauMET":cms.InputTag("hltL2TauIsolationProducer","L2TauIsolationInfoAssociator"),
    "SingleTau":cms.InputTag("hltL2SingleTauIsolationProducer","L2TauIsolationInfoAssociator"),
    "DoubleTau":cms.InputTag("hltL2DoubleTauIsolationProducer","L2TauIsolationInfoAssociator"),
    "SingleTauMET":cms.InputTag("hltL2SingleTauMETIsolationProducer","L2TauIsolationInfoAssociator"),
    "ElectronTau":cms.InputTag("hltL2ElectronTauIsolationProducer","L2TauIsolationInfoAssociator"),
    "MuonTau":cms.InputTag("hltL2MuonTauIsolationProducer","L2TauIsolationInfoAssociator")
    }

L1FilterDict={
    "DoubleIsoTau":cms.InputTag("hltL1sDoubleTau40"),
    "IsoElecIsoTau":cms.InputTag("hltL1sIsoEG10Tau20"),
    "NewSingleTauMET":cms.InputTag("hltL1sTau30ETM30"),
    "SingleTau":cms.InputTag(""),
    "DoubleTau":cms.InputTag(""),
    "SingleTauMET":cms.InputTag(""),
    "ElectronTau":cms.InputTag(""),
    "MuonTau":cms.InputTag("")
    }

L2FilterDict={    
    "DoubleIsoTau":cms.InputTag("hltFilterL2EcalIsolationDoubleTau"),
    "IsoElecIsoTau":cms.InputTag("hltFilterL2EcalIsolationElectronTau"),
    "NewSingleTauMET":cms.InputTag("hltFilterL2EcalIsolationSingleTauMET"),
    "SingleTau":cms.InputTag(""),
    "DoubleTau":cms.InputTag(""),
    "SingleTauMET":cms.InputTag(""),
    "ElectronTau":cms.InputTag(""),
    "MuonTau":cms.InputTag("")
    }

L25FilterDict={
    "DoubleIsoTau":(cms.InputTag("hltFilterL25PixelTracksLeadingTrackPtCutDoubleTau"),cms.InputTag("hltFilterL25PixelTracksIsolationDoubleTau")),
    "IsoElecIsoTau":(cms.InputTag("hltFilterL25PixelTracksLeadingTrackPtCutElectronTau"),cms.InputTag("hltFilterL25PixelTracksIsolationElectronTau")),
    "NewSingleTauMET":(cms.InputTag("hltFilterL25PixelTracksLeadingTrackPtCutSingleTauMET"),cms.InputTag("hltFilterL25PixelTracksIsolationSingleTauMET")),
    "SingleTau":(cms.InputTag(""),cms.InputTag("")),
    "DoubleTau":(cms.InputTag(""),cms.InputTag("")),
    "SingleTauMET":(cms.InputTag(""),cms.InputTag("")),
    "ElectronTau":(cms.InputTag(""),cms.InputTag("")),
    "MuonTau":(cms.InputTag(""),cms.InputTag(""))
    }


"""
End the dictionary definitions, start the process definition

"""


process = cms.Process("TEST")

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Geometry.CaloEventSetup.CaloTopology_cfi")
process.load("FWCore/MessageService/MessageLogger_cfi")

"""
Automatically load a list of files from ZEEData based on the data string.
If you don't want to use ZEE RelVal data, then you'll need to comment out this section and add a command instead to import your own data source...
"""
from ElectroWeakAnalysis.TauTriggerEfficiency.ZEEData_cfi import DataDict
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend(DataDict[data])
process.source = source
"""
...end of data load section.
"""



process.load("ElectroWeakAnalysis.TauTriggerEfficiency.ElectronProducer_cff")

process.eidCutBasedExt.electronQuality = eidqual

"""
Define the main analysis process.
Note that a number of parameters not found in the TTEffAnalyzer are set here. These are to support triggers with extra filters, when I get round to moving that functionality into the main tree. For the moment they don't matter.

Because of the CMS insistence on using CMS primitives not python primitives the dictionaries have to contain these.
"""

process.ETTEffAnalysis = cms.EDAnalyzer("TTEffAnalyzer",
        PFTauCollection         = cms.InputTag("ElectronProducer","Electrons"),
	L1extraTauJetSource	= cms.InputTag("hltL1extraParticles:Tau"),
        L1extraCentralJetSource = cms.InputTag("hltL1extraParticles:Central"),
        L1bitInfoSource         = cms.InputTag("l1CaloSim", "L1BitInfos"),
	L1JetMatchingCone	= cms.double(0.5),
        L2AssociationCollection = L2AssocDict[path],
        L2matchingDeltaR        = cms.double(0.3),
        outputFileName          = cms.string("test_%s_%s_%d.root" %(path,eidqual,data)),
        doFilter                = cms.bool(UseNewHLTDict[path]),
        L1filter                = L1FilterDict[path],
        L2filter                = L2FilterDict[path],
        l25JetSource            = cms.InputTag("hltL25TauPixelTracksConeIsolation"),
        l25PtCutSource          = cms.InputTag("hltL25TauPixelTracksLeadingTrackPtCutSelector"),
        l25IsoSource            = cms.InputTag("hltL25TauPixelTracksIsolationSelector"),
        l25MatchingCone         = cms.double(0.3),
        l25filter0              = L25FilterDict[path][0],
        l25filter1              = L25FilterDict[path][1]
                                        
)
process.load("ElectroWeakAnalysis.TauTriggerEfficiency.HLTFilter_cff")

"""
This list of HLTs should be suitable for electron analysis - weakest possible electron triggers.
"""
process.IncludedHLTs.HLTSelection = cms.VInputTag("HLT_IsoEle15_L1",
                                                  "HLT_LooseIsoEle15_LW_L1R",
                                                  "HLT_Ele10_SW_L1R",
                                                  "HLT_DoubleIsoEle10_L1I"
                                                  )

"""
If we're using one of the new trigger types, we need to re-run the HLT. This section conditionally loads the necessary includes for this. Re-runs both the base HLT 2E30 trigger table and then an additional tau one.
"""

if (UseNewHLTDict[path]):
    print "Using new HLT"
    process.load("Configuration.StandardSequences.GeometryPilot2_cff")
    process.load("Configuration.StandardSequences.MagneticField_cff")
    process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
    process.GlobalTag.globaltag = 'STARTUP_V4::All'
    process.load("Configuration.StandardSequences.L1Emulator_cff")
    process.load("Configuration.StandardSequences.L1TriggerDefaultMenu_cff")
    process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
    process.load("HLTrigger.Configuration.HLT_2E30_cff")
    #process.load("DQMOffline.Trigger.Tau.HLTTau_V_Fixed_cff")
    process.load("ElectroWeakAnalysis.TauTriggerEfficiency.HLT_Tau_V1_cff")


"""
Comment in whichever process you want to run.

"""

process.p = cms.Path(process.ElectronProductionSequence+process.IncludedHLTs+process.ETTEffAnalysis)

#process.p = cms.Path(process.ElectronProductionSequence+process.ETTEffAnalysis)

#process.p = cms.Path(process.ElectronProductionSequence+process.IncludedHLTs)

#process.p = cms.Path(process.ElectronProductionSequence)

#process.p = cms.Path(process.IncludedHLTs)
