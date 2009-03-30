{
  
  gROOT->LoadMacro("../Tools/NicePlot.C");
  InitNicePlot();
  gROOT->LoadMacro("../Tools/Comparator.C");
  //  Manager::Init();

  gStyle->SetOptStat(1111);

  string dirRECO         = "DQMData/Benchmarks/jets/RECO";
  string dirPAT          = "DQMData/Benchmarks/jets/PAT";
//  string dirRECOmuons        = "DQMData/Benchmarks/muons/RECO";
//  string dirPATmuons         = "DQMData/Benchmarks/muons/PAT";

  const char* file = "benchmark.root";

  enum Modes {
    JET,
    MUON,
    ELECTRON,
    MET,
    PHOTON,
    TAU,
    
  };
  
  bool highpt = false;
  float ptMin = 0;
  float ptMax = 9999;
  Style* style1 = sback;
  Style* style2 = s1;
  Comparator::Mode mode = Comparator::SCALE;

   int etamode = TAU;

   string outdir = "Jets";

  switch( etamode ) {
  case JET:
    dirRECO= "DQMData/Benchmarks/jets/RECO";
    dirPAT  = "DQMData/Benchmarks/jets/PAT";
    outdir = ".";
    break;
  case MUON:
    dirRECO = "DQMData/Benchmarks/muons/RECO";
    dirPAT  = "DQMData/Benchmarks/muons/PAT";
    outdir = ".";
    break;
    default:
    break;
  }   

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
   SavePlot("pt", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1, -4,4);
   TCanvas c2("c2", "eta benchmark");
   FormatPad( &c2, false );
   comp.Draw("eta", mode);
   SavePlot("eta", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1, -4,4);
   TCanvas c3("c3", "phi benchmark");
   FormatPad( &c3, false );
   comp.Draw("phi", mode);
   SavePlot("phi", outdir.c_str() );
   comp.SetAxis(5);


//   TCanvas c2("c2", "Muon benchmark");
//   FormatPad( &c1, false );
//   comp.SetAxis(5); 
//   comp.Draw("muons/pt", mode);
//   SavePlot("pt", outdir.c_str() );

  
}
