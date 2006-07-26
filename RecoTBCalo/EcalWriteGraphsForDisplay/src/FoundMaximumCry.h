#ifndef  FoundMaximumCry_H
#define  FoundMaximumCry_H
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

class  FoundMaximumCry: public edm::EDAnalyzer {
   public:
      explicit FoundMaximumCry ( const edm::ParameterSet& );
      ~FoundMaximumCry();


      virtual void analyze( const edm::Event&, const edm::EventSetup& );
      virtual void beginJob(edm::EventSetup const&);
      virtual void endJob();
 private:
      
      std::string EBdigiCollection_;
      std::string digiProducer_;

      int ievt_;
            
};



#endif
