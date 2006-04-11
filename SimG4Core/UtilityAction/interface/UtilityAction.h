#ifndef SimG4Core_UtilityAction_H
#define SimG4Core_UtilityAction_H
 
#include "FWCore/ParameterSet/interface/ParameterSet.h"
 
#include <boost/cstdint.hpp>
 
class UtilityAction
{
public:
    explicit UtilityAction(edm::ParameterSet const & p) 
	: m_pUtilityAction(p) {}
    virtual ~UtilityAction() {}
private:
    edm::ParameterSet m_pUtilityAction;
};
 
#endif
