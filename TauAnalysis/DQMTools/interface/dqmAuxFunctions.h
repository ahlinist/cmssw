#ifndef TauAnalysis_DQMTools_dqmAuxFunctions_h
#define TauAnalysis_DQMTools_dqmAuxFunctions_h

#include "DQMServices/Core/interface/DQMStore.h"

#include <string>

const std::string parKeyword = "#PAR#";
const std::string plotKeyword = "#PLOT#";
const std::string rangeKeyword = "#RANGE";
const std::string processDirKeyword = "#PROCESSDIR#";

std::string dqmDirectoryName(const std::string&);
std::string dqmSubDirectoryName(const std::string&, const std::string&);
void dqmCopyRecursively(DQMStore&, const std::string&, const std::string&, double, int, bool);

const std::string dqmSeparator = "/";
const std::string dqmSeparator2 = std::string(dqmSeparator).append(dqmSeparator);
//const std::string dqmRootDirectory = std::string(dqmSeparator).append("DQMData").append(dqmSeparator);
const std::string dqmRootDirectory = "";

void separateMonitorElementFromDirectoryName(const std::string&, std::string&, std::string&);

#endif
