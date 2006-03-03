///////////////////////////////////////////////////////////////////////////////
// File: PHcalValidInfoNxN.h
// Persistent NxN informaion from analysis in SimG4HcalValidation
///////////////////////////////////////////////////////////////////////////////
#ifndef  PHcalValidInfoNxN_H
#define  PHcalValidInfoNxN_H

#include <string>
#include <vector>
#include <memory>

class SimG4HcalValidation;


class PHcalValidInfoNxN {

  friend class SimG4HcalValidation;

public:
       
  PHcalValidInfoNxN(): nNxN(0), ecalNxNr(0), hcalNxNr(0.), hoNxNr(0.), 
    etotNxNr(0.), ecalNxN(0.), hcalNxN(0.), hoNxN(0.), etotNxN(0.) {}
  virtual ~PHcalValidInfoNxN() {}

  // access
  std::vector<float> idnxn() const {return idNxN;}
  std::vector<float>  enxn() const {return  eNxN;}
  std::vector<float>  tnxn() const {return  tNxN;}
  int                 nnxn() const {return  nNxN;}
  
  float           ecalnxnr() const {return ecalNxNr;}
  float           hcalnxnr() const {return hcalNxNr;}
  float             honxnr() const {return   hoNxNr;}
  float           etotnxnr() const {return etotNxNr;}

  float           ecalnxn () const {return ecalNxN ;}
  float           hcalnxn () const {return hcalNxN ;}
  float             honxn () const {return   hoNxN ;}
  float           etotnxn () const {return etotNxN ;}
  

  // fill
  void fillHvsE        (double ee, double he, double hoe, double etot);
  void fillEcollectNxN (double een, double hen, double hoen, double etotn);
  void fillTProfileNxN (double e, int i, double t);

private:

  int                nNxN;
  float              ecalNxNr, hcalNxNr, hoNxNr, etotNxNr;
  float              ecalNxN,  hcalNxN,  hoNxN,  etotNxN;
  std::vector<float> idNxN; // float for int
  std::vector<float> eNxN;
  std::vector<float> tNxN;

};

#endif
