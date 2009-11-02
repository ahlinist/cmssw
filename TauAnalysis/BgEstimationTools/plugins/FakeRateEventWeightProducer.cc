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
  produces<double>();
}

FakeRateEventWeightProducer::~FakeRateEventWeightProducer()
{
//--- nothing to be done yet...
}

void FakeRateEventWeightProducer::produce(edm::Event& evt, const edm::EventSetup&) 
{ 
  //std::cout << "<FakeRateEventWeightProducer::produce>:" << std::endl;

  if ( cfgError_ ) return;

  edm::Handle<edm::View<reco::BaseTau> > allTauJets;
  evt.getByLabel(allTauJetSource_, allTauJets);

  edm::Handle<edm::View<reco::Candidate> > preselTauJets;
  evt.getByLabel(preselTauJetSource_, preselTauJets);

  double fakeRateJetWeightSum = 0.;

  unsigned numTauJets = allTauJets->size();
  for ( unsigned iTauJet = 0; iTauJet < numTauJets; ++iTauJet ) {
    edm::RefToBase<reco::BaseTau> tauJetRef = allTauJets->refAt(iTauJet);
    
    double tauJetIdEff = 1.;
    double qcdJetFakeRate = 1.;
    
    bool tauJetDiscr_passed = true;
    
    getTauJetProperties(evt, tauJetRef, iTauJet, preselTauJets, tauJetIdEff, qcdJetFakeRate, tauJetDiscr_passed);
    
    double fakeRateJetWeight = 0.;
    
    if ( method_ == "simple" ) {
      fakeRateJetWeight = qcdJetFakeRate;
    } else if ( method_ == "CDF" ) {
      if ( tauJetIdEff > qcdJetFakeRate ) {
	fakeRateJetWeight = ( tauJetDiscr_passed ) ? 
	  -qcdJetFakeRate*(1. - tauJetIdEff)/(tauJetIdEff - qcdJetFakeRate) : qcdJetFakeRate*tauJetIdEff/(tauJetIdEff - qcdJetFakeRate);
      }
    }

    //std::cout << " --> jet weight = " << fakeRateJetWeight << std::endl;

    fakeRateJetWeightSum += fakeRateJetWeight;
  }

  double fakeRateEventWeight = fakeRateJetWeightSum;

  //std::cout << " --> event weight = " << fakeRateEventWeight << std::endl;

  std::auto_ptr<double> fakeRateEventWeightPtr(new double(fakeRateEventWeight));
  
  evt.put(fakeRateEventWeightPtr);
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(FakeRateEventWeightProducer);
