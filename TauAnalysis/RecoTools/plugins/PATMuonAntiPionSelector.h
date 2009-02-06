#ifndef TauAnalysis_RecoTools_PATMuonAntiPionSelector_h
#define TauAnalysis_RecoTools_PATMuonAntiPionSelector_h

/** \class PATMuonAntiPionSelector
 *
 * Select muon that pass linear discriminant based on combination of
 * calorimeter and track segment compatibility,
 * in order to veto pions misidentified as muons
 * 
 * \author Alfredo Gurrola, Texas A&M;
 *  modified by Christian Veelken
 *
 * \version $Revision: 1.1 $
 *
 * $Id: PATMuonAntiPionSelector.h,v 1.1 2009/01/29 13:22:18 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Handle.h"

#include "DataFormats/PatCandidates/interface/Muon.h"

class PATMuonAntiPionSelectorImp
{
 public:

  typedef std::vector<pat::Muon> collection;

  PATMuonAntiPionSelectorImp(const edm::ParameterSet&);

  std::vector<const pat::Muon*>::const_iterator begin() const { return selected_.begin(); }
  std::vector<const pat::Muon*>::const_iterator end() const { return selected_.end(); }

  void select(const edm::Handle<collection>&, edm::Event&, const edm::EventSetup&);

  size_t size() const { return selected_.size(); }

 private:

  std::vector<const pat::Muon*> selected_;

  double coeffCaloComp_;
  double coeffSegmComp_;
  double cut_;
};

#endif
