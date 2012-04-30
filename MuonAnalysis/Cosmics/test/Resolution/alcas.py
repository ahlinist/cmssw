################################################################################
# For data.
################################################################################

asMUO10004 = [
    ('frontier://FrontierProd/CMS_COND_31X_ALIGNMENT', {
        'GlobalPositionRcd': 'GlobalAlignment_v2_offline',
        'TrackerAlignmentRcd': 'TrackerAlignment_GR10_v1_offline',
        'TrackerAlignmentErrorRcd': 'TrackerAlignmentErrors_GR10_v1_offline',
        'CSCAlignmentRcd': 'CSCAlignment_2009_v4_offline',
        'CSCAlignmentErrorRcd': 'CSCAlignmentError_2009_v3_offline',
        'DTAlignmentRcd': 'DTAlignment_2009_v4_offline',
        'DTAlignmentErrorRcd': 'DTAlignmentError_2009_v4_offline',
        }
     ),
    ('frontier://FrontierProd/CMS_COND_310X_ALIGN', {
        'TrackerSurfaceDeformationRcd': 'TrackerSurfaceDeformations_zero'
        }
     )
    ]

asMay10 = [
    ('frontier://FrontierProd/CMS_COND_31X_ALIGNMENT', {
        'GlobalPositionRcd': 'GlobalAlignment_v3_offline',
        'TrackerAlignmentRcd': 'TrackerAlignment_GR10_v4_offline',
        'TrackerAlignmentErrorRcd': 'TrackerAlignmentErrors_GR10_v2_offline',
        'CSCAlignmentRcd': 'CSCAlignment_2009_v5_offline',
        'CSCAlignmentErrorRcd': 'CSCAlignmentError_2009_v3_offline',
        'DTAlignmentRcd': 'DTAlignment_2009_v4_offline',
        'DTAlignmentErrorRcd': 'DTAlignmentError_2009_v4_offline',
        }
     ),
    ('frontier://FrontierProd/CMS_COND_310X_ALIGN', {
        'TrackerSurfaceDeformationRcd': 'TrackerSurfaceDeformations_zero'
        }
     )
    ]

as42Prompt = [
    ('frontier://FrontierProd/CMS_COND_31X_ALIGNMENT', {
        'GlobalPositionRcd': 'GlobalAlignment_2009_v2_express',
        'TrackerAlignmentRcd': 'TrackerAlignment_2009_v1_prompt',
        'TrackerAlignmentErrorRcd': 'TrackerAlignmentErr_2009_v2_express',
        'CSCAlignmentRcd': 'CSCAlignment_2009_v2_express',
        'CSCAlignmentErrorRcd': 'CSCAlignmentError_2009_v1_express',
        'DTAlignmentRcd': 'DTAlignment_2009_v1_express',
        'DTAlignmentErrorRcd': 'DTAlignmentError_2009_v1_express',
        }
     ),
    ('frontier://FrontierProd/CMS_COND_310X_ALIGN', {
        'TrackerSurfaceDeformationRcd': 'TrackerSurfaceDeformations_zero'
        }
     )
    ]

NominalMu = [
    ('sqlite_file:Design.db', {
        'DTAlignmentRcd': 'DTAlignmentRcd',
        'CSCAlignmentRcd': 'CSCAlignmentRcd'
        }
     )
    ]

NewTk = [
    ('frontier://FrontierProd/CMS_COND_31X_ALIGNMENT', {
        'TrackerAlignmentRcd': 'TrackerAlignment_GR10_v5_offline',
        'GlobalPositionRcd': 'GlobalAlignment_v4_offline'
        }
     ),
    ('frontier://FrontierProd/CMS_COND_310X_ALIGN', {
        'TrackerSurfaceDeformationRcd': 'TrackerSurfaceDeformations_v1_offline'
        }
     )
    ]

NewMu = [
    ('frontier://FrontierProd/CMS_COND_31X_ALIGNMENT', {
        'DTAlignmentRcd': 'DTAlignment_2009_v5_offline',
        'CSCAlignmentRcd': 'CSCAlignment_2009_v6_offline'
        }
     )
    ]

NewMuAPEs = [
    ('sqlite_file:initialMuonAlignment_DT-Aug11_CSC-Aug12_SetAPE.db', {
        'DTAlignmentErrorRcd': 'DTAlignmentErrorRcd',
        'CSCAlignmentErrorRcd': 'CSCAlignmentErrorRcd'
        }
     )
    ]

From173665 = [
    ('sqlite_file:alignments_FT_R_44_V9_173665.db',
     dict((x, x+'JMT') for x in ['GlobalPositionRcd',
                                 'TrackerAlignmentRcd',
                                 'TrackerAlignmentErrorRcd',
                                 'DTAlignmentRcd',
                                 'DTAlignmentErrorRcd',
                                 'CSCAlignmentRcd',
                                 'CSCAlignmentErrorRcd'])
     )
    ]

################################################################################
# For MC.
################################################################################

MCStripsPeakMode = [
    ('frontier://FrontierProd/CMS_COND_31X_STRIP', {
        'SiStripNoisesRcd': 'SiStripNoise_PeakMode_TickCorr_v2_mc'
        }
     )
    ]

MCStripsDecoMode = [
    ('frontier://FrontierProd/CMS_COND_31X_STRIP', {
        'SiStripNoisesRcd': 'SiStripNoise_DecoMode_TickCorr_v2_mc'
        }
     )
    ]

MCCosmicTrigger = [
    ('frontier://FrontierProd/CMS_COND_31X_L1T', {
        'L1MuCSCTFConfigurationRcd': 'L1MuCSCTFConfiguration_key-70511_mc',
        'L1MuDTTFMasksRcd': 'L1MuDTTFMasks_key-dttf11_RS_080_mc',
        'L1MuDTTFParametersRcd': 'L1MuDTTFParameters_key-dttf11_TSC_08_17_bot_mc',
        'L1RPCBxOrConfigRcd': 'L1RPCBxOrConfig_key-LHC7_BOTTOM_mc',
        'L1RPCConeDefinitionRcd': 'L1RPCConeDefinition_key-LHC7_BOTTOM_mc',
        'L1RPCConfigRcd': 'L1RPCConfig_key-LHC7_BOTTOM_mc',
        'L1RPCHsbConfigRcd': 'L1RPCHsbConfig_key-LHC7_BOTTOM_mc'
        }
     )
    ]

MCIdealAli = [
    ('frontier://FrontierProd/CMS_COND_31X_ALIGNMENT', {
        'CSCAlignmentRcd': 'CSCIdealGeometry310me42_mc',
        'CSCAlignmentErrorRcd': 'CSCIdealGeometryErrors310me42_mc',
        }
     ),
    ('frontier://FrontierProd/CMS_COND_31X_FROM21X', {
        'DTAlignmentRcd': 'DTIdealGeometry200_mc',
        'DTAlignmentErrorRcd': 'DTIdealGeometryErrors200_mc',
        'TrackerAlignmentRcd': 'TrackerIdealGeometry210_mc',
        'TrackerAlignmentErrorRcd': 'TrackerIdealGeometryErrors210_mc'
        }
     ),
    ('frontier://FrontierProd/CMS_COND_310X_ALIGN', {
        'TrackerSurfaceDeformationRcd': 'TrackerSurfaceDeformations_zero'
        }
     ),
    ]

MCIdealAli2 = [
    ('frontier://FrontierProd/CMS_COND_31X_FROM21X', {
        'DTT0Rcd': 't0Fake_20X_Sept15_mc',
        }
     ),
    ('frontier://PromptProd/CMS_COND_31X_DT', {
        'DTTtrigRcd': 'DTTtrig_IDEAL_cosmics_V01_mc'
        }
     ),
    ('frontier://FrontierProd/CMS_COND_31X_RPC', {
        'RPCStripNoisesRcd': 'RPCStripNoises_v1_mc'
        }
     ),
    ('frontier://FrontierProd/CMS_COND_31X_STRIP', {
        'SiStripApvGainRcd': 'SiStripApvGain_RealisticMCSim_v2_mc',
        }
     )
    ]

MCStart42 = [
    # Only need to change tracker alignment scenario if base globaltag is START44_V9B.
    ('frontier://FrontierProd/CMS_COND_31X_ALIGNMENT', {
        'TrackerAlignmentRcd': 'TrackerAlignment_2010Realistic_mc',
        'TrackerAlignmentErrorRcd': 'TrackerAlignmentErrors_2010Realistic_mc',
        }
     ),
    ('frontier://FrontierProd/CMS_COND_310X_ALIGN', {
        'TrackerSurfaceDeformationRcd': 'TrackerSurfaceDeformations_zero'
        }
     ),
    ]

MCStart44 = [
    ('frontier://FrontierProd/CMS_COND_31X_ALIGNMENT', {
        'TrackerAlignmentRcd': 'TrackerAlignment_2011Realistic_v1_mc',
        }
     ),
    ('frontier://FrontierProd/CMS_COND_310X_ALIGN', {
        'TrackerSurfaceDeformationRcd': 'TrackerSurfaceDeformations_2011Realistic_v1_mc',
        }
     ),
    ]

MCFixedDTTTrig = [
    ('frontier://FrontierPrep/CMS_COND_DT', {
        'DTTtrigRcd': ('DTTtrig_cosmics_mc_prep_V01', 'cosmics'),
        }
     )
    ]

################################################################################
# Some appropriate combinations of the above.
################################################################################

GlobalTag          = 'GR_R_42_V13::All', []
GTMUO10004         = 'GR_R_42_V13::All', asMUO10004
GTMay10            = 'GR_R_42_V13::All', asMay10
GT42Prompt         = 'GR_R_42_V13::All', as42Prompt

NewTkNominalMu     = 'GR_R_44_V12::All', NewTk + NominalMu
NewTkNewMu         = 'GR_R_44_V12::All', NewTk + NewMu
NewTkNewMuWithAPEs = 'GR_R_44_V12::All', NewTk + NewMu + NewMuAPEs

MCCosTrgPeakStart42  = 'START44_V9B::All', MCCosmicTrigger + MCStripsPeakMode + MCStart42 + MCFixedDTTTrig
MCCosTrgPeakStartV9B = 'START44_V9B::All', MCCosmicTrigger + MCStripsPeakMode + MCFixedDTTTrig
MCCosTrgDecoStartV9B = 'START44_V9B::All', MCCosmicTrigger + MCStripsDecoMode + MCFixedDTTTrig
MCCosTrgPeakIdealAli = 'START44_V9B::All', MCCosmicTrigger + MCStripsPeakMode + MCIdealAli + MCFixedDTTTrig
MCCosTrgPeakMCV9B    = 'MC_44_V9B::All',   MCCosmicTrigger + MCStripsPeakMode + MCFixedDTTTrig
MCCosTrgPeakIdealAli2 = 'START44_V9B::All', MCCosmicTrigger + MCStripsPeakMode + MCIdealAli + MCIdealAli2 + MCFixedDTTTrig

MCCosTrgPeakStartV9BwMuAPEs = 'START44_V9B::All', MCCosmicTrigger + MCStripsPeakMode + MCFixedDTTTrig + NewMuAPEs

MCCosMC42Peak           = 'COSMC_42_PEAK::All', []
MCCosMC42PeakFixed      = 'COSMC_42_PEAK::All', MCFixedDTTTrig
MCCosMC42PeakFixedIdeal = 'COSMC_42_PEAK::All', MCFixedDTTTrig + MCIdealAli
MCCosMC42PeakFixedStart44 = 'COSMC_42_PEAK::All', MCFixedDTTTrig + MCStart44

MCCosMC42PeakB = 'COSMC_42_PEAB::All', []
