#ifndef TauAnalysis_Core_binningAuxFunctions_h
#define TauAnalysis_Core_binningAuxFunctions_h

#include <vector>
#include <string>

std::string encodeBinningStringRep(const std::string&, const std::string&, const std::string&);
void decodeBinningStringRep(const std::string&, std::string&, std::string&, std::string&, int&);

std::string encodeVDoubleStringRep(const std::vector<double>&);
std::vector<double> decodeVDoubleStringRep(const std::string&, int&);

std::string encodeVStringStringRep(const std::vector<std::string>&);
std::vector<std::string> decodeVStringStringRep(const std::string&, int&);

#endif
