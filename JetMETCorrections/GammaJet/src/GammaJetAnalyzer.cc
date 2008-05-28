// -*- C++ -*-
//
// Package:    GammaJetAnalyzer
// Class:      GammaJetAnalyzer
// 
/**\class GammaJetAnalyzer GammaJetAnalyzer.cc MyAnalysis/GammaJetAnalyzer/src/GammaJetAnalyzer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Daniele del Re
//         Created:  Thu Sep 13 16:00:15 CEST 2007
// $Id: GammaJetAnalyzer.cc,v 1.6 2008/03/27 15:54:38 delre Exp $
//
//

//
// constructors and destructor
//

#include "MyAnalysis/GammaJetAnalysis/interface/GammaJetAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloTopology/interface/CaloSubdetectorTopology.h"
#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"

#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/PhotonFwd.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
//#include "DataFormats/JetReco/interface/CalogJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "DataFormats/EgammaReco/interface/ClusterShape.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h" 
#include "RecoCaloTools/Selectors/interface/CaloConeSelector.h"
#include "RecoCaloTools/MetaCollections/interface/CaloRecHitMetaCollections.h"

#include "RecoEcal/EgammaCoreTools/interface/ClusterShapeAlgo.h"

#include "MagneticField/Engine/interface/MagneticField.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "TrackingTools/MaterialEffects/interface/PropagatorWithMaterial.h"
#include "TrackingTools/GeomPropagators/interface/AnalyticalPropagator.h"
#include "TrackingTools/GsfTools/interface/MultiTrajectoryStateTransform.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "TrackingTools/PatternTools/interface/TransverseImpactPointExtrapolator.h"
#include "TrackingTools/GsfTools/interface/GsfPropagatorAdapter.h"

#include "RecoEgamma/EgammaTools/interface/ECALPositionCalculator.h"

using namespace reco;

GammaJetAnalyzer::GammaJetAnalyzer(const edm::ParameterSet& iConfig)
{
  MCTruthCollection_ = iConfig.getUntrackedParameter<edm::InputTag>("MCTruthCollection");
  trackTags_ = iConfig.getUntrackedParameter<edm::InputTag>("tracks");
  Photonsrc_ = iConfig.getUntrackedParameter<edm::InputTag>( "Photonsrc" );
  Jetsrc_ = iConfig.getUntrackedParameter<edm::InputTag>( "jets" );
  JetGensrc_ = iConfig.getUntrackedParameter<edm::InputTag>( "jetsgen" );
  METsrc_ = iConfig.getUntrackedParameter<edm::InputTag>( "met" );
  METGensrc_ = iConfig.getUntrackedParameter<edm::InputTag>( "genMet" );
  HBhitsrc_ = iConfig.getUntrackedParameter<edm::InputTag>( "hbhits" );
  recoCollection_    = iConfig.getParameter<std::string>("recoCollection");
  recoProducer_      = iConfig.getParameter<std::string>("recoProducer");
  filename_ = iConfig.getUntrackedParameter<string>("HistOutFile"); 
  hOutputFile   = new TFile( filename_.c_str(), "RECREATE" ) ;
  PtPhoton1st = new TH1D( "PtPhoton1st", "Pt spectrum of the most energetic photon", 50, 0.0, 400.0);
  PtPhoton2st = new TH1D( "PtPhoton2st", "Pt spectrum of the 2' most energetic photon", 50, 0.0, 400.0);
  PtPhoton3st = new TH1D( "PtPhoton3st", "Pt spectrum of the 3' most energetic photon", 50, 0.0, 400.0);
  PtPhotonMC1st = new TH1D( "PtPhotonMC1st", "Pt spectrum of the most energetic photonMC", 50, 0.0, 400.0);
  PtPhotonMC2st = new TH1D( "PtPhotonMC2st", "Pt spectrum of the 2' most energetic photonMC", 50, 0.0, 400.0);
  PtPhotonMC3st = new TH1D( "PtPhotonMC3st", "Pt spectrum of the 3' most energetic photonMC", 50, 0.0, 400.0);
  
}


GammaJetAnalyzer::~GammaJetAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
GammaJetAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   nMC = nPhot = nJet = nJetGen = 0;

   using reco::TrackCollection;
  
   double maxptphoton1(0), maxptphoton2(0), maxptphoton3(0);

   // get MC info from GenParticleCandidates 
   Handle<GenParticleCollection> genParticles;
   iEvent.getByLabel( "genParticles", genParticles );
   
   // get tracks
   Handle<TrackCollection> tracks;
   iEvent.getByLabel(trackTags_,tracks);

   // get photons
   Handle<PhotonCollection>  PhotonHandle;
   iEvent.getByLabel( Photonsrc_, PhotonHandle );

   // get calo jet collection
   Handle<CaloJetCollection> jets;
   iEvent.getByLabel(Jetsrc_, jets);

   // get gen jet collection
   Handle<GenJetCollection> jetsgen;
   iEvent.getByLabel(JetGensrc_, jetsgen);

   // get MET
   Handle<CaloMETCollection> calomethandle;
   iEvent.getByLabel(METsrc_, calomethandle);

   // get gen MET
   Handle<GenMETCollection> genmethandle;
   iEvent.getByLabel(METGensrc_, genmethandle);
  
   // get HCAL info
   Handle<HBHERecHitCollection> hbhe;
   iEvent.getByLabel(HBhitsrc_, hbhe);
   const HBHERecHitMetaCollection mhbhe(*hbhe);
 
   // get ECAL reco hits
   Handle<EBRecHitCollection> ecalhits;
   const EBRecHitCollection* rhits=0;
   iEvent.getByLabel( recoProducer_, recoCollection_, ecalhits);
   const EcalRecHitMetaCollection mecalhits(*ecalhits);    
   rhits = ecalhits.product(); // get a ptr to the product

   // get geometry
   edm::ESHandle<CaloGeometry> geoHandle;
   iSetup.get<IdealGeometryRecord>().get(geoHandle);
   const CaloGeometry* geometry = geoHandle.product();
   const CaloSubdetectorGeometry* geometry_p = geoHandle->getSubdetectorGeometry(DetId::Ecal, EcalBarrel);

   // get topology
   const CaloSubdetectorTopology *topology_p;
   edm::ESHandle<CaloTopology> topoHandle;
   iSetup.get<CaloTopologyRecord>().get(topoHandle);
   topology_p = topoHandle->getSubdetectorTopology(DetId::Ecal, EcalBarrel);

   edm::ESHandle<TrackerGeometry> trackerHandle_;
   edm::ESHandle<MagneticField> theMagField;
   iSetup.get<IdealMagneticFieldRecord>().get(theMagField);
   iSetup.get<TrackerDigiGeometryRecord>().get(trackerHandle_);
   const GsfPropagatorAdapter* geomPropFw_ = new GsfPropagatorAdapter(AnalyticalPropagator(theMagField.product(), alongMomentum));
   const TrajectoryStateTransform* theTransform_ = 0;
  
   ClusterShapeAlgo algo;

   // Loop over MC truth
   
   for ( GenParticleCollection::const_iterator p = genParticles->begin();
	p != genParticles->end(); ++p ) {
     
     //     if(nMC>=2000) {cout << "number of MC particle is larger than 2000. Skipping" << endl; continue;}
     if(nMC>=100) {continue;}  // to reduce the root file size

     pdgIdMC[nMC] = p->pdgId();
     statusMC[nMC] = p->status();
     massMC[nMC] = p->mass();
     pxMC[nMC] = p->px();	 
     pyMC[nMC] = p->py();	 
     pzMC[nMC] = p->pz();	 
     eMC[nMC] = p->energy();	 
     etaMC[nMC] = p->eta();	 
     phiMC[nMC] = p->phi();	 
     
     if(p->pdgId()==22 && p->status()==1){
       
       double phiTrue = p->phi();
       double etaTrue = p->eta();
       double eTrue  = p->energy();
       double etTrue  = p->energy()/cosh(etaTrue);  
       
       if (etTrue>maxptphoton1) {
  	 maxptphoton1 = etTrue;
       }
       if (etTrue>maxptphoton2 && etTrue<maxptphoton1) maxptphoton2 = etTrue;
       if (etTrue>maxptphoton3 && etTrue<maxptphoton2) maxptphoton3 = etTrue;     
       
     }
     
     if (p->numberOfMothers() > 0 ) { 
       const Candidate * mom = p->mother();
       for( size_t j = 0; j < genParticles->size(); ++ j ) {
	 const Candidate * ref = &((*genParticles)[j]);
	 if (mom->px() == ref->px() && mom->py() == ref->py() && mom->pz()==ref->pz() 
	     && mom->status() == ref->status() && mom->pdgId()==ref->pdgId())
	   {
	     motherIDMC[nMC] = j;
	   }
       }  
     } else {
       motherIDMC[nMC] = -1;
     }
     
     nMC++;

   }

   if(maxptphoton1>-900.) PtPhotonMC1st->Fill(maxptphoton1);
   if(maxptphoton2>-900.) PtPhotonMC2st->Fill(maxptphoton2);
   if(maxptphoton3>-900.) PtPhotonMC3st->Fill(maxptphoton3);

   maxptphoton1 = 0; maxptphoton2 = 0; maxptphoton3 = 0;

   // Loop over reco photons

   for ( PhotonCollection::const_iterator it = PhotonHandle->begin(); 
	 it != PhotonHandle->end(); it++ ) {
     
     if(it->energy()<5.) continue;
     if(nPhot>=40) {cout << "number of photons is larger than 40. Skipping" << endl; continue;}
     
     pxPhot[nPhot] = it->px();	 
     pyPhot[nPhot] = it->py();	 
     pzPhot[nPhot] = it->pz();	 
     ePhot[nPhot] = it->energy();	 
     etaPhot[nPhot] = it->eta();	 
     phiPhot[nPhot] = it->phi();	      
     
     double ptphoton = it->pt();
     if (ptphoton>maxptphoton1) {
       maxptphoton1 = ptphoton;
     }
     if (ptphoton>maxptphoton2 && ptphoton<maxptphoton1) maxptphoton2 = ptphoton;
     if (ptphoton>maxptphoton3 && ptphoton<maxptphoton2) maxptphoton3 = ptphoton;     
     
     double ptiso02(0.), ptiso025(0.), ptiso03(0.), ptiso035(0.), ptiso04(0.);
     int ntrkiso02(0), ntrkiso025(0), ntrkiso03(0), ntrkiso035(0), ntrkiso04(0);
     double ptisoatecal02(0.), ptisoatecal025(0.), ptisoatecal03(0.), ptisoatecal035(0.), ptisoatecal04(0.);
     int ntrkisoatecal02(0), ntrkisoatecal025(0), ntrkisoatecal03(0), ntrkisoatecal035(0), ntrkisoatecal04(0);
     
     // calculate track isolation for different cone values

     for(TrackCollection::const_iterator itTrack = tracks->begin();
	 itTrack != tracks->end();                      
	 ++itTrack) {
       double etaTrack = itTrack->innerMomentum().eta();  double phiTrack = itTrack->innerMomentum().phi();
       
       double deltaPhi = phiTrack-it->phi();
       double deltaEta = etaTrack-it->eta();
       if(deltaPhi > Geom::pi()) deltaPhi -= 2.*Geom::pi();
       if(deltaPhi < -Geom::pi()) deltaPhi += 2.*Geom::pi();
       double deltaR = std::sqrt(deltaEta*deltaEta+deltaPhi*deltaPhi);
       
       if(deltaR < .2)  {ptiso02  += sqrt(itTrack->innerMomentum().Mag2()); ntrkiso02++; }
       if(deltaR < .25) {ptiso025 += sqrt(itTrack->innerMomentum().Mag2()); ntrkiso025++;}
       if(deltaR < .3)  {ptiso03  += sqrt(itTrack->innerMomentum().Mag2()); ntrkiso03++; }
       if(deltaR < .35) {ptiso035 += sqrt(itTrack->innerMomentum().Mag2()); ntrkiso035++;}
       if(deltaR < .4)  {ptiso04  += sqrt(itTrack->innerMomentum().Mag2()); ntrkiso04++; }
       
       const math::XYZVector tempvec = itTrack->innerMomentum();
       const math::XYZPoint temppos = itTrack->innerPosition();
       int tempcharge = itTrack->charge();
       
       ECALPositionCalculator tempcalculator;      
       
       double phitrk = tempcalculator.ecalPhi(tempvec,temppos,tempcharge);
       double etatrk = tempcalculator.ecalEta(tempvec,temppos);
       
       deltaPhi = phitrk-it->phi();
       deltaEta = etatrk-it->eta();      
       
       deltaR = std::sqrt(deltaEta*deltaEta+deltaPhi*deltaPhi);
       
       if(deltaPhi > Geom::pi()) deltaPhi -= 2.*Geom::pi();
       if(deltaPhi < -Geom::pi()) deltaPhi += 2.*Geom::pi();
       
       if(deltaR < .2)  {ptisoatecal02  += sqrt(itTrack->innerMomentum().Mag2()); ntrkisoatecal02++; }
       if(deltaR < .25) {ptisoatecal025 += sqrt(itTrack->innerMomentum().Mag2()); ntrkisoatecal025++;}
       if(deltaR < .3)  {ptisoatecal03  += sqrt(itTrack->innerMomentum().Mag2()); ntrkisoatecal03++; }
       if(deltaR < .35) {ptisoatecal035 += sqrt(itTrack->innerMomentum().Mag2()); ntrkisoatecal035++;}
       if(deltaR < .4)  {ptisoatecal04  += sqrt(itTrack->innerMomentum().Mag2()); ntrkisoatecal04++; }
       
     }
     
     ptiso02Phot[nPhot] = ptiso02;
     ntrkiso02Phot[nPhot] = ntrkiso02;
     ptiso025Phot[nPhot] = ptiso025;
     ntrkiso025Phot[nPhot] = ntrkiso025;
     ptiso03Phot[nPhot] = ptiso03;
     ntrkiso03Phot[nPhot] = ntrkiso03;
     ptiso035Phot[nPhot] = ptiso035;
     ntrkiso035Phot[nPhot] = ntrkiso035;
     ptiso04Phot[nPhot] = ptiso04;
     ntrkiso04Phot[nPhot] = ntrkiso04;

     ptisoatecal02Phot[nPhot] = ptisoatecal02;
     ntrkisoatecal02Phot[nPhot] = ntrkisoatecal02;
     ptisoatecal025Phot[nPhot] = ptisoatecal025;
     ntrkisoatecal025Phot[nPhot] = ntrkisoatecal025;
     ptisoatecal03Phot[nPhot] = ptisoatecal03;
     ntrkisoatecal03Phot[nPhot] = ntrkisoatecal03;
     ptisoatecal035Phot[nPhot] = ptisoatecal035;
     ntrkisoatecal035Phot[nPhot] = ntrkisoatecal035;
     ptisoatecal04Phot[nPhot] = ptisoatecal04;
     ntrkisoatecal04Phot[nPhot] = ntrkisoatecal04;

     // calculate HCAL isolation
  
     double hcalEnergy = 0;
     CaloConeSelector selector1(0.1, geometry, DetId::Hcal);
     std::auto_ptr<CaloRecHitMetaCollectionV> selected=selector1.select(it->eta(),it->phi(),mhbhe);
     for (CaloRecHitMetaCollectionV::const_iterator hit=selected->begin(); hit != selected->end(); ++hit) hcalEnergy += hit->energy();
     hcalovecal01Phot[nPhot] = hcalEnergy/it->energy();
     hcalEnergy = 0;
     CaloConeSelector selector2(0.2, geometry, DetId::Hcal);
     selected = selector2.select(it->eta(),it->phi(),mhbhe);
     for (CaloRecHitMetaCollectionV::const_iterator hit=selected->begin(); hit != selected->end(); ++hit) hcalEnergy += hit->energy();
     hcalovecal02Phot[nPhot] = hcalEnergy/it->energy();
     hcalEnergy = 0;
     CaloConeSelector selector25(0.25, geometry, DetId::Hcal);
     selected = selector25.select(it->eta(),it->phi(),mhbhe);
     for (CaloRecHitMetaCollectionV::const_iterator hit=selected->begin(); hit != selected->end(); ++hit) hcalEnergy += hit->energy();
     hcalovecal025Phot[nPhot] = hcalEnergy/it->energy();
     hcalEnergy = 0;  
     CaloConeSelector selector3(0.3, geometry, DetId::Hcal); 
     selected = selector3.select(it->eta(),it->phi(),mhbhe); 
     for (CaloRecHitMetaCollectionV::const_iterator hit=selected->begin(); hit != selected->end(); ++hit) hcalEnergy += hit->energy(); 
     hcalovecal03Phot[nPhot] = hcalEnergy/it->energy(); 
     hcalEnergy = 0; 
     CaloConeSelector selector4(0.4, geometry, DetId::Hcal); 
     selected = selector4.select(it->eta(),it->phi(),mhbhe); 
     for (CaloRecHitMetaCollectionV::const_iterator hit=selected->begin(); hit != selected->end(); ++hit) hcalEnergy += hit->energy(); 
     hcalovecal04Phot[nPhot] = hcalEnergy/it->energy(); 
     hcalEnergy = 0;  
     CaloConeSelector selector5(0.5, geometry, DetId::Hcal);  
     selected = selector5.select(it->eta(),it->phi(),mhbhe);  
     for (CaloRecHitMetaCollectionV::const_iterator hit=selected->begin(); hit != selected->end(); ++hit) hcalEnergy += hit->energy();  
     hcalovecal05Phot[nPhot] = hcalEnergy/it->energy();  
 
    // calculate ECAL isolation 

     double ecalEnergy = 0; 
     CaloConeSelector selectorecal2(0.2, geometry, DetId::Ecal); 
     selected=selectorecal2.select(it->eta(),it->phi(),mecalhits); 
     for (CaloRecHitMetaCollectionV::const_iterator hit=selected->begin(); hit != selected->end(); ++hit) ecalEnergy += hit->energy(); 
     ecaliso02Phot[nPhot] = ecalEnergy; 
     ecalEnergy = 0;  
     CaloConeSelector selectorecal3(0.3, geometry, DetId::Ecal);  
     selected=selectorecal3.select(it->eta(),it->phi(),mecalhits);  
     for (CaloRecHitMetaCollectionV::const_iterator hit=selected->begin(); hit != selected->end(); ++hit) ecalEnergy += hit->energy();  
     ecaliso03Phot[nPhot] = ecalEnergy;  
     ecalEnergy = 0;   
     CaloConeSelector selectorecal35(0.35, geometry, DetId::Ecal);   
     selected=selectorecal35.select(it->eta(),it->phi(),mecalhits);   
     for (CaloRecHitMetaCollectionV::const_iterator hit=selected->begin(); hit != selected->end(); ++hit) ecalEnergy += hit->energy();   
     ecaliso035Phot[nPhot] = ecalEnergy;   
     ecalEnergy = 0;    
     CaloConeSelector selectorecal4(0.4, geometry, DetId::Ecal);    
     selected=selectorecal4.select(it->eta(),it->phi(),mecalhits);    
     for (CaloRecHitMetaCollectionV::const_iterator hit=selected->begin(); hit != selected->end(); ++hit) ecalEnergy += hit->energy();    
     ecaliso04Phot[nPhot] = ecalEnergy;    
     ecalEnergy = 0;     
     CaloConeSelector selectorecal5(0.5, geometry, DetId::Ecal);     
     selected=selectorecal5.select(it->eta(),it->phi(),mecalhits);     
     for (CaloRecHitMetaCollectionV::const_iterator hit=selected->begin(); hit != selected->end(); ++hit) ecalEnergy += hit->energy();     
     ecaliso05Phot[nPhot] = ecalEnergy;     
     
     // cluster shape variables
     
     BasicClusterRef tempCluster = it->superCluster()->seed();
     
     if(TMath::Abs(tempCluster->eta())<1.47){
       reco::ClusterShape tempShape=algo.Calculate(*tempCluster, rhits, &(*geometry_p), &(*topology_p),4.2);
       sMajMajPhot[nPhot]=tempShape.sMajMaj();
       sMinMinPhot[nPhot]=tempShape.sMinMin();
       FisherPhot[nPhot]=tempShape.fisher();
     }else{
       sMajMajPhot[nPhot]=-100.;
       sMinMinPhot[nPhot]=-100.;
       FisherPhot[nPhot]=-100.;
     }

     nPhot++;
    
   }
   if(maxptphoton1>-900.) PtPhoton1st->Fill(maxptphoton1);
   if(maxptphoton2>-900.) PtPhoton2st->Fill(maxptphoton2);
   if(maxptphoton3>-900.) PtPhoton3st->Fill(maxptphoton3);

   // Loop over reco Jets

   for ( CaloJetCollection::const_iterator it = jets->begin(); 
	it != jets->end(); it++ ) {

     if(nJet>=100) {cout << "number of reco jets is larger than 100. Skipping" << endl; continue;}
     pxJet[nJet] = it->px();	 
     pyJet[nJet] = it->py();	 
     pzJet[nJet] = it->pz();	 
     eJet[nJet] = it->energy();	 
     etaJet[nJet] = it->eta();	 
     phiJet[nJet] = it->phi();	      
     
     nJet++;
   }
   
   // Loop over gen Jets

   for ( GenJetCollection::const_iterator it = jetsgen->begin(); 
	it != jetsgen->end(); it++ ) {

     if(nJetGen>=100) {cout << "number of gen jets is larger than 100. Skipping" << endl; continue;}
     pxJetGen[nJetGen] = it->px();	 
     pyJetGen[nJetGen] = it->py();	 
     pzJetGen[nJetGen] = it->pz();	 
     eJetGen[nJetGen] = it->energy();	 
     etaJetGen[nJetGen] = it->eta();	 
     phiJetGen[nJetGen] = it->phi();	      
     
     nJetGen++;
   }

   // Fill reco MET

//    const CaloMETCollection *calometcol = calomethandle.product();
//    const CaloMET calomet = calometcol->front();

//    pxMet = calomet.px();	 
//    pyMet = calomet.py();	 
//    eMet = calomet.energy();	 
//    phiMet = calomet.phi();	      
     
   // Fill gen MET

   const GenMETCollection *genmetcol = genmethandle.product();
   const GenMET genmet = genmetcol->front();

   pxMetGen = genmet.px();	 
   pyMetGen = genmet.py();	 
   eMetGen = genmet.energy();	 
   phiMetGen = genmet.phi();	      
   
   event++;  
   m_tree->Fill();

}


// ------------ method called once each job just before starting event loop  ------------
void 
GammaJetAnalyzer::beginJob(const edm::EventSetup&)
{
  m_tree = new TTree ("pippo","Analysis tree") ;
  //  m_tree->SetAutoSave (10000000) ;
  m_tree->Branch("event",&event,"event/I");
  m_tree->Branch("nMC",&nMC,"nMC/I");
  m_tree->Branch("pdgIdMC",&pdgIdMC,"pdgIdMC[nMC]/I");
//   m_tree->Branch("massMC ",&massMC ,"massMC[nMC]/F");
  m_tree->Branch("statusMC",&statusMC,"statusMC[nMC]/I");
  m_tree->Branch("motherIDMC",&motherIDMC,"motherIDMC[nMC]/I");
  m_tree->Branch("pxMC ",&pxMC ,"pxMC[nMC]/F");
  m_tree->Branch("pyMC ",&pyMC ,"pyMC[nMC]/F");
  m_tree->Branch("pzMC ",&pzMC ,"pzMC[nMC]/F");
  m_tree->Branch("eMC  ",&eMC  ,"eMC[nMC]/F");
  m_tree->Branch("etaMC",&etaMC,"etaMC[nMC]/F");
  m_tree->Branch("phiMC",&phiMC,"phiMC[nMC]/F");

  m_tree->Branch("nPhot",&nPhot,"nPhot/I");
  m_tree->Branch("pxPhot ",&pxPhot ,"pxPhot[nPhot]/F");
  m_tree->Branch("pyPhot ",&pyPhot ,"pyPhot[nPhot]/F");
  m_tree->Branch("pzPhot ",&pzPhot ,"pzPhot[nPhot]/F");
  m_tree->Branch("ePhot  ",&ePhot  ,"ePhot[nPhot]/F");
  m_tree->Branch("etaPhot",&etaPhot,"etaPhot[nPhot]/F");
  m_tree->Branch("phiPhot",&phiPhot,"phiPhot[nPhot]/F");
  m_tree->Branch("ptiso02Phot",&ptiso02Phot,"ptiso02Phot[nPhot]/F");
  m_tree->Branch("ntrkiso02Phot",&ntrkiso02Phot,"ntrkiso02Phot[nPhot]/I");
  m_tree->Branch("ptiso025Phot",&ptiso02Phot,"ptiso025Phot[nPhot]/F");
  m_tree->Branch("ntrkiso025Phot",&ntrkiso02Phot,"ntrkiso025Phot[nPhot]/I");
  m_tree->Branch("ptiso03Phot",&ptiso02Phot,"ptiso03Phot[nPhot]/F");
  m_tree->Branch("ntrkiso03Phot",&ntrkiso03Phot,"ntrkiso03Phot[nPhot]/I");
  m_tree->Branch("ptiso035Phot",&ptiso035Phot,"ptiso035Phot[nPhot]/F");
  m_tree->Branch("ntrkiso035Phot",&ntrkiso035Phot,"ntrkiso035Phot[nPhot]/I");
  m_tree->Branch("ptiso04Phot",&ptiso04Phot,"ptiso04Phot[nPhot]/F");
  m_tree->Branch("ptisoatecal02Phot",&ptisoatecal02Phot,"ptisoatecal02Phot[nPhot]/F");
  m_tree->Branch("ntrkisoatecal02Phot",&ntrkisoatecal02Phot,"ntrkisoatecal02Phot[nPhot]/I");
  m_tree->Branch("ptisoatecal025Phot",&ptisoatecal02Phot,"ptisoatecal025Phot[nPhot]/F");
  m_tree->Branch("ntrkisoatecal025Phot",&ntrkisoatecal02Phot,"ntrkisoatecal025Phot[nPhot]/I");
  m_tree->Branch("ptisoatecal03Phot",&ptisoatecal02Phot,"ptisoatecal03Phot[nPhot]/F");
  m_tree->Branch("ntrkisoatecal03Phot",&ntrkisoatecal03Phot,"ntrkisoatecal03Phot[nPhot]/I");
  m_tree->Branch("ptisoatecal035Phot",&ptisoatecal035Phot,"ptisoatecal035Phot[nPhot]/F");
  m_tree->Branch("ntrkisoatecal035Phot",&ntrkisoatecal035Phot,"ntrkisoatecal035Phot[nPhot]/I");
  m_tree->Branch("ptisoatecal04Phot",&ptisoatecal04Phot,"ptisoatecal04Phot[nPhot]/F");
  m_tree->Branch("ntrkiso04Phot",&ntrkiso04Phot,"ntrkiso04Phot[nPhot]/I");
  m_tree->Branch("hcalovecal01Phot",&hcalovecal01Phot,"hcalovecal01Phot[nPhot]/F");
  m_tree->Branch("hcalovecal02Phot",&hcalovecal02Phot,"hcalovecal02Phot[nPhot]/F");
  m_tree->Branch("hcalovecal025Phot",&hcalovecal025Phot,"hcalovecal025Phot[nPhot]/F");
  m_tree->Branch("hcalovecal03Phot",&hcalovecal03Phot,"hcalovecal03Phot[nPhot]/F"); 
  m_tree->Branch("hcalovecal04Phot",&hcalovecal04Phot,"hcalovecal04Phot[nPhot]/F"); 
  m_tree->Branch("hcalovecal05Phot",&hcalovecal05Phot,"hcalovecal05Phot[nPhot]/F"); 
  m_tree->Branch("ecaliso02Phot",&ecaliso02Phot,"ecaliso02Phot[nPhot]/F"); 
  m_tree->Branch("ecaliso03Phot",&ecaliso03Phot,"ecaliso03Phot[nPhot]/F");  
  m_tree->Branch("ecaliso035Phot",&ecaliso035Phot,"ecaliso035Phot[nPhot]/F");  
  m_tree->Branch("ecaliso04Phot",&ecaliso04Phot,"ecaliso04Phot[nPhot]/F");  
  m_tree->Branch("ecaliso05Phot",&ecaliso05Phot,"ecaliso05Phot[nPhot]/F");  
  m_tree->Branch("LATPhot",&LATPhot,"LATPhot[nPhot]/F");
  m_tree->Branch("sMajMajPhot",&sMajMajPhot,"sMajMaj2Phot[nPhot]/F");
  m_tree->Branch("sMinMinPhot",&sMinMinPhot,"sMinMin2Phot[nPhot]/F");
  m_tree->Branch("FisherPhot",&FisherPhot,"FisherPhot[nPhot]/F");

  m_tree->Branch("nJet",&nJet,"nJet/I");
  m_tree->Branch("pxJet ",&pxJet ,"pxJet[nJet]/F");
  m_tree->Branch("pyJet ",&pyJet ,"pyJet[nJet]/F");
  m_tree->Branch("pzJet ",&pzJet ,"pzJet[nJet]/F");
  m_tree->Branch("eJet  ",&eJet  ,"eJet[nJet]/F");
  m_tree->Branch("etaJet",&etaJet,"etaJet[nJet]/F");
  m_tree->Branch("phiJet",&phiJet,"phiJet[nJet]/F");

  m_tree->Branch("nJetGen",&nJetGen,"nJetGen/I");
  m_tree->Branch("pxJetGen ",&pxJetGen ,"pxJetGen[nJetGen]/F");
  m_tree->Branch("pyJetGen ",&pyJetGen ,"pyJetGen[nJetGen]/F");
  m_tree->Branch("pzJetGen ",&pzJetGen ,"pzJetGen[nJetGen]/F");
  m_tree->Branch("eJetGen  ",&eJetGen  ,"eJetGen[nJetGen]/F");
  m_tree->Branch("etaJetGen",&etaJetGen,"etaJetGen[nJetGen]/F");
  m_tree->Branch("phiJetGen",&phiJetGen,"phiJetGen[nJetGen]/F");

  m_tree->Branch("pxMet ",&pxMet ,"pxMet/F");
  m_tree->Branch("pyMet ",&pyMet ,"pyMet/F");
  m_tree->Branch("pzMet ",&pzMet ,"pzMet/F");
  m_tree->Branch("eMet  ",&eMet  ,"eMet/F");
  m_tree->Branch("etaMet",&etaMet,"etaMet/F");
  m_tree->Branch("phiMet",&phiMet,"phiMet/F");

  m_tree->Branch("pxMetGen ",&pxMetGen ,"pxMetGen/F");
  m_tree->Branch("pyMetGen ",&pyMetGen ,"pyMetGen/F");
  m_tree->Branch("pzMetGen ",&pzMetGen ,"pzMetGen/F");
  m_tree->Branch("eMetGen  ",&eMetGen  ,"eMetGen/F");
  m_tree->Branch("etaMetGen",&etaMetGen,"etaMetGen/F");
  m_tree->Branch("phiMetGen",&phiMetGen,"phiMetGen/F");

  event = 0;  
}

// ------------ method called once each job just after ending the event loop  ------------
void 
GammaJetAnalyzer::endJob() {

   m_tree->Write();

   hOutputFile->Write() ;
   hOutputFile->Close() ;
}


//define this as a plug-in
DEFINE_FWK_MODULE(GammaJetAnalyzer);
