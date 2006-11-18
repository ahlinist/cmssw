// -*- C++ -*-
//
// Package:    JTPropagationAssociator
// Class:      JTPropagationAssociator
// 
/**\class JTPropagationAssociator JTPropagationAssociator.cc RecoBTag/JTPropagationAssociator/src/JTPropagationAssociator.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Ioana Anghel
//         Created:  Fri Nov 17 17:54:44 CST 2006
// $Id: JTPropagationAssociator.cc,v 1.00 2006/10/28 17:10:54 fwyzard Exp $
//
//


// system include files
#include <memory>
#include <string>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "Geometry/Surface/interface/TkRotation.h"
#include "Geometry/Surface/interface/BoundCylinder.h"
#include "Geometry/Surface/interface/SimpleCylinderBounds.h"

#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"

#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h" 
#include "MagneticField/Engine/interface/MagneticField.h"
 
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/BTauReco/interface/JetTracksAssociation.h"
#include "DataFormats/Math/interface/Vector3D.h"

#include "TrackingTools/Records/interface/TrackingComponentsRecord.h"
#include "TrackingTools/MaterialEffects/interface/PropagatorWithMaterial.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"

// Math
#include "Math/GenVector/VectorUtil.h"
#include "Math/GenVector/PxPyPzE4D.h"

using namespace std;
using namespace edm;
using namespace reco;

//
// class decleration
//

class JTPropagationAssociator : public edm::EDProducer {
   public:
      explicit JTPropagationAssociator( const edm::ParameterSet& iConfig );
      ~JTPropagationAssociator();

      void produce( edm::Event& iEvent, const edm::EventSetup& iSetup );
      void beginJob( const edm::EventSetup& iSetup );

   private:
     JetTracksAssociationCollection * associate(const edm::Handle<CaloJetCollection> & jets,
                                const edm::Handle<TrackCollection> & tracks ) const;

     // ----------member data ---------------------------
     edm::InputTag m_tracksSrc;
     edm::InputTag m_jetsSrc;
     double m_deltaRCut;
     
     const TrackingGeometry *         m_trackingGeometry;
     const MagneticField *            m_magneticField;
     const Propagator *               m_propagator;
     const TrajectoryStateTransform * m_transformer;
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
JTPropagationAssociator::JTPropagationAssociator(const edm::ParameterSet& iConfig)
{
  produces<reco::JetTracksAssociationCollection>();
  m_tracksSrc = iConfig.getParameter<edm::InputTag>("tracks");
  m_jetsSrc   = iConfig.getParameter<edm::InputTag>("jets");
  m_deltaRCut = iConfig.getParameter<double>("coneSize");
}


JTPropagationAssociator::~JTPropagationAssociator()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

void
JTPropagationAssociator::beginJob(const edm::EventSetup& iSetup) {
   // get tracker geometry
   edm::ESHandle<TrackerGeometry> trackingGeometryHandle;
   iSetup.get<TrackerDigiGeometryRecord>().get( trackingGeometryHandle );
   m_trackingGeometry = trackingGeometryHandle.product();
  
   // get magnetic field
   edm::ESHandle<MagneticField> magneticFieldHandle;
   iSetup.get<IdealMagneticFieldRecord>().get( magneticFieldHandle );
   m_magneticField = magneticFieldHandle.product();
  
   // get propagator
   edm::ESHandle<Propagator> propagatorHandle;
   iSetup.get<TrackingComponentsRecord>().get( "PropagatorWithMaterial", propagatorHandle );
   m_propagator = propagatorHandle.product();

   // build a transformer
   m_transformer = new TrajectoryStateTransform();
}

// ------------ method called to produce the data  ------------
void
JTPropagationAssociator::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   Handle<CaloJetCollection> jets;
   iEvent.getByLabel(m_jetsSrc, jets);
   Handle<TrackCollection> tracks;
   iEvent.getByLabel(m_tracksSrc, tracks);
   
   std::auto_ptr<JetTracksAssociationCollection> jetTracks(associate( jets, tracks ));
   iEvent.put(jetTracks);
}

JetTracksAssociationCollection * JTPropagationAssociator::associate( const edm::Handle<CaloJetCollection> & jets,
                                                                     const edm::Handle<TrackCollection>   & tracks ) const
{
  ReferenceCountingPointer<Surface> ecalWall( new BoundCylinder(
                                                    GlobalPoint(0.,0.,0.), 
                                                    TkRotation<float>(),
                                                    SimpleCylinderBounds( 130, 130, -300, 300 ) ) );

  JetTracksAssociationCollection * outputCollection = new JetTracksAssociationCollection();
  //loop on jets and associate
  for (size_t j = 0; j < jets->size(); j++) {
    #ifdef DEBUG
    LogDebug("JTPropagationAssociator") << "->   Jet " << setw(2) << j << " pT: " << setprecision(2) << setw(6) << (*jets)[j].pt() << " eta: " << setprecision(2) << setw(5) << (*jets)[j].eta() << " phi: " << setprecision(2) << setw(5) << (*jets)[j].phi();
    #endif
    for (size_t t = 0; t < tracks->size(); t++) {
      TrajectoryStateOnSurface outerState = m_transformer->outerStateOnSurface( (*tracks)[t], *m_trackingGeometry, m_magneticField );
      if (! outerState.isValid())
        continue;
        
      TrajectoryStateOnSurface extraState = m_propagator->propagate( outerState, *ecalWall );      
      if (! extraState.isValid())
        continue;
      
      math::GlobalVector _position( 
          extraState.globalPosition().x(), 
          extraState.globalPosition().y(), 
          extraState.globalPosition().z());
      double delta  = ROOT::Math::VectorUtil::DeltaR( (*jets)[j].p4().Vect(), _position );
      bool   inside = (delta < m_deltaRCut);
      #ifdef DEBUG
      LogDebug("JTPropagationAssociator") << "   Track " << setw(2) << t << " pT: " << setprecision(2) << setw(6) << (*tracks)[t].pt() << " eta: " << setprecision(2) << setw(5) << (*tracks)[t].eta() << " phi: " << setprecision(2) << setw(5) << (*tracks)[t].phi()
                                          << "   delta R: " << setprecision(2) << setw(4) << delta << " is inside: " << inside;
      #endif
      if (inside) 
        outputCollection->insert(edm::Ref<CaloJetCollection>(jets, j), edm::Ref<TrackCollection>(tracks, t));
    }
  } 

  return outputCollection;
}

//define this as a plug-in
DEFINE_FWK_MODULE(JTPropagationAssociator);
