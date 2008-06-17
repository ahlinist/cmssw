#define OverlapHistos_cxx
#include "TrackingAnalysis/Cosmics/test/OverlapHistos.h"
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TF1.h>
#include <TProfile.h>

#include <iostream>

void OverlapHistos::Loop(){
  const char charAB[2] = { 'A', 'B' };
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();

  TDirectory* curDir = gDirectory;
  TDirectory* hDir = gROOT->mkdir("ResidualHistograms");
  TDirectory* resDir = hDir->mkdir("ResHistos");
  TDirectory* predDir = hDir->mkdir("PredHistos");
  TDirectory* hitDir = hDir->mkdir("HitHistos");
  TDirectory* posDir = hDir->mkdir("PosHistos");
  TDirectory* pullDir = hDir->mkdir("PullHistos");
  curDir->cd();
  //
  // containers of ids and histograms for all overlaps 
  //   found in the tree (in order / reverse order of the ids)
  //
  std::map<DetIdPair,unsigned int> allIndices;
  std::vector<TH1*> allResHistos[2];        // double difference
  std::vector<TH1*> allResHistosPlusY[2];    // double diff local y > 0
  std::vector<TH1*> allResHistosMinusY[2];   // double diff local y < 0
  std::vector<TH1*> allResHistosPlusX[2];    // double diff local x > 0
  std::vector<TH1*> allResHistosMinusX[2];   // double diff local x < 0
  std::vector<TH1*> allPredErrHistos[2];    // error on predicted difference
  std::vector<TH1*> allPredErrHistosFirst[2]; // error on predicted position for first overlap module
  std::vector<TH1*> allPredErrHistosSecond[2]; // error on predicted position for second overlap module
  std::vector<TH1*> allHitErrHistos[2];     // error in hit difference
  std::vector<TH1*> allHitErrHistosFirst[2]; // error on hit position for first overlap module
  std::vector<TH1*> allHitErrHistosSecond[2]; // error on hit position for second overlap module
  std::vector<TH1*> allSimRecHistos[2];     // SimHit-RecHit position
  std::vector<TH1*> allSimTrkHistos[2];     // SimHit-TrackHit position
  std::vector<TH1*> allDxdzHistos[2];       // local dxdz
  std::vector<TH1*> allRadHistos[2];        // radius (global position)
  std::vector<TH1*> allPhiHistos[2];        // phi (global position)
  std::vector<TH1*> allZHistos[2];          // z (global position)
  std::vector<TH1*> allXHistos[2];        // x (local position)
  std::vector<TH1*> allYHistos[2];          // y (local position)
  std::vector<TH1*> allDoublePullHistos[2];
  std::vector<TH1*> allHitPullHistos[2];
  std::vector<TH1*> allPredPullHistos[2];
#ifdef HISTOS2D
  std::vector<TH2*> allResVsAngleHistos[2];  // resolution vs angle
  std::vector<TH2*> allddVsLocalXHistos[2]; //double diff vs local x interesting for TOB layers
  std::vector<TH2*> allddVsLocalYHistos[2]; //double diff vs local y interesting for TIB layers
  std::vector<TH2*> allddVsDxdzHistos[2]; //double diff vs dx/dz
  std::vector<TH2*> allddVsDydzHistos[2]; //double diff vs dy/dz
  std::vector<TH2*> allLocalXVsLocalYHistos[2]; //local x vs local y
  std::vector<TH2*> allDxdzVsDydzHistos[2]; //dx/dz vs dy/dz
  std::vector<TH2*> allLocalXVsDxdzHistos[2]; //local X vs dx/dz
  std::vector<TH2*> allLocalYVsDxdzHistos[2]; //local Y vs dx/dz
  std::vector<TH2*> allLocalXVsDydzHistos[2]; //local X vs dy/dz
  std::vector<TH2*> allLocalYVsDydzHistos[2]; //local Y vs dy/dz
  std::vector<TH2*> alldPreddSimVsdHitdSimHistos[2]; //dPred-dSim vs dHit-dSim  
#endif
  TH2F* widthVsAngle = new TH2F("widthVsAngle", "width vs angle", 75, -0.75, 0.75, 400, -0.2, 0.2);
  widthVsAngle->GetXaxis()->SetTitle("dx/dz");
  widthVsAngle->GetYaxis()->SetTitle("#Deltax_{hit}-#Deltax_{pred}");
  TH1F* simRecHistoAll = new TH1F("simRecAll","simRec all",400,-2.0,2.0);
  TH1F* simRecMomentum = new TH1F("simRecMom","simRec momentum",100,0,20.0);
  TH2F* widthVsAngle_TOB1 = new TH2F("widthVsAngleTOB1", "width vs angle TOB1", 75, -0.75, 0.75, 400, -0.2, 0.2);
  TH2F* widthVsAngle_TOB2 = new TH2F("widthVsAngleTOB2", "width vs angle TOB2", 75, -0.75, 0.75, 400, -0.2, 0.2);
  TH2F* widthVsAngle_TOB3 = new TH2F("widthVsAngleTOB3", "width vs angle TOB3", 75, -0.75, 0.75, 400, -0.2, 0.2);
  TH2F* widthVsAngle_TOB4 = new TH2F("widthVsAngleTOB4", "width vs angle TOB4", 75, -0.75, 0.75, 400, -0.2, 0.2);

  TH2F* width2VsAngle_TOB1 = new TH2F("width2VsAngleTOB1", "width2 vs angle TOB1", 75, -0.75, 0.75, 400, -0.2, 0.2);
  TH2F* width2VsAngle_TOB2 = new TH2F("width2VsAngleTOB2", "width2 vs angle TOB2", 75, -0.75, 0.75, 400, -0.2, 0.2);
  TH2F* width2VsAngle_TOB3 = new TH2F("width2VsAngleTOB3", "width2 vs angle TOB3", 75, -0.75, 0.75, 400, -0.2, 0.2);
  TH2F* width2VsAngle_TOB4 = new TH2F("width2VsAngleTOB4", "width2 vs angle TOB4", 75, -0.75, 0.75, 400, -0.2, 0.2);
  TH2F* predErr2VsAngle_TOB1 = new TH2F("predErr2VsAngleTOB1", "predErr2 vs angle TOB1", 75, -0.75, 0.75, 400, -0.2, 0.2);
  TH2F* predErr2VsAngle_TOB2 = new TH2F("predErr2VsAngleTOB2", "predErr2 vs angle TOB2", 75, -0.75, 0.75, 400, -0.2, 0.2);
  TH2F* predErr2VsAngle_TOB3 = new TH2F("predErr2VsAngleTOB3", "predErr2 vs angle TOB3", 75, -0.75, 0.75, 400, -0.2, 0.2);
  TH2F* predErr2VsAngle_TOB4 = new TH2F("predErr2VsAngleTOB4", "predErr2 vs angle TOB4", 75, -0.75, 0.75, 400, -0.2, 0.2);
  TH2F* hitErr2VsAngle_TOB1 = new TH2F("hitErr2VsAngleTOB1", "hitErr2 vs angle TOB1", 75, -0.75, 0.75, 400, -0.2, 0.2);
  TH2F* hitErr2VsAngle_TOB2 = new TH2F("hitErr2VsAngleTOB2", "hitErr2 vs angle TOB2", 75, -0.75, 0.75, 400, -0.2, 0.2);
  TH2F* hitErr2VsAngle_TOB3 = new TH2F("hitErr2VsAngleTOB3", "hitErr2 vs angle TOB3", 75, -0.75, 0.75, 400, -0.2, 0.2);
  TH2F* hitErr2VsAngle_TOB4 = new TH2F("hitErr2VsAngleTOB4", "hitErr2 vs angle TOB4", 75, -0.75, 0.75, 400, -0.2, 0.2);
  TH2F* alphaVsAngle_TOB1 = new TH2F("alphaVsAngleTOB1", "alpha vs angle TOB1", 75, -0.75, 0.75, 400, -0.2, 0.2);
  TH2F* alphaVsAngle_TOB2 = new TH2F("alphaVsAngleTOB2", "alpha vs angle TOB2", 75, -0.75, 0.75, 400, -0.2, 0.2);
  TH2F* alphaVsAngle_TOB3 = new TH2F("alphaVsAngleTOB3", "alpha vs angle TOB3", 75, -0.75, 0.75, 400, -0.2, 0.2);
  TH2F* alphaVsAngle_TOB4 = new TH2F("alphaVsAngleTOB4", "alpha vs angle TOB4", 75, -0.75, 0.75, 400, -0.2, 0.2);
  TH1F* deltaXPred = new TH1F("deltaXPred", "delta x predicted", 220, -1.1, 1.1);
  TH1F* doubleDiff = new TH1F("doubleDiff", "double difference", 220, -1.1, 1.1);

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    // cut on 30deg local angle in x (any of the two modules)
    //if ( fabs(predDX[0])>0.6 || fabs(predDX[1])>0.6 )  continue;
    // extrapolation accuracy
        if ( predEDeltaX<1.e-9 || predEDeltaX>0.005 )  continue;
    // chi2 cut
    //if ( TMath::Prob(chi2,2*found-4)<1.e-3 )  continue;
    if ( chi2 >30 ) continue; 
    //
    widthVsAngle->Fill(predDX[0],(hitX[0] + relSignX*hitX[1]) - (predX[0] + relSignX*predX[1]));
   //(dx/dz, deltaHit-deltaPred)
    deltaXPred->Fill(predX[0] + relSignX*predX[1]);
    doubleDiff->Fill((hitX[0] + relSignX*hitX[1]) - (predX[0] + relSignX*predX[1]));
  
    unsigned int subdet;
    unsigned int layer;
    unsigned int stereo;
    unsigned int id = detids[0];
    decode(id,subdet,layer,stereo);
    // sequential layer number in TIB & TOB (for debugging)
    int k(-1);
    if ( subdet==3 && layer>0 && layer<5 ) {
      k = layer;
    }
    else if ( subdet==5 && layer>0 && layer<7 ) {
      k = layer + 4;
    }
    
    if(k==5)  widthVsAngle_TOB1->Fill(predDX[0],(hitX[0] + relSignX*hitX[1]) - (predX[0] + relSignX*predX[1]));
    
    if(k==6)  widthVsAngle_TOB2->Fill(predDX[0],(hitX[0] + relSignX*hitX[1]) - (predX[0] + relSignX*predX[1]));
    
    if(k==7)  widthVsAngle_TOB3->Fill(predDX[0],(hitX[0] + relSignX*hitX[1]) - (predX[0] + relSignX*predX[1]));
    
    if(k==8)  widthVsAngle_TOB4->Fill(predDX[0],(hitX[0] + relSignX*hitX[1]) - (predX[0] + relSignX*predX[1]));
    

    // (ordered) DetId pair
    DetIdPair idPair(min(detids[0],detids[1]),
		     max(detids[0],detids[1]));
    // histogram pointers for this pair
    TH1* resHisto(0);
    TH1* resHistoPlusX(0);
    TH1* resHistoMinusX(0);
    TH1* resHistoPlusY(0);
    TH1* resHistoMinusY(0);
    TH1* predErrHisto(0);
    TH1* predErrHistoFirst(0);
    TH1* predErrHistoSecond(0);
    TH1* hitErrHisto(0);
    TH1* hitErrHistoFirst(0);
    TH1* hitErrHistoSecond(0);
    TH1* simRecHisto(0);
    TH1* simTrkHisto(0);
    TH1* dxdzHisto(0);
    TH1* posHistos[3];
    TH1* posHistosLocal[2];
    TH1* doublePullHisto(0);
    TH1* hitPullHisto(0);
    TH1* predPullHisto(0);
#ifdef HISTOS2D
    TH2* resVsAngle(0);
    TH2* ddVsLocalX(0);
    TH2* ddVsLocalY(0);
    TH2* ddVsDxdz(0);
    TH2* ddVsDydz(0);
    TH2* localXVsLocalY(0);
    TH2* dxdzVsDydz(0);
    TH2* localXVsDxdz(0);
    TH2* localYVsDxdz(0);
    TH2* localXVsDydz(0);
    TH2* localYVsDydz(0);
    TH2* dPreddSimVsdHitdSim(0);
#endif
    unsigned int ind(0);
    std::map<DetIdPair,unsigned int>::iterator it = allIndices.find(idPair);
    if ( it==allIndices.end() ) {
      //
      // new pair: create histograms
      //
      ind = allResHistos[0].size();
      allIndices[idPair] = ind;
      char hn[32];
      hDir->cd();
      
      // create histograms for reverse / normal order of DetIds
      for ( int j=0; j<2; ++j ) {
	resDir->cd();
	sprintf(hn,"%c%d",charAB[j],ind);
	resHisto = new TH1F(hn,hn,400,-0.2,0.2);
	allResHistos[j].push_back(resHisto);
	sprintf(hn,"rhplusX%c%d",charAB[j],ind);
	resHistoPlusX = new TH1F(hn,hn,400,-0.2,0.2);
	allResHistosPlusX[j].push_back(resHistoPlusX);
	sprintf(hn,"rhminusX%c%d",charAB[j],ind);
	resHistoMinusX = new TH1F(hn,hn,400,-0.2,0.2);
	allResHistosMinusX[j].push_back(resHistoMinusX);
	sprintf(hn,"rhplusY%c%d",charAB[j],ind);
	resHistoPlusY = new TH1F(hn,hn,400,-0.2,0.2);
	allResHistosPlusY[j].push_back(resHistoPlusY);
	sprintf(hn,"rhminusY%c%d",charAB[j],ind);
	resHistoMinusY = new TH1F(hn,hn,400,-0.2,0.2);
	allResHistosMinusY[j].push_back(resHistoMinusY);
	predDir->cd();
	sprintf(hn,"pSig%c%d",charAB[j],ind);
	predErrHisto = new TH1F(hn,hn,100,0,0.01);
	allPredErrHistos[j].push_back(predErrHisto);
	sprintf(hn,"pSigFirst%c%d",charAB[j],ind);
	predErrHistoFirst = new TH1F(hn,hn,100,0,0.01);
	allPredErrHistosFirst[j].push_back(predErrHistoFirst);
	sprintf(hn,"pSigSecond%c%d",charAB[j],ind);
	predErrHistoSecond = new TH1F(hn,hn,100,0,0.01);
	allPredErrHistosSecond[j].push_back(predErrHistoSecond);
	hitDir->cd();
	sprintf(hn,"hSig%c%d",charAB[j],ind);
	hitErrHisto = new TH1F(hn,hn,100,0,0.05);
	allHitErrHistos[j].push_back(hitErrHisto);
	sprintf(hn,"hSigFirst%c%d",charAB[j],ind);
	hitErrHistoFirst = new TH1F(hn,hn,100,0,0.01);
	allHitErrHistosFirst[j].push_back(hitErrHistoFirst);
	sprintf(hn,"hSigSecond%c%d",charAB[j],ind);
	hitErrHistoSecond = new TH1F(hn,hn,100,0,0.01);
	allHitErrHistosSecond[j].push_back(hitErrHistoSecond);
	hDir->cd();
	sprintf(hn,"simrec%c%d",charAB[j],ind);
	simRecHisto = new TH1F(hn,hn,400,-0.2,0.2);
	allSimRecHistos[j].push_back(simRecHisto);                        
	sprintf(hn,"simtrk%c%d",charAB[j],ind);
	simTrkHisto = new TH1F(hn,hn,400,-0.2,0.2);
	allSimTrkHistos[j].push_back(simTrkHisto);   
	sprintf(hn,"hDxdz%c%d",charAB[j],ind);
	dxdzHisto = new TH1F(hn,hn,100,-2.,2.);
	allDxdzHistos[j].push_back(dxdzHisto);
	posDir->cd();
	sprintf(hn,"rad%c%d",charAB[j],ind);
	posHistos[0] = new TH1F(hn,hn,110,0,110.);
	allRadHistos[j].push_back(posHistos[0]);
	sprintf(hn,"phi%c%d",charAB[j],ind);
	posHistos[1] = new TH1F(hn,hn,100,-TMath::Pi(),TMath::Pi());
	allPhiHistos[j].push_back(posHistos[1]);
	sprintf(hn,"z%c%d",charAB[j],ind);
	posHistos[2] = new TH1F(hn,hn,135,-270,270);
	allZHistos[j].push_back(posHistos[2]);
	sprintf(hn,"x%c%d",charAB[j],ind);
	posHistosLocal[0] = new TH1F(hn,hn,200,-10,10);
	allXHistos[j].push_back(posHistosLocal[0]);
	sprintf(hn,"y%c%d",charAB[j],ind);
	posHistosLocal[1] = new TH1F(hn,hn,200,-10,10);
	allYHistos[j].push_back(posHistosLocal[1]);
	pullDir->cd();
	sprintf(hn,"dp%c%d",charAB[j],ind);
	doublePullHisto = new TH1F(hn,hn,150,-15.,15.);
	allDoublePullHistos[j].push_back(doublePullHisto);
	sprintf(hn,"hp%c%d",charAB[j],ind);
	hitPullHisto = new TH1F(hn,hn,150,-15.,15.);
	allHitPullHistos[j].push_back(hitPullHisto);
	sprintf(hn,"pp%c%d",charAB[j],ind);
	predPullHisto = new TH1F(hn,hn,150,-15.,15.);
	allPredPullHistos[j].push_back(predPullHisto);
#ifdef HISTOS2D
	sprintf(hn,"resVsAngle%c%d",charAB[j],ind);
	resVsAngle = new TH2F(hn,hn, 75, -0.75, 0.75, 400, -0.2, 0.2);
	allResVsAngleHistos[j].push_back(resVsAngle);
	sprintf(hn,"ddVsLocalX%c%d",charAB[j],ind);
	ddVsLocalX = new TH2F(hn,hn, 500, -5, 5, 100, -0.1, 0.1);
	allddVsLocalXHistos[j].push_back(ddVsLocalX);
	sprintf(hn,"ddVsLocalY%c%d",charAB[j],ind);
	ddVsLocalY = new TH2F(hn,hn, 1000, -10, 10, 100, -0.1, 0.1);
	allddVsLocalYHistos[j].push_back(ddVsLocalY);
	sprintf(hn,"ddVsDxdz%c%d",charAB[j],ind);
	ddVsDxdz = new TH2F(hn,hn, 100, -1, 1, 100, -0.1, 0.1);
	allddVsDxdzHistos[j].push_back(ddVsDxdz);
	sprintf(hn,"ddVsDydz%c%d",charAB[j],ind);
	ddVsDydz = new TH2F(hn,hn, 100, -1, 1, 100, -0.1, 0.1);
	allddVsDydzHistos[j].push_back(ddVsDydz);
	sprintf(hn,"localXVsLocalY%c%d",charAB[j],ind);
	localXVsLocalY = new TH2F(hn,hn, 500, -5, 5, 1000, -10, 10);
	allLocalXVsLocalYHistos[j].push_back(localXVsLocalY);
	sprintf(hn,"dxdzVsDydz%c%d",charAB[j],ind);
	dxdzVsDydz = new TH2F(hn,hn, 100, -1, 1, 100, -1, 1);
	allDxdzVsDydzHistos[j].push_back(dxdzVsDydz);
	sprintf(hn,"localXVsDxdz%c%d",charAB[j],ind);
	localXVsDxdz = new TH2F(hn,hn, 100, -1, 1, 500, -5, 5);
	allLocalXVsDxdzHistos[j].push_back(localXVsDxdz);
	sprintf(hn,"localYVsDxdz%c%d",charAB[j],ind);
	localYVsDxdz = new TH2F(hn,hn, 100, -1, 1, 1000, -10, 10);
	allLocalYVsDxdzHistos[j].push_back(localYVsDxdz);
	sprintf(hn,"localXVsDydz%c%d",charAB[j],ind);
	localXVsDydz = new TH2F(hn,hn, 100, -1, 1, 500, -5, 5);
	allLocalXVsDydzHistos[j].push_back(localXVsDydz);
	sprintf(hn,"localYVsDydz%c%d",charAB[j],ind);
	localYVsDydz = new TH2F(hn,hn, 100, -1, 1, 1000, -10, 10);
	allLocalYVsDydzHistos[j].push_back(localYVsDydz);
	sprintf(hn,"dPreddSimVsdHitdSim%c%d",charAB[j],ind);
	dPreddSimVsdHitdSim = new TH2F(hn,hn, 40, -0.02, 0.02, 40, -0.02, 0.02);
	alldPreddSimVsdHitdSimHistos[j].push_back(dPreddSimVsdHitdSim);
#endif
      }
      curDir->cd();
    }
    else {
      //
      // histograms exist: choose index for normal / reverse order
      //
      ind = (*it).second;
    }
    // normal / reverse order?
    int j = idPair.first==detids[0] ? 0 : 1;
    // histogram pointers
    resHisto = allResHistos[j][ind];
    resHistoPlusX = allResHistosPlusX[j][ind];
    resHistoMinusX = allResHistosMinusX[j][ind];
    resHistoPlusY = allResHistosPlusY[j][ind];
    resHistoMinusY = allResHistosMinusY[j][ind];
    predErrHisto = allPredErrHistos[j][ind];
    predErrHistoFirst = allPredErrHistosFirst[j][ind];
    predErrHistoSecond = allPredErrHistosSecond[j][ind];
    hitErrHisto = allHitErrHistos[j][ind];
    hitErrHistoFirst = allHitErrHistosFirst[j][ind];
    hitErrHistoSecond = allHitErrHistosSecond[j][ind];
    simRecHisto = allSimRecHistos[j][ind];
    simTrkHisto = allSimTrkHistos[j][ind];
    dxdzHisto = allDxdzHistos[j][ind];
    posHistos[0] = allRadHistos[j][ind];
    posHistos[1] = allPhiHistos[j][ind];
    posHistos[2] = allZHistos[j][ind];
    posHistosLocal[0] = allXHistos[j][ind];
    posHistosLocal[1] = allYHistos[j][ind];
    doublePullHisto = allDoublePullHistos[j][ind];
    hitPullHisto = allHitPullHistos[j][ind];
    predPullHisto = allPredPullHistos[j][ind];
#ifdef HISTOS2D
    resVsAngle = allResVsAngleHistos[j][ind];
#endif
    // local x difference / sum for hits
    double deltaHit = hitX[0] + relSignX*hitX[1];
    // local x difference / sum for predicted states
    double deltaPred = predX[0] + relSignX*predX[1];
    double deltaSim = simX[0] + relSignX*simX[1];
#ifdef HISTOS2D
    ddVsLocalX = allddVsLocalXHistos[j][ind];
    ddVsLocalY = allddVsLocalYHistos[j][ind];
    ddVsDxdz = allddVsDxdzHistos[j][ind];
    ddVsDydz = allddVsDydzHistos[j][ind];
    localXVsLocalY = allLocalXVsLocalYHistos[j][ind];
    dxdzVsDydz = allDxdzVsDydzHistos[j][ind];
    localXVsDxdz = allLocalXVsDxdzHistos[j][ind];
    localYVsDxdz = allLocalYVsDxdzHistos[j][ind];
    localXVsDydz = allLocalXVsDydzHistos[j][ind];
    localYVsDydz = allLocalYVsDydzHistos[j][ind];
    dPreddSimVsdHitdSim = alldPreddSimVsdHitdSimHistos[j][ind];
#endif
    // fill histograms
    resHisto->Fill(deltaHit-deltaPred);
    if (predX[0]>0) resHistoPlusX->Fill(deltaHit-deltaPred);
    if (predX[0]<0) resHistoMinusX->Fill(deltaHit-deltaPred);
    if (predY[0]>0) resHistoPlusY->Fill(deltaHit-deltaPred);
    if (predY[0]<0) resHistoMinusY->Fill(deltaHit-deltaPred);
    //fold in spread in central value to the error here. (syst)
    //    double predEDeltaXSys = predHisto->getRMS();
    //    double predEDeltaXTot = sqrt(predEDeltaX*predEDeltaX+predEDeltaXSys*predEDeltaXSys);
    predErrHisto->Fill(predEDeltaX);
    predErrHistoFirst->Fill(predEX[0]);
    predErrHistoSecond->Fill(predEX[1]);
    hitErrHisto->Fill(sqrt(hitEX[0]*hitEX[0]+hitEX[1]*hitEX[1]));
    hitErrHistoFirst->Fill(hitEX[0]);
    hitErrHistoSecond->Fill(hitEX[1]);
    if(simX[0]>-90 && simX[1]>-90){
      //      if( fabs(deltaSim-deltaHit) < 0.04 ) {
      simRecHisto->Fill(deltaSim-deltaHit);
      simRecHistoAll->Fill(deltaSim-deltaHit);
      //simRecMomentum->Fill((deltaSim-deltaHit)/(0.0136*0.16*3));
      simRecMomentum->Fill((0.0136*0.16*3)/(deltaSim-deltaHit));
      simTrkHisto->Fill(deltaSim-deltaPred);
      //}
    }
    dxdzHisto->Fill(predDX[0]);
    posHistos[0]->Fill(sqrt(gX[0]*gX[0]+gY[0]*gY[0]));
    posHistos[1]->Fill(atan2(gY[0],gX[0]));
    posHistos[2]->Fill(gZ[0]);
    posHistosLocal[0]->Fill(predX[0]);
    //    cout << "predX, predY = " << predX[0] << "," << predY[0] << endl;
    posHistosLocal[1]->Fill(predY[0]);    
    doublePullHisto->Fill((deltaHit-deltaPred)/
			  sqrt(hitEX[0]*hitEX[0]+hitEX[1]*hitEX[1]+predEDeltaX*predEDeltaX));
    if(simX[0]>-90 && simX[1]>-90){
      hitPullHisto->Fill((deltaSim-deltaHit)/sqrt(hitEX[0]*hitEX[0]+hitEX[1]*hitEX[1]));
      predPullHisto->Fill((deltaSim-deltaPred)/predEDeltaX);
    }
#ifdef HISTOS2D
    resVsAngle->Fill(predDX[0],deltaHit-deltaPred);
    ddVsLocalX->Fill(predX[0],deltaHit-deltaPred);
    ddVsLocalY->Fill(predY[0],deltaHit-deltaPred);
    ddVsDxdz->Fill(predDX[0],deltaHit-deltaPred);
    ddVsDydz->Fill(predDY[0],deltaHit-deltaPred);
    localXVsLocalY->Fill(predX[0],predY[0]);
    dxdzVsDydz->Fill(predDX[0],predDY[0]);
    localXVsDxdz->Fill(predDX[0],predX[0]);
    localYVsDxdz->Fill(predDX[0],predY[0]);
    localXVsDydz->Fill(predDY[0],predX[0]);
    localYVsDydz->Fill(predDY[0],predY[0]);
    dPreddSimVsdHitdSim->Fill(deltaSim-deltaPred,deltaSim-deltaHit);
#endif
  }

  //
  // End of loop over the tree: prepare final containers (after cleaning)
  //
  detIdPairs_.reserve(allIndices.size());
  for ( int j=0; j<2; ++j ) {
    residualHistos_[j].reserve(allIndices.size());
    residualHistosPlusX_[j].reserve(allIndices.size());  
    residualHistosMinusX_[j].reserve(allIndices.size()); 
    residualHistosPlusY_[j].reserve(allIndices.size());  
    residualHistosMinusY_[j].reserve(allIndices.size()); 
    predErrHistos_[j].reserve(allIndices.size());
    predErrHistosFirst_[j].reserve(allIndices.size());
    predErrHistosSecond_[j].reserve(allIndices.size());
    hitErrHistos_[j].reserve(allIndices.size());
    hitErrHistosFirst_[j].reserve(allIndices.size());
    hitErrHistosSecond_[j].reserve(allIndices.size());
    simRecHistos_[j].reserve(allIndices.size());
    simTrkHistos_[j].reserve(allIndices.size());  
    dxdzHistos_[j].reserve(allIndices.size());
    radHistos_[j].reserve(allIndices.size());
    phiHistos_[j].reserve(allIndices.size());
    zHistos_[j].reserve(allIndices.size());
    doublePullHistos_[j].reserve(allIndices.size());
    hitPullHistos_[j].reserve(allIndices.size());
    predPullHistos_[j].reserve(allIndices.size());
#ifdef HISTOS2D
    resVsAngleHistos_[j].reserve(allIndices.size());
    ddVsLocalXHistos_[j].reserve(allIndices.size());
    ddVsLocalYHistos_[j].reserve(allIndices.size());
    ddVsDxdzHistos_[j].reserve(allIndices.size());
    ddVsDydzHistos_[j].reserve(allIndices.size());
    localXVsLocalYHistos_[j].reserve(allIndices.size());
    dxdzVsDydzHistos_[j].reserve(allIndices.size());    
    localXVsDxdzHistos_[j].reserve(allIndices.size());    
    localYVsDxdzHistos_[j].reserve(allIndices.size());    
    localXVsDydzHistos_[j].reserve(allIndices.size());    
    localYVsDydzHistos_[j].reserve(allIndices.size());    
    dPreddSimVsdHitdSimHistos_[j].reserve(allIndices.size());
#endif
  }
  //
  // loop over all pairs, check entries and create new index
  // 
  unsigned n(0);
  for ( std::map<DetIdPair,unsigned int>::const_iterator ih=allIndices.begin();
	ih!=allIndices.end(); ++ih ) {
    //
    // pointers to histograms
    //
    TH1* resHistos[2];
    TH1* resHistosPlusY[2];
    TH1* resHistosMinusY[2];
    TH1* resHistosPlusX[2];
    TH1* resHistosMinusX[2];
    TH1* predErrHistos[2];
    TH1* predErrHistosFirst[2];
    TH1* predErrHistosSecond[2];
    TH1* hitErrHistos[2];
    TH1* hitErrHistosFirst[2];
    TH1* hitErrHistosSecond[2];
    TH1* simRecHistos[2];
    TH1* simTrkHistos[2];
    TH1* dxdzHistos[2];
    TH1* posHistos[3][2];
    TH1* posHistosLocal[2][2];
    TH1* doublePullHistos[2];
    TH1* hitPullHistos[2];
    TH1* predPullHistos[2];
#ifdef HISTOS2D
    TH2* resVsAngleHistos[2];
    TH2* ddVsLocalXHistos[2];
    TH2* ddVsLocalYHistos[2];
    TH2* ddVsDxdzHistos[2];
    TH2* ddVsDydzHistos[2];
    TH2* localXVsLocalYHistos[2];
    TH2* dxdzVsDydzHistos[2];
    TH2* localXVsDxdzHistos[2];
    TH2* localYVsDxdzHistos[2];
    TH2* localXVsDydzHistos[2];
    TH2* localYVsDydzHistos[2];
    TH2* dPreddSimVsdHitdSimHistos[2];
#endif

    for ( int j=0; j<2; ++j ) {
      resHistos[j] = allResHistos[j][(*ih).second];
      resHistosPlusX[j] = allResHistosPlusX[j][(*ih).second];
      resHistosMinusX[j] = allResHistosMinusX[j][(*ih).second];
      resHistosPlusY[j] = allResHistosPlusY[j][(*ih).second];
      resHistosMinusY[j] = allResHistosMinusY[j][(*ih).second];
      predErrHistos[j] = allPredErrHistos[j][(*ih).second];
      predErrHistosFirst[j] = allPredErrHistosFirst[j][(*ih).second];
      predErrHistosSecond[j] = allPredErrHistosSecond[j][(*ih).second];
      hitErrHistos[j] = allHitErrHistos[j][(*ih).second];
      hitErrHistosFirst[j] = allHitErrHistosFirst[j][(*ih).second];
      hitErrHistosSecond[j] = allHitErrHistosSecond[j][(*ih).second];
      simRecHistos[j] = allSimRecHistos[j][(*ih).second];
      simTrkHistos[j] = allSimTrkHistos[j][(*ih).second];
      dxdzHistos[j] = allDxdzHistos[j][(*ih).second];
      posHistos[0][j] = allRadHistos[j][(*ih).second];
      posHistos[1][j] = allPhiHistos[j][(*ih).second];
      posHistos[2][j] = allZHistos[j][(*ih).second];
      posHistosLocal[0][j] = allXHistos[j][(*ih).second];
      posHistosLocal[1][j] = allYHistos[j][(*ih).second];
      doublePullHistos[j] = allDoublePullHistos[j][(*ih).second];
      hitPullHistos[j] = allHitPullHistos[j][(*ih).second];
      predPullHistos[j] = allPredPullHistos[j][(*ih).second];
#ifdef HISTOS2D
      resVsAngleHistos[j] = allResVsAngleHistos[j][(*ih).second];
      ddVsLocalXHistos[j] = allddVsLocalXHistos[j][(*ih).second];
      ddVsLocalYHistos[j] = allddVsLocalYHistos[j][(*ih).second];
      ddVsDxdzHistos[j] = allddVsDxdzHistos[j][(*ih).second];
      ddVsDydzHistos[j] = allddVsDydzHistos[j][(*ih).second];
      localXVsLocalYHistos[j] = allLocalXVsLocalYHistos[j][(*ih).second];
      dxdzVsDydzHistos[j] = allDxdzVsDydzHistos[j][(*ih).second];
      localXVsDxdzHistos[j] = allLocalXVsDxdzHistos[j][(*ih).second];
      localYVsDxdzHistos[j] = allLocalYVsDxdzHistos[j][(*ih).second];
      localXVsDydzHistos[j] = allLocalXVsDydzHistos[j][(*ih).second];
      localYVsDydzHistos[j] = allLocalYVsDydzHistos[j][(*ih).second];
      dPreddSimVsdHitdSimHistos[j] = alldPreddSimVsdHitdSimHistos[j][(*ih).second];
#endif
    }
    
    //
    // select on at least 100 entries in one of the two histograms
    //

    if ( resHistos[0]->Integral()>110 || resHistos[1]->Integral()>110 ) {
      //
      // copy to final containers and rename histograms with new index
      //   (for easier access in the browser)
      //
      detIdPairs_.push_back((*ih).first);
      char hn[32];
      for ( int j=0; j<2; ++j ) {
	sprintf(hn,"rh%c%3.3d",charAB[j],n);
	resHistos[j]->SetNameTitle(hn,hn);
	residualHistos_[j].push_back(resHistos[j]);
	sprintf(hn,"rplusXh%c%3.3d",charAB[j],n);
	resHistosPlusX[j]->SetNameTitle(hn,hn);
	residualHistosPlusX_[j].push_back(resHistosPlusX[j]);
	sprintf(hn,"rminusXh%c%3.3d",charAB[j],n);
	resHistosMinusX[j]->SetNameTitle(hn,hn);
	residualHistosMinusX_[j].push_back(resHistosMinusX[j]);
	sprintf(hn,"rplusYh%c%3.3d",charAB[j],n);
	resHistosPlusY[j]->SetNameTitle(hn,hn);
	residualHistosPlusY_[j].push_back(resHistosPlusY[j]);
	sprintf(hn,"rminusYh%c%3.3d",charAB[j],n);
	resHistosMinusY[j]->SetNameTitle(hn,hn);
	residualHistosMinusY_[j].push_back(resHistosMinusY[j]);
	sprintf(hn,"pSig2%c%3.3d",charAB[j],n);
	predErrHistos[j]->SetNameTitle(hn,hn);
	predErrHistos_[j].push_back(predErrHistos[j]);
	sprintf(hn,"pSigFirst2%c%3.3d",charAB[j],n);
	predErrHistosFirst[j]->SetNameTitle(hn,hn);
	predErrHistosFirst_[j].push_back(predErrHistosFirst[j]);
	sprintf(hn,"pSigSecond2%c%3.3d",charAB[j],n);
	predErrHistosSecond[j]->SetNameTitle(hn,hn);
	predErrHistosSecond_[j].push_back(predErrHistosSecond[j]);
	sprintf(hn,"hSig2%c%3.3d",charAB[j],n);
	hitErrHistos[j]->SetNameTitle(hn,hn);
	hitErrHistos_[j].push_back(hitErrHistos[j]);
	sprintf(hn,"hSigFirst2%c%3.3d",charAB[j],n);
	hitErrHistosFirst[j]->SetNameTitle(hn,hn);
	hitErrHistosFirst_[j].push_back(hitErrHistosFirst[j]);
	sprintf(hn,"hSigSecond2%c%3.3d",charAB[j],n);
	hitErrHistosSecond[j]->SetNameTitle(hn,hn);
	hitErrHistosSecond_[j].push_back(hitErrHistosSecond[j]);
	sprintf(hn,"hSimRec2%c%3.3d",charAB[j],n);
	simRecHistos[j]->SetNameTitle(hn,hn);
	simRecHistos_[j].push_back(simRecHistos[j]);
	sprintf(hn,"hSimTrk2%c%3.3d",charAB[j],n);
	simTrkHistos[j]->SetNameTitle(hn,hn);
	simTrkHistos_[j].push_back(simTrkHistos[j]);
	sprintf(hn,"hDxdz%c%3.3d",charAB[j],n);
	dxdzHistos[j]->SetNameTitle(hn,hn);
	dxdzHistos_[j].push_back(dxdzHistos[j]);
	sprintf(hn,"rad%c%3.3d",charAB[j],n);
	posHistos[0][j]->SetNameTitle(hn,hn);
	radHistos_[j].push_back(posHistos[0][j]);
	sprintf(hn,"phi%c%3.3d",charAB[j],n);
	posHistos[1][j]->SetNameTitle(hn,hn);
	phiHistos_[j].push_back(posHistos[1][j]);
	sprintf(hn,"z%c%3.3d",charAB[j],n);
	posHistos[2][j]->SetNameTitle(hn,hn);
	zHistos_[j].push_back(posHistos[2][j]);
	sprintf(hn,"x%c%3.3d",charAB[j],n);
	posHistosLocal[0][j]->SetNameTitle(hn,hn);
	xHistos_[j].push_back(posHistosLocal[0][j]);
	sprintf(hn,"y%c%3.3d",charAB[j],n);
	posHistosLocal[1][j]->SetNameTitle(hn,hn);
	yHistos_[j].push_back(posHistosLocal[1][j]);
	sprintf(hn,"doublePull%c%3.3d",charAB[j],n);
	doublePullHistos[j]->SetNameTitle(hn,hn);
	doublePullHistos_[j].push_back(doublePullHistos[j]);
	sprintf(hn,"hitPull%c%3.3d",charAB[j],n);
	hitPullHistos[j]->SetNameTitle(hn,hn);
	hitPullHistos_[j].push_back(hitPullHistos[j]);
	sprintf(hn,"predPull%c%3.3d",charAB[j],n);
	predPullHistos[j]->SetNameTitle(hn,hn);
	predPullHistos_[j].push_back(predPullHistos[j]);
#ifdef HISTOS2D
	sprintf(hn,"resVsAngle%c%3.3d",charAB[j],n);
	resVsAngleHistos[j]->SetNameTitle(hn,hn);
	resVsAngleHistos_[j].push_back(resVsAngleHistos[j]);
	sprintf(hn,"ddVsLocalX%c%3.3d",charAB[j],n);
	ddVsLocalXHistos[j]->SetNameTitle(hn,hn);
	ddVsLocalXHistos_[j].push_back(ddVsLocalXHistos[j]);
	sprintf(hn,"ddVsLocalY%c%3.3d",charAB[j],n);
	ddVsLocalYHistos[j]->SetNameTitle(hn,hn);
	ddVsLocalYHistos_[j].push_back(ddVsLocalYHistos[j]);
	sprintf(hn,"ddVsDxdz%c%3.3d",charAB[j],n);
	ddVsDxdzHistos[j]->SetNameTitle(hn,hn);
	ddVsDxdzHistos_[j].push_back(ddVsDxdzHistos[j]);
	sprintf(hn,"ddVsDydz%c%3.3d",charAB[j],n);
	ddVsDydzHistos[j]->SetNameTitle(hn,hn);
	ddVsDydzHistos_[j].push_back(ddVsDydzHistos[j]);
	sprintf(hn,"localXVsLocalY%c%3.3d",charAB[j],n);
	localXVsLocalYHistos[j]->SetNameTitle(hn,hn);
	localXVsLocalYHistos_[j].push_back(localXVsLocalYHistos[j]);
	sprintf(hn,"dxdzVsDydz%c%3.3d",charAB[j],n);
	dxdzVsDydzHistos[j]->SetNameTitle(hn,hn);
	dxdzVsDydzHistos_[j].push_back(dxdzVsDydzHistos[j]);
	sprintf(hn,"localXVsDxdz%c%3.3d",charAB[j],n);
	localXVsDxdzHistos[j]->SetNameTitle(hn,hn);
	localXVsDxdzHistos_[j].push_back(localXVsDxdzHistos[j]);
	sprintf(hn,"localYVsDxdz%c%3.3d",charAB[j],n);
	localYVsDxdzHistos[j]->SetNameTitle(hn,hn);
	localYVsDxdzHistos_[j].push_back(localYVsDxdzHistos[j]);
	sprintf(hn,"localXVsDydz%c%3.3d",charAB[j],n);
	localXVsDydzHistos[j]->SetNameTitle(hn,hn);
	localXVsDydzHistos_[j].push_back(localXVsDydzHistos[j]);
	sprintf(hn,"localYVsDydz%c%3.3d",charAB[j],n);
	localYVsDydzHistos[j]->SetNameTitle(hn,hn);
	localYVsDydzHistos_[j].push_back(localYVsDydzHistos[j]);
	sprintf(hn,"dPreddSimVsdHitdSim%c%3.3d",charAB[j],n);
	dPreddSimVsdHitdSimHistos[j]->SetNameTitle(hn,hn);
	dPreddSimVsdHitdSimHistos_[j].push_back(dPreddSimVsdHitdSimHistos[j]);
#endif
      }
      ++n;
    }
    else {
      //
      // remove the other histograms
      //
      for ( int j=0; j<2; ++j ) {
	delete resHistos[j];
	delete resHistosPlusX[j];
	delete resHistosMinusX[j];
	delete resHistosPlusY[j];
	delete resHistosMinusY[j];
	delete predErrHistos[j];
	delete predErrHistosFirst[j];
	delete predErrHistosSecond[j];
	delete hitErrHistos[j];
	delete hitErrHistosFirst[j];
	delete hitErrHistosSecond[j];
	delete simRecHistos[j];
	delete simTrkHistos[j];
	delete dxdzHistos[j];
	delete posHistos[0][j];
	delete posHistos[1][j];
	delete posHistos[2][j];
	delete posHistosLocal[0][j];
	delete posHistosLocal[1][j];
	delete doublePullHistos[j];
	delete hitPullHistos[j];
	delete predPullHistos[j];
#ifdef HISTOS2D
	delete resVsAngleHistos[j];
	delete ddVsLocalXHistos[j];
	delete ddVsLocalYHistos[j];
	delete ddVsDxdzHistos[j];
	delete ddVsDydzHistos[j];
	delete localXVsLocalYHistos[j];
	delete dxdzVsDydzHistos[j];
	delete localXVsDxdzHistos[j];
	delete localYVsDxdzHistos[j];
	delete localXVsDydzHistos[j];
	delete localYVsDydzHistos[j];
	delete dPreddSimVsdHitdSimHistos[j];
#endif
      }
    }
  }

  // Summary histograms (one bin / pair)
  //   (units: micrometer)
  //
  char hn[32];
  sprintf(hn,"sigMean");
  TH1* predErrMeans = new TH1F(hn,hn,n,-0.5,n-0.5);
  predErrMeans->GetXaxis()->SetTitle("module pair index");
  predErrMeans->GetYaxis()->SetTitle("<#sigma(#Deltax_{pred}> [#mum]");
  sprintf(hn,"sigMeanFirst");
  TH1* predErrMeansFirst = new TH1F(hn,hn,n,-0.5,n-0.5);
  predErrMeansFirst->GetXaxis()->SetTitle("module pair index");
  predErrMeansFirst->GetYaxis()->SetTitle("<#sigma(x_{pred1}> [#mum]");
  sprintf(hn,"sigMeanSecond");
  TH1* predErrMeansSecond = new TH1F(hn,hn,n,-0.5,n-0.5);
  predErrMeansSecond->GetXaxis()->SetTitle("module pair index");
  predErrMeansSecond->GetYaxis()->SetTitle("<#sigma(x_{pred2}> [#mum]");
  sprintf(hn,"simRec");
  TH1* simRec = new TH1F(hn,hn,n,-0.5,n-0.5);
  simRec->GetXaxis()->SetTitle("module pair index");
  simRec->GetYaxis()->SetTitle("#sigma(SimHit-RecHit) [#mum]");
  sprintf(hn,"simTrk");
  TH1* simTrk = new TH1F(hn,hn,n,-0.5,n-0.5);
  simTrk->GetXaxis()->SetTitle("module pair index");
  simTrk->GetYaxis()->SetTitle("#sigma(SimHit-PredHit) [#mum]");
  sprintf(hn,"mean");
  TH1* meanDiffs = new TH1F(hn,hn,n,-0.5,n-0.5);
  meanDiffs->GetXaxis()->SetTitle("module pair index");
  meanDiffs->GetYaxis()->SetTitle("<#Deltax_{hit}-#Deltax_{pred}> [#mum]");
  sprintf(hn,"meanPlus");
  TH1* meanDiffsPlus = new TH1F(hn,hn,n,-0.5,n-0.5);
  meanDiffsPlus->GetXaxis()->SetTitle("module pair index");
  meanDiffsPlus->GetYaxis()->SetTitle("fitted mean #Deltax_{hit}-#Deltax_{pred} y>0[#mum]");
  sprintf(hn,"meanMinus");
  TH1* meanDiffsMinus = new TH1F(hn,hn,n,-0.5,n-0.5);
  meanDiffsMinus->GetXaxis()->SetTitle("module pair index");
  meanDiffsMinus->GetYaxis()->SetTitle("fitted mean #Deltax_{hit}-#Deltax_{pred} y<0[#mum]");
  sprintf(hn,"width");
  TH1* sigmaDiffs = new TH1F(hn,hn,n,-0.5,n-0.5);
  sigmaDiffs->GetXaxis()->SetTitle("module pair index");
  sigmaDiffs->GetYaxis()->SetTitle("fitted width #Deltax_{hit}-#Deltax_{pred} [#mum]");
  TH1* hitErrMeans = new TH1F("hitSigX","hitSigX",n,-0.5,n-0.5);
  TH1* dxdzMeans = new TH1F("dxdz","dxdz",n,-0.5,n-0.5);
  TH1* layerHisto = new TH1F("layer","layer",n,-0.5,n-0.5);
  TH1* stereoHisto = new TH1F("stereo","stereo",n,-0.5,n-0.5);
  TH1* radHisto = new TH1F("radius","radius",n,-0.5,n-0.5);
  TH1* phiHisto = new TH1F("phi","phi",n,-0.5,n-0.5);
  TH1* zHisto = new TH1F("z","z",n,-0.5,n-0.5);
  TH1* xHisto = new TH1F("x","x",n,-0.5,n-0.5);
  TH1* yHisto = new TH1F("y","y",n,-0.5,n-0.5);
  TH1* statHisto = new TH1F("stat","stat",n,-0.5,n-0.5);
  TH1* doublePulls = new TH1F("doublePull","doublePull",n,-0.5,n-0.5);
  TH1* hitPulls = new TH1F("hitPull","hitPull",n,-0.5,n-0.5);
  TH1* predPulls = new TH1F("predPull","predPull",n,-0.5,n-0.5);
#ifdef HISTOS2D
  TH1* ddVsLocalYSlope = new TH1F("ddYslope","ddYslope",n,-0.5,n-0.5);
  ddVsLocalYSlope->GetYaxis()->SetTitle("double diff vs local Y slope");
  TH1* ddVsLocalYOffset = new TH1F("ddYoffset","ddYoffset",n,-0.5,n-0.5);
  ddVsLocalYOffset->GetYaxis()->SetTitle("double diff vs local Y offset");
#endif

  //
  // Filling of summary histograms (loop over final containers)
  //
  TH1* resHisto(0);
  TH1* resHistoPlusX(0);
  TH1* resHistoMinusX(0);
  TH1* resHistoPlusY(0);
  TH1* resHistoMinusY(0);
  TH1* predErrHisto(0);
  TH1* predErrHistoFirst(0);
  TH1* predErrHistoSecond(0);
  TH1* simRecHisto(0);
  TH1* simTrkHisto(0);
  TH1* hitErrHisto(0);
  TH1* dxdzHisto(0);
  TH1* posHistos[3];
  TH1* posHistosLocal[2];
  TH1* doublePullHisto(0);
  TH1* hitPullHisto(0);
  TH1* predPullHisto(0);
#ifdef HISTOS2D
  TH2* ddVsLocalY(0);
#endif
  const int kNotDraw = 1<<9;
  for ( unsigned int i=0; i<n; ++i ) {
    //
      // Choose DetId order with more entries and retrieve histogram pointers
    //
    int j = 
      residualHistos_[0][i]->GetEntries()>residualHistos_[1][i]->GetEntries() ? 0 : 1;
    resHisto = residualHistos_[j][i];
    resHistoPlusX = residualHistosPlusX_[j][i];
    resHistoMinusX = residualHistosMinusX_[j][i];
    resHistoPlusY = residualHistosPlusY_[j][i];
    resHistoMinusY = residualHistosMinusY_[j][i];
    predErrHisto = predErrHistos_[j][i];
    predErrHistoFirst = predErrHistosFirst_[j][i];
    predErrHistoSecond = predErrHistosSecond_[j][i];
    simRecHisto = simRecHistos_[j][i];
    simTrkHisto = simTrkHistos_[j][i];
    hitErrHisto = hitErrHistos_[j][i];
    dxdzHisto = dxdzHistos_[j][i];
    posHistos[0] = radHistos_[j][i];
    posHistos[1] = phiHistos_[j][i];
    posHistos[2] = zHistos_[j][i];
    posHistosLocal[0] = xHistos_[j][i];
    posHistosLocal[1] = yHistos_[j][i];
    doublePullHisto = doublePullHistos_[j][i];
    hitPullHisto = hitPullHistos_[j][i];
    predPullHisto = predPullHistos_[j][i];
#ifdef HISTOS2D
    ddVsLocalY = ddVsLocalYHistos_[j][i];
#endif
//     std::cout << "det IDs: " << detIdPairs_[i].first << " "
//	      << detIdPairs_[i].second << " ";
//     std::cout << resHisto->GetName() << " " << resHisto->GetEntries() << " ";
//     if ( resHisto->GetEntries()>0 )
//       std::cout << (resHisto->GetBinContent(0)+resHisto->GetBinContent(201))/
// 	resHisto->GetEntries() << " ";
//     else
//       std::cout << "0 ";
//     std::cout << std::endl;

    unsigned int subdet;
    unsigned int layer;
    unsigned int stereo;
    
    unsigned int id = j==0 ? detIdPairs_[i].first : detIdPairs_[i].second;
    decode(id,subdet,layer,stereo);
    // sequential layer number in TIB & TOB (for debugging)
    int k(-1);
    if ( subdet==3 && layer>0 && layer<5 ) {
      k = layer;
    }
    else if ( subdet==5 && layer>0 && layer<7 ) {
      k = layer + 4;
    }

    //
    // fill summaries
    //
    if (resHisto->Integral()>110 ) {
      // debug info on layers
      layerHisto->SetBinContent(i+1,k);
      stereoHisto->SetBinContent(i+1,stereo);
      // statistics
      statHisto->SetBinContent(i+1,resHisto->Integral());
      // global position
      fillMean(i+1,radHisto,posHistos[0]);
      fillMean(i+1,phiHisto,posHistos[1]);
      fillMean(i+1,zHisto,posHistos[2]);
      fillMean(i+1,xHisto,posHistosLocal[0]);
      fillMean(i+1,yHisto,posHistosLocal[1]);
      if (k >= 1 && k <= 4) {
	hDir->cd();
#ifdef HISTOS2D
      fillSlope(i+1,ddVsLocalYSlope, ddVsLocalYOffset, ddVsLocalY);
#endif
      curDir->cd();
      }
      // Gaussian fit to double-difference
      //
      //resHisto->Fit("gaus","Q0R","",resHisto->GetMean()-0.025,resHisto->GetMean()+0.025);
      resHisto->Fit("gaus","Q0R","",resHisto->GetMean()-3*resHisto->GetRMS(),resHisto->GetMean()+3*resHisto->GetRMS());
      //gStyle->SetOptFit(1011);
      //resHisto->Fit("gaus","LQ0R");
      resHisto->GetFunction("gaus")->ResetBit(kNotDraw);
      //cout << "for index " << i << " chisq is " << resHisto->GetFunction("gaus")->GetChisquare() << "  RMS is " << resHisto->GetRMS() << "  fit sigma is " << resHisto->GetFunction("gaus")->GetParameter(2)  << endl;
      double mDiff = resHisto->GetFunction("gaus")->GetParameter(1);
      double emDiff = resHisto->GetFunction("gaus")->GetParError(1);
      meanDiffs->SetBinContent(i+1,10000*mDiff);
      meanDiffs->SetBinError(i+1,10000*emDiff);
      if( predY[0] > 0) { //need to sort out if predY>0 before the overlap summary
	//cout << "positive: predY = " << predY[0] << endl;
	meanDiffsPlus->SetBinContent(i+1,10000*mDiff);
	meanDiffsPlus->SetBinError(i+1,10000*emDiff);
      }
      if( predY[0] < 0) {
	//cout << "negative: predY = " << predY[0] << endl;
	meanDiffsMinus->SetBinContent(i+1,10000*mDiff);
	meanDiffsMinus->SetBinError(i+1,10000*emDiff);
      }
      double sDiff = resHisto->GetFunction("gaus")->GetParameter(2);
      double esDiff = resHisto->GetFunction("gaus")->GetParError(2);
      sigmaDiffs->SetBinContent(i+1,10000*sDiff);
      sigmaDiffs->SetBinError(i+1,10000*esDiff);
      // predicted error // fold in some uncertainty from the spread (systematic)
      fillMean(i+1,predErrMeans,predErrHisto,10000.);
      // fillMeanWithSyst(i+1,predErrMeans,predErrHisto,predErrHistoFirst,10000.);
      //predicted error for each overlap module individually
      fillMean(i+1,predErrMeansFirst,predErrHistoFirst,10000.);
      fillMean(i+1,predErrMeansSecond,predErrHistoSecond,10000.);
      fillWidth(i+1,simRec,simRecHisto,10000);
      fillWidth(i+1,simTrk,simTrkHisto,10000);
      // hit error
      fillMean(i+1,hitErrMeans,hitErrHisto,10000.);
      // local dxdz
      fillMean(i+1,dxdzMeans,dxdzHisto);
      // double pull
      fillWidth(i+1,doublePulls,doublePullHisto,1.);
      // hit pull
      fillWidth(i+1,hitPulls,hitPullHisto,1.);
      // pred pull
//       fillWidth(i+1,predPulls,predPullHisto,1.);
      predPulls->SetBinContent(i+1,predPullHisto->GetRMS());

      //print out to check the ordering of the index within each layer
            std::cout << "Index: " << i << "   Layer: " << k << "   Phi: " << posHistos[1]->GetMean() << "   r: " << posHistos[0]->GetMean() << "   z: " << posHistos[2]->GetMean() << "  dx/dz: " << dxdzHisto->GetMean() << "   detID = " << detIdPairs_[i].first << " and " << detIdPairs_[i].second << std::endl;
      //    std::cout << " geographical ID: " << id << std::endl;

//       predErrMeans->SetBinContent(i+1,predErrHisto->GetMean());
//       predErrMeans->SetBinError(i+1,predErrHisto->GetRMS()/predErrHisto->Integral());
//       hitErrMeans->SetBinContent(i+1,hitErrHisto->GetMean());
//       hitErrMeans->SetBinError(i+1,hitErrHisto->GetRMS()/hitErrHisto->Integral());
//       double pred2Mean = predErrHisto->GetMean();
//       double pred2Sigma = predErrHisto->GetRMS()/sqrt(predErrHisto->Integral());
//       double sum2Mean = resHisto->GetFunction("gaus")->GetParameter(2);
//       double sum2Sigma = 2*sum2Mean*resHisto->GetFunction("gaus")->GetParError(2);
//       sum2Mean *= sum2Mean;
//       double diffMean = sum2Mean - pred2Mean;
//       diffMean = diffMean>0 ? sqrt(diffMean) : -sqrt(-diffMean);
//       double diffSigma = sqrt(sum2Sigma*sum2Sigma+pred2Sigma*pred2Sigma)/2./fabs(diffMean);
//       predErrMeans->SetBinContent(i+1,diffMean);
//       predErrMeans->SetBinError(i+1,diffSigma);

      if(k==5)   { width2VsAngle_TOB1->Fill(dxdzHisto->GetMean(),sDiff*sDiff);
	predErr2VsAngle_TOB1->Fill(dxdzHisto->GetMean(),predErrHisto->GetMean()*predErrHisto->GetMean());
      hitErr2VsAngle_TOB1->Fill(dxdzHisto->GetMean(),hitErrHisto->GetMean()*hitErrHisto->GetMean());
      alphaVsAngle_TOB1->Fill(dxdzHisto->GetMean(),sqrt((sDiff*sDiff-predErrHisto->GetMean()*predErrHisto->GetMean())/hitErrHisto->GetMean()*hitErrHisto->GetMean()));
    }
      if(k==6)   { width2VsAngle_TOB2->Fill(dxdzHisto->GetMean(),sDiff*sDiff);
	predErr2VsAngle_TOB2->Fill(dxdzHisto->GetMean(),predErrHisto->GetMean()*predErrHisto->GetMean());
      hitErr2VsAngle_TOB2->Fill(dxdzHisto->GetMean(),hitErrHisto->GetMean()*hitErrHisto->GetMean());
      alphaVsAngle_TOB2->Fill(dxdzHisto->GetMean(),sqrt((sDiff*sDiff-predErrHisto->GetMean()*predErrHisto->GetMean())/hitErrHisto->GetMean()*hitErrHisto->GetMean()));
    }
      if(k==7)   { width2VsAngle_TOB3->Fill(dxdzHisto->GetMean(),sDiff*sDiff);
	predErr2VsAngle_TOB3->Fill(dxdzHisto->GetMean()*dxdzHisto->GetMean(),predErrHisto->GetMean()*predErrHisto->GetMean());
      hitErr2VsAngle_TOB3->Fill(dxdzHisto->GetMean(),hitErrHisto->GetMean()*hitErrHisto->GetMean());
      alphaVsAngle_TOB3->Fill(dxdzHisto->GetMean(),sqrt((sDiff*sDiff-predErrHisto->GetMean()*predErrHisto->GetMean())/hitErrHisto->GetMean()*hitErrHisto->GetMean()));
    }
      if(k==8)   {
	width2VsAngle_TOB4->Fill(dxdzHisto->GetMean(),sDiff*sDiff);
	predErr2VsAngle_TOB4->Fill(dxdzHisto->GetMean(),predErrHisto->GetMean()*predErrHisto->GetMean());
	hitErr2VsAngle_TOB4->Fill(dxdzHisto->GetMean(),hitErrHisto->GetMean()*hitErrHisto->GetMean());
	alphaVsAngle_TOB4->Fill(dxdzHisto->GetMean(),sqrt((sDiff*sDiff-predErrHisto->GetMean()*predErrHisto->GetMean())/hitErrHisto->GetMean()*hitErrHisto->GetMean()));
      }
    }
  }
}

void
OverlapHistos::showPair (unsigned int i) const
{
  std::cout << "DetPair subdet / layer / stereo = " << std::endl;
  if ( i<detIdPairs_.size() ) {
    unsigned int subdet;
    unsigned int layer;
    unsigned int stereo;
    decode(detIdPairs_[i].first,subdet,layer,stereo);
    std::cout << " " << subdet << " " << layer << " " << stereo << std::endl;
    decode(detIdPairs_[i].second,subdet,layer,stereo);
    std::cout << " " << subdet << " " << layer << " " << stereo << std::endl;
  }
  else {
    std::cout << "  -" << std::endl;
    std::cout << "  -" << std::endl;
  }
}

void
OverlapHistos::decode (unsigned int id, unsigned int& subdet, 
		  unsigned int& layer, unsigned int& stereo)  const
{
  subdet = (id>>25) & 0x7;
  if ( subdet==3 ) {
    layer = (id>>14) & 0x7;
  }
  else if ( subdet==5 ) {
    layer = (id>>14) & 0x7;
  }
  else {
    layer = 0;
  }
  stereo = id&0x3;
}

TH1*
OverlapHistos::residualHistogram (unsigned int i, bool first) const
{
  int j = first ? 0 : 1;
  if ( i<detIdPairs_.size() ) 
    return residualHistos_[j][i];
  else
    return 0;
}

void
OverlapHistos::fillMean (int ibin, TH1* resultHisto, TH1* inputHisto,
		    float scale) const
{
  double sum = inputHisto->Integral();
  if ( sum>0.00001 ) {
    resultHisto->SetBinContent(ibin,scale*inputHisto->GetMean());
    resultHisto->SetBinError(ibin,scale*inputHisto->GetRMS());
  }
}


void
OverlapHistos::fillMeanWithSyst (int ibin, TH1* resultHisto, TH1* inputHisto,
		           TH1* inputSystHisto, float scale) const
{
  double sum = inputHisto->Integral();
  double systEntries = inputSystHisto->GetEntries();
  if ( sum>0.00001 ) {
    resultHisto->SetBinContent(ibin,scale*inputHisto->GetMean());
    //    resultHisto->SetBinError(ibin,scale*(sqrt(((inputHisto->GetRMS()*inputHisto->GetRMS())/sum)+((inputSystHisto->GetRMS()*inputSystHisto->GetRMS())/systEntries)+0.0000168)));
    //the momentum estimation gives an uncertainty of 45% on 1/p, which scales
    //as the uncertainty in the multiple scattering uncertainty
    resultHisto->SetBinError(ibin,scale*(sqrt(((inputHisto->GetRMS()*inputHisto->GetRMS())/sum)+((inputSystHisto->GetRMS()*inputSystHisto->GetRMS())/systEntries)+(0.45*inputHisto->GetMean()*0.45*inputHisto->GetMean()))));
  }
}

void OverlapHistos::fillSlope (int ibin, TH1* resultSlopeHisto, TH1* resultOffsetHisto, TH2* inputHisto) const {
  double sum = inputHisto->Integral();
  if ( sum>0.00001 ) {
    const int kNotDraw = 1<<9;
    inputHisto->RebinX(40);
    TProfile *prof = inputHisto->ProfileX();
    prof->Fit("pol1","Q0","",-5.0, 5.0);
    prof->GetFunction("pol1")->ResetBit(kNotDraw);
    resultSlopeHisto->SetBinContent(ibin,prof->GetFunction("pol1")->GetParameter(1));
    resultSlopeHisto->SetBinError(ibin,prof->GetFunction("pol1")->GetParError(1));
    resultOffsetHisto->SetBinContent(ibin,prof->GetFunction("pol1")->GetParameter(0));
    resultOffsetHisto->SetBinError(ibin,prof->GetFunction("pol1")->GetParError(0));
  }
}

void
OverlapHistos::fillWidth (int ibin, TH1* resultHisto, TH1* inputHisto,
		    float scale) const
{
  double sum = inputHisto->Integral();
  if ( sum>0.00001 ) {
    inputHisto->Fit("gaus","Q0");
    double sigma = inputHisto->GetFunction("gaus")->GetParameter(2);
    double error_sigma = inputHisto->GetFunction("gaus")->GetParError(2);
    resultHisto->SetBinContent(ibin,scale*sigma);
    resultHisto->SetBinError(ibin,scale*error_sigma);
  }
}
