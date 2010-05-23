
#ifndef TauAnalysis_Core_CaloMEtHistManager_h  
#define TauAnalysis_Core_CaloMEtHistManager_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "TauAnalysis/Core/interface/MEtHistManager.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "Math/GenVector/VectorUtil.h"
#include "Math/GenVector/PxPyPzE4D.h"
#include "DataFormats/Math/interface/Vector3D.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/LorentzVectorFwd.h"

#include <vector>
#include <string>

class CaloMEtHistManager : public MEtHistManager
{
 public:  
  explicit CaloMEtHistManager(const edm::ParameterSet&);
  ~CaloMEtHistManager();

 private:
//--- histogram booking and filling functions 
//    inherited from HistManagerBase class
  void bookHistogramsImp();
  void fillHistogramsImp(const edm::Event&, const edm::EventSetup&, double);

//--- histograms
  MonitorElement* hRAWplusJESplusMUONplusTAU_MEtPt_;
  MonitorElement* hRAWplusJESplusMUONplusTAU_MEtPhi_;
  MonitorElement* hRAWplusJESplusMUONplusTAU_MEtPx_;
  MonitorElement* hRAWplusJESplusMUONplusTAU_MEtPy_;
  MonitorElement* hRAWplusJESplusMUON_MEtPt_;
  MonitorElement* hRAWplusJESplusMUON_MEtPhi_;
  MonitorElement* hRAWplusJESplusMUON_MEtPx_;
  MonitorElement* hRAWplusJESplusMUON_MEtPy_;
  MonitorElement* hRAWplusMUONplusTAU_MEtPt_;
  MonitorElement* hRAWplusMUONplusTAU_MEtPhi_;
  MonitorElement* hRAWplusMUONplusTAU_MEtPx_;
  MonitorElement* hRAWplusMUONplusTAU_MEtPy_;
  MonitorElement* hRAW_MEtPt_;
  MonitorElement* hRAW_MEtPhi_;
  MonitorElement* hRAW_MEtPx_;
  MonitorElement* hRAW_MEtPy_;
  MonitorElement* hRAW_MEtSignificance_;
  MonitorElement* hRAWplusJES_MEtPt_;
  MonitorElement* hRAWplusJES_MEtPhi_;
  MonitorElement* hRAWplusJES_MEtPx_;
  MonitorElement* hRAWplusJES_MEtPy_;
  MonitorElement* hMUON_MExCorrection_;
  MonitorElement* hMUON_MEyCorrection_;
  MonitorElement* hTAU_MExCorrection_;
  MonitorElement* hTAU_MEyCorrection_;
  MonitorElement* hJES_MExCorrection_;
  MonitorElement* hJES_MEyCorrection_;
  MonitorElement* hRAWplusJESplusMUONplusTAUMEtPtDiffGen_;
  MonitorElement* hRAWplusJESplusMUONplusTAUMEtPtResGen_;
  MonitorElement* hRAWplusJESplusMUONplusTAUMEtPtRecVsGen_;
  MonitorElement* hRAWplusJESplusMUONplusTAUMEtPhiDiffGen_;
  MonitorElement* hRAWplusJESplusMUONplusTAUMEtPhiRecVsGen_;
  MonitorElement* hRAWplusJESplusMUONMEtPtDiffGen_;
  MonitorElement* hRAWplusJESplusMUONMEtPtResGen_;
  MonitorElement* hRAWplusJESplusMUONMEtPtRecVsGen_;
  MonitorElement* hRAWplusJESplusMUONMEtPhiDiffGen_;
  MonitorElement* hRAWplusJESplusMUONMEtPhiRecVsGen_;
  MonitorElement* hRAWplusJESMEtPtDiffGen_;
  MonitorElement* hRAWplusJESMEtPtResGen_;
  MonitorElement* hRAWplusJESMEtPtRecVsGen_;
  MonitorElement* hRAWplusJESMEtPhiDiffGen_;
  MonitorElement* hRAWplusJESMEtPhiRecVsGen_;
  MonitorElement* hRAWMEtPtDiffGen_;
  MonitorElement* hRAWMEtPtResGen_;
  MonitorElement* hRAWMEtPtRecVsGen_;
  MonitorElement* hRAWMEtPhiDiffGen_;
  MonitorElement* hRAWMEtPhiRecVsGen_;
  MonitorElement* hGenMEtDeltaRAWplusJESplusMUONplusTAUMEt_Pt_;
  MonitorElement* hGenMEtDeltaRAWplusJESplusMUONplusTAUMEt_Phi_;
  MonitorElement* hGenMEtDeltaRAWplusJESplusMUONplusTAUMEt_Px_;
  MonitorElement* hGenMEtDeltaRAWplusJESplusMUONplusTAUMEt_Py_;
  MonitorElement* hGenMEtDeltaRAWplusJESplusMUONMEt_Pt_;
  MonitorElement* hGenMEtDeltaRAWplusJESplusMUONMEt_Phi_;
  MonitorElement* hGenMEtDeltaRAWplusJESplusMUONMEt_Px_;
  MonitorElement* hGenMEtDeltaRAWplusJESplusMUONMEt_Py_;
  MonitorElement* hGenMEtDeltaRAWplusJESMEt_Pt_;
  MonitorElement* hGenMEtDeltaRAWplusJESMEt_Phi_;
  MonitorElement* hGenMEtDeltaRAWplusJESMEt_Px_;
  MonitorElement* hGenMEtDeltaRAWplusJESMEt_Py_;
  MonitorElement* hGenMEtDeltaRAWMEt_Pt_;
  MonitorElement* hGenMEtDeltaRAWMEt_Phi_;
  MonitorElement* hGenMEtDeltaRAWMEt_Px_;
  MonitorElement* hGenMEtDeltaRAWMEt_Py_;
};

#endif
