import FWCore.ParameterSet.Config as cms

visPtCutCorrectionMuon15 = cms.PSet(
    # correction for muon Pt > 15 GeV, abs(eta) < 2.1
    # (applied in muon + tau-jet channel)
    formula = cms.string("[0]*0.5*(1.0+[1]*x)*(1.0-TMath::Erf(-[2]*(x-[3])))"),
    xMin = cms.double(90.),
    xMax = cms.double(1000.),
    par0 = cms.PSet(
        formula = cms.string("[0]+[1]/TMath::Power(TMath::Max(10.,x-[2]),[3])"),
        xMin = cms.double(90.),
        xMax = cms.double(1000.),
        par0 = cms.double(0.887982),
        par1 = cms.double(1.86925),
        par2 = cms.double(80.3915),
        par3 = cms.double(0.416581)
    ),
    par1 = cms.PSet(
        formula = cms.string("[0]/TMath::Power(TMath::Max(10.,x-[1]),[2])"),
        xMin = cms.double(90.),
        xMax = cms.double(1000.),
        par0 = cms.double(354.892),
        par1 = cms.double(-17.8402),
        par2 = cms.double(1.31975)
    ),
    par2 = cms.PSet(
        formula = cms.string("[0]*0.5*(1.0-[1]*TMath::Erf(-[2]*(x-[3])*TMath::Power(TMath::Abs(x-[3]),[4])))"),
        xMin = cms.double(90.),
        xMax = cms.double(400.),
        par0 = cms.double(17.636),
        par1 = cms.double(2.63433),
        par2 = cms.double(0.000165353),
        par3 = cms.double(106.536),
        par4 = cms.double(0.832556)
    ),
    par3 = cms.PSet(
        formula = cms.string("[0]+[1]/TMath::Power(TMath::Max(10.,x-[2]),[3])"),
        xMin = cms.double(90.),
        xMax = cms.double(400.),
        par0 = cms.double(-1.0126),
        par1 = cms.double(1.60375),
        par2 = cms.double(80.0107),
        par3 = cms.double(0.0644025)
    )    
)

visPtCutCorrectionTau20 = cms.PSet(
    # correction for tau-jet Pt > 20 GeV, abs(eta) < 2.3
    # (applied in muon + tau-jet and electron + tau-jet channels)
    formula = cms.string("[0]*0.5*(1.0+[1]*x)*(1.0-TMath::Erf(-[2]*(x-[3])))"),
    xMin = cms.double(90.),
    xMax = cms.double(1000.),
    par0 = cms.PSet(
        formula = cms.string("[0]*0.5*(1.0-TMath::Erf(-[1]*TMath::Power(TMath::Max(10.,x-[2]),[3])))"),
        xMin = cms.double(90.),
        xMax = cms.double(1000.),
        par0 = cms.double(0.920995),
        par1 = cms.double(2.68021e-06),
        par2 = cms.double(-48.7599),
        par3 = cms.double(2.51838)
    ),
    par1 = cms.PSet(
        formula = cms.string("[0]/TMath::Power(TMath::Max(10.,x-[1]),[2])"),
        xMin = cms.double(90.),
        xMax = cms.double(1000.),
        par0 = cms.double(7.75696),
        par1 = cms.double(74.5532),
        par2 = cms.double(0.725178)
    ),
    par2 = cms.PSet(
        formula = cms.string("[0]*0.5*(1.0-[1]*TMath::Erf(-[2]*(x-[3])*TMath::Power(TMath::Abs(x-[3]),[4])))"),
        xMin = cms.double(90.),
        xMax = cms.double(300.),
        par0 = cms.double(13.4063),
        par1 = cms.double(4.62657),
        par2 = cms.double(0.000134817),
        par3 = cms.double(105.208),
        par4 = cms.double(0.63186)
    ),
    par3 = cms.PSet(
        formula = cms.string("[0]+[1]/TMath::Power(TMath::Max(10.,x-[2]),[3])"),
        xMin = cms.double(90.),
        xMax = cms.double(400.),
        par0 = cms.double(-0.79061),
        par1 = cms.double(1.56585),
        par2 = cms.double(84.8923),
        par3 = cms.double(0.0913261)
    )    
)
