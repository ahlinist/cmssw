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
      "/store/relval/CMSSW_3_8_7/RelValZMM/GEN-SIM-RECO/START38_V13-v1/" +
      file for file in """
      0017/E81C0282-93FC-DF11-B15D-00261894390A.root
      0017/50641150-9DFC-DF11-AC76-001A92971B54.root
      0016/AEA8DD07-8BFC-DF11-A505-003048678FE4.root
      0016/A4A2387F-8CFC-DF11-BCBF-00304867C136.root
      0016/86090D81-8BFC-DF11-AB45-003048679228.root
      0016/5418F1FD-89FC-DF11-9610-002618943852.root
      0016/124ACA84-8AFC-DF11-8D9B-0030486791F2.root
      """.split()
    ]
    jobOptions.globalTag = "START38_V14::All"
    jobOptions.reportEvery = 1
    jobOptions.isRealData = False
    jobOptions.use35XInput = False
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
    jobOptions.globalTag = "START38_V14::All"
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
      "/store/data/Run2010B/Mu/RECO/PromptReco-v2/000/148/953/"+
      file for file in """
      046655B5-D1E1-DF11-AC22-0030487CD906.root
      264F934A-D0E1-DF11-9725-001D09F2424A.root
      2C6FDB4A-D0E1-DF11-8894-001D09F24DDA.root
      429FAE03-D1E1-DF11-AB0E-0030487C8CBE.root
      42C8584A-D0E1-DF11-BEE9-001D09F2905B.root
      48D4F6E6-CEE1-DF11-9CBB-0030487CAF0E.root
      4A6E35B9-D1E1-DF11-BFFC-0030487CD7EA.root
      62BBFC98-CFE1-DF11-B5CE-001D09F295A1.root
      76F93BE5-CEE1-DF11-8CD8-00304879FA4C.root
      7C3F2A4A-D0E1-DF11-977F-0030487C6062.root
      9679CFE3-CEE1-DF11-89B2-003048F024FA.root
      AE275399-CFE1-DF11-8CE9-0030487CD7EA.root
      D23E709B-CFE1-DF11-801C-0030487CD13A.root
      F46275E2-CEE1-DF11-9E93-003048F1BF66.root
      FED2F305-D1E1-DF11-8595-0030487D05B0.root
      """.split()
    ]
    jobOptions.globalTag = "GR_R_38X_V15::All"
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
    jobOptions.globalTag = "GR_R_38X_V15::All"
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
    jobOptions.skimType = "ElectronPhoton"
    jobOptions.globalTag = "START38_V14::All"
    jobOptions.reportEvery = 1
    jobOptions.isRealData = False
    jobOptions.use35XInput = False
    jobOptions.wantSummary = True
    jobOptions.hltProcessName = "REDIGI36X"

  elif options.jobType == "testFall10":
    jobOptions.inputFiles = """
        /store/mc/Fall10/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/GEN-SIM-RECO/START38_V12-v2/0006/E2250407-B3E4-DF11-941F-E0CB4E29C4C6.root
        /store/mc/Fall10/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/GEN-SIM-RECO/START38_V12-v2/0006/6E58848F-87E4-DF11-81C0-001F2965D25E.root
        /store/mc/Fall10/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/GEN-SIM-RECO/START38_V12-v2/0005/CCE7C256-03E4-DF11-BD14-00261834B5BA.root
        /store/mc/Fall10/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/GEN-SIM-RECO/START38_V12-v2/0005/C8C7A3EB-ACE3-DF11-8D6D-00D0680B8852.root
        /store/mc/Fall10/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/GEN-SIM-RECO/START38_V12-v2/0005/A8CA5933-BDE3-DF11-961D-A4BADB3CEBCE.root
        /store/mc/Fall10/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/GEN-SIM-RECO/START38_V12-v2/0005/90E09BA7-AEE3-DF11-A22D-001EC9EAFEBE.root
        """.split()
    jobOptions.skimType = "ElectronPhoton"
    jobOptions.globalTag = "START38_V14::All"
    jobOptions.reportEvery = 1
    jobOptions.isRealData = False
    jobOptions.wantSummary = True
    jobOptions.hltProcessName = "HLT"

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


