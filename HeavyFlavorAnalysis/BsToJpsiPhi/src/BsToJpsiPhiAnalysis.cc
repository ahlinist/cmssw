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
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/Candidate/interface/CompositeCandidateFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"
// #include ""

#include <iostream>
#include <TMath.h>

using namespace reco;
using namespace edm;
using namespace std;

BsToJpsiPhiAnalysis::BsToJpsiPhiAnalysis(const edm::ParameterSet& iConfig) : theConfig_(iConfig)
{
  bCandLabel_ = iConfig.getParameter<InputTag>("BCandLabel");
  trackLabel_ = iConfig.getParameter<InputTag>("TrackLabel");
  vertexLabel_ = iConfig.getParameter<InputTag>("VertexLabel");
  outputFile_ = iConfig.getUntrackedParameter<std::string>("outputFile");
  kvfPSet_ = iConfig.getParameter<edm::ParameterSet>("KVFParameters");
  isSim_ = iConfig.getParameter<bool>("isSim");
  deltaR_ = iConfig.getParameter<double>("deltaRforMatching");
  
  edm::LogInfo("RecoVertex/BsToJpsiPhiAnalysis")<< "Initializing Bs to Jpsi Phi analyser  - Output file: " << outputFile_ <<"\n";
  
}



BsToJpsiPhiAnalysis::~BsToJpsiPhiAnalysis() {
  
}


void BsToJpsiPhiAnalysis::beginJob(edm::EventSetup const& setup)
{
  edm::ESHandle<TrackAssociatorBase> theAssociatorByHits;
  setup.get<TrackAssociatorRecord>().get("TrackAssociatorByHits",theAssociatorByHits);
  associatorByHits_ = (TrackAssociatorByHits *) theAssociatorByHits.product();
  
  // create the root tree the events are written to
  bsRootTree_ = new BsToJpsiPhiRootTree(outputFile_);
	
  event_counter_ = 0;
  flag_1 = 0;
  flag_2 = 0;
  flag_3 = 0;
  flag_4 = 0;
  flag_5 = 0;
  bdjpsikstar = 0;
  bdjpsiks = 0;
}

void BsToJpsiPhiAnalysis::endJob() 
{
  // delete the root tree
  delete bsRootTree_;
  
  cout << "Total number of Events: " << event_counter_ << endl;

}

void
BsToJpsiPhiAnalysis::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  event_counter_++;
  if(event_counter_ % 500 == 0) cout << "event number " << event_counter_ << endl;
  try {
    //    edm::LogInfo("HeavyFlavorAnalysis/BsToJpsiPhiAnalysis") << "Reconstructing event number: " << iEvent.id() << "\n";
    
    // get the Bs collection 
    edm::Handle<CandidateView> bCandCollection;
    // access the physics object (b candidate)
    iEvent.getByLabel(bCandLabel_,bCandCollection);
    
    //    edm::LogInfo("HeavyFlavorAnalysis/BsToJpsiPhiAnalysis") << "Found: " << bCandCollection->size() << " reconstructed Bs" << "\n";
    
    // get primary vertices  
    edm::Handle<reco::VertexCollection> vertices;
    // access the physics object (vertices)
    iEvent.getByLabel(vertexLabel_,vertices);
    const Vertex * primaryVertex;
    if (vertices->size() > 0){
      primaryVertex = &(*vertices->begin());
    } else {
      primaryVertex = 0;
    }

    // get the builder to transform tracks to TransientTrack
    edm::ESHandle<TransientTrackBuilder> theB;
    iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theB);	
    // is the a Bs candidate and a primary vertex?
    if(bCandCollection->size()>0 && primaryVertex != 0) {
      
      // get the simVertex collection and decay vertex
      edm::Handle<TrackingVertexCollection> trackingTruthVertexCollectionH;
      const TrackingVertexCollection *trackingTruthVertexCollection = 0;
      const TrackingVertex * simVertex = 0;
      if(isSim_){
	iEvent.getByLabel("mergedtruth", trackingTruthVertexCollectionH);
	trackingTruthVertexCollection   = trackingTruthVertexCollectionH.product();
	// 				cout << "trackingTruthVertexCollection: " << trackingTruthVertexCollection << endl;
	simVertex = getSimVertex(trackingTruthVertexCollection);
	// 				cout << "simVertex: " << simVertex << endl;
      }
      
      // loop over Bs candidates
			for (CandidateView::const_iterator bit=(*bCandCollection).begin();bit!=(*bCandCollection).end();bit++) {
			  bsRootTree_->resetEntries();
			  // are the particles associated?
			  bool trueMuPlus = false;			
			  bool trueMuMinus = false;
			  bool trueKPlus = false;
			  bool trueKMinus = false;
			  
			  // for association
			  
			  if ((*bit).numberOfDaughters()!=2){
					continue;
			  }
			  
			  // get the jpsi candidate
			  const Candidate *jpsi = bit->daughter(0);
			  TransientTrack muPlusTk;				
			  TransientTrack muMinusTk;	
			  
			  //loop over muons				
			  for (Candidate::const_iterator muit=jpsi->begin();muit!=jpsi->end();muit++){	
			    const Muon  * mucand = dynamic_cast<const Muon*> (&*muit->masterClone());
			    if(mucand == 0 ) continue;
			    TrackRef mutk = mucand->track();
			    TransientTrack muttkp   = (*theB).build(&mutk);				
			    // mu+ = -13
			    if(muttkp.charge() > 0){
			      muPlusTk = muttkp; 
			      if(isSim_ && (simVertex != 0)){
				TrackingParticleRef tpr;
				for (TrackingVertex::tp_iterator dt = simVertex->daughterTracks_begin(); dt!= simVertex->daughterTracks_end(); ++dt) {
				  if((**dt).pdgId() == -13){
				    tpr= *dt;
				    break;
				  }
				}
				double dR = deltaRRecoSim(&muPlusTk , &tpr);			
				if(dR < deltaR_){
				  trueMuPlus = true;
				}
			      }
			    } else {
			      // mu- = 13
			      muMinusTk = muttkp;
			      if(isSim_ && (simVertex != 0)){
				TrackingParticleRef tpr;
				for (TrackingVertex::tp_iterator dt = simVertex->daughterTracks_begin(); dt!= simVertex->daughterTracks_end(); ++dt) {
				  if((**dt).pdgId() == 13){
				    tpr= *dt;
				    break;
				  }
				}			
				double dR = deltaRRecoSim(&muMinusTk , &tpr);			
				if(dR < deltaR_){
				  trueMuMinus = true;
				}
			      }
			    }
			  }
			  //end loop over muons

			  ////////////////////
			  // phi 
			  ////////////////////

			  // get the phi candidate
			  const Candidate *phi = bit->daughter(1);
			  TransientTrack kPlusTk;				
			  TransientTrack kMinusTk;				

			  //loop over kaons				
			  for (Candidate::const_iterator kit=phi->begin();kit!=phi->end();kit++){					
			    const RecoCandidate* kcand = dynamic_cast<const RecoCandidate*> (&*kit->masterClone());
			    if(kcand == 0) continue;
			    TrackRef ktk = kcand->track();
			    TransientTrack kttkp   = (*theB).build(&ktk);
			    // K+ = 321
			    if(kttkp.charge() > 0 ){
			      kPlusTk = kttkp;
			      if(isSim_ && (simVertex != 0)){
				TrackingParticleRef tpr;
				for (TrackingVertex::tp_iterator dt = simVertex->daughterTracks_begin(); dt!= simVertex->daughterTracks_end(); ++dt) {
				  if((**dt).pdgId() == 321){
				    tpr= *dt;
				    break;
				  }
				}			
				double dR = deltaRRecoSim(&kPlusTk , &tpr);			
				if(dR < deltaR_){
				  trueKPlus = true;
				}
			      }
			    } else {
			      // K- = -321
			      kMinusTk = kttkp;
			      if(isSim_ && (simVertex != 0)){
				TrackingParticleRef tpr;
				for (TrackingVertex::tp_iterator dt = simVertex->daughterTracks_begin(); dt!= simVertex->daughterTracks_end(); ++dt) {
				  if((**dt).pdgId() == -321){
				    tpr= *dt;
				    break;
				  }
				}			
				double dR = deltaRRecoSim(&kMinusTk , &tpr);			
				if(dR < deltaR_){
				  trueKMinus = true;
				}
			      }
			    }
			  }
			  //end loop over kaons
			  
			  //the final state muons and kaons from the Bs->J/PsiPhi->mumuKK decay
			  
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
			  allParticles.push_back(pFactory.particle (muPlusTk,muon_mass,chi,ndf,muon_sigma));
			  allParticles.push_back(pFactory.particle (muMinusTk,muon_mass,chi,ndf,muon_sigma));
			  
			  allParticles.push_back(pFactory.particle (kPlusTk,kaon_mass,chi,ndf,kaon_sigma));
			  allParticles.push_back(pFactory.particle (kMinusTk,kaon_mass,chi,ndf,kaon_sigma));
			
			  //creating the constraint for the J/Psi mass
			  ParticleMass m_jpsi = 3.09687;
			  
			  //creating the two track mass constraint
			  MultiTrackKinematicConstraint *  j_psi_c = new  TwoTrackMassKinematicConstraint(m_jpsi);
			  
			  //creating the fitter (global fit)
			  KinematicConstrainedVertexFitter kcvFitter;
			  
			  //obtaining the resulting tree
			  RefCountedKinematicTree myTree = kcvFitter.fit(allParticles, j_psi_c);
			  
			  // write the Bs in the root tree
			  bsRootTree_->addDecay(myTree, primaryVertex);
			  
			  bsRootTree_->setAssociation(trueMuPlus && trueMuMinus && trueKPlus && trueKMinus);
			  
			  if(bsRootTree_->getAssociation()){
			    bsRootTree_->addSimDecay(simVertex, &trackingTruthVertexCollection->front());
			  }

			  // HLT code for trigger bits

			  edm::Handle<edm::TriggerResults>  hltresults;
			  edm::InputTag tag("TriggerResults::HLT");
			  iEvent.getByLabel(tag, hltresults);

			  edm::TriggerNames triggerNames_;
			  triggerNames_.init(* hltresults);

			  // flag = 1 HLT_Mu3
			  // flag = 2 HLT_Mu5
			  // flag = 3 HLT_DoubleIsoMu3
			  // flag = 4 HLT_DoubleMu3
			  // flag = 5 HLT_DoubleMu3_JPsi

			  flag_1 = 0;
			  flag_2 = 0;
			  flag_3 = 0;
			  flag_4 = 0;
			  flag_5 = 0;

			  int itrig_1 = triggerNames_.triggerIndex("HLT_Mu3");
			  int itrig_2 = triggerNames_.triggerIndex("HLT_Mu5");
			  int itrig_3 = triggerNames_.triggerIndex("HLT_DoubleIsoMu3");
			  int itrig_4 = triggerNames_.triggerIndex("HLT_DoubleMu3");
			  int itrig_5 = triggerNames_.triggerIndex("HLT_DoubleMu3_JPsi");

			  if (hltresults->accept(itrig_1)) flag_1 = 1;
			  if (hltresults->accept(itrig_2)) flag_2 = 1;
			  if (hltresults->accept(itrig_3)) flag_3 = 1;
			  if (hltresults->accept(itrig_4)) flag_4 = 1;
			  if (hltresults->accept(itrig_5)) flag_5 = 1;

			  bdjpsikstar = 0 ;
			  bdjpsikstar = JpsiKstarFlag(trackingTruthVertexCollection);

			  bdjpsiks = 0 ;
			  bdjpsiks = JpsiKsFlag(trackingTruthVertexCollection);
			  
			  bsRootTree_->getBdFlags(bdjpsikstar,bdjpsiks);			  
			  bsRootTree_->getTrigBit(flag_1,flag_2,flag_3,flag_4,flag_5);			  
			  bsRootTree_->fill();
			}
			// end loop over Bs
    } 
  } catch (std::exception & err) {
    cout
      << "Exception during event number: " << iEvent.id()
      << "\n" << err.what() << "\n";
  }
}

//
// Returns the first Bs to Jpsi phi vertex
//
const TrackingVertex * BsToJpsiPhiAnalysis::getSimVertex(const TrackingVertexCollection * trackingTruthVertexCollection) const
{
  //iterate over al SimVertices
  for(TrackingVertexCollection::const_iterator v=trackingTruthVertexCollection->begin(); v!=trackingTruthVertexCollection->end(); ++v){	   
    bool bs = false;
    bool jpsi = false;
    bool phi = false; 
    bool kplus = false;
    bool kminus = false;
    bool muminus = false;
    bool muplus = false;
    // look if the daughters of the vertex are mu+, mu-, k+ and k-
    if (v->daughterTracks().size()==4) {
      for (TrackingVertex::tp_iterator dt = v->daughterTracks_begin(); dt!= v->daughterTracks_end(); ++dt) {
	if((**dt).pdgId() == -13){
	  muplus = true;
	}
	if((**dt).pdgId() == 13){
	  muminus = true;
	}
	if((**dt).pdgId() == 321){
	  kplus = true;
				}
	if((**dt).pdgId() == -321){
	  kminus = true;
	}
      }
    }
    if(kplus && kminus && muminus && muplus){
      // 			cout << "number of genVertices: " << v->genVertices().size() << endl;
    }
    // look if the mothers of the vertex are Bs, Jpsi and phi 
    if (v->genVertices().size() == 3) {
      for (TrackingVertex::genv_iterator  dt = v->genVertices_begin(); dt!= v->genVertices_end(); ++dt){
	if( (*dt)->particles_in_size() > 1) std::cout << "more than one in particle !!!" << endl;
	if( ((*dt)->particles_in_size() == 1) && (((*(*dt)->particles_in_const_begin())->pdg_id() == 531) || ((*(*dt)->particles_in_const_begin())->pdg_id() == -531)) ){
	  bs = true;
					cout << "found Bs" << endl;
	}
	if( ((*dt)->particles_in_size() == 1) && (((*(*dt)->particles_in_const_begin())->pdg_id() == 443) || ((*(*dt)->particles_in_const_begin())->pdg_id() == -443)) ){
	  jpsi = true;
	  cout << "found jpsi" << endl;
	}
	if( ((*dt)->particles_in_size() == 1) && (((*(*dt)->particles_in_const_begin())->pdg_id() == 333) || ((*(*dt)->particles_in_const_begin())->pdg_id() == -333)) ){
	  phi = true;
	  cout << "found phi" << endl;
	}
			}
    }
    if(kplus && kminus && muminus && muplus){ 
      return &(*v);
    }
  }
  return 0;
}

// flag for Jpsi K*

int BsToJpsiPhiAnalysis::JpsiKstarFlag(const TrackingVertexCollection * trackingTruthVertexCollection)
{
  //iterate over al SimVertices
  for(TrackingVertexCollection::const_iterator v=trackingTruthVertexCollection->begin(); v!=trackingTruthVertexCollection->end(); ++v){	   
    int flagbdjpsikstar = 0;
    bool bd = false;
    bool jpsi = false;
    bool Kstar = false; 
    bool kaon = false;
    bool pion = false;
    bool muminus = false;
    bool muplus = false;
    // look if the daughters of the vertex are mu+, mu-, k+/- and pi+/-
    if (v->daughterTracks().size()==4) {
      for (TrackingVertex::tp_iterator dt = v->daughterTracks_begin(); dt!= v->daughterTracks_end(); ++dt) {
	if((**dt).pdgId() == -13){
	  muplus = true;
	}
	if((**dt).pdgId() == 13){
	  muminus = true;
	}
	if((**dt).pdgId() == 321 || (**dt).pdgId()== -321){
	  kaon = true;
	}
	if((**dt).pdgId() == -211 || (**dt).pdgId()== 211){
	  pion = true;
	}
      }
    }
    // look if the mothers of the vertex are Bs, Jpsi and Kstar
    if (v->genVertices().size() == 3) {
      for (TrackingVertex::genv_iterator  dt = v->genVertices_begin(); dt!= v->genVertices_end(); ++dt){
	if( (*dt)->particles_in_size() > 1) std::cout << "more than one in particle !!!" << endl;
	if( ((*dt)->particles_in_size() == 1) && (((*(*dt)->particles_in_const_begin())->pdg_id() == 511) || ((*(*dt)->particles_in_const_begin())->pdg_id() == -511)) ){
	  bd = true;
					cout << "found Bd" << endl;
	}
	if( ((*dt)->particles_in_size() == 1) && (((*(*dt)->particles_in_const_begin())->pdg_id() == 443) || ((*(*dt)->particles_in_const_begin())->pdg_id() == -443)) ){
	  jpsi = true;
	  cout << "found jpsi" << endl;
	}
	if( ((*dt)->particles_in_size() == 1) && (((*(*dt)->particles_in_const_begin())->pdg_id() == 313) || ((*(*dt)->particles_in_const_begin())->pdg_id() == -313)) ){
	  Kstar = true;
	  cout << "found Kstar" << endl;
	}
			}
    }
    if(bd && jpsi && Kstar){ 
    //    if(muplus && muminus && kaon && pion){ 
      flagbdjpsikstar = 1;
    } else
      flagbdjpsikstar = 0;

    return flagbdjpsikstar;
  }
}

// flag for Jpsi Ks

int BsToJpsiPhiAnalysis::JpsiKsFlag(const TrackingVertexCollection * trackingTruthVertexCollection)
{
  //iterate over al SimVertices
  for(TrackingVertexCollection::const_iterator v=trackingTruthVertexCollection->begin(); v!=trackingTruthVertexCollection->end(); ++v){	   
    int flagbdjpsiks = 0;
    bool bd = false;
    bool jpsi = false;
    bool Ks = false; 
    bool pionp = false;
    bool pionm = false;
    bool muminus = false;
    bool muplus = false;
    // look if the daughters of the vertex are mu+, mu-, k+/- and pi+/-
    if (v->daughterTracks().size()==4) {
      for (TrackingVertex::tp_iterator dt = v->daughterTracks_begin(); dt!= v->daughterTracks_end(); ++dt) {
	if((**dt).pdgId() == -13){
	  muplus = true;
	}
	if((**dt).pdgId() == 13){
	  muminus = true;
	}
	if((**dt).pdgId() == 211){
	  pionp = true;
	}
	if((**dt).pdgId() == -211){
	  pionm = true;
	}
      }
    }
    // look if the mothers of the vertex are Bs, Jpsi and Kstar
    if (v->genVertices().size() == 3) {
      for (TrackingVertex::genv_iterator  dt = v->genVertices_begin(); dt!= v->genVertices_end(); ++dt){
	if( (*dt)->particles_in_size() > 1) std::cout << "more than one in particle !!!" << endl;
	if( ((*dt)->particles_in_size() == 1) && (((*(*dt)->particles_in_const_begin())->pdg_id() == 511) || ((*(*dt)->particles_in_const_begin())->pdg_id() == -511)) ){
	  bd = true;
					cout << "found Bd" << endl;
	}
	if( ((*dt)->particles_in_size() == 1) && (((*(*dt)->particles_in_const_begin())->pdg_id() == 443) || ((*(*dt)->particles_in_const_begin())->pdg_id() == -443)) ){
	  jpsi = true;
	  cout << "found jpsi" << endl;
	}
	if( ((*dt)->particles_in_size() == 1) && (((*(*dt)->particles_in_const_begin())->pdg_id() == 310) || ((*(*dt)->particles_in_const_begin())->pdg_id() == -310)) ){
	  Ks = true;
	  cout << "found Ks" << endl;
	}
			}
    }
    if(bd && jpsi && Ks){ 
    //    if(muplus && muminus && kaon && pion){ 
      flagbdjpsiks = 1;
    } else
      flagbdjpsiks = 0;

    return flagbdjpsiks;
  }
}

//
// calculates deltaR between Reco and Sim track
//
double BsToJpsiPhiAnalysis::deltaRRecoSim(const TransientTrack * ttk, const TrackingParticleRef * tpr)
{
  double deltaPhi = fabs(ttk->track().phi()-(*tpr)->phi());
  if (deltaPhi > TMath::Pi()) deltaPhi = 2*TMath::Pi() - deltaPhi;
  double deltaEta = ttk->track().eta()-(*tpr)->eta();
  return sqrt( deltaEta*deltaEta + deltaPhi*deltaPhi );
}
