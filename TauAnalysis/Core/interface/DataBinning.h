#ifndef TauAnalysis_Core_DataBinning_h  
#define TauAnalysis_Core_DataBinning_h

/** \class DataBinning
 *
 * Store number of events passing selection in different bins of DataBinning object.
 *
 * Class can be used for Data and Monte Carlo, 
 * but is restricted to use reconstruction level information only
 * (cannot compute acceptance, purity, stability).
 * 
 * \author Christian Veelken, UC Davis
 *         (inspired by code written for H1 by Paul Laycock, University of Liverpool)
 *
 * \version $Revision: 1.2 $
 *
 * $Id: DataBinning.h,v 1.2 2010/01/15 17:38:10 veelken Exp $
 *
 */

#include "TauAnalysis/Core/interface/BinningBase.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <vector>
#include <string>

struct binEntryType
{
  binEntryType()
    : binContent_(0.), binSumw2_(0.) {}
  binEntryType& operator+=(double value)
  {
    binContent_ += value;
    binSumw2_ += (value*value);
    return (*this);
  }
  binEntryType& operator+=(const binEntryType& operand)
  {
    binContent_ += operand.binContent_;
    binSumw2_ += operand.binSumw2_;
    return (*this);
  }
  double binContent_;
  double binSumw2_;
};

class DataBinning : public BinningBase
{
 public:
  DataBinning();
  DataBinning(const edm::ParameterSet&);
  DataBinning(const DataBinning&);
  DataBinning(const std::string&, const BinGrid&, const std::vector<double>&, const std::vector<double>&);
  ~DataBinning();

  const std::string& name() const { return name_; }

  const BinGrid* binGrid() const { return binGrid_; }

  void bin(const std::vector<double>&, double = 1.);

  void print(std::ostream&) const;

  std::vector<binResultType> getBinResults(unsigned) const;
  std::vector<binResultType> getBinResultsSum() const;

 protected:
  virtual std::vector<std::string> encodeStringRep() const;
  virtual void decodeStringRep(std::vector<std::string>&);

  std::vector<binEntryType> binEntries_;
  unsigned numBins_;
};

#endif  


