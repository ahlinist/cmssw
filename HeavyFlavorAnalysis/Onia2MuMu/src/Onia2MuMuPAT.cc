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

      // ---- order by pT ----
      // if(it->pt()<it2->pt()){
      //   myCand.addDaughter(*it, "muon1");
      //   myCand.addDaughter(*it2,"muon2");
      // }else{
      //   myCand.addDaughter(*it, "muon2");
      //   myCand.addDaughter(*it2,"muon1");	
      // }

      // ---- order by purity ----
      myCand.addDaughter(*it, "muon1");
      myCand.addDaughter(*it2,"muon2");	

      // ---- candidate's 4momentum from muons ----  
      // LorentzVector jpsi = it->p4() + it2->p4();
         
      // ---- candidate's 4momentum from tracks ----  
      Track tr = *it->track();
      Track tr2 = *it2->track();
      LorentzVector trp4(tr.px(),tr.py(),tr.pz(),sqrt(tr.p()*tr.p() + 0.011163613));
      LorentzVector tr2p4(tr2.px(),tr2.py(),tr2.pz(),sqrt(tr2.p()*tr2.p() + 0.011163613));
      LorentzVector jpsi = trp4 + tr2p4;

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
	
	reco::GenParticleRef genMu1 = it->genParticleRef();
	reco::GenParticleRef genMu2 = it2->genParticleRef();
	if (genMu1.isNonnull() && genMu2.isNonnull()) {
	  reco::GenParticleRef mom1 = genMu1->motherRef();
	  reco::GenParticleRef mom2 = genMu2->motherRef();
	  if (mom1.isNonnull() && (mom1 == mom2)) {
	    if (mom1.isTransient()) std::cerr << "Mom1 is transient???" << std::endl;
	    myCand.setGenParticleRef(mom1); // set
	    myCand.embedGenParticle();      // and embed
	    std::pair<int, float> MCinfo = findJpsiMCInfo(mom1);
	    myCand.addUserInt("momPDGId",MCinfo.first);
	    myCand.addUserFloat("ppdlTrue",MCinfo.second);
	  } else {
	    myCand.addUserInt("momPDGId",0);
	    myCand.addUserFloat("ppdlTrue",-99.);
	  }
	} else {
	  myCand.addUserInt("momPDGId",0);
	  myCand.addUserFloat("ppdlTrue",-99.);
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
Onia2MuMuPAT::isAbHadron(int pdgID) {

  if (abs(pdgID) == 511 || abs(pdgID) == 521 || abs(pdgID) == 531 || abs(pdgID) == 5122) return true;
  return false;

}

std::pair<int, float>  
Onia2MuMuPAT::findJpsiMCInfo(reco::GenParticleRef genJpsi) {

  int momJpsiID = 0;
  float trueLife = -99.;

  TVector3 trueVtx(0.0,0.0,0.0);
  TVector3 trueP(0.0,0.0,0.0);
  TVector3 trueVtxMom(0.0,0.0,0.0);

  trueVtx.SetXYZ(genJpsi->vertex().x(),genJpsi->vertex().y(),genJpsi->vertex().z());
  trueP.SetXYZ(genJpsi->momentum().x(),genJpsi->momentum().y(),genJpsi->momentum().z());
	    
  bool aBhadron = false;
  reco::GenParticleRef Jpsimom = genJpsi->motherRef();       // find mothers
  if (Jpsimom.isNull()) {
    std::pair<int, float> result = make_pair(momJpsiID, trueLife);
    return result;
  } else {
    if (isAbHadron(Jpsimom->pdgId())) {
      momJpsiID = Jpsimom->pdgId();
      trueVtxMom.SetXYZ(Jpsimom->vertex().x(),Jpsimom->vertex().y(),Jpsimom->vertex().z());
      aBhadron = true;
    } else {
      reco::GenParticleRef Jpsigrandmom = Jpsimom->motherRef();
      if (Jpsigrandmom.isNonnull() && isAbHadron(Jpsigrandmom->pdgId())) {
	momJpsiID = Jpsigrandmom->pdgId();
	trueVtxMom.SetXYZ(Jpsigrandmom->vertex().x(),Jpsigrandmom->vertex().y(),Jpsigrandmom->vertex().z());
	aBhadron = true;
      }
    }
    if (!aBhadron) {
      momJpsiID = Jpsimom->pdgId();
      trueVtxMom.SetXYZ(Jpsimom->vertex().x(),Jpsimom->vertex().y(),Jpsimom->vertex().z());
    }
    TVector3 vdiff = trueVtx - trueVtxMom;
    trueLife = vdiff.Perp()*3.09688/trueP.Perp();
  } 

  std::pair<int, float> result = make_pair(momJpsiID, trueLife);
  return result;

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
