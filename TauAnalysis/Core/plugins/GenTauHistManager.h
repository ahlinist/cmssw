#ifndef TauAnalysis_Core_GenTauHistManager_h
#define TauAnalysis_Core_GenTauHistManager_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "TauAnalysis/Core/interface/HistManagerBase.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include <vector>
#include <string>

class GenTauHistManager : public HistManagerBase
{
 public:
  explicit GenTauHistManager(const edm::ParameterSet&);
  ~GenTauHistManager();

 private:
//--- histogram booking and filling functions
//    inherited from HistManagerBase class
  void bookHistogramsImp();
  void fillHistogramsImp(const edm::Event&, const edm::EventSetup&, double);

//--- configuration parameters
  edm::InputTag genParticleSrc_;

//--- histograms
  MonitorElement* hNumGenTaus_;

  MonitorElement* hGenTauPt_;
  MonitorElement* hGenTauEta_;
  MonitorElement* hGenTauPtVsEta_;
  MonitorElement* hGenTauPhi_;

  MonitorElement* hGenTauDecayMode_;
  MonitorElement* hVisEnFractionVsGenTauDecayMode_;

  MonitorElement* hVisEnFractionProduct_;

  MonitorElement* hGenTauQEDradFraction_;
};

#endif


