#include "RecoBTag/CombinedSV/interface/KernelEstimatedHistogram.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include <iostream>

using namespace std;

combsv::KernelEstimatedHistogram::KernelEstimatedHistogram(
    const combsv::CombinedSVCalibrationHistogram & hist ) :
  combsv::CombinedSVCalibrationHistogram ( hist )
{
  for ( vector< float >::const_iterator i=m_binValues.begin(); 
        i!=m_binValues.end() ; ++i )
  {
    m_smoothed.push_back ( *i );
  }
}

float combsv::KernelEstimatedHistogram::binContent ( int bin ) const
{
  return m_smoothed[bin];
}
