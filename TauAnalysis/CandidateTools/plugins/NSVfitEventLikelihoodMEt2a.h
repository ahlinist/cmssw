#ifndef TauAnalysis_CandidateTools_NSVfitEventLikelihoodMEt2a_h
#define TauAnalysis_CandidateTools_NSVfitEventLikelihoodMEt2a_h

/** \class NSVfitEventLikelihoodMEt2a
 *
 * Plugin for computing likelihood for neutrinos produced in tau lepton decays
 * to match missing transverse momentum reconstructed in the event
 *
 * NOTE: This version is specific to MVA MET
 *       and takes into account the expected response for hadrons as function of qT
 *
 * \author Christian Veelken, LLR
 *
 * \version $Revision: 1.1 $
 *
 * $Id: NSVfitEventLikelihoodMEt2a.h,v 1.1 2013/02/08 18:07:03 veelken Exp $
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "RecoMET/METAlgorithms/interface/SignAlgoResolutions.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitEventLikelihood.h"
#include "TauAnalysis/CandidateTools/interface/PFMEtSignInterface.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesis.h"

#include <TMatrixD.h>
#include <TVectorD.h>
#include <TFormula.h>

#include <list>

class NSVfitEventLikelihoodMEt2a : public NSVfitEventLikelihood
{
 public:
  NSVfitEventLikelihoodMEt2a(const edm::ParameterSet&);
  ~NSVfitEventLikelihoodMEt2a();

  void beginJob(NSVfitAlgorithmBase*);
  void beginEvent(const edm::Event&, const edm::EventSetup&);
  void beginCandidate(const NSVfitEventHypothesis*) const;

  double operator()(const NSVfitEventHypothesis*) const;

 private:

  double power_;

  edm::InputTag srcMEtCovMatrix_;

  TFormula* response_;

  mutable TMatrixD metCov_;
  mutable double   metCovDet_;
  mutable TMatrixD metCovInverse_;

  mutable double metCovInverse00_;
  mutable double metCovInverse01_;
  mutable double metCovInverse10_;
  mutable double metCovInverse11_;

  mutable double hadRecoilPx_measured_;
  mutable double hadRecoilPy_measured_;

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
