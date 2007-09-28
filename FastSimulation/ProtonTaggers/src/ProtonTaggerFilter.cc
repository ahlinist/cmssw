/**
 * file ProtonTaggerFilter.cc
 *
 * Selection of two most forward going protons, 
 * one in clockwise and one in anti-clockwise beam direction.
 * Access to near-beam detector acceptances.
 *
 * Author: Dmitry Zaborov
 */

// Version: $Id: ProtonTaggerFilter.cc,v 1.2 2007/09/28 08:50:13 dzaborov Exp $

#include "FastSimulation/ProtonTaggers/interface/ProtonTaggerFilter.h"

#include "FWCore/PluginManager/interface/PluginManager.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "DataFormats/Common/interface/Handle.h"

#include "HepMC/GenEvent.h"

//#include "CLHEP/Random/RandGaussQ.h"

#include "TFile.h"

#include <iostream>


/** read (and verify) parameters */

ProtonTaggerFilter::ProtonTaggerFilter(edm::ParameterSet const & p)      
{
  std::cout << "ProtonTaggerFilter: Initializing ..." << std::endl;
  
  // ... get parameters

  beam1mode = p.getParameter<unsigned int>("beam1mode");
  beam2mode = p.getParameter<unsigned int>("beam2mode");

  beamCombiningMode = p.getParameter<unsigned int>("beamCombiningMode");
  
  switch (beam1mode)
  {
    case 1: std::cout << "Option chosen for beam 1: 420" << std::endl; break;
    case 2: std::cout << "Option chosen for beam 1: 220" << std::endl; break;
    case 3: std::cout << "Option chosen for beam 1: 420 and 220" << std::endl; break;
    case 4: std::cout << "Option chosen for beam 1: 420 or 220" << std::endl; break;
    default: throw cms::Exception("FastSimulation/ProtonTaggers") 
                           << "Error: beam1mode cannot be " << beam1mode;
  }
  
  switch (beam2mode)
  {
    case 1: std::cout << "Option chosen for beam 2: 420" << std::endl; break;
    case 2: std::cout << "Option chosen for beam 2: 220" << std::endl; break;
    case 3: std::cout << "Option chosen for beam 2: 420 and 220" << std::endl; break;
    case 4: std::cout << "Option chosen for beam 2: 420 or 220" << std::endl; break;
    default: throw cms::Exception("FastSimulation/ProtonTaggers")
                           << "Error: beam2mode cannot be " << beam2mode;
  }

  switch (beamCombiningMode)
  {
    case 1: std::cout << "Option chosen: one proton is sufficient" << std::endl; break;
    case 2: std::cout << "Option chosen: two protons should be tagged" << std::endl; break;
    case 3: std::cout << "Option chosen: two protons should be tagged as 220+220 or 420+420" << std::endl; break;
    case 4: std::cout << "Option chosen: two protons should be tagged as 220+420 or 420+220" << std::endl; break;
    default: throw cms::Exception("FastSimulation/ProtonTaggers") 
                             << "Error: beamCombiningMode cannot be " << beamCombiningMode;
  }

  if (((beam1mode != 4) || (beam2mode != 4)) && (beamCombiningMode > 2)) {
    std::cerr << "Warning: beamCombiningMode = " << beamCombiningMode 
              << " only makes sence with beam1mode = beam2mode = 4" << std::endl;
  }

  std::cout << "ProtonTaggerFilter: Initialized" << std::endl;
}


/** just empty */

ProtonTaggerFilter::~ProtonTaggerFilter() {;}


/** initialize detector acceptance table */

void ProtonTaggerFilter::beginJob(const edm::EventSetup & es)
{
  std::cout << "ProtonTaggerFilter: Getting ready ..." << std::endl;

  edm::FileInPath myDataFile("FastSimulation/ProtonTaggers/data/acceptance_420_220.root");
  std::string fullPath = myDataFile.fullPath();

  std::cout << "Opening " << fullPath << std::endl;
  TFile f(fullPath.c_str());

  if (f.Get("description") != NULL)
      std::cout << "Description found: " << f.Get("description")->GetTitle() << std::endl;
  
  std::cout << "Reading acceptance tables " << std::endl;
  
  helper420beam1.Init(f, "a420");
  helper420beam2.Init(f, "a420_b2");

  helper220beam1.Init(f, "a220");
  helper220beam2.Init(f, "a220_b2");

  helper420a220beam1.Init(f, "a420a220");
  helper420a220beam2.Init(f, "a420a220_b2");

  f.Close();

  std::cout << "ProtonTaggerFilter: Ready" << std::endl;
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
 
  double pz1 = 0.0;
  double pz2 = 0.0;
  double pt1  = -10.0;
  double pt2  = -10.0;
  double phi1 = 0;
  double phi2 = 0;

  // ... loop over particles, find the most energetic proton in either direction
  
  for ( HepMC::GenEvent::particle_const_iterator piter = genEvent->particles_begin();
	    piter != genEvent->particles_end();
	    ++piter )
  {
    HepMC::GenParticle* p = *piter;

    if (p->pdg_id() == 2212) { // proton id
      
      //std::cout << " pdg_id: "  << p->pdg_id() << " eta: " << p->momentum().eta() << " e: " 
      //          <<  p->momentum().e() << " pz: " << p->momentum().pz() << std::endl;

      float pz = p->momentum().pz();

      if (pz>pz1 && pz<Ebeam) {
          pz1 = pz;
	  pt1 = p->momentum().perp();
          phi1= p->momentum().phi();
      }

      if (pz<pz2 && pz>-Ebeam) {
          pz2 = pz;
	  pt2 = p->momentum().perp();
          phi2= p->momentum().phi();
      }
      
      //std::cout << "pz1 " << pz1 << " pz2: "  << pz2 << " pz: " << pz << std::endl;
    }
  }

  // ... return false if no forward protons found

  if ((pt1<0) && (pt2<0)) return false;
  
  // ... first set all acceptances to zero

  float acc420b1, acc220b1, acc420and220b1, acc420or220b1; // beam 1 (clockwise)
  float acc420b2, acc220b2, acc420and220b2, acc420or220b2; // beam 2 (anti-clockwise)

  acc420b1 = acc220b1 = acc420and220b1 = acc420or220b1 = 0;
  acc420b2 = acc220b2 = acc420and220b2 = acc420or220b2 = 0;

  // ... then compute acceptance for any sensible proton 1

  if (pt1>0) {

    // ... compute kinimatical variable

    double xi  = 1.0 - pz1/Ebeam;		    // fractional momentum loss
    double t   = (-pt1*pt1 - mp*mp*xi*xi) / (1-xi); // "t"
    double phi = phi1;
    
    if (xi<0.0) xi=-10.0;
    if (xi>1.0) xi=10.0;
    
    // ... get acceptance from tables

    //std::cout << "pz1: " << pz1 << " pt1: " <<  pt1 << " xi: " << xi
    //          << " t: " << t << " phi: " << phi << std::endl;

    acc420b1       = helper420beam1.GetAcceptance(t, xi, phi);
    acc220b1       = helper220beam1.GetAcceptance(t, xi, phi);
    acc420and220b1 = helper420a220beam1.GetAcceptance(t, xi, phi);

    acc420or220b1  = acc420b1 + acc220b1 - acc420and220b1;

    //std::cout << "+acc420b1: " << acc420b1 << " acc220b1: " << acc220b1 << " acc420and220b1: " << acc420and220b1 << " acc420or220b1: " << acc420or220b1  << std::endl;
  }

  // ... the same for proton 2

  if (pt2>0) {
  
    double xi  = 1.0 + pz2/Ebeam;		  // fractional momentum loss
    double t   = (-pt2*pt2-mp*mp*xi*xi) / (1-xi); // "t"
    double phi = phi2;
    
    if (xi<0.0) xi=-10.0;
    if (xi>1.0) xi=10.0;

    //std::cout << "-pz2: " << pz2 << " pt2: " <<  pt2 << " xi: " << xi
    //          << " t: " << t << " phi: " << phi << std::endl;

    acc420b2       = helper420beam2.GetAcceptance(t, xi, phi);
    acc220b2       = helper220beam2.GetAcceptance(t, xi, phi);
    acc420and220b2 = helper420a220beam2.GetAcceptance(t, xi, phi);
    
    acc420or220b2  = acc420b2 + acc220b2 - acc420and220b2;

    //std::cout << "+acc420b2: " << acc420b2 << " acc220b2: " << acc220b2 << " acc420and220b2: " << acc420and220b2 << " acc420or220b2: " << acc420or220b2 << std::endl;
  }
  
  // ... make a decision
  
  //float rnd1 = RandGauss::shoot(0.,1.);
  //float rnd2 = RandGauss::shoot(0.,1.);

  const float acceptThreshold = 0.5;
  
  bool p1at220m = false;
  bool p1at420m = false;

  bool p2at220m = false;
  bool p2at420m = false;

  if (acc420and220b1 > acceptThreshold) p1at220m = p1at420m = true;
  if (acc420b1       > acceptThreshold) p1at420m = true;
  if (acc220b1       > acceptThreshold) p1at220m = true;
  
  if (acc420and220b2 > acceptThreshold) p2at220m = p2at420m = true;
  if (acc420b2       > acceptThreshold) p2at420m = true;
  if (acc220b2       > acceptThreshold) p2at220m = true;

  //if (p1at220m) std::cout << "got proton 1 at 220 m" << std::endl;
  //if (p1at420m) std::cout << "got proton 1 at 420 m" << std::endl;
  //if (p2at220m) std::cout << "got proton 2 at 220 m" << std::endl;
  //if (p2at420m) std::cout << "got proton 2 at 420 m" << std::endl;

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

  //if (p1accepted) std::cout << "proton 1 accepted" << std::endl;
  //if (p2accepted) std::cout << "proton 2 accepted" << std::endl;

  switch (beamCombiningMode) {

    case 1:  // ... either of two protons
     if (p1accepted || p2accepted) return true; else return false;

    case 2:  // ... both protons
     if (p1accepted && p2accepted) return true; else return false;

    case 3:  // ... 220+220 or 420+420
     if ((p1at220m && p2at220m) || (p1at420m && p2at420m)) return true; else return false;

    case 4:  // ... 220+420 or 420+220
     if ((p1at220m && p2at420m) || (p1at420m && p2at220m)) return true; else return false;

  }
  
  return false;
}

// ... define CMSSW module

DEFINE_FWK_MODULE(ProtonTaggerFilter);
