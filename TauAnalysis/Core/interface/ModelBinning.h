#ifndef TauAnalysis_Core_ModelBinning_h  
#define TauAnalysis_Core_ModelBinning_h

/** \class ModelBinning
 *
 * Store number of events passing selections (on generator and reconstruction level)
 * in different bins of ModelBinning object.
 *
 * Class can be used for estimating acceptance, purity and stability in Monte Carlo level
 * and needs access to generator level as well as reconstruction level information
 * 
 * \author Christian Veelken, UC Davis
 *         (inspired by code written for H1 by Paul Laycock, University of Liverpool)
 *
 * \version $Revision: 1.3 $
 *
 * $Id: ModelBinning.h,v 1.3 2010/09/28 11:23:29 jkolb Exp $
 *
 */

#include "TauAnalysis/Core/interface/BinningBase.h"
#include "TauAnalysis/Core/interface/DataBinning.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include <vector>
#include <string>

struct binEntryType_model
{
  binEntryType_model() {}
  binEntryType_model& operator+=(const binEntryType_model& operand)
  {
    gen_      += operand.gen_;
    rec_      += operand.rec_;
    stay_     += operand.stay_;
    lost_     += operand.lost_;
    smearIn_  += operand.smearIn_;
    smearOut_ += operand.smearOut_;
    return (*this);
  }
  binEntryType gen_;
  binEntryType rec_;
  binEntryType stay_;
  binEntryType lost_;
  binEntryType smearIn_;
  binEntryType smearOut_;
};

class ModelBinning : public BinningBase
{
 public:
  ModelBinning();
  ModelBinning(const edm::ParameterSet&);
  ~ModelBinning();

  const std::string& name() const { return name_; }

  const BinGrid* binGrid() const { return binGrid_; }

  void bin(const std::vector<double>&, double = 1.);
  void bin(const std::vector<double>&, bool, double, const std::vector<double>&, bool, double);

  DataBinning getAcceptance() const;
  DataBinning getPurity() const;
  DataBinning getStability() const;

  void print(std::ostream&) const;

  std::vector<binResultType> getBinResults(unsigned) const;
  std::vector<binResultType> getBinResultsSum() const;

 protected:
  virtual std::vector<std::string> encodeStringRep() const;
  virtual void decodeStringRep(std::vector<std::string>&);

  void encodeBinEntryStringRep(std::vector<std::string>&, unsigned, const char*, const binEntryType&) const;
  binEntryType* findBinEntry(unsigned, const std::string&);

  std::vector<binEntryType_model> binEntries_;
  unsigned numBins_;

  void printBinEntry(std::ostream&, const binEntryType_model&) const;
};

#endif  


