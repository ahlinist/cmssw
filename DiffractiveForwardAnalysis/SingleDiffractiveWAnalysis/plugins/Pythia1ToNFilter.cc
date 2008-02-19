#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include <vector>

class Pythia1ToNFilter : public edm::EDFilter {
    public:
       explicit Pythia1ToNFilter(const edm::ParameterSet&);
       ~Pythia1ToNFilter();

       virtual bool filter(edm::Event&, const edm::EventSetup&);
   private:
	edm::InputTag genParticlesTag_;
	std::vector<int> pdgIDs_;
	std::vector<int> statusCodes_;
};

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/Candidate/interface/Candidate.h"

Pythia1ToNFilter::Pythia1ToNFilter(const edm::ParameterSet& pset){
	genParticlesTag_ = pset.getParameter<edm::InputTag>("GenParticlesTag");
	pdgIDs_ = pset.getParameter<std::vector<int> >("DecayPdgIDs");
	statusCodes_ = pset.getParameter<std::vector<int> >("DecayStatusCodes");
	assert(pdgIDs_.size() == statusCodes_.size());
	LogTrace("") << "Decay is (pdg id,status):\n"
		     << "(" << pdgIDs_[0] << "," << statusCodes_[0] << ") -->";
	for(size_t i = 1; i < pdgIDs_.size(); ++i){
		     LogTrace("") << " + (" << pdgIDs_[i] << "," << statusCodes_[i] << ")\n";
	}    
}

Pythia1ToNFilter::~Pythia1ToNFilter(){}

bool Pythia1ToNFilter::filter(edm::Event& evt, const edm::EventSetup& es){
	edm::Handle<reco::CandidateCollection> genParticles;
	evt.getByLabel(genParticlesTag_,genParticles);

	//Find the mother particle...uses the first one it finds.
	std::vector<const reco::Candidate*> daughters;
	for(reco::CandidateCollection::const_iterator part = genParticles->begin(); part != genParticles->end(); ++part){
		if((part->pdgId() == pdgIDs_[0])&&((part->status() == statusCodes_[0])||(part->status() == -1))){
			//if(part->numberOfDaughters() != pdgIDs_.size()) continue;
			for(size_t idx = 0; idx < part->numberOfDaughters(); ++idx) daughters.push_back(part->daughter(idx));
			break;
		}	
	}
	//Loop over daughters
	std::vector<int> auxvec = pdgIDs_;
	for(size_t i = 1; i < pdgIDs_.size(); ++i){
		for(std::vector<const reco::Candidate*>::iterator dau = daughters.begin(); dau != daughters.end(); ++dau){
			if(((*dau)->pdgId() == pdgIDs_[i])&&(((*dau)->status() == statusCodes_[i])||((*dau)->status() == -1))){
				auxvec.erase(auxvec.begin() + i);
				break;
			}
		}
	}
	
	LogTrace("") << "Missing entries:\n";
	for(size_t i = 1; i < pdgIDs_.size(); ++i) LogTrace("") << "(" << pdgIDs_[i] << "," << statusCodes_[i] << ")\n";
	
	return (auxvec.size() == 1);
}
DEFINE_FWK_MODULE(Pythia1ToNFilter);

