#define OverlapHistos_cxx
#include "OverlapHistos.h"
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TF1.h>
#include <TProfile.h>

#include <iostream>
#include <algorithm>
#include <ctime>

#include "overlapHelper.C"

using namespace std;

  /**
   * The argument is the number of entries in the tree which 
   * you want to process.
   * -1 is all entries (default argument of method)
   */
void OverlapHistos::Loop(int redEntries){
  unsigned int subdet;
  unsigned int layer;
  unsigned int stereo;
  int part;
  unsigned int id;

  if (fChain == 0) return;
  //
  // loop over all pairs, check entries and create new index
  // 
  time_t start,end;
  time (&start);

  Long64_t nentries = ( redEntries == -1) ? fChain->GetEntriesFast() : redEntries;
  cout << "Run on : " << nentries<<" entries\n";
  TDirectory* curDir = gDirectory;

  TFile * out = new TFile(outputFile,"RECREATE");
  TDirectory* hDir = out->mkdir("ResidualHistograms");
  TDirectory* resDir = out->mkdir("ResHistos");
  TDirectory* predDir = out->mkdir("PredHistos");
  TDirectory* hitDir = out->mkdir("HitHistos");
  TDirectory* posDir = out->mkdir("PosHistos");
  TDirectory* pullDir = out->mkdir("PullHistos");
#ifdef HISTOS2D
  TDirectory* corrDir = out->mkdir("CorrelationHistos");
#endif
  out->cd();

  unsigned int overlapIds_[2];
  TTree* rootTree_;
  rootTree_ = new TTree("Overlaps","Overlaps");
  rootTree_->Branch("detids",overlapIds_,"id[2]/i");

  //
  // containers of ids and histograms for all overlaps 
  //   found in the tree (in order / reverse order of the ids)
  //
  std::map<DetIdPair,unsigned int> allIndices;

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

  TH1F* pahtH = new TH1F("path", "path", 200, -200., 0.);


  Long64_t nbytes = 0, nb = 0;

  //
  // the overlapCounter map is used in a first pass to count the number of entries 
  // each overlap-pair vould have. This allows to look only at the entries for 
  // which the pair has enough entries
  //
  TH1F *layerH = new TH1F("layerH","layerH",34,0.5,34.5);
  TH1F *stereoH = new TH1F("stereoH","stereoH",2,-0.5,+1.5);
  std::map<DetIdPair, iiPair> overlapCounter;
  int ctr= 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;

    if (!cut()) continue;
    decode(detids[0],subdet,layer,part,stereo);
    
    if ((!acceptDet[0]) && (!acceptDet[subdet])) continue;
    if ((acceptLayer>0) && (acceptLayer!=layer)) continue;
    if (!acceptStereo[stereo&1]) continue;
    DetIdPair idPair(min(detids[0],detids[1]),
		     max(detids[0],detids[1]));
    if (!checkAdjacent(idPair.first, idPair.second, path)) continue;
    
//     cout << "first pass "<<min(detids[0],detids[1])<<" "<<
// 		     max(detids[0],detids[1])<<" \n";
    std::map<DetIdPair,iiPair>::iterator it = overlapCounter.find(idPair);
    if ( it==overlapCounter.end() ) {
      it = overlapCounter.insert(it, pair<DetIdPair,iiPair>(idPair, iiPair(0,0)));
//     } else {
//       ++((*it).second);
    }
    int j = idPair.first==detids[0] ? 0 : 1;
    if (j==0) ++((*it).second.first);
      else ++((*it).second.second);
    ++ctr;
  }
  cout << "Have " << ctr<< " entries after first pass\n";
  cout << "Have " << overlapCounter.size()<< " overlap pairs after first pass (before cut)\n";

  ctr=0;
  for ( std::map<DetIdPair, iiPair>::const_iterator ih=overlapCounter.begin();
	ih!=overlapCounter.end(); ++ih ) {
    if (max((*ih).second.first, (*ih).second.second) > threshold) {
      ++ctr;
//     decode((*ih).first.first,subdet,layer,part,stereo);
//     cout << "Pair "<< n<<" : " << (*ih).first.first<< " ( ";
//     printId((*ih).first.first);
//     cout <<" ) " << (*ih).first.second << " ( ";
//     printId((*ih).first.second);     
// //     decode((*ih).first.second,subdet,layer,part,stereo);
// //     cout <<" ) " << (*ih).first.second << " ( " << subdet << " " << layer << " " << stereo 
//     cout << " ) "<<(*ih).second  << " " << resHistos->GetEntries() 
// 	 << (resHistos->GetEntries()>threshold  ? " YES " : "     " )
// 	 << endl;

//     cout << "check "<<(*ih).first.first<<" "<<(*ih).first.second<<
//     " " << (*ih).second.first<<" "<<(*ih).second.second<<endl;
    }
  }
  cout << "Overlaps identified (after cuts): " << ctr<<endl;
//return;

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

  detIdPairs_.reserve(allIndices.size());
  residualHistos_.reserve(allIndices.size());
  residualHistosPlusX_.reserve(allIndices.size());  
  residualHistosMinusX_.reserve(allIndices.size()); 
  residualHistosPlusY_.reserve(allIndices.size());  
  residualHistosMinusY_.reserve(allIndices.size()); 
  predErrHistos_.reserve(allIndices.size());
  predErrHistosFirst_.reserve(allIndices.size());
  predErrHistosSecond_.reserve(allIndices.size());
  hitErrHistos_.reserve(allIndices.size());
  hitErrHistosFirst_.reserve(allIndices.size());
  hitErrHistosSecond_.reserve(allIndices.size());
//     simRecHistos_.reserve(allIndices.size());
//     simTrkHistos_.reserve(allIndices.size());  
  dxdzHistos_.reserve(allIndices.size());
  radHistos_.reserve(allIndices.size());
  phiHistos_.reserve(allIndices.size());
  zHistos_.reserve(allIndices.size());
  doublePullHistos_.reserve(allIndices.size());
  hitPullHistos_.reserve(allIndices.size());
  predPullHistos_.reserve(allIndices.size());
#ifdef HISTOS2D
  resVsAngleHistos_.reserve(allIndices.size());
  ddVsLocalXHistos_.reserve(allIndices.size());
  ddVsLocalYHistos_.reserve(allIndices.size());
  ddVsDxdzHistos_.reserve(allIndices.size());
  ddVsDydzHistos_.reserve(allIndices.size());
  if (allPlots){
    localXVsLocalYHistos_.reserve(allIndices.size());
    dxdzVsDydzHistos_.reserve(allIndices.size());    
    localXVsDxdzHistos_.reserve(allIndices.size());    
    localYVsDxdzHistos_.reserve(allIndices.size());    
    localXVsDydzHistos_.reserve(allIndices.size());    
    localYVsDydzHistos_.reserve(allIndices.size());    
    dPreddSimVsdHitdSimHistos_.reserve(allIndices.size());
  }
  localY_.reserve(allIndices.size());
  localYE_.reserve(allIndices.size());
  dxdz_.reserve(allIndices.size());
  dxdzE_.reserve(allIndices.size());
  dd_.reserve(allIndices.size());
  ddE_.reserve(allIndices.size());
#endif

  nbytes = 0; nb = 0;
  ctr=0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;

//     cout << "test "<<min(detids[0],detids[1])<<" "<<
// 		     max(detids[0],detids[1])<<" ";
    if (!cut()) continue;//{cout <<" cut\n";continue;}

    //
    widthVsAngle->Fill(predDX[0],(hitX[0] + relSignX*hitX[1]) - (predX[0] + relSignX*predX[1]));
   //(dx/dz, deltaHit-deltaPred)
    deltaXPred->Fill(predX[0] + relSignX*predX[1]);
    doubleDiff->Fill((hitX[0] + relSignX*hitX[1]) - (predX[0] + relSignX*predX[1]));
    pahtH->Fill(path);
  
    id = detids[0];
    decode(id,subdet,layer,part,stereo);
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

    if (!checkAdjacent(idPair.first, idPair.second, path)) continue;//{ cout <<"adj\n";continue;}

    std::map<DetIdPair, iiPair>::iterator it2 = overlapCounter.find(idPair);


    if ( it2==overlapCounter.end() ) continue;//{cout << endl;continue;}
    int j = idPair.first==detids[0] ? 0 : 1;
//     cout << " "<<j<<
//      " "<<(*it2).second.first<<" "<<(*it2).second.second<<endl;
    if ((j==0 ? (*it2).second.first : (*it2).second.second) <= threshold) continue;
    ++ctr;
    unsigned int ind(0);

    std::map<DetIdPair,unsigned int>::iterator it = allIndices.find(idPair);
    if ( it==allIndices.end() ) {
      //
      // new pair: create histograms
      //
      ind = residualHistos_.size();
      allIndices[idPair] = ind;
      detIdPairs_.push_back(idPair);

      overlapIds_[0] = idPair.first;
      overlapIds_[1] = idPair.second;
      rootTree_->Fill();

      char hn[32];
      hDir->cd();
//       cout << "idPair "<<ind<<endl;
      // create histograms for reverse / normal order of DetIds
	resDir->cd();
	sprintf(hn,"rh%4.4d",ind);
	resHisto = new TH1F(hn,hn,200,-0.2,0.2);
	residualHistos_.push_back(resHisto);
	sprintf(hn,"rplusXh%4.4d",ind);
	resHistoPlusX = new TH1F(hn,hn,200,-0.2,0.2);
	residualHistosPlusX_.push_back(resHistoPlusX);
	sprintf(hn,"rminusXh%4.4d",ind);
	resHistoMinusX = new TH1F(hn,hn,200,-0.2,0.2);
	residualHistosMinusX_.push_back(resHistoMinusX);
	sprintf(hn,"rplusYh%4.4d",ind);
	resHistoPlusY = new TH1F(hn,hn,200,-0.2,0.2);
	residualHistosPlusY_.push_back(resHistoPlusY);
	sprintf(hn,"rminusYh%4.4d",ind);
	resHistoMinusY = new TH1F(hn,hn,200,-0.2,0.2);
	residualHistosMinusY_.push_back(resHistoMinusY);
	predDir->cd();
	sprintf(hn,"pSig%4.4d",ind);
	predErrHisto = new TH1F(hn,hn,100,0,0.01);
	predErrHistos_.push_back(predErrHisto);
	sprintf(hn,"pSigFirst%4.4d",ind);
	predErrHistoFirst = new TH1F(hn,hn,100,0,0.01);
	predErrHistosFirst_.push_back(predErrHistoFirst);
	sprintf(hn,"pSigSecond%4.4d",ind);
	predErrHistoSecond = new TH1F(hn,hn,100,0,0.01);
	predErrHistosSecond_.push_back(predErrHistoSecond);
	hitDir->cd();
	sprintf(hn,"hSig%4.4d",ind);
	hitErrHisto = new TH1F(hn,hn,100,0,0.05);
	hitErrHistos_.push_back(hitErrHisto);
	sprintf(hn,"hSigFirst%4.4d",ind);
	hitErrHistoFirst = new TH1F(hn,hn,100,0,0.01);
	hitErrHistosFirst_.push_back(hitErrHistoFirst);
	sprintf(hn,"hSigSecond%4.4d",ind);
	hitErrHistoSecond = new TH1F(hn,hn,100,0,0.01);
	hitErrHistosSecond_.push_back(hitErrHistoSecond);
	hDir->cd();
// 	sprintf(hn,"simrec%4.4d",ind);
// 	simRecHisto = new TH1F(hn,hn,400,-0.2,0.2);
// 	simRecHistos_.push_back(simRecHisto);                        
// 	sprintf(hn,"simtrk%4.4d",ind);
// 	simTrkHisto = new TH1F(hn,hn,400,-0.2,0.2);
// 	simTrkHistos_.push_back(simTrkHisto);   
	sprintf(hn,"hDxdz%4.4d",ind);
	dxdzHisto = new TH1F(hn,hn,100,-2.,2.);
	dxdzHistos_.push_back(dxdzHisto);
	posDir->cd();
	sprintf(hn,"rad%4.4d",ind);
	posHistos[0] = new TH1F(hn,hn,110,0,110.);
	radHistos_.push_back(posHistos[0]);
	sprintf(hn,"phi%4.4d",ind);
	posHistos[1] = new TH1F(hn,hn,100,-TMath::Pi(),TMath::Pi());
	phiHistos_.push_back(posHistos[1]);
	sprintf(hn,"z%4.4d",ind);
	posHistos[2] = new TH1F(hn,hn,135,-270,270);
	zHistos_.push_back(posHistos[2]);
	sprintf(hn,"x%4.4d",ind);
	posHistosLocal[0] = new TH1F(hn,hn,200,-10,10);
	xHistos_.push_back(posHistosLocal[0]);
	sprintf(hn,"y%4.4d",ind);
	posHistosLocal[1] = new TH1F(hn,hn,200,-10,10);
	yHistos_.push_back(posHistosLocal[1]);
	pullDir->cd();
	sprintf(hn,"doublePull%4.4d",ind);
	doublePullHisto = new TH1F(hn,hn,150,-15.,15.);
	doublePullHistos_.push_back(doublePullHisto);
	sprintf(hn,"hitPull%4.4d",ind);
	hitPullHisto = new TH1F(hn,hn,150,-15.,15.);
	hitPullHistos_.push_back(hitPullHisto);
	sprintf(hn,"predPull%4.4d",ind);
	predPullHisto = new TH1F(hn,hn,150,-15.,15.);
	predPullHistos_.push_back(predPullHisto);
#ifdef HISTOS2D
	corrDir->cd();
	sprintf(hn,"resVsAngle%4.4d",ind);
	resVsAngle = new TH2F(hn,hn, 75, -0.75, 0.75, 100, -0.2, 0.2);
	resVsAngleHistos_.push_back(resVsAngle);
	sprintf(hn,"ddVsLocalX%4.4d",ind);
	ddVsLocalX = new TH2F(hn,hn, 100, -5, 5, 100, -0.2, 0.2);
	ddVsLocalXHistos_.push_back(ddVsLocalX);
	sprintf(hn,"ddVsLocalY%4.4d",ind);
	ddVsLocalY = new TH2F(hn,hn, 100, -10, 10, 100, -0.2, 0.2);
	ddVsLocalYHistos_.push_back(ddVsLocalY);
	sprintf(hn,"ddVsDxdz%4.4d",ind);
	ddVsDxdz = new TH2F(hn,hn, 100, -1, 1, 100, -0.2, 0.2);
	ddVsDxdzHistos_.push_back(ddVsDxdz);
	sprintf(hn,"ddVsDydz%4.4d",ind);
	ddVsDydz = new TH2F(hn,hn, 100, -1, 1, 100, -0.2, 0.2);
	ddVsDydzHistos_.push_back(ddVsDydz);
	if (allPlots){
	  sprintf(hn,"localXVsLocalY%4.4d",ind);
	  localXVsLocalY = new TH2F(hn,hn, 50, -5, 5, 50, -10, 10);
	  localXVsLocalYHistos_.push_back(localXVsLocalY);
	  sprintf(hn,"dxdzVsDydz%4.4d",ind);
	  dxdzVsDydz = new TH2F(hn,hn, 50, -1, 1, 50, -1, 1);
	  dxdzVsDydzHistos_.push_back(dxdzVsDydz);
	  sprintf(hn,"localXVsDxdz%4.4d",ind);
	  localXVsDxdz = new TH2F(hn,hn, 50, -1, 1, 50, -5, 5);
	  localXVsDxdzHistos_.push_back(localXVsDxdz);
	  sprintf(hn,"localYVsDxdz%4.4d",ind);
	  localYVsDxdz = new TH2F(hn,hn, 50, -1, 1, 50, -10, 10);
	  localYVsDxdzHistos_.push_back(localYVsDxdz);
	  sprintf(hn,"localXVsDydz%4.4d",ind);
	  localXVsDydz = new TH2F(hn,hn, 50, -1, 1, 50, -5, 5);
	  localXVsDydzHistos_.push_back(localXVsDydz);
	  sprintf(hn,"localYVsDydz%4.4d",ind);
	  localYVsDydz = new TH2F(hn,hn, 50, -1, 1, 50, -10, 10);
	  localYVsDydzHistos_.push_back(localYVsDydz);
	  sprintf(hn,"dPreddSimVsdHitdSim%4.4d",ind);
	  dPreddSimVsdHitdSim = new TH2F(hn,hn, 40, -0.02, 0.02, 40, -0.02, 0.02);
	  dPreddSimVsdHitdSimHistos_.push_back(dPreddSimVsdHitdSim);
	}
	localY_.push_back(vector<double>());
	localYE_.push_back(vector<double>());
	dd_.push_back(vector<double>());
	ddE_.push_back(vector<double>());
	dxdz_.push_back(vector<double>());
	dxdzE_.push_back(vector<double>());
	
#endif
      curDir->cd();
//cout << "Bookedok\n";
    }
    else {
      //
      // histograms exist: choose index for normal / reverse order
      //
      ind = (*it).second;
    }
    // normal / reverse order?
    // histogram pointers
    resHisto = residualHistos_[ind];
    resHistoPlusX = residualHistosPlusX_[ind];
    resHistoMinusX = residualHistosMinusX_[ind];
    resHistoPlusY = residualHistosPlusY_[ind];
    resHistoMinusY = residualHistosMinusY_[ind];
    predErrHisto = predErrHistos_[ind];
    predErrHistoFirst = predErrHistosFirst_[ind];
    predErrHistoSecond = predErrHistosSecond_[ind];
    hitErrHisto = hitErrHistos_[ind];
    hitErrHistoFirst = hitErrHistosFirst_[ind];
    hitErrHistoSecond = hitErrHistosSecond_[ind];
//     simRecHisto = simRecHistos_[ind];
//     simTrkHisto = simTrkHistos_[ind];
    dxdzHisto = dxdzHistos_[ind];
    posHistos[0] = radHistos_[ind];
    posHistos[1] = phiHistos_[ind];
    posHistos[2] = zHistos_[ind];
    posHistosLocal[0] = xHistos_[ind];
    posHistosLocal[1] = yHistos_[ind];
    doublePullHisto = doublePullHistos_[ind];
    hitPullHisto = hitPullHistos_[ind];
    predPullHisto = predPullHistos_[ind];
    // local x difference / sum for hits
    double deltaHit = hitX[0] + relSignX*hitX[1];
    // local x difference / sum for predicted states
    double deltaPred = predX[0] + relSignX*predX[1];
    double deltaSim = simX[0] + relSignX*simX[1];
#ifdef HISTOS2D
    resVsAngle = resVsAngleHistos_[ind];
    ddVsLocalX = ddVsLocalXHistos_[ind];
    ddVsLocalY = ddVsLocalYHistos_[ind];
    ddVsDxdz = ddVsDxdzHistos_[ind];
    ddVsDydz = ddVsDydzHistos_[ind];
    if (allPlots) {
      localXVsLocalY = localXVsLocalYHistos_[ind];
      dxdzVsDydz = dxdzVsDydzHistos_[ind];
      localXVsDxdz = localXVsDxdzHistos_[ind];
      localYVsDxdz = localYVsDxdzHistos_[ind];
      localXVsDydz = localXVsDydzHistos_[ind];
      localYVsDydz = localYVsDydzHistos_[ind];
      dPreddSimVsdHitdSim = dPreddSimVsdHitdSimHistos_[ind];
    }
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
//     if(simX[0]>-90 && simX[1]>-90){
//       //      if( fabs(deltaSim-deltaHit) < 0.04 ) {
//       simRecHisto->Fill(deltaSim-deltaHit);
//       simRecHistoAll->Fill(deltaSim-deltaHit);
//       //simRecMomentum->Fill((deltaSim-deltaHit)/(0.0136*0.16*3));
//       simRecMomentum->Fill((0.0136*0.16*3)/(deltaSim-deltaHit));
//       simTrkHisto->Fill(deltaSim-deltaPred);
//       //}
//     }
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
    if (allPlots){
      localXVsLocalY->Fill(predX[0],predY[0]);
      dxdzVsDydz->Fill(predDX[0],predDY[0]);
      localXVsDxdz->Fill(predDX[0],predX[0]);
      localYVsDxdz->Fill(predDX[0],predY[0]);
      localXVsDydz->Fill(predDY[0],predX[0]);
      localYVsDydz->Fill(predDY[0],predY[0]);
      dPreddSimVsdHitdSim->Fill(deltaSim-deltaPred,deltaSim-deltaHit);
    }    
    localY_[ind].push_back(predY[0]);
    localYE_[ind].push_back(predEY[0]);
    dxdz_[ind].push_back(predDX[0]);
    dxdzE_[ind].push_back(predEDX[0]);
    dd_[ind].push_back(deltaHit-deltaPred);
    ddE_[ind].push_back(predEDeltaX);
#endif

  }

  cout << "Entries after cut: "<<ctr<<endl;
  cout << "Pairs: "<<allIndices.size()<<endl;



  time (&end);
  double dif = difftime (end,start);
  printf ("It took  %.2lf seconds to loop through the events.\n", dif );
  start = end;

  // Summary histograms (one bin / pair)
  //   (units: micrometer)
  //
  unsigned n = allIndices.size();

  out->cd();
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
  hitErrMeans->GetYaxis()->SetTitle("#sigma(#Deltax_{hit}) [#mum]");
  dxdzMeans->GetYaxis()->SetTitle("Local dx/dz");
  layerHisto->GetYaxis()->SetTitle("Layer");
  stereoHisto->GetYaxis()->SetTitle("Stereo");
  radHisto->GetYaxis()->SetTitle("Radius [cm]");
  phiHisto->GetYaxis()->SetTitle("#phi");
  zHisto->GetYaxis()->SetTitle("z [cm]");
  xHisto->GetYaxis()->SetTitle("Local x [cm]");
  yHisto->GetYaxis()->SetTitle("Local y [cm]");
  statHisto->GetYaxis()->SetTitle("Entries");
  doublePulls->GetYaxis()->SetTitle("Pull DD");
  hitPulls->GetYaxis()->SetTitle("Pull (#Deltax_{hit})");
  predPulls->GetYaxis()->SetTitle("Pull (#Deltax_{pred})");

#ifdef HISTOS2D
  TH1F* ddVsLocalXSlope = new TH1F("ddXslope","ddXslope",n,-0.5,n-0.5);
  ddVsLocalXSlope->GetXaxis()->SetTitle("double diff vs local X slope");
  TH1F* ddVsLocalXOffset = new TH1F("ddXoffset","ddXoffset",n,-0.5,n-0.5);
  ddVsLocalXOffset->GetXaxis()->SetTitle("double diff vs local X offset");
  TH1F* ddVsLocalYSlope = new TH1F("ddYslope","ddYslope",n,-0.5,n-0.5);
  ddVsLocalYSlope->GetYaxis()->SetTitle("double diff vs local Y slope");
  TH1F* ddVsLocalYOffset = new TH1F("ddYoffset","ddYoffset",n,-0.5,n-0.5);
  ddVsLocalYOffset->GetYaxis()->SetTitle("double diff vs local Y offset");

  TH1F* ddVsDxdzSlope = new TH1F("ddDxslope","ddDxslope",n,-0.5,n-0.5);
  ddVsDxdzSlope->GetYaxis()->SetTitle("double diff vs local dX/dZ slope");
  TH1F* ddVsDxdzOffset = new TH1F("ddDxoffset","ddDxoffset",n,-0.5,n-0.5);
  ddVsDxdzOffset->GetYaxis()->SetTitle("double diff vs local dX/dZ offset");
  TH1F* ddVsDydzSlope = new TH1F("ddDyslope","ddDyslope",n,-0.5,n-0.5);
  ddVsDydzSlope->GetYaxis()->SetTitle("double diff vs local dy/dZ slope");
  TH1F* ddVsDydzOffset = new TH1F("ddDyoffset","ddDyoffset",n,-0.5,n-0.5);
  ddVsDydzOffset->GetYaxis()->SetTitle("double diff vs local dy/dZ offset");
#endif

  //
  // Filling of summary histograms (loop over final containers)
  //
  const int kNotDraw = 1<<9;

  time (&start);
   for ( unsigned int i=0; i<n; ++i ) {
//  for ( unsigned int i=0; i<1; ++i ) {
    //
      // Choose DetId order with more entries and retrieve histogram pointers
    //
    resHisto = residualHistos_[i];
      resHistoPlusX = residualHistosPlusX_[i];
      resHistoMinusX = residualHistosMinusX_[i];
      resHistoPlusY = residualHistosPlusY_[i];
      resHistoMinusY = residualHistosMinusY_[i];
    predErrHisto = predErrHistos_[i];
    predErrHistoFirst = predErrHistosFirst_[i];
    predErrHistoSecond = predErrHistosSecond_[i];
//     simRecHisto = simRecHistos_[i];
//     simTrkHisto = simTrkHistos_[i];
    hitErrHisto = hitErrHistos_[i];
    dxdzHisto = dxdzHistos_[i];
    posHistos[0] = radHistos_[i];
    posHistos[1] = phiHistos_[i];
    posHistos[2] = zHistos_[i];
    posHistosLocal[0] = xHistos_[i];
    posHistosLocal[1] = yHistos_[i];
    doublePullHisto = doublePullHistos_[i];
    hitPullHisto = hitPullHistos_[i];
    predPullHisto = predPullHistos_[i];
#ifdef HISTOS2D
    ddVsLocalX = ddVsLocalXHistos_[i];
    ddVsLocalY = ddVsLocalYHistos_[i];
    ddVsDxdz = ddVsDxdzHistos_[i];
    ddVsDydz = ddVsDydzHistos_[i];
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
    
    //     unsigned int id;
    decode(detIdPairs_[i].first,subdet,layer,part,stereo);
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
    if (resHisto->GetEntries()> threshold ) {
      // debug info on layers
      layerHisto->SetBinContent(i+1,k);
      stereoHisto->SetBinContent(i+1,stereo);
      // statistics
      statHisto->SetBinContent(i+1,resHisto->GetEntries());
      // global position
      fillMean(i+1,radHisto,posHistos[0]);
      fillMean(i+1,phiHisto,posHistos[1]);
      fillMean(i+1,zHisto,posHistos[2]);
      fillMean(i+1,xHisto,posHistosLocal[0]);
      fillMean(i+1,yHisto,posHistosLocal[1]);
      hDir->cd();

      // Do this only if <10% of the hits are inside the area
//       cout << resHisto->GetBinContent(0) <<" "<<resHisto->GetBinContent(resHisto->GetNbinsX()+1) <<" "<<resHisto->GetEntries() <<" "<<
// ( (resHisto->GetBinContent(0)+resHisto->GetBinContent(resHisto->GetNbinsX()+1)) / resHisto->GetEntries())
// <<endl;
      if (( (resHisto->GetBinContent(0)+resHisto->GetBinContent(resHisto->GetNbinsX()+1)) / resHisto->GetEntries()) < 0.1) {

#ifdef HISTOS2D
	// Now the plots on the slopes dd vs. local pos.
	AdjacentType type = checkAdjacentType(detIdPairs_[i].first, detIdPairs_[i].second);
	if (type == parallelToStrips) {
	  fillSlope(i+1,ddVsLocalYSlope, ddVsLocalYOffset, localY_[i], localYE_[i], dd_[i], ddE_[i]);
	} else {
	  fillSlope(i+1,ddVsDxdzSlope, ddVsDxdzOffset, dxdz_[i], dxdzE_[i], dd_[i], ddE_[i]);
	  fillSlope(i+1,ddVsLocalXSlope, ddVsLocalXOffset, ddVsLocalX);
	}

	fillSlope(i+1,ddVsDydzSlope, ddVsDydzOffset, ddVsDydz);
#endif

	curDir->cd();
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
      } else {
        cout << "Overflow/underflow problem with overlap ID "<<i<<endl;
	cout << "DD summary plots will not be filled\n";
      }
      // predicted error // fold in some uncertainty from the spread (systematic)
      fillMean(i+1,predErrMeans,predErrHisto,10000.);
      // fillMeanWithSyst(i+1,predErrMeans,predErrHisto,predErrHistoFirst,10000.);
      //predicted error for each overlap module individually
      fillMean(i+1,predErrMeansFirst,predErrHistoFirst,10000.);
      fillMean(i+1,predErrMeansSecond,predErrHistoSecond,10000.);
//       fillWidth(i+1,simRec,simRecHisto,10000);
//       fillWidth(i+1,simTrk,simTrkHisto,10000);
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
//             std::cout << "Index: " << i << "   Layer: " << k << "   Phi: " << posHistos[1]->GetMean() << "   r: " << posHistos[0]->GetMean() << "   z: " << posHistos[2]->GetMean() << "  dx/dz: " << dxdzHisto->GetMean() << "   detID = " << detIdPairs_[i].first << " and " << detIdPairs_[i].second << std::endl;
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

//       if(k==5)   { width2VsAngle_TOB1->Fill(dxdzHisto->GetMean(),sDiff*sDiff);
// 	predErr2VsAngle_TOB1->Fill(dxdzHisto->GetMean(),predErrHisto->GetMean()*predErrHisto->GetMean());
//       hitErr2VsAngle_TOB1->Fill(dxdzHisto->GetMean(),hitErrHisto->GetMean()*hitErrHisto->GetMean());
//       alphaVsAngle_TOB1->Fill(dxdzHisto->GetMean(),sqrt((sDiff*sDiff-predErrHisto->GetMean()*predErrHisto->GetMean())/hitErrHisto->GetMean()*hitErrHisto->GetMean()));
//     }
//       if(k==6)   { width2VsAngle_TOB2->Fill(dxdzHisto->GetMean(),sDiff*sDiff);
// 	predErr2VsAngle_TOB2->Fill(dxdzHisto->GetMean(),predErrHisto->GetMean()*predErrHisto->GetMean());
//       hitErr2VsAngle_TOB2->Fill(dxdzHisto->GetMean(),hitErrHisto->GetMean()*hitErrHisto->GetMean());
//       alphaVsAngle_TOB2->Fill(dxdzHisto->GetMean(),sqrt((sDiff*sDiff-predErrHisto->GetMean()*predErrHisto->GetMean())/hitErrHisto->GetMean()*hitErrHisto->GetMean()));
//     }
//       if(k==7)   { width2VsAngle_TOB3->Fill(dxdzHisto->GetMean(),sDiff*sDiff);
// 	predErr2VsAngle_TOB3->Fill(dxdzHisto->GetMean()*dxdzHisto->GetMean(),predErrHisto->GetMean()*predErrHisto->GetMean());
//       hitErr2VsAngle_TOB3->Fill(dxdzHisto->GetMean(),hitErrHisto->GetMean()*hitErrHisto->GetMean());
//       alphaVsAngle_TOB3->Fill(dxdzHisto->GetMean(),sqrt((sDiff*sDiff-predErrHisto->GetMean()*predErrHisto->GetMean())/hitErrHisto->GetMean()*hitErrHisto->GetMean()));
//     }
//       if(k==8)   {
// 	width2VsAngle_TOB4->Fill(dxdzHisto->GetMean(),sDiff*sDiff);
// 	predErr2VsAngle_TOB4->Fill(dxdzHisto->GetMean(),predErrHisto->GetMean()*predErrHisto->GetMean());
// 	hitErr2VsAngle_TOB4->Fill(dxdzHisto->GetMean(),hitErrHisto->GetMean()*hitErrHisto->GetMean());
// 	alphaVsAngle_TOB4->Fill(dxdzHisto->GetMean(),sqrt((sDiff*sDiff-predErrHisto->GetMean()*predErrHisto->GetMean())/hitErrHisto->GetMean()*hitErrHisto->GetMean()));
//       }
    }
  }
  time (&end);
  dif = difftime (end,start);
  printf ("It took  %.2lf seconds to do the fits.\n", dif );
  start=end;
  out->Write();
  time (&end);
  dif = difftime (end,start);
  printf ("It took  %.2lf seconds to save the plots.\n", dif );
  start=end;
  //out->Close();

//   time (&end);
//   dif = difftime (end,start);
//   printf ("It took  %.2lf seconds to close the file.\n", dif );

}





bool
OverlapHistos::cut() const
{
  // cut on 30deg local angle in x (any of the two modules)
  //if ( fabs(predDX[0])>0.6 || fabs(predDX[1])>0.6 )  continue;
  // extrapolation accuracy
  if ( predEDeltaX<1.e-9 || predEDeltaX>0.005 ) return false;
  // chi2 cut
  //if ( TMath::Prob(chi2,2*found-4)<1.e-3 )  continue;
  if ( chi2[0] >200 ) return false;

  return true;
}

void
OverlapHistos::showPair (unsigned int i) const
{
  std::cout << "DetPair subdet / layer / stereo = " << std::endl;
  if ( i<detIdPairs_.size() ) {
    unsigned int subdet;
    unsigned int layer;
    unsigned int stereo;
    int part;
    decode(detIdPairs_[i].first,subdet,layer,part,stereo);
    std::cout << " " << subdet << " " << layer << " " << stereo << std::endl;
    decode(detIdPairs_[i].second,subdet,layer,part,stereo);
    std::cout << " " << subdet << " " << layer << " " << stereo << std::endl;
  }
  else {
    std::cout << "  -" << std::endl;
    std::cout << "  -" << std::endl;
  }
}


// TH1*
// OverlapHistos::residualHistogram (unsigned int i, bool first) const
// {
//   int j = first ? 0 : 1;
//   if ( i<detIdPairs_.size() ) 
//     return residualHistos_[j][i];
//   else
//     return 0;
// }

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
