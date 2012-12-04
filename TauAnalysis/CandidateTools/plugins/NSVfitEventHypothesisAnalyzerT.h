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

  MonitorElement* leg1Pt_;
  MonitorElement* leg2Pt_;
  MonitorElement* metPt_;
  MonitorElement* svFitMass_;
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

  MonitorElement* svFitIsValidSolution_;
  
  MonitorElement* svFitMassVsSigmaDownXL_;
  MonitorElement* svFitMassVsSigmaDownXL_oneProng0pi0_;
  MonitorElement* svFitMassVsSigmaDownXL_oneProng1pi0_;
  MonitorElement* svFitMassVsSigmaDownXL_oneProng2pi0_;
  MonitorElement* svFitMassVsSigmaDownXL_threeProng0pi0_;

  MonitorElement* svFitMassVsNLL_;

  MonitorElement* svFitMassVsMEtXL_;
  MonitorElement* svFitMassVsMEtXL_oneProng0pi0_;
  MonitorElement* svFitMassVsMEtXL_oneProng1pi0_;
  MonitorElement* svFitMassVsMEtXL_oneProng2pi0_;
  MonitorElement* svFitMassVsMEtXL_threeProng0pi0_;

  struct plotEntryType
  {
    plotEntryType(const std::string& dqmDirectory, double minDPhi12, double maxDPhi12, int isValidSolution, 
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
      dqmDirectory_full.Append("plotEntryType1");
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
    ~plotEntryType() {}
    void bookHistograms(DQMStore& dqmStore)
    {
      dqmStore.setCurrentFolder(dqmDirectory_.data());
            
      dPhi12_ = dqmStore.book1D("dPhi12", "dPhi12", 180, 0., 180.);

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

      svFitMassVsMEt_ = 
	dqmStore.book2D("svFitMassVsMEt",             
			"svFitMassVsMEt", 100, 0., 0.5*svFitMassMax_, 100, 0., svFitMassMax_);
      svFitSigmaVsMEt_ = 
	dqmStore.book2D("svFitSigmaVsMEt",             
			"svFitSigmaVsMEt", 100, 0., 0.5*svFitMassMax_, 100, 0., svFitSigmaMax_);

      svFitMass_ = 
	dqmStore.book1D("svFitMass",             
			"svFitMass", numBinsSVfitMass_, 0., svFitMassMax_);
      svFitMass_oneProng0pi0_ = 
	dqmStore.book1D("svFitMass_oneProng0pi0",             
			"svFitMass_oneProng0pi0", numBinsSVfitMass_, 0., svFitMassMax_);
      svFitMass_oneProng1pi0_ = 
	dqmStore.book1D("svFitMass_oneProng1pi0",             
			"svFitMass_oneProng1pi0", numBinsSVfitMass_, 0., svFitMassMax_);
      svFitMass_oneProng2pi0_ = 
	dqmStore.book1D("svFitMass_oneProng2pi0",             
			"svFitMass_oneProng2pi0", numBinsSVfitMass_, 0., svFitMassMax_);
      svFitMass_threeProng0pi0_ = 
	dqmStore.book1D("svFitMass_threeProng0pi0",             
			"svFitMass_threeProng0pi0", numBinsSVfitMass_, 0., svFitMassMax_);

      visMass_ = 
	dqmStore.book1D("visMass",             
			"visMass", numBinsSVfitMass_, 0., svFitMassMax_);
    }      
    void fillHistograms(bool isValidSolution,
			const reco::Candidate::LorentzVector& recLeg1P4, int recLeg1DecayMode,
			const reco::Candidate::LorentzVector& recLeg2P4, int recLeg2DecayMode,
			double svFitMass,
			double svFitMass_mean, double svFitMass_median, double svFitMass_maximum, double svFitMass_maxInterpol,
			double svFitSigma,
			double recMEtPt, 
			double evtWeight)
    {
      if ( isValidSolution_ < 0 &&  isValidSolution ) return;
      if ( isValidSolution_ > 0 && !isValidSolution ) return;

      double dPhi12 = TMath::ACos(TMath::Cos(recLeg1P4.phi() - recLeg2P4.phi()));
      dPhi12 *= TMath::RadToDeg();
      
      if ( (dPhi12 > minDPhi12_ || minDPhi12_ <= 0.) &&
	   (dPhi12 < maxDPhi12_ || maxDPhi12_ <= 0.) ) {

	dPhi12_->Fill(dPhi12, evtWeight);

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

	svFitMassVsMEt_->Fill(recMEtPt, svFitMass, evtWeight);
	svFitSigmaVsMEt_->Fill(recMEtPt, svFitSigma, evtWeight);

	svFitMass_->Fill(svFitMass, evtWeight);
	if ( recLeg1DecayMode == reco::PFTau::kOneProng0PiZero ) {
	  svFitMass_oneProng0pi0_->Fill(svFitMass, evtWeight);
	} else if ( recLeg1DecayMode == reco::PFTau::kOneProng1PiZero ) {	
	  svFitMass_oneProng1pi0_->Fill(svFitMass, evtWeight);
	} else if ( recLeg1DecayMode == reco::PFTau::kOneProng2PiZero ) {
	  svFitMass_oneProng2pi0_->Fill(svFitMass, evtWeight);
	} else if ( recLeg1DecayMode == reco::PFTau::kThreeProng0PiZero ) {	
	  svFitMass_threeProng0pi0_->Fill(svFitMass, evtWeight);
	}

	double visMass = (recLeg1P4 + recLeg2P4).mass();
	visMass_->Fill(visMass, evtWeight);
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
    
    MonitorElement* dPhi12_;

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

    MonitorElement* svFitMassVsMEt_;
    MonitorElement* svFitSigmaVsMEt_;

    MonitorElement* svFitMass_;
    MonitorElement* svFitMass_oneProng0pi0_;
    MonitorElement* svFitMass_oneProng1pi0_;
    MonitorElement* svFitMass_oneProng2pi0_;
    MonitorElement* svFitMass_threeProng0pi0_;    

    MonitorElement* visMass_;
  };

  std::vector<plotEntryType*> plotEntries_;
  
  long numEvents_processed_;
  double numEventsWeighted_processed_;
};

#endif   
