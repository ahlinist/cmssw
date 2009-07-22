#ifndef PARTICLEFILTER_H_
#define PARTICLEFILTER_H_

#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "RecoParticleFlow/PFAnalyses/interface/ParticleFiltrationDelegate.h"

#include "DataFormats/ParticleFlowReco/interface/ParticleFiltrationDecision.h"

/**
 * @class ParticleFilter
 * @brief Checks testbeam vetos etc to ensure event contains isolated particles
 *
 * Workhorse used is a ParticleFiltrationDelegate
 *
 * The class used is determined at runtime from
 * 	 "ParticleFiltrationDelegateType"
 * from the edm::ParameterSet.
 *
 * See ParticleFiltrationDelegate documentation for new details.
 *
 * Note this class PRODUCES a ParticleFiltrationDecision
 *
 * @author Jamie Ballin
 * @date April 2009
 */
class ParticleFilter: public edm::EDFilter {

public:
	explicit ParticleFilter(const edm::ParameterSet&);
	virtual ~ParticleFilter();

private:
	virtual void beginJob(const edm::EventSetup&);
	/**
	 * Returns true if the ParticleFiltrationDelegate thinks the event was worthy of further study
	 * False otherwise
	 * Specific details of the decision are stored in the event as a ParticleFiltrationDecision object
	 *
	 * @param bool useful or not?
	 */
	virtual bool filter(edm::Event&, const edm::EventSetup&);
	virtual void endJob();

	pftools::ParticleFiltrationDelegate* pfd_;


	std::string pfdType_;
	std::string tbfdType_;

};

#endif /* PARTICLEFILTER_H_ */

