// -*- C++ -*-
// A. Palma
// Package:     TrackingEfficiencyAnalysis
// Class:       TrackingEfficiencyAnalysis
// To look at the main code: go directly to:  TrackingEfficiencyAnalysis::analyze
// ------------------------------------------------------

#include "SUSYBSMAnalysis/Zprime2eeAnalysis/interface/TrackingEfficiencyAnalysis.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/GeometryVector/interface/GlobalVector.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "SimDataFormats/Vertex/interface/SimVertex.h"
#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"


#include <vector>
 
#define PI 3.141592654
#define TWOPI 6.283185308

using namespace std;


// ------------ constuctor   -------------------------------
TrackingEfficiencyAnalysis::TrackingEfficiencyAnalysis(const edm::ParameterSet& iConfig)
{

  //mytree = new TTree("tree","tr");

  sc_weight = 1.;
 

  trackConeSize_ = iConfig.getParameter<double>("trackConeSize");
  vetoConeSize_ = iConfig.getParameter<double>("vetoConeSize");
  minTagProbeInvMass_ = iConfig.getParameter<double>("minTagProbeInvMass");
  tagEtMin_ = iConfig.getParameter<double>("tagEtMin");
  probeEtMin_ = iConfig.getParameter<double>("probeEtMin");
  zVertexCut_ = iConfig.getParameter<double>("zVertexCut");

  debug=1;
  event_tot=0;
  gsf_sizegreater0=0;
  sc_sizegreater1=0;


}


// ------------ destructor  -------------------------------
TrackingEfficiencyAnalysis::~TrackingEfficiencyAnalysis()
{
}


// ------------ method called once each job just before starting event loop  ------------
void TrackingEfficiencyAnalysis::beginJob(const edm::EventSetup&)
{
  edm::Service<TFileService> fs;

  tagProbeEvents = 0.;
  scWithMatchedTrack = 0.;

  //  HISTO INITIALISATION :
  
  // histos: on all events:

  sccoll_size = fs->make<TH1I>("sccoll_size","sccoll_size",20,0.,20.);
  gsfcoll_size = fs->make<TH1I>("gsfcoll_size","gsfcoll_size",20,0.,20.);
  
  //histos: tag properties
  h_tagCollectionSize = fs->make<TH1F>("h_tagCollectionSize", "h_tagCollectionSize",10,0.,10.);
  h_tagEnergy = fs->make<TH1F>("tagEnergy","tagEnergy",400,0.,800.);
 
  //histos:probe properties
  h_trackPtAroundSCFrac = fs->make<TH1F>("h_trackPtAroundSCFrac", "h_trackPtAroundSCFrac",200,0.,4.);
  h_trackPtAroundSC = fs->make<TH1F>("h_trackPtAroundSC", "h_trackPtAroundSC",400,0.,400.);
  h_TagProbeInvMass = fs->make<TH1F>("h_TagProbeInvMass", "h_TagProbeInvMass",600,0.,600.);

  h_matchedTrackPtoverSCEt = fs->make<TH1F>("h_matchedTrackPtoverSCEt", "h_matchedTrackPtoverSCEt", 200,0.,4.);
  h_minDeltaRTrackSC = fs->make<TH1F>("h_minDeltaRTrackSC", "h_minDeltaRTrackSC",  200, 0., 4. );

  //Define tree
  mytree = fs->make<TTree>("tree","tr");
 
  //Define branches
  // mytree->Branch("sc_Z_iso_m_branch",&sc_Z_iso_m_var,"sc_Z_iso_m_branch/F");
  
}

// Main code to analyse data: look for a high energy e+e- pair
// asking at least one GSF electron + another SC
// ---------------------------------------------------------------------------------------
void TrackingEfficiencyAnalysis::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  using namespace edm;
  using namespace std; 
  using namespace reco;
  
  if (debug)
      {
    LogDebug("thisAnalysis") << "=======================================================" ; 
      LogDebug("thisAnalysis") << "iEvent.id().run() = " << iEvent.id().run() << "  iEvent.id().event() = " << iEvent.id().event()  ; 
      LogDebug("thisAnalysis") << "=======================================================" ; 
  }

  //HepLorentzVector and SuperClusterRef initialisation:

  HepLorentzVector superclusone;
  HepLorentzVector superclustwo;
  HepLorentzVector superclusone_vtxcor;
  HepLorentzVector superclustwo_vtxcor;

  reco::SuperClusterRef hottestobj;
  reco::SuperClusterRef sndhottestobj;
  std::vector<reco::SuperClusterRef>::const_iterator  hottestsc;

  // private variable initialisation:

  //-------Retrieve different collections-------


  edm::Handle<CaloTowerCollection> hhitBarrelHandle;
  //try {iEvent.getByLabel("caloTowers", hhitBarrelHandle);} catch (cms::Exception& ex) { }
  iEvent.getByLabel("towerMaker", hhitBarrelHandle);
  const CaloTowerCollection* towerCollection = hhitBarrelHandle.product();

  //Get Calo Geometry
  edm::ESHandle<CaloGeometry> pG;
  iSetup.get<IdealGeometryRecord>().get(pG);
  const CaloGeometry* caloGeom = pG.product();

  //Reconstructed vertexes
  edm::Handle<reco::VertexCollection> vertices;
  try {iEvent.getByLabel("offlinePrimaryVerticesFromCTFTracks",vertices);} catch (cms::Exception& ex) {
    LogDebug("thisAnalysis")<<"could not found the product with producer offlinePrimaryVerticesFromCTFTracks" << ex ;
 }
  const VertexCollection* VertexData = vertices.product();

  // Get the general tracks
  edm::Handle<reco::TrackCollection> trackHandle;
  try {iEvent.getByLabel("ctfWithMaterialTracks", trackHandle);} catch (cms::Exception& ex) { 
    LogDebug("thisAnalysis")<<"could not found the product with producer ctfWithMaterialTracks" << ex ;
}
  const reco::TrackCollection* trackcoll = trackHandle.product();

  //total number of events on which the code is run:
  event_tot++;
  if( (event_tot%100)  == 0)cout<<"----------------------------------------event # "<<event_tot<<" ------------------------------------------------"<<endl;
  
  // Get PixelMatchGsfElectron Collection -----------------------------------------
  reco::PixelMatchGsfElectronCollection::const_iterator gsfhottest;

  Handle<reco::PixelMatchGsfElectronCollection> pixelmatchelec;
  try{ iEvent.getByLabel("pixelMatchGsfElectrons","",pixelmatchelec); } catch(cms::Exception& ex) {
    LogDebug("thisAnalysis")<<"could not found the product with producer pixelMatchGsfElectrons" << ex ;
  }
  const reco::PixelMatchGsfElectronCollection* collpixelmatchelec = pixelmatchelec.product();
  if(debug) LogDebug("thisAnalysis") << "collpixelmatchelec->size() = " <<  collpixelmatchelec->size() ;

  //Get the hybrid and endcap supercluster collection
  Handle<reco::SuperClusterCollection> pHybridSuperClusters;
  Handle<reco::SuperClusterCollection> pIslandSuperClusters;

  try {
    iEvent.getByLabel("hybridSuperClusters","", pHybridSuperClusters);
    iEvent.getByLabel("islandSuperClusters","islandEndcapSuperClusters", pIslandSuperClusters);
  } catch ( cms::Exception& ex ) {
    LogDebug("thisAnalysis")<<"could not found the product with producer hybridSuperClusters or islandEndcapSuperClusters" << ex ;
  }

  const reco::SuperClusterCollection* hybridSuperClusters = pHybridSuperClusters.product();
  const reco::SuperClusterCollection* islandSuperClusters = pIslandSuperClusters.product();

  //Merge these two collections into one (sclusters collection)
  std::vector<const reco::SuperCluster*> sclusters;
  
  for(reco::SuperClusterCollection::const_iterator it = islandSuperClusters->begin(); it != islandSuperClusters->end(); ++it) {
    sclusters.push_back( &(*it) );
  }

  for(reco::SuperClusterCollection::const_iterator it2 = hybridSuperClusters->begin(); it2 != hybridSuperClusters->end(); ++it2) {
    sclusters.push_back( &(*it2) );
  }
  
  
    LogDebug("thisAnalysis") << "hybridSuperClusters->size() = " <<  hybridSuperClusters->size() ;
    LogDebug("thisAnalysis") << "islandSuperClusters->size() = " <<  islandSuperClusters->size() ;
    LogDebug("thisAnalysis") << "sclusters.size() = " <<  sclusters.size() ;

  gsfcoll_size->Fill(collpixelmatchelec->size() ,sc_weight);
  sccoll_size->Fill(sclusters.size() ,sc_weight);
  

  if(  collpixelmatchelec->size() > 0)  gsf_sizegreater0++;
  

  //if gsf collection size is > 0 and if (other) sc collection size is > 0 ...
  if( (collpixelmatchelec->size() > 0) && (sclusters.size() > 1)  ) {

    sc_sizegreater1++;

    /////////////////////////LOOK FOR TAGS: PIXELMATCHGSFELECTRONS with cuts

    //apply selection cuts to PixelMatchGsfElectrons
    reco::PixelMatchGsfElectronCollection TagCollection;

    for(reco::PixelMatchGsfElectronCollection::const_iterator tagIt = collpixelmatchelec->begin(); tagIt != collpixelmatchelec->end(); ++tagIt) {

      double tagEt = tagIt->energy() * sin(2*atan( exp( -1. * tagIt->eta()) ) );
      double tagAbsEta = fabs( tagIt->eta() ); 

      if( tagAbsEta < 1.444 || (tagAbsEta > 1.560 && tagAbsEta < 2.5) ) //tag ele must be in ECAL fiducial region
	if( tagEt > tagEtMin_ ) //tag ele must have Et above certain threshold
	  TagCollection.push_back( *tagIt );
      
    }
    
    LogDebug("thisAnalysis")<<"Found "<<TagCollection.size()<<" Tag electrons, now looking for probes..."<<endl;
    
    h_tagCollectionSize->Fill( TagCollection.size() );
    

    ////////////////////////////////LOOK FOR PROBES: ECAL SC
    
    //association map: one Tag - many Probes

    std::map<const reco::PixelMatchGsfElectron*, std::vector<const reco::SuperCluster*> > TagProbeMap;

    for(reco::PixelMatchGsfElectronCollection::const_iterator selTagIt = TagCollection.begin(); selTagIt != TagCollection.end(); ++selTagIt){
      
      for( std::vector<const reco::SuperCluster*>::const_iterator scIt_eb = sclusters.begin(); scIt_eb != sclusters.end(); ++scIt_eb) { 
	
	LogDebug("thisAnalysis")<<"SCGsfInvMass( &(*scIt_eb), &(*selTagIt) "<<SCGsfInvMass( *scIt_eb, &(*selTagIt) ) <<endl;
	
	if( SCbelongsToAGsf( *scIt_eb , &(*selTagIt) ) )
	  continue;

	double probeEt = (*scIt_eb)->energy() * sin(2*atan( exp( -1. * (*scIt_eb)->eta()) ) );
	  if( probeEt > probeEtMin_){
	
	    TagProbeMap[&(*selTagIt)].push_back( *scIt_eb );
	    LogDebug("thisAnalysis")<<"Added Probe SC to Tag"<<endl;
	  
	  }
	
      }
	
      }//end of loop over Tags to fill the one-to-many association map    
    
    
    if( TagCollection.size() >0 ){
    
    //////////////////////////////////DUMP TAG&PROBE EVENT ON HISTOS
    
      for(reco::PixelMatchGsfElectronCollection::const_iterator selTagIt = TagCollection.begin(); selTagIt != TagCollection.end(); ++selTagIt){
      
      if( TagProbeMap[&(*selTagIt)].size() == 0 || TagProbeMap[&(*selTagIt)].size() > 1 ) //exclude event if there is more than one probe for the tag 
	continue;

      h_tagEnergy->Fill(selTagIt->energy());
      
      for(std::vector<const reco::SuperCluster*>::const_iterator It = TagProbeMap[&(*selTagIt)].begin(); It != TagProbeMap[&(*selTagIt)].end(); ++It){
	
	Hep3Vector tagVertex = getVertex( &(*selTagIt) );

	if( SCGsfInvMass( *It, &(*selTagIt) ) < minTagProbeInvMass_ )  //Tag-Probe invariant mass cut
	  continue; 
	
	std::pair<int,float> trackisol = SCtrackIsolation( *It,trackcoll, &(*selTagIt) );
	
	LogDebug("thisAnalysis")<<"Probe analysis: # of track in cone: "<<trackisol.first<<" with total pT: "<<trackisol.second<<endl;
	
	h_trackPtAroundSCFrac->Fill( trackisol.second / ( (*It)->energy() * sin(2*atan( exp( -1. * (*It)->eta()))) )  );
	h_trackPtAroundSC->Fill( trackisol.second );
	h_TagProbeInvMass->Fill(SCGsfInvMass( *It, &(*selTagIt) ));

	std::pair<int,float> hasTrack = scHasMatchedTrack(*It,trackcoll, &(*selTagIt) );
	h_matchedTrackPtoverSCEt->Fill( hasTrack.second / ( (*It)->energy() * sin(2*atan( exp( -1. * (*It)->eta()))) )  );
	
	tagProbeEvents++;
	
	h_minDeltaRTrackSC->Fill( closestTrackToSc( *It, trackcoll, &(*selTagIt) )   );

	if(hasTrack.first>=1)
	  scWithMatchedTrack++;
      }
      
    }
      
    }//end loop on Tags 

    
  }//end of if size of collection is > 1

}//end of analyzer




// ------------ method called once each job just after ending the event loop  ------------
void 
TrackingEfficiencyAnalysis::endJob() {

  cout<<"***********Tag-Probe events: "<<tagProbeEvents<<" events with track-matched SC: "<<scWithMatchedTrack<<" Integrated tracking efficiency: "<<scWithMatchedTrack/tagProbeEvents<<endl;

}
//define this as a plug-in
DEFINE_FWK_MODULE(TrackingEfficiencyAnalysis);


////////////////////////////////////////////////////////////MEMBER FUNCTIONS FOR ANALYZER////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------------------------------------------------------------------------

bool TrackingEfficiencyAnalysis::SCbelongsToAGsf(const reco::SuperCluster* sc , const reco::PixelMatchGsfElectron* gsf) {

  bool alreadyUsed = false;

//be careful not to take the same SC of the gsf
  
    HepLorentzVector ele4V, sc4V;
    
    Hep3Vector tagVertex(gsf->gsfTrack()->vertex().x(), gsf->gsfTrack()->vertex().y(), gsf->gsfTrack()->vertex().z());
    double corrScEta = getScCorrectedEta( sc, tagVertex );

    sc4V.setRThetaPhi( sc->energy(), 2*atan( exp( -1. * corrScEta ) ), sc->phi() );
    LogDebug("thisAnalysis") <<"[SCbelongsToAGsf] sc theta:"<<2*atan( exp( -1. * corrScEta ) )<<endl;

    ele4V.setRThetaPhi( gsf->superCluster()->energy(), gsf->theta(), gsf->phi() );
    LogDebug("thisAnalysis") <<"[SCbelongsToAGsf] gsf theta:"<<gsf->theta()<<endl;

    if( ele4V.deltaR(sc4V) < vetoConeSize_ )
      alreadyUsed = true;
  
    return alreadyUsed;
}


//--------------------------------------------------------------------------------------------------------------------------------------------------

double TrackingEfficiencyAnalysis::SCGsfInvMass(const reco::SuperCluster* sc , const reco::PixelMatchGsfElectron* ele) {

    Hep3Vector ele3V, sc3V;

    Hep3Vector tagVertex(ele->gsfTrack()->vertex().x(), ele->gsfTrack()->vertex().y(), ele->gsfTrack()->vertex().z());
    double corrScEta = getScCorrectedEta( sc, tagVertex );

    LogDebug("thisAnalysis") <<"[SCGsfInvMass]sc "<<sc->energy()<<" "<<2*atan( exp( -1. * corrScEta ) )<<" "<<sc->phi()<<endl;
    LogDebug("thisAnalysis") <<"[SCGsfInvMass]ele "<<ele->superCluster()->energy()<<" "<<ele->theta()<<" "<<ele->phi()<<endl;
    
    sc3V.setRThetaPhi( sc->energy(), 2*atan( exp( -1. * corrScEta ) ), sc->phi() );
    ele3V.setRThetaPhi( ele->superCluster()->energy(), ele->theta(), ele->phi() );

    HepLorentzVector ele4V(ele->superCluster()->energy(),ele3V);
    HepLorentzVector sc4V(sc->energy(),sc3V);

    return (ele4V + sc4V).m();

}


//--------------------------------------------------------------------------------------------------------------------------------------------------

std::pair<int,float> TrackingEfficiencyAnalysis::SCtrackIsolation(const reco::SuperCluster* sc, const reco::TrackCollection* trackcoll, const reco::PixelMatchGsfElectron* ele)
{ 
  
  double tagVertexZ = ele->gsfTrack()->vertex().z();
  double trackVertexZ;

  Hep3Vector track3V, sc3V;

  Hep3Vector tagVertex(ele->gsfTrack()->vertex().x(), ele->gsfTrack()->vertex().y(), ele->gsfTrack()->vertex().z());
  double corrScEta = getScCorrectedEta( sc, tagVertex );

  sc3V.setRThetaPhi( sc->energy(), 2*atan( exp( -1. * corrScEta ) ), sc->phi() );
  
  LogDebug("thisAnalysis") <<"[SCtrackIsolation] sc theta:"<<2*atan( exp( -1. * corrScEta ) )<<endl;

  float ptTracksInCone = 0.;
  int nTracksInCone = 0;

  for(reco::TrackCollection::const_iterator trackIt = trackcoll->begin(); trackIt != trackcoll->end(); trackIt++){
    
    trackVertexZ = trackIt->vertex().z();

    track3V.setRThetaPhi( sqrt( trackIt->momentum().mag2() ), trackIt->theta(), trackIt->phi() );
    
    if(track3V.deltaR(sc3V) < trackConeSize_ && track3V.deltaR(sc3V) > vetoConeSize_ ){
     
      LogDebug("thisAnalysis") << "[SCtrackIsolation] deltaZ between sc and track: "<< (trackVertexZ - tagVertexZ) << endl;

      nTracksInCone++;
      ptTracksInCone += track3V.perp();

    }
    

  }
  
  std::pair<int,float> pair;
  pair.first = nTracksInCone;
  pair.second = ptTracksInCone;
  
  return pair;
  
}


//--------------------------------------------------------------------------------------------------------------------------------------------------


std::pair<int,float> TrackingEfficiencyAnalysis::scHasMatchedTrack(const reco::SuperCluster* sc, const reco::TrackCollection* trackcoll, const reco::PixelMatchGsfElectron* ele )
{ 

  double tagVertexZ = ele->gsfTrack()->vertex().z();
  double trackVertexZ;

  Hep3Vector track3V, sc3V;

  Hep3Vector tagVertex(ele->gsfTrack()->vertex().x(), ele->gsfTrack()->vertex().y(), ele->gsfTrack()->vertex().z());
 
  double corrScEta = getScCorrectedEta( sc, tagVertex );
  sc3V.setRThetaPhi( sc->energy(), 2*atan( exp( -1. * corrScEta ) ), sc->phi() );
  
  LogDebug("thisAnalysis") <<"[SCtrackIsolation] sc theta:"<<2*atan( exp( -1. * corrScEta ) )<<endl;

  float ptTracksInCone = 0.;
  int nTracksInCone = 0;

  for(reco::TrackCollection::const_iterator trackIt = trackcoll->begin(); trackIt != trackcoll->end(); trackIt++){
    
    trackVertexZ = trackIt->vertex().z();

    track3V.setRThetaPhi( sqrt( trackIt->momentum().mag2() ), trackIt->theta(), trackIt->phi() );
    
    if( track3V.deltaR(sc3V) < vetoConeSize_ ){
     
      if( fabs(trackVertexZ - tagVertexZ) < zVertexCut_){
	nTracksInCone++;
	ptTracksInCone += track3V.perp();
      }      
      
    }
    
    
  }
  
  std::pair<int,float> pair;
  pair.first = nTracksInCone;
  pair.second = ptTracksInCone;
  
  return pair;
  
}


//--------------------------------------------------------------------------------------------------------------------------------------------------


Hep3Vector TrackingEfficiencyAnalysis::getVertex(const reco::PixelMatchGsfElectron* ele )
{
  
  Hep3Vector vertex(ele->gsfTrack()->vertex().x(), ele->gsfTrack()->vertex().y(), ele->gsfTrack()->vertex().z() );

  return vertex;


}


//--------------------------------------------------------------------------------------------------------------------------------------------------


double TrackingEfficiencyAnalysis::getScCorrectedEta(const reco::SuperCluster* sc, Hep3Vector primaryVertex ) 
{

  double eta = 0.;
  
  double corrScX = sc->position().x() - primaryVertex.x();
  double corrScY = sc->position().y() - primaryVertex.y();
  double corrScZ = sc->position().z() - primaryVertex.z() ;

  eta = sc->eta();
  
   LogDebug("thisAnalysis") <<"[getScCorrectedEta] primaryVertex: x "<< primaryVertex.x() << " y "<<primaryVertex.y() << " z" <<primaryVertex.z() <<endl;
   LogDebug("thisAnalysis") <<"[getScCorrectedEta] scPosition: x "<< sc->position().x() << " y "<< sc->position().y() << " z" << sc->position().z() <<endl;
    
   LogDebug("thisAnalysis") <<"[getScCorrectedEta] eta "<< eta << endl;

  double corrScPerp = sqrt( corrScX*corrScX + corrScY*corrScY );

  Hep3Vector scPos( corrScX, corrScY, corrScZ );

  double corrScEta =scPos.eta();

  LogDebug("thisAnalysis") <<"[getScCorrectedEta] eta (cor) "<< corrScEta << endl;;

  return eta;
  
}


//--------------------------------------------------------------------------------------------------------------------------------------------------


double TrackingEfficiencyAnalysis::closestTrackToSc(const reco::SuperCluster* sc, const reco::TrackCollection* trackcoll, const reco::PixelMatchGsfElectron* ele )
{ 

  double tagVertexZ = ele->gsfTrack()->vertex().z();

  Hep3Vector track3V, sc3V;

  Hep3Vector tagVertex(ele->gsfTrack()->vertex().x(), ele->gsfTrack()->vertex().y(), ele->gsfTrack()->vertex().z());
 
  double corrScEta = getScCorrectedEta( sc, tagVertex );
  sc3V.setRThetaPhi( sc->energy(), 2*atan( exp( -1. * corrScEta ) ), sc->phi() );
  
  double deltaRMin = 9999.;

  for(reco::TrackCollection::const_iterator trackIt = trackcoll->begin(); trackIt != trackcoll->end(); trackIt++){
    
  track3V.setRThetaPhi( sqrt( trackIt->momentum().mag2() ), trackIt->theta(), trackIt->phi() );
    
  if( track3V.deltaR(sc3V) < deltaRMin )
    deltaRMin = track3V.deltaR(sc3V);
  
  
  }
  
  
  return deltaRMin;
  
}



