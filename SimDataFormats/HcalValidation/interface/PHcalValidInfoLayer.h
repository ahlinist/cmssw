///////////////////////////////////////////////////////////////////////////////
// File: PHcalValidInfoLayer.h
// Persistent Layer and Hit informaion from analysis in SimG4HcalValidation
///////////////////////////////////////////////////////////////////////////////
#ifndef  PHcalValidInfoLayer_H
#define  PHcalValidInfoLayer_H

#include <string>
#include <vector>
#include <memory>

class SimG4HcalValidation;

class PHcalValidInfoLayer {

  friend class SimG4HcalValidation;

public:
       
  PHcalValidInfoLayer(): hitN(0), eHO(0.0),eHBHE(0.0), elongHF(0.0),
			 eshortHF(0.0), eEcalHF(0.0), eHcalHF(0.0) {}
  virtual ~PHcalValidInfoLayer() {}

  // access
  int                    nHit()   const {return hitN;}

  float                   eho()   const {return eHO;}    
  float                 ehbhe()   const {return eHBHE;}    
  float               elonghf()   const {return elongHF;}    
  float              eshorthf()   const {return eshortHF;}    
  float               eecalhf()   const {return eEcalHF;}    
  float               ehcalhf()   const {return eHcalHF;}    

  std::vector<float>   elayer()   const {return eLayer;}
  std::vector<float>   edepth()   const {return eDepth;}

  std::vector<float>   etaHit()   const {return hitEta;} 
  std::vector<float>   phiHit()   const {return hitPhi;} 
  std::vector<float>     eHit()   const {return hitE;} 
  std::vector<float>     tHit()   const {return hitTime;} 
  std::vector<float> layerHit()   const {return hitLayer;} 
  std::vector<float>    idHit()   const {return hitId;} 

  // filling
  void fillLayers (double el[], double ed[], double ho, double hbhe);
  void fillHF     (double fibl, double fibs, double enec, double enhc);
  void fillHits   (int Nhits, int lay, int unitID, double eta, double phi, 
		   double ehit, double t); 
  //  void clear();


private:

  int                hitN;
  float              eHO, eHBHE;
  float              elongHF, eshortHF, eEcalHF, eHcalHF;
  std::vector<float> eLayer;
  std::vector<float> eDepth;
  // SimHits parameters
  std::vector<float> hitLayer; // float for int
  std::vector<float> hitId;    // float for int
  std::vector<float> hitEta;
  std::vector<float> hitPhi;
  std::vector<float> hitE;
  std::vector<float> hitTime;

};

#endif
