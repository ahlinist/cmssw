#ifndef PhaseSpacePart_h
#define PhaseSpacePart_h
//
// PhaseSpacePart by droll (10/12/2005)
//

// include files
#include "IOMC/CosmicMuonGenerator/src/CosmicMuonFunction.cc"
#include <iostream>

using namespace std;

class PhaseSpacePart{
public:
  // constructor
  PhaseSpacePart(){
    El = 666.; Eu = 666.; 
    Tl = 666.; Tu = 666.;
    Maximum = 0.;
    Integral = 0.;
    Pl = 0.; Pu = 0.;
  }
  // destructor
  ~PhaseSpacePart(){}
private:
  double El; double Eu; // lower/upper energy bound
  double Tl; double Tu; // lower/upper theta bound
  double Maximum;       // maximum of differential function in volume
  double Integral;      // integral of differential function in volume
  double Pl; double Pu; // lower/upper probability bound
public:
  // set (define) lower and upper (energy and theta) bounds of volume
  void setBorders(double lE, double uE, double lT, double uT);
  // set (calculate) integral int_{Eu,El} int_{Tu,Tl} N(E,T) dE dT and maximum of N(E,T)
  void setIntAndMax(unsigned int Nstep);
  // set probability range of PhaseSpacePart
  void setProbabilities(double lP, double uP);
  // return lower/upper energy bound
  double loEnergy();
  double upEnergy();
  // return lower/upper theta bound
  double loTheta();
  double upTheta();
  // return maximum of PhaseSpacePart
  double maximum();
  // return integral of PhaseSpacePart
  double integral();
  // return lower/upper probability bound
  double loProb();
  double upProb();
};
#endif
