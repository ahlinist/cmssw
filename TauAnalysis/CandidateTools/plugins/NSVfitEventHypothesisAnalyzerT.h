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

  std::string dqmDirectory_;

  MonitorElement* svFitIsValidSolution_;
  
  struct plotEntryType1
  {
    plotEntryType1(const std::string& dqmDirectory, double minDPhi12, double maxDPhi12, int isValidSolution)
      : minDPhi12_(minDPhi12),
	maxDPhi12_(maxDPhi12),
	isValidSolution_(isValidSolution)
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

      svFitMass_mean_        = dqmStore.book1D("svFitMass_mean",        "svFitMass_mean",        250, 0., 250.);
      svFitMass_median_      = dqmStore.book1D("svFitMass_median",      "svFitMass_median",      250, 0., 250.);
      svFitMass_maximum_     = dqmStore.book1D("svFitMass_maximum",     "svFitMass_maximum",     250, 0., 250.);
      svFitMass_maxInterpol_ = dqmStore.book1D("svFitMass_maxInterpol", "svFitMass_maxInterpol", 250, 0., 250.);
      
      svFitMassVsSigma_mean_ = 
	dqmStore.book2D("svFitMassVsSigma_mean",        
			"svFitMassVsSigma_mean", 50, 0.,  50., 50, 0., 250.);
      svFitMassVsSigma_median_ = 
	dqmStore.book2D("svFitMassVsSigma_median",      
			"svFitMassVsSigma_median", 50, 0.,  50., 50, 0., 250.);
      svFitMassVsSigma_maximum_ = 
	dqmStore.book2D("svFitMassVsSigma_maximum",     
			"svFitMassVsSigma_maximum", 50, 0.,  50., 50, 0., 250.);
      svFitMassVsSigma_maxInterpol_ = 
	dqmStore.book2D("svFitMassVsSigma_maxInterpol", 
			"svFitMassVsSigma_maxInterpol", 50, 0.,  50., 50, 0., 250.);
    }      
    void fillHistograms(bool isValidSolution,
			const reco::Candidate::LorentzVector& recLeg1P4, const reco::Candidate::LorentzVector& recLeg2P4,  
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

	svFitMass_mean_ ->Fill(svFitMass_mean, evtWeight);
	svFitMass_median_->Fill(svFitMass_median, evtWeight);
	svFitMass_maximum_->Fill(svFitMass_maximum, evtWeight);
	svFitMass_maxInterpol_->Fill(svFitMass_maxInterpol, evtWeight);

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

    MonitorElement* svFitMass_mean_;
    MonitorElement* svFitMass_median_;
    MonitorElement* svFitMass_maximum_;
    MonitorElement* svFitMass_maxInterpol_;

    MonitorElement* svFitMassVsSigma_mean_;
    MonitorElement* svFitMassVsSigma_median_;
    MonitorElement* svFitMassVsSigma_maximum_;
    MonitorElement* svFitMassVsSigma_maxInterpol_;
  };

  struct plotEntryType2
  {
    plotEntryType2(const std::string& dqmDirectory, double minSVfitSigma, double maxSVfitSigma, int isValidSolution)
      : minSVfitSigma_(minSVfitSigma),
	maxSVfitSigma_(maxSVfitSigma),
	isValidSolution_(isValidSolution)
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

      leg1Pt_     = dqmStore.book1D("leg1Pt",     "leg1Pt",     250, 0., 250.);
      leg2Pt_     = dqmStore.book1D("leg2Pt",     "leg2Pt",     250, 0., 250.);
      visMass_    = dqmStore.book1D("visMass",    "visMass",    250, 0., 250.);
      dPhi12_     = dqmStore.book1D("dPhi12",     "dPhi12",     180, 0., TMath::Pi());
      diTauPt_    = dqmStore.book1D("diTauPt",    "diTauPt",    250, 0., 250.);
      svFitMass_  = dqmStore.book1D("svFitMass",  "svFitMass",  250, 0., 250.);
      svFitSigma_ = dqmStore.book1D("svFitSigma", "svFitSigma", 250, 0., 250.);     

      svFitMassVsLeg1Pt_  = dqmStore.book2D("svFitMassVsLeg1Pt",  "svFitMassVsLeg1Pt",  150, 0., 150., 50, 0., 250.);
      svFitMassVsLeg2Pt_  = dqmStore.book2D("svFitMassVsLeg2Pt",  "svFitMassVsLeg2Pt",  150, 0., 150., 50, 0., 250.);
      svFitMassVsVisMass_ = dqmStore.book2D("svFitMassVsVisMass", "svFitMassVsVisMass",  50, 0., 250., 50, 0., 250.);
      svFitMassVsDPhi12_  = dqmStore.book2D("svFitMassVsDPhi12",  "svFitMassVsDPhi12",   36, 0., TMath::Pi(), 50, 0., 250.);
      svFitMassVsDiTauPt_ = dqmStore.book2D("svFitMassVsDiTauPt", "svFitMassVsDiTauPt", 150, 0., 150., 50, 0., 250.);

      svFitMassVsLeg1PtErr_  = dqmStore.book2D("svFitMassVsLeg1PtErr",  "svFitMassVsLeg1PtErr",  100, -25., +25., 50, 0., 250.);
      svFitSigmaVsLeg1PtErr_ = dqmStore.book2D("svFitSigmaVsLeg1PtErr", "svFitSigmaVsLeg1PtErr", 100, -25., +25., 50, 0.,  50.);
      svFitMassVsLeg2PtErr_  = dqmStore.book2D("svFitMassVsLeg2PtErr",  "svFitMassVsLeg2PtErr",  100, -25., +25., 50, 0., 250.);
      svFitSigmaVsLeg2PtErr_ = dqmStore.book2D("svFitSigmaVsLeg2PtErr", "svFitSigmaVsLeg2PtErr", 100, -25., +25., 50, 0.,  50.);

      svFitMassVsProdAngle_rf_ = 
	dqmStore.book2D("svFitMassVsProdAngle_rf",  
			"svFitMassVsProdAngle_rf", 36, 0., TMath::Pi(), 150, 0., 150.);
      svFitSigmaVsProdAngle_rf_ = 
	dqmStore.book2D("svFitSigmaVsProdAngle_rf", 
			"svFitSigmaVsProdAngle_rf", 36, 0., TMath::Pi(), 50, 0., 50.);

      svFitMassVsSigma_ = 
	dqmStore.book2D("svFitMassVsSigma",             
			"svFitMassVsSigma", 50, 0.,  50., 50, 0., 250.);
      
      metErrPx_ = dqmStore.book1D("metErrPx", "metErrPx", 200, -100., +100.);
      metErrPy_ = dqmStore.book1D("metErrPy", "metErrPy", 200, -100., +100.);
      metErrPt_ = dqmStore.book1D("metErrPt", "metErrPt", 250,    0.,  250.);
      metCov_   = dqmStore.book1D("metCov",   "metCov",   250,    0.,  250.);
      metPull_  = dqmStore.book1D("metPull",  "metPull",  250,    0.,   25.);

      svFitMassVsMEtCov_ = 
	dqmStore.book2D("svFitMassVsMEtCov", 
			"svFitMassVsMEtCov", 50, 0., 50., 50, 0., 250.);
      svFitSigmaVsMEtCov_ = 
	dqmStore.book2D("svFitSigmaVsMEtCov", 
			"svFitSigmaVsMEtCov", 50, 0., 50., 50, 0., 50.);      
      svFitMassVsMEtErr_ = 
	dqmStore.book2D("svFitMassVsMEtErr", 
			"svFitMassVsMEtErr", 40, 0., 100., 50, 0., 250.);
      svFitSigmaVsMEtErr_ = 
	dqmStore.book2D("svFitSigmaVsMEtErr", 
			"svFitSigmaVsMEtErr", 40, 0., 100., 50, 0., 50.);
      svFitMassVsMEtPull_ = 
	dqmStore.book2D("svFitMassVsMEtPull", 
			"svFitMassVsMEtPull", 100, 0., +10., 50, 0., 250.);
      svFitSigmaVsMEtPull_ = 
	dqmStore.book2D("svFitSigmaVsMEtPull", 
			"svFitSigmaVsMEtPull", 100, 0., +10., 50, 0., 50.);
      svFitMassVsMEtErrProjLeg1_ = 
	dqmStore.book2D("svFitMassVsMEtErrProjLeg1", 
			"svFitMassVsMEtErrProjLeg1", 100, -50., 50., 50, 0., 250.);
      svFitSigmaVsMEtErrProjLeg1_ = 
	dqmStore.book2D("svFitSigmaVsMEtErrProjLeg1", 
			"svFitSigmaVsMEtErrProjLeg1", 100, -50., 50., 50, 0., 50.);
      svFitMassVsMEtPullProjLeg1_ = 
	dqmStore.book2D("svFitMassVsMEtPullProjLeg1", 
			"svFitMassVsMEtPullProjLeg1", 100, 0., 10., 50, 0., 250.);
      svFitSigmaVsMEtPullProjLeg1_ = 
	dqmStore.book2D("svFitSigmaVsMEtPullProjLeg1", 
			"svFitSigmaVsMEtPullProjLeg1", 100, 0., 10., 50, 0., 50.);
      svFitMassVsMEtErrProjLeg2_ = 
	dqmStore.book2D("svFitMassVsMEtErrProjLeg2", 
			"svFitMassVsMEtErrProjLeg2", 100, -50., 50., 50, 0., 250.);
      svFitSigmaVsMEtErrProjLeg2_ = 
	dqmStore.book2D("svFitSigmaVsMEtErrProjLeg2", 
			"svFitSigmaVsMEtErrProjLeg2", 100, -50., 50., 50, 0., 50.);
      svFitMassVsMEtPullProjLeg2_ = 
	dqmStore.book2D("svFitMassVsMEtPullProjLeg2", 
			"svFitMassVsMEtPullProjLeg2", 100, 0., 10., 50, 0., 250.);
      svFitSigmaVsMEtPullProjLeg2_ = 
	dqmStore.book2D("svFitSigmaVsMEtPullProjLeg2", 
			"svFitSigmaVsMEtPullProjLeg2", 100, 0., 10., 50, 0., 50.);
    }
    void fillHistograms(bool isValidSolution,
			const reco::Candidate::LorentzVector& genLeg1P4, const reco::Candidate::LorentzVector& recLeg1P4, 
			const reco::Candidate::LorentzVector& genLeg2P4, const reco::Candidate::LorentzVector& recLeg2P4, 
			double svFitMass, double svFitSigma, double diTauPt, double prodAngle_rf,
			double metCov, const reco::Candidate::LorentzVector& rec_minus_genMEtP4, double metPull, 
			double metErrProjLeg1, double metPullProjLeg1, double metErrProjLeg2, double metPullProjLeg2, 
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
	
	svFitMassVsLeg1PtErr_->Fill(recLeg1Pt - genLeg1Pt, svFitMass, evtWeight);
	svFitSigmaVsLeg1PtErr_->Fill(recLeg1Pt - genLeg1Pt, svFitSigma, evtWeight);
	svFitMassVsLeg2PtErr_->Fill(recLeg2Pt - genLeg2Pt, svFitMass, evtWeight);
	svFitSigmaVsLeg2PtErr_->Fill(recLeg2Pt - genLeg2Pt, svFitSigma, evtWeight);

	svFitMassVsLeg1Pt_->Fill(recLeg1Pt, svFitMass, evtWeight);
	svFitMassVsLeg2Pt_->Fill(recLeg2Pt, svFitMass, evtWeight);
	svFitMassVsVisMass_->Fill(visMass, svFitMass, evtWeight);
	svFitMassVsDPhi12_->Fill(dPhi12, svFitMass, evtWeight);      
	svFitMassVsDiTauPt_->Fill(diTauPt, svFitMass, evtWeight);

	svFitMassVsProdAngle_rf_->Fill(prodAngle_rf, svFitMass, evtWeight);
	svFitSigmaVsProdAngle_rf_->Fill(prodAngle_rf, svFitSigma, evtWeight);
	
	svFitMassVsSigma_->Fill(svFitSigma, svFitMass, evtWeight);   
	
	metErrPx_->Fill(rec_minus_genMEtP4.px(), evtWeight);
	metErrPy_->Fill(rec_minus_genMEtP4.py(), evtWeight);
	metErrPt_->Fill(metErr, evtWeight);
	metCov_->Fill(metCov, evtWeight);
	metPull_->Fill(metPull, evtWeight);
	
	svFitMassVsMEtCov_->Fill(metCov, svFitMass, evtWeight);
	svFitSigmaVsMEtCov_->Fill(metCov, svFitSigma, evtWeight);
	svFitMassVsMEtErr_->Fill(metErr, svFitMass, evtWeight);
	svFitSigmaVsMEtErr_->Fill(metErr, svFitSigma, evtWeight);
	svFitMassVsMEtPull_->Fill(metPull, svFitMass, evtWeight);
	svFitSigmaVsMEtPull_->Fill(metPull, svFitSigma, evtWeight);
	svFitMassVsMEtErrProjLeg1_->Fill(metErrProjLeg1, svFitMass, evtWeight);
	svFitSigmaVsMEtErrProjLeg1_->Fill(metErrProjLeg1, svFitSigma, evtWeight);
	svFitMassVsMEtPullProjLeg1_->Fill(metPullProjLeg1, svFitMass, evtWeight);
	svFitSigmaVsMEtPullProjLeg1_->Fill(metPullProjLeg1, svFitSigma, evtWeight);
	svFitMassVsMEtErrProjLeg2_->Fill(metErrProjLeg2, svFitMass, evtWeight);
	svFitSigmaVsMEtErrProjLeg2_->Fill(metErrProjLeg2, svFitSigma, evtWeight);
	svFitMassVsMEtPullProjLeg2_->Fill(metPullProjLeg2, svFitMass, evtWeight);
	svFitSigmaVsMEtPullProjLeg2_->Fill(metPullProjLeg2, svFitSigma, evtWeight);
      }
    }

    void finalizeHistograms()
    {}

    std::string dqmDirectory_;

    double minSVfitSigma_;
    double maxSVfitSigma_;
    int isValidSolution_;

    MonitorElement* leg1Pt_;
    MonitorElement* leg2Pt_;
    MonitorElement* visMass_;
    MonitorElement* dPhi12_;
    MonitorElement* diTauPt_;
    MonitorElement* svFitMass_;
    MonitorElement* svFitSigma_;

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

    MonitorElement* metErrPx_;
    MonitorElement* metErrPy_;
    MonitorElement* metErrPt_;
    MonitorElement* metCov_;
    MonitorElement* metPull_;

    MonitorElement* svFitMassVsMEtCov_;
    MonitorElement* svFitSigmaVsMEtCov_;
    MonitorElement* svFitMassVsMEtErr_;
    MonitorElement* svFitSigmaVsMEtErr_;
    MonitorElement* svFitMassVsMEtPull_;
    MonitorElement* svFitSigmaVsMEtPull_;
    MonitorElement* svFitMassVsMEtErrProjLeg1_;
    MonitorElement* svFitSigmaVsMEtErrProjLeg1_;
    MonitorElement* svFitMassVsMEtPullProjLeg1_;
    MonitorElement* svFitSigmaVsMEtPullProjLeg1_;
    MonitorElement* svFitMassVsMEtErrProjLeg2_;
    MonitorElement* svFitSigmaVsMEtErrProjLeg2_;
    MonitorElement* svFitMassVsMEtPullProjLeg2_;
    MonitorElement* svFitSigmaVsMEtPullProjLeg2_;
  };

  std::vector<plotEntryType1*> plotEntries1_;
  std::vector<plotEntryType2*> plotEntries2_;

  long numEvents_processed_;
  double numEventsWeighted_processed_;
};

#endif   
