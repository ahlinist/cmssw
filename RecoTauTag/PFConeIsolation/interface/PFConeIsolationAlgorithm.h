#ifndef PFConeIsolationAlgorithm_H
#define PFConeIsolationAlgorithm_H

#include "DataFormats/BTauReco/interface/PFIsolatedTauTagInfo.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

using namespace std;
using namespace reco;
using namespace edm;

class  PFConeIsolationAlgorithm  {
 public:
  PFConeIsolationAlgorithm(){}  
  PFConeIsolationAlgorithm(const ParameterSet& parameters){
    // parameters of the considered charged hadr. cand.'s, based on their rec. tk properties :
    ChargedHadrCand_tkminPt_            = parameters.getParameter<double>("ChargedHadrCand_tkminPt");
    ChargedHadrCand_tkminPixelHitsn_    = parameters.getParameter<int>("ChargedHadrCand_tkminPixelHitsn");
    ChargedHadrCand_tkminTrackerHitsn_  = parameters.getParameter<int>("ChargedHadrCand_tkminTrackerHitsn");
    ChargedHadrCand_tkmaxipt_           = parameters.getParameter<double>("ChargedHadrCand_tkmaxipt");
    ChargedHadrCand_tkmaxChi2_          = parameters.getParameter<double>("ChargedHadrCand_tkmaxChi2");
    ChargedHadrCand_tktorefpointDZ_     = parameters.getParameter<double>("ChargedHadrCand_tktorefpointDZ");
    // parameters of the considered neutral hadr. cand.'s, based on their rec. HCAL clus. properties : 
    NeutrHadrCand_HcalclusminE_         = parameters.getParameter<double>("NeutrHadrCand_HcalclusminE");
    // parameters of the considered gamma cand.'s, based on their rec. ECAL clus. properties :
    GammaCand_EcalclusminE_             = parameters.getParameter<double>("GammaCand_EcalclusminE");
    // 
    UsePVconstraint_                    = parameters.getParameter<bool>("UsePVconstraint");
    //
    UseOnlyChargedHadrCand_             = parameters.getParameter<bool>("UseOnlyChargedHadrCand"); 
    Cand_minPt_                         = parameters.getParameter<double>("Cand_minPt");
    MatchingConeSize_                   = parameters.getParameter<double>("MatchingConeSize");
    SignalConeSize_                     = parameters.getParameter<double>("SignalConeSize");
    IsolConeSize_                       = parameters.getParameter<double>("IsolConeSize"); 
    UseOnlyChargedHadr_for_LeadCand_    = parameters.getParameter<bool>("UseOnlyChargedHadr_for_LeadCand"); 
    LeadCand_minPt_                     = parameters.getParameter<double>("LeadCand_minPt"); 
    IsolRing_Candsn_                    = parameters.getParameter<int>("IsolRing_Candsn");   
    UseSignalConeVariableSize_          = parameters.getParameter<bool>("UseSignalConeVariableSize"); 
    SignalConeVariableSize_Parameter_   = parameters.getParameter<double>("SignalConeVariableSize_Parameter");
    SignalConeVariableSize_max_         = parameters.getParameter<double>("SignalConeVariableSize_max");
    SignalConeVariableSize_min_         = parameters.getParameter<double>("SignalConeVariableSize_min");
  }
  ~PFConeIsolationAlgorithm(){}
  PFIsolatedTauTagInfo tag(const GenericJetRef&,const PFCandidateRefVector&,const Vertex&); // access to PFCandidateRefVector which composes the GenericJet through tag(.,.) member function input parameter would be temporary. Access would become trough the GenericJet.
 private:
  // parameters of the considered charged hadr. cand.'s, based on their rec. tk properties :
  double ChargedHadrCand_tkminPt_;
  int ChargedHadrCand_tkminPixelHitsn_;
  int ChargedHadrCand_tkminTrackerHitsn_;
  double ChargedHadrCand_tkmaxipt_;
  double ChargedHadrCand_tkmaxChi2_;
  double ChargedHadrCand_tktorefpointDZ_;
  // parameters of the considered neutral hadr. cand.'s, based on their rec. HCAL clus. properties : 
  double NeutrHadrCand_HcalclusminE_;
  // parameters of the considered gamma cand.'s, based on their rec. ECAL clus. properties :
  double GammaCand_EcalclusminE_;
  // 
  bool UsePVconstraint_;
  //
  bool UseOnlyChargedHadrCand_; 
  double MatchingConeSize_;
  double SignalConeSize_;
  double IsolConeSize_; 
  double Cand_minPt_; 
  bool UseOnlyChargedHadr_for_LeadCand_; 
  double LeadCand_minPt_; 
  int IsolRing_Candsn_;   
  bool UseSignalConeVariableSize_; 
  double SignalConeVariableSize_Parameter_;
  double SignalConeVariableSize_max_;
  double SignalConeVariableSize_min_; 
};
#endif 

