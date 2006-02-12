#ifndef  PEcalValidInfo_H
#define  PEcalValidInfo_H

/*----------------------------------------------------------
Class Description:
      The Class, PEcalValidInfo, includes all the quantities 
    needed to validate for the Simulation of Eletromagnetic 
    Calorimetor. 
       The Objects of this class will be save into regular 
    Root file vis EDProducer.

Author: X.HUANG ( huangxt@fnal.gov )
Date:  Dec, 2005

---------------------------------------------------------*/

#include <string>
#include <vector>
#include <CLHEP/Vector/LorentzVector.h>

class EcalTestAnalysis; 

class PEcalValidInfo 
{
   friend  class   EcalTestAnalysis;
   friend  class   PreshowerTestAnalysis;
   typedef  std::vector<float>   FloatVector;

public:
   PEcalValidInfo()
  :e1(0.0),e4(0.0),e9(0.0),e16(0.0),e25(0.0),
   totalEInEE(0.0), totalEInEB(0.0), totalEInES(0.0),
   totalHits(0), nHitsInEE(0),nHitsInEB(0),nHitsInES(0)
{

 }


   ~PEcalValidInfo() {} 

   // Get functions.
   float  e1x1() const { return e1; }
   float  e2x2() const { return e4; }
   float  e3x3() const { return e9; }
   float  e4x4() const { return e16;}
   float  e5x5() const { return e25;}

   float  eInEE() const { return totalEInEE; }
   float  eInEB() const { return totalEInEB; }
   float  eInES() const { return totalEInES; }

   int    hitsInEcal() const { return totalHits; }
   int    hitsInEE() const { return nHitsInEE; }
   int    hitsInEB() const { return nHitsInEB; }
   int    hitsInES() const { return nHitsInES; }
       
   FloatVector  bX0() const { return eBX0; }
   FloatVector  eX0() const { return eEX0; }

   FloatVector  phiOfEEHits() const { return phiOfEECaloG4Hit; }
   FloatVector  etaOfEEHits() const { return etaOfEECaloG4Hit; }
   FloatVector  tOfEEHits()   const { return tOfEECaloG4Hit;   }
   FloatVector  eOfEEHits()   const { return eOfEECaloG4Hit;   }

   FloatVector  phiOfEBHits() const { return phiOfEBCaloG4Hit; }
   FloatVector  etaOfEBHits() const { return etaOfEBCaloG4Hit; }
   FloatVector  tOfEBHits()   const { return tOfEBCaloG4Hit;   }
   FloatVector  eOfEBHits()   const { return eOfEBCaloG4Hit;   }

   FloatVector  phiOfiESHits() const { return phiOfESCaloG4Hit; }
   FloatVector  etaOfESHits() const { return etaOfESCaloG4Hit; }
   FloatVector  tOfESHits()   const { return tOfESCaloG4Hit;   }
   FloatVector  eOfESHits()   const { return eOfESCaloG4Hit;   }

   HepLorentzVector  momentum() const { return theMomentum; }
   HepLorentzVector  vertex() const  { return theVertex; }
   

private:
 
   float  e1;       //Energy deposition in cluser1x1
   float  e4;       //Energy deposition in cluser2x2
   float  e9;       //Energy deposition in cluser3x3
   float  e16;      //Energy deposition in cluser4x4
   float  e25;      //Energy deposition in cluser5x5

 
   float  totalEInEE;       //The Total Energy deposited in EE;
   float  totalEInEB;       //The Total Energy deposited in EB;
   float  totalEInES;       //The Total Energy deposited in ES;


   int totalHits;          //Total number of Hits.
   int nHitsInEE;          //Total number of Hits in EE.
   int nHitsInEB;          //Total number of Hits in EB.
   int nHitsInES;          //Total number of Hits in ES.
   
   FloatVector eBX0;       // longitudinal Energy deposition In EB.
   FloatVector eEX0;       // longitudinal Energy deposition In EE.
    
   FloatVector  phiOfEECaloG4Hit;    // Phi of Hits.
   FloatVector  etaOfEECaloG4Hit;    // Eta of Hits.
   FloatVector  tOfEECaloG4Hit;      // Tof of Hits.
   FloatVector  eOfEECaloG4Hit;      // Energy depostion of Hits.
 
   FloatVector  phiOfESCaloG4Hit;    // Phi of Hits.
   FloatVector  etaOfESCaloG4Hit;    // Eta of Hits.
   FloatVector  tOfESCaloG4Hit;      // Tof of Hits.
   FloatVector  eOfESCaloG4Hit;      // Energy depostion of Hits.

   FloatVector  phiOfEBCaloG4Hit;    // Phi of Hits.
   FloatVector  etaOfEBCaloG4Hit;    // Eta of Hits.
   FloatVector  tOfEBCaloG4Hit;      // Tof of Hits.
   FloatVector  eOfEBCaloG4Hit;      // Energy depostion of Hits.



   int thePID;                      // add more ??
   HepLorentzVector theMomentum;  
   HepLorentzVector theVertex;
};


#endif

