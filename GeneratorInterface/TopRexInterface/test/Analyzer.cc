 

#include <iostream>

#include "GeneratorInterface/TopRexInterface/test/Analyzer.h"
  
 
#include "DataFormats/Common/interface/Ref.h" 
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetfwd.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Handle.h"
 
#include "TFile.h"
#include "TH1.h"
 
#include "FWCore/Framework/interface/MakerMacros.h"
 
using namespace edm;
using namespace std;
using namespace reco;
 
//  Cell cellJet;


class JetSort{
public:
  bool operator()(const GenJet& a, const GenJet& b) {
    return a.pt() > b.pt();
  }
};



Analyzer::Analyzer( const ParameterSet& pset )
   : fOutputFileName( pset.getUntrackedParameter<string>("HistOutFile", 
        std::string("Myanalysis.root")) ),
      fOutputFile(0), 
     P_J_E(0), P_J_PT(0), P_J_ETA(0), P_J_NN(0), 
     C_J_E(0), C_J_PT(0), C_J_ETA(0), C_J_NN(0)
 
{
}

void Analyzer::beginJob( const EventSetup& )
{
   fOutputFile   = new TFile( fOutputFileName.c_str(), "RECREATE" ) ;
   H_P_E[0]  = new TH1D("E 0",  "Energy PI0", 100,  0., 100. ) ;
   H_P_E[1]  = new TH1D("E 1",  "Energy PI+", 100,  0., 100. ) ;
   H_P_E[2]  = new TH1D("E 2",  "Energy PI-", 100,  0., 100. ) ;
   H_P_E[3]  = new TH1D("E 3",  "Energy K0", 100,  0., 100. ) ;
   H_P_E[4]  = new TH1D("E 4",  "Energy K+", 100,  0., 100. ) ;
   H_P_E[5]  = new TH1D("E 5",  "Energy K-", 100,  0., 100. ) ;
   H_P_E[6]  = new TH1D("E 6",  "Energy Gam", 100,  0., 100. ) ;
   H_P_E[7]  = new TH1D("E 7",  "Energy E+", 100,  0., 100. ) ;
   H_P_E[8]  = new TH1D("E 8",  "Energy E-", 100,  0., 100. ) ;
   H_P_E[9]  = new TH1D("E 9",  "Energy MU+", 100,  0., 100. ) ;
   H_P_E[10] = new TH1D("E 10", "Energy MU-", 100,  0., 100. ) ;

   H_P_PT[0]  = new TH1D("PT 0",  "Pt PI0", 100,  0., 20. ) ;
   H_P_PT[1]  = new TH1D("PT 1",  "Pt PI+", 100,  0., 20. ) ;
   H_P_PT[2]  = new TH1D("PT 2",  "Pt PI-", 100,  0., 20. ) ;
   H_P_PT[3]  = new TH1D("PT 3",  "Pt K0", 100,  0., 20. ) ;
   H_P_PT[4]  = new TH1D("PT 4",  "Pt K+", 100,  0., 20. ) ;
   H_P_PT[5]  = new TH1D("PT 5",  "Pt K-", 100,  0., 20. ) ;
   H_P_PT[6]  = new TH1D("PT 6",  "Pt Gam", 100,  0., 20. ) ;
   H_P_PT[7]  = new TH1D("PT 7",  "Pt E+", 100,  0., 20. ) ;
   H_P_PT[8]  = new TH1D("PT 8",  "Pt E-", 100,  0., 20. ) ;
   H_P_PT[9]  = new TH1D("PT 9",  "Pt MU+", 100,  0., 20. ) ;
   H_P_PT[10] = new TH1D("PT 10", "Pt MU-", 100,  0., 20. ) ;

   H_P_ETA[0]  = new TH1D("ETA 0",  "eta PI0", 100,  -7., 7. ) ;
   H_P_ETA[1]  = new TH1D("ETA 1",  "eta PI+", 100,  -7., 7. ) ;
   H_P_ETA[2]  = new TH1D("ETA 2",  "eta PI-", 100,  -7., 7. ) ;
   H_P_ETA[3]  = new TH1D("ETA 3",  "eta K0",  100,  -7., 7. ) ;
   H_P_ETA[4]  = new TH1D("ETA 4",  "eta K+",  100,  -7., 7. ) ;
   H_P_ETA[5]  = new TH1D("ETA 5",  "eta K-",  100,  -7., 7. ) ;
   H_P_ETA[6]  = new TH1D("ETA 6",  "eta Gam", 100,  -7., 7. ) ;
   H_P_ETA[7]  = new TH1D("ETA 7",  "eta E+",  100,  -7., 7. ) ;
   H_P_ETA[8]  = new TH1D("ETA 8",  "eta E-",  100,  -7., 7. ) ;
   H_P_ETA[9]  = new TH1D("ETA 9",  "eta MU+", 100,  -7., 7. ) ;
   H_P_ETA[10] = new TH1D("ETA 10", "eta MU-", 100,  -7., 7. ) ;

   H_P_PHI[0]  = new TH1D("PHI 0",  "phi PI0", 100,  -4., 4. ) ;
   H_P_PHI[1]  = new TH1D("PHI 1",  "phi PI+", 100,  -4., 4. ) ;
   H_P_PHI[2]  = new TH1D("PHI 2",  "phi PI-", 100,  -4., 4. ) ;
   H_P_PHI[3]  = new TH1D("PHI 3",  "phi K0",  100,  -4., 4. ) ;
   H_P_PHI[4]  = new TH1D("PHI 4",  "phi K+",  100,  -4., 4. ) ;
   H_P_PHI[5]  = new TH1D("PHI 5",  "phi K-",  100,  -4., 4. ) ;
   H_P_PHI[6]  = new TH1D("PHI 6",  "phi Gam", 100,  -4., 4. ) ;
   H_P_PHI[7]  = new TH1D("PHI 7",  "phi E+",  100,  -4., 4. ) ;
   H_P_PHI[8]  = new TH1D("PHI 8",  "phi E-",  100,  -4., 4. ) ;
   H_P_PHI[9]  = new TH1D("PHI 9",  "phi MU+", 100,  -4., 4. ) ;
   H_P_PHI[10] = new TH1D("PHI 10", "phi MU-", 100,  -4., 4. ) ;

   H_P_NN[0]  = new TH1D("MULT PI0",  "mult PI0", 100,  0., 50. ) ;
   H_P_NN[1]  = new TH1D("MULT PI+",  "mult PI+", 100,  0., 50. ) ;
   H_P_NN[2]  = new TH1D("MULT PI-",  "mult PI-", 100,  0., 50. ) ;
   H_P_NN[3]  = new TH1D("MULT K0",   "mult K0",  100,  0., 50. ) ;
   H_P_NN[4]  = new TH1D("MULT K+",   "mult K+",  100,  0., 50. ) ;
   H_P_NN[5]  = new TH1D("MULT K-",   "mult K-",  100,  0., 50. ) ;
   H_P_NN[6]  = new TH1D("MULT Gam",  "mult Gam", 100,  0., 50. ) ;
   H_P_NN[7]  = new TH1D("MULT E+",   "mult E+",  100,  0., 50. ) ;
   H_P_NN[8]  = new TH1D("MULT E-",   "mult E-",  100,  0., 50. ) ;
   H_P_NN[9]  = new TH1D("MULT MU+",  "mult MU+", 100,  0., 50. ) ;
   H_P_NN[10] = new TH1D("MULT MU-",  "mult MU-", 100,  0., 50. ) ;
   //
   P_J_E   = new TH1D("P JET E",   "P jet E", 100,  0., 1000. ) ;
   P_J_PT  = new TH1D("P JET PT",  "Pjet PT", 100,  0., 500. ) ;
   P_J_ETA = new TH1D("P JET ETA",  "P jet ETA", 100,  -7., 7. ) ;
   P_J_M   = new TH1D("P JET M",  "P jet M", 100,  0., 200. ) ;
   P_J_NN  = new TH1D("P JET MULT",  "P jet MULT", 100,  0., 30. ) ;

   C_J_E  = new TH1D("C JET E",   "C jet E", 100,  0., 1000. ) ;
   C_J_PT = new TH1D("C JET PT",  "C jet PT", 100,  0., 500. ) ;
   C_J_ETA = new TH1D("C JET ETA",  "C jet ETA", 100,  -7., 7. ) ;
   C_J_M   = new TH1D("C JET M",  "C jet M", 100,  0., 200. ) ;
   C_J_NN = new TH1D("C JET MULT",  "C jet MULT", 100,  0., 30. ) ;

   //

   return ;
}
 
void Analyzer::analyze( const Event& iEvent, const EventSetup& )
{
   Handle< HepMCProduct > EvtHandle ;
   iEvent.getByLabel( "source", EvtHandle ) ;


    const  HepMC::GenEvent* myGenEvent = EvtHandle->GetEvent();

    double pp_e, pp_pt, pp_eta, pp_phi;
    int MULT[11], npp;

    for(int i=0; i<11; ++i) {
      MULT[i] = 0; 
    }

      int km = 0;
      //      cout << "******************** PYTHIA JETS " << endl;
        
   for ( HepMC::GenEvent::particle_const_iterator p = myGenEvent->particles_begin();
	  p != myGenEvent->particles_end(); ++p ) {
      
       if ((*p)->pdg_id()==98 ) 
     { 
        pp_e   = (*p)->momentum().e();
        pp_pt  = (*p)->momentum().perp();
        pp_eta = (*p)->momentum().eta(); 
	if((*p)->momentum().perp() > 20.) {
        double ppx, ppy, ppz, pm;
        ppx = (*p)->momentum().px();
        ppy = (*p)->momentum().py();
        ppz = (*p)->momentum().pz();
        pm = pp_e*pp_e - ppx*ppx - ppy*ppy - ppz*ppz;
        if(pm>0.)
          {pm = sqrt(pm);}
        else
          {pm=0;}
	km = km + 1;
      P_J_E->Fill(pp_e);
      P_J_PT->Fill(pp_pt);
      P_J_M->Fill(pm);
     }
    }


         if ((*p)->status()==1 ) 
     { 
        pp_e   = (*p)->momentum().e();
        pp_pt  = (*p)->momentum().perp();
        pp_eta = (*p)->momentum().eta(); 
        pp_phi = (*p)->momentum().phi();

        npp = -1;
	if ((*p)->pdg_id()==111 )  npp = 0; 
	if ((*p)->pdg_id()==211 )  npp = 1;
	if ((*p)->pdg_id()==-211 ) npp = 2; 
        if (abs( (*p)->pdg_id()) ==311 ) npp = 3; 
        if ((*p)->pdg_id()==321 )  npp = 4; 
        if ((*p)->pdg_id()==-321 ) npp = 5; 
        if ((*p)->pdg_id()==22 )   npp = 6; 
        if ((*p)->pdg_id()==-11 )  npp = 7; 
	if ((*p)->pdg_id()==11 )   npp = 8; 
	if ((*p)->pdg_id()==-13 )  npp = 9; 
	if ((*p)->pdg_id()==13 )  npp = 10; 

        if(npp>=0) {
            HistPart( npp, pp_e, pp_pt, pp_eta, pp_phi);
            MULT[npp] = MULT[npp] + 1;
	}             
   }
}
    for(int i=0; i<11; ++i) 
    {
     H_P_NN[i]->Fill(MULT[i]);
    }
     P_J_NN->Fill(km);

    /* analysis of jets */

   Handle< GenJetCollection > JetsHandle ;
   iEvent.getByLabel( "iterativeCone5GenJets", JetsHandle );
   //  if(JetsHandle->size())

    std::vector<GenJet> tmpJet;
    tmpJet.clear();

   if(JetsHandle->size())
    { 
       for(GenJetCollection::const_iterator it=JetsHandle->begin(); 
	   it!=JetsHandle->end();it++) //{
      tmpJet.push_back(*it);
     std::stable_sort(tmpJet.begin(),tmpJet.end(),JetSort());
    }
   int nj = 0; 
   for ( unsigned int jetIndex = 0; jetIndex < tmpJet.size(); ++jetIndex ) {
      GenJet pJ = tmpJet[jetIndex];
     if(pJ.pt() > 20.) {
       nj += 1;
        double ppx, ppy, ppz, ppe, pm;
        ppe = pJ.energy();
        ppx = pJ.px();
        ppy = pJ.py();
        ppz = pJ.pz();
        pm = ppe*ppe - ppx*ppx - ppy*ppy - ppz*ppz;
         if(pm>0.)
          {pm = sqrt(pm);}
         else
          {pm=0;}
     C_J_E->Fill(ppe);
      C_J_PT->Fill(pJ.pt());
      C_J_ETA->Fill(pJ.eta());
      C_J_M->Fill(pm);
     } 
   }

      C_J_NN->Fill(nj);

 }


/////////////////////
void Analyzer::endJob()
{
   fOutputFile->Write() ;
   fOutputFile->Close() ;
   return ;
}

void Analyzer::HistPart(int &i, double &e, double &pt, double &eta, double &phi)
{
   H_P_E[i]->Fill(e);
   H_P_PT[i]->Fill(pt);
   H_P_ETA[i]->Fill(eta);
   H_P_PHI[i]->Fill(phi);
   return ;
}


 
DEFINE_FWK_MODULE(Analyzer)
