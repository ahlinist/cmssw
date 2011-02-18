#include "TauAnalysis/Core/plugins/GenTauHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "PhysicsTools/JetMCUtils/interface/JetMCTag.h"

#include "TauAnalysis/GenSimTools/interface/genParticleAuxFunctions.h"
#include "TauAnalysis/CandidateTools/interface/candidateAuxFunctions.h"
#include "TauAnalysis/Core/interface/histManagerAuxFunctions.h"

#include <TMath.h>

#include <stdlib.h>

GenTauHistManager::GenTauHistManager(const edm::ParameterSet& cfg)
  : HistManagerBase(cfg)
{
  //std::cout << "<GenTauHistManager::GenTauHistManager>:" << std::endl;

  genParticleSrc_ = cfg.getParameter<edm::InputTag>("genParticleSource");
  //std::cout << " genParticleSrc = " << genParticleSrc_ << std::endl;
}

GenTauHistManager::~GenTauHistManager()
{
// nothing to be done yet...
}

void GenTauHistManager::bookHistogramsImp()
{
  //std::cout << "<GenTauHistManager::bookHistogramsImp>:" << std::endl;
 
  hNumGenTaus_ = book1D("NumGenTaus", "gen. Tau Leptons in Event", 10, -0.5, 9.5);

  hGenTauPt_ = book1D("GenTauPt", "gen. Tau Lepton P_{T}", 75, 0., 150.);
  hGenTauEta_ = book1D("GenTauEta", "gen. Tau Lepton #eta", 100, -5., +5.);
  hGenTauPtVsEta_ = book2D("TauPtVsEta", "gen. Tau Lepton P_{T} vs. #eta", 40, -5., +5., 30, 0., 150.);
  hGenTauPhi_ = book1D("GenTauPhi", "gen. Tau Lepton #phi", 36, -TMath::Pi(), +TMath::Pi());

  hGenTauDecayMode_ = book1D("GenTauDecayMode", "gen. Tau Lepton decay mode", 20, -0.5, 19.5);
  setAxisLabelsGenTauDecayMode(hGenTauDecayMode_->getTH1()->GetXaxis());
  hVisEnFractionVsGenTauDecayMode_ 
    = bookProfile1D("VisEnFractionVsGenTauDecayMode", "gen. Tau Lepton vis. Energy fraction vs. decay mode", 20, -0.5, 19.5);
  setAxisLabelsGenTauDecayMode(hVisEnFractionVsGenTauDecayMode_->getTH1()->GetXaxis());

  hVisEnFractionProduct_ = book1D("VisEnFractionProduct", "VisEnFractionProduct", 200, -0.5, +1.5);

  hGenTauQEDradFraction_ = book1D("GenTauQEDradFraction", "gen. Tau Lepton QED rad. Energy fraction", 200, -0.5, +1.5);
}

const reco::GenParticle* getGenDecayingTauLepton(const reco::GenParticle* genParticle)
{
  unsigned numGenDaughters = genParticle->numberOfDaughters();
  for ( unsigned iGenDaughter = 0; iGenDaughter < numGenDaughters; ++iGenDaughter ) {
    const reco::GenParticle* genDaughter = genParticle->daughterRef(iGenDaughter).get();

    if ( TMath::Abs(genDaughter->pdgId()) == 15 ) return getGenDecayingTauLepton(genDaughter);
  }

  return genParticle;
}



void GenTauHistManager::fillHistogramsImp(const edm::Event& evt, const edm::EventSetup& es, double evtWeight)
{
  //std::cout << "<GenTauHistManager::fillHistogramsImp>:" << std::endl;

  edm::Handle<reco::GenParticleCollection> genParticles;
  evt.getByLabel(genParticleSrc_, genParticles);

  unsigned numGenTauLeptons = 0;

  double genVisEnergyFractionTauPlus  = -1.;
  double genVisEnergyFractionTauMinus = -1.;

  for ( reco::GenParticleCollection::const_iterator genParticle = genParticles->begin();
	genParticle != genParticles->end(); ++genParticle ) {

//--- skip all particles other than tau leptons
    if ( TMath::Abs(genParticle->pdgId()) != 15 ) continue; 

//--- skip "secondary" tau leptons from tau --> tau + gamma radiation
//    present in HepEVT record
    if ( findMotherWithPdgId(&(*genParticle), 15) != 0 ) continue;
    
    ++numGenTauLeptons;
    
    hGenTauPt_->Fill(genParticle->pt(), evtWeight);
    hGenTauEta_->Fill(genParticle->eta(), evtWeight);
    hGenTauPtVsEta_->Fill(genParticle->eta(), genParticle->pt(), evtWeight);
    hGenTauPhi_->Fill(genParticle->phi(), evtWeight);

//--- in case of tau leptons which emmit tau --> tau + gamma radiation,
//    traverse to decaying tau lepton, in order not to confuse decay mode determination
//    by presence of additional gammas
    const reco::GenParticle* genDecayingTauLepton = getGenDecayingTauLepton(&(*genParticle));
    std::vector<const reco::GenParticle*> genDecayProducts;
    findDaughters(&(*genParticle), genDecayProducts);
    std::string genTauDecayMode = getGenTauDecayMode(&(*genParticle));

    hGenTauDecayMode_->getTH1()->Fill(genTauDecayMode.data(), evtWeight);

    reco::Candidate::LorentzVector genVisMomentum = getVisMomentum(genDecayProducts);
    if ( genDecayingTauLepton->energy() > 0. ) {
      double genVisEnergyFraction = genVisMomentum.energy()/genDecayingTauLepton->energy();

      hVisEnFractionVsGenTauDecayMode_->getTProfile()->Fill(genTauDecayMode.data(), genVisEnergyFraction, evtWeight);

      if      ( genParticle->pdgId() == -15 ) genVisEnergyFractionTauPlus  = genVisEnergyFraction;
      else if ( genParticle->pdgId() == +15 ) genVisEnergyFractionTauMinus = genVisEnergyFraction;
    }

    if ( genParticle->energy() > 0. ) hGenTauQEDradFraction_->Fill(1. - genDecayingTauLepton->energy()/genParticle->energy(), evtWeight);
  }

  hNumGenTaus_->Fill(numGenTauLeptons, evtWeight);

  if ( numGenTauLeptons == 2 && genVisEnergyFractionTauPlus >= 0. && genVisEnergyFractionTauMinus >= 0. ) {
    hVisEnFractionProduct_->Fill(genVisEnergyFractionTauPlus*genVisEnergyFractionTauMinus, evtWeight);
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, GenTauHistManager, "GenTauHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, GenTauHistManager, "GenTauHistManager");

#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<GenTauHistManager> GenTauAnalyzer;

DEFINE_FWK_MODULE(GenTauAnalyzer);
