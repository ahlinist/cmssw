import FWCore.ParameterSet.Config as cms

visPtCutCorrectionMuon15 = cms.PSet(
    # correction for muon Pt > 15 GeV, abs(eta) < 2.1
    # (applied in muon + tau-jet channel)
    variable = cms.string('X'),
    formula = cms.string('[0]*[1]*0.5*(1.0-TMath::Erf(-(x-([2]+[3]))/(TMath::Sqrt(2.)*[4])))'),
    xMin = cms.double(0),
    xMax = cms.double(1),
    parameter = cms.PSet(
        par0 = cms.PSet(
            variable = cms.string('dPhi'),
            formula = cms.string('[0]'),
            xMin = cms.double(0),
            xMax = cms.double(3.14159),
            parameter = cms.PSet(
                par0 = cms.PSet(
                    variable = cms.string('mass'),
                    formula = cms.string('[0]/TMath::Power(TMath::Max(1.e-2,x-[1]),[2])+[3]'),
                    xMin = cms.double(90),
                    xMax = cms.double(90),
                    parameter = cms.PSet(
                        par0 = cms.double(255.983),
                        par1 = cms.double(109.016),
                        par2 = cms.double(0.000371346),
                        par3 = cms.double(-254.234)
                    )
                )
            )
        ),
        par1 = cms.PSet(
            variable = cms.string('dPhi'),
            formula = cms.string('[0]+[1]*x+[2]*0.5*(3.*x*x-1.)'),
            xMin = cms.double(0),
            xMax = cms.double(3.14159),
            parameter = cms.PSet(
                par0 = cms.PSet(
                    variable = cms.string('mass'),
                    formula = cms.string('[0]'),
                    xMin = cms.double(90),
                    xMax = cms.double(90),
                    parameter = cms.PSet(
                        par0 = cms.double(0.789324)
                    )
                ),
                par1 = cms.PSet(
                    variable = cms.string('mass'),
                    formula = cms.string('[0]'),
                    xMin = cms.double(90),
                    xMax = cms.double(90),
                    parameter = cms.PSet(
                        par0 = cms.double(0.118926)
                    )
                ),
                par2 = cms.PSet(
                    variable = cms.string('mass'),
                    formula = cms.string('[0]'),
                    xMin = cms.double(90),
                    xMax = cms.double(90),
                    parameter = cms.PSet(
                        par0 = cms.double(-0.00498757)
                    )
                )
            )
        ),
        par2 = cms.PSet(
            variable = cms.string('dPhi'),
            formula = cms.string('[0]'),
            xMin = cms.double(0),
            xMax = cms.double(3.14159),
            parameter = cms.PSet(
                par0 = cms.PSet(
                    variable = cms.string('mass'),
                    formula = cms.string('[0]/([1]+x)'),
                    xMin = cms.double(90),
                    xMax = cms.double(90),
                    parameter = cms.PSet(
                        par0 = cms.double(30),
                        par1 = cms.double(6.93434e-10)
                    )
                )
            )
        ),
        par3 = cms.PSet(
            variable = cms.string('dPhi'),
            formula = cms.string('[0]+[1]*x'),
            xMin = cms.double(0),
            xMax = cms.double(3.14159),
            parameter = cms.PSet(
                par0 = cms.PSet(
                    variable = cms.string('mass'),
                    formula = cms.string('[0]*0.5*([1]-TMath::Erf(-[2]*(x-[3])*TMath::Power(TMath::Abs(x-[3]),[4])))'),
                    xMin = cms.double(90),
                    xMax = cms.double(90),
                    parameter = cms.PSet(
                        par0 = cms.double(0.131578),
                        par1 = cms.double(-0.380877),
                        par2 = cms.double(0.0151143),
                        par3 = cms.double(152.848),
                        par4 = cms.double(0.235714)
                    )
                ),
                par1 = cms.PSet(
                    variable = cms.string('mass'),
                    formula = cms.string('TMath::Max([0]*(x-[1]),[2])'),
                    xMin = cms.double(90),
                    xMax = cms.double(90),
                    parameter = cms.PSet(
                        par0 = cms.double(-0.000488121),
                        par1 = cms.double(196.325),
                        par2 = cms.double(-0.00888356)
                    )
                )
            )
        ),
        par4 = cms.PSet(
            variable = cms.string('dPhi'),
            formula = cms.string('[0]+[1]*x'),
            xMin = cms.double(0),
            xMax = cms.double(3.14159),
            parameter = cms.PSet(
                par0 = cms.PSet(
                    variable = cms.string('mass'),
                    formula = cms.string('[0]'),
                    xMin = cms.double(90),
                    xMax = cms.double(90),
                    parameter = cms.PSet(
                        par0 = cms.double(0.0852626)
                    )
                ),
                par1 = cms.PSet(
                    variable = cms.string('mass'),
                    formula = cms.string('[0]'),
                    xMin = cms.double(90),
                    xMax = cms.double(90),
                    parameter = cms.PSet(
                        par0 = cms.double(-0.0116553)
                    )
                ),
            )
        )
    )
)

visPtCutCorrectionTau20 = cms.PSet(
    # correction for tau-jet Pt > 20 GeV, abs(eta) < 2.3
    # (applied in muon + tau-jet and electron + tau-jet channels)
        variable = cms.string('X'),
    formula = cms.string('[0]*[1]*0.5*(1.0-TMath::Erf(-(x-([2]+[3]))/(TMath::Sqrt(2.)*[4])))'),
    xMin = cms.double(0),
    xMax = cms.double(1),
    parameter = cms.PSet(
        par0 = cms.PSet(
            variable = cms.string('dPhi'),
            formula = cms.string('[0]'),
            xMin = cms.double(0),
            xMax = cms.double(3.14159),
            parameter = cms.PSet(
                par0 = cms.PSet(
                    variable = cms.string('mass'),
                    formula = cms.string('[0]/TMath::Power(TMath::Max(1.e-2,x-[1]),[2])+[3]'),
                    xMin = cms.double(90),
                    xMax = cms.double(90),
                    parameter = cms.PSet(
                        par0 = cms.double(30.4217),
                        par1 = cms.double(52.785),
                        par2 = cms.double(1.27506),
                        par3 = cms.double(0.998027)
                    )
                )
            )
        ),
        par1 = cms.PSet(
            variable = cms.string('dPhi'),
            formula = cms.string('[0]+[1]*x+[2]*0.5*(3.*x*x-1.)'),
            xMin = cms.double(0),
            xMax = cms.double(3.14159),
            parameter = cms.PSet(
                par0 = cms.PSet(
                    variable = cms.string('mass'),
                    formula = cms.string('[0]'),
                    xMin = cms.double(90),
                    xMax = cms.double(90),
                    parameter = cms.PSet(
                        par0 = cms.double(0.897699)
                    )
                ),
                par1 = cms.PSet(
                    variable = cms.string('mass'),
                    formula = cms.string('[0]'),
                    xMin = cms.double(90),
                    xMax = cms.double(90),
                    parameter = cms.PSet(
                        par0 = cms.double(0.111121)
                    )
                ),
                par2 = cms.PSet(
                    variable = cms.string('mass'),
                    formula = cms.string('[0]'),
                    xMin = cms.double(90),
                    xMax = cms.double(90),
                    parameter = cms.PSet(
                        par0 = cms.double(-0.0129291)
                    )
                )
            )
        ),
        par2 = cms.PSet(
            variable = cms.string('dPhi'),
            formula = cms.string('[0]'),
            xMin = cms.double(0),
            xMax = cms.double(3.14159),
            parameter = cms.PSet(
                par0 = cms.PSet(
                    variable = cms.string('mass'),
                    formula = cms.string('[0]/([1]+x)'),
                    xMin = cms.double(90),
                    xMax = cms.double(90),
                    parameter = cms.PSet(
                        par0 = cms.double(40),
                        par1 = cms.double(-1.82483e-10)
                    )
                )
            )
        ),
        par3 = cms.PSet(
            variable = cms.string('dPhi'),
            formula = cms.string('[0]+[1]*x'),
            xMin = cms.double(0),
            xMax = cms.double(3.14159),
            parameter = cms.PSet(
                par0 = cms.PSet(
                    variable = cms.string('mass'),
                    formula = cms.string('[0]*0.5*([1]-TMath::Erf(-[2]*(x-[3])*TMath::Power(TMath::Abs(x-[3]),[4])))'),
                    xMin = cms.double(90),
                    xMax = cms.double(90),
                    parameter = cms.PSet(
                        par0 = cms.double(0.313877),
                        par1 = cms.double(-0.318444),
                        par2 = cms.double(1.96913e-06),
                        par3 = cms.double(95.4816),
                        par4 = cms.double(2.14619)
                    )
                ),
                par1 = cms.PSet(
                    variable = cms.string('mass'),
                    formula = cms.string('TMath::Max([0]*(x-[1]),[2])'),
                    xMin = cms.double(90),
                    xMax = cms.double(90),
                    parameter = cms.PSet(
                        par0 = cms.double(-0.00055167),
                        par1 = cms.double(174.244),
                        par2 = cms.double(-0.0283399)
                    )
                )
            )
        ),
        par4 = cms.PSet(
            variable = cms.string('dPhi'),
            formula = cms.string('[0]+[1]*x'),
            xMin = cms.double(0),
            xMax = cms.double(3.14159),
            parameter = cms.PSet(
                par0 = cms.PSet(
                    variable = cms.string('mass'),
                    formula = cms.string('[0]'),
                    xMin = cms.double(90),
                    xMax = cms.double(90),
                    parameter = cms.PSet(
                        par0 = cms.double(0.21476)
                    )
                ),
                par1 = cms.PSet(
                    variable = cms.string('mass'),
                    formula = cms.string('[0]'),
                    xMin = cms.double(90),
                    xMax = cms.double(90),
                    parameter = cms.PSet(
                        par0 = cms.double(-0.0444115)
                    )
                )
            )
        )
    )
)

