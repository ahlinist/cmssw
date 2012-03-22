#ifndef TauAnalysis_CandidateTools_NSVfitEventLikelihoodMEt2_h
#define TauAnalysis_CandidateTools_NSVfitEventLikelihoodMEt2_h

/** \class NSVfitEventLikelihoodMEt2
 *
 * Plugin for computing likelihood for neutrinos produced in tau lepton decays
 * to match missing transverse momentum reconstructed in the event
 *
 * New version using covariance matrix of (PF)MET significance calculation
 * (CMS AN-10/400) to compute the likehood
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.7 $
 *
 * $Id: NSVfitEventLikelihoodMEt2.h,v 1.7 2012/03/16 17:35:40 veelken Exp $
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "RecoMET/METAlgorithms/interface/SignAlgoResolutions.h"
#include "RecoMET/METAlgorithms/interface/SigInputObj.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitEventLikelihood.h"
#include "TauAnalysis/CandidateTools/interface/PFMEtSignInterface.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesis.h"

#include <TMatrixD.h>
#include <TVectorD.h>

#include <list>

class NSVfitEventLikelihoodMEt2 : public NSVfitEventLikelihood
{
 public:
  NSVfitEventLikelihoodMEt2(const edm::ParameterSet&);
  ~NSVfitEventLikelihoodMEt2();

  void beginJob(NSVfitAlgorithmBase*);
  void beginEvent(const edm::Event&, const edm::EventSetup&);
  void beginCandidate(const NSVfitEventHypothesis*) const;

  double operator()(const NSVfitEventHypothesis*) const;

 private:

  double power_;

  edm::InputTag srcMEtCovMatrix_;

  PFMEtSignInterface* pfMEtSign_;

  mutable TMatrixD pfMEtCov_;
  mutable double   pfMEtCovDet_;
  mutable TMatrixD pfMEtCovInverse_;

  mutable double pfMEtCovInverse00_;
  mutable double pfMEtCovInverse01_;
  mutable double pfMEtCovInverse10_;
  mutable double pfMEtCovInverse11_;

  mutable double residual_fitted0_;
  mutable double residual_fitted1_;

  mutable double nllConstTerm_;

  struct tailProbCorrFunctionType
  {
    tailProbCorrFunctionType(const std::string& pluginName, const edm::ParameterSet& cfg)
      : function_(0)
    {
      std::string formula = cfg.getParameter<std::string>("formula");
      std::string functionName = Form("%s_formula", pluginName.data());
      function_ = new TFormula(functionName.data(), formula.data());
      xMin_ = cfg.getParameter<double>("xMin");
      xMax_ = cfg.getParameter<double>("xMax");
      numParameter_ = function_->GetNpar();   
      if ( numParameter_ > 0 ) {
	edm::ParameterSet cfgPars = cfg.getParameter<edm::ParameterSet>("parameter");
	parameter_.resize(numParameter_);
	for ( int iParameter = 0; iParameter < numParameter_; ++iParameter ) {
	  parameter_[iParameter] = cfgPars.getParameter<double>(Form("par%i", iParameter));
	  function_->SetParameter(iParameter, parameter_[iParameter]);
	}
      }
    }

    ~tailProbCorrFunctionType()
    {
      delete function_;
    }

    double eval(double x) const
    {
      double x_limited = x;
      if ( x_limited < xMin_ ) x_limited = xMin_;
      if ( x_limited > xMax_ ) x_limited = xMax_;
      return function_->Eval(x_limited);
    }
    
    TFormula* function_;
    double xMin_;
    double xMax_;
    int numParameter_;
    std::vector<double> parameter_;
  };

  tailProbCorrFunctionType* tailProbCorrFunction_;
};

#endif
