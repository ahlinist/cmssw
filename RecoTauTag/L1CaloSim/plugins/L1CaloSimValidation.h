#ifndef L1CaloSimValidation_H
#define L1CaloSimValidation_H

/** \class L1CaloSimValidation
 *
 *
 *  This class is an EDFilter for selecting events
 *  passing chosen HLT bits
 *
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/L1Trigger/interface/L1EtMissParticle.h"
#include "DataFormats/L1Trigger/interface/L1JetParticle.h"
#include "DataFormats/L1Trigger/interface/L1JetParticleFwd.h"

#include "TH1.h"

using namespace edm;
using namespace l1extra;
using namespace std;

class GetEtGreater {
  public:
  template <typename T> bool operator () (const T& i, const T& j) {
    return (i.et() > j.et());
  }
};

class L1CaloSimValidation : public edm::EDAnalyzer {

    public:
	typedef math::XYZTLorentzVector LorentzVector;

        L1CaloSimValidation(const edm::ParameterSet&);
        ~L1CaloSimValidation();


   private:
	virtual void beginJob(const edm::ParameterSet&, const edm::EventSetup&);
	virtual void analyze(const edm::Event&, const edm::EventSetup&);
	virtual void endJob();
	
	void fillHists(const edm::Event& iEvent);
	double L1JetEtUncorr(const double);

        // Input parameters
	InputTag _L1extraTauJetSource;  
        InputTag _L1extraCentralJetSource;
	InputTag _L1calosimTauJetSource;  
        InputTag _L1calosimCentralJetSource;

	double   _L1JetMatchingCone;


	//--- histograms
	TH1* _hL1extraNTauJet;
	TH1* _hL1extraTauJetEt;
	TH1* _hL1extraTauJetEta;
	TH1* _hL1extraTauJetPhi;

	TH1* _hL1calosimNTauJet;
	TH1* _hL1calosimTauJetEt;
	TH1* _hL1calosimTauJetEta;
	TH1* _hL1calosimTauJetPhi;

	TH1* _hL1extraNCentralJet;
	TH1* _hL1extraCentralJetEt;
	TH1* _hL1extraCentralJetEta;
	TH1* _hL1extraCentralJetPhi;

	TH1* _hL1calosimNCentralJet;
	TH1* _hL1calosimCentralJetEt;
	TH1* _hL1calosimCentralJetEta;
	TH1* _hL1calosimCentralJetPhi;

	TH1* _hMatchedL1TauJetDeltaEt;
	TH1* _hMatchedL1TauJetDeltaEta;
	TH1* _hMatchedL1TauJetDeltaPhi;

	TH1* _hMatchedL1CentralJetDeltaEt;
	TH1* _hMatchedL1CentralJetDeltaEta;
	TH1* _hMatchedL1CentralJetDeltaPhi;

	TH1* _hMatchedL1TauJetRelDeltaEt;
	TH1* _hMatchedL1CentralJetRelDeltaEt;


	// Highest Et jet
	TH1* _hL1extraTauJet1Et;
	TH1* _hL1extraTauJet1Eta;
	TH1* _hL1extraTauJet1Phi;

	TH1* _hL1calosimTauJet1Et;
	TH1* _hL1calosimTauJet1Eta;
	TH1* _hL1calosimTauJet1Phi;

	TH1* _hL1extraCentralJet1Et;
	TH1* _hL1extraCentralJet1Eta;
	TH1* _hL1extraCentralJet1Phi;

	TH1* _hL1calosimCentralJet1Et;
	TH1* _hL1calosimCentralJet1Eta;
	TH1* _hL1calosimCentralJet1Phi;


	TH1* _hMatchedL1TauJet1DeltaEt;
	TH1* _hMatchedL1CentralJet1DeltaEt;
	TH1* _hMatchedL1TauJet1RelDeltaEt;
	TH1* _hMatchedL1CentralJet1RelDeltaEt;

	
};
#endif
