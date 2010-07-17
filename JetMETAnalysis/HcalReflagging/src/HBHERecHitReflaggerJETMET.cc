// -*- C++ -*-
//
// Package:    HBHERecHitReflaggerJETMET
// Class:      HBHERecHitReflaggerJETMET
// 
/**\class HBHERecHitReflaggerJETMET HBHERecHitReflaggerJETMET.cc 

 Description: [Creates a new collection of rechits from an existing collection, and changes rechit flags according to new set of user-supplied conditions.  ]
 Code name is a bit of a misnomer -- this code only reflags HF rechits.  
 Other modules will reflag HBHE, etc.

*/
//
// Original Author:  Dinko Ferencek,8 R-004,+41227676479,  Jeff Temple, 6-1-027
//         Created:  Thu Mar 11 13:42:11 CET 2010
// $Id: HBHERecHitReflaggerJETMET.cc,v 1.7 2010/06/15 13:37:17 temple Exp $
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

#include "CalibCalorimetry/HcalAlgos/interface/HcalLogicalMapGenerator.h"
#include "CondFormats/HcalObjects/interface/HcalLogicalMap.h"

#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"

#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"
#include "RecoLocalCalo/HcalRecAlgos/interface/HcalCaloFlagLabels.h"
#include "Geometry/HcalTowerAlgo/src/HcalHardcodeGeometryData.h" // for eta bounds

#include "CondFormats/HcalObjects/interface/HcalChannelStatus.h"
#include "CondFormats/HcalObjects/interface/HcalChannelQuality.h"
#include "CondFormats/HcalObjects/interface/HcalCondObjectContainer.h"
#include "CondFormats/DataRecord/interface/HcalChannelQualityRcd.h"
#include "FWCore/Framework/interface/ESHandle.h"


using namespace std;
using namespace edm;

//
// class declarations
//


// Use this class to compare Energies
template <class T>
class compareEnergyTimePair {
public:
  bool operator()(const T& h1,
                  const T& h2) const {
    return (h1.first < h2.first);
  }
};



class HBHERecHitReflaggerJETMET : public edm::EDProducer {
   public:
      explicit HBHERecHitReflaggerJETMET(const edm::ParameterSet&);
      ~HBHERecHitReflaggerJETMET();

private:
  virtual void beginJob() ;
  virtual void beginRun(Run& r, const EventSetup& c);
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  void dumpInfo();
  void SetTimingShapedFlags(HBHERecHit& hbhe); // Phil's flagging algorithm

  // ----------member data ---------------------------
  int debug_;
  edm::InputTag hbheInputLabel_;
  int  hbheFlagBit_;
  
  // specify energy threshold
  double  Ethresh_;
  // specify number of channels/hpd that must be above threshold for flagging to occur
  int Nhits_;

  // Basic test checks # of channels/hpd above threshold.  Enable this bool to convert test to
  // # of channels/rbx  (1 HPD = 18 channels, 1 RBX = 4 HPDs = 72 channels)
  bool RBXflag_; 

  // Add extra parameters here
  // ability to mark only some part of HPD?  Don't flag rechits above/below certain energy?

  // Store channels marked as bad in the channel status map
  std::map<HcalDetId, unsigned int> badstatusmap; // Is this necessary for HBHE reflagger?


  HcalLogicalMap *logicalMap_;  // needed for HPD/RBX mapping
  edm::Handle<HBHERecHitCollection> hbheRecHits;  // HBHE rec hit collection


  // Variables for Phil's timing algorithm
  std::vector<std::pair<double,double> > tfilterEnvelope_;
  bool ignorelowest_;
  bool ignorehighest_;
  double win_offset_;
  double win_gain_;
  int hbheTimingFlagBit_;

  // event counter
  int evtcount_;
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
HBHERecHitReflaggerJETMET::HBHERecHitReflaggerJETMET(const edm::ParameterSet& ps)
{

   //register your products
   produces<HBHERecHitCollection>();
   debug_              = ps.getUntrackedParameter<int>("debug",0);
   hbheInputLabel_     = ps.getUntrackedParameter<InputTag>("hbheInputLabel",edm::InputTag("hbhereco"));
   hbheFlagBit_        = ps.getUntrackedParameter<int>("hbheFlagBit",HcalCaloFlagLabels::HBHEHpdHitMultiplicity); 
   Nhits_              = ps.getUntrackedParameter<int>("Nhits",14);
   Ethresh_            = ps.getUntrackedParameter<double>("Ethresh",0.5);
   RBXflag_            = ps.getUntrackedParameter<bool>("RBXflag",false); // if specified, flag on number of hits within an RBX, rather than within an HPD

   // Phil's timing algorithm
   const edm::ParameterSet& psTshaped = ps.getParameter<edm::ParameterSet>("timingshapedcutsParamet\
ers");
   std::vector<double> dummy= psTshaped.getParameter<std::vector<double> >("tfilterEnvelope");
   // His 'tfilterEnvelope' is a set of (energy,time) pairs.
   // We need to convert the vector of doubles (as read in from the cfg) to a vector of pairs
   for (unsigned int i=0;i<dummy.size();i+=2)
     {
       if (i+1<dummy.size())
	 {
	   std::pair<double,double> temp(dummy[i],dummy[i+1]);
	   tfilterEnvelope_.push_back(temp);
	 }
     }
   // All other variables come directly from cfg file
   ignorelowest_    = psTshaped.getParameter<bool>("ignorelowest");
   ignorehighest_   = psTshaped.getParameter<bool>("ignorehighest");
   win_offset_      = psTshaped.getParameter<double>("win_offset");
   win_gain_        = psTshaped.getParameter<double>("win_gain");
   // Use hbheTimingFlagBit to set the appropriate flag position
   hbheTimingFlagBit_   = psTshaped.getUntrackedParameter<int>("hbheTimingFlagBit",HcalCaloFlagLabels::HBHETimingShapedCutsBits); 
   evtcount_=0;
}  //HBHERecHitReflaggerJETMET::HBHERecHitReflaggerJETMET()



HBHERecHitReflaggerJETMET::~HBHERecHitReflaggerJETMET()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}




void HBHERecHitReflaggerJETMET::beginRun(Run& r, const EventSetup& c)
{
  // Get known bad channels from channel status db
  // This is an example of how to get the list of dead channels -- not needed here
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
HBHERecHitReflaggerJETMET::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  if (evtcount_%1000==0 && debug_>0) cout <<"HBHE rechit Reflagger"<<endl;

   // read HBHE RecHits
   if (!iEvent.getByLabel(hbheInputLabel_,hbheRecHits))
     {
       if (debug_>0) std::cout <<"Unable to find HBHERecHitCollection with label '"<<hbheInputLabel_<<"'"<<std::endl;
       return;
     }

   // prepare the output HBHE RecHit collection
   std::auto_ptr<HBHERecHitCollection> pOut(new HBHERecHitCollection());
   
   // map of HPDs and number of channels above threshold
   std::map<int, int> HPDmap;
   std::map<int, int> RBXmap;  // tracks the number of channels above threshold in an RBX.  Might be useful to create a separate flag on RBX noise?

   // initial loop on rechits to determine number of channels> threshold in each HPD, RBX


   for (HBHERecHitCollection::const_iterator recHit=hbheRecHits->begin(); recHit!=hbheRecHits->end(); ++recHit) 
     {
       if (recHit->energy()<Ethresh_) continue; // impose energy requirement
       int HPDindex=logicalMap_->getHcalFrontEndId(recHit->detid()).rmIndex();
       int RBXindex=logicalMap_->getHcalFrontEndId(recHit->detid()).rbxIndex();
       ++HPDmap[HPDindex];
       ++RBXmap[RBXindex];
     }

   // loop over rechits, and set the new bit you wish to use
   for (HBHERecHitCollection::const_iterator recHit=hbheRecHits->begin(); recHit!=hbheRecHits->end(); ++recHit) 
     {
       HBHERecHit newhit = (HBHERecHit)(*recHit);
       HcalDetId id(newhit.detid().rawId());
       //int depth = newhit.id().depth();
       //int ieta  = newhit.id().ieta();
       int HPDindex=logicalMap_->getHcalFrontEndId(recHit->detid()).rmIndex();
       int RBXindex=logicalMap_->getHcalFrontEndId(recHit->detid()).rbxIndex();
       // Add rechit to collection
       
       if (
	   (HPDmap[HPDindex]>=Nhits_ && RBXflag_==false) || // HPD requirement
	   (RBXmap[RBXindex]>=Nhits_ && RBXflag_==true)  // RBX requirement
	   )
	 {
	   newhit.setFlagField(1,hbheFlagBit_);
	   if (debug_>1) cout <<"HBHE Noise Flag Bit "<<hbheFlagBit_<<" set for Channel "<<newhit.id()<<endl;
	 }
       else
	 newhit.setFlagField(0,hbheFlagBit_);

       if (debug_>5) cout <<"HBHE Noise Flag Bit for "<<newhit.id()<<"  = "<<newhit.flagField(hbheFlagBit_)<<endl;

       // Set Phil's timing bit
       SetTimingShapedFlags(newhit);
       
       pOut->push_back(newhit);
     }

   // put the re-flagged HBHE RecHit collection into the Event
   if (debug_>2) cout <<"Adding new HBHE rechit collection"<<endl;
   iEvent.put(pOut);
   ++evtcount_;
}

// ------------ method called once each job just before starting event loop  ------------
void 
HBHERecHitReflaggerJETMET::beginJob()
{
  HcalLogicalMapGenerator gen;
  logicalMap_=new HcalLogicalMap(gen.createMap());

  dumpInfo();
}

// ------------ method called once each job just after ending the event loop  ------------
void 
HBHERecHitReflaggerJETMET::endJob() {
}


// Print out all parameters used in the reflagger



void HBHERecHitReflaggerJETMET::dumpInfo()
{
  // Dump out all the parameters used in the reflagger
  if (debug_==0) return;

  cout <<"  Dumping out all parameters!"<<endl;
  cout <<" HPD Hit Multiplicity flag bit set to : "<<hbheFlagBit_<<endl;
  cout <<"  An HPD must have at least "<<Nhits_<<" greater than "<<Ethresh_<<" to be considered noisy."<<endl;
  cout <<"  If this is condition is met, ALL channels in the HPD are flagged as noisy."<<endl;
  
  cout <<"\n\nHBHE Timing Flag will be written to flag bit "<<hbheTimingFlagBit_<<endl;
  cout <<"Timing Energy/Time parameters are:"<<endl;
  for (unsigned int i=0;i<tfilterEnvelope_.size();++i)
    cout <<"\t"<<tfilterEnvelope_[i].first<<"\t"<<tfilterEnvelope_[i].second<<endl;
  cout <<"ignorelowest  = "<<ignorelowest_<<endl;
  cout <<"ignorehighest = "<<ignorehighest_<<endl;
  cout <<"win_offset    = "<<win_offset_<<endl;
  cout <<"win_gain      = "<<win_gain_<<endl;
}// dumpInfo()


void HBHERecHitReflaggerJETMET::SetTimingShapedFlags(HBHERecHit& hbhe)
{
  // This is Phil's algorithm, taken almost directly from RecoLocalCalo/HcalRecAlgos/src/HBHETimingShapedFlag.cc 
  // Only difference is that status is treated as a single bit, instead of a set of 3 bits
  // (only one bit is currently used in HBHETimingShapedFlag.cc, though, so there's effectively no difference)

  int status=0;  // treat as single bit, although three bits are allowed in the actual rechit flag (within RecoLocalCalo/HcalRecAlgos)

  // tfilterEnvelope stores doubles of energy and time; 
  //make sure we're checking over an even number of values
  // energies are also assumed to appear in increasing order

  // need at least two values to make comparison, and must
  // always have energy, time pair; otherwise, assume "in time" and don't set bits
  if (tfilterEnvelope_.size()==0)
    return;

  double twinmin, twinmax;  // min, max 'good' time; values outside this range have flag set
  double rhtime=hbhe.time();
  double energy=hbhe.energy();
  unsigned int i=0; // index to track envelope index

  if (energy<tfilterEnvelope_[0].first) // less than lowest energy threshold
    {
      // Can skip timing test on cells below lowest threshold if so desired
      if (ignorelowest_) 
	return;
      else
	twinmax=tfilterEnvelope_[0].second;
    }
  else
    {
      // Loop over energies in tfilterEnvelope
      for (i=0;i<tfilterEnvelope_.size();++i)
	{
	  // Identify tfilterEnvelope index for this rechit energy
	  if (tfilterEnvelope_[i].first>energy)
	    break;
	}

      if (i==tfilterEnvelope_.size())
	{
	  // Skip timing test on cells above highest threshold if so desired
	  if (ignorehighest_)
	    return;
	  else
	    twinmax=tfilterEnvelope_[i-1].second;
	}
      else
	{
	  // Perform linear interpolation between energy boundaries

	  double energy1  = tfilterEnvelope_[i-1].first;
	  double lim1     = tfilterEnvelope_[i-1].second;
	  double energy2  = tfilterEnvelope_[i].first;
	  double lim2     = tfilterEnvelope_[i].second;
	
	  twinmax=lim1+((lim2-lim1)*(energy-energy1)/(energy2-energy1));
	}
    }
  // Apply offset, gain
  twinmax=win_offset_+twinmax*win_gain_;  
  twinmin=win_offset_-twinmax*win_gain_;
  // Set status high if time outside expected range
  if (rhtime<=twinmin || rhtime >= twinmax)
    status=1; // set status to 1
  if (status>0 && debug_>0)
    cout <<"HBHE Timing Flag Bit for "<<hbhe.id()<<"   is set; \tEnergy  = "<<hbhe.energy()<<"  time = "<<hbhe.time()<<endl;
    
  if (status>0)
    hbhe.setFlagField(1,hbheTimingFlagBit_);
  else
    hbhe.setFlagField(0,hbheTimingFlagBit_);
  return;
} //SetTimingShapedFlags(HBHERecHit& hbhe)




//define this as a plug-in
DEFINE_FWK_MODULE(HBHERecHitReflaggerJETMET);
