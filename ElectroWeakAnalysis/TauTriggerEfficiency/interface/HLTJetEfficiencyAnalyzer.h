#ifndef HLTJetEfficiencyAnalyzer_H
#define HLTJetEfficiencyAnalyzer_H

/** \class HLTJetEfficiencyAnalyzer
 *  \author Sami Lehti  -  HIP Helsinki
 *
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Utilities/interface/InputTag.h"

class TTree;

class HLTJetEfficiencyAnalyzer {

    public:
        HLTJetEfficiencyAnalyzer();
        ~HLTJetEfficiencyAnalyzer();

        void Setup(const edm::ParameterSet&,TTree *l1tree);
        void fill(const edm::Event&, const edm::EventSetup&);

   private:

	void reset();

        // Input parameters
	edm::InputTag HLTJetSource;
//	edm::InputTag MCJetSource;

        // Output tree and related variables
        TTree *jettree;

	int njets;
	float *jetpt,*jeteta,*jetphi;
};
#endif
