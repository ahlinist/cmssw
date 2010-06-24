#ifndef L1TauEfficiencyAnalyzer_H
#define L1TauEfficiencyAnalyzer_H

/** \class L1TauEfficiencyAnalyzer
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
//#include "FWCore/Framework/interface/TriggerNames.h"
#include "FWCore/Common/interface/TriggerNames.h"

#include "DataFormats/Math/interface/LorentzVector.h"

#include<vector>

// Forward declarations
namespace reco {
        class Candidate;
}
class TTree;

class L1TauEfficiencyAnalyzer {

    public:
	typedef math::XYZTLorentzVector LorentzVector;

        L1TauEfficiencyAnalyzer();
        ~L1TauEfficiencyAnalyzer();

        void Setup(const edm::ParameterSet&,TTree *l1tree);
        void fill(const edm::Event&, const edm::EventSetup&, const LorentzVector&);
        void fill(const edm::Event&, const edm::EventSetup&, const reco::Candidate&);
	double L1JetEtUncorr(const double corPt);

   private:
        void fillCaloRegion(const edm::Event& iEvent, const edm::EventSetup& iSetup, unsigned regionId);

        // Input parameters
        edm::InputTag L1extraTauJetSource;
        edm::InputTag L1extraCentralJetSource;
        edm::InputTag L1extraMETSource;
        edm::InputTag L1extraMHTSource;
        edm::InputTag L1CaloRegionSource;

        edm::InputTag L1GtReadoutRecordSource;
        edm::InputTag L1GtObjectMapRecordSource;
        edm::InputTag HLTResultsSource;

        edm::InputTag L1TauTriggerSource;

	double   jetMatchingCone;
        std::vector<unsigned> isolationThresholds;
        std::vector<double> thresholds;

        // Output tree and related variables
        TTree *l1tree;

        float jetPt, jetEt, jetUncorrEt, jetEta, jetPhi, jetMinDR;
        int jetsInMatchCone;
        bool hasL1Jet, hasL1TauJet, hasL1CenJet;
        bool hasTauVeto;
        std::vector<int> l1Isolations;
	bool hasTriggeredL1TauJet;
 	bool hasTriggeredL1CenJet;

	float met;
	float mht;

	int _L1EvtCnt;
	int _HltEvtCnt;
	bool *_l1Flag;
	bool *_hltFlag;

        bool selectNearest;

	//edm::TriggerNames _triggerNames;
	std::vector<std::string> _triggerNames;
};
#endif
