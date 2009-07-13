{
  
  gROOT->LoadMacro("../Tools/NicePlot.C");
  InitNicePlot();
  gROOT->LoadMacro("../Tools/Comparator.C");
  //  Manager::Init();

  gStyle->SetOptStat(1111);

  string dirRECO        = "DQMData/Benchmarks/mets/MET/RECO";
  string dirPAT         = "DQMData/Benchmarks/mets/MET/PAT";

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
   TCanvas c3("c3", "METTask_MEx");
   FormatPad( &c3, false );
   comp.Draw("METTask_MEx", mode);
   SavePlot("METTask_MEx", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-500,500); 
   TCanvas c3("c3", "METTask_MEy");
   FormatPad( &c3, false );
   comp.Draw("METTask_MEy", mode);
   SavePlot("METTask_MEy", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-500,500);
   TCanvas c3("c3", "METTask_Ez");
   FormatPad( &c3, false );
   comp.Draw("METTask_Ez", mode);
   SavePlot("METTask_Ez", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,51);
   TCanvas c3("c3", "METTask_METSig");
   FormatPad( &c3, false );
   comp.Draw("METTask_METSig", mode);
   SavePlot("METTask_METSig", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0,1000);
   TCanvas c3("c3", "METTask_MET");
   FormatPad( &c3, false );
   comp.Draw("METTask_MET", mode);
   SavePlot("METTask_MET", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-4,4);
   TCanvas c3("c3", "METTask_METPhi");
   FormatPad( &c3, false );
   comp.Draw("METTask_METPhi", mode);   
   SavePlot("METTask_METPhi", outdir.c_str() );   
   comp.SetAxis(5);

   comp.SetAxis(1,0,2000);
   TCanvas c3("c3", "METTask_SumET");
   FormatPad( &c3, false );
   comp.Draw("METTask_SumET", mode);
   SavePlot("METTask_SumET", outdir.c_str() );
   comp.SetAxis(5);


}
