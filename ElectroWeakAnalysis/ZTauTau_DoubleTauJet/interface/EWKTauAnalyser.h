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

#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>

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
  int findTauDecayMode(const edm::Event& iEvent, 
                       const edm::EventSetup& iSetup, std::vector<int>& tauTypeList, bool fillHist=true);
  int selectTau(const reco::PFTau& jet);
  void findInvariantMass(const reco::PFTau& jet);
  std::pair<unsigned int, unsigned int> getTauIndices(std::map<unsigned int, unsigned int>& smap);
  //  bool matchWithHLTJet(const PFTau& tau, const Handle<reco::HLTFilterObjectWithRefs>& hltHandle, double& dr);
  void printStatistics(std::ostream& out);
  static bool applySelection(int bitWord, int bitPosition); 

public:

  static void TauObject(Handle<reco::PFTauCollection> tauHandle, int iTau, reco::PFTau& tau); 
  static bool matchWithHLTJet(const reco::PFTau& tau, const reco::CaloJetCollection& caloJets, double& dr);
  static void bit_print(int word, int pos=8);
  static double getTauCharge(const reco::PFTau& tau, int dflag=0);
  static void printCandidate(const reco::PFTau& tau);
  static void printEvent(const reco::PFTau& tau1, const reco::PFTau& tau2);
  static double sumOfIPSignificance(const reco::PFTau& tau1, const reco::PFTau& tau2, int dflag=0); 
  static bool prongsAsExpected(const reco::PFTau& tau1, const reco::PFTau& tau2, int dflag=0);

private:

  int _debugFlg;

  std::string _PFTaus;
  std::string _TauDiscriminatorByIsolationProducer;
  std::string _jetTagSrc;
  std::string _metSrc;
  std::string _genJetSrc;
  std::string _caloJetSrc;
  std::string _electronSrc;
  std::string _muonSrc;
  std::string _trackSrc;
  std::string _vertexSrc;
  std::string _hltJetSrc;
  std::string _genParticles;
  std::string _caloInvMassSrc;
  std::string _ipSrc;

  double _maxTauMass;
  double _tightTauLeadingTrkPt;
  double _looseTauLeadingTrkPt;
  double _leadingTrkPtFrac;
  double _minZMass; 
  double _maxZMass; 
     int _maxExtraJets;
  double _minIPSignificanceSum;
  double _minJetEt;
  double _maxHadTowerEnergy;

  std::string _jetCorrector;
  double _jetEtMinMETCorr;
  double _jetEMfracMETCorr;

  double _matchingCone;
  double _signalCone;
  double _isolationCone;
  double _leadingTrackPt;
  double _minimumTrackPt;
  double _clusterTrackMatchingCone;

  std::string _statFile;
  std::string _dataType;
  std::string _analysisType;
  std::string _selbitStr;
     int _selectionBits;
  double _eventCharge;
  
  int nEvent;

  TH1F* nJetsH;
  TH1F* nTauCandH;
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
  TH1D* invMassMC2H;
  TH1D* invMassMC3H;
  TH1D* maxHcalEnergyH;
  TH1D* maxHcalEnergy2H;
  TH1D* emEnergyFracH;
  TH1D* emEnergyFrac2H;
  TH1D* eOverPH;
  TH1D* etOverPtH;
  TH1D* hmaxOverPH;
  TH1D* htotOverPH;

  TH1F* nChHadronH;
  TH1F* nChHadronSigH;
  TH1F* nChHadronIsoH;
  TH1F* nGammaH;
  TH1F* nGammaSigH;
  TH1F* nGammaIsoH;
  TH1F* nNeuHadronH;
  TH1F* nNeuHadronSigH;
  TH1F* nNeuHadronIsoH;

  TH1F* nSelectedTracksH;
  TH1F* nSignalTracksH;
  TH1F* nIsolationTracksH;

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
  TH1F* tauDecayModeH; 
  TH1F* tauDecayProdH;
  TH1F* tauProdGammaOnlyH;
  TH1F* tauDecayProdNoGamH;
  TH2I* tauDecayProd2DH;
  TH1F* tauDecayProdMlt1H;
  TH1F* tauDecayProdMgt1H;

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
  TH1F* nTightTauH;
  TH1F* nLooseTauH;
  TH1F* statH;

  TH1F* selIndexTauH;

  TH1D* dRTauAndHLTH;

  TProfile* jetPtWithEtaP;
  TProfile* trkPtWithEtaP;

  TH1F* fTauDecayModeH; 
  TH1F* sTauDecayModeH; 

  TH1D* tauDisc2H;
  TH1D* tauDisc1H;

  TH1D* leadingTrackPt1H;
  TH1D* leadingTrackPt2H;

  TH1D* ltPtFrac1H;
  TH1D* ltPtFrac2H;

  TH1D* tauEta1H;
  TH1D* tauEta2H;

  TH1D* tauMass1H;
  TH1D* tauMass2H;

  TH1D* ltSipt1H;
  TH1D* ltSipt2H;

  TH1D* siptSumH;
  TH1D* siptSum2H;

  TH1D* maxEtTauH;
  TH1D* minEtTauH;

  int _nStat[13];
  int _nEvtCharge[3];
};
#endif
