
/*********************************/
/*********************************/
/**                             **/
/** Stacked Tracker Simulations **/
/**        Andrew W. Rose       **/
/**             2008            **/
/**                             **/
/*********************************/
/*********************************/

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "SLHCUpgradeSimulations/Utilities/interface/StackedTrackerGeometryRecord.h"
#include "SLHCUpgradeSimulations/Utilities/interface/StackedTrackerGeometry.h"
#include "SLHCUpgradeSimulations/Utilities/interface/StackedTrackerDetUnit.h"
#include "SLHCUpgradeSimulations/Utilities/interface/StackedTrackerDetId.h"

#include "SimDataFormats/SLHC/interface/StackedTrackerTypes.h"

#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"

#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/DetId/interface/DetId.h"

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/SiPixelDigi/interface/PixelDigi.h"

// output
#include <iostream>
#include <math.h>

//
// class decleration
//
using namespace std;
using namespace edm;
using namespace cmsUpgrades;

class stubproductiontest : public edm::EDAnalyzer {
   public:
      explicit stubproductiontest(const edm::ParameterSet&);
      ~stubproductiontest();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------
			ESHandle<StackedTrackerGeometry> StackedTrackerGeomHandle;
			const StackedTrackerGeometry *theStackedTrackers;
			StackedTrackerGeometry::StackContainerIterator StackedTrackerIterator;

			Handle< LocalStub_PSimHit_Collection > 				SimLocalStubHandle;
			Handle< GlobalStub_PSimHit_Collection > 			SimGlobalStubHandle;
			Handle< Tracklet_PSimHit_Collection > 				SimTrackletHandle;
 			map< DetId, vector<const LocalStub_PSimHit_ *> > 	SimLocalStubs;
			map< DetId, vector<const GlobalStub_PSimHit_ *> > 	SimGlobalStubs;
			map< DetId, vector<const Tracklet_PSimHit_ *> > 	SimTracklets;

			Handle< LocalStub_PixelDigi_Collection > 			DigiLocalStubHandle;
			Handle< GlobalStub_PixelDigi_Collection > 			DigiGlobalStubHandle;
			Handle< Tracklet_PixelDigi_Collection > 			DigiTrackletHandle;
			map< DetId, vector<const LocalStub_PixelDigi_ *> >	DigiLocalStubs;
			map< DetId, vector<const GlobalStub_PixelDigi_ *> > DigiGlobalStubs;
			map< DetId, vector<const Tracklet_PixelDigi_ *> > 	DigiTracklets;
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
stubproductiontest::stubproductiontest(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
}


stubproductiontest::~stubproductiontest()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
stubproductiontest::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Framework handles for the EVENT hits, digis, etc...

    StackedTrackerDetId id;

/////////////////////////////////////////////////////////////////////////////
	iEvent.getByLabel( "LocalStubsFromSimHits" , SimLocalStubHandle);
	const LocalStub_PSimHit_Collection *tempSimLocalStubs = SimLocalStubHandle.product();
	LocalStub_PSimHit_Collection::const_iterator tempSimLocalStubIter;

	for (  tempSimLocalStubIter = tempSimLocalStubs->begin(); tempSimLocalStubIter != tempSimLocalStubs->end() ; ++tempSimLocalStubIter ) {
		id = tempSimLocalStubIter->Id();
		SimLocalStubs[id].push_back(reinterpret_cast<const LocalStub_PSimHit_*>(&(* tempSimLocalStubIter)));
	}
/////////////////////////////////////////////////////////////////////////////
	iEvent.getByLabel( "GlobalStubsFromSimHits" , SimGlobalStubHandle);
	const GlobalStub_PSimHit_Collection *tempSimGlobalStubs = SimGlobalStubHandle.product();
	GlobalStub_PSimHit_Collection::const_iterator tempSimGlobalStubIter;

	for (  tempSimGlobalStubIter = tempSimGlobalStubs->begin(); tempSimGlobalStubIter != tempSimGlobalStubs->end() ; ++tempSimGlobalStubIter ) {
		id = tempSimGlobalStubIter->Id();
		SimGlobalStubs[id].push_back(reinterpret_cast<const GlobalStub_PSimHit_*>(&(* tempSimGlobalStubIter)));
	}
/////////////////////////////////////////////////////////////////////////////
	iEvent.getByLabel( "TrackletsFromSimHits" , "ShortTracklets" , SimTrackletHandle);
	const Tracklet_PSimHit_Collection *tempSimTracklets = SimTrackletHandle.product();
	Tracklet_PSimHit_Collection::const_iterator tempSimTrackletIter;

	for (  tempSimTrackletIter = tempSimTracklets->begin(); tempSimTrackletIter != tempSimTracklets->end() ; ++tempSimTrackletIter ) {
		id = tempSimTrackletIter->stub(0)->Id();
		SimTracklets[id].push_back(reinterpret_cast<const Tracklet_PSimHit_*>(&(* tempSimTrackletIter)));
	}
/////////////////////////////////////////////////////////////////////////////
	iEvent.getByLabel( "LocalStubsFromPixelDigis" , DigiLocalStubHandle);
	const LocalStub_PixelDigi_Collection *tempDigiLocalStubs = DigiLocalStubHandle.product();
	LocalStub_PixelDigi_Collection::const_iterator tempDigiLocalStubIter;

	for (  tempDigiLocalStubIter = tempDigiLocalStubs->begin(); tempDigiLocalStubIter != tempDigiLocalStubs->end() ; ++tempDigiLocalStubIter ) {
		id = tempDigiLocalStubIter->Id();
		DigiLocalStubs[id].push_back(reinterpret_cast<const LocalStub_PixelDigi_*>(&(* tempDigiLocalStubIter)));
	}
/////////////////////////////////////////////////////////////////////////////
	iEvent.getByLabel( "GlobalStubsFromPixelDigis" , DigiGlobalStubHandle);
	const GlobalStub_PixelDigi_Collection *tempDigiGlobalStubs = DigiGlobalStubHandle.product();
	GlobalStub_PixelDigi_Collection::const_iterator tempDigiGlobalStubIter;

	for (  tempDigiGlobalStubIter = tempDigiGlobalStubs->begin(); tempDigiGlobalStubIter != tempDigiGlobalStubs->end() ; ++tempDigiGlobalStubIter ) {
		id = tempDigiGlobalStubIter->Id();
		DigiGlobalStubs[id].push_back(reinterpret_cast<const GlobalStub_PixelDigi_*>(&(* tempDigiGlobalStubIter)));
	}
/////////////////////////////////////////////////////////////////////////////
	iEvent.getByLabel( "TrackletsFromPixelDigis" , "ShortTracklets" , DigiTrackletHandle);
	const Tracklet_PixelDigi_Collection *tempDigiTracklets = DigiTrackletHandle.product();
	Tracklet_PixelDigi_Collection::const_iterator tempDigiTrackletIter;

	for (  tempDigiTrackletIter = tempDigiTracklets->begin(); tempDigiTrackletIter != tempDigiTracklets->end() ; ++tempDigiTrackletIter ) {
		id = tempDigiTrackletIter->stub(0)->Id();
		DigiTracklets[id].push_back(reinterpret_cast<const Tracklet_PixelDigi_*>(&(* tempDigiTrackletIter)));
	}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	cout<< "Found "<<(theStackedTrackers->stacks().size())<<" stacks." <<endl;

	for (  StackedTrackerIterator = theStackedTrackers->stacks().begin(); StackedTrackerIterator != theStackedTrackers->stacks().end() ; ++StackedTrackerIterator ) {
		id = (**StackedTrackerIterator).Id() ;	
		int NSimLocalStubs=0;	
		int NSimGlobalStubs=0;	
		int NSimTracklets=0;	
		int NDigiLocalStubs=0;	
		int NDigiGlobalStubs=0;	
		int NDigiTracklets=0;	

		if ( SimLocalStubs.find( id ) != SimLocalStubs.end() ) 		NSimLocalStubs = SimLocalStubs.find( id )->second.size();
		if ( SimGlobalStubs.find( id ) != SimGlobalStubs.end() ) 	NSimGlobalStubs = SimGlobalStubs.find( id )->second.size();
		if ( SimTracklets.find( id ) != SimTracklets.end() ) 		NSimTracklets = SimTracklets.find( id )->second.size();
		if ( DigiLocalStubs.find( id ) != DigiLocalStubs.end() ) 	NDigiLocalStubs = DigiLocalStubs.find( id )->second.size();
		if ( DigiGlobalStubs.find( id ) != DigiGlobalStubs.end() ) 	NDigiGlobalStubs = DigiGlobalStubs.find( id )->second.size();
		if ( DigiTracklets.find( id ) != DigiTracklets.end() ) 		NDigiTracklets = DigiTracklets.find( id )->second.size();

		if ( NSimLocalStubs||NSimGlobalStubs||NSimTracklets||NDigiLocalStubs||NDigiGlobalStubs||NDigiTracklets){
			cout<<"---------------------------------------------------------------------"<<endl;
			if ( NSimLocalStubs ) 	cout << "stack: "<< id.layer() << ":" <<id.iPhi()<<":"<<id.iZ() << " contains " << NSimLocalStubs << 	" Local Stubs from Simhits." << endl; //<< id.UID()
			if ( NDigiLocalStubs ) 	cout << "stack: "<< id.layer() << ":" <<id.iPhi()<<":"<<id.iZ() << " contains " << NDigiLocalStubs << 	" Local Stubs from Digis." << endl; //<< id.UID()
			if ( NSimGlobalStubs ) 	cout << "stack: "<< id.layer() << ":" <<id.iPhi()<<":"<<id.iZ() << " contains " << NSimGlobalStubs << 	" Global Stubs from Simhits." << endl; //<< id.UID()
			if ( NDigiGlobalStubs ) cout << "stack: "<< id.layer() << ":" <<id.iPhi()<<":"<<id.iZ() << " contains " << NDigiGlobalStubs << 	" Global Stubs from Digis." << endl; //<< id.UID()
			if ( NSimTracklets ) 	cout << "stack: "<< id.layer() << ":" <<id.iPhi()<<":"<<id.iZ() << " contains " << NSimTracklets << 	" Starting Tracklets from Simhits." << endl; //<< id.UID()
			if ( NDigiTracklets ) 	cout << "stack: "<< id.layer() << ":" <<id.iPhi()<<":"<<id.iZ() << " contains " << NDigiTracklets << 	" Starting Tracklets from Digis." << endl; //<< id.UID()

			cout << "stack: "<< id <<endl;
			for(int i=0;i!=NSimLocalStubs;++i) 		cout << "Sim\n"  << SimLocalStubs.find( id )->second.at(i)->print(1);
			for(int i=0;i!=NDigiLocalStubs;++i) 	cout << "Digi\n" << DigiLocalStubs.find( id )->second.at(i)->print(1);
			for(int i=0;i!=NSimGlobalStubs;++i) 	cout << "Sim\n"  << SimGlobalStubs.find( id )->second.at(i)->print(1);
			for(int i=0;i!=NDigiGlobalStubs;++i) 	cout << "Digi\n" << DigiGlobalStubs.find( id )->second.at(i)->print(1);
			for(int i=0;i!=NSimTracklets;++i) 		cout << "Sim\n"  << SimTracklets.find( id )->second.at(i)->print(1);
			for(int i=0;i!=NDigiTracklets;++i) 		cout << "Digi\n" << DigiTracklets.find( id )->second.at(i)->print(1);
		}
	}


	SimLocalStubs.clear();
	SimGlobalStubs.clear();
	SimTracklets.clear();
	DigiLocalStubs.clear();
	DigiGlobalStubs.clear();
	DigiTracklets.clear();

}

// ------------ method called once each job just before starting event loop  ------------
void 
stubproductiontest::beginJob(const edm::EventSetup& iSetup)
{
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Framework handles for the EVENTSETUP tracker geometry, L1 stack geometry, etc...

 iSetup.get<StackedTrackerGeometryRecord>().get(StackedTrackerGeomHandle);
 theStackedTrackers = StackedTrackerGeomHandle.product();
}

// ------------ method called once each job just after ending the event loop  ------------
void 
stubproductiontest::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(stubproductiontest);
