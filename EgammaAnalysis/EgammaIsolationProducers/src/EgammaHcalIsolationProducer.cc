//*****************************************************************************
// File:      EgammaHcalIsolationProducer.cc
// ----------------------------------------------------------------------------
// OrigAuth:  Matthias Mozer
// Institute: IIHE-VUB
//=============================================================================
//*****************************************************************************


#include "EgammaAnalysis/EgammaIsolationProducers/interface/EgammaHcalIsolationProducer.h"

// Framework
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandAssociation.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"

#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"

#include "Geometry/Records/interface/IdealGeometryRecord.h"

EgammaHcalIsolationProducer::EgammaHcalIsolationProducer(const edm::ParameterSet& config) : conf_(config)
{
 // use configuration file to setup input/output collection names
  emObjectProducer_               = conf_.getParameter<edm::InputTag>("emObjectProducer");

  hcalRecHitProducer_           = conf_.getParameter<edm::InputTag>("hcalRecHitProducer");

  egHcalIsoPtMin_               = conf_.getParameter<double>("etMin");
  egHcalIsoConeSizeIn_            = conf_.getParameter<double>("intRadius");
  egHcalIsoConeSizeOut_            = conf_.getParameter<double>("extRadius");

 absolut_ = conf_.getParameter<bool>("absolut");

  //register your products
  produces < reco::CandViewDoubleAssociations >();
}


EgammaHcalIsolationProducer::~EgammaHcalIsolationProducer(){}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
EgammaHcalIsolationProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  
  // Get the  filtered objects
  edm::Handle< edm::View<reco::Candidate> > emObjectHandle;
  iEvent.getByLabel(emObjectProducer_,emObjectHandle);
  
  // Get the barrel hcal hits
  edm::Handle<HBHERecHitCollection> hcalRecHitHandle;
  iEvent.getByLabel(hcalRecHitProducer_, hcalRecHitHandle);
  
  HBHERecHitMetaCollection mhbhe =  HBHERecHitMetaCollection(*hcalRecHitHandle); 

  //Get Calo Geometry
  edm::ESHandle<CaloGeometry> pG;
  iSetup.get<IdealGeometryRecord>().get(pG);
  const CaloGeometry* caloGeom = pG.product();


  reco::CandViewDoubleAssociations* isoMap = new reco::CandViewDoubleAssociations( reco::CandidateBaseRefProd( emObjectHandle ) );


  EgammaHcalIsolation myHadIsolation(egHcalIsoConeSizeOut_,
			      egHcalIsoConeSizeIn_,
			      egHcalIsoPtMin_,
			      caloGeom,
			      &mhbhe) ;

  
  for( size_t i = 0 ; i < emObjectHandle->size(); ++i) {
    double isoValue = myHadIsolation.getHcalEtSum(&(emObjectHandle->at(i)));
    if(absolut_==true){
      isoMap->setValue(i,isoValue);
    }
    else{
      reco::SuperClusterRef sc = (emObjectHandle->at(i)).get<reco::SuperClusterRef>();
      double et = sc.get()->energy()*sin(2*atan(exp(-sc.get()->eta())));
      isoMap->setValue(i,isoValue/et);
    }
  }



  std::auto_ptr<reco::CandViewDoubleAssociations> isolMap(isoMap);
  iEvent.put(isolMap);

}

//define this as a plug-in
//DEFINE_FWK_MODULE(EgammaHcalIsolationProducer);
