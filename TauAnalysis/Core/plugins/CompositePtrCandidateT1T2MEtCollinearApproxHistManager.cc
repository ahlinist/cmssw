#include "TauAnalysis/Core/plugins/CompositePtrCandidateT1T2MEtCollinearApproxHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"
#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEtFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/Common/interface/AssociationVector.h"
#include "DataFormats/PatCandidates/interface/MET.h"

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
CompositePtrCandidateT1T2MEtCollinearApproxHistManager<T1,T2>::CompositePtrCandidateT1T2MEtCollinearApproxHistManager(const edm::ParameterSet& cfg)
  : HistManagerBase(cfg)
{
  //std::cout << "<CompositePtrCandidateT1T2MEtCollinearApproxHistManager::CompositePtrCandidateT1T2MEtCollinearApproxHistManager>:" << std::endl;

  diTauCandidateSrc_ = cfg.getParameter<edm::InputTag>("diTauCandidateSource");
  //std::cout << " diTauCandidateSrc = " << diTauCandidateSrc_ << std::endl;

  diTauLeg1WeightExtractors_ = getTauJetWeightExtractors<T1>(cfg, "diTauLeg1WeightSource");
  diTauLeg2WeightExtractors_ = getTauJetWeightExtractors<T2>(cfg, "diTauLeg2WeightSource");
  
  requireGenMatch_ = cfg.getParameter<bool>("requireGenMatch");
  //std::cout << " requireGenMatch = " << requireGenMatch_ << std::endl;

  std::string normalization_string = cfg.getParameter<std::string>("normalization");
  normMethod_ = getNormMethod(normalization_string, "diTauCandidates");

  makeCollinearApproxX1X2histograms_ = cfg.getParameter<bool>("makeCollinearApproxX1X2histograms");

  makeCollinearApproxMassDepHistograms_ = cfg.getParameter<bool>("makeCollinearApproxMassDepHistograms");
  
  collinearApproxMassDPhi12Incr_ = TMath::Pi()/18.;
  numCollinearApproxMassDiTauPtBins_ = 25;
  collinearApproxMassDiTauPtIncr_ = 2.;
  numCollinearApproxMassMEtPtBins_ = 25;
  collinearApproxMassMEtPtIncr_ = 2.;
}

template<typename T1, typename T2>
CompositePtrCandidateT1T2MEtCollinearApproxHistManager<T1,T2>::~CompositePtrCandidateT1T2MEtCollinearApproxHistManager()
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
void CompositePtrCandidateT1T2MEtCollinearApproxHistManager<T1,T2>::bookHistogramsImp()
{
  //std::cout << "<CompositePtrCandidateT1T2MEtCollinearApproxHistManager::bookHistogramsImp>:" << std::endl;

  hGenX1_ = book1D("GenX1", "gen. X_{1}", 51, -0.01, 1.01);
  hGenX2_ = book1D("GenX2", "gen. X_{2}", 51, -0.01, 1.01);
  if ( makeCollinearApproxX1X2histograms_ ) {
    hGenX1vsX2_ = book2D("GenX1vsX2", "gen. X_{1} vs. X_{2}", 51, -0.01, 1.01, 51, -0.01, 1.01);
  
    hX1vsGenX1_ = book2D("X1vsGenX1", "X_{1} vs. gen. X_{1}", 21, -0.01, 1.01, 100, -2.5, +2.5);
    hX2vsGenX2_ = book2D("X2vsGenX2", "X_{2} vs. gen. X_{2}", 21, -0.01, 1.01, 100, -2.5, +2.5);
  }

  hGenLeg1ProjGenMEt_ = book1D("GenLeg1ProjGenMEt", "gen. leg_{1} P_{T} || missing E_{T}", 75, 0., 150.);
  hGenLeg2ProjGenMEt_ = book1D("GenLeg2ProjGenMEt", "gen. leg_{2} P_{T} || missing E_{T}", 75, 0., 150.);
  
  bookWeightHistograms(*dqmStore_, "DiTauCandidateWeight", "Composite Weight", 
		       hDiTauCandidateWeightPosUnweighted_, hDiTauCandidateWeightPosWeighted_, 
		       hDiTauCandidateWeightNegUnweighted_, hDiTauCandidateWeightNegWeighted_);
  
  hCollinearApproxEffDPhi12dep_ = book1D("CollinearApproxEffDPhi12dep", "Eff. of collinear Approximation as function of #Delta#phi_{1,2}", 36, -epsilon, TMath::Pi() + epsilon);
  hCollinearApproxEffDiTauPtDep_ = book1D("CollinearApproxEffDiTauPtDep", "Eff. of collinear Approximation as function of Visible P_{T}", 50, 0., 100.);
  hCollinearApproxEffMEtDep_ = book1D("CollinearApproxEffMEtDep", "Eff. of collinear Approximation  as function of MEt", 75, 0., 150.);

  hCollinearApproxEta_ = book1D("CollinearApproxEta", "Collinear Approximation #eta", 100, -5., +5.);
  hCollinearApproxMass_ = book1D("CollinearApproxMass", "Collinear Approximation Mass", 50, 0., 250.);
  hCollinearApproxMassVsPt_ = book2D("CollinearApproxMassVsPt", "Collinear Approximation Mass vs. P_{T}", 30, 0., 150., 25, 0., 250.);
  hCollinearApproxMassVsDPhi12_ = book2D("CollinearApproxMassVsDPhi12", "Collinear Approximation Mass vs. #Delta#phi_{1,2}", 18, -epsilon, TMath::Pi() + epsilon, 25, 0., 250.);
  hCollinearApproxX1_ = book1D("CollinearApproxX1", "Collinear Approximation X_{1}", 100, -2.5, +2.5);
  hCollinearApproxX2_ = book1D("CollinearApproxX2", "Collinear Approximation X_{2}", 100, -2.5, +2.5);
  if ( makeCollinearApproxX1X2histograms_ ) {
    hCollinearApproxX1vsX2_ = book2D("CollinearApproxX1vsX2", "Collinear Approximation X_{1} vs. X_{2}", 50, -2.5, +2.5, 50, -2.5, +2.5);
  }

  if ( makeCollinearApproxMassDepHistograms_ ) {
    int numDPhi12bins = TMath::CeilNint(TMath::Pi()/collinearApproxMassDPhi12Incr_);
    for ( int iDPhi12bin = 0; iDPhi12bin < numDPhi12bins; ++iDPhi12bin ) {
      std::ostringstream iDPhi12binString;
      iDPhi12binString << std::setfill('0') << std::setw(2) << iDPhi12bin;
    
      std::string hCollinearApproxMassDPhi12depName_i 
	= std::string("CollinearApproxMassDPhi12dep").append("_").append(iDPhi12binString.str());
      hCollinearApproxMassDPhi12dep_.push_back(book1D(hCollinearApproxMassDPhi12depName_i,
						      hCollinearApproxMassDPhi12depName_i, 50, 0., 250.));
    }
    
    for ( unsigned iDiTauPtBin = 0; iDiTauPtBin < numCollinearApproxMassDiTauPtBins_; ++iDiTauPtBin ) {
      std::ostringstream iDiTauPtBinString;
      iDiTauPtBinString << std::setfill('0') << std::setw(2) << iDiTauPtBin;
    
      std::string hCollinearApproxMassDiTauPtDepName_i 
	= std::string("CollinearApproxMassDiTauPtDep").append("_").append(iDiTauPtBinString.str());
      hCollinearApproxMassDiTauPtDep_.push_back(book1D(hCollinearApproxMassDiTauPtDepName_i,
						       hCollinearApproxMassDiTauPtDepName_i, 50, 0., 250.));
    }

    for ( unsigned iMEtPtBin = 0; iMEtPtBin < numCollinearApproxMassMEtPtBins_; ++iMEtPtBin ) {
      std::ostringstream iMEtPtBinString;
      iMEtPtBinString << std::setfill('0') << std::setw(2) << iMEtPtBin;
    
      std::string hCollinearApproxMassMEtPtDepName_i 
	= std::string("CollinearApproxMassMEtPtDep").append("_").append(iMEtPtBinString.str());
      hCollinearApproxMassMEtPtDep_.push_back(book1D(hCollinearApproxMassMEtPtDepName_i,
						     hCollinearApproxMassMEtPtDepName_i, 50, 0., 250.));
    }
  }
}

template<typename T1, typename T2>
double CompositePtrCandidateT1T2MEtCollinearApproxHistManager<T1,T2>::getDiTauCandidateWeight(const CompositePtrCandidateT1T2MEt<T1,T2>& diTauCandidate)
{
  double diTauLeg1Weight = getTauJetWeight<T1>(*diTauCandidate.leg1(), diTauLeg1WeightExtractors_);
  double diTauLeg2Weight = getTauJetWeight<T2>(*diTauCandidate.leg2(), diTauLeg2WeightExtractors_);
  return (diTauLeg1Weight*diTauLeg2Weight);
}

template<typename T1, typename T2>
void CompositePtrCandidateT1T2MEtCollinearApproxHistManager<T1,T2>::fillHistogramsImp(const edm::Event& evt, const edm::EventSetup& es, double evtWeight)
{  
  //std::cout << "<CompositePtrCandidateT1T2MEtCollinearApproxHistManager::fillHistogramsImp>:" << std::endl; 

  typedef std::vector<CompositePtrCandidateT1T2MEt<T1,T2> > CompositePtrCandidateCollection;
  edm::Handle<CompositePtrCandidateCollection> diTauCandidates;
  evt.getByLabel(diTauCandidateSrc_, diTauCandidates);

  //std::cout << " diTauCandidates.size = " << diTauCandidates->size() << std::endl;

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

    if ( diTauCandidate->p4Leg1gen().energy() > epsilon && 
	 diTauCandidate->p4Leg2gen().energy() > epsilon ) {

      hGenX1_->Fill(diTauCandidate->x1gen(), weight);
      hGenX2_->Fill(diTauCandidate->x2gen(), weight);

      if ( makeCollinearApproxX1X2histograms_ ) {
	hGenX1vsX2_->Fill(diTauCandidate->x1gen(), diTauCandidate->x2gen(), weight);
	
	hX1vsGenX1_->Fill(diTauCandidate->x1gen(), diTauCandidate->x1CollinearApprox(), weight);
	hX2vsGenX2_->Fill(diTauCandidate->x2gen(), diTauCandidate->x2CollinearApprox(), weight);
      }
      
      if ( diTauCandidate->met()->pt() > 0. && dynamic_cast<const pat::MET*>(diTauCandidate->met().get()) ) {
	const pat::MET* recoMEt = dynamic_cast<const pat::MET*>(diTauCandidate->met().get());

	double genMEtPt = recoMEt->genMET()->pt();
	double genMEtPx = recoMEt->genMET()->px();
	double genMEtPy = recoMEt->genMET()->py();
	
	double genLeg1ProjGenMEt = (diTauCandidate->p4VisLeg1gen().px()*genMEtPx + diTauCandidate->p4VisLeg1gen().py()*genMEtPy)/genMEtPt;
	double genLeg2ProjGenMEt = (diTauCandidate->p4VisLeg2gen().px()*genMEtPx + diTauCandidate->p4VisLeg2gen().py()*genMEtPy)/genMEtPt;
	
	hGenLeg1ProjGenMEt_->Fill(genLeg1ProjGenMEt, weight);
	hGenLeg2ProjGenMEt_->Fill(genLeg2ProjGenMEt, weight);
      }
    }

    fillWeightHistograms(hDiTauCandidateWeightPosUnweighted_, hDiTauCandidateWeightPosWeighted_, 
			 hDiTauCandidateWeightNegUnweighted_, hDiTauCandidateWeightNegWeighted_, diTauCandidateWeight);

    hCollinearApproxEffDPhi12dep_->Fill(diTauCandidate->dPhi12(), weight);
    hCollinearApproxEffDiTauPtDep_->Fill(diTauCandidate->p4Vis().pt(), weight);
    hCollinearApproxEffMEtDep_->Fill(diTauCandidate->met()->pt(), weight);

    hCollinearApproxEta_->Fill(diTauCandidate->p4CollinearApprox().eta(), weight);
    hCollinearApproxMass_->Fill(diTauCandidate->p4CollinearApprox().mass(), weight);
    hCollinearApproxMassVsPt_->Fill(diTauCandidate->p4Vis().pt(), diTauCandidate->p4CollinearApprox().mass(), weight);
    hCollinearApproxMassVsDPhi12_->Fill(diTauCandidate->dPhi12(), diTauCandidate->p4CollinearApprox().mass(), weight);
    hCollinearApproxX1_->Fill(diTauCandidate->x1CollinearApprox(), weight);
    hCollinearApproxX2_->Fill(diTauCandidate->x2CollinearApprox(), weight);
    if ( makeCollinearApproxX1X2histograms_ ) {
      hCollinearApproxX1vsX2_->Fill(diTauCandidate->x1CollinearApprox(), diTauCandidate->x2CollinearApprox(), weight);
    }

    if ( makeCollinearApproxMassDepHistograms_  ) {
      int iDPhi12bin = TMath::FloorNint(diTauCandidate->dPhi12()/collinearApproxMassDPhi12Incr_);
      if ( iDPhi12bin >= 0 && iDPhi12bin < (int)hCollinearApproxMassDPhi12dep_.size() )
	hCollinearApproxMassDPhi12dep_[iDPhi12bin]->Fill(diTauCandidate->p4CollinearApprox().mass(), weight);
      
      int iDiTauPtBin = TMath::FloorNint(diTauCandidate->p4Vis().pt()/collinearApproxMassDiTauPtIncr_);
      if ( iDiTauPtBin >= 0 && iDiTauPtBin < (int)hCollinearApproxMassDiTauPtDep_.size() )
	hCollinearApproxMassDiTauPtDep_[iDiTauPtBin]->Fill(diTauCandidate->p4CollinearApprox().mass(), weight);
      
      int iMEtPtBin = TMath::FloorNint(diTauCandidate->met()->pt()/collinearApproxMassMEtPtIncr_);
      if ( iMEtPtBin >= 0 && iMEtPtBin < (int)hCollinearApproxMassMEtPtDep_.size() )
	hCollinearApproxMassMEtPtDep_[iMEtPtBin]->Fill(diTauCandidate->p4CollinearApprox().mass(), weight);
    }
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/Candidate/interface/Candidate.h" 
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

typedef CompositePtrCandidateT1T2MEtCollinearApproxHistManager<reco::Candidate, reco::Candidate> DiCandidatePairCollinearApproxHistManager;
typedef CompositePtrCandidateT1T2MEtCollinearApproxHistManager<pat::Electron, pat::Tau> PATElecTauPairCollinearApproxHistManager;
typedef CompositePtrCandidateT1T2MEtCollinearApproxHistManager<pat::Muon, pat::Tau> PATMuTauPairCollinearApproxHistManager;
typedef CompositePtrCandidateT1T2MEtCollinearApproxHistManager<pat::Tau, pat::Tau> PATDiTauPairCollinearApproxHistManager;
typedef CompositePtrCandidateT1T2MEtCollinearApproxHistManager<pat::Electron, pat::Muon> PATElecMuPairCollinearApproxHistManager;

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, DiCandidatePairCollinearApproxHistManager, "DiCandidatePairCollinearApproxHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, DiCandidatePairCollinearApproxHistManager, "DiCandidatePairCollinearApproxHistManager");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATElecTauPairCollinearApproxHistManager, "PATElecTauPairCollinearApproxHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, PATElecTauPairCollinearApproxHistManager, "PATElecTauPairCollinearApproxHistManager");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATMuTauPairCollinearApproxHistManager, "PATMuTauPairCollinearApproxHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, PATMuTauPairCollinearApproxHistManager, "PATMuTauPairCollinearApproxHistManager");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATDiTauPairCollinearApproxHistManager, "PATDiTauPairCollinearApproxHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, PATDiTauPairCollinearApproxHistManager, "PATDiTauPairCollinearApproxHistManager");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATElecMuPairCollinearApproxHistManager, "PATElecMuPairCollinearApproxHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, PATElecMuPairCollinearApproxHistManager, "PATElecMuPairCollinearApproxHistManager");
  
#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<DiCandidatePairCollinearApproxHistManager> DiCandidatePairCollinearApproxAnalyzer;
typedef HistManagerAdapter<PATElecTauPairCollinearApproxHistManager> PATElecTauPairCollinearApproxAnalyzer;
typedef HistManagerAdapter<PATMuTauPairCollinearApproxHistManager> PATMuTauPairCollinearApproxAnalyzer;
typedef HistManagerAdapter<PATDiTauPairCollinearApproxHistManager> PATDiTauPairCollinearApproxAnalyzer;
typedef HistManagerAdapter<PATElecMuPairCollinearApproxHistManager> PATElecMuPairCollinearApproxAnalyzer;

DEFINE_ANOTHER_FWK_MODULE(DiCandidatePairCollinearApproxAnalyzer);
DEFINE_ANOTHER_FWK_MODULE(PATElecTauPairCollinearApproxAnalyzer);
DEFINE_ANOTHER_FWK_MODULE(PATMuTauPairCollinearApproxAnalyzer);
DEFINE_ANOTHER_FWK_MODULE(PATDiTauPairCollinearApproxAnalyzer);
DEFINE_ANOTHER_FWK_MODULE(PATElecMuPairCollinearApproxAnalyzer);

