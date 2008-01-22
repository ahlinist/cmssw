#include <iostream>

#include "GeneratorInterface/MuEnrichInterface/test/MuAnalyzer.h"
 
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticleFwd.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertexContainer.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertex.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h" 
#include "DataFormats/Provenance/interface/EventID.h"

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
 
#include "FWCore/Framework/interface/MakerMacros.h"


using namespace edm;
using namespace std;

typedef RefVector<vector<TrackingParticle> > TrackingParticleContainer;
typedef vector<TrackingParticle>             TrackingParticleCollection;

typedef TrackingParticleRefVector::iterator               tp_iterator;
typedef TrackingParticle::g4t_iterator                   g4t_iterator;
typedef TrackingParticle::genp_iterator                 genp_iterator;
typedef TrackingVertex::genv_iterator                   genv_iterator;
typedef TrackingVertex::g4v_iterator                     g4v_iterator;


 
MuAnalyzer::MuAnalyzer( const ParameterSet& pset ): 
  fOutputFileName( pset.getUntrackedParameter<string>("HistOutFile",std::string("TestMuProducer.root")) ),
  maxeta(pset.getUntrackedParameter<double>("maxeta", 2.5)),
  ptmin(pset.getUntrackedParameter<double>("ptmin",1.8)),
  intlum(pset.getUntrackedParameter<double>("intlumi",4.5e-4)),
  wtype(pset.getUntrackedParameter<int>("type",0))
{
}

void MuAnalyzer::beginJob( const EventSetup& )
{
  fOutputFile   = new TFile( fOutputFileName.c_str(), "RECREATE" ) ;
  fHistNMu  = new TH1D(  "HistNMu", "Number of muons above cut", 5,  0., 5. ) ;
  fHistPtMu  = new TH1D(  "HistPtMu", "Muon transverse momentum", 120,  0., 30. ) ;
  fHistMuweight  = new TH1D(  "HistMuweight", "Mu event weights", 100, 0., 1. ) ;
  fHistMuRZ  = new TH2D(  "HistMuRZ", "Mu production Vertex R vs Z", 300, -5700., 5700.,300, 0., 3000.) ;
  fHistMuXY  = new TH2D(  "HistMuXY", "Mu production Vertex XY", 300, -150., 150.,300, -150., 150.) ;
  fHistMuPvsPt  = new TH2D(  "HistMuPvsPt", "Mu P vs Pt", 100, 0., 60.,100, 0., 60.) ;
  fHistMuPvsEta  = new TH2D(  "HistMuPvsEta", "Mu P vs Eta", 100, -2.5, 2.5,100, 0., 60.) ;
  fHistMuZ   = new TH1D(  "HistMuZ", "Mu Vertex Z",   600, -300., 300. ) ;     
  fHistMuX   = new TH1D(  "HistMuX", "Mu Vertex X",   600, -300., 300. ) ;     
  fHistMuY   = new TH1D(  "HistMuY", "Mu Vertex Y",   600, -300., 300. ) ;     
  fHistMuEta   = new TH1D(  "HistMuEta", "Eta of muon",   100, -5., 5. ) ;
  fHistMuPhi  = new TH1D(  "HistMuPhi", "Mu Phi", 300, -3.142, 3.142) ;
  fHistRate33   = new TH1D(  "HistRate33", "Rate at Low lumi",   100, 0., 80. ) ;
  fHistRate34   = new TH1D(  "HistRate34", "Rate at High lumi",   100, 0., 80. ) ;
  fHistRate34B   = new TH1D(  "HistRate34B", "Rate at High lumi from b",   100, 0., 80. ) ;
  fHistRate34C   = new TH1D(  "HistRate34C", "Rate at High lumi from c",   100, 0., 80. ) ;
  fHistRate34kaon   = new TH1D(  "HistRate34kaon", "Rate at High lumi from K",   100, 0., 80. ) ;
  fHistRate34pion   = new TH1D(  "HistRate34pion", "Rate at High lumi from pi/eta",   100, 0., 80. ) ;
  fHistRate34tau   = new TH1D(  "HistRate34tau", "Rate at High lumi from tau",   100, 0., 80. ) ;
  fHistRate34W   = new TH1D(  "HistRate34W", "Rate at High lumi from W",   100, 0., 80. ) ;
  fHistRate34Z   = new TH1D(  "HistRate34Z", "Rate at High lumi from Z",   100, 0., 80. ) ;
  fHistRate34Other   = new TH1D(  "HistRate34Other", "Rate at High lumi from Other",   100, 0., 80. ) ;
  fHistParentDecayLength   = new TH1D(  "HistMuParentDecayLength", "Mu Parent Decay Length",   630, 0., 6300. ) ;
  fHistMuParent = new TH1D(  "HistMuParent", "Mu Parent ifd",   1500, 0., 1500. ) ;
  fHistMuParentStatus   = new TH1D(  "HistMuParentStatus", "Mu Parent Status",   15, 0., 15 ) ;
  fHistIsub   = new TH1D(  "HistIsub", "Process producing muons",   90, 10., 100. ) ;
  tmuon = new TTree ("Muon","Highest Muon Pt Generator Info");
  tmuon->Branch("run",&runnum,"runnum/I");
  tmuon->Branch("evt",&evtnum,"evtnum/I");
  tmuon->Branch("parton",&parton,"parton/I");
  tmuon->Branch("simmuon",&simmuon,"simmuon/O");
  tmuon->Branch("simpt",&simpt,"simpt/D");
  tmuon->Branch("eta",&etamu,"etamu/D");
  tmuon->Branch("phi",&phimu,"phimu/D");
  tmuon->Branch("px",&pxmu,"pxmu/D");
  tmuon->Branch("py",&pymu,"pymu/D");
  tmuon->Branch("pz",&pzmu,"pzmu/D");
  tmuon->Branch("pt",&ptmu,"ptmu/D");
  tmuon->Branch("vx",&vxmu,"vxmu/D");
  tmuon->Branch("vy",&vymu,"vymu/D");
  tmuon->Branch("vz",&vzmu,"vzmu/D");
  tmuon->Branch("vxge",&vxge,"vxge/D");
  tmuon->Branch("vyge",&vyge,"vyge/D");
  tmuon->Branch("vzge",&vzge,"vzge/D");
  tmuon->Branch("wt",&wtmu,"wtu/D");
  tmuon->Branch("parent",&pid,"pid/I");
  tmuon->Branch("decaylength",&decayl,"decayl/D");
  tmuon->Branch("isub",&isub,"isub/I");
 return ;
}
 
void MuAnalyzer::analyze( const Event& e, const EventSetup& )
{
      
  runnum=(int) e.id().run();
  evtnum=(int) e.id().event();
  // Sim track info 
  Handle<TrackingParticleCollection>  TruthTrackContainer ;
  e.getByType(TruthTrackContainer);
  double simptmax=ptmin;
  simmuon=false;
  vxge=vyge=vzge=-99999.;
  const TrackingParticleCollection *tPC   = TruthTrackContainer.product();
  for (TrackingParticleCollection::const_iterator t = tPC -> begin(); t != tPC -> end(); ++t) {
    int simpdgid=t->pdgId();
    if ( abs(simpdgid)==13) {
      bool wtype1ok=false;
      if ( wtype==1 ) wtype1ok=true;
	for (TrackingParticle::genp_iterator hepT = t -> genParticle_begin();
	     hepT !=  t -> genParticle_end(); ++hepT) {
	  double pthep=(*hepT)->momentum().perp();
	  double etahep=(*hepT)->momentum().eta();
	  if ( wtype1ok ) {
	    if (pthep > 4.0 )wtype1ok=false ;
	    if (etahep>maxeta)wtype1ok=false ;
	    if (etahep< 1.2 && pthep<3.0)wtype1ok=false ;
	    if (etahep> 1.2 && etahep< 1.7 && pthep<1.8)wtype1ok=false;
	    if (etahep> 1.7 && etahep<maxeta &&(*hepT)->momentum().rho()<3.5)wtype1ok=false;
	  }
	  if ( ( wtype1ok && wtype==1) || (wtype!=1 && pthep>simptmax && etahep<maxeta ) ){
	    cout << " Found a muon pt="<<t->pt()<<" eta="<<t->eta()<<"gen: pt eta"<<pthep<<" "<<etahep<<endl;
	    simmuon=true;
	    simptmax=t->pt();
	    simpt=simptmax;



	    /* cout << " HepMC Track Pt " <<pthep<<" and id "<<(*hepT)->pdg_id()<<endl;    
	    for (TrackingParticle::g4t_iterator g4T = t -> g4Track_begin();
		 g4T !=  t -> g4Track_end(); ++g4T) {
	      cout << " Geant Track Pt " << g4T->momentum().perp() << endl;   
	    }
	    */
	    TrackingVertexRef parentV = t -> parentVertex();
	    if (parentV.isNull()) {
	      cout << "No parent vertex" << endl;
	    } else {  
	      cout << " Parent  vtx position " << parentV -> position() << endl;
	      vxge=parentV -> position().x();   
	      vyge=parentV -> position().y();   
	      vzge=parentV -> position().z();   
	      // Loop over source track(s)
	      for (tp_iterator iTP = parentV -> sourceTracks_begin(); iTP != parentV -> sourceTracks_end(); ++iTP) {
		cout << "  Source   starts: " << (*iTP)->vertex() <<", source id: " <<(*iTP)->pdgId();
		for (g4t_iterator g4T  = (*iTP)->g4Track_begin(); g4T != (*iTP)->g4Track_end(); ++g4T) cout << ", p " <<  g4T->momentum();
		cout << endl;     	       
		//TrackingVertexRef parentdecayV= (*iTP)->decayVertex();
		//if (parentdecayV != parentV) cout << " Parent Decay Vertex not equal to daughter origin"<<endl;
	      }         
	    }
	  }
	}
    }
  }

  // gen track info
  Handle< HepMCProduct > EvtHandle ;
  e.getByLabel( "VtxSmeared", EvtHandle ) ;
  //int eventsinrun=0; 
  const HepMC::GenEvent* Evt = EvtHandle->GetEvent() ;
  if (Evt != 0 ) {   
    int nmuon=0;
    double ptmax=ptmin;
    double wt=1.;
    parton=0;
    if ( Evt->weights().size() > 0 ) wt=Evt->weights()[0];
    for ( HepMC::GenEvent::vertex_const_iterator
	    vit=Evt->vertices_begin(); vit!=Evt->vertices_end(); ++vit )
      {
	HepMC::FourVector muprodv=(*vit)->position();
	for ( HepMC::GenVertex::particles_out_const_iterator
		pout=(*vit)->particles_out_const_begin();
	      pout!=(*vit)->particles_out_const_end(); ++pout )
	  {
	    double pt=(*pout)->momentum().perp();
	    double p=(*pout)->momentum().rho();
	    int id=(*pout)->pdg_id();
	    if (abs(id) < 7 && parton==0 ){
	      //cout << " Original particle: "<<id <<" status="<<(*pout)->status()<< endl;  
	      parton=abs(id);
	    }
	    double eta = abs((*pout)->momentum().eta());
	    double phi = (*pout)->momentum().phi();
	    bool wtype1ok=false;
	    if ( abs(id) == 13 && wtype==1 ) wtype1ok=true;
	    if ( wtype1ok ) {
	      if( pt > 4.0 )wtype1ok=false ;
	      if (eta>maxeta)wtype1ok=false ;
	      if (eta< 1.2 && pt<3.0)wtype1ok=false ;
	      if (eta> 1.2 && eta< 1.7 && pt<1.8)wtype1ok=false;
	      if (eta> 1.7 && eta<maxeta && p<3.5)wtype1ok=false;
	    }
	    if ((abs(id) == 13 && pt>ptmin && wtype != 1)||(wtype==1 && wtype1ok)) 
	      {	
		++nmuon;
		if ( pt > ptmax && eta<maxeta) {
		  ptmax=pt;
		  etamu=(*pout)->momentum().eta();
		  phimu=phi;
		  ptmu=pt;
		  decayl=-1;
		  vxmu=muprodv.x();
		  vymu=muprodv.y();
		  vzmu=muprodv.z();
		  pxmu=(*pout)->momentum().x();
		  pymu=(*pout)->momentum().y();
		  pzmu=(*pout)->momentum().z();
		  pid=0;
		  for (HepMC::GenVertex::particles_in_const_iterator ic=(*vit)->particles_in_const_begin() ; ic!=(*vit)->particles_in_const_end() ; ic++ )
		    {
		      pid=(*ic)->pdg_id();
		      //MuonParents[ic]->print() ;
		      fHistMuParent->Fill(abs(pid));
		      fHistMuParentStatus->Fill((*ic)->status());
		      if ((*ic)->production_vertex() != 0 ) {
			HepMC::FourVector parentprodv=(*ic)->production_vertex()->position();
			double decaylength=sqrt((muprodv.x()-parentprodv.x())*(muprodv.x()-parentprodv.x())
						+(muprodv.y()-parentprodv.y())*(muprodv.y()-parentprodv.y())
						+(muprodv.z()-parentprodv.z())*(muprodv.z()-parentprodv.z()));
			decayl=decaylength;
			//cout << "Decay Length ="<<decaylength<<endl;
			fHistParentDecayLength->Fill(decaylength);
		      }else cout << " Could not find muon parent production vertex "<<endl;
		    }
		}
	      }
	  }
      }
    if ( nmuon == 0 ) 
      {
	cout << " There is NO Muon in this event ! " << endl ;
	return ;
      } else {
	TH1D *fH=fHistRate34Other;
	int apid=abs(pid);
	if ( apid==5|| (apid>500 && apid<600)){
	  fH=fHistRate34B;
	if(wtype==3)wt/=8.4;
	}else if ( apid==4 || (apid>400 && apid<500) ){
	  fH=fHistRate34C;
	  if(wtype==3)wt/=6.0;
	}else if ( apid==15){
	  fH=fHistRate34tau;
	  if(wtype==3)wt/=8.7;
	}else if ( apid==311 || apid==321 || apid==130 || apid==310 ){
	fH=fHistRate34kaon;
	if(wtype==3)wt/=7.3;
	}else if ( apid==23 ) fH=fHistRate34Z;
	else if ( apid==24 ) fH=fHistRate34W;
	else if ( apid==211 || apid==221){
	  fH=fHistRate34pion;
	  if(wtype==3)wt/=0.8;
      }
	isub=Evt->signal_process_id();
	fHistIsub->Fill(Evt->signal_process_id()) ;
	fHistMuRZ->Fill(vzmu,sqrt(vxmu*vxmu+vymu*vymu),wt);
	fHistMuEta->Fill(etamu,wt);
	fHistMuPhi->Fill(phimu,wt);
	fHistPtMu->Fill(ptmu,wt ) ;
	fHistMuXY->Fill(vxmu,vymu,wt);
	fHistMuZ->Fill(vzmu,wt);
	fHistMuPvsPt->Fill(ptmu,sqrt(ptmu*ptmu+pzmu*pzmu),wt);
	fHistMuPvsEta->Fill(etamu,sqrt(ptmu*ptmu+pzmu*pzmu),wt);
	
	int imax;
	cout << "Pt max "<<ptmax<<" integrated lum "<<intlum<<endl;
	imax=(int) (ptmax/0.8);
	for (int i=0 ;i<imax ; ++i ) {
	  double pp;
	  double ww;
	  pp=0.4+i*0.8;
	  ww=2*(wt)/intlum;
	  fHistRate33->Fill(pp,ww);
	  fHistRate34->Fill(pp,ww*5);
	  fH->Fill(pp,ww*5);
	}
      }
    cout<<" There is are "<<nmuon<<" muons in this event ! " << endl ;
    fHistMuweight->Fill(wt);
    //cout << " About to fill nmu"<<endl;
    fHistNMu->Fill(nmuon,wt);
    wtmu=wt;
    tmuon->Fill();
    //cout << "Event finished"<<endl;
    return ;
  }
}

void MuAnalyzer::endJob()
{
       
fOutputFile->Write() ;
fOutputFile->Close() ;
   
return ;
}
 
DEFINE_FWK_MODULE(MuAnalyzer);
