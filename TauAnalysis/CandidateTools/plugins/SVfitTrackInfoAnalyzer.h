#ifndef TauAnalysis_CandidateTools_SVfitTrackInfoAnalyzer_h
#define TauAnalysis_CandidateTools_SVfitTrackInfoAnalyzer_h

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "DataFormats/CLHEP/interface/AlgebraicObjects.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitDecayVertexFitter.h"

#include <TMath.h>
#include <TString.h>

#include <string>

namespace
{
  double compScalarProduct(const AlgebraicVector3& p1, const AlgebraicVector3& p2)
  {
    return (p1(0)*p2(0) + p1(1)*p2(1) + p1(2)*p2(2));
  }

  AlgebraicVector3 compCrossProduct(const AlgebraicVector3& p1, const AlgebraicVector3& p2)
  {    
    double p3_x = p1(1)*p2(2) - p1(2)*p2(1);
    double p3_y = p1(2)*p2(0) - p1(0)*p2(2);
    double p3_z = p1(0)*p2(1) - p1(1)*p2(0);
    return AlgebraicVector3(p3_x, p3_y, p3_z);
  }

  double norm2(const AlgebraicVector3& p)
  {
    return compScalarProduct(p, p);
  }

  AlgebraicVector3 normalize(const AlgebraicVector3& p)
  {
    double p_mag2 = norm2(p);
    if ( p_mag2 > 0. ) {
      double p_mag = TMath::Sqrt(p_mag2);
      return AlgebraicVector3(p(0)/p_mag, p(1)/p_mag, p(2)/p_mag);
    } else {
      return AlgebraicVector3(0., 0., 0.);
    }
  }

  void fillHistograms1d(MonitorElement* histogramError, MonitorElement* histogramSigma, MonitorElement* histogramPull,
			const AlgebraicVector3& residual, const AlgebraicMatrix33& cov, 
			const AlgebraicVector3& u,
			double evtWeight)
  {
    double error = compScalarProduct(residual, u);
    histogramError->Fill(error, evtWeight);
    double sigma2 = ROOT::Math::Similarity(u, cov);
    if ( sigma2 > 0. ) {
      double sigma = TMath::Sqrt(sigma2);
      histogramSigma->Fill(sigma, evtWeight);
      histogramPull->Fill(error/sigma, evtWeight);
    }
  }

  std::string getPt_string(double ptMin, double ptMax)
  {
    std::string retVal = "";
    if      ( ptMin > 0. && ptMax > 0. ) retVal = Form("pt%1.0fto%1.0f", ptMin, ptMax);
    else if ( ptMin > 0.               ) retVal = Form("ptGt%1.0f", ptMin);
    else if ( ptMax > 0.               ) retVal = Form("ptLt%1.0f", ptMax);
    return retVal;
  }

  std::string getEta_string(double eta, bool addSign)
  {
    TString retVal = "";
    if ( addSign ) {
      if ( eta < 0. ) retVal.Append("-");
      if ( eta > 0. ) retVal.Append("+");
    }
    retVal.Append(Form("%1.1f", TMath::Abs(eta)));
    retVal.ReplaceAll(".", "p");
    return retVal.Data();
  }

  std::string getEta_string(double etaMin, double etaMax)
  {
    std::string retVal = "";
    if      ( etaMin > -5. && etaMax < +5. ) retVal = Form("eta%sto%s", getEta_string(etaMin, true).data(), getEta_string(etaMax, true).data());    
    else if ( etaMin > -5.                 ) retVal = Form("etaGt%s", getEta_string(etaMin, false).data());
    else if ( etaMax < +5.                 ) retVal = Form("etaLt%s", getEta_string(etaMax, false).data());
    return retVal;
  }
}

class SVfitTrackInfoAnalyzer : public edm::EDAnalyzer 
{
 public:
  // constructor 
  explicit SVfitTrackInfoAnalyzer(const edm::ParameterSet&);
    
  // destructor
  ~SVfitTrackInfoAnalyzer();
    
 private:
  void beginJob();
  void analyze(const edm::Event&, const edm::EventSetup&);

  std::string moduleLabel_;

  edm::InputTag srcGenParticles_;
  edm::InputTag srcLeptons_;

  typedef std::vector<edm::InputTag> vInputTag;
  vInputTag srcWeights_;

  NSVfitDecayVertexFitter* decayVertexFitAlgorithm_;

  std::string dqmDirectory_;

  struct plotEntryType
  {
    plotEntryType(const std::string& dqmDirectory, double ptMin, double ptMax, double etaMin, double etaMax)
      : ptMin_(ptMin),
	ptMax_(ptMax),
	etaMin_(etaMin),
	etaMax_(etaMax)
    {
      dqmDirectory_ = dqmDirectory;
      std::string pt_string = getPt_string(ptMin, ptMax);
      std::string eta_string = getEta_string(etaMin, etaMax);
      if ( pt_string != "" || eta_string != "" ) dqmDirectory_.append("/");
      if ( pt_string != "" ) dqmDirectory_.append(pt_string);
      if ( pt_string != "" && eta_string != "" ) dqmDirectory_.append("_");
      if ( eta_string != "" ) dqmDirectory_.append(eta_string);
    }
    ~plotEntryType() {}

    void bookHistograms(DQMStore& dqmStore)
    {
      dqmStore.setCurrentFolder(dqmDirectory_.data());

      errorX_    = dqmStore.book1D("errorX",    "errorX",    2000, -0.1, +0.1);
      sigmaX_    = dqmStore.book1D("sigmaX",    "sigmaX",    1000,  0.,   0.1);
      pullX_     = dqmStore.book1D("pullX",     "pullX",      250,  0.,  25.);
      errorY_    = dqmStore.book1D("errorY",    "errorY",    2000, -0.1, +0.1);
      sigmaY_    = dqmStore.book1D("sigmaY",    "sigmaY",    1000,  0.,   0.1);
      pullY_     = dqmStore.book1D("pullY",     "pullY",      250,  0.,  25.);
      errorZ_    = dqmStore.book1D("errorZ",    "errorZ",    2000, -0.1, +0.1);
      sigmaZ_    = dqmStore.book1D("sigmaZ",    "sigmaZ",    1000,  0.,   0.1);
      pullZ_     = dqmStore.book1D("pullZ",     "pullZ",      250,  0.,  25.);

      errorParl_ = dqmStore.book1D("errorParl", "errorParl", 2000, -0.1, +0.1);
      sigmaParl_ = dqmStore.book1D("sigmaParl", "sigmaParl", 1000,  0.,   0.1);
      pullParl_  = dqmStore.book1D("pullParl",  "pullParl",   250,  0.,  25.);
      errorPerp_ = dqmStore.book1D("errorPerp", "errorPerp", 2000, -0.1, +0.1);
      sigmaPerp_ = dqmStore.book1D("sigmaPerp", "sigmaPerp", 1000,  0.,   0.1);
      pullPerp_  = dqmStore.book1D("pullPerp",  "pullPerp",   250,  0.,  25.);
      errorR_    = dqmStore.book1D("errorR",    "errorR",    2000, -0.1, +0.1);
      sigmaR_    = dqmStore.book1D("sigmaR",    "sigmaR",    1000,  0.,   0.1);
      pullR_     = dqmStore.book1D("pullR",     "pullR",      250,  0.,  25.);

      error2d_   = dqmStore.book1D("error2d",   "error2d",   2000, -0.1, +0.1);
      sigma2d_   = dqmStore.book1D("sigma2d",   "sigma2d",   1000,  0.,   0.1);
      pull2d_    = dqmStore.book1D("pull2d",    "pull2d",     250,  0.,  25.);

      error3d_   = dqmStore.book1D("error3d",   "error3d",   2000, -0.1, +0.1);
      sigma3d_   = dqmStore.book1D("sigma3d",   "sigma3d",   1000,  0.,   0.1);
      pull3d_    = dqmStore.book1D("pull3d",    "pull3d",     250,  0.,  25.);
    }

    void fillHistograms(double pt, double eta, 
			const AlgebraicVector3& genProdVertexPos, const AlgebraicVector3& genDecayVertexPos, 
			const AlgebraicVector3& recDecayVertexPos, const AlgebraicMatrix33& recDecayVertexCov,
			double evtWeight)
    {
      if ( pt  > ptMin_  && (pt  < ptMax_ || ptMax_ < 0.) &&
	   eta > etaMin_ &&  eta < etaMax_ ) {
	AlgebraicVector3 residual = recDecayVertexPos - genDecayVertexPos;

	AlgebraicVector3 uX(1., 0., 0.);
	AlgebraicVector3 uY(0., 1., 0.);
	AlgebraicVector3 uZ(0., 0., 1.);

	fillHistograms1d(errorX_, sigmaX_, pullX_, residual, recDecayVertexCov, uX, evtWeight);
	fillHistograms1d(errorY_, sigmaY_, pullY_, residual, recDecayVertexCov, uY, evtWeight);
	fillHistograms1d(errorZ_, sigmaZ_, pullZ_, residual, recDecayVertexCov, uZ, evtWeight);

	AlgebraicVector3 uR    = normalize(genDecayVertexPos - genProdVertexPos);
	AlgebraicVector3 uPerp = normalize(compCrossProduct(uZ, uR));
	AlgebraicVector3 uParl = compCrossProduct(uPerp, uR);

	fillHistograms1d(errorParl_, sigmaParl_, pullParl_, residual, recDecayVertexCov, uParl, evtWeight);
	fillHistograms1d(errorPerp_, sigmaPerp_, pullPerp_, residual, recDecayVertexCov, uPerp, evtWeight);
	fillHistograms1d(errorR_,    sigmaR_,    pullR_,    residual, recDecayVertexCov, uR,    evtWeight);

	AlgebraicVector3 uResidual2d = normalize(AlgebraicVector3(residual(0), residual(1), 0.));
	fillHistograms1d(error2d_, sigma2d_, pull2d_, residual, recDecayVertexCov, uResidual2d, evtWeight);

	AlgebraicVector3 uResidual3d = normalize(residual);
	fillHistograms1d(error3d_, sigma3d_, pull3d_, residual, recDecayVertexCov, uResidual3d, evtWeight);
      }
    }

    std::string dqmDirectory_;
    double ptMin_;
    double ptMax_;
    double etaMin_;
    double etaMax_;

    MonitorElement* errorX_;
    MonitorElement* sigmaX_;
    MonitorElement* pullX_;
    MonitorElement* errorY_;
    MonitorElement* sigmaY_;
    MonitorElement* pullY_;
    MonitorElement* errorZ_;
    MonitorElement* sigmaZ_;
    MonitorElement* pullZ_;
    
    MonitorElement* errorParl_;
    MonitorElement* sigmaParl_;
    MonitorElement* pullParl_;
    MonitorElement* errorPerp_;
    MonitorElement* sigmaPerp_;
    MonitorElement* pullPerp_;
    MonitorElement* errorR_;
    MonitorElement* sigmaR_;
    MonitorElement* pullR_;

    MonitorElement* error2d_;
    MonitorElement* sigma2d_;
    MonitorElement* pull2d_;

    MonitorElement* error3d_;
    MonitorElement* sigma3d_;
    MonitorElement* pull3d_;
  };

  std::vector<plotEntryType*> plotEntries1Prong_;
  std::vector<plotEntryType*> plotEntries3Prong_;
};

#endif   
