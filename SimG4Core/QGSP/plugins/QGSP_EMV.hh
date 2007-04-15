#ifndef SimG4Core_QGSP_EMV_H
#define SimG4Core_QGSP_EMV_H

#ifndef G4V7

#include "SimG4Core/Physics/interface/PhysicsList.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class QGSP_EMV: public PhysicsList
{
public:
    QGSP_EMV(const edm::ParameterSet & p);
};

#endif
#endif

