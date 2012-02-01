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

#include<string>
#include<vector>

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
	double MHTJetThreshold;

        struct Value {
          Value(const edm::InputTag& s, const std::string& b): src(s), branch(b), value(0) {}
          edm::InputTag src;
          std::string branch;
          float value;
        };

        std::vector<Value> values;
};
#endif
