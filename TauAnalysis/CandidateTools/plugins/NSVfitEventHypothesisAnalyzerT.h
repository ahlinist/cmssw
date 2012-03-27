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

#include "DataFormats/Candidate/interface/Candidate.h"

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

  MonitorElement* svFitIsValidSolution_;
  
  struct plotEntryType1
  {
    plotEntryType1(const std::string& dqmDirectory, double minDPhi12, double maxDPhi12, int isValidSolution, 
		   int numBinsSVfitMass, double svFitMassMax,
		   int numBinsSVfitSigma, double svFitSigmaMax)
      : minDPhi12_(minDPhi12),
	maxDPhi12_(maxDPhi12),
	isValidSolution_(isValidSolution),
	numBinsSVfitMass_(numBinsSVfitMass), 
	svFitMassMax_(svFitMassMax),
	numBinsSVfitSigma_(numBinsSVfitSigma), 
	svFitSigmaMax_(svFitSigmaMax)
    {
      TString dqmDirectory_full = dqmDirectory.data();
      if ( !dqmDirectory_full.EndsWith("/") ) dqmDirectory_full.Append("/");
      if      ( minDPhi12_ <= 0. && 
		maxDPhi12_ <= 0. ) dqmDirectory_full.Append("");
      else if ( minDPhi12_ <= 0. ) dqmDirectory_full.Append(Form("dPhiLt%1.0f", maxDPhi12_));
      else if ( maxDPhi12_ <= 0. ) dqmDirectory_full.Append(Form("dPhiGt%1.0f", minDPhi12_));
      else                         dqmDirectory_full.Append(Form("dPhi%1.0fto%1.0f", minDPhi12_, maxDPhi12_));
      dqmDirectory_full.ReplaceAll(".", "_");
      if ( !dqmDirectory_full.EndsWith("/") ) dqmDirectory_full.Append("/");
      if ( isValidSolution_ > 0 ) dqmDirectory_full.Append("validSVfitSolution");
      if ( isValidSolution_ < 0 ) dqmDirectory_full.Append("invalidSVfitSolution");
      dqmDirectory_ = dqmDirectory_full.Data();
    }
    ~plotEntryType1() {}
    void bookHistograms(DQMStore& dqmStore)
    {
      dqmStore.setCurrentFolder(dqmDirectory_.data());
      
      svFitMass_             = dqmStore.book1D("svFitMass",             "svFitMass",             numBinsSVfitMass_, 0., svFitMassMax_);
      svFitMass_mean_        = dqmStore.book1D("svFitMass_mean",        "svFitMass_mean",        numBinsSVfitMass_, 0., svFitMassMax_);
      svFitMass_median_      = dqmStore.book1D("svFitMass_median",      "svFitMass_median",      numBinsSVfitMass_, 0., svFitMassMax_);
      svFitMass_maximum_     = dqmStore.book1D("svFitMass_maximum",     "svFitMass_maximum",     numBinsSVfitMass_, 0., svFitMassMax_);
      svFitMass_maxInterpol_ = dqmStore.book1D("svFitMass_maxInterpol", "svFitMass_maxInterpol", numBinsSVfitMass_, 0., svFitMassMax_);
      
      svFitSigma_ = dqmStore.book1D("svFitSigma", "svFitSigma", numBinsSVfitSigma_, 0., svFitSigmaMax_);

      svFitMassVsSigma_ = 
	dqmStore.book2D("svFitMassVsSigma",        
			"svFitMassVsSigma", 100, 0., svFitSigmaMax_, 100, 0., svFitMassMax_);
      svFitMassVsSigma_mean_ = 
	dqmStore.book2D("svFitMassVsSigma_mean",        
			"svFitMassVsSigma_mean", 100, 0., svFitSigmaMax_, 100, 0., svFitMassMax_);
      svFitMassVsSigma_median_ = 
	dqmStore.book2D("svFitMassVsSigma_median",      
			"svFitMassVsSigma_median", 100, 0., svFitSigmaMax_, 100, 0., svFitMassMax_);
      svFitMassVsSigma_maximum_ = 
	dqmStore.book2D("svFitMassVsSigma_maximum",     
			"svFitMassVsSigma_maximum", 100, 0., svFitSigmaMax_, 100, 0., svFitMassMax_);
      svFitMassVsSigma_maxInterpol_ = 
	dqmStore.book2D("svFitMassVsSigma_maxInterpol", 
			"svFitMassVsSigma_maxInterpol", 100, 0., svFitSigmaMax_, 100, 0., svFitMassMax_);
    }      
    void fillHistograms(bool isValidSolution,
			const reco::Candidate::LorentzVector& recLeg1P4, const reco::Candidate::LorentzVector& recLeg2P4,  
			double svFitMass,
			double svFitMass_mean, double svFitMass_median, double svFitMass_maximum, double svFitMass_maxInterpol,
			double svFitSigma,
			double evtWeight)
    {
      if ( isValidSolution_ < 0 &&  isValidSolution ) return;
      if ( isValidSolution_ > 0 && !isValidSolution ) return;

      double dPhi12 = TMath::ACos(TMath::Cos(recLeg1P4.phi() - recLeg2P4.phi()));
      dPhi12 *= TMath::RadToDeg();
      
      if ( (dPhi12 > minDPhi12_ || minDPhi12_ <= 0.) &&
	   (dPhi12 < maxDPhi12_ || maxDPhi12_ <= 0.) ) {

	svFitMass_->Fill(svFitMass, evtWeight);
	svFitMass_mean_->Fill(svFitMass_mean, evtWeight);
	svFitMass_median_->Fill(svFitMass_median, evtWeight);
	svFitMass_maximum_->Fill(svFitMass_maximum, evtWeight);
	svFitMass_maxInterpol_->Fill(svFitMass_maxInterpol, evtWeight);

	svFitSigma_->Fill(svFitSigma, evtWeight);

	svFitMassVsSigma_->Fill(svFitSigma, svFitMass, evtWeight);
	svFitMassVsSigma_mean_->Fill(svFitSigma, svFitMass_mean, evtWeight);
	svFitMassVsSigma_median_->Fill(svFitSigma, svFitMass_median, evtWeight);
	svFitMassVsSigma_maximum_->Fill(svFitSigma, svFitMass_maximum, evtWeight);
	svFitMassVsSigma_maxInterpol_->Fill(svFitSigma, svFitMass_maxInterpol, evtWeight);
      }
    }

    void finalizeHistograms()
    {}

    std::string dqmDirectory_;

    double minDPhi12_;
    double maxDPhi12_;
    int isValidSolution_;

    int numBinsSVfitMass_;
    double svFitMassMax_;
    int numBinsSVfitSigma_;
    double svFitSigmaMax_;

    MonitorElement* svFitMass_;
    MonitorElement* svFitMass_mean_;
    MonitorElement* svFitMass_median_;
    MonitorElement* svFitMass_maximum_;
    MonitorElement* svFitMass_maxInterpol_;

    MonitorElement* svFitSigma_;

    MonitorElement* svFitMassVsSigma_;
    MonitorElement* svFitMassVsSigma_mean_;
    MonitorElement* svFitMassVsSigma_median_;
    MonitorElement* svFitMassVsSigma_maximum_;
    MonitorElement* svFitMassVsSigma_maxInterpol_;
  };

  struct plotEntryType2
  {
    plotEntryType2(const std::string& dqmDirectory, double minSVfitSigma, double maxSVfitSigma, int isValidSolution,
		   int numBinsSVfitMass, double svFitMassMax,
		   int numBinsSVfitSigma, double svFitSigmaMax)
      : minSVfitSigma_(minSVfitSigma),
	maxSVfitSigma_(maxSVfitSigma),
	isValidSolution_(isValidSolution),
	numBinsSVfitMass_(numBinsSVfitMass), 
	svFitMassMax_(svFitMassMax),
	numBinsSVfitSigma_(numBinsSVfitSigma), 
	svFitSigmaMax_(svFitSigmaMax)
    {
      TString dqmDirectory_full = dqmDirectory.data();
      if ( !dqmDirectory_full.EndsWith("/") ) dqmDirectory_full.Append("/");
      if      ( minSVfitSigma_ <= 0. && 
		maxSVfitSigma_ <= 0. ) dqmDirectory_full.Append("");
      else if ( minSVfitSigma_ <= 0. ) dqmDirectory_full.Append(Form("sigmaSVfitLt%1.0f", maxSVfitSigma_));
      else if ( maxSVfitSigma_ <= 0. ) dqmDirectory_full.Append(Form("sigmaSVfitGt%1.0f", minSVfitSigma_));
      else                             dqmDirectory_full.Append(Form("sigmaSVfit%1.0fto%1.0f", minSVfitSigma_, maxSVfitSigma_));
      dqmDirectory_full.ReplaceAll(".", "_");
      if ( !dqmDirectory_full.EndsWith("/") ) dqmDirectory_full.Append("/");
      if ( isValidSolution_ > 0 ) dqmDirectory_full.Append("validSVfitSolution");
      if ( isValidSolution_ < 0 ) dqmDirectory_full.Append("invalidSVfitSolution");
      dqmDirectory_ = dqmDirectory_full.Data();
    }
    ~plotEntryType2() {}
    void bookHistograms(DQMStore& dqmStore)
    {
      dqmStore.setCurrentFolder(dqmDirectory_.data());

      leg1Pt_         = dqmStore.book1D("leg1Pt",         "leg1Pt",         numBinsSVfitMass_/2, 0., 0.5*svFitMassMax_);
      leg2Pt_         = dqmStore.book1D("leg2Pt",         "leg2Pt",         numBinsSVfitMass_/2, 0., 0.5*svFitMassMax_);
      visMass_        = dqmStore.book1D("visMass",        "visMass",        numBinsSVfitMass_, 0., svFitMassMax_);
      dPhi12_         = dqmStore.book1D("dPhi12",         "dPhi12",         180, 0., TMath::Pi());
      diTauPt_        = dqmStore.book1D("diTauPt",        "diTauPt",        250, 0., 250.);
      svFitMass_      = dqmStore.book1D("svFitMass",      "svFitMass",      numBinsSVfitMass_, 0., svFitMassMax_);
      svFitSigma_     = dqmStore.book1D("svFitSigma",     "svFitSigma",     numBinsSVfitSigma_, 0., svFitSigmaMax_);     
      svFitSigmaUp_   = dqmStore.book1D("svFitSigmaUp",   "svFitSigmaUp",   numBinsSVfitSigma_, 0., svFitSigmaMax_);     
      svFitSigmaDown_ = dqmStore.book1D("svFitSigmaDown", "svFitSigmaDown", numBinsSVfitSigma_, 0., svFitSigmaMax_);     
 
      svFitMassVsLeg1Pt_ = 
	dqmStore.book2D("svFitMassVsLeg1Pt",  
			"svFitMassVsLeg1Pt", 100, 0., 0.5*svFitMassMax_, 100, 0., svFitMassMax_);
      svFitMassVsLeg2Pt_ = 
	dqmStore.book2D("svFitMassVsLeg2Pt",  
			"svFitMassVsLeg2Pt", 100, 0., 0.5*svFitMassMax_, 100, 0., svFitMassMax_);
      svFitMassVsVisMass_ = 
	dqmStore.book2D("svFitMassVsVisMass", 
			"svFitMassVsVisMass", 100, 0., svFitMassMax_, 100, 0., svFitMassMax_);
      svFitMassVsDPhi12_  = 
	dqmStore.book2D("svFitMassVsDPhi12",  
			"svFitMassVsDPhi12", 36, 0., TMath::Pi(), 100, 0., svFitMassMax_);
      svFitMassVsDiTauPt_ = 
	dqmStore.book2D("svFitMassVsDiTauPt", 
			"svFitMassVsDiTauPt", 50, 0., 150., 100, 0., svFitMassMax_);

      svFitMassVsProdAngle_rf_ = 
	dqmStore.book2D("svFitMassVsProdAngle_rf",  
			"svFitMassVsProdAngle_rf", 36, 0., TMath::Pi(), 100, 0., svFitMassMax_);
      svFitSigmaVsProdAngle_rf_ = 
	dqmStore.book2D("svFitSigmaVsProdAngle_rf", 
			"svFitSigmaVsProdAngle_rf", 36, 0., TMath::Pi(), 100, 0., svFitSigmaMax_);

      svFitMassVsSigma_ = 
	dqmStore.book2D("svFitMassVsSigma",             
			"svFitMassVsSigma", 100, 0., svFitSigmaMax_, 100, 0., svFitMassMax_);
      svFitMassVsSigmaUp_ = 
	dqmStore.book2D("svFitMassVsSigmaUp",             
			"svFitMassVsSigmaUp", 100, 0., svFitSigmaMax_, 100, 0., svFitMassMax_);
      svFitMassVsSigmaDown_ = 
	dqmStore.book2D("svFitMassVsSigmaDown",             
			"svFitMassVsSigmaDown", 100, 0., svFitSigmaMax_, 100, 0., svFitMassMax_);
      
      metErrPx_ = dqmStore.book1D("metErrPx", "metErrPx", 200, -100., +100.);
      metErrPy_ = dqmStore.book1D("metErrPy", "metErrPy", 200, -100., +100.);
      metErrPt_ = dqmStore.book1D("metErrPt", "metErrPt", 250,    0.,  250.);
      metCov_   = dqmStore.book1D("metCov",   "metCov",   250,    0.,  250.);
      metPull_  = dqmStore.book1D("metPull",  "metPull",  250,    0.,   25.);
    }
    void fillHistograms(bool isValidSolution,
			const reco::Candidate::LorentzVector& genLeg1P4, const reco::Candidate::LorentzVector& recLeg1P4, 
			const reco::Candidate::LorentzVector& genLeg2P4, const reco::Candidate::LorentzVector& recLeg2P4, 
			double svFitMass, double svFitSigma, double svFitSigmaUp, double svFitSigmaDown, 
			double diTauPt, double prodAngle_rf,
			double metCov, const reco::Candidate::LorentzVector& rec_minus_genMEtP4, double metPull, 
			double evtWeight)
    {
      if ( isValidSolution_ < 0 &&  isValidSolution ) return;
      if ( isValidSolution_ > 0 && !isValidSolution ) return;

      if ( (svFitSigma > minSVfitSigma_ || minSVfitSigma_ <= 0.) &&
	   (svFitSigma < maxSVfitSigma_ || maxSVfitSigma_ <= 0.) ) {
	double genLeg1Pt = genLeg1P4.pt();
	double recLeg1Pt = recLeg1P4.pt();
	double genLeg2Pt = genLeg2P4.pt();
	double recLeg2Pt = recLeg2P4.pt();

	double visMass = (recLeg1P4 + recLeg2P4).mass();
	
	double dPhi12 = TMath::ACos(TMath::Cos(recLeg1P4.phi() - recLeg2P4.phi()));
	
	double metErr = rec_minus_genMEtP4.pt();

	leg1Pt_->Fill(recLeg1Pt, evtWeight);
	leg2Pt_->Fill(recLeg2Pt, evtWeight);
	visMass_->Fill(visMass, evtWeight);
	dPhi12_->Fill(dPhi12, evtWeight);
	diTauPt_->Fill(diTauPt, evtWeight);
	svFitMass_->Fill(svFitMass, evtWeight);
	svFitSigma_->Fill(svFitSigma, evtWeight);
	svFitSigmaUp_->Fill(svFitSigmaUp, evtWeight);
	svFitSigmaDown_->Fill(svFitSigmaDown, evtWeight);
	
	svFitMassVsLeg1Pt_->Fill(recLeg1Pt, svFitMass, evtWeight);
	svFitMassVsLeg2Pt_->Fill(recLeg2Pt, svFitMass, evtWeight);
	svFitMassVsVisMass_->Fill(visMass, svFitMass, evtWeight);
	svFitMassVsDPhi12_->Fill(dPhi12, svFitMass, evtWeight);      
	svFitMassVsDiTauPt_->Fill(diTauPt, svFitMass, evtWeight);

	svFitMassVsProdAngle_rf_->Fill(prodAngle_rf, svFitMass, evtWeight);
	svFitSigmaVsProdAngle_rf_->Fill(prodAngle_rf, svFitSigma, evtWeight);
	
	svFitMassVsSigma_->Fill(svFitSigma, svFitMass, evtWeight);   
	svFitMassVsSigmaUp_->Fill(svFitSigmaUp, svFitMass, evtWeight);   
	svFitMassVsSigmaDown_->Fill(svFitSigmaDown, svFitMass, evtWeight);   
	
	metErrPx_->Fill(rec_minus_genMEtP4.px(), evtWeight);
	metErrPy_->Fill(rec_minus_genMEtP4.py(), evtWeight);
	metErrPt_->Fill(metErr, evtWeight);
	metCov_->Fill(metCov, evtWeight);
	metPull_->Fill(metPull, evtWeight);
      }
    }

    void finalizeHistograms()
    {}

    std::string dqmDirectory_;

    double minSVfitSigma_;
    double maxSVfitSigma_;
    int isValidSolution_;

    int numBinsSVfitMass_;
    double svFitMassMax_;
    int numBinsSVfitSigma_;
    double svFitSigmaMax_;

    MonitorElement* leg1Pt_;
    MonitorElement* leg2Pt_;
    MonitorElement* visMass_;
    MonitorElement* dPhi12_;
    MonitorElement* diTauPt_;
    MonitorElement* svFitMass_;
    MonitorElement* svFitSigma_;
    MonitorElement* svFitSigmaUp_;
    MonitorElement* svFitSigmaDown_;

    MonitorElement* svFitMassVsLeg1Pt_;
    MonitorElement* svFitMassVsLeg2Pt_;
    MonitorElement* svFitMassVsVisMass_;
    MonitorElement* svFitMassVsDPhi12_;
    MonitorElement* svFitMassVsDiTauPt_;

    MonitorElement* svFitMassVsLeg1PtErr_;
    MonitorElement* svFitSigmaVsLeg1PtErr_;
    MonitorElement* svFitMassVsLeg2PtErr_;
    MonitorElement* svFitSigmaVsLeg2PtErr_;
    
    MonitorElement* svFitMassVsProdAngle_rf_;
    MonitorElement* svFitSigmaVsProdAngle_rf_;

    MonitorElement* svFitMassVsSigma_;
    MonitorElement* svFitMassVsSigmaUp_;
    MonitorElement* svFitMassVsSigmaDown_;

    MonitorElement* metErrPx_;
    MonitorElement* metErrPy_;
    MonitorElement* metErrPt_;
    MonitorElement* metCov_;
    MonitorElement* metPull_;
  };

  std::vector<plotEntryType1*> plotEntries1_;
  std::vector<plotEntryType2*> plotEntries2_;

  long numEvents_processed_;
  double numEventsWeighted_processed_;
};

#endif   
