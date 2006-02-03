#ifndef CosmicMuonGenerator_h
#define CosmicMuonGenerator_h
//
// CosmicMuonGenerator by droll (04/DEC/2005)
//

// include files
#include "IOMC/CosmicMuonGenerator/src/CosmicMuonFunction.cc"
#include "IOMC/CosmicMuonGenerator/interface/CosmicMuonParameters.h"
#include "IOMC/CosmicMuonGenerator/interface/PhaseSpacePart.h"
#include "IOMC/CosmicMuonGenerator/interface/SingleParticleEvent.h"
#include <iostream>
#include "TRandom2.h"

using namespace std;

// class definitions
class CosmicMuonGenerator{
public:
  // constructor
  CosmicMuonGenerator(){
    // set default control parameters
    NumberOfEvents = 100;
    RanSeed = 123456;
    MinE =     10.;
    MaxE =   5000.;
    MinTheta =  0.*Deg2Rad;
    MaxTheta = 80.*Deg2Rad;
    MinPhi =    0.*Deg2Rad;
    MaxPhi =  360.*Deg2Rad;
    MinT0  = -12.5;
    MaxT0  =  12.5;
    ElossScaleFactor = 1.0;
    EventRate = 0.;

    SumIntegrals = 0.;
    Ngen = 0.;
    Nsel = 0.;
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
  ~CosmicMuonGenerator(){}
  // event with one particle
  SingleParticleEvent OneMuoEvt;

private:
  // default control parameters
  unsigned int NumberOfEvents; // number of events to be generated
  unsigned int RanSeed; // seed of random number generator
  double MinE;     // min. E     [GeV]
  double MaxE;     // max. E     [GeV]
  double MinTheta; // min. theta [rad]
  double MaxTheta; // max. theta [rad]
  double MinPhi;   // min. phi   [rad]
  double MaxPhi;   // max. phi   [rad]
  double MinT0;    // min. t0   [ns]
  double MaxT0;    // max. t0   [ns]
  double ElossScaleFactor; // scale factor for energy loss
  double EventRate; // number of muons per second [Hz]
  // other stuff needed
  double SumIntegrals; // sum of phase space integrals
  TRandom2 RanGen; // random number generator (periodicity > 10**14)
  PhaseSpacePart V[30]; // phase pace parts
  double Ngen; // number of generated events
  double Nsel; // number of selected events
  double Target3dRadius; // radius of sphere around target (cylinder)
  double SurfaceRadius; // radius for area on surface that has to be considered (for event generation)
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
  void setRanSeed(unsigned int N);
  void setMinE(double E);
  void setMaxE(double E);
  void setMinTheta(double Theta);
  void setMaxTheta(double Theta);
  void setMinPhi(double Phi);
  void setMaxPhi(double Phi);
  void setMinT0(double T0);
  void setMaxT0(double T0);
  void setElossScaleFactor(double ElossScaleFact);
  // initialize the generator
  void initialize();
  // generate next event/muon
  void nextEvent();
  // prints rate + statistics
  void terminate();
  // initialize, generate and terminate the Cosmic Muon Generator
  void runCMG();
  // returns event rate
  double getRate();
};
#endif
