#ifndef ISOLATEDPIONEXTRACTOR_H_
#define ISOLATEDPIONEXTRACTOR_H_

#include "RecoParticleFlow/PFAnalyses/interface/ParticleSearch.h"
#include "RecoParticleFlow/PFAnalyses/interface/EventDelegate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFSimParticleFwd.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include <vector>
#include <TH1.h>
#include <TH2.h>
#include <TF1.h>
#include <ostream>

namespace pftools {

struct RelativeIsolation {
	unsigned r1Index_;
	unsigned r2Index_;
	double dR12_;

	RelativeIsolation(unsigned r1, unsigned r2, double dR) :
		r1Index_(r1), r2Index_(r2), dR12_(dR) {
	}

};

bool operator<(const pftools::RelativeIsolation& a, const pftools::RelativeIsolation& b);

std::ostream& operator<<(std::ostream& s, const pftools::RelativeIsolation& ri);

/**
 *
 * @class IsolatedParticleExtractor
 * @brief This class looks for isolated pions in the event.
 *
 * @author Jamie Ballin
 * @date April 2009
 */
class IsolatedParticleExtractor: public pftools::ParticleSearch {
public:
	IsolatedParticleExtractor();
	virtual ~IsolatedParticleExtractor();

	/**
	 * Look for isolated PFCandidates in the event.
	 * @param event
	 * @param setup
	 * @param possible candidates to consider
	 * @return indices into the supplied candidate collection
	 */
	virtual std::vector<unsigned> findCandidates(const edm::Event& event,
			const edm::EventSetup& setup,
			const reco::PFCandidateCollection& cands);

	virtual std::vector<unsigned> findCandidates(const edm::Event& event,
			const edm::EventSetup& setup,
			const reco::PFCandidateCollection& cands, const reco::PFSimParticleCollection& sims);


	virtual void init(const edm::ParameterSet& parameters);

private:

	/**
	 * Isolation required in the ECAL
	 */
	double deltaREcalIsolation_;

	/**
	 * Go into detailed investigation mode...
	 */
	bool investigateIsolation_;

	bool allowNeutralContamination_;

	std::string neutralContaminationEquation_;

	TF1* neutralContamFunc_;

	edm::Service<TFileService> fileservice_;
	TH1F* pionCount_;
	TH1F* pionsSelected_;
	TH1F* pionNeighbours_;
	TH1F* pionSeparation_;
	TH1F* pionEnergy_;
	TH1F* pionSelectedEnergy_;
	TH1F* pionSelectedEta_;
	TH2F* pionSimEnergyContamination_;
	TH2F* pi0SimContrib_;
	TH2F* pionEnergySep_;
	TH2F* pionEnergyContamination_;
	TH2F* pionTypeContamination_;

	/**
	 * Returns a map of candidates ranked by their relative isolation. The key is the index into
	 * the PFCandidateCollection, the value is the deltaR between that Candidate and its nearest neighbour
	 * @param cands
	 * @return
	 */
	std::vector<pftools::RelativeIsolation> rankCandidatesBySeparation(
			const std::vector<unsigned>& sourceCands, const std::vector<
					unsigned>& targetCands,
			const reco::PFCandidateCollection& cands);

	void estimateContamination(const std::vector<unsigned> candIndices,
			const reco::PFCandidateCollection& cands,
			const reco::PFSimParticleCollection& sims);

};

}

#endif /* ISOLATEDPIONEXTRACTOR_H_ */





