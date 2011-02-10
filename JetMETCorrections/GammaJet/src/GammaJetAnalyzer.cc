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
// $Id: GammaJetAnalyzer.cc,v 1.42 2011/01/09 18:56:16 pandolf Exp $
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
#include "DataFormats/VertexReco/interface/VertexFwd.h"
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

#include "SimDataFormats/Track/interface/SimTrack.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"
#include "SimDataFormats/Vertex/interface/SimVertex.h"
#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

#include "DataFormats/PatCandidates/interface/Photon.h"

#include "TLorentzVector.h"
#include "TRegexp.h"
#include "TString.h"

#include <set>
#include <algorithm>


using namespace edm;
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
    h1_hbherh_detid = new TH1F("hbherh_detid", "", 6, -0.5, 5.5);
    h1_etaPhot = new TH1F("etaPhot", "", 50, -5.5, 5.5);
    h2_n_vs_eta = new TH2D("n_vs_eta", "", 10, 0., 2.5, 1000, 0., 1000.);
  _debug = iConfig.getParameter<bool>("debug");
  MCTruthCollection_ = iConfig.getUntrackedParameter<edm::InputTag>("MCTruthCollection");
  triggerTag_ = iConfig.getUntrackedParameter<edm::InputTag>("TriggerTag");
  trackTags_ = iConfig.getUntrackedParameter<edm::InputTag>("tracks");
  Vertexsrc_ = iConfig.getUntrackedParameter<edm::InputTag>("vertices");
  Photonsrc_ = iConfig.getUntrackedParameter<edm::InputTag>("Photonsrc");
  Jetsrckt4_ = iConfig.getUntrackedParameter<edm::InputTag>("jetskt4");
  Jetsrckt6_ = iConfig.getUntrackedParameter<edm::InputTag>("jetskt6");
  Jetsrcakt5_ = iConfig.getUntrackedParameter<edm::InputTag>("jetsakt5");
  Jetsrcakt7_ = iConfig.getUntrackedParameter<edm::InputTag>("jetsakt7");
  JetJPTsrcak5_ = iConfig.getUntrackedParameter<edm::InputTag>("jetsjptak5");
  JetPFsrckt4_ = iConfig.getUntrackedParameter<edm::InputTag>("jetspfkt4");
  JetPFsrckt6_ = iConfig.getUntrackedParameter<edm::InputTag>("jetspfkt6");
  JetPFsrcakt5_ = iConfig.getUntrackedParameter<edm::InputTag>("jetspfakt5");
  JetPFsrcakt7_ = iConfig.getUntrackedParameter<edm::InputTag>("jetspfakt7");
  JetGensrckt4_ = iConfig.getUntrackedParameter<edm::InputTag>("jetsgenkt4");
  JetGensrckt6_ = iConfig.getUntrackedParameter<edm::InputTag>("jetsgenkt6");
  JetGensrcakt5_ = iConfig.getUntrackedParameter<edm::InputTag>("jetsgenakt5");
  JetGensrcakt7_ = iConfig.getUntrackedParameter<edm::InputTag>("jetsgenakt7");
  METsrc_ = iConfig.getUntrackedParameter<edm::InputTag>("met");
  METGensrc_ = iConfig.getUntrackedParameter<edm::InputTag>("genMet");
  HBhitsrc_ = iConfig.getUntrackedParameter<edm::InputTag>("hbhits");
  recoCollection_    = iConfig.getParameter<std::string>("recoCollection");
  recoProducer_      = iConfig.getParameter<std::string>("recoProducer");
  JetCorrector_akt5_ = iConfig.getParameter<std::string>("JetCorrectionService_akt5");
  JetCorrector_akt7_ = iConfig.getParameter<std::string>("JetCorrectionService_akt7");
  JetCorrector_jptak5_ = iConfig.getParameter<std::string>("JetCorrectionService_jptak5");
  JetCorrector_pfakt5_ = iConfig.getParameter<std::string>("JetCorrectionService_pfakt5");
  JetCorrector_pfakt7_ = iConfig.getParameter<std::string>("JetCorrectionService_pfakt7");
  genjetptthr_ = iConfig.getParameter<double>("genjetptthr");
  calojetptthr_ = iConfig.getParameter<double>("calojetptthr");
  pfjetptthr_ = iConfig.getParameter<double>("pfjetptthr");
  jptjetptthr_ = iConfig.getParameter<double>("jptjetptthr");
  genjetnmin_ = iConfig.getParameter<int>("genjetnmin");
  pfjetnmin_ = iConfig.getParameter<int>("pfjetnmin");
  jptjetnmin_ = iConfig.getParameter<int>("jptjetnmin");
  Xsec_ = iConfig.getParameter<double>("Xsec");
  jetID_ = new reco::helper::JetIDHelper(iConfig.getParameter<ParameterSet>("JetIDParams"));
  aHLTNames = new std::vector<std::string>;
  aHLTResults = new std::vector<bool>;
 
}


GammaJetAnalyzer::~GammaJetAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
   TFile* file_prova = TFile::Open("prova.root", "recreate");
   file_prova->cd();
   h1_hbherh_detid->Write();
   h1_etaPhot->Write();
   h2_n_vs_eta->Write();
   file_prova->Close();

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
GammaJetAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   nMC = nPhot = nJet_akt5 = nJet_kt4 = nJet_kt6 = nJet_pfkt4 = nJet_pfakt5 = nJet_pfkt6 = nJetGen_kt4 = nJetGen_akt5 = nJetGen_kt6 = 0;
   nJet_jptak5  = 0;
   nJet_pfakt7 = nJet_akt7 = nJetGen_akt7 = 0;
   nvertex = 0;

   aHLTNames->clear();
   aHLTResults->clear();

   using reco::TrackCollection;
  
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
   Handle<VertexCollection> VertexHandle;
   //Handle<vector<Vertex> > VertexHandle;
   iEvent.getByLabel("offlinePrimaryVertices", VertexHandle);
   //iEvent.getByLabel(Vertexsrc_, VertexHandle);
   
   // get photons
   Handle<PhotonCollection>  PhotonHandle;
   iEvent.getByLabel(Photonsrc_, PhotonHandle);

   // get PFCandidates
   Handle<PFCandidateCollection>  PFCandidates;
   iEvent.getByLabel("particleFlow", PFCandidates);

   // get calo jet collection
   Handle<CaloJetCollection> jetsakt5;
   iEvent.getByLabel(Jetsrcakt5_, jetsakt5);
   Handle<CaloJetCollection> jetsakt7;
   iEvent.getByLabel(Jetsrcakt7_, jetsakt7);
   Handle<CaloJetCollection> jetskt4;
   iEvent.getByLabel(Jetsrckt4_, jetskt4);
   Handle<CaloJetCollection> jetskt6;
   iEvent.getByLabel(Jetsrckt6_, jetskt6);

   // get JPT collection
   Handle<JPTJetCollection> jptjetsak5;
   iEvent.getByLabel(JetJPTsrcak5_, jptjetsak5);

   // get PF jets collection
   Handle<PFJetCollection> pfjetskt4;
   iEvent.getByLabel(JetPFsrckt4_, pfjetskt4);
   Handle<PFJetCollection> pfjetsakt5;
   iEvent.getByLabel(JetPFsrcakt5_, pfjetsakt5);
   Handle<PFJetCollection> pfjetsakt7;
   iEvent.getByLabel(JetPFsrcakt7_, pfjetsakt7);
   Handle<PFJetCollection> pfjetskt6;
   iEvent.getByLabel(JetPFsrckt6_, pfjetskt6);

   //get jet correctors
   const JetCorrector* corrector_akt5 = JetCorrector::getJetCorrector (JetCorrector_akt5_, iSetup);
   const JetCorrector* corrector_akt7 = JetCorrector::getJetCorrector (JetCorrector_akt7_, iSetup);
   const JetCorrector* corrector_jptak5 = JetCorrector::getJetCorrector (JetCorrector_jptak5_, iSetup);
   const JetCorrector* corrector_pfakt5 = JetCorrector::getJetCorrector (JetCorrector_pfakt5_, iSetup);
   const JetCorrector* corrector_pfakt7 = JetCorrector::getJetCorrector (JetCorrector_pfakt7_, iSetup);
 
   // get gen jet collection
   Handle<GenJetCollection> jetsgenkt4;
   if( isMC ) iEvent.getByLabel(JetGensrckt4_, jetsgenkt4);
   Handle<GenJetCollection> jetsgenkt6;
   if( isMC ) iEvent.getByLabel(JetGensrckt6_, jetsgenkt6);
   Handle<GenJetCollection> jetsgenakt5;
   if( isMC ) iEvent.getByLabel(JetGensrcakt5_, jetsgenakt5);
   Handle<GenJetCollection> jetsgenakt7;
   if( isMC ) iEvent.getByLabel(JetGensrcakt7_, jetsgenakt7);

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

   Handle<CaloMETCollection>  muJESCorrMEThandle;
   iEvent.getByLabel("metMuonJESCorAK5", muJESCorrMEThandle);

   Handle<CaloMETCollection>  caloMEThandleNoHF;
   iEvent.getByLabel("metNoHF", caloMEThandleNoHF);

   Handle<CaloMETCollection>  muCorrMEThandle;
   iEvent.getByLabel("corMetGlobalMuons", muCorrMEThandle);

  
   // get HCAL info
   Handle<HBHERecHitCollection> hbhe;
   iEvent.getByLabel(HBhitsrc_, hbhe);
   const HBHERecHitMetaCollection mhbhe(*hbhe);


   // get ECAL reco hits
   Handle<EBRecHitCollection> ecalhitseb;
   const EBRecHitCollection* rhitseb=0;
   iEvent.getByLabel(recoProducer_, recoCollection_, ecalhitseb);
   //const EcalRecHitMetaCollection mecalhits(*ecalhits);    
   rhitseb = ecalhitseb.product(); // get a ptr to the product

   Handle<EERecHitCollection> ecalhitsee;
   const EERecHitCollection* rhitsee=0;
   iEvent.getByLabel(recoProducer_, "EcalRecHitsEE", ecalhitsee);
   rhitsee = ecalhitsee.product(); // get a ptr to the product

   // get geometry
   edm::ESHandle<CaloGeometry> geoHandle;
   //   iSetup.get<IdealGeometryRecord>().get(geoHandle);
   iSetup.get<CaloGeometryRecord>().get(geoHandle);
   const CaloGeometry* geometry = geoHandle.product();

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

   hltPass = false;



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
//   HLTNames.init(*hltTriggerResultHandle);
     HLTNames = iEvent.triggerNames(*hltTriggerResultHandle);
     std::string tempnames;
     hltCount = hltTriggerResultHandle->size();
     //std::cout << "hltTriggerResult->size(): " << hltCount << std::endl;

      for (int i = 0 ; i != hltCount; ++i) {


        TString hltName_tstr(HLTNames.triggerName(i));
        std::string hltName_str(HLTNames.triggerName(i));
        TRegexp reg("Photon");

        if ( hltName_tstr.Contains(reg) ) {

          aHLTNames->push_back(hltName_str);
          aHLTResults->push_back(hltTriggerResultHandle->accept(i));


        }
     } // for i

     hltNamesLen = tempnames.length();

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
	 
	 nElectronsGen_akt5[nJetGen_akt5] = nElectronsGen;
	 p = &p4ElectronsGen;
	 eElectronsGen_akt5[nJetGen_akt5] = p->E() / it->energy();
	 
	 nPhotonsGen_akt5[nJetGen_akt5] = nPhotonsGen;
	 p = &p4PhotonsGen;
	 ePhotonsGen_akt5[nJetGen_akt5] = p->E() / it->energy();
	 
	 nTracksGen_akt5[nJetGen_akt5] = nTracksGen;
	 p = &p4TracksGen;
	 eTracksGen_akt5[nJetGen_akt5] = p->E() / it->energy();
	 
	 nNeutralHadronsGen_akt5[nJetGen_akt5] = nNeutralHadronsGen;
	 p = &p4NeutralHadronsGen;
	 eNeutralHadronsGen_akt5[nJetGen_akt5] = p->E() / it->energy();
	 
	 nHFHadronsGen_akt5[nJetGen_akt5] = nHFHadronsGen;
	 p = &p4HFHadronsGen;
	 eHFHadronsGen_akt5[nJetGen_akt5] = p->E() / it->energy();
	 
	 nHFEMGen_akt5[nJetGen_akt5] = nHFEMGen;
	 p = &p4HFEMGen;
	 eHFEMGen_akt5[nJetGen_akt5] = p->E() / it->energy();
	 
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
     
   } //if(isMC)
   
     // Get the primary vertex coordinates
   for (VertexCollection::const_iterator it = VertexHandle->begin(); 
	it != VertexHandle->end(); ++it) {
     
     vx[nvertex] = (it->isValid()) ? it->x() : 999.;
     vy[nvertex] = (it->isValid()) ? it->y() : 999.;
     vz[nvertex] = (it->isValid()) ? it->z() : 999.;
     
     vntracks[nvertex] = (it->isValid()) ? it->tracksSize() : 0;
     vchi2[nvertex] = (it->isValid()) ? it->normalizedChi2() : 100.;
     vndof[nvertex] = (it->isValid()) ? it->ndof() : 0.;
     
     nvertex++;
   }
   
   
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
     hasPixelSeedPhot[nPhot] = it->hasPixelSeed();
     
     const Ptr<CaloCluster> theSeed = it->superCluster()->seed(); 
     
     const EBRecHitCollection* rechits = ( fabs(it->eta())<1.479 ) ? rhitseb : rhitsee;
     
     // photon ID (spike ID) related info:
     // timing:
     std::pair<DetId, float> maxRH = EcalClusterTools::getMaximum( *theSeed, &(*rechits) );
     DetId seedCrystalId = maxRH.first;
     EcalRecHitCollection::const_iterator seedRH = rechits->find(seedCrystalId);
     timePhot[nPhot] = (float)seedRH->time();
     // swiss cross:
     e4SwissCrossPhot[nPhot] = (fabs(it->eta())>=1.479) ? 0. :
       ( EcalClusterTools::eLeft( *theSeed, &(*rechits), topology ) +
	 EcalClusterTools::eRight( *theSeed, &(*rechits), topology ) +
	 EcalClusterTools::eTop( *theSeed, &(*rechits), topology ) +
	 EcalClusterTools::eBottom( *theSeed, &(*rechits), topology ) );

     
     double ptiso0015(0.), ptiso035(0.), ptiso04(0.);
     int ntrkiso0015(0), ntrkiso035(0), ntrkiso04(0);

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
       
       if (deltaR < .015)  {ptiso0015  += sqrt(itTrack->innerMomentum().Perp2()); ntrkiso0015++; }
       if (deltaR < .35)   {ptiso035 += sqrt(itTrack->innerMomentum().Perp2()); ntrkiso035++;}
       if (deltaR < .4)   {ptiso04 += sqrt(itTrack->innerMomentum().Perp2()); ntrkiso04++;}
       
     }
     
     ptiso0015Phot[nPhot] = ptiso0015;
     ntrkiso0015Phot[nPhot] = ntrkiso0015;
     ptiso035Phot[nPhot] = ptiso035;
     ntrkiso035Phot[nPhot] = ntrkiso035;
     ptiso04Phot[nPhot] = ptiso04;
     ntrkiso04Phot[nPhot] = ntrkiso04;

     // calculate HCAL isolation
  
     double hcalEnergy = 0;
     reco::SuperClusterRef sc = it->get<reco::SuperClusterRef>();
     CaloConeSelector selector4(0.4, geometry, DetId::Hcal); 
     std::auto_ptr<CaloRecHitMetaCollectionV> selected = selector4.select(sc->eta(),sc->phi(),mhbhe); 
     for (CaloRecHitMetaCollectionV::const_iterator hit=selected->begin(); hit != selected->end(); ++hit) hcalEnergy += hit->energy(); 
     hcalovecal04Phot[nPhot] = hcalEnergy/it->energy(); 

   h1_etaPhot->Fill( sc->eta() );
   for( CaloRecHitMetaCollectionV::const_iterator ihbherh=selected->begin(); ihbherh!=selected->end(); ++ihbherh ) {
     h1_hbherh_detid->Fill( ihbherh->detid().subdetId() );
   }
     h2_n_vs_eta->Fill( fabs(sc->eta()) , selected->size() );

     CaloClusterPtr SCseed = it->superCluster()->seed();

     // calculate ECAL isolation 

     // ecal isolation with SC seed rechits removal
     SuperClusterHitsEcalIsolation scBasedIsolation(rhitseb,rhitsee);
     scBasedIsolation.setExtRadius(0.1);
     scBasedIsolation.excludeHalo(false);
     scBasedIsolation.setExtRadius(0.4);
     ecaliso04Phot[nPhot]  = scBasedIsolation.getSum(iEvent,iSetup,&(*SCseed));

     // cluster shape variables
     
     if (TMath::Abs(SCseed->eta())<1.47){
       Cluster2ndMoments moments = EcalClusterTools::cluster2ndMoments(*SCseed, *rhitseb);
       std::vector<float> etaphimoments = EcalClusterTools::localCovariances(*SCseed, &(*rhitseb), &(*topology));
       sMajMajPhot[nPhot]=moments.sMaj;
       sMinMinPhot[nPhot]=moments.sMin;
       alphaPhot[nPhot]=moments.alpha;
       sEtaEtaPhot[nPhot]=etaphimoments[0];
       sEtaPhiPhot[nPhot]=etaphimoments[1];
       sPhiPhiPhot[nPhot]=etaphimoments[2];
     }else{
       sMajMajPhot[nPhot]=-100.;
       sMinMinPhot[nPhot]=-100.;
       alphaPhot[nPhot]=-100.;
       sEtaEtaPhot[nPhot] = it->sigmaEtaEta();//-100.;
       sEtaPhiPhot[nPhot]=-100.;
       sPhiPhiPhot[nPhot]=-100.;
     }
     E1Phot[nPhot] = SCseed->energy();//-100.;
     E9Phot[nPhot] = it->e3x3();//-100.;
     E25Phot[nPhot] = it->e5x5();//-100.;

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
       
       jetID_->calculate(iEvent, *it);
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
       n90Jet_akt5[nJet_akt5] = jetID_->hitsInN90();	      
       n90HitsJet_akt5[nJet_akt5] = jetID_->n90Hits();	      
       fHPDJet_akt5[nJet_akt5] = jetID_->fHPD();	      
       fRBXJet_akt5[nJet_akt5] = jetID_->fRBX();	      
       
       nJet_akt5++;
     }
   }

   for (CaloJetCollection::const_iterator it = jetsakt7->begin(); 
	it != jetsakt7->end(); ++it) {
     
     if (nJet_akt7>=100) {cout << "number of reco jets akt 07 is larger than 100. Skipping" << endl; continue;}
     if (it->pt() > calojetptthr_) {
       jetID_->calculate(iEvent, *it);
       ptJet_akt7[nJet_akt7] = it->pt();	 
 
       // Jet Energy Scale Corrections on-the-fly     
       CaloJet  correctedJet = *it;
       double scale = corrector_akt7->correction(it->p4());
       correctedJet.scaleEnergy(scale);
       ptCorrJet_akt7[nJet_akt7] = correctedJet.pt();

       eJet_akt7[nJet_akt7] = it->energy();	 
       etaJet_akt7[nJet_akt7] = it->eta();	 
       phiJet_akt7[nJet_akt7] = it->phi();	      
       //emfJet_akt7[nJet_akt7] = it->emEnergyFraction();
       emfJet_akt7[nJet_akt7] = fixEMF(it->emEnergyFraction(), it->eta());     
       n90Jet_akt7[nJet_akt7] = jetID_->hitsInN90();	      
       n90HitsJet_akt7[nJet_akt7] = jetID_->n90Hits();	      
       fHPDJet_akt7[nJet_akt7] = jetID_->fHPD();	      
       fRBXJet_akt7[nJet_akt7] = jetID_->fRBX();	      
       
       nJet_akt7++;
     }
   }
   
   
   for (JPTJetCollection::const_iterator it = jptjetsak5->begin(); 
	it != jptjetsak5->end(); ++it) {
     
     if (nJet_jptak5>=100) {cout << "number of reco jets jptakt 05 is larger than 100. Skipping" << endl; continue;}
     if (nJet_jptak5 < jptjetnmin_ || it->pt() > jptjetptthr_) {
       
       // Jet Energy Scale Corrections on-the-fly     
       JPTJet  correctedJet = *it;
       double scale = corrector_jptak5->correction(it->p4());
       correctedJet.scaleEnergy(scale);
       ptCorrJet_jptak5[nJet_jptak5] = correctedJet.pt();

       ptJet_jptak5[nJet_jptak5] = it->pt();	 
       eJet_jptak5[nJet_jptak5] = it->energy();	 
       etaJet_jptak5[nJet_jptak5] = it->eta();	 
       phiJet_jptak5[nJet_jptak5] = it->phi();	      
       //emfJet_jptak5[nJet_jptak5] = fixEMF(it->emEnergyFraction(), it->eta());
       
       nJet_jptak5++;
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
       
       float sumPt_cands=0.;
       float sumPt2_cands=0.;
       float rms_cands=0.;
       
       for (vector<PFCandidatePtr>::const_iterator jt = pfCandidates.begin();
	    jt != pfCandidates.end(); ++jt) {
	 
	 PFCandidate::ParticleType id = (*jt)->particleId();
	 // Convert particle momentum to normal TLorentzVector, wrong type :(
	 math::XYZTLorentzVectorD const& p4t = (*jt)->p4();
	 TLorentzVector p4(p4t.px(), p4t.py(), p4t.pz(), p4t.energy());
	 TLorentzVector jetp4;
       jetp4.SetPtEtaPhiE(it->pt(), it->eta(), it->phi(), it->energy());
	 
       sumPt_cands += p4.Pt();
       sumPt2_cands += (p4.Pt()*p4.Pt());
       //float deltaR = it->p4().DeltaR(p4);
       float deltaR = jetp4.DeltaR(p4);
       rms_cands += (p4.Pt()*p4.Pt()*deltaR*deltaR);


	 // Store PFCandidates for two leading jets
	 
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


       ptDJet_pfakt5[nJet_pfakt5] = sqrt( sumPt2_cands )/sumPt_cands;
       rmsCandJet_pfakt5[nJet_pfakt5] = rms_cands/(sumPt_cands*sumPt_cands);

       
       const TLorentzVector *p = 0;
       
       nChargedHadrons_pfakt5[nJet_pfakt5] =  nChargedHadrons;
       p = &p4ChargedHadrons;
       eChargedHadrons_pfakt5[nJet_pfakt5] = p->E() / it->energy();
       
       nElectrons_pfakt5[nJet_pfakt5] =  nElectrons;
       p = &p4Electrons;
       eElectrons_pfakt5[nJet_pfakt5] = p->E() / it->energy();
       
       nMuons_pfakt5[nJet_pfakt5] =  nMuons;
       p = &p4Muons;
       eMuons_pfakt5[nJet_pfakt5] = p->E() / it->energy();
       
       nPhotons_pfakt5[nJet_pfakt5] =  nPhotons;
       p = &p4Photons;
       ePhotons_pfakt5[nJet_pfakt5] = p->E() / it->energy();
       
       nNeutralHadrons_pfakt5[nJet_pfakt5] =  nNeutralHadrons;
       p = &p4NeutralHadrons;
       eNeutralHadrons_pfakt5[nJet_pfakt5] = p->E() / it->energy();
       
       nHFHadrons_pfakt5[nJet_pfakt5] =  nHFHadrons;
       p = &p4HFHadrons;
       eHFHadrons_pfakt5[nJet_pfakt5] = p->E() / it->energy();
       
       nHFEM_pfakt5[nJet_pfakt5] =  nHFEM;
       p = &p4HFEM;
       eHFEM_pfakt5[nJet_pfakt5] = p->E() / it->energy();
       
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

       ++nJet_pfakt7;

     } 
   }

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
   
   // Fill caloMET
   
   const CaloMETCollection *calometcol = calomethandle.product();
   CaloMET const& calomet = calometcol->front();
   
   sMet = calomet.sumEt();
   eMet = calomet.pt();	 
   phiMet = calomet.phi();	      
   signifMet = calomet.mEtSig();	      
   
   // Fill muJEScaloMET
   
   const CaloMETCollection *muJEScalometcol = muJESCorrMEThandle.product();
   CaloMET const& muJEScalomet = muJEScalometcol->front();
   
   smuCorrMet = muJEScalomet.sumEt();
   emuCorrMet = muJEScalomet.pt();	 
   phimuCorrMet = muJEScalomet.phi();	      
   signifmuCorrMet = muJEScalomet.mEtSig();	      

 
   // Fill corrcaloMET

   const CaloMETCollection *mucalometcol = muCorrMEThandle.product();
   CaloMET const& mucalomet = mucalometcol->front();
   
   sCorrMet = mucalomet.sumEt();
   eCorrMet = mucalomet.pt();	 
   phiCorrMet = mucalomet.phi();	      
   signifCorrMet = mucalomet.mEtSig();	      

 
   // Fill caloMETNoHF

   const CaloMETCollection *calometcolNoHF = caloMEThandleNoHF.product();
   CaloMET const& calometNoHF = calometcolNoHF->front();
   
   sNoHFMet = calometNoHF.sumEt();
   eNoHFMet = calometNoHF.pt();	 
   phiNoHFMet = calometNoHF.phi();	      
   signifNoHFMet = calometNoHF.mEtSig();	      


   // Fill tcMET

   const View<MET> *tcmetcol = tcmethandle.product();
   MET const& tcmet = tcmetcol->front();
   
   stcMet = tcmet.sumEt();
   etcMet = tcmet.pt();
   phitcMet = tcmet.phi();
   signiftcMet = tcmet.mEtSig();	      
     
   // Fill pfMET

   const PFMETCollection *pfmetcol = pfmethandle.product();
   PFMET const& pfmet = pfmetcol->front();

   spfMet = pfmet.sumEt();
   epfMet = pfmet.pt();
   phipfMet = pfmet.phi();
   signifpfMet = pfmet.mEtSig();	      

   sMetGen = 0.;
   eMetGen = 0.;
   phiMetGen = 0.;
   signifMetGen = 0.;

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
     signifMetGen = genmet.mEtSig();

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

  m_tree->Branch("nPhot",&nPhot,"nPhot/I");
  m_tree->Branch("ptPhot ",&ptPhot ,"ptPhot[nPhot]/F");
  m_tree->Branch("ePhot  ",&ePhot  ,"ePhot[nPhot]/F");
  m_tree->Branch("escPhot  ",&escPhot  ,"escPhot[nPhot]/F");
  m_tree->Branch("eseedPhot  ",&eseedPhot  ,"eseedPhot[nPhot]/F");
  m_tree->Branch("etaPhot",&etaPhot,"etaPhot[nPhot]/F");
  m_tree->Branch("phiPhot",&phiPhot,"phiPhot[nPhot]/F");
  m_tree->Branch("timePhot",&timePhot,"timePhot[nPhot]/F");
  m_tree->Branch("e4SwissCrossPhot",&e4SwissCrossPhot,"e4SwissCrossPhot[nPhot]/F");
  m_tree->Branch("hasPixelSeedPhot",&hasPixelSeedPhot,"hasPixelSeedPhot[nPhot]/I");

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

  m_tree->Branch("hcalovecal04Phot",&hcalovecal04Phot,"hcalovecal04Phot[nPhot]/F"); 
  m_tree->Branch("ecaliso04Phot",&ecaliso04Phot,"ecaliso04Phot[nPhot]/F");  

  m_tree->Branch("sMajMajPhot",&sMajMajPhot,"sMajMaj2Phot[nPhot]/F");
  m_tree->Branch("sMinMinPhot",&sMinMinPhot,"sMinMin2Phot[nPhot]/F");
  m_tree->Branch("alphaPhot",&alphaPhot,"alphaPhot[nPhot]/F");
  m_tree->Branch("sEtaEtaPhot",&sEtaEtaPhot,"sEtaEtaPhot[nPhot]/F");
  m_tree->Branch("sEtaPhiPhot",&sEtaPhiPhot,"sEtaPhiPhot[nPhot]/F");
  m_tree->Branch("sPhiPhiPhot",&sPhiPhiPhot,"sPhiPhiPhot[nPhot]/F");
  m_tree->Branch("E1Phot",&E1Phot,"E1Phot[nPhot]/F");
  m_tree->Branch("E9Phot",&E9Phot,"E9Phot[nPhot]/F");
  m_tree->Branch("E25Phot",&E25Phot,"E25Phot[nPhot]/F");


  m_tree->Branch("nJet_akt5",&nJet_akt5,"nJet_akt5/I");
  m_tree->Branch("ptJet_akt5 ",&ptJet_akt5 ,"ptJet_akt5[nJet_akt5]/F");
  m_tree->Branch("ptCorrJet_akt5 ",&ptCorrJet_akt5 ,"ptCorrJet_akt5[nJet_akt5]/F");
  m_tree->Branch("eJet_akt5  ",&eJet_akt5  ,"eJet_akt5[nJet_akt5]/F");
  m_tree->Branch("etaJet_akt5",&etaJet_akt5,"etaJet_akt5[nJet_akt5]/F");
  m_tree->Branch("phiJet_akt5",&phiJet_akt5,"phiJet_akt5[nJet_akt5]/F");
  m_tree->Branch("emfJet_akt5",&emfJet_akt5,"emfJet_akt5[nJet_akt5]/F");
  m_tree->Branch("n90Jet_akt5",&n90Jet_akt5,"n90Jet_akt5[nJet_akt5]/F");
  m_tree->Branch("n90HitsJet_akt5",&n90HitsJet_akt5,"n90HitsJet_akt5[nJet_akt5]/F");
  m_tree->Branch("fHPDJet_akt5",&fHPDJet_akt5,"fHPDJet_akt5[nJet_akt5]/F");
  m_tree->Branch("fRBXJet_akt5",&fRBXJet_akt5,"fRBXJet_akt5[nJet_akt5]/F");

  m_tree->Branch("nJet_akt7",&nJet_akt7,"nJet_akt7/I");
  m_tree->Branch("ptJet_akt7 ",&ptJet_akt7 ,"ptJet_akt7[nJet_akt7]/F");
  m_tree->Branch("ptCorrJet_akt7 ",&ptCorrJet_akt7 ,"ptCorrJet_akt7[nJet_akt5]/F");
  m_tree->Branch("eJet_akt7  ",&eJet_akt7  ,"eJet_akt7[nJet_akt7]/F");
  m_tree->Branch("etaJet_akt7",&etaJet_akt7,"etaJet_akt7[nJet_akt7]/F");
  m_tree->Branch("phiJet_akt7",&phiJet_akt7,"phiJet_akt7[nJet_akt7]/F");
  m_tree->Branch("emfJet_akt7",&emfJet_akt7,"emfJet_akt7[nJet_akt7]/F");
  m_tree->Branch("n90Jet_akt7",&n90Jet_akt7,"n90Jet_akt7[nJet_akt7]/F");
  m_tree->Branch("n90HitsJet_akt7",&n90HitsJet_akt7,"n90HitsJet_akt7[nJet_akt7]/F");
  m_tree->Branch("fHPDJet_akt7",&fHPDJet_akt7,"fHPDJet_akt7[nJet_akt7]/F");
  m_tree->Branch("fRBXJet_akt7",&fRBXJet_akt7,"fRBXJet_akt7[nJet_akt7]/F");


  m_tree->Branch("nJet_jptak5",&nJet_jptak5,"nJet_jptak5/I");
  m_tree->Branch("ptJet_jptak5 ",&ptJet_jptak5 ,"ptJet_jptak5[nJet_jptak5]/F");
  m_tree->Branch("ptCorrJet_jptak5 ",&ptCorrJet_jptak5 ,"ptCorrJet_jptak5[nJet_jptak5]/F");
  m_tree->Branch("eJet_jptak5  ",&eJet_jptak5  ,"eJet_jptak5[nJet_jptak5]/F");
  m_tree->Branch("etaJet_jptak5",&etaJet_jptak5,"etaJet_jptak5[nJet_jptak5]/F");
  m_tree->Branch("phiJet_jptak5",&phiJet_jptak5,"phiJet_jptak5[nJet_jptak5]/F");
  m_tree->Branch("emfJet_jptak5",&emfJet_jptak5,"emfJet_jptak5[nJet_jptak5]/F");

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
  m_tree->Branch("ptDJet_pfakt5",&ptDJet_pfakt5,"ptDJet_pfakt5[nJet_pfakt5]/F");
  m_tree->Branch("rmsCandJet_pfakt5",&rmsCandJet_pfakt5,"rmsCandJet_pfakt5[nJet_pfakt5]/F");

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


  m_tree->Branch("nJet_pfakt7",&nJet_pfakt7,"nJet_pfakt7/I");
  m_tree->Branch("ptJet_pfakt7 ",&ptJet_pfakt7 ,"ptJet_pfakt7[nJet_pfakt7]/F");
  m_tree->Branch("ptCorrJet_pfakt7 ",&ptCorrJet_pfakt7 ,"ptCorrJet_pfakt7[nJet_pfakt7]/F");
  m_tree->Branch("eJet_pfakt7  ",&eJet_pfakt7  ,"eJet_pfakt7[nJet_pfakt7]/F");
  m_tree->Branch("etaJet_pfakt7",&etaJet_pfakt7,"etaJet_pfakt7[nJet_pfakt7]/F");
  m_tree->Branch("phiJet_pfakt7",&phiJet_pfakt7,"phiJet_pfakt7[nJet_pfakt7]/F");

  m_tree->Branch("nJet_pfkt6",&nJet_pfkt6,"nJet_pfkt6/I");
  m_tree->Branch("ptJet_pfkt6 ",&ptJet_pfkt6 ,"ptJet_pfkt6[nJet_pfkt6]/F");
  m_tree->Branch("eJet_pfkt6  ",&eJet_pfkt6  ,"eJet_pfkt6[nJet_pfkt6]/F");
  m_tree->Branch("etaJet_pfkt6",&etaJet_pfkt6,"etaJet_pfkt6[nJet_pfkt6]/F");
  m_tree->Branch("phiJet_pfkt6",&phiJet_pfkt6,"phiJet_pfkt6[nJet_pfkt6]/F");


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


  // pz,eta always zero, px,py redundant
  m_tree->Branch("sMet  ",&sMet  ,"sMet/F");
  m_tree->Branch("eMet  ",&eMet  ,"eMet/F");
  m_tree->Branch("phiMet",&phiMet,"phiMet/F");
  m_tree->Branch("signifMet",&signifMet,"signifMet/F");

  m_tree->Branch("sCorrMet  ",&sCorrMet  ,"sCorrMet/F");
  m_tree->Branch("eCorrMet  ",&eCorrMet  ,"eCorrMet/F");
  m_tree->Branch("phiCorrMet",&phiCorrMet,"phiCorrMet/F");
  m_tree->Branch("signifCorrMet",&signifCorrMet,"signifCorrMet/F");

  m_tree->Branch("smuCorrMet  ",&smuCorrMet  ,"smuCorrMet/F");
  m_tree->Branch("emuCorrMet  ",&emuCorrMet  ,"emuCorrMet/F");
  m_tree->Branch("phimuCorrMet",&phimuCorrMet,"phimuCorrMet/F");
  m_tree->Branch("signifmuCorrMet",&signifmuCorrMet,"signifmuCorrMet/F");

  m_tree->Branch("sNoHFMet  ",&sNoHFMet  ,"sNoHFMet/F");
  m_tree->Branch("eNoHFMet  ",&eNoHFMet  ,"eNoHFMet/F");
  m_tree->Branch("phiNoHFMet",&phiNoHFMet,"phiNoHFMet/F");
  m_tree->Branch("signifNoHFMet",&signifNoHFMet,"signifNoHFMet/F");

  m_tree->Branch("stcMet  ",&stcMet  ,"stcMet/F");
  m_tree->Branch("etcMet  ",&etcMet  ,"etcMet/F");
  m_tree->Branch("phitcMet",&phitcMet,"phitcMet/F");
  m_tree->Branch("signiftcMet",&signiftcMet,"signiftcMet/F");

  m_tree->Branch("spfMet  ",&spfMet  ,"spfMet/F");
  m_tree->Branch("epfMet  ",&epfMet  ,"epfMet/F");
  m_tree->Branch("phipfMet",&phipfMet,"phipfMet/F");
  m_tree->Branch("signifpfMet",&signifpfMet,"signifpfMet/F");

  m_tree->Branch("sMetGen  ",&sMetGen  ,"sMetGen/F");
  m_tree->Branch("eMetGen  ",&eMetGen  ,"eMetGen/F");
  m_tree->Branch("phiMetGen",&phiMetGen,"phiMetGen/F");
  m_tree->Branch("signifMetGen",&signifMetGen,"signifMetGen/F");

  m_tree->Branch("sMetGen2  ",&sMetGen2  ,"sMetGen2/F");
  m_tree->Branch("eMetGen2  ",&eMetGen2  ,"eMetGen2/F");
  m_tree->Branch("phiMetGen2",&phiMetGen2,"phiMetGen2/F");

  //vertex info
  m_tree->Branch("nvertex",&nvertex,"nvertex/I");

  m_tree->Branch("vxMC",&vxMC,"vxMC/F");
  m_tree->Branch("vyMC",&vyMC,"vyMC/F");
  m_tree->Branch("vzMC",&vzMC,"vzMC/F");

  m_tree->Branch("vx",&vx,"vx[nvertex]/F");
  m_tree->Branch("vy",&vy,"vy[nvertex]/F");
  m_tree->Branch("vz",&vz,"vz[nvertex]/F");
  m_tree->Branch("vntracks",&vntracks,"vntracks[nvertex]/F");
  m_tree->Branch("vchi2",&vchi2,"vchi2[nvertex]/F");
  m_tree->Branch("vndof",&vndof,"vndof[nvertex]/F");

  // Set trigger bits of interest
  nHLT = 13;
  hltTriggers["HLT_Photon10_L1R"] = 0;
  hltTriggers["HLT_Photon10_LooseEcalIso_TrackIso_L1R"] = 1;
  hltTriggers["HLT_Photon15_L1R"] = 2;
  hltTriggers["HLT_Photon20_L1R"] = 3;
  hltTriggers["HLT_Photon20_LooseEcalIso_TrackIso_L1R"] = 4;
  hltTriggers["HLT_Photon25_L1R"] = 5;
  hltTriggers["HLT_Photon25_LooseEcalIso_TrackIso_L1R"] = 6;
  hltTriggers["HLT_Photon30_L1R_1E31"] = 7;
  hltTriggers["HLT_Photon70_L1R"] = 8;

  hltTriggers["HLT_Photon10_Cleaned_L1R"] = 9;
  hltTriggers["HLT_Photon15_Cleaned_L1R"] = 10;
  hltTriggers["HLT_Photon20_Cleaned_L1R"] = 11;
  hltTriggers["HLT_Photon30_Cleaned_L1R"] = 12;


  //m_tree->Branch("hltPass",&hltPass,"hltPass/O");
  //m_tree->Branch("hltCount",&hltCount,"hltCount/I");
  m_tree->Branch("nHLT",&nHLT,"nHLT/I");
  m_tree->Branch("hltNamesLen",&hltNamesLen,"hltNamesLen/I");
  //m_tree->Branch("HLTNames",&aHLTNames,"HLTNames[hltNamesLen]/C,6000");
  m_tree->Branch("HLTNames",&aHLTNames);
  m_tree->Branch("HLTResults",&aHLTResults);
  //m_tree->Branch("HLTResults",&aHLTResults,"HLTResults[nHLT]/O");

  m_tree->Branch("Xsec",  &Xsec_, "Xsec/D");


  event = 0;  
}

// ------------ method called once each job just after ending the event loop  ------------
void GammaJetAnalyzer::endJob() {

  
  outfile->cd("myanalysis");
  m_tree->Write();
  outfile->Close();
  delete jetID_;

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
  if (save && level > 0) {
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
