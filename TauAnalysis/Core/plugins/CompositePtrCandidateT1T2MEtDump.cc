#include "TauAnalysis/Core/plugins/CompositePtrCandidateT1T2MEtDump.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/Math/interface/angle.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "TauAnalysis/Core/interface/eventDumpAuxFunctions.h"
#include "TauAnalysis/GenSimTools/interface/genParticleAuxFunctions.h"
#include "TauAnalysis/CandidateTools/interface/candidateAuxFunctions.h"

#include <TMath.h>

template<typename T1, typename T2>
CompositePtrCandidateT1T2MEtDump<T1,T2>::CompositePtrCandidateT1T2MEtDump(const edm::ParameterSet& cfg)
  : ObjectDumpBase(cfg),
    diTauCandidateSource_(cfg.getParameter<edm::InputTag>("diTauCandidateSource")),
    genParticleSource_(cfg.getParameter<edm::InputTag>("genParticleSource"))
{
  if ( cfg.exists("svFitAlgorithms") ) {
    typedef std::vector<edm::ParameterSet> vParameterSet;
    vParameterSet cfgSVfitAlgorithms = cfg.getParameter<vParameterSet>("svFitAlgorithms");
    for ( vParameterSet::const_iterator cfgSVfitAlgorithm = cfgSVfitAlgorithms.begin();
	  cfgSVfitAlgorithm != cfgSVfitAlgorithms.end(); ++cfgSVfitAlgorithm ) {
      svFitAlgorithmType svfitAlgorithm;
      svfitAlgorithm.algorithmName_ = cfgSVfitAlgorithm->getParameter<std::string>("name");    
      if ( cfgSVfitAlgorithm->exists("polarizationHypotheses") ) {
	svfitAlgorithm.polarizationHypotheses_ = cfgSVfitAlgorithm->getParameter<vstring>("polarizationHypotheses");
      } else {
	svfitAlgorithm.polarizationHypotheses_.push_back(std::string("Unknown"));
      }
      svFitAlgorithms_.push_back(svfitAlgorithm);
    }
  }

  if ( cfg.exists("annotations") ) {
    typedef std::vector<edm::ParameterSet> vParameterSet;
    vParameterSet cfgAnnotations = cfg.getParameter<vParameterSet>("annotations");
    for ( vParameterSet::const_iterator cfgAnnotation = cfgAnnotations.begin();
	  cfgAnnotation != cfgAnnotations.end(); ++cfgAnnotation ) {
      annotationType* annotation = new annotationType(*cfgAnnotation);
      annotations_.push_back(annotation);
    }
  }
}

template<typename T1, typename T2>
CompositePtrCandidateT1T2MEtDump<T1,T2>::~CompositePtrCandidateT1T2MEtDump()
{
  for ( typename std::vector<annotationType*>::iterator it = annotations_.begin();
	it != annotations_.end(); ++it ) {
    delete (*it);
  }
}

double compDeltaRlegNu(const reco::Candidate::LorentzVector& p4leg, const reco::GenParticleCollection& genParticles)
{
  std::vector<int> tauLeptonPdgIds;
  tauLeptonPdgIds.push_back(+15);
  tauLeptonPdgIds.push_back(-15);

  const reco::GenParticle* genTauLepton = findGenParticle(p4leg, genParticles);

  if ( genTauLepton ) {
    reco::Candidate::LorentzVector genVisMomentum = getVisMomentum(genTauLepton, &genParticles);
    reco::Candidate::LorentzVector genInvisMomentum = getInvisMomentum(genTauLepton, &genParticles);

    return angle(genVisMomentum, genInvisMomentum)*180./TMath::Pi();
  }

  return -1.;
}

template<typename T1, typename T2>
void CompositePtrCandidateT1T2MEtDump<T1,T2>::print(const edm::Event& evt, const edm::EventSetup& es) const
{
  if ( !outputStream_ ) {
    edm::LogError ("print") << " Data-member outputStream undefined --> skipping !!";
    return;
  }

  std::cout << "<CompositePtrCandidateT1T2MEtDump::print>:" << std::endl;
  std::cout << " src = " << diTauCandidateSource_.label() << std::endl;

  typedef std::vector<CompositePtrCandidateT1T2MEt<T1,T2> > CompositePtrCandidateCollection;
  edm::Handle<CompositePtrCandidateCollection> diTauCandidates;
  evt.getByLabel(diTauCandidateSource_, diTauCandidates);

  edm::Handle<reco::GenParticleCollection> genParticles;
  if( genParticleSource_.label() != "") evt.getByLabel(genParticleSource_, genParticles);
      
  unsigned iDiTauCandidate = 0;
  for ( typename CompositePtrCandidateCollection::const_iterator diTauCandidate = diTauCandidates->begin(); 
	diTauCandidate != diTauCandidates->end(); ++diTauCandidate ) {
    *outputStream_ << "DiTauCandidate(" << iDiTauCandidate << "):" << std::endl;    
    *outputStream_ << " Pt = " << diTauCandidate->pt() << std::endl;
    *outputStream_ << " genPt = " << diTauCandidate->p4gen().pt() << std::endl;
    *outputStream_ << " theta = " << diTauCandidate->theta()*180./TMath::Pi() 
		   << " (eta = " << diTauCandidate->eta() << ")" << std::endl;
    *outputStream_ << " phi = " << diTauCandidate->phi()*180./TMath::Pi() << std::endl;
    *outputStream_ << " Leg1" << std::endl;
    *outputStream_ << "  Pt = " << diTauCandidate->leg1()->pt() << std::endl;
    *outputStream_ << "  genPt = " << diTauCandidate->p4VisLeg1gen().pt() << std::endl;
    *outputStream_ << "  theta = " << diTauCandidate->leg1()->theta()*180./TMath::Pi() 
		   << " (eta = " << diTauCandidate->leg1()->eta() << ")" << std::endl;
    *outputStream_ << "  phi = " << diTauCandidate->leg1()->phi()*180./TMath::Pi() << std::endl;
    *outputStream_ << "  pdgId = " << diTauCandidate->leg1()->pdgId() << std::endl;
    *outputStream_ << " Leg2" << std::endl;
    *outputStream_ << "  Pt = " << diTauCandidate->leg2()->pt() << std::endl;
    *outputStream_ << "  genPt = " << diTauCandidate->p4VisLeg2gen().pt() << std::endl;
    *outputStream_ << "  theta = " << diTauCandidate->leg2()->theta()*180./TMath::Pi() 
		   << " (eta = " << diTauCandidate->leg2()->eta() << ")" << std::endl;
    *outputStream_ << "  phi = " << diTauCandidate->leg2()->phi()*180./TMath::Pi() << std::endl;
    *outputStream_ << "  pdgId = " << diTauCandidate->leg2()->pdgId() << std::endl;
    *outputStream_ << " dPhi(Leg1,Leg2) = " << diTauCandidate->dPhi12()*180./TMath::Pi() << std::endl;    
    *outputStream_ << " M(visible) = " << diTauCandidate->p4Vis().mass() << std::endl;
    *outputStream_ << " Mt(Leg1+MET) = " << diTauCandidate->mt1MET() << std::endl;
    *outputStream_ << " Mt(Leg2+MET) = " << diTauCandidate->mt2MET() << std::endl;
    *outputStream_ << " PzetaDiff = " << (diTauCandidate->pZeta() - 1.5*diTauCandidate->pZetaVis()) << std::endl;
    *outputStream_ << " M(CDF method) = " << diTauCandidate->p4CDFmethod().mass() << std::endl;
    *outputStream_ << " M(collinear Approx.) = " << diTauCandidate->p4CollinearApprox().mass() << std::endl;
    *outputStream_ << "  x1 = " << diTauCandidate->x1CollinearApprox() 
		   << " (gen. = " << diTauCandidate->x1gen() << ")" << std::endl;
    *outputStream_ << "  x2 = " << diTauCandidate->x2CollinearApprox() 
		   << " (gen. = " << diTauCandidate->x2gen() << ")" << std::endl;
    std::string collinearApproxStatus = ( diTauCandidate->collinearApproxIsValid() ) ? "valid" : "invalid";
    *outputStream_ << " (collinear Approx. " << collinearApproxStatus << ")" << std::endl;
    *outputStream_ << " genM = " << diTauCandidate->p4gen().mass() << std::endl;
    if( genParticleSource_.label() != "") {
      *outputStream_ << " dR(leg1, nu1) = " << compDeltaRlegNu(diTauCandidate->leg1()->p4(), *genParticles) << std::endl;
      *outputStream_ << " dR(leg2, nu2) = " << compDeltaRlegNu(diTauCandidate->leg2()->p4(), *genParticles) << std::endl;
    }
    for ( typename std::vector<svFitAlgorithmType>::const_iterator svFitAlgorithm = svFitAlgorithms_.begin();
	  svFitAlgorithm != svFitAlgorithms_.end(); ++svFitAlgorithm ) {
      for ( vstring::const_iterator polarizationHypothesis = svFitAlgorithm->polarizationHypotheses_.begin();
	    polarizationHypothesis != svFitAlgorithm->polarizationHypotheses_.end(); ++polarizationHypothesis ) {
	*outputStream_ << "SVfit algorithm = " << svFitAlgorithm->algorithmName_ << "," 
		       << " polarization = " << (*polarizationHypothesis) << ":" << std::endl;
	const SVfitDiTauSolution* solution = diTauCandidate->svFitSolution(svFitAlgorithm->algorithmName_, *polarizationHypothesis);
	if ( solution ) *outputStream_ << (*solution);	
      }
    }
    for ( typename std::vector<annotationType*>::const_iterator annotation = annotations_.begin();
	  annotation != annotations_.end(); ++annotation ) {
      if ( (*(*annotation)->condition_)(*diTauCandidate) ) std::cout << (*annotation)->text_ << std::endl;
    }
    ++iDiTauCandidate;
  }

  *outputStream_ << std::endl;
}

#include "DataFormats/Candidate/interface/Candidate.h" 
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

typedef CompositePtrCandidateT1T2MEtDump<reco::Candidate, reco::Candidate> DiCandidatePairDump;
typedef CompositePtrCandidateT1T2MEtDump<pat::Electron, pat::Tau> PATElecTauPairDump;
typedef CompositePtrCandidateT1T2MEtDump<pat::Muon, pat::Tau> PATMuTauPairDump;
typedef CompositePtrCandidateT1T2MEtDump<pat::Tau, pat::Tau> PATDiTauPairDump;
typedef CompositePtrCandidateT1T2MEtDump<pat::Electron, pat::Muon> PATElecMuPairDump;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, DiCandidatePairDump, "DiCandidatePairDump");
DEFINE_EDM_PLUGIN(ObjectDumpPluginFactory, DiCandidatePairDump, "DiCandidatePairDump");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATElecTauPairDump, "PATElecTauPairDump");
DEFINE_EDM_PLUGIN(ObjectDumpPluginFactory, PATElecTauPairDump, "PATElecTauPairDump");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATMuTauPairDump, "PATMuTauPairDump");
DEFINE_EDM_PLUGIN(ObjectDumpPluginFactory, PATMuTauPairDump, "PATMuTauPairDump");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATDiTauPairDump, "PATDiTauPairDump");
DEFINE_EDM_PLUGIN(ObjectDumpPluginFactory, PATDiTauPairDump, "PATDiTauPairDump");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATElecMuPairDump, "PATElecMuPairDump");
DEFINE_EDM_PLUGIN(ObjectDumpPluginFactory, PATElecMuPairDump, "PATElecMuPairDump");

