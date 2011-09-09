#ifndef TauAnalysis_RecoTools_VertexMultiplicityVsRhoPFNeutralReweightProducer_h
#define TauAnalysis_RecoTools_VertexMultiplicityVsRhoPFNeutralReweightProducer_h

/** \class VertexMultiplicityVsRhoPFNeutralReweightProducer
 *
 * Reweight  Monte Carlo simulated events to match the in-time and out-of-time
 * pile-up distribution observed in data.
 *
 * The method is described in the presentation:
 *
 *   https://indico.cern.ch/getFile.py/access?contribId=47&sessionId=1&resId=0&materialId=slides&confId=142201
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
 * \version $Revision: 1.1 $
 *
 * $Id: VertexMultiplicityVsRhoPFNeutralReweightProducer.h,v 1.1 2011/08/19 14:11:40 veelken Exp $
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

