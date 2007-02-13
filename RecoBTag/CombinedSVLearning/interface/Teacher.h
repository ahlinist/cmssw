#ifndef RecoBTag_Teacher
#define RecoBTag_Teacher

#include "DataFormats/BTauReco/interface/TaggingVariable.h"

class Teacher {
   public:
     /**
      *  Abstract base class that defines how to teach
      *  a btagging algorithm
      */

     virtual void teach ( const reco::TaggingVariableList & l, char tag ) = 0;
     virtual ~Teacher() {};
};

#endif
