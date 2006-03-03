///////////////////////////////////////////////////////////////////////////////
// File: PHcalValidInfoNxN.cc
// Description: Persistent NxN analysis informaion for SimG4HcalValidation
///////////////////////////////////////////////////////////////////////////////
#include "SimDataFormats/HcalValidation/interface/PHcalValidInfoNxN.h"
//#include <iostream>

void PHcalValidInfoNxN::fillHvsE(double ee, double he, double hoe, 
				 double etot) {
  ecalNxNr = (float)ee;
  hcalNxNr = (float)he;
  hoNxNr   = (float)hoe;
  etotNxNr = (float)etot;
}

void PHcalValidInfoNxN::fillEcollectNxN(double een, double hen, double hoen,
					double etotn) {
  ecalNxN = (float)een;
  hcalNxN = (float)hen;
  hoNxN   = (float)hoen;
  etotNxN = (float)etotn;
}

void PHcalValidInfoNxN::fillTProfileNxN (double e, int i, double t) {  
  idNxN.push_back((float)i);
  eNxN.push_back((float)e);
  tNxN.push_back((float)t);
  nNxN++;

  //  std::cout << " fillTProfileNxN - nNxN = " << nNxN << std::endl;

}
