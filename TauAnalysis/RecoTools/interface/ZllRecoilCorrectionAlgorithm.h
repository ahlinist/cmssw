#ifndef TauAnalysis_RecoTools_ZllRecoilCorrectionAlgorithm_h
#define TauAnalysis_RecoTools_ZllRecoilCorrectionAlgorithm_h

/** \class ZllRecoilCorrectionAlgorithm
 *
 * Correct for difference in hadronic recoil between Monte Carlo simulation and Data
 * in Z --> l+ l- events
 * (cf. CMS AN-10-264 for a description of the method)
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.5 $
 *
 * $Id: ZllRecoilCorrectionAlgorithm.h,v 1.5 2011/01/19 10:10:33 veelken Exp $
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/PatCandidates/interface/MET.h"

#include "TauAnalysis/RecoTools/interface/ZllRecoilCorrectionParameterSet.h"

class ZllRecoilCorrectionAlgorithm
{

 public:
  explicit ZllRecoilCorrectionAlgorithm(const edm::ParameterSet&);
  ~ZllRecoilCorrectionAlgorithm();

  pat::MET buildZllCorrectedMEt(const pat::MET&, const reco::Candidate::LorentzVector&, const reco::Candidate::LorentzVector&) const;

 private:

  // MEt resolution parameters for MC/Data
  ZllRecoilCorrectionParameterSet* corrParameterData_;
  ZllRecoilCorrectionParameterSet* corrParameterMC_;

  std::pair<double, double> compSigma(const ZllRecoilCorrectionParameterSet&, double) const;

  double shiftByUncertainty_;
};

#endif


