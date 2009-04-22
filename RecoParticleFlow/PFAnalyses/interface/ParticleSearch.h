/*
 * ParticleSearch.h
 *
 *  Created on: 22-Apr-2009
 *      Author: jamie
 */

#ifndef PARTICLESEARCH_H_
#define PARTICLESEARCH_H_

#include "RecoParticleFlow/PFAnalyses/interface/Initialisable.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"

namespace pftools {
class ParticleSearch : public pftools::Initialisable {
public:
	ParticleSearch();
	virtual ~ParticleSearch();

	virtual std::vector<unsigned> findCandidates(const edm::Event& event,
			const edm::EventSetup& setup, const reco::PFCandidateCollection& cands) = 0;

};
}

#endif /* PARTICLESEARCH_H_ */
