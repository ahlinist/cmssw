// -*- C++ -*-
//
// Package:    PatAlgos
// Class:      EgammaUserDataProducer
//
/**\class EgammaUserDataProducer EgammaUserDataProducer.h ElectroWeakAnalysis/MultiBosons/interface/EgammaUserDataProducer.h

Description: Test module for UserData in PAT

Implementation:

*/
//
// Original Author:  Freya Blekman
//         Created:  Mon Apr 21 10:03:50 CEST 2008
// $Id: EgammaUserDataProducer.cc,v 1.1 2009/01/28 19:50:36 musella Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/Common/interface/OwnVector.h"

#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"
#include "RecoLocalCalo/EcalRecAlgos/interface/EcalSeverityLevelAlgo.h"

#include <string>
#include <vector>

namespace vgamma {
  //
  // class decleration
  //
  template <typename EgammaType>
  class EgammaUserDataProducer : public edm::EDProducer {
  public:
    explicit EgammaUserDataProducer(const edm::ParameterSet&);
    ~EgammaUserDataProducer();


  private:
    virtual void produce(edm::Event&, const edm::EventSetup&);

    // ----------member data ---------------------------
    void putMap(edm::Event & iEvent, edm::Handle<edm::View<EgammaType> >& egammas, std::vector<float>& vfloat, const std::string& name);

    edm::InputTag src_;
    edm::InputTag ebRechits_;
    edm::InputTag eeRechits_;
  };

  //
  // constructors and destructor
  //
  template <typename EgammaType>
  EgammaUserDataProducer<EgammaType>::EgammaUserDataProducer (
    const edm::ParameterSet& iConfig
    ):
    src_(iConfig.getParameter<edm::InputTag>("src")),
    ebRechits_(iConfig.getParameter<edm::InputTag>("ebRechits")),
    eeRechits_(iConfig.getParameter<edm::InputTag>("eeRechits"))
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

    produces<edm::ValueMap<float> > ("swissCross");
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
//     edm::Handle<EcalRecHitCollection> eeRechits;
//     edm::Handle<EcalRecHitCollection> ebRechits;

    iEvent.getByLabel(src_, egammas);
//     iEvent.getByLabel(ebRechits_, ebRechits);
//     iEvent.getByLabel(eeRechits_, eeRechits);


    EcalClusterLazyTools lazyTools(iEvent, iSetup, ebRechits_, eeRechits_);

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

    vector<float> swissCross;

    for( size_t ip = 0; ip<egammas->size(); ip++ ) {
      const reco::BasicCluster &cluster = *((*egammas)[ip].superCluster()->seed());

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

//       DetId id = cluster.seed();
//       float sx;
//       if (/* id is in EB*/)
//         EcalSeverityLevelAlgo::swissCross(id, ebRechits);
//       swissCross .push_back()

    }

    putMap(iEvent,egammas,e1x3,"e1x3");
    putMap(iEvent,egammas,e3x1,"e3x1");
    putMap(iEvent,egammas,e1x5,"e1x5");
    putMap(iEvent,egammas,e2x2,"e2x2");
    putMap(iEvent,egammas,e3x2,"e3x2");
    putMap(iEvent,egammas,e3x3,"e3x3");
    putMap(iEvent,egammas,e4x4,"e4x4");
    putMap(iEvent,egammas,e5x5,"e5x5");

    putMap(iEvent,egammas,e2x5Right,"e2x5Right");
    putMap(iEvent,egammas,e2x5Left,"e2x5Left");
    putMap(iEvent,egammas,e2x5Top,"e2x5Top");
    putMap(iEvent,egammas,e2x5Bottom,"e2x5Bottom");
    putMap(iEvent,egammas,e2x5Max,"e2x5Max");

    putMap(iEvent,egammas,eLeft,"eLeft");
    putMap(iEvent,egammas,eRight,"eRight");
    putMap(iEvent,egammas,eTop,"eTop");
    putMap(iEvent,egammas,eBottom,"eBottom");

    putMap(iEvent,egammas,eMax,"eMax");
    putMap(iEvent,egammas,e2nd,"e2nd");

    putMap(iEvent,egammas,etaLat,"etaLat");
    putMap(iEvent,egammas,phiLat,"phiLat");
    putMap(iEvent,egammas,lat,"lat");

    putMap(iEvent,egammas,covEtaEta,"covEtaEta");
    putMap(iEvent,egammas,covEtaPhi,"covEtaPhi");
    putMap(iEvent,egammas,covPhiPhi,"covPhiPhi");

    putMap(iEvent,egammas,covIEtaIEta,"covIEtaIEta");
    putMap(iEvent,egammas,covIEtaIPhi,"covIEtaIPhi");
    putMap(iEvent,egammas,covIPhiIPhi,"covIPhiIPhi");

    putMap(iEvent,egammas,zernike20,"zernike20");
    putMap(iEvent,egammas,zernike42,"zernike42");
  }

  template <typename EgammaType>
  void
  EgammaUserDataProducer<EgammaType>::putMap (
    edm::Event & iEvent,
    edm::Handle<edm::View<EgammaType> >& egammas,
    std::vector<float>& vfloat,
    const std::string& name
    )
  {
    using namespace std;
    using namespace edm;

    auto_ptr<ValueMap<float> > prod( new ValueMap<float>() );
    ValueMap<float>::Filler intfiller (*prod);
    intfiller.insert(egammas, vfloat.begin(), vfloat.end());
    intfiller.fill();
    iEvent.put(prod,name);
  }

} // namespace::vgamma
