// -*- C++ -*-
//
// Package:    PATLambdaB2JpsiLambda
// Class:      PATLambdaB2JpsiLambda
// 
/**\class PATLambdaB2JpsiLambda PATLambdaB2JpsiLambda.cc HeavyFlavorAnalysis/Examples/src/PATLambdaB2JpsiLambda.cc

 Description: <one line class summary>
Make rootTuple for b->s JPsi(mu+mu-) analyses

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Keith Ulmer
//         Created:  Wed May  7 13:15:04 MDT 2008
// $Id: PATLambdaB2JpsiLambda.cc,v 1.4 2009/12/18 22:19:55 kaulmer Exp $
//
//


// system include files
#include <memory>

// user include files
#include "HeavyFlavorAnalysis/Examples/interface/PATLambdaB2JpsiLambda.h"

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
PATLambdaB2JpsiLambda::PATLambdaB2JpsiLambda(const edm::ParameterSet& iConfig)
  :
  hepMC(iConfig.getUntrackedParameter<std::string>("HepMC",std::string("source")) ),
  hlTriggerResults_(iConfig.getUntrackedParameter<edm::InputTag>("HLTriggerResults",edm::InputTag("TriggerResults::HLT")) ),
  blist(iConfig.getUntrackedParameter("Blist",std::string("Bd"))),
  muonType(iConfig.getUntrackedParameter("muonType",std::string("muons"))),
  vtxSample( iConfig.getUntrackedParameter<std::string>("VtxSample",std::string("offlinePrimaryVertices")) ), 

  tree_(0), l1_mu3(0), l1_2mu3(0), l1_muOpen(0), l1_mu0(0),
  hlt_mu3(0), hlt_mu5(0), hlt_mu7(0), hlt_mu9(0), hlt_2mu0(0), hlt_2mu3(0), hlt_2mu3JPsi(0), hltBJPsiMuMu(0), nB(0),
  priVtxX(0), priVtxY(0), priVtxZ(0), priVtxXE(0), priVtxYE(0), priVtxZE(0), priVtxCL(0),
  bMass(0), bVtxCL(0), bPx(0), bPy(0), bPz(0), bPxE(0), bPyE(0), bPzE(0), 
  bctauMPVBS(0),
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

  truePriVtxX(0), truePriVtxY(0), truePriVtxZ(0), trueBPx(0), trueBPy(0), trueBPz(0), trueBDecayVtxX(0), trueBDecayVtxY(0), trueBDecayVtxZ(0),
  trueBResPx(0), trueBResPy(0), trueBResPz(0), trueBResDecayVtxX(0), trueBResDecayVtxY(0), trueBResDecayVtxZ(0),
  trueVPx(0), trueVPy(0), trueVPz(0), trueVDecayVtxX(0), trueVDecayVtxY(0), trueVDecayVtxZ(0),
  trueJPx(0), trueJPy(0), trueJPz(0), trueJDecayVtxX(0), trueJDecayVtxY(0), trueJDecayVtxZ(0),
  trueMumPx(0), trueMumPy(0), trueMumPz(0), trueMumD0(0), trueMupPx(0), trueMupPy(0), trueMupPz(0), trueMupD0(0),
  trueVTrkpPx(0), trueVTrkpPy(0), trueVTrkpPz(0), trueVTrkpD0(0),
  trueVTrkmPx(0), trueVTrkmPy(0), trueVTrkmPz(0), trueVTrkmD0(0),
  trueBResTrkPx(0), trueBResTrkPy(0), trueBResTrkPz(0), trueBResTrkD0(0), trueBResTrkChg(0),
  truthMatch(0), truthLambda(0), truthPsi(0), prompt(0)

{
   //now do what ever initialization is needed
}


PATLambdaB2JpsiLambda::~PATLambdaB2JpsiLambda()
{
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  
}


//
// member functions
//

// ------------ method called to for each event  ------------
void


PATLambdaB2JpsiLambda::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
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

   //  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   // try reconstruction without fitting modules
   //  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   Handle<vector<VertexCompositeCandidate> > theV0Handle;
   iEvent.getByLabel("generalV0Candidates", "Lambda", theV0Handle);

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

       cout << " momentum of iVee from v0producer = " << iVee->momentum() << endl;

       //check momentum and add pion first, proton second
       if (iVee->daughter(0)->momentum().mag2() < iVee->daughter(1)->momentum().mag2()){
	 v0daughters.push_back( *(dynamic_cast<const reco::RecoChargedCandidate *>
				  (iVee->daughter(0))) );
	 v0daughters.push_back( *(dynamic_cast<const reco::RecoChargedCandidate *>
				  (iVee->daughter(1))) );
       } else {
	 v0daughters.push_back( *(dynamic_cast<const reco::RecoChargedCandidate *>
				  (iVee->daughter(1))) );
	 v0daughters.push_back( *(dynamic_cast<const reco::RecoChargedCandidate *>
				  (iVee->daughter(0))) );
       }
       for(unsigned int j = 0; j < v0daughters.size(); ++j) {
	 theDaughterTracks.push_back(v0daughters[j].track());
       }
       cout << "starting muon collection loop with " << thePATMuonHandle->size() << " muons" << endl;
       for ( std::vector<pat::Muon>::const_iterator iMuonP = thePATMuonHandle->begin();
	     iMuonP != thePATMuonHandle->end(); ++iMuonP ) {
	 //check for mu+ first
	 if (iMuonP->charge() == 1) {
	   TrackRef glbTrackP = iMuonP->track();
	   //cout << "got track" << endl;
	   if ( glbTrackP.isNull() ) {
	     cout << "continue due to no track ref" << endl;
	     continue;
	   }
	   bool match = false;
	   for(unsigned int j = 0; j < theDaughterTracks.size(); ++j) {
	     if (glbTrackP->charge() == theDaughterTracks[j]->charge() && 
		 glbTrackP->momentum() == theDaughterTracks[j]->momentum() ) {
	       std::cout << "Match found between muP and V0 track early with p = " << glbTrackP->momentum() << " and " << theDaughterTracks[j]->momentum() << endl;
	       match = true;
	     }
	     if (match) break;
	   }
	   if (match) { 
	     std::cout << "Match found between muP and V0 track" << endl;
	     continue; 
	   } // Track is already used in making the V0

	   //next check for mu-
	   for ( std::vector<pat::Muon>::const_iterator iMuonM = thePATMuonHandle->begin();
		 iMuonM != thePATMuonHandle->end(); ++iMuonM ) {
	     if (iMuonM->charge() == -1) {

	       TrackRef glbTrackM = iMuonM->track();
	       if ( glbTrackM.isNull() ) {
		 cout << "continue from no track ref" << endl;
		 continue;
	       }
	       //check that neither muon is the same track as one of the pion tracks
	       for(unsigned int j = 0; j < theDaughterTracks.size(); ++j) {
		 if (glbTrackM->charge() == theDaughterTracks[j]->charge() && 
		     glbTrackM->momentum() == theDaughterTracks[j]->momentum() ) {
		   std::cout << "Match found between muM and V0 track early with p = " << glbTrackM->momentum() << " and " << theDaughterTracks[j]->momentum() << endl;
		   match = true;
		 }
		 if (match) break;
	       }
	       if (match) { 
		 std::cout << "Match found between muM and V0 track" << endl;;
		 continue; 
	       } // Track is already used in making the V0
	       //have two good oppositely charged muons and 2 pions. try to vertex them
	       cout << "have 4 good tracks including good oppositely charged muons. " << endl;

	       cout << "size of theDaughterTracks = " << theDaughterTracks.size() << endl;

	       TransientTrack pionTT(theDaughterTracks[0], &(*bFieldHandle) );
	       TransientTrack protonTT(theDaughterTracks[1], &(*bFieldHandle) );
	       TransientTrack muon1TT(glbTrackP, &(*bFieldHandle) );
	       TransientTrack muon2TT(glbTrackM, &(*bFieldHandle) );


	       //Creating a KinematicParticleFactory
	       KinematicParticleFactoryFromTransientTrack pFactory;
	       
	       //The mass of a muon and the insignificant mass sigma 
	       //to avoid singularities in the covariance matrix.
	       ParticleMass muon_mass = 0.10565837; //pdg mass
	       ParticleMass pion_mass = 0.13957018;
	       ParticleMass proton_mass = 0.938272013;
	       ParticleMass lambda_mass = 1.115683;
	       ParticleMass psi_mass = 3.096916;
	       float muon_sigma = muon_mass*1.e-6;
	       float pion_sigma = pion_mass*1.e-6;
	       float proton_sigma = proton_mass*1.e-6;
	       float lambda_sigma = 0.000006;

	       //initial chi2 and ndf before kinematic fits.
	       float chi = 0.;
	       float ndf = 0.;
	       vector<RefCountedKinematicParticle> VeeParticles;
	       vector<RefCountedKinematicParticle> muonParticles;
	       VeeParticles.push_back(pFactory.particle(pionTT,pion_mass,chi,ndf,pion_sigma));
	       VeeParticles.push_back(pFactory.particle(protonTT,proton_mass,chi,ndf,proton_sigma));
	       muonParticles.push_back(pFactory.particle(muon1TT,muon_mass,chi,ndf,muon_sigma));
	       muonParticles.push_back(pFactory.particle(muon2TT,muon_mass,chi,ndf,muon_sigma));

	       KinematicParticleVertexFitter fitter;   
	       RefCountedKinematicTree veeVertexFitTree;
	       veeVertexFitTree = fitter.fit(VeeParticles); 
	       if (!veeVertexFitTree->isValid()) {
		 std::cout << "invalid vertex from the ks vertex fit" << std::endl;
		 continue; 
	       }
	       veeVertexFitTree->movePointerToTheTop();

	       RefCountedKinematicParticle vee_vFit_noMC = veeVertexFitTree->currentParticle();
	       RefCountedKinematicVertex vee_vFit_vertex_noMC = veeVertexFitTree->currentDecayVertex();

	       veeVertexFitTree->movePointerToTheFirstChild();
	       RefCountedKinematicParticle veePi1 = veeVertexFitTree->currentParticle();
	       veeVertexFitTree->movePointerToTheNextChild();
	       RefCountedKinematicParticle veePi2 = veeVertexFitTree->currentParticle();

	       KinematicParameters veePi1KP = veePi1->currentState().kinematicParameters();
	       KinematicParameters veePi2KP = veePi2->currentState().kinematicParameters();
	       KinematicParameters veePipKP;
	       KinematicParameters veePimKP;

	       if ( veePi1->currentState().particleCharge() > 0 ) veePipKP = veePi1KP;
	       if ( veePi1->currentState().particleCharge() < 0 ) veePimKP = veePi1KP;
	       if ( veePi2->currentState().particleCharge() > 0 ) veePipKP = veePi2KP;
	       if ( veePi2->currentState().particleCharge() < 0 ) veePimKP = veePi2KP;	       

	       RefCountedKinematicTree psiVertexFitTree;
	       psiVertexFitTree = fitter.fit(muonParticles); 
	       if (!psiVertexFitTree->isValid()) {
		 std::cout << "caught an exception in the psi vertex fit" << std::endl;
		 continue; 
	       }
	       psiVertexFitTree->movePointerToTheTop();
	       
	       RefCountedKinematicParticle psi_vFit_noMC = psiVertexFitTree->currentParticle();
	       RefCountedKinematicVertex psi_vFit_vertex_noMC = psiVertexFitTree->currentDecayVertex();

	       // Do MC for Vee and JPsi cands and do mass constrained vertex fit
	       // creating the constraint with a small sigma to put in the resulting covariance 
	       // matrix in order to avoid singularities

	       KinematicParticleFitter csFitterVee;
	       KinematicConstraint * lam_c = new MassKinematicConstraint(lambda_mass,lambda_sigma);
	       // add mass constraint to the lambda fit to do a constrained fit:  
 
	       veeVertexFitTree = csFitterVee.fit(lam_c,veeVertexFitTree);
	       if (!veeVertexFitTree->isValid()){
		 std::cout << "caught an exception in the lambda mass constraint fit" << std::endl;
		 continue; 
	       }
	       
	       veeVertexFitTree->movePointerToTheTop();
	       RefCountedKinematicParticle vee_vFit_withMC = veeVertexFitTree->currentParticle();

	       vector<RefCountedKinematicParticle> vFitMCParticles;
	       vFitMCParticles.push_back(pFactory.particle(muon1TT,muon_mass,chi,ndf,muon_sigma));
	       vFitMCParticles.push_back(pFactory.particle(muon2TT,muon_mass,chi,ndf,muon_sigma));	       
	       vFitMCParticles.push_back(vee_vFit_withMC);
	       MultiTrackKinematicConstraint *  j_psi_c = new  TwoTrackMassKinematicConstraint(psi_mass);
	       KinematicConstrainedVertexFitter kcvFitter;
	       RefCountedKinematicTree vertexFitTree = kcvFitter.fit(vFitMCParticles, j_psi_c);
	       vertexFitTree = fitter.fit(vFitMCParticles);  
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

	       if ( bDecayVertexMC->chiSquared()<0 || bDecayVertexMC->chiSquared()>1000 ) {
		 cout << " failed chi2 cut in MC fit" << endl;
		 continue;
	       }

	       if ( bCandMC->currentState().mass() > 10 ) continue;

	       // get children from final B fit
	       vertexFitTree->movePointerToTheFirstChild();
	       RefCountedKinematicParticle mu1CandMC = vertexFitTree->currentParticle();
	       vertexFitTree->movePointerToTheNextChild();
	       RefCountedKinematicParticle mu2CandMC = vertexFitTree->currentParticle();
	       vertexFitTree->movePointerToTheNextChild();
	       RefCountedKinematicParticle ksCandMC = vertexFitTree->currentParticle();

	       // get mu+ and mu- momenta from final B fit
	       KinematicParameters psiMu1KP = mu1CandMC->currentState().kinematicParameters();
	       KinematicParameters psiMu2KP = mu2CandMC->currentState().kinematicParameters();
	       KinematicParameters psiMupKP;
	       KinematicParameters psiMumKP;
	       
	       if ( mu1CandMC->currentState().particleCharge() > 0 ) psiMupKP = psiMu1KP;
	       if ( mu1CandMC->currentState().particleCharge() < 0 ) psiMumKP = psiMu1KP;
	       if ( mu2CandMC->currentState().particleCharge() > 0 ) psiMupKP = psiMu2KP;
	       if ( mu2CandMC->currentState().particleCharge() < 0 ) psiMumKP = psiMu2KP;	 

	       // get batchlor pion momentum from final B fit
	       KinematicParameters VCandKP = ksCandMC->currentState().kinematicParameters();

	       // fill candidate variables now
	       bMass->push_back(bCandMC->currentState().mass());
	       bPx->push_back(bCandMC->currentState().globalMomentum().x());
	       bPy->push_back(bCandMC->currentState().globalMomentum().y());
	       bPz->push_back(bCandMC->currentState().globalMomentum().z());
	       
	       bPxE->push_back( sqrt( bCandMC->currentState().kinematicParametersError().matrix()(3,3) ) );
	       bPyE->push_back( sqrt( bCandMC->currentState().kinematicParametersError().matrix()(4,4) ) );
	       bPzE->push_back( sqrt( bCandMC->currentState().kinematicParametersError().matrix()(5,5) ) );
	       
	       
	       //vee_vFit_noMC->currentState().kinematicParametersError().matrix()(3,3);   //7x7 matrix: (x,y,z,p_x,p_y,p_z,m)
	       
	       //(*ffBMCVtxC2).push_back(bDecayVertexMC->chiSquared());
	       //cout << "chi2 = " << bDecayVertexMC->chiSquared() << " and dof = " << bDecayVertexMC->degreesOfFreedom() << endl;
	       bVtxCL->push_back( ChiSquaredProbability((double)(bDecayVertexMC->chiSquared()),(double)(bDecayVertexMC->degreesOfFreedom())) );
	       bDecayVtxX->push_back((*bDecayVertexMC).position().x());
	       bDecayVtxY->push_back((*bDecayVertexMC).position().y());
	       bDecayVtxZ->push_back((*bDecayVertexMC).position().z());
	       bDecayVtxXE->push_back( sqrt((*bDecayVertexMC).error().cxx()) );
	       bDecayVtxYE->push_back( sqrt((*bDecayVertexMC).error().cyy()) );
	       bDecayVtxZE->push_back( sqrt((*bDecayVertexMC).error().czz()) );
	       
	       VDecayVtxX->push_back( vee_vFit_vertex_noMC->position().x() );
	       VDecayVtxY->push_back( vee_vFit_vertex_noMC->position().y() );
	       VDecayVtxZ->push_back( vee_vFit_vertex_noMC->position().z() );
	       
	       VDecayVtxXE->push_back( sqrt(vee_vFit_vertex_noMC->error().cxx()) );
	       VDecayVtxYE->push_back( sqrt(vee_vFit_vertex_noMC->error().cyy()) );
	       VDecayVtxZE->push_back( sqrt(vee_vFit_vertex_noMC->error().czz()) );
	       VVtxCL->push_back( ChiSquaredProbability((double)(vee_vFit_vertex_noMC->chiSquared()),(double)(vee_vFit_vertex_noMC->degreesOfFreedom())) );
	       
	       VPx->push_back( VCandKP.momentum().x() );
	       VPy->push_back( VCandKP.momentum().y() );
	       VPz->push_back( VCandKP.momentum().z() );
	       
	       VMass->push_back( vee_vFit_noMC->currentState().mass() );   
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

	       VTrkpPx->push_back(veePipKP.momentum().x());
	       VTrkpPy->push_back(veePipKP.momentum().y());
	       VTrkpPz->push_back(veePipKP.momentum().z());
	       VTrkpMass->push_back(veePipKP.mass());
	       if ( theDaughterTracks[0]->charge() > 0 ) {
		 VTrkpD0->push_back( theDaughterTracks[0]->d0() );
		 VTrkpD0E->push_back( theDaughterTracks[0]->d0Error() ); 
	       } else {
		 VTrkpD0->push_back( theDaughterTracks[1]->d0() );
		 VTrkpD0E->push_back( theDaughterTracks[1]->d0Error() ); 
	       }

	       VTrkmPx->push_back(veePimKP.momentum().x());
	       VTrkmPy->push_back(veePimKP.momentum().y());
	       VTrkmPz->push_back(veePimKP.momentum().z());
	       VTrkmMass->push_back(veePimKP.mass());
	       if ( theDaughterTracks[0]->charge() < 0 ) {
		 VTrkmD0->push_back( theDaughterTracks[0]->d0() );
		 VTrkmD0E->push_back( theDaughterTracks[0]->d0Error() ); 
	       } else {
		 VTrkmD0->push_back( theDaughterTracks[1]->d0() );
		 VTrkmD0E->push_back( theDaughterTracks[1]->d0Error() ); 
	       }

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
	       
	       //calculate most probable value with beamspot constraint for ctau in 3D

	       AlgebraicMatrix31 pB;
	       pB(0,0) = bCandMC->currentState().globalMomentum().x();
	       pB(1,0) = bCandMC->currentState().globalMomentum().y();
	       pB(2,0) = bCandMC->currentState().globalMomentum().z();

	       AlgebraicMatrix13 pBT;
	       pBT(0,0) = bCandMC->currentState().globalMomentum().x();
	       pBT(0,1) = bCandMC->currentState().globalMomentum().y();
	       pBT(0,2) = bCandMC->currentState().globalMomentum().z();

	       AlgebraicMatrix31 PVBS;
	       PVBS(0,0) = bestVtxBS->position().x();
	       PVBS(0,1) = bestVtxBS->position().y();
	       PVBS(0,2) = bestVtxBS->position().z();
	       AlgebraicMatrix31 BV;
	       BV(0,0) = bDecayVertexMC->position().x();
	       BV(0,1) = bDecayVertexMC->position().y();
	       BV(0,2) = bDecayVertexMC->position().z();
	       AlgebraicMatrix31 lxyzBS = BV-PVBS;
	       AlgebraicMatrix33 PVBSError(bestVtxBS->error());
	       AlgebraicMatrix33 BVError(bDecayVertexMC->error().matrix_new());
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

	       nB++;
	       
	       VeeParticles.clear();
	       muonParticles.clear();
	       vFitMCParticles.clear();
	       
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
     iEvent.getByLabel("genParticlesPlusSim",genParticles);
     
     genKsPsi = -1; genKstarpPsi = -1; genLambdaPsi = -1; prompt = 1; feedup = -1; feeddown = -1;
     
     for( size_t k = 0; k < genParticles->size(); k++ ) {
       // check if any of our signals were generated
       
       const Candidate & BCand = (*genParticles)[ k ];
       
       if ( abs(BCand.pdgId())==511 && abs(BCand.daughter(0)->pdgId())!=511 ) {
	 
	 //look for the B
	 cout << "found B0";
	 int ipsi(-1), iks(-1);
	 bool wrong = false;
	 for ( uint i = 0; i < BCand.numberOfDaughters(); i++){
	   // check B0 for psi and ks daughters
	   const Candidate * genDau = BCand.daughter(i);
	   cout << " =" << genDau->pdgId();
	   int imu1(-1), imu2(-1),  ipi1(-1), ipi2(-1);
	   for ( uint j = 0; j < genDau->numberOfDaughters(); j++){
	     const Candidate * genGDau = genDau->daughter(j);
	     cout << " ==" << genGDau->pdgId();
	     // here also check for nuclear interactions
	     if ( genGDau->pdgId()==13 && (abs(genDau->pdgId())==443) ) imu1 = j;
	     if ( genGDau->pdgId()==-13 && (abs(genDau->pdgId())==443) ) imu2 = j;
	     if ( genGDau->pdgId()==211 && (abs(genDau->pdgId())==310) ) ipi1 = j;
	     if ( genGDau->pdgId()==-211 && (abs(genDau->pdgId())==310) ) ipi2 = j;
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
	 if (ipsi!=-1&&iks!=-1) {
	   genKsPsi = 1;
	   cout << " found genKsPsi";
	 }	 
	 cout << endl;
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
	   cout << " found genKstarpPsi" << endl;
	   genKstarpPsi =1;
	 }
	 cout << endl;
       } //check for genparticle with id = 521 for a B+
       
       // check for Lambda_b
       // watch for nuclear interactions!
       if ( abs(BCand.pdgId())==5122 ) {
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
	     if ( abs(genDau->daughter(j)->pdgId())==211 && abs(genDau->pdgId())==3122 ) ipi = j;
	     if ( abs(genDau->daughter(j)->pdgId())==2212 && abs(genDau->pdgId())==3122 ) ip = j;
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
	 cout << endl;
	 if (ipsi!=-1&&ilam!=-1) {
	   cout << " setting genLambdaPsi = 1" << endl;
	   genLambdaPsi = 1;

	   //fill MC truth info

	   //write out info from daughters
	   const Candidate * genpsi =  BCand.daughter(ipsi);
	   const Candidate * genlam =  BCand.daughter(ilam);
	   
	   truePriVtxX = BCand.vx();
	   truePriVtxY = BCand.vy();
	   truePriVtxZ = BCand.vz();
	   
	   trueBPx = BCand.px();
	   trueBPy = BCand.py();
	   trueBPz = BCand.pz();
	   
	   trueJPx = genpsi->px();
	   trueJPy = genpsi->py();
	   trueJPz = genpsi->pz();
	   trueBDecayVtxX = genpsi->vx();
	   trueBDecayVtxY = genpsi->vy();
	   trueBDecayVtxZ = genpsi->vz();
	   
	   for (uint j=0; j<genpsi->numberOfDaughters(); j++) {
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
	   
	   trueVPx = genlam->px();
	   trueVPy = genlam->py();
	   trueVPz = genlam->pz();
	   
	   cout << "starting daughters loop with genlam id = " << genlam->pdgId() << " and with " << genlam->numberOfDaughters() << " daughters" << endl;
	   
	   for (uint j=0; j<genlam->numberOfDaughters(); j++) {
	     
	     cout << "genlam daughter has id = " << genlam->daughter(j)->pdgId() << " with ndau = " << genlam->daughter(j)->numberOfDaughters() << endl;
	     cout << "no granddaughters, filling genlam info" << endl;
	     if ( genlam->daughter(j)->charge()>0 ) {
	       cout << "filling V0 track truth for signal candiate" << endl;
	       trueVTrkpPx = genlam->daughter(j)->px();
	       trueVTrkpPy = genlam->daughter(j)->py();
	       trueVTrkpPz = genlam->daughter(j)->pz();
	       trueVDecayVtxX = genlam->daughter(j)->vx();
	       trueVDecayVtxY = genlam->daughter(j)->vy();
	       trueVDecayVtxZ = genlam->daughter(j)->vz();
	     }
	     if ( genlam->daughter(j)->charge()<0 ) {
	       trueVTrkmPx = genlam->daughter(j)->px();
	       trueVTrkmPy = genlam->daughter(j)->py();
	       trueVTrkmPz = genlam->daughter(j)->pz();
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
	   float RcutPi = 0.20;
	   float RcutVtx = 20.;	   

	   truthMatch.clear(); truthLambda.clear(); truthPsi.clear();

	   for (uint i = 0; i<mupPx->size(); i++) {
	     //loop to check all B candidates found

	     bool istrueMup = false;
	     bool istrueMum = false;
	     bool istruePip = false;
	     bool istruePim = false;
	     bool istrueLambda = false;
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
	     float deltaRlamvtx = sqrt( (trueVDecayVtxX - VDecayVtxX->at(i))*
				       (trueVDecayVtxX - VDecayVtxX->at(i)) +
				       (trueVDecayVtxY - VDecayVtxY->at(i))*
				       (trueVDecayVtxY - VDecayVtxY->at(i)) +
				       (trueVDecayVtxZ - VDecayVtxZ->at(i))*
				       (trueVDecayVtxZ - VDecayVtxZ->at(i)) );	
	     
	     if ( istrueMup & istrueMum ) istruePsi = true;
	     if ( istruePip & istruePim && (deltaRlamvtx<RcutVtx)) istrueLambda = true;
	     if ( istruePsi & istrueLambda ) istrueB = true;

	     if (istruePsi) {
	       cout << "true Psi from reco from cand " << i << endl;
	       truthPsi.push_back(1);
	     } else truthPsi.push_back(-1);
	     if (istrueLambda) {
	       cout << "true Lambda from reco from cand " << i << endl;
	       truthLambda.push_back(1);
	     } else truthLambda.push_back(-1);
	     if (istrueB) {
	       cout << "true B from reco from cand " << i << endl;
	       truthMatch.push_back(1);
	     }    else truthMatch.push_back(-1);
	     
	   }
	   

	  






	   
	 } // if true signal event was generated
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

       // check for JPsiLambda feed up // currently from B+->JPsi K1+(1270) or B+->JPsi K+
       if (abs(BCand.pdgId())==521) {
	 bool psidau = false;
	 bool kdau = false;
	 bool k1pdau = false;
	 for ( uint i = 0; i < BCand.numberOfDaughters(); i++){
	   const Candidate * genDau = BCand.daughter(i);
	   if ( genDau->pdgId()==443 ) psidau = true;
	   if ( abs(genDau->pdgId())==321 ) kdau = true;
	   if ( abs(genDau->pdgId())==10323 ) k1pdau = true;
	 }
	 if ( psidau && kdau ) feedup = 1;
	 if ( psidau && k1pdau ) feedup = 2;
       }
       
       // and from B0->JPsi K10(1270), B0->JPsi K*0(892), B0->JPsi Ks
       if (abs(BCand.pdgId())==511) {
	 bool psidau = false;
	 bool ksdau = false;
	 bool kst0dau = false;
	 bool k10dau = false;
	 bool pipGDau = false;
	 bool pizGDau = false;
	 for ( uint i = 0; i < BCand.numberOfDaughters(); i++){
	   const Candidate * genDau = BCand.daughter(i);
	   if ( genDau->pdgId()==443 ) psidau = true;
	   if ( abs(genDau->pdgId())==310 ||  abs(genDau->pdgId())==311 ) ksdau = true;
	   if ( abs(genDau->pdgId())==313 ) {
	     kst0dau = true;
	     for ( uint j = 0; j < genDau->numberOfDaughters(); j++){
	       const Candidate * genGDau = genDau->daughter(j);
	       if ( abs(genGDau->pdgId())==211 ) pipGDau = true;
	       if ( abs(genGDau->pdgId())==111 ) pizGDau = true;
	     }    
	   }
	   if ( abs(genDau->pdgId())==10313 ) k10dau = true;
	 }
	 if ( psidau && ksdau ) feedup = 3;
	 if ( psidau && kst0dau && pipGDau ) feedup = 4;
	 if ( psidau && kst0dau && pizGDau ) feedup = 5;
	 if ( psidau && k10dau ) feedup = 6;
       }

       // check feeddown from Lambda_B->Psi(2S) Lambda
       if (abs(BCand.pdgId())==5122) {
	 bool psi2Sdau = false;
	 bool lambdadau = false;
	 for ( uint i = 0; i < BCand.numberOfDaughters(); i++){
	   const Candidate * genDau = BCand.daughter(i);
	   if ( genDau->pdgId()==100443 ) psi2Sdau = true;
	   if ( abs(genDau->pdgId())==3122 ) lambdadau = true;
	 }
	 if ( psi2Sdau && lambdadau ) feeddown = 1;
       }

       if (abs(BCand.pdgId())==511) {
	 bool chic1dau = false;
	 bool ksdau = false;
	 for ( uint i = 0; i < BCand.numberOfDaughters(); i++){
	   const Candidate * genDau = BCand.daughter(i);
	   if ( abs(genDau->pdgId())==20443 ) chic1dau = true;
	   if ( abs(genDau->pdgId())==310 ) ksdau = true;
	 }
	 if ( chic1dau && ksdau ) feeddown = 2;
       }

       if (abs(BCand.pdgId())==521) {
	 bool psidau = false;
	 bool kstpdau = false;
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
	 }
	 if ( psidau && kstpdau && pipGDau ) feeddown = 3; 
	 if ( psidau && kstpdau && kpGDau ) feeddown = 4;
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
     
     //fill truthMatch with zeros if we didn't fidn a truth match
     if ( truthMatch.size()==0 ) { //if no signal event has been found, fill with zeros
       for (uint i = 0; i<mupPx->size(); i++) {
	 truthPsi.push_back(0);
	 truthLambda.push_back(0);
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
   bPxE->clear(); bPyE->clear(); bPzE->clear();
   bctauMPVBS->clear();
   bDecayVtxX->clear(); bDecayVtxY->clear(); bDecayVtxZ->clear(); 
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
   prompt = 0; truthMatch.clear(); truthLambda.clear(); truthPsi.clear();
}

void PATLambdaB2JpsiLambda::fillPsi(const reco::Candidate& genpsi) {
  
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

void PATLambdaB2JpsiLambda::fillV0(const reco::Candidate& genv0) {
  
  for (uint i=0; i<genv0.numberOfDaughters(); i++) {
    if (genv0.daughter(i)->charge()>0 && genv0.numberOfDaughters()==2) {
      cout << "filling V0 truth info for non signal candidate" << endl;
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
PATLambdaB2JpsiLambda::beginJob(const edm::EventSetup&)
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
  tree_->Branch("bDecayVtxX",&bDecayVtxX);
  tree_->Branch("bDecayVtxY",&bDecayVtxY);
  tree_->Branch("bDecayVtxZ",&bDecayVtxZ);
  tree_->Branch("bDecayVtxXE",&bDecayVtxXE);
  tree_->Branch("bDecayVtxYE",&bDecayVtxYE);
  tree_->Branch("bDecayVtxZE",&bDecayVtxZE);
  tree_->Branch("bctauMPVBS",&bctauMPVBS);
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
  tree_->Branch("truthLambda",&truthLambda);
  tree_->Branch("truthPsi",&truthPsi);
}


// ------------ method called once each job just after ending the event loop  ------------
void 
PATLambdaB2JpsiLambda::endJob() {
  tree_->GetDirectory()->cd();
  tree_->Write();
}

//define this as a plug-in
DEFINE_FWK_MODULE(PATLambdaB2JpsiLambda);

