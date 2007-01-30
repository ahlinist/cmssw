#include "RecoBTag/CombinedSV/interface/CombinedSVAlgorithm.h"
#include <set>
#include <limits>
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"
// magnetic field
#include "MagneticField/Engine/interface/MagneticField.h"
// track related
// #include "DataFormats/TrackReco/interface/HitPattern.h"

// lifetime-signed impact parameter
#include "RecoBTag/BTagTools/interface/SignedTransverseImpactParameter.h"
#include "RecoBTag/BTagTools/interface/SignedImpactParameter3D.h"

// vertex related
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include "RecoVertex/VertexTools/interface/VertexDistanceXY.h"
// #include "RecoVertex/VertexTools/interface/VertexDistance3D.h"
#include "RecoVertex/AdaptiveVertexFit/interface/AdaptiveVertexFitter.h"

#include "Geometry/CommonDetAlgo/interface/Measurement1D.h"
#include "CLHEP/Vector/LorentzVector.h"
#include "RecoBTag/CombinedSV/interface/BKinematics.h"
#include "RecoBTag/CombinedSV/interface/CombinedJet.h"
#include "RecoBTag/CombinedSV/interface/TTracksFromRecoVertex.h"
#include "RecoBTag/CombinedSV/interface/CombinedData.h"
#include "RecoBTag/CombinedSV/interface/TaggingVariablesComputer.h"

using namespace std;
using namespace reco;

namespace {
   typedef numeric_limits<double> num;
   typedef numeric_limits<long double> numl;

  double computePBLong ( const GlobalVector & pAll,
      const GlobalVector & pB )
  {
    double ret = 0.;
    if ( pAll.mag() > 0 ) ret = (pAll*pB)/pAll.mag();
    return ret;
  }

  double computePBTrans ( double pBmag,
      double pBLong, double pAllmag )
  {
    double ret = 0.;
    if (pBLong < pAllmag) ret = sqrt( pBmag * pBmag - pBLong*pBLong);
    return ret;
  }

  reco::Particle sum ( const vector < reco::TransientTrack > & t )
  {
    double px=0.;
    double py=0.;
    double pz=0.;
    double E=0.;
    reco::Particle::Charge charge=0;
    for ( vector< TransientTrack >::const_iterator i=t.begin(); i!=t.end() ; ++i )
    {
      px+=i->impactPointState().globalMomentum().x();
      py+=i->impactPointState().globalMomentum().y();
      pz+=i->impactPointState().globalMomentum().z();
      E+=0.; // all massless FIXME
    }
    return reco::Particle ( charge, reco::Particle::LorentzVector ( px, py, pz, E ) );
  }

  vector <combsv::CombinedTrack > getSecondaryTracks(
      const vector<combsv::CombinedVertex> & vtces )
  {
    vector<combsv::CombinedTrack> ret;

    for ( vector<combsv::CombinedVertex>::const_iterator vtx = vtces.begin();
          vtx != vtces.end(); vtx++ )
    {
      const vector<combsv::CombinedTrack> & tmp = vtx->bTagTracks();
      for ( vector < combsv::CombinedTrack >::const_iterator trk = tmp.begin();
            trk != tmp.end(); trk++ )
      {
        ret.push_back( *trk );
      }
    }
    return ret;
  }
}

void combsv::CombinedSVAlgorithm::setMagneticField ( const MagneticField * field )
{
  magneticField_ = field;
  filters_.vertexFilter().setMagneticField ( field );
}

combsv::CombinedSVAlgorithm::CombinedSVAlgorithm( const MagneticField * field,
    const VertexReconstructor & v, const NCategoriesDiscriminatorComputer * c,
    const CombinedSVTaggingVariables & vars, const FilterFactory & ff,
    double vcc, double tismm ) :
  filters_ ( ff ), btagRector_ ( BTagVertexReconstructor ( v, ff.trackFilter() ) ), magneticField_ ( field ), 
  discriminatorComputer_ ( c ), variables_ ( vars ),
  vertexCharmCut_(vcc), trackIpSignificanceMin2DMin_(tismm) ,
  trackInfoBuilder_ ( TrackInfoBuilder ( field ) ),
  vtxBuilder_ ( PseudoVertexBuilder (tismm, ff.trackFilter(), &trackInfoBuilder_, field ) )
{}

combsv::CombinedSVAlgorithm::~CombinedSVAlgorithm()
{
  delete discriminatorComputer_;
}

reco::CombinedSVTagInfo combsv::CombinedSVAlgorithm::tag ( const vector < reco::TransientTrack > & tracks )
{
  if ( tracks.size() < 2 )
  {
    edm::LogInfo ("CombinedSVAlgorithm" )
      << "trying to tag with fewer than two tracks and no other info.";
    reco::TaggingVariableList x;
    return reco::CombinedSVTagInfo ( x, -1. );
  }

  edm::LogInfo ("CombinedSVAlgorithm") << "    --- *** --- ";
  edm::LogInfo ("CombinedSVAlgorithm") << "tagging w/ tracks only";

  try {
    AdaptiveVertexFitter fitter;
    TransientVertex pv = fitter.vertex ( tracks );
    edm::LogInfo ( "CombinedSVAlgorithm") << "primary is at " << pv.position();
    reco::Particle jet = sum ( tracks );
    return tag ( pv, jet, tracks );
  } catch ( ... ) {
    edm::LogInfo ("CombinedSVAlgorithm" )
      << "exception caught when trying to tag!";
    reco::TaggingVariableList x;
    return reco::CombinedSVTagInfo ( x, -1. );
  }
}

void combsv::CombinedSVAlgorithm::adjust ( const reco::Vertex & primVertex,
    const reco::Particle & jet )
{
  filters_.vertexFilter().setPrimary ( primVertex );

  GlobalVector jetdir ( jet.px(), jet.py(), jet.pz() );
  // Setting the track info builder also affects 
  // the pseudovertex builder
  trackInfoBuilder_.setJet ( primVertex, jetdir );
}

reco::CombinedSVTagInfo combsv::CombinedSVAlgorithm::tag ( const reco::Vertex & primVertex,
                                    const reco::Particle & jet,
                                    const vector < reco::TransientTrack > & itracks )
{
  if ( ! ( filters_.jetFilter() ( jet ) ) )
  {
    // did not pass the jet filter
    reco::TaggingVariableList x;
    return reco::CombinedSVTagInfo ( x, -1. );
  }

  /**
   * This is the main tagging routine for the combined b-tagging
   * algorithm
   */

  // adjust all builders and filters to the new jet
  adjust ( primVertex, jet );

  // filter tracks
  std::vector < CombinedTrack > etracks;
  std::vector < reco::TransientTrack > tracks;
  for ( vector< reco::TransientTrack >::const_iterator i=itracks.begin();
        i!=itracks.end() ; ++i )
  {
    combsv::CombinedTrack t = trackInfoBuilder_.build ( *i );
    // if ( filters_.trackFilter() ( t, trackIpSignificanceMin2DMin_ ) )
    if ( filters_.trackFilter() ( t ) )
    {
      etracks.push_back ( t );
      tracks.push_back ( *i );
    }
  }

  LogDebug("") << tracks.size() << " out of "
               << itracks.size() << " tracks survived track filter.";

  pair < reco::btag::Vertices::VertexType, vector < combsv::CombinedVertex > > svtces = 
    btagRector_.vertices ( tracks, etracks, filters_.vertexFilter(), vtxBuilder_ );

  reco::btag::Vertices::VertexType vertexType = svtces.first;
  const vector < combsv::CombinedVertex > & vtces = svtces.second;

  LogDebug("") << "we have " << vtces.size() << " vertices. VtxType: "
               << reco::btag::Vertices::name ( vertexType );

  combsv::CombinedJet cjet = createJetInfo ( etracks, getSecondaryTracks( vtces), vtces, vertexType );

  combsv::CombinedData data ( primVertex, jet, vtces, vertexType, 
      tracks, etracks, cjet, jet.pt(), jet.eta() );

  /* 
   * So now we can compute the tagging variables
  */
  reco::TaggingVariableList singleVarVector = TaggingVariablesComputer::compute 
       ( variables_, data );

  /*
   * finally compute the discriminator computer
   */
  double d = discriminatorComputer_->compute ( singleVarVector, vertexType );
  return reco::CombinedSVTagInfo ( singleVarVector, d );
}

double combsv::CombinedSVAlgorithm::minFlightDistanceSignificance2D ( 
    const GlobalVector & pAll, reco::btag::Vertices::VertexType vertexType,
    const vector < combsv::CombinedVertex > & vtces ) const
{
  if ( vertexType != reco::btag::Vertices::RecoVertex ) return -1.;
  /**
   * compute min B flight distance in 2D,
   * if multiple secondary vertices (case "RecoVertex"),
   */
 
  double ret = num::infinity();

  // create instance of helper classes
  VertexDistanceXY vertexDistance2DCalculator;

  for ( vector< combsv::CombinedVertex >::const_iterator v = vtces.begin();
        v != vtces.end(); v++ )
  {
    // 2D case
    Measurement1D m2d = vertexDistance2DCalculator.signedDistance(
                      filters_.constVertexFilter().primaryVertex(), *v, pAll);

    if ( m2d.value() < ret ) ret=m2d.value();
  }

  return ret;
}

double combsv::CombinedSVAlgorithm::computeFirstTrackAboveCharmMass(
    const std::vector<combsv::CombinedTrack> & etracks )
{
  /**
   * Determine (lifetime-singed 2D) impact parameter of first track
   * above given mass threshold (vertexCharmCut_)
   * Idea: if the secondary vertex is due to a charmed hadron,
   *       there will be a distinct gap in the distribution of the
   *       impact parameters:
   *       see e.g. http://www-ekp.physik.uni-karlsruhe.de/~weiser/thesis/P108.html
   */

  // define vector which holds all d0 - track associations, sorted by d0 significance
  // filled with all tracks in jet
  // -> revisit later once ip calculation,etc have moved to final
  //    location
  multimap< double , CombinedTrack , greater<double> > sortedTrackMapSignedD0Significance;

  // fill sorted multimap with 2D impact parameter
  for ( vector<combsv::CombinedTrack>::const_iterator i = etracks.begin();
        i != etracks.end(); i++)
  {
    sortedTrackMapSignedD0Significance.insert(make_pair(i->ip2D().significance(), *i ));
  }

  reco::BKinematics tmpKinematics( magneticField_ );

  // loop over tracks sorted by lifetime-signed impact parameter (2D)
  for (  multimap<double,CombinedTrack,greater<double> >::const_iterator trk =
         sortedTrackMapSignedD0Significance.begin(); trk !=
         sortedTrackMapSignedD0Significance.end(); trk++)
  {
    double ip2DSigni = trk->first;

    tmpKinematics.add ( trk->second );

    double mass = tmpKinematics.getMass();

    if (mass > vertexCharmCut_) {
      return ip2DSigni;
    }
  }
  return -1.;
}

combsv::CombinedJet combsv::CombinedSVAlgorithm::createJetInfo (
    const vector<combsv::CombinedTrack> & alltracks,
    const vector < combsv::CombinedTrack > & secondaries,
    const vector < combsv::CombinedVertex > & vtces,
    reco::btag::Vertices::VertexType vertexType )
{
  reco::BKinematics allTrackKinematics( magneticField_, alltracks );

  // get vector given by sum of all tracks in jet
  GlobalVector pAll = allTrackKinematics.get3Vector();

  // create instance of small helper class used to determine
  // kinematic properties based on a selection of tracks
  reco::BKinematics bTrackKinematics( magneticField_, secondaries );

  // compute vertex related variables
  // (based on all tracks at all secondary vertices)
  double mass = bTrackKinematics.getMass();
  double energyBTracks   = bTrackKinematics.getEnergy();
  // GlobalVector pB = bTrackKinematics.get3Vector();

  double energyAllTracks = allTrackKinematics.getEnergy();

  double fracEnergy = 0.;
  if (energyBTracks > 0. && energyAllTracks > 0. ) {
    fracEnergy = energyBTracks/energyAllTracks;
  }

  int vertexMult = secondaries.size();

  double minFDSig2D = minFlightDistanceSignificance2D ( pAll, vertexType, vtces );
  double first2DSignedIPSigniAboveCut = computeFirstTrackAboveCharmMass( alltracks );
  return CombinedJet( mass, vertexMult, fracEnergy,
                          minFDSig2D, first2DSignedIPSigniAboveCut );
}

const CombinedSVTaggingVariables & combsv::CombinedSVAlgorithm::variables() const
{
  return variables_;
}

const MagneticField * combsv::CombinedSVAlgorithm::magneticField() const
{
  return magneticField_;
}
