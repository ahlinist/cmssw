#ifndef TauAnalysis_DQMTools_dqmAuxFunctions_h
#define TauAnalysis_DQMTools_dqmAuxFunctions_h

#include "DQMServices/Core/interface/DQMStore.h"

#include <TPRegexp.h>

#include <string>

const std::string parKeyword = "#PAR#";
const std::string plotKeyword = "#PLOT#";
const std::string rangeKeyword = "#RANGE";
const std::string processDirKeyword = "#PROCESSDIR#";

enum { kKeep, kDrop };

typedef std::pair<int, TPRegexp> outputCommandEntry;

std::string dqmDirectoryName(const std::string&);
std::string dqmSubDirectoryName(const std::string&, const std::string&);

void dqmCheckExistence(DQMStore&, const std::string&, const std::string&, int, int&);
void dqmRegisterHistogram(DQMStore&, TH1*, const std::string&);

void dqmCopyMonitorElement(DQMStore&, const std::string&, const std::string&, const std::string&, const std::string&, double, int = 1);
void dqmCopyRecursively(DQMStore&, const std::string&, const std::string&, double, int, bool, std::vector<outputCommandEntry>* = 0);

const std::string dqmSeparator = "/";
const std::string dqmSeparator2 = std::string(dqmSeparator).append(dqmSeparator);
//const std::string dqmRootDirectory = std::string(dqmSeparator).append("DQMData").append(dqmSeparator);
const std::string dqmRootDirectory = "";

const std::string meOptionsSeparator = "#";

void separateMonitorElementFromDirectoryName(const std::string&, std::string&, std::string&);

#endif
