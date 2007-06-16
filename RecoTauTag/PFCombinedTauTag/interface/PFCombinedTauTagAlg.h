#ifndef PFCombinedTauTagAlg_H
#define PFCombinedTauTagAlg_H

#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"

#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include "RecoVertex/VertexPrimitives/interface/ConvertError.h"
#include "RecoVertex/VertexPrimitives/interface/VertexException.h"
#include "RecoVertex/AdaptiveVertexFit/interface/AdaptiveVertexFitter.h"
#include "RecoVertex/VertexTools/interface/VertexDistance3D.h"

#include "DataFormats/BTauReco/interface/PFCombinedTauTagInfo.h"
#include "DataFormats/BTauReco/interface/PFIsolatedTauTagInfo.h"
#include "DataFormats/BTauReco/interface/TaggingVariable.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/GeometryVector/interface/GlobalTag.h"
#include "DataFormats/GeometryVector/interface/Vector3DBase.h"
#include "DataFormats/GeometryVector/interface/Point3DBase.h"

#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/Exception.h" 
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h" 

#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"

#include "RecoBTag/BTagTools/interface/SignedTransverseImpactParameter.h"
#include "RecoBTag/BTagTools/interface/SignedImpactParameter3D.h"

#include "RecoTauTag/PFCombinedTauTag/interface/LikelihoodRatio.h"

#include "Math/GenVector/VectorUtil.h"
#include "Math/GenVector/PxPyPzE4D.h"

#include <math.h>

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"

using namespace std; 
using namespace edm;
using namespace reco;

typedef Vector3DBase<float,GlobalTag> Global3DVector;
typedef Point3DBase<float,GlobalTag> Global3DPoint;

class PFCombinedTauTagAlg{
public: 
  PFCombinedTauTagAlg(const ParameterSet& parameters):TransientTrackBuilder_(0),LikelihoodRatio_(0){
    // --- parameters filled in RecoTauTag/PFCombinedTauTag/data/combinedTauTag.cfi
    // ------ PF charged hadron cand. selection
    UseOnlyChargedHadrCand_                                    = parameters.getParameter<bool>("UseOnlyChargedHadrCand"); 
    ChargedHadrCand_minPt_                                     = parameters.getParameter<double>("ChargedHadrCand_minPt");
    MatchingConeSize_                                          = parameters.getParameter<double>("MatchingConeSize");
    ChargedHadrLeadChargedHadrCand_ConeSize_                   = parameters.getParameter<double>("ChargedHadrLeadChargedHadrCand_ConeSize"); 
    UseSignalConeVariableSize_                                 = parameters.getParameter<bool>("UseSignalConeVariableSize"); 
    SignalConeSize_ifnotvariable_                              = parameters.getParameter<double>("SignalConeSize_ifnotvariable");
    SignalConeVariableSize_Parameter_                          = parameters.getParameter<double>("SignalConeVariableSize_Parameter");
    SignalConeVariableSize_max_                                = parameters.getParameter<double>("SignalConeVariableSize_max");
    SignalConeVariableSize_min_                                = parameters.getParameter<double>("SignalConeVariableSize_min");    
    IsolConeSize_                                              = parameters.getParameter<double>("IsolConeSize"); 
    IsolRing_Candsmaxn_                                        = parameters.getParameter<int>("IsolRing_Candsmaxn");   
    UseOnlyChargedHadr_for_LeadCand_                           = parameters.getParameter<bool>("UseOnlyChargedHadr_for_LeadCand"); 
    LeadChargedHadrCand_minPt_                                 = parameters.getParameter<double>("LeadChargedHadrCand_minPt"); 
    LeadChargedHadrCand_minPt_case1signalChargedHadrCand_      = parameters.getParameter<double>("LeadChargedHadrCand_minPt_case1signalChargedHadrCand"); 
    LeadChargedHadrCand_minPt_case3signalChargedHadrCands_     = parameters.getParameter<double>("LeadChargedHadrCand_minPt_case3signalChargedHadrCands"); 
    // ------ PF gamma cand. selection
    GammaCandLeadChargedHadrCand_ConeSize_                     = parameters.getParameter<double>("GammaCandLeadChargedHadrCand_ConeSize");
    // ------ PF neutral hadron cand. selection
    NeutrHadrCandLeadChargedHadrCand_ConeSize_                 = parameters.getParameter<double>("NeutrHadrCandLeadChargedHadrCand_ConeSize");
    // ------ likelihood function selection
    Use_GammaCands_number_case1signalChargedHadrCand_                = parameters.getParameter<bool>("Use_GammaCands_number_case1signalChargedHadrCand");	
    Use_GammaCands_radius_case1signalChargedHadrCand_                = parameters.getParameter<bool>("Use_GammaCands_radius_case1signalChargedHadrCand");
    Use_GammaCandsE_ratio_case1signalChargedHadrCand_                = parameters.getParameter<bool>("Use_GammaCandsE_ratio_case1signalChargedHadrCand");
    Use_isolGammaCandsE_o_jetalternatE_case1signalChargedHadrCand_   = parameters.getParameter<bool>("Use_isolGammaCandsE_o_jetalternatE_case1signalChargedHadrCand");
    Use_ChargedHadrCandEt_o_jetEt_case1signalChargedHadrCand_        = parameters.getParameter<bool>("Use_ChargedHadrCandEt_o_jetEt_case1signalChargedHadrCand");
    Use_ChargedHadrCand_ipt_significance_case1signalChargedHadrCand_ = parameters.getParameter<bool>("Use_ChargedHadrCand_ipt_significance_case1signalChargedHadrCand");
    Use_GammaCands_number_case3signalChargedHadrCands_               = parameters.getParameter<bool>("Use_GammaCands_number_case3signalChargedHadrCands");	
    Use_GammaCands_radius_case3signalChargedHadrCands_               = parameters.getParameter<bool>("Use_GammaCands_radius_case3signalChargedHadrCands");
    Use_GammaCandsE_ratio_case3signalChargedHadrCands_               = parameters.getParameter<bool>("Use_GammaCandsE_ratio_case3signalChargedHadrCands");
    Use_isolGammaCandsE_o_jetalternatE_case3signalChargedHadrCands_  = parameters.getParameter<bool>("Use_isolGammaCandsE_o_jetalternatE_case3signalChargedHadrCands"); 
    Use_ChargedHadrCandsEt_o_jetEt_case3signalChargedHadrCands_      = parameters.getParameter<bool>("Use_ChargedHadrCandsEt_o_jetEt_case3signalChargedHadrCands");
    Use_signedflightpath_significance_case3signalChargedHadrCands_   = parameters.getParameter<bool>("Use_signedflightpath_significance_case3signalChargedHadrCands");
    // ------ 1 pi-prong/e/mu/discrimination parameters
    CaloHitLeadChargedHadrCand_ConeSize_                       = parameters.getParameter<double>("CaloHitLeadChargedHadrCand_ConeSize");
    electron_selection_1stmin_ECALEt_o_LeadChargedHadrCandPt_  = parameters.getParameter<double>("electron_selection_1stmin_ECALEt_o_LeadChargedHadrCandPt");
    electron_selection_1stmax_ECALEt_o_LeadChargedHadrCandPt_  = parameters.getParameter<double>("electron_selection_1stmax_ECALEt_o_LeadChargedHadrCandPt");
    electron_selection_1stmax_HCALEt_o_LeadChargedHadrCandPt_  = parameters.getParameter<double>("electron_selection_1stmax_HCALEt_o_LeadChargedHadrCandPt");
    electron_selection_2ndmin_ECALEt_o_LeadChargedHadrCandPt_  = parameters.getParameter<double>("electron_selection_2ndmin_ECALEt_o_LeadChargedHadrCandPt");
    electron_selection_2ndmax_ECALEt_o_LeadChargedHadrCandPt_  = parameters.getParameter<double>("electron_selection_2ndmax_ECALEt_o_LeadChargedHadrCandPt");
    electron_selection_2ndmax_HCALEt_o_LeadChargedHadrCandPt_  = parameters.getParameter<double>("electron_selection_2ndmax_HCALEt_o_LeadChargedHadrCandPt");
    muon_selection_max_ECALEt_o_LeadChargedHadrCandPt_         = parameters.getParameter<double>("muon_selection_max_ECALEt_o_LeadChargedHadrCandPt");
    muon_selection_max_HCALEt_o_LeadChargedHadrCandPt_         = parameters.getParameter<double>("muon_selection_max_HCALEt_o_LeadChargedHadrCandPt");
  }
  PFCombinedTauTagAlg():TransientTrackBuilder_(0),LikelihoodRatio_(0){
    // ------ PF charged hadron cand. selection
    UseOnlyChargedHadrCand_                                    = true; 
    ChargedHadrCand_minPt_                                     = 1.5;
    MatchingConeSize_                                          = 0.17;
    ChargedHadrLeadChargedHadrCand_ConeSize_                   = 0.6;
    UseSignalConeVariableSize_                                 = true; 
    SignalConeSize_ifnotvariable_                              = 0.07;
    SignalConeVariableSize_Parameter_                          = 3.5;
    SignalConeVariableSize_max_                                = 0.09;
    SignalConeVariableSize_min_                                = 0.05;    
    IsolConeSize_                                              = 0.4; 
    IsolRing_Candsmaxn_                                        = 0;   
    UseOnlyChargedHadr_for_LeadCand_                           = true; 
    LeadChargedHadrCand_minPt_                                 = 6.; 
    LeadChargedHadrCand_minPt_case1signalChargedHadrCand_      = 6.; 
    LeadChargedHadrCand_minPt_case3signalChargedHadrCands_     = 6.; 
    // ------ PF gamma cand. selection
    GammaCandLeadChargedHadrCand_ConeSize_                     = 0.45;
    // ------ PF neutral hadron cand. selection
    NeutrHadrCandLeadChargedHadrCand_ConeSize_                 = 0.45;
    // ------ likelihood function selection
    Use_GammaCands_number_case1signalChargedHadrCand_                = true;	
    Use_GammaCands_radius_case1signalChargedHadrCand_                = true;
    Use_GammaCandsE_ratio_case1signalChargedHadrCand_                = false;
    Use_isolGammaCandsE_o_jetalternatE_case1signalChargedHadrCand_   = true;
    Use_ChargedHadrCandEt_o_jetEt_case1signalChargedHadrCand_        = false;
    Use_ChargedHadrCand_ipt_significance_case1signalChargedHadrCand_ = true;
    Use_GammaCands_number_case3signalChargedHadrCands_               = true;	
    Use_GammaCands_radius_case3signalChargedHadrCands_               = true;
    Use_GammaCandsE_ratio_case3signalChargedHadrCands_               = false;
    Use_isolGammaCandsE_o_jetalternatE_case3signalChargedHadrCands_  = true;
    Use_ChargedHadrCandsEt_o_jetEt_case3signalChargedHadrCands_      = false;
    Use_signedflightpath_significance_case3signalChargedHadrCands_   = true;
    // ------ 1 pi-prong/e/mu/discrimination parameters
    CaloHitLeadChargedHadrCand_ConeSize_                       = 0.3;
    electron_selection_1stmin_ECALEt_o_LeadChargedHadrCandPt_  = 0.8;
    electron_selection_1stmax_ECALEt_o_LeadChargedHadrCandPt_  = 2.;
    electron_selection_1stmax_HCALEt_o_LeadChargedHadrCandPt_  = 0.05;
    electron_selection_2ndmin_ECALEt_o_LeadChargedHadrCandPt_  = 0.85;
    electron_selection_2ndmax_ECALEt_o_LeadChargedHadrCandPt_  = 1.25;
    electron_selection_2ndmax_HCALEt_o_LeadChargedHadrCandPt_  = 0.17;
    muon_selection_max_ECALEt_o_LeadChargedHadrCandPt_         = 0.1;
    muon_selection_max_HCALEt_o_LeadChargedHadrCandPt_         = 0.4;  
  } 
  ~PFCombinedTauTagAlg(){
    if (TransientTrackBuilder_!=0) delete TransientTrackBuilder_;
    if (LikelihoodRatio_!=0) delete LikelihoodRatio_;
  }
  PFCombinedTauTagInfo tag(const PFIsolatedTauTagInfoRef&,const Vertex&,Event&,const EventSetup&);
  void setLikelihoodRatio(LikelihoodRatio* x){LikelihoodRatio_=x;}
 private:
  void init(const EventSetup&);
  TaggingVariableList taggingvariablesList();
  double ChargedHadrCand_signedipt_significance(const Vertex&,const PFCandidateRef);
  double ChargedHadrCand_signedip3D_significance(const Vertex&,const PFCandidateRef);
  double signedflightpath_significance(const Vertex&);
  double HCALtowersEtSum_around_ChargedHadrCand(const PFIsolatedTauTagInfoRef&,Event&,const EventSetup&,const TrackRef);
  double ECALcellsEtSum_around_ChargedHadrCand(const PFIsolatedTauTagInfoRef&,Event&,const EventSetup&,const TrackRef);
  const TransientTrackBuilder* TransientTrackBuilder_;
  bool UseOnlyChargedHadrCand_; 
  double ChargedHadrCand_minPt_;
  double MatchingConeSize_;
  double ChargedHadrLeadChargedHadrCand_ConeSize_;
  bool UseSignalConeVariableSize_; 
  double SignalConeSize_ifnotvariable_;
  double SignalConeVariableSize_Parameter_;
  double SignalConeVariableSize_max_;
  double SignalConeVariableSize_min_; 
  double IsolConeSize_; 
  double IsolRing_Candsmaxn_; 
  bool UseOnlyChargedHadr_for_LeadCand_; 
  double LeadChargedHadrCand_minPt_; 
  double LeadChargedHadrCand_minPt_case1signalChargedHadrCand_; 
  double LeadChargedHadrCand_minPt_case3signalChargedHadrCands_; 
  // ------ PF gamma cand. selection
  double GammaCandLeadChargedHadrCand_ConeSize_;
  // ------ PF neutral hadron cand. selection
  double NeutrHadrCandLeadChargedHadrCand_ConeSize_;
  // ------ likelihood function selection
  LikelihoodRatio* LikelihoodRatio_;
  bool Use_GammaCands_number_case1signalChargedHadrCand_;	
  bool Use_GammaCands_radius_case1signalChargedHadrCand_;
  bool Use_GammaCandsE_ratio_case1signalChargedHadrCand_;
  bool Use_isolGammaCandsE_o_jetalternatE_case1signalChargedHadrCand_;
  bool Use_ChargedHadrCandEt_o_jetEt_case1signalChargedHadrCand_;
  bool Use_ChargedHadrCand_ipt_significance_case1signalChargedHadrCand_;
  bool Use_GammaCands_number_case3signalChargedHadrCands_;	
  bool Use_GammaCands_radius_case3signalChargedHadrCands_;
  bool Use_GammaCandsE_ratio_case3signalChargedHadrCands_;
  bool Use_isolGammaCandsE_o_jetalternatE_case3signalChargedHadrCands_;
  bool Use_ChargedHadrCandsEt_o_jetEt_case3signalChargedHadrCands_;
  bool Use_signedflightpath_significance_case3signalChargedHadrCands_;
  // ------ 1 pi-prong/e/mu/discrimination
  double CaloHitLeadChargedHadrCand_ConeSize_;
  double electron_selection_1stmin_ECALEt_o_LeadChargedHadrCandPt_;
  double electron_selection_1stmax_ECALEt_o_LeadChargedHadrCandPt_;
  double electron_selection_1stmax_HCALEt_o_LeadChargedHadrCandPt_;
  double electron_selection_2ndmin_ECALEt_o_LeadChargedHadrCandPt_;
  double electron_selection_2ndmax_ECALEt_o_LeadChargedHadrCandPt_;
  double electron_selection_2ndmax_HCALEt_o_LeadChargedHadrCandPt_;
  double muon_selection_max_ECALEt_o_LeadChargedHadrCandPt_;
  double muon_selection_max_HCALEt_o_LeadChargedHadrCandPt_;
  // ------
  bool passed_LeadChargedHadrCand_selection;
  bool passed_ChargedHadrCand_signalcone_selection;
  bool passed_ChargedHadrCand_isolationring_selection;
  bool passed_ChargedHadrCand_selection;
  bool passed_noGammaCand_selection;
  bool needs_LikelihoodRatio_discrimination;
  bool is_GoodTauCand;
  bool passed_cutelectron;
  bool infact_GoodElectronCand;
  bool passed_cutmuon;  
  bool infact_GoodMuonCand;
  Global3DVector* recjet_G3DV_;
  math::XYZTLorentzVector recjet_alternatXYZTLorentzVector_;
  double TauCandJet_ref_et;
  PFCandidateRefVector ChargedHadrCands_;
  PFCandidateRefVector signalChargedHadrCands_;
  PFCandidateRefVector isolChargedHadrCands_;
  PFCandidateRefVector filteredChargedHadrCands_;
  math::XYZTLorentzVector filteredChargedHadrCands_XYZTLorentzVectorsum_;
  int GammaCands_number_;
  double GammaCands_radius_;
  PFCandidateRefVector GammaCands_;
  math::XYZTLorentzVector GammaCands_XYZTLorentzVectorsum_;
  math::XYZTLorentzVector isolGammaCands_XYZTLorentzVectorsum_;
  int NeutrHadrCands_number_;
  double NeutrHadrCands_radius_;
  PFCandidateRefVector NeutrHadrCands_;
  math::XYZTLorentzVector NeutrHadrCands_XYZTLorentzVectorsum_;
  double GammaCands_Eratio_;
  double ChargedHadrCandsEt_o_jetEt_;
  double GammaCandsE_o_jetalternatE_;
  double isolGammaCandsE_o_jetalternatE_;
  double LeadChargedHadrCand_signedipt_significance_; 
  double LeadChargedHadrCand_signedip3D_significance_; 
  double signedflightpath_significance_;
  double ECALEt_o_LeadChargedHadrCandPt_;
  double HCALEt_o_LeadChargedHadrCandPt_;
  bool couldnotobtain_LeadChargedHadrCand_signedipt;
  bool couldnotobtain_LeadChargedHadrCand_signedip3D;
  bool couldnotproduce_SV;
  bool couldnotobtain_ECALEt_o_LeadChargedHadrCandPt;
  bool couldnotobtain_HCALEt_o_LeadChargedHadrCandPt;
};
#endif 
