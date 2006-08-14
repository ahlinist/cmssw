#ifndef SimG4Core_QGSP_H
#define SimG4Core_QGSP_H

#include "SimG4Core/Physics81/interface/PhysicsList.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class QGSP: public PhysicsList
{
public:
    QGSP(const edm::ParameterSet & p);
};

#endif

