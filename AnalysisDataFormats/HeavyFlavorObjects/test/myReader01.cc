#include "myReader01.hh"
#include <vector>
#include "TRandom.h"
#define MMUON 0.10566
#define MKAON 0.49368

// ----------------------------------------------------------------------
// Run with: ./runMyReader01 -c chains/bg-test -D root 
//           ./runMyReader01 -f test.root 
// ----------------------------------------------------------------------


// ----------------------------------------------------------------------
myReader01::myReader01(TChain *tree, TString evtClassName): treeReader01(tree, evtClassName) {
  cout << "--> myReader01> This is the start ..." << endl;
}
// ----------------------------------------------------------------------
myReader01::~myReader01() {
  cout << "--> myReader01> This is the end ..." << endl;
}

// ----------------------------------------------------------------------
void myReader01::startAnalysis() {
  cout << "--> myReader01> startAnalysis: ..." << endl;
}


void myReader01::eventProcessing() {
  TAnaTrack *pTrack, *pTrack_tag;
  TAnaTrack *pTrack1, *pTrack2;
  TAnaCand  *pCand;
  TAnaMuon  *pMuon;
  TGenCand *pGen1, *pGen2, *pGEn1, *pGEn2;
  TGenCand *pGen, *pgen;  
  fRun = fpEvt->fRunNumber;  
  TLorentzVector m1, m2, res;
  
  
  
  int Npt = 6;
  double PTbin[] = {0., 2., 3., 4., 5., 6., 20.};		 
  									
  int Neta = 5;							
  double Etabin[] = {-2.4, -1.2, -0.4, 0.4, 1.2, 2.4};
  
 // int Npt = 7;
 // double PTbin[] = {0., 2., 3., 4., 5., 6., 10., 20.};		 
  								
 // int Neta = 16;							
 // double Etabin[] = {-2.4, -2.1, -1.8, -1.5, -1.2, -0.9, -0.6, -0.3, 0., 0.3, 0.6, 0.9, 1.2, 1.5, 1.8, 2.1, 2.4};      

  int npt = 12;
  double pTbin[] = {0., 2., 2.5, 3., 3.5, 4., 4.5, 5., 5.5, 6., 7., 9., 20.};		//	used for t/m_EtaPT_neg/pos
												// 	to compare with J/Psi 
  int neta = 14;										// 	with J/Psi binning
  double etabin[] = {-2.4, -1.8, -1.5, -1.2, -1.0, -0.8, -0.6, 0., 0.6, 0.8, 1.0, 1.2, 1.5, 1.8, 2.4};
  int ncharge = 1;
  int charge[] = {-1, 1};
  
  
   cout << "----------------------------------------------------------------------" << endl;
   cout << "myReader01> Found " << fpEvt->nGenCands() << " gen cands in event" << endl;
   cout << "myReader01> Found " << fpEvt->nSigTracks() << " sig tracks in event" << endl;
   cout << "myReader01> Found " << fpEvt->nRecTracks() << " rec tracks in event" << endl;
   cout << "myReader01> Found " << fpEvt->nCands() << " cands in event" << endl;
   cout << "----------------------------------------------------------------------" << endl;
   ((TH1D*)fpHistFile->Get("n403"))->Fill(fpEvt->nGenCands());
   ((TH1D*)fpHistFile->Get("n402"))->Fill(fpEvt->nCands());
   ((TH1D*)fpHistFile->Get("n400"))->Fill(fpEvt->nRecTracks());
   ((TH1D*)fpHistFile->Get("n401"))->Fill(fpEvt->nSigTracks());     
  
  
  fRes = 100553;
  
/////////////////////////////////////////////////  ---- Truth Candidate

  cout << "------------------------------ Truth Candidate Stuff ----------------------------------------" << endl;
  
  if ( fpEvt->nCands() > 0){
  	for (int it = 0; it < fpEvt->nCands(); ++it) {
		pCand = fpEvt->getCand(it);
		if ( pCand->fType == -67 ){
			((TH1D*)fpHistFile->Get("ct100"))->Fill(pCand->fMass);
    			((TH1D*)fpHistFile->Get("ct101"))->Fill(pCand->fPlab.Perp()); 
   			((TH1D*)fpHistFile->Get("ct102"))->Fill(pCand->fPlab.Eta());
   			((TH1D*)fpHistFile->Get("ct103"))->Fill(pCand->fPlab.Phi());
			}
			
		if ( pCand->fType == 67 ){
			((TH1D*)fpHistFile->Get("ct100a"))->Fill(pCand->fMass);
    			((TH1D*)fpHistFile->Get("ct101a"))->Fill(pCand->fPlab.Perp()); 
   			((TH1D*)fpHistFile->Get("ct102a"))->Fill(pCand->fPlab.Eta());
   			((TH1D*)fpHistFile->Get("ct103a"))->Fill(pCand->fPlab.Phi());
			}	  
  	}
 }
  
     
////////////////////////////////////////////////  ---- Probe Muons Kinematics 
  
  cout << "------------------------------ Probe Muons Kinematics ----------------------------------------" << endl;
  
  int globalmuon(0), trakermuon(0), standalonemuon(0); 
  for (int s = 1; s < fpEvt->nSigTracks(); ++s) {
  	if ( s%2 == 0 ) continue;
  	pTrack = fpEvt->getSigTrack(s);
	((TH1D*)fpHistFile->Get("sMuID"))->Fill(pTrack->fMuID);
	
	if ( pTrack->fMuID == -1 ) continue; // Make sure that track is reconstructed as a muon !!
	
	if ( pTrack->fMuID & 0x1<<1 ) {
		globalmuon++;
		cout << " Global Muon " << s << endl;		
		((TH1D*)fpHistFile->Get("sG101"))->Fill(pTrack->fPlab.Perp());
		
		if ( (pTrack->fPlab.Eta() >= 1.04) || (pTrack->fPlab.Eta() <= -1.04) ) ((TH1D*)fpHistFile->Get("sG101b"))->Fill(pTrack->fPlab.Perp());
			else ((TH1D*)fpHistFile->Get("sG101a"))->Fill(pTrack->fPlab.Perp());
		if ( pTrack->fPlab.Perp() <= 2. ) ((TH1D*)fpHistFile->Get("sG102a"))->Fill(pTrack->fPlab.Eta());
		((TH1D*)fpHistFile->Get("sG102"))->Fill(pTrack->fPlab.Eta());	
	}
	
	if ( pTrack->fMuID & 0x1<<2 ) {
		trakermuon++;	
		cout << " Tracker Muon " << endl;
		((TH1D*)fpHistFile->Get("sT101"))->Fill(pTrack->fPlab.Perp());
		
		if ( (pTrack->fPlab.Eta() >= 1.04) || (pTrack->fPlab.Eta() <= -1.04) ) ((TH1D*)fpHistFile->Get("sT101b"))->Fill(pTrack->fPlab.Perp());
			else ((TH1D*)fpHistFile->Get("sT101a"))->Fill(pTrack->fPlab.Perp());
		if ( pTrack->fPlab.Perp() <= 2. ) ((TH1D*)fpHistFile->Get("sT102a"))->Fill(pTrack->fPlab.Eta());
		((TH1D*)fpHistFile->Get("sT102"))->Fill(pTrack->fPlab.Eta());
	}
	
        if ( pTrack->fMuID & 0x1<<0 ) {
		standalonemuon++;
		cout << " Standalone Muon " << endl;
		((TH1D*)fpHistFile->Get("sS101"))->Fill(pTrack->fPlab.Perp());
		
		if ( (pTrack->fPlab.Eta() >= 1.04) || (pTrack->fPlab.Eta() <= -1.04) ) ((TH1D*)fpHistFile->Get("sS101b"))->Fill(pTrack->fPlab.Perp());
			else ((TH1D*)fpHistFile->Get("sS101a"))->Fill(pTrack->fPlab.Perp());
		if ( pTrack->fPlab.Perp() <= 2. ) ((TH1D*)fpHistFile->Get("sS102a"))->Fill(pTrack->fPlab.Eta());
		((TH1D*)fpHistFile->Get("sS102"))->Fill(pTrack->fPlab.Eta());
	}
		
   }
   
   
   
   ((TH1D*)fpHistFile->Get("nG"))->Fill(globalmuon);
   ((TH1D*)fpHistFile->Get("nT"))->Fill(trakermuon);
   ((TH1D*)fpHistFile->Get("nS"))->Fill(standalonemuon);

  
  
 
      
      
////////////////////////////////////////////////  ---- Tag Muons Kinematics 
  
  cout << "------------------------------ Tag Muons Kinematics ----------------------------------------" << endl;
  
  int Globalmuon(0); 
  for (int s = 0; s < fpEvt->nSigTracks(); ++s) {
  	if ( s%2 == 1 ) continue;
  	pTrack = fpEvt->getSigTrack(s);
	
	pTrack_tag = fpEvt->getRecTrack(pTrack->fIndex);
	((TH1D*)fpHistFile->Get("stagMuID"))->Fill(pTrack_tag->fMuID);
		
	if ( pTrack_tag->fMuID == -1 ) continue; // Make sure that track is reconstructed as a muon !!
	
	if ( pTrack_tag->fPlab.Perp() < 3. ) continue;
	if ( pTrack_tag->fPlab.Eta() > 2.4 ) continue;
	if ( pTrack_tag->fPlab.Eta() < -2.4 ) continue;
	
	if ( !((pTrack_tag->fMuID & 0x1<<1) && (pTrack_tag->fMuID & 0x1<<2)) ) continue;
		Globalmuon++;
		cout << " Global Muon " << s << endl;		
		((TH1D*)fpHistFile->Get("tagG101"))->Fill(pTrack_tag->fPlab.Perp());
		
		if ( (pTrack_tag->fMCID == 13) || (pTrack_tag->fMCID == -13) ) {	
			
			((TH1D*)fpHistFile->Get("tagMatchedG101"))->Fill(pTrack_tag->fPlab.Perp());
			
		}	
	
		
   }

////////////////////////////////////////////////////// ---- (All) Muons Kinematics 
	cout << "------------------------------ (All) Muons Kinematics ----------------------------------------" << endl;
	int glotra(0);
	for (int m = 1; m < fpEvt->nMuons(); ++m) {

		pMuon = fpEvt->getMuon(m);
		
		if ( pMuon->fMuID == -1 ) continue;
		
		((TH1D*)fpHistFile->Get("mMuID"))->Fill(pMuon->fMuID);
		((TH1D*)fpHistFile->Get("mTimeNdof"))->Fill(pMuon->fTimeNdof);
		if ( pMuon->fMuID & 0x1<<1 ) {
			
			((TH1D*)fpHistFile->Get("mG101"))->Fill(pMuon->fGlobalPlab.Perp());
			((TH1D*)fpHistFile->Get("mG101a"))->Fill(pMuon->fInnerPlab.Perp());
			((TH1D*)fpHistFile->Get("mG101b"))->Fill(pMuon->fOuterPlab.Perp());
			((TH1D*)fpHistFile->Get("mG102"))->Fill(pMuon->fGlobalPlab.Eta());
			((TH1D*)fpHistFile->Get("mG102a"))->Fill(pMuon->fInnerPlab.Eta());
			((TH1D*)fpHistFile->Get("mG102b"))->Fill(pMuon->fOuterPlab.Eta());
			
			// Number of hits in the Muon Systems
			
			((TH1D*)fpHistFile->Get("mNhitsRPC"))->Fill(pMuon->fNhitsRPC);
			if ( (pMuon->fOuterPlab.Eta() >= 1.04) || (pMuon->fOuterPlab.Eta() <= -1.04) ) ((TH1D*)fpHistFile->Get("mNhitsCSC"))->Fill(pMuon->fNhitsCSC);
			else ((TH1D*)fpHistFile->Get("mNhitsDT"))->Fill(pMuon->fNhitsDT);
			
		}
		
		if ( pMuon->fMuID & 0x1<<2 ) {
			
			((TH1D*)fpHistFile->Get("mT101"))->Fill(pMuon->fGlobalPlab.Perp());
			((TH1D*)fpHistFile->Get("mT101a"))->Fill(pMuon->fInnerPlab.Perp());
			((TH1D*)fpHistFile->Get("mT101b"))->Fill(pMuon->fOuterPlab.Perp());
			((TH1D*)fpHistFile->Get("mT102"))->Fill(pMuon->fGlobalPlab.Eta());
			((TH1D*)fpHistFile->Get("mT102a"))->Fill(pMuon->fInnerPlab.Eta());
			((TH1D*)fpHistFile->Get("mT102b"))->Fill(pMuon->fOuterPlab.Eta());
		
		}
		
		if ( pMuon->fMuID & 0x1<<0 ) {
			
			((TH1D*)fpHistFile->Get("mS101"))->Fill(pMuon->fGlobalPlab.Perp());
			((TH1D*)fpHistFile->Get("mS101a"))->Fill(pMuon->fInnerPlab.Perp());
			((TH1D*)fpHistFile->Get("mS101b"))->Fill(pMuon->fOuterPlab.Perp());
			((TH1D*)fpHistFile->Get("mS102"))->Fill(pMuon->fGlobalPlab.Eta());
			((TH1D*)fpHistFile->Get("mS102a"))->Fill(pMuon->fInnerPlab.Eta());
			((TH1D*)fpHistFile->Get("mS102b"))->Fill(pMuon->fOuterPlab.Eta());
			
		}
		
		if ( pTrack->fMuID & 0x1<<15 ) {
			((TH1D*)fpHistFile->Get("mC101a"))->Fill(pMuon->fInnerPlab.Perp());
		}
		
		
		if ( pMuon->fMuID & 0x1<<2 && pMuon->fMuID & 0x1<<1 ) glotra++;
		
	}
   
   
   ((TH1D*)fpHistFile->Get("nGT"))->Fill(glotra);
////////////////////////////////////////////////////// ---- RecTracks stuff
    
   cout << "------------------------------ RecTracks stuff ----------------------------------------" << endl;

   for (int it = 0; it < fpEvt->nRecTracks(); ++it) {
     pTrack = fpEvt->getRecTrack(it);
     
     ((TH1D*)fpHistFile->Get("rMCID"))->Fill(pTrack->fMCID);
	  
      	if ( pTrack->fMCID == 13 ){
      		((TH1D*)fpHistFile->Get("r101a"))->Fill(pTrack->fPlab.Perp());
		((TH1D*)fpHistFile->Get("r102a"))->Fill(pTrack->fPlab.Eta());
		((TH1D*)fpHistFile->Get("r103a"))->Fill(pTrack->fPlab.Phi());
      		
		if ( (pTrack->fPlab.Perp() <= 20.) && (pTrack->fPlab.Perp() >= 3.) && (pTrack->fPlab.Eta() <= 2.4) && (pTrack->fPlab.Eta() >= -2.4)){
			((TH2D*)fpHistFile->Get("tEtaPt_neg"))->Fill(pTrack->fPlab.Eta() , pTrack->fPlab.Perp());
			}
		
		if ( pTrack->fMuID == -1 ) continue; // Make sure that track is reconstructed as a muon !!
		
		if ( (pTrack->fMuID & 0x1<<1) && (pTrack->fMuID & 0x1<<2)){
			
			if ( (pTrack->fPlab.Perp() <= 20.) && (pTrack->fPlab.Perp() >= 3.) && (pTrack->fPlab.Eta() <= 2.4) && (pTrack->fPlab.Eta() >=
			-2.4)){
				((TH2D*)fpHistFile->Get("mEtaPt_neg"))->Fill(pTrack->fPlab.Eta() , pTrack->fPlab.Perp());
			}
			
		}
      	}     
      
 	if ( pTrack->fMCID == -13 ){
	     ((TH1D*)fpHistFile->Get("r101b"))->Fill(pTrack->fPlab.Perp());
	     ((TH1D*)fpHistFile->Get("r102b"))->Fill(pTrack->fPlab.Eta());
	     ((TH1D*)fpHistFile->Get("r103b"))->Fill(pTrack->fPlab.Phi());
	     
	     if ( (pTrack->fPlab.Perp() <= 20.) && (pTrack->fPlab.Perp() >= 3.) && (pTrack->fPlab.Eta() <= 2.4) && (pTrack->fPlab.Eta() >= -2.4)){
	     		((TH2D*)fpHistFile->Get("tEtaPt_pos"))->Fill(pTrack->fPlab.Eta() , pTrack->fPlab.Perp());
	     	}
		
		if ( pTrack->fMuID == -1 ) continue; // Make sure that track is reconstructed as a muon !!
		
		if ( (pTrack->fMuID & 0x1<<1) && (pTrack->fMuID & 0x1<<2) ){
			
			if ( (pTrack->fPlab.Perp() <= 20.) && (pTrack->fPlab.Perp() >= 3.) && (pTrack->fPlab.Eta() <= 2.4) && (pTrack->fPlab.Eta() >= -2.4)){
				((TH2D*)fpHistFile->Get("mEtaPt_pos"))->Fill(pTrack->fPlab.Eta() , pTrack->fPlab.Perp());
			}
			
		}
	} 
	   
	if ( pTrack->fMuID == -1 ) continue; 
	
	if ( (pTrack->fMuID & 0x1<<1) && (pTrack->fMuID & 0x1<<2) && (pTrack->fPlab.Perp() >= 3.)){
	
		((TH1D*)fpHistFile->Get("r101c"))->Fill(pTrack->fPlab.Perp());
		((TH1D*)fpHistFile->Get("r102c"))->Fill(pTrack->fPlab.Eta());
		((TH1D*)fpHistFile->Get("r103c"))->Fill(pTrack->fPlab.Phi());
		
	}  
	   
//     cout << "R: "; pTrack->dump(); 		
   }
  
  
////////////////////////////////////////// ------ Resonance Muons
  
  cout << "------------------------------ Resonance Muons ----------------------------------------" << endl;  
  TGenCand *pMom1, *pMom2;
  TGenCand *pMom;  
  double index_pos1 = -1; double index_pos2 = -1; 
  double index_neg1 = -1; double index_neg2 = -1;
  
  bool a = false; bool b = false; bool c = false; bool d = false;
  
  for (int it = 0; it < fpEvt->nCands(); ++it) {
  	pCand = fpEvt->getCand(it);
  	pTrack1 = fpEvt->getSigTrack(2*it+1);
	pTrack2 = fpEvt->getSigTrack(2*it);
	pTrack_tag = fpEvt->getRecTrack(pTrack2->fIndex);
	
  	if ( (pTrack1->fGenIndex >= 0) && (pTrack2->fGenIndex >= 0) ){
			
		pGEn1 = fpEvt->getGenCand(pTrack1->fGenIndex);
		pGEn2 = fpEvt->getGenCand(pTrack2->fGenIndex);	
				
		cout << "pGEn1->fID = "  << pGEn1->fID << endl; 
		cout << "pGEn2->fID = "  << pGEn2->fID << endl;
		
		if ( abs(pGEn1->fID) == 13 && abs(pGEn2->fID) == 13 ){
		
			pMom1 = fpEvt->getGenCand(pGEn1->fMom1);
			pMom2 = fpEvt->getGenCand(pGEn2->fMom1);
				
			cout << "pMom1->fID = " << pMom1->fID << endl;
			cout << "pMom2->fID = " << pMom2->fID << endl;
				
			if ( pMom1->fID == fRes && pMom2->fID == fRes ){
			
  				if ( TMath::Abs(pTrack1->fPlab.Eta()-pTrack2->fPlab.Eta()) < 0.3 ) continue;
				
  				if ( (pTrack1->fPlab.Perp() >= 3.) && (pTrack1->fPlab.Eta() <= 2.4) && (pTrack1->fPlab.Eta() >= -2.4) && (pTrack1->fMuID != -1) && (pTrack1->fMuID & 0x1<<1)  && (pTrack1->fMuID & 0x1<<2) ) {
												
						if ( pTrack1->fQ > 0 && index_pos1 != pTrack1->fGenIndex  && index_pos2 != pTrack1->fGenIndex ){
							posmuoneta = pTrack1->fPlab.Eta();
							posmuonpt = pTrack1->fPlab.Perp();
							index_pos1 = pTrack1->fGenIndex;
							cout << "POS pTrack1->fGenIndex = " << pTrack1->fGenIndex << endl;
							a = true;
							}
						 if ( pTrack1->fQ < 0 && index_neg1 != pTrack1->fGenIndex  && index_neg2 != pTrack1->fGenIndex ){
							negmuoneta = pTrack1->fPlab.Eta();
							negmuonpt = pTrack1->fPlab.Perp();
							index_neg1 = pTrack1->fGenIndex;
							cout << "NEG pTrack1->fGenIndex = " << pTrack1->fGenIndex << endl;
							b = true;
							}
					}		
						
  				if ( (pTrack2->fPlab.Perp() >= 3.) && (pTrack2->fPlab.Eta() <= 2.4) && (pTrack2->fPlab.Eta() >= -2.4) && (pTrack_tag->fMuID != -1) && (pTrack_tag->fMuID & 0x1<<1)  && (pTrack_tag->fMuID & 0x1<<2) ) {
						
						if ( pTrack2->fQ > 0  && index_pos1 != pTrack2->fGenIndex  && index_pos2 != pTrack2->fGenIndex ){
							posmuoneta = pTrack2->fPlab.Eta();
							posmuonpt = pTrack2->fPlab.Perp();
							index_pos2 = pTrack2->fGenIndex;
							cout << "POS pTrack2->fGenIndex = " << pTrack2->fGenIndex << endl;
							c = true;
							}
						 if ( pTrack2->fQ < 0 && index_neg1 != pTrack2->fGenIndex  && index_neg2 != pTrack2->fGenIndex){
							negmuoneta = pTrack2->fPlab.Eta();
							negmuonpt = pTrack2->fPlab.Perp();
							index_neg2 = pTrack2->fGenIndex;
							cout << "NEG pTrack2->fGenIndex = " << pTrack2->fGenIndex << endl;
							d = true;
							}	
					}
				
				
				if ( !(a || c) ) { posmuoneta=-10 ; posmuonpt = -1;}
				if ( !(b || d) ) { negmuoneta=-10 ; negmuonpt = -1;} 									
				fTree1->Fill();		
				}
		}
	}					
  }
      
////////////////////////////////////////// ------ T&P stuff
  
  cout << "------------------------------ T&P stuff ----------------------------------------" << endl;
    

  
  bool cut = false;
  TString Label = "hltL1sL1SingleMu3:HLT::";
  if ( fpEvt->nCands() > 0){
  	for (int it = 0; it < fpEvt->nCands(); ++it) {
		cout << "-----------Candidate Stuff-------------------" << endl;
		pCand = fpEvt->getCand(it);
		
		((TH1D*)fpHistFile->Get("c100"))->Fill(pCand->fMass);
		
		if ( pCand->fType == 67 || pCand->fType == -67) continue;
		
		pTrack1 = fpEvt->getSigTrack(2*it+1);
		pTrack2 = fpEvt->getSigTrack(2*it);
		cout << "pCand->fMass = " << pCand->fMass <<", pCand->fType = " << pCand->fType << endl;
		
		pTrack_tag = fpEvt->getRecTrack(pTrack2->fIndex);
		
		if ( pTrack_tag->fMuID == -1 ) continue; // Make sure that track is reconstructed as a muon !!
		if ( pTrack_tag->fPlab.Perp() < 3. ) continue;
		if ( pTrack_tag->fPlab.Eta() > 2.4 ) continue;
		if ( pTrack_tag->fPlab.Eta() < -2.4 ) continue;
		if ( !((pTrack_tag->fMuID & 0x1<<1) && (pTrack_tag->fMuID & 0x1<<2)) ) continue;
		
		if  ( !isMatchedToTrig(pTrack_tag, Label) ) continue;
				
		if ( (pTrack1->fGenIndex >= 0) && (pTrack2->fGenIndex >= 0) ){
			
			pGEn1 = fpEvt->getGenCand(pTrack1->fGenIndex);
			pGEn2 = fpEvt->getGenCand(pTrack2->fGenIndex);	
				
			cout << "pGEn1->fID = "  << pGEn1->fID << endl; 
			cout << "pGEn2->fID = "  << pGEn2->fID << endl;
		
			if ( abs(pGEn1->fID) == 13 && abs(pGEn2->fID) == 13 ){
		
				pMom1 = fpEvt->getGenCand(pGEn1->fMom1);
				pMom2 = fpEvt->getGenCand(pGEn2->fMom1);
				
				cout << "pMom1->fID = " << pMom1->fID << endl;
				cout << "pMom2->fID = " << pMom2->fID << endl;
				
				if ( pMom1->fID == fRes && pMom2->fID == fRes ){
				
					cout << "------Reduced Tree-------" << endl;
				
					pq = pTrack1->fQ;
					cout << "pq = "<< pq <<endl;
					pf = pTrack1->fPlab.Phi();
					pe = pTrack1->fPlab.Eta();
					pp = pTrack1->fPlab.Perp();
					pm = pTrack1->fMuID;
				
					double p_theta = pTrack1->fPlab.Theta();		
					double cot_p_theta = TMath::Tan(1/p_theta);			
					double p_sqrt = TMath::Sqrt( 1 + cot_p_theta*cot_p_theta );
						
					pP = pTrack1->fPlab.Perp()*p_sqrt;
					
					cout << " pp = " << pp << " pP = " << pP << " p_theta = " << p_theta << endl;
					
					tq = pTrack2->fQ;
					tf = pTrack2->fPlab.Phi();
					te = pTrack2->fPlab.Eta();
					tp = pTrack2->fPlab.Perp();
					cout << "tp = "<< tp <<endl;
				
					double t_theta = pTrack2->fPlab.Theta();
					double cot_t_theta = TMath::Tan(1/t_theta);			
					double t_sqrt = TMath::Sqrt( 1 + cot_t_theta*cot_t_theta );
				
					tP = pTrack2->fPlab.Perp()*t_sqrt;
				
					cout << " tp = " << tp << " tP = " << tP << " t_theta = " << t_theta << endl;
				
					double deta = TMath::Abs(te - pe);
					double dphi = TMath::Abs(tf - pf);
				
					dR = TMath::Sqrt((deta*deta)+(dphi*dphi));
					
					uf = pCand->fPlab.Phi();
					cout << "uf = "<< uf <<endl;
					ue = pCand->fPlab.Eta();
					up = pCand->fPlab.Perp();
					um = pCand->fMass;
					fTree->Fill();
				
				
					if (fRes ==  553 || fRes ==  100553 || fRes ==  200553){
				
					//	cut = (pTrack1->fPlab.Eta()*pTrack2->fPlab.Eta() > 0.);
					//	cut = ( TMath::Abs(pTrack2->fPlab.Eta() - pTrack1->fPlab.Eta()) < 0.5 );
						cut = true;
					
					}
							
					if (fRes ==  443 ){
			
					//	cut = ( TMath::Abs(pTrack1->fPlab.Eta()-pTrack2->fPlab.Eta()) > 0.2 );
					//	cut = ( pTrack2->fPlab.Perp() > 6. );
					/*	if ( TMath::Abs(pTrack2->fPlab.Eta()) < 1.){ cut = ( pTrack2->fPlab.Perp() > 4. );}  // cut on tag muon's pt
						if ( TMath::Abs(pTrack2->fPlab.Eta()) > 1.){ cut = true; }	*/
						cut = true;
				
					}
					
					
					if ( cut == false ) continue;

					// 443 J/Psi , 553 Y(1S), 100553 Y(2S) and Y(3S) !??!?? 
				
					cout << " Signals are really comig from the resonance " <<endl;
				
					////////////////////////////////////////  Filling mtMatched histograms
				
					cout << "------------Filling mtMatched histograms-----------" <<endl;			
					
					for ( int ieta = 0; ieta < Neta; ++ieta ){
						for ( int ipt = 0; ipt < Npt; ++ipt ){
							if( ( pTrack1->fPlab.Eta() <= Etabin[ieta+1] ) && ( pTrack1->fPlab.Eta() > Etabin[ieta] ) ){
								if ( ( pTrack1->fPlab.Perp() >= PTbin[ipt] ) && ( pTrack1->fPlab.Perp() < PTbin[ipt+1] ) ){
									if ( pTrack1->fQ < 0 ){
										((TH1D*)fpHistFile->Get(Form("mtMatched,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", Etabin[ieta],
										Etabin[ieta+1], PTbin[ipt], PTbin[ipt+1],charge[0])))->Fill(pCand->fMass);
									}
									if ( pTrack1->fQ > 0 ){
										((TH1D*)fpHistFile->Get(Form("mtMatched,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", Etabin[ieta],
										Etabin[ieta+1], PTbin[ipt], PTbin[ipt+1], charge[1])))->Fill(pCand->fMass);
									}
								}
							}
						}	
					}
					
					if ( (pTrack1->fMuID!= -1) && (pTrack1->fMuID & 0x1<<1)  && (pTrack1->fMuID & 0x1<<2) ){
										
					////////////////////////////////////////  Filling mmMatched histograms 
				
					cout << "------------Filling mmMatched histograms-----------" <<endl;
						
						for ( int ieta = 0; ieta < Neta; ++ieta ){
							for ( int ipt = 0; ipt < Npt; ++ipt ){
								if( ( pTrack1->fPlab.Eta() <= Etabin[ieta+1] ) && ( pTrack1->fPlab.Eta() > Etabin[ieta] ) ){
									if ( ( pTrack1->fPlab.Perp() >= PTbin[ipt] ) && ( pTrack1->fPlab.Perp() < PTbin[ipt+1] ) ){
										if ( pTrack1->fQ < 0 ){
											((TH1D*)fpHistFile->Get(Form("mmMatched,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", Etabin[ieta], Etabin[ieta+1], PTbin[ipt], PTbin[ipt+1],charge[0])))->Fill(pCand->fMass);
										}
										if ( pTrack1->fQ > 0 ){
											((TH1D*)fpHistFile->Get(Form("mmMatched,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", Etabin[ieta], Etabin[ieta+1], PTbin[ipt], PTbin[ipt+1],charge[1])))->Fill(pCand->fMass);
										}
									}
								}
							}
						}
			
					}
				
				}
			}
			
		}
		
		cut = false;
		
		if (fRes ==  553 || fRes ==  100553 || fRes ==  200553){
			
		//	cut = (pTrack1->fPlab.Eta()*pTrack2->fPlab.Eta() > 0.);
		//	cut = ( TMath::Abs(pTrack2->fPlab.Eta() - pTrack1->fPlab.Eta()) < 0.5 );
			cut = true;
					
		}
					
							
		if (fRes ==  443 ){
					
	 	//	cut = ( TMath::Abs(pTrack1->fPlab.Eta()-pTrack2->fPlab.Eta()) > 0.2 );
		//	cut = ( pTrack2->fPlab.Perp() > 6. );	
		/*	if ( TMath::Abs(pTrack2->fPlab.Eta()) < 1.){ cut = ( pTrack2->fPlab.Perp() > 4. );}  // cut on tag muon's pt
			if ( TMath::Abs(pTrack2->fPlab.Eta()) > 1.){ cut = true; }	*/
			cut = true;			
		}
				
		if ( cut == false ) continue;
		
		/////////////////////////////////  Filling mt histograms
		
		cout << "------------Filling mt histograms-----------" <<endl;
		
		for ( int ieta = 0; ieta < Neta; ++ieta ){
			for ( int ipt = 0; ipt < Npt; ++ipt ){
				if( ( pTrack1->fPlab.Eta() <= Etabin[ieta+1] ) && ( pTrack1->fPlab.Eta() > Etabin[ieta] ) ){
					if ( ( pTrack1->fPlab.Perp() >= PTbin[ipt] ) && ( pTrack1->fPlab.Perp() < PTbin[ipt+1] ) ){
						if ( pTrack1->fQ < 0 ){
							((TH1D*)fpHistFile->Get(Form("mt,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", Etabin[ieta], Etabin[ieta+1], PTbin[ipt],
							PTbin[ipt+1],charge[0])))->Fill(pCand->fMass);
						}
						if ( pTrack1->fQ > 0 ){
							((TH1D*)fpHistFile->Get(Form("mt,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", Etabin[ieta], Etabin[ieta+1], PTbin[ipt],							PTbin[ipt+1], charge[1])))->Fill(pCand->fMass);
						}
					}
				}
			}
		}
				
		if ( (pTrack1->fMuID!= -1) && (pTrack1->fMuID & 0x1<<1) && (pTrack1->fMuID & 0x1<<2) ){
		
		////////////////////////////////////////////  Filling the mm histograms
			
		cout << "------------Filling mm histograms-----------" <<endl;	
			
			for ( int ieta = 0; ieta < Neta; ++ieta ){
				for ( int ipt = 0; ipt < Npt; ++ipt ){
					if( ( pTrack1->fPlab.Eta() <= Etabin[ieta+1] ) && ( pTrack1->fPlab.Eta() > Etabin[ieta] ) ){
						if ( ( pTrack1->fPlab.Perp() >= PTbin[ipt] ) && ( pTrack1->fPlab.Perp() < PTbin[ipt+1] ) ){
							if ( pTrack1->fQ < 0 ){
								((TH1D*)fpHistFile->Get(Form("mm,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", Etabin[ieta], Etabin[ieta+1], PTbin[ipt],
								PTbin[ipt+1],charge[0])))->Fill(pCand->fMass);
							}
							if ( pTrack1->fQ > 0 ){
								((TH1D*)fpHistFile->Get(Form("mm,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", Etabin[ieta], Etabin[ieta+1], PTbin[ipt],
								PTbin[ipt+1],charge[1])))->Fill(pCand->fMass);
							}
						}
					}
				}
			}
		
		}
		
		
		if ( !((pTrack1->fMuID!= -1) && (pTrack1->fMuID & 0x1<<1) && (pTrack1->fMuID & 0x1<<2)) ){
		
		///////////////////////////////////////////  Filling mmbar histograms 
		
		cout << "------------Filling mmbar histograms-----------" <<endl;
	
			for ( int ieta = 0; ieta < Neta; ++ieta ){
				for ( int ipt = 0; ipt < Npt; ++ipt ){
					if( ( pTrack1->fPlab.Eta() <= Etabin[ieta+1] ) && ( pTrack1->fPlab.Eta() > Etabin[ieta] ) ){
						if ( ( pTrack1->fPlab.Perp() >= PTbin[ipt] ) && ( pTrack1->fPlab.Perp() < PTbin[ipt+1] ) ){
							if ( pTrack1->fQ < 0 ){
								((TH1D*)fpHistFile->Get(Form("mmbar,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", Etabin[ieta], Etabin[ieta+1],
								PTbin[ipt], PTbin[ipt+1],charge[0])))->Fill(pCand->fMass);
							}
							if ( pTrack1->fQ > 0 ){
								((TH1D*)fpHistFile->Get(Form("mmbar,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", Etabin[ieta], Etabin[ieta+1],
								PTbin[ipt], PTbin[ipt+1],charge[1])))->Fill(pCand->fMass);
							}
						}
					}
				}
			}
		
		}				
	
	
  				
 	 }
  }

//////////////////////////////////////////////// Gen stuff

  // ==============================================
  // GENERATOR LEVEL
  // ==============================================
  
  cout << "--------------Gen Stuff-----------" << endl;
  
  for( int ig = 0; ig < fpEvt->nGenCands(); ig++ ){
    pGen1 = fpEvt->getGenCand(ig);
    ((TH1D*)fpHistFile->Get("g100"))->Fill(pGen1->fID);
    if (pGen1->fID == fRes && pGen1->fStatus == 2){
    	((TH1D*)fpHistFile->Get("g100a"))->Fill(pGen1->fID);
    	((TH1D*)fpHistFile->Get("g100aS"))->Fill(pGen1->fStatus);
    	((TH1D*)fpHistFile->Get("g101a"))->Fill(pGen1->fP.Pt());
    	((TH1D*)fpHistFile->Get("g102a"))->Fill(pGen1->fP.Eta());
    	((TH1D*)fpHistFile->Get("g103a"))->Fill(pGen1->fP.Phi());              
    }
    
    if ( pGen1->fID == 13 || pGen1->fID == -13 ){	
    	((TH1D*)fpHistFile->Get("g100b"))->Fill(pGen1->fID);
    	((TH1D*)fpHistFile->Get("g100bS"))->Fill(pGen1->fStatus);   
    	((TH1D*)fpHistFile->Get("g101b"))->Fill(pGen1->fP.Pt());
    	((TH1D*)fpHistFile->Get("g102b"))->Fill(pGen1->fP.Eta());
    	((TH1D*)fpHistFile->Get("g103b"))->Fill(pGen1->fP.Phi());
    }
           
  }    
//	fpEvt->dumpGenBlock();
	   
  fpHistFile->cd();
  fillHist();
  // fTree->Fill();
  
}

bool myReader01::isMatchedToTrig(TAnaTrack *pTag, TString Label){
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
void myReader01::fillHist() {


}

// ----------------------------------------------------------------------
void myReader01::bookHist() {

  // ---------------------
  // Legend:
  // m = matched; c = candidates; r = reconstructed; g = generated
  // 100 = mass; 200 = momentum; 300 = geometrics; 400 = numerics
  // ---------------------

//std::ostringstream ss;

int x(2), w(10);  

int Npt = 6;
double PTbin[] = {0., 2., 3., 4., 5., 6., 20.};			//   used 	
								//    for 
int Neta = 5;							//   mmbar
double Etabin[] = {-2.4, -1.2, -0.4, 0.4, 1.2, 2.4};

//int Npt = 7;
//double PTbin[] = {0., 2., 3., 4., 5., 6., 10., 20.};		 
  								
//int Neta = 16;							
//double Etabin[] = {-2.4, -2.1, -1.8, -1.5, -1.2, -0.9, -0.6, -0.3, 0., 0.3, 0.6, 0.9, 1.2, 1.5, 1.8, 2.1, 2.4};      


int npt = 12;
double pTbin[] = {0., 2., 2.5, 3., 3.5, 4., 4.5, 5., 5.5, 6., 7., 9., 20.};	//	used for t/m_EtaPT_neg/pos
										// 	to compare with J/Psi 
int neta = 14;									// 	with J/Psi binning
double etabin[] = {-2.4, -1.8, -1.5, -1.2, -1.0, -0.8, -0.6, 0., 0.6, 0.8, 1.0, 1.2, 1.5, 1.8, 2.4};

int ncharge = 1;
int charge[] = {-1, 1};

int binnum(50);

double lowmass(8.7);
double highmass(11.2);


if ( fRes == 443 ){
	lowmass = 2.8;
	highmass = 3.4;
	binnum = 30;
 	cout << "myReader: Running over J/psi: " << lowmass << " " << highmass << endl;
} else {
 	cout << "myReader: Running over Ups: " << lowmass << " " << highmass << endl;

}

TH1 *h;
TH2 *k;
TH1 *b[x][w];

//	changed to mmbar binning
k = new TH2D("mt,pt-eta", "mt,pt-eta", Npt, PTbin, Neta, Etabin);


k = new TH2D("mmmbar,pt-eta", "mmbar,pt-eta", Npt, PTbin, Neta, Etabin);

cout << "--> myReader> bookHist> " << endl;

// T&P mass histograms
//	changed to mmbar binning
for ( int ieta = 0; ieta < Neta; ++ieta ){
	for ( int ipt = 0; ipt < Npt; ++ipt ){
		for ( int icharge = 0; icharge <= ncharge; ++icharge ){
			h = new TH1D(Form("mt,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", Etabin[ieta], Etabin[ieta+1], PTbin[ipt], PTbin[ipt+1], charge[icharge]),
				     Form("mt,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", Etabin[ieta], Etabin[ieta+1], PTbin[ipt], PTbin[ipt+1], charge[icharge]),
				     binnum, lowmass, highmass);
		}
	}	
}     

for ( int ieta = 0; ieta < Neta; ++ieta ){
	for ( int ipt = 0; ipt < Npt; ++ipt ){
		for ( int icharge = 0; icharge <= ncharge; ++icharge ){
			h = new TH1D(Form("mm,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", Etabin[ieta], Etabin[ieta+1], PTbin[ipt], PTbin[ipt+1], charge[icharge]),
				     Form("mm,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", Etabin[ieta], Etabin[ieta+1], PTbin[ipt], PTbin[ipt+1], charge[icharge]),
				     binnum, lowmass, highmass);
		}     
	}
}

for ( int ieta = 0; ieta < Neta; ++ieta ){
	for ( int ipt = 0; ipt < Npt; ++ipt ){
		for ( int icharge = 0; icharge <= ncharge; ++icharge ){
			h = new TH1D(Form("mmbar,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", Etabin[ieta], Etabin[ieta+1], PTbin[ipt], PTbin[ipt+1], charge[icharge]),
				     Form("mmbar,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", Etabin[ieta], Etabin[ieta+1], PTbin[ipt], PTbin[ipt+1], charge[icharge]),
				     binnum, lowmass, highmass);
		}     
	}
}     

// TruthMatched histograms for pull distributions
for ( int ieta = 0; ieta < Neta; ++ieta ){
	for ( int ipt = 0; ipt < Npt; ++ipt ){
		for ( int icharge = 0; icharge <= ncharge; ++icharge ){
			h = new TH1D(Form("mtMatched,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", Etabin[ieta], Etabin[ieta+1], PTbin[ipt], PTbin[ipt+1], charge[icharge]),
				     Form("mtMatched,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", Etabin[ieta], Etabin[ieta+1], PTbin[ipt], PTbin[ipt+1], charge[icharge]),
				     binnum, lowmass, highmass);
		}
	}	
}

for ( int ieta = 0; ieta < Neta; ++ieta ){
	for ( int ipt = 0; ipt < Npt; ++ipt ){
		for ( int icharge = 0; icharge <= ncharge; ++icharge ){
			h = new TH1D(Form("mmMatched,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", Etabin[ieta], Etabin[ieta+1], PTbin[ipt], PTbin[ipt+1], charge[icharge]),
				     Form("mmMatched,eta%.1f_%.1f,pt%.1f_%.1f,Q%.1d", Etabin[ieta], Etabin[ieta+1], PTbin[ipt], PTbin[ipt+1], charge[icharge]),
				     binnum, lowmass, highmass);
		}     
	}
}     


h = new TH1D("n400", "nrectrk", 120, 0, 120.);
h = new TH1D("n401", "nsigtrk", 20, 0, 20.);
h = new TH1D("n402", "ncand", 20, 0, 20.);
h = new TH1D("n403", "ngencand", 1000, 0, 1000.);


////////////////////////////////////////////////  ---- Signal stuff 

h = new TH1D("sMuID",  "MuID", 900, -10.,890.);
h = new TH1D("stagMuID",  "MuID", 900, -10.,890.);

h = new TH1D("sG101",  "Global Muons -- Pt", 60, 0., 30.);
h = new TH1D("sG101a", "Global Muons in Barrel -- Pt", 60, 0., 30.);
h = new TH1D("sG101b", "Global Muons in EndCaps -- Pt", 60, 0., 30.);
h = new TH1D("sG102a", "Low Pt Global Muons -- Eta", 60, -3., 3.);
h = new TH1D("sG102",  "Global Muons -- Eta", 60, -3., 3.);
h = new TH1D("sT101",  "Tracker Muons -- Pt", 60, 0., 30.);
h = new TH1D("sT101a", "Tracker Muons in Barrel -- Pt", 60, 0., 30.);
h = new TH1D("sT101b", "Tracker Muons in EndCaps -- Pt", 60, 0., 30.);
h = new TH1D("sT102a", "Low Pt Tracker Muons -- Eta", 60, -3., 3.);
h = new TH1D("sT102",  "Tracker Muons -- Eta", 60, -3., 3.);
h = new TH1D("sS101",  "Standalone Muons -- Pt", 60, 0., 30.);
h = new TH1D("sS101a", "Standalone Muons in Barrel -- Pt", 60, 0., 30.);
h = new TH1D("sS101b", "Standalone muons in EndCaps -- Pt", 60, 0., 30.);
h = new TH1D("sS102a", "Low Pt Standalone Muons -- Eta", 60, -3., 3.);
h = new TH1D("sS102",  "Calo Muons -- Eta", 60, -3., 3.);

h = new TH1D("nG", " # of Global Muons", 10, 0.,10.);
h = new TH1D("nT", " # of Traker muons", 10, 0.,10.);
h = new TH1D("nS", " # of Standalone muons", 10, 0.,10.);
h = new TH1D("nGT", " # of Global&Tracker Muons", 10, 0.,10.);

h = new TH1D("tagG101",  "Tag, Global Muons -- Pt", 60, 0., 30.);
h = new TH1D("tagMatchedG101",  "Tag Matched, Global Muons -- Pt", 60, 0., 30.);

h = new TH1D("tag_pt",  "tag_pt", 20, 0., 20.);
h = new TH1D("trig_dR",  "trig_dR", 50, 0., 1.);
h = new TH1D("trig_dEta",  "trig_dEta", 50, 0., 1.);
h = new TH1D("trig_dPhi",  "trig_dPhi", 50, 0., 1.);
h = new TH1D("trig_dEta_aftercut",  "trig_dEta", 50, 0., 1.);

///////////////////////////////////////////////   ----  Muon Stuff

h = new TH1D("mMuID",  "MuID", 900, -10.,890.);
h = new TH1D("mTimeNdof",  "TimeNdof", 80, -1.,39.);
h = new TH1D("mNhitsDT",  "NhitsDT", 50, -1.,49.);
h = new TH1D("mNhitsCSC",  "NhitsCSC", 50, -1.,49.);
h = new TH1D("mNhitsRPC",  "NhitsRPC", 10, -1.,9.);

h = new TH1D("mG101",  "Global (Global) Muons -- Pt", 60, 0., 30.);
h = new TH1D("mG101a",  "Global (Inner) Muons -- Pt", 60, 0., 30.);
h = new TH1D("mG101b",  "Global (Outer) Muons -- Pt", 60, 0., 30.);
h = new TH1D("mT101",  "Tracker (Global) Muons -- Pt", 60, 0., 30.);
h = new TH1D("mT101a",  "Tracker (Inner) Muons -- Pt", 60, 0., 30.);
h = new TH1D("mT101b",  "Tracker (Outer) Muons -- Pt", 60, 0., 30.);
h = new TH1D("mS101",  "StandAlone (Global) Muons -- Pt", 60, 0., 30.);
h = new TH1D("mS101a",  "StandAlone (Inner) Muons -- Pt", 60, 0., 30.);
h = new TH1D("mS101b",  "StandAlone (Outer) Muons -- Pt", 60, 0., 30.);
h = new TH1D("mG102",  "Global (Global) Muons -- Eta", 60, -3., 3.);
h = new TH1D("mG102a",  "Global (Inner) Muons -- Eta", 60, -3., 3.);
h = new TH1D("mG102b",  "Global (Outer) Muons -- Eta", 60, -3., 3.);
h = new TH1D("mT102",  "Tracker (Global) Muons -- Eta", 60, -3., 3.);
h = new TH1D("mT102a",  "Tracker (Inner) Muons -- Eta", 60, -3., 3.);
h = new TH1D("mT102b",  "Tracker (Outer) Muons -- Eta", 60, -3., 3.);
h = new TH1D("mS102",  "StandAlone (Global) Muons -- Eta", 60, -3., 3.);
h = new TH1D("mS102a",  "StandAlone (Inner) Muons -- Eta", 60, -3., 3.);
h = new TH1D("mS102b",  "StandAlone (Outer) Muons -- Eta", 60, -3., 3.);
h = new TH1D("mC101a",  "Calo (Inner) Muons -- Pt", 60, 0., 30.);


///////////////////////////////////////////////  ---- Rec Stuff

h = new TH1D("rMCID", "MCID", 3000, -1500, 1500);
h = new TH1D("r101a", "(Matched) Muon -- Pt, Neg", Npt, PTbin);
h = new TH1D("r101b", "(Matched) Muon -- Pt, Pos", Npt, PTbin);
h = new TH1D("r102a", "(Matched) Muon -- Eta, Neg", Neta, Etabin);
h = new TH1D("r102b", "(Matched) Muon -- Eta, Pos", Neta, Etabin);
h = new TH1D("r103a", "(Matched) Muon -- Phi, Neg", 40, -4., 4.);
h = new TH1D("r103b", "(Matched) Muon -- Phi, Pos", 40, -4., 4.);
h = new TH1D("r101c", "(Reco -- Global) Muon -- Pt, Neg", 34, 3., 20.);
h = new TH1D("r102c", "(Reco -- Global) Muon -- Eta", 48, -2.4, 2.4);
h = new TH1D("r103c", "(Reco -- Global) Muon -- Phi, Neg", 40, -4., 4.);

k = new TH2D("tEtaPt_neg", "tEtaPt_neg", Neta, Etabin, Npt, PTbin);
k = new TH2D("tEtaPt_pos", "tEtaPt_pos", Neta, Etabin, Npt, PTbin);	//      used for MC truth Efficiency	
k = new TH2D("mEtaPt_neg", "mEtaPt_neg", Neta, Etabin, Npt, PTbin);	//	changed to mmbar binning
k = new TH2D("mEtaPt_pos", "mEtaPt_pos", Neta, Etabin, Npt, PTbin);


///////////////////////////////////////////////  ---- Candidate Stuff

k = new TH2D("c150", "Resonance Pt vs Resonance Eta", 40, 0., 40., 40, -6., 6.);

h = new TH1D("c100", "Resonance Mass", 120, 1.,13.);
h = new TH1D("c101", "Resonance PT", 40, 0., 40.);
h = new TH1D("c102", "Resonance Eta", 40, -6., 6.);
h = new TH1D("c103", "Resonance Phi", 40, -4., 4.);

///////////////////////////////////////////////  ---- Truth Candidate Stuff

h = new TH1D("ct100", "Truth Candidate Mass", 120, 1.,13.);
h = new TH1D("ct101", "Truth Candidate PT", 40, 0., 40.);
h = new TH1D("ct102", "Truth Candidate Eta", 40, -6., 6.);
h = new TH1D("ct103", "Truth Candidate Phi", 40, -4., 4.);

h = new TH1D("ct100a", "Truth Candidate Mass", 120, 1.,13.);
h = new TH1D("ct101a", "Truth Candidate PT", 40, 0., 40.);
h = new TH1D("ct102a", "Truth Candidate Eta", 40, -6., 6.);
h = new TH1D("ct103a", "Truth Candidate Phi", 40, -4., 4.);

///////////////////////////////////////////////  ---- Gen Stuff

h = new TH1D("g100", "Gen Particle ID", 1200, -600, 600);
h = new TH1D("g100a", "Resonance ID", 200, 400, 600);
h = new TH1D("g100b", "MuonID", 40, -20, 20);
h = new TH1D("g100aS", "Resonance Status", 40, -20, 20);
h = new TH1D("g100bS", "Muon Status", 40, -20, 20);
\
h = new TH1D("g101a", "Resonance -- Pt", 40, 0., 40.);
h = new TH1D("g101b", "Muon -- Pt", 40, 0., 40.);
h = new TH1D("g102a", "Resonance --.Eta", 40, -6., 6.);
h = new TH1D("g102b", "Muon -- Eta", 40, -3., 3.);
h = new TH1D("g103a", "Resonance -- Phi", 40, -4., 4.);
h = new TH1D("g103b", "Muon -- Phi", 40, -4., 4.);


///////////////////////////////////////////// ---- Reduced Tree
fTree = new TTree("events", "events");
fTree->Branch("run",    &fRun ,"run/I");
fTree->Branch("pq",    &pq ,"pq/I");
fTree->Branch("pf",    &pf ,"pf/F");
fTree->Branch("pe",    &pe ,"pe/F");
fTree->Branch("pm",    &pm ,"pm/F");
fTree->Branch("pp",    &pp ,"pp/F");
fTree->Branch("pP",    &pp ,"pP/F");
fTree->Branch("tq",    &tq ,"tq/I");
fTree->Branch("tf",    &tf ,"tf/F");
fTree->Branch("te",    &te ,"te/F");
fTree->Branch("tp",    &tp ,"tp/F");
fTree->Branch("tP",    &tP ,"tP/F");
fTree->Branch("dR",    &dR ,"dR/F");
fTree->Branch("ue",    &ue ,"ue/F");
fTree->Branch("up",    &up ,"up/F");
fTree->Branch("um",    &um ,"um/F");

fTree1 = new TTree("muons", "muons");
fTree1->Branch("posmuoneta", &posmuoneta ,"posmuoneta/F");
fTree1->Branch("posmuonpt",  &posmuonpt ,"posmuonpt/F");
fTree1->Branch("negmuoneta", &negmuoneta ,"negmuoneta/F");
fTree1->Branch("negmuonpt",  &negmuonpt ,"negmuonpt/F");

}


// --------------------------------------------------------------------------------------------------
void myReader01::readCuts(TString filename, int dump) {
  char  buffer[200];
  fCutFile = filename;
  if (dump) cout << "Reading " << fCutFile.Data() << " for myReader cut settings" << endl;
  sprintf(buffer, "%s", fCutFile.Data());
  ifstream is(buffer);
  char CutName[100];
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
    sscanf(buffer, "%s %f", CutName, &CutValue);

    if (!strcmp(CutName, "TYPE")) {
      TYPE = int(CutValue); ok = 1;
      if (dump) cout << "TYPE:           " << TYPE << endl;
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

