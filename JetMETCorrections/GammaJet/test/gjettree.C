#define gjettree_cxx
#include "gjettree.h"
#include <TF1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <JetMETCorrections/GammaJet/test/TMVAPhotons_MLP.class.C>
#include <JetMETCorrections/GammaJet/test/TMVAPhotons_1_MLP.class.C>
#include <JetMETCorrections/GammaJet/test/TMVAPhotons_2_MLP.class.C>
#include <JetMETCorrections/GammaJet/test/TMVAPhotons_3_MLP.class.C>
#include <JetMETCorrections/GammaJet/test/TMVAPhotons_4_MLP.class.C>
void gjettree::Loop(double cross, int algo, int NEVT)
{
//   In a ROOT session, you can do:
//      Root > .L gjettree.C
//      Root > gjettree t
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

  double w = cross / nentries * npb;

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
    int ileadmcphot = -1;
    int ileadmcquark = -1;
    
    isphoton = issignal = -999;
    weight = nniso = nniso_int =  ptph = ptj = etaph = etaj = phiph = phij = pt2jet = ptphottrue = ptjettrue = ptquarktrue = phiphottrue = phijettrue = phiquarktrue = -999.;

    // FINDING THE LEADING RECONSTRUCTED PHOTON
    
    for(int i=0; i<nPhot; i++){
            
      if(TMath::Abs(etaPhot[i])<1.3) {
		
	if(ePhot[i]/cosh(etaPhot[i])>maxptphot) {
	  maxptphot = ePhot[i]/cosh(etaPhot[i]);
	  imaxptphot = i;		
	}

      } 
      allptphot->Fill(ePhot[i]/cosh(etaPhot[i]));
      
    }

    // LOOP OVER JETS - FINDING THE RECOILING RECO JET

    // picking the selected jet algo
    if(algo == 1) {
      nJet = nJet_ite;
      for(int j=0; j<nJet_ite; j++){
	pxJet [j]=pxJet_ite [j];
	pyJet [j]=pyJet_ite [j];
	pzJet [j]=pzJet_ite [j];
	eJet  [j]=eJet_ite  [j];
	etaJet[j]=etaJet_ite[j];
      	phiJet[j]=phiJet_ite[j];
      }
    }else if(algo == 2) {
      nJet = nJet_kt;
      for(int j=0; j<nJet_kt; j++){
	pxJet [j]=pxJet_kt [j];
	pyJet [j]=pyJet_kt [j];
	pzJet [j]=pzJet_kt [j];
	eJet  [j]=eJet_kt  [j];
	etaJet[j]=etaJet_kt[j];
      	phiJet[j]=phiJet_kt[j];
      }
    }else if(algo == 3) {
      nJet = nJet_sis;
      for(int j=0; j<nJet_sis; j++){
	pxJet [j]=pxJet_sis [j];
	pyJet [j]=pyJet_sis [j];
	pzJet [j]=pzJet_sis [j];
	eJet  [j]=eJet_sis  [j];
	etaJet[j]=etaJet_sis[j];
      	phiJet[j]=phiJet_sis[j];
      }
    }else cout << "NO SUCH JET ALGO!!!!" << endl;
           
    double mindeltaphi = 2*pi;
    int imindeltaphi = -1;
    int njetsoverthreshold = 0;
    
    for(int j=0; j<nJet; j++){
      
      allptjet->Fill(eJet[j]/cosh(etaJet[j]));
      
      if(eJet[j]/cosh(etaJet[j])>10.){
	
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
//     (*inputVec)[5] = ecaliso03/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot]));
    (*inputVec)[5] = (ecaliso04Phot[imaxptphot]-ePhot[imaxptphot])/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot]));

    double nnval_int = classReader->GetMvaValue( *inputVec );
    double nnval = 0;
    if(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])<60)                                                     nnval = classReader1->GetMvaValue( *inputVec );
    if(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])<100 && ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])>60)  nnval = classReader2->GetMvaValue( *inputVec );
    if(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])<200 && ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])>100) nnval = classReader3->GetMvaValue( *inputVec );
    if(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])>200)                                                    nnval = classReader4->GetMvaValue( *inputVec );
    
    if(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])>25 && TMath::Abs(etaPhot[imaxptphot])<1.3) {
      if(matched){
	pts = ePhot[imaxptphot]/cosh(etaPhot[imaxptphot]);
	ntrkisos = ntrkiso035Phot[imaxptphot];
	ptisos = ptiso035Phot[imaxptphot]/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot]));
	ptisoatecals = ptisoatecal035Phot[imaxptphot]/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot]));
	hcalovecals = hcalovecal04Phot[imaxptphot];
	sMajMajs = sMajMajPhot[imaxptphot];
	sMinMins = sMinMinPhot[imaxptphot];
	ecalisos = (ecaliso04Phot[imaxptphot]-ePhot[imaxptphot])/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot]));
	//	S_tree->Fill();
      } else {
	ptb = ePhot[imaxptphot]/cosh(etaPhot[imaxptphot]);
	ntrkisob = ntrkiso035Phot[imaxptphot];
	ptisob = ptiso035Phot[imaxptphot]/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot]));
	ptisoatecalb = ptisoatecal035Phot[imaxptphot]/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot]));
	hcalovecalb = hcalovecal04Phot[imaxptphot];
	sMajMajb = sMajMajPhot[imaxptphot];
	sMinMinb = sMinMinPhot[imaxptphot];
	ecalisob = (ecaliso04Phot[imaxptphot]-ePhot[imaxptphot])/(ePhot[imaxptphot]/cosh(etaPhot[imaxptphot]));
	//	B_tree->Fill();	   
      }
      nniso_int = nnval_int;
      nniso = nnval;
    }

    // variable calculation

    double ptphot = ePhot[imaxptphot]/cosh(etaPhot[imaxptphot]);
    double ptphotgen =  eMC[ileadmcphot]/cosh(etaMC[ileadmcphot]);
    double ptjet = eJet[imindeltaphi]/cosh(etaJet[imindeltaphi]);
    double ptjetgen = eJetGen[imindeltaphigen]/cosh(etaJetGen[imindeltaphigen]);
    double ptjetgenquark = eMC[ileadmcquark]/cosh(etaMC[ileadmcquark]);

    // cuts

    bool etacut =  TMath::Abs(etaPhot[imaxptphot]) < 1.3; 
    bool ptcut = ePhot[imaxptphot]/cosh(etaPhot[imaxptphot])>25;
    
    if(!etacut) continue;
    if(!ptcut) continue;    
        
    isphoton = matched;
    issignal = ptphotgen>25 && ileadmcphot>-.5;
    weight = w;
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
    ana_tree->Fill();

  }

  delete inputVec;
}


void gjettree::BookHistos()
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
   ana_tree->Branch("nniso_int",&nniso_int,"nniso_int/F");
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
   allptjet = new TH1D("allptjet","allptjet",100,0.,700.);
   alldeltaphi = new TH1D("alldeltaphi","alldeltaphi",200,-2*pi,2*pi);
   alldeltar = new TH1D("alldeltar","alldeltar",200,0.,1.);
   alldeltae = new TH1D("alldeltae","alldeltar",200,-.1,0.1);
   alldeltardeltae = new TH2D("alldeltardeltae","alldeltardeltae",200,0.,1.,200,-.1,0.1);

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
    if (buffer[0] == '#') {
      std::cout << "   Skip: " << buffer << std::endl;
      continue;
    }
    sscanf(buffer, "%s", pName);
    std::cout << "   Add: " << buffer << std::endl;
    chain->Add(pName);
  }
  is.close();
  return chain;

}

