#ifndef  EcalWriteGraphsWindows_H
#define  EcalWriteGraphsWindows_H
/**\class EcalTimingLaser

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>

*/

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/EcalDetId/interface/EcalTrigTowerDetId.h"

#include <vector>
#include <string>
#include "TGraph.h"

//#include "RecoTBCalo/EcalWriteGraphsForDisplay/test/H4Geom.h"
//#include<fstream>
//#include<map>
//#include<stl_pair>


//

class EcalWriteGraphsWindows : public edm::EDAnalyzer {
   public:
      explicit EcalWriteGraphsWindows ( const edm::ParameterSet& );
      ~EcalWriteGraphsWindows();


      virtual void analyze( const edm::Event&, const edm::EventSetup& );
      virtual void beginJob(edm::EventSetup const&);
      virtual void endJob();
 private:
      
      std::string rootfile_;
      std::string EBdigiCollection_;
      std::string digiProducer_;

      std::vector<TGraph>  event_graphs_;
 
      int nx_;
      int ny_;
      std::string cryName_;
      int cry_;
      std::string gr_base_name_;
      int * window_;

      int ievt_;
      bool Setting0k_;
      
};



#endif
