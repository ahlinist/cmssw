#ifndef CosmicMuonGenerator_h
#define CosmicMuonGenerator_h
//
// CosmicMuonGenerator by droll (04/DEC/2005)
// modified by P. Biallass 29.03.2006 to implement new cosmic generator (CMSCGEN.cc) 
//

// include files
#include "IOMC/CosmicMuonGenerator/interface/CMSCGENnorm.h"
#include "IOMC/CosmicMuonGenerator/interface/CMSCGEN.h"
#include "IOMC/CosmicMuonGenerator/interface/CosmicMuonParameters.h"
#include "IOMC/CosmicMuonGenerator/interface/SingleParticleEvent.h"
#include <iostream>
#include "TRandom2.h"


using namespace std;

// class definitions
class CosmicMuonGenerator{
public:
  // constructor
  CosmicMuonGenerator(){
    //initialize class which normalizes flux (added by P.Biallass 29.3.2006)
    Norm = new CMSCGENnorm();
    //initialize class which produces the cosmic muons  (modified by P.Biallass 29.3.2006)
    Cosmics = new CMSCGEN();
    // set default control parameters
    NumberOfEvents = 100;
    RanSeed = 135799468;
    MinE =     2.;
    MaxE =   10000.;
    MinTheta =  0.*Deg2Rad;
    MaxTheta = 88.*Deg2Rad;
    MinPhi =    0.*Deg2Rad;
    MaxPhi =  360.*Deg2Rad;
    MinT0  = -12.5;
    MaxT0  =  12.5;
    ElossScaleFactor = 1.0;
    RadiusOfTarget = 8000.;
    ZDistOfTarget = 15000.;
    TrackerOnly = false;
    MTCCHalf = false;
    EventRate = 0.;
    rateErr_stat = 0.;
    rateErr_syst = 0.;

    SumIntegrals = 0.;
    Ngen = 0.;
    Nsel = 0.;
    Ndiced = 0.;
    NotInitialized = true;
    Target3dRadius = 0.;
    SurfaceRadius = 0.;

    cout << endl;
    cout << "*********************************************************" << endl;
    cout << "*********************************************************" << endl;
    cout << "***                                                   ***" << endl;
    cout << "***  C O S M I C  M U O N  G E N E R A T O R  (vC++)  ***" << endl;
    cout << "***                                                   ***" << endl;
    cout << "*********************************************************" << endl;
    cout << "*********************************************************" << endl;
    cout << endl;
  }
  // destructor
  ~CosmicMuonGenerator(){
    delete Norm; 
    delete Cosmics;
  }
  // event with one particle
  SingleParticleEvent OneMuoEvt;
 
 

private:
  //initialize class which normalizes flux (added by P.Biallass 29.3.2006)
  CMSCGENnorm*  Norm ;
  //initialize class which produces the cosmic muons  (modified by P.Biallass 29.3.2006)
  CMSCGEN* Cosmics ; 
  // default control parameters
  unsigned int NumberOfEvents; // number of events to be generated
  int    RanSeed; // seed of random number generator
  double MinE;     // min. E     [GeV]
  double MaxE;     // max. E     [GeV]
  double MinTheta; // min. theta [rad]
  double MaxTheta; // max. theta [rad]
  double MinPhi;   // min. phi   [rad]
  double MaxPhi;   // max. phi   [rad]
  double MinT0;    // min. t0   [ns]
  double MaxT0;    // max. t0   [ns]
  double ElossScaleFactor; // scale factor for energy loss
  double RadiusOfTarget; // Radius of target-cylinder which cosmics HAVE to hit [mm], default is CMS-dimensions
  double ZDistOfTarget; // z-length of target-cylinder which cosmics HAVE to hit [mm], default is CMS-dimensions
  bool   TrackerOnly; //if set to "true" detector with tracker-only setup is used, so no material or B-field outside is considerd
  bool   MTCCHalf; //if set to "true" muons are sure to hit half of CMS important for MTCC, 
                   //still material and B-field of whole CMS is considered
  double EventRate; // number of muons per second [Hz]
  double rateErr_stat; // stat. error of number of muons per second [Hz]
  double rateErr_syst; // syst. error of number of muons per second [Hz] from error of known flux
  // other stuff needed
  double SumIntegrals; // sum of phase space integrals
  double Ngen; // number of generated events
  double Nsel; // number of selected events
  double Ndiced; // number of diced events
  double Target3dRadius; // radius of sphere around target (cylinder)
  double SurfaceRadius; // radius for area on surface that has to be considered (for event generation)
  // random number generator (periodicity > 10**14)
  TRandom2 RanGen; 
  // check user input
  bool NotInitialized;
  void checkIn();
  // check, if muon is pointing into target
  bool goodOrientation();
  // event display: initialize + display
  void initEvDis();
  void displayEv();

public:
  // set parameters
  void setNumberOfEvents(unsigned int N);
  void setRanSeed(int N);
  void setMinE(double E);
  void setMaxE(double E);
  void setMinTheta(double Theta);
  void setMaxTheta(double Theta);
  void setMinPhi(double Phi);
  void setMaxPhi(double Phi);
  void setMinT0(double T0);
  void setMaxT0(double T0);
  void setElossScaleFactor(double ElossScaleFact);
  void setRadiusOfTarget(double R);
  void setZDistOfTarget(double Z);
  void setTrackerOnly(bool Tracker);
  void setMTCCHalf(bool MTCC);
  // initialize the generator
  void initialize();
   // prints rate + statistics
  void terminate();
  // initialize, generate and terminate the Cosmic Muon Generator
  void runCMG();
  // returns event rate
  double getRate();
  // generate next event/muon
  void nextEvent();
};
#endif
