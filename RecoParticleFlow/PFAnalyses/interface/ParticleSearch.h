
#ifndef PARTICLESEARCH_H_
#define PARTICLESEARCH_H_

#include "RecoParticleFlow/PFAnalyses/interface/Initialisable.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"

namespace pftools {
/**
 * @class ParticleSearch
 * @brief Provides an interface for searching for PFCandidates matching some criteria
 * specified by the implementation class.
 *
 * @note Abstract class.
 *
 *
 * @author Jamie Ballin
 * @date April 2009
 */
class ParticleSearch : public pftools::Initialisable {
public:
	ParticleSearch();
	virtual ~ParticleSearch();

	/**
	 * Look for PFCandidates in the event which match certain criteria
	 * @param event
	 * @param setup
	 * @param possible candidates to consider
	 * @return indices into the supplied candidate collection
	 */
	virtual std::vector<unsigned> findCandidates(const edm::Event& event,
			const edm::EventSetup& setup, const reco::PFCandidateCollection& cands) = 0;

};
}

#endif /* PARTICLESEARCH_H_ */
