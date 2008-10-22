import FWCore.ParameterSet.Config as cms
import copy

from TrackingTools.TransientTrack.TransientTrackBuilder_cfi import *
from RecoJets.JetAssociationProducers.ic5PFJetTracksAssociatorAtVertex_cfi import *
#PFTauTagInfo Producer
from RecoTauTag.RecoTau.PFRecoTauTagInfoProducer_cfi import *
from RecoTauTag.RecoTau.PFRecoTauProducer_cfi import *
from RecoTauTag.RecoTau.PFRecoTauDiscriminationByIsolation_cfi import *
from RecoTauTag.RecoTau.PFRecoTauProducer_cfi import *
from RecoTauTag.RecoTau.PFRecoTauDiscriminationByLeadingTrackPtCut_cfi import *
from RecoTauTag.RecoTau.PFRecoTauDiscriminationByLeadingTrackFinding_cfi import *
from RecoTauTag.RecoTau.PFRecoTauDiscriminationAgainstElectron_cfi import *
from RecoTauTag.RecoTau.PFRecoTauDiscriminationAgainstMuon_cfi import *
from ElectroWeakAnalysis.EWKTau.tauLdgDiscrimination_cfi import *
from ElectroWeakAnalysis.EWKTau.tauMassWithElecDiscrimination_cfi import *

### Modules for Standard Tau Id ###

pfRecoTauProducerForETau = copy.deepcopy(pfRecoTauProducer)
### Match ###
pfRecoTauProducerForETau.MatchingConeMetric = "DR"
pfRecoTauProducerForETau.MatchingConeSizeFormula = "0.1"
pfRecoTauProducerForETau.MatchingConeSize_max = 0.1
pfRecoTauProducerForETau.MatchingConeSize_min = 0.0
### Trk Sig ###
pfRecoTauProducerForETau.TrackerSignalConeMetric = "DR"
pfRecoTauProducerForETau.TrackerSignalConeSizeFormula = "5.0/ET"
pfRecoTauProducerForETau.TrackerSignalConeSize_min = 0.07
pfRecoTauProducerForETau.TrackerSignalConeSize_max = 0.15
pfRecoTauProducerForETau.LeadTrack_minPt = 5.0
pfRecoTauProducerForETau.LeadChargedHadrCand_minPt = 5.0
### Trk Iso ###
pfRecoTauProducerForETau.TrackerIsolConeMetric = "DR"
pfRecoTauProducerForETau.TrackerIsolConeSizeFormula = "0.5"
pfRecoTauProducerForETau.TrackerIsolConeSize_max = "1.0"
pfRecoTauProducerForETau.TrackerIsolConeSize_min = "0.0"
pfRecoTauProducerForETau.Track_IsolAnnulus_minNhits = 8
pfRecoTauProducerForETau.ChargedHadrCand_IsolAnnulus_minNhits = 8
pfRecoTauProducerForETau.ChargedHadrCand_minPt = 1.0
pfRecoTauProducerForETau.Track_minPt = 1.0
### Ecl Sig ###
pfRecoTauProducerForETau.ECALSignalConeMetric = "DR"
pfRecoTauProducerForETau.ECALSignalConeSizeFormula = "0.15"
pfRecoTauProducerForETau.ECALSignalConeSize_max = "0.15"
pfRecoTauProducerForETau.ECALSignalConeSize_min = "0.0"
### Ecl Iso ###
pfRecoTauProducerForETau.ECALIsolConeMetric = "DR"   
pfRecoTauProducerForETau.ECALIsolConeSizeFormula = "0.50"
pfRecoTauProducerForETau.ECALIsolConeSize_max = "1.0"
pfRecoTauProducerForETau.ECALIsolConeSize_min = "0.0"
pfRecoTauProducerForETau.GammaCand_minPt = 1.5

### Discriminators ###
pfRecoTauLdgTrkFindForETau = copy.deepcopy(pfRecoTauDiscriminationByLeadingTrackFinding)
pfRecoTauLdgTrkFindForETau.PFTauProducer = 'allLayer0PfTausForETau' #'allLayer0PfTausForETauEffForETauKin'

pfRecoTauLdgTrkPtCutForETau = copy.deepcopy(pfRecoTauDiscriminationByLeadingTrackPtCut)
pfRecoTauLdgTrkPtCutForETau.PFTauProducer = 'allLayer0PfTausForETau' #'allLayer0PfTausForETauEffForETauLdgTrk'
pfRecoTauLdgTrkPtCutForETau.MinPtLeadingTrack = 5.0

pfRecoTauTrkIsoDiscrForETau = copy.deepcopy(pfRecoTauDiscriminationByIsolation)
pfRecoTauTrkIsoDiscrForETau.ApplyDiscriminationByECALIsolation = False
pfRecoTauTrkIsoDiscrForETau.PFTauProducer = 'allLayer0PfTausForETau' #'allLayer0PfTausForETauEffForETauMassWithElec'

pfRecoTauEclIsoDiscrForETau = copy.deepcopy(pfRecoTauDiscriminationByIsolation)          
pfRecoTauEclIsoDiscrForETau.ApplyDiscriminationByTrackerIsolation = False
pfRecoTauEclIsoDiscrForETau.ApplyDiscriminationByECALIsolation = True
pfRecoTauEclIsoDiscrForETau.PFTauProducer = 'allLayer0PfTausForETau' #'allLayer0PfTausForETauEffForETauTrkIsod'

pfRecoTauElecRejDiscrForETau = copy.deepcopy(pfRecoTauDiscriminationAgainstElectron)
pfRecoTauElecRejDiscrForETau.PFTauProducer = 'allLayer0PfTausForETau' #'allLayer0PfTausForETauEffForETauEclIsod'

pfRecoTauMuonRejDiscrForETau = copy.deepcopy(pfRecoTauDiscriminationAgainstElectron)
pfRecoTauMuonRejDiscrForETau.PFTauProducer = 'allLayer0PfTausForETau' 

pfRecoTauIsoDiscrForETau = copy.deepcopy(pfRecoTauDiscriminationByIsolation)
pfRecoTauIsoDiscrForETau.PFTauProducer = 'pfRecoTauProducerForETau'


### Modules For Tau Id in Efficiency measurement ###

pfRecoTauTagInfoProducerForETauEff = copy.deepcopy(pfRecoTauTagInfoProducer)
pfRecoTauTagInfoProducerForETauEff.ChargedHadrCand_tkminPt = 0.3
pfRecoTauProducerForETauEff = copy.deepcopy(pfRecoTauProducer)
pfRecoTauProducerForETauEff.PFTauTagInfoProducer = "pfRecoTauTagInfoProducerForETauEff"
### Match ###    
pfRecoTauProducerForETauEff.MatchingConeMetric = "DR"       
pfRecoTauProducerForETauEff.MatchingConeSizeFormula = "0.1"
pfRecoTauProducerForETauEff.MatchingConeSize_max = 0.1
pfRecoTauProducerForETauEff.MatchingConeSize_min = 0.0
### Trk Sig ###
pfRecoTauProducerForETauEff.TrackerSignalConeMetric = "DR"
pfRecoTauProducerForETauEff.TrackerSignalConeSizeFormula = "0.15"
pfRecoTauProducerForETauEff.TrackerSignalConeSize_min = 0.0
pfRecoTauProducerForETauEff.TrackerSignalConeSize_max = 0.15
pfRecoTauProducerForETauEff.LeadTrack_minPt = 5.0
pfRecoTauProducerForETauEff.LeadChargedHadrCand_minPt = 5.0
### Trk Iso ###
pfRecoTauProducerForETauEff.TrackerIsolConeMetric = "DR"
pfRecoTauProducerForETauEff.TrackerIsolConeSizeFormula = "0.7" 
pfRecoTauProducerForETauEff.TrackerIsolConeSize_max = "1.0"
pfRecoTauProducerForETauEff.TrackerIsolConeSize_min = "0.0"
pfRecoTauProducerForETauEff.Track_IsolAnnulus_minNhits = 8
pfRecoTauProducerForETauEff.ChargedHadrCand_minPt = 0.3
### Ecl Sig ###
pfRecoTauProducerForETauEff.ECALSignalConeMetric = "DR"   
pfRecoTauProducerForETauEff.ECALSignalConeSizeFormula = "0.15"
pfRecoTauProducerForETauEff.ECALSignalConeSize_max = "0.15"
pfRecoTauProducerForETauEff.ECALSignalConeSize_min = "0.0"
### Ecl Iso ###
pfRecoTauProducerForETauEff.ECALIsolConeMetric = "DR"     
pfRecoTauProducerForETauEff.ECALIsolConeSizeFormula = "0.7"
pfRecoTauProducerForETauEff.ECALIsolConeSize_max = "1.0"
pfRecoTauProducerForETauEff.ECALIsolConeSize_min = "0.0"
pfRecoTauProducerForETauEff.GammaCand_minPt = 1.5

### Discriminators ###
pfRecoTauLdgTrkFindForETauEff = copy.deepcopy(pfRecoTauDiscriminationByLeadingTrackFinding)
pfRecoTauLdgTrkFindForETauEff.PFTauProducer = 'allLayer0PfTausForETauEff' #'allLayer0PfTausForETauEffForETauKinEff'

pfRecoTauLdgTrkPtCutForETauEff = copy.deepcopy(pfRecoTauDiscriminationByLeadingTrackPtCut)
pfRecoTauLdgTrkPtCutForETauEff.PFTauProducer = 'allLayer0PfTausForETauEff' #'allLayer0PfTausForETauEffForETauLdgTrkEff'
pfRecoTauLdgTrkPtCutForETauEff.MinPtLeadingTrack = 5.0

pfRecoTauTrkIsoDiscrForETauEff = copy.deepcopy(pfRecoTauDiscriminationByIsolation)          
pfRecoTauTrkIsoDiscrForETauEff.ApplyDiscriminationByECALIsolation = False
pfRecoTauTrkIsoDiscrForETauEff.PFTauProducer = 'allLayer0PfTausForETauEff' #'allLayer0PfTausForETauEffForETauMassWithElecEff'

pfRecoTauEclIsoDiscrForETauEff = copy.deepcopy(pfRecoTauDiscriminationByIsolation)          
pfRecoTauEclIsoDiscrForETauEff.ApplyDiscriminationByTrackerIsolation = False
pfRecoTauEclIsoDiscrForETauEff.ApplyDiscriminationByECALIsolation = True 
pfRecoTauEclIsoDiscrForETauEff.PFTauProducer = 'allLayer0PfTausForETauEff' #'allLayer0PfTausForETauEffForETauTrkIsodEff'

pfRecoTauElecRejDiscrForETauEff = copy.deepcopy(pfRecoTauDiscriminationAgainstElectron)
pfRecoTauElecRejDiscrForETauEff.PFTauProducer = 'allLayer0PfTausForETauEff' #'allLayer0PfTausForETauEffForETauEclIsodEff'

pfRecoTauMuonRejDiscrForETauEff = copy.deepcopy(pfRecoTauDiscriminationAgainstElectron)
pfRecoTauMuonRejDiscrForETauEff.PFTauProducer = 'allLayer0PfTausForETauEff' 

pfRecoTauIsoDiscrForETauEff = copy.deepcopy(pfRecoTauDiscriminationByIsolation)
pfRecoTauIsoDiscrForETauEff.PFTauProducer = 'pfRecoTauProducerForETauEff'

PFTauForETauPreamble = cms.Sequence(
                                     ic5PFJetTracksAssociatorAtVertex
                                     *pfRecoTauTagInfoProducer
                                   )

PFTauForETau = cms.Sequence(
                             pfRecoTauProducerForETau
                           )

PFTauForETauEff = cms.Sequence(
                               pfRecoTauTagInfoProducerForETauEff
                              *pfRecoTauProducerForETauEff
                              )
           

