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
  fillMCInfoBd(genParticles);
  
  
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
      
      CompositeCandidate Jpsi;
      Jpsi.addDaughter(trk1);
      Jpsi.addDaughter(trk2);
      AddFourMomenta addP4;
      addP4.set(Jpsi);
      
      if ( abs(Jpsi.mass()-3.096916) > JpsiMassWindowBeforeFit_ || Jpsi.pt() < JpsiPtCut_) continue;
      // passed jpsi mass and pt cut
      if(bsRootTree_->iPassedCutIdent_   < 2 ) bsRootTree_->iPassedCutIdent_ = 2 ;
      
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
	    
	    if (!Mu1Ref.isNonnull() || !Mu2Ref.isNonnull() || !trk1Ref.isNonnull() || !trk1Ref.isNonnull()) continue;
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
	    
	    // deltaR MC matching!
	    
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
	    
	    bsRootTree_->getInfoK1(K1mcId,K1momId,K1gmomId,K1Truth);
	    
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
	    
	    bsRootTree_->getInfoK2(K2mcId,K2momId,K2gmomId,K2Truth);
	    
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
	    
	    bsRootTree_->getInfoMu1(Mu1mcId,Mu1momId,Mu1gmomId,Mu1Truth);
	    
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
	    
	    bsRootTree_->getInfoMu2(Mu2mcId,Mu2momId,Mu2gmomId,Mu2Truth);
	    
	    isMatched = -10;
	    if (K1Truth==1 && K2Truth==1 && Mu1Truth==1 & Mu2Truth==1) isMatched = 1;
	    bsRootTree_->getMCmatch(isMatched);
	    
	  } // trk2 loop
	} // trk1 loop
      } // loop on muons
    } // end muCounter1 and muCounter2 if statement
  } // loop on muons
  
  bsRootTree_->fill();

}



void BsToJpsiPhiAnalysis::fillMCInfo( edm::Handle<GenParticleCollection> & genParticles){
  
  
  for( size_t i = 0; i < genParticles->size(); ++ i ) {
    const Candidate & p = (*genParticles)[ i ];
    int MC_particleID=p.pdgId();
    if (abs(MC_particleID) == 443 && p.status()==2){
      const Candidate & gmo=*(p.mother());
      bsRootTree_->nBsDau_ = gmo.numberOfDaughters();
      bsRootTree_->momID_ = gmo.pdgId();
      if (bsRootTree_->nBsDau_ == 2){
	const Candidate & da1=*(gmo.daughter( 0 ));
	const Candidate & da2=*(gmo.daughter( 1 ));
	bsRootTree_->dau_1_ID_ = da1.pdgId();
	bsRootTree_->dau_2_ID_ = da2.pdgId();
      }
    }
  }
  
  
  
  
  //added by Alex: check type of event
  const Candidate * genBs, *genJpsi, *genPhi, *genMu1, *genMu2, *genK1, *genK2;
  genBs = genJpsi = genPhi = genMu1 = genMu2 = genK1 = genK2 = 0;
  
  int GenNumberOfFoundBsMesons = 0;
  
  for( size_t i = 0; i < genParticles->size(); ++ i ) {
    const GenParticle & genBsCand = (*genParticles)[ i ];
    int MC_particleID=genBsCand.pdgId();    
    if( abs(MC_particleID == 531) ){
      GenNumberOfFoundBsMesons++;
      bsRootTree_->isGenBsEvent_ = 1;
      
      // find daughters
      int numBsDaughters = genBsCand.numberOfDaughters();
      bsRootTree_->GenNumberOfBsDaughters_ = numBsDaughters;

      // events Bs->Jpsi KK
      if (numBsDaughters == 3 ){
	const Candidate* Jpsi = 0;
	const Candidate* Kp = 0;
	const Candidate* Km = 0;
	for(int j = 0; j < numBsDaughters; ++ j) {
	  const Candidate * d = genBsCand.daughter( j );
	  int dauId = d->pdgId();
	  if(abs(dauId) == 443 ) Jpsi = d;
	  if(abs(dauId) == 321 ) Kp = d;
	  if(abs(dauId) == 321 ) Km = d;
	}
	if (Jpsi && Kp && Km)
	  bsRootTree_->isGenBsJpsiKKEvent_ = 1;
      }
      
      if (numBsDaughters == 2){
	const Candidate * candJpsi = 0;
	const Candidate * candPhi = 0;
	for(int j = 0; j < numBsDaughters; ++ j) {
	  const Candidate * d = genBsCand.daughter( j );
	  int dauId = d->pdgId();
	  
	  if(abs(dauId) == 443 ) candJpsi = d;
	  if(abs(dauId) == 333  )candPhi = d;
	  
	}
	
	bool bFoundJpsiPhi = ((candJpsi!=0) && (candPhi!=0)); // found Bs -> Jpsi Phi
	bsRootTree_->isGenBsJpsiPhiEvent_ = bFoundJpsiPhi;
	if(bFoundJpsiPhi){
	  // loop over jpsi daughters, check for muons
	  const Candidate * candMu1 = 0;
	  const Candidate * candMu2 = 0;
	  for(unsigned int k=0; k < candJpsi->numberOfDaughters(); k++){
	    const Candidate * jpsiDa = candJpsi->daughter( k );
	    if(  jpsiDa->pdgId() == 13 ) candMu1  = jpsiDa;
	    if(  jpsiDa->pdgId() == -13 ) candMu2 = jpsiDa;
	    //	    cout<<"      **** AS: jpsiDaughter pid = " << jpsiDa->pdgId() << endl;
	  }
	  
	  
	  // loop over phi daughters check for Kaons
	  const Candidate * candKaon1 = 0;
	  const Candidate * candKaon2 = 0;
	  for(unsigned int ikaon=0 ; ikaon < candPhi->numberOfDaughters(); ikaon++){
	    const Candidate * candK = candPhi->daughter ( ikaon );
	    if( candK->pdgId() == 321 ) candKaon1 = candK;
	    if( candK->pdgId() == -321 ) candKaon2 = candK;
	    //	    cout<<"     ****** AS phi daughter pid = " << candK -> pdgId() << endl;
	  }
	  // if all fulfilled, set Gen flag and set pointers to gen particles
	  
	  if( candMu1 && candMu2 && candKaon1 && candKaon2 ){
	    bsRootTree_->isGenBsJpsiPhiMuMuKKEvent_ = 1;
	    
	    genBs = &genBsCand;
	    genPhi = candPhi;
	    genJpsi = candJpsi;
	    genMu1 = candMu1;
	    genMu2 = candMu2;
	    genK1 = candKaon1;
	    genK2 = candKaon2;
	    
	    // consistency check
	    bool Bconsistent = genBs && genPhi && genJpsi && genMu1 && genMu2 && genK1 && genK2;
	    if( !Bconsistent ) {
	      cout<<"    **** AS:  !Bconsistent " << endl;
	      exit(1);
	      
	    }
	  }

	}//  end if(bFoundJpsiPhi)
      }
      
    }
    
  }// AS: end loop over GenParticles
  
  
  // fill particles into tree
  if(genBs) {
    bsRootTree_->genBsM_   =  genBs->mass();
    bsRootTree_->genBsPt_  =  genBs->pt();
    bsRootTree_->genBsPz_  =  genBs->pz();
    bsRootTree_->genBsEta_ =  genBs->eta();
    bsRootTree_->genBsPhi_ =  genBs->phi();
  }
  if(genPhi) {
    bsRootTree_->genPhiM_   =  genPhi->mass();
    bsRootTree_->genPhiPt_  =  genPhi->pt();
    bsRootTree_->genPhiPz_  =  genPhi->pz();
    bsRootTree_->genPhiEta_ =  genPhi->eta();
    bsRootTree_->genPhiPhi_ =  genPhi->phi();
    
    bsRootTree_->genK1M_   =  genK1->mass();
    bsRootTree_->genK1Pt_  =  genK1->pt();
    bsRootTree_->genK1Pz_  =  genK1->pz();
    bsRootTree_->genK1Eta_ =  genK1->eta();
    bsRootTree_->genK1Phi_ =  genK1->phi();
    
    bsRootTree_->genK2M_   =  genK2->mass();
    bsRootTree_->genK2Pt_  =  genK2->pt();
    bsRootTree_->genK2Pz_  =  genK2->pz();
    bsRootTree_->genK2Eta_ =  genK2->eta();
    bsRootTree_->genK2Phi_ =  genK2->phi();
  }
  if(genJpsi) {
    bsRootTree_->genJpsiM_   =  genJpsi->mass();
    bsRootTree_->genJpsiPt_  =  genJpsi->pt();
    bsRootTree_->genJpsiPz_  =  genJpsi->pz();
    bsRootTree_->genJpsiEta_ =  genJpsi->eta();
    bsRootTree_->genJpsiPhi_ =  genJpsi->phi();
    
    bsRootTree_->genMu1M_   =  genMu1->mass();
    bsRootTree_->genMu1Pt_  =  genMu1->pt();
    bsRootTree_->genMu1Pz_  =  genMu1->pz();
    bsRootTree_->genMu1Eta_ =  genMu1->eta();
    bsRootTree_->genMu1Phi_ =  genMu1->phi();
    
    bsRootTree_->genMu2M_   =  genMu2->mass();
    bsRootTree_->genMu2Pt_  =  genMu2->pt();
    bsRootTree_->genMu2Pz_  =  genMu2->pz();
    bsRootTree_->genMu2Eta_ =  genMu2->eta();
    bsRootTree_->genMu2Phi_ =  genMu2->phi();
  }
  
}


void BsToJpsiPhiAnalysis::fillMCInfoBd( edm::Handle<GenParticleCollection> & genParticles){

  int GenNumberOfFoundBdMesons = 0;
  
  for( size_t i = 0; i < genParticles->size(); ++ i ) {
    const GenParticle & genBdCand = (*genParticles)[ i ];
    int MC_particleID=genBdCand.pdgId();    
    if( abs(MC_particleID == 511) ){
      GenNumberOfFoundBdMesons++;
      bsRootTree_->isGenBdEvent_ = 1;
      
      // find daughters
      int numBdDaughters = genBdCand.numberOfDaughters();
      bsRootTree_->GenNumberOfBdDaughters_ = numBdDaughters;
      
      const Candidate * candJpsi = 0;
      const Candidate * candKstar = 0;
      for(int j = 0; j < numBdDaughters; ++ j) {
	const Candidate * d = genBdCand.daughter( j );
	int dauId = d->pdgId();
	
	bsRootTree_->BdDausId_ = dauId;

	if(abs(dauId) == 443 ) candJpsi = d;
	if(abs(dauId) == 313  )candKstar = d;
	
      }
      bool bFoundJpsiKstar = ((candJpsi!=0) && (candKstar!=0)); // found Bd -> Jpsi Kstar
      bsRootTree_->isGenBdJpsiKstarEvent_ = bFoundJpsiKstar;
      if(bFoundJpsiKstar){
	// loop over jpsi daughters, check for muons
	const Candidate * candMu1 = 0;
	const Candidate * candMu2 = 0;
	for(unsigned int k=0; k < candJpsi->numberOfDaughters(); k++){
	  const Candidate * jpsiDa = candJpsi->daughter( k );
	  if(  jpsiDa->pdgId() == 13 ) candMu1  = jpsiDa;
	  if(  jpsiDa->pdgId() == -13 ) candMu2 = jpsiDa;
	}
	
	
	// loop over kstar daughters check for Kaons
	const Candidate * candKaon1 = 0;
	const Candidate * candKaon2 = 0;
	for(unsigned int ikaon=0 ; ikaon < candKstar->numberOfDaughters(); ikaon++){
	  const Candidate * candK = candKstar->daughter ( ikaon );
	  if( candK->pdgId() == 321 ) candKaon1 = candK;
	  if( candK->pdgId() == -211 ) candKaon2 = candK;
	}
	const Candidate * candKaon3 = 0;
	const Candidate * candKaon4 = 0;
	for(unsigned int ikaon=0 ; ikaon < candKstar->numberOfDaughters(); ikaon++){
	  const Candidate * candK = candKstar->daughter ( ikaon );
	  if( candK->pdgId() == -321 ) candKaon3 = candK;
	  if( candK->pdgId() == 211 ) candKaon4 = candK;
	}
	const Candidate * candKaon5 = 0;
	const Candidate * candKaon6 = 0;
	for(unsigned int ikaon=0 ; ikaon < candKstar->numberOfDaughters(); ikaon++){
	  const Candidate * candK = candKstar->daughter ( ikaon );
	  if( candK->pdgId() == -211 ) candKaon5 = candK;
	  if( candK->pdgId() == 321 ) candKaon6 = candK;
	}
	const Candidate * candKaon7 = 0;
	const Candidate * candKaon8 = 0;
	for(unsigned int ikaon=0 ; ikaon < candKstar->numberOfDaughters(); ikaon++){
	  const Candidate * candK = candKstar->daughter ( ikaon );
	  if( candK->pdgId() == 211 ) candKaon7 = candK;
	  if( candK->pdgId() == -321 ) candKaon8 = candK;
	}
	
	
	if( candMu1 && candMu2 && candKaon1 && candKaon2 ){
	  bsRootTree_->isGenBdJpsiKstarMuMuKpiEvent_ = 1;
	}
	if( candMu1 && candMu2 && candKaon3 && candKaon4 ){
	  bsRootTree_->isGenBdJpsiKstarMuMuKpiEvent_ = 1;
	}
	if( candMu1 && candMu2 && candKaon5 && candKaon6 ){
	  bsRootTree_->isGenBdJpsiKstarMuMuKpiEvent_ = 1;
	}
	if( candMu1 && candMu2 && candKaon7 && candKaon8 ){
	  bsRootTree_->isGenBdJpsiKstarMuMuKpiEvent_ = 1;
	}
	
	
      }//  end if(bFoundJpsiPhi)
      
    }
    
  }// AS: end loop over GenParticles
  
}

