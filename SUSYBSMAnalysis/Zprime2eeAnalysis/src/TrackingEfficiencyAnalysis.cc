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

  mytree = new TTree("tree","tr");

  sc_weight = 1.;
 

  trackConeSize_ = iConfig.getParameter<double>("trackConeSize");
  eleIsoVetoConeSize_ = iConfig.getParameter<double>("eleIsoVetoConeSize");
  minTagProbeInvMass_ = iConfig.getParameter<double>("minTagProbeInvMass");
  maxTagProbeInvMass_ = iConfig.getParameter<double>("maxTagProbeInvMass");
  
  tagEtMin_ = iConfig.getParameter<double>("tagEtMin");
  probeEtMin_ = iConfig.getParameter<double>("probeEtMin");
  zVertexCut_ = iConfig.getParameter<double>("zVertexCut");
  hcalconesizemin_ = iConfig.getParameter<double>("hcalconesizemin");
  hcalconesizemax_ = iConfig.getParameter<double>("hcalconesizemax");
  hcalptMin_ = iConfig.getParameter<double>("hcalptMin");
  
  eleIsoTrackConeSize_ = iConfig.getParameter<double>("eleIsoTrackConeSize");
  eleIsoTrackMinPt_ = iConfig.getParameter<double>("eleIsoTrackMinPt");
  max_tag_tkPt_over_elePt_ = iConfig.getParameter<double>("max_tag_tkPt_over_elePt");
  max_tag_tkNumInCone_ = iConfig.getParameter<double>("max_tag_tkNumInCone");
 
  tagHcalEt_over_Et_max_ = iConfig.getParameter<double>("tagHcalEt_over_Et_max");
  probeHcalEt_over_Et_max_ = iConfig.getParameter<double>("probeHcalEt_over_Et_max");
 
  ecalconesize_ = iConfig.getParameter<double>("ecalconesize");
  max_ecalEt_over_tagEt_ = iConfig.getParameter<double>("max_ecalEt_over_tagEt");

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
  tagProbeEventsWithEBprobe = 0.;
  tagProbeEventsWithEEprobe = 0.;
  
  scWithMatchedTrack = 0.;
  EBscWithMatchedTrack = 0.;
  EEscWithMatchedTrack = 0.;

  //  HISTO INITIALISATION :
  
  // histos: on all events:

  sccoll_size = fs->make<TH1I>("sccoll_size","sccoll_size",20,0.,20.);
  gsfcoll_size = fs->make<TH1I>("gsfcoll_size","gsfcoll_size",20,0.,20.);
  

  h_totRecoEvents = fs->make<TH1F>("h_totRecoEvents", "h_totRecoEvents", 2, 0, 2. );
  h_totTagProbeEvents = fs->make<TH1F>("h_totTagProbeEvents", "h_totTagProbeEvents", 2, 0, 2. );

  //histos: tag properties
  h_tagCollectionSize = fs->make<TH1F>("h_tagCollectionSize", "h_tagCollectionSize",10,0.,10.);
  h_tagEnergy = fs->make<TH1F>("tagEnergy","tagEnergy",400,0.,800.);
  h_tagEt = fs->make<TH1F>("tagEt","tagEt",400,0.,800.);
  h_tagHcalEt_over_Et = fs->make<TH1F>("h_tagHcalEt_over_Et", "h_tagHcalEt_over_Et",200,0.,4.);
  h_tagEcalEt_over_Et = fs->make<TH1F>("h_tagEcalEt_over_Et", "h_tagEcalEt_over_Et",200,0.,4.);
  h_minDeltaRTrackGsfEle = fs->make<TH1F>("h_minDeltaRTrackGsfEle", "h_minDeltaRTrackGsfEle",  500, 0., 2. );
  h_tagTKIsoVar = fs->make<TH1F>("h_tagTKIsoVar", "h_tagTKIsoVar",500,-1.,4.);
  h_tagTKNumInCone = fs->make<TH1F>("h_tagTKNumInCone", "h_tagTKNumInCone",41,-1.,40.);

  //histos:probe properties
  h_probeCollectionSize = fs->make<TH1F>("h_probeCollectionSize", "h_probeCollectionSize",10,0.,10.);
  h_trackPtAroundSCFrac = fs->make<TH1F>("h_trackPtAroundSCFrac", "h_trackPtAroundSCFrac",200,0.,4.);
  h_trackPtAroundSC = fs->make<TH1F>("h_trackPtAroundSC", "h_trackPtAroundSC",400,0.,400.);
  h_TagProbeInvMass = fs->make<TH1F>("h_TagProbeInvMass", "h_TagProbeInvMass",600,0.,600.);
  h_probeEt = fs->make<TH1F>("probeEt","probeEt",400,0.,800.);
  h_probeHcalEt_over_Et = fs->make<TH1F>("h_probeHcalEt_over_Et", "h_probeHcalEt_over_Et",200,0.,4.);

  h_matchedTrackPtoverSCEt = fs->make<TH1F>("h_matchedTrackPtoverSCEt", "h_matchedTrackPtoverSCEt", 200,0.,4.);
  h_nearestTrackPoverSCE = fs->make<TH1F>("h_nearestTrackPoverSCE", "h_nearestTrackPoverSCE", 200,0.,4.);
  h_trackPtInConeMinusNearestTrackPt = fs->make<TH1F>("h_trackPtInConeMinusNearestTrackPtOverSCEt", "h_trackPtInConeMinusNearestTrackPtOverSCEt", 500, -1.,4.);
  h_minDeltaRTrackSC = fs->make<TH1F>("h_minDeltaRTrackSC", "h_minDeltaRTrackSC",  200, 0., 4. );
  
  //Define tree
  mytree = fs->make<TTree>("tree","tr");
 
  //Define branches
  mytree->Branch("TPinvMass",&invMass4Tree,"TPinvMass/D");
  mytree->Branch("probeIsolationVariable",&probeIsolationVariable,"probeIsolationVariable/D");
  mytree->Branch("probeHCALIsolationVariable",&probeHCALIsolationVariable,"probeHCALIsolationVariable/D");
  mytree->Branch("isTPeventSelected",&isTPeventSelected,"isTPeventSelected/O");
  
}

// Main code to analyse data: look for a high energy e+e- pair
// asking at least one GSF electron + another SC
// ---------------------------------------------------------------------------------------
void TrackingEfficiencyAnalysis::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  using namespace edm;
  using namespace std; 
  using namespace reco;
  
//   if (debug)
//       {
//     cout << "=======================================================" ; 
//       cout << "iEvent.id().run() = " << iEvent.id().run() << "  iEvent.id().event() = " << iEvent.id().event()  ; 
//       cout << "=======================================================" ; 
//   }

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
    cout<<"could not found the product with producer offlinePrimaryVerticesFromCTFTracks" << ex ;
 }
  const VertexCollection* VertexData = vertices.product();

  // Get the general tracks
  edm::Handle<reco::TrackCollection> trackHandle;
  try {iEvent.getByLabel("ctfWithMaterialTracks", trackHandle);} catch (cms::Exception& ex) { 
    cout<<"could not found the product with producer ctfWithMaterialTracks" << ex ;
}
  const reco::TrackCollection* trackcoll = trackHandle.product();

  //total number of events on which the code is run:
  event_tot++;

  h_totRecoEvents ->Fill(1.);

  if( (event_tot%100)  == 0)cout<<"----------------------------------------event # "<<event_tot<<" ------------------------------------------------"<<endl;
  
  // Get PixelMatchGsfElectron Collection -----------------------------------------
  reco::PixelMatchGsfElectronCollection::const_iterator gsfhottest;

  Handle<reco::PixelMatchGsfElectronCollection> pixelmatchelec;
  try{ iEvent.getByLabel("pixelMatchGsfElectrons","",pixelmatchelec); } catch(cms::Exception& ex) {
    cout<<"could not found the product with producer pixelMatchGsfElectrons" << ex ;
  }
  const reco::PixelMatchGsfElectronCollection* collpixelmatchelec = pixelmatchelec.product();
 
  //Get the hybrid and endcap supercluster collection
  Handle<reco::SuperClusterCollection> pHybridSuperClusters;
  Handle<reco::SuperClusterCollection> pIslandSuperClusters;

  try {
    iEvent.getByLabel("correctedHybridSuperClusters","", pHybridSuperClusters);
    iEvent.getByLabel("correctedEndcapSuperClustersWithPreshower","", pIslandSuperClusters);
  } catch ( cms::Exception& ex ) {
    cout<<"could not found the product with producer hybridSuperClusters or islandEndcapSuperClusters" << ex ;
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
  


  //get basic clusters (for ECAL isolation) - begin
  Handle<reco::BasicClusterCollection> pHybridBasicClusters;
  Handle<reco::BasicClusterCollection> pIslandBasicClusters;

  iEvent.getByLabel("hybridSuperClusters","", pHybridBasicClusters);
  iEvent.getByLabel("islandBasicClusters","islandEndcapBasicClusters", pIslandBasicClusters);
  
  const reco::BasicClusterCollection* hybridBasicClusters = pHybridBasicClusters.product();
  const reco::BasicClusterCollection* islandBasicClusters = pIslandBasicClusters.product();

  std::vector<const reco::BasicCluster*> bclusters;
  
  for (reco::BasicClusterCollection::const_iterator ibc = hybridBasicClusters->begin(); 
       ibc < hybridBasicClusters->end(); ibc++ )
    bclusters.push_back(&(*ibc));
  
  for (reco::BasicClusterCollection::const_iterator iec = islandBasicClusters->begin(); 
       iec < islandBasicClusters->end(); iec++ )
    bclusters.push_back(&(*iec));
  
  //get basic clusters (for ECAL isolation) - end
  
  //cout << "hybridSuperClusters->size() = " <<  hybridSuperClusters->size() ;
  //cout << "islandSuperClusters->size() = " <<  islandSuperClusters->size() ;
  //cout << "sclusters.size() = " <<  sclusters.size() ;

  gsfcoll_size->Fill(collpixelmatchelec->size() ,sc_weight);
  sccoll_size->Fill(sclusters.size() ,sc_weight);
  

  if(  collpixelmatchelec->size() > 0)  gsf_sizegreater0++;
  

  //if gsf collection size is > 0 and if (other) sc collection size is > 0 ...
  if( (collpixelmatchelec->size() > 0) && (sclusters.size() > 1)  ) {
    
    sc_sizegreater1++;
    
    /////////////////////////LOOK FOR TAGS: PIXELMATCHGSFELECTRONS with cuts
    
    //apply selection cuts to PixelMatchGsfElectrons
    typedef std::vector<const reco::PixelMatchGsfElectron*> PixelMatchGsfElectronPointerCollection;
    std::vector<const reco::PixelMatchGsfElectron*> TagCollection;
    //reco::PixelMatchGsfElectronCollection TagCollection;
    
    for(PixelMatchGsfElectronCollection::const_iterator tagIt = collpixelmatchelec->begin(); tagIt != collpixelmatchelec->end(); ++tagIt) {

      double tagEt = tagIt->energy() * sin(2*atan( exp( -1. * tagIt->eta()) ) );
      double tagAbsEta = fabs( tagIt->eta() ); 
      double tagHcalEt = hcaletisol( &*tagIt, towerCollection );
      
      h_tagEt->Fill( tagEt );
      
      double tagHcalEt_over_Et = tagHcalEt / tagEt;
      h_tagHcalEt_over_Et->Fill( tagHcalEt_over_Et );

      double tagEcalEt_over_Et = ecaletisol( tagIt->superCluster(), bclusters ) / tagEt;
      h_tagEcalEt_over_Et->Fill( tagEcalEt_over_Et );
      
      
      double tag_tkPt_over_elePt = eleTrackIsolation( trackcoll, &(*tagIt) ).second / tagEt;
      double tag_tkNumInCone = eleTrackIsolation( trackcoll, &(*tagIt) ).first;


      h_tagTKIsoVar ->Fill( tag_tkPt_over_elePt );
      h_tagTKNumInCone -> Fill( tag_tkNumInCone );

      if( tagAbsEta < 1.444 || (tagAbsEta > 1.560 && tagAbsEta < 2.5) ) //tag ele must be in ECAL fiducial region
	if( tagEt > tagEtMin_ ) //tag ele must have Et above certain threshold
	  if( tag_tkPt_over_elePt < max_tag_tkPt_over_elePt_ )//isolation cut for Tags - Tracks
	    if( tagEcalEt_over_Et < max_ecalEt_over_tagEt_ )
	      if( tag_tkNumInCone <= max_tag_tkNumInCone_)
		if( tagHcalEt_over_Et < tagHcalEt_over_Et_max_)//isolation cut for Tags - HCAL/ECAL
		  TagCollection.push_back( &(*tagIt) );
      
    }
    
    //    cout<<"Found "<<TagCollection.size()<<" Tag electrons, now looking for probes..."<<endl;
    
    h_tagCollectionSize->Fill( TagCollection.size() );
    

    ////////////////////////////////LOOK FOR PROBES: ECAL SC
    
    //association map: one Tag - many Probes
    
    std::map<const reco::PixelMatchGsfElectron*, std::vector<const reco::SuperCluster*> > TagProbeMap;
    
    for(PixelMatchGsfElectronPointerCollection::const_iterator selTagIt = TagCollection.begin(); selTagIt != TagCollection.end(); ++selTagIt){
      
      for( reco::SuperClusterCollection::const_iterator scIt_eb = hybridSuperClusters->begin(); scIt_eb != hybridSuperClusters->end(); ++scIt_eb) { 
	
	//cout<<" SC  energy "<<scIt_eb->energy()<<" ele energy "<< (*selTagIt)->superCluster()->energy()<<endl;
	//cout<<" SC  address "<<&(*scIt_eb)<<" ele address "<<&( *( (*selTagIt)->superCluster() )  )<<endl;
	
	if( &(*scIt_eb) == &( *( (*selTagIt)->superCluster() )  )  )
	  continue;
	
	double probeEt = scIt_eb->energy() * sin(2*atan( exp( -1. * scIt_eb->eta()) ) );
	double probeAbsEta = fabs( scIt_eb->eta() );
	
	if( probeAbsEta < 1.444 || (probeAbsEta > 1.560 && probeAbsEta < 2.5) )//probe must be in ECAL fiducial region
	  if( probeEt > probeEtMin_)
	    TagProbeMap[*selTagIt].push_back( &(*scIt_eb) );
	
      }
      
      for( reco::SuperClusterCollection::const_iterator scIt_eb = islandSuperClusters->begin(); scIt_eb != islandSuperClusters->end(); ++scIt_eb) { 
	
	if( &(*scIt_eb) == &( *( (*selTagIt)->superCluster() )  )  )
          continue;
	
	double probeEt = scIt_eb->energy() * sin(2*atan( exp( -1. * scIt_eb->eta()) ) );
	double probeAbsEta = fabs( scIt_eb->eta() );
	
	if( probeAbsEta < 1.444 || (probeAbsEta > 1.560 && probeAbsEta < 2.5) )//probe must be in ECAL fiducial region
	  if( probeEt > probeEtMin_ )
	    TagProbeMap[*selTagIt].push_back( &(*scIt_eb) );
	
      }
      
      
    }//end of loop over Tags to fill the one-to-many association map    
    
    
    if( TagCollection.size() >0 ){
      
      //////////////////////////////////DUMP TAG&PROBE EVENT ON HISTOS
      
      for(PixelMatchGsfElectronPointerCollection::const_iterator selTagIt = TagCollection.begin(); selTagIt != TagCollection.end(); ++selTagIt){
	
	h_probeCollectionSize ->Fill( TagProbeMap[*selTagIt].size() );

	if( TagProbeMap[*selTagIt].size() == 0 || TagProbeMap[*selTagIt].size() > 1  ) //exclude event if there is more than one probe for the tag 
	  continue;
	
	h_tagEnergy->Fill((*selTagIt)->energy());
	
	h_minDeltaRTrackGsfEle->Fill( deltaRofClosestTrackToGsfEle( trackcoll, *selTagIt )   );
	
	//cout<<"hcaletisol "<<hcaletisol( *selTagIt, towerCollection )<<endl;
	
	for(std::vector<const reco::SuperCluster*>::const_iterator It = TagProbeMap[*selTagIt].begin(); It != TagProbeMap[*selTagIt].end(); ++It){
	  
	  Hep3Vector tagVertex = getVertex( *selTagIt );
	  
	  if( SCGsfInvMass( *It, *selTagIt ) < minTagProbeInvMass_ || SCGsfInvMass( *It, *selTagIt ) > maxTagProbeInvMass_ )  //Tag-Probe invariant mass cut
	    continue; 
	  
	  std::pair<int,float> trackisol = SCtrackIsolation( *It,trackcoll, *selTagIt );
	  
	  
	  //cout<<"Probe analysis: # of track in cone: "<<trackisol.first<<" with total pT: "<<trackisol.second<<endl;
	  
	  double probeEt = ( (*It)->energy() * sin(2*atan( exp( -1. * (*It)->eta()))) );
	  
	  h_trackPtAroundSCFrac->Fill( trackisol.second / probeEt  );
	  h_trackPtAroundSC->Fill( trackisol.second );
	  h_TagProbeInvMass->Fill(SCGsfInvMass( *It, *selTagIt ));
	  
	  std::pair<int,float> hasTrack = scHasMatchedTrack(*It,trackcoll, *selTagIt );
	  
	  h_matchedTrackPtoverSCEt->Fill( hasTrack.second / probeEt  );
	  
	  double pOfTrackNearestToProbe = 0.;
	  double ptOfTrackNearestToProbe = 0.;
	  
	  if( deltaRofClosestTrackToSc( *It, trackcoll, *selTagIt ) < trackConeSize_){

	    pOfTrackNearestToProbe = closestTrackToSC( trackcoll, *It )->p();
	    ptOfTrackNearestToProbe = closestTrackToSC( trackcoll, *It )->pt();

	  }

	  h_trackPtInConeMinusNearestTrackPt -> Fill( (trackisol.second - ptOfTrackNearestToProbe) / probeEt);
	  
	  h_nearestTrackPoverSCE -> Fill(  pOfTrackNearestToProbe / ( (*It)->energy() ) );

	  invMass4Tree = SCGsfInvMass( *It, *selTagIt );//write TP inv mass on tree	  
	  
	  double probeHcalEt = hcaletisol( *It, towerCollection );
	  h_probeEt->Fill( probeEt );	
	  h_probeHcalEt_over_Et->Fill( probeHcalEt / probeEt );
	  probeHCALIsolationVariable = probeHcalEt / probeEt;
	  probeIsolationVariable = (trackisol.second - ptOfTrackNearestToProbe) / probeEt;
	  
	  if( fabs ( (*It)->eta() ) < 1.444  ){
	    if( (probeHcalEt / probeEt) < probeHcalEt_over_Et_max_ ){   
	  
	      h_totTagProbeEvents ->Fill(1.);
	      
	      tagProbeEvents++; 
	      tagProbeEventsWithEBprobe++;
	      
	      if( deltaROfClosestGsfEleToSC( collpixelmatchelec , *It ) < 0.1 ){
		//	    if( deltaROfClosestGsfEleToSC( *It, trackcoll, *selTagIt ) < 0.1 ){
		
		isTPeventSelected = 1.;
		
		scWithMatchedTrack++;
		EBscWithMatchedTrack++;
	      }
	      else isTPeventSelected = 0.;
	      
	    }
	    
	  }
	  
	  if( fabs ( (*It)->eta() ) < 2.5  && fabs ( (*It)->eta() ) > 1.560 ){
	    if( (probeHcalEt / probeEt) < probeHcalEt_over_Et_max_ ){
	      
	      tagProbeEvents++; 
	      h_totTagProbeEvents ->Fill(1.);
	      
	      tagProbeEventsWithEEprobe++;
	      
	      if( deltaROfClosestGsfEleToSC( collpixelmatchelec , *It  ) < 0.1 ){
		
		isTPeventSelected = 1.;
		
		scWithMatchedTrack++;
		EEscWithMatchedTrack++;
	      }
	      
	    else isTPeventSelected = 0.;
	    }
	  }	  
	  
	  h_minDeltaRTrackSC->Fill( deltaRofClosestTrackToSc( *It, trackcoll, *selTagIt )   );
	  
	  mytree -> Fill();
	  
	}//end loop on Probes for each Tag
      }//end loop on Tags
      
    }
    
  }//end of if size of collection is > 1
  
  
}//end of analyzer




// ------------ method called once each job just after ending the event loop  ------------
void 
TrackingEfficiencyAnalysis::endJob() {

  cout<<"***********Tag-Probe events: "<<tagProbeEvents<<" events with track-matched SC: "<<scWithMatchedTrack<<" Integrated tracking efficiency: "<<scWithMatchedTrack/tagProbeEvents<<endl;

  cout<<"***********Tag-Probe events with probe in ECAL Barrel: "<<tagProbeEventsWithEBprobe<<" events with track-matched SC: "<<EBscWithMatchedTrack<<" Integrated tracking efficiency: "<<EBscWithMatchedTrack/tagProbeEventsWithEBprobe<<endl;

  cout<<"***********Tag-Probe events with probe in ECAL Endcaps: "<<tagProbeEventsWithEEprobe<<" events with track-matched SC: "<<EEscWithMatchedTrack<<" Integrated tracking efficiency: "<<EEscWithMatchedTrack/tagProbeEventsWithEEprobe<<endl;

}
//define this as a plug-in
DEFINE_FWK_MODULE(TrackingEfficiencyAnalysis);


////////////////////////////////////////////////////////////MEMBER FUNCTIONS FOR ANALYZER////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------------------------------------------------------------------------

double TrackingEfficiencyAnalysis::SCGsfInvMass(const reco::SuperCluster* sc , const reco::PixelMatchGsfElectron* ele) {

    Hep3Vector ele3V, sc3V;

    Hep3Vector tagVertex(ele->gsfTrack()->vertex().x(), ele->gsfTrack()->vertex().y(), ele->gsfTrack()->vertex().z());
    double corrScEta = getScCorrectedEta( sc, tagVertex );

    //cout <<"[SCGsfInvMass]sc "<<sc->energy()<<" "<<2*atan( exp( -1. * corrScEta ) )<<" "<<sc->phi()<<endl;
    //cout <<"[SCGsfInvMass]ele "<<ele->superCluster()->energy()<<" "<<ele->theta()<<" "<<ele->gsfTrack()->innerMomentum().phi()<<endl;
    
    sc3V.setRThetaPhi( sc->energy(), 2*atan( exp( -1. * corrScEta ) ), sc->phi() );
    ele3V.setRThetaPhi( ele->superCluster()->energy(), ele->theta(), ele->gsfTrack()->innerMomentum().phi() );

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
  
  //cout <<"[SCtrackIsolation] sc theta:"<<2*atan( exp( -1. * corrScEta ) )<<endl;

  float ptTracksInCone = 0.;
  int nTracksInCone = 0;

  for(reco::TrackCollection::const_iterator trackIt = trackcoll->begin(); trackIt != trackcoll->end(); trackIt++){
    
    trackVertexZ = trackIt->vertex().z();

    track3V.setRThetaPhi( trackIt->p(), trackIt->theta(), trackIt->innerMomentum().phi() );
    

    if(track3V.deltaR(sc3V) < trackConeSize_ ){
     
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

std::pair<int,float> TrackingEfficiencyAnalysis::eleTrackIsolation( const reco::TrackCollection* trackcoll, const reco::PixelMatchGsfElectron* ele)
{ 
  
  Hep3Vector track3V, ele3V;


  ele3V.setRThetaPhi( ele->superCluster()->energy(), 2*atan( exp( -1. * ele->eta() ) ), ele->phi() );
  
  
  float ptTracksInCone = 0.;
  int nTracksInCone = 0;

  for(reco::TrackCollection::const_iterator trackIt = trackcoll->begin(); trackIt != trackcoll->end(); trackIt++){
    
    track3V.setRThetaPhi( trackIt->p(), trackIt->theta(), trackIt->innerMomentum().phi() );
    
    if(track3V.perp() > eleIsoTrackMinPt_ && track3V.deltaR(ele3V) < eleIsoTrackConeSize_ && track3V.deltaR(ele3V) > eleIsoVetoConeSize_){
      
      nTracksInCone++;
      //      ptTracksInCone += pow( ( track3V.perp()/ ele->gsfTrack()->pt() ) , 2 ) ;
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
  
  float ptTracksInCone = 0.;
  int nTracksInCone = 0;

  for(reco::TrackCollection::const_iterator trackIt = trackcoll->begin(); trackIt != trackcoll->end(); trackIt++){
    
    trackVertexZ = trackIt->vertex().z();

    track3V.setRThetaPhi( trackIt->p() , trackIt->theta(), trackIt->innerMomentum().phi() );
    
    if( track3V.deltaR(sc3V) < 0.1 ){
     
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
  
  double corrScPerp = sqrt( corrScX*corrScX + corrScY*corrScY );

  Hep3Vector scPos( corrScX, corrScY, corrScZ );

  double corrScEta =scPos.eta();

  return eta;
  
}


//--------------------------------------------------------------------------------------------------------------------------------------------------

double TrackingEfficiencyAnalysis::deltaRofClosestTrackToSc(const reco::SuperCluster* sc, const reco::TrackCollection* trackcoll, const reco::PixelMatchGsfElectron* ele )
{ 

  double tagVertexZ = ele->gsfTrack()->vertex().z();

  Hep3Vector track3V, sc3V;

  Hep3Vector tagVertex(ele->gsfTrack()->vertex().x(), ele->gsfTrack()->vertex().y(), ele->gsfTrack()->vertex().z());
 
  double corrScEta = getScCorrectedEta( sc, tagVertex );
  sc3V.setRThetaPhi( sc->energy(), 2*atan( exp( -1. * corrScEta ) ), sc->phi() );
  
  double deltaRMin = 9999.;

  for(reco::TrackCollection::const_iterator trackIt = trackcoll->begin(); trackIt != trackcoll->end(); trackIt++){
    
  track3V.setRThetaPhi( trackIt->p(), trackIt->theta(), trackIt->innerMomentum().phi() );
    
  if( track3V.deltaR(sc3V) < deltaRMin )
    deltaRMin = track3V.deltaR(sc3V);
  
  }
  
  
  return deltaRMin;
  
}


//--------------------------------------------------------------------------------------------------------------------------------------

const reco::Track* TrackingEfficiencyAnalysis::closestTrackToGsfEle( const reco::TrackCollection* trackcoll, const reco::PixelMatchGsfElectron* ele )
{

  Hep3Vector track3V, ele3V;

  ele3V.setRThetaPhi( ele->superCluster()->energy(), 2*atan( exp( -1. * ele->eta() ) ), ele->phi() );

  double deltaRMin = 9999.;

  const reco::Track* closestTrack;

  for(reco::TrackCollection::const_iterator trackIt = trackcoll->begin(); trackIt != trackcoll->end(); trackIt++){

    track3V.setRThetaPhi( trackIt->p(), trackIt->theta(), trackIt->innerMomentum().phi() );

    if( track3V.deltaR(ele3V) < deltaRMin ){
     
      deltaRMin = track3V.deltaR(ele3V);
      closestTrack = &(*trackIt);
    
    }
    
  }
  
  
  
  return closestTrack;
  
}

//---------------------------------------------------------------------------------------------------------------------------------------


const reco::Track* TrackingEfficiencyAnalysis::closestTrackToSC( const reco::TrackCollection* trackcoll, const reco::SuperCluster* sc )
{

  Hep3Vector track3V, sc3V;

  sc3V.setRThetaPhi( sc->energy(), 2*atan( exp( -1. * sc->eta() ) ), sc->phi() );

  double deltaRMin = 9999.;

  const reco::Track* closestTrack;

  for(reco::TrackCollection::const_iterator trackIt = trackcoll->begin(); trackIt != trackcoll->end(); trackIt++){

    track3V.setRThetaPhi( trackIt->p(), trackIt->theta(), trackIt->innerMomentum().phi() );

    if( track3V.deltaR(sc3V) < deltaRMin ){
     
      deltaRMin = track3V.deltaR(sc3V);
      closestTrack = &(*trackIt);
    
    }
    
  }
  
  
  
  return closestTrack;
  
}

//---------------------------------------------------------------------------------------------------------------------------------------


double TrackingEfficiencyAnalysis::deltaROfClosestGsfEleToSC( const reco::PixelMatchGsfElectronCollection* elecoll, const reco::SuperCluster* sc )
{

  Hep3Vector ele3V, sc3V;

  sc3V.setRThetaPhi( sc->energy(), 2*atan( exp( -1. * sc->eta() ) ), sc->phi() );

  double deltaRMin = 9999.;

  for(reco::PixelMatchGsfElectronCollection::const_iterator trackIt = elecoll->begin(); trackIt != elecoll->end(); trackIt++){

    ele3V.setRThetaPhi( trackIt->superCluster()->energy(), trackIt->theta(), trackIt->phi() );

    if( ele3V.deltaR(sc3V) < deltaRMin )
      deltaRMin = ele3V.deltaR(sc3V);
        
  }
  
  return deltaRMin;
  
}

//---------------------------------------------------------------------------------------------------------------------------------------


double TrackingEfficiencyAnalysis::hcaletisol(const reco::PixelMatchGsfElectron* ele, const CaloTowerCollection* hbhe){

  using namespace edm; // needed for all fwk related classes
  using namespace std;
  using namespace reco;

  float hcalIsol=0.;

  float candSCphi = ele->gsfTrack()->innerMomentum().phi();
  float candSCeta = ele->eta();

  for(CaloTowerCollection::const_iterator hbheItr = hbhe->begin(); hbheItr != hbhe->end(); ++hbheItr){
    double HcalHit_eta=hbheItr->eta();
    double HcalHit_phi=hbheItr->phi();
    float HcalHit_pth=hbheItr->hadEt();
    
    if(HcalHit_pth>hcalptMin_) {
      float deltaphi;
      //      if(HcalHit_phi<0) HcalHit_phi+=TWOPI;
      HcalHit_phi+=PI;
      //if(candSCphi<0) candSCphi+=TWOPI;
      deltaphi=fabs(HcalHit_phi-candSCphi);
      if(deltaphi>TWOPI) deltaphi-=TWOPI;
      if(deltaphi>PI) deltaphi=TWOPI-deltaphi;
      float deltaeta=fabs(HcalHit_eta-candSCeta);
      float newDelta= sqrt(deltaphi*deltaphi+ deltaeta*deltaeta);
      if(newDelta < hcalconesizemax_ && newDelta > hcalconesizemin_) hcalIsol+=HcalHit_pth;
      
    }      
  }
  return hcalIsol;
}

//---------------------------------------------------------------------------------------------------------------------------------------


double TrackingEfficiencyAnalysis::hcaletisol(const reco::SuperCluster* ele, const CaloTowerCollection* hbhe){

  using namespace edm; // needed for all fwk related classes
  using namespace std;
  using namespace reco;

  float hcalIsol=0.;

  float candSCphi = ele->phi();
  float candSCeta = ele->eta();

  for(CaloTowerCollection::const_iterator hbheItr = hbhe->begin(); hbheItr != hbhe->end(); ++hbheItr){
    double HcalHit_eta=hbheItr->eta();
    double HcalHit_phi=hbheItr->phi();
    float HcalHit_pth=hbheItr->hadEt();
    
    if(HcalHit_pth>hcalptMin_) {
      float deltaphi;
      //      if(HcalHit_phi<0) HcalHit_phi+=TWOPI;
      HcalHit_phi+=PI;
      //if(candSCphi<0) candSCphi+=TWOPI;
      deltaphi=fabs(HcalHit_phi-candSCphi);
      if(deltaphi>TWOPI) deltaphi-=TWOPI;
      if(deltaphi>PI) deltaphi=TWOPI-deltaphi;
      float deltaeta=fabs(HcalHit_eta-candSCeta);
      float newDelta= sqrt(deltaphi*deltaphi+ deltaeta*deltaeta);
      if(newDelta < hcalconesizemax_ && newDelta > hcalconesizemin_) hcalIsol+=HcalHit_pth;
      
    }      
  }
  return hcalIsol;
}

//---------------------------------------------------------------------------------------------------------------------------------------


double TrackingEfficiencyAnalysis::ecaletisol(reco::SuperClusterRef maxsupercluster, std::vector<const reco::BasicCluster*> bclusters){

float ecalIsol=0.;
float candSCphi = maxsupercluster->phi();
float candSCeta = maxsupercluster->eta();

if(debug) {
LogDebug("Zprime2eeAnaGsfEm")<<"energie(raw and presh) eta and phi of max SC "
<<maxsupercluster->rawEnergy()<<" "
<<maxsupercluster->preshowerEnergy()<<" "
<<maxsupercluster->eta()<<" "
<<maxsupercluster->phi();
}

bool MATCHEDSC = true;
const reco::BasicCluster *cluster= 0;

//loop over basic clusters
for(std::vector<const reco::BasicCluster*>::const_iterator cItr = bclusters.begin(); cItr != bclusters.end(); ++cItr){

cluster = *cItr;
float ebc_bce    = cluster->energy();
float ebc_bceta  = cluster->eta();
float ebc_bcphi  = cluster->phi();
float ebc_bcet   = ebc_bce*sin(2*atan(exp(ebc_bceta)));
float newDelta;

//     if (ebc_bcet > etMin && ebc_bcalgo == 0) {
//       if (ebc_bcchi2 < 30.) {
if(MATCHEDSC){
bool inSuperCluster = false;

      reco::basicCluster_iterator theEclust = maxsupercluster->clustersBegin();
      // loop over the basic clusters of the matched supercluster
      for(;theEclust != maxsupercluster->clustersEnd();
        theEclust++) {
	if (&(**theEclust) ==  cluster) inSuperCluster = true;
      }
      if (!inSuperCluster) {
      float deltaphi;
      if(ebc_bcphi<0) ebc_bcphi+=TWOPI;
      if(candSCphi<0) candSCphi+=TWOPI;
      deltaphi=fabs(ebc_bcphi-candSCphi);
      if(deltaphi>TWOPI) deltaphi-=TWOPI;
      if(deltaphi>PI) deltaphi=TWOPI-deltaphi;
      float deltaeta=fabs(ebc_bceta-candSCeta);
      newDelta= sqrt(deltaphi*deltaphi+ deltaeta*deltaeta);
      if(newDelta < ecalconesize_) {
        ecalIsol+=ebc_bcet;
	}
      }
    }
    //   } // matches ebc_bcchi2
    // } // matches ebc_bcet && ebc_bcalgo

  }
  return ecalIsol;
}


//------------------------------------------------------------------------------------------------------------------------------------

double TrackingEfficiencyAnalysis::deltaRofClosestTrackToGsfEle( const reco::TrackCollection* trackcoll, const reco::PixelMatchGsfElectron* ele )
{

  Hep3Vector track3V, ele3V;

  ele3V.setRThetaPhi( ele->superCluster()->energy(), 2*atan( exp( -1. * ele->eta() ) ), ele->phi() );

  double deltaRMin = 9999.;

  for(reco::TrackCollection::const_iterator trackIt = trackcoll->begin(); trackIt != trackcoll->end(); trackIt++){

    track3V.setRThetaPhi( trackIt->p(), trackIt->theta(), trackIt->innerMomentum().phi() );

    if( track3V.deltaR(ele3V) < deltaRMin )
      deltaRMin = track3V.deltaR(ele3V);


  }


  return deltaRMin;

}
