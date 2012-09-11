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
const int minNumTracksRefit = 2;

SVfitTrackInfoAnalyzer::SVfitTrackInfoAnalyzer(const edm::ParameterSet& cfg)
  : moduleLabel_(cfg.getParameter<std::string>("@module_label")),
    decayVertexFitAlgorithm_(0)
{
  srcGenParticles_ = cfg.getParameter<edm::InputTag>("srcGenParticles");
  srcLeptons_ = cfg.getParameter<vInputTag>("srcLeptons");
  srcVerticesWithoutBS_ = cfg.getParameter<edm::InputTag>("srcVerticesWithoutBS");
  srcVerticesWithBS_ = cfg.getParameter<edm::InputTag>("srcVerticesWithBS");
  srcBeamSpot_ = cfg.getParameter<edm::InputTag>("srcBeamSpot");

  srcWeights_ = cfg.getParameter<vInputTag>("srcWeights");
  
  dqmDirectory_ = cfg.getParameter<std::string>("dqmDirectory");

  edm::ParameterSet cfgDecayVertexFitAlgorithm(cfg);
  cfgDecayVertexFitAlgorithm.addParameter<unsigned>("minNumTracksFit", minNumTracksFit);
  decayVertexFitAlgorithm_ = new NSVfitDecayVertexFitter(cfgDecayVertexFitAlgorithm);

  edm::ParameterSet cfgProdVertexFitAlgorithm(cfg);
  cfgProdVertexFitAlgorithm.addParameter<edm::InputTag>("srcBeamSpot", srcBeamSpot_.label());
  cfgProdVertexFitAlgorithm.addParameter<std::string>("algorithm", "AdaptiveVertexFitter");
  cfgProdVertexFitAlgorithm.addParameter<unsigned>("minNumTracksRefit", minNumTracksRefit);
  edm::ParameterSet cfgProdVertexFitAlgorithm_woBeamSpot(cfgProdVertexFitAlgorithm);
  cfgProdVertexFitAlgorithm_woBeamSpot.addParameter<bool>("applyBeamSpotConstraint", false);
  prodVertexFitAlgorithm_woBeamSpot_ = new NSVfitEventVertexRefitter(cfgProdVertexFitAlgorithm_woBeamSpot);
  edm::ParameterSet cfgProdVertexFitAlgorithm_wBeamSpot(cfgProdVertexFitAlgorithm);
  cfgProdVertexFitAlgorithm_wBeamSpot.addParameter<bool>("applyBeamSpotConstraint", true);
  prodVertexFitAlgorithm_wBeamSpot_ = new NSVfitEventVertexRefitter(cfgProdVertexFitAlgorithm_wBeamSpot);
  
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
      plotEntries1Prong_.push_back(new plotEntryType1(std::string(dqmDirectory_).append("/1Prong"), ptMin, ptMax, etaMin, etaMax));
    }
    plotEntries1Prong_.push_back(new plotEntryType1(std::string(dqmDirectory_).append("/1Prong"), ptMin, ptMax, -9.9, +9.9));
  }
  for ( size_t idxEtaBin = 0; idxEtaBin < (etaBinning.size() - 1); ++idxEtaBin ) {
    double etaMin = etaBinning[idxEtaBin];
    double etaMax = etaBinning[idxEtaBin + 1];
    plotEntries1Prong_.push_back(new plotEntryType1(std::string(dqmDirectory_).append("/1Prong"), -1., -1., etaMin, etaMax));
    plotEntries3Prong_.push_back(new plotEntryType1(std::string(dqmDirectory_).append("/3Prong"), -1., -1., etaMin, etaMax));
  }
  plotEntries1Prong_.push_back(new plotEntryType1(std::string(dqmDirectory_).append("/1Prong"), -1., -1., -9.9, +9.9));
  plotEntries3Prong_.push_back(new plotEntryType1(std::string(dqmDirectory_).append("/3Prong"), -1., -1., -9.9, +9.9));

  plotEntryProdVertex_woBSoriginal_ = new plotEntryType2("prodVertex_woBSoriginal", false, prodVertexFitAlgorithm_woBeamSpot_);
  plotEntryProdVertex_wBSoriginal_  = new plotEntryType2("prodVertex_wBSoriginal", false, prodVertexFitAlgorithm_wBeamSpot_);
  
  plotEntryProdVertex_woBSrefitted_ = new plotEntryType2("prodVertex_woBSrefitted", true, prodVertexFitAlgorithm_woBeamSpot_);
  plotEntryProdVertex_wBSrefitted_  = new plotEntryType2("prodVertex_wBSrefitted", true, prodVertexFitAlgorithm_wBeamSpot_);
}

SVfitTrackInfoAnalyzer::~SVfitTrackInfoAnalyzer()
{
  delete decayVertexFitAlgorithm_;

  for ( std::vector<plotEntryType1*>::iterator it = plotEntries1Prong_.begin();
	it != plotEntries1Prong_.end(); ++it ) {
    delete (*it);
  }
  for ( std::vector<plotEntryType1*>::iterator it = plotEntries3Prong_.begin();
	it != plotEntries3Prong_.end(); ++it ) {
    delete (*it);
  }

  delete plotEntryProdVertex_woBSoriginal_;
  delete plotEntryProdVertex_wBSoriginal_;

  delete plotEntryProdVertex_woBSrefitted_;
  delete plotEntryProdVertex_wBSrefitted_;
}

void SVfitTrackInfoAnalyzer::beginJob()
{
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    throw cms::Exception("SVfitTrackInfoAnalyzer") 
      << " Failed to access dqmStore --> histograms will NEITHER be booked NOR filled !!\n";
  }

  DQMStore& dqmStore = (*edm::Service<DQMStore>());

  for ( std::vector<plotEntryType1*>::iterator plotEntry = plotEntries1Prong_.begin();
	plotEntry != plotEntries1Prong_.end(); ++ plotEntry ) {
    (*plotEntry)->bookHistograms(dqmStore);
  }
  for ( std::vector<plotEntryType1*>::iterator plotEntry = plotEntries3Prong_.begin();
	plotEntry != plotEntries3Prong_.end(); ++plotEntry ) {
    (*plotEntry)->bookHistograms(dqmStore);
  }

  plotEntryProdVertex_woBSoriginal_->bookHistograms(dqmStore);
  plotEntryProdVertex_wBSoriginal_->bookHistograms(dqmStore);

  plotEntryProdVertex_woBSrefitted_->bookHistograms(dqmStore);
  plotEntryProdVertex_wBSrefitted_->bookHistograms(dqmStore);

  dqmStore.setCurrentFolder(dqmDirectory_.data());

  residualBeamSpotX_                 = dqmStore.book1D("residualBeamSpotX",                 "residualBeamSpotX",                 2000,  -0.1,  +0.1);
  sigmaBeamSpotX_                    = dqmStore.book1D("sigmaBeamSpotX",                    "sigmaBeamSpotX",                    1000,   0.,    0.1);
  pullBeamSpotX_                     = dqmStore.book1D("pullBeamSpotX",                     "pullBeamSpotX",                      500, -25.,  +25.);
  residualBeamSpotY_                 = dqmStore.book1D("residualBeamSpotY",                 "residualBeamSpotY",                 2000,  -0.1,  +0.1);
  sigmaBeamSpotY_                    = dqmStore.book1D("sigmaBeamSpotY",                    "sigmaBeamSpotY",                    1000,   0.,    0.1);
  pullBeamSpotY_                     = dqmStore.book1D("pullBeamSpotY",                     "pullBeamSpotY",                      500, -25.,  +25.);
  residualBeamSpotZ_                 = dqmStore.book1D("residualBeamSpotZ",                 "residualBeamSpotZ",                 2000,  -0.1,  +0.1);
  sigmaBeamSpotZ_                    = dqmStore.book1D("sigmaBeamSpotZ",                    "sigmaBeamSpotZ",                    1000,   0.,   10.0);
  pullBeamSpotZ_                     = dqmStore.book1D("pullBeamSpotZ",                     "pullBeamSpotZ",                      500, -25.,  +25.);

  pullBeamSpot2d_                    = dqmStore.book1D("pullBeamSpot2d",                    "pullBeamSpot2d",                     500, -25.,  +25.);
  pullBeamSpot3d_                    = dqmStore.book1D("pullBeamSpot3d",                    "pullBeamSpot3d",                     500, -25.,  +25.);

  genTauDecayDistanceNormalized1vs2_ = dqmStore.book2D("genTauDecayDistanceNormalized1vs2", "genTauDecayDistanceNormalized1vs2",  250,   0.,   25., 250, 0., 25.);
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

  decayVertexFitAlgorithm_->beginEvent(evt, es);
  prodVertexFitAlgorithm_woBeamSpot_->beginEvent(evt, es);
  prodVertexFitAlgorithm_wBeamSpot_->beginEvent(evt, es);

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
  for ( vInputTag::const_iterator srcLeptons_i = srcLeptons_.begin();
	srcLeptons_i != srcLeptons_.end(); ++srcLeptons_i ) {
    edm::Handle<CandidateView> leptons;
    evt.getByLabel(*srcLeptons_i, leptons);
    std::vector<matchedTauDecayType*> matchedTauDecays_i;
    matchRecToGenTauDecays(*leptons, genTaus, 0.3, matchedTauDecays_i, trackBuilder, decayVertexFitAlgorithm_);
    matchedTauDecays.insert(matchedTauDecays.end(), matchedTauDecays_i.begin(), matchedTauDecays_i.end());
  }
  
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
    //(*matchedTauDecay)->print(std::cout);
    if ( (*matchedTauDecay)->recLeadTrack_ ) {
      const AlgebraicVector3& genProdVertexPos = (*matchedTauDecay)->genTauProdVertexPos_;
      const AlgebraicVector3& genDecayVertexPos = (*matchedTauDecay)->genTauDecayVertexPos_;
      if ( (*matchedTauDecay)->recLeadTrackTrajectory_ ) {
	SVfitTrackExtrapolation recLeadTrackExtrapolation(*(*matchedTauDecay)->recLeadTrackTrajectory_, genDecayVertexPos);
	if ( !recLeadTrackExtrapolation.errorFlag() ) {
	  const AlgebraicVector3& recDecayVertexPos = recLeadTrackExtrapolation.point_of_closest_approach();
	  const AlgebraicMatrix33& recDecayVertexCov = recLeadTrackExtrapolation.covariance();
	  for ( std::vector<plotEntryType1*>::iterator plotEntry = plotEntries1Prong_.begin();
		plotEntry != plotEntries1Prong_.end(); ++ plotEntry ) {	  
	    //std::cout << "processing 1-Prong:" << std::endl;
	    (*plotEntry)->fillHistograms((*matchedTauDecay)->genTauP4_, 
					 genProdVertexPos, genDecayVertexPos,
					 (*matchedTauDecay)->recLeadTrack_->pt(), (*matchedTauDecay)->recLeadTrack_->eta(),
					 recDecayVertexPos, recDecayVertexCov,
					 evtWeight);
	  }
	}
      }
      if ( (*matchedTauDecay)->hasRecTauDecayVertex_ ) {
	const AlgebraicVector3& recDecayVertexPos = (*matchedTauDecay)->recTauDecayVertexPos_;
	const AlgebraicMatrix33& recDecayVertexCov = (*matchedTauDecay)->recTauDecayVertexCov_;
	for ( std::vector<plotEntryType1*>::iterator plotEntry = plotEntries3Prong_.begin();
	      plotEntry != plotEntries3Prong_.end(); ++ plotEntry ) {	  
	  //std::cout << "processing 3-Prong:" << std::endl;
	  (*plotEntry)->fillHistograms((*matchedTauDecay)->genTauP4_, 
				       genProdVertexPos, genDecayVertexPos,
				       (*matchedTauDecay)->recLeadTrack_->pt(), (*matchedTauDecay)->recLeadTrack_->eta(),
				       recDecayVertexPos, recDecayVertexCov,
				       evtWeight);
	}
      }
    }
  }

//--- get beamspot
  edm::Handle<reco::BeamSpot> beamSpotHandle;
  evt.getByLabel(srcBeamSpot_, beamSpotHandle);
  const reco::BeamSpot* beamSpot = beamSpotHandle.product();
  if ( !beamSpot ) {
    edm::LogError ("SVfitTrackInfoAnalyzer::analyze")
      << " Failed to access BeamSpot !!";
  }

//--- vertex matched to reconstructed tau decay products
  typedef edm::View<reco::Vertex> VertexView;
  edm::Handle<VertexView> verticesWithoutBS;
  evt.getByLabel(srcVerticesWithoutBS_, verticesWithoutBS);
  if ( verticesWithoutBS->size() != 1 ) return;
  const reco::Vertex& recProdVertexWithoutBS = verticesWithoutBS->front();
  edm::Handle<VertexView> verticesWithBS;
  evt.getByLabel(srcVerticesWithBS_, verticesWithBS);
  if ( verticesWithBS->size() != 1 ) return;
  const reco::Vertex& recProdVertexWithBS = verticesWithBS->front();
    
  std::vector<const reco::Track*> tauTracks;
  for ( std::vector<matchedTauDecayType*>::const_iterator matchedTauDecay = matchedTauDecays.begin();
	matchedTauDecay != matchedTauDecays.end(); ++matchedTauDecay ) {
    tauTracks.insert(tauTracks.end(), (*matchedTauDecay)->recTracks_.begin(), (*matchedTauDecay)->recTracks_.end());
  }

  if ( matchedTauDecays.size() >= 1 ) {
    const AlgebraicVector3& genProdVertexPos = matchedTauDecays.front()->genTauProdVertexPos_;

    plotEntryProdVertex_woBSoriginal_->fillHistograms(genProdVertexPos, recProdVertexWithoutBS, tauTracks, beamSpot, evtWeight);
    plotEntryProdVertex_wBSoriginal_->fillHistograms(genProdVertexPos, recProdVertexWithBS, tauTracks, beamSpot, evtWeight);
    
    plotEntryProdVertex_woBSrefitted_->fillHistograms(genProdVertexPos, recProdVertexWithoutBS, tauTracks, beamSpot, evtWeight);
    plotEntryProdVertex_wBSrefitted_->fillHistograms(genProdVertexPos, recProdVertexWithBS, tauTracks, beamSpot, evtWeight);

    AlgebraicVector3 beamSpotPos(beamSpot->position().x(), beamSpot->position().y(), beamSpot->position().z());
    VertexState beamSpotState(*beamSpot);    
    AlgebraicMatrix33 beamSpotCov;
    beamSpotCov(0, 0) = beamSpotState.error().cxx();
    beamSpotCov(0, 1) = beamSpotState.error().cyx();
    beamSpotCov(0, 2) = beamSpotState.error().czx();
    beamSpotCov(1, 0) = beamSpotState.error().cyx();
    beamSpotCov(1, 1) = beamSpotState.error().cyy();
    beamSpotCov(1, 2) = beamSpotState.error().czy();
    beamSpotCov(2, 0) = beamSpotState.error().czx();
    beamSpotCov(2, 1) = beamSpotState.error().czy();
    beamSpotCov(2, 2) = beamSpotState.error().czz();

    //printVector("beamSpotPos", beamSpotPos);
    //printMatrix("beamSpotCov", beamSpotCov);

    AlgebraicVector3 residualBeamSpot = genProdVertexPos - beamSpotPos;
    
    double residualBeamSpotX = residualBeamSpot(0);
    double residualBeamSpotY = residualBeamSpot(1);
    double residualBeamSpotZ = residualBeamSpot(2);
    
    fillHistograms1d(residualBeamSpotX_, sigmaBeamSpotX_, pullBeamSpotX_, residualBeamSpotX, TMath::Sqrt(beamSpotCov(0, 0)), evtWeight);
    fillHistograms1d(residualBeamSpotY_, sigmaBeamSpotY_, pullBeamSpotY_, residualBeamSpotY, TMath::Sqrt(beamSpotCov(1, 1)), evtWeight);
    fillHistograms1d(residualBeamSpotZ_, sigmaBeamSpotZ_, pullBeamSpotZ_, residualBeamSpotZ, TMath::Sqrt(beamSpotCov(2, 2)), evtWeight);
    
    pullBeamSpot2d_->Fill(pull2d(residualBeamSpot, beamSpotCov), evtWeight);
    pullBeamSpot3d_->Fill(pull3d(residualBeamSpot, beamSpotCov), evtWeight);
  }

  if ( matchedTauDecays.size() == 2 ) {
    const matchedTauDecayType* matchedTauDecay1 = matchedTauDecays.at(0);
    double genTauDecayDistance1 = TMath::Sqrt(norm2(matchedTauDecay1->genTauDecayVertexPos_ - matchedTauDecay1->genTauProdVertexPos_));
    double a1 = (matchedTauDecay1->genTauP4_.P()/tauLeptonMass)*cTauLifetime;
    const matchedTauDecayType* matchedTauDecay2 = matchedTauDecays.at(1);
    double genTauDecayDistance2 = TMath::Sqrt(norm2(matchedTauDecay2->genTauDecayVertexPos_ - matchedTauDecay2->genTauProdVertexPos_));
    double a2 = (matchedTauDecay2->genTauP4_.P()/tauLeptonMass)*cTauLifetime;
    if ( a1 > 0. && a2 > 0. ) genTauDecayDistanceNormalized1vs2_->Fill(genTauDecayDistance1/a1, genTauDecayDistance2/a2, evtWeight);
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(SVfitTrackInfoAnalyzer);





