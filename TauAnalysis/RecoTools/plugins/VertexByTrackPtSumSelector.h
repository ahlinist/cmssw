#ifndef TauAnalysis_RecoTools_VertexByTrackPtSumSelector_h
#define TauAnalysis_RecoTools_VertexByTrackPtSumSelector_h

/** \class VertexByTrackPtSumSelector
 *
 * Auxiliary class for reweighting (pile-up) MC to match 
 * multiplicity distribution of vertices with
 *   sum(trackPt) > threshold
 * observed in the analyzed data sample 
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.4 $
 *
 * $Id: VertexByTrackPtSumSelector.h,v 1.4 2009/02/06 14:19:27 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Handle.h"

#include "DataFormats/VertexReco/interface/Vertex.h"

class VertexByTrackPtSumSelectorImp
{
 public:

  typedef std::vector<reco::Vertex> collection;

  VertexByTrackPtSumSelectorImp(const edm::ParameterSet&);

  std::vector<const reco::Vertex*>::const_iterator begin() const { return selected_.begin(); }
  std::vector<const reco::Vertex*>::const_iterator end() const { return selected_.end(); }

  void select(const edm::Handle<collection>&, edm::Event&, const edm::EventSetup&);

  size_t size() const { return selected_.size(); }

 private:

  std::vector<const reco::Vertex*> selected_;

  double trackPtSumMin_;
};

#endif
