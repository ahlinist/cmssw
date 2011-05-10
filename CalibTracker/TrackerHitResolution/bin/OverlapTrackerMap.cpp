#include <iostream>
#include <cassert>
#include <TROOT.h>
#include <TSystem.h>
#include <Cintex/Cintex.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>
#include <TGraph.h>
#include <TF1.h>
#include <TFormula.h>
#include <TStyle.h>
#include <TKey.h>
#include <vector>
#include "TLegend.h"

#include "CommonTools/TrackerMap/interface/TrackerMap.h"
#include "CommonTools/TrackerMap/interface/TmModule.h"

#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "TopQuarkAnalysis/TopTools/interface/LRHelpFunctions.h"
#include "CalibTracker/TrackerHitResolution/bin/overlapHelper.C"

using namespace std;
    TH1* resHistos;
    TH1* resHistosY;
    TH1* predErrHistos;
    TH1* predErrHistosFirst;
    TH1* predErrHistosSecond;
    TH1* hitErrHistos;
    TH1* hitErrHistosFirst;
    TH1* hitErrHistosSecond;
    TH1* simRecHistos;
    TH1* simTrkHistos;
    TH1* dxdzHistos;
    TH1* posHistos[3];
    TH1* posHistosLocal[2];
    TH1* doublePullHistos;
    TH1* hitPullHistos;
    TH1* predPullHistos;
#ifdef HISTOS2D
    TH2* resVsAngleHistos;
    TH2* ddVsLocalXHistos;
    TH2* ddVsLocalYHistos;
    TH2* ddVsDxdzHistos;
    TH2* ddVsDydzHistos;
    TH2* localXVsLocalYHistos;
    TH2* dxdzVsDydzHistos;
    TH2* localXVsDxdzHistos;
    TH2* localYVsDxdzHistos;
    TH2* localXVsDydzHistos;
    TH2* localYVsDydzHistos;
    TH2* dPreddSimVsdHitdSimHistos;
#endif

  TH1* predErrMeans;
  TH1* predErrMeansFirst;
  TH1* predErrMeansSecond;
  TH1* simRec;
  TH1* simTrk;
  TH1F* meanDiffs, *meanDiffsY;
  TH1F* sigmaDiffs, *sigmaDiffsY;
  TH1* hitErrMeans;
  TH1* dxdzMeans;
  TH1* layerHisto;
  TH1* stereoHisto;
  TH1* radHisto;
  TH1* phiHisto;
  TH1* zHisto;
  TH1* xHisto;


  TH1* yHisto;


  TH1* statHisto;
  TH1* doublePulls;
  TH1* hitPulls;
  TH1* predPulls;  
  TH1* ddXslope; 
  TH1* ddYslope;
  TH1* ddDxslope;
  TH1* dydzMeans;

//TH1* TrackAngle;
//TH2* TrackAnglevsClusterWidth;

  // Decleration of legend
  TLegend* leg;
  TLegend* leg_histTIB12;
  TLegend* leg_histTIB34;
  TLegend* leg_histTOB1234;
  TLegend* leg_histTOB56;


   // Declaration of leaf types
   UInt_t          detids[2];
   double TrackAngle;
   double ClusterWidths;

   // List of branches
   TBranch        *b_id;   //!
   TBranch TrackAngles;
   TBranch ClusterWidth;

   TDirectory* baseDir;
   typedef std::pair<unsigned int, unsigned int> DetIdPair;
   typedef pair<int,float> IntFloatPair;
   typedef map<int, IntFloatPair> DetInfoMap;
   DetInfoMap parMap, perpMap;
   DetInfoMap parMapY, perpMapY;

void loadSummaryPlots();
void setTDRStyle();
//void fillAll(string dataFilename, TrackerMap* tmapPar, TrackerMap* tmapPerp, bool doY = false);
void fillAll(int i,  DetIdPair idPair, TrackerMap* tmapPar, TrackerMap* tmapPerp, double val, bool doY = false );
void fillMap(DetInfoMap & theMap, int detId, double shift);


int main(int argc, const char* argv[]) { 
  cout << "Hello world!\n";

  TString InputDir =  argv[1];
  const char* name = argv[2];
  std::string OutPutDir = argv[3];
  TString outputDir = OutPutDir;
  cout<<"Output dir = "<<OutPutDir<<endl;

  TFile* theFile = new TFile (InputDir + name + TString(".root")) ;
  baseDir = gDirectory;
  TTree *tree = (TTree*)gDirectory->Get("Overlaps");
  tree->SetBranchAddress("detids", detids, &b_id);

  loadSummaryPlots();
  setTDRStyle();

  TrackerMap* tmap=new TrackerMap("CMS 7TeV 2010");
  
  TrackerMap* tmapPar   =new TrackerMap("Relative shift (overlap parallel to Z)");
  TrackerMap* tmapPerp  =new TrackerMap("Relative shift (overlap perpendicular to Z)");
  TrackerMap* tmapParY  =new TrackerMap("Relative shift in Y (overlap parallel to Z)");
  TrackerMap* tmapPerpY =new TrackerMap("Relative shift in Y (overlap perpendicular to Z)");
  
  //////////////////////////////////////////
  //
  //  FOR HIT RESOLUTION ......
  //
  //
  //
  //
  //
  /////////////////////////////////////////

  cout << "Calculating Hit Resolutions....."<<endl;

  int n_tot = layerHisto->GetEntries();
  int n_TIB12 = 0;
  int n_TIB34 = 0;
  int n_TOB1234 = 0;
  int n_TOB56 = 0;

  float alignment_cut = 0.0008;
  float momentum_cut = 5.0;
  float path_median = 4.0;
  
  for (int i = 1; i<=n_tot; i++){
    //cout << "==========================" << endl;
    float alignX = ddXslope->GetBinContent(i)*xHisto->GetBinError(i);
    float alignY = ddYslope->GetBinContent(i)*yHisto->GetBinError(i);

    float alignDX = ddDxslope->GetBinContent(i)*dxdzMeans->GetBinError(i);
    float alignDY = ddDxslope->GetBinContent(i)*dydzMeans->GetBinError(i);

    //cout << " values of alignX and Y = " << alignX << " and " << alignY << endl;
    //cout << " values of alignDX and Y = " << alignDX << " and " << alignDY << endl;

    if ( fabs(alignX)<alignment_cut && fabs(alignY)<alignment_cut && fabs(alignDX)<alignment_cut && fabs(alignDY)<alignment_cut) {
      
      if (layerHisto->GetBinContent(i)==1||layerHisto->GetBinContent(i)==2) n_TIB12++;
      if (layerHisto->GetBinContent(i)==3||layerHisto->GetBinContent(i)==4) n_TIB34++;
      if (layerHisto->GetBinContent(i)==5||layerHisto->GetBinContent(i)==6||
	  layerHisto->GetBinContent(i)==7||layerHisto->GetBinContent(i)==8) n_TOB1234++;
      if (layerHisto->GetBinContent(i)==9||layerHisto->GetBinContent(i)==10) n_TOB56++;
    } 
    //else cout << "cutting from misalignment: " << alignX << " or " << alignY << " or " << alignDX << " or " << alignDY << endl;
  }  

  cout << "number of entries: TIB12 = " << n_TIB12 << " TIB34 = " << n_TIB34 << 
      " TOB1234 = " << n_TOB1234 << " TOB56 = " << n_TOB56 << endl;

  float widthm, widthe;
  float predm, prede;
  float hitm, hite;
  float hitmdata, hitedata;
  float ratio, ratioe;
  float slopeX, slopeY;
  float rangeX, rangeY;


  TH1F* hitresTIB12 = new TH1F("hitresTIB12", "hit resolution TIB12", n_TIB12, -0.5, n_TIB12-0.5);
  TH1F* hitsigmaTIB12 = new TH1F("hitsigTIB12", "hit position uncertainty", n_TIB12, -0.5, n_TIB12-0.5);
  TH1F* ratio_hTIB12 = new TH1F("ratioTIB12", "ratio TIB12", n_TIB12, -0.5, n_TIB12-0.5);
  TH1F* ddTIB12 = new TH1F("ddTIB12", "double difference width TIB12", n_TIB12, -0.5, n_TIB12-0.5);
  TH1F* predTIB12 = new TH1F("predTIB12", "mean pred uncertainty TIB12", n_TIB12, -0.5, n_TIB12-0.5);

  TH1F* hitresTIB34 = new TH1F("hitresTIB34", "hit resolution TIB34", n_TIB34, -0.5, n_TIB34-0.5);
  TH1F* hitsigmaTIB34 = new TH1F("hitsigTIB34", "hit position uncertainty", n_TIB34, -0.5, n_TIB34-0.5);
  TH1F* ratio_hTIB34 = new TH1F("ratioTIB34", "ratio TIB34", n_TIB34, -0.5, n_TIB34-0.5);

  TH1F* hitresTOB1234 = new TH1F("hitresTOB1234", "hit resolution TOB1234", n_TOB1234, -0.5, n_TOB1234-0.5);
  TH1F* hitsigmaTOB1234 = new TH1F("hitsigTOB1234", "hit position uncertainty", n_TOB1234, -0.5, n_TOB1234-0.5);
  TH1F* ratio_hTOB1234 = new TH1F("ratioTOB1234", "ratio TOB1234", n_TOB1234, -0.5, n_TOB1234-0.5);

  TH1F* hitresTOB56 = new TH1F("hitresTOB56", "hit resolution TOB56", n_TOB56, -0.5, n_TOB56-0.5);
  TH1F* hitsigmaTOB56 = new TH1F("hitsigTOB56", "hit position uncertainty", n_TOB56, -0.5, n_TOB56-0.5);
  TH1F* ratio_hTOB56 = new TH1F("ratioTOB56", "ratio TOB56", n_TOB56, -0.5, n_TOB56-0.5);
  TH1F* hitres1DTIB12 = new TH1F("hitres1DTIB12", "hit resolution TIB12", 50, 0, 100);
  TH1F* hitres1DTIB34 = new TH1F("hitres1DTIB34", "hit resolution TIB34", 50, 0, 100);
  TH1F* hitres1DTOB1234 = new TH1F("hitres1DTOB1234", "hit resolution TOB1234", 50, 0, 100);
  TH1F* hitres1DTOB56 = new TH1F("hitres1DTOB56", "hit resolution TOB56", 100, 0, 100);
  TH1F* pred1DTIB12 = new TH1F("pred1DTIB12", "predicted CPE  TIB12", 100, 0, 100);
  TH1F* pred1DTIB34 = new TH1F("pred1DTIB34", "predicted CPE  TIB34", 100, 0, 100);
  TH1F* pred1DTOB1234 = new TH1F("pred1DTOB1234", "predicted CPE  TOB1234", 100, 0, 100);
  TH1F* pred1DTOB56 = new TH1F("pred1DTOB56", "predicted CPE  TOB56", 100, 0, 100);

  TH1F* TrackAngleTOB56 = new TH1F("TrackAngleTOB56", "TrackAngle  TOB56", 400, -0.75, 0.75);
  TH1F* TrackAngleTOB1234 = new TH1F("TrackAngleTOB1234", "TrackAngle  TOB1234", 400, -0.75, 0.75);
  TH1F* TrackAngleTIB12 = new TH1F("TrackAngleTIB12", "TrackAngle  TIB12", 400, -0.75, 0.75);
  TH1F* TrackAngleTIB34 = new TH1F("TrackAngleTIB34", "TrackAngle  TIB34", 400, -0.75, 0.75);

  TH2F* TrackAnglevsClusterWidthTOB56 = new TH2F("TrackAnglevsClusterWidthTOB56", "TrackAngle vs Cluster Width  TOB56", 100, 0, 10, 400, -0.75, 0.75);
  TH2F* TrackAnglevsClusterWidthTOB1234 = new TH2F("TrackAnglevsClusterWidthTOB1234", "TrackAngle vs Cluster Width TOB1234", 100, 0, 10, 400, -0.75, 0.75);
  TH2F* TrackAnglevsClusterWidthTIB12 = new TH2F("TrackAnglevsClusterWidthTIB12", "TrackAngle vs Cluster Width TIB12", 100, 0, 10, 400, -0.75, 0.75);
  TH2F* TrackAnglevsClusterWidthTIB34 = new TH2F("TrackAnglevsClusterWidthTIB34", "TrackAngle vs Cluster Width TIB34", 100, 0, 10, 400, -0.75, 0.75);

  int iTIB12 = 1;
  int iTIB34 = 1;
  int iTOB1234 = 1;
  int iTOB56 = 1;

  for (int i = 1; i<=n_tot; i++){
    widthm = (sigmaDiffs->GetBinContent(i));
    widthe = (sigmaDiffs->GetBinError(i));
    predm = predErrMeans->GetBinContent(i);
    prede = predErrMeans->GetBinError(i);
    hitm =  hitErrMeans->GetBinContent(i);
    hite =  hitErrMeans->GetBinError(i);
    slopeX = ddXslope->GetBinContent(i);
    slopeY = ddYslope->GetBinContent(i);
    rangeX = xHisto->GetBinError(i);
    rangeY = yHisto->GetBinError(i);

    float alignX = ddXslope->GetBinContent(i)*xHisto->GetBinError(i);
    float alignY = ddYslope->GetBinContent(i)*yHisto->GetBinError(i);

    float alignDX = ddDxslope->GetBinContent(i)*dxdzMeans->GetBinError(i);
    float alignDY = ddDxslope->GetBinContent(i)*dydzMeans->GetBinError(i);

    //cout << "slopeX = " << slopeX << " and rangeX = " << rangeX << ", slopeY = " << slopeY << " and rangeY = " << rangeY << "               " << slopeX*rangeX << " and " << slopeY*rangeY <<   endl;
    
    if ( fabs(alignX)>alignment_cut || fabs(alignY)>alignment_cut || fabs(alignDX)>alignment_cut || fabs(alignDY)>alignment_cut ) {
      continue;
    }

    cout << "  errors layer " << layerHisto->GetBinContent(i) << ": stat on width = " << widthe;

    //add on an extra uncertainty to the fitted width because of the alignment
    //take the single biggest misalignment uncertainty
    float alignUncert = fabs(alignX);
    if ( fabs(alignY) > alignUncert ) alignUncert = fabs(alignY);
    if ( fabs(alignDX) > alignUncert ) alignUncert = fabs(alignDX);
    if ( fabs(alignDY) > alignUncert ) alignUncert = fabs(alignDY);
    alignUncert = alignUncert*10000;

    cout << " align = " << alignUncert;
    widthe = sqrt(widthe*widthe+(alignUncert*alignUncert));
    
    cout << " stat on prediction = " << prede;
    //add on extra uncertainty to predicted mean from tracking due to mulple scattering uncertainty
    float msUncert = ((0.0136/momentum_cut)*sqrt(0.025)*path_median)*10000;
    prede = sqrt(prede*prede+(msUncert*msUncert));

    cout << " MS = " << msUncert;
    
    if (widthm>predm) {
      hitmdata = sqrt(widthm*widthm-predm*predm);
      hitedata = sqrt(widthe*widthe+prede*prede);
    } else {
      cout << "BAD point: pred mean > dd width with entry = " << i << " and mean = "<< predm << " and widthm = " << widthm << endl;
      //hitmdata = -1000.0;
      //hitedata = -1000.0;
      //break;
    }

    cout << " total = " << hitedata << endl;     

    //ratio = hitm/hitmdata;
    ratio = hitmdata/hitm;
    ratioe = sqrt( ((hite/hitm)*(hite/hitm)) + ((hitedata/hitmdata)*(hitedata/hitmdata)) )*ratio;

    //////////////////////////////////////////
    //
    //  FOR TRACKER MAP ......
    //
    //////////////////////////////////////////

    tree->GetEntry(i-1);
    tmap->fill(detids[0],hitmdata/sqrt(2));
    tmap->fill(detids[1],hitmdata/sqrt(2));

    DetIdPair idPair(detids[0], detids[1]);
    //DetInfoMap parMap, perpMap;
    
    AdjacentType type = checkAdjacentType(idPair.first, idPair.second);
    double res = hitmdata/sqrt(2);
    
    //if (doY) res = meanDiffsY->GetBinContent(i);
    //else res = meanDiffs->GetBinContent(i);
   
    if (type == parallelToStrips) {
      fillMap(parMap, idPair.first, res);
      fillMap(parMap, idPair.second, res);
      fillMap(parMapY, idPair.first, res);
      fillMap(parMapY, idPair.second, res);
    } else {
      fillMap(perpMap, idPair.first, res);
      fillMap(perpMap, idPair.second, res);
      fillMap(perpMapY, idPair.first, res);
      fillMap(perpMapY, idPair.second, res);
    }

    //fillAll(i, idPair, tmapPar, tmapPerp, val);
    //fillAll(i, idPair, tmapParY, tmapPerpY, val, true);
    

    if (layerHisto->GetBinContent(i)==1||layerHisto->GetBinContent(i)==2){
      hitresTIB12->SetBinContent(iTIB12,hitmdata/sqrt(2));
      hitresTIB12->SetBinError(iTIB12,hitedata/sqrt(2));
      hitres1DTIB12->Fill(hitmdata/sqrt(2));
      pred1DTIB12->Fill(hitm/sqrt(2));
      hitsigmaTIB12->SetBinContent(iTIB12,hitm/sqrt(2));
      hitsigmaTIB12->SetBinError(iTIB12,hite/sqrt(2));
      ratio_hTIB12->SetBinContent(iTIB12,ratio);
      ratio_hTIB12->SetBinError(iTIB12,ratioe);
      ddTIB12->SetBinContent(iTIB12,widthm/sqrt(2));
      ddTIB12->SetBinError(iTIB12,widthe/sqrt(2));
      predTIB12->SetBinContent(iTIB12, predm/sqrt(2));
      predTIB12->SetBinError(iTIB12, prede/sqrt(2));
      
      TrackAngleTIB12 ->Fill(TrackAngle);
      TrackAnglevsClusterWidthTIB12 ->Fill(ClusterWidths, TrackAngle);

      iTIB12++;
    }
    
    if (layerHisto->GetBinContent(i)==3||layerHisto->GetBinContent(i)==4){
      //cout << "ratio = " << ratio << " ratio error = " << ratioe << " hitm = " << hitm << "+-" << hite << " hit data = " << hitmdata << "+-" << hitedata << endl;
    //cout << "ratio fractional error = " << ratioe/ratio << " and data fractional error = " << hitedata/hitmdata << " ratio error = " << ratio*hitedata/hitmdata << endl;
      hitresTIB34->SetBinContent(iTIB34,hitmdata/sqrt(2));
      hitresTIB34->SetBinError(iTIB34,hitedata/sqrt(2));
      hitres1DTIB34->Fill(hitmdata/sqrt(2));
      pred1DTIB34->Fill(hitm/sqrt(2));
      hitsigmaTIB34->SetBinContent(iTIB34,hitm/sqrt(2));
      hitsigmaTIB34->SetBinError(iTIB34,hite/sqrt(2));
      ratio_hTIB34->SetBinContent(iTIB34,ratio);
      ratio_hTIB34->SetBinError(iTIB34,ratioe);
      
      TrackAngleTIB34 ->Fill(TrackAngle);
      TrackAnglevsClusterWidthTIB34 ->Fill(ClusterWidths, TrackAngle);
      
      iTIB34++;
    }
    
    if (layerHisto->GetBinContent(i)==5||layerHisto->GetBinContent(i)==6||layerHisto->GetBinContent(i)==7||layerHisto->GetBinContent(i)==8){
      hitresTOB1234->SetBinContent(iTOB1234,hitmdata/sqrt(2));
      hitresTOB1234->SetBinError(iTOB1234,hitedata/sqrt(2));
      hitres1DTOB1234->Fill(hitmdata/sqrt(2));
      cout << "hitresTOP1234 ent = "<<hitres1DTOB1234->GetBinContent(i)<<endl;
      pred1DTOB1234->Fill(hitm/sqrt(2));
      hitsigmaTOB1234->SetBinContent(iTOB1234,hitm/sqrt(2));
      hitsigmaTOB1234->SetBinError(iTOB1234,hite/sqrt(2));
      ratio_hTOB1234->SetBinContent(iTOB1234,ratio);
      ratio_hTOB1234->SetBinError(iTOB1234,ratioe);

      TrackAngleTOB1234 ->Fill(TrackAngle);
      TrackAnglevsClusterWidthTOB1234 ->Fill(ClusterWidths, TrackAngle);

      iTOB1234++;
    }
    
    if (layerHisto->GetBinContent(i)==9||layerHisto->GetBinContent(i)==10){

      hitresTOB56->SetBinContent(iTOB56,hitmdata/sqrt(2));
      hitresTOB56->SetBinError(iTOB56,hitedata/sqrt(2));
      hitres1DTOB56->Fill(hitmdata/sqrt(2));
      pred1DTOB56->Fill(hitm/sqrt(2));
      hitsigmaTOB56->SetBinContent(iTOB56,hitm/sqrt(2));
      hitsigmaTOB56->SetBinError(iTOB56,hite/sqrt(2));
      ratio_hTOB56->SetBinContent(iTOB56,ratio);
      ratio_hTOB56->SetBinError(iTOB56,ratioe);

      TrackAngleTOB56 ->Fill(TrackAngle);
      TrackAnglevsClusterWidthTOB56 ->Fill(ClusterWidths, TrackAngle);

      iTOB56++;
    }
  }


  ofstream myfile;
  myfile.open (OutPutDir + TString("/") + name + TString(".txt"));


  //TrackAngle 
  //TrackAnglevsClusterWidth







  cout << "12, 34, 1234, 56 = "<<TrackAngleTIB12->GetEntries()<<", "<<TrackAngleTIB34->GetEntries()<<", "<<TrackAngleTOB1234->GetEntries()<<", "<<TrackAngleTOB56->GetEntries()<<endl;
  cout << "12, 34, 1234, 56 Mean = "<<TrackAngleTIB12->GetMean()<<", "<<TrackAngleTIB34->GetMean()<<", "<<TrackAngleTOB1234->GetMean()<<", "<<TrackAngleTOB56->GetMean()<<endl;
  cout << "12, 34, 1234, 56 (2D)= "<<TrackAnglevsClusterWidthTIB12->GetEntries()<<", "<<TrackAnglevsClusterWidthTIB34->GetEntries()<<", "<<TrackAnglevsClusterWidthTOB1234->GetEntries()<<", "<<TrackAnglevsClusterWidthTOB56->GetEntries()<<endl; 
cout << "12, 34, 1234, 56 (2D)= "<<TrackAnglevsClusterWidthTIB12->GetMean()<<", "<<TrackAnglevsClusterWidthTIB34->GetMean()<<", "<<TrackAnglevsClusterWidthTOB1234->GetMean()<<", "<<TrackAnglevsClusterWidthTOB56->GetMean()<<endl;   
  

  TCanvas * c2 = new TCanvas("canvas2","canvas2",800,600);
  c2->cd();
  TrackAngleTIB12->Draw();
  TrackAngleTIB12->GetXaxis()->SetTitle("Track Angle");
  c2->SaveAs(outputDir + TString("/TrackAngleTIB12_")+name+TString(".png"));  
  c2->SaveAs(outputDir + TString("/TrackAngleTIB12_")+name+TString(".eps"));  

  TCanvas * c3 = new TCanvas("canvas3","canvas3",800,600);
  c3->cd();
  TrackAnglevsClusterWidthTIB12->Draw();
  TrackAnglevsClusterWidthTIB12->GetXaxis()->SetTitle("Cluster Width");
  TrackAnglevsClusterWidthTIB12->GetYaxis()->SetTitle("Track Angle");
  c3->SaveAs(outputDir + TString("/TrackAnglevsClusterWidthTIB12_")+name+TString(".eps"));
  c3->SaveAs(outputDir + TString("/TrackAnglevsClusterWidthTIB12_")+name+TString(".png"));

  TCanvas * c4 = new TCanvas("canvas4","canvas4",800,600);
  c4->cd();
  TrackAngleTIB34->Draw();
  TrackAngleTIB34->GetXaxis()->SetTitle("Track Angle");
  c4->SaveAs(outputDir + TString("/TrackAngleTIB34_")+name+TString(".png"));  
  c4->SaveAs(outputDir + TString("/TrackAngleTIB34_")+name+TString(".eps"));  

  TCanvas * c5 = new TCanvas("canvas5","canvas5",800,600);
  c5->cd();
  TrackAnglevsClusterWidthTIB34->Draw();
  TrackAnglevsClusterWidthTIB34->GetXaxis()->SetTitle("Cluster Width");
  TrackAnglevsClusterWidthTIB34->GetYaxis()->SetTitle("Track Angle");
  c5->SaveAs(outputDir + TString("/TrackAnglevsClusterWidthTIB34_")+name+TString(".eps"));
  c5->SaveAs(outputDir + TString("/TrackAnglevsClusterWidthTIB34_")+name+TString(".png"));


  TCanvas * c6 = new TCanvas("canvas6","canvas6",800,600);
  c6->cd();
  TrackAngleTOB1234->Draw();
  TrackAngleTOB1234->GetXaxis()->SetTitle("Track Angle");
  c6->SaveAs(outputDir + TString("/TrackAngleTOB1234_")+name+TString(".png"));  
  c6->SaveAs(outputDir + TString("/TrackAngleTOB1234_")+name+TString(".eps"));  

  TCanvas * c7 = new TCanvas("canvas7","canvas7",800,600);
  c7->cd();
  TrackAnglevsClusterWidthTOB1234->Draw();
  TrackAnglevsClusterWidthTOB1234->GetXaxis()->SetTitle("Cluster Width");
  TrackAnglevsClusterWidthTOB1234->GetYaxis()->SetTitle("Track Angle");
  c7->SaveAs(outputDir + TString("/TrackAnglevsClusterWidthTOB1234_")+name+TString(".eps"));
  c7->SaveAs(outputDir + TString("/TrackAnglevsClusterWidthTOB1234_")+name+TString(".png"));

  TCanvas * c8 = new TCanvas("canvas8","canvas8",800,600);
  c8->cd();
  TrackAngleTOB56->Draw();
  TrackAngleTOB56->GetXaxis()->SetTitle("Track Angle");
  c8->SaveAs(outputDir + TString("/TrackAngleTOB56_")+name+TString(".png"));  
  c8->SaveAs(outputDir + TString("/TrackAngleTOB56_")+name+TString(".eps"));  

  TCanvas * c9 = new TCanvas("canvas9","canvas9",800,600);
  c9->cd();
  TrackAnglevsClusterWidthTOB56->Draw();
  TrackAnglevsClusterWidthTOB56->GetXaxis()->SetTitle("Cluster Width");
  TrackAnglevsClusterWidthTOB56->GetYaxis()->SetTitle("Track Angle");
  c9->SaveAs(outputDir + TString("/TrackAnglevsClusterWidthTOB56_")+name+TString(".eps"));
  c9->SaveAs(outputDir + TString("/TrackAnglevsClusterWidthTOB56_")+name+TString(".png"));




  TCanvas * c1 = new TCanvas("canvas1","canvas1",800,600);


  //hitres1DTIB12->Draw();
  if (hitres1DTIB12->GetEntries() != 0 ){
    hitres1DTIB12->Fit("gaus");
    myfile << "hitres1dTIB12: Resolution [um] = " <<  hitres1DTIB12->GetFunction("gaus")->GetParameter(1) << " +/- " <<  hitres1DTIB12->GetFunction("gaus")->GetParameter(2) <<endl;
    c1->SaveAs(outputDir + TString("/hitres1DTIB12_")+name+TString(".png"));  
    c1->SaveAs(outputDir + TString("/hitres1DTIB12_")+name+TString(".eps"));  
  }

  //hitres1DTIB34->Draw();
  if (hitres1DTIB34->GetEntries() != 0 ){
    hitres1DTIB34->Fit("gaus");
    myfile << "hitres1dTIB34: Resolution [um] = " <<  hitres1DTIB34->GetFunction("gaus")->GetParameter(1) << " +/- " <<  hitres1DTIB34->GetFunction("gaus")->GetParameter(2) <<endl;
    c1->SaveAs(outputDir + TString("/hitres1DTIB34_")+name+TString(".png"));  
    c1->SaveAs(outputDir + TString("/hitres1DTIB34_")+name+TString(".eps"));  
  }

  //hitres1DTOB1234->Draw();
  if (hitres1DTOB1234->GetEntries() != 0 ){
    hitres1DTOB1234->Fit("gaus");
    myfile << "hitres1dTOB1234: Resolution [um] = " <<  hitres1DTOB1234->GetFunction("gaus")->GetParameter(1) << " +/- " <<  hitres1DTOB1234->GetFunction("gaus")->GetParameter(2) <<endl;
    c1->SaveAs(outputDir + TString("/hitres1DTOB1234_")+name+TString(".png"));  
    c1->SaveAs(outputDir + TString("/hitres1DTOB1234_")+name+TString(".eps"));  
  }

  //hitres1DTOB56->Draw();
  if (hitres1DTOB56->GetEntries() != 0 ){
    hitres1DTOB56->Fit("gaus");
    myfile << "hitres1dTOB56: Resolution [um] = " <<  hitres1DTOB56->GetFunction("gaus")->GetParameter(1) << " +/- " <<  hitres1DTOB56->GetFunction("gaus")->GetParameter(2) <<endl;
    c1->SaveAs(outputDir + TString("/hitres1DTOB56_")+name+TString(".png")); 
    c1->SaveAs(outputDir + TString("/hitres1DTOB56_")+name+TString(".eps")); 
  }
  
  //pred1DTIB12->Draw();
  pred1DTIB12->Fit("gaus");
  c1->SaveAs(outputDir + TString("/pred1DTIB12_")+name+TString(".png"));
  c1->SaveAs(outputDir + TString("/pred1DTIB12_")+name+TString(".eps")); 

  //pred1DTIB34->Draw();
  pred1DTIB34->Fit("gaus");
  c1->SaveAs(outputDir + TString("/pred1DTIB34_")+name+TString(".png"));
  c1->SaveAs(outputDir + TString("/pred1DTIB34_")+name+TString(".eps"));

  //pred1DTOB1234->Draw();
  pred1DTOB1234->Fit("gaus");
  c1->SaveAs(outputDir + TString("/pred1DTOB1234_")+name+TString(".png"));  
  c1->SaveAs(outputDir + TString("/pred1DTOB1234_")+name+TString(".eps"));  

  //pred1DTOB56->Draw();
  pred1DTOB56->Fit("gaus");
  c1->SaveAs(outputDir + TString("/pred1DTOB56_")+name+TString(".png"));
  c1->SaveAs(outputDir + TString("/pred1DTOB56_")+name+TString(".eps"));

  //first do plot to show how resolution is found
  ddTIB12->SetLineColor(2);
  ddTIB12->GetYaxis()->SetTitle("Uncertainty [#mum]");
  ddTIB12->GetXaxis()->SetTitle("");
  ddTIB12->GetXaxis()->SetLabelSize(0);
  ddTIB12->GetXaxis()->SetTickLength(0);
  ddTIB12->SetMarkerStyle(24);    //20
  ddTIB12->SetMarkerColor(2);
  hitresTIB12->SetLineColor(4);
  hitresTIB12->SetMarkerStyle(25);  //21
  hitresTIB12->SetMarkerColor(4);
  predTIB12->SetLineColor(1);
  predTIB12->SetMarkerStyle(26);
  predTIB12->SetMarkerColor(1);
  ddTIB12->SetMaximum(60);
  //ddTIB12->Draw();
  //predTIB12->Draw("same");
  //hitresTIB12->Draw("same");
  
  leg = new TLegend(0.30,0.72,0.64,0.91);
  leg->AddEntry(ddTIB12,"double difference width","p");
  leg->AddEntry(hitresTIB12,"Hit resolution from data","p");
  leg->AddEntry(predTIB12,"<#sigma(#Deltax_{pred})>","p");
  leg->SetTextSize(0.055);
  leg->SetFillColor(0);
  //leg->Draw("same");

  c1->SaveAs(outputDir + TString("/showRes.png"));

  hitresTIB12->Fit("pol0");  
  hitresTIB12->SetLineColor(2);
  hitsigmaTIB12->GetYaxis()->SetTitle("Uncertainty [#mum]");
  hitsigmaTIB12->GetXaxis()->SetTitle("");
  hitsigmaTIB12->GetXaxis()->SetLabelSize(0);
  hitsigmaTIB12->GetXaxis()->SetTickLength(0);
  hitresTIB12->SetMarkerStyle(24);    //20
  hitresTIB12->SetMarkerColor(2);
  hitsigmaTIB12->SetLineColor(4);
  hitsigmaTIB12->SetMarkerStyle(25);  //21
  hitsigmaTIB12->SetMarkerColor(4);
  hitresTIB12->SetMaximum(45);
  hitresTIB12->SetMinimum(0);
  //hitresTIB12->Draw();
  //hitsigmaTIB12->Draw("same");
  
  leg_histTIB12 = new TLegend(0.30,0.72,0.64,0.91);
  leg_histTIB12->AddEntry(hitresTIB12,"Hit resolution from data","p");
  leg_histTIB12->AddEntry(hitsigmaTIB12,"<#sigma(#Deltax_{hit})>","p");
  leg_histTIB12->SetTextSize(0.055);
  leg_histTIB12->SetFillColor(0);
  //leg_histTIB12->Draw("same");

  TString fileRes = outputDir + TString("/Resolution_") + name;
  TString fileRatio = outputDir + TString("/ratio_") + name;
  c1->SaveAs(fileRes+"_TIB12.png");
  c1->SaveAs(fileRes+"_TIB12.eps");
  
  ratio_hTIB12->Fit("pol0");
  ratio_hTIB12->GetYaxis()->SetTitle("Ratio of resolution from data and calculation");
  ratio_hTIB12->GetXaxis()->SetTitle("");
  ratio_hTIB12->GetXaxis()->SetLabelSize(0);
  ratio_hTIB12->GetXaxis()->SetTickLength(0);
  //ratio_hTIB12->Draw();
  c1->SaveAs(fileRatio+"_TIB12.png");
  c1->SaveAs(fileRatio+"_TIB12.eps");

  hitresTIB34->Fit("pol0");  
  hitresTIB34->SetLineColor(2);
  hitsigmaTIB34->GetYaxis()->SetTitle("Uncertainty [#mum]");
  hitsigmaTIB34->GetXaxis()->SetTitle("");
  hitsigmaTIB34->GetXaxis()->SetLabelSize(0);
  hitsigmaTIB34->GetXaxis()->SetTickLength(0);
  hitresTIB34->SetMarkerStyle(24);    //20
  hitresTIB34->SetMarkerColor(2);
  hitsigmaTIB34->SetLineColor(4);
  hitsigmaTIB34->SetMarkerStyle(25);  //21
  hitsigmaTIB34->SetMarkerColor(4);
  //hitresTIB34->SetMaximum(200);
  //hitresTIB34->Draw();
  //hitsigmaTIB34->Draw("same");
  
  leg_histTIB34 = new TLegend(0.60,0.72,0.94,0.91);
  leg_histTIB34->AddEntry(hitresTIB34,"Hit resolution from data","p");
  leg_histTIB34->AddEntry(hitsigmaTIB34,"<#sigma(#Deltax_{hit})>","p");
  leg_histTIB34->SetTextSize(0.055);
  leg_histTIB34->SetFillColor(0);
  //leg_histTIB34->Draw("same");
  c1->SaveAs(fileRes+"_TIB34.png");
  c1->SaveAs(fileRes+"_TIB34.eps");
  
  ratio_hTIB34->Fit("pol0");
  ratio_hTIB34->GetYaxis()->SetTitle("Ratio of resolution from data and calculation");
  ratio_hTIB34->GetXaxis()->SetTitle("");
  ratio_hTIB34->GetXaxis()->SetLabelSize(0);
  ratio_hTIB34->GetXaxis()->SetTickLength(0);
  //ratio_hTIB34->Draw();
  c1->SaveAs(fileRatio+"_TIB34.png");
  c1->SaveAs(fileRatio+"_TIB34.eps");

  hitresTOB1234->Fit("pol0");  
  hitresTOB1234->SetLineColor(2);
  hitsigmaTOB1234->GetYaxis()->SetTitle("Uncertainty [#mum]");
  hitsigmaTOB1234->GetXaxis()->SetTitle("");
  hitsigmaTOB1234->GetXaxis()->SetLabelSize(0);
  hitsigmaTOB1234->GetXaxis()->SetTickLength(0);
  hitresTOB1234->SetMarkerStyle(24);    //20
  hitresTOB1234->SetMarkerColor(2);
  hitsigmaTOB1234->SetLineColor(4);
  hitsigmaTOB1234->SetMarkerStyle(25);  //21
  hitsigmaTOB1234->SetMarkerColor(4);
  //hitresTOB1234->SetMaximum(200);
  //hitresTOB1234->Draw();
  //hitsigmaTOB1234->Draw("same");
  
  leg_histTOB1234 = new TLegend(0.60,0.72,0.94,0.91);
  leg_histTOB1234->AddEntry(hitresTOB1234,"Hit resolution from data","p");
  leg_histTOB1234->AddEntry(hitsigmaTOB1234,"<#sigma(#Deltax_{hit})>","p");
  leg_histTOB1234->SetTextSize(0.055);
  leg_histTOB1234->SetFillColor(0);
  //leg_histTOB1234->Draw("same");
  c1->SaveAs(fileRes+"_TOB1234.png");
  c1->SaveAs(fileRes+"_TOB1234.eps");
  
  ratio_hTOB1234->Fit("pol0");
  ratio_hTOB1234->GetYaxis()->SetTitle("Ratio of resolution from data and calculation");
  ratio_hTOB1234->GetXaxis()->SetTitle("");
  ratio_hTOB1234->GetXaxis()->SetLabelSize(0);
  ratio_hTOB1234->GetXaxis()->SetTickLength(0);
  //ratio_hTOB1234->Draw();
  c1->SaveAs(fileRatio+"_TOB1234.png");
  c1->SaveAs(fileRatio+"_TOB1234.eps");

  hitresTOB56->Fit("pol0");  
  hitresTOB56->SetLineColor(2);
  hitsigmaTOB56->GetYaxis()->SetTitle("Uncertainty [#mum]");
  hitsigmaTOB56->GetXaxis()->SetTitle("");
  hitsigmaTOB56->GetXaxis()->SetLabelSize(0);
  hitsigmaTOB56->GetXaxis()->SetTickLength(0);
  hitresTOB56->SetMarkerStyle(24);    //20
  hitresTOB56->SetMarkerColor(2);
  hitsigmaTOB56->SetLineColor(4);
  hitsigmaTOB56->SetMarkerStyle(25);  //21
  hitsigmaTOB56->SetMarkerColor(4);
  //hitresTOB56->SetMaximum(200);
  //hitresTOB56->Draw();
  //hitsigmaTOB56->Draw("same");
  
  leg_histTOB56 = new TLegend(0.60,0.72,0.94,0.91);
  leg_histTOB56->AddEntry(hitresTOB56,"Hit resolution from data","p");
  leg_histTOB56->AddEntry(hitsigmaTOB56,"<#sigma(#Deltax_{hit})>","p");
  leg_histTOB56->SetTextSize(0.055);
  leg_histTOB56->SetFillColor(0);
  //leg_histTOB56->Draw("same");
  c1->SaveAs(fileRes+"_TOB56.png");
  c1->SaveAs(fileRes+"_TOB56.eps");
  
  ratio_hTOB56->Fit("pol0");
  ratio_hTOB56->GetYaxis()->SetTitle("Ratio of resolution from data and calculation");
  ratio_hTOB56->GetXaxis()->SetTitle("");
  ratio_hTOB56->GetXaxis()->SetLabelSize(0);
  ratio_hTOB56->GetXaxis()->SetTickLength(0);
  //ratio_hTOB56->Draw();
  c1->SaveAs(fileRatio+"_TOB56.png");
  c1->SaveAs(fileRatio+"_TOB56.eps");



  //////////////////////////////////////////
  //
  //  FOR TRACKER MAP ......
  //
  //
  //
  //
  //
  /////////////////////////////////////////


  for (DetInfoMap::iterator it = parMap.begin(); it != parMap.end(); ++it) {
    tmapPar->fill(it->first,it->second.second/it->second.first);
  }
  for (DetInfoMap::iterator it = perpMap.begin(); it != perpMap.end(); ++it) {
    tmapPerp->fill(it->first,it->second.second/it->second.first);
  }
  parMap.clear();
  perpMap.clear();


  for (DetInfoMap::iterator it = parMapY.begin(); it != parMapY.end(); ++it) {
    tmapParY->fill(it->first,it->second.second/it->second.first);
  }
  for (DetInfoMap::iterator it = perpMapY.begin(); it != perpMapY.end(); ++it) {
    tmapPerpY->fill(it->first,it->second.second/it->second.first);
  }
  parMapY.clear();
  perpMapY.clear();
  

  cout << "Creating Tracker Map....."<<endl;

  std::string out = OutPutDir + "/TrackerMap.png";
  tmap->save(false,0,0,out,4500,2400);

  std::string outPar = OutPutDir + "/TrackerMapPar.png";
  tmapPar->save(false,0,0,outPar,4500,2400);
  
  std::string outPerp = OutPutDir + "/TrackerMapPerp.png";
  tmapPerp->save(false,0,0,outPerp,4500,2400);

  
  std::string outParY = OutPutDir + "/TrackerMapParY.png";
  tmapParY->save(false,0,0,outParY,4500,2400);
  
  
  std::string outPerpY = OutPutDir + "/TrackerMapPerpY.png";
  tmapPerpY->save(false,0,0,outPerpY,4500,2400);
  

  //tmap->save(true,10,40,out,4500,2400);

}



/*
void fillAll(int i,  DetIdPair idPair, TrackerMap* tmapPar, TrackerMap* tmapPerp, double val, bool doY )
{

  
  //DetInfoMap parMap, perpMap;
  
  AdjacentType type = checkAdjacentType(idPair.first, idPair.second);
  double res;
 
  //if (doY) res = meanDiffsY->GetBinContent(i);
  //else res = meanDiffs->GetBinContent(i);
 
  res = val;

  if (type == parallelToStrips) {
    fillMap(parMap, idPair.first, res);
    fillMap(parMap, idPair.second, res);
  } else {
    fillMap(perpMap, idPair.first, res);
    fillMap(perpMap, idPair.second, res);
  }
}
*/





void fillMap(DetInfoMap & theMap, int detId, double shift){
  DetInfoMap::iterator it = theMap.find(detId);
  if ( it==theMap.end() ) {
    theMap[detId] = IntFloatPair(1, fabs(shift));
  } else {
    ++(theMap[detId].first);
    theMap[detId].second += fabs(shift);
  }
}



void loadSummaryPlots() {
  baseDir->cd();
  predErrMeans = (TH1*) gDirectory->Get("sigMean");
  predErrMeansFirst = (TH1*) gDirectory->Get("sigMeanFirst");
  predErrMeansSecond = (TH1*) gDirectory->Get("sigMeanSecond");
  simRec = (TH1*) gDirectory->Get("simRec");
  simTrk = (TH1*) gDirectory->Get("simTrk");
  meanDiffs = (TH1F*) gDirectory->Get("mean");
  sigmaDiffs = (TH1F*) gDirectory->Get("width");
  meanDiffsY = (TH1F*) gDirectory->Get("meanY");
  sigmaDiffsY = (TH1F*) gDirectory->Get("widthY");
  hitErrMeans = (TH1*) gDirectory->Get("hitSigX");
  dxdzMeans = (TH1*) gDirectory->Get("dxdz");
  dydzMeans = (TH1*) gDirectory->Get("dydz");
  layerHisto = (TH1*) gDirectory->Get("layer");
  stereoHisto = (TH1*) gDirectory->Get("stereo");
  radHisto = (TH1*) gDirectory->Get("radius");
  phiHisto = (TH1*) gDirectory->Get("phi");
  zHisto = (TH1*) gDirectory->Get("z");
  xHisto = (TH1*) gDirectory->Get("x");
  yHisto = (TH1*) gDirectory->Get("y");
  statHisto = (TH1*) gDirectory->Get("stat");
  doublePulls = (TH1*) gDirectory->Get("doublePull");
  hitPulls = (TH1*) gDirectory->Get("hitPull");
  predPulls = (TH1*) gDirectory->Get("predPull");
  ddXslope = (TH1*) gDirectory->Get("ddXslope");
  ddYslope = (TH1*) gDirectory->Get("ddYslope");
  ddDxslope = (TH1*) gDirectory->Get("ddDxslope");
  
  //TrackAngle = (TH1*) gDirectory->Get("TrackAngle");
  //TrackAnglevsClusterWidth = (TH2*) gDirectory->Get("TrackAnglevsClusterWidth");


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
 cout <<"All loaded\n";
//   ff->Close();
}

void setTDRStyle() {

   TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");

// For the canvas:
   tdrStyle->SetCanvasBorderMode(0);
   tdrStyle->SetCanvasColor(kWhite);
   tdrStyle->SetCanvasDefH(600); //Height of canvas
   tdrStyle->SetCanvasDefW(1100); //Width of canvas
   tdrStyle->SetCanvasDefX(0);   //POsition on screen
   tdrStyle->SetCanvasDefY(0);

// For the Pad:
   tdrStyle->SetPadBorderMode(0);
   // tdrStyle->SetPadBorderSize(Width_t size = 1);
   tdrStyle->SetPadColor(kWhite);
   tdrStyle->SetPadGridX(false);
   tdrStyle->SetPadGridY(false);
   tdrStyle->SetGridColor(0);
   tdrStyle->SetGridStyle(3);
   tdrStyle->SetGridWidth(1);

// For the frame:
   tdrStyle->SetFrameBorderMode(0);
   tdrStyle->SetFrameBorderSize(1);
   tdrStyle->SetFrameFillColor(0);
   tdrStyle->SetFrameFillStyle(0);
   tdrStyle->SetFrameLineColor(1);
   tdrStyle->SetFrameLineStyle(1);
   tdrStyle->SetFrameLineWidth(1);

// For the histo:
   // tdrStyle->SetHistFillColor(1);
   // tdrStyle->SetHistFillStyle(0);
   //tdrStyle->SetHistLineColor(1);
   //tdrStyle->SetHistLineStyle(0);
   tdrStyle->SetHistLineWidth(1.5);
   // tdrStyle->SetLegoInnerR(Float_t rad = 0.5);
   // tdrStyle->SetNumberContours(Int_t number = 20);

//   tdrStyle->SetEndErrorSize(2);
//   tdrStyle->SetErrorMarker(20);
//   tdrStyle->SetErrorX(0.);

   //tdrStyle->SetMarkerStyle(20);

//For the fit/function:
   tdrStyle->SetOptFit(1);
   tdrStyle->SetFitFormat("5.4g");
   tdrStyle->SetFuncColor(2);
   tdrStyle->SetFuncStyle(1);
   tdrStyle->SetFuncWidth(1);

//For the date:
   tdrStyle->SetOptDate(0);
   // tdrStyle->SetDateX(Float_t x = 0.01);
   // tdrStyle->SetDateY(Float_t y = 0.01);

// // For the statistics box:
//   tdrStyle->SetOptFile(0);
//   tdrStyle->SetOptStat(0); // To display the mean and RMS:    
//   tdrStyle->SetOptStat("mr");
   tdrStyle->SetOptStat("");
   tdrStyle->SetStatColor(kWhite);
   tdrStyle->SetStatFont(42);
    tdrStyle->SetStatFontSize(0.025);
   tdrStyle->SetStatTextColor(1);
   tdrStyle->SetStatFormat("6.4g");
   tdrStyle->SetStatBorderSize(1);
   tdrStyle->SetStatH(0.1);
   tdrStyle->SetStatW(0.15);
   // tdrStyle->SetStatStyle(Style_t style = 1001);
   // tdrStyle->SetStatX(Float_t x = 0);
   // tdrStyle->SetStatY(Float_t y = 0);

// Margins:
   tdrStyle->SetPadTopMargin(0.05);
   tdrStyle->SetPadBottomMargin(0.13);
   tdrStyle->SetPadLeftMargin(0.10);
   tdrStyle->SetPadRightMargin(0.02);

// For the Global title:

   tdrStyle->SetOptTitle(0);
   tdrStyle->SetTitleFont(42);
   tdrStyle->SetTitleColor(1);
   tdrStyle->SetTitleTextColor(1);
   tdrStyle->SetTitleFillColor(10);
   tdrStyle->SetTitleFontSize(0.05);
   // tdrStyle->SetTitleH(0); // Set the height of the title box
   // tdrStyle->SetTitleW(0); // Set the width of the title box
   // tdrStyle->SetTitleX(0); // Set the position of the title box
   // tdrStyle->SetTitleY(0.985); // Set the position of the title box
   // tdrStyle->SetTitleStyle(Style_t style = 1001);
   // tdrStyle->SetTitleBorderSize(2);

// For the axis titles:

   tdrStyle->SetTitleColor(1, "XYZ");
   tdrStyle->SetTitleFont(42, "XYZ");
   //tdrStyle->SetTitleSize(0.06, "XYZ");
   tdrStyle->SetTitleSize(0.052, "XYZ");
   // tdrStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
   // tdrStyle->SetTitleYSize(Float_t size = 0.02);
   //tdrStyle->SetTitleXOffset(0.9);
   tdrStyle->SetTitleXOffset(0.875);
   tdrStyle->SetTitleYOffset(0.90);
   //tdrStyle->SetTitleYOffset(1.05);
   // tdrStyle->SetTitleOffset(1.1, "Y"); // Another way to set the  Offset

// For the axis labels:

   tdrStyle->SetLabelColor(1, "XYZ");
   tdrStyle->SetLabelFont(42, "XYZ");
   tdrStyle->SetLabelOffset(0.007, "XYZ");
   tdrStyle->SetLabelSize(0.05, "XYZ");

// For the axis:

   tdrStyle->SetAxisColor(1, "XYZ");
   tdrStyle->SetStripDecimals(kTRUE);
   tdrStyle->SetTickLength(0.03, "XYZ");
   tdrStyle->SetNdivisions(510, "XYZ");
   tdrStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
   tdrStyle->SetPadTickY(1);

// Change for log plots:
   tdrStyle->SetOptLogx(0);
   tdrStyle->SetOptLogy(0);
   tdrStyle->SetOptLogz(0);

// Postscript options:
   tdrStyle->SetPaperSize(20.,20.);
   // tdrStyle->SetLineScalePS(Float_t scale = 3);
   // tdrStyle->SetLineStyleString(Int_t i, const char* text);
   // tdrStyle->SetHeaderPS(const char* header);
   // tdrStyle->SetTitlePS(const char* pstitle);

   // tdrStyle->SetBarOffset(Float_t baroff = 0.5);
   // tdrStyle->SetBarWidth(Float_t barwidth = 0.5);
   // tdrStyle->SetPaintTextFormat(const char* format = "g");
   // tdrStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
   // tdrStyle->SetTimeOffset(Double_t toffset);
   // tdrStyle->SetHistMinimumZero(kTRUE);

   tdrStyle->cd();

}
