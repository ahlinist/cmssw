// -*- C++ -*-
//
// Package:    JPsiMuAnalyzer
// Class:      JPsiMuAnalyzer
// 
/**\class JPsiMuAnalyzer JPsiMuAnalyzer.cc Analysis/JPsiMuAnalyzer/src/JPsiMuAnalyzer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  David Lopes Pegna
//         Created:  Sun Aug 10 14:16:18 EDT 2008
// $Id$
//
//


// system include files
#include <memory>
#include <iostream>
using namespace std;

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

// Added for tracks/muons
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"


#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "PhysicsTools/CandUtils/interface/AddFourMomenta.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "DataFormats/Candidate/interface/VertexCompositeCandidate.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "TH1.h"
#include "TTree.h"
#include "TMath.h"

#include "DataFormats/BeamSpot/interface/BeamSpot.h"

#include <Rtypes.h>
#include "DataFormats/Math/interface/Error.h"
#include "DataFormats/Math/interface/Point3D.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"

#include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h"

#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "PhysicsTools/CandUtils/interface/CandMatcherNew.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include <DataFormats/VertexReco/interface/VertexFwd.h>
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"

#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"

#include "PhysicsTools/RecoUtils/interface/CandCommonVertexFitter.h"

#include "DataFormats/Common/interface/ValueMap.h"
#include "RecoEgamma/ElectronIdentification/interface/ElectronIDAlgo.h"
#include "DataFormats/EgammaReco/interface/ClusterShape.h"
#include "DataFormats/EgammaReco/interface/ClusterShapeFwd.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/BasicClusterShapeAssociation.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "DataFormats/EgammaCandidates/interface/ConversionFwd.h"

#include "DataFormats/Candidate/interface/OverlapChecker.h"
 
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Framework/interface/TriggerNames.h"
#include <TString.h>

#include "DataFormats/Common/interface/AssociationVector.h"
typedef edm::AssociationVector<reco::CandidateRefProd, std::vector<double> > IsolationCollection;

#include "AnalysisDataFormats/Egamma/interface/ElectronID.h"
#include "AnalysisDataFormats/Egamma/interface/ElectronIDAssociation.h"

#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloTopology/interface/CaloTopology.h"
 #include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"

//#include "RecoMuon/MuonIdentification/interface/IdGlobalFunctions.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"

#include "DataFormats/HLTReco/interface/TriggerEventWithRefs.h"
#include "DataFormats/HLTReco/interface/TriggerFilterObjectWithRefs.h"

#include "DataFormats/HLTReco/interface/TriggerEvent.h"

#include <RecoVertex/KinematicFitPrimitives/interface/KinematicParticleFactoryFromTransientTrack.h>
#include "RecoVertex/KinematicFitPrimitives/interface/ParticleMass.h"
#include "RecoVertex/KinematicFitPrimitives/interface/RefCountedKinematicParticle.h"
#include "RecoVertex/KinematicFitPrimitives/interface/MultiTrackKinematicConstraint.h"
#include "RecoVertex/KinematicFit/interface/KinematicConstrainedVertexFitter.h"
#include "RecoVertex/KinematicFitPrimitives/interface/RefCountedKinematicTree.h"
#include "RecoVertex/KinematicFitPrimitives/interface/RefCountedKinematicParticle.h"
#include "RecoVertex/KinematicFitPrimitives/interface/RefCountedKinematicVertex.h"
#include "RecoVertex/KinematicFit/interface/TwoTrackMassKinematicConstraint.h"
#include "RecoVertex/KinematicFit/interface/KinematicParticleFitter.h"
#include "RecoVertex/KinematicFit/interface/MassKinematicConstraint.h"

#include "DataFormats/TrackReco/interface/HitPattern.h"
//
// class declaration
//

class JPsiMuAnalyzer2 : public edm::EDAnalyzer {
   public:
      explicit JPsiMuAnalyzer2(const edm::ParameterSet&);
      ~JPsiMuAnalyzer2();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------

  // ntuple variables

  double jpsimass,jpsiphi,jpsipt,jpsimu1pt,jpsimu2pt,
  jpsimu1phi,jpsimu2phi,jpsimu1eta,jpsimu2eta, 
  jpsimu1z,jpsimu2z,jpsieta,jpsilxy,
  jpsilxy2,jpsilxy3,jpsilxyerr3,jpsilxyorig,jpsilxy2orig,jpsivtxprob;
  int jpsimu1cat,jpsimu2cat,mucat,muqual[11];
  double muphi,mueta,mupt,muz,muz2,muchi2,muchi2glo,mucharge,mud0,muIP,muIP2,muIPErr,
  InvMassMu13,InvMassMu23,InvMassMu123,InvMassMuTrk,musegcomp,mucalcomp; 
  int TrueMuJpsi1Id, TrueMuJpsi2Id, TrueMuSlId, TrueJpsiId, TrueJpsiMom, TrueJpsiMomId,
  TrueVtxJpsiId, TrueJpsiGMomId, TrueMuGMothId, TrueMuGGMothId,
  TrueMuJpsi1MothId[10],TrueMuJpsi2MothId[10],
  TrueMuJpsi1GMothId,TrueMuJpsi1GGMothId,TrueMuJpsi2GMothId,TrueMuJpsi2GGMothId,TrueMuMothId[10];
  double TrueJpsiPt,TrueJpsiEta,TrueJpsiPhi,TrueMuSlPt,TrueMuSlEta,TrueMuSlPhi,
  TrueElecSlPt,TrueElecSlEta,TrueElecSlPhi;
  int munhits,munhitsglo,muonglob,BBType,NJpsiEv,elecid,elecMult,muMult,convMult,convTracks[150];
  double MCbphi,MCbbarphi,MCbpt,MCbbarpt,MCbbarmass,MCbeta,MCbbeta,MCben,MCbben;   
  double elecpt,elecphi,eleceta,eleceOverP,elecfBrem,elecseed,elecseedoverpin,
  elechoverE,deltaPhiIn,deltaEtaIn,elecInvMas1,elecInvMas2,elecIP,elecIP2,sigmaee,e3355,deltaRconv[150];
  int TrueElecSlId,TrueElecMothId[10],TrueElecGMothId,TrueElecGGMothId,MCnb,MCnbb;
  int NevtJpsi,NevtMuMcSl,MCJpsiMothId,MCJpsiGMothId,MCJpsiGGMothId;
  int NevtMu,TruMCMuMothId[100],TruMCMuGMothId[100],TruMCMuGGMothId[100];
  int NevtElec,NevtPho,TruMCElecMothId[100],TruMCElecGMothId[100],TruMCElecGGMothId[100],elecSlFlag,elecConvFlag,elecFakeFlag;
  double DeltaRMu[100],DeltaRElec[100],DeltaRPho[1000];
  int TrueBestMuMothId[10],TrueBestMuGMothId,TrueBestMuGGMothId,
  TrueBestMuJpsi1MothId[10],TrueBestMuJpsi1GMothId,TrueBestMuJpsi1GGMothId,
  TrueBestMuJpsi2MothId[10],TrueBestMuJpsi2GMothId,TrueBestMuJpsi2GGMothId;
  double TrueBestMuMothVtx[3],TrueBestMuJpsi1MothVtx[3],TrueBestMuJpsi2MothVtx[3];
  int TrueBestElecMothId[10],TrueBestElecGMothId,TrueBestElecGGMothId;
  int indexBestMuJpsi1,indexBestMuJpsi2,indexBestMuSl;
  int matchL1,matchL2,matchL3,matchHLT1,matchHLT2,matchHLT3,matchTrkL,matchTrkHLT,matchMu2nd,NTrkStudy[5];
  double PVerrx,PVerry,JpsiTestMass,JpsiTestvtxprob,JpsiTestPt,JpsiTestEta,TrackTestPt,TrackTestEta;
  double minDRBestMuJpsi1,minDRBestMuJpsi2,minDRBestMuSl,minDRBestElecSl,Bmass,BmassC,KstMass,Bmass2,Bmass2C;
  double Bvtxprob, Bphi,BPt1,BPx1,BPy1,Bsumpt,B1Lxy,B1Lxy2,Berrx1,Berry1,Berrxy1,kaonpt,kaoneta,kaonphi, 
  kaonchi2, BPt2,BPx2,BPy2,Bsumpt2,B2Lxy,B2Lxy2,Bphi2,Bvtxprob2,Berrx2,Berry2,Berrxy2,kst1pt,kst2pt,kst1eta,kst2eta,kst1phi,kst2phi,kst1chi2,kst2chi2;
  int trk1muov,trk2muov,trk1nhits,trk2nhits,trk3nhits,MCKid,MCKmomid,MCKGmomid,
  MCK21id,MCK21momid,MCK21Gmomid,MCK22id,MCK22momid,MCK22Gmomid;
  double Dmass1,Dmass2,Dsmass1,Dsmass2,DvtxProb,trkD1pt,trkD2pt,trkD3pt,trk1chi2,trk2chi2,trk3chi2;
  double MCKdr,MCK21dr,MCK22dr,MCJpsiLxy,MCBpPt,MCBpLxy,MCBpdauLxy,MCB0Pt,MCBpP,MCB0P,MCBpEta,MCB0Eta,MCB0dauLxy;
  int MCBpNdau,MCB0Ndau,MCBpdauId[30],MCB0dauId[30],MCB0Kstc,ncomb1,ncomb2,ncomb3; 
  int kaonmuov, kaonmuov2, kaonnhits,kst1nhits,kst2nhits,NevtKaon1,NevtKaon2,NevtKaon3,NevtKaon4,
  kaonpixH,kaontrkH,kst1pixH,kst1trkH,kst2pixH,kst2trkH;
  int trigflag[160];

      TH1F * fHist2muMass; 
      TH1F * fHistDeltaPhi; 
      TH1F * fHistRes; 
      TH1F * fHistMuSLPt;
      TH1F * fHistMuDauPt1;    
      TH1F * fHistMuDauPt2;    

    TH1D *  BBdphiFC;
    TH1D *  BBdphiFEX;
    TH1D *  BBdphiGS;
    
    TH1F * HistJpsiM1;  
    TH1F * HistJpsiM2;  
    TH1F * HistPhiRec;
    TH1F * HistComb1;
    TH1F * HistComb2;
    TH1F * HistComb3;
    TH1F * HistMass1;
    TH1F * HistMass2;
    TH1F * HistMass3;
    TH1F * HistMass4;

  TTree * ntp1;

};

//
// constants, enums and typedefs:
//

//
// static data member definitions
//

//
// constructors and destructor
//
JPsiMuAnalyzer2::JPsiMuAnalyzer2(const edm::ParameterSet& iConfig)

{
 
  jpsimass=-99,jpsiphi=-99.,jpsipt=-99.,jpsimu1pt=-99.,jpsimu2pt=-99.,
  jpsimu1cat=-99,jpsimu2cat=-99,
  jpsimu1phi=-99., jpsimu2phi=-99., jpsimu1eta=-99., jpsimu2eta=-99.,
  jpsimu1z=-99., jpsimu2z=-99.,jpsieta=-99.,jpsilxy=-99.,
  jpsilxy2=-99.,jpsilxy3=-99.,jpsilxyerr3=-99.,jpsilxyorig=-99.,jpsilxy2orig=-99.,jpsivtxprob=-99.;
  muphi=-99., mueta=-99., mupt=-99.,mucat=-99,muchi2=-99.,muchi2glo=-99,munhits=-99,
  munhitsglo=-99,mucharge=-99,mud0=-99.,muz=-99.,
  InvMassMu13=-99.,InvMassMu23=-99.,InvMassMu123=-99.,InvMassMuTrk=-99.,
  musegcomp=-99.,mucalcomp=-99.,
  muz2=-99.,muIP=-99., muIP2=-99.,muIPErr=-99.,muonglob=-99; 
  TrueMuJpsi1Id=-99, TrueMuJpsi2Id=-99, TrueMuSlId=-99, TrueJpsiId=-99, TrueJpsiMom=-99, TrueJpsiMomId=-99, 
  TrueVtxJpsiId=-99, TrueJpsiGMomId=-99, TrueMuGMothId=-99, TrueMuGGMothId=-99;
  TrueJpsiPt=-99. ,TrueJpsiEta=-99.,TrueJpsiPhi=-99.,TrueMuSlPt=-99.,TrueMuSlEta=-99.,TrueMuSlPhi=-99.,
  TrueElecSlPt=-99.,TrueElecSlEta=-99.,TrueElecSlPhi=-99.;
  MCbphi=-99, MCbbarphi=-99, MCbpt=-99, MCbbarpt=-99, MCbbarmass=-99,MCnb=-99,MCnbb=-99,
  MCbeta=-99.,MCbbeta=-99.,MCben=-99.,MCbben=-99.,BBType=-99, NJpsiEv=-99, muMult=-99;
  elecphi=-99, elecpt=-99, elecid=-99, convMult=-99;
  for(int i=0;i<11;i++) muqual[i]=-99; 
  for(int i=0;i<150;i++) convTracks[i]=-99;  
  elecMult=-99, elecIP=-99, elecIP2=-99, sigmaee=-99, e3355=-99.;
  for(int i=0;i<150;i++) deltaRconv[i]=-99;
  eleceta=-99, eleceOverP=-99, elecfBrem=-99,  
  elecseed=-99,elecseedoverpin=-99,elechoverE=-99,deltaPhiIn=-99,deltaEtaIn=-99,
  TrueElecSlId=-99,TrueElecGMothId=-99 ,TrueElecGGMothId=-99; 
  elecInvMas1=-99.,elecInvMas2=-99.;
  elecSlFlag=-99, elecConvFlag=-99, elecFakeFlag=-99;
  for(int i=0;i<10;i++) {TrueMuJpsi1MothId[i]=-99; TrueMuJpsi2MothId[i]=-99; TrueMuMothId[i]=-99; TrueElecMothId[i]=-99;}
  TrueMuJpsi1GMothId=-99,TrueMuJpsi1GGMothId=-99,TrueMuJpsi2GMothId=-99,TrueMuJpsi2GGMothId=-99;
  NevtJpsi=-99,NevtMuMcSl=-99,MCJpsiMothId=-99,MCJpsiGMothId=-99,MCJpsiGGMothId=-99;
  NevtMu=-99; for(int i=0;i<100;i++) { TruMCMuMothId[i]=-99; TruMCMuGMothId[i]=-99; TruMCMuGGMothId[i]=-99;}
  NevtElec=-99, NevtPho=-99; for(int i=0;i<100;i++) { TruMCElecMothId[i]=-99; TruMCElecGMothId[i]=-99; TruMCElecGGMothId[i]=-99;}
  for(int i=0;i<100;i++){DeltaRMu[i]=-99;DeltaRElec[i]=-99;}
  for(int i=0;i<1000;i++) DeltaRPho[i]=-99;
  for(int i=0;i<5;i++) NTrkStudy[i]=-99;
  TrueBestMuGMothId=-99,TrueBestMuGGMothId=-99,
  TrueBestMuJpsi1GMothId=-99,TrueBestMuJpsi1GGMothId=-99,TrueBestMuJpsi2GMothId=-99,TrueBestMuJpsi2GGMothId=-99,
  TrueBestElecGMothId=-99,TrueBestElecGGMothId=-99;
  for(int i=0;i<10;i++) {TrueBestMuMothId[i]=-99;TrueBestElecMothId[i]=-99;
  TrueBestMuJpsi1MothId[i]=-99;TrueBestMuJpsi2MothId[i]=-99;}
  for(int i=0;i<2;i++){TrueBestMuMothVtx[i]=-99; TrueBestMuJpsi1MothVtx[i]=-99; TrueBestMuJpsi2MothVtx[i]=-99;}
  minDRBestMuJpsi1=-99,minDRBestMuJpsi2=-99,minDRBestMuSl=-99.,indexBestMuJpsi1=-99,
  indexBestMuJpsi2=-99,indexBestMuSl=-99,minDRBestElecSl=-99.,Bmass=-99.,BmassC=-99.,Bmass2C=-99.,
  PVerrx=-99.,PVerry=-99.,JpsiTestMass=-99,JpsiTestvtxprob=-99,JpsiTestPt=-99,JpsiTestEta=-99,TrackTestPt=-99,TrackTestEta=-99,
  matchL1=-99,matchL2=-99,matchL3=-99,matchTrkL=-99,matchTrkHLT=-99,
  matchHLT1=-99,matchHLT2=-99,matchHLT3=-99,matchMu2nd=-99,
  NevtKaon1=-99,NevtKaon2=-99,NevtKaon3=-99,NevtKaon4=-99,
  Bvtxprob=-99.,BPt1=-99.,BPx1=-99.,BPy1=-99.,Bsumpt=-99.,Bphi=-99.,B1Lxy=-99.,B1Lxy2=-99.,
  Berrx1=-99.,Berry1=-99.,Berrxy1=-99.,kaonpt=-99.,
  kaoneta=-99.,kaonphi=-99.,kaonmuov=-99,kaonmuov2=-99,kaonchi2=-99.,kaonnhits=-99;
  kaonpixH=-99,kaontrkH=-99,kst1pixH=-99,kst1trkH=-99,kst2pixH=-99,kst2trkH=-99;
  Bvtxprob2=-99.,BPt2=-99.,BPx2=-99.,BPy2=-99.,Bsumpt2=-99.,
  B2Lxy=-99.,B2Lxy2=-99.,Bphi2=-99.,Berrx2=-99.,Berry2=-99.,Berrxy2=-99.,KstMass=-99.,Bmass2=-99.,kst1pt=-99.,kst2pt=-99.,
  kst1eta=-99.,kst2eta=-99.,kst1phi=-99.,kst2phi=-99.,kst1nhits=-99,kst2nhits=-99,DvtxProb=-99.,
  kst1chi2=-99.,kst2chi2=-99.,Dmass1=-99.,Dmass2=-99.,Dsmass1=-99.,Dsmass2=-99.,trk1muov=-99,trk2muov=-99,trkD1pt=-99.,
  trkD2pt=-99.,trkD3pt=-99.,trk1nhits=-99,trk2nhits=-99,trk3nhits=-99,
  MCKid=-99,MCKmomid=-99,MCKGmomid=-99,MCK21id=-99,MCK21momid=-99,MCK21Gmomid=-99,
  MCK22id=-99,MCK22momid=-99,MCK22Gmomid=-99,
  trk1chi2=-99.,trk2chi2=-99.,trk3chi2=-99.;
  MCKdr=-99.,MCK21dr=-99.,MCK22dr=-99.,MCJpsiLxy=-99.,MCBpPt=-99.,MCBpLxy=-99.,
  MCBpdauLxy=-99.,MCB0Pt=-99.,MCBpP=-99.,MCB0P=-99.,
  MCB0Kstc=-99,ncomb1=-99,ncomb2=-99,ncomb3=-99, 
  MCBpEta=-99.,MCB0Eta=-99.,MCBpNdau=-99,MCB0Ndau=-99, MCB0dauLxy=-99.;
  for(int i=0;i<30;i++) {MCBpdauId[i]=-99; MCB0dauId[i]=-99;}
  for(int i=0;i<160;i++) trigflag[i]=-99;

  //now do what ever initialization is needed
  edm::Service<TFileService> fs;
  fHist2muMass  = fs->make<TH1F>(  "Hist2muMass", "2-mu inv. mass", 100,  1.5, 4.5 ) ;
  fHistDeltaPhi  = fs->make<TH1F>(  "fHistDeltaPhi", "Delta Phi", 30,  0,4.  ) ;
  fHistRes = fs->make<TH1F>("fHistRes","Delta Phi -Delta BB", 50,-2.,2.);
  fHistMuSLPt = fs->make<TH1F>("fHistMuSLPt","mu pt SL",50,0.,10.);
  fHistMuDauPt1 = fs->make<TH1F>("fHistMuDauPt1","mu pt J/Psi1",50,0.,10.);
  fHistMuDauPt2 = fs->make<TH1F>("fHistMuDauPt2","mu pt J/Psi2",50,0.,10.);
  BBdphiFC = fs->make<TH1D>("bbbar dPhi FC","bbbar dPhi FC",32, 0.0, 3.2);
  BBdphiFEX = fs->make<TH1D>("bbbar dPhi FEX","bbbar dPhi FEX",32, 0.0, 3.2);
  BBdphiGS = fs->make<TH1D>("bbbar dPhi GS","bbbar dPhi GS",32, 0.0, 3.2);
  HistJpsiM1 = fs->make<TH1F>("HistJpsiM1","J/Psi Mass GMuons",50, 2.5, 3.5);
  HistJpsiM2 = fs->make<TH1F>("HistJpsiM2","J/Psi Mass GMuons",50, 2.5, 3.5);
  HistPhiRec = fs->make<TH1F>("HistPhiRec","J/Psi-Mu SL Phi Rec",30,0.,4.);
  HistComb1 = fs->make<TH1F>("HistComb1","",4,0.,4.);
  HistComb2 = fs->make<TH1F>("HistComb2","",4,0.,4.);
  HistComb3 = fs->make<TH1F>("HistComb3","",4,0.,4.);
  HistMass1 = fs->make<TH1F>("HistMass1","",150,2.8,3.3);
  HistMass2 = fs->make<TH1F>("HistMass2","",100,5.0,5.5);
  HistMass3 = fs->make<TH1F>("HistMass3","",100,5.0,5.5);
  HistMass4 = fs->make<TH1F>("HistMass4","",100,0.77,1.01);

  ntp1 = fs->make<TTree>("ntp1","ntp1");

}


JPsiMuAnalyzer2::~JPsiMuAnalyzer2()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
JPsiMuAnalyzer2::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace reco;

  jpsimass=-99,jpsiphi=-99.,jpsipt=-99.,jpsimu1pt=-99.,jpsimu2pt=-99.,
  jpsimu1cat=-99,jpsimu2cat=-99,
  jpsimu1phi=-99., jpsimu2phi=-99., jpsimu1eta=-99., jpsimu2eta=-99., jpsimu1z=-99., jpsimu2z=-99., 
  jpsieta=-99.,jpsilxy=-99.,
  jpsilxy2=-99.,jpsilxy3=-99.,jpsilxyerr3=-99.,jpsilxyorig=-99.,jpsilxy2orig=-99.,jpsivtxprob=-99.;
  muphi=-99., mueta=-99., mupt=-99.,mucat=-99,muchi2=-99.,muchi2glo=-99,munhits=-99,
  munhitsglo=-99,mucharge=-99,mud0=-99.,muz=-99.,InvMassMu13=-99.,InvMassMu23=-99.,InvMassMu123=-99.,InvMassMuTrk=-99.,
  musegcomp=-99.,mucalcomp=-99.,
  muz2=-99,muIP=-99., muIP2=-99.,muIPErr=-99.,muonglob=-99;
  TrueMuJpsi1Id=-99, TrueMuJpsi2Id=-99, TrueMuSlId=-99, TrueJpsiId=-99, TrueJpsiMom=-99, TrueJpsiMomId=-99, 
  TrueVtxJpsiId=-99, TrueJpsiGMomId=-99, TrueMuGMothId=-99, TrueMuGGMothId=-99;
  TrueJpsiPt=-99. ,TrueJpsiEta=-99.,TrueJpsiPhi=-99.,TrueMuSlPt=-99.,TrueMuSlEta=-99.,TrueMuSlPhi=-99.,
  TrueElecSlPt=-99.,TrueElecSlEta=-99.,TrueElecSlPhi=-99.;
  MCbphi=-99,MCbbarphi=-99, MCbpt=-99, MCbbarpt=-99, MCbbarmass=-99,MCnb=-99,MCnbb=-99,
  MCbeta=-99.,MCbbeta=-99.,MCben=-99.,MCbben=-99.,BBType=-99, NJpsiEv=-99, muMult=-99;
  elecpt=-99, elecphi=-99, elecid=-99, convMult=-99;
  for(int i=0;i<11;i++) muqual[i]=-99; 
  for(int i=0;i<150;i++) convTracks[i]=-99; 
  elecMult=-99, elecIP=-99, elecIP2=-99, sigmaee=-99, e3355=-99.;
  for(int i=0;i<150;i++) deltaRconv[i]=-99;
  eleceta=-99, eleceOverP=-99, elecfBrem=-99, 
  elecseed=-99,elecseedoverpin=-99,elechoverE=-99,deltaPhiIn=-99,deltaEtaIn=-99, 
  TrueElecSlId=-99,TrueElecGMothId=-99 ,TrueElecGGMothId=-99;
  elecInvMas1=-99.,elecInvMas2=-99.;
  elecSlFlag=-99, elecConvFlag=-99, elecFakeFlag=-99;
   for(int i=0;i<10;i++) {TrueMuJpsi1MothId[i]=-99; TrueMuJpsi2MothId[i]=-99; TrueMuMothId[i]=-99; TrueElecMothId[i]=-99;}
  TrueMuJpsi1GMothId=-99,TrueMuJpsi1GGMothId=-99,TrueMuJpsi2GMothId=-99,TrueMuJpsi2GGMothId=-99;
  NevtJpsi=-99,NevtMuMcSl=-99,MCJpsiMothId=-99,MCJpsiGMothId=-99,MCJpsiGGMothId=-99;  
  NevtMu=-99; for(int i=0;i<100;i++) { TruMCMuMothId[i]=-99; TruMCMuGMothId[i]=-99; TruMCMuGGMothId[i]=-99;}
  NevtElec=-99, NevtPho=-99; for(int i=0;i<100;i++) { TruMCElecMothId[i]=-99; TruMCElecGMothId[i]=-99; TruMCElecGGMothId[i]=-99;}
  for(int i=0;i<100;i++){DeltaRMu[i]=-99;DeltaRElec[i]=-99;}
  for(int i=0;i<1000;i++) DeltaRPho[i]=-99;
  for(int i=0;i<5;i++) NTrkStudy[i]=-99;
  TrueBestMuGMothId=-99,TrueBestMuGGMothId=-99,
  TrueBestMuJpsi1GMothId=-99,TrueBestMuJpsi1GGMothId=-99,TrueBestMuJpsi2GMothId=-99,TrueBestMuJpsi2GGMothId=-99,
  TrueBestElecGMothId=-99,TrueBestElecGGMothId=-99;
  for(int i=0;i<10;i++) {TrueBestMuMothId[i]=-99;TrueBestElecMothId[i]=-99;
  TrueBestMuJpsi1MothId[i]=-99;TrueBestMuJpsi2MothId[i]=-99;}
  for(int i=0;i<2;i++){TrueBestMuMothVtx[i]=-99; TrueBestMuJpsi1MothVtx[i]=-99; TrueBestMuJpsi2MothVtx[i]=-99;}
  minDRBestMuJpsi1=-99,minDRBestMuJpsi2=-99,minDRBestMuSl=-99.,
  indexBestMuJpsi1=-99,indexBestMuJpsi2=-99,indexBestMuSl=-99,minDRBestElecSl=-99.,Bmass=-99.,BmassC=-99.,
  Bmass2C=-99.,PVerrx=-99.,PVerry=-99.,JpsiTestMass=-99,JpsiTestvtxprob=-99,JpsiTestPt=-99,JpsiTestEta=-99,TrackTestPt=-99,TrackTestEta=-99,
  matchL1=-99,matchL2=-99,matchL3=-99,matchTrkL=-99,matchTrkHLT=-99,
  matchHLT1=-99,matchHLT2=-99,matchHLT3=-99,matchMu2nd=-99,
  NevtKaon1=-99,NevtKaon2=-99,NevtKaon3=-99,NevtKaon4=-99,
  Bvtxprob=-99.,BPt1=-99.,BPx1=-99.,BPy1=-99.,Bsumpt=-99.,Bphi=-99.,B1Lxy=-99.,B1Lxy2=-99.,
  Berrx1=-99.,Berry1=-99.,Berrxy1=-99.,kaonpt=-99.,
  kaoneta=-99.,kaonphi=-99.,kaonmuov=-99,kaonmuov2=-99,kaonchi2=-99.,kaonnhits=-99;
  kaonpixH=-99,kaontrkH=-99,kst1pixH=-99,kst1trkH=-99,kst2pixH=-99,kst2trkH=-99;
  Bvtxprob2=-99.,BPt2=-99.,BPx2=-99.,BPy2=-99.,Bsumpt2=-99.,
  B2Lxy=-99.,B2Lxy2=-99.,Berrx2=-99.,Berry2=-99.,Berrxy2=-99.,Bphi2=-99.,KstMass=-99.,Bmass2=-99.,kst1pt=-99.,kst2pt=-99.,
  kst1eta=-99.,kst2eta=-99.,kst1phi=-99.,kst2phi=-99.,kst1nhits=-99,kst2nhits=-99,DvtxProb=-99.,
  kst1chi2=-99.,kst2chi2=-99.,Dmass1=-99.,Dmass2=-99.,Dsmass1=-99.,Dsmass2=-99.,trk1muov=-99,trk2muov=-99,trkD1pt=-99.,
  trkD2pt=-99.,trkD3pt=-99.,trk1nhits=-99,trk2nhits=-99,
  MCKid=-99,MCKmomid=-99,MCKGmomid=-99,MCK21id=-99,MCK21momid=-99,MCK21Gmomid=-99, 
  MCK22id=-99,MCK22momid=-99,MCK22Gmomid=-99,
  trk3nhits=-99,trk1chi2=-99.,trk2chi2=-99.,trk3chi2=-99.;
  MCKdr=-99.,MCK21dr=-99.,MCK22dr=-99.,MCJpsiLxy=-99.,MCBpPt=-99.,MCBpLxy=-99.,
  MCBpdauLxy=-99.,MCB0Pt=-99.,MCBpP=-99.,
  MCB0Kstc=-99,ncomb1=-99,ncomb2=-99,ncomb3=-99,
  MCB0P=-99.,MCBpEta=-99.,MCB0Eta=-99.,MCBpNdau=-99,MCB0Ndau=-99, MCB0dauLxy=-99.;
  for(int i=0;i<30;i++) {MCBpdauId[i]=-99; MCB0dauId[i]=-99;}
  for(int i=0;i<160;i++) trigflag[i]=-99; 


// trigger
edm::Handle<edm::TriggerResults>  hltresults;
//iEvent.getByLabel("TriggerResults", hltresults);

edm::InputTag tag("TriggerResults::HLT");
iEvent.getByLabel(tag, hltresults);

edm::TriggerNames triggerNames_;
triggerNames_.init(* hltresults);



int ntrigs = hltresults->size();
if (ntrigs==0){std::cout << "%HLTInfo -- No trigger name given in TriggerResults of the input " << std::endl;}

    for (int itrig = 0; itrig != ntrigs; ++itrig){

      TString trigName=triggerNames_.triggerName(itrig);
      bool accept = hltresults->accept(itrig);

      if (accept){trigflag[itrig] = 1;}
      else {trigflag[itrig] = 0;}

//	std::cout << "%HLTInfo --  Number of HLT Triggers: " << ntrigs << std::endl;
//	std::cout << "%HLTInfo --  HLTTrigger(" << itrig << "): " << trigName << " = " << accept << std::endl;
    }

//int itrig1 = triggerNames_.triggerIndex("HLT_DoubleMu3");
//        if (hltresults->accept(itrig1)) std::cout << "%HLTInfo --  : pass HLT_DoubleMu3"  << std::endl;
//

Handle<GenParticleCollection> genParticles;
   iEvent.getByLabel("genParticles", genParticles);


  int VtxIn=-99;
  double MinVtxProb=-999.;

reco::BeamSpot vertexBeamSpot;
edm::Handle<reco::BeamSpot> recoBeamSpotHandle;
iEvent.getByType(recoBeamSpotHandle);
vertexBeamSpot = *recoBeamSpotHandle;

 double BSx=vertexBeamSpot.x0();
 double BSy=vertexBeamSpot.y0();

// const Point & BSposition(0,0,0); 
   //= Point(vertexBeamSpot->position());

   Handle<reco::VertexCollection> recVtxs;
    iEvent.getByLabel("offlinePrimaryVertices", recVtxs);

   for(size_t i = 0; i < recVtxs->size(); ++ i) {
    const Vertex &vtx = (*recVtxs)[i];
    double RecVtxProb=TMath::Prob(vtx.chi2(),vtx.ndof());
    if(RecVtxProb>MinVtxProb){
    VtxIn=i;
    MinVtxProb=RecVtxProb;
    }
   }

  const Vertex &RecVtx = (*recVtxs)[VtxIn];
  double PVx=0.,PVy=0.;
  if(VtxIn!=-99)  {PVx = RecVtx.x(); PVy= RecVtx.y();PVerrx=RecVtx.xError(); PVerry=RecVtx.yError();}
   else {PVx=BSx; PVy=BSy;} 

#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
//Read eID results
  std::vector<edm::Handle<edm::ValueMap<float> > > eIDValueMap(4); 
  iEvent.getByLabel( "eidRobustLoose" , eIDValueMap[0] ); 
  const edm::ValueMap<float> & eIDmapRL = * eIDValueMap[0] ;
  iEvent.getByLabel( "eidRobustTight" , eIDValueMap[1] ); 
  const edm::ValueMap<float> & eIDmapRT = * eIDValueMap[1] ;
  iEvent.getByLabel( "eidLoose" , eIDValueMap[2] ); 
  const edm::ValueMap<float> & eIDmapL = * eIDValueMap[2] ;
  iEvent.getByLabel( "eidTight" , eIDValueMap[3] ); 
  const edm::ValueMap<float> & eIDmapT = * eIDValueMap[3] ;



// get the previously produced final state match
  Handle<GenParticleMatch> mcMatchMap;
  iEvent.getByLabel( "MCMatch", mcMatchMap );

  // create the extended matcher that includes automatic parent matching
  //  MCCandMatcher<CandidateCollection, GenParticleCollection> match( * mcMatchMap );
  utilsNew::CandMatcher<GenParticleCollection> match(*mcMatchMap); 


  double MinDmDiff=999.,ptMax=-999.;
  double MinVtxDiff=999., MinVtxJpsiDiff=-999.;
  double JpsiMass=-999., JpsiMass2=-999., JpsiPhi=-999.,
  JpsiEta=-999., JpsiPt=-999., JpsiMu1Pt=-999., JpsiMu2Pt=-999., 
  JpsiMu1Phi=-999., JpsiMu2Phi=-999., JpsiMu1Eta=-999., JpsiMu2Eta=-999., JpsiMu1z=-999.,JpsiMu2z=-999.,
  JpsiLxy=-999., JpsiLxy2=-999.,JpsiLxyOrig=-999.,JpsiLxy2Orig=-999.,MuSlPhi=-999.; 
  int JpsiMu1Cat=-999, JpsiMu2Cat=-999;
  int GMuIn1=-99, GMuIn2=-99, GMuSlIn=-99, VtxJpsiIn=-99;

  double ptElecMax=-999;    
  int GElecSlIn=-99;

   ESHandle<MagneticField> B;
   iSetup.get<IdealMagneticFieldRecord>().get(B);

  
 Handle<GsfElectronCollection> gsfElectrons;
 iEvent.getByLabel("pixelMatchGsfElectrons",gsfElectrons);
 
 cout << "there are electrons: " << gsfElectrons->size() << endl;

 // Find global muons
//// Handle<RecoChargedCandidateCollection> muons;
/// iEvent.getByLabel("allGlobalMuonTracks", muons); 
//  Handle<MuonRef> muons;
//  iEvent.getByLabel("goodMuons", muons);

 // Find muons
 Handle<MuonCollection> muons;
// iEvent.getByLabel("paramMuons","ParamL3Muons", muons); 
 iEvent.getByLabel("muons", muons);


    // Find global muons
    Handle<TrackCollection> globalMuons;
    iEvent.getByLabel("globalMuons", globalMuons); 

    // Find tracker muons
//    Handle<MuonCollection> trackerMuons;
//    iEvent.getByLabel("trackerMuons", trackerMuons); 

 cout << "there are globalMuons: " << globalMuons->size() << endl;
 cout << "there are reco:Muons: " << muons->size() << endl;


 Handle<VertexCompositeCandidateCollection> fittedJpsi;
 iEvent.getByLabel("fittedJpsi", fittedJpsi); 

 cout << "there are muons: " << muons->size() << endl;

    ncomb1=0,ncomb2=0,ncomb3=0;


   for(size_t i = 0; i < muons->size(); ++ i) {
   const Muon & trk1 = (*muons)[i];
//    const Candidate & trk1 = (*muons)[i]; 
   if(!trk1.isGlobalMuon() && !trk1.isTrackerMuon()) continue;     
////     if(!trk1.isGlobalMuon()) continue;

     for(size_t j = i+1; j < muons->size(); ++ j) {
     const Muon & trk2 = (*muons)[j];
 //     const Candidate & trk2 = (*muons)[j];
   if(!trk2.isGlobalMuon() && !trk2.isTrackerMuon()) continue;
////     if(!trk2.isGlobalMuon()) continue;

     if(trk1.charge()==trk2.charge()) continue;

     if(!trk1.isGood(Muon::TrackerMuonArbitrated)) continue;
     if(!trk2.isGood(Muon::TrackerMuonArbitrated)) continue;
     
      CompositeCandidate Jpsi;
      Jpsi.addDaughter( trk1 );
      Jpsi.addDaughter( trk2 );     
      AddFourMomenta addP4;
      addP4.set( Jpsi );
     
//     if(abs(Jpsi.mass()-3.096916)<MinDmDiff){
//     MinDmDiff= abs(Jpsi.mass()-3.096916);

  // CandCommonVertexFitter<KalmanVertexFitter> fitter;
  //  fitter.set(B.product());
  //   VertexCompositeCandidate fittedJpsi(Jpsi);
  //  fitter.set(fittedJpsi);

    edm::ESHandle<TransientTrackBuilder> theB;
    iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theB);
// do the vertex fit
    TrackRef trkMu1Ref = trk1.get<TrackRef>();
    TrackRef trkMu2Ref = trk2.get<TrackRef>();

    vector<TransientTrack> t_tks;
    t_tks.push_back((*theB).build(&trkMu1Ref));
    t_tks.push_back((*theB).build(&trkMu2Ref));

    //    if (t_tks.size()!=3) continue;

    KalmanVertexFitter kvf;
    TransientVertex tv = kvf.vertex(t_tks);

     if (!tv.isValid()) continue; 

    Vertex vertex = tv;
    
    GlobalPoint secondaryVertex = tv.position();

     double vtxProb=TMath::Prob(vertex.chi2(),vertex.ndof());
   cout << "J/Psi vtx chi2: " << vtxProb << " and mass: " << Jpsi.mass() << endl; 
     ncomb1++;
     HistMass1->Fill(Jpsi.mass());
//////    if(vtxProb>MinVtxJpsiDiff){
     if(abs(Jpsi.mass()-3.096916)<MinDmDiff){
     MinDmDiff= abs(Jpsi.mass()-3.096916);

//////     MinVtxJpsiDiff= vtxProb;
//     JpsiMass=fittedJpsi.mass();
     jpsivtxprob=vtxProb;

     GMuIn1=i; GMuIn2=j;
     JpsiMass=Jpsi.mass();
     JpsiPhi=Jpsi.phi();
     JpsiEta=Jpsi.eta();
     JpsiPt=Jpsi.pt();
     JpsiMu1Pt=trk1.pt();
     JpsiMu2Pt=trk2.pt();
     if(trk1.isTrackerMuon() && !trk1.isGlobalMuon()) JpsiMu1Cat=1;
     else if(!trk1.isTrackerMuon() && trk1.isGlobalMuon()) JpsiMu1Cat=2;
     else if(trk1.isTrackerMuon() && trk1.isGlobalMuon()) JpsiMu1Cat=3;
     else if(!trk1.isTrackerMuon() && !trk1.isGlobalMuon()) JpsiMu1Cat=4;
     if(trk2.isTrackerMuon() && !trk2.isGlobalMuon()) JpsiMu2Cat=1; 
     else if(!trk2.isTrackerMuon() && trk2.isGlobalMuon()) JpsiMu2Cat=2;
     else if(trk2.isTrackerMuon() && trk2.isGlobalMuon()) JpsiMu2Cat=3; 
     else if(!trk2.isTrackerMuon() && !trk2.isGlobalMuon()) JpsiMu2Cat=4;
     JpsiMu1Phi=trk1.phi();
     JpsiMu2Phi=trk2.phi();
     JpsiMu1Eta=trk1.eta();
     JpsiMu2Eta=trk2.eta();
     const Track* MuTrk1 = trk1.get<TrackRef>().get();
     const Track* MuTrk2 = trk2.get<TrackRef>().get();
     JpsiMu1z=MuTrk1->dz(vertexBeamSpot.position());
     JpsiMu2z=MuTrk2->dz(vertexBeamSpot.position());
     if(Jpsi.pt()!=0) JpsiLxy=((secondaryVertex.x()-BSx)*Jpsi.px()+(secondaryVertex.y()-BSy)*Jpsi.py())/Jpsi.pt();
     if(Jpsi.pt()!=0) JpsiLxy2=((secondaryVertex.x()-PVx)*Jpsi.px()+(secondaryVertex.y()-PVy)*Jpsi.py())/Jpsi.pt(); 
     }
     
//    VertexCompositeCandidate fittedJpsi(Jpsi);
//    fitter.set(fittedJpsi);

//     double vtxProb=fittedJpsi.vertexNormalizedChi2();
//     if(vtxProb<MinVtxDiff){
//     MinVtxDiff= vtxProb;
//     JpsiMass2=fittedJpsi.mass();
//     }
     
     }
     
   }  
  
   for(size_t i = 0; i < fittedJpsi->size(); ++ i) {
   const VertexCompositeCandidate & Jpsi = (*fittedJpsi)[i];
     double vtxProb=Jpsi.vertexNormalizedChi2();
  //   cout << "J/Psi vtx chi2: " << vtxProb << " and mass: " << Jpsi.mass() << endl;
     if(vtxProb<MinVtxDiff){
     VtxJpsiIn=i;
     MinVtxDiff= vtxProb;
     JpsiMass2=Jpsi.mass();
    if(Jpsi.pt()!=0) JpsiLxyOrig=((Jpsi.vx()-BSx)*Jpsi.px()+(Jpsi.vy()-BSy)*Jpsi.py())/Jpsi.pt();
    if(Jpsi.pt()!=0) JpsiLxy2Orig=((Jpsi.vx()-PVx)*Jpsi.px()+(Jpsi.vy()-PVy)*Jpsi.py())/Jpsi.pt();
     }
   }  
    
   HistJpsiM1->Fill(JpsiMass); 
   HistJpsiM2->Fill(JpsiMass2); 


// jpsi+K as control sample

   int TrkIn=-99;

   if(GMuIn1!=-99 && GMuIn2!=-99){ 
   double MinBVtx=-999;
   const Candidate & mu1jpsi = (*muons)[GMuIn1];
   const Candidate & mu2jpsi = (*muons)[GMuIn2];
   Handle<CandidateView> AllTracks;
   iEvent.getByLabel("allTracks",AllTracks);
   for(size_t j=0;j < AllTracks->size(); ++j){
   CompositeCandidate Jpsicand;
   Jpsicand.addDaughter(mu1jpsi);Jpsicand.addDaughter(mu2jpsi);
   AddFourMomenta addP40; addP40.set(Jpsicand);
   if(abs(Jpsicand.mass()-3.097)>0.15) continue;
   OverlapChecker overlap;
   const Candidate & trk =(*AllTracks)[j];
   if(overlap(mu1jpsi,trk)!=0 || overlap(mu2jpsi,trk)!=0) continue;

   if(trk.pt()<0.8) continue;
//   if(trk.pt()<0.7) continue;
   double deltaR=TMath::Sqrt((Jpsicand.eta()-trk.eta())*(Jpsicand.eta()-trk.eta())+
   (Jpsicand.phi()-trk.phi())*(Jpsicand.phi()-trk.phi())); 
//   if(deltaR>1.5) continue;

   CompositeCandidate Bcand;
   Bcand.addDaughter(mu1jpsi);Bcand.addDaughter(mu2jpsi);Bcand.addDaughter(trk);
   AddFourMomenta addP4; addP4.set(Bcand);

// isolation
   double sumpt=0.00000000001;
   for(size_t k=0; k< AllTracks->size(); ++k){
   const Candidate & trkT =(*AllTracks)[k]; 
   if(TMath::Sqrt((Bcand.eta()-trkT.eta())*(Bcand.eta()-trkT.eta())+
    (Bcand.phi()-trkT.phi())*(Bcand.phi()-trkT.phi())) >1.5) continue; 
   sumpt+=trkT.pt();
   } 
//   if(Bcand.pt()/sumpt<0.3) continue;
//


   double KaonMassSq=0.243717;
   double KaonE=sqrt(KaonMassSq+trk.px()*trk.px()+trk.py()*trk.py()+trk.pz()*trk.pz());

   edm::ESHandle<TransientTrackBuilder> theB;
   iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theB);
   TrackRef trkMu1Ref = mu1jpsi.get<TrackRef>();
   TrackRef trkMu2Ref = mu2jpsi.get<TrackRef>();
   TrackRef trkkaon = trk.get<TrackRef>();
   vector<TransientTrack> t_tks;
   t_tks.push_back((*theB).build(&trkMu1Ref)); 
   t_tks.push_back((*theB).build(&trkMu2Ref)); 
   t_tks.push_back((*theB).build(&trkkaon)); 

   if(!trkMu1Ref.isNonnull() || !trkMu2Ref.isNonnull() || !trkkaon.isNonnull()) continue;
   TransientTrack ttMuP= t_tks[0];
   TransientTrack ttMuM= t_tks[1];
   TransientTrack ttK = t_tks[2];
   KinematicParticleFactoryFromTransientTrack pFactory;
   ParticleMass muon_mass = 0.1056583;
   ParticleMass kaon_mass = 0.493677;
   float muon_sigma = 0.0000001;
   float kaon_sigma = 0.000016; 
   float chi = 0.;
   float ndf = 0.;
//      vector<RefCountedKinematicParticle> muonParticles;
//      muonParticles.push_back(pFactory.particle (ttMuP,muon_mass,chi,ndf,muon_sigma));
//      muonParticles.push_back(pFactory.particle (ttMuM,muon_mass,chi,ndf,muon_sigma));
      vector<RefCountedKinematicParticle> allParticles;
      allParticles.push_back(pFactory.particle (ttMuP,muon_mass,chi,ndf,muon_sigma));
      allParticles.push_back(pFactory.particle (ttMuM,muon_mass,chi,ndf,muon_sigma));
      allParticles.push_back(pFactory.particle (ttK,kaon_mass,chi,ndf,kaon_sigma));
      ParticleMass jpsi = 3.09687;
//      KinematicParticleVertexFitter kpvFitter; 
//      RefCountedKinematicTree jpTree = kpvFitter.fit(muonParticles); 
//      KinematicParticleFitter csFitter;   
//      float jp_m_sigma = 0.00004;
//      KinematicConstraint * jpsi_c2 = new MassKinematicConstraint(jpsi,jp_m_sigma);
//      jpTree = csFitter.fit(jpsi_c2,jpTree);
//      jpTree->movePointerToTheTop();
//      RefCountedKinematicParticle jpsi_part = jpTree->currentParticle();
//      allParticles.push_back(jpsi_part);
//      RefCountedKinematicTree myTree = kpvFitter.fit(allParticles);  

      MultiTrackKinematicConstraint *  j_psi_c = new  TwoTrackMassKinematicConstraint(jpsi);
      KinematicConstrainedVertexFitter kcvFitter;
      RefCountedKinematicTree myTree = kcvFitter.fit(allParticles, j_psi_c);
      myTree->movePointerToTheTop();
      RefCountedKinematicParticle bmes = myTree->currentParticle();
      RefCountedKinematicVertex b_dec_vertex = myTree->currentDecayVertex();
      if(!b_dec_vertex->vertexIsValid()) continue;
      AlgebraicVector7 b_par = bmes->currentState().kinematicParameters().vector();
      AlgebraicSymMatrix77 bs_er = bmes->currentState().kinematicParametersError().matrix(); 
       double vtxProb = TMath::Prob(bmes->chiSquared(),bmes->degreesOfFreedom());

///////  if(b_par[6]<4.9 ||  b_par[6]>5.6) continue; 
/////    if(trkkaon.get()->normalizedChi2()>10. || trkkaon.get()->numberOfValidHits()<5) continue;
    int kaonmuOV=0;
    for(size_t k = 0; k < muons->size(); ++ k) {
    const Muon & mu2= (*muons)[k];
    if(mu2.isGlobalMuon() && mu2.isGood(Muon::TrackerMuonArbitrated) &&
       mu2.isGood(Muon::TMOneStationLoose)  &&
       muon::isGoodMuon(mu2,Muon::TM2DCompatibilityLoose)){
    if(overlap(mu2,trk)!=0) kaonmuOV++;  }
    }
//////    if(kaonmuOV!=0) continue;
/*     int pixhits=0;
     const reco::HitPattern& p = trkkaon.get()->hitPattern();
     for (int i=0; i<p.numberOfHits(); i++) {
       uint32_t hit = p.getHitPattern(i);
       if (p.validHitFilter(hit) && p.pixelBarrelHitFilter(hit)) pixhits++;
     }      
     if(pixhits==0) continue;*/
 /*  KalmanVertexFitter kvf;
   TransientVertex tv = kvf.vertex(t_tks);
   if(!tv.isValid()) continue;
   Vertex vertex =tv;
   GlobalPoint secondaryVertex = tv.position();
   double vtxProb = TMath::Prob(vertex.chi2(),vertex.ndof());*/
     if(b_par[6]>4.9 && b_par[6]<5.6) ncomb2++;
     HistMass2->Fill(b_par[6]);
   if(vtxProb>MinBVtx){
   MinBVtx=vtxProb;
   TrkIn=j; 
   Bmass = sqrt((KaonE+mu1jpsi.energy()+mu2jpsi.energy())*(KaonE+mu1jpsi.energy()+mu2jpsi.energy())
   -(trk.px()+mu1jpsi.px()+mu2jpsi.px())*(trk.px()+mu1jpsi.px()+mu2jpsi.px())
   -(trk.py()+mu1jpsi.py()+mu2jpsi.py())*(trk.py()+mu1jpsi.py()+mu2jpsi.py())
   -(trk.pz()+mu1jpsi.pz()+mu2jpsi.pz())*(trk.pz()+mu1jpsi.pz()+mu2jpsi.pz()));
   BmassC=b_par[6];
   Bvtxprob=vtxProb;
   Bphi=Bcand.phi(); 
   BPt1=Bcand.pt();
   BPx1= Bcand.px();
   BPy1= Bcand.py(); 
   Bsumpt=sumpt;
//   if(Bcand.pt()!=0) B1Lxy=((secondaryVertex.x()-BSx)*Bcand.px()+(secondaryVertex.y()-BSy)*Bcand.py())/Bcand.pt();
//   if(Bcand.pt()!=0) B1Lxy2=((secondaryVertex.x()-PVx)*Bcand.px()+(secondaryVertex.y()-PVy)*Bcand.py())/Bcand.pt();
   if(Bcand.pt()!=0) B1Lxy=((b_dec_vertex->position().x()-BSx)*Bcand.px()+(b_dec_vertex->position().y()-BSy)*Bcand.py())/Bcand.pt();
   if(Bcand.pt()!=0) B1Lxy2=((b_dec_vertex->position().x()-PVx)*Bcand.px()+(b_dec_vertex->position().y()-PVy)*Bcand.py())/Bcand.pt();
   Berrx1=bs_er(1,1);
   Berry1=bs_er(2,2); 
   Berrxy1=bs_er(1,2);
   kaonpt=trk.pt();
   kaoneta=trk.eta();
   kaonphi=trk.phi();
    kaonchi2=trkkaon.get()->normalizedChi2();
    kaonnhits=trkkaon.get()->numberOfValidHits();
     int pixhits=0;
     const reco::HitPattern& p = trkkaon.get()->hitPattern();
     for (int i=0; i<p.numberOfHits(); i++) {
       uint32_t hit = p.getHitPattern(i);
       if (p.validHitFilter(hit) && p.pixelBarrelHitFilter(hit)) pixhits++;
       if (p.validHitFilter(hit) && p.pixelEndcapHitFilter(hit)) pixhits++;
     }
    kaonpixH=pixhits;
    kaontrkH=p.numberOfValidTrackerHits();   
    kaonmuov=0; 
    kaonmuov2=0;
    for(size_t k = 0; k < muons->size(); ++ k) {
    const Candidate & mu= (*muons)[k];
    if(overlap(mu,trk)!=0) kaonmuov++;
    const Muon & mu2= (*muons)[k];
    if(mu2.isGood(Muon::TrackerMuonArbitrated) && 
       mu2.isGood(Muon::TMOneStationLoose)  &&
       muon::isGoodMuon(mu2,Muon::TM2DCompatibilityLoose)){ 
    if(overlap(mu2,trk)!=0) kaonmuov2++;  }
    } 
   }

   }
  
      if(TrkIn!=-99){
      double MinDRK=999.;
      const Candidate & trk =(*AllTracks)[TrkIn];
         for(size_t i = 0; i < genParticles->size(); ++ i) {
          const GenParticle & p = (*genParticles)[i];
          double DeltaRK = TMath::Sqrt((p.eta()-trk.eta())*(p.eta()-trk.eta())+
          (p.phi()-trk.phi())*(p.phi()-trk.phi()));
          if(DeltaRK<MinDRK && DeltaRK<0.05){MinDRK=DeltaRK; 
          MCKid=p.pdgId();
          MCKdr=MinDRK;
          if(p.mother()!=0) MCKmomid=p.mother()->pdgId();
          if(p.mother()!=0 && p.mother()->mother()!=0) MCKGmomid=p.mother()->mother()->pdgId(); }
      }
      }

   } 

// Jpsi + K*(K+pi-)
  if(GMuIn1!=-99 && GMuIn2!=-99){ 
   int Trk21In=-99,Trk22In=-99;  
   double MinBVtx2=-999;
   const Candidate & mu1jpsi = (*muons)[GMuIn1];
   const Candidate & mu2jpsi = (*muons)[GMuIn2];
   Handle<CandidateView> AllTracks2;
   iEvent.getByLabel("allTracks2",AllTracks2);
   for(size_t j=0;j < AllTracks2->size(); ++j){
    for(size_t k=j+1; k< AllTracks2->size(); ++k){   
     const Candidate & trk1 =(*AllTracks2)[j];
     const Candidate & trk2 =(*AllTracks2)[k]; 
     if(trk1.charge()==trk2.charge()) continue;

//     if(trk1.pt()<0.5) continue;
//     if(trk2.pt()<0.5) continue;
     if(trk1.pt()<0.8) continue;
     if(trk2.pt()<0.8) continue;


   CompositeCandidate Jpsicand;
   Jpsicand.addDaughter(mu1jpsi);Jpsicand.addDaughter(mu2jpsi);
   AddFourMomenta addP40; addP40.set(Jpsicand);

   double deltaR1=TMath::Sqrt((Jpsicand.eta()-trk1.eta())*(Jpsicand.eta()-trk1.eta())+
   (Jpsicand.phi()-trk1.phi())*(Jpsicand.phi()-trk1.phi()));
//   if(deltaR1>1.5) continue;
   double deltaR2=TMath::Sqrt((Jpsicand.eta()-trk2.eta())*(Jpsicand.eta()-trk2.eta())+
   (Jpsicand.phi()-trk2.phi())*(Jpsicand.phi()-trk2.phi()));
//   if(deltaR2>1.5) continue;
   TrackRef trkkst1 = trk1.get<TrackRef>();
   TrackRef trkkst2 = trk2.get<TrackRef>();
////   if(trkkst1.get()->numberOfValidHits()<5) continue;
////   if(trkkst2.get()->numberOfValidHits()<5) continue;

      double KaonMassSq=0.243717;
      double KaonE1=sqrt(KaonMassSq+trk1.px()*trk1.px()+trk1.py()*trk1.py()+trk1.pz()*trk1.pz());
      double KaonE2=sqrt(KaonMassSq+trk2.px()*trk2.px()+trk2.py()*trk2.py()+trk2.pz()*trk2.pz());
 
      int K1flag=0, K2flag=0;
      double Kstmass1  = sqrt((KaonE1+trk2.energy())*(KaonE1+trk2.energy())
   -(trk1.px()+trk2.px())*(trk1.px()+trk2.px())
   -(trk1.py()+trk2.py())*(trk1.py()+trk2.py())
   -(trk1.pz()+trk2.pz())*(trk1.pz()+trk2.pz()));
      double Kstmass2  = sqrt((KaonE2+trk1.energy())*(KaonE2+trk1.energy())
   -(trk1.px()+trk2.px())*(trk1.px()+trk2.px())
   -(trk1.py()+trk2.py())*(trk1.py()+trk2.py())
   -(trk1.pz()+trk2.pz())*(trk1.pz()+trk2.pz()));
    if(abs(Kstmass1-0.892) < abs(Kstmass2-0.892)){
    if(abs(Kstmass1-0.892)>0.12) continue; K1flag=1;
    } else{
    if(abs(Kstmass2-0.892)>0.12) continue; K2flag=1;
    }
    
   if(abs(Jpsicand.mass()-3.097)>0.15) continue;
   OverlapChecker overlap;
   if(overlap(mu1jpsi,trk1)!=0 || overlap(mu2jpsi,trk1)!=0) continue;
   if(overlap(mu1jpsi,trk2)!=0 || overlap(mu2jpsi,trk2)!=0) continue;

   CompositeCandidate Bcand;
   Bcand.addDaughter(mu1jpsi);Bcand.addDaughter(mu2jpsi);Bcand.addDaughter(trk1); 
   Bcand.addDaughter(trk2);
   AddFourMomenta addP4; addP4.set(Bcand);

// isolation
   double sumpt=0.00000000001;
   for(size_t k2=0; k2< AllTracks2->size(); ++k2){
   const Candidate & trkT =(*AllTracks2)[k2];
   if(TMath::Sqrt((Bcand.eta()-trkT.eta())*(Bcand.eta()-trkT.eta())+
    (Bcand.phi()-trkT.phi())*(Bcand.phi()-trkT.phi())) >1.2) continue;
   sumpt+=trkT.pt();
   }
   if(Bcand.pt()/sumpt<0.4) continue;
//


   edm::ESHandle<TransientTrackBuilder> theB;
   iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theB);
   TrackRef trkMu1Ref = mu1jpsi.get<TrackRef>();
   TrackRef trkMu2Ref = mu2jpsi.get<TrackRef>();
//   TrackRef trkkst1 = trk1.get<TrackRef>();
//   TrackRef trkkst2 = trk2.get<TrackRef>();
   vector<TransientTrack> t_tks;
   t_tks.push_back((*theB).build(&trkMu1Ref)); 
   t_tks.push_back((*theB).build(&trkMu2Ref)); 
   t_tks.push_back((*theB).build(&trkkst1)); 
   t_tks.push_back((*theB).build(&trkkst2)); 

   if(!trkMu1Ref.isNonnull() || !trkMu2Ref.isNonnull() || !trkkst1.isNonnull() || !trkkst2.isNonnull() ) continue;
   TransientTrack ttMuP= t_tks[0];
   TransientTrack ttMuM= t_tks[1];
   TransientTrack ttK = t_tks[2];
   TransientTrack ttP = t_tks[3];
   KinematicParticleFactoryFromTransientTrack pFactory;
   ParticleMass muon_mass = 0.1056583;
   ParticleMass kaon_mass = 0.493677;
   ParticleMass pi_mass = 0.139570;
   float muon_sigma = 0.0000001;
   float kaon_sigma = 0.000016;
   float pi_sigma = 0.000016;
   float chi = 0.;
   float ndf = 0.;
      vector<RefCountedKinematicParticle> allParticles;
      allParticles.push_back(pFactory.particle (ttMuP,muon_mass,chi,ndf,muon_sigma));
      allParticles.push_back(pFactory.particle (ttMuM,muon_mass,chi,ndf,muon_sigma));
      if(K1flag==1) {allParticles.push_back(pFactory.particle (ttK,kaon_mass,chi,ndf,kaon_sigma));
      allParticles.push_back(pFactory.particle (ttP,pi_mass,chi,ndf,pi_sigma));}
      else{allParticles.push_back(pFactory.particle (ttP,kaon_mass,chi,ndf,kaon_sigma));
      allParticles.push_back(pFactory.particle (ttK,pi_mass,chi,ndf,pi_sigma));}
      ParticleMass jpsi = 3.09687;
      MultiTrackKinematicConstraint *  j_psi_c = new  TwoTrackMassKinematicConstraint(jpsi);
      KinematicConstrainedVertexFitter kcvFitter;
      RefCountedKinematicTree myTree = kcvFitter.fit(allParticles, j_psi_c);
      myTree->movePointerToTheTop();
      RefCountedKinematicParticle bmes = myTree->currentParticle();
      RefCountedKinematicVertex b_dec_vertex = myTree->currentDecayVertex();
      if(!b_dec_vertex->vertexIsValid()) continue;
      AlgebraicVector7 b_par = bmes->currentState().kinematicParameters().vector();
      AlgebraicSymMatrix77 bs_er = bmes->currentState().kinematicParametersError().matrix();
      double vtxProb = TMath::Prob(bmes->chiSquared(),bmes->degreesOfFreedom());

//////      if(b_par[6]<4.9 ||  b_par[6]>5.6) continue;
/////    if(trkkst1.get()->normalizedChi2()>10. || trkkst1.get()->numberOfValidHits()<5) continue;
/////    if(trkkst2.get()->normalizedChi2()>10. || trkkst2.get()->numberOfValidHits()<5) continue;
    int kaonmuOV=0;
    for(size_t k = 0; k < muons->size(); ++ k) {
    const Muon & mu2= (*muons)[k];
    if(mu2.isGlobalMuon() && mu2.isGood(Muon::TrackerMuonArbitrated) &&
       mu2.isGood(Muon::TMOneStationLoose)  &&
       muon::isGoodMuon(mu2,Muon::TM2DCompatibilityLoose)){
    if(overlap(mu2,trk1)!=0) kaonmuOV++;  }
    }
/////    if(kaonmuOV!=0) continue;
    int kaonmuOV2=0;
    for(size_t k = 0; k < muons->size(); ++ k) {
    const Muon & mu2= (*muons)[k];
    if(mu2.isGlobalMuon() && mu2.isGood(Muon::TrackerMuonArbitrated) &&
       mu2.isGood(Muon::TMOneStationLoose)  &&
       muon::isGoodMuon(mu2,Muon::TM2DCompatibilityLoose)){
    if(overlap(mu2,trk2)!=0) kaonmuOV2++;  }
    }
/////    if(kaonmuOV2!=0) continue;

/*     int pixhits=0;
     const reco::HitPattern& p1 = trkkst1.get()->hitPattern();
     for (int i=0; i<p1.numberOfHits(); i++) {       uint32_t hit = p1.getHitPattern(i);
       if (p1.validHitFilter(hit) && p1.pixelBarrelHitFilter(hit)) pixhits++;
     }
     if(pixhits==0) continue;
     int pixhits2=0;
     const reco::HitPattern& p2 = trkkst2.get()->hitPattern();
     for (int i=0; i<p2.numberOfHits(); i++) {       uint32_t hit = p2.getHitPattern(i);
       if (p2.validHitFilter(hit) && p2.pixelBarrelHitFilter(hit)) pixhits2++;
     }
     if(pixhits2==0) continue;
*/

//   KalmanVertexFitter kvf;
//   TransientVertex tv = kvf.vertex(t_tks);
//   if(!tv.isValid()) continue;
//   Vertex vertex =tv;
//   GlobalPoint secondaryVertex = tv.position();
//   double vtxProb = TMath::Prob(vertex.chi2(),vertex.ndof());
      if(b_par[6]>4.9 && b_par[6]<5.6) ncomb3++;
      HistMass3->Fill(b_par[6]);
     if(K1flag==1) HistMass4->Fill(Kstmass1); else if(K2flag==1) HistMass4->Fill(Kstmass2);
   if(vtxProb>MinBVtx2){
   MinBVtx2=vtxProb;
   if(K1flag==1){
   Bmass2 = sqrt((KaonE1+trk2.energy()+mu1jpsi.energy()+mu2jpsi.energy())*(KaonE1+trk2.energy()+mu1jpsi.energy()+mu2jpsi.energy())
   -(trk1.px()+trk2.px()+mu1jpsi.px()+mu2jpsi.px())*(trk1.px()+trk2.px()+mu1jpsi.px()+mu2jpsi.px())
   -(trk1.py()+trk2.py()+mu1jpsi.py()+mu2jpsi.py())*(trk1.py()+trk2.py()+mu1jpsi.py()+mu2jpsi.py())
   -(trk1.pz()+trk2.pz()+mu1jpsi.pz()+mu2jpsi.pz())*(trk1.pz()+trk2.pz()+mu1jpsi.pz()+mu2jpsi.pz()));
   KstMass=Kstmass1;}
   else if(K2flag==1){
   Bmass2 = sqrt((KaonE2+trk1.energy()+mu1jpsi.energy()+mu2jpsi.energy())*(KaonE2+trk1.energy()+mu1jpsi.energy()+mu2jpsi.energy())
   -(trk1.px()+trk2.px()+mu1jpsi.px()+mu2jpsi.px())*(trk1.px()+trk2.px()+mu1jpsi.px()+mu2jpsi.px())
   -(trk1.py()+trk2.py()+mu1jpsi.py()+mu2jpsi.py())*(trk1.py()+trk2.py()+mu1jpsi.py()+mu2jpsi.py())
   -(trk1.pz()+trk2.pz()+mu1jpsi.pz()+mu2jpsi.pz())*(trk1.pz()+trk2.pz()+mu1jpsi.pz()+mu2jpsi.pz()));
   KstMass=Kstmass2;}
   Bmass2C=b_par[6];  
   
   Bvtxprob2=vtxProb;
   if(K1flag==1) {Trk21In=j; Trk22In=k;}
   else if(K2flag==1) {Trk21In=k; Trk22In=j;}
   Bphi2=Bcand.phi();
   BPt2=Bcand.pt();  
   BPx2=Bcand.px();
   BPy2=Bcand.py();
   Bsumpt2=sumpt;
//   if(Bcand.pt()!=0) B2Lxy=((secondaryVertex.x()-BSx)*Bcand.px()+(secondaryVertex.y()-BSy)*Bcand.py())/Bcand.pt();
//   if(Bcand.pt()!=0) B2Lxy2=((secondaryVertex.x()-PVx)*Bcand.px()+(secondaryVertex.y()-PVy)*Bcand.py())/Bcand.pt();
   if(Bcand.pt()!=0) B2Lxy=((b_dec_vertex->position().x()-BSx)*Bcand.px()+
    (b_dec_vertex->position().y()-BSy)*Bcand.py())/Bcand.pt();
   if(Bcand.pt()!=0) B2Lxy2=((b_dec_vertex->position().x()-PVx)*Bcand.px()+
    (b_dec_vertex->position().y()-PVy)*Bcand.py())/Bcand.pt();
   Berrx2=bs_er(1,1);
   Berry2=bs_er(2,2);
   Berrxy2=bs_er(1,2); 

   kst1pt=trk1.pt();
   kst2pt=trk2.pt();
   kst1eta=trk1.eta();
   kst2eta=trk2.eta();
   kst1phi=trk1.phi();
   kst2phi=trk2.phi();
    kst1chi2=trkkst1.get()->normalizedChi2();
    kst1nhits=trkkst1.get()->numberOfValidHits();
    kst2chi2=trkkst2.get()->normalizedChi2();
    kst2nhits=trkkst2.get()->numberOfValidHits();
     int pixhits=0;
     const reco::HitPattern& p = trkkst1.get()->hitPattern();
     for (int i=0; i<p.numberOfHits(); i++) {
       uint32_t hit = p.getHitPattern(i);
       if (p.validHitFilter(hit) && p.pixelBarrelHitFilter(hit)) pixhits++;
       if (p.validHitFilter(hit) && p.pixelEndcapHitFilter(hit)) pixhits++;
     }
    kst1pixH=pixhits;
    kst1trkH=p.numberOfValidTrackerHits();
     int pixhits2=0;
     const reco::HitPattern& p2 = trkkst2.get()->hitPattern();
     for (int i=0; i<p2.numberOfHits(); i++) {
       uint32_t hit = p2.getHitPattern(i);
       if (p2.validHitFilter(hit) && p2.pixelBarrelHitFilter(hit)) pixhits2++;
       if (p2.validHitFilter(hit) && p2.pixelEndcapHitFilter(hit)) pixhits2++;
     }
    kst2pixH=pixhits2;
    kst2trkH=p2.numberOfValidTrackerHits();

   }
   } //loop trk1
   } //loop trk2

      if(Trk21In!=-99){
      double MinDRK21=999.;
      const Candidate & trk =(*AllTracks2)[Trk21In];
         for(size_t i = 0; i < genParticles->size(); ++ i) {
          const GenParticle & p = (*genParticles)[i];
          double DeltaRK = TMath::Sqrt((p.eta()-trk.eta())*(p.eta()-trk.eta())+
          (p.phi()-trk.phi())*(p.phi()-trk.phi()));
          if(DeltaRK<MinDRK21 && DeltaRK<0.05){MinDRK21=DeltaRK;
          MCK21id=p.pdgId();
          MCK21dr=MinDRK21;
          if(p.mother()!=0) MCK21momid=p.mother()->pdgId();
          if(p.mother()!=0 && p.mother()->mother()!=0) MCK21Gmomid=p.mother()->mother()->pdgId(); }
      }
      }
      if(Trk22In!=-99){
      double MinDRK22=999.;
      const Candidate & trk =(*AllTracks2)[Trk22In];
         for(size_t i = 0; i < genParticles->size(); ++ i) {
          const GenParticle & p = (*genParticles)[i];
          double DeltaRK = TMath::Sqrt((p.eta()-trk.eta())*(p.eta()-trk.eta())+
          (p.phi()-trk.phi())*(p.phi()-trk.phi()));
          if(DeltaRK<MinDRK22 && DeltaRK<0.05){MinDRK22=DeltaRK;
          MCK22id=p.pdgId();
          MCK22dr=MinDRK22;
          if(p.mother()!=0) MCK22momid=p.mother()->pdgId();
          if(p.mother()!=0 && p.mother()->mother()!=0) MCK22Gmomid=p.mother()->mother()->pdgId(); }
      }
     }


   }  //Jpsi

     HistComb1->Fill(ncomb1);
     HistComb2->Fill(ncomb2);
     HistComb3->Fill(ncomb3);

// trk multiplicity
  
  for(int i=0; i<5; i++) NTrkStudy[i]=0;
    if(GMuIn1!=-99 && GMuIn2!=-99){
   const Candidate & mu1jpsi = (*muons)[GMuIn1];   
   const Candidate & mu2jpsi = (*muons)[GMuIn2];
   Handle<CandidateView> AllTracks;
   iEvent.getByLabel("allTracks",AllTracks);
   for(size_t j=0;j < AllTracks->size(); ++j){
     const Candidate & trk1 =(*AllTracks)[j];
   OverlapChecker overlap;
   if(overlap(mu1jpsi,trk1)!=0 || overlap(mu2jpsi,trk1)!=0) continue;
   TrackRef trkRef1 = trk1.get<TrackRef>();
   if(trkRef1.get()->normalizedChi2()>=1.9 || trkRef1.get()->numberOfValidHits()<=11) continue;
   if(trk1.pt()>3.) NTrkStudy[0]++;
   if(trk1.pt()>3. && trk1.pt()<5.) NTrkStudy[1]++;
   if(trk1.pt()>5. && trk1.pt()<8.) NTrkStudy[2]++;
   if(trk1.pt()>8. && trk1.pt()<11.) NTrkStudy[3]++;
   if(trk1.pt()>11.) NTrkStudy[4]++;
   }
   }

///////// D0(Kpi)+mu

   double MinBVtxDsl=-999.;
   Handle<CandidateView> AllTracksD;
   iEvent.getByLabel("allTracks2",AllTracksD);
   for(size_t j=0;j < AllTracksD->size(); ++j){
    for(size_t k=j+1; k< AllTracksD->size(); ++k){
     const Candidate & trk1 =(*AllTracksD)[j];
     const Candidate & trk2 =(*AllTracksD)[k];
     if(trk1.charge()==trk2.charge()) continue;
      double KaonMassSq=0.243717;
      double KaonE1=sqrt(KaonMassSq+trk1.px()*trk1.px()+trk1.py()*trk1.py()+trk1.pz()*trk1.pz());
      double KaonE2=sqrt(KaonMassSq+trk2.px()*trk2.px()+trk2.py()*trk2.py()+trk2.pz()*trk2.pz());

     double DmassT1  = sqrt((KaonE1+trk2.energy())*(KaonE1+trk2.energy())
   -(trk1.px()+trk2.px())*(trk1.px()+trk2.px())
   -(trk1.py()+trk2.py())*(trk1.py()+trk2.py())
   -(trk1.pz()+trk2.pz())*(trk1.pz()+trk2.pz()));
     double DmassT2  = sqrt((KaonE2+trk1.energy())*(KaonE2+trk1.energy())
   -(trk1.px()+trk2.px())*(trk1.px()+trk2.px())
   -(trk1.py()+trk2.py())*(trk1.py()+trk2.py())
   -(trk1.pz()+trk2.pz())*(trk1.pz()+trk2.pz()));

    OverlapChecker overlap;
    for(size_t kk = 0; kk < muons->size(); ++ kk) {
    const Candidate & trkM = (*muons)[kk];
    if(overlap(trkM,trk1)!=0 || overlap(trkM,trk2)!=0) continue;

   edm::ESHandle<TransientTrackBuilder> theB;
   iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theB);
   TrackRef trkD1 = trk1.get<TrackRef>();
   TrackRef trkD2 = trk2.get<TrackRef>();
   TrackRef trkD3 = trkM.get<TrackRef>();
   if(!trkD1.isNonnull() || !trkD2.isNonnull() || !trkD3.isNonnull()) continue;
   vector<TransientTrack> t_tks;
   t_tks.push_back((*theB).build(&trkD1));
   t_tks.push_back((*theB).build(&trkD2));
   t_tks.push_back((*theB).build(&trkD3));
   KalmanVertexFitter kvf;
   TransientVertex tv = kvf.vertex(t_tks);
   if(!tv.isValid()) continue;
   Vertex vertex =tv;
   GlobalPoint secondaryVertex = tv.position();
   double vtxProb = TMath::Prob(vertex.chi2(),vertex.ndof());

/*     double DsmassT1  = sqrt((KaonE1+trk2.energy()+trkS.energy())*(KaonE1+trk2.energy()+trkS.energy())
   -(trk1.px()+trk2.px()+trkS.px())*(trk1.px()+trk2.px()+trkS.px())
   -(trk1.py()+trk2.py()+trkS.py())*(trk1.py()+trk2.py()+trkS.py())
   -(trk1.pz()+trk2.pz()+trkS.pz())*(trk1.pz()+trk2.pz()+trkS.pz()));
     double DsmassT2  = sqrt((KaonE2+trk1.energy()+trkS.energy())*(KaonE2+trk1.energy()+trkS.energy())
   -(trk1.px()+trk2.px()+trkS.px())*(trk1.px()+trk2.px()+trkS.px())
   -(trk1.py()+trk2.py()+trkS.py())*(trk1.py()+trk2.py()+trkS.py())
   -(trk1.pz()+trk2.pz()+trkS.pz())*(trk1.pz()+trk2.pz()+trkS.pz()));
*/
   int Flag=0;
   if(abs(DmassT1-1.8645)<abs(DmassT2-1.8645)) Flag=1;
   else Flag=2;

   if(vtxProb>MinBVtxDsl){
   MinBVtxDsl=vtxProb;

    if(Flag==1) {Dmass1=DmassT1; Dmass2=DmassT2;}
    if(Flag==2) {Dmass1=DmassT2; Dmass2=DmassT1;}
    DvtxProb=vtxProb;
    trkD1pt=trk1.pt(); trkD2pt=trk2.pt(); trkD3pt=trkM.pt();
    trk1chi2=trkD1.get()->normalizedChi2();
    trk2chi2=trkD2.get()->normalizedChi2();
    trk3chi2=trkD3.get()->normalizedChi2();
    trk1nhits=trkD1.get()->numberOfValidHits();
    trk2nhits=trkD2.get()->numberOfValidHits();
    trk3nhits=trkD3.get()->numberOfValidHits();
    trk1muov=0;
    trk2muov=0;
    for(size_t kkk = 0; kkk < muons->size(); ++ kkk) {
    const Muon & mu2= (*muons)[kkk];
    if(mu2.isGood(Muon::TrackerMuonArbitrated) &&
       mu2.isGood(Muon::TMOneStationLoose)  &&
       muon::isGoodMuon(mu2,Muon::TM2DCompatibilityLoose)){
    if(overlap(mu2,trk1)!=0) trk1muov++; 
    if(overlap(mu2,trk2)!=0) trk2muov++;
       }
    }
   
   } //best cand

  } //trk3
  } //trk2
  }//trk1


// vertex by hand
    edm::ESHandle<TransientTrackBuilder> theB;
    iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theB);
// do the vertex fit
   float lxy=-99.,lxyerr=-99.;
   if(GMuIn1!=-99 && GMuIn2!=-99){
    const Candidate & trkMu1 = (*muons)[GMuIn1];
    const Candidate & trkMu2 = (*muons)[GMuIn2];   
   TrackRef trkMu1Ref = trkMu1.get<TrackRef>();
   TrackRef trkMu2Ref = trkMu2.get<TrackRef>();

    vector<TransientTrack> t_tks;
    t_tks.push_back((*theB).build(&trkMu1Ref));
    t_tks.push_back((*theB).build(&trkMu2Ref));
                                  
    //    if (t_tks.size()!=3) continue;
                         
    KalmanVertexFitter kvf;
    TransientVertex tv = kvf.vertex(t_tks);

     if (tv.isValid()) {
                      
    Vertex vertex = tv;
                        
   // get vertex position and error to calculate the decay length significance
    GlobalPoint secondaryVertex = tv.position();
    GlobalError err = tv.positionError();
 
    //calculate decay length  significance w.r.t. the beamspot
     GlobalPoint displacementFromBeamspot( -1*((vertexBeamSpot.x0() -secondaryVertex.x()) +  (secondaryVertex.z() - vertexBeamSpot.z0()) * vertexBeamSpot.dxdz()), -1*((vertexBeamSpot.y0() - secondaryVertex.y())+ (secondaryVertex.z() -vertexBeamSpot.z0()) * vertexBeamSpot.dydz()), 0);
 
    lxy = displacementFromBeamspot.perp();
    lxyerr = sqrt(err.rerr(displacementFromBeamspot));
   } else {lxy=99.;lxyerr=99.;}
  } 

   if(GMuIn1!=-99 && GMuIn2!=-99){
    for(size_t k = 0; k < muons->size(); ++ k) {
     if(k==GMuIn1 || k==GMuIn2) continue;
      const Muon & trk = (*muons)[k];
//     const Candidate & trk = (*muons)[k];
      if(!trk.isGlobalMuon() && !trk.isTrackerMuon()) continue;
  //    if(!trk.isGlobalMuon()) continue;
      if(!trk.isGood(Muon::TrackerMuonArbitrated)) continue;
      if(!trk.isGood(Muon::TMOneStationLoose)) continue;
      if(!muon::isGoodMuon(trk,Muon::TM2DCompatibilityLoose)) continue;
      const Track* MuTrk = trk.innerTrack().get();
      if(MuTrk->numberOfValidHits()<=11) continue;
      if(MuTrk->normalizedChi2()>=1.9) continue;
      double ptM=trk.pt();
     if(ptM>ptMax) {GMuSlIn=k; MuSlPhi=trk.phi(); ptMax=ptM; }
    }
   }


//electron block
  for(size_t j=0; j < gsfElectrons->size(); ++j){
   const GsfElectron & elec = (*gsfElectrons)[j]; 
   double ptE=elec.pt();
     if(ptE>ptElecMax) {GElecSlIn=j; ptElecMax=ptE; }
  }

  

   if(JpsiPhi!=-999. && MuSlPhi!=-999.){
     double dPhiRec=abs(JpsiPhi-MuSlPhi);
     if(dPhiRec>3.1457) dPhiRec=2.*3.1457-dPhiRec;
     HistPhiRec->Fill(dPhiRec);
   }


/// MC truth matching

 int Nb=0,Nbb=0, BBEvt=0;
 int bIn=-99,bbarIn=-99;
 int JpsiIn=-99, MuIn=-99; 

  int Njpsi=0;

  NevtJpsi=0;
  NevtMuMcSl=0;
  NevtMu=0;
  NevtElec=0;
  // for photon for now only variable in module
  NevtPho=0;

      
   for(size_t i = 0; i < genParticles->size(); ++ i) {
     const GenParticle & p = (*genParticles)[i];
     int id = p.pdgId();
     int st = p.status();  

      if(abs(id)==5) BBEvt++;
      // Find b and bbar
      if (st==3) {
        if (id==5) Nb++;
        if (id==-5) Nbb++;
      } else if (st==2) {
        if (id==5) bIn = i;
	if (id==-5) bbarIn = i;
      }


     if(id==443 && st==2) {
      JpsiIn=i; fHist2muMass->Fill(p.mass());
       int ndau = p.numberOfDaughters();
       if(p.pt()!=0) MCJpsiLxy=((p.vx()-BSx)*p.px()+(p.vy()-BSy)*p.py())/p.pt();
        for(size_t j = 0; j < ndau; ++ j) {
         const Candidate * Jpsidau = p.daughter( j );
         int dauId = Jpsidau->pdgId();
           if(dauId==13) fHistMuDauPt1->Fill(Jpsidau->pt());
           if(dauId==-13) fHistMuDauPt2->Fill(Jpsidau->pt()); 
        }
      }
     
      if(id==443 && st==2) {
       NevtJpsi++;
       const Candidate * mom(0),*Gmom(0),*GGmom(0);
       mom=p.mother(0);
       if(mom!=0) Gmom=p.mother(0)->mother(0); 
       if(mom!=0 && Gmom!=0) GGmom=p.mother(0)->mother(0)->mother(0);
       if(mom!=0) MCJpsiMothId=abs(mom->pdgId());
       if(Gmom!=0) MCJpsiGMothId=abs(Gmom->pdgId());
       if(GGmom!=0) MCJpsiGGMothId=abs(GGmom->pdgId());
       }    

       if(abs(id)==521){
        MCBpPt=p.pt();
        MCBpP=p.p();
        MCBpEta=p.eta();
        if(p.pt()!=0)  MCBpLxy=((p.vx()-BSx)*p.px()+(p.vy()-BSy)*p.py())/p.pt();
        MCBpNdau=p.numberOfDaughters();
        for(size_t j = 0; j < MCBpNdau; ++ j) {
         const Candidate * Bpdau = p.daughter( j );
         MCBpdauId[j] = Bpdau->pdgId();
         if(Bpdau->pt()!=0 && Bpdau->pdgId()==443) MCBpdauLxy=((Bpdau->vx()-BSx)*Bpdau->px()+(Bpdau->vy()-BSy)*Bpdau->py())/Bpdau->pt();
        }
       }  

       if(abs(id)==511){
        MCB0Pt=p.pt();
        MCB0P=p.p();
        MCB0Eta=p.eta();
        MCB0Ndau=p.numberOfDaughters();
        for(size_t j = 0; j < MCB0Ndau; ++ j) {
         const Candidate * B0dau = p.daughter( j );
         MCB0dauId[j] = B0dau->pdgId();
         if(B0dau->pt()!=0 && B0dau->pdgId()==443) MCB0dauLxy=((B0dau->vx()-BSx)*B0dau->px()+(B0dau->vy()-BSy)*B0dau->py())/B0dau->pt();
         if(abs(B0dau->pdgId())==313 && 
     (abs((B0dau->daughter(0))->pdgId())==321 || abs((B0dau->daughter(0))->pdgId())==211)) MCB0Kstc=1; else MCB0Kstc=0;
        }
       } 


//     double pt = p.pt(), eta = p.eta(), phi = p.phi(), mass = p.mass();
//     double vx = p.vx(), vy = p.vy(), vz = p.vz();
//     int charge = p.charge();
  //   if(abs(id)==13) {
  //    const Candidate * mom=p.mother(0);
  //     if(abs(mom->pdgId()) != 521 && abs(mom->pdgId())!=511) continue; 
         
 /*      if(abs(id) == 511 || abs(id) == 521){
          int n = p.numberOfDaughters();
           for(size_t j = 0; j < n; ++ j) {
           const Candidate * Bdau = p.daughter( j );
           int dauId = Bdau->pdgId();
           if(abs(dauId)==13){ 
             float deltaPhi(0.);
             deltaPhi=fabs(Jpsi.phi()-Bdau->phi());
             if(deltaPhi>3.1457) deltaPhi=2*3.1457-deltaPhi;
              fHistDeltaPhi->Fill(deltaPhi) ;
            }
          }
        } 
 */
      if(abs(id) == 13){
          int n = p.numberOfMothers();
           for(size_t jj = 0; jj < n; ++ jj) {
           const Candidate * mom = p.mother( jj );
           int momId = mom->pdgId();
           if(abs(momId)!=443) MuIn=i; 
           if(abs(momId)!=443 && abs(momId)>400 && abs(momId)<600) NevtMuMcSl++;
           }
       } 
           
	   
   //   double deltaPhi(0);
   //   deltaPhi=abs(Jpsi.phi()-p.phi());
   //   if(deltaPhi>3.1457) deltaPhi=2*3.1457-deltaPhi;
   //   fHistDeltaPhi->Fill(deltaPhi) ;   
      
//        int n = p.numberOfDaughters();
//        for(size_t j = 0; j < n; ++ j) {
//        const Candidate * d = p.daughter( j );
//        int dauId = d->pdgId();
//        }
   //   }
   
   }

    int MCMuIn[100];
    int MCElecIn[100];
    int MCPhoIn[1000];
    for(int i=0;i<100;i++) {MCMuIn[i]=-99;MCElecIn[i]=-99;}
    for(int i=0;i<1000;i++) MCPhoIn[i]=-99;

   for(size_t i = 0; i < genParticles->size(); ++ i) {
     const GenParticle & p = (*genParticles)[i];
     int id = p.pdgId();
     if(abs(id)==13){
     MCMuIn[NevtMu]=i; 
     if(p.mother(0)!=0) TruMCMuMothId[NevtMu]=p.mother(0)->pdgId();
       if(p.mother(0)!=0){
        if(p.mother(0)->mother(0)!=0) TruMCMuGMothId[NevtMu]=p.mother(0)->mother(0)->pdgId();
        if(p.mother(0)->mother(0)!=0){ 
        if(p.mother(0)->mother(0)->mother(0)!=0) TruMCMuGGMothId[NevtMu]=p.mother(0)->mother(0)->mother(0)->pdgId();
        }
       } 
     NevtMu++;
     }
     if(abs(id)==11){
     MCElecIn[NevtElec]=i;
     if(p.mother(0)!=0) TruMCElecMothId[NevtElec]=p.mother(0)->pdgId();
       if(p.mother(0)!=0){
        if(p.mother(0)->mother(0)!=0) TruMCElecGMothId[NevtElec]=p.mother(0)->mother(0)->pdgId();
        if(p.mother(0)->mother(0)!=0){
        if(p.mother(0)->mother(0)->mother(0)!=0) TruMCElecGGMothId[NevtElec]=p.mother(0)->mother(0)->mother(0)->pdgId();
        }
       }
     NevtElec++;
     }
     if(abs(id)==22){
     MCPhoIn[NevtPho]=i; NevtPho++;
     }
    }

// simple flags
   elecSlFlag=0, elecConvFlag=0, elecFakeFlag=0;
   for(int i=0;i<NevtElec;i++){ 
   if(abs(TruMCElecMothId[i])>400 && abs(TruMCElecMothId[i])<10000) elecSlFlag++;
   if(abs(TruMCElecMothId[i])==111) elecConvFlag++;
   if(TruMCElecMothId[i]==-99) elecFakeFlag++;}
//////

      if(JpsiIn!=-99 && MuIn!=-99 && bIn!=-99 && bbarIn!=-99) {
     const GenParticle & Jpsi = (*genParticles)[JpsiIn];
     const GenParticle & Mu = (*genParticles)[MuIn];
 
           float deltaPhi(0.);
             deltaPhi=fabs(Jpsi.phi()-Mu.phi());
             if(deltaPhi>3.1457) deltaPhi=2.*3.1457-deltaPhi;
             
        fHistDeltaPhi->Fill(deltaPhi) ;
        fHistMuSLPt->Fill(Mu.pt());

       const Candidate & bquark = (*genParticles)[bIn];
       const Candidate & bbquark = (*genParticles)[bbarIn];
       double dPhibb=fabs(bquark.phi()-bbquark.phi());
       if(dPhibb>3.1457) dPhibb=2.*3.1457-dPhibb;
      
       fHistRes->Fill(deltaPhi-dPhibb);

      }
   
      if(bIn!=-99 && bbarIn!=-99){
         const Candidate & bquark = (*genParticles)[bIn];
        const Candidate & bbquark = (*genParticles)[bbarIn];
        double       dPhibb=fabs(bquark.phi()-bbquark.phi());

        if (Nb > 0 && Nbb > 0) {
           BBdphiFC->Fill(dPhibb); BBType=1;
        } else if ((Nb > 0 && Nbb == 0) || (Nb == 0 && Nbb > 0)) {
           BBdphiFEX->Fill(dPhibb);BBType=2;
        } else if (Nb == 0 && Nbb == 0) {
           BBdphiGS->Fill(dPhibb); BBType=3;
        } 

      } else if (BBEvt==0) BBType=4;

      MCnb=0,MCnbb=0;
      MCnb=Nb;
      MCnbb=Nbb;

///////////////////////////////////////////////
  NevtKaon1=0,NevtKaon2=0,NevtKaon3=0,NevtKaon4=0;
  Handle<GenParticleMatch> KmcMatchMap;
  iEvent.getByLabel( "KaonMCMatch", KmcMatchMap );

  // create the extended matcher that includes automatic parent matching
  //  MCCandMatcher<CandidateCollection, GenParticleCollection> match( * mcMatchMap );
  utilsNew::CandMatcher<GenParticleCollection> Kmatch(*KmcMatchMap);
 Handle<CandidateView> srcTracks;
 iEvent.getByLabel("allTracks", srcTracks);
      for(size_t j=0;j<srcTracks->size();++j){
       CandidateBaseRef trk = srcTracks->refAt(j);
       GenParticleRef mcKaon = (*KmcMatchMap )[trk];
       if(mcKaon.get()==0) continue;
       const Candidate & track =(*srcTracks)[j];
       TrackRef trkkaon = track.get<TrackRef>();
       if(trkkaon.get()->normalizedChi2()>=1.9 || trkkaon.get()->numberOfValidHits()<=11) continue; 
       if(trk.get()->pt()>3 && trk.get()->pt()<=5) NevtKaon1++;
       if(trk.get()->pt()>5 && trk.get()->pt()<=8) NevtKaon2++;
       if(trk.get()->pt()>8 && trk.get()->pt()<=11) NevtKaon3++;
       if(trk.get()->pt()>11) NevtKaon4++;
       }

////////////////////////////////////////////////

// truth matching 
 Handle<CandidateView> src;
//  iEvent.getByLabel("allGlobalMuonTracks", src); 
  iEvent.getByLabel("muons", src);
      for(size_t j=0;j<src->size();++j){
       CandidateBaseRef trk = src->refAt(j);
       GenParticleRef mcmu = (*mcMatchMap )[trk];
     //  cout << "here! " << mcmu.get() << endl;
       }

 Handle<CandidateView> Jpsisrc;
  iEvent.getByLabel("fittedJpsi", Jpsisrc);


 if(GMuIn1!=-99 && GMuIn2!=-99){
    CandidateBaseRef muJpsi1 = src->refAt(GMuIn1);
    CandidateBaseRef muJpsi2 = src->refAt(GMuIn2);
   
    GenParticleRef mcmuJpsi1 = (*mcMatchMap )[muJpsi1];
    GenParticleRef mcmuJpsi2 = (*mcMatchMap )[muJpsi2];
 
       if(GMuSlIn!=-99){
        if(GMuIn1==GMuIn2 || GMuIn1==GMuSlIn || GMuIn2==GMuSlIn) Njpsi++;
        CandidateBaseRef muSl = src->refAt(GMuSlIn); 
        if(muJpsi1==muJpsi2 || muJpsi1==muSl || muJpsi2==muSl) Njpsi++;       
        GenParticleRef mcmuSl = (*mcMatchMap )[muSl];
        if(mcmuJpsi1.get()==mcmuJpsi2.get() || mcmuJpsi1.get()==mcmuSl.get() || mcmuJpsi2.get()==mcmuSl.get()) Njpsi++;
        }
   
    if(mcmuJpsi1.get()!=0 && mcmuJpsi2.get()!=0){
       TrueMuJpsi1Id=mcmuJpsi1.get()->pdgId();
       TrueMuJpsi2Id=mcmuJpsi2.get()->pdgId(); 
      int nMoth1 = mcmuJpsi1.get()->numberOfMothers();
       for(size_t jj = 0; jj < nMoth1; ++ jj) {
       const Candidate * mom = mcmuJpsi1.get()->mother( jj );
       TrueMuJpsi1MothId[jj] = mom->pdgId();
       }
       TrueMuJpsi1GMothId= mcmuJpsi1.get()->mother(0)->mother(0)->pdgId();
       TrueMuJpsi1GGMothId= mcmuJpsi1.get()->mother(0)->mother(0)->mother(0)->pdgId(); 
       int nMoth2 = mcmuJpsi2.get()->numberOfMothers();
       for(size_t jj = 0; jj < nMoth2; ++ jj) {
       const Candidate * mom = mcmuJpsi2.get()->mother( jj );
       TrueMuJpsi2MothId[jj] = mom->pdgId();
       }
      TrueMuJpsi2GMothId= mcmuJpsi2.get()->mother(0)->mother(0)->pdgId();
      TrueMuJpsi2GGMothId= mcmuJpsi2.get()->mother(0)->mother(0)->mother(0)->pdgId();
      if(mcmuJpsi1.get()->mother(0)->pdgId() == mcmuJpsi2.get()->mother(0)->pdgId() ) {
      TrueJpsiMom=1; TrueJpsiId=mcmuJpsi1.get()->mother(0)->pdgId();
      TrueJpsiPt=mcmuJpsi1.get()->mother(0)->pt();
      TrueJpsiEta=mcmuJpsi1.get()->mother(0)->eta();
      TrueJpsiPhi= mcmuJpsi1.get()->mother(0)->phi();
      TrueJpsiMomId=mcmuJpsi1.get()->mother(0)->mother(0)->pdgId();
      TrueJpsiGMomId=mcmuJpsi1.get()->mother(0)->mother(0)->mother(0)->pdgId();}
      
     }
    } else {
         TrueMuJpsi1Id=-999;
         TrueMuJpsi2Id=-999;
         TrueJpsiId=-999;
         TrueJpsiPt=-999;
         TrueJpsiEta=-999;
         TrueJpsiPhi=-999; 
         TrueJpsiMom=-999;
         TrueJpsiMomId=-999;
         TrueJpsiGMomId=-999;
    }
 
    if(GMuSlIn!=-99){
      CandidateBaseRef muSl = src->refAt(GMuSlIn);
      GenParticleRef mcmuSl = (*mcMatchMap )[muSl];
      if(mcmuSl.get()!=0){
      TrueMuSlId=mcmuSl.get()->pdgId();
      //   TrueMuSlmotherId=mcmuSl.mother().pdgId(); 
          int nMoth = mcmuSl.get()->numberOfMothers();
	  // TrueMuSLNMoth=nMoth;
          for(size_t jj = 0; jj < nMoth; ++ jj) {
          const Candidate * mom = mcmuSl.get()->mother( jj );
          TrueMuMothId[jj] = mom->pdgId();
          }
	  if(mcmuSl.get()->mother(0)->mother(0)!=0) 
	  TrueMuGMothId=mcmuSl.get()->mother(0)->mother(0)->pdgId();
	  if(mcmuSl.get()->mother(0)->mother(0)->mother(0)->mother(0)!=0) 
	  TrueMuGGMothId=mcmuSl.get()->mother(0)->mother(0)->mother(0)->pdgId();    
      }
   } else {
         TrueMuSlId=-999;
	 TrueMuGMothId=-999;
	 TrueMuGGMothId=-999;  
   }

 Handle<CandidateView> srcE;
  iEvent.getByLabel("allElectrons", srcE); 
      if(GElecSlIn!=-99){
       CandidateBaseRef elecSl = srcE->refAt(GElecSlIn);  
       GenParticleRef mcelecSl = (*mcMatchMap)[elecSl];  
        if(mcelecSl.get()!=0){
         TrueElecSlId=mcelecSl.get()->pdgId();
          int nMoth = mcelecSl.get()->numberOfMothers();
          for(size_t jj = 0; jj < nMoth; ++ jj) {
          const Candidate * mom = mcelecSl.get()->mother( jj );
          TrueElecMothId[jj] = mom->pdgId();
          }
	  if(mcelecSl.get()->mother(0)->mother(0)!=0) 
	  TrueElecGMothId=mcelecSl.get()->mother(0)->mother(0)->pdgId();
	  if(mcelecSl.get()->mother(0)->mother(0)->mother(0)->mother(0)!=0) 
	  TrueElecGGMothId=mcelecSl.get()->mother(0)->mother(0)->mother(0)->pdgId(); 
        }
     } else{
     TrueElecSlId=-999;
     TrueElecGMothId=-999;
     TrueElecGGMothId=-999;
     }
    

       if(bIn!=-99 && bbarIn!=-99){
       const Candidate & bquark = (*genParticles)[bIn];
       const Candidate & bbquark = (*genParticles)[bbarIn];
       MCbphi=bquark.phi();
       MCbbarphi=bbquark.phi();
       MCbpt=bquark.pt();
       MCbbarpt=bbquark.pt(); 
       MCbeta=bquark.eta();
       MCbbeta=bbquark.eta();
       MCben=bquark.energy();
       MCbben=bbquark.energy();
       
       CompositeCandidate bbar;
       bbar.addDaughter( bquark );
       bbar.addDaughter( bbquark );     
       AddFourMomenta addP4;
       addP4.set( bbar );
       MCbbarmass = bbar.mass(); 

       }else {
       MCbphi=-999; MCbbarphi=-999; MCbpt=-999.;MCbbarpt=-999.; 
       MCbeta=-999; MCbbeta=-999; MCben=-999; MCbben=-999; MCbbarmass = -999.;}

    if(VtxJpsiIn!=-99){
    CandidateBaseRef Jpsi = Jpsisrc->refAt(VtxJpsiIn);
    cout << "fitted Jpsi ref: " << Jpsi.get() << endl;
    GenParticleRef mcJpsi = (*mcMatchMap )[Jpsi];
     if(mcJpsi.get()!=0){ 
     TrueVtxJpsiId = mcJpsi.get()->pdgId(); 
     } else {TrueVtxJpsiId=-88;}

    } else {
   TrueVtxJpsiId=-99;
    } 

// DeltaR Truth Matching
  int BestMCMuIn=-99, BestMCMuJpsi1In=-99, BestMCMuJpsi2In=-99, BestMCElecIn=-99, BestMCPhoIn=-99;
  double MinDRMu=999.,MinDRMuJpsi1=999.,MinDRMuJpsi2=999.,MinDRElec=999.,MinDRPho=999.; 

  if(GMuIn1!=-99){
    const Candidate & muJpsi1 = (*muons)[GMuIn1];
    for(int i=0;i<NevtMu;i++){
      if(MCMuIn[i]==-99) continue;
      const Candidate & muGen = (*genParticles)[MCMuIn[i]];
      DeltaRMu[i] = TMath::Sqrt((muGen.eta()-muJpsi1.eta())*(muGen.eta()-muJpsi1.eta())+
      (muGen.phi()-muJpsi1.phi())*(muGen.phi()-muJpsi1.phi()));
      if(DeltaRMu[i]<MinDRMuJpsi1 && DeltaRMu[i]<0.02){MinDRMuJpsi1=DeltaRMu[i];BestMCMuJpsi1In=MCMuIn[i]; }
    }
   }
  if(GMuIn2!=-99){
    const Candidate & muJpsi2 = (*muons)[GMuIn2];
    for(int i=0;i<NevtMu;i++){
      if(MCMuIn[i]==-99) continue;
      if(MCMuIn[i]==BestMCMuJpsi1In) continue;
      const Candidate & muGen = (*genParticles)[MCMuIn[i]];
      DeltaRMu[i] = TMath::Sqrt((muGen.eta()-muJpsi2.eta())*(muGen.eta()-muJpsi2.eta())+
      (muGen.phi()-muJpsi2.phi())*(muGen.phi()-muJpsi2.phi()));
      if(DeltaRMu[i]<MinDRMuJpsi2 && DeltaRMu[i]<0.02){MinDRMuJpsi2=DeltaRMu[i];BestMCMuJpsi2In=MCMuIn[i]; }
    }
   }

  if(GMuSlIn!=-99){
    const Candidate & muSL = (*muons)[GMuSlIn];
    for(int i=0;i<NevtMu;i++){
      if(MCMuIn[i]==-99) continue;
      if(MCMuIn[i]==BestMCMuJpsi1In || MCMuIn[i]==BestMCMuJpsi2In) continue;
      const Candidate & muGen = (*genParticles)[MCMuIn[i]];
      DeltaRMu[i] = TMath::Sqrt((muGen.eta()-muSL.eta())*(muGen.eta()-muSL.eta())+
      (muGen.phi()-muSL.phi())*(muGen.phi()-muSL.phi()));
      if(DeltaRMu[i]<MinDRMu && DeltaRMu[i]<0.02){MinDRMu=DeltaRMu[i];BestMCMuIn=MCMuIn[i]; }
    }
   }


  if(GElecSlIn!=-99){
    const Candidate & elecSL = (*gsfElectrons)[GElecSlIn];
    for(int i=0;i<NevtElec;i++){ 
      if(MCElecIn[i]==-99) continue;
      const Candidate & elecGen = (*genParticles)[MCElecIn[i]];
      DeltaRElec[i] = TMath::Sqrt((elecGen.eta()-elecSL.eta())*(elecGen.eta()-elecSL.eta())+
      (elecGen.phi()-elecSL.phi())*(elecGen.phi()-elecSL.phi()));
      if(DeltaRElec[i]<MinDRElec && DeltaRElec[i]<0.25){MinDRElec=DeltaRElec[i];BestMCElecIn=MCElecIn[i]; }
    }
    for(int i=0;i<NevtPho;i++){
      if(MCPhoIn[i]==-99) continue;
      const Candidate & phoGen = (*genParticles)[MCPhoIn[i]];
      DeltaRPho[i] = TMath::Sqrt((phoGen.eta()-elecSL.eta())*(phoGen.eta()-elecSL.eta())+
      (phoGen.phi()-elecSL.phi())*(phoGen.phi()-elecSL.phi()));
      if(DeltaRPho[i]<MinDRPho && DeltaRPho[i]<0.1){MinDRPho=DeltaRPho[i];BestMCPhoIn=MCPhoIn[i]; }
    }

   }

       if(BestMCMuJpsi1In!=-99){
         const Candidate & muGen = (*genParticles)[BestMCMuJpsi1In];
          int nMoth = muGen.numberOfMothers();
          for(size_t jj = 0; jj < nMoth; ++ jj) {
          const Candidate * mom = muGen.mother( jj );
          TrueBestMuJpsi1MothId[jj] = mom->pdgId();
          }
          TrueBestMuJpsi1MothVtx[0]=muGen.mother(0)->vx();
          TrueBestMuJpsi1MothVtx[1]=muGen.mother(0)->vy();
          TrueBestMuJpsi1MothVtx[2]=muGen.mother(0)->vz(); 
          if(muGen.mother(0)->mother(0)!=0)
          TrueBestMuJpsi1GMothId=muGen.mother(0)->mother(0)->pdgId();
          if(muGen.mother(0)->mother(0)->mother(0)->mother(0)!=0)
          TrueBestMuJpsi1GGMothId=muGen.mother(0)->mother(0)->mother(0)->pdgId();
        }

       if(BestMCMuJpsi2In!=-99){
         const Candidate & muGen = (*genParticles)[BestMCMuJpsi2In];
          int nMoth = muGen.numberOfMothers();
          for(size_t jj = 0; jj < nMoth; ++ jj) {
          const Candidate * mom = muGen.mother( jj );
          TrueBestMuJpsi2MothId[jj] = mom->pdgId();
          }
          TrueBestMuJpsi2MothVtx[0]=muGen.mother(0)->vx();
          TrueBestMuJpsi2MothVtx[1]=muGen.mother(0)->vy();
          TrueBestMuJpsi2MothVtx[2]=muGen.mother(0)->vz();
          if(muGen.mother(0)->mother(0)!=0)
          TrueBestMuJpsi2GMothId=muGen.mother(0)->mother(0)->pdgId();
          if(muGen.mother(0)->mother(0)->mother(0)->mother(0)!=0)
          TrueBestMuJpsi2GGMothId=muGen.mother(0)->mother(0)->mother(0)->pdgId();
        }

       if(BestMCMuIn!=-99){
         const Candidate & muGen = (*genParticles)[BestMCMuIn];
          int nMoth = muGen.numberOfMothers();
          for(size_t jj = 0; jj < nMoth; ++ jj) {
          const Candidate * mom = muGen.mother( jj );
          TrueBestMuMothId[jj] = mom->pdgId();
          }
          TrueBestMuMothVtx[0]=muGen.mother(0)->vx();
          TrueBestMuMothVtx[1]=muGen.mother(0)->vy();
          TrueBestMuMothVtx[2]=muGen.mother(0)->vz();
          TrueMuSlPt=muGen.pt();
          TrueMuSlEta=muGen.eta(); 
          TrueMuSlPhi=muGen.phi(); 
          if(muGen.mother(0)->mother(0)!=0)
          TrueBestMuGMothId=muGen.mother(0)->mother(0)->pdgId();
          if(muGen.mother(0)->mother(0)->mother(0)->mother(0)!=0)
          TrueBestMuGGMothId=muGen.mother(0)->mother(0)->mother(0)->pdgId();
        }
       if(BestMCElecIn!=-99){
         const Candidate & elecGen = (*genParticles)[BestMCElecIn];
          int nMoth = elecGen.numberOfMothers();
          for(size_t jj = 0; jj < nMoth; ++ jj) {
          const Candidate * mom = elecGen.mother( jj );
          TrueBestElecMothId[jj] = mom->pdgId();
          }
         TrueElecSlPt=elecGen.pt();
         TrueElecSlEta=elecGen.eta();
         TrueElecSlPhi=elecGen.phi();
          if(elecGen.mother(0)->mother(0)!=0)
          TrueBestElecGMothId=elecGen.mother(0)->mother(0)->pdgId();
          if(elecGen.mother(0)->mother(0)->mother(0)->mother(0)!=0)
          TrueBestElecGGMothId=elecGen.mother(0)->mother(0)->mother(0)->pdgId();
        }

        minDRBestMuJpsi1=MinDRMuJpsi1;
        minDRBestMuJpsi2=MinDRMuJpsi2;
        minDRBestMuSl=MinDRMu;
        indexBestMuJpsi1=BestMCMuJpsi1In;
        indexBestMuJpsi2=BestMCMuJpsi2In;
        indexBestMuSl=BestMCMuIn;
        minDRBestElecSl=MinDRElec;

///

       elecMult = gsfElectrons->size();
       muMult = muons->size();

const VertexCompositeCandidate & Jpsi = (*fittedJpsi)[VtxJpsiIn];

       jpsimass=JpsiMass;
       jpsimu1pt=JpsiMu1Pt;
       jpsimu2pt=JpsiMu2Pt;
       jpsimu1cat=JpsiMu1Cat;
       jpsimu2cat=JpsiMu2Cat;
       jpsimu1phi=JpsiMu1Phi;
       jpsimu2phi=JpsiMu2Phi;
       jpsimu1eta=JpsiMu1Eta;
       jpsimu2eta=JpsiMu2Eta;
       jpsimu1z=JpsiMu1z;
       jpsimu2z=JpsiMu2z;
       jpsiphi=JpsiPhi;
       jpsieta=JpsiEta;
       jpsipt=JpsiPt;
       jpsilxy=JpsiLxy;
       jpsilxy2=JpsiLxy2;
       jpsilxyorig=JpsiLxyOrig;
       jpsilxy2orig=JpsiLxy2Orig;
       jpsilxy3=lxy;
       jpsilxyerr3=lxyerr;
//       if(VtxJpsiIn!=-99) jpsivtxprob =TMath::Prob(Jpsi.vertexChi2(),Jpsi.vertexNdof());  
       if(GMuSlIn!=-99){
       const Muon & muSL = (*muons)[GMuSlIn];
       muphi=muSL.phi();
       mueta=muSL.eta();
       mupt=muSL.pt();
       mucharge=muSL.charge(); 
       for(int i=0;i<11;i++) muqual[i]=0;
       if(muSL.isGood(Muon::TrackerMuonArbitrated)) muqual[0]=1;
       if(muSL.isGood(Muon::AllArbitrated)) muqual[1]=1;
       if(muSL.isGood(Muon::GlobalMuonPromptTight)) muqual[2]=1;
       if(muon::isGoodMuon(muSL,Muon::TMLastStationLoose)) muqual[3]=1;
       if(muon::isGoodMuon(muSL,Muon::TMLastStationTight)) muqual[4]=1;
       if(muon::isGoodMuon(muSL,Muon::TM2DCompatibilityLoose)) muqual[5]=1;
       if(muon::isGoodMuon(muSL,Muon::TM2DCompatibilityTight)) muqual[6]=1;
       if(muSL.isGood(Muon::TMOneStationLoose)) muqual[7]=1;
       if(muSL.isGood(Muon::TMOneStationTight)) muqual[8]=1;
       if(muSL.isGood(Muon::TMLastStationOptimizedLowPtLoose)) muqual[9]=1;
       if(muSL.isGood(Muon::TMLastStationOptimizedLowPtTight)) muqual[10]=1;
       musegcomp=muSL.segmentCompatibility();
       mucalcomp=muSL.caloCompatibility();

     if(muSL.isTrackerMuon() && !muSL.isGlobalMuon()) mucat=1;
     else if(!muSL.isTrackerMuon() && muSL.isGlobalMuon()) mucat=2;
     else if(muSL.isTrackerMuon() && muSL.isGlobalMuon()) mucat=3;
     else if(!muSL.isTrackerMuon() && !muSL.isGlobalMuon()) mucat=4;
        if(muSL.isGlobalMuon()){
         muonglob=1;
         const Track* MuTrk = muSL.globalTrack().get();
         muchi2glo=MuTrk->normalizedChi2();
         munhitsglo=MuTrk->numberOfValidHits();
         }else muonglob=0;
       const Track* MuTrk = muSL.innerTrack().get();
       muchi2=MuTrk->normalizedChi2();
       munhits=MuTrk->numberOfValidHits();
       mud0=MuTrk->d0();
       muz=MuTrk->dz(vertexBeamSpot.position());
       muz2=MuTrk->dz(RecVtx.position());
       muIP=MuTrk->dxy(vertexBeamSpot.position());
       muIP2=MuTrk->dxy(RecVtx.position());
       muIPErr=MuTrk->d0Error();
   //    muIP2=MuTrk->parameter(3);
          if(GMuIn1!=-99 && GMuIn2!=-99){
           const Muon & muJpsi1 = (*muons)[GMuIn1];
           const Muon & muJpsi2 = (*muons)[GMuIn2];
           CompositeCandidate mu13;
           mu13.addDaughter( muJpsi1 );
           mu13.addDaughter( muSL );     
           AddFourMomenta addP41;
           addP41.set( mu13 );
           InvMassMu13=mu13.mass();
           
           CompositeCandidate mu23;
           mu23.addDaughter( muJpsi2 );
           mu23.addDaughter( muSL );
           AddFourMomenta addP42;
           addP42.set( mu23 );
           InvMassMu23=mu23.mass();
           
           CompositeCandidate mu123;
           mu123.addDaughter( muJpsi1 );
           mu123.addDaughter( muJpsi2 );
           mu123.addDaughter( muSL );
           AddFourMomenta addP43;
           addP43.set( mu123 );
           InvMassMu123=mu123.mass();

             double SmallMuTrk=10000.;
             for(size_t k = 0; k < muons->size(); ++ k) {
             if(k==GMuIn1 || k==GMuIn2 || k==GMuSlIn) continue;
             const Muon & trk = (*muons)[k];
             CompositeCandidate musltrk;
             musltrk.addDaughter( muSL );
             musltrk.addDaughter( trk );
             AddFourMomenta addP4;
             addP4.set( musltrk );
             if(abs(musltrk.mass()-3.097)<SmallMuTrk) {InvMassMuTrk=musltrk.mass(); SmallMuTrk=abs(musltrk.mass()-3.097);}
             }

           }
           else {InvMassMu13=-999., InvMassMu23=-999., InvMassMu123=-999.,InvMassMuTrk=-999.;}
      }else{muphi=-999.,mueta=-999.,mupt=-999.,mucat=-999,muz=-999.,muIP=-999.,muIP2=-999.,muIPErr=-999.,muchi2glo=-999,muchi2=-999,musegcomp=-999.,mucalcomp=-999.,
      muz2=-999.,munhits=-999,munhitsglo=-999,mud0=-999.;mucharge=-999;}

       double MinEEMass1=999.,MinEEMass2=999.;
        if(GElecSlIn!=-99){
       const Candidate &elecSL= (*gsfElectrons)[GElecSlIn];

       edm::Ref<reco::GsfElectronCollection> electronRef(gsfElectrons,GElecSlIn);
       cout << "electron " << GElecSlIn << " , Robust Loose = " << eIDmapRL[electronRef] 
	      << " , Robust Tight = " << eIDmapRT[electronRef] 
	      << " , Loose = " << eIDmapL[electronRef] 
	      << " , Tight = " << eIDmapT[electronRef] 
	      << std::endl;

       elecphi=elecSL.phi();
       elecpt=elecSL.pt();
       eleceta = elecSL.p4().Eta();
       const GsfTrack* elecTrk = elecSL.get<GsfTrackRef>().get();       
       elecIP=elecTrk->dxy(vertexBeamSpot.position());
       elecIP2=elecTrk->dxy(RecVtx.position());

// inv mass with any track
         Handle<CandidateView> Tracks1;
         iEvent.getByLabel("ConvTracks1",Tracks1);  
         for(size_t j=0; j < Tracks1->size(); ++j){
           OverlapChecker overlap;
           const Candidate & trk= (*Tracks1)[j];
           if(trk.charge()==elecSL.charge()) continue;
           if(overlap(elecSL,trk)!=0) continue; 
           CompositeCandidate dielec;
           dielec.addDaughter( elecSL );dielec.addDaughter( trk );
           AddFourMomenta addP4;addP4.set( dielec );
           if(dielec.mass()<MinEEMass1){
            elecInvMas1=dielec.mass();MinEEMass1=dielec.mass();}
          } 
         Handle<CandidateView> Tracks2;
         iEvent.getByLabel("ConvTracks2",Tracks2);
         for(size_t j=0; j < Tracks2->size(); ++j){
           OverlapChecker overlap;
           const Candidate & trk= (*Tracks2)[j];
           if(trk.charge()==elecSL.charge()) continue;
           if(overlap(elecSL,trk)!=0) continue;
           CompositeCandidate dielec;
           dielec.addDaughter( elecSL );dielec.addDaughter( trk );
           AddFourMomenta addP4;addP4.set( dielec );
           if(dielec.mass()<MinEEMass2){
            elecInvMas2=dielec.mass();MinEEMass2=dielec.mass();}
          }


         Handle<ConversionCollection> Conv;
         iEvent.getByLabel("conversions",Conv);
         convMult=Conv->size();
          for(size_t j=0; j< Conv->size(); ++j){
          const Conversion & cand = (*Conv)[j];
          convTracks[j] = cand.nTracks();
            for (unsigned int i=0; i<cand.tracks().size(); i++) {
             if(elecSL.charge()!=cand.tracks()[i]->charge())  continue;
             deltaRconv[j]=TMath::Sqrt((elecSL.eta()-cand.tracks()[i]->eta())*(elecSL.eta()-cand.tracks()[i]->eta())
             +(elecSL.phi()-cand.tracks()[i]->phi())*(elecSL.phi()-cand.tracks()[i]->phi())); 
           //    CompositeCandidate dielec;
          //   const Candidate &trk = elecSL; 
          //   trk.setP4(cand.tracks()[i]->p4());
          //   dielec.addDaughter( elecSL );dielec.addDaughter(trk );
          //   AddFourMomenta addP4;addP4.set( dielec );
          //   float elecInvMas2=dielec.mass();
            }
          }

// isolation
//          Handle<IsolationCollection> elecIso;
//          iEvent.getByLabel("allElectronIsolations",elecIso);
//          CandidateRef elec1 = elecSL.masterClone().castTo<CandidateRef>();
//          const  double iso1 = elecIso->value( elec1.key() ); 
//

      const GsfElectron & elecCand = (*gsfElectrons)[GElecSlIn];
       eleceOverP = elecCand.eSuperClusterOverP();
       elecseed=elecCand.superCluster()->seed()->energy();
       double pin  = elecCand.trackMomentumAtVtx().R(); 
       elecseedoverpin=elecseed/pin;
       double pout = elecCand.trackMomentumOut().R(); 
       elecfBrem = (pin-pout)/pin;
       elechoverE=elecCand.hadronicOverEm();
 
 //// const reco::ClusterShapeRef& shapeRef = ElectronIDAlgo::getClusterShape(elecCand, iEvent);
//
/*  edm::Handle<reco::BasicClusterShapeAssociationCollection> clusterShapeHandleBarrel;
   edm::Handle<reco::BasicClusterShapeAssociationCollection> clusterShapeHandleEndcap;
   iEvent.getByLabel("barrelClusterShapeAssociation", clusterShapeHandleBarrel);
   iEvent.getByLabel("endcapClusterShapeAssociation", clusterShapeHandleEndcap);
//   iEvent.getByLabel("multi5x5BarrelShapeAssoc", clusterShapeHandleBarrel);
//   iEvent.getByLabel("multi5x5EndcapShapeAssoc", clusterShapeHandleEndcap);
 

  // Find entry in map corresponding to seed BasicCluster of SuperCluster
   reco::BasicClusterShapeAssociationCollection::const_iterator seedShpItr;
 
   if (elecCand.classification()<100) {
     seedShpItr=clusterShapeHandleBarrel->find(elecCand.superCluster()->seed());
    if (elecCand.classification()==40 && seedShpItr == clusterShapeHandleBarrel->end()) 
         seedShpItr=clusterShapeHandleEndcap->find(elecCand.superCluster()->seed());
   } else {
     seedShpItr=clusterShapeHandleEndcap->find(elecCand.superCluster()->seed());
   }
 
   const reco::ClusterShapeRef& shapeRef= seedShpItr->val; 
   sigmaee = sqrt(shapeRef->covEtaEta()); 
*/
  const reco::BasicCluster& seedClus = *(elecCand.superCluster()->seed());
  const DetId firstDetId = seedClus.getHitsByDetId()[0];// this is NOT the seed but all hits will be either endcap or barrel
  
  //now get the CaloTopology and rec hits
  //note in practice you wouldnt hardcode the hit InputTags
  edm::ESHandle<CaloTopology> caloTopologyHandle;
  iSetup.get<CaloTopologyRecord>().get(caloTopologyHandle);
  edm::ESHandle<CaloGeometry> caloGeometryHandle; 
  iSetup.get<CaloGeometryRecord>().get(caloGeometryHandle);

  edm::Handle<EcalRecHitCollection> ebReducedRecHitsHandle;
  iEvent.getByLabel("reducedEcalRecHitsEB",ebReducedRecHitsHandle);
  edm::Handle<EcalRecHitCollection> eeReducedRecHitsHandle;
  iEvent.getByLabel("reducedEcalRecHitsEE",eeReducedRecHitsHandle);

  const CaloTopology* caloTopology = caloTopologyHandle.product();
  const CaloGeometry *caloGeometry = caloGeometryHandle.product(); 
  const EcalRecHitCollection* ebRecHits = ebReducedRecHitsHandle.product();
  const EcalRecHitCollection* eeRecHits = eeReducedRecHitsHandle.product();
  
  if(firstDetId.subdetId()==EcalBarrel){
    std::vector<float> localCov = EcalClusterTools::covariances(seedClus,ebRecHits,caloTopology,caloGeometry);
    sigmaee =  sqrt(localCov[0]); 
    e3355= EcalClusterTools::e3x3(seedClus,ebRecHits,caloTopology)/EcalClusterTools::e5x5(seedClus,ebRecHits,caloTopology);
  }else if(firstDetId.subdetId()==EcalEndcap){ 
    //identical to sigmaiEtaiEta which would be EcalClusterTools::localCovariances(seedClus,ebRecHits,caloTopology)
    std::vector<float> localCov = EcalClusterTools::covariances(seedClus,eeRecHits,caloTopology,caloGeometry);
    sigmaee =  sqrt(localCov[0]);
    e3355= EcalClusterTools::e3x3(seedClus,eeRecHits,caloTopology)/EcalClusterTools::e5x5(seedClus,eeRecHits,caloTopology);
  }

// 

  deltaPhiIn = elecCand.deltaPhiSuperClusterTrackAtVtx();
  deltaEtaIn = elecCand.deltaEtaSuperClusterTrackAtVtx();
  
     int emapVal(0);
       if(eIDmapRL[electronRef]!=0) emapVal=1;
       if(eIDmapRT[electronRef]!=0) emapVal=2;
       if(eIDmapL[electronRef]!=0) emapVal=3;
       if(eIDmapT[electronRef]!=0) emapVal=4;  
       elecid=emapVal;
       } else {elecphi=-999.; elecpt=-999.; elecpt=-999.; elecIP=-999.; elecIP2=-999.;} 

       NJpsiEv=Njpsi; 

//edm::Ref<reco::PixelMatchGsfElectronCollection> electronRef(gsfElectrons,GElecSlIn); 
//edm::Handle<reco::ElectronIDCollection> electronIDAssocHandle;
//  iEvent.getByLabel("eidCutBased", electronIDAssocHandle);
//reco::ElectronIDAssociationCollection::const_iterator electronIDAssocItr;
//    electronIDAssocItr = electronIDAssocHandle->find(electronRef);
//    const reco::ElectronIDRef& id = electronIDAssocItr->val;
//    bool cutBasedID = id->cutBasedDecision();   

/*  std::vector<edm::Handle<edm::ValueMap<float> > > eIDValueMapN(9); 
  iEvent.getByLabel( "eidCutBased" , eIDValueMapN[0] ); 
  const edm::ValueMap<float> & eIDmap1 = * eIDValueMapN[0] ;
  iEvent.getByLabel( "eidCutBasedClasses" , eIDValueMapN[1] );
  const edm::ValueMap<float> & eIDmap2 = * eIDValueMapN[1] ;
  iEvent.getByLabel( "eidCutBasedClassesTight" , eIDValueMapN[2] );
  const edm::ValueMap<float> & eIDmap3 = * eIDValueMapN[2] ;
  iEvent.getByLabel( "eidLikelihood" , eIDValueMapN[3] );
  const edm::ValueMap<float> & eIDmap4 = * eIDValueMapN[3] ;
  iEvent.getByLabel( "eidNeuralNet" , eIDValueMapN[4] );
  const edm::ValueMap<float> & eIDmap5 = * eIDValueMapN[4] ;
  iEvent.getByLabel( "eidCutBasedExt" , eIDValueMapN[5] );
  const edm::ValueMap<float> & eIDmap6 = * eIDValueMapN[5] ;
  iEvent.getByLabel( "eidCutBasedClassesExt" , eIDValueMapN[6] );
  const edm::ValueMap<float> & eIDmap7 = * eIDValueMapN[6] ;
  iEvent.getByLabel( "eidLikelihoodExt" , eIDValueMapN[7] );
  const edm::ValueMap<float> & eIDmap8 = * eIDValueMapN[7] ;
  iEvent.getByLabel( "eidNeuralNetExt" , eIDValueMapN[8] );
  const edm::ValueMap<float> & eIDmap9 = * eIDValueMapN[8] ;
*/
/////////////////////trigger match//////////////////
/*

edm::InputTag tag2("hltTriggerSummaryAOD::HLT");
edm::Handle<trigger::TriggerEvent> trgEvent;
iEvent.getByLabel(tag2,trgEvent);

const trigger::size_type numFilterObjects(trgEvent->sizeFilters());
// cout
//    << "Used Processname: " << trgEvent->usedProcessName() << "\n"
//    << "Number of TriggerFilterObjects: " << numFilterObjects << "\n"
//    << "The TriggerFilterObjects: #, tag";
//  for ( trigger::size_type i = 0; i != numFilterObjects; ++i ) cout 
//    << i << " " << trgEvent->filterTag(i).encode() << endl;

  std::string              theL1CollectionLabel;
  std::vector<std::string> theHltCollectionLabels;

  theHltCollectionLabels.clear();
  for ( trigger::size_type i = 0; i != numFilterObjects; ++i  ) {
    string module = trgEvent->filterTag(i).encode();
    if ( TString(module).Contains("L1Filtered") ) 
      theL1CollectionLabel = module; 
    else if ( TString(module).Contains("Filtered") ) 
      theHltCollectionLabels.push_back(module); 
  }

InputTag tagL1, tagHLT; // Used as a shortcut for the current CollectionLabel

  // Get the L1 candidates //
  vector<trigger::TriggerObject> L1cands;
   L1cands.clear();
  tagL1 = InputTag(theL1CollectionLabel);
  if ( trgEvent->filterIndex(tagL1) >= trgEvent->sizeFilters() ) {
   cout << "No L1 Collection with label " << tagL1 << endl;
    return;
  } else {
   const trigger::TriggerObjectCollection& TOC(trgEvent->getObjects()); 
   for(int j=0; j < TOC.size(); j++)
   L1cands.push_back(TOC[j]);
//   cout << "L1 id: " << L1cand.id() << endl;
//  size_t filterIndex = trgEvent->filterIndex(tag);
//    trgEvent->getObjects();
  }
 // theNumberOfL1Events++;

  // Get the HLT candidates //
  unsigned int numHltLabels = theHltCollectionLabels.size();
  vector< vector<trigger::TriggerObject> > hltCands(numHltLabels);
  for ( unsigned int i = 0; i < numHltLabels; i++ ) {
    hltCands[i].clear();
    tagHLT = InputTag(theHltCollectionLabels[i]);
    if ( trgEvent->filterIndex(tagHLT) >= trgEvent->sizeFilters() )
      cout <<"No HLT Collection with label "<< tagHLT << endl;
    else {
     const trigger::Keys& KEYS(trgEvent->filterKeys(trgEvent->filterIndex(tagHLT)));
     const trigger::TriggerObjectCollection& TOC(trgEvent->getObjects());
      for(int j=0; j < KEYS.size(); j++)
       {trigger::size_type hltf=KEYS[j];
       hltCands[i].push_back(TOC[hltf]);}
  //     cout << "HLT n: " << j << "id: " << TOC[hltf].id() << "and pt: " << TOC[hltf].pt() << endl;}
     }
  }



  matchL1=0,matchL2=0,matchL3=0,matchHLT1=0,matchHLT2=0,matchHLT3=0,matchTrkL=0,matchTrkHLT=0,matchMu2nd=0;

  if(GMuIn1!=-99){
    const Candidate & muJpsi1 = (*muons)[GMuIn1];
 
 for ( size_t candL = 0; candL < L1cands.size(); candL++ ) {
    if(matchL1>0) continue;
    double eta= L1cands[candL].eta();
    double phi= L1cands[candL].phi();

    double deltaR1= TMath::Sqrt((muJpsi1.eta()-eta)*(muJpsi1.eta()-eta)+(muJpsi1.phi()-phi)*(muJpsi1.phi()-phi));
    if(deltaR1<0.05) matchL1++;
 }

  for ( size_t  i = 0; i < numHltLabels; i++ ) { 
    for ( size_t candNum = 0; candNum < hltCands[i].size(); candNum++ ) {
    if(matchHLT1>0) continue;
    double eta= hltCands[i][candNum].eta();
    double phi= hltCands[i][candNum].phi();   

    double deltaR1= TMath::Sqrt((muJpsi1.eta()-eta)*(muJpsi1.eta()-eta)+(muJpsi1.phi()-phi)*(muJpsi1.phi()-phi));
    if(deltaR1<0.05) matchHLT1++;
    }
  } 

  }



if(GMuIn2!=-99){
    const Candidate & muJpsi2 = (*muons)[GMuIn2];

 for ( size_t candL = 0; candL < L1cands.size(); candL++ ) {
    if(matchL2>0) continue;
    double eta= L1cands[candL].eta();
    double phi= L1cands[candL].phi();

    double deltaR2= TMath::Sqrt((muJpsi2.eta()-eta)*(muJpsi2.eta()-eta)+(muJpsi2.phi()-phi)*(muJpsi2.phi()-phi));
    if(deltaR2<0.05) matchL2++;
 }

  for ( size_t  i = 0; i < numHltLabels; i++ ) {
    for ( size_t candNum = 0; candNum < hltCands[i].size(); candNum++ ) {
    if(matchHLT2>0) continue;
    double eta= hltCands[i][candNum].eta();
    double phi= hltCands[i][candNum].phi();

    double deltaR2= TMath::Sqrt((muJpsi2.eta()-eta)*(muJpsi2.eta()-eta)+(muJpsi2.phi()-phi)*(muJpsi2.phi()-phi));
    if(deltaR2<0.05) matchHLT2++;
    }
  }
  }

if(GMuSlIn!=-99){
    const Candidate & muSl = (*muons)[GMuSlIn];

 for ( size_t candL = 0; candL < L1cands.size(); candL++ ) {
    if(matchL3>0) continue;
    double eta= L1cands[candL].eta();
    double phi= L1cands[candL].phi();

    double deltaR3= TMath::Sqrt((muSl.eta()-eta)*(muSl.eta()-eta)+(muSl.phi()-phi)*(muSl.phi()-phi));
    if(deltaR3<0.05) matchL3++;
 }

  for ( size_t  i = 0; i < numHltLabels; i++ ) {
    for ( size_t candNum = 0; candNum < hltCands[i].size(); candNum++ ) {
    if(matchHLT3>0) continue;
    double eta= hltCands[i][candNum].eta();
    double phi= hltCands[i][candNum].phi();

    double deltaR3= TMath::Sqrt((muSl.eta()-eta)*(muSl.eta()-eta)+(muSl.phi()-phi)*(muSl.phi()-phi));
    if(deltaR3<0.05) matchHLT3++;
    }
  }
  }


  if(GMuIn1!=-99){
    const Candidate & muJpsi1 = (*muons)[GMuIn1];

    const Candidate & muJpsi2 = (*muons)[GMuIn2];

   double MinVtx=-999;
   Handle<CandidateView> AllTracks;
   iEvent.getByLabel("allTracks",AllTracks);
   for(size_t j=0;j < AllTracks->size(); ++j){
   OverlapChecker overlap;
   const Candidate & trk =(*AllTracks)[j];
   if(overlap(muJpsi1,trk)!=0) continue;
   if(trk.charge()==muJpsi1.charge()) continue;
   if(trk.pt()<2.5) continue;

   CompositeCandidate Jpsicand;
   Jpsicand.addDaughter(muJpsi1);Jpsicand.addDaughter(trk);
   AddFourMomenta addP4; addP4.set(Jpsicand);

   edm::ESHandle<TransientTrackBuilder> theB;
   iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theB);
   TrackRef trkMu1Ref = muJpsi1.get<TrackRef>();
   TrackRef trkTest = trk.get<TrackRef>();
   vector<TransientTrack> t_tks;
   t_tks.push_back((*theB).build(&trkMu1Ref)); 
   t_tks.push_back((*theB).build(&trkTest)); 
   KalmanVertexFitter kvf;
   TransientVertex tv = kvf.vertex(t_tks);
   if(!tv.isValid()) continue;
   Vertex vertex =tv;
   GlobalPoint secondaryVertex = tv.position();
   double vtxProb = TMath::Prob(vertex.chi2(),vertex.ndof());
   if(vtxProb>MinVtx){
  
   matchMu2nd=0,matchTrkL=0,matchTrkHLT=0;

   MinVtx=vtxProb;
   JpsiTestMass=Jpsicand.mass();
   JpsiTestvtxprob=vtxProb;
   JpsiTestPt=Jpsicand.pt();
   JpsiTestEta=Jpsicand.eta();
   TrackTestPt=trk.pt();
   TrackTestEta=trk.eta(); 

   if(overlap(muJpsi2,trk)!=0) matchMu2nd++; 

 for ( size_t candL = 0; candL < L1cands.size(); candL++ ) {
    if(matchTrkL>0) continue;
    double eta= L1cands[candL].eta();
    double phi= L1cands[candL].phi();

    double deltaR= TMath::Sqrt((trk.eta()-eta)*(trk.eta()-eta)+(trk.phi()-phi)*(trk.phi()-phi));
    if(deltaR<0.05) matchTrkL++;
 }

  for ( size_t  i = 0; i < numHltLabels; i++ ) {
    for ( size_t candNum = 0; candNum < hltCands[i].size(); candNum++ ) {
    if(matchTrkHLT>0) continue;
    double eta= hltCands[i][candNum].eta();
    double phi= hltCands[i][candNum].phi();

    double deltaR= TMath::Sqrt((trk.eta()-eta)*(trk.eta()-eta)+(trk.phi()-phi)*(trk.phi()-phi));
    if(deltaR<0.05) matchTrkHLT++;
    }
  }


   }
  }
  }
*/
//////////////////////////////////////////////


       ntp1->Fill();

}


// ------------ method called once each job just before starting event loop  ------------
void 
JPsiMuAnalyzer2::beginJob(const edm::EventSetup&)
{

ntp1->Branch("JpsiMass",&jpsimass,"jpsimass/D");
ntp1->Branch("JpsiPhi",&jpsiphi,"jpsiphi/D");
ntp1->Branch("JpsiEta",&jpsieta,"jpsieta/D");
ntp1->Branch("JpsiPt",&jpsipt,"jpsipt/D");
ntp1->Branch("JpsiMu1Pt",&jpsimu1pt,"jpsimu1pt/D");
ntp1->Branch("JpsiMu2Pt",&jpsimu2pt,"jpsimu2pt/D");
ntp1->Branch("JpsiMu1Cat",&jpsimu1cat,"jpsimu1cat/I");
ntp1->Branch("JpsiMu2Cat",&jpsimu2cat,"jpsimu2cat/I");
ntp1->Branch("JpsiMu1phi",&jpsimu1phi,"jpsimu1phi/D");
ntp1->Branch("JpsiMu2phi",&jpsimu2phi,"jpsimu2phi/D");
ntp1->Branch("JpsiMu1eta",&jpsimu1eta,"jpsimu1eta/D");
ntp1->Branch("JpsiMu2eta",&jpsimu2eta,"jpsimu2eta/D");
ntp1->Branch("JpsiMu1z",&jpsimu1z,"jpsimu1z/D");
ntp1->Branch("JpsiMu2z",&jpsimu2z,"jpsimu2z/D");
ntp1->Branch("JpsiLxy",&jpsilxy,"jpsilxy/D");
ntp1->Branch("JpsiLxy2",&jpsilxy2,"jpsilxy2/D");
ntp1->Branch("JpsiLxyOrig",&jpsilxyorig,"jpsilxyorig/D");
ntp1->Branch("JpsiLxy2Orig",&jpsilxy2orig,"jpsilxy2orig/D");
ntp1->Branch("JpsiLxy3",&jpsilxy3,"jpsilxy3/D");
ntp1->Branch("JpsiLxyErr3",&jpsilxyerr3,"jpsilxyerr3/D");
ntp1->Branch("JpsiVtxProb",&jpsivtxprob,"jpsivtxprob/D");
ntp1->Branch("MuPhi",&muphi,"muphi/D");
ntp1->Branch("MuEta",&mueta,"mueta/D");
ntp1->Branch("MuPt",&mupt,"mupt/D");
ntp1->Branch("MuQual",&muqual,"muqual[11]/I");
ntp1->Branch("MuSegComp",&musegcomp,"musegcomp/D");
ntp1->Branch("MuCalComp",&mucalcomp,"mucalcomp/D");
ntp1->Branch("MuCat",&mucat,"mucat/I");
ntp1->Branch("MuGlob",&muonglob,"muonglob/I");
ntp1->Branch("MuChi2",&muchi2,"muchi2/D");
ntp1->Branch("MuChi2Glo",&muchi2glo,"muchi2glo/D");
ntp1->Branch("MunHits",&munhits,"munhits/I");
ntp1->Branch("MunHitsGlo",&munhitsglo,"munhitsglo/I");
ntp1->Branch("Mucharge",&mucharge,"mucharge/D");
ntp1->Branch("Mud0",&mud0,"mud0/D");
ntp1->Branch("Muz",&muz,"muz/D");
ntp1->Branch("Muz2",&muz2,"muz2/D");
ntp1->Branch("MuIP",&muIP,"muIP/D");
ntp1->Branch("MuIP2",&muIP2,"muIP2/D");
ntp1->Branch("MuIPErr",&muIPErr,"muIPErr/D");
ntp1->Branch("InvMassMu13",&InvMassMu13,"InvMassMu13/D");
ntp1->Branch("InvMassMu23",&InvMassMu23,"InvMassMu23/D");
ntp1->Branch("InvMassMu123",&InvMassMu123,"InvMassMu123/D");
ntp1->Branch("InvMassMuTrk",&InvMassMuTrk,"InvMassMuTrk/D");
ntp1->Branch("TruMuJpsi1Id",&TrueMuJpsi1Id,"TrueMuJpsi1Id/I");
ntp1->Branch("TruMuJpsi2Id",&TrueMuJpsi2Id,"TrueMuJpsi2Id/I");
ntp1->Branch("TruMuJpsi1MothId",&TrueMuJpsi1MothId,"TrueMuJpsi1MothId[10]/I");
ntp1->Branch("TruMuJpsi2MothId",&TrueMuJpsi2MothId,"TrueMuJpsi2MothId[10]/I");
ntp1->Branch("TruMuJpsi1GMothId",&TrueMuJpsi1GMothId,"TrueMuJpsi1GMothId/I");
ntp1->Branch("TruMuJpsi1GGMothId",&TrueMuJpsi1GGMothId,"TrueMuJpsi1GGMothId/I");
ntp1->Branch("TruMuJpsi2GMothId",&TrueMuJpsi2GMothId,"TrueMuJpsi2GMothId/I");
ntp1->Branch("TruMuJpsi2GGMothId",&TrueMuJpsi2GGMothId,"TrueMuJpsi2GGMothId/I");
ntp1->Branch("TruMuSlId",&TrueMuSlId,"TrueMuSlId/I");
ntp1->Branch("TruMuSlPt",&TrueMuSlPt,"TrueMuSlPt/D");
ntp1->Branch("TruMuSlEta",&TrueMuSlEta,"TrueMuSlEta/D");
ntp1->Branch("TruMuSlPhi",&TrueMuSlPhi,"TrueMuSlPhi/D");
ntp1->Branch("TruMuSlMothId",&TrueMuMothId,"TrueMuMothId[10]/I");
ntp1->Branch("TruJpsiId",&TrueJpsiId,"TrueJpsiId/I");
ntp1->Branch("TruJpsiPt",&TrueJpsiPt,"TrueJpsiPt/D");
ntp1->Branch("TruJpsiEta",&TrueJpsiEta,"TrueJpsiEta/D");
ntp1->Branch("TruJpsiPhi",&TrueJpsiPhi,"TrueJpsiPhi/D");
ntp1->Branch("TruJpsiMom",&TrueJpsiMom,"TrueJpsiMom/I");
ntp1->Branch("TruJpsiMomId",&TrueJpsiMomId,"TrueJpsiMomId/I");
ntp1->Branch("TruVtxJpsiId",&TrueVtxJpsiId,"TrueVtxJpsiId/I");
ntp1->Branch("TruJpsiGMomId",&TrueJpsiGMomId,"TrueJpsiGMomId/I");
ntp1->Branch("TruMuGMomId",&TrueMuGMothId,"TrueMuGMothId/I");
ntp1->Branch("TruMuGGMomId",&TrueMuGGMothId,"TrueMuGGMothId/I");
ntp1->Branch("MCbpt",&MCbpt,"MCbpt/D");
ntp1->Branch("MCbbarpt",&MCbbarpt,"MCbbarpt/D");
ntp1->Branch("MCbphi",&MCbphi,"MCbphi/D");
ntp1->Branch("MCbbarphi",&MCbbarphi,"MCbbarphi/D");
ntp1->Branch("MCbbarmass",&MCbbarmass,"MCbbarmass/D");
ntp1->Branch("MCbeta",&MCbeta,"MCbeta/D");
ntp1->Branch("MCbbeta",&MCbbeta,"MCbbeta/D");
ntp1->Branch("MCben",&MCben,"MCben/D");
ntp1->Branch("MCbben",&MCbben,"MCbben/D");
ntp1->Branch("MCNb",&MCnb,"MCnb/I");
ntp1->Branch("MCNbb",&MCnbb,"MCnbb/I");
ntp1->Branch("BBType",&BBType,"BBType/I");
ntp1->Branch("elecPt",&elecpt,"elecpt/D");
ntp1->Branch("elecPhi",&elecphi,"elecphi/D");
ntp1->Branch("elecEta",&eleceta,"eleceta/D");
ntp1->Branch("elecIP",&elecIP,"elecIP/D");
ntp1->Branch("elecIP2",&elecIP2,"elecIP2/D");
ntp1->Branch("elecMult",&elecMult,"elecMult/I");
ntp1->Branch("muMult",&muMult,"muMult/I");
ntp1->Branch("elecInvMas1",&elecInvMas1,"elecInvMas1/D");
ntp1->Branch("elecInvMas2",&elecInvMas2,"elecInvMas2/D");
ntp1->Branch("elecEOverP",&eleceOverP,"eleceOverP/D");
ntp1->Branch("elecfBrem",&elecfBrem,"elecfBrem/D");
ntp1->Branch("elecId",&elecid,"elecid/I");
ntp1->Branch("elecSeed",&elecseed,"elecseed/D");
ntp1->Branch("elecSeedOverPin",&elecseedoverpin,"elecseedoverpin/D");
ntp1->Branch("elecHOverE",&elechoverE,"elechoverE/D");
ntp1->Branch("elecDeltaPhiIn",&deltaPhiIn,"deltaPhiIn/D");
ntp1->Branch("elecDeltaEtaIn",&deltaEtaIn,"deltaEtaIn/D");
ntp1->Branch("elecsigmaee",&sigmaee,"sigmaee/D");
ntp1->Branch("elec3355",&e3355,"e3355/D");
ntp1->Branch("convMult",&convMult,"convMult/I");
ntp1->Branch("convTracks",&convTracks,"convTracks[150]/I");
ntp1->Branch("convDeltaR",&deltaRconv,"deltaRconv[150]/D");
ntp1->Branch("TruElecSlId",&TrueElecSlId,"TrueElecSlId/I");
ntp1->Branch("TruElecSlPt",&TrueElecSlPt,"TrueElecSlPt/D");
ntp1->Branch("TruElecSlEta",&TrueElecSlEta,"TrueElecSlEta/D");
ntp1->Branch("TruElecSlPhi",&TrueElecSlPhi,"TrueElecSlPhi/D");
ntp1->Branch("TruElecSlMothId",&TrueElecMothId,"TrueElecMothId[10]/I");
ntp1->Branch("TruElecGMomId",&TrueElecGMothId,"TrueElecGMothId/I");
ntp1->Branch("TruElecGGMomId",&TrueElecGGMothId,"TrueElecGGMothId/I");
ntp1->Branch("NJpsiEv",&NJpsiEv,"NJpsiEv/I");
ntp1->Branch("NEvtJpsi",&NevtJpsi,"NevtJpsi/I");
ntp1->Branch("NEvtMuMcSl",&NevtMuMcSl,"NevtMuMcSl/I");
ntp1->Branch("MCJpsiMothId",&MCJpsiMothId,"MCJpsiMothId/I");
ntp1->Branch("MCJpsiGMothId",&MCJpsiGMothId,"MCJpsiGMothId/I");
ntp1->Branch("MCJpsiGGMothId",&MCJpsiGGMothId,"MCJpsiGGMothId/I");
ntp1->Branch("NEvtMu",&NevtMu,"NevtMu/I");
ntp1->Branch("TruMCMuMothId",&TruMCMuMothId,"TruMCMuMothId[100]/I");
ntp1->Branch("TruMCMuGMothId",&TruMCMuGMothId,"TruMCMuGMothId[100]/I");
ntp1->Branch("TruMCMuGGMothId",&TruMCMuGGMothId,"TruMCMuGGMothId[100]/I");
ntp1->Branch("NEvtElec",&NevtElec,"NevtElec/I");
ntp1->Branch("TruMCElecMothId",&TruMCElecMothId,"TruMCElecMothId[100]/I");
ntp1->Branch("TruMCElecGMothId",&TruMCElecGMothId,"TruMCElecGMothId[100]/I");
ntp1->Branch("TruMCElecGGMothId",&TruMCElecGGMothId,"TruMCElecGGMothId[100]/I");
ntp1->Branch("elecSlFlag",&elecSlFlag,"elecSlFlag/I");
ntp1->Branch("elecConvFlag",&elecConvFlag,"elecConvFlag/I");
ntp1->Branch("elecFakeFlag",&elecFakeFlag,"elecFakeFlag/I");
ntp1->Branch("DeltaRMu",&DeltaRMu,"DeltaRMu[100]/D");
ntp1->Branch("DeltaRElec",&DeltaRElec,"DeltaRElec[100]/D");
ntp1->Branch("NEvtPho",&NevtPho,"NevtPho/I");
//ntp1->Branch("DeltaRPho",&DeltaRPho,"DeltaRPho[1000]/D");
ntp1->Branch("TruBestMuJpsi1MothId",&TrueBestMuJpsi1MothId,"TrueBestMuJpsi1MothId[10]/I");
ntp1->Branch("TruBestMuJpsi1MothVtx",&TrueBestMuJpsi1MothVtx,"TrueBestMuJpsi1MothVtx[3]/D");
ntp1->Branch("TruBestMuJpsi1GMomId",&TrueBestMuJpsi1GMothId,"TrueBestMuJpsi1GMothId/I");
ntp1->Branch("TruBestMuJpsi1GGMomId",&TrueBestMuJpsi1GGMothId,"TrueBestMuJpsi1GGMothId/I");
ntp1->Branch("TruBestMuJpsi2MothId",&TrueBestMuJpsi2MothId,"TrueBestMuJpsi2MothId[10]/I");
ntp1->Branch("TruBestMuJpsi2MothVtx",&TrueBestMuJpsi2MothVtx,"TrueBestMuJpsi2MothVtx[3]/D");
ntp1->Branch("TruBestMuJpsi2GMomId",&TrueBestMuJpsi2GMothId,"TrueBestMuJpsi2GMothId/I");
ntp1->Branch("TruBestMuJpsi2GGMomId",&TrueBestMuJpsi2GGMothId,"TrueBestMuJpsi2GGMothId/I");
ntp1->Branch("TruBestMuSlMothId",&TrueBestMuMothId,"TrueBestMuMothId[10]/I");
ntp1->Branch("TruBestMuSlMothVtx",&TrueBestMuMothVtx,"TrueBestMuMothVtx[3]/D");
ntp1->Branch("TruBestMuSlGMomId",&TrueBestMuGMothId,"TrueBestMuGMothId/I");
ntp1->Branch("TruBestMuSlGGMomId",&TrueBestMuGGMothId,"TrueBestMuGGMothId/I");
ntp1->Branch("TruBestElecSlMothId",&TrueBestElecMothId,"TrueBestElecMothId[10]/I");
ntp1->Branch("TruBestElecGMomId",&TrueBestElecGMothId,"TrueBestElecGMothId/I");
ntp1->Branch("TruBestElecGGMomId",&TrueBestElecGGMothId,"TrueBestElecGGMothId/I");
ntp1->Branch("minDRBestMuJpsi1",&minDRBestMuJpsi1,"minDRBestMuJpsi1/D");
ntp1->Branch("minDRBestMuJpsi2",&minDRBestMuJpsi2,"minDRBestMuJpsi2/D");
ntp1->Branch("minDRBestMuSl",&minDRBestMuSl,"minDRBestMuSl/D");
ntp1->Branch("indexBestMuJpsi1",&indexBestMuJpsi1,"indexBestMuJpsi1/I");
ntp1->Branch("indexBestMuJpsi2",&indexBestMuJpsi2,"indexBestMuJpsi2/I");
ntp1->Branch("indexBestMuSl",&indexBestMuSl,"indexBestMuSl/I");
ntp1->Branch("minDRBestElecSl",&minDRBestElecSl,"minDRBestElecSl/D");
ntp1->Branch("NevtK1",&NevtKaon1,"NevtKaon1/I");
ntp1->Branch("NevtK2",&NevtKaon2,"NevtKaon2/I");
ntp1->Branch("NevtK3",&NevtKaon3,"NevtKaon3/I");
ntp1->Branch("NevtK4",&NevtKaon4,"NevtKaon4/I");
ntp1->Branch("BMass",&Bmass,"Bmass/D");
ntp1->Branch("BMassC",&BmassC,"BmassC/D");
ntp1->Branch("BvtxProb",&Bvtxprob,"Bvtxprob/D");
ntp1->Branch("BPhi",&Bphi,"Bphi/D");
ntp1->Branch("BPt1",&BPt1,"BPt1/D");
ntp1->Branch("BPx1",&BPx1,"BPx1/D");
ntp1->Branch("BPy1",&BPy1,"BPy1/D");
ntp1->Branch("Bsumpt",&Bsumpt,"Bsumpt/D");
ntp1->Branch("B1Lxy",&B1Lxy,"B1Lxy/D");
ntp1->Branch("B1Lxy2",&B1Lxy2,"B1Lxy2/D");
ntp1->Branch("Berrx1",&Berrx1,"Berrx1/D");
ntp1->Branch("Berry1",&Berry1,"Berry1/D");
ntp1->Branch("Berrxy1",&Berrxy1,"Berrxy1/D");
ntp1->Branch("KPt",&kaonpt,"kaonpt/D");
ntp1->Branch("KEta",&kaoneta,"kaoneta/D");
ntp1->Branch("KPhi",&kaonphi,"kaonphi/D");
ntp1->Branch("Kchi2",&kaonchi2,"kaonchi2/D");
ntp1->Branch("KmuOv",&kaonmuov,"kaonmuov/I");
ntp1->Branch("KmuOv2",&kaonmuov2,"kaonmuov2/I");
ntp1->Branch("Knhits",&kaonnhits,"kaonnhits/I");
ntp1->Branch("Kpixhits",&kaonpixH,"kaonpixH/I");
ntp1->Branch("Ktrkhits",&kaontrkH,"kaontrkH/I");
ntp1->Branch("BMass2",&Bmass2,"Bmass2/D");
ntp1->Branch("BMass2C",&Bmass2C,"Bmass2C/D");
ntp1->Branch("KstMass",&KstMass,"KstMass/D");
ntp1->Branch("BvtxProb2",&Bvtxprob2,"Bvtxprob2/D");
ntp1->Branch("BPhi2",&Bphi2,"Bphi2/D");
ntp1->Branch("BPt2",&BPt2,"BPt2/D");
ntp1->Branch("BPx2",&BPx2,"BPx2/D");
ntp1->Branch("BPy2",&BPy2,"BPy2/D");
ntp1->Branch("Bsumpt2",&Bsumpt2,"Bsumpt2/D");
ntp1->Branch("B2Lxy",&B2Lxy,"B2Lxy/D");
ntp1->Branch("B2Lxy2",&B2Lxy2,"B2Lxy2/D");
ntp1->Branch("Berrx2",&Berrx2,"Berrx2/D");
ntp1->Branch("Berry2",&Berry2,"Berry2/D");
ntp1->Branch("Berrxy2",&Berrxy2,"Berrxy2/D");
ntp1->Branch("Kst1Pt",&kst1pt,"kst1pt/D");
ntp1->Branch("Kst1Eta",&kst1eta,"kst1eta/D");
ntp1->Branch("Kst1Phi",&kst1phi,"kst1phi/D");
ntp1->Branch("Kst1chi2",&kst1chi2,"kst1chi2/D");
ntp1->Branch("Kst1nhits",&kst1nhits,"kst1nhits/I");
ntp1->Branch("Kst2Pt",&kst2pt,"kst2pt/D");
ntp1->Branch("Kst2Eta",&kst2eta,"kst2eta/D");
ntp1->Branch("Kst2Phi",&kst2phi,"kst2phi/D");
ntp1->Branch("Kst2chi2",&kst2chi2,"kst2chi2/D");
ntp1->Branch("Kst2nhits",&kst2nhits,"kst2nhits/I");
ntp1->Branch("Kst1pixhits",&kst1pixH,"kst1pixH/I");
ntp1->Branch("Kst1trkhits",&kst1trkH,"kst1trkH/I");
ntp1->Branch("Kst2pixhits",&kst2pixH,"kst2pixH/I");
ntp1->Branch("Kst2trkhits",&kst2trkH,"kst2trkH/I");
ntp1->Branch("PVerrx",&PVerrx,"PVerrx/D");
ntp1->Branch("PVerry",&PVerry,"PVerry/D");
ntp1->Branch("JpsiTestMass",&JpsiTestMass,"JpsiTestMass/D");
ntp1->Branch("JpsiTestvtxprob",&JpsiTestvtxprob,"JpsiTestvtxprob/D");
ntp1->Branch("JpsiTestPt",&JpsiTestPt,"JpsiTestPt/D");
ntp1->Branch("JpsiTestEta",&JpsiTestEta,"JpsiTestEta/D");
ntp1->Branch("TrackTestPt",&TrackTestPt,"TrackTestPt/D");
ntp1->Branch("TrackTestEta",&TrackTestEta,"TrackTestEta/D");
ntp1->Branch("NTrkStudy",&NTrkStudy,"NTrkStudy[5]/I");
ntp1->Branch("Dmass1",&Dmass1,"Dmass1/D");
ntp1->Branch("Dmass2",&Dmass2,"Dmass2/D");
ntp1->Branch("Dsmass1",&Dsmass1,"Dsmass1/D");
ntp1->Branch("Dsmass2",&Dsmass2,"Dsmass2/D");
ntp1->Branch("DvtxProb",&DvtxProb,"DvtxProb/D");
ntp1->Branch("TrkD1pt",&trkD1pt,"trkD1pt/D");
ntp1->Branch("TrkD2pt",&trkD2pt,"trkD2pt/D");
ntp1->Branch("TrkD3pt",&trkD3pt,"trkD3pt/D");
ntp1->Branch("TrkD1muOv",&trk1muov,"trk1muov/I");
ntp1->Branch("TrkD2muOv",&trk2muov,"trk2muov/I");
ntp1->Branch("trk1chi2",&trk1chi2,"trk1chi2/D");
ntp1->Branch("trk2chi2",&trk2chi2,"trk2chi2/D");
ntp1->Branch("trk3chi2",&trk3chi2,"trk3chi2/D");
ntp1->Branch("trk1nhits",&trk1nhits,"trk1nhits/I");
ntp1->Branch("trk2nhits",&trk2nhits,"trk2nhits/I");
ntp1->Branch("trk3nhits",&trk3nhits,"trk3nhits/I");
ntp1->Branch("MCKid",&MCKid,"MCKid/I");
ntp1->Branch("MCKdr",&MCKdr,"MCKdr/D");
ntp1->Branch("MCKmomid",&MCKmomid,"MCKmomid/I");
ntp1->Branch("MCKGmomid",&MCKGmomid,"MCKGmomid/I");
ntp1->Branch("MCK21id",&MCK21id,"MCK21id/I");
ntp1->Branch("MCK21dr",&MCK21dr,"MCK21dr/D");
ntp1->Branch("MCK21momid",&MCK21momid,"MCK21momid/I");
ntp1->Branch("MCK21Gmomid",&MCK21Gmomid,"MCK21Gmomid/I");
ntp1->Branch("MCK22id",&MCK22id,"MCK22id/I");
ntp1->Branch("MCK22dr",&MCK22dr,"MCK22dr/D");
ntp1->Branch("MCK22momid",&MCK22momid,"MCK22momid/I");
ntp1->Branch("MCK22Gmomid",&MCK22Gmomid,"MCK22Gmomid/I");
ntp1->Branch("MCJpsiLxy",&MCJpsiLxy,"MCJpsiLxy/D");
ntp1->Branch("MCBpPt",&MCBpPt,"MCBpPt/D");
ntp1->Branch("MCBpLxy",&MCBpLxy,"MCBpLxy/D");
ntp1->Branch("MCBpdauLxy",&MCBpdauLxy,"MCBpdauLxy/D");
ntp1->Branch("MCB0Pt",&MCB0Pt,"MCB0Pt/D");
ntp1->Branch("MCBpP",&MCBpP,"MCBpP/D");
ntp1->Branch("MCB0P",&MCB0P,"MCB0P/D");
ntp1->Branch("MCBpEta",&MCBpEta,"MCBpEta/D");
ntp1->Branch("MCB0Eta",&MCB0Eta,"MCB0Eta/D");
ntp1->Branch("MCBpNdau",&MCBpNdau,"MCBpNdau/I");
ntp1->Branch("MCB0Ndau",&MCB0Ndau,"MCB0Ndau/I");
ntp1->Branch("MCBpdauId",&MCBpdauId,"MCBpdauId[30]/I");
ntp1->Branch("MCB0dauId",&MCB0dauId,"MCB0dauId[30]/I");
ntp1->Branch("MCB0dauLxy",&MCB0dauLxy,"MCB0dauLxy/D");
ntp1->Branch("MCB0Kstc",&MCB0Kstc,"MCB0Kstc/I");
ntp1->Branch("ncomb1",&ncomb1,"ncomb1/I");
ntp1->Branch("ncomb2",&ncomb2,"ncomb2/I");
ntp1->Branch("ncomb3",&ncomb3,"ncomb3/I");
ntp1->Branch("matchL1",&matchL1,"matchL1/I");
ntp1->Branch("matchL2",&matchL2,"matchL2/I");
ntp1->Branch("matchL3",&matchL3,"matchL3/I");
ntp1->Branch("matchHLT1",&matchHLT1,"matchHLT1/I");
ntp1->Branch("matchHLT2",&matchHLT2,"matchHLT2/I");
ntp1->Branch("matchHLT3",&matchHLT3,"matchHLT3/I");
ntp1->Branch("matchTrkMu2nd",&matchMu2nd,"matchMu2nd/I");
ntp1->Branch("matchTrkL",&matchTrkL,"matchTrkL/I");
ntp1->Branch("matchTrkHLT",&matchTrkHLT,"matchTrkHLT/I");
ntp1->Branch("TriggerFlag",&trigflag,"trigflag[160]/I");
}

// ------------ method called once each job just after ending the event loop  ------------
void 
JPsiMuAnalyzer2::endJob() {

}

//define this as a plug-in
DEFINE_FWK_MODULE(JPsiMuAnalyzer2);
