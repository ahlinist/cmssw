#ifndef TauAnalysis_BgEstimationTools_TauIdEffValidationHistManager_h  
#define TauAnalysis_BgEstimationTools_TauIdEffValidationHistManager_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

#include "TauAnalysis/Core/interface/HistManagerBase.h"

#include <vector>
#include <string>

class TauIdEffValidationHistManager : public HistManagerBase 
{
 public:  
  explicit TauIdEffValidationHistManager(const edm::ParameterSet&);
  ~TauIdEffValidationHistManager();
  
 private:
//--- histogram booking and filling functions 
//    inherited from HistManagerBase class
  void bookHistogramsImp();
  void fillHistogramsImp(const edm::Event&, const edm::EventSetup&, double);

//--- configuration parameters
  edm::InputTag tauSrc_;

  typedef std::vector<std::string> vstring;
  vstring frTypes_;
  vstring effTypes_;

  struct valPlotConfigEntryType
  {
    valPlotConfigEntryType(const std::string cutEffName, const vstring& cuts)
      : cutEffName_(cutEffName),
	cuts_(cuts)
    {}
    std::string cutEffName_;
    vstring cuts_;
  };

  std::vector<valPlotConfigEntryType> valPlotConfigEntries_;

//--- histograms
  struct valPlotEntryType
  {
    valPlotEntryType(DQMStore&, const std::string&, const std::string&, const std::string&, const std::string&, const vstring&);
    ~valPlotEntryType();
    void fillHistograms(const pat::Tau&, double);
    MonitorElement* hTauPt_;
    MonitorElement* hTauAssocJetPt_;
    MonitorElement* hTauEta_;
    MonitorElement* hTauPhi_;
    MonitorElement* hTauJetRadius_;
    MonitorElement* hTauNumTracksSignalCone_;
    MonitorElement* hTauNumTracksIsoCone_;
    MonitorElement* hTauLeadTrkPt_;
    std::string effTypeLabel_;
    std::string patTauEffName_;
    std::vector<StringCutObjectSelector<pat::Tau>*> cuts_;
  };

  std::vector<valPlotEntryType*> valPlotEntries_;
};

#endif  


