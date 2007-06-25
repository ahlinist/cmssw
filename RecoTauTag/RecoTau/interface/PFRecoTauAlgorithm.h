#ifndef PFRecoTauAlgorithm_H
#define PFRecoTauAlgorithm_H

#include "DataFormats/BTauReco/interface/PFIsolatedTauTagInfo.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/TauReco/interface/Tau.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

using namespace std;
using namespace reco;
using namespace edm;
class  PFRecoTauAlgorithm  {
 public:
  PFRecoTauAlgorithm(){}  
  PFRecoTauAlgorithm(const ParameterSet& parameters){
    LeadCand_minPt_                     = parameters.getParameter<double>("LeadCand_minPt"); 
    MatchingConeSize_                   = parameters.getParameter<double>("MatchingConeSize");
    //
    TrackerSignalConeSize_              = parameters.getParameter<double>("TrackerSignalConeSize");
    UseTrackerSignalConeVariableSize_          = parameters.getParameter<bool>("UseTrackerSignalConeVariableSize"); 
    TrackerSignalConeVariableSize_Parameter_   = parameters.getParameter<double>("TrackerSignalConeVariableSize_Parameter");
    TrackerSignalConeVariableSize_max_         = parameters.getParameter<double>("TrackerSignalConeVariableSize_max");
    TrackerSignalConeVariableSize_min_         = parameters.getParameter<double>("TrackerSignalConeVariableSize_min");
    TrackerIsolConeSize_                = parameters.getParameter<double>("TrackerIsolConeSize"); 
    //
    ECALSignalConeSize_                 = parameters.getParameter<double>("ECALSignalConeSize");    
    ECALIsolConeSize_                   = parameters.getParameter<double>("ECALIsolConeSize");      
  }
  ~PFRecoTauAlgorithm(){}
  Tau tag(const PFIsolatedTauTagInfo& myTagInfo); 
 private:

  double LeadCand_minPt_; 
  double MatchingConeSize_;
  double TrackerSignalConeSize_;
  bool UseTrackerSignalConeVariableSize_; 
  double TrackerSignalConeVariableSize_Parameter_;
  double TrackerSignalConeVariableSize_max_;
  double TrackerSignalConeVariableSize_min_; 
  double TrackerIsolConeSize_; 
  //
  double ECALSignalConeSize_;  
  double ECALIsolConeSize_;  
};
#endif 

