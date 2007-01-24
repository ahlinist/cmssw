#include "RecoBTag/CombinedSV/interface/CombinedSVCalibrationCategory.h"
#include "RecoBTag/CombinedSV/interface/CombinedSVCalibrationHistogram.h"
#include <iostream>

using namespace std;

combsv::CombinedSVCalibrationHistogram::CombinedSVCalibrationHistogram(
    double ptMin, double ptMax, double etaMin, double etaMax,
    reco::btag::Vertices::VertexType vertexType,
    combsv::Partons::PartonType      partonType,
    reco::btag::TaggingVariableName taggingVar,
    const vector<double> & binLowerEdge ) : binLowerEdge_ ( binLowerEdge )
{
  // add new category (=new histogram) based on information given in constructor
  CombinedSVCalibrationCategory calibCategory(ptMin,  ptMax,
        etaMin, etaMax, vertexType, partonType, taggingVar);
}

combsv::CombinedSVCalibrationHistogram::CombinedSVCalibrationHistogram()
{
  cout << "*** default constructor of :CombinedSVCalibrationHistogram *** "<< endl;
}

void combsv::CombinedSVCalibrationHistogram::startCalibration() {
  //
  // book histogram
  //

  reset();
  m_binULimits.clear();

  for ( vector<double>::const_iterator binIter = binLowerEdge_.begin(); 
        binIter != binLowerEdge_.end(); binIter++) {
    m_binULimits.push_back(*binIter);    
  } //for binIter
  m_binValues.resize(m_binULimits.size()+1);

} // void startCalibration

// ========================================================================================================
void combsv::CombinedSVCalibrationHistogram::updateCalibration(
    const combsv::CombinedSVCalibInput &,
    const vector<double> & binContent)
{
  // This update function is called only once in the 
  // conversion from ASCII -> XML based calibration
  // It sets the content of each bin to the value read out from the 
  // ASCII file
  //

  if (binContent.size() != m_binValues.size()) {

    cout << "### ERROR in combsv::CombinedSVCalibrationHistogram::updateCalibration" << endl;
    cout << "###       HISTOGRAM SIZES DO NOT MATCH                                " << endl;
    cout << "### from ASCII            " << binContent.size()                        << endl;
    cout << "### from booked histogram " << m_binValues.size()                       << endl;
  }

  // int i = 0;
  m_binValues.clear();
  for ( vector<double>::const_iterator valueIter = binContent.begin(); 
        valueIter != binContent.end(); valueIter ++ )
  {
    cout << "*** setting bin content to " << (*valueIter) << endl;
    // m_binValues[i] = (*valueIter); ?
    m_binValues.push_back ( *valueIter );
  } //for valueIter

} // void updatedCalibration

void combsv::CombinedSVCalibrationHistogram::finishCalibration()
{ }
