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
// $Id: GammaJetAnalyzer.cc,v 1.7 2009/02/12 12:13:58 delre Exp $
//
//

//
// constructors and destructor
//

#include "JetMETCorrections/GammaJet/interface/GammaJetAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloTopology/interface/CaloSubdetectorTopology.h"
#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"

#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/PhotonFwd.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
//#include "DataFormats/JetReco/interface/CalogJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaReco/interface/ClusterShape.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "DataFormats/EgammaCandidates/interface/ConversionFwd.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h" 
#include "RecoCaloTools/Selectors/interface/CaloConeSelector.h"
#include "RecoCaloTools/MetaCollections/interface/CaloRecHitMetaCollections.h"

#include "RecoEcal/EgammaCoreTools/interface/ClusterShapeAlgo.h"

#include "MyAnalysis/IsolationTools/interface/SuperClusterHitsEcalIsolation.h"

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

// HLT trigger
#include "FWCore/Framework/interface/TriggerNamesService.h"
#include <FWCore/Framework/interface/TriggerNames.h>
#include <DataFormats/Common/interface/TriggerResults.h>
#define MAXHLTBITS    200

#include "RecoEgamma/EgammaTools/interface/ECALPositionCalculator.h"

using namespace reco;

GammaJetAnalyzer::GammaJetAnalyzer(const edm::ParameterSet& iConfig)
{
  MCTruthCollection_ = iConfig.getUntrackedParameter<edm::InputTag>("MCTruthCollection");
  triggerTag_ = iConfig.getUntrackedParameter<edm::InputTag>("TriggerTag");
  trackTags_ = iConfig.getUntrackedParameter<edm::InputTag>("tracks");
  Photonsrc_ = iConfig.getUntrackedParameter<edm::InputTag>( "Photonsrc" );
  Jetsrcite_ = iConfig.getUntrackedParameter<edm::InputTag>( "jetsite" );
  Jetsrckt4_ = iConfig.getUntrackedParameter<edm::InputTag>( "jetskt4" );
  Jetsrckt6_ = iConfig.getUntrackedParameter<edm::InputTag>( "jetskt6" );
  Jetsrcsis5_ = iConfig.getUntrackedParameter<edm::InputTag>( "jetssis5" );
  Jetsrcsis7_ = iConfig.getUntrackedParameter<edm::InputTag>( "jetssis7" );
  Pfjetsrc_ = iConfig.getUntrackedParameter<edm::InputTag>( "pfjets" );
  JetGensrcite_ = iConfig.getUntrackedParameter<edm::InputTag>( "jetsgenite" );
  JetGensrckt4_ = iConfig.getUntrackedParameter<edm::InputTag>( "jetsgenkt4" );
  JetGensrckt6_ = iConfig.getUntrackedParameter<edm::InputTag>( "jetsgenkt6" );
  JetGensrcsis5_ = iConfig.getUntrackedParameter<edm::InputTag>( "jetsgensis5" );
  JetGensrcsis7_ = iConfig.getUntrackedParameter<edm::InputTag>( "jetsgensis7" );
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
   nMC = nPhot = nconvPhot = nJet_ite = nJet_kt4 = nJet_sis5 = nJet_kt6 = nJet_sis7 = nJet_pfite = nJetGen_ite = nJetGen_kt4 = nJetGen_sis5 = nJetGen_kt6 = nJetGen_sis7 = 0;

   using reco::TrackCollection;
  
   double maxptphoton1(0), maxptphoton2(0), maxptphoton3(0);

   //   // get generated pt hat
   //   Handle<double> genEventScale; 
   //   iEvent.getByLabel( "genEventScale", genEventScale ); 

   Handle<GenEventInfoProduct> hEventInfo;
   iEvent.getByLabel("generator", hEventInfo);

   // get MC info from GenParticleCandidates 
   Handle<GenParticleCollection> genParticles;
   iEvent.getByLabel( "genParticles", genParticles );
   
   // get tracks
   Handle<TrackCollection> tracks;
   iEvent.getByLabel(trackTags_,tracks);

   // get photons
   Handle<PhotonCollection>  PhotonHandle;
   iEvent.getByLabel( Photonsrc_, PhotonHandle );

   // get converted photons
   Handle<ConversionCollection> convertedPhotonHandle; // get the Converted Photon info
   iEvent.getByLabel("conversions", "", convertedPhotonHandle);
   const reco::ConversionCollection convphoCollection = *(convertedPhotonHandle.product());

   // get calo jet collection
   Handle<CaloJetCollection> jetsite;
   iEvent.getByLabel(Jetsrcite_, jetsite);
   Handle<CaloJetCollection> jetskt4;
   iEvent.getByLabel(Jetsrckt4_, jetskt4);
   Handle<CaloJetCollection> jetskt6;
   iEvent.getByLabel(Jetsrckt6_, jetskt6);
   Handle<CaloJetCollection> jetssis5;
   iEvent.getByLabel(Jetsrcsis5_, jetssis5);
   Handle<CaloJetCollection> jetssis7;
   iEvent.getByLabel(Jetsrcsis7_, jetssis7);

   // get PF jets collection
   Handle<PFJetCollection> pfjetsite;
   iEvent.getByLabel(Pfjetsrc_, pfjetsite);   

   // get gen jet collection
   Handle<GenJetCollection> jetsgenite;
   iEvent.getByLabel(JetGensrcite_, jetsgenite);
   Handle<GenJetCollection> jetsgenkt4;
   iEvent.getByLabel(JetGensrckt4_, jetsgenkt4);
   Handle<GenJetCollection> jetsgenkt6;
   iEvent.getByLabel(JetGensrckt6_, jetsgenkt6);
   Handle<GenJetCollection> jetsgensis5;
   iEvent.getByLabel(JetGensrcsis5_, jetsgensis5);
   Handle<GenJetCollection> jetsgensis7;
   iEvent.getByLabel(JetGensrcsis7_, jetsgensis7);

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

   Handle<EERecHitCollection> ecalhitsee;
   const EERecHitCollection* rhitsee=0;
   iEvent.getByLabel( recoProducer_, "EcalRecHitsEE", ecalhitsee);
   rhitsee = ecalhits.product(); // get a ptr to the product

   // get geometry
   edm::ESHandle<CaloGeometry> geoHandle;
   //   iSetup.get<IdealGeometryRecord>().get(geoHandle);
   iSetup.get<CaloGeometryRecord>().get(geoHandle);
   const CaloGeometry* geometry = geoHandle.product();
   const CaloSubdetectorGeometry* geometry_p = geoHandle->getSubdetectorGeometry(DetId::Ecal, EcalBarrel);

   // get topology
   const CaloSubdetectorTopology *topology_p;
   edm::ESHandle<CaloTopology> topoHandle;
   iSetup.get<CaloTopologyRecord>().get(topoHandle);
   topology_p = topoHandle->getSubdetectorTopology(DetId::Ecal, EcalBarrel);

   //   edm::ESHandle<TrackerGeometry> trackerHandle_;
   edm::ESHandle<MagneticField> theMagField;
   iSetup.get<IdealMagneticFieldRecord>().get(theMagField);
   //   iSetup.get<TrackerDigiGeometryRecord>().get(trackerHandle_);
  
   ClusterShapeAlgo algo;

//---------------------------HLT Trigger ---------------------------------------------------------------------------------------------
// You Can See HLT Name list ->  " JetMETCorrections/GammaJet/test/HLTList.txt " file 

   for(unsigned int iHLT=0; iHLT<MAXHLTBITS; ++iHLT) 
   {
   	aHLTResults[iHLT] = false;
   }
   
   strcpy(aHLTNames,"");
   hltNamesLen = 0;

   edm::Handle<edm::TriggerResults> hltTriggerResultHandle;
   iEvent.getByLabel(triggerTag_, hltTriggerResultHandle);

   if(!hltTriggerResultHandle.isValid()) 
   {
   	std::cout << "invalid handle for HLT TriggerResults" << std::endl;
   } 
   else 
   {
   	edm::TriggerNames HLTNames;
   	HLTNames.init(*hltTriggerResultHandle);
   
   	std::string tempnames;  
   	hltCount = hltTriggerResultHandle->size();
   	//std::cout << "hltTriggerResult->size(): " << hltCount << std::endl;

   	for(int i = 0 ; i < hltCount ; i++) 
   	{
   		tempnames += HLTNames.triggerName(i) + ":";
   		aHLTResults[i] = hltTriggerResultHandle->accept(i);
   		//cout << i <<"....." << HLTNames.triggerName(i).c_str() << ".... : " << hltTriggerResultHandle->accept(i) << endl;
   	}

   	hltNamesLen = tempnames.length();
   	strcpy(aHLTNames,tempnames.c_str());

   }
//--------------------------------------------------------------------------------------------------------------------------------------

   // Loop over MC truth
   
   //   genpt = *genEventScale;   
   if (hEventInfo->binningValues().size() > 0)
     genpt = hEventInfo->binningValues()[0];

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
       
       double etaTrue = p->eta();
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
     escPhot[nPhot] = it->superCluster()->energy();	 
     etaPhot[nPhot] = it->eta();	 
     phiPhot[nPhot] = it->phi();	      
     
     double ptphoton = it->pt(); 
     if (ptphoton>maxptphoton1) {
       maxptphoton1 = ptphoton;
     }
     if (ptphoton>maxptphoton2 && ptphoton<maxptphoton1) maxptphoton2 = ptphoton;
     if (ptphoton>maxptphoton3 && ptphoton<maxptphoton2) maxptphoton3 = ptphoton;     
     
     double ptiso0015(0.),  ptiso035(0.), ptiso05(0.), ptiso07(0.), ptiso1(0.);
     int ntrkiso0015(0), ntrkiso035(0), ntrkiso05(0), ntrkiso07(0), ntrkiso1(0);
//      double ptisoatecal02(0.), ptisoatecal025(0.), ptisoatecal03(0.), ptisoatecal035(0.), ptisoatecal04(0.);
//      int ntrkisoatecal02(0), ntrkisoatecal025(0), ntrkisoatecal03(0), ntrkisoatecal035(0), ntrkisoatecal04(0);
     
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
       
       if(deltaR < .015)  {ptiso0015  += sqrt(itTrack->innerMomentum().Mag2()); ntrkiso0015++; }
       if(deltaR < .35)   {ptiso035 += sqrt(itTrack->innerMomentum().Mag2()); ntrkiso035++;}
       if(deltaR < .5)    {ptiso05  += sqrt(itTrack->innerMomentum().Mag2()); ntrkiso05++; }
       if(deltaR < .7)    {ptiso07 += sqrt(itTrack->innerMomentum().Mag2()); ntrkiso07++;}
       if(deltaR < 1.)    {ptiso1  += sqrt(itTrack->innerMomentum().Mag2()); ntrkiso1++; }
       

       // Trk isolation at ECAL commented for now

//        const math::XYZVector tempvec = itTrack->innerMomentum();
//        const math::XYZPoint temppos = itTrack->innerPosition();
//        int tempcharge = itTrack->charge();
       
//        ECALPositionCalculator tempcalculator;      
       
//        double phitrk = tempcalculator.ecalPhi(tempvec,temppos,tempcharge);
//        double etatrk = tempcalculator.ecalEta(tempvec,temppos);
       
//        deltaPhi = phitrk-it->phi();
//        deltaEta = etatrk-it->eta();      
       
//        deltaR = std::sqrt(deltaEta*deltaEta+deltaPhi*deltaPhi);
       
//        if(deltaPhi > Geom::pi()) deltaPhi -= 2.*Geom::pi();
//        if(deltaPhi < -Geom::pi()) deltaPhi += 2.*Geom::pi();
       
//        if(deltaR < .2)  {ptisoatecal02  += sqrt(itTrack->innerMomentum().Mag2()); ntrkisoatecal02++; }
//        if(deltaR < .25) {ptisoatecal025 += sqrt(itTrack->innerMomentum().Mag2()); ntrkisoatecal025++;}
//        if(deltaR < .3)  {ptisoatecal03  += sqrt(itTrack->innerMomentum().Mag2()); ntrkisoatecal03++; }
//        if(deltaR < .35) {ptisoatecal035 += sqrt(itTrack->innerMomentum().Mag2()); ntrkisoatecal035++;}
//        if(deltaR < .4)  {ptisoatecal04  += sqrt(itTrack->innerMomentum().Mag2()); ntrkisoatecal04++; }
       
     }
     
     ptiso0015Phot[nPhot] = ptiso0015;
     ntrkiso0015Phot[nPhot] = ntrkiso0015;
     ptiso035Phot[nPhot] = ptiso035;
     ntrkiso035Phot[nPhot] = ntrkiso035;
     ptiso05Phot[nPhot] = ptiso05;
     ntrkiso05Phot[nPhot] = ntrkiso05;
     ptiso07Phot[nPhot] = ptiso07;
     ntrkiso07Phot[nPhot] = ntrkiso07;
     ptiso1Phot[nPhot] = ptiso1;
     ntrkiso1Phot[nPhot] = ntrkiso1;

//      ptisoatecal02Phot[nPhot] = ptisoatecal02;
//      ntrkisoatecal02Phot[nPhot] = ntrkisoatecal02;
//      ptisoatecal025Phot[nPhot] = ptisoatecal025;
//      ntrkisoatecal025Phot[nPhot] = ntrkisoatecal025;
//      ptisoatecal03Phot[nPhot] = ptisoatecal03;
//      ntrkisoatecal03Phot[nPhot] = ntrkisoatecal03;
//      ptisoatecal035Phot[nPhot] = ptisoatecal035;
//      ntrkisoatecal035Phot[nPhot] = ntrkisoatecal035;
//      ptisoatecal04Phot[nPhot] = ptisoatecal04;
//      ntrkisoatecal04Phot[nPhot] = ntrkisoatecal04;

     // calculate HCAL isolation
  
     double hcalEnergy = 0;
     reco::SuperClusterRef sc = it->get<reco::SuperClusterRef>();
     CaloConeSelector selector1(0.1, geometry, DetId::Hcal);
     std::auto_ptr<CaloRecHitMetaCollectionV> selected=selector1.select(sc->eta(),sc->phi(),mhbhe);
     for (CaloRecHitMetaCollectionV::const_iterator hit=selected->begin(); hit != selected->end(); ++hit) hcalEnergy += hit->energy();
     hcalovecal01Phot[nPhot] = hcalEnergy/it->energy();
     hcalEnergy = 0;
     CaloConeSelector selector15(0.15, geometry, DetId::Hcal);
     selected = selector15.select(sc->eta(),sc->phi(),mhbhe);
     for (CaloRecHitMetaCollectionV::const_iterator hit=selected->begin(); hit != selected->end(); ++hit) hcalEnergy += hit->energy();
     hcalovecal015Phot[nPhot] = hcalEnergy/it->energy();
     hcalEnergy = 0;
     CaloConeSelector selector4(0.4, geometry, DetId::Hcal); 
     selected = selector4.select(sc->eta(),sc->phi(),mhbhe); 
     for (CaloRecHitMetaCollectionV::const_iterator hit=selected->begin(); hit != selected->end(); ++hit) hcalEnergy += hit->energy(); 
     hcalovecal04Phot[nPhot] = hcalEnergy/it->energy(); 
     hcalEnergy = 0;  
     CaloConeSelector selector5(0.5, geometry, DetId::Hcal);  
     selected = selector5.select(sc->eta(),sc->phi(),mhbhe);  
     for (CaloRecHitMetaCollectionV::const_iterator hit=selected->begin(); hit != selected->end(); ++hit) hcalEnergy += hit->energy();  
     hcalovecal05Phot[nPhot] = hcalEnergy/it->energy();  
     CaloConeSelector selector7(0.7, geometry, DetId::Hcal);
     selected = selector7.select(sc->eta(),sc->phi(),mhbhe);
     for (CaloRecHitMetaCollectionV::const_iterator hit=selected->begin(); hit != selected->end(); ++hit) hcalEnergy += hit->energy();
     hcalovecal07Phot[nPhot] = hcalEnergy/it->energy();
     hcalEnergy = 0;  
     CaloConeSelector selector10(1., geometry, DetId::Hcal); 
     selected = selector10.select(sc->eta(),sc->phi(),mhbhe); 
     for (CaloRecHitMetaCollectionV::const_iterator hit=selected->begin(); hit != selected->end(); ++hit) hcalEnergy += hit->energy(); 
     hcalovecal1Phot[nPhot] = hcalEnergy/it->energy(); 
     hcalEnergy = 0; 
 
     // calculate ECAL isolation 

     // ecal isolation with SC rechits removal
     SuperClusterHitsEcalIsolation scBasedIsolation(rhits,rhitsee);
     scBasedIsolation.setExtRadius(0.1);
     scBasedIsolation.excludeHalo(false);
     ecaliso01Phot[nPhot]  = scBasedIsolation.getSum(iEvent,iSetup,&(*sc));
     scBasedIsolation.setExtRadius(0.15);
     ecaliso015Phot[nPhot]  = scBasedIsolation.getSum(iEvent,iSetup,&(*sc));
     scBasedIsolation.setExtRadius(0.4);
     ecaliso04Phot[nPhot]  = scBasedIsolation.getSum(iEvent,iSetup,&(*sc));
     scBasedIsolation.setExtRadius(0.5);
     ecaliso05Phot[nPhot]  = scBasedIsolation.getSum(iEvent,iSetup,&(*sc));
     scBasedIsolation.setExtRadius(0.7);
     ecaliso07Phot[nPhot]  = scBasedIsolation.getSum(iEvent,iSetup,&(*sc));
     scBasedIsolation.setExtRadius(1.);
     ecaliso1Phot[nPhot]  = scBasedIsolation.getSum(iEvent,iSetup,&(*sc));
     // leave also old implementation
     double ecalEnergy = 0;
     CaloConeSelector selectorecal2(0.4, geometry, DetId::Ecal);
     selected=selectorecal2.select(it->eta(),it->phi(),mecalhits);
     for (CaloRecHitMetaCollectionV::const_iterator hit=selected->begin(); hit != selected->end(); ++hit) ecalEnergy += hit->energy();
     ecaliso04oldPhot[nPhot] = ecalEnergy;
     

     // cluster shape variables
     
     //     BasicClusterRef tempCluster = it->superCluster()->seed();
     CaloClusterPtr tempCluster = it->superCluster()->seed();

     if(TMath::Abs(tempCluster->eta())<1.47){
       reco::ClusterShape tempShape=algo.Calculate(*tempCluster, rhits, &(*geometry_p), &(*topology_p),4.7);
       sMajMajPhot[nPhot]=tempShape.sMajMaj();
       sMinMinPhot[nPhot]=tempShape.sMinMin();
       FisherPhot[nPhot]=tempShape.fisher();
       alphaPhot[nPhot]=tempShape.s_alpha();
       sEtaEtaPhot[nPhot]=tempShape.sEtaEta();
       sPhiPhiPhot[nPhot]=tempShape.sPhiPhi();
       E1Phot[nPhot]=tempShape.eMax();
       E9Phot[nPhot]=tempShape.e3x3();
       E25Phot[nPhot]=tempShape.e5x5();
       // leave also old implementation
       tempShape=algo.Calculate(*tempCluster, rhits, &(*geometry_p), &(*topology_p),4.2);
       sMajMajoldPhot[nPhot]=tempShape.sMajMaj();
       sMinMinoldPhot[nPhot]=tempShape.sMinMin();
     }else{
       sMajMajPhot[nPhot]=-100.;
       sMinMinPhot[nPhot]=-100.;
       FisherPhot[nPhot]=-100.;
       alphaPhot[nPhot]=-100.;
       sEtaEtaPhot[nPhot]=-100.;
       sPhiPhiPhot[nPhot]=-100.;
       E1Phot[nPhot]=-100.;
       E9Phot[nPhot]=-100.;
       E25Phot[nPhot]=-100.;
       sMajMajoldPhot[nPhot]=-100.;
       sMinMinoldPhot[nPhot]=-100.;
     }

     nPhot++;
    
   }

   // loop on converted photons
   for( reco::ConversionCollection::const_iterator iCPho = convphoCollection.begin();
	iCPho != convphoCollection.end(); iCPho++) {
     if ((*iCPho).isConverted()){
       chi2convPhot[nconvPhot] = (*iCPho).conversionVertex().chi2();
       ndofconvPhot[nconvPhot] = (*iCPho).conversionVertex().ndof();
       xconvPhot[nconvPhot] = (*iCPho).conversionVertex().position().x();
       yconvPhot[nconvPhot] = (*iCPho).conversionVertex().position().y();
       zconvPhot[nconvPhot] = (*iCPho).conversionVertex().position().z();
       ntrkconvPhot[nconvPhot] = (*iCPho).tracks().size();
       eovpconvPhot[nconvPhot] = (*iCPho).EoverP();
       etaecalconvPhot[nconvPhot] = (*iCPho).caloCluster()[0]->eta(); 
       phiecalconvPhot[nconvPhot] = (*iCPho).caloCluster()[0]->phi(); 
       energyecalconvPhot[nconvPhot] =(*iCPho).caloCluster()[0]->energy();
       nconvPhot++;
     }
   } 

   if(maxptphoton1>-900.) PtPhoton1st->Fill(maxptphoton1);
   if(maxptphoton2>-900.) PtPhoton2st->Fill(maxptphoton2);
   if(maxptphoton3>-900.) PtPhoton3st->Fill(maxptphoton3);

   // Loop over reco Jets

   for ( CaloJetCollection::const_iterator it = jetsite->begin(); 
	 it != jetsite->end(); it++ ) {
     
     if(nJet_ite>=100) {cout << "number of reco jets ite is larger than 100. Skipping" << endl; continue;}
     pxJet_ite[nJet_ite] = it->px();	 
     pyJet_ite[nJet_ite] = it->py();	 
     pzJet_ite[nJet_ite] = it->pz();	 
     eJet_ite[nJet_ite] = it->energy();	 
     etaJet_ite[nJet_ite] = it->eta();	 
     phiJet_ite[nJet_ite] = it->phi();	      
     
     nJet_ite++;
   }
   
   for ( CaloJetCollection::const_iterator it = jetskt4->begin(); 
	 it != jetskt4->end(); it++ ) {
     
     if(nJet_kt4>=100) {cout << "number of reco jets kt 04 is larger than 100. Skipping" << endl; continue;}
     pxJet_kt4[nJet_kt4] = it->px();	 
     pyJet_kt4[nJet_kt4] = it->py();	 
     pzJet_kt4[nJet_kt4] = it->pz();	 
     eJet_kt4[nJet_kt4] = it->energy();	 
     etaJet_kt4[nJet_kt4] = it->eta();	 
     phiJet_kt4[nJet_kt4] = it->phi();	      
     
     nJet_kt4++;
   }
   
   for ( CaloJetCollection::const_iterator it = jetskt6->begin(); 
	 it != jetskt6->end(); it++ ) {
     
     if(nJet_kt6>=100) {cout << "number of reco jets kt 06 is larger than 100. Skipping" << endl; continue;}
     pxJet_kt6[nJet_kt6] = it->px();	 
     pyJet_kt6[nJet_kt6] = it->py();	 
     pzJet_kt6[nJet_kt6] = it->pz();	 
     eJet_kt6[nJet_kt6] = it->energy();	 
     etaJet_kt6[nJet_kt6] = it->eta();	 
     phiJet_kt6[nJet_kt6] = it->phi();	      
     
     nJet_kt6++;
   }
   
   for ( CaloJetCollection::const_iterator it = jetssis5->begin(); 
	 it != jetssis5->end(); it++ ) {
     
     if(nJet_sis5>=100) {cout << "number of reco jets sis 05 is larger than 100. Skipping" << endl; continue;}
     pxJet_sis5[nJet_sis5] = it->px();	 
     pyJet_sis5[nJet_sis5] = it->py();	 
     pzJet_sis5[nJet_sis5] = it->pz();	 
     eJet_sis5[nJet_sis5] = it->energy();	 
     etaJet_sis5[nJet_sis5] = it->eta();	 
     phiJet_sis5[nJet_sis5] = it->phi();	      
     
     nJet_sis5++;
   }

   for ( CaloJetCollection::const_iterator it = jetssis7->begin(); 
	 it != jetssis7->end(); it++ ) {
     
     if(nJet_sis7>=100) {cout << "number of reco jets sis 07 is larger than 100. Skipping" << endl; continue;}
     pxJet_sis7[nJet_sis7] = it->px();	 
     pyJet_sis7[nJet_sis7] = it->py();	 
     pzJet_sis7[nJet_sis7] = it->pz();	 
     eJet_sis7[nJet_sis7] = it->energy();	 
     etaJet_sis7[nJet_sis7] = it->eta();	 
     phiJet_sis7[nJet_sis7] = it->phi();	      
     
     nJet_sis7++;
   }
   
   for ( PFJetCollection::const_iterator it = pfjetsite->begin(); 
	 it != pfjetsite->end(); it++ ) {
     
     if(nJet_pfite>=100) {cout << "number of reco jets pfite is larger than 100. Skipping" << endl; continue;}
     pxJet_pfite[nJet_pfite] = it->px();	 
     pyJet_pfite[nJet_pfite] = it->py();	 
     pzJet_pfite[nJet_pfite] = it->pz();	 
     eJet_pfite[nJet_pfite] = it->energy();	 
     etaJet_pfite[nJet_pfite] = it->eta();	 
     phiJet_pfite[nJet_pfite] = it->phi();	      
     
     nJet_pfite++;
   }
   
   // Loop over gen Jets

   for ( GenJetCollection::const_iterator it = jetsgenite->begin(); 
	it != jetsgenite->end(); it++ ) {

     if(nJetGen_ite>=100) {cout << "number of gen jets ite is larger than 100. Skipping" << endl; continue;}
     pxJetGen_ite[nJetGen_ite] = it->px();	 
     pyJetGen_ite[nJetGen_ite] = it->py();	 
     pzJetGen_ite[nJetGen_ite] = it->pz();	 
     eJetGen_ite[nJetGen_ite] = it->energy();	 
     etaJetGen_ite[nJetGen_ite] = it->eta();	 
     phiJetGen_ite[nJetGen_ite] = it->phi();	      
     
     nJetGen_ite++;
   }

   for ( GenJetCollection::const_iterator it = jetsgenkt4->begin(); 
	it != jetsgenkt4->end(); it++ ) {

     if(nJetGen_kt4>=100) {cout << "number of gen jets kt 04 is larger than 100. Skipping" << endl; continue;}
     pxJetGen_kt4[nJetGen_kt4] = it->px();	 
     pyJetGen_kt4[nJetGen_kt4] = it->py();	 
     pzJetGen_kt4[nJetGen_kt4] = it->pz();	 
     eJetGen_kt4[nJetGen_kt4] = it->energy();	 
     etaJetGen_kt4[nJetGen_kt4] = it->eta();	 
     phiJetGen_kt4[nJetGen_kt4] = it->phi();	      
     
     nJetGen_kt4++;
   }

   for ( GenJetCollection::const_iterator it = jetsgenkt6->begin(); 
	it != jetsgenkt6->end(); it++ ) {

     if(nJetGen_kt6>=100) {cout << "number of gen jets kt 06 is larger than 100. Skipping" << endl; continue;}
     pxJetGen_kt6[nJetGen_kt6] = it->px();	 
     pyJetGen_kt6[nJetGen_kt6] = it->py();	 
     pzJetGen_kt6[nJetGen_kt6] = it->pz();	 
     eJetGen_kt6[nJetGen_kt6] = it->energy();	 
     etaJetGen_kt6[nJetGen_kt6] = it->eta();	 
     phiJetGen_kt6[nJetGen_kt6] = it->phi();	      
     
     nJetGen_kt6++;
   }

   for ( GenJetCollection::const_iterator it = jetsgensis5->begin(); 
	it != jetsgensis5->end(); it++ ) {

     if(nJetGen_sis5>=100) {cout << "number of gen jets sis 05 is larger than 100. Skipping" << endl; continue;}
     pxJetGen_sis5[nJetGen_sis5] = it->px();	 
     pyJetGen_sis5[nJetGen_sis5] = it->py();	 
     pzJetGen_sis5[nJetGen_sis5] = it->pz();	 
     eJetGen_sis5[nJetGen_sis5] = it->energy();	 
     etaJetGen_sis5[nJetGen_sis5] = it->eta();	 
     phiJetGen_sis5[nJetGen_sis5] = it->phi();	      
     
     nJetGen_sis5++;
   }

   for ( GenJetCollection::const_iterator it = jetsgensis7->begin(); 
	it != jetsgensis7->end(); it++ ) {

     if(nJetGen_sis7>=100) {cout << "number of gen jets sis 07 is larger than 100. Skipping" << endl; continue;}
     pxJetGen_sis7[nJetGen_sis7] = it->px();	 
     pyJetGen_sis7[nJetGen_sis7] = it->py();	 
     pzJetGen_sis7[nJetGen_sis7] = it->pz();	 
     eJetGen_sis7[nJetGen_sis7] = it->energy();	 
     etaJetGen_sis7[nJetGen_sis7] = it->eta();	 
     phiJetGen_sis7[nJetGen_sis7] = it->phi();	      
     
     nJetGen_sis7++;
   }

// Fill reco MET

   const CaloMETCollection *calometcol = calomethandle.product();
   const CaloMET calomet = calometcol->front();
   
   pxMet = calomet.px();	 
   pyMet = calomet.py();	 
   eMet = calomet.energy();	 
   phiMet = calomet.phi();	      
     
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
  m_tree->Branch("genpt",&genpt,"genpt/F");
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
  m_tree->Branch("escPhot  ",&escPhot  ,"escPhot[nPhot]/F");
  m_tree->Branch("etaPhot",&etaPhot,"etaPhot[nPhot]/F");
  m_tree->Branch("phiPhot",&phiPhot,"phiPhot[nPhot]/F");

  m_tree->Branch("nconvPhot",&nconvPhot,"nconvPhot/I");
  m_tree->Branch("chi2convPhot",&chi2convPhot,"chi2convPhot[nconvPhot]/F");
  m_tree->Branch("ndofconvPhot",&ndofconvPhot,"ndofconvPhot[nconvPhot]/F");
  m_tree->Branch("xconvPhot",&xconvPhot,"xconvPhot[nconvPhot]/F");
  m_tree->Branch("yconvPhot",&yconvPhot,"yconvPhot[nconvPhot]/F");
  m_tree->Branch("zconvPhot",&zconvPhot,"zconvPhot[nconvPhot]/F");
  m_tree->Branch("ntrkconvPhot",&ntrkconvPhot,"ntrkconvPhot[nconvPhot]/I");
  m_tree->Branch("eovpconvPhot",&eovpconvPhot,"eovpconvPhot[nconvPhot]/F");
  m_tree->Branch("etaecalconvPhot",&etaecalconvPhot,"etaecalconvPhot[nconvPhot]/F");
  m_tree->Branch("phiecalconvPhot",&phiecalconvPhot,"phiecalconvPhot[nconvPhot]/F");
  m_tree->Branch("eecalconvPhot",&energyecalconvPhot,"energyecalconvPhot[nconvPhot]/F");

  m_tree->Branch("ptiso0015Phot",&ptiso0015Phot,"ptiso0015Phot[nPhot]/F");
  m_tree->Branch("ntrkiso0015Phot",&ntrkiso0015Phot,"ntrkiso0015Phot[nPhot]/I");
  m_tree->Branch("ptiso035Phot",&ptiso035Phot,"ptiso035Phot[nPhot]/F");
  m_tree->Branch("ntrkiso035Phot",&ntrkiso035Phot,"ntrkiso035Phot[nPhot]/I");
  m_tree->Branch("ptiso05Phot",&ptiso05Phot,"ptiso05Phot[nPhot]/F");
  m_tree->Branch("ntrkiso05Phot",&ntrkiso05Phot,"ntrkiso05Phot[nPhot]/I");
  m_tree->Branch("ptiso07Phot",&ptiso07Phot,"ptiso07Phot[nPhot]/F");
  m_tree->Branch("ntrkiso07Phot",&ntrkiso07Phot,"ntrkiso07Phot[nPhot]/I");
  m_tree->Branch("ptiso1Phot",&ptiso1Phot,"ptiso1Phot[nPhot]/F");
  m_tree->Branch("ntrkiso1Phot",&ntrkiso1Phot,"ntrkiso1Phot[nPhot]/I");
//   m_tree->Branch("ptisoatecal02Phot",&ptisoatecal02Phot,"ptisoatecal02Phot[nPhot]/F");
//   m_tree->Branch("ntrkisoatecal02Phot",&ntrkisoatecal02Phot,"ntrkisoatecal02Phot[nPhot]/I");
//   m_tree->Branch("ptisoatecal025Phot",&ptisoatecal02Phot,"ptisoatecal025Phot[nPhot]/F");
//   m_tree->Branch("ntrkisoatecal025Phot",&ntrkisoatecal02Phot,"ntrkisoatecal025Phot[nPhot]/I");
//   m_tree->Branch("ptisoatecal03Phot",&ptisoatecal02Phot,"ptisoatecal03Phot[nPhot]/F");
//   m_tree->Branch("ntrkisoatecal03Phot",&ntrkisoatecal03Phot,"ntrkisoatecal03Phot[nPhot]/I");
//   m_tree->Branch("ptisoatecal035Phot",&ptisoatecal035Phot,"ptisoatecal035Phot[nPhot]/F");
//   m_tree->Branch("ntrkisoatecal035Phot",&ntrkisoatecal035Phot,"ntrkisoatecal035Phot[nPhot]/I");
//   m_tree->Branch("ptisoatecal04Phot",&ptisoatecal04Phot,"ptisoatecal04Phot[nPhot]/F");
  m_tree->Branch("hcalovecal01Phot",&hcalovecal01Phot,"hcalovecal01Phot[nPhot]/F");
  m_tree->Branch("hcalovecal015Phot",&hcalovecal015Phot,"hcalovecal015Phot[nPhot]/F");
  m_tree->Branch("hcalovecal04Phot",&hcalovecal04Phot,"hcalovecal04Phot[nPhot]/F"); 
  m_tree->Branch("hcalovecal05Phot",&hcalovecal05Phot,"hcalovecal05Phot[nPhot]/F"); 
  m_tree->Branch("hcalovecal07Phot",&hcalovecal07Phot,"hcalovecal07Phot[nPhot]/F");
  m_tree->Branch("hcalovecal1Phot",&hcalovecal1Phot,"hcalovecal1Phot[nPhot]/F"); 
  m_tree->Branch("ecaliso01Phot",&ecaliso01Phot,"ecaliso01Phot[nPhot]/F"); 
  m_tree->Branch("ecaliso015Phot",&ecaliso015Phot,"ecaliso015Phot[nPhot]/F");  
  m_tree->Branch("ecaliso04Phot",&ecaliso04Phot,"ecaliso04Phot[nPhot]/F");  
  m_tree->Branch("ecaliso05Phot",&ecaliso05Phot,"ecaliso05Phot[nPhot]/F");  
  m_tree->Branch("ecaliso07Phot",&ecaliso07Phot,"ecaliso07Phot[nPhot]/F"); 
  m_tree->Branch("ecaliso1Phot",&ecaliso1Phot,"ecaliso1Phot[nPhot]/F");  
  m_tree->Branch("ecaliso04oldPhot",&ecaliso04oldPhot,"ecaliso04oldPhot[nPhot]/F");  
  m_tree->Branch("LATPhot",&LATPhot,"LATPhot[nPhot]/F");
  m_tree->Branch("sMajMajPhot",&sMajMajPhot,"sMajMaj2Phot[nPhot]/F");
  m_tree->Branch("sMinMinPhot",&sMinMinPhot,"sMinMin2Phot[nPhot]/F");
  m_tree->Branch("alphaPhot",&alphaPhot,"alphaPhot[nPhot]/F");
  m_tree->Branch("sEtaEtaPhot",&sEtaEtaPhot,"sEtaEtaPhot[nPhot]/F");
  m_tree->Branch("sPhiPhiPhot",&sPhiPhiPhot,"sPhiPhiPhot[nPhot]/F");
  m_tree->Branch("E1Phot",&E1Phot,"E1Phot[nPhot]/F");
  m_tree->Branch("E9Phot",&E9Phot,"E9Phot[nPhot]/F");
  m_tree->Branch("E25Phot",&E25Phot,"E25Phot[nPhot]/F");
  m_tree->Branch("FisherPhot",&FisherPhot,"FisherPhot[nPhot]/F");
  m_tree->Branch("sMajMajoldPhot",&sMajMajoldPhot,"sMajMaj2oldPhot[nPhot]/F");
  m_tree->Branch("sMinMinoldPhot",&sMinMinoldPhot,"sMinMin2oldPhot[nPhot]/F");

  m_tree->Branch("nJet_ite",&nJet_ite,"nJet_ite/I");
  m_tree->Branch("pxJet_ite ",&pxJet_ite ,"pxJet_ite[nJet_ite]/F");
  m_tree->Branch("pyJet_ite ",&pyJet_ite ,"pyJet_ite[nJet_ite]/F");
  m_tree->Branch("pzJet_ite ",&pzJet_ite ,"pzJet_ite[nJet_ite]/F");
  m_tree->Branch("eJet_ite  ",&eJet_ite  ,"eJet_ite[nJet_ite]/F");
  m_tree->Branch("etaJet_ite",&etaJet_ite,"etaJet_ite[nJet_ite]/F");
  m_tree->Branch("phiJet_ite",&phiJet_ite,"phiJet_ite[nJet_ite]/F");

  m_tree->Branch("nJet_kt4",&nJet_kt4,"nJet_kt4/I");
  m_tree->Branch("pxJet_kt4 ",&pxJet_kt4 ,"pxJet_kt4[nJet_kt4]/F");
  m_tree->Branch("pyJet_kt4 ",&pyJet_kt4 ,"pyJet_kt4[nJet_kt4]/F");
  m_tree->Branch("pzJet_kt4 ",&pzJet_kt4 ,"pzJet_kt4[nJet_kt4]/F");
  m_tree->Branch("eJet_kt4  ",&eJet_kt4  ,"eJet_kt4[nJet_kt4]/F");
  m_tree->Branch("etaJet_kt4",&etaJet_kt4,"etaJet_kt4[nJet_kt4]/F");
  m_tree->Branch("phiJet_kt4",&phiJet_kt4,"phiJet_kt4[nJet_kt4]/F");

  m_tree->Branch("nJet_kt6",&nJet_kt6,"nJet_kt6/I");
  m_tree->Branch("pxJet_kt6 ",&pxJet_kt6 ,"pxJet_kt6[nJet_kt6]/F");
  m_tree->Branch("pyJet_kt6 ",&pyJet_kt6 ,"pyJet_kt6[nJet_kt6]/F");
  m_tree->Branch("pzJet_kt6 ",&pzJet_kt6 ,"pzJet_kt6[nJet_kt6]/F");
  m_tree->Branch("eJet_kt6  ",&eJet_kt6  ,"eJet_kt6[nJet_kt6]/F");
  m_tree->Branch("etaJet_kt6",&etaJet_kt6,"etaJet_kt6[nJet_kt6]/F");
  m_tree->Branch("phiJet_kt6",&phiJet_kt6,"phiJet_kt6[nJet_kt6]/F");

  m_tree->Branch("nJet_sis5",&nJet_sis5,"nJet_sis5/I");
  m_tree->Branch("pxJet_sis5 ",&pxJet_sis5 ,"pxJet_sis5[nJet_sis5]/F");
  m_tree->Branch("pyJet_sis5 ",&pyJet_sis5 ,"pyJet_sis5[nJet_sis5]/F");
  m_tree->Branch("pzJet_sis5 ",&pzJet_sis5 ,"pzJet_sis5[nJet_sis5]/F");
  m_tree->Branch("eJet_sis5  ",&eJet_sis5  ,"eJet_sis5[nJet_sis5]/F");
  m_tree->Branch("etaJet_sis5",&etaJet_sis5,"etaJet_sis5[nJet_sis5]/F");
  m_tree->Branch("phiJet_sis5",&phiJet_sis5,"phiJet_sis5[nJet_sis5]/F");

  m_tree->Branch("nJet_sis7",&nJet_sis7,"nJet_sis7/I");
  m_tree->Branch("pxJet_sis7 ",&pxJet_sis7 ,"pxJet_sis7[nJet_sis7]/F");
  m_tree->Branch("pyJet_sis7 ",&pyJet_sis7 ,"pyJet_sis7[nJet_sis7]/F");
  m_tree->Branch("pzJet_sis7 ",&pzJet_sis7 ,"pzJet_sis7[nJet_sis7]/F");
  m_tree->Branch("eJet_sis7  ",&eJet_sis7  ,"eJet_sis7[nJet_sis7]/F");
  m_tree->Branch("etaJet_sis7",&etaJet_sis7,"etaJet_sis7[nJet_sis7]/F");
  m_tree->Branch("phiJet_sis7",&phiJet_sis7,"phiJet_sis7[nJet_sis7]/F");

  m_tree->Branch("nJet_pfite",&nJet_pfite,"nJet_pfite/I");
  m_tree->Branch("pxJet_pfite ",&pxJet_pfite ,"pxJet_pfite[nJet_pfite]/F");
  m_tree->Branch("pyJet_pfite ",&pyJet_pfite ,"pyJet_pfite[nJet_pfite]/F");
  m_tree->Branch("pzJet_pfite ",&pzJet_pfite ,"pzJet_pfite[nJet_pfite]/F");
  m_tree->Branch("eJet_pfite  ",&eJet_pfite  ,"eJet_pfite[nJet_pfite]/F");
  m_tree->Branch("etaJet_pfite",&etaJet_pfite,"etaJet_pfite[nJet_pfite]/F");
  m_tree->Branch("phiJet_pfite",&phiJet_pfite,"phiJet_pfite[nJet_pfite]/F");
  
  m_tree->Branch("nJetGen_ite",&nJetGen_ite,"nJetGen_ite/I");
  m_tree->Branch("pxJetGen_ite ",&pxJetGen_ite ,"pxJetGen_ite[nJetGen_ite]/F");
  m_tree->Branch("pyJetGen_ite ",&pyJetGen_ite ,"pyJetGen_ite[nJetGen_ite]/F");
  m_tree->Branch("pzJetGen_ite ",&pzJetGen_ite ,"pzJetGen_ite[nJetGen_ite]/F");
  m_tree->Branch("eJetGen_ite  ",&eJetGen_ite  ,"eJetGen_ite[nJetGen_ite]/F");
  m_tree->Branch("etaJetGen_ite",&etaJetGen_ite,"etaJetGen_ite[nJetGen_ite]/F");
  m_tree->Branch("phiJetGen_ite",&phiJetGen_ite,"phiJetGen_ite[nJetGen_ite]/F");

  m_tree->Branch("nJetGen_kt4",&nJetGen_kt4,"nJetGen_kt4/I");
  m_tree->Branch("pxJetGen_kt4 ",&pxJetGen_kt4 ,"pxJetGen_kt4[nJetGen_kt4]/F");
  m_tree->Branch("pyJetGen_kt4 ",&pyJetGen_kt4 ,"pyJetGen_kt4[nJetGen_kt4]/F");
  m_tree->Branch("pzJetGen_kt4 ",&pzJetGen_kt4 ,"pzJetGen_kt4[nJetGen_kt4]/F");
  m_tree->Branch("eJetGen_kt4  ",&eJetGen_kt4  ,"eJetGen_kt4[nJetGen_kt4]/F");
  m_tree->Branch("etaJetGen_kt4",&etaJetGen_kt4,"etaJetGen_kt4[nJetGen_kt4]/F");
  m_tree->Branch("phiJetGen_kt4",&phiJetGen_kt4,"phiJetGen_kt4[nJetGen_kt4]/F");

  m_tree->Branch("nJetGen_kt6",&nJetGen_kt6,"nJetGen_kt6/I");
  m_tree->Branch("pxJetGen_kt6 ",&pxJetGen_kt6 ,"pxJetGen_kt6[nJetGen_kt6]/F");
  m_tree->Branch("pyJetGen_kt6 ",&pyJetGen_kt6 ,"pyJetGen_kt6[nJetGen_kt6]/F");
  m_tree->Branch("pzJetGen_kt6 ",&pzJetGen_kt6 ,"pzJetGen_kt6[nJetGen_kt6]/F");
  m_tree->Branch("eJetGen_kt6  ",&eJetGen_kt6  ,"eJetGen_kt6[nJetGen_kt6]/F");
  m_tree->Branch("etaJetGen_kt6",&etaJetGen_kt6,"etaJetGen_kt6[nJetGen_kt6]/F");
  m_tree->Branch("phiJetGen_kt6",&phiJetGen_kt6,"phiJetGen_kt6[nJetGen_kt6]/F");

  m_tree->Branch("nJetGen_sis5",&nJetGen_sis5,"nJetGen_sis5/I");
  m_tree->Branch("pxJetGen_sis5 ",&pxJetGen_sis5 ,"pxJetGen_sis5[nJetGen_sis5]/F");
  m_tree->Branch("pyJetGen_sis5 ",&pyJetGen_sis5 ,"pyJetGen_sis5[nJetGen_sis5]/F");
  m_tree->Branch("pzJetGen_sis5 ",&pzJetGen_sis5 ,"pzJetGen_sis5[nJetGen_sis5]/F");
  m_tree->Branch("eJetGen_sis5  ",&eJetGen_sis5  ,"eJetGen_sis5[nJetGen_sis5]/F");
  m_tree->Branch("etaJetGen_sis5",&etaJetGen_sis5,"etaJetGen_sis5[nJetGen_sis5]/F");
  m_tree->Branch("phiJetGen_sis5",&phiJetGen_sis5,"phiJetGen_sis5[nJetGen_sis5]/F");

  m_tree->Branch("nJetGen_sis7",&nJetGen_sis7,"nJetGen_sis7/I");
  m_tree->Branch("pxJetGen_sis7 ",&pxJetGen_sis7 ,"pxJetGen_sis7[nJetGen_sis7]/F");
  m_tree->Branch("pyJetGen_sis7 ",&pyJetGen_sis7 ,"pyJetGen_sis7[nJetGen_sis7]/F");
  m_tree->Branch("pzJetGen_sis7 ",&pzJetGen_sis7 ,"pzJetGen_sis7[nJetGen_sis7]/F");
  m_tree->Branch("eJetGen_sis7  ",&eJetGen_sis7  ,"eJetGen_sis7[nJetGen_sis7]/F");
  m_tree->Branch("etaJetGen_sis7",&etaJetGen_sis7,"etaJetGen_sis7[nJetGen_sis7]/F");
  m_tree->Branch("phiJetGen_sis7",&phiJetGen_sis7,"phiJetGen_sis7[nJetGen_sis7]/F");

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

  m_tree->Branch("hltCount",&hltCount,"hltCount/I");
  m_tree->Branch("hltNamesLen",&hltNamesLen,"hltNamesLen/I");
  m_tree->Branch("HLTNames",&aHLTNames,"HLTNames[hltNamesLen]/C,6000");
  m_tree->Branch("HLTResults",&aHLTResults,"HLTResults[hltCount]/O");

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
