#include "TauAnalysis/Core/plugins/CompositePtrCandidateT1T2MEtHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"
#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEtFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "TauAnalysis/Core/interface/histManagerAuxFunctions.h"

#include <TMath.h>

const double epsilon = 0.01;

template<typename T1, typename T2>
bool matchesGenCandidatePair(const CompositePtrCandidateT1T2MEt<T1,T2>& compositePtrCandidate)
{
  bool isGenMatched = false;
// not implemented yet...
  return isGenMatched;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

template<typename T1, typename T2>
CompositePtrCandidateT1T2MEtHistManager<T1,T2>::CompositePtrCandidateT1T2MEtHistManager(const edm::ParameterSet& cfg)
  : HistManagerBase(cfg)
{
  //std::cout << "<CompositePtrCandidateT1T2MEtHistManager::CompositePtrCandidateT1T2MEtHistManager>:" << std::endl;

  diTauCandidateSrc_ = cfg.getParameter<edm::InputTag>("diTauCandidateSource");
  //std::cout << " diTauCandidateSrc = " << diTauCandidateSrc_ << std::endl;

  vertexSrc_ = cfg.getParameter<edm::InputTag>("vertexSource");
  //std::cout << " vertexSrc = " << vertexSrc_ << std::endl;

  visMassHypothesisSrc_ = ( cfg.exists("visMassHypothesisSource") ) ?  
    cfg.getParameter<edm::InputTag>("visMassHypothesisSource") : edm::InputTag();
  //std::cout << " visMassHypothesisSrc = " << visMassHypothesisSrc_ << std::endl;

  diTauLeg1WeightExtractors_ = getTauJetWeightExtractors<T1>(cfg, "diTauLeg1WeightSource");
  diTauLeg2WeightExtractors_ = getTauJetWeightExtractors<T2>(cfg, "diTauLeg2WeightSource");
  
  requireGenMatch_ = cfg.getParameter<bool>("requireGenMatch");
  //std::cout << " requireGenMatch = " << requireGenMatch_ << std::endl;

  std::string normalization_string = cfg.getParameter<std::string>("normalization");
  normMethod_ = getNormMethod(normalization_string, "diTauCandidates");
}

template<typename T1, typename T2>
CompositePtrCandidateT1T2MEtHistManager<T1,T2>::~CompositePtrCandidateT1T2MEtHistManager()
{
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
void CompositePtrCandidateT1T2MEtHistManager<T1,T2>::bookHistogramsImp()
{
  //std::cout << "<CompositePtrCandidateT1T2MEtHistManager::bookHistogramsImp>:" << std::endl;
  
  hDiTauCandidatePt_ = book1D("DiTauCandidatePt", "Composite P_{T}", 75, 0., 150.);
  hDiTauCandidateEta_ = book1D("DiTauCandidateEta", "Composite #eta", 100, -5., +5.);
  hDiTauCandidatePhi_ = book1D("DiTauCandidatePhi", "Composite #phi", 36, -TMath::Pi(), +TMath::Pi());
  hDiTauCandidateCharge_ = book1D("DiTauCandidateCharge", "Composite Charge", 11, -5.5, +5.5);
  hDiTauCandidateMass_ = book1D("DiTauCandidateMass", "Composite Invariant Mass", 50, 0., 250.);
  
  bookWeightHistograms(*dqmStore_, "DiTauCandidateWeight", "Composite Weight", 
		       hDiTauCandidateWeightPosUnweighted_, hDiTauCandidateWeightPosWeighted_, 
		       hDiTauCandidateWeightNegUnweighted_, hDiTauCandidateWeightNegWeighted_);

  hDiTauCandidateImpParSig_ = book1D("DiTauCandidateImpParSig", "#sqrt{#frac{dxy_{1}}{#Delta dxy_{1}}^{2}+#frac{dxy_{2}}{#Delta dxy_{2}}^{2}}", 30, 0., 15.);
  
  hVisPt_ = book1D("VisPt", "Visible P_{T}", 50, 0., 100.);
  hVisPhi_ = book1D("VisPhi", "Visible #phi", 36, -TMath::Pi(), +TMath::Pi());
  hVisMass_ = book1D("VisMass", "Visible Mass", 40, 0., 200.);
  hVisMassZllCombinedHypothesis_ = ( visMassHypothesisSrc_.label() != "" ) ?
    book1D("VisMassZllCombinedHypothesis", "Visible Mass (combined Value of different Event Hypotheses)", 40, 0., 200.) : 0;

  hCollinearApproxEta_ = book1D("CollinearApproxEta", "Collinear Approximation #eta", 100, -5., +5.);
  hCollinearApproxMass_ = book1D("CollinearApproxMass", "Collinear Approximation Mass", 50, 0., 250.);
  hCollinearApproxMassVsPt_ = book2D("CollinearApproxMassVsPt", "Collinear Approximation Mass vs P_{T}", 30, 0., 150., 25, 0., 250.);
  hCollinearApproxMassVsDPhi12_ = book2D("CollinearApproxMassVsDPhi12", "Collinear Approximation Mass vs #Delta#phi_{1,2}", 18, -epsilon, TMath::Pi() + epsilon, 25, 0., 250.);
  hCollinearApproxX1_ = book1D("CollinearApproxX1", "Collinear Approximation X_{1}", 51, -0.01, 1.01);
  hCollinearApproxX2_ = book1D("CollinearApproxX2", "Collinear Approximation X_{2}", 51, -0.01, 1.01);
  hCollinearApproxX1vsX2_ = book2D("CollinearApproxX1vsX2", "Collinear Approximation X_{1} vs X_{2}", 26, -0.02, 1.02, 26, -0.02, 1.02);
  
  hCDFmethodMass_ = book1D("CDFmethodMass", "CDF Method Mass", 50, 0., 250.);
  
  hMt12MET_ = book1D("Mt12MET", "Mass_{T 1,2,MET}", 50, 0., 250.);
  
  hMt1MET_ = book1D("Mt1MET", "Mass_{T 1,MET}", 40, 0., 200.);
  hMt2MET_ = book1D("Mt2MET", "Mass_{T 2,MET}", 40, 0., 200.);
  
  hDPhi12_ = book1D("DPhi12", "#Delta#phi_{1,2}", 36, -epsilon, TMath::Pi() + epsilon);
  hDR12_ = book1D("DR12", "#Delta R_{1,2}", 51, -0.1, 10.1);
  
  hVisEtaMin_ = book1D("VisEtaMin", "VisEtaMin", 60, -3., +3.);
  hVisEtaMax_ = book1D("VisEtaMax", "VisEtaMax", 60, -3., +3.);
  
  hDPhi1MET_ = book1D("DPhi1MET", "#Delta#phi_{1,MET}", 36, -epsilon, TMath::Pi() + epsilon);
  hDPhi2MET_ = book1D("DPhi2MET", "#Delta#phi_{2,MET}", 36, -epsilon, TMath::Pi() + epsilon);
  hDPhi1METvsDPhi2MET_ = book2D("DPhi1METvsDPhi2MET", "#Delta#phi_{1,MET} vs #Delta#phi_{2,MET}", 18, -epsilon, TMath::Pi() + epsilon, 18, -epsilon, TMath::Pi() + epsilon); 

  hPzetaCorr_ = book2D("PzetaCorr", "P_{#zeta} vs P_{#zeta}^{vis}", 10, 0., 50., 14, -20., 50.);
  hPzetaDiff_ = book1D("PzetaDiff", "P_{#zeta} - 1.5*P_{#zeta}^{vis}", 40, -100., +100.);
}

template<typename T1, typename T2>
double CompositePtrCandidateT1T2MEtHistManager<T1,T2>::getDiTauCandidateWeight(const CompositePtrCandidateT1T2MEt<T1,T2>& diTauCandidate)
{
  double diTauLeg1Weight = getTauJetWeight<T1>(*diTauCandidate.leg1(), diTauLeg1WeightExtractors_);
  double diTauLeg2Weight = getTauJetWeight<T2>(*diTauCandidate.leg2(), diTauLeg2WeightExtractors_);
  return (diTauLeg1Weight*diTauLeg2Weight);
}

template<typename T1, typename T2>
void CompositePtrCandidateT1T2MEtHistManager<T1,T2>::fillHistogramsImp(const edm::Event& evt, const edm::EventSetup& es, double evtWeight)
{  
  //std::cout << "<CompositePtrCandidateT1T2MEtHistManager::fillHistogramsImp>:" << std::endl; 

  typedef std::vector<CompositePtrCandidateT1T2MEt<T1,T2> > CompositePtrCandidateCollection;
  edm::Handle<CompositePtrCandidateCollection> diTauCandidates;
  evt.getByLabel(diTauCandidateSrc_, diTauCandidates);

  //std::cout << " diTauCandidates.size = " << diTauCandidates->size() << std::endl;

  edm::Handle<std::vector<reco::Vertex> > recoVertices;
  evt.getByLabel(vertexSrc_, recoVertices);

  double diTauCandidateWeightSum = 0.;
  for ( typename CompositePtrCandidateCollection::const_iterator diTauCandidate = diTauCandidates->begin(); 
	diTauCandidate != diTauCandidates->end(); ++diTauCandidate ) {
    if ( requireGenMatch_ && !matchesGenCandidatePair(*diTauCandidate) ) continue;

    diTauCandidateWeightSum += getDiTauCandidateWeight(*diTauCandidate);
  }

  for ( typename CompositePtrCandidateCollection::const_iterator diTauCandidate = diTauCandidates->begin(); 
	diTauCandidate != diTauCandidates->end(); ++diTauCandidate ) {

    //bool isGenMatched = matchesGenCandidatePair(*diTauCandidate);
    //std::cout << " Pt = " << diTauCandidate->pt() << ", phi = " << diTauCandidate->phi() << ", visMass = " << diTauCandidate->p4Vis().mass() << std::endl;
    //std::cout << " isGenMatched = " << isGenMatched << std::endl;

    if ( requireGenMatch_ && !matchesGenCandidatePair(*diTauCandidate) ) continue;

    double diTauCandidateWeight = getDiTauCandidateWeight(*diTauCandidate);
    double weight = getWeight(evtWeight, diTauCandidateWeight, diTauCandidateWeightSum);

    hDiTauCandidatePt_->Fill(diTauCandidate->pt(), weight);
    hDiTauCandidateEta_->Fill(diTauCandidate->eta(), weight);
    hDiTauCandidatePhi_->Fill(diTauCandidate->phi(), weight);
    hDiTauCandidateCharge_->Fill(diTauCandidate->charge(), weight);
    hDiTauCandidateMass_->Fill(diTauCandidate->mass(), weight);

    fillWeightHistograms(hDiTauCandidateWeightPosUnweighted_, hDiTauCandidateWeightPosWeighted_, 
			 hDiTauCandidateWeightNegUnweighted_, hDiTauCandidateWeightNegWeighted_, diTauCandidateWeight);

    const reco::Track* trackLeg1 = trackExtractorLeg1_(*diTauCandidate->leg1());
    const reco::Track* trackLeg2 = trackExtractorLeg2_(*diTauCandidate->leg2());
    if ( trackLeg1 && trackLeg2 &&
	 recoVertices->size() >= 1 ) {
      const reco::Vertex& thePrimaryEventVertex = (*recoVertices->begin());

      double trackLeg1IpSig = trackLeg1->dxy(thePrimaryEventVertex.position())/trackLeg1->dxyError();
      double trackLeg2IpSig = trackLeg2->dxy(thePrimaryEventVertex.position())/trackLeg2->dxyError();

      hDiTauCandidateImpParSig_->Fill(TMath::Sqrt(trackLeg1IpSig*trackLeg1IpSig + trackLeg2IpSig*trackLeg2IpSig), weight);
    }

    hVisPt_->Fill(diTauCandidate->p4Vis().pt(), weight);
    hVisPhi_->Fill(diTauCandidate->p4Vis().phi(), weight);
    hVisMass_->Fill(diTauCandidate->p4Vis().mass(), weight);
    if ( visMassHypothesisSrc_.label() != "" ) {
      typedef edm::RefProd<CompositePtrCandidateCollection> CompositePtrCandidateRefProd;
      typedef std::vector<float> vfloat;
      typedef edm::AssociationVector<CompositePtrCandidateRefProd, vfloat> ZtautauVisMassHypothesisCollection;
      edm::Handle<ZtautauVisMassHypothesisCollection> visMassHypotheses;
      evt.getByLabel(visMassHypothesisSrc_, visMassHypotheses);

      unsigned numDiTauCandidates = diTauCandidates->size();
      for ( unsigned idxDiTauCandidate = 0; idxDiTauCandidate < numDiTauCandidates; ++idxDiTauCandidate ) {
	edm::Ref<CompositePtrCandidateCollection> diTauCandidateRef(diTauCandidates, idxDiTauCandidate);

	if ( diTauCandidateRef.get() != &(*diTauCandidate) ) continue;

	hVisMassZllCombinedHypothesis_->Fill((*visMassHypotheses)[diTauCandidateRef], weight);
      }
    }

    if ( diTauCandidate->collinearApproxIsValid() ) {
      hCollinearApproxEta_->Fill(diTauCandidate->p4CollinearApprox().eta(), weight);
      hCollinearApproxMass_->Fill(diTauCandidate->p4CollinearApprox().mass(), weight);
      hCollinearApproxMassVsPt_->Fill(diTauCandidate->p4CollinearApprox().mass(), diTauCandidate->p4CollinearApprox().pt(), weight);
      hCollinearApproxMassVsDPhi12_->Fill(diTauCandidate->p4CollinearApprox().mass(), diTauCandidate->dPhi12(), weight);
      hCollinearApproxX1_->Fill(diTauCandidate->x1CollinearApprox(), weight);
      hCollinearApproxX2_->Fill(diTauCandidate->x2CollinearApprox(), weight);
      hCollinearApproxX1vsX2_->Fill(diTauCandidate->x1CollinearApprox(), diTauCandidate->x2CollinearApprox(), weight);
    }

    hCDFmethodMass_->Fill(diTauCandidate->p4CDFmethod().mass(), weight);

    hMt12MET_->Fill(diTauCandidate->mt12MET(), weight);

    hMt1MET_->Fill(diTauCandidate->mt1MET(), weight);
    hMt2MET_->Fill(diTauCandidate->mt2MET(), weight);

    hDPhi12_->Fill(diTauCandidate->dPhi12(), weight);
    hDR12_->Fill(diTauCandidate->dR12(), weight);

    hVisEtaMin_->Fill(diTauCandidate->visEtaMin(), weight);
    hVisEtaMax_->Fill(diTauCandidate->visEtaMax(), weight);

    hDPhi1MET_->Fill(diTauCandidate->dPhi1MET(), weight);
    hDPhi2MET_->Fill(diTauCandidate->dPhi2MET(), weight);
    hDPhi1METvsDPhi2MET_->Fill(diTauCandidate->dPhi1MET(), diTauCandidate->dPhi2MET(), weight);

    hPzetaCorr_->Fill(diTauCandidate->pZetaVis(), diTauCandidate->pZeta(), weight);
    hPzetaDiff_->Fill(diTauCandidate->pZeta() - 1.5*diTauCandidate->pZetaVis(), weight);
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/Candidate/interface/Candidate.h" 
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

typedef CompositePtrCandidateT1T2MEtHistManager<reco::Candidate, reco::Candidate> DiCandidatePairHistManager;
typedef CompositePtrCandidateT1T2MEtHistManager<pat::Electron, pat::Tau> PATElecTauPairHistManager;
typedef CompositePtrCandidateT1T2MEtHistManager<pat::Muon, pat::Tau> PATMuTauPairHistManager;
typedef CompositePtrCandidateT1T2MEtHistManager<pat::Tau, pat::Tau> PATDiTauPairHistManager;
typedef CompositePtrCandidateT1T2MEtHistManager<pat::Electron, pat::Muon> PATElecMuPairHistManager;

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, DiCandidatePairHistManager, "DiCandidatePairHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, DiCandidatePairHistManager, "DiCandidatePairHistManager");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATElecTauPairHistManager, "PATElecTauPairHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, PATElecTauPairHistManager, "PATElecTauPairHistManager");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATMuTauPairHistManager, "PATMuTauPairHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, PATMuTauPairHistManager, "PATMuTauPairHistManager");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATDiTauPairHistManager, "PATDiTauPairHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, PATDiTauPairHistManager, "PATDiTauPairHistManager");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATElecMuPairHistManager, "PATElecMuPairHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, PATElecMuPairHistManager, "PATElecMuPairHistManager");
  
#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<DiCandidatePairHistManager> DiCandidatePairAnalyzer;
typedef HistManagerAdapter<PATElecTauPairHistManager> PATElecTauPairAnalyzer;
typedef HistManagerAdapter<PATMuTauPairHistManager> PATMuTauPairAnalyzer;
typedef HistManagerAdapter<PATDiTauPairHistManager> PATDiTauPairAnalyzer;
typedef HistManagerAdapter<PATElecMuPairHistManager> PATElecMuPairAnalyzer;

DEFINE_ANOTHER_FWK_MODULE(DiCandidatePairAnalyzer);
DEFINE_ANOTHER_FWK_MODULE(PATElecTauPairAnalyzer);
DEFINE_ANOTHER_FWK_MODULE(PATMuTauPairAnalyzer);
DEFINE_ANOTHER_FWK_MODULE(PATDiTauPairAnalyzer);
DEFINE_ANOTHER_FWK_MODULE(PATElecMuPairAnalyzer);

