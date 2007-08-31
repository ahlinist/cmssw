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

#include "DataFormats/GeometryCommonDetAlgo/interface/Measurement1D.h"
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
      const map <combsv::CombinedTrack, float > & tmp = vtx->weightedTracks();
      for ( map < combsv::CombinedTrack, float >::const_iterator trk = tmp.begin();
            trk != tmp.end(); trk++ )
      {
        // a track is secondary if and only if w>0.5!
        if ( trk->second > 0.5 ) ret.push_back( trk->first );
      }
    }
    return ret;
  }
}

void combsv::CombinedSVAlgorithm::setMagneticField ( const MagneticField * field )
{
  magneticField_ = field;
}

void combsv::CombinedSVAlgorithm::setTransientTrackBuilder ( const TransientTrackBuilder * b )
{
  filters_.vertexFilter().setTransientTrackBuilder ( b );
}

combsv::CombinedSVAlgorithm::CombinedSVAlgorithm( const MagneticField * field,
    const VertexReconstructor & v, const NCategoriesDiscriminatorComputer * c,
    const CombinedSVTaggingVariables & vars, const FilterFactory & ff,
    double vcc, double tismm ) :
  filters_ ( ff ), btagRector_ ( BTagVertexReconstructor ( v, ff.trackFilter() ) ), magneticField_ ( field ), 
  discriminatorComputer_ ( c ), variables_ ( vars ),
  vertexCharmCut_(vcc), trackIpSignificanceMin2DMin_(tismm) ,
  trackInfoBuilder_ ( TrackInfoBuilder () ),
  vtxBuilder_ ( PseudoVertexBuilder (tismm, ff.trackFilter(), &trackInfoBuilder_ ) )
{}

combsv::CombinedSVAlgorithm::~CombinedSVAlgorithm()
{
  delete discriminatorComputer_;
}

void combsv::CombinedSVAlgorithm::adjust ( const reco::Vertex & primVertex,
    const reco::Particle & jet )
{
  filters_.vertexFilter().setPrimary ( primVertex );

  GlobalVector jetdir ( jet.px(), jet.py(), jet.pz() );
  // Setting the track info builder also affects 
  // the track info builder
  trackInfoBuilder_.setJet ( primVertex, jetdir );
}

reco::CombinedSVTagInfo combsv::CombinedSVAlgorithm::tag ( const reco::Vertex & primVertex,
                                    const reco::Particle & jet,
                                    const vector < reco::TransientTrack > & itracks,
                                    const JetTracksAssociationRef & jtaRef )
{
  try  {
    LogDebug("") << "Tagging with pv=" << primVertex.position() << ", jet="
                 << jet.px() << "," << jet.py() << "," << jet.pz()
                 << " and " << itracks.size() << " tracks.";
    /**
     * This is the main tagging routine for the combined b-tagging
     * algorithm
     */

    if ( ! ( filters_.jetFilter() ( jet ) ) )
    {
      // did not pass the jet filter
      reco::TaggingVariableList x;
      return reco::CombinedSVTagInfo ( x, -1., jtaRef );
    }

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
      if ( filters_.trackFilter() ( t, false ) )
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

    /*
    if ( vertexType != reco::btag::Vertices::RecoVertex )
    {
      // the filter is different now, so redo the tracks
      etracks=getSecondaryTracks ( vtces );
      tracks.clear();
      for ( vector< combsv::CombinedTrack >::const_iterator i=etracks.begin(); i!=etracks.end() ; ++i )
      { 
        tracks.push_back ( *i );
      }
    }

    LogDebug("") << "we have " << vtces.size() << " vertices of type "
                 << reco::btag::Vertices::name ( vertexType ) << " and a total of "
                 << etracks.size() << " secondary tracks.";
                 */

    /* edm::LogError("CombinedSVAlgorithm" ) <<
      "createJetInfo different API. Compute both hard-assigned and soft-assigned energies!";
      */
    combsv::CombinedJet cjet = createJetInfo ( etracks, vtces, vertexType );

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
    return reco::CombinedSVTagInfo ( singleVarVector, d, jtaRef );
  } catch ( ... ) {
    edm::LogWarning("CombinedSVAlgorithm" ) << "Unknown exception! Will return -1 tag!";
    reco::TaggingVariableList x;
    return reco::CombinedSVTagInfo ( x, -1., jtaRef );
  }
}

double combsv::CombinedSVAlgorithm::minFlightDistanceSignificance2D ( 
    reco::btag::Vertices::VertexType vertexType,
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
    Measurement1D m2d = vertexDistance2DCalculator.compatibility (
                      filters_.constVertexFilter().primaryVertex(), *v );

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

  reco::BKinematics tmpKinematics;

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
    const vector < combsv::CombinedVertex > & vtces,
    reco::btag::Vertices::VertexType vertexType )
{
  reco::BKinematics allTrackKinematics( alltracks );

  // get vector given by sum of all tracks in jet
  // GlobalVector pAll = allTrackKinematics.get3Vector();

  // create instance of small helper class used to determine
  // kinematic properties based on a selection of tracks
  int vertexMult = 0;
  reco::BKinematics bTrackKinematics;
  for ( vector< combsv::CombinedVertex >::const_iterator i=vtces.begin(); 
        i!=vtces.end() ; ++i )
  {
    bTrackKinematics.add ( *i );
    vertexMult+=i->bTagTracks().size();
  }
  // reco::BKinematics bTrackKinematics ( secondaries );
  // int vertexMult = secondaries.size();

  // compute vertex related variables
  // (based on all tracks at all secondary vertices)
  double mass = bTrackKinematics.getMass();
  double energyBTracks   = bTrackKinematics.getEnergy();
  double energyWBTracks   = bTrackKinematics.getWeightedEnergy();
  // GlobalVector pB = bTrackKinematics.get3Vector();

  double energyAllTracks = allTrackKinematics.getEnergy();
  double energyWAllTracks = allTrackKinematics.getWeightedEnergy();

  double fracEnergy = 0.;
  if (energyBTracks > 0. && energyAllTracks > 0. ) {
    fracEnergy = energyBTracks/energyAllTracks;
  }

  double fracWEnergy = 0.;
  if (energyWBTracks > 0. && energyWAllTracks > 0. ) {
    fracWEnergy = energyWBTracks/energyWAllTracks;
  }


  double minFDSig2D = minFlightDistanceSignificance2D ( vertexType, vtces );
  double first2DSignedIPSigniAboveCut = computeFirstTrackAboveCharmMass( alltracks );
  return CombinedJet( mass, vertexMult, fracEnergy,
                          minFDSig2D, first2DSignedIPSigniAboveCut,
                          vtces.size(), fracWEnergy );
}

const CombinedSVTaggingVariables & combsv::CombinedSVAlgorithm::variables() const
{
  return variables_;
}

const MagneticField * combsv::CombinedSVAlgorithm::magneticField() const
{
  return magneticField_;
}
