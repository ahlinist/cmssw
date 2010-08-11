#include "TMath.h"
#include <math.h>
#include "TF1.h"

void biasStudy(){

	//TFile *f = new TFile("biasStudyUps.root");
	//TFile *f = new TFile("biasStudyJ.root");
	TFile *f = new TFile("JpsiTagAndProbe_7TeV.root");
	TH1D *mG101a;
	mG101a = (TH1D*)gFile->Get("mG101a");
	TH1D *mG102a; 
	mG102a = (TH1D*)gFile->Get("mG102a");
	TH1D *r102c; 
	r102c = (TH1D*)gFile->Get("r102c");
	TH1D *r101c; 
	r101c = (TH1D*)gFile->Get("r101c");			
		
	TTree *t = (TTree*)f->Get("events");
	TH1D *hum = new TH1D("hum", "", 30, 2.8, 3.4);
	TH1D *ht = new TH1D("ht", "", 60, -3., 3.);
	TH1D *h0 = new TH1D("h0", "", 60, -3., 3.); setFilledHist(h0, kBlack, kYellow, 1000, 2);	
  	TH1D *h1 = new TH1D("h1", "", 60, -3., 3.); setFilledHist(h1, kRed, kRed, 3005, 2);
  	TH1D *h2 = new TH1D("h2", "", 60, -3., 3.); setFilledHist(h2, kCyan, kCyan, 3004, 2);
  	TH1D *h3 = new TH1D("h3", "", 60, -3., 3.); setFilledHist(h3, kBlue, kBlue, 3004, 2);
  	TH1D *h4 = new TH1D("h4", "", 60, -3., 3.); setFilledHist(h4, kBlack, 1, 3006, 2);
  	TH1D *h5 = new TH1D("h5", "", 60, -3., 3.); setFilledHist(h5, kMagenta, 1, 3006, 2);
	
	TH1D *h1pt = new TH1D("h1pt", "", 60, 0., 30.); setFilledHist(h1pt, kRed, kRed, 3005, 2);
	TH1D *h2pt = new TH1D("h2pt", "", 60, 0., 30.); setFilledHist(h2pt, kCyan, kCyan, 3004, 2);
	TH1D *h3pt = new TH1D("h3pt", "", 60, 0., 30.); setFilledHist(h3pt, kBlue, kBlue, 3004, 2);
  	TH1D *h4pt = new TH1D("h4pt", "", 60, 0., 30.); setFilledHist(h4pt, kBlack, 1, 3006, 2);
	TH1D *h5pt = new TH1D("h5pt", "", 60, 0., 30.); setFilledHist(h5pt, kMagenta, 1, 3006, 2);
	
	TH1D *h1R = new TH1D("h1R", "", 60, -3., 3.);
	TH1D *h2R = new TH1D("h2R", "", 60, -3., 3.); 
	TH1D *h3R = new TH1D("h3R", "", 60, -3., 3.); 
  	TH1D *h4R = new TH1D("h4R", "", 60, -3., 3.); 
	TH1D *h5R = new TH1D("h5R", "", 60, -3., 3.); 
	
	TH1D *h1Rpt = new TH1D("h1Rpt", "", 60, 0., 30.);
	TH1D *h2Rpt = new TH1D("h2Rpt", "", 60, 0., 30.); 
	TH1D *h3Rpt = new TH1D("h3Rpt", "", 60, 0., 30.); 
  	TH1D *h4Rpt = new TH1D("h4Rpt", "", 60, 0., 30.); 
	TH1D *h5Rpt = new TH1D("h5Rpt", "", 60, 0., 30.); 	
	
	
	
	TH1D *hphi_02  = new TH1D("hphi_02", "", 80, -4., 4.);
	TH1D *hphi_24  = new TH1D("hphi_24", "", 80, -4., 4.);
	TH1D *hphi_02m = new TH1D("hphi_02m", "", 80, -4., 4.);
	TH1D *hphi_24m = new TH1D("hphi_24m", "", 80, -4., 4.);	
	
	TH1D *htphi_02  = new TH1D("htphi_02", "", 80, -4., 4.);
	TH1D *htphi_24  = new TH1D("htphi_24", "", 80, -4., 4.);
	TH1D *htphi_02m = new TH1D("htphi_02m", "", 80, -4., 4.);
	TH1D *htphi_24m = new TH1D("htphi_24m", "", 80, -4., 4.);		
	
	TH1D *hteta_02  = new TH1D("hteta_02", "", 60, -3., 3.);
	TH1D *hteta_24  = new TH1D("hteta_24", "", 60, -3., 3.);
	TH1D *hteta_02m = new TH1D("hteta_02m", "", 60, -3., 3.);
	TH1D *hteta_24m = new TH1D("hteta_24m", "", 60, -3., 3.);	
	
	TH1D *htpt_02  = new TH1D("htpt_02", "", 60, 0., 30.);
	TH1D *htpt_24  = new TH1D("htpt_24", "", 60, 0., 30.);
	TH1D *htpt_02m = new TH1D("htpt_02m", "", 60, 0., 30.);
	TH1D *htpt_24m = new TH1D("htpt_24m", "", 60, 0., 30.);	
		
	TH1D *hdr_02  = new TH1D("hdr_02", "", 60, 0., 6.);
	TH1D *hdr_24  = new TH1D("hdr_24", "", 60, 0., 6.);
	TH1D *hdr_02m = new TH1D("hdr_02m", "", 60, 0., 6.);
	TH1D *hdr_24m = new TH1D("hdr_24m", "", 60, 0., 6.);	
	
	
	
	float pe, pp, te, tp, tP, dR, tf, pf, pm, um; 
  	t->SetBranchAddress("pe", &pe);
  	t->SetBranchAddress("te", &te);
  	t->SetBranchAddress("pp", &pp);
  	t->SetBranchAddress("tp", &tp);
	t->SetBranchAddress("tP", &tP);
	t->SetBranchAddress("dR", &dR);
	t->SetBranchAddress("pf", &pf);
	t->SetBranchAddress("tf", &tf);
	t->SetBranchAddress("pm", &pm);
	t->SetBranchAddress("um", &um);
	
  	Int_t nentries = Int_t(t->GetEntries());

  	Int_t nb(0), nbytes(0);
  	for (Int_t jentry = 0; jentry < nentries; jentry++) {
   	  nb = t->GetEntry(jentry);   nbytes += nb;
	  bool a = false;	
    	  ht->Fill(te); 
    	  h0->Fill(pe); 
    	  if ( TMath::Abs(te - pe) < 0.3 ) { h1->Fill(pe); h1pt->Fill(pp); }
	  
	  /// For Upsilon
	  
   	  //if (tp > 4 && TMath::Abs(te - pe) < 0.4) { h2->Fill(pe); h2pt->Fill(pp); }
    	  //if (tp > 6){ h3->Fill(pe);  h3pt->Fill(pp);}
    	  //if (TMath::Abs(te - pe) < 0.3){ h4->Fill(pe); h4pt->Fill(pp); }
    	  //if (TMath::Abs(te - pe) < 0.2){ h5->Fill(pe); h5pt->Fill(pp); }
	  
	  /// For JPsi
	  
   	  if ( TMath::Abs(te - pe) > 0.3) { h2->Fill(pe); h2pt->Fill(pp); }
    	  if ( TMath::Abs(te) < 1. &&  tp > 6.){ a = true;}
	  if ( TMath::Abs(te) > 1. ){a = true;}
	  if ( a ){ h3->Fill(pe);  h3pt->Fill(pp);}
	  
	  
    	  //if (TMath::Abs(te - pe) < 0.4){ h5->Fill(pe); h5pt->Fill(pp); }
    	  //if (dR < 1.){ 
	  h4->Fill(pe); h4pt->Fill(pp); //}

	  if ( TMath::Abs(pe) < 0.4 && TMath::Abs(pe) > 0.2 ){
	  	if ( pp < 5. && pp > 4. ){
			hphi_24->Fill(pf); htphi_24->Fill(tf);
			hteta_24->Fill(te); htpt_24->Fill(tp);
			hdr_24->Fill(dR);
			if ( pm & 0x1<<1 ){ 
				hphi_24m->Fill(pf); htphi_24m->Fill(tf); 
				hteta_24m->Fill(te); htpt_24m->Fill(tp);
				hdr_24m->Fill(dR);
			} 
	  	}
	  }
	
	  if ( TMath::Abs(pe) < 0.2 ){
	  	if ( pp < 5. && pp > 4. ){
			hphi_02->Fill(pf); htphi_02->Fill(tf);
			hteta_02->Fill(te); htpt_02->Fill(tp);
			hdr_02->Fill(dR);
			if ( pm & 0x1<<1 ){ 
				hphi_02m->Fill(pf); htphi_02m->Fill(tf);
				hteta_02m->Fill(te); htpt_02m->Fill(tp);
				hdr_02m->Fill(dR);
			} 
	  	}
	  }	
	  	
	
  	}
	
	gStyle->SetOptFit(111111111);
	TCanvas *c1 = new TCanvas("c1", "c1");
	r101c->SetLineColor(1);
	r101c->Draw();
	h4pt->Draw("same");
	h2pt->Draw("same");
	h5pt->Draw("same");	
	h3pt->Draw("same");
	h1pt->Draw("same");	
	
	
	for (int i=0; i<8 ; i++){
		r102c->SetBinContent(i,0.);
		h1->SetBinContent(i,0.);
		h3->SetBinContent(i,0.);
		h4->SetBinContent(i,0.);	
	}
	
	for (int i=55; i<61 ; i++){
		r102c->SetBinContent(i,0.);
		h1->SetBinContent(i,0.);
		h3->SetBinContent(i,0.);
		h4->SetBinContent(i,0.);	
	}	
		
	TCanvas *c2 = new TCanvas("c2", "c2");
	
	r102c->SetMinimum(0.);
	//r102c->SetMaximum(2400.);
	
	r102c->SetLineColor(3);
	r102c->Draw();
	h4->Draw("same");
	//h2->Draw("same");
	//h5->Draw("same");
	h3->Draw("same");
	h1->Draw("same");
	
	cout << h5->GetBinLowEdge(55) << endl;
	
	cout << h4->Integral(7,55) << endl;
	cout << h3->Integral(7,55) << endl;
	cout << h2->Integral(7,55) << endl;
	cout << h1->Integral(7,55) << endl;
	
	cout << h4->Integral() << endl;
	cout << h2->Integral() << endl;
	cout << h1->Integral() << endl;		
	
	TCanvas *c3 = new TCanvas("c3", "c3");
	(*h2R) = (*h2) / (*r102c);
	(*h3R) = (*h3) / (*r102c);
	(*h4R) = (*h4) / (*r102c);
	(*h5R) = (*h5) / (*r102c);
	(*h1R) = (*h1) / (*r102c);
	h3R->SetMinimum(0.);
	h3R->SetMaximum(1.5);
	//h2R->Draw();
	h3R->Draw();
	h4R->Draw("same");
	//h5R->Draw("same");
	h1R->Draw("same");
	
	TCanvas *c4 = new TCanvas("c4", "c4");
	(*h2Rpt) = (*h2pt) / (*r101c);
	(*h3Rpt) = (*h3pt) / (*r101c);
	(*h4Rpt) = (*h4pt) / (*r101c);
	(*h5Rpt) = (*h5pt) / (*r101c);
	(*h1Rpt) = (*h1pt) / (*r101c);
	h2Rpt->SetMinimum(0.);
	h2Rpt->SetMaximum(1.);	
	h2Rpt->Draw();
	h3Rpt->Draw("same");
	h4Rpt->Draw("same");
	h5Rpt->Draw("same");
	h1Rpt->Draw("same");	
	
	TCanvas *c5 = new TCanvas("c5", "c5");
	c5->Divide(5,2);
	c5->cd(1);	
	hphi_02->Draw();
	hphi_02m->SetLineColor(2);
	hphi_02m->Draw("same");
	
	c5->cd(2);
	hphi_24->Draw();
	hphi_24m->SetLineColor(2);
	hphi_24m->Draw("same");
	
	c5->cd(3);	
	htphi_02->Draw();
	htphi_02m->SetLineColor(2);
	htphi_02m->Draw("same");
	
	c5->cd(4);
	htphi_24->Draw();
	htphi_24m->SetLineColor(2);
	htphi_24m->Draw("same");	
	
	c5->cd(5);	
	htpt_02->Draw();
	htpt_02m->SetLineColor(2);
	htpt_02m->Draw("same");
	
	c5->cd(6);
	htpt_24->Draw();
	htpt_24m->SetLineColor(2);
	htpt_24m->Draw("same");	
	
	c5->cd(7);	
	hteta_02->Draw();
	hteta_02m->SetLineColor(2);
	hteta_02m->Draw("same");
	
	c5->cd(8);
	hteta_24->Draw();
	hteta_24m->SetLineColor(2);
	hteta_24m->Draw("same");	
	
	c5->cd(9);	
	hdr_02->Draw();
	hdr_02m->SetLineColor(2);
	hdr_02m->Draw("same");
	
	c5->cd(10);
	hdr_24->Draw();
	hdr_24m->SetLineColor(2);
	hdr_24m->Draw("same");	
		
}

void mass(){

	TFile *f = new TFile("JpsiTagAndProbe_7TeV.root");	
	TTree *t = (TTree*)f->Get("events");
	TH1D *hum = new TH1D("hum", "", 30, 2.8, 3.4);

	float um;
	t->SetBranchAddress("um", &um);

  	Int_t nentries = Int_t(t->GetEntries());

  	Int_t nb(0), nbytes(0);
  	for (Int_t jentry = 0; jentry < nentries; jentry++) {
   	  nb = t->GetEntry(jentry);   nbytes += nb;
	  hum->Fill(um);

	}
	
	//hum->Scale(1.7);
	
	gROOT->SetStyle("Plain");
	gStyle->SetOptStat(00000000000);
	TCanvas *c0 = new TCanvas("mass", "mass");
	hum->GetXaxis()->SetTitle("m_{#mu t}[GeV]");
	hum->GetYaxis()->SetTitle("Entries/Bin");
	hum->GetYaxis()->SetTitleOffset(1.2);
	hum->SetMarkerColor(1);
	hum->Draw("*PL");
	c0->SaveAs("jpsi_mass.eps");
}

void Mass(){

	TFile *f1 = new TFile("Ups1STagAndProbe_7TeV.root");	
	TTree *t1 = (TTree*)f1->Get("events");
	TFile *f2 = new TFile("Ups2STagAndProbe_7TeV.root");	
	TTree *t2 = (TTree*)f2->Get("events");	
	TFile *f3 = new TFile("Ups3STagAndProbe_7TeV.root");	
	TTree *t3 = (TTree*)f3->Get("events");	
	
	TH1D *hum = new TH1D("hum", "", 50, 8.7, 11.2);

	float um1, um2, um3;
	t1->SetBranchAddress("um", &um1);
	t2->SetBranchAddress("um", &um2);
	t3->SetBranchAddress("um", &um3);

  	Int_t nentries1 = Int_t(t1->GetEntries());
	Int_t nentries2 = Int_t(t2->GetEntries());
	Int_t nentries3 = Int_t(t3->GetEntries());
	
  	Int_t nb(0), nbytes(0);
  	for (Int_t jentry = 0; jentry < nentries1; jentry++) {
   	  nb = t1->GetEntry(jentry);   nbytes += nb;
	  hum->Fill(um1);
	}
	
	nb=0;
  	for (Int_t jentry = 0; jentry < nentries2; jentry++) {
   	  nb = t2->GetEntry(jentry);   nbytes += nb;
	  hum->Fill(um2,0.17);
	}	
	
	nb=0;
  	for (Int_t jentry = 0; jentry < nentries3; jentry++) {
   	  nb = t3->GetEntry(jentry);   nbytes += nb;
	  hum->Fill(um3,0.04);
	}	
	
	
	//hum->Scale(1.4);	
	
	gROOT->SetStyle("Plain");
	gStyle->SetOptStat(00000000000);
	TCanvas *c0 = new TCanvas("mass", "mass");
	hum->GetXaxis()->SetTitle("m_{#mu t}[GeV]");
	hum->GetYaxis()->SetTitle("Entries/Bin");
	hum->GetYaxis()->SetTitleOffset(1.4);
	hum->SetMarkerColor(1);
	hum->Draw("*PL");
	c0->SaveAs("upsilon_mass.eps");
}


void JpsiBiasStudy(){

	TFile *f = new TFile("JpsiTagAndProbe_7TeV_v3_tm.root");
	TH1D *r102c; 
	r102c = (TH1D*)gFile->Get("r102c");
	
	TH1D *R102c = new TH1D("R102c", "", 48, -2.4, 2.4);

	TTree *t = (TTree*)f->Get("events");
	TH1D *hum = new TH1D("hum", "", 30, 2.8, 3.4);
	TH1D *ht = new TH1D("ht", "", 48, -2.4, 2.4);
	TH1D *h0 = new TH1D("h0", "", 48, -2.4, 2.4); setFilledHist(h0, kBlack, kYellow, 1000, 2);
  	TH1D *h1 = new TH1D("h1", "", 48, -2.4, 2.4); setFilledHist(h1, kRed, kRed, 3006, 2);
  	TH1D *h2 = new TH1D("h2", "", 48, -2.4, 2.4); setFilledHist(h2, kCyan, kCyan, 3006, 2);
  	TH1D *h3 = new TH1D("h3", "", 48, -2.4, 2.4); setFilledHist(h3, kBlue+2, kBlue, 3006, 2);
  	TH1D *h4 = new TH1D("h4", "", 48, -2.4, 2.4); setFilledHist(h4, kBlack, 1, 3006, 2);
  	TH1D *h5 = new TH1D("h5", "", 48, -2.4, 2.4); setFilledHist(h5, kMagenta, 1, 3006, 2);
	TH1D *h6 = new TH1D("h6", "", 48, -2.4, 2.4); setFilledHist(h6, kGreen, 1, 3006, 2);
	TH1D *h7 = new TH1D("h7", "", 48, -2.4, 2.4); setFilledHist(h7, kAzure-5, 1, 3006, 2);
	TH1D *h8 = new TH1D("h8", "", 48, -2.4, 2.4); setFilledHist(h8, kPink+3, 1, 3006, 2);
	
	TH1D *h0R = new TH1D("h0R", "", 48, -2.4, 2.4);
	TH1D *h1R = new TH1D("h1R", "", 48, -2.4, 2.4);
	TH1D *h2R = new TH1D("h2R", "", 48, -2.4, 2.4); 
	TH1D *h3R = new TH1D("h3R", "", 48, -2.4, 2.4); 
  	TH1D *h4R = new TH1D("h4R", "", 48, -2.4, 2.4); 
	TH1D *h5R = new TH1D("h5R", "", 48, -2.4, 2.4);
	TH1D *h6R = new TH1D("h6R", "", 48, -2.4, 2.4);
	TH1D *h7R = new TH1D("h7R", "", 48, -2.4, 2.4);  		
	TH1D *h8R = new TH1D("h8R", "", 48, -2.4, 2.4);
	
	float pe, pp, te, tp, tP, dR, tf, pf, pm, um; 
  	t->SetBranchAddress("pe", &pe);
  	t->SetBranchAddress("te", &te);
  	t->SetBranchAddress("pp", &pp);
  	t->SetBranchAddress("tp", &tp);
	t->SetBranchAddress("tP", &tP);
	t->SetBranchAddress("dR", &dR);
	t->SetBranchAddress("pf", &pf);
	t->SetBranchAddress("tf", &tf);
	t->SetBranchAddress("pm", &pm);
	t->SetBranchAddress("um", &um);
	
  	Int_t nentries = Int_t(t->GetEntries());

  	Int_t nb(0), nbytes(0);
  	for (Int_t jentry = 0; jentry < nentries; jentry++) {
   	  nb = t->GetEntry(jentry);   nbytes += nb;
	  bool a = false;
	  bool b = false;	
    	  
	  
	  if ( (pm & 0x1<<1) && (pm & 0x1<<2) && (pm != -1) && ( pp >= 3. )){
	   
	   	ht->Fill(te);
    	  	h0->Fill(pe); 
    	  	if ( TMath::Abs(te - pe) < 0.3 ) { h1->Fill(pe); }	  	  
   	  	if ( TMath::Abs(te - pe) > 0.3 ) { h2->Fill(pe); }
	  	if ( tp > 4. ) h3->Fill(pe);
    	  	if ( TMath::Abs(te) < 1. &&  tp > 4.){ a = true;}
	  	if ( TMath::Abs(te) > 1. ){a = true;}
	  	if ( a ){ h4->Fill(pe); }
		if ( tp > 6. ) h5->Fill(pe);
		if ( TMath::Abs(te - pe) > 0.4 ) { h6->Fill(pe); }
		if ( TMath::Abs(te - pe) > 0.5 ) { h7->Fill(pe); }
		if ( TMath::Abs(te - pe) > 0.2 ) { h8->Fill(pe); }
	  }
	} 
	  gROOT->SetStyle("Plain");
	  gStyle->SetOptStat(00000000000);
	  TCanvas *c1 = new TCanvas("c1", "c1");	
	  h0->SetMinimum(0.);
	  h0->SetMaximum(20000.);
	  h0->Draw();
	  h0->GetXaxis()->SetTitle("#eta");
	  h0->GetYaxis()->SetTitle("Muons/Bin");
	  h0->GetYaxis()->SetTitleOffset(1.4);
	  ht->SetMarkerColor(1);
	  ht->SetLineColor(1);
	  ht->Draw("*Hsame");
	//  h4->Draw("same");
	  h2->Draw("same");
	  h5->Draw("same");
	  h3->Draw("same");
	  h1->Draw("same");
	  h6->Draw("same");
	  h7->Draw("same");
	  h8->Draw("same");	  
	  r102c->Scale(0.1);
	  r102c->SetMarkerColor(2);
	  r102c->SetLineColor(2);
	  r102c->Draw("*Hsame");	  
	  TGaxis *axis = new TGaxis(2.4,0,2.4,20000,0,200000,510,"+L"); 
	  axis->SetLineColor(2);
	  axis->SetLabelColor(2);
	  axis->Draw();
	  
	  
	  leg = new TLegend(0.3,0.6,0.7,0.9);
	  leg->AddEntry(r102c,"all muons","pl");
	  leg->AddEntry(ht,"all tags" ,"pl");
    	  leg->AddEntry(h0,"all probes" ,"f");
	//  leg->AddEntry(h4,"P_{T,tag}>4 GeV(in barrel)","f");
	  leg->AddEntry(h8,"|#eta_{tag}-#eta_{probe}|>0.2","f");
          leg->AddEntry(h3,"P_{T,tag}>4 GeV","f");
	  leg->AddEntry(h2,"|#eta_{tag}-#eta_{probe}|>0.3","f");
	  leg->AddEntry(h6,"|#eta_{tag}-#eta_{probe}|>0.4","f");
      	  leg->AddEntry(h1,"|#eta_{tag}-#eta_{probe}|<0.3","f");
	  leg->AddEntry(h7,"|#eta_{tag}-#eta_{probe}|>0.5","f");
    	  leg->AddEntry(h5,"P_{T,tag}>6 GeV","f");	  	  
          leg->Draw();
	  c1->SaveAs("jpsi_biasstudy.eps");
	 	  
	  cout << " All probes, # of etnries = " << h0->Integral() << endl;
	  cout << " TagPT > 4., # of etnries = "   << h3->Integral() << " TagPT > 4., eff =   " << h3->Integral()/h0->Integral() << endl;
	  cout << " TagPT > 4.(in the barrel), # of etnries = "   << h4->Integral() << " TagPT > 4.(in the barrel), eff =   " << h4->Integral()/h0->Integral() << endl;
	  cout << " TagPT > 6., # of etnries = "   << h5->Integral() << " TagPT > 6., eff =   " << h5->Integral()/h0->Integral() << endl;
	  cout << " DeltaEta < 0.3, # of etnries = "   << h1->Integral() << " DeltaEta < 0.3, eff =   " << h1->Integral()/h0->Integral() << endl;
	  cout << " DeltaEta > 0.2, # of etnries = "   << h8->Integral() << " DeltaEta > 0.2, eff =   " << h8->Integral()/h0->Integral() << endl;
	  cout << " DeltaEta > 0.3, # of etnries = "   << h2->Integral() << " DeltaEta > 0.3, eff =   " << h2->Integral()/h0->Integral() << endl;	  	  
	  cout << " DeltaEta > 0.4, # of etnries = "   << h6->Integral() << " DeltaEta > 0.4, eff =   " << h6->Integral()/h0->Integral() << endl;
	  cout << " DeltaEta > 0.5, # of etnries = "   << h7->Integral() << " DeltaEta > 0.5, eff =   " << h7->Integral()/h0->Integral() << endl;
	  
	  r102c->Scale(10.); 	  
	  TCanvas *c2 = new TCanvas("c2", "c2");
	  (*h0R) = (*h0) / (*r102c);
	  (*h1R) = (*h1) / (*r102c);
	  (*h2R) = (*h2) / (*r102c);
	  (*h3R) = (*h3) / (*r102c);
	  (*h4R) = (*h4) / (*r102c);
	  (*h5R) = (*h5) / (*r102c);
	  (*h6R) = (*h6) / (*r102c);
	  (*h7R) = (*h7) / (*r102c);
	  (*h8R) = (*h8) / (*r102c);
	  
	  h0R->SetMinimum(0.);
	  h0R->SetMaximum(0.1);
	  h0R->Draw();
	  h0R->GetXaxis()->SetTitle("#eta");
	  h0R->GetYaxis()->SetTitle("Ratio");
	  h0R->GetYaxis()->SetTitleOffset(1.2);
	//  h4R->Draw("same");
	  h2R->Draw("same");
	  h5R->Draw("same");
	  h3R->Draw("same");
	  h1R->Draw("same");
	  h6R->Draw("same");
	  h7R->Draw("same");
	  h8R->Draw("same");
	  leg1 = new TLegend(0.3,0.6,0.7,0.9);
	  leg1->AddEntry(h0R,"all probes" ,"f");
	//  leg1->AddEntry(h4R,"P_{T,tag}>4 GeV(in barrel)","f");
	  leg1->AddEntry(h8R,"|#eta_{tag}-#eta_{probe}|>0.2","f");
      	  leg1->AddEntry(h3R,"P_{T,tag}>4 GeV","f");
	  leg1->AddEntry(h2R,"|#eta_{tag}-#eta_{probe}|>0.3","f");
	  leg1->AddEntry(h6R,"|#eta_{tag}-#eta_{probe}|>0.4","f");
	  leg1->AddEntry(h1R,"|#eta_{tag}-#eta_{probe}|<0.3","f");
	  leg1->AddEntry(h7R,"|#eta_{tag}-#eta_{probe}|>0.5","f");
	  leg1->AddEntry(h5R,"P_{T,tag}>6 GeV","f");
          leg1->Draw();
	  c2->SaveAs("jpsi_biasstudy_ratio.eps");	
}

void UpsilonBiasStudy(){

	TFile *f = new TFile("Ups1STagAndProbe_7TeV_v3_tm.root");
	TH1D *r102c; 
	r102c = (TH1D*)gFile->Get("r102c");

	TH1D *R102c = new TH1D("R102c", "", 48, -2.4, 2.4);
	
	TTree *t = (TTree*)f->Get("events");
	TH1D *hum = new TH1D("hum", "", 30, 2.8, 3.4);
	TH1D *ht = new TH1D("ht", "", 48, -2.4, 2.4);
	TH1D *h0 = new TH1D("h0", "", 48, -2.4, 2.4); setFilledHist(h0, kBlack, kYellow, 1000, 2);
  	TH1D *h1 = new TH1D("h1", "", 48, -2.4, 2.4); setFilledHist(h1, kRed, kRed, 3006, 2);
  	TH1D *h2 = new TH1D("h2", "", 48, -2.4, 2.4); setFilledHist(h2, kCyan, kCyan, 3006, 2);
  	TH1D *h3 = new TH1D("h3", "", 48, -2.4, 2.4); setFilledHist(h3, kBlue+2, kBlue, 3006, 2);
  	TH1D *h4 = new TH1D("h4", "", 48, -2.4, 2.4); setFilledHist(h4, kBlack, 1, 3006, 2);
  	TH1D *h5 = new TH1D("h5", "", 48, -2.4, 2.4); setFilledHist(h5, kMagenta, 1, 3006, 2);
	TH1D *h6 = new TH1D("h6", "", 48, -2.4, 2.4); setFilledHist(h6, kGreen, 1, 3006, 2);
	TH1D *h7 = new TH1D("h7", "", 48, -2.4, 2.4); setFilledHist(h7, kAzure-5, 1, 3006, 2);
	TH1D *h8 = new TH1D("h8", "", 48, -2.4, 2.4); setFilledHist(h8, kPink+3, 1, 3006, 2);
	
	TH1D *h0R = new TH1D("h0R", "", 48, -2.4, 2.4); 
	TH1D *h1R = new TH1D("h1R", "", 48, -2.4, 2.4); 
	TH1D *h2R = new TH1D("h2R", "", 48, -2.4, 2.4); 
	TH1D *h3R = new TH1D("h3R", "", 48, -2.4, 2.4); 
  	TH1D *h4R = new TH1D("h4R", "", 48, -2.4, 2.4); 
	TH1D *h5R = new TH1D("h5R", "", 48, -2.4, 2.4);
	TH1D *h6R = new TH1D("h6R", "", 48, -2.4, 2.4); 
  	TH1D *h7R = new TH1D("h7R", "", 48, -2.4, 2.4); 
	TH1D *h8R = new TH1D("h8R", "", 48, -2.4, 2.4);		 	
	
	float pe, pp, te, tp, tP, dR, tf, pf, pm, um; 
  	t->SetBranchAddress("pe", &pe);
  	t->SetBranchAddress("te", &te);
  	t->SetBranchAddress("pp", &pp);
  	t->SetBranchAddress("tp", &tp);
	t->SetBranchAddress("tP", &tP);
	t->SetBranchAddress("dR", &dR);
	t->SetBranchAddress("pf", &pf);
	t->SetBranchAddress("tf", &tf);
	t->SetBranchAddress("pm", &pm);
	t->SetBranchAddress("um", &um);
	
  	Int_t nentries = Int_t(t->GetEntries());

  	Int_t nb(0), nbytes(0);
  	for (Int_t jentry = 0; jentry < nentries; jentry++) {
   	  nb = t->GetEntry(jentry);   nbytes += nb;
	  bool a = false;
	  bool b = false;	 
	  
	  if ( (pm & 0x1<<1) && (pm & 0x1<<2) && (pm != -1) && ( pp >= 3. )){
    	  	
		ht->Fill(te); 
    	  	h0->Fill(pe); 
    	  	if ( TMath::Abs(te - pe) < 0.3 ) { h1->Fill(pe); }	  	  
   	  	if ( TMath::Abs(te - pe) > 0.3 ) { h2->Fill(pe); }
	  	if ( tp > 4. ) h3->Fill(pe);
    	  	if ( TMath::Abs(te) < 1. &&  tp > 4.){ a = true;}
	  	if ( TMath::Abs(te) > 1. ){a = true;}
	  	if ( a ){ h4->Fill(pe); }
		if ( tp > 6. ) h5->Fill(pe);
		if ( TMath::Abs(te - pe) > 0.4 ) { h6->Fill(pe); }
		if ( TMath::Abs(te - pe) > 0.5 ) { h7->Fill(pe); }
		if ( TMath::Abs(te - pe) > 0.2 ) { h8->Fill(pe); }		
	  }
	} 
	  gROOT->SetStyle("Plain");
	  gStyle->SetOptStat(00000000000);
	  TCanvas *c5 = new TCanvas("c5", "c5");	
	  h0->SetMinimum(0.);
	  h0->SetMaximum(2200.);
	  h0->Draw();
	  h0->GetXaxis()->SetTitle("#eta");
	  h0->GetYaxis()->SetTitle("Muons/Bin");
	  h0->GetYaxis()->SetTitleOffset(1.2);
	  ht->SetMarkerColor(1);
	  ht->SetLineColor(1);
	  ht->Draw("*Hsame");
	  r102c->SetMarkerColor(2);
	  r102c->SetLineColor(2);
	  r102c->Draw("*Hsame");
	//  h4->Draw("same");
	  h2->Draw("same");
	  h5->Draw("same");
	  h3->Draw("same");
	  h1->Draw("same");
	  h6->Draw("same");
	  h7->Draw("same");
	  h8->Draw("same");
	  leg = new TLegend(0.3,0.4,0.7,0.7);
	  leg->AddEntry(r102c,"all muons","pl");
    	  leg->AddEntry(ht,"all tags" ,"pl");
	  leg->AddEntry(h0,"all probes" ,"f");
	//  leg->AddEntry(h4,"P_{T,tag}>4 GeV(in barrel)","f");	  
          leg->AddEntry(h3,"P_{T,tag}>4 GeV","f");
	  leg->AddEntry(h8,"|#eta_{tag}-#eta_{probe}|>0.2","f");	  
    	  leg->AddEntry(h2,"|#eta_{tag}-#eta_{probe}|>0.3","f");
	  leg->AddEntry(h6,"|#eta_{tag}-#eta_{probe}|>0.4","f");
	  leg->AddEntry(h7,"|#eta_{tag}-#eta_{probe}|>0.5","f");
	  leg->AddEntry(h1,"|#eta_{tag}-#eta_{probe}|<0.3","f");
	  leg->AddEntry(h5,"P_{T,tag}>6 GeV","f");	  
          leg->Draw();
	  c5->SaveAs("upsilon_biasstudy.eps");
	  
	  cout << " All probes, # of etnries = " << h0->Integral() << endl;
	  cout << " TagPT > 4., # of etnries = "   << h3->Integral() << " TagPT > 4., eff =   " << h3->Integral()/h0->Integral() << endl;
	  cout << " TagPT > 4.(in the barrel), # of etnries = "   << h4->Integral() << " TagPT > 4.(in the barrel), eff =   " << h4->Integral()/h0->Integral() << endl;
	  cout << " TagPT > 6., # of etnries = "   << h5->Integral() << " TagPT > 6., eff =   " << h5->Integral()/h0->Integral() << endl;
	  cout << " DeltaEta < 0.3, # of etnries = "   << h1->Integral() << " DeltaEta < 0.3, eff =   " << h1->Integral()/h0->Integral() << endl;
	  cout << " DeltaEta > 0.2, # of etnries = "   << h8->Integral() << " DeltaEta > 0.2, eff =   " << h8->Integral()/h0->Integral() << endl;  
	  cout << " DeltaEta > 0.3, # of etnries = "   << h2->Integral() << " DeltaEta > 0.3, eff =   " << h2->Integral()/h0->Integral() << endl;
	  cout << " DeltaEta > 0.4, # of etnries = "   << h6->Integral() << " DeltaEta > 0.4, eff =   " << h6->Integral()/h0->Integral() << endl;
	  cout << " DeltaEta > 0.5, # of etnries = "   << h7->Integral() << " DeltaEta > 0.5, eff =   " << h7->Integral()/h0->Integral() << endl;	  
	 
	  TCanvas *c6 = new TCanvas("c6", "c6");
	  (*h0R) = (*h0) / (*r102c);
	  (*h1R) = (*h1) / (*r102c);
	  (*h2R) = (*h2) / (*r102c);
	  (*h3R) = (*h3) / (*r102c);
	  (*h4R) = (*h4) / (*r102c);
	  (*h5R) = (*h5) / (*r102c);
	  (*h6R) = (*h6) / (*r102c);
	  (*h7R) = (*h7) / (*r102c);
	  (*h8R) = (*h8) / (*r102c);	  
	  
	  h0R->SetMinimum(0.);
	  h0R->SetMaximum(0.6);
	  h0R->Draw();
	  h0R->GetXaxis()->SetTitle("#eta");
	  h0R->GetYaxis()->SetTitle("Ratio");
	  h0R->GetYaxis()->SetTitleOffset(1.2);
	//  h4R->Draw("same");
	  h2R->Draw("same");
	  h5R->Draw("same");
	  h3R->Draw("same");
	  h1R->Draw("same");
	  h6R->Draw("same");
	  h7R->Draw("same");
	  h8R->Draw("same");	  
	  leg1 = new TLegend(0.3,0.6,0.7,0.9);
	  leg1->AddEntry(h0R,"all probes" ,"f");
	//  leg1->AddEntry(h4R,"P_{T,tag}>4 GeV(in barrel)","f");
	  leg1->AddEntry(h3R,"P_{T,tag}>4 GeV","f");
    	  leg1->AddEntry(h8R,"|#eta_{tag}-#eta_{probe}|>0.2","f");	  
    	  leg1->AddEntry(h2R,"|#eta_{tag}-#eta_{probe}|>0.3","f");	  
	  leg1->AddEntry(h6R,"|#eta_{tag}-#eta_{probe}|>0.4","f");
	  leg1->AddEntry(h7R,"|#eta_{tag}-#eta_{probe}|>0.5","f");
	  leg1->AddEntry(h1R,"|#eta_{tag}-#eta_{probe}|<0.3","f");
	  leg1->AddEntry(h5R,"P_{T,tag}>6 GeV","f");	  
          leg1->Draw();
	  c6->SaveAs("upsilon_biasstudy_ratio.eps");	
}

void JpsiPtStudy(){

	TFile *f = new TFile("JpsiTagAndProbe_7TeV_v3_tm.root");
	TH1D *r101c; 
	r101c = (TH1D*)gFile->Get("r101c");
	
	//TH1D *R102c = new TH1D("R102c", "", 48, -2.4, 2.4);

	TTree *t = (TTree*)f->Get("events");
	TH1D *hum = new TH1D("hum", "", 30, 2.8, 3.4);
	TH1D *ht = new TH1D("ht", "", 34, 3., 20.);
	TH1D *h0 = new TH1D("h0", "", 34, 3., 20.); setFilledHist(h0, kBlack, kYellow, 1000, 2);
  	TH1D *h1 = new TH1D("h1", "", 34, 3., 20.); setFilledHist(h1, kRed, kRed, 3006, 2);
  	TH1D *h2 = new TH1D("h2", "", 34, 3., 20.); setFilledHist(h2, kCyan, kCyan, 3006, 2);
  	TH1D *h3 = new TH1D("h3", "", 34, 3., 20.); setFilledHist(h3, kBlue+2, kBlue, 3006, 2);
  	TH1D *h4 = new TH1D("h4", "", 34, 3., 20.); setFilledHist(h4, kBlack, 1, 3006, 2);
  	TH1D *h5 = new TH1D("h5", "", 34, 3., 20.); setFilledHist(h5, kMagenta, 1, 3006, 2);
	TH1D *h6 = new TH1D("h6", "", 34, 3., 20.); setFilledHist(h6, kGreen, 1, 3006, 2);
	TH1D *h7 = new TH1D("h7", "", 34, 3., 20.); setFilledHist(h7, kAzure-5, 1, 3006, 2);
	TH1D *h8 = new TH1D("h8", "", 34, 3., 20.); setFilledHist(h8, kPink+3, 1, 3006, 2);
		
	TH1D *h0R = new TH1D("h0R", "", 34, 3., 20.);
	TH1D *h1R = new TH1D("h1R", "", 34, 3., 20.);
	TH1D *h2R = new TH1D("h2R", "", 34, 3., 20.); 
	TH1D *h3R = new TH1D("h3R", "", 34, 3., 20.); 
  	TH1D *h4R = new TH1D("h4R", "", 34, 3., 20.); 
	TH1D *h5R = new TH1D("h5R", "", 34, 3., 20.);
	TH1D *h6R = new TH1D("h6R", "", 34, 3., 20.);
	TH1D *h7R = new TH1D("h7R", "", 34, 3., 20.);
	TH1D *h8R = new TH1D("h8R", "", 34, 3., 20.);  		
	
	float pe, pp, te, tp, tP, dR, tf, pf, pm, um; 
  	t->SetBranchAddress("pe", &pe);
  	t->SetBranchAddress("te", &te);
  	t->SetBranchAddress("pp", &pp);
  	t->SetBranchAddress("tp", &tp);
	t->SetBranchAddress("tP", &tP);
	t->SetBranchAddress("dR", &dR);
	t->SetBranchAddress("pf", &pf);
	t->SetBranchAddress("tf", &tf);
	t->SetBranchAddress("pm", &pm);
	t->SetBranchAddress("um", &um);
	
  	Int_t nentries = Int_t(t->GetEntries());

  	Int_t nb(0), nbytes(0);
  	for (Int_t jentry = 0; jentry < nentries; jentry++) {
   	  nb = t->GetEntry(jentry);   nbytes += nb;
	  bool a = false;
	  bool b = false;	
    	  	  	  
	  if ( (pm & 0x1<<1) && (pm & 0x1<<2) && (pm != -1) && ( pp >= 3. ) ){
	   	
	   	ht->Fill(tp);
    	  	h0->Fill(pp); 
    	  	if ( TMath::Abs(te - pe) < 0.3 ) { h1->Fill(pp); }	  	  
   	  	if ( TMath::Abs(te - pe) > 0.3 ) { h2->Fill(pp); }	
	  	if ( tp > 4. ) h3->Fill(pp);
    	  	if ( TMath::Abs(te) < 1. &&  tp > 4.){ a = true;}
	  	if ( TMath::Abs(te) > 1. ){a = true;}
	  	if ( a ){ h4->Fill(pp); }
		if ( tp > 6. ) h5->Fill(pp);
		if ( TMath::Abs(te - pe) > 0.4 ) { h6->Fill(pp); }
		if ( TMath::Abs(te - pe) > 0.5 ) { h7->Fill(pp); }
		if ( TMath::Abs(te - pe) > 0.2 ) { h8->Fill(pp); }
	  }
	} 
	  gROOT->SetStyle("Plain");
	  gStyle->SetOptStat(00000000000);
	  TCanvas *c3 = new TCanvas("c3", "c3");	
	  h0->SetMinimum(0.);
	  h0->SetMaximum(110000.);
	  h0->Draw();
	  h0->GetXaxis()->SetTitle("P_{T}");
	  h0->GetYaxis()->SetTitle("Muons/Bin");
	  ht->SetMarkerColor(1);
	  ht->SetLineColor(1);
	  ht->Draw("*Hsame");
	//  h4->Draw("same");
	  h2->Draw("same");
	  h5->Draw("same");
	  h3->Draw("same");
	  h1->Draw("same");
	  h6->Draw("same");
	  h7->Draw("same");
	  h8->Draw("same");
	  r101c->Scale(0.1);
	  r101c->SetMarkerColor(2);
	  r101c->SetLineColor(2);
	  r101c->Draw("*Hsame");
	  TGaxis *axis = new TGaxis(20,0,20,110000,0,1100000,510,"+L"); 
	  axis->SetLineColor(2);
	  axis->SetLabelColor(2);
	  axis->Draw();
	  
	  leg = new TLegend(0.3,0.6,0.7,0.9);
	  leg->AddEntry(r101c,"all muons","pl");
	  leg->AddEntry(ht,"all tags" ,"pl");
    	  leg->AddEntry(h0,"all probes" ,"f");	  
	//  leg->AddEntry(h4,"P_{T,tag}>4 GeV(in barrel)","f");
	  leg->AddEntry(h8,"|#eta_{tag}-#eta_{probe}|>0.2","f");
	  leg->AddEntry(h3,"P_{T,tag}>4 GeV","f");
    	  leg->AddEntry(h2,"|#eta_{tag}-#eta_{probe}|>0.3","f");	  
	  leg->AddEntry(h6,"|#eta_{tag}-#eta_{probe}|>0.4","f");
	  leg->AddEntry(h1,"|#eta_{tag}-#eta_{probe}|<0.3","f");
	  leg->AddEntry(h7,"|#eta_{tag}-#eta_{probe}|>0.5","f");
	  leg->AddEntry(h5,"P_{T,tag}>6 GeV","f");
          leg->Draw();	
	  c3->SaveAs("jpsi_ptstudy.eps");
	 	  
	  cout << " All probes, # of etnries = " << h0->Integral() << endl;
	  cout << " TagPT > 4., # of etnries = "   << h3->Integral() << " TagPT > 4., eff =   " << h3->Integral()/h0->Integral() << endl;
	  cout << " TagPT > 4.(in the barrel), # of etnries = "   << h4->Integral() << " TagPT > 4.(in the barrel), eff =   " << h4->Integral()/h0->Integral() << endl;
	  cout << " TagPT > 6., # of etnries = "   << h5->Integral() << " TagPT > 6., eff =   " << h5->Integral()/h0->Integral() << endl;
	  cout << " DeltaEta < 0.3, # of etnries = "   << h1->Integral() << " DeltaEta < 0.3, eff =   " << h1->Integral()/h0->Integral() << endl;
	  cout << " DeltaEta > 0.2, # of etnries = "   << h8->Integral() << " DeltaEta > 0.2, eff =   " << h8->Integral()/h0->Integral() << endl;
	  cout << " DeltaEta > 0.3, # of etnries = "   << h2->Integral() << " DeltaEta > 0.3, eff =   " << h2->Integral()/h0->Integral() << endl;	  
	  cout << " DeltaEta > 0.4, # of etnries = "   << h6->Integral() << " DeltaEta > 0.4, eff =   " << h6->Integral()/h0->Integral() << endl;
	  cout << " DeltaEta > 0.5, # of etnries = "   << h7->Integral() << " DeltaEta > 0.5, eff =   " << h7->Integral()/h0->Integral() << endl;
	  r101c->Scale(5);
	  TCanvas *c4 = new TCanvas("c4", "c4");
	  (*h0R) = (*h0) / (*r101c);
	  (*h1R) = (*h1) / (*r101c);
	  (*h2R) = (*h2) / (*r101c);
	  (*h3R) = (*h3) / (*r101c);
	  (*h4R) = (*h4) / (*r101c);
	  (*h5R) = (*h5) / (*r101c);
	  (*h6R) = (*h6) / (*r101c);
	  (*h7R) = (*h7) / (*r101c);
	  (*h8R) = (*h8) / (*r101c);
	  
	  h0R->SetMinimum(0.);
	  h0R->SetMaximum(0.6);
	  h0R->Draw();
	  h0R->GetXaxis()->SetTitle("P_{T}");
	  h0R->GetYaxis()->SetTitle("Ratio");
	  h0R->GetYaxis()->SetTitleOffset(1.2);
	//  h4R->Draw("same");
	  h2R->Draw("same");
	  h5R->Draw("same");
	  h3R->Draw("same");
	  h1R->Draw("same");
	  h6R->Draw("same");
	  h7R->Draw("same");
	  h8R->Draw("same");
	  leg1 = new TLegend(0.3,0.6,0.7,0.9);
	  leg1->AddEntry(h0R,"all probes" ,"f");	  
	//  leg1->AddEntry(h4R,"P_{T,tag}>4 GeV(in barrel)","f");
	  leg1->AddEntry(h8R,"|#eta_{tag}-#eta_{probe}|>0.2","f");
      	  leg1->AddEntry(h3R,"P_{T,tag}>4 GeV","f");
    	  leg1->AddEntry(h2R,"|#eta_{tag}-#eta_{probe}|>0.3","f");	  
	  leg1->AddEntry(h6R,"|#eta_{tag}-#eta_{probe}|>0.4","f");
	  leg1->AddEntry(h1R,"|#eta_{tag}-#eta_{probe}|<0.3","f");
	  leg1->AddEntry(h7R,"|#eta_{tag}-#eta_{probe}|>0.5","f");
	  leg1->AddEntry(h5R,"P_{T,tag}>6 GeV","f");
          leg1->Draw();
	  c4->SaveAs("jpsi_ptstudy_ratio.eps");
}

void UpsilonPtStudy(){

	TFile *f = new TFile("Ups1STagAndProbe_7TeV_v3_tm.root");
	TH1D *r101c; 
	r101c = (TH1D*)gFile->Get("r101c");
	
	//TH1D *R102c = new TH1D("R102c", "", 48, -2.4, 2.4);

	TTree *t = (TTree*)f->Get("events");
	TH1D *hum = new TH1D("hum", "", 30, 2.8, 3.4);
	TH1D *ht = new TH1D("ht", "", 34, 3., 20.);
	TH1D *h0 = new TH1D("h0", "", 34, 3., 20.); setFilledHist(h0, kBlack, kYellow, 1000, 2);
  	TH1D *h1 = new TH1D("h1", "", 34, 3., 20.); setFilledHist(h1, kRed, kRed, 3006, 2);
  	TH1D *h2 = new TH1D("h2", "", 34, 3., 20.); setFilledHist(h2, kCyan, kCyan, 3006, 2);
  	TH1D *h3 = new TH1D("h3", "", 34, 3., 20.); setFilledHist(h3, kBlue+2, kBlue, 3006, 2);
  	TH1D *h4 = new TH1D("h4", "", 34, 3., 20.); setFilledHist(h4, kBlack, 1, 3006, 2);
  	TH1D *h5 = new TH1D("h5", "", 34, 3., 20.); setFilledHist(h5, kMagenta, 1, 3006, 2);
	TH1D *h6 = new TH1D("h6", "", 34, 3., 20.); setFilledHist(h6, kGreen, 1, 3006, 2);
	TH1D *h7 = new TH1D("h7", "", 34, 3., 20.); setFilledHist(h7, kAzure-5, 1, 3006, 2);
	TH1D *h8 = new TH1D("h8", "", 34, 3., 20.); setFilledHist(h8, kPink+3, 1, 3006, 2);
		
	TH1D *h0R = new TH1D("h0R", "", 34, 3., 20.);
	TH1D *h1R = new TH1D("h1R", "", 34, 3., 20.);
	TH1D *h2R = new TH1D("h2R", "", 34, 3., 20.); 
	TH1D *h3R = new TH1D("h3R", "", 34, 3., 20.); 
  	TH1D *h4R = new TH1D("h4R", "", 34, 3., 20.); 
	TH1D *h5R = new TH1D("h5R", "", 34, 3., 20.);
	TH1D *h6R = new TH1D("h6R", "", 34, 3., 20.);
	TH1D *h7R = new TH1D("h7R", "", 34, 3., 20.);
	TH1D *h8R = new TH1D("h8R", "", 34, 3., 20.);  		
	
	float pe, pp, te, tp, tP, dR, tf, pf, pm, um; 
  	t->SetBranchAddress("pe", &pe);
  	t->SetBranchAddress("te", &te);
  	t->SetBranchAddress("pp", &pp);
  	t->SetBranchAddress("tp", &tp);
	t->SetBranchAddress("tP", &tP);
	t->SetBranchAddress("dR", &dR);
	t->SetBranchAddress("pf", &pf);
	t->SetBranchAddress("tf", &tf);
	t->SetBranchAddress("pm", &pm);
	t->SetBranchAddress("um", &um);
	
  	Int_t nentries = Int_t(t->GetEntries());

  	Int_t nb(0), nbytes(0);
  	for (Int_t jentry = 0; jentry < nentries; jentry++) {
   	  nb = t->GetEntry(jentry);   nbytes += nb;
	  bool a = false;
	  bool b = false;	
    	  	  	  
	  if ( (pm & 0x1<<1) && (pm & 0x1<<2) && (pm != -1) && ( pp >= 3. ) ){
	   	
	   	ht->Fill(tp);
    	  	h0->Fill(pp); 
    	  	if ( TMath::Abs(te - pe) < 0.3 ) { h1->Fill(pp); }	  	  
   	  	if ( TMath::Abs(te - pe) > 0.3 ) { h2->Fill(pp); }	
	  	if ( tp > 4. ) h3->Fill(pp);
    	  	if ( TMath::Abs(te) < 1. &&  tp > 4.){ a = true;}
	  	if ( TMath::Abs(te) > 1. ){a = true;}
	  	if ( a ){ h4->Fill(pp); }
		if ( tp > 6. ) h5->Fill(pp);
		if ( TMath::Abs(te - pe) > 0.4 ) { h6->Fill(pp); }
		if ( TMath::Abs(te - pe) > 0.5 ) { h7->Fill(pp); }
		if ( TMath::Abs(te - pe) > 0.2 ) { h8->Fill(pp); }
	  }
	} 
	  gROOT->SetStyle("Plain");
	  gStyle->SetOptStat(00000000000);
	  TCanvas *c7 = new TCanvas("c7", "c7");	
	  h0->SetMinimum(0.);
	  h0->SetMaximum(11500.);
	  h0->Draw();
	  h0->GetXaxis()->SetTitle("P_{T}");
	  h0->GetYaxis()->SetTitle("Muons/Bin");
	  h0->GetYaxis()->SetTitleOffset(1.3);
	  ht->SetMarkerColor(1);
	  ht->SetLineColor(1);
	  ht->Draw("*Hsame");
	  r101c->SetMarkerColor(2);
	  r101c->SetLineColor(2);
	  r101c->Draw("*Hsame");
	//  h4->Draw("same");
	  h2->Draw("same");
	  h5->Draw("same");
	  h3->Draw("same");
	  h1->Draw("same");
	  h6->Draw("same");
	  h7->Draw("same");
	  h8->Draw("same");
	  leg = new TLegend(0.3,0.6,0.7,0.9);
	  leg->AddEntry(r101c,"all muons","pl");
	  leg->AddEntry(ht,"all tags" ,"pl");
    	  leg->AddEntry(h0,"all probes" ,"f");	  
	//  leg->AddEntry(h4,"P_{T,tag}>4 GeV(in barrel)","f");
      	  leg->AddEntry(h3,"P_{T,tag}>4 GeV","f");
	  leg->AddEntry(h8,"|#eta_{tag}-#eta_{probe}|>0.2","f");
    	  leg->AddEntry(h2,"|#eta_{tag}-#eta_{probe}|>0.3","f");	  
	  leg->AddEntry(h6,"|#eta_{tag}-#eta_{probe}|>0.4","f");
	  leg->AddEntry(h7,"|#eta_{tag}-#eta_{probe}|>0.5","f");
	  leg->AddEntry(h1,"|#eta_{tag}-#eta_{probe}|<0.3","f");
	  leg->AddEntry(h5,"P_{T,tag}>6 GeV","f");
          leg->Draw();	
	  c7->SaveAs("upsilon_ptstudy.eps");
	 	  
	  cout << " All probes, # of etnries = " << h0->Integral() << endl;
	  cout << " TagPT > 4., # of etnries = "   << h3->Integral() << " TagPT > 4., eff =   " << h3->Integral()/h0->Integral() << endl;
	  cout << " TagPT > 4.(in the barrel), # of etnries = "   << h4->Integral() << " TagPT > 4.(in the barrel), eff =   " << h4->Integral()/h0->Integral() << endl;
	  cout << " TagPT > 6., # of etnries = "   << h5->Integral() << " TagPT > 6., eff =   " << h5->Integral()/h0->Integral() << endl;
	  cout << " DeltaEta < 0.3, # of etnries = "   << h1->Integral() << " DeltaEta < 0.3, eff =   " << h1->Integral()/h0->Integral() << endl;
	  cout << " DeltaEta > 0.2, # of etnries = "   << h8->Integral() << " DeltaEta > 0.2, eff =   " << h8->Integral()/h0->Integral() << endl;
	  cout << " DeltaEta > 0.3, # of etnries = "   << h2->Integral() << " DeltaEta > 0.3, eff =   " << h2->Integral()/h0->Integral() << endl;	  
	  cout << " DeltaEta > 0.4, # of etnries = "   << h6->Integral() << " DeltaEta > 0.4, eff =   " << h6->Integral()/h0->Integral() << endl;
	  cout << " DeltaEta > 0.5, # of etnries = "   << h7->Integral() << " DeltaEta > 0.5, eff =   " << h7->Integral()/h0->Integral() << endl;
	  
	  TCanvas *c8 = new TCanvas("c8", "c8");
	  (*h0R) = (*h0) / (*r101c);
	  (*h1R) = (*h1) / (*r101c);
	  (*h2R) = (*h2) / (*r101c);
	  (*h3R) = (*h3) / (*r101c);
	  (*h4R) = (*h4) / (*r101c);
	  (*h5R) = (*h5) / (*r101c);
	  (*h6R) = (*h6) / (*r101c);
	  (*h7R) = (*h7) / (*r101c);
	  (*h8R) = (*h8) / (*r101c);
	  
	  h0R->SetMinimum(0.);
	  h0R->SetMaximum(0.6);
	  h0R->Draw();
	  h0R->GetXaxis()->SetTitle("P_{T}");
	  h0R->GetYaxis()->SetTitle("Ratio");
	  h0R->GetYaxis()->SetTitleOffset(1.2);
	//  h4R->Draw("same");
	  h2R->Draw("same");
	  h5R->Draw("same");
	  h3R->Draw("same");
	  h1R->Draw("same");
	  h6R->Draw("same");
	  h7R->Draw("same");
	  h8R->Draw("same");
	  leg1 = new TLegend(0.3,0.6,0.7,0.9);
	  leg1->AddEntry(h0R,"all probes" ,"f");	  
	//  leg1->AddEntry(h4R,"P_{T,tag}>4 GeV(in barrel)","f");
      	  leg1->AddEntry(h3R,"P_{T,tag}>4 GeV","f");
	  leg1->AddEntry(h8R,"|#eta_{tag}-#eta_{probe}|>0.2","f");
    	  leg1->AddEntry(h2R,"|#eta_{tag}-#eta_{probe}|>0.3","f");	  
	  leg1->AddEntry(h6R,"|#eta_{tag}-#eta_{probe}|>0.4","f");
	  leg1->AddEntry(h7R,"|#eta_{tag}-#eta_{probe}|>0.5","f");
	  leg1->AddEntry(h1R,"|#eta_{tag}-#eta_{probe}|<0.3","f");
	  leg1->AddEntry(h5R,"P_{T,tag}>6 GeV","f");
          leg1->Draw();
	  c8->SaveAs("upsilon_ptstudy_ratio.eps");
}

void biasplots(){
	int Npt = 6;
	double PTbin[] = {0., 2., 3., 4., 5., 6., 20.};											
	int Neta = 5;
	double Etabin[] = {-2.4, -1.2, -0.4, 0.4, 1.2, 2.4};
								
	//TFile *f = new TFile("BiasNeg_Nocut.root");
	TFile *f = new TFile("BiasNeg_DeltaEta0.3Inv.root");
	TH2D *hBiasNeg;
	hBiasNeg = new TH2D("hBiasNeg", "hBiasNeg", Neta, Etabin, Npt, PTbin);
	hBiasNeg = (TH2D*)gFile->Get("BiasNeg");
		
	//TFile *f1 = new TFile("BiasPos_Nocut.root");
	TFile *f1 = new TFile("BiasPos_DeltaEta0.3Inv.root");
	TH2D *hBiasPos;
	hBiasPos = new TH2D("hBiasPos", "hBiasPos", Neta, Etabin, Npt, PTbin);
	hBiasPos = (TH2D*)gFile->Get("BiasPos");
	
	TCanvas *c0 = new TCanvas("c0", "c0");
	c0->Divide(1,2);
	c0->cd(1);
	hBiasNeg->Draw("colz");
	c0->cd(2);
	hBiasPos->Draw("colz");
	
	//TFile *f2 = new TFile("JpsiTagAndProbe_7TeV_v2.root");
	TFile *f2 = new TFile("JpsiTagAndProbe_7TeV_DeltaEta0.3Inv_v2.root");
	TTree *t = (TTree*)f2->Get("muons");
	TH1D *h0 = new TH1D("Positive", "Positive", 500, -0.1, 0.1); setFilledHist(h0, kBlack, kYellow, 1000, 2);
  	TH1D *h1 = new TH1D("Negative", "Negative", 500, -0.1, 0.1); setFilledHist(h1, kRed, kRed, 3006, 2);
	float posmuoneta, posmuonpt, negmuoneta, negmuonpt; 
  	t->SetBranchAddress("posmuoneta", &posmuoneta);
  	t->SetBranchAddress("posmuonpt", &posmuonpt);
  	t->SetBranchAddress("negmuoneta", &negmuoneta);
  	t->SetBranchAddress("negmuonpt", &negmuonpt);
	double bin = -1;double Bin = -1;
	Int_t nentries = Int_t(t->GetEntries());
	
  	Int_t nb(0), nbytes(0);
  	for (Int_t jentry = 0; jentry < nentries; jentry++) {
   	  nb = t->GetEntry(jentry);   nbytes += nb;

	  if ( posmuoneta > -3 && posmuonpt > 0 ) {
	  
		if ( bin == hBiasPos->FindBin(posmuoneta,posmuonpt) ) continue;
	  	bin = hBiasPos->FindBin(posmuoneta,posmuonpt);
		cout << "BIN = " <<bin<<endl;
	  	cout<<"posmuoneta = "<<posmuoneta<<" posmuonpt = "<<posmuonpt<<endl;
		cout<<" BinContent ="<<hBiasPos->GetBinContent(bin)<<endl;
		h0->Fill(hBiasPos->GetBinContent(bin));
		
	  	}
	  
	  if ( negmuoneta > -3 && negmuonpt > 0 ) {
	  
		if ( Bin == hBiasNeg->FindBin(negmuoneta,negmuonpt) ) continue;
	  	Bin = hBiasNeg->FindBin(negmuoneta,negmuonpt);
		cout << "BIN = " <<Bin<<endl;
	  	cout<<"negmuoneta = "<<negmuoneta<<" negmuonpt = "<<negmuonpt<<endl;
		cout<<" BinContent ="<<hBiasNeg->GetBinContent(Bin)<<endl;
		h1->Fill(hBiasNeg->GetBinContent(Bin));
	  	
	  } 	
	  
	}

	  TCanvas *c1 = new TCanvas("c1", "c1");
	  c1->Divide(1,2);
	  c1->cd(1);
	  h0->Draw();
	  c1->cd(2);
	  h1->Draw();
	  //c1->SaveAs("JPsi_BiasPlots_Nocut.eps");
	  c1->SaveAs("JPsi_BiasPlots_DeltaEta0.3Inv.eps");
	  
}
