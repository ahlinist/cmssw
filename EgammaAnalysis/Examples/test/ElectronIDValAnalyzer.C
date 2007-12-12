//file1, file2 and file3 are the name of the root files storing plots
//for loose, medium and tight identification respectively
//min(max) is the lower(upper) limit on the electron pt for the X axis

void electronIdPlots (const char* _file1, const char* _file2, const char* _file3, double min, double max){
  
  gStyle->SetTitle(0);
  gStyle->SetOptStat(0000000);

  TFile* file1 = new TFile (_file1);
  TFile* file2 = new TFile (_file2);
  TFile* file3 = new TFile (_file3);

  //Efficiency of Reconstruction and Reconstruction+Identification

  TCanvas *cEfficiencyPT = new TCanvas("cRecEfficiencyPT","Electron reconstruction & identification Efficiency vs PT");
  //cEfficiency->Divide(1,4);
 
  file1->cd("Reconstruction");
  TGraphAsymmErrors * gEleEffPTRec = (TGraphAsymmErrors*)gDirectory->Get("EfficiencyReconstructionPTGraph");
  //cEfficiency->cd(1);
  gEleEffPTRec->SetLineStyle(1);
  gEleEffPTRec->SetLineColor(1);
  gEleEffPTRec->SetMarkerSize(0.5);
  gEleEffPTRec->SetMarkerStyle(23);
  gEleEffPTRec->SetMarkerColor(1);
  gEleEffPTRec->Draw("AP");
  gEleEffPTRec->GetXaxis()->SetTitle("GEN electron p_{T}");
  gEleEffPTRec->GetXaxis()->SetRangeUser(min,max);
  gEleEffPTRec->GetYaxis()->SetTitle("Efficiency");
  gEleEffPTRec->GetYaxis()->SetRangeUser(0,1.1);

  file1->cd("Rec&Id");
  TGraphAsymmErrors * gEleEffPTRec1 = (TGraphAsymmErrors*)gDirectory->Get("EfficiencyRecIdentificationPTGraph");
  //cEfficiency->cd(1);
  gEleEffPTRec1->SetLineStyle(1);
  gEleEffPTRec1->SetLineColor(4);
  gEleEffPTRec1->SetMarkerSize(0.5);
  gEleEffPTRec1->SetMarkerStyle(26);
  gEleEffPTRec1->SetMarkerColor(4);
  gEleEffPTRec1->Draw("P");

  file2->cd("Rec&Id");
  TGraphAsymmErrors * gEleEffPTRec2 = (TGraphAsymmErrors*)gDirectory->Get("EfficiencyRecIdentificationPTGraph");
  //cEfficiency->cd(1);
  gEleEffPTRec2->SetLineStyle(1);
  gEleEffPTRec2->SetLineColor(8);
  gEleEffPTRec2->SetMarkerSize(0.5);
  gEleEffPTRec2->SetMarkerStyle(20);
  gEleEffPTRec2->SetMarkerColor(8);
  gEleEffPTRec2->Draw("P");

  file3->cd("Rec&Id");
  TGraphAsymmErrors * gEleEffPTRec3 = (TGraphAsymmErrors*)gDirectory->Get("EfficiencyRecIdentificationPTGraph");
  //cEfficiency->cd(1);
  gEleEffPTRec3->SetLineStyle(1);
  gEleEffPTRec3->SetLineColor(2);
  gEleEffPTRec3->SetMarkerSize(0.5);
  gEleEffPTRec3->SetMarkerStyle(21);
  gEleEffPTRec3->SetMarkerColor(2);
  gEleEffPTRec3->Draw("P");

  TLegend* legA = new TLegend(0.6,0.2,0.85,0.4);
  legA->AddEntry(gEleEffPTRec,"Reconstruction","l"); 
  legA->AddEntry(gEleEffPTRec1,"Rec&Id loose","l"); 
  legA->AddEntry(gEleEffPTRec2,"Rec&Id medium","l"); 
  legA->AddEntry(gEleEffPTRec3,"Rec&Id tight","l"); 
  legA->SetFillColor(0);   
  legA->SetTextSize(0.04);   
  legA->Draw();
  gPad->Update();

  TCanvas *cEfficiencyEta = new TCanvas("cRecEfficiencyEta","Electron reconstruction & identification Efficiency vs Eta");
  //cEfficiency->Divide(1,4);
 
  file1->cd("Reconstruction");
  TGraphAsymmErrors * gEleEffEtaRec = (TGraphAsymmErrors*)gDirectory->Get("EfficiencyReconstructionEtaGraph");
  //cEfficiency->cd(1);
  gEleEffEtaRec->SetLineStyle(1);
  gEleEffEtaRec->SetLineColor(1);
  gEleEffEtaRec->SetMarkerSize(0.5);
  gEleEffEtaRec->SetMarkerStyle(23);
  gEleEffEtaRec->SetMarkerColor(1);
  gEleEffEtaRec->Draw("AP");
  gEleEffEtaRec->GetXaxis()->SetTitle("GEN electron #eta");
  gEleEffEtaRec->GetYaxis()->SetTitle("Efficiency");
  gEleEffEtaRec->GetYaxis()->SetRangeUser(0,1.1);

  file1->cd("Rec&Id");
  TGraphAsymmErrors * gEleEffEtaRec1 = (TGraphAsymmErrors*)gDirectory->Get("EfficiencyRecIdentificationEtaGraph");
  //cEfficiency->cd(1);
  gEleEffEtaRec1->SetLineStyle(1);
  gEleEffEtaRec1->SetLineColor(4);
  gEleEffEtaRec1->SetMarkerSize(0.5);
  gEleEffEtaRec1->SetMarkerStyle(26);
  gEleEffEtaRec1->SetMarkerColor(4);
  gEleEffEtaRec1->Draw("P");

  file2->cd("Rec&Id");
  TGraphAsymmErrors * gEleEffEtaRec2 = (TGraphAsymmErrors*)gDirectory->Get("EfficiencyRecIdentificationEtaGraph");
  //cEfficiency->cd(1);
  gEleEffEtaRec2->SetLineStyle(1);
  gEleEffEtaRec2->SetLineColor(8);
  gEleEffEtaRec2->SetMarkerSize(0.5);
  gEleEffEtaRec2->SetMarkerStyle(20);
  gEleEffEtaRec2->SetMarkerColor(8);
  gEleEffEtaRec2->Draw("P");

  file3->cd("Rec&Id");
  TGraphAsymmErrors * gEleEffEtaRec3 = (TGraphAsymmErrors*)gDirectory->Get("EfficiencyRecIdentificationEtaGraph");
  //cEfficiency->cd(1);
  gEleEffEtaRec3->SetLineStyle(1);
  gEleEffEtaRec3->SetLineColor(2);
  gEleEffEtaRec3->SetMarkerSize(0.5);
  gEleEffEtaRec3->SetMarkerStyle(21);
  gEleEffEtaRec3->SetMarkerColor(2);
  gEleEffEtaRec3->Draw("P");

  TLegend* legA1 = new TLegend(0.6,0.2,0.85,0.4);
  legA1->AddEntry(gEleEffEtaRec,"Reconstruction","l"); 
  legA1->AddEntry(gEleEffEtaRec1,"Rec&Id loose","l"); 
  legA1->AddEntry(gEleEffEtaRec2,"Rec&Id medium","l"); 
  legA1->AddEntry(gEleEffEtaRec3,"Rec&Id tight","l"); 
  legA1->SetFillColor(0);   
  legA1->SetTextSize(0.04);   
  legA1->Draw();
  gPad->Update();


  TCanvas *cEfficiencyPhi = new TCanvas("cRecEfficiencyPhi","Electron reconstruction & identification Efficiency vs Phi");
  //cEfficiency->Divide(1,4);
 
  file1->cd("Reconstruction");
  TGraphAsymmErrors * gEleEffPhiRec = (TGraphAsymmErrors*)gDirectory->Get("EfficiencyReconstructionPhiGraph");
  //cEfficiency->cd(1);
  gEleEffPhiRec->SetLineStyle(1);
  gEleEffPhiRec->SetLineColor(1);
  gEleEffPhiRec->SetMarkerSize(0.5);
  gEleEffPhiRec->SetMarkerStyle(23);
  gEleEffPhiRec->SetMarkerColor(1);
  gEleEffPhiRec->Draw("AP");
  gEleEffPhiRec->GetXaxis()->SetTitle("GEN electron #phi");
  gEleEffPhiRec->GetYaxis()->SetTitle("Efficiency");
  gEleEffPhiRec->GetYaxis()->SetRangeUser(0,1.1);

  file1->cd("Rec&Id");
  TGraphAsymmErrors * gEleEffPhiRec1 = (TGraphAsymmErrors*)gDirectory->Get("EfficiencyRecIdentificationPhiGraph");
  //cEfficiency->cd(1);
  gEleEffPhiRec1->SetLineStyle(1);
  gEleEffPhiRec1->SetLineColor(4);
  gEleEffPhiRec1->SetMarkerSize(0.5);
  gEleEffPhiRec1->SetMarkerStyle(26);
  gEleEffPhiRec1->SetMarkerColor(4);
  gEleEffPhiRec1->Draw("P");

  file2->cd("Rec&Id");
  TGraphAsymmErrors * gEleEffPhiRec2 = (TGraphAsymmErrors*)gDirectory->Get("EfficiencyRecIdentificationPhiGraph");
  //cEfficiency->cd(1);
  gEleEffPhiRec2->SetLineStyle(1);
  gEleEffPhiRec2->SetLineColor(8);
  gEleEffPhiRec2->SetMarkerSize(0.5);
  gEleEffPhiRec2->SetMarkerStyle(20);
  gEleEffPhiRec2->SetMarkerColor(8);
  gEleEffPhiRec2->Draw("P");

  file3->cd("Rec&Id");
  TGraphAsymmErrors * gEleEffPhiRec3 = (TGraphAsymmErrors*)gDirectory->Get("EfficiencyRecIdentificationPhiGraph");
  //cEfficiency->cd(1);
  gEleEffPhiRec3->SetLineStyle(1);
  gEleEffPhiRec3->SetLineColor(2);
  gEleEffPhiRec3->SetMarkerSize(0.5);
  gEleEffPhiRec3->SetMarkerStyle(21);
  gEleEffPhiRec3->SetMarkerColor(2);
  gEleEffPhiRec3->Draw("P");

  TLegend* legA2 = new TLegend(0.6,0.2,0.85,0.4);
  legA2->AddEntry(gEleEffPhiRec,"Reconstruction","l"); 
  legA2->AddEntry(gEleEffPhiRec1,"Rec&Id loose","l"); 
  legA2->AddEntry(gEleEffPhiRec2,"Rec&Id medium","l"); 
  legA2->AddEntry(gEleEffPhiRec3,"Rec&Id tight","l"); 
  legA2->SetFillColor(0);   
  legA2->SetTextSize(0.04);   
  legA2->Draw();
  gPad->Update();




  //Efficiency Identification
  TCanvas *cEffIdPT = new TCanvas("cEffIdPT","Electron identification Efficiency vs PT");
  //cEfficiency->Divide(1,4);
 
  file1->cd("Identification");
  TGraphAsymmErrors * gEleEffPTId1 = (TGraphAsymmErrors*)gDirectory->Get("EfficiencyIdentificationPTGraph");
  //cEfficiency->cd(1);
  gEleEffPTId1->SetLineStyle(1);
  gEleEffPTId1->SetLineColor(4);
  gEleEffPTId1->SetMarkerSize(0.5);
  gEleEffPTId1->SetMarkerStyle(26);
  gEleEffPTId1->SetMarkerColor(4);
  gEleEffPTId1->Draw("AP");
  gEleEffPTId1->GetXaxis()->SetTitle("GEN electron p_{T}");
  gEleEffPTId1->GetXaxis()->SetRangeUser(min,max);
  gEleEffPTId1->GetYaxis()->SetTitle("Efficiency");
  gEleEffPTId1->GetYaxis()->SetRangeUser(0,1.1);


  file2->cd("Identification");
  TGraphAsymmErrors * gEleEffPTId2 = (TGraphAsymmErrors*)gDirectory->Get("EfficiencyIdentificationPTGraph");
  //cEfficiency->cd(1);
  gEleEffPTId2->SetLineStyle(1);
  gEleEffPTId2->SetLineColor(8);
  gEleEffPTId2->SetMarkerSize(0.5);
  gEleEffPTId2->SetMarkerStyle(20);
  gEleEffPTId2->SetMarkerColor(8);
  gEleEffPTId2->Draw("P");

  file3->cd("Identification");
  TGraphAsymmErrors * gEleEffPTId3 = (TGraphAsymmErrors*)gDirectory->Get("EfficiencyIdentificationPTGraph");
  //cEfficiency->cd(1);
  gEleEffPTId3->SetLineStyle(1);
  gEleEffPTId3->SetLineColor(2);
  gEleEffPTId3->SetMarkerSize(0.5);
  gEleEffPTId3->SetMarkerStyle(21);
  gEleEffPTId3->SetMarkerColor(2);
  gEleEffPTId3->Draw("P");

  TLegend* legB = new TLegend(0.6,0.2,0.85,0.4);
  legB->AddEntry(gEleEffPTId1,"Id loose","l"); 
  legB->AddEntry(gEleEffPTId2,"Id medium","l"); 
  legB->AddEntry(gEleEffPTId3,"Id tight","l"); 
  legB->SetFillColor(0);   
  legB->SetTextSize(0.04);   
  legB->Draw();
  gPad->Update();

  TCanvas *cEffIdEta = new TCanvas("cEffIdEta","Electron identification Efficiency vs Eta");
  //cEfficiency->Divide(1,4);
 
  file1->cd("Identification");
  TGraphAsymmErrors * gEleEffEtaId1 = (TGraphAsymmErrors*)gDirectory->Get("EfficiencyIdentificationEtaGraph");
  //cEfficiency->cd(1);
  gEleEffEtaId1->SetLineStyle(1);
  gEleEffEtaId1->SetLineColor(4);
  gEleEffEtaId1->SetMarkerSize(0.5);
  gEleEffEtaId1->SetMarkerStyle(26);
  gEleEffEtaId1->SetMarkerColor(4);
  gEleEffEtaId1->Draw("AP");
  gEleEffEtaId1->GetXaxis()->SetTitle("GEN electron #eta");
  gEleEffEtaId1->GetYaxis()->SetTitle("Efficiency");
  gEleEffEtaId1->GetYaxis()->SetRangeUser(0,1.1);


  file2->cd("Identification");
  TGraphAsymmErrors * gEleEffEtaId2 = (TGraphAsymmErrors*)gDirectory->Get("EfficiencyIdentificationEtaGraph");
  //cEfficiency->cd(1);
  gEleEffEtaId2->SetLineStyle(1);
  gEleEffEtaId2->SetLineColor(8);
  gEleEffEtaId2->SetMarkerSize(0.5);
  gEleEffEtaId2->SetMarkerStyle(20);
  gEleEffEtaId2->SetMarkerColor(8);
  gEleEffEtaId2->Draw("P");

  file3->cd("Identification");
  TGraphAsymmErrors * gEleEffEtaId3 = (TGraphAsymmErrors*)gDirectory->Get("EfficiencyIdentificationEtaGraph");
  //cEfficiency->cd(1);
  gEleEffEtaId3->SetLineStyle(1);
  gEleEffEtaId3->SetLineColor(2);
  gEleEffEtaId3->SetMarkerSize(0.5);
  gEleEffEtaId3->SetMarkerStyle(21);
  gEleEffEtaId3->SetMarkerColor(2);
  gEleEffEtaId3->Draw("P");

  TLegend* legB1 = new TLegend(0.6,0.2,0.85,0.4);
  legB1->AddEntry(gEleEffEtaId1,"Id loose","l"); 
  legB1->AddEntry(gEleEffEtaId2,"Id medium","l"); 
  legB1->AddEntry(gEleEffEtaId3,"Id tight","l"); 
  legB1->SetFillColor(0);   
  legB1->SetTextSize(0.04);   
  legB1->Draw();
  gPad->Update();


  TCanvas *cEffIdPhi = new TCanvas("cEffIdPhi","Electron identification Efficiency vs Phi");
  //cEfficiency->Divide(1,4);
 
  file1->cd("Identification");
  TGraphAsymmErrors * gEleEffPhiId1 = (TGraphAsymmErrors*)gDirectory->Get("EfficiencyIdentificationPhiGraph");
  //cEfficiency->cd(1);
  gEleEffPhiId1->SetLineStyle(1);
  gEleEffPhiId1->SetLineColor(4);
  gEleEffPhiId1->SetMarkerSize(0.5);
  gEleEffPhiId1->SetMarkerStyle(26);
  gEleEffPhiId1->SetMarkerColor(4);
  gEleEffPhiId1->Draw("AP");
  gEleEffPhiId1->GetXaxis()->SetTitle("GEN electron #phi");
  gEleEffPhiId1->GetYaxis()->SetTitle("Efficiency");
  gEleEffPhiId1->GetYaxis()->SetRangeUser(0,1.1);


  file2->cd("Identification");
  TGraphAsymmErrors * gEleEffPhiId2 = (TGraphAsymmErrors*)gDirectory->Get("EfficiencyIdentificationPhiGraph");
  //cEfficiency->cd(1);
  gEleEffPhiId2->SetLineStyle(1);
  gEleEffPhiId2->SetLineColor(8);
  gEleEffPhiId2->SetMarkerSize(0.5);
  gEleEffPhiId2->SetMarkerStyle(20);
  gEleEffPhiId2->SetMarkerColor(8);
  gEleEffPhiId2->Draw("P");

  file3->cd("Identification");
  TGraphAsymmErrors * gEleEffPhiId3 = (TGraphAsymmErrors*)gDirectory->Get("EfficiencyIdentificationPhiGraph");
  //cEfficiency->cd(1);
  gEleEffPhiId3->SetLineStyle(1);
  gEleEffPhiId3->SetLineColor(2);
  gEleEffPhiId3->SetMarkerSize(0.5);
  gEleEffPhiId3->SetMarkerStyle(21);
  gEleEffPhiId3->SetMarkerColor(2);
  gEleEffPhiId3->Draw("P");

  TLegend* legB2 = new TLegend(0.6,0.2,0.85,0.4);
  legB2->AddEntry(gEleEffPhiId1,"Id loose","l"); 
  legB2->AddEntry(gEleEffPhiId2,"Id medium","l"); 
  legB2->AddEntry(gEleEffPhiId3,"Id tight","l"); 
  legB2->SetFillColor(0);   
  legB2->SetTextSize(0.04);   
  legB2->Draw();
  gPad->Update();

 
  //Purity of reconstruction and identification

  TCanvas *cPurityPT = new  TCanvas("cPurityPT","Electron Purity vs PT");
  //cEfficiency->Divide(1,4);
 
  file1->cd("RecPurity");
  TGraphAsymmErrors * gElePurPTRec = (TGraphAsymmErrors*)gDirectory->Get("PurityRecPTGraph");
  //cEfficiency->cd(1);
  gElePurPTRec->SetLineStyle(1);
  gElePurPTRec->SetLineColor(1);
  gElePurPTRec->SetMarkerSize(0.5);
  gElePurPTRec->SetMarkerStyle(23);
  gElePurPTRec->SetMarkerColor(1);
  gElePurPTRec->Draw("AP");
  gElePurPTRec->GetXaxis()->SetTitle("Reco electron p_{T}");
  gElePurPTRec->GetXaxis()->SetRangeUser(min,max);
  gElePurPTRec->GetYaxis()->SetTitle("Purity");
  gElePurPTRec->GetYaxis()->SetRangeUser(0,1.1);

  file1->cd("IdPurity");
  TGraphAsymmErrors * gElePurPTRec1 = (TGraphAsymmErrors*)gDirectory->Get("PurityIdPTGraph");
  //cEfficiency->cd(1);
  gElePurPTRec1->SetLineStyle(1);
  gElePurPTRec1->SetLineColor(4);
  gElePurPTRec1->SetMarkerSize(0.5);
  gElePurPTRec1->SetMarkerStyle(26);
  gElePurPTRec1->SetMarkerColor(4);
  gElePurPTRec1->Draw("P");

  file2->cd("IdPurity");
  TGraphAsymmErrors * gElePurPTRec2 = (TGraphAsymmErrors*)gDirectory->Get("PurityIdPTGraph");
  //cEfficiency->cd(1);
  gElePurPTRec2->SetLineStyle(1);
  gElePurPTRec2->SetLineColor(8);
  gElePurPTRec2->SetMarkerSize(0.5);
  gElePurPTRec2->SetMarkerStyle(20);
  gElePurPTRec2->SetMarkerColor(8);
  gElePurPTRec2->Draw("P");

  file3->cd("IdPurity");
  TGraphAsymmErrors * gElePurPTRec3= (TGraphAsymmErrors*)gDirectory->Get("PurityIdPTGraph");
  //cEfficiency->cd(1);
  gElePurPTRec3->SetLineStyle(1);
  gElePurPTRec3->SetLineColor(2);
  gElePurPTRec3->SetMarkerSize(0.5);
  gElePurPTRec3->SetMarkerStyle(21);
  gElePurPTRec3->SetMarkerColor(2);
  gElePurPTRec3->Draw("P");

  TLegend* legC = new TLegend(0.6,0.2,0.85,0.4);
  legC->AddEntry(gElePurPTRec,"Reconstruction","l"); 
  legC->AddEntry(gElePurPTRec1,"Id loose","l"); 
  legC->AddEntry(gElePurPTRec2,"Id medium","l"); 
  legC->AddEntry(gElePurPTRec3,"Id tight","l"); 
  legC->SetFillColor(0);   
  legC->SetTextSize(0.04);   
  legC->Draw();
  gPad->Update();



  TCanvas *cPurityEta = new  TCanvas("cPurityEta","Electron Purity vs Eta");
  //cEfficiency->Divide(1,4);
 
  file1->cd("RecPurity");
  TGraphAsymmErrors * gElePurEtaRec = (TGraphAsymmErrors*)gDirectory->Get("PurityRecEtaGraph");
  //cEfficiency->cd(1);
  gElePurEtaRec->SetLineStyle(1);
  gElePurEtaRec->SetLineColor(1);
  gElePurEtaRec->SetMarkerSize(0.5);
  gElePurEtaRec->SetMarkerStyle(23);
  gElePurEtaRec->SetMarkerColor(1);
  gElePurEtaRec->Draw("AP");
  gElePurEtaRec->GetXaxis()->SetTitle("Reco electron #eta");
  gElePurEtaRec->GetYaxis()->SetTitle("Purity");
  gElePurEtaRec->GetYaxis()->SetRangeUser(0,1.1);

  file1->cd("IdPurity");
  TGraphAsymmErrors * gElePurEtaRec1 = (TGraphAsymmErrors*)gDirectory->Get("PurityIdEtaGraph");
  //cEfficiency->cd(1);
  gElePurEtaRec1->SetLineStyle(1);
  gElePurEtaRec1->SetLineColor(4);
  gElePurEtaRec1->SetMarkerSize(0.5);
  gElePurEtaRec1->SetMarkerStyle(26);
  gElePurEtaRec1->SetMarkerColor(4);
  gElePurEtaRec1->Draw("P");

  file2->cd("IdPurity");
  TGraphAsymmErrors * gElePurEtaRec2 = (TGraphAsymmErrors*)gDirectory->Get("PurityIdEtaGraph");
  //cEfficiency->cd(1);
  gElePurEtaRec2->SetLineStyle(1);
  gElePurEtaRec2->SetLineColor(8);
  gElePurEtaRec2->SetMarkerSize(0.5);
  gElePurEtaRec2->SetMarkerStyle(20);
  gElePurEtaRec2->SetMarkerColor(8);
  gElePurEtaRec2->Draw("P");

  file3->cd("IdPurity");
  TGraphAsymmErrors * gElePurEtaRec3= (TGraphAsymmErrors*)gDirectory->Get("PurityIdEtaGraph");
  //cEfficiency->cd(1);
  gElePurEtaRec3->SetLineStyle(1);
  gElePurEtaRec3->SetLineColor(2);
  gElePurEtaRec3->SetMarkerSize(0.5);
  gElePurEtaRec3->SetMarkerStyle(21);
  gElePurEtaRec3->SetMarkerColor(2);
  gElePurEtaRec3->Draw("P");

  TLegend* legC1 = new TLegend(0.6,0.2,0.85,0.4);
  legC1->AddEntry(gElePurEtaRec,"Reconstruction","l"); 
  legC1->AddEntry(gElePurEtaRec1,"Id loose","l"); 
  legC1->AddEntry(gElePurEtaRec2,"Id medium","l"); 
  legC1->AddEntry(gElePurEtaRec3,"Id tight","l"); 
  legC1->SetFillColor(0);   
  legC1->SetTextSize(0.04);   
  legC1->Draw();
  gPad->Update();


  TCanvas *cPurityPhi = new  TCanvas("cPurityPhi","Electron Purity vs Phi");
  //cEfficiency->Divide(1,4);
 
  file1->cd("RecPurity");
  TGraphAsymmErrors * gElePurPhiRec = (TGraphAsymmErrors*)gDirectory->Get("PurityRecPhiGraph");
  //cEfficiency->cd(1);
  gElePurPhiRec->SetLineStyle(1);
  gElePurPhiRec->SetLineColor(1);
  gElePurPhiRec->SetMarkerSize(0.5);
  gElePurPhiRec->SetMarkerStyle(23);
  gElePurPhiRec->SetMarkerColor(1);
  gElePurPhiRec->Draw("AP");
  gElePurPhiRec->GetXaxis()->SetTitle("Reco electron #phi");
  gElePurPhiRec->GetYaxis()->SetTitle("Purity");
  gElePurPhiRec->GetYaxis()->SetRangeUser(0,1.1);

  file1->cd("IdPurity");
  TGraphAsymmErrors * gElePurPhiRec1 = (TGraphAsymmErrors*)gDirectory->Get("PurityIdPhiGraph");
  //cEfficiency->cd(1);
  gElePurPhiRec1->SetLineStyle(1);
  gElePurPhiRec1->SetLineColor(4);
  gElePurPhiRec1->SetMarkerSize(0.5);
  gElePurPhiRec1->SetMarkerStyle(26);
  gElePurPhiRec1->SetMarkerColor(4);
  gElePurPhiRec1->Draw("P");

  file2->cd("IdPurity");
  TGraphAsymmErrors * gElePurPhiRec2 = (TGraphAsymmErrors*)gDirectory->Get("PurityIdPhiGraph");
  //cEfficiency->cd(1);
  gElePurPhiRec2->SetLineStyle(1);
  gElePurPhiRec2->SetLineColor(8);
  gElePurPhiRec2->SetMarkerSize(0.5);
  gElePurPhiRec2->SetMarkerStyle(20);
  gElePurPhiRec2->SetMarkerColor(8);
  gElePurPhiRec2->Draw("P");

  file3->cd("IdPurity");
  TGraphAsymmErrors * gElePurPhiRec3= (TGraphAsymmErrors*)gDirectory->Get("PurityIdPhiGraph");
  //cEfficiency->cd(1);
  gElePurPhiRec3->SetLineStyle(1);
  gElePurPhiRec3->SetLineColor(2);
  gElePurPhiRec3->SetMarkerSize(0.5);
  gElePurPhiRec3->SetMarkerStyle(21);
  gElePurPhiRec3->SetMarkerColor(2);
  gElePurPhiRec3->Draw("P");

  TLegend* legC2 = new TLegend(0.6,0.2,0.85,0.4);
  legC2->AddEntry(gElePurPhiRec,"Reconstruction","l"); 
  legC2->AddEntry(gElePurPhiRec1,"Id loose","l"); 
  legC2->AddEntry(gElePurPhiRec2,"Id medium","l"); 
  legC2->AddEntry(gElePurPhiRec3,"Id tight","l"); 
  legC2->SetFillColor(0);   
  legC2->SetTextSize(0.04);   
  legC2->Draw();
  gPad->Update();
  
  TCanvas *cEfficiencyIncl = new TCanvas("cRecEfficiencyIncl","Electron reconstruction Efficiency");

  TH1D* HRECEffInclusive = (TH1D*)file1->Get("Reconstruction/HRECEffInclusive");
  TH1D* HSelEffInclusive = (TH1D*)file1->Get("Identification/HSelEffInclusive");
  TH1D* HSelEffInclusive1 = (TH1D*)file2->Get("Identification/HSelEffInclusive");
  TH1D* HSelEffInclusive2 = (TH1D*)file3->Get("Identification/HSelEffInclusive");

  HRECEffInclusive->SetTitle(0);
  HSelEffInclusive->SetTitle(0);
  HSelEffInclusive1->SetTitle(0);
  HSelEffInclusive2->SetTitle(0);
  HRECEffInclusive->SetLineStyle(1);
  HRECEffInclusive->SetLineWidth(2);
  HRECEffInclusive->SetLineColor(1);
  HRECEffInclusive->GetXaxis()->SetTitle("# matched electrons");
 //HRECEffInclusive->GetXaxis()->SetRangeUser(min,max);
  HRECEffInclusive->GetYaxis()->SetTitle("Fraction of events");
  HRECEffInclusive->GetYaxis()->SetRangeUser(0,1.1);
  HRECEffInclusive->GetXaxis()->SetRangeUser(1,4);
  HRECEffInclusive->GetXaxis()->SetBinLabel(1,"#geq 1");
  HRECEffInclusive->GetXaxis()->SetBinLabel(2,"#geq 2");
  HRECEffInclusive->GetXaxis()->SetBinLabel(3,"#geq 3");
  HRECEffInclusive->GetXaxis()->SetBinLabel(4,"#geq 4");
  HSelEffInclusive->SetLineStyle(1);
  HSelEffInclusive->SetLineWidth(2);
  HSelEffInclusive->SetLineColor(4);
  HSelEffInclusive1->SetLineStyle(1);
  HSelEffInclusive1->SetLineWidth(2);
  HSelEffInclusive1->SetLineColor(8);
  HSelEffInclusive2->SetLineStyle(1);
  HSelEffInclusive2->SetLineWidth(2);
  HSelEffInclusive2->SetLineColor(2);

  HRECEffInclusive->Draw("");
  HSelEffInclusive->Draw("same");
  HSelEffInclusive1->Draw("same");
  HSelEffInclusive2->Draw("same");
 
  TLegend* leg1 = new TLegend(0.6,0.2,0.85,0.4);
  leg1->AddEntry(HRECEffInclusive,"Reconstruction","l"); 
  leg1->AddEntry(HSelEffInclusive,"Id loose","l"); 
  leg1->AddEntry(HSelEffInclusive1,"Id medium","l"); 
  leg1->AddEntry(HSelEffInclusive2,"Id tight","l"); 
  leg1->SetFillColor(0);   
  leg1->SetTextSize(0.04);   
  leg1->Draw();
  gPad->Update();

  //Efficiency of Reconstruction with energy requirement

  TCanvas *cEfficiencyPTEne = new TCanvas("cRecEfficiencyPTEne","Electron reconstruction & identification Efficiency vs PT (energy requirement)");
  //cEfficiency->Divide(1,4);
 
  file1->cd("Energy");
  TGraphAsymmErrors * gEleEffPTEneRec = (TGraphAsymmErrors*)gDirectory->Get("EfficiencyReconstructionPTEneGraph");
  //cEfficiency->cd(1);
  gEleEffPTEneRec->SetLineStyle(1);
  gEleEffPTEneRec->SetLineColor(1);
  gEleEffPTEneRec->SetMarkerSize(0.5);
  gEleEffPTEneRec->SetMarkerStyle(23);
  gEleEffPTEneRec->SetMarkerColor(1);
  gEleEffPTEneRec->Draw("AP");
  gEleEffPTEneRec->GetXaxis()->SetTitle("GEN electron p_{T}");
  gEleEffPTEneRec->GetXaxis()->SetRangeUser(min,max);
  gEleEffPTEneRec->GetYaxis()->SetTitle("Efficiency");
  gEleEffPTEneRec->GetYaxis()->SetRangeUser(0,1.1);

  file1->cd("Energy");

  TGraphAsymmErrors * gEleEffPTEneRec1 = (TGraphAsymmErrors*)gDirectory->Get("EfficiencyRecIdentificationPTEneGraph");
  //cEfficiency->cd(1);
  gEleEffPTEneRec1->SetLineStyle(1);
  gEleEffPTEneRec1->SetLineColor(4);
  gEleEffPTEneRec1->SetMarkerSize(0.5);
  gEleEffPTEneRec1->SetMarkerStyle(26);
  gEleEffPTEneRec1->SetMarkerColor(4);
  gEleEffPTEneRec1->Draw("P");

  file2->cd("Energy");

  TGraphAsymmErrors * gEleEffPTEneRec2 = (TGraphAsymmErrors*)gDirectory->Get("EfficiencyRecIdentificationPTEneGraph");
  //cEfficiency->cd(1);
  gEleEffPTEneRec2->SetLineStyle(1);
  gEleEffPTEneRec2->SetLineColor(8);
  gEleEffPTEneRec2->SetMarkerSize(0.5);
  gEleEffPTEneRec2->SetMarkerStyle(20);
  gEleEffPTEneRec2->SetMarkerColor(8);
  gEleEffPTEneRec2->Draw("P");

  file3->cd("Energy");

  TGraphAsymmErrors * gEleEffPTEneRec3 = (TGraphAsymmErrors*)gDirectory->Get("EfficiencyRecIdentificationPTEneGraph");
  //cEfficiency->cd(1);
  gEleEffPTEneRec3->SetLineStyle(1);
  gEleEffPTEneRec3->SetLineColor(2);
  gEleEffPTEneRec3->SetMarkerSize(0.5);
  gEleEffPTEneRec3->SetMarkerStyle(21);
  gEleEffPTEneRec3->SetMarkerColor(2);
  gEleEffPTEneRec3->Draw("P");

  
  TLegend* legA20 = new TLegend(0.6,0.2,0.85,0.4);
  legA20->AddEntry(gEleEffPTEneRec,"Reconstruction","l"); 
  legA20->AddEntry(gEleEffPTEneRec1,"Id loose","l"); 
  legA20->AddEntry(gEleEffPTEneRec2,"Id medium","l"); 
  legA20->AddEntry(gEleEffPTEneRec3,"Id tight","l"); 
  legA20->SetFillColor(0);   
  legA20->SetTextSize(0.04);   
  legA20->Draw();
  gPad->Update();

  TCanvas *cEfficiencyEtaEne = new TCanvas("cRecEfficiencyEtaEne","Electron reconstruction & identification Efficiency vs Eta (energy requirement)");
  //cEfficiency->Divide(1,4);


  file1->cd("Energy");
  TGraphAsymmErrors * gEleEffEtaEneRec = (TGraphAsymmErrors*)gDirectory->Get("EfficiencyReconstructionEtaEneGraph");
  //cEfficiency->cd(1);
  gEleEffEtaEneRec->SetLineStyle(1);
  gEleEffEtaEneRec->SetLineColor(1);
  gEleEffEtaEneRec->SetMarkerSize(0.5);
  gEleEffEtaEneRec->SetMarkerStyle(23);
  gEleEffEtaEneRec->SetMarkerColor(1);
  gEleEffEtaEneRec->Draw("AP");
  gEleEffEtaEneRec->GetXaxis()->SetTitle("GEN electron #eta");
  gEleEffEtaEneRec->GetYaxis()->SetTitle("Efficiency");
  gEleEffEtaEneRec->GetYaxis()->SetRangeUser(0,1.1);

  file1->cd("Energy");

  TGraphAsymmErrors * gEleEffEtaEneRec1 = (TGraphAsymmErrors*)gDirectory->Get("EfficiencyRecIdentificationEtaEneGraph");
  //cEfficiency->cd(1);
  gEleEffEtaEneRec1->SetLineStyle(1);
  gEleEffEtaEneRec1->SetLineColor(4);
  gEleEffEtaEneRec1->SetMarkerSize(0.5);
  gEleEffEtaEneRec1->SetMarkerStyle(26);
  gEleEffEtaEneRec1->SetMarkerColor(4);
  gEleEffEtaEneRec1->Draw("P");

  file2->cd("Energy");

  TGraphAsymmErrors * gEleEffEtaEneRec2 = (TGraphAsymmErrors*)gDirectory->Get("EfficiencyRecIdentificationEtaEneGraph");
  //cEfficiency->cd(1);
  gEleEffEtaEneRec2->SetLineStyle(1);
  gEleEffEtaEneRec2->SetLineColor(8);
  gEleEffEtaEneRec2->SetMarkerSize(0.5);
  gEleEffEtaEneRec2->SetMarkerStyle(20);
  gEleEffEtaEneRec2->SetMarkerColor(8);
  gEleEffEtaEneRec2->Draw("P");

  file3->cd("Energy");

  TGraphAsymmErrors * gEleEffEtaEneRec3 = (TGraphAsymmErrors*)gDirectory->Get("EfficiencyRecIdentificationEtaEneGraph");
  //cEfficiency->cd(1);
  gEleEffEtaEneRec3->SetLineStyle(1);
  gEleEffEtaEneRec3->SetLineColor(2);
  gEleEffEtaEneRec3->SetMarkerSize(0.5);
  gEleEffEtaEneRec3->SetMarkerStyle(21);
  gEleEffEtaEneRec3->SetMarkerColor(2);
  gEleEffEtaEneRec3->Draw("P");

  TLegend* legA21 = new TLegend(0.6,0.2,0.85,0.4);
  legA21->AddEntry(gEleEffEtaEneRec,"Reconstruction","l"); 
  legA21->AddEntry(gEleEffEtaEneRec1,"Id loose","l"); 
  legA21->AddEntry(gEleEffEtaEneRec2,"Id medium","l"); 
  legA21->AddEntry(gEleEffEtaEneRec3,"Id tight","l"); 
  legA21->SetFillColor(0);   
  legA21->SetTextSize(0.04);   
  legA21->Draw();
  gPad->Update();

  TCanvas *cEfficiencyPhiEne = new TCanvas("cRecEfficiencyPhiEne","Electron reconstruction & identification Efficiency vs Phi (energy requirement)");
  //cEfficiency->Divide(1,4);


  file1->cd("Energy");
  TGraphAsymmErrors * gEleEffPhiEneRec = (TGraphAsymmErrors*)gDirectory->Get("EfficiencyReconstructionPhiEneGraph");
  //cEfficiency->cd(1);
  gEleEffPhiEneRec->SetLineStyle(1);
  gEleEffPhiEneRec->SetLineColor(1);
  gEleEffPhiEneRec->SetMarkerSize(0.5);
  gEleEffPhiEneRec->SetMarkerStyle(23);
  gEleEffPhiEneRec->SetMarkerColor(1);
  gEleEffPhiEneRec->Draw("AP");
  gEleEffPhiEneRec->GetXaxis()->SetTitle("GEN electron #phi");
  gEleEffPhiEneRec->GetYaxis()->SetTitle("Efficiency");
  gEleEffPhiEneRec->GetYaxis()->SetRangeUser(0,1.1);

  file1->cd("Energy");

  TGraphAsymmErrors * gEleEffPhiEneRec1 = (TGraphAsymmErrors*)gDirectory->Get("EfficiencyRecIdentificationPhiEneGraph");
  //cEfficiency->cd(1);
  gEleEffPhiEneRec1->SetLineStyle(1);
  gEleEffPhiEneRec1->SetLineColor(4);
  gEleEffPhiEneRec1->SetMarkerSize(0.5);
  gEleEffPhiEneRec1->SetMarkerStyle(26);
  gEleEffPhiEneRec1->SetMarkerColor(4);
  gEleEffPhiEneRec1->Draw("P");

  file2->cd("Energy");

  TGraphAsymmErrors * gEleEffPhiEneRec2 = (TGraphAsymmErrors*)gDirectory->Get("EfficiencyRecIdentificationPhiEneGraph");
  //cEfficiency->cd(1);
  gEleEffPhiEneRec2->SetLineStyle(1);
  gEleEffPhiEneRec2->SetLineColor(8);
  gEleEffPhiEneRec2->SetMarkerSize(0.5);
  gEleEffPhiEneRec2->SetMarkerStyle(20);
  gEleEffPhiEneRec2->SetMarkerColor(8);
  gEleEffPhiEneRec2->Draw("P");

  file3->cd("Energy");

  TGraphAsymmErrors * gEleEffPhiEneRec3 = (TGraphAsymmErrors*)gDirectory->Get("EfficiencyRecIdentificationPhiEneGraph");
  //cEfficiency->cd(1);
  gEleEffPhiEneRec3->SetLineStyle(1);
  gEleEffPhiEneRec3->SetLineColor(2);
  gEleEffPhiEneRec3->SetMarkerSize(0.5);
  gEleEffPhiEneRec3->SetMarkerStyle(21);
  gEleEffPhiEneRec3->SetMarkerColor(2);
  gEleEffPhiEneRec3->Draw("P");


  TLegend* legA22 = new TLegend(0.6,0.2,0.85,0.4);
  legA22->AddEntry(gEleEffPhiEneRec,"Reconstruction","l"); 
  legA22->AddEntry(gEleEffPhiEneRec1,"Id loose","l"); 
  legA22->AddEntry(gEleEffPhiEneRec2,"Id medium","l"); 
  legA22->AddEntry(gEleEffPhiEneRec3,"Id tight","l"); 
  legA22->SetFillColor(0);   
  legA22->SetTextSize(0.04);   
  legA22->Draw();
  gPad->Update();


    //Efficiency of Identification  with energy requirement
  
  TCanvas *cEfficiencyIdPTEne = new TCanvas("cEfficiencyIdPTEne","Electron identification Efficiency vs PT (energy requirement)");
  //cEfficiency->Divide(1,4);
 
  file1->cd("Energy");

  TGraphAsymmErrors * gEleEffPTEneId1 = (TGraphAsymmErrors*)gDirectory->Get("EfficiencyIdentificationPTEneGraph");
  //cEfficiency->cd(1);
  gEleEffPTEneId1->SetLineStyle(1);
  gEleEffPTEneId1->SetLineColor(4);
  gEleEffPTEneId1->SetMarkerSize(0.5);
  gEleEffPTEneId1->SetMarkerStyle(26);
  gEleEffPTEneId1->SetMarkerColor(4);
  gEleEffPTEneId1->Draw("AP");
  gEleEffPTEneId1->GetXaxis()->SetTitle("GEN electron p_{T}");
  gEleEffPTEneId1->GetXaxis()->SetRangeUser(min,max);
  gEleEffPTEneId1->GetYaxis()->SetTitle("Efficiency");
  gEleEffPTEneId1->GetYaxis()->SetRangeUser(0,1.1);


  file2->cd("Energy");

  TGraphAsymmErrors * gEleEffPTEneId2 = (TGraphAsymmErrors*)gDirectory->Get("EfficiencyIdentificationPTEneGraph");
  //cEfficiency->cd(1);
  gEleEffPTEneId2->SetLineStyle(1);
  gEleEffPTEneId2->SetLineColor(8);
  gEleEffPTEneId2->SetMarkerSize(0.5);
  gEleEffPTEneId2->SetMarkerStyle(20);
  gEleEffPTEneId2->SetMarkerColor(8);
  gEleEffPTEneId2->Draw("P");

  file3->cd("Energy");

  TGraphAsymmErrors * gEleEffPTEneId3 = (TGraphAsymmErrors*)gDirectory->Get("EfficiencyIdentificationPTEneGraph");
  //cEfficiency->cd(1);
  gEleEffPTEneId3->SetLineStyle(1);
  gEleEffPTEneId3->SetLineColor(2);
  gEleEffPTEneId3->SetMarkerSize(0.5);
  gEleEffPTEneId3->SetMarkerStyle(21);
  gEleEffPTEneId3->SetMarkerColor(2);
  gEleEffPTEneId3->Draw("P");

  
  TLegend* legAB20 = new TLegend(0.6,0.2,0.85,0.4);
  legAB20->AddEntry(gEleEffPTEneId1,"Id loose","l"); 
  legAB20->AddEntry(gEleEffPTEneId2,"Id medium","l"); 
  legAB20->AddEntry(gEleEffPTEneId3,"Id tight","l"); 
  legAB20->SetFillColor(0);   
  legAB20->SetTextSize(0.04);   
  legAB20->Draw();
  gPad->Update();

  TCanvas *cEfficiencyIdEtaEne = new TCanvas("cEfficiencyIdEtaEne","Electron identification Efficiency vs Eta (energy requirement)");
  //cEfficiency->Divide(1,4);

  file1->cd("Energy");

  TGraphAsymmErrors * gEleEffEtaEneId1 = (TGraphAsymmErrors*)gDirectory->Get("EfficiencyIdentificationEtaEneGraph");
  //cEfficiency->cd(1);
  gEleEffEtaEneId1->SetLineStyle(1);
  gEleEffEtaEneId1->SetLineColor(4);
  gEleEffEtaEneId1->SetMarkerSize(0.5);
  gEleEffEtaEneId1->SetMarkerStyle(26);
  gEleEffEtaEneId1->SetMarkerColor(4);
  gEleEffEtaEneId1->Draw("AP");
  gEleEffEtaEneId1->GetXaxis()->SetTitle("GEN electron #eta");
  gEleEffEtaEneId1->GetYaxis()->SetTitle("Efficiency");
  gEleEffEtaEneId1->GetYaxis()->SetRangeUser(0,1.1);


  file2->cd("Energy");

  TGraphAsymmErrors * gEleEffEtaEneId2 = (TGraphAsymmErrors*)gDirectory->Get("EfficiencyIdentificationEtaEneGraph");
  //cEfficiency->cd(1);
  gEleEffEtaEneId2->SetLineStyle(1);
  gEleEffEtaEneId2->SetLineColor(8);
  gEleEffEtaEneId2->SetMarkerSize(0.5);
  gEleEffEtaEneId2->SetMarkerStyle(20);
  gEleEffEtaEneId2->SetMarkerColor(8);
  gEleEffEtaEneId2->Draw("P");

  file3->cd("Energy");

  TGraphAsymmErrors * gEleEffEtaEneId3 = (TGraphAsymmErrors*)gDirectory->Get("EfficiencyIdentificationEtaEneGraph");
  //cEfficiency->cd(1);
  gEleEffEtaEneId3->SetLineStyle(1);
  gEleEffEtaEneId3->SetLineColor(2);
  gEleEffEtaEneId3->SetMarkerSize(0.5);
  gEleEffEtaEneId3->SetMarkerStyle(21);
  gEleEffEtaEneId3->SetMarkerColor(2);
  gEleEffEtaEneId3->Draw("P");

  TLegend* legAB21 = new TLegend(0.6,0.2,0.85,0.4);
  legAB21->AddEntry(gEleEffEtaEneId1,"Id loose","l"); 
  legAB21->AddEntry(gEleEffEtaEneId2,"Id medium","l"); 
  legAB21->AddEntry(gEleEffEtaEneId3,"Id tight","l"); 
  legAB21->SetFillColor(0);   
  legAB21->SetTextSize(0.04);   
  legAB21->Draw();
  gPad->Update();

  TCanvas *cEfficiencyIdPhiEne = new TCanvas("cEfficiencyIdPhiEne","Electron identification Efficiency vs Phi (energy requirement)");
  //cEfficiency->Divide(1,4);

  file1->cd("Energy");

  TGraphAsymmErrors * gEleEffPhiEneId1 = (TGraphAsymmErrors*)gDirectory->Get("EfficiencyIdentificationPhiEneGraph");
  //cEfficiency->cd(1);
  gEleEffPhiEneId1->SetLineStyle(1);
  gEleEffPhiEneId1->SetLineColor(4);
  gEleEffPhiEneId1->SetMarkerSize(0.5);
  gEleEffPhiEneId1->SetMarkerStyle(26);
  gEleEffPhiEneId1->SetMarkerColor(4);
  gEleEffPhiEneId1->Draw("AP");
  gEleEffPhiEneId1->GetXaxis()->SetTitle("GEN electron #phi");
  gEleEffPhiEneId1->GetYaxis()->SetTitle("Efficiency");
  gEleEffPhiEneId1->GetYaxis()->SetRangeUser(0,1.1);


  file2->cd("Energy");

  TGraphAsymmErrors * gEleEffPhiEneId2 = (TGraphAsymmErrors*)gDirectory->Get("EfficiencyIdentificationPhiEneGraph");
  //cEfficiency->cd(1);
  gEleEffPhiEneId2->SetLineStyle(1);
  gEleEffPhiEneId2->SetLineColor(8);
  gEleEffPhiEneId2->SetMarkerSize(0.5);
  gEleEffPhiEneId2->SetMarkerStyle(20);
  gEleEffPhiEneId2->SetMarkerColor(8);
  gEleEffPhiEneId2->Draw("P");

  file3->cd("Energy");

  TGraphAsymmErrors * gEleEffPhiEneId3 = (TGraphAsymmErrors*)gDirectory->Get("EfficiencyIdentificationPhiEneGraph");
  //cEfficiency->cd(1);
  gEleEffPhiEneId3->SetLineStyle(1);
  gEleEffPhiEneId3->SetLineColor(2);
  gEleEffPhiEneId3->SetMarkerSize(0.5);
  gEleEffPhiEneId3->SetMarkerStyle(21);
  gEleEffPhiEneId3->SetMarkerColor(2);
  gEleEffPhiEneId3->Draw("P");


  TLegend* legAB22 = new TLegend(0.6,0.2,0.85,0.4);
  legAB22->AddEntry(gEleEffPhiEneId1,"Id loose","l"); 
  legAB22->AddEntry(gEleEffPhiEneId2,"Id medium","l"); 
  legAB22->AddEntry(gEleEffPhiEneId3,"Id tight","l"); 
  legAB22->SetFillColor(0);   
  legAB22->SetTextSize(0.04);   
  legAB22->Draw();
  gPad->Update();


  //MisCharge ratio for reconstruction and identification

  TCanvas *cMisChargePT = new  TCanvas("cMisChargePT","Electron Mischarge Fraction vs PT");
  //cEfficiency->Divide(1,4);
  cMisChargePT->SetLogy();
 
  file1->cd("MisCharge");
  TGraphAsymmErrors * gEleMisChargePTRec = (TGraphAsymmErrors*)gDirectory->Get("MisChargeRecPTGraph");
  //cEfficiency->cd(1);
  gEleMisChargePTRec->SetLineStyle(1);
  gEleMisChargePTRec->SetLineColor(1);
  gEleMisChargePTRec->SetMarkerSize(1.0);
  gEleMisChargePTRec->SetMarkerStyle(23);
  gEleMisChargePTRec->SetMarkerColor(1);
  gEleMisChargePTRec->Draw("AP");
  gEleMisChargePTRec->GetXaxis()->SetTitle("Gen electron p_{T}");
  gEleMisChargePTRec->GetXaxis()->SetRangeUser(min,max);
  gEleMisChargePTRec->GetYaxis()->SetTitle("Mischarge Fraction");
  gEleMisChargePTRec->GetYaxis()->SetRangeUser(0,1.1);

  file1->cd("MisCharge");
  TGraphAsymmErrors * gEleMisChargePTRec1 = (TGraphAsymmErrors*)gDirectory->Get("MisChargeIdPTGraph");
  //cEfficiency->cd(1);
  gEleMisChargePTRec1->SetLineStyle(1);
  gEleMisChargePTRec1->SetLineColor(4);
  gEleMisChargePTRec1->SetMarkerSize(1.0);
  gEleMisChargePTRec1->SetMarkerStyle(26);
  gEleMisChargePTRec1->SetMarkerColor(4);
  gEleMisChargePTRec1->Draw("P");

  file2->cd("MisCharge");
  TGraphAsymmErrors * gEleMisChargePTRec2 = (TGraphAsymmErrors*)gDirectory->Get("MisChargeIdPTGraph");
  //cEfficiency->cd(1);
  gEleMisChargePTRec2->SetLineStyle(1);
  gEleMisChargePTRec2->SetLineColor(8);
  gEleMisChargePTRec2->SetMarkerSize(1.0);
  gEleMisChargePTRec2->SetMarkerStyle(20);
  gEleMisChargePTRec2->SetMarkerColor(8);
  gEleMisChargePTRec2->Draw("P");

  file3->cd("MisCharge");
  TGraphAsymmErrors * gEleMisChargePTRec3= (TGraphAsymmErrors*)gDirectory->Get("MisChargeIdPTGraph");
  //cEfficiency->cd(1);
  gEleMisChargePTRec3->SetLineStyle(1);
  gEleMisChargePTRec3->SetLineColor(2);
  gEleMisChargePTRec3->SetMarkerSize(1.0);
  gEleMisChargePTRec3->SetMarkerStyle(21);
  gEleMisChargePTRec3->SetMarkerColor(2);
  gEleMisChargePTRec3->Draw("P");

  TLegend* legC1 = new TLegend(0.6,0.2,0.85,0.4);
  legC1->AddEntry(gEleMisChargePTRec,"Reconstruction","l"); 
  legC1->AddEntry(gEleMisChargePTRec1,"Id loose","l"); 
  legC1->AddEntry(gEleMisChargePTRec2,"Id medium","l"); 
  legC1->AddEntry(gEleMisChargePTRec3,"Id tight","l"); 
  legC1->SetFillColor(0);   
  legC1->SetTextSize(0.04);   
  legC1->Draw();
  gPad->Update();



  TCanvas *cMisChargeEta = new  TCanvas("cMisChargeEta","Electron Mischarge Fraction vs Eta");
  //cEfficiency->Divide(1,4);
  cMisChargeEta->SetLogy();
 
  file1->cd("MisCharge");
  TGraphAsymmErrors * gEleMisChargeEtaRec = (TGraphAsymmErrors*)gDirectory->Get("MisChargeRecEtaGraph");
  //cEfficiency->cd(1);
  gEleMisChargeEtaRec->SetLineStyle(1);
  gEleMisChargeEtaRec->SetLineColor(1);
  gEleMisChargeEtaRec->SetMarkerSize(1.0);
  gEleMisChargeEtaRec->SetMarkerStyle(23);
  gEleMisChargeEtaRec->SetMarkerColor(1);
  gEleMisChargeEtaRec->Draw("AP");
  gEleMisChargeEtaRec->GetXaxis()->SetTitle("Gen electron #eta");
  gEleMisChargeEtaRec->GetYaxis()->SetTitle("Mischarge Fraction");
  gEleMisChargeEtaRec->GetYaxis()->SetRangeUser(0,1.1);

  file1->cd("MisCharge");
  TGraphAsymmErrors * gEleMisChargeEtaRec1 = (TGraphAsymmErrors*)gDirectory->Get("MisChargeIdEtaGraph");
  //cEfficiency->cd(1);
  gEleMisChargeEtaRec1->SetLineStyle(1);
  gEleMisChargeEtaRec1->SetLineColor(4);
  gEleMisChargeEtaRec1->SetMarkerSize(1.0);
  gEleMisChargeEtaRec1->SetMarkerStyle(26);
  gEleMisChargeEtaRec1->SetMarkerColor(4);
  gEleMisChargeEtaRec1->Draw("P");

  file2->cd("MisCharge");
  TGraphAsymmErrors * gEleMisChargeEtaRec2 = (TGraphAsymmErrors*)gDirectory->Get("MisChargeIdEtaGraph");
  //cEfficiency->cd(1);
  gEleMisChargeEtaRec2->SetLineStyle(1);
  gEleMisChargeEtaRec2->SetLineColor(8);
  gEleMisChargeEtaRec2->SetMarkerSize(1.0);
  gEleMisChargeEtaRec2->SetMarkerStyle(20);
  gEleMisChargeEtaRec2->SetMarkerColor(8);
  gEleMisChargeEtaRec2->Draw("P");

  file3->cd("MisCharge");
  TGraphAsymmErrors * gEleMisChargeEtaRec3= (TGraphAsymmErrors*)gDirectory->Get("MisChargeIdEtaGraph");
  //cEfficiency->cd(1);
  gEleMisChargeEtaRec3->SetLineStyle(1);
  gEleMisChargeEtaRec3->SetLineColor(2);
  gEleMisChargeEtaRec3->SetMarkerSize(1.0);
  gEleMisChargeEtaRec3->SetMarkerStyle(21);
  gEleMisChargeEtaRec3->SetMarkerColor(2);
  gEleMisChargeEtaRec3->Draw("P");

  TLegend* legC11 = new TLegend(0.6,0.2,0.85,0.4);
  legC11->AddEntry(gEleMisChargeEtaRec,"Reconstruction","l"); 
  legC11->AddEntry(gEleMisChargeEtaRec1,"Id loose","l"); 
  legC11->AddEntry(gEleMisChargeEtaRec2,"Id medium","l"); 
  legC11->AddEntry(gEleMisChargeEtaRec3,"Id tight","l"); 
  legC11->SetFillColor(0);   
  legC11->SetTextSize(0.04);   
  legC11->Draw();
  gPad->Update();

  TCanvas *cMisChargePhi = new  TCanvas("cMisChargePhi","Electron Mischarge Fraction vs Phi");
  //cEfficiency->Divide(1,4);
  cMisChargePhi->SetLogy();
 
  file1->cd("MisCharge");
  TGraphAsymmErrors * gEleMisChargePhiRec = (TGraphAsymmErrors*)gDirectory->Get("MisChargeRecPhiGraph");
  //cEfficiency->cd(1);
  gEleMisChargePhiRec->SetLineStyle(1);
  gEleMisChargePhiRec->SetLineColor(1);
  gEleMisChargePhiRec->SetMarkerSize(1.0);
  gEleMisChargePhiRec->SetMarkerStyle(23);
  gEleMisChargePhiRec->SetMarkerColor(1);
  gEleMisChargePhiRec->Draw("AP");
  gEleMisChargePhiRec->GetXaxis()->SetTitle("Gen electron #phi");
  gEleMisChargePhiRec->GetYaxis()->SetTitle("Mischarge Fraction");
  gEleMisChargePhiRec->GetYaxis()->SetRangeUser(0,1.1);

  file1->cd("MisCharge");
  TGraphAsymmErrors * gEleMisChargePhiRec1 = (TGraphAsymmErrors*)gDirectory->Get("MisChargeIdPhiGraph");
  //cEfficiency->cd(1);
  gEleMisChargePhiRec1->SetLineStyle(1);
  gEleMisChargePhiRec1->SetLineColor(4);
  gEleMisChargePhiRec1->SetMarkerSize(1.0);
  gEleMisChargePhiRec1->SetMarkerStyle(26);
  gEleMisChargePhiRec1->SetMarkerColor(4);
  gEleMisChargePhiRec1->Draw("P");

  file2->cd("MisCharge");
  TGraphAsymmErrors * gEleMisChargePhiRec2 = (TGraphAsymmErrors*)gDirectory->Get("MisChargeIdPhiGraph");
  //cEfficiency->cd(1);
  gEleMisChargePhiRec2->SetLineStyle(1);
  gEleMisChargePhiRec2->SetLineColor(8);
  gEleMisChargePhiRec2->SetMarkerSize(1.0);
  gEleMisChargePhiRec2->SetMarkerStyle(20);
  gEleMisChargePhiRec2->SetMarkerColor(8);
  gEleMisChargePhiRec2->Draw("P");

  file3->cd("MisCharge");
  TGraphAsymmErrors * gEleMisChargePhiRec3= (TGraphAsymmErrors*)gDirectory->Get("MisChargeIdPhiGraph");
  //cEfficiency->cd(1);
  gEleMisChargePhiRec3->SetLineStyle(1);
  gEleMisChargePhiRec3->SetLineColor(2);
  gEleMisChargePhiRec3->SetMarkerSize(1.0);
  gEleMisChargePhiRec3->SetMarkerStyle(21);
  gEleMisChargePhiRec3->SetMarkerColor(2);
  gEleMisChargePhiRec3->Draw("P");

  TLegend* legC12 = new TLegend(0.6,0.2,0.85,0.4);
  legC12->AddEntry(gEleMisChargePhiRec,"Reconstruction","l"); 
  legC12->AddEntry(gEleMisChargePhiRec1,"Id loose","l"); 
  legC12->AddEntry(gEleMisChargePhiRec2,"Id medium","l"); 
  legC12->AddEntry(gEleMisChargePhiRec3,"Id tight","l"); 
  legC12->SetFillColor(0);   
  legC12->SetTextSize(0.04);   
  legC12->Draw();
  gPad->Update();

}
