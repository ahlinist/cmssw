#ifndef TauAnalysis_Core_VertexHistManager_h  
#define TauAnalysis_Core_VertexHistManager_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "TauAnalysis/Core/interface/HistManagerBase.h"

#include <string>

class VertexHistManager : public HistManagerBase 
{
 public:
    explicit VertexHistManager(const edm::ParameterSet&);
  ~VertexHistManager();
  
 private: 
//--- histogram booking and filling functions 
//    inherited from HistManagerBase class
  void bookHistogramsImp();
  void fillHistogramsImp(const edm::Event&, const edm::EventSetup&, double);

//--- configuration parameters
  edm::InputTag vertexSrc_;

  bool makeVertexXvsYhistogram_;

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
};

#endif  


