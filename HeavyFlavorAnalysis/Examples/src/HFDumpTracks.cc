#include <iostream>

#include "HeavyFlavorAnalysis/Examples/interface/HFDumpTracks.h"

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

// -- Yikes!
extern TAna00Event *gHFEvent;

using namespace std;
using namespace edm;


// ----------------------------------------------------------------------
HFDumpTracks::HFDumpTracks(const edm::ParameterSet& iConfig):
  fTracksLabel(iConfig.getUntrackedParameter<string>("tracksLabel", string("ctfWithMaterialTracks"))),
  fAssociatorLabel(iConfig.getUntrackedParameter<string>("associatorLabel", string("TrackAssociatorByChi2"))), 
  fTrackingParticlesLabel(iConfig.getUntrackedParameter<string>("trackingParticlesLabel", string("trackingParticles"))) {
  cout << "----------------------------------------------------------------------" << endl;
  cout << "--- HFDumpTracks constructor" << endl;
  cout << "--- " << fTracksLabel.c_str() << endl;
  cout << "--- " << fAssociatorLabel.c_str() << endl;
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
  edm::Handle<TrackingParticleCollection> simTracks;
  iEvent.getByLabel(fTrackingParticlesLabel.c_str(), simTracks);
  reco::RecoToSimCollection recSimColl = fAssociator->associateRecoToSim(tracks, simTracks, &iEvent); 

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
      cout << "%%>   Rec. Track #" << setw(2) << rTrack.index() << " pT: " 
           << setprecision(2) << setw(6) << track.pt() 
           <<  " matched to 0 MC Tracks" << endl;
    }
    pTrack->fGenIndex = gen_id;
    pTrack->fMCID     = gen_pdg_id;
    pTrack->dump(); 
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
