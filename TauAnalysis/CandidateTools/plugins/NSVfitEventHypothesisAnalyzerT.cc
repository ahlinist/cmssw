#include "TauAnalysis/CandidateTools/plugins/NSVfitEventHypothesisAnalyzerT.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/Math/interface/deltaR.h"

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

  srcGenTauPairs_ = cfg.getParameter<edm::InputTag>("srcGenTauPairs");
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
  for ( typename std::vector<plotEntryType*>::iterator it = plotEntries_.begin();
	it != plotEntries_.end(); ++it ) {
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

  plotEntries_.push_back(new plotEntryType(
    dqmDirectory_,  -1.,  -1.,  0,  -1.,   -1.,  numBinsSVfitMass_, svFitMassMax_, numBinsSVfitSigma_, svFitSigmaMax_));
  plotEntries_.push_back(new plotEntryType(
    dqmDirectory_,  -1.,  -1., +1,  -1.,   -1.,  numBinsSVfitMass_, svFitMassMax_, numBinsSVfitSigma_, svFitSigmaMax_));
  plotEntries_.push_back(new plotEntryType(
    dqmDirectory_,  -1.,  -1., -1,  -1.,   -1.,  numBinsSVfitMass_, svFitMassMax_, numBinsSVfitSigma_, svFitSigmaMax_));

  plotEntries_.push_back(new plotEntryType(
    dqmDirectory_,   0.,  30.,  0,  -1.,   -1.,  numBinsSVfitMass_, svFitMassMax_, numBinsSVfitSigma_, svFitSigmaMax_)); 
  plotEntries_.push_back(new plotEntryType(
    dqmDirectory_,  30.,  60.,  0,  -1.,   -1.,  numBinsSVfitMass_, svFitMassMax_, numBinsSVfitSigma_, svFitSigmaMax_)); 
  plotEntries_.push_back(new plotEntryType(
    dqmDirectory_,  60.,  90.,  0,  -1.,   -1.,  numBinsSVfitMass_, svFitMassMax_, numBinsSVfitSigma_, svFitSigmaMax_)); 
  plotEntries_.push_back(new plotEntryType(
    dqmDirectory_,  90., 120.,  0,  -1.,   -1.,  numBinsSVfitMass_, svFitMassMax_, numBinsSVfitSigma_, svFitSigmaMax_)); 
  plotEntries_.push_back(new plotEntryType(
    dqmDirectory_, 120., 140.,  0,  -1.,   -1.,  numBinsSVfitMass_, svFitMassMax_, numBinsSVfitSigma_, svFitSigmaMax_)); 
  plotEntries_.push_back(new plotEntryType(
    dqmDirectory_, 140., 160.,  0,  -1.,   -1.,  numBinsSVfitMass_, svFitMassMax_, numBinsSVfitSigma_, svFitSigmaMax_)); 
  plotEntries_.push_back(new plotEntryType(
    dqmDirectory_, 160., 170.,  0,  -1.,   -1.,  numBinsSVfitMass_, svFitMassMax_, numBinsSVfitSigma_, svFitSigmaMax_)); 
  plotEntries_.push_back(new plotEntryType(
    dqmDirectory_, 170., 175.,  0,  -1.,   -1.,  numBinsSVfitMass_, svFitMassMax_, numBinsSVfitSigma_, svFitSigmaMax_)); 
  plotEntries_.push_back(new plotEntryType(
    dqmDirectory_, 175.,  -1.,  0,  -1.,   -1.,  numBinsSVfitMass_, svFitMassMax_, numBinsSVfitSigma_, svFitSigmaMax_)); 

  plotEntries_.push_back(new plotEntryType(
    dqmDirectory_,  -1.,  -1.,  0,  -1.,  50.,  numBinsSVfitMass_, svFitMassMax_, numBinsSVfitSigma_, svFitSigmaMax_));
  plotEntries_.push_back(new plotEntryType(
    dqmDirectory_,  -1.,  -1.,  0,  50., 100.,  numBinsSVfitMass_, svFitMassMax_, numBinsSVfitSigma_, svFitSigmaMax_));
  plotEntries_.push_back(new plotEntryType(
    dqmDirectory_,  -1.,  -1.,  0, 100.,  -1.,  numBinsSVfitMass_, svFitMassMax_, numBinsSVfitSigma_, svFitSigmaMax_));
  
  for ( typename std::vector<plotEntryType*>::iterator plotEntry = plotEntries_.begin();
	plotEntry != plotEntries_.end(); ++plotEntry ) {
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
  std::cout << "<NSVfitEventHypothesisAnalyzer::analyze>:" << std::endl;
  std::cout << " " << moduleLabel_ << ": src = " << srcEventHypotheses_.label() << std::endl;

  edm::Handle<NSVfitEventHypothesisCollection> svFitEventHypotheses;
  evt.getByLabel(srcEventHypotheses_, svFitEventHypotheses);

  typedef edm::View<reco::Candidate> CandidateView;
  edm::Handle<CandidateView> genTauPairs;
  evt.getByLabel(srcGenTauPairs_, genTauPairs);
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
  //std::cout << "MEtCov:" << std::endl;
  //pfMEtSignCovMatrix->Print();

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
    //std::cout << "SVfit: mass = " << svFitResonanceHypothesis->mass() << std::endl;

    assert(svFitResonanceHypothesis->numDaughters() == 2);
    const NSVfitSingleParticleHypothesis* svFitDaughter1 = dynamic_cast<const NSVfitSingleParticleHypothesis*>(
      svFitResonanceHypothesis->daughter(0));
    const reco::Candidate::LorentzVector& svFitDaughter1P4 = svFitDaughter1->p4();
    int daughter1DecayMode = reco::PFTau::kNull;
    if ( dynamic_cast<const pat::Tau*>(svFitDaughter1->particle().get()) != 0 )
      daughter1DecayMode = (dynamic_cast<const pat::Tau*>(svFitDaughter1->particle().get()))->decayMode();
    const NSVfitSingleParticleHypothesis* svFitDaughter2 = dynamic_cast<const NSVfitSingleParticleHypothesis*>(
      svFitResonanceHypothesis->daughter(1));
    const reco::Candidate::LorentzVector& svFitDaughter2P4 = svFitDaughter2->p4();
    int daughter2DecayMode = reco::PFTau::kNull;
    if ( dynamic_cast<const pat::Tau*>(svFitDaughter2->particle().get()) != 0 )
      daughter2DecayMode = (dynamic_cast<const pat::Tau*>(svFitDaughter2->particle().get()))->decayMode();

    reco::Candidate::LorentzVector recMEtP4 = svFitEventHypothesis->met()->p4();
    //std::cout << "MEt: Px = " << recMEtP4.px() << ", Py = " << recMEtP4.py() << std::endl;
    //std::cout << "cov(MEt):" << std::endl;
    //(dynamic_cast<const reco::MET*>(svFitEventHypothesis->met().get()))->getSignificanceMatrix().Print();

    for ( CandidateView::const_iterator genTauPair = genTauPairs->begin();
	  genTauPair != genTauPairs->end(); ++genTauPair ) {

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

      for ( typename std::vector<plotEntryType*>::iterator plotEntry = plotEntries_.begin();
	  plotEntry != plotEntries_.end(); ++plotEntry ) {
	(*plotEntry)->fillHistograms(
          genTauPair->p4(), 
	  genLeg1P4_matched, genLeg2P4_matched, 
	  genMEtP4,
	  svFitResonanceHypothesis, svFitEventHypothesis->nll(), 
	  svFitDaughter1P4, daughter1DecayMode, svFitDaughter2P4, daughter2DecayMode, 
	  recMEtP4,	  
	  evtWeight);
      }
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

  for ( typename std::vector<plotEntryType*>::iterator plotEntry = plotEntries_.begin();
	plotEntry != plotEntries_.end(); ++plotEntry ) {
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





