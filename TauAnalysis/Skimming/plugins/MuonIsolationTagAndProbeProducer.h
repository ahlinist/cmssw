#ifndef TauAnalysis_Skimming_MuonIsolationTagAndProbeProducer_h
#define TauAnalysis_Skimming_MuonIsolationTagAndProbeProducer_h

/** \class MuonIsolationTagAndProbeProducer
 *
 * Flag the two "golden" muons produced in Z --> mu+ mu- decays as "tag" and "probe",
 * depending on whether one of the two muons passes isolation requirements or both:
 *  o both muons fail 
 *   --> event is rejected
 *  o one muon passes, one muon fails
 *   --> the muon passing the isolation requirements is flagged as "tag",
 *       the other one as probe
 *  o both muons pass the isolation requirements
 *   --> both muons are flagged as "tag" and both are flagged as "probe"
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.2 $
 *
 * $Id: MuonIsolationTagAndProbeProducer.h,v 1.2 2010/10/22 11:51:17 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

class MuonIsolationTagAndProbeProducer : public edm::EDProducer 
{  
 public:
  explicit MuonIsolationTagAndProbeProducer(const edm::ParameterSet&);
  ~MuonIsolationTagAndProbeProducer();

  void produce(edm::Event&, const edm::EventSetup&);
  
  enum { tagBit = 1, probeBit = 2 };

private:
  edm::InputTag srcAllMuons_;
  edm::InputTag srcGoodMuons_;
  edm::InputTag srcGoodIsoMuons_;
};

#endif


