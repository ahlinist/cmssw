void plotL2Efficiency() 
{
  //Get File from tree
    TFile *f = new TFile("tteffAnalysis.root");
    TTree *t = (TTree *) (f->Get("TTEffTree"));

    TCanvas *c1 = new TCanvas("c","Efficiencies");
    c1->Divide(2,4);
    c1->cd(1);
    
    //---Et plot: L2 Tau matching,  Threshold 15, Threshold 15 + EcalIso 5
    TGraphAsymmErrors *EffEtMatch = NTEfficiency(t, "PFTauEt", "hasMatchedL2Jet==1","",20,0,100,kRed, "PF Tau E_{t}","L2 Efficiency","AP");
    TGraphAsymmErrors *EffEtThres15 = NTEfficiency(t, "PFTauEt", "hasMatchedL2Jet==1&&L2JetEt>15","",20,0,100,kGreen,"PF Tau E_{t}","L2 Efficiency","Psame");
    TGraphAsymmErrors *EffEtTh15Iso5 = NTEfficiency(t, "PFTauEt", "hasMatchedL2Jet==1&&L2JetEt>15&&L2ECALIsolationEt<5","",20,0,100,kBlue,"PF Tau E_{t}","L2 Efficiency","Psame");
    
    EffEtMatch->Draw("AP");
    EffEtThres15->Draw("Psame");
    EffEtTh15Iso5->Draw("Psame");
    
    TLegend  *legendEtTh15Iso5  = new TLegend(.75,.80,.98,.98);
    legendEtTh15Iso5->AddEntry(EffEtMatch,"L2 Tau matching","p");
    legendEtTh15Iso5->AddEntry(EffEtThres15,"Th15","p");
    legendEtTh15Iso5->AddEntry(EffEtTh15Iso5,"Th15+EcalIso5","p");
    legendEtTh15Iso5->Draw();
    
    c1->cd(2);
    //----Eta plot: L2 Tau matching,  Threshold 15, Threshold 15 + EcalIso 5 
    TGraphAsymmErrors *EffEtaMatch = NTEfficiency(t, "PFTauEta", "hasMatchedL2Jet==1&&PFTauEt>25","PFTauEt>25",20,-2.5,2.5,kRed, "#eta","L2 Efficiency","AP");
    TGraphAsymmErrors *EffEtaThres15 = NTEfficiency(t, "PFTauEta", "hasMatchedL2Jet==1&&L2JetEt>15&&PFTauEt>25","PFTauEt>25",20,-2.5,2.5,kGreen,"#eta","L2 Efficiency","Psame");
    TGraphAsymmErrors *EffEtaTh15Iso5 = NTEfficiency(t, "PFTauEta", "hasMatchedL2Jet==1&&L2JetEt>15&&L2ECALIsolationEt<5&&PFTauEt>25","PFTauEt>25",20,-2.5,2.5,kBlue,"#eta","L2 Efficiency","Psame");
    
    EffEtaMatch->Draw("AP");
    EffEtaThres15->Draw("Psame");
    EffEtaTh15Iso5->Draw("Psame");    

    TLegend  *legendEtaTh15Iso5  = new TLegend(.75,.80,.98,.98);
    legendEtaTh15Iso5->AddEntry(EffEtaMatch,"L2 Tau matching","p");
    legendEtaTh15Iso5->AddEntry(EffEtaThres15,"Th15","p");
    legendEtaTh15Iso5->AddEntry(EffEtaTh15Iso5,"Th15+EcalIso5","p");
    legendEtaTh15Iso5->Draw();


    c1->cd(3);
    //---Et plot: L2 Tau matching,  Threshold 15, Threshold 15 + EcalIso 3
    TGraphAsymmErrors *EffEtMatch = NTEfficiency(t, "PFTauEt", "hasMatchedL2Jet==1","",20,0,100,kRed, "PF Tau E_{t}","L2 Efficiency","AP");
    TGraphAsymmErrors *EffEtThres15 = NTEfficiency(t, "PFTauEt", "hasMatchedL2Jet==1&&L2JetEt>15","",20,0,100,kGreen,"PF Tau E_{t}","L2 Efficiency","Psame");
    TGraphAsymmErrors *EffEtTh15Iso3 = NTEfficiency(t, "PFTauEt", "hasMatchedL2Jet==1&&L2JetEt>15&&L2ECALIsolationEt<3","",20,0,100,kBlue,"PF Tau E_{t}","L2 Efficiency","Psame");
    
    EffEtMatch->Draw("AP");
    EffEtThres15->Draw("Psame");
    EffEtTh15Iso3->Draw("Psame");
    
    TLegend  *legendEtTh15Iso3  = new TLegend(.75,.80,.98,.98);
    legendEtTh15Iso3->AddEntry(EffEtMatch,"L2 Tau matching","p");
    legendEtTh15Iso3->AddEntry(EffEtThres15,"Th15","p");
    legendEtTh15Iso3->AddEntry(EffEtTh15Iso3,"Th15+EcalIso3","p");
    legendEtTh15Iso3->Draw();
   

    c1->cd(4);
    //----Eta plot: L2 Tau matching,  Threshold 15, Threshold 15 + EcalIso 3
    TGraphAsymmErrors *EffEtaMatch = NTEfficiency(t, "PFTauEta", "hasMatchedL2Jet==1&&PFTauEt>25","PFTauEt>25",20,-2.5,2.5,kRed, "#eta","L2 Efficiency","AP");
    TGraphAsymmErrors *EffEtaThres15 = NTEfficiency(t, "PFTauEta", "hasMatchedL2Jet==1&&L2JetEt>15&&PFTauEt>25","PFTauEt>25",20,-2.5,2.5,kGreen,"#eta","L2 Efficiency","Psame");
    TGraphAsymmErrors *EffEtaTh15Iso3 = NTEfficiency(t, "PFTauEta", "hasMatchedL2Jet==1&&L2JetEt>15&&L2ECALIsolationEt<3&&PFTauEt>25","PFTauEt>25",20,-2.5,2.5,kBlue,"#eta","L2 Efficiency","Psame");

    EffEtaMatch->Draw("AP");
    EffEtaThres15->Draw("Psame");
    EffEtaTh15Iso3->Draw("Psame");
    
    TLegend  *legendEtaTh15Iso3  = new TLegend(.75,.80,.98,.98);
    legendEtaTh15Iso3->AddEntry(EffEtaMatch,"L2 Tau matching","p");
    legendEtaTh15Iso3->AddEntry(EffEtaThres15,"Th15","p");
    legendEtaTh15Iso3->AddEntry(EffEtaTh15Iso3,"Th15+EcalIso3","p");
    legendEtaTh15Iso3->Draw();

    c1->cd(5);
    //---Et plot: L2 Tau matching,  Threshold 20, Threshold 20 + EcalIso 5
    TGraphAsymmErrors *EffEtMatch = NTEfficiency(t, "PFTauEt", "hasMatchedL2Jet==1","",20,0,100,kRed, "PF Tau E_{t}","L2 Efficiency","AP");
    TGraphAsymmErrors *EffEtThres20 = NTEfficiency(t, "PFTauEt", "hasMatchedL2Jet==1&&L2JetEt>20","",20,0,100,kGreen,"PF Tau E_{t}","L2 Efficiency","Psame");
    TGraphAsymmErrors *EffEtTh20Iso5 = NTEfficiency(t, "PFTauEt", "hasMatchedL2Jet==1&&L2JetEt>20&&L2ECALIsolationEt<5","",20,0,100,kBlue,"PF Tau E_{t}","L2 Efficiency","Psame");

    EffEtMatch->Draw("AP");
    EffEtThres20->Draw("Psame");
    EffEtTh20Iso5->Draw("Psame");
    
    TLegend  *legendEtTh20Iso5  = new TLegend(.75,.80,.98,.98);
    legendEtTh20Iso5->AddEntry(EffEtMatch,"L2 Tau matching","p");
    legendEtTh20Iso5->AddEntry(EffEtThres15,"Th20","p");
    legendEtTh20Iso5->AddEntry(EffEtTh15Iso3,"Th20+EcalIso5","p");
    legendEtTh20Iso5->Draw();
    
    c1->cd(6);
    //----Eta plot: L2 Tau matching,  Threshold 20, Threshold 20 + EcalIso 5
    TGraphAsymmErrors *EffEtMatch = NTEfficiency(t, "PFTauEta", "hasMatchedL2Jet==1&&PFTauEt>25","PFTauEt>25",20,-2.5,2.5,kRed, "#eta","L2 Efficiency","AP");
    TGraphAsymmErrors *EffEtThres20 = NTEfficiency(t, "PFTauEta", "hasMatchedL2Jet==1&&L2JetEt>20&&PFTauEt>25","PFTauEt>25",20,-2.5,2.5,kGreen,"#eta","L2 Efficiency","Psame");
    TGraphAsymmErrors *EffEtTh20Iso5 = NTEfficiency(t, "PFTauEta", "hasMatchedL2Jet==1&&L2JetEt>20&&L2ECALIsolationEt<5&&PFTauEt>25","PFTauEt>25",20,-2.5,2.5,kBlue,"#eta","L2 Efficiency","Psame");

    EffEtMatch->Draw("AP");
    EffEtThres20->Draw("Psame");
    EffEtTh20Iso5->Draw("Psame");
    
    TLegend  *legendEtaTh20Iso5  = new TLegend(.75,.80,.98,.98);
    legendEtaTh20Iso5->AddEntry(EffEtaMatch,"L2 Tau matching","p");
    legendEtaTh20Iso5->AddEntry(EffEtaThres15,"Th20","p");
    legendEtaTh20Iso5->AddEntry(EffEtaTh15Iso3,"Th20+EcalIso5","p");
    legendEtaTh20Iso5->Draw();

    c1->cd(7);
    //---Et plot: L2 Tau matching,  Threshold 20, Threshold 20 + EcalIso 3
    TGraphAsymmErrors *EffEtMatch = NTEfficiency(t, "PFTauEt", "hasMatchedL2Jet==1","",20,0,100,kRed, "PF Tau E_{t}","L2 Efficiency","AP");
    TGraphAsymmErrors *EffEtThres20 = NTEfficiency(t, "PFTauEt", "hasMatchedL2Jet==1&&L2JetEt>20","",20,0,100,kGreen,"PF Tau E_{t}","L2 Efficiency","Psame");
    TGraphAsymmErrors *EffEtTh20Iso3 = NTEfficiency(t, "PFTauEt", "hasMatchedL2Jet==1&&L2JetEt>20&&L2ECALIsolationEt<3","",20,0,100,kBlue,"PF Tau E_{t}","L2 Efficiency","Psame");

    EffEtMatch->Draw("AP");
    EffEtThres20->Draw("Psame");
    EffEtTh20Iso3->Draw("Psame");

    TLegend  *legendEtTh20Iso3  = new TLegend(.75,.80,.98,.98);
    legendEtTh20Iso3->AddEntry(EffEtMatch,"L2 Tau matching","p");
    legendEtTh20Iso3->AddEntry(EffEtThres15,"Th20","p");
    legendEtTh20Iso3->AddEntry(EffEtTh15Iso3,"Th20+EcalIso3","p");
    legendEtTh20Iso3->Draw();
    
    c1->cd(8);
    //----Eta plot: L2 Tau matching,  Threshold 20, Threshold 20 + EcalIso 3
    TGraphAsymmErrors *EffEtMatch = NTEfficiency(t, "PFTauEta", "hasMatchedL2Jet==1&&PFTauEt>25","PFTauEt>25",20,-2.5,2.5,kRed, "#eta","L2 Efficiency","AP");
    TGraphAsymmErrors *EffEtThres20 = NTEfficiency(t, "PFTauEta", "hasMatchedL2Jet==1&&L2JetEt>20&&PFTauEt>25","PFTauEt>25",20,-2.5,2.5,kGreen,"#eta","L2 Efficiency","Psame");
    TGraphAsymmErrors *EffEtTh20Iso3 = NTEfficiency(t, "PFTauEta", "hasMatchedL2Jet==1&&L2JetEt>20&&L2ECALIsolationEt<3&&PFTauEt>25","PFTauEt>25",20,-2.5,2.5,kBlue,"#eta","L2 Efficiency","Psame");

    EffEtMatch->Draw("AP");
    EffEtThres20->Draw("Psame");
    EffEtTh20Iso3->Draw("Psame");
    
    TLegend  *legendEtaTh20Iso3  = new TLegend(.75,.80,.98,.98);
    legendEtaTh20Iso3->AddEntry(EffEtaMatch,"L2 Tau matching","p");
    legendEtaTh20Iso3->AddEntry(EffEtaThres15,"Th20","p");
    legendEtaTh20Iso3->AddEntry(EffEtaTh15Iso3,"Th20+EcalIso3","p");
    legendEtaTh20Iso3->Draw();

    c1->Draw();
    c1->SaveAs("Efficiencies1.png");
   
}

TGraphAsymmErrors*
NTEfficiency(TTree* t, std::string VS1 ,char* num,char* denom,int bins,double min,double max,Color_t color = kBlack, char* titlex = "", char* titley = "", char* drawOpt =="AP")
{

  TH1F *h1 = new TH1F("h1","h1",bins,min,max);
  TH1F *h2 = new TH1F("h2","h2",bins,min,max);

  t->Draw((VS1+">>h2").c_str(),denom);
  t->Draw( (VS1+">>h1").c_str(),num);

  h1->Sumw2();
  h2->Sumw2();

  TGraphAsymmErrors* g1 = new TGraphAsymmErrors(h1,h2);
  g1->SetName("Eff");
  g1->BayesDivide(h1,h2);

  h1->Delete();
  h2->Delete();

  g1->GetXaxis()->SetLabelSize(0.09);
  g1->GetXaxis()->SetNdivisions(509);
  g1->GetYaxis()->SetNdivisions(509);
  g1->GetYaxis()->SetLabelSize(0.09);
  g1->GetXaxis()->SetTitleSize(0.06);
  g1->GetYaxis()->SetTitleSize(0.06);
  g1->GetYaxis()->SetTitleOffset(0.85);
  g1->GetYaxis()->SetRangeUser(0,1); 
  g1->SetMarkerColor(color);
  g1->SetMarkerStyle(20);
  g1->GetXaxis()->SetTitle(titlex);
  g1->GetYaxis()->SetTitle(titley);
  g1->Draw(drawOpt);
  return g1;
}


