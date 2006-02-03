#ifndef CosmicMuonFunction_cc
#define CosmicMuonFunction_cc

// function dN-dE-dTheta by droll (10/12/2005)
#include "TMath.h"
inline double dNdEdT(double E, double Theta){
  // differential rate for cosmic muons (particle data book)
  return 0.14*pow(E,-2.7)
       * (  1.000/(1. + 1.1*E*cos(Theta)/115.)
          + 0.054/(1. + 1.1*E*cos(Theta)/850.))
       * cos(Theta)*cos(Theta);
}

#endif
