#ifndef TauAnalysis_CandidateTools_NeuralMtautauAlgorithm_h
#define TauAnalysis_CandidateTools_NeuralMtautauAlgorithm_h

/** \class NeuralMtautauAlgorithm
 *
 * Reconstruct tau+ tau- invariant mass using neural network
 * 
 * \authors Christian Veelken
 *
 * \version $Revision: 1.1 $
 *
 * $Id: NeuralMtautauAlgorithm.h,v 1.1 2012/02/13 13:49:03 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Candidate/interface/Candidate.h"

#include "TMVA/Reader.h"

#include <TMatrixD.h>

#include <string>

class NeuralMtautauAlgorithm
{
 public:

  explicit NeuralMtautauAlgorithm(const edm::ParameterSet&);
  explicit NeuralMtautauAlgorithm(const std::string&);
  ~NeuralMtautauAlgorithm();

  double operator()(const reco::Candidate::LorentzVector&, const reco::Candidate::LorentzVector&, double, double, const TMatrixD&);

 private:

  void initialize(const std::string&);

  TMVA::Reader* mva_;

  // NOTE: these variables need to match the list of variables used for MVA training,
  //       defined in TauAnalysis/CandidateTools/test/trainNeuralMtautau_cfg.py
  Float_t recLeg1Px_;
  Float_t recLeg1Py_;
  Float_t recLeg1Pz_;
  Float_t recLeg2Px_;
  Float_t recLeg2Py_;
  Float_t recLeg2Pz_;
  Float_t recMEtPx_;
  Float_t recMEtPy_;
  Float_t recMEtSigmaX_;
  Float_t recMEtSigmaY_;
  Float_t recMEtCorrXY_;
};

#endif

