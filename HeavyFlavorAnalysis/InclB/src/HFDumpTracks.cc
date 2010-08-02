#include <iostream>

#include "HeavyFlavorAnalysis/InclB/interface/HFDumpTracks.h"

#include "DataFormats/Common/interface/Handle.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimTracker/Records/interface/TrackAssociatorRecord.h"
#include "SimTracker/TrackAssociation/interface/TrackAssociatorBase.h" 
#include "SimTracker/TrackAssociation/interface/TrackAssociatorByChi2.h"
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

#include "HeavyFlavorAnalysis/InclB/rootio/TAna00Event.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TAnaTrack.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TAnaCand.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TGenCand.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TAnaVertex.hh"

#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "DataFormats/Common/interface/RefToBase.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h" 

#include "CommonTools/TrackerMap/interface/TrackerMap.h"

#include "DataFormats/JetReco/interface/BasicJetCollection.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "DataFormats/GeometryVector/interface/GlobalVector.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/IPTools/interface/IPTools.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"


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
  fTracksLabel2(iConfig.getUntrackedParameter<string>("trackcandsLabel2", string("alltrackCandidates"))),
  fTrackCandsLabel(iConfig.getUntrackedParameter<string>("trackcandsLabel", string("allTracks"))), //aod matching
  fGenParticlesLabel(iConfig.getUntrackedParameter<string>("genParticlesLabel", string("genParticles"))), //aod matching
  fAssociatorLabel3(iConfig.getUntrackedParameter<string>("associatorLabel3", string("allTracksGenParticlesMatch"))), //aod matching
  fTrackingParticlesLabel(iConfig.getUntrackedParameter<string>("trackingParticlesLabel", string("trackingParticles"))),//reco matching 
  fVertexLabel(iConfig.getUntrackedParameter<string>("vertexLabel", string("offlinePrimaryVerticesWithBS"))), 
  fJetsLabel(iConfig.getUntrackedParameter<string>("jetsLabel", string("sis5TrackJets"))),
  fMuonsLabel(iConfig.getUntrackedParameter<InputTag>("muonsLabel")),
  fVerbose(iConfig.getUntrackedParameter<int>("verbose", 0)),
  fDoTruthMatching(iConfig.getUntrackedParameter<int>("doTruthMatching", 1)) {
  cout << "----------------------------------------------------------------------" << endl;
  cout << "--- HFDumpTracks constructor" << endl;
  cout << "--- " << fVerbose << endl;
  cout << "--- " << fTracksLabel.c_str() << endl;
  cout << "--- " << fDoTruthMatching << endl;  // 0 = nothing, 1 = TrackingParticles, 2 = AOD
  cout << "----------------------------------------------------------------------" << endl;
}


// ----------------------------------------------------------------------
HFDumpTracks::~HFDumpTracks() {
  
}


// ----------------------------------------------------------------------
void HFDumpTracks::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) { 

  
  nevt++;
  if (fVerbose>0) cout << "==>HFDumpTracks> new  event:  " << nevt << endl; 

  // -- get the collection of RecoTracks 
  edm::Handle<edm::View<reco::Track> > tracksView;
  iEvent.getByLabel(fTracksLabel.c_str(), tracksView); 

  Handle<reco::TrackCollection> recTrks;
  //iEvent.getByLabel("generalTracks", recTrks);
  iEvent.getByLabel(fTracksLabel.c_str(), recTrks);

  // -- get the collection of muons and store their corresponding track indices
  vector<int> muonIndices;
  Handle<MuonCollection> hMuons;
  iEvent.getByLabel(fMuonsLabel, hMuons);

  for (MuonCollection::const_iterator muon = hMuons->begin(); muon != hMuons->end(); ++muon) {
    if (muon->isGlobalMuon() ) {
      TrackRef track = muon->track();
      muonIndices.push_back((muon->track()).index());
    }
  }
  if (fVerbose > 0) cout << "==>HFDumpTracks> nMuons = " << hMuons->size() << endl;
  
  
  //*****************
  // -- Get the stuff needed for truth matching on AOD
  Handle<GenParticleMatch> MatchMap;
  Handle<GenParticleCollection> genParticlesH;
  Handle<reco::CandidateView> Tracks;
  std::vector<const reco::Candidate *> cands;
  cands.clear(); 
  std::vector<const reco::Candidate *>::const_iterator found = cands.begin();
  if (2 == fDoTruthMatching) {
    iEvent.getByLabel(fAssociatorLabel3.c_str(), MatchMap );
    iEvent.getByLabel (fGenParticlesLabel.c_str(), genParticlesH );
    iEvent.getByLabel(fTrackCandsLabel.c_str(), Tracks); 
    for (GenParticleCollection::const_iterator p = genParticlesH->begin(); p != genParticlesH->end(); ++p) {
      cands.push_back( & * p );
    }
  }
  //*******************
  //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
  //signed impact parameter
  //primary vertex
  Handle<reco::VertexCollection> primaryVertex;
  iEvent.getByLabel(fVertexLabel.c_str(),primaryVertex);
  //std::vector<Point> points;
  //selectVertices(*primaryVertex, points);
  
  edm::ESHandle<TransientTrackBuilder> builder;
  iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",builder);
  //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

  // handle to 0.5 cone ctf track jets  
  Handle<BasicJetCollection> jetsH;
  iEvent.getByLabel(fJetsLabel.c_str(),jetsH);
  const BasicJetCollection *jets   = jetsH.product(); 
  //tracks (jet constituents)
  Handle<reco::CandidateView> candidates1Handle;
  iEvent.getByLabel(fTracksLabel2.c_str(), candidates1Handle); 
  
  //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

 
  
  if (fVerbose > 0) cout << "===> Tracks " << tracksView->size() << endl;
  TAnaTrack *pTrack; 

 
  reco::TrackCollection::const_iterator t=recTrks->begin();
  

  for (unsigned int i = 0; i < tracksView->size(); ++i){    

    TrackBaseRef rTrackView(tracksView,i);
    Track trackView(*rTrackView);
    //track candidate
    const Candidate &trkcand2 = (*candidates1Handle)[i];
    const Candidate *  trkcand = &trkcand2;

    pTrack = gHFEvent->addRecTrack();
    pTrack->fIndex = i;
    pTrack->fPlab.SetPtEtaPhi(trackView.pt(),
			      trackView.eta(),
			      trackView.phi()
			      );

  
    pTrack->fHighPurity = t->quality(reco::TrackBase::highPurity);     
    pTrack->fQ = trackView.charge();
    pTrack->fChi2 = trackView.chi2();
    pTrack->fDof = int(trackView.ndof());
    pTrack->fHits = trackView.numberOfValidHits();  

    pTrack->fMuonCSCHits = trackView.hitPattern().numberOfValidMuonCSCHits();
    pTrack->fMuonDTHits  = trackView.hitPattern().numberOfValidMuonDTHits();
    pTrack->fMuonRPCHits = trackView.hitPattern().numberOfValidMuonRPCHits();
    pTrack->fMuonHits    = trackView.hitPattern().numberOfValidMuonHits();
    
    pTrack->fBPIXHits    = trackView.hitPattern().numberOfValidPixelBarrelHits(); 
    pTrack->fFPIXHits    = trackView.hitPattern().numberOfValidPixelEndcapHits();
    pTrack->fPixelHits   = trackView.hitPattern().numberOfValidPixelHits();
    
    pTrack->fStripHits   = trackView.hitPattern().numberOfValidStripHits(); 
    pTrack->fTECHits     = trackView.hitPattern().numberOfValidStripTECHits(); 
    pTrack->fTIBHits     = trackView.hitPattern().numberOfValidStripTIBHits();
    pTrack->fTIDHits     = trackView.hitPattern().numberOfValidStripTIDHits();
    pTrack->fTOBHits     = trackView.hitPattern().numberOfValidStripTOBHits(); 
    
    pTrack->fBPIXLayers  = trackView.hitPattern().pixelBarrelLayersWithMeasurement();  
    pTrack->fFPIXLayers  = trackView.hitPattern().pixelEndcapLayersWithMeasurement();  
    pTrack->fPixelLayers = trackView.hitPattern().pixelLayersWithMeasurement();  
    pTrack->fStripLayers = trackView.hitPattern().stripLayersWithMeasurement(); 
    pTrack->fTECLayers   = trackView.hitPattern().stripTECLayersWithMeasurement();
    pTrack->fTIBLayers   = trackView.hitPattern().stripTIBLayersWithMeasurement(); 
    pTrack->fTIDLayers   = trackView.hitPattern().stripTIDLayersWithMeasurement();
    pTrack->fTOBLayers   = trackView.hitPattern().stripTOBLayersWithMeasurement();

    pTrack->fMuID = 0.; 
    for (unsigned int im = 0; im < muonIndices.size(); ++im) {
      if (int(i) == muonIndices[im]) {
	pTrack->fMuID = 1.;
	if (fVerbose > 0) cout << " ==>HFDumpTracks> Found a muon!!" << endl;
      }
    }

    int gen_pdg_id(-99999), gen_id(-99999);

    // -- AOD truth matching with GenParticles
    if (2 == fDoTruthMatching) {
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

    //signed impact parameter 
    BasicJet* matchedjet;
    bool foundjet = false;
    //loop over all track jets
    int jetidx=0;
    for ( BasicJetCollection::const_iterator it = jets->begin(); it != jets->end(); it ++ ) { 
      
      std::vector< const reco::Candidate * > Constituent = it->getJetConstituentsQuick();
      //loop over tracks in track jet
      for (unsigned int j=0; j< Constituent.size(); j++) {
	const reco::Candidate * consti = Constituent[j];
	if (consti && consti->pt() == trkcand->pt() &&  consti->phi() == trkcand->phi() &&  consti->eta() == trkcand->eta() ) {
	  //found a track jet to which the track belongs->use this jet when calculating the IP
	  matchedjet   = (*it).clone(); 
	  foundjet = true;
	  if (fVerbose > 0) cout << "track " << i << " belongs to jet " << jetidx << endl;
	}
      } 
      jetidx++;
    }

    if (foundjet) {
      TLorentzVector vect;
      vect.SetPtEtaPhiE(matchedjet->pt(), matchedjet->eta(), matchedjet->phi(), matchedjet->energy());
      GlobalVector direction(vect.X(),vect.Y(),vect.Z());
      
      const TransientTrack & transientTrack = builder->build(&(*t));
      const  Vertex  *pv;
      bool pvFound = (primaryVertex->size() != 0);
      if(pvFound) {
	pv = &(*primaryVertex->begin());
	pTrack->fTip      = IPTools::signedTransverseImpactParameter(transientTrack, direction, *pv).second.value();
	pTrack->fTipE     = IPTools::signedTransverseImpactParameter(transientTrack, direction, *pv).second.error();  // 3d and transverse impact parameters
	pTrack->fTip3d    = IPTools::signedImpactParameter3D(transientTrack,direction,*pv).second.value();
	pTrack->fTip3dE   = IPTools::signedImpactParameter3D(transientTrack, direction, *pv).second.error();


	pTrack->fLip = trackView.dz((pv->position())); //re-evaluate the dz with respect to the vertex position
	pTrack->fLipE  =  trackView.dzError();

      }
    }
   
    
    t++;  
  }
  
  
}

// ------------ method called once each job just before starting event loop  ------------
//void  HFDumpTracks::beginJob(const edm::EventSetup& setup) {
void  HFDumpTracks::beginJob() {

 
  nevt = 0;
}

// ------------ method called once each job just after ending the event loop  ------------
void  HFDumpTracks::endJob() {
  cout << "HFDumpTracks>     Summary: Events processed: " << nevt << endl;
}







//define this as a plug-in
DEFINE_FWK_MODULE(HFDumpTracks);
