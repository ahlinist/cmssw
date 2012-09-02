#include "TauAnalysis/CandidateTools/plugins/SVfitTrackInfoAnalyzer.h"

#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TrackingTools/Records/interface/TransientTrackRecord.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/View.h"

#include "TauAnalysis/CandidateTools/interface/SVfitTrackExtrapolation.h"
#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"
#include "TauAnalysis/CandidateTools/interface/svFitLikelihoodDisplayAuxFunctions.h"
#include "TauAnalysis/CandidateTools/interface/candidateAuxFunctions.h"

using namespace SVfit_namespace;

const int minNumTracksFit = 2;

SVfitTrackInfoAnalyzer::SVfitTrackInfoAnalyzer(const edm::ParameterSet& cfg)
  : moduleLabel_(cfg.getParameter<std::string>("@module_label")),
    decayVertexFitAlgorithm_(0)
{
  srcGenParticles_ = cfg.getParameter<edm::InputTag>("srcGenParticles");
  srcLeptons_ = cfg.getParameter<edm::InputTag>("srcLeptons");

  srcWeights_ = cfg.getParameter<vInputTag>("srcWeights");
  
  dqmDirectory_ = cfg.getParameter<std::string>("dqmDirectory");

  edm::ParameterSet cfgDecayVertexFitAlgorithm(cfg);
  cfgDecayVertexFitAlgorithm.addParameter<unsigned>("minNumTracksFit", minNumTracksFit);
  decayVertexFitAlgorithm_ = new NSVfitDecayVertexFitter(cfgDecayVertexFitAlgorithm);

  typedef std::vector<double> vdouble;

  vdouble ptBinning;
  ptBinning.push_back(-1.);
  ptBinning.push_back(5.);
  ptBinning.push_back(10.);
  ptBinning.push_back(20.);
  ptBinning.push_back(30.);
  ptBinning.push_back(40.);
  ptBinning.push_back(100.);

  vdouble etaBinning;
  etaBinning.push_back(-2.3);
  etaBinning.push_back(-1.7);
  etaBinning.push_back(-1.2);
  etaBinning.push_back(0.);
  etaBinning.push_back(+1.2);
  etaBinning.push_back(+1.7);
  etaBinning.push_back(+2.3);

  for ( size_t idxPtBin = 0; idxPtBin < (ptBinning.size() - 1); ++idxPtBin ) {
    double ptMin = ptBinning[idxPtBin];
    double ptMax = ptBinning[idxPtBin + 1];
    for ( size_t idxEtaBin = 0; idxEtaBin < (etaBinning.size() - 1); ++idxEtaBin ) {
      double etaMin = etaBinning[idxEtaBin];
      double etaMax = etaBinning[idxEtaBin + 1];
      plotEntries1Prong_.push_back(new plotEntryType(std::string(dqmDirectory_).append("/1Prong"), ptMin, ptMax, etaMin, etaMax));
    }
    plotEntries1Prong_.push_back(new plotEntryType(std::string(dqmDirectory_).append("/1Prong"), ptMin, ptMax, -9.9, +9.9));
  }
  for ( size_t idxEtaBin = 0; idxEtaBin < (etaBinning.size() - 1); ++idxEtaBin ) {
    double etaMin = etaBinning[idxEtaBin];
    double etaMax = etaBinning[idxEtaBin + 1];
    plotEntries1Prong_.push_back(new plotEntryType(std::string(dqmDirectory_).append("/1Prong"), -1., -1., etaMin, etaMax));
    plotEntries3Prong_.push_back(new plotEntryType(std::string(dqmDirectory_).append("/3Prong"), -1., -1., etaMin, etaMax));
  }
  plotEntries1Prong_.push_back(new plotEntryType(std::string(dqmDirectory_).append("/1Prong"), -1., -1., -9.9, +9.9));
  plotEntries3Prong_.push_back(new plotEntryType(std::string(dqmDirectory_).append("/3Prong"), -1., -1., -9.9, +9.9));
}

SVfitTrackInfoAnalyzer::~SVfitTrackInfoAnalyzer()
{
  delete decayVertexFitAlgorithm_;

  for ( std::vector<plotEntryType*>::iterator it = plotEntries1Prong_.begin();
	it != plotEntries1Prong_.end(); ++it ) {
    delete (*it);
  }
  for ( std::vector<plotEntryType*>::iterator it = plotEntries3Prong_.begin();
	it != plotEntries3Prong_.end(); ++it ) {
    delete (*it);
  }
}

void SVfitTrackInfoAnalyzer::beginJob()
{
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    throw cms::Exception("SVfitTrackInfoAnalyzer") 
      << " Failed to access dqmStore --> histograms will NEITHER be booked NOR filled !!\n";
  }

  DQMStore& dqmStore = (*edm::Service<DQMStore>());

  for ( std::vector<plotEntryType*>::iterator plotEntry = plotEntries1Prong_.begin();
	plotEntry != plotEntries1Prong_.end(); ++ plotEntry ) {
    (*plotEntry)->bookHistograms(dqmStore);
  }
  for ( std::vector<plotEntryType*>::iterator plotEntry = plotEntries3Prong_.begin();
	plotEntry != plotEntries3Prong_.end(); ++plotEntry ) {
    (*plotEntry)->bookHistograms(dqmStore);
  }
}

void SVfitTrackInfoAnalyzer::analyze(const edm::Event& evt, const edm::EventSetup& es)
{
  edm::ESHandle<TransientTrackBuilder> trackBuilderHandle;
  es.get<TransientTrackRecord>().get("TransientTrackBuilder", trackBuilderHandle);
  const TransientTrackBuilder* trackBuilder = trackBuilderHandle.product();
  if ( !trackBuilder ) {
    throw cms::Exception("SVfitTrackInfoAnalyzer::analyze")
      << " Failed to access TransientTrackBuilder !!\n";
  }

  edm::Handle<reco::GenParticleCollection> genParticles;
  evt.getByLabel(srcGenParticles_, genParticles);
  
  std::vector<const reco::GenParticle*> genTaus;
  for ( reco::GenParticleCollection::const_iterator genParticle = genParticles->begin();
	genParticle != genParticles->end(); ++genParticle ) {
    int absPdgId = TMath::Abs(genParticle->pdgId());
    int status = genParticle->status();
    if ( absPdgId == 15 && status == 2 ) genTaus.push_back(&(*genParticle));
  }

  typedef edm::View<reco::Candidate> CandidateView;

  std::vector<matchedTauDecayType*> matchedTauDecays;
  edm::Handle<CandidateView> leptons;
  evt.getByLabel(srcLeptons_, leptons);
  matchRecToGenTauDecays(*leptons, genTaus, 0.3, matchedTauDecays, trackBuilder, decayVertexFitAlgorithm_);

  double evtWeight = 1.0;
  for ( vInputTag::const_iterator srcWeight = srcWeights_.begin();
	srcWeight != srcWeights_.end(); ++srcWeight ) {
    edm::Handle<double> weight;
    evt.getByLabel(*srcWeight, weight);
    evtWeight *= (*weight);
  }

  if ( evtWeight < 1.e-3 || evtWeight > 1.e+3 || TMath::IsNaN(evtWeight) ) return;

  for ( std::vector<matchedTauDecayType*>::const_iterator matchedTauDecay = matchedTauDecays.begin();
	matchedTauDecay != matchedTauDecays.end(); ++matchedTauDecay ) {
    if ( (*matchedTauDecay)->recLeadTrack_ ) {
      const AlgebraicVector3& genProdVertexPos = (*matchedTauDecay)->genTauProdVertexPos_;
      const AlgebraicVector3& genDecayVertexPos = (*matchedTauDecay)->genTauDecayVertexPos_;
      double pt = (*matchedTauDecay)->recLeadTrack_->pt();
      double eta = (*matchedTauDecay)->recLeadTrack_->eta();
      if ( (*matchedTauDecay)->recLeadTrackTrajectory_ ) {
	SVfitTrackExtrapolation recLeadTrackExtrapolation(*(*matchedTauDecay)->recLeadTrackTrajectory_, genDecayVertexPos);
	if ( !recLeadTrackExtrapolation.errorFlag() ) {
	  const AlgebraicVector3& recDecayVertexPos = recLeadTrackExtrapolation.point_of_closest_approach();
	  const AlgebraicMatrix33& recDecayVertexCov = recLeadTrackExtrapolation.covariance();
	  for ( std::vector<plotEntryType*>::iterator plotEntry = plotEntries1Prong_.begin();
		plotEntry != plotEntries1Prong_.end(); ++ plotEntry ) {	  
	    (*plotEntry)->fillHistograms(pt, eta, 
					 genProdVertexPos, genDecayVertexPos,
					 recDecayVertexPos, recDecayVertexCov,
					 evtWeight);
	  }
	}
      }
      if ( (*matchedTauDecay)->hasRecTauDecayVertex_ ) {
	const AlgebraicVector3& recDecayVertexPos = (*matchedTauDecay)->recTauDecayVertexPos_;
	const AlgebraicMatrix33& recDecayVertexCov = (*matchedTauDecay)->recTauDecayVertexCov_;
	for ( std::vector<plotEntryType*>::iterator plotEntry = plotEntries3Prong_.begin();
	      plotEntry != plotEntries3Prong_.end(); ++ plotEntry ) {	  
	  (*plotEntry)->fillHistograms(pt, eta, 
				       genProdVertexPos, genDecayVertexPos,
				       recDecayVertexPos, recDecayVertexCov,
				       evtWeight);
	}
      }
    }
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(SVfitTrackInfoAnalyzer);





