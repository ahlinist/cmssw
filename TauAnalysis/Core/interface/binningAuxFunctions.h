#ifndef TauAnalysis_Core_binningAuxFunctions_h
#define TauAnalysis_Core_binningAuxFunctions_h

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

struct binEntryType_model
{
  binEntryType_model() {}
  binEntryType_model& operator+=(const binEntryType_model& operand)
  {
    gen_ += operand.gen_;
    rec_ += operand.rec_;
    stay_ += operand.stay_;
    lost_ += operand.lost_;
    smearIn_ += operand.smearIn_;
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

std::string encodeBinningStringRep(const std::string&, const std::string&, const std::string&);
void decodeBinningStringRep(const std::string&, std::string&, std::string&, std::string&, int&);

std::string encodeVDoubleStringRep(const std::vector<double>&);
std::vector<double> decodeVDoubleStringRep(const std::string&, int&);

std::string encodeVStringStringRep(const std::vector<std::string>&);
std::vector<std::string> decodeVStringStringRep(const std::string&, int&);

void computeAcceptance(const binEntryType_model&, double&, double&);
void computePurity(const binEntryType_model&, double&, double&);
void computeStability(const binEntryType_model&, double&, double&);

#endif
