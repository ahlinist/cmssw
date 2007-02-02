// -*- C++ -*-
//
// Package:    PrintIdealGeometryAnalyzer
// Class:      PrintIdealGeometryAnalyzer
// 
/**\class PrintIdealGeometryAnalyzer PrintIdealGeometryAnalyzer.cc GeometryReaders/PrintIdealGeometryAnalyzer/src/PrintIdealGeometryAnalyzer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Chris D Jones
//         Created:  Mon Jul 25 16:58:09 EDT 2005
// $Id: PrintIdealGeometryAnalyzer.cc,v 1.1 2005/07/25 21:01:15 chrjones Exp $
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

//
// class decleration
//

class PrintIdealGeometryAnalyzer : public edm::EDAnalyzer {
   public:
      explicit PrintIdealGeometryAnalyzer( const edm::ParameterSet& );
      ~PrintIdealGeometryAnalyzer();


      virtual void analyze( const edm::Event&, const edm::EventSetup& );
   private:
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
PrintIdealGeometryAnalyzer::PrintIdealGeometryAnalyzer( const edm::ParameterSet& iConfig )
{
   //now do what ever initialization is needed

}


PrintIdealGeometryAnalyzer::~PrintIdealGeometryAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
PrintIdealGeometryAnalyzer::analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
   using namespace edm;
#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif

#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   eventsetup::ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get( pSetup );
#endif
}

//define this as a plug-in
DEFINE_FWK_MODULE(PrintIdealGeometryAnalyzer);
