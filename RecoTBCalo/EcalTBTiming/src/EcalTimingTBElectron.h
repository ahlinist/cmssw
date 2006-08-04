#ifndef EcalTimingTBElectron_H
#define EcalTimingTBElectron_H
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

#include <string>
#include "TProfile.h"
#include "TH1F.h"

//#include<fstream>
//#include<map>
//#include<stl_pair>

//

class EcalTimingTBElectron : public edm::EDAnalyzer {
   public:
      explicit EcalTimingTBElectron( const edm::ParameterSet& );
      ~EcalTimingTBElectron();


      virtual void analyze( const edm::Event&, const edm::EventSetup& );
      virtual void beginJob(edm::EventSetup const&);
      virtual void endJob();
 private:
      
      std::string rootfile_;
      std::string hitCollection_;
      std::string hitProducer_;
      std::string digiProducer_;
      std::string tdcRecInfoProducer_;
      std::string tdcRecInfoCollection_;

      float ampl_thr_;
      float chi2_thr_;
      int min_num_ev_;
      int sm_;
      int cry_;
      std::string txtFileName_;
      std::string txtFileForChGroups_;
      //std::string pndiodeProducer_;
      
      
      TProfile* amplProfile_;
      TProfile* absoluteTimingTDC_;
      TProfile* absoluteTimingNoTDC_;
      TProfile* Chi2Profile_;
      
      TProfile* relativeTiming_;

      TH1F* tTDC_;
      TH1F* tCry_;
      TH1F* CryTdc_;
      TH1F* cry_cry_;
      TProfile* tvsAmpl_;
      TProfile* amplProf_;
      
      int ievt_;
};



#endif
