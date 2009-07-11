#include "TauAnalysis/CandidateTools/plugins/MEtTopologyProducer.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/Ptr.h"

#include "DataFormats/METReco/interface/MET.h"

#include "DataFormats/Candidate/interface/Candidate.h" 

#include "AnalysisDataFormats/TauAnalysis/interface/MEtTopology.h"
#include "AnalysisDataFormats/TauAnalysis/interface/MEtTopologyFwd.h"

#include "TauAnalysis/CandidateTools/interface/FetchCollection.h"

MEtTopologyProducer::MEtTopologyProducer(const edm::ParameterSet& cfg)
  : algorithm_(cfg)
{
  srcEnergyDeposits_ = cfg.getParameter<edm::InputTag>("srcEnergyDeposits");
  srcMET_ = cfg.getParameter<edm::InputTag>("srcMET");

  produces<MEtTopologyCollection>("");
}

MEtTopologyProducer::~MEtTopologyProducer()
{
//--- nothing to be done yet...
}

void MEtTopologyProducer::produce(edm::Event& evt, const edm::EventSetup& es)
{
  typedef edm::View<reco::Candidate> energyDepositCollectionType;
  edm::Handle<energyDepositCollectionType> energyDeposits;
  pf::fetchCollection(energyDeposits, srcEnergyDeposits_, evt);

  typedef edm::View<reco::MET> metCollectionType;
  edm::Handle<metCollectionType> metCollection;
  pf::fetchCollection(metCollection, srcMET_, evt);

//--- check that there is exactly one MET object in the event
//    (missing transverse momentum is an **event level** quantity)
  if ( metCollection->size() != 1 ) {
    edm::LogError ("produce") << " Found " << metCollection->size() << " MET objects in collection = " << srcMET_ << ","
			      << " --> MEtTopology collection will NOT be produced !!";
    std::auto_ptr<MEtTopologyCollection> emptyMEtTopologyCollection(new MEtTopologyCollection());
    evt.put(emptyMEtTopologyCollection);
    return;
  }
  
  edm::Ptr<reco::MET> metPtr = metCollection->ptrAt(0);

  std::auto_ptr<MEtTopologyCollection> metTopologyCollection(new MEtTopologyCollection());
  MEtTopology metTopology = algorithm_.buildMEtTopology(metPtr, *energyDeposits);
  metTopologyCollection->push_back(metTopology);
  evt.put(metTopologyCollection);
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_ANOTHER_FWK_MODULE(MEtTopologyProducer);



