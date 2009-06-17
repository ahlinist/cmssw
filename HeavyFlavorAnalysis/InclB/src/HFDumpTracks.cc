#include <iostream>

#include "HeavyFlavorAnalysis/InclB/interface/HFDumpTracks.h"

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
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

#include "FWCore/Framework/interface/ESHandle.h"

#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAna00Event.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaTrack.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaCand.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TGenCand.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaVertex.hh"

#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "DataFormats/Common/interface/RefToBase.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h" 

#include "CommonTools/TrackerMap/interface/TrackerMap.h"


#include <TFile.h>
#include <TH1.h>

// -- Yikes!
extern TAna00Event *gHFEvent;

using namespace std;
using namespace edm;
using namespace reco;


// ----------------------------------------------------------------------
HFDumpTracks::HFDumpTracks(const edm::ParameterSet& iConfig):
  fTracksLabel(iConfig.getUntrackedParameter<string>("tracksLabel", string("generalTracks"))),
  fTrackCandsLabel(iConfig.getUntrackedParameter<string>("trackcandsLabel", string("allTracks"))), //aod matching
  fGenEventLabel(iConfig.getUntrackedParameter<string>("generatorEventLabel", string("source"))),//famos matching
  fGenParticlesLabel(iConfig.getUntrackedParameter<string>("genParticlesLabel", string("genParticles"))), //aod matching
  fSimTracksLabel(iConfig.getUntrackedParameter<string>("simTracksLabel", string("famosSimHits"))),//famos matching
  fAssociatorLabel(iConfig.getUntrackedParameter<string>("associatorLabel", string("TrackAssociatorByChi2"))),//reco matching
  fAssociatorLabel3(iConfig.getUntrackedParameter<string>("associatorLabel3", string("allTracksGenParticlesMatch"))), //aod matching
  fTrackingParticlesLabel(iConfig.getUntrackedParameter<string>("trackingParticlesLabel", string("trackingParticles"))),//reco matching
  fMuonsLabel(iConfig.getUntrackedParameter<InputTag>("muonsLabel")),
  fVerbose(iConfig.getUntrackedParameter<int>("verbose", 0)),
  fDoTruthMatching(iConfig.getUntrackedParameter<int>("doTruthMatching", 1)) {
  cout << "----------------------------------------------------------------------" << endl;
  cout << "--- HFDumpTracks constructor" << endl;
  cout << "--- " << fVerbose << endl;
  cout << "--- " << fTracksLabel.c_str() << endl;
  cout << "--- " << fDoTruthMatching << endl;  // 0 = nothing, 1 = TrackingParticles, 2 = FAMOS, 3 = AOD
  cout << "----------------------------------------------------------------------" << endl;
}


// ----------------------------------------------------------------------
HFDumpTracks::~HFDumpTracks() {
  
}


// ----------------------------------------------------------------------
void HFDumpTracks::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) { 

  static int nevt(0); 
  ++nevt;
  if (fVerbose>0) cout << "==>HFDumpTracks> new  event:  " << nevt << endl; 

  // -- get the collection of RecoTracks 
  edm::Handle<edm::View<reco::Track> > tracksView;
  iEvent.getByLabel(fTracksLabel.c_str(), tracksView);

  // -- get the collection of muons and store their corresponding track indices
  vector<int> muonIndices;
  Handle<MuonCollection> hMuons;
  iEvent.getByLabel(fMuonsLabel, hMuons);

  for (MuonCollection::const_iterator muon = hMuons->begin(); muon != hMuons->end(); ++muon) {
    TrackRef track = muon->track();
    muonIndices.push_back((muon->track()).index());
  }
  if (fVerbose > 0) cout << "==>HFDumpTracks> nMuons = " << hMuons->size() << endl;
 

  // -- get the tracking particle collection needed for truth matching. Only on RECO data tier!
  RecoToSimCollection recSimColl;
  const RecoToSimCollection recSimColl2;
  bool tp = false;
  if (1 == fDoTruthMatching) {
    try {
      edm::Handle<TrackingParticleCollection> trackingParticles;
      iEvent.getByLabel(fTrackingParticlesLabel.c_str(), trackingParticles);
      recSimColl = fAssociator->associateRecoToSim(tracksView, trackingParticles, &iEvent);
      tp = true;
    } catch (cms::Exception &ex) {
      gHFEvent->fError = gHFEvent->fError + 16;
      if (fVerbose > 0) cout << "==>HFDumpTracks>ERROR: no TrackingParticles in the event (fError=" << gHFEvent->fError << ")" << endl;  
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

  

  //*****************
  // -- Get the stuff needed for truth matching on AOD
  Handle<GenParticleMatch> MatchMap;
  Handle<GenParticleCollection> genParticlesH;
  Handle<reco::CandidateView> Tracks;
  std::vector<const reco::Candidate *> cands;
  cands.clear(); 
  std::vector<const reco::Candidate *>::const_iterator found = cands.begin();
  if (3 == fDoTruthMatching) {
    iEvent.getByLabel(fAssociatorLabel3.c_str(), MatchMap );
    iEvent.getByLabel (fGenParticlesLabel.c_str(), genParticlesH );
    iEvent.getByLabel(fTrackCandsLabel.c_str(), Tracks); 
    for (GenParticleCollection::const_iterator p = genParticlesH->begin(); p != genParticlesH->end(); ++p) {
      cands.push_back( & * p );
    }
  }
  //*******************

  
  if (fVerbose > 0) cout << "===> Tracks " << tracksView->size() << endl;
  TAnaTrack *pTrack; 

  bool first = true;
  for (unsigned int i = 0; i < tracksView->size(); ++i){    

    TrackBaseRef rTrackView(tracksView,i);
    Track trackView(*rTrackView);

    pTrack = gHFEvent->addRecTrack();
    pTrack->fIndex = i;
    pTrack->fPlab.SetPtEtaPhi(trackView.pt(),
			      trackView.eta(),
			      trackView.phi()
			      );
    pTrack->fTip = trackView.d0();
    pTrack->fLip = trackView.dz();
    pTrack->fQ = trackView.charge();
    pTrack->fChi2 = trackView.chi2();
    pTrack->fDof = int(trackView.ndof());
    pTrack->fHits = trackView.numberOfValidHits();  
    pTrack->fMuID = 0.; 
    for (unsigned int im = 0; im < muonIndices.size(); ++im) {
      if (int(i) == muonIndices[im]) {
	pTrack->fMuID = 1.;
	if (fVerbose > 0) cout << " ==>HFDumpTracks> Found a muon!!" << endl;
      }
    }

    int gen_pdg_id(-99999), gen_id(-99999), gen_cnt(0);

    // -- RECO truth matching with TrackingParticle
    if (1 == fDoTruthMatching && tp) {
      try{ 
	std::vector<std::pair<TrackingParticleRef, double> > tp = recSimColl[rTrackView];
	TrackingParticleRef tpr = tp.begin()->first;  
 	const HepMC::GenParticle *genPar = 0; 
 	for (TrackingParticle::genp_iterator pit = tpr->genParticle_begin(); 
 	     pit != tpr->genParticle_end(); 
 	     ++pit){
 	  genPar     = pit->get();
 	  gen_pdg_id = (*genPar).pdg_id();
 	  gen_id     = (*genPar).barcode()-1;
  	  gen_cnt++;
	  if (fVerbose > 0)
	    cout << "match: " << gen_id << " (pdgID " << gen_pdg_id << ")" << endl;
	}
      } catch (Exception event) {
	if (first) {
	  gHFEvent->fError = gHFEvent->fError + 32;
	  if (fVerbose > 0) cout << "==>HFDumpTracks>ERROR: matching fails (fError=" << gHFEvent->fError << ")" << endl;
	}
	first = false;
      }
    }

    // -- FAMOS truth matching via SimHit
    if (2 == fDoTruthMatching) {
      for (trackingRecHit_iterator it = trackView.recHitsBegin();  it != trackView.recHitsEnd(); it++) {
	if ((*it)->isValid()) {
	  
	  int currentId(-1);
	  if (const SiTrackerGSRecHit2D *rechit = dynamic_cast<const SiTrackerGSRecHit2D *> (it->get())) {
	    currentId = rechit->simtrackId();          
	  }
	  
	  for (SimTrackContainer::const_iterator simTrack = simTracks->begin(); 
	       simTrack != simTracks->end(); 
	       simTrack++)   { 
	    
	    if (int(simTrack->trackId()) == currentId) {
	      int igen = simTrack->genpartIndex();
	      HepMC::GenParticle *genPar = genEvent->barcode_to_particle(igen);
	      if (genPar) {
		gen_pdg_id = (*genPar).pdg_id();
		gen_id     = (*genPar).barcode()-1;  // BC(HepMC) = BC(reco)+1
		if (fVerbose > 0) printf("id = %i, bc = %i\n", gen_pdg_id, gen_id);
		goto done;
	      }
	    }
	  }
	}
      }
    done:;
    }



    // -- AOD truth matching with GenParticles
    if (3 == fDoTruthMatching) {
      //*****************
      CandidateBaseRef ref = Tracks->refAt(i);
      GenParticleRef mc = (*MatchMap)[ref];
      const Candidate *  cand = mc.get();
      if( !mc.isNull() ) {
	gen_pdg_id  = mc->pdgId();
	//extract index
	found = find( cands.begin(), cands.end(), cand );
	if ( found != cands.end() ) gen_id = found - cands.begin() ;
      }
      //****************************


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
