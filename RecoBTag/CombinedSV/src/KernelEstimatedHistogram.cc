#include "RecoBTag/CombinedSV/interface/KernelEstimatedHistogram.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"
// #include "CommonTools/Statistics/interface/Accumulator.h"
#include <iostream>

using namespace std;

namespace {
}

void combsv::KernelEstimatedHistogram::copyTrueHistogram()
{
  for ( vector< float >::const_iterator i=m_binValues.begin(); 
        i!=m_binValues.end() ; ++i )
  {
    m_smoothed.push_back ( *i );
  }
}

combsv::KernelEstimatedHistogram::KernelEstimatedHistogram(
    const combsv::CombinedSVCalibrationHistogram & hist ) :
  combsv::CombinedSVCalibrationHistogram ( hist )
{
  if ( hist.constCategory().taggingVar() == reco::btag::vertexCategory )
  {
    copyTrueHistogram();
    return;
  }

  if ( m_binULimits.size() < 2 )
  {
    throw cms::Exception ( " m_binULimits.size() < 2" );
  }

  // FIXME only for now sqrt(2pi)=2.506628274631
  float avg= 0.;
  int c=0;
  float mtot=0.;
  float w = m_binULimits[1] - m_binULimits[0];
  for ( vector< float >::const_iterator i=m_binValues.begin(); 
        i!=m_binValues.end() ; ++i )
  {
    m_smoothed.push_back ( 0. );
    float add = (*i) * ( m_binULimits[c] + 0.5 * w );
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
  for ( vector< float >::const_iterator i=m_binValues.begin(); 
        i!=m_binValues.end() ; ++i )
  {
    float value = m_binULimits[c] + 0.5 * w;
    // cout << " value=" << value;
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
  float norm = 1. / ( 2.506628274631 * m_binULimits.size() * h ) ;

  int b=0;
  for ( vector< float >::iterator s=m_smoothed.begin(); 
        s!=m_smoothed.end() ; ++s )
  {
    c=0;
    for ( vector< float >::const_iterator i=m_binValues.begin(); 
          i!=m_binValues.end() ; ++i )
    {
      float dist = ( m_binULimits[c] - m_binULimits[b] ) ;
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
