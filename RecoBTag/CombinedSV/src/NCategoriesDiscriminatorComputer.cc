#include "RecoBTag/CombinedSV/interface/NCategoriesDiscriminatorComputer.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

NCategoriesDiscriminatorComputer::NCategoriesDiscriminatorComputer (
    const AbstractDiscriminatorComputer * reco,
    const AbstractDiscriminatorComputer * pseudo,
    const AbstractDiscriminatorComputer * no ) :
  reco_(reco), pseudo_(pseudo), no_(no)
{}


NCategoriesDiscriminatorComputer::~NCategoriesDiscriminatorComputer()
{
  delete reco_;
  delete pseudo_;
  delete no_;
}

double NCategoriesDiscriminatorComputer::compute ( const reco::TaggingVariableList & s, 
    reco::btag::Vertices::VertexType t ) const
{
  using namespace reco::btag::Vertices;
  switch (t)
  {
    case ( RecoVertex ):
      return reco_->compute ( s );
    case ( PseudoVertex ):
      return pseudo_->compute ( s );
    case ( NoVertex ):
      return no_->compute ( s );
    case ( UndefVertex ):
    default:
      break;
  }

  edm::LogError("NCategoriesDiscriminatorComputer")
    << "unkown vertex type.";
  return -1.;
}
