#include <iostream>

#include "HeavyFlavorAnalysis/Examples/interface/HFDumpTracks.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Common/interface/Handle.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimTracker/TrackAssociation/interface/TrackAssociatorBase.h" 
#include "SimTracker/Records/interface/TrackAssociatorRecord.h"
#include "SimTracker/TrackAssociation/interface/TrackAssociatorByChi2.h"
#include "SimTracker/TrackAssociation/interface/TrackAssociatorByHits.h"
#include "SimDataFormats/Track/interface/SimTrack.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"

#include "DataFormats/TrackerRecHit2D/interface/SiTrackerGSRecHit2D.h" 

#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
#include "DataFormats/TrackReco/interface/TrackExtraFwd.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/TrackReco/interface/HitPattern.h"

#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAna01Event.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaTrack.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaCand.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TGenCand.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaVertex.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaMuon.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TTrgObj.hh"

#include <TFile.h>
#include <TH1.h>

// -- Yikes!
extern TAna01Event *gHFEvent;
extern TFile       *gHFFile;

using namespace std;
using namespace edm;
using namespace reco;


// ----------------------------------------------------------------------
HFDumpTracks::HFDumpTracks(const edm::ParameterSet& iConfig):
  fTracksLabel(iConfig.getUntrackedParameter<string>("tracksLabel", string("ctfWithMaterialTracks"))),
  fMuonsLabel(iConfig.getUntrackedParameter<InputTag>("muonsLabel")),
  fGenEventLabel(iConfig.getUntrackedParameter<string>("generatorEventLabel", string("source"))),
  fSimTracksLabel(iConfig.getUntrackedParameter<string>("simTracksLabel", string("famosSimHits"))),
  fAssociatorLabel(iConfig.getUntrackedParameter<string>("associatorLabel", string("TrackAssociatorByChi2"))), 
  fTrackingParticlesLabel(iConfig.getUntrackedParameter<string>("trackingParticlesLabel", string("trackingParticles"))),
  fVerbose(iConfig.getUntrackedParameter<int>("verbose", 0)),
  fDoTruthMatching(iConfig.getUntrackedParameter<int>("doTruthMatching", 1)) {
  cout << "----------------------------------------------------------------------" << endl;
  cout << "--- HFDumpTracks constructor" << endl;
  cout << "---  fTracksLabel:            " << fTracksLabel.c_str() << endl;
  cout << "---  fAssociatorLabel:        " << fAssociatorLabel.c_str() << endl;
  cout << "---  fDoTruthMatching:        " << fDoTruthMatching << endl;  // 0 = nothing, 1 = TrackingParticles, 2 = FAMOS
  cout << "---  fTrackingParticlesLabel: " << fTrackingParticlesLabel.c_str() << endl;
  cout << "----------------------------------------------------------------------" << endl;
}


// ----------------------------------------------------------------------
HFDumpTracks::~HFDumpTracks() {
  
}


// ----------------------------------------------------------------------
void HFDumpTracks::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  // -- get the collection of RecoTracks 
  edm::Handle<edm::View<reco::Track> > tracksView;
  iEvent.getByLabel(fTracksLabel.c_str(), tracksView);

  // -- get the collection of muons and store their corresponding track indices
  vector<int> muonIndices;
  Handle<MuonCollection> hMuons;
  cout << "==> HFDumpTracks> " << fMuonsLabel << endl;
  iEvent.getByLabel(fMuonsLabel, hMuons);
  const Track* tt = 0;
  for (MuonCollection::const_iterator muon = hMuons->begin(); muon != hMuons->end(); ++muon) {
    TrackRef track = muon->track();
    muonIndices.push_back((muon->track()).index());
  }
  if (fVerbose > 0) cout << "==>HFDumpTracks> nMuons = " << hMuons->size() << endl;
  TH1D *h2 = (TH1D*)gHFFile->Get("h2");
  if (h2) h2->Fill(hMuons->size());

 
  // -- get the tracking particle collection needed for truth matching. Only on RECO data tier?
  RecoToSimCollection recSimColl;
  const RecoToSimCollection recSimColl2;

  if (1 == fDoTruthMatching) {
    try {
      edm::Handle<TrackingParticleCollection> trackingParticles;
      iEvent.getByLabel(fTrackingParticlesLabel.c_str(), trackingParticles);
      recSimColl = fAssociator->associateRecoToSim(tracksView, trackingParticles, &iEvent);
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


  if (fVerbose > 0) cout << "===> Tracks " << tracksView->size() << endl;
  TAnaTrack *pTrack; 
  TH1D *h1 = (TH1D*)gHFFile->Get("h1");
  if (h1) h1->Fill(tracksView->size());
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
    pTrack->fLip = trackView.dsz();
    pTrack->fQ = trackView.charge();
    pTrack->fChi2 = trackView.chi2();
    pTrack->fDof = int(trackView.ndof());
    pTrack->fValidHits = trackView.numberOfValidHits();  
    pTrack->fMuID = 0.; 
    for (int im = 0; im < muonIndices.size(); ++im) {
      if (i == muonIndices[im]) {
	pTrack->fMuID = 1.;
	if (fVerbose > 0) cout << " ==>HFDumpTracks> Found a muon!!" << endl;
      }
    }

    // -- hits of the track
    const reco::HitPattern& p = trackView.hitPattern();
    for (int i=0; i<p.numberOfHits(); i++) {
      uint32_t hit = p.getHitPattern(i);
    }

    int gen_pdg_id(-99999), gen_id(-99999), gen_cnt(0);

    // -- simple truth matching
    //    this procedure can be redone offline on the ntuple level!
    if (0 == fDoTruthMatching) {
      gen_id     = gHFEvent->getGenIndex(trackView.px(), trackView.py(), trackView.pz(), -1); 
      if (gen_id > -1) gen_pdg_id = gHFEvent->getGenCand(gen_id)->fID; 
      if (13 == TMath::Abs(gen_pdg_id)) {
	cout << "Simple TM: "; pTrack->dump(); 
      }
    }

    // -- RECO truth matching with TrackingParticle
    if (1 == fDoTruthMatching) {
      try{ 
	cout << "hello 1" << endl;
	std::vector<std::pair<TrackingParticleRef, double> > tp = recSimColl[rTrackView];
	cout << "hello 2" << endl;
 	TrackingParticleRef tpr = tp.begin()->first;  
	cout << "hello 3" << endl;
 	const HepMC::GenParticle *genPar = 0; 
 	for (TrackingParticle::genp_iterator pit = tpr->genParticle_begin(); 
 	     pit != tpr->genParticle_end(); 
 	     ++pit){
	  cout << "hello 4" << endl;
 	  genPar     = pit->get();
 	  gen_pdg_id = (*genPar).pdg_id();
 	  gen_id     = (*genPar).barcode()-1;
 	  //gen_pt     = (*genPar).momentum().perp();
 	  //gen_phi    = (*genPar).momentum().phi();
 	  //gen_eta    = (*genPar).momentum().pseudoRapidity();
 	  gen_cnt++;
	}
      } catch (Exception event) {
	// 	cout << "%%>   Rec. Track #" << setw(2) << rTrack.index() << " pT: " 
	// 	     << setprecision(2) << setw(6) << trackView.pt() 
	// 	     <<  " matched to 0 MC Tracks" << endl;
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
	    
	    if (simTrack->trackId() == currentId) {
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
  cout << "fAssociator = " << fAssociator << endl;

  gHFFile->cd();
  TH1D *h1 = new TH1D("h2", "h2", 20, 0., 20.);

}

// ------------ method called once each job just after ending the event loop  ------------
void  HFDumpTracks::endJob() {
}

//define this as a plug-in
//DEFINE_FWK_MODULE(HFDumpTracks);
