// -*- C++ -*-
//
// Package:    PATBd2JpsiKs
// Class:      PATBd2JpsiKs
// 
/**\class PATBd2JpsiKs PATBd2JpsiKs.cc HeavyFlavorAnalysis/Examples/src/PATBd2JpsiKs.cc

 Description: <one line class summary>
Make rootTuple for b->s JPsi(mu+mu-) analyses

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Keith Ulmer
//         Created:  Wed May  7 13:15:04 MDT 2008
// $Id: PATBd2JpsiKs.cc,v 1.5 2009/12/18 22:19:34 kaulmer Exp $
//
//


// system include files
#include <memory>

// user include files
#include "HeavyFlavorAnalysis/Examples/interface/PATBd2JpsiKs.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "CommonTools/Statistics/interface/ChiSquaredProbability.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Framework/interface/TriggerNames.h"

#include "DataFormats/Candidate/interface/VertexCompositeCandidate.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"

#include "RecoVertex/KinematicFitPrimitives/interface/MultiTrackKinematicConstraint.h"
#include "RecoVertex/KinematicFit/interface/KinematicConstrainedVertexFitter.h"
#include "RecoVertex/KinematicFit/interface/TwoTrackMassKinematicConstraint.h"

#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "MagneticField/Engine/interface/MagneticField.h"

#include "DataFormats/Common/interface/RefToBase.h"
#include "DataFormats/Candidate/interface/ShallowCloneCandidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "DataFormats/Math/interface/Error.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/GenericParticle.h"

#include "DataFormats/CLHEP/interface/Migration.h"

#include "TFile.h"
#include "TTree.h"

#include <vector>
#include <utility>

//
// constants, enums and typedefs
//

  typedef math::Error<3>::type CovarianceMatrix;

//
// static data member definitions
//

//
// constructors and destructor
//
PATBd2JpsiKs::PATBd2JpsiKs(const edm::ParameterSet& iConfig)
  :
  hepMC(iConfig.getUntrackedParameter<std::string>("HepMC",std::string("source")) ),
  hlTriggerResults_(iConfig.getUntrackedParameter<edm::InputTag>("HLTriggerResults",edm::InputTag("TriggerResults::HLT")) ),
  //hlTriggerResults_(iConfig.getUntrackedParameter<edm::InputTag>("HLTriggerResults",edm::InputTag("TriggerResults::BJPSIXHLT")) ),
  blist(iConfig.getUntrackedParameter("Blist",std::string("Bd"))),
  muonType(iConfig.getUntrackedParameter("muonType",std::string("muons"))),
  vtxSample( iConfig.getUntrackedParameter<std::string>("VtxSample",std::string("offlinePrimaryVertices")) ), 
  genParticles_( iConfig.getUntrackedParameter<std::string>("genParticles_",std::string("genParticles")) ), 

  tree_(0), l1_mu3(0), l1_2mu3(0), l1_muOpen(0), l1_mu0(0),
  hlt_mu3(0), hlt_mu5(0), hlt_mu7(0), hlt_mu9(0), hlt_2mu0(0), hlt_2mu3(0), hlt_2mu3JPsi(0), hltBJPsiMuMu(0), nB(0),
  priVtxX(0), priVtxY(0), priVtxZ(0), priVtxXE(0), priVtxYE(0), priVtxZE(0), priVtxCL(0),
  bMass(0), bVtxCL(0), bPx(0), bPy(0), bPz(0), bPxE(0), bPyE(0), bPzE(0), 
  bctau(0), bctau2D(0), bctauBS(0), bctauMPV(0), bctauMPVBS(0), bctaunewBS(0), bctauMPVnewBS(0),
  bDecayVtxX(0), bDecayVtxY(0), bDecayVtxZ(0), bDecayVtxXE(0), bDecayVtxYE(0), bDecayVtxZE(0),
  bResMass(0), bResVtxCL(0), bResPx(0), bResPy(0), bResPz(0),
  bResDecayVtxX(0), bResDecayVtxY(0), bResDecayVtxZ(0), bResDecayVtxXE(0), bResDecayVtxYE(0), bResDecayVtxZE(0),
  VMass(0), VVtxCL(0), VPx(0), VPy(0), VPz(0),
  VDecayVtxX(0), VDecayVtxY(0), VDecayVtxZ(0),
  VDecayVtxXE(0), VDecayVtxYE(0), VDecayVtxZE(0),
  JMass(0), JVtxCL(0), JPx(0), JPy(0), JPz(0),
  JDecayVtxX(0), JDecayVtxY(0), JDecayVtxZ(0), JDecayVtxXE(0), JDecayVtxYE(0), JDecayVtxZE(0),
  mumPx(0), mumPy(0), mumPz(0), mumD0(0), mumD0E(0),
  mupPx(0), mupPy(0), mupPz(0), mupD0(0), mupD0E(0),
  VTrkpMass(0), VTrkpPx(0), VTrkpPy(0), VTrkpPz(0), 
  VTrkpD0(0), VTrkpD0E(0), 
  VTrkmMass(0), VTrkmPx(0), VTrkmPy(0), VTrkmPz(0), 
  VTrkmD0(0), VTrkmD0E(0), 
  bResTrkPx(0), bResTrkPy(0), bResTrkPz(0), 
  bResTrkD0(0), bResTrkD0E(0),bResTrkChg(0), 
  genKsPsi(0), genKstarpPsi(0), genLambdaPsi(0), feedup(0), feeddown(0),

  bMass2(0), bMass3(0), bMass4(0), bMass5(0), bPx2(0), bPx3(0), bPx4(0), bPx5(0),   bPy2(0), bPy3(0), bPy4(0), bPy5(0), 
  bPz2(0), bPz3(0), bPz4(0), bPz5(0), 
  bDx2(0), bDx3(0), bDx4(0), bDx5(0), bDy2(0), bDy3(0), bDy4(0), bDy5(0), bDz2(0), bDz3(0), bDz4(0), bDz5(0),

  truePriVtxX(0), truePriVtxY(0), truePriVtxZ(0), trueBPx(0), trueBPy(0), trueBPz(0), trueBDecayVtxX(0), trueBDecayVtxY(0), trueBDecayVtxZ(0),
  trueBResPx(0), trueBResPy(0), trueBResPz(0), trueBResDecayVtxX(0), trueBResDecayVtxY(0), trueBResDecayVtxZ(0),
  trueVPx(0), trueVPy(0), trueVPz(0), trueVDecayVtxX(0), trueVDecayVtxY(0), trueVDecayVtxZ(0),
  trueJPx(0), trueJPy(0), trueJPz(0), trueJDecayVtxX(0), trueJDecayVtxY(0), trueJDecayVtxZ(0),
  trueMumPx(0), trueMumPy(0), trueMumPz(0), trueMumD0(0), trueMupPx(0), trueMupPy(0), trueMupPz(0), trueMupD0(0),
  trueVTrkpPx(0), trueVTrkpPy(0), trueVTrkpPz(0), trueVTrkpD0(0),
  trueVTrkmPx(0), trueVTrkmPy(0), trueVTrkmPz(0), trueVTrkmD0(0),
  trueBResTrkPx(0), trueBResTrkPy(0), trueBResTrkPz(0), trueBResTrkD0(0), trueBResTrkChg(0),
  truthMatch(0), truthKs(0), truthPsi(0), truthMatchPAT(0), truthKsPAT(0), truthPsiPAT(0), prompt(0)

{
   //now do what ever initialization is needed
}


PATBd2JpsiKs::~PATBd2JpsiKs()
{
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  
}


//
// member functions
//

// ------------ method called to for each event  ------------
void


PATBd2JpsiKs::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using std::vector;
  using namespace edm;
  using namespace reco;
  using namespace std;

   // get event content information

   ESHandle<MagneticField> bFieldHandle;
   iSetup.get<IdealMagneticFieldRecord>().get(bFieldHandle);

   // first get HLT results
   edm::Handle<edm::TriggerResults> hltresults;
   try {
     iEvent.getByLabel(hlTriggerResults_,hltresults);
   }
   catch ( ... ) {
     cout << "Couldn't get handle on HLT Trigger!" << endl;
   }
   if (!hltresults.isValid()) {
     cout << "No Trigger Results!" << endl;
   } 
   else {
     int ntrigs=hltresults->size();
     if (ntrigs==0){
       cout << "No trigger name given in TriggerResults of the input " << endl;
     } 
     
     // get hold of trigger names - based on TriggerResults object!
     edm::TriggerNames triggerNames_;
     triggerNames_.init(*hltresults); 
     
     for (int itrig=0; itrig< ntrigs; itrig++) {
       TString trigName = triggerNames_.triggerName(itrig);
       int hltflag = (*hltresults)[itrig].accept();
       //int wasrun  = (*hltresults)[itrig].wasrun();
       //cout << "Trigger " <<  trigName << " was run = " <<  wasrun << "  and was passed = " <<  hltflag << endl;
       if (trigName=="HLT_DoubleMu3_BJPsi") hltBJPsiMuMu = hltflag;
       if (trigName=="HLT_Mu3") hlt_mu3 = hltflag;
       if (trigName=="HLT_Mu5") hlt_mu5 = hltflag;
       if (trigName=="HLT_Mu7") hlt_mu7 = hltflag;
       if (trigName=="HLT_Mu9") hlt_mu9 = hltflag;  
       if (trigName=="HLT_DoubleMu0") hlt_2mu0 = hltflag;
       if (trigName=="HLT_DoubleMu3") hlt_2mu3 = hltflag;
       if (trigName=="HLT_DoubleMu3_JPsi") hlt_2mu3JPsi = hltflag;
     }
   }


   // get L1 trigger info
   
   edm::ESHandle<L1GtTriggerMenu> menuRcd;
   iSetup.get<L1GtTriggerMenuRcd>().get(menuRcd) ;
   const L1GtTriggerMenu* menu = menuRcd.product();
   
   edm::Handle< L1GlobalTriggerReadoutRecord > gtRecord;
   iEvent.getByLabel( edm::InputTag("gtDigis"), gtRecord);
   const DecisionWord dWord = gtRecord->decisionWord();  
   
   if ( menu->gtAlgorithmResult( "L1_SingleMu3", dWord) )  l1_mu3 = 1;
   if ( menu->gtAlgorithmResult( "L1_DoubleMu3", dWord) )  l1_2mu3 = 1;
   if ( menu->gtAlgorithmResult( "L1_SingleMuOpen", dWord) )  l1_muOpen = 1;
   if ( menu->gtAlgorithmResult( "L1_SingleMu0", dWord) )  l1_mu0 = 1;


   // get primary vertex
   Handle<reco::VertexCollection> recVtxs;
   iEvent.getByLabel(vtxSample, recVtxs);
   unsigned int nVtxTrks = 0;
   reco::VertexCollection::const_iterator bestVtx = recVtxs->begin();
   for (reco::VertexCollection::const_iterator vtx = recVtxs->begin();
	vtx != recVtxs->end(); ++vtx){
     if (nVtxTrks < vtx->tracksSize() ) {
       nVtxTrks = vtx->tracksSize();
       bestVtx = vtx;
     }
   }

   //get primary with beamspot constraint
   Handle<reco::VertexCollection> recVtxsBS;
   iEvent.getByLabel("offlinePrimaryVerticesWithBS", recVtxsBS);
   
   nVtxTrks = 0;
   reco::VertexCollection::const_iterator bestVtxBS = recVtxsBS->begin();
   for (reco::VertexCollection::const_iterator vtxBS = recVtxsBS->begin();
	vtxBS != recVtxsBS->end(); ++vtxBS){
     if (nVtxTrks < vtxBS->tracksSize() ) {
       nVtxTrks = vtxBS->tracksSize();
       bestVtxBS = vtxBS;
     }
   }
   
   //get beamspot
   reco::BeamSpot beamSpot;
   edm::Handle<reco::BeamSpot> beamSpotHandle;
   iEvent.getByLabel("offlineBeamSpot", beamSpotHandle);
   if ( beamSpotHandle.isValid() ) beamSpot = *beamSpotHandle; 
   else cout << "No beam spot available from EventSetup" << endl;
   
   //  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   // try reconstruction without fitting modules
   //  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   
   Handle<vector<VertexCompositeCandidate> > theV0Handle;
   iEvent.getByLabel("generalV0Candidates", "Kshort", theV0Handle);

   Handle< vector<pat::GenericParticle> > thePATTrackHandle;
   iEvent.getByLabel("cleanLayer1TrackCands", thePATTrackHandle);

   Handle< vector<pat::Muon> > thePATMuonHandle;
   iEvent.getByLabel("cleanLayer1Muons", thePATMuonHandle);

   cout << "event has " << theV0Handle->size() << "Ks and " << thePATMuonHandle->size() << "muons." << endl; 
   float pi = 3.14159265;

   if ( theV0Handle->size()>0 && thePATMuonHandle->size()>=2 ) {
     for ( vector<VertexCompositeCandidate>::const_iterator iVee = theV0Handle->begin();
	   iVee != theV0Handle->end(); ++iVee ) {
       //get Ks tracks from V0 candidate
       vector<RecoChargedCandidate> v0daughters;
       vector<TrackRef> theDaughterTracks;
       v0daughters.push_back( *(dynamic_cast<const reco::RecoChargedCandidate *>
       			(iVee->daughter(0))) );
       v0daughters.push_back( *(dynamic_cast<const reco::RecoChargedCandidate *>
       			(iVee->daughter(1))) );

       for(unsigned int j = 0; j < v0daughters.size(); ++j) {
        theDaughterTracks.push_back(v0daughters[j].track());
       }

       pat::GenericParticle patTrack1;
       pat::GenericParticle patTrack2;
       //lets loop through the pat tracks to find the match for this reco::track
       for ( vector<pat::GenericParticle>::const_iterator iTrack = thePATTrackHandle->begin();
	     iTrack != thePATTrackHandle->end(); ++iTrack ) {
	 
	 // how to access the reco::Track object
	 TrackRef hadTrack = iTrack->track();
	 if ( hadTrack.isNull() ) {
	   cout << "continue due to no track ref" << endl;
	   continue;
	 }
	 if ( hadTrack==theDaughterTracks[0] ){
	   patTrack1 = *iTrack;
	   cout << "Found PAT1 match " << endl; 
	 }
	 if ( hadTrack==theDaughterTracks[1] ){
	   patTrack2 = *iTrack;
	   cout << "Found PAT2 match " << endl; 
	 }
	 
       }

       for ( std::vector<pat::Muon>::const_iterator iMuonP = thePATMuonHandle->begin();
	     iMuonP != thePATMuonHandle->end(); ++iMuonP ) {
	 //check for mu+ first
	 if (iMuonP->charge() == 1) {
	   const pat::Muon *patMuonP = &(*iMuonP);
	   cout << "looping through muP list. muon has charge = " << iMuonP->charge() << endl;
	   cout << "for muP: isGlobalMuon = " << iMuonP->isGlobalMuon() << "  isTrackerMuon = " << iMuonP->isTrackerMuon() << "  isStandAloneMuon = " << iMuonP->isStandAloneMuon() << "  isCaloMuon = " << iMuonP->isCaloMuon() << endl;
	   TrackRef glbTrackP = iMuonP->track();
	   //cout << "got track" << endl;
	   if ( glbTrackP.isNull() ) {
	     cout << "continue due to no track ref" << endl;
	     continue;
	   }
	   bool match = false;


	   /////////////////////////
	   //try PAT overlap check//
	   /////////////////////////

	     const reco::CandidatePtrVector & mu1P_overlaps = patTrack1.overlaps("muons");
	     if ( mu1P_overlaps.size() > 0 ) cout << "patTrack1 overlaps with a muon." << endl;
	     for (size_t i = 0; i < mu1P_overlaps.size(); ++i) {
	       const pat::Muon *mu = dynamic_cast<const pat::Muon *>(&*mu1P_overlaps[i]);
	       if (mu) {
		 // check here that muon match isn't the same as a muon used in the reco...
		 if (mu==patMuonP) cout << "match between patTrack1 and patMuonP" << endl;
	       }
	     }

	     const reco::CandidatePtrVector & mu2P_overlaps = patTrack2.overlaps("muons");
	     if ( mu2P_overlaps.size() > 0 ) cout << "patTrack2 overlaps with a muon." << endl;
	     for (size_t i = 0; i < mu2P_overlaps.size(); ++i) {
	       const pat::Muon *mu = dynamic_cast<const pat::Muon *>(&*mu2P_overlaps[i]);
	       if (mu) {
		 // check here that muon match isn't the same as a muon used in the reco...
		 if (mu==patMuonP) cout << "match between patTrack2 and patMuonP" << endl;
	       }
	     }
	     
	     ////////////////////////////


	   for(unsigned int j = 0; j < theDaughterTracks.size(); ++j) {
	     if (glbTrackP->charge() == theDaughterTracks[j]->charge() && 
		 glbTrackP->momentum() == theDaughterTracks[j]->momentum() ) {
	       //std::cout << "Match found between muP and V0 track early with p = " << glbTrackP->momentum() << " and " << theDaughterTracks[j]->momentum() << endl;
	       match = true;
	     }
	     if (match) break;
	   }
	   if (match) { 
	     std::cout << "Match found between muP and V0 track" << endl;
	     match = false;
	     continue; 
	   } // Track is already used in making the V0

	   //next check for mu-
	   for ( std::vector<pat::Muon>::const_iterator iMuonM = thePATMuonHandle->begin();
		 iMuonM != thePATMuonHandle->end(); ++iMuonM ) {
	     if (iMuonM->charge() == -1) {
	       const pat::Muon *patMuonM = &(*iMuonM);
	       cout << "looping through muM list. muon has charge = " << iMuonM->charge() << endl;
	       cout << "for muM: isGlobalMuon = " << iMuonM->isGlobalMuon() << "  isTrackerMuon = " << iMuonM->isTrackerMuon() << "  isStandAloneMuon = " << iMuonM->isStandAloneMuon() << "  isCaloMuon = " << iMuonM->isCaloMuon() << endl;
	       TrackRef glbTrackM = iMuonM->track();
	       if ( glbTrackM.isNull() ) {
		 cout << "continue from no track ref" << endl;
		 continue;
	       }


	       /////////////////////////
		 //try PAT overlap check//	   
		 /////////////////////////
		 
		 const reco::CandidatePtrVector & mu1M_overlaps = patTrack1.overlaps("muons");
		 if ( mu1M_overlaps.size() > 0 ) cout << "patTrack1 overlaps with a muonM." << endl;
		 for (size_t i = 0; i < mu1M_overlaps.size(); ++i) {
		   const pat::Muon *mu = dynamic_cast<const pat::Muon *>(&*mu1M_overlaps[i]);
		   if (mu) {
		     // check here that muon match isn't the same as a muon used in the reco...
		     if (mu==patMuonM) cout << "match between patTrack1 and patMuonM" << endl;
		   }
		 }
		 
		 const reco::CandidatePtrVector & mu2M_overlaps = patTrack2.overlaps("muons");
		 if ( mu2M_overlaps.size() > 0 ) cout << "patTrack2 overlaps with a muon." << endl;
		 for (size_t i = 0; i < mu2M_overlaps.size(); ++i) {
		   const pat::Muon *mu = dynamic_cast<const pat::Muon *>(&*mu2M_overlaps[i]);
		   if (mu) {
		     // check here that muon match isn't the same as a muon used in the reco...
		     if (mu==patMuonM) cout << "match between patTrack2 and patMuonM" << endl;
		   }
		 }
		 
		 ////////////////////////////
		 

	       //check that neither muon is the same track as one of the pion tracks
	       for(unsigned int j = 0; j < theDaughterTracks.size(); ++j) {
		 if (glbTrackM->charge() == theDaughterTracks[j]->charge() && 
		     glbTrackM->momentum() == theDaughterTracks[j]->momentum() ) {
		   //std::cout << "Match found between muM and V0 track early with p = " << glbTrackM->momentum() << " and " << theDaughterTracks[j]->momentum() << endl;
		   match = true;
		 }
		 if (match) break;
	       }
	       if (match) { 
		 std::cout << "Match found between muM and V0 track" << endl;;
		 match = false;
		 continue; 
	       } // Track is already used in making the V0
	       //have two good oppositely charged muons and 2 pions. try to vertex them
	       cout << "have 4 good tracks including good oppositely charged muons. " << endl;
	       
	       TransientTrack pion1TT(theDaughterTracks[0], &(*bFieldHandle) );
	       TransientTrack pion2TT(theDaughterTracks[1], &(*bFieldHandle) );
	       TransientTrack muon1TT(glbTrackP, &(*bFieldHandle) );
	       TransientTrack muon2TT(glbTrackM, &(*bFieldHandle) );





	       //Creating a KinematicParticleFactory
	       KinematicParticleFactoryFromTransientTrack pFactory;
	       
	       //The mass of a muon and the insignificant mass sigma 
	       //to avoid singularities in the covariance matrix.
	       ParticleMass muon_mass = 0.10565837; //pdg mass
	       ParticleMass pion_mass = 0.13957018;
	       ParticleMass ks_mass = 0.497614;
	       ParticleMass psi_mass = 3.096916;
	       float muon_sigma = muon_mass*1.e-6;
	       float pion_sigma = pion_mass*1.e-6;
	       float ks_sigma = 0.000024;
	       
	       //initial chi2 and ndf before kinematic fits.
	       float chi = 0.;
	       float ndf = 0.;
	       vector<RefCountedKinematicParticle> pionParticles;
	       vector<RefCountedKinematicParticle> muonParticles;
	       pionParticles.push_back(pFactory.particle(pion1TT,pion_mass,chi,ndf,pion_sigma));
	       pionParticles.push_back(pFactory.particle(pion2TT,pion_mass,chi,ndf,pion_sigma));
	       muonParticles.push_back(pFactory.particle(muon1TT,muon_mass,chi,ndf,muon_sigma));
	       muonParticles.push_back(pFactory.particle(muon2TT,muon_mass,chi,ndf,muon_sigma));



	       KinematicParticleVertexFitter fitter;   
	       RefCountedKinematicTree ksVertexFitTree;
	       ksVertexFitTree = fitter.fit(pionParticles); 
	       if (!ksVertexFitTree->isValid()) {
		 std::cout << "invalid vertex from the ks vertex fit" << std::endl;
		 continue; 
	       }
	       ksVertexFitTree->movePointerToTheTop();
	       
	       RefCountedKinematicParticle ks_vFit_noMC = ksVertexFitTree->currentParticle();
	       RefCountedKinematicVertex ks_vFit_vertex_noMC = ksVertexFitTree->currentDecayVertex();

	       if ( ks_vFit_vertex_noMC->chiSquared() < 0 ) cout << "negative chisq from ks fit" << endl;	       
	       ksVertexFitTree->movePointerToTheFirstChild();
	       RefCountedKinematicParticle ksPi1 = ksVertexFitTree->currentParticle();
	       ksVertexFitTree->movePointerToTheNextChild();
	       RefCountedKinematicParticle ksPi2 = ksVertexFitTree->currentParticle();

	       KinematicParameters ksPi1KP = ksPi1->currentState().kinematicParameters();
	       KinematicParameters ksPi2KP = ksPi2->currentState().kinematicParameters();
	       KinematicParameters ksPipKP;
	       KinematicParameters ksPimKP;

	       //this ksPi1KP momentum is defined at the ks fit vertex.

	       if ( ksPi1->currentState().particleCharge() > 0 ) ksPipKP = ksPi1KP;
	       if ( ksPi1->currentState().particleCharge() < 0 ) ksPimKP = ksPi1KP;
	       if ( ksPi2->currentState().particleCharge() > 0 ) ksPipKP = ksPi2KP;
	       if ( ksPi2->currentState().particleCharge() < 0 ) ksPimKP = ksPi2KP;

	       //(x,y,z,p_x,p_y,p_z,m)

	       RefCountedKinematicTree psiVertexFitTree;
	       psiVertexFitTree = fitter.fit(muonParticles); 
	       if (!psiVertexFitTree->isValid()) {
		 std::cout << "caught an exception in the psi vertex fit" << std::endl;
		 continue; 
	       }
	       psiVertexFitTree->movePointerToTheTop();
	       
	       RefCountedKinematicParticle psi_vFit_noMC = psiVertexFitTree->currentParticle();
	       RefCountedKinematicVertex psi_vFit_vertex_noMC = psiVertexFitTree->currentDecayVertex();

	       if ( psi_vFit_vertex_noMC->chiSquared() < 0 ) cout << "negative chisq from psi fit" << endl;


	       // Do MC for Ks cand and do mass constrained vertex fit
	       // creating the constraint with a small sigma to put in the resulting covariance 
	       // matrix in order to avoid singularities
	       // JPsi mass constraint is applied in the final B fit

	       KinematicParticleFitter csFitterKs;
	       KinematicConstraint * ks_c = new MassKinematicConstraint(ks_mass,ks_sigma);
	       // add mass constraint to the ks fit to do a constrained fit:  
 
	       ksVertexFitTree->movePointerToTheTop();
	       ksVertexFitTree = csFitterKs.fit(ks_c,ksVertexFitTree);
	       if (!ksVertexFitTree->isValid()){
		 std::cout << "caught an exception in the ks mass constraint fit" << std::endl;
		 continue; 
	       }
	       
	       ksVertexFitTree->movePointerToTheTop();
	       RefCountedKinematicParticle ks_vFit_withMC = ksVertexFitTree->currentParticle();

	       vector<RefCountedKinematicParticle> vFitMCParticles;
	       vFitMCParticles.push_back(pFactory.particle(muon1TT,muon_mass,chi,ndf,muon_sigma));
	       vFitMCParticles.push_back(pFactory.particle(muon2TT,muon_mass,chi,ndf,muon_sigma));
	       vFitMCParticles.push_back(ks_vFit_withMC);

	       MultiTrackKinematicConstraint *  j_psi_c = new  TwoTrackMassKinematicConstraint(psi_mass);
	       KinematicConstrainedVertexFitter kcvFitter;
	       RefCountedKinematicTree vertexFitTree = kcvFitter.fit(vFitMCParticles, j_psi_c);
	       if (!vertexFitTree->isValid()) {
		 std::cout << "caught an exception in the B vertex fit with MC" << std::endl;
		 continue;
	       }

	       vertexFitTree->movePointerToTheTop();
	       RefCountedKinematicParticle bCandMC = vertexFitTree->currentParticle();
	       RefCountedKinematicVertex bDecayVertexMC = vertexFitTree->currentDecayVertex();
	       if (!bDecayVertexMC->vertexIsValid()){
		 cout << "B MC fit vertex is not valid" << endl;
		 continue;
	       }
	       
	       if ( bDecayVertexMC->chiSquared()<0 ) {
		 cout << " negative chi2 in B fit" << endl;
		 continue;
	       }
	       
	       if ( bDecayVertexMC->chiSquared()>1000 ) continue;
	       
	       if ( bCandMC->currentState().mass() > 10 ) continue;

	       // get children from final B fit
	       vertexFitTree->movePointerToTheFirstChild();
	       RefCountedKinematicParticle mu1CandMC = vertexFitTree->currentParticle();
	       cout << "mass of mu1 = " << mu1CandMC->currentState().mass() << " and charge = " << mu1CandMC->currentState().particleCharge() << endl;
	       vertexFitTree->movePointerToTheNextChild();
	       RefCountedKinematicParticle mu2CandMC = vertexFitTree->currentParticle();
	       cout << "mass of mu2 = " << mu2CandMC->currentState().mass() << " and charge = " << mu2CandMC->currentState().particleCharge() << endl;
	       vertexFitTree->movePointerToTheNextChild();
	       RefCountedKinematicParticle ksCandMC = vertexFitTree->currentParticle();
	       cout << "mass of ks = " << ksCandMC->currentState().mass() << " and charge = " << ksCandMC->currentState().particleCharge() << endl;

	       // get mu+ and mu- momenta from final B fit
	       KinematicParameters psiMu1KP = mu1CandMC->currentState().kinematicParameters();
	       KinematicParameters psiMu2KP = mu2CandMC->currentState().kinematicParameters();
	       KinematicParameters psiMupKP;
	       KinematicParameters psiMumKP;
	       
	       if ( mu1CandMC->currentState().particleCharge() > 0 ) psiMupKP = psiMu1KP;
	       if ( mu1CandMC->currentState().particleCharge() < 0 ) psiMumKP = psiMu1KP;
	       if ( mu2CandMC->currentState().particleCharge() > 0 ) psiMupKP = psiMu2KP;
	       if ( mu2CandMC->currentState().particleCharge() < 0 ) psiMumKP = psiMu2KP;	 

	       KinematicParameters VCandKP = ksCandMC->currentState().kinematicParameters();
	       
	       // fill candidate variables now
	       bMass->push_back(bCandMC->currentState().mass());
	       bPx->push_back(bCandMC->currentState().globalMomentum().x());
	       bPy->push_back(bCandMC->currentState().globalMomentum().y());
	       bPz->push_back(bCandMC->currentState().globalMomentum().z());
	       
	       bPxE->push_back( sqrt( bCandMC->currentState().kinematicParametersError().matrix()(3,3) ) );
	       bPyE->push_back( sqrt( bCandMC->currentState().kinematicParametersError().matrix()(4,4) ) );
	       bPzE->push_back( sqrt( bCandMC->currentState().kinematicParametersError().matrix()(5,5) ) );

	       //ks_vFit_noMC->currentState().kinematicParametersError().matrix()(3,3);   //7x7 matrix: (x,y,z,p_x,p_y,p_z,m)
	       // the index starts at 0, so the position values are 0,1,2 the momentum values are 3,4,5 and the mass is 6
	       
	       bVtxCL->push_back( ChiSquaredProbability((double)(bDecayVertexMC->chiSquared()),(double)(bDecayVertexMC->degreesOfFreedom())) );
	       bDecayVtxX->push_back((*bDecayVertexMC).position().x());
	       bDecayVtxY->push_back((*bDecayVertexMC).position().y());
	       bDecayVtxZ->push_back((*bDecayVertexMC).position().z());
	       bDecayVtxXE->push_back(sqrt( abs(bDecayVertexMC->error().cxx()) ));
	       bDecayVtxYE->push_back(sqrt( abs(bDecayVertexMC->error().cyy()) ));
	       bDecayVtxZE->push_back(sqrt( abs(bDecayVertexMC->error().czz()) ));

	       VMass->push_back( ks_vFit_noMC->currentState().mass() );   
	       JMass->push_back( psi_vFit_noMC->currentState().mass() ); 
	       
	       JDecayVtxX->push_back( psi_vFit_vertex_noMC->position().x() );
	       JDecayVtxY->push_back( psi_vFit_vertex_noMC->position().y() );
	       JDecayVtxZ->push_back( psi_vFit_vertex_noMC->position().z() );
	       
	       JDecayVtxXE->push_back( sqrt(psi_vFit_vertex_noMC->error().cxx()) );
	       JDecayVtxYE->push_back( sqrt(psi_vFit_vertex_noMC->error().cyy()) );
	       JDecayVtxZE->push_back( sqrt(psi_vFit_vertex_noMC->error().czz()) );
	       JVtxCL->push_back( ChiSquaredProbability((double)(psi_vFit_vertex_noMC->chiSquared()),(double)(psi_vFit_vertex_noMC->degreesOfFreedom())) );

	       JPx->push_back( psiMumKP.momentum().x() + psiMupKP.momentum().x() );
	       JPy->push_back( psiMumKP.momentum().y() + psiMupKP.momentum().y() );
	       JPz->push_back( psiMumKP.momentum().z() + psiMupKP.momentum().z() );

	       mumPx->push_back(psiMumKP.momentum().x());
	       mumPy->push_back(psiMumKP.momentum().y());
	       mumPz->push_back(psiMumKP.momentum().z());
	       mumD0->push_back( glbTrackM->d0() );
	       mumD0E->push_back(glbTrackM->d0Error() );
	       mupPx->push_back(psiMupKP.momentum().x());
	       mupPy->push_back(psiMupKP.momentum().y());
	       mupPz->push_back(psiMupKP.momentum().z());
	       mupD0->push_back( glbTrackP->d0() );
	       mupD0E->push_back(glbTrackP->d0Error() );

	       VDecayVtxX->push_back( ks_vFit_vertex_noMC->position().x() );
	       VDecayVtxY->push_back( ks_vFit_vertex_noMC->position().y() );
	       VDecayVtxZ->push_back( ks_vFit_vertex_noMC->position().z() );
	       
	       VDecayVtxXE->push_back( sqrt(ks_vFit_vertex_noMC->error().cxx()) );
	       VDecayVtxYE->push_back( sqrt(ks_vFit_vertex_noMC->error().cyy()) );
	       VDecayVtxZE->push_back( sqrt(ks_vFit_vertex_noMC->error().czz()) );
	       VVtxCL->push_back( ChiSquaredProbability((double)(ks_vFit_vertex_noMC->chiSquared()),(double)(ks_vFit_vertex_noMC->degreesOfFreedom())) );

	       VPx->push_back( VCandKP.momentum().x() );
	       VPy->push_back( VCandKP.momentum().y() );
	       VPz->push_back( VCandKP.momentum().z() );	       

	       VTrkpPx->push_back(ksPipKP.momentum().x());
	       VTrkpPy->push_back(ksPipKP.momentum().y());
	       VTrkpPz->push_back(ksPipKP.momentum().z());
	       VTrkpMass->push_back(ksPipKP.mass());
	       if ( theDaughterTracks[0]->charge() > 0 ) {
		 VTrkpD0->push_back( theDaughterTracks[0]->d0() );
		 VTrkpD0E->push_back( theDaughterTracks[0]->d0Error() ); 
	       } else {
		 VTrkpD0->push_back( theDaughterTracks[1]->d0() );
		 VTrkpD0E->push_back( theDaughterTracks[1]->d0Error() ); 
	       }
	       VTrkmPx->push_back(ksPimKP.momentum().x());
	       VTrkmPy->push_back(ksPimKP.momentum().y());
	       VTrkmPz->push_back(ksPimKP.momentum().z());
	       VTrkmMass->push_back(ksPimKP.mass());
	       if ( theDaughterTracks[0]->charge() < 0 ) {
		 VTrkmD0->push_back( theDaughterTracks[0]->d0() );
		 VTrkmD0E->push_back( theDaughterTracks[0]->d0Error() ); 
	       } else {
		 VTrkmD0->push_back( theDaughterTracks[1]->d0() );
		 VTrkmD0E->push_back( theDaughterTracks[1]->d0Error() ); 
	       }

	       //calculate ctau and ctau2D with ctau = (mB*(Bvtx-Pvtx)*pB)/|pB|**2

	       float mb = 5.27953;	

	       //bestVtx is the best primary vertex
	       //bestVtxBS is the best primary with beamspot constraint
	       
	       float bctau_temp = (mb*((bDecayVertexMC->position().x()-bestVtx->x())*bCandMC->currentState().globalMomentum().x() +
				       (bDecayVertexMC->position().y()-bestVtx->y())*bCandMC->currentState().globalMomentum().y() +
				       (bDecayVertexMC->position().z()-bestVtx->z())*bCandMC->currentState().globalMomentum().z()   ))/
		 (bCandMC->currentState().globalMomentum()*bCandMC->currentState().globalMomentum());
	       
	       cout << "ctau = " << bctau_temp << endl;
	       
	       bctau->push_back(bctau_temp);
	       
	       float bctau2D_temp = (mb* ( (bDecayVertexMC->position().x()-bestVtx->x())*bCandMC->currentState().globalMomentum().x() +
					   (bDecayVertexMC->position().y()-bestVtx->y())*bCandMC->currentState().globalMomentum().y() ))/
		 (bCandMC->currentState().globalMomentum().x()*bCandMC->currentState().globalMomentum().x()+
		  bCandMC->currentState().globalMomentum().y()*bCandMC->currentState().globalMomentum().y());
	       
	       bctau2D->push_back(bctau2D_temp);

	       cout << "ctau2D = " << bctau2D_temp << endl;

	       float bctauBS_temp = (mb*((bDecayVertexMC->position().x()-bestVtxBS->x())*bCandMC->currentState().globalMomentum().x() +
				       (bDecayVertexMC->position().y()-bestVtxBS->y())*bCandMC->currentState().globalMomentum().y() +
				       (bDecayVertexMC->position().z()-bestVtxBS->z())*bCandMC->currentState().globalMomentum().z()   ))/
		 (bCandMC->currentState().globalMomentum()*bCandMC->currentState().globalMomentum());
	       	     	       
	       bctauBS->push_back(bctauBS_temp);
	       
	       //calculate most probable decay length as 

	       AlgebraicMatrix31 pB;
	       pB(0,0) = bCandMC->currentState().globalMomentum().x();
	       pB(1,0) = bCandMC->currentState().globalMomentum().y();
	       pB(2,0) = bCandMC->currentState().globalMomentum().z();

	       AlgebraicMatrix13 pBT;
	       pBT(0,0) = bCandMC->currentState().globalMomentum().x();
	       pBT(0,1) = bCandMC->currentState().globalMomentum().y();
	       pBT(0,2) = bCandMC->currentState().globalMomentum().z();

	       AlgebraicMatrix31 PV;
	       PV(0,0) = bestVtx->position().x();
	       PV(0,1) = bestVtx->position().y();
	       PV(0,2) = bestVtx->position().z();
	       AlgebraicMatrix31 BV;
	       BV(0,0) = bDecayVertexMC->position().x();
	       BV(0,1) = bDecayVertexMC->position().y();
	       BV(0,2) = bDecayVertexMC->position().z();
	       AlgebraicMatrix31 lxyz = BV-PV;
	       AlgebraicMatrix33 PVError(bestVtx->error());
	       AlgebraicMatrix33 BVError(bDecayVertexMC->error().matrix_new());
	       AlgebraicMatrix33 lxyzError = PVError + BVError;
	       lxyzError.Invert();

	       float bctauMPV_temp;
	       AlgebraicMatrix11 a = pBT * lxyzError * pB ;
	       AlgebraicMatrix11 b = pBT * lxyzError * lxyz;
	       double num(b(0,0));
	       double deno(a(0,0));
	       bctauMPV_temp = (num*bCandMC->currentState().mass())/(deno);
	       bctauMPV->push_back(bctauMPV_temp);


	       // calculate MPV with beamspot constrained primary vertex
	       AlgebraicMatrix31 PVBS;
	       PVBS(0,0) = bestVtxBS->position().x();
	       PVBS(0,1) = bestVtxBS->position().y();
	       PVBS(0,2) = bestVtxBS->position().z();
	       AlgebraicMatrix31 lxyzBS = BV-PVBS;
	       AlgebraicMatrix33 PVBSError(bestVtxBS->error());
	       AlgebraicMatrix33 lxyzBSError = PVBSError + BVError;
	       lxyzBSError.Invert();

	       float bctauMPVBS_temp;
	       AlgebraicMatrix11 aBS = pBT * lxyzBSError * pB ;
	       AlgebraicMatrix11 bBS = pBT * lxyzBSError * lxyzBS;
	       double numBS(bBS(0,0));
	       double denoBS(aBS(0,0));
	       bctauMPVBS_temp = (numBS*bCandMC->currentState().mass())/(denoBS);
	       bctauMPVBS->push_back(bctauMPVBS_temp);

	       cout << "bctauMPVBS = " << bctauMPVBS_temp << endl;


	       // try refitting the primary without the tracks in the B reco candidate
	       
	       // first get tracks from the original primary
	       vector<reco::TransientTrack> vertexTracks;
	       
	       for ( std::vector<TrackBaseRef >::const_iterator iTrack = bestVtxBS->tracks_begin();
		     iTrack != bestVtxBS->tracks_end(); ++iTrack) {
		 // compare primary tracks to check for matches with B cand
		 TrackRef trackRef = iTrack->castTo<TrackRef>();

		 // the 4 tracks in the B cand are theDaughterTracks[0] theDaughterTracks[1] glbTrackP glbTrackM
		 if ( (theDaughterTracks[0]==trackRef) || (theDaughterTracks[1]==trackRef) ||
		      (glbTrackP==trackRef) || (glbTrackM==trackRef) )
		   cout << "found track match with primary" << endl;
		 else {
		   TransientTrack tt(trackRef, &(*bFieldHandle) );
		   vertexTracks.push_back(tt);
		 }
	       }
	       
	       cout << "number of tracks in primary = " << bestVtxBS->tracksSize() << endl;
	       cout << "number of tracks from vertex tt vector = " << vertexTracks.size() << endl;
	       
	       // if no tracks in primary or no reco track included in primary then don't do anything
	       
	       if (  vertexTracks.size()>0 && (bestVtxBS->tracksSize()!=vertexTracks.size()) ) {
		 
		 AdaptiveVertexFitter theFitter;
		 TransientVertex v = theFitter.vertex(vertexTracks, beamSpot);
		 if ( v.isValid() ) {
		   //calculate ctau with the new vertex to compare to the old one.
		   //1. standard 3D calculation
		   float bctaunewBS_temp = (mb*((bDecayVertexMC->position().x()-v.position().x())*bCandMC->currentState().globalMomentum().x() +
						(bDecayVertexMC->position().y()-v.position().y())*bCandMC->currentState().globalMomentum().y() +
						(bDecayVertexMC->position().z()-v.position().z())*bCandMC->currentState().globalMomentum().z()   ))/
		     (bCandMC->currentState().globalMomentum()*bCandMC->currentState().globalMomentum());
		   bctaunewBS->push_back(bctaunewBS_temp);
		   cout << "bctaunewBS = " << bctaunewBS_temp << endl;
		   
		   //2. 3D MPV calculation
		   AlgebraicMatrix31 PVnewBS;
		   PVnewBS(0,0) = v.position().x();
		   PVnewBS(0,1) = v.position().y();
		   PVnewBS(0,2) = v.position().z();
		   AlgebraicMatrix31 lxyznewBS = BV-PVnewBS;
		   reco::Vertex recoV = (reco::Vertex)v;
		   AlgebraicMatrix33 PVnewBSError(recoV.error());
		   AlgebraicMatrix33 lxyznewBSError = PVnewBSError + BVError;
		   lxyznewBSError.Invert();
		   
		   float bctauMPVnewBS_temp;
		   AlgebraicMatrix11 anewBS = pBT * lxyznewBSError * pB ;
		   AlgebraicMatrix11 bnewBS = pBT * lxyznewBSError * lxyznewBS;
		   double numnewBS(bnewBS(0,0));
		   double denonewBS(anewBS(0,0));
		   bctauMPVnewBS_temp = (numnewBS*bCandMC->currentState().mass())/(denonewBS);
		   bctauMPVnewBS->push_back(bctauMPVnewBS_temp);
		   
		   cout << "bctauMPVnewBS = " << bctauMPVnewBS_temp << endl;
		 } else {
		   bctaunewBS->push_back( bctauBS_temp );
		   bctauMPVnewBS->push_back( bctauMPVBS_temp );
		 }
	       } else {
		 bctaunewBS->push_back( bctauBS_temp );
		 bctauMPVnewBS->push_back( bctauMPVBS_temp );
	       }
	       
	       nB++;
	       
	       /////////////////////////////////////////////////
		 //Try some new fits here
		 
		 
		 
		 
		 // Fit 2: same fit without either the Phi or ks mass constraints
		 KinematicParticleVertexFitter fitter2;   
		 RefCountedKinematicTree ksVertexFitTree2;
		 ksVertexFitTree2 = fitter2.fit(pionParticles); 
		 if (!ksVertexFitTree2->isValid()) { continue; }
		 ksVertexFitTree2->movePointerToTheTop();
		 RefCountedKinematicParticle ks_KP = ksVertexFitTree2->currentParticle();
		 
		 cout << "From mass5 fit, ks_KP (before constraint) momentum x = " << ks_KP->currentState().globalMomentum().x() << endl;
		 
		 RefCountedKinematicTree psiVertexFitTree2;
		 psiVertexFitTree2 = fitter2.fit(muonParticles); 
		 if (!psiVertexFitTree2->isValid()) { continue; }
		 psiVertexFitTree2->movePointerToTheTop();
		 RefCountedKinematicParticle psi_KP = psiVertexFitTree2->currentParticle();
		 
		 // problem not above here
		 
		 KinematicParticleVertexFitter fitter3;   
		 vector<RefCountedKinematicParticle> vFit_KPs;
		 vFit_KPs.push_back(ks_KP);
		 //vFit_KPs.push_back(psi_KP);
		 vFit_KPs.push_back(psi_KP);
		 RefCountedKinematicTree vertexFitTree2;
		 vertexFitTree2 = fitter3.fit(vFit_KPs);  
		 if (!vertexFitTree2->isValid()) continue;
		 vertexFitTree2->movePointerToTheTop();
		 RefCountedKinematicParticle bCand_KP = vertexFitTree2->currentParticle();
		 
		 cout << " vertex for B of no MC fit = " << bCand_KP->currentState().globalPosition() << " with momentum = " << bCand_KP->currentState().globalMomentum() << " and mass = " << bCand_KP->currentState().mass() << endl;
		 
		 bMass2->push_back(bCand_KP->currentState().mass());
		 bPx2->push_back(bCand_KP->currentState().globalMomentum().x());
		 bPy2->push_back(bCand_KP->currentState().globalMomentum().y());
		 bPz2->push_back(bCand_KP->currentState().globalMomentum().z());
		 bDx2->push_back(bCand_KP->currentState().globalPosition().x());
		 bDy2->push_back(bCand_KP->currentState().globalPosition().y());
		 bDz2->push_back(bCand_KP->currentState().globalPosition().z());
		 
		 //	          problem not below here
		 
		 // Fit 3: No JPsi fit, just mu,mu,ks (again with no mass constraint on the ks)
		 vector<RefCountedKinematicParticle> vFit_KPs3;
		 vFit_KPs3.push_back(pFactory.particle(muon1TT,muon_mass,chi,ndf,muon_sigma));
		 vFit_KPs3.push_back(pFactory.particle(muon2TT,muon_mass,chi,ndf,muon_sigma));
		 vFit_KPs3.push_back(ks_KP);
		 
		 KinematicParticleVertexFitter fitter4;   	       
		 
		 RefCountedKinematicTree vertexFitTree3;
		 vertexFitTree3 = fitter4.fit(vFit_KPs3);  
		 if (!vertexFitTree3->isValid()) continue;
		 vertexFitTree3->movePointerToTheTop();
		 RefCountedKinematicParticle bCand_KP3 = vertexFitTree3->currentParticle();
		 
		 cout << " vertex for B of no JPsi, no MC fit = " << bCand_KP3->currentState().globalPosition() << " with momentum = " << bCand_KP3->currentState().globalMomentum() << " and mass = " << bCand_KP3->currentState().mass() << endl;
		 
		 bMass3->push_back(bCand_KP3->currentState().mass());
		 bPx3->push_back(bCand_KP3->currentState().globalMomentum().x());
		 bPy3->push_back(bCand_KP3->currentState().globalMomentum().y());
		 bPz3->push_back(bCand_KP3->currentState().globalMomentum().z());
		 bDx3->push_back(bCand_KP3->currentState().globalPosition().x());
		 bDy3->push_back(bCand_KP3->currentState().globalPosition().y());
		 bDz3->push_back(bCand_KP3->currentState().globalPosition().z());
		 
		 
		 //////////////////////////////////////////////////////
		 // Fit 4: No JPsi fit, just mu,mu,ks fit, plus JPsi mass constraint on dimuon (still no ks mass constraint)
		 
		 MultiTrackKinematicConstraint *  j_psi_c4 = new  TwoTrackMassKinematicConstraint(psi_mass);
		 KinematicConstrainedVertexFitter kcvFitter4;
		 RefCountedKinematicTree myTree = kcvFitter4.fit(vFit_KPs3, j_psi_c4);
		 myTree->movePointerToTheTop();
		 RefCountedKinematicParticle bCand_KP4 = myTree->currentParticle();
		 RefCountedKinematicVertex b_dec_vertex = myTree->currentDecayVertex();
		 if(!b_dec_vertex->vertexIsValid()) continue;
		 cout << "vertex for B single fit = " << bCand_KP4->currentState().globalPosition() << " with momenutum = " << bCand_KP4->currentState().globalMomentum() << " and mass = " << bCand_KP4->currentState().mass() << endl;
		 
		 bMass4->push_back(bCand_KP4->currentState().mass());
		 bPx4->push_back(bCand_KP4->currentState().globalMomentum().x());
		 bPy4->push_back(bCand_KP4->currentState().globalMomentum().y());
		 bPz4->push_back(bCand_KP4->currentState().globalMomentum().z());
		 bDx4->push_back(bCand_KP4->currentState().globalPosition().x());
		 bDy4->push_back(bCand_KP4->currentState().globalPosition().y());
		 bDz4->push_back(bCand_KP4->currentState().globalPosition().z());
		 
		 //
	       
		 cout << "From mass5 fit, ks_KP (directly before applying constraint) momentum x = " << ks_KP->currentState().globalMomentum().x() << endl;
		 
		 /////////////////////////////////////////////////////
		 // Fit 5: Now try with Ks mass constraint in addition to fit 4
		 KinematicParticleFitter csFitterKs2;
		 KinematicConstraint * ks_c2 = new MassKinematicConstraint(ks_mass,ks_sigma);
		 
		 ksVertexFitTree2 = csFitterKs2.fit(ks_c2,ksVertexFitTree2);
		 if (!ksVertexFitTree2->isValid()){	 continue;  }
		 
		 ksVertexFitTree2->movePointerToTheTop();
		 RefCountedKinematicParticle ks_KP_MC = ksVertexFitTree2->currentParticle();
		 
		 vector<RefCountedKinematicParticle> vFit_KPs5;
		 vFit_KPs5.push_back(pFactory.particle(muon1TT,muon_mass,chi,ndf,muon_sigma));
		 vFit_KPs5.push_back(pFactory.particle(muon2TT,muon_mass,chi,ndf,muon_sigma));
		 vFit_KPs5.push_back(ks_KP_MC);
		 
		 //vFit_KPs5.push_back(ks_vFit_withMC);
		 //the one above which is the ks from the original fit gives idential results. 
		 //Now i have to see why ks_KP_MC is different from ks_vFit_withMC ???
		 
		 cout << "From mass5 fit, ks_KP_MC momentum x = " << ks_KP_MC->currentState().globalMomentum().x() << endl;
	       
		 MultiTrackKinematicConstraint *  j_psi_c5 = new  TwoTrackMassKinematicConstraint(psi_mass);
		 KinematicConstrainedVertexFitter kcvFitter5;
		 RefCountedKinematicTree vertexFitTree5 = kcvFitter5.fit(vFit_KPs5, j_psi_c5);
		 if (!vertexFitTree5->isValid()) continue;
		 vertexFitTree5->movePointerToTheTop();
		 RefCountedKinematicParticle bCand_KP5 = vertexFitTree5->currentParticle();
		 
		 cout << " vertex for B of final fit with both MCs = " << bCand_KP5->currentState().globalPosition() << " with momentum = " << bCand_KP5->currentState().globalMomentum() << " and mass = " << bCand_KP5->currentState().mass() << endl;
		 
		 bMass5->push_back(bCand_KP5->currentState().mass());
		 bPx5->push_back(bCand_KP5->currentState().globalMomentum().x());
		 bPy5->push_back(bCand_KP5->currentState().globalMomentum().y());
		 bPz5->push_back(bCand_KP5->currentState().globalMomentum().z());
		 bDx5->push_back(bCand_KP5->currentState().globalPosition().x());
		 bDy5->push_back(bCand_KP5->currentState().globalPosition().y());
		 bDz5->push_back(bCand_KP5->currentState().globalPosition().z());
		 
		 
		 
		 /////////////////////////////////////////////////
		 
		 pionParticles.clear();
		 muonParticles.clear();
		 vFitMCParticles.clear();
		 
		 vFit_KPs5.clear();
		 

	       //////////////////////////////
	       //Check PAT truth match here// PAT truth match doesn't work because the V0 tracks need to have the momentum defined at the V0 vertex
	       //////////////////////////////

	       GenParticleRef muPGP;
	       GenParticleRef muMGP;
	       GenParticleRef piPGP;
	       GenParticleRef piMGP;
	       bool psiMatch = false;
	       bool ksMatch = false;
	       bool bMatch = false;
	       
	       cout << "reco mu+ has eta = " << iMuonP->eta() << " and phi = " << iMuonP->phi() << endl;
	       for ( size_t iGP = 0; iGP<iMuonP->genParticlesSize(); ++iGP) {
		 const GenParticleRef gpRef = iMuonP->genParticleRef(iGP);
		 if (!gpRef.isNull()) {
		   cout << "genParticle " << iGP << " for mu+ has eta = " << gpRef->eta() << " and phi = " << gpRef->phi() << endl;
		   cout << "deltaR for mup = " << sqrt(  (iMuonP->eta()-gpRef->eta())*(iMuonP->eta()-gpRef->eta()) + (iMuonP->phi()-gpRef->phi())*(iMuonP->phi()-gpRef->phi()) ) << endl;
		   muPGP = gpRef;
		 } else {
		   cout << "gpRef is null for mu+" << endl;
		 }
	       }
		 
	       cout << "reco mu- has eta = " << iMuonM->eta() << " and phi = " << iMuonM->phi() << endl;
	       for ( size_t iGP = 0; iGP<iMuonM->genParticlesSize(); ++iGP) {
		 const GenParticleRef gpRef = iMuonM->genParticleRef(iGP);
		 if (!gpRef.isNull()) {
		   cout << "genParticle " << iGP << " for mu- has eta = " << gpRef->eta() << " and phi = " << gpRef->phi() << endl;
		   cout << "deltaR for mum = " << sqrt(  (iMuonM->eta()-gpRef->eta())*(iMuonM->eta()-gpRef->eta()) + (iMuonM->phi()-gpRef->phi())*(iMuonM->phi()-gpRef->phi()) ) << endl;
		   muMGP = gpRef;
		 } else {
		   cout << "gpRef is null for mu-" << endl;
		 }
	       }
	       
	       //look at V0 and track match
	       
	       cout << "patTrack1 genParticleSize = " << patTrack1.genParticlesSize() << endl;
	       cout << "reco pi+ has eta = " << patTrack1.eta() << " and phi = " << patTrack1.phi() << endl;
	       for ( size_t iGP = 0; iGP<patTrack1.genParticlesSize(); ++iGP) {
		 const GenParticleRef gpRef = patTrack1.genParticleRef(iGP);
		 cout << "got gpRef" << endl;
		 if (!gpRef.isNull()) {
		   cout << "genParticle " << iGP << " for pi+ has eta = " << gpRef->eta() << " and phi = " << gpRef->phi() << endl;
		   cout << "deltaR for pip = " << sqrt(  (patTrack1.eta()-gpRef->eta())*(patTrack1.eta()-gpRef->eta()) + (patTrack1.phi()-gpRef->phi())*(patTrack1.phi()-gpRef->phi()) ) << endl;
		   piPGP = gpRef;
		 } else {
		   cout << "gpRef is null for pi+" << endl;
		 }
	       }

	       cout << "reco pi- has eta = " << patTrack2.eta() << " and phi = " << patTrack2.phi() << endl;
	       for ( size_t iGP = 0; iGP<patTrack2.genParticlesSize(); ++iGP) {
		 const GenParticleRef gpRef = patTrack2.genParticleRef(iGP);
		 if (!gpRef.isNull()) {
		   cout << "genParticle " << iGP << " for pi- has eta = " << gpRef->eta() << " and phi = " << gpRef->phi() << endl;
		   cout << "number of mothers = " << gpRef->numberOfMothers() << endl;
		   cout << "deltaR for pim = " << sqrt(  (patTrack2.eta()-gpRef->eta())*(patTrack2.eta()-gpRef->eta()) + (patTrack2.phi()-gpRef->phi())*(patTrack2.phi()-gpRef->phi()) ) << endl;
		   piMGP = gpRef;
		 } else {
		   cout << "gpRef is null for pi-" << endl;
		 }
	       }

	       cout << "moving to truth match check from PAT matching" << endl;

	       //have all 4 track genParticles. Check for truth match

	       if ( muMGP.isNonnull() && muPGP.isNonnull() ) {
		 if ( muMGP->numberOfMothers()==1 && muPGP->numberOfMothers()==1 ) {
		   if ( muMGP->mother() == muPGP->mother() ) {
		     cout << "mu mothers match" << endl;
		     psiMatch = true;
		     cout << "mu+ mother pdgid = " << muPGP->mother()->pdgId() << endl;
		   } 
		   else cout << "mu mothers don't match" << endl;
		 } else cout << "muGPs don't both have one mother: muP mothers = " << muPGP->numberOfMothers() << "  and muM mothers = " << muMGP->numberOfMothers() << endl;
	       } else cout << "invalid ref in muons" << endl;
	       
	       if ( piMGP.isNonnull() && piPGP.isNonnull() ) {
		 if ( piMGP->numberOfMothers()==1 && piPGP->numberOfMothers()==1 ) {
		   if ( piMGP->mother() == piPGP->mother() ) {
		     cout << "pi mothers match with id = " << piMGP->mother()->pdgId() << "  and parent id = " << piMGP->mother()->mother()->pdgId() << endl;
		     ksMatch = true;
		   }  else cout << "pi mothers don't match" << endl;
		 }  else cout << "piGPs don't both have one mother: piP mothers = " << piPGP->numberOfMothers() << "  and piM mothers = " << piMGP->numberOfMothers() << endl;
	       } else cout << "invalid ref in pions" << endl;
	       
	       if (psiMatch && ksMatch) {
		 cout << "both ks and psi match, so check for common mother" << endl;
		 cout << "psi mother is pdgid " << muMGP->mother()->mother()->pdgId() << " and ks mother is " << piMGP->mother()->mother()->pdgId() << " and ks gmother is " << piMGP->mother()->mother()->mother()->pdgId() << endl;
		 if ( piMGP->mother()->mother() == muMGP->mother()->mother() ) {
		   cout << "ks and psi mothers match" << endl;
		   bMatch = true;
		 }
		 cout << "psi mother pdgid = " << muMGP->mother()->mother()->pdgId() << endl;
		 cout << "Ks mother pdgid = " << piMGP->mother()->mother()->pdgId() << endl;
	       }
	       
	       if(bMatch) truthMatchPAT.push_back(1);
	       else truthMatchPAT.push_back(-1);
	       
	       if(ksMatch) truthKsPAT.push_back(1);
	       else truthKsPAT.push_back(-1);
	       
	       if(psiMatch) truthPsiPAT.push_back(1);
	       else truthPsiPAT.push_back(-1);
	       
	     }
	   }
	 }
       } 
     } 
   } // if V0Handle > 0 and muHandle > 1

   //////////////////////////////////////////////////////
   //////// get truth information from genParticles only for events with a B candidate
   
   if (nB > 0) {
     priVtxX = bestVtx->x();
     priVtxY = bestVtx->y();
     priVtxZ = bestVtx->z();
     priVtxXE = bestVtx->xError();
     priVtxYE = bestVtx->yError();
     priVtxZE = bestVtx->zError();
     priVtxCL = ChiSquaredProbability((double)(bestVtx->chi2()),(double)(bestVtx->ndof())); 
     
     Handle<GenParticleCollection> genParticles;
     //     iEvent.getByLabel("genParticles",genParticles);
     //iEvent.getByLabel("genParticlesPlusSim",genParticles);
     iEvent.getByLabel(genParticles_, genParticles);

     genKsPsi = -1; genKstarpPsi = -1; genLambdaPsi = -1; prompt = 1; feedup = -1; feeddown = -1;
     
     for( size_t k = 0; k < genParticles->size(); k++ ) {
       // check if any of our signals were generated
       
       const Candidate & BCand = (*genParticles)[ k ];

       if ( abs(BCand.pdgId())==511 && abs(BCand.daughter(0)->pdgId())!=511 ) {
	 // only check for signal decay after possible B0 B0bar oscilation
	 cout << "found B0";
	 int ipsi(-1), iks(-1);
	 for ( uint i = 0; i < BCand.numberOfDaughters(); i++){
	   // check B0 for psi and ks daughters
	   const Candidate * genDau = BCand.daughter(i);
	   //cout << "B0 daughter " << i << " has id = " << genDau->pdgId() << endl;
	   cout << " =" << genDau->pdgId();
	   int imu1(-1), imu2(-1),  ipi1(-1), ipi2(-1);
	   bool wrong = false;
	   for ( uint j = 0; j < genDau->numberOfDaughters(); j++){
	     const Candidate * genGDau = genDau->daughter(j);
	     cout << " ==" << genGDau->pdgId();
	     // here also check for nuclear interactions
	     if ( genGDau->pdgId()==13 && abs(genDau->pdgId())==443 ) imu1 = j;
	     if ( genGDau->pdgId()==-13 && abs(genDau->pdgId())==443 ) imu2 = j;
	     if ( genGDau->pdgId()==211 && abs(genDau->pdgId())==310 ) ipi1 = j;
	     if ( genGDau->pdgId()==-211 && abs(genDau->pdgId())==310 ) ipi2 = j;
	     if ( genDau->pdgId()==443 && abs(genGDau->pdgId())!=13 && genGDau->pdgId()!=22 )
	       wrong = true;
	     if ( genDau->pdgId()==310 && abs(genGDau->pdgId())!=211 && genGDau->pdgId()!=22 )
	       wrong = true;
	   }
	   if ( genDau->pdgId()!=443 && genDau->pdgId()!=310 && genDau->pdgId()!=22 ) 
	     wrong = true;
	   if (imu1!=-1&&imu2!=-1&&!wrong) ipsi = i;
	   if (ipi1!=-1 && ipi2!=-1&&!wrong) iks = i;
	 }
	 
	 if (ipsi!=-1&&iks!=-1)
	   cout << " found genKsPsi";
	 
	 cout << endl;
	 
	 if (ipsi!=-1&&iks!=-1) {
	   
	   genKsPsi = 1;
	   //write out info from daughters
	   const Candidate * genpsi =  BCand.daughter(ipsi);
	   const Candidate * genks =  BCand.daughter(iks);
	   
	   const Candidate *BTemp = &BCand;
	   while ( (BTemp->vx() == genpsi->vx()) && (BTemp->vy() == genpsi->vy()) ) {
	     if (BTemp->numberOfMothers() > 0) {
	       BTemp = BTemp->mother(0);
	     } else {
	       cout << "Can't find mother of oscillating B!" << endl;
	       BTemp = 0;
	     }
	   }
	   
	   truePriVtxX = BTemp->vx();
	   truePriVtxY = BTemp->vy();
	   truePriVtxZ = BTemp->vz();
	   
	   trueBPx = BCand.px();
	   trueBPy = BCand.py();
	   trueBPz = BCand.pz();
	   
	   trueJPx = genpsi->px();
	   trueJPy = genpsi->py();
	   trueJPz = genpsi->pz();
	   trueBDecayVtxX = genpsi->vx();
	   trueBDecayVtxY = genpsi->vy();
	   trueBDecayVtxZ = genpsi->vz();

	   cout << "true B decay vertex = (" << genpsi->vx() << "," << genpsi->vy() << "," << genpsi->vz() << ")" << endl;

	   //cout << "getting info for mu+ and mu- with imup = " << imu1 << " and imum = " << imu2 << endl;

	   for (uint j=0; j<genpsi->numberOfDaughters(); j++) {
	     cout << "daughter " << j << " from genpsi has id = " << genpsi->daughter(j)->pdgId() << endl;
	     if (genpsi->daughter(j)->pdgId()==13) { //13 is a mu-
	       trueMumPx = genpsi->daughter(j)->px();
	       trueMumPy = genpsi->daughter(j)->py();
	       trueMumPz = genpsi->daughter(j)->pz();
	       trueJDecayVtxX = genpsi->daughter(j)->vx();
	       trueJDecayVtxY = genpsi->daughter(j)->vy();
	       trueJDecayVtxZ = genpsi->daughter(j)->vz();
	     }
	     if (genpsi->daughter(j)->pdgId()==-13) { //-13 is a mu+
	       trueMupPx = genpsi->daughter(j)->px();
	       trueMupPy = genpsi->daughter(j)->py();
	       trueMupPz = genpsi->daughter(j)->pz();
	     }
	   }

	   trueVPx = genks->px();
	   trueVPy = genks->py();
	   trueVPz = genks->pz();
	   
	   for (uint j=0; j<genks->numberOfDaughters(); j++) {
	     if ( genks->daughter(j)->charge()>0 ) {
	       trueVTrkpPx = genks->daughter(j)->px();
	       trueVTrkpPy = genks->daughter(j)->py();
	       trueVTrkpPz = genks->daughter(j)->pz();
	       trueVDecayVtxX = genks->daughter(j)->vx();
	       trueVDecayVtxY = genks->daughter(j)->vy();
	       trueVDecayVtxZ = genks->daughter(j)->vz();
	     }
	     if ( genks->daughter(j)->charge()<0 ) {
	       trueVTrkmPx = genks->daughter(j)->px();
	       trueVTrkmPy = genks->daughter(j)->py();
	       trueVTrkmPz = genks->daughter(j)->pz();
	     }
	   }
	
	   cout << "moving to truth match check from reco objects by hand" << endl;
	   
	   /////////////////////////////////////////////////////////////////////////
	   // determine MC truth

	   // calculate true eta and phi for all tracks
	   float trueMupPhi = atan(trueMupPy/trueMupPx);
	   if ( trueMupPx < 0 && trueMupPy < 0 ) trueMupPhi -= pi;
	   if ( trueMupPx < 0 && trueMupPy > 0 ) trueMupPhi += pi;
	   float trueMupP = sqrt( trueMupPx*trueMupPx +  trueMupPy*trueMupPy +  trueMupPz*trueMupPz );
	   float trueMupEta = 0.5*log( (trueMupP + trueMupPz)/(trueMupP - trueMupPz) );
	   
	   float trueMumPhi = atan(trueMumPy/trueMumPx);
	   if ( trueMumPx < 0 && trueMumPy < 0 ) trueMumPhi -= pi;
	   if ( trueMumPx < 0 && trueMumPy > 0 ) trueMumPhi += pi;
	   float trueMumP = sqrt( trueMumPx*trueMumPx +  trueMumPy*trueMumPy +  trueMumPz*trueMumPz );
	   float trueMumEta = 0.5*(log( (trueMumP + trueMumPz)/(trueMumP - trueMumPz) ) );
	   
	   float truePipPhi = atan(trueVTrkpPy/trueVTrkpPx);
	   if ( trueVTrkpPx < 0 && trueVTrkpPy < 0 ) truePipPhi -= pi;
	   if ( trueVTrkpPx < 0 && trueVTrkpPy > 0 ) truePipPhi += pi;
	   float truePipP = sqrt( trueVTrkpPx*trueVTrkpPx +  trueVTrkpPy*trueVTrkpPy +  trueVTrkpPz*trueVTrkpPz );
	   float truePipEta = 0.5*log( (truePipP + trueVTrkpPz)/(truePipP - trueVTrkpPz) );
	   
	   float truePimPhi = atan(trueVTrkmPy/trueVTrkmPx);
	   if ( trueVTrkmPx < 0 && trueVTrkmPy < 0 ) truePimPhi -= pi;
	   if ( trueVTrkmPx < 0 && trueVTrkmPy > 0 ) truePimPhi += pi;
	   float truePimP = sqrt( trueVTrkmPx*trueVTrkmPx +  trueVTrkmPy*trueVTrkmPy +  trueVTrkmPz*trueVTrkmPz );
	   float truePimEta = 0.5*log( (truePimP + trueVTrkmPz)/(truePimP - trueVTrkmPz) );
	   
	   //cout << "=======================" << endl;
	   cout << "For true B muP eta, phi = " << trueMupEta << "," << trueMupPhi << endl;
	   cout << "For true B muM eta, phi = " << trueMumEta << "," << trueMumPhi << endl;
	   cout << "For true B piP eta, phi = " << truePipEta << "," << truePipPhi << endl;
	   cout << "For true B piM eta, phi = " << truePimEta << "," << truePimPhi << endl;	 
	   
	   float RcutMu = 0.02;
	   float RcutPi = 0.10;
	   float RcutVtx = 10.;
	   
	   truthMatch.clear(); truthKs.clear(); truthPsi.clear();
	   
	   for (uint i = 0; i<mupPx->size(); i++) {
	     //loop to check all B candidates found

	     bool istrueMup = false;
	     bool istrueMum = false;
	     bool istruePip = false;
	     bool istruePim = false;
	     bool istrueKs = false;
	     bool istruePsi = false;
	     bool istrueB = false;
	     
	     // calculate eta and phi for all tracks in B candidate
	     float mupPhi = atan(mupPy->at(i)/mupPx->at(i));
	     if ( mupPx->at(i) < 0 && mupPy->at(i) < 0 ) mupPhi -= pi;
	     if ( mupPx->at(i) < 0 && mupPy->at(i) > 0 ) mupPhi += pi;
	     float mupP = sqrt( mupPx->at(i)*mupPx->at(i) +  mupPy->at(i)*mupPy->at(i) +  mupPz->at(i)*mupPz->at(i) );
	     float mupEta = 0.5*log( (mupP + mupPz->at(i))/(mupP - mupPz->at(i)) );
	     
	     float mumPhi = atan(mumPy->at(i)/mumPx->at(i));
	     if ( mumPx->at(i) < 0 && mumPy->at(i) < 0 ) mumPhi -= pi;
	     if ( mumPx->at(i) < 0 && mumPy->at(i) > 0 ) mumPhi += pi;
	     float mumP = sqrt( mumPx->at(i)*mumPx->at(i) +  mumPy->at(i)*mumPy->at(i) +  mumPz->at(i)*mumPz->at(i) );
	     float mumEta = 0.5*log( (mumP + mumPz->at(i))/(mumP - mumPz->at(i)) );	 
	     
	     float pipPhi = atan(VTrkpPy->at(i)/VTrkpPx->at(i));
	     if ( VTrkpPx->at(i) < 0 && VTrkpPy->at(i) < 0 ) pipPhi -= pi;
	     if ( VTrkpPx->at(i) < 0 && VTrkpPy->at(i) > 0 ) pipPhi += pi;
	     float pipP = sqrt( VTrkpPx->at(i)*VTrkpPx->at(i) +  VTrkpPy->at(i)*VTrkpPy->at(i) +  VTrkpPz->at(i)*VTrkpPz->at(i) );
	     float pipEta = 0.5*log( (pipP + VTrkpPz->at(i))/(pipP - VTrkpPz->at(i)) );
	     
	     float pimPhi = atan(VTrkmPy->at(i)/VTrkmPx->at(i));
	     if ( VTrkmPx->at(i) < 0 && VTrkmPy->at(i) < 0 ) pimPhi -= pi;
	     if ( VTrkmPx->at(i) < 0 && VTrkmPy->at(i) > 0 ) pimPhi += pi;
	     float pimP = sqrt( VTrkmPx->at(i)*VTrkmPx->at(i) +  VTrkmPy->at(i)*VTrkmPy->at(i) +  VTrkmPz->at(i)*VTrkmPz->at(i) );
	     float pimEta = 0.5*log( (pimP + VTrkmPz->at(i))/(pimP - VTrkmPz->at(i)) );
	     
	     cout << "For reco B muP eta, phi = " << mupEta << "," << mupPhi << endl;
	     cout << "For reco B muM eta, phi = " << mumEta << "," << mumPhi << endl;
	     cout << "For reco B piP eta, phi = " << pipEta << "," << pipPhi << endl;
	     cout << "For reco B piM eta, phi = " << pimEta << "," << pimPhi << endl;	 
	     
	     float deltaRmup = sqrt( (mupEta-trueMupEta)*(mupEta-trueMupEta) +  (mupPhi-trueMupPhi)*(mupPhi-trueMupPhi) );
	     if ( deltaRmup < RcutMu ) istrueMup = true;
	     
	     float deltaRmum = sqrt( (mumEta-trueMumEta)*(mumEta-trueMumEta) +  (mumPhi-trueMumPhi)*(mumPhi-trueMumPhi) ) ;
	     if ( deltaRmum < RcutMu ) istrueMum = true;
	     
	     float deltaRpip = sqrt( (pipEta-truePipEta)*(pipEta-truePipEta) +  (pipPhi-truePipPhi)*(pipPhi-truePipPhi) );
	     if ( deltaRpip < RcutPi ) istruePip = true;
	     
	     float deltaRpim = sqrt( (pimEta-truePimEta)*(pimEta-truePimEta) +  (pimPhi-truePimPhi)*(pimPhi-truePimPhi) );
	     if ( deltaRpim < RcutPi ) istruePim = true;
	     
	     cout << "deltaR for mup = " << deltaRmup << ", mum = " << deltaRmum << ", deltaRpip = " << deltaRpip << ", deltaRpim = " << deltaRpim << endl;

	     //check Ks vertex position truth match
	     float deltaRksvtx = sqrt( (trueVDecayVtxX - VDecayVtxX->at(i))*
				       (trueVDecayVtxX - VDecayVtxX->at(i)) +
				       (trueVDecayVtxY - VDecayVtxY->at(i))*
				       (trueVDecayVtxY - VDecayVtxY->at(i)) +
				       (trueVDecayVtxZ - VDecayVtxZ->at(i))*
				       (trueVDecayVtxZ - VDecayVtxZ->at(i)) );	     

	     if ( istrueMup & istrueMum ) istruePsi = true;
	     if ( istruePip & istruePim && (deltaRksvtx<RcutVtx) ) istrueKs = true;
	     if ( istruePsi & istrueKs ) istrueB = true;

	     if (istruePsi) {
	       cout << "true Psi from reco from cand " << i << endl;
	       truthPsi.push_back(1);
	     } else truthPsi.push_back(-1);
	     if (istrueKs) {
	       cout << "true Ks from reco from cand " << i << endl;
	       truthKs.push_back(1);
	     } else truthKs.push_back(-1);
	     if (istrueB) {
	       cout << "true B from reco from cand " << i << endl;
	       truthMatch.push_back(1);
	     } else truthMatch.push_back(-1);
	     
	   }
	   
	 }
	 
	 ///////////////////////////////////////////////////////////////////////
	   
       } // closes if (BCand == B0ID)
       
       //check for B->JPsiK*+(kspi) decay   
       if ( abs(BCand.pdgId())==521 ) {
	 cout << "found B+";
	 int ipsi(-1), ikstp(-1);
	 bool wrong = false;
	 for ( uint i = 0; i < BCand.numberOfDaughters(); i++){
	   const Candidate * genDau = BCand.daughter(i);
	   cout << " =" << genDau->pdgId();
	   int imu1(-1), imu2(-1),  ik0(-1), ipi(-1), iks(-1);
	   for ( uint j = 0; j < genDau->numberOfDaughters(); j++){
	     const Candidate * genGDau = genDau->daughter(j);
	     cout << " ==" << genGDau->pdgId();
	     if ( genGDau->pdgId()==13 && abs(genDau->pdgId())==443 ) imu1 = j;
	     if ( genGDau->pdgId()==-13 && abs(genDau->pdgId())==443 ) imu2 = j;
	     if ( genDau->pdgId()==443 && abs(genGDau->pdgId())!=13 && genGDau->pdgId()!=22 )
	       wrong = true;
	     if (genGDau->pdgId()==311 && abs(genDau->pdgId())==323 ) {
	       //K*+ decays to K0 pi and K0->Ks, so check for that, genGDau is the K0
	       for ( uint m = 0; m < genGDau->numberOfDaughters(); m++){
		 cout << " ===" << genGDau->daughter(m)->pdgId();
		 if (genGDau->daughter(m)->pdgId()==310) iks = m;
	       }
	       if (iks!=-1) {
		 const Candidate * ks = genGDau->daughter(iks);
		 int ipi1(-1), ipi2(-1);
		 for ( uint k = 0; k < ks->numberOfDaughters(); k++){
		   cout << " ====" << ks->daughter(k)->pdgId();
		   if (ks->daughter(k)->pdgId()==211) ipi1 = k;
		   if (ks->daughter(k)->pdgId()==-211) ipi2 = k; 
		   if ( abs(ks->daughter(k)->pdgId())!=211 && ks->daughter(k)->pdgId()!=22 )
		     wrong = true;
		 }
		 if (ipi1!=-1&&ipi2!=-1) {
		   ik0 = i;
		 }
	       }
	     }
	     if ( abs(genGDau->pdgId())==211 && abs(genDau->pdgId())==323 ) ipi = j;
	     if ( genDau->pdgId()==311 && abs(genGDau->pdgId())!=211 && genGDau->pdgId()!=22 )
	       wrong = true;
	   }
	   if ( genDau->pdgId()!=443 && genDau->pdgId()!=323 && genDau->pdgId()!=22 ) 
	     wrong = true;
	   if (imu1!=-1&&imu2!=-1&&!wrong) ipsi = i;
	   if (ik0!=-1 && ipi!=-1&&!wrong) ikstp = i;
	 }
	 if ( ipsi!=-1 && ikstp!=-1 ) {
	   cout << " found genKstarpPsi";
	   genKstarpPsi =1;
	 }
	 cout << endl;
       } //check for genparticle with id = 521 for a B+
       
       // check for Lambda_b
       if (abs(BCand.pdgId())==5122) {
	 cout << "found Lambda_B";
	 int ipsi(-1), ilam(-1);
	 bool wrong = false;
	 for ( uint i = 0; i < BCand.numberOfDaughters(); i++){
	   // check Lambda_b for psi and Lambda daughters
	   const Candidate * genDau = BCand.daughter(i);
	   cout << " =" << genDau->pdgId();
	   int imu1(-1), imu2(-1), ipi(-1), ip(-1);
	   for ( uint j = 0; j < genDau->numberOfDaughters(); j++){
	     cout << " ==" << genDau->daughter(j)->pdgId();
	     if (genDau->daughter(j)->pdgId()==13 && genDau->pdgId()==443) imu1 = j;
	     if (genDau->daughter(j)->pdgId()==-13 && genDau->pdgId()==443) imu2 = j; 
	     if ( abs(genDau->daughter(j)->pdgId())==211 && abs(genDau->pdgId())==3122) ipi = j;
	     if ( abs(genDau->daughter(j)->pdgId())==2212 && abs(genDau->pdgId())==3122) ip = j;
	     if ( genDau->pdgId()==443 && abs(genDau->daughter(j)->pdgId())!=13 && genDau->daughter(j)->pdgId()!=22 )
	       wrong = true;
	     if ( abs(genDau->pdgId())==3122 && abs(genDau->daughter(j)->pdgId())!=211 && abs(genDau->daughter(j)->pdgId())!=2212 && genDau->daughter(j)->pdgId()!=22 )
	       wrong = true;
	   }
	   if ( genDau->pdgId()!=443 && abs(genDau->pdgId())!=3122 && genDau->pdgId()!=22 ) 
	     wrong = true;
	   if (imu1!=-1&&imu2!=-1&&!wrong) ipsi = i;
	   if (ipi!=-1&&ip!=-1&&!wrong) ilam = i;
	 }
	 if (ipsi!=-1&&ilam!=-1) {
	   cout << " found genLambdaPsi";
	   genLambdaPsi = 1;
	 }
	 cout << endl;
       } // if (id==LambdaBID) 
       
       if ( abs(BCand.pdgId())==531 && abs(BCand.daughter(0)->pdgId())!=531 ) {
	 //only check after B_s oscilation
	 cout << "found B_s";
	 for ( uint i = 0; i < BCand.numberOfDaughters(); i++){
	   const Candidate * genDau = BCand.daughter(i);
	   cout << " =" << genDau->pdgId();
	   for ( uint j = 0; j < genDau->numberOfDaughters(); j++){
	     const Candidate * genGDau = genDau->daughter(j);
	     cout << " ==" << genGDau->pdgId();
	     for ( uint k = 0; k < genGDau->numberOfDaughters(); k++){
	       cout << " ===" << genGDau->daughter(k)->pdgId();
	     }
	   }
	 }
	 cout << endl;
       }
       

       // check to see if JPsi is prompt
       bool isPrompt = true;
       const Candidate & PsiCand = (*genParticles)[ k ];
       if (abs(PsiCand.pdgId())==443) {
	 for ( uint i = 0; i < PsiCand.numberOfMothers(); i++){
	   const Candidate * psiMom = PsiCand.mother(i);
	   cout << "psi mother has ID = " << psiMom->pdgId() << endl;
	   if ( (abs(psiMom->pdgId())<600 && abs(psiMom->pdgId())>500) || (abs(psiMom->pdgId())<6000 && abs(psiMom->pdgId())>5000) ) {
	     isPrompt = false;
	     continue;
	   } else {
	     for ( uint i = 0; i < psiMom->numberOfMothers(); i++){
	       const Candidate * psiGMom = psiMom->mother(i);
	       cout << "psi grandmother has ID = " << psiGMom->pdgId() << endl;
	       if ( (abs(psiGMom->pdgId())<600 && abs(psiGMom->pdgId())>500) ||  (abs(psiGMom->pdgId())<6000 && abs(psiGMom->pdgId())>5000) ) {
		 isPrompt = false;
		 continue;
	       } else {
		 for ( uint i = 0; i < psiGMom->numberOfMothers(); i++){
		   const Candidate * psiGGMom = psiGMom->mother(i);
		   cout << "psi greatgrandmother has ID = " << psiGGMom->pdgId() << endl;
		   if ( (abs(psiGGMom->pdgId())<600 && abs(psiGGMom->pdgId())>500) ||  (abs(psiGGMom->pdgId())<6000 && abs(psiGGMom->pdgId())>5000) ) {
		     isPrompt = false;
		     continue;
		   }
		 }
	       }
	     }
	   }
	 }
	 if (!isPrompt) prompt = -1;
       }

       // check for JPsiKs feed up // currently from B+->JPsi K+
       if (abs(BCand.pdgId())==521) {
	 bool psidau = false;
	 bool kdau = false;
	 for ( uint i = 0; i < BCand.numberOfDaughters(); i++){
	   const Candidate * genDau = BCand.daughter(i);
	   if ( genDau->pdgId()==443 ) psidau = true;
	   if ( abs(genDau->pdgId())==321 ) kdau = true;
	 }
	 if ( psidau && kdau ) feedup = 1;
       }

       if (abs(BCand.pdgId())==5122) {  //Lambda_B
	 bool psidau = false;
	 bool lamdau = false;
	 for ( uint i = 0; i < BCand.numberOfDaughters(); i++){
	   const Candidate * genDau = BCand.daughter(i);
	   if ( genDau->pdgId()==443 ) psidau = true;
	   if ( abs(genDau->pdgId())==3122 ) lamdau = true;
	 }
	 if ( psidau && lamdau ) feedup = 2;
       }
       
       // check for JPsiKs feed down // currently from 
       if (abs(BCand.pdgId())==511) {
	 bool psidau = false;
	 bool ksdau = false;
	 bool psi2Sdau = false;
	 bool chic1dau = false;
	 bool kst0dau = false;
	 bool kst20dau = false;
	 bool pipGDau = false;
	 bool pizGDau = false;
	 for ( uint i = 0; i < BCand.numberOfDaughters(); i++){
	   const Candidate * genDau = BCand.daughter(i);
	   if ( genDau->pdgId()==443 ) psidau = true;
	   if ( genDau->pdgId()==310 || abs(genDau->pdgId())==311 ) ksdau = true;
	   if ( abs(genDau->pdgId())==100443 ) psi2Sdau = true;
	   if ( abs(genDau->pdgId())==20443 ) chic1dau = true;
	   if ( abs(genDau->pdgId())==313 ) {
	     kst0dau = true;
	     for ( uint j = 0; j < genDau->numberOfDaughters(); j++){
	       const Candidate * genGDau = genDau->daughter(j);
	       if ( abs(genGDau->pdgId())==211 ) pipGDau = true;
	       if ( abs(genGDau->pdgId())==111 ) pizGDau = true;
	     }
	   }
	   if ( abs(genDau->pdgId())==315 ) kst20dau = true;
	 }
	 if ( psi2Sdau && ksdau ) feeddown = 1;
	 if ( chic1dau && ksdau ) feeddown = 2;
	 if ( psidau && kst0dau && pizGDau ) feeddown = 3;
	 if ( psidau && kst0dau && pipGDau ) feeddown = 4;
	 if ( psidau && kst20dau ) feeddown = 5;
       }
       
       if (abs(BCand.pdgId())==521) {
	 bool psidau = false;
	 bool kstpdau = false;
	 bool kst1pdau = false;
	 bool pipGDau = false;
	 bool kpGDau = false;
	 for ( uint i = 0; i < BCand.numberOfDaughters(); i++){
	   const Candidate * genDau = BCand.daughter(i);
	   if ( genDau->pdgId()==443 ) psidau = true;
	   if ( abs(genDau->pdgId())==323 ) {
	     kstpdau = true;
	     for ( uint j = 0; j < genDau->numberOfDaughters(); j++){
	       const Candidate * genGDau = genDau->daughter(j);
	       if ( abs(genGDau->pdgId())==211 ) pipGDau = true;
	       if ( abs(genGDau->pdgId())==321 ) kpGDau = true;
	     }    
	   }
	   if ( abs(genDau->pdgId())==10323 ) kst1pdau = true;
	 }
	 if ( psidau && kstpdau && pipGDau ) feeddown = 6; 
	 if ( psidau && kstpdau && kpGDau ) feeddown = 7; 
	 if ( psidau && kst1pdau ) feeddown = 8; 
       }

       // check deeper truth match for KEVIN
       
       const Candidate *v0(0);
       const Candidate *psi(0);
       bool foundV0 = false;
       bool foundPsi = false;
       if ( ( abs(BCand.pdgId())==511 || abs(BCand.pdgId())==521 || 
	      abs(BCand.pdgId())==531 || abs(BCand.pdgId())==5122 ) &&
	    genKsPsi != 1 ) {
	 // loop through daughters to search for JPsi (443) or V0 (310, 3122)
	 for ( uint i = 0; i < BCand.numberOfDaughters(); i++){
	   const Candidate * genDau = BCand.daughter(i);
	   if ( genDau->pdgId()==443 ) {psi = genDau; foundPsi = true;}
	   if ( genDau->pdgId()==310||abs(genDau->pdgId())==5122 ) {v0 = genDau; foundV0 = true;}
	   for ( uint j = 0; j < genDau->numberOfDaughters(); j++){
	     const Candidate * genGDau = genDau->daughter(j);
	     if ( genGDau->pdgId()==443 ) {psi = genGDau; foundPsi = true;}
	     if ( genGDau->pdgId()==310||abs(genGDau->pdgId())==5122 ) {v0 = genGDau; foundV0 = true;}
	     for ( uint k = 0; k < genGDau->numberOfDaughters(); k++){
	       const Candidate * genGGDau = genGDau->daughter(k);
	       if ( genGGDau->pdgId()==443 ) {psi = genGGDau; foundPsi = true;}
	       if ( genGGDau->pdgId()==310||abs(genGGDau->pdgId())==5122 ) {v0 = genGGDau; foundV0 = true;}
	       for ( uint l = 0; l < genGGDau->numberOfDaughters(); l++){
		 const Candidate * genGGGDau = genGGDau->daughter(l);
		 if ( genGGGDau->pdgId()==443 ) {psi = genGGGDau; foundPsi = true;}
		 if ( genGGGDau->pdgId()==310||abs(genGGGDau->pdgId())==5122 ) {v0 = genGGGDau; foundV0 = true;}
	       }
	     }
	   }
	 }
       }
       if (foundV0&&foundPsi) {
	 fillV0(*v0);
	 fillPsi(*psi);
       }
     }
     
     if ( truthMatch.size()==0 ) { // if no truth match to signal has been found, fill with zeros
       for (uint i = 0; i<mupPx->size(); i++) {
	 truthPsi.push_back(0);
	 truthKs.push_back(0);
	 truthMatch.push_back(0);
       }
     }
     
     
   }
   
   //fill the tree and clear the vectors
   if (nB > 0 ) {
     cout << "filling tree" << endl;
     tree_->Fill();
   }
   l1_mu3 = 0; l1_2mu3 = 0; l1_muOpen = 0; l1_mu0 = 0;
   hlt_mu3 = 0; hlt_mu5 = 0; hlt_mu7 = 0; hlt_mu9 = 0; hlt_2mu0 = 0; hlt_2mu3 = 0; hlt_2mu3JPsi = 0;
   hltBJPsiMuMu = 0;
   nB = 0;
   priVtxX = 0; priVtxY = 0; priVtxZ = 0; priVtxXE = 0; priVtxYE = 0; priVtxZE = 0; priVtxCL = 0;
   bMass->clear(); bVtxCL->clear(); bPx->clear(); bPy->clear(); bPz->clear(); 
   bPxE->clear(); bPyE->clear(); bPzE->clear(); bctau->clear(); bctau2D->clear(); bctauBS->clear(); bctauMPV->clear();  bctaunewBS->clear(); bctauMPVnewBS->clear();
   bctauMPVBS->clear(); bDecayVtxX->clear(); bDecayVtxY->clear(); bDecayVtxZ->clear(); 
   bDecayVtxXE->clear(); bDecayVtxYE->clear(); bDecayVtxZE->clear(); 
   bResMass->clear(); bResVtxCL->clear(); bResPx->clear(); bResPy->clear(); bResPz->clear(); 
   bResDecayVtxX->clear(); bResDecayVtxY->clear(); bResDecayVtxZ->clear();
   bResDecayVtxXE->clear(); bResDecayVtxYE->clear(); bResDecayVtxZE->clear();
   VMass->clear(); VVtxCL->clear(); VPx->clear(); VPy->clear(); VPz->clear();
   VDecayVtxX->clear(); VDecayVtxY->clear(); VDecayVtxZ->clear();
   VDecayVtxXE->clear(); VDecayVtxYE->clear(); VDecayVtxZE->clear();
   JMass->clear(); JVtxCL->clear(); JPx->clear(); JPy->clear(); JPz->clear();
   JDecayVtxX->clear(); JDecayVtxY->clear(); JDecayVtxZ->clear();
   JDecayVtxXE->clear(); JDecayVtxYE->clear(); JDecayVtxZE->clear();
   mumPx->clear(); mumPy->clear(); mumPz->clear(); mumD0->clear(); mumD0E->clear();
   mupPx->clear(); mupPy->clear(); mupPz->clear(); mupD0->clear(); mupD0E->clear();
   VTrkpMass->clear(); VTrkpPx->clear(); VTrkpPy->clear(); VTrkpPz->clear(); 
   VTrkpD0->clear(); VTrkpD0E->clear();
   VTrkmMass->clear(); VTrkmPx->clear(); VTrkmPy->clear(); VTrkmPz->clear();
   VTrkmD0->clear(); VTrkmD0E->clear();
   bResTrkPx->clear(); bResTrkPy->clear(); bResTrkPz->clear();
   bResTrkD0->clear(); bResTrkD0E->clear(); bResTrkChg->clear();
   genKsPsi = 0; genKstarpPsi = 0; genLambdaPsi = 0; feedup = 0; feeddown = 0;
   truePriVtxX = 0; truePriVtxY = 0; truePriVtxZ = 0; trueBPx = 0; trueBPy = 0; trueBPz = 0;
   trueBDecayVtxX = 0; trueBDecayVtxY = 0; trueBDecayVtxZ = 0; trueBResPx = 0; trueBResPy = 0; trueBResPz = 0;
   trueBResDecayVtxX = 0; trueBResDecayVtxY = 0; trueBResDecayVtxZ = 0; 
   trueVPx = 0; trueVPy = 0; trueVPz = 0;
   trueVDecayVtxX = 0; trueVDecayVtxY = 0; trueVDecayVtxZ = 0; trueJPx = 0; trueJPy = 0; trueJPz = 0;
   trueJDecayVtxX = 0; trueJDecayVtxY = 0; trueJDecayVtxZ = 0;
   trueMumPx = 0; trueMumPy = 0; trueMumPz = 0; trueMumD0 = 0;
   trueMupPx = 0; trueMupPy = 0; trueMupPz = 0; trueMupD0 = 0;
   trueVTrkpPx = 0; trueVTrkpPy = 0; trueVTrkpPz = 0; trueVTrkpD0 = 0;
   trueVTrkmPx = 0; trueVTrkmPy = 0; trueVTrkmPz = 0; trueVTrkmD0 = 0;
   trueBResTrkPx = 0; trueBResTrkPy = 0; trueBResTrkPz = 0; trueBResTrkD0 = 0; trueBResTrkChg = 0;
   prompt = 0; truthMatch.clear(); truthKs.clear(); truthPsi.clear(); truthMatchPAT.clear(); truthKsPAT.clear(); truthPsiPAT.clear(); 

   bMass2->clear(); bMass3->clear(); bMass4->clear(); bMass5->clear(); 
   bPx2->clear(); bPx3->clear(); bPx4->clear(); bPx5->clear(); bDx2->clear(); bDx3->clear(); bDx4->clear(); bDx5->clear(); 
   bPy2->clear(); bPy3->clear(); bPy4->clear(); bPy5->clear(); bDy2->clear(); bDy3->clear(); bDy4->clear(); bDy5->clear(); 
   bPz2->clear(); bPz3->clear(); bPz4->clear(); bPz5->clear(); bDz2->clear(); bDz3->clear(); bDz4->clear(); bDz5->clear(); 
}

void PATBd2JpsiKs::fillPsi(const reco::Candidate& genpsi) {
  
  for (uint i=0; i<genpsi.numberOfDaughters(); i++) {
    if (genpsi.daughter(i)->pdgId()==13) { //13 is a mu-
      trueMumPx = genpsi.daughter(i)->px();
      trueMumPy = genpsi.daughter(i)->py();
      trueMumPz = genpsi.daughter(i)->pz();
    }
    if (genpsi.daughter(i)->pdgId()==-13) { //-13 is a mu+
      trueMupPx = genpsi.daughter(i)->px();
      trueMupPy = genpsi.daughter(i)->py();
      trueMupPz = genpsi.daughter(i)->pz();
    }
  }
}

void PATBd2JpsiKs::fillV0(const reco::Candidate& genv0) {
  
  for (uint i=0; i<genv0.numberOfDaughters(); i++) {
    if (genv0.daughter(i)->charge()>0 && genv0.numberOfDaughters()==2) {
      trueVTrkpPx = genv0.daughter(i)->px();
      trueVTrkpPy = genv0.daughter(i)->py();
      trueVTrkpPz = genv0.daughter(i)->pz();
      trueVDecayVtxX = genv0.daughter(i)->vx();
      trueVDecayVtxY = genv0.daughter(i)->vy();
      trueVDecayVtxZ = genv0.daughter(i)->vz();
    }
    if (genv0.daughter(i)->charge()<0 && genv0.numberOfDaughters()==2) {
      trueVTrkmPx = genv0.daughter(i)->px();
      trueVTrkmPy = genv0.daughter(i)->py();
      trueVTrkmPz = genv0.daughter(i)->pz();
    }
  }
}

// ------------ method called once each job just before starting event loop  ------------

void 
PATBd2JpsiKs::beginJob(const edm::EventSetup&)
{
  edm::Service<TFileService> fs;
  tree_ = fs->make<TTree>("ntuple","btosmumu ntuple");

  tree_->Branch("l1_mu3",&l1_mu3,"l1_mu3/i");
  tree_->Branch("l1_2mu3",&l1_2mu3,"l1_2mu3/i");
  tree_->Branch("l1_muOpen",&l1_muOpen,"l1_muOpen/i");
  tree_->Branch("l1_mu0",&l1_mu0,"l1_mu0/i");
  tree_->Branch("hlt_mu3",&hlt_mu3,"hlt_mu3/i");
  tree_->Branch("hlt_mu5",&hlt_mu5,"hlt_mu5/i");
  tree_->Branch("hlt_mu7",&hlt_mu7,"hlt_mu7/i");
  tree_->Branch("hlt_mu9",&hlt_mu9,"hlt_mu9/i");
  tree_->Branch("hlt_2mu0",&hlt_2mu0,"hlt_2mu0/i");
  tree_->Branch("hlt_2mu3",&hlt_2mu3,"hlt_2mu3/i");
  tree_->Branch("hlt_2mu3JPsi",&hlt_2mu3JPsi,"hlt_2mu3JPsi/i");
  tree_->Branch("hltBJPsiMuMu",&hltBJPsiMuMu,"hltBJPsiMuMu/i");
  tree_->Branch("nB",&nB,"nB/i");
  tree_->Branch("priVtxX",&priVtxX, "priVtxX/f");
  tree_->Branch("priVtxY",&priVtxY, "priVtxY/f");
  tree_->Branch("priVtxZ",&priVtxZ, "priVtxZ/f");
  tree_->Branch("priVtxXE",&priVtxXE, "priVtxXE/f");
  tree_->Branch("priVtxYE",&priVtxYE, "priVtxYE/f");
  tree_->Branch("priVtxZE",&priVtxZE, "priVtxZE/f");
  tree_->Branch("priVtxCL",&priVtxCL, "priVtxCL/f");
  tree_->Branch("bMass",&bMass);
  tree_->Branch("bVtxCL",&bVtxCL);
  tree_->Branch("bPx",&bPx);
  tree_->Branch("bPy",&bPy);
  tree_->Branch("bPz",&bPz);
  tree_->Branch("bPxE",&bPxE);
  tree_->Branch("bPyE",&bPyE);
  tree_->Branch("bPzE",&bPzE);
  tree_->Branch("bctau",&bctau);
  tree_->Branch("bctau2D",&bctau2D);
  tree_->Branch("bctauBS",&bctauBS);
  tree_->Branch("bctauMPV",&bctauMPV);
  tree_->Branch("bctauMPVBS",&bctauMPVBS);
  tree_->Branch("bctaunewBS",&bctaunewBS);
  tree_->Branch("bctauMPVnewBS",&bctauMPVnewBS);
  tree_->Branch("bDecayVtxX",&bDecayVtxX);
  tree_->Branch("bDecayVtxY",&bDecayVtxY);
  tree_->Branch("bDecayVtxZ",&bDecayVtxZ);
  tree_->Branch("bDecayVtxXE",&bDecayVtxXE);
  tree_->Branch("bDecayVtxYE",&bDecayVtxYE);
  tree_->Branch("bDecayVtxZE",&bDecayVtxZE);
  tree_->Branch("bResMass",&bResMass);
  tree_->Branch("bResVtxCL",&bResVtxCL);
  tree_->Branch("bResPx",&bResPx);
  tree_->Branch("bResPy",&bResPy);
  tree_->Branch("bResPz",&bResPz);
  tree_->Branch("bResDecayVtxX",&bResDecayVtxX);
  tree_->Branch("bResDecayVtxY",&bResDecayVtxY);
  tree_->Branch("bResDecayVtxZ",&bResDecayVtxZ);
  tree_->Branch("bResDecayVtxXE",&bResDecayVtxXE);
  tree_->Branch("bResDecayVtxYE",&bResDecayVtxYE);
  tree_->Branch("bResDecayVtxZE",&bResDecayVtxZE);
  tree_->Branch("VMass",&VMass);
  tree_->Branch("VVtxCL",&VVtxCL);
  tree_->Branch("VPx",&VPx);
  tree_->Branch("VPy",&VPy);
  tree_->Branch("VPz",&VPz);
  tree_->Branch("VDecayVtxX",&VDecayVtxX);
  tree_->Branch("VDecayVtxY",&VDecayVtxY);
  tree_->Branch("VDecayVtxZ",&VDecayVtxZ);
  tree_->Branch("VDecayVtxXE",&VDecayVtxXE);
  tree_->Branch("VDecayVtxYE",&VDecayVtxYE);
  tree_->Branch("VDecayVtxZE",&VDecayVtxZE);
  tree_->Branch("JMass",&JMass);
  tree_->Branch("JVtxCL",&JVtxCL);
  tree_->Branch("JPx",&JPx);
  tree_->Branch("JPy",&JPy);
  tree_->Branch("JPz",&JPz);
  tree_->Branch("JDecayVtxX",&JDecayVtxX);
  tree_->Branch("JDecayVtxY",&JDecayVtxY);
  tree_->Branch("JDecayVtxZ",&JDecayVtxZ);
  tree_->Branch("JDecayVtxXE",&JDecayVtxXE);
  tree_->Branch("JDecayVtxYE",&JDecayVtxYE);
  tree_->Branch("JDecayVtxZE",&JDecayVtxZE);
  tree_->Branch("mumPx",&mumPx);
  tree_->Branch("mumPy",&mumPy);
  tree_->Branch("mumPz",&mumPz);
  tree_->Branch("mumD0",&mumD0);
  tree_->Branch("mumD0E",&mumD0E);
  tree_->Branch("mupPx",&mupPx);
  tree_->Branch("mupPy",&mupPy);
  tree_->Branch("mupPz",&mupPz);
  tree_->Branch("mupD0",&mupD0);
  tree_->Branch("mupD0E",&mupD0E);
  tree_->Branch("VTrkpTrk1Mass",&VTrkpMass);
  tree_->Branch("VTrkpPx",&VTrkpPx);
  tree_->Branch("VTrkpPy",&VTrkpPy);
  tree_->Branch("VTrkpPz",&VTrkpPz);
  tree_->Branch("VTrkpD0",&VTrkpD0);
  tree_->Branch("VTrkpD0E",&VTrkpD0E);
  tree_->Branch("VTrkmMass",&VTrkmMass);
  tree_->Branch("VTrkmPx",&VTrkmPx);
  tree_->Branch("VTrkmPy",&VTrkmPy);
  tree_->Branch("VTrkmPz",&VTrkmPz);
  tree_->Branch("VTrkmD0",&VTrkmD0);
  tree_->Branch("VTrkmD0E",&VTrkmD0E);
  tree_->Branch("bResTrkPx",&bResTrkPx);
  tree_->Branch("bResTrkPy",&bResTrkPy);
  tree_->Branch("bResTrkPz",&bResTrkPz);
  tree_->Branch("bResTrkD0",&bResTrkD0);
  tree_->Branch("bResTrkD0E",&bResTrkD0E);
  tree_->Branch("bResTrkChg",&bResTrkChg);
  tree_->Branch("genKsPsi", &genKsPsi, "genKsPsi/I");
  tree_->Branch("genKstarpPsi", &genKstarpPsi, "genKstarpPsi/I");
  tree_->Branch("genLambdaPsi", &genLambdaPsi, "genLambdaPsi/I");
  tree_->Branch("feedup", &feedup, "feedup/I");
  tree_->Branch("feeddown", &feeddown, "feeddown/I");

  tree_->Branch("bMass2",&bMass2);
  tree_->Branch("bMass3",&bMass3);
  tree_->Branch("bMass4",&bMass4);
  tree_->Branch("bMass5",&bMass5);
  tree_->Branch("bPx2",&bPx2);
  tree_->Branch("bPx3",&bPx3);
  tree_->Branch("bPx4",&bPx4);
  tree_->Branch("bPx5",&bPx5);
  tree_->Branch("bPy2",&bPy2);
  tree_->Branch("bPy3",&bPy3);
  tree_->Branch("bPy4",&bPy4);
  tree_->Branch("bPy5",&bPy5);
  tree_->Branch("bPz2",&bPz2);
  tree_->Branch("bPz3",&bPz3);
  tree_->Branch("bPz4",&bPz4);
  tree_->Branch("bPz5",&bPz5);
  tree_->Branch("bDx2",&bDx2);
  tree_->Branch("bDx3",&bDx3);
  tree_->Branch("bDx4",&bDx4);
  tree_->Branch("bDx5",&bDx5);
  tree_->Branch("bDy2",&bDy2);
  tree_->Branch("bDy3",&bDy3);
  tree_->Branch("bDy4",&bDy4);
  tree_->Branch("bDy5",&bDy5);
  tree_->Branch("bDz2",&bDz2);
  tree_->Branch("bDz3",&bDz3);
  tree_->Branch("bDz4",&bDz4);
  tree_->Branch("bDz5",&bDz5);


  // do branches for MC truth
  tree_->Branch("truePriVtxX", &truePriVtxX, "truePriVtxX/f");
  tree_->Branch("truePriVtxY", &truePriVtxY, "truePriVtxY/f");
  tree_->Branch("truePriVtxZ", &truePriVtxZ, "truePriVtxZ/f");
  tree_->Branch("trueBPx",&trueBPx, "trueBPx/f");
  tree_->Branch("trueBPy",&trueBPy, "trueBPy/f");
  tree_->Branch("trueBPz",&trueBPz, "trueBPz/f");
  tree_->Branch("trueBDecayVtxX",&trueBDecayVtxX, "trueBDecayVtxX/f");
  tree_->Branch("trueBDecayVtxY",&trueBDecayVtxY, "trueBDecayVtxY/f");
  tree_->Branch("trueBDecayVtxZ",&trueBDecayVtxZ, "trueBDecayVtxZ/f");
  tree_->Branch("trueBResPx",&trueBResPx, "trueBResPx/f");
  tree_->Branch("trueBResPy",&trueBResPy, "trueBResPy/f"); 
  tree_->Branch("trueBResPz",&trueBResPz, "trueBResPz/f");
  tree_->Branch("trueBResDecayVtxX",&trueBResDecayVtxX, "trueBResDecayVtxX/f");
  tree_->Branch("trueBResDecayVtxY",&trueBResDecayVtxY, "trueBResDecayVtxY/f");
  tree_->Branch("trueBResDecayVtxZ",&trueBResDecayVtxZ, "trueBResDecayVtxZ/f");
  tree_->Branch("trueVPx",&trueVPx, "trueVPx/f"); 
  tree_->Branch("trueVPy",&trueVPy, "trueVPy/f"); 
  tree_->Branch("trueVPz",&trueVPz, "trueVPz/f");
  tree_->Branch("trueVDecayVtxX",&trueVDecayVtxX, "trueVDecayVtxX/f"); 
  tree_->Branch("trueVDecayVtxY",&trueVDecayVtxY, "trueVDecayVtxY/f"); 
  tree_->Branch("trueVDecayVtxZ",&trueVDecayVtxZ, "trueVDecayVtxZ/f");
  tree_->Branch("trueJPx",&trueJPx, "trueJPx/f");
  tree_->Branch("trueJPy",&trueJPy, "trueJPy/f"); 
  tree_->Branch("trueJPz",&trueJPz, "trueJPz/f");
  tree_->Branch("trueJDecayVtxX",&trueJDecayVtxX, "trueJDecayVtxX/f"); 
  tree_->Branch("trueJDecayVtxY",&trueJDecayVtxY, "trueJDecayVtxY/f"); 
  tree_->Branch("trueJDecayVtxZ",&trueJDecayVtxZ, "trueJDecayVtxZ/f");
  tree_->Branch("trueMumPx",&trueMumPx, "trueMumPx/f"); 
  tree_->Branch("trueMumPy",&trueMumPy, "trueMumPy/f"); 
  tree_->Branch("trueMumPz",&trueMumPz, "trueMumPz/f"); 
  tree_->Branch("trueMumD0",&trueMumD0, "trueMumD0/f");
  tree_->Branch("trueMupPx",&trueMupPx, "trueMupPx/f"); 
  tree_->Branch("trueMupPy",&trueMupPy, "trueMupPy/f"); 
  tree_->Branch("trueMupPz",&trueMupPz, "trueMupPz/f"); 
  tree_->Branch("trueMupD0",&trueMupD0, "trueMupD0/f");
  tree_->Branch("trueVTrkpPx",&trueVTrkpPx, "trueVTrkpPx/f"); 
  tree_->Branch("trueVTrkpPy",&trueVTrkpPy, "trueVTrkpPy/f"); 
  tree_->Branch("trueVTrkpPz",&trueVTrkpPz, "trueVTrkpPz/f"); 
  tree_->Branch("trueVTrkpD0",&trueVTrkpD0, "trueVTrkpD0/f");
  tree_->Branch("trueVTrkmPx",&trueVTrkmPx, "trueVTrkmPx/f"); 
  tree_->Branch("trueVTrkmPy",&trueVTrkmPy, "trueVTrkmPy/f"); 
  tree_->Branch("trueVTrkmPz",&trueVTrkmPz, "trueVTrkmPz/f"); 
  tree_->Branch("trueVTrkmD0",&trueVTrkmD0, "trueVTrkmD0/f");
  tree_->Branch("trueBResTrkPx",&trueBResTrkPx, "trueBResTrkPx/f"); 
  tree_->Branch("trueBResTrkPy",&trueBResTrkPy, "trueBResTrkPy/f"); 
  tree_->Branch("trueBResTrkPz",&trueBResTrkPz, "trueBResTrkPz/f"); 
  tree_->Branch("trueBResTrkD0",&trueBResTrkD0, "trueBResTrkD0/f");
  tree_->Branch("trueBResTrkChg",&trueBResTrkChg, "trueBResTrkChg/I");
  tree_->Branch("prompt",&prompt, "prompt/I");
  tree_->Branch("truthMatch",&truthMatch);
  tree_->Branch("truthKs",&truthKs);
  tree_->Branch("truthPsi",&truthPsi);
  tree_->Branch("truthMatchPAT",&truthMatchPAT);
  tree_->Branch("truthKsPAT",&truthKsPAT);
  tree_->Branch("truthPsiPAT",&truthPsiPAT);
}


// ------------ method called once each job just after ending the event loop  ------------
void 
PATBd2JpsiKs::endJob() {
  tree_->GetDirectory()->cd();
  tree_->Write();
}

//define this as a plug-in
DEFINE_FWK_MODULE(PATBd2JpsiKs);

