// -*- C++ -*-
//
// Package:    DiElecAnalyzer
// Class:      DiElecAnalyzer
// 
/**\class DiElecAnalyzer DiElecAnalyzer.cc UPC/ElecAnalyzer/src/ElecAnalyzer.cc
 Description: <one line class summary>
 Implementation:
<Notes on implementation>
*/
//
// Original Author:  Vineet Kumar
//         Created:  Thu May 29 15:02:18 CEST 2008
// $Id: DiElecAnalyzer.cc,v 1.1 2008/06/23 12:50:58 kumarv Exp $
//
//*******************************************************************************************
// include files
// system
#include <memory>
#include <iostream>
#include <string>
#include <map>
#include <set>
// ROOT
#include <TROOT.h>
#include <TSystem.h>
#include <TFile.h>
#include <TParameter.h>
#include <TH1.h>
#include <TH2.h>
#include <TTree.h>
#include <TVector3.h>
#include <TLorentzVector.h>
//*****************************************************************************************
// CMSSW
//#include "RecoEgamma/Examples/plugins/PixelMatchGsfElectronAnalyzer.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/EgammaReco/interface/ClusterShapeFwd.h"
#include "DataFormats/EgammaReco/interface/BasicClusterShapeAssociation.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlock.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "SimDataFormats/EncodedEventId/interface/EncodedEventId.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertexContainer.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertex.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"

#include "SimTracker/TrackAssociation/test/testTrackAssociator.h"
#include "SimTracker/Records/interface/TrackAssociatorRecord.h"
#include "SimTracker/TrackAssociation/interface/TrackAssociatorBase.h" 
#include "SimTracker/TrackAssociation/interface/TrackAssociatorByChi2.h"

//*****************************************************************************************
using namespace reco;
using namespace std;
using namespace edm;
using namespace HepMC;
// class declaration
class TFile;
class TH1D;
class DiElecAnalyzer : public edm::EDAnalyzer {
public:
  explicit DiElecAnalyzer(const edm::ParameterSet&);
  ~DiElecAnalyzer();
  std::string fSourceLabel;  
  std::string fOutputFileName ;
private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  virtual void RecAnalyze(const edm::Event &iEvent, const edm::EventSetup &iSetup);
  virtual void Compare(const edm::Event &iEvent, const edm::EventSetup &iSetup);
  virtual void FlowAnalyze(const edm::Event &iEvent, const edm::EventSetup &iSetup);
  virtual void SimAnalyze(const edm::Event &iEvent, const edm::EventSetup &iSetup);    

  // ----------member data ---------------------------
  TFile*      fOutputFile ;
  TH1D*       fHistGenElecPt;
  TH1D*       fHistGenElecEta ;
  TH1D*       fHistGenElecPhi ;
  TH1D*       fHistSimElecPt;
  TH1D*       fHistSimElecEta;
  TH1D*       fHistSimElecPhi;
  
  
  TH1D*       fHistFlowCandSize;
  TH1D*       fHistFlowElecPt;
  TH1D*       fHistFlowElecEta ;
  TH1D*       fHistFlowElecPhi ;
  




  TH1D*       fHistRecElecPt;
  TH1D*       fHistRecElecEta ;
  TH1D*       fHistRecElecPhi ;
  TH1D*       fHistRecElecDPt;
  TH2D*       fHistRecElecPtDiff;
  TH2D*       fHistRecElecEtaDiff;
  TH1D*       fHistRecElecPhiDiff;
 
  TH1D*       fHistGenDiElecY;
  TH1D*       fHistGenDiElecPt;
  TH1D*       fHistGenDiElecInvM;
  
  TH1D*       fHistFlowDiElecY;
  TH1D*       fHistFlowDiElecPt;
  TH1D*       fHistFlowDiElecInvM;
  TH1D*       fHistRecDiElecY;
  TH1D*       fHistRecDiElecPt;  
  TH1D*       fHistRecDiElecInvM;
  

  unsigned int  Nevt;
};
// constants, enums and typedefs
//
//
// static data member definitions
//

//
// constructors and destructor
//

//*******************************************************************************************

DiElecAnalyzer::DiElecAnalyzer(const edm::ParameterSet& iConfig):
  
  fOutputFileName(iConfig.getUntrackedParameter<string>("HistOutFile",std::string("ElecAnalysis.root")) ),
  fOutputFile(0),fHistGenElecPt(0),fHistGenElecEta(0),fHistGenElecPhi(0), fHistSimElecPt(0), fHistSimElecEta(0),fHistSimElecPhi(0),fHistFlowCandSize(0),fHistFlowElecPt(0),fHistFlowElecEta(0),fHistFlowElecPhi(0),fHistRecElecPt(0),fHistRecElecEta(0),fHistRecElecPhi(0),fHistRecElecDPt(0), fHistRecElecPtDiff(0), fHistRecElecEtaDiff(0),fHistRecElecPhiDiff(0),fHistGenDiElecY(0),fHistGenDiElecPt(0),fHistGenDiElecInvM(0),fHistFlowDiElecY(0),fHistFlowDiElecPt(0),fHistFlowDiElecInvM(0),fHistRecDiElecY(0),fHistRecDiElecPt(0),fHistRecDiElecInvM(0),Nevt(0)
{
   //now do what ever initialization is needed
}

DiElecAnalyzer::~DiElecAnalyzer()
{
 
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  
}

//*******************************************************************************************

//
// member functions
//

// ------------ method called to for each event  ------------
void
DiElecAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  using namespace reco;
  int eventnumber = iEvent.id().event();
  int runnumber   = iEvent.id().run(); 
  //cout << "--------------------------------------------------------------------------------"<<endl;
  // cout << "EVENT NUMBER = " << eventnumber <<"   "<<"RUN NUMBER = "<<runnumber<<endl;
  // cout <<"----------------Generation-Ana----------------------------" << endl;
  //cout << "Generation Event No.= " << Nevt +1 << endl;
  
  /*
  Handle<GenParticleCollection> genParticles;
  iEvent.getByLabel("genParticles", genParticles);
  int gcnt=genParticles->size();
  

    
  for ( int i=0 ; i < gcnt-1 ; ++i ){
    //  const l1extra::L1MuonParticle & l1mu1 = (*l1muons)[i];
    const reco::GenParticle & elec1 = (*genParticles)[i];
    for ( int j=i+1; j < gcnt; ++j ){
      const reco::GenParticle & elec2 = (*genParticles)[j];
      double elcharge1 = elec1.charge(), elpt1 = elec1.pt(), eleta1 = elec1.eta(), elphi1 = elec1.phi(), elE1 = elec1.energy(), elp1 = elec1.p(),pdgId1=elec1.pdgId();
    double elcharge2 = elec2.charge(), elpt2 = elec2.pt(), eleta2 = elec2.eta(), elphi2 = elec2.phi(), elE2 = elec2.energy(),elp2 = elec2.p(),pdgId2=elec2.pdgId();
  // cout<<"pdgId1 :"<<pdgId1<<" pdgId2 "<<pdgId2<<endl;
  if ( ( i != j) && (pdgId1*pdgId2==-121)){
    cout<<"genpdgId1 :"<<pdgId1<<" pdgId2 "<<pdgId2<<endl;

    TLorentzVector  vector1,vector2,vector3;
    vector1.SetPxPyPzE(elec1.px(), elec1.py(), elec1.pz(), elec1.energy());
    vector2.SetPxPyPzE(elec2.px(), elec2.py(), elec2.pz(), elec2.energy());
    vector3=vector1+vector2;
    float UpsInvMassGen=(vector3.M());
    float UpsYFlow=(vector3.Rapidity());
    cout<<" Gen Level Inv Mass :"<<UpsInvMassGen<<endl<<endl;



  }
}
  */
  
  float pt1[100], eta1[100], phi1[100], ppx1[100], ppy1[100], ppz1[100],ener1[100];
  float pt2[100], eta2[100], phi2[100],ppx2[100], ppy2[100], ppz2[100],ener2[100];
  
  int ipositron=0, ielectron=0;
  int nmp=0;
  int nmn=0; 

  edm::Handle<HepMCProduct> evt;
  iEvent.getByLabel("source", evt);
  const HepMC::GenEvent *genEvent = evt->GetEvent();
  int gcnt = genEvent->particles_size();
  //  cout << "Total no of Particles in the event = " <<gcnt << endl;
  gcnt = 0;

  // Loops over Starlight Event: Only dielectrons (from Ups-->e+e- or from continuum)  

  for (HepMC::GenEvent::particle_const_iterator p = genEvent->particles_begin(); p != genEvent->particles_end(); ++p) 
    {
      
      float ID  = (*p)->pdg_id();
      float eta = (*p)->momentum().eta();
      float phi = (*p)->momentum().phi();
      float pt = (*p)->momentum().perp();
      float px = (*p)->momentum().px();
      float py = (*p)->momentum().py();
      float pz = (*p)->momentum().pz(); 
      float ener=sqrt((px*px+py*py+pz*pz));
      
      //  if(eta < -2.4 || eta > 2.4) continue;
      // cout<<"  "<<ID<<"  "<<px<<"  "<<py<<"  "<<pz<<"  "<<ener<<"  "<<eta<<"  "<<phi<<endl; 
      // cout <<"genrated electron Pt "<< pt<< " eta: " << eta <<" ID: "<<ID<<endl; 
      
      if(abs(ID)==11){
	
	  fHistGenElecPt->Fill(pt);
	  fHistGenElecEta->Fill(eta);
	  fHistGenElecPhi->Fill(phi);
	
	if(ID==-11) {
	  pt1[ipositron] = pt;
	  eta1[ipositron] = eta;
	  phi1[ipositron] = phi;
	  ppx1[ipositron]=px;
	  ppy1[ipositron]=py;
	  ppz1[ipositron]=pz;
	  ener1[ipositron]=ener;
	  ipositron++;
	  nmp++;
	}  
	
	if(ID==11) {
	  pt2[ielectron] = pt;
	  eta2[ielectron] = eta;
	  phi2[ielectron] = phi;
	  ppx2[ielectron]=px;
	  ppy2[ielectron]=py;
	  ppz2[ielectron]=pz;
	  ener2[ielectron]=ener;
	  ielectron++;
	  nmn++;
	}
	
      }//if abs ID: electron or positron
      
      // Invariant mass analysis of e+/- pairs
      
      if(nmp>0 && nmn>0){
	
	for (  unsigned int i=0; i< ipositron; i++ ) {
	  for ( unsigned int j=0; j< ielectron; j++ ){
	    double delphai=phi1[i]-phi2[j];
	    double eptot=ener1[i]+ener2[j];
	    double ppxtot=ppx1[i]+ppx2[j];
	    double ppytot=ppy1[i]+ppy2[j];
	    double ppztot=ppz1[i]+ppz2[j];
	    double pptot=sqrt(ppxtot*ppxtot+ppytot*ppytot+ppztot*ppztot);
	    double genupt=sqrt((ppxtot*ppxtot)+(ppytot*ppytot));
	    double thetau=acos(ppztot/pptot); 
	    double genueta=-log(tan(thetau/2.0));
	    double genuy=0.5*(log((eptot+ppztot)/(eptot-ppztot)));	
	    double genup=sqrt(ppxtot*ppxtot+ppytot*ppytot+ppztot*ppztot);
	    double genupz=sqrt(ppztot*ppztot);
	    double minv = sqrt(2.0*pt1[i]*pt2[j]*(TMath::CosH(eta1[i]-eta2[j])-TMath::Cos(phi1[i]-phi2[j])));
	    double mpz=sqrt((eptot+pptot)*(eptot-pptot));
	    //cout<<" DiElectron Gen pt  "<<genupt<<" eta "<<genueta<<" invmass" <<minv<<endl;
	    fHistGenDiElecInvM->Fill(minv);
	    fHistGenDiElecY->Fill(genuy);
	    fHistGenDiElecPt->Fill(genupt);
	    fHistRecElecPhiDiff->Fill(delphai);
	  }
	}
      }
    } // end of loop over Event.

  //  SimAnalyze(iEvent, iSetup);
  FlowAnalyze(iEvent, iSetup);
  RecAnalyze(iEvent, iSetup); 
  Compare(iEvent, iSetup);
  Nevt++;
} 

//*******************************************************************************************

void DiElecAnalyzer::SimAnalyze(const edm::Event &iEvent, const edm::EventSetup &iSetup) 
{

  using namespace edm;
  
  edm::Handle<SimTrackContainer> simTracks;
  iEvent.getByLabel("g4SimHits",simTracks);
  SimTrackContainer::const_iterator simTrack;

  // cout <<"====================================Simulated tracks: "
  //   << "========================================"<<endl;

  for (simTrack = simTracks->begin(); simTrack != simTracks->end(); ++simTrack)
    {
      if (abs((*simTrack).type()) == 11) {
	
	float spx=(*simTrack).momentum().px();
	float spy=(*simTrack).momentum().py();
	float spz=(*simTrack).momentum().pz();
	float spt=TMath::Sqrt(spx*spx+spy*spy);
	float seta = (*simTrack).momentum().eta(); 
	// if(eta < -2.4 || eta > 2.4 || pt < 5.0) continue;
	
	fHistSimElecPt->Fill(spt);
	fHistSimElecEta->Fill(seta);
	
	//cout<<"Sim pT= "<< spt <<"  Sim Eta= "<< seta <<endl;
      }//abs Id    
    }// for (simTrac loop .....................
  
}//SimAna

//*******************************************************************************************

void DiElecAnalyzer::FlowAnalyze(const edm::Event &iEvent, const edm::EventSetup &iSetup)
{
  
  using reco::TrackCollection;
//--------------------------------------PFlow Electron reco Test-------------------------------------------------------
  const PFCandidateCollection *pflow_candidates;
  // Get Particle Flow Candidates
  Handle<PFCandidateCollection> pflow_hnd;
  iEvent.getByLabel("particleFlow", pflow_hnd);
  pflow_candidates = pflow_hnd.product();
  // Loop Over Particle Flow Candidates
  int flow_size=pflow_candidates->size();
  fHistFlowCandSize->Fill(flow_size);

  // cout<<" size : "<< flow_size<<endl;
  //--------------------------Lorentz Vector Inv Mass Test---------------------------------------------------------------
  for ( int i=0 ; i < flow_size-1 ; ++i ){
    //  const l1extra::L1MuonParticle & l1mu1 = (*l1muons)[i];
    const PFCandidate & elec1 = (*pflow_hnd)[i];
    for ( int j=i+1; j < flow_size; ++j ){
      const reco::PFCandidate & elec2 = (*pflow_hnd)[j];
      double elcharge1 = elec1.charge(), elpt1 = elec1.pt(), eleta1 = elec1.eta(), elphi1 = elec1.phi(), elE1 = elec1.energy(), elp1 = elec1.p(),pdgId1=elec1.pdgId();
      double elcharge2 = elec2.charge(), elpt2 = elec2.pt(), eleta2 = elec2.eta(), elphi2 = elec2.phi(), elE2 = elec2.energy(), elp2 = elec2.p(),pdgId2=elec2.pdgId();
      // cout<<"pdgId1 :"<<pdgId1<<" pdgId2 "<<pdgId2<<endl;
      if ( ( i != j) && (pdgId1*pdgId2==-121)){
	cout<<"pdgId1 :"<<pdgId1<<" pdgId2 "<<pdgId2<<endl;
	fHistFlowElecPt->Fill(elpt1); 
	fHistFlowElecPt->Fill(elpt2);
	fHistFlowElecEta->Fill(eleta1);
	fHistFlowElecEta->Fill(eleta2);
	fHistFlowElecPhi->Fill(elphi1);
	fHistFlowElecPhi->Fill(elphi2);

	TLorentzVector  vector1,vector2,vector3;
	vector1.SetPxPyPzE(elec1.px(), elec1.py(), elec1.pz(), elec1.energy());
	vector2.SetPxPyPzE(elec2.px(), elec2.py(), elec2.pz(), elec2.energy());
	vector3=vector1+vector2;
	float UpsInvMassFlow=(vector3.M());
	float UpsYFlow=(vector3.Rapidity());
	float UpsPtFlow=(vector3.Pt());
        

	fHistFlowDiElecY->Fill(UpsYFlow);
	fHistFlowDiElecPt->Fill(UpsPtFlow);
        fHistFlowDiElecInvM->Fill(UpsInvMassFlow);
	cout<<"flowInvmass1 "<<UpsInvMassFlow<<" Y "<< UpsYFlow<<endl;
      }

    }
  }

}

void DiElecAnalyzer::RecAnalyze(const edm::Event &iEvent, const edm::EventSetup &iSetup) 
{  
  using reco::TrackCollection;
  
  //  cout <<"--------------Reco-Ana------------------------------------" << endl;
  /*
  edm::Handle<reco::TrackCollection> ctftracks;
  // iEvent.getByLabel("ctfWithMaterialTracks", ctftracks );  
  iEvent.getByLabel("rsWithMaterialTracks", ctftracks );  

  const  reco::TrackCollection  recTC = *(ctftracks.product());
  int nctf = ctftracks->size();
  
  // cout << "No. of reconstructed ctftracks = " << nctf  <<endl;  
  */
  
  float pt1[100], eta1[100], phi1[100],ppx1[100], ppy1[100], ppz1[100],ener1[100];
  float pt2[100], eta2[100], phi2[100],ppx2[100], ppy2[100], ppz2[100],ener2[100];
  int ipositron=0, ielectron=0;
  int nmp=0;
  int nmn=0; 
     
  /*
  PFCandidateCollection::const_iterator pf;
  for (pf = pflow_candidates->begin(); pf != pflow_candidates->end(); pf++) {
    const PFCandidate *particle = &(*pf);
    float CandidateEt=particle->et();
    float CandidateEta=(particle->eta());
    float CandidatePx=(particle->px());
    float CandidatePy=(particle->py());
    float CandidatePz=(particle->pz());
    float CandidateE=(particle->energy());
    float CandidatePt=(particle->pt());
    float CandidatePhi=(particle->phi());
    int CandidateCharge=(particle->charge());
    int CandidatePdgId=(particle->pdgId());
    int CandidateType=(particle->particleId());
    string CandidateName=(particle->name());
  */
    //_________________________________________________________________________________________________________________   
    
 
  edm::Handle<GsfElectronCollection> gsfElectrons;
  iEvent.getByLabel("pixelMatchGsfElectrons",gsfElectrons); 
  int recelec=gsfElectrons->size();
  for (reco::GsfElectronCollection::const_iterator gsfIter=gsfElectrons->begin(); gsfIter!=gsfElectrons->end(); gsfIter++)
    {
      
      // edm::Handle<reco::TrackCollection> tracks;
     //iEvent.getByLabel(fTracksLabel1, tracks );
      //const  reco::TrackCollection  recTC = *(tracks.product());
      //for(TrackCollection::size_type i=0; i<recTC.size(); ++i) {
      //TrackRef track(tracks, i);
      //reco::Track tt(*track);
      //   const Track * RecoCandidate::bestTrack()const
      // Track Iter =gsfIter->bestTrack();
      // GsfTrackRef Iter =gsfIter->gsfTrack();
      // double tpt=Iter->pt();
      // float tpx =tt->px();
      // cout<<" tpx :"<<tpx<<" px :"<<px<<endl;
      
     
      float eta =gsfIter->eta();
      float phi =gsfIter->phi();
      float px =gsfIter->px();
      float py =gsfIter->py();
      float pt =sqrt((px*px+py*py));
      float pz =gsfIter->pz();
      float energy=gsfIter->energy();
      float ener=sqrt((px*px+py*py+pz*pz)); 
      float charge =gsfIter->charge();
      fHistRecElecPt->Fill(pt);
      fHistRecElecEta->Fill(eta);
      fHistRecElecPhi->Fill(phi);
      if(charge > 0) {
	pt1[ipositron] = pt;
	eta1[ipositron] =eta;
	phi1[ipositron] = phi;
	ppx1[ipositron]=px;
	ppy1[ipositron]=py;
	ppz1[ipositron]=pz;
	ener1[ipositron]=ener;
	ipositron++;
	nmp++;
      }
      if(charge < 0) {
	pt2[ielectron] =pt;
	eta2[ielectron] =eta;
	phi2[ielectron] =phi;
	ppx2[ielectron]=px;
	ppy2[ielectron]=py;
	ppz2[ielectron]=pz;
	ener2[ielectron]=ener;
	ielectron++;
	nmn++;
      }
    }//for gsfIter 
  
  if(nmp>0 && nmn>0)
    {
      for ( unsigned int i=0; i< ipositron; i++ ) {
	for ( unsigned int j=0; j< ielectron; j++ ){
	  double eptot=ener1[i]+ener2[j];
	  double ppxtot=ppx1[i]+ppx2[j];
	  double ppytot=ppy1[i]+ppy2[j];
	  double ppztot=ppz1[i]+ppz2[j];
	  double pptot=sqrt(ppxtot*ppxtot+ppytot*ppytot+ppztot*ppztot);
	  double recupt=sqrt((ppxtot*ppxtot)+(ppytot*ppytot));
	  double recup=sqrt((ppxtot*ppxtot)+(ppytot*ppytot)+(ppztot*ppztot));
	  double recupz=sqrt(ppztot*ppztot);
	  double thetau=acos(ppztot/pptot); 
	  double recueta=-log(tan(thetau/2.0));
	  double recuy=0.5*(log((eptot+ppztot)/(eptot-ppztot)));
	  double minv = sqrt(2.0*pt1[i]*pt2[j]*(TMath::CosH(eta1[i]-eta2[j])-TMath::Cos(phi1[i]-phi2[j])));
	  fHistRecDiElecPt->Fill(recupt);
	  fHistRecDiElecY->Fill(recuy);
	  fHistRecDiElecInvM->Fill(minv);
	  cout<<" reco Minv :"<<minv<<" Y: "<<recuy<<endl;
	  //cout <<"rec up Pt : "<<recupt <<"eta :"<<recueta<<"Invmass :"<<minv<<endl;
	  
	}
      }
      
    }
}

//*******************************************************************************************

void 
DiElecAnalyzer::Compare(const edm::Event &iEvent, const edm::EventSetup &iSetup) 
{
  
  // cout<<"------------------------------start compare--------------------------------"<<endl;
  
  using namespace edm;
  edm::Handle<HepMCProduct> evt;
  iEvent.getByLabel("source", evt);
  const HepMC::GenEvent *genEvent = evt->GetEvent();
  int gcnt = genEvent->particles_size();
  gcnt = 0;
  
  float pt1[100], eta1[100], phi1[100], ppx1[100], ppy1[100], ppz1[100],ener1[100];
  float pt2[100], eta2[100], phi2[100],ppx2[100], ppy2[100], ppz2[100],ener2[100];

  int ipositron=0, ielectron=0;
  int nmp=0;
  int nmn=0;
  
  for (HepMC::GenEvent::particle_const_iterator p = genEvent->particles_begin(); p != genEvent->particles_end(); ++p) 
    {
      float ID     = (*p)->pdg_id();
      float geta = (*p)->momentum().eta();
      float gphi = (*p)->momentum().phi();
      float gpt = (*p)->momentum().perp();
      float gpx = (*p)->momentum().px();
      float gpy = (*p)->momentum().py();
      float gpz = (*p)->momentum().pz(); 
      float gener=sqrt((gpx*gpx+gpy*gpy+gpz*gpz));
      //  cout <<"genrated electron Pt "<< gpt<< " eta: " << geta <<" ID: "<<ID<<endl; 
      
      //-------------------------TestInvMass Ana---------------------------------------
      //-------------------------------------------------------------------------------      
      
      edm::Handle<GsfElectronCollection> gsfElectrons;
      iEvent.getByLabel("pixelMatchGsfElectrons",gsfElectrons); 
      int relec=gsfElectrons->size();
      //cout<<"size "<<relec<<endl;      
      if(relec!=2)continue;      
      //cout<<"found dielec event"<<endl;   
      for (reco::GsfElectronCollection::const_iterator gsfIter=gsfElectrons->begin();
	   gsfIter!=gsfElectrons->end(); gsfIter++){
	float eta =gsfIter->eta();
	//float pt =gsfIter.pt();
	// float recID=gsfIter->id();
	float phi =gsfIter->phi();
	float px =gsfIter->px();
	float py =gsfIter->py();
	float pt =sqrt((px*px+py*py));
	float pz =gsfIter->pz();
	float ener=sqrt((px*px+py*py+pz*pz)); 
	float charge =gsfIter->charge();

	if (ID ==11 && charge ==-1)
	  {
	    float etadiff =(geta-eta);
	    float ptdiff=(gpt-pt);
	    //cout<<"  "<<gpt<<endl;
	    //cout<<"  "<<ID<<"  "<<gpt<<"  "<<gpz<<"  "<<gener<<"  "<<geta<<"  "<<gphi<<endl;
	    //cout<<ID<<"  "<<pt<<"  "<<pz<<"  "<<ener<<"  "<<eta<<"  "<<phi<<"  "<<gphi<<endl;
	    //cout<<" pt diff :"<<ptdiff<<" etadiff "<<etadiff<<endl;
	    //cout<<"  "<<ID<<"  "<<gpx<<"  "<<gpy<<"  "<<gpz<<"  "<<gener<<"  "<<geta<<"  "<<gphi<<endl;
            fHistRecElecPtDiff->Fill(gpz,pz);
	    fHistRecElecEtaDiff->Fill(geta,eta);
	    fHistRecElecPhiDiff->Fill(gphi,phi);
	    fHistRecElecDPt->Fill(ptdiff);
	  }    
	if (ID ==-11 && charge ==1)
	  {
	    float etadiff =(geta-eta);
	    float ptdiff=(gpt-pt);
	    //cout<<ID<<"  "<<pt<<"  "<<pz<<"  "<<ener<<"  "<<eta<<"  "<<phi<<"  "<<gphi<<endl;
	    //*****cout<<"  "<<ID<<"  "<<gpx<<"  "<<gpy<<"  "<<gpz<<"  "<<gener<<"  "<<geta<<"  "<<gphi<<endl;
	    //cout<<"  "<<charge<<"  "<<px<<"  "<<py<<"  "<<pz<<"  "<<ener<<"  "<<eta<<"  "<<phi<<endl;
	    //cout<<" pt diff :"<<ptdiff<<" etadiff "<<etadiff<<endl;
	    fHistRecElecPtDiff->Fill(gpz,pz);
	    fHistRecElecEtaDiff->Fill(geta,eta);
	    // fHistRecElecPhiDiff->Fill(gphi,phi);
            fHistRecElecDPt->Fill(ptdiff);
	  } 
      }      
      
      //-------------------------TestInvMass Ana---------------------------------------
      if(ID==-11) {
        pt1[ipositron] = gpt;
        eta1[ipositron] = geta;
        phi1[ipositron] = gphi;
        ppx1[ipositron]=gpx;
        ppy1[ipositron]=gpy;
        ppz1[ipositron]=gpz;
        ener1[ipositron]=gener;
        ipositron++;
        nmp++;
      }
      
      if(ID==11) {
        pt2[ielectron] = gpt;
        eta2[ielectron] = geta;
        phi2[ielectron] = gphi;
        ppx2[ielectron]=gpx;
        ppy2[ielectron]=gpy;
        ppz2[ielectron]=gpz;
        ener2[ielectron]=gener;
        ielectron++;
        nmn++;
      }
      //-------------------------------------------------------------------------------
      
    }
  if(nmp>0 && nmn>0){
    for (  unsigned int i=0; i< ipositron; i++ ) {
      for ( unsigned int j=0; j< ielectron; j++ ){
	double eptot=ener1[i]+ener2[j];
	double ppxtot=ppx1[i]+ppx2[j];
	double ppytot=ppy1[i]+ppy2[j];
	double ppztot=ppz1[i]+ppz2[j];
	double pptot=sqrt(ppxtot*ppxtot+ppytot*ppytot+ppztot*ppztot);
	double genupt=sqrt((ppxtot*ppxtot)+(ppytot*ppytot));
	double thetau=acos(ppztot/pptot);
	double genueta=-log(tan(thetau/2.0));
	double genuy=0.5*(log((eptot+ppztot)/(eptot-ppztot)));
	double genup=sqrt(ppxtot*ppxtot+ppytot*ppytot+ppztot*ppztot);
	double genupz=sqrt(ppztot*ppztot);
	double minv = sqrt(2.0*pt1[i]*pt2[j]*(TMath::CosH(eta1[i]-eta2[j])-TMath::Cos(phi1[i]-phi2[j])));
	double mpz=sqrt((eptot+pptot)*(eptot-pptot));
	//	cout<<"  "<<minv<<endl;
	//fHistGenDiElecInvM->Fill(minv);
	//cout<<" DiElectron Gen pt  "<<genupt<<" eta "<<genueta<<" invmass" <<minv<<endl;
      }
    }
  }
}
//*******************************************************************************************
// ------------ method called once each job just before starting event loop  ------------

void DiElecAnalyzer::beginJob(const edm::EventSetup&)
{
  fOutputFile   = new TFile( fOutputFileName.c_str(), "RECREATE" ) ;
  
  fHistGenElecPt= new TH1D( "fHistGenElecPt", "Elec Pt spectrum", 100,  0., 10.)  ;
  fHistGenElecEta= new TH1D( "fHistGenElecEta", "Elec eta spectrum", 100,  -10., 10.) ;
  fHistGenElecPhi= new TH1D( "fHistGenElecPhi", "Elec phi spectrum", 20,  -3.2, 3.2)  ;

  fHistSimElecPt =new TH1D( "fHistSimElecPt", "Sim Elec Pt spectrum", 100,  0., 10.)  ;
  fHistSimElecEta=new TH1D( "fHistSimElecEta", "Sim  Elec eta spectrum", 100,  -5., 5.)  ;  
  
  fHistFlowCandSize=new TH1D( "fHistFlowCandSize","FlowCandidateSize",100,0.,10);
  fHistFlowElecPt  =new TH1D( "fHistFlowElecPt","FlowElecPt",100,0.,10.);
  fHistFlowElecEta =new TH1D( "fHistFlowElecEta","FlowElecEta",100,-10.,10.); 
  fHistFlowElecPhi =new TH1D( "fHistFlowElecPhi","FlowElecPhi",100,-5.,5.);

  fHistRecElecPt =new TH1D( "fHistRecElecPt", "Rec Elec Pt spectrum", 100,  0., 10.0)  ;
  fHistRecElecEta=new TH1D( "fHistRecElecEta", "Rec Elec eta spectrum", 100,  -10., 10.)  ;
  fHistRecElecPhi=new TH1D( "fHistRecElecPhi", "Rec Elec phi spectrum", 20,  -3.2, 3.2)  ;
  
  fHistGenDiElecInvM =new TH1D( "fHistGenDiElecMinv", "Inv mass", 200,  0.0, 20.0)  ;
  fHistGenDiElecY    =new TH1D( "fHistGenDiElecY", "DiElecY", 100,  -5.0,5.0)  ;
  fHistGenDiElecPt   =new TH1D( "fHistGenDiElecPt", "DiElecPt", 200,  0.0,2.0)  ;
  
  fHistFlowDiElecY    =new TH1D( "fHistFlowDiElecY", "FlowDiElecY", 100,  -5.0,5.0)  ;
  fHistFlowDiElecPt   =new TH1D( "fHistFlowDiElecPt", "FlowDiElecPt", 200,  0.0,2.0)  ;
  fHistFlowDiElecInvM =new TH1D( "fHistFlowDiElecMinv", "FlowInvmass", 200,  0.0, 20.0);
 
  fHistRecDiElecInvM =new TH1D( "fHistRecDiElecMinv", "Rec Inv mass", 200,  0.0, 20.0)  ;
  fHistRecDiElecY    =new TH1D( "fHistRecDiElecY", "Rec DiElecY", 100,  -5.0,5.0)  ;
  fHistRecDiElecPt   =new TH1D( "fHistRecDiElecPt", "Rec DiElecPt", 200,  0.0,2.0)  ;
  
  fHistRecElecPtDiff=new TH2D("fHistRecElecPtDiff","Del Pt",100,-15.,15.,100,-15.,15.); 
  fHistRecElecPtDiff->SetXTitle("gen p_{T}");
  fHistRecElecPtDiff->SetYTitle("reco p_{T}");
  
  fHistRecElecEtaDiff=new TH2D("fHistRecElecEtaDiff","Del Eta",100,-5.,5.,100,-5.,5.); 
  fHistRecElecEtaDiff->SetXTitle("gen #eta");
  fHistRecElecEtaDiff->SetYTitle("reco #eta");
  // fHistRecElecPhiDiff=new TH2D("fHistRecElecPhiDiff","Del Phi",100,-4.,4.,100,-4.,4.);
  //fHistRecElecPhiDiff->SetXTitle("gen #phi");
  //fHistRecElecPhiDiff->SetYTitle("reco #phi");
  fHistRecElecPhiDiff=new TH1D( "fHistRecElecPhiDiff", "Delta #phi", 100,  -4.0, 4.0)  ;
  fHistRecElecPhiDiff->SetXTitle("delta #phi");
  
  fHistRecElecDPt=new TH1D( "fHistRecElecDPt", "delta Pt", 100, -3, 3.)  ;
  fHistRecElecDPt->SetXTitle("gen p_{T} - reco p_{T}");
  fHistRecElecDPt->SetYTitle("entries");
  
  Nevt=0;
}

//*******************************************************************************************
// ------------ method called once each job just after ending the event loop  ------------

void 
DiElecAnalyzer::endJob() {
  
  fOutputFile->cd();  
  fHistGenElecPt->Write();
  fHistGenElecEta->Write();
  fHistGenElecPhi->Write();
  
  fHistFlowCandSize->Write();
  fHistFlowElecPt->Write();
  fHistFlowElecEta->Write();
  fHistFlowElecPhi->Write();
  

  fHistRecElecPt->Write();
  fHistRecElecEta->Write();
  fHistRecElecPhi->Write();
  
 
 
  
  fHistSimElecPt->Write();
  fHistSimElecEta->Write();
  
  fHistGenDiElecInvM->Write();
  fHistGenDiElecY->Write();
  fHistGenDiElecPt->Write();
  
  fHistFlowDiElecInvM->Write();
  fHistFlowDiElecY->Write();
  fHistFlowDiElecPt->Write();

  


  fHistRecDiElecInvM->Write();
  fHistRecDiElecY->Write();
  fHistRecDiElecPt->Write();
  
  fHistRecElecPtDiff->Write();
  fHistRecElecEtaDiff->Write();
  fHistRecElecPhiDiff->Write();
  
  fHistRecElecDPt->Write();
 
  //fOutputFile->Write();
  fOutputFile->Close();
  // cout << " Total number of events analysed: " << Nevt << endl;  
  
}

//define this as a plug-in
DEFINE_FWK_MODULE(DiElecAnalyzer);
