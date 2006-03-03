///////////////////////////////////////////////////////////////////////////////
// File: PHcalValidInfoJets.cc
// Description: Persistent Jet analysis informaion for SimG4HcalValidation
///////////////////////////////////////////////////////////////////////////////
#include "SimDataFormats/HcalValidation/interface/PHcalValidInfoJets.h"

// #include <iostream>

void PHcalValidInfoJets::fillTProfileJet(double e, double r,  double t) {
  jetHite.push_back((float)e);
  jetHitr.push_back((float)r);
  jetHitt.push_back((float)t);
  nJetHit++;

  //  std::cout << " fillTProfileJet - nJetHit = " << nJetHit << std::endl;
  
}

void PHcalValidInfoJets::fillEcollectJet(double ee, double he, 
					 double hoe, double etot) {
  // hardest jet properties

  ecalJet = (float)ee;
  hcalJet = (float)he;
    hoJet = (float)hoe;
  etotJet = (float)etot;
}

void PHcalValidInfoJets::fillEtaPhiProfileJet(double eta0, double phi0, 
					      double eta,  double phi,
					      double dist) {
  detaJet = (float)(eta-eta0);
  dphiJet = (float)(phi-phi0);
    drJet = (float)dist;
}

void PHcalValidInfoJets::fillJets(std::vector<double> en,
				  std::vector<double> eta,
				  std::vector<double> phi) {
  nJet = en.size();
  for (int i = 0; i < nJet; i++) {
    jetE.push_back((float)en[i]);
    jetEta.push_back((float)eta[i]);
    jetPhi.push_back((float)phi[i]);
  }

  //  std::cout << " fillJets - nJet = " << nJet << std::endl;

}

void PHcalValidInfoJets::fillDiJets(double mass) {
  dijetM = (float)mass;
}

