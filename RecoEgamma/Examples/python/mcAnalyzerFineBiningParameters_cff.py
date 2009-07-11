import FWCore.ParameterSet.Config as cms

mcAnalyzerFineBiningParameters = cms.PSet(

    Etamin = cms.double(-2.5),
    Etamax = cms.double(2.5),
    Phimax = cms.double(3.2),
    Phimin = cms.double(-3.2),
    Ptmax = cms.double(100.0),
    Pmax = cms.double(300.0),
    Eopmax = cms.double(5.0),
    Eopmaxsht = cms.double(3.0),
    Detamin = cms.double(-0.005),
    Detamax = cms.double(0.005),
    Dphimin = cms.double(-0.01),
    Dphimax = cms.double(0.01),
    Dphimatchmin = cms.double(-0.2),
    Dphimatchmax = cms.double(0.2),
    Detamatchmin = cms.double(-0.05),
    Detamatchmax = cms.double(0.05),
    Fhitsmax = cms.double(30.0),
    Lhitsmax = cms.double(10.0),
    Nbinxyz = cms.int32(200),
    Nbineop2D = cms.int32(100),
    Nbinp = cms.int32(300),
    Nbineta2D = cms.int32(100),
    Nbinfhits = cms.int32(30),
    Nbinlhits = cms.int32(5),
    Nbinpteff = cms.int32(190),
    Nbinphi2D = cms.int32(128),
    Nbindetamatch2D = cms.int32(100),
    Nbineta = cms.int32(250),
    Nbinp2D = cms.int32(100),
    Nbindeta = cms.int32(300),
    Nbinpt2D = cms.int32(100),
    Nbindetamatch = cms.int32(300),
    Nbinphi = cms.int32(128),
    Nbindphimatch = cms.int32(300),
    Nbinpt = cms.int32(300),
    Nbindphimatch2D = cms.int32(100),
    Nbindphi = cms.int32(300),
    Nbineop = cms.int32(300),
    Nbinpoptrue = cms.int32(450),
    Poptruemin = cms.double(0.3),
    Poptruemax = cms.double(1.2),
    Nbinmee = cms.int32(300),
    Meemin = cms.double(0.0),
    Meemax = cms.double(150.),
    Nbinhoe = cms.int32(200),
    Hoemin = cms.double(0.0),
    Hoemax = cms.double(0.5)

)
