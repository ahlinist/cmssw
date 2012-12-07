#ifndef TauAnalysis_CandidateTools_NSVfitEventHypothesisAnalyzerT_h
#define TauAnalysis_CandidateTools_NSVfitEventHypothesisAnalyzerT_h

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Math/interface/normalizedPhi.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesisBase.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesis.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesisByIntegration.h"

#include <TMath.h>

#include <string>

template <typename T>
class NSVfitEventHypothesisAnalyzerT : public edm::EDAnalyzer 
{
  typedef std::vector<T> NSVfitEventHypothesisCollection;

 public:
  // constructor 
  explicit NSVfitEventHypothesisAnalyzerT(const edm::ParameterSet&);
    
  // destructor
  ~NSVfitEventHypothesisAnalyzerT();
    
 private:
  void beginJob();
  void analyze(const edm::Event&, const edm::EventSetup&);
  void endJob();

  std::string moduleLabel_;

  edm::InputTag srcEventHypotheses_;

  edm::InputTag srcGenTauPairs_;
  edm::InputTag srcGenLeg1_;
  edm::InputTag srcGenLeg2_;
  edm::InputTag srcGenMEt_;

  edm::InputTag srcPFMEtCovMatrix_;
  
  typedef std::vector<edm::InputTag> vInputTag;
  vInputTag srcWeights_;

  int idxResonance_;

  int numBinsSVfitMass_;
  double svFitMassMax_;
  int numBinsSVfitSigma_;
  double svFitSigmaMax_;

  std::string dqmDirectory_;

  struct plotEntryType
  {
    plotEntryType(const std::string& dqmDirectory, 
		  double minDPhi12, double maxDPhi12, int isValidSolution, double minQt, double maxQt,
		  int numBinsSVfitMass, double svFitMassMax,
		  int numBinsSVfitSigma, double svFitSigmaMax)
      : minDPhi12_(minDPhi12),
	maxDPhi12_(maxDPhi12),
	isValidSolution_(isValidSolution),
	minQt_(minQt),
	maxQt_(maxQt),
	numBinsSVfitMass_(numBinsSVfitMass), 
	svFitMassMax_(svFitMassMax),
	numBinsSVfitSigma_(numBinsSVfitSigma), 
	svFitSigmaMax_(svFitSigmaMax)
    {
      TString dqmDirectory_full = dqmDirectory.data();
      if ( !dqmDirectory_full.EndsWith("/") ) dqmDirectory_full.Append("/");
      dqmDirectory_full.Append("plotEntryType");
      if ( !dqmDirectory_full.EndsWith("/") ) dqmDirectory_full.Append("/");
      if      ( minDPhi12_ <= 0. && 
		maxDPhi12_ <= 0. ) dqmDirectory_full.Append("");
      else if ( minDPhi12_ <= 0. ) dqmDirectory_full.Append(Form("dPhiLt%1.0f", maxDPhi12_));
      else if ( maxDPhi12_ <= 0. ) dqmDirectory_full.Append(Form("dPhiGt%1.0f", minDPhi12_));
      else                         dqmDirectory_full.Append(Form("dPhi%1.0fto%1.0f", minDPhi12_, maxDPhi12_));
      if ( !dqmDirectory_full.EndsWith("/") ) dqmDirectory_full.Append("/");
      if ( isValidSolution_ > 0 ) dqmDirectory_full.Append("validSVfitSolution");
      if ( isValidSolution_ < 0 ) dqmDirectory_full.Append("invalidSVfitSolution");
      if ( !dqmDirectory_full.EndsWith("/") ) dqmDirectory_full.Append("/");
      if      ( minQt_ <= 0. && 
		maxQt_ <= 0. ) dqmDirectory_full.Append("");
      else if ( minQt_ <= 0. ) dqmDirectory_full.Append(Form("qTlt%1.0f", maxQt_));
      else if ( maxQt_ <= 0. ) dqmDirectory_full.Append(Form("qTgt%1.0f", minQt_));
      else                         dqmDirectory_full.Append(Form("qT%1.0fto%1.0f", minQt_, maxQt_));
      dqmDirectory_full.ReplaceAll(".", "_");
      dqmDirectory_ = dqmDirectory_full.Data();
    }
    ~plotEntryType() {}
    void bookHistograms(DQMStore& dqmStore)
    {
      dqmStore.setCurrentFolder(dqmDirectory_.data());
      
      leg1Pt_                       = dqmStore.book1D("leg1Pt",                       "leg1Pt",                       numBinsSVfitMass_/2,            0., 0.5*svFitMassMax_);
      leg2Pt_                       = dqmStore.book1D("leg2Pt",                       "leg2Pt",                       numBinsSVfitMass_/2,            0., 0.5*svFitMassMax_);
      dPhi12_                       = dqmStore.book1D("dPhi12",                       "dPhi12",                                       180,            0.,              180.);
      metPt_                        = dqmStore.book1D("metPt",                        "metPt",                        numBinsSVfitMass_/2,            0., 0.5*svFitMassMax_);
      visMass_                      = dqmStore.book1D("visMass",                      "visMass",                        numBinsSVfitMass_,            0.,     svFitMassMax_);
      genMass_                      = dqmStore.book1D("genMass",                      "genMass",                        numBinsSVfitMass_,            0.,     svFitMassMax_);
      genVisMass_                   = dqmStore.book1D("genVisMass",                   "genVisMass",                     numBinsSVfitMass_,            0.,     svFitMassMax_);
      
      genDiTauPt_                   = dqmStore.book1D("genDiTauPt",                   "genDiTauPt",                                   250,            0.,             +250.);
      genDiTauEta_                  = dqmStore.book1D("genDiTauEta",                  "genDiTauEta",                                  198,          -9.9,              +9.9);
      genDiTauPhi_                  = dqmStore.book1D("genDiTauPhi",                  "genDiTauPhi",                                  360,  -TMath::Pi(),      +TMath::Pi());
  
      recDiTauPt_                   = dqmStore.book1D("recDiTauPt",                   "recDiTauPt",                                   250,            0.,             +250.);
      recDiTauEta_                  = dqmStore.book1D("recDiTauEta",                  "recDiTauEta",                                  198,          -9.9,              +9.9);
      recDiTauPhi_                  = dqmStore.book1D("recDiTauPhi",                  "recDiTauPhi",                                  360,  -TMath::Pi(),      +TMath::Pi());
      
      deltaDiTauPt_                 = dqmStore.book1D("deltaDiTauPt",                 "deltaDiTauPt",                                 350,         -100.,             +250.);
      deltaDiTauPx_                 = dqmStore.book1D("deltaDiTauPx",                 "deltaDiTauPx",                                 350,         -175.,             +175.);
      deltaDiTauPy_                 = dqmStore.book1D("deltaDiTauPy",                 "deltaDiTauPy",                                 350,         -175.,             +175.);  
      deltaDiTauEta_                = dqmStore.book1D("deltaDiTauEta",                "deltaDiTauEta",                                198,          -9.9,              +9.9);
      deltaDiTauPhi_                = dqmStore.book1D("deltaDiTauPhi",                "deltaDiTauPhi",                                360,  -TMath::Pi(),      +TMath::Pi());
      deltaDiTauMass_               = dqmStore.book1D("deltaDiTauMass",               "deltaDiTauMass",               2*numBinsSVfitMass_, -svFitMassMax_,   +svFitMassMax_);
             
      recLeg12MEtPt_                = dqmStore.book1D("recLeg12MEtPt",                "recLeg12MEtPt",                                250,            0.,             +250.);  
      recLeg12MEtPhi_               = dqmStore.book1D("recLeg12MEtPhi",               "recLeg12MEtPhi",                               360,  -TMath::Pi(),      +TMath::Pi());

      deltaLeg12MEtPt_              = dqmStore.book1D("deltaLeg12MEtPt",              "deltaLeg12MEtPt",                              350,         -100.,             +250.);
      deltaLeg12MEtPx_              = dqmStore.book1D("deltaLeg12MEtPx",              "deltaLeg12MEtPx",                              350,         -175.,             +175.);
      deltaLeg12MEtPy_              = dqmStore.book1D("deltaLeg12MEtPy",              "deltaLeg12MEtPy",                              350,         -175.,             +175.);
      deltaLeg12MEtPhi_             = dqmStore.book1D("deltaLeg12MEtPhi",             "deltaLeg12MEtPhi",                             360,  -TMath::Pi(),      +TMath::Pi());
      
      deltaMEtPt_                   = dqmStore.book1D("deltaMEtPt",                   "deltaMEtPt",                                   350,         -100.,             +250.);
      deltaMEtPx_                   = dqmStore.book1D("deltaMEtPx",                   "deltaMEtPx",                                   350,         -175.,             +175.);
      deltaMEtPy_                   = dqmStore.book1D("deltaMEtPy",                   "deltaMEtPy",                                   350,         -175.,             +175.);
      deltaMEtPhi_                  = dqmStore.book1D("deltaMEtPhi",                  "deltaMEtPhi",                                  360,  -TMath::Pi(),      +TMath::Pi());
           
      svFitMass_                    = dqmStore.book1D("svFitMass",                    "svFitMass",                      numBinsSVfitMass_,            0.,     svFitMassMax_);
      svFitSigma_                   = dqmStore.book1D("svFitSigma",                   "svFitSigma",                    numBinsSVfitSigma_,            0.,    svFitSigmaMax_);
      svFitIsValidSolution_         = dqmStore.book1D("svFitIsValidSolution",         "svFitIsValidSolution",                           2,          -0.5,               1.5);

      svFitMass_mean_               = dqmStore.book1D("svFitMass_mean",               "svFitMass_mean",                 numBinsSVfitMass_,            0.,     svFitMassMax_);
      svFitMass_median_             = dqmStore.book1D("svFitMass_median",             "svFitMass_median",               numBinsSVfitMass_,            0.,     svFitMassMax_);
      svFitMass_maximum_            = dqmStore.book1D("svFitMass_maximum",            "svFitMass_maximum",              numBinsSVfitMass_,            0.,     svFitMassMax_);
      svFitMass_maxInterpol_        = dqmStore.book1D("svFitMass_maxInterpol",        "svFitMass_maxInterpol",          numBinsSVfitMass_,            0.,     svFitMassMax_);

      svFitMassVsSigma_             = dqmStore.book2D("svFitMassVsSigma",             "svFitMassVsSigma",                             100, 0.,    svFitSigmaMax_, 100, 0., svFitMassMax_);
      svFitMassVsSigma_mean_        = dqmStore.book2D("svFitMassVsSigma_mean",        "svFitMassVsSigma_mean",                        100, 0.,    svFitSigmaMax_, 100, 0., svFitMassMax_);
      svFitMassVsSigma_median_      = dqmStore.book2D("svFitMassVsSigma_median",      "svFitMassVsSigma_median",                      100, 0.,    svFitSigmaMax_, 100, 0., svFitMassMax_);
      svFitMassVsSigma_maximum_     = dqmStore.book2D("svFitMassVsSigma_maximum",     "svFitMassVsSigma_maximum",                     100, 0.,    svFitSigmaMax_, 100, 0., svFitMassMax_);
      svFitMassVsSigma_maxInterpol_ = dqmStore.book2D("svFitMassVsSigma_maxInterpol", "svFitMassVsSigma_maxInterpol",                 100, 0.,    svFitSigmaMax_, 100, 0., svFitMassMax_);

      svFitMassVsMEt_               = dqmStore.book2D("svFitMassVsMEt",               "svFitMassVsMEt",                               100, 0., 0.5*svFitMassMax_, 100, 0., svFitMassMax_);
      svFitSigmaVsMEt_              = dqmStore.book2D("svFitSigmaVsMEt",              "svFitSigmaVsMEt",                              100, 0., 0.5*svFitMassMax_, 100, 0., svFitSigmaMax_);

      svFitMass_oneProng0pi0_       = dqmStore.book1D("svFitMass_oneProng0pi0",       "svFitMass_oneProng0pi0",         numBinsSVfitMass_,            0.,     svFitMassMax_);
      svFitMass_oneProng1pi0_       = dqmStore.book1D("svFitMass_oneProng1pi0",       "svFitMass_oneProng1pi0",         numBinsSVfitMass_,            0.,     svFitMassMax_);
      svFitMass_oneProng2pi0_       = dqmStore.book1D("svFitMass_oneProng2pi0",       "svFitMass_oneProng2pi0",         numBinsSVfitMass_,            0.,     svFitMassMax_);
      svFitMass_threeProng0pi0_     = dqmStore.book1D("svFitMass_threeProng0pi0",     "svFitMass_threeProng0pi0",       numBinsSVfitMass_,            0.,     svFitMassMax_);

      svFitMassVsNLL_               = dqmStore.book2D("svFitMassVsNLL",               "svFitMassVsNLL",                               400, 0., 10., TMath::Nint(0.5*svFitMassMax_), 0., svFitMassMax_);

    }
    void fillHistograms(const reco::Candidate::LorentzVector& genDiTauP4, 
			const reco::Candidate::LorentzVector& genLeg1P4, const reco::Candidate::LorentzVector& genLeg2P4, 
			const reco::Candidate::LorentzVector& genMEtP4,
			const NSVfitResonanceHypothesisBase* svFitResonanceHypothesis, double svFit_nll,
			const reco::Candidate::LorentzVector& recLeg1P4, int recLeg1DecayMode,
			const reco::Candidate::LorentzVector& recLeg2P4, int recLeg2DecayMode,
			const reco::Candidate::LorentzVector& recMEtP4,
			double evtWeight)
    {
      bool svFitIsValidSolution = svFitResonanceHypothesis->isValidSolution();
      if ( isValidSolution_ < 0 &&  svFitIsValidSolution ) return;
      if ( isValidSolution_ > 0 && !svFitIsValidSolution ) return;

      double dPhi12 = TMath::ACos(TMath::Cos(recLeg1P4.phi() - recLeg2P4.phi()));
      dPhi12 *= TMath::RadToDeg();

      double qT = genDiTauP4.pt();
      
      if ( (dPhi12 > minDPhi12_ || minDPhi12_ <= 0.) &&
	   (dPhi12 < maxDPhi12_ || maxDPhi12_ <= 0.) &&
	   (qT     > minQt_     || minQt_     <= 0.) &&
	   (qT     < maxQt_     || maxQt_     <= 0.) ) {
	
	leg1Pt_->Fill(recLeg1P4.pt(), evtWeight);
	leg2Pt_->Fill(recLeg2P4.pt(), evtWeight);
    	dPhi12_->Fill(dPhi12, evtWeight);
	metPt_->Fill(recMEtP4.pt(), evtWeight);
	double visMass = (recLeg1P4 + recLeg2P4).mass();
	visMass_->Fill(visMass, evtWeight);
	genMass_->Fill(genDiTauP4.mass(), evtWeight);
	double genVisMass = (genLeg1P4 + genLeg2P4).mass();
	genVisMass_->Fill(genVisMass, evtWeight);

	//std::cout << "genDiTau: Pt = " << genDiTauP4.pt() << ", eta = " << genDiTauP4.eta() << ", phi = " << genDiTauP4.phi() << std::endl;
	genDiTauPt_->Fill(genDiTauP4.pt(), evtWeight);
	genDiTauEta_->Fill(genDiTauP4.eta(), evtWeight);
	genDiTauPhi_->Fill(genDiTauP4.phi(), evtWeight);

	bool svFitIsValidSolution = svFitResonanceHypothesis->isValidSolution();
	svFitIsValidSolution_->Fill(svFitIsValidSolution, evtWeight);

	double svFitMass      = svFitResonanceHypothesis->mass();
	double svFitSigmaUp   = svFitResonanceHypothesis->massErrUp();
	double svFitSigmaDown = svFitResonanceHypothesis->massErrDown();
	double svFitSigma     = TMath::Sqrt(0.5*(svFitSigmaUp*svFitSigmaUp + svFitSigmaDown*svFitSigmaDown));

	if ( dynamic_cast<const NSVfitResonanceHypothesis*>(svFitResonanceHypothesis) ) {
	  reco::Candidate::LorentzVector recDiTauP4 = (dynamic_cast<const NSVfitResonanceHypothesis*>(svFitResonanceHypothesis))->p4_fitted(); 

	  //std::cout << "recDiTau: Pt = " << recDiTauP4.pt() << ", eta = " << recDiTauP4.eta() << ", phi = " << recDiTauP4.phi() << std::endl;
	  recDiTauPt_->Fill(recDiTauP4.pt(), evtWeight);
	  recDiTauEta_->Fill(recDiTauP4.eta(), evtWeight);
	  recDiTauPhi_->Fill(recDiTauP4.phi(), evtWeight);
	  
	  deltaDiTauPt_->Fill(recDiTauP4.pt() - genDiTauP4.pt(), evtWeight);
	  deltaDiTauPx_->Fill(recDiTauP4.px() - genDiTauP4.px(), evtWeight);
	  deltaDiTauPy_->Fill(recDiTauP4.py() - genDiTauP4.py(), evtWeight);
	  deltaDiTauEta_->Fill(recDiTauP4.eta() - genDiTauP4.eta(), evtWeight);
	  deltaDiTauPhi_->Fill(normalizedPhi(recDiTauP4.phi() - genDiTauP4.phi()), evtWeight);
	  deltaDiTauMass_->Fill(recDiTauP4.mass() - genDiTauP4.mass(), evtWeight);
	  
	  if ( TMath::Abs(recDiTauP4.mass() - svFitMass) > (1.e-2*genDiTauP4.mass()) ) {
	    std::cerr << "Problem with large rounding errors:" << std::endl;
	    std::cerr << " svFitMass = " << svFitMass << std::endl;
	    std::cerr << " recDiTauP4: E = " << recDiTauP4.E() << ", eta = " << recDiTauP4.eta() << ", phi = " << recDiTauP4.phi() << ", mass = " << recDiTauP4.mass() << std::endl;
	  }
	}
	
	reco::Candidate::LorentzVector recLeg12MEtP4 = recLeg1P4 + recLeg2P4 + recMEtP4;
	//std::cout << "recLeg12MEt: Pt = " << recLeg12MEtP4.pt() << ", eta = " << recLeg12MEtP4.eta() << ", phi = " << recLeg12MEtP4.phi() << std::endl;
	recLeg12MEtPt_->Fill(recLeg12MEtP4.pt(), evtWeight);
	recLeg12MEtPhi_->Fill(recLeg12MEtP4.phi(), evtWeight);
	
	deltaLeg12MEtPt_->Fill(recLeg12MEtP4.pt() - genDiTauP4.pt(), evtWeight);
	deltaLeg12MEtPx_->Fill(recLeg12MEtP4.px() - genDiTauP4.px(), evtWeight);
	deltaLeg12MEtPy_->Fill(recLeg12MEtP4.py() - genDiTauP4.py(), evtWeight);
	deltaLeg12MEtPhi_->Fill(normalizedPhi(recLeg12MEtP4.phi() - genDiTauP4.phi()), evtWeight);
	
	deltaMEtPt_->Fill(recMEtP4.pt() - genMEtP4.pt(), evtWeight);
	deltaMEtPx_->Fill(recMEtP4.px() - genMEtP4.px(), evtWeight);
	deltaMEtPy_->Fill(recMEtP4.py() - genMEtP4.py(), evtWeight);
	deltaMEtPhi_->Fill(normalizedPhi(recMEtP4.phi() - genMEtP4.phi()), evtWeight);
	
	svFitMass_->Fill(svFitMass, evtWeight);
	svFitSigma_->Fill(svFitSigma, evtWeight);
	svFitMassVsSigma_->Fill(svFitSigma, svFitMass, evtWeight);

	if ( dynamic_cast<const NSVfitResonanceHypothesisByIntegration*>(svFitResonanceHypothesis) ) {
	  double svFitMass_mean        = (dynamic_cast<const NSVfitResonanceHypothesisByIntegration*>(svFitResonanceHypothesis))->mass_mean();
	  double svFitMass_median      = (dynamic_cast<const NSVfitResonanceHypothesisByIntegration*>(svFitResonanceHypothesis))->mass_median();
	  double svFitMass_maximum     = (dynamic_cast<const NSVfitResonanceHypothesisByIntegration*>(svFitResonanceHypothesis))->mass_maximum();
	  double svFitMass_maxInterpol = (dynamic_cast<const NSVfitResonanceHypothesisByIntegration*>(svFitResonanceHypothesis))->mass_maxInterpol();

	  svFitMassVsSigma_mean_->Fill(svFitSigma, svFitMass_mean, evtWeight);
	  svFitMassVsSigma_median_->Fill(svFitSigma, svFitMass_median, evtWeight);
	  svFitMassVsSigma_maximum_->Fill(svFitSigma, svFitMass_maximum, evtWeight);
	  svFitMassVsSigma_maxInterpol_->Fill(svFitSigma, svFitMass_maxInterpol, evtWeight);
	}

	svFitMassVsMEt_->Fill(recMEtP4.pt(), svFitMass, evtWeight);
	svFitSigmaVsMEt_->Fill(recMEtP4.pt(), svFitSigma, evtWeight);
	
	if ( recLeg2DecayMode == reco::PFTau::kOneProng0PiZero ) {
	  svFitMass_oneProng0pi0_->Fill(svFitMass, evtWeight);
	} else if ( recLeg2DecayMode == reco::PFTau::kOneProng1PiZero ) {	
	  svFitMass_oneProng1pi0_->Fill(svFitMass, evtWeight);
	} else if ( recLeg2DecayMode == reco::PFTau::kOneProng2PiZero ) {
	  svFitMass_oneProng2pi0_->Fill(svFitMass, evtWeight);
	} else if ( recLeg2DecayMode == reco::PFTau::kThreeProng0PiZero ) {	
	  svFitMass_threeProng0pi0_->Fill(svFitMass, evtWeight);
	}
      }
    }
    void finalizeHistograms()
    {}

    std::string dqmDirectory_;

    double minDPhi12_;
    double maxDPhi12_;
    int isValidSolution_;
    double minQt_;
    double maxQt_;

    int numBinsSVfitMass_;
    double svFitMassMax_;
    int numBinsSVfitSigma_;
    double svFitSigmaMax_;
    
    MonitorElement* leg1Pt_;
    MonitorElement* leg2Pt_;
    MonitorElement* dPhi12_;
    MonitorElement* metPt_;
    MonitorElement* visMass_;
    MonitorElement* genMass_;
    MonitorElement* genVisMass_;

    MonitorElement* genDiTauPt_;
    MonitorElement* genDiTauEta_;
    MonitorElement* genDiTauPhi_;

    MonitorElement* recDiTauPt_;
    MonitorElement* recDiTauEta_;
    MonitorElement* recDiTauPhi_;
    
    MonitorElement* recLeg12MEtPt_;
    MonitorElement* recLeg12MEtPhi_;
    
    MonitorElement* deltaDiTauPt_;
    MonitorElement* deltaDiTauPx_;
    MonitorElement* deltaDiTauPy_;
    MonitorElement* deltaDiTauEta_;
    MonitorElement* deltaDiTauPhi_;
    MonitorElement* deltaDiTauMass_;
    
    MonitorElement* deltaLeg12MEtPt_;
    MonitorElement* deltaLeg12MEtPx_;
    MonitorElement* deltaLeg12MEtPy_;
    MonitorElement* deltaLeg12MEtPhi_;
    
    MonitorElement* deltaMEtPt_;
    MonitorElement* deltaMEtPx_;
    MonitorElement* deltaMEtPy_;
    MonitorElement* deltaMEtPhi_;

    MonitorElement* svFitMass_;
    MonitorElement* svFitSigma_;
    MonitorElement* svFitIsValidSolution_;

    MonitorElement* svFitMass_mean_;
    MonitorElement* svFitMass_median_;
    MonitorElement* svFitMass_maximum_;
    MonitorElement* svFitMass_maxInterpol_;

    MonitorElement* svFitMassVsSigma_;
    MonitorElement* svFitMassVsSigma_mean_;
    MonitorElement* svFitMassVsSigma_median_;
    MonitorElement* svFitMassVsSigma_maximum_;
    MonitorElement* svFitMassVsSigma_maxInterpol_;

    MonitorElement* svFitMassVsMEt_;
    MonitorElement* svFitSigmaVsMEt_;

    MonitorElement* svFitMass_oneProng0pi0_;
    MonitorElement* svFitMass_oneProng1pi0_;
    MonitorElement* svFitMass_oneProng2pi0_;
    MonitorElement* svFitMass_threeProng0pi0_;    
    
    MonitorElement* svFitMassVsNLL_;
  };

  std::vector<plotEntryType*> plotEntries_;
  
  long numEvents_processed_;
  double numEventsWeighted_processed_;
};

#endif   
