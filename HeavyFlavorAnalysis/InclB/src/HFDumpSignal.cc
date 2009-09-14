#include <iostream>

#include "HeavyFlavorAnalysis/InclB/interface/HFDumpSignal.h"

#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAna00Event.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaTrack.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaCand.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TGenCand.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaVertex.hh"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/Wrapper.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

#include "DataFormats/JetReco/interface/BasicJetCollection.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "DataFormats/GeometryVector/interface/GlobalVector.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/IPTools/interface/IPTools.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"

#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "DataFormats/Candidate/interface/Candidate.h"

// -- Yikes!
extern TAna00Event *gHFEvent;

using namespace std;
using namespace reco;
using namespace edm;


// ----------------------------------------------------------------------
HFDumpSignal::HFDumpSignal(const edm::ParameterSet& iConfig) :
  fVerbose(iConfig.getUntrackedParameter<int>("verbose", 0)),
  fJetMatch(iConfig.getUntrackedParameter<double>("jetmatch", 0.5)),
  fJetEtMin(iConfig.getUntrackedParameter<double>("jetetmin", 1)),
  fMuonLabel(iConfig.getUntrackedParameter<string>("muonLabel", string("globalMuons"))),
  fJetsLabel(iConfig.getUntrackedParameter<string>("jetsLabel", string("ic5TrackJets"))),
  fTracksLabel(iConfig.getUntrackedParameter<string>("tracksLabel", string("trackCandidates"))),
  fVertexLabel(iConfig.getUntrackedParameter<string>("vertexLabel", string("offlinePrimaryVerticesWithBS")))   {
  using namespace std;
  cout << "----------------------------------------------------------------------" << endl;
  cout << "--- HFDumpSignal constructor" << endl;
  cout << "--- " << fMuonLabel.c_str() << endl;
  cout << "----------------------------------------------------------------------" << endl;

}


// ----------------------------------------------------------------------
HFDumpSignal::~HFDumpSignal() {
  
}


// ----------------------------------------------------------------------
void HFDumpSignal::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  nevt++;

  edm::Handle<reco::MuonCollection> muons;
  iEvent.getByLabel( fMuonLabel.c_str(), muons); 
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // handle to 0.5 cone ctf track jets  
  Handle<BasicJetCollection> jetsH;
  iEvent.getByLabel(fJetsLabel.c_str(),jetsH);
  const BasicJetCollection *jets   = jetsH.product();  
  //tracks (jet constituents)
  Handle<reco::CandidateView> candidates1Handle;
  iEvent.getByLabel(fTracksLabel.c_str(), candidates1Handle); 
  //primary vertex
  Handle<reco::VertexCollection> primaryVertex;
  iEvent.getByLabel(fVertexLabel.c_str(),primaryVertex); 
 
  edm::ESHandle<TransientTrackBuilder> builder;
  iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",builder); 

  std::vector<const Candidate *> trkcands;
  trkcands.clear();
  vector<const Candidate *>::const_iterator trkfound = trkcands.begin(); 
  //get tracks 
  for (unsigned int i = 0; i < candidates1Handle->size(); ++ i ) {
    const Candidate &p2 = (*candidates1Handle)[i];
    const Candidate *  p1 = &p2;
    trkcands.push_back( & * p1 );
  }
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

  
  TAnaTrack *pTrack;
 
  int index = 0;
  if (fVerbose > 0) cout << "==>HFDumpSignal> nMuons =" << muons->size() << endl;
  for (  reco::MuonCollection::const_iterator muon = muons->begin(); muon != muons->end(); ++ muon ) { 

    pTrack            = gHFEvent->addSigTrack();

    pTrack->fMCID     = 0;                            //muon type: global, standalone or tracker muon
    if (muon->isGlobalMuon())                       
      pTrack->fMCID   = 1;          
    else if (muon->isStandAloneMuon()) 
      pTrack->fMCID   = 2; 
    else if (muon->isTrackerMuon()) 
      pTrack->fMCID   = 3; 
    else if (muon->isCaloMuon()) 
      pTrack->fMCID   = 4; 
 
    pTrack->fMuType   = 0;                            //0=RECO, 1=L1, 2=HLTL2, 3=HLTL3 
    pTrack->fMuID     = (muon->track()).index();      //index of muon track in RECO track block
    pTrack->fIndex    = index;                        //index in muon block
    pTrack->fGenIndex = -1;                           //not used
    pTrack->fQ        = muon->charge();               //charge
    pTrack->fPlab.SetPtEtaPhi(muon->pt(),
			      muon->eta(),
			      muon->phi()
			      );
    
  
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    pTrack->fLip      = -9999;
    pTrack->fLipE     = -9999;
    pTrack->fTip      = -9999;
    pTrack->fTipE     = -9999;   // 3d and transverse impact parameters
    if (muon->isGlobalMuon() && (muon->track()).index() <= candidates1Handle->size()) {
      //signed transverse ip
      //get closest jet 
      double rmin = fJetMatch; 
      BasicJet* matchedjet;
      bool found = false;
      for ( BasicJetCollection::const_iterator it = jets->begin(); it != jets->end(); it ++ ) {
	TVector3 jetcand;
	jetcand.SetPtEtaPhi(it->pt(), it->eta(), it->phi());
	double r = (pTrack->fPlab).DeltaR(jetcand);
	
	if (r<rmin && it->et() > fJetEtMin) {
	  rmin    = r;
	  matchedjet   = (*it).clone();
	  found = true;
	}
      } 
      if (found) {//found a track jet
	
	//subtract muon 
	TLorentzVector vect;
	vect.SetPtEtaPhiE(matchedjet->pt(), matchedjet->eta(), matchedjet->phi(), matchedjet->energy());
	
	bool foundmuon = false;
	std::vector< const reco::Candidate * > Constituent = matchedjet->getJetConstituentsQuick();
	for (unsigned int i=0; i< Constituent.size(); i++) {
	  
	  unsigned int idx  = 99999;
	  const reco::Candidate * consti = Constituent[i];
	  if (consti) {
	    trkfound = find( trkcands.begin(), trkcands.end(), consti );
	    if ( trkfound != trkcands.end() ) {
	      idx = trkfound - trkcands.begin() ;
	    }
	    if (idx == (muon->track()).index()) {
	      foundmuon = true;
	    }
	  } 
	}
	
	
	if (foundmuon) {
	  TLorentzVector muvect;
	  muvect.SetPtEtaPhiM(muon->track()->pt(), muon->track()->eta(), muon->track()->phi(), mmuon);
	  vect = vect - muvect;
	}
	//define direction
	GlobalVector direction(vect.X(),vect.Y(),vect.Z());
	const TransientTrack & transientTrack = builder->build(&(*(muon->track()))); 
	const  Vertex  *pv;
	edm::Ref<VertexCollection> * pvRef;
	bool pvFound = (primaryVertex->size() != 0); 
	
	if(pvFound) {
	  
	  pv = &(*primaryVertex->begin());
	  pvRef = new edm::Ref<VertexCollection>(primaryVertex,0); // we always use the first vertex at the moment 
	  
	  pTrack->fLip      = IPTools::signedImpactParameter3D(transientTrack,direction,*pv).second.value();
	  pTrack->fLipE     = IPTools::signedImpactParameter3D(transientTrack, direction, *pv).second.error();
	  pTrack->fTip      = IPTools::signedTransverseImpactParameter(transientTrack, direction, *pv).second.value();
	  pTrack->fTipE     = IPTools::signedTransverseImpactParameter(transientTrack, direction, *pv).second.error();  // 3d and transverse impact parameters
	  
	 
	}
	
      }
    }
   
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
    
    if (fVerbose > 0) pTrack->dump(); 
    index++;
    
  }
  
  
}

// ------------ method called once each job just before starting event loop  ------------
void  HFDumpSignal::beginJob(const edm::EventSetup& setup) {
  nevt=0;
}

// ------------ method called once each job just after ending the event loop  ------------
void  HFDumpSignal::endJob() { 
  cout << "HFDumpSignal> Summary: Events processed: " << nevt << endl;
}

//define this as a plug-in
//DEFINE_FWK_MODULE(HFDumpSignal);
