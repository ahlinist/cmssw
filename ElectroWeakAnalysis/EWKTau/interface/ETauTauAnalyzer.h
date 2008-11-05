#ifndef ETauTauAnalyzer_h  
#define ETauTauAnalyzer_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
#include "PhysicsTools/IsolationAlgos/interface/IsoDepositVetoFactory.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "Math/GenVector/VectorUtil.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/PatCandidates/interface/Flags.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/RecoCandidate/interface/IsoDepositVetos.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "TH1.h"
#include "TH2F.h"
#include "TLorentzVector.h"
#include "TString.h"
#include <vector>
#include <sstream>


class ETauTauAnalyzer : public edm::EDAnalyzer {

 public:
  
  explicit ETauTauAnalyzer(const edm::ParameterSet&);
  ~ETauTauAnalyzer();
  
 private:
  
  typedef math::XYZTLorentzVectorD LorentzVector;
  typedef std::vector<LorentzVector> LorentzVectorCollection;
  typedef std::vector<edm::InputTag> VInputTag;

  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  void FillTauHists(const edm::Event& evt,edm::InputTag,TH1F*,TH1F*,TH1F*); 
  void MakeElecRejPlots(const edm::Event&,edm::InputTag);
 
  VInputTag taus_;
  edm::InputTag elec_;
  VInputTag refObjects_;
  bool doTauAfterElec_,doTuning_,doMatching_;
  double eventWeight_;  

  unsigned int m_count_sel;
  edm::Handle<LorentzVectorCollection> m_tRefs;  
  
  TH2F *hTauEmFracVsEoP_ePreId, *hTauEmFracVsEoP_AePreId;
  TH2F *hTauH3x3oPVsEoP_ePreId, *hTauH3x3oPVsEoP_AePreId;
  TH1F *hTauH3x3oP, *hTauEmFrac, *hTauHoP;      
  TH1F *hTauHmaxoP, *hTauElecPreId, *hTauBremFracEoP;

  std::vector<TH1F*> m_tauEtHistoVec,m_tauEtaHistoVec,m_tauPhiHistoVec;
  TH1F *hSelHisto;
  
};  

#endif  


