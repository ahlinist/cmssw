#ifndef JetMETCorrections_METPUSubtraction_MVAPFMEtDataProducer_h
#define JetMETCorrections_METPUSubtraction_MVAPFMEtDataProducer_h

/** \class JVFJetIdProducer
 *
 * Discriminate jets originating from the hard-scatter event from pile-up jets,
 * based on the fraction of tracks within the jet that are associated to the hard-scatter vertex.
 * Jets outside the tracking acceptance are considered to originate from the hard-scatter event per default.
 * Optionally, they can be classified as pile-up.
 *
 * \authors Christian Veelken, LLR
 *
 * \version $Revision: 1.1 $
 *
 * $Id: JVFJetIdProducer.h,v 1.1 2012/08/31 08:45:32 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "PhysicsTools/SelectorUtils/interface/PFJetIDSelectionFunctor.h"
#include "RecoJets/JetProducers/interface/PileupJetIdentifier.h" //PH: migrated to CMSSW

class JVFJetIdProducer : public edm::EDProducer
{
 public:
  
  JVFJetIdProducer(const edm::ParameterSet&);
  ~JVFJetIdProducer();
  
 private:
  
  void produce(edm::Event&, const edm::EventSetup&);
  
  edm::InputTag srcJets_;

  edm::InputTag srcPFCandidates_;
  edm::InputTag srcPFCandToVertexAssociations_;
  edm::InputTag srcHardScatterVertex_;
  double minTrackPt_;
  double dZcut_;

  double JVFcut_;

  int neutralJetOption_;
  
  int verbosity_;
};

#endif
