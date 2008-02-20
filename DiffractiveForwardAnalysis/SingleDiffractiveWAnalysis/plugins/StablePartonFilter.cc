#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include <vector>

class StablePartonFilter : public edm::EDFilter {
    public:
       explicit StablePartonFilter(const edm::ParameterSet&);
       ~StablePartonFilter();

       virtual bool filter(edm::Event&, const edm::EventSetup&);
   private:
	edm::InputTag genParticlesTag_;
};

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/Candidate/interface/Candidate.h"

StablePartonFilter::StablePartonFilter(const edm::ParameterSet& pset){
	genParticlesTag_ = pset.getParameter<edm::InputTag>("GenParticlesTag");
}

StablePartonFilter::~StablePartonFilter(){}

bool StablePartonFilter::filter(edm::Event& evt, const edm::EventSetup& es){
	edm::Handle<reco::CandidateCollection> genParticles;
	evt.getByLabel(genParticlesTag_,genParticles);

	std::vector<int> partonIds;
	partonIds.push_back(1);
	partonIds.push_back(2);
	partonIds.push_back(3);
	partonIds.push_back(4);
        partonIds.push_back(5);
        partonIds.push_back(6);
	partonIds.push_back(21);
	bool accept = true;
	for(reco::CandidateCollection::const_iterator part = genParticles->begin(); part != genParticles->end(); ++part){
		for(std::vector<int>::iterator itr = partonIds.begin(); itr != partonIds.end(); ++itr){
			if((abs(part->pdgId()) == *itr)&&(part->status() == 1)){
				 LogTrace("") << "Found stable parton!!!\n";
				 accept = false;
				 break;	
			}
		}	
	}
	//Print event if it was rejected
	if(!accept){
		LogTrace("") << ">>>>>>>>>>>>>>>>>>> Event rejected!\n";
		for(reco::CandidateCollection::const_iterator part = genParticles->begin(); part != genParticles->end(); ++part){
			LogTrace("") << part->pdgId() << "  " << part->status() << "  " << part->px() << "  " << part->py() << "  " << part->pz() << "  " << part->energy() << "\n";
		}
	} else LogTrace("") << ">>>>>>>>>>>>>>>>>>> Event selected!\n";

	return accept;
}

DEFINE_FWK_MODULE(StablePartonFilter);

