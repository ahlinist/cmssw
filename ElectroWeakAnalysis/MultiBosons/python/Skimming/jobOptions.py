import copy
import sys
from ElectroWeakAnalysis.MultiBosons.Skimming.options import options as defaultOptions

def applyJobOptions(options):
  """
  Set multiple options defined by the jobType argument.
  To be called after options.parseArguments()
  """
  options.parseArguments()

  ## Attach the crabOptions to the command line arguments
  if options.crabOptions: sys.argv += options.crabOptions.split(",")

  jobOptions = copy.deepcopy(defaultOptions)

  if options.jobType == "testMC":
    jobOptions.outEvents = 100
    jobOptions.inputFiles = [
      "/store/relval/CMSSW_3_5_7/" +
      "RelValZMM/GEN-SIM-RECO/START3X_V26-v1/0012/" +
      file for file in """
        10B71379-4549-DF11-9D80-003048D15D22.root
        34FD3B1D-6949-DF11-9529-0018F3D09612.root
        4C8D7358-4449-DF11-86BB-003048678A6C.root
      """.split()
    ]
    jobOptions.globalTag = "START38_V10::All"
    jobOptions.reportEvery = 1
    jobOptions.isRealData = False
    jobOptions.use35XInput = True
    jobOptions.wantSummary = False
    jobOptions.skimType = "MuonPhoton"

  elif options.jobType == "testPOWHEG":
    jobOptions.maxEvents = -1
    jobOptions.inputFiles = [
      "file:/scratch/lgray/skimming/CMSSW_3_8_4_patch3/"+
      "src/ElectroWeakAnalysis/MultiBosons/" +
      file for file in """
        3A6643C1-D17F-DF11-8B44-001A92971B0C.root
        36FD5DFF-D67F-DF11-9C85-0026189438A9.root
      """.split()
    ]
    jobOptions.globalTag = "START38_V10::All"
    jobOptions.reportEvery = 1
    jobOptions.isRealData = False
    jobOptions.use35XInput = False
    jobOptions.wantSummary = False
    jobOptions.hltProcessName = "REDIGI36X"
    jobOptions.skimType = "MuonPhoton"

  # end of testMC options <-------------------------------------

  elif options.jobType == "testRealData":
    jobOptions.maxEvents = -1
    jobOptions.inputFiles = [
      "/store/data/Run2010A/Mu/RECO/v4/000/" +
      file for file in """
        142/933/C043478A-79A7-DF11-BB4C-001D09F2426D.root
        142/933/C277D905-95A7-DF11-A70D-003048F024F6.root
        142/933/CC18C413-9CA7-DF11-91B8-003048F118AA.root
        142/933/D433F278-9FA7-DF11-8403-003048F11114.root
        142/933/DE260EAC-A3A7-DF11-B4A8-0030487CD7B4.root
        143/657/A8367E04-73AE-DF11-9989-001D09F297EF.root
        143/657/B81CBEDC-C3AE-DF11-8909-003048F118D2.root
        143/657/BE540563-9AAE-DF11-BF6D-003048D3750A.root
        143/657/BEF1F8EB-DFAE-DF11-AF55-0030487A1FEC.root
        143/657/C0905609-92AE-DF11-809B-0030487CD77E.root
        143/657/D2E2D0AB-8EAE-DF11-A325-0030487D0D3A.root
        143/657/D689519B-D2AE-DF11-8A48-003048F024C2.root
        143/657/D8EB995B-98AE-DF11-B1CB-0030487A3232.root
        143/657/DC26FBFB-A2AE-DF11-8459-0030487C2B86.root
        143/657/DE4FFA93-95AE-DF11-AFAA-001D09F24259.root
        143/657/E07818F5-A9AE-DF11-B252-0030487CD6F2.root
      """.split()
    ]
    jobOptions.globalTag = "GR10_P_V9::All"
    jobOptions.reportEvery = 1
    jobOptions.isRealData = True
    jobOptions.use35XInput = False
    jobOptions.wantSummary = False
    jobOptions.skimType = "MuonPhoton"
    
  # end of testRealData options <-----------------------------------

  elif options.jobType == "testJetRealData":
    jobOptions.maxEvents = -1
    jobOptions.inputFiles = [
      "/store/data/Run2010B/Jet/RECO/PromptReco-v2/000/" +
      file for file in """
        147/757/1428A995-A4D6-DF11-8499-0030487C2B86.root
        147/757/360DEEC4-A1D6-DF11-9680-003048F11CF0.root
        147/757/748A647E-A2D6-DF11-AF11-001617C3B6CE.root
        147/757/B8DDD485-A2D6-DF11-9276-001617DBCF6A.root
        147/757/D4F5A10B-ADD6-DF11-A99F-0030487C7828.root
        147/755/0E36619B-98D6-DF11-98B4-001D09F2A465.root
        147/755/3E51D9A2-8CD6-DF11-9A05-000423D9997E.root
        147/755/5C885813-98D6-DF11-AA8A-001617E30CC8.root
        147/755/6063353F-97D6-DF11-A69C-001D09F24664.root
        147/755/B6D62927-9CD6-DF11-966D-003048F0258C.root
        147/755/DC73547A-8DD6-DF11-849E-0030487C8CB8.root
        147/755/E865F520-98D6-DF11-8BAC-001617C3B6CE.root
      """.split()
    ]
    jobOptions.globalTag = "GR10_P_V10::All"
    jobOptions.reportEvery = 1
    jobOptions.isRealData = True
    jobOptions.use35XInput = False
    jobOptions.wantSummary = False
    jobOptions.skimType = "Jet"
    jobOptions.hltProcessName = "HLT"
  # end of testRealData options <-----------------------------------

  elif options.jobType == "testSummer10":
    jobOptions.inputFiles = [
        "rfio:/castor/cern.ch/user/z/zkliu/TestRECO/36x_ZJet_Madgraph_tauola_TestRECO.root"
        ]
    jobOptions.skimType = "Electron"
    jobOptions.globalTag = "START38_V10::All"
    jobOptions.reportEvery = 1
    jobOptions.isRealData = False
    jobOptions.use35XInput = False
    jobOptions.wantSummary = True
    jobOptions.hltProcessName = "REDIGI36X"

  elif options.jobType == "MUPHPromptReco36X":
    jobOptions.globalTag = "GR10_P_V7::All"
    jobOptions.isRealData = True
    jobOptions.use35XInput = False
    jobOptions.maxEvents = -1
    jobOptions.skimType = "Muon"
    jobOptions.wantSummary = True

  elif options.jobType == "EPHPromptReco36X":
    jobOptions.globalTag = "GR10_P_V7::All"
    jobOptions.isRealData = True
    jobOptions.use35XInput = False
    jobOptions.maxEvents = -1
    jobOptions.skimType = "Electron"
    jobOptions.wantSummary = True

  elif options.jobType == "JETPromptReco36X":
    jobOptions.globalTag = "GR10_P_V7::All"
    jobOptions.isRealData = True
    jobOptions.use35XInput = False
    jobOptions.maxEvents = -1
    jobOptions.skimType = "Jet"
    jobOptions.wantSummary = True

  elif options.jobType == "PHMETPromptReco36X":
    jobOptions.globalTag = "GR10_P_V7::All"
    jobOptions.isRealData = True
    jobOptions.use35XInput = False
    jobOptions.maxEvents = -1
    jobOptions.skimType = "PhotonMET"
    jobOptions.wantSummary = True

  elif options.jobType == "MUPHPromptReco38X":
    jobOptions.globalTag = "GR10_P_V9::All"
    jobOptions.isRealData = True
    jobOptions.use35XInput = False
    jobOptions.maxEvents = -1
    jobOptions.skimType = "Muon"
    jobOptions.wantSummary = True

  elif options.jobType == "EPHPromptReco38X":
    jobOptions.globalTag = "GR10_P_V9::All"
    jobOptions.isRealData = True
    jobOptions.use35XInput = False
    jobOptions.maxEvents = -1
    jobOptions.skimType = "Electron"
    jobOptions.wantSummary = True

  elif options.jobType == "JETPromptReco38X":
    jobOptions.globalTag = "GR10_P_V9::All"
    jobOptions.isRealData = True
    jobOptions.use35XInput = False
    jobOptions.maxEvents = -1
    jobOptions.skimType = "Jet"
    jobOptions.wantSummary = True

  elif options.jobType == "PHMETPromptReco38X":
    jobOptions.globalTag = "GR10_P_V9::All"
    jobOptions.isRealData = True
    jobOptions.use35XInput = False
    jobOptions.maxEvents = -1
    jobOptions.skimType = "PhotonMET"
    jobOptions.wantSummary = True

  elif options.jobType == "MUPHReReco36X":
    jobOptions.globalTag = "GR_R_36X_V12B::All"
    jobOptions.isRealData = True
    jobOptions.use35XInput = False
    jobOptions.maxEvents = -1
    jobOptions.skimType = "Muon"
    jobOptions.wantSummary = True

  elif options.jobType == "EPHReReco36X":
    jobOptions.globalTag = "GR_R_36X_V12B::All"
    jobOptions.isRealData = True
    jobOptions.use35XInput = False
    jobOptions.maxEvents = -1
    jobOptions.skimType = "Electron"
    jobOptions.wantSummary = True

  elif options.jobType == "JETReReco36X":
    jobOptions.globalTag = "GR_R_36X_V12B::All"
    jobOptions.isRealData = True
    jobOptions.use35XInput = False
    jobOptions.maxEvents = -1
    jobOptions.skimType = "Jet"
    jobOptions.wantSummary = True

  elif options.jobType == "PHMETReReco36X":
    jobOptions.globalTag = "GR_R_36X_V12B::All"
    jobOptions.isRealData = True
    jobOptions.use35XInput = False
    jobOptions.maxEvents = -1
    jobOptions.skimType = "PhotonMET"
    jobOptions.wantSummary = True

  elif options.jobType == "MUPHReReco38X":
    jobOptions.globalTag = "GR_R_38X_V13A::All"
    jobOptions.isRealData = True
    jobOptions.use35XInput = False
    jobOptions.maxEvents = -1
    jobOptions.skimType = "Muon"
    jobOptions.hltPaths = ["HLT_Mu9","HLT_Mu11"]
    jobOptions.wantSummary = True

  elif options.jobType == "EPHReReco38X":
    jobOptions.globalTag = "GR_R_38X_V13A::All"
    jobOptions.isRealData = True
    jobOptions.use35XInput = False
    jobOptions.maxEvents = -1
    jobOptions.skimType = "Electron"
    jobOptions.hltPaths = ["HLT_Ele15_LW_L1R", "HLT_Ele15_SW_L1R"]
    jobOptions.wantSummary = True

  elif options.jobType == "JETReReco38X":
    jobOptions.globalTag = "GR_R_38X_V13A::All"
    jobOptions.isRealData = True
    jobOptions.use35XInput = False
    jobOptions.maxEvents = -1
    jobOptions.skimType = "Jet"
    jobOptions.wantSummary = True

  elif options.jobType == "PHMETReReco38X":
    jobOptions.globalTag = "GR_R_38X_V13A::All"
    jobOptions.isRealData = True
    jobOptions.use35XInput = False
    jobOptions.maxEvents = -1
    jobOptions.skimType = "PhotonMET"
    jobOptions.wantSummary = True

  elif options.jobType == "MUPHMC36XSpring10":
    jobOptions.globalTag = "START36_V10::All"
    jobOptions.isRealData = False
    jobOptions.use35XInput = True
    jobOptions.maxEvents = -1
    jobOptions.skimType = "Muon"
    jobOptions.hltPaths = ["HLT_Mu9","HLT_Mu11"]
    jobOptions.wantSummary = True
    jobOptions.hltProcessName = "REDIGI"

  elif options.jobType == "MUPHMC38XSpring10":
    jobOptions.globalTag = "START38_V12::All"
    jobOptions.isRealData = False
    jobOptions.use35XInput = True
    jobOptions.maxEvents = -1
    jobOptions.skimType = "Muon"
    jobOptions.hltPaths = ["HLT_Mu9","HLT_Mu11"]
    jobOptions.wantSummary = True
    jobOptions.hltProcessName = "REDIGI"

  elif options.jobType == "MUPHMC36XSummer10":
    jobOptions.globalTag = "START36_V10::All"
    jobOptions.isRealData = False
    jobOptions.use35XInput = False
    jobOptions.maxEvents = -1
    jobOptions.skimType = "Muon"
    jobOptions.hltPaths = ["HLT_Mu9","HLT_Mu11"]
    jobOptions.wantSummary = True
    jobOptions.hltProcessName = "REDIGI36X"

  elif options.jobType == "MUPHMC38XSummer10":
    jobOptions.globalTag = "START38_V12::All"
    jobOptions.isRealData = False
    jobOptions.use35XInput = False
    jobOptions.maxEvents = -1
    jobOptions.skimType = "Muon"
    jobOptions.hltPaths = ["HLT_Mu9","HLT_Mu11"]
    jobOptions.wantSummary = True
    jobOptions.hltProcessName = "REDIGI36X"

  elif options.jobType == "EPHMC38XSpring10":
    jobOptions.globalTag = "START38_V12::All"
    jobOptions.isRealData = False
    jobOptions.use35XInput = True
    jobOptions.maxEvents = -1
    jobOptions.skimType = "Electron"
    jobOptions.hltPaths = ["HLT_Ele15_LW_L1R", "HLT_Ele15_SW_L1R"]
    jobOptions.wantSummary = True
    jobOptions.hltProcessName = "REDIGI"

  elif options.jobType == "EPHMC38XSummer10":
    jobOptions.globalTag = "START38_V12::All"
    jobOptions.isRealData = False
    jobOptions.use35XInput = True
    jobOptions.maxEvents = -1
    jobOptions.skimType = "Electron"
    jobOptions.hltPaths = ["HLT_Ele15_LW_L1R", "HLT_Ele15_SW_L1R"]
    jobOptions.wantSummary = True
    jobOptions.hltProcessName = "REDIGI36X"

  elif options.jobType == "JETMC38X":
    jobOptions.globalTag = "START38_V12::All"
    jobOptions.isRealData = False
    jobOptions.use35XInput = True
    jobOptions.maxEvents = -1
    jobOptions.skimType = "Muon"
    jobOptions.hltPaths = ["HLT_Mu9","HLT_Mu11"]
    jobOptions.wantSummary = True
    jobOptions.hltProcessName = "REDIGI"

  elif options.jobType == "PHMETMC38X":
    jobOptions.globalTag = "START38_V12::All"
    jobOptions.isRealData = False
    jobOptions.use35XInput = True
    jobOptions.maxEvents = -1
    jobOptions.skimType = "Muon"
    jobOptions.hltPaths = ["HLT_Mu9","HLT_Mu11"]
    jobOptions.wantSummary = True
    jobOptions.hltProcessName = "REDIGI"

  elif options.jobType == "MC36X":
    jobOptions.globalTag = "START3X_V26::All"
    jobOptions.isRealData = False
    jobOptions.use35XInput = True
    jobOptions.maxEvents = -1

  elif options.jobType != "":
    raise RuntimeError, "Unknown jobType option `%s'" % options.jobType

  ## Set all the non-default option values equal to jobOptions to preserve options set
  ##+ in the cfg file
  for name, value in options._singletons.items() + options._lists.items():
    if value != getattr(defaultOptions, name):
      ## If it's a list, clear it first
      if name in options._lists.keys(): jobOptions.clearList(name)
      setattr(jobOptions, name, value)

  jobOptions.parseArguments()

  ## Set all the options equal to jobOptions
  for name, value in jobOptions._singletons.items() + jobOptions._lists.items():
    ## If it's a list, clear it first
    if name in options._lists.keys(): options.clearList(name)
    setattr(options, name, value)

  ## Remove duplicated input files (from e.g. using the command line
  ##+ option inputFiles_load=list.txt
  duplicates = []
  for index in range(len(options.inputFiles)):
    file = options.inputFiles[index]
    if file in options.inputFiles[index+1:]:
      ## Found a duplicated file, store its index
      duplicates.append(options.inputFiles.index(file, index+1))
  ## Now remove the duplicates in the reverse order to keep the original index
  ##+ association
  duplicates.reverse()
  for index in duplicates: del options.inputFiles[index]
  
# applyMultiOptionTag(options) <----------------------------------------------


