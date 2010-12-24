#ifndef TauAnalysis_Core_GenericEventDump_h
#define TauAnalysis_Core_GenericEventDump_h

/** \class GenericEventDump
 *
 * Base-class for print-out of event level information
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.15 $
 *
 * $Id: GenericEventDump.h,v 1.15 2010/09/02 11:30:45 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"
#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEtFwd.h"

#include "TauAnalysis/Core/interface/EventDumpBase.h"
#include "TauAnalysis/Core/interface/genericAnalyzerAuxFunctions.h"

#include <TMath.h>

#include <vector>
#include <string>

class GenericEventDump : public EventDumpBase
{
 public:
  // constructor 
  explicit GenericEventDump(const edm::ParameterSet&);
  
  // destructor
  virtual ~GenericEventDump();
  
  // derrived-class method for print-out of event level information
  virtual void analyze(const edm::Event&, const edm::EventSetup&, 
		       const GenericAnalyzer_namespace::filterResults_type&, const GenericAnalyzer_namespace::filterResults_type&, 
	               double);
  
 protected:
//--- function to count types of particles faking reconstructed electrons,
//    muons and tau-jets
  void countFakeParticles(const edm::Event&);
  
  //--- print functions to be used by derrived classes
  virtual void printEventHeaderInfo(const edm::Event&, double) const;
  virtual void printEventTriggerInfo(const edm::Event&) const;
  
  virtual void printMissingEtInfo(const edm::Event&) const;
  
  virtual void printJetInfo(const edm::Event&) const;
  
//--- configuration parameters
  edm::InputTag l1GtReadoutRecordSource_;
  edm::InputTag l1GtObjectMapRecordSource_;
  edm::InputTag hltResultsSource_;
  
  typedef std::vector<std::string> vstring;
  vstring l1BitsToPrint_;
  vstring hltPathsToPrint_;
  
  edm::InputTag genParticleSource_;
  edm::InputTag genJetSource_;
  edm::InputTag genTauJetSource_;
  edm::InputTag genEventInfoSource_;
  
  edm::InputTag patElectronSource_;
  edm::InputTag patMuonSource_;
  edm::InputTag patTauSource_;
  bool printTauIdEfficiencies_;
  edm::InputTag patJetSource_;
  
  edm::InputTag diTauCandidateSource_;
 
  edm::InputTag patCaloMEtSource_;
  edm::InputTag patPFMEtSource_;
  edm::InputTag genMEtSource_;
  
  std::vector<int> skipPdgIdsGenParticleMatch_;
  
  edm::InputTag recoTrackSource_;
  edm::InputTag recoVertexSource_;
  
  edm::InputTag pfChargedHadronSource_;
  edm::InputTag pfGammaSource_;
  edm::InputTag pfNeutralHadronSource_;
  edm::InputTag pfCandidateSource_;
  
  bool doGenInfo_;
  
//--- count different types of particles faking reconstructed electrons,
//    muons and tau-jets
  unsigned numRecoElectronsMatchingGenMuons_;
  unsigned numRecoElectronsMatchingGenElectrons_;
  unsigned numRecoElectronsMatchingGenTauJets_;
  unsigned numRecoElectronsMatchingGenBottomQuarks_;
  unsigned numRecoElectronsMatchingGenCharmQuarks_;
  unsigned numRecoElectronsMatchingGenGluons_;
  unsigned numRecoElectronsMatchingGenLightQuarks_;
  unsigned numRecoElectronsUndeterminedGenMatch_;
  
  unsigned numRecoMuonsMatchingGenMuons_;
  unsigned numRecoMuonsMatchingGenElectrons_;
  unsigned numRecoMuonsMatchingGenTauJets_;
  unsigned numRecoMuonsMatchingGenBottomQuarks_;
  unsigned numRecoMuonsMatchingGenCharmQuarks_;
  unsigned numRecoMuonsMatchingGenGluons_;
  unsigned numRecoMuonsMatchingGenLightQuarks_;
  unsigned numRecoMuonsUndeterminedGenMatch_;
  
  unsigned numRecoTauJetsMatchingGenMuons_;
  unsigned numRecoTauJetsMatchingGenElectrons_;
  unsigned numRecoTauJetsMatchingGenTauJets_;
  unsigned numRecoTauJetsMatchingGenBottomQuarks_;
  unsigned numRecoTauJetsMatchingGenCharmQuarks_;
  unsigned numRecoTauJetsMatchingGenGluons_;
  unsigned numRecoTauJetsMatchingGenLightQuarks_;
  unsigned numRecoTauJetsUndeterminedGenMatch_;
};

#endif       

