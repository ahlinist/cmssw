#ifndef SimG4Core_UtilActions_UtilityActionMaker_h
#define SimG4Core_UtilActions_UtilityActionMaker_h

#include "SimG4Core/UtilityAction/interface/UtilityActionMakerBase.h"
#include "SimG4Core/Notification/interface/SimActivityRegistryEnroller.h"
 
#include <memory>
 
template<class T>
class UtilityActionMaker : public UtilityActionMakerBase
{
public:
    UtilityActionMaker(){}
    virtual std::auto_ptr<UtilityAction> make(const edm::ParameterSet& p,
					      SimActivityRegistry& reg) const
    {
	std::auto_ptr<T> returnValue(new T(p));
	SimActivityRegistryEnroller::enroll(reg, returnValue.get());
	return std::auto_ptr<UtilityAction>(returnValue);
    }
};

#endif
