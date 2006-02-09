///////////////////////////////////////////////////////////////////////////////
// File: PHcalValidInfoLayer.h
// Persistent Layer and Hit informaion from analysis in SimG4HcalValidation
///////////////////////////////////////////////////////////////////////////////
#ifndef  PHcalValidInfoLayer_H
#define  PHcalValidInfoLayer_H

#include <string>
#include <vector>
#include <memory>

class PHcalValidInfoLayer {

public:
       
  PHcalValidInfoLayer(): eHO(0),eHBHE(0), elongHF(0), eshortHF(0), 
    eEcalHF(0), eHcalHF(0) {}
  virtual ~PHcalValidInfoLayer() {}

  void fillLayers (double el[], double ed[], double ho, double hbhe);
  void fillHF     (double fibl, double fibs, double enec, double enhc);
  void fillHits   (int Nhits, int lay, int unitID, double eta, double phi, 
		   double ehit, double t); 

  struct Hit {
    Hit() {}
    int   layer;
    int   id;
    float eta;
    float phi;
    float e;
    float t;
  };

private:

  std::vector<float> eLayer;
  std::vector<float> eDepth;
  float              eHO, eHBHE;
  float              elongHF, eshortHF, eEcalHF, eHcalHF;
  std::vector<Hit>   hits;

};

#endif
