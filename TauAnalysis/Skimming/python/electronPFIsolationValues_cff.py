import FWCore.ParameterSet.Config as cms

elecPFIsoValueCharged03 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(
        cms.PSet(
            src = cms.InputTag("elecPFIsoDepositCharged"),
            deltaR = cms.double(0.3),
            weight = cms.string('1'),
            vetos = cms.vstring('ConeVeto(0.01)','Threshold(0.0)'),
            skipDefaultVeto = cms.bool(True),
            mode = cms.string('sum')
        )
     )
)

elecPFIsoValueChargedAll03 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(
        cms.PSet(
            src = cms.InputTag("elecPFIsoDepositChargedAll"),
            deltaR = cms.double(0.3),
            weight = cms.string('1'),
            vetos = cms.vstring('ConeVeto(0.01)','Threshold(0.0)'),
            skipDefaultVeto = cms.bool(True),
            mode = cms.string('sum')
        )
    )
)

elecPFIsoValueGamma03 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(
        cms.PSet(
            src = cms.InputTag("elecPFIsoDepositGamma"),
            deltaR = cms.double(0.3),
            weight = cms.string('1'),
            vetos = cms.vstring('ConeVeto(0.01)','Threshold(0.5)'),
            skipDefaultVeto = cms.bool(True),
            mode = cms.string('sum')
        )
    )
)

elecPFIsoValueNeutral03 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(
        cms.PSet(
            src = cms.InputTag("elecPFIsoDepositNeutral"),
            deltaR = cms.double(0.3),
            weight = cms.string('1'),
            vetos = cms.vstring('ConeVeto(0.01)','Threshold(0.5)'),
            skipDefaultVeto = cms.bool(True),
            mode = cms.string('sum')
        )
    )
)

elecPFIsoValuePU03 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(
        cms.PSet(
            src = cms.InputTag("elecPFIsoDepositPU"),
            deltaR = cms.double(0.3),
            weight = cms.string('1'),
            vetos = cms.vstring('ConeVeto(0.01)','Threshold(0.5)'),
            skipDefaultVeto = cms.bool(True),
            mode = cms.string('sum')
        )
    )
)

elecPFIsoValueCharged04 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(
        cms.PSet(
            src = cms.InputTag("elecPFIsoDepositCharged"),
            deltaR = cms.double(0.4),
            weight = cms.string('1'),
            vetos = cms.vstring('ConeVeto(0.01)','Threshold(0.0)'),
            skipDefaultVeto = cms.bool(True),
            mode = cms.string('sum')
        )
    )
)

elecEBPFIsoValueChargedAll04 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(
        cms.PSet(
            src = cms.InputTag("elecPFIsoDepositChargedAll"),
            deltaR = cms.double(0.4),
            weight = cms.string('1'),
            vetos = cms.vstring('ConeVeto(0.01)','Threshold(0.0)'),
            skipDefaultVeto = cms.bool(True),
            mode = cms.string('sum')
        )
    )
)

elecEEPFIsoValueChargedAll04 = elecEBPFIsoValueChargedAll04.clone(
    deposits = cms.VPSet(
        elecEBPFIsoValueChargedAll04.deposits[0].clone(
            vetos = cms.vstring('ConeVeto(0.015)','Threshold(0.5)')
        )
    )
)
elecEBPFIsoValueGamma04 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(
        cms.PSet(
            src = cms.InputTag("elecPFIsoDepositGamma"),
            deltaR = cms.double(0.4),
            weight = cms.string('1'),
            vetos = cms.vstring('ConeVeto(0.01)','Threshold(0.5)'),
            skipDefaultVeto = cms.bool(True),
            mode = cms.string('sum')
        )
    )
)

elecEEPFIsoValueGamma04 = elecEBPFIsoValueGamma04.clone(
    deposits = cms.VPSet(
        elecEBPFIsoValueGamma04.deposits[0].clone(
            vetos = cms.vstring('ConeVeto(0.08)','Threshold(0.5)')
        )
    )
)

elecPFIsoValueNeutral04 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(
        cms.PSet(
            src = cms.InputTag("elecPFIsoDepositNeutral"),
            deltaR = cms.double(0.4),
            weight = cms.string('1'),
            vetos = cms.vstring('ConeVeto(0.01)','Threshold(0.5)'),
            skipDefaultVeto = cms.bool(True),
            mode = cms.string('sum')
        )
    )
)

elecPFIsoValuePU04 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(
        cms.PSet(
            src = cms.InputTag("elecPFIsoDepositPU"),
            deltaR = cms.double(0.4),
            weight = cms.string('1'),
            vetos = cms.vstring('ConeVeto(0.01)','Threshold(0.5)'),
            skipDefaultVeto = cms.bool(True),
            mode = cms.string('sum')
        ) 
    )
)

electronPFIsolationValuesSequence = cms.Sequence(
    elecPFIsoValueCharged03
   * elecPFIsoValueChargedAll03
   * elecPFIsoValueGamma03
   * elecPFIsoValueNeutral03
   * elecPFIsoValuePU03
   * elecPFIsoValueCharged04 
   * elecEBPFIsoValueChargedAll04 * elecEEPFIsoValueChargedAll04
   * elecEBPFIsoValueGamma04 * elecEEPFIsoValueGamma04
   * elecPFIsoValueNeutral04
   * elecPFIsoValuePU04
)
