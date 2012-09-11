#include "TauAnalysis/CandidateTools/plugins/NSVfitTauToHadInputAnalyzer.h"

#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TrackingTools/Records/interface/TransientTrackRecord.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/TauReco/interface/PFTauDecayMode.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/View.h"

#include "TauAnalysis/CandidateTools/interface/SVfitTrackExtrapolation.h"
#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"
#include "TauAnalysis/CandidateTools/interface/svFitLikelihoodDisplayAuxFunctions.h"
#include "TauAnalysis/CandidateTools/interface/candidateAuxFunctions.h"

using namespace SVfit_namespace;

const int minNumTracksRefit = 2;
const int minNumTracksFit = 2;

NSVfitTauToHadInputAnalyzer::NSVfitTauToHadInputAnalyzer(const edm::ParameterSet& cfg)
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

NSVfitTauToHadInputAnalyzer::~NSVfitTauToHadInputAnalyzer()
{
  delete eventVertexFitAlgorithm_;
  delete decayVertexFitAlgorithm_;
}

namespace
{
  // CV: code for setting axis labels copied from TauAnalysis/Core/src/histManagerAuxFunctions.cc
  //     to avoid dependency of TauAnalysis/CandidateTools on TauAnalysis/Core package

  void setAxisLabel(TAxis* axis, int tauDecayMode)
  {
//--- set label for tau decay mode passed as function argument
//   ( same labels to be used for generated and reconstructed tau decay modes,
//     except for 3 bins of generated tau decay mode histogram,
//     which are customized according to the definition in PhysicsTools/JetMCUtils/src/JetMCTag.cc )
//
//    NOTE: bin numbers start at 1 (not 0) !!
//
    axis->SetBinLabel(1 + tauDecayMode, getTauDecayModeName(tauDecayMode).data());
  }

  void setAxisLabelsGenTauDecayMode(TAxis* axis)
  {
//--- set labels for generated tau decay modes

    setAxisLabel(axis, reco::PFTauDecayMode::tauDecaysElectron);
    setAxisLabel(axis, reco::PFTauDecayMode::tauDecayMuon);
    setAxisLabel(axis, reco::PFTauDecayMode::tauDecay1ChargedPion0PiZero);
    setAxisLabel(axis, reco::PFTauDecayMode::tauDecay1ChargedPion1PiZero);
    setAxisLabel(axis, reco::PFTauDecayMode::tauDecay1ChargedPion2PiZero);
    axis->SetBinLabel(1 + reco::PFTauDecayMode::tauDecay1ChargedPion4PiZero, "oneProngOther");
    setAxisLabel(axis, reco::PFTauDecayMode::tauDecay3ChargedPion0PiZero);
    setAxisLabel(axis, reco::PFTauDecayMode::tauDecay3ChargedPion1PiZero);
    axis->SetBinLabel(1 + reco::PFTauDecayMode::tauDecay3ChargedPion4PiZero, "threeProngOther");
    axis->SetBinLabel(1 + reco::PFTauDecayMode::tauDecayOther, "rare");
  }

  void setAxisLabelsRecTauDecayMode(TAxis* axis)
  {
//--- set labels for reconstructed tau decay modes

    setAxisLabel(axis, reco::PFTauDecayMode::tauDecaysElectron);
    setAxisLabel(axis, reco::PFTauDecayMode::tauDecayMuon);
    setAxisLabel(axis, reco::PFTauDecayMode::tauDecay1ChargedPion0PiZero);
    setAxisLabel(axis, reco::PFTauDecayMode::tauDecay1ChargedPion1PiZero);
    setAxisLabel(axis, reco::PFTauDecayMode::tauDecay1ChargedPion2PiZero);
    setAxisLabel(axis, reco::PFTauDecayMode::tauDecay1ChargedPion3PiZero);
    setAxisLabel(axis, reco::PFTauDecayMode::tauDecay1ChargedPion4PiZero);
    setAxisLabel(axis, reco::PFTauDecayMode::tauDecay2ChargedPion0PiZero);
    setAxisLabel(axis, reco::PFTauDecayMode::tauDecay2ChargedPion1PiZero);
    setAxisLabel(axis, reco::PFTauDecayMode::tauDecay2ChargedPion2PiZero);
    setAxisLabel(axis, reco::PFTauDecayMode::tauDecay2ChargedPion3PiZero);
    setAxisLabel(axis, reco::PFTauDecayMode::tauDecay2ChargedPion4PiZero);
    setAxisLabel(axis, reco::PFTauDecayMode::tauDecay3ChargedPion0PiZero);
    setAxisLabel(axis, reco::PFTauDecayMode::tauDecay3ChargedPion1PiZero);
    setAxisLabel(axis, reco::PFTauDecayMode::tauDecay3ChargedPion2PiZero);
    setAxisLabel(axis, reco::PFTauDecayMode::tauDecay3ChargedPion3PiZero);
    setAxisLabel(axis, reco::PFTauDecayMode::tauDecay3ChargedPion4PiZero);
    setAxisLabel(axis, reco::PFTauDecayMode::tauDecayOther);
  }
}

void NSVfitTauToHadInputAnalyzer::beginJob()
{
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    throw cms::Exception("NSVfitTauToHadInputAnalyzer") 
      << " Failed to access dqmStore --> histograms will NEITHER be booked NOR filled !!\n";
  }

  DQMStore& dqmStore = (*edm::Service<DQMStore>());

  dqmStore.setCurrentFolder(dqmDirectory_.data());

  genTauPt_                       = dqmStore.book1D("genTauPt",                       "genTauPt",                        500,               0.,             500.);
  genTauEta_                      = dqmStore.book1D("genTauEta",                      "genTauEta",                       100,             -5.0,             +5.0);
  genTauPhi_                      = dqmStore.book1D("genTauPhi",                      "genTauPhi",                       360,     -TMath::Pi(),     +TMath::Pi());
  genTauVisEnFrac_                = dqmStore.book1D("genTauVisEnFrac",                "genTauVisEnFrac",                 100,               0.,              1.0);
  genTauVisMass_                  = dqmStore.book1D("genTauVisMass",                  "genTauVisMass",                   180,               0.,              1.8);
  genTauDecayDistance_            = dqmStore.book1D("genTauDecayDistance",            "genTauDecayDistance",            2000,               0.,              10.);
  genTauDecayDistanceNormalized_  = dqmStore.book1D("genTauDecayDistanceNormalized",  "genTauDecayDistanceNormalized",   250,               0.,              25.);
  genTauDecayMode_                = dqmStore.book1D("genTauDecayMode",                "genTauDecayMode",                  20,             -0.5,             19.5);
  setAxisLabelsGenTauDecayMode(genTauDecayMode_->getTH1()->GetXaxis());
  genTau_phi_lab_                 = dqmStore.book1D("genTau_phi_lab",                 "genTau_phi_lab",                  360,     -TMath::Pi(),     +TMath::Pi());
  genTau_gjAngle_                 = dqmStore.book1D("genTau_gjAngle",                 "genTau_gjAngle",                  180,               0.,      TMath::Pi());

  genLeadTrackPt_                 = dqmStore.book1D("genLeadTrackPt",                 "genLeadTrackPt",                  500,               0.,             500.);
  genLeadTrackEta_                = dqmStore.book1D("genLeadTrackEta",                "genLeadTrackEta",                 100,             -5.0,             +5.0);
  genLeadTrackPhi_                = dqmStore.book1D("genLeadTrackPhi",                "genLeadTrackPhi",                 360,     -TMath::Pi(),     +TMath::Pi());

  recTauHadDeltaPt_absolute_      = dqmStore.book1D("recTauHadDeltaPt_absolute",      "recTauHadDeltaPt_absolute",      1000,            -500.,            +500.);
  recTauHadDeltaPt_relative_      = dqmStore.book1D("recTauHadDeltaPt_relative",      "recTauHadDeltaPt_relative",       200,              -1.,              +1.);
  recTauHadDeltaEta_              = dqmStore.book1D("recTauHadDeltaEta",              "recTauHadDeltaEta",               200,             -0.1,             +0.1);
  recTauHadDeltaPhi_              = dqmStore.book1D("recTauHadDeltaPhi",              "recTauHadDeltaPhi",               200,             -0.1,             +0.1);
  recTauHadDeltaVisMass_absolute_ = dqmStore.book1D("recTauHadDeltaVisMass_absolute", "recTauHadDeltaVisMass_absolute",  360,             -1.8,              1.8);
  recTauHadDeltaVisMass_relative_ = dqmStore.book1D("recTauHadDeltaVisMass_relative", "recTauHadDeltaVisMass_relative",  400,             -10.,             +10.);
  recTauDecayMode_                = dqmStore.book1D("recTauDecayMode",                "recTauDecayMode",                  20,             -0.5,             19.5);
  setAxisLabelsRecTauDecayMode(recTauDecayMode_->getTH1()->GetXaxis());

  recLeadTrackDeltaPt_absolute_   = dqmStore.book1D("recLeadTrackDeltaPt_absolute",   "recLeadTrackDeltaPt_absolute",    100,             -50.,             +50.);
  recLeadTrackDeltaPt_relative_   = dqmStore.book1D("recLeadTrackDeltaPt_relative",   "recLeadTrackDeltaPt_relative",    200,              -1.,              +1.);
  recLeadTrackDeltaEta_           = dqmStore.book1D("recLeadTrackDeltaEta",           "recLeadTrackDeltaEta",             50,           -0.025,           +0.025);
  recLeadTrackDeltaPhi_           = dqmStore.book1D("recLeadTrackDeltaPhi",           "recLeadTrackDeltaPhi",             50,           -0.025,           +0.025);
  recLeadTrackNumHits_            = dqmStore.book1D("recLeadTrackNumHits",            "recLeadTrackNumHits",              50,             -0.5,             49.5);
  recLeadTrackNumPixelHits_       = dqmStore.book1D("recLeadTrackNumPixelHits",       "recLeadTrackNumPixelHits",         15,             -0.5,             14.5);  
  recLeadTrackNormalizedChi2_     = dqmStore.book1D("recLeadTrackNormalizedChi2",     "recLeadTrackNormalizedChi2",      250,               0.,              25.);
  recLeadTrackDCA_                = dqmStore.book1D("recLeadTrackDCA",                "recLeadTrackDCA",                 250,               0.,              25.);
  recLeadTrackPCApull3d_          = dqmStore.book1D("recLeadTrackPCApull3d",          "recLeadTrackPCApull3d",           250,               0.,              25.);

  recDecayVertexDeltaParl_        = dqmStore.book1D("recDecayVertexDeltaParl",        "recDecayVertexDeltaParl",         500,               0.,              0.5);
  recDecayVertexDeltaPerp_        = dqmStore.book1D("recDecayVertexDeltaPerp",        "recDecayVertexDeltaPerp",         500,               0.,              0.5);
  recDecayVertexDeltaZ_           = dqmStore.book1D("recDecayVertexDeltaZ",           "recDecayVertexDeltaZ",            500,               0.,               5.);
  recDecayVertexPull3d_           = dqmStore.book1D("recDecayVertexPull3d",           "recDecayVertexPull3d",            250,               0.,              25.);
  recDecayVertexNDoF_             = dqmStore.book1D("recDecayVertexNDoF",             "recDecayVertexNDoF",              100,               0.,             100.);
  recDecayVertexNormalizedChi2_   = dqmStore.book1D("recDecayVertexNormalizedChi2",   "recDecayVertexNormalizedChi2",    250,               0.,              25.);
}

void NSVfitTauToHadInputAnalyzer::analyze(const edm::Event& evt, const edm::EventSetup& es)
{
  edm::ESHandle<TransientTrackBuilder> trackBuilderHandle;
  es.get<TransientTrackRecord>().get("TransientTrackBuilder", trackBuilderHandle);
  const TransientTrackBuilder* trackBuilder = trackBuilderHandle.product();
  if ( !trackBuilder ) {
    throw cms::Exception("NSVfitTauToHadInputAnalyzer::analyze")
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
  if ( srcMuons_.label() != "" ) {
    evt.getByLabel(srcMuons_, muons);
    matchRecToGenTauDecays(*muons, genTaus, 0.3, matchedTauToMuDecays);
  }

  std::vector<matchedTauDecayType*> matchedTauToHadDecays;
  edm::Handle<CandidateView> taus;
  evt.getByLabel(srcTaus_, taus);
  matchRecToGenTauDecays(*taus, genTaus, 0.3, matchedTauToHadDecays);

  //std::cout << "matchedTauToHadDecays.size = " << matchedTauToHadDecays.size()  << std::endl;
  const matchedTauDecayType* matchedTau = ( matchedTauToHadDecays.size() >= 1 ) ?
    matchedTauToHadDecays.at(0) : 0;
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
  genTauVisMass_->Fill(matchedTau->genVisP4_.mass(), evtWeight);
  genTauDecayDistance_->Fill(matchedTau->genTauDecayDistance_, evtWeight);
  double a = (matchedTau->genTauP4_.P()/tauLeptonMass)*cTauLifetime;
  if ( a > 0. ) genTauDecayDistanceNormalized_->Fill(matchedTau->genTauDecayDistance_/a, evtWeight);
  genTauDecayMode_->getTH1()->Fill(matchedTau->genTauDecayMode_.data(), evtWeight);
  genTau_phi_lab_->Fill(phiLabFromLabMomenta(matchedTau->genTauP4_, matchedTau->genVisP4_), evtWeight);
  genTau_gjAngle_->Fill(gjAngleFromLabMomenta(matchedTau->genTauP4_, matchedTau->genVisP4_), evtWeight);

  const reco::GenParticle* genLeadTrack = 0;
  double genLeadTrackPt = -1.;
  for ( std::vector<const reco::GenParticle*>::const_iterator genTauDecayProduct = matchedTau->genTauDecayProducts_.begin();
	genTauDecayProduct != matchedTau->genTauDecayProducts_.end(); ++genTauDecayProduct ) {
    if ( TMath::Abs((*genTauDecayProduct)->charge()) > 0.5 && (*genTauDecayProduct)->pt() > genLeadTrackPt ) {
      genLeadTrack = (*genTauDecayProduct);
    }
  }

  if ( genLeadTrack ) {
    genLeadTrackPt_->Fill(genLeadTrack->pt(), evtWeight);
    genLeadTrackEta_->Fill(genLeadTrack->eta(), evtWeight);
    genLeadTrackPhi_->Fill(genLeadTrack->phi(), evtWeight);
  }

  double deltaPt_absolute = matchedTau->recVisP4_.pt() - matchedTau->genVisP4_.pt();
  recTauHadDeltaPt_absolute_->Fill(deltaPt_absolute, evtWeight);
  if ( matchedTau->genVisP4_.pt() > 0. ) {
    double deltaPt_relative = deltaPt_absolute/matchedTau->genVisP4_.pt();
    recTauHadDeltaPt_relative_->Fill(deltaPt_relative, evtWeight);
  }
  recTauHadDeltaEta_->Fill(matchedTau->recVisP4_.eta() - matchedTau->genVisP4_.eta(), evtWeight);
  recTauHadDeltaPhi_->Fill(matchedTau->recVisP4_.phi() - matchedTau->genVisP4_.phi(), evtWeight);
  double deltaVisMass_absolute = matchedTau->recVisP4_.mass() - matchedTau->genVisP4_.mass();
  recTauHadDeltaVisMass_absolute_->Fill(deltaVisMass_absolute, evtWeight);
  if ( matchedTau->genVisP4_.mass() > 0. ) {
    double deltaVisMass_relative = deltaVisMass_absolute/matchedTau->genVisP4_.mass();
    recTauHadDeltaVisMass_relative_->Fill(deltaVisMass_relative, evtWeight);
  }
  recTauDecayMode_->getTH1()->Fill(matchedTau->recTauDecayMode_, evtWeight);
  
  if ( genLeadTrack && matchedTau->recLeadTrack_ ) {
    double deltaPt_absolute = matchedTau->recLeadTrack_->pt() - genLeadTrack->pt();
    recLeadTrackDeltaPt_absolute_->Fill(deltaPt_absolute, evtWeight);
    if ( genLeadTrack->pt() > 0. ) {
      double deltaPt_relative = deltaPt_absolute/genLeadTrack->pt();
      recLeadTrackDeltaPt_relative_->Fill(deltaPt_relative, evtWeight);
    }
    recLeadTrackDeltaEta_->Fill(matchedTau->recLeadTrack_->eta() - genLeadTrack->eta(), evtWeight);
    recLeadTrackDeltaPhi_->Fill(matchedTau->recLeadTrack_->phi() - genLeadTrack->phi(), evtWeight);
    recLeadTrackNumHits_->Fill(matchedTau->recLeadTrack_->numberOfValidHits(), evtWeight);
    recLeadTrackNumPixelHits_->Fill(matchedTau->recLeadTrack_->hitPattern().numberOfValidPixelHits(), evtWeight);
    recLeadTrackNormalizedChi2_->Fill(matchedTau->recLeadTrack_->normalizedChi2(), evtWeight);
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
      recLeadTrackDCA_->Fill(dca, evtWeight);
      double sigma2 = ROOT::Math::Similarity(residual.Unit(), pcaCov);
      if ( sigma2 > 0. ) recLeadTrackPCApull3d_->Fill(dca/TMath::Sqrt(sigma2), evtWeight);
    }
  }

  if ( matchedTau->hasRecTauDecayVertex_ ) {
    AlgebraicVector3 residual = matchedTau->recTauDecayVertexPos_ - matchedTau->genTauDecayVertexPos_;
    reco::Candidate::Vector residual_vector(residual(0), residual(1), residual(2));
    AlgebraicVector3 tauFlight = matchedTau->genTauDecayVertexPos_ - matchedTau->genTauProdVertexPos_;
    reco::Candidate::Vector tauFlight_vector(tauFlight(0), tauFlight(1), 0.);
    reco::Candidate::Vector u_parl = normalize(tauFlight_vector);
    reco::Candidate::Vector u_z    = reco::Candidate::Vector(0., 0., 1.);
    reco::Candidate::Vector u_perp = normalize(compCrossProduct(u_z, u_parl));
    double delta_parl = compScalarProduct(residual_vector, u_parl);
    recDecayVertexDeltaParl_->Fill(delta_parl, evtWeight);
    double delta_perp = compScalarProduct(residual_vector, u_perp);
    recDecayVertexDeltaPerp_->Fill(delta_perp, evtWeight);
    recDecayVertexDeltaZ_->Fill(residual(2), evtWeight);
    double sigma2 = ROOT::Math::Similarity(residual.Unit(), matchedTau->recTauDecayVertexCov_);
    if ( sigma2 > 0. ) recDecayVertexPull3d_->Fill(TMath::Sqrt(norm2(residual)/sigma2), evtWeight);
    recDecayVertexNDoF_->Fill(matchedTau->recTauDecayVertex_.degreesOfFreedom(), evtWeight);
    recDecayVertexNormalizedChi2_->Fill(matchedTau->recTauDecayVertex_.normalisedChiSquared(), evtWeight);
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(NSVfitTauToHadInputAnalyzer);





