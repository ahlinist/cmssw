#include "RecoBTag/CombinedSV/interface/KernelEstimatedHistogram.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"
// #include "CommonTools/Statistics/interface/Accumulator.h"
#include <iostream>
#include <cmath>

using namespace std;

namespace {
}

void combsv::KernelEstimatedHistogram::copyTrueHistogram()
{
  for ( vector< float >::const_iterator i=values().begin(); 
        i!=values().end() ; ++i )
  {
    m_smoothed.push_back ( *i );
  }
}

combsv::KernelEstimatedHistogram::KernelEstimatedHistogram(
    const combsv::CombinedSVCalibrationHistogram & hist ) :
  combsv::CombinedSVCalibrationHistogram ( hist )
{
  if ( hist.constCategory().taggingVar() == reco::btau::vertexCategory )
  {
    copyTrueHistogram();
    return;
  }

  if ( numberOfBins() < 1 )
  {
    throw cms::Exception ( " numberOfBins() < 2" );
  }

  // FIXME only for now sqrt(2pi)=2.506628274631
  float avg= 0.;
  int c=0;
  float mtot=0.;
  float w = binRange(1).width();
  for ( vector< float >::const_iterator i=values().begin(); 
        i!=values().end() ; ++i )
  {
    m_smoothed.push_back ( 0. );
    float add = (*i) * ( upperLimits()[c] + 0.5 * w );
    if (finite( add ) )
    {
      avg+= add;
    }
    mtot+=(*i);
    c++;
  }
  if ( mtot >0. ) avg/= mtot;

  float var = 0.;
  c=0;
  for ( vector< float >::const_iterator i=values().begin(); 
        i!=values().end() ; ++i )
  {
    float value = upperLimits()[c] + 0.5 * w;
    if (finite(value)) var+= (*i) * ( value - avg ) * ( value - avg );
    c++;
  }
  if ( mtot > 0. ) var /= ( mtot );
  float h  = 0.9 * sqrt ( var ) * pow ( nentries_, - 0.2 );
  if ( ! ( h > 0. ) )
  {
    copyTrueHistogram();
    return;
  }
  /*
  LogDebug("") << "For " << reco::TaggingVariableTokens [ hist.constCategory().taggingVar() ] 
               << ", " << reco::btag::Vertices::name ( hist.constCategory().vertexType() ) << ": "
               << " avg=" << avg << " var=" << sqrt ( var ) << " h=" << h;
               */

  // float h = 2.0 * ( w );
  float norm = 1. / ( 2.506628274631 * upperLimits().size() * h ) ;

  int b=1;
  for ( vector< float >::iterator s=m_smoothed.begin(); 
        s!=m_smoothed.end() ; ++s )
  {
    c=1;
    for ( vector< float >::const_iterator i=values().begin(); 
          i!=values().end() ; ++i )
    {
      float dist = ( upperLimits()[c] - upperLimits()[b] ) ;
      float ex = norm * (*i) * exp ( -.5 * dist * dist / h / h );
      (*s)+=ex;
      c++;
    }
    b++;
  }
}

float combsv::KernelEstimatedHistogram::binContent ( int bin ) const
{
  return m_smoothed[bin];
}
