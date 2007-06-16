#include "RecoTauTag/PFConeIsolation/interface/PFConeIsolationAlgorithm.h"

PFIsolatedTauTagInfo PFConeIsolationAlgorithm::tag(const GenericJetRef& theGenericJet,const PFCandidateRefVector& thePFCands,const Vertex& thePV){
  PFIsolatedTauTagInfo resultExtended(thePFCands);
  resultExtended.setgenericjetRef(theGenericJet);

  resultExtended.filterPFChargedHadrCands(ChargedHadrCand_tkminPt_,ChargedHadrCand_tkminPixelHitsn_,ChargedHadrCand_tkminTrackerHitsn_,ChargedHadrCand_tkmaxipt_,ChargedHadrCand_tkmaxChi2_,ChargedHadrCand_tktorefpointDZ_,UsePVconstraint_,thePV.z(),UseOnlyChargedHadr_for_LeadCand_,MatchingConeSize_,LeadCand_minPt_);
  resultExtended.filterPFNeutrHadrCands(NeutrHadrCand_HcalclusminE_);
  resultExtended.filterPFGammaCands(GammaCand_EcalclusminE_);

  double theSignalConeSize=SignalConeSize_;
  double theJet_E=(*theGenericJet).energy();
  if(UseSignalConeVariableSize_){
    theSignalConeSize=min(SignalConeVariableSize_max_,SignalConeVariableSize_Parameter_/theJet_E);
    theSignalConeSize=max(theSignalConeSize,SignalConeVariableSize_min_);   
  }  
  math::XYZVector GenericJet_XYZVector=(*theGenericJet).momentum();   
  
  double discriminator;
  if (UseOnlyChargedHadrCand_) discriminator=resultExtended.discriminatorByIsolPFChargedHadrCandsN(GenericJet_XYZVector,MatchingConeSize_,theSignalConeSize,IsolConeSize_,UseOnlyChargedHadr_for_LeadCand_,LeadCand_minPt_,Cand_minPt_,IsolRing_Candsn_);
  else discriminator=resultExtended.discriminatorByIsolPFCandsN(GenericJet_XYZVector,MatchingConeSize_,theSignalConeSize,IsolConeSize_,UseOnlyChargedHadr_for_LeadCand_,LeadCand_minPt_,Cand_minPt_,IsolRing_Candsn_);
  resultExtended.setdiscriminator(discriminator);
  return resultExtended; 
}


