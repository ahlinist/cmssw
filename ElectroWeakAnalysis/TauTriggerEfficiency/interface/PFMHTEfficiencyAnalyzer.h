#ifndef PFMHTEfficiencyAnalyzer_H
#define PFMHTEfficiencyAnalyzer_H

/** \class PFMHTEfficiencyAnalyzer
 *
 *
 *  This class is an EDFilter for selecting events
 *  passing chosen HLT bits
 *
 *  \author Sami Lehti  -  HIP Helsinki
 *
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Utilities/interface/InputTag.h"

class TTree;

class PFMHTEfficiencyAnalyzer {

    public:
        PFMHTEfficiencyAnalyzer();
        ~PFMHTEfficiencyAnalyzer();

        void Setup(const edm::ParameterSet&,TTree*);
        void fill(const edm::Event&, const edm::EventSetup&);

   private:

        // Input parameters
	edm::InputTag PFJetSource;

        // Output tree and related variables
        TTree *pfmhttree;

	float hltPFMHT;
};
#endif
