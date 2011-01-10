// -*- C++ -*-
//
// Package:    TTEffAnalyzer
// Class:      TTEffAnalyzer
// 
/**\class TTEffAnalyzer TTEffAnalyzer.cc ElectroWeakAnalysis/TTEffAnalyzer/src/TTEffAnalyzer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Chi Nhan Nguyen
//         Created:  Wed Oct  1 13:04:54 CEST 2008
// $Id: TTEffAnalyzer.h,v 1.41 2011/01/03 12:59:37 slehti Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/LorentzVectorFwd.h"
#include "DataFormats/TauReco/interface/PFTauDiscriminator.h"

#include "FWCore/Common/interface/TriggerNames.h"

#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/L1TauEfficiencyAnalyzer.h"
#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/L25and3TauEfficiencyAnalyzer.h"
#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/L2TauEfficiencyAnalyzer.h"
#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/METEfficiencyAnalyzer.h"
#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/PFMHTEfficiencyAnalyzer.h"
#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/HLTJetEfficiencyAnalyzer.h"


#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

//
class TTEffAnalyzer : public edm::EDAnalyzer {
   public:
      explicit TTEffAnalyzer(const edm::ParameterSet&);
      ~TTEffAnalyzer();

      TTree *_TTEffTree;
      TFile *_TTEffFile;


   private:
      typedef math::XYZTLorentzVector LorentzVector;

      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      template <class T> void loop(const edm::Event& iEvent,const edm::EventSetup& iSetup, const T& collection);

      virtual void fillLV(const LorentzVector&,unsigned int i = 0); // was fill() before, but was confusing
      virtual void fill(const reco::PFTau&,unsigned int i = 0); 
      virtual void fill(const LorentzVector&,unsigned int i = 0); // this one is for the loop per MCtau
      virtual void fill(const reco::Candidate&,unsigned int i = 0);
      virtual void fillHLTinfo(const edm::Event&);


      //Helper function :RMS of the PF Candidates
      std::vector<double> clusterSeparation(const reco::PFCandidateRefVector& ,const reco::PFCandidateRefVector& );
  
      void clusterShape(const reco::PFTau& tau, std::vector<double>& rms) const;
      void clusterShape(const math::XYZTLorentzVectorCollection& clusters, std::vector<double>& rms) const;
      void getPFClusters(const PFCandidateRefVector& pfCands, math::XYZTLorentzVectorCollection& clusters) const;
      bool checkPos(const std::vector<math::XYZPoint>& CalPos, const math::XYZPoint& CandPos) const;

      // ----------member data ---------------------------
      bool DoMCTauEfficiency_;
      edm::InputTag HLTResultsSource;
      edm::TriggerNames _triggerNames;
      edm::InputTag  PFTaus_,PFTauIso_,MCTaus_,MCParticles_,PFTauMuonRej_; //Path to analyze
      std::string rootFile_;

      int _HltEvtCnt;
      bool *_hltFlag;

      edm::Handle<PFTauCollection> PFTaus;
      edm::Handle<PFTauDiscriminator> thePFTauDiscriminatorByIsolation;
      edm::Handle<PFTauDiscriminator> thePFTauDiscriminatorAgainstMuon;
      edm::Handle<std::vector<LorentzVector> > mcTaus;
      edm::Handle<reco::GenParticleCollection> mcParticles;

      // PF Variables
      uint32_t b_event, b_run, b_lumi;
      int NEGCandsInAnnulus,NHadCandsInAnnulus,MCMatch,PFTauMatch,PFMuonMatch,PFElectronMatch;
      float MCTauE,MCTauEt,MCTauEta,MCTauPhi;
      float PFPt,PFInvPt,PFEt,PFEta,PFPhi,PFProng,PFIso,PFIsoSum,PFEnergy;
      float PFClusterEtaRMS, PFClusterPhiRMS, PFClusterDrRMS;
      float pfJetChargedEmEnergy, pfJetChargedEmEnergyFraction, pfJetChargedHadronEnergy, pfJetChargedHadronEnergyFraction;
      float pfJetNeutralEmEnergy, pfJetNeutralEmEnergyFraction, pfJetNeutralHadronEnergy, pfJetNeutralHadronEnergyFraction;
      float PFSignalSumPt,PFIsoNTrks,PFIsoTrkNHits,PFIsoTrkChi2,PFIsoTrkPt;
      double MCMatchingCone;
      L1TauEfficiencyAnalyzer _L1analyzer;
      L2TauEfficiencyAnalyzer _L2analyzer;
      L25and3TauEfficiencyAnalyzer _L25and3analyzer;
      METEfficiencyAnalyzer _METanalyzer;
      PFMHTEfficiencyAnalyzer _PFMHTanalyzer;
      HLTJetEfficiencyAnalyzer _HLTJetAnalyzer;

};

