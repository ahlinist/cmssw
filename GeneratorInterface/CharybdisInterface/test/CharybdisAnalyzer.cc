/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Date$
 * $Revision$
 * Example analyzer for Charybdis produced black hole events
 * Prints out information about BH decay products and the BH itself.
 * Based on the example analyzer of HydjetInterface
 * Adaptation by Halil Gamsizkan
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <iostream>
#include "assert.h"

#include "CharybdisAnalyzer.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Event.h"
#include "CLHEP/Vector/LorentzVector.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "TFile.h"
#include "TH1.h"
 
using namespace edm;
using namespace std;

// Constructor
CharybdisAnalyzer::CharybdisAnalyzer(const ParameterSet& pset)
{ ; }


void CharybdisAnalyzer::beginJob( const EventSetup& )
{
   edm::Service<TFileService> fs;

   h_eta1 = fs->make<TH1D>( "bh_etaPythia", "EtaPythia; #eta; Particles", 80,  -5.,  5.);
   h_phi = fs->make<TH1D>( "bh_phi", "Phi; #phi; Particles", 80,  -3.5,  3.5);
   h_pt = fs->make<TH1D>( "bh_pt", "P_{T}; P_{T} (GeV); Particles/30 GeV", 100,  0,  3000);
   h_invMass = fs->make<TH1D>( "bh_invMass", "Invariant Mass; M (GeV); Events/150 GeV", 100,  0.,  14000.);
   h_st = fs->make<TH1D>( "bh_SumET", "#Sigma E_{T}; #Sigma E_{T} (GeV); Events/80 GeV", 100,  0.,  8000.);
   cnt_events = 0;
}

//______________________________________________________________________
void 
CharybdisAnalyzer::analyze( const Event& e, const EventSetup& ){
   ++cnt_events;
   Handle<HepMCProduct> EvtHandle; 
   e.getByLabel( "generator", EvtHandle );
   
   const HepMC::GenEvent* myEvt = EvtHandle->GetEvent() ;
   unsigned int cnt_particles = 0;
   unsigned int cnt_bh_particles = 0;
   unsigned int in_size;
   unsigned int out_size;
   unsigned int the_out_size=0;
   bool is_bh_decay_product;
   double st=0., invmass=0.;
   CLHEP::HepLorentzVector BHp4(0,0,0,0);

   if(!myEvt){
      cout << "Whoops, received an invalid event pointer :/\n" << "Event " << cnt_events << " skipped\n";
      return;
   }
   cout << "----- Event " << cnt_events << " ---------------\n";
   for( HepMC::GenEvent::particle_const_iterator p = myEvt->particles_begin(); p != myEvt->particles_end(); p++ ){ 
      ++cnt_particles;
      if( cnt_particles < 20 ){ //(*p)->numberOfMothers() == 2 ){
	 const HepMC::GenVertex * myvertex = (*p)->production_vertex() ;
	 in_size = 0;
	 out_size = 0;
	 if (myvertex){
	    in_size = myvertex->particles_in_size();
	    out_size = myvertex->particles_out_size();
	 }
	 
	 is_bh_decay_product = (in_size == 2 && cnt_particles < 30 );
	 if (is_bh_decay_product) {
	    ++cnt_bh_particles;
	    the_out_size = out_size;
	    double eta = (*p)->momentum().eta(), 
		   phi = (*p)->momentum().phi(),
		   pt = (*p)->momentum().perp();

	    // Print information about BH decay products
	    cout << "#" << cnt_particles
		 << ": PDG ID=" << (*p)->pdg_id() << ", "
	         << "Eta=" << eta << ", "
	         << "Phi=" << phi << ", "
	         << "Pt=" << pt << ", "
	         << "ProdVxIn=" << in_size << ", "
	         << "ProdVxOut=" << out_size << endl;

	    CLHEP::HepLorentzVector partp4((*p)->momentum().px(),
					   (*p)->momentum().py(),
					   (*p)->momentum().pz(),
					   (*p)->momentum().e()   );
	    BHp4 += partp4;
	    st += pt;
	    h_eta1->Fill(eta);
	    h_phi->Fill(phi); 
	    h_pt->Fill(pt);
	 }
	 
      }
   } // Loop over particles in event ends
   assert(the_out_size == cnt_bh_particles); // Simple check for decay product tagging logic
   invmass=BHp4.m();
   h_st->Fill(st);
   h_invMass->Fill(invmass);
   //cout << "Event record had " << cnt_particles << " particles." << endl;
   cout << "BH Decayed to " << cnt_bh_particles << " particles, SumET=" << st << " GeV, BH mass=" << invmass << " GeV\n";
}

//_____________________________________________________________
void CharybdisAnalyzer::endJob()
{  ; }

//define as a plug-in
DEFINE_FWK_MODULE(CharybdisAnalyzer);
