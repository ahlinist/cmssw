/*******************
A QuaeroSysEvent is an event with reconstructed final state objects
that can change under systematic shifts.
Bruce Knuteson   Feb 2004
*******************/

#ifndef __QuaeroSysEvent__
#define __QuaeroSysEvent__

#include "Validation/VstQuaeroUtils/interface/QuaeroRecoEvent.hh"
#include "Validation/VstQuaero/interface/QuaeroSysObject.hh"

class QuaeroSysEvent: public QuaeroRecoEvent<QuaeroSysObject>
{
public:
  QuaeroRecoEvent<QuaeroRecoObject> returnQuaeroEventWithoutSystematics() const;
protected:
  virtual double parseSystematicallyAffectedNumber(std::string blah);
};


// bktemp WARNING!  Currently the weight of the event does NOT get shifted. Apr 3 2004

#endif
