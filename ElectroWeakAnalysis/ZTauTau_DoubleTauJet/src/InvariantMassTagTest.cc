// -*- C++ -*-
//
// Package:    InvariantMassTagTest
// Class:      InvariantMassTagTest
// 
/**\class InvariantMassTagTest InvariantMassTagTest.cc RecoTauTag/InvariantMass/test/InvariantMassTagTest.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Suchandra Dutta
//         Wed Nov 15 21:18:17 CEST 2006
// $Id: InvariantMassTagTest.cc,v 1.12 2006/07/17 15:12:35 gennai Exp $
//
// user include files
// system include files
#include <memory>
#include <iostream>

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/BTauReco/interface/JetTracksAssociation.h"
#include "DataFormats/BTauReco/interface/IsolatedTauTagInfo.h"
#include "DataFormats/BTauReco/interface/TauMassTagInfo.h"
#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/TauReco/interface/PFTauDiscriminatorByIsolation.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/Math/interface/Vector3D.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/ParticleFlowReco/interface/PFSimParticle.h"
#include "DataFormats/ParticleFlowReco/interface/PFSimParticleFwd.h"


#include "RecoTauTag/RecoTau/test/InvariantMassTagTest.h"

#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>

using namespace reco;
using namespace edm;
using namespace std;

InvariantMassTagTest::InvariantMassTagTest(const edm::ParameterSet& iConfig)
{
            MassTagProducer_    = iConfig.getUntrackedParameter<string>("MassTagProd", "invariantMass");
            PFTauProducer_      = iConfig.getUntrackedParameter<string>("PFTauProducer", "pfRecoTauProducer");
  PFTauDiscByIsolationProducer_ = iConfig.getUntrackedParameter<string>("PFTauDiscriminatorByIsolationProducer","pfRecoTauDiscriminationByIsolation");
                GenJetProducer_ = iConfig.getUntrackedParameter<InputTag>("GenJetProducer");
           GenParticleProducer_ = iConfig.getUntrackedParameter<string>("genParticles", "source");
         PFSimParticleProducer_ = iConfig.getUntrackedParameter<string>("PFSimParticleProducer", "particleFlowSimParticle");
         TrackingTruthProducer_ = iConfig.getUntrackedParameter<InputTag>("TrackingTruthProducer");
           

             min_tau_jet_pt = iConfig.getUntrackedParameter<double>("MinTauJetPt",8.0);
              matching_cone = iConfig.getUntrackedParameter<double>("MatchingCone",0.1);
             leading_trk_pt = iConfig.getUntrackedParameter<double>("LeadingTrackPt",6.0);
                signal_cone = iConfig.getUntrackedParameter<double>("SignalCone", 0.07);
             isolation_cone = iConfig.getUntrackedParameter<double>("IsolationCone", 0.4);
  cluster_jet_matching_cone = iConfig.getUntrackedParameter<double>("ClusterJetMatchingCone",0.4);
  cluster_trk_matching_cone = iConfig.getUntrackedParameter<double>("ClusterTrackMatchingCone",0.08);
               inv_mass_cut = iConfig.getUntrackedParameter<double>("InvariantMassCutoff",2.5);
   cout << "InvariantMass Constructor Ready ..." << endl;
}

void InvariantMassTagTest::beginJob(const edm::EventSetup& iSetup)
{
  cout << "Entering InvariantMass::beginJob ..." << endl;
  nEvent = 0;

  edm::Service < TFileService > fs;
  TFileDirectory dir = fs->mkdir("PFAnalysis");

    discriminatorValue = dir.make<TH1F>("Discriminator",    "Value of Discriminator", 2, -0.5, 1.5);
    invMassFromPFJet = dir.make<TH1F>("InvMassPFJet",    "Invariant Mass", 100, 0.0, 10.0);
             invMass = dir.make<TH1F>("InvMass",         "Invariant Mass (Alternate Lorentz Vector)", 100, 0.0, 10.0);
      invMassSigComp = dir.make<TH1F>("InvMassSigComponents", "Invariant Mass (Signal Components)", 100, 0.0, 10.0);
            invMass1 = dir.make<TH1F>("InvMassCustomI",  "Invariant Mass (custom I )", 100, 0.0, 10.0);
            invMass2 = dir.make<TH1F>("InvMassCustomII", "Invariant Mass (custom II )", 100, 0.0, 10.0);
            invMass3 = dir.make<TH1F>("InvMassCustomIII","Invariant Mass (custom III )", 100, 0.0, 10.0);
 invMassFromSigChHad1= dir.make<TH1F>("InvMassSigChHad1","Invariant Mass From Ch.Hadron in SigCone (1 track)",100,0.0,10.0);

         nGamPFJet = dir.make<TH1F>("nGammaPFJet",        "No. of Gammas in PF Jet",          21, -0.5, 20.5);

     nNeutalHadPFJet = dir.make<TH1F>("nNeuHadPFJet",       "No. of Neutral Hadrons in PF Jet", 21, -0.5, 20.5);
    nChargedHadPFJet = dir.make<TH1F>("nChargedHadPFJet",   "No. of Charged Hadrons in PF Jet", 21, -0.5, 20.5); 
       nGamSigCone = dir.make<TH1F>("nGammaSigCone",      "No. of Gammas in SigCone",          21, -0.5, 20.5);
   nNeutalHadSigCone = dir.make<TH1F>("nNeuHadSigCone",     "No. of Neutral Hadrons in SigCone", 21, -0.5, 20.5);
  nChargedHadSigCone = dir.make<TH1F>("nChargedHad",        "No. of Charged Hadrons in SigCone", 21, -0.5, 20.5);
      nSigCandPFJet   = dir.make<TH1F>("nSigCandPFJet", "No. of Candidates in Signal Cone", 31, -0.5, 30.5);
       nGamSelected= dir.make<TH1F>("nGammaSelected",     "No. of Gammas in Selected",         21, -0.5, 20.5); 
 EtChargedHadSigCone = dir.make<TH1F>("EtChargedHadSigCone","Et of Charged Hadrons in SigCone", 100, 0.0, 100.0);
 EtNeutralHadSigCone = dir.make<TH1F>("EtNeutralHadSigCone","Et of Neutral Hadrons in SigCone", 100, 0.0, 100.0);
     EtGamSigCone  = dir.make<TH1F>("EtGammaSigCone",    "Et of Gamma in SigCone",           100, 0.0, 100.0);


        invMassHepMC = dir.make<TH1F>("InvMassHepMC",     "Invariant Mass from HepMC ", 100, 0.0, 10.0);
    invMassHepMCTrk1 = dir.make<TH1F>("InvMassHepMCTrk1", "Invariant Mass from HepMC (1 track)", 100, 0.0, 10.0);

      invMassTrueGam = dir.make<TH1F>("InvMassWithTrueGamma", "Invariant Mass using True Gammas)",100, 0, 10.);
            nGamTrue = dir.make<TH1F>("nGammaTrue", "No. of True Gammas", 21, -0.5, 20.5); 
             EResTau = dir.make<TH1F>("TauEnergyRes", "Energy Resolution of Tau Jet ", 100, -1.0, 1.0); 
      EResTauSigCone = dir.make<TH1F>("TauEnergyResSigCone", "Energy Resolution of Tau Jet Constituents in Signal Cone", 100, -1.0, 1.0); 
         EResTauVsDR = dir.make<TH2F>("TauEnergyResVsDR", "Energy Resolution of Tau Jet Vs dR wrt GenJet",50, 0.0, 0.1,  100, -1.0, 1.0); 
  EResTauSigConeVsDR = dir.make<TH2F>("TauEnergyResSigConeVsDR", "Energy Resolution of Tau Jet Constituents in Signal Cone",  50, 0.0, 0.1,  100, -1.0, 1.0); 

       invMassGenJet = dir.make<TH1F>("InvMassGenJet", "Gen Jet Invariant Mass (all constituents",100, 0, 10.);
     invMassGenJetPt = dir.make<TH1F>("InvMassGenJetPtcut", "Gen Jet Invariant Mass (constituents with pt>1GeV)",100, 0, 10.);
invMassGenJetSigCone = dir.make<TH1F>("InvMassGenJetSigCone", "Gen Jet Invariant Mass (constituents in SigCone)",100, 0, 10.);
     iDGenJetSigCone = dir.make<TH1F>("IdGenJetConstituentsSigCone", "PDG Id Gen Jet Constituents (in SigCone)",501, -0.5, 500.5);
   nGamGenJetSigCone = dir.make<TH1F>("nGammaGenJetSigCone", "No. of Gammas in Gen Jet (in SigCone)",21, -0.5, 20.5);
  nCandGenJetSigCone = dir.make<TH1F>("nCandGenJetSigCone", "No. of Constituents in Gen Jet (in SigCone)",21, -0.5, 20.5);

 ptGamGenJetSigCone = dir.make<TH1F>("PtGammaGenJetSigCone", "Pt of Gammas of GenJet SigCone", 120, 0.0, 60.0);
 ptCandGenJetSigCone  = dir.make<TH1F>("PtCandGenJetSigCone",  "Pt of ALL Constituents of GenJet SigCone", 120, 0.0, 60.0);
 ptGamGenJetSigConeNotMatched = dir.make<TH1F>("PtGammaGenJetSigConeNotMatched", "Pt of Gammas of GenJet SigCone (not matched with PF)", 120, 0.0, 60.0);
 ptCandGenJetSigConeNotMatched = dir.make<TH1F>("PtCandGenJetSigConeNotMatched", "Pt of ALL Constituents of GenJet SigCone (not matched with reco)", 120, 0.0, 60.0);

 etaGamGenJetSigCone = dir.make<TH1F>("EtaGammaGenJetSigCone", "Eta of Gammas of GenJet SigCone", 100, -4.0, 4.0);
 etaCandGenJetSigCone  = dir.make<TH1F>("EtaCandGenJetSigCone",  "Eta of ALL Constituents of GenJet SigCone", 100, -4.0, 4.0);
 etaGamGenJetSigConeNotMatched = dir.make<TH1F>("EtaGammaGenJetSigConeNotMatched", "Eta of Gammas of GenJet SigCone (not matched with PF)", 100, -4.0, 4.0);
 etaCandGenJetSigConeNotMatched = dir.make<TH1F>("EtaCandGenJetSigConeNotMatched", "Eta of ALL Constituents of GenJet SigCone (not matched with reco)", 100, -4.0, 4.0);
  EresCandGenJetSigConeMatched = dir.make<TH1F>("EneryResCandGenJetSigCone", "Energy Resolution of ALL Constituents of GenJet SigCone", 100, -1.0, 1.0);
  EresGamGenJetSigConeMatched = dir.make<TH1F>("EneryResGamGenJetSigCone", "Energy Resolution of Gammas of  GenJet SigCone", 100, -1.0, 1.0);

  EresGamGenJetSigConeMatched1 = dir.make<TH1F>("EneryResGamGenJetSigCone_range1", "Energy Resolution of Gammas of  GenJet SigCone (E < 5GeV)", 100, -1.0, 1.0);
  EresGamGenJetSigConeMatched2 = dir.make<TH1F>("EneryResGamGenJetSigCone_range2", "Energy Resolution of Gammas of  GenJet SigCone (E: 5-10GeV)", 100, -1.0, 1.0);
  EresGamGenJetSigConeMatched3 = dir.make<TH1F>("EneryResGamGenJetSigCone_range3", "Energy Resolution of Gammas of  GenJet SigCone (E: 10-20GeV) ", 100, -1.0, 1.0);
  EresGamGenJetSigConeMatched4 = dir.make<TH1F>("EneryResGamGenJetSigCone_range4", "Energy Resolution of Gammas of  GenJet SigCone (E> 20GeV)", 100, -1.0, 1.0);
     nGamGenJetSigConeNotMatched = dir.make<TH1F>("nGamGenJetSigConeNotMatched", " # of Not Matched Gammas  of  GenJet SigCone", 26, -0.5, 25.0);

 fracCandGenJetSigConeNotMatched = dir.make<TH1F>("fracCandGenJetSigConeNotMatched", " Fraction of Not Matched GenJet Candidate in SigCone", 25, 0.0, 1.0);
 fracGamGenJetSigConeNotMatched = dir.make<TH1F>("fracGamGenJetSigConeNotMatched", " Fraction of Not Matched GenJet Gammas in SigCone", 25, 0.0, 1.0);

  rVtxGamGenJetSigConeNotMatched = dir.make<TH1F>("rVtxGamGenJetSigConeNotMatched", " Vertex position for Not Matched Gammas of GenJet SigCone",100, 0.0, 200.0); 
     rVtxGamGenJetSigConeMatched = dir.make<TH1F>("rVtxGamGenJetSigConeMatched", " Vertex position for Matched Gammas of GenJet SigCone",100, 0.0, 200.0);  
                 rVtxGamSimTrack = dir.make<TH1F>("rVtxGamSimTrack", " Vertex position for  All  Gammas from All Sim Tracks in the Event",100, 0.0, 200.0);
  nVtxGamGenJetSigConeNotMatched = dir.make<TH1F>("nVtxGamGenJetSigConeNotMatched", " # of Vertices for Not Matched Gammas of GenJet SigCone",7, -1.5, 5.5);
     nVtxGamGenJetSigConeMatched = dir.make<TH1F>("nVtxGamGenJetSigConeMatched", " # of Vertices for Matched Gammas of GenJet SigCone", 7, -1.5, 5.5); 
                 nVtxGamSimTrack = dir.make<TH1F>("nVtxGamSimTrack", " # of Vertices All  Gammas from All Sim Tracks in the Event", 7, -1.5, 5.5); 


 invMassRecGam1Prong  = dir.make<TH1F>("InvMass_RecGamma_1Prong", "Invariant Mass Signal Components (1 prong Tau)",100,0,10.);
 invMassTrueGam1Prong = dir.make<TH1F>("InvMass_TrueGam_1Prong","Invariant Mass with True Gams (1 prong Tau)",100,0,10.);
   invMassNoGam3Prong = dir.make<TH1F>("InvMass_NoGamma_3Prong",  "Invariant Mass Signal Components (3 prong Tau &  no Gammas)",100,0,10.);
  invMassRecGam3Prong = dir.make<TH1F>("InvMass_RecGamma_3Prong", "Invariant Mass Signal Components (3 prong HepMC)",100,0,10.);
 invMassTrueGam3Prong = dir.make<TH1F>("InvMass_TrueGamma_3Prong","Invariant Mass with True Gammas (3 prong Tau)",100,0,10.);
          MCTrkVsRecTrk = dir.make<TH2F>("TrueTrkVsRecTrk", " Generator Level Tracks Vs Reconstructed Tracks", 11, -0.5, 10.5, 11, -0.5, 10.5);


  cout << "Leaving InvariantMass::beginJob ..." << endl;
}
void InvariantMassTagTest::analyze(const Event& iEvent, const EventSetup& iSetup)
{
  using namespace edm;
  using namespace reco;
  
  cout << "InvariantMassTagTest::analyze => Run " << iEvent.id().run() 
                                     << " Event " << iEvent.id().event() << endl;

  Handle<TrackingParticleCollection> rawPH;
  iEvent.getByLabel(TrackingTruthProducer_,  rawPH);
  const TrackingParticleCollection& tkParticles = *(rawPH.product());

  Handle<GenJetCollection> genJetsHandle;
  iEvent.getByLabel(GenJetProducer_, genJetsHandle);
  const GenJetCollection& genJets = *(genJetsHandle.product());

  Handle<PFTauCollection> thePFTauHandle;
  iEvent.getByLabel(PFTauProducer_,thePFTauHandle);

  Handle<PFTauDiscriminatorByIsolation> theTauDiscriminatorByIsolation;
  iEvent.getByLabel(PFTauDiscByIsolationProducer_, theTauDiscriminatorByIsolation);

  if (rawPH.isValid()) {
    // Check and plot # of vertices of generated photons  
    for (TrackingParticleCollection::const_iterator itrack = tkParticles.begin(); 
	 itrack != tkParticles.end(); itrack++) {
      if ( itrack->pdgId() != 22 ||  itrack->charge() != 0) continue;
      for (TrackingParticle::genp_iterator hepT = itrack->genParticle_begin(); 
	   hepT !=  itrack->genParticle_end(); ++hepT) {
	nVtxGamSimTrack->Fill( itrack->decayVertices().size());
	if (itrack->decayVertices().size() > 0) {
	  TrackingParticle::tv_iterator iTV = itrack->decayVertices_begin();
	  double xpos = (**iTV).position().X();
	  double ypos = (**iTV).position().Y();
	  double rpos = TMath::Sqrt(xpos * ypos + ypos*ypos);
	  rVtxGamSimTrack->Fill(rpos);
	}
      }
    }
  }

  for (PFTauCollection::size_type iTau = 0; iTau < thePFTauHandle->size(); iTau++) {
    PFTauRef theTau(thePFTauHandle, iTau);
    if (!theTau) {
      cerr << "Strange! Should have a valid PFTauRef for iTau=" << iTau << endl;
      continue;
    }
    const PFTau& tau = (*theTau);
    if (tau.pt() < min_tau_jet_pt) continue;
    double tau_disc = (*theTauDiscriminatorByIsolation)[theTau];
    discriminatorValue->Fill(tau_disc);

    if (tau_disc > 0.0) {
      findTrueInvMass(iEvent);
      //      const PFTauTagInfo& tagInfo     = (*(tau.pfTauTagInfoRef()));
      if (rawPH.isValid()) fillGenJetInvMass(tau, genJets, tkParticles);
      findPFInvMass(iEvent, tau);
    }
  }
}
//
// -- Calculate Invariant Mass
//
void InvariantMassTagTest::findPFInvMass(const edm::Event& iEvent,const reco::PFTau& tau) {
   PFCandidateRefVector sigCands     = tau.signalPFCands();
   PFCandidateRefVector gammaCands   = tau.pfTauTagInfoRef()->PFGammaCands();
   PFCandidateRefVector neutralCands = tau.pfTauTagInfoRef()->PFNeutrHadrCands();
   PFCandidateRefVector sigChHadCands  = tau.signalPFChargedHadrCands();

   nGamPFJet->Fill(gammaCands.size());
   nSigCandPFJet->Fill(sigCands.size());
   nNeutalHadPFJet->Fill(neutralCands.size());
   nChargedHadPFJet->Fill(tau.pfTauTagInfoRef()->PFChargedHadrCands().size());

   nGamSigCone->Fill(tau.signalPFGammaCands().size());
   nChargedHadSigCone->Fill(sigChHadCands.size());
   EtNeutralHadSigCone->Fill(tau.signalPFNeutrHadrCands().size());

   invMassFromPFJet->Fill(tau.p4().M());

   double mass = tau.alternatLorentzVect().M();
   invMass->Fill(mass);
   
   math::XYZTLorentzVector sigCanP4;
   for (PFCandidateRefVector::const_iterator iCand =sigCands.begin();iCand!=sigCands.end();iCand++){
     sigCanP4 += (**iCand).p4();
   }
   invMassSigComp->Fill(sigCanP4.M());

   if (sigChHadCands.size() == 1) invMassFromSigChHad1->Fill(mass);


   math::XYZTLorentzVector chHadCandsP4(0.,0.,0.,0.);   
   for (PFCandidateRefVector::const_iterator iHad = sigChHadCands.begin();iHad!=sigChHadCands.end();iHad++){
     chHadCandsP4 += (**iHad).p4();
     EtChargedHadSigCone->Fill((**iHad).p4().Et());
   }
   invMass1->Fill(chHadCandsP4.M());

   math::XYZTLorentzVector gammaCandsP4(0.,0.,0.,0.);   
   const TrackRef leadTrack =  tau.leadTrack();
   math::XYZVector vec1(leadTrack->px(),leadTrack->py(),leadTrack->pz());
   // Add photons only 
   int nGamma = 0;
   for (PFCandidateRefVector::const_iterator iGam = gammaCands.begin();iGam!=gammaCands.end();iGam++){
     math::XYZVector vec2((**iGam).px(), (**iGam).py(), (**iGam).pz());
     double deltaR = ROOT::Math::VectorUtil::DeltaR(vec1, vec2);
     double et = (**iGam).p4().Et();
     EtGamSigCone->Fill(et);
     if (deltaR < cluster_jet_matching_cone && (et > 0.04)  && (et < 300.0)) {
       gammaCandsP4 += (**iGam).p4();
       nGamma++;
     }
   }
   nGamSelected->Fill(nGamma);
   math::XYZTLorentzVector chHadAndGammaP4 = chHadCandsP4 + gammaCandsP4;
   invMass2->Fill(chHadAndGammaP4.M());
   
   // add also neutral hadrons
   math::XYZTLorentzVector neutralCandsP4(0.,0.,0.,0.);   
   int nNeuHad = 0;
   for (PFCandidateRefVector::const_iterator iNeu = neutralCands.begin();iNeu!=neutralCands.end();iNeu++){
     math::XYZVector vec2((**iNeu).px(), (**iNeu).py(), (**iNeu).pz());
     double deltaR = ROOT::Math::VectorUtil::DeltaR(vec1, vec2);
     double et = (**iNeu).p4().Et();
     if (deltaR < cluster_jet_matching_cone && (et > 0.04) && (et < 300.0)) {
       neutralCandsP4 += (**iNeu).p4();
       EtNeutralHadSigCone->Fill((**iNeu).p4().Et());
       nNeuHad++;
     }
   }
     
   math::XYZTLorentzVector chHadAndGammaAndNeuP4 = chHadCandsP4 + gammaCandsP4 + neutralCandsP4;
   invMass3->Fill(chHadAndGammaAndNeuP4.M());


   math::XYZVector tau3vec(tau.px(),tau.py(),tau.pz());

   edm::Handle<edm::HepMCProduct> genParticleHandle;
   iEvent.getByLabel(GenParticleProducer_, genParticleHandle);

   HepMC::GenEvent* generated_event = new HepMC::GenEvent(*(genParticleHandle->GetEvent()));
   math::XYZTLorentzVector MCGammaP4(0.,0.,0.,0.);
   int ntruegamma = 0;
   int nchargedpion = 0;
   for (HepMC::GenEvent::particle_iterator p  = generated_event->particles_begin();
                                          p != generated_event->particles_end(); 
                                          p++)
  {
    if (abs((*p)->pdg_id()) == 15 && (*p)->status() == 2) {
      math::XYZVector truetau3Vec((*p)->momentum().px(), (*p)->momentum().py(),(*p)->momentum().pz());      
      double deltaR = ROOT::Math::VectorUtil::DeltaR(tau3vec, truetau3Vec);
      if (deltaR > 0.1) continue;
      HepMC::GenVertex::particle_iterator z = (*p)->end_vertex()->particles_begin(HepMC::descendants); 
      for (; z != (*p)->end_vertex()->particles_end(HepMC::descendants); z++) {
        // count pi+- 
        if (abs((*z)->pdg_id()) == 211) nchargedpion++;
          
	if (abs((*z)->pdg_id()) == 22 ) { // gammas from pi0 
          HepMC::GenParticle* mother = (*((*z)->production_vertex())->particles_in_const_begin());
          if (mother->pdg_id() == 111) {
            ntruegamma++;
	    math::XYZTLorentzVector tempP4((*z)->momentum().px(),(*z)->momentum().py(),(*z)->momentum().pz(),(*z)->momentum().e());
	    MCGammaP4 += tempP4;
          }
	}    
      }
      break;
    }
  } 
  delete generated_event;

  math::XYZTLorentzVector chHadAndMCGammaP4 = chHadCandsP4 + MCGammaP4;
  invMassTrueGam->Fill(chHadAndMCGammaP4.M());
  nGamTrue->Fill(ntruegamma);
  MCTrkVsRecTrk->Fill(nchargedpion, sigChHadCands.size());
  MCTrkVsRecTrk->GetXaxis()->SetTitle("# of True Tracks ");
  MCTrkVsRecTrk->GetXaxis()->SetTitle("# of Reconstructed Tracks");
  if (nchargedpion == 1 && ntruegamma > 0) {
    invMassRecGam1Prong->Fill(mass);
    invMassTrueGam1Prong->Fill(chHadAndMCGammaP4.M());
  }
  if (nchargedpion == 3) {
    if (ntruegamma == 0) {
      invMassNoGam3Prong->Fill(mass);
    } else  {
      invMassRecGam3Prong->Fill(mass);
      invMassTrueGam3Prong->Fill(chHadAndMCGammaP4.M());
    }
  }
}
//
// -- Fill Invariant Mass from Gen Jet and Compare GenJet Contituents with PF Constituents
//
void InvariantMassTagTest::fillGenJetInvMass(const reco::PFTau& tau, const reco::GenJetCollection& genJets, 
                                             const TrackingParticleCollection& trakingParticles) {
  double dRGenJetTau = 999.9;
  math::XYZVector tau3vec(tau.px(),tau.py(),tau.pz());
  int index = 0;
  int min_index = -1;
  for ( GenJetCollection::const_iterator f  = genJets.begin();
                                         f != genJets.end();
                                         f++) 
  {
     double deltaR = ROOT::Math::VectorUtil::DeltaR(tau3vec, f->momentum());
     if (deltaR < dRGenJetTau) {
       dRGenJetTau = deltaR;
       min_index = index;
     }
     index++;
  }
  if (dRGenJetTau > 0.1) return;


  PFCandidateRefVector PFSigGammaCands  = tau.signalPFGammaCands();
  PFCandidateRefVector PFSigCands     = tau.signalPFCands();
  const TrackRef leadTk =  tau.leadTrack();
  math::XYZVector leadTk3Vec(leadTk->px(),leadTk->py(),leadTk->pz());

  const GenJet& j = genJets[min_index];
  invMassGenJet->Fill(j.p4().M());
  double eres_tau = (j.energy() - tau.energy())/j.energy();
  EResTau->Fill(eres_tau);
  EResTauVsDR->Fill(dRGenJetTau, eres_tau);

  std::vector <const GenParticle*> constituents = j.getGenConstituents();
  math::XYZTLorentzVector genJetSelectedP4(0.,0.,0.,0.);   
  math::XYZTLorentzVector genJetCandSigConeP4(0.,0.,0.,0.);   
  math::XYZTLorentzVector tauPFSigConeP4(0.,0.,0.,0.);   
  std::vector <const GenParticle*> genJetGammasSigCone;

  int nGenJetCandNotMatchedSigCone = 0;
  int nGenJetCandSigCone = 0;
  for (std::vector <const GenParticle*>::const_iterator ic = constituents.begin(); 
       ic != constituents.end(); ic++) {
    if ((*ic)->pt() > 1.0) genJetSelectedP4 += (*ic)->p4();
    //    double dR = ROOT::Math::VectorUtil::DeltaR(tau3vec, (*ic)->momentum());
    double dR = ROOT::Math::VectorUtil::DeltaR(leadTk3Vec, (*ic)->momentum());
    if (dR >= 0.1 || (*ic)->pt() < 1.0) continue;
    if (abs((*ic)->pdgId()) == 22) genJetGammasSigCone.push_back((*ic));

     genJetCandSigConeP4 += (*ic)->p4();
     nGenJetCandSigCone++;

    double pt     = (*ic)->pt();
    double eta    = (*ic)->eta();
    double energy = (*ic)->energy();   
            
    ptCandGenJetSigCone->Fill(pt);
    etaCandGenJetSigCone->Fill(eta);
    iDGenJetSigCone->Fill(abs((*ic)->pdgId()));
    
    double min_angle = 9999.9;
    double energy_pf = 0.0;
    for (PFCandidateRefVector::const_iterator iSigPF =PFSigCands.begin();iSigPF!=PFSigCands.end();iSigPF++){
      math::XYZVector PF3Vec((**iSigPF).px(), (**iSigPF).py(), (**iSigPF).pz());
      double dR2 = ROOT::Math::VectorUtil::DeltaR(PF3Vec, (*ic)->momentum());
      tauPFSigConeP4 +=(**iSigPF).p4();
      if (dR2 < min_angle) {
        min_angle = dR2;
        energy_pf = (**iSigPF).energy();
      }
    }
    if (min_angle > 0.05) {
      nGenJetCandNotMatchedSigCone++;
      ptCandGenJetSigConeNotMatched->Fill(pt);
      etaCandGenJetSigConeNotMatched->Fill(eta);
    } else EresCandGenJetSigConeMatched->Fill((energy-energy_pf)/energy);
  }

  invMassGenJetPt->Fill(genJetSelectedP4.M());
  nCandGenJetSigCone->Fill(nGenJetCandSigCone);
  invMassGenJetSigCone->Fill(genJetCandSigConeP4.M());
  nGamGenJetSigCone->Fill(genJetGammasSigCone.size());
  double eres_tausig = (genJetCandSigConeP4.energy()-tauPFSigConeP4.energy())/genJetCandSigConeP4.energy();

  EResTauSigCone->Fill(eres_tausig);
  EResTauSigConeVsDR->Fill(dRGenJetTau, eres_tausig);
  
  int nGenJetGamNotMatchedSigCone = 0;
  //Now Loop over Gammas in signal cone 
  for (std::vector <const GenParticle*>::const_iterator ig = genJetGammasSigCone.begin(); 
       ig != genJetGammasSigCone.end(); ig++) { 
    int nvtx = -1;
    double rvtx = -1.0;
    // Check the vertices for these gammas from tracking particle collection
    for (TrackingParticleCollection::const_iterator itrack = trakingParticles.begin(); 
	 itrack != trakingParticles.end(); itrack++) {
      if ( itrack->pdgId() != 22 ||  itrack->charge() != 0) continue;
      for (TrackingParticle::genp_iterator hepT = itrack->genParticle_begin(); 
	   hepT !=  itrack->genParticle_end(); ++hepT) {
	if ((*hepT)->momentum().x() == (*ig)->px() && 
	    (*hepT)->momentum().y() == (*ig)->py() &&
	    (*hepT)->momentum().z() == (*ig)->pz() ) {
	  nvtx = itrack->decayVertices().size();
          if (nvtx > 0) {
	    TrackingParticle::tv_iterator iTV = itrack->decayVertices_begin();
	    double xpos = (**iTV).position().X();
	    double ypos = (**iTV).position().Y();
	    rvtx = TMath::Sqrt(xpos * ypos + ypos*ypos);
          }
	}
      }
    }    
    double pt     = (*ig)->pt();
    double eta    = (*ig)->eta();
    double energy = (*ig)->energy();    

    ptGamGenJetSigCone->Fill(pt);
    etaGamGenJetSigCone->Fill(eta);
    double min_angle = 9999.9;
    double energy_pf = 0.0;
    for (PFCandidateRefVector::const_iterator iGam = PFSigGammaCands.begin();iGam!=PFSigGammaCands.end();iGam++){
      math::XYZVector PFGam3Vec((**iGam).px(), (**iGam).py(), (**iGam).pz());
      double dR2 = ROOT::Math::VectorUtil::DeltaR(PFGam3Vec, (*ig)->momentum());
      if (dR2 < min_angle) {
        min_angle = dR2;
        energy_pf = (**iGam).energy();
      }
    }
    if (min_angle > 0.05) {
      nGenJetGamNotMatchedSigCone++;
      ptGamGenJetSigConeNotMatched->Fill(pt);
      etaGamGenJetSigConeNotMatched->Fill(eta);
      nVtxGamGenJetSigConeNotMatched->Fill(nvtx);
      rVtxGamGenJetSigConeNotMatched->Fill(rvtx);
    } else {
      nVtxGamGenJetSigConeMatched->Fill(nvtx);
      rVtxGamGenJetSigConeMatched->Fill(rvtx);
      double res = (energy-energy_pf)/energy;
      EresGamGenJetSigConeMatched->Fill(res);
      if (energy < 5.0) EresGamGenJetSigConeMatched1->Fill(res);
      else if (energy >= 5.0 && energy < 10.0) EresGamGenJetSigConeMatched2->Fill(res);
      else if (energy >= 10.0 && energy < 20.0) EresGamGenJetSigConeMatched3->Fill(res);
      else if (energy >= 20.0) EresGamGenJetSigConeMatched4->Fill(res);
    }
  }
  nGamGenJetSigConeNotMatched->Fill(nGenJetGamNotMatchedSigCone);
  float frac;
  if (nGenJetCandSigCone > 0 && nGenJetCandNotMatchedSigCone > 0) {
    frac = nGenJetCandNotMatchedSigCone*1.0/nGenJetCandSigCone;
    fracCandGenJetSigConeNotMatched->Fill(frac);
  }
  if (genJetGammasSigCone.size() > 0 && nGenJetGamNotMatchedSigCone > 0) {
    frac = nGenJetGamNotMatchedSigCone*1.0/genJetGammasSigCone.size();
    fracGamGenJetSigConeNotMatched->Fill(frac);
  }
}
//
// -- End Job
//
void InvariantMassTagTest::endJob() {
  cout << " Finishing the job ...... " << endl;
}
//
// -- Find InvMass from Generator level
//
void InvariantMassTagTest::findTrueInvMass(const edm::Event& iEvent){
  edm::Handle<edm::HepMCProduct> genParticleHandle;
  iEvent.getByLabel(GenParticleProducer_, genParticleHandle);

  HepMC::GenEvent* generated_event = new HepMC::GenEvent(*(genParticleHandle->GetEvent()));
  math::XYZTLorentzVector myTauP4(0.,0.,0.,0.);
  for (HepMC::GenEvent::particle_iterator p  = generated_event->particles_begin();
                                          p != generated_event->particles_end(); 
                                          p++)
  {
    if (abs((*p)->pdg_id()) == 15 && (*p)->status() == 2) {
      math::XYZTLorentzVector tauP4((*p)->momentum().px(),(*p)->momentum().py(),(*p)->momentum().pz(),(*p)->momentum().e());
      bool lept_decay = false;
      int nchargedpion = 0;
      HepMC::GenVertex::particle_iterator z = (*p)->end_vertex()->particles_begin(HepMC::descendants);
      int ndesc =0 ;
      for (; z != (*p)->end_vertex()->particles_end(HepMC::descendants); z++) {
	ndesc++;
	if (abs((*z)->pdg_id()) == 11 || abs((*z)->pdg_id()) == 13) lept_decay = true;
        if (abs((*z)->pdg_id()) == 211) nchargedpion++;
	if (abs((*z)->pdg_id()) == 16){
	  math::XYZTLorentzVector tauNeuP4((*z)->momentum().px(),(*z)->momentum().py(),(*z)->momentum().pz(),(*z)->momentum().e());
	  myTauP4 = tauP4 - tauNeuP4;
	}    
      }
      if (lept_decay == false) {
        invMassHepMC->Fill(myTauP4.M());
        if (nchargedpion ==1) invMassHepMCTrk1->Fill(myTauP4.M());
      }  
    }
  } 
  delete generated_event;
}
DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(InvariantMassTagTest);
