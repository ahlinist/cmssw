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
	double ptMin,etaMax;

        // Output tree and related variables
        TTree *muontree;

	float muonPt,muonEta,muonPhi;
	int nMuons;
};
#endif
