#include "TauAnalysis/BgEstimationTools/plugins/FakeRateEventWeightProducer.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/Ref.h"

#include "DataFormats/TauReco/interface/CaloTau.h"
#include "DataFormats/TauReco/interface/PFTau.h"

FakeRateEventWeightProducer::FakeRateEventWeightProducer(const edm::ParameterSet& cfg)
  : FakeRateWeightProducerBase(cfg)
{ 
  for ( fakeRateTypeMap::const_iterator frTypeEntry = fakeRateTypes_.begin();
	frTypeEntry != fakeRateTypes_.end(); ++frTypeEntry ) {
    produces<double>(frTypeEntry->first);
  }
}

FakeRateEventWeightProducer::~FakeRateEventWeightProducer()
{
//--- nothing to be done yet...
}

void FakeRateEventWeightProducer::produce(edm::Event& evt, const edm::EventSetup&) 
{ 
  if ( gVerbosity_ ) {
    std::cout << "<FakeRateEventWeightProducer::produce>:" << std::endl;
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

    double fakeRateJetWeightSum = 0.;

    unsigned numTauJets = allTauJets->size();
    for ( unsigned iTauJet = 0; iTauJet < numTauJets; ++iTauJet ) {
      edm::RefToBase<reco::BaseTau> tauJetRef = allTauJets->refAt(iTauJet);
      
      double tauJetIdEff = 1.;
      double qcdJetFakeRate = 1.;
      
      bool tauJetDiscr_passed = true;
      
      getTauJetProperties(evt, tauJetRef, iTauJet, preselTauJets, frTypeEntry->second, tauJetIdEff, qcdJetFakeRate, tauJetDiscr_passed);
    
      double fakeRateJetWeight = getFakeRateJetWeight(tauJetIdEff, qcdJetFakeRate, tauJetDiscr_passed, tauJetRef.get());
      
      fakeRateJetWeightSum += fakeRateJetWeight;
    }
    
    double fakeRateEventWeight = fakeRateJetWeightSum;

    if ( gVerbosity_ ) std::cout << " --> event weight = " << fakeRateEventWeight << std::endl;
    
    std::auto_ptr<double> fakeRateEventWeightPtr(new double(fakeRateEventWeight));
    
    evt.put(fakeRateEventWeightPtr, frTypeEntry->first);
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(FakeRateEventWeightProducer);
