#include "HeavyFlavorAnalysis/BsToJpsiPhi/interface/BsToJpsiPhiAnalysis.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/Common/interface/EDProduct.h"
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/ESHandle.h"

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
#include "DataFormats/MuonReco/interface/Muon.h"

#include <iostream>
#include <TMath.h>

using namespace reco;
using namespace edm;
using namespace std;

BsToJpsiPhiAnalysis::BsToJpsiPhiAnalysis(const edm::ParameterSet& iConfig) : theConfig_(iConfig)
{
	bCandLabel_ = iConfig.getParameter<std::string>("BCandLabel");
	trackLabel_ = iConfig.getParameter<std::string>("TrackLabel");
	vertexLabel_ = iConfig.getParameter<std::string>("VertexLabel");
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

// 	// create some histograms
// 	hFile_ = new TFile ("bsPlots.root", "RECREATE" );
// 	h_Bs_mass = new TH1F("Bs_mass","Bs_mass", 100, 5.2, 5.6);
// 	h_l_xy = new TH1F("transverse_decay_length","transverse_decay_length", 100, 0., 1.);
//   	h_l = new TH1F("decay_length","decay_length", 100, 0., 5.);
//   	h_t_xy = new TH1F("proper_transverse_time","proper_transverse_time", 100, 0., 0.5);
//   	h_t = new TH1F("proper_time","proper_time", 100, 0., 5.);
//   	h_pT_muon = new TH1F("mu_pT","mu_pT", 100, 0., 50.);
//   	h_pT_Kaon = new TH1F("K_pT","K_pT", 100, 0., 50.);
//   	h_pT_Jpsi = new TH1F("Jpsi_pT","Jpsi_pT", 100, 0., 50.);
//   	h_pT_phi = new TH1F("phi_pT","phi_pT", 100, 0., 50.);
//   	h_pT_Bs = new TH1F("Bs_pT","Bs_pT", 100, 0., 50.);
// 	h_fit_prob = new TH1F("fit_prob","fit_prob", 100, 0., 1.);
	
	event_counter_ = 0;
}

void BsToJpsiPhiAnalysis::endJob() 
{

	// delete the root tree
	delete bsRootTree_;
	cout << "Total number of Events: " << event_counter_ << endl;
}

//
// member functions
//

void
BsToJpsiPhiAnalysis::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	event_counter_++;
	if(event_counter_ % 500 == 0) cout << "event number " << event_counter_ << endl;
  	try {
		edm::LogInfo("HeavyFlavorAnalysis/BsToJpsiPhiAnalysis") << "Reconstructing event number: " << iEvent.id() << "\n";
	
		//get the Bs collection 
		edm::Handle<CandidateCollection> bCandCollection;
		iEvent.getByLabel(bCandLabel_,bCandCollection);
		
		edm::LogInfo("HeavyFlavorAnalysis/BsToJpsiPhiAnalysis") << "Found: " << bCandCollection->size() << " reconstructed Bs" << "\n";
		
		// get primary vertices  
		edm::Handle<reco::VertexCollection> vertices;
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
				iEvent.getByLabel("trackingtruthprod", trackingTruthVertexCollectionH);
				trackingTruthVertexCollection   = trackingTruthVertexCollectionH.product();
				simVertex = getSimVertex(trackingTruthVertexCollection);
			}
			
			// association: does not work in CMSSW_1_3_6 on triggerd events
			// use deltaR association istead
			
// 			// get the simParticle collection 
// 			edm::Handle<TrackingParticleCollection>  trackingTruthParticelCollectionH;
// 			iEvent.getByLabel("trackingtruthprod",trackingTruthParticelCollectionH);
// 			iEvent.getByLabel("trackingtruth", "TrackTruth",trackingTruthParticelCollectionH);
// 			// get the track collection
// 			edm::Handle<reco::TrackCollection> tks;
// 			iEvent.getByLabel(trackLabel_ , tks);
// 			// associate reco tracks and simtracks
// 			cout << "before association" << endl;
// 			reco::RecoToSimCollection recSimColl=associatorByHits_->associateRecoToSim(tks, trackingTruthParticelCollectionH, &iEvent);
// 			reco::SimToRecoCollection simRecColl=associatorByHits_->associateSimToReco(tks, trackingTruthParticelCollectionH, &iEvent);
// 			cout << "after association" << endl;
			
			// loop over Bs candidates
			for (CandidateCollection::const_iterator bit=(*bCandCollection).begin();bit!=(*bCandCollection).end();bit++) {
				bsRootTree_->resetEntries();
				
				// are the particles associated?
				bool trueMuPlus = false;			
				bool trueMuMinus = false;
				bool trueKPlus = false;
				bool trueKMinus = false;
				
				// for association
// 				std::vector<std::pair<TrackingParticleRef, double> > simFound;
			
				if ((*bit).numberOfDaughters()!=2){
					continue;
				}
				
				// get the jpsi candidate
				const Candidate *jpsi = bit->daughter(0);
				TransientTrack muPlusTk;				
				TransientTrack muMinusTk;	
				//loop over muons				
				for (Candidate::const_iterator muit=jpsi->begin();muit!=jpsi->end();muit++){	
					const Muon * mucand = dynamic_cast<const Muon *> (&(*muit));
					TrackRef mutk = mucand->track();
					TransientTrack muttkp   = (*theB).build(&mutk);				
					if(muttkp.charge() > 0){
						muPlusTk = muttkp; 
						if(isSim_ && (simVertex != 0)){
// 							simFound = (recSimColl)[mutk];
// 							cout << "found " << simFound.size() << " sim particles for muplus" << endl;
							TrackingParticleRef tpr;
							for (TrackingVertex::tp_iterator dt = simVertex->daughterTracks_begin(); dt!= simVertex->daughterTracks_end(); ++dt) {
								if((**dt).pdgId() == -13){
									tpr= *dt;
									break;
								}
							}
							double dR = deltaRRecoSim(&muPlusTk , &tpr);			
							cout << "delta R: " << dR << endl;
							if(dR < deltaR_){
								trueMuPlus = true;
							}
// 							for(std::vector<std::pair<TrackingParticleRef, double> >::const_iterator isim = simFound.begin(); isim != simFound.end(); isim++){
// 								if(isim->first == tpr){
// 									cout << "simpointer.second: " << isim->second << endl;
// 									trueMuPlus = true;
// 								}
// 							}
						}
					} else {
						muMinusTk = muttkp;
						if(isSim_ && (simVertex != 0)){
// 							simFound = (recSimColl)[mutk];
// 							cout << "found " << simFound.size() << " sim particles for muminus" << endl;
							TrackingParticleRef tpr;
							for (TrackingVertex::tp_iterator dt = simVertex->daughterTracks_begin(); dt!= simVertex->daughterTracks_end(); ++dt) {
								if((**dt).pdgId() == 13){
									tpr= *dt;
									break;
								}
							}			
							double dR = deltaRRecoSim(&muMinusTk , &tpr);			
							cout << "delta R: " << dR << endl;
							if(dR < deltaR_){
								trueMuMinus = true;
							}
// 							for(std::vector<std::pair<TrackingParticleRef, double> >::const_iterator isim = simFound.begin(); isim != simFound.end(); isim++){
// 								if(isim->first == tpr){
// 									cout << "simpointer.second: " << isim->second << endl;
//	 								trueMuMinus = true;
// 								}
// 							}
						}
					}
				}//end loop over muons
				
				const Candidate *phi = bit->daughter(1);
				TransientTrack kPlusTk;				
				TransientTrack kMinusTk;				
				//loop over kaons				
				for (Candidate::const_iterator kit=phi->begin();kit!=phi->end();kit++){
					const RecoCandidate* kcand = dynamic_cast<const RecoCandidate*> (&(*kit));
					TrackRef ktk = kcand->track();
					TransientTrack kttkp   = (*theB).build(&ktk);
					if(kttkp.charge() > 0 ){
						kPlusTk = kttkp;
						if(isSim_ && (simVertex != 0)){
// 							simFound = (recSimColl)[ktk];
// 							cout << "found " << simFound.size() << " sim particles for kplus" << endl;
							TrackingParticleRef tpr;
							for (TrackingVertex::tp_iterator dt = simVertex->daughterTracks_begin(); dt!= simVertex->daughterTracks_end(); ++dt) {
								if((**dt).pdgId() == 321){
									tpr= *dt;
									break;
								}
							}			
							double dR = deltaRRecoSim(&kPlusTk , &tpr);			
							cout << "delta R: " << dR << endl;
							if(dR < deltaR_){
								trueKPlus = true;
							}
// 							for(std::vector<std::pair<TrackingParticleRef, double> >::const_iterator isim = simFound.begin(); isim != simFound.end(); isim++){
// 								if(isim->first == tpr){
// 									cout << "simpointer.second: " << isim->second << endl;
// 									trueKPlus = true;
// 								}
// 							}
						}
					} else {
						kMinusTk = kttkp;
						if(isSim_ && (simVertex != 0)){
//	 						simFound = (recSimColl)[ktk];
// 							cout << "found " << simFound.size() << " sim particles for kminus" << endl;
							TrackingParticleRef tpr;
							for (TrackingVertex::tp_iterator dt = simVertex->daughterTracks_begin(); dt!= simVertex->daughterTracks_end(); ++dt) {
								if((**dt).pdgId() == -321){
									tpr= *dt;
									break;
								}
							}			
							double dR = deltaRRecoSim(&kMinusTk , &tpr);			
							cout << "delta R: " << dR << endl;
							if(dR < deltaR_){
								trueKMinus = true;
							}
// 							for(std::vector<std::pair<TrackingParticleRef, double> >::const_iterator isim = simFound.begin(); isim != simFound.end(); isim++){
// 								if(isim->first == tpr){
// 									cout << "simpointer.second: " << isim->second << endl;
// 									trueKMinus = true;
// 								}
// 							}
						}
					}
				}//end loop over kaons
				
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
			
				//making particles
				vector<RefCountedKinematicParticle> allParticles;
				allParticles.push_back(pFactory.particle (muPlusTk,muon_mass,chi,ndf,muon_sigma));
				allParticles.push_back(pFactory.particle (muMinusTk,muon_mass,chi,ndf,muon_sigma));
			
				allParticles.push_back(pFactory.particle (kPlusTk,kaon_mass,chi,ndf,kaon_sigma));
				allParticles.push_back(pFactory.particle (kMinusTk,kaon_mass,chi,ndf,kaon_sigma));
			
				//creating the constraint for the J/Psi mass
				ParticleMass m_jpsi = 3.09687;
			
				//creating the two track mass constraint
				MultiTrackKinematicConstraint *  j_psi_c = new  TwoTrackMassKinematicConstraint(m_jpsi);
			
				//creating the fitter
				KinematicConstrainedVertexFitter kcvFitter;
			
				//obtaining the resulting tree
				RefCountedKinematicTree myTree = kcvFitter.fit(allParticles, j_psi_c);
// 				cout << "Global fit done:\n";
				
				// write the Bs in the root tree
				bsRootTree_->addDecay(myTree, primaryVertex);
				
				bsRootTree_->setAssociation(trueMuPlus && trueMuMinus && trueKPlus && trueKMinus);

				if(bsRootTree_->getAssociation()){
					bsRootTree_->addSimDecay(simVertex, &trackingTruthVertexCollection->front());
				}
				bsRootTree_->fill();
			}// end loop over Bs
		} 
		
  	} catch (std::exception & err) {
    cout
      << "Exception during event number: " << iEvent.id()
      << "\n" << err.what() << "\n";
  }
}


// void BsToJpsiPhiAnalysis::fillhistograms(const RefCountedKinematicTree& myTree) const
// {
// 		
// 	//accessing the tree components, move pointer to top
//   	myTree->movePointerToTheTop();
// 
// 	//We are now at the top of the decay tree getting the B_s reconstructed KinematicPartlcle
// 	RefCountedKinematicParticle b_s = myTree->currentParticle();
// 	double prob = TMath::Prob(b_s->chiSquared(), (int)b_s->degreesOfFreedom());
// 	h_fit_prob->Fill(prob);
// 	if(prob > 0.001){
// 		h_Bs_mass->Fill(b_s->currentState().mass());
// 		h_pT_Bs->Fill(b_s->currentState().globalMomentum().perp());
// 
// 		// The B_s decay vertex
// 		RefCountedKinematicVertex b_dec_vertex = myTree->currentDecayVertex();
// 		double l_xy = sqrt(b_dec_vertex->position().x()*b_dec_vertex->position().x() + b_dec_vertex->position().y()*b_dec_vertex->position().y());
// 		h_l_xy->Fill(l_xy);
// 		
// 		double proper_decay_lenth = l_xy * b_s->currentState().mass() / b_s->currentState().globalMomentum().perp();
// 		h_t_xy->Fill(proper_decay_lenth);
// 	}
// }

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
		
		// look if the mothers of the vertex are Bs, Jpsi and phi 
        if (v->genVertices().size() == 3) {
			for (TrackingVertex::genv_iterator  dt = v->genVertices_begin(); dt!= v->genVertices_end(); ++dt){
				if( ((**dt).mother()->pdg_id() == 531) || ((**dt).mother()->pdg_id() == -531) ){
					bs = true;
				}
				if( ((**dt).mother()->pdg_id() == 443) || ((**dt).mother()->pdg_id() == -443) ){
					jpsi = true;
				}
				if( ((**dt).mother()->pdg_id() == 333) || ((**dt).mother()->pdg_id() == -333) ){
					phi = true;
				}
			}
		}
		if(bs & jpsi & phi & kplus & kminus & muminus & muplus){ 
			return &(*v);
		}
	}
	return 0;
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
