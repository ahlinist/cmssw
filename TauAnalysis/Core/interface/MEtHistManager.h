#ifndef TauAnalysis_Core_MEtHistManager_h  
#define TauAnalysis_Core_MEtHistManager_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
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

class MEtHistManager : public HistManagerBase 
{
 public:  
  explicit MEtHistManager(const edm::ParameterSet&);
  ~MEtHistManager();

 protected:
//--- histogram booking and filling functions 
//    inherited from HistManagerBase class
  void bookHistogramsImp();
  void fillHistogramsImp(const edm::Event&, const edm::EventSetup&, double);

//--- configuration parameters
  edm::InputTag metSrc_;
  bool expectUniqueMEt_;
  edm::InputTag metSignificanceSrc_;
  edm::InputTag leg1Src_;
  edm::InputTag leg2Src_;
  edm::InputTag vertexSrc_;

//--- histograms
  MonitorElement* hMEtPt_;
  MonitorElement* hMEtPhi_;
  MonitorElement* hMEtPx_;
  MonitorElement* hMEtPy_;

  std::vector<MonitorElement*> hMEtPtVsNumVertices_;
  typedef std::vector<double> vdouble;
  vdouble vertexPtThresholds_;

  MonitorElement* hMEtSignificance_;

  MonitorElement* hGenMEtPt_;
  MonitorElement* hGenMEtPhi_;

  MonitorElement* hMEtPtRecVsGen_;
  MonitorElement* hMEtPhiDiffGen_;
  MonitorElement* hMEtPhiRecVsGen_;
  MonitorElement* hMEtSumEtRecVsGen_;

  MonitorElement* hMEtPtDiffVsGenMEtPt_;
  MonitorElement* hMEtPhiDiffVsGenMEtPt_;

  MonitorElement* hMEtPtDiff_;
  MonitorElement* hMEtPtPull_;

//--- projections of difference 
//    between reconstructed and generated MEt
//    in x,y direction
  MonitorElement* hMEtPxDiff_;
  MonitorElement* hMEtPxPull_;
  MonitorElement* hMEtPyDiff_;
  MonitorElement* hMEtPyPull_;

//--- projections of difference 
//    between reconstructed and generated MEt
//    in direction parallel and perpendicular 
//    to generated missing-Et
  MonitorElement* hMEtPparlGenMEtDiff_;
  MonitorElement* hMEtPparlGenMEtPull_;
  MonitorElement* hMEtPperpGenMEtDiff_;
  MonitorElement* hMEtPperpGenMEtPull_;

//--- projections of difference 
//    between reconstructed and generated MEt
//    in direction parallel and perpendicular 
//    to visible tau decay products
  MonitorElement* hMEtPparlBisectorLeg1Leg2Diff_;
  MonitorElement* hMEtPparlBisectorLeg1Leg2Pull_;
  MonitorElement* hMEtPparlLeg1Diff_;
  MonitorElement* hMEtPparlLeg1Pull_;
  MonitorElement* hMEtPparlLeg2Diff_;
  MonitorElement* hMEtPparlLeg2Pull_;
};

#endif
