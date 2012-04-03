#include "TauAnalysis/CandidateTools/plugins/NSVfitTauToHadLikelihoodMatrixElement.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitAlgorithmBase.h"
#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitTauToHadHypothesis.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/TauReco/interface/PFTau.h"

#include <TMath.h>
#include <TH2.h>

using namespace SVfit_namespace;

void normalizeMatrixColumns(TMatrixD& matrix)
{
  unsigned numRows = matrix.GetNrows();
  unsigned numColumns = matrix.GetNcols();
  for ( unsigned iColumn = 0; iColumn < numColumns; ++iColumn ) {
    double sum = 0.;    
    for ( unsigned iRow = 0; iRow < numRows; ++iRow ) {
      sum += matrix(iRow, iColumn);
    }    
    if ( sum > 0. ) {
      for ( unsigned iRow = 0; iRow < numRows; ++iRow ) {
	matrix(iRow, iColumn) /= sum;
      }
    } else {
      edm::LogError("normalizeMatrixColumns") 
	<< " Sum of elements = " << sum << " for column = " << iColumn << " --> matrix will **not** be normalized !!";
    }
  }
}

NSVfitTauToHadLikelihoodMatrixElement::NSVfitTauToHadLikelihoodMatrixElement(const edm::ParameterSet& cfg)
  : NSVfitSingleParticleLikelihood(cfg),
    inputFile_(0),
    rhoLPlus_(0),
    rhoNormLPlus_(0),
    rhoLMinus_(0),
    rhoNormLMinus_(0),
    rhoTPlus_(0),
    rhoNormTPlus_(0),
    rhoTMinus_(0),
    rhoNormTMinus_(0),
    a1Lz_(0),
    a1Tz_(0),
    rhoyMinus_(0),
    rhoyNormMinus_(0),
    rhoyPlus_(0),
    rhoyNormPlus_(0),
    a1yMinus_(0),
    a1yNormMinus_(0),
    a1yPlus_(0),
    a1yNormPlus_(0)

{
  edm::FileInPath inputFileName = cfg.getParameter<edm::FileInPath>("VMshapeFileName");
  if ( !inputFileName.isLocal() ) 
    throw cms::Exception("NSVfitTauToHadLikelihoodMatrixElement") 
      << " Failed to find File = " << inputFileName << " !!\n";
  inputFile_ = new TFile(inputFileName.fullPath().data(), "READ");
  
  rhoLPlus_      = dynamic_cast<TGraph*>(inputFile_->Get("gRhoLPlus"));
  rhoLMinus_     = dynamic_cast<TGraph*>(inputFile_->Get("gRhoLMinus"));
  rhoNormLPlus_  = dynamic_cast<TGraph*>(inputFile_->Get("gNormRhoLPlus"));
  rhoNormLMinus_ = dynamic_cast<TGraph*>(inputFile_->Get("gNormRhoLMinus"));
  rhoTPlus_      = dynamic_cast<TGraph*>(inputFile_->Get("gRhoTPlus"));
  rhoTMinus_     = dynamic_cast<TGraph*>(inputFile_->Get("gRhoTMinus"));
  rhoNormTPlus_  = dynamic_cast<TGraph*>(inputFile_->Get("gNormRhoTPlus"));
  rhoNormTMinus_ = dynamic_cast<TGraph*>(inputFile_->Get("gNormRhoTMinus"));

  a1LPlus_       = dynamic_cast<TGraph*>(inputFile_->Get("gA1LPlus"));
  a1LMinus_      = dynamic_cast<TGraph*>(inputFile_->Get("gA1LMinus"));
  a1NormLPlus_   = dynamic_cast<TGraph*>(inputFile_->Get("gNormA1LPlus"));
  a1NormLMinus_  = dynamic_cast<TGraph*>(inputFile_->Get("gNormA1LMinus"));
  a1TPlus_       = dynamic_cast<TGraph*>(inputFile_->Get("gA1TPlus"));
  a1TMinus_      = dynamic_cast<TGraph*>(inputFile_->Get("gA1TMinus"));
  a1NormTPlus_   = dynamic_cast<TGraph*>(inputFile_->Get("gNormA1TPlus"));
  a1NormTMinus_  = dynamic_cast<TGraph*>(inputFile_->Get("gNormA1TMinus"));

  a1Lz_          = dynamic_cast<TGraph*>(inputFile_->Get("a1LzFraction"));
  a1Tz_          = dynamic_cast<TGraph*>(inputFile_->Get("a1TzFraction"));
  rhoyMinus_     = dynamic_cast<TGraph*>(inputFile_->Get("gRhoYMinus"));
  rhoyNormMinus_ = dynamic_cast<TGraph*>(inputFile_->Get("gNormRhoYMinus"));
  rhoyPlus_      = dynamic_cast<TGraph*>(inputFile_->Get("gRhoYPlus"));
  rhoyNormPlus_  = dynamic_cast<TGraph*>(inputFile_->Get("gNormRhoYPlus"));
  a1yMinus_      = dynamic_cast<TGraph*>(inputFile_->Get("gA1YMinus"));
  a1yNormMinus_  = dynamic_cast<TGraph*>(inputFile_->Get("gNormA1YMinus"));
  a1yPlus_       = dynamic_cast<TGraph*>(inputFile_->Get("gA1YPlus"));
  a1yNormPlus_   = dynamic_cast<TGraph*>(inputFile_->Get("gNormA1YPlus"));

  if ( !(rhoLPlus_      &&
	 rhoLMinus_     &&
	 rhoNormLPlus_  &&
	 rhoNormLMinus_ &&
	 rhoTPlus_      &&
	 rhoTMinus_     &&
	 rhoNormTPlus_  &&
	 rhoNormTMinus_ &&
	 a1LPlus_       &&
	 a1LMinus_      &&
	 a1NormLPlus_   &&
	 a1NormLMinus_  &&
	 a1TPlus_       &&
	 a1TMinus_      &&
	 a1NormTPlus_   &&
	 a1NormTMinus_  &&
	 a1Lz_          &&
	 a1Tz_          &&
	 rhoyMinus_     &&
	 rhoyNormMinus_ &&
	 rhoyPlus_      &&
	 rhoyNormPlus_  &&
	 a1yMinus_      &&
	 a1yNormMinus_  &&
	 a1yPlus_       &&
	 a1yNormPlus_) )    
    throw cms::Exception("NSVfitTauToHadLikelihoodMatrixElement") 
      << " Failed to load TGraph objects from File = " << inputFileName_ << " !!\n";
 
  numSupportedTauDecayModes_ = 4;
  supportedTauDecayModes_.resize(numSupportedTauDecayModes_);
  supportedTauDecayModes_[0] = reco::PFTau::kOneProng0PiZero;
  supportedTauDecayModes_[1] = reco::PFTau::kOneProng1PiZero;
  supportedTauDecayModes_[2] = reco::PFTau::kOneProng2PiZero;
  supportedTauDecayModes_[3] = reco::PFTau::kThreeProng0PiZero;

  vRec_.ResizeTo(numSupportedTauDecayModes_);
  recToGenTauDecayModeMap_.ResizeTo(numSupportedTauDecayModes_, numSupportedTauDecayModes_);
  recToGenTauDecayModeMap_.Zero();
  vGen_.ResizeTo(numSupportedTauDecayModes_);
  vProb_.ResizeTo(numSupportedTauDecayModes_);

//--- create "transfer matrix"
//
//    CV: In the histogram, the generated (reconstructed) tau decay modes are on the x-axis (y-axis);
//        the x-axis (y-axis) needs to be mapped to rows (columns) of the "transfer matrix".
//
//        The aim of the "transfer matrix" M is to map from reconstructed to generated ("true") tau lepton hadronic decay modes.
//        The mapping is implemented by matrix multiplication:
//         vGen = M * vRec,
//        where vRec is a vector encoding the reconstructed hadronic tau decay mode
//        (exactly one entry in this vector is 1, all other entries are 0)
//        and vGen gives the probabilities for the "true" decay mode of the tau lepton.
//
//        The required format of M is:
//         | p(gen=1Prong0Pi0|rec=1Prong0Pi0) .. p(gen=1Prong0Pi0|rec=3Prong0Pi0) p(gen=1Prong0Pi0|rec=other) |
//         | p(gen=1Prong1Pi0|rec=1Prong0Pi0) .. p(gen=1Prong1Pi0|rec=3Prong0Pi0) p(gen=1Prong1Pi0|rec=other) |
//         | p(gen=1Prong2Pi0|rec=1Prong0Pi0) .. p(gen=1Prong2Pi0|rec=3Prong0Pi0) p(gen=1Prong2Pi0|rec=other) |
//         | p(gen=3Prong0Pi0|rec=1Prong0Pi0) .. p(gen=3Prong0Pi0|rec=3Prong0Pi0) p(gen=3Prong0Pi0|rec=other) |
//         | p(gen=other     |rec=1Prong0Pi0) .. p(gen=other     |rec=3Prong0Pi0) p(gen=other     |rec=other) |
//
//        Note that **column** of the "transfer matrix" need to be normalized to unit probability.
//

//--- load histogram correlating reconstructed to generated hadronic decay modes
//    if it exists, else take "transfer matrix" to be diagonal
  if ( cfg.exists("recToGenTauDecayModeMapFileName") ) {
    edm::FileInPath inputFileName = cfg.getParameter<edm::FileInPath>("recToGenTauDecayModeMapFileName");
    if ( !inputFileName.isLocal() ) 
      throw cms::Exception("NSVfitTauToHadLikelihoodMatrixElement") 
	<< " Failed to find File = " << inputFileName << " !!\n";
    TFile* inputFile = new TFile(inputFileName.fullPath().data(), "READ");
    TH2* recToGenTauDecayModeMap_histogram = dynamic_cast<TH2*>(inputFile->Get("recToGenTauDecayModeMap"));
    if ( !recToGenTauDecayModeMap_histogram )
      throw cms::Exception("NSVfitTauToHadLikelihoodMatrixElement") 
	<< " Failed to load Histogram mapping reconstructed to generated Tau decay modes from File = " << inputFileName_ << " !!\n";
    for ( unsigned iRow = 0; iRow < numSupportedTauDecayModes_; ++iRow ) {
      for ( unsigned iColumn = 0; iColumn < numSupportedTauDecayModes_; ++iColumn ) {
	recToGenTauDecayModeMap_(iRow, iColumn) = recToGenTauDecayModeMap_histogram->GetBinContent(iColumn + 1, iRow + 1);
      }
    }
    normalizeMatrixColumns(recToGenTauDecayModeMap_);
    delete inputFile;
  } else {
    for ( unsigned iRow = 0; iRow < numSupportedTauDecayModes_; ++iRow ) {
      for ( unsigned iColumn = 0; iColumn < numSupportedTauDecayModes_; ++iColumn ) {
	if ( iColumn == iRow ) recToGenTauDecayModeMap_(iRow, iColumn) = 1.;
	else recToGenTauDecayModeMap_(iRow, iColumn) = 0.;
      }
    }
  }

  if ( verbosity_ ) {
    std::cout << " recToGenTauDecayModeMap:" << std::endl;
    recToGenTauDecayModeMap_.Print();
  }
 
  applySinThetaFactor_ = cfg.exists("applySinThetaFactor") ?
    cfg.getParameter<bool>("applySinThetaFactor") : false;
}

NSVfitTauToHadLikelihoodMatrixElement::~NSVfitTauToHadLikelihoodMatrixElement()
{
  delete inputFile_;

  delete rhoLPlus_; 
  delete rhoNormLPlus_; 
  delete rhoLMinus_; 
  delete rhoNormLMinus_; 
  delete rhoTPlus_; 
  delete rhoNormTPlus_;
  delete rhoTMinus_; 
  delete rhoNormTMinus_;
  delete a1LPlus_; 
  delete a1NormLPlus_; 
  delete a1LMinus_; 
  delete a1NormLMinus_; 
  delete a1TPlus_; 
  delete a1NormTPlus_;
  delete a1TMinus_; 
  delete a1NormTMinus_;
  delete a1Lz_;
  delete a1Tz_;
  delete rhoyMinus_;
  delete rhoyNormMinus_;
  delete rhoyPlus_;
  delete rhoyNormPlus_;
  delete a1yMinus_;
  delete a1yNormMinus_;
  delete a1yPlus_;
  delete a1yNormPlus_;
}

void NSVfitTauToHadLikelihoodMatrixElement::beginJob(NSVfitAlgorithmBase* algorithm)
{
  algorithm->requestFitParameter(prodParticleLabel_, nSVfit_namespace::kTau_visEnFracX, pluginName_);
  algorithm->requestFitParameter(prodParticleLabel_, nSVfit_namespace::kTau_phi_lab,    pluginName_);
}

void NSVfitTauToHadLikelihoodMatrixElement::beginCandidate(const NSVfitSingleParticleHypothesis* hypothesis)
{
  const NSVfitTauToHadHypothesis* hypothesis_T = dynamic_cast<const NSVfitTauToHadHypothesis*>(hypothesis);
  assert(hypothesis_T != 0);

  if ( this->verbosity_ ) std::cout << "<NSVfitTauToHadLikelihoodMatrixElement::beginCandidate>:" << std::endl;

  const pat::Tau* tauJet = dynamic_cast<const pat::Tau*>(hypothesis_T->particle().get());
  assert(tauJet);

  int tauDecayMode = tauJet->decayMode();
  for ( unsigned iDecayMode = 0; iDecayMode < numSupportedTauDecayModes_; ++iDecayMode ) {
    if ( tauDecayMode == supportedTauDecayModes_[iDecayMode] ) vRec_(iDecayMode) = 1.;
    else vRec_(iDecayMode) = 0.;
  }

  if ( verbosity_ ) {
    std::cout << " vRec:" << std::endl;
    vRec_.Print();
  }

  vGen_ = vRec_;
  vGen_ *= recToGenTauDecayModeMap_;

  if ( verbosity_ ) {
    std::cout << " vGen:" << std::endl;
    vGen_.Print();
  }

  numWarningsUnphysicalDecay_rho_ = 0;
  numWarningsUnphysicalDecay_a1_  = 0;
}

double getZdistinguishablePion_oneProngHypothesis(const pat::Tau* tauJet)
{
//--- find "distinguishable" pion
//   (assuming reconstructed tau-jet is due to a 1-prong decay)
  double z = tauJet->leadPFChargedHadrCand()->energy()/tauJet->energy();
  if ( z > 1. || z < 0. ) {
    edm::LogWarning ("getZdistinguishablePion_oneProngHypothesis")
      << "Momentum of tau constituent exceeds tau-jet momentum !!" << std::endl; 
    z = 0.5;
  }
  return z;
}

double NSVfitTauToHadLikelihoodMatrixElement::compProb_pionDecay(double visEnFracX, int polSign, double tauLeptonPt) const
{
//--- compute probability for observed tau decay products 
//    to be compatible with tau- --> pi- nu decay

  double prob = 1. + polSign*(2.*visEnFracX - 1.);
  if ( applyVisPtCutCorrection_ ){ 
    double probCorr = 1.;
    const double epsilon_regularization = 1.e-3;
    if ( tauLeptonPt > visPtCutThreshold_ ) {     
      double xCut = visPtCutThreshold_/tauLeptonPt;
      probCorr = (1./(0.5*(1. + polSign)*(1. - square(xCut) + epsilon_regularization) 
                    + 0.5*(1. - polSign)*square(1. - xCut + epsilon_regularization)));
    }
    prob *= probCorr;
  }
  return prob;
}

double NSVfitTauToHadLikelihoodMatrixElement::compProb_rhoDecay(double visEnFracX, double z, int polSign, double tauLeptonPt) const
{
//--- compute probability for observed tau decay products 
//    to be compatible with tau- --> rho- nu --> pi- pi0 nu decay

  double probLz = 3.*square(2.*z - 1.);
  double probTz = 6.*z*(1. - z);
  double probLx = (polSign == +1) ? rhoLPlus_->Eval(visEnFracX) : rhoLMinus_->Eval(visEnFracX);
  double probTx = (polSign == +1) ? rhoTPlus_->Eval(visEnFracX) : rhoTMinus_->Eval(visEnFracX);

  double xCut = ( applyVisPtCutCorrection_ ) ?
    visPtCutThreshold_/tauLeptonPt : 0.0; 

  double mL = ( polSign == +1 ) ? 
    (rhoNormLPlus_->Eval(1.0)  - rhoNormLPlus_->Eval(xCut)) : 
    (rhoNormLMinus_->Eval(1.0) - rhoNormLMinus_->Eval(xCut));
  double mT = ( polSign == +1 ) ? 
    (rhoNormTPlus_->Eval(1.0)  - rhoNormTPlus_->Eval(xCut)) :
    (rhoNormTMinus_->Eval(1.0) - rhoNormTMinus_->Eval(xCut));
  if ( mL <= 0. && mT <= 0. ) {
    if ( numWarningsUnphysicalDecay_rho_ < 3 ) 
      edm::LogWarning ("NSVfitTauToHadLikelihoodMatrixElement::compProb_rhoDecay")
	<< "Failed to compute vector meson line-shape integral, xCut = " << xCut << " !!" << std::endl;
    mL = 1.e-3;
    mT = 1.e-3;
    ++numWarningsUnphysicalDecay_rho_;
  }
  double prob = (probLz*probLx + probTz*probTx)/(mL*probLz + mT*probTz);
  return prob;
}

double getZdistinguishablePion_threeProngHypothesis(const pat::Tau* tauJet)
{
//--- find "distinguishable" pion
//   (assuming reconstructed tau-jet is due to a 3-prong decay)
  double z = -1.;
  const reco::PFCandidateRefVector& tauSignalPFChargedHadrons = tauJet->signalPFChargedHadrCands();
  if ( tauSignalPFChargedHadrons.size() == 1 ) {
    z = tauJet->leadPFChargedHadrCand()->energy()/tauJet->energy();
  } else if ( tauSignalPFChargedHadrons.size() == 3 ) {
    int tauCharge = tauJet->charge();
    for ( reco::PFCandidateRefVector::const_iterator tauSignalPFChargedHadron = tauSignalPFChargedHadrons.begin();
	  tauSignalPFChargedHadron != tauSignalPFChargedHadrons.end(); ++tauSignalPFChargedHadron ) {
      if ( ((*tauSignalPFChargedHadron)->charge()*tauCharge) < -0.5 ) z = (*tauSignalPFChargedHadron)->energy()/tauJet->energy();
    }
  }
  if ( z > 1. || z < 0. ) {
    edm::LogWarning ("getZdistinguishablePion_threeProngHypothesis")
      << "Momentum of tau constituent exceeds tau-jet momentum !!" << std::endl; 
    z = 0.5;
  }
  return z;
}

double NSVfitTauToHadLikelihoodMatrixElement::compProb_a1Decay(double visEnFracX, double z, int polSign, double tauLeptonPt) const
{
//--- compute probability for observed tau decay products 
//    to be compatible with tau- --> a1- nu --> pi- pi0 p0 nu and tau- --> a1- nu --> pi+ pi- pi- nu decays

  double probLz = a1Lz_->Eval(z);
  double probTz = a1Tz_->Eval(z);
  if ( probLz <= 0. && probTz <= 0. ){
    // if both prob are 0, then use f(x) instead of f(x|z)
    probLz = 1.;
    probTz = 1.;
  }

  double probLx = (polSign == +1) ? a1LPlus_->Eval(visEnFracX) : a1LMinus_->Eval(visEnFracX);
  double probTx = (polSign == +1) ? a1TPlus_->Eval(visEnFracX) : a1TMinus_->Eval(visEnFracX);

  double xCut = ( applyVisPtCutCorrection_ ) ?
    visPtCutThreshold_/tauLeptonPt : 0.0; 

  double mL = ( polSign == +1 ) ? 
    (a1NormLPlus_->Eval(1.0)  - a1NormLPlus_->Eval(xCut)) : 
    (a1NormLMinus_->Eval(1.0) - a1NormLMinus_->Eval(xCut));
  double mT = ( polSign == +1 ) ? 
    (a1NormTPlus_->Eval(1.0)  - a1NormTPlus_->Eval(xCut)) :
    (a1NormTMinus_->Eval(1.0) - a1NormTMinus_->Eval(xCut));
  if ( mL <= 0. && mT <= 0. ) {
    if ( numWarningsUnphysicalDecay_a1_ < 3 ) 
      edm::LogWarning ("NSVfitTauToHadLikelihoodMatrixElement::compProb_a1Decay")
	<< "Failed to compute vector meson line-shape integral, xCut = " << xCut << " !!" << std::endl;
    mL = 1.e-3;
    mT = 1.e-3;
    ++numWarningsUnphysicalDecay_a1_;
  }
  double prob = (probLz*probLx + probTz*probTx)/(mL*probLz + mT*probTz);
  return prob;
}

double NSVfitTauToHadLikelihoodMatrixElement::operator()(const NSVfitSingleParticleHypothesis* hypothesis, int polSign) const
{
//--- compute negative log-likelihood for tau lepton decay "leg"
//    to be compatible with decay tau- --> X nu of polarized tau lepton into hadrons,
//    assuming  matrix element of V-A electroweak decay
//
//    NOTE: The formulas taken from the papers
//         [1] "Tau polarization and its correlations as a probe of new physics",
//             B.K. Bullock, K. Hagiwara and A.D. Martin,
//             Nucl. Phys. B395 (1993) 499.
//         [2] "Charged Higgs boson search at the TeVatron upgrade using tau polarization",
//             S. Raychaudhuri and D.P. Roy,
//             Phys. Rev.  D52 (1995) 1556.           
//

  const NSVfitTauToHadHypothesis* hypothesis_T = dynamic_cast<const NSVfitTauToHadHypothesis*>(hypothesis);
  assert(hypothesis_T != 0);

  if ( this->verbosity_ ) std::cout << "<NSVfitTauToHadLikelihoodMatrixElement::operator()>:" << std::endl;
  
  double decayAngle = hypothesis_T->decay_angle_rf();  
  double visEnFracX = hypothesis_T->visEnFracX();
  double visMass = hypothesis_T->p4vis_rf().mass();
  if ( visMass < chargedPionMass ) visMass = chargedPionMass;
  if ( visMass > tauLeptonMass   ) visMass = tauLeptonMass;
  double visMass2 = square(visMass);

  if ( this->verbosity_ ) {
    std::cout << " decayAngle = " << decayAngle << std::endl;
    std::cout << " visMass = " << visMass << std::endl;
    std::cout << " visEnFracX = " << visEnFracX << std::endl;
  }

  if ( !(polSign == +1 || polSign == -1) )
    throw cms::Exception("NSVfitTauToHadLikelihoodMatrixElement") 
      << " Invalid polarization = " << polSign << " !!\n";

  const pat::Tau* tauJet = dynamic_cast<const pat::Tau*>(hypothesis_T->particle().get());
  assert(tauJet);

  double tauLeptonPt = hypothesis_T->p4_fitted().pt();
  // compute fraction of a1 meson energy carried by "distinguishable" pion
  // (= charged pion for tau- --> a1- nu --> pi- pi0 p0 nu decay,
  //    pion with charge opposite to that of tau lepton charge for tau- --> a1- nu --> pi+ pi- pi- nu decay)
  double z_oneProngHypothesis = getZdistinguishablePion_oneProngHypothesis(tauJet);
  double z_threeProngHypothesis = getZdistinguishablePion_threeProngHypothesis(tauJet);

  vProb_(0) = compProb_pionDecay(visEnFracX, polSign, tauLeptonPt);
  vProb_(1) = compProb_rhoDecay(visEnFracX, z_oneProngHypothesis, polSign, tauLeptonPt);
  vProb_(2) = compProb_a1Decay(visEnFracX, z_oneProngHypothesis, polSign, tauLeptonPt);
  vProb_(3) = compProb_a1Decay(visEnFracX, z_threeProngHypothesis, polSign, tauLeptonPt);

  if ( this->verbosity_ ) {
    std::cout << " vProb:" << std::endl;
    vProb_.Print();
  }

  double prob = (vGen_*vProb_);
  if ( visEnFracX < (visMass2/tauLeptonMass2) ) {
    double visEnFracX_limit = visMass2/tauLeptonMass2;
    prob /= (1. + 1.e+6*square(visEnFracX - visEnFracX_limit));
  } else if ( visEnFracX > 1. ) {
    double visEnFracX_limit = 1.;
    prob /= (1. + 1.e+6*square(visEnFracX - visEnFracX_limit));
  }
  if ( applySinThetaFactor_ ) prob *= (0.5*TMath::Sin(decayAngle));
  
  double nll = 0.;
  if ( prob > 0. ) {
    nll = -TMath::Log(prob);
  } else {
    nll = std::numeric_limits<float>::max();
  }
  
  if ( this->verbosity_ ) std::cout << "--> nll = " << nll << std::endl;

  return nll;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(NSVfitSingleParticleLikelihoodPluginFactory, NSVfitTauToHadLikelihoodMatrixElement, "NSVfitTauToHadLikelihoodMatrixElement");
