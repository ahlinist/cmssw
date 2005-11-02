#ifndef SimG4Core_QGSC_H
#define SimG4Core_QGSC_H

#include "SimG4Core/Physics/interface/PhysicsList.h"
#include "SealKernel/Component.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class QGSC: public PhysicsList
{
    DECLARE_SEAL_COMPONENT;
public:
    QGSC(seal::Context * c, const edm::ParameterSet & p);
    virtual ~QGSC();
};

#endif

