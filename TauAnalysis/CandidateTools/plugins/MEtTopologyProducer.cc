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
  srcEnergyDeposits_ = cfg.getParameter<vInputTag>("srcEnergyDeposits");

  produces<MEtTopologyCollection>("");
}

MEtTopologyProducer::~MEtTopologyProducer()
{
//--- nothing to be done yet...
}

void MEtTopologyProducer::produce(edm::Event& evt, const edm::EventSetup& es)
{
  typedef edm::View<reco::Candidate> energyDepositCollectionType;
  std::vector<energyDepositCollectionType> energyDepositCollections;
  for ( vInputTag::const_iterator srcEnergyDeposit = srcEnergyDeposits_.begin();
	srcEnergyDeposit != srcEnergyDeposits_.end(); ++srcEnergyDeposit ) {
    edm::Handle<energyDepositCollectionType> energyDepositCollection;
    pf::fetchCollection(energyDepositCollection, *srcEnergyDeposit, evt);
    energyDepositCollections.push_back(*energyDepositCollection);
  }

  std::auto_ptr<MEtTopologyCollection> metTopologyCollection(new MEtTopologyCollection());
  MEtTopology metTopology = algorithm_.buildMEtTopology(energyDepositCollections);
  metTopologyCollection->push_back(metTopology);
  evt.put(metTopologyCollection);
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(MEtTopologyProducer);



