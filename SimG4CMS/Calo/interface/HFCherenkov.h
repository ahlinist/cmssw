///////////////////////////////////////////////////////////////////////////////
// File:  HFCherenkov.h
// Description: Generate Cherenkov photons for HF
///////////////////////////////////////////////////////////////////////////////
#ifndef HFCherenkov_h
#define HFCherenkov_h 1

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "G4DynamicParticle.hh"
#include "G4ParticleDefinition.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"

#include <vector>

class HFCherenkov {
  
public:

   HFCherenkov(edm::ParameterSet const & p);
   virtual ~HFCherenkov();
  
   int                 computeNPE(G4ParticleDefinition* pDef,double pBeta, 
				  double u, double v, double w, 
				  double step_length, double zFiber, 
				  double Dose, int Npe_Dose);

   int                 computeNPhTrapped(double pBeta, double u, double v, 
					 double w, double step_length,
					 double zFiber, double Dose,
					 int Npe_Dose);
   double              smearNPE(G4int Npe);				  

   std::vector<double> getMom();
   std::vector<double> getWL();
   std::vector<double> getWLIni();
   std::vector<double> getWLTrap();
   std::vector<double> getWLAtten();
   std::vector<double> getWLHEM();
   std::vector<double> getWLQEff();
   void                clearVectors();
					  
private:

   bool                isApplicable(const G4ParticleDefinition* aParticleType);
   // Returns true -> 'is applicable', for all charged particles.
   int                 computeNbOfPhotons(double pBeta, double step_length);
   double              computeQEff(double wavelength);
   double              computeHEMEff(double wavelength);

private:

   int                 verbosity;
   double              ref_index;
   double              lambda1, lambda2;
   double              aperture, apertureTrap;
   double              gain;
   bool                checkSurvive;

   G4ThreeVector       phMom;
   std::vector<double> wl;
   std::vector<double> momZ;
   std::vector<double> wlini;
   std::vector<double> wltrap;
   std::vector<double> wlatten;
   std::vector<double> wlhem;
   std::vector<double> wlqeff;
};

#endif

