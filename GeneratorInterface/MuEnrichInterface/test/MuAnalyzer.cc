#include <iostream>

#include "GeneratorInterface/MuEnrichInterface/test/MuAnalyzer.h"
 
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
 
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/Handle.h"
 
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
 
#include "FWCore/Framework/interface/MakerMacros.h"
 
using namespace edm;
using namespace std;

 
MuAnalyzer::MuAnalyzer( const ParameterSet& pset ): 
  fOutputFileName( pset.getUntrackedParameter<string>("HistOutFile",std::string("TestMuProducer.root")) ),
  maxeta(pset.getUntrackedParameter<double>("maxeta", 2.5)),
  ptmin(pset.getUntrackedParameter<double>("ptmin",1.8)),
  intlum(pset.getUntrackedParameter<double>("intlumi",4.5e-4)),
  wtype(pset.getUntrackedParameter<int>("type",1)),
  fOutputFile(0), fHistPtMu(0), fHistMuweight(0), fHistMuParent(0),fHistMuEta(0),fHistRate33(0),fHistRate34(0),fHistParentDecayLength(0),fHistOtherDecayLength(0)
{
}

void MuAnalyzer::beginJob( const EventSetup& )
{
  fOutputFile   = new TFile( fOutputFileName.c_str(), "RECREATE" ) ;
  fHistNMu  = new TH1D(  "HistNMu", "Number of muons above cut", 5,  0., 5. ) ;
  fHistPtMu  = new TH1D(  "HistPtMu", "Muon transverse momentum", 120,  0., 30. ) ;
  fHistMuweight  = new TH1D(  "HistMuweight", "Mu event weights", 100, 0., 1. ) ;
  fHistMuXY  = new TH2D(  "HistMuXY", "Mu production Vertex XY", 300, -150., 150.,300, -150., 150.) ;
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
  fHistParentDecayLength   = new TH1D(  "HistMuParentDecayLength", "Mu Parent Decay Length",   630, 0., 6300. ) ;
  fHistOtherDecayLength   = new TH1D(  "HistOtherDecayLength", "Other Decay Length",   600, 0., 6000. ) ;
  fHistMuParent = new TH1D(  "HistMuParent", "Mu Parent ifd",   1500, 0., 1500. ) ;
  fHistMuParentStatus   = new TH1D(  "HistMuParentStatus", "Mu Parent Status",   15, 0., 15 ) ;
  fHistIsub   = new TH1D(  "HistIsub", "Process producing muons",   90, 10., 100. ) ;
  tmuon = new TTree ("Muon","Highest Muon Pt Generator Info");
  tmuon->Branch("eta",&etamu,"etamu/D");
  tmuon->Branch("phi",&phimu,"phimu/D");
  tmuon->Branch("px",&pxmu,"pxmu/D");
  tmuon->Branch("py",&pymu,"pymu/D");
  tmuon->Branch("pz",&pzmu,"pzmu/D");
  tmuon->Branch("pt",&ptmu,"ptmu/D");
  tmuon->Branch("vx",&vxmu,"vxmu/D");
  tmuon->Branch("vy",&vymu,"vymu/D");
  tmuon->Branch("vz",&vzmu,"vzmu/D");
  tmuon->Branch("wt",&wtmu,"wtu/D");
  tmuon->Branch("parent",&pid,"pid/I");
  tmuon->Branch("decaylength",&decayl,"decayl/D");
  tmuon->Branch("isub",&isub,"isub/I");
 return ;
}
 
void MuAnalyzer::analyze( const Event& e, const EventSetup& )
{
      
  Handle< HepMCProduct > EvtHandle ;
  e.getByLabel( "source", EvtHandle ) ;
  //int eventsinrun=0; 
  const HepMC::GenEvent* Evt = EvtHandle->GetEvent() ;
  /*
  bool newrun = true;   
  int runnumber; */
  if (Evt != 0 ) {   
  /*  if ( newrun ) {
      runnumber=e.runID();
      newrun=false;
    } else if ( runnumber !=e.runID()) {
      cout<<"Events in Run "<<runnumber<<" was "<<eventsinrun<<endl;
      runnumber=e.runID();
    }
  */ 
 //cout << " Starting a new event" <<endl;
  int nmuon=0;
  double ptmax=ptmin;
  double wt=1.;
 //  ++eventsinrun;
  if ( Evt->weights().size() > 0 ) wt=Evt->weights()[0];
  for ( HepMC::GenEvent::vertex_const_iterator
	  vit=Evt->vertices_begin(); vit!=Evt->vertices_end(); ++vit )
    {
      for ( HepMC::GenVertex::particles_out_const_iterator
	      pout=(*vit)->particles_out_const_begin();
	    pout!=(*vit)->particles_out_const_end(); ++pout )
	{
	  double pt=(*pout)->momentum().perp();
	  double p=(*pout)->momentum().rho();
	  int id=(*pout)->pdg_id();
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
	  HepLorentzVector muprodv=(*vit)->position();
	  if ((abs(id) == 13 && pt>ptmin && wtype != 1)||(wtype==1 && wtype1ok)) 
	    {	
		++nmuon;
	      if ( pt > ptmax && eta<maxeta) ptmax=pt;
	      //	      cout << "Muon Pt="<<pt<< " Evt weight="<<wt<<endl;
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
	      fHistMuEta->Fill((*pout)->momentum().eta(),wt);
	      fHistMuPhi->Fill(phi,wt);
	      fHistPtMu->Fill(pt,wt ) ;
              fHistMuXY->Fill(muprodv.x(),muprodv.y());
              fHistMuZ->Fill(muprodv.z(),wt);
	      vector<HepMC::GenParticle*> MuonParents = (*vit)->listParents() ;      
	      //cout << " Number of Muon (immediate) parents = " << MuonParents.size() << endl ;
	      for (unsigned int ic=0; ic<MuonParents.size(); ic++ )
		{
		  pid=MuonParents[ic]->pdg_id();
		  //MuonParents[ic]->print() ;
		  fHistMuParent->Fill(abs(MuonParents[ic]->pdg_id()));
		  fHistMuParentStatus->Fill(MuonParents[ic]->status());
		    if (MuonParents[ic]->production_vertex() != 0 ) {
		    HepLorentzVector parentprodv=MuonParents[ic]->production_vertex()->position();
		    double decaylength=sqrt((muprodv.x()-parentprodv.x())*(muprodv.x()-parentprodv.x())
					  +(muprodv.y()-parentprodv.y())*(muprodv.y()-parentprodv.y())
					  +(muprodv.z()-parentprodv.z())*(muprodv.z()-parentprodv.z()));
		    decayl=decaylength;
		    //cout << "Decay Length ="<<decaylength<<endl;
		    fHistParentDecayLength->Fill(decaylength);
		  }else cout << " Could not find muon parent production vertex "<<endl;
		}
	    } else if (pt > ptmin){
	      //cout << " Dealing with another particle than a muon"<<endl;
	      vector<HepMC::GenParticle*> Parents = (*vit)->listParents() ;      
	      if (Parents.size() == 1 && Parents[0]->status() == 2 ) 
		{
		  if ( Parents[0]->production_vertex() != NULL ){
		    //Parents[0]->print() ;
		  HepLorentzVector parentprodv=Parents[0]->production_vertex()->position();
		  double decaylength=sqrt((muprodv.x()-parentprodv.x())*(muprodv.x()-parentprodv.x())
					  +(muprodv.y()-parentprodv.y())*(muprodv.y()-parentprodv.y())
					  +(muprodv.z()-parentprodv.z())*(muprodv.z()-parentprodv.z()));
		  fHistOtherDecayLength->Fill(decaylength);
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
      isub=Evt->signal_process_id();
      fHistIsub->Fill(Evt->signal_process_id()) ;
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
	int apid=abs(pid);
	if ( apid==5|| (apid>500 && apid<600) )fHistRate34B->Fill(pp,ww*5);
	else if ( apid==4 || (apid>400 && apid<500) )fHistRate34C->Fill(pp,ww*5);
	else if ( apid==15 )fHistRate34tau->Fill(pp,ww*5);
	else if ( apid==311 || apid==321 || apid==130 || apid==310 )fHistRate34kaon->Fill(pp,ww*5);
	else if ( apid==23 ) fHistRate34Z->Fill(pp,ww*5);
	else if ( apid==24 ) fHistRate34W->Fill(pp,ww*5);
	else if ( apid==211 || apid==221) fHistRate34pion->Fill(pp,ww*5);
      }
    }
  cout<<" There is are "<<nmuon<<" muons in this event ! " << endl ;
  fHistMuweight->Fill(wt);
  cout << " About to fill nmu"<<endl;
  fHistNMu->Fill(nmuon,wt);
  wtmu=wt;
  tmuon->Fill();
  cout << "Event finished"<<endl;
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
