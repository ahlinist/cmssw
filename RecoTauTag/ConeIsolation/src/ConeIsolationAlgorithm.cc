
#include "RecoTauTag/ConeIsolation/interface/ConeIsolationAlgorithm.h"

ConeIsolationAlgorithm::ConeIsolationAlgorithm()
{
  m_nthTrack         = 3;
  m_cutPixelHits     = 2;
  m_cutTotalHits     = 8;
  m_cutMaxTIP        = 1.;
  m_cutMinPt         = 1.;
  m_cutMaxDecayLen   = 8.;
  m_cutMaxChiSquared = 5.;
  matching_cone      =  0.1;
  signal_cone  =  0.07;
  isolation_cone = 0.45;
  pt_min_isolation = 1.;
  pt_min_leadTrack = 6.;
  dZ_vertex        = 0.2;
  n_tracks_isolation_ring = 0;


}

ConeIsolationAlgorithm::ConeIsolationAlgorithm(const ParameterSet & parameters)
{
  //FIXME: use unsigned int where needed
  m_nthTrack         = parameters.getParameter<int>("NthTrack");
  m_cutPixelHits     = parameters.getParameter<int>("MinimumNumberOfPixelHits");
  m_cutTotalHits     = parameters.getParameter<int>("MinimumNumberOfHits");
  m_cutMaxTIP        = parameters.getParameter<double>("MaximumTransverseImpactParameter");
  m_cutMinPt         = parameters.getParameter<double>("MinimumTransverseMomentum");
  m_cutMaxDecayLen   = parameters.getParameter<double>("MaximumDecayLength");
  m_cutMaxChiSquared = parameters.getParameter<double>("MaximumChiSquared");
  matching_cone      = parameters.getParameter<double>("MatchingCone");
  signal_cone        = parameters.getParameter<double>("SignalCone");
  isolation_cone     = parameters.getParameter<double>("IsolationCone"); 
  pt_min_isolation   = parameters.getParameter<double>("MinimumTransverseMomentumInIsolationRing"); 
  pt_min_leadTrack   = parameters.getParameter<double>("MinimumTransverseMomentumLeadingTrack"); 
  dZ_vertex          = parameters.getParameter<double>("DeltaZetTrackVertex"); 
  n_tracks_isolation_ring = parameters.getParameter<int>("MaximumNumberOfTracksIsolationRing"); 

}
// SimpleRecTrackFilter * theSRTF = new SimpleRecTrackFilter(ptmin,0.,2.6,tipmax,170.,ntothits);
  // I hope this is not leaking too much. Btw, that constructor shouldn't be used!
//  theFilter.add(theSRTF);
//  theFilter.add( new PixelHitsRecTrackFilter(npixhits));
edm::Ref<reco::TrackCollection> ConeIsolationAlgorithm::leadingTrack(GlobalVector dir, edm::RefVector<reco::TrackCollection> myTracks, float z_pv)
{
  math::XYZVector jet3Vec(dir.x(),dir.y(),dir.z()) ;
  double pt_min_tk = 0.;
  edm::Ref<reco::TrackCollection> leadTk;
  edm::Ref<reco::TrackCollection> tmpTk;

  for(edm::RefVector<reco::TrackCollection>::const_iterator it=myTracks.begin() ; it!=myTracks.end(); it++ )
        {
	  const Track & track = **it;
	  math::XYZVector trackMomentum = track.momentum() ;
	  double deltaR = ROOT::Math::VectorUtil::DeltaR(jet3Vec,trackMomentum ) ;
	  if(deltaR < matching_cone && track.pt() > m_cutMinPt)
	    {
	      if(track.pt() > pt_min_tk && fabs(track.dz() - z_pv) < dZ_vertex)
		
		{
		  leadTk = *it;
		  pt_min_tk = track.pt();
		}
	    }
	}
  if(pt_min_tk > pt_min_leadTrack) {
    return leadTk;
  }else{
    return tmpTk; //Check if this is working in the right way
  }
}
pair<JetTag,IsolatedTauTagInfo> ConeIsolationAlgorithm::tag(const  JetTracksAssociationRef & jetTracks, const Vertex & pv) 
{
 GlobalVector direction(jetTracks->key->px(),jetTracks->key->py(),jetTracks->key->pz());
 edm::RefVector<reco::TrackCollection> tracks=jetTracks->val;

 //find LeadingTrack
 edm::Ref<reco::TrackCollection> leadTk = leadingTrack(direction,tracks, pv.z());
 int nTracksIsolationRing=0;
 if(leadTk) {
   math::XYZVector leadTkMomentum = leadTk->momentum();
   
  
   //Find number of tracksInsideIsolationRing
   for(edm::RefVector<reco::TrackCollection>::const_iterator it=tracks.begin() ; it!=tracks.end(); it++ )
     {
       const Track & track = **it;
       if( track.pt() > m_cutMinPt && track.d0() < m_cutMaxTIP && fabs(track.dz() - pv.z())<  dZ_vertex)
	 {
	   math::XYZVector trackMomentum = track.momentum() ;
	   double deltaR = ROOT::Math::VectorUtil::DeltaR(leadTkMomentum,trackMomentum ) ;
	   if(deltaR < isolation_cone && deltaR > signal_cone)
	     nTracksIsolationRing++;
	 }
	     
     }
 }
 
 double discriminator=0;
 if(leadTk) {
   if(nTracksIsolationRing < n_tracks_isolation_ring+1) discriminator=1;
 }

JetTag resultBase(discriminator,jetTracks);
IsolatedTauTagInfo resultExtended;

return pair<JetTag,IsolatedTauTagInfo> (resultBase,resultExtended); 
}


