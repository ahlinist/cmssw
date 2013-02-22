#ifndef JetMETCorrections_METPUSubtraction_NoPileUpPFMEtProducer_h
#define JetMETCorrections_METPUSubtraction_NoPileUpPFMEtProducer_h

/** \class NoPileUpPFMEtProducer
 *
 * Produce PFMET objects from no-PU jets + "unclustered" no-PU tracks + "unclustered" neutral particles
 * ("unclustered" particles = particles not within jets)
 *
 * \authors Christian Veelken, LLR
 *
 * \version $Revision: 1.1 $
 *
 * $Id: NoPileUpPFMEtProducer.h,v 1.1 2012/08/31 08:45:32 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "JetMETCorrections/METPUSubtraction/interface/PFMEtSignInterfaceBase.h"

#include <vector>

class NoPileUpPFMEtProducer : public edm::EDProducer
{
 public:
  
  NoPileUpPFMEtProducer(const edm::ParameterSet&);
  ~NoPileUpPFMEtProducer();
  
 private:
  
  void produce(edm::Event&, const edm::EventSetup&);
  
  std::string moduleLabel_;

  edm::InputTag srcMEt_;
  edm::InputTag srcMEtCov_;
  edm::InputTag srcJetInfo_;
  edm::InputTag srcPFCandInfo_;
  edm::InputTag srcPFCandInfoLeptonMatch_;
  typedef std::vector<edm::InputTag> vInputTag;
  vInputTag srcLeptons_;
  edm::InputTag srcType0Correction_;

  double sfNoPUjets_;
  double sfNoPUjetOffsetEnCorr_;
  double sfPUjets_;
  double sfNoPUunclChargedCands_;
  double sfPUunclChargedCands_;
  double sfUnclNeutralCands_;
  double sfType0Correction_;
  
  PFMEtSignInterfaceBase* pfMEtSignInterface_;

  bool saveInputs_;

  int verbosity_;
};

#endif
