///////////////////////////////////////////////////////////////////////////////
// File: PHcalValidInfoNxN.cc
// Description: Persistent NxN analysis informaion for SimG4HcalValidation
///////////////////////////////////////////////////////////////////////////////
#include "SimG4Validation/Hcal/interface/PHcalValidInfoNxN.h"

#include <cmath>

void PHcalValidInfoNxN::fillHvsE(double ee, double he, double hoe, 
				 double etot) {
  ecalNxNr = (float)ee;
  hcalNxNr = (float)he;
  hoNxNr   = (float)hoe;
  etotNxNr = (float)etot;
}

void PHcalValidInfoNxN::fillEcollectNxN(double ee, double he, double hoe,
					double etot) {
  ecalNxN = (float)ee;
  hcalNxN = (float)he;
  hoNxN   = (float)hoe;
  etotNxN = (float)etot;
}

void PHcalValidInfoNxN::fillTProfileNxN (double e, int i, double t) {

    nXn.resize(nIxI+1);
    nXn[nIxI].id    = i;
    nXn[nIxI].e     = e;
    nXn[nIxI].t     = t;
    nIxI++;
}
