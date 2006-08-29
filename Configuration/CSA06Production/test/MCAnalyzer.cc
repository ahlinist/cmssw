#include <iostream>

#include "Configuration/Generator/test/MCAnalyzer.h"
 
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "DataFormats/Common/interface/TriggerResults.h"
 
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Handle.h"
 
#include "TFile.h"
#include "TH1.h"
 
#include "FWCore/Framework/interface/MakerMacros.h"
 
using namespace edm;
using namespace std;

 
MCAnalyzer::MCAnalyzer( const ParameterSet& pset )
   : fOutputFileName( pset.getUntrackedParameter<string>("HistOutFile",std::string("MCAnalysis.root")) ),
     fOutputFile(0), fPtHat(0), fNPart(0), fNChPart(0), fProcessID(0),  fPartID(0),
     fHist2muMass(0), fHistmuPt(0), fHistmuEta(0), fHistmuPhi(0), Nevt(0), Nhlt(0), Nall(0)
{
}

void MCAnalyzer::beginJob( const EventSetup& )
{
 
   fOutputFile   = new TFile( fOutputFileName.c_str(), "RECREATE" ) ;
   fHist2muMass  = new TH1D(  "Hist2muMass", "Dimuon invariant mass", 100,  0., 150. ) ;
   fHistmuPt  = new TH1D ( "HistmuPt", "Muon Pt spectrum", 100,  0., 100.)  ;
   fHistmuEta  = new TH1D ( "HistmuEta", "Muon eta spectrum", 20,  -5., 5.)  ;
   fHistmuPhi  = new TH1D ( "HistmuPhi", "Muon phi spectrum", 20,  -3.2, 3.2)  ;
   fPtHat = new TH1D ( "HistPtHat", "Pt_hat", 100,  0, 100.)  ;
   fNPart = new TH1D ( "HistNPart", "Number of particles", 100,  0, 2000.)  ;
   fNChPart = new TH1D ( "HistNChPart", "Number of charged particles", 100,  0, 600.)  ;
   fProcessID = new TH1D ( "HistProcessID", "Process ID", 200,  0, 200.)  ;
   fPartID = new TH1D ( "HistPartID", "Particle ID", 800,  -400, 400.)  ;
}
 
void MCAnalyzer::analyze( const Event& e, const EventSetup& )
{
   
   // get hold of (single?) TriggerResults object
   vector<Handle<TriggerResults> > trhv;
   e.getManyByType(trhv);
   const unsigned int n(trhv.size());
   if (n>0) {
     bool hlt(false);
     if (Nevt==0) Nall.resize( (*(trhv[0])).size() );
     Nevt++;
     for (unsigned int i=0; i!= (*(trhv[0])).size(); i++) {
       if ( (*(trhv[0])).accept(i) ) {
	 Nall[i]++;
         if (i<12) hlt=true;
       }
     }
     if (hlt) Nhlt++;
   }



   // HEPMC analysis

   Handle< HepMCProduct > EvtHandle ;
   
   e.getByLabel( "source", EvtHandle ) ;
   
   const HepMC::GenEvent* Evt = EvtHandle->GetEvent() ;
  
   vector<HepMC::GenParticle*> StableDesc ;
   int chrgpart = 0;
   
   for ( HepMC::GenEvent::particle_const_iterator p = Evt->particles_begin();
  	  p != Evt->particles_end(); ++p ) {
 
 
    // find stable particles
    if ((*p)->status() == 1){
     
     // store their ID 
     fPartID->Fill( (*p)->pdg_id() );
     
     // find charged particles
     if ( abs(charge((*p)->pdg_id())) == 3  ){
       chrgpart++;
     }
     
    }
    
   // store stable muons with pt > 5 GeV
   //   
   
      if ( (*p)->status() == 1 && abs((*p)->pdg_id()) == 13 && (*p)->momentum().perp() > 5){
	     StableDesc.push_back(*p) ;
      }
 
 
   }
 
 
   // processID, pthat, number of particles, number of charged particles
 
   int processid = Evt->signal_process_id(); 
   cout << "Process ID = " << processid << endl;  
   fProcessID->Fill(processid);
   
   double pthat = Evt->event_scale();
   cout << "Pt_hat = " << pthat << endl;
   fPtHat->Fill(pthat);
   
   cout << "Total number of particles  = " << Evt->particles_size() << endl;
   fNPart->Fill(Evt->particles_size());
   
   cout << "Number of charged particles  = " << chrgpart << endl;
   fNChPart->Fill(chrgpart);
   
   
   
   
   HepLorentzVector Mom2part ;
  
        
   // fill muon properties and do 2-mu inv.mass
   //
   for ( unsigned int i=0; i<StableDesc.size(); i++ )
   {
       fHistmuPt->Fill(StableDesc[i]->momentum().perp());
       fHistmuEta->Fill(StableDesc[i]->momentum().eta());
       fHistmuPhi->Fill(StableDesc[i]->momentum().phi());
       
      for ( unsigned int j=i+1; j<StableDesc.size(); j++ )
      {
         
	 // select opposite charge combo's
	 //
	 if ( (StableDesc[i]->pdg_id()*StableDesc[j]->pdg_id()) < 0 ) {
	 
	 Mom2part = StableDesc[i]->momentum() + StableDesc[j]->momentum() ;	 
	 fHist2muMass->Fill( Mom2part.m() ) ;
	
	
	}
	
      }
   
   }

}



int MCAnalyzer::charge(const int& Id){
  
  //...Purpose: to give three times the charge for a particle/parton.

  //      ID     = particle ID
  //      hepchg = particle charge times 3

  int kqa,kq1,kq2,kq3,kqj,irt,kqx,kqn;
  int hepchg;


  int ichg[109]={-1,2,-1,2,-1,2,-1,2,0,0,-3,0,-3,0,-3,0,
		 -3,0,0,0,0,0,0,3,0,0,0,0,0,0,3,0,3,6,0,0,3,6,0,0,-1,2,-1,2,-1,2,0,0,0,0,
		 -3,0,-3,0,-3,0,0,0,0,0,-1,2,-1,2,-1,2,0,0,0,0,
		 -3,0,-3,0,-3,0,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


  //...Initial values. Simple case of direct readout.
  hepchg=0;
  kqa=abs(Id);
  kqn=kqa/1000000000%10;
  kqx=kqa/1000000%10;
  kq3=kqa/1000%10;
  kq2=kqa/100%10;
  kq1=kqa/10%10;
  kqj=kqa%10;
  irt=kqa%10000;

  //...illegal or ion
  //...set ion charge to zero - not enough information
  if(kqa==0 || kqa >= 10000000) {

    if(kqn==1) {hepchg=0;}
  }
  //... direct translation
  else if(kqa<=100) {hepchg = ichg[kqa-1];}
  //... KS and KL (and undefined)
  else if(kqj == 0) {hepchg = 0;}
  //C... direct translation
  else if(kqx>0 && irt<100)
    {
      hepchg = ichg[irt-1];
      if(kqa==1000017 || kqa==1000018) {hepchg = 0;}
      if(kqa==1000034 || kqa==1000052) {hepchg = 0;}
      if(kqa==1000053 || kqa==1000054) {hepchg = 0;}
      if(kqa==5100061 || kqa==5100062) {hepchg = 6;}
    }
  //...Construction from quark content for heavy meson, diquark, baryon.
  //...Mesons.
  else if(kq3==0)
    {
      hepchg = ichg[kq2-1]-ichg[kq1-1];
      //...Strange or beauty mesons.
      if((kq2==3) || (kq2==5)) {hepchg = ichg[kq1-1]-ichg[kq2-1];}
    }
  else if(kq1 == 0) {
    //...Diquarks.
    hepchg = ichg[kq3-1] + ichg[kq2-1];
  }

  else{
    //...Baryons
    hepchg = ichg[kq3-1]+ichg[kq2-1]+ichg[kq1-1];
  }

  //... fix sign of charge
  if(Id<0 && hepchg!=0) {hepchg = -1*hepchg;}

  // cout << hepchg<< endl;
  return hepchg;


}

void MCAnalyzer::endJob()
{
       
   fOutputFile->Write() ;
   fOutputFile->Close() ;

   cout << endl;
   cout << "Trigger analysis:" << endl;
   cout << " Total number of events analysed: " << Nevt << endl;
   cout << "  HLT triggers: first 12." << endl;
   cout << "  Generator selection (if present) and G4: rest." << endl;
   for (unsigned int i=0; i!=Nall.size(); i++) {
     cout << "   Trigger # " << i << " accepted: " << Nall[i] << endl;
   }
   cout << "  HLT Triggers accepted: " << Nhlt << endl;

}
 
DEFINE_FWK_MODULE(MCAnalyzer)
