#include "JetMETAnalysis/PromptAnalysis/interface/PromptAna_CaloTowers.h"
#include "FWCore/Framework/interface/Event.h"

using namespace std;
using namespace edm;

PromptAna_CaloTowers::PromptAna_CaloTowers(const edm::ParameterSet& iConfig) 
  : inputTag(iConfig.getParameter<edm::InputTag>("InputTag"))
    , prefix  (iConfig.getParameter<std::string>  ("Prefix"  ))
    , suffix  (iConfig.getParameter<std::string>  ("Suffix"  ))
{
  produces <std::vector<double> > ( prefix + "EmEt"  + suffix );
  produces <std::vector<double> > ( prefix + "HadEt"  + suffix );
  produces <std::vector<double> > ( prefix + "EmEtVtx0"  + suffix );
  produces <std::vector<double> > ( prefix + "HadEtVtx0"  + suffix );
  produces <std::vector<double> > ( prefix + "OuterEt"  + suffix );
  produces <std::vector<double> > ( prefix + "Eta"  + suffix );
  produces <std::vector<double> > ( prefix + "Phi"  + suffix );
  produces <std::vector<double> > ( prefix + "EcalTime"  + suffix );
  produces <std::vector<double> > ( prefix + "HcalTime"  + suffix );
  produces <std::vector<int> >    ( prefix + "Ieta"  + suffix );
  produces <std::vector<int> >    ( prefix + "Iphi"  + suffix );
  produces <std::vector<int> >    ( prefix + "TowerStatusWord"  + suffix );

  produces <std::vector<float> >  ( prefix + "ECalEBXtalsE3x3"  + suffix );
  produces <std::vector<float> >  ( prefix + "ECalEBXtalsEMax"  + suffix );
  produces <std::vector<float> >  ( prefix + "ECalEBXtalsE2nd"  + suffix );
  produces <std::vector<float> >  ( prefix + "ECalEBXtalsE2nd3x3"  + suffix );
  produces <std::vector<float> >  ( prefix + "ECalEBSCEnergy"  + suffix );
  produces <std::vector<float> >  ( prefix + "ECalEBSeedEta"  + suffix );
  produces <std::vector<float> >  ( prefix + "ECalEBSeedPhi"  + suffix );
  produces <std::vector<float> >  ( prefix + "ECalEBEnergyRight"  + suffix );
  produces <std::vector<float> >  ( prefix + "ECalEBEnergyLeft"  + suffix );
  produces <std::vector<float> >  ( prefix + "ECalEBEnergyTop"  + suffix );
  produces <std::vector<float> >  ( prefix + "ECalEBEnergyBottom"  + suffix );
  produces <std::vector<int> >    ( prefix + "ECalEBNCRY805"  + suffix );
  produces <std::vector<float> >  ( prefix + "ECalEBFRook"  + suffix );

  produces <std::vector<float> >  ( prefix + "ECalEEXtalsE3x3"  + suffix );
  produces <std::vector<float> >  ( prefix + "ECalEEXtalsEMax"  + suffix );
  produces <std::vector<float> >  ( prefix + "ECalEEXtalsE2nd"  + suffix );
  produces <std::vector<float> >  ( prefix + "ECalEEXtalsE2nd3x3"  + suffix );
  produces <std::vector<float> >  ( prefix + "ECalEESCEnergy"  + suffix );
  produces <std::vector<float> >  ( prefix + "ECalEESeedEta"  + suffix );
  produces <std::vector<float> >  ( prefix + "ECalEESeedPhi"  + suffix );
  produces <std::vector<float> >  ( prefix + "ECalEEEnergyRight"  + suffix );
  produces <std::vector<float> >  ( prefix + "ECalEEEnergyLeft"  + suffix );
  produces <std::vector<float> >  ( prefix + "ECalEEEnergyTop"  + suffix );
  produces <std::vector<float> >  ( prefix + "ECalEEEnergyBottom"  + suffix );
  produces <std::vector<int> >    ( prefix + "ECalEENCRY805"  + suffix );
  produces <std::vector<float> >  ( prefix + "ECalEEFRook"  + suffix );
}

void PromptAna_CaloTowers::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) 
{
  std::auto_ptr<std::vector<double> >  emEt               ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  hadEt              ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  emEtVtx0           ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  hadEtVtx0          ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  outerEt            ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  eta                ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  phi                ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  ecalTime           ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  hcalTime           ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<int> >     ieta               ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<int> >     iphi               ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<int> >     towerStatusWord    ( new std::vector<int>()  ) ;

  std::auto_ptr<std::vector<float> >   ecalebxtalsE3x3    ( new std::vector<float>()  ) ;
  std::auto_ptr<std::vector<float> >   ecalebxtalsEmax    ( new std::vector<float>()  ) ;
  std::auto_ptr<std::vector<float> >   ecalebxtalsE2nd    ( new std::vector<float>()  ) ;
  std::auto_ptr<std::vector<float> >   ecalebxtalsE2nd3x3 ( new std::vector<float>()  ) ;
  std::auto_ptr<std::vector<float> >   ecalebscenergy     ( new std::vector<float>()  ) ;
  std::auto_ptr<std::vector<float> >   ecalebseedeta      ( new std::vector<float>()  ) ;
  std::auto_ptr<std::vector<float> >   ecalebseedphi      ( new std::vector<float>()  ) ;
  std::auto_ptr<std::vector<float> >   ecalebenright      ( new std::vector<float>()  ) ;
  std::auto_ptr<std::vector<float> >   ecalebenleft       ( new std::vector<float>()  ) ;
  std::auto_ptr<std::vector<float> >   ecalebentop        ( new std::vector<float>()  ) ;
  std::auto_ptr<std::vector<float> >   ecalebenbottom     ( new std::vector<float>()  ) ;
  std::auto_ptr<std::vector<int> >     ecalebncry805      ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<float> >   ecalebfrook        ( new std::vector<float>()  ) ;

  std::auto_ptr<std::vector<float> >   ecaleextalsE3x3    ( new std::vector<float>()  ) ;
  std::auto_ptr<std::vector<float> >   ecaleextalsEmax    ( new std::vector<float>()  ) ;
  std::auto_ptr<std::vector<float> >   ecaleextalsE2nd    ( new std::vector<float>()  ) ;
  std::auto_ptr<std::vector<float> >   ecaleextalsE2nd3x3 ( new std::vector<float>()  ) ;
  std::auto_ptr<std::vector<float> >   ecaleescenergy     ( new std::vector<float>()  ) ;
  std::auto_ptr<std::vector<float> >   ecaleeseedeta      ( new std::vector<float>()  ) ;
  std::auto_ptr<std::vector<float> >   ecaleeseedphi      ( new std::vector<float>()  ) ;
  std::auto_ptr<std::vector<float> >   ecaleeenright      ( new std::vector<float>()  ) ;
  std::auto_ptr<std::vector<float> >   ecaleeenleft       ( new std::vector<float>()  ) ;
  std::auto_ptr<std::vector<float> >   ecaleeentop        ( new std::vector<float>()  ) ;
  std::auto_ptr<std::vector<float> >   ecaleeenbottom     ( new std::vector<float>()  ) ;
  std::auto_ptr<std::vector<int> >     ecaleencry805      ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<float> >   ecaleefrook        ( new std::vector<float>()  ) ;

  //Get the ECAL SuperClusters
  edm::Handle<reco::SuperClusterCollection> pHybridSuperClusters;
  iEvent.getByLabel("hybridSuperClusters", pHybridSuperClusters);
  const reco::SuperClusterCollection* hybridSuperClusters = pHybridSuperClusters.product();
  
  edm::Handle<reco::SuperClusterCollection> pEndcapPreSuperClusters;
  iEvent.getByLabel("multi5x5SuperClustersWithPreshower", pEndcapPreSuperClusters);
  const reco::SuperClusterCollection* endcapPreSuperClusters = pEndcapPreSuperClusters.product();

  edm::Handle<EcalRecHitCollection> barrelEcalRecHitsH;
  edm::Handle<EcalRecHitCollection> endcapEcalRecHitsH;

  iEvent.getByLabel("ecalRecHit","EcalRecHitsEB", barrelEcalRecHitsH);  
  iEvent.getByLabel("ecalRecHit","EcalRecHitsEE", endcapEcalRecHitsH);
  
  allEBRecHits = barrelEcalRecHitsH.product();
  allEERecHits = endcapEcalRecHitsH.product();
  
  // get the  calo topology  from the event setup:
  edm::ESHandle<CaloTopology> pTopology;
  iSetup.get<CaloTopologyRecord>().get(pTopology);
  const CaloTopology *topology = pTopology.product();

  edm::ESHandle<CaloGeometry> geoHandle;
  iSetup.get<CaloGeometryRecord>().get(geoHandle);
  const CaloSubdetectorGeometry *geometry_eb = geoHandle->getSubdetectorGeometry(DetId::Ecal, EcalBarrel);
  const CaloSubdetectorGeometry *geometry_ee = geoHandle->getSubdetectorGeometry(DetId::Ecal, EcalEndcap);

  // loop over the super clusters in EB to find R9, FROOK and NCLY805
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
	  float en = recHitEnergy( v_id[i].first, allEBRecHits);
	  if( emax < en)  { emax = en; seedId = v_id[i].first; }
	}
      
      //Get (Eta,Phi) of the seed crystal
      const CaloCellGeometry *this_cell = geometry_eb->getGeometry(seedId);
      GlobalPoint posi = this_cell->getPosition();
      float seedeta =  posi.eta();
      float seedphi = posi.phi();
      
      //Find all crystals above 80 MeV in 5x5 matrix around seed
      CaloNavigator<DetId> cursor = CaloNavigator<DetId>(seedId, topology->getSubdetectorTopology(seedId) );
      int ncry = 0;
      float energy = 0;
      for ( int ii = -2; ii <= 2; ++ii ) 
	for ( int jj = -2; jj <= 2; ++jj ) 
	  {
	    cursor.home();
	    cursor.offsetBy( ii, jj );
	    energy = recHitEnergy( *cursor, allEBRecHits );
	    if(energy>0.08) ncry++;
	  }
 
      //Find the second most energetic crystal in immediate neighbourhood of the seed
      CaloNavigator<DetId> cursor2 = CaloNavigator<DetId>(seedId, topology->getSubdetectorTopology(seedId) );
      float energy2nd = -1.;
      energy = 0.;
      for ( int ii = -1; ii <= 1; ++ii ) 
	for ( int jj = -1; jj <= 1; ++jj ) 
	  {
	    cursor2.home();
	    cursor2.offsetBy( ii, jj );
	    energy = recHitEnergy( *cursor2, allEBRecHits );
	    if( energy2nd < energy && energy < emax)  { energy2nd = energy; }
	  }

      //Calculate frook
      float eseed    = EcalClusterTools::eMax ( *aClus , allEBRecHits );
      float eright   = EcalClusterTools::eRight ( *aClus , allEBRecHits, &(*topology) );
      float eleft    = EcalClusterTools::eLeft ( *aClus , allEBRecHits, &(*topology) ) ;
      float etop     = EcalClusterTools::eTop ( *aClus , allEBRecHits, &(*topology) ) ;
      float ebottom  = EcalClusterTools::eBottom ( *aClus , allEBRecHits, &(*topology) );

      //artur      
      //       float emaxrook = -1.;
      //       float frook    = -1.;
      //       if( eright > 0.08 ) emaxrook = eright;
      //       if( ebottom > emaxrook ) emaxrook = ebottom;
      //       if( eleft > emaxrook ) emaxrook = eleft;
      //       if( etop > emaxrook ) emaxrook = etop;
      //       if ( emaxrook > 0.1*eseed ) 
      // 	frook = emaxrook/(emaxrook + eseed);

      //francesco
      float emaxrook = 0.;
      float frook    = -1.;
      emaxrook = eright;
      if( ebottom > emaxrook ) emaxrook = ebottom;
      if( eleft > emaxrook ) emaxrook = eleft;
      if( etop > emaxrook ) emaxrook = etop;
      frook = emaxrook/(emaxrook + eseed);


      //store the values for each super cluster
      ecalebxtalsE3x3->push_back ( EcalClusterTools::e3x3 ( *aClus , allEBRecHits, &(*topology) ));
      ecalebxtalsEmax->push_back ( EcalClusterTools::eMax ( *aClus , allEBRecHits ) );
      ecalebxtalsE2nd->push_back ( EcalClusterTools::e2nd ( *aClus , allEBRecHits ) );
      ecalebenright  ->push_back ( EcalClusterTools::eRight ( *aClus , allEBRecHits, &(*topology) ) );
      ecalebenleft   ->push_back ( EcalClusterTools::eLeft ( *aClus , allEBRecHits, &(*topology) ) );
      ecalebentop    ->push_back ( EcalClusterTools::eTop ( *aClus , allEBRecHits, &(*topology) ) );
      ecalebenbottom ->push_back ( EcalClusterTools::eBottom ( *aClus , allEBRecHits, &(*topology) ) );
      ecalebscenergy ->push_back ( aClus->energy() );
      ecalebxtalsE2nd3x3->push_back ( energy2nd );
      ecalebseedeta  ->push_back ( seedeta );
      ecalebseedphi  ->push_back ( seedphi );
      ecalebncry805  ->push_back ( ncry-- );
      ecalebfrook    ->push_back ( frook );
    }
  
  // loop over the super clusters in EE to find R9, FROOK and NCLY805
  for(reco::SuperClusterCollection::const_iterator aClus = endcapPreSuperClusters->begin();
      aClus != endcapPreSuperClusters->end(); aClus++)
    {
      //Get vector of rechit DetIDs
      std::vector< std::pair<DetId, float> > v_id = aClus->hitsAndFractions();

      //Find the DetID of the most energetic crystal
      DetId seedId; 
      float emax = -1.; 
      for ( size_t i = 0; i < v_id.size(); ++i ) 
	{
	  float en = recHitEnergy( v_id[i].first, allEERecHits);
	  if( emax < en)  { emax = en; seedId = v_id[i].first; }
	}

      //Get (Eta,Phi) of the seed crystal
      const CaloCellGeometry *this_cell = geometry_ee->getGeometry(seedId);
      GlobalPoint posi = this_cell->getPosition();
      float seedeta =  posi.eta();
      float seedphi = posi.phi();
 
     //Find all crystals above 80 MeV in 5x5 matrix around seed
      CaloNavigator<DetId> cursor = CaloNavigator<DetId>(seedId, topology->getSubdetectorTopology(seedId) );
      int ncry = 0;
      float energy = 0;
      for ( int ii = -2; ii <= 2; ++ii ) 
	for ( int jj = -2; jj <= 2; ++jj ) 
	  {
	    cursor.home();
	    cursor.offsetBy( ii, jj );
	    energy = recHitEnergy( *cursor, allEERecHits );
	    if(energy>0.08) ncry++;
	  }
 
      //Find the second most energetic crystal in immediate neighbourhood of the seed
      CaloNavigator<DetId> cursor2 = CaloNavigator<DetId>(seedId, topology->getSubdetectorTopology(seedId) );
      float energy2nd = -1.;
      energy = 0.;
      for ( int ii = -1; ii <= 1; ++ii ) 
	for ( int jj = -1; jj <= 1; ++jj ) 
	  {
	    cursor2.home();
	    cursor2.offsetBy( ii, jj );
	    energy = recHitEnergy( *cursor2, allEERecHits );
	    if( energy2nd < energy && energy < emax)  { energy2nd = energy; }
	  }

      //Calculate frook
      float eseed    = EcalClusterTools::eMax ( *aClus , allEERecHits );
      float eright   = EcalClusterTools::eRight ( *aClus , allEERecHits, &(*topology) );
      float eleft    = EcalClusterTools::eLeft ( *aClus , allEERecHits, &(*topology) ) ;
      float etop     = EcalClusterTools::eTop ( *aClus , allEERecHits, &(*topology) ) ;
      float ebottom  = EcalClusterTools::eBottom ( *aClus , allEERecHits, &(*topology) );

      //artur
      //       float emaxrook = -1.;
      //       float frook    = -1.;      
      //       if( eright > 0.08 ) emaxrook = eright;
      //       if( ebottom > emaxrook ) emaxrook = ebottom;
      //       if( eleft > emaxrook ) emaxrook = eleft;
      //       if( etop > emaxrook ) emaxrook = etop;
      //       if ( emaxrook > 0.1*eseed ) 
      // 	frook = emaxrook/(emaxrook + eseed);

      //francesco
      float emaxrook = 0.;
      float frook    = -1.;      
      emaxrook = eright;
      if( ebottom > emaxrook ) emaxrook = ebottom;
      if( eleft > emaxrook ) emaxrook = eleft;
      if( etop > emaxrook ) emaxrook = etop;
      frook = emaxrook/(emaxrook + eseed);


      //store the values for each super cluster
      ecaleextalsE3x3->push_back ( EcalClusterTools::e3x3 ( *aClus , allEERecHits, &(*topology) ));
      ecaleextalsEmax->push_back ( EcalClusterTools::eMax ( *aClus , allEERecHits ) );
      ecaleextalsE2nd->push_back ( EcalClusterTools::e2nd ( *aClus , allEERecHits ) );
      ecaleeenright  ->push_back ( EcalClusterTools::eRight ( *aClus , allEERecHits, &(*topology) ) );
      ecaleeenleft   ->push_back ( EcalClusterTools::eLeft ( *aClus , allEERecHits, &(*topology) ) );
      ecaleeentop    ->push_back ( EcalClusterTools::eTop ( *aClus , allEERecHits, &(*topology) ) );
      ecaleeenbottom ->push_back ( EcalClusterTools::eBottom ( *aClus , allEERecHits, &(*topology) ) );
      ecaleescenergy ->push_back ( aClus->energy() );
      ecaleextalsE2nd3x3->push_back ( energy2nd );
      ecaleeseedeta  ->push_back ( seedeta );
      ecaleeseedphi  ->push_back ( seedphi );
      ecaleencry805  ->push_back ( ncry-- );
      ecaleefrook    ->push_back ( frook );
    }

  //Get the CaloTower Collection
  edm::Handle<CaloTowerCollection> calotowercollection;
  iEvent.getByLabel(inputTag, calotowercollection);
  
  for(CaloTowerCollection::const_iterator it = calotowercollection->begin(); it != calotowercollection->end() ; ++it )
    {
      emEt                -> push_back(it->emEt());
      hadEt               -> push_back(it->hadEt());
      emEtVtx0            -> push_back(it->emEt(0.0));
      hadEtVtx0           -> push_back(it->hadEt(0.0));
      outerEt             -> push_back(it->outerEt());
      eta                 -> push_back(it->p4().eta());
      phi                 -> push_back(it->p4().phi());
      ieta                -> push_back(it->ieta());
      iphi                -> push_back(it->iphi());
      ecalTime            -> push_back(it->ecalTime());
      hcalTime            -> push_back(it->hcalTime());
      towerStatusWord     -> push_back(it->towerStatusWord());
    }

  iEvent.put( emEt                   ,  prefix + "EmEt" + suffix );
  iEvent.put( hadEt                  ,  prefix + "HadEt" + suffix );
  iEvent.put( emEtVtx0               ,  prefix + "EmEtVtx0" + suffix );
  iEvent.put( hadEtVtx0              ,  prefix + "HadEtVtx0" + suffix );
  iEvent.put( outerEt                ,  prefix + "OuterEt" + suffix );
  iEvent.put( eta                    ,  prefix + "Eta" + suffix );
  iEvent.put( phi                    ,  prefix + "Phi" + suffix );
  iEvent.put( ieta                   ,  prefix + "Ieta"  + suffix );
  iEvent.put( iphi                   ,  prefix + "Iphi"  + suffix );
  iEvent.put( ecalTime               ,  prefix + "EcalTime"  + suffix );
  iEvent.put( hcalTime               ,  prefix + "HcalTime"  + suffix );
  iEvent.put( towerStatusWord        ,  prefix + "TowerStatusWord"  + suffix );

  iEvent.put( ecalebxtalsE3x3        ,  prefix + "ECalEBXtalsE3x3"  + suffix );
  iEvent.put( ecalebxtalsEmax        ,  prefix + "ECalEBXtalsEMax"  + suffix );
  iEvent.put( ecalebxtalsE2nd        ,  prefix + "ECalEBXtalsE2nd"  + suffix );
  iEvent.put( ecalebxtalsE2nd3x3     ,  prefix + "ECalEBXtalsE2nd3x3"  + suffix );
  iEvent.put( ecalebscenergy         ,  prefix + "ECalEBSCEnergy"  + suffix );
  iEvent.put( ecalebseedeta          ,  prefix + "ECalEBSeedEta"  + suffix );
  iEvent.put( ecalebseedphi          ,  prefix + "ECalEBSeedPhi"  + suffix );
  iEvent.put( ecalebenright          ,  prefix + "ECalEBEnergyRight"  + suffix );
  iEvent.put( ecalebenleft           ,  prefix + "ECalEBEnergyLeft"  + suffix );
  iEvent.put( ecalebentop            ,  prefix + "ECalEBEnergyTop"  + suffix );
  iEvent.put( ecalebenbottom         ,  prefix + "ECalEBEnergyBottom"  + suffix );
  iEvent.put( ecalebncry805          ,  prefix + "ECalEBNCRY805"  + suffix );
  iEvent.put( ecalebfrook            ,  prefix + "ECalEBFRook"  + suffix );

  iEvent.put( ecaleextalsE3x3        ,  prefix + "ECalEEXtalsE3x3"  + suffix );
  iEvent.put( ecaleextalsEmax        ,  prefix + "ECalEEXtalsEMax"  + suffix );
  iEvent.put( ecaleextalsE2nd        ,  prefix + "ECalEEXtalsE2nd"  + suffix );
  iEvent.put( ecaleextalsE2nd3x3     ,  prefix + "ECalEEXtalsE2nd3x3"  + suffix );
  iEvent.put( ecaleescenergy         ,  prefix + "ECalEESCEnergy"  + suffix );
  iEvent.put( ecaleeseedeta          ,  prefix + "ECalEESeedEta"  + suffix );
  iEvent.put( ecaleeseedphi          ,  prefix + "ECalEESeedPhi"  + suffix );
  iEvent.put( ecaleeenright          ,  prefix + "ECalEEEnergyRight"  + suffix );
  iEvent.put( ecaleeenleft           ,  prefix + "ECalEEEnergyLeft"  + suffix );
  iEvent.put( ecaleeentop            ,  prefix + "ECalEEEnergyTop"  + suffix );
  iEvent.put( ecaleeenbottom         ,  prefix + "ECalEEEnergyBottom"  + suffix );
  iEvent.put( ecaleencry805          ,  prefix + "ECalEENCRY805"  + suffix );
  iEvent.put( ecaleefrook            ,  prefix + "ECalEEFRook"  + suffix );
}

float PromptAna_CaloTowers::recHitEnergy(DetId id, const EcalRecHitCollection *recHits)
{
  if ( id == DetId(0) ) {
    return 0;
  } else {
    EcalRecHitCollection::const_iterator it = recHits->find( id );
    if ( it != recHits->end() ) {
      return (*it).energy();
     } else {
       //throw cms::Exception("EcalRecHitNotFound") << "The recHit corresponding to the DetId" << id.rawId() << " not found in the EcalRecHitCollection";
       // the recHit is not in the collection (hopefully zero suppressed)
       return 0;
     }
   }
   return 0;
 }
