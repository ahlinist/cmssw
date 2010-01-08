// -*- C++ -*-
//
// Package:    PATBu2JpsiKstar
// Class:      PATBu2JpsiKstar
// 
/**\class PATBu2JpsiKstar PATBu2JpsiKstar.cc HeavyFlavorAnalysis/Examples/src/PATBu2JpsiKstar.cc

 Description: <one line class summary>
Make rootTuple for b->s JPsi(mu+mu-) analyses

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Keith Ulmer
//         Created:  Wed May  7 13:15:04 MDT 2008
// $Id: PATBu2JpsiKstar.cc,v 1.4 2009/12/18 22:19:49 kaulmer Exp $
//
//


// system include files
#include <memory>

// user include files
#include "HeavyFlavorAnalysis/Examples/interface/PATBu2JpsiKstar.h"

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
#include "RecoVertex/AdaptiveVertexFit/interface/AdaptiveVertexFitter.h"
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
PATBu2JpsiKstar::PATBu2JpsiKstar(const edm::ParameterSet& iConfig)
  :
  hepMC(iConfig.getUntrackedParameter<std::string>("HepMC",std::string("source")) ),
  hlTriggerResults_(iConfig.getUntrackedParameter<edm::InputTag>("HLTriggerResults",edm::InputTag("TriggerResults::HLT")) ),
  blist(iConfig.getUntrackedParameter("Blist",std::string("Bd"))),
  muonType(iConfig.getUntrackedParameter("muonType",std::string("muons"))),
  vtxSample( iConfig.getUntrackedParameter<std::string>("VtxSample",std::string("offlinePrimaryVertices")) ), 

  tree_(0), l1_mu3(0), l1_2mu3(0), l1_muOpen(0), l1_mu0(0),
  hlt_mu3(0), hlt_mu5(0), hlt_mu7(0), hlt_mu9(0), hlt_2mu0(0), hlt_2mu3(0), hlt_2mu3JPsi(0), hltBJPsiMuMu(0), nB(0),
  priVtxX(0), priVtxY(0), priVtxZ(0), priVtxXE(0), priVtxYE(0), priVtxZE(0), priVtxCL(0),
  bMass(0), bVtxCL(0), bVtxC2(0), bPx(0), bPy(0), bPz(0), bPxE(0), bPyE(0), bPzE(0), 
  bctauMPVBS(0),
  bDecayVtxX(0), bDecayVtxY(0), bDecayVtxZ(0), bDecayVtxXE(0), bDecayVtxYE(0), bDecayVtxZE(0),
  bResMass(0), bResVtxCL(0), bResPx(0), bResPy(0), bResPz(0),
  bResDecayVtxX(0), bResDecayVtxY(0), bResDecayVtxZ(0), bResDecayVtxXE(0), bResDecayVtxYE(0), bResDecayVtxZE(0),
  VMass(0), VVtxCL(0), VPx(0), VPy(0), VPz(0),
  VDecayVtxX(0), VDecayVtxY(0), VDecayVtxZ(0),
  VDecayVtxXE(0), VDecayVtxYE(0), VDecayVtxZE(0),
  JMass(0), JVtxCL(0), JVtxC2(0), JPx(0), JPy(0), JPz(0),
  JDecayVtxX(0), JDecayVtxY(0), JDecayVtxZ(0), JDecayVtxXE(0), JDecayVtxYE(0), JDecayVtxZE(0),
  mumPx(0), mumPy(0), mumPz(0), mumD0(0), mumD0E(0),
  mupPx(0), mupPy(0), mupPz(0), mupD0(0), mupD0E(0),
  VTrkpMass(0), VTrkpPx(0), VTrkpPy(0), VTrkpPz(0), 
  VTrkpD0(0), VTrkpD0E(0), 
  VTrkmMass(0), VTrkmPx(0), VTrkmPy(0), VTrkmPz(0), 
  VTrkmD0(0), VTrkmD0E(0), 
  bResTrkPx(0), bResTrkPy(0), bResTrkPz(0), 
  bResTrkD0(0), bResTrkD0E(0),bResTrkChg(0), 
  bResTrkC2(0), bResTrkNormC2(0), bResTrkPsiD02D(0), bResTrkPsiD03D(0), bResTrkPsiD02DE(0), bResTrkPsiD03DE(0),
  bResTrkNHits(0), bResTrkNValidHits(0), bResTrkPixelLayers(0), bResTrkQual(0),
  genKsPsi(0), genKstarpPsi(0), genLambdaPsi(0), feedup(0), feeddown(0),

  JVVtxCL(0), JVVtxC2(0), JpiVtxCL(0), JpiVtxC2(0),

  truePriVtxX(0), truePriVtxY(0), truePriVtxZ(0), trueBPx(0), trueBPy(0), trueBPz(0), trueBDecayVtxX(0), trueBDecayVtxY(0), trueBDecayVtxZ(0),
  trueBResPx(0), trueBResPy(0), trueBResPz(0), trueBResDecayVtxX(0), trueBResDecayVtxY(0), trueBResDecayVtxZ(0),
  trueVPx(0), trueVPy(0), trueVPz(0), trueVDecayVtxX(0), trueVDecayVtxY(0), trueVDecayVtxZ(0),
  trueJPx(0), trueJPy(0), trueJPz(0), trueJDecayVtxX(0), trueJDecayVtxY(0), trueJDecayVtxZ(0),
  trueMumPx(0), trueMumPy(0), trueMumPz(0), trueMumD0(0), trueMupPx(0), trueMupPy(0), trueMupPz(0), trueMupD0(0),
  trueVTrkpPx(0), trueVTrkpPy(0), trueVTrkpPz(0), trueVTrkpD0(0),
  trueVTrkmPx(0), trueVTrkmPy(0), trueVTrkmPz(0), trueVTrkmD0(0),
  trueBResTrkPx(0), trueBResTrkPy(0), trueBResTrkPz(0), trueBResTrkD0(0), trueBResTrkChg(0),
  prompt(0), truthMatch(0), truthKstp(0), truthKs(0), truthPsi(0)

{
   //now do what ever initialization is needed
}


PATBu2JpsiKstar::~PATBu2JpsiKstar()
{
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  
}


//
// member functions
//

// ------------ method called to for each event  ------------
void


PATBu2JpsiKstar::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
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
   double mpion = 0.13957018;
   double mmuon = 0.10565837;
   //double mks = 0.497614;

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
	       
	       pat::GenericParticle piPATTrack;
	       //next check tracks for pion candidate from K*+ decay and select the one with the best K*0 mass
	       for ( vector<pat::GenericParticle>::const_iterator iTrack = thePATTrackHandle->begin();
		     iTrack != thePATTrackHandle->end(); ++iTrack ) {

		 //first check track doesn't overlap with the Ks or Psi candidate tracks

		 for(unsigned int k = 0; k < theDaughterTracks.size(); ++k) {
		   if (iTrack->charge() == theDaughterTracks[k]->charge() && 
		       iTrack->momentum() == theDaughterTracks[k]->momentum() ) {
		     std::cout << "Match found between pion track and V0 track" << endl;
		     match = true;
		   }
		   if (match) break;
		 }
		 if (match)	{
		   cout << "continuing" << endl;
		   match = false;
		   continue; 
		 }
		 
		 if (iTrack->charge() == glbTrackP->charge() && 
		     iTrack->momentum() == glbTrackP->momentum() ) {
		   std::cout << "Match found between pion track and mu+ track" << endl;
		   match = true;
		 }
		 if (iTrack->charge() == glbTrackM->charge() && 
		     iTrack->momentum() == glbTrackM->momentum() ) {
		   std::cout << "Match found between pion track and mu- track" << endl;
		   match = true;
		 }
		 if (match)	{
		   cout << "continuing" << endl;
		   match = false;
		   continue; 
		 }

		 // skip best Kstar mass selection
		 /*
		 //calculate the K*0 mass to find the best one
		 double VeeE = sqrt( iVee->momentum().Mag2() + mks*mks );
		 double p3E = sqrt( iTrack->momentum().Mag2() + mpion*mpion );
		 
		 float KstMass4vec = sqrt( (VeeE+p3E)*(VeeE+p3E) - 
					   ( (iVee->momentum().x()+iTrack->momentum().x()) * (iVee->momentum().x()+iTrack->momentum().x()) +
					     (iVee->momentum().y()+iTrack->momentum().y()) * (iVee->momentum().y()+iTrack->momentum().y()) +
					     (iVee->momentum().z()+iTrack->momentum().z()) * (iVee->momentum().z()+iTrack->momentum().z()) ) );
		 
		 if ( abs( KstMass4vec-KstMass ) < best_KstMassDiff ) {
		   //cout << "found new best K*0 mass = "<< KstMass4vec << " with new mass diff = " << abs( KstMass4vec-KstMass ) << " while old was " << best_KstMassDiff << endl;
		   best_KstMassDiff = abs( KstMass4vec-KstMass );
		   piPATTrack = *iTrack;
		 }
	       }
		 */
		 piPATTrack = *iTrack;
	       
	       //have two good oppositely charged muons, 2 pions from Ks and another pion. try to vertex them
	       cout << "have 5 good tracks including good oppositely charged muons. " << endl;
	       
	       TransientTrack pion1TT(theDaughterTracks[0], &(*bFieldHandle) );
	       TransientTrack pion2TT(theDaughterTracks[1], &(*bFieldHandle) );
	       TransientTrack muon1TT(glbTrackP, &(*bFieldHandle) );
	       TransientTrack muon2TT(glbTrackM, &(*bFieldHandle) );
	       TransientTrack pion3TT(piPATTrack.track(), &(*bFieldHandle) );
	       
	       double p1E = sqrt( theDaughterTracks[0]->momentum().x()*theDaughterTracks[0]->momentum().x() + 
				  theDaughterTracks[0]->momentum().y()*theDaughterTracks[0]->momentum().y() + 
				  theDaughterTracks[0]->momentum().z()*theDaughterTracks[0]->momentum().z() + 
				  mpion*mpion );
	       double p2E = sqrt( theDaughterTracks[1]->momentum().x()*theDaughterTracks[1]->momentum().x() + 
				  theDaughterTracks[1]->momentum().y()*theDaughterTracks[1]->momentum().y() + 
				  theDaughterTracks[1]->momentum().z()*theDaughterTracks[1]->momentum().z() + 
				  mpion*mpion );
	       double p3E = sqrt( piPATTrack.momentum().x()*piPATTrack.momentum().x() + 
				  piPATTrack.momentum().y()*piPATTrack.momentum().y() + 
				  piPATTrack.momentum().z()*piPATTrack.momentum().z() + 
				  mpion*mpion );
	       double mu1E =  sqrt( glbTrackP->momentum().x()*glbTrackP->momentum().x() + 
				    glbTrackP->momentum().y()*glbTrackP->momentum().y() + 
				    glbTrackP->momentum().z()*glbTrackP->momentum().z() + 
				    mmuon*mmuon );
	       double mu2E =  sqrt( glbTrackM->momentum().x()*glbTrackM->momentum().x() + 
				    glbTrackM->momentum().y()*glbTrackM->momentum().y() + 
				    glbTrackM->momentum().z()*glbTrackM->momentum().z() + 
				    mmuon*mmuon );
	       
	       cout << "Ks mass from 4 vectors = " << sqrt( (p1E+p2E)*(p1E+p2E) - (   (theDaughterTracks[0]->momentum().x()+theDaughterTracks[1]->momentum().x())*(theDaughterTracks[0]->momentum().x()+theDaughterTracks[1]->momentum().x()) + 
										      (theDaughterTracks[0]->momentum().y()+theDaughterTracks[1]->momentum().y())*(theDaughterTracks[0]->momentum().y()+theDaughterTracks[1]->momentum().y()) +  
										      (theDaughterTracks[0]->momentum().z()+theDaughterTracks[1]->momentum().z())*(theDaughterTracks[0]->momentum().z()+theDaughterTracks[1]->momentum().z() ) ) ) << endl;	    	       
	       
	       float KstMass4vec =  sqrt( (p1E+p2E+p3E)*(p1E+p2E+p3E) - 
					  ( (theDaughterTracks[0]->momentum().x()+theDaughterTracks[1]->momentum().x()+piPATTrack.momentum().x())*(theDaughterTracks[0]->momentum().x()+theDaughterTracks[1]->momentum().x()+piPATTrack.momentum().x()) + 
					    (theDaughterTracks[0]->momentum().y()+theDaughterTracks[1]->momentum().y()+piPATTrack.momentum().y())*(theDaughterTracks[0]->momentum().y()+theDaughterTracks[1]->momentum().y()+piPATTrack.momentum().y()) +  
					    (theDaughterTracks[0]->momentum().z()+theDaughterTracks[1]->momentum().z()+piPATTrack.momentum().z())*(theDaughterTracks[0]->momentum().z()+theDaughterTracks[1]->momentum().z()+piPATTrack.momentum().z() ) )
					  );
	       
	       cout << "Kst mass from 4 vector sum = " << KstMass4vec << endl;

	       float KstPx = ( iVee->momentum().x() + piPATTrack.momentum().x() );
	       float KstPy = ( iVee->momentum().y() + piPATTrack.momentum().y() );
	       float KstPz = ( iVee->momentum().z() + piPATTrack.momentum().z() );
	       cout << "kstar px = " << KstPx << ", py = " << KstPy << ", pz = " << KstPz << endl;
	       
	       float BMass4vec =  sqrt( (p1E+p2E+p3E+mu1E+mu2E)*(p1E+p2E+p3E+mu1E+mu2E) - 
					( (theDaughterTracks[0]->momentum().x()+theDaughterTracks[1]->momentum().x()+glbTrackP->momentum().x()+piPATTrack.momentum().x()+glbTrackM->momentum().x())*(theDaughterTracks[0]->momentum().x()+theDaughterTracks[1]->momentum().x()+piPATTrack.momentum().x()+glbTrackP->momentum().x()+glbTrackM->momentum().x()) + 
					  (theDaughterTracks[0]->momentum().y()+theDaughterTracks[1]->momentum().y()+glbTrackP->momentum().y()+piPATTrack.momentum().y()+glbTrackM->momentum().y())*(theDaughterTracks[0]->momentum().y()+theDaughterTracks[1]->momentum().y()+piPATTrack.momentum().y()+glbTrackP->momentum().y()+glbTrackM->momentum().y()) +  
					  (theDaughterTracks[0]->momentum().z()+theDaughterTracks[1]->momentum().z()+piPATTrack.momentum().z()+glbTrackP->momentum().z()+glbTrackM->momentum().z())*(theDaughterTracks[0]->momentum().z()+theDaughterTracks[1]->momentum().z()+piPATTrack.momentum().z()+glbTrackP->momentum().z()+glbTrackM->momentum().z() ) )
					);
	       
	       cout << "B mass from 4 vector sum = " << BMass4vec << endl;
	       
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
	       
	       cout << "ks fit done with mass = " << ks_vFit_noMC->currentState().mass() << endl;
	       
	       ksVertexFitTree->movePointerToTheFirstChild();
	       RefCountedKinematicParticle ksPi1 = ksVertexFitTree->currentParticle();
	       ksVertexFitTree->movePointerToTheNextChild();
	       RefCountedKinematicParticle ksPi2 = ksVertexFitTree->currentParticle();
	       
	       KinematicParameters ksPi1KP = ksPi1->currentState().kinematicParameters();
	       KinematicParameters ksPi2KP = ksPi2->currentState().kinematicParameters();
	       KinematicParameters ksPipKP;
	       KinematicParameters ksPimKP;
	       
	       if ( ksPi1->currentState().particleCharge() > 0 ) ksPipKP = ksPi1KP;
	       if ( ksPi1->currentState().particleCharge() < 0 ) ksPimKP = ksPi1KP;
	       if ( ksPi2->currentState().particleCharge() > 0 ) ksPipKP = ksPi2KP;
	       if ( ksPi2->currentState().particleCharge() < 0 ) ksPimKP = ksPi2KP;
	       
	       RefCountedKinematicTree psiVertexFitTree;
	       psiVertexFitTree = fitter.fit(muonParticles); 
	       if (!psiVertexFitTree->isValid()) {
		 std::cout << "caught an exception in the psi vertex fit" << std::endl;
		 continue; 
	       }
	       psiVertexFitTree->movePointerToTheTop();
	       
	       RefCountedKinematicParticle psi_vFit_noMC = psiVertexFitTree->currentParticle();
	       RefCountedKinematicVertex psi_vFit_vertex_noMC = psiVertexFitTree->currentDecayVertex();

	       // Do MC for Ks and JPsi cands and do mass constrained vertex fit
	       // creating the constraint with a small sigma to put in the resulting covariance 
	       // matrix in order to avoid singularities
	       
	       KinematicParticleFitter csFitterKs;
	       KinematicConstraint * ks_c = new MassKinematicConstraint(ks_mass,ks_sigma);
	       // add mass constraint to the ks fit to do a constrained fit:  
	       
	       ksVertexFitTree = csFitterKs.fit(ks_c,ksVertexFitTree);
	       if (!ksVertexFitTree->isValid()){
		 std::cout << "caught an exception in the ks mass constraint fit" << std::endl;
		 continue; 
	       }
	       
	       ksVertexFitTree->movePointerToTheTop();
	       RefCountedKinematicParticle ks_vFit_withMC = ksVertexFitTree->currentParticle();
	       RefCountedKinematicVertex ks_vFit_vertex_withMC = ksVertexFitTree->currentDecayVertex();
	       
	       vector<RefCountedKinematicParticle> vFitMCParticles;
	       vFitMCParticles.push_back(pFactory.particle(muon1TT,muon_mass,chi,ndf,muon_sigma));
	       vFitMCParticles.push_back(pFactory.particle(muon2TT,muon_mass,chi,ndf,muon_sigma));
	       vFitMCParticles.push_back(pFactory.particle(pion3TT,pion_mass,chi,ndf,pion_sigma));
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
	       
	       cout << "B mass from fit = " << bCandMC->currentState().mass() << endl;

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
	       RefCountedKinematicParticle piCandMC = vertexFitTree->currentParticle();
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
	       KinematicParameters piCandKP = piCandMC->currentState().kinematicParameters();
	       KinematicParameters VCandKP = ksCandMC->currentState().kinematicParameters();

	       // do JPsi plus pi from Kst fit to check goodness of candidate
	       vector<RefCountedKinematicParticle> muonpiParticles;
	       muonpiParticles.push_back(pFactory.particle(muon1TT,muon_mass,chi,ndf,muon_sigma));
	       muonpiParticles.push_back(pFactory.particle(muon2TT,muon_mass,chi,ndf,muon_sigma));
	       muonpiParticles.push_back(pFactory.particle(pion3TT,pion_mass,chi,ndf,pion_sigma));
	       RefCountedKinematicTree psipiVertexFitTree;
	       psipiVertexFitTree = fitter.fit(muonpiParticles); 
	       if (!psipiVertexFitTree->isValid())	 continue; 
	       psipiVertexFitTree->movePointerToTheTop();
	       RefCountedKinematicVertex psipi_vFit_vertex_noMC = psipiVertexFitTree->currentDecayVertex();

	       // do JPsi plus Ks from Kst fit to check goodness of candidate
	       vector<RefCountedKinematicParticle> muonVParticles;
	       muonVParticles.push_back(pFactory.particle(muon1TT,muon_mass,chi,ndf,muon_sigma));
	       muonVParticles.push_back(pFactory.particle(muon2TT,muon_mass,chi,ndf,muon_sigma));
	       muonVParticles.push_back(ks_vFit_withMC);
	       RefCountedKinematicTree psiVVertexFitTree;
	       psiVVertexFitTree = fitter.fit(muonVParticles); 
	       if (!psiVVertexFitTree->isValid())	 continue; 
	       psiVVertexFitTree->movePointerToTheTop();
	       RefCountedKinematicVertex psiV_vFit_vertex_noMC = psiVVertexFitTree->currentDecayVertex();

	       // fill candidate variables now
	       bMass->push_back(bCandMC->currentState().mass());
	       bPx->push_back(bCandMC->currentState().globalMomentum().x());
	       bPy->push_back(bCandMC->currentState().globalMomentum().y());
	       bPz->push_back(bCandMC->currentState().globalMomentum().z());
	       
	       bPxE->push_back( sqrt( bCandMC->currentState().kinematicParametersError().matrix()(3,3) ) );
	       bPyE->push_back( sqrt( bCandMC->currentState().kinematicParametersError().matrix()(4,4) ) );
	       bPzE->push_back( sqrt( bCandMC->currentState().kinematicParametersError().matrix()(5,5) ) );
	       
	       //ks_vFit_noMC->currentState().kinematicParametersError().matrix()(3,3);   //7x7 matrix: (x,y,z,p_x,p_y,p_z,m)
	       
	       bVtxCL->push_back( ChiSquaredProbability((double)(bDecayVertexMC->chiSquared()),(double)(bDecayVertexMC->degreesOfFreedom())) );
	       bVtxC2->push_back( bDecayVertexMC->chiSquared() );
	       bDecayVtxX->push_back((*bDecayVertexMC).position().x());
	       bDecayVtxY->push_back((*bDecayVertexMC).position().y());
	       bDecayVtxZ->push_back((*bDecayVertexMC).position().z());
	       bDecayVtxXE->push_back(sqrt((*bDecayVertexMC).error().cxx()));
	       bDecayVtxYE->push_back(sqrt((*bDecayVertexMC).error().cyy()));
	       bDecayVtxZE->push_back(sqrt((*bDecayVertexMC).error().czz()));
	       
	       VMass->push_back( ks_vFit_noMC->currentState().mass() );   
	       //bResMass->push_back( kstCand->currentState().mass() );
	       bResMass->push_back(KstMass4vec);
	       
	       //cout << "difference between Kst mass from fit and 4 vector = " << KstMass4vec-kstCand->currentState().mass() << endl;
	       
	       JMass->push_back( psi_vFit_noMC->currentState().mass() );
	       
	       JDecayVtxX->push_back( psi_vFit_vertex_noMC->position().x() );
	       JDecayVtxY->push_back( psi_vFit_vertex_noMC->position().y() );
	       JDecayVtxZ->push_back( psi_vFit_vertex_noMC->position().z() );
	       
	       JDecayVtxXE->push_back( sqrt(psi_vFit_vertex_noMC->error().cxx()) );
	       JDecayVtxYE->push_back( sqrt(psi_vFit_vertex_noMC->error().cyy()) );
	       JDecayVtxZE->push_back( sqrt(psi_vFit_vertex_noMC->error().czz()) );
	       JVtxCL->push_back( ChiSquaredProbability((double)(psi_vFit_vertex_noMC->chiSquared()),(double)(psi_vFit_vertex_noMC->degreesOfFreedom())) );
	       JVtxC2->push_back( psi_vFit_vertex_noMC->chiSquared() );

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

	       // fill test candidate vertex chi2 variables here
	       JpiVtxCL->push_back( ChiSquaredProbability((double)(psipi_vFit_vertex_noMC->chiSquared()),(double)(psipi_vFit_vertex_noMC->degreesOfFreedom())) );
	       JpiVtxC2->push_back( psipi_vFit_vertex_noMC->chiSquared() );

	       JVVtxCL->push_back( ChiSquaredProbability((double)(psiV_vFit_vertex_noMC->chiSquared()),(double)(psiV_vFit_vertex_noMC->degreesOfFreedom())) );
	       JVVtxC2->push_back( psiV_vFit_vertex_noMC->chiSquared() );

	       bResPx->push_back( VCandKP.momentum().x() + piCandKP.momentum().x() );
	       bResPy->push_back( VCandKP.momentum().y() + piCandKP.momentum().y() );
	       bResPz->push_back( VCandKP.momentum().z() + piCandKP.momentum().z() );

	       cout << "Kst momentum from final fit = (" << VCandKP.momentum().x() + piCandKP.momentum().x() << "," << VCandKP.momentum().y() + piCandKP.momentum().y() << "," << VCandKP.momentum().z() + piCandKP.momentum().z() << ")" << endl;

	       cout << "kstar momentum from earlier px = " << KstPx << ", py = " << KstPy << ", pz = " << KstPz << endl;

	       bResTrkPx->push_back( piCandKP.momentum().x() );
	       bResTrkPy->push_back( piCandKP.momentum().y() );
	       bResTrkPz->push_back( piCandKP.momentum().z() );
	       bResTrkD0->push_back( piPATTrack.track()->d0() );
	       bResTrkD0E->push_back( piPATTrack.track()->d0Error() );
	       bResTrkChg->push_back( piPATTrack.charge() );

	       bResTrkC2->push_back( piPATTrack.track()->chi2() ); 
	       bResTrkNormC2->push_back( piPATTrack.track()->normalizedChi2() ); 
	       bResTrkNHits->push_back( piPATTrack.track()->numberOfValidHits() + piPATTrack.track()->numberOfLostHits() ); 
	       bResTrkNValidHits->push_back( piPATTrack.track()->numberOfValidHits() ); 
	       bResTrkPixelLayers->push_back( piPATTrack.track()->hitPattern().pixelLayersWithMeasurement() ); 

	       GlobalPoint psiVertex =	psi_vFit_vertex_noMC->position();
	       TrajectoryStateClosestToPoint posTSCP = pion3TT.trajectoryStateClosestToPoint( psiVertex );
	       if ( posTSCP.isValid() ) {
		 double tip = posTSCP.perigeeParameters().transverseImpactParameter();
		 double lip = posTSCP.perigeeParameters().longitudinalImpactParameter();
		 double ip3D = sqrt( tip*tip + lip*lip);
		 bResTrkPsiD02D->push_back( tip );
		 bResTrkPsiD03D->push_back( ip3D );
		 if ( posTSCP.hasError() ) {
		   double tipe = posTSCP.perigeeError().transverseImpactParameterError();
		   double lipe = posTSCP.perigeeError().longitudinalImpactParameterError();
		   double ip3De = sqrt( tipe*tipe + lipe*lipe );
		   bResTrkPsiD02DE->push_back( tipe );
		   bResTrkPsiD03DE->push_back( ip3De );
		 } else {
		   bResTrkPsiD02DE->push_back( -999.0 );
		   bResTrkPsiD03DE->push_back( -999.0 );
		 }
	       } else {
		 bResTrkPsiD02D->push_back( -999.0 );
		 bResTrkPsiD03D->push_back( -999.0 );
		 bResTrkPsiD02DE->push_back( -999.0 );
		 bResTrkPsiD03DE->push_back( -999.0 );
	       }
	       
	       //Track Quality:
	       reco::TrackBase::TrackQuality trackQualityUndef         =  reco::TrackBase::qualityByName("undefQuality");
	       reco::TrackBase::TrackQuality trackQualityLoose         =  reco::TrackBase::qualityByName("loose");
	       reco::TrackBase::TrackQuality trackQualityTight         =  reco::TrackBase::qualityByName("tight");
	       reco::TrackBase::TrackQuality trackQualityhighPur       =  reco::TrackBase::qualityByName("highPurity");
	       int trkQuality  = -1;
	       if(piPATTrack.track()->quality(trackQualityUndef))          { trkQuality = 5; cout << "undef = true" << endl;}
	       if(piPATTrack.track()->quality(trackQualityLoose))          {trkQuality = 0;cout << "loose = true" << endl;}
	       if(piPATTrack.track()->quality(trackQualityTight))          {trkQuality = 1;cout << "tight = true" << endl;}
	       if(piPATTrack.track()->quality(trackQualityhighPur))        {trkQuality = 2;cout << "highpur = true" << endl;}
	       //if(piPATTrack.track()->quality(trackQualityConfirmed))      {trkQuality = 3;cout << "confirmed = true" << endl;}
	       //if(piPATTrack.track()->quality(trackQualityGoodIterative))  {trkQuality = 4;cout << "good iterative = true" << endl;}
	       //int trkQuality = piPATTrack.track()->quality(trackQuality_);
	       bResTrkQual->push_back( trkQuality );
	       
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
	       
	       // next check the primary calculated without our B candidate tracks

	       //  edm::RefToBase<reco::Track> TrackBaseRef	       

	       // first get tracks from the original primary
	       vector<reco::TransientTrack> vertexTracks;

	       for ( std::vector<TrackBaseRef >::const_iterator iTrack = bestVtxBS->tracks_begin();
		     iTrack != bestVtxBS->tracks_end(); ++iTrack) {
		 cout << "found a track in the primary" << endl;

		 // compare primary tracks to check for matches with B cand
		 
		 TrackRef trackRef = iTrack->castTo<TrackRef>();

		 // the 5 tracks in the B cand are

		 cout << "match with V0 track 1	= " << (theDaughterTracks[0]==trackRef) << endl;
		 cout << "match with V0 track 2	= " << (theDaughterTracks[1]==trackRef) << endl;
		 cout << "match with psi track 1 = " << (glbTrackP==trackRef) << endl;
		 cout << "match with psi track 2 = " << (glbTrackM==trackRef) << endl;
		 cout << "match with Kst track 1 = " << (piPATTrack.track()==trackRef) << endl;

		 if ( (theDaughterTracks[0]==trackRef) || (theDaughterTracks[1]==trackRef) ||
		      (glbTrackP==trackRef) || (glbTrackM==trackRef) || (piPATTrack.track()==trackRef) )
		   cout << "found track match with primary" << endl;
		 else {
		   TransientTrack tt(trackRef, &(*bFieldHandle) );
		   vertexTracks.push_back(tt);
		 }
	       }

	       cout << "number of tracks in primary = " << bestVtxBS->tracksSize() << endl;
	       cout << "number of tracks from vertex tt vector = " << vertexTracks.size() << endl;

	       // if no tracks in primary then don't do anything.

	       if (  vertexTracks.size()>0 ) {

		 AdaptiveVertexFitter theFitter;
		 TransientVertex v = theFitter.vertex(vertexTracks, beamSpot);
		 if ( v.isValid() ) {
		   //calculate ctau with the new vertex to compare to the old one.
		   //1. standard 3D calculation
		   //2. 3D MPV calculation
		 }
	       

	       cout << "best vertex with BS constraint from event = (" << bestVtxBS->position().x() << ","
		    << bestVtxBS->position().y() << ","
		    << bestVtxBS->position().z() << ")" << endl;
	       
	       if (v.isValid())
		 cout << "best vertex with BS constraint from my fit = (" << v.position().x() << ","
		      << v.position().y() << ","
		      << v.position().z() << ")" << endl;
	       else cout <<"Invalid fitted vertex\n";

	       }	       

	       nB++;
	       
	       
	       pionParticles.clear();
	       muonParticles.clear();
	       vFitMCParticles.clear();
	       muonVParticles.clear();
	       muonpiParticles.clear();
	       }
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
	     if ( abs(genGDau->pdgId())==311 && abs(genDau->pdgId())==323 ) {
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
	     if ( abs(genDau->pdgId())==323 && abs(genGDau->pdgId())!=211 && abs(genGDau->pdgId())!=311 && genGDau->pdgId()!=22 )
	       wrong = true;
	   }
	   if ( genDau->pdgId()!=443 && abs(genDau->pdgId())!=323 && genDau->pdgId()!=22 ) 
	     wrong = true;
	   if (imu1!=-1&&imu2!=-1&&!wrong) ipsi = i;
	   if (ik0!=-1 && ipi!=-1&&!wrong) ikstp = i;
	 }
	 if ( ipsi!=-1 && ikstp!=-1 ) {
	   cout << " found genKstarpPsi";
	 }
	 cout << endl;
	 if ( ipsi!=-1 && ikstp!=-1 ) {
	   genKstarpPsi =1;
	   
	   //fill MC truth info
	   
	   //write out info from daughters
	   const Candidate * genpsi =  BCand.daughter(ipsi);
	   const Candidate * genkstp =  BCand.daughter(ikstp);
	   
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

	   trueBResPx = genkstp->px();
	   trueBResPy = genkstp->py();
	   trueBResPz = genkstp->pz();
	   
	   for (uint j=0; j<genkstp->numberOfDaughters(); j++) {
	     if ( abs(genkstp->daughter(j)->pdgId())==311 && genkstp->daughter(j)->daughter(0)->pdgId()==310 ) {
	       const Candidate * genks = genkstp->daughter(j)->daughter(0);
	       trueVPx = genks->px();
	       trueVPy = genks->py();
	       trueVPz = genks->pz();
	       for (uint k=0; k<genks->numberOfDaughters(); k++){
		 if (genks->daughter(k)->charge()>0) {
		   trueVTrkpPx = genks->daughter(k)->px();
		   trueVTrkpPy = genks->daughter(k)->py();
		   trueVTrkpPz = genks->daughter(k)->pz();
		   trueVDecayVtxX = genks->daughter(k)->vx();
		   trueVDecayVtxY = genks->daughter(k)->vy();
		   trueVDecayVtxZ = genks->daughter(k)->vz();
		 }
		 if (genks->daughter(k)->charge()<0) {
		   trueVTrkmPx = genks->daughter(k)->px();
		   trueVTrkmPy = genks->daughter(k)->py();
		   trueVTrkmPz = genks->daughter(k)->pz();
		 }
	       }
	     }
	     if ( abs(genkstp->daughter(j)->pdgId())==211 ) {
	       trueBResTrkPx = genkstp->daughter(j)->px();
	       trueBResTrkPy = genkstp->daughter(j)->py();
	       trueBResTrkPz = genkstp->daughter(j)->pz();
	       trueBResTrkChg = genkstp->daughter(j)->charge();
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

	   float trueBResPiPhi = atan(trueBResTrkPy/trueBResTrkPx);
	   if ( trueBResTrkPx < 0 && trueBResTrkPy < 0 ) trueBResPiPhi -= pi;
	   if ( trueBResTrkPx < 0 && trueBResTrkPy > 0 ) trueBResPiPhi += pi;
	   float trueBResPiP = sqrt( trueBResTrkPx*trueBResTrkPx +  trueBResTrkPy*trueBResTrkPy +  trueBResTrkPz*trueBResTrkPz );
	   float trueBResPiEta = 0.5*log( (trueBResPiP + trueBResTrkPz)/(trueBResPiP - trueBResTrkPz) );

	   float RcutMu = 0.02;
	   float RcutPi = 0.05;
	   float RcutVtx = 10.;

	   truthMatch.clear(); truthKstp.clear(); truthKs.clear(); truthPsi.clear();
	   
	   for (uint i = 0; i<mupPx->size(); i++) {
	     //loop to check all B candidates found

	     bool istrueMup = false;
	     bool istrueMum = false;
	     bool istruePip = false;
	     bool istruePim = false;
	     bool istrueBResPi = false;
	     bool istrueKs = false;
	     bool istrueKstp = false;
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

	     float brespiPhi = atan(bResTrkPy->at(i)/bResTrkPx->at(i));
	     if ( bResTrkPx->at(i) < 0 && bResTrkPy->at(i) < 0 ) brespiPhi -= pi;
	     if ( bResTrkPx->at(i) < 0 && bResTrkPy->at(i) > 0 ) brespiPhi += pi;
	     float brespiP = sqrt( bResTrkPx->at(i)*bResTrkPx->at(i) +  bResTrkPy->at(i)*bResTrkPy->at(i) +  bResTrkPz->at(i)*bResTrkPz->at(i) );
	     float brespiEta = 0.5*log( (brespiP + bResTrkPz->at(i))/(brespiP - bResTrkPz->at(i)) );

	     cout << "For reco B muP eta, phi     = " << mupEta << "," << mupPhi << endl;
	     cout << "For reco B muM eta, phi     = " << mumEta << "," << mumPhi << endl;
	     cout << "For reco B piP eta, phi     = " << pipEta << "," << pipPhi << endl;
	     cout << "For reco B piM eta, phi     = " << pimEta << "," << pimPhi << endl;	 
	     cout << "For reco B brespi eta, phi = " << brespiEta << "," << brespiPhi << endl;	 
	     
	     float deltaRmup = sqrt( (mupEta-trueMupEta)*(mupEta-trueMupEta) +  (mupPhi-trueMupPhi)*(mupPhi-trueMupPhi) );
	     if ( deltaRmup < RcutMu ) istrueMup = true;
	     
	     float deltaRmum = sqrt( (mumEta-trueMumEta)*(mumEta-trueMumEta) +  (mumPhi-trueMumPhi)*(mumPhi-trueMumPhi) ) ;
	     if ( deltaRmum < RcutMu ) istrueMum = true;
	     
	     float deltaRpip = sqrt( (pipEta-truePipEta)*(pipEta-truePipEta) +  (pipPhi-truePipPhi)*(pipPhi-truePipPhi) );
	     if ( deltaRpip < RcutPi ) istruePip = true;
	     
	     float deltaRpim = sqrt( (pimEta-truePimEta)*(pimEta-truePimEta) +  (pimPhi-truePimPhi)*(pimPhi-truePimPhi) );
	     if ( deltaRpim < RcutPi ) istruePim = true;
	     
	     float deltaRbrespi = sqrt( (brespiEta-trueBResPiEta)*(brespiEta-trueBResPiEta) +  (brespiPhi-trueBResPiPhi)*(brespiPhi-trueBResPiPhi) );
	     if ( deltaRbrespi < RcutPi ) istrueBResPi = true;
	     
	     cout << "deltaR for mup = " << deltaRmup << ", mum = " << deltaRmum << ", deltaRpip = " << deltaRpip << ", deltaRpim = " << deltaRpim << ", brespi = " << deltaRbrespi << endl;

	     //check Ks vertex position truth match
	     float deltaRksvtx = sqrt( (trueVDecayVtxX - VDecayVtxX->at(i))*
				       (trueVDecayVtxX - VDecayVtxX->at(i)) +
				       (trueVDecayVtxY - VDecayVtxY->at(i))*
				       (trueVDecayVtxY - VDecayVtxY->at(i)) +
				       (trueVDecayVtxZ - VDecayVtxZ->at(i))*
				       (trueVDecayVtxZ - VDecayVtxZ->at(i)) );

	     if ( istrueMup && istrueMum ) istruePsi = true;
	     if ( istruePip && istruePim && (deltaRksvtx<RcutVtx)) istrueKs = true;
	     if ( istrueKs && istrueBResPi ) istrueKstp = true;
	     if ( istruePsi && istrueKstp ) istrueB = true;

	     if (istruePsi) {
	       cout << "true Psi from reco from cand " << i << endl;
	       truthPsi.push_back(1);
	     } else truthPsi.push_back(-1);
	     if (istrueKstp) {
	       cout << "true Kstp from reco from cand " << i << endl;
	       truthKstp.push_back(1);
	     } else truthKstp.push_back(-1);
	     if (istrueKs) {
	       cout << "true Ks from reco from cand " << i << endl;
	       truthKs.push_back(1);
	     } else truthKs.push_back(-1);
	     if (istrueB) {
	       cout << "true B from reco from cand " << i << endl;
	       truthMatch.push_back(1);
	     }    else truthMatch.push_back(-1);
	     
	   }
	   
	 }
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

       // check for JPsiKstp feed up // currently from B+->JPsi K+ or B0->JPsi Ks
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
       if (abs(BCand.pdgId())==511) {
	 bool psidau = false;
	 bool ksdau = false;
	 for ( uint i = 0; i < BCand.numberOfDaughters(); i++){
	   const Candidate * genDau = BCand.daughter(i);
	   if ( genDau->pdgId()==443 ) psidau = true;
	   if ( abs(genDau->pdgId())==310 ||  abs(genDau->pdgId())==311 ) ksdau = true;
	 }
	 if ( psidau && ksdau ) feedup = 2;
       }
       
       // check for JPsiKstp feed down // currently from B0->JPsi K10(1270), B0->JPsi K*0(892), B0->JPsi K2*0(1430)
       if (abs(BCand.pdgId())==511) {
	 bool psidau = false;
	 bool kst0dau = false;
	 bool kst20dau = false;
	 bool k10dau = false;
	 bool chic1dau = false;
	 bool ksdau = false;
	 bool psi2Sdau = false;
	 bool pipGDau = false;
	 bool pizGDau = false;
	 for ( uint i = 0; i < BCand.numberOfDaughters(); i++){
	   const Candidate * genDau = BCand.daughter(i);
	   if ( genDau->pdgId()==443 ) psidau = true;
	   if ( abs(genDau->pdgId())==313 ) {
	     kst0dau = true;
	     for ( uint j = 0; j < genDau->numberOfDaughters(); j++){
	       const Candidate * genGDau = genDau->daughter(j);
	       if ( abs(genGDau->pdgId())==211 ) pipGDau = true;
	       if ( abs(genGDau->pdgId())==111 ) pizGDau = true;
	     }
	   }
	   if ( abs(genDau->pdgId())==315 ) kst20dau = true;
	   if ( abs(genDau->pdgId())==10313 ) k10dau = true;
	   if ( abs(genDau->pdgId())==20443 ) chic1dau = true;
	   if ( abs(genDau->pdgId())==310 )  ksdau = true;
	   if ( abs(genDau->pdgId())== 100443)  psi2Sdau = true;
	 }
	 if ( psidau && kst0dau && pipGDau ) feeddown = 1;
	 if ( psidau && kst0dau && pizGDau ) feeddown = 2; 
	 if ( psidau && kst20dau ) feeddown = 3;
	 if ( psidau && k10dau ) feeddown = 4;
	 if ( chic1dau && ksdau ) feeddown = 5;
	 if ( chic1dau && kst0dau && pipGDau ) feeddown = 6;
	 if ( chic1dau && kst0dau && pizGDau ) feeddown = 7;
	 if ( psi2Sdau && ksdau ) feeddown = 8;
       }
       
       // currently from B+->Psi(2S) K*+, B+->JPsi K1+(1270), B+->JPsi K2*+(1430)
       if (abs(BCand.pdgId())==521) {
	 bool psidau = false;
	 bool kstpdau = false;
	 bool psi2Sdau = false;
	 bool k1pdau = false;
	 bool k2stpdau = false;
	 bool chic1dau = false;
	 bool ksdau = false;
	 bool pidau = false;
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
	   if ( abs(genDau->pdgId())==100443 ) psi2Sdau = true;
	   if ( abs(genDau->pdgId())==10323 ) k1pdau = true;
	   if ( abs(genDau->pdgId())==325 ) k2stpdau = true;
	   if ( abs(genDau->pdgId())==211 ) pidau = true;
	   if ( abs(genDau->pdgId())==310 ) ksdau = true;   
	   if ( abs(genDau->pdgId())==20443 ) chic1dau = true;
	 }
	 if ( psi2Sdau && kstpdau && pipGDau ) feeddown = 9;
	 if ( psi2Sdau && kstpdau && kpGDau ) feeddown = 10;	 
	 if ( chic1dau && kstpdau && pipGDau ) feeddown = 11;
	 if ( chic1dau && kstpdau && kpGDau ) feeddown = 12;
	 if ( chic1dau && ksdau && pidau ) feeddown = 13;
	 if ( psidau && k2stpdau ) feeddown = 14;
 	 if ( psidau && k1pdau ) feeddown = 15;
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
     
     if ( truthMatch.size()==0 ) { //if no truth match to signal, fill with zeros
       for (uint i = 0; i<mupPx->size(); i++) {
	 truthPsi.push_back(0);
	 truthKstp.push_back(0);
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
   hlt_mu3 = 0; hlt_mu5 = 0; hlt_mu7 = 0; hlt_mu9 = 0; hlt_2mu3 = 0; hlt_2mu3 = 0; hlt_2mu3JPsi = 0;
   hltBJPsiMuMu = 0;
   nB = 0;
   priVtxX = 0; priVtxY = 0; priVtxZ = 0; priVtxXE = 0; priVtxYE = 0; priVtxZE = 0; priVtxCL = 0;
   bMass->clear(); bVtxCL->clear(); bVtxC2->clear(); bPx->clear(); bPy->clear(); bPz->clear(); 
   bPxE->clear(); bPyE->clear(); bPzE->clear(); bctauMPVBS->clear();
   bDecayVtxX->clear(); bDecayVtxY->clear(); bDecayVtxZ->clear(); 
   bDecayVtxXE->clear(); bDecayVtxYE->clear(); bDecayVtxZE->clear(); 
   bResMass->clear(); bResVtxCL->clear(); bResPx->clear(); bResPy->clear(); bResPz->clear(); 
   bResDecayVtxX->clear(); bResDecayVtxY->clear(); bResDecayVtxZ->clear();
   bResDecayVtxXE->clear(); bResDecayVtxYE->clear(); bResDecayVtxZE->clear();
   VMass->clear(); VVtxCL->clear(); VPx->clear(); VPy->clear(); VPz->clear();
   VDecayVtxX->clear(); VDecayVtxY->clear(); VDecayVtxZ->clear();
   VDecayVtxXE->clear(); VDecayVtxYE->clear(); VDecayVtxZE->clear();
   JMass->clear(); JVtxCL->clear(); JVtxC2->clear(); JPx->clear(); JPy->clear(); JPz->clear();
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
   bResTrkC2->clear(); bResTrkNormC2->clear(); bResTrkPsiD02D->clear(); bResTrkPsiD03D->clear(); 
   bResTrkPsiD02DE->clear(); bResTrkPsiD03DE->clear(); bResTrkNHits->clear(); bResTrkNValidHits->clear(); 
   bResTrkPixelLayers->clear(); bResTrkQual->clear();
   genKsPsi = 0; genKstarpPsi = 0; genLambdaPsi = 0; feedup = 0; feeddown = 0;
   JVVtxCL->clear(); JVVtxC2->clear(); JpiVtxCL->clear(); JpiVtxC2->clear();
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
   prompt = 0;  truthMatch.clear(); truthKstp.clear(); truthKs.clear(); truthPsi.clear();
}

void PATBu2JpsiKstar::fillPsi(const reco::Candidate& genpsi) {
  
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

void PATBu2JpsiKstar::fillV0(const reco::Candidate& genv0) {
  
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
PATBu2JpsiKstar::beginJob(const edm::EventSetup&)
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
  tree_->Branch("bVtxC2",&bVtxC2);
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
  tree_->Branch("JVtxC2",&JVtxC2);
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
  tree_->Branch("bResTrkC2",&bResTrkC2);
  tree_->Branch("bResTrkNormC2",&bResTrkNormC2);
  tree_->Branch("bResTrkPsiD02D", &bResTrkPsiD02D);
  tree_->Branch("bResTrkPsiD03D", &bResTrkPsiD03D);
  tree_->Branch("bResTrkPsiD02DE", &bResTrkPsiD02DE);
  tree_->Branch("bResTrkPsiD03DE", &bResTrkPsiD03DE);
  tree_->Branch("bResTrkNHits",&bResTrkNHits);
  tree_->Branch("bResTrkNValidHits",&bResTrkNValidHits);
  tree_->Branch("bResTrkPixelLayers",&bResTrkPixelLayers);
  tree_->Branch("bResTrkQual",&bResTrkQual);
  tree_->Branch("genKsPsi", &genKsPsi, "genKsPsi/I");
  tree_->Branch("genKstarpPsi", &genKstarpPsi, "genKstarpPsi/I");
  tree_->Branch("genLambdaPsi", &genLambdaPsi, "genLambdaPsi/I");
  tree_->Branch("feedup", &feedup, "feedup/I");
  tree_->Branch("feeddown", &feeddown, "feeddown/I");
  tree_->Branch("JVVtxCL",&JVVtxCL);
  tree_->Branch("JVVtxC2",&JVVtxC2);
  tree_->Branch("JpiVtxCL",&JpiVtxCL);
  tree_->Branch("JpiVtxC2",&JpiVtxC2);

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
  tree_->Branch("truthKstp",&truthKstp);
  tree_->Branch("truthKs",&truthKs);
  tree_->Branch("truthPsi",&truthPsi);
}


// ------------ method called once each job just after ending the event loop  ------------
void 
PATBu2JpsiKstar::endJob() {
  tree_->GetDirectory()->cd();
  tree_->Write();
}

//define this as a plug-in
DEFINE_FWK_MODULE(PATBu2JpsiKstar);

