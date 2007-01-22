#ifndef RecoBTag_NCategoriesDiscriminatorComputer_h
#define RecoBTag_NCategoriesDiscriminatorComputer_h

#include "RecoBTag/CombinedSV/interface/AbstractDiscriminatorComputer.h"

class NCategoriesDiscriminatorComputer {
  /** 
   *  A discriminator computer that knows of the concept
   *  of the three vertex categories.
   *
   *  We own ADCs!
   */
public:
  NCategoriesDiscriminatorComputer ( 
       const AbstractDiscriminatorComputer * reco,
       const AbstractDiscriminatorComputer * pseudo,
       const AbstractDiscriminatorComputer * no );

  ~NCategoriesDiscriminatorComputer();

  double compute ( const reco::TaggingVariableList &,
                         reco::btag::Vertices::VertexType t ) const;

private:
  const AbstractDiscriminatorComputer * reco_;
  const AbstractDiscriminatorComputer * pseudo_;
  const AbstractDiscriminatorComputer * no_;
};

#endif
