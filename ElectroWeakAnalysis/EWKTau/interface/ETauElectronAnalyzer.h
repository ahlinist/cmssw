#ifndef ETauElectronAnalyzer_h  
#define ETauElectronAnalyzer_h

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

class ETauElectronAnalyzer : public edm::EDAnalyzer {

 public:
  
  

  explicit ETauElectronAnalyzer(const edm::ParameterSet&);
  ~ETauElectronAnalyzer();
  
 private:
  
  typedef math::XYZTLorentzVectorD LorentzVector;
  typedef std::vector<LorentzVector> LorentzVectorCollection;
  typedef std::vector<edm::InputTag> VInputTag;

  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  void FillElecHists(const edm::Event& evt,edm::InputTag,TH1F*,TH1F*,TH1F*);
  void FillElecIsoHistos(const edm::Event&,edm::InputTag);
  void TuneIsoDeposit(const pat::Electron&,std::string);
  

  VInputTag elecs_;
  VInputTag refObjects_;
  unsigned int n_elecs_max_;
  bool doMatching_,doTuning_,doVeto_;
  double eventWeight_;  

  unsigned int m_count_sel;
  edm::Handle<LorentzVectorCollection> m_eRefs,m_tRefs;
  std::vector<unsigned int> m_refInd;

  TH1F *hTrkIsoElec, *hEclIsoElec, *hHclIsoElec;
  TH1F *hEoPElec, *hDphiInElec, *hDetaInElec, *hHoEElec, *hSigEtaEtaElec;

  
  std::vector<TH1F*> m_elecEtHistoVec,m_elecEtaHistoVec,m_elecPhiHistoVec;
  std::vector<TH1F*> m_trkIsoHistoVec,m_eclIsoHistoVec,m_eclIsoHistoVec_Bar,
    m_eclIsoHistoVec_End,m_hclIsoHistoVec;
  std::vector<TH2F*> m_eclIsoHistoVecVsEta;
  
  TH1F *hSelHisto;
  
};  

#endif  


