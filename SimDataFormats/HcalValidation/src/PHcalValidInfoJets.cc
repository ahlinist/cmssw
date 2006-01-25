///////////////////////////////////////////////////////////////////////////////
// File: PHcalValidInfoJets.cc
// Description: Persistent Jet analysis informaion for SimG4HcalValidation
///////////////////////////////////////////////////////////////////////////////
#include "SimDataFormats/HcalValidation/interface/PHcalValidInfoJets.h"

#include <cmath>

void PHcalValidInfoJets::fillTProfileJet(double e, double r,  double t) {

  jetHit.resize(nJetHit+1);
  jetHit[nJetHit].e = (float)e;
  jetHit[nJetHit].r = (float)r;
  jetHit[nJetHit].t = (float)t;
  nJetHit++;
}

void PHcalValidInfoJets::fillEcollectJet(double ee, double he, 
					 double hoe, double etot) {
 
  ecalJet = (float)ee;
  hcalJet = (float)he;
  hoJet   = (float)hoe;
  etotJet = (float)etot;
}

void PHcalValidInfoJets::fillEtaPhiProfileJet(double eta0, double phi0, 
					      double eta,  double phi,
					      double dist) {

  detaJet = (float)(eta-eta0);
  dphiJet = (float)(phi-phi0);
  drJet   = (float)dist;
}

void PHcalValidInfoJets::fillJets(std::vector<double> en,
				  std::vector<double> eta,
				  std::vector<double> phi) {

  int nJets = en.size();
  jets.resize(nJets);
  for (int i = 0; i < nJets; i++) {
    jets[i].e   = (float)en[i];
    jets[i].eta = (float)eta[i];
    jets[i].phi = (float)phi[i];
  }
}

void PHcalValidInfoJets::fillDiJets(double mass) {
  dijetM = (float)mass;
}

