#ifndef TauAnalysis_CandidateTools_NSVfitProducer_h
#define TauAnalysis_CandidateTools_NSVfitProducer_h

/** \class NSVfitProducer
 *
 * Produce data-formats storing solutions of nSVfit algorithm
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.2 $
 *
 * $Id: NSVfitProducerT.h,v 1.2 2011/03/23 17:46:39 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitAlgorithmBase.h"

#include <TStopwatch.h>

#include <vector>
#include <string>

template <typename T>
class NSVfitProducerT : public edm::EDProducer 
{
 public:
  explicit NSVfitProducerT(const edm::ParameterSet&);
  ~NSVfitProducerT();

  void beginJob();

  void produce(edm::Event&, const edm::EventSetup&);

  void endJob();

 private:
  std::string moduleLabel_;

  std::string instanceLabel_;

  NSVfitAlgorithmBase* algorithm_;
  
  typedef std::vector<T> NSVfitEventHypothesisCollection;

  typedef std::vector<std::string> vstring;
  vstring inputParticleNames_;
  typedef std::vector<edm::InputTag> vInputTag;
  vInputTag srcInputParticles_;
  unsigned numInputParticles_;

  double dRmin_; // minimum eta-phi separation between any pair of input particles

  edm::InputTag srcMEt_;
  edm::InputTag srcPrimaryVertex_;

  TStopwatch* timer_;
  long numSVfitCalls_;
  unsigned instanceId_;
  static unsigned instanceCounter_;
};

#endif

