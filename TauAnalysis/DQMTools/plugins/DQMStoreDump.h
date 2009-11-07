#ifndef TauAnalysis_DQMTools_DQMStoreDump_h
#define TauAnalysis_DQMTools_DQMStoreDump_h

/** \class DQMStoreDump
 *  
 *  Class to print-out names of all objects held by DQMStore
 *
 *  $Date: 2009/08/17 16:07:21 $
 *  $Revision: 1.4 $
 *  \author Christian Veelken, UC Davis
 */

// framework & common header files
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class DQMStoreDump : public edm::EDAnalyzer
{
 public:
  explicit DQMStoreDump(const edm::ParameterSet&);
  virtual ~DQMStoreDump();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();  
};

#endif


