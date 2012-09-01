#include "TauAnalysis/CandidateTools/plugins/NSVfitTauToMuInputAnalyzer.h"

#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TrackingTools/Records/interface/TransientTrackRecord.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "TauAnalysis/CandidateTools/interface/SVfitTrackExtrapolation.h"
#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"
#include "TauAnalysis/CandidateTools/interface/svFitLikelihoodDisplayAuxFunctions.h"

using namespace SVfit_namespace;

const int minNumTracksRefit = 2;
const int minNumTracksFit = 2;

NSVfitTauToMuInputAnalyzer::NSVfitTauToMuInputAnalyzer(const edm::ParameterSet& cfg)
  : moduleLabel_(cfg.getParameter<std::string>("@module_label")),
    eventVertexFitAlgorithm_(0),
    decayVertexFitAlgorithm_(0)
{
  srcGenParticles_ = cfg.getParameter<edm::InputTag>("srcGenParticles");
  srcElectrons_ = cfg.getParameter<edm::InputTag>("srcElectrons");
  srcMuons_ = cfg.getParameter<edm::InputTag>("srcMuons");
  srcTaus_ = cfg.getParameter<edm::InputTag>("srcTaus");
  srcRecVertex_ = cfg.getParameter<edm::InputTag>("srcRecVertex");

  srcWeights_ = cfg.getParameter<vInputTag>("srcWeights");
  
  dqmDirectory_ = cfg.getParameter<std::string>("dqmDirectory");

  edm::ParameterSet cfgEventVertexFitAlgo(cfg);
  cfgEventVertexFitAlgo.addParameter<unsigned>("minNumTracksRefit", minNumTracksRefit);
  eventVertexFitAlgorithm_ = new NSVfitEventVertexRefitter(cfgEventVertexFitAlgo);

  edm::ParameterSet cfgDecayVertexFitAlgorithm(cfg);
  cfgDecayVertexFitAlgorithm.addParameter<unsigned>("minNumTracksFit", minNumTracksFit);
  decayVertexFitAlgorithm_ = new NSVfitDecayVertexFitter(cfgDecayVertexFitAlgorithm);
}

NSVfitTauToMuInputAnalyzer::~NSVfitTauToMuInputAnalyzer()
{
  delete eventVertexFitAlgorithm_;
  delete decayVertexFitAlgorithm_;
}

void NSVfitTauToMuInputAnalyzer::beginJob()
{
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    throw cms::Exception("NSVfitTauToMuInputAnalyzer") 
      << " Failed to access dqmStore --> histograms will NEITHER be booked NOR filled !!\n";
  }

  DQMStore& dqmStore = (*edm::Service<DQMStore>());

  dqmStore.setCurrentFolder(dqmDirectory_.data());

  genTauPt_                 = dqmStore.book1D("genTauPt",                     "genTauPt",                  500,               0.,             500.);
  genTauEta_                = dqmStore.book1D("genTauEta",                    "genTauEta",                 100,             -5.0,             +5.0);
  genTauPhi_                = dqmStore.book1D("genTauPhi",                    "genTauPhi",                 360,     -TMath::Pi(),     +TMath::Pi());
  genTauVisEnFrac_          = dqmStore.book1D("genTauVisEnFrac",              "genTauVisEnFrac",           100,               0.,              1.0);
  genTauDecayDistance_      = dqmStore.book1D("genTauDecayDistance",          "genTauDecayDistance",      2000,               0.,              10.);
  genTau_phi_lab_           = dqmStore.book1D("genTau_phi_lab",               "genTau_phi_lab",            360,     -TMath::Pi(),     +TMath::Pi());
  genTau_gjAngle_           = dqmStore.book1D("genTau_gjAngle",               "genTau_gjAngle",            180,               0.,      TMath::Pi());

  genMuonPt_                = dqmStore.book1D("genMuonPt",                    "genMuonPt",                 500,               0.,             500.);
  genMuonEta_               = dqmStore.book1D("genMuonEta",                   "genMuonEta",                100,             -5.0,             +5.0);
  genMuonPhi_               = dqmStore.book1D("genMuonPhi",                   "genMuonPhi",                360,     -TMath::Pi(),     +TMath::Pi());

  recMuonDeltaPt_absolute_  = dqmStore.book1D("recMuonDeltaPt_absolute",      "recMuonDeltaPt_absolute",   100,             -50.,             +50.);
  recMuonDeltaPt_relative_  = dqmStore.book1D("recMuonDeltaPt_relative",      "recMuonDeltaPt_relative",   200,              -1.,              +1.);
  recMuonDeltaEta_          = dqmStore.book1D("recMuonDeltaEta",              "recMuonDeltaEta",           100,             -0.5,             +0.5);
  recMuonDeltaPhi_          = dqmStore.book1D("recMuonDeltaPhi",              "recMuonDeltaPhi",           360,     -TMath::Pi(),     +TMath::Pi());

  recTrackDeltaPt_absolute_ = dqmStore.book1D("recLeadTrackDeltaPt_absolute", "recTrackDeltaPt_absolute",  100,             -50.,             +50.);
  recTrackDeltaPt_relative_ = dqmStore.book1D("recLeadTrackDeltaPt_relative", "recTrackDeltaPt_relative",  200,              -1.,              +1.);
  recTrackDeltaEta_         = dqmStore.book1D("recLeadTrackDeltaEta",         "recTrackDeltaEta",           50,           -0.025,           +0.025);
  recTrackDeltaPhi_         = dqmStore.book1D("recLeadTrackDeltaPhi",         "recTrackDeltaPhi",           50,           -0.025,           +0.025);
  recTrackNumHits_          = dqmStore.book1D("recLeadTrackNumHits",          "recTrackNumHits",            50,             -0.5,             49.5);
  recTrackNumPixelHits_     = dqmStore.book1D("recLeadTrackNumPixelHits",     "recTrackNumPixelHits",       15,             -0.5,             14.5);  
  recTrackNormalizedChi2_   = dqmStore.book1D("recLeadTrackNormalizedChi2",   "recTrackNormalizedChi2",    250,               0.,              25.);
  recTrackDCA_              = dqmStore.book1D("recLeadTrackDCA",              "recTrackDCA",               250,               0.,              25.);
  recTrackPCApull3d_        = dqmStore.book1D("recLeadTrackPCApull3d",        "recTrackPCApull3d",         250,               0.,              25.);
}

namespace
{
  double norm2(const AlgebraicVector3& p)
  {
    return square(p(0)) + square(p(1)) + square(p(2));
  }
}

void NSVfitTauToMuInputAnalyzer::analyze(const edm::Event& evt, const edm::EventSetup& es)
{
  edm::ESHandle<TransientTrackBuilder> trackBuilderHandle;
  es.get<TransientTrackRecord>().get("TransientTrackBuilder", trackBuilderHandle);
  const TransientTrackBuilder* trackBuilder = trackBuilderHandle.product();
  if ( !trackBuilder ) {
    throw cms::Exception("NSVfitTauToMuInputAnalyzer::analyze")
      << " Failed to access TransientTrackBuilder !!\n";
  }

  edm::Handle<reco::GenParticleCollection> genParticles;
  evt.getByLabel(srcGenParticles_, genParticles);
  
  edm::Handle<reco::VertexCollection> recVertices;
  evt.getByLabel(srcRecVertex_, recVertices);
  const reco::Vertex* recVertex = 0;
  if ( recVertices->size() >= 1 ) recVertex = &recVertices->front();
  else return;

  std::vector<const reco::GenParticle*> genTaus;
  for ( reco::GenParticleCollection::const_iterator genParticle = genParticles->begin();
	genParticle != genParticles->end(); ++genParticle ) {
    int absPdgId = TMath::Abs(genParticle->pdgId());
    int status = genParticle->status();
    if ( absPdgId == 15 && status == 2 ) genTaus.push_back(&(*genParticle));
  }

  typedef edm::View<reco::Candidate> CandidateView;

  std::vector<matchedTauDecayType*> matchedTauToElecDecays;
  edm::Handle<CandidateView> electrons;
  if ( srcElectrons_.label() != "" ) {
    evt.getByLabel(srcElectrons_, electrons);
    matchRecToGenTauDecays(*electrons, genTaus, 0.3, matchedTauToElecDecays);
  }

  std::vector<matchedTauDecayType*> matchedTauToMuDecays;
  edm::Handle<CandidateView> muons;
  evt.getByLabel(srcMuons_, muons);
  matchRecToGenTauDecays(*muons, genTaus, 0.3, matchedTauToMuDecays);

  std::vector<matchedTauDecayType*> matchedTauToHadDecays;
  edm::Handle<CandidateView> taus;
  if ( srcTaus_.label() != "" ) {
    evt.getByLabel(srcTaus_, taus);
    matchRecToGenTauDecays(*taus, genTaus, 0.3, matchedTauToHadDecays);
  }

  //std::cout << "matchedTauToMuDecays.size = " << matchedTauToMuDecays.size()  << std::endl;
  const matchedTauDecayType* matchedTau = matchedTauToMuDecays.at(0);
  if ( !matchedTau ) return;

  std::vector<matchedTauDecayType*> matchedTauDecays;
  matchedTauDecays.insert(matchedTauDecays.end(), matchedTauToElecDecays.begin(), matchedTauToElecDecays.end());
  matchedTauDecays.insert(matchedTauDecays.end(), matchedTauToMuDecays.begin(), matchedTauToMuDecays.end());
  matchedTauDecays.insert(matchedTauDecays.end(), matchedTauToHadDecays.begin(), matchedTauToHadDecays.end());
  
  std::vector<const reco::Track*> tauTracks;
  for ( std::vector<matchedTauDecayType*>::const_iterator matchedTauDecay = matchedTauDecays.begin();
	matchedTauDecay != matchedTauDecays.end(); ++matchedTauDecay ) {
    tauTracks.insert(tauTracks.end(), (*matchedTauDecay)->recTracks_.begin(), (*matchedTauDecay)->recTracks_.end());
  }
  TransientVertex recVertex_refitted = eventVertexFitAlgorithm_->refit(recVertex, &tauTracks);
  AlgebraicVector3 recVertexPos_refitted;
  AlgebraicMatrix33 recVertexCov_refitted;
  bool recVertexIsValid = recVertex_refitted.isValid();
  if ( recVertexIsValid ) {
    recVertexPos_refitted = AlgebraicVector3(recVertex_refitted.position().x(), recVertex_refitted.position().y(), recVertex_refitted.position().z());
    recVertexCov_refitted = recVertex_refitted.positionError().matrix_new();
  }

  double evtWeight = 1.0;
  for ( vInputTag::const_iterator srcWeight = srcWeights_.begin();
	srcWeight != srcWeights_.end(); ++srcWeight ) {
    edm::Handle<double> weight;
    evt.getByLabel(*srcWeight, weight);
    evtWeight *= (*weight);
  }

  if ( evtWeight < 1.e-3 || evtWeight > 1.e+3 || TMath::IsNaN(evtWeight) ) return;

  genTauPt_->Fill(matchedTau->genTauP4_.pt(), evtWeight);
  genTauEta_->Fill(matchedTau->genTauP4_.eta(), evtWeight);
  genTauPhi_->Fill(matchedTau->genTauP4_.phi(), evtWeight);
  genTauVisEnFrac_->Fill(matchedTau->genVisP4_.E()/matchedTau->genTauP4_.E(), evtWeight);
  genTauDecayDistance_->Fill(matchedTau->genTauDecayDistance_, evtWeight);
  genTau_phi_lab_->Fill(phiLabFromLabMomenta(matchedTau->genTauP4_, matchedTau->genVisP4_), evtWeight);
  genTau_gjAngle_->Fill(gjAngleFromLabMomenta(matchedTau->genTauP4_, matchedTau->genVisP4_), evtWeight);

  const reco::GenParticle* genMuon = 0;
  for ( std::vector<const reco::GenParticle*>::const_iterator genTauDecayProduct = matchedTau->genTauDecayProducts_.begin();
	genTauDecayProduct != matchedTau->genTauDecayProducts_.end(); ++genTauDecayProduct ) {
    if ( TMath::Abs((*genTauDecayProduct)->pdgId()) == 13 ) {
      genMuon = (*genTauDecayProduct);
      break;
    }
  }

  if ( genMuon ) {
    genMuonPt_->Fill(genMuon->pt(), evtWeight);
    genMuonEta_->Fill(genMuon->eta(), evtWeight);
    genMuonPhi_->Fill(genMuon->phi(), evtWeight);
  }

  double deltaPt_absolute = matchedTau->recVisP4_.pt() - matchedTau->genVisP4_.pt();
  recMuonDeltaPt_absolute_->Fill(deltaPt_absolute, evtWeight);
  if ( matchedTau->genVisP4_.pt() > 0. ) {
    double deltaPt_relative = deltaPt_absolute/matchedTau->genVisP4_.pt();
    recMuonDeltaPt_relative_->Fill(deltaPt_relative, evtWeight);
  }
  recMuonDeltaEta_->Fill(matchedTau->recVisP4_.eta() - matchedTau->genVisP4_.eta(), evtWeight);
  recMuonDeltaPhi_->Fill(matchedTau->recVisP4_.phi() - matchedTau->genVisP4_.phi(), evtWeight);

  if ( genMuon && matchedTau->recLeadTrack_ ) {
    double deltaPt_absolute = matchedTau->recLeadTrack_->pt() - genMuon->pt();
    recTrackDeltaPt_absolute_->Fill(deltaPt_absolute, evtWeight);
    if ( genMuon->pt() > 0. ) {
      double deltaPt_relative = deltaPt_absolute/genMuon->pt();
      recTrackDeltaPt_relative_->Fill(deltaPt_relative, evtWeight);
    }
    recTrackDeltaEta_->Fill(matchedTau->recLeadTrack_->eta() - genMuon->eta(), evtWeight);
    recTrackDeltaPhi_->Fill(matchedTau->recLeadTrack_->phi() - genMuon->phi(), evtWeight);
    recTrackNumHits_->Fill(matchedTau->recLeadTrack_->numberOfValidHits(), evtWeight);
    recTrackNumPixelHits_->Fill(matchedTau->recLeadTrack_->hitPattern().numberOfValidPixelHits(), evtWeight);
    recTrackNormalizedChi2_->Fill(matchedTau->recLeadTrack_->normalizedChi2(), evtWeight);
    if ( recVertexIsValid ) {
      SVfitTrackExtrapolation leadTrack_extrapolation(*matchedTau->recLeadTrackTrajectory_, matchedTau->genTauDecayVertexPos_);
      const AlgebraicVector3& pcaPos = leadTrack_extrapolation.point_of_closest_approach();
      AlgebraicMatrix33 pcaCov = leadTrack_extrapolation.covariance();
      for ( unsigned iRow = 0; iRow < 3; ++iRow ) {
	for ( unsigned iColumn = 0; iColumn < 3; ++iColumn ) {
	  pcaCov(iRow, iColumn) += recVertexCov_refitted(iRow, iColumn);
	}
      }
      AlgebraicVector3 residual = pcaPos - matchedTau->genTauDecayVertexPos_;
      double dca = TMath::Sqrt(norm2(residual));
      recTrackDCA_->Fill(dca, evtWeight);
      double sigma2 = ROOT::Math::Similarity(residual.Unit(), pcaCov);
      if ( sigma2 > 0. ) recTrackPCApull3d_->Fill(dca/TMath::Sqrt(sigma2), evtWeight);
    }
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(NSVfitTauToMuInputAnalyzer);





