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
   friend  class   SimHitSingleTest;
   friend  class   EcalSimHitsValidProducer;
   typedef  std::vector<float>   FloatVector;

public:
   PEcalValidInfo()
  :ee1(0.0),ee4(0.0),ee9(0.0),ee16(0.0),ee25(0.0),
   eb1(0.0),eb4(0.0),eb9(0.0),eb16(0.0),eb25(0.0),
   totalEInEE(0.0), totalEInEB(0.0), totalEInES(0.0),
   totalHits(0), nHitsInEE(0),nHitsInEB(0),nHitsInES(0),nHitsIn1ES(0),nHitsIn2ES(0) 
{

 }


   ~PEcalValidInfo() {} 

   // Get functions.
   float  ee1x1() const { return ee1; }
   float  ee2x2() const { return ee4; }
   float  ee3x3() const { return ee9; }
   float  ee4x4() const { return ee16;}
   float  ee5x5() const { return ee25;}

   float  eb1x1() const { return eb1; }
   float  eb2x2() const { return eb4; }
   float  eb3x3() const { return eb9; }
   float  eb4x4() const { return eb16;}
   float  eb5x5() const { return eb25;}

   float  eInEE()  const { return totalEInEE; }
   float  eInEB()  const { return totalEInEB; }
   float  eInES()  const { return totalEInES; }

   float  eInEEzp()  const { return totalEInEEzp; }
   float  eInEEzm()  const { return totalEInEEzm; }

   float  eInESzp()  const { return totalEInESzp; }
   float  eInESzm()  const { return totalEInESzm; }

   int    hitsInEcal() const { return totalHits; }
   int    hitsInEE()   const { return nHitsInEE; }
   int    hitsInEB()   const { return nHitsInEB; }
   int    hitsInES()   const { return nHitsInES; }
   int    hitsIn1ES()  const { return nHitsIn1ES;}
   int    hitsIn2ES()  const { return nHitsIn2ES;}
  
   int    hitsIn1ESzp()  const { return nHitsIn1ESzp;}
   int    hitsIn1ESzm()  const { return nHitsIn1ESzm;}
   int    hitsIn2ESzp()  const { return nHitsIn2ESzp;}
   int    hitsIn2ESzm()  const { return nHitsIn2ESzm;}       

   int    crystalInEB()   const { return nCrystalInEB;}
   int    crystalInEEzp() const { return nCrystalInEEzp; }
   int    crystalInEEzm() const { return nCrystalInEEzm; }

   FloatVector  bX0() const { return eBX0; }
   FloatVector  eX0() const { return eEX0; }


   FloatVector  eIn1ES() const { return eOf1ES; }
   FloatVector  eIn2ES() const { return eOf2ES; }
   FloatVector  zOfInES()  const { return zOfES;  }

   FloatVector  eIn1ESzp() const { return eOf1ESzp; }
   FloatVector  eIn1ESzm() const { return eOf1ESzm; }
 
   FloatVector  eIn2ESzp() const { return eOf2ESzp; }
   FloatVector  eIn2ESzm() const { return eOf2ESzm; }

   FloatVector  phiOfEEHits() const { return phiOfEECaloG4Hit; }
   FloatVector  etaOfEEHits() const { return etaOfEECaloG4Hit; }
   FloatVector  tOfEEHits()   const { return tOfEECaloG4Hit;   }
   FloatVector  eOfEEHits()   const { return eOfEECaloG4Hit;   }
   FloatVector  eOfEEPlusHits()    const { return eOfEEPlusCaloG4Hit;   }
   FloatVector  eOfEEMinusHits()   const { return eOfEEMinusCaloG4Hit;   }


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
   
   int pId()  const { return thePID; }   

private:
 
   float  ee1;       //Energy deposition in cluser1x1
   float  ee4;       //Energy deposition in cluser2x2
   float  ee9;       //Energy deposition in cluser3x3
   float  ee16;      //Energy deposition in cluser4x4
   float  ee25;      //Energy deposition in cluser5x5

   float  eb1;       //Energy deposition in cluser1x1
   float  eb4;       //Energy deposition in cluser2x2
   float  eb9;       //Energy deposition in cluser3x3
   float  eb16;      //Energy deposition in cluser4x4
   float  eb25;      //Energy deposition in cluser5x5


 
   float  totalEInEE;       //The Total Energy deposited in EE;
   float  totalEInEB;       //The Total Energy deposited in EB;
   float  totalEInES;       //The Total Energy deposited in ES;
 
   float  totalEInEEzp;
   float  totalEInEEzm;
   float  totalEInESzp;
   float  totalEInESzm;



   int totalHits;          //Total number of Hits.
   int nHitsInEE;          //Total number of Hits in EE.
   int nHitsInEB;          //Total number of Hits in EB.
   int nHitsInES;          //Total number of Hits in ES.
   int nHitsIn1ES;         //Total number of Hits in 1st Layer of ES;
   int nHitsIn2ES;         //Total number of Hits in 2nd Layer of ES;

   int nHitsIn1ESzp;
   int nHitsIn1ESzm;
   int nHitsIn2ESzp;
   int nHitsIn2ESzm;       

   int nCrystalInEB;
   int nCrystalInEEzp;
   int nCrystalInEEzm;


   FloatVector eBX0;       // longitudinal Energy deposition In EB.
   FloatVector eEX0;       // longitudinal Energy deposition In EE.

   FloatVector  eOf1ES;    // Energy deposition of Hits in 1st layer of ES;
   FloatVector  eOf2ES;    // Energy deposition of Hits in 2nd layer of ES;              
   FloatVector  zOfES;


   FloatVector  eOf1ESzp;
   FloatVector  eOf1ESzm;
   FloatVector  eOf2ESzp;
   FloatVector  eOf2ESzm;

   FloatVector  phiOfEECaloG4Hit;    // Phi of Hits.
   FloatVector  etaOfEECaloG4Hit;    // Eta of Hits.
   FloatVector  tOfEECaloG4Hit;      // Tof of Hits.
   FloatVector  eOfEECaloG4Hit;      // Energy depostion of Hits.
   FloatVector  eOfEEPlusCaloG4Hit;       // Energy depostion of Hits.
   FloatVector  eOfEEMinusCaloG4Hit;      // Energy depostion of Hits.

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

