#include "TauAnalysis/Core/plugins/CompositePtrCandidateT1T2MEtNSVfitHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DQMServices/Core/interface/DQMStore.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/Candidate/interface/Candidate.h" 

#include "PhysicsTools/JetMCUtils/interface/JetMCTag.h"

#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"
#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEtFwd.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesisBase.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesisBaseFwd.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesisByIntegration.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesisBase.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesisByIntegration.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleParticleHypothesisBase.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "TauAnalysis/Core/interface/histManagerAuxFunctions.h"
#include "TauAnalysis/CandidateTools/interface/candidateAuxFunctions.h"
#include "TauAnalysis/CandidateTools/interface/generalAuxFunctions.h"
#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"

#include <TMath.h>
#include <TH1.h>

const double epsilon = 0.01;

template<typename T1, typename T2>
bool matchesGenCandidatePair(const CompositePtrCandidateT1T2MEt<T1,T2>& compositePtrCandidate)
{
  bool isGenMatched = false;
// not implemented yet...
  return isGenMatched;
}

template<typename T1, typename T2>
CompositePtrCandidateT1T2MEtNSVfitHistManager<T1,T2>::CompositePtrCandidateT1T2MEtNSVfitHistManager(const edm::ParameterSet& cfg)
  : HistManagerBase(cfg)
{
  //std::cout << "<CompositePtrCandidateT1T2MEtNSVfitHistManager::CompositePtrCandidateT1T2MEtNSVfitHistManager>:" << std::endl;

  diTauCandidateSrc_ = cfg.getParameter<edm::InputTag>("diTauCandidateSource");
  //std::cout << " diTauCandidateSrc = " << diTauCandidateSrc_.label() << std::endl;

  genParticleSrc_ = cfg.getParameter<edm::InputTag>("genParticleSource");
  //std::cout << " genParticleSrc = " << genParticleSrc_.label() << std::endl;

  edm::ParameterSet cfg_nSVfitEventHypotheses = cfg.getParameter<edm::ParameterSet>("nSVfitEventHypotheses");
  typedef std::vector<std::string> vstring;

//--- get NSVfitEventHypothesis objects via InputTag,
//    directly from the edm::Event
  vstring nSVfitEventHypothesisNames_src = cfg_nSVfitEventHypotheses.getParameterNamesForType<edm::InputTag>();
  for ( vstring::const_iterator nSVfitEventHypothesisName = nSVfitEventHypothesisNames_src.begin();
	nSVfitEventHypothesisName != nSVfitEventHypothesisNames_src.end(); ++nSVfitEventHypothesisName ) {
    massHypothesisEntryType* massHypothesisEntry = new massHypothesisEntryType();
    massHypothesisEntry->nSVfitEventHypothesisSrc_ = cfg_nSVfitEventHypotheses.getParameter<edm::InputTag>(*nSVfitEventHypothesisName);
    massHypothesisEntry->dqmDirectory_ = dqmDirectoryName(dqmDirectory_store_).append(*nSVfitEventHypothesisName);
    massHypothesisEntry->dqmDirectory_ = dqmDirectoryName(massHypothesisEntry->dqmDirectory_);
    massHypothesisEntries_.push_back(massHypothesisEntry);
  }

//--- get NSVfitEventHypotheses from diTau objects,
//    via CompositeRefCandidateT1T2MEt::nSVfitSolution(name) calls
  vstring nSVfitEventHypothesisNames_name = cfg_nSVfitEventHypotheses.getParameterNamesForType<std::string>();
  for ( vstring::const_iterator nSVfitEventHypothesisName = nSVfitEventHypothesisNames_name.begin();
	nSVfitEventHypothesisName != nSVfitEventHypothesisNames_name.end(); ++nSVfitEventHypothesisName ) {
    massHypothesisEntryType* massHypothesisEntry = new massHypothesisEntryType();
    massHypothesisEntry->nSVfitEventHypothesisName_ = cfg_nSVfitEventHypotheses.getParameter<std::string>(*nSVfitEventHypothesisName);
    massHypothesisEntry->dqmDirectory_ = dqmDirectoryName(dqmDirectory_store_).append(*nSVfitEventHypothesisName);
    massHypothesisEntry->dqmDirectory_ = dqmDirectoryName(massHypothesisEntry->dqmDirectory_);
    massHypothesisEntries_.push_back(massHypothesisEntry);
  }

  diTauLeg1WeightExtractors_ = getTauJetWeightExtractors<T1>(cfg, "diTauLeg1WeightSource");
  diTauLeg2WeightExtractors_ = getTauJetWeightExtractors<T2>(cfg, "diTauLeg2WeightSource");

  requireGenMatch_ = cfg.getParameter<bool>("requireGenMatch");
  //std::cout << " requireGenMatch = " << requireGenMatch_ << std::endl;

  pdgIdsElectron_.push_back(+11);
  pdgIdsElectron_.push_back(-11);
  pdgIdsMuon_.push_back(+13);
  pdgIdsMuon_.push_back(-13);
  pdgIdsPhoton_.push_back(22);
  for ( int iQuarkType = 1; iQuarkType <= 6; ++iQuarkType ) {
    pdgIdsJet_.push_back(+iQuarkType);
    pdgIdsJet_.push_back(-iQuarkType);
  }
  pdgIdsJet_.push_back(21);

  std::string normalization_string = cfg.getParameter<std::string>("normalization");
  normMethod_ = getNormMethod(normalization_string, "diTauCandidates");
}

template<typename T1, typename T2>
CompositePtrCandidateT1T2MEtNSVfitHistManager<T1,T2>::~CompositePtrCandidateT1T2MEtNSVfitHistManager()
{
  for ( typename std::vector<massHypothesisEntryType*>::iterator it = massHypothesisEntries_.begin();
	it != massHypothesisEntries_.end(); ++it ) {
    delete (*it);
  }

  for ( typename std::vector<FakeRateJetWeightExtractor<T1>*>::iterator it = diTauLeg1WeightExtractors_.begin();
	it != diTauLeg1WeightExtractors_.end(); ++it ) {
    delete (*it);
  }

  for ( typename std::vector<FakeRateJetWeightExtractor<T2>*>::iterator it = diTauLeg2WeightExtractors_.begin();
	it != diTauLeg2WeightExtractors_.end(); ++it ) {
    delete (*it);
  }
}

template<typename T1, typename T2>
void CompositePtrCandidateT1T2MEtNSVfitHistManager<T1,T2>::bookHistogramsImp()
{
  //std::cout << "<CompositePtrCandidateT1T2MEtNSVfitHistManager::bookHistogramsImp>:" << std::endl;

  for ( typename std::vector<massHypothesisEntryType*>::iterator massHypothesisEntry = massHypothesisEntries_.begin();
	massHypothesisEntry != massHypothesisEntries_.end(); ++massHypothesisEntry ) {
    const std::string& dqmDirectory = (*massHypothesisEntry)->dqmDirectory_;

    (*massHypothesisEntry)->hMass_   = book1D(*dqmStore_, dqmDirectory, "Mass",   "Mass",    50, 0., 250.);
    (*massHypothesisEntry)->hMassL_  = book1D(*dqmStore_, dqmDirectory, "MassL",  "MassL",  100, 0., 500.);
    (*massHypothesisEntry)->hMassXL_ = book1D(*dqmStore_, dqmDirectory, "MassXL", "MassXL", 150, 0., 750.);

    (*massHypothesisEntry)->hMassGenLeg2Electron_ =
      book1D(*dqmStore_, dqmDirectory, "MassGenLeg2Electron", "MassGenLeg2Electron", 50, 0., 250.);
    (*massHypothesisEntry)->hMassGenLeg2Muon_ =
      book1D(*dqmStore_, dqmDirectory, "MassGenLeg2Muon",     "MassGenLeg2Muon",     50, 0., 250.);
    (*massHypothesisEntry)->hMassGenLeg2Photon_ =
      book1D(*dqmStore_, dqmDirectory, "MassGenLeg2Photon",   "MassGenLeg2Photon",   50, 0., 250.);
    (*massHypothesisEntry)->hMassGenLeg2Jet_ =
      book1D(*dqmStore_, dqmDirectory, "MassGenLeg2Jet",      "MassGenLeg2Jet",      50, 0., 250.);

    (*massHypothesisEntry)->hMassMedian_      = book1D(*dqmStore_, dqmDirectory, "Mass_median",      "Mass_median",      100,  0., 500.);
    (*massHypothesisEntry)->hMassMean_        = book1D(*dqmStore_, dqmDirectory, "Mass_mean",        "Mass_mean",        100,  0., 500.);
    (*massHypothesisEntry)->hMassMaximum_     = book1D(*dqmStore_, dqmDirectory, "Mass_maximum",     "Mass_maximum",     100,  0., 500.);
    (*massHypothesisEntry)->hMassMaxInterpol_ = book1D(*dqmStore_, dqmDirectory, "Mass_maxInterpol", "Mass_maxInterpol", 100,  0., 500.);

    (*massHypothesisEntry)->hMassPull_ = book1D(*dqmStore_, dqmDirectory, "MassPull", "MassPull", 100, -5., +5.);

    (*massHypothesisEntry)->hMassSum_           = 0; // book histogram when filling first value
    (*massHypothesisEntry)->hMassSumNormalized_ = 0;

    (*massHypothesisEntry)->hMassHadRecoilPtLt10_   =
      book1D(*dqmStore_, dqmDirectory, "MassHadRecoilPtLt10",    "MassHadRecoilPtLt10",   100, 0., 500.);
    (*massHypothesisEntry)->hMassHadRecoilPt10to20_ =
      book1D(*dqmStore_, dqmDirectory, "MassHadRecoilPt10to20",  "MassHadRecoilPt10to20", 100, 0., 500.);
    (*massHypothesisEntry)->hMassHadRecoilPt20to30_ =
      book1D(*dqmStore_, dqmDirectory, "MassHadRecoilPt20to30",  "MassHadRecoilPt20to30", 100, 0., 500.);
    (*massHypothesisEntry)->hMassHadRecoilPtGt30_   =
      book1D(*dqmStore_, dqmDirectory, "MassHadRecoilPtGt30",    "MassHadRecoilPtGt30",   100, 0., 500.);
    (*massHypothesisEntry)->hHadRecoilPt_ = book1D(*dqmStore_, dqmDirectory, "HadRecoilPt", "HadRecoilPt", 20, 0., 100.);

    (*massHypothesisEntry)->hMassDPhiGt175_ =
      book1D(*dqmStore_, dqmDirectory, "MassDPhiGt175", "MassDPhiGt175",       100, 0., 500.);
    (*massHypothesisEntry)->hMassDPhi170to175_ =
      book1D(*dqmStore_, dqmDirectory, "MassDPhi170to175", "MassDPhi170to175", 100, 0., 500.);
    (*massHypothesisEntry)->hMassDPhi160to170_ =
      book1D(*dqmStore_, dqmDirectory, "MassDPhi160to170", "MassDPhi160to170", 100, 0., 500.);
    (*massHypothesisEntry)->hMassDPhi140to160_ =
      book1D(*dqmStore_, dqmDirectory, "MassDPhi140to160", "MassDPhi140to160", 100, 0., 500.);
    (*massHypothesisEntry)->hMassDPhi120to140_ =
      book1D(*dqmStore_, dqmDirectory, "MassDPhi120to140", "MassDPhi120to140", 100, 0., 500.);
    (*massHypothesisEntry)->hMassDPhiLt120_ =
      book1D(*dqmStore_, dqmDirectory, "MassDPhiLt120", "MassDPhiLt120",       100, 0., 500.);
    (*massHypothesisEntry)->hDPhi_ = book1D(*dqmStore_, dqmDirectory, "DPhi", "DPhi", 36, -epsilon, TMath::Pi() + epsilon);
  }

  bookWeightHistograms(*dqmStore_, "DiTauCandidateWeight", "Composite Weight",
		       hDiTauCandidateWeightPosLog_, hDiTauCandidateWeightNegLog_, hDiTauCandidateWeightZero_,
		       hDiTauCandidateWeightLinear_);
}

template<typename T1, typename T2>
double CompositePtrCandidateT1T2MEtNSVfitHistManager<T1,T2>::getDiTauCandidateWeight(const CompositePtrCandidateT1T2MEt<T1,T2>& diTauCandidate)
{
  double diTauLeg1Weight = getTauJetWeight<T1>(*diTauCandidate.leg1(), diTauLeg1WeightExtractors_);
  double diTauLeg2Weight = getTauJetWeight<T2>(*diTauCandidate.leg2(), diTauLeg2WeightExtractors_);
  return (diTauLeg1Weight*diTauLeg2Weight);
}

//
//-------------------------------------------------------------------------------
//

template <typename T>
std::string genDecayMode(const T& lepton)
{
  return "undefined";
}

template <>
std::string genDecayMode<pat::Electron>(const pat::Electron& electron)
{
  const reco::GenParticle* genLepton = electron.genLepton();
  if      ( !genLepton                ) return "undefined";
  else if (  genLepton->pdgId() == 11 ) return "electron";
  else if (  genLepton->pdgId() == 13 ) return "muon";
  else if (  genLepton->pdgId() == 15 ) return getGenTauDecayMode(genLepton);
  else                                  return "undefined";
}

template <>
std::string genDecayMode<pat::Muon>(const pat::Muon& muon)
{
  const reco::GenParticle* genLepton = muon.genLepton();
  if      ( !genLepton                ) return "undefined";
  else if (  genLepton->pdgId() == 11 ) return "electron";
  else if (  genLepton->pdgId() == 13 ) return "muon";
  else if (  genLepton->pdgId() == 15 ) return getGenTauDecayMode(genLepton);
  else                                  return "undefined";
}

template <>
std::string genDecayMode<pat::Tau>(const pat::Tau& tau)
{
  const reco::GenJet* genJet = tau.genJet();
  if   ( !genJet ) return "undefined";
  else             return JetMCTagUtils::genTauDecayMode(*genJet);
}

//
//-------------------------------------------------------------------------------
//

template <typename T>
std::string recDecayMode(const T&)
{
  return "undefined";
}

template <>
std::string recDecayMode<pat::Electron>(const pat::Electron&)
{
  return "electron";
}

template <>
std::string recDecayMode<pat::Muon>(const pat::Muon&)
{
  return "muon";
}

template <>
std::string recDecayMode<pat::Tau>(const pat::Tau& tau)
{
  return getTauDecayModeName(tau.decayMode());
}

//
//-------------------------------------------------------------------------------
//

template<typename T1, typename T2>
void CompositePtrCandidateT1T2MEtNSVfitHistManager<T1,T2>::fillHistogramsImp(const edm::Event& evt, const edm::EventSetup& es, double evtWeight)
{
  //std::cout << "<CompositePtrCandidateT1T2MEtNSVfitHistManager::fillHistogramsImp>:" << std::endl;

  typedef std::vector<CompositePtrCandidateT1T2MEt<T1,T2> > CompositePtrCandidateCollection;
  edm::Handle<CompositePtrCandidateCollection> diTauCandidates;
  getCollection(evt, diTauCandidateSrc_, diTauCandidates);

  //std::cout << " diTauCandidateSrc = " << diTauCandidateSrc_.label() << ":"
  //	      << " diTauCandidates.size = " << diTauCandidates->size() << std::endl;

  edm::Handle<reco::GenParticleCollection> genParticles;
  if ( genParticleSrc_.label() != "" ) evt.getByLabel(genParticleSrc_, genParticles);

  double diTauCandidateWeightSum = 0.;
  for ( typename CompositePtrCandidateCollection::const_iterator diTauCandidate = diTauCandidates->begin();
	diTauCandidate != diTauCandidates->end(); ++diTauCandidate ) {
    if ( requireGenMatch_ && !matchesGenCandidatePair(*diTauCandidate) ) continue;

    diTauCandidateWeightSum += getDiTauCandidateWeight(*diTauCandidate);
  }

  for ( typename CompositePtrCandidateCollection::const_iterator diTauCandidate = diTauCandidates->begin();
	diTauCandidate != diTauCandidates->end(); ++diTauCandidate ) {

    //bool isGenMatched = matchesGenCandidatePair(*diTauCandidate->second);
    //std::cout << " Pt = " << diTauCandidate->second->pt() << ", phi = " << diTauCandidate->second->phi() << ","
    //          << " visMass = " << diTauCandidate->second->p4Vis().mass() << std::endl;
    //std::cout << " isGenMatched = " << isGenMatched << std::endl;

    if ( requireGenMatch_ && !matchesGenCandidatePair(*diTauCandidate) ) continue;

    double diTauCandidateWeight = getDiTauCandidateWeight(*diTauCandidate);
    double weight = getWeight(evtWeight, diTauCandidateWeight, diTauCandidateWeightSum);

    //std::cout << "diTauCandidate leg1: Pt = " << diTauCandidate->leg1()->pt() << ","
    //	        << " eta = " << diTauCandidate->leg1()->eta() << ", phi = " << diTauCandidate->leg1()->phi() << std::endl;
    //std::cout << "diTauCandidate leg2: Pt = " << diTauCandidate->leg2()->pt() << ","
    //	        << " eta = " << diTauCandidate->leg2()->eta() << ", phi = " << diTauCandidate->leg2()->phi() << std::endl;

    for ( typename std::vector<massHypothesisEntryType*>::iterator massHypothesisEntry = massHypothesisEntries_.begin();
	  massHypothesisEntry != massHypothesisEntries_.end(); ++massHypothesisEntry ) {

      const NSVfitEventHypothesisBase* matchedEventHypothesis = 0;
      if ( (*massHypothesisEntry)->nSVfitEventHypothesisName_ != "" ) {
	matchedEventHypothesis = diTauCandidate->nSVfitSolution((*massHypothesisEntry)->nSVfitEventHypothesisName_);
      } else if ( (*massHypothesisEntry)->nSVfitEventHypothesisSrc_.label() != "" ) {
	edm::Handle<NSVfitEventHypothesisBaseCollection> nSVfitEventHypotheses;
	evt.getByLabel((*massHypothesisEntry)->nSVfitEventHypothesisSrc_, nSVfitEventHypotheses);
	
	for ( NSVfitEventHypothesisBaseCollection::const_iterator nSVfitEventHypothesis = nSVfitEventHypotheses->begin();
	      nSVfitEventHypothesis != nSVfitEventHypotheses->end(); ++nSVfitEventHypothesis ) {
	  
	  size_t numResonances = nSVfitEventHypothesis->numResonances();   
	  assert(numResonances == 1);
	  const NSVfitResonanceHypothesisBase* resonance = nSVfitEventHypothesis->resonance(0);
	  
	  if ( !resonance->isValidSolution() ) continue;
	  
	  size_t numDaughters = resonance->numDaughters();   
	  assert(numDaughters == 2);
	  const NSVfitSingleParticleHypothesisBase* daughter1 = resonance->daughter(0);
	  const NSVfitSingleParticleHypothesisBase* daughter2 = resonance->daughter(1);
	  
	  const reco::Candidate::LorentzVector& p4Daughter1 = daughter1->particle()->p4();
          const reco::Candidate::LorentzVector& p4Daughter2 = daughter2->particle()->p4();

	  //std::cout << "nSVfit leg1: Pt = " << p4Daughter1.pt() << "," 
	  //	      << " eta = " << p4Daughter1.eta() << ", phi = " << p4Daughter1.phi() << std::endl;
	  //std::cout << "nSVfit leg1: Pt = " << p4Daughter2.pt() << "," 
	  //	      << " eta = " << p4Daughter2.eta() << ", phi = " << p4Daughter2.phi() << std::endl;
	  
	  if ( (deltaR(diTauCandidate->leg1()->p4(), p4Daughter1) < epsilon &&
		deltaR(diTauCandidate->leg2()->p4(), p4Daughter2) < epsilon) ||
	       (deltaR(diTauCandidate->leg1()->p4(), p4Daughter2) < epsilon &&
		deltaR(diTauCandidate->leg2()->p4(), p4Daughter1) < epsilon) ) {
	    matchedEventHypothesis = &(*nSVfitEventHypothesis);
	    break;
	  }
	}
      }

      if ( !matchedEventHypothesis ) {
	edm::LogWarning("CompositePtrCandidateT1T2MEtNSVfitHistManager::fillHistogramsImp")
	  << " Failed to match diTauCandidate to NSVfitEventHypothesis object --> skipping !!";
	continue;
      }
      
      const NSVfitResonanceHypothesisBase* nSVfitResonanceHypothesis = matchedEventHypothesis->resonance(0);
      double recMass = nSVfitResonanceHypothesis->mass();
      double genMass = diTauCandidate->p4gen().mass();

      (*massHypothesisEntry)->hMass_->Fill(recMass, weight);
      (*massHypothesisEntry)->hMassL_->Fill(recMass, weight);
      (*massHypothesisEntry)->hMassXL_->Fill(recMass, weight);

      if ( genParticles.isValid() ) {
	fillHistogramGenMatch((*massHypothesisEntry)->hMassGenLeg2Electron_,
			      recMass, diTauCandidate->leg2()->p4(), *genParticles, pdgIdsElectron_, weight);
	fillHistogramGenMatch((*massHypothesisEntry)->hMassGenLeg2Muon_,
			      recMass, diTauCandidate->leg2()->p4(), *genParticles, pdgIdsMuon_,     weight);
	fillHistogramGenMatch((*massHypothesisEntry)->hMassGenLeg2Photon_,
			      recMass, diTauCandidate->leg2()->p4(), *genParticles, pdgIdsPhoton_,   weight);
	fillHistogramGenMatch((*massHypothesisEntry)->hMassGenLeg2Jet_,
			      recMass, diTauCandidate->leg2()->p4(), *genParticles, pdgIdsJet_,      weight);
      }

      if ( dynamic_cast<const NSVfitResonanceHypothesisByIntegration*>(nSVfitResonanceHypothesis) ) {
	const NSVfitResonanceHypothesisByIntegration* nSVfitResonanceHypothesis_byIntegration = 
	  dynamic_cast<const NSVfitResonanceHypothesisByIntegration*>(nSVfitResonanceHypothesis);
	(*massHypothesisEntry)->hMassMedian_->Fill(nSVfitResonanceHypothesis_byIntegration->mass_median(), weight);
	(*massHypothesisEntry)->hMassMean_->Fill(nSVfitResonanceHypothesis_byIntegration->mass_mean(), weight);
	(*massHypothesisEntry)->hMassMaximum_->Fill(nSVfitResonanceHypothesis_byIntegration->mass_maximum(), weight);
	(*massHypothesisEntry)->hMassMaxInterpol_->Fill(nSVfitResonanceHypothesis_byIntegration->mass_maxInterpol(), weight);
      }

      double sigma = ( recMass > genMass ) ? 
	nSVfitResonanceHypothesis->massErrUp() : nSVfitResonanceHypothesis->massErrDown();
      if ( sigma != 0. ) (*massHypothesisEntry)->hMassPull_->Fill((recMass - genMass)/sigma, weight);
      
      if ( dynamic_cast<const NSVfitEventHypothesisByIntegration*>(matchedEventHypothesis) ) {
	const NSVfitEventHypothesisByIntegration* matchedEventHypothesis_byIntegration = 
	  dynamic_cast<const NSVfitEventHypothesisByIntegration*>(matchedEventHypothesis);
	if ( matchedEventHypothesis_byIntegration->histMassResults() ) {
	  const TH1* histMassResults = matchedEventHypothesis_byIntegration->histMassResults();
	  double integral = histMassResults->Integral();
	  if ( integral > 0. ) 
	    fillHistogramMassResult((*massHypothesisEntry)->hMassSumNormalized_, (*massHypothesisEntry)->dqmDirectory_,
				    "MassSumNormalized", histMassResults, weight/integral);
	  fillHistogramMassResult((*massHypothesisEntry)->hMassSum_, (*massHypothesisEntry)->dqmDirectory_,
				  "MassSum", histMassResults, weight);
	}
      }

      double hadRecoilPt = (diTauCandidate->p4Vis() + diTauCandidate->met()->p4()).pt();
      if      ( hadRecoilPt < 10. ) (*massHypothesisEntry)->hMassHadRecoilPtLt10_->Fill(recMass, weight);
      else if ( hadRecoilPt < 20. ) (*massHypothesisEntry)->hMassHadRecoilPt10to20_->Fill(recMass, weight);
      else if ( hadRecoilPt < 30. ) (*massHypothesisEntry)->hMassHadRecoilPt20to30_->Fill(recMass, weight);
      else                          (*massHypothesisEntry)->hMassHadRecoilPtGt30_->Fill(recMass, weight);
      (*massHypothesisEntry)->hHadRecoilPt_->Fill(hadRecoilPt, weight);

      double dPhi = diTauCandidate->dPhi12()*180./TMath::Pi();
      if      ( dPhi > 175. ) (*massHypothesisEntry)->hMassDPhiGt175_->Fill(recMass, weight);
      else if ( dPhi > 170. ) (*massHypothesisEntry)->hMassDPhi170to175_->Fill(recMass, weight);
      else if ( dPhi > 160. ) (*massHypothesisEntry)->hMassDPhi160to170_->Fill(recMass, weight);
      else if ( dPhi > 140. ) (*massHypothesisEntry)->hMassDPhi140to160_->Fill(recMass, weight);
      else if ( dPhi > 120. ) (*massHypothesisEntry)->hMassDPhi120to140_->Fill(recMass, weight);
      else                    (*massHypothesisEntry)->hMassDPhiLt120_->Fill(recMass, weight);
      (*massHypothesisEntry)->hDPhi_->Fill(diTauCandidate->dPhi12(), weight);

      std::string leg1genDecayMode = genDecayMode(*diTauCandidate->leg1());
      std::string leg1recDecayMode = recDecayMode(*diTauCandidate->leg1());
      std::string leg2genDecayMode = genDecayMode(*diTauCandidate->leg2());
      std::string leg2recDecayMode = recDecayMode(*diTauCandidate->leg2());
      MonitorElement* me =
	(*massHypothesisEntry)->hMassGenVsRecDecayModes_[leg1genDecayMode][leg1recDecayMode][leg2genDecayMode][leg2recDecayMode];
      if ( !me ) {
//--- book MonitorElement
	std::string meName = std::string("Mass");
	meName.append("_leg1#gen_").append(leg1genDecayMode).append("_rec_").append(leg1recDecayMode);
	meName.append("_leg2#gen_").append(leg2genDecayMode).append("_rec_").append(leg2recDecayMode);
	//std::cout << "--> booking meName = " << meName << ", dqmDirectory = " << (*massHypothesisEntry)->dqmDirectory_ << std::endl;
	me = book1D(*dqmStore_, (*massHypothesisEntry)->dqmDirectory_, meName.data(), meName.data(), 150, 0., 750.);
	(*massHypothesisEntry)->hMassGenVsRecDecayModes_[leg1genDecayMode][leg1recDecayMode][leg2genDecayMode][leg2recDecayMode] = me;
      }
      me->Fill(recMass, weight);
    }

    fillWeightHistograms(hDiTauCandidateWeightPosLog_, hDiTauCandidateWeightNegLog_, hDiTauCandidateWeightZero_,
			 hDiTauCandidateWeightLinear_, diTauCandidateWeight);
  }
}

template<typename T1, typename T2>
void CompositePtrCandidateT1T2MEtNSVfitHistManager<T1,T2>::fillHistogramMassResult(
       MonitorElement*& me, const std::string& dqmDirectory, const std::string& meName, const TH1* histMassResults, double weight)
{
  if ( !me ) {
//--- check that no other MonitorElement with name given as function argument exists
    if ( checkExistence(*dqmStore_, dqmDirectory, meName) ) return;

//--- book MonitorElement
    dqmStore_->setCurrentFolder(dqmDirectory);
    std::string meName_full = dqmDirectoryName(dqmDirectory).append(meName);
    //std::cout << "<CompositePtrCandidateT1T2MEtNSVfitHistManager::fillHistogramMassResult>:" << std::endl;
    //std::cout << "--> booking meName = " << meName_full << std::endl;
    TH1* histogram = (TH1*)histMassResults->Clone(meName.data());
    histogram->SetTitle(meName.data());
    histogram->Reset();
    dqmRegisterHistogram(*dqmStore_, histogram, meName_full);
    bool dqmError = false;
    me = getMonitorElement(*dqmStore_, meName_full, dqmError);
    assert(!dqmError);
  }

  if ( weight > 0. ) me->getTH1()->Add(histMassResults, weight);
}

#include "FWCore/Framework/interface/MakerMacros.h"

typedef CompositePtrCandidateT1T2MEtNSVfitHistManager<pat::Electron, pat::Tau> PATElecTauPairNSVfitHistManager;
typedef CompositePtrCandidateT1T2MEtNSVfitHistManager<pat::Muon, pat::Tau> PATMuTauPairNSVfitHistManager;
typedef CompositePtrCandidateT1T2MEtNSVfitHistManager<pat::Tau, pat::Tau> PATDiTauPairNSVfitHistManager;
typedef CompositePtrCandidateT1T2MEtNSVfitHistManager<pat::Electron, pat::Muon> PATElecMuPairNSVfitHistManager;
typedef CompositePtrCandidateT1T2MEtNSVfitHistManager<pat::Electron, pat::Electron> PATDiElecPairNSVfitHistManager;
typedef CompositePtrCandidateT1T2MEtNSVfitHistManager<pat::Muon, pat::Muon> PATDiMuPairNSVfitHistManager;

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATElecTauPairNSVfitHistManager, "PATElecTauPairNSVfitHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, PATElecTauPairNSVfitHistManager, "PATElecTauPairNSVfitHistManager");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATMuTauPairNSVfitHistManager, "PATMuTauPairNSVfitHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, PATMuTauPairNSVfitHistManager, "PATMuTauPairNSVfitHistManager");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATDiTauPairNSVfitHistManager, "PATDiTauPairNSVfitHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, PATDiTauPairNSVfitHistManager, "PATDiTauPairNSVfitHistManager");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATElecMuPairNSVfitHistManager, "PATElecMuPairNSVfitHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, PATElecMuPairNSVfitHistManager, "PATElecMuPairNSVfitHistManager");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATDiElecPairNSVfitHistManager, "PATDiElecPairNSVfitHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, PATDiElecPairNSVfitHistManager, "PATDiElecPairNSVfitHistManager");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATDiMuPairNSVfitHistManager, "PATDiMuPairNSVfitHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, PATDiMuPairNSVfitHistManager, "PATDiMuPairNSVfitHistManager");

#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<PATElecTauPairNSVfitHistManager> PATElecTauPairNSVfitAnalyzer;
typedef HistManagerAdapter<PATMuTauPairNSVfitHistManager> PATMuTauPairNSVfitAnalyzer;
typedef HistManagerAdapter<PATDiTauPairNSVfitHistManager> PATDiTauPairNSVfitAnalyzer;
typedef HistManagerAdapter<PATElecMuPairNSVfitHistManager> PATElecMuPairNSVfitAnalyzer;
typedef HistManagerAdapter<PATDiElecPairNSVfitHistManager> PATDiElecPairNSVfitAnalyzer;
typedef HistManagerAdapter<PATDiMuPairNSVfitHistManager> PATDiMuPairNSVfitAnalyzer;

DEFINE_FWK_MODULE(PATElecTauPairNSVfitAnalyzer);
DEFINE_FWK_MODULE(PATMuTauPairNSVfitAnalyzer);
DEFINE_FWK_MODULE(PATDiTauPairNSVfitAnalyzer);
DEFINE_FWK_MODULE(PATElecMuPairNSVfitAnalyzer);
DEFINE_FWK_MODULE(PATDiElecPairNSVfitAnalyzer);
DEFINE_FWK_MODULE(PATDiMuPairNSVfitAnalyzer);

