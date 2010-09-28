import FWCore.ParameterSet.Config as cms

process = cms.Process("WMNResAnalyzer")

# Printouts
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.destinations = ['cout']
process.MessageLogger.cout = cms.untracked.PSet(
      threshold = cms.untracked.string('INFO'),
      FwkReport = cms.untracked.PSet(reportEvery=cms.untracked.int32(10))
)
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.source = cms.Source("PoolSource", 
      fileNames = cms.untracked.vstring(
#            "rfio:///castor/cern.ch/user/d/degrutto/2010/ZCands/RECO/EWK_HighPtDiMuonSkim_SD_Mu_140_116_126_23ZCands.root"
 #            "file:EdmWmnTreeMCMINUS.root",
             "file:EdmWmnTreeMCMINUSShort.root",
             "file:EdmWmnTreeDataShort.root"
      ),
#      inputCommands = cms.untracked.vstring(
#            'keep *',
#            'drop *_MEtoEDMConverter_*_*',
#            'drop *_lumiProducer_*_*'
#      )
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


# Output histograms
process.TFileService = cms.Service("TFileService", 
                                   fileName = cms.string("ResultsWMuNu_testNTuple.root"),
                                   closeFileFast = cms.untracked.bool(True)
)

process.fit = cms.EDAnalyzer("WMuNuPtAnalyzer",
                             Initval=cms.untracked.double(0), # Initial and error values for minimization
                             Errorval = cms.untracked.double(0.05),  
                             # Luminosity (dummy, only for plot tagging)
                             Lumi = cms.untracked.double(10),
                             # If doscan = true => draw likelihood curve (no minimization); else minimize (no likelihood curve)
                             Doscan = cms.untracked.bool(False),
                             # local use only
                             EdmNtuple = cms.untracked.bool(True), 
                             # dofit (0 => constant shift, 1 => constant res term, 2 => res term with pt, 3 => const shift with pt)
                             DoFit1 = cms.untracked.bool(False), # delta
                             DoFit2 = cms.untracked.bool(False), # sigma
                             DoFit3 = cms.untracked.bool(True),  # sigma(1/pt)
                             DoFit4 = cms.untracked.bool(True),  # delta(1/pt)
                             # Scan parameters (only meaningful if doscan = true)
                             NbinsScan = cms.untracked.int32(7),
                             IniScan = cms.untracked.double(-0.2),
                             EndScan = cms.untracked.double(0),
                             # if fs (full statistics) is true (default) => config.num_random**4 random values per muon; else 1 random per muon
                             Fs = cms.untracked.bool(True),
                             NumRandom = cms.untracked.int32(5),
                             # Only valid if doscan = true. Parameter of the likelihood curve
                             FitParameter = cms.untracked.int32(3),
                             # domigrad = 0 => Simplex only, domigrad = 1 => Simplex + Migrad
                             DoMigrad = cms.untracked.bool(True),
                             # local use only
                             RootFileName = cms.untracked.string("treeMC_histoData_WMINUS.root"),
                             # Histo values
                             Nbins_histo = cms.untracked.int32(40),
                             Inibin_histo = cms.untracked.int32(20),
                             Endbin_histo = cms.untracked.int32(90),
                             # mode
                             Mode = cms.untracked.bool(False),
                             # charge
                             Charge = cms.untracked.int32(-1),
                             # eta bin 
                             EtaBin = cms.untracked.int32(0)
                             )                                     
# Path
process.path = cms.Path(
    process.fit
)

