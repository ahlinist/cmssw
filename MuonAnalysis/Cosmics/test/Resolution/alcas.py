################################################################################
# For data.
################################################################################

asMUO10004 = [
    ('frontier://FrontierProd/CMS_COND_31X_ALIGNMENT', {
        'TrackerAlignmentRcd': 'TrackerAlignment_GR10_v1_offline',
        'TrackerAlignmentErrorRcd': 'TrackerAlignmentErrors_GR10_v1_offline',
        'GlobalPositionRcd': 'GlobalAlignment_v2_offline',
        'CSCAlignmentRcd': 'CSCAlignment_2009_v4_offline',
        'DTAlignmentRcd': 'DTAlignment_2009_v4_offline'
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
        'TrackerSurfaceDeformationRcd':
        'TrackerSurfaceDeformations_zero'
        }
     ),
    ]

################################################################################
# Some appropriate combinations of the above.
################################################################################

GlobalTag          = 'GR_R_44_V12::All', []
NewTkNominalMu     = 'GR_R_44_V12::All', NewTk + NominalMu
NewTkNewMu         = 'GR_R_44_V12::All', NewTk + NewMu
NewTkNewMuWithAPEs = 'GR_R_44_V12::All', NewTk + NewMu + NewMuAPEs

MCCosTrgPeakStartV9B = 'START44_V9B::All', MCCosmicTrigger + MCStripsPeakMode
MCCosTrgDecoStartV9B = 'START44_V9B::All', MCCosmicTrigger + MCStripsDecoMode
MCCosTrgPeakIdealAli = 'START44_V9B::All', MCCosmicTrigger + MCStripsPeakMode + MCIdealAli
MCCosTrgPeakMCV9B    = 'MC_44_V9B::All',   MCCosmicTrigger + MCStripsPeakMode
