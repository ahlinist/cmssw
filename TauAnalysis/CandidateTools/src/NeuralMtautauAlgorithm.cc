#include "TauAnalysis/CandidateTools/interface/NeuralMtautauAlgorithm.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "TauAnalysis/CandidateTools/interface/neuralMtautauAuxFunctions.h"

#include "TMVA/Factory.h"
#include "TMVA/Reader.h"
#include "TMVA/Tools.h"

#include <TMath.h>

NeuralMtautauAlgorithm::NeuralMtautauAlgorithm(const edm::ParameterSet& cfg)
  : NN_(0)
{
  edm::FileInPath inputFileName = cfg.getParameter<edm::FileInPath>("inputFileName");
  if ( !inputFileName.isLocal()) throw cms::Exception("NeuralMtautauAlgorithm") 
    << " Failed to find File = " << inputFileName << " !!\n";
  
  TMVA::Tools::Instance();
  
  TMVA::Reader* NN_ = new TMVA::Reader("!Color:!Silent");   
  
  NN_->AddVariable("recLeg1Px", &recLeg1Px_);
  NN_->AddVariable("recLeg1Py", &recLeg1Py_);
  NN_->AddVariable("recLeg1Pz", &recLeg1Pz_);
  NN_->AddVariable("recLeg2Px", &recLeg2Px_);
  NN_->AddVariable("recLeg2Py", &recLeg2Py_);
  NN_->AddVariable("recLeg2Pz", &recLeg2Pz_);
  NN_->AddVariable("recMEtPx", &recMEtPx_);
  NN_->AddVariable("recMEtPy", &recMEtPy_);
  NN_->AddVariable("TMath::Min(recMEtSigmaX, 1.e+2)", &recMEtSigmaX_);
  NN_->AddVariable("TMath::Min(recMEtSigmaY, 1.e+2)", &recMEtSigmaY_);
  NN_->AddVariable("recMEtCorrXY", &recMEtCorrXY_);
}

NeuralMtautauAlgorithm::~NeuralMtautauAlgorithm()
{
  delete NN_;
}

double NeuralMtautauAlgorithm::operator()(const reco::Candidate::LorentzVector& leg1P4, const reco::Candidate::LorentzVector& leg2P4, 
					  double metPx, double metPy, const TMatrixD& metCov)
{
//--- compute axis 'zeta' bisecting angle between visible decay products of the two tau leptons
  double zetaPhi = compZetaPhi(leg1P4, leg2P4);

  reco::Candidate::LorentzVector leg1P4inZetaFrame = compP4inZetaFrame(leg1P4, zetaPhi);
  reco::Candidate::LorentzVector leg2P4inZetaFrame = compP4inZetaFrame(leg2P4, zetaPhi);
  
  if ( !(metCov.GetNrows() == 2 && metCov.GetNcols() == 2) )
    throw cms::Exception("NeuralMtautauAlgorithm::operator()") 
      << "MET covariance matrix passed as function argument is of invalid format, expect matrix of format 2x2 !!\n";
  assert(metCov.GetNrows() == 2);
  assert(metCov.GetNcols() == 2);
  TMatrixD covInZetaFrame = compCovMatrixInZetaFrame(metCov, zetaPhi);
  double sigmaXinZetaFrame = TMath::Sqrt(TMath::Abs(covInZetaFrame(0,0)));
  double sigmaYinZetaFrame = TMath::Sqrt(TMath::Abs(covInZetaFrame(1,1)));
  const double epsilon = 1.e-9;
  double corrXYinZetaFrame = ( (sigmaXinZetaFrame*sigmaYinZetaFrame) > epsilon ) ?
    covInZetaFrame(0,1)/(sigmaXinZetaFrame*sigmaYinZetaFrame) : 0.;		

//--- set neural net input variables
  recLeg1Px_    = leg1P4.px();
  recLeg1Py_    = leg1P4.py();
  recLeg1Pz_    = leg1P4.pz();
  recLeg2Px_    = leg2P4.px();
  recLeg2Py_    = leg2P4.py();
  recLeg2Pz_    = leg2P4.pz();
  recMEtPx_     = metPx;
  recMEtPy_     = metPy; 
  recMEtSigmaX_ = sigmaXinZetaFrame;
  recMEtSigmaY_ = sigmaYinZetaFrame;
  recMEtCorrXY_ = corrXYinZetaFrame;

//--- compute & return neural net output
  return NN_->EvaluateRegression("trainNeuralMtautau")[0];
}

