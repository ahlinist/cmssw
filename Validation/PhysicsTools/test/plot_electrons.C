{
  
  gROOT->LoadMacro("../Tools/NicePlot.C");
  InitNicePlot();
  gROOT->LoadMacro("../Tools/Comparator.C");
  //  Manager::Init();

  gStyle->SetOptStat(1111);

  string dirRECO        = "DQMData/Benchmarks/electrons/RECO";
  string dirPAT         = "DQMData/Benchmarks/electrons/PAT";

  const char* file = "benchmark.root";
  
  bool highpt = false;
  float ptMin = 0;
  float ptMax = 9999;

  sback->SetFillColor(2);   
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
    
   comp.SetAxis(1, -30,100);
   TCanvas c1("c1", "pt benchmark");
   FormatPad( &c1, false ); 
   comp.Draw("pt", mode);
   SavePlot("electron_pt", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1, -6,6);
   TCanvas c2("c2", "eta benchmark");
   FormatPad( &c2, false );
   comp.Draw("eta", mode);
   SavePlot("electron_eta", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1, -4,4);
   TCanvas c3("c3", "phi benchmark");
   FormatPad( &c3, false );
   comp.Draw("phi", mode);
   SavePlot("electron_phi", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,150);
   TCanvas c3("c3", "DiElectronMass benchmark");
   FormatPad( &c3, false );
   comp.Draw("DiElectronMass", mode);
   SavePlot("DiElectronMass", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,10); 
   TCanvas c3("c3", " TrackIso benchmark");
   FormatPad( &c3, false );
   comp.Draw("TrackIso", mode);
   SavePlot("TrackIso", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,10); 
   TCanvas c3("c3", " CaloIso benchmark");
   FormatPad( &c3, false );
   comp.Draw("CaloIso", mode);
   SavePlot("CaloIso", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-5,5); 
   TCanvas c3("c3", "ele_matchingObjectEta benchmark");
   FormatPad( &c3, false );
   comp.Draw("ele_matchingObjectEta", mode);
   SavePlot("ele_matchingObjectEta", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1, 0,100);
   TCanvas c3("c3", "ele_matchingObjectPt benchmark");
   FormatPad( &c3, false );
   comp.Draw("ele_matchingObjectPt", mode);
   SavePlot("ele_matchingObjectPt", outdir.c_str() );
   comp.SetAxis(5);
   
   comp.SetAxis(1,-4,4);
   TCanvas c3("c3", "ele_matchingObjectPhi benchmark");
   FormatPad( &c3, false );
   comp.Draw("ele_matchingObjectPhi", mode);
   SavePlot("ele_matchingObjectPhi", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-25,25);
   TCanvas c3("c3", "ele_matchingObjectZ benchmark");
   FormatPad( &c3, false );
   comp.Draw("ele_matchingObjectZ", mode);
   SavePlot("ele_matchingObjectZ", outdir.c_str() );
   comp.SetAxis(5);
 
   comp.SetAxis(1,-2,2);
   TCanvas c3("c3", "ele_charge benchmark");
   FormatPad( &c3, false );
   comp.Draw("ele_charge", mode);
   SavePlot("ele_charge", outdir.c_str() );
   comp.SetAxis(5);
   
   comp.SetAxis(1,0,300);
   TCanvas c3("c3", "ele_vertexP benchmark");
   FormatPad( &c3, false );
   comp.Draw("ele_vertexP", mode);
   SavePlot("ele_vertexP", outdir.c_str() );
   comp.SetAxis(5);   
   
   comp.SetAxis(1,0,100);
   TCanvas c3("c3", "ele_vertexPt benchmark");
   FormatPad( &c3, false );
   comp.Draw("ele_vertexPt", mode);
   SavePlot("ele_vertexPt", outdir.c_str() );
   comp.SetAxis(5);   

   comp.SetAxis(1,-5,5);
   TCanvas c3("c3", "ele_vertexEta benchmark");
   FormatPad( &c3, false );
   comp.Draw("ele_vertexEta", mode);
   SavePlot("ele_vertexEta", outdir.c_str() );
   comp.SetAxis(5);
 
   comp.SetAxis(1,-4,4);
   TCanvas c3("c3", "ele_vertexPhi benchmark");
   FormatPad( &c3, false );
   comp.Draw("ele_vertexPhi", mode);
   SavePlot("ele_vertexPhi", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-0.1,0.1);
   TCanvas c3("c3", "ele_vertexX benchmark");
   FormatPad( &c3, false );
   comp.Draw("ele_vertexX", mode);
   SavePlot("ele_vertexX", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-0.1,0.1);
   TCanvas c3("c3", "ele_vertexY benchmark");
   FormatPad( &c3, false );
   comp.Draw("ele_vertexY", mode);
   SavePlot("ele_vertexY", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-25,25);
   TCanvas c3("c3", "ele_vertexZ benchmark");
   FormatPad( &c3, false );
   comp.Draw("ele_vertexZ", mode);
   SavePlot("ele_vertexZ", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,5);
   TCanvas c3("c3", "ele_EoverP_all benchmark");
   FormatPad( &c3, false );
   comp.Draw("ele_EoverP_all", mode);
   SavePlot("ele_EoverP_all", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,5);
   TCanvas c3("c3", "ele_EoPout benchmark");
   FormatPad( &c3, false );
   comp.Draw("ele_EoPout", mode);
   SavePlot("ele_EoPout", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-0.05,0.05);
   TCanvas c3("c3", "ele_dEtaSc_propVtx benchmark");
   FormatPad( &c3, false );
   comp.Draw("ele_dEtaSc_propVtx", mode);
   SavePlot("ele_dEtaSc_propVtx", outdir.c_str() );
   comp.SetAxis(5);
  
   comp.SetAxis(1,-0.05,0.05);
   TCanvas c3("c3", "ele_dPhiSc_propVtx benchmark");
   FormatPad( &c3, false );
   comp.Draw("ele_dPhiSc_propVtx", mode);
   SavePlot("ele_dPhiSc_propVtx", outdir.c_str() );
   comp.SetAxis(5);
   
   comp.SetAxis(1,-0.05,0.05);
   TCanvas c3("c3", "ele_dPhiCl_propOut benchmark");
   FormatPad( &c3, false );
   comp.Draw("ele_dPhiCl_propOut", mode);
   SavePlot("ele_dPhiCl_propOut", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-0.05,0.05);
   TCanvas c3("c3", "ele_HoE benchmark");
   FormatPad( &c3, false );
   comp.Draw("ele_HoE", mode);
   SavePlot("ele_HoE", outdir.c_str() );
   comp.SetAxis(5);
 
   comp.SetAxis(1,0,150);
   TCanvas c3("c3", "ele_classes benchmark");
   FormatPad( &c3, false );
   comp.Draw("ele_classes", mode);
   SavePlot("ele_classes", outdir.c_str() );
   comp.SetAxis(5);
 
   comp.SetAxis(1,0,3);
   TCanvas c3("c3", "ele_eta benchmark");
   FormatPad( &c3, false );
   comp.Draw("ele_eta", mode);
   SavePlot("ele_eta", outdir.c_str() );
   comp.SetAxis(5);
 
   comp.SetAxis(1,0,3);
   TCanvas c3("c3", "ele_eta_golden benchmark");
   FormatPad( &c3, false );
   comp.Draw("ele_eta_golden", mode);
   SavePlot("ele_eta_golden", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,3);
   TCanvas c3("c3", "ele_eta_show benchmark");
   FormatPad( &c3, false );
   comp.Draw("ele_eta_show", mode);
   SavePlot("ele_eta_show", outdir.c_str() );
   comp.SetAxis(5);
   
   comp.SetAxis(1,0,30);
   TCanvas c3("c3", "ele_foundHits benchmark");
   FormatPad( &c3, false );
   comp.Draw("ele_foundHits", mode);
   SavePlot("ele_foundHits", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,15);
   TCanvas c3("c3", "ele_chi2 benchmark");
   FormatPad( &c3, false );
   comp.Draw("ele_chi2", mode);
   SavePlot("ele_chi2", outdir.c_str() );
   comp.SetAxis(5);
 
   comp.SetAxis(1,0,100);
   TCanvas c3("c3", "ele_PinMnPout_mode benchmark");
   FormatPad( &c3, false );
   comp.Draw("ele_PinMnPout_mode", mode);
   SavePlot("ele_PinMnPout_mode", outdir.c_str() );
   comp.SetAxis(5);


}
