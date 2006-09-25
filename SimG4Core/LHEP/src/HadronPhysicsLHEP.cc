#include "HadronPhysicsLHEP.hh"

#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

HadronPhysicsLHEP::HadronPhysicsLHEP(const G4String& name)
                    :  G4VPhysicsConstructor(name) 
{
#ifdef G4v7
  theNeutrons.RegisterMe(&theLHEPNeutron);
  thePro.RegisterMe(&theLHEPPro);
  thePiK.RegisterMe(&theLHEPPiK);
#endif
}

HadronPhysicsLHEP::~HadronPhysicsLHEP() {
#ifndef G4v7
  delete theLHEPNeutron;
  delete theNeutrons;
  delete theLHEPPro;
  delete thePro;
  delete theLHEPPiK;
  delete thePiK;
#endif
}
#ifndef G4v7

void HadronPhysicsLHEP::CreateModels()
{
  theNeutrons=new G4NeutronBuilder;
  theNeutrons->RegisterMe(theLHEPNeutron=new G4LHEPNeutronBuilder);
 
  thePro=new G4ProtonBuilder;
  thePro->RegisterMe(theLHEPPro=new G4LHEPProtonBuilder);
 
  thePiK=new G4PiKBuilder;
  thePiK->RegisterMe(theLHEPPiK=new G4LHEPPiKBuilder);
 
  theMiscLHEP=new G4MiscLHEPBuilder;
  theStoppingHadron=new G4StoppingHadronBuilder;
}
#endif

void HadronPhysicsLHEP::ConstructParticle()
{
  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();

  G4BaryonConstructor pBaryonConstructor;
  pBaryonConstructor.ConstructParticle();

  G4ShortLivedConstructor pShortLivedConstructor;
  pShortLivedConstructor.ConstructParticle();  
}

#include "G4ProcessManager.hh"

void HadronPhysicsLHEP::ConstructProcess()
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
  theStoppingHadron->Build();
#endif
}
// 2002 by J.P. Wellisch
