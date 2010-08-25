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
  fPTbin[0] = 0.; fPTbin[1] = 2.; fPTbin[2] = 3.; fPTbin[3] = 5.; fPTbin[4] = 8.; fPTbin[5] =12.; fPTbin[6] = 20.;
  fYbin[0] = -2.4; fYbin[1] = -1.4; fYbin[2] = 0.; fYbin[3] = 1.4; fYbin[4] = 2.4;
  fPidTableMuIDPos = new PidTable("../tnp/PidTables/MC/Upsilon/MuID/PtTnpPos-upsilon.dat");
  fPidTableMuIDNeg = new PidTable("../tnp/PidTables/MC/Upsilon/MuID/PtTnpNeg-upsilon.dat");
  fPidTableTrigPos = new PidTable("../tnp/PidTables/MC/Upsilon/Trig/PtTnpPos-upsilon.dat");
  fPidTableTrigNeg = new PidTable("../tnp/PidTables/MC/Upsilon/Trig/PtTnpNeg-upsilon.dat");  
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
  
  acceptance(); 
  if ( isPathPreScaled(HLTPATH) ) goto end;
  if ( !isPathFired(HLTPATH) ) goto end;
  candidateSelection(2); 
  if ( 0 != fpCand  ){
    calculateWeights(0); 
    fillCandHist(); 
    if ( 0 != fgCand ) MCstudy(); 
  }
	   
  fpHistFile->cd();
  
 end:
  fillHist();
}

void xsReader::acceptance(){
  
  TGenCand *gCand(0); TAnaTrack *pTrack(0);
  TGenCand *gDau1(0); TGenCand *gDau2(0);  
  double pt, rapidity; bool match1 = false; bool match2 = false;
  int index1(-1), index2(-1);
  TLorentzVector genCand;
  for (int iG = 0; iG < fpEvt->nGenCands(); ++iG) {
    gCand = fpEvt->getGenCand(iG);
    if ( gCand->fID == RESTYPE && gCand->fStatus == 2 ){
      genCand.SetPtEtaPhiE(gCand->fP.Perp(),gCand->fP.Eta(),gCand->fP.Phi(),gCand->fP.Energy());
      if ( (gCand->fP.Perp() < 20.) && (fabs(genCand.Rapidity()) < 2.4) ){
	getBinCenters(gCand, pt ,rapidity);
	((TH2D*)fpHistFile->Get(Form("AllGenRes_%.1dS",UPSTYPE)))->Fill(pt, rapidity, 1); 
            
	gDau1 = fpEvt->getGenCand(gCand->fDau1);
	gDau2 = fpEvt->getGenCand(gCand->fDau2);
	
	if ( (fabs(gDau1->fID) == 13) && (fabs(gDau1->fID) == 13) ){
	  for (int iR = 0; iR < fpEvt->nRecTracks(); ++iR) {
	    pTrack = fpEvt->getRecTrack(iR);
	    if ( pTrack->fGenIndex == gDau1->fNumber ) {
	      index1 = pTrack->fGenIndex; 
	      match1 = true;
	    }
	    if ( pTrack->fGenIndex == gDau2->fNumber ) {
	      index2 = pTrack->fGenIndex;
	      match2 = true;
	    }
	    if ( match1 && match2 ){
	      ((TH2D*)fpHistFile->Get(Form("RecoGenRes_%.1dS",UPSTYPE)))->Fill(pt, rapidity, 1); 
	      break;
	    }
	    
	  } 
	}
      }
    }
  }
  
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


void xsReader::candidateSelection(int mode){

  fCandY = fCandPt = fCandMass = -1.; 
  fpCand = 0;
  fGenCandY = fGenCandPt = -1.;
  fgCand = 0;
  fMuon1Eta = fMuon1Pt = fMuon2Eta = fMuon2Pt = -1.;
  fGenMuon1Eta = fGenMuon1Pt = fGenMuon2Eta = fGenMuon2Pt = -1.;
  TAnaCand *pCand(0);
  vector<int> lCands, lCands_CT, lCands_CT_M1T, lCands_CT_M1T_M2T, lCands_CT_M1T_M2T_Pt1, lCands_CT_M1T_M2T_Pt1_Pt2, lCands_CT_M1T_M2T_Pt1_Pt2_CHI2; 
  for (int iC = 0; iC < fpEvt->nCands(); ++iC) {
      pCand = fpEvt->getCand(iC);
      AnaEff(pCand, 1); // 1 -- # of TruthCand before Cuts, 2 -- # of TruthCand After Cuts 
      lCands.push_back(iC);
      if (TYPE != pCand->fType) continue;
      if (pCand->fMass < MASSLO) continue;
      if (pCand->fMass > MASSHI) continue;
      lCands_CT.push_back(iC);
      TAnaTrack *pl1 = fpEvt->getSigTrack(pCand->fSig1); 
      TAnaTrack *pl2 = fpEvt->getSigTrack(pCand->fSig2);
      if ( (pl1->fMuID & MUTYPE1) != MUTYPE1 ) continue;
      lCands_CT_M1T.push_back(iC);
      if ( (pl2->fMuID & MUTYPE2) != MUTYPE2 ) continue;      
      lCands_CT_M1T_M2T.push_back(iC);
      if ( pl1->fPlab.Perp() <= PT1 ) continue;
      lCands_CT_M1T_M2T_Pt1.push_back(iC);
      if ( pl2->fPlab.Perp() <= PT2 ) continue;
      lCands_CT_M1T_M2T_Pt1_Pt2.push_back(iC);
      if ( pCand->fVtx.fChi2 > CHI2 ) continue;
      lCands_CT_M1T_M2T_Pt1_Pt2_CHI2.push_back(iC);
      
  }
  int nc(lCands.size()); int nc_CT(lCands_CT.size()); int nc_CT_M1T(lCands_CT_M1T.size()); int nc_CT_M1T_M2T(lCands_CT_M1T_M2T.size());
  int nc_CT_M1T_M2T_Pt1(lCands_CT_M1T_M2T_Pt1.size()); int nc_CT_M1T_M2T_Pt1_Pt2(lCands_CT_M1T_M2T_Pt1_Pt2.size());
  int nc_CT_M1T_M2T_Pt1_Pt2_CHI2(lCands_CT_M1T_M2T_Pt1_Pt2_CHI2.size());
  ((TH1D*)fpHistFile->Get("n2"))->Fill(nc); 
  ((TH1D*)fpHistFile->Get("n2_CandType"))->Fill(nc_CT);
  ((TH1D*)fpHistFile->Get("n2_CandType_MuType1"))->Fill(nc_CT_M1T);
  ((TH1D*)fpHistFile->Get("n2_CandType_MuType1&2"))->Fill(nc_CT_M1T_M2T);
  ((TH1D*)fpHistFile->Get("n2_CandType_MuType1&2_Pt1"))->Fill(nc_CT_M1T_M2T_Pt1);
  ((TH1D*)fpHistFile->Get("n2_CandType_MuType1&2_Pt1&2"))->Fill(nc_CT_M1T_M2T_Pt1_Pt2);
  ((TH1D*)fpHistFile->Get("n2_CandType_MuType1&2_Pt1&2_Chi2"))->Fill(nc_CT_M1T_M2T_Pt1_Pt2_CHI2);
  
  
  ((TH1D*)fpHistFile->Get("n2_cuts"))->AddBinContent(10, nc);
  ((TH1D*)fpHistFile->Get("n2_cuts"))->GetXaxis()->SetBinLabel(10, "NOCUT");
  ((TH1D*)fpHistFile->Get("n2_cuts"))->AddBinContent(12, nc_CT);
  ((TH1D*)fpHistFile->Get("n2_cuts"))->GetXaxis()->SetBinLabel(12, Form("Type^{Cand}=%d",TYPE));
  ((TH1D*)fpHistFile->Get("n2_cuts"))->AddBinContent(14, nc_CT_M1T);
  ((TH1D*)fpHistFile->Get("n2_cuts"))->GetXaxis()->SetBinLabel(14, Form("ID^{#mu_{1}}=%d",MUTYPE1));
  ((TH1D*)fpHistFile->Get("n2_cuts"))->AddBinContent(16, nc_CT_M1T_M2T);
  ((TH1D*)fpHistFile->Get("n2_cuts"))->GetXaxis()->SetBinLabel(16, Form("ID^{#mu_{2}}=%d",MUTYPE2));
  ((TH1D*)fpHistFile->Get("n2_cuts"))->AddBinContent(18, nc_CT_M1T_M2T_Pt1);
  ((TH1D*)fpHistFile->Get("n2_cuts"))->GetXaxis()->SetBinLabel(18, Form("p_{T}^{#mu_{1}}=%.1f",PT1));
  ((TH1D*)fpHistFile->Get("n2_cuts"))->AddBinContent(20, nc_CT_M1T_M2T_Pt1_Pt2);
  ((TH1D*)fpHistFile->Get("n2_cuts"))->GetXaxis()->SetBinLabel(20, Form("p_{T}^{#mu_{2}}=%.1f",PT2));
  ((TH1D*)fpHistFile->Get("n2_cuts"))->AddBinContent(22, nc_CT_M1T_M2T_Pt1_Pt2_CHI2);
  ((TH1D*)fpHistFile->Get("n2_cuts"))->GetXaxis()->SetBinLabel(22, Form("#chi^{2}<%.1f",CHI2));

  
  if (0 == nc_CT_M1T_M2T_Pt1_Pt2_CHI2) return; 
  int best(0);
  if (nc_CT_M1T_M2T_Pt1_Pt2_CHI2 > 1) {
    //cout << "MORE THAN ONE CANDIDATE  " << nc_CT_M1T_M2T_Pt1_Pt2_CHI2   <<  endl;
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
    fpCand = fpEvt->getCand(best);
    AnaEff(fpCand, 2);
    fCandPt   = fpCand->fPlab.Perp();
    fCandMass = fpCand->fMass;
    Cand.SetPtEtaPhiM(fpCand->fPlab.Perp(),fpCand->fPlab.Eta(),fpCand->fPlab.Phi(),fpCand->fMass);
    fCandY = Cand.Rapidity();
    TAnaTrack *pl1 = fpEvt->getSigTrack(fpCand->fSig1); 
    TAnaTrack *pl2 = fpEvt->getSigTrack(fpCand->fSig2);
    if ( (pl1->fGenIndex > -1) && (pl2->fGenIndex > -1) && (pl1->fGenIndex != pl2->fGenIndex) ){
      TGenCand  *gl1 = fpEvt->getGenCand(pl1->fGenIndex);
      TGenCand  *gl2 = fpEvt->getGenCand(pl2->fGenIndex);
      if ( gl1->fMom1 == gl2->fMom1 ) {
	TGenCand  *genCand = fpEvt->getGenCand(gl1->fMom1);
	((TH1D*)fpHistFile->Get("TruthCand"))->Fill(genCand->fID);
	if ( genCand->fID == RESTYPE ) {
	  truth++;
	  fgCand = fpEvt->getGenCand(gl1->fMom1); 
	  fGenCandPt   = fgCand->fP.Perp();
	  gCand.SetPtEtaPhiE(fgCand->fP.Perp(),fgCand->fP.Eta(),fgCand->fP.Phi(),fgCand->fP.Energy());
	  fGenCandY = gCand.Rapidity();
	  fGenMuon1Pt = gl1->fP.Perp(); fGenMuon2Pt = gl2->fP.Perp(); fMuon1Pt = pl1->fPlab.Perp(); fMuon2Pt = pl2->fPlab.Perp(); 
	  fGenMuon1Eta = gl1->fP.Eta(); fGenMuon2Eta = gl2->fP.Eta(); fMuon1Eta = pl1->fPlab.Eta(); fMuon2Eta = pl2->fPlab.Eta();
	}
	((TH1D*)fpHistFile->Get("n2_cuts"))->AddBinContent(2, truth);
	((TH1D*)fpHistFile->Get("n2_cuts"))->GetXaxis()->SetBinLabel(2, Form("TruthCand"));	
      }	
    }
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
      if ( genCand->fID == RESTYPE ) {
	truth++;
      }
      if ( mode == 1 ) ((TH1D*)fpHistFile->Get(Form("AnaEff_%.1dS,OverAll", UPSTYPE)))->AddBinContent(2,truth);
      if ( mode == 2 ) ((TH1D*)fpHistFile->Get(Form("AnaEff_%.1dS,OverAll", UPSTYPE)))->AddBinContent(8,truth);
      
      Cand.SetPtEtaPhiM(pCand->fPlab.Perp(),pCand->fPlab.Eta(),pCand->fPlab.Phi(),pCand->fMass);
      for ( int iy = 0; iy < fNy; ++iy ){
	for ( int ipt = 0; ipt < fNpt; ++ipt ){
	  if( ( Cand.Rapidity() <= fYbin[iy+1] ) && ( Cand.Rapidity() > fYbin[iy] ) ){
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
  for ( int ipt = 0; ipt < fNpt; ++ipt ){
    if ( ( fCandPt >= fPTbin[ipt] ) && ( fCandPt < fPTbin[ipt+1] ) ){
      ((TH1D*)fpHistFile->Get(Form("UpsilonMass,pt%.1f_%.1f", fPTbin[ipt], fPTbin[ipt+1])))->Fill(fCandMass,fWeight);
    }
  }
  
  for ( int iy = 0; iy < fNy; ++iy ){
    if ( ( fCandY >= fYbin[iy] ) && ( fCandY < fYbin[iy+1] ) ){
      ((TH1D*)fpHistFile->Get(Form("UpsilonMass,rapidity%.1f_%.1f", fYbin[iy], fYbin[iy+1])))->Fill(fCandMass,fWeight);
    }
  }
  
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
  TAnaTrack *pl1 = fpEvt->getSigTrack(fpCand->fSig1); 
  TAnaTrack *pl2 = fpEvt->getSigTrack(fpCand->fSig2);
  //cout <<pl1->fPlab.Perp()<<"  "<<pl2->fPlab.Perp() << endl;
  //cout <<pl1->fPlab.Eta()<<"  "<<pl2->fPlab.Eta() << endl;
  
  if ( pl1->fQ > 0 ){
    
    effID1 = fPidTableMuIDPos->effD(pl1->fPlab.Perp(), pl1->fPlab.Eta(), 0.);
    effTR1 = fPidTableTrigPos->effD(pl1->fPlab.Perp(), pl1->fPlab.Eta(), 0.);

  } else if ( pl1->fQ < 0 ){
    effID1 = fPidTableMuIDNeg->effD(pl1->fPlab.Perp(), pl1->fPlab.Eta(), 0.);
    effTR1 = fPidTableMuIDNeg->effD(pl1->fPlab.Perp(), pl1->fPlab.Eta(), 0.);
  }
  
  if ( pl2->fQ > 0 ){
    effID2 = fPidTableMuIDPos->effD(pl2->fPlab.Perp(), pl2->fPlab.Eta(), 0.);
    effTR2 = fPidTableTrigPos->effD(pl2->fPlab.Perp(), pl2->fPlab.Eta(), 0.);
  }
  else if ( pl2->fQ < 0 ){
    effID2 = fPidTableMuIDNeg->effD(pl2->fPlab.Perp(), pl2->fPlab.Eta(), 0.);
    effTR2 = fPidTableTrigPos->effD(pl2->fPlab.Perp(), pl2->fPlab.Eta(), 0.);
  }
  
  fWeight = 1/(effID1*effID2*effTR1*effTR2);
  //cout <<" effID1  =  "<<effID1<<", effID2 =  "<<effID2<<" effTR1  =  "<<effTR1<<", effTR2 =  "<<effTR2<<", fWeight  =  "<<  fWeight <<endl;
  
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
  k = new TH2D(Form("AllGenRes_%1.dS",  UPSTYPE), Form("AllGenRes_%1.dS", UPSTYPE), fNpt, fPTbin, fNy, fYbin); 
  k = new TH2D(Form("RecoGenRes_%1.dS", UPSTYPE), Form("RecoGenRes_%1.dS", UPSTYPE), fNpt, fPTbin, fNy, fYbin); 
  ((TH2D*)fpHistFile->Get(Form("AllGenRes_%.1dS", UPSTYPE)))->Sumw2();
  ((TH2D*)fpHistFile->Get(Form("RecoGenRes_%.1dS", UPSTYPE)))->Sumw2();
  
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
  for ( int iy = 0; iy < fNy; ++iy ){
    h = new TH1D(Form("UpsilonMass,rapidity%.1f_%.1f", fYbin[iy], fYbin[iy+1]),
		 Form("UpsilonMass,rapidity%.1f_%.1f", fYbin[iy], fYbin[iy+1]),
		 fBin, fMassLow, fMassHigh);
  }     
  for ( int ipt = 0; ipt < fNpt; ++ipt ){
    h = new TH1D(Form("UpsilonMass,pt%.1f_%.1f", fPTbin[ipt], fPTbin[ipt+1]),
		 Form("UpsilonMass,pt%.1f_%.1f", fPTbin[ipt], fPTbin[ipt+1]),
		 fBin, fMassLow, fMassHigh);
  }
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
    
    if (!strcmp(CutName, "MUTYPE1")) {
      MUTYPE1 = int(CutValue); ok = 1;
      if (dump) cout << "MUTYPE1:           " << MUTYPE1 << endl;
    }    
    
    if (!strcmp(CutName, "MUTYPE2")) {
      MUTYPE2 = int(CutValue); ok = 1;
      if (dump) cout << "MUTYPE2:           " << MUTYPE2 << endl;
    }       
    
    if (!strcmp(CutName, "PT1")) {
      PT1 = CutValue; ok = 1;
      if (dump) cout << "PT1:             " << PT1 << endl;
    }    
    
    if (!strcmp(CutName, "PT2")) {
      PT2 = CutValue; ok = 1;
      if (dump) cout << "PT2:             " << PT2 << endl;
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
    
    if (!strcmp(CutName, "BIN")) {
      BIN = int(CutValue); ok = 1;
      if (dump) cout << "BIN:              " << BIN << endl;
    }
    
    if (!strcmp(CutName, "HLTPATH")) {
      HLTPATH = SetName; ok = 1;
      if (dump) cout << "HLTPATH:   " << HLTPATH  << endl;
    } 
    
    if (!strcmp(CutName, "PTLO")) {
      PTLO = CutValue; ok = 1;
      if (dump) cout << "PTLO:           " << PTLO << " GeV" << endl;
      ibin = 11;
      hcuts->SetBinContent(ibin, PTLO);
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

