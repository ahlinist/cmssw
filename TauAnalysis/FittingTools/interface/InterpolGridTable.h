#ifndef TauAnalysis_FittingTools_InterpolGridTable_h  
#define TauAnalysis_FittingTools_InterpolGridTable_h

/** \class InterpolGridTable
 *
 * Auxiliary class for storing table of numbers 
 * (e.g. cross-sections for MSSM Higgs production;
 *       parametrized by row = mA, column = tanBeta).
 * Class provides methods for loading/saving numbers from/to XML files
 * and provides functionality to compute values for arbitary values for row/column parameters
 * by means of (linear) interpolation between stored values.
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: RunningMedian.h,v 1.1 2009/11/27 15:46:32 veelken Exp $
 *
 */

#include <TMatrixD.h>

#include <iostream>
#include <vector>
#include <string>

#include <xercesc/util/XercesDefs.hpp>

class InterpolGridTable
{
 public:  
  InterpolGridTable();
  ~InterpolGridTable();

  void loadXML(const std::string&);
  void saveXML(const std::string&);
  
  double getValue(double, double) const;

  void print(std::ostream&) const;

 private:
  typedef std::vector<double> vDouble;

  std::string rowLabel_;
  vDouble rowGrid_;
  unsigned numRows_;

  std::string columnLabel_;
  vDouble columnGrid_;
  unsigned numColumns_;
  
  TMatrixD values_; 
  
  XMLCh* xmlTag_header_;
  XMLCh* xmlTag_value_;
  XMLCh* xmlTag_row_;
  XMLCh* xmlTag_column_;
};

#endif  


