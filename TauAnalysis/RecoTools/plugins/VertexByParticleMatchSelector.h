#ifndef TauAnalysis_RecoTools_VertexByParticleMatchSelector_h
#define TauAnalysis_RecoTools_VertexByParticleMatchSelector_h

/** \class VertexByParticleMatchSelector
 *
 * Select collection of vertices associated to given list of particles
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: VertexByParticleSelector.h,v 1.1 2010/11/22 16:17:36 veelken Exp $
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

class VertexByParticleMatchSelectorImp
{
 public:

  typedef std::vector<reco::Vertex> collection;

  VertexByParticleMatchSelectorImp(const edm::ParameterSet&);

  std::vector<const reco::Vertex*>::const_iterator begin() const { return selected_.begin(); }
  std::vector<const reco::Vertex*>::const_iterator end() const { return selected_.end(); }

  void select(const edm::Handle<collection>&, edm::Event&, const edm::EventSetup&);

  size_t size() const { return selected_.size(); }

 private:

  std::vector<const reco::Vertex*> selected_;

  typedef std::vector<edm::InputTag> vInputTag;
  vInputTag srcParticleCollections_;

  double dZ_;
};

#endif
