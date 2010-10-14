/// ////////////////////////////////////////
/// Stacked Tracker Simulations          ///
/// Written by:                          ///
/// Andrew W. Rose                       ///
/// 2008                                 ///
///                                      ///
/// Changed by:                          ///
/// Nicola Pozzobon                      ///
/// UNIPD                                ///
/// 2010, June                           ///
///                                      ///
/// Added feature:                       ///
/// Possibility to have Fake Stub flag   ///
/// in Simulations and Trk ID (see       ///
/// SimDataFormats/SLHC/GlobalStub.h).   ///
/// A Stub is flagged as Fake if the     ///
/// hits come from different SimTracks.  ///
/// To speed up the cross-check with the ///
/// PixelDigiSimLink, only the first hit ///
/// in the Cluster is checked, so BE     ///
/// VERY CAREFUL!                        ///
/// This Fakeness Check is done only for ///
/// Stubd from PixelDigis. In principle  ///
/// it could be done also for Stubs from ///
/// PSimHits, but it has not been        ///
/// implemented yet.                     ///
/// ////////////////////////////////////////

#ifndef GLOBAL_STUB_BUILDER_H
#define GLOBAL_STUB_BUILDER_H

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "SLHCUpgradeSimulations/Utilities/interface/StackedTrackerGeometryRecord.h"
#include "SLHCUpgradeSimulations/Utilities/interface/StackedTrackerGeometry.h"
#include "SLHCUpgradeSimulations/Utilities/interface/StackedTrackerDetUnit.h"
#include "SLHCUpgradeSimulations/Utilities/interface/StackedTrackerDetId.h"

#include "SimDataFormats/SLHC/interface/StackedTrackerTypes.h"

#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"

#include "DataFormats/DetId/interface/DetId.h"

#include "SLHCUpgradeSimulations/Utilities/interface/classInfo.h"


#include "SimDataFormats/TrackerDigiSimLink/interface/PixelDigiSimLink.h"
#include "DataFormats/Common/interface/DetSetVector.h"

template<  typename T  >
class GlobalStubBuilder : public edm::EDProducer {

  typedef cmsUpgrades::LocalStub< T >              LocalStubType;
  typedef std::vector  < LocalStubType >            LocalStubCollectionType;
  //typedef edm::Ref< LocalStubCollectionType , LocalStubType >  LocalStubRefType;
  typedef edm::Ptr< LocalStubType >  LocalStubPtrType;
  typedef std::vector<  cmsUpgrades::GlobalStub< T > > GlobalStubCollectionType;
  
  public:
    explicit GlobalStubBuilder(const edm::ParameterSet& iConfig): mClassInfo( new cmsUpgrades::classInfo(__PRETTY_FUNCTION__) )
    {
        produces< GlobalStubCollectionType >();
        LocalStubsInputTag  = iConfig.getParameter<edm::InputTag>("LocalStubs");
    }

      ~GlobalStubBuilder(){}

  private:
    virtual void beginJob(const edm::EventSetup& iSetup)
    {
      edm::ESHandle<cmsUpgrades::StackedTrackerGeometry> StackedTrackerGeomHandle;
      iSetup.get<cmsUpgrades::StackedTrackerGeometryRecord>().get(StackedTrackerGeomHandle);
      theStackedTracker = StackedTrackerGeomHandle.product();
    }


    virtual void produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
    {
      edm::Handle< LocalStubCollectionType > LocalStubHandle;
      iEvent.getByLabel( LocalStubsInputTag , LocalStubHandle);

      /// This is needed to check genuinity 
      edm::Handle<edm::DetSetVector<PixelDigiSimLink> >  thePixelDigiSimLink;
      iEvent.getByLabel("simSiPixelDigis", thePixelDigiSimLink);
      edm::Ptr< cmsUpgrades::LocalStub< cmsUpgrades::Ref_PixelDigi_ > > dummyStub;
  
      std::auto_ptr< GlobalStubCollectionType > GlobalStubsForOutput(new GlobalStubCollectionType );

      for (  unsigned int i = 0; i != LocalStubHandle->size() ; ++i ) {
        GlobalPoint innerHitPosition = LocalStubHandle->at(i).averagePosition( &(*theStackedTracker) , 0);
        GlobalPoint outerHitPosition = LocalStubHandle->at(i).averagePosition( &(*theStackedTracker) , 1);

        GlobalPoint globalPosition( (innerHitPosition.x()+outerHitPosition.x())/2 , (innerHitPosition.y()+outerHitPosition.y())/2 , (innerHitPosition.z()+outerHitPosition.z())/2 );
        GlobalVector directionVector(  outerHitPosition.x()-innerHitPosition.x()    ,  outerHitPosition.y()-innerHitPosition.y()    ,  outerHitPosition.z()-innerHitPosition.z()    );

        cmsUpgrades::GlobalStub<T> tempGlobalStub( LocalStubPtrType(LocalStubHandle,i) , globalPosition , directionVector );
        const LocalStubPtrType refLocStub(LocalStubHandle,i);
        
        if (iEvent.isRealData() == false) {
          
          std::pair<int,bool> zxy = CheckFakeness(theStackedTracker,
                                                  thePixelDigiSimLink, 
                                                  refLocStub,
                                                  tempGlobalStub);

          if (zxy.second) tempGlobalStub.setGenuinity( false );
          else tempGlobalStub.setGenuinity( true );
          tempGlobalStub.setTrackID( zxy.first );
        }
        
        GlobalStubsForOutput->push_back( tempGlobalStub );
      }


      //std::cout  <<"Made " << GlobalStubsForOutput->size() << " global stubs of type " << (mClassInfo->TemplateTypes().begin()->second) << "." << std::endl;
      edm::OrphanHandle<GlobalStubCollectionType> GlobalStubHandle = iEvent.put(GlobalStubsForOutput);

    }

    virtual void endJob(){}

      
    // ----------member data ---------------------------
    const cmsUpgrades::StackedTrackerGeometry *theStackedTracker;
    cmsUpgrades::StackedTrackerGeometry::StackContainerIterator StackedTrackerIterator;

    edm::InputTag LocalStubsInputTag;

    const cmsUpgrades::classInfo *mClassInfo;


    std::pair<int,bool> CheckFakeness(const cmsUpgrades::StackedTrackerGeometry *theStackedTracker,
                    edm::Handle<edm::DetSetVector<PixelDigiSimLink> >  thePixelDigiSimLink, 
                    const LocalStubPtrType refLocStub,
                    cmsUpgrades::GlobalStub<T> tempGlobalStub
                  );
    
}; /// End of Class implementation

/// Specify Templates
/// Pixel Digis
template<>
  std::pair<int,bool> GlobalStubBuilder<cmsUpgrades::Ref_PixelDigi_>::CheckFakeness( const cmsUpgrades::StackedTrackerGeometry *theStackedTracker,
                                        edm::Handle<edm::DetSetVector<PixelDigiSimLink> >  thePixelDigiSimLink, 
                                        const LocalStubPtrType refLocStub,
                                        cmsUpgrades::GlobalStub<cmsUpgrades::Ref_PixelDigi_> tempGlobalStub
                                        ) 
  {

    cmsUpgrades::StackedTrackerDetId Id = tempGlobalStub.Id();
    const DetId id0=theStackedTracker->idToDet(Id,0)->geographicalId();
    const DetId id1=theStackedTracker->idToDet(Id,1)->geographicalId();
    //unsigned int subdetId0 = id0.subdetId();
    //unsigned int subdetId1 = id1.subdetId();
    bool isFAKE = true;
    /// Get corresponding Local Stub and hits
    /// composing the cluster in each module (0-1)

    const std::vector<cmsUpgrades::Ref_PixelDigi_> hits0= refLocStub->hit(0); /// Lower Cluster
    const std::vector<cmsUpgrades::Ref_PixelDigi_> hits1= refLocStub->hit(1); /// Upper Cluster
    edm::DetSet<PixelDigiSimLink> digiSimLink0 = (*thePixelDigiSimLink)[id0.rawId()];
    edm::DetSet<PixelDigiSimLink> digiSimLink1 = (*thePixelDigiSimLink)[id1.rawId()];
    std::vector<int> simTrackId0;
    std::vector<int> simTrackId1;

    /// Go on only if there are hits in module-0 (Lower Cluster)
    if (hits0.size()!=0) {
      /// Loop over hits in current module
      std::vector<cmsUpgrades::Ref_PixelDigi_>::const_iterator itLSHits0;
      for ( itLSHits0 = hits0.begin(); itLSHits0 != hits0.end(); itLSHits0++ ) {
        const cmsUpgrades::Ref_PixelDigi_ &hit = *itLSHits0;
        int theSimTrackId = -1;
        /// now "hit" is exacly like a PixelDigi object
        /// Loop over SimLink and get the SimTrack Id
        /// corresponding to current Pixel Digi
        edm::DetSet<PixelDigiSimLink>::const_iterator iterSimLink;
        for(iterSimLink = digiSimLink0.data.begin(); iterSimLink != digiSimLink0.data.end(); iterSimLink++){
          /// When the channel is the same, the link is found
          if((unsigned int)iterSimLink->channel()==(unsigned int)hit->channel()){
            theSimTrackId = iterSimLink->SimTrackId();
            simTrackId0.push_back(theSimTrackId);
          }
        }
      }
    } /// End of if (hits0.size()!=0)
    /// Go on only if there are hits in module-1 (Upper Cluster)
    /// SAME COMMENTS AS BEFORE
    if (hits1.size()!=0) {
      std::vector<cmsUpgrades::Ref_PixelDigi_>::const_iterator itLSHits1;
      for ( itLSHits1 = hits1.begin(); itLSHits1 != hits1.end(); itLSHits1++ ) {
        const cmsUpgrades::Ref_PixelDigi_ &hit = *itLSHits1;
        int theSimTrackId = -1;
        edm::DetSet<PixelDigiSimLink>::const_iterator iterSimLink;
        for(iterSimLink = digiSimLink1.data.begin(); iterSimLink != digiSimLink1.data.end(); iterSimLink++){
          if((unsigned int)iterSimLink->channel()==(unsigned int)hit->channel()){
            theSimTrackId = iterSimLink->SimTrackId();
            simTrackId1.push_back(theSimTrackId);
          }
        }
      }
    } /// End of if (hits1.size()!=0)
    /// Check pairs of SimTrack ID's to find fake Stubs:
    /// if there are pairs with different ID, this is a FAKE.
    /// If not, push back the ID of the SimTrack in the vector
    /// used to find DUPLICATES
    int tmpTrkID = -99999;
    if (simTrackId0.size()!=0 && simTrackId1.size()!=0) {
      for (unsigned int itID0=0; itID0<simTrackId0.size(); itID0++) {
        for (unsigned int itID1=0; itID1<simTrackId1.size(); itID1++) {
          if ( ( simTrackId0[itID0] == simTrackId1[itID1] ) && isFAKE ) {
            isFAKE = false; /// WARNING see comments in NicolaTauAnalyzer
            tmpTrkID = simTrackId0[itID0];
          }
        }
      }
    } /// End of Pair cross check

    std::pair<int,bool> abc;
    abc.first = tmpTrkID;
    abc.second = isFAKE; 

    return abc;
    
  }

/// Not Pixel Digis
template<typename T>
  std::pair<int,bool> GlobalStubBuilder<T>::CheckFakeness( const cmsUpgrades::StackedTrackerGeometry *theStackedTracker,
                                        edm::Handle<edm::DetSetVector<PixelDigiSimLink> >  thePixelDigiSimLink, 
                                        const LocalStubPtrType refLocStub,
                                        cmsUpgrades::GlobalStub<T> tempGlobalStub
                                        ) 
  {
    std::pair<int,bool> abc;
    abc.first = -9999;
    abc.second = true; 

    return abc;
  }

 

#endif


