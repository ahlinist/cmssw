#include "HeavyFlavorAnalysis/Onia2MuMu/interface/Onia2MuMuPAT.h"

//Headers for the data items
#include <DataFormats/TrackReco/interface/TrackFwd.h>
#include <DataFormats/TrackReco/interface/Track.h>
#include <DataFormats/MuonReco/interface/MuonFwd.h>
#include <DataFormats/MuonReco/interface/Muon.h>
#include <DataFormats/Common/interface/View.h>
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
  muons_(iConfig.getParameter<edm::InputTag>("muons")),
  selectionType1_(iConfig.getParameter<int>("higherPuritySelection")),
  selectionType2_(iConfig.getParameter<int>("lowerPuritySelection")),
  addCommonVertex_(iConfig.getParameter<bool>("addCommonVertex")),
  addMCTruth_(iConfig.getParameter<bool>("addMCTruth"))
{  
    produces<pat::CompositeCandidateCollection>();  
}


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

  Handle< View<pat::Muon> > muons;
  iEvent.getByLabel(muons_,muons);

  
  edm::ESHandle<TransientTrackBuilder> theTTBuilder;
  iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theTTBuilder);
  KalmanVertexFitter vtxFitter;


  // JPsi candidates only from muons
  for(View<pat::Muon>::const_iterator it = muons->begin(), itend = muons->end(); it != itend; ++it){
    if(!selectionMuons(*it,1)) continue;
    for(View<pat::Muon>::const_iterator it2 = it+1; it2 != itend;++it2){
      if(!selectionMuons(*it2,2)) continue;

      
      // require Tracker track. It will not work with CaloMuon (TOBE FIXED)
      if(!(it->track().get() && it2->track().get())) continue; 

      pat::CompositeCandidate myCand;

      // ---- define children ----
      if(it->pt()<it2->pt()){
	myCand.addDaughter(*it, "muon1");
	myCand.addDaughter(*it2,"muon2");
      }else{
	myCand.addDaughter(*it, "muon2");
	myCand.addDaughter(*it2,"muon1");	
      }

      // ---- define and set candidate's 4momentum  ----
      LorentzVector jpsi = it->p4() + it2->p4();
      //LorentzVector jpsi = it->track()->p4() + it2->track()->p4();
      //qh.setRecoJPsi(jpsi);
      //cout << "jpsi: " << jpsi.mass() << endl;
      myCand.setP4(jpsi);
      

      // ---- fit vertex using Tracker tracks ----
      vector<TransientTrack> t_tks;
      t_tks.push_back(theTTBuilder->build(*it->track()));  // pass the reco::Track, not  the reco::TrackRef (which can be transient)
      t_tks.push_back(theTTBuilder->build(*it2->track())); // otherwise the vertex will have transient refs inside.
      TransientVertex myVertex = vtxFitter.vertex(t_tks);
      if (myVertex.isValid()) {
          float vChi2 = myVertex.totalChiSquared();
          float vNDF  = myVertex.degreesOfFreedom();
          float vProb(TMath::Prob(vChi2,(int)vNDF));

          myCand.addUserFloat("vNChi2",vChi2/vNDF);
          myCand.addUserFloat("vProb",vProb);
          if (addCommonVertex_) {
              myCand.addUserData("commonVertex",reco::Vertex(myVertex));
          }
      } else {
          myCand.addUserFloat("vNChi2",-1);
          myCand.addUserFloat("vProb", -1);
      }
     
      // ---- MC Truth, if enabled ----
      if (addMCTruth_) {
          reco::GenParticleRef genMu1 = it->genParticleRef();
          reco::GenParticleRef genMu2 = it2->genParticleRef();
          if (genMu1.isNonnull() && genMu2.isNonnull()) {
              reco::GenParticleRef mom1 = genMu1->motherRef();
              reco::GenParticleRef mom2 = genMu2->motherRef();
              if (mom1.isNonnull() && (mom1 == mom2)) {
                  if (mom1.isTransient()) std::cerr << "Mom1 is transient???" << std::endl;
                  myCand.setGenParticleRef(mom1); // set
                  myCand.embedGenParticle();      // and embed
              }
          }
      }

      // ---- Push back output ----  
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
    return muon.isTrackerMuon() && !muon.isGlobalMuon();;
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
