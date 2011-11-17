#ifndef TauAnalysis_TauIdEfficiency_ZllRecoilCorrectionHistManager_h
#define TauAnalysis_TauIdEfficiency_ZllRecoilCorrectionHistManager_h

/** \class ZllRecoilCorrectionHistManager
 *
 * Fill Z-recoil correction control plots
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.5 $
 *
 * $Id: ZllRecoilCorrectionHistManager.h,v 1.5 2011/11/04 09:39:20 veelken Exp $
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CommonTools/Utils/interface/TFileDirectory.h"

#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"

#include <TH1.h>
#include <TH2.h>

class ZllRecoilCorrectionHistManager
{

 public:
  /// constructor
  ZllRecoilCorrectionHistManager(edm::ParameterSet const&);

  /// destructor
  virtual ~ZllRecoilCorrectionHistManager();

  /// book and fill histograms
  void bookHistograms(TFileDirectory&);
  void fillHistograms(const reco::CompositeCandidate&, const std::vector<pat::Jet>&, const pat::MET&, size_t, double, double);
  
  /// scale all bin-contents/bin-errors by factor given as function argument
  /// (to account for events lost, due to aborted skimming/crab or PAT-tuple production/lxbatch jobs)
  void scaleHistograms(double);

  friend class histogramsUvsQtNumVtxType;

 protected:

  TH1* book1D(TFileDirectory&, const std::string&, const std::string&, int, double, double);
  TH2* book2D(TFileDirectory&, const std::string&, const std::string&, int, double, double, int, double, double);
  TH2* book2D(TFileDirectory&, const std::string&, const std::string&, int, double*, int, double, double);

 private:

  TH1* histogramLepPlusPt_;
  TH1* histogramLepPlusEta_;
  TH1* histogramLepPlusPhi_;

  TH1* histogramLepMinusPt_;
  TH1* histogramLepMinusEta_;
  TH1* histogramLepMinusPhi_;

  TH1* histogramZllCandPt_;
  TH1* histogramZllCandEta_;
  TH1* histogramZllCandPhi_;
  TH1* histogramZllCandMass_;

  TH1* histogramJetPtAbsEtaLt11_;
  TH1* histogramJetResAbsEtaLt11_;
  TH1* histogramJetPtAbsEta11to17_;
  TH1* histogramJetResAbsEta11to17_;
  TH1* histogramJetPtAbsEta17to23_;
  TH1* histogramJetResAbsEta17to23_;
  TH1* histogramJetPtAbsEtaGt23_;
  TH1* histogramJetResAbsEtaGt23_;

  TH1* histogramMEtS_;
  TH1* histogramMEtL_;
  TH1* histogramMEtProjParlZ_;
  TH1* histogramMEtProjPerpZ_;

  TH2* histogramUparlDivQtVsQt_;
  TH2* histogramUparlVsQt_;
  TH2* histogramUperpDivQtVsQt_;
  TH2* histogramUperpVsQt_;
  
  struct histogramsUvsQtNumVtxType
  {
    histogramsUvsQtNumVtxType(ZllRecoilCorrectionHistManager* histManager, 
			      TFileDirectory& dir, int qTnumBins, double* qTbinning, int numVtxMin, int numVtxMax)
      : numVtxMin_(numVtxMin),
	numVtxMax_(numVtxMax)
    {
      TString label;
      if      ( numVtxMin_ == -1         ) label = Form("Le%i",   numVtxMax_);
      else if ( numVtxMax_ == -1         ) label = Form("Ge%i",   numVtxMin_);
      else if ( numVtxMin_ == numVtxMax_ ) label = Form("Eq%i",   numVtxMin_);
      else                                 label = Form("%ito%i", numVtxMin_, numVtxMax_);

      histogramUparlDivQtVsQt_ = histManager->book2D(
	dir, TString("uParlDivQtVsQt").Append(label).Data(), "u_{#parallel}/q_{T} vs q_{T}", 
	qTnumBins, qTbinning, 400,  -5.0,   +5.0);
      histogramUparlVsQt_ = histManager->book2D(
	dir, TString("uParlVsQt").Append(label).Data(),      "u_{#parallel} vs q_{T}",   
	qTnumBins, qTbinning, 120, -50.0, +250.0);
      histogramUperpDivQtVsQt_ = histManager->book2D(
        dir, TString("uPerpDivQtVsQt").Append(label).Data(), "u_{#perp}/q_{T} vs q_{T}", 
	qTnumBins, qTbinning, 400,  -5.0,   +5.0);
      histogramUperpVsQt_ = histManager->book2D(
        dir, TString("uPerpVsQt").Append(label).Data(),      "u_{#perp} vs q_{T}",  
	qTnumBins, qTbinning,  40, -50.0,  +50.0);
    }
    ~histogramsUvsQtNumVtxType() {}
    int numVtxMin_;
    int numVtxMax_;

    TH2* histogramUparlDivQtVsQt_;
    TH2* histogramUparlVsQt_;
    TH2* histogramUperpDivQtVsQt_;
    TH2* histogramUperpVsQt_;
  };

  std::vector<histogramsUvsQtNumVtxType*> histogramsUvsQtNumVtxBinned_;

  TH1* histogramVtxMultiplicity_;
  TH1* histogramRhoNeutral_;
  
  std::vector<TH1*> histograms_;
};

#endif
