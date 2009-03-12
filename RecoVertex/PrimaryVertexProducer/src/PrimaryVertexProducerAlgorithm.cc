#include "RecoVertex/PrimaryVertexProducer/interface/PrimaryVertexProducerAlgorithm.h"
#include "RecoVertex/PrimaryVertexProducer/interface/VertexHigherPtSquared.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"
#include "RecoVertex/AdaptiveVertexFit/interface/AdaptiveVertexFitter.h"
#include "RecoVertex/VertexTools/interface/VertexDistanceXY.h"
#include "RecoVertex/VertexPrimitives/interface/VertexException.h"
#include <algorithm>

using namespace reco;

//
// constructors and destructor
//
PrimaryVertexProducerAlgorithm::PrimaryVertexProducerAlgorithm(const edm::ParameterSet& conf)
  // extract relevant parts of config for components
  : theConfig(conf), 
    theTrackFilter(conf.getParameter<edm::ParameterSet>("TkFilterParameters")), 
    theTrackClusterizer(conf.getParameter<edm::ParameterSet>("TkClusParameters")), 
    theVertexSelector(VertexDistanceXY(), 
		      conf.getParameter<edm::ParameterSet>("PVSelParameters").getParameter<double>("maxDistanceToBeam"))
{
  edm::LogInfo("RecoVertex/PrimaryVertexProducerAlgorithm") 
    << "Initializing PV producer algorithm" << "\n";
  edm::LogInfo("RecoVertex/PrimaryVertexProducerAlgorithm") 
    << "PVSelParameters::maxDistanceToBeam = " 
    << conf.getParameter<edm::ParameterSet>("PVSelParameters").getParameter<double>("maxDistanceToBeam") << "\n";

  fUseBeamConstraint = conf.getParameter<bool>("useBeamConstraint");
  fVerbose           = conf.getUntrackedParameter<bool>("verbose", false);
  std::string algorithm = conf.getParameter<std::string>("algorithm");

  fapply_finder = false;
  if (algorithm == "TrimmedKalmanFinder") {
    fapply_finder = true;
    theFinder.setParameters(conf.getParameter<edm::ParameterSet>("VtxFinderParameters"));
  } else if (algorithm=="KalmanVertexFitter") {
    theFitter=new KalmanVertexFitter();
  } else if( algorithm=="AdaptiveVertexFitter") {
    theFitter=new AdaptiveVertexFitter();
  } else {
    throw VertexException("PrimaryVertexProducerAlgorithm: unknown algorithm: " + algorithm);  
  }

  edm::LogInfo("RecoVertex/PrimaryVertexProducerAlgorithm") 
    << "Using " << algorithm << "\n";
  edm::LogInfo("RecoVertex/PrimaryVertexProducerAlgorithm") 
    << "beam-constraint  " << fUseBeamConstraint << "\n"; 

  edm::LogInfo("RecoVertex/PrimaryVertexProducerAlgorithm") 
    << "PV producer algorithm initialization: done" << "\n";

}


PrimaryVertexProducerAlgorithm::~PrimaryVertexProducerAlgorithm() 
{
  if (theFitter) delete theFitter;
}


//
// member functions
//
vector<TransientVertex> 
PrimaryVertexProducerAlgorithm::vertices(const vector<reco::TransientTrack> & tracks) const
{
  std::cout<< "PrimaryVertexProducer::vertices> Obsolete function, using dummy beamspot " << std::endl;
    reco::BeamSpot dummyBeamSpot;
    dummyBeamSpot.dummy();
    return vertices(tracks,dummyBeamSpot); 
}


vector<TransientVertex> 
PrimaryVertexProducerAlgorithm::vertices(const vector<reco::TransientTrack> & tracks,
					 const reco::BeamSpot & beamSpot) const
{
  
  VertexState beamVertexState(beamSpot);

  if ( fapply_finder) {
        return theFinder.vertices( tracks );
  }
  vector<TransientVertex> pvs;


  // select tracks
  vector<TransientTrack> seltks;

  for (vector<reco::TransientTrack>::const_iterator itk = tracks.begin();
       itk != tracks.end(); itk++) {
    if (theTrackFilter(*itk)) seltks.push_back(*itk);
  }

  if(fVerbose){
    cout << "PrimaryVertexProducerAlgorithm::vertices  selected tracks=" << seltks.size() << endl;
  }

  // clusterize tracks in Z
  vector< vector<reco::TransientTrack> > clusters = 
    theTrackClusterizer.clusterize(seltks);


  // look for primary vertices in each cluster
  vector<TransientVertex> pvCand;
  int nclu=0;
  for (vector< vector<reco::TransientTrack> >::const_iterator iclus
	 = clusters.begin(); iclus != clusters.end(); iclus++) {

    if(fVerbose){
      cout << "PrimaryVertexProducerAlgorithm::vertices  cluster =" 
	   << nclu << "  tracks" << (*iclus).size() << endl;

      std::cout << "cluster tracks " << std::endl;
    }

    if( fUseBeamConstraint &&((*iclus).size()>1) ){
      if (fVerbose){cout <<  "constrained fit with "<< (*iclus).size() << " tracks"  << endl;}
      TransientVertex v = theFitter->vertex(*iclus, beamSpot);
      if (v.isValid()) pvCand.push_back(v);

      if (fVerbose){
	cout << "beamspot   x="<< beamVertexState.position().x() 
	     << " y=" << beamVertexState.position().y()
	     << " z=" << beamVertexState.position().z()
	     << " dx=" << sqrt(beamVertexState.error().cxx())
	     << " dy=" << sqrt(beamVertexState.error().cyy())
	     << " dz=" << sqrt(beamVertexState.error().czz())
	     << std::endl;
	if (v.isValid()) cout << "x,y,z=" << v.position().x() <<" " << v.position().y() << " " <<  v.position().z() << endl;
	else cout <<"Invalid fitted vertex\n";
      }

    }else if((*iclus).size()>1){
      if (fVerbose){cout <<  "unconstrained fit with "<< (*iclus).size() << " tracks"  << endl;}

      TransientVertex v = theFitter->vertex(*iclus); 
      if (v.isValid()) pvCand.push_back(v);

      if (fVerbose){
	if (v.isValid()) cout << "x,y,z=" << v.position().x() <<" " << v.position().y() << " " <<  v.position().z() << endl;
	else cout <<"Invalid fitted vertex\n";
      }

    }else if (fVerbose){
      cout <<  "cluster dropped" << endl;
    }


    nclu++;
  }// end of cluster loop


  if(fVerbose){
    cout << "PrimaryVertexProducerAlgorithm::vertices  candidates =" << pvCand.size() << endl;
  }

  // select vertices compatible with beam
  int npv=0;
  for (vector<TransientVertex>::const_iterator ipv = pvCand.begin();
       ipv != pvCand.end(); ipv++) {
    if(fVerbose){
      cout << "PrimaryVertexProducerAlgorithm::vertices cand " << npv++ << " sel=" <<
	theVertexSelector(*ipv,beamVertexState) << "   z="  << ipv->position().z() << endl;
    }
    if (theVertexSelector(*ipv,beamVertexState)) pvs.push_back(*ipv);
  }

  // sort vertices by pt**2  vertex (aka signal vertex tagging)
  sort(pvs.begin(), pvs.end(), VertexHigherPtSquared());
  

  return pvs;
  
}
