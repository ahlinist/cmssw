#ifndef TauAnalysis_RecoTools_ZllHypothesisT1T2Producer_h
#define TauAnalysis_RecoTools_ZllHypothesisT1T2Producer_h

/** \class ZllHypothesisT1T2Producer
 *
 * Produce data-formats providing information 
 * about compatibility of an 
 *  elecron + tau-jet, muon + tau-jet or electron + muon pair
 * with the hypothesis of being an 
 *  electron + electron or muon + muon pair
 * resulting from a 
 *  Z --> e+ e- or Z --> mu+ mu-
 * decay
 *
 * \authors Christian Veelken
 *
 * \version $Revision: 1.1 $
 *
 * $Id: ZllHypothesisT1T2Producer.h,v 1.1 2009/07/29 13:03:38 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "TauAnalysis/RecoTools/interface/ZllHypothesisT1T2Algorithm.h"

template<typename T1, typename T2>
class ZllHypothesisT1T2Producer : public edm::EDProducer 
{
 public:

  explicit ZllHypothesisT1T2Producer(const edm::ParameterSet&);
  ~ZllHypothesisT1T2Producer();

  void produce(edm::Event&, const edm::EventSetup&);

 private:

  ZllHypothesisT1T2Algorithm<T1,T2> algorithm_;
  
  edm::InputTag srcDiCandidatePairs_;
  
  typedef std::vector<ZllHypothesisT1T2<T1,T2> > ZllHypothesisCollection;
};

#endif

