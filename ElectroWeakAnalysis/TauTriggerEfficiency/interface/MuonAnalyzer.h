#ifndef MuonAnalyzer_H
#define MuonAnalyzer_H

/** \class MuonAnalyzer
 *
 *
 *  This class is for storing offline muon for Z mass reco
 *
 *  \author Sami Lehti  -  HIP Helsinki
 *
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Utilities/interface/InputTag.h"

class TTree;

class MuonAnalyzer {

    public:
        MuonAnalyzer();
        ~MuonAnalyzer();

        void Setup(const edm::ParameterSet&,TTree *l1tree);
        void fill(const edm::Event&, const edm::EventSetup&);

   private:

        // Input parameters
	edm::InputTag MuonSource;
        edm::InputTag muTauPairSource;
        edm::InputTag pfIsoChargedSrc;
        edm::InputTag pfIsoNeutralSrc;
        edm::InputTag pfIsoGammaSrc;
	double ptMin,etaMax;

        // Output tree and related variables
        TTree *muontree;

	float muonPt,muonEta,muonPhi;
        float muonIso03SumPt, muonIso03EmEt, muonIso03HadEt;
        float muonPFIsoCharged, muonPFIsoNeutral, muonPFIsoGamma;
        float muTauInvMass;
	int nMuons;
};
#endif
