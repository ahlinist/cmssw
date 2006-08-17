#ifndef EcalTimingTBElectronGeneral_H
#define EcalTimingTBElectronGeneral_H
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

class EcalTimingTBElectronGeneral : public edm::EDAnalyzer {
   public:
      explicit EcalTimingTBElectronGeneral( const edm::ParameterSet& );
      ~EcalTimingTBElectronGeneral();


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

      std::string txtFileName_;
      std::string txtFileForChGroups_;
      //std::string pndiodeProducer_;
      
      
      //TProfile* amplProfile_;
      TH1F* absoluteTimingTDC_[1701];
      TH1F* absoluteTimingNoTDC_[1701];
      TH1F* relativeTiming_[1701];
      TProfile* tvsAmpl_[1701];

      TProfile* Chi2Profile_;
      
      TH1F* tTDC_;
            
      int ievt_;
};



#endif
