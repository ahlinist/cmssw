#include "TauAnalysis/RecoTools/plugins/TrackAndTrajectoryDowngrade.h"

#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "TrackingTools/PatternTools/interface/Trajectory.h"
#include "TrackingTools/PatternTools/interface/TrajTrackAssociation.h"

#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/RefProd.h"

typedef std::vector<Trajectory> TrajectoryCollection;

TrackAndTrajectoryDowngrade::TrackAndTrajectoryDowngrade(const edm::ParameterSet& cfg)
  : src_(cfg.getParameter<edm::InputTag>("src")),
    pDowngrade_(cfg.getParameter<double>("pDowngrade"))
{
  produces<reco::TrackCollection>();
  produces<TrajectoryCollection>();
  produces<TrajTrackAssociationCollection>();
}
    
void TrackAndTrajectoryDowngrade::produce(edm::Event& evt, const edm::EventSetup& es) 
{
  std::auto_ptr<reco::TrackCollection> outputTracks(new reco::TrackCollection);
  std::auto_ptr<TrajectoryCollection> outputTrajectories(new TrajectoryCollection);
  std::auto_ptr<TrajTrackAssociationCollection> outputAssociations(new TrajTrackAssociationCollection);

  edm::Handle<reco::TrackCollection> inputTracks;
  evt.getByLabel(src_, inputTracks);

  edm::Handle<TrajectoryCollection> inputTrajectories;
  evt.getByLabel(src_, inputTrajectories);
  edm::Handle<TrajTrackAssociationCollection> inputAssociations;
  evt.getByLabel(src_, inputAssociations);

  edm::RefProd<reco::TrackCollection> outputTracksRefProd = evt.getRefBeforePut<reco::TrackCollection>();
  edm::RefProd<TrajectoryCollection> outputTrajectoriesRefProd = evt.getRefBeforePut<TrajectoryCollection>();

  unsigned numTracks = inputTrajectories->size();
  for ( unsigned iTrack = 0; iTrack < numTracks; ++iTrack ) {
    if ( u_.Rndm() > pDowngrade_ ) {
      edm::Ref<TrajectoryCollection> inputTrajectoryRef(inputTrajectories, iTrack);

      TrajTrackAssociationCollection::const_iterator inputAssociation = inputAssociations->find(inputTrajectoryRef);
      if ( inputAssociation != inputAssociations->end() ) {
	const edm::Ref<reco::TrackCollection>& inputTrackRef = inputAssociation->val;

	outputTracks->push_back(*inputTrackRef);
	outputTrajectories->push_back(*inputTrajectoryRef);

	edm::Ref<reco::TrackCollection> outputTrackRef(outputTracksRefProd, outputTracks->size() - 1);
	edm::Ref<TrajectoryCollection> outputTrajectoryRef(outputTrajectoriesRefProd, outputTrajectories->size() - 1);
	outputAssociations->insert(outputTrajectoryRef, outputTrackRef);
      }
    }
  }

  evt.put(outputTracks);
  evt.put(outputTrajectories);
  evt.put(outputAssociations);
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(TrackAndTrajectoryDowngrade);
