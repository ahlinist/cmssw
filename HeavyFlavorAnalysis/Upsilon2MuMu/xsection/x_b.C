#include "TMath.h"
#include <math.h>
#include "TF1.h"
#include "TH1.h"

void x_b(){
  
  TFile *f1 = new TFile("test-xb.xsReader.default.root");
  TFile *f = new TFile("111112.dimuons.xsReader.default.root");
  TTree *nm = (TTree*)f->Get("nm");
  TTree *m = (TTree*)f1->Get("m");
  TTree *mbg = (TTree*)f1->Get("mbg");
  TH1D *h10 = new TH1D("h10","#DeltaM_{#mu#mu#gamma} w Matching", 24, 0., 1.2);
  TH1D *h11 = new TH1D("h11","#DeltaR w Matching", 60, 0., 6.);
  TH1D *h12 = new TH1D("h12","#DeltaM_{#mu#mu#gamma} w Matching/#DeltaR cut", 24, 0., 1.2);
  TH1D *h13 = new TH1D("h13","#DeltaM_{#mu#mu#gamma} w Matching/#DeltaR/#chi_{b} mass range cut", 24, 0., 1.2);
  TH1D *h14 = new TH1D("h14","#DeltaM_{#mu#mu#gamma} w Matching/#DeltaR/#chi_{b}&&#Upsilon  mass range cut", 24, 0., 1.2);
  TH1D *h15 = new TH1D("h15","#gamma_{E} w Matching", 60, 0., 6.);
  TH1D *h16 = new TH1D("h16","#gamma_{#eta} w Matching", 60, -3., 3.);
  TH1D *h17 = new TH1D("h17","#gamma_{#phi} w Matching", 80, -4., 4.); 
  
  TH1D *h20 = new TH1D("h20","#DeltaM_{#mu#mu#gamma}", 24, 0., 1.2);
  TH1D *h21 = new TH1D("h21","#DeltaR", 60, 0., 6.);
  TH1D *h22 = new TH1D("h22","#DeltaM_{#mu#mu#gamma} w #DeltaR cut", 24, 0., 1.2);
  TH1D *h23 = new TH1D("h23","#DeltaM_{#mu#mu#gamma} w #DeltaR/#chi_{b}&&#Upsilon mass range cut", 24, 0., 1.2);
  TH1D *h24 = new TH1D("h24","#DeltaM_{#mu#mu#gamma} w #chi_{b}&&#Upsilon  mass range cut", 24, 0., 1.2);
  TH1D *h25 = new TH1D("h25","#gamma_{E}", 60, 0., 6.);
  TH1D *h26 = new TH1D("h26","#gamma_{#eta}", 60, -3., 3.);
  TH1D *h27 = new TH1D("h27","#gamma_{#phi}", 80, -4., 4.);
  TH1D *h28 = new TH1D("h28","#DeltaM_{#mu#mu#gamma} w #gamma_{E} > 0.25", 24, 0., 1.2);
  TH1D *h29 = new TH1D("h29","#DeltaM_{#mu#mu#gamma} w #gamma_{E} > 0.5", 24, 0., 1.2);
  TH1D *h30 = new TH1D("h30","#DeltaM_{#mu#mu#gamma} w #gamma_{E} > 0.75", 24, 0., 1.2);
  TH1D *h31 = new TH1D("h31","#DeltaM_{#mu#mu#gamma} w #gamma_{E} > 1.", 24, 0., 1.2);
  TH1D *h32 = new TH1D("h32","#DeltaM_{#mu#mu#gamma} w #Upsilon mass range cut", 24, 0., 1.2);
  TH1D *h33 = new TH1D("h33","#DeltaM_{#mu#mu#gamma} w #gamma_{E} > 0.5 && #Upsilon mass range cut", 24, 0., 1.2);
  TH1D *h34 = new TH1D("h34","#DeltaM_{#mu#mu#gamma} w #gamma_{E} > 0.75 && #Upsilon mass range cut", 24, 0., 1.2);
  TH1D *h35 = new TH1D("h35","#DeltaM_{#mu#mu#gamma} w #gamma_{E} > 1. && #Upsilon mass range cut", 24, 0., 1.2);  
  TH1D *h36 = new TH1D("h36","#DeltaM_{#mu#mu#gamma} w #gamma_{E} > 1.5", 24, 0., 1.2);
  TH1D *h37 = new TH1D("h37","#DeltaM_{#mu#mu#gamma} w #gamma_{E} > 2.", 24, 0., 1.2);
  TH1D *h38 = new TH1D("h38","#DeltaM_{#mu#mu#gamma} w #gamma_{E} > 3.", 24, 0., 1.2);
  TH1D *h39 = new TH1D("h39","#DeltaM_{#mu#mu#gamma} w #gamma_{E} > 5.", 24, 0., 1.2); 
  TH1D *h40 = new TH1D("h40","#DeltaM_{#mu#mu#gamma} w #gamma_{E} > 1.5 && #Upsilon mass range cut", 24, 0., 1.2);
  TH1D *h41 = new TH1D("h41","#DeltaM_{#mu#mu#gamma} w #gamma_{E} > 2. && #Upsilon mass range cut", 24, 0., 1.2);
  TH1D *h42 = new TH1D("h42","#DeltaM_{#mu#mu#gamma} w #gamma_{E} > 3. && #Upsilon mass range cut", 24, 0., 1.2);
  TH1D *h43 = new TH1D("h43","#DeltaM_{#mu#mu#gamma} w #gamma_{E} > 5. && #Upsilon mass range cut", 24, 0., 1.2);  
  TH1D *h44 = new TH1D("h44","#DeltaM_{#mu#mu#gamma} w #Upsilon mass range cut", 24, 0., 1.2);
  TH1D *h45 = new TH1D("h45","#DeltaM_{#mu#mu#gamma} w #Upsilon mass range cut && #DeltaR<0.1", 24, 0., 1.2);
  TH1D *h46 = new TH1D("h46","#DeltaM_{#mu#mu#gamma} w #Upsilon mass range cut && #DeltaR<0.2", 24, 0., 1.2);
  TH1D *h47 = new TH1D("h47","#DeltaM_{#mu#mu#gamma} w #Upsilon mass range cut && #DeltaR<0.3", 24, 0., 1.2);
  TH1D *h48 = new TH1D("h48","#DeltaM_{#mu#mu#gamma} w #Upsilon mass range cut && #gamma_{E} > 1.", 24, 0., 1.2);
  TH1D *h49 = new TH1D("h49","#DeltaM_{#mu#mu#gamma} w #Upsilon mass range cut && #DeltaR<0.1 && #gamma_{E} > 1.", 24, 0., 1.2);
  TH1D *h50 = new TH1D("h50","#DeltaM_{#mu#mu#gamma} w #Upsilon mass range cut && #DeltaR<0.2 && #gamma_{E} > 1.", 24, 0., 1.2);
  TH1D *h51 = new TH1D("h51","#DeltaM_{#mu#mu#gamma} w #Upsilon mass range cut && #DeltaR<0.3 && #gamma_{E} > 1.", 24, 0., 1.2);  
  TH1D *h52 = new TH1D("h52","#DeltaM_{#mu#mu#gamma} w #gamma_{E} > 1.5 && SideBand mass range cut", 24, 0., 1.2);
  TH1D *h53 = new TH1D("h53","#DeltaM_{#mu#mu#gamma} w #gamma_{E} > 2. &&  SideBand mass range cut", 24, 0., 1.2);
  TH1D *h54 = new TH1D("h54","#DeltaM_{#mu#mu#gamma} w #gamma_{E} > 3. &&  SideBand mass range cut", 24, 0., 1.2);
  TH1D *h55 = new TH1D("h55","#DeltaM_{#mu#mu#gamma} w #gamma_{E} > 5. &&  SideBand mass range cut", 24, 0., 1.2); 
  TH1D *h56 = new TH1D("h56","#DeltaM_{#mu#mu#gamma} w SideBand mass range cut", 24, 0., 1.2);
  TH1D *h57 = new TH1D("h57","#DeltaM_{#mu#mu#gamma} w SideBand mass range cut && #DeltaR<0.1", 24, 0., 1.2);
  TH1D *h58 = new TH1D("h58","#DeltaM_{#mu#mu#gamma} w SideBand mass range cut && #DeltaR<0.2", 24, 0., 1.2);
  TH1D *h59 = new TH1D("h59","#DeltaM_{#mu#mu#gamma} w SideBand mass range cut && #DeltaR<0.3", 24, 0., 1.2);
  TH1D *h60 = new TH1D("h60","#DeltaM_{#mu#mu#gamma} w SideBand mass range cut && #gamma_{E} > 1.", 24, 0., 1.2);
  TH1D *h61 = new TH1D("h61","#DeltaM_{#mu#mu#gamma} w SideBand mass range cut && #DeltaR<0.1 && #gamma_{E} > 1.", 24, 0., 1.2);
  TH1D *h62 = new TH1D("h62","#DeltaM_{#mu#mu#gamma} w SideBand mass range cut && #DeltaR<0.2 && #gamma_{E} > 1.", 24, 0., 1.2);
  TH1D *h63 = new TH1D("h63","#DeltaM_{#mu#mu#gamma} w SideBand mass range cut && #DeltaR<0.3 && #gamma_{E} > 1.", 24, 0., 1.2);

  TH1D *h64 = new TH1D("h64","#DeltaM_{#mu#mu#gamma} w Matching/#Upsilon mass range cut && #DeltaR<0.1 && #gamma_{E} > 1.", 24, 0., 1.2);
  TH1D *h65 = new TH1D("h65","#DeltaM_{#mu#mu#gamma} w Matching/#Upsilon mass range cut && #DeltaR<0.2 && #gamma_{E} > 1.", 24, 0., 1.2);
  TH1D *h66 = new TH1D("h66","#DeltaM_{#mu#mu#gamma} w Matching/#Upsilon mass range cut && #DeltaR<0.3 && #gamma_{E} > 1.", 24, 0., 1.2);
  
  TH1D *h67 = new TH1D("h67","#DeltaM_{#mu#mu#gamma} (x_b0) w Matching/#Upsilon mass range cut && #DeltaR<0.1 && #gamma_{E} > 1.", 24, 0., 1.2);
  TH1D *h68 = new TH1D("h68","#DeltaM_{#mu#mu#gamma} (x_b0) w Matching/#Upsilon mass range cut && #DeltaR<0.2 && #gamma_{E} > 1.", 24, 0., 1.2);
  TH1D *h69 = new TH1D("h69","#DeltaM_{#mu#mu#gamma} (x_b0) w Matching/#Upsilon mass range cut && #DeltaR<0.3 && #gamma_{E} > 1.", 24, 0., 1.2); 
  
  TH1D *h70 = new TH1D("h70","#DeltaM_{#mu#mu#gamma} (x_b1) w Matching/#Upsilon mass range cut && #DeltaR<0.1 && #gamma_{E} > 1.", 24, 0., 1.2);
  TH1D *h71 = new TH1D("h71","#DeltaM_{#mu#mu#gamma} (x_b1) w Matching/#Upsilon mass range cut && #DeltaR<0.2 && #gamma_{E} > 1.", 24, 0., 1.2);
  TH1D *h72 = new TH1D("h72","#DeltaM_{#mu#mu#gamma} (x_b1) w Matching/#Upsilon mass range cut && #DeltaR<0.3 && #gamma_{E} > 1.", 24, 0., 1.2);   
  
  TH1D *h73 = new TH1D("h73","#DeltaM_{#mu#mu#gamma} (x_b2) w Matching/#Upsilon mass range cut && #DeltaR<0.1 && #gamma_{E} > 1.", 24, 0., 1.2);
  TH1D *h74 = new TH1D("h74","#DeltaM_{#mu#mu#gamma} (x_b2) w Matching/#Upsilon mass range cut && #DeltaR<0.2 && #gamma_{E} > 1.", 24, 0., 1.2);
  TH1D *h75 = new TH1D("h75","#DeltaM_{#mu#mu#gamma} (x_b2) w Matching/#Upsilon mass range cut && #DeltaR<0.3 && #gamma_{E} > 1.", 24, 0., 1.2);     
  
  TH1D *h76 = new TH1D("h76","#DeltaM_{#mu#mu#gamma} w SideBand [10.7-11.2] mass range cut", 24, 0., 1.2);
  TH1D *h77 = new TH1D("h77","#DeltaM_{#mu#mu#gamma} w SideBand [10.7-11.2] mass range cut && #DeltaR<0.1", 24, 0., 1.2);
  TH1D *h78 = new TH1D("h78","#DeltaM_{#mu#mu#gamma} w SideBand [10.7-11.2] mass range cut && #DeltaR<0.2", 24, 0., 1.2);
  TH1D *h79 = new TH1D("h79","#DeltaM_{#mu#mu#gamma} w SideBand [10.7-11.2] mass range cut && #DeltaR<0.3", 24, 0., 1.2);
  TH1D *h80 = new TH1D("h80","#DeltaM_{#mu#mu#gamma} w SideBand [10.7-11.2] mass range cut && #gamma_{E} > 1.", 24, 0., 1.2);
  TH1D *h81 = new TH1D("h81","#DeltaM_{#mu#mu#gamma} w SideBand [10.7-11.2] mass range cut && #DeltaR<0.1 && #gamma_{E} > 1.", 24, 0., 1.2);
  TH1D *h82 = new TH1D("h82","#DeltaM_{#mu#mu#gamma} w SideBand [10.7-11.2] mass range cut && #DeltaR<0.2 && #gamma_{E} > 1.", 24, 0., 1.2);
  TH1D *h83 = new TH1D("h83","#DeltaM_{#mu#mu#gamma} w SideBand [10.7-11.2] mass range cut && #DeltaR<0.3 && #gamma_{E} > 1.", 24, 0., 1.2);
  
  TH1D *h84 = new TH1D("h84","#DeltaM_{#mu#mu#gamma} w Background Matching/#Upsilon mass range cut && #DeltaR<0.1 && #gamma_{E} > 1.", 24, 0., 1.2);
  TH1D *h85 = new TH1D("h85","#DeltaM_{#mu#mu#gamma} w Background Matching/#Upsilon mass range cut && #DeltaR<0.2 && #gamma_{E} > 1.", 24, 0., 1.2);
  TH1D *h86 = new TH1D("h86","#DeltaM_{#mu#mu#gamma} w Background Matching/#Upsilon mass range cut && #DeltaR<0.3 && #gamma_{E} > 1.", 24, 0., 1.2);
  
  float um, uP, ue, up, gE, gP, ge, gp, dR, xbm;
  float m_um, m_uP, m_ue, m_up, m_gE, m_gP, m_ge, m_gp, m_dR, m_xbm;
  float mbg_um, mbg_uP, mbg_ue, mbg_up, mbg_gE, mbg_gP, mbg_ge, mbg_gp, mbg_dR, mbg_xbm;
  int m_xbid;
  nm->SetBranchAddress("um", &um);
  nm->SetBranchAddress("uP", &uP);
  nm->SetBranchAddress("ue", &ue);
  nm->SetBranchAddress("up", &up);
  nm->SetBranchAddress("gE", &gE);
  nm->SetBranchAddress("gP", &gP);
  nm->SetBranchAddress("ge", &ge);
  nm->SetBranchAddress("gp", &gp);
  nm->SetBranchAddress("dR", &dR);
  nm->SetBranchAddress("xbm", &xbm);
  
  m->SetBranchAddress("m_um", &m_um);
  m->SetBranchAddress("m_uP", &m_uP);
  m->SetBranchAddress("m_ue", &m_ue);
  m->SetBranchAddress("m_up", &m_up);
  m->SetBranchAddress("m_gE", &m_gE);
  m->SetBranchAddress("m_gP", &m_gP);
  m->SetBranchAddress("m_ge", &m_ge);
  m->SetBranchAddress("m_gp", &m_gp);
  m->SetBranchAddress("m_dR", &m_dR);
  m->SetBranchAddress("m_xbid", &m_xbid);
  m->SetBranchAddress("m_xbm", &m_xbm);
  
  mbg->SetBranchAddress("mbg_um", &mbg_um);
  mbg->SetBranchAddress("mbg_uP", &mbg_uP);
  mbg->SetBranchAddress("mbg_ue", &mbg_ue);
  mbg->SetBranchAddress("mbg_up", &mbg_up);
  mbg->SetBranchAddress("mbg_gE", &mbg_gE);
  mbg->SetBranchAddress("mbg_gP", &mbg_gP);
  mbg->SetBranchAddress("mbg_ge", &mbg_ge);
  mbg->SetBranchAddress("mbg_gp", &mbg_gp);
  mbg->SetBranchAddress("mbg_dR", &mbg_dR);
  mbg->SetBranchAddress("mbg_xbm", &mbg_xbm);  
  
  
  Int_t NEntries = Int_t(mbg->GetEntries());
  
  Int_t NB(0), NBytes(0);
  for (Int_t entry = 0; entry < NEntries; entry++) {
    NB = mbg->GetEntry(entry);   NBytes += NB;
    if ( mbg_gE > 1. && (mbg_um<9.6) && (mbg_um>9.2) && (mbg_dR < 0.1)) h84->Fill(mbg_xbm-mbg_um);
    if ( mbg_gE > 1. && (mbg_um<9.6) && (mbg_um>9.2) && (mbg_dR < 0.2)) h85->Fill(mbg_xbm-mbg_um);
    if ( mbg_gE > 1. && (mbg_um<9.6) && (mbg_um>9.2) && (mbg_dR < 0.3)) h86->Fill(mbg_xbm-mbg_um);
    
  }

  
  Int_t nentries = Int_t(m->GetEntries());
  
  Int_t nb(0), nbytes(0);
  for (Int_t jentry = 0; jentry < nentries; jentry++) {
    nb = m->GetEntry(jentry);   nbytes += nb;
    
    h10->Fill(m_xbm-m_um);
    h11->Fill(m_dR);
    h15->Fill(m_gE);
    h16->Fill(m_ge);
    h17->Fill(m_gp);
    if ( m_dR < 0.2 ) h12->Fill(m_xbm-m_um);
    if ( (m_xbm<10.) && (m_xbm>9.8) && (m_dR<0.2) ) h13->Fill(m_xbm-m_um);
    if ( (m_xbm<10.) && (m_xbm>9.8) && (m_um<9.6) && (m_um>9.2) ) h14->Fill(m_xbm-m_um);
    
    if ( m_gE > 1. && (m_um<9.6) && (m_um>9.2) && (m_dR < 0.1)) h64->Fill(m_xbm-m_um);
    if ( m_gE > 1. && (m_um<9.6) && (m_um>9.2) && (m_dR < 0.2)) h65->Fill(m_xbm-m_um);
    if ( m_gE > 1. && (m_um<9.6) && (m_um>9.2) && (m_dR < 0.3)) h66->Fill(m_xbm-m_um);
    
    if ( m_xbid == 10551  ){
      if ( m_gE > 1. && (m_um<9.6) && (m_um>9.2) && (m_dR < 0.1)) h67->Fill(m_xbm-m_um);
      if ( m_gE > 1. && (m_um<9.6) && (m_um>9.2) && (m_dR < 0.2)) h68->Fill(m_xbm-m_um);
      if ( m_gE > 1. && (m_um<9.6) && (m_um>9.2) && (m_dR < 0.3)) h69->Fill(m_xbm-m_um);
    }
      
    if ( m_xbid == 20553  ){
      if ( m_gE > 1. && (m_um<9.6) && (m_um>9.2) && (m_dR < 0.1)) h70->Fill(m_xbm-m_um);
      if ( m_gE > 1. && (m_um<9.6) && (m_um>9.2) && (m_dR < 0.2)) h71->Fill(m_xbm-m_um);
      if ( m_gE > 1. && (m_um<9.6) && (m_um>9.2) && (m_dR < 0.3)) h72->Fill(m_xbm-m_um);
    }      
      
    if ( m_xbid == 555  ){
      if ( m_gE > 1. && (m_um<9.6) && (m_um>9.2) && (m_dR < 0.1)) h73->Fill(m_xbm-m_um);
      if ( m_gE > 1. && (m_um<9.6) && (m_um>9.2) && (m_dR < 0.2)) h74->Fill(m_xbm-m_um);
      if ( m_gE > 1. && (m_um<9.6) && (m_um>9.2) && (m_dR < 0.3)) h75->Fill(m_xbm-m_um);
    }         
      
      
  }
  
  Int_t Nentries = Int_t(nm->GetEntries());
  
  Int_t Nb(0), Nbytes(0);
  for (Int_t Jentry = 0; Jentry < Nentries; Jentry++) {
    Nb = nm->GetEntry(Jentry);   Nbytes += Nb;
    
    h20->Fill(xbm-um);
    h21->Fill(dR);
    h25->Fill(gE);
    h26->Fill(ge);
    h27->Fill(gp);
    if ( dR < 0.2 ) h22->Fill(xbm-um);
    if ( (xbm<10.) && (xbm>9.8) && (um<9.6) && (um>9.2) && (dR<0.2) ) h23->Fill(xbm-um);
    if ( (xbm<10.) && (xbm>9.8) && (um<9.6) && (um>9.2) ) h24->Fill(xbm-um);
    if ( gE > 0.25 ) h28->Fill(xbm-um); 
    if ( gE > 0.5 ) h29->Fill(xbm-um);
    if ( gE > 0.75 ) h30->Fill(xbm-um);
    if ( gE > 1. ) h31->Fill(xbm-um);
    if ( (um<9.6) && (um>9.2)) h32->Fill(xbm-um); 
    if ( gE > 0.5 && (um<9.6) && (um>9.2)) h33->Fill(xbm-um);
    if ( gE > 0.75 && (um<9.6) && (um>9.2)) h34->Fill(xbm-um);
    if ( gE > 1. && (um<9.6) && (um>9.2)) h35->Fill(xbm-um);
    if ( gE > 1.5 ) h36->Fill(xbm-um); 
    if ( gE > 2. ) h37->Fill(xbm-um);
    if ( gE > 3. ) h38->Fill(xbm-um);
    if ( gE > 5. ) h39->Fill(xbm-um);
    
    if ( gE > 1.5 && (um<9.6) && (um>9.2)) h40->Fill(xbm-um); 
    if ( gE > 2. && (um<9.6) && (um>9.2)) h41->Fill(xbm-um);
    if ( gE > 3. && (um<9.6) && (um>9.2)) h42->Fill(xbm-um);
    if ( gE > 5. && (um<9.6) && (um>9.2)) h43->Fill(xbm-um);
    if ( (um<9.6) && (um>9.2)) h44->Fill(xbm-um);
    if ( (um<9.6) && (um>9.2) && (dR < 0.1)) h45->Fill(xbm-um);
    if ( (um<9.6) && (um>9.2) && (dR < 0.2)) h46->Fill(xbm-um);
    if ( (um<9.6) && (um>9.2) && (dR < 0.3)) h47->Fill(xbm-um);
    if ( gE > 1. && (um<9.6) && (um>9.2)) h48->Fill(xbm-um);
    if ( gE > 1. && (um<9.6) && (um>9.2) && (dR < 0.1)) h49->Fill(xbm-um);
    if ( gE > 1. && (um<9.6) && (um>9.2) && (dR < 0.2)) h50->Fill(xbm-um);
    if ( gE > 1. && (um<9.6) && (um>9.2) && (dR < 0.3)) h51->Fill(xbm-um);
    
    if ( gE > 1.5 && (um<9.1) && (um>8.7)) h52->Fill(xbm-um); 
    if ( gE > 2. && (um<9.1) && (um>8.7)) h53->Fill(xbm-um);
    if ( gE > 3. && (um<9.1) && (um>8.7)) h54->Fill(xbm-um);
    if ( gE > 5. && (um<9.1) && (um>8.7)) h55->Fill(xbm-um);
    
    if ( (um<9.1) && (um>8.7)) h56->Fill(xbm-um);
    if ( (um<9.1) && (um>8.7) && (dR < 0.1)) h57->Fill(xbm-um);
    if ( (um<9.1) && (um>8.7) && (dR < 0.2)) h58->Fill(xbm-um);
    if ( (um<9.1) && (um>8.7) && (dR < 0.3)) h59->Fill(xbm-um);
    if ( gE > 1. && (um<9.1) && (um>8.7)) h60->Fill(xbm-um);
    if ( gE > 1. && (um<9.1) && (um>8.7) && (dR < 0.1)) h61->Fill(xbm-um);
    if ( gE > 1. && (um<9.1) && (um>8.7) && (dR < 0.2)) h62->Fill(xbm-um);
    if ( gE > 1. && (um<9.1) && (um>8.7) && (dR < 0.3)) h63->Fill(xbm-um);    
    
    if ( (um<11.2) && (um>10.7)) h76->Fill(xbm-um);
    if ( (um<11.2) && (um>10.7) && (dR < 0.1)) h77->Fill(xbm-um);
    if ( (um<11.2) && (um>10.7) && (dR < 0.2)) h78->Fill(xbm-um);
    if ( (um<11.2) && (um>10.7) && (dR < 0.3)) h79->Fill(xbm-um);
    if ( gE > 1. && (um<11.2) && (um>10.7)) h80->Fill(xbm-um);
    if ( gE > 1. && (um<11.2) && (um>10.7) && (dR < 0.1)) h81->Fill(xbm-um);
    if ( gE > 1. && (um<11.2) && (um>10.7) && (dR < 0.2)) h82->Fill(xbm-um);
    if ( gE > 1. && (um<11.2) && (um>10.7) && (dR < 0.3)) h83->Fill(xbm-um);     
    
    
  }
  
  
  gStyle->SetOptFit(0000000000);
  gStyle->SetOptStat(0000000000);
  /*
  TCanvas *c2 = new TCanvas("c2", "c2");
  h56->SetLineColor(2);
  h57->SetLineColor(2);
  h58->SetLineColor(2);
  h59->SetLineColor(2);
  h60->SetLineColor(2);
  h61->SetLineColor(2);
  h62->SetLineColor(2);
  h63->SetLineColor(2);  
  c2->Divide(4,2);
  c2->cd(1);
  h76->Draw();
  h56->Draw("same");
  c2->cd(2);
  h77->Draw();
  h57->Draw("same");
  c2->cd(3);
  h58->Draw();
  h78->Draw("same");
  c2->cd(4);
  h59->Draw();
  h79->Draw("same");
  c2->cd(5);
  h60->Draw();
  h80->Draw("same");
  c2->cd(6);
  h61->Draw();
  h81->Draw("same");
  c2->cd(7);
  h62->Draw();
  h82->Draw("same");
  c2->cd(8);
  h63->Draw();
  h83->Draw("same");  */
  
  
  double scale = h49->Integral(14,24) / h61->Integral(14,24);
  cout << h49->Integral(14,24) <<  " "  << h61->Integral(14,24) <<  " "  << scale << endl;
  TH1D *h61copy = (TH1D*)h61->Clone(); 
  h61copy->SetName("h61copy");
  /*  TCanvas *c1 = new TCanvas("c1", "c1");
  c1->Divide(2,2);
  c1->cd(1);  
  h49->Draw();
  c1->cd(2);
  h61copy->Draw();
  c1->cd(3);
  h61->SetLineColor(2);
  h61->Scale(scale);
  h49->Draw();
  h61->Draw("same");
  c1->cd(4);
  h49->Draw();
  h61->Draw("same"); 
  h64->SetLineColor(3);
  h64->Draw("same");   */
  
  double scle = h50->Integral(14,24) / h62->Integral(14,24);
  cout << h50->Integral(14,24) <<  " "  << h62->Integral(14,24) <<  " "  << scle << endl;
  TH1D *h62copy = (TH1D*)h62->Clone(); 
  h62copy->SetName("h62copy");
  /*  TCanvas *c2 = new TCanvas("c2", "c2");
  c2->Divide(2,2);
  c2->cd(1);  
  h50->Draw();
  c2->cd(2);
  h62copy->Draw();
  c2->cd(3);
  h62->SetLineColor(2);
  h62->Scale(scle);
  h50->Draw();
  h62->Draw("same");
  c2->cd(4);
  h50->Draw();
  h62->Draw("same"); 
  h65->SetLineColor(3);
  h65->Draw("same");   */
  
  double scl = h51->Integral(14,24) / h63->Integral(14,24);
  cout << h51->Integral(14,24) <<  " "  << h63->Integral(14,24) <<  " "  << scl << endl;
  TH1D *h63copy = (TH1D*)h63->Clone(); 
  h63copy->SetName("h63copy");
  /*  TCanvas *c3 = new TCanvas("c3", "c3");
  c3->Divide(2,2);
  c3->cd(1);  
  h51->Draw();
  c3->cd(2);
  h63copy->Draw();
  c3->cd(3);
  h63->SetLineColor(2);
  h63->Scale(scl);
  h51->Draw();
  h63->Draw("same");
  c3->cd(4);
  h51->Draw();
  h63->Draw("same"); 
  h66->SetLineColor(3);
  h66->Draw("same");  */
  
  TCanvas *c1 = new TCanvas("c1", "c1");
  //c1->Divide(3,1);
  //c1->Divide(2,1);
  /* c1->cd(1);
  h61->SetLineColor(2);
  h61->Scale(scale);
  h64->SetLineColor(3);
  h49->Draw();
  h61->Draw("same"); 
  h64->Draw("same");
  h67->SetLineColor(4);
  h70->SetLineColor(5);
  h73->SetLineColor(6);
  h67->Draw("same");
  h70->Draw("same");
  h73->Draw("same");*/
  //h84->SetLineColor(7);
  //h84->Draw("same");
  //c1->cd(1);
  
  h62->SetLineColor(2);
  h62->Scale(scle);
  
  TH1D *hDiff = (TH1D*)h50->Clone(); 
  hDiff->SetName("hDiff");
  for ( Int_t i = 1; i <= h50->GetNbinsX(); i++  ){
    cout << "h50->GetBinContent(i) = " << h50->GetBinContent(i) <<  "h62->GetBinContent(i) = " << h62->GetBinContent(i) << endl;
    double binContent = h50->GetBinContent(i) - h62->GetBinContent(i);
    cout << "binContent = " << binContent  << endl;
    if ( binContent < 0  ) binContent = 0;
    hDiff->SetBinContent(i,binContent);
    binContent = 0;
  }  
  
  h65->SetLineColor(3);
  h50->Draw();
  h62->Draw("same"); 
  h65->Draw("same"); 
  h68->SetLineColor(4);
  h71->SetLineColor(5);
  h74->SetLineColor(6);
  hDiff->SetLineColor(7);
  h68->Draw("same");
  h71->Draw("same");
  h74->Draw("same");
  hDiff->Draw("same");
  
  
  
  legg = new TLegend(0.5,0.5,0.9,0.9);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.04); legg->SetTextFont(132); 
  //legg->SetHeader("negative muons");
  legge = legg->AddEntry(h50, "x_b - Ups1S[9.4-9.5] ","l"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(h62, "x_b - Sideband[8.7-9.1]","l"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(hDiff, "Difference btw black and red","l"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(h65, "Gen Matched x_b - Ups1S","l"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(h68, "Gen Matched x_b0 - Ups1S","l"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(h71, "Gen Matched x_b1 - Ups1S","l"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(h74, "Gen Matched x_b2 - Ups1S","l"); legge->SetTextColor(kBlack);
  legg->Draw();
  
  // h85->SetLineColor(7);
  //h85->Draw("same");
  TCanvas *c3 = new TCanvas("c3", "c3");
  //c1->cd(2);
  h63->SetLineColor(2);
  h63->Scale(scl);
  
  TH1D *hDiffe = (TH1D*)h51->Clone(); 
  hDiffe->SetName("hDiffe");
  for ( Int_t i = 1; i <= h51->GetNbinsX(); i++  ){
    cout << "h51->GetBinContent(i) = " << h51->GetBinContent(i) <<  "h63->GetBinContent(i) = " << h63->GetBinContent(i) << endl;
    double binContent = h51->GetBinContent(i) - h63->GetBinContent(i);
    cout << "binContent = " << binContent  << endl;
    if ( binContent < 0  ) binContent = 0;
    hDiffe->SetBinContent(i,binContent);
    binContent = 0;
  }  
  
  h66->SetLineColor(3); 
  h51->Draw();
  h63->Draw("same"); 
  h66->Draw("same");
  h69->SetLineColor(4);
  h72->SetLineColor(5);
  h75->SetLineColor(6);
  hDiffe->SetLineColor(7);
  h69->Draw("same");
  h72->Draw("same");
  h75->Draw("same");
  hDiffe->Draw("same");
  //h86->SetLineColor(7);
  //h86->Draw("same");
  
  legg1 = new TLegend(0.5,0.5,0.9,0.9);
  legg1->SetFillStyle(0); legg1->SetBorderSize(0); legg1->SetTextSize(0.04); legg1->SetTextFont(132); 
  //legg->SetHeader("negative muons");
  legge1 = legg1->AddEntry(h51, "x_b - Ups1S[9.4-9.5] ","l"); legge->SetTextColor(kBlack);
  legge1 = legg1->AddEntry(h63, "x_b - Sideband[8.7-9.1]","l"); legge->SetTextColor(kBlack);
  legge1 = legg1->AddEntry(hDiffe, "Difference btw black and red","l"); legge->SetTextColor(kBlack);
  legge1 = legg1->AddEntry(h66, "Gen Matched x_b - Ups1S","l"); legge->SetTextColor(kBlack);
  legge1 = legg1->AddEntry(h69, "Gen Matched x_b0 - Ups1S","l"); legge->SetTextColor(kBlack);
  legge1 = legg1->AddEntry(h72, "Gen Matched x_b1 - Ups1S","l"); legge->SetTextColor(kBlack);
  legge1 = legg1->AddEntry(h75, "Gen Matched x_b2 - Ups1S","l"); legge->SetTextColor(kBlack);
  legg1->Draw();
  
  /*  TCanvas *c1 = new TCanvas("c1", "c1");
  c1->Divide(2,2);
  c1->cd(1);
  h10->Draw();
  c1->cd(2);
  h20->Draw();
  c1->cd(3);
  h14->Draw();
  c1->cd(4);
  h24->Draw();
  TCanvas *c2 = new TCanvas("c2", "c2");
  c2->Divide(2,2);
  c2->cd(1);
  h11->Draw();
  c2->cd(2);
  h21->Draw();
  c2->cd(3);
  h15->Draw();
  c2->cd(4);
  h25->Draw();  
  TCanvas *c3 = new TCanvas("c3", "c3");
  c3->Divide(2,2);
  c3->cd(1);
  h12->Draw();
  c3->cd(2);
  h22->Draw();  
  c3->cd(3);
  h13->Draw();
  c3->cd(4);
  h23->Draw();   
  TCanvas *c4 = new TCanvas("c4", "c4");
  c4->Divide(2,2);
  c4->cd(1);
  h16->Draw();
  c4->cd(2);
  h26->Draw();  
  c4->cd(3);
  h17->Draw();
  c4->cd(4);
  h27->Draw();  
  TCanvas *c5 = new TCanvas("c5", "c5");
  c5->Divide(2,2);
  c5->cd(1);
  h21->Draw();
  c5->cd(2);
  h25->Draw();  
  c5->cd(3);
  h26->Draw();
  c5->cd(4);
  h27->Draw();
  TCanvas *c6 = new TCanvas("c6", "c6");
  c6->Divide(2,2);
  c6->cd(1);
  h20->Draw();
  c6->cd(2);
  h24->Draw();  
  c6->cd(3);
  h22->Draw();
  c6->cd(4);
  h23->Draw();  
  TCanvas *c7 = new TCanvas("c7", "c7");
  c7->Divide(2,2);
  c7->cd(1);
  h28->Draw();
  c7->cd(2);
  h29->Draw();  
  c7->cd(3);
  h30->Draw();
  c7->cd(4);
  h31->Draw();  
  TCanvas *c8 = new TCanvas("c8", "c8");
  c8->Divide(2,2);
  c8->cd(1);
  h32->Draw();
  c8->cd(2);
  h33->Draw();  
  c8->cd(3);
  h34->Draw();
  c8->cd(4);
  h35->Draw();   
  TCanvas *c9 = new TCanvas("c9", "c9");
  c9->Divide(2,2);
  c9->cd(1);
  h36->Draw();
  c9->cd(2);
  h37->Draw();  
  c9->cd(3);
  h38->Draw();
  c9->cd(4);
  h39->Draw();   
  TCanvas *c10 = new TCanvas("c10", "c10");
  c10->Divide(2,2);
  c10->cd(1);
  h40->Draw();
  c10->cd(2);
  h41->Draw();  
  c10->cd(3);
  h42->Draw();
  c10->cd(4);
  h43->Draw();  
  TCanvas *c11 = new TCanvas("c11", "c11");
  c11->Divide(2,2);
  c11->cd(1);
  h44->Draw();
  c11->cd(2);
  h45->Draw();  
  c11->cd(3);
  h46->Draw();
  c11->cd(4);
  h47->Draw();   
  TCanvas *c12 = new TCanvas("c12", "c12");
  c12->Divide(2,2);
  c12->cd(1);
  h48->Draw();
  c12->cd(2);
  h49->Draw();  
  c12->cd(3);
  h50->Draw();
  c12->cd(4);
  h51->Draw();
  TCanvas *c13 = new TCanvas("c13", "c13");
  c13->Divide(2,2);
  c13->cd(1);
  h52->Draw();
  c13->cd(2);
  h53->Draw();  
  c13->cd(3);
  h54->Draw();
  c13->cd(4);
  h55->Draw();
  TCanvas *c14 = new TCanvas("c14", "c14");
  c14->Divide(2,2);
  c14->cd(1);
  h56->Draw();
  c14->cd(2);
  h57->Draw();  
  c14->cd(3);
  h58->Draw();
  c14->cd(4);
  h59->Draw();  
  TCanvas *c15 = new TCanvas("c15", "c15");
  c15->Divide(2,2);
  c15->cd(1);
  h60->Draw();
  c15->cd(2);
  h61->Draw();  
  c15->cd(3);
  h62->Draw();
  c15->cd(4);
  h63->Draw();   */
  
  
  
  
  
  
}

void Fit(){

  TFile *f = new TFile("test-xb.xsReader.default.root");
  TF1 *f1 = new TF1("f1", f_Gauss, 0.2, 0.6, 3);
  f1->SetParNames("Area 1", "Peak 1", "Sigma 1");
  f1->SetParameters(20, 0.45, 0.1);
  TH1D *mass;
  mass = (TH1D*)gFile->Get("h5");
  mass->Fit(f1);
}

void FitAll(){

  TFile *f = new TFile("test-xb.xsReader.default.root");
  TF1 *f0 = new TF1("f0", f_p1a3G, 0.2, 0.6, 9);
  f0->SetParNames("Area 1", "Peak 1", "Sigma 1", 
		  "Area 2", "Peak 2", "Sigma 2", 
		  "Area 3", "Peak 3", "Sigma 3"); 
  TH1D *mass;
  mass = (TH1D*)gFile->Get("h2");
  mass->Draw();
  f0->SetParameters(2., 0.40, 0.07, 6.4, 0.43, 0.08, 17, 0.09, 0.45);
  f0->SetParLimits(0, 0., 2.);
  //f0->FixParameter(0, 2.);
  f0->FixParameter(1, 0.40);
  //f0->SetParLimits(2, 0.02, 0.12);
  f0->FixParameter(2, 0.08);
  f0->FixParameter(4, 0.43);
  //f0->SetParLimits(5, 0.02, 0.12);
  f0->FixParameter(5, 0.07); 
  f0->FixParameter(7, 0.45);
  //f0->SetParLimits(8, 0.02, 0.12);
  f0->FixParameter(8, 0.09); 
    
  mass->Fit(f0);


  
}


// ----------------------------------------------------------------------
double f_p1(double *x, double *par) {
  return par[0] + par[1]*x[0]; 
}

// ----------------------------------------------------------------------
double f_Gauss(double *x, double *par) {
  // par[0] -> area
  // par[1] -> mean
  // par[2] -> sigma

  double sqrt2pi = 2.506628275;

  if (par[2] > 0.) {
    Double_t arg = (x[0] - par[1]) / par[2];
    Double_t fitval =  (par[0]/(sqrt2pi*par[2])) * TMath::Exp(-0.5*arg*arg);
    return fitval;
  }
  else {
    return -1.;
  }
}

// ----------------------------------------------------------------------
// pol1 and 3 Gauss 
double f_p1a3G(double *x, double *par) {
  //   par[0] = area of gaussian
  //   par[1] = mean of gaussian
  //   par[2] = sigma of gaussian
  //   par[3] = par 0 of pol1
  //   par[4] = par 1 of pol1
  //return  (f_p1(x, &par[9]) + f_Gauss(x, &par[0]) + f_Gauss(x, &par[3]) + f_Gauss(x, &par[6]));
  return  (f_Gauss(x, &par[0]) + f_Gauss(x, &par[3]) + f_Gauss(x, &par[6]));
}
