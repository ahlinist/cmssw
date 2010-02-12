#include "TauAnalysis/Core/plugins/CompositePtrCandidateT1T2MEtDump.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/Common/interface/Handle.h"
#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"

#include "TauAnalysis/Core/interface/eventDumpAuxFunctions.h"
#include "TauAnalysis/GenSimTools/interface/genParticleAuxFunctions.h"

#include <TMath.h>

template<typename T1, typename T2>
CompositePtrCandidateT1T2MEtDump<T1,T2>::CompositePtrCandidateT1T2MEtDump(const edm::ParameterSet& cfg)
  : ObjectDumpBase(cfg),
    diTauCandidateSource_(cfg.getParameter<edm::InputTag>("diTauCandidateSource"))
{
//--- nothing to be done yet...
}

template<typename T1, typename T2>
CompositePtrCandidateT1T2MEtDump<T1,T2>::~CompositePtrCandidateT1T2MEtDump()
{
//--- nothing to be done yet...
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
      
  unsigned iDiTauCandidate = 0;
  for ( typename CompositePtrCandidateCollection::const_iterator diTauCandidate = diTauCandidates->begin(); 
	diTauCandidate != diTauCandidates->end(); ++diTauCandidate ) {
    *outputStream_ << "DiTauCandidate(" << iDiTauCandidate << "):" << std::endl;
    *outputStream_ << " Pt = " << diTauCandidate->pt() << std::endl;
    *outputStream_ << " theta = " << diTauCandidate->theta()*180./TMath::Pi() 
		   << " (eta = " << diTauCandidate->eta() << ")" << std::endl;
    *outputStream_ << " phi = " << diTauCandidate->phi()*180./TMath::Pi() << std::endl;
    *outputStream_ << " Leg1" << std::endl;
    *outputStream_ << "  Pt = " << diTauCandidate->leg1()->pt() << std::endl;
    *outputStream_ << "  theta = " << diTauCandidate->leg1()->theta()*180./TMath::Pi() 
		   << " (eta = " << diTauCandidate->leg1()->eta() << ")" << std::endl;
    *outputStream_ << "  phi = " << diTauCandidate->leg1()->phi()*180./TMath::Pi() << std::endl;
    *outputStream_ << "  pdgId = " << diTauCandidate->leg1()->pdgId() << std::endl;
    *outputStream_ << " Leg2" << std::endl;
    *outputStream_ << "  Pt = " << diTauCandidate->leg2()->pt() << std::endl;
    *outputStream_ << "  theta = " << diTauCandidate->leg2()->theta()*180./TMath::Pi() 
		   << " (eta = " << diTauCandidate->leg2()->eta() << ")" << std::endl;
    *outputStream_ << "  phi = " << diTauCandidate->leg2()->phi()*180./TMath::Pi() << std::endl;
    *outputStream_ << "  pdgId = " << diTauCandidate->leg2()->pdgId() << std::endl;
    *outputStream_ << " dPhi(Leg1,Leg2) = " << diTauCandidate->dPhi12()*180./TMath::Pi() << std::endl;
    *outputStream_ << " M(visible) = " << diTauCandidate->p4Vis().mass() << std::endl;
    *outputStream_ << " Mt(Leg1+MET) = " << diTauCandidate->mt1MET() << std::endl;
    *outputStream_ << " Mt(Leg2+MET) = " << diTauCandidate->mt2MET() << std::endl;
    *outputStream_ << " M(CDF method) = " << diTauCandidate->p4CDFmethod().mass() << std::endl;
    *outputStream_ << " M(collinear Approx.) = " << diTauCandidate->p4CollinearApprox().mass() << std::endl;
    *outputStream_ << "  x1 = " << diTauCandidate->x1CollinearApprox() 
		   << " (gen. = " << diTauCandidate->x1gen() << ")" << std::endl;
    *outputStream_ << "  x2 = " << diTauCandidate->x2CollinearApprox() 
		   << " (gen. = " << diTauCandidate->x2gen() << ")" << std::endl;
    std::string collinearApproxStatus = ( diTauCandidate->collinearApproxIsValid() ) ? "valid" : "invalid";
    *outputStream_ << " (collinear Approx. " << collinearApproxStatus << ")" << std::endl;
    ++iDiTauCandidate;
  }

  *outputStream_ << std::endl;
}

typedef CompositePtrCandidateT1T2MEtDump<reco::Candidate, reco::Candidate> DiCandidatePairDump;
typedef CompositePtrCandidateT1T2MEtDump<pat::Electron, pat::Tau> PATElecTauPairDump;
typedef CompositePtrCandidateT1T2MEtDump<pat::Muon, pat::Tau> PATMuTauPairDump;
typedef CompositePtrCandidateT1T2MEtDump<pat::Tau, pat::Tau> PATDiTauPairDump;
typedef CompositePtrCandidateT1T2MEtDump<pat::Electron, pat::Muon> PATElecMuPairDump;

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

