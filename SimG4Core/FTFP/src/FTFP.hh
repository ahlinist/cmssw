#ifndef SimG4Core_FTFP_H
#define SimG4Core_FTFP_H

#include "SimG4Core/Physics/interface/PhysicsList.h"
#include "SealKernel/Component.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class FTFP: public PhysicsList
{
    DECLARE_SEAL_COMPONENT;
public:
    FTFP(seal::Context * c, const edm::ParameterSet & p);
    virtual ~FTFP();
};

#endif



