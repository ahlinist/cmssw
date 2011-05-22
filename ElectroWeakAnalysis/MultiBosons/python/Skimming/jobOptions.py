import copy
import sys
from ElectroWeakAnalysis.MultiBosons.Skimming.options import options as defaultOptions


## IDEA: manipulate the command line arguments in sys.argv to parse only the
## jobType first and then parse the rest of the options

def applyJobOptions(options):
  """
  Set multiple options defined by the jobType argument.
  To be called after options.parseArguments()
  """
  options.parseArguments()
  
  ## Attach the options.options to the command line arguments
  if options.options: sys.argv += options.options

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
  elif options.jobType == "testMC414":
    jobOptions.outEvents = 100
    jobOptions.inputFiles = [
      "/store/relval/CMSSW_4_1_4/RelValZMM/GEN-SIM-RECO/START311_V2-v1/" +
      file for file in """
      0013/40E72E1E-8F60-E011-A5A1-002618FDA211.root      
      """.split()
    ]
    jobOptions.globalTag = "START311_V2::All"
    jobOptions.reportEvery = 1
    jobOptions.isRealData = False
    jobOptions.use35XInput = False
    jobOptions.wantSummary = False
    jobOptions.skimType = "MuonPhoton"
  elif options.jobType == "testMC42X":
    jobOptions.outEvents = 100
    jobOptions.inputFiles = [
      "/store/relval/CMSSW_4_2_3/RelValZMM/GEN-SIM-RECO/START42_V12-v2/" +
      file for file in """
      0062/10F19058-767B-E011-9392-001A92810A96.root
      0062/1EE45FED-617B-E011-A1DC-00304867901A.root
      """.split()
      ]
    jobOptions.globalTag = "START42_V12::All"
    jobOptions.reportEvery = 1
    jobOptions.isRealData = False
    jobOptions.isAOD = True
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
  #      000/163/737/BE694CDA-7D74-E011-80E5-003048F118E0.root
  #      000/163/664/BA4E582E-3374-E011-AB7D-0030487C6A66.root
  elif options.jobType == "testRealData":
    jobOptions.outEvents = 500
    jobOptions.inputFiles = [
      "file:/hdfs/store/data/Run2010A/Mu/AOD/Apr21ReReco-v1/0000/"+
      file for file in """
      C47C8A0C-956F-E011-80FC-001CC4A934D8.root
      204C2D3F-0B71-E011-860F-001E0B48E92A.root
      9AB3C608-0871-E011-AD21-0017A4771008.root
      """.split()
    ]
    jobOptions.globalTag = "GR_R_42_V11::All"
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
    jobOptions.globalTag = "GR_R_311_V2::All"
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

  elif options.jobType == "testWinter10":
    jobOptions.inputFiles =  ["/store/mc/Winter10/" + \
      "DYToMuMu_M-20_CT10_TuneZ2_7TeV-powheg-pythia/" + \
      "GEN-SIM-RECO/E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/0033/" + \
      "FE75602D-4810-E011-B662-1CC1DE051038.root"
      ]
    jobOptions.skimType = "Dimuon"
    jobOptions.globalTag = "START39_V8::All"
    jobOptions.reportEvery = 1
    jobOptions.isRealData = False
    jobOptions.wantSummary = True
    jobOptions.hltProcessName = "REDIGI39X"

  elif options.jobType == "Winter10":
    jobOptions.inputFiles =  ["/store/mc/Winter10/" + \
      "DYToMuMu_M-20_CT10_TuneZ2_7TeV-powheg-pythia/" + \
      "GEN-SIM-RECO/E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/0033/" + \
      "FE75602D-4810-E011-B662-1CC1DE051038.root"
      ]
    jobOptions.skimType = "Dimuon"
    jobOptions.globalTag = "START39_V8::All"
    jobOptions.reportEvery = 1000
    jobOptions.isRealData = False
    jobOptions.wantSummary = True
    jobOptions.hltProcessName = "REDIGI39X"

  elif options.jobType == "Dec22ReReco":
    jobOptions.inputFiles =  ["/store/data/Run2010B/Mu/RECO/" + \
      "Dec22ReReco_v1/0025/60BB3418-F60F-E011-8952-90E6BA0D09CB.root"
      ]
    jobOptions.skimType = "Dimuon"
    jobOptions.globalTag = "GR_R_39X_V6::All"
    jobOptions.reportEvery = 1000
    jobOptions.isRealData = True
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

  elif options.jobType == "MUPHPromptReco414":
    jobOptions.globalTag = "GR_R_311_V2::All"
    jobOptions.isRealData = True
    jobOptions.use35XInput = False
    jobOptions.maxEvents = -1
    jobOptions.skimType = "MuonPhoton"
    jobOptions.wantSummary = True
    jobOptions.isAOD=True

  elif options.jobType == "MUPHReReco414":
    jobOptions.globalTag = "GR_R_311_V2::All"
    jobOptions.isRealData = True
    jobOptions.use35XInput = False
    jobOptions.maxEvents = -1
    jobOptions.skimType = "MuonPhoton"
    jobOptions.wantSummary = True
    jobOptions.isAOD=True

  elif options.jobType == "MUPHData42X":
    jobOptions.globalTag = "GR_R_42_V13::All"
    jobOptions.isRealData = True
    jobOptions.use35XInput = False
    jobOptions.maxEvents = -1
    jobOptions.skimType = "MuonPhoton"
    jobOptions.wantSummary = True
    jobOptions.isAOD=True

  elif options.jobType == "MUPHMCSpring11":
    jobOptions.globalTag = "START311_V2:All"
    jobOptions.isRealData = False
    jobOptions.use35XInput = False
    jobOptions.maxEvents = -1
    jobOptions.skimType = "MuonPhoton"
    jobOptions.wantSummary = True
    jobOptions.isAOD=True

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


