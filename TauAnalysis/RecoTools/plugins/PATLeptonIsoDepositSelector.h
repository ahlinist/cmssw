#ifndef TauAnalysis_RecoTools_PATLeptonIsoDepositSelector_h
#define TauAnalysis_RecoTools_PATLeptonIsoDepositSelector_h

/** \class PATLeptonIsoDepositSelector
 *
 * Flexible selection of pat::Leptons based on IsoDeposits;
 * a pat::Electron, pat::Muon or pat::Tau passed the selection
 * if the sum of deposits of the speficied type and with Pt > ptMin 
 * in an annulus within dRvetoCone (inner cone) and dRisoCone (outer cone)
 * is less than sumPtMax and the number of such deposits is less than numMax
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.2 $
 *
 * $Id: PATLeptonIsoDepositSelector.h,v 1.2 2009/03/12 10:47:20 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Handle.h"

#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"
#include "DataFormats/PatCandidates/interface/Isolation.h"

#include <vector>
#include <string>

template <class T>
class PATLeptonIsoDepositSelector
{
 public:
  typedef std::vector<T> collection;

  explicit PATLeptonIsoDepositSelector(const edm::ParameterSet&);
  ~PATLeptonIsoDepositSelector();

  typename std::vector<const T*>::const_iterator begin() const { return selected_.begin(); }
  typename std::vector<const T*>::const_iterator end() const { return selected_.end(); }

  void select(const edm::Handle<collection>&, const edm::Event&, const edm::EventSetup&);

  size_t size() const { return selected_.size(); }

 private:
  std::vector<const T*> selected_;

  pat::IsolationKeys isoDepositType_;

  reco::isodeposit::AbsVetos isoParam_;

  double dRisoCone_;

  double sumPtMin_;
  int numMin_;
  double sumPtMax_;
  int numMax_;
  
  int cfgError_;
};

#endif
