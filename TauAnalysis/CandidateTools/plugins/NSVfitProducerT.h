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
 * $Id: NSVfitProducer.h,v 1.2 2010/09/28 11:23:28 jkolb Exp $
 *
 */

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitAlgorithmBase.h"

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

 private:
  std::string moduleLabel_;

  NSVfitAlgorithmBase* algorithm_;
  
  typedef std::vector<T> NSVfitEventHypothesisCollection;

  typedef std::vector<std::string> vstring;
  vstring inputParticleNames_;
  typedef std::vector<edm::InputTag> vInputTag;
  vInputTag srcInputParticles_;
  unsigned numInputParticles_;

  double dRmin_; // minimum eta-phi separation between any pair of input particles

  edm::InputTag srcMEt_;
};

#endif

