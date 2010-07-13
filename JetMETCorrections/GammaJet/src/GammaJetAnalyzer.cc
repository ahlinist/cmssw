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
// $Id: GammaJetAnalyzer.cc,v 1.35 2010/06/17 16:54:53 pandolf Exp $
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
//#include "DataFormats/VertexReco/interface/VertexCollection.h"
//#include "DataFormats/JetReco/interface/CaloJetCollection.h"
//#include "DataFormats/JetReco/interface/JPTJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/JPTJet.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaReco/interface/ClusterShape.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "DataFormats/EgammaCandidates/interface/ConversionFwd.h"

#include "JetMETCorrections/Objects/interface/JetCorrector.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h" 
#include "RecoCaloTools/Selectors/interface/CaloConeSelector.h"
#include "RecoCaloTools/MetaCollections/interface/CaloRecHitMetaCollections.h"

//#include "RecoEcal/EgammaCoreTools/interface/ClusterShapeAlgo.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"

#include "MyAnalysis/IsolationTools/interface/SuperClusterHitsEcalIsolation.h"

#include "MagneticField/Engine/interface/MagneticField.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
//#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
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
#include <FWCore/Common/interface/TriggerNames.h>
#include <DataFormats/Common/interface/TriggerResults.h>
#define MAXHLTBITS    200

#include "SimDataFormats/Track/interface/SimTrack.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"
#include "SimDataFormats/Vertex/interface/SimVertex.h"
#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

#include "DataFormats/PatCandidates/interface/Photon.h"

#include "TLorentzVector.h"

#include <set>
#include <algorithm>

using namespace reco;

// Signed version of delta_phi
inline float GammaJetAnalyzer::delta_phi(float phi1, float phi2) {

  float dphi = fabs(phi1 - phi2);
  float sgn = (phi1 >= phi2 ? +1. : -1.);
  return sgn * (dphi <= TMath::Pi() ? dphi : TMath::TwoPi() - dphi);
}

// Mirror-symmetric delta_eta
inline float GammaJetAnalyzer::delta_eta(float eta1, float eta2) {

  return (eta2 >= 0 ? eta1 - eta2 : eta2 - eta1);
}

inline double GammaJetAnalyzer::oplus(double a, double b) {
  return sqrt(a*a + b*b);
}

// in HF: emf -> R = (Eshort - Elong) / (Ehort + ELong)
// emf = Eshort / (Eshort + Elong)
inline double GammaJetAnalyzer::fixEMF(double emf, double eta) {
  return (fabs(eta)<=3.0 ? emf : 0.5*(1+emf));
}

GammaJetAnalyzer::GammaJetAnalyzer(const edm::ParameterSet& iConfig)
{
  _debug = iConfig.getParameter<bool>("debug");
  MCTruthCollection_ = iConfig.getUntrackedParameter<edm::InputTag>("MCTruthCollection");
  triggerTag_ = iConfig.getUntrackedParameter<edm::InputTag>("TriggerTag");
  trackTags_ = iConfig.getUntrackedParameter<edm::InputTag>("tracks");
  Vertexsrc_ = iConfig.getUntrackedParameter<edm::InputTag>("vertices");
  Photonsrc_ = iConfig.getUntrackedParameter<edm::InputTag>("Photonsrc");
  Jetsrcite_ = iConfig.getUntrackedParameter<edm::InputTag>("jetsite");
  Jetsrckt4_ = iConfig.getUntrackedParameter<edm::InputTag>("jetskt4");
  Jetsrckt6_ = iConfig.getUntrackedParameter<edm::InputTag>("jetskt6");
  Jetsrcakt5_ = iConfig.getUntrackedParameter<edm::InputTag>("jetsakt5");
  //Jetsrcakt7_ = iConfig.getUntrackedParameter<edm::InputTag>("jetsakt7");
  //Jetsrcsis5_ = iConfig.getUntrackedParameter<edm::InputTag>("jetssis5");
  //Jetsrcsis7_ = iConfig.getUntrackedParameter<edm::InputTag>("jetssis7");
  JetJPTsrcak5_ = iConfig.getUntrackedParameter<edm::InputTag>("jetsjptak5");
  JetPFsrcite_ = iConfig.getUntrackedParameter<edm::InputTag>("jetspfite");
  JetPFsrckt4_ = iConfig.getUntrackedParameter<edm::InputTag>("jetspfkt4");
  JetPFsrckt6_ = iConfig.getUntrackedParameter<edm::InputTag>("jetspfkt6");
  JetPFsrcakt5_ = iConfig.getUntrackedParameter<edm::InputTag>("jetspfakt5");
  JetPFsrcakt7_ = iConfig.getUntrackedParameter<edm::InputTag>("jetspfakt7");
  //JetPFsrcsis5_ = iConfig.getUntrackedParameter<edm::InputTag>("jetspfsis5");
  //JetPFsrcsis7_ = iConfig.getUntrackedParameter<edm::InputTag>("jetspfsis7");
  JetGensrcite_ = iConfig.getUntrackedParameter<edm::InputTag>("jetsgenite");
  JetGensrckt4_ = iConfig.getUntrackedParameter<edm::InputTag>("jetsgenkt4");
  JetGensrckt6_ = iConfig.getUntrackedParameter<edm::InputTag>("jetsgenkt6");
  JetGensrcakt5_ = iConfig.getUntrackedParameter<edm::InputTag>("jetsgenakt5");
  JetGensrcakt7_ = iConfig.getUntrackedParameter<edm::InputTag>("jetsgenakt7");
  //JetGensrcsis5_ = iConfig.getUntrackedParameter<edm::InputTag>("jetsgensis5");
  //JetGensrcsis7_ = iConfig.getUntrackedParameter<edm::InputTag>("jetsgensis7");
  METsrc_ = iConfig.getUntrackedParameter<edm::InputTag>("met");
  METGensrc_ = iConfig.getUntrackedParameter<edm::InputTag>("genMet");
  HBhitsrc_ = iConfig.getUntrackedParameter<edm::InputTag>("hbhits");
  recoCollection_    = iConfig.getParameter<std::string>("recoCollection");
  recoProducer_      = iConfig.getParameter<std::string>("recoProducer");
  JetCorrector_akt5_ = iConfig.getParameter<std::string>("JetCorrectionService_akt5");
  JetCorrector_pfakt5_ = iConfig.getParameter<std::string>("JetCorrectionService_pfakt5");
  JetCorrector_pfakt7_ = iConfig.getParameter<std::string>("JetCorrectionService_pfakt7");
  genjetptthr_ = iConfig.getParameter<double>("genjetptthr");
  calojetptthr_ = iConfig.getParameter<double>("calojetptthr");
  pfjetptthr_ = iConfig.getParameter<double>("pfjetptthr");
  jptjetptthr_ = iConfig.getParameter<double>("jptjetptthr");
  genjetnmin_ = iConfig.getParameter<int>("genjetnmin");
  pfjetnmin_ = iConfig.getParameter<int>("pfjetnmin");
  jptjetnmin_ = iConfig.getParameter<int>("jptjetnmin");
//   PtPhoton1st = new TH1D( "PtPhoton1st", "Pt spectrum of the most energetic photon", 50, 0.0, 400.0);
//   PtPhoton2st = new TH1D( "PtPhoton2st", "Pt spectrum of the 2' most energetic photon", 50, 0.0, 400.0);
//   PtPhoton3st = new TH1D( "PtPhoton3st", "Pt spectrum of the 3' most energetic photon", 50, 0.0, 400.0);
//   PtPhotonMC1st = new TH1D( "PtPhotonMC1st", "Pt spectrum of the most energetic photonMC", 50, 0.0, 400.0);
//   PtPhotonMC2st = new TH1D( "PtPhotonMC2st", "Pt spectrum of the 2' most energetic photonMC", 50, 0.0, 400.0);
//   PtPhotonMC3st = new TH1D( "PtPhotonMC3st", "Pt spectrum of the 3' most energetic photonMC", 50, 0.0, 400.0);
 
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
   nMC = nPhot = nconvPhot = nJet_ite = nJet_kt4 = nJet_akt5 = nJet_sis5 = nJet_kt6 = nJet_sis7 = nJet_pfite = nJet_pfkt4 = nJet_pfakt5 = nJet_pfsis5 = nJet_pfkt6 = nJet_pfsis7 = nJetGen_ite = nJetGen_kt4 = nJetGen_akt5 = nJetGen_sis5 = nJetGen_kt6 = nJetGen_sis7 = 0;
   nJet_jptak5  = 0;
   nJet_pfakt7 = nJetGen_akt7 = 0;
   nSIM = nPF = 0;

   using reco::TrackCollection;
  
   double maxptphoton1(0), maxptphoton2(0), maxptphoton3(0);

   //   // get generated pt hat
   //   Handle<double> genEventScale; 
   //   iEvent.getByLabel("genEventScale", genEventScale); 

   isMC = !iEvent.isRealData(); // separate MC processing
   store = iEvent.eventAuxiliary().storeNumber(); // study stability across a store
   lbn = iEvent.luminosityBlock(); // sum LBN lumi for normalization
   bx = iEvent.bunchCrossing(); // study effect of out-of-time pile-up
   orbit = iEvent.orbitNumber(); // study beam heating with time (longer bunches)
   run = iEvent.id().run(); // unique ID - part 1
   event = iEvent.id().event(); // unique ID - part 2


   Handle<GenEventInfoProduct> hEventInfo;
   if( isMC ) iEvent.getByLabel("generator", hEventInfo);


   // ------ MC INFORMATION:

   // get MC info from GenParticleCandidates 
   Handle<GenParticleCollection> genParticles;
   if( isMC ) iEvent.getByLabel("genParticles", genParticles);
   
   // get GEANT sim tracks and vertices (includes conversions)
   Handle<SimTrackContainer> simTracks_h;
   const SimTrackContainer* simTracks;
   if( isMC ) iEvent.getByLabel("g4SimHits", simTracks_h);
   simTracks = (simTracks_h.isValid()) ? simTracks_h.product() : 0;
   
   Handle<SimVertexContainer> simVert_h;
   const SimVertexContainer* simVertices;
   if( isMC ) iEvent.getByLabel("g4SimHits", simVert_h);
   simVertices = (simVert_h.isValid()) ? simVert_h.product() : 0;
   

   // ------ RECO INFORMATION:

   // get tracks
   Handle<TrackCollection> tracks;
   iEvent.getByLabel(trackTags_,tracks);
   
   // get primary vertices
   //Handle<VertexCollection> VertexHandle;
   Handle<vector<Vertex> > VertexHandle;
   //iEvent.getByLabel("offlinePrimaryVertices", VertexHandle);
   iEvent.getByLabel(Vertexsrc_, VertexHandle);
   
   // get photons
   Handle<PhotonCollection>  PhotonHandle;
   iEvent.getByLabel(Photonsrc_, PhotonHandle);

   // get converted photons
   Handle<ConversionCollection> convertedPhotonHandle; // get the Converted Photon info
   iEvent.getByLabel("conversions", "", convertedPhotonHandle);
   const reco::ConversionCollection convphoCollection = *(convertedPhotonHandle.product());

   // get PFCandidates
   Handle<PFCandidateCollection>  PFCandidates;
   iEvent.getByLabel("particleFlow", PFCandidates);

   // get calo jet collection
   Handle<CaloJetCollection> jetsite;
   iEvent.getByLabel(Jetsrcite_, jetsite);
   Handle<CaloJetCollection> jetsakt5;
   iEvent.getByLabel(Jetsrcakt5_, jetsakt5);
   Handle<CaloJetCollection> jetskt4;
   iEvent.getByLabel(Jetsrckt4_, jetskt4);
   Handle<CaloJetCollection> jetskt6;
   iEvent.getByLabel(Jetsrckt6_, jetskt6);
//   Handle<CaloJetCollection> jetssis5;
//   iEvent.getByLabel(Jetsrcsis5_, jetssis5);
//   Handle<CaloJetCollection> jetssis7;
//   iEvent.getByLabel(Jetsrcsis7_, jetssis7);

   // get JPT collection
   Handle<JPTJetCollection> jptjetsak5;
   iEvent.getByLabel(JetJPTsrcak5_, jptjetsak5);

   // get PF jets collection
   Handle<PFJetCollection> pfjetsite;
   iEvent.getByLabel(JetPFsrcite_, pfjetsite);   
   Handle<PFJetCollection> pfjetskt4;
   iEvent.getByLabel(JetPFsrckt4_, pfjetskt4);
   Handle<PFJetCollection> pfjetsakt5;
   iEvent.getByLabel(JetPFsrcakt5_, pfjetsakt5);
   Handle<PFJetCollection> pfjetsakt7;
   iEvent.getByLabel(JetPFsrcakt7_, pfjetsakt7);
//   Handle<PFJetCollection> pfjetssis5;
//   iEvent.getByLabel(JetPFsrcsis5_, pfjetssis5);
   Handle<PFJetCollection> pfjetskt6;
   iEvent.getByLabel(JetPFsrckt6_, pfjetskt6);
//   Handle<PFJetCollection> pfjetssis7;
//   iEvent.getByLabel(JetPFsrcsis7_, pfjetssis7);

   //get jet correctors
   const JetCorrector* corrector_akt5 = JetCorrector::getJetCorrector (JetCorrector_akt5_, iSetup);
   const JetCorrector* corrector_pfakt5 = JetCorrector::getJetCorrector (JetCorrector_pfakt5_, iSetup);
   const JetCorrector* corrector_pfakt7 = JetCorrector::getJetCorrector (JetCorrector_pfakt7_, iSetup);
 
   // get gen jet collection
   Handle<GenJetCollection> jetsgenite;
   if( isMC ) iEvent.getByLabel(JetGensrcite_, jetsgenite);
   Handle<GenJetCollection> jetsgenkt4;
   if( isMC ) iEvent.getByLabel(JetGensrckt4_, jetsgenkt4);
   Handle<GenJetCollection> jetsgenkt6;
   if( isMC ) iEvent.getByLabel(JetGensrckt6_, jetsgenkt6);
   Handle<GenJetCollection> jetsgenakt5;
   if( isMC ) iEvent.getByLabel(JetGensrcakt5_, jetsgenakt5);
   Handle<GenJetCollection> jetsgenakt7;
   if( isMC ) iEvent.getByLabel(JetGensrcakt7_, jetsgenakt7);
//   Handle<GenJetCollection> jetsgensis5;
//   if( isMC ) iEvent.getByLabel(JetGensrcsis5_, jetsgensis5);
//   Handle<GenJetCollection> jetsgensis7;
//   if( isMC ) iEvent.getByLabel(JetGensrcsis7_, jetsgensis7);

   // get caloMET
   Handle<CaloMETCollection> calomethandle;
   iEvent.getByLabel(METsrc_, calomethandle);

   // get tcMET
   Handle< View<MET> > tcmethandle; 
   iEvent.getByLabel("tcMet", tcmethandle);

   // get pfMET
   Handle<PFMETCollection> pfmethandle;
   iEvent.getByLabel("pfMet", pfmethandle);

   // get gen MET
   Handle<GenMETCollection> genmethandle;
   if( isMC ) iEvent.getByLabel(METGensrc_, genmethandle);

   Handle<GenMETCollection> genmethandle2;
   if( isMC ) iEvent.getByLabel("genMetCalo", genmethandle2);
  
   // get HCAL info
   Handle<HBHERecHitCollection> hbhe;
   iEvent.getByLabel(HBhitsrc_, hbhe);
   const HBHERecHitMetaCollection mhbhe(*hbhe);
 
   // get ECAL reco hits
   Handle<EBRecHitCollection> ecalhits;
   const EBRecHitCollection* rhits=0;
   iEvent.getByLabel(recoProducer_, recoCollection_, ecalhits);
   const EcalRecHitMetaCollection mecalhits(*ecalhits);    
   rhits = ecalhits.product(); // get a ptr to the product

   Handle<EERecHitCollection> ecalhitsee;
   const EERecHitCollection* rhitsee=0;
   iEvent.getByLabel(recoProducer_, "EcalRecHitsEE", ecalhitsee);
   rhitsee = ecalhits.product(); // get a ptr to the product

   // get geometry
   edm::ESHandle<CaloGeometry> geoHandle;
   //   iSetup.get<IdealGeometryRecord>().get(geoHandle);
   iSetup.get<CaloGeometryRecord>().get(geoHandle);
   const CaloGeometry* geometry = geoHandle.product();
   const CaloSubdetectorGeometry* geometry_p = geoHandle->getSubdetectorGeometry(DetId::Ecal, EcalBarrel);

   // get topology
// const CaloSubdetectorTopology *topology_p;
// edm::ESHandle<CaloTopology> topoHandle;
// iSetup.get<CaloTopologyRecord>().get(topoHandle);
// topology_p = topoHandle->getSubdetectorTopology(DetId::Ecal, EcalBarrel);

   edm::ESHandle<CaloTopology> pTopology;
   iSetup.get<CaloTopologyRecord>().get(pTopology);
   const CaloTopology *topology = pTopology.product();

   //   edm::ESHandle<TrackerGeometry> trackerHandle_;
   //edm::ESHandle<MagneticField> theMagField;
   //iSetup.get<IdealMagneticFieldRecord>().get(theMagField);
   //   iSetup.get<TrackerDigiGeometryRecord>().get(trackerHandle_);
  
   //ClusterShapeAlgo algo;

//---------------------------HLT Trigger ---------------------------------------------------------------------------------------------
// You Can See HLT Name list ->  " JetMETCorrections/GammaJet/test/HLTList.txt " file 

   for (unsigned int iHLT = 0; iHLT != MAXHLTBITS; ++iHLT) 
   {
   	aHLTResults[iHLT] = false;
   }
   hltPass = false;

   strcpy(aHLTNames,"");
   hltNamesLen = 0;

   edm::Handle<edm::TriggerResults> hltTriggerResultHandle;
   iEvent.getByLabel(triggerTag_, hltTriggerResultHandle);

   if (!hltTriggerResultHandle.isValid()) 
   {
   	std::cout << "invalid handle for HLT TriggerResults" << std::endl;
   } 
   else 
   {
   	edm::TriggerNames HLTNames;
//   	HLTNames.init(*hltTriggerResultHandle);
      HLTNames = iEvent.triggerNames(*hltTriggerResultHandle);

   	std::string tempnames;  
   	hltCount = hltTriggerResultHandle->size();
   	//std::cout << "hltTriggerResult->size(): " << hltCount << std::endl;

   	for (int i = 0 ; i != hltCount; ++i) {

	  tempnames += HLTNames.triggerName(i) + ":";
	  //aHLTResults[i] = hltTriggerResultHandle->accept(i);
	  //cout << i <<"....." << HLTNames.triggerName(i).c_str() << ".... : " << hltTriggerResultHandle->accept(i) << endl;

	  map<string, int>::const_iterator it 
	    = hltTriggers.find(HLTNames.triggerName(i));
	  if (it != hltTriggers.end()) {
	    int itrig = it->second;
	    aHLTResults[itrig] = hltTriggerResultHandle->accept(i);
	    hltPass |= aHLTResults[itrig];
	  }
   	} // for i

   	hltNamesLen = tempnames.length();
   	strcpy(aHLTNames,tempnames.c_str());

   } // HLT isValid
//--------------------------------------------------------------------------------------------------------------------------------------

   // Loop over MC truth

   genpt = 0.;

   if( isMC ) {
     
     //   genpt = *genEventScale;   
     if (hEventInfo->binningValues().size() > 0)
       genpt = hEventInfo->binningValues()[0];

     // Figure out the true vertex from the partons:
     // Each event has exactly eight partons (status==3);
     // 0-1: incoming protons (pdgId==2212),
     // 2-3: ISR partons (|pdgId|<=21, udsg only, no c/b),
     // 4-5: incoming partons (|pdgId|<=21, udscg, no b?),
     // 6-7: outgoing partons (|pdgId|<=22, udscg, photons)
     // Every parton 2-7 has the same production vertex (0 for 0-1)
     //assert(genParticles->at(2).status()==3);
     //assert(fabs(genParticles->at(2).pdgId())<100); //<25-><100 to include Z,W
     vxMC = genParticles->at(2).vx();
     vyMC = genParticles->at(2).vy();
     vzMC = genParticles->at(2).vz();

     // Momentum conservation in px (py, pt?): 
     // outgoing + ISR - incoming ~ 0 (couple of GeV)
     // Is the small discrepancy due to the virtual mass of the partons?
     set<int> mothers;
     map<const GenParticle*, int> mapMC;

     for (GenParticleCollection::const_iterator p = genParticles->begin();
          p != genParticles->end(); ++p) {
       
       if (nMC>=(nMaxMC-1)) {continue;}  // to reduce the root file size
       // need space for the mom, eventually

       // Find the stable photon
       if (p->pdgId()==22 && p->status()==1){
         
         double etaTrue = p->eta();
         double etTrue  = p->energy()/cosh(etaTrue);  
         
         if (etTrue>maxptphoton1) {
    	 maxptphoton1 = etTrue;
         }
         if (etTrue>maxptphoton2 && etTrue<maxptphoton1) maxptphoton2 = etTrue;
         if (etTrue>maxptphoton3 && etTrue<maxptphoton2) maxptphoton3 = etTrue;
       } // stable photon
       

       //int motherIDMC = -1;
       if (p->numberOfMothers() > 0) { 
         const Candidate * mom = p->mother();
         for (size_t j = 0; j != genParticles->size(); ++j) {
         const Candidate * ref = &((*genParticles)[j]);
         //if (mom->px() == ref->px() && mom->py() == ref->py()
         //&& mom->pz() == ref->pz() && mom->status() == ref->status()
         //&& mom->pdgId()==ref->pdgId()) {

           //assert(mom==ref); // address of the candidate is the same?
           //above works in about 99.7% of events

         if (mom==ref) {
           motherIDMC[nMC] = j;
           //motherIDMC = j;
         }
         }
       }

       // Select only a subset of particles to reduce size:
       // All the partons (8)
       // All the stable (status==1) particles within deltaR<1.0
       // around outgoing partons
       // Parents (status==2) of the stable photons and electrons in above
     
       double deltaR = 0;
       if (p->status() == 1) {
         double deltaR1 = oplus(delta_eta(p->eta(),etaMC[6]),
                          delta_phi(p->phi(),phiMC[6]));
         double deltaR2 = oplus(delta_eta(p->eta(),etaMC[7]),
                          delta_phi(p->phi(),phiMC[7]));
         deltaR = min(deltaR1, deltaR2);
       }

       // Neutral particles kept with >200 MeV (ECAL ZS threshold)
       // Charged particles kept with >75 MeV (tracking threshold)
       if (p->status()==3 || (p->status()==1 && deltaR<1.0 &&
                             (p->pt()>0.200 ||
                             (p->charge()!=0 && p->pt()>0.075))) ) {

         pdgIdMC[nMC] = p->pdgId();
         statusMC[nMC] = p->status();
         //massMC[nMC] = p->mass();
         ptMC[nMC] = p->pt();
         eMC[nMC] = p->energy();	 
         etaMC[nMC] = p->eta();	 
         phiMC[nMC] = p->phi();	 
         
         mapMC[&(*p)] = nMC;
         ++nMC; 

         // if stable photon/electron, find parent
         if (p->status() == 1 && motherIDMC[nMC] != -1
             //&& (pdgIdMC[nMC] == kPhoton || pdgIdMC[nMC] == kElectron)) {//bug
             && (p->pdgId() == kPhoton || p->pdgId() == kElectron)) {
             
           //const Candidate * mom = p->mother();
           const GenParticle *mom = (const GenParticle*)p->mother();
           if (mom->status() == 2
               && (mom->pdgId()<81 || mom->pdgId()>100) // remove MC internal 
               && mothers.find(motherIDMC[nMC]) == mothers.end()) {

           mothers.insert(motherIDMC[nMC]);
           
           if (nMC>=nMaxMC) {continue;}  // to reduce the root file size
           pdgIdMC[nMC] = mom->pdgId();
           statusMC[nMC] = mom->status();
           //massMC[nMC] = mom->mass();
           ptMC[nMC] = mom->pt();
           eMC[nMC] = mom->energy();
           etaMC[nMC] = mom->eta();
           phiMC[nMC] = mom->phi(); 

           mapMC[mom] = nMC;
           ++nMC; 
         }
         } // stable photon has parent
       } // keep particle

     } // loop particles

   
     //const double genjetptthr = 5.; // already implicit in GenJet reco
     //const int genjetnmin = 4;
     // Loop over gen Jets

     for (GenJetCollection::const_iterator it = jetsgenite->begin(); 
        it != jetsgenite->end(); ++it) {

       if (nJetGen_ite>=100) {cout << "number of gen jets ite is larger than 100. Skipping" << endl; continue;}
       if (nJetGen_ite < genjetnmin_ || it->pt() > genjetptthr_) {

         ptJetGen_ite[nJetGen_ite] = it->pt();
         eJetGen_ite[nJetGen_ite] = it->energy();	 
         etaJetGen_ite[nJetGen_ite] = it->eta();	 
         phiJetGen_ite[nJetGen_ite] = it->phi();	      
       
         nJetGen_ite++;
       }
     }

     for (GenJetCollection::const_iterator it = jetsgenkt4->begin(); 
        it != jetsgenkt4->end(); ++it) {

       if (nJetGen_kt4>=100) {cout << "number of gen jets kt 04 is larger than 100. Skipping" << endl; continue;}
       if (nJetGen_kt4 < genjetnmin_ || it->pt() > genjetptthr_) {

         ptJetGen_kt4[nJetGen_kt4] = it->pt();
         eJetGen_kt4[nJetGen_kt4] = it->energy();	 
         etaJetGen_kt4[nJetGen_kt4] = it->eta();	 
         phiJetGen_kt4[nJetGen_kt4] = it->phi();	      
         
         nJetGen_kt4++;
       }
     }

     for (GenJetCollection::const_iterator it = jetsgenkt6->begin(); 
        it != jetsgenkt6->end(); ++it) {

       if (nJetGen_kt6>=100) {cout << "number of gen jets kt 06 is larger than 100. Skipping" << endl; continue;}
       if (nJetGen_kt6 < genjetnmin_ || it->pt() > genjetptthr_) {

         ptJetGen_kt6[nJetGen_kt6] = it->pt();
         eJetGen_kt6[nJetGen_kt6] = it->energy();	 
         etaJetGen_kt6[nJetGen_kt6] = it->eta();	 
         phiJetGen_kt6[nJetGen_kt6] = it->phi();	      
         
         nJetGen_kt6++;
       }
     }


     //----- Figure out the particle decays in tracker volume BEGIN ------

     if (_debug)
       cout << Form("Figuring out the particle decays for event %d", event)
        << endl << flush;

     // Vertices only return trackID of their parent SimTrack
     // Figure out the mapping from trackID to SimTrack
     map<unsigned int, const SimTrack*> trackMap;
     for (SimTrackContainer::const_iterator iSim = simTracks->begin();
        iSim != simTracks->end(); ++iSim) {
       if (!iSim->noVertex()) {
         assert(trackMap.find(iSim->trackId())==trackMap.end());
         trackMap[iSim->trackId()] = &(*iSim);
       }
     }

     if (_debug)
       cout << "Found mapping TrackID->ParentSimTrack" << endl << flush;

     // Find all SimTracks that come from decays before the ECAL
     // and find their parent SimTracks
     map<const SimTrack*, const SimTrack*> promptParent; // daughter->mother
     map<const SimTrack*, set<const SimTrack*> > promptDecays; // m->ds
     map<const SimTrack*, const SimVertex*> promptVertex; // daughter->vertex

     for (SimTrackContainer::const_iterator iSim = simTracks->begin();
        iSim != simTracks->end(); ++iSim) {
    
       if (!iSim->noVertex()) {
      
         // Find the parent vertex and see if it classifies as an early decay
         // Exclude the primary vertex (noParent)
         SimVertex const& vtx = (*simVertices)[iSim->vertIndex()];
         if (!vtx.noParent() && vtx.position().Rho() < 129 &&
        fabs(vtx.position().z()) < 304) {
        
        // Find parent SimParticle that produced this vertex
        // vtx->parentIndex is NOT a vector index :( so use trackMap
        assert(trackMap.find(vtx.parentIndex())!=trackMap.end());
        const SimTrack* p = trackMap[vtx.parentIndex()];
        promptParent[&(*iSim)] = p;
        promptDecays[p].insert(&(*iSim));
        //promptVertex[p] = &vtx;
        promptVertex[&(*iSim)] = &vtx;
         } // early decay
       } // has vertex
     } // for simTracks

     if (_debug)
       cout << "Found mapping DaughterSimTracks<-(Vertex)->ParentSimTrack"
        << endl << flush;

     // Find grandparent SimTracks all the way up the chain
     map<const SimTrack*, const SimTrack*> chainParents;// g.daughter->grandma
     map<const SimTrack*, set<const SimTrack*> > chainDecays; // gm->gds

     for (map<const SimTrack*, const SimTrack*>::const_iterator iSim
         = promptParent.begin(); iSim != promptParent.end(); ++iSim) {
    
       // Check that the SimTrack has no daughters itself (=grandchild)
       if (promptDecays.find(iSim->first)==promptDecays.end()) {
         // Find the first SimTrack in the parentage chain (=grandparent)
         const SimTrack *p = iSim->second;
         while (promptParent.find(p) != promptParent.end())
         p = promptParent[p];
         chainParents[iSim->first] = p;
         chainDecays[p].insert(iSim->first);
       } // is grandchild
     } // for promptParent

     if (_debug)
       cout << "Found mapping GrandDaughterSimTracks<->GrandParentSimTrack"
          << " (i.e. identified decay chains)" << endl << flush;

     // Prune the decay chains to enrich useful information:
     // - truncate (electron) brems
     // - truncate non-primary photon conversions

     for (map<const SimTrack*, set<const SimTrack*> >::const_iterator iSim
         = chainDecays.begin(); iSim != chainDecays.end(); ++iSim) {

       // iteratively go down the chain and remove decays
       pruneKids(iSim->first, promptDecays, promptParent, promptVertex, 0);
     }

     if (_debug)
       cout << "Pruned the decay chains" << endl << flush;

     // Associate grandParents to GenParticles
     map<const GenParticle*, const SimTrack*> decayedSims;
     map<const SimTrack*, const GenParticle*> decayedGens;

     for (map<const SimTrack*, set<const SimTrack*> >::const_iterator iSim
          = chainDecays.begin(); iSim != chainDecays.end(); ++iSim) {
    
       if (iSim->first->noGenpart()) {
         if (_debug)
         cout << Form("Error: no GenPart found for %d (%1.3g)",
                    iSim->first->type(),
                    iSim->first->momentum().pt()) << endl;
         //assert(!iSim->first->noGenpart());
         continue;
       }
    
       // Make sure the decay chain wasn't already pruned out
       if (promptDecays.find(iSim->first)!=promptDecays.end() &&
         promptDecays[iSim->first].size()!=0) {
      
         // NB: genpartIndex offset by 1
         const GenParticle* iGen =
         &(*genParticles)[iSim->first->genpartIndex()-1];
         assert(iGen->pdgId()==iSim->first->type());
         decayedSims[iGen] = iSim->first;
         decayedGens[iSim->first] = iGen;
       }
     } // for chainParents 

     if (_debug)
       cout << "Found mappings GrandParentSimTracks<->GenParticles" << endl;

     // Save the particles (conversion) for the primary photon
     for (map<const GenParticle*, const SimTrack*>::const_iterator iGen
         = decayedSims.begin(); iGen != decayedSims.end(); ++iGen) {

       const GenParticle *p = iGen->first;
       if (p->pdgId()==22 && p->mother()->status()==3
         && p->mother()->pdgId()==22) {

         if (_debug)
         cout << "Decay chain for primary photon Gen id 22:" << endl;
         bool saved = printChildren(decayedSims[p], promptDecays,
                       promptVertex, 0, true);
         if (saved && mapMC.find(p)!=mapMC.end()) {
            statusMC[mapMC[p]] *= -1;
         }
       } // is primary photon
     } // for iGen 

     if (_debug)
       cout << "Found mappings for primary photon (if any)" << endl;
       
     //----- Figure out the particle decays in tracker volume END ------


     for (GenJetCollection::const_iterator it = jetsgenakt5->begin(); 
        it != jetsgenakt5->end(); ++it) {
       
       if (nJetGen_akt5>=100) {cout << "number of gen jets kt 05 is larger than 100. Skipping" << endl; continue;}
       if (nJetGen_akt5 < genjetnmin_ || it->pt() > genjetptthr_) {
         
       ptJetGen_akt5[nJetGen_akt5] = it->pt();	 
       eJetGen_akt5[nJetGen_akt5] = it->energy();	 
       etaJetGen_akt5[nJetGen_akt5] = it->eta();	 
       phiJetGen_akt5[nJetGen_akt5] = it->phi();	      
       
       // Extra variables for PFlow
       Int_t nMuonsGen = 0;
       TLorentzVector p4MuonsGen;

       Int_t nElectronsGen = 0;
       TLorentzVector p4ElectronsGen;
    
       Int_t nPhotonsGen = 0;
       TLorentzVector p4PhotonsGen;
    
       Int_t nTracksGen = 0;
       TLorentzVector p4TracksGen;
    
       Int_t nNeutralHadronsGen = 0;
       TLorentzVector p4NeutralHadronsGen;
    
       Int_t nHFHadronsGen = 0;
       TLorentzVector p4HFHadronsGen;
    
       Int_t nHFEMGen = 0;
       TLorentzVector p4HFEMGen;
    
       Int_t nNeutronsGen = 0;
       TLorentzVector p4NeutronsGen;
    
       Int_t nK0LGen = 0;
       TLorentzVector p4K0LGen;
    
       Int_t nK0SGen = 0;
       TLorentzVector p4K0SGen;
    
       Int_t nLambdasGen = 0;
       TLorentzVector p4LambdasGen;
    
       Int_t nCsiGen = 0;
       TLorentzVector p4CsiGen;
    
       Int_t nOtherNeutralHadronsGen = 0;
       TLorentzVector p4OtherNeutralHadronsGen;
    
    
       vector<const GenParticle*> jetParticles = it->getGenConstituents();
       vector<const GenParticle*> shortPtcls;

       //----- Select the particle decays in tracker volume BEGIN ------      

       const int ijet = it - jetsgenakt5->begin();
       if (_debug)
         cout << Form("Found mappings GenParticles<->GrandParentSimTracks"
                  " for jet %d of pT=%1.3f eta=%1.3g",
                  ijet, it->pt(), it->eta())
        << endl << flush;

       // Print out decay chains for this jet iteratively
       // NB: this method will also save the SIM particles when asked
       for (vector< const GenParticle* >::const_iterator iGen
             = jetParticles.begin(); iGen != jetParticles.end(); ++iGen) {
      
         if (_debug)
         cout << Form("Gen id %d (pT=%1.3g GeV, eta=%1.3g)",
        	      (*iGen)->pdgId(), (*iGen)->pt(), (*iGen)->eta());
        
         if (decayedSims.find(*iGen) != decayedSims.end()) {
        
         if (_debug)
           cout << " decay chain:" << endl;
         assert((*iGen)->pdgId()==decayedSims[*iGen]->type());
         bool saved = printChildren(decayedSims[*iGen],
        			    promptDecays, promptVertex, 0,
        			    (ijet<2 ? true : false)); // save to file?
         if (saved && mapMC.find(*iGen)!=mapMC.end()) {
           //assert(statusMC[mapMC[*iGen]]!=2);
           //assert(mapMC.find(*iGen)!=mapMC.end());
           statusMC[mapMC[*iGen]] *= -1;
         }
         }
         else
         if (_debug)
           cout << endl;
       }

       //----- Select out the particle decays in tracker volume END------

       // Sum up the different types of GenParticle energy
       for (vector< const GenParticle* >::iterator iPart = jetParticles.begin();
          iPart != jetParticles.end(); ++iPart) {
        
         Int_t partPdgId = (*iPart)->pdgId();
         // Convert particle momentum to normal TLorentzVector, wrong type :(
         math::XYZTLorentzVectorD const& p4t = (*iPart)->p4();
         TLorentzVector p4(p4t.px(), p4t.py(), p4t.pz(), p4t.energy());
        
         if (fabs(partPdgId)==13) { //muons
           nMuonsGen += 1;
           p4MuonsGen += p4;
         } else if (fabs(partPdgId)==11) { //electrons
           if( fabs((*iPart)->eta())<2.5) { //tracker acceptance
             nElectronsGen += 1;
             p4ElectronsGen += p4;
           } else if( fabs((*iPart)->eta())<3. ) { //if no track, they're just photons
             nPhotonsGen += 1;
             p4PhotonsGen += p4;
           } else { //HFEM in HF 
             nHFEMGen += 1;
             p4HFEMGen += p4;
           }
         } else if ((*iPart)->charge() != 0) { // charged hadrons
           if( fabs((*iPart)->eta())<2.5) { //tracker acceptance
             nTracksGen += 1;
             p4TracksGen += p4;
           } else { //if no track, they're just hadrons
             if( fabs((*iPart)->eta())<3. ) { //HCAL acceptance
               nNeutralHadronsGen += 1;
               p4NeutralHadronsGen += p4;
             } else { //HF
               nHFHadronsGen += 1;
               p4HFHadronsGen += p4;
             }
           } //if no track
         } else if (partPdgId==22) { //photons
           if( fabs((*iPart)->eta())<3.) { //tracker acceptance
             nPhotonsGen += 1;
             p4PhotonsGen += p4;
             //save photons and later check for conversions:
             //photon conversions switched OFF!
             //shortPtcls.push_back(*iPart); //(only if eta<3)
           } else {
             nHFEMGen += 1;
             p4HFEMGen += p4;
           }
         } else if ((fabs(partPdgId) != 12) && (fabs(partPdgId) != 14)
        	  && (fabs(partPdgId) != 16)) { // veto neutrinos
        
         if( fabs((*iPart)->eta())<3. ) {
           nNeutralHadronsGen += 1;
           p4NeutralHadronsGen += p4;
         } else {
           nHFHadronsGen += 1;
           p4HFHadronsGen += p4;
         }
         
         // Decay K0S and Lambda later and correct fractions
         if (abs(partPdgId)==310 || abs(partPdgId)==3122)
           shortPtcls.push_back(*iPart);

         //save single neutral hadron components
         if( abs(partPdgId)==2112 ) { //neutrons
           nNeutronsGen += 1;
           p4NeutronsGen += p4;
         } else if( abs(partPdgId)==130 ) { //K0L's
           nK0LGen += 1;
           p4K0LGen += p4;
         } else if( abs(partPdgId)==310 ) { //K0S's
           nK0SGen += 1;
           p4K0SGen += p4;
         } else if( abs(partPdgId)==3122 ) { //lambda's
           nLambdasGen += 1;
           p4LambdasGen += p4;
         } else if( abs(partPdgId)==3322 ) { //csi's
           nCsiGen += 1;
           p4CsiGen += p4;
         } else { //not much else
           nOtherNeutralHadronsGen += 1;
           p4OtherNeutralHadronsGen += p4;
         }

         } //if neutral hadrons
        
       } //for jetParticles
    
       // ------------------ BEGIN short decays (photons, K0S's and lambdas)

       for (vector<const GenParticle*>::const_iterator iGen = shortPtcls.begin();
          iGen != shortPtcls.end(); ++iGen) {
        
         // Check if GenParticle corresponds to a decayed SimTrack
         if (decayedSims.find(*iGen) != decayedSims.end()) {
        
         // Check that the SimTrack Decay products were stored
         const SimTrack *trk = decayedSims[*iGen];
         if (promptDecays.find(trk) != promptDecays.end()) {
        
           // Convert track momentum to normal TLorentzVector, wrong type :(
           math::XYZTLorentzVectorD const& p4t = (*iGen)->p4();
           TLorentzVector p4mom(p4t.px(), p4t.py(), p4t.pz(), p4t.energy());
           if ((*iGen)->pdgId()==22) {
             nPhotonsGen -= 1;
             p4PhotonsGen -= p4mom;
           }
           else {
             if( fabs((*iGen)->eta())<3. ) {
               nNeutralHadronsGen -= 1;
               p4NeutralHadronsGen -= p4mom;
             } else {
               nHFHadronsGen -= 1;
               p4HFHadronsGen -= p4mom;
             }
           }
        
           set<const SimTrack*> const& kids = promptDecays.find(trk)->second;
        
           for (set<const SimTrack*>::const_iterator iSim = kids.begin();
        	iSim != kids.end(); ++iSim) {
        
             // Convert track momentum to normal TLorentzVector, wrong type :(
             math::XYZTLorentzVectorD const& p4t = (*iSim)->momentum();
             TLorentzVector p4(p4t.px(), p4t.py(), p4t.pz(), p4t.energy());
          
             const SimVertex *vtx = promptVertex[*iSim];
        
             double vertR = vtx->position().Rho();
             double vertZ = vtx->position().z();
             //double trkEta = trk->momentum().eta();
             bool decayedBeforeCalo =  ((vertR < 129.) && ( vertZ < 304.));
        
             // Check if the decay happened early enough for the
             // charged track to be reconstructed
             bool decayTrackable = (vertR < 30. && fabs(p4.Eta()) < 2.5
        			    && (*iSim)->charge() != 0 && p4.Pt() > 0.075);
        
             if (decayedBeforeCalo && (*iSim)->type()==111) { //pizeros
               if( fabs(p4.Eta())<3. ) {
                 nPhotonsGen += 2;  //both
                 p4PhotonsGen += p4;
               } else {
                 nHFEMGen += 2;  //both
                 p4HFEMGen += p4;
               }
             }
             else if (decayTrackable) {
               if( fabs((*iSim)->type())==11 ) { //electrons
                 nElectronsGen += 1;
                 p4ElectronsGen += p4;
               } else {
                 nTracksGen += 1;
                 p4TracksGen += p4;
               }
             } else {
               //if( (*iGen)->pdgId()==22 ) { //photons //BEFORE: wrong??
               if( (*iSim)->type()==22 || fabs((*iSim)->type())==11 ) { //photons or electrons
                 if( fabs(p4.Eta())<3. ) {
                   nPhotonsGen += 1;
                   p4PhotonsGen += p4;
                 } else {
                   nHFEMGen += 1;
                   p4HFEMGen += p4;
                 }
               } else {
                 if( fabs(p4.Eta())<3. ) {
                   nNeutralHadronsGen += 1;
                   p4NeutralHadronsGen += p4;
                 } else {
                   nHFHadronsGen += 1;
                   p4HFHadronsGen += p4;
                 }
               } //if-else photons
             } //if-else decay trackable
           } // for iSim loop on kids
         } // has promptDecays
         } // has decayedSims
       } // for iGen

       // ------------------ END short decays
    
       const TLorentzVector *p = 0;

       nMuonsGen_akt5[nJetGen_akt5] = nMuonsGen;
       p = &p4MuonsGen;
       eMuonsGen_akt5[nJetGen_akt5] = p->E() / it->energy();
       ptMuonsGen_akt5[nJetGen_akt5] = p->Pt() / it->pt();
       phiMuonsGen_akt5[nJetGen_akt5] = (p->Pt() ?
                                    delta_phi(p->Phi(), it->phi()) : 0);
       etaMuonsGen_akt5[nJetGen_akt5] = (p->Pt() ? p->Eta() - it->eta() : 0);
    
       nElectronsGen_akt5[nJetGen_akt5] = nElectronsGen;
       p = &p4ElectronsGen;
       eElectronsGen_akt5[nJetGen_akt5] = p->E() / it->energy();
       ptElectronsGen_akt5[nJetGen_akt5] = p->Pt() / it->pt();
       phiElectronsGen_akt5[nJetGen_akt5] = (p->Pt() ?
                                    delta_phi(p->Phi(), it->phi()) : 0);
       etaElectronsGen_akt5[nJetGen_akt5] = (p->Pt() ? p->Eta() - it->eta() : 0);
    
       nPhotonsGen_akt5[nJetGen_akt5] = nPhotonsGen;
       p = &p4PhotonsGen;
       ePhotonsGen_akt5[nJetGen_akt5] = p->E() / it->energy();
       ptPhotonsGen_akt5[nJetGen_akt5] = p->Pt() / it->pt();
       phiPhotonsGen_akt5[nJetGen_akt5] = (p->Pt() ? 
                                   delta_phi(p->Phi(), it->phi()) : 0);
       etaPhotonsGen_akt5[nJetGen_akt5] = (p->Pt() ? p->Eta() - it->eta() : 0);
    
       nTracksGen_akt5[nJetGen_akt5] = nTracksGen;
       p = &p4TracksGen;
       eTracksGen_akt5[nJetGen_akt5] = p->E() / it->energy();
       ptTracksGen_akt5[nJetGen_akt5] = p->Pt() / it->pt();
       phiTracksGen_akt5[nJetGen_akt5] = (p->Pt() ?
                                  delta_phi(p->Phi(), it->phi()) : 0);
       etaTracksGen_akt5[nJetGen_akt5] = (p->Pt() ? p->Eta() - it->eta() : 0);

       nNeutralHadronsGen_akt5[nJetGen_akt5] = nNeutralHadronsGen;
       p = &p4NeutralHadronsGen;
       eNeutralHadronsGen_akt5[nJetGen_akt5] = p->E() / it->energy();
       ptNeutralHadronsGen_akt5[nJetGen_akt5] = p->Pt() / it->pt();
       phiNeutralHadronsGen_akt5[nJetGen_akt5] = (p->Pt() ?
                                     delta_phi(p->Phi(), it->phi()) : 0);
       etaNeutralHadronsGen_akt5[nJetGen_akt5] =(p->Pt() ?
                                         p->Eta() - it->eta() : 0);
       
       nHFHadronsGen_akt5[nJetGen_akt5] = nHFHadronsGen;
       p = &p4HFHadronsGen;
       eHFHadronsGen_akt5[nJetGen_akt5] = p->E() / it->energy();
       ptHFHadronsGen_akt5[nJetGen_akt5] = p->Pt() / it->pt();
       phiHFHadronsGen_akt5[nJetGen_akt5] = (p->Pt() ?
                                     delta_phi(p->Phi(), it->phi()) : 0);
       etaHFHadronsGen_akt5[nJetGen_akt5] =(p->Pt() ?
                                         p->Eta() - it->eta() : 0);
       
       nHFEMGen_akt5[nJetGen_akt5] = nHFEMGen;
       p = &p4HFEMGen;
       eHFEMGen_akt5[nJetGen_akt5] = p->E() / it->energy();
       ptHFEMGen_akt5[nJetGen_akt5] = p->Pt() / it->pt();
       phiHFEMGen_akt5[nJetGen_akt5] = (p->Pt() ?
                                     delta_phi(p->Phi(), it->phi()) : 0);
       etaHFEMGen_akt5[nJetGen_akt5] =(p->Pt() ?
                                         p->Eta() - it->eta() : 0);
       


       nNeutronsGen_akt5[nJetGen_akt5] = nNeutronsGen;
       p = &p4NeutronsGen;
       eNeutronsGen_akt5[nJetGen_akt5] = p->E() / it->energy();

       nK0LGen_akt5[nJetGen_akt5] = nK0LGen;
       p = &p4K0LGen;
       eK0LGen_akt5[nJetGen_akt5] = p->E() / it->energy();

       nK0SGen_akt5[nJetGen_akt5] = nK0SGen;
       p = &p4K0SGen;
       eK0SGen_akt5[nJetGen_akt5] = p->E() / it->energy();

       nLambdasGen_akt5[nJetGen_akt5] = nLambdasGen;
       p = &p4LambdasGen;
       eLambdasGen_akt5[nJetGen_akt5] = p->E() / it->energy();

       nCsiGen_akt5[nJetGen_akt5] = nCsiGen;
       p = &p4CsiGen;
       eCsiGen_akt5[nJetGen_akt5] = p->E() / it->energy();

       nOtherNeutralHadronsGen_akt5[nJetGen_akt5] = nOtherNeutralHadronsGen;
       p = &p4OtherNeutralHadronsGen;
       eOtherNeutralHadronsGen_akt5[nJetGen_akt5] = p->E() / it->energy();

       nJetGen_akt5++;
     } // if >genjetptthr
     } // gen_akt5

     for (GenJetCollection::const_iterator it = jetsgenakt7->begin(); 
       it != jetsgenakt7->end(); ++it) {
     
       if (nJetGen_akt7>=100) {cout << "number of gen jets antikt 07 is larger than 100. Skipping" << endl; continue;}
       ptJetGen_akt7[nJetGen_akt7] = it->pt();	 
       eJetGen_akt7[nJetGen_akt7] = it->energy();	 
       etaJetGen_akt7[nJetGen_akt7] = it->eta();	 
       phiJetGen_akt7[nJetGen_akt7] = it->phi();	      
       
       nJetGen_akt7++;
     }

//   for (GenJetCollection::const_iterator it = jetsgensis5->begin(); 
//      it != jetsgensis5->end(); ++it) {

//     if (nJetGen_sis5>=100) {cout << "number of gen jets sis 05 is larger than 100. Skipping" << endl; continue;}
//     if (nJetGen_sis5 < genjetnmin_ || it->pt() > genjetptthr_) {

//       ptJetGen_sis5[nJetGen_sis5] = it->pt();
//       eJetGen_sis5[nJetGen_sis5] = it->energy();	 
//       etaJetGen_sis5[nJetGen_sis5] = it->eta();	 
//       phiJetGen_sis5[nJetGen_sis5] = it->phi();	      
//       
//       nJetGen_sis5++;
//     }
//   }

//   for (GenJetCollection::const_iterator it = jetsgensis7->begin(); 
//      it != jetsgensis7->end(); ++it) {

//     if (nJetGen_sis7>=100) {cout << "number of gen jets sis 07 is larger than 100. Skipping" << endl; continue;}
//     if (nJetGen_sis7 < genjetnmin_ || it->pt() > genjetptthr_) {

//       ptJetGen_sis7[nJetGen_sis7] = it->pt();
//       eJetGen_sis7[nJetGen_sis7] = it->energy();	 
//       etaJetGen_sis7[nJetGen_sis7] = it->eta();	 
//       phiJetGen_sis7[nJetGen_sis7] = it->phi();	      
//       
//       nJetGen_sis7++;
//     }
//   }

  } //if(isMC)



//    if (maxptphoton1>-900.) PtPhotonMC1st->Fill(maxptphoton1);
//    if (maxptphoton2>-900.) PtPhotonMC2st->Fill(maxptphoton2);
//    if (maxptphoton3>-900.) PtPhotonMC3st->Fill(maxptphoton3);

   maxptphoton1 = 0; maxptphoton2 = 0; maxptphoton3 = 0;

   // Get the primary vertex coordinates
   nvertex = VertexHandle->size();

   vx = (VertexHandle->begin()->isValid()) ? VertexHandle->begin()->x() : 999.;
   vy = (VertexHandle->begin()->isValid()) ? VertexHandle->begin()->y() : 999.;
   vz = (VertexHandle->begin()->isValid()) ? VertexHandle->begin()->z() : 999.;

   vntracks = (VertexHandle->begin()->isValid()) ? VertexHandle->begin()->tracksSize() : 0;
   vchi2 = (VertexHandle->begin()->isValid()) ? VertexHandle->begin()->normalizedChi2() : 100.;
   vndof = (VertexHandle->begin()->isValid()) ? VertexHandle->begin()->ndof() : 0.;



   // Loop over reco photons

   for (PhotonCollection::const_iterator it = PhotonHandle->begin(); 
	 it != PhotonHandle->end(); ++it) {
     
     if (it->energy()<3.) continue;
     if (nPhot>=40) {cout << "number of photons is larger than 40. Skipping" << endl; continue;}
     
     ptPhot[nPhot] = it->pt();
     ePhot[nPhot] = it->energy();	 
     escPhot[nPhot] = it->superCluster()->energy();	 
     eseedPhot[nPhot] = it->superCluster()->seed()->energy();	 
     etaPhot[nPhot] = it->eta();	 
     phiPhot[nPhot] = it->phi();	      
     
     double ptphoton = it->pt(); 
     if (ptphoton>maxptphoton1) {
       maxptphoton1 = ptphoton;
     }
     if (ptphoton>maxptphoton2 && ptphoton<maxptphoton1) maxptphoton2 = ptphoton;
     if (ptphoton>maxptphoton3 && ptphoton<maxptphoton2) maxptphoton3 = ptphoton;     
     
     double ptiso0015(0.), ptiso035(0.), ptiso04(0.), ptiso05(0.),
       ptiso07(0.), ptiso1(0.);
     int ntrkiso0015(0), ntrkiso035(0), ntrkiso04(0), ntrkiso05(0),
       ntrkiso07(0), ntrkiso1(0);

     // Fill default photon ID variables
     pid_jurECAL[nPhot] = it->ecalRecHitSumEtConeDR04();//isolationEcalRecHit
     pid_twrHCAL[nPhot] = it->hcalTowerSumEtConeDR04();//isolationHcalRecHit
     pid_HoverE[nPhot] = it->hadronicOverEm();
     pid_hlwTrack[nPhot] = it->trkSumPtHollowConeDR04();//isolationHollowTrkCone
     pid_etawid[nPhot] = it->sigmaIetaIeta();//sqrt(it->covEtaEta());

     // fill default ID variables from
     // https://twiki.cern.ch/twiki/bin/view/CMS/PhotonIDAnalysis for 31X
     // (followed from https://twiki.cern.ch/twiki/bin/view/CMS/PhotonID)
     // using reco equivalents to pat::Photon methods
     if (abs(etaPhot[nPhot]) < 1.479) {

       pid_isEM[nPhot] =    (pid_jurECAL[nPhot]  < 5 + 0.15*ptPhot[nPhot] &&
			     pid_twrHCAL[nPhot]  < 5 &&
			     pid_HoverE[nPhot]   < 0.5);
       pid_isLoose[nPhot] = (pid_isEM[nPhot] &&
			     pid_jurECAL[nPhot]  < 5 + 0.0045*ptPhot[nPhot] &&
			     pid_HoverE[nPhot]   < 0.15 &&
			     pid_hlwTrack[nPhot] < 9);
       pid_isTight[nPhot] = (pid_isLoose[nPhot] &&
			     pid_etawid[nPhot]   < 0.013);
     }
     else {

       pid_isEM[nPhot] =    (pid_jurECAL[nPhot]  < 5 + 0.15*ptPhot[nPhot] &&
			     pid_twrHCAL[nPhot]  < 7 &&
			     pid_HoverE[nPhot]   < 0.5);
       pid_isLoose[nPhot] = (pid_isEM[nPhot] &&
			     pid_jurECAL[nPhot]  < 5 + 0.02*ptPhot[nPhot] &&
			     pid_HoverE[nPhot]   < 0.15 &&
			     pid_hlwTrack[nPhot] < 9);
       pid_isTight[nPhot] = (pid_isLoose[nPhot] &&
			     pid_etawid[nPhot]   < 0.026); // fixed default
     }

     // Need to reconstruct PAT photon to get the decisions
     // nah, doesn't fill isolation automatically, need to run some produces
     //pat::Photon pho(*it);
     //const vector<pat::Photon::IdPair> &phoids = pho.photonIDs();
     //pid_isEM[nPhot] = false;
     //pid_isLoose[nPhot] = false;
     //pid_isTight[nPhot] = false;
     //for (unsigned int i = 0; i != phoids.size(); ++i) {
     //if (phoids[i].first=="EM") pid_isEM[nPhot] = phoids[i].second;
     //if (phoids[i].first=="Loose") pid_isLoose[nPhot] = phoids[i].second;
     //if (phoids[i].first=="Tight") pid_isTight[nPhot] = phoids[i].second;
     //}

     // PAT equivalents of photon ID variables
     //if (_debug) {
     //if (!(fabs(pid_jurECAL[nPhot]-pho.ecalIso())<1e-3)) {
     // cerr << Form("jurECAL %1.4g, ecaliso %1.4g",
     //	      pid_jurECAL[nPhot],pho.ecalIso()) << endl << flush;
     // //assert(fabs(pid_jurECAL[nPhot]-pho.ecalIso())<1e-3);
     //}
     //if (!(fabs(pid_twrHCAL[nPhot]-pho.hcalIso())<1e-3)) {
     // cerr << Form("twrHCAL %1.4g, hcaliso %1.4g",
     //	      pid_twrHCAL[nPhot],pho.hcalIso()) << endl << flush;
     // //assert(fabs(pid_twrHCAL[nPhot]-pho.hcalIso())<1e-3);
     //}
     //if (!(fabs(pid_hlwTrack[nPhot]-pho.trackIso())<1e-3)) {
     // cerr << Form("hlwTrack %1.4g, trackiso %1.4g",
     //	      pid_hlwTrack[nPhot],pho.trackIso()) << endl << flush;
     // //assert(fabs(pid_hlwTrack[nPhot]-pho.trackIso())<1e-3);
     //}
     //}

     // calculate track isolation for different cone values

     for (TrackCollection::const_iterator itTrack = tracks->begin();
	 itTrack != tracks->end(); ++itTrack) {

       double etaTrack = itTrack->innerMomentum().eta();
       double phiTrack = itTrack->innerMomentum().phi();
       
       double deltaPhi = phiTrack-it->phi();
       double deltaEta = etaTrack-it->eta();
       if (deltaPhi > Geom::pi()) deltaPhi -= 2.*Geom::pi();
       if (deltaPhi < -Geom::pi()) deltaPhi += 2.*Geom::pi();
       double deltaR = std::sqrt(deltaEta*deltaEta+deltaPhi*deltaPhi);
       
       if (deltaR < .015)  {ptiso0015  += sqrt(itTrack->innerMomentum().Mag2()); ntrkiso0015++; }
       if (deltaR < .35)   {ptiso035 += sqrt(itTrack->innerMomentum().Mag2()); ntrkiso035++;}
       if (deltaR < .4)   {ptiso04 += sqrt(itTrack->innerMomentum().Mag2()); ntrkiso04++;}
       if (deltaR < .5)    {ptiso05  += sqrt(itTrack->innerMomentum().Mag2()); ntrkiso05++; }
       if (deltaR < .7)    {ptiso07 += sqrt(itTrack->innerMomentum().Mag2()); ntrkiso07++;}
       if (deltaR < 1.)    {ptiso1  += sqrt(itTrack->innerMomentum().Mag2()); ntrkiso1++; }
       
     }
     
     ptiso0015Phot[nPhot] = ptiso0015;
     ntrkiso0015Phot[nPhot] = ntrkiso0015;
     ptiso035Phot[nPhot] = ptiso035;
     ntrkiso035Phot[nPhot] = ntrkiso035;
     ptiso04Phot[nPhot] = ptiso04;
     ntrkiso04Phot[nPhot] = ntrkiso04;
     ptiso05Phot[nPhot] = ptiso05;
     ntrkiso05Phot[nPhot] = ntrkiso05;
     ptiso07Phot[nPhot] = ptiso07;
     ntrkiso07Phot[nPhot] = ntrkiso07;
     ptiso1Phot[nPhot] = ptiso1;
     ntrkiso1Phot[nPhot] = ntrkiso1;

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
 

     CaloClusterPtr SCseed = it->superCluster()->seed();

     // calculate ECAL isolation 

     // ecal isolation with SC seed rechits removal
     SuperClusterHitsEcalIsolation scBasedIsolation(rhits,rhitsee);
     scBasedIsolation.setExtRadius(0.1);
     scBasedIsolation.excludeHalo(false);
     ecaliso01Phot[nPhot]  = scBasedIsolation.getSum(iEvent,iSetup,&(*SCseed));
     scBasedIsolation.setExtRadius(0.15);
     ecaliso015Phot[nPhot]  = scBasedIsolation.getSum(iEvent,iSetup,&(*SCseed));
     scBasedIsolation.setExtRadius(0.4);
     ecaliso04Phot[nPhot]  = scBasedIsolation.getSum(iEvent,iSetup,&(*SCseed));
     scBasedIsolation.setExtRadius(0.5);
     ecaliso05Phot[nPhot]  = scBasedIsolation.getSum(iEvent,iSetup,&(*SCseed));
     scBasedIsolation.setExtRadius(0.7);
     ecaliso07Phot[nPhot]  = scBasedIsolation.getSum(iEvent,iSetup,&(*SCseed));
     scBasedIsolation.setExtRadius(1.);
     ecaliso1Phot[nPhot]  = scBasedIsolation.getSum(iEvent,iSetup,&(*SCseed));

     // cluster shape variables
     
     if (TMath::Abs(SCseed->eta())<1.47){
       //reco::ClusterShape tempShape=algo.Calculate(*SCseed, rhits, &(*geometry_p), &(*topology_p),4.7);
       Cluster2ndMoments moments = EcalClusterTools::cluster2ndMoments(*SCseed, *rhits);
       std::vector<float> etaphimoments = EcalClusterTools::localCovariances(*SCseed, &(*rhits), &(*topology));
       sMajMajPhot[nPhot]=moments.sMaj;
       sMinMinPhot[nPhot]=moments.sMin;
       //FisherPhot[nPhot]=tempShape.fisher();
       alphaPhot[nPhot]=moments.alpha;
       sEtaEtaPhot[nPhot]=etaphimoments[0];
       sPhiPhiPhot[nPhot]=etaphimoments[1];
       //E1Phot[nPhot]=tempShape.eMax();
       //E9Phot[nPhot]=tempShape.e3x3();
       //E25Phot[nPhot]=tempShape.e5x5();
     }else{
       sMajMajPhot[nPhot]=-100.;
       sMinMinPhot[nPhot]=-100.;
       FisherPhot[nPhot]=-100.;
       alphaPhot[nPhot]=-100.;
       sEtaEtaPhot[nPhot] = it->sigmaEtaEta();//-100.;
       sPhiPhiPhot[nPhot]=-100.;
       E1Phot[nPhot] = SCseed->energy();//-100.;
       E9Phot[nPhot] = it->e3x3();//-100.;
       E25Phot[nPhot] = it->e5x5();//-100.;
     }

     if (_debug && fabs(SCseed->eta())<1.47 &&
	 fabs(it->maxEnergyXtal()-387)>1 && // weird bug in 31X?
	 (fabs(sEtaEtaPhot[nPhot]-it->sigmaEtaEta())>1e-4 ||
	  //fabs(E1Phot[nPhot]-it->maxEnergyXtal())>1e-4 ||
	  fabs(E1Phot[nPhot]-SCseed->energy())>1e-4 ||
	  fabs(E9Phot[nPhot]-it->e3x3())>1e-4 ||
	  fabs(E25Phot[nPhot]-it->e5x5())>1e-4)) {
       cout << Form("Eta %1.3g\n"
		    "sEtaEta %1.3g vs %1.3f\n"
		    "E1      %1.3g vs %1.3g\n"
		    "E9      %1.3g vs %1.3g\n"
		    "E25     %1.3g vs %1.3g\n",
		    SCseed->eta(),
		    sEtaEtaPhot[nPhot], it->sigmaEtaEta(),
		    E1Phot[nPhot], SCseed->energy(),
		    E9Phot[nPhot], it->e3x3(),
		    E25Phot[nPhot], it->e5x5());
     }

     ++nPhot;
    
   }

   // loop on converted photons
   for (reco::ConversionCollection::const_iterator iCPho =
	  convphoCollection.begin(); iCPho != convphoCollection.end(); ++iCPho){
     if ((*iCPho).isConverted()){

     if (nconvPhot>=10) {cout << "number of conversion photons is larger than 10. Skipping" << endl; continue;}
       chi2convPhot[nconvPhot] = (*iCPho).conversionVertex().chi2();
       ndofconvPhot[nconvPhot] = (*iCPho).conversionVertex().ndof();
       // conversionVertex does not seem to work too well, often rho=phi=z=0
       rconvPhot[nconvPhot] = (*iCPho).conversionVertex().position().rho();
       phiconvPhot[nconvPhot] = (*iCPho).conversionVertex().position().phi();
       zconvPhot[nconvPhot] = (*iCPho).conversionVertex().position().z();
       ntrkconvPhot[nconvPhot] = (*iCPho).tracks().size();
       eovpconvPhot[nconvPhot] = (*iCPho).EoverP();
       etaecalconvPhot[nconvPhot] = (*iCPho).caloCluster()[0]->eta(); 
       phiecalconvPhot[nconvPhot] = (*iCPho).caloCluster()[0]->phi(); 
       energyecalconvPhot[nconvPhot] =(*iCPho).caloCluster()[0]->energy();
       // additional quality checks on conversion pairs
       d0convPhot[nconvPhot] = (*iCPho).distOfMinimumApproach();
       algoconvPhot[nconvPhot] = (*iCPho).algo();
       detaecalconvPhot[nconvPhot] = (*iCPho).dEtaTracksAtEcal();
       dphiecalconvPhot[nconvPhot] = (*iCPho).dPhiTracksAtEcal();
       dphivtxconvPhot[nconvPhot] = (*iCPho).dPhiTracksAtVtx();
       pairsepconvPhot[nconvPhot] = (*iCPho).pairCotThetaSeparation();
       pairmassconvPhot[nconvPhot] = (*iCPho).pairInvariantMass();
       // track-wise quality (one track guaranteed with isConverted()
       trchi21convPhot[nconvPhot] = (*iCPho).tracks()[0]->chi2();
       trndof1convPhot[nconvPhot] = (*iCPho).tracks()[0]->ndof();
       trqual1convPhot[nconvPhot] = (*iCPho).tracks()[0]->qualityMask();
       trpt1convPhot[nconvPhot] = (*iCPho).tracks()[0]->pt();
       trerr1convPhot[nconvPhot] = (*iCPho).tracks()[0]->ptError();
       if (ntrkconvPhot[nconvPhot] > 1) {
	 trchi22convPhot[nconvPhot] = (*iCPho).tracks()[1]->chi2();
	 trndof2convPhot[nconvPhot] = (*iCPho).tracks()[1]->ndof();
	 trqual2convPhot[nconvPhot] = (*iCPho).tracks()[1]->qualityMask();
	 trpt2convPhot[nconvPhot] = (*iCPho).tracks()[1]->pt();
	 trerr2convPhot[nconvPhot] = (*iCPho).tracks()[1]->ptError();
       }
       else {
	 trchi22convPhot[nconvPhot] = -99;
	 trndof2convPhot[nconvPhot] = -99;
	 trqual2convPhot[nconvPhot] = -99;
	 trpt2convPhot[nconvPhot] = -99;
	 trerr2convPhot[nconvPhot] = -99;
       }

       ++nconvPhot;
     }
   } 

//    if (maxptphoton1>-900.) PtPhoton1st->Fill(maxptphoton1);
//    if (maxptphoton2>-900.) PtPhoton2st->Fill(maxptphoton2);
//    if (maxptphoton3>-900.) PtPhoton3st->Fill(maxptphoton3);

   // Loop over reco Jets

   /*
   // Could replace multiple explicit lines with a loop in the future
   for (int i = 0; i != nAlgo; ++i) {
     
       for (JetCollection::const_iterator it = jets[i]->begin(); 
	     it != jets[i]->end(); ++it) {
	 
	 if (nJet[i]>=100) {
	   cout << Form("number of reco jets %s is larger than 100. Skipping",
			algo[i]->first) << endl; continue; }
	 int nj = nJet[i];
	 ptJet[i][nj] = it->pt();
	 eJet[i][nj] = it->energy();
	 etaJet[i][nj] = it->eta();
	 phiJet[i][nj] = it->phi();

	 if (algo[i]->second==kCaloAlgo)
	   emfJet[i][nj] = ((CaloJet*)it)->emEnergyFraction();
	 if (algo[i]->second==kPFAlgo) {
	   emfJet[i][nj] = 1-((PFJet*)it)->neutralHadronEnergyFraction();
	 }	 

	 ++nJet[i];
       } // it
   } // i
   */


   for (CaloJetCollection::const_iterator it = jetsite->begin(); 
	 it != jetsite->end(); ++it) {
     
     if (nJet_ite>=100) {cout << "number of reco jets ite is larger than 100. Skipping" << endl; continue;}
     if (it->pt() > calojetptthr_) {

       ptJet_ite[nJet_ite] = it->pt();
       eJet_ite[nJet_ite] = it->energy();	 
       etaJet_ite[nJet_ite] = it->eta();	 
       phiJet_ite[nJet_ite] = it->phi();	      
       //emfJet_ite[nJet_ite] = it->emEnergyFraction();
       emfJet_ite[nJet_ite] = fixEMF(it->emEnergyFraction(), it->eta());
       
       nJet_ite++;
     }
   }
   
   for (CaloJetCollection::const_iterator it = jetskt4->begin(); 
	 it != jetskt4->end(); ++it) {
     
     if (nJet_kt4>=100) {cout << "number of reco jets kt 04 is larger than 100. Skipping" << endl; continue;}
     if (it->pt() > calojetptthr_) {

       ptJet_kt4[nJet_kt4] = it->pt();
       eJet_kt4[nJet_kt4] = it->energy();	 
       etaJet_kt4[nJet_kt4] = it->eta();	 
       phiJet_kt4[nJet_kt4] = it->phi();
       //emfJet_kt4[nJet_kt4] = it->emEnergyFraction();	      
       emfJet_kt4[nJet_kt4] = fixEMF(it->emEnergyFraction(), it->eta());

       nJet_kt4++;
     }
   }
   
   for (CaloJetCollection::const_iterator it = jetskt6->begin(); 
	 it != jetskt6->end(); ++it) {
     
     if (nJet_kt6>=100) {cout << "number of reco jets kt 06 is larger than 100. Skipping" << endl; continue;}
     if (it->pt() > calojetptthr_) {
       
       ptJet_kt6[nJet_kt6] = it->pt(); 
       eJet_kt6[nJet_kt6] = it->energy();	 
       etaJet_kt6[nJet_kt6] = it->eta();	 
       phiJet_kt6[nJet_kt6] = it->phi();	      
       //emfJet_kt6[nJet_kt6] = it->emEnergyFraction();     
       emfJet_kt6[nJet_kt6] = fixEMF(it->emEnergyFraction(), it->eta());

       nJet_kt6++;
     }
   }

   for (CaloJetCollection::const_iterator it = jetsakt5->begin(); 
	 it != jetsakt5->end(); ++it) {
     
     if (nJet_akt5>=100) {cout << "number of reco jets akt 05 is larger than 100. Skipping" << endl; continue;}
     if (it->pt() > calojetptthr_) {
       
       ptJet_akt5[nJet_akt5] = it->pt();	 

       // Jet Energy Scale Corrections on-the-fly     
       CaloJet  correctedJet = *it;
       double scale = corrector_akt5->correction(it->p4());
       correctedJet.scaleEnergy(scale);
       ptCorrJet_akt5[nJet_akt5] = correctedJet.pt();

       eJet_akt5[nJet_akt5] = it->energy();	 
       etaJet_akt5[nJet_akt5] = it->eta();	 
       phiJet_akt5[nJet_akt5] = it->phi();	      
       //emfJet_akt5[nJet_akt5] = it->emEnergyFraction();
       emfJet_akt5[nJet_akt5] = fixEMF(it->emEnergyFraction(), it->eta());
       
       nJet_akt5++;
     }
   }

   //for (CaloJetCollection::const_iterator it = jetsakt7->begin(); 
   // it != jetsakt7->end(); ++it) {
     
   //if (nJet_akt7>=100) {cout << "number of reco jets akt 07 is larger than 100. Skipping" << endl; continue;}
   //ptJet_akt7[nJet_akt7] = it->pt();	 
   //eJet_akt7[nJet_akt7] = it->energy();	 
   //etaJet_akt7[nJet_akt7] = it->eta();	 
   //phiJet_akt7[nJet_akt7] = it->phi();	      
   ////emfJet_akt7[nJet_akt7] = it->emEnergyFraction();
   //emfJet_akt7[nJet_akt7] = fixEMF(it->emEnergyFraction(), it->eta());     

   //nJet_akt7++;
   //}
   
// for (CaloJetCollection::const_iterator it = jetssis5->begin(); 
//     it != jetssis5->end(); ++it) {
//   
//   if (nJet_sis5>=100) {cout << "number of reco jets sis 05 is larger than 100. Skipping" << endl; continue;}
//   if (it->pt() > calojetptthr_) {
//     
//     ptJet_sis5[nJet_sis5] = it->pt();
//     eJet_sis5[nJet_sis5] = it->energy();	 
//     etaJet_sis5[nJet_sis5] = it->eta();	 
//     phiJet_sis5[nJet_sis5] = it->phi();	      
//     //emfJet_sis5[nJet_sis5] = it->emEnergyFraction();
//     emfJet_sis5[nJet_sis5] = fixEMF(it->emEnergyFraction(), it->eta());
//     
//     nJet_sis5++;
//   }
// }

// for (CaloJetCollection::const_iterator it = jetssis7->begin(); 
//     it != jetssis7->end(); ++it) {
//   
//   if (nJet_sis7>=100) {cout << "number of reco jets sis 07 is larger than 100. Skipping" << endl; continue;}
//   if (it->pt() > calojetptthr_) {
//     
//     ptJet_sis7[nJet_sis7] = it->pt();
//     eJet_sis7[nJet_sis7] = it->energy();	 
//     etaJet_sis7[nJet_sis7] = it->eta();	 
//     phiJet_sis7[nJet_sis7] = it->phi();	      
//     //emfJet_sis7[nJet_sis7] = it->emEnergyFraction();
//     emfJet_sis7[nJet_sis7] = fixEMF(it->emEnergyFraction(), it->eta());
//     
//     nJet_sis7++;
//   }
// }
   
   for (JPTJetCollection::const_iterator it = jptjetsak5->begin(); 
       it != jptjetsak5->end(); ++it) {
     
     if (nJet_jptak5>=100) {cout << "number of reco jets jptakt 05 is larger than 100. Skipping" << endl; continue;}
     if (nJet_jptak5 < jptjetnmin_ || it->pt() > jptjetptthr_) {
       
       ptJet_jptak5[nJet_jptak5] = it->pt();	 
       eJet_jptak5[nJet_jptak5] = it->energy();	 
       etaJet_jptak5[nJet_jptak5] = it->eta();	 
       phiJet_jptak5[nJet_jptak5] = it->phi();	      
       //emfJet_jptak5[nJet_jptak5] = fixEMF(it->emEnergyFraction(), it->eta());
       
       nJet_jptak5++;
     }
   }

   for (PFJetCollection::const_iterator it = pfjetsite->begin(); 
	 it != pfjetsite->end(); ++it) {
     
     if (nJet_pfite>=100) {cout << "number of reco jets pfite is larger than 100. Skipping" << endl; continue;}
     if (nJet_pfite < pfjetnmin_ || it->pt() > pfjetptthr_) {

       ptJet_pfite[nJet_pfite] = it->pt();
       eJet_pfite[nJet_pfite] = it->energy();	 
       etaJet_pfite[nJet_pfite] = it->eta();	 
       phiJet_pfite[nJet_pfite] = it->phi();	      
     
       nJet_pfite++;
     }
   }

   for (PFJetCollection::const_iterator it = pfjetskt4->begin(); 
	 it != pfjetskt4->end(); ++it) {
     
     if (nJet_pfkt4>=100) {cout << "number of reco jets pfkt4 is larger than 100. Skipping" << endl; continue;}
     if (nJet_pfkt4 < pfjetnmin_ || it->pt() > pfjetptthr_) {

       ptJet_pfkt4[nJet_pfkt4] = it->pt();
       eJet_pfkt4[nJet_pfkt4] = it->energy();	 
       etaJet_pfkt4[nJet_pfkt4] = it->eta();	 
       phiJet_pfkt4[nJet_pfkt4] = it->phi();	      
       
       nJet_pfkt4++;
     }
   }

   for (PFJetCollection::const_iterator it = pfjetsakt5->begin(); 
	 it != pfjetsakt5->end(); ++it) {
     
     if (nJet_pfakt5>=100) {cout << "number of reco jets pfakt5 is larger than 100. Skipping" << endl; continue;}
     if (nJet_pfakt5 < pfjetnmin_ || it->pt() > pfjetptthr_) {

     ptJet_pfakt5[nJet_pfakt5] = it->pt();
     eJet_pfakt5[nJet_pfakt5] = it->energy();	 
     etaJet_pfakt5[nJet_pfakt5] = it->eta();	 
     phiJet_pfakt5[nJet_pfakt5] = it->phi();	      
     
     // Jet Energy Scale Corrections on-the-fly     
     PFJet  correctedJet = *it;
     double scale = corrector_pfakt5->correction(it->p4());
     correctedJet.scaleEnergy(scale);
     ptCorrJet_pfakt5[nJet_pfakt5] = correctedJet.pt();

     // Extra variables for PFlow studies
     Int_t nChargedHadrons = 0;
     Int_t nPhotons = 0;
     Int_t nNeutralHadrons = 0;
     Int_t nElectrons = 0;
     Int_t nMuons = 0;
     Int_t nHFHadrons = 0;
     Int_t nHFEM = 0;

     TLorentzVector p4ChargedHadrons;
     TLorentzVector p4Photons;
     TLorentzVector p4NeutralHadrons;
     TLorentzVector p4Electrons;
     TLorentzVector p4Muons;
     TLorentzVector p4HFHadrons;
     TLorentzVector p4HFEM;

     vector<PFCandidatePtr> pfCandidates = it->getPFConstituents();


     for (vector<PFCandidatePtr>::const_iterator jt = pfCandidates.begin();
          jt != pfCandidates.end(); ++jt) {

       PFCandidate::ParticleType id = (*jt)->particleId();
       // Convert particle momentum to normal TLorentzVector, wrong type :(
       math::XYZTLorentzVectorD const& p4t = (*jt)->p4();
       TLorentzVector p4(p4t.px(), p4t.py(), p4t.pz(), p4t.energy());

       // Store PFCandidates for two leading jets
       int ijet = it - pfjetsakt5->begin();
       if (ijet < 2 && nPF < nMaxPF) {

	 int pdgId = (*jt)->translateTypeToPdgId(id);
	 // "antiparticlate" the dummy neutral HF particles
	 // to tell them apart from h and gamma elsewhere
	 if (id==PFCandidate::h_HF || id==PFCandidate::egamma_HF) pdgId *= -1;
	 pdgIdPF[nPF] = pdgId;
	 ptPF[nPF] = (*jt)->pt();
	 ePF[nPF] = (*jt)->energy();
	 etaPF[nPF] = (*jt)->eta();
	 phiPF[nPF] = (*jt)->phi();

	 ++nPF;
       }

       if (id==PFCandidate::h) { // charged hadrons
         nChargedHadrons += 1;
	 p4ChargedHadrons += p4;
       }
       if (id==PFCandidate::e) { // electrons
         nElectrons += 1;
	 p4Electrons += p4;
       }
       if (id==PFCandidate::mu) { // muons
         nMuons += 1;
	 p4Muons += p4;
       }
       if (id==PFCandidate::gamma) { // photons
         nPhotons += 1;
	 p4Photons += p4;
       }
       if (id==PFCandidate::h0) { // neutral hadrons
         nNeutralHadrons += 1;
	 p4NeutralHadrons += p4;
       }
       if (id==PFCandidate::h_HF) { // HF hadrons
         nHFHadrons += 1;
	 p4HFHadrons += p4;
       }
       if (id==PFCandidate::egamma_HF) { // HF EM clusters
         nHFEM += 1;
	 p4HFEM += p4;
       }

     } //for PFCandidates

     const TLorentzVector *p = 0;

     nChargedHadrons_pfakt5[nJet_pfakt5] =  nChargedHadrons;
     p = &p4ChargedHadrons;
     eChargedHadrons_pfakt5[nJet_pfakt5] = p->E() / it->energy();
     ptChargedHadrons_pfakt5[nJet_pfakt5] = p->Pt() / it->pt();
     phiChargedHadrons_pfakt5[nJet_pfakt5] = (p->Pt() ?
					   delta_phi(p->Phi(), it->phi()) : 0);
     etaChargedHadrons_pfakt5[nJet_pfakt5] = (p->Pt() ?
					      p->Eta() - it->eta() : 0);
  
     nElectrons_pfakt5[nJet_pfakt5] =  nElectrons;
     p = &p4Electrons;
     eElectrons_pfakt5[nJet_pfakt5] = p->E() / it->energy();
     ptElectrons_pfakt5[nJet_pfakt5] = p->Pt() / it->pt();
     phiElectrons_pfakt5[nJet_pfakt5] = (p->Pt() ?
					 delta_phi(p->Phi(), it->phi()) : 0);
     etaElectrons_pfakt5[nJet_pfakt5] = (p->Pt() ? p->Eta() - it->eta() : 0);
  
     nMuons_pfakt5[nJet_pfakt5] =  nMuons;
     p = &p4Muons;
     eMuons_pfakt5[nJet_pfakt5] = p->E() / it->energy();
     ptMuons_pfakt5[nJet_pfakt5] = p->Pt() / it->pt();
     phiMuons_pfakt5[nJet_pfakt5] = (p->Pt() ?
				     delta_phi(p->Phi(), it->phi()) : 0);
     etaMuons_pfakt5[nJet_pfakt5] = (p->Pt() ? p->Eta() - it->eta() : 0);

     nPhotons_pfakt5[nJet_pfakt5] =  nPhotons;
     p = &p4Photons;
     ePhotons_pfakt5[nJet_pfakt5] = p->E() / it->energy();
     ptPhotons_pfakt5[nJet_pfakt5] = p->Pt() / it->pt();
     phiPhotons_pfakt5[nJet_pfakt5] = (p->Pt() ?
				       delta_phi(p->Phi(), it->phi()) : 0);
     etaPhotons_pfakt5[nJet_pfakt5] = (p->Pt() ? p->Eta() - it->eta() : 0);

     nNeutralHadrons_pfakt5[nJet_pfakt5] =  nNeutralHadrons;
     p = &p4NeutralHadrons;
     eNeutralHadrons_pfakt5[nJet_pfakt5] = p->E() / it->energy();
     ptNeutralHadrons_pfakt5[nJet_pfakt5] = p->Pt() / it->pt();
     phiNeutralHadrons_pfakt5[nJet_pfakt5] = (p->Pt() ?
					   delta_phi(p->Phi(), it->phi()) : 0);
     etaNeutralHadrons_pfakt5[nJet_pfakt5] = (p->Pt() ?
					      p->Eta() - it->eta() : 0);
  
     nHFHadrons_pfakt5[nJet_pfakt5] =  nHFHadrons;
     p = &p4HFHadrons;
     eHFHadrons_pfakt5[nJet_pfakt5] = p->E() / it->energy();
     ptHFHadrons_pfakt5[nJet_pfakt5] = p->Pt() / it->pt();
     phiHFHadrons_pfakt5[nJet_pfakt5] = (p->Pt() ?
					 delta_phi(p->Phi(), it->phi()) : 0);
     etaHFHadrons_pfakt5[nJet_pfakt5] = (p->Pt() ? p->Eta() - it->eta() : 0);

     nHFEM_pfakt5[nJet_pfakt5] =  nHFEM;
     p = &p4HFEM;
     eHFEM_pfakt5[nJet_pfakt5] = p->E() / it->energy();
     ptHFEM_pfakt5[nJet_pfakt5] = p->Pt() / it->pt();
     phiHFEM_pfakt5[nJet_pfakt5] = (p->Pt() ?
				    delta_phi(p->Phi(), it->phi()) : 0);
     etaHFEM_pfakt5[nJet_pfakt5] = (p->Pt() ? p->Eta() - it->eta() : 0);

     ++nJet_pfakt5;

   } // if >pfjetptthr     
   } // pfakt5

   for (PFJetCollection::const_iterator it = pfjetsakt7->begin(); 
	 it != pfjetsakt7->end(); ++it) {
     
     if (nJet_pfakt7>=100) {cout << "number of reco jets pfakt7 is larger than 100. Skipping" << endl; continue;}
     if (nJet_pfakt7 < pfjetnmin_ || it->pt() > pfjetptthr_) {

       ptJet_pfakt7[nJet_pfakt7] = it->pt();
       eJet_pfakt7[nJet_pfakt7] = it->energy();	 
       etaJet_pfakt7[nJet_pfakt7] = it->eta();	 
       phiJet_pfakt7[nJet_pfakt7] = it->phi();	      
     
       // Jet Energy Scale Corrections on-the-fly     
       PFJet  correctedJet = *it;
       double scale = corrector_pfakt7->correction(it->p4());
       correctedJet.scaleEnergy(scale);
       ptCorrJet_pfakt7[nJet_pfakt7] = correctedJet.pt();

       // Extra variables for PFlow studies
       Int_t nChargedHadrons = 0;
       Int_t nPhotons = 0;
       Int_t nNeutralHadrons = 0;
       Int_t nElectrons = 0;
       Int_t nMuons = 0;
       Int_t nHFHadrons = 0;
       Int_t nHFEM = 0;
     
       TLorentzVector p4ChargedHadrons;
       TLorentzVector p4Photons;
       TLorentzVector p4NeutralHadrons;
       TLorentzVector p4Electrons;
       TLorentzVector p4Muons;
       TLorentzVector p4HFHadrons;
       TLorentzVector p4HFEM;
     
       vector<PFCandidatePtr> pfCandidates = it->getPFConstituents();
     
       for (vector<PFCandidatePtr>::const_iterator jt = pfCandidates.begin();
            jt != pfCandidates.end(); ++jt) {
     
         PFCandidate::ParticleType id = (*jt)->particleId();
         // Convert particle momentum to normal TLorentzVector, wrong type :(
         math::XYZTLorentzVectorD const& p4t = (*jt)->p4();
         TLorentzVector p4(p4t.px(), p4t.py(), p4t.pz(), p4t.energy());
     
         // Store PFCandidates for two leading jets
         int ijet = it - pfjetsakt7->begin();
         if (ijet < 2 && nPF < nMaxPF) {
     
         int pdgId = (*jt)->translateTypeToPdgId(id);
         // "antiparticlate" the dummy neutral HF particles
         // to tell them apart from h and gamma elsewhere
         if (id==PFCandidate::h_HF || id==PFCandidate::egamma_HF) pdgId *= -1;
         pdgIdPF[nPF] = pdgId;
         ptPF[nPF] = (*jt)->pt();
         ePF[nPF] = (*jt)->energy();
         etaPF[nPF] = (*jt)->eta();
         phiPF[nPF] = (*jt)->phi();
     
         ++nPF;
         }
     
         if (id==PFCandidate::h) { // charged hadrons
           nChargedHadrons += 1;
         p4ChargedHadrons += p4;
         }
         if (id==PFCandidate::e) { // electrons
           nElectrons += 1;
         p4Electrons += p4;
         }
         if (id==PFCandidate::mu) { // muons
           nMuons += 1;
         p4Muons += p4;
         }
         if (id==PFCandidate::gamma) { // photons
           nPhotons += 1;
         p4Photons += p4;
         }
         if (id==PFCandidate::h0) { // neutral hadrons
           nNeutralHadrons += 1;
         p4NeutralHadrons += p4;
         }
         if (id==PFCandidate::h_HF) { // HF hadrons
           nHFHadrons += 1;
         p4HFHadrons += p4;
         }
         if (id==PFCandidate::egamma_HF) { // HF EM clusters
           nHFEM += 1;
         p4HFEM += p4;
         }
     
       } //for PFCandidates
     
       const TLorentzVector *p = 0;
     
       nChargedHadrons_pfakt7[nJet_pfakt7] =  nChargedHadrons;
       p = &p4ChargedHadrons;
       eChargedHadrons_pfakt7[nJet_pfakt7] = p->E() / it->energy();
       ptChargedHadrons_pfakt7[nJet_pfakt7] = p->Pt() / it->pt();
       phiChargedHadrons_pfakt7[nJet_pfakt7] = (p->Pt() ?
        				   delta_phi(p->Phi(), it->phi()) : 0);
       etaChargedHadrons_pfakt7[nJet_pfakt7] = (p->Pt() ?
        				      p->Eta() - it->eta() : 0);
     
       nElectrons_pfakt7[nJet_pfakt7] =  nElectrons;
       p = &p4Electrons;
       eElectrons_pfakt7[nJet_pfakt7] = p->E() / it->energy();
       ptElectrons_pfakt7[nJet_pfakt7] = p->Pt() / it->pt();
       phiElectrons_pfakt7[nJet_pfakt7] = (p->Pt() ?
        				 delta_phi(p->Phi(), it->phi()) : 0);
       etaElectrons_pfakt7[nJet_pfakt7] = (p->Pt() ? p->Eta() - it->eta() : 0);
     
       nMuons_pfakt7[nJet_pfakt7] =  nMuons;
       p = &p4Muons;
       eMuons_pfakt7[nJet_pfakt7] = p->E() / it->energy();
       ptMuons_pfakt7[nJet_pfakt7] = p->Pt() / it->pt();
       phiMuons_pfakt7[nJet_pfakt7] = (p->Pt() ?
        			     delta_phi(p->Phi(), it->phi()) : 0);
       etaMuons_pfakt7[nJet_pfakt7] = (p->Pt() ? p->Eta() - it->eta() : 0);
     
       nPhotons_pfakt7[nJet_pfakt7] =  nPhotons;
       p = &p4Photons;
       ePhotons_pfakt7[nJet_pfakt7] = p->E() / it->energy();
       ptPhotons_pfakt7[nJet_pfakt7] = p->Pt() / it->pt();
       phiPhotons_pfakt7[nJet_pfakt7] = (p->Pt() ?
        			       delta_phi(p->Phi(), it->phi()) : 0);
       etaPhotons_pfakt7[nJet_pfakt7] = (p->Pt() ? p->Eta() - it->eta() : 0);
     
       nNeutralHadrons_pfakt7[nJet_pfakt7] =  nNeutralHadrons;
       p = &p4NeutralHadrons;
       eNeutralHadrons_pfakt7[nJet_pfakt7] = p->E() / it->energy();
       ptNeutralHadrons_pfakt7[nJet_pfakt7] = p->Pt() / it->pt();
       phiNeutralHadrons_pfakt7[nJet_pfakt7] = (p->Pt() ?
        				   delta_phi(p->Phi(), it->phi()) : 0);
       etaNeutralHadrons_pfakt7[nJet_pfakt7] = (p->Pt() ?
        				      p->Eta() - it->eta() : 0);
     
       nHFHadrons_pfakt7[nJet_pfakt7] =  nHFHadrons;
       p = &p4HFHadrons;
       eHFHadrons_pfakt7[nJet_pfakt7] = p->E() / it->energy();
       ptHFHadrons_pfakt7[nJet_pfakt7] = p->Pt() / it->pt();
       phiHFHadrons_pfakt7[nJet_pfakt7] = (p->Pt() ?
        				 delta_phi(p->Phi(), it->phi()) : 0);
       etaHFHadrons_pfakt7[nJet_pfakt7] = (p->Pt() ? p->Eta() - it->eta() : 0);
     
       nHFEM_pfakt7[nJet_pfakt7] =  nHFEM;
       p = &p4HFEM;
       eHFEM_pfakt7[nJet_pfakt7] = p->E() / it->energy();
       ptHFEM_pfakt7[nJet_pfakt7] = p->Pt() / it->pt();
       phiHFEM_pfakt7[nJet_pfakt7] = (p->Pt() ?
        			    delta_phi(p->Phi(), it->phi()) : 0);
       etaHFEM_pfakt7[nJet_pfakt7] = (p->Pt() ? p->Eta() - it->eta() : 0);
    
       ++nJet_pfakt7;

     } 
   }

// for (PFJetCollection::const_iterator it = pfjetssis5->begin(); 
//     it != pfjetssis5->end(); ++it) {
//   
//   if (nJet_pfsis5>=100) {cout << "number of reco jets pfsis5 is larger than 100. Skipping" << endl; continue;}
//   if (nJet_pfsis5 < pfjetnmin_ || it->pt() > pfjetptthr_) {

//     ptJet_pfsis5[nJet_pfsis5] = it->pt();
//     eJet_pfsis5[nJet_pfsis5] = it->energy();	 
//     etaJet_pfsis5[nJet_pfsis5] = it->eta();	 
//     phiJet_pfsis5[nJet_pfsis5] = it->phi();	      
//     
//     nJet_pfsis5++;
//   }
// }

   for (PFJetCollection::const_iterator it = pfjetskt6->begin(); 
	 it != pfjetskt6->end(); ++it) {
     
     if (nJet_pfkt6>=100) {cout << "number of reco jets pfkt6 is larger than 100. Skipping" << endl; continue;}
     if (nJet_pfkt6 < pfjetnmin_ || it->pt() > pfjetptthr_) {

       ptJet_pfkt6[nJet_pfkt6] = it->pt();
       eJet_pfkt6[nJet_pfkt6] = it->energy();	 
       etaJet_pfkt6[nJet_pfkt6] = it->eta();	 
       phiJet_pfkt6[nJet_pfkt6] = it->phi();	      
       
       nJet_pfkt6++;
     }
   }

// for (PFJetCollection::const_iterator it = pfjetssis7->begin(); 
//     it != pfjetssis7->end(); ++it) {
//   
//   if (nJet_pfsis7>=100) {cout << "number of reco jets pfsis7 is larger than 100. Skipping" << endl; continue;}
//   if (nJet_pfsis7 < pfjetnmin_ || it->pt() > pfjetptthr_) {

//     ptJet_pfsis7[nJet_pfsis7] = it->pt();
//     eJet_pfsis7[nJet_pfsis7] = it->energy();	 
//     etaJet_pfsis7[nJet_pfsis7] = it->eta();	 
//     phiJet_pfsis7[nJet_pfsis7] = it->phi();	      
//     
//     nJet_pfsis7++;
//   }
// }
   // Fill caloMET

   const CaloMETCollection *calometcol = calomethandle.product();
   CaloMET const& calomet = calometcol->front();
   
   sMet = calomet.sumEt();
   eMet = calomet.pt();	 
   phiMet = calomet.phi();	      

   // Fill tcMET

   const View<MET> *tcmetcol = tcmethandle.product();
   MET const& tcmet = tcmetcol->front();
   
   stcMet = tcmet.sumEt();
   etcMet = tcmet.pt();
   phitcMet = tcmet.phi();
     
   // Fill pfMET

   const PFMETCollection *pfmetcol = pfmethandle.product();
   PFMET const& pfmet = pfmetcol->front();

   spfMet = pfmet.sumEt();
   epfMet = pfmet.pt();
   phipfMet = pfmet.phi();

   sMetGen = 0.;
   eMetGen = 0.;
   phiMetGen = 0.;

   sMetGen2 = 0.;
   eMetGen2 = 0.;
   phiMetGen2 = 0.;

   if( isMC ) {
     // Fill gen MET

     const GenMETCollection *genmetcol = genmethandle.product();
     GenMET const& genmet = genmetcol->front();

     sMetGen = genmet.sumEt();
     eMetGen = genmet.energy();
     phiMetGen = genmet.phi();

     const GenMETCollection *genmetcol2 = genmethandle2.product();
     GenMET const& genmet2 = genmetcol2->front();

     sMetGen2 = genmet2.sumEt();
     eMetGen2 = genmet2.energy();
     phiMetGen2 = genmet2.phi();

   } //if is MC
   
   //event++;  
   m_tree->Fill();

}


// ------------ method called once each job just before starting event loop  ------------
void 
GammaJetAnalyzer::beginJob()
{

  //m_tree = fs_->make<TTree>("pippo","Analysis tree");
  outfile = TFile::Open("output.root", "RECREATE");
  outfile->mkdir("myanalysis");
  outfile->cd("myanalysis");

  m_tree = new TTree ("pippo","Analysis tree") ;
  //  m_tree->SetAutoSave (10000000) ;
  m_tree->Branch("genpt",&genpt,"genpt/F");

  m_tree->Branch("isMC",&isMC,"isMC/O");
  m_tree->Branch("store",&store,"store/I");
  m_tree->Branch("lbn",&lbn,"lbn/I");
  m_tree->Branch("bx",&bx,"bx/I");
  m_tree->Branch("orbit",&orbit,"orbit/I");
  m_tree->Branch("run",&run,"run/I");
  m_tree->Branch("event",&event,"event/I");

  // Problem: nMC==100 always, and sometimes last particle has very high pT
  // => could be losing interesting particles, even quarks/gluons (status==2)
  // hmmm, status==1 particles have pT less than ~4.5 GeV for pThat>500 => ok
  m_tree->Branch("nMC",&nMC,"nMC/I");
  m_tree->Branch("pdgIdMC",&pdgIdMC,"pdgIdMC[nMC]/I");
  m_tree->Branch("statusMC",&statusMC,"statusMC[nMC]/I");
  m_tree->Branch("motherIDMC",&motherIDMC,"motherIDMC[nMC]/I");
  // Most MC particles have mass, but why do photons (status=1 and 3) have mass?
  //m_tree->Branch("massMC ",&massMC ,"massMC[nMC]/F");
  //to a good approximation, m = sqrt(e^2 - (pt*cosh(eta))^2), when m>1e-6 GeV
  m_tree->Branch("ptMC ",&ptMC ,"ptMC[nMC]/F");
  m_tree->Branch("eMC  ",&eMC  ,"eMC[nMC]/F");
  m_tree->Branch("etaMC",&etaMC,"etaMC[nMC]/F");
  m_tree->Branch("phiMC",&phiMC,"phiMC[nMC]/F");

  m_tree->Branch("nSIM",&nSIM,"nSIM/I");
  m_tree->Branch("pdgIdSIM",&pdgIdSIM,"pdgIdSIM[nSIM]/I");
  m_tree->Branch("statusSIM",&statusSIM,"statusSIM[nSIM]/I");
  m_tree->Branch("ptSIM ",&ptSIM ,"ptSIM[nSIM]/F");
  m_tree->Branch("eSIM  ",&eSIM  ,"eSIM[nSIM]/F");
  m_tree->Branch("etaSIM",&etaSIM,"etaSIM[nSIM]/F");
  m_tree->Branch("phiSIM",&phiSIM,"phiSIM[nSIM]/F");
  m_tree->Branch("rSIM",&rSIM,"rSIM[nSIM]/F");
  m_tree->Branch("zSIM",&zSIM,"zSIM[nSIM]/F");

  m_tree->Branch("nPF",&nPF,"nPF/I");
  m_tree->Branch("pdgIdPF",&pdgIdPF,"pdgIdPF[nPF]/I");
  m_tree->Branch("ptPF ",&ptPF ,"ptPF[nPF]/F");
  m_tree->Branch("ePF  ",&ePF  ,"ePF[nPF]/F");
  m_tree->Branch("etaPF",&etaPF,"etaPF[nPF]/F");
  m_tree->Branch("phiPF",&phiPF,"phiPF[nPF]/F");

  m_tree->Branch("nPhot",&nPhot,"nPhot/I");
  m_tree->Branch("ptPhot ",&ptPhot ,"ptPhot[nPhot]/F");
  m_tree->Branch("ePhot  ",&ePhot  ,"ePhot[nPhot]/F");
  m_tree->Branch("escPhot  ",&escPhot  ,"escPhot[nPhot]/F");
  m_tree->Branch("eseedPhot  ",&eseedPhot  ,"eseedPhot[nPhot]/F");
  m_tree->Branch("etaPhot",&etaPhot,"etaPhot[nPhot]/F");
  m_tree->Branch("phiPhot",&phiPhot,"phiPhot[nPhot]/F");

  m_tree->Branch("nconvPhot",&nconvPhot,"nconvPhot/I");
  m_tree->Branch("chi2convPhot",&chi2convPhot,"chi2convPhot[nconvPhot]/F");
  m_tree->Branch("ndofconvPhot",&ndofconvPhot,"ndofconvPhot[nconvPhot]/F");
  m_tree->Branch("rconvPhot",&rconvPhot,"rconvPhot[nconvPhot]/F");
  m_tree->Branch("phiconvPhot",&phiconvPhot,"phiconvPhot[nconvPhot]/F");
  m_tree->Branch("zconvPhot",&zconvPhot,"zconvPhot[nconvPhot]/F");
  m_tree->Branch("ntrkconvPhot",&ntrkconvPhot,"ntrkconvPhot[nconvPhot]/I");
  m_tree->Branch("eovpconvPhot",&eovpconvPhot,"eovpconvPhot[nconvPhot]/F");
  m_tree->Branch("etaecalconvPhot",&etaecalconvPhot,"etaecalconvPhot[nconvPhot]/F");
  m_tree->Branch("phiecalconvPhot",&phiecalconvPhot,"phiecalconvPhot[nconvPhot]/F");
  m_tree->Branch("eecalconvPhot",&energyecalconvPhot,"energyecalconvPhot[nconvPhot]/F");

  m_tree->Branch("algoconvPhot",&algoconvPhot,"algoconvPhot[nconvPhot]/I");
  m_tree->Branch("d0convPhot",&d0convPhot,"d0convPhot[nconvPhot]/F");
  m_tree->Branch("detaecalconvPhot",&detaecalconvPhot,"detaecalconvPhot[nconvPhot]/F");
  m_tree->Branch("dphiecalconvPhot",&dphiecalconvPhot,"dphiecalconvPhot[nconvPhot]/F");
  m_tree->Branch("dphivtxconvPhot",&dphivtxconvPhot,"dphivtxconvPhot[nconvPhot]/F");
  m_tree->Branch("pairsepconvPhot",&pairsepconvPhot,"pairsepconvPhot[nconvPhot]/F");
  m_tree->Branch("pairmassconvPhot",&pairmassconvPhot,"pairmassconvPhot[nconvPhot]/F");

  m_tree->Branch("trchi21convPhot",&trchi21convPhot,"trchi21convPhot[nconvPhot]/F");
  m_tree->Branch("trndof1convPhot",&trndof1convPhot,"trndof1convPhot[nconvPhot]/F");
  m_tree->Branch("trqual1convPhot",&trqual1convPhot,"trqual1convPhot[nconvPhot]/I");
  m_tree->Branch("trpt1convPhot",&trpt1convPhot,"trpt1convPhot[nconvPhot]/F");
  m_tree->Branch("trerr1convPhot",&trerr1convPhot,"trerr1convPhot[nconvPhot]/F");

  m_tree->Branch("trchi22convPhot",&trchi22convPhot,"trchi22convPhot[nconvPhot]/F");
  m_tree->Branch("trndof2convPhot",&trndof2convPhot,"trndof2convPhot[nconvPhot]/F");
  m_tree->Branch("trqual2convPhot",&trqual2convPhot,"trqual2convPhot[nconvPhot]/I");
  m_tree->Branch("trpt2convPhot",&trpt2convPhot,"trpt2convPhot[nconvPhot]/F");
  m_tree->Branch("trerr2convPhot",&trerr2convPhot,"trerr2convPhot[nconvPhot]/F");

  // Default photon ID
  m_tree->Branch("pid_isEM",&pid_isEM,"pid_isEM[nPhot]/O");
  m_tree->Branch("pid_isLoose",&pid_isLoose,"pid_isLoose[nPhot]/O");
  m_tree->Branch("pid_isTight",&pid_isTight,"pid_isTight[nPhot]/O");
  m_tree->Branch("pid_jurECAL",&pid_jurECAL,"pid_jurECAL[nPhot]/F");
  m_tree->Branch("pid_twrHCAL",&pid_twrHCAL,"pid_twrHCAL[nPhot]/F");
  m_tree->Branch("pid_HoverE",&pid_HoverE,"pid_HoverE[nPhot]/F");
  m_tree->Branch("pid_hlwTrack",&pid_hlwTrack,"pid_hlwTarck[nPhot]/F");
  m_tree->Branch("pid_etawid",&pid_etawid,"pid_etawid[nPhot]/F");

  m_tree->Branch("ptiso0015Phot",&ptiso0015Phot,"ptiso0015Phot[nPhot]/F");
  m_tree->Branch("ntrkiso0015Phot",&ntrkiso0015Phot,"ntrkiso0015Phot[nPhot]/I");
  m_tree->Branch("ptiso035Phot",&ptiso035Phot,"ptiso035Phot[nPhot]/F");
  m_tree->Branch("ntrkiso035Phot",&ntrkiso035Phot,"ntrkiso035Phot[nPhot]/I");
  m_tree->Branch("ptiso04Phot",&ptiso04Phot,"ptiso04Phot[nPhot]/F");
  m_tree->Branch("ntrkiso04Phot",&ntrkiso04Phot,"ntrkiso04Phot[nPhot]/I");
  m_tree->Branch("ptiso05Phot",&ptiso05Phot,"ptiso05Phot[nPhot]/F");
  m_tree->Branch("ntrkiso05Phot",&ntrkiso05Phot,"ntrkiso05Phot[nPhot]/I");
  m_tree->Branch("ptiso07Phot",&ptiso07Phot,"ptiso07Phot[nPhot]/F");
  m_tree->Branch("ntrkiso07Phot",&ntrkiso07Phot,"ntrkiso07Phot[nPhot]/I");
  m_tree->Branch("ptiso1Phot",&ptiso1Phot,"ptiso1Phot[nPhot]/F");
  m_tree->Branch("ntrkiso1Phot",&ntrkiso1Phot,"ntrkiso1Phot[nPhot]/I");

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

  /*
  // Could replace multiple explicit lines with a loop in the future
  for (int i = 0; i != nAlgo; ++i) {

    string s = Form("%s_%s", (algo[i]->second==kGenAlgo ? "Gen" : ""),
                    algo[i]->first);
    string c = s.c_str();

    m_tree->Branch(Form("nJet%s", c), &nJet[i], Form("nJet%s/I", c));
    m_tree->Branch(Form("ptJet%s", c), &ptJet[i],
		   Form("ptJet%s[nJet%s]/F", c, c));
    m_tree->Branch(Form("eJet%s", c), &eJet[i],
		   Form("eJet%s[nJet%s]/F", c, c));
    m_tree->Branch(Form("etaJet%s", c), &etaJet[i],
		   Form("etaJet%s[nJet%s]/F", c, c));
    m_tree->Branch(Form("phiJet%s", c), &phiJet[i],
		   Form("phiJet%s[nJet%s]/F", c, c));

    if (algo[i]->second==kCaloAlgo) {
      m_tree->Branch(Form("emfJet%s", c), &emfJet[i],
		     Form("emfJet%s[nJet%s]/F", c, c));
    }
    if (algo[i]->second==kPFAlgo) {
      m_tree->Branch(Form("emfJet%s", c), &emfJet[i],
		     Form("emfJet%s[nJet%s]/F", c, c));
    }
  }
  */

  m_tree->Branch("nJet_ite",&nJet_ite,"nJet_ite/I");
  m_tree->Branch("ptJet_ite ",&ptJet_ite ,"ptJet_ite[nJet_ite]/F");
  m_tree->Branch("eJet_ite  ",&eJet_ite  ,"eJet_ite[nJet_ite]/F");
  m_tree->Branch("etaJet_ite",&etaJet_ite,"etaJet_ite[nJet_ite]/F");
  m_tree->Branch("phiJet_ite",&phiJet_ite,"phiJet_ite[nJet_ite]/F");
  m_tree->Branch("emfJet_ite",&emfJet_ite,"emfJet_ite[nJet_ite]/F");

  m_tree->Branch("nJet_kt4",&nJet_kt4,"nJet_kt4/I");
  m_tree->Branch("ptJet_kt4 ",&ptJet_kt4 ,"ptJet_kt4[nJet_kt4]/F");
  m_tree->Branch("eJet_kt4  ",&eJet_kt4  ,"eJet_kt4[nJet_kt4]/F");
  m_tree->Branch("etaJet_kt4",&etaJet_kt4,"etaJet_kt4[nJet_kt4]/F");
  m_tree->Branch("phiJet_kt4",&phiJet_kt4,"phiJet_kt4[nJet_kt4]/F");
  m_tree->Branch("emfJet_kt4",&emfJet_kt4,"emfJet_kt4[nJet_kt4]/F");

  m_tree->Branch("nJet_kt6",&nJet_kt6,"nJet_kt6/I");
  m_tree->Branch("ptJet_kt6 ",&ptJet_kt6 ,"ptJet_kt6[nJet_kt6]/F");
  m_tree->Branch("eJet_kt6  ",&eJet_kt6  ,"eJet_kt6[nJet_kt6]/F");
  m_tree->Branch("etaJet_kt6",&etaJet_kt6,"etaJet_kt6[nJet_kt6]/F");
  m_tree->Branch("phiJet_kt6",&phiJet_kt6,"phiJet_kt6[nJet_kt6]/F");
  m_tree->Branch("emfJet_kt6",&emfJet_kt6,"emfJet_kt6[nJet_kt6]/F");

  m_tree->Branch("nJet_akt5",&nJet_akt5,"nJet_akt5/I");
  m_tree->Branch("ptJet_akt5 ",&ptJet_akt5 ,"ptJet_akt5[nJet_akt5]/F");
  m_tree->Branch("ptCorrJet_akt5 ",&ptCorrJet_akt5 ,"ptCorrJet_akt5[nJet_akt5]/F");
  m_tree->Branch("eJet_akt5  ",&eJet_akt5  ,"eJet_akt5[nJet_akt5]/F");
  m_tree->Branch("etaJet_akt5",&etaJet_akt5,"etaJet_akt5[nJet_akt5]/F");
  m_tree->Branch("phiJet_akt5",&phiJet_akt5,"phiJet_akt5[nJet_akt5]/F");
  m_tree->Branch("emfJet_akt5",&emfJet_akt5,"emfJet_akt5[nJet_akt5]/F");

  //m_tree->Branch("nJet_akt7",&nJet_akt7,"nJet_akt7/I");
  //m_tree->Branch("ptJet_akt7 ",&ptJet_akt7 ,"ptJet_akt7[nJet_akt7]/F");
  //m_tree->Branch("eJet_akt7  ",&eJet_akt7  ,"eJet_akt7[nJet_akt7]/F");
  //m_tree->Branch("etaJet_akt7",&etaJet_akt7,"etaJet_akt7[nJet_akt7]/F");
  //m_tree->Branch("phiJet_akt7",&phiJet_akt7,"phiJet_akt7[nJet_akt7]/F");
  //m_tree->Branch("emfJet_akt7",&emfJet_akt7,"emfJet_akt7[nJet_akt7]/F");

  m_tree->Branch("nJet_sis5",&nJet_sis5,"nJet_sis5/I");
  m_tree->Branch("ptJet_sis5 ",&ptJet_sis5 ,"ptJet_sis5[nJet_sis5]/F");
  m_tree->Branch("eJet_sis5  ",&eJet_sis5  ,"eJet_sis5[nJet_sis5]/F");
  m_tree->Branch("etaJet_sis5",&etaJet_sis5,"etaJet_sis5[nJet_sis5]/F");
  m_tree->Branch("phiJet_sis5",&phiJet_sis5,"phiJet_sis5[nJet_sis5]/F");
  m_tree->Branch("emfJet_sis5",&emfJet_sis5,"emfJet_sis5[nJet_sis5]/F");

  m_tree->Branch("nJet_sis7",&nJet_sis7,"nJet_sis7/I");
  m_tree->Branch("ptJet_sis7 ",&ptJet_sis7 ,"ptJet_sis7[nJet_sis7]/F");
  m_tree->Branch("eJet_sis7  ",&eJet_sis7  ,"eJet_sis7[nJet_sis7]/F");
  m_tree->Branch("etaJet_sis7",&etaJet_sis7,"etaJet_sis7[nJet_sis7]/F");
  m_tree->Branch("phiJet_sis7",&phiJet_sis7,"phiJet_sis7[nJet_sis7]/F");
  m_tree->Branch("emfJet_sis7",&emfJet_sis7,"emfJet_kt4[nJet_sis7]/F");

  m_tree->Branch("nJet_jptak5",&nJet_jptak5,"nJet_jptak5/I");
  m_tree->Branch("ptJet_jptak5 ",&ptJet_jptak5 ,"ptJet_jptak5[nJet_jptak5]/F");
  m_tree->Branch("eJet_jptak5  ",&eJet_jptak5  ,"eJet_jptak5[nJet_jptak5]/F");
  m_tree->Branch("etaJet_jptak5",&etaJet_jptak5,"etaJet_jptak5[nJet_jptak5]/F");
  m_tree->Branch("phiJet_jptak5",&phiJet_jptak5,"phiJet_jptak5[nJet_jptak5]/F");
  m_tree->Branch("emfJet_jptak5",&emfJet_jptak5,"emfJet_jptak5[nJet_jptak5]/F");

  m_tree->Branch("nJet_pfite",&nJet_pfite,"nJet_pfite/I");
  m_tree->Branch("ptJet_pfite ",&ptJet_pfite ,"ptJet_pfite[nJet_pfite]/F");
  m_tree->Branch("eJet_pfite  ",&eJet_pfite  ,"eJet_pfite[nJet_pfite]/F");
  m_tree->Branch("etaJet_pfite",&etaJet_pfite,"etaJet_pfite[nJet_pfite]/F");
  m_tree->Branch("phiJet_pfite",&phiJet_pfite,"phiJet_pfite[nJet_pfite]/F");

  m_tree->Branch("nJet_pfkt4",&nJet_pfkt4,"nJet_pfkt4/I");
  m_tree->Branch("ptJet_pfkt4 ",&ptJet_pfkt4 ,"ptJet_pfkt4[nJet_pfkt4]/F");
  m_tree->Branch("eJet_pfkt4  ",&eJet_pfkt4  ,"eJet_pfkt4[nJet_pfkt4]/F");
  m_tree->Branch("etaJet_pfkt4",&etaJet_pfkt4,"etaJet_pfkt4[nJet_pfkt4]/F");
  m_tree->Branch("phiJet_pfkt4",&phiJet_pfkt4,"phiJet_pfkt4[nJet_pfkt4]/F");

  m_tree->Branch("nJet_pfakt5",&nJet_pfakt5,"nJet_pfakt5/I");
  m_tree->Branch("ptJet_pfakt5 ",&ptJet_pfakt5 ,"ptJet_pfakt5[nJet_pfakt5]/F");
  m_tree->Branch("ptCorrJet_pfakt5 ",&ptCorrJet_pfakt5 ,"ptCorrJet_pfakt5[nJet_pfakt5]/F");
  m_tree->Branch("eJet_pfakt5  ",&eJet_pfakt5  ,"eJet_pfakt5[nJet_pfakt5]/F");
  m_tree->Branch("etaJet_pfakt5",&etaJet_pfakt5,"etaJet_pfakt5[nJet_pfakt5]/F");
  m_tree->Branch("phiJet_pfakt5",&phiJet_pfakt5,"phiJet_pfakt5[nJet_pfakt5]/F");

//   // Extra variables for PFlow studies
  m_tree->Branch("nChargedHadrons_pfakt5",nChargedHadrons_pfakt5,"nChargedHadrons_pfakt5[nJet_pfakt5]/I");
  m_tree->Branch("nPhotons_pfakt5",       nPhotons_pfakt5,       "nPhotons_pfakt5[nJet_pfakt5]/I");
  m_tree->Branch("nMuons_pfakt5",         nMuons_pfakt5,         "nMuons_pfakt5[nJet_pfakt5]/I");
  m_tree->Branch("nElectrons_pfakt5",     nElectrons_pfakt5,     "nElectrons_pfakt5[nJet_pfakt5]/I");
  m_tree->Branch("nNeutralHadrons_pfakt5",nNeutralHadrons_pfakt5,"nNeutralHadrons_pfakt5[nJet_pfakt5]/I");
  m_tree->Branch("nHFHadrons_pfakt5",     nHFHadrons_pfakt5,     "nHFHadrons_pfakt5[nJet_pfakt5]/I");
  m_tree->Branch("nHFEM_pfakt5",     nHFEM_pfakt5,     "nHFEM_pfakt5[nJet_pfakt5]/I");

  m_tree->Branch("eChargedHadrons_pfakt5",eChargedHadrons_pfakt5,"eChargedHadrons_pfakt5[nJet_pfakt5]/F");
  m_tree->Branch("ePhotons_pfakt5",ePhotons_pfakt5,"ePhotons_pfakt5[nJet_pfakt5]/F");
  m_tree->Branch("eMuons_pfakt5",eMuons_pfakt5,"eMuons_pfakt5[nJet_pfakt5]/F");
  m_tree->Branch("eElectrons_pfakt5",eElectrons_pfakt5,"eElectrons_pfakt5[nJet_pfakt5]/F");
  m_tree->Branch("eNeutralHadrons_pfakt5",eNeutralHadrons_pfakt5,"eNeutralHadrons_pfakt5[nJet_pfakt5]/F");
  m_tree->Branch("eHFHadrons_pfakt5",eHFHadrons_pfakt5,"eHFHadrons_pfakt5[nJet_pfakt5]/F");
  m_tree->Branch("eHFEM_pfakt5",eHFEM_pfakt5,"eHFEM_pfakt5[nJet_pfakt5]/F");

  m_tree->Branch("ptChargedHadrons_pfakt5",ptChargedHadrons_pfakt5,"ptChargedHadrons_pfakt5[nJet_pfakt5]/F");
  m_tree->Branch("ptPhotons_pfakt5",       ptPhotons_pfakt5,       "ptPhotons_pfakt5[nJet_pfakt5]/F");
  m_tree->Branch("ptMuons_pfakt5",         ptMuons_pfakt5,         "ptMuons_pfakt5[nJet_pfakt5]/F");
  m_tree->Branch("ptElectrons_pfakt5",     ptElectrons_pfakt5,     "ptElectrons_pfakt5[nJet_pfakt5]/F");
  m_tree->Branch("ptNeutralHadrons_pfakt5",ptNeutralHadrons_pfakt5,"ptNeutralHadrons_pfakt5[nJet_pfakt5]/F");
  m_tree->Branch("ptHFHadrons_pfakt5",     ptHFHadrons_pfakt5,     "ptHFHadrons_pfakt5[nJet_pfakt5]/F");
  m_tree->Branch("ptHFEM_pfakt5",     ptHFEM_pfakt5,     "ptHFEM_pfakt5[nJet_pfakt5]/F");

  m_tree->Branch("phiChargedHadrons_pfakt5",phiChargedHadrons_pfakt5,"phiChargedHadrons_pfakt5[nJet_pfakt5]/F");
  m_tree->Branch("phiPhotons_pfakt5",       phiPhotons_pfakt5,       "phiPhotons_pfakt5[nJet_pfakt5]/F");
  m_tree->Branch("phiMuons_pfakt5",         phiMuons_pfakt5,         "phiMuons_pfakt5[nJet_pfakt5]/F");
  m_tree->Branch("phiElectrons_pfakt5",     phiElectrons_pfakt5,     "phiElectrons_pfakt5[nJet_pfakt5]/F");
  m_tree->Branch("phiNeutralHadrons_pfakt5",phiNeutralHadrons_pfakt5,"phiNeutralHadrons_pfakt5[nJet_pfakt5]/F");
  m_tree->Branch("phiHFHadrons_pfakt5",     phiHFHadrons_pfakt5,     "phiHFHadrons_pfakt5[nJet_pfakt5]/F");
  m_tree->Branch("phiHFEM_pfakt5",     phiHFEM_pfakt5,     "phiHFEM_pfakt5[nJet_pfakt5]/F");

  m_tree->Branch("etaChargedHadrons_pfakt5",etaChargedHadrons_pfakt5,"etaChargedHadrons_pfakt5[nJet_pfakt5]/F");
  m_tree->Branch("etaPhotons_pfakt5",       etaPhotons_pfakt5,       "etaPhotons_pfakt5[nJet_pfakt5]/F");
  m_tree->Branch("etaMuons_pfakt5",         etaMuons_pfakt5,         "etaMuons_pfakt5[nJet_pfakt5]/F");
  m_tree->Branch("etaElectrons_pfakt5",     etaElectrons_pfakt5,     "etaElectrons_pfakt5[nJet_pfakt5]/F");
  m_tree->Branch("etaNeutralHadrons_pfakt5",etaNeutralHadrons_pfakt5,"etaNeutralHadrons_pfakt5[nJet_pfakt5]/F");
  m_tree->Branch("etaHFHadrons_pfakt5",     etaHFHadrons_pfakt5,     "etaHFHadrons_pfakt5[nJet_pfakt5]/F");
  m_tree->Branch("etaHFEM_pfakt5",     etaHFEM_pfakt5,     "etaHFEM_pfakt5[nJet_pfakt5]/F");

  m_tree->Branch("nJet_pfakt7",&nJet_pfakt7,"nJet_pfakt7/I");
  m_tree->Branch("ptJet_pfakt7 ",&ptJet_pfakt7 ,"ptJet_pfakt7[nJet_pfakt7]/F");
  m_tree->Branch("ptCorrJet_pfakt7 ",&ptCorrJet_pfakt7 ,"ptCorrJet_pfakt7[nJet_pfakt7]/F");
  m_tree->Branch("eJet_pfakt7  ",&eJet_pfakt7  ,"eJet_pfakt7[nJet_pfakt7]/F");
  m_tree->Branch("etaJet_pfakt7",&etaJet_pfakt7,"etaJet_pfakt7[nJet_pfakt7]/F");
  m_tree->Branch("phiJet_pfakt7",&phiJet_pfakt7,"phiJet_pfakt7[nJet_pfakt7]/F");

//   // Extra variables for PFlow studies
  m_tree->Branch("nChargedHadrons_pfakt7",nChargedHadrons_pfakt7,"nChargedHadrons_pfakt7[nJet_pfakt7]/I");
  m_tree->Branch("nPhotons_pfakt7",       nPhotons_pfakt7,       "nPhotons_pfakt7[nJet_pfakt7]/I");
  m_tree->Branch("nMuons_pfakt7",         nMuons_pfakt7,         "nMuons_pfakt7[nJet_pfakt7]/I");
  m_tree->Branch("nElectrons_pfakt7",     nElectrons_pfakt7,     "nElectrons_pfakt7[nJet_pfakt7]/I");
  m_tree->Branch("nNeutralHadrons_pfakt7",nNeutralHadrons_pfakt7,"nNeutralHadrons_pfakt7[nJet_pfakt7]/I");
  m_tree->Branch("nHFHadrons_pfakt7",     nHFHadrons_pfakt7,     "nHFHadrons_pfakt7[nJet_pfakt7]/I");
  m_tree->Branch("nHFEM_pfakt7",     nHFEM_pfakt7,     "nHFEM_pfakt7[nJet_pfakt7]/I");

  m_tree->Branch("eChargedHadrons_pfakt7",eChargedHadrons_pfakt7,"eChargedHadrons_pfakt7[nJet_pfakt7]/F");
  m_tree->Branch("ePhotons_pfakt7",ePhotons_pfakt7,"ePhotons_pfakt7[nJet_pfakt7]/F");
  m_tree->Branch("eMuons_pfakt7",eMuons_pfakt7,"eMuons_pfakt7[nJet_pfakt7]/F");
  m_tree->Branch("eElectrons_pfakt7",eElectrons_pfakt7,"eElectrons_pfakt7[nJet_pfakt7]/F");
  m_tree->Branch("eNeutralHadrons_pfakt7",eNeutralHadrons_pfakt7,"eNeutralHadrons_pfakt7[nJet_pfakt7]/F");
  m_tree->Branch("eHFHadrons_pfakt7",eHFHadrons_pfakt7,"eHFHadrons_pfakt7[nJet_pfakt7]/F");
  m_tree->Branch("eHFEM_pfakt7",eHFEM_pfakt7,"eHFEM_pfakt7[nJet_pfakt7]/F");

  m_tree->Branch("ptChargedHadrons_pfakt7",ptChargedHadrons_pfakt7,"ptChargedHadrons_pfakt7[nJet_pfakt7]/F");
  m_tree->Branch("ptPhotons_pfakt7",       ptPhotons_pfakt7,       "ptPhotons_pfakt7[nJet_pfakt7]/F");
  m_tree->Branch("ptMuons_pfakt7",         ptMuons_pfakt7,         "ptMuons_pfakt7[nJet_pfakt7]/F");
  m_tree->Branch("ptElectrons_pfakt7",     ptElectrons_pfakt7,     "ptElectrons_pfakt7[nJet_pfakt7]/F");
  m_tree->Branch("ptNeutralHadrons_pfakt7",ptNeutralHadrons_pfakt7,"ptNeutralHadrons_pfakt7[nJet_pfakt7]/F");
  m_tree->Branch("ptHFHadrons_pfakt7",     ptHFHadrons_pfakt7,     "ptHFHadrons_pfakt7[nJet_pfakt7]/F");
  m_tree->Branch("ptHFEM_pfakt7",     ptHFEM_pfakt7,     "ptHFEM_pfakt7[nJet_pfakt7]/F");

  m_tree->Branch("phiChargedHadrons_pfakt7",phiChargedHadrons_pfakt7,"phiChargedHadrons_pfakt7[nJet_pfakt7]/F");
  m_tree->Branch("phiPhotons_pfakt7",       phiPhotons_pfakt7,       "phiPhotons_pfakt7[nJet_pfakt7]/F");
  m_tree->Branch("phiMuons_pfakt7",         phiMuons_pfakt7,         "phiMuons_pfakt7[nJet_pfakt7]/F");
  m_tree->Branch("phiElectrons_pfakt7",     phiElectrons_pfakt7,     "phiElectrons_pfakt7[nJet_pfakt7]/F");
  m_tree->Branch("phiNeutralHadrons_pfakt7",phiNeutralHadrons_pfakt7,"phiNeutralHadrons_pfakt7[nJet_pfakt7]/F");
  m_tree->Branch("phiHFHadrons_pfakt7",     phiHFHadrons_pfakt7,     "phiHFHadrons_pfakt7[nJet_pfakt7]/F");
  m_tree->Branch("phiHFEM_pfakt7",     phiHFEM_pfakt7,     "phiHFEM_pfakt7[nJet_pfakt7]/F");

  m_tree->Branch("etaChargedHadrons_pfakt7",etaChargedHadrons_pfakt7,"etaChargedHadrons_pfakt7[nJet_pfakt7]/F");
  m_tree->Branch("etaPhotons_pfakt7",       etaPhotons_pfakt7,       "etaPhotons_pfakt7[nJet_pfakt7]/F");
  m_tree->Branch("etaMuons_pfakt7",         etaMuons_pfakt7,         "etaMuons_pfakt7[nJet_pfakt7]/F");
  m_tree->Branch("etaElectrons_pfakt7",     etaElectrons_pfakt7,     "etaElectrons_pfakt7[nJet_pfakt7]/F");
  m_tree->Branch("etaNeutralHadrons_pfakt7",etaNeutralHadrons_pfakt7,"etaNeutralHadrons_pfakt7[nJet_pfakt7]/F");
  m_tree->Branch("etaHFHadrons_pfakt7",     etaHFHadrons_pfakt7,     "etaHFHadrons_pfakt7[nJet_pfakt7]/F");
  m_tree->Branch("etaHFEM_pfakt7",     etaHFEM_pfakt7,     "etaHFEM_pfakt7[nJet_pfakt7]/F");


  m_tree->Branch("nJet_pfsis5",&nJet_pfsis5,"nJet_pfsis5/I");
  m_tree->Branch("ptJet_pfsis5 ",&ptJet_pfsis5 ,"ptJet_pfsis5[nJet_pfsis5]/F");
  m_tree->Branch("eJet_pfsis5  ",&eJet_pfsis5  ,"eJet_pfsis5[nJet_pfsis5]/F");
  m_tree->Branch("etaJet_pfsis5",&etaJet_pfsis5,"etaJet_pfsis5[nJet_pfsis5]/F");
  m_tree->Branch("phiJet_pfsis5",&phiJet_pfsis5,"phiJet_pfsis5[nJet_pfsis5]/F");

  m_tree->Branch("nJet_pfkt6",&nJet_pfkt6,"nJet_pfkt6/I");
  m_tree->Branch("ptJet_pfkt6 ",&ptJet_pfkt6 ,"ptJet_pfkt6[nJet_pfkt6]/F");
  m_tree->Branch("eJet_pfkt6  ",&eJet_pfkt6  ,"eJet_pfkt6[nJet_pfkt6]/F");
  m_tree->Branch("etaJet_pfkt6",&etaJet_pfkt6,"etaJet_pfkt6[nJet_pfkt6]/F");
  m_tree->Branch("phiJet_pfkt6",&phiJet_pfkt6,"phiJet_pfkt6[nJet_pfkt6]/F");

  m_tree->Branch("nJet_pfsis7",&nJet_pfsis7,"nJet_pfsis7/I");
  m_tree->Branch("ptJet_pfsis7 ",&ptJet_pfsis7 ,"ptJet_pfsis7[nJet_pfsis7]/F");
  m_tree->Branch("eJet_pfsis7  ",&eJet_pfsis7  ,"eJet_pfsis7[nJet_pfsis7]/F");
  m_tree->Branch("etaJet_pfsis7",&etaJet_pfsis7,"etaJet_pfsis7[nJet_pfsis7]/F");
  m_tree->Branch("phiJet_pfsis7",&phiJet_pfsis7,"phiJet_pfsis7[nJet_pfsis7]/F");

  m_tree->Branch("nJetGen_ite",&nJetGen_ite,"nJetGen_ite/I");
  m_tree->Branch("ptJetGen_ite ",&ptJetGen_ite ,"ptJetGen_ite[nJetGen_ite]/F");
  m_tree->Branch("eJetGen_ite  ",&eJetGen_ite  ,"eJetGen_ite[nJetGen_ite]/F");
  m_tree->Branch("etaJetGen_ite",&etaJetGen_ite,"etaJetGen_ite[nJetGen_ite]/F");
  m_tree->Branch("phiJetGen_ite",&phiJetGen_ite,"phiJetGen_ite[nJetGen_ite]/F");

  m_tree->Branch("nJetGen_akt5",&nJetGen_akt5,"nJetGen_akt5/I");
  m_tree->Branch("ptJetGen_akt5 ",&ptJetGen_akt5,
		 "ptJetGen_akt5[nJetGen_akt5]/F");
  m_tree->Branch("eJetGen_akt5  ",&eJetGen_akt5,
		 "eJetGen_akt5[nJetGen_akt5]/F");
  m_tree->Branch("etaJetGen_akt5",&etaJetGen_akt5,
		 "etaJetGen_akt5[nJetGen_akt5]/F");
  m_tree->Branch("phiJetGen_akt5",&phiJetGen_akt5,
		 "phiJetGen_akt5[nJetGen_akt5]/F");

//   // Extra variables for PFlow studies
  m_tree->Branch("nMuonsGen_akt5", nMuonsGen_akt5,
		 "nMuonsGen_akt5[nJetGen_akt5]/I");
  m_tree->Branch("nElectronsGen_akt5", nElectronsGen_akt5,
		 "nElectronsGen_akt5[nJetGen_akt5]/I");
  m_tree->Branch("nPhotonsGen_akt5", nPhotonsGen_akt5,
		 "nPhotonsGen_akt5[nJetGen_akt5]/I");
  m_tree->Branch("nTracksGen_akt5", nTracksGen_akt5,
		 "nTracksGen_akt5[nJetGen_akt5]/I");
  m_tree->Branch("nNeutralHadronsGen_akt5", nNeutralHadronsGen_akt5,
		 "nNeutralHadronsGen_akt5[nJetGen_akt5]/I");
  m_tree->Branch("nHFHadronsGen_akt5", nHFHadronsGen_akt5,
		 "nHFHadronsGen_akt5[nJetGen_akt5]/I");
  m_tree->Branch("nHFEMGen_akt5", nHFEMGen_akt5,
		 "nHFEMGen_akt5[nJetGen_akt5]/I");

  m_tree->Branch("nNeutronsGen_akt5", nNeutronsGen_akt5,
		 "nNeutronsGen_akt5[nJetGen_akt5]/I");
  m_tree->Branch("nK0LGen_akt5", nK0LGen_akt5,
		 "nK0LGen_akt5[nJetGen_akt5]/I");
  m_tree->Branch("nK0SGen_akt5", nK0SGen_akt5,
		 "nK0SGen_akt5[nJetGen_akt5]/I");
  m_tree->Branch("nLambdasGen_akt5", nLambdasGen_akt5,
		 "nLambdasGen_akt5[nJetGen_akt5]/I");
  m_tree->Branch("nCsiGen_akt5", nCsiGen_akt5,
		 "nCsiGen_akt5[nJetGen_akt5]/I");
  m_tree->Branch("nOtherNeutralHadronsGen_akt5", nOtherNeutralHadronsGen_akt5,
		 "nOtherNeutralHadronsGen_akt5[nJetGen_akt5]/I");

  m_tree->Branch("eMuonsGen_akt5", eMuonsGen_akt5,
		 "eMuonsGen_akt5[nJetGen_akt5]/F");
  m_tree->Branch("eElectronsGen_akt5", eElectronsGen_akt5,
		 "eElectronsGen_akt5[nJetGen_akt5]/F");
  m_tree->Branch("ePhotonsGen_akt5", ePhotonsGen_akt5,
		 "ePhotonsGen_akt5[nJetGen_akt5]/F");
  m_tree->Branch("eTracksGen_akt5", eTracksGen_akt5,
		 "eTracksGen_akt5[nJetGen_akt5]/F");
  m_tree->Branch("eNeutralHadronsGen_akt5", eNeutralHadronsGen_akt5,
		 "eNeutralHadronsGen_akt5[nJetGen_akt5]/F");
  m_tree->Branch("eHFHadronsGen_akt5", eHFHadronsGen_akt5,
		 "eHFHadronsGen_akt5[nJetGen_akt5]/F");
  m_tree->Branch("eHFEMGen_akt5", eHFEMGen_akt5,
		 "eHFEMGen_akt5[nJetGen_akt5]/F");

  m_tree->Branch("eNeutronsGen_akt5", eNeutronsGen_akt5,
		 "eNeutronsGen_akt5[nJetGen_akt5]/F");
  m_tree->Branch("eK0LGen_akt5", eK0LGen_akt5,
		 "eK0LGen_akt5[nJetGen_akt5]/F");
  m_tree->Branch("eK0SGen_akt5", eK0SGen_akt5,
		 "eK0SGen_akt5[nJetGen_akt5]/F");
  m_tree->Branch("eLambdasGen_akt5", eLambdasGen_akt5,
		 "eLambdasGen_akt5[nJetGen_akt5]/F");
  m_tree->Branch("eCsiGen_akt5", eCsiGen_akt5,
		 "eCsiGen_akt5[nJetGen_akt5]/F");
  m_tree->Branch("eOtherNeutralHadronsGen_akt5", eOtherNeutralHadronsGen_akt5,
		 "eOtherNeutralHadronsGen_akt5[nJetGen_akt5]/F");

  m_tree->Branch("ptMuonsGen_akt5", ptMuonsGen_akt5,
		 "ptMuonsGen_akt5[nJetGen_akt5]/F");
  m_tree->Branch("ptElectronsGen_akt5", ptElectronsGen_akt5,
		 "ptElectronsGen_akt5[nJetGen_akt5]/F");
  m_tree->Branch("ptPhotonsGen_akt5", ptPhotonsGen_akt5,
		 "ptPhotonsGen_akt5[nJetGen_akt5]/F");
  m_tree->Branch("ptTracksGen_akt5", ptTracksGen_akt5,
		 "ptTracksGen_akt5[nJetGen_akt5]/F");
  m_tree->Branch("ptNeutralHadronsGen_akt5", ptNeutralHadronsGen_akt5,
		 "ptNeutralHadronsGen_akt5[nJetGen_akt5]/F");
  m_tree->Branch("ptHFHadronsGen_akt5", ptHFHadronsGen_akt5,
		 "ptHFHadronsGen_akt5[nJetGen_akt5]/F");
  m_tree->Branch("ptHFEMGen_akt5", ptHFEMGen_akt5,
		 "ptHFEMGen_akt5[nJetGen_akt5]/F");

  m_tree->Branch("phiMuonsGen_akt5", phiMuonsGen_akt5,
		 "phiMuonsGen_akt5[nJetGen_akt5]/F");
  m_tree->Branch("phiElectronsGen_akt5", phiElectronsGen_akt5,
		 "phiElectronsGen_akt5[nJetGen_akt5]/F");
  m_tree->Branch("phiPhotonsGen_akt5", phiPhotonsGen_akt5,
		 "phiPhotonsGen_akt5[nJetGen_akt5]/F");
  m_tree->Branch("phiTracksGen_akt5", phiTracksGen_akt5,
		 "phiTracksGen_akt5[nJetGen_akt5]/F");
  m_tree->Branch("phiNeutralHadronsGen_akt5", phiNeutralHadronsGen_akt5,
		 "phiNeutralHadronsGen_akt5[nJetGen_akt5]/F");
  m_tree->Branch("phiHFHadronsGen_akt5", phiHFHadronsGen_akt5,
		 "phiHFHadronsGen_akt5[nJetGen_akt5]/F");
  m_tree->Branch("phiHFEMGen_akt5", phiHFEMGen_akt5,
		 "phiHFEMGen_akt5[nJetGen_akt5]/F");

  m_tree->Branch("etaMuonsGen_akt5", etaMuonsGen_akt5,
		 "etaMuonsGen_akt5[nJetGen_akt5]/F");
  m_tree->Branch("etaElectronsGen_akt5", etaElectronsGen_akt5,
		 "etaElectronsGen_akt5[nJetGen_akt5]/F");
  m_tree->Branch("etaPhotonsGen_akt5", etaPhotonsGen_akt5,
		 "etaPhotonsGen_akt5[nJetGen_akt5]/F");
  m_tree->Branch("etaTracksGen_akt5", etaTracksGen_akt5,
		 "etaTracksGen_akt5[nJetGen_akt5]/F");
  m_tree->Branch("etaNeutralHadronsGen_akt5", etaNeutralHadronsGen_akt5,
		 "etaNeutralHadronsGen_akt5[nJetGen_akt5]/F");
  m_tree->Branch("etaHFHadronsGen_akt5", etaHFHadronsGen_akt5,
		 "etaHFHadronsGen_akt5[nJetGen_akt5]/F");
  m_tree->Branch("etaHFEMGen_akt5", etaHFEMGen_akt5,
		 "etaHFEMGen_akt5[nJetGen_akt5]/F");

  m_tree->Branch("nJetGen_akt7",&nJetGen_akt7,"nJetGen_akt7/I");
  m_tree->Branch("ptJetGen_akt7 ",&ptJetGen_akt7 ,"ptJetGen_akt7[nJetGen_akt7]/F");
  m_tree->Branch("eJetGen_akt7  ",&eJetGen_akt7  ,"eJetGen_akt7[nJetGen_akt7]/F");
  m_tree->Branch("etaJetGen_akt7",&etaJetGen_akt7,"etaJetGen_akt7[nJetGen_akt7]/F");
  m_tree->Branch("phiJetGen_akt7",&phiJetGen_akt7,"phiJetGen_akt7[nJetGen_akt7]/F");

  m_tree->Branch("nJetGen_kt4",&nJetGen_kt4,"nJetGen_kt4/I");
  m_tree->Branch("ptJetGen_kt4 ",&ptJetGen_kt4 ,"ptJetGen_kt4[nJetGen_kt4]/F");
  m_tree->Branch("eJetGen_kt4  ",&eJetGen_kt4  ,"eJetGen_kt4[nJetGen_kt4]/F");
  m_tree->Branch("etaJetGen_kt4",&etaJetGen_kt4,"etaJetGen_kt4[nJetGen_kt4]/F");
  m_tree->Branch("phiJetGen_kt4",&phiJetGen_kt4,"phiJetGen_kt4[nJetGen_kt4]/F");

  m_tree->Branch("nJetGen_kt6",&nJetGen_kt6,"nJetGen_kt6/I");
  m_tree->Branch("ptJetGen_kt6 ",&ptJetGen_kt6 ,"ptJetGen_kt6[nJetGen_kt6]/F");
  m_tree->Branch("eJetGen_kt6  ",&eJetGen_kt6  ,"eJetGen_kt6[nJetGen_kt6]/F");
  m_tree->Branch("etaJetGen_kt6",&etaJetGen_kt6,"etaJetGen_kt6[nJetGen_kt6]/F");
  m_tree->Branch("phiJetGen_kt6",&phiJetGen_kt6,"phiJetGen_kt6[nJetGen_kt6]/F");

  m_tree->Branch("nJetGen_sis5",&nJetGen_sis5,"nJetGen_sis5/I");
  m_tree->Branch("ptJetGen_sis5",&ptJetGen_sis5 ,"ptJetGen_sis5[nJetGen_sis5]/F");
  m_tree->Branch("eJetGen_sis5  ",&eJetGen_sis5  ,"eJetGen_sis5[nJetGen_sis5]/F");
  m_tree->Branch("etaJetGen_sis5",&etaJetGen_sis5,"etaJetGen_sis5[nJetGen_sis5]/F");
  m_tree->Branch("phiJetGen_sis5",&phiJetGen_sis5,"phiJetGen_sis5[nJetGen_sis5]/F");

  m_tree->Branch("nJetGen_sis7",&nJetGen_sis7,"nJetGen_sis7/I");
  m_tree->Branch("ptJetGen_sis7 ",&ptJetGen_sis7 ,"ptJetGen_sis7[nJetGen_sis7]/F");
  m_tree->Branch("eJetGen_sis7  ",&eJetGen_sis7  ,"eJetGen_sis7[nJetGen_sis7]/F");
  m_tree->Branch("etaJetGen_sis7",&etaJetGen_sis7,"etaJetGen_sis7[nJetGen_sis7]/F");
  m_tree->Branch("phiJetGen_sis7",&phiJetGen_sis7,"phiJetGen_sis7[nJetGen_sis7]/F");


  // pz,eta always zero, px,py redundant
  m_tree->Branch("sMet  ",&sMet  ,"sMet/F");
  m_tree->Branch("eMet  ",&eMet  ,"eMet/F");
  m_tree->Branch("phiMet",&phiMet,"phiMet/F");

  m_tree->Branch("stcMet  ",&stcMet  ,"stcMet/F");
  m_tree->Branch("etcMet  ",&etcMet  ,"etcMet/F");
  m_tree->Branch("phitcMet",&phitcMet,"phitcMet/F");

  m_tree->Branch("spfMet  ",&spfMet  ,"spfMet/F");
  m_tree->Branch("epfMet  ",&epfMet  ,"epfMet/F");
  m_tree->Branch("phipfMet",&phipfMet,"phipfMet/F");

  m_tree->Branch("sMetGen  ",&sMetGen  ,"sMetGen/F");
  m_tree->Branch("eMetGen  ",&eMetGen  ,"eMetGen/F");
  m_tree->Branch("phiMetGen",&phiMetGen,"phiMetGen/F");

  m_tree->Branch("sMetGen2  ",&sMetGen2  ,"sMetGen2/F");
  m_tree->Branch("eMetGen2  ",&eMetGen2  ,"eMetGen2/F");
  m_tree->Branch("phiMetGen2",&phiMetGen2,"phiMetGen2/F");

  //HLT_Photon10_L1R HLT_Photon10_LooseEcalIso_TrackIso_L1R HLT_Photon15_L1R HLT_Photon20_LooseEcalIso_TrackIso_L1R HLT_Photon25_L1R HLT_Photon25_LooseEcalIso_TrackIso_L1R HLT_Photon30_L1R_1E31 HLT_Photon70_L1R

  //vertex info
  m_tree->Branch("nvertex",&nvertex,"nvertex/I");

  m_tree->Branch("vxMC",&vxMC,"vxMC/F");
  m_tree->Branch("vyMC",&vyMC,"vyMC/F");
  m_tree->Branch("vzMC",&vzMC,"vzMC/F");

  m_tree->Branch("vx",&vx,"vx/F");
  m_tree->Branch("vy",&vy,"vy/F");
  m_tree->Branch("vz",&vz,"vz/F");
  m_tree->Branch("vntracks",&vntracks,"vntracks/F");
  m_tree->Branch("vchi2",&vchi2,"vchi2/F");
  m_tree->Branch("vndof",&vndof,"vndof/F");

  // Set trigger bits of interest
  nHLT = 8;
  hltTriggers["HLT_Photon10_L1R"] = 0;
  hltTriggers["HLT_Photon10_LooseEcalIso_TrackIso_L1R"] = 1;
  hltTriggers["HLT_Photon15_L1R"] = 2;
  hltTriggers["HLT_Photon20_LooseEcalIso_TrackIso_L1R"] = 3;
  hltTriggers["HLT_Photon25_L1R"] = 4;
  hltTriggers["HLT_Photon25_LooseEcalIso_TrackIso_L1R"] = 5;
  hltTriggers["HLT_Photon30_L1R_1E31"] = 6;
  hltTriggers["HLT_Photon70_L1R"] = 7;

  m_tree->Branch("hltPass",&hltPass,"hltPass/O");
  //m_tree->Branch("hltCount",&hltCount,"hltCount/I");
  m_tree->Branch("nHLT",&nHLT,"nHLT/I");
  m_tree->Branch("hltNamesLen",&hltNamesLen,"hltNamesLen/I");
  m_tree->Branch("HLTNames",&aHLTNames,"HLTNames[hltNamesLen]/C,6000");
  //m_tree->Branch("HLTResults",&aHLTResults,"HLTResults[hltCount]/O");
  m_tree->Branch("HLTResults",&aHLTResults,"HLTResults[nHLT]/O");

  event = 0;  
}

// ------------ method called once each job just after ending the event loop  ------------
void GammaJetAnalyzer::endJob() {

  
  outfile->cd("myanalysis");
  m_tree->Write();
  outfile->Close();
  //outfile->Delete();
  
//   //avoid writing the tree second time (automatically)
//  m_tree->Delete();

}

// Method for iterative printing of decay chains
bool GammaJetAnalyzer::printChildren(const SimTrack* p, 
		    map<const SimTrack*, set<const SimTrack*> > const& ptokids,
		    map<const SimTrack*, const SimVertex*> const& ptovtx,
		    int level, bool save) {

  // Print parent
  bool hasvtx = (ptovtx.find(p) != ptovtx.end());
  if (_debug) {
    for (int i = 0; i != 2*level; ++i) cout << " "; // pad with spaces
    cout << Form("* %d (%1.3g GeV, %1.3g",
		 p->type(), p->momentum().pt(),p->momentum().eta());
    if (hasvtx)
      cout << Form(" => r %1.3g cm, z %1.3g cm)",
		   ptovtx.find(p)->second->position().Rho(),
		   ptovtx.find(p)->second->position().z()) << endl;
    else
      cout << ")" << endl;
  }
  
  bool hasKids = (ptokids.find(p) != ptokids.end());

  // Save only SIM tracks not already in GenParticle list
  bool saved = false;
  if (save && level > 0 && nSIM < nMaxSIM) {
    pdgIdSIM[nSIM] = p->type();
    statusSIM[nSIM] = (hasKids ? 2 : 1);
    ptSIM[nSIM] = p->momentum().pt();
    eSIM[nSIM] = p->momentum().energy();
    etaSIM[nSIM] = p->momentum().eta();
    phiSIM[nSIM] = p->momentum().phi();
    if (hasvtx) {
      rSIM[nSIM] = ptovtx.find(p)->second->position().Rho();
      zSIM[nSIM] = ptovtx.find(p)->second->position().z();
    }
    else {
      rSIM[nSIM] = zSIM[nSIM] = 0.;
      //assert(hasvtx);
    }
    
    ++nSIM;
    saved = true;
  }

  // Print children, if any
  if (hasKids) {

    set<const SimTrack*> const& kids = ptokids.find(p)->second;
    for (set<const SimTrack*>::const_iterator iKid = kids.begin();
	 iKid != kids.end(); ++iKid)
      saved |= printChildren(*iKid, ptokids, ptovtx, level+1, save);
  } // if kids

  return saved;
 } // printChildren

// Go down in chain and remove unwanted decays
bool GammaJetAnalyzer::pruneKids(const SimTrack* p,
     map<const SimTrack*, set<const SimTrack*> > & decays,
     map<const SimTrack*, const SimTrack*> & parent,
     map<const SimTrack*, const SimVertex*> & vertex,
     int level) {

  // No children, go one level back
  if (decays.find(p)==decays.end()) return false;
  
  // Prune kids and see if there are any grandchildren left after pruning
  set<const SimTrack*> const& kids = decays.find(p)->second;
  bool hasGrandKids = false;
  bool hasSameType = false;
  unsigned int nPhotons = 0;
  unsigned int nElectrons = 0;
  for (set<const SimTrack*>::const_iterator iKid = kids.begin();
       iKid != kids.end(); ++iKid) {

    bool hasKids = pruneKids(*iKid, decays, parent, vertex, level+1);
    hasGrandKids = hasGrandKids || hasKids;
    hasSameType = hasSameType || (*iKid)->type()==p->type(); 
    if ((*iKid)->type()==22) ++nPhotons;
    if (abs((*iKid)->type())==11) ++nElectrons;
  }
  // if there are grandkids, don't prune kids as we need the whole chain
  if (hasGrandKids) return true;

  // See if we have some reason to prune the kids
  double pt  = p->momentum().pt();
  bool prune = (hasSameType && nPhotons==kids.size()-1) // bremsstrahlung
    || (nElectrons==kids.size() && level!=0) // non-primary photon conversion
    || (abs(p->type())==11 && nPhotons==kids.size()) // weird brem (no e)
    || (abs(p->type())==11 && nPhotons==kids.size()-nElectrons) // ionization
    || (p->type()==111 && pt<2. && nPhotons==kids.size()) // low pT pi0
    || (p->type()==22 && pt<2. && nElectrons==kids.size()); // low pT conv
  // || (kids.size()==1); // not a real decay?
  // (NB: electron brems can produce more than one photon)
  // (NG: electrons can turn into a photon with much less pT?)
  // (NB: photon conversions can produce only one electron)
  // (NB: pizeros can produce only one photon with most of pT)
  // (NB: pi+ decays seem to only produce a muon, no neutrinos) 

  // Prune, i.e. remove the parent decay and all the kids from maps
  if (prune) {

    for (set<const SimTrack*>::const_iterator iKid = kids.begin();
	 iKid != kids.end(); ++iKid) {
      parent.erase(*iKid);
      vertex.erase(*iKid);
    } // for kids
    decays.erase(p);

    return false;
  } // if prune
  else // no pruning done
    return true; 

} // pruneKids

//define this as a plug-in
DEFINE_FWK_MODULE(GammaJetAnalyzer);
