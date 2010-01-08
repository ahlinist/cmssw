// -*- C++ -*-
//
// Package:    PATObjectExample
// Class:      PATObjectExample
// 
/**\class PATObjectExample PATObjectExample.cc HeavyFlavorAnalysis/Examples/src/PATObjectExample.cc

 Description: Example showing how to access and use PAT objects as needed
for B physics analysis

*/
//
// Original Author:  Keith Ulmer
//         Created:  Mon Jul  6 10:53:49 MDT 2009
// $Id: PATObjectExample.cc,v 1.1 2009/07/06 23:23:44 kaulmer Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

//#include "DataFormats/PatCandidates/interface/PATObject.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/Candidate/interface/VertexCompositeCandidate.h"
#include "DataFormats/V0Candidate/interface/V0Candidate.h"
#include "RecoVertex/V0Producer/interface/V0Producer.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/GenericParticle.h"

//
// class decleration
//

class PATObjectExample : public edm::EDAnalyzer {
   public:
      explicit PATObjectExample(const edm::ParameterSet&);
      ~PATObjectExample();


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
PATObjectExample::PATObjectExample(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed

}


PATObjectExample::~PATObjectExample()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
PATObjectExample::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  using std::vector;
  using namespace edm;
  using namespace reco;
  using namespace std;

   Handle<vector<VertexCompositeCandidate> > theV0Handle;
   iEvent.getByLabel("generalV0Candidates", "Kshorts", theV0Handle);

   Handle<MuonCollection> theMuonHandle;
   iEvent.getByLabel("muons", theMuonHandle );

   Handle< TrackCollection > theTrackHandle;
   iEvent.getByLabel("generalTracks", theTrackHandle);

   Handle< vector<pat::GenericParticle> > thePATTrackHandle;
   iEvent.getByLabel("selectedLayer1TrackCands", thePATTrackHandle);

   Handle< vector<pat::Muon> > thePATMuonHandle;
   iEvent.getByLabel("cleanLayer1Muons", thePATMuonHandle);
   
   for ( MuonCollection::const_iterator iMu = theMuonHandle->begin();
	 iMu != theMuonHandle->end(); ++iMu ) {
   }

   // loop through the PAT muons
   for ( vector<pat::Muon>::const_iterator iMu = thePATMuonHandle->begin();
	 iMu != thePATMuonHandle->end(); ++iMu ) {
     
     // how to access the reco::Muon
     TrackRef muTrack = iMu->track();
     if ( muTrack.isNull() ) {
       cout << "continue due to no track ref" << endl;
       continue;
     }
     
     cout << "Reconstructed muon pT = " << muTrack->pt() << " and eta = " << muTrack->eta() << endl;
     
     // get MC truth match
     if ( iMu->genParticlesSize()>0 ) {
       const std::vector<reco::GenParticleRef> gpRefs = iMu->genParticleRefs();
       for ( size_t iGP = 0; iGP<iMu->genParticlesSize(); ++iGP) {
	 const GenParticleRef gpRef = iMu->genParticleRef(iGP);
	 if (!gpRef.isNull())
	   cout << "genParticle " << iGP << " pT = " << gpRef->pt() << " and eta = " << gpRef->eta() << endl;
       }
     }
     
   }

     
     // loop through the PAT tracks
   for ( vector<pat::GenericParticle>::const_iterator iTrack = thePATTrackHandle->begin();
	 iTrack != thePATTrackHandle->end(); ++iTrack ) {

     // how to access the reco::Track object
     TrackRef hadTrack = iTrack->track();
     if ( hadTrack.isNull() ) {
       cout << "continue due to no track ref" << endl;
       continue;
     }

     //check if PAT track has overlap with muon
     const reco::CandidatePtrVector & mu_overlaps = iTrack->overlaps("muons");
     if ( mu_overlaps.size() > 0 ) cout << "track overlaps with a muon." << endl;
     for (size_t i = 0; i < mu_overlaps.size(); ++i) {
       std::cout << "  muon " <<  i << " pt = " <<  mu_overlaps[i]->pt() <<  std::endl;
       // try to convert in a pat::Electron
       const pat::Muon *mu = dynamic_cast<const pat::Muon *>(&*mu_overlaps[i]);
       if (mu) {
	 // check here that muon match isn't the same as a muon used in the reco...

       }
     }

     // get MC truth match

     if ( iTrack->genParticlesSize()>0 ) {
       const std::vector<reco::GenParticleRef> gpRefs = iTrack->genParticleRefs();
     }
   }
   
}


// ------------ method called once each job just before starting event loop  ------------
void 
PATObjectExample::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
PATObjectExample::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(PATObjectExample);
