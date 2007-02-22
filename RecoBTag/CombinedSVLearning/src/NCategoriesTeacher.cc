#include "RecoBTag/CombinedSVLearning/interface/NCategoriesTeacher.h"
#include "RecoBTag/CombinedSV/interface/PartonTypes.h"
#include "DataFormats/BTauReco/interface/VertexTypes.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "RecoBTag/CombinedSV/interface/CombinedSVCalibInput.h"

namespace {
  combsv::CombinedSVCalibInput createCalibInput ( const reco::TaggingVariableList & l, char tag )
  {
    combsv::Partons::PartonType ptype = combsv::Partons::type ( tag ); 
    reco::btag::Vertices::VertexType vtype = reco::btag::Vertices::UndefVertex;
    double jetpt=0.;
    double jeteta=0.;
    using namespace reco::btau;
    for ( reco::TaggingVariableList::const_iterator i=l.begin(); i!=l.end() ; ++i )
    {
      switch ( i->first )
      {
        case vertexCategory:
          vtype=(reco::btag::Vertices::VertexType) (i->second);
          break;
        case jetPt:
          jetpt=i->second;
          break;
        case jetEta:
          jeteta=i->second;
          break;
        default:
          break;
      }
    }

    return combsv::CombinedSVCalibInput ( vtype, ptype, vertexCategory, jetpt, jeteta );
  }

  void normalize ( CalibratedHistogramXML & histo )
  {
    double tot=0.;
    for ( vector< float >::const_iterator i=histo.upperLimits().begin(); i!=histo.upperLimits().end() ; ++i )
    {
      tot+=(*i);
    }
    edm::LogError("LikelihoodTeacher") << "need to normalize " << tot;
  }
}

NCategoriesTeacher::NCategoriesTeacher ( BTagAlgorithmCalibration * reco,
    BTagAlgorithmCalibration * pseudo,
    BTagAlgorithmCalibration * no ) :
  reco_ ( LikelihoodTeacher ( reco, reco::btag::Vertices::RecoVertex ) ),
  pseudo_ ( LikelihoodTeacher ( pseudo, reco::btag::Vertices::PseudoVertex ) ),
  no_ ( LikelihoodTeacher ( no, reco::btag::Vertices::NoVertex ) )
{}

void NCategoriesTeacher::teach ( const reco::TaggingVariableList & l, char tag )
{
  combsv::CombinedSVCalibInput ip = createCalibInput ( l, tag );
  edm::LogInfo("NCategoriesTeacher") << "teaching " << ip;

  // the category is taught for every vertex type
  reco_.teachCategory ( l, tag );
  pseudo_.teachCategory ( l, tag );
  no_.teachCategory ( l, tag );

  switch ( ip.vertexType() )
  {
    case reco::btag::Vertices::RecoVertex:
    {
      reco_.teach ( l, tag );
      break;
    }
    case reco::btag::Vertices::PseudoVertex:
    {
      pseudo_.teach ( l, tag );
      break;
    }
    case reco::btag::Vertices::NoVertex:
    {
      no_.teach ( l, tag );
      break;
    }
    default:
      edm::LogError("NCategoriesTeacher") << "teaching an unknown vertex category: "
                << reco::btag::Vertices::name ( ip.vertexType() );
      return;
  }
}

const BTagAlgorithmCalibration * NCategoriesTeacher::recoCalibration() const
{
  return reco_.calibration();
}

const BTagAlgorithmCalibration * NCategoriesTeacher::pseudoCalibration() const
{
  return pseudo_.calibration();
}

const BTagAlgorithmCalibration * NCategoriesTeacher::noCalibration() const
{
  return no_.calibration();
}
