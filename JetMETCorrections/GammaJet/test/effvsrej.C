void effvsrej(){
  gROOT->SetStyle("Plain");
  
  gStyle->SetPalette(1);
  //  gStyle->SetStatStyle(0); // for a completely transparent stat box
  gStyle->SetOptFit(111110); 
  gStyle->SetOptFile(1); 
  
  gStyle->SetMarkerStyle(20);
  gStyle->SetMarkerSize(.6);
  gStyle->SetMarkerColor(1);
  
  gStyle->SetTitleBorderSize(0);  // no border around histogram title (font size can't be calculated anyways ...)
  
  gROOT->ForceStyle();
  TCanvas *c0 = new TCanvas("c0","--c0--",472,0,800,800);
  gStyle->SetOptStat(0);
  gStyle->SetStatY(.9);
  gStyle->SetStatX(.9);

  gROOT->ProcessLine(".! mkdir eps");
  gROOT->ProcessLine(".! mkdir eps/effvsrej");
  

  TH1D p1; TH1D p2; TH1D p3; TH1D p4; TH1D p5; TH1D p6; TH1D p7; TH1D p8; 
  p1.SetMarkerStyle(8); p1.SetMarkerSize(0.8);
  p2.SetMarkerColor(kRed); p2.SetFillColor(kRed);   p2.SetMarkerStyle(8); p2.SetMarkerSize(0.8);
  p3.SetMarkerColor(kBlue);  p3.SetFillColor(kBlue);     p3.SetMarkerStyle(8); p3.SetMarkerSize(0.8);
  p4.SetMarkerColor(kGreen); p4.SetFillColor(kGreen);    p4.SetMarkerStyle(8); p4.SetMarkerSize(0.8);
  p5.SetMarkerColor(kMagenta);  p5.SetFillColor(kMagenta);     p5.SetMarkerStyle(8); p5.SetMarkerSize(0.8);
  p6.SetMarkerColor(39); p6.SetFillColor(39);    p6.SetMarkerStyle(8); p6.SetMarkerSize(0.8);
  p7.SetMarkerColor(7); p7.SetFillColor(7);    p7.SetMarkerStyle(8); p7.SetMarkerSize(0.8);

  //background file
  //TFile file_QCD_120_170("/u1/delre/mixbkg_last.root");
  TFile file_QCD_120_170("output_QCD_ite/output_QCD_120.root");
  //TFile file_QCD_120_170("/u1/delre/mixsignal.root");
  //signal file
  //  TFile file_120_170("testsig.root");
  //TFile file_120_170("/u1/delre/mixsignal_last.root");
  TFile file_120_170("output_PhotonJets_ite/output_PhotonJets_120.root");

  TH1D rejvseffecal0151d("rejvseffecal0151d","rej vs eff ecal015",50,0.,1.);
  TH2D rejvseffecal015("rejvseffecal015","rej vs eff ecal015",200,0.,1.,200,0.2,1.);
  TH2D rejvseffecal02("rejvseffecal02","rej vs eff ecal02",200,0.,1.,200,0.2,1.);
  TH2D rejvseffecal025("rejvseffecal025","rej vs eff ecal025",200,0.,1.,200,0.2,1.);
  TH2D rejvseffecal03("rejvseffecal03","rej vs eff ecal03",200,0.,1.,200,0.2,1.);
  TH2D rejvseffecal035("rejvseffecal035","rej vs eff ecal035",200,0.,1.,200,0.2,1.);
  TH2D rejvseffecal04("rejvseffecal04","rej vs eff ecal04",200,0.,1.,200,0.2,1.);
  for (int i = 1; i<((TH1D*)gDirectory->Get("ecaliso015relphot"))->GetXaxis()->GetNbins()+1;i++){
    file_120_170.cd();
    int nsig = ((TH1D*)gDirectory->Get("ecaliso015relphot"))->Integral(1,i);
    file_QCD_120_170.cd();
    int nbkg = ((TH1D*)gDirectory->Get("ecaliso015relbkg"))->Integral(1,i);
    file_120_170.cd();
    double eff = nsig / ((TH1D*)gDirectory->Get("ecaliso015relphot"))->GetEntries();
    file_QCD_120_170.cd();
    double rej = 1- nbkg / ((TH1D*)gDirectory->Get("ecaliso015relbkg"))->GetEntries();
    rejvseffecal015.Fill(eff,rej);
    if(eff>rejvseffecal0151d.GetBinContent(int(rej*200)))
       rejvseffecal0151d.SetBinContent(int(rej*200),eff);
    file_120_170.cd();
    nsig = ((TH1D*)gDirectory->Get("ecaliso02relphot"))->Integral(1,i);
    file_QCD_120_170.cd();
    nbkg = ((TH1D*)gDirectory->Get("ecaliso02relbkg"))->Integral(1,i);
    file_120_170.cd();
    eff = nsig / ((TH1D*)gDirectory->Get("ecaliso02relphot"))->GetEntries();
    file_QCD_120_170.cd();
    rej = 1- nbkg / ((TH1D*)gDirectory->Get("ecaliso02relbkg"))->GetEntries();
    rejvseffecal02.Fill(eff,rej);
    file_120_170.cd();
    nsig = ((TH1D*)gDirectory->Get("ecaliso025relphot"))->Integral(1,i);
    file_QCD_120_170.cd();
    nbkg = ((TH1D*)gDirectory->Get("ecaliso025relbkg"))->Integral(1,i);
    file_120_170.cd();
    eff = nsig / ((TH1D*)gDirectory->Get("ecaliso025relphot"))->GetEntries();
    file_QCD_120_170.cd();
    rej = 1- nbkg / ((TH1D*)gDirectory->Get("ecaliso025relbkg"))->GetEntries();
    rejvseffecal025.Fill(eff,rej);
    file_120_170.cd();
    nsig = ((TH1D*)gDirectory->Get("ecaliso03relphot"))->Integral(1,i);
    file_QCD_120_170.cd();
    nbkg = ((TH1D*)gDirectory->Get("ecaliso03relbkg"))->Integral(1,i);
    file_120_170.cd();
    eff = nsig / ((TH1D*)gDirectory->Get("ecaliso03relphot"))->GetEntries();
    file_QCD_120_170.cd();
    rej = 1- nbkg / ((TH1D*)gDirectory->Get("ecaliso03relbkg"))->GetEntries();
    rejvseffecal03.Fill(eff,rej);
    file_120_170.cd();
    nsig = ((TH1D*)gDirectory->Get("ecaliso035relphot"))->Integral(1,i);
    file_QCD_120_170.cd();
    nbkg = ((TH1D*)gDirectory->Get("ecaliso035relbkg"))->Integral(1,i);
    file_120_170.cd();
    eff = nsig / ((TH1D*)gDirectory->Get("ecaliso035relphot"))->GetEntries();
    file_QCD_120_170.cd();
    rej = 1- nbkg / ((TH1D*)gDirectory->Get("ecaliso035relbkg"))->GetEntries();
    rejvseffecal035.Fill(eff,rej);
    file_120_170.cd();
    nsig = ((TH1D*)gDirectory->Get("ecaliso04relphot"))->Integral(1,i);
    file_QCD_120_170.cd();
    nbkg = ((TH1D*)gDirectory->Get("ecaliso04relbkg"))->Integral(1,i);
    file_120_170.cd();
    eff = nsig / ((TH1D*)gDirectory->Get("ecaliso04relphot"))->GetEntries();
    file_QCD_120_170.cd();
    rej = 1- nbkg / ((TH1D*)gDirectory->Get("ecaliso04relbkg"))->GetEntries();
    rejvseffecal04.Fill(eff,rej);     
  }

  TLegendEntry *legge;
  TLegend *leg;
  leg = new TLegend(0.15,0.15,0.5,0.5);
  leg->SetFillStyle(0); leg->SetBorderSize(0.); leg->SetTextSize(0.05);
  leg->SetFillColor(0);
  legge = leg->AddEntry(&p1, "#Delta R < 0.15", "p");
  legge = leg->AddEntry(&p2, "#Delta R < 0.2", "p");
  legge = leg->AddEntry(&p3, "#Delta R < 0.25", "p");
  legge = leg->AddEntry(&p4, "#Delta R < 0.3", "p");
  legge = leg->AddEntry(&p5, "#Delta R < 0.35", "p");
  legge = leg->AddEntry(&p6, "#Delta R < 0.4", "p");

  rejvseffecal015.SetAxisRange(.95,1.,"X");
  rejvseffecal015.SetTitle("");  
  rejvseffecal015.SetXTitle("Efficiency");
  rejvseffecal015.SetYTitle("Rejection");
  rejvseffecal015.SetTitleOffset(1.3,"Y");
  rejvseffecal015.SetStats(0);
  rejvseffecal015.Draw("");
  rejvseffecal02.SetMarkerColor(kRed);
  rejvseffecal02.Draw("same");
  rejvseffecal025.SetMarkerColor(kBlue);
  rejvseffecal025.Draw("same");
  rejvseffecal03.SetMarkerColor(kGreen);
  rejvseffecal03.Draw("same");
  rejvseffecal035.SetMarkerColor(kMagenta);
  rejvseffecal035.Draw("same");
  rejvseffecal04.SetMarkerColor(39);
  rejvseffecal04.Draw("same");
  leg->Draw();
  c0->SaveAs("eps/effvsrej/rejvseffecal.eps");
  c0->SaveAs("eps/effvsrej/rejvseffecal.gif");

  rejvseffecal0151d.SetAxisRange(.6,1.,"Y");
  rejvseffecal0151d.SetTitle("");  
  rejvseffecal0151d.SetXTitle("Efficiency");
  rejvseffecal0151d.SetYTitle("Rejection");
  rejvseffecal0151d.SetTitleOffset(1.3,"Y");
  rejvseffecal0151d.SetStats(0);
  rejvseffecal0151d.Smooth();
  rejvseffecal0151d.Draw("C");
  c0->SaveAs("eps/effvsrej/rejvseffecal1d.eps");
  c0->SaveAs("eps/effvsrej/rejvseffecal1d.gif");

  TH2D rejvseffecalrec02("rejvseffrececal02","rej vs eff ecal02",200,0.,1.,200,0.2,1.);
  TH2D rejvseffecalrec03("rejvseffrececal03","rej vs eff ecal03",200,0.,1.,200,0.2,1.);
  TH2D rejvseffecalrec035("rejvseffrececal035","rej vs eff ecal035",200,0.,1.,200,0.2,1.);
  TH2D rejvseffecalrec04("rejvseffrececal04","rej vs eff ecal04",200,0.,1.,200,0.2,1.);
  TH2D rejvseffecalrec05("rejvseffrececal05","rej vs eff ecal05",200,0.,1.,200,0.2,1.);
  file_120_170.cd();
  for (int i = 1; i<((TH1D*)gDirectory->Get("ecaliso02recrelphot"))->GetXaxis()->GetNbins()+1;i++){
    file_120_170.cd();
    int nsig = ((TH1D*)gDirectory->Get("ecaliso02recrelphot"))->Integral(1,i);
    file_QCD_120_170.cd();
    int nbkg = ((TH1D*)gDirectory->Get("ecaliso02recrelbkg"))->Integral(1,i);
    file_120_170.cd();
    double eff = nsig / ((TH1D*)gDirectory->Get("ecaliso02recrelphot"))->GetEntries();
    file_QCD_120_170.cd();
    double rej = 1- nbkg / ((TH1D*)gDirectory->Get("ecaliso02recrelbkg"))->GetEntries();
    rejvseffecalrec02.Fill(eff,rej);
    file_120_170.cd();
    nsig = ((TH1D*)gDirectory->Get("ecaliso03recrelphot"))->Integral(1,i);
    file_QCD_120_170.cd();
    nbkg = ((TH1D*)gDirectory->Get("ecaliso03recrelbkg"))->Integral(1,i);
    file_120_170.cd();
    eff = nsig / ((TH1D*)gDirectory->Get("ecaliso03recrelphot"))->GetEntries();
    file_QCD_120_170.cd();
    rej = 1- nbkg / ((TH1D*)gDirectory->Get("ecaliso03recrelbkg"))->GetEntries();
    rejvseffecalrec03.Fill(eff,rej);
    file_120_170.cd();
    nsig = ((TH1D*)gDirectory->Get("ecaliso035recrelphot"))->Integral(1,i);
    file_QCD_120_170.cd();
    nbkg = ((TH1D*)gDirectory->Get("ecaliso035recrelbkg"))->Integral(1,i);
    file_120_170.cd();
    eff = nsig / ((TH1D*)gDirectory->Get("ecaliso035recrelphot"))->GetEntries();
    file_QCD_120_170.cd();
    rej = 1- nbkg / ((TH1D*)gDirectory->Get("ecaliso035recrelbkg"))->GetEntries();
    rejvseffecalrec035.Fill(eff,rej);
    file_120_170.cd();
    nsig = ((TH1D*)gDirectory->Get("ecaliso04recrelphot"))->Integral(1,i);
    file_QCD_120_170.cd();
    nbkg = ((TH1D*)gDirectory->Get("ecaliso04recrelbkg"))->Integral(1,i);
    file_120_170.cd();
    eff = nsig / ((TH1D*)gDirectory->Get("ecaliso04recrelphot"))->GetEntries();
    file_QCD_120_170.cd();
    rej = 1- nbkg / ((TH1D*)gDirectory->Get("ecaliso04recrelbkg"))->GetEntries();
    rejvseffecalrec04.Fill(eff,rej);     
    file_120_170.cd();
    nsig = ((TH1D*)gDirectory->Get("ecaliso05recrelphot"))->Integral(1,i);
    file_QCD_120_170.cd();
    nbkg = ((TH1D*)gDirectory->Get("ecaliso05recrelbkg"))->Integral(1,i);
    file_120_170.cd();
    eff = nsig / ((TH1D*)gDirectory->Get("ecaliso05recrelphot"))->GetEntries();
    file_QCD_120_170.cd();
    rej = 1- nbkg / ((TH1D*)gDirectory->Get("ecaliso05recrelbkg"))->GetEntries();
    rejvseffecalrec05.Fill(eff,rej);     
  }

  TLegendEntry *legge;
  TLegend *leg;
  leg = new TLegend(0.15,0.15,0.5,0.5);
  leg->SetFillStyle(0); leg->SetBorderSize(0.); leg->SetTextSize(0.05);
  leg->SetFillColor(0);
  legge = leg->AddEntry(&p1, "#Delta R < 0.2", "p");
  legge = leg->AddEntry(&p2, "#Delta R < 0.3", "p");
  legge = leg->AddEntry(&p3, "#Delta R < 0.35", "p");
  legge = leg->AddEntry(&p4, "#Delta R < 0.4", "p");
  legge = leg->AddEntry(&p5, "#Delta R < 0.5", "p");

  rejvseffecalrec02.SetAxisRange(.0,1.,"X");
  rejvseffecalrec02.SetTitle("");  
  rejvseffecalrec02.SetXTitle("Efficiency");
  rejvseffecalrec02.SetYTitle("Rejection");
  rejvseffecalrec02.SetTitleOffset(1.3,"Y");
  rejvseffecalrec02.SetStats(0);
  rejvseffecalrec02.Draw("");
  rejvseffecalrec03.SetMarkerColor(kRed);
  rejvseffecalrec03.Draw("same");
  rejvseffecalrec035.SetMarkerColor(kBlue);
  rejvseffecalrec035.Draw("same");
  rejvseffecalrec04.SetMarkerColor(kGreen);
  rejvseffecalrec04.Draw("same");
  rejvseffecalrec05.SetMarkerColor(kMagenta);
  rejvseffecalrec05.Draw("same");
  leg->Draw();
  c0->SaveAs("eps/effvsrej/rejvseffecalrec.eps");
  c0->SaveAs("eps/effvsrej/rejvseffecalrec.gif");


  TH2D ptiso02("ptiso02","rej vs eff ecal02",400,0.3,1.,400,0.6,1.);
  TH2D ptiso025("ptiso025","rej vs eff ecal025",400,0.3,1.,400,0.6,1.);
  TH2D ptiso03("ptiso03","rej vs eff ecal03",400,0.3,1.,400,0.6,1.);
  TH2D ptiso035("ptiso035","rej vs eff ecal035",400,0.3,1.,400,0.6,1.);
  TH2D ptiso04("ptiso04","rej vs eff ecal04",400,0.3,1.,400,0.6,1.);

  file_120_170.cd();
  for (int i = 1; i<((TH1D*)gDirectory->Get("ptiso02relphot"))->GetXaxis()->GetNbins()+1;i++){
    file_120_170.cd();
    int nsig = ((TH1D*)gDirectory->Get("ptiso02relphot"))->Integral(0,i);
    file_QCD_120_170.cd();
    int nbkg = ((TH1D*)gDirectory->Get("ptiso02relbkg"))->Integral(0,i);
    file_120_170.cd();
    double eff = nsig / ((TH1D*)gDirectory->Get("ptiso02relphot"))->GetEntries();
    file_QCD_120_170.cd();
    double rej = 1- nbkg / ((TH1D*)gDirectory->Get("ptiso02relbkg"))->GetEntries();
    ptiso02.Fill(eff,rej);
    file_120_170.cd();
    nsig = ((TH1D*)gDirectory->Get("ptiso025relphot"))->Integral(0,i);
    file_QCD_120_170.cd();
    nbkg = ((TH1D*)gDirectory->Get("ptiso025relbkg"))->Integral(0,i);
    file_120_170.cd();
    eff = nsig / ((TH1D*)gDirectory->Get("ptiso025relphot"))->GetEntries();
    file_QCD_120_170.cd();
    rej = 1- nbkg / ((TH1D*)gDirectory->Get("ptiso025relbkg"))->GetEntries();
    ptiso025.Fill(eff,rej);
    file_120_170.cd();
    nsig = ((TH1D*)gDirectory->Get("ptiso03relphot"))->Integral(0,i);
    file_QCD_120_170.cd();
    nbkg = ((TH1D*)gDirectory->Get("ptiso03relbkg"))->Integral(0,i);
    file_120_170.cd();
    eff = nsig / ((TH1D*)gDirectory->Get("ptiso03relphot"))->GetEntries();
    file_QCD_120_170.cd();
    rej = 1- nbkg / ((TH1D*)gDirectory->Get("ptiso03relbkg"))->GetEntries();
    ptiso03.Fill(eff,rej);
    file_120_170.cd();
    nsig = ((TH1D*)gDirectory->Get("ptiso035relphot"))->Integral(0,i);
    file_QCD_120_170.cd();
    nbkg = ((TH1D*)gDirectory->Get("ptiso035relbkg"))->Integral(0,i);
    file_120_170.cd();
    eff = nsig / ((TH1D*)gDirectory->Get("ptiso035relphot"))->GetEntries();
    file_QCD_120_170.cd();
    rej = 1- nbkg / ((TH1D*)gDirectory->Get("ptiso035relbkg"))->GetEntries();
    ptiso035.Fill(eff,rej);
    file_120_170.cd();
    nsig = ((TH1D*)gDirectory->Get("ptiso04relphot"))->Integral(0,i);
    file_QCD_120_170.cd();
    nbkg = ((TH1D*)gDirectory->Get("ptiso04relbkg"))->Integral(0,i);
    file_120_170.cd();
    eff = nsig / ((TH1D*)gDirectory->Get("ptiso04relphot"))->GetEntries();
    file_QCD_120_170.cd();
    rej = 1- nbkg / ((TH1D*)gDirectory->Get("ptiso04relbkg"))->GetEntries();
    ptiso04.Fill(eff,rej);
    
  }

  TLegendEntry *legge2;
  TLegend *leg2;
  leg2 = new TLegend(0.15,0.15,0.5,0.5);
  leg2->SetFillStyle(0); leg2->SetBorderSize(0.); leg2->SetTextSize(0.05);
  leg2->SetFillColor(0);
  legge2 = leg2->AddEntry(&p1, "#Delta R < 0.2", "p");
  legge2 = leg2->AddEntry(&p2, "#Delta R < 0.25", "p");
  legge2 = leg2->AddEntry(&p3, "#Delta R < 0.3", "p");
  legge2 = leg2->AddEntry(&p4, "#Delta R < 0.35", "p");
  legge2 = leg2->AddEntry(&p5, "#Delta R < 0.4", "p");
  ptiso02.SetTitle("");  
  ptiso02.SetXTitle("Efficiency");
  ptiso02.SetYTitle("Rejection");
  ptiso02.SetTitleOffset(1.3,"Y");
  ptiso02.SetStats(0);
  ptiso02.Draw("");
  ptiso025.SetMarkerColor(kRed);
  ptiso025.Draw("same");
  ptiso03.SetMarkerColor(kBlue);
  ptiso03.Draw("same");
  ptiso035.SetMarkerColor(kGreen);
  ptiso035.Draw("same");
  ptiso04.SetMarkerColor(kMagenta);
  ptiso04.Draw("same");
  leg2->Draw();
  c0->SaveAs("eps/effvsrej/rejvseffptiso.eps");
  c0->SaveAs("eps/effvsrej/rejvseffptiso.gif");

  ptiso02.SetAxisRange(.9,1.,"Y");
  ptiso02.Draw("");
  ptiso025.SetMarkerColor(kRed);
  ptiso025.Draw("same");
  ptiso03.SetMarkerColor(kBlue);
  ptiso03.Draw("same");
  ptiso035.SetMarkerColor(kGreen);
  ptiso035.Draw("same");
  ptiso04.SetMarkerColor(kMagenta);
  ptiso04.Draw("same");
  leg2->Draw();
  
  c0->SaveAs("eps/effvsrej/rejvseffptisozoom.eps");
  c0->SaveAs("eps/effvsrej/rejvseffptisozoom.gif");

  TH2D ptisoatecal02("ptisoatecal02","rej vs eff ecal02",400,0.3,1.,400,0.6,1.);
  TH2D ptisoatecal025("ptisoatecal025","rej vs eff ecal025",400,0.3,1.,400,0.6,1.);
  TH2D ptisoatecal03("ptisoatecal03","rej vs eff ecal03",400,0.3,1.,400,0.6,1.);
  TH2D ptisoatecal035("ptisoatecal035","rej vs eff ecal035",400,0.3,1.,400,0.6,1.);
  TH2D ptisoatecal04("ptisoatecal04","rej vs eff ecal04",400,0.3,1.,400,0.6,1.);

  file_120_170.cd();
  for (int i = 1; i<((TH1D*)gDirectory->Get("ptisoatecal02relphot"))->GetXaxis()->GetNbins()+1;i++){
    file_120_170.cd();
    int nsig = ((TH1D*)gDirectory->Get("ptisoatecal02relphot"))->Integral(0,i);
    file_QCD_120_170.cd();
    int nbkg = ((TH1D*)gDirectory->Get("ptisoatecal02relbkg"))->Integral(0,i);
    file_120_170.cd();
    double eff = nsig / ((TH1D*)gDirectory->Get("ptisoatecal02relphot"))->GetEntries();
    file_QCD_120_170.cd();
    double rej = 1- nbkg / ((TH1D*)gDirectory->Get("ptisoatecal02relbkg"))->GetEntries();
    ptisoatecal02.Fill(eff,rej);
    file_120_170.cd();
    nsig = ((TH1D*)gDirectory->Get("ptisoatecal025relphot"))->Integral(0,i);
    file_QCD_120_170.cd();
    nbkg = ((TH1D*)gDirectory->Get("ptisoatecal025relbkg"))->Integral(0,i);
    file_120_170.cd();
    eff = nsig / ((TH1D*)gDirectory->Get("ptisoatecal025relphot"))->GetEntries();
    file_QCD_120_170.cd();
    rej = 1- nbkg / ((TH1D*)gDirectory->Get("ptisoatecal025relbkg"))->GetEntries();
    ptisoatecal025.Fill(eff,rej);
    file_120_170.cd();
    nsig = ((TH1D*)gDirectory->Get("ptisoatecal03relphot"))->Integral(0,i);
    file_QCD_120_170.cd();
    nbkg = ((TH1D*)gDirectory->Get("ptisoatecal03relbkg"))->Integral(0,i);
    file_120_170.cd();
    eff = nsig / ((TH1D*)gDirectory->Get("ptisoatecal03relphot"))->GetEntries();
    file_QCD_120_170.cd();
    rej = 1- nbkg / ((TH1D*)gDirectory->Get("ptisoatecal03relbkg"))->GetEntries();
    ptisoatecal03.Fill(eff,rej);
    file_120_170.cd();
    nsig = ((TH1D*)gDirectory->Get("ptisoatecal035relphot"))->Integral(0,i);
    file_QCD_120_170.cd();
    nbkg = ((TH1D*)gDirectory->Get("ptisoatecal035relbkg"))->Integral(0,i);
    file_120_170.cd();
    eff = nsig / ((TH1D*)gDirectory->Get("ptisoatecal035relphot"))->GetEntries();
    file_QCD_120_170.cd();
    rej = 1- nbkg / ((TH1D*)gDirectory->Get("ptisoatecal035relbkg"))->GetEntries();
    ptisoatecal035.Fill(eff,rej);
    file_120_170.cd();
    nsig = ((TH1D*)gDirectory->Get("ptisoatecal04relphot"))->Integral(0,i);
    file_QCD_120_170.cd();
    nbkg = ((TH1D*)gDirectory->Get("ptisoatecal04relbkg"))->Integral(0,i);
    file_120_170.cd();
    eff = nsig / ((TH1D*)gDirectory->Get("ptisoatecal04relphot"))->GetEntries();
    file_QCD_120_170.cd();
    rej = 1- nbkg / ((TH1D*)gDirectory->Get("ptisoatecal04relbkg"))->GetEntries();
    ptisoatecal04.Fill(eff,rej);
    
  }

  ptisoatecal02.SetTitle("");  
  ptisoatecal02.SetXTitle("Efficiency");
  ptisoatecal02.SetYTitle("Rejection");
  ptisoatecal02.SetTitleOffset(1.3,"Y");
  ptisoatecal02.SetStats(0);
  ptisoatecal02.Draw("");
  ptisoatecal025.SetMarkerColor(kRed);
  ptisoatecal025.Draw("same");
  ptisoatecal03.SetMarkerColor(kBlue);
  ptisoatecal03.Draw("same");
  ptisoatecal035.SetMarkerColor(kGreen);
  ptisoatecal035.Draw("same");
  ptisoatecal04.SetMarkerColor(kMagenta);
  ptisoatecal04.Draw("same");
  leg2->Draw();
  
  c0->SaveAs("eps/effvsrej/rejvseffptisoatecal.eps");
  c0->SaveAs("eps/effvsrej/rejvseffptisoatecal.gif");

  ptisoatecal02.SetAxisRange(.6,1.,"Y");
  ptisoatecal02.Draw("");
  ptisoatecal025.SetMarkerColor(kRed);
  ptisoatecal025.Draw("same");
  ptisoatecal03.SetMarkerColor(kBlue);
  ptisoatecal03.Draw("same");
  ptisoatecal035.SetMarkerColor(kGreen);
  ptisoatecal035.Draw("same");
  ptisoatecal04.SetMarkerColor(kMagenta);
  ptisoatecal04.Draw("same");
  c0->SaveAs("eps/effvsrej/rejvseffptisoatecalzoom.eps");
  c0->SaveAs("eps/effvsrej/rejvseffptisoatecalzoom.gif");

  TLegendEntry *legge4;
  TLegend *leg4;
  leg4 = new TLegend(0.15,0.15,0.5,0.5);
  leg4->SetFillStyle(0); leg4->SetBorderSize(0.); leg4->SetTextSize(0.05);
  leg4->SetFillColor(0);
  legge4 = leg4->AddEntry(&p4, "pt iso at vtx", "p");
  legge4 = leg4->AddEntry(&p5, "pt iso at ECAL", "p");
  ptiso035.SetTitle("");  
  ptiso035.SetAxisRange(.6,1.,"Y");
  ptiso035.SetXTitle("Efficiency");
  ptiso035.SetYTitle("Rejection");
  ptiso035.Draw("");
  ptisoatecal04.Draw("same");
  leg4->Draw();
  c0->SaveAs("eps/effvsrej/rejvseffcompnoecalecalzoom.eps");
  c0->SaveAs("eps/effvsrej/rejvseffcompnoecalecalzoom.gif");


  TH2D hcalovecal01("hcalovecal01","rej vs eff ecal01",400,0.,1.,400,0.6,1.);
  TH2D hcalovecal02("hcalovecal02","rej vs eff ecal02",400,0.,1.,400,0.6,1.);
  TH2D hcalovecal025("hcalovecal025","rej vs eff ecal025",400,0.,1.,400,0.6,1.);
  TH2D hcalovecal03("hcalovecal03","rej vs eff ecal03",400,0.,1.,400,0.6,1.);
  TH2D hcalovecal04("hcalovecal04","rej vs eff ecal04",400,0.,1.,400,0.6,1.);
  TH2D hcalovecal05("hcalovecal05","rej vs eff ecal05",400,0.,1.,400,0.6,1.);
  TLegendEntry *legge3;
  TLegend *leg3;
  leg3 = new TLegend(0.15,0.15,0.5,0.5);
  leg3->SetFillStyle(0); leg3->SetBorderSize(0.); leg3->SetTextSize(0.05);
  leg3->SetFillColor(0);
  legge3 = leg3->AddEntry(&p1, "#Delta R < 0.1", "p");
  legge3 = leg3->AddEntry(&p2, "#Delta R < 0.2", "p");
  legge3 = leg3->AddEntry(&p3, "#Delta R < 0.25", "p");
  legge3 = leg3->AddEntry(&p4, "#Delta R < 0.3", "p");
  legge3 = leg3->AddEntry(&p5, "#Delta R < 0.4", "p");
  legge3 = leg3->AddEntry(&p6, "#Delta R < 0.5", "p");

  file_120_170.cd();
  for (int i = 1; i<((TH1D*)gDirectory->Get("hcalovecal01phot"))->GetXaxis()->GetNbins()+1;i++){
    file_120_170.cd();
    int nsig = ((TH1D*)gDirectory->Get("hcalovecal01phot"))->Integral(0,i);
    file_QCD_120_170.cd();
    int nbkg = ((TH1D*)gDirectory->Get("hcalovecal01bkg"))->Integral(0,i);
    file_120_170.cd();
    double eff = nsig / ((TH1D*)gDirectory->Get("hcalovecal01phot"))->GetEntries();
    file_QCD_120_170.cd();
    double rej = 1- nbkg / ((TH1D*)gDirectory->Get("hcalovecal01bkg"))->GetEntries();
    hcalovecal01.Fill(eff,rej);
    file_120_170.cd();
    nsig = ((TH1D*)gDirectory->Get("hcalovecal02phot"))->Integral(0,i);
    file_QCD_120_170.cd();
    nbkg = ((TH1D*)gDirectory->Get("hcalovecal02bkg"))->Integral(0,i);
    file_120_170.cd();
    eff = nsig / ((TH1D*)gDirectory->Get("hcalovecal02phot"))->GetEntries();
    file_QCD_120_170.cd();
    rej = 1- nbkg / ((TH1D*)gDirectory->Get("hcalovecal02bkg"))->GetEntries();
    hcalovecal02.Fill(eff,rej);
    file_120_170.cd();
    nsig = ((TH1D*)gDirectory->Get("hcalovecal025phot"))->Integral(0,i);
    file_QCD_120_170.cd();
    nbkg = ((TH1D*)gDirectory->Get("hcalovecal025bkg"))->Integral(0,i);
    file_120_170.cd();
    eff = nsig / ((TH1D*)gDirectory->Get("hcalovecal025phot"))->GetEntries();
    file_QCD_120_170.cd();
    rej = 1- nbkg / ((TH1D*)gDirectory->Get("hcalovecal025bkg"))->GetEntries();
    hcalovecal025.Fill(eff,rej);
    file_120_170.cd();
    nsig = ((TH1D*)gDirectory->Get("hcalovecal03phot"))->Integral(0,i);
    file_QCD_120_170.cd();
    nbkg = ((TH1D*)gDirectory->Get("hcalovecal03bkg"))->Integral(0,i);
    file_120_170.cd();
    eff = nsig / ((TH1D*)gDirectory->Get("hcalovecal03phot"))->GetEntries();
    file_QCD_120_170.cd();
    rej = 1- nbkg / ((TH1D*)gDirectory->Get("hcalovecal03bkg"))->GetEntries();
    hcalovecal03.Fill(eff,rej);
    file_120_170.cd();
    nsig = ((TH1D*)gDirectory->Get("hcalovecal04phot"))->Integral(0,i);
    file_QCD_120_170.cd();
    nbkg = ((TH1D*)gDirectory->Get("hcalovecal04bkg"))->Integral(0,i);
    file_120_170.cd();
    eff = nsig / ((TH1D*)gDirectory->Get("hcalovecal04phot"))->GetEntries();
    file_QCD_120_170.cd();
    rej = 1- nbkg / ((TH1D*)gDirectory->Get("hcalovecal04bkg"))->GetEntries();
    hcalovecal04.Fill(eff,rej);
    file_120_170.cd();
    nsig = ((TH1D*)gDirectory->Get("hcalovecal05phot"))->Integral(0,i);
    file_QCD_120_170.cd();
    nbkg = ((TH1D*)gDirectory->Get("hcalovecal05bkg"))->Integral(0,i);
    file_120_170.cd();
    eff = nsig / ((TH1D*)gDirectory->Get("hcalovecal05phot"))->GetEntries();
    file_QCD_120_170.cd();
    rej = 1- nbkg / ((TH1D*)gDirectory->Get("hcalovecal05bkg"))->GetEntries();
    hcalovecal05.Fill(eff,rej);
  }

  hcalovecal01.SetTitle("");  
  hcalovecal01.SetXTitle("Efficiency");
  hcalovecal01.SetYTitle("Rejection");
  hcalovecal01.SetTitleOffset(1.3,"Y");
  hcalovecal01.SetStats(0);
  hcalovecal01.Draw("");
  hcalovecal02.SetMarkerColor(kRed);
  hcalovecal02.Draw("same");
  hcalovecal025.SetMarkerColor(kBlue);
  hcalovecal025.Draw("same");
  hcalovecal03.SetMarkerColor(kGreen);
  hcalovecal03.Draw("same");
  hcalovecal04.SetMarkerColor(kMagenta);
  hcalovecal04.Draw("same");
  hcalovecal05.SetMarkerColor(39);
  hcalovecal05.Draw("same");
  leg3->Draw();
  c0->SaveAs("eps/effvsrej/rejvseffhcalovecal.eps");
  c0->SaveAs("eps/effvsrej/rejvseffhcalovecal.gif");

  hcalovecal01.SetAxisRange(.9,1.,"Y");
  hcalovecal01.Draw("");
  hcalovecal02.SetMarkerColor(kRed);
  hcalovecal02.Draw("same");
  hcalovecal025.SetMarkerColor(kBlue);
  hcalovecal025.Draw("same");
  hcalovecal03.SetMarkerColor(kGreen);
  hcalovecal03.Draw("same");
  hcalovecal04.SetMarkerColor(kMagenta);
  hcalovecal04.Draw("same");
  hcalovecal05.SetMarkerColor(39);
  hcalovecal05.Draw("same");

  c0->SaveAs("eps/effvsrej/rejvseffhcalovecalzoom.eps");
  c0->SaveAs("eps/effvsrej/rejvseffhcalovecalzoom.gif");

  TH2D ntrkiso("ntrkiso","rej vs eff ntrkiso",400,0.,1.,400,0.6,1.);

  file_120_170.cd();
  for (int i = 1; i<((TH1D*)gDirectory->Get("ntrkiso03phot"))->GetXaxis()->GetNbins()+1;i++){
    file_120_170.cd();
    int nsig = ((TH1D*)gDirectory->Get("ntrkiso03phot"))->Integral(0,i);
    file_QCD_120_170.cd();
    int nbkg = ((TH1D*)gDirectory->Get("ntrkiso03bkg"))->Integral(0,i);
    file_120_170.cd();
    double eff = nsig / ((TH1D*)gDirectory->Get("ntrkiso03phot"))->GetEntries();
    file_QCD_120_170.cd();
    double rej = 1-nbkg / ((TH1D*)gDirectory->Get("ntrkiso03bkg"))->GetEntries();
    ntrkiso.Fill(eff,rej);
   }

  ntrkiso.SetTitle("");  
  ntrkiso.SetXTitle("Efficiency");
  ntrkiso.SetYTitle("Rejection");
  ntrkiso.SetTitleOffset(1.3,"Y");
  ntrkiso.SetStats(0);
  ntrkiso.Draw("");

  c0->SaveAs("eps/effvsrej/rejvseffntrkiso.eps");
  c0->SaveAs("eps/effvsrej/rejvseffntrkiso.gif");

  TH2D sMajMaj("sMajMaj","rej vs eff sMajMaj",400,0.,1.,400,0.6,1.);

  file_120_170.cd();
  for (int i = 1; i<((TH1D*)gDirectory->Get("sMajMajphot"))->GetXaxis()->GetNbins()+1;i++){
    file_120_170.cd();
    int nsig = ((TH1D*)gDirectory->Get("sMajMajphot"))->Integral(0,i);
    file_QCD_120_170.cd();
    int nbkg = ((TH1D*)gDirectory->Get("sMajMajbkg"))->Integral(0,i);
    file_120_170.cd();
    double eff = nsig / ((TH1D*)gDirectory->Get("sMajMajphot"))->GetEntries();
    file_QCD_120_170.cd();
    double rej = 1-nbkg / ((TH1D*)gDirectory->Get("sMajMajbkg"))->GetEntries();
    sMajMaj.Fill(eff,rej);
   }

  sMajMaj.SetTitle("");  
  sMajMaj.SetXTitle("Efficiency");
  sMajMaj.SetYTitle("Rejection");
  sMajMaj.SetTitleOffset(1.3,"Y");
  sMajMaj.SetStats(0);
  sMajMaj.Draw("");

  c0->SaveAs("eps/effvsrej/rejvseffsMajMaj.eps");
  c0->SaveAs("eps/effvsrej/rejvseffsMajMaj.gif");

  sMajMaj.SetAxisRange(.9,1.,"Y");
  sMajMaj.Draw("");

  c0->SaveAs("eps/effvsrej/rejvseffsMajMajzoom.eps");
  c0->SaveAs("eps/effvsrej/rejvseffsMajMajzoom.gif");

  TH2D sMinMin("sMinMin","rej vs eff sMinMin",400,0.,1.,400,0.6,1.);

  file_120_170.cd();
  for (int i = 1; i<((TH1D*)gDirectory->Get("sMinMinphot"))->GetXaxis()->GetNbins()+1;i++){
    file_120_170.cd();
    int nsig = ((TH1D*)gDirectory->Get("sMinMinphot"))->Integral(0,i);
    file_QCD_120_170.cd();
    int nbkg = ((TH1D*)gDirectory->Get("sMinMinbkg"))->Integral(0,i);
    file_120_170.cd();
    double eff = nsig / ((TH1D*)gDirectory->Get("sMinMinphot"))->GetEntries();
    file_QCD_120_170.cd();
    double rej = 1-nbkg / ((TH1D*)gDirectory->Get("sMinMinbkg"))->GetEntries();
    sMinMin.Fill(eff,rej);
   }

  sMinMin.SetTitle("");  
  sMinMin.SetXTitle("Efficiency");
  sMinMin.SetYTitle("Rejection");
  sMinMin.SetTitleOffset(1.3,"Y");
  sMinMin.SetStats(0);
  sMinMin.Draw("");

  c0->SaveAs("eps/effvsrej/rejvseffsMinMin.eps");
  c0->SaveAs("eps/effvsrej/rejvseffsMinMin.gif");

  sMinMin.SetAxisRange(.9,1.,"Y");
  sMinMin.Draw("");

  c0->SaveAs("eps/effvsrej/rejvseffsMinMinzoom.eps");
  c0->SaveAs("eps/effvsrej/rejvseffsMinMinzoom.gif");

  TH2D nn("nn","rej vs eff nn",1000,0.,1.,1000,0.6,1.);
  TH2D nn_old("nn_old","rej vs eff nn",1000,0.,1.,1000,0.6,1.);

  file_120_170.cd();
  for (int i = 1; i<((TH1D*)gDirectory->Get("nnphot"))->GetXaxis()->GetNbins()+1;i++){
    file_120_170.cd();
    int nsig = ((TH1D*)gDirectory->Get("nnphot"))->Integral(0,i);
    file_QCD_120_170.cd();
    int nbkg = ((TH1D*)gDirectory->Get("nnbkg"))->Integral(0,i);
    file_120_170.cd();
    double eff = 1- nsig / ((TH1D*)gDirectory->Get("nnphot"))->GetEntries();
    file_QCD_120_170.cd();
    double rej = nbkg / ((TH1D*)gDirectory->Get("nnbkg"))->GetEntries();
    nn.Fill(eff,rej);
    file_120_170.cd();
    nsig = ((TH1D*)gDirectory->Get("nnphot_old"))->Integral(0,i);
    file_QCD_120_170.cd();
    nbkg = ((TH1D*)gDirectory->Get("nnbkg_old"))->Integral(0,i);
    file_120_170.cd();
    double eff = 1- nsig / ((TH1D*)gDirectory->Get("nnphot_old"))->GetEntries();
    file_QCD_120_170.cd();
    double rej = nbkg / ((TH1D*)gDirectory->Get("nnbkg_old"))->GetEntries();
    nn_old.Fill(eff,rej);
   }

  nn.SetTitle("");  
  nn.SetXTitle("Efficiency");
  nn.SetYTitle("Rejection");
  nn.SetTitleOffset(1.3,"Y");
  nn.SetStats(0);
  nn.Draw("");

  c0->SaveAs("eps/effvsrej/rejvseffnn.eps");
  c0->SaveAs("eps/effvsrej/rejvseffnn.gif");

  nn.SetAxisRange(.9,1.,"Y");
  nn.Draw("");

  c0->SaveAs("eps/effvsrej/rejvseffnnzoom.eps");
  c0->SaveAs("eps/effvsrej/rejvseffnnzoom.gif");

 nn.SetAxisRange(.6,1.,"Y");
   nn.Draw("");
  nn_old.SetMarkerColor(kRed);
  nn_old.Draw("same");
 
  c0->SaveAs("eps/effvsrej/rejvseffnnvsold.eps");
  c0->SaveAs("eps/effvsrej/rejvseffnnvsold.gif");
  nn.SetAxisRange(.9,1.,"Y");
   nn.Draw("");
  nn_old.SetMarkerColor(kRed);
  nn_old.Draw("same");
  c0->SaveAs("eps/effvsrej/rejvseffnnvsoldzoom.eps");


  nn.SetAxisRange(.75,1.,"Y");
  nn.Draw("");
  ptiso03.SetAxisRange(.75,1.,"Y");  
  ptiso03.SetMarkerColor(kRed);
  ptiso03.Draw("same");
  ntrkiso.SetAxisRange(.75,1.,"Y");  
  ntrkiso.SetMarkerColor(7);
  ntrkiso.Draw("same");
  hcalovecal04.SetAxisRange(.75,1.,"Y");
  hcalovecal04.SetMarkerColor(kBlue);
  hcalovecal04.Draw("same");
  rejvseffecalrec04.SetAxisRange(.75,1.,"Y");
  rejvseffecalrec04.SetMarkerColor(kGreen);
  rejvseffecalrec04.Draw("same");
  sMajMaj.SetAxisRange(.75,1.,"Y");  
  sMajMaj.SetMarkerColor(kMagenta);
  sMajMaj.Draw("same");
  sMinMin.SetAxisRange(.75,1.,"Y");  
  sMinMin.SetMarkerColor(39);
  sMinMin.Draw("same");
  TLegendEntry *legge5;
  TLegend *leg5;
  leg5 = new TLegend(0.15,0.15,0.5,0.5);
  leg5->SetFillStyle(0); leg5->SetBorderSize(0.); leg5->SetTextSize(0.05);
  leg5->SetFillColor(0);
  legge5 = leg5->AddEntry(&p1, "NN", "p");
  legge5 = leg5->AddEntry(&p2, "pt iso", "p");
  legge5 = leg5->AddEntry(&p7, "Ntrk iso", "p");
  legge5 = leg5->AddEntry(&p3, "HCAL/ECAL", "p");
  legge5 = leg5->AddEntry(&p4, "ECAL iso", "p");
  legge5 = leg5->AddEntry(&p5, "Cluster Major axis", "p");
  legge5 = leg5->AddEntry(&p6, "Cluster Minor axis", "p");

  leg5->Draw();

  c0->SaveAs("eps/effvsrej/rejvseffcomp.eps");
  c0->SaveAs("eps/effvsrej/rejvseffcomp.gif");

}
