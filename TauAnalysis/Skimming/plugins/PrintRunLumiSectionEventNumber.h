#ifndef TauAnalysis_Skimming_PrintRunLumiSectionEventNumber_h
#define TauAnalysis_Skimming_PrintRunLumiSectionEventNumber_h

/** \class PrintRunLumiSectionEventNumber
 *
 * Write run + luminosity section + event numbers
 * of processed events to output stream
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.3 $
 *
 * $Id: PrintRunLumiSectionEventNumber.h,v 1.3 2010/12/03 16:11:49 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <iostream>
#include <string>

class PrintRunLumiSectionEventNumber : public edm::EDAnalyzer
{
 public:
  // constructor 
  explicit PrintRunLumiSectionEventNumber(const edm::ParameterSet&);
    
  // destructor
  virtual ~PrintRunLumiSectionEventNumber();
    
 private:
  void analyze(const edm::Event&, const edm::EventSetup&);

  std::ostream* outputStream_;

  int cfgError_;

  bool isOutputFile_;

  std::string separator_;
};

#endif   
