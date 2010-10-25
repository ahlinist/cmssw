import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# Compute ratio of parton luminosities at LHC @ xxx TeV center-of-mass energy vs. TeVatron
# for:
#   o gluon + gluon
#   o b + bbar 
#   o u + ubar, d + dbar
#--------------------------------------------------------------------------------

process = cms.Process('compPartonLuminosity')

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(0)         
)

process.source = cms.Source("EmptySource")

pdfSetFilePath = "/afs/cern.ch/user/v/veelken/scratch0/CMSSW_3_8_5/src/TauAnalysis/FittingTools/test"

process.compPartonLuminosity = cms.EDAnalyzer("PartonLuminosityAnalyzer",
    ##pdfSet = cms.string(pdfSetFilePath + "/" + "MSTW2008nlo68cl.LHgrid"),
    pdfSet = cms.string("MRST2004nlo.LHgrid"),                                      

    sqrtS_TeVatron = cms.double(1960.), # units = GeV
    sqrtS_LHC = cms.double(7000.),      # units = GeV
                                              
    massMin = cms.double(100.),
    massMax = cms.double(500.),

    canvasSizeX = cms.int32(800),
    canvasSizeY = cms.int32(640),

    xScale = cms.string("linear"),
    yScale = cms.string("log"),                                          

    yMin = cms.double(0.5),
    yMax = cms.double(1000.),                                          
 
    ##outputFilePath = cms.string("./plots"),
    outputFileName = cms.string("compPartonLuminosity.png")
)

process.p = cms.Path(process.compPartonLuminosity)
