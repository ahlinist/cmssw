

#include "RecoTauTag/ConeIsolation/interface/ConeIsolationAlgorithm.h"
using namespace std;
using namespace reco;
using namespace edm;


ConeIsolationAlgorithm::ConeIsolationAlgorithm()
{

}

ConeIsolationAlgorithm::ConeIsolationAlgorithm(const ParameterSet & parameters)
{
  //FIXME: use unsigned int where needed
  m_cutPixelHits     = parameters.getParameter<int>("MinimumNumberOfPixelHits");//not used
  m_cutTotalHits     = parameters.getParameter<int>("MinimumNumberOfHits");
  m_cutMaxTIP        = parameters.getParameter<double>("MaximumTransverseImpactParameter");
  m_cutMinPt         = parameters.getParameter<double>("MinimumTransverseMomentum");
  m_cutMaxChiSquared = parameters.getParameter<double>("MaximumChiSquared");
  dZ_vertex          = parameters.getParameter<double>("DeltaZetTrackVertex");//To be modified
  useVertexConstrain_ = parameters.getParameter<bool>("useVertex");

  matching_cone      = parameters.getParameter<double>("MatchingCone");
  signal_cone        = parameters.getParameter<double>("SignalCone");
  isolation_cone     = parameters.getParameter<double>("IsolationCone"); 
  pt_min_isolation   = parameters.getParameter<double>("MinimumTransverseMomentumInIsolationRing"); 
  pt_min_leadTrack   = parameters.getParameter<double>("MinimumTransverseMomentumLeadingTrack"); 
  n_tracks_isolation_ring = parameters.getParameter<int>("MaximumNumberOfTracksIsolationRing"); 
  
  useFixedSizeIsolationCone = parameters.getParameter<bool>("UseFixedSizeIsolationCone"); 
  variableConeParameter = parameters.getParameter<double>("VariableConeParameter");
}

pair<JetTag,IsolatedTauTagInfo> ConeIsolationAlgorithm::tag(const  JetTracksAssociationRef & jetTracks, const Vertex & pv) 
{

 edm::RefVector<reco::TrackCollection> tracks=jetTracks->val;
 edm::RefVector<reco::TrackCollection> myTracks;

 //Selection of the Tracks
 float z_pv = pv.z();
 edm::RefVector<reco::TrackCollection>::const_iterator it = tracks.begin();
 
for(;it!= tracks.end();it++)
   {
     if((*it)->pt() > m_cutMinPt &&
	(*it)->normalizedChi2() < m_cutMaxChiSquared &&
	fabs((*it)->d0())< m_cutMaxTIP&&
	(*it)->recHitsSize() >= (unsigned int)m_cutTotalHits &&
	(*it)->hitPattern().numberOfValidPixelHits() >= m_cutPixelHits)

       {
	 if(useVertexConstrain_ && z_pv > -500.) {
	   if(fabs((*it)->dz() - z_pv) < dZ_vertex){
	     myTracks.push_back(*it);
	   }
	 }else{
	   myTracks.push_back(*it);

	 }
       }
   }
 IsolatedTauTagInfo resultExtended(myTracks);

 double r_isoCone = isolation_cone;
 double etJet = jetTracks->key->pt();
 if(!useFixedSizeIsolationCone){
   r_isoCone = std::min(isolation_cone, variableConeParameter / etJet);
 }


 //now I can use it for the discriminator;
 math::XYZVector jetDir(jetTracks->key->px(),jetTracks->key->py(),jetTracks->key->pz());   
 double discriminator = 0;
 if(useVertexConstrain_) {
   //In this case all the selected tracks comes from the same vertex, so no need to pass the dZ_vertex requirement to the discriminator 
   discriminator =  resultExtended.discriminator(jetDir, matching_cone, signal_cone, r_isoCone, pt_min_leadTrack, pt_min_isolation,  n_tracks_isolation_ring); 
 }else{
   //In this case the dZ_vertex is used to associate the tracks to the Z_imp parameter of the Leading Track
   discriminator =  resultExtended.discriminator(jetDir, matching_cone, signal_cone, r_isoCone, pt_min_leadTrack, pt_min_isolation,  n_tracks_isolation_ring, dZ_vertex); 
 }
   JetTag resultBase(discriminator,jetTracks);

  return pair<JetTag,IsolatedTauTagInfo> (resultBase,resultExtended); 
}


