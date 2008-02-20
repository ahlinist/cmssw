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
	//std::vector<const reco::Candidate*> daughters;
	bool accept = false;
	for(reco::CandidateCollection::const_iterator part = genParticles->begin(); part != genParticles->end(); ++part){
		if((abs(part->pdgId()) == pdgIDs_[0])&&((part->status() == statusCodes_[0])||(statusCodes_[0] == -1))){
			//if(part->numberOfDaughters() != pdgIDs_.size()) continue;
			 LogTrace("") << part->pdgId() << "  " << part->status() << "  "
				      << part->px() << "  " << part->py() << "  "
				      << part->pz() << "  " << part->energy() << "\n";
			std::vector<int> auxpdgIds = pdgIDs_;
			std::vector<int> auxstatusCodes = statusCodes_;
			for(size_t idx = 0; idx < part->numberOfDaughters(); ++idx){
				//daughters.push_back(part->daughter(idx));
				const reco::Candidate* dau = part->daughter(idx);
				LogTrace("") << dau->pdgId() << "  " << dau->status() << "  "
                                             << dau->px() << "  " << dau->py() << "  "
                                             << dau->pz() << "  " << dau->energy() << "\n";
				//Try to find daughter in pdgId vector
				for(size_t k = 1; k < auxpdgIds.size(); ++k){
					if((abs(dau->pdgId()) == auxpdgIds[k])&&((dau->status() == auxstatusCodes[k])||(auxstatusCodes[k] == -1))){
						LogTrace("") << "Found entry (" << auxpdgIds[k] << "," << auxstatusCodes[k] << ")\n";
						auxpdgIds.erase(auxpdgIds.begin() + k);
						auxstatusCodes.erase(auxstatusCodes.begin() + k);
					}
					
				}
			}
			if((auxpdgIds.size() == 1)&&(auxpdgIds[0] == abs(part->pdgId()))){
				accept = true;
				//Print accepted chain
				LogTrace("") << "Accepted 1->N chain:\n"
					     << part->pdgId() << "  " << part->status() << "  "
					     << part->px() << "  " << part->py() << "  "
					     << part->pz() << "  " << part->energy() << "\n";
				for(size_t idx = 0; idx < part->numberOfDaughters(); ++idx){
					const reco::Candidate* dau = part->daughter(idx);
					LogTrace("") << dau->pdgId() << "  " << dau->status() << "  "
						     << dau->px() << "  " << dau->py() << "  "
						     << dau->pz() << "  " << dau->energy() << "\n";
				}		
				break;
			}
		}	
	}
	if(!accept) LogTrace("") << "Event rejected!\n";
	else LogTrace("") << "Event selected!\n";

	return accept;
}

DEFINE_FWK_MODULE(Pythia1ToNFilter);

