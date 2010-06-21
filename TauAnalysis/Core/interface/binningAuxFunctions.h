#ifndef TauAnalysis_Core_binningAuxFunctions_h
#define TauAnalysis_Core_binningAuxFunctions_h

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "TauAnalysis/Core/interface/BinGrid.h"
#include "TauAnalysis/Core/interface/BinningBase.h"
#include "TauAnalysis/Core/interface/ModelBinning.h"

#include <vector>
#include <string>
#include <iostream>

void printBinCenterPosition(std::ostream&, const BinGrid*, unsigned, bool = true);

double getBinContent(const std::vector<binResultType>&, const char*);
double getBinSumw2(const std::vector<binResultType>&, const char*);

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
