#ifndef _ElectroWeakAnalysis_MultiBosons_EgammaUserDataProducer_h_
#define _ElectroWeakAnalysis_MultiBosons_EgammaUserDataProducer_h_
// -*- C++ -*-
//
// Package:    PatAlgos
// Class:      EgammaUserDataProducer
//
/**\class EgammaUserDataProducer EgammaUserDataProducer.h ElectroWeakAnalysis/MultiBosons/interface/EgammaUserDataProducer.h

Description: Test module for UserData in PAT

Provided by Pasqualle Musella with probable contributions from
Freya Blekman and Giovanni Petrucciani. Adapted for VGamma by
Jan Veverka. Many thanks to Lindsey Gray for debugging issues
with templates!

Implementation:

*/


// system include files
#include <memory>
#include <string>
#include <vector>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/Common/interface/OwnVector.h"

#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"

#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"
#include "RecoLocalCalo/EcalRecAlgos/interface/EcalSeverityLevelAlgo.h"
#include "CondFormats/DataRecord/interface/EcalChannelStatusRcd.h"

namespace vgamma {
  //
  // class decleration
  //
  // EgammaType is meant to be one of the electron or photon flavors
  // with the superCluster method, e.g. reco::Photon, pat::Electron etc.
  template <typename EgammaType>
  class EgammaUserDataProducer : public edm::EDProducer {
  public:
    explicit EgammaUserDataProducer(const edm::ParameterSet&);
    ~EgammaUserDataProducer();


  private:
    virtual void produce(edm::Event&, const edm::EventSetup&);

    // ----------member data ---------------------------
    template <typename UserDataType>
    void putMap(edm::Event & iEvent,
                edm::Handle<edm::View<EgammaType> >& egammas,
                std::vector<UserDataType>& vUserData,
                const std::string& name);

    edm::InputTag src_;
    edm::InputTag ebRecHits_;
    edm::InputTag eeRecHits_;
  };

  //
  // constructors and destructor
  //
  template <typename EgammaType>
  EgammaUserDataProducer<EgammaType>::EgammaUserDataProducer (
    const edm::ParameterSet& iConfig
    ):
    src_(iConfig.getParameter<edm::InputTag>("src")),
    ebRecHits_(iConfig.getParameter<edm::InputTag>("ebRecHits")),
    eeRecHits_(iConfig.getParameter<edm::InputTag>("eeRecHits"))
  {
    produces<edm::ValueMap<float> >("e1x3");
    produces<edm::ValueMap<float> >("e3x1");
    produces<edm::ValueMap<float> >("e1x5");
    produces<edm::ValueMap<float> >("e2x2");
    produces<edm::ValueMap<float> >("e3x2");
    produces<edm::ValueMap<float> >("e3x3");
    produces<edm::ValueMap<float> >("e4x4");
    produces<edm::ValueMap<float> >("e5x5");

    produces<edm::ValueMap<float> > ("e2x5Right");
    produces<edm::ValueMap<float> > ("e2x5Left");
    produces<edm::ValueMap<float> > ("e2x5Top");
    produces<edm::ValueMap<float> > ("e2x5Bottom");
    produces<edm::ValueMap<float> > ("e2x5Max");

    produces<edm::ValueMap<float> > ("eLeft");
    produces<edm::ValueMap<float> > ("eRight");
    produces<edm::ValueMap<float> > ("eTop");
    produces<edm::ValueMap<float> > ("eBottom");

    produces<edm::ValueMap<float> > ("eMax");
    produces<edm::ValueMap<float> > ("e2nd");

    produces<edm::ValueMap<float> > ("etaLat");
    produces<edm::ValueMap<float> > ("phiLat");
    produces<edm::ValueMap<float> > ("lat");

    produces<edm::ValueMap<float> > ("covEtaEta");
    produces<edm::ValueMap<float> > ("covEtaPhi");
    produces<edm::ValueMap<float> > ("covPhiPhi");

    produces<edm::ValueMap<float> > ("covIEtaIEta");
    produces<edm::ValueMap<float> > ("covIEtaIPhi");
    produces<edm::ValueMap<float> > ("covIPhiIPhi");

    produces<edm::ValueMap<float> > ("zernike20");
    produces<edm::ValueMap<float> > ("zernike42");

    produces<edm::ValueMap<float> > ("seedTime");
    produces<edm::ValueMap<float> > ("seedOutOfTimeChi2");
    produces<edm::ValueMap<float> > ("seedChi2");
    produces<edm::ValueMap<int  > > ("seedRecoFlag");
    produces<edm::ValueMap<int  > > ("seedSeverityLevel");
    produces<edm::ValueMap<float> > ("seedSwissCross");
    produces<edm::ValueMap<float> > ("seedE1OverE9");

    produces<edm::ValueMap<float> > ("scRoundness");
    produces<edm::ValueMap<float> > ("scAngle");

    produces<edm::ValueMap<float> > ("sMin");
    produces<edm::ValueMap<float> > ("sMaj");
    produces<edm::ValueMap<float> > ("alphaSMinMaj");

    produces<edm::ValueMap<float> > ("seedE2overE9");
  }

  template <typename EgammaType>
  EgammaUserDataProducer<EgammaType>::~EgammaUserDataProducer()
  {
  }

  // ------------ method called to for each event  ------------
  template <typename EgammaType>
  void
  EgammaUserDataProducer<EgammaType>::produce (
    edm::Event& iEvent, const edm::EventSetup& iSetup
    )
  {
    using namespace std;
    using namespace edm;

    edm::Handle<View<EgammaType> > egammas;
    edm::Handle<EcalRecHitCollection> eeRecHits;
    edm::Handle<EcalRecHitCollection> ebRecHits;
    edm::ESHandle<EcalChannelStatus> channelStatus;

    iEvent.getByLabel(src_, egammas);
    iEvent.getByLabel(ebRecHits_, ebRecHits);
    iEvent.getByLabel(eeRecHits_, eeRecHits);
    iSetup.get<EcalChannelStatusRcd>().get(channelStatus);


    EcalClusterLazyTools lazyTools(iEvent, iSetup, ebRecHits_, eeRecHits_);

    vector<float> e1x3;
    vector<float> e3x1;
    vector<float> e1x5;
    vector<float> e2x2;
    vector<float> e3x2;
    vector<float> e3x3;
    vector<float> e4x4;
    vector<float> e5x5;

    vector<float> e2x5Right;
    vector<float> e2x5Left;
    vector<float> e2x5Top;
    vector<float> e2x5Bottom;
    vector<float> e2x5Max;

    vector<float> eLeft;
    vector<float> eRight;
    vector<float> eTop;
    vector<float> eBottom;

    vector<float> eMax;
    vector<float> e2nd;

    vector<float> etaLat;
    vector<float> phiLat;
    vector<float> lat;

    vector<float> covEtaEta;
    vector<float> covEtaPhi;
    vector<float> covPhiPhi;

    vector<float> covIEtaIEta;
    vector<float> covIEtaIPhi;
    vector<float> covIPhiIPhi;

    vector<float> zernike20;
    vector<float> zernike42;

    vector<float> seedTime;
    vector<float> seedOutOfTimeChi2;
    vector<float> seedChi2;
    vector<int>   seedRecoFlag;
    vector<int>   seedSeverityLevel;
    vector<float> seedSwissCross;
    vector<float> seedE1OverE9;

    vector<float> scRoundness;
    vector<float> scAngle;

    vector<float> sMin;
    vector<float> sMaj;
    vector<float> alphaSMinMaj;

    vector<float> seedE2overE9;

    typename View<EgammaType>::const_iterator egamma;
    for(egamma = egammas->begin(); egamma < egammas->end(); ++egamma) {
      const reco::CaloCluster &cluster = *( egamma->superCluster()->seed() );

      e1x3       .push_back(lazyTools.e1x3(cluster));
      e3x1       .push_back(lazyTools.e3x1(cluster));
      e1x5       .push_back(lazyTools.e1x5(cluster));
      e2x2       .push_back(lazyTools.e2x2(cluster));
      e3x2       .push_back(lazyTools.e3x2(cluster));
      e3x3       .push_back(lazyTools.e3x3(cluster));
      e4x4       .push_back(lazyTools.e4x4(cluster));
      e5x5       .push_back(lazyTools.e5x5(cluster));

      e2x5Right  .push_back(lazyTools.e2x5Right  (cluster));
      e2x5Left   .push_back(lazyTools.e2x5Left   (cluster));
      e2x5Top    .push_back(lazyTools.e2x5Top    (cluster));
      e2x5Bottom .push_back(lazyTools.e2x5Bottom (cluster));
      e2x5Max    .push_back(lazyTools.e2x5Max    (cluster));

      eLeft      .push_back(lazyTools.eLeft      (cluster));
      eRight     .push_back(lazyTools.eRight     (cluster));
      eTop       .push_back(lazyTools.eTop       (cluster));
      eBottom    .push_back(lazyTools.eBottom    (cluster));

      eMax       .push_back(lazyTools.eMax       (cluster));
      e2nd       .push_back(lazyTools.e2nd       (cluster));

      std::vector<float> lats = lazyTools.lat(cluster);
      etaLat     .push_back(lats[0]);
      phiLat     .push_back(lats[1]);
      lat        .push_back(lats[2]);

      std::vector<float> covs= lazyTools.covariances(cluster);
      covEtaEta  .push_back(covs[0]);
      covEtaPhi  .push_back(covs[1]);
      covPhiPhi  .push_back(covs[2]);

      std::vector<float> lcovs = lazyTools.localCovariances(cluster);
      covIEtaIEta.push_back(lcovs[0]);
      covIEtaIPhi.push_back(lcovs[1]);
      covIPhiIPhi.push_back(lcovs[2]);

      zernike20  .push_back(lazyTools.zernike20  (cluster));
      zernike42  .push_back(lazyTools.zernike42  (cluster));

      // Spike cleaning variables
      float time          = -999.;
      float outOfTimeChi2 = -999.;
      float chi2          = -999.;
      int   recoFlag      = -999;
      float swissCross    = -999.;
      float E1OverE9      = -999.;
      int   severityLevel = -999;
      float E2overE9      = -999.;

      DetId id = lazyTools.getMaximum(cluster).first;
      const EcalRecHitCollection & recHits = ( egamma->isEB() ?
        *ebRecHits :
        *eeRecHits
        );
      EcalRecHitCollection::const_iterator rh = recHits.find(id);
      if (rh != recHits.end()) {
        time          = rh->time();
        outOfTimeChi2 = rh->outOfTimeChi2();
        chi2          = rh->chi2();
        recoFlag      = rh->recoFlag();
        severityLevel = EcalSeverityLevelAlgo::severityLevel(id, recHits,
                                                             *channelStatus);
        swissCross    = EcalSeverityLevelAlgo::swissCross(id, recHits);
        E1OverE9      = EcalSeverityLevelAlgo::E1OverE9(id, recHits);
	E2overE9      = EcalSeverityLevelAlgo::E2overE9(id, recHits);
      } else {
        LogWarning("SpikeCleaningVariables") << "Didn't find seed rechit!" << endl;
      }

      seedTime         .push_back(time);
      seedOutOfTimeChi2.push_back(outOfTimeChi2);
      seedChi2         .push_back(chi2);
      seedRecoFlag     .push_back(recoFlag);
      seedSeverityLevel.push_back(severityLevel);
      seedSwissCross   .push_back(swissCross);
      seedE1OverE9     .push_back(E1OverE9);
      seedE2overE9     .push_back(E2overE9);

      float roundness = -999.;
      float angle = -999.;

      if(egamma->isEB()) {
	std::vector<float> roundnessAndAngle = 
	  EcalClusterTools::roundnessBarrelSuperClusters(*(egamma->superCluster()),*ebRecHits.product(),0);
	roundness = roundnessAndAngle[0];
	angle = roundnessAndAngle[1];
      }

      scRoundness.push_back(roundness);
      scAngle    .push_back(angle);
      
      float smin = -999.;
      float smax = -999.;
      float alphasminmaj = -999.;

      if(egamma->isEB()) {
	Cluster2ndMoments theMoments = EcalClusterTools::cluster2ndMoments(*(egamma->superCluster()),*ebRecHits.product());
	smin = theMoments.sMin;
	smax = theMoments.sMaj;
	alphasminmaj = theMoments.alpha;
      } else if (egamma->isEE()) {
	Cluster2ndMoments theMoments = EcalClusterTools::cluster2ndMoments(*(egamma->superCluster()),*eeRecHits.product());
	smin = theMoments.sMin;
	smax = theMoments.sMaj;
	alphasminmaj = theMoments.alpha;
      }
	
      sMin.push_back(smin);
      sMaj.push_back(smax);
      alphaSMinMaj.push_back(alphasminmaj);
								   

    } // for(egamma = egammas->begin(); egamma < egammas->end(); ++egamma)

    putMap<float>(iEvent,egammas,e1x3,"e1x3");
    putMap<float>(iEvent,egammas,e3x1,"e3x1");
    putMap<float>(iEvent,egammas,e1x5,"e1x5");
    putMap<float>(iEvent,egammas,e2x2,"e2x2");
    putMap<float>(iEvent,egammas,e3x2,"e3x2");
    putMap<float>(iEvent,egammas,e3x3,"e3x3");
    putMap<float>(iEvent,egammas,e4x4,"e4x4");
    putMap<float>(iEvent,egammas,e5x5,"e5x5");

    putMap<float>(iEvent,egammas,e2x5Right,"e2x5Right");
    putMap<float>(iEvent,egammas,e2x5Left,"e2x5Left");
    putMap<float>(iEvent,egammas,e2x5Top,"e2x5Top");
    putMap<float>(iEvent,egammas,e2x5Bottom,"e2x5Bottom");
    putMap<float>(iEvent,egammas,e2x5Max,"e2x5Max");

    putMap<float>(iEvent,egammas,eLeft,"eLeft");
    putMap<float>(iEvent,egammas,eRight,"eRight");
    putMap<float>(iEvent,egammas,eTop,"eTop");
    putMap<float>(iEvent,egammas,eBottom,"eBottom");

    putMap<float>(iEvent,egammas,eMax,"eMax");
    putMap<float>(iEvent,egammas,e2nd,"e2nd");

    putMap<float>(iEvent,egammas,etaLat,"etaLat");
    putMap<float>(iEvent,egammas,phiLat,"phiLat");
    putMap<float>(iEvent,egammas,lat,"lat");

    putMap<float>(iEvent,egammas,covEtaEta,"covEtaEta");
    putMap<float>(iEvent,egammas,covEtaPhi,"covEtaPhi");
    putMap<float>(iEvent,egammas,covPhiPhi,"covPhiPhi");

    putMap<float>(iEvent,egammas,covIEtaIEta,"covIEtaIEta");
    putMap<float>(iEvent,egammas,covIEtaIPhi,"covIEtaIPhi");
    putMap<float>(iEvent,egammas,covIPhiIPhi,"covIPhiIPhi");

    putMap<float>(iEvent,egammas,zernike20,"zernike20");
    putMap<float>(iEvent,egammas,zernike42,"zernike42");

    putMap<float>(iEvent,egammas,seedTime,"seedTime");
    putMap<float>(iEvent,egammas,seedOutOfTimeChi2,"seedOutOfTimeChi2");
    putMap<float>(iEvent,egammas,seedChi2,"seedChi2");
    putMap<int  >(iEvent,egammas,seedRecoFlag,"seedRecoFlag");
    putMap<int  >(iEvent,egammas,seedSeverityLevel,"seedSeverityLevel");
    putMap<float>(iEvent,egammas,seedSwissCross,"seedSwissCross");
    putMap<float>(iEvent,egammas,seedE1OverE9,"seedE1OverE9");
    
    putMap<float>(iEvent,egammas,scRoundness,"scRoundness");
    putMap<float>(iEvent,egammas,scAngle,"scAngle");

    putMap<float>(iEvent,egammas,sMin,"sMin");
    putMap<float>(iEvent,egammas,sMaj,"sMaj");
    putMap<float>(iEvent,egammas,alphaSMinMaj,"alphaSMinMaj");

    putMap<float>(iEvent,egammas,seedE2overE9,"seedE2overE9");

  } // EgammaUserDataProducer<EgammaType>::produce

  template <typename EgammaType> template <typename UserDataType>
  void
  EgammaUserDataProducer<EgammaType>::putMap (
    edm::Event & iEvent,
    edm::Handle<edm::View<EgammaType> >& egammas,
    std::vector<UserDataType>& vUserData,
    const std::string& name
    )
  {
    using namespace std;
    using namespace edm;

    auto_ptr<ValueMap<UserDataType> > prod( new ValueMap<UserDataType>() );
    typename ValueMap<UserDataType>::Filler filler (*prod);
    filler.insert(egammas, vUserData.begin(), vUserData.end());
    filler.fill();
    iEvent.put(prod, name);
  } // EgammaUserDataProducer<EgammaType>::putMap

} // namespace::vgamma

#endif
