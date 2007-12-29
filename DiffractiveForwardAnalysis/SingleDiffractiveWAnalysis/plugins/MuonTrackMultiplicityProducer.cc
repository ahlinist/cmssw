#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

class MuonTrackMultiplicityProducer : public edm::EDProducer {
public:
  MuonTrackMultiplicityProducer( const edm::ParameterSet & );
private:
  void produce( edm::Event &, const edm::EventSetup & );
  edm::InputTag muonsLabel_;
  edm::InputTag tracksLabel_;	

  double trketamin_;
  double trketamax_;	
};

#include "FWCore/ParameterSet/interface/ParameterSetfwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Common/interface/Handle.h"

#include "DataFormats/Common/interface/AssociationVector.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h"

#include <vector>

MuonTrackMultiplicityProducer::MuonTrackMultiplicityProducer(const edm::ParameterSet& conf) {
	muonsLabel_=conf.getParameter<edm::InputTag>("MuonsLabel");
	tracksLabel_=conf.getParameter<edm::InputTag>("TracksLabel");

	trketamin_ = conf.getUntrackedParameter<double>("TracksEtaMin",0.0);
	trketamax_ = conf.getUntrackedParameter<double>("TracksEtaMax",5.0);

	produces<edm::AssociationVector<reco::CandidateRefProd,std::vector<unsigned int> > >();
}

void MuonTrackMultiplicityProducer::produce(edm::Event& evt, const edm::EventSetup& iSetup) {
	//Muons
	edm::Handle<reco::CandidateCollection> muonsColl;
        evt.getByLabel(muonsLabel_,muonsColl);

	//Tracks
	edm::Handle<reco::CandidateCollection> tracksColl;  
	evt.getByLabel(tracksLabel_,tracksColl);

	reco::CandidateRefProd ref(muonsColl);
	std::auto_ptr<edm::AssociationVector<reco::CandidateRefProd,std::vector<unsigned int> > > trackMultiplicities(new edm::AssociationVector<reco::CandidateRefProd,std::vector<unsigned int> >(ref));
	//edm::AssociationVector<reco::CandidateRefProd,std::vector<unsigned int> > trackMultiplicities(reco::CandidateRefProd(muonsColl));
	size_t i_muon = 0;
	for(reco::CandidateCollection::const_iterator muon = muonsColl->begin(); muon != muonsColl->end(); muon++) {
		if(!muon->hasMasterClone()) throw edm::Exception(edm::errors::Configuration,"MuonTrackMultiplicityProducerError") << ">>>>>>>> Must use muon shallow clones";

		const reco::Muon *mu = dynamic_cast<const reco::Muon*>(&*muon->masterClone());
		reco::TrackRef muctftrack = mu->track();

		unsigned int ntracks = 0;
		bool foundTrack = false;
		for(reco::CandidateCollection::const_iterator track = tracksColl->begin(); track != tracksColl->end(); track++) {
			const reco::RecoChargedCandidate &chgcand = dynamic_cast<const reco::RecoChargedCandidate&>(*track);
                        reco::TrackRef trackref = chgcand.track();
			if(muctftrack == trackref){
				LogTrace("") << ">>>>>>> Found Muon Track!\n";
				foundTrack = true;
				continue;
			}
			if((fabs(track->eta()) >= trketamin_)&&(fabs(track->eta()) <= trketamax_)){
				ntracks++;
			}
		}
		trackMultiplicities->setValue(i_muon++, ntracks);
		//trackMultiplicities[i_muon++] = ntracks;
	}
	evt.put(trackMultiplicities);
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(MuonTrackMultiplicityProducer);

