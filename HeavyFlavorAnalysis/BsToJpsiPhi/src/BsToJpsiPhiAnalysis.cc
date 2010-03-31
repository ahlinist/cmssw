// -*- C++ -*-
//
// Package:    BsToJPsiPhiAnalysis
// Class:      BsToJPsiPhiAnalysis
// 
/* class BsToJPsiPhiAnalysis BsToJPsiPhiAnalysis.cc HeavyFlavorAnalysis/BsToJpsiPhi/src/BsToJPsiPhiAnalysis.cc

 Description: Reconstruction of the decay channel Bs-->J/Psi(mumu) Phi(KK)

 Implementation: V01-312-04

*/
//
// Original Author:  Giordano Cerizza
//         Authors:  Giordano Cerizza, Alexander Schmidt, Virginia Azzolini, Barbara Mejias
//         Created:  Mon June 15 09:16:18 EDT 2009
// 
//

#include "HeavyFlavorAnalysis/BsToJpsiPhi/interface/BsToJpsiPhiAnalysis.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/Common/interface/EDProduct.h"
// #include "FWCore/Framework/interface/Handle.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Framework/interface/TriggerNames.h"
#include "FWCore/Framework/interface/TriggerReport.h"
#include "FWCore/Framework/interface/TriggerNamesService.h"

#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"
#include "SimTracker/Records/interface/TrackAssociatorRecord.h"

#include "RecoVertex/KinematicFitPrimitives/interface/ParticleMass.h"
#include "RecoVertex/KinematicFitPrimitives/interface/MultiTrackKinematicConstraint.h"
#include <RecoVertex/KinematicFitPrimitives/interface/KinematicParticleFactoryFromTransientTrack.h>
// #include "RecoVertex/KinematicFitPrimitives/interface/"
#include "RecoVertex/KinematicFit/interface/KinematicConstrainedVertexFitter.h"
#include "RecoVertex/KinematicFit/interface/TwoTrackMassKinematicConstraint.h"
#include "RecoVertex/KinematicFit/interface/KinematicParticleVertexFitter.h"
#include "RecoVertex/KinematicFit/interface/KinematicParticleFitter.h"
#include "RecoVertex/KinematicFit/interface/MassKinematicConstraint.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/Candidate/interface/CompositeCandidateFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonTrackLinks.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/CaloMuon.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
// #include "DataFormats/HepMCCandidate/interface/GenParticleCandidate.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"
#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "DataFormats/Candidate/interface/OverlapChecker.h"
#include "PhysicsTools/CandUtils/interface/AddFourMomenta.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "TLorentzRotation.h"
#include "RecoVertex/KinematicFitPrimitives/interface/RefCountedKinematicVertex.h"
#include "RecoVertex/KinematicFitPrimitives/interface/RefCountedKinematicParticle.h"
#include "RecoVertex/KinematicFitPrimitives/interface/RefCountedKinematicTree.h"
#include "DataFormats/GeometryCommonDetAlgo/interface/Measurement1D.h"
#include "RecoVertex/VertexTools/interface/VertexDistance3D.h"
#include "RecoVertex/VertexTools/interface/VertexDistanceXY.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertexContainer.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/TrackReco/interface/DeDxData.h"
#include "DataFormats/TrackReco/interface/DeDxHit.h"
#include "RecoTracker/DeDx/interface/DeDxTools.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripMatchedRecHit2D.h"
#include "DataFormats/TrackerRecHit2D/interface/ProjectedSiStripRecHit2D.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHit.h"
#include "TrackingTools/PatternTools/interface/TrajTrackAssociation.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
//#include "CondFormats/DataRecord/interface/SiStripDeDxPion_3D_Rcd.h"

#include <iostream>
#include <TMath.h>

using namespace reco;
using namespace edm;
using namespace std;

BsToJpsiPhiAnalysis::BsToJpsiPhiAnalysis(const edm::ParameterSet& iConfig) : theConfig_(iConfig)
{

  thegenParticlesLabel_ = iConfig.getParameter<InputTag>("genParticlesLabel");
  trackLabelK_ = iConfig.getParameter<edm::InputTag>("TrackLabel_K");
  trackLabelPi_ = iConfig.getParameter<edm::InputTag>("TrackLabel_pi");
  triggerTag_ = iConfig.getParameter<edm::InputTag>("TriggerTag");
  muonTag_ = iConfig.getParameter<edm::InputTag>("MuonTag");
  StoreDeDxInfo_ = iConfig.getParameter<bool>("StoreDeDxInfo");
  JpsiMassWindowBeforeFit_ = iConfig.getParameter<double>("JpsiMassWindowBeforeFit");

  BsLowerMassCutBeforeFit_  = iConfig.getParameter<double>("BsLowerMassCutBeforeFit");
  BsUpperMassCutBeforeFit_  = iConfig.getParameter<double>("BsUpperMassCutBeforeFit");
  BsLowerMassCutAfterFit_  = iConfig.getParameter<double>("BsLowerMassCutAfterFit");
  BsUpperMassCutAfterFit_  = iConfig.getParameter<double>("BsUpperMassCutAfterFit");

  JpsiMassWindowAfterFit_ = iConfig.getParameter<double>("JpsiMassWindowAfterFit");
  JpsiPtCut_ =  iConfig.getParameter<double>("JpsiPtCut");
  KaonTrackPtCut_ = iConfig.getParameter<double>("KaonTrackPtCut");
  PhiMassWindowBeforeFit_ = iConfig.getParameter<double>("PhiMassWindowBeforeFit");
  PhiMassWindowAfterFit_ = iConfig.getParameter<double>("PhiMassWindowAfterFit");

  outputFile_ = iConfig.getUntrackedParameter<std::string>("outputFile");

  edm::LogInfo("RecoVertex/BsToJpsiPhiAnalysis")<< "Initializing Bs to Jpsi Phi analyser  - Output file: " << outputFile_ <<"\n";

}

BsToJpsiPhiAnalysis::~BsToJpsiPhiAnalysis() {}

void BsToJpsiPhiAnalysis::beginJob(edm::EventSetup const& setup)
{
  bsRootTree_ = new BsToJpsiPhiRootTree(outputFile_);

}

void BsToJpsiPhiAnalysis::endJob() 
{
  delete bsRootTree_;
  cout << "Total number of Events: " << event_counter_ << endl;
}

void
BsToJpsiPhiAnalysis::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  event_counter_++;

  bsRootTree_->resetEntries();
  
  /////////////////////////
  // dE/dx info
  /////////////////////////
  
  Handle<DeDxDataValueMap> energyLossHandle;
  
  
  if(StoreDeDxInfo_)  iEvent.getByLabel("dedxMedian", energyLossHandle);
  
  
  /////////////////////////////////
  // Get primary vertices  
  /////////////////////////////////
  
  int VtxIndex=-99;
  double minVtxProb=-999.;
  double MinBVtx=-999.;
  
  reco::BeamSpot vertexBeamSpot;
  edm::Handle<reco::BeamSpot> recoBeamSpotHandle;
  iEvent.getByType(recoBeamSpotHandle);
  vertexBeamSpot = *recoBeamSpotHandle;
  
  BSx = vertexBeamSpot.x0();
  BSy = vertexBeamSpot.y0();
  BSz = vertexBeamSpot.z0();
  
  Handle<reco::VertexCollection> recVtxs;
  iEvent.getByLabel("offlinePrimaryVertices", recVtxs);
  
  for(size_t i = 0; i < recVtxs->size(); ++ i) {
    const Vertex &vtx = (*recVtxs)[i];
    double RecVtxProb=TMath::Prob(vtx.chi2(),(int)vtx.ndof());
    if(RecVtxProb>minVtxProb){
      VtxIndex=i;
      minVtxProb=RecVtxProb;
    }
  }
  
  const Vertex &RecVtx = (*recVtxs)[VtxIndex];
  double PVx=0.,PVy=0., PVz=0;
  if(VtxIndex!=-99)
    {
      PVx = RecVtx.x(); 
      PVy= RecVtx.y();
      PVz= RecVtx.z();
      PVerrx=RecVtx.xError();
      PVerry=RecVtx.yError();
      PVerry=RecVtx.zError();
    }
  else {
    PVx=BSx; 
    PVy=BSy;
    PVz=BSz;
  } 
  
  bsRootTree_->getVtx(BSx,BSy,BSz,PVx,PVy,PVz,PVerrx,PVerry,PVerrz);
  
  /////////////////////////////////    
  // MC info 
  /////////////////////////////////
  edm::Handle<GenParticleCollection> genParticles;
  iEvent.getByLabel(thegenParticlesLabel_ , genParticles );
  
  // call function which does the job: filling MC info
  fillMCInfo(genParticles);
  
  //////////////////////////////////
  //
  // HLT code for trigger bits
  //
  // flag = 1 HLT_Mu3
  // flag = 2 HLT_Mu5
  // flag = 3 HLT_Mu9
  // flag = 4 HLT_DoubleIsoMu3
  // flag = 5 HLT_DoubleMu3
  // flag = 6 HLT_DoubleMu3_JPsi
  //
  ///////////////////////////////////
  
  edm::Handle<edm::TriggerResults>  hltresults;
  iEvent.getByLabel(triggerTag_, hltresults);
  
  edm::TriggerNames triggerNames_;
  triggerNames_.init(* hltresults);
  
  int ntrigs = hltresults->size();
  for (int itrig = 0; itrig != ntrigs; ++itrig){
    
    TString trigName=triggerNames_.triggerName(itrig);
    if (trigName=="HLT_Mu3") bsRootTree_->triggerbit_HLTmu3_ = hltresults->accept(itrig);
    if (trigName=="HLT_Mu5") bsRootTree_->triggerbit_HLTmu5_ = hltresults->accept(itrig);
    if (trigName=="HLT_Mu9") bsRootTree_->triggerbit_HLTmu9_ = hltresults->accept(itrig);
    if (trigName=="HLT_DoubleIsoMu3")  bsRootTree_->triggerbit_HLTdoubleIsoMu3_   = hltresults->accept(itrig);
    if (trigName=="HLT_DoubleMu3")     bsRootTree_->triggerbit_HLTdoubleMu3_      = hltresults->accept(itrig);
    if (trigName=="HLT_DoubleMu3_JPsi")bsRootTree_->triggerbit_HLTdoubleMu3_JPsi_ = hltresults->accept(itrig);
  }
  
  // **************************
  // start loop over muons
  
  edm::Handle<reco::MuonCollection> allmuons;
  iEvent.getByLabel(muonTag_,allmuons);
  
  
  // variables to determine minima of fit probability and mass diff.
  double minVtxP = -99.;
  double minVtxP2 = -99.;
  double minVtxP3 = -99.;
  double minJpsiM = 200.;
  
  for(size_t i=0; i < allmuons->size(); ++i){
    const Muon & trk1 = (*allmuons)[i];
    if(!trk1.isGlobalMuon() && !trk1.isTrackerMuon()) continue;
    for (size_t j=i+1; j < allmuons->size(); ++j){
      const Muon & trk2 = (*allmuons)[j];
      if(!trk2.isGlobalMuon() && !trk2.isTrackerMuon()) continue;
      
      if(trk1.charge()==trk2.charge()) continue;
      // passed opposite sign cut
      if(bsRootTree_->iPassedCutIdent_   < 1 ) bsRootTree_->iPassedCutIdent_ = 1 ;
      if(bsRootTree_->iPassedCutIdentBd_   < 1 ) bsRootTree_->iPassedCutIdentBd_ = 1 ;
      
      CompositeCandidate Jpsi;
      Jpsi.addDaughter(trk1);
      Jpsi.addDaughter(trk2);
      AddFourMomenta addP4;
      addP4.set(Jpsi);
      
      if ( abs(Jpsi.mass()-3.096916) > JpsiMassWindowBeforeFit_ || Jpsi.pt() < JpsiPtCut_) continue;
      // passed jpsi mass and pt cut
      if(bsRootTree_->iPassedCutIdent_   < 2 ) bsRootTree_->iPassedCutIdent_ = 2 ;
      if(bsRootTree_->iPassedCutIdentBd_   < 2 ) bsRootTree_->iPassedCutIdentBd_ = 2 ;
      
      edm::ESHandle<TransientTrackBuilder> theB;
      iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theB);	
      TrackRef trkMu1Ref = trk1.get<TrackRef>();
      TrackRef trkMu2Ref = trk2.get<TrackRef>();
      
      vector<TransientTrack> trk_all;
      trk_all.push_back((*theB).build(&trkMu1Ref));
      trk_all.push_back((*theB).build(&trkMu2Ref));
      
      KalmanVertexFitter kvf;
      TransientVertex tv = kvf.vertex(trk_all);
      
      if (!tv.isValid()) continue; 
      // valid jpsi vertex
      if(bsRootTree_->iPassedCutIdent_   < 3 ) bsRootTree_->iPassedCutIdent_ = 3 ;
      if(bsRootTree_->iPassedCutIdentBd_   < 3 ) bsRootTree_->iPassedCutIdentBd_ = 3 ;
      
      Vertex vertex = tv;
      GlobalPoint secondaryVertex = tv.position();
      
      double vtxProb_Jpsi = TMath::Prob(vertex.chi2(),(int)vertex.ndof()); 
      
      // fill the Jpsi closest to the true Jpsi mass in the tree
      if(abs(Jpsi.mass()-3.096916)<minJpsiM){
	minJpsiM = abs(Jpsi.mass()-3.096916);
	bsRootTree_->JpsiVtxProb_ = vtxProb_Jpsi;
	MuCounter1 = i;
	MuCounter2 = j;	  
	bsRootTree_->JpsiM_alone_ = Jpsi.mass();	  
	bsRootTree_->JpsiPhi_alone_ = Jpsi.phi();	  
	bsRootTree_->JpsiEta_alone_ = Jpsi.eta();	  
	bsRootTree_->JpsiPt_alone_ = Jpsi.pt();	  
	bsRootTree_->JpsiMu1Pt_ = trk1.pt();	  
	bsRootTree_->JpsiMu2Pt_ = trk2.pt();	  
	bsRootTree_->JpsiMu1Phi_ = trk1.phi();	  
	bsRootTree_->JpsiMu2Phi_ = trk2.phi();	  
	bsRootTree_->JpsiMu1Eta_ = trk1.eta();	  
	bsRootTree_->JpsiMu2Eta_ = trk2.eta();	  
	
	// muon categories:
	// 1: tracker muons
	// 2: global muons
	// 3: global + tracker muon
	// 4: neither tracker nor global
	
	if (trk1.isTrackerMuon() && !trk1.isGlobalMuon())       bsRootTree_->JpsiMuon1Cat_ = 1;
	else if (!trk1.isTrackerMuon() && trk1.isGlobalMuon())  bsRootTree_->JpsiMuon1Cat_ = 2;
	else if (trk1.isTrackerMuon() && trk1.isGlobalMuon())   bsRootTree_->JpsiMuon1Cat_ = 3;
	else if (!trk1.isTrackerMuon() && !trk1.isGlobalMuon()) bsRootTree_->JpsiMuon1Cat_ = 4;
	
	if (trk2.isTrackerMuon() && !trk2.isGlobalMuon())       bsRootTree_->JpsiMuon2Cat_ = 1;
	else if (!trk2.isTrackerMuon() && trk2.isGlobalMuon())  bsRootTree_->JpsiMuon2Cat_ = 2;
	else if (trk2.isTrackerMuon() && trk2.isGlobalMuon())   bsRootTree_->JpsiMuon2Cat_ = 3;
	else if (!trk2.isTrackerMuon() && !trk2.isGlobalMuon()) bsRootTree_->JpsiMuon2Cat_ = 4;
      }

      ////////////////////////////////////	
      ////////////////////////////////////
      ////           Phi
      ////////////////////////////////////
      ////////////////////////////////////
      
      if (MuCounter1!=-10 && MuCounter2!=-10){
	const Candidate & mu1jpsi = (*allmuons)[MuCounter1];
	const Candidate & mu2jpsi = (*allmuons)[MuCounter2];
	
	Handle<CandidateView> allTracks;
	iEvent.getByLabel(trackLabelK_, allTracks);
	
	for (size_t k=0; k< allTracks->size(); ++k){
	  for (size_t l=k+1; l< allTracks->size(); ++l){
	    
	    const Candidate & track1 = (*allTracks)[k];
	    const Candidate & track2 = (*allTracks)[l];
	    
	    if (track1.charge()==track2.charge()) continue;
	    if (track1.pt() < KaonTrackPtCut_) continue;
	    if (track2.pt() < KaonTrackPtCut_) continue;
	    
	    // passed kaon opposite sign and pt cut
	    if(bsRootTree_->iPassedCutIdent_   < 4 ) bsRootTree_->iPassedCutIdent_ = 4 ;
	    
	    bsRootTree_->K1Pt_before_   = track1.pt();
	    bsRootTree_->K1Pz_before_   = track1.pz();
	    bsRootTree_->K1Eta_before_  = track1.eta();
	    bsRootTree_->K1Phi_before_  = track1.phi();
	    bsRootTree_->K2Pt_before_   = track2.pt();
	    bsRootTree_->K2Pz_before_   = track2.pz();
	    bsRootTree_->K2Eta_before_  = track2.eta();
	    bsRootTree_->K2Phi_before_  = track2.phi();
	    
	    // phi candidate
	    
	    CompositeCandidate PhiCand;
	    PhiCand.addDaughter(track1);
	    PhiCand.addDaughter(track2);
	    AddFourMomenta ad;
	    ad.set(PhiCand);
	    
	    if (abs(PhiCand.mass()-1.019) > PhiMassWindowBeforeFit_) continue;
	    // passed phi mass window before fit
	    if(bsRootTree_->iPassedCutIdent_   < 5 ) bsRootTree_->iPassedCutIdent_ = 5 ;
	    
	    bsRootTree_->PhiMass_before_ = PhiCand.mass();	  
	    bsRootTree_->PhiPhi_before_ = PhiCand.phi();
	    bsRootTree_->PhiEta_before_ = PhiCand.eta();
	    bsRootTree_->PhiPt_before_ = PhiCand.pt();
	    bsRootTree_->PhiPz_before_ = PhiCand.pz();
	    
	    // jpsi candidate
	    
	    CompositeCandidate JpsiCand;
	    JpsiCand.addDaughter(mu1jpsi);
	    JpsiCand.addDaughter(mu2jpsi);
	    AddFourMomenta sum;
	    sum.set(JpsiCand);
	    
	    if (abs(JpsiCand.mass()-3.097) > JpsiMassWindowBeforeFit_) continue;	      
	    // passed jpsi mass window before fit
	    if(bsRootTree_->iPassedCutIdent_   < 6 ) bsRootTree_->iPassedCutIdent_ = 6 ;
	    
	    bsRootTree_->JpsiMass_before_ = JpsiCand.mass();	  
	    bsRootTree_->JpsiPhi_before_ = JpsiCand.phi();
	    bsRootTree_->JpsiEta_before_ = JpsiCand.eta();
	    bsRootTree_->JpsiPt_before_ = JpsiCand.pt();
	    bsRootTree_->JpsiPz_before_ = JpsiCand.pz();
	    
	    // check on the overlap
	    
	    OverlapChecker overlap;
	    if (overlap(mu1jpsi,track1)!=0 || overlap(mu2jpsi,track1)!=0) continue;
	    if (overlap(mu1jpsi,track2)!=0 || overlap(mu2jpsi,track2)!=0) continue;
	    
	    // passed muon - track overlap check
	    if(bsRootTree_->iPassedCutIdent_   < 7 ) bsRootTree_->iPassedCutIdent_ = 7 ;
	    
	    
	    // B candidate
	    
	    CompositeCandidate BCand;
	    BCand.addDaughter(mu1jpsi);
	    BCand.addDaughter(mu2jpsi);
	    BCand.addDaughter(track1);
	    BCand.addDaughter(track2);
	    AddFourMomenta add4mom;
	    add4mom.set(BCand);
	    
	    if (BCand.mass() < BsLowerMassCutBeforeFit_ || BCand.mass() > BsUpperMassCutBeforeFit_) continue;
	    // passed Bs mass cut before fit
	    if(bsRootTree_->iPassedCutIdent_   < 8 ) bsRootTree_->iPassedCutIdent_ = 8 ;
	    
	    bsRootTree_->BsMass_before_ = BCand.mass();	   	      
	    bsRootTree_->BsPt_before_ = BCand.pt();
	    bsRootTree_->BsPz_before_ = BCand.pz();
	    bsRootTree_->BsPhi_before_ = BCand.phi();
	    bsRootTree_->BsEta_before_ = BCand.eta();
	    
	    // start fit on the B candidates
	    
	    edm::ESHandle<TransientTrackBuilder> theB;
	    iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theB);	
	    TrackRef Mu1Ref = mu1jpsi.get<TrackRef>();
	    TrackRef Mu2Ref = mu2jpsi.get<TrackRef>();
	    TrackRef trk1Ref = track1.get<TrackRef>();
	    TrackRef trk2Ref = track2.get<TrackRef>();
	    
	    bsRootTree_->K1Pt_error_   = trk1Ref->ptError();
	    bsRootTree_->K2Pt_error_   = trk2Ref->ptError();
	    
	    vector<TransientTrack> t_tracks;
	    t_tracks.push_back((*theB).build(&Mu1Ref));
	    t_tracks.push_back((*theB).build(&Mu2Ref));
	    t_tracks.push_back((*theB).build(&trk1Ref));
	    t_tracks.push_back((*theB).build(&trk2Ref));
	    
	    if (!Mu1Ref.isNonnull() || !Mu2Ref.isNonnull() || !trk1Ref.isNonnull() || !trk2Ref.isNonnull()) continue;
	    // checked track references
	    if(bsRootTree_->iPassedCutIdent_   < 9 ) bsRootTree_->iPassedCutIdent_ = 9 ;
	    
	    TransientTrack track_MuP = t_tracks[0];
	    TransientTrack track_MuM = t_tracks[1];
	    TransientTrack track_Kp = t_tracks[2];
	    TransientTrack track_Km = t_tracks[3];
	    
	    //Creating a KinematicParticleFactory
	    KinematicParticleFactoryFromTransientTrack pFactory;
	    
	    //The mass of a muon and the insignificant mass sigma to avoid singularities in the covariance matrix.
	    ParticleMass muon_mass = 0.1056583;
	    ParticleMass kaon_mass = 0.493677;
	    ParticleMass pi_mass = 0.139570;
	    
	    float muon_sigma = 0.0000000001;
	    float kaon_sigma = 0.000016;
	    float pi_sigma = 0.000016;	      
	    
	    //initial chi2 and ndf before kinematic fits. The chi2 of the reconstruction is not considered
	    float chi = 0.;
	    float ndf = 0.;	    
	    
	    vector<RefCountedKinematicParticle> allParticlesMu;
	    allParticlesMu.push_back(pFactory.particle (track_MuP,muon_mass,chi,ndf,muon_sigma));
	    allParticlesMu.push_back(pFactory.particle (track_MuM,muon_mass,chi,ndf,muon_sigma));
	    
	    // probability for the KK hypothesis for the charged trks
	    vector<RefCountedKinematicParticle> allParticlesTrk;
	    allParticlesTrk.push_back(pFactory.particle (track_Kp,kaon_mass,chi,ndf,kaon_sigma));
	    allParticlesTrk.push_back(pFactory.particle (track_Km,kaon_mass,chi,ndf,kaon_sigma));
	    
	    KinematicParticleVertexFitter Fitter;
	    RefCountedKinematicTree JpsiTree = Fitter.fit(allParticlesMu);

	    // if the fit fails, do not consider this as candidate
	    if(JpsiTree->isEmpty()) continue;

	    KinematicParticleFitter constFitter;
	    
	    ParticleMass jpsiM = 3.09687;
	    float jpsiMsigma = 0.00004;
	    KinematicConstraint * jpsi_const = new MassKinematicConstraint(jpsiM,jpsiMsigma);
	    
	    JpsiTree = constFitter.fit(jpsi_const,JpsiTree);
	    
	    // if the fit fails, do not consider this as candidate
	    if(JpsiTree->isEmpty()) continue;

	    JpsiTree->movePointerToTheTop();
	    RefCountedKinematicParticle Jpsi_branch = JpsiTree->currentParticle();
	    allParticlesTrk.push_back(Jpsi_branch);
	    
	    RefCountedKinematicTree myTree_Bs = Fitter.fit(allParticlesTrk);
	    
	    // if the fit fails, do not consider this as candidate
	    if(myTree_Bs->isEmpty())  continue;
 
	    
	    // probability for the Kpi hypothesis for the charged trks
	    vector<RefCountedKinematicParticle> allParticlesTrk2;
	    allParticlesTrk2.push_back(pFactory.particle (track_Kp,kaon_mass,chi,ndf,kaon_sigma));
	    allParticlesTrk2.push_back(pFactory.particle (track_Km,pi_mass,chi,ndf,pi_sigma));
	    
	    JpsiTree->movePointerToTheTop();
	    RefCountedKinematicParticle Jpsi_branch2 = JpsiTree->currentParticle();
	    allParticlesTrk2.push_back(Jpsi_branch2);
	    
	    RefCountedKinematicTree myTree_Bs2 = Fitter.fit(allParticlesTrk2);
	    
	    //if the fit for the alternative hypothesis fails, this is not a reason to discard the event
	    // => assign fit probability -10 in this case
	    
	    if( !myTree_Bs2->isEmpty()) {
	      myTree_Bs2->movePointerToTheTop();
	      RefCountedKinematicParticle bs2 = myTree_Bs2->currentParticle();
	      RefCountedKinematicVertex bVertex2 = myTree_Bs2->currentDecayVertex();
	      
	      if (bVertex2->vertexIsValid()) {
		AlgebraicVector7 b_par2 = bs2->currentState().kinematicParameters().vector();
		AlgebraicSymMatrix77 bs_er2 = bs2->currentState().kinematicParametersError().matrix();
		double vtxprob_Bs2 = TMath::Prob(bs2->chiSquared(), (int)bs2->degreesOfFreedom());
	      
		if (vtxprob_Bs2 > minVtxP2){
		  
		  if (abs(JpsiCand.mass()-3.097) > JpsiMassWindowAfterFit_ || JpsiCand.pt() < JpsiPtCut_) continue;
		  if (abs(PhiCand.mass()-1.019) > PhiMassWindowAfterFit_) continue;
		  if (BCand.mass() < BsLowerMassCutAfterFit_ || BCand.mass() > BsUpperMassCutAfterFit_) continue;
		  
		  bsRootTree_->BsVtxProbKpi_ = vtxprob_Bs2;
		  minVtxP2 = vtxprob_Bs2;
		  
		  bsRootTree_->BfitM_Kpi_ = b_par2[6];
		  
		  bsRootTree_->setFitParKpi(myTree_Bs2);
		}
	      }
	    }
	    else{
	      	  bsRootTree_->BsVtxProbKpi_ = -10;
		  bsRootTree_->BfitM_Kpi_ = -10;
	    }
	    
	    // probability for the pipi hypothesis for the charged trks
	    vector<RefCountedKinematicParticle> allParticlesTrk3;
	    allParticlesTrk3.push_back(pFactory.particle (track_Kp,pi_mass,chi,ndf,pi_sigma));
	    allParticlesTrk3.push_back(pFactory.particle (track_Km,pi_mass,chi,ndf,pi_sigma));
	    
	    JpsiTree->movePointerToTheTop();
	    RefCountedKinematicParticle Jpsi_branch3 = JpsiTree->currentParticle();
	    allParticlesTrk3.push_back(Jpsi_branch3);
	    
	    RefCountedKinematicTree myTree_Bs3 = Fitter.fit(allParticlesTrk2);

	    if( !myTree_Bs3->isEmpty() ){	    
	      myTree_Bs3->movePointerToTheTop();
	      RefCountedKinematicParticle bs3 = myTree_Bs3->currentParticle();
	      RefCountedKinematicVertex bVertex3 = myTree_Bs3->currentDecayVertex();
	      
	      if (bVertex3->vertexIsValid()) {
		AlgebraicVector7 b_par3 = bs3->currentState().kinematicParameters().vector();
		AlgebraicSymMatrix77 bs_er3 = bs3->currentState().kinematicParametersError().matrix();
		double vtxprob_Bs3 = TMath::Prob(bs3->chiSquared(), (int)bs3->degreesOfFreedom());
		
		if (vtxprob_Bs3 > minVtxP3){
		  
		  if (abs(JpsiCand.mass()-3.097) > JpsiMassWindowAfterFit_ || JpsiCand.pt() < JpsiPtCut_) continue;
		  if (abs(PhiCand.mass()-1.019) > PhiMassWindowAfterFit_) continue;
		  if (BCand.mass() < BsLowerMassCutAfterFit_ || BCand.mass() > BsUpperMassCutAfterFit_) continue;
		  
		  bsRootTree_->BsVtxProbpipi_ = vtxprob_Bs3;
		  minVtxP3 = vtxprob_Bs3;
		  
		  bsRootTree_->BfitM_pipi_ = b_par3[6];
		  
		  bsRootTree_->setFitParpipi(myTree_Bs3);
		  
		}
	      }
	    }
	    else{
	      bsRootTree_->BsVtxProbpipi_ = -10;
	      bsRootTree_->BfitM_pipi_ = -10;
	    }
	    

	    // now go back to the original KK hypothesis
	    myTree_Bs->movePointerToTheTop();
	    RefCountedKinematicParticle bs = myTree_Bs->currentParticle();
	    RefCountedKinematicVertex bVertex = myTree_Bs->currentDecayVertex();
	    
	    if (!bVertex->vertexIsValid()) continue;
	    // vertex is valid
	    if(bsRootTree_->iPassedCutIdent_   < 10 ) bsRootTree_->iPassedCutIdent_ = 10 ;
	    
	    AlgebraicVector7 b_par = bs->currentState().kinematicParameters().vector();
	    AlgebraicSymMatrix77 bs_er = bs->currentState().kinematicParametersError().matrix();
	    double vtxprob_Bs = TMath::Prob(bs->chiSquared(), (int)bs->degreesOfFreedom());
	    
	    
	    if (vtxprob_Bs > minVtxP){
	      TrkCounter1=k;
	      TrkCounter2=l;
	      
	      if (abs(JpsiCand.mass()-3.097) > JpsiMassWindowAfterFit_ || JpsiCand.pt() < JpsiPtCut_) continue;
	      // passed jpsi mass window after fit
	      if(bsRootTree_->iPassedCutIdent_   < 11 ) bsRootTree_->iPassedCutIdent_ = 11 ;
	      
	      if (abs(PhiCand.mass()-1.019) > PhiMassWindowAfterFit_) continue;
	      // passed phi mass window after fit
	      if(bsRootTree_->iPassedCutIdent_   < 12 ) bsRootTree_->iPassedCutIdent_ = 12 ;
	      
	      if (BCand.mass() < BsLowerMassCutAfterFit_ || BCand.mass() > BsUpperMassCutAfterFit_) continue;
	      // passed Bs mass window after fit
	      if(bsRootTree_->iPassedCutIdent_   < 13 ) bsRootTree_->iPassedCutIdent_ = 13 ;
	      
	      
	      
	      ////////////////////////
	      // fill kinematic info to tree
	      //////////////////////////
	      
	      bsRootTree_->chi2_Bs_  = bs->chiSquared();
	      bsRootTree_->ndof_Bs_   =(int)bs->degreesOfFreedom();
	      bsRootTree_->BsVtxProb_ = vtxprob_Bs;
	      minVtxP = vtxprob_Bs;
	      bsRootTree_->BfitM_KK_ = b_par[6];		
	      
	      bsRootTree_->setFitParKK(myTree_Bs);

	      bsRootTree_->BsVtx_x_ = bVertex->position().x();
	      bsRootTree_->BsVtx_y_ = bVertex->position().y();
	      bsRootTree_->BsVtx_z_ = bVertex->position().z();

	      bsRootTree_->BsMass_after_ = BCand.mass();
	      bsRootTree_->BsPt_after_ = BCand.pt();
	      bsRootTree_->BsPz_after_ = BCand.pz();
	      bsRootTree_->BsPhi_after_ = BCand.phi();
	      bsRootTree_->BsEta_after_ = BCand.eta();
	      
	      bsRootTree_->JpsiMass_after_ = JpsiCand.mass();
	      bsRootTree_->JpsiPhi_after_ = JpsiCand.phi();	  
	      bsRootTree_->JpsiEta_after_ = JpsiCand.eta();	  
	      bsRootTree_->JpsiPt_after_ = JpsiCand.pt();	  
	      bsRootTree_->JpsiPz_after_ = JpsiCand.pz();	  
	      
	      bsRootTree_->PhiMass_after_ = PhiCand.mass();
	      bsRootTree_->PhiPhi_after_ = PhiCand.phi();	  
	      bsRootTree_->PhiEta_after_ = PhiCand.eta();	  
	      bsRootTree_->PhiPt_after_ = PhiCand.pt();	  
	      bsRootTree_->PhiPz_after_ = PhiCand.pz();	  
	      
	      bsRootTree_->K1Pt_after_   = track1.pt();
	      bsRootTree_->K1Pz_after_   = track1.pz();
	      bsRootTree_->K1Eta_after_  = track1.eta();
	      bsRootTree_->K1Phi_after_  = track1.phi();
	      bsRootTree_->K2Pt_after_   = track2.pt();
	      bsRootTree_->K2Pz_after_   = track2.pz();
	      bsRootTree_->K2Eta_after_  = track2.eta();
	      bsRootTree_->K2Phi_after_  = track2.phi();
	      
	      bsRootTree_->K1Chi2_ = trk1Ref.get()->normalizedChi2();
	      bsRootTree_->K1nHits_= trk1Ref.get()->numberOfValidHits();
	      bsRootTree_->K2Chi2_ = trk2Ref.get()->normalizedChi2();
	      bsRootTree_->K2nHits_= trk2Ref.get()->numberOfValidHits();
	      bsRootTree_->Mu1Chi2_ = Mu1Ref.get()->normalizedChi2();
	      bsRootTree_->Mu1nHits_= Mu1Ref.get()->numberOfValidHits();
	      bsRootTree_->Mu2Chi2_ = Mu2Ref.get()->normalizedChi2();
	      bsRootTree_->Mu2nHits_ =Mu2Ref.get()->numberOfValidHits();
	      
	      // dedx info
	      if(StoreDeDxInfo_){
		const DeDxDataValueMap &  eloss  = *energyLossHandle;
		double dedxTrk = eloss[trk1Ref].dEdx();
		double errdedxTrk = eloss[trk1Ref].dEdxError();
		int NumdedxTrk = eloss[trk1Ref].numberOfMeasurements();
		
		bsRootTree_->getDeDx(dedxTrk,errdedxTrk,NumdedxTrk);
	      }
	      
	      ////////////////////////////////////////////////
	      // proper decay time and proper decay length
	      ////////////////////////////////////////////////
	      
	      if(BCand.pt()!=0) {
		BLxy=((bVertex->position().x()-BSx)*BCand.px()+(bVertex->position().y()-BSy)*BCand.py())/BCand.pt();
		BLxy2=((bVertex->position().x()-PVx)*BCand.px()+(bVertex->position().y()-PVy)*BCand.py())/BCand.pt();
	      }
	      
	      Bsct1 = BLxy*BCand.mass()/BCand.pt();
	      Bsct2 = BLxy2*BCand.mass()/BCand.pt();
	      
	      BerrX=bs_er(1,1);
	      BerrY=bs_er(2,2);
	      BerrXY=bs_er(1,2); 
	      
	      bsRootTree_->getLXY(BLxy,BLxy2,BerrX,BerrY,BerrXY,Bsct1,Bsct2);
	      
	      double Dist3d = -5;
	      double dDist3d = -5;
	      double Time3d = -5;
	      double dTime3d = -5;
	      VertexDistance3D vdist3d;
	      Dist3d = vdist3d.distance(bVertex->vertexState(),RecVtx).value();
	      dDist3d = vdist3d.distance(bVertex->vertexState(),RecVtx).error();
	      Time3d = Dist3d * BCand.mass()/BCand.pt() *100. /3.;
	      dTime3d = dDist3d * BCand.mass()/BCand.pt() * 100. /3.;
	      
	      bsRootTree_->get3d(Dist3d,dDist3d,Time3d,dTime3d);
	      
	      double Dist = -5;
	      double dDist = -5;
	      double Time = -5;
	      double dTime = -5;
	      VertexDistanceXY vdist;
	      Dist = vdist.distance(bVertex->vertexState(),RecVtx).value();
	      dDist = vdist.distance(bVertex->vertexState(),RecVtx).error();
	      Time = Dist * BCand.mass()/BCand.pt() *100. /3.;
	      dTime = dDist * BCand.mass()/BCand.pt() * 100. /3.;
	      
	      bsRootTree_->get1d(Dist,dDist,Time,dTime);
	      
	      ////////////////////////////////////
	      // transversity basis angles
	      ////////////////////////////////////
	      
	      TLorentzVector pbs;
	      pbs.SetXYZM(BCand.px(),BCand.py(),BCand.pz(),BCand.mass());
	      
	      TLorentzVector pmuplus;
	      double mu1mass = sqrt(mu1jpsi.energy()*mu1jpsi.energy()-
				    mu1jpsi.px()*mu1jpsi.px()-
				    mu1jpsi.py()*mu1jpsi.py()-
				    mu1jpsi.pz()*mu1jpsi.pz());
	      
	      pmuplus.SetXYZM(mu1jpsi.px(),mu1jpsi.py(),mu1jpsi.pz(),mu1mass);
	      
	      TLorentzVector pmuminus;
	      double mu2mass = sqrt(mu2jpsi.energy()*mu2jpsi.energy()-
				    mu2jpsi.px()*mu2jpsi.px()-
				    mu2jpsi.py()*mu2jpsi.py()-
				    mu2jpsi.pz()*mu2jpsi.pz());
	      pmuminus.SetXYZM(mu2jpsi.px(),mu2jpsi.py(),mu2jpsi.pz(),mu2mass);
	      
	      TLorentzVector ptotmm = pmuplus + pmuminus;
	      
	      TLorentzVector pkplus;
	      double k1mass = sqrt(track1.energy()*track1.energy()-
				   track1.px()*track1.px()-
				   track1.py()*track1.py()-
				   track1.pz()*track1.pz());
	      pkplus.SetXYZM(track1.px(),track1.py(),track1.pz(),k1mass);
	      
	      TLorentzVector pkminus;
	      double k2mass = sqrt(track2.energy()*track2.energy()-
				   track2.px()*track2.px()-
				   track2.py()*track2.py()-
				   track2.pz()*track2.pz());
	      pkminus.SetXYZM(track2.px(),track2.py(),track2.pz(),k2mass);
	      
	      TLorentzVector ptotkk = pkplus + pkminus;
	      
	      TLorentzVector ptotjp = pmuplus + pmuminus + pkplus + pkminus;
	      
	      // boosting in JPsi restframe
	      TLorentzVector pjpsi;                                                                                                           
	      pjpsi = pmuplus + pmuminus;
	      
	      TLorentzVector pphi;
	      pphi = pkplus + pkminus;
	      
	      // the betas for the boost
	      TVector3 p3_JPsi;
	      p3_JPsi = pjpsi.Vect();
	      p3_JPsi *= -1./pjpsi.E();
	      
	      // the boost matrix
	      TLorentzRotation boost_jpsi(p3_JPsi);
	      TLorentzVector p_JPsi_JPsi;
	      p_JPsi_JPsi = boost_jpsi.VectorMultiplication(pjpsi);
	      
	      // the different momenta in the new frame                                                                                                       
	      TLorentzVector p_JPsi_muplus;
	      TLorentzVector p_JPsi_muminus; 
	      TLorentzVector p_JPsi_Kplus;
	      TLorentzVector p_JPsi_Kminus;
	      TLorentzVector p_JPsi_phi;                                                                       
	      TLorentzVector p_JPsi_Bs;   
	      
	      p_JPsi_muplus = boost_jpsi.VectorMultiplication(pmuplus);                                                                      
	      p_JPsi_muminus = boost_jpsi.VectorMultiplication(pmuminus);                                                                           
	      p_JPsi_Kplus = boost_jpsi.VectorMultiplication(pkplus);                                                                                              
	      p_JPsi_Kminus = boost_jpsi.VectorMultiplication(pkminus);                                                                                     
	      p_JPsi_phi = boost_jpsi.VectorMultiplication(pphi);
	      p_JPsi_Bs = boost_jpsi.VectorMultiplication(pbs);
	      
	      // the 3-momenta
	      
	      TVector3 p3_JPsi_muplus;
	      p3_JPsi_muplus = p_JPsi_muplus.Vect();
	      TVector3 p3_JPsi_muminus;
	      p3_JPsi_muminus = p_JPsi_muminus.Vect();
	      
	      TVector3 p3_JPsi_Kplus;
	      p3_JPsi_Kplus = p_JPsi_Kplus.Vect();
	      TVector3 p3_JPsi_Kminus;
	      p3_JPsi_Kminus = p_JPsi_Kminus.Vect();
	      
	      TVector3 p3_JPsi_phi;
	      p3_JPsi_phi = p_JPsi_phi.Vect();
	      TVector3 p3_JPsi_Bs;
	      p3_JPsi_Bs = p_JPsi_Bs.Vect();
	      
	      // coordinate system
	      
	      TVector3 x,y,z;
	      x = p3_JPsi_phi.Unit();
	      y = p3_JPsi_Kplus.Unit() - (x * (x * p3_JPsi_Kplus.Unit()));
	      y = y.Unit();
	      z = x.Cross(y);
	      
	      // Transversity Basis
	      
	      angle_costheta = p3_JPsi_muplus.Unit() * z;
	      
	      double cos_phi = p3_JPsi_muplus.Unit() * x / TMath::Sqrt(1 - angle_costheta*angle_costheta);
	      double sin_phi = p3_JPsi_muplus.Unit() * y / TMath::Sqrt(1 - angle_costheta*angle_costheta);
	      angle_phi = TMath::ACos(cos_phi);
	      if (sin_phi < 0){
		angle_phi =  -angle_phi;
	      }
	      
	      // boosting in phi restframe                                                                                                          
	      // the betas for the boost
	      
	      TVector3 p3_phi;
	      p3_phi = pphi.Vect();
	      p3_phi *= -1./pphi.E();
	      
	      // the boost matrix
	      
	      TLorentzRotation boost_phi(p3_phi);
	      TLorentzVector p_phi_phi;
	      p_phi_phi = boost_phi.VectorMultiplication(pphi);
	      
	      // the different momenta in the new frame
	      
	      TLorentzVector p_phi_Kplus;
	      TLorentzVector p_phi_JPsi;
	      TLorentzVector p_phi_Bs;
	      
	      p_phi_Kplus = boost_phi.VectorMultiplication(pkplus);
	      p_phi_JPsi = boost_phi.VectorMultiplication(pjpsi);
	      p_phi_Bs = boost_phi.VectorMultiplication(pbs);
	      
	      // the 3-momenta
	      
	      TVector3 p3_phi_Kplus;
	      p3_phi_Kplus = p_phi_Kplus.Vect();
	      TVector3 p3_phi_JPsi;
	      p3_phi_JPsi = p_phi_JPsi.Vect();
	      TVector3 p3_phi_Bs;
	      p3_phi_Bs = p_phi_Bs.Vect();
	      
	      angle_cospsi = -1 * p3_phi_Kplus.Unit() * p3_phi_JPsi.Unit();
	      
	      // set cos of angle between bs momentum and decay length
	      AngleBsDecayLength = ((bVertex->position().x()-PVx) * BCand.px() + (bVertex->position().y()-PVy) * BCand.py() + 
				    (bVertex->position().z()-PVz) * BCand.pz()) / sqrt(((bVertex->position().x()-PVx) * (bVertex->position().x()-PVx) + 
											(bVertex->position().y()-PVy) * (bVertex->position().y()-PVy) + 
											(bVertex->position().z()-PVz) * (bVertex->position().z()-PVz)) * 
										       (BCand.px()*BCand.px() + BCand.py()*BCand.py() + 
											BCand.pz()*BCand.pz()));
	      
	      bsRootTree_->getAngles(angle_costheta,angle_phi,angle_cospsi,AngleBsDecayLength);
	      
	      //////////////////////////////////////////////
	      // number of pixel/tracker/muons hits kaons
	      //////////////////////////////////////////////
	      
	      int pixhits1 = 0;
	      // hit pattern of the track
	      const reco::HitPattern& p = trk1Ref.get()->hitPattern();
	      // loop over the hits of the track
	      for (int i=0; i<p.numberOfHits(); i++) {
		uint32_t hit = p.getHitPattern(i);
		// if the hit is valid and in pixel barrel & endcap, print out the layer
		if (p.validHitFilter(hit) && p.pixelBarrelHitFilter(hit)) pixhits1++;
		if (p.validHitFilter(hit) && p.pixelEndcapHitFilter(hit)) pixhits1++;
	      }
	      bsRootTree_->K1pixH_ = pixhits1;
	      // count the number of valid tracker *** hits ***
	      bsRootTree_->K1trkH_= p.numberOfValidTrackerHits();
	      // count the number of tracker *** layers *** with measurement
	      bsRootTree_->K1trkLay_ =p.trackerLayersWithMeasurement();
	      bsRootTree_->K1muDTh_  =p.numberOfValidMuonDTHits();      // not-null, valid, muon DT
	      bsRootTree_->K1muCSCh_ =p.numberOfValidMuonCSCHits();    // not-null, valid, muon CSC
	      bsRootTree_->K1muRPCh_ =p.numberOfValidMuonRPCHits();     // not-null, valid, muon RPC
	      
	      int pixhits2=0;
	      const reco::HitPattern& p2 = trk2Ref.get()->hitPattern();
	      for (int i=0; i<p2.numberOfHits(); i++) {
		uint32_t hit = p2.getHitPattern(i);
		if (p2.validHitFilter(hit) && p2.pixelBarrelHitFilter(hit)) pixhits2++;
		if (p2.validHitFilter(hit) && p2.pixelEndcapHitFilter(hit)) pixhits2++;
	      }
	      bsRootTree_->K2pixH_   = pixhits2;
	      bsRootTree_->K2trkH_   = p2.numberOfValidTrackerHits();
	      bsRootTree_->K2trkLay_ = p2.trackerLayersWithMeasurement();
	      bsRootTree_->K2muDTh_  = p2.numberOfValidMuonDTHits();      // not-null, valid, muon DT
	      bsRootTree_->K2muCSCh_ = p2.numberOfValidMuonCSCHits();    // not-null, valid, muon CSC
	      bsRootTree_->K2muRPCh_ = p2.numberOfValidMuonRPCHits();     // not-null, valid, muon RPC
	      
	      ///////////////////////////////////////////////		
	      // number of pixel/tracker/muons hits muons
	      ///////////////////////////////////////////////
	      
	      int pixhits3 = 0;
	      const reco::HitPattern& p3 = Mu1Ref.get()->hitPattern();
	      for (int i=0; i<p3.numberOfHits(); i++) {
		uint32_t hit = p3.getHitPattern(i);
		if (p3.validHitFilter(hit) && p3.pixelBarrelHitFilter(hit)) pixhits3++;
		if (p3.validHitFilter(hit) && p3.pixelEndcapHitFilter(hit)) pixhits3++;
	      }
	      bsRootTree_->Mu1pixH_   = pixhits3;
	      bsRootTree_->Mu1trkH_   = p3.numberOfValidTrackerHits();
	      bsRootTree_->Mu1trkLay_ = p3.trackerLayersWithMeasurement();
	      bsRootTree_->Mu1muDTh_  = p3.numberOfValidMuonDTHits();      // not-null, valid, muon DT
	      bsRootTree_->Mu1muCSCh_ = p3.numberOfValidMuonCSCHits();    // not-null, valid, muon CSC
	      bsRootTree_->Mu1muRPCh_ = p3.numberOfValidMuonRPCHits();     // not-null, valid, muon RPC
	      
	      int pixhits4=0;
	      const reco::HitPattern& p4 = Mu2Ref.get()->hitPattern();
	      for (int i=0; i<p4.numberOfHits(); i++) {
		uint32_t hit = p4.getHitPattern(i);
		if (p4.validHitFilter(hit) && p4.pixelBarrelHitFilter(hit)) pixhits4++;
		if (p4.validHitFilter(hit) && p4.pixelEndcapHitFilter(hit)) pixhits4++;
	      }
	      bsRootTree_->Mu2pixH_   = pixhits4;
	      bsRootTree_->Mu2trkH_   = p4.numberOfValidTrackerHits();
	      bsRootTree_->Mu2trkLay_ = p4.trackerLayersWithMeasurement();
	      bsRootTree_->Mu2muDTh_  = p4.numberOfValidMuonDTHits();      // not-null, valid, muon DT
	      bsRootTree_->Mu2muCSCh_ = p4.numberOfValidMuonCSCHits();    // not-null, valid, muon CSC
	      bsRootTree_->Mu2muRPCh_ = p4.numberOfValidMuonRPCHits();     // not-null, valid, muon RPC
	    }
	    

	    // deltaR
	    
	    K1Truth = 0;
	    if(TrkCounter1!=-10){
	      double MinDRK=999.;
	      for(size_t i = 0; i < genParticles->size(); ++ i) {
		const GenParticle & p = (*genParticles)[i];
		double DeltaRK1 = TMath::Sqrt((p.eta()-track1.eta())*(p.eta()-track1.eta())+
					      (p.phi()-track1.phi())*(p.phi()-track1.phi()));
		if(DeltaRK1<MinDRK && DeltaRK1<0.05){
		  MinDRK=DeltaRK1;
		  K1mcId=p.pdgId();
		  if(p.mother()!=0) K1momId=p.mother()->pdgId();
		  if(p.mother()!=0 && p.mother()->mother()!=0) K1gmomId=p.mother()->mother()->pdgId(); 
		  if (abs(K1momId)==333 && abs(K1gmomId)==531) K1Truth = 1;
		}
	      }
	    }
	    
	    K2Truth = 0;
	    if(TrkCounter2!=-10){
	      double MinDRK=999.;
	      for(size_t i = 0; i < genParticles->size(); ++ i) {
		const GenParticle & p = (*genParticles)[i];
		double DeltaRK2 = TMath::Sqrt((p.eta()-track2.eta())*(p.eta()-track2.eta())+
					      (p.phi()-track2.phi())*(p.phi()-track2.phi()));
		if(DeltaRK2<MinDRK && DeltaRK2<0.05){
		  MinDRK=DeltaRK2;
		  K2mcId=p.pdgId();
		  if(p.mother()!=0) K2momId=p.mother()->pdgId();
		  if(p.mother()!=0 && p.mother()->mother()!=0) K2gmomId=p.mother()->mother()->pdgId(); 
		  if (abs(K2momId)==333 && abs(K2gmomId)==531) K2Truth = 1;
		}
	      }
	    }
	    
	    Mu1Truth = 0;
	    if(MuCounter1!=-10){
	      double MinDRMu=999.;
	      for(size_t i = 0; i < genParticles->size(); ++ i) {
		const GenParticle & p = (*genParticles)[i];
		double DeltaRMu1 = TMath::Sqrt((p.eta()-mu1jpsi.eta())*(p.eta()-mu1jpsi.eta())+
					       (p.phi()-mu1jpsi.phi())*(p.phi()-mu1jpsi.phi()));
		if(DeltaRMu1<MinDRMu && DeltaRMu1<0.05){
		  MinDRMu=DeltaRMu1;
		  Mu1mcId=p.pdgId();
		  if(p.mother()!=0) Mu1momId=p.mother()->pdgId();
		  if(p.mother()!=0 && p.mother()->mother()!=0) Mu1gmomId=p.mother()->mother()->pdgId(); 
		  if (abs(Mu1momId)==443 && abs(Mu1gmomId)==531) Mu1Truth = 1;
		}
	      }
	    }
	    
	    Mu2Truth = 0;
	    if(MuCounter2!=-10){
	      double MinDRMu=999.;
	      for(size_t i = 0; i < genParticles->size(); ++ i) {
		const GenParticle & p = (*genParticles)[i];
		double DeltaRMu2 = TMath::Sqrt((p.eta()-mu2jpsi.eta())*(p.eta()-mu2jpsi.eta())+
					       (p.phi()-mu2jpsi.phi())*(p.phi()-mu2jpsi.phi()));
		if(DeltaRMu2<MinDRMu && DeltaRMu2<0.05){
		  MinDRMu=DeltaRMu2;
		  Mu2mcId=p.pdgId();
		  if(p.mother()!=0) Mu2momId=p.mother()->pdgId();
		  if(p.mother()!=0 && p.mother()->mother()!=0) Mu2gmomId=p.mother()->mother()->pdgId(); 
		  if (abs(Mu2momId)==443 && abs(Mu2gmomId)==531) Mu2Truth= 1;
		}
	      }
	    }
	    
	    isMatched = -10;
	    if (K1Truth==1 && K2Truth==1 && Mu1Truth==1 & Mu2Truth==1) isMatched = 1;
	    bsRootTree_->getMCmatch(isMatched);
	    
	  } // trk2 loop
	} // trk1 loop
      } // mu counter

      ////////////////////////////////////
      ////////////////////////////////////
      ////           Kstar
      ////////////////////////////////////
      ////////////////////////////////////
      
      if (MuCounter1!=-10 && MuCounter2!=-10){
	const Candidate & mu1jpsi = (*allmuons)[MuCounter1];
	const Candidate & mu2jpsi = (*allmuons)[MuCounter2];
	
	Handle<CandidateView> allTracks;
	iEvent.getByLabel(trackLabelPi_, allTracks);
	
	for (size_t i=0; i< allTracks->size(); ++i){
	  for (size_t j=i+1; j< allTracks->size(); ++j){
	    const Candidate & track1 = (*allTracks)[i];
	    const Candidate & track2 = (*allTracks)[j];
	    if (track1.charge()==track2.charge()) continue;
	    if (track1.pt()<0.5) continue;
	    if (track2.pt()<0.5) continue;
	        
	    if(bsRootTree_->iPassedCutIdentBd_   < 4 ) bsRootTree_->iPassedCutIdentBd_ = 4 ;
	        
	    // kstar candidate
	        
	    double KaonMassSq=0.243717;
	    double KaonE1=sqrt(KaonMassSq+track1.px()*track1.px()+track1.py()*track1.py()+track1.pz()*track1.pz());
	    double KaonE2=sqrt(KaonMassSq+track2.px()*track2.px()+track2.py()*track2.py()+track2.pz()*track2.pz());
	    int K1flag=0;
	    int K2flag=0;
	    double Kstmass1  = sqrt((KaonE1+track2.energy())*(KaonE1+track2.energy())
				    -(track1.px()+track2.px())*(track1.px()+track2.px())
				    -(track1.py()+track2.py())*(track1.py()+track2.py())
				    -(track1.pz()+track2.pz())*(track1.pz()+track2.pz()));
	    double Kstmass2  = sqrt((KaonE2+track1.energy())*(KaonE2+track1.energy())
				    -(track1.px()+track2.px())*(track1.px()+track2.px())
				    -(track1.py()+track2.py())*(track1.py()+track2.py())
				    -(track1.pz()+track2.pz())*(track1.pz()+track2.pz()));
	        
	    if(abs(Kstmass1-0.892) < abs(Kstmass2-0.892)){
	      if(abs(Kstmass1-0.892)>0.12) continue;
	      K1flag=1;
	    } else{
	      if(abs(Kstmass2-0.892)>0.12) continue;
	      K2flag=1;
	    }
	    if(bsRootTree_->iPassedCutIdentBd_   < 5 ) bsRootTree_->iPassedCutIdentBd_ = 5 ;
	        
	    // jpsi candidate
	        
	    CompositeCandidate JpsiCand;
	    JpsiCand.addDaughter(mu1jpsi);
	    JpsiCand.addDaughter(mu2jpsi);
	    AddFourMomenta sum;
	    sum.set(JpsiCand);
	        
	    if (abs(JpsiCand.mass()-3.097)>0.15) continue;
	    if(bsRootTree_->iPassedCutIdentBd_   < 6 ) bsRootTree_->iPassedCutIdentBd_ = 6 ;
	        
	    // check on the overlap
	        
	    OverlapChecker overlap;
	    if (overlap(mu1jpsi,track1)!=0 || overlap(mu2jpsi,track1)!=0) continue;
	    if (overlap(mu1jpsi,track2)!=0 || overlap(mu2jpsi,track2)!=0) continue;
	        
	    if(bsRootTree_->iPassedCutIdentBd_   < 7 ) bsRootTree_->iPassedCutIdentBd_ = 7 ;

	    // B candidate
	        
	    CompositeCandidate BdCand;
	    BdCand.addDaughter(mu1jpsi);
	    BdCand.addDaughter(mu2jpsi);
	    BdCand.addDaughter(track1);
	    BdCand.addDaughter(track2);
	    AddFourMomenta add4mom;
	    add4mom.set(BdCand);
	      
	    if (BdCand.mass() < 4.5 || BdCand.mass() > 6.) continue;
	        
	    if(bsRootTree_->iPassedCutIdentBd_   < 8 ) bsRootTree_->iPassedCutIdentBd_ = 8 ;
	        
	    ///////////////////////////////////////////////////////////////////////////////
	    // list of particle for sequential kinematic vertex fitter - new giordano
	    ///////////////////////////////////////////////////////////////////////////////
	        
	    edm::ESHandle<TransientTrackBuilder> theB;
	    iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theB);
	    TrackRef trkMu1Ref = mu1jpsi.get<TrackRef>();
	    TrackRef trkMu2Ref = mu2jpsi.get<TrackRef>();
	    TrackRef trkkst1 = track1.get<TrackRef>();
	    TrackRef trkkst2 = track2.get<TrackRef>();
	        
	    vector<TransientTrack> t_tks;
	    t_tks.push_back((*theB).build(&trkMu1Ref));
	    t_tks.push_back((*theB).build(&trkMu2Ref));
	    t_tks.push_back((*theB).build(&trkkst1));
	    t_tks.push_back((*theB).build(&trkkst2));
	        
	    if(!trkMu1Ref.isNonnull() || !trkMu2Ref.isNonnull() || !trkkst1.isNonnull() || !trkkst2.isNonnull() ) continue;
	    if(bsRootTree_->iPassedCutIdentBd_   < 9 ) bsRootTree_->iPassedCutIdentBd_ = 9 ;
	        
	    TransientTrack ttMuP= t_tks[0];
	    TransientTrack ttMuM= t_tks[1];
	    TransientTrack ttK = t_tks[2];
	    TransientTrack ttP = t_tks[3];
	    KinematicParticleFactoryFromTransientTrack pFactory;
	        
	    ParticleMass muon_mass = 0.1056583;
	    ParticleMass kaon_mass = 0.493677;
	    ParticleMass pi_mass = 0.139570;
	        
	    float muon_sigma = 0.0000001;
	    float kaon_sigma = 0.000016;
	    float pi_sigma = 0.000016;
	    float chi = 0.;
	    float ndf = 0.;
	        
	    vector<RefCountedKinematicParticle> allMuons;
	    allMuons.push_back(pFactory.particle (ttMuP,muon_mass,chi,ndf,muon_sigma));
	    allMuons.push_back(pFactory.particle (ttMuM,muon_mass,chi,ndf,muon_sigma));
	    vector<RefCountedKinematicParticle> KpiParticles;
	    if(K1flag==1) {
	      KpiParticles.push_back(pFactory.particle (ttK,kaon_mass,chi,ndf,kaon_sigma));
	      KpiParticles.push_back(pFactory.particle (ttP,pi_mass,chi,ndf,pi_sigma));
	    }
	    else{
	      KpiParticles.push_back(pFactory.particle (ttP,kaon_mass,chi,ndf,kaon_sigma));
	      KpiParticles.push_back(pFactory.particle (ttK,pi_mass,chi,ndf,pi_sigma));
	    }
	        
	    KinematicParticleVertexFitter Fitter;
	    RefCountedKinematicTree JpsiTree = Fitter.fit(allMuons);

	    // if the fit fails, do not consider this as candidate
	    if(JpsiTree->isEmpty()) continue;
	        
	    KinematicParticleFitter constFitter;
	        
	    ParticleMass jpsiM = 3.09687;
	    float jpsiMsigma = 0.00004;
	    KinematicConstraint * jpsi_const = new MassKinematicConstraint(jpsiM,jpsiMsigma);
	        
	    JpsiTree = constFitter.fit(jpsi_const,JpsiTree);
	        
	    // if the fit fails, do not consider this as candidate
	    if(JpsiTree->isEmpty()) continue;
	        
	    JpsiTree->movePointerToTheTop();
	    RefCountedKinematicParticle Jpsi_branch = JpsiTree->currentParticle();
	    KpiParticles.push_back(Jpsi_branch);
	        
	    RefCountedKinematicTree myTree = Fitter.fit(KpiParticles);
	        
	    // if the fit fails, do not consider this as candidate
	    if(myTree->isEmpty())  continue;
	    myTree->movePointerToTheTop();
	    RefCountedKinematicParticle bmes = myTree->currentParticle();
	    RefCountedKinematicVertex bVertex = myTree->currentDecayVertex();
	        
	    if (!bVertex->vertexIsValid()) continue;
	    if(bsRootTree_->iPassedCutIdentBd_   < 10 ) bsRootTree_->iPassedCutIdentBd_ = 10 ;
	        
	    AlgebraicVector7 b_par = bmes->currentState().kinematicParameters().vector();
	    AlgebraicSymMatrix77 bd_er = bmes->currentState().kinematicParametersError().matrix();
	    double vtxProb = TMath::Prob(bmes->chiSquared(),(int)bmes->degreesOfFreedom());

	    if(vtxProb>MinBVtx){
	            
	      if (abs(JpsiCand.mass()-3.097) > JpsiMassWindowAfterFit_ || JpsiCand.pt() < JpsiPtCut_) continue;
	      // passed jpsi mass window after fit
	      if(bsRootTree_->iPassedCutIdentBd_   < 11 ) bsRootTree_->iPassedCutIdentBd_ = 11 ;
	            
	      if (K1flag == 1){
		if(abs(Kstmass1-0.892)>0.12) continue;
	      }
	      if (K2flag == 1){
		if(abs(Kstmass2-0.892)>0.12) continue;
	      }
	      // passed jpsi kstar window after fit
	      if(bsRootTree_->iPassedCutIdentBd_   < 12 ) bsRootTree_->iPassedCutIdentBd_ = 12 ;
	      if (BdCand.mass() < BsLowerMassCutAfterFit_ || BdCand.mass() > BsUpperMassCutAfterFit_) continue;
	      // passed Bd mass window after fit
	      if(bsRootTree_->iPassedCutIdentBd_   < 13 ) bsRootTree_->iPassedCutIdentBd_ = 13 ;
	    }

	    // deltaR matching!

	    int K1Truth = 0;
	    if(TrkCounter1!=-10){
	      double MinDRK=999.;
	      for(size_t i = 0; i < genParticles->size(); ++ i) {
		const GenParticle & p = (*genParticles)[i];
		double DeltaRK1 = TMath::Sqrt((p.eta()-track1.eta())*(p.eta()-track1.eta())+
					      (p.phi()-track1.phi())*(p.phi()-track1.phi()));
		if(DeltaRK1<MinDRK && DeltaRK1<0.05){
		  MinDRK=DeltaRK1;
		  K1mcIdBd=p.pdgId();
		  if(p.mother()!=0) K1momIdBd=p.mother()->pdgId();
		  if(p.mother()!=0 && p.mother()->mother()!=0) K1gmomIdBd=p.mother()->mother()->pdgId();
		  if (abs(K1momIdBd)==313 && abs(K1gmomIdBd)==511) K1Truth = 1;
		}
	      }
	    }

	    int K2Truth = 0;
	    if(TrkCounter2!=-10){
	      double MinDRK=999.;
	      for(size_t i = 0; i < genParticles->size(); ++ i) {
		const GenParticle & p = (*genParticles)[i];
		double DeltaRK2 = TMath::Sqrt((p.eta()-track2.eta())*(p.eta()-track2.eta())+
					      (p.phi()-track2.phi())*(p.phi()-track2.phi()));
		if(DeltaRK2<MinDRK && DeltaRK2<0.05){
		  MinDRK=DeltaRK2;
		  K2mcIdBd=p.pdgId();
		  if(p.mother()!=0) K2momIdBd=p.mother()->pdgId();
		  if(p.mother()!=0 && p.mother()->mother()!=0) K2gmomIdBd=p.mother()->mother()->pdgId();
		  if (abs(K2momIdBd)==313 && abs(K2gmomIdBd)==511) K2Truth = 1;
		}
	      }
	    }
	    
	    int Mu1Truth = 0;
	    if(MuCounter1!=-10){
	      double MinDRMu=999.;
	      for(size_t i = 0; i < genParticles->size(); ++ i) {
		const GenParticle & p = (*genParticles)[i];
		double DeltaRMu1 = TMath::Sqrt((p.eta()-mu1jpsi.eta())*(p.eta()-mu1jpsi.eta())+
					       (p.phi()-mu1jpsi.phi())*(p.phi()-mu1jpsi.phi()));
		if(DeltaRMu1<MinDRMu && DeltaRMu1<0.05){
		  MinDRMu=DeltaRMu1;
		  Mu1mcIdBd=p.pdgId();
		  if(p.mother()!=0) Mu1momIdBd=p.mother()->pdgId();
		  if(p.mother()!=0 && p.mother()->mother()!=0) Mu1gmomIdBd=p.mother()->mother()->pdgId();
		  if (abs(Mu1momIdBd)==443 && abs(Mu1gmomIdBd)==511 && abs(Mu1mcIdBd)==13) Mu1Truth = 1;
		}
	      }
	    }
	    
	    int Mu2Truth = 0;
	    if(MuCounter2!=-10){
	      double MinDRMu=999.;
	      for(size_t i = 0; i < genParticles->size(); ++ i) {
		const GenParticle & p = (*genParticles)[i];
		double DeltaRMu2 = TMath::Sqrt((p.eta()-mu2jpsi.eta())*(p.eta()-mu2jpsi.eta())+
					       (p.phi()-mu2jpsi.phi())*(p.phi()-mu2jpsi.phi()));
		if(DeltaRMu2<MinDRMu && DeltaRMu2<0.05){
		  MinDRMu=DeltaRMu2;
		  Mu2mcIdBd=p.pdgId();
		  if(p.mother()!=0) Mu2momIdBd=p.mother()->pdgId();
		  if(p.mother()!=0 && p.mother()->mother()!=0) Mu2gmomIdBd=p.mother()->mother()->pdgId();
		  if (abs(Mu2momIdBd)==443 && abs(Mu2gmomIdBd)==511 && abs(Mu2mcIdBd)==13) Mu2Truth= 1;
		}
	      }
	    }
	    
	    isMatchedBd = -10;
	    if (K1Truth==1 && K2Truth==1 && Mu1Truth==1 && Mu2Truth==1 &&
		(abs(K1mcIdBd)==321 && abs(K2mcIdBd)==211) || (abs(K1mcIdBd)==211 && abs(K2mcIdBd)==321)) isMatchedBd = 1;
	    bsRootTree_->isMatchedBd_ = isMatchedBd;
	    
	    
	  } // trk2 end loop
	} // trk1 end loop
      } // mu counter for kstar      

      
    } // loop on muons2
  } // loop on muons1
  
  bsRootTree_->fill();

}



void BsToJpsiPhiAnalysis::fillMCInfo( edm::Handle<GenParticleCollection> & genParticles){


  for( size_t i = 0; i < genParticles->size(); ++ i ) {
    const GenParticle & genBsCand = (*genParticles)[ i ];
    int MC_particleID=genBsCand.pdgId();

    if (abs(MC_particleID) == 5 ||   // b quark
        // bottom mesons
        abs(MC_particleID) == 511 ||   // Bd
        abs(MC_particleID) == 521 ||   // B+
        abs(MC_particleID) == 10511 ||    // B_0*0
        abs(MC_particleID) == 10521 ||    // B_0*+
        abs(MC_particleID) == 513 ||   // B*d
        abs(MC_particleID) == 523 ||   // B*d+
        abs(MC_particleID) == 10513 ||   // B1(L)0
        abs(MC_particleID) == 10523 ||   // B1(L)+
        abs(MC_particleID) == 20513 ||   // B1(H)0
        abs(MC_particleID) == 20523 ||   // B1(H)+
        abs(MC_particleID) == 515 ||    // B2*_0
        abs(MC_particleID) == 525 ||    // B2*_+
        abs(MC_particleID) == 531 ||   // Bs
        abs(MC_particleID) == 10531 ||    // B_s0*_0
        abs(MC_particleID) == 533 ||   // B*s
        abs(MC_particleID) == 10533 ||   // Bs1(L)0
        abs(MC_particleID) == 20533 ||   // Bs1(H)0
        abs(MC_particleID) == 535 ||    // Bs2*_0
        abs(MC_particleID) == 541 ||   // Bc+
        abs(MC_particleID) == 10541 ||   // B*c0+
        abs(MC_particleID) == 543 ||   // B*c+
        abs(MC_particleID) == 10543 ||   // Bc1(L)+
        abs(MC_particleID) == 20543 ||   // Bc1(H)+
        abs(MC_particleID) == 545 ||    // Bc2*_0
        // bb mesons
        abs(MC_particleID) == 551 ||   // etab(1S)
        abs(MC_particleID) == 10551 ||   // chib(1P)
        abs(MC_particleID) == 100551 ||   // etab(2S)
        abs(MC_particleID) == 110551 ||   // chib(2P)
        abs(MC_particleID) == 200551 ||   // etab(3S)
        abs(MC_particleID) == 210551 ||   // chib(3P)
        abs(MC_particleID) == 553 ||   // upsilon(1S)
        abs(MC_particleID) == 10553 ||   // hb(1P)
        abs(MC_particleID) == 20553 ||   // chib1(1P)
        abs(MC_particleID) == 30553 ||   // upsilon1(1D)
        abs(MC_particleID) == 100553 ||   // upsilon(2S)
        abs(MC_particleID) == 110553 ||   // hb(2P)
        abs(MC_particleID) == 120553 ||   // chib1(2P)
        abs(MC_particleID) == 130553 ||   // upsilon1(2D)
        abs(MC_particleID) == 200553 ||   // upsilon(3S)
        abs(MC_particleID) == 210553 ||   // hb(3P)
        abs(MC_particleID) == 220553 ||   // chib1(3P)
        abs(MC_particleID) == 300553 ||   // upsilon(4S)
        abs(MC_particleID) == 9000553 ||   // upsilon(10860)
        abs(MC_particleID) == 9010553 ||   // upsilon(11020)
        abs(MC_particleID) == 555 ||   // chib2(1P)
        abs(MC_particleID) == 10555 ||   // etab2(1D)
        abs(MC_particleID) == 20555 ||   // upsilon2(1D)
        abs(MC_particleID) == 100555 ||   // chib2(2P)
        abs(MC_particleID) == 110555 ||   // etab2(2D)
        abs(MC_particleID) == 120555 ||   // upsilon2(2D)
        abs(MC_particleID) == 200555 ||   // chib2(3P)
        abs(MC_particleID) == 557 ||   // upsilon3(1D)
        abs(MC_particleID) == 100557 ||   // upsilon3(2D)
        // bottom barions
        abs(MC_particleID) == 5122 ||   // lambda_b0
        abs(MC_particleID) == 5112 ||   // sigma_b-
        abs(MC_particleID) == 5212 ||   // sigma_b0
        abs(MC_particleID) == 5222 ||   // sigma_b+
        abs(MC_particleID) == 5114 ||   // sigma*_b-
        abs(MC_particleID) == 5214 ||   // sigma*_b0
        abs(MC_particleID) == 5224 ||   // sigma*_b+
        abs(MC_particleID) == 5132 ||   // Xi_b-
        abs(MC_particleID) == 5232 ||   // Xi_b0
        abs(MC_particleID) == 5312 ||   // Xi'_b-
        abs(MC_particleID) == 5322 ||   // Xi'_b0
        abs(MC_particleID) == 5314 ||   // Xi*_b-
        abs(MC_particleID) == 5324 ||   // Xi*_b0
        abs(MC_particleID) == 5332 ||   // Omega_b-
        abs(MC_particleID) == 5334 ||   // Omega*_b-
        abs(MC_particleID) == 5142 ||   // Xi_bc0
        abs(MC_particleID) == 5242 ||   // Xi_bc+
        abs(MC_particleID) == 5412 ||   // Xi'_bc0
        abs(MC_particleID) == 5422 ||   // Xi'_bc+
        abs(MC_particleID) == 5414 ||   // Xi*_bc0
        abs(MC_particleID) == 5424 ||   // Xi*_bc+
        abs(MC_particleID) == 5342 ||   // Omega_bc0
        abs(MC_particleID) == 5432 ||   // Omega'_bc0
        abs(MC_particleID) == 5434 ||   // Omega*_bc0
        abs(MC_particleID) == 5442 ||   // Omega_bcc+
        abs(MC_particleID) == 5444 ||   // Omega*_bcc+
        abs(MC_particleID) == 5512 ||   // Xi_bb-
        abs(MC_particleID) == 5522 ||   // Xi_bb0
        abs(MC_particleID) == 5514 ||   // Xi*_bb-
        abs(MC_particleID) == 5524 ||   // Xi*_bb0
        abs(MC_particleID) == 5532 ||   // Omega_bb-
        abs(MC_particleID) == 5524 ||   // Omega*_bb-
        abs(MC_particleID) == 5542 ||   // Omega_bbc0
        abs(MC_particleID) == 5544 ||   // Omega*_bbc0
        abs(MC_particleID) == 5554 )   // Omega_bbb-
      {
        bsRootTree_->BmesonsId_ = MC_particleID;
	
        int numBsDaughters = genBsCand.numberOfDaughters();
        bsRootTree_->GenNumberOfDaughters_ = numBsDaughters;
	// generator variables
	bsRootTree_->BMMC_ = genBsCand.mass();
	bsRootTree_->BPtMC_ = genBsCand.pt();
	bsRootTree_->BPzMC_ = genBsCand.pz();
	bsRootTree_->BEtaMC_ = genBsCand.eta();
	bsRootTree_->BPhiMC_ = genBsCand.phi();
	//generated primary vertex
	if (abs(MC_particleID)== 531){
	  bsRootTree_->genBsVtx_x_= genBsCand.mother(0)->vx();
	  bsRootTree_->genBsVtx_y_= genBsCand.mother(0)->vy();
	  bsRootTree_->genBsVtx_z_= genBsCand.mother(0)->vz();
	}
	
        for(int j = 0; j < numBsDaughters; ++ j) {
	  const Candidate * Bsdau = genBsCand.daughter( j );
	  bsRootTree_->BDauIdMC_[j] = Bsdau->pdgId();
	  // generator variables
	  bsRootTree_->BDauMMC_[j] = Bsdau->mass();
	  bsRootTree_->BDauPtMC_[j] = Bsdau->pt();
	  bsRootTree_->BDauPzMC_[j] = Bsdau->pz();
	  bsRootTree_->BDauEtaMC_[j] = Bsdau->eta();
	  bsRootTree_->BDauPhiMC_[j] = Bsdau->phi();
	  //Generated secondary vertex.
	  if (Bsdau->pdgId()== 333){
	    bsRootTree_->genBsSVtx_x_= Bsdau->vx();
	    bsRootTree_->genBsSVtx_y_= Bsdau->vy();
	    bsRootTree_->genBsSVtx_z_= Bsdau->vz();
	  }
	} // loop Bs daughters
      }
  }
}
