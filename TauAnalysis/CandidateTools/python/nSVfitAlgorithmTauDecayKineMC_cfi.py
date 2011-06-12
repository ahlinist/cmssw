import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------
# Tau decay Kinematics parametrized as function of deltaR vs. Pt
pset_leg1_pt_dR = cms.PSet(
    wsName = cms.string('ws_prefit'),
    momName = cms.string('leg1Pt'),
    momType = cms.string('pt'),
    sepTimesMomName = cms.string('leg1VisInvisDeltaRLabTimesPt'),
    sepType = cms.string('dR')
)

pset_leg2_pt_dR = cms.PSet(
    wsName = cms.string('ws_prefit'),
    momName = cms.string('leg2Pt'),
    momType = cms.string('pt'),
    sepTimesMomName = cms.string('leg2VisInvisDeltaRLabTimesPt'),
    sepType = cms.string('dR')
)

nSVfitTauDecayLikelihoodMC_pt_dR_all = cms.PSet(
    pluginName = cms.string("nSVfitTauDecayLikelihoodMC"),
    pluginType = cms.string("NSVfitTauDecayLikelihoodMC"),
    decayModeParameters = cms.PSet(
        electron = pset_leg1_pt_dR.clone(
            inputFileName = cms.FileInPath("TauAnalysis/CandidateTools/data/mcTauDecayKine_Electron_Muon_leg1_dR_all_ws_prefit.root"),
            pdfName = cms.string('pdf_Electron_Muon_AllMom_leg1VisInvisDeltaRLab_leg1_dR_all')
        ),
        muon = pset_leg1_pt_dR.clone(
            inputFileName = cms.FileInPath("TauAnalysis/CandidateTools/data/mcTauDecayKine_Electron_Muon_leg1_dR_all_ws_prefit.root"),
            pdfName = cms.string('pdf_Electron_Muon_AllMom_leg1VisInvisDeltaRLab_leg1_dR_all')
        ),
        oneProngZeroPi0s = pset_leg2_pt_dR.clone(
            inputFileName = cms.FileInPath("TauAnalysis/CandidateTools/data/mcTauDecayKine_OneProng0Pi0_leg2_dR_all_ws_prefit.root"),
            pdfName = cms.string('pdf_OneProng0Pi0_AllMom_leg2VisInvisDeltaRLab_leg2_dR_all')
        ),
        oneProngGtZeroPi0s = pset_leg2_pt_dR.clone(
            inputFileName = cms.FileInPath("TauAnalysis/CandidateTools/data/mcTauDecayKine_OneProngGt0Pi0_leg2_dR_all_ws_prefit.root"),
            pdfName = cms.string('pdf_OneProngGt0Pi0_AllMom_leg2VisInvisDeltaRLab_leg2_dR_all')
        ),
        threeProngZeroPi0s = pset_leg2_pt_dR.clone(
            inputFileName = cms.FileInPath("TauAnalysis/CandidateTools/data/mcTauDecayKine_ThreeProng0Pi0_leg2_dR_all_ws_prefit.root"),
            pdfName = cms.string('pdf_ThreeProng0Pi0_AllMom_leg2VisInvisDeltaRLab_leg2_dR_all')
        ),
        other = pset_leg2_pt_dR.clone(
            inputFileName = cms.FileInPath("TauAnalysis/CandidateTools/data/mcTauDecayKine_OneProngGt0Pi0_leg2_dR_all_ws_prefit.root"),
            pdfName = cms.string('pdf_OneProngGt0Pi0_AllMom_leg2VisInvisDeltaRLab_leg2_dR_all')
        )
    ),
    verbosity = cms.int32(0)
)

nSVfitTauDecayLikelihoodMC_pt_dR_selected = copy.deepcopy(nSVfitTauDecayLikelihoodMC_pt_dR_all)
nSVfitTauDecayLikelihoodMC_pt_dR_selected.decayModeParameters.electron.inputFileName = \
  cms.FileInPath("TauAnalysis/CandidateTools/data/mcTauDecayKine_Electron_Muon_leg1_dR_selected_ws_prefit.root")
nSVfitTauDecayLikelihoodMC_pt_dR_selected.decayModeParameters.electron.pdfName = \
  cms.string('pdf_Electron_Muon_AllMom_leg1VisInvisDeltaRLab_leg1_dR_selected1')
nSVfitTauDecayLikelihoodMC_pt_dR_selected.decayModeParameters.muon.inputFileName = \
  cms.FileInPath("TauAnalysis/CandidateTools/data/mcTauDecayKine_Electron_Muon_leg1_dR_selected_ws_prefit.root")
nSVfitTauDecayLikelihoodMC_pt_dR_selected.decayModeParameters.muon.pdfName = \
  cms.string('pdf_Electron_Muon_AllMom_leg1VisInvisDeltaRLab_leg1_dR_selected1')
nSVfitTauDecayLikelihoodMC_pt_dR_selected.decayModeParameters.oneProngZeroPi0s.inputFileName = \
  cms.FileInPath("TauAnalysis/CandidateTools/data/mcTauDecayKine_OneProng0Pi0_leg2_dR_selected_ws_prefit.root")
nSVfitTauDecayLikelihoodMC_pt_dR_selected.decayModeParameters.oneProngZeroPi0s.pdfName = \
  cms.string('pdf_OneProng0Pi0_AllMom_leg2VisInvisDeltaRLab_leg2_dR_selected1')
nSVfitTauDecayLikelihoodMC_pt_dR_selected.decayModeParameters.oneProngGtZeroPi0s.inputFileName = \
  cms.FileInPath("TauAnalysis/CandidateTools/data/mcTauDecayKine_OneProngGt0Pi0_leg2_dR_selected_ws_prefit.root")
nSVfitTauDecayLikelihoodMC_pt_dR_selected.decayModeParameters.oneProngGtZeroPi0s.pdfName = \
  cms.string('pdf_OneProngGt0Pi0_AllMom_leg2VisInvisDeltaRLab_leg2_dR_selected1')
nSVfitTauDecayLikelihoodMC_pt_dR_selected.decayModeParameters.threeProngZeroPi0s.inputFileName = \
  cms.FileInPath("TauAnalysis/CandidateTools/data/mcTauDecayKine_ThreeProng0Pi0_leg2_dR_selected_ws_prefit.root")
nSVfitTauDecayLikelihoodMC_pt_dR_selected.decayModeParameters.threeProngZeroPi0s.pdfName = \
  cms.string('pdf_ThreeProng0Pi0_AllMom_leg2VisInvisDeltaRLab_leg2_dR_selected1')
nSVfitTauDecayLikelihoodMC_pt_dR_selected.decayModeParameters.other.inputFileName = \
  cms.FileInPath("TauAnalysis/CandidateTools/data/mcTauDecayKine_OneProngGt0Pi0_leg2_dR_selected_ws_prefit.root")
nSVfitTauDecayLikelihoodMC_pt_dR_selected.decayModeParameters.other.pdfName = \
  cms.string('pdf_OneProngGt0Pi0_AllMom_leg2VisInvisDeltaRLab_leg2_dR_selected1')
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# Tau decay Kinematics parametrized as function of 3d-angle vs. Energy
pset_leg1_energy_angle = cms.PSet(
    wsName = cms.string('ws_prefit'),
    momName = cms.string('leg1Energy'),
    momType = cms.string('energy'),
    sepTimesMomName = cms.string('leg1VisInvisAngleLabTimesEnergy'),
    sepType = cms.string('angle')
)

pset_leg2_energy_angle = cms.PSet(
    wsName = cms.string('ws_prefit'),
    momName = cms.string('leg2Energy'),
    momType = cms.string('energy'),
    sepTimesMomName = cms.string('leg2VisInvisAngleLabTimesEnergy'),
    sepType = cms.string('angle')
)

nSVfitTauDecayLikelihoodMC_energy_angle_all = cms.PSet(
    pluginName = cms.string("nSVfitTauDecayLikelihoodMC"),
    pluginType = cms.string("NSVfitTauDecayLikelihoodMC"),
    decayModeParameters = cms.PSet(
        electron = pset_leg1_energy_angle.clone(
            inputFileName = cms.FileInPath("TauAnalysis/CandidateTools/data/mcTauDecayKine_Electron_Muon_leg1_angle_all_ws_prefit.root"),
            pdfName = cms.string('pdf_Electron_Muon_AllMom_leg1VisInvisAngleLab_leg1_angle_all')
        ),
        muon = pset_leg1_energy_angle.clone(
            inputFileName = cms.FileInPath("TauAnalysis/CandidateTools/data/mcTauDecayKine_Electron_Muon_leg1_angle_all_ws_prefit.root"),
            pdfName = cms.string('pdf_Electron_Muon_AllMom_leg1VisInvisAngleLab_leg1_angle_all')
        ),
        oneProngZeroPi0s = pset_leg2_energy_angle.clone(
            inputFileName = cms.FileInPath("TauAnalysis/CandidateTools/data/mcTauDecayKine_OneProng0Pi0_leg2_angle_all_ws_prefit.root"),
            pdfName = cms.string('pdf_OneProng0Pi0_AllMom_leg2VisInvisAngleLab_leg2_angle_all')
        ),
        oneProngGtZeroPi0s = pset_leg2_energy_angle.clone(
            inputFileName = cms.FileInPath("TauAnalysis/CandidateTools/data/mcTauDecayKine_OneProngGt0Pi0_leg2_angle_all_ws_prefit.root"),
            pdfName = cms.string('pdf_OneProngGt0Pi0_AllMom_leg2VisInvisAngleLab_leg2_angle_all')
        ),
        threeProngZeroPi0s = pset_leg2_energy_angle.clone(
            inputFileName = cms.FileInPath("TauAnalysis/CandidateTools/data/mcTauDecayKine_ThreeProng0Pi0_leg2_angle_all_ws_prefit.root"),
            pdfName = cms.string('pdf_ThreeProng0Pi0_AllMom_leg2VisInvisAngleLab_leg2_angle_all')
        ),
        other = pset_leg2_energy_angle.clone(
            inputFileName = cms.FileInPath("TauAnalysis/CandidateTools/data/mcTauDecayKine_OneProngGt0Pi0_leg2_angle_all_ws_prefit.root"),
            pdfName = cms.string('pdf_OneProngGt0Pi0_AllMom_leg2VisInvisAngleLab_leg2_angle_all')
        )
    ),
    verbosity = cms.int32(0)
)

nSVfitTauDecayLikelihoodMC_energy_angle_selected = copy.deepcopy(nSVfitTauDecayLikelihoodMC_energy_angle_all)
nSVfitTauDecayLikelihoodMC_energy_angle_selected.decayModeParameters.electron.inputFileName = \
  cms.FileInPath("TauAnalysis/CandidateTools/data/mcTauDecayKine_Electron_Muon_leg1_angle_selected_ws_prefit.root")
nSVfitTauDecayLikelihoodMC_energy_angle_selected.decayModeParameters.electron.pdfName = \
  cms.string('pdf_Electron_Muon_AllMom_leg1VisInvisAngleLab_leg1_angle_selected1')
nSVfitTauDecayLikelihoodMC_energy_angle_selected.decayModeParameters.muon.inputFileName = \
  cms.FileInPath("TauAnalysis/CandidateTools/data/mcTauDecayKine_Electron_Muon_leg1_angle_selected_ws_prefit.root")
nSVfitTauDecayLikelihoodMC_energy_angle_selected.decayModeParameters.muon.pdfName = \
  cms.string('pdf_Electron_Muon_AllMom_leg1VisInvisAngleLab_leg1_angle_selected1')
nSVfitTauDecayLikelihoodMC_energy_angle_selected.decayModeParameters.oneProngZeroPi0s.inputFileName = \
  cms.FileInPath("TauAnalysis/CandidateTools/data/mcTauDecayKine_OneProng0Pi0_leg2_angle_selected_ws_prefit.root")
nSVfitTauDecayLikelihoodMC_energy_angle_selected.decayModeParameters.oneProngZeroPi0s.pdfName = \
  cms.string('pdf_OneProng0Pi0_AllMom_leg2VisInvisAngleLab_leg2_angle_selected1')
nSVfitTauDecayLikelihoodMC_energy_angle_selected.decayModeParameters.oneProngGtZeroPi0s.inputFileName = \
  cms.FileInPath("TauAnalysis/CandidateTools/data/mcTauDecayKine_OneProngGt0Pi0_leg2_angle_selected_ws_prefit.root")
nSVfitTauDecayLikelihoodMC_energy_angle_selected.decayModeParameters.oneProngGtZeroPi0s.pdfName = \
  cms.string('pdf_OneProngGt0Pi0_AllMom_leg2VisInvisAngleLab_leg2_angle_selected1')
nSVfitTauDecayLikelihoodMC_energy_angle_selected.decayModeParameters.threeProngZeroPi0s.inputFileName = \
  cms.FileInPath("TauAnalysis/CandidateTools/data/mcTauDecayKine_ThreeProng0Pi0_leg2_angle_selected_ws_prefit.root")
nSVfitTauDecayLikelihoodMC_energy_angle_selected.decayModeParameters.threeProngZeroPi0s.pdfName = \
  cms.string('pdf_ThreeProng0Pi0_AllMom_leg2VisInvisAngleLab_leg2_angle_selected1')
nSVfitTauDecayLikelihoodMC_energy_angle_selected.decayModeParameters.other.inputFileName = \
  cms.FileInPath("TauAnalysis/CandidateTools/data/mcTauDecayKine_OneProngGt0Pi0_leg2_angle_selected_ws_prefit.root")
nSVfitTauDecayLikelihoodMC_energy_angle_selected.decayModeParameters.other.pdfName = \
  cms.string('pdf_OneProngGt0Pi0_AllMom_leg2VisInvisAngleLab_leg2_angle_selected1')
#--------------------------------------------------------------------------------
