#ifndef G4EMBuilder_h
#define G4EMBuilder_h 1

#include "globals.hh"
#include "G4ios.hh"

#include "G4PhotoElectricEffect.hh"
#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4MultipleScattering.hh"
#include "G4eIonisation.hh"
#ifdef G4V7
#include "G4eBremsstrahlung.hh"
#else
#include "G4eBremsstrahlungCMS.hh"
#endif
#include "G4eplusAnnihilation.hh"

#include "G4EMTailorer.hh"
#include "G4SynchrotronRadiation.hh"
#include "G4ElectroNuclearBuilder.hh"

class G4EMBuilder 
{
  public: 
    G4EMBuilder();
    virtual ~G4EMBuilder();

  public: 
    void Build();
    
    void Synch(G4String & aState);
    void GammaNuclear(G4String & aState);

  protected:
    G4PhotoElectricEffect thePhotoEffect;
    G4ComptonScattering theComptonEffect;
    G4GammaConversion thePairProduction;
  
    G4MultipleScattering theElectronMultipleScattering;
    G4eIonisation theElectronIonisation;
#ifdef G4V7
    G4eBremsstrahlung theElectronBremsStrahlung;
#else
    G4eBremsstrahlungCMS theElectronBremsStrahlung;
#endif
    G4MultipleScattering thePositronMultipleScattering;
    G4eIonisation thePositronIonisation; 
#ifdef G4V7
    G4eBremsstrahlung thePositronBremsStrahlung;  
#else
    G4eBremsstrahlungCMS thePositronBremsStrahlung;  
#endif
    G4eplusAnnihilation theAnnihilation;
    G4bool wasActivated;
    
    G4EMTailorer * theT;
    G4SynchrotronRadiation theElectronSynch;
    G4SynchrotronRadiation thePositronSynch;
    G4bool synchOn;
    G4ElectroNuclearBuilder theGNPhysics;
    G4bool gammNucOn;
};
// 2002 by J.P. Wellisch
#endif





