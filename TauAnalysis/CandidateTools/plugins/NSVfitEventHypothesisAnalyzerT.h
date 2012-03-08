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

  edm::InputTag src_;
  
  typedef std::vector<edm::InputTag> vInputTag;
  vInputTag srcWeights_;

  std::string dqmDirectory_;

  MonitorElement* svFitIsValidSolution_;
  
  struct plotEntryType
  {
    plotEntryType(const std::string& dqmDirectory)
      : dqmDirectory_(dqmDirectory)
    {}
    ~plotEntryType() {}
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

      svFitMass_mean_        = dqmStore.book1D("svFitMass_mean",        "svFitMass_mean",        250, 0., 250.);
      svFitMass_median_      = dqmStore.book1D("svFitMass_median",      "svFitMass_median",      250, 0., 250.);
      svFitMass_maximum_     = dqmStore.book1D("svFitMass_maximum",     "svFitMass_maximum",     250, 0., 250.);
      svFitMass_maxInterpol_ = dqmStore.book1D("svFitMass_maxInterpol", "svFitMass_maxInterpol", 250, 0., 250.);

      svFitMassVsLeg1Pt_  = dqmStore.book2D("svFitMassVsLeg1Pt",  "svFitMassVsLeg1Pt",  150, 0., 150., 50, 0., 250.);
      svFitMassVsLeg2Pt_  = dqmStore.book2D("svFitMassVsLeg2Pt",  "svFitMassVsLeg2Pt",  150, 0., 150., 50, 0., 250.);
      svFitMassVsVisMass_ = dqmStore.book2D("svFitMassVsVisMass", "svFitMassVsVisMass",  50, 0., 250., 50, 0., 250.);
      svFitMassVsDPhi12_  = dqmStore.book2D("svFitMassVsDPhi12",  "svFitMassVsDPhi12",   36, 0., TMath::Pi(), 50, 0., 250.);
      svFitMassVsDiTauPt_ = dqmStore.book2D("svFitMassVsDiTauPt", "svFitMassVsDiTauPt", 150, 0., 150., 50, 0., 250.);

      svFitMassVsProdAngle_rf_ = 
	dqmStore.book2D("svFitMassVsProdAngle_rf",  
			"svFitMassVsProdAngle_rf", 36, 0., TMath::Pi(), 150, 0., 150.);
      svFitSigmaVsProdAngle_rf_ = 
	dqmStore.book2D("svFitSigmaVsProdAngle_rf", 
			"svFitSigmaVsProdAngle_rf", 36, 0., TMath::Pi(), 50, 0., 50.);

      svFitMassVsSigma_ = 
	dqmStore.book2D("svFitMassVsSigma",             
			"svFitMassVsSigma", 50, 0.,  50., 50, 0., 250.);

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

      avSVfitMassVsLeg1And2Pt_  = dqmStore.book2D("avSVfitMassVsLeg1And2Pt",  "avSVfitMassVsLeg1And2Pt",  40, 0., 100., 40, 0., 100.);
      avSVfitSigmaVsLeg1And2Pt_ = dqmStore.book2D("avSVfitSigmaVsLeg1And2Pt", "avSVfitSigmaVsLeg1And2Pt", 40, 0., 100., 40, 0., 100.);
      normLeg1And2Pt_           = dqmStore.book2D("normLeg1And2Pt",           "normLeg1And2Pt",           40, 0., 100., 40, 0., 100.);
    }
    void fillHistograms(const reco::Candidate::LorentzVector& leg1P4, 
			const reco::Candidate::LorentzVector& leg2P4,
			double svFitMass, double svFitSigma, double diTauPt, double prodAngle_rf,
			double svFitMass_mean, double svFitMass_median, double svFitMass_maximum, double svFitMass_maxInterpol,
			double evtWeight)
    {
      double leg1Pt = leg1P4.pt();
      double leg2Pt = leg2P4.pt();

      double visMass = (leg1P4 + leg2P4).mass();

      double dPhi12 = TMath::ACos(TMath::Cos(leg1P4.phi() - leg2P4.phi()));

      leg1Pt_->Fill(leg1Pt, evtWeight);
      leg2Pt_->Fill(leg2Pt, evtWeight);
      visMass_->Fill(visMass, evtWeight);
      dPhi12_->Fill(dPhi12, evtWeight);
      diTauPt_->Fill(diTauPt, evtWeight);
      svFitMass_->Fill(svFitMass, evtWeight);
      svFitSigma_->Fill(svFitSigma, evtWeight);
      
      svFitMass_mean_ ->Fill(svFitMass_mean, evtWeight);
      svFitMass_median_->Fill(svFitMass_median, evtWeight);
      svFitMass_maximum_->Fill(svFitMass_maximum, evtWeight);
      svFitMass_maxInterpol_->Fill(svFitMass_maxInterpol, evtWeight);

      svFitMassVsLeg1Pt_->Fill(leg1Pt, svFitMass, evtWeight);
      svFitMassVsLeg2Pt_->Fill(leg2Pt, svFitMass, evtWeight);
      svFitMassVsVisMass_->Fill(visMass, svFitMass, evtWeight);
      svFitMassVsDPhi12_->Fill(dPhi12, svFitMass, evtWeight);      
      svFitMassVsDiTauPt_->Fill(diTauPt, svFitMass, evtWeight);

      svFitMassVsProdAngle_rf_->Fill(prodAngle_rf, svFitMass, evtWeight);
      svFitSigmaVsProdAngle_rf_->Fill(prodAngle_rf, svFitSigma, evtWeight);

      svFitMassVsSigma_->Fill(svFitSigma, svFitMass, evtWeight);   
      svFitMassVsSigma_mean_->Fill(svFitSigma, svFitMass_mean, evtWeight);
      svFitMassVsSigma_median_->Fill(svFitSigma, svFitMass_median, evtWeight);
      svFitMassVsSigma_maximum_->Fill(svFitSigma, svFitMass_maximum, evtWeight);
      svFitMassVsSigma_maxInterpol_->Fill(svFitSigma, svFitMass_maxInterpol, evtWeight);

      avSVfitMassVsLeg1And2Pt_->Fill(leg1Pt, leg2Pt, svFitMass*evtWeight);
      avSVfitSigmaVsLeg1And2Pt_->Fill(leg1Pt, leg2Pt, svFitSigma*evtWeight);
      normLeg1And2Pt_->Fill(leg1Pt, leg2Pt, evtWeight);
    }

    void finalizeHistograms()
    {
      int numBinsX = normLeg1And2Pt_->getTH1()->GetNbinsX();
      for ( int iBinX = 1; iBinX <= numBinsX; ++iBinX ) {
	int numBinsY = normLeg1And2Pt_->getTH1()->GetNbinsY();
	for ( int iBinY = 1; iBinY <= numBinsY; ++iBinY ) {
	  double norm = normLeg1And2Pt_->getTH1()->GetBinContent(iBinX, iBinY);
	  if ( norm > 0. ) {
	    double avSVfitMass = avSVfitMassVsLeg1And2Pt_->getTH1()->GetBinContent(iBinX, iBinY);
	    avSVfitMass /= norm;
	    avSVfitMassVsLeg1And2Pt_->getTH1()->SetBinContent(iBinX, iBinY, avSVfitMass);
	    double avSVfitSigma = avSVfitSigmaVsLeg1And2Pt_->getTH1()->GetBinContent(iBinX, iBinY);
	    avSVfitSigma /= norm;
	    avSVfitSigmaVsLeg1And2Pt_->getTH1()->SetBinContent(iBinX, iBinY, avSVfitSigma);
	  }
	}
      }
    }

    std::string dqmDirectory_;

    MonitorElement* leg1Pt_;
    MonitorElement* leg2Pt_;
    MonitorElement* visMass_;
    MonitorElement* dPhi12_;
    MonitorElement* diTauPt_;
    MonitorElement* svFitMass_;
    MonitorElement* svFitSigma_;

    MonitorElement* svFitMass_mean_;
    MonitorElement* svFitMass_median_;
    MonitorElement* svFitMass_maximum_;
    MonitorElement* svFitMass_maxInterpol_;

    MonitorElement* svFitMassVsLeg1Pt_;
    MonitorElement* svFitMassVsLeg2Pt_;
    MonitorElement* svFitMassVsVisMass_;
    MonitorElement* svFitMassVsDPhi12_;
    MonitorElement* svFitMassVsDiTauPt_;

    MonitorElement* svFitMassVsProdAngle_rf_;
    MonitorElement* svFitSigmaVsProdAngle_rf_;

    MonitorElement* svFitMassVsSigma_;
    MonitorElement* svFitMassVsSigma_mean_;
    MonitorElement* svFitMassVsSigma_median_;
    MonitorElement* svFitMassVsSigma_maximum_;
    MonitorElement* svFitMassVsSigma_maxInterpol_;

    MonitorElement* avSVfitMassVsLeg1And2Pt_;
    MonitorElement* avSVfitSigmaVsLeg1And2Pt_;
    MonitorElement* normLeg1And2Pt_;
  };

  plotEntryType* plotsSVfitSolutionValid_;
  plotEntryType* plotsSVfitSolutionInvalid_;

  long numEvents_processed_;
  double numEventsWeighted_processed_;
};

#endif   
