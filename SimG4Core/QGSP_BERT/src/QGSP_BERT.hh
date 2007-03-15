#ifndef SimG4Core_QGSP_BERT_H
#define SimG4Core_QGSP_BERT_H
 
#ifndef G4V7
 
#include "SimG4Core/Physics/interface/PhysicsList.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
 
class QGSP_BERT: public PhysicsList
{
public:
    QGSP_BERT(const edm::ParameterSet & p);
};
 
#endif
#endif


