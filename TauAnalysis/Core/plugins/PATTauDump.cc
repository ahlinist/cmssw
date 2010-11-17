#include "TauAnalysis/Core/plugins/PATTauDump.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

#include "TauAnalysis/Core/interface/eventDumpAuxFunctions.h"
#include "TauAnalysis/GenSimTools/interface/genParticleAuxFunctions.h"
#include "TauAnalysis/CandidateTools/interface/candidateAuxFunctions.h"

#include <TMath.h>

PATTauDump::PATTauDump(const edm::ParameterSet& cfg)
  : ObjectDumpBase(cfg),
    patTauSource_(cfg.getParameter<edm::InputTag>("tauSource")),
    genParticleSource_(cfg.getParameter<edm::InputTag>("genParticleSource"))
{
  printTauIdEfficiencies_ = cfg.exists("printTauIdEfficiencies") ?
    cfg.getParameter<bool>("printTauIdEfficiencies") : false;

  typedef std::vector<int> vint;
  skipPdgIdsGenParticleMatch_ = ( cfg.exists("skipPdgIdsGenParticleMatch") ) ?
    cfg.getParameter<vint>("skipPdgIdsGenParticleMatch") : vint();
}

PATTauDump::~PATTauDump()
{
//--- nothing to be done yet...
}

void printTauEfficiency(std::ostream& outputStream, const pat::Tau& patTau,
			const char* frTypeLabel, const char* patName, const char* tauAnalysisName)
{
  outputStream << "  " << frTypeLabel << " = "
	       << patTau.efficiency(patName).value()
             //<< " (" << patTau.efficiency(std::string("bgEstFakeRateJetWeight_").append(tauAnalysisName).data()).value() << ")"
	       << std::endl;
}

void PATTauDump::print(const edm::Event& evt, const edm::EventSetup& es) const
{
  if ( !outputStream_ ) {
    edm::LogError ("print") << " Data-member outputStream undefined --> skipping !!";
    return;
  }

  std::cout << "<PATTauDump::print>:" << std::endl;
  std::cout << " src = " << patTauSource_.label() << std::endl;

  edm::Handle<pat::TauCollection> patTaus;
  evt.getByLabel(patTauSource_, patTaus);

  edm::Handle<reco::GenParticleCollection> genParticles;
  if( genParticleSource_.label() != "") evt.getByLabel(genParticleSource_, genParticles);

  unsigned iTau = 0;
  for ( pat::TauCollection::const_iterator patTau = patTaus->begin();
	patTau != patTaus->end(); ++patTau ) {
    *outputStream_ << "Tau(" << iTau << "):" << std::endl;
    *outputStream_ << " Et = " << patTau->et() << std::endl;
    *outputStream_ << " theta = " << patTau->theta()*180./TMath::Pi()
		   << " (eta = " << patTau->eta() << ")" << std::endl;
    *outputStream_ << " phi = " << patTau->phi()*180./TMath::Pi() << std::endl;
    *outputStream_ << " charge = " << patTau->charge() << std::endl;
    *outputStream_ << " decayMode = " << getTauDecayModeName(patTau->decayMode()) << std::endl;
    *outputStream_ << " visMass = " << patTau->mass() << std::endl;
    *outputStream_ << " leading Track" << std::endl;
    printTrackInfo(patTau->leadTrack(), patTau->vertex(), true, false, outputStream_);
    *outputStream_ << " #signal Tracks = " << patTau->signalTracks().size() << std::endl;
    *outputStream_ << "(#signal PFChargedHadrons = " << patTau->signalPFChargedHadrCands().size() << ")" << std::endl;
    *outputStream_ << " tauId" << std::endl;
    *outputStream_ << "  leadingTrackFinding = " << patTau->tauID("leadingTrackFinding") << std::endl;
    *outputStream_ << "  leadingTrackPtCut = " << patTau->tauID("leadingTrackPtCut") << std::endl;

    *outputStream_ << "  trackIsolation = ";
    if (patTau->isTauIDAvailable("trackIsolation"))
      *outputStream_ << patTau->tauID("trackIsolation");
    else
      *outputStream_ << "UNAVAILABLE";
    *outputStream_ << std::endl;

    double sumPtIsolationConeTracks = 0.;
    for ( reco::TrackRefVector::const_iterator isolationTrack = patTau->isolationTracks().begin();
	  isolationTrack != patTau->isolationTracks().end(); ++isolationTrack ) {
      if ( (*isolationTrack)->pt() > 1.0 ) sumPtIsolationConeTracks += (*isolationTrack)->pt();
    }
    *outputStream_ << "  trackIsolation (from isolation cone Tracks) = " << sumPtIsolationConeTracks << std::endl;
    double sumPtIsolationConePFChargedHadrons = 0.;
    for ( reco::PFCandidateRefVector::const_iterator pfChargedHadron = patTau->isolationPFChargedHadrCands().begin();
	  pfChargedHadron != patTau->isolationPFChargedHadrCands().end(); ++pfChargedHadron ) {
      if ( (*pfChargedHadron)->pt() > 1.0 ) sumPtIsolationConePFChargedHadrons += (*pfChargedHadron)->pt();
    }
    *outputStream_ << "  trackIsolation (from isolation cone PFChargedHadrons) = " << sumPtIsolationConePFChargedHadrons << std::endl;

    *outputStream_ << "  ecalIsolation = ";
    if (patTau->isTauIDAvailable("ecalIsolation"))
      *outputStream_ << patTau->tauID("ecalIsolation");
    else
      *outputStream_ << "UNAVAILABLE";
    *outputStream_ << std::endl;

    double sumPtIsolationConePFGammas = 0.;
    for ( reco::PFCandidateRefVector::const_iterator pfGamma = patTau->isolationPFGammaCands().begin();
	  pfGamma != patTau->isolationPFGammaCands().end(); ++pfGamma ) {
      if ( (*pfGamma)->pt() > 1.5 ) sumPtIsolationConePFGammas += (*pfGamma)->pt();
    }
    *outputStream_ << "  ecalIsolation (from isolation cone PFGammas) = " << sumPtIsolationConePFGammas << std::endl;
    *outputStream_ << "  pfCandidateIsolation: Pt = " << patTau->particleIso() << ", "
		   << " #particles = " << patTau->isolationPFCands().size() << std::endl;
    *outputStream_ << "  pfChargedHadronIsolation: Pt = " << patTau->chargedHadronIso() << ","
		   << " #particles = " << patTau->isolationPFChargedHadrCands().size() << std::endl;
    *outputStream_ << "  pfNeutralHadronIsolation: Pt = " << patTau->neutralHadronIso() << ", "
		   << " #particles = " << patTau->isolationPFNeutrHadrCands().size() << std::endl;
    *outputStream_ << "  pfGammaIsolation: Pt  = " << patTau->photonIso() << ","
		   << " #particles = " << patTau->isolationPFGammaCands().size() << std::endl;
    *outputStream_ << " jetRadius = " << TMath::Sqrt(patTau->etaetaMoment() + patTau->phiphiMoment()) << std::endl;
    *outputStream_ << " eVeto = " << patTau->tauID("againstElectron") << std::endl;
    *outputStream_ << " EcalStripSumE/P = " << patTau->ecalStripSumEOverPLead() << std::endl;
    *outputStream_ << " BremsRecoveryE/P = " << patTau->bremsRecoveryEOverPLead() << std::endl;
    *outputStream_ << " HCAL3x3/P = " << patTau->hcal3x3OverPLead() << std::endl;
    *outputStream_ << " muVeto = " << patTau->tauID("againstMuon") << std::endl;
    *outputStream_ << " vertex" << std::endl;
    printVertexInfo(patTau->vertex(), outputStream_);
    if ( genParticleSource_.label() != "" )
      *outputStream_ << "* matching gen. pdgId = "
		     << getMatchingGenParticlePdgId(patTau->p4(), *genParticles, &skipPdgIdsGenParticleMatch_) << std::endl;
    if ( printTauIdEfficiencies_ ) {
      *outputStream_ << " pat::Tau id. efficiencies (byStandardChain):" << std::endl
		     << "  Ztautau = " << patTau->efficiency("effByStandardChainZTTsim").value() << std::endl;
      *outputStream_ << " pat::Tau fake-rates (byStandardChain):" << std::endl;
      printTauEfficiency(*outputStream_, *patTau, "ppMuX", "frByStandardChainMuEnrichedQCDsim", "qcdMuEnriched");
      printTauEfficiency(*outputStream_, *patTau, "QCD, highest Pt jet", "frByStandardChainDiJetHighPtsim", "qcdDiJetLeadJet");
      printTauEfficiency(*outputStream_, *patTau, "QCD, second highest Pt jet", "frByStandardChainDiJetSecondPtsim", "qcdDiJetSecondLeadJet");
      printTauEfficiency(*outputStream_, *patTau, "WplusJets", "frByStandardChainWJetssim", "WplusJets");
    }
    ++iTau;
  }

  *outputStream_ << std::endl;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATTauDump, "PATTauDump");
DEFINE_EDM_PLUGIN(ObjectDumpPluginFactory, PATTauDump, "PATTauDump");
