/*
 * CandidateConverter.h
 *
 *  Created on: 22-Apr-2009
 *      Author: jamie
 */

#ifndef CANDIDATETOWRAPPER_H_
#define CANDIDATETOWRAPPER_H_

#include "DataFormats/ParticleFlowReco/interface/Calibratable.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "FWCore/Framework/interface/Event.h"

#include <vector>

namespace pftools {

class CandidateConverter {
public:
	CandidateConverter(unsigned debug_ = 0);
	virtual ~CandidateConverter();

	virtual pftools::CandidateWrapper convert(const reco::PFCandidate& cand);

	virtual void extractRechitsFromCandidate(const reco::PFCandidate& cand,
			std::vector<CalibratableElement>& ecal, std::vector<
					CalibratableElement>& hcal);

	virtual void extractClustersFromCandidate(const reco::PFCandidate& cand,
			std::vector<CalibratableElement>& ecal, std::vector<
					CalibratableElement>& hcal);

private:
	unsigned debug_;
};

}

#endif /* CANDIDATETOWRAPPER_H_ */
