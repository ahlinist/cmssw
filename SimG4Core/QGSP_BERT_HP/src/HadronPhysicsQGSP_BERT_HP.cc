#include "HadronPhysicsQGSP_BERT_HP.hh"

#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

HadronPhysicsQGSP_BERT_HP::HadronPhysicsQGSP_BERT_HP(const G4String& name)
                    :  G4VPhysicsConstructor(name) 
{
  //Neutron
  theNeutrons.RegisterMe(&theQGSPNeutron);
  //
  theNeutrons.RegisterMe(&theLEPNeutron);
  theLEPNeutron.SetMinEnergy(19.9*MeV);
  theLEPNeutron.SetMinInelasticEnergy(9.5*GeV);
  theLEPNeutron.SetMaxInelasticEnergy(25*GeV);
  //
  theNeutrons.RegisterMe(&theBertiniNeutron);
  theBertiniNeutron.SetMaxEnergy(9.9*GeV);
  theBertiniNeutron.SetMinEnergy(19.9*MeV);
  //
  theNeutrons.RegisterMe(&theHPNeutron);

  thePro.RegisterMe(&theQGSPPro);
  thePro.RegisterMe(&theLEPPro);
  theLEPPro.SetMaxEnergy(25*GeV);
  //
  thePro.RegisterMe(&theBertiniPro);
  theLEPPro.SetMinEnergy(9.5*GeV);
  theBertiniPro.SetMaxEnergy(9.9*GeV);
  
  thePiK.RegisterMe(&theQGSPPiK);
  thePiK.RegisterMe(&theLEPPiK);
  theLEPPiK.SetMaxEnergy(25*GeV);
  //
  thePiK.RegisterMe(&theBertiniPiK);
  theLEPPiK.SetMinPionEnergy(9.5*GeV);
  theBertiniPiK.SetMaxEnergy(9.9*GeV);
}

HadronPhysicsQGSP_BERT_HP::~HadronPhysicsQGSP_BERT_HP() {}

void HadronPhysicsQGSP_BERT_HP::ConstructParticle()
{
  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();

  G4BaryonConstructor pBaryonConstructor;
  pBaryonConstructor.ConstructParticle();

  G4ShortLivedConstructor pShortLivedConstructor;
  pShortLivedConstructor.ConstructParticle();  
}

#include "G4ProcessManager.hh"

void HadronPhysicsQGSP_BERT_HP::ConstructProcess()
{
  theNeutrons.Build();
  thePro.Build();
  thePiK.Build();
  theMiscLHEP.Build();
  theStoppingHadron.Build();
}
// 2002 by J.P. Wellisch
