///////////////////////////////////////////////////////////////////////////////
// File: PHcalValidInfoLayer.cc
// Description: Persistent Layer and Hit informaion for SimG4HcalValidation
///////////////////////////////////////////////////////////////////////////////
#include "SimDataFormats/HcalValidation/interface/PHcalValidInfoLayer.h"
//#include <iostream>

void PHcalValidInfoLayer::fillLayers(double el[], double ed[], double ho,
				     double hbhe) {

  for (int i = 0; i < 20; i++) {
    double en  = 0.001*el[i]; // GeV
    eLayer.push_back((float)en);
  }
  for (int i = 0; i < 4; i++) {
    double en  = 0.001*ed[i]; // GeV
    eDepth.push_back((float)en);
  }
  eHO   = (float)ho;
  eHBHE = (float)hbhe; // MeV
}

void PHcalValidInfoLayer::fillHF(double fibl, double fibs, double enec,
				 double enhc) {
  elongHF  = (float)fibl;
  eshortHF = (float)fibs;
  eEcalHF  = (float)enec;
  eHcalHF  = (float)enhc;
}

void PHcalValidInfoLayer::fillHits(int nHits, int lay, int unitID, double eta,
				   double phi, double ehit, double t){

  hitLayer.push_back((float)lay);
  hitId.push_back((float)unitID);
  hitEta.push_back((float)eta);
  hitPhi.push_back((float)phi);
  hitE.push_back((float)ehit);
  hitTime.push_back((float)t);
  hitN++;

  //  std::cout << " fillHits: nHits,hitN = " << nHits << "," << hitN << std::endl;

}
