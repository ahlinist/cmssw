#ifndef RecoBTag_AbstractDiscriminatorComputer_h
#define RecoBTag_AbstractDiscriminatorComputer_h

#include "DataFormats/BTauReco/interface/TaggingVariable.h"

class AbstractDiscriminatorComputer {
public:

  virtual double compute ( const reco::TaggingVariableList & ) const = 0;
  virtual ~AbstractDiscriminatorComputer() {};
};

#endif
