double* IfitBin(TH1D* dataInput, TH1D* sigTemplate, TH1D* bkgTemplate)
{
   // Start TFractionFitter
   double Spara(0), eSpara(0);
   double Bpara(0), eBpara(0);
   TObjArray *mc = new TObjArray(2);
   mc->Add(sigTemplate);
   mc->Add(bkgTemplate);

   TFractionFitter *fitTemplate = new TFractionFitter(dataInput, mc);
   fitTemplate->Constrain(0, 0.0, 1.0);
   fitTemplate->Constrain(1, 0.0, 1.0);
   int status = fitTemplate->Fit();
   cout<<" Fitting status = "<<status<<endl;
   if (status == 0) {
      fitTemplate->GetResult(0, Spara, eSpara);
      fitTemplate->GetResult(1, Bpara, eBpara);

      cout<<" Fitting result = "<<endl;
      cout<<"               Chi2 = "<<fitTemplate->GetChisquare()<<endl;
      cout<<"                NDF = "<<fitTemplate->GetNDF()<<endl;
      cout<<"               Prob = "<<fitTemplate->GetProb()<<endl;
      cout<<"      Signal fraction     = "<<Spara<<"; Error = "<<eSpara<<endl;
      cout<<"      Background fraction = "<<Bpara<<"; Error = "<<eBpara<<endl;
   }

   TH1D *FitResultReal = (TH1D*)sigTemplate->Clone();
   TH1D *FitResultFake = (TH1D*)bkgTemplate->Clone();
   TH1D *FitResultAll  = (TH1D*)dataInput->Clone();
   FitResultReal->SetName("ResultReal");
   FitResultFake->SetName("ResultFake");
   FitResultAll->SetName("ResultAll");
   FitResultReal->Scale(1./FitResultReal->Integral()*Spara*dataInput->Integral());
   FitResultFake->Scale(1./FitResultFake->Integral()*Bpara*dataInput->Integral());
   FitResultAll->Reset();
   FitResultAll->Add(FitResultReal);
   FitResultAll->Add(FitResultFake);
   
   TCanvas *c1 = new TCanvas("c1", "", 600, 400);
   c1->cd();
   FitResultAll->SetXTitle("#sigma_{i#etai#eta}");
   FitResultAll->SetYTitle("Number of photons");
   FitResultAll->SetMinimum(0);
   FitResultAll->SetMaximum(FitResultAll->GetMaximum()*1.4);
   FitResultAll->SetLineColor(1);
   FitResultAll->SetLineWidth(2);
   FitResultAll->Draw();
   dataInput->SetMarkerStyle(21);
   dataInput->SetMarkerSize(0.7);
   dataInput->SetLineColor(1);
   dataInput->SetLineWidth(2);
   dataInput->Draw("PE1same");
   FitResultReal->SetLineColor(2);
   FitResultReal->SetFillColor(2);
   FitResultReal->SetFillStyle(3002);
   FitResultReal->Draw("same");
   FitResultFake->SetLineColor(4);
   FitResultFake->SetFillColor(4);
   FitResultFake->SetFillStyle(3004);
   FitResultFake->Draw("same");
   TLegend *leg1 = new TLegend(0.5,0.5,0.9,0.85);
   char text[200];
   leg1->SetFillColor(0);
   leg1->SetShadowColor(0);
   leg1->SetFillStyle(0);
   leg1->SetBorderSize(0);
   leg1->SetLineColor(0);
   sprintf(text,"Data: %5.1f events", dataInput->Integral());
   leg1->AddEntry(dataInput, text, "pl");
   sprintf(text,"Fitted: %5.1f events", FitResultAll->Integral());
   leg1->AddEntry(FitResultAll, text, "l");
   sprintf(text,"Signal %5.1f #pm %5.1f events", FitResultReal->Integral(), eSpara/Spara*FitResultReal->Integral());
   leg1->AddEntry(FitResultReal, text, "f");
   sprintf(text,"Background %5.1f #pm %5.1f events", FitResultFake->Integral(), eBpara/Bpara*FitResultFake->Integral());
   leg1->AddEntry(FitResultFake, text, "f");
   leg1->Draw();

   return;
}
