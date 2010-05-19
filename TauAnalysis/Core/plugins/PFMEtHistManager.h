#ifndef TauAnalysis_Core_PFMEtHistManager_h  
#define TauAnalysis_Core_PFMEtHistManager_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "TauAnalysis/Core/interface/HistManagerBase.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "Math/GenVector/VectorUtil.h"
#include "Math/GenVector/PxPyPzE4D.h"
#include "DataFormats/Math/interface/Vector3D.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/LorentzVectorFwd.h"

#include <vector>
#include <string>

class PFMEtHistManager : public HistManagerBase 
{
 public:  
  explicit PFMEtHistManager(const edm::ParameterSet&);
  ~PFMEtHistManager();

 private:
//--- histogram booking and filling functions 
//    inherited from HistManagerBase class
  void bookHistogramsImp();
  void fillHistogramsImp(const edm::Event&, const edm::EventSetup&, double);

//--- configuration parameters
  edm::InputTag metSrc_;
  edm::InputTag metSignificanceSrc_;

//--- histograms
  MonitorElement* hMEtPt_;
  MonitorElement* hMEtPhi_;
  MonitorElement* hMEtPx_;
  MonitorElement* hMEtPy_;
  MonitorElement* hMEtSignificance_;

  MonitorElement* hMEtPtDiffGen_;
  MonitorElement* hMEtPtResGen_;

  MonitorElement* hMEtPxDiffGen_;
  MonitorElement* hMEtPxResGen_;
  MonitorElement* hMEtPyDiffGen_;
  MonitorElement* hMEtPyResGen_;
  MonitorElement* hMEtPparlDiffGen_;
  MonitorElement* hMEtPparlResGen_;
  MonitorElement* hMEtPperpDiffGen_;
  MonitorElement* hMEtPperpResGen_;

  MonitorElement* hMEtPtRecVsGen_;
  MonitorElement* hMEtPhiDiffGen_;
  MonitorElement* hMEtPhiRecVsGen_;
  MonitorElement* hMEtSumEtRecVsGen_;

  MonitorElement* hGenMEtPt_;
  MonitorElement* hGenMEtPhi_;

  MonitorElement* hMEtPtDiffVsGenMEtPt_;
  MonitorElement* hMEtPhiDiffVsGenMEtPt_;
};

#endif
