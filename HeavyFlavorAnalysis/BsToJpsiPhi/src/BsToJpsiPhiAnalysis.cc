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
  track1_ = iConfig.getParameter<edm::InputTag>("Tracks1");
  track2_ = iConfig.getParameter<edm::InputTag>("Tracks2");
  usePixel_ = iConfig.getParameter<bool>("usePixel"); 
  useStrip_ = iConfig.getParameter<bool>("useStrip");
  trajtracks_ = iConfig.getParameter<edm::InputTag>("TrajectoryTracks");
  triggerTag_ = iConfig.getParameter<edm::InputTag>("TriggerTag");
  muonTag_ = iConfig.getParameter<edm::InputTag>("MuonTag");
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

  using namespace DeDxTools; 
 
  try {
    bsRootTree_->resetEntries();

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
    
    for( size_t i = 0; i < genParticles->size(); ++ i ) {
      const Candidate & p = (*genParticles)[ i ];
      int MC_particleID=p.pdgId();
      if (abs(MC_particleID) == 443 && p.status()==2){
	const Candidate & gmo=*(p.mother());
	nBsDau=gmo.numberOfDaughters();
	momID=gmo.pdgId();
	if (nBsDau ==2){
	  const Candidate & da1=*(gmo.daughter( 0 ));
	  const Candidate & da2=*(gmo.daughter( 1 ));
	  dau_1_ID=da1.pdgId();
	  dau_2_ID=da2.pdgId();
	}
      }
    }
    
    bsRootTree_->getParams(nBsDau,momID,dau_1_ID,dau_2_ID);
    
    flagKstar = JpsiKstarFlag(iEvent);
    flagKs = JpsiKsFlag(iEvent);
    flagPhi = JpsiPhiFlag(iEvent);
    bsRootTree_->getBdFlags(flagKstar,flagKs,flagPhi);
  
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

    flag_1 = 0;
    flag_2 = 0;
    flag_3 = 0;
    flag_4 = 0;
    flag_5 = 0;
    flag_6 = 0;

    /*
    int hltBits[6];
    for (int i=0; i<6; i++) hltBits[i]= -10;
    string HLTbitNames[6] = {"HLT_Mu3", "HLT_Mu5", "HLT_Mu9", "HLT_DoubleIsoMu3", "HLT_DoubleMu3", "HLT_DoubleMu3_JPsi"};

    // check if trigger name in config
    const unsigned int n(hltresults->size());
    for (int ihlt = 0; ihlt < 6; ihlt++) {
      hltBits[ihlt] = 0;
      unsigned int triggerIndex(triggerNames_.triggerIndex(HLTbitNames[ihlt]) );
      if (triggerIndex<n) hltBits[ihlt] = triggerIndex;
      bsRootTree_->getTrigBit(hltBits[0],hltBits[1],hltBits[2],hltBits[3],hltBits[4],hltBits[5]);
    }
    */

    int ntrigs = hltresults->size();
    for (int itrig = 0; itrig != ntrigs; ++itrig){
      
      TString trigName=triggerNames_.triggerName(itrig);
      
      if (trigName=="HLT_Mu3"){
	bool accept = hltresults->accept(itrig);
	if (accept) flag_1 = 1;	
	}
      
      if (trigName=="HLT_Mu5"){
	bool accept = hltresults->accept(itrig);
	if (accept) flag_2 = 1;	
      }
      
      if (trigName=="HLT_Mu9"){
	bool accept = hltresults->accept(itrig);
	if (accept) flag_3 = 1;	
      }
      
      if (trigName=="HLT_DoubleIsoMu3"){
	bool accept = hltresults->accept(itrig);
	if (accept) flag_4 = 1;	
      }
      
      if (trigName=="HLT_DoubleMu3"){
	bool accept = hltresults->accept(itrig);
	if (accept) flag_5 = 1;	
      }
      
      if (trigName=="HLT_DoubleMu3_JPsi"){
	bool accept = hltresults->accept(itrig);
	if (accept) flag_6 = 1;	
      }
      
      bsRootTree_->getTrigBit(flag_1,flag_2,flag_3,flag_4,flag_5,flag_6);			  
      
    }

    // all muons
   
    edm::Handle<reco::MuonCollection> allmuons;
    iEvent.getByLabel(muonTag_,allmuons);

    for(size_t i=0; i < allmuons->size(); ++i){
      const Muon & trk1 = (*allmuons)[i];
      if(!trk1.isGlobalMuon() && !trk1.isTrackerMuon()) continue;
      for (size_t j=i+1; j < allmuons->size(); ++j){
	const Muon & trk2 = (*allmuons)[j];
	if(!trk2.isGlobalMuon() && !trk2.isTrackerMuon()) continue;

	if(trk1.charge()==trk2.charge()) continue;

	CompositeCandidate Jpsi;
	Jpsi.addDaughter(trk1);
	Jpsi.addDaughter(trk2);
	AddFourMomenta addP4;
	addP4.set(Jpsi);
	
	if (abs(Jpsi.mass()-3.096916)>0.150 || Jpsi.pt()<3) continue;

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

	Vertex vertex = tv;
	GlobalPoint secondaryVertex = tv.position();

	double vtxProb_Jpsi = TMath::Prob(vertex.chi2(),(int)vertex.ndof()); 
	
	double minM = 200.;

	if(abs(Jpsi.mass()-3.096916)<minM){
	  minM = abs(Jpsi.mass()-3.096916);
	  JpsiVtxProb = vtxProb_Jpsi;
	  MuCounter1 = i;
	  MuCounter2 = j;	  
	  JpsiM_alone = Jpsi.mass();	  
	  JpsiPhi_alone = Jpsi.phi();	  
	  JpsiEta_alone = Jpsi.phi();	  
	  JpsiPt_alone = Jpsi.pt();	  
	  JpsiMu1Pt = trk1.pt();	  
	  JpsiMu2Pt = trk2.pt();	  
	  JpsiMu1Phi = trk1.phi();	  
	  JpsiMu2Phi = trk2.phi();	  
	  JpsiMu1Eta = trk1.eta();	  
	  JpsiMu2Eta = trk2.eta();	  
	  
	  // muon categories:
	  // 1: tracker muons
	  // 2: global muons
	  // 3: global + tracker muon
	  // 4: neither tracker nor global
	  
	  if (trk1.isTrackerMuon() && !trk1.isGlobalMuon()) JpsiMuon1Cat = 1;
	  else if (!trk1.isTrackerMuon() && trk1.isGlobalMuon()) JpsiMuon1Cat = 2;
	  else if (trk1.isTrackerMuon() && trk1.isGlobalMuon()) JpsiMuon1Cat = 3;
	  else if (!trk1.isTrackerMuon() && !trk1.isGlobalMuon()) JpsiMuon1Cat = 4;
	  
	  if (trk2.isTrackerMuon() && !trk2.isGlobalMuon()) JpsiMuon2Cat = 1;
	  else if (!trk2.isTrackerMuon() && trk2.isGlobalMuon()) JpsiMuon2Cat = 2;
	  else if (trk2.isTrackerMuon() && trk2.isGlobalMuon()) JpsiMuon2Cat = 3;
	  else if (!trk2.isTrackerMuon() && !trk2.isGlobalMuon()) JpsiMuon2Cat = 4;
	  
	  bsRootTree_->getJpsiAlone(JpsiVtxProb,MuCounter1,MuCounter2,JpsiM_alone,JpsiPhi_alone,JpsiEta_alone,JpsiPt_alone,
				    JpsiMu1Pt,JpsiMu2Pt,JpsiMu1Phi,JpsiMu2Phi,JpsiMu1Eta,JpsiMu2Eta,JpsiMuon1Cat,JpsiMuon2Cat);
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

	  for (size_t i=0; i< allTracks->size(); ++i){
	    for (size_t j=i+1; j< allTracks->size(); ++j){

	      const Candidate & track1 = (*allTracks)[i];
	      const Candidate & track2 = (*allTracks)[j];

	      if (track1.charge()==track2.charge()) continue;
	      if (track1.pt()<0.5) continue;
	      if (track2.pt()<0.5) continue;

	      // phi candidate

	      CompositeCandidate PhiCand;
	      PhiCand.addDaughter(track1);
	      PhiCand.addDaughter(track2);
	      AddFourMomenta ad;
	      ad.set(PhiCand);

	      if (abs(PhiCand.mass()-1.019)>0.05) continue;
	      PhiMass_before = PhiCand.mass();

	      // jpsi candidate

	      CompositeCandidate JpsiCand;
	      JpsiCand.addDaughter(mu1jpsi);
	      JpsiCand.addDaughter(mu2jpsi);
	      AddFourMomenta sum;
	      sum.set(JpsiCand);

	      if (abs(JpsiCand.mass()-3.097)>0.15) continue;	      
	      JpsiMass_before = JpsiCand.mass();

	      // check on the overlap
	      
	      OverlapChecker overlap;
	      if (overlap(mu1jpsi,track1)!=0 || overlap(mu2jpsi,track1)!=0) continue;
	      if (overlap(mu1jpsi,track2)!=0 || overlap(mu2jpsi,track2)!=0) continue;

	      // B candidate

	      CompositeCandidate BCand;
	      BCand.addDaughter(mu1jpsi);
	      BCand.addDaughter(mu2jpsi);
	      BCand.addDaughter(track1);
	      BCand.addDaughter(track2);
	      AddFourMomenta add4mom;
	      add4mom.set(BCand);

	      if (BCand.mass() < 4.5 || BCand.mass() > 6.) continue;
	      BsMass_before = BCand.mass();
	      bsRootTree_->getPar(BsMass_before,JpsiMass_before,PhiMass_before);
	      

	      // start fit on the B candidates

	      edm::ESHandle<TransientTrackBuilder> theB;
	      iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theB);	
	      TrackRef Mu1Ref = mu1jpsi.get<TrackRef>();
	      TrackRef Mu2Ref = mu2jpsi.get<TrackRef>();
	      TrackRef trk1Ref = track1.get<TrackRef>();
	      TrackRef trk2Ref = track2.get<TrackRef>();
	      
	      vector<TransientTrack> t_tracks;
	      t_tracks.push_back((*theB).build(&Mu1Ref));
	      t_tracks.push_back((*theB).build(&Mu2Ref));
	      t_tracks.push_back((*theB).build(&trk1Ref));
	      t_tracks.push_back((*theB).build(&trk2Ref));
	      
	      if (!Mu1Ref.isNonnull() || !Mu2Ref.isNonnull() || !trk1Ref.isNonnull() || !trk1Ref.isNonnull()) continue;
	      
	      TransientTrack track_MuP = t_tracks[0];
	      TransientTrack track_MuM = t_tracks[1];
	      TransientTrack track_Kp = t_tracks[2];
	      TransientTrack track_Km = t_tracks[3];
	    
	      //Creating a KinematicParticleFactory
	      KinematicParticleFactoryFromTransientTrack pFactory;
	      
	      //The mass of a muon and the insignificant mass sigma to avoid singularities in the covariance matrix.
	      ParticleMass muon_mass = 0.1056583;
	      ParticleMass kaon_mass = 0.493677;
	      
	      float muon_sigma = 0.0000000001;
	      float kaon_sigma = 0.000016;
	      
	      //initial chi2 and ndf before kinematic fits. The chi2 of the reconstruction is not considered
	      float chi = 0.;
	      float ndf = 0.;	    
	      
	      //making particles with the fitter
	      vector<RefCountedKinematicParticle> allParticles;
	      allParticles.push_back(pFactory.particle (track_MuP,muon_mass,chi,ndf,muon_sigma));
	      allParticles.push_back(pFactory.particle (track_MuM,muon_mass,chi,ndf,muon_sigma));	    
	      allParticles.push_back(pFactory.particle (track_Kp,kaon_mass,chi,ndf,kaon_sigma));
	      allParticles.push_back(pFactory.particle (track_Km,kaon_mass,chi,ndf,kaon_sigma));	      

	      //creating the constraint for the J/Psi mass
	      ParticleMass m_jpsi = 3.09687;
	      //creating the two track mass constraint
 	      MultiTrackKinematicConstraint *  j_psi_c = new  TwoTrackMassKinematicConstraint(m_jpsi);
	      //creating the fitter (global fit)
	      KinematicConstrainedVertexFitter kcvFitter;
	      //obtaining the resulting tree
	      RefCountedKinematicTree myTree_Bs = kcvFitter.fit(allParticles, j_psi_c);
	      
	      myTree_Bs->movePointerToTheTop();
	      RefCountedKinematicParticle bs = myTree_Bs->currentParticle();
	      RefCountedKinematicVertex bVertex = myTree_Bs->currentDecayVertex();
	      
	      if (!bVertex->vertexIsValid()) continue;
	      AlgebraicVector7 b_par = bs->currentState().kinematicParameters().vector();
	      AlgebraicSymMatrix77 bs_er = bs->currentState().kinematicParametersError().matrix(); 
	      double vtxprob_Bs = TMath::Prob(bs->chiSquared(), (int)bs->degreesOfFreedom());
	      
	      double minP = -99.;
	      if (vtxprob_Bs > minP){
		TrkCounter1=i;
		TrkCounter2=j;
		minP = vtxprob_Bs;

		if (abs(JpsiCand.mass()-3.097)>0.1 && JpsiCand.pt()<3) continue;
		if (abs(PhiCand.mass()-1.019)>0.02) continue;
		if (BCand.mass()<5.2 || BCand.mass()>5.7) continue;
		
		chi2_Bs = bs->chiSquared();
		ndof_Bs =(int)bs->degreesOfFreedom();
		BsVtxProb = vtxprob_Bs;
		BsM = BCand.mass();
		BsPt = BCand.pt();
		BsPhi = BCand.phi();
		BsEta = BCand.eta();
		BmassC = b_par[6];		

		/*
		bool child = myTree_Bs->movePointerToTheFirstChild();
		if (child){
		  RefCountedKinematicParticle jpsi = myTree_Bs->currentParticle();		
		  AlgebraicVector7 jpsi_par = jpsi->currentState().kinematicParameters().vector();
		  JpsiM = jpsi_par[6];
		}
		
		bool nchild = myTree_Bs->movePointerToTheNextChild();
		if (nchild){
		  RefCountedKinematicParticle phi = myTree_Bs->currentParticle();		
		  AlgebraicVector7 phi_par = phi->currentState().kinematicParameters().vector();
		  PhiM = phi_par[6];
		}
		*/

		JpsiM = JpsiCand.mass();
		JpsiPhi = JpsiCand.phi();	  
		JpsiEta = JpsiCand.phi();	  
		JpsiPt = JpsiCand.pt();	  
		PhiM = PhiCand.mass();
		PhiPhi = PhiCand.phi();	  
		PhiEta = PhiCand.phi();	  
		PhiPt = PhiCand.pt();	  

		bsRootTree_->getJpsi(JpsiM,JpsiPhi,JpsiEta,JpsiPt);
		bsRootTree_->getPhi(PhiM,PhiPhi,PhiEta,PhiPt);
		bsRootTree_->getBs(chi2_Bs,ndof_Bs,BsVtxProb,BsM,BsPhi,BsEta,BsPt,BmassC);
		
		K1Pt=track1.pt();
		K2Pt=track2.pt();
		K1Eta=track1.eta();
		K2Eta=track2.eta();
		K1Phi=track1.phi();
		K2Phi=track2.phi();
		K1Chi2=trk1Ref.get()->normalizedChi2();
		K1nHits=trk1Ref.get()->numberOfValidHits();
		K2Chi2=trk2Ref.get()->normalizedChi2();
		K2nHits=trk2Ref.get()->numberOfValidHits();
		
		Mu1Pt=mu1jpsi.pt();
		Mu2Pt=mu2jpsi.pt();
		Mu1Eta=mu1jpsi.eta();
		Mu2Eta=mu2jpsi.eta();
		Mu1Phi=mu1jpsi.phi();
		Mu2Phi=mu2jpsi.phi();
		Mu1Chi2=Mu1Ref.get()->normalizedChi2();
		Mu1nHits=Mu1Ref.get()->numberOfValidHits();
		Mu2Chi2=Mu2Ref.get()->normalizedChi2();
		Mu2nHits=Mu2Ref.get()->numberOfValidHits();
		
		//////////////////////////
		// vertexing
		//////////////////////////

		if(BCand.pt()!=0) 
		  BLxy=((bVertex->position().x()-BSx)*BCand.px()+(bVertex->position().y()-BSy)*BCand.py())/BCand.pt();
		if(BCand.pt()!=0) 
		  BLxy2=((bVertex->position().x()-PVx)*BCand.px()+(bVertex->position().y()-PVy)*BCand.py())/BCand.pt();
		
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
		// angular analysis
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
		K1pixH=pixhits1;
		// count the number of valid tracker *** hits ***
		K1trkH=p.numberOfValidTrackerHits();
		// count the number of tracker *** layers *** with measurement
		K1trkLay=p.trackerLayersWithMeasurement();
		K1muDTh=p.numberOfValidMuonDTHits();      // not-null, valid, muon DT
		K1muCSCh=p.numberOfValidMuonCSCHits();    // not-null, valid, muon CSC
		K1muRPCh=p.numberOfValidMuonRPCHits();     // not-null, valid, muon RPC
		
		int pixhits2=0;
		const reco::HitPattern& p2 = trk2Ref.get()->hitPattern();
		for (int i=0; i<p2.numberOfHits(); i++) {
		  uint32_t hit = p2.getHitPattern(i);
		  if (p2.validHitFilter(hit) && p2.pixelBarrelHitFilter(hit)) pixhits2++;
		  if (p2.validHitFilter(hit) && p2.pixelEndcapHitFilter(hit)) pixhits2++;
		}
		K2pixH=pixhits2;
		K2trkH=p2.numberOfValidTrackerHits();
                K2trkLay=p2.trackerLayersWithMeasurement();
                K2muDTh=p2.numberOfValidMuonDTHits();      // not-null, valid, muon DT
                K2muCSCh=p2.numberOfValidMuonCSCHits();    // not-null, valid, muon CSC
                K2muRPCh=p2.numberOfValidMuonRPCHits();     // not-null, valid, muon RPC


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
		Mu1pixH=pixhits3;
		Mu1trkH=p3.numberOfValidTrackerHits();
                Mu1trkLay=p3.trackerLayersWithMeasurement();
                Mu1muDTh=p3.numberOfValidMuonDTHits();      // not-null, valid, muon DT
                Mu1muCSCh=p3.numberOfValidMuonCSCHits();    // not-null, valid, muon CSC
                Mu1muRPCh=p3.numberOfValidMuonRPCHits();     // not-null, valid, muon RPC

		int pixhits4=0;
		const reco::HitPattern& p4 = Mu2Ref.get()->hitPattern();
		for (int i=0; i<p4.numberOfHits(); i++) {
		  uint32_t hit = p4.getHitPattern(i);
		  if (p4.validHitFilter(hit) && p4.pixelBarrelHitFilter(hit)) pixhits4++;
		  if (p4.validHitFilter(hit) && p4.pixelEndcapHitFilter(hit)) pixhits4++;
		}
		Mu2pixH=pixhits4;
		Mu2trkH=p4.numberOfValidTrackerHits();
                Mu2trkLay=p4.trackerLayersWithMeasurement();
                Mu2muDTh=p4.numberOfValidMuonDTHits();      // not-null, valid, muon DT
                Mu2muCSCh=p4.numberOfValidMuonCSCHits();    // not-null, valid, muon CSC
                Mu2muRPCh=p4.numberOfValidMuonRPCHits();     // not-null, valid, muon RPC

		
		bsRootTree_->getK1(K1Pt,K1Eta,K1Phi,K1Chi2,K1nHits,K1pixH,K1trkH,K1trkLay,K1muDTh,K1muCSCh,K1muRPCh);
		bsRootTree_->getK2(K2Pt,K2Eta,K2Phi,K2Chi2,K2nHits,K2pixH,K2trkH,K2trkLay,K2muDTh,K2muCSCh,K2muRPCh);
		bsRootTree_->getMu1(Mu1Pt,Mu1Eta,Mu1Phi,Mu1Chi2,Mu1nHits,Mu1pixH,Mu1trkH,Mu1trkLay,Mu1muDTh,Mu1muCSCh,Mu1muRPCh);
		bsRootTree_->getMu2(Mu2Pt,Mu2Eta,Mu2Phi,Mu2Chi2,Mu2nHits,Mu2pixH,Mu2trkH,Mu2trkLay,Mu2muDTh,Mu2muCSCh,Mu2muRPCh);
	      }
	      
	      // deltaR matching!
	      
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

	      // jpsi candidate

	      CompositeCandidate JpsiCand;
	      JpsiCand.addDaughter(mu1jpsi);
	      JpsiCand.addDaughter(mu2jpsi);
	      AddFourMomenta sum;
	      sum.set(JpsiCand);

	      if (abs(JpsiCand.mass()-3.097)>0.15) continue;	      

	      // check on the overlap
	      
	      OverlapChecker overlap;
	      if (overlap(mu1jpsi,track1)!=0 || overlap(mu2jpsi,track1)!=0) continue;
	      if (overlap(mu1jpsi,track2)!=0 || overlap(mu2jpsi,track2)!=0) continue;

	      // B candidate

	      CompositeCandidate BdCand;
	      BdCand.addDaughter(mu1jpsi);
	      BdCand.addDaughter(mu2jpsi);
	      BdCand.addDaughter(track1);
	      BdCand.addDaughter(track2);
	      AddFourMomenta add4mom;
	      add4mom.set(BdCand);

	      if (BdCand.mass() < 4.5 || BdCand.mass() > 6.) continue;

	      // fitting
	      
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

	      vector<RefCountedKinematicParticle> allParticles;
	      allParticles.push_back(pFactory.particle (ttMuP,muon_mass,chi,ndf,muon_sigma));
	      allParticles.push_back(pFactory.particle (ttMuM,muon_mass,chi,ndf,muon_sigma));
	      if(K1flag==1) {
		allParticles.push_back(pFactory.particle (ttK,kaon_mass,chi,ndf,kaon_sigma));
		allParticles.push_back(pFactory.particle (ttP,pi_mass,chi,ndf,pi_sigma));
	      }
	      else{
		allParticles.push_back(pFactory.particle (ttP,kaon_mass,chi,ndf,kaon_sigma));
		allParticles.push_back(pFactory.particle (ttK,pi_mass,chi,ndf,pi_sigma));
	      }

	      ParticleMass jpsi = 3.09687;
	      MultiTrackKinematicConstraint *  j_psi_c = new  TwoTrackMassKinematicConstraint(jpsi);
	      KinematicConstrainedVertexFitter kcvFitter;
	      RefCountedKinematicTree myTree = kcvFitter.fit(allParticles, j_psi_c);

	      myTree->movePointerToTheTop();
	      RefCountedKinematicParticle bmes = myTree->currentParticle();
	      RefCountedKinematicVertex b_dec_vertex = myTree->currentDecayVertex();

	      if(!b_dec_vertex->vertexIsValid()) continue;
	      AlgebraicVector7 b_par = bmes->currentState().kinematicParameters().vector();
	      AlgebraicSymMatrix77 bd_er = bmes->currentState().kinematicParametersError().matrix();
	      double vtxProb = TMath::Prob(bmes->chiSquared(),(int)bmes->degreesOfFreedom());
	      
	      double MinBVtx = -99.;
	      
	      if(vtxProb>MinBVtx){
		MinBVtx=vtxProb;
		if(K1flag==1){
		  BdM = sqrt((KaonE1+trk2.energy()+mu1jpsi.energy()+mu2jpsi.energy())*(KaonE1+trk2.energy()+mu1jpsi.energy()+mu2jpsi.energy())
				-(trk1.px()+trk2.px()+mu1jpsi.px()+mu2jpsi.px())*(trk1.px()+trk2.px()+mu1jpsi.px()+mu2jpsi.px())
				-(trk1.py()+trk2.py()+mu1jpsi.py()+mu2jpsi.py())*(trk1.py()+trk2.py()+mu1jpsi.py()+mu2jpsi.py())
				-(trk1.pz()+trk2.pz()+mu1jpsi.pz()+mu2jpsi.pz())*(trk1.pz()+trk2.pz()+mu1jpsi.pz()+mu2jpsi.pz()));
		  KstarM = Kstmass1;
		}
		else if(K2flag==1){
		  BdM = sqrt((KaonE2+trk1.energy()+mu1jpsi.energy()+mu2jpsi.energy())*(KaonE2+trk1.energy()+mu1jpsi.energy()+mu2jpsi.energy())
				-(trk1.px()+trk2.px()+mu1jpsi.px()+mu2jpsi.px())*(trk1.px()+trk2.px()+mu1jpsi.px()+mu2jpsi.px())
				-(trk1.py()+trk2.py()+mu1jpsi.py()+mu2jpsi.py())*(trk1.py()+trk2.py()+mu1jpsi.py()+mu2jpsi.py())
				-(trk1.pz()+trk2.pz()+mu1jpsi.pz()+mu2jpsi.pz())*(trk1.pz()+trk2.pz()+mu1jpsi.pz()+mu2jpsi.pz()));
		  KstarM = Kstmass2;
		}
		BdmassC = b_par[6];  

		BdVtxProb = vtxProb;
		BdPhi = BdCand.phi();
		BdPt = BdCand.pt();  
		BdEta = BdCand.eta();  
		JpsiM_Bd = JpsiCand.mass(); 
		
		/*
		if(BdCand.pt()!=0) BdLxy=((b_dec_vertex->position().x()-BSx)*BdCand.px()+
					  (b_dec_vertex->position().y()-BSy)*BdCand.py())/BdCand.pt();
		if(BdCand.pt()!=0) BdLxy2=((b_dec_vertex->position().x()-PVx)*BdCand.px()+
					   (b_dec_vertex->position().y()-PVy)*BdCand.py())/BdCand.pt();

		  Bdct1 = BdLxy*BdCand.mass()/BdCand.pt();
		  Bdct12 = BdLxy2*BdCand.mass()/BdCand.pt();
		  
		Bderrx2=bd_er(1,1);
		Bderry2=bd_er(2,2);
		Bderrxy2=bd_er(1,2); 
		*/

		// set cos of angle between bs momentum and decay length
		AngleBdDecayLength = ((b_dec_vertex->position().x()-PVx) * BdCand.px() + (b_dec_vertex->position().y()-PVy) * BdCand.py() + (b_dec_vertex->position().z()-PVz) * BdCand.pz()) / sqrt(((b_dec_vertex->position().x()-PVx) * (b_dec_vertex->position().x()-PVx) + (b_dec_vertex->position().y()-PVy) * (b_dec_vertex->position().y()-PVy) + (b_dec_vertex->position().z()-PVz) * (b_dec_vertex->position().z()-PVz)) * (BdCand.px()*BdCand.px() + BdCand.py()*BdCand.py() + BdCand.pz()*BdCand.pz()));

		bsRootTree_->getBdPar(BdM,BdmassC,KstarM,BdVtxProb,BdPhi,BdEta,BdPt,JpsiM_Bd,AngleBdDecayLength);

	      }
	    } // end trk1
	  } // end trk2
	} // end muon loop
	
      } // end muCounter1 and muCounter2 if statement
    } // loop on muons
    bsRootTree_->fill();
  } catch (std::exception & err) {
    cout
      << "Exception during event number: " << iEvent.id()
      << "\n" << err.what() << "\n";
  }
}


int BsToJpsiPhiAnalysis::JpsiKstarFlag(const edm::Event &iEvent)
{
    Handle<GenParticleCollection> genParticles;
    iEvent.getByLabel(thegenParticlesLabel_ , genParticles );

    flagKstar = 0;
    for( size_t i = 0; i < genParticles->size(); ++ i ) {
      const Candidate & p = (*genParticles)[ i ];
      int MC_particleID=p.pdgId();
      if (abs(MC_particleID) == 443 && p.status()==2){
	// Bd = 511/-511
	const Candidate & gmo=*(p.mother());
	int nchildrenBs=gmo.numberOfDaughters();
	int MC_momID=gmo.pdgId();
	// Jpsi = 443/-443	
	const Candidate & da1=*(gmo.daughter( 0 ));
	int nchildrenJpsi = da1.numberOfDaughters();
	int MC_dauJpsi=da1.pdgId();
	// kstar = 313/-313	
	const Candidate & da2=*(gmo.daughter( 1 ));
	int nchildrenKstar=da2.numberOfDaughters();
	int MC_dauKstar=da2.pdgId();
	// muons = 13/-13
	const Candidate & gda1=*(da1.daughter( 0 ));
	const Candidate & gda2=*(da1.daughter( 1 ));
	// k/pi = 321/-211 || -321/211
	const Candidate & gda3=*(da2.daughter( 0 ));
	const Candidate & gda4=*(da2.daughter( 1 ));
	// flag!
	if(nchildrenBs == 2 && abs(MC_momID) == 511 && 
	   nchildrenJpsi == 2 && abs(MC_dauJpsi) == 443 &&
	   nchildrenKstar == 2 && abs(MC_dauKstar) == 313  &&					       
	   abs(gda1.pdgId()) == 13 && abs(gda2.pdgId()) == 13 &&
	   abs(gda3.pdgId()) == 321 && abs(gda4.pdgId()) == 211) { 
	  flagKstar = 1;	  
	  return flagKstar;
	}
      }
    }
    return 0;
}

int BsToJpsiPhiAnalysis::JpsiKsFlag(const edm::Event &iEvent)
{
    Handle<GenParticleCollection> genParticles;
    iEvent.getByLabel(thegenParticlesLabel_ , genParticles );

    flagKs = 0;
    for( size_t i = 0; i < genParticles->size(); ++ i ) {
      const Candidate & p = (*genParticles)[ i ];
      int MC_particleID=p.pdgId();
      if (abs(MC_particleID) == 443 && p.status()==2){
	// Bd = 511/-511
	const Candidate & gmo=*(p.mother());
	int nchildrenBd=gmo.numberOfDaughters();
	int MC_momID=gmo.pdgId();
	// Jpsi = 443/-443	
	const Candidate & da1=*(gmo.daughter( 0 ));
	int nchildrenJpsi = da1.numberOfDaughters();
	int MC_dauJpsi=da1.pdgId();
	// Ks = 310/-310	
	const Candidate & da2=*(gmo.daughter( 1 ));
	int nchildrenKs=da2.numberOfDaughters();
	int MC_dauKs=da2.pdgId();
	// muons = 13/-13
	const Candidate & gda1=*(da1.daughter( 0 ));
	const Candidate & gda2=*(da1.daughter( 1 ));
	// pions = 211/-211
	const Candidate & gda3=*(da2.daughter( 0 ));
	const Candidate & gda4=*(da2.daughter( 1 ));
	// flag!
	if(nchildrenBd == 2 && abs(MC_momID) == 511 && 
	   nchildrenJpsi == 2 && abs(MC_dauJpsi) == 443 &&
	   nchildrenKs == 2 && abs(MC_dauKs) == 310 &&					       
	   abs(gda1.pdgId()) == 13 && abs(gda2.pdgId()) == 13 &&
	   abs(gda3.pdgId()) == 211 && abs(gda4.pdgId()) == 211) { 
	  flagKs = 1;	  
	  return flagKs;
	}
      }
    }
    return 0;
}

int BsToJpsiPhiAnalysis::JpsiPhiFlag(const edm::Event &iEvent)
{
    Handle<GenParticleCollection> genParticles;
    iEvent.getByLabel(thegenParticlesLabel_ , genParticles );

    flagPhi = 0;
    for( size_t i = 0; i < genParticles->size(); ++ i ) {
      const Candidate & p = (*genParticles)[ i ];
      int MC_particleID=p.pdgId();
      if (abs(MC_particleID) == 443 && p.status()==2){
	// Bs = 531/-531
	const Candidate & gmo=*(p.mother());
	int nchildrenBs=gmo.numberOfDaughters();
	int MC_momID=gmo.pdgId();
	// Jpsi = 443/-443	
	const Candidate & da1=*(gmo.daughter( 0 ));
	int nchildrenJpsi = da1.numberOfDaughters();
	int MC_dauJpsi=da1.pdgId();
	// phi = 333/-333	
	const Candidate & da2=*(gmo.daughter( 1 ));
	int nchildrenPhi=da2.numberOfDaughters();
	int MC_dauPhi=da2.pdgId();
	// muons = 13/-13
	const Candidate & gda1=*(da1.daughter( 0 ));
	const Candidate & gda2=*(da1.daughter( 1 ));
	// kaons = 321/-321
	const Candidate & gda3=*(da2.daughter( 0 ));
	const Candidate & gda4=*(da2.daughter( 1 ));
	// flag!
	if(nchildrenBs == 2 && abs(MC_momID) == 531 && 
	   nchildrenJpsi == 2 && abs(MC_dauJpsi) == 443 &&
	   nchildrenPhi == 2 && abs(MC_dauPhi) == 333 &&					       
	   abs(gda1.pdgId()) == 13 && abs(gda2.pdgId()) == 13 &&
	   abs(gda3.pdgId()) == 321 && abs(gda4.pdgId()) == 321) { 
	  flagPhi = 1;	  
	  return flagPhi;
	}
      }
    }
    return 0;
}


//  LocalWords:  pkminus
