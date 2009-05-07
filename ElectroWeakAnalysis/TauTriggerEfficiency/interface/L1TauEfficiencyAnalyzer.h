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

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/L1Trigger/interface/L1JetParticle.h"
#include "DataFormats/L1Trigger/interface/L1JetParticleFwd.h"

#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMap.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Framework/interface/TriggerNames.h"

#include "DataFormats/HLTReco/interface/TriggerFilterObjectWithRefs.h"

#include "DataFormats/TauReco/interface/CaloTau.h"
#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/Math/interface/LorentzVector.h"

#include <TTree.h>
#include <TFile.h>

#include <string>

using namespace edm;
using namespace l1extra;
using namespace reco;
using namespace std;

class L1TauEfficiencyAnalyzer {

    public:
	typedef math::XYZTLorentzVector LorentzVector;

        L1TauEfficiencyAnalyzer();
        ~L1TauEfficiencyAnalyzer();

        void Setup(const edm::ParameterSet&,TTree *l1tree);
        void fill(const edm::Event&, const reco::PFTau&);
        void fill(const edm::Event&, const reco::CaloTau&);
        void fill(const edm::Event&, const LorentzVector&);
        void fill(const edm::Event&, const reco::GsfElectron&);
	double L1JetEtUncorr(const double corPt);

   private:
        // Input parameters
	InputTag PFTauCollectionSource;  
	InputTag L1extraTauJetSource;  
        InputTag L1extraCentralJetSource;
        InputTag L1bitInfoSource;

        InputTag L1GtReadoutRecordSource;
        InputTag L1GtObjectMapRecordSource;
        InputTag HLTResultsSource;

        InputTag L1TauTriggerSource;

	double   jetMatchingCone;

        // Output tree and related variables
        TTree *l1tree;

        float jetPt, jetEt, jetUncorrEt, jetEta, jetPhi;
        char hasL1Jet, hasL1TauJet, hasL1CenJet;
        char hasTauVeto, hasEmTauVeto, hasHadTauVeto, hasIsolationVeto, hasPartialIsolationVeto, hasSumEtBelowThres, hasMaxEt, hasSoft, hasHard;

	char hasTriggeredL1TauJet;
 	char hasTriggeredL1CenJet;

	int _L1EvtCnt;
	int _HltEvtCnt;
	bool *_l1Flag;
	bool *_hltFlag;

	edm::TriggerNames _triggerNames;

};
#endif
