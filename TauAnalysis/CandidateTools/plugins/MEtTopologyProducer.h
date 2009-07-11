#ifndef TauAnalysis_CandidateTools_MEtTopologyProducer_h
#define TauAnalysis_CandidateTools_MEtTopologyProducer_h

/** \class MEtTopologyProducer
 *
 * Produce data-formats providing information 
 * about distribution of energy deposits in the event 
 * with respect to direction of missing Et vector
 * 
 * \authors Christian Veelken
 *
 * \version $Revision: 1.1 $
 *
 * $Id: MEtTopologyProducer.h,v 1.1 2009/06/10 09:33:09 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "TauAnalysis/CandidateTools/interface/MEtTopologyAlgorithm.h"

class MEtTopologyProducer : public edm::EDProducer 
{
 public:

  explicit MEtTopologyProducer(const edm::ParameterSet&);
  ~MEtTopologyProducer();

  void produce(edm::Event&, const edm::EventSetup&);

 private:

  MEtTopologyAlgorithm algorithm_;
  
  edm::InputTag srcMET_;
  edm::InputTag srcEnergyDeposits_;
};

#endif

