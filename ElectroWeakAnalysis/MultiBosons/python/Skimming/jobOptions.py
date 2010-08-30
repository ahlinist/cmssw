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
    options.hltPaths = ["HLT_Mu9"]
    return
    # end of testSpring10McCern options <-------------------------------------

  if options.jobType == "testPromptRecoV4Cern":
    options.maxEvents = 1
    options.inputFiles = [
      "/store/data/Run2010A/Mu/RECO/Jun14thReReco_v1/0005/" +
      file for file in """
        00E4E6F2-EF7A-DF11-AF08-001E0B48E92A.root
        02546700-F07A-DF11-B921-00237DA1CD92.root
        089CCAFE-EF7A-DF11-8A36-00237DA12CA0.root
        0A33DA73-F07A-DF11-B717-00237DA10D14.root
        0AF279DA-F37A-DF11-844B-0017A4770430.root
        2475F48D-F17A-DF11-BAAD-0017A4770008.root
        2C0B3D6D-F47A-DF11-A646-0017A4770C38.root
        2EC3A00D-F07A-DF11-9B09-00237DA41368.root
        30212CEB-EF7A-DF11-9EAE-0017A4770828.root
        3CDCFEE7-EF7A-DF11-8FE4-0017A4771004.root
        3E6B78EA-F17A-DF11-AEF9-0017A4771030.root
        40ACF3E7-EF7A-DF11-A840-0017A4770430.root
        48CE97B5-F47A-DF11-9BF4-001E0B4A0EFC.root
        584B5E90-F37A-DF11-A8D5-0017A4770024.root
        6A86E9F7-EF7A-DF11-A7AF-0017A4770C34.root
        6AC5E58D-F17A-DF11-AAC3-0017A4770004.root
        720CA171-F47A-DF11-BF4D-0017A477041C.root
        72BE83F5-EF7A-DF11-9669-001CC443B76C.root
        84070BDC-EF7A-DF11-BA3C-00237DA13CAC.root
        94B4688F-EF7A-DF11-9512-0017A4770C10.root
        94C12994-EF7A-DF11-91F9-001E0B5FC422.root
        A8DD8DF0-EF7A-DF11-9176-001E0BEACAB8.root
        B0A68691-F37A-DF11-920E-00237DA14F92.root
        B25323B8-F47A-DF11-932C-0017A4770028.root
        C06427E0-EF7A-DF11-861D-0017A4771004.root
        CA4BCC71-F07A-DF11-8275-0017A4771000.root
        CC6AA9E2-EF7A-DF11-A84D-001E0B5FC422.root
        DE7D7CD9-F17A-DF11-83AB-0017A4770004.root
        DED36991-F17A-DF11-A857-0017A477141C.root
        E6F21093-EF7A-DF11-9985-0017A4770C38.root
        EEBDBF25-F47A-DF11-9E8F-0017A477083C.root
        F25FF224-F47A-DF11-AE84-0017A4770828.root
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
