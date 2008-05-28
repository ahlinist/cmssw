#define testgjet_new_cxx
#include "testgjet_new.h"
#include <TF1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <MyAnalysis/GammaJetAnalysis/test/TMVAPhotons_MLP.class.C>
#include <MyAnalysis/GammaJetAnalysis/test/TMVAPhotons_1_MLP_new.class.C>
#include <MyAnalysis/GammaJetAnalysis/test/TMVAPhotons_2_MLP_new.class.C>
#include <MyAnalysis/GammaJetAnalysis/test/TMVAPhotons_3_MLP_new.class.C>
#include <MyAnalysis/GammaJetAnalysis/test/TMVAPhotons_4_MLP_new.class.C>
void testgjet_new::Loop(int NEVT)
{
//   In a ROOT session, you can do:
//      Root > .L testgjet_new.C
//      Root > testgjet_new t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch

  double pi = 3.14159;

  if (fChain == 0) return;
  
  Int_t nentries = Int_t(fChain->GetEntries());
  
  std::cout << "Entries are: " << nentries << std::endl;

  if(NEVT<nentries) {

    nentries = NEVT;
    std::cout << "but running on " << nentries << std::endl;

  }

  //  gROOT->LoadMacro("/u1/delre/TMVA/macros/weights/TMVAPhotons_MLP.class.C++");

  std::vector<std::string> inputVars;
  inputVars.push_back("ntrkiso");
  inputVars.push_back("ptiso");
//   inputVars.push_back("ptisoatecal");
  inputVars.push_back("hcalovecal");
  inputVars.push_back("sMajMaj");
  inputVars.push_back("sMinMin");
  inputVars.push_back("ecaliso");

  IClassifierReader* classReader;
  classReader = new ReadMLP( inputVars );
  IClassifierReader* classReader1;
  classReader1 = new ReadMLP1( inputVars );
  IClassifierReader* classReader2;
  classReader2 = new ReadMLP2( inputVars );
  IClassifierReader* classReader3;
  classReader3 = new ReadMLP3( inputVars );
  IClassifierReader* classReader4;
  classReader4 = new ReadMLP4( inputVars );

  BookHistos();

  std::vector<double>* inputVec = new std::vector<double>( 6 );

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    
    if (jentry%10000 == 0) std::cout << "Event " << jentry << std::endl;      

    double maxptphot = 0;
    int imaxptphot = -1;
    bool passcuts = 0;
    bool passloosecuts = 0;
    
    int ileadmcphot = -1;
    int ileadmcquark = -1;
    
    isphoton = issignal = -999;
    weight = nniso = ptph = ptj = etaph = etaj = phiph = phij = pt2jet = ptphottrue = ptjettrue = ptquarktrue = phiphottrue = phijettrue = phiquarktrue = -999.;

    // FINDING THE LEADING RECONSTRUCTED PHOTON
    
    int countino(0);
    double maxphotpt(0), maxphoteta(0);
    double maxjetpt(0);

    for(int i=0; i<nPhot; i++){
      
      bool tempmatched(0);      

      for(int j=0; j<nMC; j++){

	if(pdgIdMC[j]==22 && statusMC[j]==3){
	  double deltaR = sqrt(pow(etaMC[j]-etaPhot[i],2)+pow(phiMC[j]-phiPhot[i],2));
	  double deltaE = (eMC[j]-ePhot[i])/eMC[j];
	  
	  if(deltaR<0.1 && deltaE>-.02 && deltaE<.15) tempmatched = 1;
	}

      }
      
      if(ePhot[i]/cosh(etaPhot[i])>25 && TMath::Abs(etaPhot[i])<1.2) {
		
	if(ePhot[i]/cosh(etaPhot[i])>maxptphot) {
	  maxptphot = ePhot[i]/cosh(etaPhot[i]);
	  imaxptphot = i;		
	}

      } 

      allptphot->Fill(ePhot[i]/cosh(etaPhot[i]));
      
    }

   
    // LOOP OVER JETS - FINDING THE RECOILING RECO JET
    
    double mindeltaphi = 2*pi;
    int imindeltaphi = -1;
    int njetsoverthreshold = 0;
    
    for(int j=0; j<nJet; j++){
      
      allptjet->Fill(eJet[j]/cosh(etaJet[j]));

      if(eJet[j]/cosh(etaJet[j])>20.){
	
	double deltaphi = TMath::Abs(phiJet[j] - phiPhot[imaxptphot]) - pi;	  
	alldeltaphi->Fill(deltaphi);
	
	if(TMath::Abs(deltaphi)<mindeltaphi) {
	  mindeltaphi = TMath::Abs(deltaphi);
	  imindeltaphi = j;
	}

	// exclude the leading photon from the jet counting
	if(deltaphi>-pi+0.2) njetsoverthreshold++;
      }
    }

    double pt2ndjet(0);
    for(int j=0; j<nJet; j++){

      double deltaphi = TMath::Abs(phiJet[j] - phiPhot[imaxptphot]) - pi;	  
      if(j != imindeltaphi && deltaphi>-pi+0.2){

	if(eJet[j]/cosh(etaJet[j])>pt2ndjet && eJet[j]/cosh(etaJet[j])>15) 
	  pt2ndjet = eJet[j]/cosh(etaJet[j]);

      }
      
    }

    // FINDING THE LEADING MC PHOTON AND RECOILING QUARK
    
    bool matched(0);
    
    for(int i=0; i<nMC; i++){
      
      if(pdgIdMC[i]==22 && statusMC[i]==3){
	double deltaR = sqrt(pow(etaMC[i]-etaPhot[imaxptphot],2)+pow(phiMC[i]-phiPhot[imaxptphot],2));
	double deltaE = (eMC[i]-ePhot[imaxptphot])/eMC[i];
	alldeltar->Fill(deltaR);
	alldeltae->Fill(deltaE);
	alldeltardeltae->Fill(deltaR,deltaE);
	  	
	if(deltaR<0.1 && deltaE>-.02 && deltaE<.15) matched = 1;
	ileadmcphot = i;
      }
      
      if(pdgIdMC[i]!=22 && motherIDMC[i]==4){
	ileadmcquark = i;
      }

    }
    
    // continue only if the reco photon matches the MC one
    //    if(!matched) continue;
    
    double mindeltaphigen = 2*pi;
    int imindeltaphigen = -1;
    int njetsgenoverthreshold = 0;
    
    // LOOP OVER GEN JETS - FINDING THE RECOILING GEN JET
	
    for(int j=0; j<nJetGen; j++){
      
      if(eJetGen[j]/cosh(etaJetGen[j])>20.){
	
	double deltaphigen = TMath::Abs(phiJetGen[j] - phiMC[ileadmcphot]) - pi;
	
	if(TMath::Abs(deltaphigen)<mindeltaphigen) {
	  mindeltaphigen = TMath::Abs(deltaphigen);
	  imindeltaphigen = j;
	}
	njetsgenoverthreshold++;
      }
    }                

    double ecaliso015(0),  ecaliso02(0),  ecaliso025(0), ecaliso03(0), ecaliso035(0), ecaliso04(0);
    
    for(int u=0; u<nPhot; u++){
      double deltaR = sqrt(pow(etaPhot[u]-etaPhot[imaxptphot],2)+pow(phiPhot[u]-phiPhot[imaxptphot],2));
      if (TMath::Abs(deltaR)<0.15 && imaxptphot!=u) ecaliso015 += ePhot[u]/cosh(etaPhot[u]);	  
      if (TMath::Abs(deltaR)<0.2 && imaxptphot!=u) ecaliso02 += ePhot[u]/cosh(etaPhot[u]);	  
      if (TMath::Abs(deltaR)<0.25 && imaxptphot!=u) ecaliso025 += ePhot[u]/cosh(etaPhot[u]);
      if (TMath::Abs(deltaR)<0.3 && imaxptphot!=u) ecaliso03 += ePhot[u]/cosh(etaPhot[u]);
      if (TMath::Abs(deltaR)<0.35 && imaxptphot!=u) ecaliso035 += ePhot[u]/cosh(etaPhot[u]);
      if (TMath::Abs(deltaR)<0.4 && imaxptphot!=u) ecaliso04 += ePhot[u]/cosh(etaPhot[u]);
    }
    
    (*inputVec)[0] = ntrkiso035Phot[imaxptphot];
    (*inputVec)[1] = ptiso035Phot[imaxptphot]/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot]));
    // 	(*inputVec)[2] = ptisoatecal035Phot[imaxptphot]/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot]));
    (*inputVec)[2] = hcalovecal04Phot[imaxptphot];
    (*inputVec)[3] = sMajMajPhot[imaxptphot];
    (*inputVec)[4] = sMinMinPhot[imaxptphot];
    (*inputVec)[5] = (ecaliso04Phot[imaxptphot]-ePhot[imaxptphot])/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot]));

    double nnval_old = classReader->GetMvaValue( *inputVec );
    double nnval = 0;
    if(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])<60)                                                     nnval = classReader1->GetMvaValue( *inputVec );
    if(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])<100 && ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])>60)  nnval = classReader2->GetMvaValue( *inputVec );
    if(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])<200 && ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])>100) nnval = classReader3->GetMvaValue( *inputVec );
    if(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])>200)                                                    nnval = classReader4->GetMvaValue( *inputVec );
    
    if(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])>25 && TMath::Abs(etaPhot[imaxptphot])<1.2) {
      if(matched){
	sMajMajphot->Fill(sMajMajPhot[imaxptphot]);
	sMinMinphot->Fill(sMinMinPhot[imaxptphot]);
	sMinMinsMajMajphot->Fill(sMinMinPhot[imaxptphot],sMajMajPhot[imaxptphot]);
	Latphot->Fill(LATPhot[imaxptphot]);
	fisherphot->Fill(FisherPhot[imaxptphot]);	
	ptiso03phot->Fill(ptiso03Phot[imaxptphot]);
	ptiso02relphot->Fill(ptiso02Phot[imaxptphot]/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])));
	ptiso025relphot->Fill(ptiso025Phot[imaxptphot]/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])));
	ptiso03relphot->Fill(ptiso03Phot[imaxptphot]/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])));
	ptiso035relphot->Fill(ptiso035Phot[imaxptphot]/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])));
	ptiso04relphot->Fill(ptiso04Phot[imaxptphot]/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])));
	ntrkiso03phot->Fill(ntrkiso03Phot[imaxptphot]);
	ptisoatecal03phot->Fill(ptisoatecal03Phot[imaxptphot]);
	ptisoatecal02relphot->Fill(ptisoatecal02Phot[imaxptphot]/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])));
	ptisoatecal025relphot->Fill(ptisoatecal025Phot[imaxptphot]/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])));
	ptisoatecal03relphot->Fill(ptisoatecal03Phot[imaxptphot]/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])));
	ptisoatecal035relphot->Fill(ptisoatecal035Phot[imaxptphot]/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])));
	ptisoatecal04relphot->Fill(ptisoatecal04Phot[imaxptphot]/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])));
	ptisovsatecal03relphot->Fill(ptiso03Phot[imaxptphot]/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])),ptisoatecal03Phot[imaxptphot]/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])));
	ntrkisoatecal03phot->Fill(ntrkisoatecal03Phot[imaxptphot]);
	hcalovecal01phot->Fill(hcalovecal01Phot[imaxptphot]);
	hcalovecal02phot->Fill(hcalovecal02Phot[imaxptphot]);
	hcalovecal025phot->Fill(hcalovecal025Phot[imaxptphot]);
	hcalovecal03phot->Fill(hcalovecal03Phot[imaxptphot]);
	hcalovecal04phot->Fill(hcalovecal04Phot[imaxptphot]);
	hcalovecal05phot->Fill(hcalovecal05Phot[imaxptphot]);
	ecaliso04phot->Fill(ecaliso04);	  
	ecaliso015relphot->Fill(ecaliso015/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])));	  
	ecaliso02relphot->Fill(ecaliso02/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])));	  
	ecaliso025relphot->Fill(ecaliso025/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])));	  
	ecaliso03relphot->Fill(ecaliso03/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])));	  
	ecaliso035relphot->Fill(ecaliso035/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])));	  
	ecaliso04relphot->Fill(ecaliso04/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])));	  
	ecaliso02recrelphot->Fill((ecaliso02Phot[imaxptphot]-ePhot[imaxptphot])/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])));	  
	ecaliso03recrelphot->Fill((ecaliso03Phot[imaxptphot]-ePhot[imaxptphot])/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])));	  
	ecaliso035recrelphot->Fill((ecaliso035Phot[imaxptphot]-ePhot[imaxptphot])/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])));	  
	ecaliso04recrelphot->Fill((ecaliso04Phot[imaxptphot]-ePhot[imaxptphot])/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])));	  
	ecaliso05recrelphot->Fill((ecaliso05Phot[imaxptphot]-ePhot[imaxptphot])/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])));	  
	
	pts = ePhot[imaxptphot]/cosh(etaPhot[imaxptphot]);
	ntrkisos = ntrkiso035Phot[imaxptphot];
	ptisos = ptiso035Phot[imaxptphot]/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot]));
	ptisoatecals = ptisoatecal035Phot[imaxptphot]/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot]));
	hcalovecals = hcalovecal04Phot[imaxptphot];
	sMajMajs = sMajMajPhot[imaxptphot];
	sMinMins = sMinMinPhot[imaxptphot];
	ecalisos = (ecaliso04Phot[imaxptphot]-ePhot[imaxptphot])/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot]));
	S_tree->Fill();
	nnphot_old->Fill(nnval_old);
	nnphot->Fill(nnval);
      } else {
	if(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])>maxjetpt) maxjetpt = ePhot[imaxptphot]/cosh(etaPhot[imaxptphot]);	    
	sMajMajbkg->Fill(sMajMajPhot[imaxptphot]);
	sMinMinbkg->Fill(sMinMinPhot[imaxptphot]);
	sMinMinsMajMajbkg->Fill(sMinMinPhot[imaxptphot],sMajMajPhot[imaxptphot]);
	Latbkg->Fill(LATPhot[imaxptphot]);
	fisherbkg->Fill(FisherPhot[imaxptphot]);	
	ptiso03bkg->Fill(ptiso03Phot[imaxptphot]);
	ptiso02relbkg->Fill(ptiso02Phot[imaxptphot]/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])));
	ptiso025relbkg->Fill(ptiso025Phot[imaxptphot]/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])));
	ptiso03relbkg->Fill(ptiso03Phot[imaxptphot]/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])));
	ptiso035relbkg->Fill(ptiso035Phot[imaxptphot]/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])));
	ptiso04relbkg->Fill(ptiso04Phot[imaxptphot]/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])));
	ntrkiso03bkg->Fill(ntrkiso03Phot[imaxptphot]);
	ptisoatecal03bkg->Fill(ptisoatecal03Phot[imaxptphot]);
	ptisoatecal02relbkg->Fill(ptisoatecal02Phot[imaxptphot]/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])));
	ptisoatecal025relbkg->Fill(ptisoatecal025Phot[imaxptphot]/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])));
	ptisoatecal03relbkg->Fill(ptisoatecal03Phot[imaxptphot]/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])));
	ptisoatecal035relbkg->Fill(ptisoatecal035Phot[imaxptphot]/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])));
	ptisoatecal04relbkg->Fill(ptisoatecal04Phot[imaxptphot]/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])));
	ptisovsatecal03relbkg->Fill(ptiso03Phot[imaxptphot]/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])),ptisoatecal03Phot[imaxptphot]/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])));
	ntrkisoatecal03bkg->Fill(ntrkisoatecal03Phot[imaxptphot]);
	hcalovecal01bkg->Fill(hcalovecal01Phot[imaxptphot]);
	hcalovecal02bkg->Fill(hcalovecal02Phot[imaxptphot]);
	hcalovecal025bkg->Fill(hcalovecal025Phot[imaxptphot]);
	hcalovecal03bkg->Fill(hcalovecal03Phot[imaxptphot]);
	hcalovecal04bkg->Fill(hcalovecal04Phot[imaxptphot]);
	hcalovecal05bkg->Fill(hcalovecal05Phot[imaxptphot]);
	ecaliso04bkg->Fill(ecaliso04);	  
	ecaliso015relbkg->Fill(ecaliso015/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])));	  
	ecaliso02relbkg->Fill(ecaliso02/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])));	  
	ecaliso025relbkg->Fill(ecaliso025/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])));	  
	ecaliso03relbkg->Fill(ecaliso03/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])));	  
	ecaliso035relbkg->Fill(ecaliso035/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])));	  
	ecaliso04relbkg->Fill(ecaliso04/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])));	  
	ecaliso02recrelbkg->Fill((ecaliso02Phot[imaxptphot]-ePhot[imaxptphot])/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])));	  
	ecaliso03recrelbkg->Fill((ecaliso03Phot[imaxptphot]-ePhot[imaxptphot])/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])));	  
	ecaliso035recrelbkg->Fill((ecaliso035Phot[imaxptphot]-ePhot[imaxptphot])/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])));	  
	ecaliso04recrelbkg->Fill((ecaliso04Phot[imaxptphot]-ePhot[imaxptphot])/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])));	  
	ecaliso05recrelbkg->Fill((ecaliso05Phot[imaxptphot]-ePhot[imaxptphot])/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])));	  

	ptb = ePhot[imaxptphot]/cosh(etaPhot[imaxptphot]);
	ntrkisob = ntrkiso035Phot[imaxptphot];
	ptisob = ptiso035Phot[imaxptphot]/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot]));
	ptisoatecalb = ptisoatecal035Phot[imaxptphot]/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot]));
	hcalovecalb = hcalovecal04Phot[imaxptphot];
	sMajMajb = sMajMajPhot[imaxptphot];
	sMinMinb = sMinMinPhot[imaxptphot];
	ecalisob = (ecaliso04Phot[imaxptphot]-ePhot[imaxptphot])/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot]));
	B_tree->Fill();	   
	nnbkg_old->Fill(nnval_old);
	nnbkg->Fill(nnval);
      }
      nniso = nnval;
    }

    // variable calculation

    double ptphot = ePhot[imaxptphot]/cosh(etaPhot[imaxptphot]);
    double ptphotgen =  eMC[ileadmcphot]/cosh(etaMC[ileadmcphot]);
    double ptjet = eJet[imindeltaphi]/cosh(etaJet[imindeltaphi]);
    double diffpt = ptjet - ptphot;

    double ptjetgen = eJetGen[imindeltaphigen]/cosh(etaJetGen[imindeltaphigen]);
    double diffptgen = ptphotgen - ptjetgen;

    double ptjetgenquark = eMC[ileadmcquark]/cosh(etaMC[ileadmcquark]);
    double diffptgenquark = ptphotgen - ptjetgenquark;

    // cuts

    bool etacut =  TMath::Abs(etaPhot[imaxptphot]) < 1.47; 
    bool ptcut = ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])>28;
    bool istrkcut = ptiso03Phot[imaxptphot]/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])) < 0.04;
    bool istrkatecalcut = ptisoatecal03Phot[imaxptphot]/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])) < 0.04;
    bool ntrkisocut = ntrkisoatecal03Phot[imaxptphot] < 3;
    bool hcalovecalcut = TMath::Abs(hcalovecal025Phot[imaxptphot]) < .03;
    bool fishercut = FisherPhot[imaxptphot] > -0.35 && FisherPhot[imaxptphot] < 0. ;
    bool deltaphicut = TMath::Abs(phiJet[imindeltaphi] - phiPhot[imaxptphot])-pi<.2 && TMath::Abs(phiJet[imindeltaphi] - phiPhot[imaxptphot])-pi>-.2;
    bool njetoverthrcut = njetsoverthreshold == 1;
    // new cuts
    bool pt2jetcut = pt2ndjet/ptphot < 0.2;
    bool nnphotcut = nniso>0.94;
    bool etanewcut =  TMath::Abs(etaPhot[imaxptphot]) < 1.2; 
    bool etajetcut =  TMath::Abs(etaJet[imindeltaphi]) < 1.2; 
    
    //    passcuts = etacut && ptcut && istrkcut && istrkatecalcut && ntrkisocut && hcalovecalcut && fishercut && deltaphicut && njetoverthrcut; 
    passloosecuts = etacut && ptcut && deltaphicut && njetoverthrcut;     

    passcuts = etacut && ptcut && istrkatecalcut && ntrkisocut && hcalovecalcut && fishercut && deltaphicut && njetoverthrcut;     
    // new cuts
    //    passcuts = nnphotcut && ptcut && etanewcut && etajetcut && deltaphicut && pt2jetcut;
    passcuts =  istrkatecalcut && ntrkisocut && hcalovecalcut && fishercut  && ptcut && etanewcut && etajetcut && deltaphicut && pt2jetcut;

    if(etacut) {
      deltaptnormvspt_nocut->Fill(ptphot,diffpt/ptphot);
      pt_nocut->Fill(ptphot);
      if(matched) pt_nocut_matched->Fill(ptphot);
      if(ptcut) {
	deltaptnormvspt_cut1->Fill(ptphot,diffpt/ptphot);
	pt_cut1->Fill(ptphot);
	if(matched) pt_cut1_matched->Fill(ptphot);
	// 	if(istrkcut) {
	// 	  deltaptnormvspt_cut2->Fill(ptphot,diffpt/ptphot);
	// 	  pt_cut2->Fill(ptphot);
	  if(istrkatecalcut) {
	    deltaptnormvspt_cut3->Fill(ptphot,diffpt/ptphot);
	    pt_cut3->Fill(ptphot);
	    if(matched) pt_cut3_matched->Fill(ptphot);
	    if(ntrkisocut) {
	      deltaptnormvspt_cut4->Fill(ptphot,diffpt/ptphot);
	      pt_cut4->Fill(ptphot);
	      if(matched) pt_cut4_matched->Fill(ptphot);
	      if(hcalovecalcut) {
		deltaptnormvspt_cut5->Fill(ptphot,diffpt/ptphot);
		pt_cut5->Fill(ptphot);
		if(matched) pt_cut5_matched->Fill(ptphot);
		if(fishercut) {
		  deltaptnormvspt_cut6->Fill(ptphot,diffpt/ptphot);
		  pt_cut6->Fill(ptphot);
		  if(matched) pt_cut6_matched->Fill(ptphot);
		  if(njetoverthrcut) {
		    deltaptnormvspt_cut7->Fill(ptphot,diffpt/ptphot);    
		    pt_cut7->Fill(ptphot);    
		    if(matched) pt_cut7_matched->Fill(ptphot);    
		    if(deltaphicut) {
// 		      deltaptnormvspt_all->Fill(ptphot,diffpt/ptphot);    
// 		      pt_all->Fill(ptphot); 
// 		      if(matched) pt_all_matched->Fill(ptphot); 
		    }
		  }
		}
	      }
	    }
	  }
	  //	}
      }
    }
    
    // MC leading photon spectrum
    bestptphotgen->Fill(eMC[ileadmcphot]/cosh(etaMC[ileadmcphot]));
    bestptphotgenlarge->Fill(eMC[ileadmcphot]/cosh(etaMC[ileadmcphot]));
    if(TMath::Abs(etaMC[ileadmcphot]) < 1.47) bestptphotgen_etapt->Fill(eMC[ileadmcphot]/cosh(etaMC[ileadmcphot]));

    // RECO PHOTON AND JET PLOTS

    if(etacut && ptcut) {
      bestptphot->Fill(ptphot);
      bestptphotlarge->Fill(ptphot);      
      bestptjet->Fill(ptjet);
      bestdeltaphi->Fill(TMath::Abs(phiJet[imindeltaphi] - phiPhot[imaxptphot]) - pi);
      bestdeltaphizoom->Fill(TMath::Abs(phiJet[imindeltaphi] - phiPhot[imaxptphot]) - pi);
      deltaptnormvsdeltaphi->Fill(TMath::Abs(TMath::Abs(phiJet[imindeltaphi] - phiPhot[imaxptphot]) - pi),diffpt/ptphot); 
      deltaphi_all->Fill(TMath::Abs(phiJet[imindeltaphi] - phiPhot[imaxptphot]) - pi);
      if(deltaphicut){
	njetoverth_all->Fill(njetsoverthreshold);
	ptjet2ndovptphot_all->Fill(pt2ndjet/ptphot);
	ptjet2_all->Fill(pt2ndjet/ptphot);
	etaphot_all->Fill(etaPhot[imaxptphot]);
	etajet_all->Fill(etaJet[imindeltaphi]);
      }
    }
      
    if(passcuts) { 
      deltapt->Fill(diffpt);      
      deltaptnorm->Fill(diffpt/ptphot);
      ratioptnorm->Fill(ptjet/ptphot);
      if(TMath::Abs(etaJet[imindeltaphi])<1.4) deltaptnormbar->Fill(diffpt/ptphot);      
      deltaptvseta->Fill(etaPhot[imaxptphot],diffpt/ptphot);
      deltaptnormvseta->Fill(etaPhot[imaxptphot],diffpt/ptphot);
      ratioptnormvspt->Fill(ptphot,ptjet/ptphot);    
      deltaptnormvspt->Fill(ptphot,diffpt/ptphot);      
      resophot->Fill((ptphot - ptphotgen)/ptphotgen);
      resophotvspt->Fill(ptphotgen,(ptphot - ptphotgen)/ptphotgen);
      deltaptnormvspt_all->Fill(ptphot,diffpt/ptphot);    
      pt_all->Fill(ptphot); 
      if(matched) pt_all_matched->Fill(ptphot); 
    }
      
    if(passloosecuts)   {
      ratioptnorm_loose->Fill(ptjet/ptphot);  
      ratioptnormvspt_loose->Fill(ptphot,ptjet/ptphot);  
      pt_loose->Fill(ptphot); 
    }

    // GEN JET AND JET RESOLUTION PLOTS
    
    if(etacut && ptcut) {
      bestdeltaphijet->Fill(TMath::Abs(phiJetGen[imindeltaphigen] - phiMC[ileadmcphot]) - pi);
      bestdeltaphijetzoom->Fill(TMath::Abs(phiJetGen[imindeltaphigen] - phiMC[ileadmcphot]) - pi);
      deltaptjetnormvspt_etaptcut->Fill(ptphotgen,diffptgen/ptphotgen);
      deltaptjetnorm_etaptcut->Fill(diffptgen/ptphotgen);
      if(deltaphicut){
	deltaptjetnormvspt_deltaphi->Fill(ptphotgen,diffptgen/ptphotgen);
	deltaptjetnorm_deltaphi->Fill(diffptgen/ptphotgen);
      }
    }

    if(passcuts) {     
      deltaptjet->Fill(diffptgen);      
      deltaptjetnorm->Fill(diffptgen/ptphotgen);
      deltaptjetnormvspt->Fill(ptphotgen,diffptgen/ptphotgen);
      if(TMath::Abs(etaJetGen[imindeltaphigen])<1.4) deltaptjetnormbar->Fill(diffptgen/ptphotgen);      
      deltaptvsetajet->Fill(etaJetGen[imindeltaphigen],diffptgen/ptphotgen);
      double deltaRjet = sqrt(pow(etaJet[imindeltaphi]-etaJetGen[imindeltaphigen],2)+pow(phiJet[imindeltaphi]-phiJetGen[imindeltaphigen],2));
      double deltaEjet = (eJet[imindeltaphi]-eJetGen[imindeltaphigen])/eJetGen[imindeltaphigen];
      deltarjet->Fill(deltaRjet);
      deltaejet->Fill(deltaEjet);
      deltardeltaejet->Fill(deltaRjet,deltaEjet);
      resojet->Fill((ptjet-ptjetgen)/ptjetgen);
      resojetvspt->Fill(ptjetgen,(ptjet-ptjetgen)/ptjetgen);
    }

    // QUARK PLOTS AND COMPARISON WITH GEN JET

    if(etacut && ptcut) {
      bestdeltaphijetquark->Fill(TMath::Abs(phiMC[ileadmcquark] - phiMC[ileadmcphot]) - pi);
      bestdeltaphijetquarkzoom->Fill(TMath::Abs(phiMC[ileadmcquark] - phiMC[ileadmcphot]) - pi);
      deltaptjetquarknormvsdeltaphi->Fill(TMath::Abs(TMath::Abs(phiMC[ileadmcquark] - phiMC[ileadmcphot]) - pi),diffptgenquark/ptphotgen);      
      deltaptjetquarknormvspt_etaptcut->Fill(ptphotgen,diffptgen/ptphotgen);
      deltaptjetquarknorm_etaptcut->Fill(diffptgen/ptphotgen);
      if(deltaphicut) {
	deltaptjetquarknormvspt_deltaphi->Fill(ptphotgen,diffptgen/ptphotgen);
	deltaptjetquarknorm_deltaphi->Fill(diffptgen/ptphotgen);
      }
    }

    if(passcuts) {     
      deltaptjetquark->Fill(diffptgenquark);      
      deltaptjetquarknorm->Fill(diffptgenquark/ptphotgen);
      deltaptjetquarknormvspt->Fill(ptphotgen,diffptgenquark/ptphotgen);
      if(TMath::Abs(etaMC[ileadmcquark])<1.4) deltaptjetquarknormbar->Fill(diffptgenquark/ptphotgen);      
      deltaptvsetajetquark->Fill(etaMC[ileadmcquark],diffptgenquark/ptphotgen);
      double deltaRjetquark = sqrt(pow(etaJet[imindeltaphi]-etaMC[ileadmcquark],2)+pow(phiJet[imindeltaphi]-phiMC[ileadmcquark],2));
      double deltaEjetquark = (eJet[imindeltaphi]-eMC[ileadmcquark])/eMC[ileadmcquark];
      deltarjetquark->Fill(deltaRjetquark);
      deltaejetquark->Fill(deltaEjetquark);
      deltardeltaejetquark->Fill(deltaRjetquark,deltaEjetquark);
      resojetMC->Fill((ptjetgen-ptjetgenquark)/ptjetgenquark);
      resojetMCvspt->Fill(ptjetgenquark,(ptjetgen-ptjetgenquark)/ptjetgenquark);    
    }
    
    isphoton = matched;
    issignal = ptphotgen>25;
    weight = -1;
    ptph = ptphot;
    etaph =  etaPhot[imaxptphot];
    phiph = phiPhot[imaxptphot];
    ptj = ptjet;
    etaj = etaJet[imindeltaphi];
    phij = phiJet[imindeltaphi];
    pt2jet = pt2ndjet;
    ptjettrue = ptjetgen;
    //    etajettrue = etaJetGen[imindeltaphigen];
    phijettrue = phiJetGen[imindeltaphigen];
    ptquarktrue = ptjetgenquark;
    //    etaquarktrue = etaMC[ileadmcquark];
    phiquarktrue = phiMC[ileadmcquark];
    //    if(ptph>25) ana_tree->Fill();
  }

  delete inputVec;
}


void testgjet_new::BookHistos()
{
   S_tree = new TTree ("TreeS","Reduced tree for photon studies: S") ;
   B_tree = new TTree ("TreeB","Reduced tree for photon studies: B") ;
   ana_tree = new TTree ("AnaTree","Reduced tree for final analysis") ;

   S_tree->Branch("pt",&pts,"pts/F");
   S_tree->Branch("ptiso",&ptisos,"ptisos/F");
   S_tree->Branch("ntrkiso",&ntrkisos,"ntrkisos/I");
   S_tree->Branch("ptisoatecal",&ptisoatecals,"ptisoatecals/F");
   S_tree->Branch("hcalovecal",&hcalovecals,"hcalovecal0s/F");
   S_tree->Branch("sMajMaj",&sMajMajs,"sMajMajs/F");
   S_tree->Branch("sMinMin",&sMinMins,"sMinMins/F");
   S_tree->Branch("ecaliso",&ecalisos,"ecalisos/F");
   B_tree->Branch("pt",&ptb,"ptb/F");
   B_tree->Branch("ptiso",&ptisob,"ptisob/F");
   B_tree->Branch("ntrkiso",&ntrkisob,"ntrkisob/I");
   B_tree->Branch("ptisoatecal",&ptisoatecalb,"ptisoatecalb/F");
   B_tree->Branch("hcalovecal",&hcalovecalb,"hcalovecalb/F");
   B_tree->Branch("sMajMaj",&sMajMajb,"sMajMajb/F");
   B_tree->Branch("sMinMin",&sMinMinb,"sMinMinb/F");
   B_tree->Branch("ecaliso",&ecalisob,"ecalisob/F");

   ana_tree->Branch("isphoton",&isphoton,"isphoton/I");
   ana_tree->Branch("issignal",&issignal,"issignal/I");
   ana_tree->Branch("weight",&weight,"weight/F");
   ana_tree->Branch("nniso",&nniso,"nniso/F");
   ana_tree->Branch("ptphot",&ptph,"ptph/F");
   ana_tree->Branch("ptjet",&ptj,"ptj/F");
   ana_tree->Branch("etaphot",&etaph,"etaph/F");
   ana_tree->Branch("etajet",&etaj,"etaj/F");
   ana_tree->Branch("phiphot",&phiph,"phiph/F");
   ana_tree->Branch("phijet",&phij,"phij/F");
   ana_tree->Branch("pt2jet",&pt2jet,"pt2jet/F");
   ana_tree->Branch("ptphottrue",&ptphottrue,"ptphottrue/F");
   ana_tree->Branch("ptjettrue",&ptjettrue,"ptjettrue/F");
   ana_tree->Branch("ptquarktrue",&ptquarktrue,"ptquarktrue/F");
   ana_tree->Branch("phiphottrue",&phiphottrue,"phiphottrue/F");
   ana_tree->Branch("phijettrue",&phijettrue,"phijettrue/F");
   ana_tree->Branch("phiquarktrue",&phiquarktrue,"phiquarktrue/F");


   double pi = 3.14159;
   allptphot = new TH1D("allptphot","allptphot",100,0.,700.);
   bestptphot = new TH1D("bestptphot","bestptphot",100,0.,700.);
   bestptphotlarge = new TH1D("bestptphotlarge","bestptphotlarge",200,0.,2000.);
   allptjet = new TH1D("allptjet","allptjet",100,0.,700.);
   bestptjet = new TH1D("bestptjet","bestptjet",100,0.,700.);

   resophot = new TH1D("resophot","resophot",200,-.2,.2);
   resophotvspt = new TH2D("resophotvspt","resophotvspt",100,0.,700.,200,-.2,.2);
   biasphotvspt_fitted = new TH1D("biasphotvspt_fitted","biasphotvspt",100,0.,700);
   resophotvspt_fitted = new TH1D("resophotvspt_fitted","resophotvspt",100,0.,700);

   resojet = new TH1D("resojet","resojet",200,-1,1.);
   resojetvspt = new TH2D("resojetvspt","resojetvspt",100,0.,700.,200,-1.,1.);
   biasjetvspt_fitted = new TH1D("biasjetvspt_fitted","biasjetvspt",100,0.,700);
   resojetvspt_fitted = new TH1D("resojetvspt_fitted","resojetvspt",100,0.,700);

   resojetMC = new TH1D("resojetMC","resojetMC",200,-1,1.);
   resojetMCvspt = new TH2D("resojetMCvspt","resojetMCvspt",100,0.,700.,200,-1.,1.);
   biasjetMCvspt_fitted = new TH1D("biasjetMCvspt_fitted","biasjetMCvspt",100,0.,700);
   resojetMCvspt_fitted = new TH1D("resojetMCvspt_fitted","resojetMCvspt",100,0.,700);

   bestptphotgen = new TH1D("bestptphotgen","bestptphotgen",100,0.,700.);
   bestptphotgen_etapt = new TH1D("bestptphotgen_etapt","bestptphotgen_etapt",100,0.,700.);
   bestptphotgenlarge = new TH1D("bestptphotgenlarge","bestptphotgenlarge",200,0.,2000.);

   deltapt = new TH1D("deltapt","deltapt",100,-200.,200.);
   deltaptnorm = new TH1D("deltaptnorm","deltaptnorm",100,-1.,1.);
   ratioptnorm = new TH1D("ratioptnorm","ratioptnorm",100,0.,2.);
   ratioptnorm_loose = new TH1D("ratioptnorm_loose","ratioptnorm_loose",100,0.,2.);
   deltaptnormbar = new TH1D("deltaptnormbar","deltaptnormbar",100,-1.,1.);
   ratioptnormvspt = new TH2D("ratioptnormvspt","ratioptnorm",100,0.,700.,200,0.,2.);
   ratioptnormvspt_loose = new TH2D("ratioptnormvspt_loose","ratioptnorm_loose",100,0.,700.,200,0.,2.);
   deltaptnormvspt = new TH2D("deltaptnormvspt","deltaptnorm",100,0.,700.,200,-1.,1.);
   deltaptnormvspt_nocut = new TH2D("deltaptnormvspt_nocut","deltaptnorm no cut",100,0.,700.,200,-1.,1.);
   deltaptnormvspt_cut1 = new TH2D("deltaptnormvspt_cut1","deltaptnorm cut1",100,0.,700.,200,-1.,1.);
   deltaptnormvspt_cut2 = new TH2D("deltaptnormvspt_cut2","deltaptnorm cut2",100,0.,700.,200,-1.,1.);
   deltaptnormvspt_cut3 = new TH2D("deltaptnormvspt_cut3","deltaptnorm cut3",100,0.,700.,200,-1.,1.);
   deltaptnormvspt_cut4 = new TH2D("deltaptnormvspt_cut4","deltaptnorm cut4",100,0.,700.,200,-1.,1.);
   deltaptnormvspt_cut5 = new TH2D("deltaptnormvspt_cut5","deltaptnorm cut5",100,0.,700.,200,-1.,1.);
   deltaptnormvspt_cut6 = new TH2D("deltaptnormvspt_cut6","deltaptnorm cut6",100,0.,700.,200,-1.,1.);
   deltaptnormvspt_cut7 = new TH2D("deltaptnormvspt_cut7","deltaptnorm cut7",100,0.,700.,200,-1.,1.);
   deltaptnormvspt_all  = new TH2D("deltaptnormvspt_all","deltaptnorm all",100,0.,700.,200,-1.,1.);

   pt_nocut_matched = new TH1D("pt_nocut_matched","pt no cut",100,0.,700.);
   pt_cut1_matched = new TH1D("pt_cut1_matched","pt cut1",100,0.,700.);
   pt_cut2_matched = new TH1D("pt_cut2_matched","pt cut2",100,0.,700.);
   pt_cut3_matched = new TH1D("pt_cut3_matched","pt cut3",100,0.,700.);
   pt_cut4_matched = new TH1D("pt_cut4_matched","pt cut4",100,0.,700.);
   pt_cut5_matched = new TH1D("pt_cut5_matched","pt cut5",100,0.,700.);
   pt_cut6_matched = new TH1D("pt_cut6_matched","pt cut6",100,0.,700.);
   pt_cut7_matched = new TH1D("pt_cut7_matched","pt cut7",100,0.,700.);
   pt_all_matched  = new TH1D("pt_all_matched ","pt cut7",100,0.,700.);

   pt_nocut= new TH1D("pt_nocut","pt no cut",100,0.,700.);
   pt_cut1 = new TH1D("pt_cut1","pt cut1",100,0.,700.);
   pt_cut2 = new TH1D("pt_cut2","pt cut2",100,0.,700.);
   pt_cut3 = new TH1D("pt_cut3","pt cut3",100,0.,700.);
   pt_cut4 = new TH1D("pt_cut4","pt cut4",100,0.,700.);
   pt_cut5 = new TH1D("pt_cut5","pt cut5",100,0.,700.);
   pt_cut6 = new TH1D("pt_cut6","pt cut6",100,0.,700.);
   pt_cut7 = new TH1D("pt_cut7","pt cut7",100,0.,700.);
   pt_all  = new TH1D("pt_all ","pt cut7",100,0.,700.);
   pt_loose  = new TH1D("pt_loose ","pt loose",100,0.,700.);

   biasdeltaptnormvspt_fitted = new TH1D("biasdeltaptnormvspt","deltaptnorm",100,0.,700.);
   resodeltaptnormvspt_fitted = new TH1D("resodeltaptnormvspt","deltaptnorm",100,0.,700.);   
   deltaptnormvseta = new TH2D("deltaptnormvseta","deltaptnorm",100,-5.,5.,200,-1.,1.);
   biasdeltaptnormvseta_fitted = new TH1D("biasdeltaptnormvseta","deltaptnorm",100,-5.,5.);
   resodeltaptnormvseta_fitted = new TH1D("resodeltaptnormvseta","deltaptnorm",100,-5.,5.);   

   deltaptjet = new TH1D("deltaptjet","deltaptjet",100,-200.,200.);
   deltaptjetnorm = new TH1D("deltaptjetnorm","deltaptjetnorm",100,-1.,1.);
   deltaptjetnormbar = new TH1D("deltaptjetnormbar","deltaptjetnormbar",100,-1.,1.);
   deltaptjetnorm_etaptcut = new TH1D("deltaptjetnorm_etaptcut","deltaptjetnorm after eta and pt cut",100,-1.,1.);
   deltaptjetnorm_deltaphi = new TH1D("deltaptjetnorm_deltaphi","deltaptjetnorm after deltaphi, eta and pt",100,-1.,1.);

   deltaptjetquark = new TH1D("deltaptjetquark","deltaptjetquark",100,-200.,200.);
   deltaptjetquarknorm = new TH1D("deltaptjetquarknorm","deltaptjetquarknorm",100,-1.,1.);
   deltaptjetquarknorm_etaptcut = new TH1D("deltaptjetquarknorm_etaptcut","deltaptjetquarknorm after eta and pt cut",100,-1.,1.);
   deltaptjetquarknorm_deltaphi = new TH1D("deltaptjetquarknorm_deltaphi","deltaptjetquarknorm after deltaphi, eta and pt",100,-1.,1.);
   deltaptjetnormvspt = new TH2D("deltaptjetnormvspt","deltaptjetnorm",100,0.,700.,200,-1.,1.);
   deltaptjetnormvspt_etaptcut = new TH2D("deltaptjetnormvspt_etaptcut","deltaptjetnorm after eta and pt cut",100,0.,700.,200,-1.,1.);
   deltaptjetnormvspt_deltaphi = new TH2D("deltaptjetnormvspt_deltaphi","deltaptjetnorm after deltaphi, eta and pt",100,0.,700.,200,-1.,1.);
   biasdeltaptjetnormvspt_fitted = new TH1D("biasdeltaptjetnormvspt","deltaptjetnorm",100,0.,700.);
   resodeltaptjetnormvspt_fitted = new TH1D("resodeltaptjetnormvspt","deltaptjetnorm",100,0.,700.);   

   deltaptjetquarknormbar = new TH1D("deltaptjetquarknormbar","deltaptjetquarknormbar",100,-1.,1.);
   deltaptnormvsdeltaphi = new TH2D("deltaptnormvsdeltaphi","deltaptnormvsdeltaphi",20,0.,1.,200,-1.,1.);
   deltaptvseta = new TProfile("deltaptvseta","deltaptvseta",20,-4.,4.);
   deltaptjetquarknormvspt = new TH2D("deltaptjetquarknormvspt","deltaptjetquarknorm",100,0.,700.,200,-1.,1.);
   deltaptjetquarknormvspt_etaptcut = new TH2D("deltaptjetquarknormvspt_etaptcut","deltaptjetquarknorm after eta and pt cut",100,0.,700.,200,-1.,1.);
   deltaptjetquarknormvspt_deltaphi = new TH2D("deltaptjetquarknormvspt_deltaphi","deltaptjetquarknorm after deltaphi, eta and pt",100,0.,700.,200,-1.,1.);
   deltaptjetquarknormvsdeltaphi = new TH2D("deltaptjetquarknormvsdeltaphi","deltaptjetquarknormvsdeltaphi",20,0.,1.,200,-1.,1.);
   biasdeltaptjetquarknormvspt_fitted = new TH1D("biasdeltaptjetquarknormvspt","deltaptjetquarknorm",100,0.,700.);
   resodeltaptjetquarknormvspt_fitted = new TH1D("resodeltaptjetquarknormvspt","deltaptjetquarknorm",100,0.,700.);   
 
   alldeltaphi = new TH1D("alldeltaphi","alldeltaphi",200,-2*pi,2*pi);
   bestdeltaphi = new TH1D("bestdeltaphi","bestdeltaphi",200,-2*pi,2*pi);
   bestdeltaphizoom = new TH1D("bestdeltaphizoom","bestdeltaphizoom",200,-.2*pi,.2*pi);
   alldeltar = new TH1D("alldeltar","alldeltar",200,0.,1.);
   alldeltae = new TH1D("alldeltae","alldeltar",200,-.1,0.1);
   alldeltardeltae = new TH2D("alldeltardeltae","alldeltardeltae",200,0.,1.,200,-.1,0.1);

   bestdeltaphijet = new TH1D("bestdeltaphijet","bestdeltaphijet",200,-2*pi,2*pi);
   bestdeltaphijetzoom = new TH1D("bestdeltaphijetzoom","bestdeltaphijetzoom",200,-.2*pi,.2*pi);
   deltarjet = new TH1D("deltarjet","deltarjet",200,0.,1.);
   deltaejet = new TH1D("deltaejet","deltaejet",100,-.5,0.5);
   deltardeltaejet = new TH2D("deltardeltaejet","deltardeltaejet",200,0.,1.,100,-.5,0.5);
   deltaptvsetajet = new TProfile("deltaptvsetajet","deltaptvsetajet",20,-4.,4.);
  
   bestdeltaphijetquark = new TH1D("bestdeltaphijetquark","bestdeltaphijetquark",200,-2*pi,2*pi);
   bestdeltaphijetquarkzoom = new TH1D("bestdeltaphijetquarkzoom","bestdeltaphijetquarkzoom",200,-.2*pi,.2*pi);
   deltarjetquark = new TH1D("deltarjetquark","deltarjetquark",200,0.,1.);
   deltaejetquark = new TH1D("deltaejetquark","deltaejetquark",100,-.5,0.5);
   deltardeltaejetquark = new TH2D("deltardeltaejetquark","deltardeltaejetquark",200,0.,1.,100,-.5,0.5);
   deltaptvsetajetquark = new TProfile("deltaptvsetajetquark","deltaptvsetajetquark",20,-4.,4.);
   
   sMajMajphot = new TH1D("sMajMajphot","2^{nd} Maj Moment for #gamma",1000,0.,2.);
   sMinMinphot = new TH1D("sMinMinphot","2^{nd} Min Moment for #gamma",1000,0.,2.);
   sMinMinsMajMajphot = new TH2D("sMinMinMajMajphot","2^{nd} Min vs Maj Moment for #gamma",200,0.,2.,200,0.,2.);
   Latphot = new TH1D("Latphot","LAT for #gamma",200,0,2.);
   fisherphot = new TH1D("fisherphot","Fisher for #gamma",200,-2.,0.);
   ptiso03phot = new TH1D("ptiso03phot","trk isolation R=0.3 for #gamma", 100, 0., 30. );
   ptisoatecal03phot = new TH1D("ptisoatecal03phot","trk isolation at ecal R=0.3 for #gamma", 100, 0., 30. );;
   ntrkiso03phot = new TH1D("ntrkiso03phot","# trk isolation R=0.3 for #gamma", 10, 0., 10. );
   ptisovsatecal03relphot = new TH2D("ptisovsatecal03relphot","trk isolation normalized R=0.3 for #gamma", 400, 0., 1. , 400, 0., 1. );
   ntrkisoatecal03phot = new TH1D("ntrkisoatecal03phot","# trk isolation at ecal R=0.3 for #gamma", 10, 0., 10. );

   ptiso02relphot = new TH1D("ptiso02relphot","trk isolation normalized R=0.2 for #gamma", 1000, 0., 1. );
   ptiso025relphot = new TH1D("ptiso025relphot","trk isolation normalized R=0.25 for #gamma", 1000, 0., 1. );
   ptiso03relphot = new TH1D("ptiso03relphot","trk isolation normalized R=0.3 for #gamma", 1000, 0., 1. );
   ptiso035relphot = new TH1D("ptiso035relphot","trk isolation normalized R=0.35 for #gamma", 1000, 0., 1. );
   ptiso04relphot = new TH1D("ptiso04relphot","trk isolation normalized R=0.4 for #gamma", 1000, 0., 1. );
   ptisoatecal02relphot = new TH1D("ptisoatecal02relphot","trk isolation normalized R=0.2 for #gamma", 1000, 0., 1. );
   ptisoatecal025relphot = new TH1D("ptisoatecal025relphot","trk isolation normalized R=0.25 for #gamma", 1000, 0., 1. );
   ptisoatecal03relphot = new TH1D("ptisoatecal03relphot","trk isolation normalized R=0.3 for #gamma", 1000, 0., 1. );
   ptisoatecal035relphot = new TH1D("ptisoatecal035relphot","trk isolation normalized R=0.35 for #gamma", 1000, 0., 1. );
   ptisoatecal04relphot = new TH1D("ptisoatecal04relphot","trk isolation normalized R=01. for #gamma", 1000, 0., 1. );

   hcalovecal01phot = new TH1D("hcalovecal01phot","HCAL/ECAL isolation R=0.1 for #gamma", 1000, -.3, .5 );
   hcalovecal02phot = new TH1D("hcalovecal02phot","HCAL/ECAL isolation R=0.2 for #gamma", 1000, -.3, .5 );
   hcalovecal025phot = new TH1D("hcalovecal025phot","HCAL/ECAL isolation R=0.25 for #gamma", 1000, -.3, .5 );
   hcalovecal03phot = new TH1D("hcalovecal03phot","HCAL/ECAL isolation R=0.3 for #gamma", 1000, -.3, .5 );
   hcalovecal04phot = new TH1D("hcalovecal04phot","HCAL/ECAL isolation R=0.4 for #gamma", 1000, -.3, .5 );
   hcalovecal05phot = new TH1D("hcalovecal05phot","HCAL/ECAL isolation R=0.5 for #gamma", 1000, -.3, .5 );

   ecaliso04phot = new TH1D("ecaliso04phot","ECAL isol R=0.4 for #gamma", 200, 0., 50. );

   ecaliso015relphot = new TH1D("ecaliso015relphot","ECAL isol relative R=0.15 for #gamma", 1000, 0., .5 );
   ecaliso02relphot = new TH1D("ecaliso02relphot","ECAL isol relative R=0.2 for #gamma", 1000, 0., .5 );
   ecaliso025relphot = new TH1D("ecaliso025relphot","ECAL isol relative R=0.25 for #gamma", 1000, 0., .5 );
   ecaliso03relphot = new TH1D("ecaliso03relphot","ECAL isol relative R=0.3 for #gamma", 1000, 0., .5 );
   ecaliso035relphot = new TH1D("ecaliso035relphot","ECAL isol relative R=0.35 for #gamma", 1000, 0., .5 );
   ecaliso04relphot = new TH1D("ecaliso04relphot","ECAL isol relative R=0.4 for #gamma", 1000, 0., .5 );

   ecaliso02recrelphot = new TH1D("ecaliso02recrelphot","ECAL isol relative R=0.2 for #gamma", 1000, 0., .5 );
   ecaliso03recrelphot = new TH1D("ecaliso03recrelphot","ECAL isol relative R=0.3 for #gamma", 1000, 0., .5 );
   ecaliso035recrelphot = new TH1D("ecaliso035recrelphot","ECAL isol relative R=0.35 for #gamma", 1000, 0., .5 );
   ecaliso04recrelphot = new TH1D("ecaliso04recrelphot","ECAL isol relative R=0.4 for #gamma", 1000, 0., .5 );
   ecaliso05recrelphot = new TH1D("ecaliso05recrelphot","ECAL isol relative R=0.5 for #gamma", 1000, 0., .5 );

   nnphot = new TH1D("nnphot","NN output for #gamma", 1000, -1., 1. );
   nnphot_old = new TH1D("nnphot_old","NN output for #gamma", 1000, -1., 1. );

   sMajMajbkg = new TH1D("sMajMajbkg","2^{nd} Maj Moment for bkg",1000,0.,2.);
   sMinMinbkg = new TH1D("sMinMinbkg","2^{nd} Min Moment for bkg",1000,0.,2.);
   sMinMinsMajMajbkg = new TH2D("sMinMinMajMajbkg","2^{nd} Min vs Maj Moment for bkg",200,0.,2.,200,0.,2.);
   Latbkg = new TH1D("Latbkg","LAT for #gamma",200,0,2.);
   fisherbkg = new TH1D("fisherbkg","Fisher for bkg",200,-2.,0.);
   ptiso03bkg = new TH1D("ptiso03bkg","trk isolation R=0.3 for bkg", 100, 0., 30. );
   ntrkiso03bkg = new TH1D("ntrkiso03bkg","# trk isolation R=0.3 for bkg", 10, 0., 10. );
   ptisoatecal03bkg = new TH1D("ptisoatecal03bkg","trk isolation at ecal R=0.3 for bkg", 100, 0., 30. );;
   ptisovsatecal03relbkg = new TH2D("ptisovsatecal03relbkg","trk isolation normalized R=0.3 for bkg", 400, 0., 1. , 400, 0., 1. );
   ntrkisoatecal03bkg = new TH1D("ntrkisoatecal03bkg","# trk isolation at ecal R=0.3 for bkg", 10, 0., 10. );
   ecaliso04bkg = new TH1D("ecaliso04bkg","ECAL isol R=0.4 for bkg", 200, 0., 50. );

   ptiso02relbkg = new TH1D("ptiso02relbkg","trk isolation normalized R=0.2 for bkg", 1000, 0., 1. );
   ptiso025relbkg = new TH1D("ptiso025relbkg","trk isolation normalized R=0.25 for bkg", 1000, 0., 1. );
   ptiso03relbkg = new TH1D("ptiso03relbkg","trk isolation normalized R=0.3 for bkg", 1000, 0., 1. );
   ptiso035relbkg = new TH1D("ptiso035relbkg","trk isolation normalized R=0.35 for bkg", 1000, 0., 1. );
   ptiso04relbkg = new TH1D("ptiso04relbkg","trk isolation normalized R=0.4 for bkg", 1000, 0., 1. );
   ptisoatecal02relbkg = new TH1D("ptisoatecal02relbkg","trk isolation normalized R=0.2 for bkg", 1000, 0., 1. );
   ptisoatecal025relbkg = new TH1D("ptisoatecal025relbkg","trk isolation normalized R=0.25 for bkg", 1000, 0., 1. );
   ptisoatecal03relbkg = new TH1D("ptisoatecal03relbkg","trk isolation normalized R=0.3 for bkg", 1000, 0., 1. );
   ptisoatecal035relbkg = new TH1D("ptisoatecal035relbkg","trk isolation normalized R=0.35 for bkg", 1000, 0., 1. );
   ptisoatecal04relbkg = new TH1D("ptisoatecal04relbkg","trk isolation normalized R=0.4 for bkg", 1000, 0., 1. );

   hcalovecal01bkg = new TH1D("hcalovecal01bkg","HCAL/ECAL isolation R=0.1 for bkg", 1000, -.3, .5 );
   hcalovecal02bkg = new TH1D("hcalovecal02bkg","HCAL/ECAL isolation R=0.2 for bkg", 1000, -.3, .5 );
   hcalovecal025bkg = new TH1D("hcalovecal025bkg","HCAL/ECAL isolation R=0.25 for bkg", 1000, -.3, .5 );
   hcalovecal03bkg = new TH1D("hcalovecal03bkg","HCAL/ECAL isolation R=0.3 for bkg", 1000, -.3, .5 );
   hcalovecal04bkg = new TH1D("hcalovecal04bkg","HCAL/ECAL isolation R=0.4 for bkg", 1000, -.3, .5 );
   hcalovecal05bkg = new TH1D("hcalovecal05bkg","HCAL/ECAL isolation R=0.5 for bkg", 1000, -.3, .5 );

   ecaliso015relbkg = new TH1D("ecaliso015relbkg","ECAL isol relative R=0.25 for bkg", 1000, 0., .5 );
   ecaliso02relbkg = new TH1D("ecaliso02relbkg","ECAL isol relative R=0.25 for bkg", 1000, 0., .5 );
   ecaliso025relbkg = new TH1D("ecaliso025relbkg","ECAL isol relative R=0.25 for bkg", 1000, 0., .5 );
   ecaliso03relbkg = new TH1D("ecaliso03relbkg","ECAL isol relative R=0.3 for bkg", 1000, 0., .5 );
   ecaliso035relbkg = new TH1D("ecaliso035relbkg","ECAL isol relative R=0.35 for bkg", 1000, 0., .5 );
   ecaliso04relbkg = new TH1D("ecaliso04relbkg","ECAL isol relative R=0.4 for bkg", 1000, 0., .5 );

   ecaliso02recrelbkg = new TH1D("ecaliso02recrelbkg","ECAL isol relative R=0.2 for bkg", 1000, 0., .5 );
   ecaliso03recrelbkg = new TH1D("ecaliso03recrelbkg","ECAL isol relative R=0.3 for bkg", 1000, 0., .5 );
   ecaliso035recrelbkg = new TH1D("ecaliso035recrelbkg","ECAL isol relative R=0.35 for bkg", 1000, 0., .5 );
   ecaliso04recrelbkg = new TH1D("ecaliso04recrelbkg","ECAL isol relative R=0.4 for bkg", 1000, 0., .5 );
   ecaliso05recrelbkg = new TH1D("ecaliso05recrelbkg","ECAL isol relative R=0.5 for bkg", 1000, 0., .5 );

   nnbkg = new TH1D("nnbkg","NN output for bkg", 1000, -1., 1. );
   nnbkg_old = new TH1D("nnbkg_old","NN output for bkg", 1000, -1., 1. );

   etaphot_all =  new TH1D("etaphot_all","#eta(#gamma) after pt cuts and loose iso", 30, -1.47, 1.47 );
   etajet_all =  new TH1D("etajet_all","#eta(jet) after pt cuts, loose iso", 30, -7, 7 );
   deltaphi_all =  new TH1D("deltaphi_all","#Delta(#phi) after pt, eta cuts and loose iso", 100, -2, 2. );
   njetoverth_all =  new TH1D("njetoverth_all","Num of jets after pt, eta cuts and loose iso", 10, 0, 10. );
   ptjet2ndovptphot_all =  new TH1D("ptjet2ndovptphot_all","pt of the second jet over pt phot after pt, eta cuts and loose iso", 300, 0, .8 );
   ptjet2_all =  new TH1D("ptjet2_all","pt of the second jet, eta cuts and loose iso", 300, 0., 200. );

}


TChain * getchain(char *thechain) {

  TChain *chain = new TChain("pippo");
  std::cout << "Chaining ... " << thechain << std::endl;
  char pName[2000];
  char buffer[200];
  sprintf(buffer, "%s", thechain);
  ifstream is(buffer);
  cout << "files " << buffer <<  endl;
  while(is.getline(buffer, 200, '\n')){
    //    if (buffer[0] == '#') continue;
    sscanf(buffer, "%s", pName);
    std::cout << "   Add: " << buffer << std::endl;
    chain->Add(pName);
  }
  is.close();
  return chain;

}

