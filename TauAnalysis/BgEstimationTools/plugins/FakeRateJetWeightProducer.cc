#include "TauAnalysis/BgEstimationTools/plugins/FakeRateJetWeightProducer.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/Ref.h"

#include "DataFormats/TauReco/interface/CaloTau.h"
#include "DataFormats/TauReco/interface/PFTau.h"

#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/PatCandidates/interface/LookupTableRecord.h"

typedef edm::ValueMap<pat::LookupTableRecord> LookupTableMap;

FakeRateJetWeightProducer::FakeRateJetWeightProducer(const edm::ParameterSet& cfg)
  : FakeRateWeightProducerBase(cfg)
{
  for ( fakeRateTypeMap::const_iterator frTypeEntry = fakeRateTypes_.begin();
	frTypeEntry != fakeRateTypes_.end(); ++frTypeEntry ) {
    produces<LookupTableMap>(frTypeEntry->first);
  }
}

FakeRateJetWeightProducer::~FakeRateJetWeightProducer()
{
//--- nothing to be done yet...
}

void FakeRateJetWeightProducer::produce(edm::Event& evt, const edm::EventSetup&)
{
  if ( gVerbosity_ ) {
    std::cout << "<FakeRateJetWeightProducer::produce>:" << std::endl;
    std::cout << " moduleLabel = " << moduleLabel_ << std::endl;
  }

  if ( cfgError_ ) return;

  edm::Handle<edm::View<reco::BaseTau> > allTauJets;
  evt.getByLabel(allTauJetSource_, allTauJets);

  edm::Handle<edm::View<reco::Candidate> > preselTauJets;
  evt.getByLabel(preselTauJetSource_, preselTauJets);

  for ( fakeRateTypeMap::const_iterator frTypeEntry = fakeRateTypes_.begin();
	frTypeEntry != fakeRateTypes_.end(); ++frTypeEntry ) {
    if ( gVerbosity_ ) std::cout << "frType = " << frTypeEntry->first << std::endl;

    std::vector<pat::LookupTableRecord> fakeRateJetWeights;

    unsigned numTauJets = allTauJets->size();
    for ( unsigned iTauJet = 0; iTauJet < numTauJets; ++iTauJet ) {
      edm::RefToBase<reco::BaseTau> tauJetRef = allTauJets->refAt(iTauJet);

      double tauJetIdEff = 1.;
      double qcdJetFakeRate = 1.;

      bool tauJetDiscr_passed = true;

      getTauJetProperties(evt, tauJetRef, iTauJet, preselTauJets, frTypeEntry->second, tauJetIdEff, qcdJetFakeRate, tauJetDiscr_passed);
     
      double fakeRateJetWeight = getFakeRateJetWeight(tauJetIdEff, qcdJetFakeRate, tauJetDiscr_passed, tauJetRef.get());

      if ( gVerbosity_ )
        std::cout << " Pt = " << tauJetRef->pt() << ", eta = " << tauJetRef->eta() << ", phi = " << tauJetRef->phi() << ","
		  << " jet-radius = " << getJetRadius(*tauJetRef) << ":" << " jet weight = " << fakeRateJetWeight << std::endl;

      fakeRateJetWeights.push_back(pat::LookupTableRecord(fakeRateJetWeight));
    }

    std::auto_ptr<LookupTableMap> fakeRateJetWeightMap(new LookupTableMap());
    LookupTableMap::Filler valueMapFiller(*fakeRateJetWeightMap);
    valueMapFiller.insert(allTauJets, fakeRateJetWeights.begin(), fakeRateJetWeights.end());
    valueMapFiller.fill();

    evt.put(fakeRateJetWeightMap, frTypeEntry->first);
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(FakeRateJetWeightProducer);
