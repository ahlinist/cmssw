import FWCore.ParameterSet.Config as cms

import copy

from PhysicsTools.PatAlgos.tools.jetTools import *
from PhysicsTools.PatUtils.tools.metUncertaintyTools import runMEtUncertainties

#--------------------------------------------------------------------------------
# Selection of W --> e nu candidate events passing VTBF selection
# documented in CMS PAS EWK-10-005
#--------------------------------------------------------------------------------

def filterTightWenuCandidates(process, isMC):

    process.tightWenuSelectionSequence = cms.Sequence()

    # Trigger requirements
    process.load("HLTrigger.HLTfilters.hltHighLevel_cfi")
    process.wenuHLTFilter = process.hltHighLevel.clone(
        TriggerResultsTag = cms.InputTag("TriggerResults", "", "HLT"),
        HLTPaths = cms.vstring([
            # single electron triggers (2011 Run B and Summer'11 MC)
            'HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v1',
            # single electron triggers (2011 Run B)
            'HLT_Ele25_CaloIdL_CaloIsoVL_TrkIdVL_TrkIsoVL_v5',
            # single electron triggers (Summer'11 MC)
            'HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v2',
        ]),
        throw = cms.bool(False)
    )
    process.tightWenuSelectionSequence += process.wenuHLTFilter

    # Vertex selection
    process.goodVertex = cms.EDFilter("VertexSelector",
        src = cms.InputTag("offlinePrimaryVertices"),
        cut = cms.string("isValid & ndof >= 4 & abs(z) < 24 & abs(position.Rho) < 2"),
        filter = cms.bool(True)
    )
    process.tightWenuSelectionSequence += process.goodVertex
    
    process.load("CommonTools.ParticleFlow.pfNoPileUp_cff")
    process.pfPileUp.Enable = cms.bool(True)
    process.pfPileUp.checkClosestZVertex = cms.bool(True)
    process.tightWenuSelectionSequence += process.pfNoPileUpSequence

    process.load("CommonTools.ParticleFlow.pfParticleSelection_cff")
    process.tightWenuSelectionSequence += process.pfParticleSelectionSequence

    process.load("PhysicsTools/PatAlgos/patSequences_cff")

    # compute electron IsoDeposits
    process.load("TauAnalysis.Skimming.electronPFIsolationDeposits_cff")
    process.load("TauAnalysis.Skimming.electronPFIsolationValues_cff")

    process.electronPFIsolationSequence =  cms.Sequence(
        process.electronPFIsolationDepositsSequence
       * process.electronPFIsolationValuesSequence
    )                                         
    process.tightWenuSelectionSequence += process.electronPFIsolationSequence
    
    # configure pat::Electron production
    
    process.patElectrons.isoDeposits = cms.PSet(
        # CV: strings for IsoDeposits defined in PhysicsTools/PatAlgos/plugins/PATMuonProducer.cc
        pfChargedHadrons = cms.InputTag("elecPFIsoDepositCharged"),
        pfNeutralHadrons = cms.InputTag("elecPFIsoDepositNeutral"),
        pfPhotons = cms.InputTag("elecPFIsoDepositGamma"),
        user = cms.VInputTag(
            cms.InputTag("elecPFIsoDepositChargedAll"),
            cms.InputTag("elecPFIsoDepositPU")
        )
    )

    process.patElectrons.userIsolation = cms.PSet(
        # CV: strings for Isolation values defined in PhysicsTools/PatAlgos/src/MultiIsolator.cc
        pfChargedHadron = cms.PSet(
            deltaR = cms.double(0.4),
            src = process.patElectrons.isoDeposits.pfChargedHadrons,
            vetos = process.elecPFIsoValueCharged04.deposits[0].vetos,
            skipDefaultVeto = process.elecPFIsoValueCharged04.deposits[0].skipDefaultVeto
        ),
        pfNeutralHadron = cms.PSet(
            deltaR = cms.double(0.4),
            src = process.patElectrons.isoDeposits.pfNeutralHadrons,
            vetos = process.elecPFIsoValueNeutral04.deposits[0].vetos,
            skipDefaultVeto = process.elecPFIsoValueNeutral04.deposits[0].skipDefaultVeto
        ),
        pfGamma = cms.PSet(
            deltaR = cms.double(0.4),
            src = process.patElectrons.isoDeposits.pfPhotons,
            vetos = process.elecEBPFIsoValueGamma04.deposits[0].vetos,
            skipDefaultVeto = process.elecEBPFIsoValueGamma04.deposits[0].skipDefaultVeto
        ),
        user = cms.VPSet(
            cms.PSet(
                deltaR = cms.double(0.4),
                src = process.patElectrons.isoDeposits.user[0],
                vetos = process.elecEBPFIsoValueChargedAll04.deposits[0].vetos,
                skipDefaultVeto = process.elecEBPFIsoValueChargedAll04.deposits[0].skipDefaultVeto
            ),
            cms.PSet(
                deltaR = cms.double(0.4),
                src = process.patElectrons.isoDeposits.user[1],
                vetos = process.elecPFIsoValuePU04.deposits[0].vetos,
                skipDefaultVeto = process.elecPFIsoValuePU04.deposits[0].skipDefaultVeto
            ),
            cms.PSet(
                deltaR = cms.double(0.4),
                src = process.patElectrons.isoDeposits.pfPhotons,
                vetos = process.elecEEPFIsoValueGamma04.deposits[0].vetos,
                skipDefaultVeto = process.elecEEPFIsoValueGamma04.deposits[0].skipDefaultVeto
            ),
            cms.PSet(
                deltaR = cms.double(0.4),
                src = process.patElectrons.isoDeposits.user[0],
                vetos = process.elecEEPFIsoValueChargedAll04.deposits[0].vetos,
                skipDefaultVeto = process.elecEEPFIsoValueChargedAll04.deposits[0].skipDefaultVeto
            )
        )
    )

    process.patElectrons.addGenMatch = cms.bool(False)
    process.patElectrons.embedHighLevelSelection = cms.bool(True)
    process.patElectrons.usePV = cms.bool(False) # compute transverse impact parameter wrt. beamspot (not event vertex)

    # select tight electrons, no isolation cuts applied
    process.selectedElectronsWP80 = cms.EDFilter("PATElectronSelector",
        src = cms.InputTag("patElectrons"),
        cut = cms.string(
            'pt > 25 & abs(eta) < 2.1 & ' + \
            '(isEB & ' + \
            ' sigmaIetaIeta < 0.010 & ' + \
            ' deltaPhiSuperClusterTrackAtVtx < 0.06 & deltaEtaSuperClusterTrackAtVtx < 0.004 & ' + \
            ' hadronicOverEm < 0.04) | ' + \
            '(isEE & ' + \
            ' sigmaIetaIeta < 0.030 & ' + \
            ' deltaPhiSuperClusterTrackAtVtx < 0.03 & deltaEtaSuperClusterTrackAtVtx < 0.007 & ' + \
            ' hadronicOverEm < 0.025)'
        ),
        filter = cms.bool(True)
    )

    process.selectedElectronsWP80conversionVeto = cms.EDFilter("NPATElectronConversionFinder",
        src = cms.InputTag("selectedElectronsWP80"),                                                       
        filter = cms.bool(True)
    )                                                               

    # select loose electrons, used for di-electron veto
    process.selectedElectronsWP95 = cms.EDFilter("PATElectronSelector",
        src = cms.InputTag("patElectrons"),
        cut = cms.string(
            'pt > 20 & abs(eta) < 2.5 & ' + \
            'gsfTrack.isNonnull & gsfTrack.trackerExpectedHitsInner.numberOfHits <= 1 &' + \
            '(isEB & ' + \
            ' sigmaIetaIeta < 0.010 & ' + \
            ' deltaPhiSuperClusterTrackAtVtx < 0.80 & deltaEtaSuperClusterTrackAtVtx < 0.007 & ' + \
            ' hadronicOverEm < 0.15) | ' + \
            '(isEE & ' + \
            ' sigmaIetaIeta < 0.030 & ' + \
            ' deltaPhiSuperClusterTrackAtVtx < 0.70 & deltaEtaSuperClusterTrackAtVtx < 0.010 & ' + \
            ' hadronicOverEm < 0.07)'
        ),
        filter = cms.bool(False)
    )

    # select tight electrons which are isolated
    process.selectedIsoElectronsWP80 = cms.EDFilter("PATElectronSelector",
        src = cms.InputTag("selectedElectronsWP80conversionVeto"),
        cut = cms.string(
            '(isEB & ' + \
            '(userIsolation("pat::User1Iso")' + \
            ' + max(0., userIsolation("pat::PfNeutralHadronIso") + userIsolation("pat::PfGammaIso")' + \
            '          - 0.5*userIsolation("pat::User2Iso"))) < 0.06*pt) | ' + \
            '(isEE & ' + \
            '(userIsolation("pat::User4Iso")' + \
            ' + max(0., userIsolation("pat::PfNeutralHadronIso") + userIsolation("pat::User3Iso")' + \
            '          - 0.5*userIsolation("pat::User2Iso"))) < 0.06*pt)'
        ),                                
        filter = cms.bool(False)
    )

    process.patDefaultSequence.replace(
      process.patElectrons,
      process.patElectrons
     * process.selectedElectronsWP80 * process.selectedElectronsWP80conversionVeto * process.selectedIsoElectronsWP80
     * process.selectedElectronsWP95)
                                       
    # configure pat::Jet production
    # (enable L2L3Residual corrections in case running on Data)
    jetCorrections = [ 'L1FastJet', 'L2Relative', 'L3Absolute' ]
    if not isMC:
        jetCorrections.append('L2L3Residual')
    
    switchJetCollection(
        process,
        cms.InputTag('ak5PFJets'),
        doJTA = True,
        doBTagging = False,
        jetCorrLabel = ( 'AK5PF', cms.vstring(jetCorrections) ),
        doType1MET = False,
        doJetID = True,
        jetIdLabel = "ak5",
        outputModule = ''
    )

    # configure pat::MET production
    process.load("PhysicsTools.PatUtils.patPFMETCorrections_cff")
    process.tightWenuSelectionSequence += process.kt6PFJets
    process.tightWenuSelectionSequence += process.ak5PFJets
    process.tightWenuSelectionSequence += process.patDefaultSequence
    
    doSmearJets = None
    if isMC:
        doSmearJets = True
    else:
        doSmearJets = False
    runMEtUncertainties(
        process,
        electronCollection = cms.InputTag('selectedIsoElectronsWP80'),
        photonCollection = '',
        muonCollection = '',
        tauCollection = '',
        jetCollection = cms.InputTag('patJets'),
        doSmearJets = doSmearJets,
        addToPatDefaultSequence = False
    )

    if isMC:
        process.patPFMet.addGenMET = cms.bool(True)
        process.patPFJetMETtype1p2Corr.jetCorrLabel = cms.string("L3Absolute")
    
        process.tightWenuSelectionSequence += process.metUncertaintySequence
    else:
        process.patPFMet.addGenMET = cms.bool(False)
        process.patPFJetMETtype1p2Corr.jetCorrLabel = cms.string("L2L3Residual")
    
        process.tightWenuSelectionSequence += process.patJetsNotOverlappingWithLeptonsForMEtUncertainty
        process.tightWenuSelectionSequence += process.producePatPFMETCorrections

    # Apply event selection cuts
    process.isoElectronWP80Filter = cms.EDFilter("CandViewCountFilter",
        src = cms.InputTag("selectedIsoElectronsWP80"),
        minNumber = cms.uint32(1)
    )
    process.tightWenuSelectionSequence += process.isoElectronWP80Filter

    process.diElectronVeto = cms.EDFilter("PATCandViewMaxFilter",
        src = cms.InputTag("selectedElectronsWP95"),
        maxNumber = cms.uint32(1)
    )
    process.tightWenuSelectionSequence += process.diElectronVeto
    
    process.WenuCandidates = cms.EDProducer("PATElecNuPairProducer",
        srcVisDecayProducts = cms.InputTag('selectedIsoElectronsWP80'),
        srcMET = cms.InputTag('patType1CorrectedPFMet'),
        verbosity = cms.untracked.int32(0)
    )
    process.tightWenuSelectionSequence += process.WenuCandidates

    process.selectedWenuCandidates = cms.EDFilter("PATElecNuPairSelector",
        src = cms.InputTag("WenuCandidates"),
        cut = cms.string('mt > 50.'),
        filter = cms.bool(False)
    )                                                    
    process.tightWenuSelectionSequence += process.selectedWenuCandidates

    process.tightWenuFilter = cms.EDFilter("CandViewCountFilter",
        src = cms.InputTag("selectedWenuCandidates"),
        minNumber = cms.uint32(1)
    )
    process.tightWenuSelectionSequence += process.tightWenuFilter
