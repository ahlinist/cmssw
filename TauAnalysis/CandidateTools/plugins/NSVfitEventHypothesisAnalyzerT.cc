#include "TauAnalysis/CandidateTools/plugins/NSVfitEventHypothesisAnalyzerT.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesisBase.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesis.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesisByIntegration.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleParticleHypothesis.h"
#include "AnalysisDataFormats/TauAnalysis/interface/PFMEtSignCovMatrix.h"

#include <TMatrixD.h>
#include <TVectorD.h>

template <typename T>
NSVfitEventHypothesisAnalyzerT<T>::NSVfitEventHypothesisAnalyzerT(const edm::ParameterSet& cfg)
  : moduleLabel_(cfg.getParameter<std::string>("@module_label")),
    numEvents_processed_(0),
    numEventsWeighted_processed_(0.)
{
  srcEventHypotheses_ = cfg.getParameter<edm::InputTag>("srcEventHypotheses");

  srcGenLeg1_ = cfg.getParameter<edm::InputTag>("srcGenLeg1");
  srcGenLeg2_ = cfg.getParameter<edm::InputTag>("srcGenLeg2");
  srcGenMEt_ = cfg.getParameter<edm::InputTag>("srcGenMEt");

  srcPFMEtCovMatrix_ = cfg.getParameter<edm::InputTag>("srcPFMEtCovMatrix");

  srcWeights_ = cfg.getParameter<vInputTag>("srcWeights");

  idxResonance_ = ( cfg.exists("idxResonance") ) ?
    cfg.getParameter<int>("idxResonance") : 0;

  numBinsSVfitMass_ = cfg.getParameter<int>("numBinsSVfitMass");
  svFitMassMax_ = cfg.getParameter<double>("svFitMassMax");
  numBinsSVfitSigma_ = cfg.getParameter<int>("numBinsSVfitSigma");
  svFitSigmaMax_ = cfg.getParameter<double>("svFitSigmaMax");

  dqmDirectory_ = cfg.getParameter<std::string>("dqmDirectory");
}

template <typename T>
NSVfitEventHypothesisAnalyzerT<T>::~NSVfitEventHypothesisAnalyzerT()
{
  for ( typename std::vector<plotEntryType1*>::iterator it = plotEntries1_.begin();
	it != plotEntries1_.end(); ++it ) {
    delete (*it);
  }

  for ( typename std::vector<plotEntryType2*>::iterator it = plotEntries2_.begin();
	it != plotEntries2_.end(); ++it ) {
    delete (*it);
  }
}

template <typename T>
void NSVfitEventHypothesisAnalyzerT<T>::beginJob()
{
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    throw cms::Exception("NSVfitEventHypothesisAnalyzer") 
      << " Failed to access dqmStore --> histograms will NEITHER be booked NOR filled !!\n";
  }

  DQMStore& dqmStore = (*edm::Service<DQMStore>());

  dqmStore.setCurrentFolder(dqmDirectory_.data());

  svFitIsValidSolution_ = dqmStore.book1D("svFitIsValidSolution", "svFitIsValidSolution",  2, -0.5, 1.5);

  svFitMassVsSigmaXL_ = 
    dqmStore.book2D("svFitMassVsVisMassXL", 
		    "svFitMassVsVisMassXL", 
		    TMath::Nint(svFitSigmaMax_), 0., svFitSigmaMax_, TMath::Nint(svFitMassMax_), 0., svFitMassMax_);
  svFitMassVsSigmaXL_oneProng0pi0_ = 
    dqmStore.book2D("svFitMassVsVisMassXL_oneProng0pi0", 
		    "svFitMassVsVisMassXL_oneProng0pi0", 
		    TMath::Nint(svFitSigmaMax_), 0., svFitSigmaMax_, TMath::Nint(svFitMassMax_), 0., svFitMassMax_);
  svFitMassVsSigmaXL_oneProng1pi0_ = 
    dqmStore.book2D("svFitMassVsVisMassXL_oneProng1pi0", 
		    "svFitMassVsVisMassXL_oneProng1pi0", 
		    TMath::Nint(svFitSigmaMax_), 0., svFitSigmaMax_, TMath::Nint(svFitMassMax_), 0., svFitMassMax_);
  svFitMassVsSigmaXL_oneProng2pi0_ = 
    dqmStore.book2D("svFitMassVsVisMassXL_oneProng2pi0", 
		    "svFitMassVsVisMassXL_oneProng2pi0", 
		    TMath::Nint(svFitSigmaMax_), 0., svFitSigmaMax_, TMath::Nint(svFitMassMax_), 0., svFitMassMax_);
  svFitMassVsSigmaXL_threeProng0pi0_ = 
    dqmStore.book2D("svFitMassVsVisMassXL_threeProng0pi0", 
		    "svFitMassVsVisMassXL_threeProng0pi0", 
		    TMath::Nint(svFitSigmaMax_), 0., svFitSigmaMax_, TMath::Nint(svFitMassMax_), 0., svFitMassMax_);
  
  plotEntries1_.push_back(new plotEntryType1(
    dqmDirectory_,  -1.,  -1.,  0, numBinsSVfitMass_, svFitMassMax_, numBinsSVfitSigma_, svFitSigmaMax_));
  plotEntries1_.push_back(new plotEntryType1(
    dqmDirectory_,  -1.,  -1., +1, numBinsSVfitMass_, svFitMassMax_, numBinsSVfitSigma_, svFitSigmaMax_));
  plotEntries1_.push_back(new plotEntryType1(
    dqmDirectory_,  -1.,  -1., -1, numBinsSVfitMass_, svFitMassMax_, numBinsSVfitSigma_, svFitSigmaMax_));
  plotEntries1_.push_back(new plotEntryType1(
    dqmDirectory_,   0.,  30.,  0, numBinsSVfitMass_, svFitMassMax_, numBinsSVfitSigma_, svFitSigmaMax_)); 
  plotEntries1_.push_back(new plotEntryType1(
    dqmDirectory_,  30.,  60.,  0, numBinsSVfitMass_, svFitMassMax_, numBinsSVfitSigma_, svFitSigmaMax_)); 
  plotEntries1_.push_back(new plotEntryType1(
    dqmDirectory_,  60.,  90.,  0, numBinsSVfitMass_, svFitMassMax_, numBinsSVfitSigma_, svFitSigmaMax_)); 
  plotEntries1_.push_back(new plotEntryType1(
    dqmDirectory_,  90., 120.,  0, numBinsSVfitMass_, svFitMassMax_, numBinsSVfitSigma_, svFitSigmaMax_)); 
  plotEntries1_.push_back(new plotEntryType1(
    dqmDirectory_, 120., 140.,  0, numBinsSVfitMass_, svFitMassMax_, numBinsSVfitSigma_, svFitSigmaMax_)); 
  plotEntries1_.push_back(new plotEntryType1(
    dqmDirectory_, 140., 160.,  0, numBinsSVfitMass_, svFitMassMax_, numBinsSVfitSigma_, svFitSigmaMax_)); 
  plotEntries1_.push_back(new plotEntryType1(
    dqmDirectory_, 160., 170.,  0, numBinsSVfitMass_, svFitMassMax_, numBinsSVfitSigma_, svFitSigmaMax_)); 
  plotEntries1_.push_back(new plotEntryType1(
    dqmDirectory_, 170., 175.,  0, numBinsSVfitMass_, svFitMassMax_, numBinsSVfitSigma_, svFitSigmaMax_)); 
  plotEntries1_.push_back(new plotEntryType1(
    dqmDirectory_, 175.,  -1.,  0, numBinsSVfitMass_, svFitMassMax_, numBinsSVfitSigma_, svFitSigmaMax_)); 
  for ( typename std::vector<plotEntryType1*>::iterator plotEntry = plotEntries1_.begin();
	plotEntry != plotEntries1_.end(); ++plotEntry ) {
    (*plotEntry)->bookHistograms(dqmStore);
  }

  plotEntries2_.push_back(
    new plotEntryType2(dqmDirectory_,  -1.,  -1.,  0, numBinsSVfitMass_, svFitMassMax_, numBinsSVfitSigma_, svFitSigmaMax_));
  plotEntries2_.push_back(
    new plotEntryType2(dqmDirectory_,  -1.,  -1., +1, numBinsSVfitMass_, svFitMassMax_, numBinsSVfitSigma_, svFitSigmaMax_));
  plotEntries2_.push_back(
    new plotEntryType2(dqmDirectory_,  -1.,  -1., -1, numBinsSVfitMass_, svFitMassMax_, numBinsSVfitSigma_, svFitSigmaMax_));
  plotEntries2_.push_back(
    new plotEntryType2(dqmDirectory_,   0.,  15.,  0, numBinsSVfitMass_, svFitMassMax_, numBinsSVfitSigma_, svFitSigmaMax_));
  plotEntries2_.push_back(
    new plotEntryType2(dqmDirectory_,  15.,  25.,  0, numBinsSVfitMass_, svFitMassMax_, numBinsSVfitSigma_, svFitSigmaMax_));
  plotEntries2_.push_back(
    new plotEntryType2(dqmDirectory_,  25.,  35.,  0, numBinsSVfitMass_, svFitMassMax_, numBinsSVfitSigma_, svFitSigmaMax_));
  plotEntries2_.push_back(
    new plotEntryType2(dqmDirectory_,  35.,  45.,  0, numBinsSVfitMass_, svFitMassMax_, numBinsSVfitSigma_, svFitSigmaMax_));
  plotEntries2_.push_back(
    new plotEntryType2(dqmDirectory_,  45.,  -1.,  0, numBinsSVfitMass_, svFitMassMax_, numBinsSVfitSigma_, svFitSigmaMax_));
  for ( typename std::vector<plotEntryType2*>::iterator plotEntry = plotEntries2_.begin();
	plotEntry != plotEntries2_.end(); ++plotEntry ) {
    (*plotEntry)->bookHistograms(dqmStore);
  }
}

namespace
{
  double square(double x)
  {
    return x*x;
  }

  double compProjVecXY(const reco::Candidate::LorentzVector& p, const reco::Candidate::LorentzVector& dir)
  {
    double retVal = 0.;
    if ( dir.pt() > 0. ) {
      retVal = (p.px()*dir.px() + p.py()*dir.py())/dir.pt();
    }
    return retVal;
  }

  double compProjCovUncertaintyXY(const TMatrixD& cov, const reco::Candidate::LorentzVector& dir)
  {
    double retVal = 0.;
    if ( dir.pt() > 0. ) {
      retVal = TMath::Sqrt(dir.px()*(cov(0,0)*dir.px() + cov(0,1)*dir.py())
			 + dir.py()*(cov(1,0)*dir.px() + cov(1,1)*dir.py()))/dir.pt();
    }
    return retVal;
  }
}

template <typename T>
void NSVfitEventHypothesisAnalyzerT<T>::analyze(const edm::Event& evt, const edm::EventSetup& es)
{
  edm::Handle<NSVfitEventHypothesisCollection> svFitEventHypotheses;
  evt.getByLabel(srcEventHypotheses_, svFitEventHypotheses);

  typedef edm::View<reco::Candidate> CandidateView;
  edm::Handle<CandidateView> genLeg1;
  evt.getByLabel(srcGenLeg1_, genLeg1);
  edm::Handle<CandidateView> genLeg2;
  evt.getByLabel(srcGenLeg2_, genLeg2);

  int numGenTauDecays = genLeg1->size();
  if ( srcGenLeg2_.label() != srcGenLeg1_.label() ) numGenTauDecays += genLeg2->size();
  if ( !(numGenTauDecays == 2) ) return;
  const reco::Candidate::LorentzVector& genLeg1P4 = genLeg1->at(0).p4();
  const reco::Candidate::LorentzVector& genLeg2P4 = ( srcGenLeg2_.label() != srcGenLeg1_.label() ) ?
    genLeg2->at(0).p4() : genLeg1->at(1).p4();

  typedef edm::View<reco::MET> MEtView;
  edm::Handle<MEtView> genMEt;
  evt.getByLabel(srcGenMEt_, genMEt);
  if ( !genMEt->size() == 1 ) 
    throw cms::Exception("NSVfitEventHypothesisAnalyzer::analyze") 
      << "Failed to find unique gen. MET object !!\n";
  const reco::Candidate::LorentzVector& genMEtP4 = genMEt->front().p4();

  edm::Handle<PFMEtSignCovMatrix> pfMEtSignCovMatrix;
  evt.getByLabel(srcPFMEtCovMatrix_, pfMEtSignCovMatrix);

  double evtWeight = 1.0;
  for ( vInputTag::const_iterator srcWeight = srcWeights_.begin();
	srcWeight != srcWeights_.end(); ++srcWeight ) {
    edm::Handle<double> weight;
    evt.getByLabel(*srcWeight, weight);
    evtWeight *= (*weight);
  }

  if ( evtWeight < 1.e-3 || evtWeight > 1.e+3 || TMath::IsNaN(evtWeight) ) return;

  for ( typename NSVfitEventHypothesisCollection::const_iterator svFitEventHypothesis = svFitEventHypotheses->begin();
	svFitEventHypothesis != svFitEventHypotheses->end(); ++svFitEventHypothesis ) {
    if ( !((int)svFitEventHypothesis->numResonances() > idxResonance_) )
      throw cms::Exception("NSVfitEventHypothesisAnalyzer") 
	<< " Failed to find resonance #" << idxResonance_ << " in NSVfitEventHypothesis !!\n";
    const NSVfitResonanceHypothesisBase* svFitResonanceHypothesis = svFitEventHypothesis->resonance(idxResonance_);

    bool svFitIsValidSolution = svFitResonanceHypothesis->isValidSolution();
    svFitIsValidSolution_->Fill(svFitIsValidSolution, evtWeight);

    double svFitMass      = svFitResonanceHypothesis->mass();
    double svFitSigmaUp   = svFitResonanceHypothesis->massErrUp();
    double svFitSigmaDown = svFitResonanceHypothesis->massErrDown();
    double svFitSigma     = TMath::Sqrt(square(svFitSigmaUp) + square(svFitSigmaDown));

    svFitMassVsSigmaXL_->Fill(svFitSigma, svFitMass, evtWeight);
    
    assert(svFitResonanceHypothesis->numDaughters() == 2);
    const NSVfitSingleParticleHypothesis* svFitDaughter1 = dynamic_cast<const NSVfitSingleParticleHypothesis*>(
      svFitResonanceHypothesis->daughter(0));
    const reco::Candidate::LorentzVector& svFitDaughter1P4 = svFitDaughter1->p4();
    const NSVfitSingleParticleHypothesis* svFitDaughter2 = dynamic_cast<const NSVfitSingleParticleHypothesis*>(
      svFitResonanceHypothesis->daughter(1));
    const reco::Candidate::LorentzVector& svFitDaughter2P4 = svFitDaughter2->p4();

    double dRcombination1 = square(deltaR(svFitDaughter1P4, genLeg1P4)) + square(deltaR(svFitDaughter2P4, genLeg2P4));
    double dRcombination2 = square(deltaR(svFitDaughter1P4, genLeg2P4)) + square(deltaR(svFitDaughter2P4, genLeg1P4));
    reco::Candidate::LorentzVector genLeg1P4_matched, genLeg2P4_matched;
    if ( dRcombination1 < dRcombination2 ) {
      genLeg1P4_matched = genLeg1P4;
      genLeg2P4_matched = genLeg2P4;
    } else {
      genLeg1P4_matched = genLeg2P4;
      genLeg2P4_matched = genLeg1P4;
    }

    if ( dynamic_cast<const pat::Tau*>(svFitDaughter1->particle().get()) == 0 &&
	 dynamic_cast<const pat::Tau*>(svFitDaughter2->particle().get()) != 0 ) { // tau-pair decay to lepton + tau-jet
      const pat::Tau* tau = dynamic_cast<const pat::Tau*>(svFitDaughter2->particle().get());
      int tauDecayMode = tau->decayMode();
      if ( tauDecayMode == reco::PFTau::kOneProng0PiZero ) 
	svFitMassVsSigmaXL_oneProng0pi0_->Fill(svFitSigma, svFitMass, evtWeight);
      else if ( tauDecayMode == reco::PFTau::kOneProng1PiZero )
	svFitMassVsSigmaXL_oneProng1pi0_->Fill(svFitSigma, svFitMass, evtWeight);
      else if ( tauDecayMode == reco::PFTau::kOneProng2PiZero ) 
	svFitMassVsSigmaXL_oneProng2pi0_->Fill(svFitSigma, svFitMass, evtWeight);
      else if ( tauDecayMode == reco::PFTau::kThreeProng0PiZero )
	svFitMassVsSigmaXL_threeProng0pi0_->Fill(svFitSigma, svFitMass, evtWeight);
    }

    double diTauPt, prodAngle_rf;
    if ( dynamic_cast<const NSVfitResonanceHypothesis*>(svFitResonanceHypothesis) ) {
      diTauPt = (dynamic_cast<const NSVfitResonanceHypothesis*>(svFitResonanceHypothesis))->p4_fitted().pt();
      prodAngle_rf = (dynamic_cast<const NSVfitResonanceHypothesis*>(svFitResonanceHypothesis))->prod_angle_rf();
    } else {
      diTauPt = (svFitDaughter1P4 + svFitDaughter2P4 + svFitEventHypothesis->met()->p4()).pt();
      prodAngle_rf = 0.;
    }

    double svFitMass_mean        = 0.;
    double svFitMass_median      = 0.;
    double svFitMass_maximum     = 0.;
    double svFitMass_maxInterpol = 0.;
    if ( dynamic_cast<const NSVfitResonanceHypothesisByIntegration*>(svFitResonanceHypothesis) ) {
      svFitMass_mean        = (dynamic_cast<const NSVfitResonanceHypothesisByIntegration*>(svFitResonanceHypothesis))->mass_mean();
      svFitMass_median      = (dynamic_cast<const NSVfitResonanceHypothesisByIntegration*>(svFitResonanceHypothesis))->mass_median();
      svFitMass_maximum     = (dynamic_cast<const NSVfitResonanceHypothesisByIntegration*>(svFitResonanceHypothesis))->mass_maximum();
      svFitMass_maxInterpol = (dynamic_cast<const NSVfitResonanceHypothesisByIntegration*>(svFitResonanceHypothesis))->mass_maxInterpol();
    }

    reco::Candidate::LorentzVector recMEtP4 = svFitEventHypothesis->met()->p4();
    TVectorD covEigenValues(2);
    pfMEtSignCovMatrix->EigenVectors(covEigenValues);
    double metCov = TMath::Power(square(covEigenValues(0)) + square(covEigenValues(1)), 0.25);
    reco::Candidate::LorentzVector rec_minus_genMEtP4 = recMEtP4 - genMEtP4;
    double metPull = rec_minus_genMEtP4.pt()/compProjCovUncertaintyXY(*pfMEtSignCovMatrix, recMEtP4 - genMEtP4);

    for ( typename std::vector<plotEntryType1*>::iterator plotEntry = plotEntries1_.begin();
	  plotEntry != plotEntries1_.end(); ++plotEntry ) {
      (*plotEntry)->fillHistograms(
        svFitIsValidSolution,			     
	svFitDaughter1P4, svFitDaughter2P4, 
	svFitMass,
	svFitMass_mean, svFitMass_median, svFitMass_maximum, svFitMass_maxInterpol, 
	svFitSigma, 
	recMEtP4.pt(),
	evtWeight);
    }
    
    for ( typename std::vector<plotEntryType2*>::iterator plotEntry = plotEntries2_.begin();
	  plotEntry != plotEntries2_.end(); ++plotEntry ) {
      (*plotEntry)->fillHistograms(
        svFitIsValidSolution,			     
	genLeg1P4_matched, svFitDaughter1P4, genLeg2P4_matched, svFitDaughter2P4, svFitMass, svFitSigma, svFitSigmaUp, svFitSigmaDown, 
	diTauPt, prodAngle_rf,
	recMEtP4, genMEtP4, metCov, metPull, 
	evtWeight);
    }
  }

  ++numEvents_processed_;
  numEventsWeighted_processed_ += evtWeight;
}

template <typename T>
void NSVfitEventHypothesisAnalyzerT<T>::endJob()
{
  //std::cout << "<NSVfitEventHypothesisAnalyzer::endJob>:" << std::endl;
  //std::cout << " " << moduleLabel_ << ":"
  //	      << " num. Events processed = " << numEvents_processed_ << "," 
  //	      << " weighted = " << numEventsWeighted_processed_ << std::endl;

  for ( typename std::vector<plotEntryType1*>::iterator plotEntry = plotEntries1_.begin();
	plotEntry != plotEntries1_.end(); ++plotEntry ) {
    (*plotEntry)->finalizeHistograms();
  }

  for ( typename std::vector<plotEntryType2*>::iterator plotEntry = plotEntries2_.begin();
	plotEntry != plotEntries2_.end(); ++plotEntry ) {
    (*plotEntry)->finalizeHistograms();
  }
}

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesis.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesisByIntegration.h"

typedef NSVfitEventHypothesisAnalyzerT<NSVfitEventHypothesis> NSVfitEventHypothesisAnalyzer;
typedef NSVfitEventHypothesisAnalyzerT<NSVfitEventHypothesisByIntegration> NSVfitEventHypothesisByIntegrationAnalyzer;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(NSVfitEventHypothesisAnalyzer);
DEFINE_FWK_MODULE(NSVfitEventHypothesisByIntegrationAnalyzer);





