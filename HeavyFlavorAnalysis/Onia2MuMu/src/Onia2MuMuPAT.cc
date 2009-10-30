#include "HeavyFlavorAnalysis/Onia2MuMu/interface/Onia2MuMuPAT.h"

//Headers for the data items
#include <DataFormats/TrackReco/interface/Track.h>
#include <DataFormats/TrackReco/interface/TrackFwd.h>
#include <DataFormats/MuonReco/interface/Muon.h>
#include <DataFormats/MuonReco/interface/MuonFwd.h>
#include <DataFormats/Candidate/interface/LeafCandidate.h>
#include <DataFormats/HepMCCandidate/interface/GenParticle.h>
#include <DataFormats/PatCandidates/interface/CompositeCandidate.h>
#include <DataFormats/PatCandidates/interface/Muon.h>


//Headers for services and tools
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"

#include "TMath.h"
#include "Math/VectorUtil.h"



Onia2MuMuPAT::Onia2MuMuPAT(const edm::ParameterSet& iConfig):
  selectionType1_(iConfig.getParameter<int>("higherPuritySelection")),
  selectionType2_(iConfig.getParameter<int>("lowerPuritySelection"))
{  produces<pat::CompositeCandidateCollection>();  }


Onia2MuMuPAT::~Onia2MuMuPAT()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
Onia2MuMuPAT::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{  
  using namespace edm;
  using namespace std;
  using namespace reco;
  typedef Candidate::LorentzVector LorentzVector;


  std::auto_ptr<pat::CompositeCandidateCollection> oniaOutput(new pat::CompositeCandidateCollection);
  
  //  Handle<TriggerResults> trigger;
  //iEvent.getByLabel("TriggerResults",trigger);

  //Handle< vector<GenParticle> > particles;
  //iEvent.getByLabel("genParticles",particles);
    
  //Handle< vector<Track> > tracks;
  //iEvent.getByLabel("generalTracks",tracks);

  Handle< vector<Muon> > muons;
  iEvent.getByLabel("muons",muons);

  
  edm::ESHandle<TransientTrackBuilder> theTTBuilder;
  iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theTTBuilder);
  KalmanVertexFitter vtxFitter;


  // JPsi candidates only from muons
  for(vector<Muon>::const_iterator it = muons->begin(); it!=muons->end();++it){
    if(!selectionMuons(*it,1)) continue;
    for(vector<Muon>::const_iterator it2 = it+1; it2!=muons->end();++it2){
      if(!selectionMuons(*it2,2)) continue;

      
      // require Tracker track. It will not work with CaloMuon (TOBE FIXED)
      if(!(it->track().get() && it2->track().get())) continue; 

      pat::CompositeCandidate myCand;


      // ---- fit vertex using Tracker tracks ----
      vector<TransientTrack> t_tks;
      t_tks.push_back(theTTBuilder->build(it->track().get()));
      t_tks.push_back(theTTBuilder->build(it2->track().get()));
      TransientVertex myVertex = vtxFitter.vertex(t_tks);

      float vChi2 = myVertex.totalChiSquared();
      float vNDF  = myVertex.degreesOfFreedom();
      float vProb(TMath::Prob(vChi2,(int)vNDF));

      myCand.addUserFloat("vNChi2",vChi2/vNDF);
      myCand.addUserFloat("vProb",vProb);
      //qh.setRecoVertex(myVertex);      



      // ---- define PAT::Muons and add them to the candidate ----
      pat::Muon myMuonA(*it);  myMuonA.embedTrack();
      pat::Muon myMuonB(*it2); myMuonB.embedTrack();

      if(it->pt()<it2->pt()){
	myCand.addDaughter(myMuonA,"muon1");
	myCand.addDaughter(myMuonB,"muon2");
      }else{
	myCand.addDaughter(myMuonA,"muon2");
	myCand.addDaughter(myMuonB,"muon1");	
      }


      // ---- define and set candidate's 4momentum  ----
      LorentzVector jpsi = it->p4() + it2->p4();
      //LorentzVector jpsi = it->track()->p4() + it2->track()->p4();
      //qh.setRecoJPsi(jpsi);
      //cout << "jpsi: " << jpsi.mass() << endl;
      myCand.setP4(jpsi);
      
      oniaOutput->push_back(myCand);
    }
  }
  

  iEvent.put(oniaOutput);

  
 
}

bool
Onia2MuMuPAT::selectionMuons(const reco::Muon& muon,int selectionType) const{
  using namespace std;
  int actualSelection;
  if(selectionType ==1) actualSelection = selectionType1_; //selection for higherPurity muons
  else actualSelection = selectionType2_; //selection for lowerPurity muons

  switch(actualSelection){
  case 1:  //select global muon
    return muon.isGlobalMuon();
    break;
  case 2:  //select tracker muon
    return muon.isTrackerMuon();;
    break;
  case 3:  //select calo muon
    return muon.isCaloMuon();;
    break;
  default:
    cout << "ERROR: acutalSelection is not matching to any known selection" << endl;   
    return false;
  }


}


// ------------ method called once each job just before starting event loop  ------------
void 
Onia2MuMuPAT::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
Onia2MuMuPAT::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(Onia2MuMuPAT);
