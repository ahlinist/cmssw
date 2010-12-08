#include "tnpReader2.hh"
#include <vector>
#include "TRandom.h"
#define MMUON 0.10566
#define MKAON 0.49368

// ----------------------------------------------------------------------
// Run with: ./runTNPReaders -c chains/bg-test -D root 
//           ./runTNPReaders -f test.root 
// ----------------------------------------------------------------------

const int  tnpReader2::fNpt;
const int  tnpReader2::fNeta;
const int  tnpReader2::fNq;

// ----------------------------------------------------------------------
tnpReader2::tnpReader2(TChain *tree, TString evtClassName): treeReaderTNP(tree, evtClassName) {
  cout << "--> tnpReader2> This is the start ..." << endl;
  fpJSON = new JSON("/shome/bora/root/json/json_147196_149442");
  fPTbin[0] = 3.; fPTbin[1] = 4.; fPTbin[2] = 5.; fPTbin[3] = 6.; fPTbin[4] = 20.;
  fEtabin[0] = -2.4; fEtabin[1] = -1.2; fEtabin[2] = -0.4; fEtabin[3] = 0.4; fEtabin[4] = 1.2; fEtabin[5] = 2.4;
  fQ[0] = -1;  fQ[1] = 1;
}
// ----------------------------------------------------------------------
tnpReader2::~tnpReader2() {
  cout << "--> tnpReader2> This is the end ..." << endl;
}

// ----------------------------------------------------------------------
void tnpReader2::startAnalysis() {
  cout << "--> tnpReader2> startAnalysis: ..." << endl;
}

void tnpReader2::eventProcessing() {
  
  if ( SAMPLE == 2  ) if ( !fpJSON->good(fRun, fLS) ) goto end;
  Trigger = -1;
   
  //cout << " NEW EVENT  " << endl;
  if ( SAMPLE == 1 ) MCTruth(MODE);  // For running on MC
  if ( isPathPreScaled(HLTPATH_TAG) ) goto end;
  if ( !isPathFired(HLTPATH_TAG) ) goto end;
  TagSelection();
  for ( unsigned int v = 0; v < fCand.size(); ++v   ) {
    /// For Single Muon Trigger
    //if ( !isMatchedToTrig(fCand[v],HLTLABEL_TAG,1)) continue;    //0 -- No Trigger Matching , 1 -- Trigger Matching for Tag , 2 -- Trigger Matching for Probe
    ///
    if ( !isMatchedToDoubleTrig(fCand[v],HLTLABEL_TAG,1)) continue;  //0 -- No Trigger Matching , 1 -- Trigger Matching for Tag to Single Muon T.O., 
    //2 -- Trigger Matching for Tag to Double Muon T.O. // 3 -- Trigger Matching for Probe to Double Muon T.O.
  }
  
  if ( MODE == 2 ) {
    if ( isPathPreScaled(HLTPATH_PROBE) ) goto end;  // Only for Trigger Study
  }
  
  ProbeSelection();
  Info();
  
  if ( MODE == 1 ){ // MuID Efficiency Mode
    
    for ( unsigned int y = 0; y < fCandPS.size(); ++y   ) {
      
      fillHist(fCandPS[y], 1, false); // Mode: 1 -- mt
      if ( SAMPLE == 1 ) {  // For running on DATA
	if ( truthMatch(fCandPS[y]) ) fillHist(fCandPS[y], 4, false); // Mode: 4 -- mtMatched
      }
      isGoodProbe(fCandPS[y]);
      
    }
    
    for ( unsigned int x = 0; x < fCandGP.size(); ++x   ) {
      
      fillHist(fCandGP[x],2, false); // Mode: 2 -- mm
      if ( truthMatch(fCandGP[x]) ) fillHist(fCandGP[x], 5, false); // Mode: 5 -- mmMatched
      
    }
    
    for ( unsigned int z = 0; z < fCandnotGP.size(); ++z   ) {
      
      fillHist(fCandnotGP[z],3, false);  // Mode: 3 -- mmbar
      
    }
    
  } else if ( MODE == 2  ){  // Trigger Efficiency Mode
    for ( unsigned int y = 0; y < fCandPS.size(); ++y   ) {
      fillHist(fCandPS[y], 1, false); // Mode: 1 -- mt
      if ( SAMPLE == 1 ) {  // For running on MC
	if ( truthMatch(fCandPS[y]) ) fillHist(fCandPS[y], 4, false); // Mode: 4 -- mtMatched
      }
      /// For Single Muon Trigger
      //isMatchedToTrig(fCandPS[y],HLTLABEL_PROBE,2);
      ///
      isMatchedToDoubleTrig(fCandPS[y],HLTLABEL_PROBE,2);
    }
    
    for ( unsigned int q = 0; q < fCandTT2.size(); ++q   ) {
      isMatchedToDoubleTrig(fCandTT2[q],HLTLABEL_PROBE,3);
    }
    
    for ( unsigned int x = 0; x < fCandTP.size(); ++x   ) {
      
      fillHist(fCandTP[x],2, false); // Mode: 2 -- mm
      if ( truthMatch(fCandTP[x]) ) fillHist(fCandTP[x], 5, false); // Mode: 5 -- mmMatched
      
    }
    
    for ( unsigned int z = 0; z < fCandnotTP.size(); ++z   ) {
      
      fillHist(fCandnotTP[z],3, false);  // Mode: 3 -- mmbar
    
    }

  } else if ( MODE == 3 ){
    
  }
  
  
  
  
  
 end:
  freePointers();
  
}

void tnpReader2::freePointers(){
  TAnaCand *pCand(0); 
  while (!fCand.empty())
    {
      // get first 'element'
      pCand = fCand.front();
      // remove it from the list
      fCand.erase(fCand.begin());
    }
  
  while (!fCandTT.empty())
    {
      pCand = fCandTT.front();
      fCandTT.erase(fCandTT.begin());
    }  
  
  while (!fCandTT2.empty())
    {
      pCand = fCandTT2.front();
      fCandTT2.erase(fCandTT2.begin());
    }
  
  while (!fCandPS.empty())
    {
      pCand = fCandPS.front();
      fCandPS.erase(fCandPS.begin());
    }
  
  while (!fCandTP.empty())
    {
      pCand = fCandTP.front();
      fCandTP.erase(fCandTP.begin());
    }
  
  while (!fCandnotTP.empty())
    {
      pCand = fCandnotTP.front();
      fCandnotTP.erase(fCandnotTP.begin());
    }
  
  while (!fCandGP.empty())
    {
      pCand = fCandGP.front();
      fCandGP.erase(fCandGP.begin());
    }
  
  while (!fCandnotGP.empty())
    {
      pCand = fCandnotGP.front();
      fCandnotGP.erase(fCandnotGP.begin());
    }
}

void tnpReader2::MCTruth(int mode){
  TAnaTrack *pTrack(0);
  if ( mode == 1 ){ // For MuID
    for (int it = 0; it < fpEvt->nRecTracks(); ++it) {
      pTrack = fpEvt->getRecTrack(it);
      
      if ( pTrack->fMCID == 13 ){
	if ( (pTrack->fPlab.Perp() <= 20.) && (pTrack->fPlab.Perp() >= 3.) && (pTrack->fPlab.Eta() <= 2.4) && (pTrack->fPlab.Eta() >= -2.4)){
	  ((TH2D*)fpHistFile->Get("tEtaPt_neg"))->Fill(pTrack->fPlab.Eta() , pTrack->fPlab.Perp());
	}
	
	if ( (pTrack->fMuID & 0x1<< MUTYPE1) && (pTrack->fMuID & 0x1<< MUTYPE2)){
	  if ( (pTrack->fPlab.Perp() <= 20.) && (pTrack->fPlab.Perp() >= 3.) && (pTrack->fPlab.Eta() <= 2.4) && (pTrack->fPlab.Eta() >= -2.4)){
	    ((TH2D*)fpHistFile->Get("mEtaPt_neg"))->Fill(pTrack->fPlab.Eta() , pTrack->fPlab.Perp());
	  }
	}
      }     
      
      if ( pTrack->fMCID == -13 ){
	if ( (pTrack->fPlab.Perp() <= 20.) && (pTrack->fPlab.Perp() >= 3.) && (pTrack->fPlab.Eta() <= 2.4) && (pTrack->fPlab.Eta() >= -2.4)){
	  ((TH2D*)fpHistFile->Get("tEtaPt_pos"))->Fill(pTrack->fPlab.Eta() , pTrack->fPlab.Perp());
	}
	
	if ( (pTrack->fMuID & 0x1<< MUTYPE1) && (pTrack->fMuID & 0x1<< MUTYPE2) ){
	  if ( (pTrack->fPlab.Perp() <= 20.) && (pTrack->fPlab.Perp() >= 3.) && (pTrack->fPlab.Eta() <= 2.4) && (pTrack->fPlab.Eta() >= -2.4)){
	    ((TH2D*)fpHistFile->Get("mEtaPt_pos"))->Fill(pTrack->fPlab.Eta() , pTrack->fPlab.Perp());
	  }
	}
      } 
    }
  }
  
  if ( mode == 2 ){ // For Trigger
    for (int it = 0; it < fpEvt->nRecTracks(); ++it) {
      pTrack = fpEvt->getRecTrack(it);
      if ( (pTrack->fMuID & 0x1<< MUTYPE1) && (pTrack->fMuID & 0x1<< MUTYPE2) ){
	if ( pTrack->fMCID == 13 ){
	  if ( (pTrack->fPlab.Perp() <= 20.) && (pTrack->fPlab.Perp() >= 3.) && (pTrack->fPlab.Eta() <= 2.4) && (pTrack->fPlab.Eta() >= -2.4)){
	    ((TH2D*)fpHistFile->Get("tEtaPt_neg"))->Fill(pTrack->fPlab.Eta() , pTrack->fPlab.Perp());
	  }
	
	  if ( isRecTrackMatchedToTrig(pTrack, HLTLABEL_PROBE)  ){ 
	    if ( (pTrack->fPlab.Perp() <= 20.) && (pTrack->fPlab.Perp() >= 3.) && (pTrack->fPlab.Eta() <= 2.4) && (pTrack->fPlab.Eta() >= -2.4)){
	      ((TH2D*)fpHistFile->Get("mEtaPt_neg"))->Fill(pTrack->fPlab.Eta() , pTrack->fPlab.Perp());
	    }
	  }
	}     
      
	if ( pTrack->fMCID == -13 ){
	  if ( (pTrack->fPlab.Perp() <= 20.) && (pTrack->fPlab.Perp() >= 3.) && (pTrack->fPlab.Eta() <= 2.4) && (pTrack->fPlab.Eta() >= -2.4)){
	    ((TH2D*)fpHistFile->Get("tEtaPt_pos"))->Fill(pTrack->fPlab.Eta() , pTrack->fPlab.Perp());
	  }
	  
	  if ( isRecTrackMatchedToTrig(pTrack, HLTLABEL_PROBE)  ){ 
	    if ( (pTrack->fPlab.Perp() <= 20.) && (pTrack->fPlab.Perp() >= 3.) && (pTrack->fPlab.Eta() <= 2.4) && (pTrack->fPlab.Eta() >= -2.4)){
	      ((TH2D*)fpHistFile->Get("mEtaPt_pos"))->Fill(pTrack->fPlab.Eta() , pTrack->fPlab.Perp());
	    }
	  }
	}
      } 
    }
  }
  
}

bool tnpReader2::isPathPreScaled( TString Path ){
  bool PreScale = false;
  for (int a = 0; a < NHLT ; ++a) {
    if ( fpEvt->fHLTNames[a] ==  Path  && fpEvt->fHLTError[a] & 1  ) {
      PreScale = true;
      //cout << Path << " is prescaled!!!! "  << endl;
    }
  }
  return PreScale;
}

bool tnpReader2::isPathFired( TString Path ){
  bool HLT_Path = false;
  bool v1 = false;
  bool LS_v1 = false;
  
  for (int a = 0; a < NHLT ; ++a) {
    //cout << "fpEvt->fHLTNames[a]  = " << fpEvt->fHLTNames[a] << endl;
  }
  
  for (int a = 0; a < NHLT ; ++a) {
    if ( fpEvt->fHLTNames[a] ==  Path  && fpEvt->fHLTResult[a] == 1  ) {
      HLT_Path = true;
      
      //cout << Path << " fired!!!! "  << endl;
      for (int b = 0; b < NHLT ; ++b){
	if ( fpEvt->fHLTResult[b] == 1  ){
	  if ( fpEvt->fHLTNames[b] ==  "HLT_DoubleMu0"  ) {
	    //cout << fpEvt->fHLTNames[b] << endl;
	    Trigger = 1;
	  }
	}
      }
    }
  }
  
  if ( HLT_Path ){
    TTrgObj *pTrig(0);
    for (int s = 0; s < fpEvt->nTrgObj() ; ++s) {
      pTrig = fpEvt->getTrgObj(s);
      //  cout << "pTrig->fLabel is " << pTrig->fLabel << endl;
    }
  }
  return HLT_Path;
}

void tnpReader2::TagSelection(){
  
  TAnaCand *pCand(0);  TAnaTrack *pTag(0); TAnaTrack *pTrack(0);
  for (int i = 0; i < fpEvt->nCands(); ++i) {
    pCand = fpEvt->getCand(i);
    if ( pCand->fType != TYPE ) continue;
    pTag = fpEvt->getSigTrack(pCand->fSig1);
    if ( pTag->fPlab.Perp() < PT_TAG ) continue;
    if ( pTag->fPlab.Eta() > ETAHI_TAG ) continue;
    if ( pTag->fPlab.Eta() < ETALO_TAG ) continue;
    if ( !((pTag->fMuID & 0x1<< MUTYPE1 ) && (pTag->fMuID & 0x1<< MUTYPE2)) ) continue;
    
    // SOME CHANGES to Mimic Spainard's Study
    //if ( !(pTag->fMuID & 0x1<< MUTYPE1) ) continue;
   
    //pTrack = fpEvt->getRecTrack(pTag->fIndex);
    //if ( !TrackSelection(pTrack,1) ) continue;
    
    //if ( pTag->fTrackQuality != 2 ) continue;
    //cout << " pTag->fIndex = "  << pTag->fIndex << endl;
    fCand.push_back(pCand);
  }
  
}

void tnpReader2::ProbeSelection(){
  
  TAnaCand *pCand(0);  TAnaTrack *pProbe(0); TAnaTrack *pTrack(0); TAnaTrack *pTag(0);
  for (unsigned int i = 0; i < fCandTT.size(); ++i) {
    pCand = fCandTT[i];
    if ( pCand->fMass < MASSLO  ) continue;
    if ( pCand->fMass > MASSHI  ) continue;
    if ( pCand->fMaxDoca > MAXDOCA  ) continue;
    pProbe = fpEvt->getSigTrack(pCand->fSig2);
    if ( pProbe->fPlab.Perp() < PT_PROBE ) continue;
    
    if ( MODE == 2  ) if ( !((pProbe->fMuID & 0x1<< MUTYPE1 ) && (pProbe->fMuID & 0x1<< MUTYPE2)) ) continue;
    
    pTag = fpEvt->getSigTrack(pCand->fSig1);
    if ( RESTYPE == 443 ) if ( (pProbe->fQ*(pProbe->fPlab.Phi() - pTag->fPlab.Phi())) > 0  ) { 
        continue; // Cowboy Veto For JPsi muons
     }

     // SOME CHANGES to Mimic Spainard's Study
     //pTrack = fpEvt->getRecTrack(pProbe->fIndex);
     //if ( !TrackSelection(pTrack,2)) continue; 

     //if ( pProbe->fTrackQuality != 2 ) continue;
     //cout << " pProbe->fIndex = "  << pProbe->fIndex << endl;
     fCandPS.push_back(pCand);
   }

 }

 bool tnpReader2::TrackSelection(TAnaTrack *pTrack, int mode){
   bool passed = false;					
   if ((pTrack->fChi2/pTrack->fDof) > 4) return passed;
   if (fabs(pTrack->fd0) > 2 ) return passed;
   if (fabs(pTrack->fdz) > 30 ) return passed;
   int pixelhits(0), siliconhits(0);
   for (int i=0; i < 20; i++){
     cout << " NEW PATTERN  " << endl;
     /* for (int j=9; j>=0; j--) {
	int bit = (pTrack->fHitPattern[i] >> j) & 0x1;
	cout << bit << endl;;
	}*/
     if ( !(~(pTrack->fHitPattern[i]) & 0x1) ) continue;
     if ( !(~(pTrack->fHitPattern[i]) & 0x2) ) continue;
     if ( !(pTrack->fHitPattern[i] & 0x1 << 10)) continue;
     if ( (pTrack->fHitPattern[i] & 0x1 << 9) ) siliconhits++;
     if ( (pTrack->fHitPattern[i] & 0x1 << 8) && (pTrack->fHitPattern[i] & 0x1 << 7)) siliconhits++;
     if ( (pTrack->fHitPattern[i] & 0x1 << 7) && !(pTrack->fHitPattern[i] & 0x1 << 8) && !(pTrack->fHitPattern[i] & 0x1 << 9) ) pixelhits++;
     if ( !(pTrack->fHitPattern[i] & 0x1 << 7) && (pTrack->fHitPattern[i] & 0x1 << 8) && !(pTrack->fHitPattern[i] & 0x1 << 9) ) pixelhits++;
   }
   // if ( pixelhits >= 1 ) cout << pTrack->fPlab.Eta() << endl;
   //cout <<"pixelhits = " << pixelhits <<"siliconhits = "<<siliconhits << endl;
   //cout <<"validhits = " << pTrack->fValidHits << endl;
   if ( (pixelhits > 1) && (siliconhits > 11) ) {
     //cout << " Valid Track "  << endl;
     passed = true;
   }
   return passed;
 }



 void tnpReader2::Info(){

   TAnaCand *pCand(0);  TAnaTrack *pTag(0);
   for (int i = 0; i < fCandPS.size(); ++i) {
     pCand = fpEvt->getCand(i);
     pTag = fpEvt->getSigTrack(pCand->fSig1);
     ((TH1D*)fpHistFile->Get("Tag_pt"))->Fill(pTag->fPlab.Perp());
     ((TH1D*)fpHistFile->Get("Cand_MaxDoca"))->Fill(pCand->fMaxDoca);
     ((TH1D*)fpHistFile->Get("Cand_Chi2"))->Fill(pCand->fVtx.fChi2);
   }

 }


bool tnpReader2::isRecTrackMatchedToTrig(TAnaTrack *pTrack, TString Label){
   bool Matched = false;
   TTrgObj *pTrig(0);
   
   TLorentzVector track;
   track.SetPtEtaPhiM(pTrack->fPlab.Pt(), pTrack->fPlab.Eta(), pTrack->fPlab.Phi(), MMUON);
   for (int s = 0; s < fpEvt->nTrgObj() ; ++s) {
     pTrig = fpEvt->getTrgObj(s);
     if ( !(Label.CompareTo(pTrig->fLabel)) ) {
       //cout << "For Track: pTrig->fLabel is " << pTrig->fLabel << endl;;
       double track_dR = track.DeltaR(pTrig->fP);
       double track_dEta = TMath::Abs(pTrack->fPlab.Eta() - pTrig->fP.Eta());
       double track_dPhi = TMath::Abs(pTrack->fPlab.Phi() - pTrig->fP.Phi());
       if ( ( track_dPhi < DPHI ) && ( track_dEta < DETA )) {
	 Matched = true;				
	   //cout << " Trigger Matched to Track " << endl;
	 break;
	 }
     }
   }
   
   return Matched;
}



 bool tnpReader2::isMatchedToTrig(TAnaCand *pCand, TString Label, int mode){
   bool HLTlabel = false;
   TTrgObj *pTrig(0);

   if ( mode == 0 ){
     HLTlabel = true;
     fCandTT.push_back(pCand);
   }

   if ( mode == 1 ){ // For Matching Tag muon to T.O.
     TLorentzVector tag;
     TAnaTrack *pTag(0);
     pTag = fpEvt->getSigTrack(pCand->fSig1);
     tag.SetPtEtaPhiM(pTag->fPlab.Pt(), pTag->fPlab.Eta(), pTag->fPlab.Phi(), MMUON);
     for (int s = 0; s < fpEvt->nTrgObj() ; ++s) {
       pTrig = fpEvt->getTrgObj(s);
       if ( !(Label.CompareTo(pTrig->fLabel)) ) {
	 //cout << "For Tag: pTrig->fLabel is " << pTrig->fLabel << endl;;
	 double tag_dR = tag.DeltaR(pTrig->fP);
	 double tag_dEta = TMath::Abs(pTag->fPlab.Eta() - pTrig->fP.Eta());
	 double tag_dPhi = TMath::Abs(pTag->fPlab.Phi() - pTrig->fP.Phi());
	 ((TH1D*)fpHistFile->Get("Tag_trig_dEta"))->Fill(tag_dEta);
	 ((TH1D*)fpHistFile->Get("Tag_trig_dPhi"))->Fill(tag_dPhi);
	 if ( ( tag_dPhi < DPHI ) && ( tag_dEta < DETA )) {
	   HLTlabel = true;				
	   ((TH1D*)fpHistFile->Get("Tag_trig_dR_aftercuts"))->Fill(tag_dR);
	   //cout << " Trigger Matched to Tag " << endl;
	   fCandTT.push_back(pCand);
	   break;
	 }
       }
     }
   }

   if ( mode == 2 ){ // For Matching Probe muon to T.O.
     TLorentzVector probe;
     TAnaTrack *pProbe(0);
     pProbe = fpEvt->getSigTrack(pCand->fSig2);
     probe.SetPtEtaPhiM(pProbe->fPlab.Pt(), pProbe->fPlab.Eta(), pProbe->fPlab.Phi(), MMUON);
     for (int s = 0; s < fpEvt->nTrgObj() ; ++s) {
       pTrig = fpEvt->getTrgObj(s);
       if ( !(Label.CompareTo(pTrig->fLabel)) ) {
	 double probe_dR = probe.DeltaR(pTrig->fP);
	 double probe_dEta = TMath::Abs(pProbe->fPlab.Eta() - pTrig->fP.Eta());
	 double probe_dPhi = TMath::Abs(pProbe->fPlab.Phi() - pTrig->fP.Phi());
	 ((TH1D*)fpHistFile->Get("Probe_trig_dEta"))->Fill(probe_dEta);
	 ((TH1D*)fpHistFile->Get("Probe_trig_dPhi"))->Fill(probe_dPhi);
	 if ( ( probe_dPhi < DPHI ) && ( probe_dEta < DETA )) {
	   HLTlabel = true;				
	   ((TH1D*)fpHistFile->Get("Probe_trig_dR_aftercuts"))->Fill(probe_dR);
	   //cout << " Trigger Matched to Probe " << endl;
	   fCandTP.push_back(pCand); // will be used for mm
	   break;
	 } 
       }
     }

     if ( !(HLTlabel) ) fCandnotTP.push_back(pCand);  // wiil be used for mmbar 

   }
     return HLTlabel;
 }



 bool tnpReader2::isMatchedToDoubleTrig(TAnaCand *pCand, TString Label, int mode){
   bool HLTlabel = false;
   TTrgObj *pTrig(0);
   int t(-1);
   if ( mode == 0 ){
     HLTlabel = true;
     fCandTT.push_back(pCand);
   }

   if ( mode == 1 ){ // For Matching Tag muon to Single Muon T.O.
     TLorentzVector tag;
     TAnaTrack *pTag(0);
     pTag = fpEvt->getSigTrack(pCand->fSig1);
     tag.SetPtEtaPhiM(pTag->fPlab.Pt(), pTag->fPlab.Eta(), pTag->fPlab.Phi(), MMUON);
     for (int s = 0; s < fpEvt->nTrgObj() ; ++s) {
       pTrig = fpEvt->getTrgObj(s);
       if ( !(Label.CompareTo(pTrig->fLabel)) ) {
	 double tag_dR = tag.DeltaR(pTrig->fP);
	 double tag_dEta = TMath::Abs(pTag->fPlab.Eta() - pTrig->fP.Eta());
	 double tag_dPhi = TMath::Abs(pTag->fPlab.Phi() - pTrig->fP.Phi());
	 ((TH1D*)fpHistFile->Get("Tag_trig_dEta"))->Fill(tag_dEta);
	 ((TH1D*)fpHistFile->Get("Tag_trig_dPhi"))->Fill(tag_dPhi);
	 if ( ( tag_dPhi < DPHI ) && ( tag_dEta < DETA )) {
	   HLTlabel = true;				
	   ((TH1D*)fpHistFile->Get("Tag_trig_dR_aftercuts"))->Fill(tag_dR);
	   fCandTT.push_back(pCand);
	   break;
	 }
       }
     }
   }
   
   if ( mode == 2 ){ // For Matching Tag muon to Double Muon T.O.
     TLorentzVector tagD;
     TAnaTrack *pTagD(0);
     pTagD = fpEvt->getSigTrack(pCand->fSig1);
     tagD.SetPtEtaPhiM(pTagD->fPlab.Pt(), pTagD->fPlab.Eta(), pTagD->fPlab.Phi(), MMUON);
     for (int s = 0; s < fpEvt->nTrgObj() ; ++s) {
       pTrig = fpEvt->getTrgObj(s);
       if ( !(Label.CompareTo(pTrig->fLabel)) ) {
	 double tagD_dR = tagD.DeltaR(pTrig->fP);
	 double tagD_dEta = TMath::Abs(pTagD->fPlab.Eta() - pTrig->fP.Eta());
	 double tagD_dPhi = TMath::Abs(pTagD->fPlab.Phi() - pTrig->fP.Phi());
	 ((TH1D*)fpHistFile->Get("Tag_Dtrig_dEta"))->Fill(tagD_dEta);
	 ((TH1D*)fpHistFile->Get("Tag_Dtrig_dPhi"))->Fill(tagD_dPhi);
	 if ( ( tagD_dPhi < DPHI ) && ( tagD_dEta < DETA )) {
	   HLTlabel = true;				
	   ((TH1D*)fpHistFile->Get("Tag_Dtrig_dR_aftercuts"))->Fill(tagD_dR);
	   //cout << " Tag matched to Double mu T.O.  " << endl;
	   fCandTT2.push_back(pCand); 
	   t=s;
	   break;
	 } 
       }
     }
     
     if ( !HLTlabel ) {
       TAnaTrack *p(0);
       p = fpEvt->getSigTrack(pCand->fSig2);
       if ( Trigger > 0. ){
	 ((TH1D*)fpHistFile->Get("mmbar_Triggered_Tag_Pt"))->Fill(pTagD->fPlab.Pt());
	 ((TH1D*)fpHistFile->Get("mmbar_Triggered_Tag_Eta"))->Fill(pTagD->fPlab.Eta());
	 ((TH1D*)fpHistFile->Get("mmbar_Triggered_Probe_Pt"))->Fill(p->fPlab.Pt());
	 ((TH1D*)fpHistFile->Get("mmbar_Triggered_Probe_Eta"))->Fill(p->fPlab.Eta());	
       }
       
       if ( Trigger  < 0.) {
	 ((TH1D*)fpHistFile->Get("mmbar_NotTriggered_Tag_Pt"))->Fill(pTagD->fPlab.Pt());
	 ((TH1D*)fpHistFile->Get("mmbar_NotTriggered_Tag_Eta"))->Fill(pTagD->fPlab.Eta());
	 ((TH1D*)fpHistFile->Get("mmbar_NotTriggered_Probe_Pt"))->Fill(p->fPlab.Pt());
	 ((TH1D*)fpHistFile->Get("mmbar_NotTriggered_Probe_Eta"))->Fill(p->fPlab.Eta());
	 double dEta =  TMath::Abs(pTagD->fPlab.Eta() - p->fPlab.Eta());
	 TLorentzVector P;
	 P.SetPtEtaPhiM(p->fPlab.Pt(), p->fPlab.Eta(), p->fPlab.Phi(), MMUON);
	 double dr = tagD.DeltaR(P);
	 ((TH1D*)fpHistFile->Get("mmbar_NotTriggered_TagProbe_dEta"))->Fill(dEta);
	 ((TH1D*)fpHistFile->Get("mmbar_NotTriggered_TagProbe_dR"))->Fill(dr);
       }
       
       fCandnotTP.push_back(pCand);  // wiil be used for mmbar 
     }
     
   }
   
   if ( mode == 3 ){ // For Matching Probe muon to Double Muon T.O.
     TLorentzVector probe;
     TAnaTrack *pProbe(0);
     pProbe = fpEvt->getSigTrack(pCand->fSig2);
     probe.SetPtEtaPhiM(pProbe->fPlab.Pt(), pProbe->fPlab.Eta(), pProbe->fPlab.Phi(), MMUON);
     for (int s = 0; s < fpEvt->nTrgObj() ; ++s) {
       if ( s == t ) continue;
       pTrig = fpEvt->getTrgObj(s);
       if ( !(Label.CompareTo(pTrig->fLabel)) ) {
	 double probe_dR = probe.DeltaR(pTrig->fP);
	 double probe_dEta = TMath::Abs(pProbe->fPlab.Eta() - pTrig->fP.Eta());
	 double probe_dPhi = TMath::Abs(pProbe->fPlab.Phi() - pTrig->fP.Phi());
	 ((TH1D*)fpHistFile->Get("Probe_trig_dEta"))->Fill(probe_dEta);
	 ((TH1D*)fpHistFile->Get("Probe_trig_dPhi"))->Fill(probe_dPhi);
	 if ( ( probe_dPhi < DPHI ) && ( probe_dEta < DETA )) {
	   HLTlabel = true;
	   ((TH1D*)fpHistFile->Get("Probe_trig_dR_aftercuts"))->Fill(probe_dR);
	   //cout << " Probe to Double mu T.O. " << endl;
	   fCandTP.push_back(pCand); // will be used for mm
	   TAnaTrack *t(0);
	   t = fpEvt->getSigTrack(pCand->fSig1);
	   if ( Trigger > 0. ){
	     ((TH1D*)fpHistFile->Get("mm_Triggered_Tag_Pt"))->Fill(t->fPlab.Pt());
	     ((TH1D*)fpHistFile->Get("mm_Triggered_Tag_Eta"))->Fill(t->fPlab.Eta());
	     ((TH1D*)fpHistFile->Get("mm_Triggered_Probe_Pt"))->Fill(pProbe->fPlab.Pt());
	     ((TH1D*)fpHistFile->Get("mm_Triggered_Probe_Eta"))->Fill(pProbe->fPlab.Eta());
	     double deta =  TMath::Abs(t->fPlab.Eta() - pProbe->fPlab.Eta());
	     TLorentzVector T;
	     T.SetPtEtaPhiM(t->fPlab.Pt(), t->fPlab.Eta(), t->fPlab.Phi(), MMUON);
	     double dR = probe.DeltaR(T);
	     ((TH1D*)fpHistFile->Get("mm_Triggered_TagProbe_dEta"))->Fill(deta);
	     ((TH1D*)fpHistFile->Get("mm_Triggered_TagProbe_dR"))->Fill(dR);
	   }
	   break;
	}
       }
     }
     
     if ( !HLTlabel ) {
       fCandnotTP.push_back(pCand);  // wiil be used for mmbar 
     }
   }
  
   return HLTlabel;
 }



bool tnpReader2::isGoodProbe(TAnaCand *pCand){
  bool GoodProbe = false;
  
  TAnaTrack *pProbe(0);
  pProbe = fpEvt->getSigTrack(pCand->fSig2);
  if ( (pProbe->fMuID & 0x1<< MUTYPE1 ) && (pProbe->fMuID & 0x1<< MUTYPE2) ){
  //if ( pProbe->fMuID & 0x1<< MUTYPE2 ){  
    GoodProbe = true;
    fCandGP.push_back(pCand); // will be used for mm
    //cout << " GOOD PROBE " << endl;
  }
  if ( !(GoodProbe) )  fCandnotGP.push_back(pCand);  // wiil be used for mmbar 
  
    return GoodProbe;
}

bool tnpReader2::truthMatch(TAnaCand *pCand) {
  bool TruthMatch = false;
  TAnaTrack *pProbe(0);   TAnaTrack *pTag(0);
  TGenCand  *pGenP(0);   TGenCand  *pGenT(0);
  TGenCand  *pMomP(0);   TGenCand  *pMomT(0);
  pTag = fpEvt->getSigTrack(pCand->fSig1);
  pProbe = fpEvt->getSigTrack(pCand->fSig2);
  if ( (pTag->fGenIndex >= 0) && (pProbe->fGenIndex >= 0) ){
    
    pGenP = fpEvt->getGenCand(pProbe->fGenIndex);
    pGenT = fpEvt->getGenCand(pTag->fGenIndex);	
    //cout << "pGenP->fID = "  << pGenP->fID << endl; 
    //cout << "pGenT->fID = "  << pGenT->fID << endl;
    
    if ( abs(pGenP->fID) == 13 && abs(pGenT->fID) == 13 ){
      
      pMomP = fpEvt->getGenCand(pGenP->fMom1);
      pMomT = fpEvt->getGenCand(pGenT->fMom1);
      //cout << "pMomP->fID = " << pMomP->fID << endl;
      //cout << "pMomT->fID = " << pMomT->fID << endl;
      
      if ( pMomP->fID == RESTYPE && pMomT->fID == RESTYPE ){
	TruthMatch = true;
      }
    }
  }
  
  return TruthMatch;
}

// ----------------------------------------------------------------------
void tnpReader2::fillHist(TAnaCand *pCand,  int mode, bool CombineEndCaps) {
  
  TAnaTrack *pProbe(0);
  if ( mode == 1 ){
    pProbe = fpEvt->getSigTrack(pCand->fSig2);
    for ( int ieta = 0; ieta < fNeta; ++ieta ){
      for ( int ipt = 0; ipt < fNpt; ++ipt ){
	  
	if ( CombineEndCaps ){
	  if ( pProbe->fPlab.Eta() < -1.1 ){
	    if( ( pProbe->fPlab.Eta() <= fEtabin[ieta+1] ) && ( pProbe->fPlab.Eta() > fEtabin[ieta] ) ){
	      if ( ( pProbe->fPlab.Perp() >= fPTbin[ipt] ) && ( pProbe->fPlab.Perp() < fPTbin[ipt+1] ) ){
		if ( pProbe->fQ < 0 ){
		  ((TH1D*)fpHistFile->Get(Form("mt,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", fabs(fEtabin[ieta+1]), fabs(fEtabin[ieta]), fPTbin[ipt],
					       fPTbin[ipt+1],fQ[0])))->Fill(pCand->fMass);
		}
		if ( pProbe->fQ > 0 ){
		  ((TH1D*)fpHistFile->Get(Form("mt,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", fabs(fEtabin[ieta+1]), fabs(fEtabin[ieta]), fPTbin[ipt],
					       fPTbin[ipt+1],fQ[1])))->Fill(pCand->fMass);
		}
	      }
	    }
	  }
	  
	  
	  if ( pProbe->fPlab.Eta() > -1.1 ){
	    if( ( pProbe->fPlab.Eta() <= fEtabin[ieta+1] ) && ( pProbe->fPlab.Eta() > fEtabin[ieta] ) ){
	      if ( ( pProbe->fPlab.Perp() >= fPTbin[ipt] ) && ( pProbe->fPlab.Perp() < fPTbin[ipt+1] ) ){
		if ( pProbe->fQ < 0 ){
		  ((TH1D*)fpHistFile->Get(Form("mt,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", fEtabin[ieta], fEtabin[ieta+1], fPTbin[ipt],
					       fPTbin[ipt+1],fQ[0])))->Fill(pCand->fMass);
		}
		if ( pProbe->fQ > 0 ){
		  ((TH1D*)fpHistFile->Get(Form("mt,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", fEtabin[ieta], fEtabin[ieta+1], fPTbin[ipt],
					       fPTbin[ipt+1],fQ[1])))->Fill(pCand->fMass);
		}
	      }
	    }
	  }
	}
	
	if ( !CombineEndCaps ){
	  
	  if( ( pProbe->fPlab.Eta() <= fEtabin[ieta+1] ) && ( pProbe->fPlab.Eta() > fEtabin[ieta] ) ){
	    if ( ( pProbe->fPlab.Perp() >= fPTbin[ipt] ) && ( pProbe->fPlab.Perp() < fPTbin[ipt+1] ) ){
	      if ( pProbe->fQ < 0 ){
		((TH1D*)fpHistFile->Get(Form("mt,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", fEtabin[ieta], fEtabin[ieta+1], fPTbin[ipt],
					     fPTbin[ipt+1],fQ[0])))->Fill(pCand->fMass);
	      }
	      if ( pProbe->fQ > 0 ){
		((TH1D*)fpHistFile->Get(Form("mt,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", fEtabin[ieta], fEtabin[ieta+1], fPTbin[ipt],
					     fPTbin[ipt+1],fQ[1])))->Fill(pCand->fMass);
	      }
	    }
	  }
	  
	}
	
      }
    }
  } else  if ( mode == 2 ){
    pProbe = fpEvt->getSigTrack(pCand->fSig2);
    for ( int ieta = 0; ieta < fNeta; ++ieta ){
      for ( int ipt = 0; ipt < fNpt; ++ipt ){
	
	if ( CombineEndCaps ){
	  if ( pProbe->fPlab.Eta() < -1.1 ){
	    if( ( pProbe->fPlab.Eta() <= fEtabin[ieta+1] ) && ( pProbe->fPlab.Eta() > fEtabin[ieta] ) ){
	      if ( ( pProbe->fPlab.Perp() >= fPTbin[ipt] ) && ( pProbe->fPlab.Perp() < fPTbin[ipt+1] ) ){
		if ( pProbe->fQ < 0 ){
		  ((TH1D*)fpHistFile->Get(Form("mm,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", fabs(fEtabin[ieta+1]), fabs(fEtabin[ieta]), fPTbin[ipt],
					       fPTbin[ipt+1],fQ[0])))->Fill(pCand->fMass);
		}
		if ( pProbe->fQ > 0 ){
		  ((TH1D*)fpHistFile->Get(Form("mm,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", fabs(fEtabin[ieta+1]), fabs(fEtabin[ieta]), fPTbin[ipt],
					       fPTbin[ipt+1],fQ[1])))->Fill(pCand->fMass);
		}
	      }
	    }
	  }
	  
	  
	  if ( pProbe->fPlab.Eta() > -1.1 ){
	    if( ( pProbe->fPlab.Eta() <= fEtabin[ieta+1] ) && ( pProbe->fPlab.Eta() > fEtabin[ieta] ) ){
	      if ( ( pProbe->fPlab.Perp() >= fPTbin[ipt] ) && ( pProbe->fPlab.Perp() < fPTbin[ipt+1] ) ){
		if ( pProbe->fQ < 0 ){
		  ((TH1D*)fpHistFile->Get(Form("mm,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", fEtabin[ieta], fEtabin[ieta+1], fPTbin[ipt],
					       fPTbin[ipt+1],fQ[0])))->Fill(pCand->fMass);
		}
		if ( pProbe->fQ > 0 ){
		  ((TH1D*)fpHistFile->Get(Form("mm,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", fEtabin[ieta], fEtabin[ieta+1], fPTbin[ipt],
					       fPTbin[ipt+1],fQ[1])))->Fill(pCand->fMass);
		}
	      }
	    }
	  }
	}
	
	if ( !CombineEndCaps ){
	  
	  if( ( pProbe->fPlab.Eta() <= fEtabin[ieta+1] ) && ( pProbe->fPlab.Eta() > fEtabin[ieta] ) ){
	    if ( ( pProbe->fPlab.Perp() >= fPTbin[ipt] ) && ( pProbe->fPlab.Perp() < fPTbin[ipt+1] ) ){
	      if ( pProbe->fQ < 0 ){
		((TH1D*)fpHistFile->Get(Form("mm,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", fEtabin[ieta], fEtabin[ieta+1], fPTbin[ipt],
					     fPTbin[ipt+1],fQ[0])))->Fill(pCand->fMass);
	      }
	      if ( pProbe->fQ > 0 ){
		((TH1D*)fpHistFile->Get(Form("mm,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", fEtabin[ieta], fEtabin[ieta+1], fPTbin[ipt],
					     fPTbin[ipt+1],fQ[1])))->Fill(pCand->fMass);
	      }
	    }
	  }
	  
	}
	
      }
    }
  } else if ( mode == 3 ){
    pProbe = fpEvt->getSigTrack(pCand->fSig2);
    for ( int ieta = 0; ieta < fNeta; ++ieta ){
      for ( int ipt = 0; ipt < fNpt; ++ipt ){
	  
	if ( CombineEndCaps ){
	  if ( pProbe->fPlab.Eta() < -1.1 ){
	    if( ( pProbe->fPlab.Eta() <= fEtabin[ieta+1] ) && ( pProbe->fPlab.Eta() > fEtabin[ieta] ) ){
	      if ( ( pProbe->fPlab.Perp() >= fPTbin[ipt] ) && ( pProbe->fPlab.Perp() < fPTbin[ipt+1] ) ){
		if ( pProbe->fQ < 0 ){
		  ((TH1D*)fpHistFile->Get(Form("mmbar,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", fabs(fEtabin[ieta+1]), fabs(fEtabin[ieta]), fPTbin[ipt],
					       fPTbin[ipt+1],fQ[0])))->Fill(pCand->fMass);
		}
		if ( pProbe->fQ > 0 ){
		  ((TH1D*)fpHistFile->Get(Form("mmbar,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", fabs(fEtabin[ieta+1]), fabs(fEtabin[ieta]), fPTbin[ipt],
					       fPTbin[ipt+1],fQ[1])))->Fill(pCand->fMass);
		}
	      }
	    }
	  }
	    
	  if ( pProbe->fPlab.Eta() > -1.1 ){
	    if( ( pProbe->fPlab.Eta() <= fEtabin[ieta+1] ) && ( pProbe->fPlab.Eta() > fEtabin[ieta] ) ){
	      if ( ( pProbe->fPlab.Perp() >= fPTbin[ipt] ) && ( pProbe->fPlab.Perp() < fPTbin[ipt+1] ) ){
		if ( pProbe->fQ < 0 ){
		  ((TH1D*)fpHistFile->Get(Form("mmbar,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", fEtabin[ieta], fEtabin[ieta+1], fPTbin[ipt],
					       fPTbin[ipt+1],fQ[0])))->Fill(pCand->fMass);
		}
		if ( pProbe->fQ > 0 ){
		  ((TH1D*)fpHistFile->Get(Form("mmbar,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", fEtabin[ieta], fEtabin[ieta+1], fPTbin[ipt],
					       fPTbin[ipt+1],fQ[1])))->Fill(pCand->fMass);
		}
	      }
	    }
	  }
	}
	
	
	if ( !CombineEndCaps ){
	  
	  if( ( pProbe->fPlab.Eta() <= fEtabin[ieta+1] ) && ( pProbe->fPlab.Eta() > fEtabin[ieta] ) ){
	    if ( ( pProbe->fPlab.Perp() >= fPTbin[ipt] ) && ( pProbe->fPlab.Perp() < fPTbin[ipt+1] ) ){
	      if ( pProbe->fQ < 0 ){
		((TH1D*)fpHistFile->Get(Form("mmbar,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", fEtabin[ieta], fEtabin[ieta+1], fPTbin[ipt],
					     fPTbin[ipt+1],fQ[0])))->Fill(pCand->fMass);
	      }
	      if ( pProbe->fQ > 0 ){
		((TH1D*)fpHistFile->Get(Form("mmbar,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", fEtabin[ieta], fEtabin[ieta+1], fPTbin[ipt],
					     fPTbin[ipt+1],fQ[1])))->Fill(pCand->fMass);
	      }
	    }
	  }
	  
	}
	
      }
    }
  }   if ( mode == 4 ){
    pProbe = fpEvt->getSigTrack(pCand->fSig2);
    for ( int ieta = 0; ieta < fNeta; ++ieta ){
      for ( int ipt = 0; ipt < fNpt; ++ipt ){
	
	if ( CombineEndCaps ){
	  if ( pProbe->fPlab.Eta() < -1.1 ){
	    if( ( pProbe->fPlab.Eta() <= fEtabin[ieta+1] ) && ( pProbe->fPlab.Eta() > fEtabin[ieta] ) ){
	      if ( ( pProbe->fPlab.Perp() >= fPTbin[ipt] ) && ( pProbe->fPlab.Perp() < fPTbin[ipt+1] ) ){
		if ( pProbe->fQ < 0 ){
		  ((TH1D*)fpHistFile->Get(Form("mtMatched,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", fabs(fEtabin[ieta+1]), fabs(fEtabin[ieta]), fPTbin[ipt],
					       fPTbin[ipt+1],fQ[0])))->Fill(pCand->fMass);
		}
		if ( pProbe->fQ > 0 ){
		  ((TH1D*)fpHistFile->Get(Form("mtMatched,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", fabs(fEtabin[ieta+1]), fabs(fEtabin[ieta]), fPTbin[ipt],
					       fPTbin[ipt+1],fQ[1])))->Fill(pCand->fMass);
		}
	      }
	    }
	  }
	  
	  if ( pProbe->fPlab.Eta() > -1.1 ){
	    if( ( pProbe->fPlab.Eta() <= fEtabin[ieta+1] ) && ( pProbe->fPlab.Eta() > fEtabin[ieta] ) ){
	      if ( ( pProbe->fPlab.Perp() >= fPTbin[ipt] ) && ( pProbe->fPlab.Perp() < fPTbin[ipt+1] ) ){
		if ( pProbe->fQ < 0 ){
		  ((TH1D*)fpHistFile->Get(Form("mtMatched,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", fEtabin[ieta], fEtabin[ieta+1], fPTbin[ipt],
					       fPTbin[ipt+1],fQ[0])))->Fill(pCand->fMass);
		}
		if ( pProbe->fQ > 0 ){
		  ((TH1D*)fpHistFile->Get(Form("mtMatched,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", fEtabin[ieta], fEtabin[ieta+1], fPTbin[ipt],
					       fPTbin[ipt+1],fQ[1])))->Fill(pCand->fMass);
		}
	      }
	    }
	  }
	}
	
	if ( !CombineEndCaps ){
	  
	  if( ( pProbe->fPlab.Eta() <= fEtabin[ieta+1] ) && ( pProbe->fPlab.Eta() > fEtabin[ieta] ) ){
	    if ( ( pProbe->fPlab.Perp() >= fPTbin[ipt] ) && ( pProbe->fPlab.Perp() < fPTbin[ipt+1] ) ){
	      if ( pProbe->fQ < 0 ){
		((TH1D*)fpHistFile->Get(Form("mtMatched,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", fEtabin[ieta], fEtabin[ieta+1], fPTbin[ipt],
					     fPTbin[ipt+1],fQ[0])))->Fill(pCand->fMass);
	      }
	      if ( pProbe->fQ > 0 ){
		((TH1D*)fpHistFile->Get(Form("mtMatched,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", fEtabin[ieta], fEtabin[ieta+1], fPTbin[ipt],
					     fPTbin[ipt+1],fQ[1])))->Fill(pCand->fMass);
	      }
	    }
	  }
	  
	}
	
      }
    }
  } else  if ( mode == 5 ){
    pProbe = fpEvt->getSigTrack(pCand->fSig2);
    for ( int ieta = 0; ieta < fNeta; ++ieta ){
      for ( int ipt = 0; ipt < fNpt; ++ipt ){
	
	if ( CombineEndCaps ){
	  if ( pProbe->fPlab.Eta() < -1.1 ){
	    if( ( pProbe->fPlab.Eta() <= fEtabin[ieta+1] ) && ( pProbe->fPlab.Eta() > fEtabin[ieta] ) ){
	      if ( ( pProbe->fPlab.Perp() >= fPTbin[ipt] ) && ( pProbe->fPlab.Perp() < fPTbin[ipt+1] ) ){
		if ( pProbe->fQ < 0 ){
		  ((TH1D*)fpHistFile->Get(Form("mmMatched,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", fabs(fEtabin[ieta+1]), fabs(fEtabin[ieta]), fPTbin[ipt],
					       fPTbin[ipt+1],fQ[0])))->Fill(pCand->fMass);
		}
		if ( pProbe->fQ > 0 ){
		  ((TH1D*)fpHistFile->Get(Form("mmMatched,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", fabs(fEtabin[ieta+1]), fabs(fEtabin[ieta]), fPTbin[ipt],
					       fPTbin[ipt+1],fQ[1])))->Fill(pCand->fMass);
		}
	      }
	    }
	  }
	    
	  if ( pProbe->fPlab.Eta() > -1.1 ){
	    if( ( pProbe->fPlab.Eta() <= fEtabin[ieta+1] ) && ( pProbe->fPlab.Eta() > fEtabin[ieta] ) ){
	      if ( ( pProbe->fPlab.Perp() >= fPTbin[ipt] ) && ( pProbe->fPlab.Perp() < fPTbin[ipt+1] ) ){
		if ( pProbe->fQ < 0 ){
		  ((TH1D*)fpHistFile->Get(Form("mmMatched,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", fEtabin[ieta], fEtabin[ieta+1], fPTbin[ipt],
					       fPTbin[ipt+1],fQ[0])))->Fill(pCand->fMass);
		}
		if ( pProbe->fQ > 0 ){
		  ((TH1D*)fpHistFile->Get(Form("mmMatched,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", fEtabin[ieta], fEtabin[ieta+1], fPTbin[ipt],
					       fPTbin[ipt+1],fQ[1])))->Fill(pCand->fMass);
		}
	      }
	    }
	  }
	}
	
	if ( !CombineEndCaps ){ 
	  
	  if( ( pProbe->fPlab.Eta() <= fEtabin[ieta+1] ) && ( pProbe->fPlab.Eta() > fEtabin[ieta] ) ){
	    if ( ( pProbe->fPlab.Perp() >= fPTbin[ipt] ) && ( pProbe->fPlab.Perp() < fPTbin[ipt+1] ) ){
	      if ( pProbe->fQ < 0 ){
		((TH1D*)fpHistFile->Get(Form("mmMatched,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", fEtabin[ieta], fEtabin[ieta+1], fPTbin[ipt],
					     fPTbin[ipt+1],fQ[0])))->Fill(pCand->fMass);
	      }
	      if ( pProbe->fQ > 0 ){
		((TH1D*)fpHistFile->Get(Form("mmMatched,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", fEtabin[ieta], fEtabin[ieta+1], fPTbin[ipt],
					     fPTbin[ipt+1],fQ[1])))->Fill(pCand->fMass);
	      }
	    }
	  }
	  
	}
	
      }
    }
  }
  
}

// ----------------------------------------------------------------------
void tnpReader2::bookHist() {
  
  TH1 *h;
  TH2 *k;
  
  fBin = BIN;
  fMassLow = MASSLO;
  fMassHigh = MASSHI;
    
  k = new TH2D("mt,pt-eta", "mt,pt-eta", fNpt, fPTbin, fNeta, fEtabin);
  k = new TH2D("mmmbar,pt-eta", "mmbar,pt-eta", fNpt, fPTbin, fNeta, fEtabin);
  
  h = new TH1D("Tag_trig_dR_aftercuts",  "Tag_trig_dR_aftercuts", 50, 0., 1.);
  h = new TH1D("Tag_trig_dEta", "Tag_trig_dEta", 50, 0., 1.);
  h = new TH1D("Tag_trig_dPhi", "Tag_trig_dPhi", 50, 0., 1.);
  
  h = new TH1D("Tag_Dtrig_dR_aftercuts",  "Tag_Dtrig_dR_aftercuts", 50, 0., 1.);
  h = new TH1D("Tag_Dtrig_dEta", "Tag_Dtrig_dEta", 50, 0., 1.);
  h = new TH1D("Tag_Dtrig_dPhi", "Tag_Dtrig_dPhi", 50, 0., 1.);
  
  h = new TH1D("Probe_trig_dR_aftercuts",  "Probe_trig_dR_aftercuts", 50, 0., 1.);
  h = new TH1D("Probe_trig_dEta", "Probe_trig_dEta", 50, 0., 1.);
  h = new TH1D("Probe_trig_dPhi", "Probe_trig_dPhi", 50, 0., 1.); 
  
  h = new TH1D("GenJpsi", "GenJpsi", 5, 0., 5.);
  
  h = new TH1D("mmbar_Triggered_Tag_Pt","mmbar_Triggered_Tag_Pt", 40, 0., 40.);
  h = new TH1D("mmbar_Triggered_Tag_Eta","mmbar_Triggered_Tag_Eta", 60, -3., 3.);
  h = new TH1D("mmbar_Triggered_Probe_Pt","mmbar_Triggered_Probe_Pt", 40, 0., 40.);
  h = new TH1D("mmbar_Triggered_Probe_Eta","mmbar_Triggered_Probe_Eta", 60, -3., 3.);
  h = new TH1D("mmbar_NotTriggered_Tag_Pt","mmbar_NotTriggered_Tag_Pt", 40, 0., 40.);
  h = new TH1D("mmbar_NotTriggered_Tag_Eta","mmbar_NotTriggered_Tag_Eta", 60, -3., 3.);
  h = new TH1D("mmbar_NotTriggered_Probe_Pt","mmbar_NotTriggered_Probe_Pt", 40, 0., 40.);
  h = new TH1D("mmbar_NotTriggered_Probe_Eta","mmbar_NotTriggered_Probe_Eta", 60, -3., 3.);
  h = new TH1D("mmbar_NotTriggered_TagProbe_dEta","mmbar_NotTriggered_TagProbe_dEta", 40, 0., 2.);
  h = new TH1D("mmbar_NotTriggered_TagProbe_dR","mmbar_NotTriggered_TagProbe_dR", 40, 0., 2.);
  h = new TH1D("mm_Triggered_Tag_Pt","mm_Triggered_Tag_Pt", 40, 0., 40.);
  h = new TH1D("mm_Triggered_Tag_Eta","mm_Triggered_Tag_Eta", 60, -3., 3.);
  h = new TH1D("mm_Triggered_Probe_Pt","mm_Triggered_Probe_Pt", 40, 0., 40.);
  h = new TH1D("mm_Triggered_Probe_Eta","mm_Triggered_Probe_Eta", 60, -3., 3.);
  h = new TH1D("mm_Triggered_TagProbe_dEta","mm_Triggered_TagProbe_dEta", 40, 0., 2.);
  h = new TH1D("mm_Triggered_TagProbe_dR","mm_Triggered_TagProbe_dR", 40, 0., 2.);
  
  // Infoo Histograms
  h = new TH1D("Tag_pt","Tag_pt", 40, 0., 40.);
  h = new TH1D("Cand_MaxDoca","Cand_MaxDoca", 50, 0., 0.05);
  h = new TH1D("Cand_Chi2","Cand_Chi2", 50, 0., 10.);

  //MC Truth Histograms
  k = new TH2D("tEtaPt_neg", "tEtaPt_neg", fNeta, fEtabin, fNpt, fPTbin);
  k = new TH2D("tEtaPt_pos", "tEtaPt_pos", fNeta, fEtabin, fNpt, fPTbin);	
  k = new TH2D("mEtaPt_neg", "mEtaPt_neg", fNeta, fEtabin, fNpt, fPTbin);	
  k = new TH2D("mEtaPt_pos", "mEtaPt_pos", fNeta, fEtabin, fNpt, fPTbin);

  //  Efficiency Histograms
  for ( int ieta = 0; ieta < fNeta; ++ieta ){
    for ( int ipt = 0; ipt < fNpt; ++ipt ){
      for ( int iq = 0; iq <= fNq; ++iq ){
	h = new TH1D(Form("mt,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", fEtabin[ieta], fEtabin[ieta+1], fPTbin[ipt], fPTbin[ipt+1], fQ[iq]),
		     Form("mt,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", fEtabin[ieta], fEtabin[ieta+1], fPTbin[ipt], fPTbin[ipt+1], fQ[iq]),
		     fBin, fMassLow, fMassHigh);
      }
    }	
  }
  
  for ( int ieta = 0; ieta < fNeta; ++ieta ){
    for ( int ipt = 0; ipt < fNpt; ++ipt ){
      for ( int iq = 0; iq <= fNq; ++iq ){
	h = new TH1D(Form("mm,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", fEtabin[ieta], fEtabin[ieta+1], fPTbin[ipt], fPTbin[ipt+1], fQ[iq]),
		     Form("mm,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", fEtabin[ieta], fEtabin[ieta+1], fPTbin[ipt], fPTbin[ipt+1], fQ[iq]),
		     fBin, fMassLow, fMassHigh);
      }
    }	
  }
  
  for ( int ieta = 0; ieta < fNeta; ++ieta ){
    for ( int ipt = 0; ipt < fNpt; ++ipt ){
      for ( int iq = 0; iq <= fNq; ++iq ){
	h = new TH1D(Form("mmbar,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", fEtabin[ieta], fEtabin[ieta+1], fPTbin[ipt], fPTbin[ipt+1], fQ[iq]),
		     Form("mmbar,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", fEtabin[ieta], fEtabin[ieta+1], fPTbin[ipt], fPTbin[ipt+1], fQ[iq]),
		     fBin, fMassLow, fMassHigh);
      }
    }	
  }  
  
  for ( int ieta = 0; ieta < fNeta; ++ieta ){
    for ( int ipt = 0; ipt < fNpt; ++ipt ){
      for ( int iq = 0; iq <= fNq; ++iq ){
	h = new TH1D(Form("mtMatched,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", fEtabin[ieta], fEtabin[ieta+1], fPTbin[ipt], fPTbin[ipt+1], fQ[iq]),
		     Form("mtMatched,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", fEtabin[ieta], fEtabin[ieta+1], fPTbin[ipt], fPTbin[ipt+1], fQ[iq]),
		     fBin, fMassLow, fMassHigh);
      }
    }	
  }
  
  for ( int ieta = 0; ieta < fNeta; ++ieta ){
    for ( int ipt = 0; ipt < fNpt; ++ipt ){
      for ( int iq = 0; iq <= fNq; ++iq ){
	h = new TH1D(Form("mmMatched,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", fEtabin[ieta], fEtabin[ieta+1], fPTbin[ipt], fPTbin[ipt+1], fQ[iq]),
		     Form("mmMatched,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", fEtabin[ieta], fEtabin[ieta+1], fPTbin[ipt], fPTbin[ipt+1], fQ[iq]),
		     fBin, fMassLow, fMassHigh);
      }
    }	
  }
  
}


// --------------------------------------------------------------------------------------------------
void tnpReader2::readCuts(TString filename, int dump) {
  char  buffer[200];
  fCutFile = filename;
  if (dump) cout << "Reading " << fCutFile.Data() << " for tnpReader2 cut settings" << endl;
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
    
    if (!strcmp(CutName, "SAMPLE")) {
      SAMPLE = int(CutValue); ok = 1;
      if (dump) cout << "SAMPLE:          " << SAMPLE << endl;
    }
    
    if (!strcmp(CutName, "MODE")) {
      MODE = int(CutValue); ok = 1;
      if (dump) cout << "MODE:            " << MODE << endl;
    }
    
    if (!strcmp(CutName, "TYPE")) {
      TYPE = int(CutValue); ok = 1;
      if (dump) cout << "TYPE:           " << TYPE << endl;
    }
    
    if (!strcmp(CutName, "RESTYPE")) {
      RESTYPE = int(CutValue); ok = 1;
      if (dump) cout << "RESTYPE:        " << RESTYPE << endl;
    }
    
    if (!strcmp(CutName, "HLTPATH_TAG")) {
      HLTPATH_TAG = SetName; ok = 1;
      if (dump) cout << "HLTPATH_TAG:   " << HLTPATH_TAG  << endl;
    }    
    
    if (!strcmp(CutName, "HLTPATH_PROBE")) {
      HLTPATH_PROBE = SetName; ok = 1;
      if (dump) cout << "HLTPATH_PROBE: " << HLTPATH_PROBE  << endl;
    }
    
    if (!strcmp(CutName, "MUTYPE1")) {
      MUTYPE1 = int(CutValue); ok = 1;
      if (dump) cout << "MUTYPE1:         " << MUTYPE1 << endl;
    }
    
    if (!strcmp(CutName, "MUTYPE2")) {
      MUTYPE2 = int(CutValue); ok = 1;
      if (dump) cout << "MUTYPE2:         " << MUTYPE2 << endl;
    }   
    
    if (!strcmp(CutName, "MASSLO")) {
      MASSLO = CutValue; ok = 1;
      if (dump) cout << "MASSLO:         " << MASSLO << endl;
    }
    
    if (!strcmp(CutName, "MASSHI")) {
      MASSHI = CutValue; ok = 1;
      if (dump) cout << "MASSHI:        " << MASSHI << endl;
    }
    
    if (!strcmp(CutName, "BIN")) {
      BIN = int(CutValue); ok = 1;
      if (dump) cout << "BIN:            " << BIN << endl;
    }     
    
    if (!strcmp(CutName, "MAXDOCA")) {
      MAXDOCA = CutValue; ok = 1;
      if (dump) cout << "MAXDOCA:       " << MAXDOCA << endl;
    }     
    
    if (!strcmp(CutName, "DETA")) {
      DETA = CutValue; ok = 1;
      if (dump) cout << "DETA:           " << DETA << endl;
    } 
    
    if (!strcmp(CutName, "DPHI")) {
      DPHI = CutValue; ok = 1;
      if (dump) cout << "DPHI:           " << DPHI << endl;
    }     	  
    
    if (!strcmp(CutName, "PT_TAG")) {
      PT_TAG = CutValue; ok = 1;
      if (dump) cout << "PT_TAG:          " << PT_TAG << endl;
      ibin = 11;
      hcuts->SetBinContent(ibin, PT_TAG);
      hcuts->GetXaxis()->SetBinLabel(ibin, "p_{T}^{Tag}(l) [GeV]");
    }
    
    if (!strcmp(CutName, "PT_PROBE")) {
      PT_PROBE = CutValue; ok = 1;
      if (dump) cout << "PT_PROBE:        " << PT_PROBE << endl;
      ibin = 12;
      hcuts->SetBinContent(ibin, PT_PROBE);
      hcuts->GetXaxis()->SetBinLabel(ibin, "p_{T}^{Probe}(l) [GeV]");
    }
    
    
    if (!strcmp(CutName, "ETALO_TAG")) {
      ETALO_TAG = CutValue; ok = 1;
      if (dump) cout << "ETALO_TAG:     " << ETALO_TAG << endl;
      ibin = 13;
      hcuts->SetBinContent(ibin, ETALO_TAG);
      hcuts->GetXaxis()->SetBinLabel(ibin, "#eta_{Tag}^{min}(l)");
    }
    
    if (!strcmp(CutName, "ETAHI_TAG")) {
      ETAHI_TAG = CutValue; ok = 1;
      if (dump) cout << "ETAHI_TAG:      " << ETAHI_TAG << endl;
      ibin = 14;
      hcuts->SetBinContent(ibin, ETAHI_TAG);
      hcuts->GetXaxis()->SetBinLabel(ibin, "#eta_{Tag}^{max}(l)");
    }
    
    if (!strcmp(CutName, "HLTLABEL_TAG")) {
      HLTLABEL_TAG = SetName; ok = 1;
      if (dump) cout << "HLTLABEL_TAG:   " << HLTLABEL_TAG  << endl;
    } 
    
    if (!strcmp(CutName, "HLTLABEL_PROBE")) {
      HLTLABEL_PROBE = SetName; ok = 1;
      if (dump) cout << "HLTLABEL_PROBE:   " << HLTLABEL_PROBE  << endl;
    } 
    
    if (!ok) cout << "==> ERROR: Don't know about variable " << CutName << endl;
  }
  
  if (dump)  cout << "------------------------------------" << endl;
}

