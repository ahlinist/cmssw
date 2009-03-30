// -*- C++ -*-
//
// Package:    TrackMultiplicity
// Class:      TrackMultiplicity
//
/**\class TrackMultiplicity TrackMultiplicity.cc DiffractiveForwardAnalysis/TrackMultiplicity/src/TrackMultiplicity.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Felipe Torres Da Silva De Araujo
//         Created:  Fri Mar 13 21:22:58 CET 2009
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

//MessageLogger
#include "FWCore/MessageLogger/interface/MessageLogger.h"

//ROOT
#include <TROOT.h>
#include <TH1.h>
#include <TH2.h>

//TrackCollection
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

//JetCollection
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/GenJet.h"

//deltaR
#include "DataFormats/Math/interface/deltaR.h"

using namespace edm;
using namespace std;
using namespace math;
using namespace reco;

//
// class decleration
//

class TrackMultiplicity : public edm::EDProducer {
   public:
      explicit TrackMultiplicity(const edm::ParameterSet&);
      ~TrackMultiplicity();

   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------
    double TrackPtCut;

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
TrackMultiplicity::TrackMultiplicity(const edm::ParameterSet& iConfig) :
        TrackPtCut( iConfig.getParameter<double>( "TrackPtCut" ) )
{
    produces< int >( "trackMult" ).setBranchAlias( "trackMult" );


}


TrackMultiplicity::~TrackMultiplicity()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
TrackMultiplicity::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    auto_ptr< int > trackMult( new int );


    Handle<CaloJetCollection> jets;
    iEvent.getByLabel("L2L3CorJetSC5Calo",jets);

    //#jets above et_cut
    double jet1Eta = 0.;
    double jet1Phi = 0.;
    double jet2Eta = 0.;
    double jet2Phi = 0.;
    for (CaloJetCollection::const_iterator i_jet = jets->begin(); i_jet != (jets->begin()+2); i_jet++)
    {
        if (i_jet == jets->begin())
        {
            jet1Eta = i_jet->eta();
            jet1Phi = i_jet->phi();
        }

        if (i_jet == jets->begin()+1)
        {
            jet2Eta = i_jet->eta();
            jet2Phi = i_jet->phi();
        }

    }



    //track Handle
    Handle< TrackCollection > trackHandle;
    iEvent.getByLabel ("generalTracks", trackHandle);
    int goodTracksCount = 0;
    for (TrackCollection::const_iterator track = trackHandle->begin(); track != trackHandle->end(); ++ track)
    {
        //cout << "oi" << endl;
        if (fabs(track->eta()) < 2.0 && track->pt() > TrackPtCut)
        {
            if ((deltaR(track->eta(),track->phi(),jet1Eta,jet1Phi) > 0.5) && (deltaR(track->eta(),track->phi(),jet2Eta,jet2Phi) > 0.5))
            {
                goodTracksCount++;
            }
        }

    }
    *trackMult = goodTracksCount;


    iEvent.put( trackMult, "trackMult");


}

// ------------ method called once each job just before starting event loop  ------------
void
TrackMultiplicity::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
TrackMultiplicity::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(TrackMultiplicity);
