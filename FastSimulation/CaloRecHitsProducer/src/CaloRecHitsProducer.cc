#include "FWCore/PluginManager/interface/PluginManager.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "FastSimulation/CaloRecHitsProducer/interface/CaloRecHitsProducer.h"
#include "FastSimulation/CaloRecHitsProducer/interface/HcalRecHitsMaker.h"
#include "FastSimulation/CaloRecHitsProducer/interface/EcalBarrelRecHitsMaker.h"
#include "FastSimulation/CaloRecHitsProducer/interface/EcalEndcapRecHitsMaker.h"
#include "FastSimulation/CaloRecHitsProducer/interface/EcalPreshowerRecHitsMaker.h"

// Random engine
#include "FastSimulation/Utilities/interface/RandomEngine.h"
#include <TRandom3.h>

#include <iostream>

CaloRecHitsProducer::CaloRecHitsProducer(edm::ParameterSet const & p)
  : HcalRecHitsMaker_(NULL)
{    

  // Initialize the random number generator service
  edm::Service<edm::RandomNumberGenerator> rng;
  if ( ! rng.isAvailable() ) {
    throw cms::Exception("Configuration")
      << "CaloRecHitsProducer requires the RandomGeneratorService\n"
         "which is not present in the configuration file.\n"
         "You must add the service in the configuration file\n"
         "or remove the module that requires it";
  }

  bool useTRandom = p.getParameter<bool>("UseTRandomEngine");
  if ( !useTRandom ) { 
    random = new RandomEngine(&(*rng));
  } else {
    TRandom3* anEngine = new TRandom3();
    anEngine->SetSeed(rng->mySeed());
    random = new RandomEngine(anEngine);
  }

  edm::ParameterSet RecHitsParameters = p.getParameter<edm::ParameterSet>("RecHitsFactory");
  edm::ParameterSet CalibParameters = p.getParameter<edm::ParameterSet>("ContFact"); 

  EBrechitCollection_ = RecHitsParameters.getParameter<std::string>("EBrechitCollection");
  EErechitCollection_ = RecHitsParameters.getParameter<std::string>("EErechitCollection");
  ESrechitCollection_ = RecHitsParameters.getParameter<std::string>("ESrechitCollection");
  
  doDigis_ = RecHitsParameters.getParameter<bool>("doDigis");
  doMiscalib_ = RecHitsParameters.getParameter<bool>("doMiscalib");

  produces<HBHERecHitCollection>();
  produces<HORecHitCollection>();
  produces<HFRecHitCollection>();
  produces<EBRecHitCollection>(EBrechitCollection_);
  produces<EERecHitCollection>(EErechitCollection_);
  produces<ESRecHitCollection>(ESrechitCollection_);
  if(doDigis_)
    {
      produces<HBHEDigiCollection>();
      produces<HODigiCollection>();
      produces<HFDigiCollection>();
      produces<EBDigiCollection>();
      produces<EEDigiCollection>();
    }


  HcalRecHitsMaker_ = 
    new HcalRecHitsMaker(RecHitsParameters,random);
  EcalBarrelRecHitsMaker_ = 
    new EcalBarrelRecHitsMaker(RecHitsParameters,CalibParameters,random);
  EcalEndcapRecHitsMaker_ = 
    new EcalEndcapRecHitsMaker(RecHitsParameters,CalibParameters,random);
  EcalPreshowerRecHitsMaker_ = 
    new EcalPreshowerRecHitsMaker(RecHitsParameters,random);
}

CaloRecHitsProducer::~CaloRecHitsProducer() 
{ 
  std::cout << " Destructor CaloRecHitsProducer " << std::endl;

  if (EcalBarrelRecHitsMaker_) delete EcalBarrelRecHitsMaker_;
  if (EcalEndcapRecHitsMaker_) delete EcalEndcapRecHitsMaker_;
  if (EcalPreshowerRecHitsMaker_) delete EcalPreshowerRecHitsMaker_;
  if (HcalRecHitsMaker_) delete HcalRecHitsMaker_; 

  if ( random ) { 
    if ( random->theRootEngine() ) delete random->theRootEngine();
    delete random;
  }

  std::cout << " Done " << std::endl;
}

void CaloRecHitsProducer::beginJob(const edm::EventSetup & es)
{
  std::cout << " (Fast)RecHitsProducer initializing " << std::endl;
  EcalBarrelRecHitsMaker_->init(es,doDigis_,doMiscalib_);
  EcalEndcapRecHitsMaker_->init(es,doDigis_,doMiscalib_);
  EcalPreshowerRecHitsMaker_->init(es); 
  HcalRecHitsMaker_->init(es,doDigis_);
}

void CaloRecHitsProducer::endJob()
{ 
    std::cout << " (Fast)RecHitsProducer terminating " << std::endl; 
}

void CaloRecHitsProducer::produce(edm::Event & iEvent, const edm::EventSetup & es)
{
  // create empty outputs for HCAL 
  // see RecoLocalCalo/HcalRecProducers/src/HcalSimpleReconstructor.cc
  std::auto_ptr<EBRecHitCollection> receb(new EBRecHitCollection);  // ECAL Barrel
  std::auto_ptr<EERecHitCollection> recee(new EERecHitCollection);  // ECAL Endcap
  std::auto_ptr<ESRecHitCollection> reces(new ESRecHitCollection);  // ECAL pre-shower

  std::auto_ptr<HBHERecHitCollection> rec1(new HBHERecHitCollection); // Barrel+Endcap
  std::auto_ptr<HORecHitCollection> rec2(new HORecHitCollection);     // Outer
  std::auto_ptr<HFRecHitCollection> rec3(new HFRecHitCollection);     // Forward
 
  std::auto_ptr<EBDigiCollection> digieb(new EBDigiCollection(1));
  std::auto_ptr<EEDigiCollection> digiee(new EEDigiCollection(1));
  std::auto_ptr<HBHEDigiCollection> digihbhe(new HBHEDigiCollection);
  std::auto_ptr<HODigiCollection> digiho(new HODigiCollection);
  std::auto_ptr<HFDigiCollection> digihf(new HFDigiCollection);
 

  EcalBarrelRecHitsMaker_->loadEcalBarrelRecHits(iEvent,*receb,*digieb);

  EcalEndcapRecHitsMaker_->loadEcalEndcapRecHits(iEvent,*recee,*digiee);

  EcalPreshowerRecHitsMaker_->loadEcalPreshowerRecHits(iEvent,*reces);

  HcalRecHitsMaker_->loadHcalRecHits(iEvent,*rec1,*rec2,*rec3,*digihbhe,*digiho,*digihf);

  iEvent.put(receb,EBrechitCollection_);
  iEvent.put(recee,EErechitCollection_);
  iEvent.put(reces,ESrechitCollection_);
  iEvent.put(rec1);
  iEvent.put(rec2);
  iEvent.put(rec3);
  if(doDigis_)
    {
      iEvent.put(digihbhe);
      iEvent.put(digiho);
      iEvent.put(digihf);
      iEvent.put(digieb);
      iEvent.put(digiee);
    }
}

DEFINE_FWK_MODULE(CaloRecHitsProducer);
