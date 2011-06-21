#include "TauAnalysis/Core/plugins/VBFCompositePtrCandidateT1T2MEtEventT3HistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DQMServices/Core/interface/DQMStore.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/Candidate/interface/Candidate.h" 

#include "PhysicsTools/JetMCUtils/interface/JetMCTag.h"

#include "AnalysisDataFormats/TauAnalysis/interface/VBFCompositePtrCandidateT1T2MEtEventT3.h"
#include "AnalysisDataFormats/TauAnalysis/interface/VBFCompositePtrCandidateT1T2MEtEventT3Fwd.h"
#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"

#include "TauAnalysis/Core/interface/histManagerAuxFunctions.h"
//#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"

#include <TMath.h>

const double epsilon = 0.01;

template<typename T1, typename T2>
bool matchesGenCandidatePair(const CompositePtrCandidateT1T2MEt<T1,T2>& compositePtrCandidate)
{
  bool isGenMatched = false;
// not implemented yet...
  return isGenMatched;
}

template<typename T1, typename T2, typename T3>
VBFCompositePtrCandidateT1T2MEtEventT3HistManager<T1,T2,T3>::VBFCompositePtrCandidateT1T2MEtEventT3HistManager(
  const edm::ParameterSet& cfg)
  : HistManagerBase(cfg)
{
  //std::cout << "<VBFCompositePtrCandidateT1T2MEtEventT3HistManager::VBFCompositePtrCandidateT1T2MEtEventT3HistManager>:" << std::endl;

  vbfEventSrc_ = cfg.getParameter<edm::InputTag>("vbfEventSource");
  //std::cout << " vbfEventSrc = " << vbfEventSrc_.label() << std::endl;

  diTauLeg1WeightExtractors_ = getTauJetWeightExtractors<T1>(cfg, "diTauLeg1WeightSource");
  diTauLeg2WeightExtractors_ = getTauJetWeightExtractors<T2>(cfg, "diTauLeg2WeightSource");

  requireGenMatch_ = cfg.getParameter<bool>("requireGenMatch");
  //std::cout << " requireGenMatch = " << requireGenMatch_ << std::endl;

  std::string normalization_string = cfg.getParameter<std::string>("normalization");
  normMethod_ = getNormMethod(normalization_string, "diTauCandidates");
}

template<typename T1, typename T2, typename T3>
VBFCompositePtrCandidateT1T2MEtEventT3HistManager<T1,T2,T3>::~CompositePtrCandidateT1T2MEtNSVfitHistManager()
{
  for ( typename std::vector<FakeRateJetWeightExtractor<T1>*>::iterator it = diTauLeg1WeightExtractors_.begin();
	it != diTauLeg1WeightExtractors_.end(); ++it ) {
    delete (*it);
  }

  for ( typename std::vector<FakeRateJetWeightExtractor<T2>*>::iterator it = diTauLeg2WeightExtractors_.begin();
	it != diTauLeg2WeightExtractors_.end(); ++it ) {
    delete (*it);
  }
}

template<typename T1, typename T2, typename T3>
void VBFCompositePtrCandidateT1T2MEtEventT3HistManager<T1,T2,T3>::bookHistogramsImp()
{
  //std::cout << "<CompositePtrCandidateT1T2MEtNSVfitHistManager::bookHistogramsImp>:" << std::endl;

  hTagJet1Pt_     = book1D("TagJet1Pt",     "TagJet1Pt",      30,           0.,         150.);
  hTagJet1Eta_    = book1D("TagJet1Eta",    "TagJet1Eta",    120,         -6.0,         +6.0);
  hTagJet1Phi_    = book1D("TagJet1Phi",    "TagJet1Phi",     36, -TMath::Pi(), +TMath::Pi());
  
  hTagJet2Pt_     = book1D("TagJet2Pt",     "TagJet2Pt",      50,           0.,         150.);
  hTagJet2Eta_    = book1D("TagJet2Eta",    "TagJet2Eta",    120,         -6.0,         +6.0);
  hTagJet2Phi_    = book1D("TagJet2Phi",    "TagJet2Phi",     36, -TMath::Pi(), +TMath::Pi());
  
  hTagJetMass_    = book1D("TagJetMass",    "TagJetMass",    250,        -0.01,        2500.);
  hTagJetDEta_    = book1D("TagJetDEta",    "TagJetDEta",    120,        -0.01,         12.0);
  hTagJetDPhi_    = book1D("TagJetDPhi",    "TagJetDPhi",     36,           0.,  TMath::Pi());
  hTagJetEtaProd_ = book1D("TagJetEtaProd", "TagJetEtaProd",   3,          -1.5,        +1.5);
    
  hCentralJetPt_  = book1D("CentralJetPt",  "CentralJetPt",   30,           0.,         150.);
  hCentralJetEta_ = book1D("CentralJetEta", "CentralJetEta", 120,         -6.0,         +6.0);
  hCentralJetPhi_ = book1D("CentralJetPhi", "CentralJetPhi",  36, -TMath::Pi(), +TMath::Pi());

  bookWeightHistograms(*dqmStore_, "DiTauCandidateWeight", "Composite Weight",
		       hDiTauCandidateWeightPosLog_, hDiTauCandidateWeightNegLog_, hDiTauCandidateWeightZero_,
		       hDiTauCandidateWeightLinear_);
}

template<typename T1, typename T2, typename T3>
double VBFCompositePtrCandidateT1T2MEtEventT3HistManager<T1,T2,T3>::getDiTauCandidateWeight(
  const CompositePtrCandidateT1T2MEt<T1,T2>& diTauCandidate)
{
  double diTauLeg1Weight = getTauJetWeight<T1>(*diTauCandidate.leg1(), diTauLeg1WeightExtractors_);
  double diTauLeg2Weight = getTauJetWeight<T2>(*diTauCandidate.leg2(), diTauLeg2WeightExtractors_);
  return (diTauLeg1Weight*diTauLeg2Weight);
}

template<typename T1, typename T2, typename T3>
void VBFCompositePtrCandidateT1T2MEtEventT3HistManager<T1,T2,T3>::fillHistogramsImp(
  const edm::Event& evt, const edm::EventSetup& es, double evtWeight)
{
  //std::cout << "<VBFCompositePtrCandidateT1T2MEtEventT3HistManager::fillHistogramsImp>:" << std::endl;

  typedef std::vector<VBFCompositePtrCandidateT1T2MEtEventT3<T1,T2,T3> > VBFEventCollection;
  edm::Handle<VBFEventCollection> vbfEventHypotheses;
  getCollection(evt, vbfEventSrc_, vbfEventHypotheses);

  //std::cout << " vbfEventSrc = " << vbfEventSrc_.label() << ":"
  //	      << " vbfEventHypotheses.size = " << vbfEventHypotheses->size() << std::endl;

  double diTauCandidateWeightSum = 0.;
  for ( typename VBFEventCollection::const_iterator vbfEventHypothesis = vbfEventHypotheses->begin();
	vbfEventHypothesis != vbfEventHypotheses->end(); ++vbfEventHypothesis ) {
    if ( requireGenMatch_ && !matchesGenCandidatePair(*vbfEventHypothesis->diTau()) ) continue;

    diTauCandidateWeightSum += getDiTauCandidateWeight(*vbfEventHypothesis->diTau());
  }

  for ( typename VBFEventCollection::const_iterator vbfEventHypothesis = vbfEventHypotheses->begin();
	vbfEventHypothesis != vbfEventHypotheses->end(); ++vbfEventHypothesis ) {

    if ( requireGenMatch_ && !matchesGenCandidatePair(*vbfEventHypothesis->diTau()) ) continue;

    double diTauCandidateWeight = getDiTauCandidateWeight(*vbfEventHypothesis->diTau());
    double weight = getWeight(evtWeight, diTauCandidateWeight, diTauCandidateWeightSum);

    hTagJet1Pt_->Fill(vbfEventHypothesis->tagJet1()->pt(), weight); 
    hTagJet1Eta_->Fill(vbfEventHypothesis->tagJet1()->eta(), weight); 
    hTagJet1Phi_->Fill(vbfEventHypothesis->tagJet1()->phi(), weight); 
  
    hTagJet2Pt_->Fill(vbfEventHypothesis->tagJet2()->pt(), weight);  
    hTagJet2Eta_->Fill(vbfEventHypothesis->tagJet2()->eta(), weight);  
    hTagJet2Phi_->Fill(vbfEventHypothesis->tagJet2()->phi(), weight);  
  
    hTagJetMass_->Fill(vbfEventHypothesis->mjj(), weight);  
    hTagJetDEta_->Fill(vbfEventHypothesis->dEta(), weight);   
    hTagJetDPhi_->Fill(vbfEventHypothesis->dPhi(), weight); 
    double tagJetEtaProd = vbfEventHypothesis->tagJet1()->eta()*vbfEventHypothesis->tagJet2()->eta();
    if ( tagJetEtaProd != 0. ) tagJetEtaProd /= TMath::Abs(tagJetEtaProd);
    hTagJetEtaProd_->Fill(tagJetEtaProd, weight);  
    
    typedef edm::Ptr<T3> jetPtr;
    std::vector<jetPtr> centralJets = vbfEventHypothesis->centralJets();
    for ( typename std::vector<jetPtr>::const_iterator centralJet = centralJets.begin();
	  centralJet != centralJets.end(); ++centralJet ) {
      hCentralJetPt_->Fill((*centralJet)->pt(), weight); 
      hCentralJetEta_->Fill((*centralJet)->eta(), weight); 
      hCentralJetPhi_ ->Fill((*centralJet)->phi(), weight); 
    }
    
    fillWeightHistograms(hDiTauCandidateWeightPosLog_, hDiTauCandidateWeightNegLog_, hDiTauCandidateWeightZero_,
			 hDiTauCandidateWeightLinear_, diTauCandidateWeight);
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

typedef VBFCompositePtrCandidateT1T2MEtEventT3HistManager<pat::Electron, pat::Tau, pat::Jet> PATElecTauPairVBFEventHistManager;
typedef VBFCompositePtrCandidateT1T2MEtEventT3HistManager<pat::Muon, pat::Tau, pat::Jet> PATMuTauPairVBFEventHistManager;
typedef VBFCompositePtrCandidateT1T2MEtEventT3HistManager<pat::Tau, pat::Tau, pat::Jet> PATDiTauPairVBFEventHistManager;
typedef VBFCompositePtrCandidateT1T2MEtEventT3HistManager<pat::Electron, pat::Muon, pat::Jet> PATElecMuPairVBFEventHistManager;
typedef VBFCompositePtrCandidateT1T2MEtEventT3HistManager<pat::Electron, pat::Electron, pat::Jet> PATDiElecPairVBFEventHistManager;
typedef VBFCompositePtrCandidateT1T2MEtEventT3HistManager<pat::Muon, pat::Muon, pat::Jet> PATDiMuPairVBFEventHistManager;

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATElecTauPairVBFEventHistManager, "PATElecTauPairVBFEventHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, PATElecTauPairVBFEventHistManager, "PATElecTauPairVBFEventHistManager");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATMuTauPairVBFEventHistManager, "PATMuTauPairVBFEventHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, PATMuTauPairVBFEventHistManager, "PATMuTauPairVBFEventHistManager");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATDiTauPairVBFEventHistManager, "PATDiTauPairVBFEventHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, PATDiTauPairVBFEventHistManager, "PATDiTauPairVBFEventHistManager");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATElecMuPairVBFEventHistManager, "PATElecMuPairVBFEventHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, PATElecMuPairVBFEventHistManager, "PATElecMuPairVBFEventHistManager");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATDiElecPairVBFEventHistManager, "PATDiElecPairVBFEventHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, PATDiElecPairVBFEventHistManager, "PATDiElecPairVBFEventHistManager");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATDiMuPairVBFEventHistManager, "PATDiMuPairVBFEventHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, PATDiMuPairVBFEventHistManager, "PATDiMuPairVBFEventHistManager");

#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<PATElecTauPairVBFEventHistManager> PATElecTauPairVBFEventAnalyzer;
typedef HistManagerAdapter<PATMuTauPairVBFEventHistManager> PATMuTauPairVBFEventAnalyzer;
typedef HistManagerAdapter<PATDiTauPairVBFEventHistManager> PATDiTauPairVBFEventAnalyzer;
typedef HistManagerAdapter<PATElecMuPairVBFEventHistManager> PATElecMuPairVBFEventAnalyzer;
typedef HistManagerAdapter<PATDiElecPairVBFEventHistManager> PATDiElecPairVBFEventAnalyzer;
typedef HistManagerAdapter<PATDiMuPairVBFEventHistManager> PATDiMuPairVBFEventAnalyzer;

DEFINE_FWK_MODULE(PATElecTauPairVBFEventAnalyzer);
DEFINE_FWK_MODULE(PATMuTauPairVBFEventAnalyzer);
DEFINE_FWK_MODULE(PATDiTauPairVBFEventAnalyzer);
DEFINE_FWK_MODULE(PATElecMuPairVBFEventAnalyzer);
DEFINE_FWK_MODULE(PATDiElecPairVBFEventAnalyzer);
DEFINE_FWK_MODULE(PATDiMuPairVBFEventAnalyzer);

