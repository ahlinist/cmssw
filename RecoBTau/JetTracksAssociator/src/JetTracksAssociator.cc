// -*- C++ -*-
//
// Package:    JetTracksAssociator
// Class:      JetTracksAssociator
// 
/**\class JetTracksAssociator JetTracksAssociator.cc RecoBTag/JetTracksAssociator/src/JetTracksAssociator.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Andrea Rizzi
//         Created:  Wed Apr 12 11:12:49 CEST 2006
// $Id: JetTracksAssociator.cc,v 1.8 2006/08/08 14:50:43 tboccali Exp $
//
//


// system include files
#include <memory>
#include <string>
#include <iostream>
using namespace std;
#include "FWCore/MessageLogger/interface/MessageLogger.h"

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/BTauReco/interface/JetTracksAssociation.h"

#include "DataFormats/Math/interface/Vector3D.h"

// Math
#include "Math/GenVector/VectorUtil.h"
#include "Math/GenVector/PxPyPzE4D.h"

using namespace reco;

//
// class decleration
//

class JetTracksAssociator : public edm::EDProducer {
   public:
      explicit JetTracksAssociator(const edm::ParameterSet&);
      ~JetTracksAssociator();


      virtual void produce(edm::Event&, const edm::EventSetup&);
   private:
     JetTracksAssociationCollection * associate(const edm::Handle<CaloJetCollection> & jets,
                                const edm::Handle<TrackCollection> & tracks ) const;
     bool trackIsInJetCone( const Jet & jet , const Track & track ) const;

     // ----------member data ---------------------------
     string m_tracksSrc;
     string m_jetsSrc;
     double m_deltaRCut;
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
JetTracksAssociator::JetTracksAssociator(const edm::ParameterSet& iConfig)
{
  produces<reco::JetTracksAssociationCollection>();
  m_tracksSrc = iConfig.getParameter<string>("tracks");
  m_jetsSrc   = iConfig.getParameter<string>("jets");
  m_deltaRCut = iConfig.getParameter<double>("coneSize");
}


JetTracksAssociator::~JetTracksAssociator()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
JetTracksAssociator::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   Handle<CaloJetCollection> jets;
   iEvent.getByLabel(m_jetsSrc, jets);
   Handle<TrackCollection> tracks;
   iEvent.getByLabel(m_tracksSrc, tracks);
   
   std::auto_ptr<JetTracksAssociationCollection> jetTracks(associate(jets,tracks));
   iEvent.put(jetTracks);
}

JetTracksAssociationCollection * JetTracksAssociator::associate( const edm::Handle<CaloJetCollection> & jets,
                                                                 const edm::Handle<TrackCollection>   & tracks ) const
{
  JetTracksAssociationCollection * outputCollection = new JetTracksAssociationCollection();
  //loop on jets and associate
  for (size_t j = 0; j < jets->size(); j++)
  {
    //cout << boolalpha;
    //cout << fixed;

#ifdef DEBUG
    LogDebug("JetTracksAssociator") << "->   Jet " << setw(2) << j << " pT: " << setprecision(2) << setw(6) << (*jets)[j].pt() << " eta: " << setprecision(2) << setw(5) << (*jets)[j].eta() << " phi: " << setprecision(2) << setw(5) << (*jets)[j].phi();
#endif
    for (size_t t=0; t < tracks->size() ; t++) {
      double delta  = ROOT::Math::VectorUtil::DeltaR((*jets)[j].p4().Vect(), (*tracks)[t].momentum());
      bool   inside = (delta < m_deltaRCut);
#ifdef DEBUG
       LogDebug("JetTracksAssociator") << "   Track " << setw(2) << t << " pT: " << setprecision(2) << setw(6) << (*tracks)[t].pt() << " eta: " << setprecision(2) << setw(5) << (*tracks)[t].eta() << " phi: " << setprecision(2) << setw(5) << (*tracks)[t].phi()
				       << "   delta R: " << setprecision(2) << setw(4) << delta << " is inside: " << inside;
#endif
      if (inside) 
        outputCollection->insert(edm::Ref<CaloJetCollection>(jets, j), edm::Ref<TrackCollection>(tracks, t));
    }
  } 

  return outputCollection;
}

//define this as a plug-in
DEFINE_FWK_MODULE(JetTracksAssociator);
