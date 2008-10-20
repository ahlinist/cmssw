void plotL1Efficiency() {

	gROOT->LoadMacro("./tdrstyle.C");
	setTDRStyle();

	TFile* inFile = new TFile("test.root");
	TTree* tt1 = (TTree *) (inFile->Get("TTEffTree"));


  	//_____________________________________________________________

	// PFTau Et
  	tt1->Draw("PFTauEt>>hnum(25.,0.,100.)","hasMatchedL1Jet==1","e");
  	tt1->Draw("PFTauEt>>hden(25.,0.,100.)","","h");

        TH1F *hnum = (TH1F*)gDirectory->Get("hnum");
        TH1F *hden = (TH1F*)gDirectory->Get("hden");

  	//hnum->SetAxisRange(0.,1.,"X");
  	hnum->GetXaxis()->SetTitle("Tau jet E_{T} (GeV)");
  	hnum->GetYaxis()->SetTitle("Efficiency");
  	hnum->SetMarkerColor(kBlack);
  	hnum->SetMarkerSize(1.);
  	hnum->SetLineWidth(1);
  	hnum->SetLineColor(kBlack);
  	hnum->SetMarkerStyle(kFullDotLarge);

  	hnum->Divide(hden);
  	hnum->Draw("e");

	gPad->SaveAs("Eff_PFTauEt.C");

        // L1Tau Et
        tt1->Draw("L1JetEt>>hnum(25.,0.,100.)","hasMatchedL1Jet==1","e");
        tt1->Draw("L1JetEt>>hden(25.,0.,100.)","","h");

        TH1F *hnum = (TH1F*)gDirectory->Get("hnum");
        TH1F *hden = (TH1F*)gDirectory->Get("hden");

        //hnum->SetAxisRange(0.,1.,"X");
        hnum->GetXaxis()->SetTitle("L1 jet E_{T} (GeV)");
        hnum->GetYaxis()->SetTitle("Efficiency");
        hnum->SetMarkerColor(kBlack);
        hnum->SetMarkerSize(1.);
        hnum->SetLineWidth(1);
        hnum->SetLineColor(kBlack);
        hnum->SetMarkerStyle(kFullDotLarge);

        hnum->Divide(hden);
        hnum->Draw("e");

        gPad->SaveAs("Eff_L1JetEt.C");

}
