#ifndef SimG4Core_QGSP_BERT_HP_H
#define SimG4Core_QGSP_BERT_HP_H 1

#include "SimG4Core/Physics/interface/PhysicsList.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class QGSP_BERT_HP: public PhysicsList
{
public:
  QGSP_BERT_HP(G4LogicalVolumeToDDLogicalPartMap& map, const edm::ParameterSet & p);
};

#endif

