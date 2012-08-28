#ifndef TauAnalysis_TauIdEfficiency_NoPileUpMEtInputHistManager_h
#define TauAnalysis_TauIdEfficiency_NoPileUpMEtInputHistManager_h

/** \class NoPileUpMEtInputHistManager
 *
 * Fill control plots of input variables entering no-PU MET computation
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.14 $
 *
 * $Id: NoPileUpMEtInputHistManager.h,v 1.14 2012/05/04 15:57:38 veelken Exp $
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h" 

#include "CommonTools/Utils/interface/TFileDirectory.h"

#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/METReco/interface/CommonMETData.h"

#include "TauAnalysis/CandidateTools/interface/candidateAuxFunctions.h"

#include <TH1.h>
#include <TMath.h>

#include <vector>
#include <string>

class NoPileUpMEtInputHistManager
{

 public:
  /// constructor
  NoPileUpMEtInputHistManager(edm::ParameterSet const&);

  /// destructor
  virtual ~NoPileUpMEtInputHistManager();

  /// book and fill histograms
  void bookHistograms(TFileDirectory&);
  void fillHistograms(const reco::CompositeCandidate&, const std::vector<pat::Muon>&, const std::vector<pat::Jet>&, const fwlite::Event&, double);
  
  /// scale all bin-contents/bin-errors by factor given as function argument
  /// (to account for events lost, due to aborted skimming/crab or PAT-tuple production/lxbatch jobs)
  void scaleHistograms(double);

 protected:

  TH1* book1D(TFileDirectory&, const std::string&, const std::string&, int, double, double);

 private:

  edm::InputTag src_;

  typedef std::vector<std::string> vstring;
  vstring inputsToPlot_;

  struct histogramEntryType
  {
    histogramEntryType(NoPileUpMEtInputHistManager* histManager, TFileDirectory& dir, const edm::InputTag& src)
      : src_(src)
    {
      bookHistograms(histManager, dir);
    }
    ~histogramEntryType() {}
    void bookHistograms(NoPileUpMEtInputHistManager* histManager, TFileDirectory& dir)
    {
      histogramPt_    = histManager->book1D(dir, "Pt",    "P_{T}",         75,          0.0,        150.0);
      histogramPparl_ = histManager->book1D(dir, "Pparl", "P_{perp}",     140,       -275.0,        +75.0);   
      histogramPperp_ = histManager->book1D(dir, "Pperp", "P_{parl}",      50,        -50.0,        +50.0);   
      histogramPhi_   = histManager->book1D(dir, "Phi",   "#phi",          36, -TMath::Pi(), +TMath::Pi());
      histogramSumEt_ = histManager->book1D(dir, "SumEt", "#Sigma E_{T}", 100,          0.0,       1000.0);
    }
    void fillHistograms(const reco::Candidate::LorentzVector& ZllCandP4, const fwlite::Event& evt, double evtWeight)
    {
      edm::Handle<CommonMETData> metData;
      evt.getByLabel(src_, metData);

      histogramPt_->Fill(metData->met, evtWeight);

      int errorFlag = 0;
      std::pair<double, double> uT = compMEtProjU(ZllCandP4, metData->mex, metData->mey, errorFlag);
      if ( !errorFlag ) {
	double uParl = uT.first;
	double uPerp = uT.second;
	
	histogramPparl_->Fill(uParl, evtWeight);
	histogramPperp_->Fill(uPerp, evtWeight);
      }

      histogramPt_->Fill(metData->met, evtWeight);
      histogramSumEt_->Fill(metData->sumet, evtWeight);
    }

    edm::InputTag src_;

    TH1* histogramPt_;
    TH1* histogramPparl_;
    TH1* histogramPperp_;
    TH1* histogramPhi_;
    TH1* histogramSumEt_;
  };

  std::vector<histogramEntryType*> histogramEntries_;

  std::vector<TH1*> histograms_;
};

#endif
