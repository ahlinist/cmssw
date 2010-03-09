#include <iostream>
#include "tdrstyle.C"

using namespace std;


// remarks:
// in nTuple producer ic and sc algos were skipped...
// so I did the same here...
// 
// 
// 



Double_t getMaximum(TH1*, TH1*);
TCanvas makePlot(TCanvas, TH1D, TH1D, int, char, char);

void plot_basicvars() {

//   TString dataset="Jan23-900-JetCorr";
  TString dataset="Jan23-900-UnCorr";


  setTDRStyle();
  gROOT->ForceStyle();
  //tdr style, remember to include the file tdrStyle
//   tdrStyle->SetOptStat(111111);
//  tdrStyle->SetOptStat(111);
  tdrStyle->SetOptStat(0);

  gStyle->SetPalette(1);

  // to use if one does not use tdr style...
//   gROOT->SetStyle("Plain");
//   gStyle->SetPalette(1);
//   gStyle->SetOptStat(111111);
//   gStyle->SetStatStyle(3001);


  char *legheader="Data,  #sqrt{s} = 900 GeV   CMS Preliminary";

  cout<<getMaximum(variousEff, variousEff)<<endl;


  
  ptall->SetTitle(dataset);
  mapall->SetTitle(dataset);
  map->SetTitle(dataset);
  mapcleaned->SetTitle(dataset);
  pt->SetTitle(dataset);
  ptcleaned->SetTitle(dataset);
  map->SetTitle(dataset);
  mapcleaned->SetTitle(dataset);
  Ebarrel->SetTitle(dataset);
  Ebarrelcleaned->SetTitle(dataset);
  Eendcap->SetTitle(dataset);
  Eendcapcleaned->SetTitle(dataset);
  resemf->SetTitle(dataset);
  fhpd->SetTitle(dataset);
  frbx->SetTitle(dataset);
  n90hits->SetTitle(dataset);

  ak5njets->SetTitle(dataset);
  ak5nalljets->SetTitle(dataset);
  ak5ncleanedjets->SetTitle(dataset);

  ic5njets->SetTitle(dataset);
  ic5nalljets->SetTitle(dataset);
  ic5ncleanedjets->SetTitle(dataset);

//   sc5njets->SetTitle(dataset);
//   sc5nalljets->SetTitle(dataset);
//   sc5ncleanedjets->SetTitle(dataset);

//   kt4njets->SetTitle(dataset);
//   kt4nalljets->SetTitle(dataset);
//   kt4ncleanedjets->SetTitle(dataset);

  ak5nconst->SetTitle(dataset);
  ak5nconstcleaned->SetTitle(dataset);

  ic5nconst->SetTitle(dataset);
  ic5nconstcleaned->SetTitle(dataset);

//   sc5nconst->SetTitle(dataset);
//   sc5nconstcleaned->SetTitle(dataset);

//   kt4nconst->SetTitle(dataset);
//   kt4nconstcleaned->SetTitle(dataset);

//   // -------------------------------------------------
//   // newly added histos...
//   phi->SetTitle(dataset);
//   phicleaned->SetTitle(dataset);
//   eta->SetTitle(dataset);
//   etacleaned->SetTitle(dataset);



  TCanvas *c = new TCanvas("c","",600,600);


  //-----------------------------


//   c->SetLogy(1);
//   ptall->SetYTitle("number of jets / bin");
//   ptall->Draw();
//   c->SaveAs(dataset+"_pt_alljets.gif");
//   pt->SetYTitle("number of jets / bin");
//   pt->Draw();
//   c->SaveAs(dataset+"_pt_alljets_ptgt8.gif");
//   ptcleaned->SetYTitle("number of jets / bin");
//   ptcleaned->Draw();
//   c->SaveAs(dataset+"_pt_allcleanedjets_ptgt8.gif");
//   Ebarrel->SetYTitle("number of jets / bin");
//   Ebarrel->Draw();
//   c->SaveAs(dataset+"_Ebarrel_alljets_ptgt8.gif");
//   Ebarrelcleaned->SetYTitle("number of jets / bin");
//   Ebarrelcleaned->Draw();
//   c->SaveAs(dataset+"_Ebarrel_allcleanedjets_ptgt8.gif");
//   Eendcap->SetYTitle("number of jets / bin");
//   Eendcap->Draw();
//   c->SaveAs(dataset+"_Eendcap_alljets_ptgt8.gif");
//   Eendcapcleaned->SetYTitle("number of jets / bin");
//   Eendcapcleaned->Draw();
//   c->SaveAs(dataset+"_Eendcap_allcleanedjets_ptgt8.gif");

  c->SetLogy(1);
  resemf->Rebin(2);
  resemf->SetYTitle("number of jets / bin");
  resemf->SetLineWidth(2);
  resemf->Draw();
  TLegend *leg = new TLegend(0.35,0.65,0.75,0.85);
  leg->SetFillStyle(4000);
  leg->SetFillColor(kWhite);
  leg->SetBorderSize(0);
//   leg->AddEntry(resemfdijets,"di-jet events","l");
  leg->SetHeader(legheader);
  leg->Draw();
  c->SaveAs(dataset+"_resEMF.gif");
  c->SaveAs(dataset+"_resEMF.eps");

  c->SetLogy(1);
  fhpd->Rebin(2);
  fhpd->SetYTitle("number of jets / bin");
  fhpd->SetLineWidth(2);
  fhpd->Draw();
  TLegend *leg = new TLegend(0.35,0.65,0.75,0.85);
  leg->SetFillStyle(4000);
  leg->SetFillColor(kWhite);
  leg->SetBorderSize(0);
//   leg->AddEntry(resemfdijets,"di-jet events","l");
  leg->SetHeader(legheader);
  leg->Draw();
  c->SaveAs(dataset+"_fhpd.gif");
  c->SaveAs(dataset+"_fhpd.eps");

  c->SetLogy(1);
  frbx->Rebin(2);
  frbx->SetYTitle("number of jets / bin");
  frbx->SetLineWidth(2);
  frbx->Draw();
  TLegend *leg = new TLegend(0.35,0.65,0.75,0.85);
  leg->SetFillStyle(4000);
  leg->SetFillColor(kWhite);
  leg->SetBorderSize(0);
//   leg->AddEntry(resemfdijets,"di-jet events","l");
  leg->SetHeader(legheader);
  leg->Draw();
  c->SaveAs(dataset+"_frbx.gif");
  c->SaveAs(dataset+"_frbx.eps");
  
//   n90hits->SetYTitle("number of jets / bin");
//   n90hits->SetLineWidth(2);
//   n90hits->Draw();
//   c->SaveAs(dataset+"_n90hits_jets_ptgt8.gif");

//   mapall->Draw("colz");
//   c->SaveAs(dataset+"_phiVSeta_jets.gif");
//   map->Draw("colz");
//   c->SaveAs(dataset+"_phiVSeta_jets_ptgt8.gif");
//   c->SaveAs(dataset+"_phiVSeta_jets_ptgt8.eps");

//   mapcleaned->Draw("colz");
//   c->SaveAs(dataset+"_phiVSeta_cleanedjets_ptgt8.gif");
//   c->SaveAs(dataset+"_phiVSeta_cleanedjets_ptgt8.eps");


//   c->Clear();
  
//   int max=ak5njets->GetEntries();


  // ak5 and ic5 njets
  c->SetLogy(1);
  int icakmax=getMaximum(ak5njets,ic5njets);
  ak5njets->SetMaximum(1.5*icakmax);
  ak5njets->SetLineStyle(1);
  ak5njets->SetLineWidth(2);
  ic5njets->SetLineStyle(2);
  ic5njets->SetLineWidth(2);
  ak5njets->SetYTitle("number of jets / bin");
  ak5njets->Draw();
  ic5njets->SetYTitle("number of jets / bin");
  ic5njets->Draw("sames");

  TLegend *leg = new TLegend(0.35,0.65,0.75,0.85);
  leg->SetFillStyle(4000);
  leg->SetFillColor(kWhite);
  leg->SetBorderSize(0);
  leg->AddEntry(ak5njets,"Anti-k_{t}5","l");
  leg->AddEntry(ic5njets,"IterativeCone5","l");
  leg->SetHeader(legheader);
  leg->Draw();
  c->SaveAs(dataset+"_Njets_ak5ic5.gif");
  c->SaveAs(dataset+"_Njets_ak5ic5.eps");

  // ak5 and ic5 njetscleaned
  c->SetLogy(1);
  int icakmax=getMaximum(ak5ncleanedjets,ic5ncleanedjets);
  ak5ncleanedjets->SetMaximum(1.5*icakmax);
  ak5ncleanedjets->SetLineStyle(1);
  ak5ncleanedjets->SetLineWidth(2);
  ic5ncleanedjets->SetLineStyle(2);
  ic5ncleanedjets->SetLineWidth(2);
  ak5ncleanedjets->SetYTitle("number of jets / bin");
  ak5ncleanedjets->Draw();
  ic5ncleanedjets->SetYTitle("number of jets / bin");
  ic5ncleanedjets->Draw("sames");

  TLegend *leg = new TLegend(0.35,0.65,0.75,0.85);
  leg->SetFillStyle(4000);
  leg->SetFillColor(kWhite);
  leg->SetBorderSize(0);
  leg->AddEntry(ak5ncleanedjets,"Anti-k_{t}5","l");
  leg->AddEntry(ic5ncleanedjets,"IterativeCone5","l");
  leg->SetHeader(legheader);
  leg->Draw();
  c->SaveAs(dataset+"_Ncleanedjets_ak5ic5.gif");
  c->SaveAs(dataset+"_Ncleanedjets_ak5ic5.eps");


  // ak5 and ic5 pt
  c->SetLogy(1);
  int icakptmax=getMaximum(pt,ic5pt);
  pt->SetMaximum(1.5*icakptmax);
  pt->SetLineStyle(1);
  pt->SetLineWidth(2);
  ic5pt->SetLineStyle(2);
  ic5pt->SetLineWidth(2);
  pt->SetYTitle("number of jets / bin");
  pt->Draw();
  ic5pt->SetYTitle("number of jets / bin");
  ic5pt->Draw("sames");

  TLegend *leg = new TLegend(0.35,0.65,0.75,0.85);
  leg->SetFillStyle(4000);
  leg->SetFillColor(kWhite);
  leg->SetBorderSize(0);
  leg->AddEntry(pt,"Anti-k_{t}5","l");
  leg->AddEntry(ic5pt,"IterativeCone5","l");
  leg->SetHeader(legheader);
  leg->Draw();
  c->SaveAs(dataset+"_pt_ak5ic5.gif");
  c->SaveAs(dataset+"_pt_ak5ic5.eps");


//  int allmax=ak5nalljets->GetEntries();
//  ak5nalljets->SetMaximum(1.5*allmax);
//  ic5nalljets->SetMaximum(1.5*allmax);
// //  sc5nalljets->SetMaximum(1.5*allmax);
// //  kt4nalljets->SetMaximum(1.5*allmax);
 
//   ak5nalljets->SetLineStyle(1);
//   ak5nalljets->SetLineWidth(2);
//   ic5nalljets->SetLineStyle(2);
//   ic5nalljets->SetLineWidth(2);
// //   sc5nalljets->SetLineStyle(3);
// //   sc5nalljets->SetLineWidth(2);
// //   kt4nalljets->SetLineStyle(4);
// //   kt4nalljets->SetLineWidth(2);

//   ak5nalljets->SetYTitle("number of jets / bin");
//   ak5nalljets->Draw();
//   ic5nalljets->SetYTitle("number of jets / bin");
//   ic5nalljets->Draw("sames");
// //   sc5nalljets->Draw("sames");
// //   kt4nalljets->SetYTitle("number of jets / bin");
// //   kt4nalljets->Draw("sames");
  
//  TLegend *leg = new TLegend(0.35,0.65,0.75,0.85);
//  leg->SetFillStyle(4000);
//  leg->SetFillColor(kWhite);
//  leg->SetBorderSize(0);
//  leg->AddEntry(ak5nalljets,"Anti-k_{t}5","l");
//  leg->AddEntry(ic5nalljets,"IterativeCone5","l");
// //  leg->AddEntry(sc5nalljets,"SisCone5","l");
// //  leg->AddEntry(kt4nalljets,"k_{t}4","l");
//  leg->Draw();
//  c->SaveAs(dataset+"_NjetsComp_jets.gif");


//   int cleanedmax=ak5ncleanedjets->GetEntries();

//   ak5ncleanedjets->SetMaximum(1.5*cleanedmax);
//   ic5ncleanedjets->SetMaximum(1.5*cleanedmax);
// //   sc5ncleanedjets->SetMaximum(1.5*cleanedmax);
// //   kt4ncleanedjets->SetMaximum(1.5*cleanedmax);

//   ak5ncleanedjets->SetLineStyle(1);
//   ak5ncleanedjets->SetLineWidth(2);
//   ic5ncleanedjets->SetLineStyle(2);
//   ic5ncleanedjets->SetLineWidth(2);
// //   sc5ncleanedjets->SetLineStyle(3);
// //   sc5ncleanedjets->SetLineWidth(2);
// //   kt4ncleanedjets->SetLineStyle(4);
// //   kt4ncleanedjets->SetLineWidth(2);

//   ak5ncleanedjets->SetYTitle("number of jets / bin");
//   ak5ncleanedjets->Draw();
//   ic5ncleanedjets->Draw("sames");
// //   sc5ncleanedjets->Draw("sames");
// //   kt4ncleanedjets->SetYTitle("number of jets / bin");
// //   kt4ncleanedjets->Draw("sames");
//   TLegend *leg = new TLegend(0.35,0.65,0.75,0.85);
//   leg->SetFillStyle(4000);
//   leg->SetFillColor(kWhite);
//   leg->SetBorderSize(0);
//   leg->AddEntry(ak5ncleanedjets,"Anti-k_{t}5","l");
//   leg->AddEntry(ic5ncleanedjets,"IterativeCone5","l");
// //   leg->AddEntry(sc5ncleanedjets,"SisCone5","l");
// //   leg->AddEntry(kt4ncleanedjets,"k_{t}4","l");
//   leg->Draw();
//   c->SaveAs(dataset+"_NcleanedjetsComp_jets_ptgt8.gif");


//   //CONSTITUENTS
//   int maxc=ak5nconst->GetEntries();

//   ak5nconst->SetMaximum(1.5*maxc);
//   ic5nconst->SetMaximum(1.5*maxc);
// //   sc5nconst->SetMaximum(1.5*maxc);
// //   kt4nconst->SetMaximum(1.5*maxc);
//   ak5nconst->SetLineStyle(1);
//   ak5nconst->SetLineWidth(2);
//   ic5nconst->SetLineStyle(2);
//   ic5nconst->SetLineWidth(2);
// //   sc5nconst->SetLineStyle(3);
// //   sc5nconst->SetLineWidth(2);
// //   kt4nconst->SetLineStyle(4);
// //   kt4nconst->SetLineWidth(2);

//   c->SetLogy(1);
//   ak5nconst->SetYTitle("number of jets / bin");
//   ak5nconst->Draw();
//   ic5nconst->SetYTitle("number of jets / bin");
//   ic5nconst->Draw("sames");
// //   sc5nconst->Draw("sames");
// //   kt4nconst->SetYTitle("number of jets / bin");
// //   kt4nconst->Draw("sames");
//   TLegend *leg = new TLegend(0.35,0.65,0.75,0.85);
//   leg->SetFillStyle(4000);
//   leg->SetFillColor(kWhite);
//   leg->SetBorderSize(0);
//   leg->AddEntry(ak5nconst,"Anti-k_{t}5","l");
//   leg->AddEntry(ic5nconst,"IterativeCone5","l");
//   //  leg->AddEntry(sc5nconst,"SisCone5","l");
// //   leg->AddEntry(kt4nconst,"k_{t}4","l");
//   leg->Draw();
//   c->SaveAs(dataset+"_NconstComp_jets_ptgt8.gif");
  
//   int cleanedmaxc=ak5nconstcleaned->GetEntries();

//   ak5nconstcleaned->SetMaximum(1.5*cleanedmaxc);
//   ic5nconstcleaned->SetMaximum(1.5*cleanedmaxc);
// //   sc5nconstcleaned->SetMaximum(1.5*cleanedmaxc);
// //   kt4nconstcleaned->SetMaximum(1.5*cleanedmaxc);

//   ak5nconstcleaned->SetLineStyle(1);
//   ak5nconstcleaned->SetLineWidth(2);
//   ic5nconstcleaned->SetLineStyle(2);
//   ic5nconstcleaned->SetLineWidth(2);
// //   sc5nconstcleaned->SetLineStyle(3);
// //   sc5nconstcleaned->SetLineWidth(2);
// //   kt4nconstcleaned->SetLineStyle(4);
// //   kt4nconstcleaned->SetLineWidth(2);

//   ak5nconstcleaned->SetYTitle("number of jets / bin");
//   ak5nconstcleaned->Draw();
//   ic5nconstcleaned->SetYTitle("number of jets / bin");
//   ic5nconstcleaned->Draw("sames");
// //   sc5nconstcleaned->Draw("sames");
// //   kt4nconstcleaned->SetYTitle("number of jets / bin");
// //   kt4nconstcleaned->Draw("sames");
//   TLegend *leg = new TLegend(0.35,0.65,0.75,0.85);
//   leg->SetFillStyle(4000);
//   leg->SetFillColor(kWhite);
//   leg->SetBorderSize(0);
//   leg->AddEntry(ak5nconstcleaned,"Anti-k_{t}5","l");
//   leg->AddEntry(ic5nconstcleaned,"IterativeCone5","l");
// //   leg->AddEntry(sc5nconstcleaned,"SisCone5","l");
// //   leg->AddEntry(kt4nconstcleaned,"k_{t}4","l");
//   leg->Draw();
//   c->SaveAs(dataset+"_NconstcleanedComp_jets_ptgt8.gif");


////--------------------------------



  // -------------------------------------------------
  // newly added histos...
  phi->SetTitle(dataset);
  phicleaned->SetTitle(dataset);

  eta->SetTitle(dataset);
  etacleaned->SetTitle(dataset);


  // phi and phi cleaned
  phi->Rebin(4);
  phicleaned->Rebin(4);
  int phimax=getMaximum(phi, phicleaned);

  c->SetLogy(0);
//   phi->SetMaximum(1.5*phimax);
  phi->SetLineStyle(1);
  phi->SetLineWidth(2);
  phi->SetYTitle("number of jets / bin");
  phi->SetMinimum(0);
//   phi->SetMaximum(220);
  phi->SetMaximum(1.8*phimax);
  phi->Draw();
//   c->SaveAs(dataset+"_phi.gif");
//   phicleaned->SetMaximum(1.5*phimax);
  phicleaned->SetLineStyle(4);
  phicleaned->SetLineWidth(2);
  phicleaned->SetYTitle("number of jets / bin");
  phicleaned->Draw("sames");
  TLegend *leg = new TLegend(0.35,0.65,0.75,0.85);
  leg->SetFillStyle(4000);
  leg->SetFillColor(kWhite);
  leg->SetBorderSize(0);
  leg->AddEntry(phi,"Anti-k_{t}5, uncleaned","l");
  leg->AddEntry(phicleaned,"Anti-k_{t}5, cleaned","l");
  leg->SetHeader(legheader);
  leg->Draw();

  c->SaveAs(dataset+"_phi.eps");
  c->SaveAs(dataset+"_phi.gif");

  // eta and eta cleaned
  eta->Rebin(3);
  etacleaned->Rebin(3);
  int etamax=getMaximum(eta, etacleaned);

  c->SetLogy(0);

  eta->SetLineStyle(1);
  eta->SetLineWidth(2);
//   eta->SetMaximum(350);
  eta->SetMaximum(1.8*etamax);
  eta->SetYTitle("number of jets / bin");
  eta->Draw();
//   c->SaveAs(dataset+"_eta.gif");
  etacleaned->SetLineStyle(4);
  etacleaned->SetLineWidth(2);
  etacleaned->Draw("sames");
  TLegend *leg = new TLegend(0.35,0.65,0.75,0.85);
  leg->SetFillStyle(4000);
  leg->SetFillColor(kWhite);
  leg->SetBorderSize(0);
  leg->AddEntry(phi,"Anti-k_{t}5, uncleaned","l");
  leg->AddEntry(phicleaned,"Anti-k_{t}5, cleaned","l");
  leg->SetHeader(legheader);
  leg->Draw();
  c->SaveAs(dataset+"_eta.eps");
  c->SaveAs(dataset+"_eta.gif");

//   etacleaned->SetLineStyle(1);
//   etacleaned->SetYTitle("number of jets / bin");
//   etacleaned->Draw();
//   c->SaveAs(dataset+"_etacleaned.gif");


  // pt and pt  cleaned

  c->SetLogy(1);

  pt->Rebin(2);
  ptcleaned->Rebin(2);
  double MAX=getMaximum(pt,ptcleaned);
  pt->SetMaximum(1.1*MAX);
  pt->SetLineStyle(1);
  pt->SetLineWidth(2);
  pt->SetYTitle("number of jets / bin");
  pt->Draw();
//   c->SaveAs(dataset+"_pt.gif");
  ptcleaned->SetLineStyle(4);
  ptcleaned->SetLineWidth(2);
  ptcleaned->Draw("sames");
  TLegend *leg = new TLegend(0.35,0.65,0.75,0.85);
  leg->SetFillStyle(4000);
  leg->SetFillColor(kWhite);
  leg->SetBorderSize(0);
  leg->AddEntry(phi,"Anti-k_{t}5, uncleaned","l");
  leg->AddEntry(phicleaned,"Anti-k_{t}5, cleaned","l");
  leg->SetHeader(legheader);
  leg->Draw();
  c->SaveAs(dataset+"_pt.eps");
  c->SaveAs(dataset+"_pt.gif");

//   ptcleaned->SetLineStyle(1);
//   ptcleaned->SetYTitle("number of jets / bin");
//   ptcleaned->Draw();
//   c->SaveAs(dataset+"_ptcleaned.gif");
 


//   // dijetptall1UNcleaned and dijetptall1cleaned
//   c->SetLogy(0);
//   dijetptall1->Rebin(2);
//   dijetptall1cleaned->Rebin(2);
//   double MAX=getMaximum(dijetptall1, dijetptall1cleaned);
//   dijetptall1->SetMaximum(1.1*MAX);
//   dijetptall1->SetLineStyle(1);
//   dijetptall1->SetLineWidth(2);
//   dijetptall1->SetXTitle("jet p_{T} [GeV]");
//   dijetptall1->SetYTitle("number of jets / bin");
//   dijetptall1->Draw();
//   dijetptall1cleaned->SetLineStyle(4);
//   dijetptall1cleaned->SetLineWidth(2);
//   dijetptall1cleaned->Draw("sames");
  
//   TLegend *leg = new TLegend(0.35,0.65,0.75,0.85);
//   leg->SetFillStyle(4000);
//   leg->SetFillColor(kWhite);
//   leg->SetBorderSize(0);
//   leg->SetHeader(legheader);
//   leg->AddEntry(dijetptall1,"Anti-k_{t}5, first jet, uncleaned","l");
//   leg->AddEntry(dijetptall1cleaned,"Anti-k_{t}5, first jet, cleaned","l");
//   leg->Draw();

//   c->SaveAs(dataset+"_dijet_ptall1_and_ptall1cleaned.gif");
//   c->SaveAs(dataset+"_dijet_ptall1_and_ptall1cleaned.eps");


//   // dijetptall2UNcleaned and dijetptall2cleaned
//   c->SetLogy(0);
//   dijetptall2->Rebin(2);
//   dijetptall2cleaned->Rebin(2);
//   double MAX=getMaximum(dijetptall2, dijetptall2cleaned);
//   dijetptall2->SetMaximum(1.1*MAX);
//   dijetptall2->SetLineStyle(1);
//   dijetptall2->SetLineWidth(2);
//   dijetptall2->SetXTitle("jet p_{T}[GeV]");
//   dijetptall2->SetYTitle("number of jets / bin");
//   dijetptall2->Draw();
//   dijetptall2cleaned->SetLineStyle(4);
//   dijetptall2cleaned->SetLineWidth(2);
//   dijetptall2cleaned->Draw("sames");
  
//   TLegend *leg = new TLegend(0.35,0.65,0.75,0.85);
//   leg->SetFillStyle(4000);
//   leg->SetFillColor(kWhite);
//   leg->SetBorderSize(0);
//   leg->SetHeader(legheader);
//   leg->AddEntry(dijetptall2,"Anti-k_{t}5, second jet, uncleaned","l");
//   leg->AddEntry(dijetptall2cleaned,"Anti-k_{t}5, second jet, cleaned","l");
//   leg->Draw();

//   c->SaveAs(dataset+"_dijet_ptall2_and_ptall2cleaned.gif");
//   c->SaveAs(dataset+"_dijet_ptall2_and_ptall2cleaned.eps");

  // dijetptalltotUNcleaned and dijetptalltotcleaned
  c->SetLogy(1);
  dijetptall2->Add(dijetptall1);
  dijetptall2cleaned->Add(dijetptall1cleaned);
//   dijetptall2->Rebin(2);
//   dijetptall2cleaned->Rebin(2);
  double MAX=getMaximum(dijetptall2, dijetptall2cleaned);
  dijetptall2->SetMaximum(1.1*MAX);
  dijetptall2->SetLineStyle(1);
  dijetptall2->SetLineWidth(2);
  dijetptall2->SetXTitle("jet p_{T}[GeV]");
  dijetptall2->SetYTitle("number of jets / bin");
  dijetptall2->Draw();
  dijetptall2cleaned->SetLineStyle(4);
  dijetptall2cleaned->SetLineWidth(2);
  dijetptall2cleaned->Draw("sames");
  
  TLegend *leg = new TLegend(0.35,0.65,0.75,0.85);
  leg->SetFillStyle(4000);
  leg->SetFillColor(kWhite);
  leg->SetBorderSize(0);
  leg->SetHeader(legheader);
  leg->AddEntry(dijetptall2,"Anti-k_{t}5, first 2 jets uncleaned","l");
  leg->AddEntry(dijetptall2cleaned,"Anti-k_{t}5, first 2 jets cleaned","l");
  leg->Draw();

  c->SaveAs(dataset+"_dijet_ptalltot.gif");
  c->SaveAs(dataset+"_dijet_ptalltot.eps");




  // dijetdphiUNcleaned and dijetdphicleaned
  c->SetLogy(0);
  dijetdphi->Rebin(3);
  dijetdphicleaned->Rebin(3);
  double MAX=getMaximum(dijetdphi, dijetdphicleaned);
  dijetdphi->SetMaximum(1.8*MAX);
  dijetdphi->SetLineStyle(1);
  dijetdphi->SetLineWidth(2);
  dijetdphi->SetXTitle("#Delta #phi_{jj}");
  dijetdphi->SetYTitle("number of jets / bin");
  dijetdphi->Draw();
  dijetdphicleaned->SetLineStyle(4);
  dijetdphicleaned->SetLineWidth(2);
  dijetdphicleaned->Draw("sames");
  
  TLegend *leg = new TLegend(0.35,0.65,0.75,0.85);
  leg->SetFillStyle(4000);
  leg->SetFillColor(kWhite);
  leg->SetBorderSize(0);
  leg->SetHeader(legheader);
  leg->AddEntry(dijetdphi,"Anti-k_{t}5, uncleaned","l");
  leg->AddEntry(dijetdphicleaned,"Anti-k_{t}5, cleaned","l");
  leg->Draw();

  c->SaveAs(dataset+"_dijet_dphi.gif");
  c->SaveAs(dataset+"_dijet_dphi.eps");


    // dijeteta
  c->SetLogy(0);
  dijeteta->SetTitle(dataset);
  dijetetacleaned->SetTitle(dataset);
  double MAX=getMaximum(dijeteta,dijetetacleaned);
  dijeteta->SetMaximum(2.5*MAX);
  dijeteta->SetLineStyle(1);
  dijeteta->SetLineWidth(2);
  dijeteta->SetYTitle("number of jets / bin");
  dijeteta->Draw("hist");
  dijetetacleaned->SetLineStyle(4);
  dijetetacleaned->SetLineWidth(2);
  dijetetacleaned->Draw("histsame");

  TLegend *leg = new TLegend(0.35,0.65,0.75,0.85);
  leg->SetFillStyle(4000);
  leg->SetFillColor(kWhite);
  leg->SetBorderSize(0);
  leg->SetHeader(legheader);
  leg->AddEntry(dijeteta,"Anti-k_{t}5, uncleaned","l");
  leg->AddEntry(dijetetacleaned,"Anti-k_{t}5, cleaned","l");
  leg->Draw();


  c->SaveAs(dataset+"_dijet_eta.gif");
  c->SaveAs(dataset+"_dijet_eta.eps");


  c->SetLogy(0);
  dijetphi->SetTitle(dataset);
  dijetphicleaned->SetTitle(dataset);
  double MAX=getMaximum(dijetphi,dijetphicleaned);
  dijetphi->SetMaximum(2.5*MAX);
  dijetphi->SetMinimum(0);
  dijetphi->SetLineStyle(1);
  dijetphi->SetLineWidth(2);
  dijetphi->SetYTitle("number of jets / bin");
  dijetphi->Draw("hist");
  dijetphicleaned->SetLineStyle(4);
  dijetphicleaned->SetLineWidth(2);
  dijetphicleaned->Draw("histsame");

  TLegend *leg = new TLegend(0.35,0.65,0.75,0.85);
  leg->SetFillStyle(4000);
  leg->SetFillColor(kWhite);
  leg->SetBorderSize(0);
  leg->SetHeader(legheader);
  leg->AddEntry(dijetphi,"Anti-k_{t}5, uncleaned","l");
  leg->AddEntry(dijetphicleaned,"Anti-k_{t}5, cleaned","l");
  leg->Draw();

  c->SaveAs(dataset+"_dijet_phi.gif");
  c->SaveAs(dataset+"_dijet_phi.eps");




  ////--------------------------



//   //ak5NlooseTracks and ak5NtightTracks


//   //  c=makePlot(c, ak5NlooseTracks, ak5NtightTracks, 1, "asd", "fgh");

//   c->SetLogy(1);
//   ak5NlooseTracks->SetLineStyle(1);
//   ak5NlooseTracks->SetLineWidth(2);
//   ak5NlooseTracks->SetXTitle("number of tracks in cone");
//   ak5NlooseTracks->SetYTitle("number of jets / bin");
//   ak5NlooseTracks->Draw();
//   ak5NtightTracks->SetLineStyle(4);
//   ak5NtightTracks->SetLineWidth(2);
//   ak5NtightTracks->Draw("sames");
  
//   TLegend *leg = new TLegend(0.35,0.65,0.75,0.85);
//   leg->SetFillStyle(4000);
//   leg->SetFillColor(kWhite);
//   leg->SetBorderSize(0);
//   leg->AddEntry(ak5NlooseTracks,"Anti-k_{t}5, loose tracks","l");
//   leg->AddEntry(ak5NtightTracks,"Anti-k_{t}5, tight tracks","l");
//   leg->Draw();

//   c->SaveAs(dataset+"_ak5N_loose_and_tight_tracks.gif");


//   // ChFracLoose and ChFracTight
  

//   c->SetLogy(1);
//   ChFracLoose->SetLineStyle(1);
//   ChFracLoose->SetLineWidth(2);
//   ChFracLoose->SetXTitle("charged fraction");
//   ChFracLoose->SetYTitle("number of jets / bin");
//   ChFracLoose->Draw();
//   ChFracTight->SetLineStyle(4);
//   ChFracTight->SetLineWidth(2);
//   ChFracTight->Draw("sames");
  
//   TLegend *leg = new TLegend(0.35,0.65,0.75,0.85);
//   leg->SetFillStyle(4000);
//   leg->SetFillColor(kWhite);
//   leg->SetBorderSize(0);
//   leg->AddEntry(ChFracLoose,"Anti-k_{t}5, loose tracks","l");
//   leg->AddEntry(ChFracTight,"Anti-k_{t}5, tight tracks","l");
//   leg->Draw();

//   c->SaveAs(dataset+"_chFrac_loose_and_tight.gif");


//   // Di-jets
//   // dijetptall1 and dijetptall2

//   c->SetLogy(1);
//   double MAX=getMaximum(dijetptall1,dijetptall2);
//   cout<<MAX<<endl;
//   dijetptall1->SetMaximum(1.1*MAX);
//   dijetptall1->SetLineStyle(1);
//   dijetptall1->SetLineWidth(2);
//   dijetptall1->SetYTitle("number of jets / bin");
//     dijetptall1->Draw();
//   dijetptall2->SetLineStyle(4);
//   dijetptall2->SetLineWidth(2);
//   dijetptall2->Draw("sames");
  
//   TLegend *leg = new TLegend(0.35,0.65,0.75,0.85);
//   leg->SetFillStyle(4000);
//   leg->SetFillColor(kWhite);
//   leg->SetBorderSize(0);
//   leg->AddEntry(dijetptall1,"Anti-k_{t}5, first jet of di-jet event","l");
//   leg->AddEntry(dijetptall2,"Anti-k_{t}5, second jet of di-jet event","l");
//   leg->Draw();

//   c->SaveAs(dataset+"_dijet_ptall1_and_ptall2.gif");

//   // di-jet delta phi
//   c->SetLogy(1);
//   dijetdphi->SetLineStyle(1);
//   dijetdphi->SetLineWidth(2);
//   dijetdphi->SetYTitle("number of jets / bin");
//   dijetdphi->Draw();
  
// //   TLegend *leg = new TLegend(0.35,0.65,0.75,0.85);
// //   leg->SetFillStyle(4000);
// //   leg->SetFillColor(kWhite);
// //   leg->SetBorderSize(0);
// //   leg->AddEntry(dijetdphi,"di-jet events","l");
// //   leg->Draw();

//   c->SaveAs(dataset+"_dijet_delta_phi.gif");

//   // mapalldijets
//   c->SetLogy(0);
//   mapalldijets->SetTitle(dataset);
//   //palette->SetX1(0.5);
//   //TFrame->SetX2(0.1);
//   mapalldijets->Draw("colz");
//   c->SaveAs(dataset+"_dijet_phiVSeta.gif");
//   c->SaveAs(dataset+"_dijet_phiVSeta.eps");

//   // ak5NlooseTracksdijets and ak5NtightTracksdijets
//   c->SetLogy(0);
//   double MAX=getMaximum(ak5NlooseTracksdijets, ak5NlooseTracksdijets);
//   cout<<MAX<<endl;
//   ak5NlooseTracksdijets->SetMaximum(1.1*MAX);
//   ak5NlooseTracksdijets->SetLineStyle(1);
//   ak5NlooseTracksdijets->SetLineWidth(2);
//   ak5NlooseTracksdijets->SetXTitle("number of tracks in cone");
//   ak5NlooseTracksdijets->SetYTitle("number of jets / bin");
//   ak5NlooseTracksdijets->Draw();
//   ak5NtightTracksdijets->SetLineStyle(4);
//   ak5NtightTracksdijets->SetLineWidth(2);
//   ak5NtightTracksdijets->Draw("sames");
  
//   TLegend *leg = new TLegend(0.35,0.65,0.75,0.85);
//   leg->SetFillStyle(4000);
//   leg->SetFillColor(kWhite);
//   leg->SetBorderSize(0);
//   leg->AddEntry(ak5NlooseTracksdijets,"Anti-k_{t}5, di-jets, loose tracks","l");
//   leg->AddEntry(ak5NtightTracksdijets,"Anti-k_{t}5, di-jets, tight tracks","l");
//   leg->Draw();

//   c->SaveAs(dataset+"_dijet_ak5N_loose_and_tight_tracks.gif");

//   // ChFracLoosedijets and ChFracTightdijets
//   c->SetLogy(0);
//   ChFracLoosedijets->SetLineStyle(1);
//   ChFracLoosedijets->SetLineWidth(2);
//   ChFracLoosedijets->SetXTitle("charged fraction");
//   ChFracLoosedijets->SetYTitle("number of jets / bin");
//   ChFracLoosedijets->Draw();
//   ChFracTightdijets->SetLineStyle(4);
//   ChFracTightdijets->SetLineWidth(2);
//   ChFracTightdijets->Draw("sames");
  
//   TLegend *leg = new TLegend(0.35,0.65,0.75,0.85);
//   leg->SetFillStyle(4000);
//   leg->SetFillColor(kWhite);
//   leg->SetBorderSize(0);
//   leg->AddEntry(ChFracLoosedijets,"loose tracks in cone","l");
//   leg->AddEntry(ChFracTightdijets,"tight tracks in cone","l");
//   leg->Draw();

//   c->SaveAs(dataset+"_dijet_ChFrac_loose_and_tight.gif");



//   // dijetptall1cleaned and dijetptall2cleaned
//   c->SetLogy(0);
//   double MAX=getMaximum(dijetptall1cleaned, dijetptall2cleaned);
//   cout<<MAX<<endl;
//   dijetptall1cleaned->SetMaximum(1.1*MAX);
//   dijetptall1cleaned->SetLineStyle(1);
//   dijetptall1cleaned->SetLineWidth(2);
//   dijetptall1cleaned->SetXTitle("p_{T} of cleaned jets [GeV]");
//   dijetptall1cleaned->SetYTitle("number of jets / bin");
//   dijetptall1cleaned->Draw();
//   dijetptall2cleaned->SetLineStyle(4);
//   dijetptall2cleaned->SetLineWidth(2);
//   dijetptall2cleaned->Draw("sames");
  
//   TLegend *leg = new TLegend(0.35,0.65,0.75,0.85);
//   leg->SetFillStyle(4000);
//   leg->SetFillColor(kWhite);
//   leg->SetBorderSize(0);
//   leg->AddEntry(dijetptall1cleaned,"Anti-k_{t}5, first cleaned jet of di-jet events","l");
//   leg->AddEntry(dijetptall2cleaned,"Anti-k_{t}5, second cleaned jet of di-jet events","l");
//   leg->Draw();

//   c->SaveAs(dataset+"_dijet_ptall1_and_ptall2_cleaned.gif");


//   // dijetdphicleaned
//   c->SetLogy(0);
//   dijetdphicleaned->SetLineStyle(1);
//   dijetdphicleaned->SetLineWidth(2);
//   dijetdphicleaned->SetYTitle("number of jets / bin");
//   dijetdphicleaned->Draw();
  
// //   TLegend *leg = new TLegend(0.35,0.65,0.75,0.85);
// //   leg->SetFillStyle(4000);
// //   leg->SetFillColor(kWhite);
// //   leg->SetBorderSize(0);
// //   leg->AddEntry(dijetdphicleaned,"cleaned di-jet events","l");
// //   leg->Draw();

//   c->SaveAs(dataset+"_dijet_delta_phicleaned.gif");

//   // mapalldijetscleaned
//   c->SetLogy(0);
//   mapalldijetscleaned->SetTitle(dataset);
//   mapalldijetscleaned->Draw("colz");
//   c->SaveAs(dataset+"_dijet_phiVSeta_cleaned.gif");

  // ak5njetsindijets and ak5njetsindijetscleaned
  c->SetLogy(1);
  double MAX=getMaximum(ak5njetsindijets, ak5njetsindijetscleaned);
  ak5njetsindijets->SetMaximum(1.3*MAX);
  ak5njetsindijets->SetLineStyle(1);
  ak5njetsindijets->SetLineWidth(2);
  ak5njetsindijets->SetXTitle("number of jets");
  ak5njetsindijets->SetYTitle("number of jets / bin");
  ak5njetsindijets->SetBinContent(1,0);
  ak5njetsindijets->Draw();
  ak5njetsindijetscleaned->SetLineStyle(4);
  ak5njetsindijetscleaned->SetLineWidth(2);
  ak5njetsindijetscleaned->SetBinContent(1,0);
  ak5njetsindijetscleaned->Draw("sames");
  
  TLegend *leg = new TLegend(0.35,0.65,0.75,0.85);
  leg->SetFillStyle(4000);
  leg->SetFillColor(kWhite);
  leg->SetBorderSize(0);
  leg->AddEntry(ak5njetsindijets,"Anti-k_{t}5, di-jets","l");
  leg->AddEntry(ak5njetsindijetscleaned,"Anti-k_{t}5, cleaned di-jets","l");
  leg->SetHeader(legheader);
  leg->Draw();

  c->SaveAs(dataset+"_dijet_njets.gif");
  c->SaveAs(dataset+"_dijet_njets.eps");

//   // resemfdijets
//   c->SetLogy(0);
//   resemfdijets->SetLineStyle(1);
//   resemfdijets->SetLineWidth(2);
//   resemfdijets->SetYTitle("number of jets / bin");
//   resemfdijets->Draw();
  
// //   TLegend *leg = new TLegend(0.35,0.65,0.75,0.85);
// //   leg->SetFillStyle(4000);
// //   leg->SetFillColor(kWhite);
// //   leg->SetBorderSize(0);
// //   leg->AddEntry(resemfdijets,"di-jet events","l");
// //   leg->Draw();

//   c->SaveAs(dataset+"_dijet_resEMF.gif");

//   // fhpddijets
//   c->SetLogy(0);
//   fhpddijets->SetLineStyle(1);
//   fhpddijets->SetLineWidth(2);
//   fhpddijets->SetYTitle("entires / bin");
//   fhpddijets->Draw();
  
// //   TLegend *leg = new TLegend(0.35,0.65,0.75,0.85);
// //   leg->SetFillStyle(4000);
// //   leg->SetFillColor(kWhite);
// //   leg->SetBorderSize(0);
// //   leg->AddEntry(fhpddijets,"di-jet events","l");
// //   leg->Draw();

//   c->SaveAs(dataset+"_dijet_fhpd.gif");


//   // frbxdijets
//   c->SetLogy(0);
//   frbxdijets->SetLineStyle(1);
//   frbxdijets->SetLineWidth(2);
//   frbxdijets->SetYTitle("number of jets / bin");
//   frbxdijets->Draw();
  
// //   TLegend *leg = new TLegend(0.35,0.65,0.75,0.85);
// //   leg->SetFillStyle(4000);
// //   leg->SetFillColor(kWhite);
// //   leg->SetBorderSize(0);
// //   leg->AddEntry(frbxdijets,"di-jet events","l");
// //   leg->Draw();

//   c->SaveAs(dataset+"_dijet_frbx.gif");

//   // n90hitsdijets
//   c->SetLogy(0);
//   n90hitsdijets->SetLineStyle(1);
//   n90hitsdijets->SetLineWidth(2);
//   n90hitsdijets->SetYTitle("number of jets / bin");
//   n90hitsdijets->Draw();
  
// //   TLegend *leg = new TLegend(0.35,0.65,0.75,0.85);
// //   leg->SetFillStyle(4000);
// //   leg->SetFillColor(kWhite);
// //   leg->SetBorderSize(0);
// //   leg->AddEntry(n90hitsdijets,"di-jet events","l");
// //   leg->Draw();

//   c->SaveAs(dataset+"_dijet_n90hits.gif");



//   // variousEff
//   c->SetLogy(0);
//   variousEff->SetLineStyle(1);
//   variousEff->SetLineWidth(2);
//   variousEff->SetYTitle("efficiency");
//   variousEff->Draw();
  
// //   TLegend *leg = new TLegend(0.35,0.65,0.75,0.85);
// //   leg->SetFillStyle(4000);
// //   leg->SetFillColor(kWhite);
// //   leg->SetBorderSize(0);
// //   leg->AddEntry(variousEff,"di-jet events","l");
// //   leg->Draw();

//   c->SaveAs(dataset+"_variousEff.gif");

//   // variousEffindijets
//   c->SetLogy(0);
//   variousEffindijets->SetLineStyle(1);
//   variousEffindijets->SetLineWidth(2);
//   variousEffindijets->SetYTitle("efficiency");
//   variousEffindijets->Draw();
  
// //   TLegend *leg = new TLegend(0.35,0.65,0.75,0.85);
// //   leg->SetFillStyle(4000);
// //   leg->SetFillColor(kWhite);
// //   leg->SetBorderSize(0);
// //   leg->AddEntry(variousEffindijets,"di-jet events","l");
// //   leg->Draw();

//   c->SaveAs(dataset+"_dijets_variousEff.gif");



////------------------------------------

}


Double_t getMaximum(TH1 *histo1, TH1 *histo2) {
  int n=histo1->GetNbinsX();
  double max1=0;
  for(int i=1;i<=n;i++){
    if(histo1->GetBinContent(i)>max1) {
      max1=histo1->GetBinContent(i);
    }
  }
  double max2=0;
  for(int i=1;i<=n;i++){
    if(histo2->GetBinContent(i)>max2) {
      max2=histo2->GetBinContent(i);
    }
    
  }
  if(max1>max2) {
    return max1;
  }
  else return max2;
}



TCanvas makePlot(TCanvas c, TH1D histo1, TH1D histo2, int logscale, char *legTitle1, char legTitle2) {
  c->SetLog(logscale);
  histo1->SetLineStyle(1);
  histo1->SetLineWidth(2);
  histo1->Draw();
  histo2->SetLineStyle(4);
  histo2->SetLineWidth(2);
  histo2->Drw("sames");

  TLegend *leg = new TLegend(0.35,0.65,0.85,0.85);
  leg->SetFillStyle(4000);
  leg->SetFillColor(kWhite);
  leg->SetBorderSize(0);
  leg->AddEntry(histo1,legTitle1,"l");
  leg->AddEntry(histo2,legTitle2,"l");
  leg->Draw();

  return c;
}
