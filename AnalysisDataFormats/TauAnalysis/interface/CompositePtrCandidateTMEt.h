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
 * \version $Revision: 1.2 $
 *
 * $Id: CompositePtrCandidateTMEt.h,v 1.2 2011/02/19 13:31:59 veelken Exp $
 *
 */

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Candidate/interface/CandidateFwd.h" 
#include "DataFormats/Candidate/interface/Candidate.h" 
#include "DataFormats/Candidate/interface/LeafCandidate.h" 
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/Common/interface/Ptr.h"

#include "AnalysisDataFormats/TauAnalysis/interface/SVfitWtauNuSolution.h"
#include "AnalysisDataFormats/TauAnalysis/interface/SVfitLegSolution.h"

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

  bool hasSVFitSolutions() const { return (svFitSolutionMap_.begin() != svFitSolutionMap_.end()); }
  const SVfitWtauNuSolution* svFitSolution(const std::string& algorithm, int* errorFlag = 0) const
  {
    std::map<std::string, SVfitWtauNuSolution>::const_iterator svFitSolution = svFitSolutionMap_.find(algorithm);
    
    if ( svFitSolution != svFitSolutionMap_.end() ) {
      return &svFitSolution->second;
    } else {      
      if ( errorFlag ) {
	(*errorFlag) = 1;
      } else {
	edm::LogError ("CompositePtrCandidateTMEt::svFitSolution") 
	  << " No SVfit solution defined for algorithm = " << algorithm << " !!";
	std::cout << "available: algorithmName = { " << std::endl;
        bool isFirst = true;
	for ( std::map<std::string, SVfitWtauNuSolution>::const_iterator algorithm = svFitSolutionMap_.begin();
	      algorithm != svFitSolutionMap_.end(); ++algorithm ) {
          if ( !isFirst ) std::cout << ", ";
	  std::cout << algorithm->first;
	  isFirst = false;
	}
	std::cout << " }" << std::endl;
      }

      return 0;
    }
  }

 private:
  
  /// allow only CompositePtrCandidateTMEtAlgorithm to change values of data-members
  template<typename T_type> friend class CompositePtrCandidateTMEtAlgorithm; 

  /// set transverse mass of visible decay products + missing transverse momentum
  void setMt(double mt) { mt_ = mt; }
  /// set acoplanarity angle (angle in transverse plane) between visible decay products 
  /// and missing transverse momentum
  void setDPhi(double dPhi) { dPhi_ = dPhi; }

  void addSVfitSolution(const std::string& algorithm, const SVfitWtauNuSolution& solution)
  {
    svFitSolutionMap_.insert(std::pair<std::string, SVfitWtauNuSolution>(algorithm, solution));
  }

  /// references/pointers to decay products and missing transverse momentum
  TPtr visDecayProducts_;
  MEtPtr met_;

  /// transverse mass of visible decay products + missing transverse momentum
  double mt_;
  /// acoplanarity angle (angle in transverse plane) between visible decay products
  /// and missing transverse momentum
  double dPhi_;

   /// solutions of secondary vertex based mass reconstruction algorithm
  std::map<std::string, SVfitWtauNuSolution> svFitSolutionMap_; // key = algorithmName
};

#include "DataFormats/PatCandidates/interface/Tau.h"

typedef CompositePtrCandidateTMEt<pat::Tau> PATTauNuPair;

#endif
