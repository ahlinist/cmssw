#include "TauAnalysis/Core/plugins/ZllHypothesisT1T2HistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/PatCandidates/interface/Tau.h"

#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"

#include "TauAnalysis/Core/interface/histManagerAuxFunctions.h"

#include <TMath.h>

//
//-----------------------------------------------------------------------------------------------------------------------
//

template<typename T1, typename T2>
ZllHypothesisT1T2HistManager<T1,T2>::ZllHypothesisT1T2HistManager(const edm::ParameterSet& cfg)
  : HistManagerBase(cfg)
{
  //std::cout << "<ZllHypothesisT1T2HistManager::ZllHypothesisT1T2HistManager>:" << std::endl;

  ZllHypothesisSrc_ = cfg.getParameter<edm::InputTag>("ZllHypothesisSource");
  //std::cout << " ZllHypothesisSrc = " << ZllHypothesis_ << std::endl;

  lepton1WeightExtractors_ = getTauJetWeightExtractors<T1>(cfg, "lepton1WeightSource");
  lepton2WeightExtractors_ = getTauJetWeightExtractors<T2>(cfg, "lepton2WeightSource");

  std::string normalization_string = cfg.getParameter<std::string>("normalization");
  normMethod_ = getNormMethod(normalization_string, "diTauCandidates");
}

template<typename T1, typename T2>
ZllHypothesisT1T2HistManager<T1,T2>::~ZllHypothesisT1T2HistManager()
{
  for ( typename std::vector<FakeRateJetWeightExtractor<T1>*>::iterator it = lepton1WeightExtractors_.begin();
	it != lepton1WeightExtractors_.end(); ++it ) {
    delete (*it);
  }
  
  for ( typename std::vector<FakeRateJetWeightExtractor<T2>*>::iterator it = lepton2WeightExtractors_.begin();
	it != lepton2WeightExtractors_.end(); ++it ) {
    delete (*it);
  }
}

template<typename T1, typename T2>
void ZllHypothesisT1T2HistManager<T1,T2>::bookHistogramsImp()
{
  //std::cout << "<ZllHypothesisT1T2HistManager::bookHistogramsImp>:" << std::endl;

  hGenLepton1Pt_ = book1D("GenLepton1Pt", "gen. P_{T}^{#ell1}", 75, 0., 150.);
  hGenLepton1Eta_ = book1D("GenLepton1Eta", "gen. #eta_{#ell1}", 60, -3., +3.);
  hGenLepton1Phi_ = book1D("GenLepton1Phi", "gen. #phi_{#ell1}", 36, -TMath::Pi(), +TMath::Pi());

  hGenLepton2Pt_ = book1D("GenLepton2Pt", "gen. P_{T}^{#ell2}", 75, 0., 150.);
  hGenLepton2Eta_ = book1D("GenLepton2Eta", "gen. #eta_{#ell2}", 60, -3., +3.);
  hGenLepton2Phi_ = book1D("GenLepton2Phi", "gen. #phi_{#ell2}", 36, -TMath::Pi(), +TMath::Pi());

  hGenVisMass_ = book1D("GenVisMass", "gen. Z^{0} Mass", 40, 0., 200.);

  hLepton1bestMatchPt_ = book1D("Lepton1bestMatchPt", "P_{T} of rec. Object best matching #ell_{1}", 75, 0., 150.);
  hLepton1bestMatchEta_= book1D("Lepton1bestMatchEta", "#eta of rec. Object best matching #ell_{1}", 60, -3., +3.);
  hLepton1bestMatchPhi_ = book1D("Lepton1bestMatchPhi", "#phi of rec. Object best matching #ell_{1}", 36, -TMath::Pi(), +TMath::Pi());
  hLepton1bestMatchType_ = book1D("Lepton1bestMatchType", "Type of rec. Object best matching #ell_{1}", 10, -0.5, 9.5);

  hLepton2bestMatchPt_ = book1D("Lepton2bestMatchPt", "P_{T} of rec. Object best matching #ell_{2}", 75, 0., 150.);
  hLepton2bestMatchEta_= book1D("Lepton2bestMatchEta", "#eta of rec. Object best matching #ell_{2}", 60, -3., +3.);
  hLepton2bestMatchPhi_ = book1D("Lepton2bestMatchPhi", "#phi of rec. Object best matching #ell_{2}", 36, -TMath::Pi(), +TMath::Pi());
  hLepton2bestMatchType_ = book1D("Lepton2bestMatchType", "Type of rec. Object best matching #ell_{2}", 10, -0.5, 9.5);

  hLepton1bestMatchPtRes_ = book1D("Lepton1bestMatchPtRes", "gen. P_{T}^{#ell1} - P_{T} of best matching rec. Object", 50, -25., +25.);
  hLepton1bestMatchEtaRes_ = book1D("Lepton1bestMatchEtaRes", "gen. #eta_{#ell1} - #eta of best matching rec. Object", 100, -0.25, +0.25);
  hLepton1bestMatchPhiRes_ = book1D("Lepton1bestMatchPhiRes", "gen. #phi_{#ell1} - #phi of best matching rec. Object", 100, -0.25, +0.25);

  hLepton2bestMatchPtRes_ = book1D("Lepton2bestMatchPtRes", "gen. P_{T}^{#ell2} - P_{T} of best matching rec. Object", 50, -25., +25.);
  hLepton2bestMatchEtaRes_ = book1D("Lepton2bestMatchEtaRes", "gen. #eta_{#ell2} - #eta of best matching rec. Object", 100, -0.25, +0.25);
  hLepton2bestMatchPhiRes_ = book1D("Lepton2bestMatchPhiRes", "gen. #phi_{#ell2} - #phi of best matching rec. Object", 100, -0.25, +0.25);

  hVisMassBestMach_ = book1D("VisMassBestMach", "Z #rightarrow #ell^{+} #ell^{-} Mass hypothesis", 40, 0., 200.);

  hVisMassFromCaloJets_ = book1D("VisMassFromCaloJets", "hypothetic Z^{0} Mass from calo. Jets", 40, 0., 200.);
  hVisMassFromPFJets_ = book1D("VisMassFromPFJets", "hypothetic Z^{0} Mass from particle-flow Jets", 40, 0., 200.);
  hVisMassFromTracks_ = book1D("VisMassFromTracks", "hypothetic Z^{0} Mass from Tracks", 40, 0., 200.);
  hVisMassFromGsfElectrons_ = book1D("VisMassFromGsfElectrons", "hypothetic Z^{0} Mass from GSF Electrons", 40, 0., 200.);
  hVisMassFromGsfTracks_ = book1D("VisMassFromGsfTracks", "hypothetic Z^{0} Mass from GSF Tracks", 40, 0., 200.);

  bookWeightHistograms(*dqmStore_, "ZllHypothesisWeight", "ZllHypothesis Weight", 
		       hZllHypothesisWeightPosLog_, hZllHypothesisWeightNegLog_, hZllHypothesisWeightZero_,
		       hZllHypothesisWeightLinear_);
}

template<typename T1, typename T2>
double ZllHypothesisT1T2HistManager<T1,T2>::getZllHypothesisWeight(const ZllHypothesisT1T2<T1,T2>& ZllHypothesis)
{
  double lepton1Weight = getTauJetWeight<T1>(*ZllHypothesis.diCandidatePair()->leg1(), lepton1WeightExtractors_);
  double lepton2Weight = getTauJetWeight<T2>(*ZllHypothesis.diCandidatePair()->leg2(), lepton2WeightExtractors_);
  return (lepton1Weight*lepton2Weight);
}

template<typename T1, typename T2>
void ZllHypothesisT1T2HistManager<T1,T2>::fillHistogramsImp(const edm::Event& evt, const edm::EventSetup& es, double evtWeight)
{  
  //std::cout << "<ZllHypothesisT1T2HistManager::fillHistogramsImp>:" << std::endl; 

  typedef std::vector<ZllHypothesisT1T2<T1,T2> > ZllHypothesisCollection;
  edm::Handle<ZllHypothesisCollection> ZllHypotheses;
  evt.getByLabel(ZllHypothesisSrc_, ZllHypotheses);

  double ZllHypothesisWeightSum = 0.;
  for ( typename ZllHypothesisCollection::const_iterator ZllHypothesis = ZllHypotheses->begin();
	ZllHypothesis != ZllHypotheses->end(); ++ZllHypothesis ) {
    ZllHypothesisWeightSum += getZllHypothesisWeight(*ZllHypothesis);
  }

  for ( typename ZllHypothesisCollection::const_iterator ZllHypothesis = ZllHypotheses->begin();
	ZllHypothesis != ZllHypotheses->end(); ++ZllHypothesis ) {

    double ZllHypothesisWeight = getZllHypothesisWeight(*ZllHypothesis);
    double weight = getWeight(evtWeight, ZllHypothesisWeight, ZllHypothesisWeightSum);

    hLepton1bestMatchPt_->Fill(ZllHypothesis->p4Lepton1bestMatch().pt(), weight);
    hLepton1bestMatchEta_->Fill(ZllHypothesis->p4Lepton1bestMatch().eta(), weight);
    hLepton1bestMatchPhi_->Fill(ZllHypothesis->p4Lepton1bestMatch().phi(), weight);
    hLepton1bestMatchType_->Fill(ZllHypothesis->typeLepton1bestMatch(), weight);

    hLepton2bestMatchPt_->Fill(ZllHypothesis->p4Lepton2bestMatch().pt(), weight);
    hLepton2bestMatchEta_->Fill(ZllHypothesis->p4Lepton2bestMatch().eta(), weight);
    hLepton2bestMatchPhi_->Fill(ZllHypothesis->p4Lepton2bestMatch().phi(), weight);
    hLepton2bestMatchType_->Fill(ZllHypothesis->typeLepton2bestMatch(), weight);

    if ( ZllHypothesis->genLepton1().isAvailable() ) {
      hGenLepton1Pt_->Fill(ZllHypothesis->genLepton1()->pt(), weight);
      hGenLepton1Eta_->Fill(ZllHypothesis->genLepton1()->eta(), weight);
      hGenLepton1Phi_->Fill(ZllHypothesis->genLepton1()->phi(), weight);

      hLepton1bestMatchPtRes_->Fill(ZllHypothesis->genLepton1()->pt() - ZllHypothesis->p4Lepton1bestMatch().pt(), weight);
      hLepton1bestMatchEtaRes_->Fill(ZllHypothesis->genLepton1()->eta() - ZllHypothesis->p4Lepton1bestMatch().eta(), weight);
      hLepton1bestMatchPhiRes_->Fill(ZllHypothesis->genLepton1()->phi() - ZllHypothesis->p4Lepton1bestMatch().phi(), weight);
    }

    if ( ZllHypothesis->genLepton2().isAvailable() ) {
      hGenLepton2Pt_->Fill(ZllHypothesis->genLepton2()->pt(), weight);
      hGenLepton2Eta_->Fill(ZllHypothesis->genLepton2()->eta(), weight);
      hGenLepton2Phi_->Fill(ZllHypothesis->genLepton2()->phi(), weight);

      hLepton2bestMatchPtRes_->Fill(ZllHypothesis->genLepton2()->pt() - ZllHypothesis->p4Lepton2bestMatch().pt(), weight);
      hLepton2bestMatchEtaRes_->Fill(ZllHypothesis->genLepton2()->eta() - ZllHypothesis->p4Lepton2bestMatch().eta(), weight);
      hLepton2bestMatchPhiRes_->Fill(ZllHypothesis->genLepton2()->phi() - ZllHypothesis->p4Lepton2bestMatch().phi(), weight);
    }

    if ( ZllHypothesis->genLepton1().isAvailable() &&
	 ZllHypothesis->genLepton2().isAvailable() ) {
      hGenVisMass_->Fill((ZllHypothesis->genLepton1()->p4() + ZllHypothesis->genLepton2()->p4()).mass(), weight);
    }

    hVisMassBestMach_->Fill(ZllHypothesis->p4ZbestMatch().mass(), weight);

    if ( ZllHypothesis->lepton1matchedCaloJet().isAvailable() &&
	 ZllHypothesis->lepton2matchedCaloJet().isAvailable() ) {
      hVisMassFromCaloJets_->Fill((ZllHypothesis->lepton1matchedCaloJet()->p4() + ZllHypothesis->lepton2matchedCaloJet()->p4()).mass(), weight);
    }

    if ( ZllHypothesis->lepton1matchedPFJet().isAvailable() &&
	 ZllHypothesis->lepton2matchedPFJet().isAvailable() ) {
      hVisMassFromPFJets_->Fill((ZllHypothesis->lepton1matchedPFJet()->p4() + ZllHypothesis->lepton2matchedPFJet()->p4()).mass(), weight);
    }

    if ( ZllHypothesis->lepton1matchedTrack().isAvailable() &&
	 ZllHypothesis->lepton2matchedTrack().isAvailable() ) {
      const edm::Ptr<reco::Track> track1 = ZllHypothesis->lepton1matchedTrack();
      reco::Particle::LorentzVector track1Momentum(track1->px(), track1->py(), track1->pz(), track1->p());
      const edm::Ptr<reco::Track> track2 = ZllHypothesis->lepton2matchedTrack();
      reco::Particle::LorentzVector track2Momentum(track2->px(), track2->py(), track2->pz(), track2->p());
      hVisMassFromTracks_->Fill((track1Momentum + track2Momentum).mass(), weight);
    }

    if ( ZllHypothesis->lepton1matchedGsfElectron().isAvailable() &&
	 ZllHypothesis->lepton2matchedGsfElectron().isAvailable() ) {
      hVisMassFromCaloJets_->Fill((ZllHypothesis->lepton1matchedGsfElectron()->p4() + ZllHypothesis->lepton2matchedGsfElectron()->p4()).mass(), weight);
    }

    if ( ZllHypothesis->lepton1matchedGsfTrack().isAvailable() &&
	 ZllHypothesis->lepton2matchedGsfTrack().isAvailable() ) {
      const edm::Ptr<reco::Track> gsfTrack1 = ZllHypothesis->lepton1matchedGsfTrack();
      reco::Particle::LorentzVector gsfTrack1Momentum(gsfTrack1->px(), gsfTrack1->py(), gsfTrack1->pz(), gsfTrack1->p());
      const edm::Ptr<reco::Track> gsfTrack2 = ZllHypothesis->lepton2matchedGsfTrack();
      reco::Particle::LorentzVector gsfTrack2Momentum(gsfTrack2->px(), gsfTrack2->py(), gsfTrack2->pz(), gsfTrack2->p());
      hVisMassFromTracks_->Fill((gsfTrack1Momentum + gsfTrack2Momentum).mass(), weight);
    }

    fillWeightHistograms(hZllHypothesisWeightPosLog_, hZllHypothesisWeightNegLog_, hZllHypothesisWeightZero_, 
			 hZllHypothesisWeightLinear_, ZllHypothesisWeight);
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

typedef ZllHypothesisT1T2HistManager<pat::Electron, pat::Tau> ZllHypothesisElecTauHistManager;
typedef ZllHypothesisT1T2HistManager<pat::Muon, pat::Tau> ZllHypothesisMuTauHistManager;
typedef ZllHypothesisT1T2HistManager<pat::Tau, pat::Tau> ZllHypothesisDiTauHistManager;
typedef ZllHypothesisT1T2HistManager<pat::Electron, pat::Muon> ZllHypothesisElecMuHistManager;

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, ZllHypothesisElecTauHistManager, "ZllHypothesisElecTauHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, ZllHypothesisElecTauHistManager, "ZllHypothesisElecTauHistManager");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, ZllHypothesisMuTauHistManager, "ZllHypothesisMuTauHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, ZllHypothesisMuTauHistManager, "ZllHypothesisMuTauHistManager");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, ZllHypothesisDiTauHistManager, "ZllHypothesisDiTauHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, ZllHypothesisDiTauHistManager, "ZllHypothesisDiTauHistManager");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, ZllHypothesisElecMuHistManager, "ZllHypothesisElecMuHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, ZllHypothesisElecMuHistManager, "ZllHypothesisElecMuHistManager");
  
#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<ZllHypothesisElecTauHistManager> ZllHypothesisElecTauAnalyzer;
typedef HistManagerAdapter<ZllHypothesisMuTauHistManager> ZllHypothesisMuTauAnalyzer;
typedef HistManagerAdapter<ZllHypothesisDiTauHistManager> ZllHypothesisDiTauAnalyzer;
typedef HistManagerAdapter<ZllHypothesisElecMuHistManager> ZllHypothesisElecMuAnalyzer;

DEFINE_FWK_MODULE(ZllHypothesisElecTauAnalyzer);
DEFINE_FWK_MODULE(ZllHypothesisMuTauAnalyzer);
DEFINE_FWK_MODULE(ZllHypothesisDiTauAnalyzer);
DEFINE_FWK_MODULE(ZllHypothesisElecMuAnalyzer);
