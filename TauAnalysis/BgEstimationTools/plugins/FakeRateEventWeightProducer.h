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
 * \version $Revision: 1.6 $
 *
 * $Id: FakeRateEventWeightProducer.h,v 1.6 2010/09/28 11:23:25 jkolb Exp $
 *
 */

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/BgEstimationTools/interface/FakeRateWeightProducerBase.h"

class FakeRateEventWeightProducer : public FakeRateWeightProducerBase
{
 public:
  explicit FakeRateEventWeightProducer(const edm::ParameterSet&);
  ~FakeRateEventWeightProducer();
  
 private:
  void beginJob() {}
  void produce(edm::Event&, const edm::EventSetup&);
  void endJob() {}
};

#endif  


