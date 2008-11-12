#ifndef ElectroWeakAnalysis_EWKTau_dqmAuxFunctions_h
#define ElectroWeakAnalysis_EWKTau_dqmAuxFunctions_h

#include <string>

const std::string parKeyword = "#PAR#";
const std::string plotKeyword = "#PLOT#";

std::string replace_string(const std::string& src, const std::string& keyword, const std::string& parameter, int& errorFlag);

const std::string dqmSeparator = "/";
const std::string dqmPrefix = std::string(dqmSeparator).append("DQMData").append(dqmSeparator);

void separateHistogramFromDirectoryName(const std::string& histogramAndDirectoryName, std::string& histogramName, std::string& directoryName);

#endif
