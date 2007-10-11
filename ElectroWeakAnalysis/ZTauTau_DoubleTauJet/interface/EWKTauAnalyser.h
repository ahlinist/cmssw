#ifndef EWKTauAnalyser_H
#define EWKTauAnalyser_H

#include <string>
#include <vector>
#include <fstream>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/JetReco/interface/GenJet.h"

//#include "DataFormats/HLTReco/interface/HLTFilterObject.h"


#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>

//class PFTau;
class TFile;
//class GenJet;

class EWKTauAnalyser: public edm::EDAnalyzer {

public:
  /// Constructor
  explicit EWKTauAnalyser(const edm::ParameterSet&);

  /// Destructor
  ~EWKTauAnalyser() {}

  /// Destructor
  virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup);

protected: 

  /// BeginJob
  void beginJob(const edm::EventSetup& iSetup);

  /// Begin Run
  void beginRun(const edm::EventSetup& iSetup);

  /// End Job
  void endJob();

  bool getGenJet(const reco::PFTau& jet, const reco::GenJetCollection& genJets,
		 math::XYZTLorentzVector& genJetP4, int& decayMode, double& dr);
  void findIsolatedTaus(const edm::Event& iEvent, const edm::EventSetup& iSetup); 
  void findMET(const edm::Event& iEvent, const edm::EventSetup& iSetup); 
  void findMuons(const edm::Event& iEvent, const edm::EventSetup& iSetup); 
  void findElectrons(const edm::Event& iEvent, const edm::EventSetup& iSetup); 
  void findInvMassFromCalo(const edm::Event& iEvent, const edm::EventSetup& iSetup); 
  void findTauImpactParameter(const edm::Event& iEvent, const edm::EventSetup& iSetup); 
  int findTauDecayMode(const edm::Event& iEvent, const edm::EventSetup& iSetup, std::vector<int>& tauTypeList);
  int selectTau(const reco::PFTau& jet);
  void findInvariantMass(const reco::PFTau& jet);
  std::pair<unsigned int, unsigned int> getTauIndices(std::map<unsigned int, unsigned int>& smap);
  bool matchWithHLTJet(const reco::PFTau& tau, const reco::CaloJetCollection& caloJets, double& dr);
  //  bool matchWithHLTJet(const PFTau& tau, const Handle<reco::HLTFilterObjectWithRefs>& hltHandle, double& dr);


private:

  std::string PFTaus_;
  std::string TauDiscriminatorByIsolationProducer_;
  std::string metSrc_;
  std::string jetTagSrc_;
  std::string genJetSrc_;
  std::string caloJetSrc_;
  std::string jetCorrector_;
  std::string muonSrc_;
  std::string trackSrc_;
  std::string vertexSrc_;
  std::string electronSrc_;
  std::string caloInvMassSrc_;
  std::string genParticles_;
  std::string ipSrc_;
  std::string statFile_;
  std::string hltJetSrc_;

  double maxTauMass_;
  double tightTauLeadingTrkPt_;
  double looseTauLeadingTrkPt_;
  double minZMass_; 
  double maxZMass_; 

  double jetEtMinMETCorr_;
  double jetEMfracMETCorr_;

  double matchingCone_;
  double isolationCone_;
  double signalCone_;
  double leadingTrackPt_;
  double minimumTrackPt_;
  double clusterTrackMatchingCone_;

  int nEvent;

  TFile* theFile;

  TH1I* nTauCandH;
  TH1D* leadingTrackPtH;  
  TH1D* leadingTrackSiptH;  
  TH1D* ltIPSig1PH;  
  TH1D* ltIPSig3PH;  
  TH1D* invMassH;
  TH1D* invMass2H;
  TH1D* invMass3H;
  TH1D* invMass4H;
  TH1D* invMass5H;
  TH1D* invMassGenH;
  TH1D* invMassMCH;
  TH1D* invMassMC1H;
  TH1D* invMassMC3H;
  TH1D* maxHcalEnergyH;
  TH1D* emEnergyFracH;
  TH1D* emEnergyFrac2H;
  TH1I* nChHadronH;
  TH1I* nChHadronSigH;
  TH1I* nChHadronIsoH;
  TH1I* nGammaH;
  TH1I* nGammaSigH;
  TH1I* nGammaIsoH;
  TH1I* nNeuHadronH;
  TH1I* nNeuHadronSigH;
  TH1I* nNeuHadronIsoH;

  TH1I* nSelectedTracksH;
  TH1I* nSignalTracksH;
  TH1I* nIsolationTracksH;

  TH1D* tauPtH;
  TH1D* tauEtaH;
  TH1D* tauPtGenH;
  TH1D* tauEtaGenH;
  TH1D* tauPtDiscH;
  TH1D* tauEtaDiscH;
  TH1D* tauPhiH;
  TH1D* tauZVtxH;
  TH1D* tauChargeH;
  TH1D* tauCh1PrH;
  TH1D* tauCh3PrH;
  TH1D* tauDiscH;
  TH1D* tauDiscFirst2JetsH;
  TH1D* sumPtChHadIsoH;
  TH1D* sumPtGammaIsoH;

  TH1D* chHadEnergyJetH;
  TH1D* neuHadEnergyJetH;
  TH1D* chEMEnergyJetH;
  TH1D* chMuEnergyJetH;
  TH1D* neuEMEnergyJetH;
  TH1F* chMultJetH;
  TH1F* neuMultJetH;
  TH1F* muMultJetH;

  TH1D* tauDiscOrigH;
  TH1D* zMassH;
  TH1D* zEtaH;
  TH1D* zPhiH;
  TH1D* deltaRH;
  TH1D* deltaPhiH;
  TH1D* cosThetaH;

  TH1D* ptDiffH;
  TH2D* ptScatH;
  TH1I* tauDecayModeH; 
  TH1I* tauDecayProdH;

  TH1D* metH;
  TH1D* sumEtH;
  TH1D* metSigH;

  TH1D* caloInvMassH;

  TH1D* ip2DH;
  TH1D* ip3DH;
  TH1D* ip2DSigH;
  TH1D* ip3DSigH;

  TH1D* ltPtFracH;
  TH2D* ltvsJetPtH;
  TH1I* nTightTauH;
  TH1I* nLooseTauH;
  TH1I* statH;

  TH1D* dRTauAndHLTH;

  TProfile* jetPtWithEtaP;
  TProfile* trkPtWithEtaP;

  std::string fileName;
  int debugFlg;
  int applyDisc;
  std::string dataType;

  int nStat[12];

protected:
  std::ofstream fStatLog_;
};
#endif
