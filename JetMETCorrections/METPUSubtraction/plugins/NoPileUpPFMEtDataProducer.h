#ifndef JetMETCorrections_METPUSubtraction_NoPileUpPFMEtDataProducer_h
#define JetMETCorrections_METPUSubtraction_NoPileUpPFMEtDataProducer_h

/** \class NoPileUpPFMEtDataProducer
 *
 * Produce input objects used to compute MVA/No-PU MET
 *
 * \authors Phil Harris, CERN
 *          Christian Veelken, LLR
 *
 * \version $Revision: 1.1 $
 *
 * $Id: NoPileUpPFMEtDataProducer.h,v 1.1 2012/08/31 08:45:32 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "PhysicsTools/SelectorUtils/interface/PFJetIDSelectionFunctor.h"
#include "RecoJets/JetProducers//interface/PileupJetIdentifier.h" //PH: migrate to CMSSW

#include "JetMETCorrections/METPUSubtraction/interface/PFMEtSignInterfaceBase.h"

class NoPileUpPFMEtDataProducer : public edm::EDProducer
{
 public:
  
  NoPileUpPFMEtDataProducer(const edm::ParameterSet&);
  ~NoPileUpPFMEtDataProducer();
  
 private:
  
  void produce(edm::Event&, const edm::EventSetup&);
  
  std::string moduleLabel_;

  edm::InputTag srcJets_;
  edm::InputTag srcJetIds_;
  double minJetPt_;
  PileupJetIdentifier::Id jetIdSelection_;
  std::string jetEnOffsetCorrLabel_;
  
  edm::InputTag srcPFCandidates_;
  edm::InputTag srcPFCandToVertexAssociations_;
  edm::InputTag srcJetsForMEtCov_;
  double minJetPtForMEtCov_;
  edm::InputTag srcHardScatterVertex_;
  double dZcut_;
  
  double minPFNeutralHadronPt_;

  PFJetIDSelectionFunctor* looseJetIdAlgo_;
  
  PFMEtSignInterfaceBase* pfMEtSignInterface_;

  int maxWarnings_;
  int numWarnings_;

  int verbosity_;
};

#endif
