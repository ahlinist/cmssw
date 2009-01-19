#include <iostream>

#include "HeavyFlavorAnalysis/InclB/interface/HFDumpTracks.h"

#include "DataFormats/Common/interface/Handle.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimTracker/Records/interface/TrackAssociatorRecord.h"
#include "SimTracker/TrackAssociation/interface/TrackAssociatorBase.h" 
#include "SimTracker/TrackAssociation/interface/TrackAssociatorByChi2.h"

#include "DataFormats/TrackReco/interface/TrackExtraFwd.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"

#include "FWCore/Framework/interface/ESHandle.h"

#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAna00Event.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaTrack.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaCand.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TGenCand.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaVertex.hh"

#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleCandidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "DataFormats/Common/interface/RefToBase.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"


// -- Yikes!
extern TAna00Event *gHFEvent;

using namespace std;
using namespace edm;
using namespace reco;


// ----------------------------------------------------------------------
HFDumpTracks::HFDumpTracks(const edm::ParameterSet& iConfig):
  fTracksLabel(iConfig.getUntrackedParameter<string>("tracksLabel", string("allTracks"))),
  fAssociatorLabel(iConfig.getUntrackedParameter<string>("associatorLabel", string("allTracksGenParticlesMatch"))), 
  fGenParticlesLabel(iConfig.getUntrackedParameter<string>("genParticlesLabel", string("genParticleCandidates"))) {
  cout << "----------------------------------------------------------------------" << endl;
  cout << "--- HFDumpTracks constructor" << endl;
  cout << "--- " << fTracksLabel.c_str() << endl;
  cout << "--- " << fAssociatorLabel.c_str() << endl;
  cout << "--- " << fGenParticlesLabel.c_str() << endl;
  cout << "----------------------------------------------------------------------" << endl;
}


// ----------------------------------------------------------------------
HFDumpTracks::~HFDumpTracks() {
  
}


// ----------------------------------------------------------------------
void HFDumpTracks::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  nevt++;

  // -- get the collection of RecoTracks 
  Handle<CandidateCollection> Tracks;
  iEvent.getByLabel(fTracksLabel.c_str(), Tracks); 

  Handle<CandMatchMap> TracksMap;
  iEvent.getByLabel(fAssociatorLabel.c_str(), TracksMap );

  //get genParticles to extract index
  edm::Handle<reco::CandidateCollection> particles;
  iEvent.getByLabel (fGenParticlesLabel.c_str(), particles );
  int gen_id = -99999; int gen_pdg_id = -99999;
  std::vector<const reco::Candidate *> cands_;
  cands_.clear();
  std::vector<const reco::Candidate *>::const_iterator found = cands_.begin();
  for( reco::CandidateCollection::const_iterator p = particles->begin(); p != particles->end(); ++ p ) {
    cands_.push_back( & * p );
  } 
 
  //loop over tracks
  TAnaTrack *aTrack; 
  int i=0;
  for( reco::CandidateCollection::const_iterator t = Tracks->begin(); t != Tracks->end(); ++ t ) {
  
    CandidateRef cand(Tracks, i);
    TrackRef trk = t->get<TrackRef>();
    aTrack = gHFEvent->addRecTrack();
    aTrack->fIndex = i;
    aTrack->fPlab.SetPtEtaPhi(trk->pt(),
			      trk->eta(),
			      trk->phi()
			      );

    aTrack->fTip = trk->d0();
    aTrack->fTipE = trk->d0Error();
    aTrack->fLip = trk->dz();
    aTrack->fLipE = trk->dzError();
    aTrack->fQ = trk->charge();
    aTrack->fChi2 = trk->chi2();
    aTrack->fDof = int(trk->ndof());
    aTrack->fHits = trk->numberOfValidHits(); 


    //MC matching (delta R)
    gen_id     = -99999;
    gen_pdg_id = -99999;
    CandMatchMap::const_iterator m = TracksMap->find(cand);
    if( m != TracksMap->end() ) {
      const Candidate *part  = &*(m->val);
      gen_pdg_id  = part->pdgId();
      //extract index
      found = find( cands_.begin(), cands_.end(), part );
      if ( found != cands_.end() ) gen_id = found - cands_.begin() ;
    }
    //else cout << "no match " << endl;

    aTrack->fGenIndex = gen_id;
    aTrack->fMCID     = gen_pdg_id;
  
    //aTrack->dump();

    i++;  
  } 
 
 
  /*edm::Handle<reco::TrackCollection> tracks;
  iEvent.getByLabel("ctfWithMaterialTracks", tracks);
  
  // -- get the tracking particle collection needed for truth matching. Only on RECO data tier!
  edm::Handle<TrackingParticleCollection> simTracks;
  iEvent.getByLabel("trackingParticles", simTracks);
  reco::RecoToSimCollection recSimColl = fAssociator->associateRecoToSim(tracks, simTracks, &iEvent); 

  TAnaTrack *pTrack; 
  int index=0;
  for (unsigned int i = 0; i < tracks->size(); ++i){    
    reco::TrackRef rTrack(tracks, i);
    reco::Track track(*rTrack);

    pTrack = gHFEvent->addRecTrack();
    pTrack->fIndex = rTrack.index();
    pTrack->fPlab.SetPtEtaPhi(track.pt(),
			      track.eta(),
			      track.phi()
			      );
    pTrack->fTip = track.d0();
    pTrack->fTipE = track.d0Error();
    pTrack->fLip = track.dz();
    pTrack->fLipE = track.dzError();
    pTrack->fQ = track.charge();
    pTrack->fChi2 = track.chi2();
    pTrack->fDof = int(track.ndof());
    pTrack->fHits = track.numberOfValidHits();  

    int gen_pdg_id(-99999), gen_id(-99999), gen_cnt(0);
    try{ 
      std::vector<std::pair<TrackingParticleRef, double> > tp = recSimColl[rTrack];
      TrackingParticleRef tpr = tp.begin()->first;  // ??? This takes the first associated simulated track ???        
      const HepMC::GenParticle *genPar = 0; 
      for (TrackingParticle::genp_iterator pit = tpr->genParticle_begin(); 
	   pit != tpr->genParticle_end(); 
	   ++pit){
	genPar     = pit->get();
	gen_pdg_id = (*genPar).pdg_id();
	gen_id     = (*genPar).barcode()-1;
	//gen_pt     = (*genPar).momentum().perp();
	//gen_phi    = (*genPar).momentum().phi();
	//gen_eta    = (*genPar).momentum().pseudoRapidity();
	gen_cnt++;
      }
    } catch (Exception event) {
      //cout << "%%>   Rec. Track #" << setw(2) << rTrack.index() << " pT: "<< setprecision(2) << setw(6) << track.pt() <<  " matched to 0 MC Tracks" << endl;
    }
    pTrack->fGenIndex = gen_id;
    pTrack->fMCID     = gen_pdg_id;
    //if (index==0) cout << "===> Tracks " << endl;
    pTrack->dump(); 
    index++;
    }*/
  

}

// ------------ method called once each job just before starting event loop  ------------
void  HFDumpTracks::beginJob(const edm::EventSetup& setup) {
  //edm::ESHandle<TrackAssociatorBase> theAssociator;
  //setup.get<TrackAssociatorRecord>().get("TrackAssociatorByChi2", theAssociator);
  //fAssociator = (TrackAssociatorBase*)theAssociator.product();

  nevt = 0;
}

// ------------ method called once each job just after ending the event loop  ------------
void  HFDumpTracks::endJob() {

  cout << "HFDumpTracks> Summary: Events processed: " << nevt << endl;
}

//define this as a plug-in
//DEFINE_FWK_MODULE(HFDumpTracks);
