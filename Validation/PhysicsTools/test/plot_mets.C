{
  
  gROOT->LoadMacro("../Tools/NicePlot.C");
  InitNicePlot();
  gROOT->LoadMacro("../Tools/Comparator.C");
  //  Manager::Init();

  gStyle->SetOptStat(1111);

  string dirRECO        = "DQMData/Benchmarks/mets/RECO";
  string dirPAT         = "DQMData/Benchmarks/mets/PAT";

  const char* file = "benchmark.root";
  
  bool highpt = false;
  float ptMin = 0;
  float ptMax = 9999;
  sback->SetFillColor(3);
  Style* style1 = sback;
  Style* style2 = s1;
  Comparator::Mode mode = Comparator::SCALE;

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

  
}
