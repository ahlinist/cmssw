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
 * \version $Revision: 1.2 $
 *
 * $Id: EwkTauValidation.h,v 1.2 2010/01/21 09:40:36 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "DQMServices/Core/interface/DQMStore.h"

#include "TauAnalysis/Skimming/interface/EwkElecTauValHistManager.h"
#include "TauAnalysis/Skimming/interface/EwkMuTauValHistManager.h"

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
  DQMStore* dqmStore_;
  std::string dqmDirectory_;
  int dqmError_;
  int maxNumWarnings_;

  EwkElecTauValHistManager* elecTauHistManager_;
  EwkMuTauValHistManager* muTauHistManager_;
};

#endif
