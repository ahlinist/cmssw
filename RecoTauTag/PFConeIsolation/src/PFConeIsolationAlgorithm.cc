#include "RecoTauTag/PFConeIsolation/interface/PFConeIsolationAlgorithm.h"

pair<JetTag,PFIsolatedTauTagInfo> PFConeIsolationAlgorithm::tag(const PFJetRef& thePFJet,const PFCandidateRefVector& thePFCands,const Vertex& thePV){
  PFIsolatedTauTagInfo resultExtended(thePFCands);
  resultExtended.setpfjetRef(thePFJet);

  math::XYZTLorentzVector alternatLorentzVect;
  alternatLorentzVect.SetPx(0.);
  alternatLorentzVect.SetPy(0.);
  alternatLorentzVect.SetPz(0.);
  alternatLorentzVect.SetE(0.);
  for (PFCandidateRefVector::const_iterator iGammaCand=resultExtended.PFGammaCands().begin();iGammaCand!=resultExtended.PFGammaCands().end();iGammaCand++) alternatLorentzVect+=(**iGammaCand).p4();
  for (PFCandidateRefVector::const_iterator iChargedHadrCand=resultExtended.PFChargedHadrCands().begin();iChargedHadrCand!=resultExtended.PFChargedHadrCands().end();iChargedHadrCand++) alternatLorentzVect+=(**iChargedHadrCand).p4();  
  resultExtended.setalternatLorentzVect(alternatLorentzVect);

  resultExtended.filterPFChargedHadrCands(ChargedHadrCand_tkminPt_,ChargedHadrCand_tkminPixelHitsn_,ChargedHadrCand_tkminTrackerHitsn_,ChargedHadrCand_tkmaxipt_,ChargedHadrCand_tkmaxChi2_,ChargedHadrCand_tktorefpointDZ_,UsePVconstraint_,thePV.z(),UseOnlyChargedHadr_for_LeadCand_,MatchingConeSize_,LeadCand_minPt_);
  resultExtended.filterPFNeutrHadrCands(NeutrHadrCand_HcalclusminE_);
  resultExtended.filterPFGammaCands(GammaCand_EcalclusminE_);

  double theTrackerSignalConeSize=TrackerSignalConeSize_;
  double theJet_E=(*thePFJet).energy();
  if(UseTrackerSignalConeVariableSize_){
    theTrackerSignalConeSize=min(TrackerSignalConeVariableSize_max_,TrackerSignalConeVariableSize_Parameter_/theJet_E);
    theTrackerSignalConeSize=max(theTrackerSignalConeSize,TrackerSignalConeVariableSize_min_);   
  }  
  math::XYZVector PFJet_XYZVector=(*thePFJet).momentum();   
  
  double trackerdiscriminator=resultExtended.discriminatorByIsolPFChargedHadrCandsN(PFJet_XYZVector,MatchingConeSize_,theTrackerSignalConeSize,TrackerIsolConeSize_,UseOnlyChargedHadr_for_LeadCand_,LeadCand_minPt_,ChargedHadrCand_minPt_,TrackerIsolRing_Candsn_);
  if (trackerdiscriminator==1) resultExtended.setpassedtrackerisolation(true);
  else resultExtended.setpassedtrackerisolation(false);
  double ECALdiscriminator=resultExtended.discriminatorByIsolPFGammaCandsN(PFJet_XYZVector,MatchingConeSize_,ECALSignalConeSize_,ECALIsolConeSize_,UseOnlyChargedHadr_for_LeadCand_,LeadCand_minPt_,GammaCand_minPt_,ECALIsolRing_Candsn_);
  if (ECALdiscriminator==1) resultExtended.setpassedECALisolation(true);
  else resultExtended.setpassedECALisolation(false);

  double discriminator;
  if ((!UseGammaCand_ && trackerdiscriminator==1) || (UseGammaCand_ && trackerdiscriminator==1 && ECALdiscriminator==1)) discriminator=1.;
  else discriminator=0.;
  JetTag resultBase(discriminator);

  return pair<JetTag,PFIsolatedTauTagInfo>(resultBase,resultExtended); 
}


