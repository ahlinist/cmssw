//## Simple code to access info in FEDRawDataCollection

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/FEDRawData/interface/FEDNumbering.h"
#include "EventFilter/HcalRawToDigi/interface/HcalHTRData.h"
#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"
#include "EventFilter/HcalRawToDigi/interface/HcalDCCHeader.h"
#include "DataFormats/FEDRawData/interface/FEDRawData.h"
#include "DataFormats/FEDRawData/interface/FEDHeader.h"

using namespace std;
using namespace edm;

#include "RecoHcal/HcalPromptAnalysis/interface/AccessFEDinfo.h"

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
AccessFEDinfo::AccessFEDinfo(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
}


AccessFEDinfo::~AccessFEDinfo()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
AccessFEDinfo::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif


   //######## L1 Event ID from GT FED
   edm::Handle<FEDRawDataCollection> theRaw ;
   iEvent.getByLabel("hltAlCaHcalFEDSelector",theRaw) ;
   const FEDRawData& data = theRaw->FEDData(FEDNumbering::MINTriggerGTPFEDID) ;
   FEDHeader header(data.data()) ;
   cout << "L1IDNumber = " <<  header.lvl1ID() << endl;

   //######## HCAL stuff
   edm::Handle<FEDRawDataCollection> rawraw;
   //iEvent.getByLabel("source",rawraw);
   iEvent.getByLabel("hltAlCaHcalFEDSelector",rawraw);
   
   vector<int> fedUnpackList_;
   for (int i=FEDNumbering::MINHCALFEDID; i<=FEDNumbering::MAXHCALFEDID; i++) 
     {
       fedUnpackList_.push_back(i);
     }

   // Loop over all FEDs reporting the event
   int N_US_Spigots=0;

   for (vector<int>::const_iterator i=fedUnpackList_.begin();i!=fedUnpackList_.end(); i++) 
     {

       const FEDRawData& raw = rawraw->FEDData(*i);
       const HcalDCCHeader* dccHeader=(const HcalDCCHeader*)(raw.data());

       FEDHeader headerHcal(raw.data()) ;
       cout << "FED " << *i << "  ,  L1number = " << headerHcal.lvl1ID() << endl;

       // 	int EvFragLength = raw.size();
       // 	unsigned long dccEvtNum = dccHeader->getDCCEventNumber();
       // 	int dccBCN = dccHeader->getBunchId();
       // 	int dccOrN = dccHeader->getOrbitNumber();
       
	// 	//some print out
	// 	cout << "EvFragLength: " << EvFragLength << endl;
	// 	cout << "dccEvtNum: " << dccEvtNum << endl;
	// 	cout << "dccBCN: " << dccBCN << endl;
	// 	cout << "dccOrN: "<<  dccOrN << endl;

	HcalHTRData htr; 
	for (int spigot=0; spigot<HcalDCCHeader::SPIGOT_COUNT; spigot++) {    
	  if (!dccHeader->getSpigotPresent(spigot)) continue;

	  dccHeader->getSpigotData(spigot, htr, raw.size());
	  const unsigned short* HTRraw = htr.getRawData();

	  // This is the US bit that you are interested in
	  bool htrUnSuppressed=(HTRraw[6]>>15 & 0x0001);
	  bool htrEmpty=(HTRraw[2] & 0x4);

	  if(htrUnSuppressed==true)
	    N_US_Spigots++;

	  if(htrUnSuppressed==false)
	    cout << "##### Spigot " << spigot << " in FED " << *i << " is not US ######" << endl;
	  
	  if(htrEmpty==true)
	    cout << "##### Spigot " << spigot << " in FED " << *i << " is an EMPTY EVENT ######" << endl;
	  
	}//loop over spigots 

     }//loop over FEDs

   //counters
   //int N_tot_Spigots = fedUnpackList_.size() * HcalDCCHeader::SPIGOT_COUNT;
   // cout  << "fedUnpackList_.size()" << fedUnpackList_.size() << endl;
   // cout << "N_tot_Spigots" << N_tot_Spigots << endl;

   //check is event is unsuppressed
   //if(N_US_Spigots == N_tot_Spigots)
   if(N_US_Spigots>0)
     {
       CountUSEvents++;
       cout <<  "---> N_US_Spigots: " << N_US_Spigots << endl;
       cout << "---> US event number: " << iEvent.id().event() << endl;
     }
   CountTotEvents++;

}


// ------------ method called once each job just before starting event loop  ------------
void 
AccessFEDinfo::beginJob()
{
  CountTotEvents=0;
  CountUSEvents=0;
  ratioUSevents=0.;
}

// ------------ method called once each job just after ending the event loop  ------------
void 
AccessFEDinfo::endJob() {

  ratioUSevents = float(CountUSEvents)/float(CountTotEvents);

  cout << "CountTotEvents: " << CountTotEvents << endl;
  cout << "CountUSEvents: " << CountUSEvents << endl;
  cout << "ratioUSevents: " << ratioUSevents << endl;

}
