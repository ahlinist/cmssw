#ifndef CombinedTauTagAlg_H
#define CombinedTauTagAlg_H

#include "TrackingTools/GeomPropagators/interface/AnalyticalPropagator.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"

#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include "RecoVertex/VertexPrimitives/interface/ConvertError.h"
#include "RecoVertex/VertexPrimitives/interface/VertexException.h"
#include "RecoVertex/AdaptiveVertexFit/interface/AdaptiveVertexFitter.h"
#include "RecoVertex/VertexTools/interface/VertexDistance3D.h"

#include "DataFormats/BTauReco/interface/CombinedTauTagInfo.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/BTauReco/interface/JetTracksAssociation.h"
#include "DataFormats/BTauReco/interface/IsolatedTauTagInfo.h"
#include "DataFormats/BTauReco/interface/TaggingVariable.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h" 
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

#include "RecoTauTag/CombinedTauTag/interface/ECALBounds.h"
#include "RecoTauTag/CombinedTauTag/interface/LikelihoodRatio.h"

#include "Math/GenVector/VectorUtil.h"
#include "Math/GenVector/PxPyPzE4D.h"

#include "CLHEP/Vector/LorentzVector.h"

#include <math.h>

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"

using namespace std; 
using namespace edm;
using namespace reco;

typedef Vector3DBase<float,GlobalTag> Global3DVector;
typedef Point3DBase<float,GlobalTag> Global3DPoint;

const double pi=3.14159265358979323846264;
const double chargedpi_mass=0.13957018;      //PDG Particle Physics Booklet, 2004
  
class CombinedTauTagAlg{
public: 
  CombinedTauTagAlg(const ParameterSet& parameters):theTransientTrackBuilder(0),theLikelihoodRatio(0){
    // --- parameters filled in RecoTauTag/CombinedTauTag/data/combinedTauTag.cfi
    // ------ tracker selection
    MinimumTransverseMomentum_                                = parameters.getParameter<double>("MinimumTransverseMomentum");
    MatchingCone_                                             = parameters.getParameter<double>("MatchingCone");
    TracksLeadingTrackCone_                                   = parameters.getParameter<double>("TracksLeadingTrackCone");
    EvolutiveSignalCone_                                      = parameters.getParameter<bool>("EvolutiveSignalCone");
    SignalCone_ifnotEvolutive_                                = parameters.getParameter<double>("SignalCone_ifnotEvolutive");
    SignalConeVariableSize_Parameter_                         = parameters.getParameter<double>("SignalConeVariableSize_Parameter");
    SignalConeVariableSize_max_                               = parameters.getParameter<double>("SignalConeVariableSize_max");
    SignalConeVariableSize_min_                               = parameters.getParameter<double>("SignalConeVariableSize_min");
    IsolationCone_                                            = parameters.getParameter<double>("IsolationCone"); 
    MinimumTransverseMomentumLeadingTrack_                    = parameters.getParameter<double>("MinimumTransverseMomentumLeadingTrack"); 
    MinimumTransverseMomentumLeadingTrack_case1signalTrack_   = parameters.getParameter<double>("MinimumTransverseMomentumLeadingTrack_case1signalTrack"); 
    MinimumTransverseMomentumLeadingTrack_case3signalTracks_  = parameters.getParameter<double>("MinimumTransverseMomentumLeadingTrack_case3signalTracks"); 
    MaximumNumberOfTracksIsolationRing_                       = parameters.getParameter<int>("MaximumNumberOfTracksIsolationRing");
    // ------ ECAL clus. selection
    ECALclusleadtk_conesize_                                  = parameters.getParameter<double>("ECALclusleadtk_conesize");
    ECALclus_min_e_                                           = parameters.getParameter<double>("ECALclus_min_e");
    matchingECALclustrack_deltaR_                             = parameters.getParameter<double>("matchingECALclustrack_deltaR");
    // ------ likelihood function selection
    use_neutralECALclus_number_case1signaltk_                 = parameters.getParameter<bool>("use_neutralECALclus_number_case1signaltk");	
    use_neutralECALclus_radius_case1signaltk_                 = parameters.getParameter<bool>("use_neutralECALclus_radius_case1signaltk");
    use_neutralE_ratio_case1signaltk_                         = parameters.getParameter<bool>("use_neutralE_ratio_case1signaltk");
    use_isolneutrE_o_tkEneutrE_case1signaltk_                 = parameters.getParameter<bool>("use_isolneutrE_o_tkEneutrE_case1signaltk");
    use_tkEt_o_jetEt_case1signaltk_                           = parameters.getParameter<bool>("use_tkEt_o_jetEt_case1signaltk");
    use_leadtk_ipt_significance_case1signaltk_                = parameters.getParameter<bool>("use_leadtk_ipt_significance_case1signaltk");
    use_neutralECALclus_number_case3signaltks_                = parameters.getParameter<bool>("use_neutralECALclus_number_case3signaltks");	
    use_neutralECALclus_radius_case3signaltks_                = parameters.getParameter<bool>("use_neutralECALclus_radius_case3signaltks");
    use_neutralE_ratio_case3signaltks_                        = parameters.getParameter<bool>("use_neutralE_ratio_case3signaltks");
    use_isolneutrE_o_tksEneutrE_case3signaltks_               = parameters.getParameter<bool>("use_isolneutrE_o_tksEneutrE_case3signaltks"); 
    use_tksEt_o_jetEt_case3signaltks_                         = parameters.getParameter<bool>("use_tksEt_o_jetEt_case3signaltks");
    use_signedflightpath_significance_case3signaltks_         = parameters.getParameter<bool>("use_signedflightpath_significance_case3signaltks");
    // ------ 1 pi-prong/e/mu/discrimination parameters
    calotk_conesize_                                          = parameters.getParameter<double>("calotk_conesize");
    electron_selection_1stmin_ECALEt_o_leadtkPt_              = parameters.getParameter<double>("electron_selection_1stmin_ECALEt_o_leadtkPt");
    electron_selection_1stmax_ECALEt_o_leadtkPt_              = parameters.getParameter<double>("electron_selection_1stmax_ECALEt_o_leadtkPt");
    electron_selection_1stmax_HCALEt_o_leadtkPt_              = parameters.getParameter<double>("electron_selection_1stmax_HCALEt_o_leadtkPt");
    electron_selection_2ndmin_ECALEt_o_leadtkPt_              = parameters.getParameter<double>("electron_selection_2ndmin_ECALEt_o_leadtkPt");
    electron_selection_2ndmax_ECALEt_o_leadtkPt_              = parameters.getParameter<double>("electron_selection_2ndmax_ECALEt_o_leadtkPt");
    electron_selection_2ndmax_HCALEt_o_leadtkPt_              = parameters.getParameter<double>("electron_selection_2ndmax_HCALEt_o_leadtkPt");
    muon_selection_max_ECALEt_o_leadtkPt_                     = parameters.getParameter<double>("muon_selection_max_ECALEt_o_leadtkPt");
    muon_selection_max_HCALEt_o_leadtkPt_                     = parameters.getParameter<double>("muon_selection_max_HCALEt_o_leadtkPt");
  }
  CombinedTauTagAlg():theTransientTrackBuilder(0),theLikelihoodRatio(0){
    // ------ tracker selection
    MinimumTransverseMomentum_                                = 1.;
    MatchingCone_                                             = 0.17;
    TracksLeadingTrackCone_                                   = 0.6;
    EvolutiveSignalCone_                                      = true;
    SignalCone_ifnotEvolutive_                                = 0.07;
    SignalConeVariableSize_Parameter_                         = 3.5;
    SignalConeVariableSize_max_                               = 0.09;
    SignalConeVariableSize_min_                               = 0.05;
    IsolationCone_                                            = 0.4;
    MinimumTransverseMomentumLeadingTrack_                    = 2.5;
    MinimumTransverseMomentumLeadingTrack_case1signalTrack_   = 5.;
    MinimumTransverseMomentumLeadingTrack_case3signalTracks_  = 2.5;
    MaximumNumberOfTracksIsolationRing_                       = 0;
    // ------ ECAL clus. selection
    ECALclusleadtk_conesize_                                  = 0.5;
    ECALclus_min_e_                                           = 1.;
    // ------ likelihood function selection
    use_neutralECALclus_number_case1signaltk_                 = true;	
    use_neutralECALclus_radius_case1signaltk_                 = true;
    use_neutralE_ratio_case1signaltk_                         = false;
    use_isolneutrE_o_tkEneutrE_case1signaltk_                 = true;
    use_tkEt_o_jetEt_case1signaltk_                           = false;
    use_leadtk_ipt_significance_case1signaltk_                = true;
    use_neutralECALclus_number_case3signaltks_                = true;	
    use_neutralECALclus_radius_case3signaltks_                = true;
    use_neutralE_ratio_case3signaltks_                        = false;
    use_isolneutrE_o_tksEneutrE_case3signaltks_               = true;
    use_tksEt_o_jetEt_case3signaltks_                         = false;
    use_signedflightpath_significance_case3signaltks_         = true;
    // ------ 1 pi-prong/e/mu/discrimination parameters
    calotk_conesize_                                          = 0.3;
    electron_selection_1stmin_ECALEt_o_leadtkPt_              = 0.8;
    electron_selection_1stmax_ECALEt_o_leadtkPt_              = 2.;
    electron_selection_1stmax_HCALEt_o_leadtkPt_              = 0.05;
    electron_selection_2ndmin_ECALEt_o_leadtkPt_              = 0.85;
    electron_selection_2ndmax_ECALEt_o_leadtkPt_              = 1.25;
    electron_selection_2ndmax_HCALEt_o_leadtkPt_              = 0.17;
    muon_selection_max_ECALEt_o_leadtkPt_                     = 0.1;
    muon_selection_max_HCALEt_o_leadtkPt_                     = 0.4;  
  } 
  ~CombinedTauTagAlg(){
    if (theTransientTrackBuilder!=0) delete theTransientTrackBuilder;
    if (theLikelihoodRatio!=0) delete theLikelihoodRatio;
  };
  pair<JetTag,CombinedTauTagInfo> tag(const IsolatedTauTagInfoRef&,const Vertex&,Event&,const EventSetup&);
  void setLikelihoodRatio(LikelihoodRatio* x){theLikelihoodRatio=x;}
 private:
  void init(const EventSetup&);
  math::XYZPoint recTrackImpactPositiononECAL(Event&,const EventSetup&,TrackRef);
  void AssociateECALcluster_to_track();
  TaggingVariableList taggingvariablesList();
  double rectk_signedipt_significance(const Vertex&,const TrackRef);
  double rectk_signedip3D_significance(const Vertex&,const TrackRef);
  double signedflightpath_significance(const Vertex&);
  double HCALtowersEtSum_around_rectk(const IsolatedTauTagInfoRef&,Event&,const EventSetup&,TrackRef);
  double ECALcellsEtSum_around_rectk(const IsolatedTauTagInfoRef&,Event&,const EventSetup&,TrackRef);
  const TransientTrackBuilder* theTransientTrackBuilder;
  RefVector<TrackCollection> therecTracks;
  // ------ tracker selection parameters:
  double MinimumTransverseMomentum_;
  double MatchingCone_;
  double TracksLeadingTrackCone_;
  bool EvolutiveSignalCone_;
  double SignalCone_ifnotEvolutive_;
  double SignalConeVariableSize_Parameter_;
  double SignalConeVariableSize_max_;
  double SignalConeVariableSize_min_;
  double IsolationCone_;
  double MinimumTransverseMomentumLeadingTrack_;
  double MinimumTransverseMomentumLeadingTrack_case1signalTrack_;
  double MinimumTransverseMomentumLeadingTrack_case3signalTracks_;
  int MaximumNumberOfTracksIsolationRing_;
  // ------ ECAL clus. selection parameters:
  double ECALclusleadtk_conesize_;
  double ECALclus_min_e_;
  double matchingECALclustrack_deltaR_;
  // ------ likelihood function selection
  LikelihoodRatio* theLikelihoodRatio;
  bool use_neutralECALclus_number_case1signaltk_;	
  bool use_neutralECALclus_radius_case1signaltk_;
  bool use_neutralE_ratio_case1signaltk_;
  bool use_isolneutrE_o_tkEneutrE_case1signaltk_;
  bool use_tkEt_o_jetEt_case1signaltk_;
  bool use_leadtk_ipt_significance_case1signaltk_;
  bool use_neutralECALclus_number_case3signaltks_;	
  bool use_neutralECALclus_radius_case3signaltks_;
  bool use_neutralE_ratio_case3signaltks_;
  bool use_isolneutrE_o_tksEneutrE_case3signaltks_;
  bool use_tksEt_o_jetEt_case3signaltks_;
  bool use_signedflightpath_significance_case3signaltks_;
  // ------ 1 pi-prong/e/mu/discrimination
  double calotk_conesize_;
  double electron_selection_1stmin_ECALEt_o_leadtkPt_;
  double electron_selection_1stmax_ECALEt_o_leadtkPt_;
  double electron_selection_1stmax_HCALEt_o_leadtkPt_;
  double electron_selection_2ndmin_ECALEt_o_leadtkPt_;
  double electron_selection_2ndmax_ECALEt_o_leadtkPt_;
  double electron_selection_2ndmax_HCALEt_o_leadtkPt_;
  double muon_selection_max_ECALEt_o_leadtkPt_;
  double muon_selection_max_HCALEt_o_leadtkPt_;
  // ------
  bool passed_leadtk_selection;
  bool passed_tracker_signalcone_selection;
  bool passed_tracker_isolationring_selection;
  bool passed_tracker_selection;
  bool passed_neutrale_selection;
  bool needs_LikelihoodRatio_discrimination;
  bool is_GoodTauCand;
  bool passed_cutelectron;
  bool infact_GoodElectronCand;
  bool passed_cutmuon;  
  bool infact_GoodMuonCand;
  Global3DVector* the_recjet_G3DV;
  HepLorentzVector the_recjet_alternatHepLV;
  double TauCandJet_ref_et;
  vector<HepLorentzVector> chargedpicand_fromtk_HepLV;
  vector<HepLorentzVector> chargedpicand_frompropagtk_HepLV;
  vector<HepLorentzVector> chargedpicand_fromECALclus_HepLV;
  vector<const BasicCluster*> chargedpicand_ECALclus;
  vector<HepLorentzVector> signalchargedpicand_fromtk_HepLV;
  RefVector<TrackCollection> filtered_chargedpicand_tk;
  vector<HepLorentzVector> filtered_chargedpicand_fromtk_HepLV;
  HepLorentzVector filtered_chargedpicand_fromtk_HepLVsum;
  int the_neutralECALclus_number;
  double the_neutralECALclus_radius;
  vector<HepLorentzVector> ECALclus_withmass0_HepLV;
  vector<const BasicCluster*> ECALclus;
  vector<HepLorentzVector> gammacand_fromneutralECALclus_HepLV;
  vector<const BasicCluster*> gammacand_neutralECALclus;
  double the_neutralE_ratio;
  double the_tksEt_o_jetEt;
  double the_neutrE_o_tksEneutrE;
  double the_isolneutrE_o_tksEneutrE;
  double the_leadtk_signedipt_significance; 
  double the_leadtk_signedip3D_significance; 
  double the_signedflightpath_significance;
  double ECALEt_o_leadtkPt;
  double HCALEt_o_leadtkPt;
  bool couldnotobtain_leadtk_signedipt;
  bool couldnotobtain_leadtk_signedip3D;
  bool couldnotproduce_SV;
  bool couldnotobtain_ECALEt_o_leadtkPt;
  bool couldnotobtain_HCALEt_o_leadtkPt;
};
#endif 
