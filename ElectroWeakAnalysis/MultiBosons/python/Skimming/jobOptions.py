def applyJobOptions(options):
  """
  Set multiple options defined by the jobType argument.
  To be called after options.parseArguments()
  """
  if options.jobType == "testSpring10McCern":
    options.maxEvents = 10
    options.inputFiles = [
      "/store/relval/CMSSW_3_5_7/" +
      "RelValZMM/GEN-SIM-RECO/START3X_V26-v1/0012/" +
      file for file in """
        10B71379-4549-DF11-9D80-003048D15D22.root
        34FD3B1D-6949-DF11-9529-0018F3D09612.root
        4C8D7358-4449-DF11-86BB-003048678A6C.root
      """.split()
    ]
    options.globalTag = "START3X_V26::All"
    options.reportEvery = 1
    options.isRealData = False
    options.use35XInput = True
    options.isMaxEventsOutput = True
    options.wantSummary = False
    return
    # end of testSpring10McCern options <-------------------------------------

  if options.jobType == "testPromptRecoV4Cern":
    options.maxEvents = 1
    options.inputFiles = [
      "/store/data/Run2010A/Mu/RECO/Jun14thReReco_v1/0006/" +
      file for file in """
        B0222EAB-B07B-DF11-AE0A-001E0B5FE542.root
      """.split()
    ]
    options.globalTag = "GR10_P_V7::All"
    options.reportEvery = 1
    options.isRealData = True
    options.use35XInput = False
    options.isMaxEventsOutput = True
    options.wantSummary = False
    return
    # end of testPromptRecoV4Cern options <-----------------------------------

  elif options.jobType == "PromptReco_v4":
    options.globalTag = "GR10_P_V7::All"
    options.isRealData = True
    options.use35XInput = False
    options.maxEvents = -1

  elif options.jobType == "Zgamma":
    options.globalTag = "START3X_V26::All"
    options.isRealData = False
    options.use35XInput = True
    options.maxEvents = -1

  elif options.jobType != "":
    raise RuntimeError, "Unknown jobType option `%s'" % options.jobType


# applyMultiOptionTag(options) <----------------------------------------------
