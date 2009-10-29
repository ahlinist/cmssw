{
  
  gROOT->LoadMacro("../Tools/NicePlot.C");
  InitNicePlot();
  gROOT->LoadMacro("../Tools/Comparator.C");
  //  Manager::Init();

  gStyle->SetOptStat(1111);

  string dirRECO        = "DQMData/Benchmarks/taus/RECO";
  string dirPAT         = "DQMData/Benchmarks/taus/PAT";

  const char* file = "benchmark.root";

  bool highpt = false;
  float ptMin = 0;
  float ptMax = 9999;
  //sback->SetFillColor(4);
  Style* style1 = sback;
  Style* style2 = s1;
  Comparator::Mode mode = Comparator::NORMAL;

//  string outdir = "Jets";
    string outdir ="."; 

  Comparator comp(file,
		  dirPAT.c_str(),
		  file,
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
   comp.Draw("eta", mode);
   SavePlot("tau_eta", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1, -4,4);
   TCanvas c3("c3", "phi benchmark");
   FormatPad( &c3, false );
   comp.Draw("phi", mode);
   SavePlot("tau_phi", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1, 0,450);
   TCanvas c3("c3", "energy benchmark");
   FormatPad( &c3, false );
   comp.Draw("energy", mode);
   SavePlot("tau_energy", outdir.c_str() );
   comp.SetAxis(5);


   comp.SetAxis(1, 0,500);
   TCanvas c3("c3", "PF Pt of Highest Pt charged particle within the tau-jet benchmark");
   FormatPad( &c3, false );
   comp.Draw("PF Pt of Highest Pt charged particle within the tau-jet", mode);
   SavePlot("PF_Pt_of_Highest_Pt_charged_particle_within_the_tau-jet", outdir.c_str() );
   comp.SetAxis(5);


   comp.SetAxis(1, 0,500);
   TCanvas c3("c3", "PF Pt of the highest Pt photon within the tau-jet");
   FormatPad( &c3, false );
   comp.Draw("PF Pt of the highest Pt photon within the tau-jet", mode);
   SavePlot("PF_Pt_of_the_highest_Pt_photon_within_the_tau-jet", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1, 0,500);
   TCanvas c3("c3", "PF Pt of the highest Pt particle (of any type) within the tau-jet");
   FormatPad( &c3, false );
   comp.Draw("PF Pt of the highest Pt particle (of any type) within the tau-jet", mode);
   SavePlot("PF_Pt_of_the_highest_Pt_particle_of_any_type_within_the_tau-jet", outdir.c_str() );
   comp.SetAxis(5);


  
}
