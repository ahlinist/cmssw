#ifndef SusyAnalysis_JetEventSelector_h_
#define SusyAnalysis_JetEventSelector_h_
/// Jet selector.
/// 
/// Selects on Et, eta and EM fraction of each jet
///
/// Original author: W. Adam, 10/4/08
///
/// $Id: JetEventSelector.h,v 1.5 2009/01/16 09:18:06 fronga Exp $

// system include files
#include <memory>

// user include files
#include "SusyAnalysis/EventSelector/interface/SusyEventSelector.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "DataFormats/PatCandidates/interface/Jet.h"

#include <vector>
#include <string>

class JetEventSelector : public SusyEventSelector {
public:
  JetEventSelector (const edm::ParameterSet&);
  virtual bool select (const edm::Event&) const;
  virtual ~JetEventSelector () {}
private:
  edm::InputTag jetTag_;       ///< tag for input collection
  std::string corrStep_;       ///< jet correction step (see pat::Jet)
  std::string corrFlavour_;    ///< jet correction flavour (ditto)
  std::vector<double> minPt_;  ///< lower Et cuts (also defines min. #jets)
  std::vector<double> maxEta_; ///< upper |eta| cuts (also defines min. #jets)
  std::vector<double> maxFem_; ///< upper cut on EM fraction

  template <class C>
  struct IndexSorter {
    IndexSorter (const C& values, bool decreasing = true) : 
      values_(values), decrease_(decreasing) {}
    std::vector<size_t> operator() () const {
      std::vector<size_t> result;
      result.reserve(values_.size());
      for ( size_t i=0; i<values_.size(); ++i )  result.push_back(i);
      sort(result.begin(),result.end(),*this);
      return result;
    }
    bool operator() (size_t a, size_t b) {
      if ( decrease_ )
	return values_[a]>values_[b];
      else
	return values_[a]<values_[b];
    }
    const C& values_;
    bool decrease_;
  };

};
#endif
