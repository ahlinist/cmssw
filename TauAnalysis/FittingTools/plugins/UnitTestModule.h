#ifndef TauAnalysis_FittingTools_UnitTestModule_h  
#define TauAnalysis_FittingTools_UnitTestModule_h

/** \class UnitTestModule
 *
 * Dummmy class derrived from EDAnalyzer
 * implemented for the purpose of performing unit-tests
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: UnitTestModule.h,v 1.1 2010/06/11 10:35:22 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <string>

class UnitTestModule : public edm::EDAnalyzer
{
 public:
  
  explicit UnitTestModule(const edm::ParameterSet&);
  ~UnitTestModule();
  
 private:

  void beginJob() {}
  void analyze(const edm::Event&, const edm::EventSetup&) {}
  void endJob();
};

#endif  


