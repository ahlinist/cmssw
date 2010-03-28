#include "JetMETAnalysis/PromptAnalysis/interface/PromptAna_ECALspikes.h"
#include "FWCore/Framework/interface/Event.h"

using namespace std;
using namespace edm;

PromptAna_ECALspikes::PromptAna_ECALspikes(const edm::ParameterSet& iConfig) 
  : inputTag(iConfig.getParameter<edm::InputTag>("InputTag"))
    , prefix  (iConfig.getParameter<std::string>  ("Prefix"  ))
    , suffix  (iConfig.getParameter<std::string>  ("Suffix"  ))
{
  produces <std::vector<float> >  ( prefix + "ECalEBSeedEta"  + suffix );
  produces <std::vector<float> >  ( prefix + "ECalEBSeedPhi"  + suffix );
  produces <std::vector<float> >  ( prefix + "ECalEBSeedEnergy"  + suffix );
  produces <std::vector<float> >  ( prefix + "ECalEBSeedE3x3"  + suffix );
  produces <std::vector<float> >  ( prefix + "ECalEBSeedERight"  + suffix );
  produces <std::vector<float> >  ( prefix + "ECalEBSeedELeft"  + suffix );
  produces <std::vector<float> >  ( prefix + "ECalEBSeedETop"  + suffix );
  produces <std::vector<float> >  ( prefix + "ECalEBSeedEBottom"  + suffix );

  produces <std::vector<float> >  ( prefix + "ECalEBSeedChi2"  + suffix );
  produces <std::vector<float> >  ( prefix + "ECalEBSeedTime"  + suffix );
  produces <std::vector<int> >    ( prefix + "ECalEBSeedRecoFlag"  + suffix );
  //  produces <std::vector<float> >  ( prefix + "ECalEBUnCalSeedChi2"  + suffix );

  produces <std::vector<float> >  ( prefix + "HCALenergyUp"   + suffix );
  produces <std::vector<float> >  ( prefix + "HCALenergy3x3"  + suffix );
  produces <std::vector<float> >  ( prefix + "HCALenergy5x5"  + suffix );
  produces <std::vector<float> >  ( prefix + "HCALenergy7x7"  + suffix );
  produces <std::vector<float> >  ( prefix + "HCALenergy9x9"  + suffix );

  produces <std::vector<float> >  ( prefix + "ECALenergy5x5"  + suffix );
  produces <std::vector<float> >  ( prefix + "ECALenergy7x7"  + suffix );
  produces <std::vector<float> >  ( prefix + "ECALenergy9x9"  + suffix );
  produces <std::vector<float> >  ( prefix + "ECALenergy11x11"  + suffix );
  produces <std::vector<float> >  ( prefix + "ECALenergy13x13"  + suffix );
}

void PromptAna_ECALspikes::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) 
{
  std::auto_ptr<std::vector<float> >   ecalebseedeta      ( new std::vector<float>()  ) ;
  std::auto_ptr<std::vector<float> >   ecalebseedphi      ( new std::vector<float>()  ) ;
  std::auto_ptr<std::vector<float> >   ecalebseedenergy   ( new std::vector<float>()  ) ;
  std::auto_ptr<std::vector<float> >   ecalebseeden3x3    ( new std::vector<float>()  ) ;
  std::auto_ptr<std::vector<float> >   ecalebseedenright  ( new std::vector<float>()  ) ;
  std::auto_ptr<std::vector<float> >   ecalebseedenleft   ( new std::vector<float>()  ) ;
  std::auto_ptr<std::vector<float> >   ecalebseedentop    ( new std::vector<float>()  ) ;
  std::auto_ptr<std::vector<float> >   ecalebseedenbottom ( new std::vector<float>()  ) ;

  std::auto_ptr<std::vector<float> >   ecalebseedchi2      ( new std::vector<float>() ) ;
  std::auto_ptr<std::vector<float> >   ecalebseedtime      ( new std::vector<float>() ) ;
  std::auto_ptr<std::vector<int> >     ecalebseedflag      ( new std::vector<int>()   ) ;
  //  std::auto_ptr<std::vector<float> >   ecalebuncalseedchi2 ( new std::vector<float>() ) ;

  std::auto_ptr<std::vector<float> >   hcalenergyup        ( new std::vector<float>() ) ;
  std::auto_ptr<std::vector<float> >   hcalenergy3x3       ( new std::vector<float>() ) ;
  std::auto_ptr<std::vector<float> >   hcalenergy5x5       ( new std::vector<float>() ) ;
  std::auto_ptr<std::vector<float> >   hcalenergy7x7       ( new std::vector<float>() ) ;
  std::auto_ptr<std::vector<float> >   hcalenergy9x9       ( new std::vector<float>() ) ;

  std::auto_ptr<std::vector<float> >   ecalenergy5x5       ( new std::vector<float>() ) ;
  std::auto_ptr<std::vector<float> >   ecalenergy7x7       ( new std::vector<float>() ) ;
  std::auto_ptr<std::vector<float> >   ecalenergy9x9       ( new std::vector<float>() ) ;
  std::auto_ptr<std::vector<float> >   ecalenergy11x11     ( new std::vector<float>() ) ;
  std::auto_ptr<std::vector<float> >   ecalenergy13x13     ( new std::vector<float>() ) ;

  //Get the ECAL SuperClusters
  edm::Handle<reco::SuperClusterCollection> pHybridSuperClusters;
  iEvent.getByLabel("hybridSuperClusters", pHybridSuperClusters);
  const reco::SuperClusterCollection* hybridSuperClusters = pHybridSuperClusters.product();
  
  //Get ECAL Rechit collections
  edm::Handle<EcalRecHitCollection> barrelEcalRecHitsH;
  //  edm::Handle<EcalUncalibratedRecHitCollection> barrelEcalUnCalRecHitsH;

  iEvent.getByLabel("ecalRecHit","EcalRecHitsEB", barrelEcalRecHitsH);  
  //  iEvent.getByLabel("ecalGlobalUncalibRecHit","EcalUncalibRecHitsEB", barrelEcalUnCalRecHitsH);
  
  allEBRecHits = barrelEcalRecHitsH.product();
  //  allEBUnCalRecHits = barrelEcalUnCalRecHitsH.product();
  
  //Create a CaloRecHitMetaCollection
  edm::Handle< HBHERecHitCollection > HcalRechitsH ;
  iEvent.getByLabel("hbhereco","",HcalRechitsH);

  allHcalRecHits = HcalRechitsH.product();

  // get the  calo topology  from the event setup:
  edm::ESHandle<CaloTopology> pTopology;
  iSetup.get<CaloTopologyRecord>().get(pTopology);
  const CaloTopology *topology = pTopology.product();
  
  // get ECAL barrel topology
  edm::ESHandle<CaloTopology> theCaloTopology;
  iSetup.get<CaloTopologyRecord>().get(theCaloTopology); 
  const CaloSubdetectorTopology* theEBTopology   = theCaloTopology->getSubdetectorTopology(DetId::Ecal,EcalBarrel);

  // get HcalTopology
  edm::ESHandle<HcalTopology> htopo;
  iSetup.get<IdealGeometryRecord>().get(htopo);
  const HcalTopology* theHBHETopology = htopo.product();

  //Get CaloGeometry
  edm::ESHandle<CaloGeometry> geoHandle;
  iSetup.get<CaloGeometryRecord>().get(geoHandle);
  const CaloSubdetectorGeometry *geometry_eb = geoHandle->getSubdetectorGeometry(DetId::Ecal, EcalBarrel);
  const CaloSubdetectorGeometry *geometry_hcal = geoHandle->getSubdetectorGeometry(DetId::Hcal, 4);

  // loop over the super clusters in EB
  for(reco::SuperClusterCollection::const_iterator aClus = hybridSuperClusters->begin();
      aClus != hybridSuperClusters->end(); aClus++)
    {
      //Get vector of rechit DetIDs
      std::vector< std::pair<DetId, float> > v_id = aClus->hitsAndFractions();

      //Find the DetID of the most energetic crystal
      DetId seedId; 
      float emax = -1.; 
      for ( size_t i = 0; i < v_id.size(); ++i ) 
	{	  
	  float en = recHitEnergyECAL( v_id[i].first, allEBRecHits);
	  if( emax < en)  { emax = en; seedId = v_id[i].first; }
	}
      
      //Get (Eta,Phi) of the seed crystal
      const CaloCellGeometry *this_cell = geometry_eb->getGeometry(seedId);
      GlobalPoint posi = this_cell->getPosition();
      float seedeta    =  posi.eta();
      float seedphi    = posi.phi();
      
      //Get Chi2, time etc.
      float seedchi2      = 0.;
      float seedtime      = 0.;
      int seedflag        = 0.;
      //      float uncalseedchi2 = 0.;

      EcalRecHitCollection::const_iterator it = allEBRecHits->find( seedId );
      //      EcalUncalibratedRecHitCollection::const_iterator itUnCal = allEBUnCalRecHits->find( seedId );
      
      if ( it != allEBRecHits->end() ) 
	{
	  seedchi2 = (*it).chi2();
	  seedtime = (*it).time();
	  seedflag = (int)(*it).recoFlag();
	}
      
//       if( itUnCal != allEBUnCalRecHits->end() )
// 	{
// 	  uncalseedchi2    = (*itUnCal).chi2();
// 	}
      
      //////////////////////////////////////////////
      /////ECAL energies around ECAL seeds//////////
      //////////////////////////////////////////////

      float ECALenergy5x5   = 0.;
      float ECALenergy7x7   = 0.;
      float ECALenergy9x9   = 0.;
      float ECALenergy11x11 = 0.;
      float ECALenergy13x13 = 0.;
      
      //Sum up energy around spike crystal in ECAL
      CaloNavigator<DetId> cursorE = CaloNavigator<DetId>(seedId, theEBTopology );
      
      //In a 5x5 matrix around the spike
      for ( int ii = -2; ii <= 2; ++ii ) for ( int jj = -2; jj <= 2; ++jj ) 
	  {
	    cursorE.home();
	    cursorE.offsetBy( ii, jj );
	    if( recHitEnergyECAL( *cursorE, allEBRecHits ) > 0.3 )
	      ECALenergy5x5 += recHitEnergyECAL( *cursorE, allEBRecHits );
	  }

      //In a 7x7 matrix around the spike
      for ( int ii = -3; ii <= 3; ++ii ) for ( int jj = -3; jj <= 3; ++jj ) 
	  {
	    cursorE.home();
	    cursorE.offsetBy( ii, jj );
	    if( recHitEnergyECAL( *cursorE, allEBRecHits ) > 0.3 )
	      ECALenergy7x7 += recHitEnergyECAL( *cursorE, allEBRecHits );
	  }

      //In a 9x9 matrix around the spike
      for ( int ii = -4; ii <= 4; ++ii ) for ( int jj = -4; jj <= 4; ++jj ) 
	  {
	    cursorE.home();
	    cursorE.offsetBy( ii, jj );
	    if( recHitEnergyECAL( *cursorE, allEBRecHits ) > 0.3 )
	      ECALenergy9x9 += recHitEnergyECAL( *cursorE, allEBRecHits );
	  }

      //In a 11x11 matrix around the spike
      for ( int ii = -5; ii <= 5; ++ii ) for ( int jj = -5; jj <= 5; ++jj ) 
	  {
	    cursorE.home();
	    cursorE.offsetBy( ii, jj );
	    if( recHitEnergyECAL( *cursorE, allEBRecHits ) > 0.3 )
	      ECALenergy11x11 += recHitEnergyECAL( *cursorE, allEBRecHits );
	  }

      //In a 13x13 matrix around the spike
      for ( int ii = -6; ii <= 6; ++ii ) for ( int jj = -6; jj <= 6; ++jj ) 
	  {
	    cursorE.home();
	    cursorE.offsetBy( ii, jj );
	    if( recHitEnergyECAL( *cursorE, allEBRecHits ) > 0.3 )
	      ECALenergy13x13 += recHitEnergyECAL( *cursorE, allEBRecHits );
	  }

      /////////////////////////////////////////////////
      ////////HCAL energies around ECAL seeds//////////
      /////////////////////////////////////////////////

      DetId hcalDetId ;
      hcalDetId = geometry_hcal->getClosestCell(posi);
      
      float HCALenergyUp  = 0.;
      float HCALenergy3x3 = 0.;
      float HCALenergy5x5 = 0.;
      float HCALenergy7x7 = 0.;
      float HCALenergy9x9 = 0.;

      //energy in HCAL tower above the spike
      HBHERecHitCollection::const_iterator it1 = allHcalRecHits->find( hcalDetId );
      if ( it1 != allHcalRecHits->end() )
	if( (*it1).energy()>1.0 )
	  HCALenergyUp = (*it1).energy();

      //Sum up energy around spike crystal in ECAL
      CaloNavigator<DetId> cursor = CaloNavigator<DetId>(hcalDetId, theHBHETopology );
      
      //In a 3x3 matrix around the spike
      for ( int ii = -1; ii <= 1; ++ii ) for ( int jj = -1; jj <= 1; ++jj ) 
	  {
	    cursor.home();
	    cursor.offsetBy( ii, jj );
	    if( recHitEnergyHCAL( *cursor, allHcalRecHits ) > 1.0 )
	      HCALenergy3x3 += recHitEnergyHCAL( *cursor, allHcalRecHits );
	  }

      //In a 5x5 matrix around the spike
      for ( int ii = -2; ii <= 2; ++ii ) for ( int jj = -2; jj <= 2; ++jj ) 
	  {
	    cursor.home();
	    cursor.offsetBy( ii, jj );
	    if( recHitEnergyHCAL( *cursor, allHcalRecHits ) > 1.0 )
	      HCALenergy5x5 += recHitEnergyHCAL( *cursor, allHcalRecHits );
	  }

      //In a 7x7 matrix around the spike
      for ( int ii = -3; ii <= 3; ++ii ) for ( int jj = -3; jj <= 3; ++jj ) 
	  {
	    cursor.home();
	    cursor.offsetBy( ii, jj );
	    if( recHitEnergyHCAL( *cursor, allHcalRecHits ) > 1.0 )
	      HCALenergy7x7 += recHitEnergyHCAL( *cursor, allHcalRecHits );
	  }

      //In a 9x9 matrix around the spike
      for ( int ii = -4; ii <= 4; ++ii ) for ( int jj = -4; jj <= 4; ++jj ) 
	  {
	    cursor.home();
	    cursor.offsetBy( ii, jj );
	    if( recHitEnergyHCAL( *cursor, allHcalRecHits ) > 1.0 )
	      HCALenergy9x9 += recHitEnergyHCAL( *cursor, allHcalRecHits );
	  }

      //store the values for each seeds
      ecalebseedeta       ->push_back ( seedeta );
      ecalebseedphi       ->push_back ( seedphi );
      ecalebseedenergy    ->push_back ( emax );

      ecalebseedchi2      ->push_back ( seedchi2 );
      ecalebseedtime      ->push_back ( seedtime );
      ecalebseedflag      ->push_back ( seedflag );
      //      ecalebuncalseedchi2 ->push_back ( uncalseedchi2 );

      //store the values for each super cluster, to identify spikes
      ecalebseeden3x3    ->push_back ( EcalClusterTools::e3x3 ( *aClus , allEBRecHits, &(*topology) ) );
      ecalebseedenright  ->push_back ( EcalClusterTools::eRight ( *aClus , allEBRecHits, &(*topology) ) );
      ecalebseedenleft   ->push_back ( EcalClusterTools::eLeft ( *aClus , allEBRecHits, &(*topology) ) );
      ecalebseedentop    ->push_back ( EcalClusterTools::eTop ( *aClus , allEBRecHits, &(*topology) ) );
      ecalebseedenbottom ->push_back ( EcalClusterTools::eBottom ( *aClus , allEBRecHits, &(*topology) ) );
      
      //HCAL energies
      hcalenergyup        ->push_back ( HCALenergyUp );
      hcalenergy3x3       ->push_back ( HCALenergy3x3);
      hcalenergy5x5       ->push_back ( HCALenergy5x5);
      hcalenergy7x7       ->push_back ( HCALenergy7x7);
      hcalenergy9x9       ->push_back ( HCALenergy9x9);
      
      ecalenergy5x5       ->push_back ( ECALenergy5x5);
      ecalenergy7x7       ->push_back ( ECALenergy7x7);
      ecalenergy9x9       ->push_back ( ECALenergy9x9);
      ecalenergy11x11     ->push_back ( ECALenergy11x11);
      ecalenergy13x13     ->push_back ( ECALenergy13x13);
    }
  
  iEvent.put( ecalebseedeta          ,  prefix + "ECalEBSeedEta"  + suffix );
  iEvent.put( ecalebseedphi          ,  prefix + "ECalEBSeedPhi"  + suffix );
  iEvent.put( ecalebseedenergy       ,  prefix + "ECalEBSeedEnergy"  + suffix );

  iEvent.put( ecalebseeden3x3        ,  prefix + "ECalEBSeedE3x3"  + suffix );
  iEvent.put( ecalebseedenright      ,  prefix + "ECalEBSeedERight"  + suffix );
  iEvent.put( ecalebseedenleft       ,  prefix + "ECalEBSeedELeft"  + suffix );
  iEvent.put( ecalebseedentop        ,  prefix + "ECalEBSeedETop"  + suffix );
  iEvent.put( ecalebseedenbottom     ,  prefix + "ECalEBSeedEBottom"  + suffix );

  iEvent.put( ecalebseedchi2         ,  prefix + "ECalEBSeedChi2"  + suffix );
  iEvent.put( ecalebseedtime         ,  prefix + "ECalEBSeedTime"  + suffix );
  iEvent.put( ecalebseedflag         ,  prefix + "ECalEBSeedRecoFlag"  + suffix );
  //  iEvent.put( ecalebuncalseedchi2    ,  prefix + "ECalEBUnCalSeedChi2"  + suffix );

  iEvent.put( hcalenergyup           ,  prefix + "HCALenergyUp"  + suffix );
  iEvent.put( hcalenergy3x3          ,  prefix + "HCALenergy3x3"  + suffix );
  iEvent.put( hcalenergy5x5          ,  prefix + "HCALenergy5x5"  + suffix );
  iEvent.put( hcalenergy7x7          ,  prefix + "HCALenergy7x7"  + suffix );
  iEvent.put( hcalenergy9x9          ,  prefix + "HCALenergy9x9"  + suffix );

  iEvent.put( ecalenergy5x5          ,  prefix + "ECALenergy5x5"  + suffix );
  iEvent.put( ecalenergy7x7          ,  prefix + "ECALenergy7x7"  + suffix );
  iEvent.put( ecalenergy9x9          ,  prefix + "ECALenergy9x9"  + suffix );
  iEvent.put( ecalenergy11x11        ,  prefix + "ECALenergy11x11"  + suffix );
  iEvent.put( ecalenergy13x13        ,  prefix + "ECALenergy13x13"  + suffix );
}

float PromptAna_ECALspikes::recHitEnergyECAL(DetId id, const EcalRecHitCollection *recHits)
{
  if ( id == DetId(0) ) {
    return 0;
  } else {
    EcalRecHitCollection::const_iterator it = recHits->find( id );
    if ( it != recHits->end() ) {
      return (*it).energy();
     } else {
       return 0;
     }
   }
   return 0;
 }

float PromptAna_ECALspikes::recHitEnergyHCAL(DetId id, const HBHERecHitCollection *recHits)
{
  if ( id == DetId(0) ) {
    return 0;
  } else {
    HBHERecHitCollection::const_iterator it = recHits->find( id );
    if ( it != recHits->end() ) {
      return (*it).energy();
     } else {
       return 0;
     }
   }
   return 0;
 }
