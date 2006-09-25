
#include "Configuration/CSA06Skimming/interface/MCParticlePairFilter.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include <iostream>

using namespace edm;
using namespace std;


MCParticlePairFilter::MCParticlePairFilter(const edm::ParameterSet& iConfig) :
label_(iConfig.getUntrackedParameter("moduleLabel",std::string("source"))),
minInvMass(iConfig.getUntrackedParameter("MinInvMass", 0.)),
maxInvMass(iConfig.getUntrackedParameter("MaxInvMass", 14000.)),
minDeltaPhi(iConfig.getUntrackedParameter("MinDeltaPhi", 0.)),
maxDeltaPhi(iConfig.getUntrackedParameter("MaxDeltaPhi", 6.3))
{
   //here do whatever other initialization is needed
   vector<int> defpid1;
   defpid1.push_back(0) ;
   particleID1 = iConfig.getUntrackedParameter< vector<int> >("ParticleID1",defpid1);
   vector<int> defpid2;
   defpid2.push_back(0) ;
   particleID2 = iConfig.getUntrackedParameter< vector<int> >("ParticleID2",defpid2);
   vector<int> defcharge;
   defcharge.push_back(0);
   particleCharge = iConfig.getUntrackedParameter< vector<int> >("ParticleCharge",defcharge);
   vector<double> defptmin;   
   defptmin.push_back(0.);
   ptMin = iConfig.getUntrackedParameter< vector<double> >("MinPt", defptmin);
   vector<double> defetamin ;
   defetamin.push_back(-10.);
   etaMin = iConfig.getUntrackedParameter< vector<double> >("MinEta", defetamin);
   vector<double> defetamax ;
   defetamax.push_back(10.);
   etaMax = iConfig.getUntrackedParameter< vector<double> >("MaxEta", defetamax);
   vector<int> defstat ;
   defstat.push_back(0);
   status = iConfig.getUntrackedParameter< vector<int> >("Status", defstat);

   
    // check for correct size
    if (ptMin.size() != 2 
	|| particleCharge.size() != 2 
	|| etaMin.size() != 2 
	|| etaMax.size() != 2 
	|| status.size() != 2 ) {
      cout << "WARNING: MCParticlePairFilter : size of some vectors not matching with 2!!" << endl;
    }
    
    // if particleCharge size is smaller than 2, fill up with defaults
    if (2 > particleCharge.size()) {
      vector<int> defcharge2;
      for (unsigned int i = 0; i < 2; i++){ defcharge2.push_back(0);}
      particleCharge = defcharge2;   
    } 
    // if ptMin size smaller than 2, fill up further with defaults
    if (2 > ptMin.size() ){
       vector<double> defptmin2 ;
       for (unsigned int i = 0; i < 2; i++){ defptmin2.push_back(0.);}
       ptMin = defptmin2;   
    } 
    // if etaMin size smaller than 2, fill up further with defaults
    if (2 > etaMin.size() ){
       vector<double> defetamin2 ;
       for (unsigned int i = 0; i < 2; i++){ defetamin2.push_back(-10.);}
       etaMin = defetamin2;   
    } 
    // if etaMax size smaller than 2, fill up further with defaults
    if (2 > etaMax.size() ){
       vector<double> defetamax2 ;
       for (unsigned int i = 0; i < 2; i++){ defetamax2.push_back(10.);}
       etaMin = defetamax2;   
    }     
    // if status size smaller than 2, fill up further with defaults
    if (2 > status.size() ){
       vector<int> defstat2 ;
       for (unsigned int i = 0; i < 2; i++){ defstat2.push_back(0);}
       status = defstat2;   
    } 

}


MCParticlePairFilter::~MCParticlePairFilter()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


// ------------ method called to skim the data  ------------
bool MCParticlePairFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   bool accepted = false;
   Handle<HepMCProduct> evt;
   iEvent.getByLabel(label_, evt);
   const double pi = 3.14159;

   vector<HepMC::GenParticle*> typeApassed;
   vector<HepMC::GenParticle*> typeBpassed;


    HepMC::GenEvent * myGenEvent = new  HepMC::GenEvent(*(evt->GetEvent()));
     
   
    for ( HepMC::GenEvent::particle_iterator p = myGenEvent->particles_begin();
	  p != myGenEvent->particles_end(); ++p ) {

      // check for type A conditions
      bool gottypeAID = false;
      for(unsigned int i=0; i<particleID1.size(); ++i) {
	if(abs((*p)->pdg_id()) == particleID1[i] || particleID1[i] == 0) {
	  gottypeAID = true;
	  break;
	}
      }
      if(gottypeAID) {
	if(particleCharge[0]==0 || ((*p)->pdg_id()*particleCharge[0]) >= 0) {
	  // passed charge criterium
	  if ( (*p)->momentum().perp() > ptMin[0] && (*p)->momentum().eta() > etaMin[0] 
	       && (*p)->momentum().eta() < etaMax[0] && ((*p)->status() == status[0] || status[0] == 0)) { 
	    // passed A type conditions ...
	    // ... now check pair-conditions with B type passed particles
	    unsigned int i=0;
	    double deltaphi;
	    double phi1 = (*p)->momentum().phi();
	    double phi2;
	    HepLorentzVector totmomentum = (*p)->momentum();
	    double invmass;
	    while(!accepted && i<typeBpassed.size()) {
	      totmomentum += typeBpassed[i]->momentum();
	      invmass = totmomentum.m();
	      if(invmass > minInvMass && invmass < maxInvMass) {
		phi2 = typeBpassed[i]->momentum().phi();
		deltaphi = fabs(phi1-phi2);
		if(deltaphi > pi) deltaphi = 2.*pi-deltaphi;
		if(deltaphi > minDeltaPhi && deltaphi < maxDeltaPhi) {
		  accepted = true;
		}
	      }
	      i++;
	    }
	  }
	}
      }
      
      // if we found a matching pair quit the loop
      if(accepted) break;
      else typeApassed.push_back((*p));   // else remember the particle to have passed type A conditions
      
      // check for type B conditions


 // check for type A conditions
      bool gottypeBID = false;
      for(unsigned int i=0; i<particleID2.size(); ++i) {
	if(abs((*p)->pdg_id()) == particleID2[i] || particleID2[i] == 0) {
	  gottypeBID = true;
	  break;
	}
      }
      if(gottypeBID) {
	if(particleCharge[1]==0 || ((*p)->pdg_id()*particleCharge[1]) >= 0) {
	  // passed charge criterium
	  if ( (*p)->momentum().perp() > ptMin[1] && (*p)->momentum().eta() > etaMin[1] 
	       && (*p)->momentum().eta() < etaMax[1] && ((*p)->status() == status[1] || status[1] == 0)) { 
	    // passed B type conditions ...
	    // ... now check pair-conditions with A type passed particles vector
	    unsigned int i=0;
	    double deltaphi;
	    double phi1 = (*p)->momentum().phi();
	    double phi2;
	    HepLorentzVector totmomentum = (*p)->momentum();
	    double invmass;
	    while(!accepted && i<typeApassed.size()) {
	      if((*p) != typeApassed[i]) {
		totmomentum += typeApassed[i]->momentum();
		invmass = totmomentum.m();
		if(invmass > minInvMass && invmass < maxInvMass) {
		  phi2 = typeApassed[i]->momentum().phi();
		  deltaphi = fabs(phi1-phi2);
		  if(deltaphi > pi) deltaphi = 2.*pi-deltaphi;
		  if(deltaphi > minDeltaPhi && deltaphi < maxDeltaPhi) {
		    accepted = true;
		  }
		}
	      }
	      i++;
	    }
	  }
	}
      }
      
      // if we found a matching pair quit the loop
      if(accepted) break;
      else typeBpassed.push_back((*p));   // else remember the particle to have passed type B conditions
      
    }
    
    delete myGenEvent; 

    if (accepted){ return true; } else {return false;}
    
}

