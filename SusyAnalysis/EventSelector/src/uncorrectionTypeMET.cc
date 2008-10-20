#include "SusyAnalysis/EventSelector/interface/uncorrectionTypeMET.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

pat::MET::UncorrectionType
pat::uncorrectionTypeMET (const std::string& uncorrectionName) 
{
  //
  // compare with known uncorrection types 
  //   (+ "none" for corrected MET)
  //
  if (uncorrectionName == "uncorrALL")
    return pat::MET::UncorrectionType(pat::MET::uncorrALL);
  if (uncorrectionName == "uncorrJES")
    return pat::MET::UncorrectionType(pat::MET::uncorrJES);
  if (uncorrectionName == "uncorrMUON")
    return pat::MET::UncorrectionType(pat::MET::uncorrMUON);
  if (uncorrectionName == "none")
    return pat::MET::uncorrMAXN;
  
  edm::LogWarning("uncorrectionTypeMET") 
    << "unknown uncorrection type " << uncorrectionName
    << " ; return corrected MET";
  return pat::MET::uncorrMAXN;
}

