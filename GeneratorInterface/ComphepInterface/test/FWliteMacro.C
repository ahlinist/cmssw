{
// Load and enable FWLite in Root

gSystem->Load("libFWCoreFWLite.so"); 
AutoLibraryLoader::enable();

//Open input file 
//TFile file("/tmp/sznajder/mc-physval-130_pre3-SingleMuMinus-Pt50_IdealGeom.root");
//TFile file("/tmp/sznajder/mc-physval-130_pre3-SingleMuMinus-Pt50_ShortTermScenario.root");
TFile file("/tmp/sznajder/mc-physval-130_pre3-SingleMuMinus-Pt50_LongTermScenario.root");

// Open output file
//TFile histofile("reco_resol_50GEV_idealgeom.root","RECREATE");
//TFile histofile("reco_resol_50GEV_shortterm.root","RECREATE");
TFile histofile("reco_resol_50GEV_longterm.root","RECREATE");

// Set generation charge and Pt
 float pt_gen = 50. ;
 int q_gen = -1 ;

  

// book histograms

  TH1F* hDPtDCA_sta = new TH1F("Pt_dca_sta","P_{t} DCA - Ideal Geometry - STA",200,400,600);
  TH1F* hDPtDCA_glb = new TH1F("Pt_dca_glb","P_{t} DCA - Ideal Geometry - GLB",200,400,600);

  TH1F* h1oPt_resol_sta = new TH1F("Pt_resol_sta","1/P_{t} Resolution DCA - Ideal Geometry - STA",100,-1,1);
  TH1F* h1oPt_resol_glb = new TH1F("Pt_resol_glb","1/P_{t} Resolution DCA - Ideal Geometry - GLB",100,-1,1);

  TH2F* h2_eta_vs_resol_glb = new TH2F("eta_vs_resol_glb","Eta vs 1/P_{t} Resolution - Ideal Geometry",50,-2.5,2.5,100,-1,1);


//  TH1F* hDPtIn_sta = new TH1F("PtIn_sta","P_{t}^{Track} inner state - Ideal Geometry - STA",200,0,100);
//  TH1F* hDPtIn_glg = new TH1F("PtIn_glb","P_{t}^{Track} inner state - Ideal Geometry - GLB",200,0,100);

//  TH1F* hDPtOut_sta = new TH1F("PtOut_sta","P_{t}^{Track} outer state - Ideal Geometry - STA",200,0,100);
//  TH1F* hDPtOut_glb = new TH1F("PtOut_glb","P_{t}^{Track} outer state - Ideal Geometry - GLB",200,0,100);

  // Get the Tree
  TTree *tree = (TTree*)file.Get("Events");

  std::vector<reco::Track> trackCollection;
 
  // Get branch of muon reco tracks 
//  TBranch *branch_sta = tree->GetBranch("recoTracks_standAloneMuons__T.obj");
  TBranch *branch_sta = tree->GetBranch("recoTracks_standAloneMuons__RP.obj");
//  TBranch *branch_glb = tree->GetBranch("recoTracks_globalMuons__T.obj");
  TBranch *branch_glb = tree->GetBranch("recoTracks_globalMuons__RP.obj");

/*
  // Fill STA muon info
   
  // Loop over events 
  branch_sta->SetAddress(&trackCollection);
  for ( unsigned int index = 0; index < tree->GetEntries(); ++index ) {

    // Get the event
//    std::cout << "event: " << index << std::endl;
    branch_sta->GetEntry(index);
//    std::cout << "content: " << trackCollection.size() << std::endl;

    double pt_inner=0.0, pt_outer=0.0;
    // Loop over tracks in the event
    for ( unsigned int bindex = 0; bindex < trackCollection.size(); ++bindex ) {
//      std::cout << "track: " << bindex << std::endl;
      
      // Get track pointer
      reco::Track* track = (reco::Track*)trackCollection[bindex];
      
      // Get pt
      double pt = track->pt();

      // Fill histos 
      hDPtDCA_sta->Fill(pt_inner );
      h1oPt_resol_sta->Fill((1./pt-1./100.)/(1./100.));
    }
  }//end evt loop

*/


  // Fill GLB muon info
   
  // Loop over events 
  branch_glb->SetAddress(&trackCollection);
  for ( unsigned int index = 0; index < tree->GetEntries(); ++index ) {

    // Get the event
//    std::cout << "event: " << index << std::endl;
    branch_glb->GetEntry(index);
//    std::cout << "content: " << trackCollection.size() << std::endl;

    // Loop over tracks in the event
    for ( unsigned int bindex = 0; bindex < trackCollection.size(); ++bindex ) {
//      std::cout << "track: " << bindex << std::endl;
      
      // Get track pointer
      reco::Track* track = (reco::Track*)trackCollection[bindex];
      
      // Get pt
      double pt = track->pt();

      // Get eta
      double eta = track->eta();

      // Get eta
      double q = track->charge();

      // q/pt sigma
      float sigma = ( q/pt - q_gen/pt_gen ) / ( q_gen/pt_gen ); 


      // Fill histos 
      hDPtDCA_glb->Fill(pt);
      h1oPt_resol_glb->Fill(sigma);
      h2_eta_vs_resol_glb->Fill(eta, sigma);

    }
  }//end evt loop


  // Fit resol histo
  h2_eta_vs_resol_glb->FitSlicesY();
  
  //Get histos from fit 
//  TH1D *h2_eta_vs_resol_glb_1 = (TH1D*)gDirectory->Get("h2_eta_vs_resol_glb_1");
//  TH1D *h2_eta_vs_resol_glb_2 = (TH1D*)gDirectory->Get("h2_eta_vs_resol_glb_2");
  

  // save histograms
  histofile.Write();
  histofile.Close();
  
}

