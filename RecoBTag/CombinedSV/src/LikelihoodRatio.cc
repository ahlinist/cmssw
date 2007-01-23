#include "RecoBTag/CombinedSV/interface/LikelihoodRatio.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/BTauReco/interface/TaggingVariable.h"
#include <cmath>

using namespace std;

namespace {
   typedef std::numeric_limits<long double> numl;

   double getPt ( const reco::TaggingVariableList & s )
   {
     for ( reco::TaggingVariableList::const_iterator i=s.begin(); i!=s.end() ; ++i )
     {
       if ( i->first == reco::btag::jetPt ) return i->second;
     }
     edm::LogError("LikelihoodRatio") << "jetpt not defined in tagging variables collection"
                                      << endl;
     return -1.;
   }

   double getEta ( const reco::TaggingVariableList & s )
   {
     for ( reco::TaggingVariableList::const_iterator i=s.begin(); i!=s.end() ; ++i )
     {
       if ( i->first == reco::btag::jetEta ) return i->second;
     }
     edm::LogError("LikelihoodRatio") << "jeteta not defined in tagging variables collection"
                                      << endl;
     return -1.;
   }

   bool printTaggingVariables()
   {
     return true;
   }
}

LikelihoodRatio::~LikelihoodRatio()
{
  delete bTagCalibration_;
}

LikelihoodRatio::LikelihoodRatio ( const BTagAlgorithmCalibration * calib, 
    reco::btag::Vertices::VertexType tp, double p ) :
  bTagCalibration_ ( calib ), priorCharmInBG_ ( p ), priorUDSG_ ( 1. - p ),
  vtxtype_ ( tp )
{}

void LikelihoodRatio::setCharmPrior( double charmPrior )
{
  if (charmPrior >= 0.0 && charmPrior <= 1.0) {
    priorCharmInBG_ = charmPrior;
  } else {
    edm::LogError ( "LikelihoodRatio") << "charm prior in invalid range!";
    // message: invalid range, use default
    // or throw exception?
    priorCharmInBG_ = 0.25;
  } // if
  priorUDSG_  = 1.0 - priorCharmInBG_;
}

double LikelihoodRatio::compute ( const reco::TaggingVariableList & s ) const
{
  double jetpt=getPt ( s );
  double jeteta= getEta ( s );

  edm::LogInfo ( "LikelihoodRatio") << "now perform computation for "
       << reco::btag::Vertices::name(vtxtype_) << "(" << vtxtype_ << "), pt="
       << jetpt << ", eta=" << jeteta;
  // iterate over vector of variable pairs to construct likelihoods,
  // combine to discriminator value
  long double bLikelihood    = 1.0;
  long double cLikelihood    = 1.0;
  long double udsgLikelihood = 1.0;

  LogDebug ("") << s.size() << " variables.";

  for ( reco::TaggingVariableList::const_iterator i = s.begin(); i != s.end(); i++ )
  {
    reco::btag::TaggingVariableName taggingVar = i->first;
    if ( taggingVar == reco::btag::jetPt || taggingVar == reco::btag::jetEta )
    {
      LogDebug("") << "jetpt and jeteta are ignored for the moment." << endl;
      continue;
    }
    long double variableValue = i->second;
    if ( !(isfinite ( variableValue )) )
    {
      edm::LogWarning ( "LikelihoodRatio" ) << "grummel: value for "
           << reco::TaggingVariableTokens [ taggingVar ] << "=" << variableValue
           << " ... skipping!";
      continue;
    }

    combsv::CombinedSVCalibInput binfo ( vtxtype_, combsv::Partons::B, taggingVar, jetpt, jeteta );
    combsv::CombinedSVCalibInput cinfo ( vtxtype_, combsv::Partons::C, taggingVar, jetpt, jeteta );
    combsv::CombinedSVCalibInput uinfo ( vtxtype_, combsv::Partons::UDSG, taggingVar, jetpt, jeteta );

    long double b = getPDFValue( variableValue, binfo );
    long double c = getPDFValue( variableValue, cinfo );
    long double u = getPDFValue( variableValue, uinfo );

    if ( fabs(b) < numl::min() && fabs(c) < numl::min() && fabs(u) < numl::min() )
    {
      edm::LogWarning ( "LikelihoodRatio") << "warning: all likelihoods at 0 for "
           << reco::TaggingVariableTokens [ taggingVar ] << " (" << taggingVar << ") value="
           << variableValue << "." << " Ignoring this variable.";
      b=1.;c=1.;u=1.;
    }

    if ( printTaggingVariables() )
    {
      LogDebug("") << reco::TaggingVariableTokens [ taggingVar ] << " v=" << variableValue
           << ": " << b << ", " << c << ", " << u;
    }

    if ( fabs(b) < numl::min() )
    {
      cout << "[LikelihoodRatio] warning: probability of zero for B ("
           << combsv::Partons::B << "), "
           << " vertex type " << reco::btag::Vertices::name (vtxtype_) << "("
           << vtxtype_ << "), " << reco::TaggingVariableTokens [ taggingVar ] << " ("
           << taggingVar << ") value=" << variableValue << "?" << endl;
    }

    bLikelihood    *= b;
    cLikelihood    *= c;
    udsgLikelihood *= u;

  } //for varIter

  LogDebug ("LikelihoodRatio") << "b=" << bLikelihood << ", c=" << cLikelihood
       << ", udsg=" << udsgLikelihood;

  long double ratioC    = bLikelihood / (bLikelihood + cLikelihood);
  if ( !finite ( ratioC ) ) ratioC=0.;

  long double ratioUDSG = bLikelihood / (bLikelihood + udsgLikelihood);
  if ( !finite ( ratioUDSG ) ) ratioUDSG=0.;

  long double combinedVariable = priorCharmInBG_ * ratioC + priorUDSG_ * ratioUDSG;

  edm::LogInfo("LikelihoodRatio") << "tag ends, return " << combinedVariable
       << " (ratioC=" << ratioC << ", ratioUDSG=" << ratioUDSG << ", priorCharm="
       << priorCharmInBG_ << ", priorUDSG=" << priorUDSG_ << ")";

  return combinedVariable;
}

long double LikelihoodRatio::getPDFValue( double variableValue,
    const combsv::CombinedSVCalibInput & input ) const
{
  long double returnValue = 1.0; // values are multiplied, hence 1.0 doesn't change things

  // protect against not filled variables
  if ( isnan ( variableValue ) )
  {
    cout << "[LikelihoodRatio] ::getPDFValue warning "
         << " variable not filled: value=" << variableValue;
             /*
             << "tagging variable "
             << reco::btag::TaggingVariableName ( taggingVar )
             << " parton type " << combsv::Partons::name ( partonType )
             */
    return returnValue;
  } // if variableValue

  // fetch corresponding histogram from calibration
  // this does not seem to work properly yet
  const CalibratedHistogramXML *pdfHisto = bTagCalibration_->getCalibData( input );

  if (pdfHisto) {
    returnValue = pdfHisto->value(variableValue);
  } else {
    edm::LogError ( "LikelihoodRatio") << "Error: no pdfHisto for: " << input.describe();
    return 1.0;
  }

  if (!(isfinite(returnValue)))
  {
    cout << "[LikelihoodRatio] warning getPDFValue has non-finite value!" << endl;
    returnValue=1.;
  }
  return returnValue;
}

const BTagAlgorithmCalibration & LikelihoodRatio::calibration() const
{
  return *( bTagCalibration_ );
}
