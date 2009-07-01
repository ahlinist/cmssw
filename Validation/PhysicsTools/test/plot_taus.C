{
  
  gROOT->LoadMacro("../Tools/NicePlot.C");
  InitNicePlot();
  gROOT->LoadMacro("../Tools/Comparator.C");
  //  Manager::Init();

  gStyle->SetOptStat(1111);

  string dirRECO        = "DQMData/Benchmarks/taus/RECO";
  string dirPAT         = "DQMData/Benchmarks/taus/PAT";

  const char* file1 = "benchmark1.root";
  const char* file2 = "benchmark2.root";  

  bool highpt = false;
  float ptMin = 0;
  float ptMax = 9999;
  sback->SetFillColor(4);
  Style* style1 = sback;
  Style* style2 = s1;
  Comparator::Mode mode = Comparator::NORMAL;

//  string outdir = "Jets";
    string outdir ="."; 

  Comparator comp(file1,
		  dirPAT.c_str(),
		  file2,
		  dirRECO.c_str());
  comp.SetStyles(style1, style2, "RECO Objects", "PAT Objects");


   TCanvas c0("c0", "legend", 400, 200);    
   FormatPad( &c0, false ); 
   comp.Legend().Draw();
   SavePlot("legend", outdir.c_str() );
    
   comp.SetAxis(1, -30,100);
   TCanvas c1("c1", "pt benchmark");
   FormatPad( &c1, false ); 
   comp.Draw("pt", mode);
   SavePlot("tau_pt", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1, -6,6);
   TCanvas c2("c2", "eta benchmark");
   FormatPad( &c2, false );
//    TH1* h0 = Histo( key, 0);
//    TH1* h1 = Histo( key, 1);
//    double pv = h1->KolmogorovTest(h0, "OU");
//   double pv = TH1* h1->KolmogorovTest(TH1* h0, "OU");
//   gStyle->SetStatColor(9);

   comp.Draw("eta", mode);
   SavePlot("tau_eta", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1, -4,4);
   TCanvas c3("c3", "phi benchmark");
   FormatPad( &c3, false );
   comp.Draw("phi", mode);
   SavePlot("tau_phi", outdir.c_str() );
   comp.SetAxis(5);
  
}
