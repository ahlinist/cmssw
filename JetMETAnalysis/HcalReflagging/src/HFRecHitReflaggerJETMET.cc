// -*- C++ -*-
//
// Package:    HFRecHitReflaggerJETMET
// Class:      HFRecHitReflaggerJETMET
// 
/**\class HFRecHitReflaggerJETMET HFRecHitReflaggerJETMET.cc 

 Description: [Creates a new collection of rechits from an existing collection, and changes rechit flags according to new set of user-supplied conditions.  ]
 Code name is a bit of a misnomer -- this code only reflags HF rechits.  
 Other modules will reflag HBHE, etc.

*/
//
// Original Author:  Dinko Ferencek,8 R-004,+41227676479,  Jeff Temple, 6-1-027
//         Created:  Thu Mar 11 13:42:11 CET 2010
// $Id: HFRecHitReflaggerJETMET.cc,v 1.4 2010/05/19 09:20:27 temple Exp $
//
//

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "Geometry/HcalTowerAlgo/src/HcalHardcodeGeometryData.h" // for eta bounds

#include "CondFormats/HcalObjects/interface/HcalChannelStatus.h"
#include "CondFormats/HcalObjects/interface/HcalChannelQuality.h"
#include "CondFormats/HcalObjects/interface/HcalCondObjectContainer.h"
#include "CondFormats/DataRecord/interface/HcalChannelQualityRcd.h"
#include "FWCore/Framework/interface/ESHandle.h"

// Add all the algorithms we need for the reflagger
#include "RecoLocalCalo/HcalRecAlgos/interface/HcalSeverityLevelComputer.h"
#include "RecoLocalCalo/HcalRecAlgos/interface/HcalSeverityLevelComputerRcd.h"

#include "RecoLocalCalo/HcalRecAlgos/interface/HcalCaloFlagLabels.h"
#include "RecoLocalCalo/HcalRecAlgos/interface/HcalHF_PETalgorithm.h"
#include "RecoLocalCalo/HcalRecAlgos/interface/HcalHF_S9S1algorithm.h"

using namespace std;
using namespace edm;

//
// class declaration
//

class HFRecHitReflaggerJETMET : public edm::EDProducer {
   public:
      explicit HFRecHitReflaggerJETMET(const edm::ParameterSet&);
      ~HFRecHitReflaggerJETMET();

private:
  virtual void beginJob() ;
  virtual void beginRun(Run& r, const EventSetup& c);
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  void dumpInfo();
  void SetInTimeWindow(HFRecHit& hf);

  // ----------member data ---------------------------
  int debug_;
  edm::InputTag hfInputLabel_;
  
  // Booleans to indicate which flag to run
  std::vector<int> FlagsToSet_;


  // Set Up Flag Objects
  HcalHF_S9S1algorithm*       hfS9S1_;
  HcalHF_S9S1algorithm*       hfS8S1_;
  HcalHF_PETalgorithm*        hfPET_;

  int FlagsToSkip_HFInTimeWindow_;
  std::vector<double> HFlongwindowMinTime_;
  std::vector<double> HFlongwindowMaxTime_;
  double HFlongwindowEthresh_;
  std::vector<double> HFshortwindowMinTime_;
  std::vector<double> HFshortwindowMaxTime_;
  double HFshortwindowEthresh_;

  // Store channels marked as bad in the channel status map
  std::map<HcalDetId, unsigned int> badstatusmap;

  edm::Handle<HFRecHitCollection> hfRecHits;
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
HFRecHitReflaggerJETMET::HFRecHitReflaggerJETMET(const edm::ParameterSet& ps)
{
   //register your products
   produces<HFRecHitCollection>();

   // clear all pointers
   hfS9S1_=0;
   hfS8S1_=0;
   hfPET_=0;
   FlagsToSet_.clear();

   // get cfg values
   debug_               = ps.getUntrackedParameter<int>("debug",0);
   hfInputLabel_        = ps.getUntrackedParameter<InputTag>("hfInputLabel",edm::InputTag("hfreco"));

   FlagsToSet_          = ps.getParameter< std::vector<int> >("FlagsToSet");
   if (debug_>0)
     {
       cout <<"The following HF rechit flags will be set"<<endl;
       cout <<"(flag names may be found at https://twiki.cern.ch/twiki/bin/view/CMS/HcalRecHitFlagAssignments)"<<endl<<endl;
       for (unsigned int i=0;i<FlagsToSet_.size();++i)
	 cout <<FlagsToSet_[i]<<endl;
     }

   const edm::ParameterSet& psTIME = ps.getParameter<edm::ParameterSet>("HFInWindowStat");

   // TEST 
   if (debug_>0)
     cout <<"psTIME pset parameters = "<<psTIME<<endl;

   HFlongwindowMinTime_    = psTIME.getParameter<std::vector<double> >("hflongMinWindowTime");
   HFlongwindowMaxTime_    = psTIME.getParameter<std::vector<double> >("hflongMaxWindowTime");
   HFlongwindowEthresh_    = psTIME.getParameter<double>("hflongEthresh");
   HFshortwindowMinTime_   = psTIME.getParameter<std::vector<double> >("hfshortMinWindowTime");
   HFshortwindowMaxTime_   = psTIME.getParameter<std::vector<double> >("hfshortMaxWindowTime");
   HFshortwindowEthresh_   = psTIME.getParameter<double>("hfshortEthresh");
   FlagsToSkip_HFInTimeWindow_ = psTIME.getParameter<int>("flagsToSkip");

   const edm::ParameterSet& psPET    = ps.getParameter<edm::ParameterSet>("PETstat");

   if (debug_>0)
     cout <<"psPET pset parameters = "<<psPET<<endl;
   hfPET_    = new HcalHF_PETalgorithm(psPET.getParameter<std::vector<double> >("short_R"),
				       psPET.getParameter<std::vector<double> >("shortEnergyParams"),
				       psPET.getParameter<std::vector<double> >("shortETParams"),
				       psPET.getParameter<std::vector<double> >("long_R"),
				       psPET.getParameter<std::vector<double> >("longEnergyParams"),
				       psPET.getParameter<std::vector<double> >("longETParams"),
				       psPET.getParameter<int>("flagsToSkip"),
				       psPET.getParameter<std::vector<double> >("short_R_29"),
				       psPET.getParameter<std::vector<double> >("long_R_29")
				       );


   const edm::ParameterSet& psS9S1   = ps.getParameter<edm::ParameterSet>("S9S1stat");
   if (debug_>0)
     cout <<"psS9S1 pset parameters = "<<psS9S1<<endl;

   hfS9S1_   = new HcalHF_S9S1algorithm(psS9S1.getParameter<std::vector<double> >("short_optimumSlope"),
					psS9S1.getParameter<std::vector<double> >("shortEnergyParams"),
					psS9S1.getParameter<std::vector<double> >("shortETParams"),
					psS9S1.getParameter<std::vector<double> >("long_optimumSlope"),
					psS9S1.getParameter<std::vector<double> >("longEnergyParams"),
					psS9S1.getParameter<std::vector<double> >("longETParams"),
					psS9S1.getParameter<int>("flagsToSkip"),
					psS9S1.getParameter<bool>("isS8S1")
					);
   
   const edm::ParameterSet& psS8S1   = ps.getParameter<edm::ParameterSet>("S8S1stat");
   if (debug_>0)
     cout <<"psS8S1 pset parameters = "<<psS8S1<<endl;

   hfS8S1_   = new HcalHF_S9S1algorithm(psS8S1.getParameter<std::vector<double> >("short_optimumSlope"),
					psS8S1.getParameter<std::vector<double> >("shortEnergyParams"),
					psS8S1.getParameter<std::vector<double> >("shortETParams"),
					psS8S1.getParameter<std::vector<double> >("long_optimumSlope"),
					psS8S1.getParameter<std::vector<double> >("longEnergyParams"),
					psS8S1.getParameter<std::vector<double> >("longETParams"),
					psS8S1.getParameter<int>("flagsToSkip"),
					psS8S1.getParameter<bool>("isS8S1")
					);
   


}  //HFRecHitReflaggerJETMET::HFRecHitReflaggerJETMET()



HFRecHitReflaggerJETMET::~HFRecHitReflaggerJETMET()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}




void HFRecHitReflaggerJETMET::beginRun(Run& r, const EventSetup& c)
{
  // BEGIN RUN NOT YET WORKING?   WHY NOT?

  edm::ESHandle<HcalChannelQuality> p;
  c.get<HcalChannelQualityRcd>().get(p);
  HcalChannelQuality* chanquality_= new HcalChannelQuality(*p.product());

  std::vector<DetId> mydetids = chanquality_->getAllChannels();
  for (std::vector<DetId>::const_iterator i = mydetids.begin();i!=mydetids.end();++i)
    {
      if (i->det()!=DetId::Hcal) continue; // not an hcal cell
      HcalDetId id=HcalDetId(*i);
      int status=(chanquality_->getValues(id))->getValue();
      if ( (status & (1<<HcalChannelStatus::HcalCellDead))==0 ) continue;
      badstatusmap[id]=status;
    }
}


// ------------ method called to produce the data  ------------
void
HFRecHitReflaggerJETMET::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   // read HF RecHits
   if (!iEvent.getByLabel(hfInputLabel_,hfRecHits))
     {
       if (debug_>0) std::cout <<"Unable to find HFRecHitCollection with label '"<<hfInputLabel_<<"'"<<std::endl;
       return;
     }

   edm::ESHandle<HcalChannelQuality> p;
   iSetup.get<HcalChannelQualityRcd>().get(p);
   HcalChannelQuality* myqual = new HcalChannelQuality(*p.product());

   edm::ESHandle<HcalSeverityLevelComputer> mycomputer;
   iSetup.get<HcalSeverityLevelComputerRcd>().get(mycomputer);
   const HcalSeverityLevelComputer* mySeverity = mycomputer.product();



   // prepare the output HF RecHit collection
   std::auto_ptr<HFRecHitCollection> pOut(new HFRecHitCollection());
   for (HFRecHitCollection::const_iterator recHit=hfRecHits->begin(); recHit!=hfRecHits->end(); ++recHit) 
     {
       HFRecHit newhit = (HFRecHit)(*recHit);
       // clear all flags that will be used by reflagger
       for (unsigned int i=0;i<FlagsToSet_.size();++i)
	   newhit.setFlagField(0,FlagsToSet_[i]);
       // Add rechit to collection
       pOut->push_back(newhit);
     } // loop on original rechit collection
   
   // Now run reflaggers
   for (unsigned int i=0;i<FlagsToSet_.size();++i)
     {
       int value=FlagsToSet_[i];
       for (HFRecHitCollection::iterator newhit=pOut->begin(); newhit!=pOut->end();++newhit)
	 {
	   int depth=newhit->id().depth();
	   int ieta=newhit->id().ieta();

	   // Time window flag can be set at any point
	   if (value==2)
	     SetInTimeWindow(*newhit);

	   // For remaining flags, order matters.
	   // Set the HFPET bit; should be done first
	   if (value==4 && (depth==2 || abs(ieta)==29 )) 
		hfPET_->HFSetFlagFromPET(*newhit,*pOut,myqual,mySeverity);
	   // Set the HFS8S1Ratio flag next
	   else if (value==3 && (depth==2 || abs(ieta)==29 ))
	     hfS8S1_->HFSetFlagFromS9S1(*newhit,*pOut,myqual,mySeverity);
	   // Set the HFLongShort bit using S9S1; should generally be done last
	   else if (value==0 && depth==1 && abs(ieta)!=29)
	     hfS9S1_->HFSetFlagFromS9S1(*newhit,*pOut,myqual, mySeverity);
	 }

     }
   // put the re-flagged HF RecHit collection into the Event
   iEvent.put(pOut);
} // HFRecHitReflaggerJETMET::produce(....)

// ------------ method called once each job just before starting event loop  ------------
void 
HFRecHitReflaggerJETMET::beginJob()
{
  dumpInfo();
}

// ------------ method called once each job just after ending the event loop  ------------
void 
HFRecHitReflaggerJETMET::endJob() {
}


void HFRecHitReflaggerJETMET::SetInTimeWindow(HFRecHit& hf)
{
  // flagsToSkip shouldn't be necessary here (no dependence on other flags or other rechits),
  // but allow for the possibility of using it

  if ((FlagsToSkip_HFInTimeWindow_&HcalCaloFlagLabels::HFInTimeWindow)>0)
    return;
  // Remaining code copied directly from 
  // RecoLocalCalo/HcalRecAlgos/src/HcalHFStatusBitFromDigis.cc

  // FLAG:  HcalCaloLabels:: HFInTimeWindow
  // Timing algorithm
  if (hf.id().depth()==1)
    {
      if (hf.energy()>=HFlongwindowEthresh_)
	{
	  float mult=1./hf.energy();
	  float enPow=1.;
	  float mintime=0;
	  float maxtime=0;
	  for (unsigned int i=0;i<HFlongwindowMinTime_.size();++i)
	    {
	      mintime+=HFlongwindowMinTime_[i]*enPow;
	      maxtime+=HFlongwindowMaxTime_[i]*enPow;
	      enPow*=mult;
	    }
	  if (hf.time()<mintime || hf.time()>maxtime)
	    hf.setFlagField(1,HcalCaloFlagLabels::HFInTimeWindow);
	}
    }
  else if (hf.id().depth()==2)
    {
      if (hf.energy()>=HFshortwindowEthresh_)
	{
	  float mult=1./hf.energy();
	  float enPow=1.;
	  float mintime=0;
	  float maxtime=0;
	  for (unsigned int i=0;i<HFshortwindowMinTime_.size();++i)
	    {
	      mintime+=HFshortwindowMinTime_[i]*enPow;
	      maxtime+=HFshortwindowMaxTime_[i]*enPow;
	      enPow*=mult;
	    }
	  if (hf.time()<mintime || hf.time()>maxtime)
	    hf.setFlagField(1,HcalCaloFlagLabels::HFInTimeWindow);
	}
    }



}


void HFRecHitReflaggerJETMET::dumpInfo()
{

}



//define this as a plug-in
DEFINE_FWK_MODULE(HFRecHitReflaggerJETMET);
