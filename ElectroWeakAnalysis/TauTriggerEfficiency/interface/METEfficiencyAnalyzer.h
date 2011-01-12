#ifndef METEfficiencyAnalyzer_H
#define METEfficiencyAnalyzer_H

/** \class METEfficiencyAnalyzer
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

//#include "DataFormats/METReco/interface/PFMET.h"
//#include "DataFormats/METReco/interface/PFMETFwd.h"

class TTree;

class METEfficiencyAnalyzer {

    public:
        METEfficiencyAnalyzer();
        ~METEfficiencyAnalyzer();

        void Setup(const edm::ParameterSet&,TTree *l1tree);
        void fill(const edm::Event&, const edm::EventSetup&);

   private:

        // Input parameters
	edm::InputTag HLTMETSource;
        edm::InputTag METSource;
	edm::InputTag MCSource;
	edm::InputTag METCleaningSource;

        // Output tree and related variables
        TTree *mettree;

	float hltMET,MET,mcMET;
	int METclean;
};
#endif
