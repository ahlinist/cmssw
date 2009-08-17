#ifndef TauAnalysis_BgEstimationTools_FakeRateEventWeightProducer_h  
#define TauAnalysis_BgEstimationTools_FakeRateEventWeightProducer_h

/** \class FakeRateEventWeightProducer
 *
 * Produce ValueMap associating reco::PFTau objects with pat::LookupTableRecords;
 * entries of the pat::LookupTableRecords represent the probability for a QCD-jet
 * with a given (jet-radius, eta and Et) to fake a tau-jet signature
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.4 $
 *
 * $Id: FakeRateEventWeightProducer.h,v 1.4 2009/08/12 14:34:53 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <string>

class FakeRateEventWeightProducer : public edm::EDProducer
{
 public:
  
  explicit FakeRateEventWeightProducer(const edm::ParameterSet&);
  ~FakeRateEventWeightProducer() {}
  
 private:

  void beginJob(const edm::EventSetup&) {}
  void produce(edm::Event&, const edm::EventSetup&);
  void endJob() {}

//--- configuration parameters
  edm::InputTag tauJetSource_;
  edm::InputTag tauJetIdEffSource_;
  edm::InputTag qcdJetFakeRateSource_;
  edm::InputTag tauJetDiscrSource_;
  double tauJetDiscrThreshold_;
};

#endif  


