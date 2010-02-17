// description on https://twiki.cern.ch/twiki/bin/viewauth/CMS/BsJpsiPhi_AWG
#include "HeavyFlavorAnalysis/BsToJpsiPhi/interface/BsToJpsiPhiAnalysis.h"
#include "DataFormats/Common/interface/EDProduct.h"
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
#include "DataFormats/Math/interface/deltaR.h"
#include "HeavyFlavorAnalysis/BsToJpsiPhi/interface/KinematicFitInterface.h"
#include "RecoVertex/PrimaryVertexProducer/interface/PrimaryVertexProducerAlgorithm.h"

#include <iostream>
#include <TMath.h>

using namespace reco;
using namespace edm;
using namespace std;

BsToJpsiPhiAnalysis::BsToJpsiPhiAnalysis(const edm::ParameterSet& iConfig) : theConfig_(iConfig), 
									     nominalJpsiMass( 3.096916 ),
									     nominalPhiMass(1.019 ),
									     nominalMuonMass(0.1056583),
									     nominalKaonMass(0.493677),
									     nominalPionMass(0.139570),
									     nominalKstarMass(0.892)
{

  thegenParticlesLabel_ = iConfig.getParameter<InputTag>("genParticlesLabel");
  trackLabelK_ = iConfig.getParameter<edm::InputTag>("TrackLabel_K");
  trackLabelPi_ = iConfig.getParameter<edm::InputTag>("TrackLabel_pi");
  triggerTag_ = iConfig.getParameter<edm::InputTag>("TriggerTag");
  muonTag_ = iConfig.getParameter<edm::InputTag>("MuonTag");
  StoreDeDxInfo_ = iConfig.getParameter<bool>("StoreDeDxInfo");
  saveL1DoubleMuOpenTriggeredOnly_ = iConfig.getParameter<bool>("saveL1DoubleMuOpenTriggeredOnly");
  JpsiMassWindowBeforeFit_ = iConfig.getParameter<double>("JpsiMassWindowBeforeFit");

  BsLowerMassCutBeforeFit_  = iConfig.getParameter<double>("BsLowerMassCutBeforeFit");
  BsUpperMassCutBeforeFit_  = iConfig.getParameter<double>("BsUpperMassCutBeforeFit");
  BsLowerMassCutAfterFit_  = iConfig.getParameter<double>("BsLowerMassCutAfterFit");
  BsUpperMassCutAfterFit_  = iConfig.getParameter<double>("BsUpperMassCutAfterFit");

  JpsiMassWindowAfterFit_ = iConfig.getParameter<double>("JpsiMassWindowAfterFit");
  JpsiPtCut_ =  iConfig.getParameter<double>("JpsiPtCut");
  KaonTrackPtCut_ = iConfig.getParameter<double>("KaonTrackPtCut");
  BdKaonTrackPtCut_ = iConfig.getParameter<double>("BdKaonTrackPtCut");
  PhiMassWindowBeforeFit_ = iConfig.getParameter<double>("PhiMassWindowBeforeFit");
  PhiMassWindowAfterFit_ = iConfig.getParameter<double>("PhiMassWindowAfterFit");

  KstarMassWindowBeforeFit_ = iConfig.getParameter<double>("KstarMassWindowBeforeFit");
  KstarMassWindowAfterFit_ = iConfig.getParameter<double>("KstarMassWindowAfterFit");
  BdLowerMassCutBeforeFit_ = iConfig.getParameter<double>("BdLowerMassCutBeforeFit");
  BdUpperMassCutBeforeFit_ = iConfig.getParameter<double>("BdUpperMassCutBeforeFit");

  BdLowerMassCutAfterFit_ = iConfig.getParameter<double>("BdLowerMassCutAfterFit");
  BdUpperMassCutAfterFit_ = iConfig.getParameter<double>("BdUpperMassCutAfterFit");

  verbose_                = iConfig.getParameter<bool>("verbose");
  outputFile_ = iConfig.getUntrackedParameter<std::string>("outputFile");
  event_counter_ = 0;

  edm::LogInfo("RecoVertex/BsToJpsiPhiAnalysis")<< "Initializing Bs to Jpsi Phi analyser  - Output file: " << outputFile_ <<"\n";


 

}

BsToJpsiPhiAnalysis::~BsToJpsiPhiAnalysis() {}

void BsToJpsiPhiAnalysis::beginJob(edm::EventSetup const& setup)
{
  bsRootTree_ = new BsToJpsiPhiRootTree();
  bsRootTree_->createTree(outputFile_);

}

void BsToJpsiPhiAnalysis::endJob() 
{
  bsRootTree_->writeFile();
  delete bsRootTree_;
  cout << "Total number of Events: " << event_counter_ << endl;
}

void
BsToJpsiPhiAnalysis::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  event_counter_++;

  bsRootTree_->resetEntries();
  

  /////////////////////////////////
  // Get primary vertices  
  /////////////////////////////////
  
  int VtxIndex=-99;
  double minVtxProb=-999.;
  double MinBVtxHyp1=-999.;
  
  double BSx;
  double BSy;
  double BSz;
  double PVx;
  double PVy;
  double PVz;
  double PVerrx;
  double PVerry;
  double PVerrz;

  BSx=BSy=BSz=PVx=PVy=PVz=PVerrx=PVerry=PVerrz=-9999999;

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
  
  if(verbose_ == true){
    std::cout<<"Beam Spot (x,y,z) = ("<< BSx << ", "<< BSy << ", "<< BSz 
	     << ")  Primary Vtx = (" << PVx <<" ," << PVy << " ," <<PVz<< ")"<< std::endl;  
  }

  /////////////////////////////////    
  // MC info 
  /////////////////////////////////
  
  edm::Handle<GenParticleCollection> genParticles;
  
  if(thegenParticlesLabel_.label() != "" ){
    iEvent.getByLabel(thegenParticlesLabel_ , genParticles );
    
    // call function which does the job: filling MC info
    fillMCInfo(genParticles);
  }
  
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
    if (trigName=="HLT_DoubleMu0")     bsRootTree_->triggerbit_HLTdoubleMu0_      = hltresults->accept(itrig);
    if (trigName=="HLT_DoubleMu3_JPsi")bsRootTree_->triggerbit_HLTdoubleMu3_JPsi_ = hltresults->accept(itrig);
    if (trigName=="HLT_L1DoubleMuOpen")bsRootTree_->triggerbit_HLTL1DoubleMuOpen_ = hltresults->accept(itrig);
    if (trigName=="HLT_L1MuOpen")bsRootTree_->triggerbit_HLTL1MuOpen_ = hltresults->accept(itrig);
  }
  
  // continue only if it is a  triggered event
  if(saveL1DoubleMuOpenTriggeredOnly_ == true)
    if(bsRootTree_->triggerbit_HLTL1DoubleMuOpen_ != 1 ) return;


  /////////////////////////
  // dE/dx info
  /////////////////////////
  Handle<DeDxDataValueMap> energyLossHandle;
  if(StoreDeDxInfo_)  iEvent.getByLabel("dedxHarmonic2", energyLossHandle);
  
  
 
  // **************************
  // start loop over muons
  
  edm::Handle<reco::MuonCollection> allmuons;
  iEvent.getByLabel(muonTag_,allmuons);
  
  if(verbose_==true){
    if(allmuons->size()>0){
      std::cout<<"******found number of muons = "<< allmuons->size() << std::endl;
    }
  }
  
  // variables to determine minima of fit probability
  double minVtxP = -99.;   //KK hypothesis
  double minJpsiP = -99;   // jpsi alone
  
  // loop on muons
  for(size_t i=0; i < allmuons->size(); ++i){
    const Muon & mu1 = (*allmuons)[i];
    if(!mu1.isGlobalMuon() && !mu1.isTrackerMuon()) continue;
    for (size_t j=i+1; j < allmuons->size(); ++j){
      const Muon & mu2 = (*allmuons)[j];
      if(!mu2.isGlobalMuon() && !mu2.isTrackerMuon()) continue;
      
      if(verbose_==true){
	std::cout<<"******mu1.isGlobalMuon() == " <<mu1.isGlobalMuon()<<"  mu1.isTrackerMuon() == " <<mu1.isTrackerMuon()
		 <<"  mu2.isGlobalMuon() == " <<mu2.isGlobalMuon()<<"  mu2.isTrackerMuon() == " <<mu2.isTrackerMuon()<<std::endl;
      }

      if(mu1.charge()==mu2.charge()) continue;
      if(verbose_==true){
	std::cout<<"******MUONS HAVE OPPOSITE CHARGE: mu1.charge() = " << mu1.charge()
		 <<"  mu2.charge() = " << mu2.charge() << std::endl;
      }

      if(!muon::isGoodMuon(mu1, muon::TM2DCompatibilityTight)) continue;
      if(!muon::isGoodMuon(mu2, muon::TM2DCompatibilityTight)) continue;
      
      // passed opposite sign cut
      if(bsRootTree_->iPassedCutIdent_   < 1 ) bsRootTree_->iPassedCutIdent_ = 1 ;
      if(bsRootTree_->iPassedCutIdentBd_   < 1 ) bsRootTree_->iPassedCutIdentBd_ = 1 ;
      
      CompositeCandidate Jpsi;
      Jpsi.addDaughter(mu1);
      Jpsi.addDaughter(mu2);
      AddFourMomenta addP4;
      addP4.set(Jpsi);
      
      if(verbose_==true){
	std::cout<<"******Di-Muon Mass = " << Jpsi.mass() << std::endl;
      }

      if ( abs(Jpsi.mass()- nominalJpsiMass ) > JpsiMassWindowBeforeFit_ || Jpsi.pt() < JpsiPtCut_) continue;
      // passed jpsi mass and pt cut
      if(bsRootTree_->iPassedCutIdent_   < 2 ) bsRootTree_->iPassedCutIdent_ = 2 ;
      if(bsRootTree_->iPassedCutIdentBd_   < 2 ) bsRootTree_->iPassedCutIdentBd_ = 2 ;
      
      edm::ESHandle<TransientTrackBuilder> theB;
      iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theB);	
      TrackRef trkMu1Ref = mu1.get<TrackRef>();
      TrackRef trkMu2Ref = mu2.get<TrackRef>();
      
      vector<TransientTrack> trk_all;
      trk_all.push_back((*theB).build(&trkMu1Ref));
      trk_all.push_back((*theB).build(&trkMu2Ref));
      
      KalmanVertexFitter kvf;
      TransientVertex tv = kvf.vertex(trk_all);
      
      if (!tv.isValid()) continue; 
      if(verbose_==true){
	std::cout<<"****** MUONS HAVE VALID VERTEX FIT"<< std::endl;
      }
      // valid jpsi vertex
      if(bsRootTree_->iPassedCutIdent_   < 3 ) bsRootTree_->iPassedCutIdent_ = 3 ;
      if(bsRootTree_->iPassedCutIdentBd_   < 3 ) bsRootTree_->iPassedCutIdentBd_ = 3 ;
      
      Vertex vertex = tv;
      double vtxProb_Jpsi = TMath::Prob(vertex.chi2(),(int)vertex.ndof()); 
      
      // fill the Jpsi with highest vertex probability in the tree
      if(vtxProb_Jpsi > minJpsiP){
	minJpsiP = vtxProb_Jpsi;
	
	bsRootTree_->JpsiVtxProb_ = vtxProb_Jpsi;
	bsRootTree_->JpsiM_alone_ = Jpsi.mass();	  
	bsRootTree_->JpsiPhi_alone_ = Jpsi.phi();	  
	bsRootTree_->JpsiEta_alone_ = Jpsi.eta();	  
	bsRootTree_->JpsiPt_alone_ = Jpsi.pt();	  
	bsRootTree_->JpsiMu1Pt_alone_ = mu1.pt();	  
	bsRootTree_->JpsiMu2Pt_alone_ = mu2.pt();	  
	bsRootTree_->JpsiMu1Phi_alone_ = mu1.phi();	  
	bsRootTree_->JpsiMu2Phi_alone_ = mu2.phi();	  
	bsRootTree_->JpsiMu1Eta_alone_ = mu1.eta();	  
	bsRootTree_->JpsiMu2Eta_alone_ = mu2.eta();	  
	
	// muon categories:
	// 1: tracker muons
	// 2: global muons
	// 3: global + tracker muon
	// 4: neither tracker nor global
	
	if (mu1.isTrackerMuon() && !mu1.isGlobalMuon())       bsRootTree_->JpsiMuon1Cat_alone_ = 1;
	else if (!mu1.isTrackerMuon() && mu1.isGlobalMuon())  bsRootTree_->JpsiMuon1Cat_alone_ = 2;
	else if (mu1.isTrackerMuon() && mu1.isGlobalMuon())   bsRootTree_->JpsiMuon1Cat_alone_ = 3;
	else if (!mu1.isTrackerMuon() && !mu1.isGlobalMuon()) bsRootTree_->JpsiMuon1Cat_alone_ = 4;
	
	if (mu2.isTrackerMuon() && !mu2.isGlobalMuon())       bsRootTree_->JpsiMuon2Cat_alone_ = 1;
	else if (!mu2.isTrackerMuon() && mu2.isGlobalMuon())  bsRootTree_->JpsiMuon2Cat_alone_ = 2;
	else if (mu2.isTrackerMuon() && mu2.isGlobalMuon())   bsRootTree_->JpsiMuon2Cat_alone_ = 3;
	else if (!mu2.isTrackerMuon() && !mu2.isGlobalMuon()) bsRootTree_->JpsiMuon2Cat_alone_ = 4;
      }

      ////////////////////////////////////	
      ////////////////////////////////////
      ////           Phi
      ////////////////////////////////////
      ////////////////////////////////////
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
	    
	    // phi candidate
	    CompositeCandidate PhiCand;
	    PhiCand.addDaughter(track1);
	    PhiCand.addDaughter(track2);
	    AddFourMomenta ad;
	    ad.set(PhiCand);
	    
	    if (abs(PhiCand.mass()- nominalPhiMass) > PhiMassWindowBeforeFit_) continue;
	    // passed phi mass window before fit
	    if(bsRootTree_->iPassedCutIdent_   < 5 ) bsRootTree_->iPassedCutIdent_ = 5 ;
	    

	    
	    // check on the overlap
	    OverlapChecker overlap;
	    if (overlap(mu1,track1)!=0 || overlap(mu2,track1)!=0) continue;
	    if (overlap(mu1,track2)!=0 || overlap(mu2,track2)!=0) continue;
	    
	    // passed muon - track overlap check
	    if(bsRootTree_->iPassedCutIdent_   < 7 ) bsRootTree_->iPassedCutIdent_ = 7 ;
	    
	    
	    // B candidate
	    
	    CompositeCandidate BCand;
	    BCand.addDaughter(mu1);
	    BCand.addDaughter(mu2);
	    BCand.addDaughter(track1);
	    BCand.addDaughter(track2);
	    AddFourMomenta add4mom;
	    add4mom.set(BCand);
	    
	    if (BCand.mass() < BsLowerMassCutBeforeFit_ || BCand.mass() > BsUpperMassCutBeforeFit_) continue;
	    // passed Bs mass cut before fit
	    if(bsRootTree_->iPassedCutIdent_   < 8 ) bsRootTree_->iPassedCutIdent_ = 8 ;
	    
	    
	    // start fit on the B candidates
	    TrackRef trk1Ref = track1.get<TrackRef>();
	    TrackRef trk2Ref = track2.get<TrackRef>();
	    
	    vector<TransientTrack> t_tracks;
	    t_tracks.push_back((*theB).build(&trkMu1Ref));
	    t_tracks.push_back((*theB).build(&trkMu2Ref));
	    t_tracks.push_back((*theB).build(&trk1Ref));
	    t_tracks.push_back((*theB).build(&trk2Ref));
	    
	    if (!trkMu1Ref.isNonnull() || !trkMu2Ref.isNonnull() || !trk1Ref.isNonnull() || !trk2Ref.isNonnull()) continue;
	    // checked track references
	    if(bsRootTree_->iPassedCutIdent_   < 9 ) bsRootTree_->iPassedCutIdent_ = 9 ;
	    
	    //call fit interface
	    KinematicFitInterface Kfitter;
	    bool fitSuccess = Kfitter.doFit(t_tracks, nominalMuonMass,  nominalKaonMass, nominalKaonMass);
	    
	    if(fitSuccess != 1) continue;
	    
	    if(bsRootTree_->iPassedCutIdent_   < 10 ) bsRootTree_->iPassedCutIdent_ = 10 ;

	    double vtxprob_Bs = Kfitter.getProb();
	    RefCountedKinematicParticle bs = Kfitter.getParticle();
	    RefCountedKinematicVertex bVertex = Kfitter.getVertex();
	    AlgebraicVector7 b_par = bs->currentState().kinematicParameters().vector();
	    AlgebraicSymMatrix77 bs_er = bs->currentState().kinematicParametersError().matrix();

	    double fittedBsMass = b_par[6];
	    
	    // check if it is a valid candidate to be counted (verify passed AfterFit cuts)
	    if (abs(Jpsi.mass() - nominalJpsiMass) < JpsiMassWindowAfterFit_ && Jpsi.pt() > JpsiPtCut_ &&
		abs(PhiCand.mass() - nominalPhiMass) < PhiMassWindowAfterFit_ &&
		fittedBsMass > BsLowerMassCutAfterFit_ && fittedBsMass < BsUpperMassCutAfterFit_  ) bsRootTree_->BsNumberOfCandidates_++;

	    // store values in root tree if vtx probability is higher than already stored one
	    if (vtxprob_Bs > minVtxP){
	     
	      if (abs(Jpsi.mass() - nominalJpsiMass) > JpsiMassWindowAfterFit_ || Jpsi.pt() < JpsiPtCut_) continue;
	      // passed jpsi mass window after fit
	      if(bsRootTree_->iPassedCutIdent_   < 11 ) bsRootTree_->iPassedCutIdent_ = 11 ;
	      
	      if (abs(PhiCand.mass() - nominalPhiMass) > PhiMassWindowAfterFit_) continue;
	      // passed phi mass window after fit
	      if(bsRootTree_->iPassedCutIdent_   < 12 ) bsRootTree_->iPassedCutIdent_ = 12 ;
	      
	      if (fittedBsMass < BsLowerMassCutAfterFit_ || fittedBsMass > BsUpperMassCutAfterFit_) continue;
	      // passed Bs mass window after fit
	      if(bsRootTree_->iPassedCutIdent_   < 13 ) bsRootTree_->iPassedCutIdent_ = 13 ;
	      
	      
	      minVtxP = vtxprob_Bs;

	      reco::Vertex reFitVertex;
	      //recalculate primary vertex without tracks from B
	      reco::Vertex tmpFitVertex = reVertex(recVtxs, iEvent,iSetup, trkMu1Ref, trkMu2Ref, trk1Ref, trk2Ref);
	      if(tmpFitVertex.isValid()) reFitVertex = tmpFitVertex;
	      else reFitVertex = reco::Vertex(RecVtx);   // use the original vertex if the refit vertex is invalid
	      bsRootTree_->PVx_refit_ = reFitVertex.x();
	      bsRootTree_->PVy_refit_ = reFitVertex.y();
	      bsRootTree_->PVz_refit_ = reFitVertex.z();

	      bsRootTree_->PVerrx_refit_ = reFitVertex.xError();
	      bsRootTree_->PVerry_refit_ = reFitVertex.yError();
	      bsRootTree_->PVerrz_refit_ = reFitVertex.zError();
	      
	      ////////////////////////
	      // fill kinematic info to tree
	      //////////////////////////
	      
	      bsRootTree_->BsFitChi2_  = bs->chiSquared();
	      bsRootTree_->BsFitNdof_   =(int)bs->degreesOfFreedom();
	      bsRootTree_->BsFitVtxProb_ = vtxprob_Bs;
	    
	      GlobalVector Bsvec(b_par[3], b_par[4], b_par[5]); // the fitted momentum vector of the Bs
	      bsRootTree_->BsFitM_ = b_par[6];		
	      bsRootTree_->BsFitEta_ = Bsvec.eta();
	      bsRootTree_->BsFitPt_  = Bsvec.perp();
	      bsRootTree_->BsFitPz_  = Bsvec.z();
	      bsRootTree_->BsFitPhi_ = Bsvec.phi();

	      RefCountedKinematicTree reftree = Kfitter.getTree();
	      setFitParKK(reftree);

	      bsRootTree_->BsFitVtx_x_ = bVertex->position().x();
	      bsRootTree_->BsFitVtx_y_ = bVertex->position().y();
	      bsRootTree_->BsFitVtx_z_ = bVertex->position().z();

	      bsRootTree_->BsM_nofit_ = BCand.mass();
	      bsRootTree_->BsPt_nofit_ = BCand.pt();
	      bsRootTree_->BsPz_nofit_ = BCand.pz();
	      bsRootTree_->BsPhi_nofit_ = BCand.phi();
	      bsRootTree_->BsEta_nofit_ = BCand.eta();
	      
	      bsRootTree_->JpsiM_nofit_ = Jpsi.mass();
	      bsRootTree_->JpsiPhi_nofit_ = Jpsi.phi();	  
	      bsRootTree_->JpsiEta_nofit_ = Jpsi.eta();	  
	      bsRootTree_->JpsiPt_nofit_ = Jpsi.pt();	  
	      bsRootTree_->JpsiPz_nofit_ = Jpsi.pz();	  
	      
	      bsRootTree_->PhiM_nofit_ = PhiCand.mass();
	      bsRootTree_->PhiPhi_nofit_ = PhiCand.phi();	  
	      bsRootTree_->PhiEta_nofit_ = PhiCand.eta();	  
	      bsRootTree_->PhiPt_nofit_ = PhiCand.pt();	  
	      bsRootTree_->PhiPz_nofit_ = PhiCand.pz();	  
	      
	      bsRootTree_->K1Pt_nofit_   = track1.pt();
	      bsRootTree_->K1Pz_nofit_   = track1.pz();
	      bsRootTree_->K1Eta_nofit_  = track1.eta();
	      bsRootTree_->K1Phi_nofit_  = track1.phi();
	      bsRootTree_->K1Key_nofit_  = trk1Ref.key();
	      bsRootTree_->K2Pt_nofit_   = track2.pt();
	      bsRootTree_->K2Pz_nofit_   = track2.pz();
	      bsRootTree_->K2Eta_nofit_  = track2.eta();
	      bsRootTree_->K2Phi_nofit_  = track2.phi();
	      bsRootTree_->K2Key_nofit_  = trk2Ref.key();
	      
	      bsRootTree_->K1Chi2_ = trk1Ref.get()->normalizedChi2();
	      bsRootTree_->K1nHits_= trk1Ref.get()->numberOfValidHits();
	      bsRootTree_->K2Chi2_ = trk2Ref.get()->normalizedChi2();
	      bsRootTree_->K2nHits_= trk2Ref.get()->numberOfValidHits();
	      bsRootTree_->Mu1Chi2_ = trkMu1Ref.get()->normalizedChi2();
	      bsRootTree_->Mu1nHits_= trkMu1Ref.get()->numberOfValidHits();
	      bsRootTree_->Mu2Chi2_ = trkMu2Ref.get()->normalizedChi2();
	      bsRootTree_->Mu2nHits_ =trkMu2Ref.get()->numberOfValidHits();
	      
	      // dedx info
	      if(StoreDeDxInfo_){
		const DeDxDataValueMap &  eloss  = *energyLossHandle;
		double dedxTrk = eloss[trk1Ref].dEdx();
		double errdedxTrk = eloss[trk1Ref].dEdxError();
		int NumdedxTrk = eloss[trk1Ref].numberOfMeasurements();
		
		bsRootTree_->getDeDx(dedxTrk,errdedxTrk,NumdedxTrk);
	      }
	      
	      ////////////////////////////////////////////////
	      // proper decay time and proper decay length with the refitted vertex
	      ////////////////////////////////////////////////
	      
	      VertexDistanceXY vdist;	      
	      if(Bsvec.perp()!=0) {
		bsRootTree_->BsLxy_    = vdist.distance( reFitVertex, bVertex->vertexState() ).value(); 
		bsRootTree_->BsLxyErr_ = vdist.distance( reFitVertex, bVertex->vertexState() ).error(); 
		if (  (bVertex->position().x()- reFitVertex.x())*Bsvec.x()+(bVertex->position().y()-reFitVertex.y())*Bsvec.y() < 0  )
		  bsRootTree_->BsLxy_ = -1.0 * bsRootTree_->BsLxy_;   // in case negative sign is necessary 
		bsRootTree_->BsCt_     = bsRootTree_->BsLxy_     *  fittedBsMass/Bsvec.perp();
		bsRootTree_->BsCtErr_  = bsRootTree_->BsLxyErr_  *  fittedBsMass/Bsvec.perp();
	      }
	      bsRootTree_->BsErrX_  = bs_er(1,1);
	      bsRootTree_->BsErrY_  = bs_er(2,2);
	      bsRootTree_->BsErrXY_ = bs_er(1,2); 
	      
	      VertexDistance3D vdist3d;
	      bsRootTree_->BsDist3d_    = vdist3d.distance(bVertex->vertexState(),reFitVertex).value();
	      bsRootTree_->BsDist3dErr_ = vdist3d.distance(bVertex->vertexState(),reFitVertex).error();
	      bsRootTree_->BsTime3d_    = bsRootTree_->BsDist3d_    * fittedBsMass/Bsvec.perp() * 100. /3.;
	      bsRootTree_->BsTime3dErr_ = bsRootTree_->BsDist3dErr_ * BCand.mass()/Bsvec.perp() * 100. /3.;
	      
	 	      
	     
	      bsRootTree_->BsDist2d_     = vdist.distance(bVertex->vertexState(),reFitVertex).value();
	      bsRootTree_->BsDist2dErr_ = vdist.distance(bVertex->vertexState(),reFitVertex).error();
	      bsRootTree_->BsTime2d_     = bsRootTree_->BsDist2d_ * fittedBsMass/Bsvec.perp() *100. /3.;
	      bsRootTree_->BsTime2dErr_  = bsRootTree_->BsDist2dErr_ * fittedBsMass/Bsvec.perp() * 100. /3.;
	      
	     
	   	      
	      ////////////////////////////////////
	      // transversity basis angles
	      ////////////////////////////////////
	      
	      TLorentzVector pbs;
	      pbs.SetXYZM(BCand.px(),BCand.py(),BCand.pz(),BCand.mass());
	      
	      TLorentzVector pmuplus;
	      double mu1mass = sqrt(mu1.energy()*mu1.energy()-
				    mu1.px()*mu1.px()-
				    mu1.py()*mu1.py()-
				    mu1.pz()*mu1.pz());
	      
	      pmuplus.SetXYZM(mu1.px(),mu1.py(),mu1.pz(),mu1mass);
	      
	      TLorentzVector pmuminus;
	      double mu2mass = sqrt(mu2.energy()*mu2.energy()-
				    mu2.px()*mu2.px()-
				    mu2.py()*mu2.py()-
				    mu2.pz()*mu2.pz());
	      pmuminus.SetXYZM(mu2.px(),mu2.py(),mu2.pz(),mu2mass);
	      
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
	      const reco::HitPattern& p3 = trkMu1Ref.get()->hitPattern();
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
	      const reco::HitPattern& p4 = trkMu2Ref.get()->hitPattern();
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
	  

	      // deltaR matching
	      bool K1Truth = MCmatching( track1, genParticles, bsRootTree_->K1mcId_, bsRootTree_->K1momId_, bsRootTree_->K1gmomId_, 333, 531);
	      bool K2Truth = MCmatching( track2, genParticles, bsRootTree_->K2mcId_, bsRootTree_->K2momId_, bsRootTree_->K2gmomId_, 333, 531);
	      bool Mu1Truth= MCmatching( mu1,    genParticles, bsRootTree_->Mu1mcId_,bsRootTree_->Mu1momId_,bsRootTree_->Mu1gmomId_, 443, 531);
	      bool Mu2Truth= MCmatching( mu2,    genParticles, bsRootTree_->Mu2mcId_,bsRootTree_->Mu2momId_,bsRootTree_->Mu2gmomId_, 443, 531);
	      if (K1Truth==1 && K2Truth==1 && Mu1Truth==1 && Mu2Truth==1)  bsRootTree_->isMatched_ = 1;
	      else bsRootTree_->isMatched_ = 0;
	    }
	
	  } // trk2 loop
	} // trk1 loop
 

      ////////////////////////////////////
      ////////////////////////////////////
      ////           Kstar
      ////////////////////////////////////
      ////////////////////////////////////
      
      
      
      Handle<CandidateView> allTracksPi;
      iEvent.getByLabel(trackLabelPi_, allTracksPi);
      
      for (size_t i=0; i< allTracksPi->size(); ++i){
	for (size_t j=i+1; j< allTracksPi->size(); ++j){
	
	  const Candidate & track1 = (*allTracksPi)[i];
	  const Candidate & track2 = (*allTracksPi)[j];
	    if (track1.charge()==track2.charge()) continue;
	    if (track1.pt() < BdKaonTrackPtCut_) continue;
	    if (track2.pt() < BdKaonTrackPtCut_) continue;
	        
	    if(bsRootTree_->iPassedCutIdentBd_   < 4 ) bsRootTree_->iPassedCutIdentBd_ = 4 ;
	        
	    // kstar candidate
	        
	    double KaonMassSq = nominalKaonMass * nominalKaonMass;
	    double KaonE1 = sqrt(KaonMassSq+track1.px()*track1.px()+track1.py()*track1.py()+track1.pz()*track1.pz());
	    double KaonE2 = sqrt(KaonMassSq+track2.px()*track2.px()+track2.py()*track2.py()+track2.pz()*track2.pz());
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
	        
	    if(abs(Kstmass1 - nominalKstarMass) < abs(Kstmass2 - nominalKstarMass)){
	      if(abs(Kstmass1 - nominalKstarMass) > KstarMassWindowBeforeFit_) continue;
	      K1flag=1;
	    } else{
	      if(abs(Kstmass2 - nominalKstarMass) > KstarMassWindowBeforeFit_) continue;
	      K2flag=1;
	    }
	    if(bsRootTree_->iPassedCutIdentBd_   < 5 ) bsRootTree_->iPassedCutIdentBd_ = 5 ;
	        
	  
	    if (abs(Jpsi.mass() - nominalJpsiMass) > JpsiMassWindowBeforeFit_) continue;
	    if(bsRootTree_->iPassedCutIdentBd_   < 6 ) bsRootTree_->iPassedCutIdentBd_ = 6 ;
	        
	    // check on the overlap
	        
	    OverlapChecker overlap;
	    if (overlap(mu1,track1)!=0 || overlap(mu2,track1)!=0) continue;
	    if (overlap(mu1,track2)!=0 || overlap(mu2,track2)!=0) continue;
	        
	    if(bsRootTree_->iPassedCutIdentBd_   < 7 ) bsRootTree_->iPassedCutIdentBd_ = 7 ;

	    // B candidate
	        
	    CompositeCandidate BdCand;
	    BdCand.addDaughter(mu1);
	    BdCand.addDaughter(mu2);
	    BdCand.addDaughter(track1);
	    BdCand.addDaughter(track2);
	    AddFourMomenta add4mom;
	    add4mom.set(BdCand);
	      
	    if (BdCand.mass() < BdLowerMassCutBeforeFit_ || BdCand.mass() > BdUpperMassCutBeforeFit_) continue;
	        
	    if(bsRootTree_->iPassedCutIdentBd_   < 8 ) bsRootTree_->iPassedCutIdentBd_ = 8 ;

	    edm::ESHandle<TransientTrackBuilder> theB;
	    iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theB);
	
	    TrackRef trkkst1 = track1.get<TrackRef>();
	    TrackRef trkkst2 = track2.get<TrackRef>();
	        
	    vector<TransientTrack> t_tks;
	    t_tks.push_back((*theB).build(&trkMu1Ref));
	    t_tks.push_back((*theB).build(&trkMu2Ref));
	    t_tks.push_back((*theB).build(&trkkst1));
	    t_tks.push_back((*theB).build(&trkkst2));
	        
	    if(!trkMu1Ref.isNonnull() || !trkMu2Ref.isNonnull() || !trkkst1.isNonnull() || !trkkst2.isNonnull() ) continue;
	    if(bsRootTree_->iPassedCutIdentBd_   < 9 ) bsRootTree_->iPassedCutIdentBd_ = 9 ;
	        
	    KinematicFitInterface KfitterHyp1;
            bool fitSuccessHyp1 = KfitterHyp1.doFit(t_tks, nominalMuonMass,  nominalKaonMass, nominalPionMass);
	    KinematicFitInterface KfitterHyp2;
            bool fitSuccessHyp2 = KfitterHyp2.doFit(t_tks, nominalMuonMass,  nominalPionMass, nominalKaonMass);
            if(!fitSuccessHyp1 || !fitSuccessHyp2) continue; 
	      
	    if(bsRootTree_->iPassedCutIdentBd_   < 10 ) bsRootTree_->iPassedCutIdentBd_ = 10 ;
	
	    RefCountedKinematicParticle bmesHyp1 = KfitterHyp1.getParticle();
	    RefCountedKinematicVertex bVertexHyp1 = KfitterHyp1.getVertex();
	    AlgebraicVector7 b_parHyp1 = bmesHyp1->currentState().kinematicParameters().vector();
	    AlgebraicSymMatrix77 bd_erHyp1 = bmesHyp1->currentState().kinematicParametersError().matrix();
	    double vtxProbHyp1 = TMath::Prob(bmesHyp1->chiSquared(),(int)bmesHyp1->degreesOfFreedom());

	    RefCountedKinematicParticle bmesHyp2 =  KfitterHyp2.getParticle();
	    RefCountedKinematicVertex bVertexHyp2 = KfitterHyp2.getVertex();
	    AlgebraicVector7 b_parHyp2 = bmesHyp2->currentState().kinematicParameters().vector();
	    AlgebraicSymMatrix77 bd_erHyp2 = bmesHyp2->currentState().kinematicParametersError().matrix();
	    double vtxProbHyp2 = TMath::Prob(bmesHyp2->chiSquared(),(int)bmesHyp2->degreesOfFreedom());

	
	    // 	    // temporary check
	    // 	    if( fabs(vtxProbHyp1 - vtxProbHyp2) > 0.001 ) {
	    // 	      std::cout<<"vtx probs not equal" << std::endl;
	    // 	      exit(1);
	    // 	    }
	    
	    double fittedBdMassHyp1 =  b_parHyp1[6];
	    double fittedBdMassHyp2 =  b_parHyp2[6];
	    
	    if (abs(Jpsi.mass() - nominalJpsiMass) < JpsiMassWindowAfterFit_ && Jpsi.pt() > JpsiPtCut_ &&
		(abs(Kstmass1 - nominalKstarMass)< KstarMassWindowAfterFit_  ||   
		 abs(Kstmass2 - nominalKstarMass)< KstarMassWindowAfterFit_) &&
	       ( ( fittedBdMassHyp1 > BdLowerMassCutAfterFit_ && fittedBdMassHyp1 < BdUpperMassCutAfterFit_ ) ||
		( fittedBdMassHyp2 > BdLowerMassCutAfterFit_ && fittedBdMassHyp2 < BdUpperMassCutAfterFit_ ) ) ) bsRootTree_->BdNumberOfCandidates_++;
	    
	    if(vtxProbHyp1>MinBVtxHyp1){
	      
	      if (abs(Jpsi.mass() - nominalJpsiMass) > JpsiMassWindowAfterFit_ || Jpsi.pt() < JpsiPtCut_) continue;
	      // passed jpsi mass window after fit
	      if(bsRootTree_->iPassedCutIdentBd_   < 11 ) bsRootTree_->iPassedCutIdentBd_ = 11 ;
	            
	      if( abs(Kstmass1 - nominalKstarMass)> KstarMassWindowAfterFit_  &&   
		  abs(Kstmass2 - nominalKstarMass)> KstarMassWindowAfterFit_ ) continue;
	      // if(abs(Kstmass2-0.892)> KstarMassWindowAfterFit_) continue;
	      
	      // passed jpsi kstar window after fit
	      if(bsRootTree_->iPassedCutIdentBd_   < 12 ) bsRootTree_->iPassedCutIdentBd_ = 12 ;
	      if ( ( fittedBdMassHyp1 < BdLowerMassCutAfterFit_ || fittedBdMassHyp1 > BdUpperMassCutAfterFit_ ) &&
		   ( fittedBdMassHyp2 < BdLowerMassCutAfterFit_ || fittedBdMassHyp2 > BdUpperMassCutAfterFit_ )) continue;
	      // passed Bd mass window after fit
	      if(bsRootTree_->iPassedCutIdentBd_   < 13 ) bsRootTree_->iPassedCutIdentBd_ = 13 ;


	      MinBVtxHyp1 = vtxProbHyp1;

	   
	      reco::Vertex reFitVertex;
	      //recalculate primary vertex without tracks from B
	      reco::Vertex tmpFitVertex = reVertex(recVtxs, iEvent,iSetup, trkMu1Ref, trkMu2Ref, trkkst1, trkkst2);
	      if(tmpFitVertex.isValid()) reFitVertex = tmpFitVertex;
	      else reFitVertex = reco::Vertex(RecVtx);   // use the original vertex if the refit vertex is invalid

	      bsRootTree_->BdPVx_refit_    = reFitVertex.x();
	      bsRootTree_->BdPVy_refit_    = reFitVertex.y();
	      bsRootTree_->BdPVz_refit_    = reFitVertex.z();

	      bsRootTree_->BdPVerrx_refit_ = reFitVertex.xError();
	      bsRootTree_->BdPVerry_refit_ = reFitVertex.yError();
	      bsRootTree_->BdPVerrz_refit_ = reFitVertex.zError();
	      


	      bsRootTree_->BdFitChi2_Hyp1_  = bmesHyp1->chiSquared();
	      bsRootTree_->BdFitNdof_Hyp1_   =(int)bmesHyp1->degreesOfFreedom();
	      bsRootTree_->BdFitChi2_Hyp2_  = bmesHyp2->chiSquared();
	      bsRootTree_->BdFitNdof_Hyp2_   =(int)bmesHyp2->degreesOfFreedom();

	      bsRootTree_->BdFitVtxProb_Hyp1_ = vtxProbHyp1;
	      bsRootTree_->BdFitVtxProb_Hyp2_ = vtxProbHyp2;
	      bsRootTree_->BdFitM_Hyp1_ = b_parHyp1[6];		
	      bsRootTree_->BdFitM_Hyp2_ = b_parHyp2[6];		

	      GlobalVector BdvecHyp1(b_parHyp1[3], b_parHyp1[4], b_parHyp1[5]); // the fitted momentum vector 	
	      bsRootTree_->BdFitEta_Hyp1_ = BdvecHyp1.eta();
	      bsRootTree_->BdFitPt_Hyp1_  = BdvecHyp1.perp();
	      bsRootTree_->BdFitPz_Hyp1_  = BdvecHyp1.z();
	      bsRootTree_->BdFitPhi_Hyp1_ = BdvecHyp1.phi();

	      GlobalVector BdvecHyp2(b_parHyp2[3], b_parHyp2[4], b_parHyp2[5]); // the fitted momentum vector 	
	      bsRootTree_->BdFitEta_Hyp2_ = BdvecHyp2.eta();
	      bsRootTree_->BdFitPt_Hyp2_  = BdvecHyp2.perp();
	      bsRootTree_->BdFitPz_Hyp2_  = BdvecHyp2.z();
	      bsRootTree_->BdFitPhi_Hyp2_ = BdvecHyp2.phi();

	      //	      bsRootTree_->setFitParBdKstar(myTree);
	      RefCountedKinematicTree reftree1 = KfitterHyp1.getTree();
	      RefCountedKinematicTree reftree2 = KfitterHyp2.getTree() ;
	      setFitParHyp1( reftree1 );
	      setFitParHyp2( reftree2 );

	      bsRootTree_->BdFitVtx_x_Hyp1_ = bVertexHyp1->position().x();
	      bsRootTree_->BdFitVtx_y_Hyp1_ = bVertexHyp1->position().y();
	      bsRootTree_->BdFitVtx_z_Hyp1_ = bVertexHyp1->position().z();
	 
	      bsRootTree_->BdFitVtx_x_Hyp2_ = bVertexHyp2->position().x();
	      bsRootTree_->BdFitVtx_y_Hyp2_ = bVertexHyp2->position().y();
	      bsRootTree_->BdFitVtx_z_Hyp2_ = bVertexHyp2->position().z();

	      bsRootTree_->BdM_nofit_ = BdCand.mass();
	      bsRootTree_->BdPt_nofit_ = BdCand.pt();
	      bsRootTree_->BdPz_nofit_ = BdCand.pz();
	      bsRootTree_->BdPhi_nofit_ = BdCand.phi();
	      bsRootTree_->BdEta_nofit_ = BdCand.eta();
	      
	      bsRootTree_->KstarMass_nofit_Hyp1_ = Kstmass1;
	      bsRootTree_->KstarMass_nofit_Hyp2_ = Kstmass2;	      

	      bsRootTree_->BdK1Pt_nofit_   = track1.pt();
	      bsRootTree_->BdK1Pz_nofit_   = track1.pz();
	      bsRootTree_->BdK1Eta_nofit_  = track1.eta();
	      bsRootTree_->BdK1Phi_nofit_  = track1.phi();
	      bsRootTree_->BdK1Key_nofit_  = trkkst1.key();
	      bsRootTree_->BdK2Pt_nofit_   = track2.pt();
	      bsRootTree_->BdK2Pz_nofit_   = track2.pz();
	      bsRootTree_->BdK2Eta_nofit_  = track2.eta();
	      bsRootTree_->BdK2Phi_nofit_  = track2.phi();
	      bsRootTree_->BdK2Key_nofit_  = trkkst2.key();	      

	      RefCountedKinematicVertex bdVertex;
	      AlgebraicSymMatrix77 bd_er;
	      GlobalVector Bdvec;
	      double Bdmass;
	      if(K1flag==1)       {bdVertex =  bVertexHyp1; bd_er = bd_erHyp1; Bdvec = BdvecHyp1; Bdmass = fittedBdMassHyp1; }
	      else if (K2flag==1) {bdVertex =  bVertexHyp2; bd_er = bd_erHyp2; Bdvec = BdvecHyp2; Bdmass = fittedBdMassHyp2; }
	      else {std::cout<<"error flag" << std::endl;  exit(1);}



	      ////////////////////////////////////////////////
	      // proper decay time and proper decay length with the refitted vertex
	      ////////////////////////////////////////////////
	      
	      VertexDistanceXY vdist;	      
	      if(Bdvec.perp()!=0) {
		bsRootTree_->BdLxy_    = vdist.distance( reFitVertex, bdVertex->vertexState() ).value(); 
		bsRootTree_->BdLxyErr_ = vdist.distance( reFitVertex, bdVertex->vertexState() ).error(); 
		if (  (bdVertex->position().x()- reFitVertex.x())*Bdvec.x()+(bdVertex->position().y()-reFitVertex.y())*Bdvec.y() < 0  )
		  bsRootTree_->BdLxy_ = -1.0 * bsRootTree_->BdLxy_;   // in case negative sign is necessary 
		bsRootTree_->BdCt_     = bsRootTree_->BdLxy_     *  Bdmass/Bdvec.perp();
		bsRootTree_->BdCtErr_  = bsRootTree_->BdLxyErr_  *  Bdmass/Bdvec.perp();
	      }

// 	      if(BdCand.pt()!=0) {
//                 bsRootTree_->BdLxy_ = ((bdVertex->position().x()-PVx)*Bdvec.x()+(bdVertex->position().y()-PVy)*Bdvec.y())/Bdvec.perp();
//                 bsRootTree_->BdCt_  = bsRootTree_->BdLxy_*Bdmass/Bdvec.perp();
//               }

              bsRootTree_->BdErrX_  = bd_er(1,1);
              bsRootTree_->BdErrY_  = bd_er(2,2);
              bsRootTree_->BdErrXY_ = bd_er(1,2); 
              
              VertexDistance3D vdist3d;
              bsRootTree_->BdDist3d_    = vdist3d.distance(bdVertex->vertexState(),RecVtx).value();
              bsRootTree_->BdDist3dErr_ = vdist3d.distance(bdVertex->vertexState(),RecVtx).error();
              bsRootTree_->BdTime3d_    = bsRootTree_->BdDist3d_    * Bdmass/Bdvec.perp() * 100. /3.;
              bsRootTree_->BdTime3dErr_ = bsRootTree_->BdDist3dErr_ * Bdmass/Bdvec.perp() * 100. /3.;
              
                       
              bsRootTree_->BdDist2d_     = vdist.distance(bdVertex->vertexState(),RecVtx).value();
              bsRootTree_->BdDist2dErr_ = vdist.distance(bdVertex->vertexState(),RecVtx).error();
              bsRootTree_->BdTime2d_     = bsRootTree_->BdDist2d_ * Bdmass/Bdvec.perp() *100. /3.;
              bsRootTree_->BdTime2dErr_  = bsRootTree_->BdDist2dErr_ * Bdmass/Bdvec.perp() * 100. /3.;
        
	       // deltaR matching
	      bool K1Truth = MCmatching( track1, genParticles, bsRootTree_->BdK1mcId_, bsRootTree_->BdK1momId_, bsRootTree_->BdK1gmomId_, 313, 511);
	      bool K2Truth = MCmatching( track2, genParticles, bsRootTree_->BdK2mcId_, bsRootTree_->BdK2momId_, bsRootTree_->BdK2gmomId_, 313, 511);
	      bool Mu1Truth= MCmatching( mu1,    genParticles, bsRootTree_->BdMu1mcId_,bsRootTree_->BdMu1momId_,bsRootTree_->BdMu1gmomId_, 443, 511);
	      bool Mu2Truth= MCmatching( mu2,    genParticles, bsRootTree_->BdMu2mcId_,bsRootTree_->BdMu2momId_,bsRootTree_->BdMu2gmomId_, 443, 511);
	      
	      if (K1Truth==1 && K2Truth==1 && Mu1Truth==1 && Mu2Truth==1)  bsRootTree_->isMatchedBd_ = 1;
	      else bsRootTree_->isMatchedBd_ = 0; 
	    }	      

	    
	  } // trk2 end loop
	} // trk1 end loop
  

      
    } // loop on muons2
  } // loop on muons1
  
  bsRootTree_->fill();

}



void BsToJpsiPhiAnalysis::fillMCInfo( edm::Handle<GenParticleCollection> & genParticles){

  int iNumberOfBdecays = 0;

  // this is a list of all the PDG ids of B mesons
  std::set<int> listOfBmesonIds;
  listOfBmesonIds.insert(511 );   // Bd
  listOfBmesonIds.insert(521 );   // B+
  listOfBmesonIds.insert(10511 );    // B_0*0
  listOfBmesonIds.insert(10521 );    // B_0*+
  listOfBmesonIds.insert(513 );   // B*d
  listOfBmesonIds.insert(523 );   // B*d+
  listOfBmesonIds.insert(10513 );   // B1(L)0
  listOfBmesonIds.insert(10523 );   // B1(L)+
  listOfBmesonIds.insert(20513 );   // B1(H)0
  listOfBmesonIds.insert(20523 );   // B1(H)+
  listOfBmesonIds.insert(515 );    // B2*_0
  listOfBmesonIds.insert(525 );    // B2*_+
  listOfBmesonIds.insert(531 );   // Bs
  listOfBmesonIds.insert(10531 );    // B_s0*_0
  listOfBmesonIds.insert(533 );   // B*s
  listOfBmesonIds.insert(10533 );   // Bs1(L)0
  listOfBmesonIds.insert(20533 );   // Bs1(H)0
  listOfBmesonIds.insert(535 );    // Bs2*_0
  listOfBmesonIds.insert(541 );   // Bc+
  listOfBmesonIds.insert(10541 );   // B*c0+
  listOfBmesonIds.insert(543 );   // B*c+
  listOfBmesonIds.insert(10543 );   // Bc1(L)+
  listOfBmesonIds.insert(20543 );   // Bc1(H)+
  listOfBmesonIds.insert(545 );    // Bc2*_0
  
  listOfBmesonIds.insert(551 );   // etab(1S)
  listOfBmesonIds.insert(10551 );   // chib(1P)
  listOfBmesonIds.insert(100551 );   // etab(2S)
  listOfBmesonIds.insert(110551 );   // chib(2P)
  listOfBmesonIds.insert(200551 );   // etab(3S)
  listOfBmesonIds.insert(210551 );   // chib(3P)
  listOfBmesonIds.insert(553 );   // upsilon(1S)
  listOfBmesonIds.insert(10553 );   // hb(1P)
  listOfBmesonIds.insert(20553 );   // chib1(1P)
  listOfBmesonIds.insert(30553 );   // upsilon1(1D)
  listOfBmesonIds.insert(100553 );   // upsilon(2S)
  listOfBmesonIds.insert(110553 );   // hb(2P)
  listOfBmesonIds.insert(120553 );   // chib1(2P)
  listOfBmesonIds.insert(130553 );   // upsilon1(2D)
  listOfBmesonIds.insert(200553 );   // upsilon(3S)
  listOfBmesonIds.insert(210553 );   // hb(3P)
  listOfBmesonIds.insert(220553 );   // chib1(3P)
  listOfBmesonIds.insert(300553 );   // upsilon(4S)
  listOfBmesonIds.insert(9000553 );   // upsilon(10860)
  listOfBmesonIds.insert(9010553 );   // upsilon(11020)
  listOfBmesonIds.insert(555 );   // chib2(1P)
  listOfBmesonIds.insert(10555 );   // etab2(1D)
  listOfBmesonIds.insert(20555 );   // upsilon2(1D)
  listOfBmesonIds.insert(100555 );   // chib2(2P)
  listOfBmesonIds.insert(110555 );   // etab2(2D)
  listOfBmesonIds.insert(120555 );   // upsilon2(2D)
  listOfBmesonIds.insert(200555 );   // chib2(3P)
  listOfBmesonIds.insert(557 );   // upsilon3(1D)
  listOfBmesonIds.insert(100557 );   // upsilon3(2D)
  
  listOfBmesonIds.insert(5122 );   // lambda_b0
  listOfBmesonIds.insert(5112 );   // sigma_b-
  listOfBmesonIds.insert(5212 );   // sigma_b0
  listOfBmesonIds.insert(5222 );   // sigma_b+
  listOfBmesonIds.insert(5114 );   // sigma*_b-
  listOfBmesonIds.insert(5214 );   // sigma*_b0
  listOfBmesonIds.insert(5224 );   // sigma*_b+
  listOfBmesonIds.insert(5132 );   // Xi_b-
  listOfBmesonIds.insert(5232 );   // Xi_b0
  listOfBmesonIds.insert(5312 );   // Xi'_b-
  listOfBmesonIds.insert(5322 );   // Xi'_b0
  listOfBmesonIds.insert(5314 );   // Xi*_b-
  listOfBmesonIds.insert(5324 );   // Xi*_b0
  listOfBmesonIds.insert(5332 );   // Omega_b-
  listOfBmesonIds.insert(5334 );   // Omega*_b-
  listOfBmesonIds.insert(5142 );   // Xi_bc0
  listOfBmesonIds.insert(5242 );   // Xi_bc+
  listOfBmesonIds.insert(5412 );   // Xi'_bc0
  listOfBmesonIds.insert(5422 );   // Xi'_bc+
  listOfBmesonIds.insert(5414 );   // Xi*_bc0
  listOfBmesonIds.insert(5424 );   // Xi*_bc+
  listOfBmesonIds.insert(5342 );   // Omega_bc0
  listOfBmesonIds.insert(5432 );   // Omega'_bc0
  listOfBmesonIds.insert(5434 );   // Omega*_bc0
  listOfBmesonIds.insert(5442 );   // Omega_bcc+
  listOfBmesonIds.insert(5444 );   // Omega*_bcc+
  listOfBmesonIds.insert(5512 );   // Xi_bb-
  listOfBmesonIds.insert(5522 );   // Xi_bb0
  listOfBmesonIds.insert(5514 );   // Xi*_bb-
  listOfBmesonIds.insert(5524 );   // Xi*_bb0
  listOfBmesonIds.insert(5532 );   // Omega_bb-
  listOfBmesonIds.insert(5524 );   // Omega*_bb-
  listOfBmesonIds.insert(5542 );   // Omega_bbc0
  listOfBmesonIds.insert(5544 );   // Omega*_bbc0
  listOfBmesonIds.insert(554 );   // Omega_bbb-
  
  // loop over all particles
  for( size_t i = 0; i < genParticles->size(); ++ i ) {
    const GenParticle & genBsCand = (*genParticles)[ i ];
    int MC_particleID=genBsCand.pdgId();
    int absMC_particleID = abs(MC_particleID);

    // if this particle id is in the list (i.e. if it is a B meson)
    if( listOfBmesonIds.find( absMC_particleID ) != listOfBmesonIds.end()){
      
      // check if this particle has no daughter which is a B meson (cascade decays)
      // loop over daughters
      bool hasBDaughter=0;
      int numBsDaughters = genBsCand.numberOfDaughters();      
      for(int idau=0; idau < numBsDaughters; idau++) 
	if( listOfBmesonIds.find( abs(genBsCand.daughter(idau)->pdgId())) != listOfBmesonIds.end() ) hasBDaughter=1;

      // if this is a real B decay (no B mesons as daughters
      if(hasBDaughter == 0){
	//count the number of B decays, should be equal two, for bbbar events
	iNumberOfBdecays++;
	int arrayIndex = iNumberOfBdecays - 1; // array index starts at zero

	// protect array bounds
	if(arrayIndex>=9) break;

        bsRootTree_->BmesonsId_[arrayIndex] = MC_particleID;
		
        bsRootTree_->GenNumberOfDaughters_[arrayIndex] = numBsDaughters;
	// generator variables
	bsRootTree_->BMMC_[arrayIndex] = genBsCand.mass();
	bsRootTree_->BPtMC_[arrayIndex] = genBsCand.pt();
	bsRootTree_->BPzMC_[arrayIndex] = genBsCand.pz();
	bsRootTree_->BEtaMC_[arrayIndex] = genBsCand.eta();
	bsRootTree_->BPhiMC_[arrayIndex] = genBsCand.phi();
	bsRootTree_->BVtxMC_x_[arrayIndex] = genBsCand.mother(0)->vx();
	bsRootTree_->BVtxMC_y_[arrayIndex] = genBsCand.mother(0)->vy();
	bsRootTree_->BVtxMC_z_[arrayIndex] = genBsCand.mother(0)->vz();
	//generated primary vertex
	if (abs(MC_particleID)== 531){
	  bsRootTree_->genBsVtx_x_= genBsCand.mother(0)->vx();
	  bsRootTree_->genBsVtx_y_= genBsCand.mother(0)->vy();
	  bsRootTree_->genBsVtx_z_= genBsCand.mother(0)->vz();
	}
	
        for(int j = 0; j < numBsDaughters; ++ j) {
	  if(j>=14) break; // protect array bounds
	  const Candidate * Bsdau = genBsCand.daughter( j );
	  bsRootTree_->BDauIdMC_[arrayIndex][j] = Bsdau->pdgId();
	  // generator variables
	  bsRootTree_->BDauMMC_[arrayIndex][j] = Bsdau->mass();
	  bsRootTree_->BDauPtMC_[arrayIndex][j] = Bsdau->pt();
	  bsRootTree_->BDauPzMC_[arrayIndex][j] = Bsdau->pz();
	  bsRootTree_->BDauEtaMC_[arrayIndex][j] = Bsdau->eta();
	  bsRootTree_->BDauPhiMC_[arrayIndex][j] = Bsdau->phi();
	  bsRootTree_->BSVtxMC_x_[arrayIndex]   =    Bsdau->vx(); 
	  bsRootTree_->BSVtxMC_y_[arrayIndex]   =    Bsdau->vy(); 
	  bsRootTree_->BSVtxMC_z_[arrayIndex]   =    Bsdau->vz(); 
	  //Generated secondary vertex.
	  if ( abs(Bsdau->pdgId())== 333){
	    bsRootTree_->genBsSVtx_x_= Bsdau->vx();
	    bsRootTree_->genBsSVtx_y_= Bsdau->vy();
	    bsRootTree_->genBsSVtx_z_= Bsdau->vz();
	  }


	  
	  // daughter of daughter (muons, kaons in case of jpsi phi)
	  int numBsDaughtersDaughters = Bsdau->numberOfDaughters();
	  bsRootTree_->GenNumberOfDaughtersDaughters_[arrayIndex][j] = numBsDaughtersDaughters;
	  for(int k=0; k< numBsDaughtersDaughters; k++){
	    if(k>=9) break; //protect array bounds
	    const Candidate * Bsdaudau = Bsdau->daughter(k);

	    bsRootTree_->BDauDauIdMC_[arrayIndex][j][k] = Bsdaudau->pdgId();
	    // generator variables
	    bsRootTree_->BDauDauMMC_[arrayIndex][j][k] = Bsdaudau->mass();
	    bsRootTree_->BDauDauPtMC_[arrayIndex][j][k] = Bsdaudau->pt();
	    bsRootTree_->BDauDauPzMC_[arrayIndex][j][k] = Bsdaudau->pz();
	    bsRootTree_->BDauDauEtaMC_[arrayIndex][j][k] = Bsdaudau->eta();
	    bsRootTree_->BDauDauPhiMC_[arrayIndex][j][k] = Bsdaudau->phi();

	  }// loop Bs daughters daughters
	} // loop Bs daughters
	
 	  // calculate gen c tau 
	double deltaX =  bsRootTree_->BSVtxMC_x_[arrayIndex] - 	bsRootTree_->BVtxMC_x_[arrayIndex];
	double deltaY =  bsRootTree_->BSVtxMC_y_[arrayIndex] - 	bsRootTree_->BVtxMC_y_[arrayIndex];
	bsRootTree_->BLxy_MC_[arrayIndex] = sqrt( deltaX*deltaX + deltaY*deltaY);
	if(deltaX * genBsCand.px() + deltaY * genBsCand.py() < 0 )  bsRootTree_->BLxy_MC_[arrayIndex] = -1. *  bsRootTree_->BLxy_MC_[arrayIndex];
	bsRootTree_->BCt_MC_[arrayIndex] = bsRootTree_->BLxy_MC_[arrayIndex] * bsRootTree_->BMMC_[arrayIndex] / 	bsRootTree_->BPtMC_[arrayIndex];
      }
    }
  
    // check if there is a Jpsi (prompt or non-prompt) in the event
    if(absMC_particleID == 443 ) bsRootTree_->isGenJpsiEvent_ = 1;
    
  }

  bsRootTree_->GenNumberOfBdecays_ = iNumberOfBdecays;

}




//------------------------------------------
void BsToJpsiPhiAnalysis::setFitParKK(RefCountedKinematicTree& myTree)
{
  
  
  vector< RefCountedKinematicParticle > bs_children = myTree->finalStateParticles();
  
  // first particle: kaon 1   
  
  AlgebraicVector7 bs_par1 = bs_children[0]->currentState().kinematicParameters().vector();
  for(int i=0; i<7; i++)  bsRootTree_->K1Fit_par_[i] = bs_par1[i];
  
  AlgebraicSymMatrix77 bs_err1 = bs_children[0]->currentState().kinematicParametersError().matrix();
  bsRootTree_->K1Fit_sigX_ = sqrt(bs_err1(0,0));
  bsRootTree_->K1Fit_sigY_ = sqrt(bs_err1(1,1));
  bsRootTree_->K1Fit_sigZ_ = sqrt(bs_err1(2,2));
  bsRootTree_->K1Fit_sigPX_ = sqrt(bs_err1(3,3));
  bsRootTree_->K1Fit_sigPY_ = sqrt(bs_err1(4,4));
  bsRootTree_->K1Fit_sigPZ_ = sqrt(bs_err1(5,5));
  
  // first particle: kaon 2  
  
    
  AlgebraicVector7 bs_par2 = bs_children[1]->currentState().kinematicParameters().vector();
  for(int i=0; i<7; i++) bsRootTree_->K2Fit_par_[i] = bs_par2[i];
  
  AlgebraicSymMatrix77 bs_err2 = bs_children[1]->currentState().kinematicParametersError().matrix();
  bsRootTree_->K2Fit_sigX_ = sqrt(bs_err2(0,0));
  bsRootTree_->K2Fit_sigY_ = sqrt(bs_err2(1,1));
  bsRootTree_->K2Fit_sigZ_ = sqrt(bs_err2(2,2));
  bsRootTree_->K2Fit_sigPX_ = sqrt(bs_err2(3,3));
  bsRootTree_->K2Fit_sigPY_ = sqrt(bs_err2(4,4));
  bsRootTree_->K2Fit_sigPZ_ = sqrt(bs_err2(5,5));
  
}


//------------------------------------------
void BsToJpsiPhiAnalysis::setFitParHyp1(RefCountedKinematicTree& myTree)
{
  
  
  vector< RefCountedKinematicParticle > bs_children = myTree->finalStateParticles();
  
  // first particle: kaon 1   
  
  AlgebraicVector7 bs_par1 = bs_children[0]->currentState().kinematicParameters().vector();
  for(int i=0; i<7; i++)  bsRootTree_->BdK1_kpi_par_Hyp1_[i] = bs_par1[i];
  
  AlgebraicSymMatrix77 bs_err1 = bs_children[0]->currentState().kinematicParametersError().matrix();
  bsRootTree_->BdK1_kpi_sigX_Hyp1_ = sqrt(bs_err1(0,0));
  bsRootTree_->BdK1_kpi_sigY_Hyp1_ = sqrt(bs_err1(1,1));
  bsRootTree_->BdK1_kpi_sigZ_Hyp1_ = sqrt(bs_err1(2,2));
  bsRootTree_->BdK1_kpi_sigPX_Hyp1_ = sqrt(bs_err1(3,3));
  bsRootTree_->BdK1_kpi_sigPY_Hyp1_ = sqrt(bs_err1(4,4));
  bsRootTree_->BdK1_kpi_sigPZ_Hyp1_ = sqrt(bs_err1(5,5));
  
  // first particle: kaon 2  
  
    
  AlgebraicVector7 bs_par2 = bs_children[1]->currentState().kinematicParameters().vector();
  for(int i=0; i<7; i++) bsRootTree_->BdK2_kpi_par_Hyp1_[i] = bs_par2[i];
  
  AlgebraicSymMatrix77 bs_err2 = bs_children[1]->currentState().kinematicParametersError().matrix();
  bsRootTree_->BdK2_kpi_sigX_Hyp1_ = sqrt(bs_err2(0,0));
  bsRootTree_->BdK2_kpi_sigY_Hyp1_ = sqrt(bs_err2(1,1));
  bsRootTree_->BdK2_kpi_sigZ_Hyp1_ = sqrt(bs_err2(2,2));
  bsRootTree_->BdK2_kpi_sigPX_Hyp1_ = sqrt(bs_err2(3,3));
  bsRootTree_->BdK2_kpi_sigPY_Hyp1_ = sqrt(bs_err2(4,4));
  bsRootTree_->BdK2_kpi_sigPZ_Hyp1_ = sqrt(bs_err2(5,5));
  
}

//------------------------------------------
void BsToJpsiPhiAnalysis::setFitParHyp2(RefCountedKinematicTree& myTree)
{
  
  
  vector< RefCountedKinematicParticle > bs_children = myTree->finalStateParticles();
  
  // first particle: kaon 1   
  
  AlgebraicVector7 bs_par1 = bs_children[0]->currentState().kinematicParameters().vector();
  for(int i=0; i<7; i++)  bsRootTree_->BdK1_kpi_par_Hyp2_[i] = bs_par1[i];
  
  AlgebraicSymMatrix77 bs_err1 = bs_children[0]->currentState().kinematicParametersError().matrix();
  bsRootTree_->BdK1_kpi_sigX_Hyp2_ = sqrt(bs_err1(0,0));
  bsRootTree_->BdK1_kpi_sigY_Hyp2_ = sqrt(bs_err1(1,1));
  bsRootTree_->BdK1_kpi_sigZ_Hyp2_ = sqrt(bs_err1(2,2));
  bsRootTree_->BdK1_kpi_sigPX_Hyp2_ = sqrt(bs_err1(3,3));
  bsRootTree_->BdK1_kpi_sigPY_Hyp2_ = sqrt(bs_err1(4,4));
  bsRootTree_->BdK1_kpi_sigPZ_Hyp2_ = sqrt(bs_err1(5,5));
  
  // first particle: kaon 2  
  
    
  AlgebraicVector7 bs_par2 = bs_children[1]->currentState().kinematicParameters().vector();
  for(int i=0; i<7; i++) bsRootTree_->BdK2_kpi_par_Hyp2_[i] = bs_par2[i];
  
  AlgebraicSymMatrix77 bs_err2 = bs_children[1]->currentState().kinematicParametersError().matrix();
  bsRootTree_->BdK2_kpi_sigX_Hyp2_ = sqrt(bs_err2(0,0));
  bsRootTree_->BdK2_kpi_sigY_Hyp2_ = sqrt(bs_err2(1,1));
  bsRootTree_->BdK2_kpi_sigZ_Hyp2_ = sqrt(bs_err2(2,2));
  bsRootTree_->BdK2_kpi_sigPX_Hyp2_ = sqrt(bs_err2(3,3));
  bsRootTree_->BdK2_kpi_sigPY_Hyp2_ = sqrt(bs_err2(4,4));
  bsRootTree_->BdK2_kpi_sigPZ_Hyp2_ = sqrt(bs_err2(5,5));
  
}



bool  BsToJpsiPhiAnalysis::MCmatching(const Candidate & track1,  edm::Handle<GenParticleCollection> & genParticles,
				      int &K1mcId, int &K1momId, int &K1gmomId,
				      int condMom, int condGMom){
  if(thegenParticlesLabel_.label() == "" ) return 0;
  bool K1Truth = 0;
  double MinDRK=999.;
  
  K1mcId = -9999999;
  K1momId = -9999999;
  K1gmomId = -9999999;

  for(size_t i = 0; i < genParticles->size(); ++ i) {
    const GenParticle & p = (*genParticles)[i];
    double DeltaRK1 = deltaR(p.eta(), p.phi(), track1.eta(), track1.phi() );
   
    if(DeltaRK1<MinDRK && DeltaRK1<0.05){
      MinDRK=DeltaRK1;
      K1mcId=p.pdgId();
      if(p.mother()!=0) K1momId=p.mother()->pdgId();
      if(p.mother()!=0 && p.mother()->mother()!=0) K1gmomId=p.mother()->mother()->pdgId(); 
      if (abs(K1momId)==condMom && abs(K1gmomId)==condGMom) K1Truth = 1;
      else K1Truth = 0;
    }
  }

  return K1Truth;
  
}

	    
reco::Vertex BsToJpsiPhiAnalysis::reVertex(const edm::Handle<reco::VertexCollection> &handle, const edm::Event &iEvent, const edm::EventSetup& iSetup,
					   TrackRef trk1, TrackRef trk2, TrackRef trk3, TrackRef trk4){


  //copied from Onia2MuMu/VertexReProducer
  const edm::Provenance *prov = handle.provenance();
  if (prov == 0) throw cms::Exception("CorruptData") << "Vertex handle doesn't have provenance.";
  edm::ParameterSetID psid = prov->psetID();
  
  edm::pset::Registry *psregistry = edm::pset::Registry::instance();
  edm::ParameterSet psetFromProvenance;
  if (!psregistry->getMapped(psid, psetFromProvenance)) 
    throw cms::Exception("CorruptData") << "Vertex handle parameter set ID id = " << psid;
  
  if (prov->moduleName() != "PrimaryVertexProducer") 
    throw cms::Exception("Configuration") << "Vertices to re-produce don't come from a PrimaryVertexProducer, but from a " << prov->moduleName() <<".\n";
  
  edm::InputTag tracksTag_   = psetFromProvenance.getParameter<edm::InputTag>("TrackLabel");
  edm::InputTag   beamSpotTag_ = psetFromProvenance.getParameter<edm::InputTag>("beamSpotLabel");
  
  Handle<reco::TrackCollection> pvtracks;   iEvent.getByLabel(tracksTag_,   pvtracks);
  Handle<reco::BeamSpot>        pvbeamspot; iEvent.getByLabel(beamSpotTag_, pvbeamspot);
  
  TrackCollection muonLess;
  muonLess.reserve(pvtracks->size()-4);

  for (size_t i = 0; i < pvtracks->size(); ++i) {
    if (i == trk1.key()){
      double dpt = trk1->pt() - (*pvtracks)[i].pt();   double deta =  trk1->eta() - (*pvtracks)[i].eta(); double dphi= trk1->phi() - (*pvtracks)[i].phi();
      double delta = dpt*dpt + deta*deta + dphi*dphi;
      if( delta > 0.0001){
	std::cout<<"BsToJpsiPhiAnalysis::reVertex: ERROR" << std::endl;
	exit(1);
      }
    } 
    if (i == trk2.key()){
      double dpt = trk2->pt() - (*pvtracks)[i].pt();   double deta =  trk2->eta() - (*pvtracks)[i].eta(); double dphi= trk2->phi() - (*pvtracks)[i].phi();
      double delta = dpt*dpt + deta*deta + dphi*dphi;
      if( delta > 0.0001){
	std::cout<<"BsToJpsiPhiAnalysis::reVertex: ERROR" << std::endl;
	exit(1);
      }      
    } 
    if (i == trk3.key()){
      double dpt = trk3->pt() - (*pvtracks)[i].pt();   double deta =  trk3->eta() - (*pvtracks)[i].eta(); double dphi= trk3->phi() - (*pvtracks)[i].phi();
      double delta = dpt*dpt + deta*deta + dphi*dphi;
      if( delta > 0.0001){
	std::cout<<"BsToJpsiPhiAnalysis::reVertex: ERROR" << std::endl;
	exit(1);
      }
    } 
  }


 for (size_t i = 0; i < pvtracks->size(); ++i) {
   if (i == trk1.key()) continue;
   if (i == trk2.key()) continue;
   if (i == trk3.key()) continue;
   if (i == trk4.key()) continue;
   muonLess.push_back((*pvtracks)[i]);
 }


 edm::ESHandle<TransientTrackBuilder> theB;
 iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theB);
 
 vector<reco::TransientTrack> t_tks; t_tks.reserve(muonLess.size());
 
 for (reco::TrackCollection::const_iterator it = muonLess.begin(); it != muonLess.end(); ++it) {
   t_tks.push_back((*theB).build(*it));
   t_tks.back().setBeamSpot(*pvbeamspot);
 }

 vector<TransientVertex> pvs = PrimaryVertexProducerAlgorithm(psetFromProvenance).vertices(t_tks, *pvbeamspot);
 
 if(pvs.size() > 0) return reco::Vertex(pvs.front());
 return reco::Vertex();
}
