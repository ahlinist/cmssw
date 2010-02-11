#ifndef TauAnalysis_Skimming_EwkTauValidation_h
#define TauAnalysis_Skimming_EwkTauValidation_h

/** \class EwkTauValidation
 *
 * Booking and filling of histograms for data-quality monitoring purposes
 * in EWK tau analyses; individual channels are implemented in separate Ewk..HistManager classes,
 * so far:
 *  o Z --> electron + tau-jet channel (EwkElecTauHistManager)
 *  o Z --> muon + tau-jet channel (EwkMuTauHistManager)
 * 
 * \authors Letizia Lusito,
 *          Joshua Swanson,
 *          Christian Veelken
 *
 * \version $Revision: 1.1 $
 *
 * $Id: EwkTauValidation.h,v 1.1 2010/02/10 16:14:21 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "DQMServices/Core/interface/DQMStore.h"

#include "TauAnalysis/Skimming/interface/EwkValHistManagerBase.h"

#include <string>

class EwkTauValidation : public edm::EDAnalyzer 
{
 public:
  EwkTauValidation(const edm::ParameterSet&);
  ~EwkTauValidation();

  void beginJob();
  void analyze(const edm::Event&, const edm::EventSetup&);
  void endJob();
  
 private:
  void bookHistograms(edm::RunNumber_t runNumber);

  typedef std::vector<edm::ParameterSet> vParameterSet;
  vParameterSet cfgChannels_;

  typedef std::vector<EwkValHistManagerBase*> vEwkValHistManagerBase;
  std::map<edm::RunNumber_t, vEwkValHistManagerBase> histManagers_;

  DQMStore* dqmStore_;
  std::string dqmDirectory_;

  int dqmError_;

  int maxNumWarnings_;
};

#endif
