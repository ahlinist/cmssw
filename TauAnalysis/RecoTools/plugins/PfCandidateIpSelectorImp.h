#ifndef TauAnalysis_RecoTools_PfCandidateIpSelectorImp_h
#define TauAnalysis_RecoTools_PfCandidateIpSelectorImp_h

/** \class PfCandidateIpSelectorImp
 *
 * Select charged PFCandidates with specified IP parameters
 * all non-charged candidates are selected by default
 * 
 * \author Jeff Kolb, University of Notre Dame
 *
 */

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

#include <vector>

class PfCandidateIpSelectorImp
{
  public:
   
	typedef std::vector<reco::PFCandidate> collection;
    
	explicit PfCandidateIpSelectorImp(const edm::ParameterSet&);
    ~PfCandidateIpSelectorImp();

    std::vector<const reco::PFCandidate*>::const_iterator begin() const { return selected_.begin(); }
    std::vector<const reco::PFCandidate*>::const_iterator end() const { return selected_.end(); }

    void select(const edm::Handle<collection>&, edm::Event&, const edm::EventSetup&);
    
    size_t size() const { return selected_.size(); }

  private:
    std::vector<const reco::PFCandidate*> selected_;

    edm::InputTag vertexSrc_;

    double rhoMax_;
	bool keepNeutrals_;

};

#endif
