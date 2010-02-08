import FWCore.ParameterSet.Config as cms
import copy

# define configuration parameters for submission of Z --> e + mu jobs to CERN batch system
# (running over skimmed samples stored on CASTOR)

intLumiZtoElecMu_Data_10TeV = float(200.)

#--------------------------------------------------------------------------------
# Z --> tau+ tau- sample
#
intLumiZtoElecMu_Ztautau_10TeV = float(1063.9)
corrFactorZtoElecMu_Ztautau_10TeV = float(1.)

fileNamesZtoElecMu_Ztautau_10TeV_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_10TeV/elecMuSkim_1.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_10TeV/elecMuSkim_2.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_10TeV/elecMuSkim_3.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_10TeV/elecMuSkim_4.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_10TeV/elecMuSkim_5.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_10TeV/elecMuSkim_6.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_10TeV/elecMuSkim_7.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_10TeV/elecMuSkim_8.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_10TeV/elecMuSkim_9.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_10TeV/elecMuSkim_10.root'
)
    
fileNamesZtoElecMu_Ztautau_10TeV_part02 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_10TeV/elecMuSkim_11.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_10TeV/elecMuSkim_12.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_10TeV/elecMuSkim_13.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_10TeV/elecMuSkim_14.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_10TeV/elecMuSkim_15.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_10TeV/elecMuSkim_16.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_10TeV/elecMuSkim_17.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_10TeV/elecMuSkim_18.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_10TeV/elecMuSkim_19.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_10TeV/elecMuSkim_20.root'
)

fileNamesZtoElecMu_Ztautau_10TeV_part03 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_10TeV/elecMuSkim_21.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_10TeV/elecMuSkim_22.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_10TeV/elecMuSkim_23.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_10TeV/elecMuSkim_24.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_10TeV/elecMuSkim_25.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_10TeV/elecMuSkim_26.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_10TeV/elecMuSkim_27.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_10TeV/elecMuSkim_28.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_10TeV/elecMuSkim_29.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_10TeV/elecMuSkim_30.root'
)

fileNamesZtoElecMu_Ztautau_10TeV_part04 = cms.untracked.vstring(  
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_10TeV/elecMuSkim_31.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_10TeV/elecMuSkim_32.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_10TeV/elecMuSkim_33.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_10TeV/elecMuSkim_34.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_10TeV/elecMuSkim_35.root'
)

genPhaseSpaceCutZtoElecMu_Ztautau_10TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

plotsOutputFileNameZtoElecMu_Ztautau_10TeV = cms.string('plotsZtoElecMu_Ztautau_10TeV_partXX.root')
patTupleOutputFileNameZtoElecMu_Ztautau_10TeV = cms.string('patTupleZtoElecMu_Ztautau_10TeV_partXX.root')
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# Z --> mu+ mu- sample
#
intLumiZtoElecMu_Zmumu_10TeV = float(1029.1)
corrFactorZtoElecMu_Zmumu_10TeV = float(1.)

fileNamesZtoElecMu_Zmumu_10TeV_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_1.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_2.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_3.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_4.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_5.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_6.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_7.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_8.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_9.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_10.root'
)

fileNamesZtoElecMu_Zmumu_10TeV_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_11.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_12.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_13.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_14.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_15.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_16.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_17.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_18.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_19.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_20.root'
)

fileNamesZtoElecMu_Zmumu_10TeV_part03 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_21.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_22.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_23.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_24.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_25.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_26.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_27.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_28.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_29.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_30.root'
)

fileNamesZtoElecMu_Zmumu_10TeV_part04 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_31.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_32.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_33.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_34.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_35.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_36.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_37.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_38.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_39.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_40.root'
)

fileNamesZtoElecMu_Zmumu_10TeV_part05 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_41.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_42.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_43.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_44.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_45.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_46.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_47.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_48.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_49.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2mumu_10TeV/elecMuSkim_50.root'
)

genPhaseSpaceCutZtoElecMu_Zmumu_10TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

plotsOutputFileNameZtoElecMu_Zmumu_10TeV = cms.string('plotsZtoElecMu_Zmumu_10TeV_partXX.root')
patTupleOutputFileNameZtoElecMu_Zmumu_10TeV = cms.string('patTupleZtoElecMu_Zmumu_10TeV_partXX.root')
#--------------------------------------------------------------------------------



#--------------------------------------------------------------------------------
# Z + Jets sample

intLumiZtoElecMu_ZplusJets_10TeV = float(259.0)
corrFactorZtoElecMu_ZplusJets_10TeV = float(1.)

fileNamesZtoElecMu_ZplusJets_10TeV_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_1.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_2.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_3.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_4.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_5.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_6.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_7.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_8.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_9.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_10.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_11.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_12.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_13.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_14.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_15.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_16.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_17.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_18.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_19.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_20.root'
)

fileNamesZtoElecMu_ZplusJets_10TeV_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_21.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_22.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_23.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_24.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_25.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_26.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_27.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_28.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_29.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_30.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_31.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_32.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_33.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_34.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_35.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_36.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_37.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_38.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_39.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_40.root'
)

fileNamesZtoElecMu_ZplusJets_10TeV_part03 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_41.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_42.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_43.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_44.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_45.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_46.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_47.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_48.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_49.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_50.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_51.root'
)

intLumiZtoElecMu_ZeePlusJets_10TeV = intLumiZtoElecMu_ZplusJets_10TeV
corrFactorZtoElecMu_ZeePlusJets_10TeV = corrFactorZtoElecMu_ZplusJets_10TeV
fileNamesZtoElecMu_ZeePlusJets_10TeV_part01 = fileNamesZtoElecMu_ZplusJets_10TeV_part01
fileNamesZtoElecMu_ZeePlusJets_10TeV_part02 = fileNamesZtoElecMu_ZplusJets_10TeV_part02
fileNamesZtoElecMu_ZeePlusJets_10TeV_part03 = fileNamesZtoElecMu_ZplusJets_10TeV_part03

patTupleOutputFileNameZtoElecMu_ZeePlusJets_10TeV = cms.untracked.string('patTupleZtoElecMu_ZeePlusJets_10TeV_partXX.root')

plotsOutputFileNameZtoElecMu_ZeePlusJets_10TeV = cms.string('plotsZtoElecMu_ZeePlusJets_10TeV_partXX.root')

genPhaseSpaceCutZtoElecMu_ZeePlusJets_10TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('genElectronsFromZs'),
    minNumber = cms.uint32(2)
)

intLumiZtoElecMu_ZmumuPlusJets_10TeV = intLumiZtoElecMu_ZplusJets_10TeV
corrFactorZtoElecMu_ZmumuPlusJets_10TeV = corrFactorZtoElecMu_ZplusJets_10TeV
fileNamesZtoElecMu_ZmumuPlusJets_10TeV_part01 = fileNamesZtoElecMu_ZplusJets_10TeV_part01
fileNamesZtoElecMu_ZmumuPlusJets_10TeV_part02 = fileNamesZtoElecMu_ZplusJets_10TeV_part02
fileNamesZtoElecMu_ZmumuPlusJets_10TeV_part03 = fileNamesZtoElecMu_ZplusJets_10TeV_part03

patTupleOutputFileNameZtoElecMu_ZmumuPlusJets_10TeV = cms.untracked.string('patTupleZtoElecMu_ZmumuPlusJets_10TeV_partXX.root')

plotsOutputFileNameZtoElecMu_ZmumuPlusJets_10TeV = cms.string('plotsZtoElecMu_ZmumuPlusJets_10TeV_partXX.root')

genPhaseSpaceCutZtoElecMu_ZmumuPlusJets_10TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('genMuonsFromZs'),
    minNumber = cms.uint32(2)
)

intLumiZtoElecMu_ZtautauPlusJets_10TeV = intLumiZtoElecMu_ZplusJets_10TeV
corrFactorZtoElecMu_ZtautauPlusJets_10TeV = corrFactorZtoElecMu_ZplusJets_10TeV
fileNamesZtoElecMu_ZtautauPlusJets_10TeV_part01 = fileNamesZtoElecMu_ZplusJets_10TeV_part01
fileNamesZtoElecMu_ZtautauPlusJets_10TeV_part02 = fileNamesZtoElecMu_ZplusJets_10TeV_part02
fileNamesZtoElecMu_ZtautauPlusJets_10TeV_part03 = fileNamesZtoElecMu_ZplusJets_10TeV_part03

patTupleOutputFileNameZtoElecMu_ZtautauPlusJets_10TeV = cms.untracked.string('patTupleZtoElecMu_ZtautauPlusJets_10TeV_partXX.root')

plotsOutputFileNameZtoElecMu_ZtautauPlusJets_10TeV = cms.string('plotsZtoElecMu_ZtautauPlusJets_10TeV_partXX.root')

genPhaseSpaceCutZtoElecMu_ZtautauPlusJets_10TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('genTausFromZs'),
    minNumber = cms.uint32(2)
)
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# W + jets sample
#
intLumiZtoElecMu_WplusJets_10TeV = float(367.5)
corrFactorZtoElecMu_WplusJets_10TeV = float(1.)

fileNamesZtoElecMu_WplusJets_10TeV_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_1.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_2.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_3.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_4.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_5.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_6.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_7.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_8.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_9.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_10.root'
)

fileNamesZtoElecMu_WplusJets_10TeV_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_11.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_12.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_13.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_14.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_15.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_16.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_17.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_18.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_19.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_20.root'
)

fileNamesZtoElecMu_WplusJets_10TeV_part03 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_21.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_22.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_23.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_24.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_25.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_26.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_27.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_28.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_29.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_30.root'
)

fileNamesZtoElecMu_WplusJets_10TeV_part04 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_31.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_32.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_33.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_34.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_35.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_36.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_37.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_38.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_39.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_40.root'
)

fileNamesZtoElecMu_WplusJets_10TeV_part05 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_41.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_42.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_43.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_44.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_45.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_46.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_47.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_48.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_49.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_50.root',
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/10TeV/ZJets-madgraph/akalinow-SkimElMu_314_pass1/elecMuSkim_51.root'
)

genPhaseSpaceCutZtoElecMu_WplusJets_10TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)   
    
plotsOutputFileNameZtoElecMu_WplusJets_10TeV = cms.string('plotsZtoElecMu_WplusJets_10TeV_partXX.root')
patTupleOutputFileNameZtoElecMu_WplusJets_10TeV = cms.string('patTupleZtoElecMu_WplusJets_10TeV_partXX.root')
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# ttbar + jets sample

intLumiZtoElecMu_TTplusJets_10TeV = float(2181.1)
corrFactorZtoElecMu_TTplusJets_10TeV = float(1.)

fileNamesZtoElecMu_TTplusJets_10TeV_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_1.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_2.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_3.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_4.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_5.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_6.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_7.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_8.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_9.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_10.root'
)

fileNamesZtoElecMu_TTplusJets_10TeV_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_11.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_12.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_13.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_14.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_15.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_16.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_17.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_18.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_19.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_20.root'
)

fileNamesZtoElecMu_TTplusJets_10TeV_part03 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_21.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_22.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_23.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_24.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_25.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_26.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_27.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_28.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_29.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_30.root'
)

fileNamesZtoElecMu_TTplusJets_10TeV_part04 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_31.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_32.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_33.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_34.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_35.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_36.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_37.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_38.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_39.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_40.root'
)

fileNamesZtoElecMu_TTplusJets_10TeV_part05 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_41.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_42.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_43.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_44.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_45.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_46.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_47.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_48.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_49.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_50.root'
)

fileNamesZtoElecMu_TTplusJets_10TeV_part06 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_51.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_52.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_53.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_54.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_55.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_56.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_57.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_58.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_59.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_60.root'
)

fileNamesZtoElecMu_TTplusJets_10TeV_part07 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_61.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_62.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_63.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_64.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_65.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_66.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_67.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_68.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_69.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_70.root'
)

fileNamesZtoElecMu_TTplusJets_10TeV_part08 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_71.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_72.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_73.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_74.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_75.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_76.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_77.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_78.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_79.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_80.root'
)

fileNamesZtoElecMu_TTplusJets_10TeV_part09 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_81.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_82.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_83.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_84.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_85.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_86.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_87.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_88.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_89.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_90.root'
)

fileNamesZtoElecMu_TTplusJets_10TeV_part10 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_91.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_92.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_93.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_94.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_95.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_96.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_97.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_98.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_99.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_100.root'
)

fileNamesZtoElecMu_TTplusJets_10TeV_part11 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_101.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_102.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_103.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_104.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_105.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_106.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_107.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_108.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_109.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_110.root'
)

fileNamesZtoElecMu_TTplusJets_10TeV_part12 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_111.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_112.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_113.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_114.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_115.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_116.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_117.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_118.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_119.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_120.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_121.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_122.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_10TeV/elecMuSkim_123.root'
)

genPhaseSpaceCutZtoElecMu_TTplusJets_10TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

plotsOutputFileNameZtoElecMu_TTplusJets_10TeV = cms.string('plotsZtoElecMu_TTplusJets_10TeV_partXX.root')
patTupleOutputFileNameZtoElecMu_TTplusJets_10TeV = cms.string('patTupleZtoElecMu_TTplusJets_10TeV_partXX.root')
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# muon enriched QCD sample generated with Pythia (no cut on Pt(hat))

intLumiZtoElecMu_InclusivePPmuX_10TeV = float(0.0851)
corrFactorZtoElecMu_InclusivePPmuX_10TeV = float(1.)

fileNamesZtoElecMu_InclusivePPmuX_10TeV_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_10TeV/elecMuSkim_1.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_10TeV/elecMuSkim_2.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_10TeV/elecMuSkim_3.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_10TeV/elecMuSkim_4.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_10TeV/elecMuSkim_5.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_10TeV/elecMuSkim_6.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_10TeV/elecMuSkim_7.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_10TeV/elecMuSkim_8.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_10TeV/elecMuSkim_9.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_10TeV/elecMuSkim_10.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_10TeV/elecMuSkim_11.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_10TeV/elecMuSkim_12.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_10TeV/elecMuSkim_13.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_10TeV/elecMuSkim_14.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_10TeV/elecMuSkim_15.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_10TeV/elecMuSkim_16.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_10TeV/elecMuSkim_17.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_10TeV/elecMuSkim_18.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_10TeV/elecMuSkim_19.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_10TeV/elecMuSkim_20.root'
)

fileNamesZtoElecMu_InclusivePPmuX_10TeV_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_10TeV/elecMuSkim_21.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_10TeV/elecMuSkim_22.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_10TeV/elecMuSkim_23.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_10TeV/elecMuSkim_24.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_10TeV/elecMuSkim_25.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_10TeV/elecMuSkim_26.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_10TeV/elecMuSkim_27.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_10TeV/elecMuSkim_28.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_10TeV/elecMuSkim_29.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_10TeV/elecMuSkim_30.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_10TeV/elecMuSkim_31.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_10TeV/elecMuSkim_32.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_10TeV/elecMuSkim_33.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_10TeV/elecMuSkim_34.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_10TeV/elecMuSkim_35.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_10TeV/elecMuSkim_36.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_10TeV/elecMuSkim_37.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_10TeV/elecMuSkim_38.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_10TeV/elecMuSkim_39.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_10TeV/elecMuSkim_40.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_10TeV/elecMuSkim_41.root'
)

genPhaseSpaceCutZtoElecMu_InclusivePPmuX_10TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('ptHat < 20. | leadingGenMuon.pt < 15.')
)

plotsOutputFileNameZtoElecMu_InclusivePPmuX_10TeV = cms.string('plotsZtoElecMu_InclusivePPmuX_10TeV_partXX.root')
patTupleOutputFileNameZtoElecMu_InclusivePPmuX_10TeV = cms.untracked.string('patTupleZtoElecMu_InclusivePPmuX_10TeV_partXX.root')
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# muon enriched QCD sample (Pt(hat) > 20 GeV && muon with Pt > 15 GeV)
#
# NOTE: the submission of the PPmuXptGt20 sample is split into four parts,
#       in order to reduce the execution time of individual cmsRun jobs
#       (and also because the length of vstrings is limited to 255 entries)
#
intLumiZtoElecMu_PPmuXptGt20_10TeV = float(44.8)
corrFactorZtoElecMu_PPmuXptGt20_10TeV = float(1.)

fileNamesZtoElecMu_PPmuXptGt20_10TeV_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_1.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_2.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_3.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_4.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_5.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_6.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_7.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_8.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_9.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_10.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_11.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_12.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_13.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_14.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_15.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_16.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_17.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_18.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_19.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_20.root'
)

fileNamesZtoElecMu_PPmuXptGt20_10TeV_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_21.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_22.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_23.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_24.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_25.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_26.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_27.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_28.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_29.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_30.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_31.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_32.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_33.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_34.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_35.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_36.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_37.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_38.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_39.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_40.root'
)

fileNamesZtoElecMu_PPmuXptGt20_10TeV_part03 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_41.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_42.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_43.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_44.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_45.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_46.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_47.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_48.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_49.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_50.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_51.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_52.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_53.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_54.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_55.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_56.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_57.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_58.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_59.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_60.root'
)

fileNamesZtoElecMu_PPmuXptGt20_10TeV_part04 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_61.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_62.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_63.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_64.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_65.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_66.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_67.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_68.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_69.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_70.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_71.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_72.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_73.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_74.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_75.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_76.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_77.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_78.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_79.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_80.root'
)

fileNamesZtoElecMu_PPmuXptGt20_10TeV_part05 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_81.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_82.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_83.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_84.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_85.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_86.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_87.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_88.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_89.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_90.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_91.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_92.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_93.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_94.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_95.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_96.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_97.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_98.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_99.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_100.root'
)

fileNamesZtoElecMu_PPmuXptGt20_10TeV_part06 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_101.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_102.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_103.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_104.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_105.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_106.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_107.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_108.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_109.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_110.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_111.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_112.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_113.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_114.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_115.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_116.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_117.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_118.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_119.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_120.root'
)

fileNamesZtoElecMu_PPmuXptGt20_10TeV_part07 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_121.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_122.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_123.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_124.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_125.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_126.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_127.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_128.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_129.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_130.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_131.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_132.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_133.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_134.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_135.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_136.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_137.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_138.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_139.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_140.root'
)

fileNamesZtoElecMu_PPmuXptGt20_10TeV_part08 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_141.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_142.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_143.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_144.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_145.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_146.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_147.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_148.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_149.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_150.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_151.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_152.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_153.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_154.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_155.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_156.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_157.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_158.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_159.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_160.root'
)

fileNamesZtoElecMu_PPmuXptGt20_10TeV_part09 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_161.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_162.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_163.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_164.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_165.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_166.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_167.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_168.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_169.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_170.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_171.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_172.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_173.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_174.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_175.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_176.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_177.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_178.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_179.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_180.root'
)

fileNamesZtoElecMu_PPmuXptGt20_10TeV_part10 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_181.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_182.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_183.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_184.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_185.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_186.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_187.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_188.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_189.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_190.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_191.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_192.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_193.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_194.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_195.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_196.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_197.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_198.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_199.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_200.root'
)

fileNamesZtoElecMu_PPmuXptGt20_10TeV_part11 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_201.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_202.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_203.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_204.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_205.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_206.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_207.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_208.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_209.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_210.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_211.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_212.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_213.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_214.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_215.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_216.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_217.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_218.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_219.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_220.root'
)

fileNamesZtoElecMu_PPmuXptGt20_10TeV_part12 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_221.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_222.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_223.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_224.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_225.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_226.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_227.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_228.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_229.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_230.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_231.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_232.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_233.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_234.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_235.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_236.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_237.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_238.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_239.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_240.root'
)

fileNamesZtoElecMu_PPmuXptGt20_10TeV_part13 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_241.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_242.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_243.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_244.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_245.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_246.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_247.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_248.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_249.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_250.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_251.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_252.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_253.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_254.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_255.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_256.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_257.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_258.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_259.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_260.root'
)

fileNamesZtoElecMu_PPmuXptGt20_10TeV_part14 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_261.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_262.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_263.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_264.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_265.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_266.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_267.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_268.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_269.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_270.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_271.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_272.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_273.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_274.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_275.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_276.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_277.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_278.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_279.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_280.root'
)

fileNamesZtoElecMu_PPmuXptGt20_10TeV_part15 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_281.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_282.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_283.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_284.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_285.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_286.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_287.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_288.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_289.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_290.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_291.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_292.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_293.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_294.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_295.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_296.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_297.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_298.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_299.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_300.root'
)

fileNamesZtoElecMu_PPmuXptGt20_10TeV_part16 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_301.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_302.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_303.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_304.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_305.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_306.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_307.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_308.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_309.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_310.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_311.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_312.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_313.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_314.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_315.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_316.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_317.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_318.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_319.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_320.root'
)

fileNamesZtoElecMu_PPmuXptGt20_10TeV_part17 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_321.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_322.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_323.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_324.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_325.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_326.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_327.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_328.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_329.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_330.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_331.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_332.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_333.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_334.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_335.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_336.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_337.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_338.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_339.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_340.root'
)

fileNamesZtoElecMu_PPmuXptGt20_10TeV_part18 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_341.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_342.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_343.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_344.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_345.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_346.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_347.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_348.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_349.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_350.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_351.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_352.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_353.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_354.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_355.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_356.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_357.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_358.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_359.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_360.root'
)

fileNamesZtoElecMu_PPmuXptGt20_10TeV_part19 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_361.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_362.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_363.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_364.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_365.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_366.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_367.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_368.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_369.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_370.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_371.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_372.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_373.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_374.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_375.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_376.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_377.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_378.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_379.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_380.root'
)

fileNamesZtoElecMu_PPmuXptGt20_10TeV_part20 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_381.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_382.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_383.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_384.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_385.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_386.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_387.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_388.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_389.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_390.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_391.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_392.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_393.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_394.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_395.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_396.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_397.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_398.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_399.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_400.root'
)

fileNamesZtoElecMu_PPmuXptGt20_10TeV_part21 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_401.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_402.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_403.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_404.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_405.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_406.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_407.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_408.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_409.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_410.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_411.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_412.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_413.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_414.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_415.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_416.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_417.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_418.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_419.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_420.root'
)

fileNamesZtoElecMu_PPmuXptGt20_10TeV_part22 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_421.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_422.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_423.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_424.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_425.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_426.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_427.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_428.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_429.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_430.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_431.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_432.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_433.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_434.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_435.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_436.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_437.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_438.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_439.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_440.root'
)

fileNamesZtoElecMu_PPmuXptGt20_10TeV_part23 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_441.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_442.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_443.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_444.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_445.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_446.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_447.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_448.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_449.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_450.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_451.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_452.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_453.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_454.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_455.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_456.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_457.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_458.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_459.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_460.root'
)

fileNamesZtoElecMu_PPmuXptGt20_10TeV_part24 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_461.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_462.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_463.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_464.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_465.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_466.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_467.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_468.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_469.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_470.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_471.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_472.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_473.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_474.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_475.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_476.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_477.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_478.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_479.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_480.root'
)

fileNamesZtoElecMu_PPmuXptGt20_10TeV_part25 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_481.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_482.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_483.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_484.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_485.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_486.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_487.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_488.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_489.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_490.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_491.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_492.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_493.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_494.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_495.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/InclusiveMu15_10TeV/elecMuSkim_496.root'
)

genPhaseSpaceCutZtoElecMu_PPmuXptGt20_10TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

plotsOutputFileNameZtoElecMu_PPmuXptGt20_10TeV = cms.string('plotsZtoElecMu_PPmuXptGt20_10TeV_partXX.root')
patTupleOutputFileNameZtoElecMu_PPmuXptGt20_10TeV = cms.string('patTupleZtoElecMu_PPmuXptGt20_10TeV_partXX.root')
#--------------------------------------------------------------------------------
