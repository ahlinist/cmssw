#include "DataFormats/TrackingRecHit/interface/TrackingRecHitFwd.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"

#include "Calibration/IsolatedParticles/interface/MatchingSimTrack.h"

#include <iostream>

namespace spr{

edm::SimTrackContainer::const_iterator matchedSimTrack(const edm::Event& iEvent, edm::Handle<edm::SimTrackContainer>& SimTk, edm::Handle<edm::SimVertexContainer>& SimVtx, const reco::Track* pTrack, TrackerHitAssociator& associate, bool debug){
  
  edm::SimTrackContainer::const_iterator itr = SimTk->end();;

  edm::SimTrackContainer::const_iterator simTrkItr;
  edm::SimVertexContainer::const_iterator simVtxItr;
 
  //Get the vector of PsimHits associated to TrackerRecHits and select the 
  //matching SimTrack on the basis of maximum occurance of trackIds
  std::vector<unsigned int> trkId, trkOcc;
  int i=0;
  for (trackingRecHit_iterator iTrkHit = pTrack->recHitsBegin(); iTrkHit != pTrack->recHitsEnd(); ++iTrkHit, ++i) {
    
    std::vector<PSimHit> matchedSimIds =  associate.associateHit((**iTrkHit));
    for (unsigned int isim=0; isim<matchedSimIds.size(); isim++) {
      unsigned tkId = matchedSimIds[isim].trackId();
      bool found = false;
      for (unsigned int j=0; j<trkId.size(); j++) {
	if ( tkId == trkId[j] ) {
	  trkOcc[j]++;
	  found = true;
	  break;
	}
      }
      if (!found) { trkId.push_back(tkId); trkOcc.push_back(1); }
    }
  }

  if (debug) {
    std::cout << "Reconstructed Track with " << i << " recHits.";
    for (unsigned int isim=0; isim<trkId.size(); isim++){
      std::cout << "\n trkId " << trkId[isim] << "  Occurance " << trkOcc[isim] << ", ";
    } 
    std::cout << std::endl;
  }
  
  int matchedId=0; 
  unsigned int matchSimTrk=0;
  if (trkOcc.size() > 0) {
    unsigned int maxTrkOcc=0, idxMax=0;
    for(unsigned int j=0; j<trkOcc.size(); j++) {
      if(trkOcc[j] > maxTrkOcc ) { maxTrkOcc = trkOcc[j]; idxMax = j; }
    }
    matchSimTrk = trkId[idxMax];
    for (simTrkItr = SimTk->begin(); simTrkItr!= SimTk->end(); simTrkItr++) {
      if ( simTrkItr->trackId() == matchSimTrk ) {
	matchedId = simTrkItr->type();
	if (debug) std::cout << "matched trackId (maximum occurance) " << matchSimTrk << " type " << matchedId << std::endl;
	itr = simTrkItr;
	break;
      }
    }
  }
  
  if (matchedId==0 && debug) {
    std::cout << "Could not find matched SimTrk and track history now " << std::endl;
  }

  return itr;
}

  //Returns a vector of TrackId originating from the matching SimTrack
std::vector<int> matchedSimTrackId(const edm::Event& iEvent, edm::Handle<edm::SimTrackContainer>& SimTk, edm::Handle<edm::SimVertexContainer>& SimVtx, const reco::Track* pTrack, TrackerHitAssociator& associate, bool debug) {

  // get the matching SimTrack
  edm::SimTrackContainer::const_iterator trkInfo = spr::matchedSimTrack(iEvent, SimTk, SimVtx, pTrack,associate, debug);
  unsigned int matchSimTrk = trkInfo->trackId();
  if (debug) std::cout << "matchedSimTrackId finds the SimTrk ID of the current track to be " << matchSimTrk << std::endl;
  
  std::vector<int> matchTkid;
  if( trkInfo->type() != 0) {
    edm::SimTrackContainer::const_iterator simTrkItr;
    for(simTrkItr = SimTk->begin(); simTrkItr!= SimTk->end(); simTrkItr++){
      if (validSimTrack(matchSimTrk, simTrkItr, SimTk, SimVtx, false))
	matchTkid.push_back((int)simTrkItr->trackId());
    }
  }
  return matchTkid;
}

// Checks if this SimTrack=thisTrkItr originates from the one with trackId=simTkId
bool validSimTrack(unsigned int simTkId, edm::SimTrackContainer::const_iterator thisTrkItr, edm::Handle<edm::SimTrackContainer>& SimTk, edm::Handle<edm::SimVertexContainer>& SimVtx, bool debug){
  
  if (debug) std::cout << "Inside validSimTrack: trackId " << thisTrkItr->trackId() << " vtxIndex " <<  thisTrkItr->vertIndex() << " to be matched to " << simTkId << std::endl;

  //This track originates from simTkId
  if (thisTrkItr->trackId() == simTkId) return true;

  //Otherwise trace back the history using SimTracks and SimVertices
  int vertIndex = thisTrkItr->vertIndex();
  if (vertIndex == -1 || vertIndex >= (int)SimVtx->size()) return false;

  edm::SimVertexContainer::const_iterator simVtxItr= SimVtx->begin();
  for (int iv=0; iv<vertIndex; iv++) simVtxItr++;
  int parent = simVtxItr->parentIndex();
  if (debug) std::cout << "validSimTrack:: parent index " << parent <<" ";
  if (parent < 0 && simVtxItr != SimVtx->begin()) {
    const math::XYZTLorentzVectorD pos1 = simVtxItr->position();
    for (simVtxItr = SimVtx->begin(); simVtxItr != SimVtx->end(); ++simVtxItr) {
      if (simVtxItr->parentIndex() > 0) {
	const math::XYZTLorentzVectorD pos2 = pos1 - simVtxItr->position();
	double dist = pos2.P();
	if (dist < 0.001) {
	  parent = simVtxItr->parentIndex();
	  break;
	}
      }
    }
  }
  if (debug) std::cout << "final index " << parent << std::endl;;
  for(edm::SimTrackContainer::const_iterator simTrkItr= SimTk->begin(); simTrkItr!= SimTk->end(); simTrkItr++){
    if ((int)simTrkItr->trackId() == parent && simTrkItr != thisTrkItr) return  validSimTrack(simTkId, simTrkItr, SimTk, SimVtx, debug) ; 
  }
  
  return false;
  
}

  //Returns the parent of a SimTrack
edm::SimTrackContainer::const_iterator parentSimTrack(edm::SimTrackContainer::const_iterator thisTrkItr, edm::Handle<edm::SimTrackContainer>& SimTk, edm::Handle<edm::SimVertexContainer>& SimVtx, bool debug){
  
  edm::SimTrackContainer::const_iterator itr = SimTk->end();

  int vertIndex = thisTrkItr->vertIndex();
  int type = thisTrkItr->type(); int charge = (int)thisTrkItr->charge();
  if (debug) std::cout << "SimTrackParent " << thisTrkItr->trackId() << " Vertex " <<  vertIndex << " Type " << type << " Charge " << charge << std::endl;

  if( vertIndex == -1 )                      return thisTrkItr;  
  else if (vertIndex >= (int)SimVtx->size()) return itr; 

  edm::SimVertexContainer::const_iterator simVtxItr= SimVtx->begin();
  for (int iv=0; iv<vertIndex; iv++) simVtxItr++;
  int parent = simVtxItr->parentIndex();

  if (parent < 0 && simVtxItr != SimVtx->begin()) {
    const math::XYZTLorentzVectorD pos1 = simVtxItr->position();
    for (simVtxItr = SimVtx->begin(); simVtxItr != SimVtx->end(); ++simVtxItr) {
      if (simVtxItr->parentIndex() > 0) {
	const math::XYZTLorentzVectorD pos2 = pos1 - simVtxItr->position();
	double dist = pos2.P();
	if (dist < 0.001) {
	  parent = simVtxItr->parentIndex();
	  break;
	}
      }
    }
  }
  for (edm::SimTrackContainer::const_iterator simTrkItr= SimTk->begin(); simTrkItr!= SimTk->end(); simTrkItr++){
    if ((int)simTrkItr->trackId() == parent && simTrkItr != thisTrkItr) return  parentSimTrack(simTrkItr, SimTk, SimVtx, debug); 
  }

  return thisTrkItr; 
}


}
