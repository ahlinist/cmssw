#include <iostream>

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"

// essentials !!!
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ServiceRegistry/interface/Service.h" 
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH1.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"

//MC-TESTER header files
#include "Generate.h"
#include "HepMCEvent.H"
#include "Setup.H"

class MCTesterCMS : public edm::EDAnalyzer
{

   public:
   
      //
      explicit MCTesterCMS( const edm::ParameterSet& ) ;
      virtual ~MCTesterCMS() {} // no need to delete ROOT stuff
                                   // as it'll be deleted upon closing TFile
      
      virtual void analyze( const edm::Event&, const edm::EventSetup& ) ;
      virtual void beginJob() ;
      virtual void endRun( const edm::Run&, const edm::EventSetup& ) ;
      virtual void endJob() ;

   private:
   
     
}; 

using namespace edm;
using namespace std;

MCTesterCMS::MCTesterCMS( const ParameterSet& pset )
{

}

void MCTesterCMS::beginJob()
{
  
  // Service<TFileService> fs;
  // fNPartFinalState = fs->make<TH1D>(  "NPartFinalState",  "Number of final state particles", fNPart,  0., (double)fNPart );
    
  MC_Initialize();


  return ;
  
}

void MCTesterCMS::analyze( const Event& e, const EventSetup& )
{
    
   // here's an example of accessing particles in the event record (HepMCProduct)
   //
   Handle< HepMCProduct > EvtHandle ;
  
   // find initial (unsmeared, unfiltered,...) HepMCProduct
   //
   e.getByLabel( "generator", EvtHandle ) ;
  
   const HepMC::GenEvent* Evt = EvtHandle->GetEvent() ;

   HepMCEvent temp_event((*(HepMC::GenEvent*)Evt),false);
   MC_Analyze(&temp_event);
     
   return ;
   
}

void MCTesterCMS::endRun( const edm::Run& r, const edm::EventSetup& )
{

   return;

}


void MCTesterCMS::endJob()
{

   MC_Finalize();

   return ;
}
 
DEFINE_FWK_MODULE(MCTesterCMS);
