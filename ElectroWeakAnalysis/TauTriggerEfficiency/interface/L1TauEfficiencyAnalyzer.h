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
#include "FWCore/Utilities/interface/InputTag.h"

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
		edm::InputTag OfflinePVSource;


        edm::InputTag L1GtReadoutRecordSource;
        edm::InputTag L1GtObjectMapRecordSource;

        edm::InputTag L1TauTriggerSource;

	double   jetMatchingCone;
        std::vector<unsigned> isolationThresholds;
        std::vector<double> thresholds;

		// good PV cuts
		int goodPVminNdof;
		double goodPVmaxAbsZ;
		double goodPVmaxRho;

        // Output tree and related variables
        TTree *l1tree;

        float jetPt, jetEt, jetUncorrEt, jetEta, jetPhi, jetMinDR;
        int jetsInMatchCone;
        bool hasL1Jet, hasL1TauJet, hasL1CenJet;
        bool hasTauVeto;
        std::vector<int> l1Isolations;
        std::vector<float> l1CaloRegions;        
		 int numOfflinePV; // To count total number of offline primary vertices 
		 int numGoodOfflinePV; // To count only the number of GOOD offline primary vertices
	bool hasTriggeredL1TauJet;
 	bool hasTriggeredL1CenJet;

	float met;
	float mht;

	int _L1EvtCnt;
	bool *_l1Flag;

        bool selectNearest;
};
#endif
