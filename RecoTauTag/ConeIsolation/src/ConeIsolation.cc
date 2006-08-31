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
// Original Author:  Simone Gennai
//      Created:  Thu Apr  6 09:56:23 CEST 2006
// $Id: ConeIsolation.cc,v 1.8 2006/08/08 07:53:48 tboccali Exp $
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

#include <DataFormats/VertexReco/interface/Vertex.h>
#include "FWCore/MessageLogger/interface/MessageLogger.h"

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
  jetTrackSrc = iConfig.getParameter<string>("JetTrackSrc");
  vertexSrc = iConfig.getParameter<string>("vertexSrc");
  usingVertex = iConfig.getParameter<bool>("useVertex");
  m_algo = new ConeIsolationAlgorithm(iConfig);
  
  std::string modulname = iConfig.getParameter<string>( "@module_label" );
   produces<reco::JetTagCollection>();
   produces<reco::JetTagCollection>().setBranchAlias( modulname );
   std::string extCollectionName = modulname;
   int stringStart = modulname.size() - 7;
   extCollectionName.erase(stringStart, 7);
   extCollectionName =  extCollectionName + "TagInfos";
   produces<reco::IsolatedTauTagInfoCollection>();       //Only one producer
   produces<reco::IsolatedTauTagInfoCollection>().setBranchAlias( extCollectionName );
   

}


ConeIsolation::~ConeIsolation()
{
  delete m_algo;
}



//
// member functions
//
// ------------ method called to produce the data  ------------
void
ConeIsolation::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   //Get jets with tracks
   Handle<reco::JetTracksAssociationCollection> jetTracksAssociation;
   iEvent.getByLabel(jetTrackSrc,jetTracksAssociation);
   
   reco::JetTagCollection * baseCollection = new reco::JetTagCollection();

   reco::IsolatedTauTagInfoCollection * extCollection = new reco::IsolatedTauTagInfoCollection();

   Vertex::Error e;
   e(0,0)=1;
   e(1,1)=1;
   e(2,2)=1;
   Vertex::Point p(0,0,0);
   Vertex myPV(p,e,1,1,1);
   //Get pixel vertices
   Handle<reco::VertexCollection> vertices;
   //      iEvent.getByLabel(vertexSrc,"PrimaryVertex",vertices);
      iEvent.getByLabel(vertexSrc,vertices);
   //   iEvent.getByLabel(vertexSrc,vertices);
   if(usingVertex)
     {

       const reco::VertexCollection vertCollection = *(vertices.product());
       reco::VertexCollection::const_iterator ci = vertCollection.begin();
       int i=0;
       if(!vertCollection.size()) return;
       for(;ci!=vertCollection.end();ci++){
	 edm::LogInfo("ConeIsolation::produce()")
	   <<" Vertex: "<<i<<" ("
	   <<ci->x()<<", "
	   <<ci->y()<<", "
	   <<ci->z()<<")"<<endl;
	 myPV = *ci;
       }
     }
   
  //cout << "here-0.5" << jetTracksAssociation <<endl;     
  JetTracksAssociationCollection::const_iterator it = jetTracksAssociation->begin();
  for(; it != jetTracksAssociation->end(); it++)
     {
       int i = it->key.index();
       pair<JetTag,IsolatedTauTagInfo> myPair =m_algo->tag(edm::Ref<JetTracksAssociationCollection>(jetTracksAssociation,i),myPV); 
       baseCollection->push_back(myPair.first);    
       //still need to set the JetTag
       extCollection->push_back(myPair.second);
       i++;
     }


   std::auto_ptr<reco::JetTagCollection> resultBase(baseCollection);
   edm::OrphanHandle <reco::JetTagCollection >  myJetTag =  iEvent.put(resultBase);
   int cc=0;
   reco::IsolatedTauTagInfoCollection::iterator myInfo = extCollection->begin();   for(;myInfo!=extCollection->end();myInfo++)
     {
       myInfo->setJetTag(JetTagRef(myJetTag,cc)); 
       cc++;
     }


   
   std::auto_ptr<reco::IsolatedTauTagInfoCollection> resultExt(extCollection);  
   iEvent.put(resultExt);

}

//define this as a plug-in
//DEFINE_FWK_MODULE(ConeIsolation)

