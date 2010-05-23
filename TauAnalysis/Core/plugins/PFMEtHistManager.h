#ifndef TauAnalysis_Core_PFMEtHistManager_h  
#define TauAnalysis_Core_PFMEtHistManager_h

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

class PFMEtHistManager : public MEtHistManager
{
 public:  
  explicit PFMEtHistManager(const edm::ParameterSet&);
  ~PFMEtHistManager();
};

#endif
