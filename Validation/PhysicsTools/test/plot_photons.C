{
  
  gROOT->LoadMacro("../Tools/NicePlot.C");
  InitNicePlot();
  gROOT->LoadMacro("../Tools/Comparator.C");
  //  Manager::Init();

  gStyle->SetOptStat(1111);

  string dirRECO        = "DQMData/Benchmarks/photons/RECO";
  string dirPAT         = "DQMData/Benchmarks/photons/PAT";

  const char* file = "benchmark.root";
  
  bool highpt = false;
  float ptMin = 0;
  float ptMax = 9999;
  sback->SetFillColor(6);
  Style* style1 = sback;
  Style* style2 = s1;
  Comparator::Mode mode = Comparator::NORMAL;

//  string outdir = "Jets";
    string outdir ="."; 

  Comparator comp(file,
		  dirRECO.c_str(),
		  file,
		  dirPAT.c_str());
  comp.SetStyles(style1, style2, "RECO Objetcs", "PAT Objects");


   TCanvas c0("c0", "legend", 400, 200);    
   FormatPad( &c0, false ); 
   comp.Legend().Draw();
   SavePlot("legend", outdir.c_str() );
    
   comp.SetAxis(1,-0.5,9);
   TCanvas c1("c1", " nPhoAll benchmark");
   FormatPad( &c1, false ); 
   comp.Draw("nPhoAll", mode);
   SavePlot("nPhoAll", outdir.c_str() );
   comp.SetAxis(5);
   
   comp.SetAxis(1,-0.5,9);       
   TCanvas c1("c1", " nPhoBarrel benchmark");
   FormatPad( &c1, false );
   comp.Draw("nPhoBarrel", mode);
   SavePlot("nPhoBarrel", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-0.5,9);       
   TCanvas c1("c1", " nPhoEndcap  benchmark");
   FormatPad( &c1, false );
   comp.Draw("nPhoEndcap", mode);
   SavePlot("nPhoEndcap", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0 ,500);
   TCanvas c1("c1", " scEAll benchmark");
   FormatPad( &c1, false );
   comp.Draw("scEAll", mode);
   SavePlot("scEAll", outdir.c_str() );
   comp.SetAxis(5);  

   comp.SetAxis(1,0,500);  
   TCanvas c1("c1", " scEBarrel benchmark");
   FormatPad( &c1, false );
   comp.Draw("scEBarrel", mode);
   SavePlot("scEBarrel", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,500);
   TCanvas c1("c1", " scEEndcap benchmark");
   FormatPad( &c1, false );
   comp.Draw("scEEndcap", mode);
   SavePlot("scEEndcap", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,500);
   TCanvas c1("c1", " scEtAll benchmark");
   FormatPad( &c1, false );
   comp.Draw("scEtAll", mode);
   SavePlot("scEtAll", outdir.c_str() );
   comp.SetAxis(5);  

   comp.SetAxis(1,0,500);  
   TCanvas c1("c1", "scEtBarrel benchmark");
   FormatPad( &c1, false );
   comp.Draw("scEtBarrel", mode);
   SavePlot("scEtBarrel", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,500);
   TCanvas c1("c1", " scEtEndcap benchmark");
   FormatPad( &c1, false );
   comp.Draw("scEtEndcap", mode);
   SavePlot("scEtEndcap", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0 ,500);
   TCanvas c1("c1", " r9All benchmark");
   FormatPad( &c1, false );
   comp.Draw("r9All", mode);
   SavePlot("r9All", outdir.c_str() );
   comp.SetAxis(5);  

   comp.SetAxis(1,0,500);  
   TCanvas c1("c1", " r9Barrel benchmark");
   FormatPad( &c1, false );
   comp.Draw("r9Barrel", mode);
   SavePlot("r9Barrel", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,500);
   TCanvas c1("c1", " r9Endcap benchmark");
   FormatPad( &c1, false );
   comp.Draw("r9Endcap", mode);
   SavePlot("r9Endcap", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-4,4);
   TCanvas c1("c1", " scEta benchmark");
   FormatPad( &c1, false );
   comp.Draw("scEta", mode);
   SavePlot("scEta", outdir.c_str() );
   comp.SetAxis(5);  

   comp.SetAxis(1,-4 ,4);  
   TCanvas c1("c1", " benchmark");
   FormatPad( &c1, false );
   comp.Draw("scPhi", mode);
   SavePlot("scPhi", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0 ,500);
   TCanvas c1("c1", " phoEAll benchmark");
   FormatPad( &c1, false );
   comp.Draw("phoEAll", mode);
   SavePlot("phoAll", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,500);
   TCanvas c1("c1", " phoEBarrel benchmark");
   FormatPad( &c1, false );
   comp.Draw("phoEBarrel", mode);
   SavePlot("phoEBarrel", outdir.c_str() );
   comp.SetAxis(5);  

   comp.SetAxis(1,0,500);  
   TCanvas c1("c1", " phoEEndcap benchmark");
   FormatPad( &c1, false );
   comp.Draw("phoEEndcap", mode);
   SavePlot("phoEEndcap", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0 ,500); 
   TCanvas c1("c1", " phoEtAll benchmark");
   FormatPad( &c1, false );
   comp.Draw("phoEtAll", mode);
   SavePlot("phoEtAll", outdir.c_str() );
   comp.SetAxis(5);
   
   comp.SetAxis(1,0,500);  
   TCanvas c1("c1", " phoEtBarrel benchmark");
   FormatPad( &c1, false );
   comp.Draw("phoEtBarrel", mode);
   SavePlot("phoEtBarrel", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,500);  
   TCanvas c1("c1", " phoEtEndcap benchmark");
   FormatPad( &c1, false );
   comp.Draw("phoEtEndcap", mode);
   SavePlot("phoEtEndcap", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-4,4);
   TCanvas c1("c1", " phoEta benchmark");
   FormatPad( &c1, false );
   comp.Draw("phoEta", mode);
   SavePlot("phoEta", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-4 ,4);
   TCanvas c1("c1", " phoPhi benchmark");
   FormatPad( &c1, false );
   comp.Draw("phoPhi", mode);
   SavePlot("phoPhi", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-5 ,5);
   TCanvas c1("c1", " nConvAll benchmark");
   FormatPad( &c1, false );
   comp.Draw("nConvAll", mode);
   SavePlot("nConvAll", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-5 ,5);
   TCanvas c1("c1", " nConvBarrel benchmark");
   FormatPad( &c1, false );
   comp.Draw("nConvBarrel", mode);
   SavePlot("nConvBarrel", outdir.c_str() );
   comp.SetAxis(5);  

   comp.SetAxis(1, -5 ,5);  
   TCanvas c1("c1", " nConvAll benchmark");
   FormatPad( &c1, false );
   comp.Draw("nConvEndcap", mode);
   SavePlot("nConvEndcap", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-5,5);
   TCanvas c1("c1", " convEta benchmark");
   FormatPad( &c1, false );
   comp.Draw("convEta", mode);
   SavePlot("convEta", outdir.c_str() );
   comp.SetAxis(5);  

   comp.SetAxis(1,-4 ,4);
   TCanvas c1("c1", " convPhi benchmark");
   FormatPad( &c1, false );
   comp.Draw("convPhi", mode);
   SavePlot("convPhi", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,5);
   TCanvas c1("c1", " EoverPTracksAll benchmark");
   FormatPad( &c1, false );
   comp.Draw("EoverPTracksAll", mode);
   SavePlot("EoverPTracksAll", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,5);
   TCanvas c1("c1", " EoverPTracksBarrel benchmark");
   FormatPad( &c1, false );
   comp.Draw("EoverPTracksBarrel", mode);
   SavePlot("EoverPTracksBarrel", outdir.c_str() );
   comp.SetAxis(5);  

   comp.SetAxis(1,0,5);  
   TCanvas c1("c1", " EoverPTracksEndcap benchmark");
   FormatPad( &c1, false );
   comp.Draw("EoverPTracksEndcap", mode);
   SavePlot("EoverPTracksEndcap", outdir.c_str() );
   comp.SetAxis(5);


   comp.SetAxis(1,0,1);
   TCanvas c1("c1", " DPhiTracksAtVtxAll benchmark");
   FormatPad( &c1, false );
   comp.Draw("DPhiTracksAtVtxAll", mode);
   SavePlot("DPhiTracksAtVtxAll", outdir.c_str() );
   comp.SetAxis(5);


   comp.SetAxis(1,0,1);
   TCanvas c1("c1", " DPhiTracksAtVtxBarrel benchmark");
   FormatPad( &c1, false );
   comp.Draw("DPhiTracksAtVtxBarrel", mode);
   SavePlot("DPhiTracksAtVtxBarrel", outdir.c_str() );
   comp.SetAxis(5);  

   comp.SetAxis(1,0,1);  
   TCanvas c1("c1", " DPhiTracksAtVtxEndcap benchmark");
   FormatPad( &c1, false );
   comp.Draw("DPhiTracksAtVtxEndcap", mode);
   SavePlot("DPhiTracksAtVtxEndcap", outdir.c_str() );
   comp.SetAxis(5);


   comp.SetAxis(1,0,1);
   TCanvas c1("c1", "DCotTracksxAll benchmark");
   FormatPad( &c1, false );
   comp.Draw("DCotTracksxAll", mode);
   SavePlot("DCotTracksxAll", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,1);
   TCanvas c1("c1", "DCotTracksxBarrel benchmark");
   FormatPad( &c1, false );
   comp.Draw("DCotTracksxBarrel", mode);
   SavePlot("DCotTracksxBarrel", outdir.c_str() );
   comp.SetAxis(5);  

   comp.SetAxis(1,0,1);  
   TCanvas c1("c1", "DCotTracksxEndcap benchmark");
   FormatPad( &c1, false );
   comp.Draw("DCotTracksxEndcap", mode);
   SavePlot("DCotTracksxEndcap", outdir.c_str() );
   comp.SetAxis(5);


   comp.SetAxis(1,0,1.5);
   TCanvas c1("c1", " InvMassAll benchmark");
   FormatPad( &c1, false );
   comp.Draw("InvMassAll", mode);
   SavePlot("InvMassAll", outdir.c_str() );
   comp.SetAxis(5);


   comp.SetAxis(1,0,1.5);
   TCanvas c1("c1", " InvMassBarrel benchmark");
   FormatPad( &c1, false );
   comp.Draw("InvMassBarrel", mode);
   SavePlot("InvMassBarrel", outdir.c_str() );
   comp.SetAxis(5);  

   comp.SetAxis(1,0,1.5);  
   TCanvas c1("c1", " InvMassEndcap benchmark");
   FormatPad( &c1, false );
   comp.Draw("InvMassEndcap", mode);
   SavePlot("InvMassEndcap", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,1);
   TCanvas c1("c1", "DPhiTracksAtEcalAll benchmark");
   FormatPad( &c1, false );
   comp.Draw("DPhiTracksAtEcalAll", mode);
   SavePlot("DPhiTracksAtEcalAll", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,1);
   TCanvas c1("c1", "DPhiTracksAtEcalBarrel benchmark");
   FormatPad( &c1, false );
   comp.Draw("DPhiTracksAtEcalBarrel", mode);
   SavePlot("DPhiTracksAtEcalBarrel", outdir.c_str() );
   comp.SetAxis(5);  

   comp.SetAxis(1,0,1);  
   TCanvas c1("c1", "DPhiTracksAtEcalEndcap benchmark");
   FormatPad( &c1, false );
   comp.Draw("DPhiTracksAtEcalEndcap", mode);
   SavePlot("DPhiTracksAtEcalEndcap", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,1);
   TCanvas c1("c1", "DEtaTracksAtEcalAll benchmark");
   FormatPad( &c1, false );
   comp.Draw("DEtaTracksAtEcalAll", mode);
   SavePlot("DEtaTracksAtEcalAll", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,1);
   TCanvas c1("c1", "DEtaTracksAtEcalBarrel benchmark");
   FormatPad( &c1, false );
   comp.Draw("DEtaTracksAtEcalBarrel", mode);
   SavePlot("DEtaTracksAtEcalBarrel", outdir.c_str() );
   comp.SetAxis(5);   

   comp.SetAxis(1,0,1);
   TCanvas c1("c1", "DEtaTracksAtEcalEndcap benchmark");
   FormatPad( &c1, false );
   comp.Draw("DEtaTracksAtEcalEndcap", mode);
   SavePlot("DEtaTracksAtEcalEndcap", outdir.c_str() );
   comp.SetAxis(5);   

   comp.SetAxis(1,-25,25);
   TCanvas c1("c1", " zPVFromTracks benchmark");
   FormatPad( &c1, false );
   comp.Draw("zPVFromTracks", mode);
   SavePlot("zPVFromTracks", outdir.c_str() );
   comp.SetAxis(5);


////////////Non Iso Photons

   comp.SetAxis(1,-0.5,9);
   TCanvas c1("c1", " nPhoNoIsoAll benchmark");
   FormatPad( &c1, false ); 
   comp.Draw("nPhoNoIsoAll", mode);
   SavePlot("nPhoNoIsoAll", outdir.c_str() );
   comp.SetAxis(5);
   
   comp.SetAxis(1,-0.5,9);       
   TCanvas c1("c1", " nPhoNoIsoBarrel benchmark");
   FormatPad( &c1, false );
   comp.Draw("nPhoNoIsoBarrel", mode);
   SavePlot("nPhoNoIsoBarrel", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-0.5,9);       
   TCanvas c1("c1", " nPhoNoIsoEndcap  benchmark");
   FormatPad( &c1, false );
   comp.Draw("nPhoNoIsoEndcap", mode);
   SavePlot("nPhoNoIsoEndcap", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0 ,500);
   TCanvas c1("c1", " scENoIsoAll benchmark");
   FormatPad( &c1, false );
   comp.Draw("scENoIsoAll", mode);
   SavePlot("scENoIsoAll", outdir.c_str() );
   comp.SetAxis(5);  

   comp.SetAxis(1,0,500);  
   TCanvas c1("c1", " scENoIsoBarrel benchmark");
   FormatPad( &c1, false );
   comp.Draw("scENoIsoBarrel", mode);
   SavePlot("scENoIsoBarrel", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,500);
   TCanvas c1("c1", " scENoIsoEndcap benchmark");
   FormatPad( &c1, false );
   comp.Draw("scENoIsoEndcap", mode);
   SavePlot("scENoIsoEndcap", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,500);
   TCanvas c1("c1", " scEtNoIsoAll benchmark");
   FormatPad( &c1, false );
   comp.Draw("scEtNoIsoAll", mode);
   SavePlot("scEtNoIsoAll", outdir.c_str() );
   comp.SetAxis(5);  

   comp.SetAxis(1,0,500);  
   TCanvas c1("c1", "scEtNoIsoBarrel benchmark");
   FormatPad( &c1, false );
   comp.Draw("scEtNoIsoBarrel", mode);
   SavePlot("scEtNoIsoBarrel", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,500);
   TCanvas c1("c1", " scEtNoIsoEndcap benchmark");
   FormatPad( &c1, false );
   comp.Draw("scEtNoIsoEndcap", mode);
   SavePlot("scEtNoIsoEndcap", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0 ,500);
   TCanvas c1("c1", " r9NoIsoAll benchmark");
   FormatPad( &c1, false );
   comp.Draw("r9NoIsoAll", mode);
   SavePlot("r9NoIsoAll", outdir.c_str() );
   comp.SetAxis(5);  

   comp.SetAxis(1,0,500);  
   TCanvas c1("c1", " r9NoIsoBarrel benchmark");
   FormatPad( &c1, false );
   comp.Draw("r9NoIsoBarrel", mode);
   SavePlot("r9NoIsoBarrel", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,500);
   TCanvas c1("c1", " r9NoIsoEndcap benchmark");
   FormatPad( &c1, false );
   comp.Draw("r9NoIsoEndcap", mode);
   SavePlot("r9NoIsoEndcap", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-4,4);
   TCanvas c1("c1", " scEtaNoIso benchmark");
   FormatPad( &c1, false );
   comp.Draw("scEtaNoIso", mode);
   SavePlot("scEtaNoIso", outdir.c_str() );
   comp.SetAxis(5);  

   comp.SetAxis(1,-4 ,4);  
   TCanvas c1("c1", " scPhiNoIso benchmark");
   FormatPad( &c1, false );
   comp.Draw("scPhiNoIso", mode);
   SavePlot("scPhiNoIso", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0 ,500);
   TCanvas c1("c1", " phoENoIsoAll benchmark");
   FormatPad( &c1, false );
   comp.Draw("phoENoIsoAll", mode);
   SavePlot("phoNoIsoAll", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,500);
   TCanvas c1("c1", " phoENoIsoBarrel benchmark");
   FormatPad( &c1, false );
   comp.Draw("phoENoIsoBarrel", mode);
   SavePlot("phoENoIsoBarrel", outdir.c_str() );
   comp.SetAxis(5);  

   comp.SetAxis(1,0,500);  
   TCanvas c1("c1", " phoENoIsoEndcap benchmark");
   FormatPad( &c1, false );
   comp.Draw("phoENoIsoEndcap", mode);
   SavePlot("phoENoIsoEndcap", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0 ,500); 
   TCanvas c1("c1", " phoEtNoIsoAll benchmark");
   FormatPad( &c1, false );
   comp.Draw("phoEtNoIsoAll", mode);
   SavePlot("phoEtNoIsoAll", outdir.c_str() );
   comp.SetAxis(5);
   
   comp.SetAxis(1,0,500);  
   TCanvas c1("c1", " phoEtNoIsoBarrel benchmark");
   FormatPad( &c1, false );
   comp.Draw("phoEtNoIsoBarrel", mode);
   SavePlot("phoEtNoIsoBarrel", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,500);  
   TCanvas c1("c1", " phoEtNoIsoEndcap benchmark");
   FormatPad( &c1, false );
   comp.Draw("phoEtNoIsoEndcap", mode);
   SavePlot("phoEtNoIsoEndcap", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-4,4);
   TCanvas c1("c1", " phoEtaNoIso benchmark");
   FormatPad( &c1, false );
   comp.Draw("phoEtaNoIso", mode);
   SavePlot("phoEtaNoIso", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-4 ,4);
   TCanvas c1("c1", " phoPhiNoIso benchmark");
   FormatPad( &c1, false );
   comp.Draw("phoPhiNoIso", mode);
   SavePlot("phoPhiNoIso", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-5 ,5);
   TCanvas c1("c1", " nConvNoIsoAll benchmark");
   FormatPad( &c1, false );
   comp.Draw("nConvNoIsoAll", mode);
   SavePlot("nConvNoIsoAll", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-5 ,5);
   TCanvas c1("c1", " nConvNoIsoBarrel benchmark");
   FormatPad( &c1, false );
   comp.Draw("nConvNoIsoBarrel", mode);
   SavePlot("nConvNoIsoBarrel", outdir.c_str() );
   comp.SetAxis(5);  

   comp.SetAxis(1, -5 ,5);  
   TCanvas c1("c1", " nConvNoIsoAll benchmark");
   FormatPad( &c1, false );
   comp.Draw("nConvNoIsoEndcap", mode);
   SavePlot("nConvNoIsoEndcap", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-5,5);
   TCanvas c1("c1", " convEtaNoIso benchmark");
   FormatPad( &c1, false );
   comp.Draw("convEtaNoIso", mode);
   SavePlot("convEtaNoIso", outdir.c_str() );
   comp.SetAxis(5);  

   comp.SetAxis(1,-4 ,4);
   TCanvas c1("c1", " convPhiNoIso benchmark");
   FormatPad( &c1, false );
   comp.Draw("convPhiNoIso", mode);
   SavePlot("convPhiNoIso", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,5);
   TCanvas c1("c1", " EoverPTracksNoIsoAll benchmark");
   FormatPad( &c1, false );
   comp.Draw("EoverPTracksNoIsoAll", mode);
   SavePlot("EoverPTracksNoIsoAll", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,5);
   TCanvas c1("c1", " EoverPTracksNoIsoBarrel benchmark");
   FormatPad( &c1, false );
   comp.Draw("EoverPTracksNoIsoBarrel", mode);
   SavePlot("EoverPTracksNoIsoBarrel", outdir.c_str() );
   comp.SetAxis(5);  

   comp.SetAxis(1,0,5);  
   TCanvas c1("c1", " EoverPTracksNoIsoEndcap benchmark");
   FormatPad( &c1, false );
   comp.Draw("EoverPTracksNoIsoEndcap", mode);
   SavePlot("EoverPTracksNoIsoEndcap", outdir.c_str() );
   comp.SetAxis(5);


   comp.SetAxis(1,0,1);
   TCanvas c1("c1", " DPhiTracksAtVtxNoIsoAll benchmark");
   FormatPad( &c1, false );
   comp.Draw("DPhiTracksAtVtxNoIsoAll", mode);
   SavePlot("DPhiTracksAtVtxNoIsoAll", outdir.c_str() );
   comp.SetAxis(5);


   comp.SetAxis(1,0,1);
   TCanvas c1("c1", " DPhiTracksAtVtxNoIsoBarrel benchmark");
   FormatPad( &c1, false );
   comp.Draw("DPhiTracksAtVtxNoIsoBarrel", mode);
   SavePlot("DPhiTracksAtVtxNoIsoBarrel", outdir.c_str() );
   comp.SetAxis(5);  

   comp.SetAxis(1,0,1);  
   TCanvas c1("c1", " DPhiTracksAtVtxNoIsoEndcap benchmark");
   FormatPad( &c1, false );
   comp.Draw("DPhiTracksAtVtxNoIsoEndcap", mode);
   SavePlot("DPhiTracksAtVtxNoIsoEndcap", outdir.c_str() );
   comp.SetAxis(5);


   comp.SetAxis(1,0,1);
   TCanvas c1("c1", "DCotTracksNoIsoAll benchmark");
   FormatPad( &c1, false );
   comp.Draw("DCotTracksNoIsoAll", mode);
   SavePlot("DCotTracksNoIsoAll", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,1);
   TCanvas c1("c1", "DCotTracksxNoIsoBarrel benchmark");
   FormatPad( &c1, false );
   comp.Draw("DCotTracksNoIsoBarrel", mode);
   SavePlot("DCotTracksNoIsoBarrel", outdir.c_str() );
   comp.SetAxis(5);  

   comp.SetAxis(1,0,1);  
   TCanvas c1("c1", "DCotTracksNoIsoEndcap benchmark");
   FormatPad( &c1, false );
   comp.Draw("DCotTracksNoIsoEndcap", mode);
   SavePlot("DCotTracksNoIsoEndcap", outdir.c_str() );
   comp.SetAxis(5);


   comp.SetAxis(1,0,1.5);
   TCanvas c1("c1", " InvMassNoIsoAll benchmark");
   FormatPad( &c1, false );
   comp.Draw("InvMassNoIsoAll", mode);
   SavePlot("InvMassNoIsoAll", outdir.c_str() );
   comp.SetAxis(5);


   comp.SetAxis(1,0,1.5);
   TCanvas c1("c1", " InvMassNoIsoBarrel benchmark");
   FormatPad( &c1, false );
   comp.Draw("InvMassNoIsoBarrel", mode);
   SavePlot("InvMassNoIsoBarrel", outdir.c_str() );
   comp.SetAxis(5);  

   comp.SetAxis(1,0,1.5);  
   TCanvas c1("c1", " InvMassNoIsoEndcap benchmark");
   FormatPad( &c1, false );
   comp.Draw("InvMassNoIsoEndcap", mode);
   SavePlot("InvMassNoIsoEndcap", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,1);
   TCanvas c1("c1", "DPhiTracksAtEcalNoIsoAll benchmark");
   FormatPad( &c1, false );
   comp.Draw("DPhiTracksAtEcalNoIsoAll", mode);
   SavePlot("DPhiTracksAtEcalNoIsoAll", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,1);
   TCanvas c1("c1", "DPhiTracksAtEcalNoIsoBarrel benchmark");
   FormatPad( &c1, false );
   comp.Draw("DPhiTracksAtEcalNoIsoBarrel", mode);
   SavePlot("DPhiTracksAtEcalNoIsoBarrel", outdir.c_str() );
   comp.SetAxis(5);  

   comp.SetAxis(1,0,1);  
   TCanvas c1("c1", "DPhiTracksAtEcalNoIsoEndcap benchmark");
   FormatPad( &c1, false );
   comp.Draw("DPhiTracksAtEcalNoIsoEndcap", mode);
   SavePlot("DPhiTracksAtEcalNoIsoEndcap", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,1);
   TCanvas c1("c1", "DEtaTracksAtEcalNoIsoAll benchmark");
   FormatPad( &c1, false );
   comp.Draw("DEtaTracksAtEcalNoIsoAll", mode);
   SavePlot("DEtaTracksAtEcalNoIsoAll", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,1);
   TCanvas c1("c1", "DEtaTracksAtEcalNoIsoBarrel benchmark");
   FormatPad( &c1, false );
   comp.Draw("DEtaTracksAtEcalNoIsoBarrel", mode);
   SavePlot("DEtaTracksAtEcalNoIsoBarrel", outdir.c_str() );
   comp.SetAxis(5);   

   comp.SetAxis(1,0,1);
   TCanvas c1("c1", "DEtaTracksAtEcalNoIsoEndcap benchmark");
   FormatPad( &c1, false );
   comp.Draw("DEtaTracksAtEcalNoIsoEndcap", mode);
   SavePlot("DEtaTracksAtEcalNoIsoEndcap", outdir.c_str() );
   comp.SetAxis(5);   

   comp.SetAxis(1,-25,25);  
   TCanvas c1("c1", " zPVFromTracksNoIso benchmark");
   FormatPad( &c1, false );
   comp.Draw("zPVFromTracksNoIso", mode);
   SavePlot("zPVFromTracksNoIso", outdir.c_str() );
   comp.SetAxis(5);
 


  
}
