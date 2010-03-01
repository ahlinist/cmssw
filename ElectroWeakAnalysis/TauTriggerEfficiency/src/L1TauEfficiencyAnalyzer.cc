#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/L1TauEfficiencyAnalyzer.h"
//#include "PhysicsTools/Utilities/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctJetCand.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloRegion.h"

#include "TF1.h"

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

// Default constructor
L1TauEfficiencyAnalyzer::L1TauEfficiencyAnalyzer()
{}


L1TauEfficiencyAnalyzer::~L1TauEfficiencyAnalyzer(){
  /*
	cout << endl;
	cout << "Events analyzed " << nEvents << endl;
	cout << endl;
  */
}

void L1TauEfficiencyAnalyzer::Setup(const edm::ParameterSet& iConfig,TTree *trigtree)
{
  PFTauCollectionSource = iConfig.getParameter<edm::InputTag>("PFTauCollection");
  L1extraTauJetSource = iConfig.getParameter<edm::InputTag>("L1extraTauJetSource");
  L1extraCentralJetSource = iConfig.getParameter<edm::InputTag>("L1extraCentralJetSource");
  L1CaloRegionSource = iConfig.getParameter<edm::InputTag>("L1CaloRegionSource");

  L1GtReadoutRecordSource = iConfig.getParameter<edm::InputTag>("L1GtReadoutRecord");
  L1GtObjectMapRecordSource = iConfig.getParameter<edm::InputTag>("L1GtObjectMapRecord");;
  HLTResultsSource = iConfig.getParameter<edm::InputTag>("HltResults");;
  
  L1TauTriggerSource = iConfig.getParameter<edm::InputTag>("L1TauTriggerSource");

  jetMatchingCone = iConfig.getParameter<double>("L1JetMatchingCone");
  isolationThreshold = iConfig.getParameter<unsigned>("L1IsolationThreshold");

  l1tree = trigtree;

  // Setup branches
  l1tree->Branch("L1JetPt", &jetPt, "L1JetPt/F");
  l1tree->Branch("L1JetEt", &jetPt, "L1JetEt/F");
  l1tree->Branch("L1JetEta", &jetEta, "L1JetEta/F");
  l1tree->Branch("L1JetPhi", &jetPhi, "L1JetPhi/F");
  l1tree->Branch("L1JetMatchDR", &jetMinDR);
  l1tree->Branch("L1TauVeto", &hasTauVeto);
  l1tree->Branch("L1IsolationRegions", &l1Isolation);
  l1tree->Branch("hasMatchedL1Jet", &hasL1Jet, "hasMatchedL1Jet/B");
  l1tree->Branch("hasMatchedL1TauJet", &hasL1TauJet, "hasMatchedL1TauJet/B");
  l1tree->Branch("hasMatchedL1CenJet", &hasL1CenJet, "hasMatchedL1CenJet/B");

  l1tree->Branch("hasTriggeredAndMatchedL1TauJet", &hasTriggeredL1TauJet, "hasTriggeredAndMatchedL1TauJet/B");
  l1tree->Branch("hasTriggeredAndMatchedL1CenJet", &hasTriggeredL1CenJet, "hasTriggeredAndMatchedL1CenJet/B");


  _L1EvtCnt = 0;
  _HltEvtCnt = 0;
  _l1Flag = new bool[128]; 
  _hltFlag = new bool[512]; 
}

void L1TauEfficiencyAnalyzer::fill(const edm::Event& iEvent, const reco::GsfElectron& tau) {
  fill(iEvent,tau.p4());
}

void L1TauEfficiencyAnalyzer::fill(const edm::Event& iEvent, const reco::PFTau& tau) {
	fill(iEvent,tau.p4());
}

void L1TauEfficiencyAnalyzer::fill(const edm::Event& iEvent, const reco::CaloTau& tau) {
        fill(iEvent,tau.p4());
}

void L1TauEfficiencyAnalyzer::fill(const edm::Event& iEvent, const LorentzVector& tau) {

  // Reset variables
  jetPt = 0.0;
  jetEt = 0.0;
  jetUncorrEt = 0.0;
  jetEta = 0.0;
  jetPhi = -999.0;
  jetMinDR = 0.0;
  hasL1Jet = 0;
  hasL1TauJet = 0;
  hasL1CenJet = 0;
  hasTauVeto = false;
  l1Isolation = 0;
  hasTriggeredL1TauJet = 0;
  hasTriggeredL1CenJet = 0;

  unsigned jetRegionId = 0;

  // Get data from event 
  Handle<L1JetParticleCollection> l1TauHandle;
  iEvent.getByLabel(L1extraTauJetSource, l1TauHandle);
  if(!l1TauHandle.isValid()) {
    std::cout << "%L1TauEffAnalyzer -- No L1extra TauJet found! " << std::endl;
    return;
  }

  Handle<L1JetParticleCollection> l1CentralJetHandle;
  iEvent.getByLabel(L1extraCentralJetSource, l1CentralJetHandle);
  if(!l1CentralJetHandle.isValid()) {
    std::cout << "%L1TauEffAnalyzer -- No L1extra central jet found! " << std::endl;
    return;
  }

  // Process L1 triggered taus
  const L1JetParticleCollection & l1Taus = *(l1TauHandle.product());
  L1JetParticleCollection::const_iterator iJet;

  // Match for PF tau and L1extra tau
  jetMinDR = 99999999.;
  for(iJet = l1Taus.begin(); iJet != l1Taus.end(); ++iJet) {
    //if(iJet->et() <= 5.)
    //  continue;

    double DR = deltaR(iJet->eta(), iJet->phi(), tau.Eta(), tau.Phi());
    if(DR < jetMatchingCone && DR < jetMinDR) {
      jetMinDR = DR;
      jetPt = iJet->pt();
      jetEt = iJet->et();
      jetUncorrEt = L1JetEtUncorr(jetEt);
      jetEta = iJet->eta();
      jetPhi = iJet->phi();
      hasL1Jet = 1;
      hasL1TauJet = 1;
      jetRegionId = iJet->gctJetCand()->regionId().rawId();
      /*
      std::cout << "L1Analyzer " << __LINE__ << ": " << iJet->gctJetCand()->regionId().rawId()
                << " etaIndex " << iJet->gctJetCand()->etaIndex() << " phiIndex " << iJet->gctJetCand()->phiIndex()
                << std::endl;
      */                
    }
  }

  // If not found, match PF tau and L1extra central jet
  if(!hasL1Jet) {
    for(iJet = l1CentralJetHandle->begin(); iJet != l1CentralJetHandle->end(); ++iJet) {
      //if(iJet->et() <= 5.)
      //  continue;

      double DR = deltaR(iJet->eta(), iJet->phi(), tau.Eta(), tau.Phi());
      if(DR < jetMatchingCone && DR < jetMinDR) {
        jetMinDR = DR;
        jetPt = iJet->pt();
        jetEt = iJet->et();
	jetUncorrEt = L1JetEtUncorr(jetEt);
        jetEta = iJet->eta();
        jetPhi = iJet->phi();
        hasL1Jet = 1;
        hasL1CenJet = 1;
        jetRegionId = iJet->gctJetCand()->regionId().rawId();
        /*
        std::cout << "L1Analyzer " << __LINE__ << ": " << iJet->gctJetCand()->regionId().rawId() 
                  << " etaIndex " << iJet->gctJetCand()->etaIndex() << " phiIndex " << iJet->gctJetCand()->phiIndex()
                  << std::endl;
        */
      }
    }
  }

  if(hasL1Jet)
    fillCaloRegion(iEvent, jetRegionId);
      

  // Store L1 trigger bits
  edm::Handle<L1GlobalTriggerReadoutRecord>      l1GTRR;
  edm::Handle<L1GlobalTriggerObjectMapRecord>    l1GTOMRec;

  iEvent.getByLabel(L1GtReadoutRecordSource,l1GTRR);
  if(!l1GTRR.isValid()) {
    std::cout << "%L1TauEffAnalyzer -- No L1GlobalTriggerReadouRecord found! " << std::endl;
    return;
  }

  iEvent.getByLabel(L1GtObjectMapRecordSource,l1GTOMRec);
  if(!l1GTOMRec.isValid()) {
    std::cout << "%L1TauEffAnalyzer -- No L1GlobalTriggerObjectMapRecord found! " << std::endl;
    return;
  }

  TString algoBitToName[128];
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
        algoBitToName[itrig] = TString( (*itMap).algoName() );
        
        l1tree->Branch(algoBitToName[itrig],_l1Flag+itrig,algoBitToName[itrig]+"/B");
      }
      _L1EvtCnt++;
    }
    for (unsigned int iBit = 0; iBit < numberTriggerBits; ++iBit) {     
      // ...Fill the corresponding accepts in branch-variables
      _l1Flag[iBit] = (bool)gtDecisionWord[iBit];
      //std::cout << "L1 TD: "<<iBit<<" "<<algoBitToName[iBit]<<" "<<gtDecisionWord[iBit]<< std::endl;
    }
  }


  // Store HLT trigger bits
  edm::Handle<edm::TriggerResults> hltresults;
  iEvent.getByLabel(HLTResultsSource,hltresults);
  if(!hltresults.isValid()) {
    std::cout << "%L1TauEffAnalyzer -- No HltResults found! " << std::endl;
    return;
  }

  if (hltresults.isValid()) {
    int ntrigs = hltresults->size();
    _triggerNames.init(* hltresults);

    // 1st event : Book as many branches as trigger paths provided in the input...
    if (_HltEvtCnt==0){
      for (int itrig = 0; itrig != ntrigs; ++itrig) {
        TString trigName = _triggerNames.triggerName(itrig);
        l1tree->Branch(trigName,_hltFlag+itrig,trigName+"/B");
      }
      _HltEvtCnt++;
    }
    // ...Fill the corresponding accepts in branch-variables
    for (int itrig = 0; itrig != ntrigs; ++itrig){
      string trigName=_triggerNames.triggerName(itrig);
      bool accept = hltresults->accept(itrig);

      _hltFlag[itrig] = accept;
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
	  hasTriggeredL1TauJet = 1;
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
	  hasTriggeredL1CenJet = 1;
	  //std::cout<<"Found triggered jet: "<<jetCandRefVec[iL1Jet]->et()
	  //   <<" "<<jetCandRefVec[iL1Jet]->eta()
	  //   <<" "<<jetCandRefVec[iL1Jet]->phi()<<std::endl;
	}
      }
    }

  }


} 

void L1TauEfficiencyAnalyzer::fillCaloRegion(const edm::Event& iEvent, unsigned regionId) {
  edm::Handle<std::vector<L1CaloRegion> > caloRegionHandle;
  if(!iEvent.getByLabel(L1CaloRegionSource, caloRegionHandle)) {
    std::cout << "No L1CaloRegion! with label " << L1CaloRegionSource << std::endl;
    return;
  }

  if(caloRegionHandle->size() == 0) {
    std::cout << "L1CaloRegion size is zero!" << std::endl;
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
      /*
      std::cout << "L1Analyzer " << __LINE__ << ": " << iter->id().rawId()
                << " etaIndex " << iter->gctEta() << " phiIndex " << iter->gctPhi()
                << " et " << iter->et()
                << " tauVeto " << iter->tauVeto()
                << std::endl;
      */
      found = iter;
    }
  }
  if(found == caloRegionHandle->end()) {
    std::cout << "Didn't find matching L1CaloRegion" << std::endl;
    return;
  }

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

  for(iter = caloRegionHandle->begin(); iter != caloRegionHandle->end(); ++iter) {
    if(((iter->gctEta() == nextEta || iter->gctEta() == prevEta) &&
        (iter->gctPhi() == nextPhi || iter->gctPhi() == prevPhi)) ||
       (iter->gctEta() == eta && (iter->gctPhi() == nextPhi || iter->gctPhi() == prevPhi)) ||
       (iter->gctPhi() == phi && (iter->gctEta() == nextEta || iter->gctEta() == prevEta))) {
      /*
      std::cout << "L1Analyzer " << __LINE__ << ": " << iter->id().rawId()
                << " etaIndex " << iter->gctEta() << " phiIndex " << iter->gctPhi()
                << " et " << iter->et()
                << " tauVeto " << iter->tauVeto()
                << std::endl;
      */
      if(iter->et() < isolationThreshold)
        ++l1Isolation;
      hasTauVeto = hasTauVeto || found->tauVeto();
    }
  }
  //std::cout << "L1Analyzer " << __LINE__ << ": L1 isolation " << l1Isolation << " (number of adjacent cells with Et < " << isolationThreshold << " GeV; should be 7/8 for tau)" << std::endl;
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

