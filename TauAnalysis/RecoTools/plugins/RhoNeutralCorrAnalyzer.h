#ifndef TauAnalysis_RecoTools_RhoNeutralCorrAnalyzer_h
#define TauAnalysis_RecoTools_RhoNeutralCorrAnalyzer_h

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DQMServices/Core/interface/MonitorElement.h"

#include <string>
#include <vector>

class RhoNeutralCorrAnalyzer : public edm::EDAnalyzer 
{
 public:

  // constructor 
  explicit RhoNeutralCorrAnalyzer(const edm::ParameterSet&);
    
  // destructor
  ~RhoNeutralCorrAnalyzer();
    
 private:

  void beginJob();
  void analyze(const edm::Event&, const edm::EventSetup&);
  void endJob();

  std::string moduleLabel_;

  edm::InputTag srcVertices_;

  edm::InputTag srcRhoNeutral_;
  edm::InputTag srcRhoChargedHadronsNoPileUp_;
  edm::InputTag srcRhoChargedHadronsPileUp_;
  edm::InputTag srcRhoChargedHadrons_;
  edm::InputTag srcRhoNeutralHadrons_;
  edm::InputTag srcRhoPhotons_;

  edm::InputTag srcGenPileUp_;

  typedef std::vector<edm::InputTag> vInputTag;
  vInputTag srcWeights_;

  MonitorElement* meNumVerticesVsGenPileUp_;
  MonitorElement* meNumVertices_;

  MonitorElement* meRhoNeutralVsGenPileUp_;
  MonitorElement* meRhoNeutral_;
  MonitorElement* meRhoChargedHadronsNoPileUpVsGenPileUp_;
  MonitorElement* meRhoChargedHadronsNoPileUp_;
  MonitorElement* meRhoChargedHadronsPileUpVsGenPileUp_;
  MonitorElement* meRhoChargedHadronsPileUp_;
  MonitorElement* meRhoChargedHadronsVsGenPileUp_;
  MonitorElement* meRhoChargedHadrons_;
  MonitorElement* meRhoNeutralHadrons_;
  MonitorElement* meRhoPhotons_;
  
  MonitorElement* meRhoNeutralDivRhoChargedHadronsNoPileUpVsGenPileUp_;
  MonitorElement* meRhoNeutralDivRhoChargedHadronsNoPileUp_;
  MonitorElement* meRhoNeutralDivRhoChargedHadronsPileUpVsGenPileUp_;
  MonitorElement* meRhoNeutralDivRhoChargedHadronsPileUp_;
  MonitorElement* meRhoNeutralDivRhoChargedHadronsVsGenPileUp_;
  MonitorElement* meRhoNeutralDivRhoChargedHadrons_;
};

#endif   
