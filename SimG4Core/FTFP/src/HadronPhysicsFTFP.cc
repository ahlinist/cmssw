#include "HadronPhysicsFTFP.hh"

#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

HadronPhysicsFTFP::HadronPhysicsFTFP(const G4String& name)
                    :  G4VPhysicsConstructor(name) 
{
#ifdef G4V7
  theNeutrons.RegisterMe(&theFTFPNeutron);
  theNeutrons.RegisterMe(&theLEPNeutron);
  theLEPNeutron.SetMaxInelasticEnergy(25*GeV);

  thePro.RegisterMe(&theFTFPPro);
  thePro.RegisterMe(&theLEPPro);
  theLEPPro.SetMaxEnergy(25*GeV);

  thePiK.RegisterMe(&theFTFPPiK);
  thePiK.RegisterMe(&theLEPPiK);
  theLEPPiK.SetMaxEnergy(25*GeV);
#endif
}

HadronPhysicsFTFP::~HadronPhysicsFTFP() {
#ifndef G4V7
  delete theNeutrons;
  delete theLEPNeutron;
  delete theFTFPNeutron;
  delete thePiK;
  delete theLEPPiK;
  delete theFTFPPiK;
  delete thePro;
  delete theLEPPro;
  delete theFTFPPro;
  delete theMiscLHEP;
#endif
}
#ifndef G4V7

void HadronPhysicsFTFP::CreateModels()
{

  theNeutrons=new G4NeutronBuilder;
  theNeutrons->RegisterMe(theFTFPNeutron=new G4FTFPNeutronBuilder);
  theNeutrons->RegisterMe(theLEPNeutron=new G4LEPNeutronBuilder);
  theLEPNeutron->SetMaxInelasticEnergy(25*GeV);

  thePro=new G4ProtonBuilder;
  thePro->RegisterMe(theFTFPPro=new G4FTFPProtonBuilder);
  thePro->RegisterMe(theLEPPro=new G4LEPProtonBuilder);
  theLEPPro->SetMaxEnergy(25*GeV);

  thePiK=new G4PiKBuilder;
  thePiK->RegisterMe(theFTFPPiK=new G4FTFPPiKBuilder);
  thePiK->RegisterMe(theLEPPiK=new G4LEPPiKBuilder);
  theLEPPiK->SetMaxEnergy(25*GeV);

  theMiscLHEP=new G4MiscLHEPBuilder;
}
#endif

void HadronPhysicsFTFP::ConstructParticle()
{
  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();

  G4BaryonConstructor pBaryonConstructor;
  pBaryonConstructor.ConstructParticle();

  G4ShortLivedConstructor pShortLivedConstructor;
  pShortLivedConstructor.ConstructParticle();  
}

#include "G4ProcessManager.hh"

void HadronPhysicsFTFP::ConstructProcess()
{
#ifdef G4V7
  theNeutrons.Build();
  thePro.Build();
  thePiK.Build();
  theMiscLHEP.Build();
  theStoppingHadron.Build();
  theHadronQED.Build();
#else
  CreateModels();
  theNeutrons->Build();
  thePro->Build();
  thePiK->Build();
  theMiscLHEP->Build();
#endif
}
// 2002 by J.P. Wellisch
