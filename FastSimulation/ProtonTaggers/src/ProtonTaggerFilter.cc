/**
 * file ProtonTaggerFilter.cc
 *
 * Selection of two most forward going protons, one in clockwise
 * one in anti-clockwise direction.
 * Access to Roman Pot acceptances
 *
 * Authors: Marek Tasevsky, Patrick Janot, Dmitry Zaborov
 *
 * v1(famos): Nov 23, 2003
 * v2(famos): July 25, 2005
 * v3(cmssw): Sep 2007
 *
 */

// Version: $Id:  $

#include "FastSimulation/ProtonTaggers/interface/ProtonTaggerFilter.h"

#include "FWCore/PluginManager/interface/PluginManager.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "DataFormats/Common/interface/Handle.h"

#include "HepMC/GenEvent.h"

#include "CLHEP/Random/RandGaussQ.h"

#include <iostream>


/** read (and verify) parameters */

ProtonTaggerFilter::ProtonTaggerFilter(edm::ParameterSet const & p)      
{    
  // ... get parameters

  //pathToAcceptanceTables = p.getParameter<std::string>("pathToAcceptanceTables");
  pathToAcceptanceTables = "FastSimulation/ProtonTaggers/data";
  
  beam1mode = p.getParameter<unsigned int>("beam1mode");
  beam2mode = p.getParameter<unsigned int>("beam2mode");

  beamCombiningMode = p.getParameter<unsigned int>("beamCombiningMode");
  
  if (beam1mode > 4) throw cms::Exception("FastSimulation/ProtonTaggers") 
                           << "Error: beam1mode cannot be " << beam1mode;
  if (beam2mode > 4) throw cms::Exception("FastSimulation/ProtonTaggers") 
                           << "Error: beam2mode cannot be " << beam2mode;

  if (beamCombiningMode > 3) throw cms::Exception("FastSimulation/ProtonTaggers") 
                             << "Error: beamCombiningMode cannot be " << beamCombiningMode;

  if (((beam1mode != 4) || (beam2mode != 4)) && (beamCombiningMode > 1)) {
    std::cerr << "Warning: beamCombiningMode = " << beamCombiningMode 
              << " only makes sence with beam1mode = beam2mode = 4" << std::endl;
    //beamCombiningMode = 1;
  }
}


/** just empty */

ProtonTaggerFilter::~ProtonTaggerFilter() {;}


/** initialize detector acceptance table */

void ProtonTaggerFilter::beginJob(const edm::EventSetup & es)
{
  rpAcceptance.InitRPA(pathToAcceptanceTables);
}


/** nothing to be done here */

void ProtonTaggerFilter::endJob() {;}


/** Compute the detector acceptances and decide whether to filter the event */

bool ProtonTaggerFilter::filter(edm::Event & iEvent, const edm::EventSetup & es)
{
  // ... get generated event

  edm::Handle<edm::HepMCProduct> evtSource;
  iEvent.getByLabel("source",evtSource);
  const HepMC::GenEvent* genEvent = evtSource->GetEvent();

  // ... initialize some variables

  const double mp = 0.938272029; // just a proton mass
  const double Ebeam=7000.0;     // beam energy - would be better to read from parameters
 
  double Pzpgen1 = 0.0;
  double Pzpgen2 = 0.0;
  double Ptgen1  = -10.0;
  double Ptgen2  = -10.0;
  double PhiGen1 = 0;
  double PhiGen2 = 0;

  // ... loop over particles, find the most energetic proton in either direction
  
  for ( HepMC::GenEvent::particle_const_iterator piter = genEvent->particles_begin();
	    piter != genEvent->particles_end();
	    ++piter )
  {
    HepMC::GenParticle* p = *piter;

    if (p->pdg_id() == 2212) { // proton id
      
      //std::cout << " pdg_id: "  << p->pdg_id() << " eta: " << p->momentum().eta() << " e: " 
      //          <<  p->momentum().e() << " pz: " << p->momentum().pz() << std::endl;

      float Pzpgen = p->momentum().pz();

      if (Pzpgen>Pzpgen1 && Pzpgen<Ebeam) {
          Pzpgen1=Pzpgen;
	  Ptgen1 = p->momentum().perp();
          PhiGen1= p->momentum().phi();
      }

      if (Pzpgen<Pzpgen2 && Pzpgen>-Ebeam) {
          Pzpgen2=Pzpgen;
	  Ptgen2 = p->momentum().perp();
          PhiGen2= p->momentum().phi();
      }
      
      //std::cout << "Pzpgen1 " << Pzpgen1 << " Pzpgen2: "  << Pzpgen2 << " Pzpgen: " << Pzpgen << std::endl;
    }
  }

  // ... return false if no forward protons found

  if ((Ptgen1<0) && (Ptgen2<0)) return false;
  
  /* Detectors     beam1 (clwise)    beam2 (a-clwise)
   *
   * 420m              myAcc11           myAcc21
   * 220m              myAcc12           myAcc22
   * 420m.and.220m     myAcc13           myAcc23
   * 420m.or.220m      myAcc14           myAcc24
   */

  float Acc11,Acc12,Acc13,Acc14,Acc21,Acc22,Acc23,Acc24;
  Acc11=Acc12=Acc13=Acc14=Acc21=Acc22=Acc23=Acc24=0;

  // ... compute acceptance for any sensible proton 1

  if (Ptgen1>0) {

    // ... compute fractional momentum losses of the two protons

    double XiGen1 = 1.0 - Pzpgen1/Ebeam;

    if (XiGen1<0.0) XiGen1=-10.0;
    if (XiGen1>1.0) XiGen1=10.0;

    // ... compute "t"

    double tGen1=(-Ptgen1*Ptgen1-mp*mp*XiGen1*XiGen1)/(1-XiGen1);

    // ... get acceptance from tables

    rpAcceptance.getRPAcceptance1(tGen1, XiGen1, PhiGen1, Acc11, Acc12, Acc13, Acc14); 

    //std::cout << "Pzpgen1: " << Pzpgen1 << " Ptgen1: " <<  Ptgen1 << " XiGen1: " << XiGen1
    //          << " tGen1: " << tGen1 << " PhiGen1: " << PhiGen1 << std::endl;
  }

  // ... the same for proton 2

  if (Ptgen2>0) {
  
    double XiGen2 = 1.0 + Pzpgen2/Ebeam; 

    if (XiGen2<0.0) XiGen2=-10.0;
    if (XiGen2>1.0) XiGen2=10.0;

    double tGen2=(-Ptgen2*Ptgen2-mp*mp*XiGen2*XiGen2)/(1-XiGen2);

    rpAcceptance.getRPAcceptance2(tGen2, XiGen2, PhiGen2, Acc21, Acc22, Acc23, Acc24);

    //std::cout << "Pzpgen2: " << Pzpgen2 << " Ptgen2: " <<  Ptgen2 << " XiGen2: " << XiGen2
    //          << " tGen2: " << tGen2 << " PhiGen2: " << PhiGen2 << std::endl;
  }
  
  //std::cout << "Acc11: " << Acc11 << " Acc21: " << Acc21 << std::endl;
  //std::cout << "Acc12: " << Acc12 << " Acc22: " << Acc22 << std::endl;
  //std::cout << "Acc13: " << Acc13 << " Acc23: " << Acc23 << std::endl;
  //std::cout << "Acc14: " << Acc14 << " Acc24: " << Acc24 << std::endl;

  // ... make a decision
  
  //float rnd1 = RandGauss::shoot(0.,1.);
  //float rnd2 = RandGauss::shoot(0.,1.);

  const float acceptThreshold = 0.9;
  
  bool p1at220m = false;
  bool p1at420m = false;

  bool p2at220m = false;
  bool p2at420m = false;

  if (Acc13 > acceptThreshold) p1at220m = p1at420m = true;
  if (Acc11 > acceptThreshold) p1at420m = true;
  if (Acc12 > acceptThreshold) p1at220m = true;
  
  if (Acc23 > acceptThreshold) p2at220m = p2at420m = true;
  if (Acc21 > acceptThreshold) p2at420m = true;
  if (Acc22 > acceptThreshold) p2at220m = true;

  bool p1accepted = false;
  bool p2accepted = false;
  
  if ((beam1mode==1) &&  p1at420m ) p1accepted = true;
  if ((beam1mode==2) &&  p1at220m ) p1accepted = true;
  if ((beam1mode==3) &&  p1at220m && p1at420m) p1accepted = true;
  if ((beam1mode==4) && (p1at220m || p1at420m)) p1accepted = true;
  
  if ((beam2mode==1) &&  p2at420m ) p2accepted = true;
  if ((beam2mode==2) &&  p2at220m ) p2accepted = true;
  if ((beam2mode==3) &&  p2at220m && p2at420m ) p2accepted = true;
  if ((beam2mode==4) && (p2at220m || p2at420m)) p2accepted = true;

  switch (beamCombiningMode) {

    case 0:  // ... either of two protons
     if (p1accepted || p2accepted) return true; else return false;

    case 1:  // ... both protons
     if (p1accepted && p2accepted) return true; else return false;

    case 2:  // ... 220+220 or 420+420
     if ((p1at220m && p2at220m) || (p1at420m && p2at420m)) return true; else return false;

    case 3:  // ... 220+420 or 420+220
     if ((p1at220m && p2at420m) || (p1at420m && p2at220m)) return true; else return false;

  }
  
  return false;
}

// ... define CMSSW module

DEFINE_FWK_MODULE(ProtonTaggerFilter);
