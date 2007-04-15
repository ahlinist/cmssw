#ifndef SimG4Core_QGSP_G4v52_H
#define SimG4Core_QGSP_G4v52_H
 
#ifndef G4V7
 
#include "SimG4Core/Physics/interface/PhysicsList.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
 
class QGSP_G4v52: public PhysicsList
{
public:
    QGSP_G4v52(const edm::ParameterSet & p);
};
 
#endif
#endif


