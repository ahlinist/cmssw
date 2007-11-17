#include <memory>
#include <iostream>
#include <typeinfo>
#include <iomanip>

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

#include <TFile.h>

using namespace edm;
using namespace reco; 
using namespace std;
#define NEL(x) (sizeof((x))/sizeof((x)[0]))
//
//  Constructor
//
EWKTauAnalyser::EWKTauAnalyser(const ParameterSet& iConfig) {
    PFTaus_ = iConfig.getUntrackedParameter<string>("PFTaus","tauPFProducer");
    TauDiscriminatorByIsolationProducer_
       = iConfig.getUntrackedParameter<string>("TauDiscriminatorByIsolationProducer", "discriminationByIsolation");
        applyDisc = iConfig.getUntrackedParameter<int>("applyDisc", 1);
    applyHLTMatch = iConfig.getUntrackedParameter<int>("applyHLTMatch", 1);

         debugFlg = iConfig.getUntrackedParameter<int>("DebugFlg", 0);
         fileName = iConfig.getUntrackedParameter<string>("OutFileName", "test.root");
         dataType = iConfig.getUntrackedParameter<string>("DataType", "data");
       jetTagSrc_ = iConfig.getUntrackedParameter<string>("JetTagProd","coneIsolationTauJetTags");
          metSrc_ = iConfig.getUntrackedParameter<string>("metSrc", "met");
       genJetSrc_ = iConfig.getUntrackedParameter<string>("genJetSrc", "iterativeCone5GenJets");
      caloJetSrc_ = iConfig.getUntrackedParameter<string>("caloJetSrc", "iterativeCone5CaloJets");
     electronSrc_ = iConfig.getUntrackedParameter<string>("electronSrc", "pixelMatchGsfElectrons");
         muonSrc_ = iConfig.getUntrackedParameter<string>("muonSrc", "muons");
        trackSrc_ = iConfig.getUntrackedParameter<string>("trackSrc", "ctfWithMaterialTracks");
       vertexSrc_ = iConfig.getUntrackedParameter<string>("vertexSrc", "offlinePrimaryVerticesFromCTFTracks");
       hltJetSrc_ = iConfig.getUntrackedParameter<string>("hltJetSrc", "isolatedL25PixelTau");

    genParticles_ = iConfig.getUntrackedParameter<string>("genParticles", "source");
  caloInvMassSrc_ = iConfig.getUntrackedParameter<string>("caloMassTagSrc");
           ipSrc_ = iConfig.getUntrackedParameter<string>("ipSrc", "impactParameter");
        statFile_ = iConfig.getUntrackedParameter<string>("statFile", "./stat.log");

                maxTauMass_ = iConfig.getUntrackedParameter<double>("maxTauMass", 5.0);
      tightTauLeadingTrkPt_ = iConfig.getUntrackedParameter<double>("tightTauLeadingTrkPt", 15.0);
      looseTauLeadingTrkPt_ = iConfig.getUntrackedParameter<double>("looseTauLeadingTrkPt", 10.0);
          leadingTrkPtFrac_ = iConfig.getUntrackedParameter<double>("LeadingTrkPtFrac", 0.0);
                  minZMass_ = iConfig.getUntrackedParameter<double>("minZMass",  30.0);
                  maxZMass_ = iConfig.getUntrackedParameter<double>("maxZMass", 130.0);

    // JetMET correction
     jetCorrector_  = iConfig.getUntrackedParameter<string>("jetCorrector", "MCJetCorrectorIcone5");
  jetEtMinMETCorr_  = iConfig.getUntrackedParameter<double>("jetEtMinMETCorr", 20.0);
  jetEMfracMETCorr_ = iConfig.getUntrackedParameter<double>("jetEMfracMETCorr", 0.9);

              matchingCone_ = iConfig.getUntrackedParameter<double>("MatchingCone", 0.1);
                signalCone_ = iConfig.getUntrackedParameter<double>("SignalCone", 0.07);
             isolationCone_ = iConfig.getUntrackedParameter<double>("IsolationCone", 0.4);
            leadingTrackPt_ = iConfig.getUntrackedParameter<double>("LeadingTrackPt", 6.0);
            minimumTrackPt_ = iConfig.getUntrackedParameter<double>("MinimumTrackPt", 1.0);
  clusterTrackMatchingCone_ = iConfig.getUntrackedParameter<double>("ClusterTrackMatchingCone", 0.08);

  // open the stat output file
  fStatLog_.open(statFile_.c_str(), ios::out);
  if (fStatLog_) {
    fStatLog_ << setiosflags(ios::fixed);
  }
  else {
    cerr << "File: " << statFile_ << " could not be opened!" << endl;
  }
}
//
// -- Begin Job
//
void EWKTauAnalyser::beginJob(const edm::EventSetup& iSetup) {
  nEvent = 0;
  theFile = new TFile (fileName.c_str(), "RECREATE");

            nJetsH = new TH1I("nJets"," Number of candidate Jets E_T > 15 GeV", 10, -0.5, 9.5);
         nTauCandH = new TH1I("nTauCand"," Number of Tau Candidate Jets", 10, -0.5, 9.5);
            tauPtH = new TH1D("tauPt", "Tau Pt", 100, 0.0, 100.0);
           tauEtaH = new TH1D("tauEta", "Tau Eta", 100, -3.0, 3.0);
           tauPhiH = new TH1D("tauPhi", "Tau Phi", 100, 0, 6.28);
         tauPtGenH = new TH1D("tauPtGen", "Generated Tau Pt", 100, 0.0, 100.0);
        tauEtaGenH = new TH1D("tauEtaGen", "Generated Tau Eta", 100, -3.0, 3.0);
        tauPtDiscH = new TH1D("tauPtDisc", "Generated Tau Pt  Dicriminated", 100, 0.0, 100.0);
       tauEtaDiscH = new TH1D("tauEtaDisc", "Generated Tau Eta Dicriminated", 100, -3.0, 3.0);
          tauZVtxH = new TH1D("tauZVtx", "Tau Z-Vertex", 100, -50.0, 50.0);
        tauChargeH = new TH1D("tauCharge", "Tau Charge", 5, -0.5, 4.5);
         tauCh1PrH = new TH1D("tauCh1Pr", "Tau Charge for 1-Prong Taus", 5, -0.5, 4.5);
         tauCh3PrH = new TH1D("tauCh3Pr", "Tau Charge for 3-Prong Taus", 5, -0.5, 4.5);
          tauDiscH = new TH1D("tauDisc", "Tau Discriminator", 2, -0.5, 1.5);
       tauDiscFirst2JetsH = 
           new TH1D("tauDiscFirst2Jets", "Tau Discriminator for First Two Jets", 2, -0.5, 1.5);
       leadingTrackSiptH = 
           new TH1D("LeadingTrkSipt", "Leading Track Signed IP Significance", 100, 0.0, 20.0);
        ltIPSig1PH = new TH1D("LeadingTrkSipt1P", "Leading Track Signed IP Significance (1-prong)", 100, 0.0, 20.0);
        ltIPSig3PH = new TH1D("LeadingTrkSipt3P", "Leading Track Signed IP Significance (3-prong)", 100, 0.0, 20.0);
       leadingTrackPtH = 
           new TH1D("LeadingTrkPt", "Leading Track Pt", 100, 0.0, 100.0);
          invMassH = new TH1D("InvMass", "Invariant Mass", 100, 0.0, 10.0);
         invMass2H = new TH1D("InvMass2", "Invariant Mass (Signal Components)", 100, 0.0, 10.0);
         invMass3H = new TH1D("InvMass3", "Invariant Mass (custom I )", 100, 0.0, 10.0);
         invMass4H = new TH1D("InvMass4", "Invariant Mass (custom II )", 100, 0.0, 10.0);
         invMass5H = new TH1D("InvMass5", "Invariant Mass (custom III )", 100, 0.0, 10.0);
       invMassGenH = new TH1D("InvMassGen", "Invariant Mass from Generator Level", 100, 0.0, 10.0);
        invMassMCH = new TH1D("InvMassMC", "Invariant Mass from HepMC (all)", 100, 0.0, 10.0);
       invMassMC1H = new TH1D("InvMassMC1", "Invariant Mass from HepMC (1 prong - neutrino - gamma)", 100, 0.0, 10.0);
       invMassMC2H = new TH1D("InvMassMC2", "Invariant Mass from HepMC (1 prong + gamma - neutrino)", 100, 0.0, 10.0);
       invMassMC3H = new TH1D("InvMassMC3", "Invariant Mass from HepMC (multi promg)", 100, 0.0, 10.0);
    maxHcalEnergyH = new TH1D("MaxHcalEnergy", "Maximum HCAL Energy", 100, 0.0, 10.0);
    maxHcalEnergy2H = new TH1D("MaxHcalEnergy2", "Maximum HCAL Energy for non-hadronic tau decays ", 100, 0.0, 10.0);
     emEnergyFracH = new TH1D("EMFracEnergy", "Fractional EM Energy", 100, 0.0, 1.0);
    emEnergyFrac2H = new TH1D("EMFracEnergy2", "Fractional EM Energy for non-hadronic Tau decays", 100, 0.0, 1.0);
        nChHadronH = new TH1I("nChHadTotal"," Number of Charged Hadrons (Total)", 20, -0.5, 19.5);
     nChHadronSigH = new TH1I("nChHadInSigCone"," Number of Charged Hadrons in Signal Cone", 20, -0.5, 19.5);
     nChHadronIsoH = new TH1I("nChHadInIsoCone"," Number of Charged Hadrons in Isolation Cone", 20, -0.5, 19.5);
           nGammaH = new TH1I("nGammaTotal"," Number of Photons (Total)", 20, -0.5, 19.5); 
        nGammaSigH = new TH1I("nGammaSigCone"," Number of Photons in Signal Cone", 20, -0.5, 19.5);
        nGammaIsoH = new TH1I("nGammaIsoCone"," Number of Photons in Isolation Cone", 20, -0.5, 19.5);
       nNeuHadronH = new TH1I("nNeuHadronTotal"," Number of Neutral Hadrons (Total)", 20, -0.5, 19.5);
    nNeuHadronSigH = new TH1I("nNeuHadronSigCone"," Number of Neutral Hadrons in Signal Cone", 20, -0.5, 19.5);
    nNeuHadronIsoH = new TH1I("nNeuHadronIsoCone"," Number of Neutral Hadrons in Isolation Cone", 20, -0.5, 19.5);
    sumPtChHadIsoH = new TH1D("sumPtChHadIso", "Sum Pt of Ch. hadrons in Isolation Cone", 100, 0.0, 30.0);
    sumPtGammaIsoH = new TH1D("sumPtGammaIso", "Sum Et of Photons in Isolation Cone", 100, 0.0, 30.0);

   chHadEnergyJetH = new TH1D("chHadEnergyPFJet", "Charged Hadron Energy in PFJet", 100, 0.0, 100);
  neuHadEnergyJetH = new TH1D("neuHadEnergyPFJet", "Neutral Hadron Energy in PF Jet", 100, 0.0, 100.0);
    chEMEnergyJetH = new TH1D("chEMEnergyPFJet", "Charged EM Energy in PF Jet", 100, 0.0, 100.0);
    chMuEnergyJetH = new TH1D("chMuEnergyPFJet", "Charged Muon Energy in PF Jet", 100, 0.0, 100.0);
   neuEMEnergyJetH = new TH1D("neuEMEnergyPFJet", "Neutral EM Energy in PF Jet", 100, 0.0, 100.0);
        chMultJetH = new TH1F("chMultPFJet", "Charged Multiplicity in PF Jet", 20, -0.5, 19.5);
       neuMultJetH = new TH1F("neuMulPFtJet", "Neutral Multiplicity in PF Jet", 20, -0.5, 19.5);
        muMultJetH = new TH1F("muMultPFJet", "Muon Multiplicity in PF Jet", 20, -0.5, 19.5);

            zMassH = new TH1D("zmass", "Z mass", 100, 30., 130.0);
             zEtaH = new TH1D("zeta",  "Z Eta",  100, -3.0, 3.0);
             zPhiH = new TH1D("zphi",  "Z Phi", 100, 0., 6.28);
           deltaRH = new TH1D("deltaR",  "delta R between two jets", 100, 0., 6.28);
         deltaPhiH = new TH1D("deltaPhi","delta Phi between two jets", 100, 0., 6.28);
         cosThetaH = new TH1D("cosTheta","Cos Theta between two jets", 100, 0., 1);
      tauDiscOrigH = new TH1D("tauDiscOrig", "Tau Discriminator from IsolatedTauTagInfo", 2, -0.5, 1.5);
           ptDiffH = new TH1D("ptdiff", "pt(reco-gen/reco)", 100,-1.0, 1.0);
           ptScatH = new TH2D("pscatt","Gen vs Reco Tau P", 100, 10.0, 110.0, 100, 10.0, 110.0);
              metH = new TH1D("met", "Missing eT", 100, 0.0, 100.);
            sumEtH = new TH1D("sumEt", "Sum eT", 100, 0.0, 300.);
           metSigH = new TH1D("metSig", "met Significance", 100, 0.0, 10.);
     tauDecayProdH = new TH1I("tauDecayProd", "# of Decay products (excluding neutrinos)", 21, -0.5, 20.5);
     tauDecayProdNoGamH = 
        new TH1I("tauDecayProdNoGamma", "# of Decay products (excluding neutrino & gamma)", 21, -0.5, 20.5);
     tauDecayProd2DH = 
        new TH2I("tauDecayProd2D", "# of non-gamma products vs #of gamma products", 21, -0.5, 20.5, 21, -0.5, 20.5);
     tauDecayProdMlt1H = new TH1I("tauDecayProdMlt1", "# of Decay products (inv. mass < 1.0)", 21, -0.5, 20.5);
     tauDecayProdMgt1H = new TH1I("tauDecayProdMgt1", "# of Decay products (inv. mass > 1.0)", 21, -0.5, 20.5);
     tauProdGammaOnlyH = new TH1I("tauDecayGammaOnly", "# of Gammas from Tau", 21, -0.5, 20.5);

       caloInvMassH = new TH1D("CaloInvariantMass","Invariant Mass from Caloremeter",100, 0, 10.0);
      tauDecayModeH = new TH1I("tauDecayMode"," Tau Decay Mode", 3, -0.5, 2.5);
              ip2DH = new TH1D("ip2D", "2D Impact parameter as Tau Tag(value)", 100, 0, 0.3);
           ip2DSigH = new TH1D("ip2DSig", "2D Impact parameter as Tau Tag(significance)", 100, 0, 20);
              ip3DH = new TH1D("ip3D", "3D Impact parameter as Tau Tag(value)", 100, 0, 100);
           ip3DSigH = new TH1D("ip3DSig", "3D Impact parameter as Tau Tag(significance)", 100, 0.0, 3000);

          ltPtFracH = new TH1D("ltPtFrac", "Leading Trk Pt wrt Jet Pt", 100, 0.0, 1.0);
         ltvsJetPtH = new TH2D("ltvsJetPt","Leading Trk vs Jet Pt", 100, 0.0, 100.0, 100, 0.0, 100.0);

         nTightTauH = new TH1I("nTightTau"," Number of Tight Taus in the event", 3, -0.5, 2.5);
         nLooseTauH = new TH1I("nLooseTau"," Number of Loose Taus in the event", 3, -0.5, 2.5);
              statH = new TH1I("statH"," Event Statistics", 12, 0, 12);

      jetPtWithEtaP = new TProfile("jetPtWithEta","Profile of Jet Pt versus Jet Eta",100,-3,3,0,100);
      trkPtWithEtaP = new TProfile("trkPtWithEtaf","Profile of Leading Tracl Pt versus Jet Eta",100,-3,3,0,100);
            
       dRTauAndHLTH = new TH1D("dRTauAndHLT", "dR of Tau wrt nearest HLT Jet", 100, 0.0, 10.0);

       selIndexTauH = new TH1I("selBitPattarnTau", "Selection Bit Pattern of Tau", 11, -0.5, 10.5);
	
   // Do not touch the exisitng histograms, but book the some of the above again
   // this time separately for the two taus in question; since we always match with th
   // HLT jets before moving on, we'll get the correct entries most of the time
  
   // Discriminator
      tauDisc1H = new TH1D("tauDisc1", "Tau Discriminator of first Jet", 2, -0.5, 1.5);
      tauDisc2H = new TH1D("tauDisc2", "Tau Discriminator of second Jet", 2, -0.5, 1.5);

   // Leading track Pt and ratio of LT Pt to Jet Pt
      leadingTrackPt1H = new TH1D("leadingTrackPt1", "Leading Trk Pt of first tau", 100, 0.0, 100.0);
      leadingTrackPt2H = new TH1D("leadingTrackPt2", "Leading Trk Pt of second tau", 100, 0.0, 100.0);
      ltPtFrac1H = new TH1D("ltPtFrac1", "Leading Trk Pt of first tau wrt Jet Pt", 100, 0.0, 1.0);
      ltPtFrac2H = new TH1D("ltPtFrac2", "Leading Trk Pt of second tau wrt Jet Pt", 100, 0.0, 1.0);

   // Tau pseudorapidity
      tauEta1H = new TH1D("tauEta1", "Tau Eta first jet", 100, -3.0, 3.0);
      tauEta2H = new TH1D("tauEta2", "Tau Eta second jet", 100, -3.0, 3.0);

   // Invariant Mass
      tauMass1H = new TH1D("tauMass1", "Tau Invariant Mass (Signal Components, first jet)", 100, 0.0, 10.0);
      tauMass2H = new TH1D("tauMass2", "Tau Invariant Mass (Signal Components, second jet)", 100, 0.0, 10.0);

   // Impact parameter Significance of the L Track 
      ltSipt1H = new TH1D("LeadingTrkSipt1", "Leading Track Signed IP Significance (first jet)", 100, 0.0, 20.0);
      ltSipt2H = new TH1D("LeadingTrkSipt2", "Leading Track Signed IP Significance (second jet)", 100, 0.0, 20.0);
   
   // Decay mode of the 2 taus
      fTauDecayModeH = new TH1I("FTauDecayMode", "Decay Mode of the first tau", 3, -0.5, 2.5);
      sTauDecayModeH = new TH1I("STauDecayMode", "Decay Mode of the second tau", 3, -0.5, 2.5);

   // initialise Statistical variables 
   for (unsigned int i = 0; i < NEL(nStat); i++) nStat[i] = 0;
}
//
// -- Analyze
//
void EWKTauAnalyser::analyze(const Event& iEvent, const EventSetup& iSetup) {
  int run = iEvent.id().run();
  int evt = iEvent.id().event();
  nStat[0]++;
  if (debugFlg)
  cout << "==============================================" << endl
       << " Event: " << nEvent++ << endl
       << " Run # " << run << " Event # " << evt << endl
       << "==============================================" << endl;
  
  // Let's first find number of PF jets with E_T > 5 GeV in the event
  int njet = 0;
  Handle<PFJetCollection> pfJetHandle;
  iEvent.getByLabel("iterativeCone5PFJets", pfJetHandle);
  if (pfJetHandle.isValid()) {
    const PFJetCollection& pfJetColl = *(pfJetHandle.product()); 
    if (debugFlg) cout << "Found " << pfJetColl.size() << " Jet candidates" << endl;

    for (PFJetCollection::const_iterator iter  = pfJetColl.begin();
                                         iter != pfJetColl.end();
                                       ++iter)
    {
      const PFJet& obj = (*iter);
      if (obj.et() > 15.) njet++;
    }
  }
  nJetsH->Fill(njet);

  // Particle Flow based information
  Handle<PFTauCollection> tauHandle;
  iEvent.getByLabel(PFTaus_, tauHandle);
  const PFTauCollection& myTauCollection = *(tauHandle.product()); 

  // CaloJet collection from HLT
  //  Handle<reco::HLTFilter> tauHLTHandle;
  //  iEvent.getByLabel(hltJetSrc_,tauHLTHandle);
  //  cout <<"Size "<<tauHLTHandle->size()<<endl;
  //  for( unsigned int ic=0; ic <tauHLTHandle->size();ic++) {
  //   Edm::RefToBase<reco::Candidate> tauCandRef = tauHLTHandle->getParticleRef(ic);
  //   cout <<"Eta and Phi HLT "<<endl;
  //  }
  Handle<CaloJetCollection> tauHLTHandle;
  iEvent.getByLabel(hltJetSrc_, tauHLTHandle);
  const CaloJetCollection& myHLTJetCollection = *(tauHLTHandle.product()); 

  Handle<PFTauDiscriminatorByIsolation> theTauDiscriminatorByIsolation;
  iEvent.getByLabel(TauDiscriminatorByIsolationProducer_, theTauDiscriminatorByIsolation);

  edm::Handle<reco::GenJetCollection> genJetsHandle;
  iEvent.getByLabel(genJetSrc_, genJetsHandle);
  const reco::GenJetCollection& genJets = *(genJetsHandle.product());

  int n_tau_candidate = myTauCollection.size();
  if (debugFlg) cout << "Found " << n_tau_candidate << " had. tau-jet candidates" << endl;
  // Now the real stuff
  nTauCandH->Fill(n_tau_candidate);
  if (!n_tau_candidate) return;
  nStat[1]++;

  // Go to the TFile directory
  theFile->cd();

  // Find Tau Decay mode
  vector<int> tauDecayModes;
  findTauDecayMode(iEvent, iSetup, tauDecayModes);

  vector<math::XYZTLorentzVector> genTauList;

  map<unsigned int, unsigned int> selectMap;
  int nTightTau = 0,
      nLooseTau = 0;

  int nMatchedJet        = 0;
  int nDiscriminatedJet  = 0;
  int nLeadingTrackInJet = 0;  
  for (PFTauCollection::size_type iTau = 0; iTau < tauHandle->size(); iTau++) {
    PFTauRef theTau(tauHandle, iTau);
    if (!theTau) {
      cerr << "Strange! Should have a valid PFTauRef for iTau=" << iTau << endl;
      continue;
    }
    const PFTau& tau = (*theTau);

    // Prints out some quantities
    if (debugFlg)
    cout << "================" << endl
         << "Jet Number: "     << iTau << endl
         << "================" << endl;
    double minDR = 99.9;
    if (applyHLTMatch && !matchWithHLTJet(tau, myHLTJetCollection, minDR)) {
      cout << " dR of PFTauJet wrt nearest HLT Jet " << minDR << endl;
      dRTauAndHLTH->Fill(minDR);
      continue;
    }
    nMatchedJet++;
    double tau_disc = (*theTauDiscriminatorByIsolation)[theTau];
    if (debugFlg) cout << "DiscriminatorByIsolation value: " << tau_disc << endl;
    tauDiscH->Fill(tau_disc);
    if (iTau <= 1) tauDiscFirst2JetsH->Fill(tau_disc);

    // Temporary
    if (nMatchedJet == 1) 
      tauDisc1H->Fill(tau_disc);
    else if (nMatchedJet == 2)
      tauDisc2H->Fill(tau_disc);

    double tau_pt = tau.pt();
    if (debugFlg) cout << "Pt of the Tau: " << tau_pt << endl;

    PFCandidateRef theLeadPFCand = tau.leadPFChargedHadrCand();
    if (!theLeadPFCand) {      
      cerr << "No Lead PFCand " << endl;
      continue;
    }
    nLeadingTrackInJet++;

    if (applyDisc && tau_disc < 1.0) continue;
    if (tau_disc > 0.0) nDiscriminatedJet++;

    double tau_leading_trk_pt       = (*theLeadPFCand).pt();
    double tau_zvertex              = tau.vz();      // What is this quantity?
    double tau_charge               = tau.charge();
    double tau_had_energy           = tau.maximumHCALPFClusterEt();

    const PFTauTagInfo& tagInfo     = (*(tau.pfTauTagInfoRef()));
    double leading_trk_sipt         = fabs(tau.leadPFChargedHadrCandsignedSipt());

    double tau_inv_mass             = tau.p4().M();
    double tau_inv_mass2            = tau.alternatLorentzVect().M();

    int tau_ch_hadrons              = tagInfo.PFChargedHadrCands().size();
    int tau_neu_hadrons             = tagInfo.PFNeutrHadrCands().size();
    int tau_photons                 = tagInfo.PFGammaCands().size();

    int tau_sig_ch_hadrons          = tau.signalPFChargedHadrCands().size();
    int tau_sig_photons             = tau.signalPFGammaCands().size();
    int tau_sig_neu_hadrons         = tau.signalPFNeutrHadrCands().size();

    int tau_iso_ch_hadrons          = tau.isolationPFChargedHadrCands().size();
    int tau_iso_photons             = tau.isolationPFGammaCands().size();
    int tau_iso_neu_hadrons         = tau.isolationPFNeutrHadrCands().size();

    double tau_sumpt_iso_ch_hadrons = tau.isolationPFChargedHadrCandsPtSum();
    double tau_sumpt_iso_photons    = tau.isolationPFGammaCandsEtSum();

    const PFJet& pfJet              = (*(tagInfo.pfjetRef()));
    double tau_em_energy            = pfJet.chargedEmEnergyFraction()+pfJet.neutralEmEnergyFraction();

    double chHadEnergyJet           = pfJet.chargedHadronEnergy();
    double neuHadEnergyJet          = pfJet.neutralHadronEnergy();
    double chEMEnergyJet            = pfJet.chargedEmEnergy();
    double chMuEnergyJet            = pfJet.chargedMuEnergy();
    double neuEMEnergyJet           = pfJet.neutralEmEnergy();
    float  chMultJet                = pfJet.chargedMultiplicity();
    float  neuMultJet               = pfJet.neutralMultiplicity();
    float  muMultJet                = pfJet.muonMultiplicity();

    if (debugFlg)
      cout << "                       Lead PFCand pt: " << tau_leading_trk_pt       << endl
           << "             InvariantMass of the Tau: " << tau_inv_mass             << endl
           << "                    Vertex of the Tau: " << tau_zvertex              << endl
           << "                    Charge of the Tau: " << tau_charge               << endl
           << "                   Em energy fraction: " << tau_em_energy            << endl
           << "                    Max Hadron energy: " << tau_had_energy           << endl
           << "          # of PF charged hadr. cands: " << tau_ch_hadrons           << endl
           << "          # of PF neutral hadr. cands: " << tau_neu_hadrons          << endl
           << "                  # of PF gamma cands: " << tau_photons              << endl
           << "          # of SignalPFChargedHadrons: " << tau_sig_ch_hadrons       << endl
           << "       # of IsolationPFChargedHadrons: " << tau_iso_ch_hadrons       << endl
           << "          # of SignalPFGammaCandidate: " << tau_sig_photons          << endl
           << "       # of IsolationPFGammaCandidate: " << tau_iso_photons          << endl
           << "  Sum pT of Isolation Charged Hadrons: " << tau_sumpt_iso_ch_hadrons << endl
           << "Sum E_T of Isolation Gamma Candidates: " << tau_sumpt_iso_photons    << endl
           << "                     PF Jet EM Energy: " << chEMEnergyJet+neuEMEnergyJet << endl
           << endl;

    invMassH->Fill(tau_inv_mass);
    leadingTrackPtH->Fill(tau_leading_trk_pt);

    // leading_trk_pt wrt jet pt
    double tau_eta = tau.eta();
    double ratio = tau_leading_trk_pt/tau_pt;
    ltPtFracH->Fill(ratio);
    ltvsJetPtH->Fill(tau_pt, tau_leading_trk_pt);
    jetPtWithEtaP->Fill(tau_eta, tau_pt, 1);
    trkPtWithEtaP->Fill(tau_eta, tau_leading_trk_pt, 1);
 
    // Fill Histograms
    tauPtH->Fill(tau_pt);
    tauEtaH->Fill(tau_eta);
    tauPhiH->Fill(ROOT::Math::VectorUtil::Phi_0_2pi(tau.phi()));
    tauZVtxH->Fill(tau_zvertex);
    tauChargeH->Fill(fabs(tau_charge));
    if (tau_sig_ch_hadrons == 1) tauCh1PrH->Fill(fabs(tau_charge));
    if (tau_sig_ch_hadrons == 3) tauCh3PrH->Fill(fabs(tau_charge));
    leadingTrackSiptH->Fill(leading_trk_sipt);
    if (tau_ch_hadrons == 1) {
      ltIPSig1PH->Fill(leading_trk_sipt);
    }
    else { // multi prong
      ltIPSig3PH->Fill(leading_trk_sipt);
    }

    invMass2H->Fill(tau_inv_mass2);
    maxHcalEnergyH->Fill(tau_had_energy);
    emEnergyFracH->Fill(tau_em_energy);
    nChHadronH->Fill(tau_ch_hadrons);
    nChHadronSigH->Fill(tau_sig_ch_hadrons);
    nChHadronIsoH->Fill(tau_iso_ch_hadrons);
    nGammaH->Fill(tau_photons);
    nGammaSigH->Fill(tau_sig_photons);
    nGammaIsoH->Fill(tau_iso_photons);
    nNeuHadronH->Fill(tau_neu_hadrons);
    nNeuHadronSigH->Fill(tau_sig_neu_hadrons);
    nNeuHadronIsoH->Fill(tau_iso_neu_hadrons);
    sumPtChHadIsoH->Fill(tau_sumpt_iso_ch_hadrons);
    sumPtGammaIsoH->Fill(tau_sumpt_iso_photons);

    chHadEnergyJetH->Fill(chHadEnergyJet);
    neuHadEnergyJetH->Fill(neuHadEnergyJet);
    chEMEnergyJetH->Fill(chEMEnergyJet);
    chMuEnergyJetH->Fill(chMuEnergyJet);
    neuEMEnergyJetH->Fill(neuEMEnergyJet);
    chMultJetH->Fill(chMultJet);
    neuMultJetH->Fill(neuMultJet);
    muMultJetH->Fill(muMultJet);

    // Temporary
    if (nDiscriminatedJet == 1) {
      leadingTrackPt1H->Fill(tau_leading_trk_pt);
      ltPtFrac1H->Fill(ratio);
      ltSipt1H->Fill(leading_trk_sipt);
      tauMass1H->Fill(tau_inv_mass2);
      tauEta1H->Fill(tau_eta);
    }
    else if (nDiscriminatedJet == 2) {
      leadingTrackPt2H->Fill(tau_leading_trk_pt);
      ltPtFrac2H->Fill(ratio);
      ltSipt2H->Fill(leading_trk_sipt);
      tauMass2H->Fill(tau_inv_mass2);
      tauEta1H->Fill(tau_eta);
    }

    // calculate and fill custom invariant masses
    findInvariantMass(tau);

    // Now apply further selection
    int iSelect = selectTau(tau);
     
    for (unsigned int k = 0; k < 11; k++) {
      if (iSelect & (1 << k)) {
	selIndexTauH->Fill(k);
      } 
    }
    if (iSelect < 2) {
      selectMap.insert(pair<unsigned int, unsigned int>(iTau,iSelect));
      if (iSelect == 0) nTightTau++;
      else              nLooseTau++;
    } 

    // Match with the generator level tau
    math::XYZTLorentzVector genJetP4;
    double dr = 999.0;
    int decayMode = -1;
    if (getGenJet(tau, genJets, genJetP4, decayMode, dr)) {
      double genPt  = genJetP4.Pt();
      double genEta = genJetP4.eta();
      if (debugFlg) 
      cout << " ==    tauJet Pt = " << tau.pt()      << ", eta = " << tau.eta()      << ", phi = "  << tau.phi() << endl
           << " == genTauJet Pt = " << genPt << ", eta = " << genEta << ", phi = " << genJetP4.phi()
           << " dr = " << dr << ", decayMode = " << decayMode
           << endl;
      genTauList.push_back(genJetP4);


      double d = (tau_pt>0) ? (tau_pt - genPt)/tau_pt : -1;
      ptDiffH->Fill(d);
      ptScatH->Fill(genPt, tau_pt);
      tauDecayModeH->Fill(decayMode);
      if (decayMode < 2) {
        maxHcalEnergy2H->Fill(tau_had_energy);
        emEnergyFrac2H->Fill(tau_em_energy);
      }
      else { 
        invMassGenH->Fill(genJetP4.M());
      }

      // plot GenPt and GenEta for all and discriminated Taus
      tauPtGenH->Fill(genPt);
      tauEtaGenH->Fill(genEta);
      if (tau_disc > 0.0) {
        tauPtDiscH->Fill(genPt);
        tauEtaDiscH->Fill(genEta);
      }
    }
  }
  
  if (nMatchedJet > 0) nStat[2]++;
  if (nMatchedJet > 1) nStat[3]++;
  
  if (nLeadingTrackInJet > 0) nStat[4]++;
  if (nLeadingTrackInJet > 1) nStat[5]++;

  if (nDiscriminatedJet > 0) nStat[6]++;
  if (nDiscriminatedJet < 2) return;

  nStat[7]++;

  nTightTauH->Fill(nTightTau);
  nLooseTauH->Fill(nLooseTau);
  if (!nTightTau) return;
  nStat[8]++;

  // At this point call all other routines
  //  findIsolatedTaus(iEvent, iSetup);
  findMET(iEvent, iSetup);
  //findMuons(iEvent, iSetup);
  //findElectrons(iEvent, iSetup);
  //findInvMassFromCalo(iEvent, iSetup);
  //findTauImpactParameter(iEvent, iSetup);

  if (selectMap.size() < 2) return;

  // Invariant mass of the tau pair
  pair<unsigned int, unsigned int> tauPairIndex = getTauIndices(selectMap);
  if (debugFlg) cout << " Tau indices=" << tauPairIndex.first << ", " << tauPairIndex.second << endl;

  PFTauRef theTau1(tauHandle, tauPairIndex.first);
  PFTauRef theTau2(tauHandle, tauPairIndex.second);
  const PFTau& tau1 = (*theTau1);
  const PFTau& tau2 = (*theTau2);
  if (tau1.charge() * tau2.charge() != -1 ) return;
  nStat[9]++;

  if (debugFlg) cout << " Tau Momenta " << tau1.pt() << " , " << tau2.pt() << endl;
  const math::XYZTLorentzVector& P1 = tau1.p4();
  const math::XYZTLorentzVector& P2 = tau2.p4();
  math::XYZTLorentzVector ZP4 = P1 + P2;
  double mass = ZP4.M();
  if (debugFlg) cout << "Reconstructed Z Mass = " << mass << endl;
  zMassH->Fill(mass);
  zEtaH->Fill(ZP4.eta());    
  zPhiH->Fill(ROOT::Math::VectorUtil::Phi_0_2pi(ZP4.phi()));    
  
  math::XYZVector vec1(P1.x(), P1.y(), P1.z());    
  math::XYZVector vec2(P2.x(), P2.y(), P2.z());    
  double deltaR = ROOT::Math::VectorUtil::DeltaR(vec1, vec2);
  deltaRH->Fill(deltaR);
  
  double deltaPhi = ROOT::Math::VectorUtil::DeltaPhi(vec1, vec2);
  deltaPhiH->Fill(deltaPhi);
  
  double cosTheta = ROOT::Math::VectorUtil::CosTheta(vec1, vec2);
  cosThetaH->Fill(cosTheta);

  if (mass < minZMass_ || mass > maxZMass_) return;
  nStat[10]++;

  // Find Tau Decay mode
  tauDecayModes.clear();
  findTauDecayMode(iEvent, iSetup, tauDecayModes, false);
  fTauDecayModeH->Fill(tauDecayModes[0]);
  sTauDecayModeH->Fill(tauDecayModes[1]);

  // Final efficiency for  \taus decaying hadronically
  if ((tauDecayModes[0] == 2) && (tauDecayModes[1] == 2))  
    nStat[11]++;

  // Angle of a tau with the Z etc. 
}
void EWKTauAnalyser::findIsolatedTaus(const edm::Event& iEvent, const EventSetup& iSetup) 
{
  // Isolated Taus
  Handle<IsolatedTauTagInfoCollection> tauTagInfoHandle;
  iEvent.getByLabel(jetTagSrc_, tauTagInfoHandle);

  if (tauTagInfoHandle.isValid()) {
    const IsolatedTauTagInfoCollection& tauTagInfoColl = *(tauTagInfoHandle.product());
    if (debugFlg) cout << "Found " << tauTagInfoColl.size() << " Tau candidates" << endl;
      
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
        = iter->leadingSignalTrack(matchingCone_, leadingTrackPt_);
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

      double discriminator = iter->discriminator(matchingCone_, signalCone_, isolationCone_, leadingTrackPt_, minimumTrackPt_);
      tauDiscOrigH->Fill(discriminator);

      if (debugFlg) 
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
  iEvent.getByLabel(metSrc_, recoMetHandle);
  if (recoMetHandle.isValid()) {
    const CaloMETCollection& recoMetColl = *(recoMetHandle.product());

    // Loop over MET objects
    vector<math::XYZTLorentzVector> metList;
    for (CaloMETCollection::const_iterator it  = recoMetColl.begin();
                                           it != recoMetColl.end();
                                           it++)
    {
      const CaloMET& obj = (*it);
      double  met  = obj.et();
      double sumEt = obj.sumEt();
      double phi   = obj.phi();
      double mEtX  = obj.px();
      if (met < 5.0) continue;
      metList.push_back(obj.p4());
      if (debugFlg) cout << "MET: " << met << ", " << sumEt << ", " << phi << ", " << mEtX << endl;

      metH->Fill(met);
      sumEtH->Fill(sumEt);
      metSigH->Fill(obj.mEtSig());
    }

    //-- Jets
    Handle<CaloJetCollection> caloJetHandle; 
    iEvent.getByLabel(caloJetSrc_, caloJetHandle);
    const CaloJetCollection& caloJetColl = *( caloJetHandle.product() );
    if (debugFlg >= 2) 
      cout << setiosflags(ios::left) << setw(30) 
           << " # jets:" << caloJetColl.size() 
           << endl;
#if 0
    const JetCorrector* corrector = JetCorrector::getJetCorrector(jetCorrector_, iSetup);
    for (CaloJetCollection::const_iterator ijet = caloJetColl.begin(); ijet != caloJetColl.end(); ++ijet )   {
      double corr = corrector->correction(*ijet);
    }
    

    // Corrected Met 
    // type1 correction
    Type1METAlgo fType1METAlgo;
    std::auto_ptr<reco::CaloMETCollection> type1MET( new reco::CaloMETCollection() );  //Create empty output
    fType1METAlgo.run( recoMetColl, *corrector, caloJetColl, jetEtMinMETCorr_, jetEMfracMETCorr_, &*type1MET ); // Invoke the algorithm
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
  iEvent.getByLabel(muonSrc_, muonHandle);
  if (muonHandle.isValid()) {
    const MuonCollection& recoMuonColl = *(muonHandle.product());
    int nMuons = recoMuonColl.size();
    if (debugFlg) cout << "number of Offline " << muonSrc_ << " muons=" << nMuons << endl;

    for (MuonCollection::const_iterator m = recoMuonColl.begin(); m != recoMuonColl.end(); m++) {
      if (debugFlg) 
        cout << "Muon: pt= " << m->pt()
             << " eta= "     << m->eta()
             << " phi= "     << m->phi()
             << endl;
    }
  }
}
void EWKTauAnalyser::findElectrons(const edm::Event& iEvent, const EventSetup& iSetup) 
{
  Handle<PixelMatchGsfElectronCollection> electronHandle; 
  iEvent.getByLabel(electronSrc_, electronHandle);
  if (electronHandle.isValid()) {
    const PixelMatchGsfElectronCollection& recoElectronColl = *( electronHandle.product() );
    if (debugFlg >= 2) 
      cout << setiosflags(ios::left) << setw(30) 
           << " # electrons:" << recoElectronColl.size() << endl;
    for (PixelMatchGsfElectronCollection::const_iterator e  = recoElectronColl.begin(); 
                                                         e != recoElectronColl.end(); 
                                                         e++) {
      if (debugFlg) 
        cout << "Electron: pt= " << e->pt()
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
                                        f++)
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
                                                           f++)
  {
    const GenParticleCandidate* gp = (*f);
    int pid = gp->pdgId();
    if (debugFlg)
    cout << "PDG Code = " << pid << endl
         << " PDG Code Mother = ";
    for (unsigned int i = 0; i < gp->numberOfMothers(); i++) {
      const Candidate* mother = gp->mother();
      int mid = mother->pdgId();
      int status = mother->status();
      if (debugFlg) cout << "\t" << mid << ", status = " << status;
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
    if (debugFlg) cout << endl;
  }
  if (!tauFound) return false;

  genJetP4 = const_cast<math::XYZTLorentzVector&>(j.p4());
  return true;
}

void EWKTauAnalyser::findInvMassFromCalo(const edm::Event& iEvent, const EventSetup& iSetup) 
{
  Handle<TauMassTagInfoCollection> massTagInfoHandle;
  iEvent.getByLabel(caloInvMassSrc_, massTagInfoHandle);

  if (massTagInfoHandle.isValid()) {
    const TauMassTagInfoCollection& massTagInfoColl = *(massTagInfoHandle.product());

    for (TauMassTagInfoCollection::const_iterator iter  = massTagInfoColl.begin(); 
	                                          iter != massTagInfoColl.end();
                                                 ++iter) {
      const IsolatedTauTagInfo& isolatedTauTag = *(iter->getIsolatedTauTag());
      double discriminator = isolatedTauTag.discriminator(matchingCone_, signalCone_, isolationCone_, leadingTrackPt_, minimumTrackPt_);
      
      if (discriminator == 0) continue;
      double invMass = iter->getInvariantMass(matchingCone_, leadingTrackPt_, signalCone_, clusterTrackMatchingCone_); 
      if (fabs(invMass - 0.139) < 0.01) invMass = 0.0;
      if (invMass > -1.0) caloInvMassH->Fill(invMass);
    } 
  }
  else {
    cerr << "ERROR. TauMassTagInfoCollection missing!" << endl;
  }
}
void EWKTauAnalyser::findTauImpactParameter(const edm::Event& iEvent, const EventSetup& iSetup) 
{
  Handle<TauImpactParameterInfoCollection> ipInfoHandle;
  iEvent.getByLabel(ipSrc_, ipInfoHandle);

  if (ipInfoHandle.isValid()) {
    const TauImpactParameterInfoCollection& ipInfoColl = *(ipInfoHandle.product());
    for (TauImpactParameterInfoCollection::const_iterator iter  = ipInfoColl.begin(); 
	                                                  iter != ipInfoColl.end();
                                                        ++iter) 
    {
      const IsolatedTauTagInfo& isolatedTauTag = *(iter->getIsolatedTauTag());
      double discriminator = isolatedTauTag.discriminator(matchingCone_, signalCone_, 
                                       isolationCone_, leadingTrackPt_, minimumTrackPt_);
      if (discriminator == 0) continue;

      const Jet& theJet = *(isolatedTauTag.jet());
      double jetEt = theJet.et();
      double jetEta = theJet.eta();
      if (debugFlg) 
        cout << " EWKTauAnalyser::findTauImpactParameter. Candidate jet Et = " << jetEt  
             << ", isolation discriminator = "<< discriminator << endl;

      if (jetEt < 0 || jetEt > 150) continue;
      if (fabs(jetEta) > 2.2) continue;

      double cone  = 0.1;
      double ptmin = 6;
      const TrackRef leadingTrack = isolatedTauTag.leadingSignalTrack(cone,ptmin);
      if (!leadingTrack) continue;
      if (leadingTrack->recHitsSize() < 8 || leadingTrack->normalizedChi2() > 10) continue;

      const reco::TauImpactParameterTrackData* trackData = iter->getTrackData(leadingTrack);
      if (!trackData) continue;
      Measurement1D tip(trackData->transverseIp);
      Measurement1D tzip(trackData->ip3D);

      ip2DH->Fill(fabs(tip.value())*10); // in mm
      ip2DSigH->Fill(tip.significance());
      
      ip3DH->Fill(fabs(tzip.value())*10); // in mm
      ip3DSigH->Fill(tzip.significance());
    } 
  }
  else {
    cerr << "EWKTauAnalyser::findTauImpactParameter. TauImpactParameterInfoCollection missing!" << endl;
  }
}

void EWKTauAnalyser::endJob() { 
  theFile->cd();

  // Add the event statistics to a histogram
  for (unsigned int i = 0; i < NEL(nStat); i++)
    statH->SetBinContent(i+1, nStat[i]);

  theFile->Write();
  theFile->Close();

  // Print out Event Statistics
  if (fStatLog_) {
    fStatLog_ << "========================================================= " << endl
              << "              Event Statistics                            " << endl
              << "========================================================= " << endl  
              << "                        Total Number of Events = " << setw(8) << nStat[0] << endl
              << "    Events with atleast one object in Tau Coll = " << setw(8) << nStat[1] << endl
              << "  Events with atleast one Jet matched with HLT = " << setw(8) << nStat[2] << endl
              << "         Events with two Jets matched with HLT = " << setw(8) << nStat[3] << endl
              << "Events with atleast one Jet with Leading Track = " << setw(8) << nStat[4] << endl
              << "      Events with two Jets with Leading Tracks = " << setw(8) << nStat[5] << endl
              << "     Events with atleast one discriminated Tau = " << setw(8) << nStat[6] << endl
              << "            Events with two discriminated Taus = " << setw(8) << nStat[7] << endl
              << "             Events with atleast one Tight Tau = " << setw(8) << nStat[8] << endl
              << "   Events with two selected Taus (Tight+Loose) = " << setw(8) << nStat[9] << endl
              << "  Events with Z0 Mass in (" 
              <<         setprecision(1)<< setw(6) <<minZMass_ 
              <<                    " - " 
              << setprecision(1)<< setw(6) <<maxZMass_ << " GeV) = " << setw(8) << nStat[10] << endl
              << "               Both Taus decaying hadronically = " << setw(8) << nStat[11] << endl            
              << "======================================================== " << endl;  

    fStatLog_ << resetiosflags(ios::fixed);
    fStatLog_.close();
  }
}

int EWKTauAnalyser::findTauDecayMode(const edm::Event& iEvent, 
                                     const edm::EventSetup& iSetup, 
                                     vector<int>& tauTypeList, bool fillHist) 
{
  edm::Handle<edm::HepMCProduct> genParticleHandle;
  iEvent.getByLabel(genParticles_, genParticleHandle);

  int decay_mode = -1;
  HepMC::GenEvent* generated_event = new HepMC::GenEvent(*(genParticleHandle->GetEvent()));
  math::XYZTLorentzVector myTauP4(0.,0.,0.,0.);
  for (HepMC::GenEvent::particle_iterator p  = generated_event->particles_begin();
                                          p != generated_event->particles_end(); 
                                          p++)
  {
    if (abs((*p)->pdg_id()) == 15 && (*p)->status() == 2) {
      math::XYZTLorentzVector tauP4((*p)->momentum().px(),(*p)->momentum().py(),(*p)->momentum().pz(),(*p)->momentum().e());
      bool lept_decay = false;
      HepMC::GenVertex::particle_iterator z = (*p)->end_vertex()->particles_begin(HepMC::descendants);
      int ndesc = 0;
      int ngamma = 0;
      for (; z != (*p)->end_vertex()->particles_end(HepMC::descendants); z++) {
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

  if (debugFlg) {
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
int EWKTauAnalyser::selectTau(const reco::PFTau& tau) {
  double mass            = tau.alternatLorentzVect().M();
  double pt              = tau.pt();
  double leading_trk_pt  = (*(tau.leadPFChargedHadrCand())).pt();
  double charge          = tau.charge();
  int sig_ch_hadrons     = tau.signalPFChargedHadrCands().size();  
  double maxHadEnergy    = tau.maximumHCALPFClusterEt();

  int isel = 0;
  if (maxHadEnergy < 0.2)                          isel |= (1 << 6);
  if (fabs(charge) != 1.0)                         isel |= (1 << 5);
  if (sig_ch_hadrons%2 == 0)                       isel |= (1 << 4);
  if (mass < 0.0 ||  mass > maxTauMass_)           isel |= (1 << 3);
  if (leading_trk_pt/pt < leadingTrkPtFrac_)       isel |= (1 << 2); // a stricter cut means 1-prong taus
  if (leading_trk_pt < looseTauLeadingTrkPt_)      isel |= (1 << 1);
  if (leading_trk_pt < tightTauLeadingTrkPt_)      isel |= (1 << 0);

  return isel;
}
//
// -- Tight Tau
//
pair<unsigned int, unsigned int> EWKTauAnalyser::getTauIndices(map<unsigned int, unsigned int>& smap) {
  unsigned int indx1 = 999; 
  unsigned int indx2 = 999;
  
  for (map<unsigned int, unsigned int>::iterator it = smap.begin();
                                                it != smap.end(); it++) {
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
void EWKTauAnalyser::findInvariantMass(const reco::PFTau& tau) {

   PFCandidateRefVector sigCands = tau.signalPFCands();

   math::XYZTLorentzVector p1;
   for (PFCandidateRefVector::const_iterator iCand =sigCands.begin();iCand!=sigCands.end();iCand++){
     p1 += (**iCand).p4();
   }
   invMass3H->Fill(p1.M());
 
   math::XYZTLorentzVector p2;   

   PFCandidateRefVector chHadCands   = tau.signalPFChargedHadrCands();
   PFCandidateRefVector gammaCands   = tau.pfTauTagInfoRef()->PFGammaCands();
   PFCandidateRefVector neutralCands = tau.pfTauTagInfoRef()->PFNeutrHadrCands();
   
   for (PFCandidateRefVector::const_iterator iHad = chHadCands.begin();iHad!=chHadCands.end();iHad++){
     p2 += (**iHad).p4();
   }

   const TrackRef leadTrack =  tau.leadTrack();
   math::XYZVector vec1(leadTrack->px(),leadTrack->py(),leadTrack->pz());

   // Add photons only 
   for (PFCandidateRefVector::const_iterator iGam = gammaCands.begin();iGam!=gammaCands.end();iGam++){
     math::XYZVector vec2((**iGam).px(), (**iGam).py(), (**iGam).pz());
     double deltaR = ROOT::Math::VectorUtil::DeltaR(vec1, vec2);
     if (deltaR < 0.4) p2 += (**iGam).p4();
   }
   invMass4H->Fill(p2.M());
   
   // add also neutral hadrons
   for (PFCandidateRefVector::const_iterator iNeu = neutralCands.begin();iNeu!=neutralCands.end();iNeu++){
     math::XYZVector vec2((**iNeu).px(), (**iNeu).py(), (**iNeu).pz());
     double deltaR = ROOT::Math::VectorUtil::DeltaR(vec1, vec2);
     if (deltaR < 0.4) p2 += (**iNeu).p4();
   }
  
   invMass5H->Fill(p2.M());
}
//
// -- Match Tau Jet with HLT
//
/*bool EWKTauAnalyser::matchWithHLTJet(const PFTau& tau, const Handle<reco::HLTFilterObjectWithRefs>& hltHandle, double& dr)
{
  math::XYZVector tau3Vec(tau.momentum());
 for( unsigned int ic=0; ic <hltHandle->size();ic++) {
   edm::RefToBase<reco::Candidate> tauCandRef = hltHandle->getParticleRef(ic);
   cout <<"Eta and Phi HLT "<<tauCandRef->eta() <<" "<<tauCandRef->phi()<<endl;
   cout <<"Eta and Phi PFJets "<<tau.eta() <<" "<<tau.phi()<<endl;
   double deltaR = ROOT::Math::VectorUtil::DeltaR(tau3Vec, tauCandRef->p4().Vect());
    if (deltaR < dr) {
      dr = deltaR;
    }
  }
  if (dr > 0.1) return false;
  else return true;
}*/
//
// -- Match Tau Jet with HLT
//
bool EWKTauAnalyser::matchWithHLTJet(const PFTau& tau, const CaloJetCollection& caloJets, double& dr)
{
  math::XYZVector tau3Vec(tau.momentum());
  for (CaloJetCollection::const_iterator ic  = caloJets.begin();
                                        ic != caloJets.end(); ic++) {
     double deltaR = ROOT::Math::VectorUtil::DeltaR(tau3Vec, ic->momentum());
    if (deltaR < dr) {
      dr = deltaR;
    }
  }
  if (dr > 0.3) return false;
  else return true;
}
DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(EWKTauAnalyser);
