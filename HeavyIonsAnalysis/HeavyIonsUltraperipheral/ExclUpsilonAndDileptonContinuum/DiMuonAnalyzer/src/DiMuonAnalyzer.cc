// -*- C++ -*-
//
// Package:    DiMuonAnalyzer
// Class:      DiMuonAnalyzer
// 
/**
Description: <one line class summary>

Implementation:
<Notes on implementation>
*/
//
// Original Author:  Vineet
//         Created:  Mon Jun 23 09:42:50 IST 2008
// $Id: DiMuonAnalyzer.cc,v 1.1 2008/06/23 22:32:56 kumarv Exp $
//
//

//Include Files

//System
#include <memory>
#include <iostream>
#include <string>
#include <map>
#include <set>

//Root
#include <TROOT.h>
#include <TSystem.h>
#include <TFile.h>
#include <TParameter.h>
#include <TH1.h>
#include <TH2.h>
#include <TTree.h>
#include <TVector3.h>
#include <TLorentzVector.h>

// user include files

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/EDProduct.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/TrackReco/interface/TrackExtraFwd.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "SimDataFormats/EncodedEventId/interface/EncodedEventId.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertexContainer.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertex.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"

#include "SimTracker/TrackAssociation/test/testTrackAssociator.h"
#include "SimTracker/Records/interface/TrackAssociatorRecord.h"
#include "SimTracker/TrackAssociation/interface/TrackAssociatorBase.h" 
#include "SimTracker/TrackAssociation/interface/TrackAssociatorByChi2.h"

#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/TrajectoryState/interface/FreeTrajectoryState.h"

#include "RecoPixelVertexing/PixelVertexFinding/interface/PVPositionBuilder.h"
#include "RecoPixelVertexing/PixelVertexFinding/interface/PVClusterComparer.h"
#include "RecoMuon/TrackingTools/interface/MuonServiceProxy.h"
#include "RecoVertex/VertexPrimitives/interface/VertexReconstructor.h"
#include "RecoVertex/PrimaryVertexProducer/interface/TrackFilterForPVFinding.h"
#include "RecoVertex/PrimaryVertexProducer/interface/TrackClusterizerInZ.h"
#include "RecoVertex/TrimmedKalmanVertexFinder/interface/KalmanTrimmedVertexFinder.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"
#include "RecoVertex/VertexTools/interface/VertexDistance3D.h"
#include "RecoVertex/VertexTools/interface/VertexDistanceXY.h"
#include "RecoVertex/VertexTools/interface/VertexCompatibleWithBeam.h"

#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "TBDataFormats/HcalTBObjects/interface/HcalTBTriggerData.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "MagneticField/Engine/interface/MagneticField.h"

using namespace std;
using namespace edm;
using namespace HepMC;

//********************************************************************************************************************
// class declaration
//
class TFile;
class TH1D;
class DiMuonAnalyzer : public edm::EDAnalyzer {
public:
  explicit DiMuonAnalyzer(const edm::ParameterSet&);
  ~DiMuonAnalyzer();
  std::string fSourceLabel;
  std::string fTracksLabel;
  std::string fTracksLabel1;
  std::string fOutputFileName ;
  
private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  virtual void SimAnalyze(const edm::Event &iEvent, const edm::EventSetup &iSetup); 
  virtual void RecAnalyze(const edm::Event &iEvent, const edm::EventSetup &iSetup); 
  
//*******************************************************************************************
// ----------member data ---------------------------
  
  TFile*      fOutputFile ;
  TH1D*       fHistGenmuPt ;
  TH1D*       fHistGenmuEta ;
  TH1D*       fHistGenmuPhi ;
  TH1D*       fHistSimmuPt ;
  TH1D*       fHistSimmuEta ; 
  TH1D*       fHistrecmuPt; 
  TH1D*       fHistrecmuEta; 
  TH1D*       fHistrecmuPhi;
  TH1D*       fHistDiMuonGen_Y;
  TH1D*       fHistDiMuonGen_Pt;
  TH1D*       fHistDiMuonGen_P;
  TH1D*       fHistDiMuonGen_Pz;
  TH1D*       fHistDiMuonGen_InvM;
 // TH1D*       fHistDiMuonGen_Pt1;
 // TH1D*       fHistDiMuonGen_Y1;
  TH1D*       fHistDiMuonRec_Y;
  TH1D*       fHistDiMuonRec_Pt;  
  TH1D*       fHistDiMuonRec_P;  
  TH1D*       fHistDiMuonRec_Pz;  
  TH1D*       fHistDiMuonRec_InvM;
  TH1D*       fPartID ;
  unsigned int  Nevt;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//*******************************************************************************************
//
// constructors and destructor
//
DiMuonAnalyzer::DiMuonAnalyzer(const edm::ParameterSet& iConfig):
  fOutputFileName(iConfig.getUntrackedParameter<string>("HistOutFile",std::string("DiMuonAnalysis.root")) ),
  fOutputFile(0),  fHistGenmuPt(0), fHistGenmuEta(0), fHistGenmuPhi(0),fHistSimmuPt(0),fHistSimmuEta(0),fHistrecmuPt(0),fHistrecmuPhi(0),fHistDiMuonGen_Y(0),fHistDiMuonGen_Pt(0),fHistDiMuonGen_P(0),fHistDiMuonGen_Pz(0),fHistDiMuonGen_InvM(0),fHistDiMuonRec_Y(0),fHistDiMuonRec_Pt(0),fHistDiMuonRec_P(0),fHistDiMuonRec_Pz(0),fHistDiMuonRec_InvM(0),fPartID(0),Nevt(0)
{ 
 // fTracksLabel = iConfig.getUntrackedParameter<string>("inputTracklabel", "ctfWithMaterialTracks");  
  //fTracksLabel1 = iConfig.getUntrackedParameter<string>("inputTracklabel", "globalMuons"); 
  
}

//*******************************************************************************************
//
DiMuonAnalyzer::~DiMuonAnalyzer()
{
  
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  
}

//
// member functions
//

//*******************************************************************************************
// ------------ method called to for each event  ------------
void 
DiMuonAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  using namespace HepMC;
  using namespace edm;

  using reco::VertexCollection;
  using reco::TrackCollection;
  using reco::MuonCollection;

  int eventnumber = iEvent.id().event();
  int runnumber   = iEvent.id().run(); 
  cout << "--------------------------------------------------------------------------------"<<endl;
  cout << "EVENT NUMBER = " << eventnumber <<"   "<<"RUN NUMBER = "<<runnumber<<endl;
  cout <<"----------------Generation-Ana----------------------------" << endl;
  cout << "Generation Event No.= " << Nevt +1 << endl;
  
  
  edm::Handle<HepMCProduct> evt;
  iEvent.getByLabel("source", evt);
  const HepMC::GenEvent *genEvent = evt->GetEvent();
  int gcnt = genEvent->particles_size();
  cout << "Total no of Particles in the event = " <<gcnt << endl;

  gcnt = 0;
  float pt1[100], eta1[100], phi1[100], ppx1[100], ppy1[100], ppz1[100],ener1[100];
  float pt2[100], eta2[100], phi2[100],ppx2[100], ppy2[100], ppz2[100],ener2[100];
  int imuplus=0, imuminus=0;
  int nmp=0;
  int nmn=0;

  const float muonmass = 0.105658369; //  GeV

  for (HepMC::GenEvent::particle_const_iterator p = genEvent->particles_begin();
       p != genEvent->particles_end(); ++p) {
    
    int ID     = (*p)->pdg_id();
    float eta = (*p)->momentum().eta();
    float phi = (*p)->momentum().phi();
    float pt = (*p)->momentum().perp();
    float px = (*p)->momentum().px();
    float py = (*p)->momentum().py();
    float pz = (*p)->momentum().pz(); 
    float ener=sqrt((px*px+py*py+pz*pz+muonmass*muonmass));
    fPartID->Fill(ID);
    cout <<"genrated muon Pt "<< pt<< " px " << px << "py "<< py << " " <<(px*px + py*py)<<endl; 
    
    //  if(eta < -2.4 || eta > 2.4) continue;    
    if(abs(ID)==13){
      fHistGenmuPt->Fill(pt);
      fHistGenmuEta->Fill(eta);
      fHistGenmuPhi->Fill(phi);
    }
    if(ID==-13) {
      pt1[imuplus] = pt;
      eta1[imuplus] = eta;
      phi1[imuplus] = phi;
      ppx1[imuplus]=px;
      ppy1[imuplus]=py;
      ppz1[imuplus]=pz;
      ener1[imuplus]=ener;
      imuplus++;
      nmp++;
    }  
    if(ID==13) {
      pt2[imuminus] = pt;
      eta2[imuminus] = eta;
      phi2[imuminus] = phi;
      ppx2[imuminus]=px;
      ppy2[imuminus]=py;
      ppz2[imuminus]=pz;
      ener2[imuminus]=ener;
      imuminus++;
      nmn++;
    }
    
  }//genrated particle loop
  
    
  if(nmp>0 && nmn>0){
    // fill muon properties and do 2-mu inv.mass
    for ( unsigned int i=0; i< imuplus; i++ ) {
      for ( unsigned int j=0; j< imuminus; j++ ){
	double eptot=ener1[i]+ener2[j];
	double ppxtot=ppx1[i]+ppx2[j];
	double ppytot=ppy1[i]+ppy2[j];
	double ppztot=ppz1[i]+ppz2[j];
	double pptot=sqrt(ppxtot*ppxtot+ppytot*ppytot+ppztot*ppztot);
	
	double genupt=sqrt((ppxtot*ppxtot)+(ppytot*ppytot));
	double genup=sqrt(ppxtot*ppxtot+ppytot*ppytot+ppztot*ppztot);
	double genupz=sqrt(ppztot*ppztot);
	double thetau=acos(ppztot/pptot); 
	double genueta=-log(tan(thetau/2.0));
	double genuy=0.5*(log((eptot+ppztot)/(eptot-ppztot)));
	double minv = sqrt(2.0*pt1[i]*pt2[j]*(TMath::CosH(eta1[i]-eta2[j])-TMath::Cos(phi1[i]-phi2[j])));
	double mpz=sqrt((eptot+pptot)*(eptot-pptot));
	
	//cout<<" DiMuon Gen p  "<<ppxtot<<"  "<<ppytot<<"  "<<ppztot<<" "<<pptot<<" pt "<<genupt<<endl;	
	cout<<" DiMuon Gen p  "<<genup<<" pt "<<genupt<<" Pz " <<genupz<<endl;
	
	fHistDiMuonGen_InvM->Fill(minv);
	fHistDiMuonGen_Y->Fill(genuy);
	fHistDiMuonGen_Pt->Fill(genupt);
	fHistDiMuonGen_P->Fill(pptot);
	fHistDiMuonGen_Pz->Fill(ppztot);
	//	cout<<"Invmass of Up at genrator level "<<minv<<"  by ppt  "<<mpz<<endl; 
      } // for j
    } // for i
    
 }// if(nmp......
 
  
  SimAnalyze(iEvent, iSetup); 
  RecAnalyze(iEvent, iSetup);   

  Nevt++;

}//analyze

//*******************************************************************************************
//
void DiMuonAnalyzer::SimAnalyze(const edm::Event &iEvent, const edm::EventSetup &iSetup) 
{
  
  using namespace edm;
  
  edm::Handle<SimTrackContainer> simTracks;
  iEvent.getByLabel("g4SimHits",simTracks);
  SimTrackContainer::const_iterator simTrack;

  cout<<"====================================Simulated tracks: "
    << "========================================"<<endl;

  for (simTrack = simTracks->begin(); simTrack != simTracks->end(); ++simTrack){
    if (abs((*simTrack).type()) == 13) {
      
      float spx=(*simTrack).momentum().px();
      float spy=(*simTrack).momentum().py();
      float spz=(*simTrack).momentum().pz();
      float spt=TMath::Sqrt(spx*spx+spy*spy);
      float seta = (*simTrack).momentum().eta(); 
      // if(eta < -2.4 || eta > 2.4 || pt < 5.0) continue;
      
      fHistSimmuPt->Fill(spt);
      fHistSimmuEta->Fill(seta);
      
      //cout<<"Sim pT= "<< spt <<"  Sim Eta= "<< seta <<endl;
    }//abs Id    
  }// for (simTrac loop .....................
  
}//SimAna

//*******************************************************************************************
//
void DiMuonAnalyzer::RecAnalyze(const edm::Event &iEvent, const edm::EventSetup &iSetup) 
{
  using reco::TrackCollection;
  using reco::MuonCollection;

  cout <<"--------------Reco-Ana------------------------------------" << endl;
  
  //rcnt = 0;
  float pt1[100], eta1[100], phi1[100],ppx1[100], ppy1[100], ppz1[100],ener1[100];
  float pt2[100], eta2[100], phi2[100],ppx2[100], ppy2[100], ppz2[100],ener2[100];
  int imuplus=0, imuminus=0;
  int nmp=0;
  int nmn=0;
  
  // muon collection
  edm::Handle<MuonCollection> tmuons;
  iEvent.getByLabel( "muons", tmuons );      
  for (reco::MuonCollection::const_iterator muon = tmuons->begin(); muon != tmuons->end(); ++muon) {

    //if( muon->(!isStandAloneMuon()) && muon->isGlobalMuon()){
    // if(muon->isTrackerMuon()){  
    //TrackRef tt = muon->combinedMuon(); //global muon refrence	

    if(muon->isGlobalMuon()){
      
      TrackRef tt = muon->track();
      float pt = tt->pt();
      float eta = tt->eta();
      float phi = tt->phi();
      float px = tt->px();
      float py = tt->py();
      float pz = tt->pz();
      float p=(px*px+py*py+pz*pz);
      float ener=sqrt((px*px+py*py+pz*pz+0.011163691));
      int ch= tt->charge();
      
      cout<<"global muon  taking tracker  pt "<<pt<< " eta " <<eta <<endl;
      
      //if((eta >-2.0 && eta <2.0)){
      fHistrecmuPt->Fill(pt);
      fHistrecmuEta->Fill(eta);
      fHistrecmuPhi->Fill(phi);
      // cout << " rec p= " << p << " rec eta= " << eta <<" Charge :"<<tt.charge()<<endl;
      if(tt->charge() > 0) {
	pt1[imuplus] = tt->pt();
	eta1[imuplus] = tt->eta();
	phi1[imuplus] = tt->phi();
	ppx1[imuplus]=px;
	ppy1[imuplus]=py;
	ppz1[imuplus]=pz;
	ener1[imuplus]=ener;
	imuplus++;
	nmp++;
      }
      if(tt->charge() < 0) {
	pt2[imuminus] = tt->pt();
	eta2[imuminus] = tt->eta();
	phi2[imuminus] = tt->phi();
	ppx2[imuminus]=px;
	ppy2[imuminus]=py;
	ppz2[imuminus]=pz;
	ener2[imuminus]=ener;
	imuminus++;
	nmn++;
      }
      
    }//is global muon
    
  }//reconstructed track loop    

  // Dimuon Invariant mass analysis
  
  if(nmp>0 && nmn>0)
    {
      
      for ( unsigned int i=0; i< imuplus; i++ ) {
	for ( unsigned int j=0; j< imuminus; j++ ){
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
	 
	  cout<<" DiMuon Rec p  "<<recup<<" pt "<<recupt<<" Pz " <<recupz<<endl;
	  
	  // cout<<" DiMuon reco p  "<<ppxtot<<"  "<<ppytot<<"  "<<ppztot<<"P "<<pptot<<" pt  " <<recupt<<endl;	   
	  
	  fHistDiMuonRec_Y->Fill(recuy);
	  fHistDiMuonRec_Pt->Fill(recupt);
	  fHistDiMuonRec_P->Fill(pptot);
	  fHistDiMuonRec_Pz->Fill(ppztot);
	  fHistDiMuonRec_InvM->Fill(minv) ;
	} // for j
      } // for i
    
      
    }//nmp...........    
 
  cout <<"-------------END-Reco-------------------------------------" << endl <<endl;
}

//*******************************************************************************************
// ------------ method called once each job just before starting event loop  ------------
void 
DiMuonAnalyzer::beginJob(const edm::EventSetup&)
{
  fOutputFile   = new TFile( fOutputFileName.c_str(), "RECREATE" ) ;
  
  fHistGenmuPt     = new TH1D( "fHistGenmuPt", "Muon Pt spectrum", 100,  0., 10.)  ;
  fHistGenmuEta    = new TH1D( "fHistGenmuEta", "Muon eta spectrum", 100,  -5., 5.)  ;
  fHistGenmuPhi    = new TH1D( "fHistGenmuPhi", "Muon phi spectrum", 20,  -3.2, 3.2)  ;
  fHistSimmuPt  = new TH1D( "fHistSimmuPt", "SimMuon Pt spectrum", 100,  0., 10.)  ;
  fHistSimmuEta = new TH1D( "fHistSimmuEta", "SimMuon eta spectrum", 100,  -5., 5.)  ;
  fHistrecmuPt  = new TH1D( "fHistrecmuPt", "recMuon Pt spectrum", 100,  0., 10.)  ;
  fHistrecmuEta = new TH1D( "fHistrecmuEta", "recMuon eta spectrum", 100,  -5., 5.)  ; 
  fHistrecmuPhi = new TH1D( "fHistrecmuPhi", "recMuon phi spectrum", 20,  -3.2, 3.2)  ;

  fHistDiMuonGen_Y   = new TH1D(  "fHistDiMuonGen_Y", "rapadity_Up", 100,  -5., 5. ) ;
  fHistDiMuonGen_Pt  = new TH1D(  "fHistDiMuonGen_Pt", "Pt_Up", 50,  0.,0.2) ;
  fHistDiMuonGen_P   = new TH1D(  "fHistDiMuonGen_P", "P_Up", 100,  0., 50) ;
  fHistDiMuonGen_Pz  = new TH1D(  "fHistDiMuonGen_Pz", "Pz_Up", 100,  0.,30) ;
  fHistDiMuonGen_InvM = new TH1D("fHistDiMuonGen_InvM ","Gen_dimu minv (GeV)",200,7,12.0);
 // fHistDiMuonGen_Pt1    =new TH1D(  "DiMuonGenPt1", "genPt_Up1", 50,  0.,0.2) ;
  //fHistDiMuonGen_Y1     = new TH1D(  "DiMuonGenYfoeEff", "rapadity_UpforE", 100,  -5., 5. ) ;
  fHistDiMuonRec_Y   = new TH1D( "fHistDiMuonRec_Y", "Rec_rapadity_dimu", 100,  -5., 5. ) ;
  fHistDiMuonRec_Pt  = new TH1D( "fHistDiMuonRec_Pt", "Rec_Pt_dimu", 50,  0., 0.2 ) ;
  fHistDiMuonRec_P   = new TH1D( "fHistDiMuonRec_P", "Rec_P_dimu", 100,  0.,50) ;
  fHistDiMuonRec_Pz  = new TH1D( "fHistDiMuonRec_Pz", "Rec_pz_dimu", 100,  0.,30) ;
  fHistDiMuonRec_InvM = new TH1D("fHistDiMuonRec_InvM ","rec_minv_dimu (GeV)",150,7,12.0);
  fPartID       = new TH1D( "HistPartID", "Particle ID", 800,  -400, 400.)  ;
  Nevt=0;

 // TH2(const char *name,const char *title,Int_t nbinsx,Double_t xlow,Double_t xup ,Int_t nbinsy,Double_t ylow,Double_t yup)
  
 }

//*******************************************************************************************
// ------------ method called once each job just after ending the event loop  ------------
void 
DiMuonAnalyzer::endJob() 
{
  fOutputFile->cd();  
  fHistGenmuPt->Write();
  fHistGenmuEta->Write();
  fHistGenmuPhi->Write();
  fHistSimmuPt->Write();
  fHistSimmuEta->Write();
  fHistrecmuPt->Write();
  fHistrecmuEta->Write();
  fHistrecmuPhi->Write();	
  fHistDiMuonGen_Y->Write();
  fHistDiMuonGen_Pt->Write(); 
  fHistDiMuonGen_P->Write();
  fHistDiMuonGen_Pz->Write();
  fHistDiMuonGen_InvM->Write();
  // fHistDiMuonGen_Pt1->Write();
  // fHistDiMuonGen_Y1->Write(); 
  fHistDiMuonRec_Y->Write();
  fHistDiMuonRec_Pt->Write();
  fHistDiMuonRec_P->Write();
  fHistDiMuonRec_Pz->Write();
  fHistDiMuonRec_InvM->Write();
  fOutputFile->Write();
  fOutputFile->Close();
  cout << " Total number of events analysed: " << Nevt << endl;
}

//define this as a plug-in
DEFINE_FWK_MODULE(DiMuonAnalyzer);
