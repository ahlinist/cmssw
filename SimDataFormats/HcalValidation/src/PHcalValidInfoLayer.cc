///////////////////////////////////////////////////////////////////////////////
// File: PHcalValidInfoLayer.cc
// Description: Persistent Layer and Hit informaion for SimG4HcalValidation
///////////////////////////////////////////////////////////////////////////////
#include "SimDataFormats/HcalValidation/interface/PHcalValidInfoLayer.h"

#include <iostream>
#include <cmath>

void PHcalValidInfoLayer::fillLayers(double el[], double ed[], double ho,
				     double hbhe) {

  eLayer.resize(20);
  for (int i = 0; i < 20; i++) {
    double en  = 0.001*el[i];
    eLayer[i] = en;
  }
  eDepth.resize(4);
  for (int i = 0; i < 4; i++) {
    double en  = 0.001*ed[i];
    eDepth[i] = en;
  }
  eHO   = ho;
  eHBHE = hbhe;
}

void PHcalValidInfoLayer::fillHF(double fibl, double fibs, double enec,
				 double enhc) {
  elongHF  = fibl;
  eshortHF = fibs;
  eEcalHF  = enec;
  eHcalHF  = enhc;
}

void PHcalValidInfoLayer::fillHits(int nHits, int lay, int unitID, double eta,
				   double phi, double ehit, double t){

    hits.resize(nHits+1);
    hits[nHits].layer = lay;
    hits[nHits].id    = unitID;
    hits[nHits].eta   = eta;
    hits[nHits].phi   = phi;
    hits[nHits].e     = ehit;
    hits[nHits].t     = t;
}
