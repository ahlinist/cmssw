
{

   gSystem->Load("libFWCoreFWLite.so") ;
   AutoLibraryLoader::enable() ;
   
   TFile* f = new TFile("pythiaHZZ4mu.root") ;
   
   TTree* tevt = f->Get("Events") ;
   
   int nevt = tevt->GetEntries() ;
   std::cout << "Number of Events = " << nevt << std::endl ;
   
   edm::HepMCProduct EvtProd ;
   TBranch* bhepmc =
      tevt->GetBranch( "edmHepMCProduct_source__PROD.obj") ;

   bhepmc->SetAddress( & EvtProd ) ;
   
   HepMC::GenEvent*    Evt = 0 ;
   HepMC::GenVertex*   Vtx = 0 ;
   HepMC::GenParticle* Part = 0 ;
   int                 NVtx = 0 ;
   int                 NPrt = 0 ;
   
   HepMC::GenVertex*   HiggsDecVtx = 0 ;
   
   HepMC::GenEvent::particle_iterator pit ;
   HepMC::GenEvent::vertex_iterator vit ;
   HepMC::GenVertex::particle_iterator vpit ;
      
   // because I hate using std::map and there has been NO dictionary 
   // for std::vector<HepMC::GenParticle*> untill tag V00-07-00 of the
   // SimDataFormats/HepMCProduct, I'm doing a dirty trick of storing
   // momenta in one container (std::vector) and the PDG's (for checking 
   // charge) in another (TArrayI)
   // I'll fix it later   
   //
   std::vector<CLHEP::HepLorentzVector> StableMuMom ;
   //
   // I'm reserving it for 20 int's... I wish it was automatic
   //
   TArrayI StablePDG(20) ;
   
   TH1D* Hist2muMass = new TH1D( "Hist2muMass", "test 2-mu mass", 100,  60., 120. ) ;
   TH1D* Hist4muMass = new TH1D( "Hist4muMass", "test 4-mu mass", 100, 170., 210. ) ;
   
//   HepMC::GenVertex::particles_in_const_iterator pin ;
//   HepMC::GenVertex::particles_out_const_iterator pout ;
//   std::set<HepMC::GenParticle*>::const_iterator pin ;
//   std::set<HepMC::GenParticle*>::const_iterator pout ;

   for ( int iev=0; iev<nevt; iev++ )
   {

      bhepmc->GetEntry(iev) ;
      
      Evt = EvtProd->GetEvent() ;

      if ( iev == 0 ) Evt->print() ;

/* 
      for (vit=Evt->vertices_begin(); vit!=Evt->vertices_end(); vit++ )
      {
         for ( vpit=(*vit)->particles_begin(HepMC::children);
	       vpit!=(*vit)->particles_end(HepMC::children); vpit++ )
	 {
	    Part = (*vpit) ;
	    cout << " PDG ID = " << Part->pdg_code() << endl ;
	 }
      }
*/       
      HiggsDecVtx = 0 ;
      StableMuMom.clear() ;
      for ( pit=Evt->particles_begin(); pit!=Evt->particles_end(); pit++)
      {
	 Part = (*pit) ;
	 if ( Part->pdg_id() == 25 )
	 {
	    HiggsDecVtx = Part->end_vertex() ;
	    if ( HiggsDecVtx != 0 )
	    {
	       cout << " Found Higgs with valid decay vertex : " 
	               << HiggsDecVtx->barcode() 
		       << " in event " << iev << endl ;
	       break ;
	    }
	 }
      }

      if ( HiggsDecVtx == 0 )
      {
         cout << "There is NO Higgs in the event " << iev << endl ;
      }
      else
      {
         for ( pit=Evt->particles_begin(); pit!=Evt->particles_end(); pit++)
	 {
	    Part = (*pit) ;
	    if ( abs(Part->pdg_id()) == 13 && Part->status() == 1 )
	    {
	       // here's the "dirty trick" itself, storing info
	       // in 2 differnt containers...
	       //
	       StableMuMom.push_back( Part->momentum() ) ;
	       StablePDG[StableMuMom.size()-1] = Part->pdg_id() ;
	    } 
	 }
      }
      
      cout << " Found " << StableMuMom.size() << " stable muons" << endl ;
      
      // these two need to be inside the event loop
      // - or never cleaned properly
      //
      CLHEP::HepLorentzVector Mom2mu ;
      CLHEP::HepLorentzVector Mom4mu ;

      for ( unsigned int i=0; i<StableMuMom.size(); i++ )
      {
	 for ( unsigned int j=i+1; j<StableMuMom.size(); j++ )
	 {
	    if ( StablePDG.At(i)*StablePDG.At(j) > 0 ) continue ; // skip same charge pairs
	    Mom2mu = StableMuMom[i] + StableMuMom[j];
	    // double XMass2mu = Mom2mu.m() ;
	    cout << " 2-mu inv.mass = " << Mom2mu.m() << endl ;
	    Hist2muMass->Fill( Mom2mu.m() ) ;
	 }
      } 
      
      if ( StableMuMom.size() == 4 )
      {
         for ( unsigned int i=0; i<4; i++ )
	 {
	    Mom4mu += StableMuMom[i] ;
	 }
	 Hist4muMass->Fill( Mom4mu.m() ) ;
      }
      cout << " 4-mu inv.mass = " << Mom4mu.m() << endl ;                    
   }
   
   TCanvas* cnv = new TCanvas("cnv") ;
   
   cnv->Divide(2,1) ;
   
   cnv->cd(1) ;
   Hist2muMass->Draw() ;
   cnv->cd(2) ;
   Hist4muMass->Draw() ;
   
   
}
