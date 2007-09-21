#include "RecoBTag/CombinedSV/interface/CombinedSVCalibrationHistogram.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include <iostream>
#include <cmath>

using namespace std;

combsv::CombinedSVCalibrationHistogram::CombinedSVCalibrationHistogram(
    float ptMin, float ptMax, float etaMin, float etaMax,
    reco::btag::Vertices::VertexType vertexType,
    combsv::Partons::PartonType      partonType,
    reco::TaggingVariableName taggingVar,
    const vector < float > & binLowerEdge, int nentries ) : 
    CalibratedHistogramXML  ( binLowerEdge ),
    category_ ( CombinedSVCalibrationCategory (
          ptMin,  ptMax, etaMin, etaMax, vertexType, partonType, taggingVar) ),
    nentries_(nentries) 
{
}

combsv::CombinedSVCalibrationHistogram::CombinedSVCalibrationHistogram() : CalibratedHistogramXML()
{}

int combsv::CombinedSVCalibrationHistogram::entries() const
{
  return nentries_;
}

void combsv::CombinedSVCalibrationHistogram::setEntries( int i )
{
  nentries_=i;
}

void combsv::CombinedSVCalibrationHistogram::addEntries( int i )
{
  nentries_+=i;
}

combsv::CombinedSVCalibrationCategory & 
    combsv::CombinedSVCalibrationHistogram::category()
{
  return category_;
}

const combsv::CombinedSVCalibrationCategory & 
    combsv::CombinedSVCalibrationHistogram::constCategory() const
{
  return category_;
}

void combsv::CombinedSVCalibrationHistogram::addCount ( float x )
{
  if ( upperLimits().size() != numberOfBins() + 1 )
  {
    edm::LogError("CombinedSVCalibrationHistogram" )
      << "bin size mismatch: " << upperLimits().size()
      << ":" << numberOfBins();
    exit(-1);
  }
  int ctr=0;
  // normalize old data
  for ( vector< float >::const_iterator i=upperLimits().begin(); 
        i!=upperLimits().end() ; ++i )
  {
    setBinContent( ctr, binContent(ctr) * nentries_ / ( nentries_ + 1. ) );
    ctr++;
  }

  nentries_++;
  int bin=findBin(x);
  // add new data
  setBinContent(bin, ( binContent(bin) + 1. / nentries_ ) );
}

int combsv::CombinedSVCalibrationHistogram::binCount ( int bin ) const
{
  if ( nentries_ == 0 )
  {
    /* edm::LogError ( "CombinedSVCalibrationHistogram" )
      << "trying to set bin count with zero entries!"; */
    return 0;
  }
  return int ( binContent ( bin ) * nentries_ );
}

void combsv::CombinedSVCalibrationHistogram::addBinCount ( int bin, int add )
{
  if ( upperLimits().size() != numberOfBins() + 1 )
  {
    edm::LogError("CombinedSVCalibrationHistogram" )
      << "bin size mismatch: " << upperLimits().size()
      << ":" << numberOfBins();
    exit(-1);
  }
  int ctr=0;
  // normalize old data
  for ( vector< float >::const_iterator i=upperLimits().begin(); 
        i!=upperLimits().end() ; ++i )
  {
    float old=binContent(ctr);
    if ( finite(old) )
    {
      setBinContent( ctr, old * nentries_ / ( nentries_ + (float) add ) );
      // LogDebug("Normalize old data") << old << "<->" << binContent(ctr); 
    } else {
      setBinContent( ctr, 0. );
    }
    ctr++;
  }

  nentries_+=add;
  // add new data
  float oldcontent=binContent(bin);
  if ( !finite(oldcontent) )
  {
    oldcontent=0.;
  }

  float newcontent = oldcontent + (float) add / nentries_;
  if ( finite(newcontent) )
  {
    setBinContent(bin, newcontent );
  }

  totalValid = false;
}

