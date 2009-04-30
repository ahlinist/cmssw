#include "RecoParticleFlow/PFAnalyses/interface/IsolatedParticleExtractor.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include <iostream>

#include "RecoParticleFlow/PFAnalyses/interface/operations.h"

#include <algorithm>

using namespace pftools;
using namespace reco;
using namespace std;

IsolatedParticleExtractor::IsolatedParticleExtractor() {
	LogDebug("IsolatedParticleExtractor") << __PRETTY_FUNCTION__ << std::endl;
	pionCount_ = fileservice_->make<TH1F> ("pionCount", "Number of #pi", 100,
			0, 500);
	pionsSelected_ = fileservice_->make<TH1F> ("pionSelected",
			"Number of isolated #pi", 50, 0, 50);
	pionNeighbours_ = fileservice_->make<TH1F> ("pionNeighbours",
			"Number of candidates within #Delta R cut", 51, -1, 50);
	pionSeparation_ = fileservice_->make<TH1F> ("pionSeparation",
			"pionSeparation", 100, 0, 2.0);
	pionEnergy_ = fileservice_->make<TH1F> ("pionEnergy",
			"E_{#pi} for all #pi", 100, 0, 50);
	pionSelectedEnergy_ = fileservice_->make<TH1F> ("pionSelectedEnergy",
			"E_{#pi} for isolated #pi", 100, 0, 50);
	pionSelectedEta_ = fileservice_->make<TH1F> ("pionSelectedEta",
			"#eta for isolated #pi;#eta", 20, 0, 3.0);
	pionSimEnergyContamination_ = fileservice_->make<TH2F> (
			"pionSimEnergyContamination",
			"E_{sim} vs. E_{#pi reco};E_{#pi reco};E_{sim}/E_{#pi reco}", 50,
			0, 50, 20, 0, 2);
	pi0SimContrib_
			= fileservice_->make<TH2F> (
					"pi0SimContrib",
					"E_{#pi^{0}, #gamma} from sim;E_{#pi reco}; E_{#pi^{0},#gamma from sim}",
					50, 0, 50, 20, 0, 10);
	pionEnergySep_ = fileservice_->make<TH2F> ("pionEnergySep",
			"pionEnergySep;Isolated #pi E;#Delta R nearest neighbour", 100, 0,
			50, 100, 0, 2.0);
	pionEnergyContamination_ = fileservice_->make<TH2F> (
			"pionEnergyContamination",
			"pionEnergyContamination; #pi E; E nearest neighbour", 100, 0, 50,
			100, 0, 50);
	pionTypeContamination_ = fileservice_->make<TH2F> ("pionTypeContamination",
			"pionTypeContamination;#pi E; Type of nearest neighbour", 100, 0,
			50, 7, 0, 7);

}

IsolatedParticleExtractor::~IsolatedParticleExtractor() {
	LogDebug("IsolatedParticleExtractor") << __PRETTY_FUNCTION__ << std::endl;

}

vector<unsigned> IsolatedParticleExtractor::findCandidates(
		const edm::Event& event, const edm::EventSetup& setup,
		const reco::PFCandidateCollection& cands,
		const reco::PFSimParticleCollection& sims) {
	vector<unsigned> isolates = findCandidates(event, setup, cands);

	estimateContamination(isolates, cands, sims);
	return isolates;

}

vector<unsigned> IsolatedParticleExtractor::findCandidates(
		const edm::Event& event, const edm::EventSetup& setup,
		const reco::PFCandidateCollection& cands) {
	vector<unsigned> answers;

	LogDebug("IsolatedParticleExtractor") << __PRETTY_FUNCTION__ << std::endl;
	//Find list of isolated particles, blah

	unsigned eligibles(0);
	vector<unsigned> all;
	for (unsigned p(0); p < cands.size(); ++p) {
		all.push_back(p);
		if (cands[p].particleId() == PFCandidate::h) {
			++eligibles;
			//If there are no objects within deltaREcalIsolation, we're ok
			vector<unsigned> neighbours = pftools::findCandidatesInDeltaRECAL(
					cands, cands[p].positionAtECALEntrance().eta(),
					cands[p].positionAtECALEntrance().phi(),
					deltaREcalIsolation_);
			//this should at least find 1 object - i.e. the candidate itself
			if (neighbours.size() < 2) {
				answers.push_back(p);
				//Energy of selected pion candidate
				pionSelectedEnergy_->Fill(cands[p].energy());
				pionSelectedEta_->Fill(cands[p].positionAtECALEntrance().eta());
			}
			//How many neighbours?
			pionNeighbours_->Fill(neighbours.size() - 1);
			//Energy of all pion candidates
			pionEnergy_->Fill(cands[p].energy());
		}
	}
	//How many pions were there in that event?
	pionCount_->Fill(eligibles);
	vector<RelativeIsolation> isolates = rankCandidatesBySeparation(answers,
			all, cands);
	//Now, for each isolated candidate,
	vector<RelativeIsolation>::const_iterator rit = isolates.begin();
	for (; rit != isolates.end(); ++rit) {
		const PFCandidate& primary = cands[(*rit).r1Index_];
		const PFCandidate& contam = cands[(*rit).r2Index_];
		pionEnergyContamination_->Fill(primary.energy(), contam.energy());
		pionTypeContamination_->Fill(primary.energy(), contam.particleId());
		pionEnergySep_->Fill(primary.energy(), (*rit).dR12_);
	}

	//How many pions found in this event matching criteria?
	pionsSelected_->Fill(answers.size());
	cout << "Found " << answers.size() << " candidates to work on, from "
			<< eligibles << " available\n";

	return answers;
}

void IsolatedParticleExtractor::estimateContamination(const std::vector<
		unsigned> candIndices, const reco::PFCandidateCollection& cands,
		const reco::PFSimParticleCollection& sims) {

	vector<unsigned>::const_iterator cit = candIndices.begin();

	for (; cit != candIndices.end(); ++cit) {
		const PFCandidate& cand = cands[*cit];
		PFSimParticleCollection::const_iterator sit = sims.begin();
		double simEnergyInCone(0.0);
		double candEta = cand.positionAtECALEntrance().eta();
		double candPhi = cand.positionAtECALEntrance().phi();

		cout << "\tPion reco type " << cand.particleId() << ", energy "
				<< cand.energy() << ", eta/phi " << candEta << ", " << candPhi
				<< ":\n";

		for (; sit != sims.end(); ++sit) {
			const PFSimParticle& sim = *sit;
			double
					simEta =
							sim.trajectoryPoint(PFTrajectoryPoint::ECALEntrance).positionREP().eta();
			double
					simPhi =
							sim.trajectoryPoint(PFTrajectoryPoint::ECALEntrance).positionREP().phi();
			double sep = pftools::deltaR(candEta, simEta, candPhi, simPhi);
			if (sep < deltaREcalIsolation_) {
				double simE = sim.trajectoryPoint(
						PFTrajectoryPoint::ECALEntrance).momentum().E();
				cout << "\t\tSim energy " << simE << " eta/phi " << simEta
						<< ", " << simPhi << " pdg " << sim.pdgCode() << "\n";
				//Sim particle contribution
				simEnergyInCone += simE;
				if (sim.pdgCode() == 111 || sim.pdgCode() == 22) {
					//It's a pi0
					pi0SimContrib_ ->Fill(cand.energy(), simE);
				}
			}
		}
		pionSimEnergyContamination_->Fill(cand.energy(), simEnergyInCone
				/ cand.energy());
	}

}

vector<RelativeIsolation> IsolatedParticleExtractor::rankCandidatesBySeparation(
		const std::vector<unsigned>& sourceCands,
		const std::vector<unsigned>& targetCands,
		const PFCandidateCollection& cands) {

	vector<RelativeIsolation> isolates;

	vector<unsigned>::const_iterator cit = sourceCands.begin();

	for (; cit != sourceCands.end(); ++cit) {
		const PFCandidate& cand = cands[*cit];

		//Test each candidate's separation against this current value
		double smallestDR(10000);

		//The winning index in the target vector
		unsigned matchingCandidate(0);
		double thisEta = cand.positionAtECALEntrance().eta();
		double thisPhi = cand.positionAtECALEntrance().phi();

		vector<unsigned>::const_iterator target = targetCands.begin();

		for (; target != targetCands.end(); ++target) {
			double testDR = pftools::deltaR(thisEta, cands[*target].positionAtECALEntrance().eta(),
					thisPhi, cands[*target].positionAtECALEntrance().phi());
			if (testDR < smallestDR && *cit != *target) {
				//This candidate is further away
				smallestDR = testDR;
				//Set the furthest
				matchingCandidate = *target;
			}
		}

		RelativeIsolation iso(*cit, matchingCandidate, smallestDR);
		isolates.push_back(iso);

	}

	sort(isolates.rbegin(), isolates.rend());
	//	cout << "\t";
	//	copy(isolates.begin(), isolates.end(), ostream_iterator<RelativeIsolation>(cout,"\t"));
	//	cout << "\n";

	return isolates;
}

void IsolatedParticleExtractor::init(const edm::ParameterSet& parameters) {
	LogDebug("IsolatedParticleExtractor") << __PRETTY_FUNCTION__ << std::endl;
	deltaREcalIsolation_ = parameters.getParameter<double> (
			"DeltaREcalIsolation");
}

