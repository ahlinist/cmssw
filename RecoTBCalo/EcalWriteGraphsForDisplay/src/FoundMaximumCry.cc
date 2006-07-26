/**\class EcalTimingLaser

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  A. Ghezzi
//
//
#include "RecoTBCalo/EcalWriteGraphsForDisplay/src/FoundMaximumCry.h"
//#include "RecoTBCalo/EcalTimingLaser/srcgeomTable.h"
#include "DataFormats/EcalRecHit/interface/EcalUncalibratedRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/EcalDigi/interface/EcalPnDiodeDigi.h"
#include <DataFormats/EcalRawData/interface/EcalRawDataCollections.h>


#include<fstream>
#include <iomanip>
#include <iostream>
#include "TFile.h"
#include<string>
//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//

using namespace std;
//========================================================================
FoundMaximumCry::FoundMaximumCry( const edm::ParameterSet& iConfig )
//========================================================================
{
   //now do what ever initialization is needed
  digiProducer_       = iConfig.getParameter<std::string>("digiProducer");
  EBdigiCollection_   = iConfig.getParameter<std::string>("EBdigiCollection");
     
}


//========================================================================
FoundMaximumCry::~FoundMaximumCry()
//========================================================================
{
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}

//========================================================================
void
FoundMaximumCry::beginJob(edm::EventSetup const&) {
//========================================================================
 
   ievt_ =0;
}

//========================================================================
void
FoundMaximumCry::endJob() {
//========================================================================


}

//
// member functions
//

//========================================================================
void
FoundMaximumCry::analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup ) {
//========================================================================

   using namespace edm;
   using namespace cms;
   ievt_++;

   float defaultPede = 200;

    Handle< EBDigiCollection > pEBDigis;
   const EBDigiCollection* EBdigis =0;
      
   try {
     iEvent.getByLabel( digiProducer_, EBdigiCollection_, pEBDigis);
     //evt.getByLabel( digiProducer_, pEBDigis);
     EBdigis = pEBDigis.product(); // get a ptr to the produc
     cout<< "EcalAnalFitUncalibRecHitProducer: total # EBdigis: " << EBdigis->size() <<endl;
   } catch ( std::exception& ex ) {
     //     edm::LogError("EcalUncalibRecHitError") << "Error! can't get the product " << EBdigiCollection_.c_str() ;
   }
   
   if (EBdigis)
     {
       float maxAmpl=0;
       int maxcry = 0;
       for(EBDigiCollection::const_iterator itdg = EBdigis->begin(); itdg != EBdigis->end(); ++itdg) {
	 float sample[10], adc[10];
	 EBDataFrame dataframe = (*itdg);
	 EBDetId id = dataframe.id();

	 int this_ch_num = id.ic();
	 
	 for (int i = 0; i < 10; i++) {
	   sample[i]=i+1; adc[i]=0;
	   EcalMGPASample sample = dataframe.sample(i);
	   int madc = sample.adc();
	   int gainid = sample.gainId();
	   if ( gainid == 1 ){//gain 12 
	     adc[i] = (float(madc)); 
	   }
	   else if ( gainid == 2 ){// gain 6 
	     adc[i] = (float(madc)-defaultPede)*2 + defaultPede;
	   }
	   else if ( gainid == 3 ){// gain 1 
	     adc[i] = (float(madc)-defaultPede)*12 + defaultPede;
	   }
	   else{adc[i] = -100;}
	 }// end of loop over samples
	 
	 float pede = (adc[0]+adc[1])/2. ;
	 float ampl =0;
	 for(int i=0;i<10;i++){
	   if ( ( adc[i]- pede ) > ampl){
	     ampl = adc[i] - pede;
	   } 
	 }
	 
	 if(ampl > maxAmpl){
	   maxAmpl = ampl;
	   maxcry = this_ch_num;
	 }
	 
       }
       cout<<"Event: "<<ievt_<<" max cry: "<<maxcry<<" ampl: "<<maxAmpl<<endl;
     }
   else{ cout<<"Event: "<<ievt_<<" No digis found !!!"<<endl;}
}
