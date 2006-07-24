/**\class EcalTimingLaser

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  A. Ghezzi
//
//
#include "RecoTBCalo/EcalWriteGraphsForDisplay/src/EcalWriteGraphsWindows.h"
//#include "RecoTBCalo/EcalTimingLaser/srcgeomTable.h"
#include "DataFormats/EcalRecHit/interface/EcalUncalibratedRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/EcalDigi/interface/EcalPnDiodeDigi.h"
#include <DataFormats/EcalRawData/interface/EcalRawDataCollections.h>

#include "RecoTBCalo/EcalWriteGraphsForDisplay/src/H4GeomTB_old04.h"

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
EcalWriteGraphsWindows::EcalWriteGraphsWindows( const edm::ParameterSet& iConfig )
//========================================================================
{
   //now do what ever initialization is needed
   rootfile_           = iConfig.getUntrackedParameter<std::string>("rootfile","LaserTiming.root");
   digiProducer_       = iConfig.getParameter<std::string>("digiProducer");
   EBdigiCollection_   = iConfig.getParameter<std::string>("EBdigiCollection");
     
   cryName_   = iConfig.getUntrackedParameter<std::string>("crystal","850");
   nx_        = (int) (iConfig.getUntrackedParameter<double>("nx", 3.)); 
   ny_        = (int) (iConfig.getUntrackedParameter<double>("ny", 3.)); 
   cry_       = atoi(cryName_.c_str());
   gr_base_name_ = "ev_"+cryName_+"_graph_"; 
 
   Setting0k_ = true;
   window_ = 0;
}


//========================================================================
EcalWriteGraphsWindows::~EcalWriteGraphsWindows()
//========================================================================
{
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}

//========================================================================
void
EcalWriteGraphsWindows::beginJob(edm::EventSetup const&) {
//========================================================================
   if(!(nx_>0 && nx_%2 == 1 )||!(ny_>0 && ny_%2 == 1 )){cout<<" width and height has to be greater than 0 and odd!!!!"<<endl;
   Setting0k_ = false; return;}
   window_ = new int[nx_*ny_];
   H4GeomTB_old04 SMGeom; SMGeom.init();
   if(cry_<1 ||  cry_> 1700){cout<<" xtal ragne 1->1700"<<endl; Setting0k_=false; }
   SMGeom.getWindow(window_, cry_, nx_, ny_);
   
   ievt_ =0;
}

//========================================================================
void
EcalWriteGraphsWindows::endJob() {
//========================================================================


  TFile f(rootfile_.c_str(),"RECREATE");
  vector<TGraph>::iterator it = event_graphs_.begin();
  if (event_graphs_.size()==0){return ;}
  for( ; it !=  event_graphs_.end(); it++){
    it->Write();
  }
  f.Close();
}

//
// member functions
//

//========================================================================
void
EcalWriteGraphsWindows::analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup ) {
//========================================================================

   using namespace edm;
   using namespace cms;
   ievt_++;
   if(!Setting0k_){return;}
   bool Skip = ievt_ > 20 && ievt_ <610;
   Skip = Skip || ievt_ > 630 && ievt_ <1210;
   Skip = Skip || ievt_ > 1230 && ievt_ <1810;
   Skip = Skip || ievt_ > 1830;
   
   if(Skip){return;}
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
       for(EBDigiCollection::const_iterator itdg = EBdigis->begin(); itdg != EBdigis->end(); ++itdg) {
	 float sample[10], adc[10];
	 EBDataFrame dataframe = (*itdg);
	 EBDetId id = dataframe.id();

	 int this_ch_num = id.ic();
	 int ind_in_win = 0; bool Ch_in_wind=false;

	 for( int ik=0;ik<nx_*ny_;ik++){
	   if(this_ch_num == window_[ik]){
	     ind_in_win =ik;
	     Ch_in_wind = true;
	     //break;
	   }
	 }
	 if(!Ch_in_wind ) {continue;}
      
	 
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
      
	 char sevent[10];
	 sprintf(sevent,"%d",ievt_ -1);
	 char schan[4];
	 sprintf(schan,"%d",this_ch_num);

	 string this_graph = "Graph_"+string(sevent)+"_"+string(schan);
	 TGraph thisGraph(10,sample,adc);
	 thisGraph.SetTitle(this_graph.c_str());
	 thisGraph.SetName(this_graph.c_str());
	 //	 cout<<"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"<<endl;
	 event_graphs_.push_back(thisGraph);
	 //       }
       }
     }

}
