#ifndef TauAnalysis_FittingTools_InterpolGridVector_h  
#define TauAnalysis_FittingTools_InterpolGridVector_h

/** \class InterpolGridVector
 *
 * Auxiliary class for storing vector of numbers 
 * (e.g. acceptances for MSSM Higgs production;
 *       parametrized by parameter = mA).
 * Class provides methods for loading/saving numbers from/to XML files
 * and provides functionality to compute values for arbitary values for parameter values
 * by means of (linear) interpolation between stored numbers.
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: InterpolGridVector.h,v 1.1 2010/06/11 08:54:59 veelken Exp $
 *
 */

#include <TVectorD.h>

#include <iostream>
#include <vector>
#include <string>

#include <xercesc/util/XercesDefs.hpp>

class InterpolGridVector
{
 public:  
  InterpolGridVector();
  ~InterpolGridVector();

  void loadXML(const std::string&);
  void saveXML(const std::string&);
  
  double getValue(double) const;

  void print(std::ostream&) const;

 private:
  typedef std::vector<double> vDouble;

  std::string paramLabel_;
  vDouble paramGrid_;
  unsigned numElements_;
  
  TVectorD values_; 
  
  XMLCh* xmlTag_header_;
  XMLCh* xmlTag_value_;
  XMLCh* xmlTag_param_;
};

#endif  


