#ifndef TauAnalysis_RecoTools_VertexMultiplicityVsRhoPFNeutralReweightProducer_h
#define TauAnalysis_RecoTools_VertexMultiplicityVsRhoPFNeutralReweightProducer_h

/** \class VertexMultiplicityVsRhoPFNeutralReweightProducer
 *
 * Reweight simulated Monte Carlo events simulated to match the in-time and out-of-time
 * pile-up distribution observed in data.
 * Monte Carlo simulated events are reweighted

For this purpose 

 analyzed data sample

with pile-up to match the
 * vertex multiplicity distribution observed in the analyzed data sample
 *
 * NOTE:
 *      o weight > 1: fraction of events which given vertex multiplicity && neutral particle energy density higher
 *                    higher in data than in (pile-up) MC
 *      o weight = 1: fraction of events which given vertex multiplicity && neutral particle energy density same   
 *                    in data than in (pile-up) MC
 *      o weight < 1: fraction of events which given vertex multiplicity && neutral particle energy density lower  
 *                    in data than in (pile-up) MC
 *
 * \authors Alexei Raspereza, DESY
 *          Christian Veelken, LLR
 *
 * \version $Revision: 1.2 $
 *
 * $Id: VertexMultiplicityVsRhoPFNeutralReweightProducer.h,v 1.2 2011/04/13 17:05:13 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include <TFile.h>
#include <TH1.h>

class VertexMultiplicityVsRhoPFNeutralReweightProducer : public edm::EDProducer 
{
 public:
  explicit VertexMultiplicityVsRhoPFNeutralReweightProducer(const edm::ParameterSet&);
  ~VertexMultiplicityVsRhoPFNeutralReweightProducer();

  void produce(edm::Event&, const edm::EventSetup&);

 private:
  edm::InputTag srcVertices_;
  edm::InputTag srcRhoPFNeutral_;

  TFile* inputFile_;
  TH1* lut_;

  double minPUreweight_;
  double maxPUreweight_;
};

#endif

