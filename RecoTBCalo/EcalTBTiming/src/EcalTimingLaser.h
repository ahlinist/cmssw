#ifndef EcalTimingLaser_H
#define EcalTimingLaser_H
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

class EcalTimingLaser : public edm::EDAnalyzer {
   public:
      explicit EcalTimingLaser( const edm::ParameterSet& );
      ~EcalTimingLaser();


      virtual void analyze( const edm::Event&, const edm::EventSetup& );
      virtual void beginJob(edm::EventSetup const&);
      virtual void endJob();
 private:
      
      std::string rootfile_;
      std::string hitCollection_;
      std::string hitProducer_;
      std::string digiProducer_;
      float ampl_thr_;
      int min_num_ev_;
      int sm_;
      std::string txtFileName_;
      std::string txtFileForChGroups_;
      //std::string pndiodeProducer_;
      
      
      TProfile* amplProfileConv_[4];
      TProfile* absoluteTimingConv_[4];

      TProfile* amplProfileAll_[4];
      TProfile* absoluteTimingAll_[4];
      
      TProfile* Chi2ProfileConv_[4];
      
      TProfile* relativeTimingBlueConv_;
      
      TH1F* timeCry1; 
      TH1F* timeCry2; 
      
      int ievt_;
};



#endif
