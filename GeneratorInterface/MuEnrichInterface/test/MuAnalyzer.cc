#include <iostream>

#include "GeneratorInterface/MuEnrichInterface/test/MuAnalyzer.h"
 
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
 
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Handle.h"
 
#include "TFile.h"
#include "TH1.h"
 
#include "FWCore/Framework/interface/MakerMacros.h"
 
using namespace edm;
using namespace std;

 
MuAnalyzer::MuAnalyzer( const ParameterSet& pset ): 
  fOutputFileName( pset.getUntrackedParameter<string>("HistOutFile",std::string("TestMuProducer.root")) ),
  maxeta(pset.getUntrackedParameter<double>("maxeta", 2.5)),
  ptmin(pset.getUntrackedParameter<double>("ptmin",1.8)),
  intlum(pset.getUntrackedParameter<double>("intlum",4.5e-4)),
  fOutputFile(0), fHistPtMu(0), fHistMuweight(0), fHistMuParent(0),fHistMuEta(0),fHistRate33(0),fHistRate34(0),fHistParentDecayLength(0),fHistOtherDecayLength(0)
{
}

void MuAnalyzer::beginJob( const EventSetup& )
{
  fOutputFile   = new TFile( fOutputFileName.c_str(), "RECREATE" ) ;
  fHistPtMu  = new TH1D(  "HistPtMu", "Muon transverse momentum", 120,  0., 30. ) ;
  fHistMuweight  = new TH1D(  "HistMuweight", "Mu event weights", 100, 0., 1. ) ;
  fHistMuParent   = new TH1D(  "HistMuParent", "Mu Parent ID",   600, 0., 600. ) ;     
  fHistMuEta   = new TH1D(  "HistMuEta", "Eta of muon",   100, -5., 5. ) ;
  fHistRate33   = new TH1D(  "HistRate33", "Rate at Low lumi",   100, 0., 80. ) ;
  fHistRate34   = new TH1D(  "HistRate34", "Rate at High lumi",   100, 0., 80. ) ;
  fHistParentDecayLength   = new TH1D(  "HistMuParentDecayLength", "Mu Parent Decay Length",   630, 0., 6300. ) ;
  fHistOtherDecayLength   = new TH1D(  "HistOtherDecayLength", "Other Decay Length",   600, 0., 6000. ) ;
  fHistMuParentStatus   = new TH1D(  "HistMuParentStatus", "Mu Parent Status",   15, 0., 15 ) ;
  fHistIsub   = new TH1D(  "HistIsub", "Process producing muons",   90, 10., 100. ) ;

  return ;
}
 
void MuAnalyzer::analyze( const Event& e, const EventSetup& )
{
      
  Handle< HepMCProduct > EvtHandle ;
   
  e.getByLabel( "source", EvtHandle ) ;
   
  const HepMC::GenEvent* Evt = EvtHandle->GetEvent() ;

  if (Evt != 0 ) {   
  //cout << " Starting a new event" <<endl;
  int nmuon=0;
  double ptmax=-99;
  double wt=1.;

  if ( Evt->weights().size() > 0 ) wt=Evt->weights()[0];
  for ( HepMC::GenEvent::vertex_const_iterator
	  vit=Evt->vertices_begin(); vit!=Evt->vertices_end(); ++vit )
    {
      for ( HepMC::GenVertex::particles_out_const_iterator
	      pout=(*vit)->particles_out_const_begin();
	    pout!=(*vit)->particles_out_const_end(); ++pout )
	{
	  double pt=(*pout)->momentum().perp();
	  int id=(*pout)->pdg_id();
	  double eta = abs((*pout)->momentum().eta());
	  HepLorentzVector muprodv=(*vit)->position();
	  if ( abs(id) == 13 && pt>ptmin ) 
	    {	
	      ++nmuon;
	      if ( pt > ptmax && eta<maxeta) ptmax=pt;
	      cout << "Muon Pt="<<pt<< " Evt weight="<<wt<<endl;
	      fHistMuEta->Fill((*pout)->momentum().eta(),wt);
	      fHistPtMu->Fill(pt,wt ) ;
	      vector<HepMC::GenParticle*> MuonParents = (*vit)->listParents() ;      
	      //cout << " Number of Muon (immediate) parents = " << MuonParents.size() << endl ;
	      for (unsigned int ic=0; ic<MuonParents.size(); ic++ )
		{
		  MuonParents[ic]->print() ;
		  fHistMuParent->Fill(MuonParents[ic]->pdg_id());
		  fHistMuParentStatus->Fill(MuonParents[ic]->status());
		  HepLorentzVector parentprodv=MuonParents[ic]->production_vertex()->position();
		  double decaylength=sqrt((muprodv.x()-parentprodv.x())*(muprodv.x()-parentprodv.x())
					  +(muprodv.y()-parentprodv.y())*(muprodv.y()-parentprodv.y())
					  +(muprodv.z()-parentprodv.z())*(muprodv.z()-parentprodv.z()));
		  //cout << "Decay Length ="<<decaylength<<endl;
		  fHistParentDecayLength->Fill(decaylength);
					  
		}
	    } else if (pt > ptmin){
	      // cout << " Dealing with another particle than a muon"<<endl;
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
      //cout << " There is NO Muon in this event ! " << endl ;
      return ;
    } else {
      fHistIsub->Fill(Evt->signal_process_id()) ;
      int imax;
      imax=(int) (ptmax/0.8);
      for (int i=0 ;i<imax ; ++i ) {
	double pp;
	double ww;
	pp=0.4+i*0.8;
	ww=2*(ww)/intlum;
	fHistRate33->Fill(pp,ww);
	fHistRate34->Fill(pp,ww*5);
      }
    }
  //cout<<" There is are "<<nmuon<<" muons in this event ! " << endl ;
  fHistMuweight->Fill(wt);
  return ;
  }
}

void MuAnalyzer::endJob()
{
       
fOutputFile->Write() ;
fOutputFile->Close() ;
   
return ;
}
 
DEFINE_FWK_MODULE(MuAnalyzer)
