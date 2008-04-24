#ifndef RecoEgamma_MaterialConversionModules_ClusterAndHitsProducer_h_
#define RecoEgamma_MaterialConversionModules_ClusterAndHitsProducer_h_

#include <memory>
#include <vector>
#include <map>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "AnalysisDataFormats/Egamma/interface/ClusterAndHits.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHit.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripMatchedRecHit2D.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2D.h"

//


class ClusterAndHitsProducer : public edm::EDProducer 
{
  
 public:
  
  ClusterAndHitsProducer(const edm::ParameterSet& ps);
  
  virtual ~ClusterAndHitsProducer();
  
  virtual void produce(edm::Event&, const edm::EventSetup&);
  
 private:
  bool TestZPosition(double _SeedX, double _SeedY, double _SeedZ,
		     double _VtxZ,
		     double _TestX, double _TestY, double _TestZ,
		     double _TestXXerr, double _TestYYerr
		     );
  int nMaxPrintout_; // max # of printouts
  int nEvt_;         // internal counter of events
  std::string clusterandhitsCollection_;
  std::string hybridsuperclusterCollection_;
  std::string hybridsuperclusterProducer_;
   // input configuration
  edm::InputTag matchedStripRecHitsInputTag_;
  edm::InputTag rphiStripRecHitsInputTag_;
  edm::InputTag stereoStripRecHitsInputTag_;
  edm::InputTag pixelRecHitsInputTag_;
  double cluster_pt_thresh_;
  bool counterExceeded() const { return ((nEvt_ > nMaxPrintout_) || (nMaxPrintout_ < 0));}
  reco::ClusterAndHitsCollection clusterplushits_;

  std::map <DetId, std::vector<SiStripRecHit2D> > rphiMappa_;
  std::vector <DetId> rphidetIdSet_;

  std::map <DetId, std::vector<SiStripCluster> > rphiMappaClus_;

  std::map <DetId, std::vector<SiPixelRecHit> > pixMappa_;
  std::vector <DetId> pixdetIdSet_;
  std::map <DetId, std::vector<SiPixelCluster> > pixMappaClus_;


  std::map <DetId, std::vector<SiStripRecHit2D> > sterMappa_;
  std::vector <DetId> sterdetIdSet_;

  std::map <DetId, std::vector<SiStripMatchedRecHit2D> > matchedMappa_;
  std::vector <DetId> matcheddetIdSet_;

  std::string clusterMatchedRecHitsColl_;
  std::string clusterRPhiRecHitsColl_;
  std::string clusterStereoRecHitsColl_;
  std::string clusterPixelRecHitsColl_;
  std::string siClusterColl_;
  std::string siPixClusterColl_;
};


#endif


