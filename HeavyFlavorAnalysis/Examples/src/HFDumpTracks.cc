#include <iostream>

#include "HeavyFlavorAnalysis/Examples/interface/HFDumpTracks.h"

#include "DataFormats/Common/interface/Handle.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimTracker/Records/interface/TrackAssociatorRecord.h"
#include "SimTracker/TrackAssociation/interface/TrackAssociatorBase.h" 
#include "SimTracker/TrackAssociation/interface/TrackAssociatorByChi2.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "SimDataFormats/Track/interface/SimTrack.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"

#include "DataFormats/TrackerRecHit2D/interface/SiTrackerGSRecHit2D.h" 
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
#include "DataFormats/TrackReco/interface/TrackExtraFwd.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"

#include "FWCore/Framework/interface/ESHandle.h"

#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAna00Event.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaTrack.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaCand.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TGenCand.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaVertex.hh"

// -- Yikes!
extern TAna00Event *gHFEvent;

using namespace std;
using namespace edm;


// ----------------------------------------------------------------------
HFDumpTracks::HFDumpTracks(const edm::ParameterSet& iConfig):
  fTracksLabel(iConfig.getUntrackedParameter<string>("tracksLabel", string("ctfWithMaterialTracks"))),
  fGenEventLabel(iConfig.getUntrackedParameter<string>("generatorEventLabel", string("source"))),
  fSimTracksLabel(iConfig.getUntrackedParameter<string>("simTracksLabel", string("famosSimHits"))),
  fAssociatorLabel(iConfig.getUntrackedParameter<string>("associatorLabel", string("TrackAssociatorByChi2"))), 
  fTrackingParticlesLabel(iConfig.getUntrackedParameter<string>("trackingParticlesLabel", string("trackingParticles"))),
  fVerbose(iConfig.getUntrackedParameter<int>("verbose", 0)),
  fDoTruthMatching(iConfig.getUntrackedParameter<int>("doTruthMatching", 1)) {
  cout << "----------------------------------------------------------------------" << endl;
  cout << "--- HFDumpTracks constructor" << endl;
  cout << "--- " << fTracksLabel.c_str() << endl;
  cout << "--- " << fAssociatorLabel.c_str() << endl;
  cout << "--- " << fDoTruthMatching << endl;  // 0 = nothing, 1 = TrackingParticles, 2 = FAMOS
  cout << "--- " << fTrackingParticlesLabel.c_str() << endl;
  cout << "----------------------------------------------------------------------" << endl;
}


// ----------------------------------------------------------------------
HFDumpTracks::~HFDumpTracks() {
  
}


// ----------------------------------------------------------------------
void HFDumpTracks::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  // -- get the collection of RecoTracks 
  edm::Handle<reco::TrackCollection> tracks;
  iEvent.getByLabel(fTracksLabel.c_str(), tracks);  
  
  // -- get the tracking particle collection needed for truth matching. Only on RECO data tier!
  reco::RecoToSimCollection recSimColl;
  if (1 == fDoTruthMatching) {
    try {
      edm::Handle<TrackingParticleCollection> trackingParticles;
      iEvent.getByLabel(fTrackingParticlesLabel.c_str(), trackingParticles);
      recSimColl = fAssociator->associateRecoToSim(tracks, trackingParticles, &iEvent); 
    } catch (cms::Exception &ex) {
      cout << ex.explainSelf() << endl;
    }
  }

  // -- Get the stuff needed for FAMOS truth matching
  Handle<HepMCProduct> hepmc;
  const HepMC::GenEvent *genEvent;
  edm::Handle<std::vector<SimTrack> > simTracks;
  if (2 == fDoTruthMatching) {
    iEvent.getByLabel(fGenEventLabel.c_str(), hepmc);
    genEvent = hepmc->GetEvent();
    iEvent.getByLabel(fSimTracksLabel.c_str(), simTracks); 
  }      


  if (fVerbose > 0) cout << "===> Tracks " << endl;
  TAnaTrack *pTrack; 
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
    pTrack->fLip = track.dz();
    pTrack->fQ = track.charge();
    pTrack->fChi2 = track.chi2();
    pTrack->fDof = int(track.ndof());
    pTrack->fHits = track.numberOfValidHits();  

    int gen_pdg_id(-99999), gen_id(-99999), gen_cnt(0);

    // -- RECO truth matching with TrackingParticle
    if (1 == fDoTruthMatching) {
      try{ 
	std::vector<std::pair<TrackingParticleRef, double> > tp = recSimColl[rTrack];
	TrackingParticleRef tpr = tp.begin()->first;  
	const HepMC::GenParticle *genPar = 0; 
	for (TrackingParticle::genp_iterator pit = tpr->genParticle_begin(); 
	     pit != tpr->genParticle_end(); 
	     ++pit){
	  cout << ". ";
	  genPar     = pit->get();
	  gen_pdg_id = (*genPar).pdg_id();
	  gen_id     = (*genPar).barcode()-1;
	  //gen_pt     = (*genPar).momentum().perp();
	  //gen_phi    = (*genPar).momentum().phi();
	  //gen_eta    = (*genPar).momentum().pseudoRapidity();
	  gen_cnt++;
	}
	cout << endl;
      } catch (Exception event) {
	cout << "%%>   Rec. Track #" << setw(2) << rTrack.index() << " pT: " 
	     << setprecision(2) << setw(6) << track.pt() 
	     <<  " matched to 0 MC Tracks" << endl;
      }
    }

    // -- FAMOS truth matching via SimHit
    if (2 == fDoTruthMatching) {
      for (trackingRecHit_iterator it = track.recHitsBegin();  it != track.recHitsEnd(); it++) {
	if ((*it)->isValid()) {

	  int currentId(-1);
	  if (const SiTrackerGSRecHit2D *rechit = dynamic_cast<const SiTrackerGSRecHit2D *> (it->get())) {
            currentId = rechit->simtrackId();          
	  }
	  
	  for (SimTrackContainer::const_iterator simTrack = simTracks->begin(); 
	       simTrack != simTracks->end(); 
	       simTrack++)   { 

	    if (simTrack->trackId() == currentId) {
	      int igen = simTrack->genpartIndex();
	      HepMC::GenParticle *genPar = genEvent->barcode_to_particle(igen);
	      if (genPar) {
		gen_pdg_id = (*genPar).pdg_id();
		gen_id     = (*genPar).barcode()-1;
		goto done;
	      }
	    }
	  }
	}
      }
    done:;
    }            
      
    pTrack->fGenIndex = gen_id;
    pTrack->fMCID     = gen_pdg_id;
    if (fVerbose > 0) pTrack->dump(); 
  }
  

}

// ------------ method called once each job just before starting event loop  ------------
void  HFDumpTracks::beginJob(const edm::EventSetup& setup) {
  edm::ESHandle<TrackAssociatorBase> theAssociator;
  setup.get<TrackAssociatorRecord>().get(fAssociatorLabel.c_str(), theAssociator);
  fAssociator = (TrackAssociatorBase*)theAssociator.product();
}

// ------------ method called once each job just after ending the event loop  ------------
void  HFDumpTracks::endJob() {
}

//define this as a plug-in
//DEFINE_FWK_MODULE(HFDumpTracks);
