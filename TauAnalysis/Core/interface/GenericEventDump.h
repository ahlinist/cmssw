#ifndef TauAnalysis_Core_GenericEventDump_h
#define TauAnalysis_Core_GenericEventDump_h

/** \class GenericEventDump
 *
 * Base-class for print-out of event level information
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.2 $
 *
 * $Id: GenericEventDump.h,v 1.2 2009/02/11 15:41:29 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "TauAnalysis/Core/interface/EventDumpBase.h"

#include <vector>
#include <string>

class GenericEventDump : public EventDumpBase
{
 public:
  // constructor 
  explicit GenericEventDump(const edm::ParameterSet&);
  
  // destructor
  virtual ~GenericEventDump();

 protected:
//--- functions to be used by derrived classes
  virtual void printEventHeaderInfo(const edm::Event&, double) const;
  virtual void printEventTriggerInfo(const edm::Event&) const;

  virtual void printElectronInfo(const edm::Event&) const;
  virtual void printMuonInfo(const edm::Event&) const;
  virtual void printTauInfo(const edm::Event&) const;
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
  edm::InputTag genTauJetSource_;

  edm::InputTag patElectronSource_;
  edm::InputTag patMuonSource_;
  edm::InputTag patTauSource_;

  edm::InputTag patMEtSource_;
  edm::InputTag genMEtSource_;

  edm::InputTag patJetSource_;

  edm::InputTag recoTrackSource_;
};

#endif       

