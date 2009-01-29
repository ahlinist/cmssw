#
# Example python configuration file for
# DQM Histogram plotting utility
#
# In case of questions,
# please send an email to:
#
#  Christian Veelken ( christian.veelken@cern.ch )
#

import FWCore.ParameterSet.Config as cms

#
# configuration parameters for DQMFileLoader class
#--> load histograms from .root files into DQMStore;
#    histograms get stored in the directory specified by dqmDirectory_store
#
loadMuTau = cms.EDAnalyzer("DQMFileLoader",
  zMuMu1kEv = cms.PSet(
#
# load all histograms contained in file ewkTauMuAnalyzer_zMuMu1kEv.root;
# store them in directory zMuMu1kEv
# (this defines the dummy "signal")    
#
    inputFileNames = cms.vstring('ewkTauMuAnalyzer_zMuMu1kEv.root'), # single .root file
    scaleFactor = cms.double(1.),
    dqmDirectory_store = cms.string('zMuMu1kEv')
  ),
  zMuMu9kEv = cms.PSet(
#
# load all histograms contained in files ewkTauMuAnalyzer_zMuMu1kEv_01.root, ewkTauMuAnalyzer_zMuMu1kEv_02.root,
# ewkTauMuAnalyzer_zMuMu1kEv_03.root, ewkTauMuAnalyzer_zMuMu1kEv_04.root and ewkTauMuAnalyzer_zMuMu5kEv.root;
# store them in directory zMuMu9kEv
# (this defines the dummy "background")    
#    
    inputFileNames = cms.vstring('ewkTauMuAnalyzer_zMuMu1kEv_#RANGE01-04#.root', # total of 5 root files
                                 'ewkTauMuAnalyzer_zMuMu5kEv.root'),
    scaleFactor = cms.double(1.),
    dqmDirectory_store = cms.string('zMuMu9kEv')
  ),
  zMuMu10kEv = cms.PSet(
#
# load all histograms contained in file ewkTauMuAnalyzer_zMuMu1kEv.root;
# store them in directory zMuMu10kEv
# (this defines the dummy "data")    
#    
    inputFileNames = cms.vstring('ewkTauMuAnalyzer_zMuMu10kEv.root'),
    scaleFactor = cms.double(1.),
    dqmDirectory_store = cms.string('zMuMu10kEv')
  )
)

#
# configuration parameters for DQMHistAdder class
#--> retrieve all histograms within directories specified by dqmDirectories_input from DQMStore and add them;
#    the results get stored in the directory specified by dqmDirectory_output
#
addMuTau = cms.EDAnalyzer("DQMHistAdder",
  smSum = cms.PSet(
#
# sum dummy "signal" and dummy "background" histograms;
# store the resulting "total Standard Model expectation" in directory "smSum"
#    
    dqmDirectories_input = cms.vstring('zMuMu1kEv',
                                       'zMuMu9kEv'),
    dqmDirectory_output = cms.string('smSum')
  )
)

#
# configuration parameters for DQMHistPlotter class
#--> retrieve histograms that are to be plotter from DQMStore and draw them;
#    drawing options supported by ROOT can be specified in a flexible way by configuration parameters 
#
plotMuTau = cms.EDAnalyzer("DQMHistPlotter",
  processes = cms.PSet(
#
# define dummy "signal" and dummy "backgound" to be Standard Model Monte Carlo contributions;
# set legend entries for these processes to "Z --> mu mu (1kEv)" and "Z --> mu mu (9kEv)" respectively     
#
# ( NOTE: processes of type smMC get added on top of each other in stacked plots,
#         while processes of type bsmMC does always get drawn as a separate line and **not** get added to stacked plots )
#
    zMuMu1kEv = cms.PSet(
      dqmDirectory = cms.string('zMuMu1kEv'),
      legendEntry = cms.string('Z #rightarrow #mu #mu (1kEv)'),
      type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
    ),
    zMuMu9kEv = cms.PSet(
      dqmDirectory = cms.string('zMuMu9kEv'),
      legendEntry = cms.string('Z #rightarrow #mu #mu (9kEv)'),
      type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
    ),
#
# define dummy "data" to be type data;
# set legend entry to "Z --> mu mu (10kEv)"
#    
    zMuMu10kEv = cms.PSet(
      dqmDirectory = cms.string('zMuMu10kEv'),
      legendEntry = cms.string('Z #rightarrow #mu #mu (10kEv)'),      
      type = cms.string('Data') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
    ),
#
# define dummy "total Standard Model expectation" as type smSumMC;
# set legend entry to "All SM Processes".
# A separate legend entry "SM Uncertainty", referring to the uncertainty on the total Standard Model expectation,
# is automatically added by the plotting tool.
#    
# ( NOTE: a process of type smSumMC needs to be define in case Standard Model Monte Carlo contributions are drawn separately;
#         it is **not** needed in case histograms are drawn as stacked plots )
#     
    smSum = cms.PSet(
      dqmDirectory = cms.string('smSum'),
      legendEntry = cms.string('All SM Processes'),
      legendEntryErrorBand = cms.string('SM Uncertainty'),
      type = cms.string('smSumMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
    )
  ),

#
# define title, position and size of the x-axis labels
# ( the drawing options for the x-axis labels are based on http://root.cern.ch/root/html/TAxis.html ,
#   which inherits from TAttAxis and TNamed )
#                           
#( NOTE: some configuration parameters have to be **always** specified,
#        while other configuration parameters are optional;
#        in case no value has been specified for optional configuration parameters,
#        a default value is used by the plotting tool )
#                           
  xAxes = cms.PSet(
    muonPt = cms.PSet(
      xAxisTitle = cms.string('P_{T} / GeV'), # TNamed::SetTitle         ( no default; always needs to be specified )
      xAxisTitleOffset = cms.double(1.0),     # TAttAxis::SetTitleOffset ( default = 1.0 )
      xAxisTitleSize = cms.double(0.05)       # TAttAxis::SetTitleSize   ( default = 0.05 )
#     xMin = cms.double(0.)                   # TAxis::SetLimits         ( default = not used )
#     xMax = cms.double(100.)                 #     -""-
    ),
    muonEta = cms.PSet(
      xAxisTitle = cms.string('#eta'),
      xAxisTitleOffset = cms.double(1.0),
      xAxisTitleSize = cms.double(0.05)
    ),
    muonPhi = cms.PSet(
      xAxisTitle = cms.string('#phi'),
      xAxisTitleOffset = cms.double(1.0),
      xAxisTitleSize = cms.double(0.05)
    )
  ),

#
# define title, position and size of the x-axis labels
# ( the drawing options for the y-axis labels are based on http://root.cern.ch/root/html/TAxis.html ,
#   which inherits from TAttAxis and TNamed )                           
# 
  yAxes = cms.PSet(                         
    numEntries_linear = cms.PSet(
      yScale = cms.string('linear'),      # 'linear' ( TPad::SetLogy(0) ) or 'log' ( TPad::SetLogy(1) ) scale ( default = 'linear' )
      yAxisTitle = cms.string(''),        # TNamed::SetTitle             ( no default; always needs to be specified )
      yAxisTitleOffset = cms.double(1.1), # TAttAxis::SetTitleOffset     ( default = 1.0 )
      yAxisTitleSize = cms.double(0.05)   # TAttAxis::SetTitleSize       ( default = 0.05 )
#     yMin_linear = cms.double(0.)        # TAxis::SetLimits in case of linear scale ( default = not used )
#     yMax_linear = cms.double(1.)        #     -""-
#     yMin_log = cms.double(1.e-2)        # TAxis::SetLimits in case of logarithmic scale ( default = not used )
#     yMax_log = cms.double(1.e+2)        #     -""-
    ),
    numEntries_log = cms.PSet(
      yScale = cms.string('log'), # linear/log
      minY_log = cms.double(1.e-1),
      yAxisTitle = cms.string(''), 
      yAxisTitleOffset = cms.double(1.1),
      yAxisTitleSize = cms.double(0.05)
    ),
    efficiency = cms.PSet(
      yScale = cms.string('linear'), # linear/log
      yAxisTitle = cms.string('#varepsilon'), 
      yAxisTitleOffset = cms.double(1.1),
      yAxisTitleSize = cms.double(0.05)
    )
  ),

#
# define position and drawing options for legend
# ( the drawing options for the legend are based on http://root.cern.ch/root/html/TLegend.html ,
#   which inherits from TBox (via TPave), TAttLine and TAttFill (via TBox), TAttText )
#                           
# ( NOTE: legend entries get added automatically by the drawing tool,
#         according to which processes are drawn 
#
#         the configuration parameters posX, posY, sizeX and sizeY are defined in a coordinate system
#         in which (0.,0.) refers to the lower left corner and (1.,1.) to the upper right corner of the plot area )                            
#
  legends = cms.PSet(
    regular = cms.PSet(
      posX = cms.double(0.60),             # TPave::SetX1NDC             ( default = 0.50 )
      posY = cms.double(0.64),             # TPave::SetY1NDC             ( default = 0.50 ) 
      sizeX = cms.double(0.29),            # TPave::SetX2NDC(posX + sizeX) ( default = 0.39 )
      sizeY = cms.double(0.25),            # TPave::SetY2NDC(posY + sizeY) ( default = 0.34 )
      header = cms.string(''),             # TLegend::SetHeader          ( default = "" )
      option = cms.string('brNDC'),        # TPave::SetOption            ( default = "brNDC" == )
#
# ( NOTE: see http://root.cern.ch/root/html/TPave.html#TPave:TPave for description of available options )
#      
      borderSize = cms.int32(0),           # TPave::SetBorderSize        ( default = 0 == no border )
      fillColor = cms.int32(0)             # TAttFill::SetFillColor      ( default = 0 == transparent )
#
# ( NOTE: see http://root.cern.ch/root/html/TAttFill.html for list of available fill-colors )
#      
    ),
    eff_processes = cms.PSet(
      posX = cms.double(0.60),
      posY = cms.double(0.77),
      sizeX = cms.double(0.29),
      sizeY = cms.double(0.12),
      header = cms.string(''),
      option = cms.string('brNDC'),
      borderSize = cms.int32(0),
      fillColor = cms.int32(0)
    ),
    eff_overlay = cms.PSet(
      posX = cms.double(0.60),
      posY = cms.double(0.64),
      sizeX = cms.double(0.29),
      sizeY = cms.double(0.25),
      header = cms.string(''),
      option = cms.string('brNDC'),
      borderSize = cms.int32(0),
      fillColor = cms.int32(0)
    )
  ),

#
# define position and drawing options for labels
# ( the drawing options for the labels are based on http://root.cern.ch/root/html/TPaveText.html ,
#   which inherits from TBox (via TPave), TAttLine and TAttFill (via TBox), TAttText )
#
# ( NOTE: the configuration parameters posX, posY, sizeX and sizeY are defined in a coordinate system
#         in which (0.,0.) refers to the lower left corner and (1.,1.) to the upper right corner of the plot area )                           
#
  labels = cms.PSet(
    pt = cms.PSet(
      posX = cms.double(0.19),             # TPave::SetX1NDC             ( default = 0.66 )        
      posY = cms.double(0.77),             # TPave::SetY1NDC             ( default = 0.82 ) 
      sizeX = cms.double(0.12),            # TPave::SetX2NDC(posX + sizeX) ( default = 0.26 )
      sizeY = cms.double(0.04),            # TPave::SetY2NDC(posY + sizeY) ( default = 0.10 )
      option = cms.string('brNDC'),        # TPave::SetOption            ( default = "brNDC" == )
      borderSize = cms.int32(0),           # TPave::SetBorderSize        ( default = 0 == no border )
      fillColor = cms.int32(0),            # TAttFill::SetFillColor      ( default = 0 == transparent )
      textColor = cms.int32(1),            # TAttText::SetTextColor      ( default = 1 == black )
      textSize = cms.double(0.04),         # TAttText::SetTextSize       ( default = 0.05 )
      textAlign = cms.int32(22),           # TAttText::SetTextAlign      ( default = 22 == horizontally and vertically centered )
#     textAngle = cms.double(0.),          # TAttText::SetTextAngle      ( default = 0. == text drawn horizontally )
#
# ( NOTE: see http://root.cern.ch/root/html/TAttText.html for list of available alignment options and text angle definition )
#
      text = cms.vstring('P_{T} > 5 GeV')  # TPaveText::AddText
#
# ( NOTE: multiple lines of text can be added at once by specifying a list of comma separated strings for the text configuration parameter )
#      
    ),
    eta = cms.PSet(
      posX = cms.double(0.19),
      posY = cms.double(0.83),
      sizeX = cms.double(0.12),
      sizeY = cms.double(0.04),
      option = cms.string('brNDC'),
      borderSize = cms.int32(0),
      fillColor = cms.int32(0),
      textColor = cms.int32(1),
      textSize = cms.double(0.04),
      textAlign = cms.int32(22),
      text = cms.vstring('-2.5 < #eta < +2.5')
    )
  ),

#
# define draw options for histograms
# for the case where the same quantity (e.g. muon Pt)
# is drawn for different processes
#
  drawOptionSets = cms.PSet(
    separate = cms.PSet(
#
# ( NOTE: in the drawOptionSets, one PSet needs to be specified for each process to be drawn,
#         i.e. the names zMuMu1kEv, zMuMu9kEv, zMuMu10kEv and smSum specified in the following 
#              need to match the names defined in the processes section of the configuration parameters )    
#
      zMuMu1kEv = cms.PSet(
#       markerColor = cms.int32(1),        # TAttMarker::SetMarkerColor ( default = 1 == black )
#       markerSize = cms.int32(1),         # TAttMarker::SetMarkerSize ( default = 1 == small ) 
#       markerStyle = cms.int32(2),        # TAttMarker::SetMarkerStyle ( default = 2 == cross )
#
# ( NOTE: see http://root.cern.ch/root/htmldoc/TAttMarker.html for a description of the available options how markers get drawn )
#    
        lineColor = cms.int32(30),         # TAttLine::SetLineColor ( default = 0 )
        lineStyle = cms.int32(1),          # TAttLine::SetLineStyle ( default = 1 == solid line)
        lineWidth = cms.int32(2),          # TAttLine::SetLineWidth ( default = 2 == two pixels wide )
        fillColor = cms.int32(30),         # TAttFill::SetFillColor ( default = 0 == transparent )
        fillStyle = cms.int32(3004),       # TAttFill::SetFillStyle ( default = 1001 == solid fill pattern )
        drawOption = cms.string('hist'),   # TH1::Draw ( default = 1001 == solid fill pattern )
#
# ( NOTE: see http://root.cern.ch/root/html/THistPainter.html for the histogram drawing-options ;
#         "eBand" is an extra draw-option specific to the drawing tool
#        - it adds a shaded band indicating the uncertainty on the total Standard Model expectation,
#         cf. slide 11 of http://indico.cern.ch/materialDisplay.py?contribId=3&sessionId=4&materialId=slides&confId=44892 )
#        
        drawOptionLegend = cms.string('f') # TLegend::AddEntry ( default = "lpf" )
#
# ( NOTE: drawOptionLegend specifies whether histograms are referenced in the legend
#         by line (l), marker (p) or fill area (f) )
#
      ),
      zMuMu9kEv = cms.PSet(
        lineColor = cms.int32(42),
        lineStyle = cms.int32(1),
        lineWidth = cms.int32(2),
        fillColor = cms.int32(42),
        fillStyle = cms.int32(3005),
        drawOption = cms.string('hist'),
        drawOptionLegend = cms.string('f')
      ),
      zMuMu10kEv = cms.PSet(
        markerColor = cms.int32(1),
        markerSize = cms.double(1.),
        markerStyle = cms.int32(8),
        lineColor = cms.int32(1),
        lineStyle = cms.int32(1),
        lineWidth = cms.int32(2),
        drawOption = cms.string('e1p'),
        drawOptionLegend = cms.string('p')
      ),
      smSum = cms.PSet(
        lineColor = cms.int32(1),
        lineStyle = cms.int32(1),
        lineWidth = cms.int32(2),
        fillColor = cms.int32(3),
        drawOption = cms.string('eBand'),
        drawOptionLegend = cms.string('l')
      )
    ),
    stacked = cms.PSet(
      zMuMu1kEv = cms.PSet(
        lineColor = cms.int32(2),
        lineStyle = cms.int32(1),
        lineWidth = cms.int32(1),
        fillColor = cms.int32(2),
        fillStyle = cms.int32(1001), #3004
        drawOption = cms.string('hist'),
        drawOptionLegend = cms.string('f')
      ),
      zMuMu9kEv = cms.PSet(
        lineColor = cms.int32(6),
        lineStyle = cms.int32(1),
        lineWidth = cms.int32(1),
        fillColor = cms.int32(6),
        fillStyle = cms.int32(1001), #3005
        drawOption = cms.string('hist'),
        drawOptionLegend = cms.string('f')
      ),
      zMuMu10kEv = cms.PSet(
        markerColor = cms.int32(1),
        markerSize = cms.double(1.),
        markerStyle = cms.int32(8),
        lineColor = cms.int32(1),
        lineStyle = cms.int32(1),
        lineWidth = cms.int32(2),
        drawOption = cms.string('e1p'),
        drawOptionLegend = cms.string('p')
      ),
      stack = cms.vstring('zMuMu1kEv',
                          'zMuMu9kEv')
    ),   
    eff_processes = cms.PSet(
      zMuMu1kEv = cms.PSet(
        markerColor = cms.int32(1),
        markerSize = cms.double(1.),
        markerStyle = cms.int32(20),
        lineColor = cms.int32(1),
        lineStyle = cms.int32(1),
        lineWidth = cms.int32(2),
        drawOption = cms.string('e1p'),
        drawOptionLegend = cms.string('p')
      ),
      zMuMu10kEv = cms.PSet(
        markerColor = cms.int32(2),
        markerSize = cms.double(1.),
        markerStyle = cms.int32(24),
        lineColor = cms.int32(2),
        lineStyle = cms.int32(1),
        lineWidth = cms.int32(2),
        drawOption = cms.string('e1p'),
        drawOptionLegend = cms.string('p')
      )
    )
  ),

#
# define draw options for histograms
# for the case where different quantities (e.g. efficiencies for loose/medium/tight muon id.)
# is drawn (compared) for the same process
#
  drawOptionEntries = cms.PSet(
    eff_overlay01 = cms.PSet(
      markerColor = cms.int32(3),
      markerSize = cms.double(1.),
      markerStyle = cms.int32(24),
      lineColor = cms.int32(3),
      lineStyle = cms.int32(1),
      lineWidth = cms.int32(2),
      drawOption = cms.string('e1p'),
      drawOptionLegend = cms.string('p')
    ),
    eff_overlay02 = cms.PSet(
      markerColor = cms.int32(7),
      markerSize = cms.double(1.),
      markerStyle = cms.int32(24),
      lineColor = cms.int32(7),
      lineStyle = cms.int32(1),
      lineWidth = cms.int32(2),
      drawOption = cms.string('e1p'),
      drawOptionLegend = cms.string('p')
    ),
    eff_overlay03 = cms.PSet(
      markerColor = cms.int32(4),
      markerSize = cms.double(1.),
      markerStyle = cms.int32(24),
      lineColor = cms.int32(4),
      lineStyle = cms.int32(1),
      lineWidth = cms.int32(2),
      drawOption = cms.string('e1p'),
      drawOptionLegend = cms.string('p')
    ),
    eff_overlay04 = cms.PSet(
      markerColor = cms.int32(6),
      markerSize = cms.double(1.),
      markerStyle = cms.int32(24),
      lineColor = cms.int32(6),
      lineStyle = cms.int32(1),
      lineWidth = cms.int32(2),
      drawOption = cms.string('e1p'),
      drawOptionLegend = cms.string('p')
    ),
    eff_overlay05 = cms.PSet(
      markerColor = cms.int32(2),
      markerSize = cms.double(1.),
      markerStyle = cms.int32(24),
      lineColor = cms.int32(2),
      lineStyle = cms.int32(1),
      lineWidth = cms.int32(2),
      drawOption = cms.string('e1p'),
      drawOptionLegend = cms.string('p')
    )
  ),

#
# specify draw-jobs
# (each draw-job can create one or more plots)
#
# ( NOTE: see http://indico.cern.ch/materialDisplay.py?contribId=3&sessionId=4&materialId=slides&confId=44892
#         for a description how processes, x-axes and y-axes, legends, labels and drawOptionSets/drawOptionEntries
#         need to be referenced by the drawJobs )                           
#
  drawJobs = cms.PSet(
#
# plot muon Pt, eta and phi for dummy "signal" and dummy "background" processes,
# the dummy "total Standard Model expectation" in comparisson to the dummy "data"
# (the dummy "signal" and dummy "background" processes are drawn as **separate** lines)
#    
    muonKine_sep = cms.PSet(
      plots = cms.PSet(
        dqmMonitorElements = cms.vstring('#PROCESSDIR#/MuonQuantities/Muon#PAR#',
                                         '#PROCESSDIR#/MuonQuantities/MuonIdSel#PAR#'),
        processes = cms.vstring('zMuMu1kEv', 'zMuMu9kEv', 'zMuMu10kEv', 'smSum')
      ),
      parameter = cms.vstring('Pt', 'Eta', 'Phi'),
      xAxis = cms.string('muon#PAR#'),
      yAxis = cms.string('numEntries_linear'),
      legend = cms.string('regular'),
      labels = cms.vstring(''),                   
      drawOptionSet = cms.string('separate')                 
    ),
    muonKine_stack = cms.PSet(
#
# plot muon Pt, eta and phi for dummy "signal" and dummy "background" processes in comparisson to the dummy "data"
# (the dummy "signal" and dummy "background" processes are drawn as **stacked** filled areas)
#    
      plots = cms.PSet(
        dqmMonitorElements = cms.vstring('#PROCESSDIR#/MuonQuantities/Muon#PAR#',
                                         '#PROCESSDIR#/MuonQuantities/MuonIdSel#PAR#'),
        processes = cms.vstring('zMuMu1kEv', 'zMuMu9kEv', 'zMuMu10kEv')
      ),
      parameter = cms.vstring('Pt', 'Eta', 'Phi'),
      xAxis = cms.string('muon#PAR#'),
      yAxis = cms.string('numEntries_log'),
      legend = cms.string('regular'),
      labels = cms.vstring(''),                   
      drawOptionSet = cms.string('stacked'),
      stack =  cms.vstring('zMuMu1kEv', 'zMuMu9kEv')
    ),
    muonIdEff_Pt = cms.PSet(
      plots = cms.PSet(
        dqmMonitorElements = cms.vstring('#PROCESSDIR#/MuonQuantities/MuonIdEffPt'),
        processes = cms.vstring('zMuMu1kEv', 'zMuMu10kEv')
      ),
      xAxis = cms.string('muonPt'),
      yAxis = cms.string('efficiency'),
      legend = cms.string('eff_processes'),
      labels = cms.vstring('eta'),
      drawOptionSet = cms.string('eff_processes')
    ),    
    muonIdEff_Eta = cms.PSet(
      plots = cms.PSet(
        dqmMonitorElements = cms.vstring('#PROCESSDIR#/MuonQuantities/MuonIdEffEta'),
        processes = cms.vstring('zMuMu1kEv', 'zMuMu10kEv')
      ),
      xAxis = cms.string('muonEta'),
      yAxis = cms.string('efficiency'),
      legend = cms.string('eff_processes'),
      labels = cms.vstring('pt'),
      drawOptionSet = cms.string('eff_processes')
    ),  
    muonIdEff_Phi = cms.PSet(
      plots = cms.PSet(
        dqmMonitorElements = cms.vstring('#PROCESSDIR#/MuonQuantities/MuonIdEffPhi'),
        processes = cms.vstring('zMuMu1kEv', 'zMuMu10kEv')
      ),
      xAxis = cms.string('muonPhi'),
      yAxis = cms.string('efficiency'),
      legend = cms.string('eff_processes'),
      labels = cms.vstring('pt', 'eta'),
      drawOptionSet = cms.string('eff_processes')
    ),  
    muonSelEff = cms.PSet(
#
# plot comparisson of different muon Id. efficiencies
# (the different efficiencies are symbolized by markers of different colors)
#      
      plots = cms.VPSet(
        cms.PSet(
          dqmMonitorElements = cms.vstring('#PROCESSDIR#/MuonQuantities/MuonHLTmatchEff#PAR#'),
          process = cms.string('zMuMu10kEv'),
          drawOptionEntry = cms.string('eff_overlay01'),
          legendEntry = cms.string('HLT Matching')
        ),
        cms.PSet(
          dqmMonitorElements = cms.vstring('#PROCESSDIR#/MuonQuantities/MuonTrkIsoEff#PAR#'),
          process = cms.string('zMuMu10kEv'),
          drawOptionEntry = cms.string('eff_overlay02'),
          legendEntry = cms.string('Track Iso.')
        ),
        cms.PSet(
          dqmMonitorElements = cms.vstring('#PROCESSDIR#/MuonQuantities/MuonEcalIsoEff#PAR#'),
          process = cms.string('zMuMu10kEv'),
          drawOptionEntry = cms.string('eff_overlay03'),
          legendEntry = cms.string('ECAL Iso.')
        ),
        cms.PSet(
          dqmMonitorElements = cms.vstring('#PROCESSDIR#/MuonQuantities/MuonHcalIsoEff#PAR#'),
          process = cms.string('zMuMu10kEv'),
          drawOptionEntry = cms.string('eff_overlay04'),
          legendEntry = cms.string('HCAL Iso.')
        ),
        cms.PSet(
          dqmMonitorElements = cms.vstring('#PROCESSDIR#/MuonQuantities/MuonIdEff#PAR#'),
          process = cms.string('zMuMu10kEv'),
          drawOptionEntry = cms.string('eff_overlay05'),
          legendEntry = cms.string('Muon Id.')
        )
      ),
      parameter = cms.vstring('Pt', 'Eta', 'Phi'),
      title = cms.string('muonSelectionEfficiencies#PAR#'),
      xAxis = cms.string('muon#PAR#'),
      yAxis = cms.string('efficiency'),
      legend = cms.string('regular'),
      labels = cms.vstring('')
    )
  ), 

#
# define dimension of the plots;
# set canvasSizeX = 800 and canvasSizeY = 640 (canvasSizeX = canvasSizeY = 600)
# in case you want the plots to be drawn in landscape format ( to be quadratic)
#
  canvasSizeX = cms.int32(800),
  canvasSizeY = cms.int32(640),                         

#
# define path and names of output files
#
# if outputFileName has been specified, a postscript file containing all plots is created with that name;
# if indOutputFileName has been specified, individual graphics files are created
# ( #PLOT# gets replaced with the name of the MonitorElement (e.g. "MuonIdEffPhi");
#   the type of the graphics file is determined automatically from the suffix (e.g. ".png") of the file name )
#
# ( NOTE: you must specify either outputFileName or indOutputFileName - not both )                        
#
  outputFilePath = cms.string('/uscms/home/veelken/work/CMSSW_2_1_9dev/src/ElectroWeakAnalysis/EWKTau/test/plots/'),
  #outputFileName = cms.string('ewkTauMuAnalyzer_zMuMu.ps')
  indOutputFileName = cms.string('#PLOT#_zMuMu.png')
)

saveMuTau = cms.EDAnalyzer("DQMSimpleFileSaver",
  outputFileName = cms.string('ewkTauMuAnalyzer_allProcesses.root')
)
 
