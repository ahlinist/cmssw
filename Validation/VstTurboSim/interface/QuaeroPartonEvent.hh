#include <vector>
#include <string>
#include <fstream>
#include "Validation/VstQuaeroUtils/interface/QuaeroRecoEvent.hh"
#include "Validation/VstTurboSim/interface/QuaeroPartonObject.hh"

#ifndef __QuaeroPartonEvent__
#define __QuaeroPartonEvent__

//typedef QuaeroRecoEvent<QuaeroPartonObject> QuaeroPartonEvent;

class QuaeroPartonEvent : public QuaeroRecoEvent<QuaeroPartonObject>
{
  /// Support streaming output
  friend std::istream &operator>>(std::istream&, QuaeroPartonEvent &);
  /// Support streaming input
  friend std::ostream &operator<<(std::ostream&, const QuaeroPartonEvent &);
public:
  QuaeroPartonEvent(std::string _eventType="", 
		    std::string _runNumber="0", 
		    double _wt=0, 
		    const std::vector<QuaeroPartonObject>& _objects = std::vector<QuaeroPartonObject>(0), 
		    std::string _collisionType="",
 		    double _rootS=0, 
		    double _zVtx=0, 
		    double _generatedSumPt=0,
		    std::string _fullString="");

  //mrenna 
//  virtual ~QuaeroPartonEvent() {}

};

#endif
