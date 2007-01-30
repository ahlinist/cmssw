// -*- C++ -*-
//
// Package:    TrackIPProducer
// Class:      TrackIPProducer
// 
/**\class TrackIPProducer TrackIPProducer.cc RecoBTau/TrackIPProducer/src/TrackIPProducer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Andrea Rizzi
//         Created:  Thu Apr  6 09:56:23 CEST 2006
// $Id: TrackIPProducer.cc,v 1.1 2007/01/25 09:12:42 arizzi Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "RecoBTau/TrackTools/interface/TrackIPProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/BTauReco/interface/TrackIPData.h"
#include "DataFormats/BTauReco/interface/JetTracksAssociation.h"

#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"

//#include "MagneticField/Engine/interface/MagneticField.h"
//#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "RecoBTau/TrackTools/interface/SignedImpactParameter3D.h"
#include "RecoBTau/TrackTools/interface/SignedTransverseImpactParameter.h"
#include "RecoBTau/TrackTools/interface/SignedDecayLength3D.h"

//#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include <iostream>

using namespace std;
using namespace reco;
using namespace edm;

//
// constructors and destructor
//
TrackIPProducer::TrackIPProducer(const edm::ParameterSet& iConfig) : 
  m_config(iConfig)  {

  produces<reco::JetTracksIPDataAssociationCollection>(); 
  
  m_associator = m_config.getParameter<string>("jetTracks");
  m_primaryVertexProducer = m_config.getParameter<string>("primaryVertex");
}

TrackIPProducer::~TrackIPProducer()
{
}

//
// member functions
//
// ------------ method called to produce the data  ------------
void
TrackIPProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
   //input objects
   Handle<reco::JetTracksAssociationCollection> jetTracksAssociation;
   iEvent.getByLabel(m_associator,jetTracksAssociation);
   Handle<reco::VertexCollection> primaryVertex;
   iEvent.getByLabel(m_primaryVertexProducer,primaryVertex);
   

    edm::ESHandle<TransientTrackBuilder> builder;
    iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",builder);
  //  m_algo.setTransientTrackBuilder(builder.product());

  

   //output collections 
   reco::JetTracksIPDataAssociationCollection * outCollection = new reco::JetTracksIPDataAssociationCollection();

   //use first pv of the collection
   //FIXME: handle missing PV with a dummy PV
   const  Vertex  *pv;

   bool pvFound = (primaryVertex->size() != 0);
   if(pvFound)
   {
    pv = &(*primaryVertex->begin());
   }
    else 
   { // create a dummy PV
     Vertex::Error e;
     e(0,0)=0.0015*0.0015;
      e(1,1)=0.0015*0.0015;
     e(2,2)=15.*15.;
     Vertex::Point p(0,0,0);
     pv=  new Vertex(p,e,1,1,1);
   }
   
   double pvZ=pv->z();
   SignedImpactParameter3D  sip3D;  
   SignedTransverseImpactParameter sip2D;
 
   int i=0;
   JetTracksAssociationCollection::const_iterator it = jetTracksAssociation->begin();
   for(; it != jetTracksAssociation->end(); it++, i++)
     {
//      int i=it->key.key();
        int ii=it->key.key();  
        GlobalVector direction(it->key->px(),it->key->py(),it->key->pz());

        TracksInJetData data;
        data.direction=direction; 
        TrackRefVector tracks = it->val;
        for (TrackRefVector::const_iterator itTrack = tracks.begin(); itTrack != tracks.end(); ++itTrack) {
             const Track & track = **itTrack;
             const TransientTrack * transientTrack = builder->build(&(**itTrack));
             float distToAxis = SignedImpactParameter3D::distanceWithJetAxis(*transientTrack,direction,*pv).second.value();
             float dLen = SignedDecayLength3D::apply(*transientTrack,direction,*pv).second.value();

        TrackIPData ipData;
        ipData.impactParameter3D=sip3D.apply(*transientTrack,direction,*pv).second.significance();
        ipData.impactParameter2D=sip2D.apply(*transientTrack,direction,*pv).second.significance();
        data.ipData.push_back(ipData);
         cout <<i << " " << ii << " IP: " <<   ipData.impactParameter3D.value() << endl; 
        }
       cout << "insert"  << endl;
       outCollection->insert(edm::Ref<JetTracksAssociationCollection>(jetTracksAssociation, ii), data); 
       cout << "done"  << endl;
     }
       cout << "put..."  << endl;
   std::auto_ptr<reco::JetTracksIPDataAssociationCollection> result(outCollection);
   iEvent.put(result);
   cout << "done"  << endl;
 
   if(!pvFound) delete pv; //dummy pv deleted
}

//define this as a plug-in
DEFINE_FWK_MODULE(TrackIPProducer);

