import FWCore.ParameterSet.Config as cms
import copy

pset_pt_dR = cms.PSet(
    wsName = cms.string('ws_fit'),
    momName = cms.string('leg1Pt'),
    momType = cms.string('pt'),
    sepName = cms.string('sepTimesMom'),
    sepType = cms.string('dR')
)

nSVfitTauDecayLikelihoodMC_pt_dR_all = cms.PSet(
    pluginName = cms.string("nSVfitTauDecayLikelihoodMC"),
    pluginType = cms.string("NSVfitTauDecayLikelihoodMC"),
    decayModeParameters = cms.PSet(
        electron = pset_pt_dR.clone(
            inputFileName = cms.FileInPath("TauAnalysis/CandidateTools/data/mcTauDecayKine_Electron_Muon_leg1_dR_all_ws_fit.root"),
            pdfName = cms.string('pdf_Electron_Muon_AllMom_leg1VisInvisDeltaRLab_leg1_dR_all')
        ),
        muon = pset_pt_dR.clone(
            inputFileName = cms.FileInPath("TauAnalysis/CandidateTools/data/mcTauDecayKine_Electron_Muon_leg1_dR_all_ws_fit.root"),
            pdfName = cms.string('pdf_Electron_Muon_AllMom_leg1VisInvisDeltaRLab_leg1_dR_all')
        ),
        oneProngZeroPi0s = pset_pt_dR.clone(
            inputFileName = cms.FileInPath("TauAnalysis/CandidateTools/data/mcTauDecayKine_Electron_Muon_leg1_dR_all_ws_fit.root"),
            pdfName = cms.string('pdf_Electron_Muon_AllMom_leg1VisInvisDeltaRLab_leg1_dR_all')
        ),
        oneProngGtZeroPi0s = pset_pt_dR.clone(
            inputFileName = cms.FileInPath("TauAnalysis/CandidateTools/data/mcTauDecayKine_Electron_Muon_leg1_dR_all_ws_fit.root"),
            pdfName = cms.string('pdf_Electron_Muon_AllMom_leg1VisInvisDeltaRLab_leg1_dR_all')
        ),
        threeProngZeroPi0s = pset_pt_dR.clone(
            inputFileName = cms.FileInPath("TauAnalysis/CandidateTools/data/mcTauDecayKine_Electron_Muon_leg1_dR_all_ws_fit.root"),
            pdfName = cms.string('pdf_Electron_Muon_AllMom_leg1VisInvisDeltaRLab_leg1_dR_all')
        ),
        other = pset_pt_dR.clone(
            inputFileName = cms.FileInPath("TauAnalysis/CandidateTools/data/mcTauDecayKine_Electron_Muon_leg1_dR_all_ws_fit.root"),
            pdfName = cms.string('pdf_Electron_Muon_AllMom_leg1VisInvisDeltaRLab_leg1_dR_all')
        )
    ),
    verbosity = cms.int32(0)  
)

## nSVfitTauDecayLikelihoodMC_pt_dR_selected = copy.deepcopy(nSVfitTauDecayLikelihoodMC_pt_dR_all)
## nSVfitTauDecayLikelihoodMC_pt_dR_selected.decayModeParameters.electron.pdfName = \
##   'smearedLandau_Electron_Muon_AllPt_leg1VisInvisDeltaRLab_leg1_dR_all'
## nSVfitTauDecayLikelihoodMC_pt_dR_selected.decayModeParameters.muon.pdfName = \
##   ''                                                                            
## nSVfitTauDecayLikelihoodMC_pt_dR_selected.decayModeParameters.oneProngZeroPi0s.pdfName = \
##   ''                                                                                       
## nSVfitTauDecayLikelihoodMC_pt_dR_selected.decayModeParameters.oneProngGtZeroPi0s.pdfName = \
##   ''                                                                                         
## nSVfitTauDecayLikelihoodMC_pt_dR_selected.decayModeParameters.threeProngZeroPi0s.pdfName = \
##   ''                                                                                         
## nSVfitTauDecayLikelihoodMC_pt_dR_selected.decayModeParameters.other.pdfName = \
##   ''                                                                            

## pset_energy_angle = cms.PSet(
##     wsName = cms.string('ws_fit'),
##     momName = cms.string('leg1Energy'),
##     momType = cms.string('energy'),
##     sepName = cms.string('sepTimesMom'),
##     sepType = cms.string('angle')
## )

## nSVfitTauDecayLikelihoodMC_energy_angle_all = nSVfitTauDecayLikelihoodMC_pt_dR_all.clone(
##     decayModeParameters = cms.PSet(
##         electron = pset_energy_angle.clone(
##             inputFileName = cms.FileInPath("TauAnalysis/CandidateTools/data/mcTauDecayKine_Electron_Muon_leg1_energy_all_ws.root"),
##             pdfName = cms.string('smearedLandau_Electron_Muon_AllPt_leg1VisInvisAngleLab_leg1_angle_all')
##         ),
##         muon = pset_energy_angle.clone(
##             inputFileName = cms.string(''),
##             pdfName = cms.string('')
##         ),
##         oneProngZeroPi0s = pset_energy_angle.clone(
##             inputFileName = cms.string(''),
##             pdfName = cms.string('')
##         ),
##         oneProngGtZeroPi0s = pset_energy_angle.clone(
##             inputFileName = cms.string(''),
##             pdfName = cms.string('')
##         ),
##         threeProngZeroPi0s = pset_energy_angle.clone(
##             inputFileName = cms.string(''),
##             pdfName = cms.string('')
##         ),
##         other = pset_energy_angle.clone(
##             inputFileName = cms.string(''),
##             pdfName = cms.string('')
##         )
##     )
## )    
    
## nSVfitTauDecayLikelihoodMC_energy_angle_selected = copy.deepcopy(nSVfitTauDecayLikelihoodMC_energy_angle_all)
## nSVfitTauDecayLikelihoodMC_energy_angle_selected.decayModeParameters.electron.pdfName = \
##   'smearedLandau_Electron_Muon_AllPt_leg1VisInvisAngleLab_leg1_angle_selected'
## nSVfitTauDecayLikelihoodMC_pt_dR_selected.decayModeParameters.muon.pdfName = \
##   ''                                                                            
## nSVfitTauDecayLikelihoodMC_pt_dR_selected.decayModeParameters.oneProngZeroPi0s.pdfName = \
##   ''                                                                                       
## nSVfitTauDecayLikelihoodMC_pt_dR_selected.decayModeParameters.oneProngGtZeroPi0s.pdfName = \
##   ''                                                                                         
## nSVfitTauDecayLikelihoodMC_pt_dR_selected.decayModeParameters.threeProngZeroPi0s.pdfName = \
##   ''                                                                                         
## nSVfitTauDecayLikelihoodMC_pt_dR_selected.decayModeParameters.other.pdfName = \
##   ''      
