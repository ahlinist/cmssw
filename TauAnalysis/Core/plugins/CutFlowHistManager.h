#ifndef TauAnalysis_Core_CutFlowHistManager_h  
#define TauAnalysis_Core_CutFlowHistManager_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "TauAnalysis/Core/interface/HistManagerBase.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include <vector>
#include <string>

class CutFlowHistManager : public HistManagerBase 
{
 public:  
  explicit CutFlowHistManager(const edm::ParameterSet&);
  ~CutFlowHistManager();
  
 private: 
//--- histogram booking and filling functions 
//    inherited from HistManagerBase class
  void bookHistogramsImp();
  void fillHistogramsImp(const edm::Event&, const edm::EventSetup&, double);

//--- auxiliary functions
  void bookCutFlowHistogram(int);

  struct histogramEntryType
  {
    MonitorElement* hCutFlow_;
    int multiplicity_;
  };

  void fillCutFlowHistogram(MonitorElement*, const std::vector<bool>&, double);

//--- configuration parameters
  typedef std::vector<edm::InputTag> vInputTag;
  vInputTag srcEventSelFlags_;
  edm::InputTag srcGenPileUp_;
  edm::InputTag srcRecVertices_;

  unsigned firstBin_;
  unsigned lastBin_;

  enum { kGen, kRec };
  int mode_;

//--- histograms
  std::vector<histogramEntryType> histograms_;
};

#endif  


