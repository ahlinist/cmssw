import FWCore.ParameterSet.Config as cms

process = cms.Process("ZMMResAnalyzer")

# Printouts
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.destinations = ['cout']
process.MessageLogger.cout = cms.untracked.PSet(
      threshold = cms.untracked.string('INFO'),
      FwkReport = cms.untracked.PSet(reportEvery=cms.untracked.int32(100))
)
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.source = cms.Source("PoolSource", 
      fileNames = cms.untracked.vstring(
# Local Version (EdmNtuple = False)
#             "file:EdmZmmTreeMCShort.root"

# Official version (EdmNtuple = True)
             "file:EdmZmmTreeMC.root",
             "file:EdmZmmTreeData.root"
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
                                   fileName = cms.string("ResultsZMuMu_res_6pb.root"),
                                   closeFileFast = cms.untracked.bool(True)
)

process.fit = cms.EDAnalyzer("ZMuMuPtAnalyzer",
                             Initval=cms.untracked.double(0), # Initial and error values for minimization
                             Errorval = cms.untracked.double(1),  
                             # Luminosity (dummy, only for plot tagging)
                             Lumi = cms.untracked.double(6),
                             # If doscan = true => draw likelihood curve (no minimization); else minimize (no likelihood curve)
                             Doscan = cms.untracked.bool(False),
                             # doups (if you want to add another resonance to the fit, e. g. Upsilon)
                             DoUps = cms.untracked.bool(False),
                             # local use only
                             EdmNtuple = cms.untracked.bool(True), 
                             # dofit (0 => constant shift, 1 => constant res term, 2 => res term with pt, 3 => const shift with pt)
                             DoFit1 = cms.untracked.bool(False), # delta
                             DoFit2 = cms.untracked.bool(False), # sigma
                             DoFit3 = cms.untracked.bool(True),  # sigma(1/pt)
                             DoFit4 = cms.untracked.bool(True),  # delta(1/pt)
                             # Scan parameters (only meaningful if doscan = true)
                             NbinsScan = cms.untracked.int32(8),
                             IniScan = cms.untracked.double(-0.3),
                             EndScan = cms.untracked.double(1),
                             # if fs (full statistics) is true (default) => config.num_random**4 random values per muon; else 1 random per muon
                             Fs = cms.untracked.bool(False),
                             NumRandom = cms.untracked.int32(5),
                             # Only valid if doscan = true. Parameter of the likelihood curve
                             FitParameter = cms.untracked.int32(3),
                             # domigrad = 0 => Simplex only, domigrad = 1 => Simplex + Migrad
                             DoMigrad = cms.untracked.bool(True),
                             # local use only
                             RootFileName = cms.untracked.string("treeMC_histoData_6pb.root"),
                             # Histo values
                             Nbins_histomassZ = cms.untracked.int32(40),
                             Inibin_histomassZ = cms.untracked.int32(70),
                             Endbin_histomassZ = cms.untracked.int32(110),
                             Nbins_histomassU = cms.untracked.int32(10),
                             Inibin_histomassU = cms.untracked.int32(6),
                             Endbin_histomassU = cms.untracked.int32(8)
                             )                                     
# Path
process.path = cms.Path(
    process.fit
)

