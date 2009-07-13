{
  
  gROOT->LoadMacro("../Tools/NicePlot.C");
  InitNicePlot();
  gROOT->LoadMacro("../Tools/Comparator.C");
  //  Manager::Init();

  gStyle->SetOptStat(1111);

  string dirRECO        = "DQMData/Benchmarks/mets/CaloMET/RECO";
  string dirPAT         = "DQMData/Benchmarks/mets/CaloMET/PAT";

  const char* file = "benchmark.root";
  
  bool highpt = false;
  float ptMin = 0;
  float ptMax = 9999;

  sback->SetFillColor(8);   
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
   SavePlot("met_pt", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1, -6,6);
   TCanvas c2("c2", "eta benchmark");
   FormatPad( &c2, false );
   comp.Draw("eta", mode);
   SavePlot("met_eta", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1, -4,4);
   TCanvas c3("c3", "phi benchmark");
   FormatPad( &c3, false );
   comp.Draw("phi", mode);
   SavePlot("met_phi", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1, 1,5);
   TCanvas c3("c3", "met benchmark");
   FormatPad( &c3, false );
   comp.Draw("met", mode);
   SavePlot("met", outdir.c_str() );
   comp.SetAxis(5);
 

   comp.SetAxis(1,0,1);
   TCanvas c3("c3", "METTask_Nevents");
   FormatPad( &c3, false );
   comp.Draw("METTask_Nevents", mode);
   SavePlot("METTask_Nevents", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-500,500); 
   TCanvas c3("c3", "METTask_CaloMEx");
   FormatPad( &c3, false );
   comp.Draw("METTask_CaloMEx", mode);
   SavePlot("METTask_CaloMEx", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-500,500); 
   TCanvas c3("c3", "METTask_CaloMEy");
   FormatPad( &c3, false );
   comp.Draw("METTask_CaloMEy", mode);
   SavePlot("METTask_CaloMEy", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-500,500);
   TCanvas c3("c3", "METTask_CaloEz");
   FormatPad( &c3, false );
   comp.Draw("METTask_CaloEz", mode);
   SavePlot("METTask_CaloEz", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,51);
   TCanvas c3("c3", "METTask_CaloMETSig");
   FormatPad( &c3, false );
   comp.Draw("METTask_CaloMETSig", mode);
   SavePlot("METTask_CaloMETSig", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,1000);
   TCanvas c3("c3", "METTask_CaloMET");
   FormatPad( &c3, false );
   comp.Draw("METTask_CaloMET", mode);
   SavePlot("METTask_CaloMET", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-4,4);
   TCanvas c3("c3", "METTask_CaloMETPhi");
   FormatPad( &c3, false );
   comp.Draw("METTask_CaloMETPhi", mode);   
   SavePlot("METTask_CaloMETPhi", outdir.c_str() );   
   comp.SetAxis(5);

   comp.SetAxis(1,0,2000);
   TCanvas c3("c3", "METTask_CaloSumET");
   FormatPad( &c3, false );
   comp.Draw("METTask_CaloSumET", mode);
   SavePlot("METTask_CaloSumET", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-200,200);
   TCanvas c3("c3", "METTask_CaloMEx_LS");
   FormatPad( &c3, false );
   comp.Draw("METTask_CaloMEx_LS", mode);
   SavePlot("METTask_CaloMEx_LS", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-200,200);
   TCanvas c3("c3", "METTask_CaloMEy_LS");
   FormatPad( &c3, false );
   comp.Draw("METTask_CaloMEy_LS", mode);   
   SavePlot("METTask_CaloMEy_LS", outdir.c_str() );   
   comp.SetAxis(5);

   comp.SetAxis(1,0,2000);
   TCanvas c3("c3", "METTask_CaloMaxEtInEmTowers");
   FormatPad( &c3, false );
   comp.Draw("METTask_CaloMaxEtInEmTowers", mode);
   SavePlot("METTask_CaloMaxEtInEmTowers", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,2000);
   TCanvas c3("c3", "METTask_CaloMaxEtInHadTowers");
   FormatPad( &c3, false );
   comp.Draw("METTask_CaloMaxEtInHadTowers", mode);
   SavePlot("METTask_CaloMaxEtInHadTowers", outdir.c_str() );
   comp.SetAxis(5);


   comp.SetAxis(1,0,1);
   TCanvas c3("c3", "METTask_CaloEtFractionHadronic");
   FormatPad( &c3, false );
   comp.Draw("METTask_CaloEtFractionHadronic", mode);   
   SavePlot("METTask_CaloEtFractionHadronic", outdir.c_str() );   
   comp.SetAxis(5);

   comp.SetAxis(1,0,1);
   TCanvas c3("c3", "METTask_CaloEmEtFraction");
   FormatPad( &c3, false );
   comp.Draw("METTask_CaloEmEtFraction", mode);
   SavePlot("METTask_CaloEmEtFraction", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,2000);
   TCanvas c3("c3", "METTask_CaloHadEtInHB");
   FormatPad( &c3, false );
   comp.Draw("METTask_CaloHadEtInHB", mode);
   SavePlot("METTask_CaloHadEtInHB", outdir.c_str() );
   comp.SetAxis(5);


   comp.SetAxis(1,0,2000);
   TCanvas c3("c3", "METTask_CaloHadEtInHO");
   FormatPad( &c3, false );
   comp.Draw("METTask_CaloHadEtInHO", mode);   
   SavePlot("METTask_CaloHadEtInHO", outdir.c_str() );   
   comp.SetAxis(5);

   comp.SetAxis(1,0,2000);
   TCanvas c3("c3", "METTask_CaloHadEtInHE");
   FormatPad( &c3, false );
   comp.Draw("METTask_CaloHadEtInHE", mode);
   SavePlot("METTask_CaloHadEtInHE", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,2000);
   TCanvas c3("c3", "METTask_CaloHadEtInHF");
   FormatPad( &c3, false );
   comp.Draw("METTask_CaloHadEtInHF", mode);
   SavePlot("METTask_CaloHadEtInHF", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,2000);
   TCanvas c3("c3", "METTask_CaloEmEtInHF");
   FormatPad( &c3, false );
   comp.Draw("METTask_CaloEmEtInHF", mode);   
   SavePlot("METTask_CaloEmEtInHF", outdir.c_str() );   
   comp.SetAxis(5);

   comp.SetAxis(1,0,2000);
   TCanvas c3("c3", "METTask_CaloEmEtInEE");
   FormatPad( &c3, false );
   comp.Draw("METTask_CaloEmEtInEE", mode);
   SavePlot("METTask_CaloEmEtInEE", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,2000);
   TCanvas c3("c3", "METTask_CaloEmEtInEB");
   FormatPad( &c3, false );
   comp.Draw("METTask_CaloEmEtInEB", mode);
   SavePlot("METTask_CaloEmEtInEB", outdir.c_str() );
   comp.SetAxis(5);

  


}
