#include "TauAnalysis/CandidateTools/plugins/NSVfitEventInputAnalyzer.h"

#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETFwd.h"
#include "DataFormats/Common/interface/View.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"
#include "TauAnalysis/CandidateTools/interface/svFitLikelihoodDisplayAuxFunctions.h"

#include "AnalysisDataFormats/TauAnalysis/interface/PFMEtSignCovMatrix.h"

using namespace SVfit_namespace;

const int minNumTracksRefit = 2;

NSVfitEventInputAnalyzer::NSVfitEventInputAnalyzer(const edm::ParameterSet& cfg)
  : moduleLabel_(cfg.getParameter<std::string>("@module_label")),
    eventVertexFitAlgorithm_(0)
{
  srcGenParticles_ = cfg.getParameter<edm::InputTag>("srcGenParticles");
  srcElectrons_    = cfg.getParameter<edm::InputTag>("srcElectrons");
  srcMuons_        = cfg.getParameter<edm::InputTag>("srcMuons");
  srcTaus_         = cfg.getParameter<edm::InputTag>("srcTaus");
  srcMEt_          = cfg.getParameter<edm::InputTag>("srcMEt");
  srcMEtCov_       = cfg.getParameter<edm::InputTag>("srcMEtCov");
  srcGenVertex_    = cfg.getParameter<edm::InputTag>("srcGenVertex");
  srcRecVertex_    = cfg.getParameter<edm::InputTag>("srcRecVertex");

  srcWeights_      = cfg.getParameter<vInputTag>("srcWeights");
  
  dqmDirectory_    = cfg.getParameter<std::string>("dqmDirectory");

  edm::ParameterSet cfgEventVertexFitAlgo(cfg);
  cfgEventVertexFitAlgo.addParameter<unsigned>("minNumTracksRefit", minNumTracksRefit);
  eventVertexFitAlgorithm_ = new NSVfitEventVertexRefitter(cfgEventVertexFitAlgo);
}

NSVfitEventInputAnalyzer::~NSVfitEventInputAnalyzer()
{
  delete eventVertexFitAlgorithm_;
}

void NSVfitEventInputAnalyzer::beginJob()
{
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    throw cms::Exception("NSVfitEventInputAnalyzer") 
      << " Failed to access dqmStore --> histograms will NEITHER be booked NOR filled !!\n";
  }

  DQMStore& dqmStore = (*edm::Service<DQMStore>());

  dqmStore.setCurrentFolder(dqmDirectory_.data());

  genMEtPt_                = dqmStore.book1D("genMEtPt",                "genMEtPt",                 500,           0.,         500.);

  recMEtDeltaPt_           = dqmStore.book1D("recMEtDeltaPt",           "recMEtDeltaPt",            250,        -125.,        +125.);
  recMEtDeltaPparl_        = dqmStore.book1D("recMEtDeltaPparl",        "recMEtDeltaPparl",         250,        -125.,        +125.);
  recMEtDeltaPperp_        = dqmStore.book1D("recMEtDeltaPperp",        "recMEtDeltaPperp",         125,           0.,        +125.);

  recMEtSigma2d_           = dqmStore.book1D("recMEtSigma2d",           "recMEtSigma2d",            100,           0.,          50.);
  recMEtSigmaParl_         = dqmStore.book1D("recMEtSigmaParl",         "recMEtSigmaParl",          100,           0.,          50.);
  recMEtSigmaPerp_         = dqmStore.book1D("recMEtSigmaPerp",         "recMEtSigmaPerp",          100,           0.,          50.);

  recMEtPull2d_            = dqmStore.book1D("recMEtPull2d",            "recMEtPull2d",             250,           0.,         +25.);
  recMEtPullParl_          = dqmStore.book1D("recMEtPullParl",          "recMEtPullParl",           500,         -25.,         +25.);
  recMEtPullPerp_          = dqmStore.book1D("recMEtPullPerp",          "recMEtPullPerp",           500,         -25.,         +25.);

  genVertexR_              = dqmStore.book1D("genVertexR",              "genVertexR",              1000,           0.,          10.);
  genVertexPhi_            = dqmStore.book1D("genVertexPhi",            "genVertexPhi",             360, -TMath::Pi(), +TMath::Pi());
  genVertexZ_              = dqmStore.book1D("genVertexZ",              "genVertexZ",               500,         -25.,         +25.);

  recVertexDeltaX_         = dqmStore.book1D("recVertexDeltaX",         "recVertexDeltaX",         1000,        -0.25,        +0.25);
  recVertexDeltaY_         = dqmStore.book1D("recVertexDeltaY",         "recVertexDeltaY",         1000,        -0.25,        +0.25);
  recVertexDeltaZ_         = dqmStore.book1D("recVertexDeltaZ",         "recVertexDeltaZ",         1000,         -0.5,         +0.5);
  recVertexNDoF_           = dqmStore.book1D("recVertexNDoF",           "recVertexNDoF",            500,         -0.5,        499.5);
  recVertexNormalizedChi2_ = dqmStore.book1D("recVertexNormalizedChi2", "recVertexNormalizedChi2",  250,           0.,          25.);

  recVertexSigmaX_         = dqmStore.book1D("recVertexSigmaX",         "recVertexSigmaX",          500,           0.,          0.5);
  recVertexSigmaY_         = dqmStore.book1D("recVertexSigmaY",         "recVertexSigmaY",          500,           0.,          0.5);
  recVertexSigmaZ_         = dqmStore.book1D("recVertexSigmaZ",         "recVertexSigmaZ",          500,           0.,          5.0);

  recVertexPull3d_         = dqmStore.book1D("recVertexPull3d",         "recVertexPull3d",          250,           0.,         +25.);
  recVertexPullX_          = dqmStore.book1D("recVertexPullX",          "recVertexPullX",           500,         -25.,         +25.);
  recVertexPullY_          = dqmStore.book1D("recVertexPullY",          "recVertexPullY",           500,         -25.,         +25.);
  recVertexPullZ_          = dqmStore.book1D("recVertexPullZ",          "recVertexPullZ",           500,         -25.,         +25.);
}

namespace
{
  double compCovProj(const reco::Candidate::Vector& dir3, const TMatrixD& cov22)
  {
    double retVal =  dir3.x()*(cov22(0,0)*dir3.x() + cov22(0,1)*dir3.y())
                   + dir3.y()*(cov22(1,0)*dir3.x() + cov22(1,1)*dir3.y());
    return retVal;
  }

  double norm2(const AlgebraicVector3& p)
  {
    return square(p(0)) + square(p(1)) + square(p(2));
  }
}

void NSVfitEventInputAnalyzer::analyze(const edm::Event& evt, const edm::EventSetup& es)
{
  edm::Handle<reco::GenParticleCollection> genParticles;
  evt.getByLabel(srcGenParticles_, genParticles);

  std::vector<const reco::GenParticle*> genTaus;
  reco::Candidate::LorentzVector genMEtP4(0,0,0,0);
  for ( reco::GenParticleCollection::const_iterator genParticle = genParticles->begin();
	genParticle != genParticles->end(); ++genParticle ) {
    int absPdgId = TMath::Abs(genParticle->pdgId());
    int status = genParticle->status();
    if ( absPdgId == 15 && status == 2 ) genTaus.push_back(&(*genParticle));
    else if ( (absPdgId == 12 || absPdgId == 14 || absPdgId == 16) && status == 1 ) genMEtP4 += genParticle->p4();
  }

  edm::Handle<reco::PFMETCollection> recMETs;
  evt.getByLabel(srcMEt_, recMETs);
  if ( !(recMETs->size() == 1) )
    throw cms::Exception("NSVfitEventInputAnalyzer::analyze") 
      << "Failed to find unique MET object !!\n";
  const reco::PFMET& recMEt = recMETs->front();

  TMatrixD recMEtCov(2,2);
  if ( srcMEtCov_.label() != "" ) {
    edm::Handle<PFMEtSignCovMatrix> recMEtCovHandle;    
    evt.getByLabel(srcMEtCov_, recMEtCovHandle);
    recMEtCov = (*recMEtCovHandle);
  } else {
    recMEtCov = recMEt.getSignificanceMatrix();
  }

  edm::Handle<reco::Vertex> genVertex;
  evt.getByLabel(srcGenVertex_, genVertex);
  const reco::Candidate::Point& genVertexPos_point = genVertex->position();
  AlgebraicVector3 genVertexPos(genVertexPos_point.x(), genVertexPos_point.y(), genVertexPos_point.z());

  edm::Handle<reco::VertexCollection> recVertices;
  evt.getByLabel(srcRecVertex_, recVertices);
  const reco::Vertex* recVertex = 0;
  if ( recVertices->size() >= 1 ) recVertex = &recVertices->front();
  else return;
  //const reco::Candidate::Point& recVertexPos = recVertex->position();
  //AlgebraicMatrix33 recVertexCov;
  //for ( unsigned iRow = 0; iRow < 3; ++iRow ) {
  //  for ( unsigned iColumn = 0; iColumn < 3; ++iColumn ) {
  //    recVertexCov(iRow, iColumn) = recVertex->covariance()(iRow, iColumn);
  //  }
  //}

  typedef edm::View<reco::Candidate> CandidateView;

  std::vector<matchedTauDecayType*> matchedTauDecays;

  edm::Handle<CandidateView> electrons;
  if ( srcElectrons_.label() != "" ) {
    evt.getByLabel(srcElectrons_, electrons);
    matchRecToGenTauDecays(*electrons, genTaus, 0.3, matchedTauDecays);
  }

  edm::Handle<CandidateView> muons;
  if ( srcMuons_.label() != "" ) {
    evt.getByLabel(srcMuons_, muons);
    matchRecToGenTauDecays(*muons, genTaus, 0.3, matchedTauDecays);
  }

  edm::Handle<CandidateView> taus;
  if ( srcTaus_.label() != "" ) {
    evt.getByLabel(srcTaus_, taus);
    matchRecToGenTauDecays(*taus, genTaus, 0.3, matchedTauDecays);
  }

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

  genMEtPt_->Fill(genMEtP4.pt(), evtWeight);

  recMEtDeltaPt_->Fill(recMEt.pt() - genMEtP4.pt(), evtWeight);
  
  reco::Candidate::Vector residual(recMEt.px() - genMEtP4.px(), recMEt.py() - genMEtP4.py(), 0.);

  reco::Candidate::Vector u_parl = normalize(reco::Candidate::Vector(genMEtP4.px(), genMEtP4.py(), 0.));
  reco::Candidate::Vector u_perp = normalize(compCrossProduct(reco::Candidate::Vector(0., 0., 1.), u_parl));

  double residualParl = compScalarProduct(residual, u_parl);
  double residualPerp = compScalarProduct(residual, u_perp);

  recMEtDeltaPparl_->Fill(residualParl, evtWeight);
  recMEtDeltaPperp_->Fill(residualPerp, evtWeight);

  double residual_mag2 = residual.mag2();
  if ( residual_mag2 > 0. ) {
    double sigma2 = compCovProj(residual, recMEtCov)/residual_mag2;
    if ( sigma2 > 0. ) {
      recMEtSigma2d_->Fill(TMath::Sqrt(sigma2), evtWeight);
      recMEtPull2d_->Fill(TMath::Sqrt(residual_mag2/sigma2), evtWeight);
    }
  }

  double sigmaParl2 = compCovProj(u_parl, recMEtCov);
  double sigmaPerp2 = compCovProj(u_perp, recMEtCov);

  if ( sigmaParl2 > 0. ) {
    recMEtSigmaParl_->Fill(TMath::Sqrt(sigmaParl2), evtWeight);
    recMEtPullParl_ ->Fill(residualParl/TMath::Sqrt(sigmaParl2), evtWeight);
  }
  if ( sigmaPerp2 > 0. ) {
    recMEtSigmaPerp_->Fill(TMath::Sqrt(sigmaPerp2), evtWeight);
    recMEtPullPerp_ ->Fill(residualPerp/TMath::Sqrt(sigmaPerp2), evtWeight);
  }

  double genVertexR = TMath::Sqrt(square(genVertexPos(0)) + square(genVertexPos(1)));

  genVertexR_->Fill(genVertexR, evtWeight);
  genVertexPhi_->Fill(genVertexPos_point.phi(), evtWeight);
  genVertexZ_->Fill(genVertexPos(2), evtWeight);

  if ( recVertexIsValid ) {
    AlgebraicVector3 recWrtGenVertexPos = recVertexPos_refitted - genVertexPos;

    recVertexDeltaX_->Fill(recWrtGenVertexPos(0), evtWeight);
    recVertexDeltaY_->Fill(recWrtGenVertexPos(1), evtWeight);
    recVertexDeltaZ_->Fill(recWrtGenVertexPos(2), evtWeight);
    recVertexNDoF_->Fill(recVertex_refitted.degreesOfFreedom(), evtWeight);
    recVertexNormalizedChi2_->Fill(recVertex_refitted.normalisedChiSquared(), evtWeight);

    double recVertexSigmaX = TMath::Sqrt(recVertexCov_refitted(0, 0));
    double recVertexSigmaY = TMath::Sqrt(recVertexCov_refitted(1, 1));
    double recVertexSigmaZ = TMath::Sqrt(recVertexCov_refitted(2, 2));
  
    recVertexSigmaX_->Fill(recVertexSigmaX, evtWeight);
    recVertexSigmaY_->Fill(recVertexSigmaY, evtWeight);
    recVertexSigmaZ_->Fill(recVertexSigmaZ, evtWeight);

    double recVertexSigma3d = TMath::Sqrt(ROOT::Math::Similarity(recWrtGenVertexPos.Unit(), recVertexCov_refitted));
    if ( recVertexSigma3d > 0. ) recVertexPull3d_->Fill(TMath::Sqrt(norm2(recWrtGenVertexPos))/recVertexSigma3d, evtWeight);
    if ( recVertexSigmaX  > 0. ) recVertexPullX_->Fill(recWrtGenVertexPos(0)/recVertexSigmaX, evtWeight);
    if ( recVertexSigmaY  > 0. ) recVertexPullY_->Fill(recWrtGenVertexPos(1)/recVertexSigmaY, evtWeight);
    if ( recVertexSigmaZ  > 0. ) recVertexPullZ_->Fill(recWrtGenVertexPos(2)/recVertexSigmaZ, evtWeight);
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(NSVfitEventInputAnalyzer);





