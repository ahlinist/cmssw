#ifndef TauAnalysis_Core_BinningBase_h  
#define TauAnalysis_Core_BinningBase_h

/** \class BinningBase
 *
 * Pure virtual base-class for storing number of events passing selection in different bins
 * 
 * \author Christian Veelken, UC Davis
 *         (inspired by code written for H1 by Paul Laycock, University of Liverpool)
 *
 * \version $Revision: 1.2 $
 *
 * $Id: BinningBase.h,v 1.2 2010/01/15 17:38:48 veelken Exp $
 *
 */

#include "TauAnalysis/Core/interface/BinGrid.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <vector>

struct binResultType
{
  binResultType(double binContent, double binSumw2, const std::string name)
    : binContent_(binContent), 
      binSumw2_(binSumw2), 
      name_(name)
  {}

  double binContent_;
  double binSumw2_;

  std::string name_;
};

class BinningBase
{
 public:
  BinningBase();
  BinningBase(const edm::ParameterSet&);
  BinningBase(const BinningBase&);
  BinningBase(const std::string&, const BinGrid&);
  virtual ~BinningBase();

  const std::string& name() const { return name_; }

  const BinGrid* binGrid() const { return binGrid_; }

  virtual void bin(const std::vector<double>&, double = 1.) = 0;

  virtual void print(std::ostream&) const = 0;

  virtual std::vector<binResultType> getBinResults(unsigned) const = 0;
  virtual std::vector<binResultType> getBinResultsSum() const = 0;

  friend std::vector<std::string>& operator<<(std::vector<std::string>&, const BinningBase&);
  friend std::vector<std::string>& operator>>(std::vector<std::string>&, BinningBase&);

 protected:
  virtual std::vector<std::string> encodeStringRep() const;
  virtual void decodeStringRep(std::vector<std::string>&);

  std::string name_;

  const BinGrid* binGrid_;
};

std::vector<std::string>& operator<<(std::vector<std::string>&, const BinningBase&);
std::vector<std::string>& operator>>(std::vector<std::string>&, BinningBase&);

#endif  


