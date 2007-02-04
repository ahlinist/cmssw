#ifndef BTagPerformanceAnalyzer_H
#define BTagPerformanceAnalyzer_H

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "RecoBTag/Analysis/interface/BTagPABase.h"

#include <string>
#include <vector>

/** \class BTagPerformanceAnalyzer
 *
 *  Top level steering routine for b tag performance analysis.
 *
 */

class BTagPerformanceAnalyzer : public edm::EDAnalyzer {
   public:
      explicit BTagPerformanceAnalyzer(const edm::ParameterSet& pSet);

      ~BTagPerformanceAnalyzer();

      virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup);

  virtual void endJob();

   private:
  // Keeps track of the specific b tag and histograms.
   BTagPABaseABC *petBase;
};


#endif
