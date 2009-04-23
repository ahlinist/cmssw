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

/**
 * @class CandidateConverter
 * @brief Dumps PFCandidate information into a form to be stored in a Calibratable object
 *
 * Use this class to extract PFRecHit information and PFCluster information from a PFCandidate
 *
 * @author Jamie Ballin
 * @date April 2009
 */
class CandidateConverter {
public:
	/**
	 * Constructor
	 * @param debug_ Higher values => more output
	 * @return
	 */
	CandidateConverter(unsigned debug_ = 0);
	virtual ~CandidateConverter();

	/**
	 * Boils the information contained in a PFCandidate into the simple CandidateWrapper
	 * @param cand
	 * @return pftools::CandidateWrapper - simple wrapper class of essential
	 * PFCandidate information
	 */
	virtual pftools::CandidateWrapper convert(const reco::PFCandidate& cand);

	/**
	 * Loops over the cluster block elements attached to the PFCandidate and makes
	 * a vector of CalibratableElements for each of the ecal and hcal. The energy
	 * of these elements is determined by fractional energy contribution of each rechit
	 * in the cluster. (i.e. each rechit doesn't necessarily contribute all its energy
	 * to a cluster).
	 * @param cand
	 * @param ecal vector of CalibratableElement to fill
	 * @param hcal ditto
	 */
	virtual void extractRechitsFromCandidate(const reco::PFCandidate& cand,
			std::vector<CalibratableElement>& ecal, std::vector<
					CalibratableElement>& hcal);

	/**
	 * Loops over cluster block elements attached tothe PFCandidate and makes a vector
	 * of CalibratableElements for each of the ecal and hcal.
	 * @param cand
	 * @param ecal vector of CalibratableElement to fill
	 * @param hcal ditto
	 */
	virtual void extractClustersFromCandidate(const reco::PFCandidate& cand,
			std::vector<CalibratableElement>& ecal, std::vector<
					CalibratableElement>& hcal);

private:
	unsigned debug_;
};

}

#endif /* CANDIDATETOWRAPPER_H_ */
