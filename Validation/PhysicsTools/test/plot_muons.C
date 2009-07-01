{
  
  gROOT->LoadMacro("../Tools/NicePlot.C");
  InitNicePlot();
  gROOT->LoadMacro("../Tools/Comparator.C");
  //  Manager::Init();

  gStyle->SetOptStat(1111);

  string dirRECO        = "DQMData/Benchmarks/muons/RECO";
  string dirPAT         = "DQMData/Benchmarks/muons/PAT";

  const char* file = "benchmark.root";
  
  bool highpt = false;
  float ptMin = 0;
  float ptMax = 9999;
  sback->SetFillColor(7);
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

  //////////// monitoring of eta parameter
    
   comp.SetAxis(1, -6,6);
   TCanvas c1("c1", "Glb eta benchmark");
   FormatPad( &c1, false ); 
   comp.Draw("GlbMuon_Glb_eta", mode);
   SavePlot("GlbMuon_Glb_eta", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1, -6,6);
   TCanvas c2("c2", "Tk eta benchmark");
   FormatPad( &c2, false );
   comp.Draw("GlbMuon_Tk_eta", mode);
   SavePlot("GlbMuon_Tk_eta", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1, -6,6);
   TCanvas c3("c3", "Sta eta benchmark");
   FormatPad( &c3, false );
   comp.Draw("GlbMuon_Sta_eta", mode);
   SavePlot("GlbMuon_Sta_eta", outdir.c_str() );

   comp.SetAxis(5);comp.SetAxis(1, -6,6);
   TCanvas c3("c3", "Res TkGlb eta benchmark");
   FormatPad( &c3, false );
   comp.Draw("Res_TkGlb_eta", mode);
   SavePlot("Res_TkGlb_eta", outdir.c_str() );
   comp.SetAxis(5);
    
   comp.SetAxis(1, -6,6);
   TCanvas c3("c3", " Res Glb Sta eta benchmark");
   FormatPad( &c3, false );
   comp.Draw("Res_GlbSta_eta", mode);
   SavePlot("Res_GlbSta_eta", outdir.c_str() );
   comp.SetAxis(5);

   
   comp.SetAxis(1, -6,6);
   TCanvas c3("c3", "Res TkSta eta benchmark");
   FormatPad( &c3, false );
   comp.Draw("Res_TkSta_eta", mode);
   SavePlot("Res_TkSta_eta", outdir.c_str() );
   comp.SetAxis(5);

  
   comp.SetAxis(1, -6,6);
   TCanvas c3("c3", "ResVsEta TkGlb eta benchmark");
   FormatPad( &c3, false );
   comp.Draw("ResVsEta_TkGlb_eta", mode);
   SavePlot("ResVsEta_TkGlb_eta", outdir.c_str() );
   comp.SetAxis(5);
   
   comp.SetAxis(1, -6,6);
   TCanvas c3("c3", "ResVsEta GlbSta eta benchmark");
   FormatPad( &c3, false );
   comp.Draw("ResVsEta_GlbSta_eta", mode);
   SavePlot("ResVsEta_GlbSta_eta", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1, -6,6);
   TCanvas c3("c3", "ResVsEta TkSta eta benchmark");
   FormatPad( &c3, false );
   comp.Draw("ResVsEta_TkSta_eta", mode);
   SavePlot("ResVsEta_TkSta_eta", outdir.c_str() );
   comp.SetAxis(5);
   
   comp.SetAxis(1, -6,6);
   TCanvas c3("c3", "TkMuon eta benchmark");
   FormatPad( &c3, false );
   comp.Draw("TkMuon_eta", mode);
   SavePlot("TkMuon_eta", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1, -6,6);
   TCanvas c3("c3", "StaMuon eta benchmark");
   FormatPad( &c3, false );
   comp.Draw("StaMuon_eta", mode);
   SavePlot("StaMuon_eta", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1, -6,6);
   TCanvas c3("c3", "Sta Eta benchmark");
   FormatPad( &c3, false );
   comp.Draw("StaEta", mode);
   SavePlot("StaEta", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1, -6,6);
   TCanvas c3("c3", " StaEta ifCombinedAlso benchmark");
   FormatPad( &c3, false );
   comp.Draw("StaEta_ifCombinedAlso", mode);
   SavePlot("StaEta_ifCombinedAlso", outdir.c_str() );
   comp.SetAxis(5);


  // monitoring of theta parameter


   comp.SetAxis(1, 0,4);
   TCanvas c1("c1", "Glb theta benchmark");
   FormatPad( &c1, false ); 
   comp.Draw("GlbMuon_Glb_theta", mode);
   SavePlot("GlbMuon_Glb_theta", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1, 0,4);
   TCanvas c2("c2", "Tk theta benchmark");
   FormatPad( &c2, false );
   comp.Draw("GlbMuon_Tk_theta", mode);
   SavePlot("GlbMuon_Tk_theta", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1, 0,4);
   TCanvas c3("c3", "Sta theta benchmark");
   FormatPad( &c3, false );
   comp.Draw("GlbMuon_Sta_theta", mode);
   SavePlot("GlbMuon_Sta_theta", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1, 0,4);
   TCanvas c3("c3", "Res TkGlb theta benchmark");
   FormatPad( &c3, false );
   comp.Draw("Res_TkGlb_theta", mode);
   SavePlot("Res_TkGlb_theta", outdir.c_str() );
   comp.SetAxis(5);
    
   comp.SetAxis(1, 0,4);
   TCanvas c3("c3", " Res Glb Tk Sta eta benchmark");
   FormatPad( &c3, false );
   comp.Draw("Res_TkSta_theta", mode);
   SavePlot("Res_TkSta_theta", outdir.c_str() );
   comp.SetAxis(5);

   
   comp.SetAxis(1, 0,4);
   TCanvas c3("c3", "ResVsTheta TkGlb theta benchmark");
   FormatPad( &c3, false );
   comp.Draw("ResVsTheta_TkGlb_theta", mode);
   SavePlot("ResVsTheta_TkGlb_theta", outdir.c_str() );
   comp.SetAxis(5);

  
   comp.SetAxis(1, 0,4);
   TCanvas c3("c3", "ResVsTheta GlbSta theta benchmark");
   FormatPad( &c3, false );
   comp.Draw("ResVsTheta_GlbSta_theta", mode);
   SavePlot("ResVsTheta_GlbSta_theta", outdir.c_str() );
   comp.SetAxis(5);
   
   comp.SetAxis(1, 0,4);
   TCanvas c3("c3", "ResVsTheta_TkSta_theta benchmark");
   FormatPad( &c3, false );
   comp.Draw("ResVsTheta_TkSta_theta", mode);
   SavePlot("ResVsTheta_TkSta_theta", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1, 0,4);
   TCanvas c3("c3", "TkMuon theta benchmark");
   FormatPad( &c3, false );
   comp.Draw("TkMuon_theta", mode);
   SavePlot("TkMuon_theta", outdir.c_str() );
   comp.SetAxis(5);
   
   comp.SetAxis(1, 0,4);
   TCanvas c3("c3", "StaMuon theta benchmark");
   FormatPad( &c3, false );
   comp.Draw("StaMuon_theta", mode);
   SavePlot("StaMuon_theta", outdir.c_str() );
   comp.SetAxis(5);


  // monitoring of phi paramater  
             
   comp.SetAxis(1, -4,4);
   TCanvas c1("c1", "Glb phi benchmark");
   FormatPad( &c1, false ); 
   comp.Draw("GlbMuon_Glb_phi", mode);
   SavePlot("GlbMuon_Glb_phi", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1, -4,4);
   TCanvas c2("c2", "Tk phi benchmark");
   FormatPad( &c2, false );
   comp.Draw("GlbMuon_Tk_phi", mode);
   SavePlot("GlbMuon_Tk_phi", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1, -4,4);
   TCanvas c3("c3", "Sta phi benchmark");
   FormatPad( &c3, false );
   comp.Draw("GlbMuon_Sta_phi", mode);
   SavePlot("GlbMuon_Sta_phi", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1, -4,4);
   TCanvas c3("c3", "Res TkGlb phi benchmark");
   FormatPad( &c3, false );
   comp.Draw("Res_TkGlb_phi", mode);
   SavePlot("Res_TkGlb_phi", outdir.c_str() );
   comp.SetAxis(5);
    
   comp.SetAxis(1, -4,4);
   TCanvas c3("c3", " Res Glb Sta phi benchmark");
   FormatPad( &c3, false );
   comp.Draw("Res_GlbSta_phi", mode);
   SavePlot("Res_GlbSta_phi", outdir.c_str() );
   comp.SetAxis(5);

   
   comp.SetAxis(1, -4,4);
   TCanvas c3("c3", "Res TkSta phi benchmark");
   FormatPad( &c3, false );
   comp.Draw("Res_TkSta_phi", mode);
   SavePlot("Res_TkSta_phi", outdir.c_str() );
   comp.SetAxis(5);

  
   comp.SetAxis(1, -4,4);
   TCanvas c3("c3", "ResVsEta TkGlb phi benchmark");
   FormatPad( &c3, false );
   comp.Draw("ResVsPhi_TkGlb_phi", mode);
   SavePlot("ResVsPhi_TkGlb_phi", outdir.c_str() );
   comp.SetAxis(5);
   
   comp.SetAxis(1, -4,4);
   TCanvas c3("c3", "ResVsEta GlbSta phi benchmark");
   FormatPad( &c3, false );
   comp.Draw("ResVsPhi_GlbSta_phi", mode);
   SavePlot("ResVsPhi_GlbSta_ephi", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1, -4,4);
   TCanvas c3("c3", "ResVsEta TkSta phi benchmark");
   FormatPad( &c3, false );
   comp.Draw("ResVsPhi_TkSta_phi", mode);
   SavePlot("ResVsPhi_TkSta_phi", outdir.c_str() );
   comp.SetAxis(5);
   
   comp.SetAxis(1, -4,4);
   TCanvas c3("c3", "TkMuon phi benchmark");
   FormatPad( &c3, false );
   comp.Draw("TkMuon_phi", mode);
   SavePlot("TkMuon_phi", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1, -4,4);
   TCanvas c3("c3", "StaMuon phi benchmark");
   FormatPad( &c3, false );
   comp.Draw("StaMuon_phi", mode);
   SavePlot("StaMuon_phi", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1, -4,4);
   TCanvas c3("c3", "Sta Phi benchmark");
   FormatPad( &c3, false );
   comp.Draw("StaPhi", mode);
   SavePlot("StaPhi", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1, -4,4);
   TCanvas c3("c3", " StaPhi ifCombinedAlso benchmark");
   FormatPad( &c3, false );
   comp.Draw("StaPhi_ifCombinedAlso", mode);
   SavePlot("StaPhi_ifCombinedAlso", outdir.c_str() );
   comp.SetAxis(5);           
          
// monitoring of the chi2 parameter
   comp.SetAxis(1, 0,50);
   TCanvas c3("c3", " GlbMuon Glb chi2OverDf benchmark");
   FormatPad( &c3, false );
   comp.Draw("GlbMuon_Glb_chi2OverDf", mode);
   SavePlot("GlbMuon_Glb_chi2OverDf", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1, 0,50);
   TCanvas c3("c3", "  GlbMuon Sta chi2OverDf benchmark");
   FormatPad( &c3, false );
   comp.Draw("GlbMuon_Sta_chi2OverDf", mode);
   SavePlot("GlbMuon_Sta_chi2OverDf", outdir.c_str() );
   comp.SetAxis(5);
 
   comp.SetAxis(1, 0,50);
   TCanvas c3("c3", "TkMuon chi2OverDf");
   FormatPad( &c3, false );
   comp.Draw("TkMuon_chi2OverDf", mode);
   SavePlot("TkMuon_chi2OverDf", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1, 0,50);
   TCanvas c3("c3", "StaMuon_chi2OverDf benchmark");
   FormatPad( &c3, false );
   comp.Draw("StaMuon_chi2OverDf", mode);
   SavePlot("StaMuon_chi2OverDf", outdir.c_str() );
   comp.SetAxis(5);


// monitoring of the momentum
   comp.SetAxis(1,0,500);
   TCanvas c3("c3", " Glb p benchmark");
   FormatPad( &c3, false );
   comp.Draw("GlbMuon_Glb_p", mode);
   SavePlot("GlbMuon_Glb_p", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,500);
   TCanvas c3("c3", "  Tk p benchmark");
   FormatPad( &c3, false );  
   comp.Draw("GlbMuon_Tk_p", mode);
   SavePlot("GlbMuon_Tk_p", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,500);
   TCanvas c3("c3", " Sta p benchmark");
   FormatPad( &c3, false );
   comp.Draw("GlbMuon_Sta_p", mode);
   SavePlot("GlbMuon_Sta_p", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,500);
   TCanvas c3("c3", "TkMuon p benchmark");
   FormatPad( &c3, false );
   comp.Draw("TkMuon_p", mode);
   SavePlot("TkMuon_p", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,500);
   TCanvas c3("c3", " StaMuon p benchmark");
   FormatPad( &c3, false );
   comp.Draw("StaMuon_p", mode);
   SavePlot("StaMuon_p", outdir.c_str() );
   comp.SetAxis(5);

// monitoring of the transverse momentum
   comp.SetAxis(1,0,200);
   TCanvas c3("c3", "GlbMuon Glp pt benchmark");
   FormatPad( &c3, false );
   comp.Draw("GlbMuon_Glb_pt", mode);  
   SavePlot("GlbMuon_Glb_pt", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,200);
   TCanvas c3("c3", " GlbMuon Tk q benchmark");
   FormatPad( &c3, false );
   comp.Draw("GlbMuon_Tk_q", mode);  
   SavePlot("GlbMuon_Tk_q", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,200);
   TCanvas c3("c3", " Sta q benchmark");
   FormatPad( &c3, false );
   comp.Draw("GlbMuon_Sta_q", mode);
   SavePlot("GlbMuon_Sta_q", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,200);
   TCanvas c3("c3", "TkMuon pt benchmark");
   FormatPad( &c3, false );
   comp.Draw("TkMuon_pt", mode);
   SavePlot("TkMuon_pt", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,200);
   TCanvas c3("c3", " StaMuon pt benchmark");
   FormatPad( &c3, false );
   comp.Draw("StaMuon_pt", mode);
   SavePlot("StaMuon_pt", outdir.c_str() );
   comp.SetAxis(5);



// monitoring of the muon charge

   comp.SetAxis(1,-2.5,2.5);
   TCanvas c3("c3", "Glb q benchmark");
   FormatPad( &c3, false );  
   comp.Draw("GlbMuon_Glb_q", mode);
   SavePlot("GlbMuon_Glb_q", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-2.5,2.5);
   TCanvas c3("c3", " Tk q benchmark");
   FormatPad( &c3, false );
   comp.Draw("GlbMuon_Tk_q", mode);
   SavePlot("GlbMuon_Tk_q", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-2.5,2.5);
   TCanvas c3("c3", " Sta qbenchmark");
   FormatPad( &c3, false );
   comp.Draw("GlbMuon_Sta_q", mode);
   SavePlot("GlbMuon_Sta_q", outdir.c_str() );
   comp.SetAxis(5);


   comp.SetAxis(1,-2.5,2.5);
   TCanvas c3("c3", " TkMuon q benchmark");
   FormatPad( &c3, false );
   comp.Draw("TkMuon_q", mode);
   SavePlot("TkMuon_q", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-2.5,2.5);
   TCanvas c3("c3", " StaMuon q benchmark");
   FormatPad( &c3, false );
   comp.Draw("StaMuon_q", mode);
   SavePlot("StaMuon_q", outdir.c_str() );
   comp.SetAxis(5);

   

// monitoring of the momentum resolution

   comp.SetAxis(1,-0.001,0.001);
   TCanvas c3("c3", " Res_TkGlb_qOverp benchmark");
   FormatPad( &c3, false );
   comp.Draw("Res_TkGlb_qOverp", mode);
   SavePlot("Res_TkGlb_qOverp", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-0.01,0.01);
   TCanvas c3("c3", "  Res_GlbSta_qOverp benchmark");
   FormatPad( &c3, false );
   comp.Draw("Res_GlbSta_qOverp", mode);
   SavePlot("Res_GlbSta_qOverp", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-0.01,0.01);
   TCanvas c3("c3", "  Res_TkSta_qOverp benchmark");
   FormatPad( &c3, false );
   comp.Draw("Res_TkSta_qOverp", mode);
   SavePlot("Res_TkSta_qOverp", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-0.001,0.001);
   TCanvas c3("c3", "  Res_TkGlb_oneOverp benchmark");
   FormatPad( &c3, false );
   comp.Draw("Res_TkGlb_oneOverp", mode);
   SavePlot("Res_TkGlb_oneOverp", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-0.01,0.01);
   TCanvas c3("c3", " Res_GlbSta_oneOverp  benchmark");
   FormatPad( &c3, false );
   comp.Draw("Res_GlbSta_oneOverp", mode);
   SavePlot("Res_GlbSta_oneOverp", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-0.01,0.01);
   TCanvas c3("c3", "Res_TkSta_oneOverp  benchmark");
   FormatPad( &c3, false );
   comp.Draw("Res_TkSta_oneOverp", mode);
   SavePlot("Res_TkSta_oneOverp", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-0.001,0.001);
   TCanvas c3("c3", " Res_TkGlb_qOverpt benchmark");
   FormatPad( &c3, false );
   comp.Draw("Res_TkGlb_qOverpt", mode);
   SavePlot("Res_TkGlb_qOverpt", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-0.01,0.01);
   TCanvas c3("c3", " Res_GlbSta_qOverpt benchmark");
   FormatPad( &c3, false );
   comp.Draw("Res_GlbSta_qOverpt", mode);
   SavePlot("Res_GlbSta_qOverpt", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-0.01,0.01);
   TCanvas c3("c3", " Res_TkSta_qOverpt benchmark");
   FormatPad( &c3, false );
   comp.Draw("Res_TkSta_qOverpt", mode);
   SavePlot("Res_TkSta_qOverpt", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-0.001,0.001);
   TCanvas c3("c3", " Res_TkGlb_oneOverpt  benchmark");
   FormatPad( &c3, false );
   comp.Draw("Res_TkGlb_oneOverpt", mode);
   SavePlot("Res_TkGlb_oneOverpt", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-0.01,0.01);
   TCanvas c3("c3", " Res_GlbSta_oneOverpt benchmark");
   FormatPad( &c3, false );
   comp.Draw("Res_GlbSta_oneOverpt", mode);
   SavePlot("Res_GlbSta_oneOverpt", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-0.01,0.01);
   TCanvas c3("c3", " Res_TkSta_oneOverpt benchmark");
   FormatPad( &c3, false );
   comp.Draw("Res_TkSta_oneOverpt", mode);
   SavePlot("Res_TkSta_oneOverpt", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-3,3);
   TCanvas c3("c3", " ResVsEta_TkGlb_oneOverpt  benchmark");
   FormatPad( &c3, false );
   comp.Draw("ResVsEta_TkGlb_oneOverpt", mode);
   SavePlot("ResVsEta_TkGlb_oneOverpt", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-3,3);
   TCanvas c3("c3", " ResVsEta_GlbSta_oneOverpt  benchmark");
   FormatPad( &c3, false );
   comp.Draw("ResVsEta_GlbSta_oneOverpt", mode);
   SavePlot("ResVsEta_GlbSta_oneOverpt", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-3,3);
   TCanvas c3("c3", " ResVsEta_TkSta_oneOverpt benchmark");
   FormatPad( &c3, false );
   comp.Draw("ResVsEta_TkSta_oneOverpt", mode);
   SavePlot("ResVsEta_TkSta_oneOverpt", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-3,3);
   TCanvas c3("c3", " ResVsPhi_TkGlb_oneOverpt benchmark");
   FormatPad( &c3, false );
   comp.Draw("ResVsPhi_TkGlb_oneOverpt", mode);
   SavePlot("ResVsPhi_TkGlb_oneOverpt", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-3,3);
   TCanvas c3("c3", "ResVsPhi_GlbSta_oneOverpt  benchmark");
   FormatPad( &c3, false );
   comp.Draw("ResVsPhi_GlbSta_oneOverpt", mode);
   SavePlot("ResVsPhi_GlbSta_oneOverpt", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-3,3);
   TCanvas c3("c3", "ResVsPhi_TkSta_oneOverpt  benchmark");
   FormatPad( &c3, false );
   comp.Draw("ResVsPhi_TkSta_oneOverpt", mode);
   SavePlot("ResVsPhi_TkSta_oneOverpt", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,2);
   TCanvas c3("c3", "ResVsPt_TkGlb_oneOverpt  benchmark");
   FormatPad( &c3, false );
   comp.Draw("ResVsPt_TkGlb_oneOverpt", mode);
   SavePlot("ResVsPt_TkGlb_oneOverpt", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,2);
   TCanvas c3("c3", " ResVsPt_GlbSta_oneOverpt benchmark");
   FormatPad( &c3, false );
   comp.Draw("ResVsPt_GlbSta_oneOverpt", mode);
   SavePlot("ResVsPt_GlbSta_oneOverpt", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,2);
   TCanvas c3("c3", "ResVsPt_TkSta_oneOverpt  benchmark");
   FormatPad( &c3, false );
   comp.Draw("ResVsPt_TkSta_oneOverpt", mode);
   SavePlot("ResVsPt_TkSta_oneOverpt", outdir.c_str() );
   comp.SetAxis(5);
 
  
// monitoring of the recHits provenance

   comp.SetAxis(1,0,1);
   TCanvas c3("c3", " StaRh_Frac_inGlb benchmark");
   FormatPad( &c3, false );
   comp.Draw("StaRh_Frac_inGlb", mode);
   SavePlot("StaRh_Frac_inGlb", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,1);
   TCanvas c3("c3", " TkRh_Frac_inGlb benchmark");
   FormatPad( &c3, false );
   comp.Draw("TkRh_Frac_inGlb", mode);
   SavePlot("TkRh_Frac_inGlb", outdir.c_str() );
   comp.SetAxis(5);
  
   comp.SetAxis(1,0,1);
   TCanvas c3("c3", " StaRh_inGlb_Div_RhAssoSta benchmark");
   FormatPad( &c3, false );
   comp.Draw("StaRh_inGlb_Div_RhAssoSta", mode);
   SavePlot("StaRh_inGlb_Div_RhAssoSta", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,1);
   TCanvas c3("c3", " TkRh_inGlb_Div_RhAssoTk benchmark");
   FormatPad( &c3, false );
   comp.Draw("TkRh_inGlb_Div_RhAssoTk", mode);
   SavePlot("TkRh_inGlb_Div_RhAssoTk", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,1);
   TCanvas c3("c3", "GlbRh_Div_RhAssoStaTk  benchmark");
   FormatPad( &c3, false );
   comp.Draw("GlbRh_Div_RhAssoStaTk", mode);
   SavePlot("GlbRh_Div_RhAssoStaTk", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,1);
   TCanvas c3("c3", "invalidRh_Frac_inTk  benchmark");
   FormatPad( &c3, false );
   comp.Draw("invalidRh_Frac_inTk", mode);
   SavePlot("invalidRh_Frac_inTk", outdir.c_str() );
   comp.SetAxis(5);


}
