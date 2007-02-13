#include "RecoBTag/CombinedSVLearning/interface/LikelihoodTeacher.h"
#include "RecoBTag/CombinedSV/interface/PartonTypes.h"
#include "DataFormats/BTauReco/interface/VertexTypes.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

using namespace std;

namespace {
  combsv::CombinedSVCalibInput createCalibInput ( const reco::TaggingVariableList & l, char tag )
  {
    combsv::Partons::PartonType ptype = combsv::Partons::type ( tag ); 
    reco::btag::Vertices::VertexType vtype = reco::btag::Vertices::UndefVertex;
    double jetpt=0.;
    double jeteta=0.;
    using namespace reco::btag;
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
}

LikelihoodTeacher::LikelihoodTeacher ( BTagAlgorithmCalibration * calib,
    reco::btag::Vertices::VertexType t ) :
  calib_(calib), type_ ( t )
{}

void LikelihoodTeacher::teach ( const reco::TaggingVariableList & l, char tag )
{
  /*
  vector < pair < combsv::CombinedSVCalibrationCategory *,
                  combsv::CombinedSVCalibrationHistogram * > > cats = calib_->dumpCalibration();
  for ( vector< pair < combsv::CombinedSVCalibrationCategory *,
                     combsv::CombinedSVCalibrationHistogram * > >::const_iterator i=cats.begin(); 
        i!=cats.end() ; ++i )
  {
    if ( i->second->entries() > 0 )
      LogDebug("") << "Teaching! Entries: " << i->second->entries();
  }
  */

  combsv::CombinedSVCalibInput ip = createCalibInput ( l, tag );
  using namespace reco::btag;

  for ( reco::TaggingVariableList::const_iterator i=l.begin(); i!=l.end() ; ++i )
  {
    switch ( i->first )
    {
      case jetPt:
      case jetEta:
      case vertexCategory:
        // already retrieved these
        break;
      default:
      {
        ip.setTaggingVariable ( i->first );
        fill ( ip, i->second );
      }
    }
  }
}

void LikelihoodTeacher::teachCategory ( const reco::TaggingVariableList & l, char tag )
{
  // teach the category only!
  combsv::CombinedSVCalibInput ip = createCalibInput ( l, tag );
  using namespace reco::btag;
  ip.setTaggingVariable ( vertexCategory );
  ip.setVertexType ( type_ );

  for ( reco::TaggingVariableList::const_iterator i=l.begin(); i!=l.end() ; ++i )
  {
    if ( i->first == vertexCategory )
    {
      fill ( ip, i->second );
    }
  }
}

void LikelihoodTeacher::fill ( const combsv::CombinedSVCalibInput & ip, double value )
{
  combsv::CombinedSVCalibrationHistogram * histo =
    const_cast < combsv::CombinedSVCalibrationHistogram * > (calib_->getCalibData ( ip ));
  if ( histo )
  {
    // histo->fill ( value, 1. );
    // histo->addEntries ( 1 );
    histo->addCount(value);
  } else {
    edm::LogError("LikelihoodTeacher") << reco::btag::Vertices::name ( type_ )
      << ": Calibration category does not exist: " << ip;
    // exit(-1);
  }
}

const BTagAlgorithmCalibration * LikelihoodTeacher::calibration() const
{
  return calib_;
}

