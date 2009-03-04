import FWCore.ParameterSet.Config as cms

#
# Plot histograms for Z --> mu + tau channel
#
# Author: Christian Veelken, UC Davis
#

loadMuTau = cms.EDAnalyzer("DQMFileLoader",
  zMuMu1kEv = cms.PSet(
    inputFileNames = cms.vstring('ewkTauMuAnalyzer_zMuMu1kEv.root'), # single .root file
    scaleFactor = cms.double(1.),
    dqmDirectory_store = cms.string('zMuMu1kEv')
  ),
  zMuMu9kEv = cms.PSet(
    inputFileNames = cms.vstring('ewkTauMuAnalyzer_zMuMu1kEv_#RANGE01-04#.root', # total of 5 root files
                                 'ewkTauMuAnalyzer_zMuMu5kEv.root'),
    scaleFactor = cms.double(1.),
    dqmDirectory_store = cms.string('zMuMu9kEv')
  ),
  zMuMu10kEv = cms.PSet(
    inputFileNames = cms.vstring('ewkTauMuAnalyzer_zMuMu10kEv.root'),
    scaleFactor = cms.double(1.),
    dqmDirectory_store = cms.string('zMuMu10kEv')
  )
)


addMuTau = cms.EDAnalyzer("DQMHistAdder",
  smSum = cms.PSet(
    dqmDirectories_input = cms.vstring('zMuMu1kEv',
                                       'zMuMu9kEv'),
    dqmDirectory_output = cms.string('smSum')
  )
)

plotMuTau = cms.EDAnalyzer("DQMHistPlotter",
  processes = cms.PSet(
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
    zMuMu10kEv = cms.PSet(
      dqmDirectory = cms.string('zMuMu10kEv'),
      legendEntry = cms.string('Z #rightarrow #mu #mu (10kEv)'),      
      type = cms.string('Data') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
    ),
    smSum = cms.PSet(
      dqmDirectory = cms.string('smSum'),
      legendEntry = cms.string('All SM Processes'),
      legendEntryErrorBand = cms.string('SM Uncertainty'),
      type = cms.string('smSumMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
    )
  ),

  xAxes = cms.PSet(
    muonPt = cms.PSet(
      xAxisTitle = cms.string('P_{T} / GeV'),
      xAxisTitleOffset = cms.double(1.0),
      xAxisTitleSize = cms.double(0.05)       
#     xMin = cms.double(0.)                   
#     xMax = cms.double(100.)                
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

  yAxes = cms.PSet(                         
    numEntries_linear = cms.PSet(
      yScale = cms.string('linear'),      
      yAxisTitle = cms.string(''),        
      yAxisTitleOffset = cms.double(1.1), 
      yAxisTitleSize = cms.double(0.05)   
#     yMin_linear = cms.double(0.)        
#     yMax_linear = cms.double(1.)        
#     yMin_log = cms.double(1.e-2)      
#     yMax_log = cms.double(1.e+2)      
    ),
    numEntries_log = cms.PSet(
      yScale = cms.string('log'), 
      minY_log = cms.double(1.e-1),
      yAxisTitle = cms.string(''), 
      yAxisTitleOffset = cms.double(1.1),
      yAxisTitleSize = cms.double(0.05)
    ),
    efficiency = cms.PSet(
      yScale = cms.string('linear'), 
      yAxisTitle = cms.string('#varepsilon'), 
      yAxisTitleOffset = cms.double(1.1),
      yAxisTitleSize = cms.double(0.05)
    )
  ),

  legends = cms.PSet(
    regular = cms.PSet(
      posX = cms.double(0.60),            
      posY = cms.double(0.64),             
      sizeX = cms.double(0.29),        
      sizeY = cms.double(0.25),            
      header = cms.string(''),          
      option = cms.string('brNDC'),       
      borderSize = cms.int32(0),          
      fillColor = cms.int32(0)             
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

  labels = cms.PSet(
    pt = cms.PSet(
      posX = cms.double(0.19),                
      posY = cms.double(0.77),             
      sizeX = cms.double(0.12),            
      sizeY = cms.double(0.04),          
      option = cms.string('brNDC'),   
      borderSize = cms.int32(0),         
      fillColor = cms.int32(0),           
      textColor = cms.int32(1),           
      textSize = cms.double(0.04),        
      textAlign = cms.int32(22),          
#     textAngle = cms.double(0.),
      text = cms.vstring('P_{T} > 5 GeV')
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

  drawOptionSets = cms.PSet(
    separate = cms.PSet(
      zMuMu1kEv = cms.PSet(
#       markerColor = cms.int32(1),        
#       markerSize = cms.int32(1),         
#       markerStyle = cms.int32(2),       
        lineColor = cms.int32(30),        
        lineStyle = cms.int32(1),         
        lineWidth = cms.int32(2),         
        fillColor = cms.int32(30),        
        fillStyle = cms.int32(3004),      
        drawOption = cms.string('hist'),  
        drawOptionLegend = cms.string('f') 
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

  drawJobs = cms.PSet(
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

  canvasSizeX = cms.int32(800),
  canvasSizeY = cms.int32(640),                         

  outputFilePath = cms.string('/uscms/home/veelken/work/CMSSW_2_1_9dev/src/ElectroWeakAnalysis/EWKTau/test/plots/'),
  #outputFileName = cms.string('ewkTauMuAnalyzer_zMuMu.ps')
  indOutputFileName = cms.string('#PLOT#_zMuMu.png')
)

saveMuTau = cms.EDAnalyzer("DQMSimpleFileSaver",
  outputFileName = cms.string('ewkTauMuAnalyzer_allProcesses.root')
)
 
