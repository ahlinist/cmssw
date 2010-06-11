#include "TauAnalysis/FittingTools/interface/interpolGridAuxFunctions.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

int getIndex(const std::vector<double>& elements, double value)
{
  unsigned numElements = elements.size();
  for ( unsigned iElement = 0; iElement < numElements; ++iElement ) {
    double element = elements[iElement];
    if ( element == value ) return iElement;
  }

  edm::LogWarning("getIndex") << "Failed to find value = " << value << " in vector passed as function argument !!";
  return -1;
}
