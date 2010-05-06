{
gROOT->Reset();
gROOT->Clear();

gStyle->SetNdivisions(5);
gStyle->SetCanvasBorderMode(0); 
gStyle->SetPadBorderMode(1);
gStyle->SetOptTitle(1);
gStyle->SetStatFont(42);
gStyle->SetTitleFont(22);
gStyle->SetCanvasColor(10);
gStyle->SetPadColor(0);
gStyle->SetLabelFont(42,"x");
gStyle->SetLabelFont(42,"y");
gStyle->SetHistFillStyle(1001);
gStyle->SetHistFillColor(0);
gStyle->SetHistLineStyle(1);
gStyle->SetHistLineWidth(3);
//gStyle->SetLineWidth(3);
gStyle->SetHistLineColor(9);
gStyle->SetTitleXOffset(1.1);
gStyle->SetTitleYOffset(1.15);
//gStyle->SetOptStat(1111);
//gStyle->SetOptStat(kFALSE);
//gStyle->SetOptFile("Data.root");
gStyle->SetOptStat("nemr");
//gStyle->SetOptTitle("data");

//gStyle->SetOptFit(1111);
gStyle->SetStatH(0.25); 
gStyle->SetStatFontSize(0.04);



TCanvas *Canv = new TCanvas("Canv","Canv",129,17,926,703);
Canv->SetBorderSize(2);
Canv->SetFrameFillColor(0);
Canv->SetLogy(0);
Canv->cd();


 // TFile *MC = TFile::Open("MC.root");
//  TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
// MC -> cd("analyzeBasicPat");
// TH1F *mjeteta = (TH1F*)gDirectory->Get("jetEta");
// //TH1F *mjeteta = new TH1F("jetEta","jetEta",100,-3.,3.);
// mjeteta->SetLineColor(2);
// mjeteta->SetMarkerStyle(21);
// mjeteta->SetMarkerColor(2);
// mjeteta->SetMarkerSize(1);
// mjeteta->SetLineWidth(3);
// //Double_t scale = 1/mjeteta->Integral();

//  mjeteta->Scale(1/mjeteta->Integral());
// mjeteta->Draw();

//  TFile *DT = TFile::Open("Data.root");
//  TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
//  DT -> cd("analyzeBasicPat");
//   TH1F *djeteta = (TH1F*)gDirectory->Get("jetEta"); 
// //   //new TH1F("jetEta","jetEta",100,-3.,3.);
//  djeteta->SetLineColor(1);
//  djeteta->SetMarkerStyle(22);
//  djeteta->SetMarkerColor(1);
//  djeteta->SetMarkerSize(0.7);
//  djeteta->SetLineWidth(3);
// djeteta->Scale(1/djeteta->Integral());
//   djeteta->Draw("same");
// //Canv->SaveAs("JetEta.gif");


// TFile *MC = TFile::Open("MC.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
// MC -> cd ("analyzeBasicPat");
//  TH1F *mmueta  = (TH1F*)gDirectory->Get("muonEta");
//    //= new TH1F("muonEta","muonEta",100,-3.,3.);
//  mmueta->SetLineColor(2);
//  mmueta->SetMarkerStyle(21);
//  mmueta->SetMarkerColor(2);
//  mmueta->SetMarkerSize(0.7);
//  mmueta->SetLineWidth(3);
//  mmueta->Scale(1/mmueta->Integral());
//  mmueta->Draw();

// TFile *DT = TFile::Open("Data.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
// DT -> cd ("analyzeBasicPat");
//  TH1F *dmueta   = (TH1F*)gDirectory->Get("muonEta");
//    //new TH1F("muonEta","muonEta",100,-3.,3.);
// dmueta->SetLineColor(1);
//  dmueta->SetMarkerStyle(22);
//  dmueta->SetMarkerColor(1);
//  dmueta->SetMarkerSize(0.7);
//  dmueta->SetLineWidth(3);
//  dmueta->Scale(1/dmueta->Integral());
//  dmueta->Draw("same");
//  //Canv->SaveAs("MuonEta.gif");


// TFile *MC = TFile::Open("MC.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
// MC -> cd ("analyzeBasicPat");
//  TH1F *mtaueta =   (TH1F*)gDirectory->Get("tauEta");
//    //new TH1F("tauEta","tauEta",100,-3.,3.);
// mtaueta->SetLineColor(2);
//  mtaueta->SetMarkerStyle(21);
//  mtaueta->SetMarkerColor(2);
//  mtaueta->SetMarkerSize(0.7);
//  mtaueta->SetLineWidth(3);
//  mtaueta->Scale(1/mtaueta->Integral());
//  mtaueta->Draw();

// TFile *DT = TFile::Open("Data.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
// DT -> cd ("analyzeBasicPat");
//  TH1F *dtaueta =   (TH1F*)gDirectory->Get("tauEta");
//  //new TH1F("tauEta","tauEta",100,-3.,3.);
// dtaueta->SetLineColor(1);
//  dtaueta->SetMarkerStyle(22);
//  dtaueta->SetMarkerColor(1);
//  dtaueta->SetMarkerSize(0.7);
//  dtaueta->SetLineWidth(3);
// dtaueta->Scale(1/dtaueta->Integral());
//  dtaueta->Draw("same");
//  //Canv->SaveAs("TauEta.gif");



// TFile *MC = TFile::Open("MC.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
// MC -> cd ("analyzeBasicPat");
//  TH1F *mjetpt =   (TH1F*)gDirectory->Get("jetPt");
//  //new TH1F("jetPt","jetPt",50,0.,30.);
//  mjetpt->SetLineColor(2);
//  mjetpt->SetMarkerStyle(21);
//  mjetpt->SetMarkerColor(2);
//  mjetpt->SetMarkerSize(0.7);
//  mjetpt->SetLineWidth(3);
// mjetpt->Scale(1/mjetpt->Integral());
//  mjetpt->Draw();

// TFile *DT = TFile::Open("Data.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
// DT -> cd ("analyzeBasicPat");
//  TH1F *djetpt  = (TH1F*)gDirectory->Get("jetPt");
//  //new TH1F("jetPt","jetPt",50,0.,30.);
//  djetpt->SetLineColor(1);
//  djetpt->SetMarkerStyle(22);
//  djetpt->SetMarkerColor(1);
//  djetpt->SetMarkerSize(0.7);
//  djetpt->SetLineWidth(3);
// djetpt->Scale(1/djetpt->Integral());
//  djetpt->Draw("same");
//  //Canv->SaveAs("JetPt.gif");


// TFile *MC = TFile::Open("MC.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
// MC -> cd ("analyzeBasicPat");
//  TH1F *mmupt   = (TH1F*)gDirectory->Get("muonPt");
//  //new TH1F("muonPt","muonPt",50,0.,30.);
//  mmupt->SetLineColor(2);
//  mmupt->SetMarkerStyle(21);
//  mmupt->SetMarkerColor(2);
//  mmupt->SetMarkerSize(0.7);
//  mmupt->SetLineWidth(3);
// mmupt->Scale(1/mmupt->Integral());
//  mmupt->Draw();

// TFile *DT = TFile::Open("Data.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
// DT -> cd ("analyzeBasicPat");
//  TH1F *dmupt   = (TH1F*)gDirectory->Get("muonPt");
//  //new TH1F("muonPt","muonPt",50,0.,30.);
// dmupt->SetLineColor(1);
//  dmupt->SetMarkerStyle(22);
//  dmupt->SetMarkerColor(1);
//  dmupt->SetMarkerSize(0.7);
//  dmupt->SetLineWidth(3);
// dmupt->Scale(1/dmupt->Integral());
//  dmupt->Draw("same");
//  //Canv->SaveAs("MuonPt.gif");




// TFile *DT = TFile::Open("Data.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
// DT -> cd ("analyzeBasicPat");
//  TH1F *dtaupt   = (TH1F*)gDirectory->Get("tauPt");

// dtaupt->SetLineColor(1);
// dtaupt->SetMarkerStyle(22);
// dtaupt->SetMarkerColor(1);
// dtaupt->SetMarkerSize(0.7);
// dtaupt->SetLineWidth(3);
// dtaupt->Scale(1/dtaupt->Integral());
// dtaupt->Draw();

// TFile *MC = TFile::Open("MC.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
// MC -> cd ("analyzeBasicPat");
//  TH1F *mtaupt  = (TH1F*)gDirectory->Get("tauPt");
//  //new TH1F("tauPt","tauPt",50,0.,30.);
//  mtaupt->SetLineColor(2);
//  mtaupt->SetMarkerStyle(21);
//  mtaupt->SetMarkerColor(2);
//  mtaupt->SetMarkerSize(0.7);
//  mtaupt->SetLineWidth(3);
//  mtaupt->Scale(1/mtaupt->Integral());
//  mtaupt->Draw("same");
// //Canv->SaveAs("TauPt.gif");



// TFile *MC = TFile::Open("MC.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
// MC -> cd ("analyzeBasicPat");
//  TH1F *mjetphi   = (TH1F*)gDirectory->Get("jetPhi");
//  //Phi",100,-5.,5.);
// mjetphi->SetLineColor(2);
// mjetphi->SetMarkerStyle(21);
// mjetphi->SetMarkerColor(2);
// mjetphi->SetMarkerSize(0.7);
// mjetphi->SetLineWidth(3);
// mjetphi->Scale(1/mjetphi->Integral());
// mjetphi->Draw();

// TFile *DT = TFile::Open("Data.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
// DT -> cd ("analyzeBasicPat");
// TH1F *djetphi   = (TH1F*)gDirectory->Get("jetPhi");
// djetphi->SetLineColor(1);
// djetphi->SetMarkerStyle(22);
// djetphi->SetMarkerColor(1);
// djetphi->SetMarkerSize(0.7);
// djetphi->SetLineWidth(3);
// djetphi->Scale(1/djetphi->Integral());
// djetphi->Draw("same");
// //Canv->SaveAs("JetPhi.gif");


// TFile *MC = TFile::Open("MC.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
// MC -> cd ("analyzeBasicPat");
// TH1F *mmuphi   = (TH1F*)gDirectory->Get("muonPhi");
// mmuphi->SetLineColor(2);
// mmuphi->SetMarkerStyle(21);
// mmuphi->SetMarkerColor(2);
// mmuphi->SetMarkerSize(0.7);
// mmuphi->SetLineWidth(3);
// mmuphi->Scale(1/mmuphi->Integral());
// mmuphi->Draw();

// TFile *DT = TFile::Open("Data.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
// DT -> cd ("analyzeBasicPat");
// TH1F *dmuphi   = (TH1F*)gDirectory->Get("muonPhi");
// dmuphi->SetLineColor(1);
// dmuphi->SetMarkerStyle(22);
// dmuphi->SetMarkerColor(1);
// dmuphi->SetMarkerSize(0.7);
// dmuphi->SetLineWidth(3);
// dmuphi->Scale(1/dmuphi->Integral());
// dmuphi->Draw("same");
// //Canv->SaveAs("MuonPhi.gif");


// TFile *MC = TFile::Open("MC.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
// MC -> cd ("analyzeBasicPat");
// TH1F *mtauphi  = (TH1F*)gDirectory->Get("tauPhi");
// mtauphi->SetLineColor(2);
// mtauphi->SetMarkerStyle(21);
// mtauphi->SetMarkerColor(2);
// mtauphi->SetMarkerSize(0.7);
// mtauphi->SetLineWidth(3);
// mtauphi->Scale(1/mtauphi->Integral());
// mtauphi->Draw();

// TFile *DT = TFile::Open("Data.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
// DT -> cd ("analyzeBasicPat");
// TH1F *dtauphi   = (TH1F*)gDirectory->Get("tauPhi");
// dtauphi->SetLineColor(1);
// dtauphi->SetMarkerStyle(22);
// dtauphi->SetMarkerColor(1);
// dtauphi->SetMarkerSize(0.7);
// dtauphi->SetLineWidth(3);
// dtauphi->Scale(1/dtauphi->Integral());
// dtauphi->Draw("same");
// //Canv->SaveAs("TauPhi.gif");


// TFile *MC = TFile::Open("MC.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
// MC -> cd ("analyzeBasicPat");
// TH1F *mmucc  = (TH1F*)gDirectory->Get("muonCaloCompatibility");
// mmucc->SetLineColor(2);
// mmucc->SetMarkerStyle(21);
// mmucc->SetMarkerColor(2);
// mmucc->SetMarkerSize(0.7);
// mmucc->SetLineWidth(3);
// mmucc->Scale(1/mmucc->Integral());
// mmucc->Draw();

// TFile *DT = TFile::Open("Data.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
// DT -> cd ("analyzeBasicPat");
// TH1F *dmucc   = (TH1F*)gDirectory->Get("muonCaloCompatibility");
// dmucc->SetLineColor(1);
// dmucc->SetMarkerStyle(22);
// dmucc->SetMarkerColor(1);
// dmucc->SetMarkerSize(0.7);
// dmucc->SetLineWidth(3);
// dmucc->Scale(1/dmucc->Integral());
// dmucc->Draw("same");
// //Canv->SaveAs("MuonCC.gif");


// TFile *DT = TFile::Open("Data.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
// DT -> cd ("analyzeBasicPat");
//  TH1F *dmusc  = (TH1F*)gDirectory->Get("muonSegCompatibility");
// dmusc->SetLineColor(1);
// dmusc->SetMarkerStyle(22);
// dmusc->SetMarkerColor(1);
// dmusc->SetMarkerSize(0.7);
// dmusc->SetLineWidth(3);
// dmusc->Scale(1/dmusc->Integral());
// dmusc->Draw();


// TFile *MC = TFile::Open("MC.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
// MC -> cd ("analyzeBasicPat");
// TH1F *mmusc   = (TH1F*)gDirectory->Get("muonSegCompatibility");
// mmusc->SetLineColor(2);
// mmusc->SetMarkerStyle(21);
// mmusc->SetMarkerColor(2);
// mmusc->SetMarkerSize(0.7);
// mmusc->SetLineWidth(3);
// mmusc->Scale(1/mmusc->Integral());
// mmusc->Draw("same");
// //Canv->SaveAs("MuonSCB.gif");



// // TFile *DT = TFile::Open("Data.root");
// // TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
// // DT -> cd ("analyzeBasicPat");
// // TH1F *dmunc  = (TH1F*)gDirectory->Get("muonNChambers_");
// // dmunc->SetLineColor(1);
// // dmunc->SetMarkerStyle(22);
// // dmunc->SetMarkerColor(1);
// // dmunc->SetMarkerSize(0.7);
// // dmunc->SetLineWidth(3);
// // dmunc->Scale(1/dmunc->Integral());
// // dmunc->Draw();
// // TFile *MC = TFile::Open("MC.root");
// // TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
// // MC -> cd ("analyzeBasicPat");
// // TH1F *mmunc  = (TH1F*)gDirectory->Get("muonNChambers_");
// // mmunc->SetLineColor(2);
// // mmunc->SetMarkerStyle(21);
// // mmunc->SetMarkerColor(2);
// // mmunc->SetMarkerSize(0.7);
// // mmunc->SetLineWidth(3);
// // mmunc->Scale(1/mmunc->Integral());
// // mmunc->Draw("same");


// //Canv->SaveAs("MuonNC.gif");

// TFile *MC = TFile::Open("MC.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
// MC -> cd ("analyzeBasicPat");
//  TH1F *mmun  = (TH1F*)gDirectory->Get("muonN");
// mmun->SetLineColor(2);
// mmun->SetMarkerStyle(21);
// mmun->SetMarkerColor(2);
// mmun->SetMarkerSize(0.7);
// mmun->SetLineWidth(3);
// mmun->Scale(1/mmun->Integral());
// mmun->Draw();

// TFile *DT = TFile::Open("Data.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
// DT -> cd ("analyzeBasicPat");
// TH1F *dmun = (TH1F*)gDirectory->Get("muonN");
// dmun->SetLineColor(1);
// dmun->SetMarkerStyle(22);
// dmun->SetMarkerColor(1);
// dmun->SetMarkerSize(0.7);
// dmun->SetLineWidth(3);
// dmun->Scale(1/dmun->Integral());
// dmun->Draw("same");
// //Canv->SaveAs("MuonN.gif");


// TFile *DT = TFile::Open("Data.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
// DT -> cd ("analyzeBasicPat");
//  TH1F *dmuem = (TH1F*)gDirectory->Get("muonIso05emEt");
// dmuem->SetLineColor(1);
// dmuem->SetMarkerStyle(22);
// dmuem->SetMarkerColor(1);
// dmuem->SetMarkerSize(0.7);
// dmuem->SetLineWidth(3);
// dmuem->Scale(1/dmuem->Integral());
// dmuem->Draw();
// TFile *MC = TFile::Open("MC.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
// MC -> cd ("analyzeBasicPat");
//  TH1F *mmuem = (TH1F*)gDirectory->Get("muonIso05emEt");
// mmuem->SetLineColor(2);
// mmuem->SetMarkerStyle(21);
// mmuem->SetMarkerColor(2);
// mmuem->SetMarkerSize(0.7);
// mmuem->SetLineWidth(3);
// mmuem->Scale(1/mmuem->Integral());
// mmuem->Draw("same");


// //Canv->SaveAs("MuonEMIso.gif");

// TFile *DT = TFile::Open("Data.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
// DT -> cd ("analyzeBasicPat");
// TH1F *dmuha = (TH1F*)gDirectory->Get("muonIso05hadEt");
// dmuha->SetLineColor(1);
// dmuha->SetMarkerStyle(22);
// dmuha->SetMarkerColor(1);
// dmuha->SetMarkerSize(0.7);
// dmuha->SetLineWidth(3);
// dmuha->Scale(1/dmuha->Integral());
// dmuha->Draw();

// TFile *MC = TFile::Open("MC.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
// MC -> cd ("analyzeBasicPat");
// TH1F *mmuha = (TH1F*)gDirectory->Get("muonIso05hadEt");
// mmuha->SetLineColor(2);
// mmuha->SetMarkerStyle(21);
// mmuha->SetMarkerColor(2);
// mmuha->SetMarkerSize(0.7);
// mmuha->SetLineWidth(3);
// mmuha->Scale(1/mmuha->Integral());
// mmuha->Draw("same");


// //Canv->SaveAs("MuonHadIso.gif");



// TFile *DT = TFile::Open("Data.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
// DT -> cd ("analyzeBasicPat");
// TH1F *dmutk = (TH1F*)gDirectory->Get("muonIso05sumPt");
// dmutk->SetLineColor(1);
// dmutk->SetMarkerStyle(22);
// dmutk->SetMarkerColor(1);
// dmutk->SetMarkerSize(0.7);
// dmutk->SetLineWidth(3);
// dmutk->Scale(1/dmutk->Integral());
// dmutk->Draw();
// TFile *MC = TFile::Open("MC.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
// MC -> cd ("analyzeBasicPat");
// TH1F *mmutk = (TH1F*)gDirectory->Get("muonIso05sumPt");
// mmutk->SetLineColor(2);
// mmutk->SetMarkerStyle(21);
// mmutk->SetMarkerColor(2);
// mmutk->SetMarkerSize(0.7);
// mmutk->SetLineWidth(3);
// mmutk->Scale(1/mmutk->Integral());
// mmutk->Draw("same");
// //Canv->SaveAs("MuonTrkIso.gif");



/////// ******* VERTEX

TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dvtxN = (TH1F*)gDirectory->Get("vtxN");
dvtxN->SetLineColor(1);
dvtxN->SetMarkerStyle(21);
dvtxN->SetMarkerColor(1);
dvtxN->SetMarkerSize(0.7);
//dvtxN->SetLineWidth(3);
//dvtxN->Scale(1/dvtxN->Integral());

TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mvtxN = (TH1F*)gDirectory->Get("vtxN");
mvtxN->SetLineColor(2);
//mvtxN->SetFillColor(2);
//mvtxN->SetMarkerStyle(21);
//mvtxN->SetMarkerColor(2);
//mvtxN->SetMarkerSize(0.7);
mvtxN->SetLineWidth(3);
mvtxN->Scale(dvtxN->GetEntries()/mvtxN->GetEntries());
//leg = new TLegend (0.4, 0.6, 0.89, 0.89);
//leg -> AddEntry(dvtxN,"data",    
mvtxN->Draw();
dvtxN->Draw("samesPE1");
 Canv->Update();
TPaveStats *sm = (TPaveStats*)mvtxN->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dvtxN->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/vtxN.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dvtxZ = (TH1F*)gDirectory->Get("vtxZ");
//dvtxZ->SetLineColor(1);
dvtxZ->SetMarkerStyle(21);
dvtxZ->SetMarkerColor(1);
dvtxZ->SetMarkerSize(0.7);
//dvtxZ->SetLineWidth(3);
//dvtxZ->Scale(1/dvtxZ->Integral());

TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mvtxZ = (TH1F*)gDirectory->Get("vtxZ");
mvtxZ->SetLineColor(2);
TPaveStats *s = (TPaveStats*)Canv->GetPrimitive("stats");
mvtxZ->SetLineColor(2);
//mvtxZ->SetFillColor(2);
//mvtxZ->SetMarkerStyle(21);
//mvtxZ->SetMarkerColor(2);
//mvtxZ->SetMarkerSize(0.7);
mvtxZ->SetLineWidth(3);
mvtxZ->Scale(dvtxZ->GetEntries()/mvtxZ->GetEntries());
dvtxZ->Draw("PE1");
mvtxZ->Draw("sames");


Canv->Update();
TPaveStats *sm = (TPaveStats*)mvtxZ->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dvtxZ->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 


Canv->SaveAs("./DataMC/vtxZ.gif");






TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dvtxNDOF = (TH1F*)gDirectory->Get("vtxNDOF");
//dvtxZ->SetLineColor(1);
dvtxNDOF->SetMarkerStyle(21);
dvtxNDOF->SetMarkerColor(1);
dvtxNDOF->SetMarkerSize(0.7);
//dvtxZ->SetLineWidth(3);
//dvtxZ->Scale(1/dvtxZ->Integral());

TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mvtxNDOF = (TH1F*)gDirectory->Get("vtxNDOF");
mvtxNDOF->SetLineColor(2);
//mvtxNDOF->SetFillColor(2);
//mvtxZ->SetMarkerStyle(21);
//mvtxZ->SetMarkerColor(2);
//mvtxZ->SetMarkerSize(0.7);
mvtxNDOF->SetLineWidth(3);
mvtxNDOF->Scale(dvtxNDOF->GetEntries()/mvtxNDOF->GetEntries());
mvtxNDOF->Draw();
dvtxNDOF->Draw("PE1sames");

Canv->Update();
TPaveStats *sm = (TPaveStats*)mvtxNDOF->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dvtxNDOF->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 

Canv->SaveAs("./DataMC/vtxNDOF.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dvtxNTrk = (TH1F*)gDirectory->Get("vtxNTrk");
//dvtxZ->SetLineColor(1);
dvtxNTrk->SetMarkerStyle(21);
dvtxNTrk->SetMarkerColor(1);
dvtxNTrk->SetMarkerSize(0.7);
//dvtxZ->SetLineWidth(3);
//dvtxZ->Scale(1/dvtxZ->Integral());

TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mvtxNTrk = (TH1F*)gDirectory->Get("vtxNTrk");
mvtxNTrk->SetLineColor(2);
//mvtxNTrk->SetFillColor(2);
//mvtxZ->SetMarkerStyle(21);
//mvtxZ->SetMarkerColor(2);
//mvtxZ->SetMarkerSize(0.7);
mvtxNTrk->SetLineWidth(3);
mvtxNTrk->Scale(dvtxNTrk->GetEntries()/mvtxNTrk->GetEntries());
mvtxNTrk->Draw();
dvtxNTrk->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mvtxNTrk->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dvtxNTrk->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/vtxNTrk.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dvtxPChi2 = (TH1F*)gDirectory->Get("vtxPChi2");
//dvtxZ->SetLineColor(1);
dvtxPChi2->SetMarkerStyle(21);
dvtxPChi2->SetMarkerColor(1);
dvtxPChi2->SetMarkerSize(0.7);
//dvtxZ->SetLineWidth(3);
//dvtxZ->Scale(1/dvtxZ->Integral());

TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mvtxPChi2 = (TH1F*)gDirectory->Get("vtxPChi2");
mvtxPChi2->SetLineColor(2);
//mvtxPChi2->SetFillColor(2);
//mvtxZ->SetMarkerStyle(21);
//mvtxZ->SetMarkerColor(2);
//mvtxZ->SetMarkerSize(0.7);
mvtxPChi2->SetLineWidth(3);
mvtxPChi2->Scale(dvtxPChi2->GetEntries()/mvtxPChi2->GetEntries());
mvtxPChi2->Draw();
dvtxPChi2->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mvtxPChi2->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dvtxPChi2->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/vtxPChi2.gif");


// *********** MUONS

TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dmuonPt = (TH1F*)gDirectory->Get("muonPt");
dmuonPt->SetMarkerStyle(21);
dmuonPt->SetMarkerColor(1);
dmuonPt->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mmuonPt = (TH1F*)gDirectory->Get("muonPt");
mmuonPt->SetLineColor(2);
mmuonPt->Scale(dmuonPt->GetEntries()/mmuonPt->GetEntries());
mmuonPt->Draw();
dmuonPt->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mmuonPt->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dmuonPt->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/muonPt.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dmuonEta = (TH1F*)gDirectory->Get("muonEta");
dmuonEta->SetMarkerStyle(21);
dmuonEta->SetMarkerColor(1);
dmuonEta->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mmuonEta = (TH1F*)gDirectory->Get("muonEta");
mmuonEta->SetLineColor(2);
mmuonEta->Scale(dmuonEta->GetEntries()/mmuonEta->GetEntries());
dmuonEta->Draw("PE1");
mmuonEta->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mmuonEta->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dmuonEta->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/muonEta.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dmuonPhi = (TH1F*)gDirectory->Get("muonPhi");
dmuonPhi->SetMarkerStyle(21);
dmuonPhi->SetMarkerColor(1);
dmuonPhi->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mmuonPhi = (TH1F*)gDirectory->Get("muonPhi");
mmuonPhi->SetLineColor(2);
mmuonPhi->Scale(dmuonPhi->GetEntries()/mmuonPhi->GetEntries());
dmuonPhi->Draw("PE1");
mmuonPhi->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mmuonPhi->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dmuonPhi->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/muonPhi.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dmuonN = (TH1F*)gDirectory->Get("muonN");
dmuonN->SetMarkerStyle(21);
dmuonN->SetMarkerColor(1);
dmuonN->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mmuonN = (TH1F*)gDirectory->Get("muonN");
mmuonN->SetLineColor(2);
mmuonN->Scale(dmuonN->GetEntries()/mmuonN->GetEntries());
dmuonN->Draw("PE1");
mmuonN->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mmuonN->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dmuonN->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/muonN.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dmuonCharge = (TH1F*)gDirectory->Get("muonCharge");
dmuonCharge->SetMarkerStyle(21);
dmuonCharge->SetMarkerColor(1);
dmuonCharge->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mmuonCharge = (TH1F*)gDirectory->Get("muonCharge");
mmuonCharge->SetLineColor(2);
mmuonCharge->Scale(dmuonCharge->GetEntries()/mmuonCharge->GetEntries());
mmuonCharge->Draw();
dmuonCharge->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mmuonCharge->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dmuonCharge->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/muonCharge.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dmuonTracker = (TH1F*)gDirectory->Get("muonTracker");
dmuonTracker->SetMarkerStyle(21);
dmuonTracker->SetMarkerColor(1);
dmuonTracker->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mmuonTracker = (TH1F*)gDirectory->Get("muonTracker");
mmuonTracker->SetLineColor(2);
mmuonTracker->Scale(dmuonTracker->GetEntries()/mmuonTracker->GetEntries());
mmuonTracker->Draw();
dmuonTracker->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mmuonTracker->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dmuonTracker->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/muonTracker.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dmuonGlobal = (TH1F*)gDirectory->Get("muonGlobal");
dmuonGlobal->SetMarkerStyle(21);
dmuonGlobal->SetMarkerColor(1);
dmuonGlobal->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mmuonGlobal = (TH1F*)gDirectory->Get("muonGlobal");
mmuonGlobal->SetLineColor(2);
mmuonGlobal->Scale(dmuonGlobal->GetEntries()/mmuonGlobal->GetEntries());
dmuonGlobal->Draw("PE1");
mmuonGlobal->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mmuonGlobal->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dmuonGlobal->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/muonGlobal.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dmuonEtaTracker = (TH1F*)gDirectory->Get("muonEtaTracker");
dmuonEtaTracker->SetMarkerStyle(21);
dmuonEtaTracker->SetMarkerColor(1);
dmuonEtaTracker->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mmuonEtaTracker = (TH1F*)gDirectory->Get("muonEtaTracker");
mmuonEtaTracker->SetLineColor(2);
mmuonEtaTracker->Scale(dmuonEtaTracker->GetEntries()/mmuonEtaTracker->GetEntries());
dmuonEtaTracker->Draw("PE1");
mmuonEtaTracker->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mmuonEtaTracker->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dmuonEtaTracker->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/muonEtaTracker.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dmuonEtaGlobal = (TH1F*)gDirectory->Get("muonEtaGlobal");
dmuonEtaGlobal->SetMarkerStyle(21);
dmuonEtaGlobal->SetMarkerColor(1);
dmuonEtaGlobal->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mmuonEtaGlobal = (TH1F*)gDirectory->Get("muonEtaGlobal");
mmuonEtaGlobal->SetLineColor(2);
mmuonEtaGlobal->Scale(dmuonEtaGlobal->GetEntries()/mmuonEtaGlobal->GetEntries());
dmuonEtaGlobal->Draw("PE1");
mmuonEtaGlobal->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mmuonEtaGlobal->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dmuonEtaGlobal->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/muonEtaGlobal.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dmuonPhiTracker = (TH1F*)gDirectory->Get("muonPhiTracker");
dmuonPhiTracker->SetMarkerStyle(21);
dmuonPhiTracker->SetMarkerColor(1);
dmuonPhiTracker->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mmuonPhiTracker = (TH1F*)gDirectory->Get("muonPhiTracker");
mmuonPhiTracker->SetLineColor(2);
mmuonPhiTracker->Scale(dmuonPhiTracker->GetEntries()/mmuonPhiTracker->GetEntries());
mmuonPhiTracker->Draw();
dmuonPhiTracker->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mmuonPhiTracker->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dmuonPhiTracker->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/muonPhiTracker.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dmuonPhiGlobal = (TH1F*)gDirectory->Get("muonPhiGlobal");
dmuonPhiGlobal->SetMarkerStyle(21);
dmuonPhiGlobal->SetMarkerColor(1);
dmuonPhiGlobal->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mmuonPhiGlobal = (TH1F*)gDirectory->Get("muonPhiGlobal");
mmuonPhiGlobal->SetLineColor(2);
mmuonPhiGlobal->Scale(dmuonPhiGlobal->GetEntries()/mmuonPhiGlobal->GetEntries());
mmuonPhiGlobal->Draw();
dmuonPhiGlobal->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mmuonPhiGlobal->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dmuonPhiGlobal->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/muonPhiGlobal.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dmuonPtTracker = (TH1F*)gDirectory->Get("muonPtTracker");
dmuonPtTracker->SetMarkerStyle(21);
dmuonPtTracker->SetMarkerColor(1);
dmuonPtTracker->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mmuonPtTracker = (TH1F*)gDirectory->Get("muonPtTracker");
mmuonPtTracker->SetLineColor(2);
mmuonPtTracker->Scale(dmuonPtTracker->GetEntries()/mmuonPtTracker->GetEntries());
mmuonPtTracker->Draw();
dmuonPtTracker->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mmuonPtTracker->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dmuonPtTracker->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/muonPtTracker.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dmuonPtGlobal = (TH1F*)gDirectory->Get("muonPtGlobal");
dmuonPtGlobal->SetMarkerStyle(21);
dmuonPtGlobal->SetMarkerColor(1);
dmuonPtGlobal->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mmuonPtGlobal = (TH1F*)gDirectory->Get("muonPtGlobal");
mmuonPtGlobal->SetLineColor(2);
mmuonPtGlobal->Scale(dmuonPtGlobal->GetEntries()/mmuonPtGlobal->GetEntries());
mmuonPtGlobal->Draw();
dmuonPtGlobal->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mmuonPtGlobal->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dmuonPtGlobal->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/muonPtGlobal.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dmuonCaloCompatibility = (TH1F*)gDirectory->Get("muonCaloCompatibility");
dmuonCaloCompatibility->SetMarkerStyle(21);
dmuonCaloCompatibility->SetMarkerColor(1);
dmuonCaloCompatibility->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mmuonCaloCompatibility = (TH1F*)gDirectory->Get("muonCaloCompatibility");
mmuonCaloCompatibility->SetLineColor(2);
mmuonCaloCompatibility->Scale(dmuonCaloCompatibility->GetEntries()/mmuonCaloCompatibility->GetEntries());
mmuonCaloCompatibility->Draw();
dmuonCaloCompatibility->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mmuonCaloCompatibility->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dmuonCaloCompatibility->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/muonCaloCompatibility.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dmuonSegmentCompatibility = (TH1F*)gDirectory->Get("muonSegCompatibility");
dmuonSegmentCompatibility->SetMarkerStyle(21);
dmuonSegmentCompatibility->SetMarkerColor(1);
dmuonSegmentCompatibility->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mmuonSegmentCompatibility = (TH1F*)gDirectory->Get("muonSegCompatibility");
mmuonSegmentCompatibility->SetLineColor(2);
mmuonSegmentCompatibility->Scale(dmuonSegmentCompatibility->GetEntries()/mmuonSegmentCompatibility->GetEntries());
dmuonSegmentCompatibility->Draw("PE1");
mmuonSegmentCompatibility->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mmuonSegmentCompatibility->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dmuonSegmentCompatibility->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/muonSegmentCompatibility.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dmuonIso05emEt = (TH1F*)gDirectory->Get("muonIso05emEt");
dmuonIso05emEt->SetMarkerStyle(21);
dmuonIso05emEt->SetMarkerColor(1);
dmuonIso05emEt->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mmuonIso05emEt = (TH1F*)gDirectory->Get("muonIso05emEt");
mmuonIso05emEt->SetLineColor(2);
mmuonIso05emEt->Scale(dmuonIso05emEt->GetEntries()/mmuonIso05emEt->GetEntries());
mmuonIso05emEt->Draw();
dmuonIso05emEt->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mmuonIso05emEt->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dmuonIso05emEt->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/muonIso05emEt.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dmuonIso05hadEt = (TH1F*)gDirectory->Get("muonIso05hadEt");
dmuonIso05hadEt->SetMarkerStyle(21);
dmuonIso05hadEt->SetMarkerColor(1);
dmuonIso05hadEt->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mmuonIso05hadEt = (TH1F*)gDirectory->Get("muonIso05hadEt");
mmuonIso05hadEt->SetLineColor(2);
mmuonIso05hadEt->Scale(dmuonIso05hadEt->GetEntries()/mmuonIso05hadEt->GetEntries());
dmuonIso05hadEt->Draw("PE1");
mmuonIso05hadEt->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mmuonIso05hadEt->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dmuonIso05hadEt->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/muonIso05hadEt.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dmuonIso05sumPt = (TH1F*)gDirectory->Get("muonIso05sumPt");
dmuonIso05sumPt->SetMarkerStyle(21);
dmuonIso05sumPt->SetMarkerColor(1);
dmuonIso05sumPt->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mmuonIso05sumPt = (TH1F*)gDirectory->Get("muonIso05sumPt");
mmuonIso05sumPt->SetLineColor(2);
mmuonIso05sumPt->Scale(dmuonIso05sumPt->GetEntries()/mmuonIso05sumPt->GetEntries());
dmuonIso05sumPt->Draw("PE1");
mmuonIso05sumPt->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mmuonIso05sumPt->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dmuonIso05sumPt->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/muonIso05sumPt.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dmuonTrackIPxy = (TH1F*)gDirectory->Get("muonTrackIPxy");
dmuonTrackIPxy->SetMarkerStyle(21);
dmuonTrackIPxy->SetMarkerColor(1);
dmuonTrackIPxy->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mmuonTrackIPxy = (TH1F*)gDirectory->Get("muonTrackIPxy");
mmuonTrackIPxy->SetLineColor(2);
mmuonTrackIPxy->Scale(dmuonTrackIPxy->GetEntries()/mmuonTrackIPxy->GetEntries());
mmuonTrackIPxy->Draw();
dmuonTrackIPxy->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mmuonTrackIPxy->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dmuonTrackIPxy->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/muonTrackIPxy.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *dmuonNChambers = (TH1F*)gDirectory->Get("muonNChambers");
dmuonNChambers->SetMarkerStyle(21);
dmuonNChambers->SetMarkerColor(1);
dmuonNChambers->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mmuonNChambers = (TH1F*)gDirectory->Get("muonNChambers");
mmuonNChambers->SetLineColor(2);
mmuonNChambers->Scale(dmuonNChambers->GetEntries()/mmuonNChambers->GetEntries());
mmuonNChambers->Draw();
dmuonNChambers->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mmuonNChambers->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dmuonNChambers->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/muonNChambers.gif");





///  ***** JET


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *djetN = (TH1F*)gDirectory->Get("jetN");
djetN->SetMarkerStyle(21);
djetN->SetMarkerColor(1);
djetN->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mjetN = (TH1F*)gDirectory->Get("jetN");
mjetN->SetLineColor(2);
mjetN->Scale(djetN->GetEntries()/mjetN->GetEntries());
djetN->Draw("PE1");
mjetN->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mjetN->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)djetN->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/jetN.gif");

TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *djetNTrk = (TH1F*)gDirectory->Get("jetNTrk");
djetNTrk->SetMarkerStyle(21);
djetNTrk->SetMarkerColor(1);
djetNTrk->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mjetNTrk = (TH1F*)gDirectory->Get("jetNTrk");
mjetNTrk->SetLineColor(2);
mjetNTrk->Scale(djetNTrk->GetEntries()/mjetNTrk->GetEntries());
mjetNTrk->Draw();
djetNTrk->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mjetNTrk->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)djetNTrk->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/jetNTrk.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *djetPt = (TH1F*)gDirectory->Get("jetPt");
djetPt->SetMarkerStyle(21);
djetPt->SetMarkerColor(1);
djetPt->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mjetPt = (TH1F*)gDirectory->Get("jetPt");
mjetPt->SetLineColor(2);
mjetPt->Scale(djetPt->GetEntries()/mjetPt->GetEntries());
djetPt->Draw("PE1");
mjetPt->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mjetPt->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)djetPt->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/jetPt.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *djetEta = (TH1F*)gDirectory->Get("jetEta");
djetEta->SetMarkerStyle(21);
djetEta->SetMarkerColor(1);
djetEta->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mjetEta = (TH1F*)gDirectory->Get("jetEta");
mjetEta->SetLineColor(2);
mjetEta->Scale(djetEta->GetEntries()/mjetEta->GetEntries());
mjetEta->Draw();
djetEta->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mjetEta->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)djetEta->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/jetEta.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *djetPhi = (TH1F*)gDirectory->Get("jetPhi");
djetPhi->SetMarkerStyle(21);
djetPhi->SetMarkerColor(1);
djetPhi->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mjetPhi = (TH1F*)gDirectory->Get("jetPhi");
mjetPhi->SetLineColor(2);
mjetPhi->Scale(djetPhi->GetEntries()/mjetPhi->GetEntries());
djetPhi->Draw("PE1");
mjetPhi->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mjetPhi->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)djetPhi->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/jetPhi.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *djetEta010 = (TH1F*)gDirectory->Get("jetEta010");
djetEta010->SetMarkerStyle(21);
djetEta010->SetMarkerColor(1);
djetEta010->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mjetEta010 = (TH1F*)gDirectory->Get("jetEta010");
mjetEta010->SetLineColor(2);
mjetEta010->Scale(djetEta010->GetEntries()/mjetEta010->GetEntries());
mjetEta010->Draw();
djetEta010->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mjetEta010->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)djetEta010->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/jetEta010.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *djetEta1015 = (TH1F*)gDirectory->Get("jetEta1015");
djetEta1015->SetMarkerStyle(21);
djetEta1015->SetMarkerColor(1);
djetEta1015->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mjetEta1015 = (TH1F*)gDirectory->Get("jetEta1015");
mjetEta1015->SetLineColor(2);
mjetEta1015->Scale(djetEta1015->GetEntries()/mjetEta1015->GetEntries());
djetEta1015->Draw("PE1");
mjetEta1015->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mjetEta1015->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)djetEta1015->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/jetEta1015.gif");

TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *djetEta1520 = (TH1F*)gDirectory->Get("jetEta1520");
djetEta1520->SetMarkerStyle(21);
djetEta1520->SetMarkerColor(1);
djetEta1520->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mjetEta1520 = (TH1F*)gDirectory->Get("jetEta1520");
mjetEta1520->SetLineColor(2);
mjetEta1520->Scale(djetEta1520->GetEntries()/mjetEta1520->GetEntries());
djetEta1520->Draw("PE1");
mjetEta1520->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mjetEta1520->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)djetEta1520->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/jetEta1520.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *djetEta2025 = (TH1F*)gDirectory->Get("jetEta2025");
djetEta2025->SetMarkerStyle(21);
djetEta2025->SetMarkerColor(1);
djetEta2025->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mjetEta2025 = (TH1F*)gDirectory->Get("jetEta2025");
mjetEta2025->SetLineColor(2);
mjetEta2025->Scale(djetEta2025->GetEntries()/mjetEta2025->GetEntries());
djetEta2025->Draw("PE1");
mjetEta2025->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mjetEta2025->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)djetEta2025->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/jetEta2025.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *djetEta2530 = (TH1F*)gDirectory->Get("jetEta2530");
djetEta2530->SetMarkerStyle(21);
djetEta2530->SetMarkerColor(1);
djetEta2530->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mjetEta2530 = (TH1F*)gDirectory->Get("jetEta2530");
mjetEta2530->SetLineColor(2);
mjetEta2530->Scale(djetEta2530->GetEntries()/mjetEta2530->GetEntries());
mjetEta2530->Draw();
djetEta2530->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mjetEta2530->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)djetEta2530->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/jetEta2530.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *djetEta30ON = (TH1F*)gDirectory->Get("jetEta30ON");
djetEta30ON->SetMarkerStyle(21);
djetEta30ON->SetMarkerColor(1);
djetEta30ON->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mjetEta30ON = (TH1F*)gDirectory->Get("jetEta30ON");
mjetEta30ON->SetLineColor(2);
mjetEta30ON->Scale(djetEta30ON->GetEntries()/mjetEta30ON->GetEntries());
mjetEta30ON->Draw();
djetEta30ON->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mjetEta30ON->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)djetEta30ON->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/jetEta30ON.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *djetNTrk010 = (TH1F*)gDirectory->Get("jetNTrk010");
djetNTrk010->SetMarkerStyle(21);
djetNTrk010->SetMarkerColor(1);
djetNTrk010->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mjetNTrk010 = (TH1F*)gDirectory->Get("jetNTrk010");
mjetNTrk010->SetLineColor(2);
mjetNTrk010->Scale(djetNTrk010->GetEntries()/mjetNTrk010->GetEntries());
mjetNTrk010->Draw();
djetNTrk010->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mjetNTrk010->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)djetNTrk010->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/jetNTrk010.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *djetNTrk1015 = (TH1F*)gDirectory->Get("jetNTrk1015");
djetNTrk1015->SetMarkerStyle(21);
djetNTrk1015->SetMarkerColor(1);
djetNTrk1015->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mjetNTrk1015 = (TH1F*)gDirectory->Get("jetNTrk1015");
mjetNTrk1015->SetLineColor(2);
mjetNTrk1015->Scale(djetNTrk1015->GetEntries()/mjetNTrk1015->GetEntries());
mjetNTrk1015->Draw();
djetNTrk1015->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mjetNTrk1015->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)djetNTrk1015->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/jetNTrk1015.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *djetNTrk1520 = (TH1F*)gDirectory->Get("jetNTrk1520");
djetNTrk1520->SetMarkerStyle(21);
djetNTrk1520->SetMarkerColor(1);
djetNTrk1520->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mjetNTrk1520 = (TH1F*)gDirectory->Get("jetNTrk1520");
mjetNTrk1520->SetLineColor(2);
mjetNTrk1520->Scale(djetNTrk1520->GetEntries()/mjetNTrk1520->GetEntries());
djetNTrk1520->Draw("PE1");
mjetNTrk1520->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mjetNTrk1520->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)djetNTrk1520->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/jetNTrk1520.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *djetNTrk2025 = (TH1F*)gDirectory->Get("jetNTrk2025");
djetNTrk2025->SetMarkerStyle(21);
djetNTrk2025->SetMarkerColor(1);
djetNTrk2025->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mjetNTrk2025 = (TH1F*)gDirectory->Get("jetNTrk2025");
mjetNTrk2025->SetLineColor(2);
mjetNTrk2025->Scale(djetNTrk2025->GetEntries()/mjetNTrk2025->GetEntries());
djetNTrk2025->Draw("PE1");
mjetNTrk2025->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mjetNTrk2025->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)djetNTrk2025->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/jetNTrk2025.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *djetNTrk2530 = (TH1F*)gDirectory->Get("jetNTrk2530");
djetNTrk2530->SetMarkerStyle(21);
djetNTrk2530->SetMarkerColor(1);
djetNTrk2530->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mjetNTrk2530 = (TH1F*)gDirectory->Get("jetNTrk2530");
mjetNTrk2530->SetLineColor(2);
mjetNTrk2530->Scale(djetNTrk2530->GetEntries()/mjetNTrk2530->GetEntries());
djetNTrk2530->Draw("PE1");
mjetNTrk2530->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mjetNTrk2530->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)djetNTrk2530->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/jetNTrk2530.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *djetNTrk30ON = (TH1F*)gDirectory->Get("jetNTrk30ON");
djetNTrk30ON->SetMarkerStyle(21);
djetNTrk30ON->SetMarkerColor(1);
djetNTrk30ON->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mjetNTrk30ON = (TH1F*)gDirectory->Get("jetNTrk30ON");
mjetNTrk30ON->SetLineColor(2);
mjetNTrk30ON->Scale(djetNTrk30ON->GetEntries()/mjetNTrk30ON->GetEntries());
djetNTrk30ON->Draw("PE1");
mjetNTrk30ON->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mjetNTrk30ON->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)djetNTrk30ON->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/jetNTrk30ON.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *djetPhi010 = (TH1F*)gDirectory->Get("jetPhi010");
djetPhi010->SetMarkerStyle(21);
djetPhi010->SetMarkerColor(1);
djetPhi010->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mjetPhi010 = (TH1F*)gDirectory->Get("jetPhi010");
mjetPhi010->SetLineColor(2);
mjetPhi010->Scale(djetPhi010->GetEntries()/mjetPhi010->GetEntries());
mjetPhi010->Draw();
djetPhi010->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mjetPhi010->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)djetPhi010->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/jetPhi010.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *djetPhi1015 = (TH1F*)gDirectory->Get("jetPhi1015");
djetPhi1015->SetMarkerStyle(21);
djetPhi1015->SetMarkerColor(1);
djetPhi1015->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mjetPhi1015 = (TH1F*)gDirectory->Get("jetPhi1015");
mjetPhi1015->SetLineColor(2);
mjetPhi1015->Scale(djetPhi1015->GetEntries()/mjetPhi1015->GetEntries());
djetPhi1015->Draw("PE1");
mjetPhi1015->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mjetPhi1015->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)djetPhi1015->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/jetPhi1015.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *djetPhi1520 = (TH1F*)gDirectory->Get("jetPhi1520");
djetPhi1520->SetMarkerStyle(21);
djetPhi1520->SetMarkerColor(1);
djetPhi1520->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mjetPhi1520 = (TH1F*)gDirectory->Get("jetPhi1520");
mjetPhi1520->SetLineColor(2);
mjetPhi1520->Scale(djetPhi1520->GetEntries()/mjetPhi1520->GetEntries());
djetPhi1520->Draw("PE1");
mjetPhi1520->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mjetPhi1520->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)djetPhi1520->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/jetPhi1520.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *djetPhi2025 = (TH1F*)gDirectory->Get("jetPhi2025");
djetPhi2025->SetMarkerStyle(21);
djetPhi2025->SetMarkerColor(1);
djetPhi2025->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mjetPhi2025 = (TH1F*)gDirectory->Get("jetPhi2025");
mjetPhi2025->SetLineColor(2);
mjetPhi2025->Scale(djetPhi2025->GetEntries()/mjetPhi2025->GetEntries());
djetPhi2025->Draw("PE1");
mjetPhi2025->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mjetPhi2025->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)djetPhi2025->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/jetPhi2025.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *djetPhi2530 = (TH1F*)gDirectory->Get("jetPhi2530");
djetPhi2530->SetMarkerStyle(21);
djetPhi2530->SetMarkerColor(1);
djetPhi2530->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mjetPhi2530 = (TH1F*)gDirectory->Get("jetPhi2530");
mjetPhi2530->SetLineColor(2);
mjetPhi2530->Scale(djetPhi2530->GetEntries()/mjetPhi2530->GetEntries());
djetPhi2530->Draw("PE1");
mjetPhi2530->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mjetPhi2530->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)djetPhi2530->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/jetPhi2530.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *djetPhi30ON = (TH1F*)gDirectory->Get("jetPhi30ON");
djetPhi30ON->SetMarkerStyle(21);
djetPhi30ON->SetMarkerColor(1);
djetPhi30ON->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mjetPhi30ON = (TH1F*)gDirectory->Get("jetPhi30ON");
mjetPhi30ON->SetLineColor(2);
mjetPhi30ON->Scale(djetPhi30ON->GetEntries()/mjetPhi30ON->GetEntries());
mjetPhi30ON->Draw();
djetPhi30ON->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mjetPhi30ON->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)djetPhi30ON->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/jetPhi30ON.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH2F *djetEtaVsPt = (TH2F*)gDirectory->Get("jetEtaVsPt");
djetEtaVsPt->SetMarkerStyle(21);
djetEtaVsPt->SetMarkerColor(4);
djetEtaVsPt->SetMarkerSize(0.5);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH2F *mjetEtaVsPt = (TH2F*)gDirectory->Get("jetEtaVsPt");
mjetEtaVsPt->SetMarkerStyle(22);
mjetEtaVsPt->SetMarkerColor(2);
mjetEtaVsPt->SetMarkerSize(0.5);
//mjetEtaVsPt->Scale(djetEtaVsPt->GetEntries()/mjetEtaVsPt->GetEntries());
mjetEtaVsPt->Draw("P");
djetEtaVsPt->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mjetEtaVsPt->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)djetEtaVsPt->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/jetEtaVsPt.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH2F *djetPhiVsPt = (TH2F*)gDirectory->Get("jetPhiVsPt");
djetPhiVsPt->SetMarkerStyle(21);
djetPhiVsPt->SetMarkerColor(1);
djetPhiVsPt->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH2F *mjetPhiVsPt = (TH2F*)gDirectory->Get("jetPhiVsPt");
mjetPhiVsPt->SetLineColor(2);
//mjetPhiVsPt->Scale(djetPhiVsPt->GetEntries()/mjetPhiVsPt->GetEntries());
mjetPhiVsPt->Draw();
djetPhiVsPt->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mjetPhiVsPt->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)djetPhiVsPt->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/jetPhiVsPt.gif");



///// **** pfjet




TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *dpfjetN = (TH1F*)gDirectory->Get("pfjetN");
dpfjetN->SetMarkerStyle(21);
dpfjetN->SetMarkerColor(1);
dpfjetN->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mpfjetN = (TH1F*)gDirectory->Get("pfjetN");
mpfjetN->SetLineColor(2);
mpfjetN->Scale(dpfjetN->GetEntries()/mpfjetN->GetEntries());
dpfjetN->Draw("PE1");
mpfjetN->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mpfjetN->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dpfjetN->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/pfjetN.gif");

TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *dpfjetNTrk = (TH1F*)gDirectory->Get("pfjetNTrk");
dpfjetNTrk->SetMarkerStyle(21);
dpfjetNTrk->SetMarkerColor(1);
dpfjetNTrk->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mpfjetNTrk = (TH1F*)gDirectory->Get("pfjetNTrk");
mpfjetNTrk->SetLineColor(2);
mpfjetNTrk->Scale(dpfjetNTrk->GetEntries()/mpfjetNTrk->GetEntries());
mpfjetNTrk->Draw();
dpfjetNTrk->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mpfjetNTrk->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dpfjetNTrk->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/pfjetNTrk.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *dpfjetPt = (TH1F*)gDirectory->Get("pfjetPt");
dpfjetPt->SetMarkerStyle(21);
dpfjetPt->SetMarkerColor(1);
dpfjetPt->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mpfjetPt = (TH1F*)gDirectory->Get("pfjetPt");
mpfjetPt->SetLineColor(2);
mpfjetPt->Scale(dpfjetPt->GetEntries()/mpfjetPt->GetEntries());
dpfjetPt->Draw("PE1");
mpfjetPt->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mpfjetPt->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dpfjetPt->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/pfjetPt.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *dpfjetEta = (TH1F*)gDirectory->Get("pfjetEta");
dpfjetEta->SetMarkerStyle(21);
dpfjetEta->SetMarkerColor(1);
dpfjetEta->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mpfjetEta = (TH1F*)gDirectory->Get("jetEta");
mpfjetEta->SetLineColor(2);
mpfjetEta->Scale(dpfjetEta->GetEntries()/mpfjetEta->GetEntries());
mpfjetEta->Draw();
dpfjetEta->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mpfjetEta->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dpfjetEta->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/pfjetEta.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *dpfjetPhi = (TH1F*)gDirectory->Get("pfjetPhi");
dpfjetPhi->SetMarkerStyle(21);
dpfjetPhi->SetMarkerColor(1);
dpfjetPhi->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mpfjetPhi = (TH1F*)gDirectory->Get("pfjetPhi");
mpfjetPhi->SetLineColor(2);
mpfjetPhi->Scale(dpfjetPhi->GetEntries()/mpfjetPhi->GetEntries());
mpfjetPhi->Draw();
dpfjetPhi->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mpfjetPhi->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dpfjetPhi->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/pfjetPhi.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *dpfjetEta010 = (TH1F*)gDirectory->Get("pfjetEta010");
dpfjetEta010->SetMarkerStyle(21);
dpfjetEta010->SetMarkerColor(1);
dpfjetEta010->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mpfjetEta010 = (TH1F*)gDirectory->Get("pfjetEta010");
mpfjetEta010->SetLineColor(2);
mpfjetEta010->Scale(dpfjetEta010->GetEntries()/mpfjetEta010->GetEntries());
mpfjetEta010->Draw();
dpfjetEta010->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mpfjetEta010->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dpfjetEta010->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/pfjetEta010.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *dpfjetEta1015 = (TH1F*)gDirectory->Get("pfjetEta1015");
dpfjetEta1015->SetMarkerStyle(21);
dpfjetEta1015->SetMarkerColor(1);
dpfjetEta1015->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mpfjetEta1015 = (TH1F*)gDirectory->Get("pfjetEta1015");
mpfjetEta1015->SetLineColor(2);
mpfjetEta1015->Scale(dpfjetEta1015->GetEntries()/mpfjetEta1015->GetEntries());
dpfjetEta1015->Draw("PE1");
mpfjetEta1015->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mpfjetEta1015->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dpfjetEta1015->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/pfjetEta1015.gif");

TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *dpfjetEta1520 = (TH1F*)gDirectory->Get("pfjetEta1520");
dpfjetEta1520->SetMarkerStyle(21);
dpfjetEta1520->SetMarkerColor(1);
dpfjetEta1520->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mpfjetEta1520 = (TH1F*)gDirectory->Get("pfjetEta1520");
mpfjetEta1520->SetLineColor(2);
mpfjetEta1520->Scale(dpfjetEta1520->GetEntries()/mpfjetEta1520->GetEntries());
dpfjetEta1520->Draw("PE1");
mpfjetEta1520->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mpfjetEta1520->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dpfjetEta1520->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/pfjetEta1520.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *dpfjetEta2025 = (TH1F*)gDirectory->Get("pfjetEta2025");
dpfjetEta2025->SetMarkerStyle(21);
dpfjetEta2025->SetMarkerColor(1);
dpfjetEta2025->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mpfjetEta2025 = (TH1F*)gDirectory->Get("pfjetEta2025");
mpfjetEta2025->SetLineColor(2);
mpfjetEta2025->Scale(dpfjetEta2025->GetEntries()/mpfjetEta2025->GetEntries());
mpfjetEta2025->Draw();
dpfjetEta2025->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mpfjetEta2025->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dpfjetEta2025->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/pfjetEta2025.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *dpfjetEta2530 = (TH1F*)gDirectory->Get("pfjetEta2530");
dpfjetEta2530->SetMarkerStyle(21);
dpfjetEta2530->SetMarkerColor(1);
dpfjetEta2530->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mpfjetEta2530 = (TH1F*)gDirectory->Get("pfjetEta2530");
mpfjetEta2530->SetLineColor(2);
mpfjetEta2530->Scale(dpfjetEta2530->GetEntries()/mpfjetEta2530->GetEntries());
mpfjetEta2530->Draw();
dpfjetEta2530->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mpfjetEta2530->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dpfjetEta2530->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/pfjetEta2530.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *dpfjetEta30ON = (TH1F*)gDirectory->Get("pfjetEta30ON");
dpfjetEta30ON->SetMarkerStyle(21);
dpfjetEta30ON->SetMarkerColor(1);
dpfjetEta30ON->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mpfjetEta30ON = (TH1F*)gDirectory->Get("pfjetEta30ON");
mpfjetEta30ON->SetLineColor(2);
mpfjetEta30ON->Scale(dpfjetEta30ON->GetEntries()/mpfjetEta30ON->GetEntries());
mpfjetEta30ON->Draw();
dpfjetEta30ON->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mpfjetEta30ON->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dpfjetEta30ON->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/pfjetEta30ON.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *dpfjetNTrk010 = (TH1F*)gDirectory->Get("pfjetNTrk010");
dpfjetNTrk010->SetMarkerStyle(21);
dpfjetNTrk010->SetMarkerColor(1);
dpfjetNTrk010->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mpfjetNTrk010 = (TH1F*)gDirectory->Get("pfjetNTrk010");
mpfjetNTrk010->SetLineColor(2);
mpfjetNTrk010->Scale(dpfjetNTrk010->GetEntries()/mpfjetNTrk010->GetEntries());
mpfjetNTrk010->Draw();
dpfjetNTrk010->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mpfjetNTrk010->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dpfjetNTrk010->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/jpfetNTrk010.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *dpfjetNTrk1015 = (TH1F*)gDirectory->Get("pfjetNTrk1015");
dpfjetNTrk1015->SetMarkerStyle(21);
dpfjetNTrk1015->SetMarkerColor(1);
dpfjetNTrk1015->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mpfjetNTrk1015 = (TH1F*)gDirectory->Get("pfjetNTrk1015");
mpfjetNTrk1015->SetLineColor(2);
mpfjetNTrk1015->Scale(dpfjetNTrk1015->GetEntries()/mpfjetNTrk1015->GetEntries());
mpfjetNTrk1015->Draw();
dpfjetNTrk1015->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mpfjetNTrk1015->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dpfjetNTrk1015->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/pfjetNTrk1015.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *dpfjetNTrk1520 = (TH1F*)gDirectory->Get("pfjetNTrk1520");
dpfjetNTrk1520->SetMarkerStyle(21);
dpfjetNTrk1520->SetMarkerColor(1);
dpfjetNTrk1520->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mpfjetNTrk1520 = (TH1F*)gDirectory->Get("pfjetNTrk1520");
mpfjetNTrk1520->SetLineColor(2);
mpfjetNTrk1520->Scale(dpfjetNTrk1520->GetEntries()/mpfjetNTrk1520->GetEntries());
dpfjetNTrk1520->Draw("PE1");
mpfjetNTrk1520->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mpfjetNTrk1520->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dpfjetNTrk1520->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/pfjetNTrk1520.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *dpfjetNTrk2025 = (TH1F*)gDirectory->Get("pfjetNTrk2025");
dpfjetNTrk2025->SetMarkerStyle(21);
dpfjetNTrk2025->SetMarkerColor(1);
dpfjetNTrk2025->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mpfjetNTrk2025 = (TH1F*)gDirectory->Get("pfjetNTrk2025");
mpfjetNTrk2025->SetLineColor(2);
mpfjetNTrk2025->Scale(dpfjetNTrk2025->GetEntries()/mpfjetNTrk2025->GetEntries());
dpfjetNTrk2025->Draw("PE1");
mpfjetNTrk2025->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mpfjetNTrk2025->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dpfjetNTrk2025->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/pfjetNTrk2025.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *dpfjetNTrk2530 = (TH1F*)gDirectory->Get("pfjetNTrk2530");
dpfjetNTrk2530->SetMarkerStyle(21);
dpfjetNTrk2530->SetMarkerColor(1);
dpfjetNTrk2530->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mpfjetNTrk2530 = (TH1F*)gDirectory->Get("pfjetNTrk2530");
mpfjetNTrk2530->SetLineColor(2);
mpfjetNTrk2530->Scale(dpfjetNTrk2530->GetEntries()/mpfjetNTrk2530->GetEntries());
dpfjetNTrk2530->Draw("PE1");
mpfjetNTrk2530->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mpfjetNTrk2530->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dpfjetNTrk2530->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/pfjetNTrk2530.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *dpfjetNTrk30ON = (TH1F*)gDirectory->Get("pfjetNTrk30ON");
dpfjetNTrk30ON->SetMarkerStyle(21);
dpfjetNTrk30ON->SetMarkerColor(1);
dpfjetNTrk30ON->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mpfjetNTrk30ON = (TH1F*)gDirectory->Get("pfjetNTrk30ON");
mpfjetNTrk30ON->SetLineColor(2);
mpfjetNTrk30ON->Scale(dpfjetNTrk30ON->GetEntries()/mpfjetNTrk30ON->GetEntries());
dpfjetNTrk30ON->Draw("PE1");
mpfjetNTrk30ON->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mpfjetNTrk30ON->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dpfjetNTrk30ON->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/pfjetNTrk30ON.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *dpfjetPhi010 = (TH1F*)gDirectory->Get("pfjetPhi010");
dpfjetPhi010->SetMarkerStyle(21);
dpfjetPhi010->SetMarkerColor(1);
dpfjetPhi010->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mpfjetPhi010 = (TH1F*)gDirectory->Get("pfjetPhi010");
mpfjetPhi010->SetLineColor(2);
mpfjetPhi010->Scale(dpfjetPhi010->GetEntries()/mpfjetPhi010->GetEntries());
mpfjetPhi010->Draw();
dpfjetPhi010->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mpfjetPhi010->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dpfjetPhi010->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/pfjetPhi010.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *dpfjetPhi1015 = (TH1F*)gDirectory->Get("pfjetPhi1015");
dpfjetPhi1015->SetMarkerStyle(21);
dpfjetPhi1015->SetMarkerColor(1);
dpfjetPhi1015->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mpfjetPhi1015 = (TH1F*)gDirectory->Get("pfjetPhi1015");
mpfjetPhi1015->SetLineColor(2);
mpfjetPhi1015->Scale(dpfjetPhi1015->GetEntries()/mpfjetPhi1015->GetEntries());
dpfjetPhi1015->Draw("PE1");
mpfjetPhi1015->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mpfjetPhi1015->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dpfjetPhi1015->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/pfjetPhi1015.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *dpfjetPhi1520 = (TH1F*)gDirectory->Get("pfjetPhi1520");
dpfjetPhi1520->SetMarkerStyle(21);
dpfjetPhi1520->SetMarkerColor(1);
dpfjetPhi1520->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mpfjetPhi1520 = (TH1F*)gDirectory->Get("pfjetPhi1520");
mpfjetPhi1520->SetLineColor(2);
mpfjetPhi1520->Scale(dpfjetPhi1520->GetEntries()/mpfjetPhi1520->GetEntries());
dpfjetPhi1520->Draw("PE1");
mpfjetPhi1520->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mpfjetPhi1520->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dpfjetPhi1520->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/pfjetPhi1520.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *dpfjetPhi2025 = (TH1F*)gDirectory->Get("pfjetPhi2025");
dpfjetPhi2025->SetMarkerStyle(21);
dpfjetPhi2025->SetMarkerColor(1);
dpfjetPhi2025->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mpfjetPhi2025 = (TH1F*)gDirectory->Get("pfjetPhi2025");
mpfjetPhi2025->SetLineColor(2);
mpfjetPhi2025->Scale(dpfjetPhi2025->GetEntries()/mpfjetPhi2025->GetEntries());
dpfjetPhi2025->Draw("PE1");
mpfjetPhi2025->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mpfjetPhi2025->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dpfjetPhi2025->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/pfjetPhi2025.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *dpfjetPhi2530 = (TH1F*)gDirectory->Get("pfjetPhi2530");
dpfjetPhi2530->SetMarkerStyle(21);
dpfjetPhi2530->SetMarkerColor(1);
dpfjetPhi2530->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mpfjetPhi2530 = (TH1F*)gDirectory->Get("pfjetPhi2530");
mpfjetPhi2530->SetLineColor(2);
mpfjetPhi2530->Scale(dpfjetPhi2530->GetEntries()/mpfjetPhi2530->GetEntries());
dpfjetPhi2530->Draw("PE1");
mpfjetPhi2530->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mpfjetPhi2530->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dpfjetPhi2530->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/pfjetPhi2530.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *dpfjetPhi30ON = (TH1F*)gDirectory->Get("pfjetPhi30ON");
dpfjetPhi30ON->SetMarkerStyle(21);
dpfjetPhi30ON->SetMarkerColor(1);
dpfjetPhi30ON->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mpfjetPhi30ON = (TH1F*)gDirectory->Get("pfjetPhi30ON");
mpfjetPhi30ON->SetLineColor(2);
mpfjetPhi30ON->Scale(dpfjetPhi30ON->GetEntries()/mpfjetPhi30ON->GetEntries());
dpfjetPhi30ON->Draw("PE1");
mpfjetPhi30ON->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mpfjetPhi30ON->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dpfjetPhi30ON->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/pfjetPhi30ON.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH2F *dpfjetEtaVsPt = (TH2F*)gDirectory->Get("pfjetEtaVsPt");
dpfjetEtaVsPt->SetMarkerStyle(21);
dpfjetEtaVsPt->SetMarkerColor(4);
dpfjetEtaVsPt->SetMarkerSize(0.5);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH2F *mpfjetEtaVsPt = (TH2F*)gDirectory->Get("pfjetEtaVsPt");
mpfjetEtaVsPt->SetMarkerStyle(22);
mpfjetEtaVsPt->SetMarkerColor(2);
mpfjetEtaVsPt->SetMarkerSize(0.5);
//mjetEtaVsPt->Scale(djetEtaVsPt->GetEntries()/mjetEtaVsPt->GetEntries());
mpfjetEtaVsPt->Draw("P");
dpfjetEtaVsPt->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mpfjetEtaVsPt->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dpfjetEtaVsPt->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/pfjetEtaVsPt.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH2F *djetPhiVsPt = (TH2F*)gDirectory->Get("jetPhiVsPt");
djetPhiVsPt->SetMarkerStyle(21);
djetPhiVsPt->SetMarkerColor(1);
djetPhiVsPt->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH2F *mjetPhiVsPt = (TH2F*)gDirectory->Get("jetPhiVsPt");
mjetPhiVsPt->SetLineColor(2);
//mjetPhiVsPt->Scale(djetPhiVsPt->GetEntries()/mjetPhiVsPt->GetEntries());
mjetPhiVsPt->Draw();
djetPhiVsPt->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mjetPhiVsPt->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)djetPhiVsPt->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
//Canv->SaveAs("./DataMC/jetPhiVsPt.gif");




/// *** MET


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *dmetEt = (TH1F*)gDirectory->Get("metEt");
dmetEt->SetMarkerStyle(21);
dmetEt->SetMarkerColor(1);
dmetEt->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mmetEt = (TH1F*)gDirectory->Get("metEt");
mmetEt->SetLineColor(2);
mmetEt->Scale(dmetEt->GetEntries()/mmetEt->GetEntries());
mmetEt->Draw();
dmetEt->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mmetEt->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dmetEt->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/metEt.gif");

TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *dmetPt = (TH1F*)gDirectory->Get("metPt");
dmetPt->SetMarkerStyle(21);
dmetPt->SetMarkerColor(1);
dmetPt->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mmetPt = (TH1F*)gDirectory->Get("metPt");
mmetPt->SetLineColor(2);
mmetPt->Scale(dmetPt->GetEntries()/mmetPt->GetEntries());
dmetPt->Draw("PE1");
mmetPt->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mmetPt->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dmetPt->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/metPt.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *dmetPhi = (TH1F*)gDirectory->Get("metPhi");
dmetPhi->SetMarkerStyle(21);
dmetPhi->SetMarkerColor(1);
dmetPhi->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mmetPhi = (TH1F*)gDirectory->Get("metPhi");
mmetPhi->SetLineColor(2);
mmetPhi->Scale(dmetPhi->GetEntries()/mmetPhi->GetEntries());
dmetPhi->Draw("PE1");
mmetPhi->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mmetPhi->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dmetPhi->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/metPhi.gif");




TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *dpfmetEt = (TH1F*)gDirectory->Get("PFmetEt");
dpfmetEt->SetMarkerStyle(21);
dpfmetEt->SetMarkerColor(1);
dpfmetEt->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mpfmetEt = (TH1F*)gDirectory->Get("PFmetEt");
mpfmetEt->SetLineColor(2);
mpfmetEt->Scale(dpfmetEt->GetEntries()/mpfmetEt->GetEntries());
mpfmetEt->Draw();
dpfmetEt->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mpfmetEt->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dpfmetEt->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/pfmetEt.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
 TH1F *dpfmetPhi = (TH1F*)gDirectory->Get("PFmetPhi");
dpfmetPhi->SetMarkerStyle(21);
dpfmetPhi->SetMarkerColor(1);
dpfmetPhi->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mpfmetPhi = (TH1F*)gDirectory->Get("PFmetPhi");
mpfmetPhi->SetLineColor(2);
mpfmetPhi->Scale(dpfmetPhi->GetEntries()/mpfmetPhi->GetEntries());
mpfmetPhi->Draw();
dpfmetPhi->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mpfmetPhi->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
TPaveStats *s = (TPaveStats*)dpfmetPhi->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
 s->AddText("data");
 s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/pfmetPhi.gif");







//*TAU


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauN = (TH1F*)gDirectory->Get("tauN");
dtauN->SetMarkerStyle(21);
dtauN->SetMarkerColor(1);
dtauN->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauN = (TH1F*)gDirectory->Get("tauN");
mtauN->SetLineColor(2);
mtauN->Scale(dtauN->GetEntries()/mtauN->GetEntries());
dtauN->Draw("PE1");
mtauN->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauN->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauN->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauN.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauPt = (TH1F*)gDirectory->Get("tauPt");
dtauPt->SetMarkerStyle(21);
dtauPt->SetMarkerColor(1);
dtauPt->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauPt = (TH1F*)gDirectory->Get("tauPt");
mtauPt->SetLineColor(2);
mtauPt->Scale(dtauPt->GetEntries()/mtauPt->GetEntries());
dtauPt->Draw("PE1");
mtauPt->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauPt->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauPt->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauPt.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauEta = (TH1F*)gDirectory->Get("tauEta");
dtauEta->SetMarkerStyle(21);
dtauEta->SetMarkerColor(1);
dtauEta->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauEta = (TH1F*)gDirectory->Get("tauEta");
mtauEta->SetLineColor(2);
mtauEta->Scale(dtauEta->GetEntries()/mtauEta->GetEntries());
dtauEta->Draw("PE1");
mtauEta->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauEta->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauEta->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauEta.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauPhi = (TH1F*)gDirectory->Get("tauPhi");
dtauPhi->SetMarkerStyle(21);
dtauPhi->SetMarkerColor(1);
dtauPhi->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauPhi = (TH1F*)gDirectory->Get("tauPhi");
mtauPhi->SetLineColor(2);
mtauPhi->Scale(dtauPhi->GetEntries()/mtauPhi->GetEntries());
mtauPhi->Draw();
dtauPhi->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauPhi->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauPhi->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauPhi.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauCharge = (TH1F*)gDirectory->Get("tauCharge");
dtauCharge->SetMarkerStyle(21);
dtauCharge->SetMarkerColor(1);
dtauCharge->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauCharge = (TH1F*)gDirectory->Get("tauCharge");
mtauCharge->SetLineColor(2);
mtauCharge->Scale(dtauCharge->GetEntries()/mtauCharge->GetEntries());
mtauCharge->Draw();
dtauCharge->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauCharge->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauCharge->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauCharge.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauProngs = (TH1F*)gDirectory->Get("tauProngs");
dtauProngs->SetMarkerStyle(21);
dtauProngs->SetMarkerColor(1);
dtauProngs->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauProngs = (TH1F*)gDirectory->Get("tauProngs");
mtauProngs->SetLineColor(2);
mtauProngs->Scale(dtauProngs->GetEntries()/mtauProngs->GetEntries());
mtauProngs->Draw();
dtauProngs->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauProngs->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauProngs->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauProngs.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauLdgTrkPt = (TH1F*)gDirectory->Get("tauLdgTrkPt");
dtauLdgTrkPt->SetMarkerStyle(21);
dtauLdgTrkPt->SetMarkerColor(1);
dtauLdgTrkPt->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauLdgTrkPt = (TH1F*)gDirectory->Get("tauLdgTrkPt");
mtauLdgTrkPt->SetLineColor(2);
mtauLdgTrkPt->Scale(dtauLdgTrkPt->GetEntries()/mtauLdgTrkPt->GetEntries());
mtauLdgTrkPt->Draw();
dtauLdgTrkPt->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauLdgTrkPt->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauLdgTrkPt->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauLdgTrkPt.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDAEle = (TH1F*)gDirectory->Get("tauDAEle");
dtauDAEle->SetMarkerStyle(21);
dtauDAEle->SetMarkerColor(1);
dtauDAEle->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDAEle = (TH1F*)gDirectory->Get("tauDAEle");
mtauDAEle->SetLineColor(2);
mtauDAEle->Scale(dtauDAEle->GetEntries()/mtauDAEle->GetEntries());
mtauDAEle->Draw();
dtauDAEle->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDAEle->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDAEle->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDAEle.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDAMu = (TH1F*)gDirectory->Get("tauDAMu");
dtauDAMu->SetMarkerStyle(21);
dtauDAMu->SetMarkerColor(1);
dtauDAMu->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDAMu = (TH1F*)gDirectory->Get("tauDAMu");
mtauDAMu->SetLineColor(2);
mtauDAMu->Scale(dtauDAMu->GetEntries()/mtauDAMu->GetEntries());
mtauDAMu->Draw();
dtauDAMu->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDAMu->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDAMu->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDAMu.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDEcalIso = (TH1F*)gDirectory->Get("tauDEcalIso");
dtauDEcalIso->SetMarkerStyle(21);
dtauDEcalIso->SetMarkerColor(1);
dtauDEcalIso->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDEcalIso = (TH1F*)gDirectory->Get("tauDEcalIso");
mtauDEcalIso->SetLineColor(2);
mtauDEcalIso->Scale(dtauDEcalIso->GetEntries()/mtauDEcalIso->GetEntries());
mtauDEcalIso->Draw();
dtauDEcalIso->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDEcalIso->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDEcalIso->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDEcalIso.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDLdgPionIso = (TH1F*)gDirectory->Get("tauDLdgPionIso");
dtauDLdgPionIso->SetMarkerStyle(21);
dtauDLdgPionIso->SetMarkerColor(1);
dtauDLdgPionIso->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDLdgPionIso = (TH1F*)gDirectory->Get("tauDLdgPionIso");
mtauDLdgPionIso->SetLineColor(2);
mtauDLdgPionIso->Scale(dtauDLdgPionIso->GetEntries()/mtauDLdgPionIso->GetEntries());
mtauDLdgPionIso->Draw();
dtauDLdgPionIso->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDLdgPionIso->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDLdgPionIso->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDLdgPionIso.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDLdgPionPt = (TH1F*)gDirectory->Get("tauDLdgPionPt");
dtauDLdgPionPt->SetMarkerStyle(21);
dtauDLdgPionPt->SetMarkerColor(1);
dtauDLdgPionPt->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDLdgPionPt = (TH1F*)gDirectory->Get("tauDLdgPionPt");
mtauDLdgPionPt->SetLineColor(2);
mtauDLdgPionPt->Scale(dtauDLdgPionPt->GetEntries()/mtauDLdgPionPt->GetEntries());
dtauDLdgPionPt->Draw("PE1");
mtauDLdgPionPt->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDLdgPionPt->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDLdgPionPt->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDLdgPionPt.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDLdgTrkFind = (TH1F*)gDirectory->Get("tauDLdgTrkFind");
dtauDLdgTrkFind->SetMarkerStyle(21);
dtauDLdgTrkFind->SetMarkerColor(1);
dtauDLdgTrkFind->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDLdgTrkFind = (TH1F*)gDirectory->Get("tauDLdgTrkFind");
mtauDLdgTrkFind->SetLineColor(2);
mtauDLdgTrkFind->Scale(dtauDLdgTrkFind->GetEntries()/mtauDLdgTrkFind->GetEntries());
mtauDLdgTrkFind->Draw();
dtauDLdgTrkFind->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDLdgTrkFind->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDLdgTrkFind->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDLdgTrkFind.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDLdgTrkPt = (TH1F*)gDirectory->Get("tauDLdgTrkPt");
dtauDLdgTrkPt->SetMarkerStyle(21);
dtauDLdgTrkPt->SetMarkerColor(1);
dtauDLdgTrkPt->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDLdgTrkPt = (TH1F*)gDirectory->Get("tauDLdgTrkPt");
mtauDLdgTrkPt->SetLineColor(2);
mtauDLdgTrkPt->Scale(dtauDLdgTrkPt->GetEntries()/mtauDLdgTrkPt->GetEntries());
dtauDLdgTrkPt->Draw("PE1");
mtauDLdgTrkPt->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDLdgTrkPt->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDLdgTrkPt->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDLdgTrkPt.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDTaNC = (TH1F*)gDirectory->Get("tauDTaNC");
dtauDTaNC->SetMarkerStyle(21);
dtauDTaNC->SetMarkerColor(1);
dtauDTaNC->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDTaNC = (TH1F*)gDirectory->Get("tauDTaNC");
mtauDTaNC->SetLineColor(2);
mtauDTaNC->Scale(dtauDTaNC->GetEntries()/mtauDTaNC->GetEntries());
dtauDTaNC->Draw("PE1");
mtauDTaNC->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDTaNC->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDTaNC->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDTaNC.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDTrkIso = (TH1F*)gDirectory->Get("tauDTrkIso");
dtauDTrkIso->SetMarkerStyle(21);
dtauDTrkIso->SetMarkerColor(1);
dtauDTrkIso->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDTrkIso = (TH1F*)gDirectory->Get("tauDTrkIso");
mtauDTrkIso->SetLineColor(2);
mtauDTrkIso->Scale(dtauDTrkIso->GetEntries()/mtauDTrkIso->GetEntries());
mtauDTrkIso->Draw();
dtauDTrkIso->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDTrkIso->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDTrkIso->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDTrkIso.gif");



// TFile *DT = TFile::Open("Data.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
// DT -> cd ("analyzeBasicPat");
// TH1F *dtau = (TH1F*)gDirectory->Get("tau");
// dtau->SetMarkerStyle(21);
// dtau->SetMarkerColor(1);
// dtau->SetMarkerSize(0.7);
// TFile *MC = TFile::Open("MC.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
// MC -> cd ("analyzeBasicPat");
// TH1F *mtau = (TH1F*)gDirectory->Get("tau");
// mtau->SetLineColor(2);
// mtau->Scale(dtau->GetEntries()/mtau->GetEntries());
// mtau->Draw();
// dtau->Draw("PE1sames");
// Canv->Update();
// TPaveStats *sm = (TPaveStats*)mtau->GetListOfFunctions()->FindObject("stats");
// TPaveStats *s = (TPaveStats*)dtau->GetListOfFunctions()->FindObject("stats");
// sm->AddText("mc");
// sm->SetTextColor(2);
// s->SetX1NDC(0.55);
// s->SetX2NDC (0.75);
// s->SetTextColor(1);
// s->AddText("data");
// s->DrawClone();
// sm->DrawClone(); 
// Canv->SaveAs("./DataMC/tau.gif");

///  ***** TAU 0 10



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauEta010 = (TH1F*)gDirectory->Get("tauEta010");
dtauEta010->SetMarkerStyle(21);
dtauEta010->SetMarkerColor(1);
dtauEta010->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauEta010 = (TH1F*)gDirectory->Get("tauEta010");
mtauEta010->SetLineColor(2);
mtauEta010->Scale(dtauEta010->GetEntries()/mtauEta010->GetEntries());
dtauEta010->Draw("PE1");
mtauEta010->Draw("sames");
Canv->Update();
TPaveStats *s = (TPaveStats*)dtauEta010->GetListOfFunctions()->FindObject("stats");
s->SetX1NDC(0.55);
s->SetX2NDC(0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
TPaveStats *sm = (TPaveStats*)mtauEta010->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauEta010.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauPhi010 = (TH1F*)gDirectory->Get("tauPhi010");
dtauPhi010->SetMarkerStyle(21);
dtauPhi010->SetMarkerColor(1);
dtauPhi010->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauPhi010 = (TH1F*)gDirectory->Get("tauPhi010");
mtauPhi010->SetLineColor(2);
mtauPhi010->Scale(dtauPhi010->GetEntries()/mtauPhi010->GetEntries());
mtauPhi010->Draw();
dtauPhi010->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauPhi010->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauPhi010->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauPhi010.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauCharge010 = (TH1F*)gDirectory->Get("tauCharge010");
dtauCharge010->SetMarkerStyle(21);
dtauCharge010->SetMarkerColor(1);
dtauCharge010->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauCharge010 = (TH1F*)gDirectory->Get("tauCharge010");
mtauCharge010->SetLineColor(2);
mtauCharge010->Scale(dtauCharge010->GetEntries()/mtauCharge010->GetEntries());
mtauCharge010->Draw();
dtauCharge010->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauCharge010->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauCharge010->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauCharge010.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauProngs010 = (TH1F*)gDirectory->Get("tauProngs010");
dtauProngs010->SetMarkerStyle(21);
dtauProngs010->SetMarkerColor(1);
dtauProngs010->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauProngs010 = (TH1F*)gDirectory->Get("tauProngs010");
mtauProngs010->SetLineColor(2);
mtauProngs010->Scale(dtauProngs010->GetEntries()/mtauProngs010->GetEntries());
mtauProngs010->Draw();
dtauProngs010->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauProngs010->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauProngs010->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauProngs010.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauLdgTrkPt010 = (TH1F*)gDirectory->Get("tauLdgTrkPt010");
dtauLdgTrkPt010->SetMarkerStyle(21);
dtauLdgTrkPt010->SetMarkerColor(1);
dtauLdgTrkPt010->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauLdgTrkPt010 = (TH1F*)gDirectory->Get("tauLdgTrkPt");
mtauLdgTrkPt010->SetLineColor(2);
mtauLdgTrkPt010->Scale(dtauLdgTrkPt010->GetEntries()/mtauLdgTrkPt010->GetEntries());
mtauLdgTrkPt010->Draw();
dtauLdgTrkPt010->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauLdgTrkPt010->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauLdgTrkPt010->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauLdgTrkPt010.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDAEle010 = (TH1F*)gDirectory->Get("tauDAEle010");
dtauDAEle010->SetMarkerStyle(21);
dtauDAEle010->SetMarkerColor(1);
dtauDAEle010->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDAEle010 = (TH1F*)gDirectory->Get("tauDAEle010");
mtauDAEle010->SetLineColor(2);
mtauDAEle010->Scale(dtauDAEle010->GetEntries()/mtauDAEle010->GetEntries());
mtauDAEle010->Draw();
dtauDAEle010->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDAEle010->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDAEle010->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDAEle010.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDAMu010 = (TH1F*)gDirectory->Get("tauDAMu010");
dtauDAMu010->SetMarkerStyle(21);
dtauDAMu010->SetMarkerColor(1);
dtauDAMu010->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDAMu010 = (TH1F*)gDirectory->Get("tauDAMu010");
mtauDAMu010->SetLineColor(2);
mtauDAMu010->Scale(dtauDAMu010->GetEntries()/mtauDAMu010->GetEntries());
mtauDAMu010->Draw();
dtauDAMu010->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDAMu010->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDAMu010->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDAMu010.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDEcalIso010 = (TH1F*)gDirectory->Get("tauDEcalIso010");
dtauDEcalIso010->SetMarkerStyle(21);
dtauDEcalIso010->SetMarkerColor(1);
dtauDEcalIso010->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDEcalIso010 = (TH1F*)gDirectory->Get("tauDEcalIso010");
mtauDEcalIso010->SetLineColor(2);
mtauDEcalIso010->Scale(dtauDEcalIso010->GetEntries()/mtauDEcalIso010->GetEntries());
mtauDEcalIso010->Draw();
dtauDEcalIso010->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDEcalIso010->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDEcalIso010->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDEcalIso010.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDLdgPionIso010 = (TH1F*)gDirectory->Get("tauDLdgPionIso010");
dtauDLdgPionIso010->SetMarkerStyle(21);
dtauDLdgPionIso010->SetMarkerColor(1);
dtauDLdgPionIso010->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDLdgPionIso010 = (TH1F*)gDirectory->Get("tauDLdgPionIso010");
mtauDLdgPionIso010->SetLineColor(2);
mtauDLdgPionIso010->Scale(dtauDLdgPionIso010->GetEntries()/mtauDLdgPionIso010->GetEntries());
mtauDLdgPionIso010->Draw();
dtauDLdgPionIso010->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDLdgPionIso010->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDLdgPionIso010->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDLdgPionIso010.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDLdgPionPt010 = (TH1F*)gDirectory->Get("tauDLdgPionPt010");
dtauDLdgPionPt010->SetMarkerStyle(21);
dtauDLdgPionPt010->SetMarkerColor(1);
dtauDLdgPionPt010->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDLdgPionPt010 = (TH1F*)gDirectory->Get("tauDLdgPionPt010");
mtauDLdgPionPt010->SetLineColor(2);
mtauDLdgPionPt010->Scale(dtauDLdgPionPt010->GetEntries()/mtauDLdgPionPt010->GetEntries());
 dtauDLdgPionPt010->Draw("PE1");
tauDLdgPionPt010->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDLdgPionPt010->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDLdgPionPt010->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDLdgPionPt010.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDLdgTrkFind010 = (TH1F*)gDirectory->Get("tauDLdgTrkFind010");
dtauDLdgTrkFind010->SetMarkerStyle(21);
dtauDLdgTrkFind010->SetMarkerColor(1);
dtauDLdgTrkFind010->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDLdgTrkFind010 = (TH1F*)gDirectory->Get("tauDLdgTrkFind010");
mtauDLdgTrkFind010->SetLineColor(2);
mtauDLdgTrkFind010->Scale(dtauDLdgTrkFind010->GetEntries()/mtauDLdgTrkFind010->GetEntries());
mtauDLdgTrkFind010->Draw();
dtauDLdgTrkFind010->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDLdgTrkFind010->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDLdgTrkFind010->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDLdgTrkFind010.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDLdgTrkPt010 = (TH1F*)gDirectory->Get("tauDLdgTrkPt010");
dtauDLdgTrkPt010->SetMarkerStyle(21);
dtauDLdgTrkPt010->SetMarkerColor(1);
dtauDLdgTrkPt010->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDLdgTrkPt010 = (TH1F*)gDirectory->Get("tauDLdgTrkPt010");
mtauDLdgTrkPt010->SetLineColor(2);
mtauDLdgTrkPt010->Scale(dtauDLdgTrkPt010->GetEntries()/mtauDLdgTrkPt010->GetEntries());
dtauDLdgTrkPt010->Draw("PE1");
mtauDLdgTrkPt010->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDLdgTrkPt010->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDLdgTrkPt010->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDLdgTrkPt010.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDTaNC010 = (TH1F*)gDirectory->Get("tauDTaNC010");
dtauDTaNC010->SetMarkerStyle(21);
dtauDTaNC010->SetMarkerColor(1);
dtauDTaNC010->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDTaNC010 = (TH1F*)gDirectory->Get("tauDTaNC010");
mtauDTaNC010->SetLineColor(2);
mtauDTaNC010->Scale(dtauDTaNC010->GetEntries()/mtauDTaNC010->GetEntries());
dtauDTaNC010->Draw("PE1");
mtauDTaNC010->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDTaNC010->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDTaNC010->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDTaNC010.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDTrkIso010 = (TH1F*)gDirectory->Get("tauDTrkIso010");
dtauDTrkIso010->SetMarkerStyle(21);
dtauDTrkIso010->SetMarkerColor(1);
dtauDTrkIso010->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F  *mtauDTrkIso010 = (TH1F*)gDirectory->Get("tauDTrkIso010");
mtauDTrkIso010->SetLineColor(2);
mtauDTrkIso010->Scale(dtauDTrkIso010->GetEntries()/mtauDTrkIso010->GetEntries());
mtauDTrkIso010->Draw();
dtauDTrkIso010->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDTrkIso010->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDTrkIso010->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDTrkIso010.gif");




//////////// ******** TAU 1015

// TFile *DT = TFile::Open("Data.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
// DT -> cd ("analyzeBasicPat");
// TH1F *dtauN1015 = (TH1F*)gDirectory->Get("tauN1015");
// dtauN1015->SetMarkerStyle(21);
// dtauN1015->SetMarkerColor(1);
// dtauN1015->SetMarkerSize(0.7);
// TFile *MC = TFile::Open("MC.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
// MC -> cd ("analyzeBasicPat");
// TH1F *mtauN1015 = (TH1F*)gDirectory->Get("tauN1015");
// mtauN1015->SetLineColor(2);
// mtauN1015->Scale(dtauN1015->GetEntries()/mtauN1015->GetEntries());
// dtauN1015->Draw("PE1");
// mtauN1015->Draw("sames");
// Canv->Update();
// TPaveStats *sm = (TPaveStats*)mtauN1015->GetListOfFunctions()->FindObject("stats");
// TPaveStats *s = (TPaveStats*)dtauN1015->GetListOfFunctions()->FindObject("stats");
// sm->AddText("mc");
// sm->SetTextColor(2);
// s->SetX1NDC(0.55);
// s->SetX2NDC (0.75);
// s->SetTextColor(1);
// s->AddText("data");
// s->DrawClone();
// sm->DrawClone(); 
// Canv->SaveAs("./DataMC/tauN1015.gif");


// TFile *DT = TFile::Open("Data.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
// DT -> cd ("analyzeBasicPat");
// TH1F *dtauPt1015 = (TH1F*)gDirectory->Get("tauPt1015");
// dtauPt1015->SetMarkerStyle(21);
// dtauPt1015->SetMarkerColor(1);
// dtauPt1015->SetMarkerSize(0.7);
// TFile *MC = TFile::Open("MC.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
// MC -> cd ("analyzeBasicPat");
// TH1F *mtauPt1015 = (TH1F*)gDirectory->Get("tauPt1015");
// mtauPt1015->SetLineColor(2);
// mtauPt1015->Scale(dtauPt1015->GetEntries()/mtauPt1015->GetEntries());
// mtauPt1015->Draw();
// dtauPt1015->Draw("PE1sames");
// Canv->Update();
// TPaveStats *sm = (TPaveStats*)mtauPt1015->GetListOfFunctions()->FindObject("stats");
// TPaveStats *s = (TPaveStats*)dtauPt1015->GetListOfFunctions()->FindObject("stats");
// sm->AddText("mc");
// sm->SetTextColor(2);
// s->SetX1NDC(0.55);
// s->SetX2NDC (0.75);
// s->SetTextColor(1);
// s->AddText("data");
// s->DrawClone();
// sm->DrawClone(); 
// Canv->SaveAs("./DataMC/tauPt1015.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauEta1015 = (TH1F*)gDirectory->Get("tauEta1015");
dtauEta1015->SetMarkerStyle(21);
dtauEta1015->SetMarkerColor(1);
dtauEta1015->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauEta1015 = (TH1F*)gDirectory->Get("tauEta1015");
mtauEta1015->SetLineColor(2);
mtauEta1015->Scale(dtauEta1015->GetEntries()/mtauEta1015->GetEntries());
dtauEta1015->Draw("PE1");
mtauEta1015->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauEta1015->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauEta1015->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauEta1015.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauPhi1015 = (TH1F*)gDirectory->Get("tauPhi1015");
dtauPhi1015->SetMarkerStyle(21);
dtauPhi1015->SetMarkerColor(1);
dtauPhi1015->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauPhi1015 = (TH1F*)gDirectory->Get("tauPhi1015");
mtauPhi1015->SetLineColor(2);
mtauPhi1015->Scale(dtauPhi1015->GetEntries()/mtauPhi1015->GetEntries());
dtauPhi1015->Draw("PE1");
mtauPhi1015->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauPhi1015->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauPhi1015->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauPhi1015.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauCharge1015 = (TH1F*)gDirectory->Get("tauCharge1015");
dtauCharge1015->SetMarkerStyle(21);
dtauCharge1015->SetMarkerColor(1);
dtauCharge1015->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauCharge1015 = (TH1F*)gDirectory->Get("tauCharge1015");
mtauCharge1015->SetLineColor(2);
mtauCharge1015->Scale(dtauCharge1015->GetEntries()/mtauCharge1015->GetEntries());
mtauCharge1015->Draw();
dtauCharge1015->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauCharge1015->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauCharge1015->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauCharge1015.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauProngs1015 = (TH1F*)gDirectory->Get("tauProngs1015");
dtauProngs1015->SetMarkerStyle(21);
dtauProngs1015->SetMarkerColor(1);
dtauProngs1015->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauProngs1015 = (TH1F*)gDirectory->Get("tauProngs1015");
mtauProngs1015->SetLineColor(2);
mtauProngs1015->Scale(dtauProngs1015->GetEntries()/mtauProngs1015->GetEntries());
mtauProngs1015->Draw();
dtauProngs1015->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauProngs1015->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauProngs1015->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauProngs1015.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauLdgTrkPt1015 = (TH1F*)gDirectory->Get("tauLdgTrkPt1015");
dtauLdgTrkPt1015->SetMarkerStyle(21);
dtauLdgTrkPt1015->SetMarkerColor(1);
dtauLdgTrkPt1015->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauLdgTrkPt1015 = (TH1F*)gDirectory->Get("tauLdgTrkPt1015");
mtauLdgTrkPt1015->SetLineColor(2);
mtauLdgTrkPt1015->Scale(dtauLdgTrkPt1015->GetEntries()/mtauLdgTrkPt1015->GetEntries());
mtauLdgTrkPt1015->Draw();
dtauLdgTrkPt1015->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauLdgTrkPt1015->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauLdgTrkPt1015->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauLdgTrkPt1015.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDAEle1015 = (TH1F*)gDirectory->Get("tauDAEle1015");
dtauDAEle1015->SetMarkerStyle(21);
dtauDAEle1015->SetMarkerColor(1);
dtauDAEle1015->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDAEle1015 = (TH1F*)gDirectory->Get("tauDAEle1015");
mtauDAEle1015->SetLineColor(2);
mtauDAEle1015->Scale(dtauDAEle1015->GetEntries()/mtauDAEle1015->GetEntries());
mtauDAEle1015->Draw();
dtauDAEle1015->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDAEle1015->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDAEle1015->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDAEle1015.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDAMu1015 = (TH1F*)gDirectory->Get("tauDAMu1015");
dtauDAMu1015->SetMarkerStyle(21);
dtauDAMu1015->SetMarkerColor(1);
dtauDAMu1015->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDAMu1015 = (TH1F*)gDirectory->Get("tauDAMu1015");
mtauDAMu1015->SetLineColor(2);
mtauDAMu1015->Scale(dtauDAMu1015->GetEntries()/mtauDAMu1015->GetEntries());
mtauDAMu1015->Draw();
dtauDAMu1015->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDAMu1015->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDAMu1015->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDAMu1015.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDEcalIso1015 = (TH1F*)gDirectory->Get("tauDEcalIso1015");
dtauDEcalIso1015->SetMarkerStyle(21);
dtauDEcalIso1015->SetMarkerColor(1);
dtauDEcalIso1015->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDEcalIso1015 = (TH1F*)gDirectory->Get("tauDEcalIso1015");
mtauDEcalIso1015->SetLineColor(2);
mtauDEcalIso1015->Scale(dtauDEcalIso1015->GetEntries()/mtauDEcalIso1015->GetEntries());
mtauDEcalIso1015->Draw();
dtauDEcalIso1015->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDEcalIso1015->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDEcalIso1015->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDEcalIso1015.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDLdgPionIso1015 = (TH1F*)gDirectory->Get("tauDLdgPionIso1015");
dtauDLdgPionIso1015->SetMarkerStyle(21);
dtauDLdgPionIso1015->SetMarkerColor(1);
dtauDLdgPionIso1015->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDLdgPionIso1015 = (TH1F*)gDirectory->Get("tauDLdgPionIso1015");
mtauDLdgPionIso1015->SetLineColor(2);
mtauDLdgPionIso1015->Scale(dtauDLdgPionIso1015->GetEntries()/mtauDLdgPionIso1015->GetEntries());
mtauDLdgPionIso1015->Draw();
dtauDLdgPionIso1015->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDLdgPionIso1015->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDLdgPionIso1015->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDLdgPionIso1015.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDLdgPionPt1015 = (TH1F*)gDirectory->Get("tauDLdgPionPt1015");
dtauDLdgPionPt1015->SetMarkerStyle(21);
dtauDLdgPionPt1015->SetMarkerColor(1);
dtauDLdgPionPt1015->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDLdgPionPt1015 = (TH1F*)gDirectory->Get("tauDLdgPionPt1015");
mtauDLdgPionPt1015->SetLineColor(2);
mtauDLdgPionPt1015->Scale(dtauDLdgPionPt1015->GetEntries()/mtauDLdgPionPt1015->GetEntries());
dtauDLdgPionPt1015->Draw("PE1");
mtauDLdgPionPt1015->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDLdgPionPt1015->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDLdgPionPt1015->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDLdgPionPt1015.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDLdgTrkFind1015 = (TH1F*)gDirectory->Get("tauDLdgTrkFind1015");
dtauDLdgTrkFind1015->SetMarkerStyle(21);
dtauDLdgTrkFind1015->SetMarkerColor(1);
dtauDLdgTrkFind1015->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDLdgTrkFind1015 = (TH1F*)gDirectory->Get("tauDLdgTrkFind1015");
mtauDLdgTrkFind1015->SetLineColor(2);
mtauDLdgTrkFind1015->Scale(dtauDLdgTrkFind1015->GetEntries()/mtauDLdgTrkFind1015->GetEntries());
mtauDLdgTrkFind1015->Draw();
dtauDLdgTrkFind1015->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDLdgTrkFind1015->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDLdgTrkFind1015->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDLdgTrkFind1015.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDLdgTrkPt1015 = (TH1F*)gDirectory->Get("tauDLdgTrkPt1015");
dtauDLdgTrkPt1015->SetMarkerStyle(21);
dtauDLdgTrkPt1015->SetMarkerColor(1);
dtauDLdgTrkPt1015->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDLdgTrkPt1015 = (TH1F*)gDirectory->Get("tauDLdgTrkPt1015");
mtauDLdgTrkPt1015->SetLineColor(2);
mtauDLdgTrkPt1015->Scale(dtauDLdgTrkPt1015->GetEntries()/mtauDLdgTrkPt1015->GetEntries());
dtauDLdgTrkPt1015->Draw("PE1");
mtauDLdgTrkPt1015->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDLdgTrkPt1015->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDLdgTrkPt1015->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDLdgTrkPt1015.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDTaNC1015 = (TH1F*)gDirectory->Get("tauDTaNC1015");
dtauDTaNC1015->SetMarkerStyle(21);
dtauDTaNC1015->SetMarkerColor(1);
dtauDTaNC1015->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDTaNC1015 = (TH1F*)gDirectory->Get("tauDTaNC1015");
mtauDTaNC1015->SetLineColor(2);
mtauDTaNC1015->Scale(dtauDTaNC1015->GetEntries()/mtauDTaNC1015->GetEntries());
dtauDTaNC1015->Draw("PE1");
mtauDTaNC1015->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDTaNC1015->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDTaNC1015->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDTaNC1015.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDTrkIso1015 = (TH1F*)gDirectory->Get("tauDTrkIso1015");
dtauDTrkIso1015->SetMarkerStyle(21);
dtauDTrkIso1015->SetMarkerColor(1);
dtauDTrkIso1015->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDTrkIso1015 = (TH1F*)gDirectory->Get("tauDTrkIso1015");
mtauDTrkIso1015->SetLineColor(2);
mtauDTrkIso1015->Scale(dtauDTrkIso1015->GetEntries()/mtauDTrkIso1015->GetEntries());
mtauDTrkIso1015->Draw();
dtauDTrkIso1015->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDTrkIso1015->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDTrkIso1015->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDTrkIso1015.gif");


////////  TAU 1520


// TFile *DT = TFile::Open("Data.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
// DT -> cd ("analyzeBasicPat");
// TH1F *dtauN1520 = (TH1F*)gDirectory->Get("tauN1520");
// dtauN1520->SetMarkerStyle(21);
// dtauN1520->SetMarkerColor(1);
// dtauN1520->SetMarkerSize(0.7);
// TFile *MC = TFile::Open("MC.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
// MC -> cd ("analyzeBasicPat");
// TH1F *mtauN1520 = (TH1F*)gDirectory->Get("tauN1520");
// mtauN1520->SetLineColor(2);
// mtauN1520->Scale(dtauN1520->GetEntries()/mtauN1520->GetEntries());
// dtauN1520->Draw("PE1");
// mtauN1520->Draw("sames");
// Canv->Update();
// TPaveStats *sm = (TPaveStats*)mtauN1520->GetListOfFunctions()->FindObject("stats");
// TPaveStats *s = (TPaveStats*)dtauN1520->GetListOfFunctions()->FindObject("stats");
// sm->AddText("mc");
// sm->SetTextColor(2);
// s->SetX1NDC(0.55);
// s->SetX2NDC (0.75);
// s->SetTextColor(1);
// s->AddText("data");
// s->DrawClone();
// sm->DrawClone(); 
// Canv->SaveAs("./DataMC/tauN1520.gif");


// TFile *DT = TFile::Open("Data.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
// DT -> cd ("analyzeBasicPat");
// TH1F *dtauPt1520 = (TH1F*)gDirectory->Get("tauPt1520");
// dtauPt1520->SetMarkerStyle(21);
// dtauPt1520->SetMarkerColor(1);
// dtauPt1520->SetMarkerSize(0.7);
// TFile *MC = TFile::Open("MC.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
// MC -> cd ("analyzeBasicPat");
// TH1F *mtauPt1520 = (TH1F*)gDirectory->Get("tauPt1520");
// mtauPt1520->SetLineColor(2);
// mtauPt1520->Scale(dtauPt1520->GetEntries()/mtauPt1520->GetEntries());
// mtauPt1520->Draw();
// dtauPt1520->Draw("PE1sames");
// Canv->Update();
// TPaveStats *sm = (TPaveStats*)mtauPt1520->GetListOfFunctions()->FindObject("stats");
// TPaveStats *s = (TPaveStats*)dtauPt1520->GetListOfFunctions()->FindObject("stats");
// sm->AddText("mc");
// sm->SetTextColor(2);
// s->SetX1NDC(0.55);
// s->SetX2NDC (0.75);
// s->SetTextColor(1);
// s->AddText("data");
// s->DrawClone();
// sm->DrawClone(); 
// Canv->SaveAs("./DataMC/tauPt1520.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauEta1520 = (TH1F*)gDirectory->Get("tauEta1520");
dtauEta1520->SetMarkerStyle(21);
dtauEta1520->SetMarkerColor(1);
dtauEta1520->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauEta1520 = (TH1F*)gDirectory->Get("tauEta1520");
mtauEta1520->SetLineColor(2);
mtauEta1520->Scale(dtauEta1520->GetEntries()/mtauEta1520->GetEntries());
dtauEta1520->Draw("PE1");
mtauEta1520->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauEta1520->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauEta1520->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauEta1520.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauPhi1520 = (TH1F*)gDirectory->Get("tauPhi1520");
dtauPhi1520->SetMarkerStyle(21);
dtauPhi1520->SetMarkerColor(1);
dtauPhi1520->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauPhi1520 = (TH1F*)gDirectory->Get("tauPhi1520");
mtauPhi1520->SetLineColor(2);
mtauPhi1520->Scale(dtauPhi1520->GetEntries()/mtauPhi1520->GetEntries());
dtauPhi1520->Draw("PE1");
mtauPhi1520->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauPhi1520->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauPhi1520->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauPhi1520.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauCharge1520 = (TH1F*)gDirectory->Get("tauCharge1520");
dtauCharge1520->SetMarkerStyle(21);
dtauCharge1520->SetMarkerColor(1);
dtauCharge1520->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauCharge1520 = (TH1F*)gDirectory->Get("tauCharge1520");
mtauCharge1520->SetLineColor(2);
mtauCharge1520->Scale(dtauCharge1520->GetEntries()/mtauCharge1520->GetEntries());
mtauCharge1520->Draw();
dtauCharge1520->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauCharge1520->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauCharge1520->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauCharge1520.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauProngs1520 = (TH1F*)gDirectory->Get("tauProngs1520");
dtauProngs1520->SetMarkerStyle(21);
dtauProngs1520->SetMarkerColor(1);
dtauProngs1520->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauProngs1520 = (TH1F*)gDirectory->Get("tauProngs1520");
mtauProngs1520->SetLineColor(2);
mtauProngs1520->Scale(dtauProngs1520->GetEntries()/mtauProngs1520->GetEntries());
mtauProngs1520->Draw();
dtauProngs1520->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauProngs1520->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauProngs1520->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauProngs1520.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauLdgTrkPt1520 = (TH1F*)gDirectory->Get("tauLdgTrkPt1520");
dtauLdgTrkPt1520->SetMarkerStyle(21);
dtauLdgTrkPt1520->SetMarkerColor(1);
dtauLdgTrkPt1520->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauLdgTrkPt1520 = (TH1F*)gDirectory->Get("tauLdgTrkPt1520");
mtauLdgTrkPt1520->SetLineColor(2);
mtauLdgTrkPt1520->Scale(dtauLdgTrkPt1520->GetEntries()/mtauLdgTrkPt1520->GetEntries());
dtauLdgTrkPt1520->Draw("PE1");
mtauLdgTrkPt1520->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauLdgTrkPt1520->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauLdgTrkPt1520->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauLdgTrkPt1520.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDAEle1520 = (TH1F*)gDirectory->Get("tauDAEle1520");
dtauDAEle1520->SetMarkerStyle(21);
dtauDAEle1520->SetMarkerColor(1);
dtauDAEle1520->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDAEle1520 = (TH1F*)gDirectory->Get("tauDAEle1520");
mtauDAEle1520->SetLineColor(2);
mtauDAEle1520->Scale(dtauDAEle1520->GetEntries()/mtauDAEle1520->GetEntries());
mtauDAEle1520->Draw();
dtauDAEle1520->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDAEle1520->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDAEle1520->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDAEle1520.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDAMu1520 = (TH1F*)gDirectory->Get("tauDAMu1520");
dtauDAMu1520->SetMarkerStyle(21);
dtauDAMu1520->SetMarkerColor(1);
dtauDAMu1520->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDAMu1520 = (TH1F*)gDirectory->Get("tauDAMu1520");
mtauDAMu1520->SetLineColor(2);
mtauDAMu1520->Scale(dtauDAMu1520->GetEntries()/mtauDAMu1520->GetEntries());
mtauDAMu1520->Draw();
dtauDAMu1520->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDAMu1520->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDAMu1520->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDAMu1520.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDEcalIso1520 = (TH1F*)gDirectory->Get("tauDEcalIso1520");
dtauDEcalIso1520->SetMarkerStyle(21);
dtauDEcalIso1520->SetMarkerColor(1);
dtauDEcalIso1520->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDEcalIso1520 = (TH1F*)gDirectory->Get("tauDEcalIso1520");
mtauDEcalIso1520->SetLineColor(2);
mtauDEcalIso1520->Scale(dtauDEcalIso1520->GetEntries()/mtauDEcalIso1520->GetEntries());
mtauDEcalIso1520->Draw();
dtauDEcalIso1520->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDEcalIso1520->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDEcalIso1520->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDEcalIso1520.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDLdgPionIso1520 = (TH1F*)gDirectory->Get("tauDLdgPionIso1520");
dtauDLdgPionIso1520->SetMarkerStyle(21);
dtauDLdgPionIso1520->SetMarkerColor(1);
dtauDLdgPionIso1520->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDLdgPionIso1520 = (TH1F*)gDirectory->Get("tauDLdgPionIso1520");
mtauDLdgPionIso1520->SetLineColor(2);
mtauDLdgPionIso1520->Scale(dtauDLdgPionIso1520->GetEntries()/mtauDLdgPionIso1520->GetEntries());
mtauDLdgPionIso1520->Draw();
dtauDLdgPionIso1520->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDLdgPionIso1520->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDLdgPionIso1520->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDLdgPionIso1520.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDLdgPionPt1520 = (TH1F*)gDirectory->Get("tauDLdgPionPt1520");
dtauDLdgPionPt1520->SetMarkerStyle(21);
dtauDLdgPionPt1520->SetMarkerColor(1);
dtauDLdgPionPt1520->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDLdgPionPt1520 = (TH1F*)gDirectory->Get("tauDLdgPionPt1520");
mtauDLdgPionPt1520->SetLineColor(2);
mtauDLdgPionPt1520->Scale(dtauDLdgPionPt1520->GetEntries()/mtauDLdgPionPt1520->GetEntries());
dtauDLdgPionPt1520->Draw("PE1");
mtauDLdgPionPt1520->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDLdgPionPt1520->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDLdgPionPt1520->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDLdgPionPt1520.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDLdgTrkFind1520 = (TH1F*)gDirectory->Get("tauDLdgTrkFind1520");
dtauDLdgTrkFind1520->SetMarkerStyle(21);
dtauDLdgTrkFind1520->SetMarkerColor(1);
dtauDLdgTrkFind1520->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDLdgTrkFind1520 = (TH1F*)gDirectory->Get("tauDLdgTrkFind1520");
mtauDLdgTrkFind1520->SetLineColor(2);
mtauDLdgTrkFind1520->Scale(dtauDLdgTrkFind1520->GetEntries()/mtauDLdgTrkFind1520->GetEntries());
mtauDLdgTrkFind1520->Draw();
dtauDLdgTrkFind1520->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDLdgTrkFind1520->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDLdgTrkFind1520->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDLdgTrkFind1520.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDLdgTrkPt1520 = (TH1F*)gDirectory->Get("tauDLdgTrkPt1520");
dtauDLdgTrkPt1520->SetMarkerStyle(21);
dtauDLdgTrkPt1520->SetMarkerColor(1);
dtauDLdgTrkPt1520->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDLdgTrkPt1520 = (TH1F*)gDirectory->Get("tauDLdgTrkPt1520");
mtauDLdgTrkPt1520->SetLineColor(2);
mtauDLdgTrkPt1520->Scale(dtauDLdgTrkPt1520->GetEntries()/mtauDLdgTrkPt1520->GetEntries());
dtauDLdgTrkPt1520->Draw("PE1");
mtauDLdgTrkPt1520->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDLdgTrkPt1520->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDLdgTrkPt1520->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDLdgTrkPt1520.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDTaNC1520 = (TH1F*)gDirectory->Get("tauDTaNC1520");
dtauDTaNC1520->SetMarkerStyle(21);
dtauDTaNC1520->SetMarkerColor(1);
dtauDTaNC1520->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDTaNC1520 = (TH1F*)gDirectory->Get("tauDTaNC1520");
mtauDTaNC1520->SetLineColor(2);
mtauDTaNC1520->Scale(dtauDTaNC1520->GetEntries()/mtauDTaNC1520->GetEntries());
dtauDTaNC1520->Draw("PE1");
mtauDTaNC1520->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDTaNC1520->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDTaNC1520->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDTaNC1520.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDTrkIso1520 = (TH1F*)gDirectory->Get("tauDTrkIso1520");
dtauDTrkIso1520->SetMarkerStyle(21);
dtauDTrkIso1520->SetMarkerColor(1);
dtauDTrkIso1520->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDTrkIso1520 = (TH1F*)gDirectory->Get("tauDTrkIso1520");
mtauDTrkIso1520->SetLineColor(2);
mtauDTrkIso1520->Scale(dtauDTrkIso1520->GetEntries()/mtauDTrkIso1520->GetEntries());
mtauDTrkIso1520->Draw();
dtauDTrkIso1520->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDTrkIso1520->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDTrkIso1520->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDTrkIso1520.gif");







//// *******   TAU 2025


// TFile *DT = TFile::Open("Data.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
// DT -> cd ("analyzeBasicPat");
// TH1F *dtauN2025 = (TH1F*)gDirectory->Get("tauN2025");
// dtauN2025->SetMarkerStyle(21);
// dtauN2025->SetMarkerColor(1);
// dtauN2025->SetMarkerSize(0.7);
// TFile *MC = TFile::Open("MC.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
// MC -> cd ("analyzeBasicPat");
// TH1F *mtauN2025 = (TH1F*)gDirectory->Get("tauN2025");
// mtauN2025->SetLineColor(2);
// mtauN2025->Scale(dtauN2025->GetEntries()/mtauN2025->GetEntries());
// dtauN2025->Draw("PE1");
// mtauN2025->Draw("sames");
// Canv->Update();
// TPaveStats *sm = (TPaveStats*)mtauN2025->GetListOfFunctions()->FindObject("stats");
// TPaveStats *s = (TPaveStats*)dtauN2025->GetListOfFunctions()->FindObject("stats");
// sm->AddText("mc");
// sm->SetTextColor(2);
// s->SetX1NDC(0.55);
// s->SetX2NDC (0.75);
// s->SetTextColor(1);
// s->AddText("data");
// s->DrawClone();
// sm->DrawClone(); 
// Canv->SaveAs("./DataMC/tauN2025.gif");


// TFile *DT = TFile::Open("Data.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
// DT -> cd ("analyzeBasicPat");
// TH1F *dtauPt2025 = (TH1F*)gDirectory->Get("tauPt2025");
// dtauPt2025->SetMarkerStyle(21);
// dtauPt2025->SetMarkerColor(1);
// dtauPt2025->SetMarkerSize(0.7);
// TFile *MC = TFile::Open("MC.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
// MC -> cd ("analyzeBasicPat");
// TH1F *mtauPt2025 = (TH1F*)gDirectory->Get("tauPt2025");
// mtauPt2025->SetLineColor(2);
// mtauPt2025->Scale(dtauPt2025->GetEntries()/mtauPt2025->GetEntries());
// mtauPt2025->Draw();
// dtauPt2025->Draw("PE1sames");
// Canv->Update();
// TPaveStats *sm = (TPaveStats*)mtauPt2025->GetListOfFunctions()->FindObject("stats");
// TPaveStats *s = (TPaveStats*)dtauPt2025->GetListOfFunctions()->FindObject("stats");
// sm->AddText("mc");
// sm->SetTextColor(2);
// s->SetX1NDC(0.55);
// s->SetX2NDC (0.75);
// s->SetTextColor(1);
// s->AddText("data");
// s->DrawClone();
// sm->DrawClone(); 
// Canv->SaveAs("./DataMC/tauPt2025.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauEta2025 = (TH1F*)gDirectory->Get("tauEta2025");
dtauEta2025->SetMarkerStyle(21);
dtauEta2025->SetMarkerColor(1);
dtauEta2025->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauEta2025 = (TH1F*)gDirectory->Get("tauEta2025");
mtauEta2025->SetLineColor(2);
mtauEta2025->Scale(dtauEta2025->GetEntries()/mtauEta2025->GetEntries());
mtauEta2025->Draw();
dtauEta2025->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauEta2025->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauEta2025->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauEta2025.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauPhi2025 = (TH1F*)gDirectory->Get("tauPhi2025");
dtauPhi2025->SetMarkerStyle(21);
dtauPhi2025->SetMarkerColor(1);
dtauPhi2025->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauPhi2025 = (TH1F*)gDirectory->Get("tauPhi2025");
mtauPhi2025->SetLineColor(2);
mtauPhi2025->Scale(dtauPhi2025->GetEntries()/mtauPhi2025->GetEntries());
dtauPhi2025->Draw("PE1");
mtauPhi2025->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauPhi2025->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauPhi2025->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauPhi2025.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauCharge2025 = (TH1F*)gDirectory->Get("tauCharge2025");
dtauCharge2025->SetMarkerStyle(21);
dtauCharge2025->SetMarkerColor(1);
dtauCharge2025->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauCharge2025 = (TH1F*)gDirectory->Get("tauCharge2025");
mtauCharge2025->SetLineColor(2);
mtauCharge2025->Scale(dtauCharge2025->GetEntries()/mtauCharge2025->GetEntries());
mtauCharge2025->Draw();
dtauCharge2025->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauCharge2025->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauCharge2025->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauCharge2025.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauProngs2025 = (TH1F*)gDirectory->Get("tauProngs2025");
dtauProngs2025->SetMarkerStyle(21);
dtauProngs2025->SetMarkerColor(1);
dtauProngs2025->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauProngs2025 = (TH1F*)gDirectory->Get("tauProngs2025");
mtauProngs2025->SetLineColor(2);
mtauProngs2025->Scale(dtauProngs2025->GetEntries()/mtauProngs2025->GetEntries());
mtauProngs2025->Draw();
dtauProngs2025->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauProngs2025->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauProngs2025->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauProngs2025.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauLdgTrkPt2025 = (TH1F*)gDirectory->Get("tauLdgTrkPt2025");
dtauLdgTrkPt2025->SetMarkerStyle(21);
dtauLdgTrkPt2025->SetMarkerColor(1);
dtauLdgTrkPt2025->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauLdgTrkPt2025 = (TH1F*)gDirectory->Get("tauLdgTrkPt2025");
mtauLdgTrkPt2025->SetLineColor(2);
mtauLdgTrkPt2025->Scale(dtauLdgTrkPt2025->GetEntries()/mtauLdgTrkPt2025->GetEntries());
mtauLdgTrkPt2025->Draw();
dtauLdgTrkPt2025->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauLdgTrkPt2025->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauLdgTrkPt2025->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauLdgTrkPt2025.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDAEle2025 = (TH1F*)gDirectory->Get("tauDAEle2025");
dtauDAEle2025->SetMarkerStyle(21);
dtauDAEle2025->SetMarkerColor(1);
dtauDAEle2025->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDAEle2025 = (TH1F*)gDirectory->Get("tauDAEle2025");
mtauDAEle2025->SetLineColor(2);
mtauDAEle2025->Scale(dtauDAEle2025->GetEntries()/mtauDAEle2025->GetEntries());
mtauDAEle2025->Draw();
dtauDAEle2025->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDAEle2025->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDAEle2025->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDAEle2025.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDAMu2025 = (TH1F*)gDirectory->Get("tauDAMu2025");
dtauDAMu2025->SetMarkerStyle(21);
dtauDAMu2025->SetMarkerColor(1);
dtauDAMu2025->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDAMu2025 = (TH1F*)gDirectory->Get("tauDAMu2025");
mtauDAMu2025->SetLineColor(2);
mtauDAMu2025->Scale(dtauDAMu2025->GetEntries()/mtauDAMu2025->GetEntries());
mtauDAMu2025->Draw();
dtauDAMu2025->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDAMu2025->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDAMu2025->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDAMu2025.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDEcalIso2025 = (TH1F*)gDirectory->Get("tauDEcalIso2025");
dtauDEcalIso2025->SetMarkerStyle(21);
dtauDEcalIso2025->SetMarkerColor(1);
dtauDEcalIso2025->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDEcalIso2025 = (TH1F*)gDirectory->Get("tauDEcalIso2025");
mtauDEcalIso2025->SetLineColor(2);
mtauDEcalIso2025->Scale(dtauDEcalIso2025->GetEntries()/mtauDEcalIso2025->GetEntries());
mtauDEcalIso2025->Draw();
dtauDEcalIso2025->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDEcalIso2025->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDEcalIso2025->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDEcalIso2025.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDLdgPionIso2025 = (TH1F*)gDirectory->Get("tauDLdgPionIso2025");
dtauDLdgPionIso2025->SetMarkerStyle(21);
dtauDLdgPionIso2025->SetMarkerColor(1);
dtauDLdgPionIso2025->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDLdgPionIso2025 = (TH1F*)gDirectory->Get("tauDLdgPionIso2025");
mtauDLdgPionIso2025->SetLineColor(2);
mtauDLdgPionIso2025->Scale(dtauDLdgPionIso2025->GetEntries()/mtauDLdgPionIso2025->GetEntries());
mtauDLdgPionIso2025->Draw();
dtauDLdgPionIso2025->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDLdgPionIso2025->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDLdgPionIso2025->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDLdgPionIso2025.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDLdgPionPt2025 = (TH1F*)gDirectory->Get("tauDLdgPionPt2025");
dtauDLdgPionPt2025->SetMarkerStyle(21);
dtauDLdgPionPt2025->SetMarkerColor(1);
dtauDLdgPionPt2025->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDLdgPionPt2025 = (TH1F*)gDirectory->Get("tauDLdgPionPt2025");
mtauDLdgPionPt2025->SetLineColor(2);
mtauDLdgPionPt2025->Scale(dtauDLdgPionPt2025->GetEntries()/mtauDLdgPionPt2025->GetEntries());
dtauDLdgPionPt2025->Draw("PE1");
mtauDLdgPionPt2025->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDLdgPionPt2025->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDLdgPionPt2025->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDLdgPionPt2025.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDLdgTrkFind2025 = (TH1F*)gDirectory->Get("tauDLdgTrkFind2025");
dtauDLdgTrkFind2025->SetMarkerStyle(21);
dtauDLdgTrkFind2025->SetMarkerColor(1);
dtauDLdgTrkFind2025->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDLdgTrkFind2025 = (TH1F*)gDirectory->Get("tauDLdgTrkFind2025");
mtauDLdgTrkFind2025->SetLineColor(2);
mtauDLdgTrkFind2025->Scale(dtauDLdgTrkFind2025->GetEntries()/mtauDLdgTrkFind2025->GetEntries());
mtauDLdgTrkFind2025->Draw();
dtauDLdgTrkFind2025->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDLdgTrkFind2025->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDLdgTrkFind2025->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDLdgTrkFind2025.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDLdgTrkPt2025 = (TH1F*)gDirectory->Get("tauDLdgTrkPt2025");
dtauDLdgTrkPt2025->SetMarkerStyle(21);
dtauDLdgTrkPt2025->SetMarkerColor(1);
dtauDLdgTrkPt2025->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDLdgTrkPt2025 = (TH1F*)gDirectory->Get("tauDLdgTrkPt2025");
mtauDLdgTrkPt2025->SetLineColor(2);
mtauDLdgTrkPt2025->Scale(dtauDLdgTrkPt2025->GetEntries()/mtauDLdgTrkPt2025->GetEntries());
dtauDLdgTrkPt2025->Draw("PE1");
mtauDLdgTrkPt2025->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDLdgTrkPt2025->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDLdgTrkPt2025->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDLdgTrkPt2025.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDTaNC2025 = (TH1F*)gDirectory->Get("tauDTaNC2025");
dtauDTaNC2025->SetMarkerStyle(21);
dtauDTaNC2025->SetMarkerColor(1);
dtauDTaNC2025->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDTaNC2025 = (TH1F*)gDirectory->Get("tauDTaNC2025");
mtauDTaNC2025->SetLineColor(2);
mtauDTaNC2025->Scale(dtauDTaNC2025->GetEntries()/mtauDTaNC2025->GetEntries());
dtauDTaNC2025->Draw("PE1");
mtauDTaNC2025->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDTaNC2025->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDTaNC2025->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDTaNC2025.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDTrkIso2025 = (TH1F*)gDirectory->Get("tauDTrkIso2025");
dtauDTrkIso2025->SetMarkerStyle(21);
dtauDTrkIso2025->SetMarkerColor(1);
dtauDTrkIso2025->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDTrkIso2025 = (TH1F*)gDirectory->Get("tauDTrkIso2025");
mtauDTrkIso2025->SetLineColor(2);
mtauDTrkIso2025->Scale(dtauDTrkIso2025->GetEntries()/mtauDTrkIso2025->GetEntries());
mtauDTrkIso2025->Draw();
dtauDTrkIso2025->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDTrkIso2025->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDTrkIso2025->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDTrkIso2025.gif");




//// *******   TAU  2530


// TFile *DT = TFile::Open("Data.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
// DT -> cd ("analyzeBasicPat");
// TH1F *dtauN2530 = (TH1F*)gDirectory->Get("tauN2530");
// dtauN2530->SetMarkerStyle(21);
// dtauN2530->SetMarkerColor(1);
// dtauN2530->SetMarkerSize(0.7);
// TFile *MC = TFile::Open("MC.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
// MC -> cd ("analyzeBasicPat");
// TH1F *mtauN2530 = (TH1F*)gDirectory->Get("tauN2530");
// mtauN2530->SetLineColor(2);
// mtauN2530->Scale(dtauN2530->GetEntries()/mtauN2530->GetEntries());
// dtauN2530->Draw("PE1");
// mtauN2530->Draw("sames");
// Canv->Update();
// TPaveStats *sm = (TPaveStats*)mtauN2530->GetListOfFunctions()->FindObject("stats");
// TPaveStats *s = (TPaveStats*)dtauN2530->GetListOfFunctions()->FindObject("stats");
// sm->AddText("mc");
// sm->SetTextColor(2);
// s->SetX1NDC(0.55);
// s->SetX2NDC (0.75);
// s->SetTextColor(1);
// s->AddText("data");
// s->DrawClone();
// sm->DrawClone(); 
// Canv->SaveAs("./DataMC/tauN2530.gif");


// TFile *DT = TFile::Open("Data.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
// DT -> cd ("analyzeBasicPat");
// TH1F *dtauPt2530 = (TH1F*)gDirectory->Get("tauPt2530");
// dtauPt2530->SetMarkerStyle(21);
// dtauPt2530->SetMarkerColor(1);
// dtauPt2530->SetMarkerSize(0.7);
// TFile *MC = TFile::Open("MC.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
// MC -> cd ("analyzeBasicPat");
// TH1F *mtauPt2530 = (TH1F*)gDirectory->Get("tauPt2530");
// mtauPt2530->SetLineColor(2);
// mtauPt2530->Scale(dtauPt2530->GetEntries()/mtauPt2530->GetEntries());
// mtauPt2530->Draw();
// dtauPt2530->Draw("PE1sames");
// Canv->Update();
// TPaveStats *sm = (TPaveStats*)mtauPt2530->GetListOfFunctions()->FindObject("stats");
// TPaveStats *s = (TPaveStats*)dtauPt2530->GetListOfFunctions()->FindObject("stats");
// sm->AddText("mc");
// sm->SetTextColor(2);
// s->SetX1NDC(0.55);
// s->SetX2NDC (0.75);
// s->SetTextColor(1);
// s->AddText("data");
// s->DrawClone();
// sm->DrawClone(); 
// Canv->SaveAs("./DataMC/tauPt2530.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauEta2530 = (TH1F*)gDirectory->Get("tauEta2530");
dtauEta2530->SetMarkerStyle(21);
dtauEta2530->SetMarkerColor(1);
dtauEta2530->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauEta2530 = (TH1F*)gDirectory->Get("tauEta2530");
mtauEta2530->SetLineColor(2);
mtauEta2530->Scale(dtauEta2530->GetEntries()/mtauEta2530->GetEntries());
dtauEta2530->Draw("PE1");
mtauEta2530->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauEta2530->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauEta2530->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauEta2530.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauPhi2530 = (TH1F*)gDirectory->Get("tauPhi2530");
dtauPhi2530->SetMarkerStyle(21);
dtauPhi2530->SetMarkerColor(1);
dtauPhi2530->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauPhi2530 = (TH1F*)gDirectory->Get("tauPhi2530");
mtauPhi2530->SetLineColor(2);
mtauPhi2530->Scale(dtauPhi2530->GetEntries()/mtauPhi2530->GetEntries());
dtauPhi2530->Draw("PE1");
mtauPhi2530->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauPhi2530->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauPhi2530->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauPhi2530.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauCharge2530 = (TH1F*)gDirectory->Get("tauCharge2530");
dtauCharge2530->SetMarkerStyle(21);
dtauCharge2530->SetMarkerColor(1);
dtauCharge2530->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauCharge2530 = (TH1F*)gDirectory->Get("tauCharge2530");
mtauCharge2530->SetLineColor(2);
mtauCharge2530->Scale(dtauCharge2530->GetEntries()/mtauCharge2530->GetEntries());
dtauCharge2530->Draw("PE1");
mtauCharge2530->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauCharge2530->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauCharge2530->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauCharge2530.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauProngs2530 = (TH1F*)gDirectory->Get("tauProngs2530");
dtauProngs2530->SetMarkerStyle(21);
dtauProngs2530->SetMarkerColor(1);
dtauProngs2530->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauProngs2530 = (TH1F*)gDirectory->Get("tauProngs2530");
mtauProngs2530->SetLineColor(2);
mtauProngs2530->Scale(dtauProngs2530->GetEntries()/mtauProngs2530->GetEntries());
mtauProngs2530->Draw();
dtauProngs2530->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauProngs2530->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauProngs2530->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauProngs2530.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauLdgTrkPt2530 = (TH1F*)gDirectory->Get("tauLdgTrkPt2530");
dtauLdgTrkPt2530->SetMarkerStyle(21);
dtauLdgTrkPt2530->SetMarkerColor(1);
dtauLdgTrkPt2530->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauLdgTrkPt2530 = (TH1F*)gDirectory->Get("tauLdgTrkPt2530");
mtauLdgTrkPt2530->SetLineColor(2);
mtauLdgTrkPt2530->Scale(dtauLdgTrkPt2530->GetEntries()/mtauLdgTrkPt2530->GetEntries());
mtauLdgTrkPt2530->Draw();
dtauLdgTrkPt2530->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauLdgTrkPt2530->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauLdgTrkPt2530->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauLdgTrkPt2530.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDAEle2530 = (TH1F*)gDirectory->Get("tauDAEle2530");
dtauDAEle2530->SetMarkerStyle(21);
dtauDAEle2530->SetMarkerColor(1);
dtauDAEle2530->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDAEle2530 = (TH1F*)gDirectory->Get("tauDAEle2530");
mtauDAEle2530->SetLineColor(2);
mtauDAEle2530->Scale(dtauDAEle2530->GetEntries()/mtauDAEle2530->GetEntries());
dtauDAEle2530->Draw("PE1");
mtauDAEle2530->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDAEle2530->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDAEle2530->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDAEle2530.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDAMu2530 = (TH1F*)gDirectory->Get("tauDAMu2530");
dtauDAMu2530->SetMarkerStyle(21);
dtauDAMu2530->SetMarkerColor(1);
dtauDAMu2530->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDAMu2530 = (TH1F*)gDirectory->Get("tauDAMu2530");
mtauDAMu2530->SetLineColor(2);
mtauDAMu2530->Scale(dtauDAMu2530->GetEntries()/mtauDAMu2530->GetEntries());
dtauDAMu2530->Draw("PE1");
mtauDAMu2530->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDAMu2530->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDAMu2530->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDAMu2530.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDEcalIso2530 = (TH1F*)gDirectory->Get("tauDEcalIso2530");
dtauDEcalIso2530->SetMarkerStyle(21);
dtauDEcalIso2530->SetMarkerColor(1);
dtauDEcalIso2530->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDEcalIso2530 = (TH1F*)gDirectory->Get("tauDEcalIso2530");
mtauDEcalIso2530->SetLineColor(2);
mtauDEcalIso2530->Scale(dtauDEcalIso2530->GetEntries()/mtauDEcalIso2530->GetEntries());
dtauDEcalIso2530->Draw("PE1");
mtauDEcalIso2530->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDEcalIso2530->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDEcalIso2530->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDEcalIso2530.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDLdgPionIso2530 = (TH1F*)gDirectory->Get("tauDLdgPionIso2530");
dtauDLdgPionIso2530->SetMarkerStyle(21);
dtauDLdgPionIso2530->SetMarkerColor(1);
dtauDLdgPionIso2530->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDLdgPionIso2530 = (TH1F*)gDirectory->Get("tauDLdgPionIso2530");
mtauDLdgPionIso2530->SetLineColor(2);
mtauDLdgPionIso2530->Scale(dtauDLdgPionIso2530->GetEntries()/mtauDLdgPionIso2530->GetEntries());
dtauDLdgPionIso2530->Draw("PE1");
mtauDLdgPionIso2530->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDLdgPionIso2530->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDLdgPionIso2530->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDLdgPionIso2530.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDLdgPionPt2530 = (TH1F*)gDirectory->Get("tauDLdgPionPt2530");
dtauDLdgPionPt2530->SetMarkerStyle(21);
dtauDLdgPionPt2530->SetMarkerColor(1);
dtauDLdgPionPt2530->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDLdgPionPt2530 = (TH1F*)gDirectory->Get("tauDLdgPionPt2530");
mtauDLdgPionPt2530->SetLineColor(2);
mtauDLdgPionPt2530->Scale(dtauDLdgPionPt2530->GetEntries()/mtauDLdgPionPt2530->GetEntries());
dtauDLdgPionPt2530->Draw("PE1");
mtauDLdgPionPt2530->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDLdgPionPt2530->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDLdgPionPt2530->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDLdgPionPt2530.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDLdgTrkFind2530 = (TH1F*)gDirectory->Get("tauDLdgTrkFind2530");
dtauDLdgTrkFind2530->SetMarkerStyle(21);
dtauDLdgTrkFind2530->SetMarkerColor(1);
dtauDLdgTrkFind2530->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDLdgTrkFind2530 = (TH1F*)gDirectory->Get("tauDLdgTrkFind2530");
mtauDLdgTrkFind2530->SetLineColor(2);
mtauDLdgTrkFind2530->Scale(dtauDLdgTrkFind2530->GetEntries()/mtauDLdgTrkFind2530->GetEntries());
dtauDLdgTrkFind2530->Draw("PE1");
mtauDLdgTrkFind2530->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDLdgTrkFind2530->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDLdgTrkFind2530->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDLdgTrkFind2530.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDLdgTrkPt2530 = (TH1F*)gDirectory->Get("tauDLdgTrkPt2530");
dtauDLdgTrkPt2530->SetMarkerStyle(21);
dtauDLdgTrkPt2530->SetMarkerColor(1);
dtauDLdgTrkPt2530->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDLdgTrkPt2530 = (TH1F*)gDirectory->Get("tauDLdgTrkPt2530");
mtauDLdgTrkPt2530->SetLineColor(2);
mtauDLdgTrkPt2530->Scale(dtauDLdgTrkPt2530->GetEntries()/mtauDLdgTrkPt2530->GetEntries());
dtauDLdgTrkPt2530->Draw("PE1");
mtauDLdgTrkPt2530->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDLdgTrkPt2530->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDLdgTrkPt2530->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDLdgTrkPt2530.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDTaNC2530 = (TH1F*)gDirectory->Get("tauDTaNC2530");
dtauDTaNC2530->SetMarkerStyle(21);
dtauDTaNC2530->SetMarkerColor(1);
dtauDTaNC2530->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDTaNC2530 = (TH1F*)gDirectory->Get("tauDTaNC2530");
mtauDTaNC2530->SetLineColor(2);
mtauDTaNC2530->Scale(dtauDTaNC2530->GetEntries()/mtauDTaNC2530->GetEntries());
dtauDTaNC2530->Draw("PE1");
mtauDTaNC2530->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDTaNC2530->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDTaNC2530->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDTaNC2530.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDTrkIso2530 = (TH1F*)gDirectory->Get("tauDTrkIso2530");
dtauDTrkIso2530->SetMarkerStyle(21);
dtauDTrkIso2530->SetMarkerColor(1);
dtauDTrkIso2530->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDTrkIso2530 = (TH1F*)gDirectory->Get("tauDTrkIso2530");
mtauDTrkIso2530->SetLineColor(2);
mtauDTrkIso2530->Scale(dtauDTrkIso2530->GetEntries()/mtauDTrkIso2530->GetEntries());
dtauDTrkIso2530->Draw("PE1");
mtauDTrkIso2530->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDTrkIso2530->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDTrkIso2530->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDTrkIso2530.gif");




//// *******   TAU 30ON


// TFile *DT = TFile::Open("Data.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
// DT -> cd ("analyzeBasicPat");
// TH1F *dtauN30ON = (TH1F*)gDirectory->Get("tauN30ON");
// dtauN30ON->SetMarkerStyle(21);
// dtauN30ON->SetMarkerColor(1);
// dtauN30ON->SetMarkerSize(0.7);
// TFile *MC = TFile::Open("MC.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
// MC -> cd ("analyzeBasicPat");
// TH1F *mtauN30ON = (TH1F*)gDirectory->Get("tauN30ON");
// mtauN30ON->SetLineColor(2);
// mtauN30ON->Scale(dtauN30ON->GetEntries()/mtauN30ON->GetEntries());
// dtauN30ON->Draw("PE1");
// mtauN30ON->Draw("sames");
// Canv->Update();
// TPaveStats *sm = (TPaveStats*)mtauN30ON->GetListOfFunctions()->FindObject("stats");
// TPaveStats *s = (TPaveStats*)dtauN30ON->GetListOfFunctions()->FindObject("stats");
// sm->AddText("mc");
// sm->SetTextColor(2);
// s->SetX1NDC(0.55);
// s->SetX2NDC (0.75);
// s->SetTextColor(1);
// s->AddText("data");
// s->DrawClone();
// sm->DrawClone(); 
// Canv->SaveAs("./DataMC/tauN.gif");


// TFile *DT = TFile::Open("Data.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
// DT -> cd ("analyzeBasicPat");
// TH1F *dtauPt30ON = (TH1F*)gDirectory->Get("tauPt30ON");
// dtauPt30ON->SetMarkerStyle(21);
// dtauPt30ON->SetMarkerColor(1);
// dtauPt30ON->SetMarkerSize(0.7);
// TFile *MC = TFile::Open("MC.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
// MC -> cd ("analyzeBasicPat");
// TH1F *mtauPt30ON = (TH1F*)gDirectory->Get("tauPt30ON");
// mtauPt30ON->SetLineColor(2);
// mtauPt30ON->Scale(dtauPt30ON->GetEntries()/mtauPt30ON->GetEntries());
// mtauPt30ON->Draw();
// dtauPt30ON->Draw("PE1sames");
// Canv->Update();
// TPaveStats *sm = (TPaveStats*)mtauPt30ON->GetListOfFunctions()->FindObject("stats");
// TPaveStats *s = (TPaveStats*)dtauPt30ON->GetListOfFunctions()->FindObject("stats");
// sm->AddText("mc");
// sm->SetTextColor(2);
// s->SetX1NDC(0.55);
// s->SetX2NDC (0.75);
// s->SetTextColor(1);
// s->AddText("data");
// s->DrawClone();
// sm->DrawClone(); 
// Canv->SaveAs("./DataMC/tauPt30ON.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauEta30ON = (TH1F*)gDirectory->Get("tauEta30ON");
dtauEta30ON->SetMarkerStyle(21);
dtauEta30ON->SetMarkerColor(1);
dtauEta30ON->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauEta30ON = (TH1F*)gDirectory->Get("tauEta30ON");
mtauEta30ON->SetLineColor(2);
mtauEta30ON->Scale(dtauEta30ON->GetEntries()/mtauEta30ON->GetEntries());
dtauEta30ON->Draw("PE1");
mtauEta30ON->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauEta30ON->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauEta30ON->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauEta30ON.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauPhi30ON = (TH1F*)gDirectory->Get("tauPhi30ON");
dtauPhi30ON->SetMarkerStyle(21);
dtauPhi30ON->SetMarkerColor(1);
dtauPhi30ON->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauPhi30ON = (TH1F*)gDirectory->Get("tauPhi30ON");
mtauPhi30ON->SetLineColor(2);
mtauPhi30ON->Scale(dtauPhi30ON->GetEntries()/mtauPhi30ON->GetEntries());
dtauPhi30ON->Draw("PE1");
mtauPhi30ON->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauPhi30ON->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauPhi30ON->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauPhi30ON.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauCharge30ON = (TH1F*)gDirectory->Get("tauCharge30ON");
dtauCharge30ON->SetMarkerStyle(21);
dtauCharge30ON->SetMarkerColor(1);
dtauCharge30ON->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauCharge30ON = (TH1F*)gDirectory->Get("tauCharge30ON");
mtauCharge30ON->SetLineColor(2);
mtauCharge30ON->Scale(dtauCharge30ON->GetEntries()/mtauCharge30ON->GetEntries());
dtauCharge30ON->Draw("PE1");
mtauCharge30ON->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauCharge30ON->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauCharge30ON->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauCharge30ON.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauProngs30ON = (TH1F*)gDirectory->Get("tauProngs30ON");
dtauProngs30ON->SetMarkerStyle(21);
dtauProngs30ON->SetMarkerColor(1);
dtauProngs30ON->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauProngs30ON = (TH1F*)gDirectory->Get("tauProngs30ON");
mtauProngs30ON->SetLineColor(2);
mtauProngs30ON->Scale(dtauProngs30ON->GetEntries()/mtauProngs30ON->GetEntries());
dtauProngs30ON->Draw("PE1");
mtauProngs30ON->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauProngs30ON->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauProngs30ON->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauProngs30ON.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauLdgTrkPt30ON = (TH1F*)gDirectory->Get("tauLdgTrkPt30ON");
dtauLdgTrkPt30ON->SetMarkerStyle(21);
dtauLdgTrkPt30ON->SetMarkerColor(1);
dtauLdgTrkPt30ON->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauLdgTrkPt30ON = (TH1F*)gDirectory->Get("tauLdgTrkPt30ON");
mtauLdgTrkPt30ON->SetLineColor(2);
mtauLdgTrkPt30ON->Scale(dtauLdgTrkPt30ON->GetEntries()/mtauLdgTrkPt30ON->GetEntries());
mtauLdgTrkPt30ON->Draw();
dtauLdgTrkPt30ON->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauLdgTrkPt30ON->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauLdgTrkPt30ON->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauLdgTrkPt30ON.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDAEle30ON = (TH1F*)gDirectory->Get("tauDAEle30ON");
dtauDAEle30ON->SetMarkerStyle(21);
dtauDAEle30ON->SetMarkerColor(1);
dtauDAEle30ON->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDAEle30ON = (TH1F*)gDirectory->Get("tauDAEle30ON");
mtauDAEle30ON->SetLineColor(2);
mtauDAEle30ON->Scale(dtauDAEle30ON->GetEntries()/mtauDAEle30ON->GetEntries());
dtauDAEle30ON->Draw("PE1");
mtauDAEle30ON->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDAEle30ON->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDAEle30ON->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDAEle30ON.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDAMu30ON = (TH1F*)gDirectory->Get("tauDAMu30ON");
dtauDAMu30ON->SetMarkerStyle(21);
dtauDAMu30ON->SetMarkerColor(1);
dtauDAMu30ON->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDAMu30ON = (TH1F*)gDirectory->Get("tauDAMu30ON");
mtauDAMu30ON->SetLineColor(2);
mtauDAMu30ON->Scale(dtauDAMu30ON->GetEntries()/mtauDAMu30ON->GetEntries());
dtauDAMu30ON->Draw("PE1");
mtauDAMu30ON->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDAMu30ON->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDAMu30ON->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDAMu30ON.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDEcalIso30ON = (TH1F*)gDirectory->Get("tauDEcalIso30ON");
dtauDEcalIso30ON->SetMarkerStyle(21);
dtauDEcalIso30ON->SetMarkerColor(1);
dtauDEcalIso30ON->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDEcalIso30ON = (TH1F*)gDirectory->Get("tauDEcalIso30ON");
mtauDEcalIso30ON->SetLineColor(2);
mtauDEcalIso30ON->Scale(dtauDEcalIso30ON->GetEntries()/mtauDEcalIso30ON->GetEntries());
dtauDEcalIso30ON->Draw("PE1");
mtauDEcalIso30ON->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDEcalIso30ON->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDEcalIso30ON->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDEcalIso30ON.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDLdgPionIso30ON = (TH1F*)gDirectory->Get("tauDLdgPionIso30ON");
dtauDLdgPionIso30ON->SetMarkerStyle(21);
dtauDLdgPionIso30ON->SetMarkerColor(1);
dtauDLdgPionIso30ON->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDLdgPionIso30ON = (TH1F*)gDirectory->Get("tauDLdgPionIso30ON");
mtauDLdgPionIso30ON->SetLineColor(2);
mtauDLdgPionIso30ON->Scale(dtauDLdgPionIso30ON->GetEntries()/mtauDLdgPionIso30ON->GetEntries());
dtauDLdgPionIso30ON->Draw("PE1");
mtauDLdgPionIso30ON->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDLdgPionIso30ON->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDLdgPionIso30ON->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDLdgPionIso30ON.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDLdgPionPt30ON = (TH1F*)gDirectory->Get("tauDLdgPionPt30ON");
dtauDLdgPionPt30ON->SetMarkerStyle(21);
dtauDLdgPionPt30ON->SetMarkerColor(1);
dtauDLdgPionPt30ON->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDLdgPionPt30ON = (TH1F*)gDirectory->Get("tauDLdgPionPt30ON");
mtauDLdgPionPt30ON->SetLineColor(2);
mtauDLdgPionPt30ON->Scale(dtauDLdgPionPt30ON->GetEntries()/mtauDLdgPionPt30ON->GetEntries());
dtauDLdgPionPt30ON->Draw("PE1");
mtauDLdgPionPt30ON->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDLdgPionPt30ON->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDLdgPionPt30ON->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDLdgPionPt30ON.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDLdgTrkFind30ON = (TH1F*)gDirectory->Get("tauDLdgTrkFind30ON");
dtauDLdgTrkFind30ON->SetMarkerStyle(21);
dtauDLdgTrkFind30ON->SetMarkerColor(1);
dtauDLdgTrkFind30ON->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDLdgTrkFind30ON = (TH1F*)gDirectory->Get("tauDLdgTrkFind30ON");
mtauDLdgTrkFind30ON->SetLineColor(2);
mtauDLdgTrkFind30ON->Scale(dtauDLdgTrkFind30ON->GetEntries()/mtauDLdgTrkFind30ON->GetEntries());
dtauDLdgTrkFind30ON->Draw("PE1");
mtauDLdgTrkFind30ON->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDLdgTrkFind30ON->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDLdgTrkFind30ON->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDLdgTrkFind30ON.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDLdgTrkPt30ON = (TH1F*)gDirectory->Get("tauDLdgTrkPt30ON");
dtauDLdgTrkPt30ON->SetMarkerStyle(21);
dtauDLdgTrkPt30ON->SetMarkerColor(1);
dtauDLdgTrkPt30ON->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDLdgTrkPt30ON = (TH1F*)gDirectory->Get("tauDLdgTrkPt30ON");
mtauDLdgTrkPt30ON->SetLineColor(2);
mtauDLdgTrkPt30ON->Scale(dtauDLdgTrkPt30ON->GetEntries()/mtauDLdgTrkPt30ON->GetEntries());
dtauDLdgTrkPt30ON->Draw("PE1");
mtauDLdgTrkPt30ON->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDLdgTrkPt30ON->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDLdgTrkPt30ON->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDLdgTrkPt30ON.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDTaNC30ON = (TH1F*)gDirectory->Get("tauDTaNC30ON");
dtauDTaNC30ON->SetMarkerStyle(21);
dtauDTaNC30ON->SetMarkerColor(1);
dtauDTaNC30ON->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDTaNC30ON = (TH1F*)gDirectory->Get("tauDTaNC30ON");
mtauDTaNC30ON->SetLineColor(2);
mtauDTaNC30ON->Scale(dtauDTaNC30ON->GetEntries()/mtauDTaNC30ON->GetEntries());
 mtauDTaNC30ON->Draw();
dtauDTaNC30ON->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDTaNC30ON->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDTaNC30ON->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDTaNC30ON.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dtauDTrkIso30ON = (TH1F*)gDirectory->Get("tauDTrkIso30ON");
dtauDTrkIso30ON->SetMarkerStyle(21);
dtauDTrkIso30ON->SetMarkerColor(1);
dtauDTrkIso30ON->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mtauDTrkIso30ON = (TH1F*)gDirectory->Get("tauDTrkIso30ON");
mtauDTrkIso30ON->SetLineColor(2);
mtauDTrkIso30ON->Scale(dtauDTrkIso30ON->GetEntries()/mtauDTrkIso30ON->GetEntries());
dtauDTrkIso30ON->Draw("PE1");
mtauDTrkIso30ON->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mtauDTrkIso30ON->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dtauDTrkIso30ON->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/tauDTrkIso30ON.gif");













//// DI TAU MU + TAU-JET


// TFile *DT = TFile::Open("Data.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
// DT -> cd ("analyzeBasicPat");
// TH1F *dpair = (TH1F*)gDirectory->Get("pair");
// dpair->SetMarkerStyle(21);
// dpair->SetMarkerColor(1);
// dpair->SetMarkerSize(0.7);
// TFile *MC = TFile::Open("MC.root");
// TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
// MC -> cd ("analyzeBasicPat");
// TH1F *mpair = (TH1F*)gDirectory->Get("tau");
// mpair->SetLineColor(2);
// mpair->Scale(dpair->GetEntries()/mpair->GetEntries());
// mpair->Draw();
// dpair->Draw("PE1sames");
// Canv->Update();
// TPaveStats *sm = (TPaveStats*)mpair->GetListOfFunctions()->FindObject("stats");
// TPaveStats *s = (TPaveStats*)dpair->GetListOfFunctions()->FindObject("stats");
// sm->AddText("mc");
// sm->SetTextColor(2);
// s->SetX1NDC(0.55);
// s->SetX2NDC (0.75);
// s->SetTextColor(1);
// s->AddText("data");
// s->DrawClone();
// sm->DrawClone(); 
// Canv->SaveAs("./DataMC/pair.gif");




TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dpairDPhi = (TH1F*)gDirectory->Get("pairDPhi");
dpairDPhi->SetMarkerStyle(21);
dpairDPhi->SetMarkerColor(1);
dpairDPhi->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mpairDPhi = (TH1F*)gDirectory->Get("pairDPhi");
mpairDPhi->SetLineColor(2);
mpairDPhi->Scale(dpairDPhi->GetEntries()/mpairDPhi->GetEntries());
dpairDPhi->Draw("PE1");
mpairDPhi->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mpairDPhi->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dpairDPhi->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/pairDPhi.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dpairVisMass = (TH1F*)gDirectory->Get("pairVisMass");
dpairVisMass->SetMarkerStyle(21);
dpairVisMass->SetMarkerColor(1);
dpairVisMass->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mpairVisMass = (TH1F*)gDirectory->Get("pairVisMass");
mpairVisMass->SetLineColor(2);
mpairVisMass->Scale(dpairVisMass->GetEntries()/mpairVisMass->GetEntries());
dpairVisMass->Draw("PE1");
mpairVisMass->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mpairVisMass->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dpairVisMass->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/pairVisMass.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dpairDR = (TH1F*)gDirectory->Get("pairDR");
dpairDR->SetMarkerStyle(21);
dpairDR->SetMarkerColor(1);
dpairDR->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mpairDR = (TH1F*)gDirectory->Get("pairDR");
mpairDR->SetLineColor(2);
mpairDR->Scale(dpairDR->GetEntries()/mpairDR->GetEntries());
mpairDR->Draw();
dpairDR->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mpairDR->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dpairDR->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/pairDR.gif");

TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dpairCharge = (TH1F*)gDirectory->Get("pairCharge");
dpairCharge->SetMarkerStyle(21);
dpairCharge->SetMarkerColor(1);
dpairCharge->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mpairCharge = (TH1F*)gDirectory->Get("pairCharge");
mpairCharge->SetLineColor(2);
mpairCharge->Scale(dpairCharge->GetEntries()/mpairCharge->GetEntries());
mpairCharge->Draw();
dpairCharge->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mpairCharge->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dpairCharge->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/pairCharge.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dPzetaVar = (TH1F*)gDirectory->Get("PzetaVar");
dPzetaVar->SetMarkerStyle(21);
dPzetaVar->SetMarkerColor(1);
dPzetaVar->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mPzetaVar = (TH1F*)gDirectory->Get("PzetaVar");
mPzetaVar->SetLineColor(2);
mPzetaVar->Scale(dPzetaVar->GetEntries()/mPzetaVar->GetEntries());
mPzetaVar->Draw();
dPzetaVar->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mPzetaVar->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dPzetaVar->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/PzetaVar.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dpairPt = (TH1F*)gDirectory->Get("diTauPt");
dpairPt->SetMarkerStyle(21);
dpairPt->SetMarkerColor(1);
dpairPt->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mpairPt = (TH1F*)gDirectory->Get("diTauPt");
mpairPt->SetLineColor(2);
mpairPt->Scale(dpairPt->GetEntries()/mpairPt->GetEntries());
dpairPt->Draw("PE1");
mpairPt->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mpairPt->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dpairPt->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/pairPt.gif");




TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dpairEta = (TH1F*)gDirectory->Get("diTauEta");
dpairEta->SetMarkerStyle(21);
dpairEta->SetMarkerColor(1);
dpairEta->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mpairEta = (TH1F*)gDirectory->Get("diTauEta");
mpairEta->SetLineColor(2);
mpairEta->Scale(dpairEta->GetEntries()/mpairEta->GetEntries());
dpairEta->Draw("PE1");
mpairEta->Draw("sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mpairEta->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dpairEta->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/pairEta.gif");


TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dpairPhi = (TH1F*)gDirectory->Get("diTauPhi");
dpairPhi->SetMarkerStyle(21);
dpairPhi->SetMarkerColor(1);
dpairPhi->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mpairPhi = (TH1F*)gDirectory->Get("diTauPhi");
mpairPhi->SetLineColor(2);
mpairPhi->Scale(dpairPhi->GetEntries()/mpairPhi->GetEntries());
mpairPhi->Draw();
dpairPhi->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mpairPhi->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dpairPhi->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/pairPhi.gif");



TFile *DT = TFile::Open("Data.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)DT->GetDirectory("analyzeBasicPat");
DT -> cd ("analyzeBasicPat");
TH1F *dmuMETMT = (TH1F*)gDirectory->Get("muMETMT");
dmuMETMT->SetMarkerStyle(21);
dmuMETMT->SetMarkerColor(1);
dmuMETMT->SetMarkerSize(0.7);
TFile *MC = TFile::Open("MC.root");
TDirectoryFile * analyzeBasicPat = (TDirectoryFile*)MC->GetDirectory("analyzeBasicPat");
MC -> cd ("analyzeBasicPat");
TH1F *mmuMETMT = (TH1F*)gDirectory->Get("muMETMT");
mmuMETMT->SetLineColor(2);
mmuMETMT->Scale(dmuMETMT->GetEntries()/mmuMETMT->GetEntries());
mmuMETMT->Draw();
dmuMETMT->Draw("PE1sames");
Canv->Update();
TPaveStats *sm = (TPaveStats*)mmuMETMT->GetListOfFunctions()->FindObject("stats");
TPaveStats *s = (TPaveStats*)dmuMETMT->GetListOfFunctions()->FindObject("stats");
sm->AddText("mc");
sm->SetTextColor(2);
s->SetX1NDC(0.55);
s->SetX2NDC (0.75);
s->SetTextColor(1);
s->AddText("data");
s->DrawClone();
sm->DrawClone(); 
Canv->SaveAs("./DataMC/muMETMT.gif");








MC->Close();
delete MC;
    
// if (Results)
//   Results->Close();

gROOT->Reset();
gROOT->Clear();
delete Canv;   
}
