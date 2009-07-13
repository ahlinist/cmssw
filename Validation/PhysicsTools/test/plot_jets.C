{
  
  gROOT->LoadMacro("../Tools/NicePlot.C");
  InitNicePlot();
  gROOT->LoadMacro("../Tools/Comparator.C");
  //  Manager::Init();

  gStyle->SetOptStat(1111);

  string dirRECO        = "DQMData/Benchmarks/iterativeCone5CaloJets_Corr/RECO";
  string dirPAT         = "DQMData/Benchmarks/iterativeCone5CaloJets_Corr/PAT";
//   string dirRECO        = "DQMData/Benchmarks/L2L3CorJetSC5Calo/RECO";
//   string dirPAT         = "DQMData/Benchmarks/L2L3CorJetSC5Calo/PAT";
//  string dirRECO        = "DQMData/Benchmarks/L2L3CorJetSC7Calo/RECO";
//  string dirPAT         = "DQMData/Benchmarks/L2L3CorJetSC7Calo/PAT";



  const char* file = "benchmark.root";
  
  bool highpt = false;
  float ptMin = 0;
  float ptMax = 9999;
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


   comp.SetAxis(1,0 ,500);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("E", mode);
   SavePlot("E", outdir.c_str() );
   comp.SetAxis(5);
    

   comp.SetAxis(1,0 ,4500);   
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("E_80", mode);
   SavePlot("E_80", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1, 0,6000);   
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("E_3000", mode);
   SavePlot("E_3000", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0 ,500);   
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("P", mode);
   SavePlot("P", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0 ,4500);   
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("P_80", mode);
   SavePlot("P_80", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0 ,6000);   
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("P_3000", mode);
   SavePlot("P_3000", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0 ,50);   
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("Pt", mode);
   SavePlot("Pt", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0 ,140);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("Pt_80", mode);
   SavePlot("Pt_80", outdir.c_str() );
   comp.SetAxis(5);  
   
   comp.SetAxis(1,0 ,4000);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("Pt_3000", mode);
   SavePlot("Pt_3000", outdir.c_str() );
   comp.SetAxis(5);
   
   comp.SetAxis(1,0 ,25);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("Mass", mode);
   SavePlot("Mass", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0 ,120);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("Mass_80", mode);
   SavePlot("Mass_80", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0 ,1500);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("Mass_3000", mode);
   SavePlot("Mass_3000", outdir.c_str() );
   comp.SetAxis(5);
   
   comp.SetAxis(1,-5 ,5);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("Eta", mode);
   SavePlot("Eta", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,-3.5 ,3.5);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("Phi", mode);
   SavePlot("Phi", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0, 100);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("Constituents", mode);
   SavePlot("Constituents", outdir.c_str() );
   comp.SetAxis(5);
 
   comp.SetAxis(1,0 ,50);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("EmEnergyInEB", mode);
   SavePlot("EmEnergyInEB", outdir.c_str() );
   comp.SetAxis(5);
  
   comp.SetAxis(1,0 ,200);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("EmEnergyInEB_80", mode);
   SavePlot("EmEnergyInEB_80", outdir.c_str() );
   comp.SetAxis(5);
 
    comp.SetAxis(1,0 ,3000);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("EmEnergyInEB_3000", mode);
   SavePlot("EmEnergyInEB_3000", outdir.c_str() );
   comp.SetAxis(5);
   
   comp.SetAxis(1,0 ,50);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("EmEnergyInEE", mode);
   SavePlot("EmEnergyInEE", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0 ,1000);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("EmEnergyInEE_80", mode);
   SavePlot("EmEnergyInEE_80", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0 ,2000);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("EmEnergyInEE_3000", mode);
   SavePlot("EmEnergyInEE_3000", outdir.c_str() );
   comp.SetAxis(5);
 
   comp.SetAxis(1,-20,100);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("EmEnergyInHF", mode);
   SavePlot("EmEnergyInHF", outdir.c_str() );
   comp.SetAxis(5);
  
   comp.SetAxis(1,-0.1,1.1);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("EnergyFractionEm", mode);
   SavePlot("EnergyFractionEm", outdir.c_str() );
   comp.SetAxis(5);
 

    comp.SetAxis(1,0 ,100);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("MaxEInEmTowers", mode);
   SavePlot("MaxEInEmTowers", outdir.c_str() );
   comp.SetAxis(5);
   
   comp.SetAxis(1,0 ,50);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("HadEnergyInHB", mode);
   SavePlot("HadEnergyInHB", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0 ,200);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("HadEnergyInHB_80", mode);
   SavePlot("HadEnergyInHB_80", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0 ,3000);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("HadEnergyInHB_3000", mode);
   SavePlot("HadEnergyInHB_3000", outdir.c_str() );
   comp.SetAxis(5);
 
   comp.SetAxis(1,0 ,100);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("HadEnergyInHE", mode);
   SavePlot("HadEnergyInHE", outdir.c_str() );
   comp.SetAxis(5);
  
   comp.SetAxis(1,0 ,1000);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("HadEnergyInHE_80", mode);
   SavePlot("HadEnergyInHE_80", outdir.c_str() );
   comp.SetAxis(5);
 
    comp.SetAxis(1,0 ,2000);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("HadEnergyInHE_3000", mode);
   SavePlot("HadEnergyInHE_3000", outdir.c_str() );
   comp.SetAxis(5);
   
   comp.SetAxis(1,0 ,10);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("HadEnergyInHO", mode);
   SavePlot("HadEnergyInHO", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0, 50);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("HadEnergyInHO_80", mode);
   SavePlot("HadEnergyInHO_80", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0 ,500);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("HadEnergyInHO_3000", mode);
   SavePlot("HadEnergyInHO_3000", outdir.c_str() );
   comp.SetAxis(5);
 
   comp.SetAxis(1,0 ,50);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("HadEnergyInHF", mode);
   SavePlot("HadEnergyInHF", outdir.c_str() );
   comp.SetAxis(5);
  
   comp.SetAxis(1,-0.1 ,1.1);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("EnergyFractionHadronic", mode);
   SavePlot("EnergyFractionHadronic", outdir.c_str() );
   comp.SetAxis(5);
   
   comp.SetAxis(1,0 ,100);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("MaxEInHadTowers", mode);
   SavePlot("MaxEInHadTowers", outdir.c_str() );
   comp.SetAxis(5);
   
   comp.SetAxis(1,0 ,500);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("HFLong", mode);
   SavePlot("HFLong", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0 ,200);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("HFLong_80", mode);
   SavePlot("HFLong_80", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0 ,1500);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("HFLong_3000", mode);
   SavePlot("HFLong_3000", outdir.c_str() );
   comp.SetAxis(5);
 
   comp.SetAxis(1,0 ,500);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("HFShort", mode);
   SavePlot("HFShort", outdir.c_str() );
   comp.SetAxis(5);
  
   comp.SetAxis(1,0 ,200);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("HFShort_80", mode);
   SavePlot("HFShort_80", outdir.c_str() );
   comp.SetAxis(5);
 
    comp.SetAxis(1,0 ,1500);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("HFShort_3000", mode);
   SavePlot("HFShort_3000", outdir.c_str() );
   comp.SetAxis(5);
   
   comp.SetAxis(1,0 ,500);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("HFTotal", mode);
   SavePlot("HFTotal", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0 ,3000);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("HFTotal_80", mode);
   SavePlot("HFTotal_80", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0 ,6000);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("HFTotal_3000", mode);
   SavePlot("HFTotal_3000", outdir.c_str() );
   comp.SetAxis(5);
 
   comp.SetAxis(1,0 ,50);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("N90", mode);
   SavePlot("N90", outdir.c_str() );
   comp.SetAxis(5);
  
   comp.SetAxis(1,0 ,1000);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("EFirst", mode);
   SavePlot("EFirst", outdir.c_str() );
   comp.SetAxis(5);
 
   comp.SetAxis(1,0 ,500);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("PtFirst", mode);
   SavePlot("PtFirst", outdir.c_str() );
   comp.SetAxis(5);
   
   comp.SetAxis(1,-5 ,5);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("EtaFirst", mode);
   SavePlot("EtaFirst", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1, -3.5,3.5);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("PhiFirst", mode);
   SavePlot("PhiFirst", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1, -1,1);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("DelPt", mode);
   SavePlot("DelPt", outdir.c_str() );
   comp.SetAxis(5);
 
   comp.SetAxis(1,-0.5 ,0.5);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("DelEta", mode);
   SavePlot("DelEta", outdir.c_str() );
   comp.SetAxis(5);
  
   comp.SetAxis(1,-0.5 ,0.5);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("DelPhi", mode);
   SavePlot("DelPhi", outdir.c_str() );
   comp.SetAxis(5);
 
   
   comp.SetAxis(1,0 ,2000);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("Mjj", mode);
   SavePlot("Mjj", outdir.c_str() );
   comp.SetAxis(5);

   comp.SetAxis(1,0, 10000);
   TCanvas c1("c1", "benchmark");
   FormatPad( &c1, false );
   comp.Draw("Mjj_3000", mode);
   SavePlot("Mjj_3000", outdir.c_str() );
   comp.SetAxis(5);
  
}
