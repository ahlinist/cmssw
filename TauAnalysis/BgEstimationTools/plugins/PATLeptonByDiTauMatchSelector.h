#ifndef TauAnalysis_BgEstimationTools_PATLeptonByDiTauMatchSelector_h
#define TauAnalysis_BgEstimationTools_PATLeptonByDiTauMatchSelector_h

/** \class PATLeptonByDiTauMatchSelector
 *
 * Select pat::Electron, pat::Muon and pat::Tau objects
 * matching (by deltaR) leg1, leg2 of diTau objects
 *
 * NOTE: module is Used to compute Event weights for fake-rate technique
 * 
 * \authors Christian Veelken
 *
 * \version $Revision: 1.20 $
 *
 * $Id: CompositePtrCandidateT1T2MEtProducer.h,v 1.20 2011/01/19 10:12:48 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"
#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEtFwd.h"

#include <string>

template<typename T1, typename T2>
class PATLeptonByDiTauMatchSelector : public edm::EDProducer 
{
 public:

  explicit PATLeptonByDiTauMatchSelector(const edm::ParameterSet&);
  ~PATLeptonByDiTauMatchSelector();

  void produce(edm::Event&, const edm::EventSetup&);

 private:

  typedef std::vector<CompositePtrCandidateT1T2MEt<T1,T2> > CompositePtrCandidateCollection; 
  typedef std::vector<T1> leg1Collection;
  typedef std::vector<T2> leg2Collection;

  std::string moduleLabel_;

  edm::InputTag srcDiTau_;

  edm::InputTag srcLeg1_;
  double dRmatchLeg1_;
  bool produceLeg1_;

  edm::InputTag srcLeg2_;
  double dRmatchLeg2_;
  bool produceLeg2_;
};

#endif
