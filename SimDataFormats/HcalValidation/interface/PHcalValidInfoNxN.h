///////////////////////////////////////////////////////////////////////////////
// File: PHcalValidInfoNxN.h
// Persistent NxN informaion from analysis in SimG4HcalValidation
///////////////////////////////////////////////////////////////////////////////
#ifndef  PHcalValidInfoNxN_H
#define  PHcalValidInfoNxN_H

#include <string>
#include <vector>
#include <memory>

class PHcalValidInfoNxN {

public:
       
  PHcalValidInfoNxN(): nIxI(0), ecalNxNr(0), hcalNxNr(0), hoNxNr(0), 
    etotNxNr(0), ecalNxN(0), hcalNxN(0), hoNxN(0), etotNxN(0) {}
  virtual ~PHcalValidInfoNxN() {}

  void fillHvsE        (double ee, double he, double hoe, double etot);
  void fillEcollectNxN (double ee, double he, double hoe, double etot);
  void fillTProfileNxN (double e, int i, double t);

  struct NxN {
    NxN() {}
    int   id;
    float e;
    float t;
  };

private:

  int                nIxI;
  float              ecalNxNr, hcalNxNr, hoNxNr, etotNxNr;
  float              ecalNxN, hcalNxN, hoNxN, etotNxN;
  std::vector<NxN>   nXn;

};

#endif
