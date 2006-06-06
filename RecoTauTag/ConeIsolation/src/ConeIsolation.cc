// -*- C++ -*-
//
// Package:    ConeIsolation
// Class:      ConeIsolation
// 
/**\class ConeIsolation ConeIsolation.cc RecoBTag/ConeIsolation/src/ConeIsolation.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Andrea Rizzi
//         Created:  Thu Apr  6 09:56:23 CEST 2006
// $Id: ConeIsolation.cc,v 1.7 2006/05/26 17:14:36 arizzi Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "RecoTauTag/ConeIsolation/interface/ConeIsolation.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/BTauReco/interface/IsolatedTauTagInfo.h"




//#include "MagneticField/Engine/interface/MagneticField.h"
//#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
//#include "RecoBTag/BTagTools/interface/SignedImpactParameter3D.h"
//#include "RecoBTag/BTagTools/interface/SignedTransverseImpactParameter.h"



//
// constructors and destructor
//
ConeIsolation::ConeIsolation(const edm::ParameterSet& iConfig)
{
   produces<reco::JetTagCollection>();  //Several producer so I put a label
//   produces<reco::ConeIsolationTagInfoCollection>();       //Only one producer

}


ConeIsolation::~ConeIsolation()
{

}


//
// member functions
//
// ------------ method called to produce the data  ------------
void
ConeIsolation::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   Handle<reco::JetTracksAssociationCollection> jetTracksAssociation;
   iEvent.getByLabel("JetTracksAssociation",jetTracksAssociation);
//   Handle<PrimaryVertex> tracks;
//   iEvent.getByLabel("Tracks",tracks);
//   for() //loop over jets
//   {
//    call concrete algo
//   }
   
   reco::JetTagCollection * baseCollection = new reco::JetTagCollection();

   reco::IsolatedTauTagInfoCollection * extCollection = new reco::IsolatedTauTagInfoCollection();



  Vertex::Error e;
  e(0,0)=1;
  e(1,1)=1;
  e(2,2)=1;
  Vertex::Point p(0,0,0);
  
  Vertex dummyPV(p,e,1,1,1);
   
  int i=0;
  //cout << "here-0.5" << jetTracksAssociation <<endl;     
  JetTracksAssociationCollection::const_iterator it = jetTracksAssociation->begin();
  for(; it != jetTracksAssociation->end(); it++)
     {
       
       baseCollection->push_back(m_algo.tag(edm::Ref<JetTracksAssociationCollection>(jetTracksAssociation,i),dummyPV).first);    
       i++;
     }


   std::auto_ptr<reco::JetTagCollection> resultBase(baseCollection);
   iEvent.put(resultBase);
   std::auto_ptr<reco::IsolatedTauTagInfoCollection> resultExt(extCollection);  
   iEvent.put(resultExt);

}

//define this as a plug-in
DEFINE_FWK_MODULE(ConeIsolation)

