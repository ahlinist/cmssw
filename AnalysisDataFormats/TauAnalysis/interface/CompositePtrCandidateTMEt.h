#ifndef __AnalysisDataFormats_TauAnalysis_CompositeRefCandidateTMEt_h__
#define __AnalysisDataFormats_TauAnalysis_CompositeRefCandidateTMEt_h__

/** \class CompositeRefCandidateTMEt
 *
 * Combination of visible tau decay products with missing transverse momentum 
 * (representing the undetected momentum carried away 
 *  the neutrino produced in a W --> tau nu decay and the neutrinos produced in the tau decay)
 * 
 * \authors Christian Veelken
 *
 * \version $Revision: 1.1 $
 *
 * $Id: CompositePtrCandidateTMEt.h,v 1.1 2009/07/09 12:14:42 veelken Exp $
 *
 */

#include "DataFormats/Candidate/interface/CandidateFwd.h" 
#include "DataFormats/Candidate/interface/Candidate.h" 
#include "DataFormats/Candidate/interface/LeafCandidate.h" 
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/Common/interface/Ptr.h"

template<typename T>
class CompositePtrCandidateTMEt : public reco::LeafCandidate 
{
  typedef edm::Ptr<T> TPtr;
  typedef edm::Ptr<reco::MET> MEtPtr;

 public:

  /// default constructor
  CompositePtrCandidateTMEt() {}

  /// constructor with MEt
  CompositePtrCandidateTMEt(const TPtr visDecayProducts, const reco::CandidatePtr met)
    : visDecayProducts_(visDecayProducts), 
      met_(met) 
  {}

  /// destructor
  ~CompositePtrCandidateTMEt() {}

  /// access to daughter particles
  const TPtr visDecayProducts() const { return visDecayProducts_; }
 
  /// access to missing transverse momentum
  const MEtPtr& met() const { return met_; }

  /// get transverse mass of visible decay products + missing transverse momentum
  double mt() const { return mt_; }

  /// get acoplanarity angle (angle in transverse plane) between visible decay products 
  /// and missing transverse momentum 
  double dPhi() const { return dPhi_; } 

  /// clone  object
  CompositePtrCandidateTMEt<T>* clone() const { return new CompositePtrCandidateTMEt<T>(*this); }

 private:
  
  /// allow only CompositePtrCandidateTMEtAlgorithm to change values of data-members
  template<typename T_type> friend class CompositePtrCandidateTMEtAlgorithm; 

  /// set transverse mass of visible decay products + missing transverse momentum
  void setMt(double mt) { mt_ = mt; }
  /// set acoplanarity angle (angle in transverse plane) between visible decay products 
  /// and missing transverse momentum
  void setDPhi(double dPhi) { dPhi_ = dPhi; }

  /// references/pointers to decay products and missing transverse momentum
  TPtr visDecayProducts_;
  MEtPtr met_;

  /// transverse mass of visible decay products + missing transverse momentum
  double mt_;
  /// acoplanarity angle (angle in transverse plane) between visible decay products
  /// and missing transverse momentum
  double dPhi_;
};

#include "DataFormats/PatCandidates/interface/Tau.h"

typedef CompositePtrCandidateTMEt<pat::Tau> PATTauNuPair;

#endif
