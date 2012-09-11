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

#include "DataFormats/Math/interface/AlgebraicROOTObjects.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitDecayVertexFitter.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitEventVertexRefitter.h"
#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"

#include <TMath.h>
#include <TString.h>
#include <TMatrixD.h>
#include <TVectorD.h>

#include <string>
#include <iostream>
#include <iomanip>

using namespace SVfit_namespace;

namespace
{
  void fillHistograms1d(MonitorElement* histogramResidual, MonitorElement* histogramSigma, MonitorElement* histogramPull,
			double residual, double sigma, 
			double evtWeight)
  {
    histogramResidual->Fill(residual, evtWeight);
    histogramSigma->Fill(sigma, evtWeight);
    if ( sigma > 0. ) histogramPull->Fill(residual/sigma, evtWeight);
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

  double pull2d(const AlgebraicVector3& residual, const AlgebraicMatrix33& cov)
  {
    //std::cout << "<pull2d>:" << std::endl;
    //printMatrix("cov", cov);
    double det = cov(0, 0)*cov(1, 1) - cov(0, 1)*cov(1, 0);
    //std::cout << " det = " << det << std::endl;
    if ( det != 0. ) {
      double covInverse00 =  cov(1, 1)/det;
      double covInverse01 = -cov(0, 1)/det;
      double covInverse10 = -cov(1, 0)/det;
      double covInverse11 =  cov(0, 0)/det;
      //std::cout << " covInverse:" << std::endl;
      //std::cout << "|" << std::setw(12) << covInverse00 << " " << std::setw(12) << covInverse01 << "|" << std::endl;
      //std::cout << "|" << std::setw(12) << covInverse10 << " " << std::setw(12) << covInverse11 << "|" << std::endl;
      double pull =  residual(0)*(covInverse00*residual(0) + covInverse01*residual(1))
	           + residual(1)*(covInverse10*residual(0) + covInverse11*residual(1));
      return pull;
    } else {
      return -1.;
    }
  }

  double pull3d(const AlgebraicVector3& residual, const AlgebraicMatrix33& cov)
  {
    //std::cout << "<pull3d>:" << std::endl;
    AlgebraicMatrix33 covInverse = cov;
    //printMatrix("cov", cov);
    bool statusFlag = covInverse.Invert();
    if ( statusFlag ) {
      //printMatrix("covInverse", covInverse);
      double pull =  residual(0)*(covInverse(0, 0)*residual(0) + covInverse(0, 1)*residual(1) + covInverse(0, 2)*residual(2))
	           + residual(1)*(covInverse(1, 0)*residual(0) + covInverse(1, 1)*residual(1) + covInverse(1, 2)*residual(2))
	           + residual(2)*(covInverse(2, 0)*residual(0) + covInverse(2, 1)*residual(1) + covInverse(2, 2)*residual(2));
      return pull;
    } else {
      return -1.;
    }
  }

  double angle3d(const AlgebraicVector3& p1, const AlgebraicVector3& p2)
  {
    return TMath::ACos(compScalarProduct(p1, p2)/TMath::Sqrt(norm2(p1)*norm2(p2)));
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
  typedef std::vector<edm::InputTag> vInputTag;
  vInputTag srcLeptons_;
  edm::InputTag srcVerticesWithoutBS_;
  edm::InputTag srcVerticesWithBS_;
  edm::InputTag srcBeamSpot_;

  vInputTag srcWeights_;

  NSVfitDecayVertexFitter* decayVertexFitAlgorithm_;
  NSVfitEventVertexRefitter* prodVertexFitAlgorithm_woBeamSpot_;
  NSVfitEventVertexRefitter* prodVertexFitAlgorithm_wBeamSpot_;

  std::string dqmDirectory_;

  struct plotEntryType1
  {
    plotEntryType1(const std::string& dqmDirectory, double ptMin, double ptMax, double etaMin, double etaMax)
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
    ~plotEntryType1() {}

    void bookHistograms(DQMStore& dqmStore)
    {
      dqmStore.setCurrentFolder(dqmDirectory_.data());

      residualX_                     = dqmStore.book1D("residualX",                     "residualX",                     2000,          -0.1,        +0.1);
      sigmaX_                        = dqmStore.book1D("sigmaX",                        "sigmaX",                        1000,           0.,          0.1);
      pullX_                         = dqmStore.book1D("pullX",                         "pullX",                          500,         -25.,         +25.);
      residualY_                     = dqmStore.book1D("residualY",                     "residualY",                     2000,          -0.1,        +0.1);
      sigmaY_                        = dqmStore.book1D("sigmaY",                        "sigmaY",                        1000,           0.,          0.1);
      pullY_                         = dqmStore.book1D("pullY",                         "pullY",                          500,         -25.,         +25.);
      residualZ_                     = dqmStore.book1D("residualZ",                     "residualZ",                     2000,          -0.1,        +0.1);
      sigmaZ_                        = dqmStore.book1D("sigmaZ",                        "sigmaZ",                        1000,           0.,          0.1);
      pullZ_                         = dqmStore.book1D("pullZ",                         "pullZ",                          500,         -25.,         +25.);

      residualU1_                    = dqmStore.book1D("residualU1",                    "residualU1",                    2000,          -0.1,        +0.1);
      sigmaU1_                       = dqmStore.book1D("sigmaU1",                       "sigmaU1",                       1000,           0.,          0.1);
      pullU1_                        = dqmStore.book1D("pullU1",                        "pullU1",                         500,         -25.,         +25.);
      residualU2_                    = dqmStore.book1D("residualU2",                    "residualU2",                    2000,          -0.1,        +0.1);
      sigmaU2_                       = dqmStore.book1D("sigmaU2",                       "sigmaU2",                       1000,           0.,          0.1);
      pullU2_                        = dqmStore.book1D("pullU2",                        "pullU2",                         500,         -25.,         +25.);
      residualU3_                    = dqmStore.book1D("residualU3",                    "residualU3",                    2000,          -0.1,        +0.1);
      sigmaU3_                       = dqmStore.book1D("sigmaU3",                       "sigmaU3",                       1000,           0.,          0.1);
      pullU3_                        = dqmStore.book1D("pullU3",                        "pullU3",                         500,         -25.,         +25.);

      residualV1_                    = dqmStore.book1D("residualV1",                    "residualV1",                    2000,          -0.1,        +0.1);
      sigmaV1_                       = dqmStore.book1D("sigmaV1",                       "sigmaV1",                       1000,           0.,          0.1);
      pullV1_                        = dqmStore.book1D("pullV1",                        "pullV1",                         500,         -25.,         +25.);
      residualV2_                    = dqmStore.book1D("residualV2",                    "residualV2",                    2000,          -0.1,        +0.1);
      sigmaV2_                       = dqmStore.book1D("sigmaV2",                       "sigmaV2",                       1000,           0.,          0.1);
      pullV2_                        = dqmStore.book1D("pullV2",                        "pullV2",                         500,         -25.,         +25.);
      residualV3_                    = dqmStore.book1D("residualV3",                    "residualV3",                    2000,          -0.1,        +0.1);
      sigmaV3_                       = dqmStore.book1D("sigmaV3",                       "sigmaV3",                       1000,           0.,          0.1);
      pullV3_                        = dqmStore.book1D("pullV3",                        "pullV3",                         500,         -25.,         +25.);

      pull2d_U_                      = dqmStore.book1D("pull2d_U",                      "pull2d_U",                       250,           0.,          25.);
      pull2d_V_                      = dqmStore.book1D("pull2d_V",                      "pull2d_V",                       250,           0.,          25.);
      pull3d_                        = dqmStore.book1D("pull3d",                        "pull3d",                        1000,           0.,         100.);

      EigenValue1_                   = dqmStore.book1D("EigenValue1",                   "EigenValue1",                   2000,          -1.0,        +1.0); 
      EigenValue2_                   = dqmStore.book1D("EigenValue2",                   "EigenValue2",                   2000,          -1.0,        +1.0); 
      EigenValue3_                   = dqmStore.book1D("EigenValue3",                   "EigenValue3",                   2000,          -1.0,        +1.0); 
      
      angleEigenVector1wrtU3_        = dqmStore.book1D("angleEigenVector1wrtU3",        "angleEigenVector1wrtU3",         180,           0.,  TMath::Pi());
      angleEigenVector2wrtU3_        = dqmStore.book1D("angleEigenVector2wrtU3",        "angleEigenVector2wrtU3",         180,           0.,  TMath::Pi());
      angleEigenVector3wrtU3_        = dqmStore.book1D("angleEigenVector3wrtU3",        "angleEigenVector3wrtU3",         180,           0.,  TMath::Pi());      

      angleEigenVector1wrtV3_        = dqmStore.book1D("angleEigenVector1wrtV3",        "angleEigenVector1wrtV3",         180,           0.,  TMath::Pi());
      angleEigenVector2wrtV3_        = dqmStore.book1D("angleEigenVector2wrtV3",        "angleEigenVector2wrtV3",         180,           0.,  TMath::Pi());
      angleEigenVector3wrtV3_        = dqmStore.book1D("angleEigenVector3wrtV3",        "angleEigenVector3wrtV3",         180,           0.,  TMath::Pi());      

      genTauDecayDistance_           = dqmStore.book1D("genTauDecayDistance",           "genTauDecayDistance",           2000,           0.,          10.);
      genTauDecayDistanceNormalized_ = dqmStore.book1D("genTauDecayDistanceNormalized", "genTauDecayDistanceNormalized",  250,           0.,          25.);
      genTauDecayDistanceVsP_        = dqmStore.book2D("genTauDecayDistanceVsP",        "genTauDecayDistanceVsP",         100,           0.,         500., 250, 0., 2.5);      
      genTauDecayDistanceVsPhi_      = dqmStore.book2D("genTauDecayDistanceVsPhi",      "genTauDecayDistanceVsPhi",        36, -TMath::Pi(), +TMath::Pi(), 250, 0., 2.5);
    }

    void fillHistograms(const reco::Candidate::LorentzVector& genTauP4,
			const AlgebraicVector3& genProdVertexPos, const AlgebraicVector3& genDecayVertexPos, 
			double recLeadTrackPt, double recLeadTrackEta, 
			const AlgebraicVector3& recDecayVertexPos, const AlgebraicMatrix33& recDecayVertexCov,
			double evtWeight)
    {
      if ( recLeadTrackPt  > ptMin_  && (recLeadTrackPt  < ptMax_ || ptMax_ < 0.) &&
	   recLeadTrackEta > etaMin_ &&  recLeadTrackEta < etaMax_ ) {
	AlgebraicVector3 residual = recDecayVertexPos - genDecayVertexPos;

	double residualX = residual(0);
	double residualY = residual(1);
	double residualZ = residual(2);

	fillHistograms1d(residualX_, sigmaX_, pullX_, residualX, TMath::Sqrt(recDecayVertexCov(0, 0)), evtWeight);
	fillHistograms1d(residualY_, sigmaY_, pullY_, residualY, TMath::Sqrt(recDecayVertexCov(1, 1)), evtWeight);
	fillHistograms1d(residualZ_, sigmaZ_, pullZ_, residualZ, TMath::Sqrt(recDecayVertexCov(2, 2)), evtWeight);

	AlgebraicVector3 direction_U = genDecayVertexPos - genProdVertexPos;
	//printVector("direction(U)", direction_U);

	AlgebraicVector3 u1, u2, u3;
	compLocalCoordinates(direction_U, u1, u2, u3); 

	AlgebraicVector3 residual_U = transformToLocalCoordinatesPos(residual, u1, u2, u3); 
	double residualU1 = residual_U(0);
	double residualU2 = residual_U(1);
	double residualU3 = residual_U(2);

	AlgebraicMatrix33 recDecayVertexCov_U = transformToLocalCoordinatesCov(recDecayVertexCov, u1, u2, u3); 

	fillHistograms1d(residualU1_, sigmaU1_, pullU1_, residualU1, TMath::Sqrt(recDecayVertexCov_U(0, 0)), evtWeight);
	fillHistograms1d(residualU2_, sigmaU2_, pullU2_, residualU2, TMath::Sqrt(recDecayVertexCov_U(1, 1)), evtWeight);
	fillHistograms1d(residualU3_, sigmaU3_, pullU3_, residualU3, TMath::Sqrt(recDecayVertexCov_U(2, 2)), evtWeight);

	pull2d_U_->Fill(pull2d(residual_U, recDecayVertexCov_U), evtWeight);

	AlgebraicVector3 direction_V;
	direction_V(0) = direction_U(0);
	direction_V(1) = direction_U(1);
	direction_V(2) = 0.;
	//printVector("direction(V)", direction_V);

	AlgebraicVector3 v1, v2, v3;
	compLocalCoordinates(direction_V, v1, v2, v3); 

	AlgebraicVector3 residual_V = transformToLocalCoordinatesPos(residual, v1, v2, v3); 
	double residualV1 = residual_V(0);
	double residualV2 = residual_V(1);
	double residualV3 = residual_V(2);

	AlgebraicMatrix33 recDecayVertexCov_V = transformToLocalCoordinatesCov(recDecayVertexCov, v1, v2, v3); 

	fillHistograms1d(residualV1_, sigmaV1_, pullV1_, residualV1, TMath::Sqrt(recDecayVertexCov_V(0, 0)), evtWeight);
	fillHistograms1d(residualV2_, sigmaV2_, pullV2_, residualV2, TMath::Sqrt(recDecayVertexCov_V(1, 1)), evtWeight);
	fillHistograms1d(residualV3_, sigmaV3_, pullV3_, residualV3, TMath::Sqrt(recDecayVertexCov_V(2, 2)), evtWeight);

	pull2d_V_->Fill(pull2d(residual_V, recDecayVertexCov_V), evtWeight);

	pull3d_->Fill(pull3d(residual, recDecayVertexCov), evtWeight);

	TMatrixD covMatrix(3, 3);
	for ( int iRow = 0; iRow < 3; ++iRow ) {
	  for ( int iColumn = 0; iColumn < 3; ++iColumn ) {
	    covMatrix(iRow, iColumn) = recDecayVertexCov(iRow, iColumn);
	  }
	}

	//std::cout << "<plotEntryType1::fillHistograms>:" << std::endl;
	//printVector("residual", residual);
	//printMatrix("recDecayVertexCov", recDecayVertexCov);
	//std::cout << "pull2d(U) = " << pull2d(residual_U, recDecayVertexCov_U) << std::endl;
	//std::cout << "pull3d = " << pull3d(residual, recDecayVertexCov) << std::endl;

	TVectorD EigenValues;
	TMatrixD EigenVectors = covMatrix.EigenVectors(EigenValues);

	EigenValue1_->Fill(EigenValues(0), evtWeight);
	EigenValue2_->Fill(EigenValues(1), evtWeight);
	EigenValue3_->Fill(EigenValues(2), evtWeight);
	
	AlgebraicVector3 EigenVector1, EigenVector2, EigenVector3;
	for ( int i = 0; i < 3; ++i ) {
	  EigenVector1(i) = EigenVectors(i, 0);
	  EigenVector2(i) = EigenVectors(i, 1);
	  EigenVector3(i) = EigenVectors(i, 2);
	}
	
	angleEigenVector1wrtU3_->Fill(angle3d(EigenVector1, u3), evtWeight);
	angleEigenVector2wrtU3_->Fill(angle3d(EigenVector2, u3), evtWeight);
	angleEigenVector3wrtU3_->Fill(angle3d(EigenVector3, u3), evtWeight);
	
	angleEigenVector1wrtV3_->Fill(angle3d(EigenVector1, v3), evtWeight);
	angleEigenVector2wrtV3_->Fill(angle3d(EigenVector2, v3), evtWeight);
	angleEigenVector3wrtV3_->Fill(angle3d(EigenVector3, v3), evtWeight);

	//printVector("EigenVector1", EigenVector1);
	//std::cout << "EigenValue1 = " << EigenValues(0) << std::endl;
	//printVector("cov*EigenVector1", recDecayVertexCov*EigenVector1);
	//printVector("EigenVector2", EigenVector2);
	//std::cout << "EigenValue2 = " << EigenValues(1) << std::endl;
	//printVector("cov*EigenVector2", recDecayVertexCov*EigenVector2);
	//printVector("EigenVector3", EigenVector3);
	//std::cout << "EigenValue3 = " << EigenValues(2) << std::endl;
	//printVector("cov*EigenVector3", recDecayVertexCov*EigenVector3);

	double genTauDecayDistance = TMath::Sqrt(norm2(genDecayVertexPos - genProdVertexPos));
	genTauDecayDistance_->Fill(genTauDecayDistance, evtWeight);
	double a = (genTauP4.P()/tauLeptonMass)*cTauLifetime;
	if ( a > 0. ) genTauDecayDistanceNormalized_->Fill(genTauDecayDistance/a, evtWeight);
	genTauDecayDistanceVsP_->Fill(genTauP4.P(), genTauDecayDistance, evtWeight);
	genTauDecayDistanceVsPhi_->Fill(genTauP4.Phi(), genTauDecayDistance, evtWeight);
      }
    }

    std::string dqmDirectory_;
    double ptMin_;
    double ptMax_;
    double etaMin_;
    double etaMax_;

    MonitorElement* residualX_;
    MonitorElement* sigmaX_;
    MonitorElement* pullX_;
    MonitorElement* residualY_;
    MonitorElement* sigmaY_;
    MonitorElement* pullY_;
    MonitorElement* residualZ_;
    MonitorElement* sigmaZ_;
    MonitorElement* pullZ_;
    
    MonitorElement* residualU1_;
    MonitorElement* sigmaU1_;
    MonitorElement* pullU1_;
    MonitorElement* residualU2_;
    MonitorElement* sigmaU2_;
    MonitorElement* pullU2_;
    MonitorElement* residualU3_;
    MonitorElement* sigmaU3_;
    MonitorElement* pullU3_;

    MonitorElement* residualV1_;
    MonitorElement* sigmaV1_;
    MonitorElement* pullV1_;
    MonitorElement* residualV2_;
    MonitorElement* sigmaV2_;
    MonitorElement* pullV2_;
    MonitorElement* residualV3_;
    MonitorElement* sigmaV3_;
    MonitorElement* pullV3_;

    MonitorElement* pull2d_U_;
    MonitorElement* pull2d_V_;
    MonitorElement* pull3d_;

    MonitorElement* EigenValue1_;
    MonitorElement* EigenValue2_;
    MonitorElement* EigenValue3_;
      
    MonitorElement* angleEigenVector1wrtU3_;
    MonitorElement* angleEigenVector2wrtU3_;
    MonitorElement* angleEigenVector3wrtU3_;

    MonitorElement* angleEigenVector1wrtV3_;
    MonitorElement* angleEigenVector2wrtV3_;
    MonitorElement* angleEigenVector3wrtV3_;

    MonitorElement* genTauDecayDistance_;
    MonitorElement* genTauDecayDistanceNormalized_;
    MonitorElement* genTauDecayDistanceVsP_;
    MonitorElement* genTauDecayDistanceVsPhi_;
  };

  std::vector<plotEntryType1*> plotEntries1Prong_;
  std::vector<plotEntryType1*> plotEntries3Prong_;

  struct plotEntryType2
  {
    plotEntryType2(const std::string& dqmDirectory, bool doRefit, const NSVfitEventVertexRefitter* prodVertexFitAlgorithm)
      : doRefit_(doRefit),
	prodVertexFitAlgorithm_(prodVertexFitAlgorithm)
    {
      dqmDirectory_ = dqmDirectory;
    }
    ~plotEntryType2() {}

    void bookHistograms(DQMStore& dqmStore)
    {
      dqmStore.setCurrentFolder(dqmDirectory_.data());

      residualX_                        = dqmStore.book1D("residualX",                        "residualX",                        2000,  -0.1,   +0.1);
      sigmaX_                           = dqmStore.book1D("sigmaX",                           "sigmaX",                           1000,   0.,     0.1);
      pullX_                            = dqmStore.book1D("pullX",                            "pullX",                             500, -25.,   +25.);
      residualY_                        = dqmStore.book1D("residualY",                        "residualY",                        2000,  -0.1,   +0.1);
      sigmaY_                           = dqmStore.book1D("sigmaY",                           "sigmaY",                           1000,   0.,     0.1);
      pullY_                            = dqmStore.book1D("pullY",                            "pullY",                             500, -25.,   +25.);
      residualZ_                        = dqmStore.book1D("residualZ",                        "residualZ",                        2000,  -0.1,   +0.1);
      sigmaZ_                           = dqmStore.book1D("sigmaZ",                           "sigmaZ",                           1000,   0.,     0.1);
      pullZ_                            = dqmStore.book1D("pullZ",                            "pullZ",                             500, -25.,   +25.);

      pull2d_                           = dqmStore.book1D("pull2d",                           "pull2d",                            250,   0.,    25.);
      pull3d_                           = dqmStore.book1D("pull3d",                           "pull3d",                           1000,   0.,   100.);
      
      residualXvsGenVertexWrtBeamSpotX_ = dqmStore.book2D("residualXvsGenVertexWrtBeamSpotX", "residualXvsGenVertexWrtBeamSpotX",  200,  -0.01,  +0.01, 200, -0.01, +0.01);
      residualYvsGenVertexWrtBeamSpotY_ = dqmStore.book2D("residualYvsGenVertexWrtBeamSpotY", "residualYvsGenVertexWrtBeamSpotY",  200,  -0.01,  +0.01, 200, -0.01, +0.01);
    }

    void fillHistograms(const AlgebraicVector3& genProdVertexPos, 
			const reco::Vertex& recProdVertex, 
			const std::vector<const reco::Track*>& tauTracks,
			const reco::BeamSpot* beamSpot, 
			double evtWeight)
    {
      AlgebraicVector3 recProdVertexPos;
      AlgebraicMatrix33 recProdVertexCov;
      if ( doRefit_ ) {
	TransientVertex recProdVertex_refitted = prodVertexFitAlgorithm_->refit(&recProdVertex, &tauTracks);
	recProdVertexPos = AlgebraicVector3(recProdVertex_refitted.position().x(), recProdVertex_refitted.position().y(), recProdVertex_refitted.position().z());
	recProdVertexCov = recProdVertex_refitted.positionError().matrix_new();
      } else {
	recProdVertexPos = AlgebraicVector3(recProdVertex.position().x(), recProdVertex.position().y(), recProdVertex.position().z());
	for ( int iRow = 0; iRow < 3; ++iRow ) {
	  for ( int iColumn = 0; iColumn < 3; ++iColumn ) {
	    recProdVertexCov(iRow, iColumn) = recProdVertex.covariance()(iRow, iColumn);
	  }
	}
      }

      AlgebraicVector3 residual = genProdVertexPos - recProdVertexPos;

      double residualX = residual(0);
      double residualY = residual(1);
      double residualZ = residual(2);
      
      fillHistograms1d(residualX_, sigmaX_, pullX_, residualX, TMath::Sqrt(recProdVertexCov(0, 0)), evtWeight);
      fillHistograms1d(residualY_, sigmaY_, pullY_, residualY, TMath::Sqrt(recProdVertexCov(1, 1)), evtWeight);
      fillHistograms1d(residualZ_, sigmaZ_, pullZ_, residualZ, TMath::Sqrt(recProdVertexCov(2, 2)), evtWeight);

      pull2d_->Fill(pull2d(residual, recProdVertexCov), evtWeight);
      pull3d_->Fill(pull3d(residual, recProdVertexCov), evtWeight);

      //std::cout << "<plotEntryType2::fillHistograms>:" << std::endl;
      //printVector("residual", residual);
      //printMatrix("recProdVertexCov", recProdVertexCov);
      //std::cout << "pull2d = " << pull2d(residual, recProdVertexCov) << std::endl;
      //std::cout << "pull3d = " << pull3d(residual, recProdVertexCov) << std::endl;

      residualXvsGenVertexWrtBeamSpotX_->Fill(residualX, genProdVertexPos(0) - beamSpot->position().x(), evtWeight);
      residualYvsGenVertexWrtBeamSpotY_->Fill(residualY, genProdVertexPos(1) - beamSpot->position().y(), evtWeight);
    }

    std::string dqmDirectory_;

    bool doRefit_;
    const NSVfitEventVertexRefitter* prodVertexFitAlgorithm_;

    MonitorElement* residualX_;
    MonitorElement* sigmaX_;
    MonitorElement* pullX_;
    MonitorElement* residualY_;
    MonitorElement* sigmaY_;
    MonitorElement* pullY_;
    MonitorElement* residualZ_;
    MonitorElement* sigmaZ_;
    MonitorElement* pullZ_;
        
    MonitorElement* pull2d_;
    MonitorElement* pull3d_;

    MonitorElement* residualXvsGenVertexWrtBeamSpotX_;
    MonitorElement* residualYvsGenVertexWrtBeamSpotY_;
  };

  plotEntryType2* plotEntryProdVertex_woBSoriginal_;
  plotEntryType2* plotEntryProdVertex_wBSoriginal_;

  plotEntryType2* plotEntryProdVertex_woBSrefitted_;
  plotEntryType2* plotEntryProdVertex_wBSrefitted_;
  
  MonitorElement* residualBeamSpotX_;
  MonitorElement* sigmaBeamSpotX_;
  MonitorElement* pullBeamSpotX_;
  MonitorElement* residualBeamSpotY_;
  MonitorElement* sigmaBeamSpotY_;
  MonitorElement* pullBeamSpotY_;
  MonitorElement* residualBeamSpotZ_;
  MonitorElement* sigmaBeamSpotZ_;
  MonitorElement* pullBeamSpotZ_;

  MonitorElement* pullBeamSpot2d_;
  MonitorElement* pullBeamSpot3d_;

  MonitorElement* genTauDecayDistanceNormalized1vs2_;
};

#endif   
