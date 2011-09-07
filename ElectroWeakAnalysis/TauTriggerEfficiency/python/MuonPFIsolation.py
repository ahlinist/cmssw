import FWCore.ParameterSet.Config as cms
import CommonTools.ParticleFlow.Isolation.tools_cfi as tools

def addMuonPFIsolation(process, muonSrc):
    sequence = cms.Sequence()

    process.load("CommonTools.ParticleFlow.pfNoPileUp_cff")
    process.load("CommonTools.ParticleFlow.ParticleSelectors.pfSortByType_cff")

    # No PFchs, this is just a pre-requisite for pfSortByType (and a
    # placeholder for chs for the future)
    process.pfPileUp.Enable = False
    sequence *= process.pfNoPileUpSequence

    # Do the sorting by particle types
    sequence *= process.pfSortByTypeSequence

    # Create the iso deposits
    process.muPFIsoDepositAll     = tools.isoDepositReplace(muonSrc, "pfNoPileUp")
    process.muPFIsoDepositCharged = tools.isoDepositReplace(muonSrc, "pfAllChargedHadrons")
    process.muPFIsoDepositNeutral = tools.isoDepositReplace(muonSrc, "pfAllNeutralHadrons")
    process.muPFIsoDepositGamma   = tools.isoDepositReplace(muonSrc, "pfAllPhotons")
    sequence *= (process.muPFIsoDepositAll *
                 process.muPFIsoDepositCharged *
                 process.muPFIsoDepositNeutral *
                 process.muPFIsoDepositGamma)

    # Create the valus calculators
    prototype = cms.EDProducer("CandIsolatorFromDeposits",
        deposits = cms.VPSet(
            cms.PSet(
                src = cms.InputTag("dummy"),
                deltaR = cms.double(0.4),
                weight = cms.string('1'),
                vetos = cms.vstring(""),
                skipDefaultVeto = cms.bool(True),
                mode = cms.string('sum')
            )
        )
    )

    for a in ["All", "Charged", "Neutral", "Gamma"]:
        m = prototype.clone()
        m.deposits[0].src = "muPFIsoDeposit"+a
        setattr(process, "muPFIsoValue"+a, m)
        sequence *= m

    # Set the thresholds 
    process.muPFIsoValueAll.deposits[0].vetos = ['ConeVeto(0.001)','Threshold(1.0)']
    process.muPFIsoValueCharged.deposits[0].vetos = ['ConeVeto(0.0001)','Threshold(1.0)']
    process.muPFIsoValueNeutral.deposits[0].vetos = ['ConeVeto(0.01)','Threshold(1.0)']
    process.muPFIsoValueGamma.deposits[0].vetos = ['ConeVeto(0.01)','Threshold(1.0)']

    return sequence

    
