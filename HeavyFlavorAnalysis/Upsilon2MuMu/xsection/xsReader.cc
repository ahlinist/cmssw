#include "xsReader.hh"
#include <vector>
#include "TRandom.h"
#define MMUON 0.10566
#define MKAON 0.49368

// ----------------------------------------------------------------------
// Run with: ./runXSReaders -c chains/bg-test -D root 
//           ./runXSReaders -f test.root 
// ----------------------------------------------------------------------
const int  xsReader::fNpt;
const int  xsReader::fNy;

// ----------------------------------------------------------------------
xsReader::xsReader(TChain *tree, TString evtClassName): treeReaderXS(tree, evtClassName) {
  cout << "--> xsReader> This is the start ..." << endl;
  fpJSON = new JSON("/shome/bora/root/json/json_147196_149442");
  fPTbin[0] = 0.; fPTbin[1] = 2.; fPTbin[2] = 3.; fPTbin[3] = 5.; fPTbin[4] = 8.; fPTbin[5] =12.; fPTbin[6] = 20.;
  fYbin[0] = -2.; fYbin[1] = -1.; fYbin[2] = 0.; fYbin[3] = 1.; fYbin[4] = 2.;
  fPidTableMuIDPos = new PidTable("../tnp/PidTables/MC/Jpsi/MuID/CowboyVeto/PtMmbPos-jpsi.dat");
  fPidTableMuIDNeg = new PidTable("../tnp/PidTables/MC/Jpsi/MuID/CowboyVeto/PtMmbNeg-jpsi.dat");
  //fPidTableMuIDPos = new PidTable("../tnp/PidTables/MC/Jpsi/MuID/NoVeto/PtMmbPos-jpsi.dat");
  //fPidTableMuIDNeg = new PidTable("../tnp/PidTables/MC/Jpsi/MuID/NoVeto/PtMmbNeg-jpsi.dat");  
  fPidTableTrigPos = new PidTable("../tnp/PidTables/MC/Jpsi/Trig/CowboyVeto/PtMmbPos-jpsi.dat");   // MC 
  fPidTableTrigNeg = new PidTable("../tnp/PidTables/MC/Jpsi/Trig/CowboyVeto/PtMmbNeg-jpsi.dat"); 
  
  //fPidTableMuIDPos = new PidTable("../tnp/PidTables/DATA/Jpsi/MuID/CowboyVeto/PtMmbPos-jpsi.dat");
  //fPidTableMuIDNeg = new PidTable("../tnp/PidTables/DATA/Jpsi/MuID/CowboyVeto/PtMmbNeg-jpsi.dat");
  //fPidTableTrigPos = new PidTable("../tnp/PidTables/DATA/Jpsi/Trig/MuOnia/CowboyVeto/PtMmbBoth-jpsi.dat");     // DATA
  //fPidTableTrigNeg = new PidTable("../tnp/PidTables/DATA/Jpsi/Trig/MuOnia/CowboyVeto/PtMmbBoth-jpsi.dat"); 
  
}
// ----------------------------------------------------------------------
xsReader::~xsReader() {
  cout << "--> xsReader> This is the end ..." << endl;
}

// ----------------------------------------------------------------------
void xsReader::startAnalysis() {
  cout << "--> xsReader> startAnalysis: ..." << endl;
}


void xsReader::eventProcessing() {
  
  if ( MODE == 2  ) { 
    if ( !fpJSON->good(fRun, fLS) ) goto end;
  }
  
  ////// Trigger Check Study
  // candidateSelection(2);
  //if ( 0 != fpCand  ){
  //   trigEffCheck();
  //}
  
  //GenStudy();
  
  if ( MODE == 1  ) {
    acceptance();
    preSelEff();
  }
  
  //CandidateSelection(2);
  if ( !MuIDCheck() ) goto end;
  //calculateWeights(1);
  if ( isPathPreScaled(HLTPATH) ) goto end;
  if ( !isPathFired_Match(HLTPATH,HLTLABEL) ) goto end;
  //calculateWeights(2);
  candidateSelection(2);
  
  if ( 0 != fpCand  ){
    calculateWeights(0); 
    fillCandHist(); 
  }
  
  if ( 0 != fgCand && MODE == 1 ) MCstudy(); 
  fpHistFile->cd();
  
  end:
  freePointers();  
  
}

void xsReader::freePointers(){
  while (!Cands.empty())
    {
      // remove it from the list
      Cands.erase(Cands.begin());
    }
  
  while (!Cands_ID.empty())
    {
      // remove it from the list
      Cands_ID.erase(Cands_ID.begin());
    } 
  
  while (!Cands_TM.empty())
    {
      // remove it from the list
      Cands_TM.erase(Cands_TM.begin());
    }  
  
}

void xsReader::GenStudy(){
  
  TGenCand *gCand(0); 
  TGenCand *gDau1(0); TGenCand *gDau2(0);  
  double pt, rapidity; 
  TLorentzVector genCand;
  for (int iG = 0; iG < fpEvt->nGenCands(); ++iG) {
    gCand = fpEvt->getGenCand(iG);
    if ( gCand->fID == RESTYPE && gCand->fStatus == 2 ){
      genCand.SetPtEtaPhiE(gCand->fP.Perp(),gCand->fP.Eta(),gCand->fP.Phi(),gCand->fP.Energy());
      if ( (gCand->fP.Perp() <= 20.) && (fabs(genCand.Rapidity()) <= 2.) ){
	gDau1 = fpEvt->getGenCand(gCand->fDau1);
	gDau2 = fpEvt->getGenCand(gCand->fDau2);
	
	if ( ((fabs(gDau1->fID)) == 13) && ((fabs(gDau2->fID)) == 13) ){
	  ((TH1D*)fpHistFile->Get("GenStudy_MuonPt"))->Fill(gDau1->fP.Perp());
	  ((TH1D*)fpHistFile->Get("GenStudy_MuonPt"))->Fill(gDau2->fP.Perp());
	
	  if ( (gDau1->fP.Perp() < 1.) || (gDau2->fP.Perp() < 1.) ){
	    
	    cout << "genCand.Rapidity() = "<< genCand.Rapidity() << "gCand->fP.Perp() = " << gCand->fP.Perp() << endl;
	    ((TH2D*)fpHistFile->Get("GenStudy_Cand"))->Fill(genCand.Rapidity(),gCand->fP.Perp());
	    
	  }
	}
	
      }
    }
  }
  
}


void xsReader::trigEffCheck(){
  
  TLorentzVector CAND;
  CAND.SetPtEtaPhiM(fpCand->fPlab.Perp(),fpCand->fPlab.Eta(),fpCand->fPlab.Phi(),fpCand->fMass);
  ((TH2D*)fpHistFile->Get("TriggerCheck_2Reco"))->Fill(CAND.Rapidity(),fpCand->fPlab.Perp());
  
  if ( isPathFired(HLTPATH) ){
    ((TH2D*)fpHistFile->Get("TriggerCheck_Fired"))->Fill(CAND.Rapidity(),fpCand->fPlab.Perp());
  }
  
}

void xsReader::x_btest(){
  TGenCand *gCand(0);
  TGenCand *gDau1(0);
  TGenCand *gDau2(0);
  TLorentzVector genCand;
  int u1(0); int xb0(0); int xb1(0); int xb2(0);
  
  ((TH1D*)fpHistFile->Get("h"))->GetXaxis()->SetBinLabel(2, Form("Upsilon1S"));
  ((TH1D*)fpHistFile->Get("h"))->GetXaxis()->SetBinLabel(4, Form("xb_0")); 
  ((TH1D*)fpHistFile->Get("h"))->GetXaxis()->SetBinLabel(6, Form("xb_1"));
  ((TH1D*)fpHistFile->Get("h"))->GetXaxis()->SetBinLabel(8, Form("xb_2"));
  
  for (int iG = 0; iG < fpEvt->nGenCands(); ++iG) {
    gCand = fpEvt->getGenCand(iG);
    
    if ( gCand->fID == 553 && gCand->fStatus == 2 ){
      u1++;
      genCand.SetPtEtaPhiE(gCand->fP.Perp(),gCand->fP.Eta(),gCand->fP.Phi(),gCand->fP.Energy());
      ((TH1D*)fpHistFile->Get("h0"))->Fill(genCand.M());
    }
    
    if ( gCand->fID == 10551 && gCand->fStatus == 2 ){
      xb0++;
      genCand.SetPtEtaPhiE(gCand->fP.Perp(),gCand->fP.Eta(),gCand->fP.Phi(),gCand->fP.Energy());
      ((TH1D*)fpHistFile->Get("h0"))->Fill(genCand.M());
      gDau1 = fpEvt->getGenCand(gCand->fDau1);
      gDau2 = fpEvt->getGenCand(gCand->fDau2);
      if ( gDau1->fID == 22 && gDau2->fID == 553 ) {
	((TH1D*)fpHistFile->Get("h1"))->Fill(gDau1->fP.Energy());
      }
      if ( gDau2->fID == 22 && gDau1->fID == 553 ) {
	((TH1D*)fpHistFile->Get("h1"))->Fill(gDau2->fP.Energy());
      }
    
    }
    
    if ( gCand->fID == 20553 && gCand->fStatus == 2 ){
      xb1++;
      genCand.SetPtEtaPhiE(gCand->fP.Perp(),gCand->fP.Eta(),gCand->fP.Phi(),gCand->fP.Energy());
      ((TH1D*)fpHistFile->Get("h0"))->Fill(genCand.M());
      gDau1 = fpEvt->getGenCand(gCand->fDau1);
      gDau2 = fpEvt->getGenCand(gCand->fDau2);
      if ( gDau1->fID == 22 && gDau2->fID == 553 ) {
	((TH1D*)fpHistFile->Get("h1"))->Fill(gDau1->fP.Energy());
      }
      if ( gDau2->fID == 22 && gDau1->fID == 553 ) {
	((TH1D*)fpHistFile->Get("h1"))->Fill(gDau2->fP.Energy());
      }
      
    }     
    if ( gCand->fID == 555 && gCand->fStatus == 2 ){
      xb2++;
      genCand.SetPtEtaPhiE(gCand->fP.Perp(),gCand->fP.Eta(),gCand->fP.Phi(),gCand->fP.Energy());
      ((TH1D*)fpHistFile->Get("h0"))->Fill(genCand.M());
      gDau1 = fpEvt->getGenCand(gCand->fDau1);
      gDau2 = fpEvt->getGenCand(gCand->fDau2);
      if ( gDau1->fID == 22 && gDau2->fID == 553 ) {
	((TH1D*)fpHistFile->Get("h1"))->Fill(gDau1->fP.Energy());
      }
      if ( gDau2->fID == 22 && gDau1->fID == 553 ) {
	((TH1D*)fpHistFile->Get("h1"))->Fill(gDau2->fP.Energy());
      }
    }     
  }
  
    
  ((TH1D*)fpHistFile->Get("h"))->AddBinContent(2, u1);
  ((TH1D*)fpHistFile->Get("h"))->AddBinContent(4, xb0); 
  ((TH1D*)fpHistFile->Get("h"))->AddBinContent(6, xb1);
  ((TH1D*)fpHistFile->Get("h"))->AddBinContent(8, xb2);
  
  TAnaCand *pCand(0);
  TAnaTrack *pTrack(0);
  TLorentzVector Cand, Gamma, photon, x_b, xbg_b;
  int n(-1);
  Cand.SetPtEtaPhiM(fpCand->fPlab.Perp(),fpCand->fPlab.Eta(),fpCand->fPlab.Phi(),fpCand->fMass);
  for (int iG = 0; iG < fpEvt->nRecTracks(); ++iG) {
    pTrack = fpEvt->getRecTrack(iG);
    if ( pTrack->fMCID == 22 && pTrack->fQ == 0  ){
      ++n;
      TGenCand  *gGamma = fpEvt->getGenCand(pTrack->fGenIndex);
      TGenCand  *genCand = fpEvt->getGenCand(gGamma->fMom1);
      if ( genCand->fID == 555 || genCand->fID == 20553 || genCand->fID == 10551 ) {
	Gamma.SetPtEtaPhiM(pTrack->fPlab.Perp(),pTrack->fPlab.Eta(),pTrack->fPlab.Phi(), 0.);
	TAnaTrack *pl1 = fpEvt->getSigTrack(fpCand->fSig1); 
	TAnaTrack *pl2 = fpEvt->getSigTrack(fpCand->fSig2);
	if ( (pl1->fGenIndex > -1) && (pl2->fGenIndex > -1) && (pl1->fGenIndex != pl2->fGenIndex) ){
	  TGenCand  *gl1 = fpEvt->getGenCand(pl1->fGenIndex);
	  TGenCand  *gl2 = fpEvt->getGenCand(pl2->fGenIndex);
	  if ( gl1->fMom1 == gl2->fMom1 ) {
	    TGenCand  *GenCand = fpEvt->getGenCand(gl1->fMom1);
	    if ( GenCand->fID == RESTYPE ) {
	      m_ge = pTrack->fPlab.Eta();
	      m_gp = pTrack->fPlab.Phi();
	      m_gP = pTrack->fPlab.Perp();
	      m_gE = Gamma.E();
	      m_ue = fpCand->fPlab.Eta();
	      m_up = fpCand->fPlab.Phi();
	      m_uP = fpCand->fPlab.Perp();
	      m_um = Cand.M();
	      double deltaR = Gamma.DeltaR(Cand);
	      m_dR = deltaR;
	      x_b = Cand + Gamma;
	      m_xbm = x_b.M();
	      m_xbid = genCand->fID;
	      fTree->Fill();
	      ((TH1D*)fpHistFile->Get("h2"))->Fill(x_b.M()-Cand.M());
	      if ( genCand->fID == 555 ) ((TH1D*)fpHistFile->Get("h3"))->Fill(x_b.M()-Cand.M());
	      if ( genCand->fID == 10551 ) ((TH1D*)fpHistFile->Get("h4"))->Fill(x_b.M()-Cand.M());
	      if ( genCand->fID == 20553 ) ((TH1D*)fpHistFile->Get("h5"))->Fill(x_b.M()-Cand.M());
	      
	    }
	  }
	  
	}
      }
      
      if ( !(genCand->fID == 555 || genCand->fID == 20553 || genCand->fID == 10551) ) {
	photon.SetPtEtaPhiM(pTrack->fPlab.Perp(),pTrack->fPlab.Eta(),pTrack->fPlab.Phi(), 0.);
	mbg_ge = pTrack->fPlab.Eta();
	mbg_gp = pTrack->fPlab.Phi();
	mbg_gP = pTrack->fPlab.Perp();
	mbg_gE = photon.E();
	mbg_ue = fpCand->fPlab.Eta();
	mbg_up = fpCand->fPlab.Phi();
	mbg_uP = fpCand->fPlab.Perp();
	mbg_um = Cand.M();
	double dltaR = photon.DeltaR(Cand);
	mbg_dR = dltaR;
	xbg_b = Cand + photon;
	mbg_xbm = xbg_b.M();
	fTree2->Fill();
      }
    }
  }
  
  
  ((TH1D*)fpHistFile->Get("h6"))->Fill(fpCand->fMass);
  ((TH1D*)fpHistFile->Get("n"))->Fill(n);
  TAnaTrack *pTrack1(0);
  TLorentzVector gamma, x_B;
  for (int iG = 0; iG < fpEvt->nRecTracks(); ++iG) {
    pTrack1 = fpEvt->getRecTrack(iG);
    //if ( pTrack1->fMCID == 22 && pTrack1->fQ == 0  ){
    if ( pTrack1->fQ == 0  ){  
      gamma.SetPtEtaPhiM(pTrack1->fPlab.Perp(),pTrack1->fPlab.Eta(),pTrack1->fPlab.Phi(), 0.);
      ge = pTrack->fPlab.Eta();
      gp = pTrack->fPlab.Phi();
      gP = pTrack->fPlab.Perp();
      gE = gamma.E();
      double deltar = gamma.DeltaR(Cand);
      dR = deltar;
      x_B = Cand + gamma;
      xbm = x_B.M();
      ue = fpCand->fPlab.Eta();
      up = fpCand->fPlab.Phi();
      uP = fpCand->fPlab.Perp();
      um = Cand.M();
      fTree1->Fill();
    }
  }
  
}

void xsReader::TriggerComparisonStudy(){
  
  for (int iC = 0; iC < fpEvt->nCands(); ++iC) {
    fpCand = fpEvt->getCand(iC);
    if ( fpCand->fMass > 9.7 ) continue;
    if ( fpCand->fMass < 9.2 ) continue;
    TAnaTrack *p1 = fpEvt->getSigTrack(fpCand->fSig1); 
    TAnaTrack *p2 = fpEvt->getSigTrack(fpCand->fSig2);
    if ( (p1->fMuID & MUTYPE1) != MUTYPE1 ) continue;
    if ( (p2->fMuID & MUTYPE1) != MUTYPE1 ) continue;
    if ( p1->fPlab.Eta() > ETAHI ) continue;
    if ( p1->fPlab.Eta() < ETALO ) continue;
    if ( p2->fPlab.Eta() > ETAHI ) continue;
    if ( p2->fPlab.Eta() < ETALO ) continue;
    if ( p1->fPlab.Perp() < 3.0 ) continue;
    if ( p2->fPlab.Perp() < 3.0 ) continue;
    calculateWeights(0);
  }
    
  TAnaTrack *pTrack(0); TAnaTrack *pTrack2(0);
  TLorentzVector first; TLorentzVector second; TLorentzVector cand;
  bool First = false; bool Second = false;
  for (int iC = 0; iC < fpEvt->nRecTracks(); ++iC) {
    pTrack = fpEvt->getRecTrack(iC);
    if ( pTrack->fPlab.Perp() < 3.0 ) continue;
    if ( pTrack->fPlab.Eta() > ETAHI ) continue;
    if ( pTrack->fPlab.Eta() < ETALO ) continue;
    if ( (pTrack->fMuID & MUTYPE1) != MUTYPE1 ) continue;
    if ( !First ) {
      first.SetPtEtaPhiM(pTrack->fPlab.Perp(),pTrack->fPlab.Eta(),pTrack->fPlab.Phi(),MMUON);
      First = true;
      continue;
    }
    if ( First && !Second ) {
      second.SetPtEtaPhiM(pTrack->fPlab.Perp(),pTrack->fPlab.Eta(),pTrack->fPlab.Phi(),MMUON);
      Second = true;
    }
    if ( First && Second ){
      cand = first + second;
      if ( cand.M() > 9.7 ) continue;
      if ( cand.M() < 9.2 ) continue;
      ((TH1D*)fpHistFile->Get("TriggerStudy_2Reco"))->Fill(cand.Rapidity(),cand.Pt());
      if ( isPathFired(HLTPATH) ) ((TH1D*)fpHistFile->Get("TriggerStudy_Fired"))->Fill(cand.Rapidity(),cand.Pt());
    }
  }
}

void xsReader::PathStudy(){
  
  int n(0), m15(0), m13(0), m11(0);
  int v1(0), v2(0), v3(0);
  for (int a = 0; a < NHLT ; ++a) {
    if ( fpEvt->fHLTNames[a] ==  HLTPATH  && fpEvt->fHLTResult[a] == 1 ) {
      n++;
      //cout << " Fired !!!   "   << fpEvt->fHLTNames[a] << endl;
      
      // for (int c = 0; c < NHLT ; ++c) {
      //if ( fpEvt->fHLTResult[c] == 1 ) cout  << fpEvt->fHLTNames[c] << endl;
      //}
      
      for (int b = 0; b < NHLT ; ++b) {
	if (  fpEvt->fHLTNames[b] ==  HLTPATH1  &&  fpEvt->fHLTResult[b] == 1 ) m15++;
	if (  fpEvt->fHLTNames[b] ==  HLTPATH2  &&  fpEvt->fHLTResult[b] == 1 ) m13++;
	if (  fpEvt->fHLTNames[b] ==  HLTPATH3  &&  fpEvt->fHLTResult[b] == 1 ) m11++;
	if (  fpEvt->fHLTNames[b] == "HLT_HT160U_v1" &&  fpEvt->fHLTResult[b] == 1 ) v1++;
	if (  fpEvt->fHLTNames[b] == "HLT_HT160U_v2" &&  fpEvt->fHLTResult[b] == 1 ) v2++;
	if (  fpEvt->fHLTNames[b] == "HLT_HT160U_v3" &&  fpEvt->fHLTResult[b] == 1 ) v3++;
      }
    }
  }
  
  ((TH1D*)fpHistFile->Get("hTriggerStudy"))->GetXaxis()->SetBinLabel(2, Form("HLT_DoubleMu0_Quarkonium_v1"));
  ((TH1D*)fpHistFile->Get("hTriggerStudy"))->GetXaxis()->SetBinLabel(4, Form("HLT_Mu15_v1"));
  ((TH1D*)fpHistFile->Get("hTriggerStudy"))->GetXaxis()->SetBinLabel(6, Form("HLT_Mu13_v1"));
  ((TH1D*)fpHistFile->Get("hTriggerStudy"))->GetXaxis()->SetBinLabel(8, Form("HLT_Mu11"));
  ((TH1D*)fpHistFile->Get("hTriggerStudy"))->GetXaxis()->SetBinLabel(10, Form("HLT_HT160U_v1"));
  ((TH1D*)fpHistFile->Get("hTriggerStudy"))->GetXaxis()->SetBinLabel(12, Form("HLT_HT160U_v2"));
  ((TH1D*)fpHistFile->Get("hTriggerStudy"))->GetXaxis()->SetBinLabel(14, Form("HLT_HT160U_v3"));
  ((TH1D*)fpHistFile->Get("hTriggerStudy"))->AddBinContent(2, n);
  ((TH1D*)fpHistFile->Get("hTriggerStudy"))->AddBinContent(4, m15);  
  ((TH1D*)fpHistFile->Get("hTriggerStudy"))->AddBinContent(6, m13); 
  ((TH1D*)fpHistFile->Get("hTriggerStudy"))->AddBinContent(8, m11);
  ((TH1D*)fpHistFile->Get("hTriggerStudy"))->AddBinContent(10, v1);  
  ((TH1D*)fpHistFile->Get("hTriggerStudy"))->AddBinContent(12, v2); 
  ((TH1D*)fpHistFile->Get("hTriggerStudy"))->AddBinContent(14, v3);  
  
}

void xsReader::acceptance(){
  
  TGenCand *gCand(0); TAnaTrack *pTrack(0); 
  TGenCand *gDau1(0); TGenCand *gDau2(0);  
  double pt, rapidity; bool match1 = false; bool match2 = false;
  double pt1(-1.), pt2(-1.);
  double eta1(-99.), eta2(-99);
  int index1(-1), index2(-1); 
  TLorentzVector genCand; TAnaMuon *pMuon;
  TGenCand *g2Cand;
  int m(0);
  for (int iG = 0; iG < fpEvt->nGenCands(); ++iG) {
    gCand = fpEvt->getGenCand(iG);
    if ( gCand->fID == RESTYPE && gCand->fStatus == 2 ){
      genCand.SetPtEtaPhiE(gCand->fP.Perp(),gCand->fP.Eta(),gCand->fP.Phi(),gCand->fP.Energy());
      if ( (gCand->fP.Perp() <= 20.) && (fabs(genCand.Rapidity()) <= 2.) ){
	//getBinCenters(gCand, pt ,rapidity);
	//((TH2D*)fpHistFile->Get(Form("AllGenRes_%.1dS",UPSTYPE)))->Fill(rapidity, pt);
	((TH2D*)fpHistFile->Get(Form("AllGenRes_%.1dS",UPSTYPE)))->Fill(genCand.Rapidity(), gCand->fP.Perp());
	for (int i = gCand->fDau1; i <= gCand->fDau2; ++i) {
	  g2Cand = fpEvt->getGenCand(i);
	  if (13 == TMath::Abs(g2Cand->fID)) {
	    for (int iR = 0; iR < fpEvt->nRecTracks(); ++iR) {
	      pTrack = fpEvt->getRecTrack(iR);
	      if ( pTrack->fGenIndex == g2Cand->fNumber && !(match1) ) {
		index1 = pTrack->fGenIndex;
		pt1 = pTrack->fPlab.Perp();
		eta1 = pTrack->fPlab.Eta();
		//cout << "    g2Cand->fNumber = " << g2Cand->fNumber << endl;
		match1 = true;
		break;
	      } 	      
		  
	      if ( pTrack->fGenIndex == g2Cand->fNumber  ) {
		index2 = pTrack->fGenIndex;
		pt2 = pTrack->fPlab.Perp();
		eta2 = pTrack->fPlab.Eta();
		//cout << "g2Cand->fNumber = " << g2Cand->fNumber << endl;
		match2 = true;
		break;
	      }	  
	      
	    }
	  }
	  
	} 
	
	if ( match1 && match2 ){
	  if ((pt1 >= 3.) && (pt2 >= 3.) && (eta1 >= -2.4) && (eta2 >= -2.4) && (eta1 <= 2.4) && (eta2 <= 2.4) && (pt1 <= 20.) && (pt2 <= 20.) ){
	    ////////
	    if ( ((TMath::Abs(eta1) <= 1.2) && (pt1 < 4.)) || ((TMath::Abs(eta2) <= 1.2) && (pt2 < 4.)) ){
	      match1 = false;
	      match2 = false;
	      continue;
	    }
	    ////////
	    //((TH2D*)fpHistFile->Get(Form("RecoGenRes_%.1dS",UPSTYPE)))->Fill(rapidity, pt);
	    ((TH2D*)fpHistFile->Get(Form("RecoGenRes_%.1dS",UPSTYPE)))->Fill(genCand.Rapidity(), gCand->fP.Perp());
	    match1 = false; match2 = false;
	    //cout <<"pt1 = "<<pt1<<" pt2 = "<<pt2<<" eta1 = "<<eta1<<" eta2 = "<<eta2<< endl;
	    
	  }
	}
	
      }
    }
  }
}

void xsReader::preSelEff(){
  
  TAnaCand *pCand;
  TLorentzVector Cand;
  TLorentzVector Muon1, Muon2, Composite;
  double pt, rapidity;
  double Pt, Rapidity;
  TAnaMuon *pMuon; TGenCand *g2Cand;
  TGenCand *gCand; TLorentzVector genCand; TLorentzVector GenCand; 
  TGenCand *gPar1; TGenCand *gPar2; TGenCand *gCAND;
  TAnaTrack *pTrack;
  bool match1 = false; bool match2 = false;
  bool Match1 = false; bool Match2 = false;
  
  double pt1(-1.), pt2(-1.);
  double eta1(-1.), eta2(-1.);
  for (int i = 0; i < fpEvt->nCands(); ++i) {
    pCand = fpEvt->getCand(i);
    Cand.SetPtEtaPhiM(pCand->fPlab.Perp(),pCand->fPlab.Eta(),pCand->fPlab.Phi(),pCand->fMass);
    if ( (pCand->fPlab.Perp() <= 20.) && (fabs(Cand.Rapidity()) <= 2.) ){
      TAnaTrack *pl1 = fpEvt->getSigTrack(pCand->fSig1); 
      TAnaTrack *pl2 = fpEvt->getSigTrack(pCand->fSig2);
      if ( pl1->fGenIndex > -1 && pl2->fGenIndex > -1  ){
	gPar1 = fpEvt->getGenCand(pl1->fGenIndex);
	gPar2 = fpEvt->getGenCand(pl2->fGenIndex);
	if ( (13 == TMath::Abs(gPar1->fID)) && (13 == TMath::Abs(gPar2->fID)) ){
	  if ( (gPar1->fMom1 == gPar2->fMom1)  ){
	    gCAND = fpEvt->getGenCand(gPar1->fMom1);
	    if ( (gCAND->fID == RESTYPE) && (gCAND->fStatus == 2) ){
	      GenCand.SetPtEtaPhiE(gCAND->fP.Perp(),gCAND->fP.Eta(),gCAND->fP.Phi(),gCAND->fP.Energy());
	      if ( (gCAND->fP.Perp() <= 20.) && (fabs(GenCand.Rapidity()) <= 2.) ){
		if ( (pl1->fPlab.Perp() >= 3.) && (pl2->fPlab.Perp() >= 3.) && (pl1->fPlab.Eta() >= -2.4) && (pl2->fPlab.Eta() >= -2.4) && (pl1->fPlab.Eta() <= 2.4) && (pl2->fPlab.Eta() <= 2.4) && (pl1->fPlab.Perp() <= 20.) && (pl2->fPlab.Perp() <= 20.)  ){
		  ////////
		  if ( ((TMath::Abs(pl1->fPlab.Eta()) <= 1.2) && (pl1->fPlab.Perp() < 4.)) || ((TMath::Abs(pl2->fPlab.Eta()) <= 1.2) && (pl2->fPlab.Perp() < 4.)) ) continue;
		  ///////
		  if ( ((pl1->fMuID & MUTYPE1) == MUTYPE1) && ((pl2->fMuID & MUTYPE2) == MUTYPE2)){
		    ((TH2D*)fpHistFile->Get(Form("PreSel_afterVtx_%.1dS",UPSTYPE)))->Fill(GenCand.Rapidity(),gCAND->fP.Perp());
		  }
		}
	      }
	    }
	  }
	}
      }
    }
  }
  
  
      
  for (int iG = 0; iG < fpEvt->nGenCands(); ++iG) {
    gCand = fpEvt->getGenCand(iG);
    if ( gCand->fID == RESTYPE && gCand->fStatus == 2 ){
      genCand.SetPtEtaPhiE(gCand->fP.Perp(),gCand->fP.Eta(),gCand->fP.Phi(),gCand->fP.Energy());
      if ( (gCand->fP.Perp() <= 20.) && (fabs(genCand.Rapidity()) <= 2.) ){
	for (int i = gCand->fDau1; i <= gCand->fDau2; ++i) {
	  g2Cand = fpEvt->getGenCand(i);
	  if (13 == TMath::Abs(g2Cand->fID)) {
	    for (int j = 0; j < fpEvt->nMuons(); ++j) {
	      pMuon = fpEvt->getMuon(j);
	      if ( (pMuon->fIndex > -1) && ((pMuon->fMuID & MUTYPE1) == MUTYPE1) ){
	      //if ( (pMuon->fIndex > -1) ){
		pTrack = fpEvt->getRecTrack(pMuon->fIndex);
		if ( pTrack->fGenIndex == g2Cand->fNumber && !(match1) ) {
		  Muon1.SetPtEtaPhiM(pTrack->fPlab.Perp(),pTrack->fPlab.Eta(),pTrack->fPlab.Phi(),MMUON);
		  pt1 = pTrack->fPlab.Perp();
		  eta1 = pTrack->fPlab.Eta();
		  match1 = true;
		  break;
		} 	      
		
		if ( pTrack->fGenIndex == g2Cand->fNumber  ) {
		  Muon2.SetPtEtaPhiM(pTrack->fPlab.Perp(),pTrack->fPlab.Eta(),pTrack->fPlab.Phi(),MMUON);
		  pt2 = pTrack->fPlab.Perp();
		  eta2 = pTrack->fPlab.Eta();
		  match2 = true;
		  getBinCenters(gCand, Pt ,Rapidity);
		  break;
		}
	      }
	      //}
	    }
	  }
	}  
	
	
	if ( match1 && match2 ){
	  if ((pt1 >= 3.) && (pt2 >= 3.) && (eta1 >= -2.4) && (eta2 >= -2.4) && (eta1 <= 2.4) && (eta2 <= 2.4) && (pt1 <= 20.) && (pt2 <= 20.) ){
	    //////
	    if ( ((TMath::Abs(eta1) <= 1.2) && (pt1 < 4.)) || ((TMath::Abs(eta2) <= 1.2) && (pt2 < 4.)) ){
	      match1 = false;
	      match2 = false;
	      continue;
	    }
	    //////	  
	    ((TH2D*)fpHistFile->Get(Form("PreSel_beforeVtx_%.1dS",UPSTYPE)))->Fill(genCand.Rapidity(),gCand->fP.Perp());
	    match1 = false; match2 = false;
	    //cout <<"pt1 = "<<pt1<<" pt2 = "<<pt2<<" eta1 = "<<eta1<<" eta2 = "<<eta2<< endl;
	  }
	}
	
      }
    }
  }  
  
  
}


void xsReader::GetBINCenters(TLorentzVector Cand, double &pt, double &rapidity ){
  
  double ymin(-9), ymax(-9), ptmin(-9), ptmax(-9);
  for ( int iy = 0; iy <= fNy; ++iy ){
    if ( Cand.Rapidity() < fYbin[iy] ){
      ymax = fYbin[iy];
      ymin = fYbin[iy-1];
      break;
    }
  }
  
  for ( int ipt = 0; ipt <= fNpt; ++ipt ){
    if ( Cand.Pt() < fPTbin[ipt] ){
      ptmax = fPTbin[ipt];
      ptmin = fPTbin[ipt-1];
      break;
    }
  }
  
  pt = 0.5*(ptmax+ptmin);
  rapidity = 0.5*(ymax+ymin);
  
}



void xsReader::GetBinCenters(TAnaCand *pCand, double &pt, double &rapidity ){
  
  double ymin(-9), ymax(-9), ptmin(-9), ptmax(-9);
  TLorentzVector Cand;
  Cand.SetPtEtaPhiM(pCand->fPlab.Perp(),pCand->fPlab.Eta(),pCand->fPlab.Phi(),pCand->fMass);
  for ( int iy = 0; iy <= fNy; ++iy ){
    if ( Cand.Rapidity() < fYbin[iy] ){
      ymax = fYbin[iy];
      ymin = fYbin[iy-1];
      break;
    }
  }
  
  for ( int ipt = 0; ipt <= fNpt; ++ipt ){
    if ( pCand->fPlab.Perp() < fPTbin[ipt] ){
      ptmax = fPTbin[ipt];
      ptmin = fPTbin[ipt-1];
      break;
    }
  }
  
  pt = 0.5*(ptmax+ptmin);
  rapidity = 0.5*(ymax+ymin);
  
}


void xsReader::getBinCenters(TGenCand *gCand, double &pt, double &rapidity ){
  
  double ymin(-9), ymax(-9), ptmin(-9), ptmax(-9);
  TLorentzVector genCand;
  genCand.SetPtEtaPhiE(gCand->fP.Perp(),gCand->fP.Eta(),gCand->fP.Phi(),gCand->fP.Energy());
  for ( int iy = 0; iy <= fNy; ++iy ){
    if ( genCand.Rapidity() < fYbin[iy] ){
      ymax = fYbin[iy];
      ymin = fYbin[iy-1];
      break;
    }
  }
  
  for ( int ipt = 0; ipt <= fNpt; ++ipt ){
    if ( gCand->fP.Perp() < fPTbin[ipt] ){
      ptmax = fPTbin[ipt];
      ptmin = fPTbin[ipt-1];
      break;
    }
  }
  
  pt = 0.5*(ptmax+ptmin);
  rapidity = 0.5*(ymax+ymin);
  
}

bool xsReader::isPathPreScaled( TString Path ){
  bool PreScale = false;
  for (int a = 0; a < NHLT ; ++a) {
    if ( fpEvt->fHLTNames[a] ==  Path  && fpEvt->fHLTError[a] & 1  ) {
      PreScale = true;
      //cout << Path << " is prescaled!!!! "  << endl;
    }
  }
  return PreScale;
}

bool xsReader::isPathFired( TString Path ){
  bool HLT_Path = false;
  for (int a = 0; a < NHLT ; ++a) {
    if ( fpEvt->fHLTNames[a] ==  Path  && fpEvt->fHLTResult[a] == 1  ) {
      HLT_Path = true;
      //cout << Path << " fired!!!! "  << endl;
    }
  }
  return HLT_Path;
}  
  
  
  
bool xsReader::isPathFired_Match( TString Path, TString Label ){
  bool HLT_Path = false;
  bool Leg1 = false;
  bool Leg2 = false;
  TAnaCand *pCand(0);
  TAnaCand *pCand_(0);
  double rapidity, pt;
  bool Match = false;
  for (int iC = 0; iC < Cands.size() ; ++iC) {
    pCand = Cands[iC];
  //for (int iC = 0; iC < Cands_ID.size() ; ++iC) {   
  //pCand = Cands_ID[iC];
    TLorentzVector Candi;
    Candi.SetPtEtaPhiM(pCand->fPlab.Perp(),pCand->fPlab.Eta(),pCand->fPlab.Phi(),pCand->fMass);
    ((TH2D*)fpHistFile->Get(Form("TrigCheck_before_%.1dS",UPSTYPE)))->Fill(Candi.Rapidity(), pCand->fPlab.Perp());
  }
  
  for (int a = 0; a < NHLT ; ++a) {
    if ( fpEvt->fHLTNames[a] ==  Path  && fpEvt->fHLTResult[a] == 1  ) {
      HLT_Path = true;
      //cout << Path << " fired!!!! "  << endl;
    }
  }
  
  if ( HLT_Path  ){
    for (int iC = 0; iC < Cands.size() ; ++iC) {
      pCand_ = Cands[iC];
      //for (int iC = 0; iC < Cands_ID.size() ; ++iC) {  
      //pCand = Cands_ID[iC];
      TTrgObj *pTrig(0); TTrgObj *pTrig_(0); 
      int t(-1);
      TLorentzVector tagD;
      TAnaTrack *pTagD(0);
      pTagD = fpEvt->getSigTrack(pCand_->fSig1);
      tagD.SetPtEtaPhiM(pTagD->fPlab.Pt(), pTagD->fPlab.Eta(), pTagD->fPlab.Phi(), MMUON);
      for (int s = 0; s < fpEvt->nTrgObj() ; ++s) {
	pTrig = fpEvt->getTrgObj(s);
	if ( !(Label.CompareTo(pTrig->fLabel)) ) {
	  double tagD_dR = tagD.DeltaR(pTrig->fP);
	  double tagD_dEta = TMath::Abs(pTagD->fPlab.Eta() - pTrig->fP.Eta());
	  double tagD_dPhi = TMath::Abs(pTagD->fPlab.Phi() - pTrig->fP.Phi());
	  if ( ( tagD_dPhi < DPHI ) && ( tagD_dEta < DETA )) {
	    Leg1 = true;				
	    //cout << " Leg1 matched to Double mu T.O.  " << endl;
	    t=s;
	    break;
	  } 
	}
      }
      
      TLorentzVector probe;
      TAnaTrack *pProbe(0);
      pProbe = fpEvt->getSigTrack(pCand_->fSig2);
      probe.SetPtEtaPhiM(pProbe->fPlab.Pt(), pProbe->fPlab.Eta(), pProbe->fPlab.Phi(), MMUON);
      for (int i = 0; i < fpEvt->nTrgObj() ; ++i) {
	if ( i == t ) continue;
	pTrig_ = fpEvt->getTrgObj(i);
	if ( !(Label.CompareTo(pTrig_->fLabel)) ) {
	  double probe_dR = probe.DeltaR(pTrig_->fP);
	  double probe_dEta = TMath::Abs(pProbe->fPlab.Eta() - pTrig_->fP.Eta());
	  double probe_dPhi = TMath::Abs(pProbe->fPlab.Phi() - pTrig_->fP.Phi());
	  if ( ( probe_dPhi < DPHI ) && ( probe_dEta < DETA )) {
	    Leg2 = true;
	    //cout << " Leg2 matched to Double mu T.O. " << endl;
	    break;
	  }
	}
      }
    } 
  }
  
  if ( Leg1 && Leg2 ){
    Match = true;
    //fpCand = pCand_;
    //fCandPt   = fpCand->fPlab.Perp();
    //fCandMass = fpCand->fMass;
    TLorentzVector Cand;
    Cand.SetPtEtaPhiM(pCand_->fPlab.Perp(),pCand_->fPlab.Eta(),pCand_->fPlab.Phi(),pCand_->fMass);
    //fCandY = Cand.Rapidity();
    Cands_TM.push_back(pCand_);
    ((TH2D*)fpHistFile->Get(Form("TrigCheck_after_%.1dS",UPSTYPE)))->Fill(Cand.Rapidity(), pCand_->fPlab.Perp() );
    //cout << " Match " << endl;
  }
    
  
  return Match;
}

bool xsReader::MuIDCheck(){
  bool check = false;
  TAnaCand *pCand(0);
  double rapidity, pt;
  TLorentzVector Cand;
  TAnaTrack *pP1(0); TAnaTrack *pP2(0);
  for (int iC = 0; iC < fpEvt->nCands(); ++iC) {
    pCand = fpEvt->getCand(iC);
    //for (int iC = 0; iC < Cands.size() ; ++iC) {
    //pCand = Cands[iC];
    Cand.SetPtEtaPhiM(pCand->fPlab.Perp(),pCand->fPlab.Eta(),pCand->fPlab.Phi(),pCand->fMass);
    if ( Cand.Rapidity() < -2.00 ) continue;
    if ( Cand.Rapidity() > 2.00 ) continue;
    if ( pCand->fPlab.Perp() < 0.00 ) continue;
    if ( pCand->fPlab.Perp() > 20.00 ) continue;
    ((TH2D*)fpHistFile->Get(Form("MuIDCheck_before_%.1dS",UPSTYPE)))->Fill(Cand.Rapidity(), pCand->fPlab.Perp());
    TAnaTrack *pl1 = fpEvt->getSigTrack(pCand->fSig1); 
    TAnaTrack *pl2 = fpEvt->getSigTrack(pCand->fSig2);
    if ( (pl1->fMuID & MUTYPE1) != MUTYPE1 ) continue;
    if ( (pl2->fMuID & MUTYPE2) != MUTYPE2 ) continue;
    Cands.push_back(pCand);
    //Cands_ID.push_back(pCand);
    ((TH2D*)fpHistFile->Get(Form("MuIDCheck_after_%.1dS",UPSTYPE)))->Fill(Cand.Rapidity(), pCand->fPlab.Perp()); 
    check = true;
  }
  return check;
}						

void xsReader::candidateSelection(int mode){

  fCandY = fCandPt = fCandMass = -1.; 
  fpCand = 0;
  fGenCandY = fGenCandPt = -1.;
  fgCand = 0;
  fMuon1Eta = fMuon1Pt = fMuon2Eta = fMuon2Pt = -1.;
  fGenMuon1Eta = fGenMuon1Pt = fGenMuon2Eta = fGenMuon2Pt = -1.;
  TAnaCand *pCand(0);
  vector<int> lCands, lCands_CT, lCands_CT_M1T, lCands_CT_M1T_M2T, lCands_CT_M1T_M2T_Pt1, lCands_CT_M1T_M2T_Pt1_Pt2, lCands_CT_M1T_M2T_Pt1_Pt2_CHI2; 

  for (int iC = 0; iC < Cands_TM.size() ; ++iC) {
    pCand = Cands_TM[iC];
    lCands.push_back(iC);
    TAnaTrack *pl1 = fpEvt->getSigTrack(pCand->fSig1); 
    TAnaTrack *pl2 = fpEvt->getSigTrack(pCand->fSig2);
    if (TYPE != pCand->fType) continue;
    lCands_CT.push_back(iC);
    if ( (pl1->fPlab.Perp() > PTHI) || (pl1->fPlab.Perp() < PTLO) ) continue;
    if ( (pl1->fPlab.Eta() > ETAHI) || (pl1->fPlab.Eta() < ETALO) ) continue;
    ///////
    if ( ((TMath::Abs(pl1->fPlab.Eta()) <= 1.2) && (pl1->fPlab.Perp() < 4.)) || ((TMath::Abs(pl2->fPlab.Eta()) <= 1.2) && (pl2->fPlab.Perp() < 4.)) ) continue;
    ///////
    lCands_CT_M1T_M2T_Pt1.push_back(iC);
    if ( (pl2->fPlab.Perp() > PTHI) || (pl2->fPlab.Perp() < PTLO) ) continue;
    if ( (pl2->fPlab.Eta() > ETAHI) || (pl2->fPlab.Eta() < ETALO) ) continue;
    lCands_CT_M1T_M2T_Pt1_Pt2.push_back(iC);
    //if ( pCand->fVtx.fChi2 > CHI2 ) continue;
    if ( MODE == 1 ) AnaEff(pCand, 1);
    if ( pl1->fQ*pl2->fQ > 0 ) continue;
    if (pCand->fMass < MASSLO) continue;
    if (pCand->fMass > MASSHI) continue;
    lCands_CT_M1T_M2T_Pt1_Pt2_CHI2.push_back(iC);
  }
  
    
  int nc(lCands.size()); int nc_CT(lCands_CT.size()); 
  int nc_CT_M1T_M2T_Pt1(lCands_CT_M1T_M2T_Pt1.size()); int nc_CT_M1T_M2T_Pt1_Pt2(lCands_CT_M1T_M2T_Pt1_Pt2.size());
  int nc_CT_M1T_M2T_Pt1_Pt2_CHI2(lCands_CT_M1T_M2T_Pt1_Pt2_CHI2.size());
  ((TH1D*)fpHistFile->Get("n2"))->Fill(nc); 
  ((TH1D*)fpHistFile->Get("n2_CandType"))->Fill(nc_CT);
  ((TH1D*)fpHistFile->Get("n2_CandType_MuType1&2_Pt1"))->Fill(nc_CT_M1T_M2T_Pt1);
  ((TH1D*)fpHistFile->Get("n2_CandType_MuType1&2_Pt1&2"))->Fill(nc_CT_M1T_M2T_Pt1_Pt2);
  ((TH1D*)fpHistFile->Get("n2_CandType_MuType1&2_Pt1&2_Chi2"))->Fill(nc_CT_M1T_M2T_Pt1_Pt2_CHI2);
  
  ((TH1D*)fpHistFile->Get("n2_cuts"))->AddBinContent(10, nc);
  ((TH1D*)fpHistFile->Get("n2_cuts"))->GetXaxis()->SetBinLabel(10, "NOCUT");
  ((TH1D*)fpHistFile->Get("n2_cuts"))->AddBinContent(16, nc_CT);
  ((TH1D*)fpHistFile->Get("n2_cuts"))->GetXaxis()->SetBinLabel(12, Form("Type^{Cand}=%d",TYPE));
  ((TH1D*)fpHistFile->Get("n2_cuts"))->AddBinContent(18, nc_CT_M1T_M2T_Pt1);
  ((TH1D*)fpHistFile->Get("n2_cuts"))->GetXaxis()->SetBinLabel(18, Form("p_{T}^{#mu_{1}}=%.1f",PTLO));
  ((TH1D*)fpHistFile->Get("n2_cuts"))->AddBinContent(20, nc_CT_M1T_M2T_Pt1_Pt2);
  ((TH1D*)fpHistFile->Get("n2_cuts"))->GetXaxis()->SetBinLabel(20, Form("p_{T}^{#mu_{2}}=%.1f",PTLO));
  ((TH1D*)fpHistFile->Get("n2_cuts"))->AddBinContent(22, nc_CT_M1T_M2T_Pt1_Pt2_CHI2);
  ((TH1D*)fpHistFile->Get("n2_cuts"))->GetXaxis()->SetBinLabel(22, Form("#chi^{2}<%.1f",CHI2));

  
  if (0 == nc_CT_M1T_M2T_Pt1_Pt2_CHI2) return; 
  int best(-1);
  if (nc_CT_M1T_M2T_Pt1_Pt2_CHI2 > 1) {
    cout << "MORE THAN ONE CANDIDATE  " << nc_CT_M1T_M2T_Pt1_Pt2_CHI2   <<  endl;
    double ptMax(0.), pt(0.);
    double maxDocaMax(99.), maxDoca(0.);
    double chi2Max(99.), chi2(0.);
    for (unsigned int iC = 0; iC < lCands_CT_M1T_M2T_Pt1_Pt2_CHI2.size(); ++iC) {
      pCand = fpEvt->getCand(lCands_CT_M1T_M2T_Pt1_Pt2_CHI2[iC]);       
      if ( mode == 1 ){
	pt = pCand->fPlab.Perp(); 
	if (pt > ptMax) {
	  best = lCands_CT_M1T_M2T_Pt1_Pt2_CHI2[iC]; 
	  ptMax = pt;
	}
      } else if ( mode == 2 ){
	maxDoca = pCand->fMaxDoca;
	if (maxDoca < maxDocaMax) {
	  best = lCands_CT_M1T_M2T_Pt1_Pt2_CHI2[iC]; 
	  maxDocaMax = maxDoca;
	  }
      } else if ( mode == 3 ){
	chi2 = pCand->fVtx.fChi2;
	  if (chi2 < chi2Max) {
	    best = lCands_CT_M1T_M2T_Pt1_Pt2_CHI2[iC]; 
	    chi2Max = chi2;
	  }
      } 	
    }
  } else if (nc_CT_M1T_M2T_Pt1_Pt2_CHI2 == 1) {best = lCands_CT_M1T_M2T_Pt1_Pt2_CHI2[0];}
  
  int truth(0);
  TLorentzVector Cand, gCand;
  if ( best > -1 ) {
    fpCand = Cands_TM[best];
    if ( MODE == 1 ) AnaEff(fpCand, 2);
    fCandPt   = fpCand->fPlab.Perp();
    fCandMass = fpCand->fMass;
    Cand.SetPtEtaPhiM(fpCand->fPlab.Perp(),fpCand->fPlab.Eta(),fpCand->fPlab.Phi(),fpCand->fMass);
    fCandY = Cand.Rapidity();
    TAnaTrack *pl1 = fpEvt->getSigTrack(fpCand->fSig1); 
    TAnaTrack *pl2 = fpEvt->getSigTrack(fpCand->fSig2);
    fMuon1Pt = pl1->fPlab.Perp(); fMuon2Pt = pl2->fPlab.Perp(); 
    fMuon1Eta = pl1->fPlab.Eta(); fMuon2Eta = pl2->fPlab.Eta();
    if ( (pl1->fGenIndex > -1) && (pl2->fGenIndex > -1) && (pl1->fGenIndex != pl2->fGenIndex) ){
      TGenCand  *gl1 = fpEvt->getGenCand(pl1->fGenIndex);
      TGenCand  *gl2 = fpEvt->getGenCand(pl2->fGenIndex);
      if ( (gl1->fMom1 == gl2->fMom1) && gl2->fMom1 > -1  ) {
	TGenCand  *genCand = fpEvt->getGenCand(gl1->fMom1);
	((TH1D*)fpHistFile->Get("TruthCand"))->Fill(genCand->fID);
	if ( genCand->fID == RESTYPE ) {
	  truth++;
	  fgCand = fpEvt->getGenCand(gl1->fMom1); 
	  fGenCandPt   = fgCand->fP.Perp();
	  gCand.SetPtEtaPhiE(fgCand->fP.Perp(),fgCand->fP.Eta(),fgCand->fP.Phi(),fgCand->fP.Energy());
	  fGenCandY = gCand.Rapidity();
	  fGenMuon1Pt = gl1->fP.Perp(); fGenMuon2Pt = gl2->fP.Perp(); 
	  fGenMuon1Eta = gl1->fP.Eta(); fGenMuon2Eta = gl2->fP.Eta(); 
	}
	((TH1D*)fpHistFile->Get("n2_cuts"))->AddBinContent(2, truth);
	((TH1D*)fpHistFile->Get("n2_cuts"))->GetXaxis()->SetBinLabel(2, Form("TruthCand"));	
      }
    }
  }
  
  
}

void xsReader::CandidateSelection(int mode){

  fCandY = fCandPt = fCandMass = -1.; 
  fpCand = 0;
  fGenCandY = fGenCandPt = -1.;
  fgCand = 0;
  fMuon1Eta = fMuon1Pt = fMuon2Eta = fMuon2Pt = -1.;
  fGenMuon1Eta = fGenMuon1Pt = fGenMuon2Eta = fGenMuon2Pt = -1.;
  TAnaCand *pCand(0);
  vector<int> lCands, lCands_CT, lCands_CT_M1T, lCands_CT_M1T_M2T, lCands_CT_M1T_M2T_Pt1, lCands_CT_M1T_M2T_Pt1_Pt2, lCands_CT_M1T_M2T_Pt1_Pt2_CHI2; 
  
  for (int iC = 0; iC < fpEvt->nCands() ; ++iC) {
    pCand = fpEvt->getCand(iC);
    lCands.push_back(iC);
    TAnaTrack *pl1 = fpEvt->getSigTrack(pCand->fSig1); 
    TAnaTrack *pl2 = fpEvt->getSigTrack(pCand->fSig2);
    if ( MODE == 1 ) AnaEff(pCand, 1);
    if ( pl1->fQ*pl2->fQ > 0 ) continue;
    if (TYPE != pCand->fType) continue;
    if (pCand->fMass < MASSLO) continue;
    if (pCand->fMass > MASSHI) continue;
    lCands_CT.push_back(iC);
    if ( (pl1->fPlab.Perp() > PTHI) || (pl1->fPlab.Perp() < PTLO) ) continue;
    if ( (pl1->fPlab.Eta() > ETAHI) || (pl1->fPlab.Eta() < ETALO) ) continue;
    lCands_CT_M1T_M2T_Pt1.push_back(iC);
    if ( (pl2->fPlab.Perp() > PTHI) || (pl2->fPlab.Perp() < PTLO) ) continue;
    if ( (pl2->fPlab.Eta() > ETAHI) || (pl2->fPlab.Eta() < ETALO) ) continue;
    lCands_CT_M1T_M2T_Pt1_Pt2.push_back(iC);
    //if ( pCand->fVtx.fChi2 > CHI2 ) continue;
    lCands_CT_M1T_M2T_Pt1_Pt2_CHI2.push_back(iC);
  }
  
    
  if (0 == lCands_CT_M1T_M2T_Pt1_Pt2_CHI2.size()) return; 
  int best(-1);
  if (lCands_CT_M1T_M2T_Pt1_Pt2_CHI2.size() > 1) {
    cout << "MORE THAN ONE CANDIDATE  " << lCands_CT_M1T_M2T_Pt1_Pt2_CHI2.size() <<  endl;
    double ptMax(0.), pt(0.);
    double maxDocaMax(99.), maxDoca(0.);
    double chi2Max(99.), chi2(0.);
    for (unsigned int iC = 0; iC < lCands_CT_M1T_M2T_Pt1_Pt2_CHI2.size(); ++iC) {
      pCand = fpEvt->getCand(lCands_CT_M1T_M2T_Pt1_Pt2_CHI2[iC]);       
      if ( mode == 1 ){
	pt = pCand->fPlab.Perp(); 
	if (pt > ptMax) {
	  best = lCands_CT_M1T_M2T_Pt1_Pt2_CHI2[iC]; 
	  ptMax = pt;
	}
      } else if ( mode == 2 ){
	maxDoca = pCand->fMaxDoca;
	if (maxDoca < maxDocaMax) {
	  best = lCands_CT_M1T_M2T_Pt1_Pt2_CHI2[iC]; 
	  maxDocaMax = maxDoca;
	  }
      } else if ( mode == 3 ){
	chi2 = pCand->fVtx.fChi2;
	  if (chi2 < chi2Max) {
	    best = lCands_CT_M1T_M2T_Pt1_Pt2_CHI2[iC]; 
	    chi2Max = chi2;
	  }
      } 	
    }
  } else if (lCands_CT_M1T_M2T_Pt1_Pt2_CHI2.size() == 1) {best = lCands_CT_M1T_M2T_Pt1_Pt2_CHI2[0];}
  
  int truth(0);
  TLorentzVector Cand, gCand;
  if ( best > -1 ) {
    pCand = fpEvt->getCand(best);
    Cands.push_back(pCand);
    if ( MODE == 1 ) AnaEff(pCand, 2);
  }
  
  
}



void xsReader::AnaEff(TAnaCand *pCand, int mode) {
  
  TLorentzVector Cand;
  int truth(0);
  TAnaTrack *pl1 = fpEvt->getSigTrack(pCand->fSig1); 
  TAnaTrack *pl2 = fpEvt->getSigTrack(pCand->fSig2);
  
  if ( (pl1->fGenIndex > -1) && (pl2->fGenIndex > -1) && (pl1->fGenIndex != pl2->fGenIndex)  ){
    TGenCand  *gl1 = fpEvt->getGenCand(pl1->fGenIndex);
    TGenCand  *gl2 = fpEvt->getGenCand(pl2->fGenIndex);
    if ( (gl1->fMom1 == gl2->fMom1) && ( gl1->fMom1 > -1 ) && ( gl2->fMom1 > -1 )  ) {
      TGenCand  *genCand = fpEvt->getGenCand(gl1->fMom1);
      if ( (genCand->fID == RESTYPE) && (genCand->fStatus == 2) ) {
	truth++;
      }
      
      if ( mode == 1 ) ((TH1D*)fpHistFile->Get(Form("AnaEff_%.1dS,OverAll", UPSTYPE)))->AddBinContent(2,truth);
      if ( mode == 2 ) ((TH1D*)fpHistFile->Get(Form("AnaEff_%.1dS,OverAll", UPSTYPE)))->AddBinContent(8,truth);
      
      Cand.SetPtEtaPhiM(pCand->fPlab.Perp(),pCand->fPlab.Eta(),pCand->fPlab.Phi(),pCand->fMass);
      for ( int iy = 0; iy < fNy; ++iy ){
	for ( int ipt = 0; ipt < fNpt; ++ipt ){
	  if( ( Cand.Rapidity() < fYbin[iy+1] ) && ( Cand.Rapidity() >= fYbin[iy] ) ){
	    if ( ( pCand->fPlab.Perp() >= fPTbin[ipt] ) && ( pCand->fPlab.Perp() < fPTbin[ipt+1] ) ){
	      if ( mode == 1 ) ((TH1D*)fpHistFile->Get(Form("AnaEff_%.1dS,rapidity%.1f_%.1f,pt%.1f_%.1f", UPSTYPE,  fYbin[iy], fYbin[iy+1], fPTbin[ipt], fPTbin[ipt+1])))->AddBinContent(2,truth);
	      if ( mode == 2 ) ((TH1D*)fpHistFile->Get(Form("AnaEff_%.1dS,rapidity%.1f_%.1f,pt%.1f_%.1f", UPSTYPE,  fYbin[iy], fYbin[iy+1], fPTbin[ipt], fPTbin[ipt+1])))->AddBinContent(8,truth);
	    }
	  }
	}
      }
      
    }	
  }
  
}
// ----------------------------------------------------------------------
void xsReader::fillCandHist() {
  
  ((TH1D*)fpHistFile->Get("CandMass"))->Fill(fCandMass,fWeight);
  ((TH1D*)fpHistFile->Get("CandPt"))->Fill(fCandPt,fWeight);
  ((TH1D*)fpHistFile->Get("CandRapidity"))->Fill(fCandY,fWeight);
  ((TH1D*)fpHistFile->Get("CandEta"))->Fill(fpCand->fPlab.Eta(),fWeight);
  ((TH1D*)fpHistFile->Get("UpsilonMass"))->Fill(fCandMass,fWeight);
  
  ((TH1D*)fpHistFile->Get("SigMuEta"))->Fill(fMuon1Eta);
  ((TH1D*)fpHistFile->Get("SigMuEta"))->Fill(fMuon2Eta);
  ((TH1D*)fpHistFile->Get("SigMuPt"))->Fill(fMuon1Pt);
  ((TH1D*)fpHistFile->Get("SigMuPt"))->Fill(fMuon2Pt);  
  
  ((TH1D*)fpHistFile->Get("SigMuEtaPt"))->Fill(fMuon1Eta,fMuon1Pt);  
  ((TH1D*)fpHistFile->Get("SigMuEtaPt"))->Fill(fMuon2Eta,fMuon2Pt);  
  
  for ( int iy = 0; iy < fNy; ++iy ){
    for ( int ipt = 0; ipt < fNpt; ++ipt ){
      if ( ( fCandY >= fYbin[iy] ) && ( fCandY < fYbin[iy+1] ) ){
	if ( ( fCandPt >= fPTbin[ipt] ) && ( fCandPt < fPTbin[ipt+1] ) ){
	  ((TH1D*)fpHistFile->Get(Form("UpsilonMass,rapidity%.1f_%.1f,pt%.1f_%.1f", fYbin[iy], fYbin[iy+1], fPTbin[ipt], fPTbin[ipt+1])))->Fill(fCandMass,fWeight);
	}
      }
    }
  }
}


// ----------------------------------------------------------------------
void xsReader::MCstudy(){
  
  double deltaPtCand(-99), deltaYCand(-99), deltaPtMuon1(-99), deltaEtaMuon1(-99), deltaPtMuon2(-99), deltaEtaMuon2(-99);
  ((TH2D*)fpHistFile->Get("PtResolution_Cand"))->Fill(fGenCandPt,fCandPt);
  ((TH2D*)fpHistFile->Get("PtResolution_Cand"))->GetXaxis()->SetTitle(Form("P_{T}^{GenCand}"));
  ((TH2D*)fpHistFile->Get("PtResolution_Cand"))->GetYaxis()->SetTitle(Form("P_{T}^{RecoCand}"));
  
  deltaPtCand = (fCandPt - fGenCandPt)/fGenCandPt;
  ((TH1D*)fpHistFile->Get("DeltaPtoverPt_Cand"))->Fill(deltaPtCand);
  
  ((TH2D*)fpHistFile->Get("YResolution_Cand"))->Fill(fGenCandY,fCandY);
  ((TH2D*)fpHistFile->Get("YResolution_Cand"))->GetXaxis()->SetTitle(Form("Y^{GenCand}"));
  ((TH2D*)fpHistFile->Get("YResolution_Cand"))->GetYaxis()->SetTitle(Form("Y^{RecoCand}")); 
  
  deltaYCand = (fCandY - fGenCandY)/fGenCandY;
  ((TH1D*)fpHistFile->Get("DeltaYoverY_Cand"))->Fill(deltaYCand);  
  
  ((TH2D*)fpHistFile->Get("PtResolution_Muon"))->Fill(fGenMuon1Pt,fMuon1Pt);
  ((TH2D*)fpHistFile->Get("PtResolution_Muon"))->Fill(fGenMuon2Pt,fMuon2Pt);
  ((TH2D*)fpHistFile->Get("PtResolution_Muon"))->GetXaxis()->SetTitle(Form("P_{T}^{GenMuon}"));
  ((TH2D*)fpHistFile->Get("PtResolution_Muon"))->GetYaxis()->SetTitle(Form("P_{T}^{Recomuon}"));
  
  deltaPtMuon1 = (fMuon1Pt - fGenMuon1Pt)/fGenMuon1Pt;
  deltaPtMuon2 = (fMuon2Pt - fGenMuon2Pt)/fGenMuon2Pt;
  ((TH1D*)fpHistFile->Get("DeltaPtoverPt_Muon"))->Fill(deltaPtMuon1);
  ((TH1D*)fpHistFile->Get("DeltaPtoverPt_Muon"))->Fill(deltaPtMuon2);
  
  ((TH2D*)fpHistFile->Get("#etaResolution_Muon"))->Fill(fGenMuon1Eta,fMuon1Eta);
  ((TH2D*)fpHistFile->Get("#etaResolution_Muon"))->Fill(fGenMuon2Eta,fMuon2Eta);
  ((TH2D*)fpHistFile->Get("#etaResolution_Muon"))->GetXaxis()->SetTitle(Form("#eta^{GenMuon}"));
  ((TH2D*)fpHistFile->Get("#etaResolution_Muon"))->GetYaxis()->SetTitle(Form("#eta^{Recomuon}"));
  
  deltaEtaMuon1 = (fMuon1Eta - fGenMuon1Eta)/fGenMuon1Eta;
  deltaEtaMuon2 = (fMuon2Eta - fGenMuon2Eta)/fGenMuon2Eta;
  ((TH1D*)fpHistFile->Get("DeltaEtaoverEta_Muon"))->Fill(deltaEtaMuon1);
  ((TH1D*)fpHistFile->Get("DeltaEtaoverEta_Muon"))->Fill(deltaEtaMuon2);
  
  ((TH1D*)fpHistFile->Get("MaxDoca_Cand"))->Fill(fpCand->fMaxDoca);
  
}

// ----------------------------------------------------------------------
void xsReader::calculateWeights(int mode){
  double effID1(-99); double effID2(-99);
  double effTR1(-99); double effTR2(-99);
  double MuIdWeight(-99); double TrigWeight(-99);
  TAnaCand *pCand;
  TLorentzVector Cand;
  
  if ( mode == 0 ){
    TAnaTrack *pl1 = fpEvt->getSigTrack(fpCand->fSig1); 
    TAnaTrack *pl2 = fpEvt->getSigTrack(fpCand->fSig2);
    if ( pl1->fQ > 0 ){
      effID1 = fPidTableMuIDPos->effD(pl1->fPlab.Perp(), pl1->fPlab.Eta(), 0.);
      effTR1 = fPidTableTrigPos->effD(pl1->fPlab.Perp(), pl1->fPlab.Eta(), 0.);
      
    } else if ( pl1->fQ < 0 ){
      effID1 = fPidTableMuIDNeg->effD(pl1->fPlab.Perp(), pl1->fPlab.Eta(), 0.);
      effTR1 = fPidTableTrigNeg->effD(pl1->fPlab.Perp(), pl1->fPlab.Eta(), 0.);
    }
    
    if ( pl2->fQ > 0 ){
      effID2 = fPidTableMuIDPos->effD(pl2->fPlab.Perp(), pl2->fPlab.Eta(), 0.);
      effTR2 = fPidTableTrigPos->effD(pl2->fPlab.Perp(), pl2->fPlab.Eta(), 0.);
    }  else if ( pl2->fQ < 0 ){
      effID2 = fPidTableMuIDNeg->effD(pl2->fPlab.Perp(), pl2->fPlab.Eta(), 0.);
      effTR2 = fPidTableTrigNeg->effD(pl2->fPlab.Perp(), pl2->fPlab.Eta(), 0.);
    }
    
    //fWeight = 1/(effID1*effID2*effTR1*effTR2);
    
    fWeight = 1;
    
    MuIdWeight = effID1*effID2;
    TrigWeight = effTR1*effTR2;
    
    ((TH1D*)fpHistFile->Get(Form("MuIDEff_%.1dS,OverAll", UPSTYPE)))->Fill(MuIdWeight,1./MuIdWeight);
    ((TH1D*)fpHistFile->Get(Form("TrigEff_%.1dS,OverAll", UPSTYPE)))->Fill(TrigWeight,1./TrigWeight);
    
    for ( int iy = 0; iy < fNy; ++iy ){
      for ( int ipt = 0; ipt < fNpt; ++ipt ){
	if ( ( fCandY >= fYbin[iy] ) && ( fCandY < fYbin[iy+1] ) ){
	  if ( ( fCandPt >= fPTbin[ipt] ) && ( fCandPt < fPTbin[ipt+1] ) ){
	    ((TH1D*)fpHistFile->Get(Form("MuIDEff_%.1dS,rapidity%.1f_%.1f,pt%.1f_%.1f",UPSTYPE, fYbin[iy], fYbin[iy+1], fPTbin[ipt], fPTbin[ipt+1])))->Fill(MuIdWeight,1./MuIdWeight);
	    ((TH1D*)fpHistFile->Get(Form("TrigEff_%.1dS,rapidity%.1f_%.1f,pt%.1f_%.1f",UPSTYPE, fYbin[iy], fYbin[iy+1], fPTbin[ipt], fPTbin[ipt+1])))->Fill(TrigWeight,1./TrigWeight);
	  }
	}
      }
    }  
  }
  
  if ( mode == 1 ){
    for (int iC = 0; iC < Cands.size() ; ++iC) {
      pCand = Cands[iC];
      Cand.SetPtEtaPhiM(pCand->fPlab.Perp(),pCand->fPlab.Eta(),pCand->fPlab.Phi(),pCand->fMass);
      TAnaTrack *pl1 = fpEvt->getSigTrack(pCand->fSig1);
      TAnaTrack *pl2 = fpEvt->getSigTrack(pCand->fSig2);
      if ( pl1->fPlab.Perp() > 3. && pl1->fPlab.Perp() < 20. && pl2->fPlab.Perp() > 3. && pl2->fPlab.Perp() < 20. && pl1->fPlab.Eta() > -2.4 && pl1->fPlab.Eta() < 2.4  && pl2->fPlab.Eta() > -2.4 && pl2->fPlab.Eta() < 2.4 ){
      
	if ( pl1->fQ > 0 ){
	  effID1 = fPidTableMuIDPos->effD(pl1->fPlab.Perp(), pl1->fPlab.Eta(), 0.);
	} else if ( pl1->fQ < 0 ){
	  effID1 = fPidTableMuIDNeg->effD(pl1->fPlab.Perp(), pl1->fPlab.Eta(), 0.);
	}
	
	if ( pl2->fQ > 0 ){
	  effID2 = fPidTableMuIDPos->effD(pl2->fPlab.Perp(), pl2->fPlab.Eta(), 0.);
	}  else if ( pl2->fQ < 0 ){
	  effID2 = fPidTableMuIDNeg->effD(pl2->fPlab.Perp(), pl2->fPlab.Eta(), 0.);
	}
	
	fWeight = 1;
	
	MuIdWeight = effID1*effID2;
	((TH1D*)fpHistFile->Get(Form("MuIDEff_%.1dS,OverAll", UPSTYPE)))->Fill(MuIdWeight);
	for ( int iy = 0; iy < fNy; ++iy ){
	  for ( int ipt = 0; ipt < fNpt; ++ipt ){
	    if ( ( Cand.Rapidity() >= fYbin[iy] ) && ( Cand.Rapidity() < fYbin[iy+1] ) ){
	      if ( ( pCand->fPlab.Perp() >= fPTbin[ipt] ) && ( pCand->fPlab.Perp()  < fPTbin[ipt+1] ) ){
		((TH1D*)fpHistFile->Get(Form("MuIDEff_%.1dS,rapidity%.1f_%.1f,pt%.1f_%.1f",UPSTYPE, fYbin[iy], fYbin[iy+1], fPTbin[ipt], fPTbin[ipt+1])))->Fill(MuIdWeight,1./MuIdWeight);
	      }
	    }
	  }
	}
      }  
    }
  }
  
  if ( mode == 2 ){
    for (int iC = 0; iC < Cands_TM.size() ; ++iC) {
      pCand = Cands_TM[iC];
      Cand.SetPtEtaPhiM(pCand->fPlab.Perp(),pCand->fPlab.Eta(),pCand->fPlab.Phi(),pCand->fMass);
      TAnaTrack *pl1 = fpEvt->getSigTrack(pCand->fSig1);
      TAnaTrack *pl2 = fpEvt->getSigTrack(pCand->fSig2);
      if ( pl1->fPlab.Perp() > 3. && pl1->fPlab.Perp() < 20. && pl2->fPlab.Perp() > 3. && pl2->fPlab.Perp() < 20. && pl1->fPlab.Eta() > -2.4 && pl1->fPlab.Eta() < 2.4  && pl2->fPlab.Eta() > -2.4 && pl2->fPlab.Eta() < 2.4 ){
    
	if ( pl1->fQ > 0 ){
	  effTR1 = fPidTableTrigPos->effD(pl1->fPlab.Perp(), pl1->fPlab.Eta(), 0.);
	  
	} else if ( pl1->fQ < 0 ){
	  effTR1 = fPidTableTrigNeg->effD(pl1->fPlab.Perp(), pl1->fPlab.Eta(), 0.);
	}
      
	if ( pl2->fQ > 0 ){
	  effTR2 = fPidTableTrigPos->effD(pl2->fPlab.Perp(), pl2->fPlab.Eta(), 0.);
	}  else if ( pl2->fQ < 0 ){
	  effTR2 = fPidTableTrigNeg->effD(pl2->fPlab.Perp(), pl2->fPlab.Eta(), 0.);
	}
	
	fWeight = 1;
	
	TrigWeight = effTR1*effTR2;
	
	for ( int iy = 0; iy < fNy; ++iy ){
	  for ( int ipt = 0; ipt < fNpt; ++ipt ){
	    if ( ( Cand.Rapidity() >= fYbin[iy] ) && ( Cand.Rapidity() < fYbin[iy+1] ) ){
	      if ( ( pCand->fPlab.Perp() >= fPTbin[ipt] ) && ( pCand->fPlab.Perp() < fPTbin[ipt+1] ) ){
		((TH1D*)fpHistFile->Get(Form("TrigEff_%.1dS,rapidity%.1f_%.1f,pt%.1f_%.1f",UPSTYPE, fYbin[iy], fYbin[iy+1], fPTbin[ipt], fPTbin[ipt+1])))->Fill(TrigWeight,1./TrigWeight);
	      }
	    }
	  }
	}  
      }
    }
  }
  
}

// ----------------------------------------------------------------------
bool xsReader::isMatchedToTrig(TAnaTrack *pTag, TString Label){
	bool a=false;
	TTrgObj *pTrig;
	TLorentzVector tag;
	tag.SetPtEtaPhiM(pTag->fPlab.Pt(), pTag->fPlab.Eta(), pTag->fPlab.Phi(), 0.);
	((TH1D*)fpHistFile->Get("tag_pt"))->Fill(pTag->fPlab.Pt());
	for (int s = 0; s < fpEvt->nTrgObj() ; ++s) {
    		pTrig = fpEvt->getTrgObj(s);
		//cout << "pTrig->fLabel is " << pTrig->fLabel << endl;
		
		if ( !(Label.CompareTo(pTrig->fLabel)) ) {
			cout << "pTrig->fLabel is " << pTrig->fLabel << endl;;
			double dR = tag.DeltaR(pTrig->fP);
	        	double dEta = TMath::Abs(pTag->fPlab.Eta() - pTrig->fP.Eta());
			double dPhi = TMath::Abs(pTag->fPlab.Phi() - pTrig->fP.Phi());
			((TH1D*)fpHistFile->Get("trig_dR"))->Fill(dR);
			((TH1D*)fpHistFile->Get("trig_dEta"))->Fill(dEta);
			((TH1D*)fpHistFile->Get("trig_dPhi"))->Fill(dPhi);
		//	if ( (dPhi < 0.7) && (dPhi > 0.3) ) ((TH1D*)fpHistFile->Get("trig_dEta_aftercut"))->Fill(dEta);		// Used for J/Psi
			if ( (dPhi < 0.55) && (dPhi > 0.15) ) ((TH1D*)fpHistFile->Get("trig_dEta_aftercut"))->Fill(dEta);		// Usedfor Ups1S
			if ( (dPhi < 0.7) && (dPhi > 0.3) && (dEta < 0.14)) a=true;				
	
		}
  	}
	return a;
}

// ----------------------------------------------------------------------
void xsReader::fillHist() {


}

// ----------------------------------------------------------------------
void xsReader::bookHist() {
  
  fBin = BIN;
  fMassLow = MASSLO;
  fMassHigh = MASSHI;
  
  TH1 *h;
  TH2 *k;
 
  h = new TH1D("r101a", "(Matched) Muon -- Pt, Neg", fNpt, fPTbin);
  k = new TH2D("mt,pt-eta", "mt,pt-eta", fNpt, fPTbin, fNy, fYbin);
  k = new TH2D("mmmbar,pt-eta", "mmbar,pt-eta", fNpt, fPTbin, fNy, fYbin);
  cout << "--> xsReader> bookHist> " << endl;
  
  
  // Acceptance Histograms
  k = new TH2D(Form("AllGenRes_%1.dS",  UPSTYPE), Form("AllGenRes_%1.dS", UPSTYPE), fNy, fYbin, fNpt, fPTbin); 
  k = new TH2D(Form("RecoGenRes_%1.dS", UPSTYPE), Form("RecoGenRes_%1.dS", UPSTYPE), fNy, fYbin, fNpt, fPTbin); 
  ((TH2D*)fpHistFile->Get(Form("AllGenRes_%.1dS", UPSTYPE)))->Sumw2();
  ((TH2D*)fpHistFile->Get(Form("RecoGenRes_%.1dS", UPSTYPE)))->Sumw2();
  
  // PreSel Eff
  k = new TH2D(Form("PreSel_afterVtx_%.1dS",UPSTYPE), Form("PreSel_afterVtx_%1.dS", UPSTYPE), fNy, fYbin, fNpt, fPTbin);
  k = new TH2D(Form("PreSel_beforeVtx_%.1dS",UPSTYPE), Form("PreSel_beforeVtx_%1.dS", UPSTYPE), fNy, fYbin, fNpt, fPTbin);
  ((TH2D*)fpHistFile->Get(Form("PreSel_afterVtx_%.1dS", UPSTYPE)))->Sumw2();
  ((TH2D*)fpHistFile->Get(Form("PreSel_beforeVtx_%.1dS", UPSTYPE)))->Sumw2();  
  
  // MuID Check
  k = new TH2D(Form("MuIDCheck_after_%.1dS",UPSTYPE), Form("MuIDCheck_after_%1.dS", UPSTYPE), fNy, fYbin, fNpt, fPTbin);
  k = new TH2D(Form("MuIDCheck_before_%.1dS",UPSTYPE), Form("MuIDCheck_before_%1.dS", UPSTYPE), fNy, fYbin, fNpt, fPTbin);
  ((TH2D*)fpHistFile->Get(Form("MuIDCheck_after_%.1dS", UPSTYPE)))->Sumw2();
  ((TH2D*)fpHistFile->Get(Form("MuIDCheck_before_%.1dS", UPSTYPE)))->Sumw2();   
  
  // Trig Check
  k = new TH2D(Form("TrigCheck_after_%.1dS",UPSTYPE), Form("TrigCheck_after_%1.dS", UPSTYPE), fNy, fYbin, fNpt, fPTbin);
  k = new TH2D(Form("TrigCheck_before_%.1dS",UPSTYPE), Form("TrigCheck_before_%1.dS", UPSTYPE), fNy, fYbin, fNpt, fPTbin);
  ((TH2D*)fpHistFile->Get(Form("TrigCheck_after_%.1dS", UPSTYPE)))->Sumw2();
  ((TH2D*)fpHistFile->Get(Form("TrigCheck_before_%.1dS", UPSTYPE)))->Sumw2();     
  
  
  // candidateSelection() histograms
  h = new TH1D("n2_cuts", "n2_cuts", 100, 0., 100.);
  h = new TH1D("n2", "ncand", 20, 0, 20.);
  h = new TH1D("n2_CandType", "ncand_CandType", 20, 0, 20.);
  h = new TH1D("n2_CandType_MuType1", "ncand_CandType_MuType1", 20, 0, 20.);
  h = new TH1D("n2_CandType_MuType1&2", "ncand_CandType_MuType1&2", 20, 0, 20.);
  h = new TH1D("n2_CandType_MuType1&2_Pt1", "ncand_CandType_MuType1&2_Pt1", 20, 0, 20.);
  h = new TH1D("n2_CandType_MuType1&2_Pt1&2", "ncand_CandType_MuType1&2_Pt1&2", 20, 0, 20.);
  h = new TH1D("n2_CandType_MuType1&2_Pt1&2_Chi2", "ncand_CandType_MuType1&2_Pt1&2_Chi2", 20, 0, 20.);
  h = new TH1D("TruthCand", "TruthCand", 10, 550., 560.);
  
  
  // MuID Efficiency Histograms
  for ( int iy = 0; iy < fNy; ++iy ){
    for ( int ipt = 0; ipt < fNpt; ++ipt ){
      h = new TH1D(Form("MuIDEff_%.1dS,rapidity%.1f_%.1f,pt%.1f_%.1f", UPSTYPE, fYbin[iy], fYbin[iy+1], fPTbin[ipt], fPTbin[ipt+1]),
		   Form("MuIDEff_%.1dS,rapidity%.1f_%.1f,pt%.1f_%.1f", UPSTYPE, fYbin[iy], fYbin[iy+1], fPTbin[ipt], fPTbin[ipt+1]),
		   100, 0., 1.);  
      ((TH1D*)fpHistFile->Get(Form("MuIDEff_%.1dS,rapidity%.1f_%.1f,pt%.1f_%.1f", UPSTYPE, fYbin[iy], fYbin[iy+1], fPTbin[ipt], fPTbin[ipt+1])))->Sumw2(); 
    }
  }
  h = new TH1D(Form("MuIDEff_%.1dS,OverAll", UPSTYPE), Form("MuIDEff_%.1dS,OverAll", UPSTYPE), 100, 0., 1.);
  
  // Trig Efficiency Histograms
  for ( int iy = 0; iy < fNy; ++iy ){
    for ( int ipt = 0; ipt < fNpt; ++ipt ){
      h = new TH1D(Form("TrigEff_%.1dS,rapidity%.1f_%.1f,pt%.1f_%.1f", UPSTYPE, fYbin[iy], fYbin[iy+1], fPTbin[ipt], fPTbin[ipt+1]),
		   Form("TrigEff_%.1dS,rapidity%.1f_%.1f,pt%.1f_%.1f", UPSTYPE, fYbin[iy], fYbin[iy+1], fPTbin[ipt], fPTbin[ipt+1]),
		   100, 0., 1.);  
      ((TH1D*)fpHistFile->Get(Form("TrigEff_%.1dS,rapidity%.1f_%.1f,pt%.1f_%.1f", UPSTYPE, fYbin[iy], fYbin[iy+1], fPTbin[ipt], fPTbin[ipt+1])))->Sumw2(); 
    }
  }  
  h = new TH1D(Form("TrigEff_%.1dS,OverAll", UPSTYPE), Form("TrigEff_%.1dS,OverAll", UPSTYPE), 100, 0., 1.);
  
  // Analysis Efficiency Histograms
  for ( int iy = 0; iy < fNy; ++iy ){
    for ( int ipt = 0; ipt < fNpt; ++ipt ){
      h = new TH1D(Form("AnaEff_%.1dS,rapidity%.1f_%.1f,pt%.1f_%.1f", UPSTYPE, fYbin[iy], fYbin[iy+1], fPTbin[ipt], fPTbin[ipt+1]),
		   Form("AnaEff_%.1dS,rapidity%.1f_%.1f,pt%.1f_%.1f", UPSTYPE, fYbin[iy], fYbin[iy+1], fPTbin[ipt], fPTbin[ipt+1]),
		   10, 0., 10.);
      ((TH1D*)fpHistFile->Get(Form("AnaEff_%.1dS,rapidity%.1f_%.1f,pt%.1f_%.1f", UPSTYPE,  fYbin[iy], fYbin[iy+1], fPTbin[ipt], fPTbin[ipt+1])))->GetXaxis()->SetBinLabel(2, Form("TruthCand"));
      ((TH1D*)fpHistFile->Get(Form("AnaEff_%.1dS,rapidity%.1f_%.1f,pt%.1f_%.1f", UPSTYPE,  fYbin[iy], fYbin[iy+1], fPTbin[ipt], fPTbin[ipt+1])))->GetXaxis()->SetBinLabel(8, Form("TruthCand_AfterCuts"));
      ((TH1D*)fpHistFile->Get(Form("AnaEff_%.1dS,rapidity%.1f_%.1f,pt%.1f_%.1f", UPSTYPE,  fYbin[iy], fYbin[iy+1], fPTbin[ipt], fPTbin[ipt+1])))->Sumw2(); 
    }	
  }
  
  h = new TH1D(Form("AnaEff_%.1dS,OverAll", UPSTYPE), Form("AnaEff_%.1dS,OverAll", UPSTYPE), 10, 0., 10.);
  ((TH1D*)fpHistFile->Get(Form("AnaEff_%.1dS,OverAll", UPSTYPE)))->GetXaxis()->SetBinLabel(2, Form("TruthCand"));;
  ((TH1D*)fpHistFile->Get(Form("AnaEff_%.1dS,OverAll", UPSTYPE)))->GetXaxis()->SetBinLabel(8, Form("TruthCand_AfterCuts"));;
  ((TH1D*)fpHistFile->Get(Form("AnaEff_%.1dS,OverAll", UPSTYPE)))->Sumw2();
  
  
  // fillCandHist() histograms
  h = new TH1D("CandMass", "CandMass", 44, 1, 12.);
  h = new TH1D("CandPt", "CandPt", 80, 0, 40.);
  h = new TH1D("CandRapidity", "CandRapidity", 80, -4, 4.);
  h = new TH1D("CandEta", "CandEta", 80, -4, 4.);
  h = new TH1D("UpsilonMass", "UpsilonMass", BIN, fMassLow, fMassHigh); 
  h = new TH1D("SigMuEta", "SigMuEta", 80, -4, 4.);
  h = new TH1D("SigMuPt", "SigMuPt", 80, 0, 40.);
  k = new TH2D("SigMuEtaPt", "SigMuEtaPt", 48, -2.4, 2.4, 40, 0, 20);
  
  for ( int iy = 0; iy < fNy; ++iy ){
    for ( int ipt = 0; ipt < fNpt; ++ipt ){
      h = new TH1D(Form("UpsilonMass,rapidity%.1f_%.1f,pt%.1f_%.1f", fYbin[iy], fYbin[iy+1], fPTbin[ipt], fPTbin[ipt+1]),
		   Form("UpsilonMass,rapidity%.1f_%.1f,pt%.1f_%.1f", fYbin[iy], fYbin[iy+1], fPTbin[ipt], fPTbin[ipt+1]),
		   fBin, fMassLow, fMassHigh);
      ((TH1D*)fpHistFile->Get(Form("UpsilonMass,rapidity%.1f_%.1f,pt%.1f_%.1f",  fYbin[iy], fYbin[iy+1], fPTbin[ipt], fPTbin[ipt+1])))->Sumw2(); 
    }	
  }
  
  // MCstudy() histograms
  k = new TH2D("PtResolution_Cand", "PtResolution_Cand", 100, 0, 50, 100, 0, 50);
  k = new TH2D("YResolution_Cand", "YResolution_Cand", 100, -5, 5, 100, -5, 5);
  k = new TH2D("PtResolution_Muon", "PtResolution_Muon", 100, 0, 50, 100, 0, 50);
  k = new TH2D("#etaResolution_Muon", "#etaResolution_Muon", 80, -4, 4, 80, -4, 4);
  h = new TH1D("DeltaPtoverPt_Cand", "DeltaPtoverPt_Cand", 50, -0.1, 0.1);
  h = new TH1D("DeltaYoverY_Cand", "DeltaYoverY_Cand", 50, -0.05, 0.05);
  h = new TH1D("DeltaPtoverPt_Muon", "DeltaPtoverPt_Muon", 50, -0.1, 0.1);
  h = new TH1D("DeltaEtaoverEta_Muon", "DeltaEtaoverEta_Muon", 50, -0.05, 0.05); 
  h = new TH1D("MaxDoca_Cand", "MaxDoca_Cand", 60, 0., 0.03); 
  
  
  //// Trigger Study
  //////////////////////
  k = new TH2D("TriggerStudy_2Reco","TriggerStudy_2Reco", fNy, fYbin, fNpt, fPTbin);
  k = new TH2D("TriggerStudy_Fired","TriggerStudy_Fired", fNy, fYbin, fNpt, fPTbin);
  /////////////////////
  
  //// Trigger Check
  //////////////////////
  k = new TH2D("TriggerCheck_2Reco","TriggerCheck_2Reco", fNy, fYbin, fNpt, fPTbin);
  k = new TH2D("TriggerCheck_Fired","TriggerCheck_Fired", fNy, fYbin, fNpt, fPTbin);
  /////////////////////  
  
  // GenStudy
  h = new TH1D("GenStudy_MuonPt", "GenStudy_MuonPt", 80, 0, 40.);
  k = new TH2D("GenStudy_Cand","GenStudy_Cand", fNy, fYbin, fNpt, fPTbin);
  
  
  ///////////////////////////////////////////// ---- Reduced Tree
  fTree = new TTree("m", "m");
  fTree->Branch("m_um",  &m_um ,"m_um/F");
  fTree->Branch("m_uP",  &m_uP ,"m_uP/F");
  fTree->Branch("m_ue",  &m_ue ,"m_ue/F");
  fTree->Branch("m_up",  &m_up ,"m_up/F");
  fTree->Branch("m_gE",  &m_gE ,"m_gE/F");
  fTree->Branch("m_gP",  &m_gP ,"m_gP/F");
  fTree->Branch("m_ge",  &m_ge ,"m_ge/F");
  fTree->Branch("m_gp",  &m_gp ,"m_gp/F");
  fTree->Branch("m_xbm",  &m_xbm ,"m_xbm/F");
  fTree->Branch("m_xbid",  &m_xbid ,"m_xbid/I");
  fTree->Branch("m_dR",  &m_dR ,"m_dR/F");
  
  fTree1 = new TTree("nm", "nm");
  fTree1->Branch("um",  &um ,"um/F");
  fTree1->Branch("uP",  &uP ,"uP/F");
  fTree1->Branch("ue",  &ue ,"ue/F");
  fTree1->Branch("up",  &up ,"up/F");  
  fTree1->Branch("gE",  &gE ,"gE/F");
  fTree1->Branch("gP",  &gP ,"gP/F");
  fTree1->Branch("ge",  &ge ,"ge/F");
  fTree1->Branch("gp",  &gp ,"gp/F");
  fTree1->Branch("dR",  &dR ,"dR/F");
  fTree1->Branch("xbm",  &xbm ,"xbm/F");
  
  fTree2 = new TTree("mbg", "mbg");
  fTree2->Branch("mbg_um",  &mbg_um ,"mbg_um/F");
  fTree2->Branch("mbg_uP",  &mbg_uP ,"mbg_uP/F");
  fTree2->Branch("mbg_ue",  &mbg_ue ,"mbg_ue/F");
  fTree2->Branch("mbg_up",  &mbg_up ,"mbg_up/F");
  fTree2->Branch("mbg_gE",  &mbg_gE ,"mbg_gE/F");
  fTree2->Branch("mbg_gP",  &mbg_gP ,"mbg_gP/F");
  fTree2->Branch("mbg_ge",  &mbg_ge ,"mbg_ge/F");
  fTree2->Branch("mbg_gp",  &mbg_gp ,"mbg_gp/F");
  fTree2->Branch("mbg_xbm", &mbg_xbm ,"mbg_xbm/F");
  fTree2->Branch("mbg_dR",  &mbg_dR ,"mbg_dR/F");
    
  

  // x_b histos
  h = new TH1D("h","Histo", 10, 0., 10.);
  h = new TH1D("n","Number of Photons per Event", 100, 0., 100.);
  h = new TH1D("h0","Mass", 100, 9., 10.);
  h = new TH1D("h1","Photon Energy", 100, 0., 5.);
  h = new TH1D("h2","Mass_mumugamma", 40, 0.2, 0.6);
  h = new TH1D("h3","Mass_mumugamma x_b2", 40, 0.2, 0.6);
  h = new TH1D("h4","Mass_mumugamma x_b0", 40, 0.2, 0.6);
  h = new TH1D("h5","Mass_mumugamma x_b1", 40, 0.2, 0.6);
  h = new TH1D("h6","DiMuon Mass", 50, 8.7, 11.2);
  h = new TH1D("hGenAngle","hGenAngle", 1000, 0., 10.);
  h = new TH1D("hSgMatchedAngle","hSgMatchedAngle", 1000, 0., 10.);
  h = new TH1D("hBgMatchedAngle","hBgMatchedAngle", 1000, 0., 10.);
  h = new TH1D("hAngle","hAngle", 1000, 0., 10.);
  
 
  // trigger study
  h = new TH1D("hTriggerStudy","Trigger Study", 20, 0., 20.);
  
}


// --------------------------------------------------------------------------------------------------
void xsReader::readCuts(TString filename, int dump) {
  char  buffer[200];
  fCutFile = filename;
  if (dump) cout << "Reading " << fCutFile.Data() << " for xsReader cut settings" << endl;
  sprintf(buffer, "%s", fCutFile.Data());
  ifstream is(buffer);
  char CutName[100];
  char SetName[100];
  float CutValue;
  int ok(0);

  TString fn(fCutFile.Data());

  if (dump) {
    cout << "====================================" << endl;
    cout << "Cut file  " << fCutFile.Data() << endl;
    cout << "------------------------------------" << endl;
  }

  TH1D *hcuts = new TH1D("hcuts", "", 1000, 0., 1000.);
  hcuts->GetXaxis()->SetBinLabel(1, fn.Data());
  int ibin; 

  while (is.getline(buffer, 200, '\n')) {
    ok = 0;
    if (buffer[0] == '#') {continue;}
    if (buffer[0] == '/') {continue;}
    sscanf(buffer, "%s %f %s", CutName, &CutValue, SetName);

    if (!strcmp(CutName, "TYPE")) {
      TYPE = int(CutValue); ok = 1;
      if (dump) cout << "TYPE:           " << TYPE << endl;
    }
    
    if (!strcmp(CutName, "MODE")) {
      MODE = int(CutValue); ok = 1;
      if (dump) cout << "MODE:           " << MODE << endl;
    }
        
    if (!strcmp(CutName, "MUTYPE1")) {
      MUTYPE1 = int(CutValue); ok = 1;
      if (dump) cout << "MUTYPE1:           " << MUTYPE1 << endl;
    }    
    
    if (!strcmp(CutName, "MUTYPE2")) {
      MUTYPE2 = int(CutValue); ok = 1;
      if (dump) cout << "MUTYPE2:           " << MUTYPE2 << endl;
    }       
    
    if (!strcmp(CutName, "CHI2")) {
      CHI2 = CutValue; ok = 1;
      if (dump) cout << "CHI2:              " << CHI2 << endl;
    }   
    
    if (!strcmp(CutName, "RESTYPE")) {
      RESTYPE = int(CutValue); ok = 1;
      if (dump) cout << "RESTYPE:         " << RESTYPE << endl;
    }
    
    if (!strcmp(CutName, "UPSTYPE")) {
      UPSTYPE = int(CutValue); ok = 1;
      if (dump) cout << "UPSTYPE:         " << UPSTYPE << endl;
    }
    
    if (!strcmp(CutName, "MASSLO")) {
      MASSLO = CutValue; ok = 1;
      if (dump) cout << "MASSLO:          " << MASSLO << endl;
    }           
    
    if (!strcmp(CutName, "MASSHI")) {
      MASSHI = CutValue; ok = 1;
      if (dump) cout << "MASSHI:         " << MASSHI << endl;
    }   
    
    if (!strcmp(CutName, "DETA")) {
      DETA = CutValue; ok = 1;
      if (dump) cout << "DETA:           " << DETA << endl;
    } 
    
    if (!strcmp(CutName, "DPHI")) {
      DPHI = CutValue; ok = 1;
      if (dump) cout << "DPHI:           " << DPHI << endl;
    }
    
    if (!strcmp(CutName, "BIN")) {
      BIN = int(CutValue); ok = 1;
      if (dump) cout << "BIN:              " << BIN << endl;
    }
    
    if (!strcmp(CutName, "HLTPATH")) {
      HLTPATH = SetName; ok = 1;
      if (dump) cout << "HLTPATH:   " << HLTPATH  << endl;
    } 
    
    if (!strcmp(CutName, "HLTLABEL")) {
      HLTLABEL = SetName; ok = 1;
      if (dump) cout << "HLTLABEL:   " << HLTLABEL  << endl;
    }     
    
    if (!strcmp(CutName, "HLTPATH1")) {
      HLTPATH1 = SetName; ok = 1;
      if (dump) cout << "HLTPATH1:   " << HLTPATH1  << endl;
    }   
    
    if (!strcmp(CutName, "HLTPATH2")) {
      HLTPATH2 = SetName; ok = 1;
      if (dump) cout << "HLTPATH2:   " << HLTPATH2  << endl;
    }
     
     if (!strcmp(CutName, "HLTPATH3")) {
       HLTPATH3 = SetName; ok = 1;
       if (dump) cout << "HLTPATH3:   " << HLTPATH3  << endl;
    }   

    if (!strcmp(CutName, "PTLO")) {
      PTLO = CutValue; ok = 1;
      if (dump) cout << "PTLO:           " << PTLO << " GeV" << endl;
      ibin = 11;
      hcuts->SetBinContent(ibin, PTLO);
      hcuts->GetXaxis()->SetBinLabel(ibin, "p_{T}^{min}(l) [GeV]");
    }

    if (!strcmp(CutName, "PTHI")) {
      PTHI = CutValue; ok = 1;
      if (dump) cout << "PTHI:           " << PTHI << " GeV" << endl;
      ibin = 11;
      hcuts->SetBinContent(ibin, PTHI);
      hcuts->GetXaxis()->SetBinLabel(ibin, "p_{T}^{min}(l) [GeV]");
    }    
    
    if (!strcmp(CutName, "ETALO")) {
      ETALO = CutValue; ok = 1;
      if (dump) cout << "ETALO:           " << ETALO << endl;
      ibin = 13;
      hcuts->SetBinContent(ibin, ETALO);
      hcuts->GetXaxis()->SetBinLabel(ibin, "#eta_{T}^{min}(l)");
    }

    if (!strcmp(CutName, "ETAHI")) {
      ETAHI = CutValue; ok = 1;
      if (dump) cout << "ETAHI:           " << ETAHI << endl;
      ibin = 14;
      hcuts->SetBinContent(ibin, ETAHI);
      hcuts->GetXaxis()->SetBinLabel(ibin, "#eta_{T}^{max}(l)");
    }

    if (!ok) cout << "==> ERROR: Don't know about variable " << CutName << endl;
  }

  if (dump)  cout << "------------------------------------" << endl;
}

