#include "HeavyFlavorAnalysis/Onia2MuMu/interface/Onia2MuMuPAT.h"

//Headers for the data items
#include <DataFormats/TrackReco/interface/TrackFwd.h>
#include <DataFormats/TrackReco/interface/Track.h>
#include <DataFormats/MuonReco/interface/MuonFwd.h>
#include <DataFormats/MuonReco/interface/Muon.h>
#include <DataFormats/Common/interface/View.h>
#include <DataFormats/HepMCCandidate/interface/GenParticle.h>
#include <DataFormats/PatCandidates/interface/Muon.h>
#include <DataFormats/VertexReco/interface/VertexFwd.h>

//Headers for services and tools
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"

#include "TMath.h"
#include "Math/VectorUtil.h"
#include "TVector3.h"


Onia2MuMuPAT::Onia2MuMuPAT(const edm::ParameterSet& iConfig):
  muons_(iConfig.getParameter<edm::InputTag>("muons")),
  thebeamspot_(iConfig.getParameter<edm::InputTag>("beamSpotTag")),
  thePVs_(iConfig.getParameter<edm::InputTag>("primaryVertexTag")),
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

  Handle<BeamSpot> theBeamSpot;
  iEvent.getByLabel(thebeamspot_,theBeamSpot);
  BeamSpot bs = *theBeamSpot;
  TVector3 vBs;
  vBs.SetXYZ(bs.x0(), bs.y0(), 0);

  Handle<VertexCollection> priVtxs;
  iEvent.getByLabel(thePVs_, priVtxs);
  TVector3 vPv;
  if ( priVtxs->begin() != priVtxs->end() ) {
    Vertex privtx = *(priVtxs->begin());
    vPv.SetXYZ(privtx.position().x(), privtx.position().y(), 0);
  }
  else {
    vPv.SetXYZ(0, 0, 0);
  }

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

      // require Tracker track. It will not work with CaloMuon (TO BE FIXED)
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

      // ---- define and set candidate's 4momentum  ----  (TO BE FIXED)
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

	  TVector3 vtx;
	  vtx.SetXYZ(myVertex.position().x(),myVertex.position().y(),0);
	  TVector3 pperp(jpsi.px(), jpsi.py(), 0);
        
          // lifetime using PV
	  TVector3 vdiff = vtx - vPv;
	  double cosAlpha = vdiff.Dot(pperp)/(vdiff.Perp()*pperp.Perp());
	  double ctauPV = vdiff.Perp()*cosAlpha*3.09688/pperp.Perp();
	  myCand.addUserFloat("ppdlPV",ctauPV);
          myCand.addUserFloat("cosAlpha",cosAlpha);
          // lifetime using BS
          vdiff = vtx - vBs;
	  cosAlpha = vdiff.Dot(pperp)/(vdiff.Perp()*pperp.Perp());
	  double ctauBS = vdiff.Perp()*cosAlpha*3.09688/pperp.Perp();
          myCand.addUserFloat("ppdlBS",ctauBS);

          if (addCommonVertex_) {
              myCand.addUserData("commonVertex",reco::Vertex(myVertex));
          }
	 

      } else {
          myCand.addUserFloat("vNChi2",-1);
          myCand.addUserFloat("vProb", -1);
          myCand.addUserFloat("ppdlPV",-100);
          myCand.addUserFloat("cosAlpha",-100);
          myCand.addUserFloat("ppdlBS",-100);
          if (addCommonVertex_) {
              myCand.addUserData("commonVertex",reco::Vertex());
          }
      }
     
      // ---- MC Truth, if enabled ----
      if (addMCTruth_) {
	TVector3 trueVtx(0.0,0.0,0.0);
	TVector3 trueP(0.0,0.0,0.0);
	TVector3 trueVtxMom(0.0,0.0,0.0);
	
	reco::GenParticleRef genMu1 = it->genParticleRef();
	reco::GenParticleRef genMu2 = it2->genParticleRef();
	if (genMu1.isNonnull() && genMu2.isNonnull()) {
	  reco::GenParticleRef mom1 = genMu1->motherRef();
	  reco::GenParticleRef mom2 = genMu2->motherRef();
	  if (mom1.isNonnull() && (mom1 == mom2)) {
	    if (mom1.isTransient()) std::cerr << "Mom1 is transient???" << std::endl;
	    myCand.setGenParticleRef(mom1); // set
	    myCand.embedGenParticle();      // and embed
	    trueVtx.SetXYZ(mom1->vertex().x(),mom1->vertex().y(),mom1->vertex().z());
	    trueP.SetXYZ(mom1->momentum().x(),mom1->momentum().y(),mom1->momentum().z());
	    
	    bool aBhadron = false;
	    reco::GenParticleRef grandmom1 = mom1->motherRef();       // find mothers
	    if (grandmom1.isNull()) {
	      myCand.addUserFloat("momPDGid",-1);
	      myCand.addUserFloat("ppdlTrue",0);
	    } else {
	      if (isAbHadron(grandmom1->pdgId())) {
		myCand.addUserFloat("momPDGid",grandmom1->pdgId());
		trueVtxMom.SetXYZ(grandmom1->vertex().x(),grandmom1->vertex().y(),grandmom1->vertex().z());
		aBhadron = true;
	      } else {
		reco::GenParticleRef grandmom2 = grandmom1->motherRef();
		if (grandmom2.isNonnull() && isAbHadron(grandmom2->pdgId())) {
		  myCand.addUserFloat("momPDGid",grandmom2->pdgId());
		  trueVtxMom.SetXYZ(grandmom2->vertex().x(),grandmom2->vertex().y(),grandmom2->vertex().z());
		  aBhadron = true;
		}
	      }
	      if (!aBhadron) {
		myCand.addUserFloat("momPDGid",grandmom1->pdgId());
                trueVtxMom.SetXYZ(grandmom1->vertex().x(),grandmom1->vertex().y(),grandmom1->vertex().z());
	      }
	      TVector3 vdiff = trueVtx - trueVtxMom;
	      double ctauTrue = vdiff.Perp()*3.09688/trueP.Perp();
	      myCand.addUserFloat("ppdlTrue",ctauTrue);
	      
	    }
	  }
	}  
      }

      // ---- Push back output ----  
      oniaOutput->push_back(myCand);
    }
  }

  std::sort(oniaOutput->begin(),oniaOutput->end(),pTComparator_);

  iEvent.put(oniaOutput);

}

bool
Onia2MuMuPAT::selectionMuons(const reco::Muon& muon,int selectionType) const{
  using namespace std;
  int actualSelection;
  if(selectionType ==1) actualSelection = selectionType1_; //selection for higherPurity muons
  else actualSelection = selectionType2_;                  //selection for lowerPurity muons

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

bool 
Onia2MuMuPAT::isAbHadron(int pdgID) const {

  if (abs(pdgID) == 511 || abs(pdgID) == 521 || abs(pdgID) == 531 || abs(pdgID) == 5122) return true;
  return false;

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
