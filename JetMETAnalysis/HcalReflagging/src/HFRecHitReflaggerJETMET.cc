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
// $Id: HFRecHitReflaggerJETMET.cc,v 1.3 2010/04/20 20:43:46 temple Exp $
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
  // Threshold function gets values from polynomial-parameterized functions
  double GetThreshold(const int base, const std::vector<double>& params);
  double GetThreshold(const double base, const std::vector<double>& params);

  // Perform a check of a rechit's S9/S1 value compared to a given threshold
  bool   CheckS9S1(const HFRecHit& hf); 

  // Perform a check of a rechit's |L-S|/|L+S| ratio, compared to a paremeterized threshold
  bool   CheckPET(const HFRecHit& hf);

  // Get S9S1, PET values
  double GetS9S1value(const HFRecHit& hf);
  double GetPETvalue(const HFRecHit& hf);

  // ----------member data ---------------------------
  int debug_;
  edm::InputTag hfInputLabel_;
  int  hfFlagBit_;
  // Select the test you wish to run
  bool hfBitAlwaysOn_;
  bool hfBitAlwaysOff_;

  // Short fibers
  std::vector<int> hf_Short_Use_PET_;
  std::vector< std::vector<double> > hf_Short_PET_Energy_Thresh_; // runs from |ieta|=29 to 41
  std::vector< std::vector<double> > hf_Short_PET_ET_Thresh_;
  bool hf_Short_Rcut_parameterizeET_;
  std::vector< std::vector<double> > hf_Short_Rcut_values_;
  std::vector< std::vector<double> > hf_Short_S9S1_Energy_Thresh_;
  std::vector< std::vector<double> > hf_Short_S9S1_ET_Thresh_;
  std::vector< std::vector<double> > hf_Short_S9S1_Slope_Intercept_;

  // Long fibers
  std::vector<int> hf_Long_Use_PET_;
  std::vector< std::vector<double> > hf_Long_PET_Energy_Thresh_; // runs from |ieta|=29 to 41
  std::vector< std::vector<double> > hf_Long_PET_ET_Thresh_;
  bool hf_Long_Rcut_parameterizeET_;
std::vector< std::vector<double> > hf_Long_Rcut_values_;
  std::vector< std::vector<double> > hf_Long_S9S1_Energy_Thresh_;
  std::vector< std::vector<double> > hf_Long_S9S1_ET_Thresh_;
  std::vector< std::vector<double> > hf_Long_S9S1_Slope_Intercept_;

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
   debug_               = ps.getUntrackedParameter<int>("debug",0);
   hfInputLabel_        = ps.getUntrackedParameter<InputTag>("hfInputLabel",edm::InputTag("hfreco"));
   hfFlagBit_           = ps.getUntrackedParameter<int>("hfFlagBit",HcalCaloFlagLabels::HFLongShort); 
      
   // sanity check bits -- turn bit on or off always
   hfBitAlwaysOn_       = ps.getUntrackedParameter<bool>("hfBitAlwaysOn",false);
   hfBitAlwaysOff_      = ps.getUntrackedParameter<bool>("hfBitAlwaysOff",false);

   stringstream name;
   
   // Short Fiber Parameters
   hf_Short_Use_PET_    = ps.getParameter<std::vector<int> >("hf_Short_Use_PET");
   name.str("");
   const edm::ParameterSet& HF_Short_PET_Energy_Thresh = ps.getParameter<edm::ParameterSet>("hf_Short_PET_Energy_Thresh");
   //std::vector<double> junk=(HF_Short_PET_Energy_Thresh.getParameter<std::vector<double> >("hf_Short_PET_energyThresh29"));
   for (int i=29;i<=41;++i)
     {
       name<<"hf_Short_PET_energyThresh"<<i;
       std::vector<double> junk=(HF_Short_PET_Energy_Thresh.getParameter<std::vector<double> >(name.str().c_str()));
       hf_Short_PET_Energy_Thresh_.push_back(junk);

       name.str("");
     }
   
   const edm::ParameterSet& HF_Short_PET_ET_Thresh = ps.getParameter<edm::ParameterSet>("hf_Short_PET_ET_Thresh");
   for (int i=29;i<=41;++i)
     {
       name<<"hf_Short_PET_ETThresh"<<i;
       std::vector<double> junk=(HF_Short_PET_ET_Thresh.getParameter<std::vector<double> >(name.str().c_str()));
       hf_Short_PET_ET_Thresh_.push_back(junk);
       name.str("");
     }
   const edm::ParameterSet&  HF_Short_Rcut = ps.getParameter<edm::ParameterSet>("hf_Short_Rcut");
   hf_Short_Rcut_parameterizeET_=HF_Short_Rcut.getUntrackedParameter<bool>("hf_Short_Rcut_parameterizeET",false);
   for (int i=29;i<=41;++i)
     {
       name<<"hf_Short_Rcut_values"<<i;
       std::vector<double> junk=(HF_Short_Rcut.getParameter<std::vector<double> >(name.str().c_str()));
       hf_Short_Rcut_values_.push_back(junk);
       name.str("");
     }

   const edm::ParameterSet& HF_Short_S9S1_Energy_Thresh = ps.getParameter<edm::ParameterSet>("hf_Short_S9S1_Energy_Thresh");
   for (int i=29;i<=41;++i)
     {
       name<<"hf_Short_S9S1_energyThresh"<<i;
       std::vector<double> junk=(HF_Short_S9S1_Energy_Thresh.getParameter<std::vector<double> >(name.str().c_str()));
       hf_Short_S9S1_Energy_Thresh_.push_back(junk);
       name.str("");
     }

   const edm::ParameterSet& HF_Short_S9S1_ET_Thresh = ps.getParameter<edm::ParameterSet>("hf_Short_S9S1_ET_Thresh");
   for (int i=29;i<=41;++i)
     {
       name<<"hf_Short_S9S1_ETThresh"<<i;
       std::vector<double> junk=(HF_Short_S9S1_ET_Thresh.getParameter<std::vector<double> >(name.str().c_str()));
       hf_Short_S9S1_ET_Thresh_.push_back(junk);
       name.str("");
     }
   
   const edm::ParameterSet& HF_Short_S9S1_Slope_Intercept = ps.getParameter<edm::ParameterSet>("hf_Short_S9S1_Slope_Intercept");
   for (int i=29;i<=41;++i)
     {
       name<<"hf_Short_S9S1_SlopeIntercept"<<i;
       std::vector<double> junk=(HF_Short_S9S1_Slope_Intercept.getParameter<std::vector<double> >(name.str().c_str()));
       hf_Short_S9S1_Slope_Intercept_.push_back(junk);
       name.str("");
     }

   for (unsigned int i=0;i<hf_Short_S9S1_Slope_Intercept_.size();++i)
     {
       if (hf_Short_S9S1_Slope_Intercept_[i].size()==1) // no intercept specified; only slope.  Set intercept = -1*(slope)*log(E_thresh)
	 {
	   hf_Short_S9S1_Slope_Intercept_[i].push_back(-1*hf_Short_S9S1_Slope_Intercept_[i][0]*log(GetThreshold(int(29+i),hf_Short_PET_Energy_Thresh_[i])));
	 }
     }

   // Repeat for long fibers
   hf_Long_Use_PET_    = ps.getParameter<std::vector<int> >("hf_Long_Use_PET");
   name.str("");
   const edm::ParameterSet& HF_Long_PET_Energy_Thresh = ps.getParameter<edm::ParameterSet>("hf_Long_PET_Energy_Thresh");
   for (int i=29;i<=41;++i)
     {
       name<<"hf_Long_PET_energyThresh"<<i;
       std::vector<double> junk=(HF_Long_PET_Energy_Thresh.getParameter<std::vector<double> >(name.str().c_str()));
       hf_Long_PET_Energy_Thresh_.push_back(junk);
       name.str("");
     }
   const edm::ParameterSet& HF_Long_PET_ET_Thresh = ps.getParameter<edm::ParameterSet>("hf_Long_PET_ET_Thresh");
   for (int i=29;i<=41;++i)
     {
       name<<"hf_Long_PET_ETThresh"<<i;
       std::vector<double> junk=(HF_Long_PET_ET_Thresh.getParameter<std::vector<double> >(name.str().c_str()));
       hf_Long_PET_ET_Thresh_.push_back(junk);
       name.str("");
     }
   const edm::ParameterSet&  HF_Long_Rcut = ps.getParameter<edm::ParameterSet>("hf_Long_Rcut");
   hf_Long_Rcut_parameterizeET_=HF_Long_Rcut.getUntrackedParameter<bool>("hf_Long_Rcut_parameterizeET",false);
   for (int i=29;i<=41;++i)
     {
       name<<"hf_Long_Rcut_values"<<i;
      std::vector<double> junk= (HF_Long_Rcut.getParameter<std::vector<double> >(name.str().c_str()));
       hf_Long_Rcut_values_.push_back(junk);
       name.str("");
     }

   const edm::ParameterSet& HF_Long_S9S1_Energy_Thresh = ps.getParameter<edm::ParameterSet>("hf_Long_S9S1_Energy_Thresh");
   for (int i=29;i<=41;++i)
     {
       name<<"hf_Long_S9S1_energyThresh"<<i;
       std::vector<double> junk=(HF_Long_S9S1_Energy_Thresh.getParameter<std::vector<double> >(name.str().c_str()));
       hf_Long_S9S1_Energy_Thresh_.push_back(junk);
       name.str("");
     }
   
   const edm::ParameterSet& HF_Long_S9S1_ET_Thresh = ps.getParameter<edm::ParameterSet>("hf_Long_S9S1_ET_Thresh");
   for (int i=29;i<=41;++i)
     {
       name<<"hf_Long_S9S1_ETThresh"<<i;
       std::vector<double> junk=(HF_Long_S9S1_ET_Thresh.getParameter<std::vector<double> >(name.str().c_str()));
       hf_Long_S9S1_ET_Thresh_.push_back(junk);
       name.str("");
     }
   
   const edm::ParameterSet& HF_Long_S9S1_Slope_Intercept = ps.getParameter<edm::ParameterSet>("hf_Long_S9S1_Slope_Intercept");
   for (int i=29;i<=41;++i)
     {
       name<<"hf_Long_S9S1_SlopeIntercept"<<i;
       std::vector<double> junk=(HF_Long_S9S1_Slope_Intercept.getParameter<std::vector<double> >(name.str().c_str()));
       hf_Long_S9S1_Slope_Intercept_.push_back(junk);
       name.str("");
     }

   for (unsigned int i=0;i<hf_Long_S9S1_Slope_Intercept_.size();++i)
     {
       if (hf_Long_S9S1_Slope_Intercept_[i].size()==1) // no intercept specified; only slope.  Set intercept = -1*(slope)*log(E_thresh)
	 {
	   hf_Long_S9S1_Slope_Intercept_[i].push_back(-1*hf_Long_S9S1_Slope_Intercept_[i][0]*log(GetThreshold(int(29+i),hf_Long_PET_Energy_Thresh_[i])));
	 }
     }
}  //HFRecHitReflaggerJETMET::HFRecHitReflaggerJETMET()



HFRecHitReflaggerJETMET::~HFRecHitReflaggerJETMET()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}




void HFRecHitReflaggerJETMET::beginRun(Run& r, const EventSetup& c)
{
  // BEGIN RUN NOT YET WORKING?   WHY NOT?
  // WE NEED THIS TO SKIP THE REFLAGGING OF CHANNELS WHOSE PARTNERS ARE DEAD!
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

   // prepare the output HF RecHit collection
   std::auto_ptr<HFRecHitCollection> pOut(new HFRecHitCollection());
   
   // loop over rechits, and set the new bit you wish to use
   for (HFRecHitCollection::const_iterator recHit=hfRecHits->begin(); recHit!=hfRecHits->end(); ++recHit) {
     HFRecHit newhit = (HFRecHit)(*recHit);
     // Set bit to be on for all hits
     if (hfBitAlwaysOn_)
       newhit.setFlagField(1,hfFlagBit_);

     // Set bit to be off for all hits
     else if (hfBitAlwaysOff_)
       newhit.setFlagField(0,hfFlagBit_);

     else
       {
	 HcalDetId id(newhit.detid().rawId());
	 int depth = newhit.id().depth();
	 int ieta  = newhit.id().ieta();
	 int flagvalue=-1;

	 // For each ieta ring, one may specify whether to use PET or S9S1.
	 // Note that S9S1 doesn't behave sensibly for short fibers,
	 // and PET doesn't perform optimally for long fibers (except at |ieta|=29).
	 if (depth==2)  // Short fiber tests
	   {
	     // Run either PET or S9S1 tests, depending on input configurable for the given ieta ring
	     if (hf_Short_Use_PET_[abs(ieta)-29]==1)
	       CheckPET(newhit)==true  ? flagvalue = 1 : flagvalue = 0;
	     else
	       CheckS9S1(newhit)==true ? flagvalue = 1 : flagvalue = 0;
	   }
	 else if (depth==1) // Long fiber tests
	   {
	     // Run either PET or S9S1 tests, depending on input configurable for the given ieta ring
	     if (hf_Long_Use_PET_[abs(ieta)-29]==1)
	       CheckPET(newhit)==true  ? flagvalue = 1 : flagvalue = 0;
	     else
	       CheckS9S1(newhit)==true ? flagvalue = 1 : flagvalue = 0;
	   }

	 // Set flag bit based on test; if no tests set, don't change flag
	 if (flagvalue!=-1)
	   newhit.setFlagField(flagvalue, hfFlagBit_);

	 if (debug_>0 && flagvalue==1)
	   cout <<"FOUND NOISY HF!  "<<id<<"  Energy= "<<newhit.energy()<<"  Run = "<< iEvent.run()<<"  Event = "<<iEvent.id().event()<<endl;

       } // hfBitAlwaysOn/Off bits not set; run other tests

     // Add rechit to collection
     pOut->push_back(newhit);
   }
   
   // put the re-flagged HF RecHit collection into the Event
   iEvent.put(pOut);
 
}

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

bool HFRecHitReflaggerJETMET::CheckPET(const HFRecHit& hf)
{
  /*
    Runs 'PET' test:  Computes ratio |L-S|/(L+S) for channel passing ieta-dependent energy and ET cuts.
    Channel is marked as noise if ratio exceeds ratio R(E)
  */

  HcalDetId id(hf.detid().rawId());
  int iphi = id.iphi();
  int ieta = id.ieta();
  double energy=hf.energy();
  int depth = id.depth();
  double fEta=fabs(0.5*(theHFEtaBounds[abs(ieta)-29]+theHFEtaBounds[abs(ieta)-28]));
  double ET = energy/cosh(fEta);
  double threshold=0;
  
  if (debug_>1) std::cout <<"<RechitReflaggerJETMET::CheckPET>  energy = "<<energy<<"  ieta = "<<ieta<<std::endl;

  if (depth==1)  //long fibers
    {
      // Check that energy, ET above threshold
      threshold=GetThreshold(ieta, hf_Long_PET_Energy_Thresh_[abs(ieta)-29]);
      if (energy<threshold)
	return false;
      threshold=GetThreshold(ieta, hf_Long_PET_ET_Thresh_[abs(ieta)-29] );
      if (ET<threshold)
	return false;
      
      // Get threshold cut to use at this energy, and compute the |L-S|/(L+S) ratio

      double PETcut=GetThreshold(energy,hf_Long_Rcut_values_[abs(ieta)-29]);
      double PETvalue=GetPETvalue(hf);
      if (debug_>1)
	std::cout <<"  <HFRecHitReflaggerJETMET::CheckPET>  ieta = "<<ieta<<"  energy threshold = "<<GetThreshold(ieta, hf_Long_PET_Energy_Thresh_[abs(ieta)-29])<<"  ET threshold = "<<GetThreshold(ieta,hf_Long_PET_ET_Thresh_[abs(ieta)-29] )<<"   ENERGY = "<<energy<<"   PET threshold = "<<PETcut<<std::endl;

      if (PETvalue>PETcut) 
	{
	  if (debug_>2) std::cout <<"***** FOUND CHANNEL FAILING PET CUT!  CHANNEL = "<<ieta<<",  "<<iphi<<",  "<<depth<<std::endl;
	  return true;  // channel looks noisy
	}
      else return false; // channel doesn't look noisy
    } // if (depth==1)

  else if (depth==2)  //short fibers
    {
      // Check that energy, ET above threshold
      threshold=GetThreshold(ieta, hf_Short_PET_Energy_Thresh_[abs(ieta)-29]);
      if (energy<threshold)
	return false;
      threshold=GetThreshold(ieta, hf_Short_PET_ET_Thresh_[abs(ieta)-29] );
      if (ET<threshold)
	return false;
      
      // Get threshold cut to use at this energy, and compute the |L-S|/(L+S) ratio

      double PETcut=GetThreshold(energy,hf_Short_Rcut_values_[abs(ieta)-29]);
      double PETvalue=GetPETvalue(hf);
      if (debug_>1)
	std::cout <<"  <HFRecHitReflaggerJETMET::CheckPET>  ieta = "<<ieta<<"  energy threshold = "<<GetThreshold(ieta, hf_Short_PET_Energy_Thresh_[abs(ieta)-29])<<"  ET threshold = "<<GetThreshold(ieta,hf_Short_PET_ET_Thresh_[abs(ieta)-29] )<<"   ENERGY = "<<energy<<"   PET threshold = "<<PETcut<<std::endl;

      if (PETvalue>PETcut) 
	{
	  if (debug_>2) std::cout <<"***** FOUND CHANNEL FAILING PET CUT!  CHANNEL = "<<ieta<<",  "<<iphi<<",  "<<depth<<std::endl;
	  return true;  // channel looks noisy
	}
      else return false; // channel doesn't look noisy


    } // else if (depth==2)

  return false; // should never reach this part
} // CheckPET(HFRecHit& hf)



bool HFRecHitReflaggerJETMET::CheckS9S1(const HFRecHit& hf)
{
  // Apply S9/S1 test  -- only sensible for long fibers at the moment!


  // Step 1:  Get the appropriate info from the rechit
  HcalDetId id(hf.detid().rawId());
  int iphi = id.iphi();
  int ieta = id.ieta();
  double energy=hf.energy();
  int depth = id.depth();
  double fEta=fabs(0.5*(theHFEtaBounds[abs(ieta)-29]+theHFEtaBounds[abs(ieta)-28]));
  double ET = energy/cosh(fEta);
  double threshold=0;

  
  // Step 2 :  Calculate S9/S1 for this rechit
  double S9S1value=GetS9S1value(hf); // get the ratio S9/S1

  if (depth==1) // long fiber
    {
      // Step 3L:  Check that rechit energy and ET are both above threshold
      threshold=GetThreshold(ieta, hf_Long_S9S1_Energy_Thresh_[abs(ieta)-29]);
      if (energy<threshold)
	return false;
      threshold=GetThreshold(ieta, hf_Long_S9S1_ET_Thresh_[abs(ieta)-29]);
      if (ET<threshold)
	return false;

      // Step 4L:  Calculate S9/S1 minimum allowed value, using slope & intercept information from cfg file.
      if (hf_Long_S9S1_Slope_Intercept_[abs(ieta)-29].size()<2)
	{ 
	  if (debug_>1)
	    cout <<"Invalid Size of Slope_Intercept vector for Long ieta = "<<ieta<<":   size = "<<hf_Long_S9S1_Slope_Intercept_[abs(ieta)-29].size()<<endl;
	  return false;  // false means channel won't be flagged
	}
      double S9S1slope= hf_Long_S9S1_Slope_Intercept_[abs(ieta)-29][0];
      double S9S1intercept=hf_Long_S9S1_Slope_Intercept_[abs(ieta)-29][1];

      // S9S1 cut has form [0]+[1]*log[E], 
      double S9S1cut = S9S1intercept+S9S1slope*(log(energy));

      if (debug_>1) 
	std::cout <<"  <HFRecHitReflaggerJETMET::CheckS9S1>  ieta = "<<ieta<<"  energy threshold = "<<GetThreshold(ieta, hf_Long_S9S1_Energy_Thresh_[abs(ieta)-29])<<"  ET threshold = "<<GetThreshold(ieta,hf_Long_S9S1_ET_Thresh_[abs(ieta)-29] )<<"   ENERGY = "<<energy<<"   S9S1cut = "<<S9S1intercept<<" + "<<S9S1slope<<"*log("<<energy<<") = "<<S9S1cut<<"   S9S1 value = "<<S9S1value<<endl;

      if (S9S1value<S9S1cut) 
	{
	  if (debug_>0) std::cout <<"***** FOUND CHANNEL FAILING S9S1 CUT!  CHANNEL = "<<ieta<<",  "<<iphi<<",  "<<depth<<std::endl;
	  return true;
	}
      else return false;
    }

  // ****************WARNING!!!!!****************  SHORT FIBERS DON'T HAVE TUNED THRESHOLDS!  USE PET CUT FOR SHORT FIBERS! ******* //
  else if (depth==2)  //short fiber
    {
      // Step 3S:  Check that rechit energy and ET are both above threshold
      threshold=GetThreshold(ieta, hf_Short_S9S1_Energy_Thresh_[abs(ieta)-29]);
      if (energy<threshold)
	return false;
      threshold=GetThreshold(ieta, hf_Short_S9S1_ET_Thresh_[abs(ieta)-29] );
      if (ET<threshold)
	return false;

      // Step 4S:  Calculate S9/S1 minimum allowed value, using slope & intercept information from cfg file.
      if (hf_Short_S9S1_Slope_Intercept_[abs(ieta)-29].size()<2)
	{ 
	  if (debug_>1)
	    cout <<"Invalid Size of Slope_Intercept vector for Short ieta = "<<ieta<<":   size = "<<hf_Short_S9S1_Slope_Intercept_[abs(ieta)-29].size()<<endl;
	  return false;
	}
      double S9S1slope= hf_Short_S9S1_Slope_Intercept_[abs(ieta)-29][0];
      double S9S1intercept=hf_Short_S9S1_Slope_Intercept_[abs(ieta)-29][1];

      // S9S1 cut has form [0]+[1]*log[E], 
      double S9S1cut = S9S1intercept+S9S1slope*(log(energy));
   
      if (debug_>1) 
	std::cout <<"  <HFRecHitReflaggerJETMET::CheckS9S1>  ieta = "<<ieta<<"  energy threshold = "<<GetThreshold(ieta, hf_Short_S9S1_Energy_Thresh_[abs(ieta)-29])<<"  ET threshold = "<<GetThreshold(ieta,hf_Short_S9S1_ET_Thresh_[abs(ieta)-29] )<<"   ENERGY = "<<energy<<"   S9S1cut = "<<S9S1intercept<<" + "<<S9S1slope<<"*log("<<energy<<") = "<<S9S1cut<<"   S9S1 value = "<<S9S1value<<endl;

      if (S9S1value<S9S1cut) 
	{
	  if (debug_>0) std::cout <<"***** FOUND CHANNEL FAILING S9S1 CUT!  CHANNEL = "<<ieta<<",  "<<iphi<<",  "<<depth<<std::endl;
	  return true;
	}
      else return false;
    }

  // Code should never get here
  return false;
}




double HFRecHitReflaggerJETMET::GetS9S1value(const HFRecHit& hf)
{
  // sum all energies around cell hf (including hf itself)
  // Then subtract hf energy, and divide by this energy to form ratio S9/S1

  HcalDetId id(hf.detid().rawId());
  int iphi = id.iphi();
  int ieta = id.ieta();
  double energy=hf.energy();
  int depth = id.depth();
  if (debug_>1)  std::cout <<"\t<HFRecHitReflaggerJETMET::GetS9S1value>  Channel = ("<<iphi<<",  "<<ieta<<",  "<<depth<<")  :  Energy = "<<energy<<std::endl; 

  double S9S1=0; // starting value

  int testphi=-99;
 
  // Part A:  Check fixed iphi, and vary ieta
  for (int d=1;d<=2;++d) // depth loop
    {
      for (int i=ieta-1;i<=ieta+1;++i) // ieta loop
	{
	  testphi=iphi;
	  // Special case when ieta=39, since ieta=40 only has phi values at 3,7,11,...
	  // phi=3 covers 3,4,5,6
	  if (abs(ieta)==39 && abs(i)>39 && testphi%4==1)
	    testphi-=2;
	  while (testphi<0) testphi+=72;
	  if (i==ieta && d==depth) continue;  // don't add the cell itself
	  // Look to see if neighbor is in rechit collection
	  HcalDetId neighbor(HcalForward, i,testphi,d);
	  HFRecHitCollection::const_iterator neigh=hfRecHits->find(neighbor);
	  if (neigh!=hfRecHits->end())
	    S9S1+=neigh->energy();
	}
    }

  // Part B: Fix ieta, and loop over iphi.  A bit more tricky, because of iphi wraparound and different segmentation at 40, 41
  
  int phiseg=2; // 10 degree segmentation for most of HF (1 iphi unit = 5 degrees)
  if (abs(ieta)>39) phiseg=4; // 20 degree segmentation for |ieta|>39
  for (int d=1;d<=2;++d)
    {
      for (int i=iphi-phiseg;i<=iphi+phiseg;i+=phiseg)
	{
	  if (i==iphi) continue;  // don't add the cell itself, or its depthwise partner (which is already counted above)
	  testphi=i;
	  // Our own modular function, since default produces results -1%72 = -1
	  while (testphi<0) testphi+=72;
	  while (testphi>72) testphi-=72;
	  // Look to see if neighbor is in rechit collection
	  HcalDetId neighbor(HcalForward, ieta,testphi,d);
	  HFRecHitCollection::const_iterator neigh=hfRecHits->find(neighbor);
	  if (neigh!=hfRecHits->end())
	    S9S1+=neigh->energy();
	}
    }
  
  if (abs(ieta)==40) // add extra cells for 39/40 boundary due to increased phi size at ieta=40.
    {
      for (int d=1;d<=2;++d) // add cells from both depths!
	{
	  HcalDetId neighbor(HcalForward, 39*abs(ieta)/ieta,(iphi+2)%72,d);  
	  HFRecHitCollection::const_iterator neigh=hfRecHits->find(neighbor);
	  if (neigh!=hfRecHits->end())
	      S9S1+=neigh->energy();
	}
    }
    
  S9S1/=energy;  // divide to form actual ratio
  return S9S1;
} // GetS9S1value



double HFRecHitReflaggerJETMET::GetPETvalue(const HFRecHit& hf)
{
  HcalDetId id(hf.detid().rawId());
  int iphi = id.iphi();
  int ieta = id.ieta();
  int depth = id.depth();
  double energy=hf.energy();
  if (debug_>1)  std::cout <<"\t<HFRecHitReflaggerJETMET::GetPETvalue>  Channel = ("<<iphi<<",  "<<ieta<<",  "<<depth<<")  :  Energy = "<<energy<<std::endl; 

  HcalDetId pId(HcalForward, ieta,iphi,3-depth); // get partner;
  // Check if partner is in known dead cell list; if so, don't flag
  if (badstatusmap.find(pId)!=badstatusmap.end())
    return 0;

  double epartner=0; // assume no partner
  // TO DO:  Add protection against cells marked dead in status database
  HFRecHitCollection::const_iterator part=hfRecHits->find(pId);
  if ( part!=hfRecHits->end() ) epartner=part->energy();
  return 1.*(energy-epartner)/(energy+epartner);
}



// Get thresholds, assuming thresholds are parameterized as power series in some variable
 
double HFRecHitReflaggerJETMET::GetThreshold(const int base, const std::vector<double>& params)
{
  double thresh=0;
  for (unsigned int i=0;i<params.size();++i)
    {
      thresh+=params[i]*pow(fabs(base),(int)i);
    }
  return thresh;
}

double HFRecHitReflaggerJETMET::GetThreshold(const double base, const std::vector<double>& params)
{
  double thresh=0;
  for (unsigned int i=0;i<params.size();++i)
    {
      thresh+=params[i]*pow(fabs(base),(int)i);
    }
  return thresh;
}



// Print out all parameters used in the reflagger

void HFRecHitReflaggerJETMET::dumpInfo()
{
  // Dump out all the parameters used in the reflagger
  if (debug_==0) return;
  else
    {
      cout <<"  Dumping out all parameters!"<<endl;
      cout <<" SHORT FIBERS"<<endl<<endl;
      for (int i=29;i<=41;++i)
	{
	  cout <<"|ieta| = "<<i<<"  Using PET ? "<<hf_Short_Use_PET_[i-29]<<endl;
	  if (hf_Short_Use_PET_[i-29])
	    {
	      cout <<"\tPET Energy thresh:  > ";
	      for (unsigned int j=0;j<hf_Short_PET_Energy_Thresh_[i-29].size();++j)
		{
		  cout <<hf_Short_PET_Energy_Thresh_[i-29][j]<<"*pow(|ieta|,"<<j<<")";
		  if (j==(hf_Short_PET_Energy_Thresh_[i-29].size()-1))
		    cout <<endl;
		  else
		    cout <<" + ";
		}
	      cout <<"\tPET ET thresh:  > ";
	      for (unsigned int j=0;j<hf_Short_PET_ET_Thresh_[i-29].size();++j)
		{
		  cout <<hf_Short_PET_ET_Thresh_[i-29][j]<<"*pow(|ieta|,"<<j<<")";
		  if (j==(hf_Short_PET_ET_Thresh_[i-29].size()-1))
		    cout <<endl;
		  else
		    cout <<" + ";
		}
	      cout <<"\tR cut > ";
	      string X="Energy";
	      if (hf_Short_Rcut_parameterizeET_)
		X="ET";
	      for (unsigned int j=0;j<hf_Short_Rcut_values_[i-29].size();++j)
		{
		  cout <<hf_Short_Rcut_values_[i-29][j]<<"*pow("<<X.c_str()<<","<<j<<")";
		  if (j==hf_Short_Rcut_values_[i-29].size()-1)
		    cout<<endl;
		  else
		    cout <<" + ";
		}
		     
	    } // if use PET
	  else
	    {
	      cout <<"\tS9S1 Energy Thresh: > ";
	      for (unsigned int j=0;j<hf_Short_S9S1_Energy_Thresh_[i-29].size();++j)
		{
		  cout <<hf_Short_S9S1_Energy_Thresh_[i-29][j]<<"*pow(|ieta|,"<<j<<")";
		  if (j==(hf_Short_S9S1_Energy_Thresh_[i-29].size()-1))
		    cout <<endl;
		  else
		    cout <<" + ";
		}
	      cout <<"\tS9S1 ET thresh:  > ";
	      for (unsigned int j=0;j<hf_Short_S9S1_ET_Thresh_[i-29].size();++j)
		{
		  cout <<hf_Short_S9S1_ET_Thresh_[i-29][j]<<"*pow(|ieta|,"<<j<<")";
		  if (j==(hf_Short_S9S1_ET_Thresh_[i-29].size()-1))
		    cout <<endl;
		  else
		    cout <<" + ";
		}
	      cout <<"\tS9S1 cut  (Cut form is SLOPE*log(E)+Intercept):  "<<endl;
	      if (hf_Short_S9S1_Slope_Intercept_[i-29].size()>0)
		cout <<"\t\tSlope = "<<hf_Short_S9S1_Slope_Intercept_[i-29][0]<<endl;
	      else
		cout <<"\t\tSlope UNAVAILABLE!"<<endl;
	      if (hf_Short_S9S1_Slope_Intercept_[i-29].size()>1)
		cout <<"\t\tIntercept = "<<hf_Short_S9S1_Slope_Intercept_[i-29][1]<<endl;
	      else
		cout <<"\t\tIntercept UNAVAILABLE!"<<endl;
	    } // using S9S1
	  cout <<endl<<endl;
	} // for (int i=29;i<=41;++i)
      
      cout <<" LONG FIBERS"<<endl<<endl;
      for (int i=29;i<=41;++i)
	{
	  cout <<"|ieta| = "<<i<<"  Using PET ? "<<hf_Long_Use_PET_[i-29]<<endl;
	  if (hf_Long_Use_PET_[i-29])
	    {
	      cout <<"\tPET Energy thresh:  > ";
	      for (unsigned int j=0;j<hf_Long_PET_Energy_Thresh_[i-29].size();++j)
		{
		  cout <<hf_Long_PET_Energy_Thresh_[i-29][j]<<"*pow(|ieta|,"<<j<<")";
		  if (j==(hf_Long_PET_Energy_Thresh_[i-29].size()-1))
		    cout <<endl;
		  else
		    cout <<" + ";
		}
	      cout <<"\tPET ET thresh:  > ";
	      for (unsigned int j=0;j<hf_Long_PET_ET_Thresh_[i-29].size();++j)
		{
		  cout <<hf_Long_PET_ET_Thresh_[i-29][j]<<"*pow(|ieta|,"<<j<<")";
		  if (j==(hf_Long_PET_ET_Thresh_[i-29].size()-1))
		    cout <<endl;
		  else
		    cout <<" + ";
		}
	      cout <<"\tR cut > ";
	      string X="Energy";
	      if (hf_Long_Rcut_parameterizeET_)
		X="ET";
	      for (unsigned int j=0;j<hf_Long_Rcut_values_[i-29].size();++j)
		{
		  cout <<hf_Long_Rcut_values_[i-29][j]<<"*pow("<<X.c_str()<<","<<j<<")";
		  if (j==hf_Long_Rcut_values_[i-29].size()-1)
		    cout<<endl;
		  else
		    cout <<" + ";
		}
		     
	    } // if use PET
	  else
	    {
	      cout <<"\tS9S1 Energy Thresh: > ";
	      for (unsigned int j=0;j<hf_Long_S9S1_Energy_Thresh_[i-29].size();++j)
		{
		  cout <<hf_Long_S9S1_Energy_Thresh_[i-29][j]<<"*pow(|ieta|,"<<j<<")";
		  if (j==(hf_Long_S9S1_Energy_Thresh_[i-29].size()-1))
		    cout <<endl;
		  else
		    cout <<" + ";
		}
	      cout <<"\tS9S1 ET thresh:  > ";
	      for (unsigned int j=0;j<hf_Long_S9S1_ET_Thresh_[i-29].size();++j)
		{
		  cout <<hf_Long_S9S1_ET_Thresh_[i-29][j]<<"*pow(|ieta|,"<<j<<")";
		  if (j==(hf_Long_S9S1_ET_Thresh_[i-29].size()-1))
		    cout <<endl;
		  else
		    cout <<" + ";
		}
	      cout <<"\tS9S1 cut  (Cut form is SLOPE*log(E)+Intercept):  "<<endl;
	      if (hf_Long_S9S1_Slope_Intercept_[i-29].size()>0)
		cout <<"\t\tSlope = "<<hf_Long_S9S1_Slope_Intercept_[i-29][0]<<endl;
	      else
		cout <<"\t\tSlope UNAVAILABLE!"<<endl;
	      if (hf_Long_S9S1_Slope_Intercept_[i-29].size()>1)
		cout <<"\t\tIntercept = "<<hf_Long_S9S1_Slope_Intercept_[i-29][1]<<endl;
	      else
		cout <<"\t\tIntercept UNAVAILABLE!"<<endl;
	    } // using S9S1
	  cout <<endl<<endl;
	} // for (int i=29;i<=41;++i)
    } // if (debug_>0)

}



//define this as a plug-in
DEFINE_FWK_MODULE(HFRecHitReflaggerJETMET);
