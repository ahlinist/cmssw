#include "HadronPhysicsQGSC.hh"

#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

HadronPhysicsQGSC::HadronPhysicsQGSC(const G4String& name)
                    :  G4VPhysicsConstructor(name) 
{
#ifdef G4v7
  theNeutrons.RegisterMe(&theQGSCNeutron);
  theNeutrons.RegisterMe(&theLEPNeutron);
  theLEPNeutron.SetMaxInelasticEnergy(25*GeV);

  thePro.RegisterMe(&theQGSCPro);
  thePro.RegisterMe(&theLEPPro);
  theLEPPro.SetMaxEnergy(25*GeV);

  thePiK.RegisterMe(&theQGSCPiK);
  thePiK.RegisterMe(&theLEPPiK);
  theLEPPiK.SetMaxEnergy(25*GeV);
#endif
}

HadronPhysicsQGSC::~HadronPhysicsQGSC() {
#ifndef G4v7
  delete theMiscLHEP;
  delete theQGSCPro;
  delete theLEPPro;
  delete thePro;
  delete theQGSCPiK;
  delete theLEPPiK;
  delete thePiK;
#endif
}
#ifndef G4v7

void HadronPhysicsQGSC::CreateModels()
{
  theNeutrons=new G4NeutronBuilder;
  theNeutrons->RegisterMe(theQGSCNeutron=new G4QGSCNeutronBuilder);
  theNeutrons->RegisterMe(theLEPNeutron=new G4LEPNeutronBuilder);
  theLEPNeutron->SetMaxInelasticEnergy(25*GeV);

  thePro=new G4ProtonBuilder;
  thePro->RegisterMe(theQGSCPro=new G4QGSCProtonBuilder);
  thePro->RegisterMe(theLEPPro=new G4LEPProtonBuilder);
  theLEPPro->SetMaxEnergy(25*GeV);

  thePiK=new G4PiKBuilder;
  thePiK->RegisterMe(theQGSCPiK=new G4QGSCPiKBuilder);
  thePiK->RegisterMe(theLEPPiK=new G4LEPPiKBuilder);
  theLEPPiK->SetMaxEnergy(25*GeV);

  theMiscLHEP=new G4MiscLHEPBuilder;
}
#endif

void HadronPhysicsQGSC::ConstructParticle()
{
  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();

  G4BaryonConstructor pBaryonConstructor;
  pBaryonConstructor.ConstructParticle();

  G4ShortLivedConstructor pShortLivedConstructor;
  pShortLivedConstructor.ConstructParticle();  
}

#include "G4ProcessManager.hh"

void HadronPhysicsQGSC::ConstructProcess()
{
#ifdef G4v7
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
