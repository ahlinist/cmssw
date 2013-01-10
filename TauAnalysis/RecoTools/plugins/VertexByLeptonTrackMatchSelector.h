#ifndef TauAnalysis_RecoTools_VertexByLeptonTrackMatchSelector_h
#define TauAnalysis_RecoTools_VertexByLeptonTrackMatchSelector_h

/** \class VertexByLeptonTrackMatchSelector
 *
 * Select "the" primary event vertex
 * by requiring vertex to be matched to tracks of given collection of leptons
 * 
 * \author Christian Veelken, LLR
 *
 * \version $Revision: 1.1 $
 *
 * $Id: VertexByLeptonTrackMatchSelector.h,v 1.1 2011/04/09 10:49:01 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/VertexReco/interface/Vertex.h"

#include <vector>

class VertexByLeptonTrackMatchSelectorImp
{
 public:

  typedef std::vector<reco::Vertex> collection;

  VertexByLeptonTrackMatchSelectorImp(const edm::ParameterSet&);

  std::vector<const reco::Vertex*>::const_iterator begin() const { return selected_.begin(); }
  std::vector<const reco::Vertex*>::const_iterator end() const { return selected_.end(); }

  void select(const edm::Handle<collection>&, edm::Event&, const edm::EventSetup&);

  size_t size() const { return selected_.size(); }

 private:

  std::vector<const reco::Vertex*> selected_;

  typedef std::vector<edm::InputTag> vInputTag;
  vInputTag srcLeptons_;

  edm::InputTag srcTrackToVertexAssociations_;

  double dZ_;
  double minAssocProb_;

  int verbosity_;
};

#endif
