#ifndef ETauAnalyzer_h  
#define ETauAnalyzer_h

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


#include "TH1.h"
#include "TLorentzVector.h"
#include "TString.h"
#include <vector>
#include <sstream>






class ETauAnalyzer : public edm::EDAnalyzer {

 public:
  
  

  explicit ETauAnalyzer(const edm::ParameterSet&);
  ~ETauAnalyzer();
  
 private:
  
  typedef math::XYZTLorentzVectorD LorentzVector;
  typedef std::vector<LorentzVector> LorentzVectorCollection;
  typedef std::vector<edm::InputTag> VInputTag;

  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  void  FillElecHists(const edm::Event& evt,edm::InputTag,TH1F*,TH1F*,TH1F*);
  void  FillTauHists(const edm::Event& evt,edm::InputTag,TH1F*,TH1F*,TH1F*);
  void FillElecIsoHistos(const edm::Event&,edm::InputTag);
  void TuneIsoDeposit(const pat::Electron&,std::string);
  std::vector<bool> MatchFinalObjects(TLorentzVector&,TLorentzVector&);
  void MakeMetLVs(const edm::Event&,TLorentzVector&,TLorentzVector&,
		  TLorentzVector&,TLorentzVector&,TLorentzVector&);
  std::vector<double> MakeMetPlots(TLorentzVector&,TLorentzVector&,
				   TLorentzVector&,TLorentzVector&,
				   TLorentzVector&,TLorentzVector&,
				   TLorentzVector&);
  void GetRefObjects(const edm::Event&);

  VInputTag elecs_,taus_;
  VInputTag refObjects_;
  edm::InputTag met_;
  bool doTauAfterElec_,doMetCuts_;
  double eMetDphi_,eMetMass_;
  bool doMatching_,doZee_;
  
  unsigned int m_count_sel;
  edm::Handle<LorentzVectorCollection> m_eRefs,m_tRefs;
  std::vector<unsigned int> m_refInd;

  TH1F *hPatL1ElecEt,*hPatL1ElecEta,*hPatL1ElecPhi;
  TH1F *hSelKinElecEt,*hSelKinElecEta,*hSelKinElecPhi;
  TH1F *hSelMchHLTElecEt,*hSelMchHLTElecEta,*hSelMchHLTElecPhi;
  TH1F *hSelTrkIsoElecEt,*hSelTrkIsoElecEta,*hSelTrkIsoElecPhi;
  TH1F *hSelEclIsoElecEt,*hSelEclIsoElecEta,*hSelEclIsoElecPhi;
  TH1F *hSelHclIsoElecEt,*hSelHclIsoElecEta,*hSelHclIsoElecPhi;
  TH1F *hSelIdElecEt,*hSelIdElecEta,*hSelIdElecPhi;
  TH1F *hSelIpElecEt,*hSelIpElecEta,*hSelIpElecPhi;
  TH1F *hTrkIsoElec, *hEclIsoElec, *hHclIsoElec;
  TH1F *hEoPElec, *hDphiInElec, *hDetaInElec, *hHoEElec, *hSigEtaEtaElec;

  TH1F *hPatL1TauEt,*hPatL1TauEta,*hPatL1TauPhi;
  TH1F *hSelKinTauEt,*hSelKinTauEta,*hSelKinTauPhi;
  TH1F *hSelMchHLTTauEt,*hSelMchHLTTauEta,*hSelMchHLTTauPhi;
  TH1F *hSelTrkIsoTauEt,*hSelTrkIsoTauEta,*hSelTrkIsoTauPhi;
  TH1F *hSelEclIsoTauEt,*hSelEclIsoTauEta,*hSelEclIsoTauPhi;
  TH1F *hSelHclIsoTauEt,*hSelHclIsoTauEta,*hSelHclIsoTauPhi;
  TH1F *hSelTrkTauEt,*hSelTrkTauEta,*hSelTrkTauPhi;
  TH1F *hSelLdgTrkTauEt,*hSelLdgTrkTauEta,*hSelLdgTrkTauPhi;
  TH1F *hSelElecRejTauEt,*hSelElecRejTauEta,*hSelElecRejTauPhi;
  TH1F *hSelCrackTauEt,*hSelCrackTauEta,*hSelCrackTauPhi;
  TH1F *hSelProngTauEt,*hSelProngTauEta,*hSelProngTauPhi;
  
  TH1F *hRawMet, *hType1Met, *hMuonMet, *hMuonType1Met,*hGenMet;
  TH1F *hRawMetRes, *hType1MetRes, *hMuonMetRes, *hMuonType1MetRes;
  TH1F *hRawMetPhi, *hType1MetPhi, *hMuonMetPhi, *hMuonType1MetPhi,*hGenMetPhi;
  TH1F *hRawMetPhiRes, *hType1MetPhiRes, *hMuonMetPhiRes, *hMuonType1MetPhiRes;
  TH1F *hM_eMet, *hDphi_eMet, *hDphi_tMet; 
  
  TH1F *hMvis;
  
  std::vector<TH1F*> trkIsoHistoVec,eclIsoHistoVec,hclIsoHistoVec;

  TH1F *hSelHisto;
  
};  

#endif  


