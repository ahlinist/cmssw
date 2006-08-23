#include "SimG4Core/Physics81/interface/PhysicsList.h"
#include "SimG4Core/Physics81/interface/DDG4ProductionCuts.h"

#include "G4LossTableManager.hh"

PhysicsList::PhysicsList(const edm::ParameterSet & p) 
  : G4VModularPhysicsList(), m_pPhysics(p), prodCuts(0)
{}
 
PhysicsList::~PhysicsList() 
{ 
    if (m_pPhysics.getUntrackedParameter<int>("Verbosity") > 1)
	std::cout << " G4BremsstrahlungThreshold was " 
		  << G4LossTableManager::Instance()->BremsstrahlungTh()/GeV 
		  << " GeV " << std::endl;
    if (prodCuts!=0) delete prodCuts; 
}

void PhysicsList::SetCuts() 
{ 
    SetDefaultCutValue(m_pPhysics.getParameter<double>("DefaultCutValue")*cm);
    SetCutsWithDefault();
    
    G4LossTableManager::Instance()->SetBremsstrahlungTh
	(m_pPhysics.getParameter<double>("G4BremsstrahlungThreshold")*GeV);
    
    if (m_pPhysics.getParameter<bool>("CutsPerRegion"))
    {
	std::cout << " Setting Production Cuts per Region" << std::endl;
	DDG4ProductionCuts prodCuts;
	prodCuts.SetVerbosity(m_pPhysics.getUntrackedParameter<int>("Verbosity",0));
	prodCuts.update();
    }

    int v = m_pPhysics.getUntrackedParameter<int>("Verbosity");
    if (v > 1)
    {
	G4LossTableManager::Instance()->SetVerbose(v-1);
	G4VUserPhysicsList::DumpCutValuesTable();
    }
}

