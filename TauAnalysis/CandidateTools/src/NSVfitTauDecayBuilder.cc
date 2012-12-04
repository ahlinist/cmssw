#include "TauAnalysis/CandidateTools/interface/NSVfitTauDecayBuilder.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "DataFormats/TauReco/interface/PFTauDecayMode.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/CLHEP/interface/AlgebraicObjects.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitAlgorithmBase.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitParameter.h"
#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"
#include "TauAnalysis/CandidateTools/interface/candidateAuxFunctions.h"

#include <TMath.h>
#include <Math/VectorUtil.h>

using namespace SVfit_namespace;

const unsigned minNumTracksFit = 2;

NSVfitTauDecayBuilder::NSVfitTauDecayBuilder(const edm::ParameterSet& cfg)
  : NSVfitSingleParticleBuilderBase(cfg),
    algorithm_(0),
    trackBuilder_(0),
    decayVertexFitAlgorithm_(0),
    idxFitParameter_nuInvMass_(-1)
{
  edm::ParameterSet cfgTrackQualityCuts = cfg.getParameter<edm::ParameterSet>("trackQualityCuts");
  trackMinNumHits_      = cfgTrackQualityCuts.getParameter<unsigned>("minNumHits");
  trackMinNumPixelHits_ = cfgTrackQualityCuts.getParameter<unsigned>("minNumPixelHits");
  trackMaxChi2DoF_      = cfgTrackQualityCuts.getParameter<double>("maxChi2DoF");
  trackMaxDeltaPoverP_  = cfgTrackQualityCuts.getParameter<double>("maxDeltaPoverP");
  trackMinPt_           = cfgTrackQualityCuts.getParameter<double>("minPt");

  edm::ParameterSet cfgVertexFitAlgorithm(cfg);
  cfgVertexFitAlgorithm.addParameter<unsigned>("minNumTracksFit", minNumTracksFit);
  decayVertexFitAlgorithm_ = new NSVfitDecayVertexFitter(cfgVertexFitAlgorithm);

  fixToGenVisEnFracX_ = ( cfg.exists("fixToGenVisEnFracX") ) ? cfg.getParameter<bool>("fixToGenVisEnFracX")  : false;
  fixToGenPhiLab_     = ( cfg.exists("fixToGenPhiLab")     ) ? cfg.getParameter<bool>("fixToGenPhiLab")      : false;
  fixToGenNuInvMass_  = ( cfg.exists("fixToGenNuInvMass")  ) ? cfg.getParameter<bool>("fixToGenNuInvMass")   : false;
  fixToGenDeltaR_     = ( cfg.exists("fixToGenDeltaR")     ) ? cfg.getParameter<bool>("fixToGenDeltaR")      : false;
  fixToGenVisP4_      = ( cfg.exists("fixToGenVisP4")      ) ? cfg.getParameter<bool>("fixToGenVisP4")       : false;

  initializeToGen_    = ( cfg.exists("initializeToGen")    ) ? cfg.getParameter<bool>("initializeToGen")     : false;
  if ( fixToGenVisEnFracX_ || 
       fixToGenPhiLab_     ||
       fixToGenNuInvMass_  ||
       fixToGenDeltaR_     ||
       fixToGenVisP4_      ) {
    initializeToGen_ = true;
  }

  if ( initializeToGen_ ) {
    srcGenTaus_ = cfg.getParameter<edm::InputTag>("srcGenTaus");
    dRmatch_ = cfg.getParameter<double>("dRmatch");
  }
}

NSVfitTauDecayBuilder::~NSVfitTauDecayBuilder()
{
  delete decayVertexFitAlgorithm_;
}

void NSVfitTauDecayBuilder::beginJob(NSVfitAlgorithmBase* algorithm)
{
  algorithm_ = algorithm;

  // Map the fit parameters to indices.
  idxFitParameter_visEnFracX_ = getFitParameterIdx(algorithm, prodParticleLabel_, nSVfit_namespace::kTau_visEnFracX);
  idxFitParameter_phi_lab_    = getFitParameterIdx(algorithm, prodParticleLabel_, nSVfit_namespace::kTau_phi_lab);
  idxFitParameter_nuInvMass_  = getFitParameterIdx(algorithm, prodParticleLabel_, nSVfit_namespace::kTau_nuInvMass, true);
  idxFitParameter_deltaR_     = getFitParameterIdx(algorithm, prodParticleLabel_, nSVfit_namespace::kTau_decayDistance_lab_shift, true);

  if ( verbosity_ ) {
    std::cout << "<NSVfitTauDecayBuilder::beginJob>:" << std::endl;
    std::cout << " pluginName = " << pluginName_ << std::endl;
    std::cout << " idxFitParameter_visEnFracX = " << idxFitParameter_visEnFracX_ << std::endl;
    std::cout << " idxFitParameter_phi_lab = " << idxFitParameter_phi_lab_ << std::endl;
    std::cout << " idxFitParameter_nuInvMass = " << idxFitParameter_nuInvMass_ << std::endl;
    std::cout << " idxFitParameter_deltaR_ = " << idxFitParameter_deltaR_ << std::endl;
  }

  if ( fixToGenVisEnFracX_ && idxFitParameter_visEnFracX_  != -1 ) algorithm->fixFitParameter(idxFitParameter_visEnFracX_);
  if ( fixToGenPhiLab_     && idxFitParameter_phi_lab_     != -1 ) algorithm->fixFitParameter(idxFitParameter_phi_lab_);
  if ( fixToGenNuInvMass_  && idxFitParameter_nuInvMass_   != -1 ) algorithm->fixFitParameter(idxFitParameter_nuInvMass_);
  if ( fixToGenDeltaR_     && idxFitParameter_deltaR_      != -1 ) algorithm->fixFitParameter(idxFitParameter_deltaR_);
}

void NSVfitTauDecayBuilder::beginEvent(const edm::Event& evt, const edm::EventSetup& es)
{
  NSVfitBuilderBase::beginEvent(evt, es);

//--- get pointer to TransientTrackBuilder
  edm::ESHandle<TransientTrackBuilder> trackBuilderHandle;
  es.get<TransientTrackRecord>().get("TransientTrackBuilder", trackBuilderHandle);
  trackBuilder_ = trackBuilderHandle.product();
  if ( !trackBuilder_ ) {
    throw cms::Exception("NSVfitTauDecayBuilder::beginEvent")
      << " Failed to access TransientTrackBuilder !!\n";
  }

  decayVertexFitAlgorithm_->beginEvent(evt, es);
  
  if ( initializeToGen_ ) {
    evt.getByLabel(srcGenTaus_, genParticles_);
  }
}

namespace
{
  bool isHigherPt(const reco::Track* track1, const reco::Track* track2)
  {
    return (track1->pt() > track2->pt());
  }
}

void NSVfitTauDecayBuilder::initialize(NSVfitTauDecayHypothesis* hypothesis, const reco::Candidate* visCandidate) const
{
  hypothesis->p3Vis_unit_ = visCandidate->p4().Vect().Unit();
  hypothesis->visMass_ = visCandidate->mass();

  // Add protection against zero mass.  
  // If lower than the electron mass, set it to the electron mass.
  if ( hypothesis->visMass_ < 5.1e-4 ) {
    hypothesis->visMass_ = 5.1e-4;
  }

  // Set tau lepton decay mode
  hypothesis->decayMode_ = getDecayMode(visCandidate);

  // If this is a leptonic tau decay, we need to setup the limits on the
  // neutrino system invariant mass parameter.
  // In case reconstructed mass of visible decay products exceeds 1.6 GeV,
  // assume measurement error and "truncate" @ 1.6 GeV.
  if ( !nuSystemIsMassless() ) {
    NSVfitParameter* fitParameter = algorithm_->getFitParameter(idxFitParameter_nuInvMass_);
    assert(fitParameter);
    fitParameter->setUpperLimit(SVfit_namespace::tauLeptonMass - TMath::Min(hypothesis->visMass_, 1.6));
  }

  // Extract the associated tracks, and fit a vertex if possible.
  hypothesis->tracks_ = extractTracks(visCandidate);

  // Sort tracks by **decreasing** Pt and determine "leading" (highest Pt) track
  std::sort(hypothesis->tracks_.begin(), hypothesis->tracks_.end(), isHigherPt);

  unsigned idx = 0;
  for ( std::vector<const reco::Track*>::const_iterator track = hypothesis->tracks_.begin();
	track != hypothesis->tracks_.end(); ++track ) {
    if ( this->verbosity_ ) std::cout << "Track #" << idx << ": Pt = " << (*track)->pt() << ", eta = " << (*track)->eta() << ", phi = " << (*track)->phi() << std::endl;
    const reco::HitPattern& trackHitPattern = (*track)->hitPattern();
    if ( trackHitPattern.numberOfValidTrackerHits() >= (int)trackMinNumHits_ &&
	 trackHitPattern.numberOfValidPixelHits() >= (int)trackMinNumPixelHits_ &&
	 (*track)->normalizedChi2() < trackMaxChi2DoF_ &&
	 ((*track)->ptError()/(*track)->pt()) < trackMaxDeltaPoverP_ &&
	 (*track)->pt() > trackMinPt_ ) {
      if ( this->verbosity_ ) std::cout << " passes quality cuts." << std::endl;
      hypothesis->selTracks_.push_back(*track);
    } else {
      if ( this->verbosity_ ) std::cout << " FAILS quality cuts." << std::endl;
    }
    ++idx;
  }

  if ( hypothesis->selTracks_.size() >= 1 ) hypothesis->leadTrack_ = hypothesis->selTracks_.at(0);
  else hypothesis->leadTrack_ = 0;

  for ( std::vector<const reco::Track*>::const_iterator track = hypothesis->selTracks_.begin();
	track != hypothesis->selTracks_.end(); ++track ) {
    reco::TransientTrack trajectory = trackBuilder_->build(*track);
    hypothesis->selTrackTrajectories_.push_back(trajectory);
  }

  if ( hypothesis->selTrackTrajectories_.size() >= 1 ) hypothesis->leadTrackTrajectory_ = &hypothesis->selTrackTrajectories_.at(0);
  else hypothesis->leadTrackTrajectory_ = 0;

  if ( initializeToGen_ ) {
    bool isMatched = false;
    int idx = 0;
    for ( GenParticleView::const_iterator genParticle = genParticles_->begin();
	  genParticle != genParticles_->end() && !isMatched; ++genParticle ) {
      if ( verbosity_ ) {
	std::cout << "genParticle #" << idx << " (pdgId = " << genParticle->pdgId() << "):" 
		  << " Pt = " << genParticle->pt() << ", eta = " << genParticle->eta() << ", phi = " << genParticle->phi() 
		  << " (status = " << genParticle->status() << ")" << std::endl;
      }
      if ( TMath::Abs(genParticle->pdgId()) == 15 ) {
	const reco::GenParticle* genTau = &(*genParticle);
	std::vector<const reco::GenParticle*> genTauDecayProducts;
	findDaughters(genTau, genTauDecayProducts, -1);
	reco::Candidate::LorentzVector genVisP4 = getVisMomentum(genTauDecayProducts);
	double dR = deltaR(genParticle->p4(), visCandidate->p4());
	if ( verbosity_ ) std::cout << "dR = " << dR << std::endl;
	if ( dR < dRmatch_ ) {
	  std::string genTauDecayMode = getGenTauDecayMode(genTau);
	  if ( verbosity_ ) std::cout << "genTauDecayMode = " << genTauDecayMode << std::endl;
	  std::string recTauDecayMode;
	  if      ( getDecayMode(visCandidate) == reco::PFTauDecayMode::tauDecaysElectron ) recTauDecayMode = "electron";
	  else if ( getDecayMode(visCandidate) == reco::PFTauDecayMode::tauDecayMuon      ) recTauDecayMode = "muon";
	  else                                                                              recTauDecayMode = "hadronic";
	  if ( verbosity_ ) std::cout << "recTauDecayMode = " << recTauDecayMode << std::endl;
	  // CV: check if reconstructed and generated decay modes match
	  //    (either electron, muon or hadronic)
    	  if ( (genTauDecayMode == "electron" && 
	        getDecayMode(visCandidate) == reco::PFTauDecayMode::tauDecaysElectron) ||
	       (genTauDecayMode == "muon" && 
	        getDecayMode(visCandidate) == reco::PFTauDecayMode::tauDecayMuon) ||
  	       (!(genTauDecayMode == "electron" || genTauDecayMode == "muon") && 
	        !(getDecayMode(visCandidate) == reco::PFTauDecayMode::tauDecaysElectron || getDecayMode(visCandidate) == reco::PFTauDecayMode::tauDecayMuon)) ) {
	    const reco::Candidate::LorentzVector& genTauP4 = genTau->p4();
	    genVisEnFracX_ = genVisP4.energy()/genTauP4.energy();
	    genPhiLab_ = phiLabFromLabMomenta(genTauP4, genVisP4);
	    reco::Candidate::LorentzVector genInvisP4 = getInvisMomentum(genTauDecayProducts);
	    genNuInvMass_ = genInvisP4.mass();
	    // CV: Add protection against rounding errors
	    if ( genNuInvMass_ < 0. ) genNuInvMass_ = 1.e-4;
	    reco::Candidate::Point genTauProdVertex = genTau->vertex();
	    reco::Candidate::Point genTauDecayVertex = getDecayVertex(genTau);
	    genDeltaR_ = TMath::Sqrt((genTauDecayVertex - genTauProdVertex).mag2());
	    genVisP4_ = genVisP4;
	    if ( verbosity_ ) {
	      std::cout << " found match." << std::endl;
	      std::cout << "initializing:" << std::endl;
	      std::cout << " genVisEnFracX = " << genVisEnFracX_ << std::endl;
	      std::cout << " genPhiLab = " << genPhiLab_ << std::endl;
	      std::cout << " genNuInvMass = " << genNuInvMass_ << std::endl;
	      std::cout << " genDeltaR = " << genDeltaR_ << std::endl;
	      std::cout << " genVisP4: Pt = " << genVisP4_.pt() << ", eta = " << genVisP4_.eta() << ", phi = " << genVisP4_.phi() << std::endl;
	    }
	    isMatched = true;	    
	  } 
	}
      }
      ++idx;
    }

    if ( !isMatched )
      throw cms::Exception("NSVfitTauDecayBuilder::initialize")
	<< "Failed to find gen. Tau matching reconstructed decay products:" 
	<< " Pt = " << visCandidate->pt() << ", eta = " << visCandidate->eta() << ", phi = " << visCandidate->phi() << " !!\n";
    
    if ( fixToGenVisP4_ ) {
      hypothesis->p4_         = genVisP4_;
      hypothesis->p3Vis_unit_ = genVisP4_.Vect().Unit();
      hypothesis->visMass_    = genVisP4_.mass();
    }

    if ( idxFitParameter_visEnFracX_  != -1 ) algorithm_->setFitParameterInitialValue(idxFitParameter_visEnFracX_, genVisEnFracX_);
    if ( idxFitParameter_phi_lab_     != -1 ) algorithm_->setFitParameterInitialValue(idxFitParameter_phi_lab_,    genPhiLab_);
    if ( idxFitParameter_nuInvMass_   != -1 ) algorithm_->setFitParameterInitialValue(idxFitParameter_nuInvMass_,  genNuInvMass_);
  }
}

void
NSVfitTauDecayBuilder::finalize(NSVfitSingleParticleHypothesis* hypothesis) const
{
  //std::cout << "<NSVfitTauDecayBuilder>:" << std::endl;
  //std::cout << " hypothesis " << hypothesis->name() << " #" << hypothesis->barcode() << ": " << hypothesis << std::endl;

  // Cast to the concrete tau decay hypothesis
  NSVfitTauDecayHypothesis* hypothesis_T = dynamic_cast<NSVfitTauDecayHypothesis*>(hypothesis);
  assert(hypothesis_T);

  const NSVfitEventHypothesis* event = hypothesis_T->mother()->eventHypothesis();
  assert(event);

  AlgebraicVector3 eventVertexPos = event->eventVertexPos();
  const AlgebraicMatrix33& eventVertexCov = event->eventVertexCov();
  if ( verbosity_ >= 2 ) {
    printVector("eventVertexPos", eventVertexPos);
    printMatrix("eventVertexCov", eventVertexCov);
  }

  hypothesis_T->expectedDecayDistance_  = 0.;
  hypothesis_T->expectedDecayVertexPos_ = eventVertexPos;
  hypothesis_T->expectedDecayVertexCov_ = eventVertexCov;

  hypothesis_T->hasDecayVertexFit_ = false;
  if ( idxFitParameter_deltaR_ != -1 ) {
    if ( hypothesis_T->selTracks_.size() >= minNumTracksFit ) {
      TransientVertex decayVertex = decayVertexFitAlgorithm_->fitSecondaryVertex(hypothesis_T->selTracks_);
      if ( decayVertex.isValid() ) {
	hypothesis_T->reconstructedDecayVertexPos_(0) = decayVertex.position().x();
	hypothesis_T->reconstructedDecayVertexPos_(1) = decayVertex.position().y();
	hypothesis_T->reconstructedDecayVertexPos_(2) = decayVertex.position().z();
	hypothesis_T->reconstructedDecayVertexCov_ = decayVertex.positionError().matrix_new();
	hypothesis_T->hasDecayVertexFit_ = true;
      
	AlgebraicMatrix33 reconstructedDecayVertex_wrt_eventVertexCov;
	for ( unsigned iRow = 0; iRow < 3; ++iRow ) {
	  for ( unsigned iColumn = 0; iColumn < 3; ++iColumn ) {
	    reconstructedDecayVertex_wrt_eventVertexCov(iRow, iColumn) = hypothesis_T->reconstructedDecayVertexCov_(iRow, iColumn) + eventVertexCov(iRow, iColumn);
	  }
	}
	double EigenValue1, EigenValue2, EigenValue3;
	extractEigenValues(reconstructedDecayVertex_wrt_eventVertexCov, EigenValue1, EigenValue2, EigenValue3);
	if ( EigenValue1 < 1.e-4 ) EigenValue1 = 1.e-4; // CV: assume resolution of tau decay vertex reconstruction 
	                                                //     to be never better than 100 microns in tau direction
	double sigma = TMath::Sqrt(EigenValue1);
      
	if ( verbosity_ >= 2 ) {
	  printVector("decayVertexPos", hypothesis_T->reconstructedDecayVertexPos_);
	  printMatrix("decayVertexCov", hypothesis_T->reconstructedDecayVertexCov_);
	  std::cout << "restricting fitParameter #" << idxFitParameter_deltaR_ << " to range " << (-5.*sigma) << ".." << (+5.*sigma) << std::endl;
	}

	double fitParameterLowerLimit = -5.*sigma;
	double fitParameterUpperLimit = +5.*sigma;
	if ( initializeToGen_ ) {
	  double recDeltaR = TMath::Sqrt(norm2(hypothesis_T->reconstructedDecayVertexPos_ - eventVertexPos));
	  algorithm_->setFitParameterInitialValue(idxFitParameter_deltaR_, genDeltaR_ - recDeltaR);
	  fitParameterLowerLimit = TMath::Min(fitParameterLowerLimit, -3*sigma + (genDeltaR_ - recDeltaR));
	  fitParameterUpperLimit = TMath::Max(fitParameterUpperLimit, +3*sigma + (genDeltaR_ - recDeltaR));
	} else {
	  algorithm_->setFitParameterInitialValue(idxFitParameter_deltaR_, 0.);
	}
	algorithm_->setFitParameterLimit(idxFitParameter_deltaR_, fitParameterLowerLimit, fitParameterUpperLimit);
	algorithm_->setFitParameterStepSize(idxFitParameter_deltaR_, 0.25*sigma);            
      }
    }
    if ( !hypothesis_T->hasDecayVertexFit_ ) {
      hypothesis_T->reconstructedDecayVertexPos_(0) = 0.;
      hypothesis_T->reconstructedDecayVertexPos_(1) = 0.;
      hypothesis_T->reconstructedDecayVertexPos_(2) = 0.;
      for ( int iRow = 0; iRow < 3; ++iRow ) {
	for ( int iColumn = 0; iColumn < 3; ++iColumn ) {
	  hypothesis_T->reconstructedDecayVertexCov_(iRow, iColumn) = 1.e+3;
	}
      }
      
      double fitParameterLowerLimit =  0.;
      double fitParameterUpperLimit = 10.;
      if ( initializeToGen_ ) { 
	double enTau_lab  = genVisP4_.energy()/genVisEnFracX_;
	double pTau_lab = TMath::Sqrt(square(enTau_lab) - tauLeptonMass2);
	double a = (pTau_lab/tauLeptonMass)*cTauLifetime;
	if ( verbosity_ >= 2 ) std::cout << "a(gen) = " << a << " --> setting fitParameter #" << idxFitParameter_deltaR_ << " to " << (genDeltaR_/a) << std::endl;
	algorithm_->setFitParameterInitialValue(idxFitParameter_deltaR_, genDeltaR_/a);
	fitParameterUpperLimit = TMath::Max(fitParameterUpperLimit, 5. + (genDeltaR_/a));
      } else {
	algorithm_->setFitParameterInitialValue(idxFitParameter_deltaR_, 1.);
      }
      algorithm_->setFitParameterLimit(idxFitParameter_deltaR_, fitParameterLowerLimit, fitParameterUpperLimit);
      algorithm_->setFitParameterStepSize(idxFitParameter_deltaR_, 0.25);
    }
  }
}

bool NSVfitTauDecayBuilder::applyFitParameter(NSVfitSingleParticleHypothesis* hypothesis, const double* param) const
{
  // Cast to the concrete tau decay hypothesis
  NSVfitTauDecayHypothesis* hypothesis_T = dynamic_cast<NSVfitTauDecayHypothesis*>(hypothesis);
  assert(hypothesis_T);
  
  double visEnFracX = param[idxFitParameter_visEnFracX_];
  double phi_lab    = param[idxFitParameter_phi_lab_];
  double pVis_lab   = hypothesis_T->p4().P();
  double enVis_lab  = hypothesis_T->p4().energy();
  double visMass    = hypothesis_T->visMass();
  double nuInvMass  = ( nuSystemIsMassless() ) ? 0. : param[idxFitParameter_nuInvMass_];

  if ( fixToGenVisEnFracX_ ) visEnFracX = genVisEnFracX_;
  if ( fixToGenPhiLab_     ) phi_lab    = genPhiLab_;
  if ( fixToGenNuInvMass_  ) nuInvMass  = genNuInvMass_;

  const reco::Candidate::Vector& p3Vis_unit = hypothesis_T->p3Vis_unit();
  
  bool isValidSolution = true;

//--- compute energy and momentum of visible decay products in tau lepton rest-frame
  double pVis2_rf = (tauLeptonMass2 - square(visMass + nuInvMass))*(tauLeptonMass2 - square(visMass - nuInvMass))/(4.*tauLeptonMass2);
  if ( pVis2_rf < 0. ) {
    pVis2_rf = 0.;
    isValidSolution = false;
  }
  double pVis_rf = TMath::Sqrt(pVis2_rf);
  double enVis_rf = TMath::Sqrt(square(pVis_rf) + square(visMass));

//--- compute relativistic beta and gamma factors
  double beta2 = 1. - square(visEnFracX*tauLeptonMass/enVis_lab);
  assert(beta2 >= 0.);
  double beta = TMath::Sqrt(beta2);
  double gamma2 = 1./(1. - beta2);

//--- compute Gottfried-Jackson angle
//   (angle of visible decay products wrt. tau lepton flight direction)
  double cosGjAngle_rf = (visEnFracX*tauLeptonMass - enVis_rf)/(beta*pVis_rf);  
  if        ( cosGjAngle_rf < -1. ) {
    if ( verbosity_ >= 2 ) std::cout << "cosGjAngle_rf = " << cosGjAngle_rf << " --> setting isValidSolution = false." << std::endl;
    cosGjAngle_rf = -1.;
    isValidSolution = false;
  } else if ( cosGjAngle_rf > +1. ) {
    if ( verbosity_ >= 2 ) std::cout << "cosGjAngle_rf = " << cosGjAngle_rf << " --> setting isValidSolution = false." << std::endl;
    cosGjAngle_rf = +1.;
    isValidSolution = false;
  }
  double gjAngle_rf = TMath::ACos(cosGjAngle_rf);

//--- compute component of visible momentum perpendicular to tau flight direction
//   (NB: the perpendicular component of the visible momentum vector
//        is invariant under Lorentz boost in tau direction, 
//        i.e. is identical in tau rest-frame and laboratory frame)
  double pVis_perp = pVis_rf*TMath::Sin(gjAngle_rf);

  double gjAngle_lab = TMath::ASin(pVis_perp/pVis_lab);

//--- compute tau lepton energy and momentum in laboratory frame  
  double pTau_lab = TMath::Sqrt(gamma2 - 1.)*tauLeptonMass;
  double enTau_lab = TMath::Sqrt(square(pTau_lab) + tauLeptonMass2);
  reco::Candidate::Vector p3Tau_unit = motherDirection(p3Vis_unit, gjAngle_lab, phi_lab);
  reco::Candidate::LorentzVector p4Tau_lab = reco::Candidate::LorentzVector(
    pTau_lab*p3Tau_unit.x(), pTau_lab*p3Tau_unit.y(), pTau_lab*p3Tau_unit.z(), enTau_lab);

  hypothesis_T->p4_fitted_  = p4Tau_lab;
  hypothesis_T->dp4_        = (p4Tau_lab - hypothesis_T->p4_);

  reco::Candidate::Vector boostToTauRF_vector = p4Tau_lab.BoostToCM();
  hypothesis_T->p4invis_rf_ = ROOT::Math::VectorUtil::boost(hypothesis_T->dp4_, boostToTauRF_vector);
  hypothesis_T->p4vis_rf_   = ROOT::Math::VectorUtil::boost(hypothesis_T->p4(), boostToTauRF_vector);

  hypothesis_T->visEnFracX_ = visEnFracX;
  hypothesis_T->gjAngle_    = gjAngle_rf;
  hypothesis_T->phiLab_     = phi_lab;
  
//--- update track based information
  if ( idxFitParameter_deltaR_ != -1 ) {
    const NSVfitEventHypothesis* event = hypothesis_T->mother()->eventHypothesis();
    AlgebraicVector3 eventVertexPos = event->eventVertexPos();
    if ( verbosity_ >= 2  ) printVector(" eventVertexPos", eventVertexPos);
    
    hypothesis_T->expectedFlightPath_unit_ = AlgebraicVector3(p3Tau_unit.x(), p3Tau_unit.y(), p3Tau_unit.z());
    hypothesis_T->expectedDecayDistanceJacobiFactor_ = 1.;
    if ( hypothesis_T->hasDecayVertexFit_ ) {      
      if ( !fixToGenDeltaR_ ) {
	double decayDistance0 = TMath::Sqrt(norm2(hypothesis_T->reconstructedDecayVertexPos_ - eventVertexPos));
	double decayDistance_shift = param[idxFitParameter_deltaR_];
	hypothesis_T->expectedDecayDistance_ = decayDistance0 + decayDistance_shift;
      }
    } else if ( hypothesis_T->leadTrackTrajectory_ ) {
      if ( !fixToGenDeltaR_ ) {
	double a = (pTau_lab/tauLeptonMass)*cTauLifetime;
	hypothesis_T->expectedDecayDistanceJacobiFactor_ = a;
	hypothesis_T->expectedDecayDistance_ = param[idxFitParameter_deltaR_]*a;
      }
    }
    if ( hypothesis_T->expectedDecayDistance_ < 0. || hypothesis_T->expectedDecayDistance_ > 25. ) {
      if ( verbosity_ >= 2 ) std::cout << "expectedDecayDistance = " << hypothesis_T->expectedDecayDistance_ << " --> setting isValidSolution = false." << std::endl;
      hypothesis_T->expectedDecayDistance_ = 0.;
      isValidSolution = false;
    }
    
    if ( isValidSolution ) {
      //hypothesis_T->expectedDecayVertexPos_ = compDecayPosition_line(eventVertexPos, hypothesis_T->expectedDecayDistance_, hypothesis_T->expectedFlightPath_unit_);
      hypothesis_T->expectedDecayVertexPos_ = compDecayPosition_helix(eventVertexPos, hypothesis_T->expectedDecayDistance_, p4Tau_lab, hypothesis_T->particle()->charge());

      if ( hypothesis_T->leadTrackTrajectory_ && !hypothesis_T->hasDecayVertexFit_ ) {      	
	SVfitTrackExtrapolation leadTrack_extrapolation(*hypothesis_T->leadTrackTrajectory_, hypothesis_T->expectedDecayVertexPos_);
	hypothesis_T->reconstructedDecayVertexPos_ = leadTrack_extrapolation.point_of_closest_approach();
	hypothesis_T->reconstructedDecayVertexCov_ = leadTrack_extrapolation.covariance();
	hypothesis_T->leadTrackExtrapolationError_ = leadTrack_extrapolation.errorFlag();
	if ( verbosity_ >= 2  ) {
	  std::cout << "SVfitTrackExtrapolation:" << std::endl;
	  printVector("reconstructedDecayVertexPos", hypothesis_T->reconstructedDecayVertexPos_);
	  printMatrix("reconstructedDecayVertexCov", hypothesis_T->reconstructedDecayVertexCov_);
	}
      }
    }
  }

  if ( verbosity_ ) {
    std::cout << "<NSVfitTauDecayBuilder::applyFitParameter>:" << std::endl;
    std::cout << " hypothesis " << hypothesis->name() << " #" << hypothesis->barcode() << ": " << hypothesis << std::endl;
    std::cout << " visEnFracX = " << visEnFracX << std::endl;
    std::cout << " phi_lab = " << phi_lab << std::endl;
    std::cout << " visMass = " << visMass << std::endl;
    std::cout << " nuInvMass = " << hypothesis_T->p4invis_rf_.mass() << std::endl;
    std::cout << " gjAngle_rf = " << gjAngle_rf << std::endl;
    if ( idxFitParameter_deltaR_ != -1 ) std::cout << " decayDistance = " << hypothesis_T->expectedDecayDistance_ << std::endl;
    std::cout << "p4Vis (lab): E = " << hypothesis_T->p4_.energy() << ","
	      << " px = " << hypothesis_T->p4_.px() << ", py = " << hypothesis_T->p4_.py() << ","
	      << " pz = " << hypothesis_T->p4_.pz() << std::endl;
    std::cout << "p4Tau (lab): E = " << p4Tau_lab.energy() << ","
	      << " px = " << p4Tau_lab.px() << ", py = " << p4Tau_lab.py() << ","
	      << " pz = " << p4Tau_lab.pz() << std::endl;
    std::cout << "p4Vis (rf): E = " << hypothesis_T->p4vis_rf_.energy() << ","
	      << " mass = " << hypothesis_T->p4vis_rf_.mass()
	      << " px = " << hypothesis_T->p4vis_rf_.px() << ", py = " << hypothesis_T->p4vis_rf_.py() << ","
	      << " pz = " << hypothesis_T->p4vis_rf_.pz() << std::endl;
    std::cout << "p4Invis (rf): E = " << hypothesis_T->p4invis_rf_.energy() << ","
              << " mass = " << hypothesis_T->p4invis_rf_.mass()
	      << " px = " << hypothesis_T->p4invis_rf_.px() << ", py = " << hypothesis_T->p4invis_rf_.py() << ","
	      << " pz = " << hypothesis_T->p4invis_rf_.pz() << std::endl;
    std::cout << "isValidSolution = " << isValidSolution << std::endl;
  }

  hypothesis_T->isValidSolution_ = isValidSolution;

  return isValidSolution;
}

void NSVfitTauDecayBuilder::print(std::ostream& stream) const
{
  stream << "<NSVfitTauDecayBuilderBase::print>:" << std::endl;
  stream << " pluginName = " << pluginName_ << std::endl;
  stream << " pluginType = " << pluginType_ << std::endl;
  stream << " prodParticleLabel = " << prodParticleLabel_ << std::endl;
  stream << " idxFitParameter_visEnFracX = " << idxFitParameter_visEnFracX_ << std::endl;
  stream << " idxFitParameter_phi_lab = " << idxFitParameter_phi_lab_ << std::endl;
  stream << " idxFitParameter_nuInvMass = " << idxFitParameter_nuInvMass_ << std::endl;
}

//
//-------------------------------------------------------------------------------
//

void applyOptionalFitParameter(const double* param, int idxFitParameter, double& value)
{
  if   ( idxFitParameter != -1 ) value = param[idxFitParameter];
  else                           value = 0.;
}

