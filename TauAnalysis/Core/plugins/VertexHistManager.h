#ifndef TauAnalysis_Core_VertexHistManager_h  
#define TauAnalysis_Core_VertexHistManager_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "TauAnalysis/Core/interface/HistManagerBase.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include <vector>
#include <string>

class VertexHistManager : public HistManagerBase 
{
 public:
    explicit VertexHistManager(const edm::ParameterSet&);
  ~VertexHistManager();
  
 private: 
//--- histogram booking and filling functions 
//    inherited from HistManagerBase class
  void bookHistograms();
  void fillHistograms(const edm::Event&, const edm::EventSetup&);

//--- configuration parameters
  edm::InputTag vertexSrc_;

  std::string dqmDirectory_store_;

//--- histograms
  MonitorElement* hVertexX_;
  MonitorElement* hVertexSigmaX_;
  MonitorElement* hVertexY_;
  MonitorElement* hVertexSigmaY_;
  MonitorElement* hVertexXvsY_;
  MonitorElement* hVertexZ_;
  MonitorElement* hVertexSigmaZ_;

  MonitorElement* hVertexNumTracks_;

  MonitorElement* hVertexChi2Prob_;

  int dqmError_;
};

#endif  


