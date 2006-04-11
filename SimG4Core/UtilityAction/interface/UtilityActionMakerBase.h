#ifndef SimG4Core_UtilityAction_UtilityActionMakerBase_h
#define SimG4Core_UtilityAction_UtilityActionMakerBase_h

#include "SimG4Core/UtilityAction/interface/UtilityAction.h"

#include <memory>

class SimActivityRegistry;
namespace edm { class ParameterSet; }

class UtilityActionMakerBase
{
public:
    UtilityActionMakerBase(){}
    virtual ~UtilityActionMakerBase(){}
    virtual std::auto_ptr<UtilityAction> make(const edm::ParameterSet&,
					      SimActivityRegistry&) const = 0;
};
            
#endif
