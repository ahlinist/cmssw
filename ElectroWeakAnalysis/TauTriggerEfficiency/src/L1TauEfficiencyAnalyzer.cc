#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/L1TauEfficiencyAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/L1Trigger/interface/L1JetParticle.h"
#include "DataFormats/L1Trigger/interface/L1JetParticleFwd.h"

#include "DataFormats/L1Trigger/interface/L1EtMissParticle.h"
#include "DataFormats/L1Trigger/interface/L1EtMissParticleFwd.h"

#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMap.h"


#include "DataFormats/HLTReco/interface/TriggerFilterObjectWithRefs.h"

#include "DataFormats/Candidate/interface/Candidate.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"


#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctJetCand.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloRegion.h"

#include "CondFormats/L1TObjects/interface/L1GctJetFinderParams.h"
#include "CondFormats/DataRecord/interface/L1GctJetFinderParamsRcd.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/TriggerNamesService.h"

#include <TTree.h>
#include <TF1.h>

#include <string>
#include <algorithm>
#include <iostream>

namespace {
  class EtaPhiHelper {
  public:
    EtaPhiHelper(unsigned minE, unsigned maxE, unsigned minP, unsigned maxP):
      minEta(minE), maxEta(maxE), minPhi(minP), maxPhi(maxP) {}

    unsigned nextPhi(unsigned phi) const {
      if(phi >= maxPhi)
        return minPhi;
      else
        return phi+1;
    }

    unsigned prevPhi(unsigned phi) const {
      if(phi <= minPhi)
        return maxPhi;
      else
        return phi-1;
    }

    unsigned nextEta(unsigned eta) const {
      if(eta >= maxEta)
        return eta;
      else
        return eta+1;
    }

    unsigned prevEta(unsigned eta) const {
      if(eta <= minEta)
        return eta;
      else
        return eta-1;
    }

  private:
    const unsigned minEta;
    const unsigned maxEta;
    const unsigned minPhi;
    const unsigned maxPhi;
  };
}

using namespace edm;
using namespace l1extra;
using namespace reco;
using namespace std;

// Default constructor
L1TauEfficiencyAnalyzer::L1TauEfficiencyAnalyzer():
  l1tree(0),
  l1CaloRegions(9, -1),
  _l1Flag(new bool[128])
{}


L1TauEfficiencyAnalyzer::~L1TauEfficiencyAnalyzer(){
  delete[] _l1Flag;
  /*
	cout << endl;
	cout << "Events analyzed " << nEvents << endl;
	cout << endl;
  */
}

void L1TauEfficiencyAnalyzer::Setup(const edm::ParameterSet& iConfig,TTree *trigtree)
{
  L1extraTauJetSource = iConfig.getParameter<edm::InputTag>("L1extraTauJetSource");
  L1extraMETSource = iConfig.getParameter<edm::InputTag>("L1extraMETSource");
  L1extraMHTSource = iConfig.getParameter<edm::InputTag>("L1extraMHTSource");
  L1extraCentralJetSource = iConfig.getParameter<edm::InputTag>("L1extraCentralJetSource");
  L1CaloRegionSource = iConfig.getParameter<edm::InputTag>("L1CaloRegionSource");

  L1GtReadoutRecordSource = iConfig.getParameter<edm::InputTag>("L1GtReadoutRecord");
  L1GtObjectMapRecordSource = iConfig.getParameter<edm::InputTag>("L1GtObjectMapRecord");
  
  L1TauTriggerSource = iConfig.getParameter<edm::InputTag>("L1TauTriggerSource");

  jetMatchingCone = iConfig.getParameter<double>("L1JetMatchingCone");
  isolationThresholds = iConfig.getParameter<std::vector<unsigned> >("L1IsolationThresholds");

  // Vertex and good vertex parameters. The cuts should be replaced in the future by the cut string parser...
  OfflinePVSource = iConfig.getParameter<edm::InputTag>("OfflinePVSource");
  goodPVminNdof  = iConfig.getParameter<int>("goodPVminNdof");
  goodPVmaxAbsZ  = iConfig.getParameter<double>("goodPVmaxAbsZ");
  goodPVmaxRho   = iConfig.getParameter<double>("goodPVmaxRho");


  if(isolationThresholds.size() == 0)
    throw cms::Exception("Configuration") << "At least one item in L1IsolationThresholds is needed!" << std::endl;

  std::string matchMode = iConfig.getParameter<std::string>("L1JetMatchingMode");
  if(matchMode == "nearestDR")
    selectNearest = true;
  else if(matchMode == "highestEt")
    selectNearest = false;
  else
    throw cms::Exception("Configuration") << "L1JetMatchingMode should be 'nearestDR' or 'highestEt', was '" << matchMode << "'" << std::endl;


  l1tree = trigtree;

  // Setup branches
  l1tree->Branch("L1JetPt", &jetPt);
  l1tree->Branch("L1JetEt", &jetEt);
  l1tree->Branch("L1JetEta", &jetEta);
  l1tree->Branch("L1JetPhi", &jetPhi);
  l1tree->Branch("L1JetMatchDR", &jetMinDR);
  l1tree->Branch("L1TauVeto", &hasTauVeto);
  l1tree->Branch("hasMatchedL1Jet", &hasL1Jet);
  l1tree->Branch("hasMatchedL1TauJet", &hasL1TauJet);
  l1tree->Branch("hasMatchedL1CenJet", &hasL1CenJet);
  l1tree->Branch("L1MET", &met);
  l1tree->Branch("L1MHT", &mht);
  l1tree->Branch("L1JetsInMatchingCone", &jetsInMatchCone);

  l1Isolations.resize(isolationThresholds.size(), 0);
  thresholds.resize(isolationThresholds.size(), 0.);
  for(size_t i=0; i<isolationThresholds.size(); ++i) {
    l1tree->Branch(Form("L1IsolationRegions_%uGeV", isolationThresholds[i]), &(l1Isolations[i]));
  }

  l1tree->Branch("L1CaloRegionEt", &l1CaloRegions);

  l1tree->Branch("numOfflinePV", &numOfflinePV);
  l1tree->Branch("numGoodOfflinePV", &numGoodOfflinePV);

  l1tree->Branch("hasTriggeredAndMatchedL1TauJet", &hasTriggeredL1TauJet);
  l1tree->Branch("hasTriggeredAndMatchedL1CenJet", &hasTriggeredL1CenJet);


  _L1EvtCnt = 0;
}

void L1TauEfficiencyAnalyzer::fill(const edm::Event& iEvent, const edm::EventSetup& iSetup, const reco::Candidate& tau) {
  fill(iEvent, iSetup, tau.p4());
}

void L1TauEfficiencyAnalyzer::fill(const edm::Event& iEvent, const edm::EventSetup& iSetup, const LorentzVector& tau) {

  // Reset variables
  jetPt = 0.0;
  jetEt = 0.0;
  jetUncorrEt = 0.0;
  jetEta = 0.0;
  jetPhi = -999.0;
  jetMinDR = 0.0;
  hasL1Jet = false;
  hasL1TauJet = false;
  hasL1CenJet = false;
  hasTauVeto = false;
  std::fill(l1Isolations.begin(), l1Isolations.end(), 0);
  std::fill(l1CaloRegions.begin(), l1CaloRegions.end(), -1);
  numOfflinePV = 0; // Event primary vertex counter
  numGoodOfflinePV = 0; // Event GOOD primary vertex counter
  hasTriggeredL1TauJet = false;
  hasTriggeredL1CenJet = false;
  met=0.;
  mht=0.;
  jetsInMatchCone=0;

  unsigned jetRegionId = 0;

  // Get data from event 
  Handle<L1JetParticleCollection> l1TauHandle;
  iEvent.getByLabel(L1extraTauJetSource, l1TauHandle);
  if(!l1TauHandle.isValid()) {
    edm::LogError("TTEffAnalyzer") << "L1TauEffAnalyzer -- No L1extra TauJet found! " << std::endl;
    return;
  }

  // Get data from event 
  Handle<L1EtMissParticleCollection> l1METHandle;
  iEvent.getByLabel(L1extraMETSource, l1METHandle);
  if(!l1METHandle.isValid()) {
    edm::LogWarning("TTEffAnalyzer") << "%L1TauEffAnalyzer -- No L1extra MET found! " << std::endl;
    return;
  }

  // Get data from event 
  Handle<L1EtMissParticleCollection> l1MHTHandle;
  iEvent.getByLabel(L1extraMHTSource, l1MHTHandle);
  if(!l1MHTHandle.isValid()) {
    edm::LogWarning("TTEffAnalyzer") << "%L1TauEffAnalyzer -- No L1extra MHT found! " << std::endl;
    return;
  }

  Handle<L1JetParticleCollection> l1CentralJetHandle;
  iEvent.getByLabel(L1extraCentralJetSource, l1CentralJetHandle);
  if(!l1CentralJetHandle.isValid()) {
    edm::LogWarning("TTEffAnalyzer") << "%L1TauEffAnalyzer -- No L1extra central jet found! " << std::endl;
    return;
  }


  //Fill the MET/MHT
  if(l1METHandle->size()>0)
    met = l1METHandle->at(0).et();
  else 
    met=0;

  if(l1MHTHandle->size()>0)
    mht = l1MHTHandle->at(0).et();
  else 
    mht=0;


   // Count the number of offline primary vertices
   edm::Handle<reco::VertexCollection> offlinePVHandle; // Handle to the actual vertex collection

   try {
 	  iEvent.getByLabel(OfflinePVSource, offlinePVHandle); // Try to get collection
   } catch (...) {
	  edm::LogWarning("TTEffAnalyzer") << "No offlinePV found with label " << OfflinePVSource << std::endl;
   }
 
   if ( offlinePVHandle.isValid() ) {
 	  numOfflinePV = offlinePVHandle->size();
   }
 
   // Loop over each offline PV and count only those considered "good"
   for (vector<Vertex>::const_iterator iOfflinePV = offlinePVHandle->begin(); iOfflinePV != offlinePVHandle->end(); iOfflinePV++) {
 	  if ( (iOfflinePV->isValid() == true) 	&&
           (iOfflinePV->isFake() == false) 	&&
 		   (iOfflinePV->ndof() >= 4) 		&&
 		   (fabs(iOfflinePV->z()) < 24.0)	&&
 		   (iOfflinePV->position().rho() < 2.0) ){
 		   numGoodOfflinePV++;	   
 	  }
   }



  // Process L1 triggered taus
  const L1JetParticleCollection & l1Taus = *(l1TauHandle.product());
  L1JetParticleCollection::const_iterator iJet;

  // Match for PF tau and L1extra tau
  jetMinDR = 99999999.;
  double jetMaxEt = 0.;
  for(iJet = l1Taus.begin(); iJet != l1Taus.end(); ++iJet) {
    //if(iJet->et() <= 5.)
    //  continue;

    double DR = deltaR(iJet->eta(), iJet->phi(), tau.Eta(), tau.Phi());
    if(DR < jetMatchingCone) {
      ++jetsInMatchCone;
      if((selectNearest && DR < jetMinDR) ||
         (!selectNearest && iJet->et() > jetMaxEt)) {
        jetMinDR = DR;
        jetPt = iJet->pt();
        jetEt = iJet->et();
        jetUncorrEt = L1JetEtUncorr(jetEt);
        jetEta = iJet->eta();
        jetPhi = iJet->phi();
        hasL1Jet = true;
        hasL1TauJet = true;
        hasL1CenJet = false;
        jetRegionId = iJet->gctJetCand()->regionId().rawId();
        /*
          std::cout << "L1Analyzer " << __LINE__ << ": " << iJet->gctJetCand()->regionId().rawId()
                    << " etaIndex " << iJet->gctJetCand()->etaIndex() << " phiIndex " << iJet->gctJetCand()->phiIndex()
                    << " tau jet et " << jetEt << " eta " << jetEta << " phi " << jetPhi
                    << std::endl;
        */
      }
    }
  }

  // Match L1 central jets, if L1TauJet was found insidet
  // jetMatchingCone, jetMinDR holds the DR in there.
  for(iJet = l1CentralJetHandle->begin(); iJet != l1CentralJetHandle->end(); ++iJet) {
      //if(iJet->et() <= 5.)
      //  continue;

      double DR = deltaR(iJet->eta(), iJet->phi(), tau.Eta(), tau.Phi());
      if(DR < jetMatchingCone) {
        ++jetsInMatchCone;
        if((selectNearest && DR < jetMinDR) ||
           (!selectNearest && iJet->et() > jetMaxEt)) {
          jetMinDR = DR;
          jetPt = iJet->pt();
          jetEt = iJet->et();
          jetUncorrEt = L1JetEtUncorr(jetEt);
          jetEta = iJet->eta();
          jetPhi = iJet->phi();
          hasL1Jet = true;
          hasL1TauJet = false;
          hasL1CenJet = true;
          jetRegionId = iJet->gctJetCand()->regionId().rawId();
          /*
            std::cout << "L1Analyzer " << __LINE__ << ": " << iJet->gctJetCand()->regionId().rawId() 
                      << " etaIndex " << iJet->gctJetCand()->etaIndex() << " phiIndex " << iJet->gctJetCand()->phiIndex()
                      << " central jet et " << jetEt << " eta " << jetEta << " phi " << jetPhi
                      << std::endl;
          */
        }
      }
  }

  if(hasL1Jet) {
    //std::cout << "L1Analyzer " << __LINE__ << ": L1 jet et " << jetEt << std::endl;
    fillCaloRegion(iEvent, iSetup, jetRegionId);
  }
      

  // Store L1 trigger bits
  edm::Handle<L1GlobalTriggerReadoutRecord>      l1GTRR;
  edm::Handle<L1GlobalTriggerObjectMapRecord>    l1GTOMRec;

  iEvent.getByLabel(L1GtReadoutRecordSource,l1GTRR);
  if(!l1GTRR.isValid()) {
    edm::LogWarning("TTEffAnalyzer") << "%L1TauEffAnalyzer -- No L1GlobalTriggerReadouRecord found! " << std::endl;
    return;
  }

  iEvent.getByLabel(L1GtObjectMapRecordSource,l1GTOMRec);
  if(!l1GTOMRec.isValid()) {
    edm::LogWarning("TTEffAnalyzer") << "%L1TauEffAnalyzer -- No L1GlobalTriggerObjectMapRecord found! " << std::endl;
    return;
  }

  // 1st event : Book as many branches as trigger paths provided in the input...
  if (l1GTRR.isValid() and l1GTOMRec.isValid()) {  
    DecisionWord gtDecisionWord = l1GTRR->decisionWord();
    const unsigned int numberTriggerBits(gtDecisionWord.size());
    if (_L1EvtCnt==0){
      // get ObjectMaps from ObjectMapRecord
      const std::vector<L1GlobalTriggerObjectMap>& objMapVec =  l1GTOMRec->gtObjectMap();
      // 1st event : Book as many branches as trigger paths provided in the input...
      for (std::vector<L1GlobalTriggerObjectMap>::const_iterator itMap = objMapVec.begin();
           itMap != objMapVec.end(); ++itMap) {
        // Get trigger bits
        int itrig = (*itMap).algoBitNumber();
        // Get trigger names
        TString algoBitToName( (*itMap).algoName() );
        
        l1tree->Branch(algoBitToName, _l1Flag+itrig);
      }
      _L1EvtCnt++;
    }
    for (unsigned int iBit = 0; iBit < numberTriggerBits; ++iBit) {     
      // ...Fill the corresponding accepts in branch-variables
      _l1Flag[iBit] = (bool)gtDecisionWord[iBit];
      //std::cout << "L1 TD: "<<iBit<<" "<<algoBitToName[iBit]<<" "<<gtDecisionWord[iBit]<< std::endl;
    }
  }


  // Find fired L1 trigger objects
  std::vector<l1extra::L1JetParticleRef> tauCandRefVec;
  std::vector<l1extra::L1JetParticleRef> jetCandRefVec;

  edm::Handle<trigger::TriggerFilterObjectWithRefs> l1TriggeredTaus;
  if(iEvent.getByLabel(L1TauTriggerSource,l1TriggeredTaus)) {

    tauCandRefVec.clear();
    jetCandRefVec.clear();

    l1TriggeredTaus->getObjects(trigger::TriggerL1TauJet,tauCandRefVec);
    l1TriggeredTaus->getObjects(trigger::TriggerL1CenJet,jetCandRefVec);

    //std::cout<<tauCandRefVec.size()<<std::endl;
    float minDR = 99999999.;
    for( unsigned int iL1Tau=0; iL1Tau <tauCandRefVec.size();iL1Tau++) { 
      if(&tauCandRefVec[iL1Tau]) {
	
	double DR = deltaR(tauCandRefVec[iL1Tau]->eta(), tauCandRefVec[iL1Tau]->phi(), 
			   tau.Eta(), tau.Phi());
	if(DR < jetMatchingCone && DR < minDR) {
	  minDR = DR;
	  hasTriggeredL1TauJet = true;
	  //std::cout<<"Found triggered tau: "<<tauCandRefVec[iL1Tau]->et()
	  //   <<" "<<tauCandRefVec[iL1Tau]->eta()
	  //   <<" "<<tauCandRefVec[iL1Tau]->phi()<<std::endl;
	}
      }
    }
    minDR = 99999999.;
    for( unsigned int iL1Jet=0; iL1Jet <jetCandRefVec.size();iL1Jet++) {  
      if(&jetCandRefVec[iL1Jet]) {
	double DR = deltaR(jetCandRefVec[iL1Jet]->eta(), jetCandRefVec[iL1Jet]->phi(), 
			   tau.Eta(), tau.Phi());
	if(DR < jetMatchingCone && DR < minDR) {
	  minDR = DR;
	  hasTriggeredL1CenJet = true;
	  //std::cout<<"Found triggered jet: "<<jetCandRefVec[iL1Jet]->et()
	  //   <<" "<<jetCandRefVec[iL1Jet]->eta()
	  //   <<" "<<jetCandRefVec[iL1Jet]->phi()<<std::endl;
	}
      }
    }

  }


} 

void L1TauEfficiencyAnalyzer::fillCaloRegion(const edm::Event& iEvent, const edm::EventSetup& iSetup, unsigned regionId) {
  edm::Handle<std::vector<L1CaloRegion> > caloRegionHandle;
  if(!iEvent.getByLabel(L1CaloRegionSource, caloRegionHandle)) {
    edm::LogWarning("TTEffAnalyzer") << "No L1CaloRegion! with label " << L1CaloRegionSource << std::endl;
    return;
  }

  if(caloRegionHandle->size() == 0) {
    edm::LogWarning("TTEffAnalyzer") << "L1CaloRegion size is zero!" << std::endl;
    return;
  }
      

  std::vector<L1CaloRegion>::const_iterator iter = caloRegionHandle->begin();
  std::vector<L1CaloRegion>::const_iterator found = caloRegionHandle->end();
  unsigned minEta = iter->gctEta();
  unsigned maxEta = iter->gctEta();
  unsigned minPhi = iter->gctPhi();
  unsigned maxPhi = iter->gctPhi();
  for(; iter != caloRegionHandle->end(); ++iter) {
    minEta = std::min(minEta, iter->gctEta());
    maxEta = std::max(maxEta, iter->gctEta());
    minPhi = std::min(minPhi, iter->gctPhi());
    maxPhi = std::max(maxPhi, iter->gctPhi());

    if(regionId == iter->id().rawId()) {
      found = iter;
    }
  }
  if(found == caloRegionHandle->end()) {
    edm::LogWarning("TTEffAnalyzer") << "Didn't find matching L1CaloRegion" << std::endl;
    return;
  }

  // Effectively this is what is done in L1Trigger/GlobalCaloTrigger/src/L1GctHardwareJetFinder.cc
  // and L1Trigger/GlobalCaloTrigger/plugins/L1GctEmulator.cc
  edm::ESHandle< L1GctJetFinderParams > jfPars ;
  iSetup.get< L1GctJetFinderParamsRcd >().get(jfPars);
  double rgnEtLsbGeV = jfPars->getRgnEtLsbGeV();
  for(size_t i=0; i<isolationThresholds.size(); ++i) {
    thresholds[i] = isolationThresholds[i]/rgnEtLsbGeV; // transform GeV to gct internal units
  }
  /*
  std::cout << "L1Analyzer " << __LINE__ 
            << ": threshold gct " << jfPars->getTauIsoEtThresholdGct()
            << " GeV " << jfPars->getTauIsoEtThresholdGeV()
            << " rgnEtLsb " << jfPars->getRgnEtLsbGeV()
            << " our threshold GeV " << isolationThresholds[0]
            << " gct " << static_cast<unsigned>(isolationThresholds[0]/jfPars->getRgnEtLsbGeV())
            << std::endl;
  std::cout << "L1Analyzer " << __LINE__ << ": " << found->id().rawId()
            << " etaIndex " << found->gctEta() << " phiIndex " << found->gctPhi()
            << " et (raw) " << found->et()
            << " et (scaled) " << found->et()*jfPars->getRgnEtLsbGeV()
            << " tauVeto " << found->tauVeto()
            << std::endl;
  */


  hasTauVeto = found->tauVeto();

  unsigned eta = found->gctEta();
  unsigned phi = found->gctPhi();

  EtaPhiHelper helper(minEta, maxEta, minPhi, maxPhi);
  unsigned nextEta = helper.nextEta(eta);
  unsigned prevEta = helper.prevEta(eta);
  unsigned nextPhi = helper.nextPhi(phi);
  unsigned prevPhi = helper.prevPhi(phi);

  
  if(nextEta == eta || prevEta == eta)
    throw cms::Exception("LogicError") << "Logic error with etas in " << __LINE__ << ":" << __LINE__;

  // The region of the jet is always in the index 0
  size_t iRegion=0;
  l1CaloRegions[iRegion] = found->et()*rgnEtLsbGeV; // transform internal units to GeV
  ++iRegion;  

  for(iter = caloRegionHandle->begin(); iter != caloRegionHandle->end(); ++iter) {
    if(((iter->gctEta() == nextEta || iter->gctEta() == prevEta) &&
        (iter->gctPhi() == nextPhi || iter->gctPhi() == prevPhi)) ||
       (iter->gctEta() == eta && (iter->gctPhi() == nextPhi || iter->gctPhi() == prevPhi)) ||
       (iter->gctPhi() == phi && (iter->gctEta() == nextEta || iter->gctEta() == prevEta))) {
      /*
      std::cout << "L1Analyzer " << __LINE__ << ": " << iter->id().rawId()
                << " etaIndex " << iter->gctEta() << " phiIndex " << iter->gctPhi()
                << " et (raw) " << iter->et()
                << " et (scaled) " << iter->et()*jfPars->getRgnEtLsbGeV()
                << " tauVeto " << iter->tauVeto()
                << std::endl;
      */
      for(size_t i=0; i<thresholds.size(); ++i) {
        if(iter->et() < thresholds[i])
          ++(l1Isolations[i]);
      }

      l1CaloRegions[iRegion] = iter->et()*rgnEtLsbGeV; // transform internal units to GeV
      ++iRegion;

      hasTauVeto = hasTauVeto || iter->tauVeto();
    }
  }
  /*
  std::cout << "L1Analyzer " << __LINE__ << ": L1 isolation " << l1Isolations[1] << " (number of adjacent cells with Et < " << isolationThresholds[1] << " GeV; should be 7/8 for tau)" 
            << " hasTauVeto " << hasTauVeto
            << std::endl;
  */
}


double L1TauEfficiencyAnalyzer::L1JetEtUncorr(const double corPt)
{
  TF1 *Response = new TF1("Response","[0]-[1]/(pow(log10(x),[2])+[3])+[4]/x",4,5000);
  Response->SetParameter(0,0.976811);
  Response->SetParameter(1,14.2444);
  Response->SetParameter(2,4.47607);
  Response->SetParameter(3,18.482);
  Response->SetParameter(4,0.717231);  
  double r = Response->Eval(corPt);
  double Pt = corPt*r;
  //cout<<"Corrected jet Pt = "<<corPt<<" GeV, Response = "<<r<<", Uncorrected jet Pt = "<<Pt<<" GeV"<<endl;                  
  return Pt;
}

