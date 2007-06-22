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
    UseOnlyChargedHadr_for_LeadCand_    = parameters.getParameter<bool>("UseOnlyChargedHadr_for_LeadCand"); 
    LeadCand_minPt_                     = parameters.getParameter<double>("LeadCand_minPt"); 
    MatchingConeSize_                   = parameters.getParameter<double>("MatchingConeSize");
    //
    ChargedHadrCand_minPt_              = parameters.getParameter<double>("ChargedHadrCand_minPt");
    TrackerSignalConeSize_              = parameters.getParameter<double>("TrackerSignalConeSize");
    UseTrackerSignalConeVariableSize_          = parameters.getParameter<bool>("UseTrackerSignalConeVariableSize"); 
    TrackerSignalConeVariableSize_Parameter_   = parameters.getParameter<double>("TrackerSignalConeVariableSize_Parameter");
    TrackerSignalConeVariableSize_max_         = parameters.getParameter<double>("TrackerSignalConeVariableSize_max");
    TrackerSignalConeVariableSize_min_         = parameters.getParameter<double>("TrackerSignalConeVariableSize_min");
    TrackerIsolConeSize_                = parameters.getParameter<double>("TrackerIsolConeSize"); 
    TrackerIsolRing_Candsn_             = parameters.getParameter<int>("TrackerIsolRing_Candsn");   
    //
    UseGammaCand_                       = parameters.getParameter<bool>("UseGammaCand"); 
    GammaCand_minPt_                    = parameters.getParameter<double>("GammaCand_minPt");       
    ECALSignalConeSize_                 = parameters.getParameter<double>("ECALSignalConeSize");    
    ECALIsolConeSize_                   = parameters.getParameter<double>("ECALIsolConeSize");      
    ECALIsolRing_Candsn_                = parameters.getParameter<int>("ECALIsolRing_Candsn");
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
  bool UseOnlyChargedHadr_for_LeadCand_; 
  double LeadCand_minPt_; 
  double MatchingConeSize_;
  //
  double TrackerSignalConeSize_;
  bool UseTrackerSignalConeVariableSize_; 
  double TrackerSignalConeVariableSize_Parameter_;
  double TrackerSignalConeVariableSize_max_;
  double TrackerSignalConeVariableSize_min_; 
  double TrackerIsolConeSize_; 
  double ChargedHadrCand_minPt_; 
  int TrackerIsolRing_Candsn_;   
  //
  bool UseGammaCand_; 
  double GammaCand_minPt_;
  double ECALSignalConeSize_;  
  double ECALIsolConeSize_;  
  double ECALIsolRing_Candsn_;    
};
#endif 

