#include <memory>
#include <iostream>
#include <typeinfo>
#include <iomanip>
#include <cstdlib>
#include <climits>
#include <cmath>
#include <algorithm>

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "ElectroWeakAnalysis/ZTauTau_DoubleTauJet/interface/EWKTauAnalyser.h"

#include "DataFormats/TauReco/interface/PFTauDiscriminatorByIsolation.h"
#include "DataFormats/BTauReco/interface/IsolatedTauTagInfo.h"

// MET
#include "DataFormats/METReco/interface/CaloMET.h"
#include "JetMETCorrections/Type1MET/interface/Type1METAlgo.h"

// muons
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

// vertices + tracks
#include "DataFormats/VertexReco/interface/Vertex.h"

// electron
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectronFwd.h"
// Invariant Mass from Caloremeter
#include "DataFormats/BTauReco/interface/TauMassTagInfo.h"
#include "DataFormats/BTauReco/interface/TauImpactParameterInfo.h"

#include "DataFormats/JetReco/interface/PFJet.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"

#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleCandidate.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"

#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"

#include "Math/GenVector/VectorUtil.h"
#include "Math/GenVector/PxPyPzE4D.h"

#include "DataFormats/L1Trigger/interface/L1JetParticle.h"
#include "DataFormats/L1Trigger/interface/L1JetParticleFwd.h"

using namespace edm;
using namespace reco; 
using namespace std;

#define NEL(x) (sizeof((x))/sizeof((x)[0]))

//
//  Constructor
//
EWKTauAnalyser::EWKTauAnalyser(const ParameterSet& iConfig) :
  _debugFlg(iConfig.getUntrackedParameter<int>("DebugFlg", 0)),
  _PFTaus(iConfig.getUntrackedParameter<string>("PFTaus","tauPFProducer")),
  _TauDiscriminatorByIsolationProducer(iConfig.getUntrackedParameter<string>("TauDiscriminatorByIsolationProducer", "discriminationByIsolation")),
  _jetTagSrc(iConfig.getUntrackedParameter<string>("JetTagProd","coneIsolationTauJetTags")),
  _metSrc(iConfig.getUntrackedParameter<string>("metSrc", "met")),
  _genJetSrc(iConfig.getUntrackedParameter<string>("genJetSrc", "iterativeCone5GenJets")),
  _caloJetSrc(iConfig.getUntrackedParameter<string>("caloJetSrc", "iterativeCone5CaloJets")),
  _electronSrc(iConfig.getUntrackedParameter<string>("electronSrc", "pixelMatchGsfElectrons")),
  _muonSrc(iConfig.getUntrackedParameter<string>("muonSrc", "muons")),
  _trackSrc(iConfig.getUntrackedParameter<string>("trackSrc", "ctfWithMaterialTracks")),
  _vertexSrc(iConfig.getUntrackedParameter<string>("vertexSrc", "offlinePrimaryVerticesFromCTFTracks")),
  _hltJetSrc(iConfig.getUntrackedParameter<string>("hltJetSrc", "isolatedL25PixelTau")),
  _genParticles(iConfig.getUntrackedParameter<string>("genParticles", "source")),
  _caloInvMassSrc(iConfig.getUntrackedParameter<string>("caloMassTagSrc")),
  _ipSrc(iConfig.getUntrackedParameter<string>("ipSrc", "impactParameter")),
  _maxTauMass(iConfig.getUntrackedParameter<double>("maxTauMass", 5.0)),
  _tightTauLeadingTrkPt(iConfig.getUntrackedParameter<double>("tightTauLeadingTrkPt", 12.0)),
  _looseTauLeadingTrkPt(iConfig.getUntrackedParameter<double>("looseTauLeadingTrkPt", 10.0)),
  _leadingTrkPtFrac(iConfig.getUntrackedParameter<double>("LeadingTrkPtFrac", 0.45)),
  _minZMass(iConfig.getUntrackedParameter<double>("minZMass",  30.0)),
  _maxZMass(iConfig.getUntrackedParameter<double>("maxZMass", 130.0)),
  _maxExtraJets(iConfig.getUntrackedParameter<int>("maxExtraJets", 1)),
  _minIPSignificanceSum(iConfig.getUntrackedParameter<double>("minIPSignificanceSum", 0.5)),
  _minJetEt(iConfig.getUntrackedParameter<double>("minJetEt", 15)),
  _maxHadTowerEnergy(iConfig.getUntrackedParameter<double>("maxHadTowerEnergy", 2.5)),
  _jetCorrector(iConfig.getUntrackedParameter<string>("jetCorrector", "MCJetCorrectorIcone5")),
  _jetEtMinMETCorr(iConfig.getUntrackedParameter<double>("jetEtMinMETCorr", 20.0)),
  _jetEMfracMETCorr(iConfig.getUntrackedParameter<double>("jetEMfracMETCorr", 0.9)),
  _matchingCone(iConfig.getUntrackedParameter<double>("MatchingCone", 0.1)),
  _signalCone(iConfig.getUntrackedParameter<double>("SignalCone", 0.07)),
  _isolationCone(iConfig.getUntrackedParameter<double>("IsolationCone", 0.4)),
  _leadingTrackPt(iConfig.getUntrackedParameter<double>("LeadingTrackPt", 6.0)),
  _minimumTrackPt(iConfig.getUntrackedParameter<double>("MinimumTrackPt", 1.0)),
  _clusterTrackMatchingCone(iConfig.getUntrackedParameter<double>("ClusterTrackMatchingCone", 0.08)),
  _statFile(iConfig.getUntrackedParameter<string>("statFile", "./stat.log")),
  _dataType(iConfig.getUntrackedParameter<string>("dataType", "qcd")),
  _analysisType(iConfig.getUntrackedParameter<string>("analysisType", "standard")),
  _selbitStr(iConfig.getUntrackedParameter<string>("selBits", "011011111111")),
  _eventCharge(iConfig.getUntrackedParameter<double>("eventCharge", -1.0))
{
    //  0 - offline discriminator (isolation) on each candidate
    //  1 - nJets
    //  2 - maxHadTowerEt - do should be ON
    //  3 - charge        - do
    //  4 - odd number of prongs - do
    //  5 - tau inv. mass        - do
    //  6 - leading track pt and pt fraction - do
    //  7 - the two candidates should have opposite charges
    //  8 - retain events only with 1 and 1/3 prong combination
    //  9 - apply sum of square of 2d IP significance  
    // 10 - visible Z mass  
    // 11 - retain OS/SS events only at an early stage  
    _selectionBits = strtol(_selbitStr.c_str(), NULL, 2);
}
//
// -- Begin Job
//
void EWKTauAnalyser::beginJob(const edm::EventSetup& iSetup) {
  nEvent = 0;
    edm::Service<TFileService> fs;

      nJetsH = fs->make<TH1F>("nJets"," Number of candidate Jets E_T > 15 GeV", 10, -0.5, 9.5);
   nTauCandH = fs->make<TH1F>("nTauCand"," Number of Tau Candidate Jets", 10, -0.5, 9.5);
      tauPtH = fs->make<TH1D>("tauPt", "Tau Pt", 100, 0.0, 100.0);
     tauEtaH = fs->make<TH1D>("tauEta", "Tau Eta", 100, -3.0, 3.0);
     tauPhiH = fs->make<TH1D>("tauPhi", "Tau Phi", 100, 0, 6.28);
    tauZVtxH = fs->make<TH1D>("tauZVtx", "Tau Z-Vertex", 100, -50.0, 50.0);
  tauChargeH = fs->make<TH1D>("tauCharge", "Tau Charge", 5, -0.5, 4.5);
   tauCh1PrH = fs->make<TH1D>("tauCh1Pr", "Tau Charge for 1-Prong Taus", 5, -0.5, 4.5);
   tauCh3PrH = fs->make<TH1D>("tauCh3Pr", "Tau Charge for 3-Prong Taus", 5, -0.5, 4.5);
    tauDiscH = fs->make<TH1D>("tauDisc", "Tau Discriminator", 2, -0.5, 1.5);

  tauDiscFirst2JetsH = fs->make<TH1D>("tauDiscFirst2Jets", "Tau Discriminator for First Two Jets", 2, -0.5, 1.5);
  leadingTrackSiptH = fs->make<TH1D>("LeadingTrkSipt", "Leading Track Signed IP Significance", 100, 0.0, 20.0);
  ltIPSig1PH = fs->make<TH1D>("LeadingTrkSipt1P", "Leading Track Signed IP Significance (1-prong)", 100, 0.0, 20.0);
  ltIPSig3PH = fs->make<TH1D>("LeadingTrkSipt3P", "Leading Track Signed IP Significance (3-prong)", 100, 0.0, 20.0);
  leadingTrackPtH = fs->make<TH1D>("LeadingTrkPt", "Leading Track Pt", 100, 0.0, 100.0);
   invMassH = fs->make<TH1D>("InvMass", "Invariant Mass", 100, 0.0, 10.0);
  invMass2H = fs->make<TH1D>("InvMass2", "Invariant Mass (Signal Components)", 100, 0.0, 10.0);
  invMass3H = fs->make<TH1D>("InvMass3", "Invariant Mass (custom I )", 100, 0.0, 10.0);
  invMass4H = fs->make<TH1D>("InvMass4", "Invariant Mass (custom II )", 100, 0.0, 10.0);
  invMass5H = fs->make<TH1D>("InvMass5", "Invariant Mass (custom III )", 100, 0.0, 10.0);
  maxHcalEnergyH = fs->make<TH1D>("MaxHcalEnergy", "Maximum HCAL Energy", 100, 0.0, 10.0);
   emEnergyFracH = fs->make<TH1D>("EMFracEnergy", "Fractional EM Energy", 100, 0.0, 1.0);

         eOverPH = fs->make<TH1D>("eOverP", "E over P of the candidate", 100, 0.0, 1.0);
       etOverPtH = fs->make<TH1D>("etOverPt", "eT over pT of the candidate", 100, 0.0, 1.0);
      hmaxOverPH = fs->make<TH1D>("hmaxOverP", "Max Hcal E over P of the candidate", 100, 0.0, 1.0);
      htotOverPH = fs->make<TH1D>("htotOverP", "Total Hcal E over P of the candidate", 100, 0.0, 1.0);

     nChHadronH = fs->make<TH1F>("nChHadTotal"," Number of Charged Hadrons (Total)", 20, -0.5, 19.5);
  nChHadronSigH = fs->make<TH1F>("nChHadInSigCone"," Number of Charged Hadrons in Signal Cone", 20, -0.5, 19.5);
  nChHadronIsoH = fs->make<TH1F>("nChHadInIsoCone"," Number of Charged Hadrons in Isolation Cone", 20, -0.5, 19.5);
      nGammaH = fs->make<TH1F>("nGammaTotal"," Number of Photons (Total)", 20, -0.5, 19.5); 
   nGammaSigH = fs->make<TH1F>("nGammaSigCone"," Number of Photons in Signal Cone", 20, -0.5, 19.5);
   nGammaIsoH = fs->make<TH1F>("nGammaIsoCone"," Number of Photons in Isolation Cone", 20, -0.5, 19.5);
  nNeuHadronH = fs->make<TH1F>("nNeuHadronTotal"," Number of Neutral Hadrons (Total)", 20, -0.5, 19.5);
  nNeuHadronSigH = fs->make<TH1F>("nNeuHadronSigCone"," Number of Neutral Hadrons in Signal Cone", 20, -0.5, 19.5);
  nNeuHadronIsoH = fs->make<TH1F>("nNeuHadronIsoCone"," Number of Neutral Hadrons in Isolation Cone", 20, -0.5, 19.5);
  sumPtChHadIsoH = fs->make<TH1D>("sumPtChHadIso", "Sum Pt of Ch. hadrons in Isolation Cone", 100, 0.0, 30.0);
  sumPtGammaIsoH = fs->make<TH1D>("sumPtGammaIso", "Sum Et of Photons in Isolation Cone", 100, 0.0, 30.0);

   chHadEnergyJetH = fs->make<TH1D>("chHadEnergyPFJet", "Charged Hadron Energy in PFJet", 100, 0.0, 100);
  neuHadEnergyJetH = fs->make<TH1D>("neuHadEnergyPFJet", "Neutral Hadron Energy in PF Jet", 100, 0.0, 100.0);
    chEMEnergyJetH = fs->make<TH1D>("chEMEnergyPFJet", "Charged EM Energy in PF Jet", 100, 0.0, 100.0);
    chMuEnergyJetH = fs->make<TH1D>("chMuEnergyPFJet", "Charged Muon Energy in PF Jet", 100, 0.0, 100.0);
   neuEMEnergyJetH = fs->make<TH1D>("neuEMEnergyPFJet", "Neutral EM Energy in PF Jet", 100, 0.0, 100.0);
        chMultJetH = fs->make<TH1F>("chMultPFJet", "Charged Multiplicity in PF Jet", 20, -0.5, 19.5);
       neuMultJetH = fs->make<TH1F>("neuMulPFtJet", "Neutral Multiplicity in PF Jet", 20, -0.5, 19.5);
        muMultJetH = fs->make<TH1F>("muMultPFJet", "Muon Multiplicity in PF Jet", 20, -0.5, 19.5);

  zMassH = fs->make<TH1D>("zmass", "Z mass", 100, 30., 130.0);
   zEtaH = fs->make<TH1D>("zeta",  "Z Eta",  100, -3.0, 3.0);
   zPhiH = fs->make<TH1D>("zphi",  "Z Phi", 100, 0., 6.28);

    deltaRH = fs->make<TH1D>("deltaR",  "delta R between two jets", 100, 0., 6.28);
  deltaPhiH = fs->make<TH1D>("deltaPhi","delta Phi between two jets", 100, 0., 6.28);
  cosThetaH = fs->make<TH1D>("cosTheta","Cos Theta between two jets", 100, 0., 1);

  tauDiscOrigH = fs->make<TH1D>("tauDiscOrig", "Tau Discriminator from IsolatedTauTagInfo", 2, -0.5, 1.5);
     metH = fs->make<TH1D>("met", "Missing eT", 100, 0.0, 100.);
   sumEtH = fs->make<TH1D>("sumEt", "Sum eT", 100, 0.0, 300.);
  metSigH = fs->make<TH1D>("metSig", "met Significance", 100, 0.0, 10.);

   caloInvMassH = fs->make<TH1D>("CaloInvariantMass","Invariant Mass from Caloremeter",100, 0, 10.0);
  tauDecayModeH = fs->make<TH1F>("tauDecayMode"," Tau Decay Mode", 3, -0.5, 2.5);

     ip2DH = fs->make<TH1D>("ip2D", "2D Impact parameter as Tau Tag(value)", 100, 0, 0.3);
  ip2DSigH = fs->make<TH1D>("ip2DSig", "2D Impact parameter as Tau Tag(significance)", 100, 0, 20);
     ip3DH = fs->make<TH1D>("ip3D", "3D Impact parameter as Tau Tag(value)", 100, 0, 100);
  ip3DSigH = fs->make<TH1D>("ip3DSig", "3D Impact parameter as Tau Tag(significance)", 100, 0.0, 3000);

   ltPtFracH = fs->make<TH1D>("ltPtFrac", "Leading Trk Pt wrt Jet Pt", 100, 0.0, 1.0);
  ltvsJetPtH = fs->make<TH2D>("ltvsJetPt","Leading Trk vs Jet Pt", 100, 0.0, 100.0, 100, 0.0, 100.0);

  nTightTauH = fs->make<TH1F>("nTightTau"," Number of Tight Taus in the event", 3, -0.5, 2.5);
  nLooseTauH = fs->make<TH1F>("nLooseTau"," Number of Loose Taus in the event", 3, -0.5, 2.5);
  statH = fs->make<TH1F>("statH"," Event Statistics", 12, 0, 12);

  jetPtWithEtaP = fs->make<TProfile>("jetPtWithEta","Profile of Jet Pt versus Jet Eta",100,-3,3,0,100);
  trkPtWithEtaP = fs->make<TProfile>("trkPtWithEtaf","Profile of Leading Tracl Pt versus Jet Eta",100,-3,3,0,100);
            
  dRTauAndHLTH = fs->make<TH1D>("dRTauAndHLT", "dR of Tau wrt nearest HLT Jet", 100, 0.0, 10.0);
  selIndexTauH = fs->make<TH1F>("selBitPattarnTau", "Selection Bit Pattern of Tau", 11, -0.5, 10.5);
	
  // Do not touch the exisitng histograms, but book the some of the above again
  // this time separately for the two taus in question; since we always match with th
  // HLT jets before moving on, we'll get the correct entries most of the time
  
  // Discriminator
  tauDisc1H = fs->make<TH1D>("tauDisc1", "Tau Discriminator of first Jet", 2, -0.5, 1.5);
  tauDisc2H = fs->make<TH1D>("tauDisc2", "Tau Discriminator of second Jet", 2, -0.5, 1.5);

  // Leading track Pt and ratio of LT Pt to Jet Pt
  leadingTrackPt1H = fs->make<TH1D>("leadingTrackPt1", "Leading Trk Pt of first tau", 100, 0.0, 100.0);
  leadingTrackPt2H = fs->make<TH1D>("leadingTrackPt2", "Leading Trk Pt of second tau", 100, 0.0, 100.0);
  ltPtFrac1H = fs->make<TH1D>("ltPtFrac1", "Leading Trk Pt of first tau wrt Jet Pt", 100, 0.0, 1.0);
  ltPtFrac2H = fs->make<TH1D>("ltPtFrac2", "Leading Trk Pt of second tau wrt Jet Pt", 100, 0.0, 1.0);

  // Tau pseudorapidity
  tauEta1H = fs->make<TH1D>("tauEta1", "Tau Eta first jet", 100, -3.0, 3.0);
  tauEta2H = fs->make<TH1D>("tauEta2", "Tau Eta second jet", 100, -3.0, 3.0);

  // Invariant Mass
  tauMass1H = fs->make<TH1D>("tauMass1", "Tau Invariant Mass (Signal Components, first jet)", 100, 0.0, 10.0);
  tauMass2H = fs->make<TH1D>("tauMass2", "Tau Invariant Mass (Signal Components, second jet)", 100, 0.0, 10.0);

  // Impact parameter Significance of the L Track 
  ltSipt1H = fs->make<TH1D>("LeadingTrkSipt1", "Leading Track Signed IP Significance (first jet)", 100, 0.0, 20.0);
  ltSipt2H = fs->make<TH1D>("LeadingTrkSipt2", "Leading Track Signed IP Significance (second jet)", 100, 0.0, 20.0);
   
  siptSumH = fs->make<TH1D>("siptSum", "Leading Track Signed IP Significance Sum", 100, 0.0, 10.0);
  siptSum2H = fs->make<TH1D>("siptSum2", "Leading Track Signed IP Significance Sum", 100, 0.0, 2.0);

  maxEtTauH = fs->make<TH1D>("maxEtTau", "Tau candidate with maxEt", 100, 0.0, 100.0);
  minEtTauH = fs->make<TH1D>("minEtTau", "Tau candidate with minEt", 100, 0.0, 100.0);

  if (_dataType == "signal") {
       tauPtGenH = fs->make<TH1D>("tauPtGen", "Generated Tau Pt", 100, 0.0, 100.0);
      tauEtaGenH = fs->make<TH1D>("tauEtaGen", "Generated Tau Eta", 100, -3.0, 3.0);
      tauPtDiscH = fs->make<TH1D>("tauPtDisc", "Generated Tau Pt  Dicriminated", 100, 0.0, 100.0);
     tauEtaDiscH = fs->make<TH1D>("tauEtaDisc", "Generated Tau Eta Dicriminated", 100, -3.0, 3.0);
     invMassGenH = fs->make<TH1D>("InvMassGen", "Invariant Mass from Generator Level", 100, 0.0, 10.0);
      invMassMCH = fs->make<TH1D>("InvMassMC", "Invariant Mass from HepMC (all)", 100, 0.0, 10.0);
     invMassMC1H = fs->make<TH1D>("InvMassMC1", "Invariant Mass from HepMC (1 prong - neutrino - gamma)", 100, 0.0, 10.0);
     invMassMC2H = fs->make<TH1D>("InvMassMC2", "Invariant Mass from HepMC (1 prong + gamma - neutrino)", 100, 0.0, 10.0);
     invMassMC3H = fs->make<TH1D>("InvMassMC3", "Invariant Mass from HepMC (multi promg)", 100, 0.0, 10.0);
     maxHcalEnergy2H = fs->make<TH1D>("MaxHcalEnergy2", "Maximum HCAL Energy for non-hadronic tau decays ", 100, 0.0, 10.0);
      emEnergyFrac2H = fs->make<TH1D>("EMFracEnergy2", "Fractional EM Energy for non-hadronic Tau decays", 100, 0.0, 1.0);
     ptDiffH = fs->make<TH1D>("ptdiff", "pt(reco-gen/reco)", 100,-1.0, 1.0);
     ptScatH = fs->make<TH2D>("pscatt","Gen vs Reco Tau P", 100, 10.0, 110.0, 100, 10.0, 110.0);
          tauDecayProdH = fs->make<TH1F>("tauDecayProd", "# of Decay products (excluding neutrinos)", 21, -0.5, 20.5);
     tauDecayProdNoGamH = fs->make<TH1F>("tauDecayProdNoGamma", "# of Decay products (excluding neutrino & gamma)", 21, -0.5, 20.5);
        tauDecayProd2DH = fs->make<TH2I>("tauDecayProd2D", "# of non-gamma products vs #of gamma products", 21, -0.5, 20.5, 21, -0.5, 20.5);
      tauDecayProdMlt1H = fs->make<TH1F>("tauDecayProdMlt1", "# of Decay products (inv. mass < 1.0)", 21, -0.5, 20.5);
      tauDecayProdMgt1H = fs->make<TH1F>("tauDecayProdMgt1", "# of Decay products (inv. mass > 1.0)", 21, -0.5, 20.5);
      tauProdGammaOnlyH = fs->make<TH1F>("tauDecayGammaOnly", "# of Gammas from Tau", 21, -0.5, 20.5);

     // Decay mode of the 2 taus
     fTauDecayModeH = fs->make<TH1F>("FTauDecayMode", "Decay Mode of the first tau", 3, -0.5, 2.5);
     sTauDecayModeH = fs->make<TH1F>("STauDecayMode", "Decay Mode of the second tau", 3, -0.5, 2.5);
  } 

  // initialise Statistical variables 
  cout << "NEL(_nStat)=" << NEL(_nStat) << ", NEL(_nEvtCharge)=" << NEL(_nEvtCharge) << endl;
  for (unsigned int i = 0; i < NEL(_nStat); ++i) _nStat[i] = 0;
  for (unsigned int i = 0; i < NEL(_nEvtCharge); ++i) _nEvtCharge[i] = 0;
}
//
// -- Analyze
//
void EWKTauAnalyser::analyze(const Event& iEvent, const EventSetup& iSetup) {
  int run = iEvent.id().run();
  int evt = iEvent.id().event();
  _nStat[0]++;
  if (_debugFlg)
  cout << "==============================================" << endl
       << " Event: " << nEvent++ << endl
       << " Run # " << run << " Event # " << evt << endl
       << "==============================================" << endl;
  
  // Particle Flow based information
  Handle<PFTauCollection> tauHandle;
  iEvent.getByLabel(_PFTaus, tauHandle);
  const PFTauCollection& myTauCollection = *(tauHandle.product()); 

  Handle<CaloJetCollection> tauHLTHandle;
  iEvent.getByLabel(_hltJetSrc, tauHLTHandle);
  const CaloJetCollection& myHLTJetCollection = *(tauHLTHandle.product()); 

  Handle<PFTauDiscriminatorByIsolation> theTauDiscriminatorByIsolation;
  iEvent.getByLabel(_TauDiscriminatorByIsolationProducer, theTauDiscriminatorByIsolation);

  edm::Handle<reco::GenJetCollection> genJetsHandle;
  iEvent.getByLabel(_genJetSrc, genJetsHandle);
  const reco::GenJetCollection& genJets = *(genJetsHandle.product());

  int n_candidate = myTauCollection.size();
  if (_debugFlg) cout << "Found " << n_candidate << " had. tau-jet candidates" << endl;

  // Now the real stuff
  nTauCandH->Fill(n_candidate);
  if (!n_candidate) return;
  _nStat[1]++;

  if (_dataType == "signal") {
    // Find Tau Decay mode
    vector<int> tauDecayModes;
    findTauDecayMode(iEvent, iSetup, tauDecayModes);
  }

  // ----------------------------------------------------
  // Find Tau candidates that match with the trigger jets
  // ----------------------------------------------------
  int nDiscriminatedJet  = 0,
      nLeadingTrackInJet = 0;  
  vector<int> tauCandidateList;
  vector<int> extraJetList;
  for (PFTauCollection::size_type iTau = 0; iTau < tauHandle->size(); ++iTau) {
    PFTauRef theTau(tauHandle, iTau);
    if (!theTau) {
      cerr << "EWKTauAnalyser::analyze(): Strange! invalid PFTauRef for iTau=" 
           << iTau << endl;
      continue;
    }
    const PFTau& tau = (*theTau);

    // Avoid low Et candidates
    if (tau.et() < _minJetEt) continue;   // 15 GeV by default

    // Match with HLT jets that triggered the event
    double minDR = 99.9;
    bool matched = matchWithHLTJet(tau, myHLTJetCollection, minDR);
    if (_debugFlg > 3) cout << "EWKTauAnalyser::analyze(): iTau=" << iTau 
                            << ", dR(PFTauJet, nearest HLT Jet) = " << minDR << endl;
    dRTauAndHLTH->Fill(minDR);
    if (matched) {
      tauCandidateList.push_back(iTau);

      PFCandidateRef theLeadPFCand = tau.leadPFChargedHadrCand();
      if (!theLeadPFCand) {      
        cerr << "EWKTauAnalyser::analyze(): No Leading PFCand, iTau=" << iTau << endl;
        continue;
      }
      nLeadingTrackInJet++;

      double disc = (*theTauDiscriminatorByIsolation)[theTau];
      if (disc != 1.0) continue;
      nDiscriminatedJet++;
    }
    else {
      extraJetList.push_back(iTau);
    }
  }
  // Retain events only with two PFJets each matched with HLT 
  if (tauCandidateList.size() != 2) return;
  _nStat[2]++;

  // Each candidate should have an associated leading track 
  if (nLeadingTrackInJet < 2) return; 
  _nStat[3]++;

  // Each should pass offline isolation
  if ( applySelection(_selectionBits, 0) && (nDiscriminatedJet < 2) ) return;
  _nStat[4]++;

  // Retain events with only n extra jets (with eT > 15 GeV)
  int nExtraJets = extraJetList.size();
  nJetsH->Fill(nExtraJets);
  if ( applySelection(_selectionBits, 1) && (nExtraJets > _maxExtraJets) ) return;
  _nStat[5]++;
  
  // Now look at the two jet properties 
  PFTau taua, taub;
  TauObject(tauHandle, tauCandidateList[0], taua); 
  TauObject(tauHandle, tauCandidateList[1], taub); 

  // Sort the candidate list in decreasing eT
  if (taua.et() < taub.et()) {
    reverse(tauCandidateList.begin(), tauCandidateList.end());
    TauObject(tauHandle, tauCandidateList[0], taua); 
    TauObject(tauHandle, tauCandidateList[1], taub); 
  }
  maxEtTauH->Fill(taua.et());
  minEtTauH->Fill(taub.et());

  // Optionally, retain OS/SS events only 
  double tch =  getTauCharge(taua) * getTauCharge(taub);
  if (isnan(tch) || isinf(tch)) tch = 0.0; // why??

  int chIndx = 2;
  if (tch == 0.0) chIndx = 0;
  else if (tch > 0.0) chIndx = 1;
  _nEvtCharge[chIndx]++;
  if (_debugFlg > 3) cout << "ch1*ch2 = " << tch << ", chIndx = " << chIndx << endl;
  if ( applySelection(_selectionBits, 11) && (tch != _eventCharge) ) return;

  // Fill a few histograms before any further selection 
  double siptSum = sumOfIPSignificance(taua, taub);
  siptSumH->Fill(siptSum);

  const math::XYZTLorentzVector& Pa = taua.p4();
  const math::XYZTLorentzVector& Pb = taub.p4();
  math::XYZTLorentzVector Pab = Pa + Pb;
  
  math::XYZVector veca(Pa.x(), Pa.y(), Pa.z());    
  math::XYZVector vecb(Pb.x(), Pb.y(), Pb.z());    
  double deltaR = ROOT::Math::VectorUtil::DeltaR(veca, vecb);
  deltaRH->Fill(deltaR);
  
  double deltaPhi = ROOT::Math::VectorUtil::DeltaPhi(veca, vecb);
  deltaPhiH->Fill(deltaPhi);
  
  double cosTheta = ROOT::Math::VectorUtil::CosTheta(veca, vecb);
  cosThetaH->Fill(cosTheta);

  // ---------------------
  // Begin tau selection
  // ---------------------
  map<unsigned int, unsigned int> selectMap;
  int nTightTau = 0,
      nLooseTau = 0;
  for (unsigned int i = 0; i < tauCandidateList.size(); ++i)  {
    int iTau = tauCandidateList[i];
    PFTauRef theTau(tauHandle, iTau);
    const PFTau& tau = (*theTau);

    PFCandidateRef theLeadPFCand = tau.leadPFChargedHadrCand();
    if (!theLeadPFCand) {      
      cerr << "EWKTauAnalyser::analyze(): No Leading PFCand, iTau = " << iTau << endl;
      continue;
    }

    double disc = (*theTauDiscriminatorByIsolation)[theTau];
    if (_debugFlg > 3) cout << "DiscriminatorByIsolation = " << disc << endl;
    tauDiscH->Fill(disc);
    tauDiscFirst2JetsH->Fill(disc);

    // Temporary
    if (i == 0) 
      tauDisc1H->Fill(disc);
    else
      tauDisc2H->Fill(disc);

    if ( applySelection(_selectionBits, 0) && (disc != 1.0) ) continue;

    double leading_trk_pt       = (*theLeadPFCand).pt();
    double zvertex              = tau.vz();      // What is this quantity?
    double charge               = getTauCharge(tau);
    double max_had_tower_energy = tau.maximumHCALPFClusterEt();

    const PFTauTagInfo& tagInfo = (*(tau.pfTauTagInfoRef()));
    double leading_trk_sipt     = TMath::Abs(tau.leadPFChargedHadrCandsignedSipt());

    double inv_mass             = tau.p4().M();
    double inv_mass2            = tau.alternatLorentzVect().M();

    int ch_hadrons              = tagInfo.PFChargedHadrCands().size();
    int neu_hadrons             = tagInfo.PFNeutrHadrCands().size();
    int n_photons               = tagInfo.PFGammaCands().size();

    int sig_ch_hadrons          = tau.signalPFChargedHadrCands().size();
    int sig_photons             = tau.signalPFGammaCands().size();
    int sig_neu_hadrons         = tau.signalPFNeutrHadrCands().size();

    int iso_ch_hadrons          = tau.isolationPFChargedHadrCands().size();
    int iso_photons             = tau.isolationPFGammaCands().size();
    int iso_neu_hadrons         = tau.isolationPFNeutrHadrCands().size();

    double sumpt_iso_ch_hadrons = tau.isolationPFChargedHadrCandsPtSum();
    double sumpt_iso_photons    = tau.isolationPFGammaCandsEtSum();

    const PFJet& pfJet          = (*(tagInfo.pfjetRef()));
    double em_energy_frac       = pfJet.chargedEmEnergyFraction() + pfJet.neutralEmEnergyFraction();

    double chHadEnergyJet       = pfJet.chargedHadronEnergy();
    double neuHadEnergyJet      = pfJet.neutralHadronEnergy();
    double ehad_tot             = chHadEnergyJet + neuHadEnergyJet;
    double chEMEnergyJet        = pfJet.chargedEmEnergy();
    double chMuEnergyJet        = pfJet.chargedMuEnergy();
    double neuEMEnergyJet       = pfJet.neutralEmEnergy();
    float  chMultJet            = pfJet.chargedMultiplicity();
    float  neuMultJet           = pfJet.neutralMultiplicity();
    float  muMultJet            = pfJet.muonMultiplicity();

    double pt = tau.pt();
    if (_debugFlg > 3)
      cout << "                                 iTau: " << iTau << endl
           << "                        Pt of the Tau: " << pt << endl
           << "                       Lead PFCand pt: " << leading_trk_pt       << endl
           << "             InvariantMass of the Tau: " << inv_mass             << endl
           << "                    Vertex of the Tau: " << zvertex              << endl
           << "                    Charge of the Tau: " << charge               << endl
           << "                   Em energy fraction: " << em_energy_frac       << endl
           << "                    Max Hadron energy: " << max_had_tower_energy << endl
           << "          # of PF charged hadr. cands: " << ch_hadrons           << endl
           << "          # of PF neutral hadr. cands: " << neu_hadrons          << endl
           << "                  # of PF gamma cands: " << n_photons            << endl
           << "          # of SignalPFchargedHadrons: " << sig_ch_hadrons       << endl
           << "       # of IsolationPFChargedHadrons: " << iso_ch_hadrons       << endl
           << "          # of SignalPFGammaCandidate: " << sig_photons          << endl
           << "       # of IsolationPFGammaCandidate: " << iso_photons          << endl
           << "  Sum pT of Isolation Charged Hadrons: " << sumpt_iso_ch_hadrons << endl
           << "Sum E_T of Isolation Gamma Candidates: " << sumpt_iso_photons    << endl
           << "                     PF Jet EM Energy: " << chEMEnergyJet+neuEMEnergyJet << endl;

    invMassH->Fill(inv_mass);
    leadingTrackPtH->Fill(leading_trk_pt);

    // leading_trk_pt wrt jet pt
    double eta = tau.eta();
    double ratio = leading_trk_pt/pt;
    ltPtFracH->Fill(ratio);
    ltvsJetPtH->Fill(pt, leading_trk_pt);
    jetPtWithEtaP->Fill(eta, pt, 1);
    trkPtWithEtaP->Fill(eta, leading_trk_pt, 1);
 
    // Fill Histograms
    tauPtH->Fill(pt);
    tauEtaH->Fill(eta);
    tauPhiH->Fill(ROOT::Math::VectorUtil::Phi_0_2pi(tau.phi()));
    tauZVtxH->Fill(zvertex);
    tauChargeH->Fill(TMath::Abs(charge));
    if (sig_ch_hadrons == 1) tauCh1PrH->Fill(TMath::Abs(charge));
    if (sig_ch_hadrons == 3) tauCh3PrH->Fill(TMath::Abs(charge));
    leadingTrackSiptH->Fill(leading_trk_sipt);
    if (sig_ch_hadrons == 1) {
      ltIPSig1PH->Fill(leading_trk_sipt);
    }
    else { // multi prong
      ltIPSig3PH->Fill(leading_trk_sipt);
    }

    invMass2H->Fill(inv_mass2);

    double p = tau.p();
    eOverPH->Fill(tau.energy()/p);
    hmaxOverPH->Fill(max_had_tower_energy/p);
    htotOverPH->Fill(ehad_tot/p);
    etOverPtH->Fill(tau.et()/pt);

    maxHcalEnergyH->Fill(max_had_tower_energy);
    emEnergyFracH->Fill(em_energy_frac);
    nChHadronH->Fill(ch_hadrons);
    nChHadronSigH->Fill(sig_ch_hadrons);
    nChHadronIsoH->Fill(iso_ch_hadrons);
    nGammaH->Fill(n_photons);
    nGammaSigH->Fill(sig_photons);
    nGammaIsoH->Fill(iso_photons);
    nNeuHadronH->Fill(neu_hadrons);
    nNeuHadronSigH->Fill(sig_neu_hadrons);
    nNeuHadronIsoH->Fill(iso_neu_hadrons);
    sumPtChHadIsoH->Fill(sumpt_iso_ch_hadrons);
    sumPtGammaIsoH->Fill(sumpt_iso_photons);

    chHadEnergyJetH->Fill(chHadEnergyJet);
    neuHadEnergyJetH->Fill(neuHadEnergyJet);
    chEMEnergyJetH->Fill(chEMEnergyJet);
    chMuEnergyJetH->Fill(chMuEnergyJet);
    neuEMEnergyJetH->Fill(neuEMEnergyJet);
    chMultJetH->Fill(chMultJet);
    neuMultJetH->Fill(neuMultJet);
    muMultJetH->Fill(muMultJet);

    // Temporary
    if (i == 0) {
      leadingTrackPt1H->Fill(leading_trk_pt);
      ltPtFrac1H->Fill(ratio);
      ltSipt1H->Fill(leading_trk_sipt);
      tauMass1H->Fill(inv_mass2);
      tauEta1H->Fill(eta);
    }
    else {
      leadingTrackPt2H->Fill(leading_trk_pt);
      ltPtFrac2H->Fill(ratio);
      ltSipt2H->Fill(leading_trk_sipt);
      tauMass2H->Fill(inv_mass2);
      tauEta2H->Fill(eta);
    }

    // Calculate and fill custom invariant masses
    findInvariantMass(tau);

    // Now apply further selection
    int iSelect = selectTau(tau);
    if (iSelect < 2) {
      nLooseTau++;
      if (iSelect == 0) nTightTau++;
    }
    selectMap.insert(pair<unsigned int, unsigned int>(iTau, iSelect));
    if (_debugFlg > 3) {
      cout << "                       Selection bits: ";
      EWKTauAnalyser::bit_print(iSelect);
      cout << endl;
    }
 
    for (unsigned int k = 0; k < 11; ++k)
      if (iSelect & (1 << k)) selIndexTauH->Fill(k);

    if (_dataType == "signal") {
      vector<math::XYZTLorentzVector> genTauList;
      // Match with the generator level tau
      math::XYZTLorentzVector genJetP4;
      double dr = 999.0;
      int decayMode = -1;
      if (getGenJet(tau, genJets, genJetP4, decayMode, dr)) {
        double genPt  = genJetP4.Pt();
        double genEta = genJetP4.eta();
        if (_debugFlg > 3) 
        cout << " ==    tauJet Pt = " << tau.pt()      
             << ", eta = " << tau.eta() << ", phi = " << tau.phi() << endl
             << " == genTauJet Pt = " << genPt << ", eta = " << genEta 
             << ", phi = " << genJetP4.phi()
             << " dr = " << dr << ", decayMode = " << decayMode
             << endl;
        genTauList.push_back(genJetP4);

        double d = (pt > 0) ? (pt - genPt)/pt : -1;
        ptDiffH->Fill(d);
        ptScatH->Fill(genPt, pt);
        tauDecayModeH->Fill(decayMode);
        if (decayMode < 2) {
          maxHcalEnergy2H->Fill(max_had_tower_energy);
          emEnergyFrac2H->Fill(em_energy_frac);
        }
        else { 
          invMassGenH->Fill(genJetP4.M());
        }

        // plot GenPt and GenEta for all and discriminated Taus
        tauPtGenH->Fill(genPt);
        tauEtaGenH->Fill(genEta);
        if (disc > 0.0) {
          tauPtDiscH->Fill(genPt);
          tauEtaDiscH->Fill(genEta);
        }
      }
    }
  }
  nTightTauH->Fill(nTightTau);
  nLooseTauH->Fill(nLooseTau);

  if (nTightTau < 1) return;
  _nStat[6]++;

  if (_debugFlg) {
    printEvent(taua, taub);
    // Let's see which bits are quite
    cout << "Tau Selection bits: " << endl;
    cout << "0 - tightTauLeadingTrkPt, 1 - looseTauLeadingTrkPt, 2 - leadingTrkPtFrac" << endl 
         << "3 - tauMass, 4 - # prongs, 5 - charge, 6 - maxHadTowerEnergy" << endl;
    for (map<unsigned int, unsigned int>::iterator it = selectMap.begin();
                                                  it != selectMap.end(); ++it) 
    {
      int value = it->second;
      cout << "value = <" << value << ">, bits: "; 
      EWKTauAnalyser::bit_print(it->second); 
    }
  }
  // At this point call all other routines
  //findIsolatedTaus(iEvent, iSetup);
  //findMET(iEvent, iSetup);
  //findMuons(iEvent, iSetup);
  //findElectrons(iEvent, iSetup);
  //findInvMassFromCalo(iEvent, iSetup);
  //findTauImpactParameter(iEvent, iSetup);

  if (nLooseTau < 2) return;
  _nStat[7]++;
  
  // Invariant mass of the tau pair
  // Note that, as at this point we have only two candidates we could as well use
  // the previous taua and taub variables  
  pair<unsigned int, unsigned int> tauPairIndex = getTauIndices(selectMap);
  if (_debugFlg > 3) cout << " Tau indices = " << tauPairIndex.first << ", " << tauPairIndex.second << endl;

  PFTauRef theTau1(tauHandle, tauPairIndex.first);
  PFTauRef theTau2(tauHandle, tauPairIndex.second);
  const PFTau& tau1 = (*theTau1);
  const PFTau& tau2 = (*theTau2);

  if ( applySelection(_selectionBits, 7) ) {
    // Temporary cludge to implement background enriched sample
    double value = getTauCharge(tau1) * getTauCharge(tau2);
    cout << "== analysisType: <" << _analysisType << ">, Event Charge(ch1*ch2) = <" << value << ">" << endl;
    if (_analysisType == "QCD_Estimate") {
      if (value == -1) return;
    }
    else {
      if (value != -1) return;
    }
  }
  _nStat[8]++;

  if ( applySelection(_selectionBits, 8) && !prongsAsExpected(tau1, tau2) ) return;
  _nStat[9]++;

  siptSum = sumOfIPSignificance(tau1, tau2);
  siptSum2H->Fill(siptSum);
  if (_debugFlg) cout << "== Sum(IP Significance)**2 = <" << siptSum << ">" << endl;
  if ( applySelection(_selectionBits, 9) && (siptSum < _minIPSignificanceSum) ) return;
  _nStat[10]++;

  if (_debugFlg) cout << "== Tau Momenta: <" << tau1.pt() << ">, <" << tau2.pt() << "> GeV" << endl;
  const math::XYZTLorentzVector& P1 = tau1.p4();
  const math::XYZTLorentzVector& P2 = tau2.p4();
  math::XYZTLorentzVector ZP4 = P1 + P2;
  double mass = ZP4.M();
  if (_debugFlg) cout << "== Reconstructed Z Mass = <" << mass << "> GeV" << endl;
  zMassH->Fill(mass);
  zEtaH->Fill(ZP4.eta());    
  zPhiH->Fill(ROOT::Math::VectorUtil::Phi_0_2pi(ZP4.phi()));    
  
  // Angle of a tau with the Z etc. 
#if 0
  math::XYZVector vec1(P1.x(), P1.y(), P1.z());    
  math::XYZVector vec2(P2.x(), P2.y(), P2.z());    
  double deltaR = ROOT::Math::VectorUtil::DeltaR(vec1, vec2);
  deltaRH->Fill(deltaR);
  
  double deltaPhi = ROOT::Math::VectorUtil::DeltaPhi(vec1, vec2);
  deltaPhiH->Fill(deltaPhi);
  
  double cosTheta = ROOT::Math::VectorUtil::CosTheta(vec1, vec2);
  cosThetaH->Fill(cosTheta);
#endif
  if ( applySelection(_selectionBits, 10) && (mass < _minZMass || mass > _maxZMass) ) return;
  _nStat[11]++;

  // Find Tau Decay mode
  if (_dataType == "signal") {
    vector<int> tauDecayModes;
    tauDecayModes.clear();
    findTauDecayMode(iEvent, iSetup, tauDecayModes, false);
    fTauDecayModeH->Fill(tauDecayModes[0]);
    sTauDecayModeH->Fill(tauDecayModes[1]);

    // Final efficiency for taus decaying hadronically
    if ( (tauDecayModes[0] == 2) && (tauDecayModes[1] == 2) ) _nStat[12]++;
  }
}
void EWKTauAnalyser::findIsolatedTaus(const edm::Event& iEvent, const EventSetup& iSetup) 
{
  // Isolated Taus
  Handle<IsolatedTauTagInfoCollection> tauTagInfoHandle;
  iEvent.getByLabel(_jetTagSrc, tauTagInfoHandle);

  if (tauTagInfoHandle.isValid()) {
    const IsolatedTauTagInfoCollection& tauTagInfoColl = *(tauTagInfoHandle.product());
    if (_debugFlg > 3) cout << "Found " << tauTagInfoColl.size() << " Tau candidates" << endl;
      
    int ijet = 0;
    for (IsolatedTauTagInfoCollection::const_iterator iter  = tauTagInfoColl.begin(); 
                                                      iter != tauTagInfoColl.end(); 
                                                    ++iter) 
    {
      const Jet& jet = *(iter->jet());
      math::XYZVector jet3Vec(jet.momentum());
      double et  = jet.et();
      if (et < 10.0) continue;

      // Leading Signal Track 
      const reco::TrackRef lsTrack 
        = iter->leadingSignalTrack(_matchingCone, _leadingTrackPt);
      if ( !lsTrack.isNull() ) {
        cerr << "EWKTauAnalyser::findIsolatedTaus: leading Track:" 
                                   << " Pt = "    << lsTrack->pt() << ","
                                   << " theta = " << lsTrack->theta()*180/TMath::Pi() << ","
                                   << " phi = "   << lsTrack->phi()*180/TMath::Pi() 
                                   << endl;
      } 
      else {
        cerr << "EWKTauAnalyser::findIsolatedTaus: no leading Track found" << endl;
      }

      double eta = jet3Vec.eta();
      double phi = jet3Vec.phi();

      double discriminator = iter->discriminator(_matchingCone, _signalCone, _isolationCone, _leadingTrackPt, _minimumTrackPt);
      tauDiscOrigH->Fill(discriminator);

      if (_debugFlg > 3) 
      cout << "ijet = " << ijet++ 
           << ", eT = " << et  
           << ", eta = " << eta 
           << ", phi = " << phi 
           << ", discriminator = " << discriminator << endl;
    }
  }
  else {
    cerr << "ERROR. IsolatedTauTagInfoCollection missing!" << endl;
  }
}
void EWKTauAnalyser::findMET(const edm::Event& iEvent, const EventSetup& iSetup) 
{
  // MET
  Handle<CaloMETCollection> recoMetHandle;
  iEvent.getByLabel(_metSrc, recoMetHandle);
  if (recoMetHandle.isValid()) {
    const CaloMETCollection& recoMetColl = *(recoMetHandle.product());

    // Loop over MET objects
    vector<math::XYZTLorentzVector> metList;
    for (CaloMETCollection::const_iterator it  = recoMetColl.begin();
                                           it != recoMetColl.end();
                                         ++it)
    {
      const CaloMET& obj = (*it);
      double  met  = obj.et();
      double sumEt = obj.sumEt();
      double phi   = obj.phi();
      double mEtX  = obj.px();
      if (met < 5.0) continue;
      metList.push_back(obj.p4());
      if (_debugFlg > 3) cout << "MET: " << met << ", " << sumEt << ", " << phi << ", " << mEtX << endl;

      metH->Fill(met);
      sumEtH->Fill(sumEt);
      metSigH->Fill(obj.mEtSig());
    }

    //-- Jets
    Handle<CaloJetCollection> caloJetHandle; 
    iEvent.getByLabel(_caloJetSrc, caloJetHandle);
    const CaloJetCollection& caloJetColl = *( caloJetHandle.product() );
    if (_debugFlg >= 2) 
      cout << setiosflags(ios::left) << setw(30) 
           << " # jets:" << caloJetColl.size() 
           << endl;
#if 0
    const JetCorrector* corrector = JetCorrector::getJetCorrector(_jetCorrector, iSetup);
    for (CaloJetCollection::const_iterator ijet = caloJetColl.begin(); ijet != caloJetColl.end(); ++ijet )   {
      double corr = corrector->correction(*ijet);
    }
    

    // Corrected Met 
    // type1 correction
    Type1METAlgo fType1METAlgo;
    std::auto_ptr<reco::CaloMETCollection> type1MET( new reco::CaloMETCollection() );  //Create empty output
    fType1METAlgo.run( recoMetColl, *corrector, caloJetColl, jetEtMinMETCorr_, jetEMfracMETCorr_, &*type1MET ); 
               // Invoke the algorithm
    double tp1MET    = type1MET->at(0).pt();
    double tp1METPhi = type1MET->at(0).phi();
    double tp1SumET  = type1MET->at(0).sumEt();
#endif
  }
  else {
    cerr << "ERROR. CaloMETCollection missing!" << endl;
  }
}
void EWKTauAnalyser::findMuons(const edm::Event& iEvent, const EventSetup& iSetup) 
{
  Handle<MuonCollection> muonHandle;
  iEvent.getByLabel(_muonSrc, muonHandle);
  if (muonHandle.isValid()) 
  {
    const MuonCollection& recoMuonColl = *(muonHandle.product());
    int nMuons = recoMuonColl.size();
    if (_debugFlg > 3) cout << "EWKTauAnalyser::findMuons: number of Offline " 
                       << _muonSrc << " type muons=" << nMuons << endl;

    for (MuonCollection::const_iterator m  = recoMuonColl.begin(); 
                                        m != recoMuonColl.end(); ++m) 
    {
      if (_debugFlg > 3) 
        cout << "EWKTauAnalyser::findMuons: pt= " << m->pt()
             << " eta= "     << m->eta()
             << " phi= "     << m->phi()
             << endl;
    }
  }
}
void EWKTauAnalyser::findElectrons(const edm::Event& iEvent, const EventSetup& iSetup) 
{
  Handle<PixelMatchGsfElectronCollection> electronHandle; 
  iEvent.getByLabel(_electronSrc, electronHandle);
  if (electronHandle.isValid()) {
    const PixelMatchGsfElectronCollection& recoElectronColl = *( electronHandle.product() );
    if (_debugFlg >= 2) 
      cout << setiosflags(ios::left) << setw(30) 
           << "EWKTauAnalyser::findElectrons: # of electrons=" << recoElectronColl.size() << endl;
    for (PixelMatchGsfElectronCollection::const_iterator e  = recoElectronColl.begin(); 
                                                         e != recoElectronColl.end(); 
                                                       ++e) {
      if (_debugFlg > 3) 
        cout << "EWKTauAnalyser::findElectrons: pt= " << e->pt()
             << " eta= "         << e->eta()
             << " phi= "         << e->phi()
             << endl;
    }
  }
}
bool EWKTauAnalyser::getGenJet(const PFTau& jet, const GenJetCollection& genJets,
                               math::XYZTLorentzVector& genJetP4, int& decayMode, double& dr)
{
  math::XYZVector jet3Vec(jet.momentum());
  int index = 0;
  int min_index = -1;
  for (GenJetCollection::const_iterator f  = genJets.begin();
                                        f != genJets.end();
                                      ++f)
  {
    double deltaR = ROOT::Math::VectorUtil::DeltaR(jet3Vec, f->momentum());
    if (deltaR < dr) {
      dr = deltaR;
      min_index = index;
    }
    index++;
  }
  if (dr > 0.1) return false;

  bool tauFound = false;
  const GenJet& j = genJets[min_index];
  vector<const GenParticleCandidate*> gpColl = j.getConstituents();
  for (vector<const GenParticleCandidate*>::const_iterator f  = gpColl.begin();
                                                           f != gpColl.end();
                                                         ++f)
  {
    const GenParticleCandidate* gp = (*f);
    int pid = gp->pdgId();
    if (_debugFlg > 3)
    cout << "PDG Code = " << pid << endl
         << " PDG Code Mother = ";
    for (unsigned int i = 0; i < gp->numberOfMothers(); ++i) {
      const Candidate* mother = gp->mother();
      int mid = mother->pdgId();
      int status = mother->status();
      if (_debugFlg > 3) cout << "\t" << mid << ", status = " << status;
      if (abs(mid)==15 && status==2) {
        tauFound = true;
        // find tau decay type (e, mu, jet)
	switch (abs(pid)) {
	case 11:
	  decayMode = 0;
	  break;
	case 13:
	  decayMode = 1;
	  break;
	default:
	  decayMode = 2;
	}
        break;
      }
    }
    if (_debugFlg > 3) cout << endl;
  }
  if (!tauFound) return false;

  genJetP4 = const_cast<math::XYZTLorentzVector&>(j.p4());
  return true;
}
void EWKTauAnalyser::findInvMassFromCalo(const edm::Event& iEvent, const EventSetup& iSetup) 
{
  Handle<TauMassTagInfoCollection> massTagInfoHandle;
  iEvent.getByLabel(_caloInvMassSrc, massTagInfoHandle);

  if (massTagInfoHandle.isValid()) {
    const TauMassTagInfoCollection& massTagInfoColl = *(massTagInfoHandle.product());

    for (TauMassTagInfoCollection::const_iterator iter  = massTagInfoColl.begin(); 
	                                          iter != massTagInfoColl.end();
                                                 ++iter) {
      const IsolatedTauTagInfo& isolatedTauTag = *(iter->getIsolatedTauTag());
      double discriminator = isolatedTauTag.discriminator(_matchingCone, 
                                                          _signalCone, 
                                                          _isolationCone, 
                                                          _leadingTrackPt, 
                                                          _minimumTrackPt);
      
      if (discriminator == 0) continue;
      double invMass = iter->getInvariantMass(_matchingCone, 
                                              _leadingTrackPt, 
                                              _signalCone, 
                                              _clusterTrackMatchingCone); 
      if (TMath::Abs(invMass - 0.139) < 0.01) invMass = 0.0;
      if (invMass > -1.0) caloInvMassH->Fill(invMass);
    } 
  }
  else {
    cerr << "EWKTauAnalyser::findInvMassFromCalo: ERROR. TauMassTagInfoCollection missing!" << endl;
  }
}
void EWKTauAnalyser::findTauImpactParameter(const edm::Event& iEvent, const EventSetup& iSetup) 
{
  Handle<TauImpactParameterInfoCollection> ipInfoHandle;
  iEvent.getByLabel(_ipSrc, ipInfoHandle);

  if (ipInfoHandle.isValid()) {
    const TauImpactParameterInfoCollection& ipInfoColl = *(ipInfoHandle.product());
    for (TauImpactParameterInfoCollection::const_iterator iter  = ipInfoColl.begin(); 
	                                                  iter != ipInfoColl.end();
                                                        ++iter) 
    {
      const IsolatedTauTagInfo& isolatedTauTag = *(iter->getIsolatedTauTag());
      double discriminator = isolatedTauTag.discriminator(_matchingCone, _signalCone, 
                                       _isolationCone, _leadingTrackPt, _minimumTrackPt);
      if (discriminator == 0) continue;

      const Jet& theJet = *(isolatedTauTag.jet());
      double jetEt = theJet.et();
      double jetEta = theJet.eta();
      if (_debugFlg > 3) 
        cout << " EWKTauAnalyser::findTauImpactParameter. Candidate jet Et = " << jetEt  
             << ", isolation discriminator = "<< discriminator << endl;

      if (jetEt < 0 || jetEt > 150) continue;
      if (TMath::Abs(jetEta) > 2.2) continue;

      double cone  = 0.1;
      double ptmin = 6;
      const TrackRef leadingTrack = isolatedTauTag.leadingSignalTrack(cone,ptmin);
      if (!leadingTrack) continue;
      if (leadingTrack->recHitsSize() < 8 || leadingTrack->normalizedChi2() > 10) continue;

      const reco::TauImpactParameterTrackData* trackData = iter->getTrackData(leadingTrack);
      if (!trackData) continue;
      Measurement1D tip(trackData->transverseIp);
      Measurement1D tzip(trackData->ip3D);

      ip2DH->Fill(TMath::Abs(tip.value())*10); // in mm
      ip2DSigH->Fill(tip.significance());
      
      ip3DH->Fill(TMath::Abs(tzip.value())*10); // in mm
      ip3DSigH->Fill(tzip.significance());
    } 
  }
  else {
    cerr << "EWKTauAnalyser::findTauImpactParameter. TauImpactParameterInfoCollection missing!" << endl;
  }
}
void EWKTauAnalyser::endJob() 
{ 
  // Add the event statistics to a histogram
  for (unsigned int i = 0; i < NEL(_nStat); ++i)
    statH->SetBinContent(i+1, _nStat[i]);

  // Print out Event Statistics
  // to the terminal
#if 1
  printStatistics(cout);

  // to an output file
  ofstream fLog(_statFile.c_str(), ios::out);
  if (!fLog) {
    cerr << "File: " << _statFile << " could not be opened!" << endl;
    return;
  }
  printStatistics(fLog);
  fLog.close();
#endif
}
void EWKTauAnalyser::printStatistics(ostream& out) 
{
  out << setiosflags(ios::fixed);
  out << "----------------------------------------------------------" << endl
      << "        Statistics with Event Charge (ch1*ch2)            " << endl
      << "----------------------------------------------------------" << endl
      << "  Zero " << setw(4) << _nEvtCharge[0] << endl
      << "    SS " << setw(4) << _nEvtCharge[1] << endl
      << "    OS " << setw(4) << _nEvtCharge[2] << endl;
  string type = (_analysisType == "QCD_Estimate") ? "     same charge" : "opposite charges";
  out << "========================================================= " << endl
      << "              Event Statistics                            " << endl
      << "========================================================= " << endl  
      << "                        Total Number of Events = " << setw(8) << _nStat[0] << endl
      << "    Events with atleast one object in tau Coll = " << setw(8) << _nStat[1] << endl
      << "   Events with both Jets matched with HLT jets = " << setw(8) << _nStat[2] << endl
      << "     Events with both Jets with Leading Tracks = " << setw(8) << _nStat[3] << endl
      << "            Events with two discriminated taus = " << setw(8) << _nStat[4] << endl
      << "                    Events with nExtraJet <= " << setw(1) << _maxExtraJets << " = " 
      <<                                                    setw(8) << _nStat[5] << endl
      << "             Events with atleast one Tight tau = " << setw(8) << _nStat[6] << endl
      << "   Events with two selected taus (Tight+Loose) = " << setw(8) << _nStat[7] << endl
      << "      Events with two taus of " << type << " = "   << setw(8) << _nStat[8] << endl
      << "         Events with only 1-prong/3-prong taus = " << setw(8) << _nStat[9] << endl
      << "        Events with sum(sipt) of taus >= " << setprecision(1) << setw(5) 
                          << _minIPSignificanceSum << " = " << setw(8) << _nStat[10] << endl
      << "  Events with Z0 Mass in (" <<  setprecision(1) << setw(6) 
                                 << _minZMass <<  " - " << setprecision(1) << setw(6) 
                                 << _maxZMass << " GeV) = " << setw(8) << _nStat[11] << endl;

  if (_dataType == "signal") 
    out << "               Both taus decaying hadronically = " << setw(8) << _nStat[12] << endl;
  out << "========================================================= " << endl;  
  out << resetiosflags(ios::fixed);
}
int EWKTauAnalyser::findTauDecayMode(const edm::Event& iEvent, 
                                     const edm::EventSetup& iSetup, 
                                     vector<int>& tauTypeList, bool fillHist) 
{
  edm::Handle<edm::HepMCProduct> genParticleHandle;
  iEvent.getByLabel(_genParticles, genParticleHandle);

  int decay_mode = -1;
  HepMC::GenEvent* generated_event = new HepMC::GenEvent(*(genParticleHandle->GetEvent()));
  math::XYZTLorentzVector myTauP4(0.,0.,0.,0.);
  for (HepMC::GenEvent::particle_iterator p  = generated_event->particles_begin();
                                          p != generated_event->particles_end(); 
                                        ++p)
  {
    if (abs((*p)->pdg_id()) == 15 && (*p)->status() == 2) {
      math::XYZTLorentzVector tauP4((*p)->momentum().px(),(*p)->momentum().py(),(*p)->momentum().pz(),(*p)->momentum().e());
      bool lept_decay = false;
      HepMC::GenVertex::particle_iterator z = (*p)->end_vertex()->particles_begin(HepMC::descendants);
      int ndesc = 0;
      int ngamma = 0;
      for (; z != (*p)->end_vertex()->particles_end(HepMC::descendants); ++z) {
         if (abs((*z)->pdg_id()) == 22) ngamma++;
         else ndesc++;
         if (abs((*z)->pdg_id()) == 11 || abs((*z)->pdg_id()) == 13) lept_decay = true;
         if (abs((*z)->pdg_id()) == 11)  {
           decay_mode = 0;
         }
         else if (abs((*z)->pdg_id()) == 13) {
           decay_mode = 1;
         }
	 if (abs((*z)->pdg_id()) == 16) {
	   math::XYZTLorentzVector tauNeuP4( (*z)->momentum().px(), 
                                             (*z)->momentum().py(),
                                             (*z)->momentum().pz(),
                                             (*z)->momentum().e() );
	   myTauP4 = tauP4 - tauNeuP4;
         }    
      }
      if (lept_decay == false) {
        decay_mode = 2;

        if (fillHist) {
          tauDecayProdH->Fill(ndesc+ngamma-1);
          tauDecayProdNoGamH->Fill(ndesc-1);
          tauProdGammaOnlyH->Fill(ngamma);
          tauDecayProd2DH->Fill(ndesc-1, ngamma);
          double invMassMC = myTauP4.M();
          invMassMCH->Fill(invMassMC);
          if (invMassMC < 1.0) tauDecayProdMlt1H->Fill(ndesc+ngamma-1);
          else tauDecayProdMgt1H->Fill(ndesc+ngamma-1);
          if (ndesc == 2 && ngamma == 0) invMassMC1H->Fill(myTauP4.M());
          if (ndesc == 2 && ngamma > 0)  invMassMC2H->Fill(myTauP4.M());
          if (ndesc > 2) invMassMC3H->Fill(myTauP4.M());
        }
      }  
      tauTypeList.push_back(decay_mode);
    }
  }
  delete generated_event;

  if (_debugFlg > 3) {
    cout << "Tau Decay Mode (0=e, 1=mu, 2=jet): ";
    for (vector<int>::const_iterator it = tauTypeList.begin(); it != tauTypeList.end(); ++it) {
      cout << *it << " ";
    }
    cout << endl;
  }
  return tauTypeList.size();
}
//
// -- Select Jet with Tau properties
//
int EWKTauAnalyser::selectTau(const reco::PFTau& tau) 
{
  // Bit structure
  // 0 - tightTauLeadingTrkPt
  // 1 - looseTauLeadingTrkPt
  // 2 - leadingTrkPtFrac
  // 3 - tauMass
  // 4 - # prongs
  // 5 - charge
  // 6 - maxHadTowerEnergy

  double mass           = tau.alternatLorentzVect().M();
  double pt             = tau.pt();
  double leading_trk_pt = (*(tau.leadPFChargedHadrCand())).pt();
  double charge         = getTauCharge(tau, _debugFlg);
  int sig_ch_hadrons    = tau.signalPFChargedHadrCands().size();  
  double hadEnergy      = tau.maximumHCALPFClusterEt();

  int isel = 0;
  if ( applySelection(_selectionBits, 2) && (hadEnergy < _maxHadTowerEnergy) )   isel |= (1 << 6);
  if ( applySelection(_selectionBits, 3) && (TMath::Abs(charge) != 1.0) )        isel |= (1 << 5);
  if ( applySelection(_selectionBits, 4) && (sig_ch_hadrons%2 == 0) )            isel |= (1 << 4);
  if ( applySelection(_selectionBits, 5) && (mass < 0.0 || mass > _maxTauMass) ) isel |= (1 << 3);
  if ( applySelection(_selectionBits, 6)) {
    if (leading_trk_pt/pt < _leadingTrkPtFrac)                                   isel |= (1 << 2); // a stricter cut means 1-prong taus
    if (leading_trk_pt < _looseTauLeadingTrkPt)                                  isel |= (1 << 1);
    if (leading_trk_pt < _tightTauLeadingTrkPt)                                  isel |= (1 << 0);
  }
  return isel;
}
//
// -- retrieve tau object indices
//
pair<unsigned int, unsigned int> EWKTauAnalyser::getTauIndices(map<unsigned int, unsigned int>& smap) 
{
  unsigned int indx1 = 999; 
  unsigned int indx2 = 999;
  
  for (map<unsigned int, unsigned int>::iterator it = smap.begin();
                                                it != smap.end(); ++it) {
    if (indx1 == 999 || indx2 == 999) {
      if (it->second == 0) {
	if (indx1 == 999) indx1 = it->first;
	else if (indx2 == 999) indx2 = it->first;
      } else {
	if (indx2 == 999) indx2 = it->first;
      } 
    }      
  }
  return pair<unsigned int, unsigned int>(indx1, indx2);
}
//
// -- Calculate Invariant Mass
//
void EWKTauAnalyser::findInvariantMass(const reco::PFTau& tau) 
{
   PFCandidateRefVector sigCands = tau.signalPFCands();
   math::XYZTLorentzVector p1;
   for (PFCandidateRefVector::const_iterator iCand = sigCands.begin(); 
                                             iCand != sigCands.end(); 
                                           ++iCand) 
   {
     p1 += (**iCand).p4();
   }
   invMass3H->Fill(p1.M());
 
   math::XYZTLorentzVector p2;   
   PFCandidateRefVector chHadCands   = tau.signalPFChargedHadrCands();
   PFCandidateRefVector gammaCands   = tau.pfTauTagInfoRef()->PFGammaCands();
   PFCandidateRefVector neutralCands = tau.pfTauTagInfoRef()->PFNeutrHadrCands();
   
   for (PFCandidateRefVector::const_iterator iHad = chHadCands.begin(); 
                                             iHad != chHadCands.end(); 
                                           ++iHad)
   {
     p2 += (**iHad).p4();
   }

   const TrackRef leadTrack =  tau.leadTrack();
   math::XYZVector vec1(leadTrack->px(), leadTrack->py(), leadTrack->pz());

   // Add photons only 
   for (PFCandidateRefVector::const_iterator iGam  = gammaCands.begin(); 
                                             iGam != gammaCands.end();
                                           ++iGam)
   {
     math::XYZVector vec2((**iGam).px(), (**iGam).py(), (**iGam).pz());
     double deltaR = ROOT::Math::VectorUtil::DeltaR(vec1, vec2);
     if (deltaR < 0.4) p2 += (**iGam).p4();
   }
   invMass4H->Fill(p2.M());
   
   // add also neutral hadrons
   for (PFCandidateRefVector::const_iterator iNeu =  neutralCands.begin(); 
                                             iNeu != neutralCands.end();
                                           ++iNeu)
   {
     math::XYZVector vec2((**iNeu).px(), (**iNeu).py(), (**iNeu).pz());
     double deltaR = ROOT::Math::VectorUtil::DeltaR(vec1, vec2);
     if (deltaR < 0.4) p2 += (**iNeu).p4();
   }
  
   invMass5H->Fill(p2.M());
}
//
// -- Match Tau Jet with HLT
//
bool EWKTauAnalyser::matchWithHLTJet(const PFTau& tau, const CaloJetCollection& caloJets, double& dr)
{
  math::XYZVector tau3Vec(tau.momentum());
  for (CaloJetCollection::const_iterator ic  = caloJets.begin();
                                         ic != caloJets.end(); ++ic) {
    double deltaR = ROOT::Math::VectorUtil::DeltaR(tau3Vec, ic->momentum());
    if (deltaR < dr) dr = deltaR;
  }
  if (dr > 0.3) return false;
  return true;
}
void EWKTauAnalyser::TauObject(Handle<PFTauCollection> tauHandle, int iTau, PFTau& tau) 
{
  PFTauRef theTau(tauHandle, iTau);
  if (!theTau) {
    cerr << "Strange! Should have a valid PFTauRef for iTau=" << iTau << endl;
    return;
  }
  tau = (*theTau);
}
double EWKTauAnalyser::sumOfIPSignificance(const PFTau& tau1, const PFTau& tau2, int dflag) 
{
  double sipt_1 = tau1.leadPFChargedHadrCandsignedSipt();
  double sipt_2 = tau2.leadPFChargedHadrCandsignedSipt();
  double result = sipt_1*sipt_1 + sipt_2*sipt_2;
  if (dflag) cout << "sipt_1*sipt_1 + sipt_2*sipt_2=" << result << endl;
  return result;
}
bool EWKTauAnalyser::prongsAsExpected(const PFTau& tau1, const PFTau& tau2, int dflag) 
{
  int nch_1 = tau1.signalPFChargedHadrCands().size();  
  int nch_2 = tau2.signalPFChargedHadrCands().size();  
  if (dflag > 3) cout << "nch_1 = " << nch_1 << ", nch_2=" << nch_2 << endl;
  if (nch_1%2 == 0 || nch_2%2 == 0) return false; // must have odd # of tracks
  if (nch_1 != 1 && nch_2 != 1) return false;     // keep only 1/3 and 3/1 combinations
  return true;
}
void  EWKTauAnalyser::printEvent(const PFTau& tau1, const PFTau& tau2) 
{
  cout << "First Candidate:" << endl;
  printCandidate(tau1);  

  cout << "Second Candidate:" << endl;
  printCandidate(tau2);  

  double value = getTauCharge(tau1) * getTauCharge(tau2);
  cout << "Event Charge(ch1*ch2) = <" << value << ">" << endl;

  string p = prongsAsExpected(tau1, tau2) ? "<yes>" : "<no>";
  cout << "prongsAsExpected = " << p << endl;

  double siptSum = sumOfIPSignificance(tau1, tau2);
  cout << "Sum(IP Significance)**2 = <" << siptSum << ">" << endl;
 
  const math::XYZTLorentzVector& P1 = tau1.p4();
  const math::XYZTLorentzVector& P2 = tau2.p4();
  math::XYZTLorentzVector ZP4 = P1 + P2;
  double mass = ZP4.M();
  cout << "Reconstructed Z Mass = <" << mass << "> GeV" << endl;
}
double EWKTauAnalyser::getTauCharge(const PFTau& tau, int dflag) 
{
  double tch = 0.0;
  PFCandidateRefVector chHadCands = tau.signalPFChargedHadrCands();
  for (PFCandidateRefVector::const_iterator it  = chHadCands.begin();
                                            it != chHadCands.end(); 
                                          ++it)
  {
    tch += (**it).charge();
  }
  if (dflag > 3) 
    cout << "EWKTauAnalyser::getTauCharge: charge()=" << tau.charge() 
         << ", computed charge=" << tch << endl;
  return tch;
}
void  EWKTauAnalyser::printCandidate(const PFTau& tau)
{
    double pt = tau.pt();
    PFCandidateRef theLeadPFCand = tau.leadPFChargedHadrCand();
    if (!theLeadPFCand) {      
      cerr << "EWKTauAnalyser::printCandidate(): No Lead PFCand!" << endl;
      return;
    }
    double leading_trk_pt       = (*theLeadPFCand).pt();
    double charge               = getTauCharge(tau);
    double max_had_tower_energy = tau.maximumHCALPFClusterEt();
    double leading_trk_sipt     = TMath::Abs(tau.leadPFChargedHadrCandsignedSipt());
    double mass                 = tau.alternatLorentzVect().M();
    int sig_ch_hadrons          = tau.signalPFChargedHadrCands().size();
    int sig_photons             = tau.signalPFGammaCands().size();
    int sig_neu_hadrons         = tau.signalPFNeutrHadrCands().size();
    int iso_ch_hadrons          = tau.isolationPFChargedHadrCands().size();
    int iso_photons             = tau.isolationPFGammaCands().size();
    int iso_neu_hadrons         = tau.isolationPFNeutrHadrCands().size();
    double sumpt_iso_ch_hadrons = tau.isolationPFChargedHadrCandsPtSum();
    double sumpt_iso_photons    = tau.isolationPFGammaCandsEtSum();

    const PFTauTagInfo& tagInfo = (*(tau.pfTauTagInfoRef()));
    const PFJet& pfJet          = (*(tagInfo.pfjetRef()));
    double em_energy_frac       = pfJet.chargedEmEnergyFraction() + pfJet.neutralEmEnergyFraction();
    double chEMEnergyJet        = pfJet.chargedEmEnergy();
    double neuEMEnergyJet       = pfJet.neutralEmEnergy();
    double chHadEnergyJet       = pfJet.chargedHadronEnergy();
    double neuHadEnergyJet      = pfJet.neutralHadronEnergy();
    double ehad_tot             = chHadEnergyJet + neuHadEnergyJet;
    double p = tau.p();
    double energy = tau.energy();

    cout << "                  Pt of the Candidate: " << pt << " GeV" << endl
         << "              Energy of the Candidate: " << energy << " GeV" << endl   
         << "                   P of the Candidate: " << p << " GeV" << endl   
         << "                 Lead PFCand Track pt: " << leading_trk_pt << " GeV"     << endl
         << "                         ltrkPt/jetPt: " << leading_trk_pt/pt    << endl
         << "               Lead PFCand Track Sipt: " << leading_trk_sipt     << endl
         << "             InvariantMass of the Tau: " << mass << " GeV"       << endl
         << "                    Charge of the Tau: " << charge               << endl
         << "                   Em energy fraction: " << em_energy_frac       << endl
         << "                    Max Hadron energy: " << max_had_tower_energy << " GeV" << endl
         << "                  Max Hadron energy/P: " << max_had_tower_energy/p << endl
         << "          # of SignalPFChargedHadrons: " << sig_ch_hadrons       << endl
         << "            # of SignalPFNeutrHadrons: " << sig_neu_hadrons      << endl
         << "       # of IsolationPFChargedHadrons: " << iso_ch_hadrons       << endl
         << "         # of IsolationPFNeutrHadrons: " << iso_neu_hadrons      << endl
         << "          # of SignalPFGammaCandidate: " << sig_photons          << endl
         << "       # of IsolationPFGammaCandidate: " << iso_photons          << endl
         << "  Sum pT of Isolation Charged Hadrons: " << sumpt_iso_ch_hadrons << endl
         << "Sum E_T of Isolation Gamma Candidates: " << sumpt_iso_photons    << endl
         << "                     PF Jet EM Energy: " << chEMEnergyJet + neuEMEnergyJet << " GeV" << endl
         << "                                  E/P: " << energy/p << endl
         << "                                eT/pT: " << tau.et()/pt  << endl
         << "                           ehad_tot/P: " << ehad_tot/p << endl
         << endl;
}
bool EWKTauAnalyser::applySelection(int bitWord, int bitPosition) {
  return ( (bitWord >> bitPosition) & 0x1 ) ? true : false;
}
void EWKTauAnalyser::bit_print(int word, int pos) {
  int n = sizeof(int) * CHAR_BIT;
  if (0) cout << "CHAR_BIT=<" << CHAR_BIT << ">, sizeof(int)=<" << sizeof(int) << ">, n=<" << n << ">" << endl;
  int mask = 1 << (n-1);
  if (pos > n) pos = n;

  for (int i = 1; i <= (n-pos); ++i) 
    word <<= 1;

  for (int i = 1; i <= pos; ++i) {
    cout << (((word & mask) == 0) ? '0' : '1');
    word <<= 1;
    if ((n - pos + i) % CHAR_BIT == 0 && i != n)
      cout << ' '; 
  }
  cout << endl;
}
