// -*- C++ -*-
//
// Package:    RPCMonitorEfficiency
// Class:      RPCMonitorEfficiency
// 
/**\class RPCMonitorEfficiency RPCMonitorEfficiency.cc DQM/RPCMonitorModule/src/RPCMonitorEfficiency.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  pts/45
//         Created:  Tue May 13 12:23:34 CEST 2008
// $Id: RPCMonitorEfficiency.cc,v 1.60 2012/03/06 15:49:21 carrillo Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <Geometry/RPCGeometry/interface/RPCGeometry.h>
#include <Geometry/RPCGeometry/interface/RPCGeomServ.h>
#include <Geometry/CommonDetUnit/interface/GeomDet.h>
#include <Geometry/Records/interface/MuonGeometryRecord.h>
#include "FWCore/Framework/interface/ESHandle.h"
#include<string>
#include<fstream>
#include <DQM/RPCMonitorDigi/interface/RPCBookFolderStructure.h>
#include <Geometry/CommonTopologies/interface/RectangularStripTopology.h>
#include <Geometry/CommonTopologies/interface/TrapezoidalStripTopology.h>

//Root
#include "TFile.h"
#include "TF1.h"
#include "TH1F.h"
#include "TH1.h"
#include "TH2F.h"
#include "TROOT.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TGaxis.h"
#include <TStyle.h>
#include "TText.h"
#include "TPaveText.h"
#include "TH1D.h"
#include "TGraphErrors.h"

//
// class decleration
//

class TFile;

class RPCMonitorEfficiency : public edm::EDAnalyzer {
public:
  explicit RPCMonitorEfficiency(const edm::ParameterSet&);
  ~RPCMonitorEfficiency();  
  TFile * theFile;
  TFile * theFileOut;
  
  TH1F * statistics;

  TH1F * CosAngMB3MB4;
  
  TH1F * CosAngMB3MB4Whm2;
  TH1F * CosAngMB3MB4Whm1;
  TH1F * CosAngMB3MB4Wh0;
  TH1F * CosAngMB3MB4Wh1;
  TH1F * CosAngMB3MB4Wh2;
  
  TH1F * MeanResiduals;
  TH1F * MeanResiduals11;
  TH1F * AllResidualsBarrel;
  TH1F * AllResidualsEndCap;

  TH1F * RMSResiduals;
  TH1F * RMSResiduals11;

  TH2F * bxbarrel;
  TH2F * bxendcap;

  TH2F * signal_bxbarrel;
  TH2F * signal_bxendcap;

  //Cesare
  TH1F * EndcapEffTot;
  TH1F * EffBarrelRoll;
  TH1F * HitDen;
  TH1F * CSCDen;
  TH1F * EfficiencyPerLayer;
  TH1F * StatisticsPerLayer;
  
  TH1F * ExpLayerWm2;  
  TH1F * ExpLayerWm1;
  TH1F * ExpLayerW0;
  TH1F * ExpLayerW1;
  TH1F * ExpLayerW2;

  TH1F * ObsLayerWm2;
  TH1F * ObsLayerWm1;
  TH1F * ObsLayerW0;
  TH1F * ObsLayerW1;
  TH1F * ObsLayerW2;

  //Endcap

  //Paper

  TH1F *  residualStation1Ring2_A;
  TH1F *  residualStation1Ring2_B;
  TH1F *  residualStation1Ring2_C;

  TH1F *  residualStation1Ring3_A;
  TH1F *  residualStation1Ring3_B;
  TH1F *  residualStation1Ring3_C; 			  	

  TH1F * residualStation23Ring2_A;
  TH1F * residualStation23Ring2_B;
  TH1F * residualStation23Ring2_C;

  TH1F * residualStation23Ring3_A;
  TH1F * residualStation23Ring3_B;
  TH1F * residualStation23Ring3_C;

  TH1F * residualStation123Ring3_A;
  TH1F * residualStation123Ring3_B;
  TH1F * residualStation123Ring3_C;
 
  TH1F *  residualRB1in;
  TH1F * residualRB1out;
  TH1F *  residualRB2in;
  TH1F * residualRB2out;
  TH1F *    residualRB3;
  TH1F *    residualRB4;

  //Other residuals
  
  TH1F * residualDisk1Ring2;
  TH1F * residualDisk1Ring3;
  TH1F * residualDisk2Ring2;
  TH1F * residualDisk2Ring3;
  TH1F * residualDisk3Ring2;
  TH1F * residualDisk3Ring3;

  TH1F * residualDiskm1Ring2;
  TH1F * residualDiskm1Ring3;
  TH1F * residualDiskm2Ring2;
  TH1F * residualDiskm2Ring3;
  TH1F * residualDiskm3Ring2;
  TH1F * residualDiskm3Ring3;

  TH1F * EffBarrel_black; //Average
  TH1F * DoubleGapBarrel_black; //Double GapEff
  TH1F * CentralEffBarrel_black; //Central Zone  

  TH1F * EffBarrel; //Average
  TH1F * DoubleGapBarrel; //Double GapEff
  TH1F * CentralEffBarrel; //Central Zone
  TH1F * BXEffBarrel; //Distribution for good synchronized chambers
  TH1F * badBXEffBarrel; //Distribution for bad synchronized chambers

  TH1F * EffDistroWm2;
  TH1F * EffDistroWm1;
  TH1F * EffDistroW0;
  TH1F * EffDistroW1;
  TH1F * EffDistroW2;

  TH1F * EffDistroWm2far;
  TH1F * EffDistroWm1far;
  TH1F * EffDistroW0far;
  TH1F * EffDistroW1far;
  TH1F * EffDistroW2far;

  TH1F * DoubleGapDistroWm2;
  TH1F * DoubleGapDistroWm1;
  TH1F * DoubleGapDistroW0;
  TH1F * DoubleGapDistroW1;
  TH1F * DoubleGapDistroW2;

  TH1F * DoubleGapDistroWm2far;
  TH1F * DoubleGapDistroWm1far;
  TH1F * DoubleGapDistroW0far;
  TH1F * DoubleGapDistroW1far;
  TH1F * DoubleGapDistroW2far;

  TH1F * EffEndCap_black; //Average
  TH1F * DoubleGapEndCap_black; //Double Gap
  TH1F * CentralEffEndCap_black; //Central Zone

  TH1F * EffEndCap; //Average
  TH1F * DoubleGapEndCap; //Double Gap
  TH1F * CentralEffEndCap; //Central Zone
  TH1F * BXEffEndCap; //Distribution for good synchronized chambers
  TH1F * badBXEffEndCap; //Distribution for bad synchronized chambers

  TH1F * BXEffPositiveEndCap;
  TH1F * BXEffNegativeEndCap;
  TH1F * badBXEffPositiveEndCap;
  TH1F * badBXEffNegativeEndCap;

  TH2F * HeightVsEffR2;
  TH2F * HeightVsEffR3;

  TH1F * GregD1R2;  
  TH1F * GregD1R3;  
  TH1F * GregD2R2;  
  TH1F * GregD2R3;  
  TH1F * GregD3R2;  
  TH1F * GregD3R3;  

  // For black list
  TH1F * GregD1R2_black;
  TH1F * GregD1R3_black;
  TH1F * GregD2R2_black;
  TH1F * GregD2R3_black;
  TH1F * GregD3R2_black;
  TH1F * GregD3R3_black;

  TH1F * OcGregD1R2;  
  TH1F * OcGregD1R3;  
  TH1F * OcGregD2R2;  
  TH1F * OcGregD2R3;  
  TH1F * OcGregD3R2;  
  TH1F * OcGregD3R3;  

  TH1F * ExGregD1R2;  
  TH1F * ExGregD1R3;  
  TH1F * ExGregD2R2;  
  TH1F * ExGregD2R3;  
  TH1F * ExGregD3R2;  
  TH1F * ExGregD3R3;  

  TH1F * GregDm1R2;  
  TH1F * GregDm1R3;  
  TH1F * GregDm2R2;  
  TH1F * GregDm2R3;  
  TH1F * GregDm3R2;  
  TH1F * GregDm3R3;

  //For black list
  TH1F * GregDm1R2_black;   
  TH1F * GregDm1R3_black;   
  TH1F * GregDm2R2_black;   
  TH1F * GregDm2R3_black;   
  TH1F * GregDm3R2_black;   
  TH1F * GregDm3R3_black;
  
  TH1F * OcGregDm1R2;  
  TH1F * OcGregDm1R3;  
  TH1F * OcGregDm2R2;  
  TH1F * OcGregDm2R3;  
  TH1F * OcGregDm3R2;  
  TH1F * OcGregDm3R3;  

  TH1F * ExGregDm1R2;  
  TH1F * ExGregDm1R3;  
  TH1F * ExGregDm2R2;  
  TH1F * ExGregDm2R3;  
  TH1F * ExGregDm3R2;  
  TH1F * ExGregDm3R3;  
  
  TH1F * CLSWm2;  
  TH1F * CLSWm1;  
  TH1F * CLSW0;  
  TH1F * CLSW1;  
  TH1F * CLSW2;

  TH1F * CLSWm2layer;  
  TH1F * CLSWm1layer;  
  TH1F * CLSW0layer;  
  TH1F * CLSW1layer;  
  TH1F * CLSW2layer;

  TH1F * CLSStation1Ring2_A;
  TH1F * CLSStation1Ring2_B;
  TH1F * CLSStation1Ring2_C;

  TH1F * CLSStation23Ring2_A;
  TH1F * CLSStation23Ring2_B;
  TH1F * CLSStation23Ring2_C;

  TH1F * CLSStation123Ring3_A;
  TH1F * CLSStation123Ring3_B;
  TH1F * CLSStation123Ring3_C;

  TH1F * sectorCLSWm2[13];  
  TH1F * sectorCLSWm1[13];  
  TH1F * sectorCLSW0[13];  
  TH1F * sectorCLSW1[13];  
  TH1F * sectorCLSW2[13];

  TH1F * layerCLSWm2[7];  
  TH1F * layerCLSWm1[7];  
  TH1F * layerCLSW0[7];  
  TH1F * layerCLSW1[7];  
  TH1F * layerCLSW2[7];

  TH1F * CLSlayer[7];

  TH1F * CLS;
  
  TH1F * sectorEffWm2;  
  TH1F * sectorEffWm1;  
  TH1F * sectorEffW0;  
  TH1F * sectorEffW1;  
  TH1F * sectorEffW2;  

  TH1F * OcsectorEffWm2;  
  TH1F * OcsectorEffWm1;  
  TH1F * OcsectorEffW0;  
  TH1F * OcsectorEffW1;  
  TH1F * OcsectorEffW2;  

  TH1F * ExsectorEffWm2;  
  TH1F * ExsectorEffWm1;  
  TH1F * ExsectorEffW0;  
  TH1F * ExsectorEffW1;  
  TH1F * ExsectorEffW2;  

  TH1F * EffDistroDm3;  
  TH1F * EffDistroDm2;
  TH1F * EffDistroDm1;
  TH1F * EffDistroD1;
  TH1F * EffDistroD2;
  TH1F * EffDistroD3;

  TH1F * EffDistroDm3far;
  TH1F * EffDistroDm2far;
  TH1F * EffDistroDm1far;
  TH1F * EffDistroD1far;
  TH1F * EffDistroD2far;
  TH1F * EffDistroD3far;

  TH2F * IntegralMuography[48];//There are 48 different kind of rolls in CMS.
  TH2F * IntegralMuographyObs[48];//There are 48 different kind of rolls in CMS.
  TH2F * IntegralMuographyExp[48];//There are 48 different kind of rolls in CMS.

  TH2F * Wheelm2Summary;
  TH2F * Wheelm1Summary;
  TH2F * Wheel0Summary;
  TH2F * Wheel1Summary;
  TH2F * Wheel2Summary;

  TH2F * Diskm3Summary;
  TH2F * Diskm2Summary;
  TH2F * Diskm1Summary;
  TH2F * Disk1Summary;
  TH2F * Disk2Summary;
  TH2F * Disk3Summary;

  TH2F * PositiveEndCapSummary;
  TH2F * NegativeEndCapSummary;

  TH1F * histoRPC;
  TH2F * histoRPC_2D;
  TH1F * histoDT;
  TH2F * histoDT_2D;
  TH1F * histoCSC;
  TH2F * histoCSC_2D;
  TH1F * histoPRO;  
  TH1F * histoPROY;
  TH1F * histoPROX;
  TH2F * histoPRO_2D;
  TH1F * histoCellDistro;
  TH1F * histoRES;
  TH1F * BXDistribution;
  TH1F * Signal_BXDistribution;
  TH1F * histoRealRPC;
  TH1F * histoResidual;
  TH1F * histoResidual1;
  TH1F * histoResidual2;
  TH1F * histoResidual3;
  TH1F * histoResidualO;
  TH1F * histoCLS;
  TH2F * histoBXY;
  TH2F * histoINEF;
  
  TH1F * EffGlobWm2;
  TH1F * EffGlobWm1;
  TH1F * EffGlobW0;
  TH1F * EffGlobW1;
  TH1F * EffGlobW2;

  TH1F * EffGlobWm2far;
  TH1F * EffGlobWm1far;
  TH1F * EffGlobW0far;
  TH1F * EffGlobW1far;
  TH1F * EffGlobW2far;

  TH1F * DoubleGapWm2;
  TH1F * DoubleGapWm1;
  TH1F * DoubleGapW0;
  TH1F * DoubleGapW1;
  TH1F * DoubleGapW2;

  TH1F * DoubleGapWm2far;
  TH1F * DoubleGapWm1far;
  TH1F * DoubleGapW0far;
  TH1F * DoubleGapW1far;
  TH1F * DoubleGapW2far;

  TH1F * BXGlobWm2;
  TH1F * BXGlobWm1;
  TH1F * BXGlobW0;
  TH1F * BXGlobW1;
  TH1F * BXGlobW2;

  TH1F * BXGlobWm2far;
  TH1F * BXGlobWm1far;
  TH1F * BXGlobW0far;
  TH1F * BXGlobW1far;
  TH1F * BXGlobW2far;

  TH1F * Signal_BXGlobWm2;
  TH1F * Signal_BXGlobWm1;
  TH1F * Signal_BXGlobW0;
  TH1F * Signal_BXGlobW1;
  TH1F * Signal_BXGlobW2;

  TH1F * Signal_BXGlobWm2far;
  TH1F * Signal_BXGlobWm1far;
  TH1F * Signal_BXGlobW0far;
  TH1F * Signal_BXGlobW1far;
  TH1F * Signal_BXGlobW2far;

  TH1F * MaskedGlobWm2;
  TH1F * MaskedGlobWm1;
  TH1F * MaskedGlobW0;
  TH1F * MaskedGlobW1;
  TH1F * MaskedGlobW2;

  TH1F * MaskedGlobWm2far;
  TH1F * MaskedGlobWm1far;
  TH1F * MaskedGlobW0far;
  TH1F * MaskedGlobW1far;
  TH1F * MaskedGlobW2far;

  TH1F * AverageEffWm2;
  TH1F * AverageEffWm1;
  TH1F * AverageEffW0;
  TH1F * AverageEffW1;
  TH1F * AverageEffW2;

  TH1F * AverageEffWm2far;
  TH1F * AverageEffWm1far;
  TH1F * AverageEffW0far;
  TH1F * AverageEffW1far;
  TH1F * AverageEffW2far;
  
  TH1F * NoPredictionWm2;
  TH1F * NoPredictionWm1;
  TH1F * NoPredictionW0;
  TH1F * NoPredictionW1;
  TH1F * NoPredictionW2;

  TH1F * NoPredictionWm2far;
  TH1F * NoPredictionWm1far;
  TH1F * NoPredictionW0far;
  TH1F * NoPredictionW1far;
  TH1F * NoPredictionW2far;

  TH1F * EffGlobDm3;
  TH1F * EffGlobDm2;
  TH1F * EffGlobDm1;
  TH1F * EffGlobD1;
  TH1F * EffGlobD2;
  TH1F * EffGlobD3;

  TH1F * EffGlobDm3far;
  TH1F * EffGlobDm2far;
  TH1F * EffGlobDm1far;
  TH1F * EffGlobD1far;
  TH1F * EffGlobD2far;
  TH1F * EffGlobD3far;

  TH1F * BXGlobDm3;
  TH1F * BXGlobDm2;
  TH1F * BXGlobDm1;
  TH1F * BXGlobD1;
  TH1F * BXGlobD2;
  TH1F * BXGlobD3;
  
  TH1F * BXGlobDm3far;
  TH1F * BXGlobDm2far;
  TH1F * BXGlobDm1far;
  TH1F * BXGlobD1far;
  TH1F * BXGlobD2far;
  TH1F * BXGlobD3far;

  TH1F * Signal_BXGlobDm3;
  TH1F * Signal_BXGlobDm2;
  TH1F * Signal_BXGlobDm1;
  TH1F * Signal_BXGlobD1;
  TH1F * Signal_BXGlobD2;
  TH1F * Signal_BXGlobD3;
  
  TH1F * Signal_BXGlobDm3far;
  TH1F * Signal_BXGlobDm2far;
  TH1F * Signal_BXGlobDm1far;
  TH1F * Signal_BXGlobD1far;
  TH1F * Signal_BXGlobD2far;
  TH1F * Signal_BXGlobD3far;

  TH1F * MaskedGlobDm3;
  TH1F * MaskedGlobDm2;
  TH1F * MaskedGlobDm1;
  TH1F * MaskedGlobD1;
  TH1F * MaskedGlobD2;
  TH1F * MaskedGlobD3;
  
  TH1F * MaskedGlobDm3far;
  TH1F * MaskedGlobDm2far;
  TH1F * MaskedGlobDm1far;
  TH1F * MaskedGlobD1far;
  TH1F * MaskedGlobD2far;
  TH1F * MaskedGlobD3far;

  TH1F * AverageEffDm3;
  TH1F * AverageEffDm2;
  TH1F * AverageEffDm1;
  TH1F * AverageEffD1;
  TH1F * AverageEffD2;
  TH1F * AverageEffD3;

  TH1F * AverageEffDm3far;
  TH1F * AverageEffDm2far;
  TH1F * AverageEffDm1far;
  TH1F * AverageEffD1far;
  TH1F * AverageEffD2far;
  TH1F * AverageEffD3far;

  TH1F * NoPredictionDm3;
  TH1F * NoPredictionDm2;
  TH1F * NoPredictionDm1;
  TH1F * NoPredictionD1;
  TH1F * NoPredictionD2;
  TH1F * NoPredictionD3;

  TH1F * NoPredictionDm3far;
  TH1F * NoPredictionDm2far;
  TH1F * NoPredictionDm1far;
  TH1F * NoPredictionD1far;
  TH1F * NoPredictionD2far;
  TH1F * NoPredictionD3far;

  TH2F * ScatterPlotAlphaCLSLa1;
  TH2F * ScatterPlotAlphaCLSLa2;
  TH2F * ScatterPlotAlphaCLSLa3;
  TH2F * ScatterPlotAlphaCLSLa4;
  TH2F * ScatterPlotAlphaCLSLa5;
  TH2F * ScatterPlotAlphaCLSLa6;

  TH2F * ScatterPlotAlphaPCLSLa1;
  TH2F * ScatterPlotAlphaPCLSLa2;
  TH2F * ScatterPlotAlphaPCLSLa3;
  TH2F * ScatterPlotAlphaPCLSLa4;
  TH2F * ScatterPlotAlphaPCLSLa5;
  TH2F * ScatterPlotAlphaPCLSLa6;

  TH1F * DistBorderClu1La1;
  TH1F * DistBorderClu1La2;
  TH1F * DistBorderClu1La3;
  TH1F * DistBorderClu1La4;
  TH1F * DistBorderClu1La5;
  TH1F * DistBorderClu1La6;
  
  TH1F * DistBorderClu2La1;
  TH1F * DistBorderClu2La2;
  TH1F * DistBorderClu2La3;
  TH1F * DistBorderClu2La4;
  TH1F * DistBorderClu2La5;
  TH1F * DistBorderClu2La6;
		   
  TH1F * DistBorderClu3La1;
  TH1F * DistBorderClu3La2;
  TH1F * DistBorderClu3La3;
  TH1F * DistBorderClu3La4;
  TH1F * DistBorderClu3La5;
  TH1F * DistBorderClu3La6;
  TPaveText * pave;

private:
  virtual void beginRun(const edm::Run&, const edm::EventSetup&);
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endRun() ;
  std::string file;
  std::string fileout;
  std::ofstream RollYEff;
  std::ofstream efftxt;
  std::ofstream alignment;
  std::ofstream rollZeroEff;
  std::ofstream rollZeroPrediction;
  std::ofstream database;
  std::string BlackListFile;
  bool prodimages;
  bool makehtml;
  bool debug;
  bool stat;
  double threshold;
  double fiducialcut;
  double fiducialcutx;
  double fiducialcuty;
  bool endcap;
  bool barrel; 
  bool SkipProblematicChambers;
  std::vector<unsigned int> blacklist;
  std::vector<unsigned int> IntegralMuographyRawIdsVector[48];

 //Cesare
  std::ofstream RollBeff;
  std::ofstream RollEeff;
  std::ofstream EffChamEndcap;
  std::ofstream EffBarrelStrip;
  std::ofstream EffEndcapStrip;
};

int rollY(std::string shortname,std::map<int,std::string> rollNames){
  int myy=0;
  for(int i=1;i<22;i++){
    if(rollNames[i].compare(shortname)==0){
      myy=i;
      return myy;
    }
  }
  if(myy==0){
    std::cout<<"Check your map or your DetId for "<<shortname<<std::endl;
  }
  return myy;
}

bool IsBadRoll(uint32_t rawId,std::vector<uint32_t> thelist){
  bool isBadRoll = false;
  if(!(find(thelist.begin(),thelist.end(),rawId)==thelist.end())){
    isBadRoll=true;
  }
  return isBadRoll;
}


bool HasBadRoll(int region,uint32_t station,uint32_t ring,int k,std::vector<uint32_t> thelist){
  uint32_t sector = (k-1)/6+1;
  uint32_t subsector =k%6==0?6:k%6;
  bool hasBadRoll = false;

 std::cout<<"hasBadRoll Evaluating "<<region<<" "<<station<<" "<<ring<<" "<<k<<std::endl;

  for(uint32_t roll =1;roll<=3;roll++){
    RPCDetId ThisDetId(region,ring,station,sector,1,subsector,roll);
    std::cout<<"hasBadRoll For this input"<<region<<" "<<station<<" "<<ring<<" "<<k<<std::endl;
    RPCGeomServ rpcsrv(ThisDetId);
    std::cout<<"hasBadRoll We are getting"<<rpcsrv.name()<<std::endl;
    std::cout<<"hasBadRoll Trying to find in the list "<<ThisDetId.rawId()<<" "<<rpcsrv.name()<<std::endl;
    bool thisroll = false;
    if(!(find(thelist.begin(),thelist.end(),ThisDetId.rawId())==thelist.end())){
      std::cout<<"hasBadRoll \t found "<<rpcsrv.name()<<std::endl;
      thisroll=true;
    }
    hasBadRoll=hasBadRoll+thisroll;
  }
  if(hasBadRoll){
    std::cout<<"hasBadRoll filling GregD1R2_black in k="<<k<<std::endl;
    std::cout<<"hasBadRoll This Input"<<region<<" "<<station<<" "<<ring<<" "<<k<<"reported a bad roll"<<std::endl;
  }else{
    std::cout<<"Filling GregD1R2 in k="<<k<<std::endl;
  }
  return hasBadRoll;
}

RPCMonitorEfficiency::RPCMonitorEfficiency(const edm::ParameterSet& iConfig){
  //now do what ever initialization is needed
  file=iConfig.getUntrackedParameter<std::string>("fileName");
  fileout=iConfig.getUntrackedParameter<std::string>("fileOut");  
  prodimages=iConfig.getUntrackedParameter<bool>("prodimages");
  makehtml=iConfig.getUntrackedParameter<bool>("makehtml");
  debug=iConfig.getUntrackedParameter<bool>("debug",false);
  stat=iConfig.getUntrackedParameter<bool>("statistics",false);
  threshold=iConfig.getUntrackedParameter<double>("threshold");
  fiducialcut=iConfig.getUntrackedParameter<double>("fiducialcut",8.);
  fiducialcutx=iConfig.getUntrackedParameter<double>("fiducialcutx",fiducialcut);
  fiducialcuty=iConfig.getUntrackedParameter<double>("fiducialcuty",fiducialcut);
  endcap=iConfig.getUntrackedParameter<bool>("endcap");
  barrel=iConfig.getUntrackedParameter<bool>("barrel");
  SkipProblematicChambers=iConfig.getUntrackedParameter<bool>("SkipProblematicChambers",true);

  BlackListFile  = iConfig.getUntrackedParameter<std::string>("BlackListFile","blacklist.dat"); 

  efftxt.open("RPCDetId_Eff.dat");
  alignment.open("Alignment.dat");
  database.open("database.dat");
  RollYEff.open("rollYeff.txt");
  rollZeroEff.open("rollZeroEff.txt");
  rollZeroPrediction.open("rollZeroPrediction.txt");

 //Cesare
  RollBeff.open("RollBarrelEffStat.txt");
  RollEeff.open("RollEndcapEffStat.txt");
  EffBarrelStrip.open("EffBarrelStrip.txt");
  EffEndcapStrip.open("EffEndcapStrip.txt");
  EffChamEndcap.open("EffChamEndcap.txt");
}


RPCMonitorEfficiency::~RPCMonitorEfficiency(){}

void RPCMonitorEfficiency::beginRun(const edm::Run&,const edm::EventSetup&){
  if(debug) std::cout <<"Begin Job"<<std::endl;

  theFile = new TFile(file.c_str());
  if(!theFile)if(debug) std::cout<<"The File Doesn't exist"<<std::endl;
  theFileOut = new TFile(fileout.c_str(), "RECREATE");
  
  std::ifstream ifin(BlackListFile.c_str());

  std::cout<<"Blacklist file name "<<BlackListFile<<" status"<<!ifin<<std::endl;
  
  blacklist.clear();

  if(ifin.is_open()){
    uint32_t rawId;
    std::string name;
    while (ifin.good()){
      ifin >>rawId >>name;
      std::cout<<"rawId = "<<rawId<<" name= "<<name<<std::endl;
      blacklist.push_back(rawId);
    }
  }
  
  std::cout<<"Black list has "<<blacklist.size()<<" rolls"<<std::endl;
  ifin.close();

}


void RPCMonitorEfficiency::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  //  bool first=false;
  if(debug) std::cout <<"\t Getting the RPC Geometry"<<std::endl;
  edm::ESHandle<RPCGeometry> rpcGeo;
  iSetup.get<MuonGeometryRecord>().get(rpcGeo);

  int CanvaSizeX = 1200;
  int CanvaSizeY = 600;

  TCanvas * Ca0;
  TCanvas * Ca1;
  TCanvas * Ca11;
  TCanvas * Ca2;
  TCanvas * Ca3;
  TCanvas * Ca4;
  TCanvas * Ca5;
  TCanvas * Ca5a;
  TCanvas * Ca6;
  TCanvas * Ca7;
  TCanvas * Ca8;
  TCanvas * Ca9;
  TCanvas * Ca10;  
  TCanvas * Ca101;  

  Ca0 = new TCanvas("Ca0","Profile",400,300);
  Ca1 = new TCanvas("Ca1","Efficiency",800,600);
  Ca11 = new TCanvas("Ca11","Efficiency",800,600);
  Ca2 = new TCanvas("Ca2","Global Efficiency",CanvaSizeX,CanvaSizeY);
  Ca3 = new TCanvas("Ca3","Profile",1200,600);
  Ca4 = new TCanvas("Ca4","Residuals",800,600);
  Ca5 = new TCanvas("Ca5","BXbyRegions",800,600);
  Ca5a = new TCanvas("Ca5a","Scatter Angle Plots",800,600);
  Ca6 = new TCanvas("Ca6","ClusterSize",800,600);
  Ca7 = new TCanvas("Ca7","ResidualsEndCap",800,600);
  Ca8 = new TCanvas("Ca8","DistBorders",800,600);
  Ca9 = new TCanvas("Ca9","MeanResiduals",400,300);
  Ca10 = new TCanvas("Ca10","EfficiencyPerLayer",800,600);
  Ca101 = new TCanvas("Ca101","EfficiencyPerDisk",800,600);

  
  Ca0->SetFillColor(0); 
  Ca1->SetFillColor(0); 
  Ca11->SetFillColor(0);
  Ca2->SetFillColor(0); 
  Ca3->SetFillColor(0); 
  Ca4->SetFillColor(0); 
  Ca5->SetFillColor(0); 
  Ca5a->SetFillColor(0);
  Ca6->SetFillColor(0); 
  Ca7->SetFillColor(0); 
  Ca8->SetFillColor(0); 
  Ca9->SetFillColor(0); 
  Ca10->SetFillColor(0);
  Ca101->SetFillColor(0);
    
  //Cesare
  EffBarrelRoll = new TH1F ("EffBarrelRoll","Efficiency Roll by Roll For All The Barrel",54,-7,101);
  EndcapEffTot = new TH1F ("EndcapEffTot","Efficiency chamber by chamber",54,-7,101);
  HitDen = new TH1F ("HitDen","HitDen", 500, 0, 500);
  CSCDen = new TH1F ("CSCDen","CSCDen", 250, 0, 250);
  EfficiencyPerLayer = new TH1F("EfficiencyPerLayer","Efficiency per Layer",6,0.5,6.5);
  StatisticsPerLayer = new TH1F("StatisticsPerLayer","Statistics per Layer",6,0.5,6.5);

  ExpLayerWm2 = new TH1F ("ExpLayerWm2","Expected Wheel - 2",6,0.5,6.5);
  ExpLayerWm1 = new TH1F ("ExpLayerWm1","Expected Wheel - 1",6,0.5,6.5);
  ExpLayerW0 = new TH1F ("ExpLayerW0","Expected Wheel 0",6,0.5,6.5);
  ExpLayerW1 = new TH1F ("ExpLayerW1","Expected Wheel 1",6,0.5,6.5);
  ExpLayerW2 = new TH1F ("ExpLayerW2","Expected Wheel 2",6,0.5,6.5);

  ObsLayerWm2 = new TH1F ("ObsLayerWm2","Observed Wheel - 2",6,0.5,6.5);
  ObsLayerWm1 = new TH1F ("ObsLayerWm1","Observed Wheel - 1",6,0.5,6.5);
  ObsLayerW0 = new TH1F ("ObsLayerW0","Observed Wheel 0",6,0.5,6.5);
  ObsLayerW1 = new TH1F ("ObsLayerW1","Observed Wheel 1",6,0.5,6.5);
  ObsLayerW2 = new TH1F ("ObsLayerW2","Observed Wheel 2",6,0.5,6.5);

  
  bxendcap= new TH2F ("BXEndCap","BX Distribution for the End Cap",51,-5.0,5.0,51,0.,4.);
  bxbarrel= new TH2F ("BXBarrel","BX Distribution for the Barrel",51,-5.0,5.0,51,0.,4.);

  std::string namesIntegralMuography[48];

  namesIntegralMuography[0] = "105.7_2.28889_90";
  namesIntegralMuography[1] = "105.7_2.45238_84";
  namesIntegralMuography[2] = "105.7_2.95238_84";
  namesIntegralMuography[3] = "105.7_3.52381_42";
  namesIntegralMuography[4] = "105.7_4.05556_36";
  namesIntegralMuography[5] = "105.7_4.125_48";
  namesIntegralMuography[6] = "116.9_2.28889_90";
  namesIntegralMuography[7] = "116.9_2.45238_84";
  namesIntegralMuography[8] = "116.9_2.75556_90";
  namesIntegralMuography[9] = "116.9_2.95238_84";
  namesIntegralMuography[10] = "116.9_3.52381_42";
  namesIntegralMuography[11] = "116.9_4.11111_36";
  namesIntegralMuography[12] = "116.9_4.125_48";
  namesIntegralMuography[13] = "116.9_4.13333_60";
  namesIntegralMuography[14] = "125.2_2.28889_90";
  namesIntegralMuography[15] = "125.2_2.45238_84";
  namesIntegralMuography[16] = "125.2_2.75556_90";
  namesIntegralMuography[17] = "125.2_2.95238_84";
  namesIntegralMuography[18] = "125.2_3.52381_42";
  namesIntegralMuography[19] = "125.2_4.11111_36";
  namesIntegralMuography[20] = "125.2_4.125_48";
  namesIntegralMuography[21] = "125.2_4.13333_60";
  namesIntegralMuography[22] = "40.3_3.61625_32";
  namesIntegralMuography[23] = "47.9_1.94572_32";
  namesIntegralMuography[24] = "49_2.09312_32";
  namesIntegralMuography[25] = "51.9_3.65066_32";
  namesIntegralMuography[26] = "53.5_2.37878_32";
  namesIntegralMuography[27] = "53.9_2.22953_32";
  namesIntegralMuography[28] = "56_2.92697_32";
  namesIntegralMuography[29] = "57.4_2.75556_90";
  namesIntegralMuography[30] = "60.4_2.91769_32";
  namesIntegralMuography[31] = "61.7_2.55106_32";
  namesIntegralMuography[32] = "65.7_2.75556_90";
  namesIntegralMuography[33] = "75.9_3.29306_32";
  namesIntegralMuography[34] = "75.9_3.29581_32";
  namesIntegralMuography[35] = "76.9_2.75556_90";
  namesIntegralMuography[36] = "76.9_2.95238_84";
  namesIntegralMuography[37] = "79.4_1.73672_32";
  namesIntegralMuography[38] = "79.4_2.75556_90";
  namesIntegralMuography[39] = "79.4_2.95238_84";
  namesIntegralMuography[40] = "85.2_2.75556_90";
  namesIntegralMuography[41] = "85.2_2.95238_84";
  namesIntegralMuography[42] = "97.4_2.28889_90";
  namesIntegralMuography[43] = "97.4_2.45238_84";
  namesIntegralMuography[44] = "97.4_2.95238_84";
  namesIntegralMuography[45] = "97.4_3.52381_42";
  namesIntegralMuography[46] = "97.4_4.05556_36";
  namesIntegralMuography[47] = "97.4_4.125_48";

  for(int m =0; m<48;m++){
    std::ifstream ifin(("htmltemplates/"+namesIntegralMuography[m]+".txt").c_str());
    if(!ifin) std::cout<<"Please copy the right html template!!!!!!! you will get seg fault"<<std::endl;
    if(debug) std::cout<<"Calling file for Vector"<<"htmltemplates/"+namesIntegralMuography[m]+".txt"<<std::endl;
    IntegralMuographyRawIdsVector[m].clear();
    if(ifin.is_open()){
      uint32_t rawId;
      std::string name;
      float stripl,stripw;
      int nstrips;
      while (ifin.good()){
	ifin >>rawId >>name >>stripl >>stripw >>nstrips;
	IntegralMuographyRawIdsVector[m].push_back(rawId);
	//if(debug) std::cout<<"Building Vectors,  name "<<name<<" vector "<<namesIntegralMuography[m].c_str()<<std::endl;
      }
    }
    ifin.close();
  }
  										  		   
  signal_bxendcap= new TH2F ("signal_BXEndCap","Signal BX Distribution for the End Cap",51,-5.0,5.0,51,0.,4.);
  signal_bxbarrel= new TH2F ("signal_BXBarrel","Signal BX Distribution for the Barrel",51,-5.0,5.0,51,0.,4.);
										  		   
  EffBarrel_black = new TH1F ("EffBarrel_black","Efficiency Distribution For All The Barrel",51,-1,101);
  DoubleGapBarrel_black = new TH1F ("DoubleGapBarrel_black","Double Gap Efficiency Distribution For All The Barrel",51,-1,101);
  CentralEffBarrel_black = new TH1F ("CentralEffBarrel_black","Efficiency in central part For All The Barrel",51,-1,101);
  										  		   
  EffBarrel = new TH1F ("EffBarrel","Efficiency Distribution For All The Barrel",51,-1,101);	   
  DoubleGapBarrel = new TH1F ("DoubleGapBarrel","Double Gap Efficiency Distribution For All The Barrel",51,-1,101);
  CentralEffBarrel = new TH1F ("CentralEffBarrel","Efficiency in central part For All The Barrel",51,-1,101);
  BXEffBarrel = new TH1F ("BXEffBarrel","Efficiency Distribution For All The Barrel with good BX",51,-1,101);
  badBXEffBarrel = new TH1F ("badBXEffBarrel","Efficiency Distribution For All The Barrel with bad BX",51,-1,101);

  EffDistroWm2= new TH1F ("EffDistroWm2near","Efficiency Distribution For Near Side Wheel -2",20,0.5,100.5);
  EffDistroWm1= new TH1F ("EffDistroWm1near","Efficiency Distribution For Near Side Wheel -1",20,0.5,100.5);
  EffDistroW0= new TH1F ("EffDistroW0near","Efficiency Distribution For Near Side Wheel 0",20,0.5,100.5);
  EffDistroW1= new TH1F ("EffDistroW1near","Efficiency Distribution For Near Side Wheel 1",20,0.5,100.5);
  EffDistroW2= new TH1F ("EffDistroW2near","Efficiency Distribution For Near Side Wheel 2",20,0.5,100.5);

  EffDistroWm2far= new TH1F ("EffDistroWm2far","Efficiency Distribution For Far Side Wheel -2",20,0.5,100.5);
  EffDistroWm1far= new TH1F ("EffDistroWm1far","Efficiency Distribution For Far Side Wheel -1",20,0.5,100.5);
  EffDistroW0far= new TH1F ("EffDistroW0far","Efficiency Distribution For Far Side Wheel 0",20,0.5,100.5);
  EffDistroW1far= new TH1F ("EffDistroW1far","Efficiency Distribution For Far Side Wheel 1",20,0.5,100.5);
  EffDistroW2far= new TH1F ("EffDistroW2far","Efficiency Distribution For Far Side Wheel 2",20,0.5,100.5);
  
  DoubleGapDistroWm2= new TH1F ("DoubleGapDistroWm2near","DoubleGapEfficiency Distribution For Near Side Wheel -2",20,0.5,100.5);
  DoubleGapDistroWm1= new TH1F ("DoubleGapDistroWm1near","DoubleGapEfficiency Distribution For Near Side Wheel -1",20,0.5,100.5);
  DoubleGapDistroW0= new TH1F ("DoubleGapDistroW0near","DoubleGapEfficiency Distribution For Near Side Wheel 0",20,0.5,100.5);
  DoubleGapDistroW1= new TH1F ("DoubleGapDistroW1near","DoubleGapEfficiency Distribution For Near Side Wheel 1",20,0.5,100.5);
  DoubleGapDistroW2= new TH1F ("DoubleGapDistroW2near","DoubleGapEfficiency Distribution For Near Side Wheel 2",20,0.5,100.5);
  
  DoubleGapDistroWm2far= new TH1F ("DoubleGapDistroWm2far","DoubleGapEfficiency Distribution For Far Side Wheel -2",20,0.5,100.5);
  DoubleGapDistroWm1far= new TH1F ("DoubleGapDistroWm1far","DoubleGapEfficiency Distribution For Far Side Wheel -1",20,0.5,100.5);
  DoubleGapDistroW0far= new TH1F ("DoubleGapDistroW0far","DoubleGapEfficiency Distribution For Far Side Wheel 0",20,0.5,100.5);
  DoubleGapDistroW1far= new TH1F ("DoubleGapDistroW1far","DoubleGapEfficiency Distribution For Far Side Wheel 1",20,0.5,100.5);
  DoubleGapDistroW2far= new TH1F ("DoubleGapDistroW2far","DoubleGapEfficiency Distribution For Far Side Wheel 2",20,0.5,100.5);

  EffEndCap_black= new TH1F ("EffDistroEndCap_black ","Efficiency Distribution For All The EndCaps",51,-1,101);
  DoubleGapEndCap_black = new TH1F ("DoubleGapEndCap_black","Double Gap Efficiency Distribution For All The EndCaps",51,-1,101);
  CentralEffEndCap_black = new TH1F ("CentralEffEndCap_black","Efficiency in central part For All The EndCaps",51,-1,101);

  EffEndCap= new TH1F ("EffDistroEndCap ","Efficiency Distribution For All The EndCaps",51,-1,101);
  DoubleGapEndCap = new TH1F ("DoubleGapEndCap","Double Gap Efficiency Distribution For All The EndCaps",51,-1,101);
  CentralEffEndCap = new TH1F ("CentralEffEndCap","Efficiency in central part For All The EndCaps",51,-1,101);
  BXEffEndCap = new TH1F ("BXEffEndCap","Efficiency Distribution For All The EndCap with good BX",51,-1,101);
  badBXEffEndCap = new TH1F ("badBXEffEndCap","Efficiency Distribution For All The EndCap with bad BX",51,-1,101);

  BXEffPositiveEndCap = new TH1F ("BXEffPositiveEndCap","Efficiency Distribution Positive EndCap with good BX",51,0.5,100.5);
  BXEffNegativeEndCap = new TH1F ("BXEffNegativeEndCap","Efficiency Distribution Negative EndCap with good BX",51,0.5,100.5);

  badBXEffPositiveEndCap = new TH1F ("badBXEffPositiveEndCap","Efficiency Distribution Positive EndCap with bad BX",51,0.5,100.5);
  badBXEffNegativeEndCap = new TH1F ("badBXEffNegativeEndCap","Efficiency Distribution Negative EndCap with bad BX",51,0.5,100.5);

  HeightVsEffR2 = new TH2F ("HeightVsEffR2","Height Vs Efficiency Ring 2",100,0.,1.,100,-1.,1.);
  HeightVsEffR3 = new TH2F ("HeightVsEffR3","Height Vs Efficiency Ring 3",100,0.,1.,100,-1.,1.);

  CLSWm2= new TH1F ("ClusterSizeWm2","Cluster Size Wheel -2 per sector",12,0.5,12.5);
  CLSWm1= new TH1F ("ClusterSizeWm1","Cluster Size Wheel -1 per sector",12,0.5,12.5);
  CLSW0= new TH1F ("ClusterSizeW0","Cluster Size Wheel 0 per sector",12,0.5,12.5);
  CLSW1= new TH1F ("ClusterSizeW1","Cluster Size Wheel 1 per sector",12,0.5,12.5);
  CLSW2= new TH1F ("ClusterSizeW2","Cluster Size Wheel 2 per sector",12,0.5,12.5);

  CLSStation1Ring2_A= new TH1F ("CLSStation1Ring2_A","CLSStation1Ring2_A",10,0.5,10.5);
  CLSStation1Ring2_B= new TH1F ("CLSStation1Ring2_B","CLSStation1Ring2_B",10,0.5,10.5);
  CLSStation1Ring2_C= new TH1F ("CLSStation1Ring2_C","CLSStation1Ring2_C",10,0.5,10.5);

  CLSStation23Ring2_A= new TH1F ("CLSStation23Ring2_A","CLSStation23Ring2_A",10,0.5,10.5);
  CLSStation23Ring2_B= new TH1F ("CLSStation23Ring2_B","CLSStation23Ring2_B",10,0.5,10.5);
  CLSStation23Ring2_C= new TH1F ("CLSStation23Ring2_C","CLSStation23Ring2_C",10,0.5,10.5);

  CLSStation123Ring3_A= new TH1F ("CLSStation123Ring3_A","CLSStation123Ring3_A",10,0.5,10.5);
  CLSStation123Ring3_B= new TH1F ("CLSStation123Ring3_B","CLSStation123Ring3_B",10,0.5,10.5);
  CLSStation123Ring3_C= new TH1F ("CLSStation123Ring3_C","CLSStation123Ring3_C",10,0.5,10.5);
  
  residualRB1in  = new TH1F("residualRB1in","residualRB1in",101,-20.,20.);
  residualRB1out  = new TH1F("residualRB1out","residualRB1out",101,-20.,20.);
  residualRB2in  = new TH1F("residualRB2in","residualRB2in",101,-20.,20.);
  residualRB2out  = new TH1F("residualRB2out","residualRB2out",101,-20.,20.);
  residualRB3  = new TH1F("residualRB3","residualRB3",101,-20.,20.);
  residualRB4  = new TH1F("residualRB4","residualRB4",101,-20.,20.);

  residualStation1Ring2_A  = new TH1F("residualStation1Ring2_A","residualStation1Ring2_A",101,-20.,20.);
  residualStation1Ring2_B  = new TH1F("residualStation1Ring2_B","residualStation1Ring2_B",101,-20.,20.);
  residualStation1Ring2_C  = new TH1F("residualStation1Ring2_C","residualStation1Ring2_C",101,-20.,20.);
  residualStation1Ring3_A  = new TH1F("residualStation1Ring3_A","residualStation1Ring3_A",101,-20.,20.);
  residualStation1Ring3_B  = new TH1F("residualStation1Ring3_B","residualStation1Ring3_B",101,-20.,20.);
  residualStation1Ring3_C  = new TH1F("residualStation1Ring3_C","residualStation1Ring3_C",101,-20.,20.);
  
  residualStation23Ring2_A = new TH1F("residualStation23Ring2_A","residualStation23Ring2_A",101,-20.,20.);
  residualStation23Ring2_B = new TH1F("residualStation23Ring2_B","residualStation23Ring2_B",101,-20.,20.);
  residualStation23Ring2_C = new TH1F("residualStation23Ring2_C","residualStation23Ring2_C",101,-20.,20.);
  residualStation23Ring3_A = new TH1F("residualStation23Ring3_A","residualStation23Ring3_A",101,-20.,20.);
  residualStation23Ring3_B = new TH1F("residualStation23Ring3_B","residualStation23Ring3_B",101,-20.,20.);
  residualStation23Ring3_C = new TH1F("residualStation23Ring3_C","residualStation23Ring3_C",101,-20.,20.);

  residualStation123Ring3_A = new TH1F("residualStation123Ring3_A","residualStation123Ring3_A",101,-20.,20.);
  residualStation123Ring3_B = new TH1F("residualStation123Ring3_B","residualStation123Ring3_B",101,-20.,20.);
  residualStation123Ring3_C = new TH1F("residualStation123Ring3_C","residualStation123Ring3_C",101,-20.,20.);
    
  residualDiskm1Ring2 = new TH1F("residualDiskm1Ring2","Residuals for Disk -1 Ring 2",101,-20.,20.);
  residualDiskm1Ring3 = new TH1F("residualDiskm1Ring3","Residuals for Disk -1 Ring 3",101,-20.,20.);
  residualDiskm2Ring2 = new TH1F("residualDiskm2Ring2","Residuals for Disk -2 Ring 2",101,-20.,20.);
  residualDiskm2Ring3 = new TH1F("residualDiskm2Ring3","Residuals for Disk -2 Ring 3",101,-20.,20.);
  residualDiskm3Ring2 = new TH1F("residualDiskm3Ring2","Residuals for Disk -3 Ring 2",101,-20.,20.);
  residualDiskm3Ring3 = new TH1F("residualDiskm3Ring3","Residuals for Disk -3 Ring 3",101,-20.,20.);

  residualDisk1Ring2  = new TH1F("residualDisk1Ring2","Residuals for Disk 1 Ring 2",101,-20.,20.);
  residualDisk1Ring3  = new TH1F("residualDisk1Ring3","Residuals for Disk 1 Ring 3",101,-20.,20.);
  residualDisk2Ring2  = new TH1F("residualDisk2Ring2","Residuals for Disk 2 Ring 2",101,-20.,20.);
  residualDisk2Ring3  = new TH1F("residualDisk2Ring3","Residuals for Disk 2 Ring 3",101,-20.,20.);
  residualDisk3Ring2  = new TH1F("residualDisk3Ring2","Residuals for Disk 3 Ring 2",101,-20.,20.);
  residualDisk3Ring3  = new TH1F("residualDisk3Ring3","Residuals for Disk 3 Ring 3",101,-20.,20.);

  std::stringstream meId; 
  std::stringstream title; 
  
  for(int k=1;k<=12;k++){
    meId <<"ClusterSizeWm2Sector"<<k; 
    title <<"Cluster Size Wheel - 2 Sector "<<k; 
    sectorCLSWm2[k] = new TH1F (meId.str().c_str(),title.str().c_str(),10,0.5,10.5); meId.str(""); title.str("");
    meId <<"ClusterSizeWm1Sector"<<k; 
    title <<"Cluster Size Wheel - 1 Sector "<<k;
    sectorCLSWm1[k] = new TH1F (meId.str().c_str(),title.str().c_str(),10,0.5,10.5); meId.str(""); title.str("");
    meId <<"ClusterSizeW0Sector"<<k;
    title <<"Cluster Size Wheel 0 Sector "<<k;
    sectorCLSW0[k] = new TH1F (meId.str().c_str(),title.str().c_str(),10,0.5,10.5); meId.str(""); title.str("");
    meId <<"ClusterSizeW1Sector"<<k;
    title <<"Cluster Size Wheel 1 Sector "<<k;
    sectorCLSW1[k] = new TH1F (meId.str().c_str(),title.str().c_str(),10,0.5,10.5); meId.str(""); title.str("");
    meId <<"ClusterSizeW2Sector"<<k;
    title <<"Cluster Size Wheel 2 Sector "<<k;
    sectorCLSW2[k] = new TH1F (meId.str().c_str(),title.str().c_str(),10,0.5,10.5); meId.str(""); title.str("");
  }

  CLSWm2layer = new TH1F ("ClusterSizeWm2","Cluster Size Wheel -2 per layer",12,0.5,12.5);
  CLSWm1layer = new TH1F ("ClusterSizeWm1","Cluster Size Wheel -1 per layer",12,0.5,12.5);
  CLSW0layer = new TH1F ("ClusterSizeW0","Cluster Size Wheel 0 per layer ",12,0.5,12.5);
  CLSW1layer = new TH1F ("ClusterSizeW1","Cluster Size Wheel 1 per layer",12,0.5,12.5);
  CLSW2layer = new TH1F ("ClusterSizeW2","Cluster Size Wheel 2 per layer",12,0.5,12.5);
  
  for(int k=1;k<=6;k++){
    meId <<"ClusterSizeWm2Layer"<<k;
    title <<"Cluster Size Wheel - 2 Layer "<<k;
    layerCLSWm2[k] = new TH1F (meId.str().c_str(),title.str().c_str(),10,0.5,10.5); meId.str(""); title.str("");
    meId <<"ClusterSizeWm1Layer"<<k;
    title <<"Cluster Size Wheel - 1 Layer "<<k;
    layerCLSWm1[k] = new TH1F (meId.str().c_str(),title.str().c_str(),10,0.5,10.5); meId.str(""); title.str("");
    meId <<"ClusterSizeW0Layer"<<k;
    title <<"Cluster Size Wheel 0 Layer "<<k;
    layerCLSW0[k] = new TH1F (meId.str().c_str(),title.str().c_str(),10,0.5,10.5); meId.str(""); title.str("");
    meId <<"ClusterSizeW1Layer"<<k;
    title <<"Cluster Size Wheel 1 Layer "<<k;
    layerCLSW1[k] = new TH1F (meId.str().c_str(),title.str().c_str(),10,0.5,10.5); meId.str(""); title.str("");
    meId <<"ClusterSizeWm2Layer"<<k;
    title <<"Cluster Size Wheel 2 Layer "<<k;
    layerCLSW2[k] = new TH1F (meId.str().c_str(),title.str().c_str(),10,0.5,10.5); meId.str(""); title.str("");
  }


  sectorEffWm2= new TH1F ("AzimutalDistroWm2","Efficiency per Sector Wheel -2",12,0.5,12.5);
  sectorEffWm1= new TH1F ("AzimutalDistroWm1","Efficiency per Sector Wheel -1",12,0.5,12.5);
  sectorEffW0= new TH1F ("AzimutalDistroW0","Efficiency per Sector Wheel 0",12,0.5,12.5);
  sectorEffW1= new TH1F ("AzimutalDistroW1","Efficiency per Sector Wheel 1",12,0.5,12.5);
  sectorEffW2= new TH1F ("AzimutalDistroW2","Efficiency per Sector Wheel 2",12,0.5,12.5);

  OcsectorEffWm2= new TH1F ("OcAzimutalDistroWm2","Occupancy per Sector Wheel -2",12,0.5,12.5);
  OcsectorEffWm1= new TH1F ("OcAzimutalDistroWm1","Occupancy per Sector Wheel -1",12,0.5,12.5);
  OcsectorEffW0= new TH1F ("OcAzimutalDistroW0","Ocuppancy per Sector Wheel 0",12,0.5,12.5);
  OcsectorEffW1= new TH1F ("OcAzimutalDistroW1","Ocuppancy per Sector Wheel 1",12,0.5,12.5);
  OcsectorEffW2= new TH1F ("OcAzimutalDistroW2","Ocupancy per Sector Wheel 2",12,0.5,12.5);

  ExsectorEffWm2= new TH1F ("ExAzimutalDistroWm2","Expected per Sector Wheel -2",12,0.5,12.5);
  ExsectorEffWm1= new TH1F ("ExAzimutalDistroWm1","Expected per Sector Wheel -1",12,0.5,12.5);
  ExsectorEffW0= new TH1F ("ExAzimutalDistroW0","Expected per Sector Wheel 0",12,0.5,12.5);
  ExsectorEffW1= new TH1F ("ExAzimutalDistroW1","Expected per Sector Wheel 1",12,0.5,12.5);
  ExsectorEffW2= new TH1F ("ExAzimutalDistroW2","Expected per Sector Wheel 2",12,0.5,12.5);
  
  GregD1R2= new TH1F ("GregDistroD1R2","Efficiency for Station 1 Ring 2",36,0.5,36.5);
  GregD1R3= new TH1F ("GregDistroD1R3","Efficiency for Station 1 Ring 3",36,0.5,36.5);
  GregD2R2= new TH1F ("GregDistroD2R2","Efficiency for Station 2 Ring 2",36,0.5,36.5);
  GregD2R3= new TH1F ("GregDistroD2R3","Efficiency for Station 2 Ring 3",36,0.5,36.5);
  GregD3R2= new TH1F ("GregDistroD3R2","Efficiency for Station 3 Ring 2",36,0.5,36.5);
  GregD3R3= new TH1F ("GregDistroD3R3","Efficiency for Station 3 Ring 3",36,0.5,36.5);
  
   // For black list
  GregD1R2_black= new TH1F ("GregDistroD1R2_black","Efficiency for Station 1 Ring 2 black",36,0.5,36.5);
  GregD1R3_black= new TH1F ("GregDistroD1R3_black","Efficiency for Station 1 Ring 3 black",36,0.5,36.5);
  GregD2R2_black= new TH1F ("GregDistroD2R2_black","Efficiency for Station 2 Ring 2 black",36,0.5,36.5);
  GregD2R3_black= new TH1F ("GregDistroD2R3_black","Efficiency for Station 2 Ring 3 black",36,0.5,36.5);
  GregD3R2_black= new TH1F ("GregDistroD3R2_black","Efficiency for Station 3 Ring 2 black",36,0.5,36.5);
  GregD3R3_black= new TH1F ("GregDistroD3R3_black","Efficiency for Station 3 Ring 3 black",36,0.5,36.5);

  OcGregD1R2= new TH1F ("OcGregDistroD1R2","Occupancy Distribution for Station 1 Ring 2",36,0.5,36.5);
  OcGregD1R3= new TH1F ("OcGregDistroD1R3","Occupancy Distribution for Station 1 Ring 3",36,0.5,36.5);
  OcGregD2R2= new TH1F ("OcGregDistroD2R2","Occupancy Distribution for Station 2 Ring 2",36,0.5,36.5);
  OcGregD2R3= new TH1F ("OcGregDistroD2R3","Occupancy Distribution for Station 2 Ring 3",36,0.5,36.5);
  OcGregD3R2= new TH1F ("OcGregDistroD3R2","Occupancy Distribution for Station 3 Ring 2",36,0.5,36.5);
  OcGregD3R3= new TH1F ("OcGregDistroD3R3","Occupancy Distribution for Station 3 Ring 3",36,0.5,36.5);

  ExGregD1R2= new TH1F ("ExGregDistroD1R2","Expected Distribution for Station 1 Ring 2",36,0.5,36.5);
  ExGregD1R3= new TH1F ("ExGregDistroD1R3","Expected Distribution for Station 1 Ring 3",36,0.5,36.5);
  ExGregD2R2= new TH1F ("ExGregDistroD2R2","Expected Distribution for Station 2 Ring 2",36,0.5,36.5);
  ExGregD2R3= new TH1F ("ExGregDistroD2R3","Expected Distribution for Station 2 Ring 3",36,0.5,36.5);
  ExGregD3R2= new TH1F ("ExGregDistroD3R2","Expected Distribution for Station 3 Ring 2",36,0.5,36.5);
  ExGregD3R3= new TH1F ("ExGregDistroD3R3","Expected Distribution for Station 3 Ring 3",36,0.5,36.5);

  GregDm1R2= new TH1F ("GregDistroDm1R2","Efficiency for Station - 1 Ring 2",36,0.5,36.5);
  GregDm1R3= new TH1F ("GregDistroDm1R3","Efficiency for Station - 1 Ring 3",36,0.5,36.5);
  GregDm2R2= new TH1F ("GregDistroDm2R2","Efficiency for Station - 2 Ring 2",36,0.5,36.5);
  GregDm2R3= new TH1F ("GregDistroDm2R3","Efficiency for Station - 2 Ring 3",36,0.5,36.5);
  GregDm3R2= new TH1F ("GregDistroDm3R2","Efficiency for Station - 3 Ring 2",36,0.5,36.5);
  GregDm3R3= new TH1F ("GregDistroDm3R3","Efficiency for Station - 3 Ring 3",36,0.5,36.5);

  // For black list
  GregDm1R2_black= new TH1F ("GregDistroDm1R2_black","Efficiency for Station - 1 Ring 2 black",36,0.5,36.5);
  GregDm1R3_black= new TH1F ("GregDistroDm1R3_black","Efficiency for Station - 1 Ring 3 black",36,0.5,36.5);
  GregDm2R2_black= new TH1F ("GregDistroDm2R2_black","Efficiency for Station - 2 Ring 2 black",36,0.5,36.5);
  GregDm2R3_black= new TH1F ("GregDistroDm2R3_black","Efficiency for Station - 2 Ring 3 black",36,0.5,36.5);
  GregDm3R2_black= new TH1F ("GregDistroDm3R2_black","Efficiency for Station - 3 Ring 2 black",36,0.5,36.5);
  GregDm3R3_black= new TH1F ("GregDistroDm3R3_black","Efficiency for Station - 3 Ring 3 black",36,0.5,36.5);

  OcGregDm1R2= new TH1F ("OcGregDistroDm1R2","Occupancy Distribution for Station - 1 Ring 2",36,0.5,36.5);
  OcGregDm1R3= new TH1F ("OcGregDistroDm1R3","Occupancy Distribution for Station - 1 Ring 3",36,0.5,36.5);
  OcGregDm2R2= new TH1F ("OcGregDistroDm2R2","Occupancy Distribution for Station - 2 Ring 2",36,0.5,36.5);
  OcGregDm2R3= new TH1F ("OcGregDistroDm2R3","Occupancy Distribution for Station - 2 Ring 3",36,0.5,36.5);
  OcGregDm3R2= new TH1F ("OcGregDistroDm3R2","Occupancy Distribution for Station - 3 Ring 2",36,0.5,36.5);
  OcGregDm3R3= new TH1F ("OcGregDistroDm3R3","Occupancy Distribution for Station - 3 Ring 3",36,0.5,36.5);

  ExGregDm1R2= new TH1F ("ExGregDistroDm1R2","Expected Distribution for Station - 1 Ring 2",36,0.5,36.5);
  ExGregDm1R3= new TH1F ("ExGregDistroDm1R3","Expected Distribution for Station - 1 Ring 3",36,0.5,36.5);
  ExGregDm2R2= new TH1F ("ExGregDistroDm2R2","Expected Distribution for Station - 2 Ring 2",36,0.5,36.5);
  ExGregDm2R3= new TH1F ("ExGregDistroDm2R3","Expected Distribution for Station - 2 Ring 3",36,0.5,36.5);
  ExGregDm3R2= new TH1F ("ExGregDistroDm3R2","Expected Distribution for Station - 3 Ring 2",36,0.5,36.5);
  ExGregDm3R3= new TH1F ("ExGregDistroDm3R3","Expected Distribution for Station - 3 Ring 3",36,0.5,36.5);

  EffDistroDm3= new TH1F ("EffDistroDm3near","Efficiency Distribution For Near Side Disk -3",20,0.5,100.5);  
  EffDistroDm2= new TH1F ("EffDistroDm2near","Efficiency Distribution For Near Side Disk -2",20,0.5,100.5);
  EffDistroDm1= new TH1F ("EffDistroDm1near","Efficiency Distribution For Near Side Disk -1",20,0.5,100.5);
  EffDistroD1= new TH1F ("EffDistroD1near","Efficiency Distribution For Near Side Disk 1",20,0.5,100.5);
  EffDistroD2= new TH1F ("EffDistroD2near","Efficiency Distribution For Near Side Disk 2",20,0.5,100.5);
  EffDistroD3= new TH1F ("EffDistroD3near","Efficiency Distribution For Near Side Disk 3",20,0.5,100.5);

  EffDistroDm3far= new TH1F ("EffDistroDm3far","Efficiency Distribution For Far Side Disk -3",20,0.5,100.5);
  EffDistroDm2far= new TH1F ("EffDistroDm2far","Efficiency Distribution For Far Side Disk -2",20,0.5,100.5);
  EffDistroDm1far= new TH1F ("EffDistroDm1far","Efficiency Distribution For Far Side Disk -1",20,0.5,100.5);
  EffDistroD1far= new TH1F ("EffDistroD1far","Efficiency Distribution For Far Side Disk 1",20,0.5,100.5);
  EffDistroD2far= new TH1F ("EffDistroD2far","Efficiency Distribution For Far Side Disk 2",20,0.5,100.5);
  EffDistroD3far= new TH1F ("EffDistroD3far","Efficiency Distribution For Far Side Disk 3",20,0.5,100.5);

  DoubleGapWm2= new TH1F ("DoubleGapEffWheel_-2near","Double Gap Efficiency Near Side Wheel -2",101,0.5,101.5);
  DoubleGapWm2far= new TH1F("DoubleGapEffWheel_-2far","Double Gap Efficiency Far Side Wheel -2",105,0.5,105.5);
  EffGlobWm2= new TH1F ("GlobEfficiencyWheel_-2near","Efficiency Near Side Wheel -2",101,0.5,101.5);
  EffGlobWm2far= new TH1F ("GlobEfficiencyWheel_-2far","Efficiency Far Side Wheel -2",105,0.5,105.5);
  BXGlobWm2=  new TH1F ("GlobBXWheel_-2near","BX Near Side Wheel -2",101,0.5,101.5);
  BXGlobWm2far=  new TH1F ("GlobBXWheel_-2far","BX Far Side Wheel -2",105,0.5,105.5);
  Signal_BXGlobWm2=  new TH1F ("Signal_GlobBXWheel_-2near","Signal BX Near Side Wheel -2",101,0.5,101.5);
  Signal_BXGlobWm2far=  new TH1F ("Signal_GlobBXWheel_-2far","Signal BX Far Side Wheel -2",105,0.5,105.5);
  MaskedGlobWm2=  new TH1F ("GlobMaskedWheel_-2near","Masked Near Side Wheel -2",101,0.5,101.5);
  MaskedGlobWm2far=  new TH1F ("GlobMaskedWheel_-2far","Masked Far Side Wheel -2",105,0.5,105.5);
  AverageEffWm2= new TH1F ("AverageEfficiencyWheel_-2near","Average Efficiency Near Side Wheel -2 ",101,0.5,101.5);
  AverageEffWm2far = new TH1F ("AverageEfficiencyWheel_-2far","Average Efficiency Far Side Wheel -2 ",105,0.5,105.5);
  NoPredictionWm2= new TH1F ("NoPredictionWheel_-2near","No Predictions Near Side Wheel -2 ",101,0.5,101.5);
  NoPredictionWm2far= new TH1F ("NoPredictionWheel_-2far","No Predictions Efficiency Far Side Wheel -2 ",105,0.5,105.5);
  
  DoubleGapWm1=  new TH1F ("DoubleGapEffWheel_-1near","Double Gap Efficiency Near Side Wheel -1",101,0.5,101.5);
  DoubleGapWm1far= new TH1F ("DoubleGapEffWheel_-1far","Double Gap Efficiency Far Side Wheel -1",105,0.5,105.5);
  EffGlobWm1=  new TH1F ("GlobEfficiencyWheel_-1near","Efficiency Near Side Wheel -1",101,0.5,101.5);
  EffGlobWm1far= new TH1F ("GlobEfficiencyWheel_-1far","Efficiency Far Side Wheel -1",105,0.5,105.5);
  BXGlobWm1=  new TH1F ("GlobBXWheel_-1near","BX Near Side Wheel -1",101,0.5,101.5);
  BXGlobWm1far=  new TH1F ("GlobBXWheel_-1far","BX Far Side Wheel -1",105,0.5,105.5);
  Signal_BXGlobWm1=  new TH1F ("Signal_GlobBXWheel_-1near","Signal BX Near Side Wheel -1",101,0.5,101.5);
  Signal_BXGlobWm1far=  new TH1F ("Signal_GlobBXWheel_-1far","Signal BX Far Side Wheel -1",105,0.5,105.5);
  MaskedGlobWm1=  new TH1F ("GlobMaskedWheel_-1near","Masked Near Side Wheel -1",101,0.5,101.5);
  MaskedGlobWm1far=  new TH1F ("GlobMaskedWheel_-1far","Masked Far Side Wheel -1",105,0.5,105.5);
  AverageEffWm1= new TH1F ("AverageEfficiencyWheel_-1near","Average Efficiency Near Side Wheel -1 ",101,0.5,101.5);
  AverageEffWm1far= new TH1F ("AverageEfficiencyWheel_-1far","Average Efficiency Far Side Wheel -1 ",105,0.5,105.5);
  NoPredictionWm1= new TH1F ("NoPredictionWheel_-1near","No Predictions Near Side Wheel -1 ",101,0.5,101.5);
  NoPredictionWm1far= new TH1F ("NoPredictionWheel_-1far","No Predictions Efficiency Far Side Wheel -1 ",105,0.5,105.5);

  DoubleGapW0 =  new TH1F ("DoubleGapEffWheel_0near","Double Gap Efficiency Near Side Wheel 0",101,0.5,101.5);
  DoubleGapW0far = new TH1F ("DoubleGapEffWheel_0far","Double Gap Efficiency Far Side Wheel 0",105,0.5,105.5);
  EffGlobW0 =  new TH1F ("GlobEfficiencyWheel_0near","Efficiency Near Side Wheel 0",101,0.5,101.5);
  EffGlobW0far = new TH1F ("GlobEfficiencyWheel_0far","Efficiency Far Side Wheel 0",105,0.5,105.5);
  BXGlobW0 =  new TH1F ("GlobBXWheel_0near","BX Near Side Wheel 0",101,0.5,101.5);
  BXGlobW0far =  new TH1F ("GlobBXWheel_0far","BX Far Side Wheel 0",105,0.5,105.5);
  Signal_BXGlobW0 =  new TH1F ("Signal_GlobBXWheel_0near","Signal BX Near Side Wheel 0",101,0.5,101.5);
  Signal_BXGlobW0far =  new TH1F ("Signal_GlobBXWheel_0far","Signal BX Far Side Wheel 0",105,0.5,105.5);
  MaskedGlobW0 =  new TH1F ("GlobMaskedWheel_0near","Masked Near Side Wheel 0",101,0.5,101.5);
  MaskedGlobW0far =  new TH1F ("GlobMaskedWheel_0far","Masked Far Side Wheel 0",105,0.5,105.5);
  AverageEffW0= new TH1F ("AverageEfficiencyWheel_0near","Average Efficiency Near Side Wheel 0 ",101,0.5,101.5);
  AverageEffW0far= new TH1F ("AverageEfficiencyWheel_0far","Average Efficiency Far Side Wheel 0 ",105,0.5,105.5);
  NoPredictionW0= new TH1F ("NoPredictionWheel_0near","No Predictions Near Side Wheel 0 ",101,0.5,101.5);
  NoPredictionW0far= new TH1F ("NoPredictionWheel_0far","No Predictions Efficiency Far Side Wheel 0 ",105,0.5,105.5);

  DoubleGapW1 =  new TH1F ("DoubleGapEffWheel_1near","Double Gap Efficiency Near Side Wheel 1",101,0.5,101.5);
  DoubleGapW1far = new TH1F ("DoubleGapEffWheel_1far","Double Gap Efficiency Far Side Wheel 1",105,0.5,105.5);  
  EffGlobW1 =  new TH1F ("GlobEfficiencyWheel_1near","Efficiency Near Side Wheel 1",101,0.5,101.5);
  EffGlobW1far = new TH1F ("GlobEfficiencyWheel_1far","Efficiency Far Side Wheel 1",105,0.5,105.5);  
  BXGlobW1 =  new TH1F ("GlobBXWheel_1near","BX Near Side Wheel 1",101,0.5,101.5);
  BXGlobW1far =  new TH1F ("GlobBXWheel_1far","BX Far Side Wheel 1",105,0.5,105.5);
  Signal_BXGlobW1 =  new TH1F ("Signal_GlobBXWheel_1near","Signal BX Near Side Wheel 1",101,0.5,101.5);
  Signal_BXGlobW1far =  new TH1F ("Signal_GlobBXWheel_1far","Signal BX Far Side Wheel 1",105,0.5,105.5);
  MaskedGlobW1 =  new TH1F ("GlobMaskedWheel_1near","Masked Near Side Wheel 1",101,0.5,101.5);
  MaskedGlobW1far =  new TH1F ("GlobMaskedWheel_1far","Masked Far Side Wheel 1",105,0.5,105.5);
  AverageEffW1= new TH1F ("AverageEfficiencyWheel_1near","Average Efficiency Near Side Wheel 1 ",101,0.5,101.5);
  AverageEffW1far= new TH1F ("AverageEfficiencyWheel_1far","Average Efficiency Far Side Wheel 1 ",105,0.5,105.5);
  NoPredictionW1= new TH1F ("NoPredictionWheel_1near","No Predictions Near Side Wheel 1 ",101,0.5,101.5);
  NoPredictionW1far= new TH1F ("NoPredictionWheel_1far","No Predictions Efficiency Far Side Wheel 1 ",105,0.5,105.5);

  DoubleGapW2 =  new TH1F ("DoubleGapEffWheel_2near","Double Gap Efficiency Near Side Wheel 2",101,0.5,101.5);
  DoubleGapW2far = new TH1F ("DoubleGapEffWheel_2far","Double Gap Efficiency Far Side Wheel 2",105,0.5,105.5);
  EffGlobW2 =  new TH1F ("GlobEfficiencyWheel_2near","Efficiency Near Side Wheel 2",101,0.5,101.5);
  EffGlobW2far = new TH1F ("GlobEfficiencyWheel_2far","Efficiency Far Side Wheel 2",105,0.5,105.5);
  BXGlobW2 =  new TH1F ("GlobBXWheel_2near","BX Near Side Wheel 2",101,0.5,101.5);
  BXGlobW2far =  new TH1F ("GlobBXWheel_2far","BX Far Side Wheel 2",105,0.5,105.5);
  Signal_BXGlobW2 =  new TH1F ("Signal_GlobBXWheel_2near","Signal BX Near Side Wheel 2",101,0.5,101.5);
  Signal_BXGlobW2far =  new TH1F ("Signal_GlobBXWheel_2far","Signal BX Far Side Wheel 2",105,0.5,105.5);
  MaskedGlobW2 =  new TH1F ("GlobMaskedWheel_2near","Masked Near Side Wheel 2",101,0.5,101.5);
  MaskedGlobW2far =  new TH1F ("GlobMaskedWheel_2far","Masked Far Side Wheel 2",105,0.5,105.5);
  AverageEffW2= new TH1F ("AverageEfficiencyWheel_2near","Average Efficiency Near Side Wheel 2 ",101,0.5,101.5);
  AverageEffW2far= new TH1F ("AverageEfficiencyWheel_2far","Average Efficiency Far Side Wheel 2 ",105,0.5,105.5);
  NoPredictionW2= new TH1F ("NoPredictionWheel_2near","No Predictions Near Side Wheel 2 ",101,0.5,101.5);
  NoPredictionW2far= new TH1F ("NoPredictionWheel_2far","No Predictions Efficiency Far Side Wheel 2 ",105,0.5,105.5);
  
  //EndCap

  EffGlobD3 = new TH1F ("GlobEfficiencyDisk_3near","Efficiency Near Side Disk 3",109,0.5,109.5);
  EffGlobD3far =new TH1F ("GlobEfficiencyDisk_3far","Efficiency Far Side Disk 3",109,0.5,109.5);
  BXGlobD3 = new TH1F ("GlobBXDisk_3near","BX Near Side Disk 3",109,0.5,109.5);
  BXGlobD3far = new TH1F ("GlobBXDisk_3far","BX Far Side Disk 3",109,0.5,109.5);
  Signal_BXGlobD3 = new TH1F ("Signal_GlobBXDisk_3near","Signal BX Near Side Disk 3",109,0.5,109.5);
  Signal_BXGlobD3far = new TH1F ("Signal_GlobBXDisk_3far","Signal BX Far Side Disk 3",109,0.5,109.5);
  MaskedGlobD3 = new TH1F ("GlobMaskedDisk_3near","Masked Near Side Disk 3",109,0.5,109.5);
  MaskedGlobD3far = new TH1F ("GlobMaskedDisk_3far","Masked Far Side Disk 3",109,0.5,109.5);
  AverageEffD3=new TH1F ("AverageEfficiencyDisk_3near","Average Efficiency Near Side Disk 3 ",109,0.5,109.5);
  AverageEffD3far=new TH1F ("AverageEfficiencyDisk_3far","Average Efficiency Far Side Disk 3 ",109,0.5,109.5);
  NoPredictionD3=new TH1F ("NoPredictionDisk_3near","No Predictions Near Side Disk 3 ",109,0.5,109.5);
  NoPredictionD3far=new TH1F ("NoPredictionDisk_3far","No Predictions Efficiency Far Side Disk 3 ",109,0.5,109.5);

  EffGlobD2 = new TH1F ("GlobEfficiencyDisk_2near","Efficiency Near Side Disk 2",109,0.5,109.5);
  EffGlobD2far =new TH1F ("GlobEfficiencyDisk_2far","Efficiency Far Side Disk 2",109,0.5,109.5);
  BXGlobD2 = new TH1F ("GlobBXDisk_2near","BX Near Side Disk 2",109,0.5,109.5);
  BXGlobD2far = new TH1F ("GlobBXDisk_2far","BX Far Side Disk 2",109,0.5,109.5);
  Signal_BXGlobD2 = new TH1F ("Signal_GlobBXDisk_2near","Signal BX Near Side Disk 2",109,0.5,109.5);
  Signal_BXGlobD2far = new TH1F ("Signal_GlobBXDisk_2far","Signal BX Far Side Disk 2",109,0.5,109.5);
  MaskedGlobD2 = new TH1F ("GlobMaskedDisk_2near","Masked Near Side Disk 2",109,0.5,109.5);
  MaskedGlobD2far = new TH1F ("GlobMaskedDisk_2far","Masked Far Side Disk 2",109,0.5,109.5);
  AverageEffD2=new TH1F ("AverageEfficiencyDisk_2near","Average Efficiency Near Side Disk 2 ",109,0.5,109.5);
  AverageEffD2far=new TH1F ("AverageEfficiencyDisk_2far","Average Efficiency Far Side Disk 2 ",109,0.5,109.5);
  NoPredictionD2=new TH1F ("NoPredictionDisk_2near","No Predictions Near Side Disk 2 ",109,0.5,109.5);
  NoPredictionD2far=new TH1F ("NoPredictionDisk_2far","No Predictions Efficiency Far Side Disk 2 ",109,0.5,109.5);

  EffGlobD1 = new TH1F ("GlobEfficiencyDisk_1near","Efficiency Near Side Disk 1",109,0.5,109.5);
  EffGlobD1far =new TH1F ("GlobEfficiencyDisk_1far","Efficiency Far Side Disk 1",109,0.5,109.5);
  BXGlobD1 = new TH1F ("GlobBXDisk_1near","BX Near Side Disk 1",109,0.5,109.5);
  BXGlobD1far = new TH1F ("GlobBXDisk_1far","BX Far Side Disk 1",109,0.5,109.5);
  Signal_BXGlobD1 = new TH1F ("Signal_GlobBXDisk_1near","Signal BX Near Side Disk 1",109,0.5,109.5);
  Signal_BXGlobD1far = new TH1F ("Signal_GlobBXDisk_1far","Signal BX Far Side Disk 1",109,0.5,109.5);
  MaskedGlobD1 = new TH1F ("GlobMaskedDisk_1near","Masked Near Side Disk 1",109,0.5,109.5);
  MaskedGlobD1far = new TH1F ("GlobMaskedDisk_1far","Masked Far Side Disk 1",109,0.5,109.5);
  AverageEffD1=new TH1F ("AverageEfficiencyDisk_1near","Average Efficiency Near Side Disk 1 ",109,0.5,109.5);
  AverageEffD1far=new TH1F ("AverageEfficiencyDisk_1far","Average Efficiency Far Side Disk 1 ",109,0.5,109.5);
  NoPredictionD1=new TH1F ("NoPredictionDisk_1near","No Predictions Near Side Disk 1 ",109,0.5,109.5);
  NoPredictionD1far=new TH1F ("NoPredictionDisk_1far","No Predictions Efficiency Far Side Disk 1 ",109,0.5,109.5);

  EffGlobDm1 = new TH1F ("GlobEfficiencyDisk_m1near","Efficiency Near Side Disk -1",109,0.5,109.5);
  EffGlobDm1far =new TH1F ("GlobEfficiencyDisk_m1far","Efficiency Far Side Disk -1",109,0.5,109.5);
  BXGlobDm1 = new TH1F ("GlobBXDisk_m1near","BX Near Side Disk -1",109,0.5,109.5);
  BXGlobDm1far = new TH1F ("GlobBXDisk_m1far","BX Far Side Disk -1",109,0.5,109.5);
  Signal_BXGlobDm1 = new TH1F ("Signal_GlobBXDisk_m1near","Signal BX Near Side Disk -1",109,0.5,109.5);
  Signal_BXGlobDm1far = new TH1F ("Signal_GlobBXDisk_m1far","Signal BX Far Side Disk -1",109,0.5,109.5);
  MaskedGlobDm1 = new TH1F ("GlobMaskedDisk_m1near","Masked Near Side Disk -1",109,0.5,109.5);
  MaskedGlobDm1far = new TH1F ("GlobMaskedDisk_m1far","Masked Far Side Disk -1",109,0.5,109.5);
  AverageEffDm1=new TH1F ("AverageEfficiencyDisk_m1near","Average Efficiency Near Side Disk -1 ",109,0.5,109.5);
  AverageEffDm1far=new TH1F ("AverageEfficiencyDisk_m1far","Average Efficiency Far Side Disk -1 ",109,0.5,109.5);
  NoPredictionDm1=new TH1F ("NoPredictionDisk_m1near","No Predictions Near Side Disk -1 ",109,0.5,109.5);
  NoPredictionDm1far=new TH1F ("NoPredictionDisk_m1far","No Predictions Efficiency Far Side Disk -1 ",109,0.5,109.5);

  EffGlobDm2 = new TH1F ("GlobEfficiencyDisk_m2near","Efficiency Near Side Disk -2",109,0.5,109.5);
  EffGlobDm2far =new TH1F ("GlobEfficiencyDisk_m2far","Efficiency Far Side Disk -2",109,0.5,109.5);
  BXGlobDm2 = new TH1F ("GlobBXDisk_m2near","BX Near Side Disk -2",109,0.5,109.5);
  BXGlobDm2far = new TH1F ("GlobBXDisk_m2far","BX Far Side Disk -2",109,0.5,109.5);
  Signal_BXGlobDm2 = new TH1F ("Signal_GlobBXDisk_m2near","Signal BX Near Side Disk -2",109,0.5,109.5);
  Signal_BXGlobDm2far = new TH1F ("Signal_GlobBXDisk_m2far","Signal BX Far Side Disk -2",109,0.5,109.5);
  MaskedGlobDm2 = new TH1F ("GlobMaskedDisk_m2near","Masked Near Side Disk -2",109,0.5,109.5);
  MaskedGlobDm2far = new TH1F ("GlobMaskedDisk_m2far","Masked Far Side Disk -2",109,0.5,109.5);
  AverageEffDm2=new TH1F ("AverageEfficiencyDisk_m2near","Average Efficiency Near Side Disk -2 ",109,0.5,109.5);
  AverageEffDm2far=new TH1F ("AverageEfficiencyDisk_m2far","Average Efficiency Far Side Disk -2 ",109,0.5,109.5);
  NoPredictionDm2=new TH1F ("NoPredictionDisk_m2near","No Predictions Near Side Disk -2 ",109,0.5,109.5);
  NoPredictionDm2far=new TH1F ("NoPredictionDisk_m2far","No Predictions Efficiency Far Side Disk -2 ",109,0.5,109.5);

  EffGlobDm3 = new TH1F ("GlobEfficiencyDisk_m3near","Efficiency Near Side Disk -3",109,0.5,109.5);
  EffGlobDm3far =new TH1F ("GlobEfficiencyDisk_m3far","Efficiency Far Side Disk -3",109,0.5,109.5);
  BXGlobDm3 = new TH1F ("GlobBXDisk_m3near","BX Near Side Disk -3",109,0.5,109.5);
  BXGlobDm3far = new TH1F ("GlobBXDisk_m3far","BX Far Side Disk -3",109,0.5,109.5);
  Signal_BXGlobDm3 = new TH1F ("Signal_GlobBXDisk_m3near","Signal BX Near Side Disk -3",109,0.5,109.5);
  Signal_BXGlobDm3far = new TH1F ("Signal_GlobBXDisk_m3far","Signal BX Far Side Disk -3",109,0.5,109.5);
  MaskedGlobDm3 = new TH1F ("GlobMaskedDisk_m3near","Masked Near Side Disk -3",109,0.5,109.5);
  MaskedGlobDm3far = new TH1F ("GlobMaskedDisk_m3far","Masked Far Side Disk -3",109,0.5,109.5);
  AverageEffDm3=new TH1F ("AverageEfficiencyDisk_m3near","Average Efficiency Near Side Disk -3 ",109,0.5,109.5);
  AverageEffDm3far=new TH1F ("AverageEfficiencyDisk_m3far","Average Efficiency Far Side Disk -3 ",109,0.5,109.5);
  if(debug) std::cout<<"Creating Problematic Histogram"<<std::endl;
  NoPredictionDm3=new TH1F ("NoPredictionDisk_m3near","No Predictions Near Side Disk -3 ",109,0.5,109.5);
  NoPredictionDm3far=new TH1F ("NoPredictionDisk_m3far","No Predictions Efficiency Far Side Disk -3 ",109,0.5,109.5);

  //Summary Histograms
  
  std::string os;
  os="Efficiency_Roll_vs_Sector_Wheel_-2";                                      
  Wheelm2Summary = new TH2F (os.c_str(), os.c_str(), 12, 0.5,12.5, 22, 0.5, 22.5);
  os="Efficiency_Roll_vs_Sector_Wheel_-1";                                      
  Wheelm1Summary = new TH2F (os.c_str(), os.c_str(), 12, 0.5,12.5, 22, 0.5, 22.5);
  os="Efficiency_Roll_vs_Sector_Wheel_0";                                      
  Wheel0Summary = new TH2F (os.c_str(), os.c_str(), 12, 0.5,12.5, 22, 0.5, 22.5);
  os="Efficiency_Roll_vs_Sector_Wheel_+1";                                      
  Wheel1Summary = new TH2F (os.c_str(), os.c_str(), 12, 0.5,12.5, 22, 0.5, 22.5);
  os="Efficiency_Roll_vs_Sector_Wheel_+2";                                      
  Wheel2Summary = new TH2F (os.c_str(), os.c_str(), 12, 0.5,12.5, 22, 0.5, 22.5);
  
  os="Efficiency_Roll_vs_Sector_Disk_-3";                                      
  Diskm3Summary = new TH2F (os.c_str(), os.c_str(), 36, 0.5,36.5, 6, 0.5, 6.5);
  os="Efficiency_Roll_vs_Sector_Disk_-2";                                      
  Diskm2Summary = new TH2F (os.c_str(), os.c_str(), 36, 0.5,36.5, 6, 0.5, 6.5);
  os="Efficiency_Roll_vs_Sector_Disk_-1";                                      
  Diskm1Summary = new TH2F (os.c_str(), os.c_str(), 36, 0.5,36.5, 6, 0.5, 6.5);
  os="Efficiency_Roll_vs_Sector_Disk_+1";                                      
  Disk1Summary = new TH2F (os.c_str(), os.c_str(), 36, 0.5,36.5, 6, 0.5, 6.5);
  os="Efficiency_Roll_vs_Sector_Disk_+2";                                      
  Disk2Summary = new TH2F (os.c_str(), os.c_str(), 36, 0.5,36.5, 6, 0.5, 6.5);
  os="Efficiency_Roll_vs_Sector_Disk_+3";                                      
  Disk3Summary = new TH2F (os.c_str(), os.c_str(), 36, 0.5,36.5, 6, 0.5, 6.5);

  os="PositiveEndCapSummary";
  PositiveEndCapSummary = new TH2F (os.c_str(), os.c_str(), 36, 0.5,36.5, 6, 0.5, 6.5);
  os="NegativeEndCapSummary";                            
  NegativeEndCapSummary = new TH2F (os.c_str(), os.c_str(), 36, 0.5,36.5, 6, 0.5, 6.5);

  
  MeanResiduals = new TH1F ("Mean_Residuals_Distribution","Mean_Residuals_Distribution",20,-5,5);
  MeanResiduals11 = new TH1F ("Mean_Residuals_Distribution_1cm","Mean_Residuals_Distribution_1cm",20,-1,1);
  AllResidualsBarrel  = new TH1F ("AllResiduals_Barrel","AllResidualsBarrel",101,-20,20);
  AllResidualsEndCap  = new TH1F ("AllResiduals_EndCap","AllResidualsEndCap",101,-20,20);

  RMSResiduals = new TH1F ("RMS_Residuals_Distribution","RMS_Residuals_Distribution",20,-5,5);
  RMSResiduals11 = new TH1F ("RMS_Residuals_Distribution_1cm","RMS_Residuals_Distribution_1cm",20,-1,1);
  
  //Producing plots for residuals and global statistics.

  
  gStyle->SetOptStat(stat);
  gStyle->SetPalette(1);
  
  std::string meIdRES,folder,labeltoSave,command;
  
  folder = "DQMData/Muons/MuonSegEff/";
  
  meIdRES = folder + "Statistics";

  Ca3->Clear();
  
  statistics = (TH1F*)theFile->Get(meIdRES.c_str());
  statistics->GetXaxis()->LabelsOption("v");
  statistics->GetXaxis()->SetLabelSize(0.035);
  statistics->Draw();
  
  labeltoSave = "Statistics.png";
  Ca3->SetBottomMargin(0.35);
  Ca3->SaveAs(labeltoSave.c_str()); 
  
  Ca3->Close();

  Ca4->Clear();

  meIdRES = folder + "CosAngMB3MB4Whm2"; CosAngMB3MB4Whm2 = (TH1F*)theFile->Get(meIdRES.c_str());
  meIdRES = folder + "CosAngMB3MB4Whm1"; CosAngMB3MB4Whm1 = (TH1F*)theFile->Get(meIdRES.c_str());
  meIdRES = folder + "CosAngMB3MB4Wh0"; CosAngMB3MB4Wh0 = (TH1F*)theFile->Get(meIdRES.c_str());
  meIdRES = folder + "CosAngMB3MB4Wh1"; CosAngMB3MB4Wh1 = (TH1F*)theFile->Get(meIdRES.c_str());
  meIdRES = folder + "CosAngMB3MB4Wh2"; CosAngMB3MB4Wh2 = (TH1F*)theFile->Get(meIdRES.c_str());
  
  CosAngMB3MB4= new TH1F ("CosAngMB3MB4","Angle Cosine Between MB3 and MB3",100,0.5,1.);
  CosAngMB3MB4->Add(CosAngMB3MB4Whm2);
  CosAngMB3MB4->Add(CosAngMB3MB4Whm1);
  CosAngMB3MB4->Add(CosAngMB3MB4Wh0);
  CosAngMB3MB4->Add(CosAngMB3MB4Wh1);
  CosAngMB3MB4->Add(CosAngMB3MB4Wh2);
		    
  if(CosAngMB3MB4->Integral()!=0){
    CosAngMB3MB4->Draw();
    CosAngMB3MB4->GetXaxis()->SetTitle("Angle Cosine Between MB3 and MB4 in XY projection");
    labeltoSave = "CosAngMB3MB4.png";
    Ca4->SaveAs(labeltoSave.c_str()); 
    
    Ca4->Clear();
    CosAngMB3MB4Whm2->Draw();
    CosAngMB3MB4Whm2->GetXaxis()->SetTitle("Angle Cosine Between MB3 and MB4 in XY projection Wheel -2");
    labeltoSave = "CosAngMB3MB4Whm2.png";
    Ca4->SaveAs(labeltoSave.c_str()); 

    Ca4->Clear();
    CosAngMB3MB4Whm1->Draw();
    CosAngMB3MB4Whm1->GetXaxis()->SetTitle("Angle Cosine Between MB3 and MB4 in XY projection Wheel -1");
    labeltoSave = "CosAngMB3MB4Whm1.png";
    Ca4->SaveAs(labeltoSave.c_str());

    Ca4->Clear();
    CosAngMB3MB4Wh0->Draw();
    CosAngMB3MB4Wh0->GetXaxis()->SetTitle("Angle Cosine Between MB3 and MB4 in XY projection Wheel 0");
    labeltoSave = "CosAngMB3MB4Wh0.png";
    Ca4->SaveAs(labeltoSave.c_str()); 

    Ca4->Clear();
    CosAngMB3MB4Wh1->Draw();
    CosAngMB3MB4Wh1->GetXaxis()->SetTitle("Angle Cosine Between MB3 and MB4 in XY projection Wheel 1");
    labeltoSave = "CosAngMB3MB4Wh1.png";
    Ca4->SaveAs(labeltoSave.c_str()); 

    Ca4->Clear();
    CosAngMB3MB4Wh2->Draw();
    CosAngMB3MB4Wh2->GetXaxis()->SetTitle("Angle Cosine Between MB3 and MB4 in XY projection Wheel 2");
    labeltoSave = "CosAngMB3MB4Wh2.png";
    Ca4->SaveAs(labeltoSave.c_str()); 

 
  }

  folder = "DQMData/Muons/MuonSegEff/Residuals/Barrel/";
 
  command = "mkdir resBarrel"; system(command.c_str());
  
  Ca4->Clear();
  
  if(barrel){
    meIdRES = folder + "GlobalResidualsClu1La1"; histoRES = (TH1F*)theFile->Get(meIdRES.c_str());  histoRES->Draw(); labeltoSave = "resBarrel/ResidualsClu1La1.png"; histoRES->GetXaxis()->SetTitle("(cm)");
    if(histoRES->GetEntries()!=0) Ca4->SetLogy(); Ca4->SaveAs(labeltoSave.c_str()); Ca4->Clear();
    meIdRES = folder + "GlobalResidualsClu1La2"; histoRES = (TH1F*)theFile->Get(meIdRES.c_str());  histoRES->Draw(); labeltoSave = "resBarrel/ResidualsClu1La2.png"; histoRES->GetXaxis()->SetTitle("(cm)");
    if(histoRES->GetEntries()!=0) Ca4->SetLogy(); Ca4->SaveAs(labeltoSave.c_str()); Ca4->Clear();
    meIdRES = folder + "GlobalResidualsClu1La3"; histoRES = (TH1F*)theFile->Get(meIdRES.c_str());  histoRES->Draw(); labeltoSave = "resBarrel/ResidualsClu1La3.png"; histoRES->GetXaxis()->SetTitle("(cm)");
    if(histoRES->GetEntries()!=0) Ca4->SetLogy(); Ca4->SaveAs(labeltoSave.c_str()); Ca4->Clear();
    meIdRES = folder + "GlobalResidualsClu1La4"; histoRES = (TH1F*)theFile->Get(meIdRES.c_str());  histoRES->Draw(); labeltoSave = "resBarrel/ResidualsClu1La4.png"; histoRES->GetXaxis()->SetTitle("(cm)");
    if(histoRES->GetEntries()!=0) Ca4->SetLogy(); Ca4->SaveAs(labeltoSave.c_str()); Ca4->Clear();
    meIdRES = folder + "GlobalResidualsClu1La5"; histoRES = (TH1F*)theFile->Get(meIdRES.c_str());  histoRES->Draw(); labeltoSave = "resBarrel/ResidualsClu1La5.png"; histoRES->GetXaxis()->SetTitle("(cm)");
    if(histoRES->GetEntries()!=0) Ca4->SetLogy(); Ca4->SaveAs(labeltoSave.c_str()); Ca4->Clear();
    meIdRES = folder + "GlobalResidualsClu1La6"; histoRES = (TH1F*)theFile->Get(meIdRES.c_str());  histoRES->Draw(); labeltoSave = "resBarrel/ResidualsClu1La6.png"; histoRES->GetXaxis()->SetTitle("(cm)");
    Ca4->SetLogy(); Ca4->SaveAs(labeltoSave.c_str()); Ca4->Clear();
    
    meIdRES = folder + "GlobalResidualsClu2La1"; histoRES = (TH1F*)theFile->Get(meIdRES.c_str());  histoRES->Draw(); labeltoSave = "resBarrel/ResidualsClu2La1.png"; histoRES->GetXaxis()->SetTitle("(cm)");
    if(histoRES->GetEntries()!=0) Ca4->SetLogy(); Ca4->SaveAs(labeltoSave.c_str()); Ca4->Clear();
    meIdRES = folder + "GlobalResidualsClu2La2"; histoRES = (TH1F*)theFile->Get(meIdRES.c_str());  histoRES->Draw(); labeltoSave = "resBarrel/ResidualsClu2La2.png"; histoRES->GetXaxis()->SetTitle("(cm)");
    if(histoRES->GetEntries()!=0) Ca4->SetLogy(); Ca4->SaveAs(labeltoSave.c_str()); Ca4->Clear();
    meIdRES = folder + "GlobalResidualsClu2La3"; histoRES = (TH1F*)theFile->Get(meIdRES.c_str());  histoRES->Draw(); labeltoSave = "resBarrel/ResidualsClu2La3.png"; histoRES->GetXaxis()->SetTitle("(cm)");
    if(histoRES->GetEntries()!=0) Ca4->SetLogy(); Ca4->SaveAs(labeltoSave.c_str()); Ca4->Clear();
    meIdRES = folder + "GlobalResidualsClu2La4"; histoRES = (TH1F*)theFile->Get(meIdRES.c_str());  histoRES->Draw(); labeltoSave = "resBarrel/ResidualsClu2La4.png"; histoRES->GetXaxis()->SetTitle("(cm)");
    if(histoRES->GetEntries()!=0) Ca4->SetLogy(); Ca4->SaveAs(labeltoSave.c_str()); Ca4->Clear();
    meIdRES = folder + "GlobalResidualsClu2La5"; histoRES = (TH1F*)theFile->Get(meIdRES.c_str());  histoRES->Draw(); labeltoSave = "resBarrel/ResidualsClu2La5.png"; histoRES->GetXaxis()->SetTitle("(cm)");
    if(histoRES->GetEntries()!=0) Ca4->SetLogy(); Ca4->SaveAs(labeltoSave.c_str()); Ca4->Clear();
    meIdRES = folder + "GlobalResidualsClu2La6"; histoRES = (TH1F*)theFile->Get(meIdRES.c_str());  histoRES->Draw(); labeltoSave = "resBarrel/ResidualsClu2La6.png"; histoRES->GetXaxis()->SetTitle("(cm)");
    if(histoRES->GetEntries()!=0) Ca4->SetLogy(); Ca4->SaveAs(labeltoSave.c_str()); Ca4->Clear();
    
    meIdRES = folder + "GlobalResidualsClu3La1"; histoRES = (TH1F*)theFile->Get(meIdRES.c_str());  histoRES->Draw(); labeltoSave = "resBarrel/ResidualsClu3La1.png"; histoRES->GetXaxis()->SetTitle("(cm)");
    if(histoRES->GetEntries()!=0) Ca4->SetLogy(); Ca4->SaveAs(labeltoSave.c_str()); Ca4->Clear();
    meIdRES = folder + "GlobalResidualsClu3La2"; histoRES = (TH1F*)theFile->Get(meIdRES.c_str());  histoRES->Draw(); labeltoSave = "resBarrel/ResidualsClu3La2.png"; histoRES->GetXaxis()->SetTitle("(cm)");
    if(histoRES->GetEntries()!=0) Ca4->SetLogy(); Ca4->SaveAs(labeltoSave.c_str()); Ca4->Clear();
    meIdRES = folder + "GlobalResidualsClu3La3"; histoRES = (TH1F*)theFile->Get(meIdRES.c_str());  histoRES->Draw(); labeltoSave = "resBarrel/ResidualsClu3La3.png"; histoRES->GetXaxis()->SetTitle("(cm)");
    if(histoRES->GetEntries()!=0) Ca4->SetLogy(); Ca4->SaveAs(labeltoSave.c_str()); Ca4->Clear();
    meIdRES = folder + "GlobalResidualsClu3La4"; histoRES = (TH1F*)theFile->Get(meIdRES.c_str());  histoRES->Draw(); labeltoSave = "resBarrel/ResidualsClu3La4.png"; histoRES->GetXaxis()->SetTitle("(cm)");
    if(histoRES->GetEntries()!=0) Ca4->SetLogy(); Ca4->SaveAs(labeltoSave.c_str()); Ca4->Clear();
    meIdRES = folder + "GlobalResidualsClu3La5"; histoRES = (TH1F*)theFile->Get(meIdRES.c_str());  histoRES->Draw(); labeltoSave = "resBarrel/ResidualsClu3La5.png"; histoRES->GetXaxis()->SetTitle("(cm)");
    if(histoRES->GetEntries()!=0) Ca4->SetLogy(); Ca4->SaveAs(labeltoSave.c_str()); Ca4->Clear();
    meIdRES = folder + "GlobalResidualsClu3La6"; histoRES = (TH1F*)theFile->Get(meIdRES.c_str());  histoRES->Draw(); labeltoSave = "resBarrel/ResidualsClu3La6.png"; histoRES->GetXaxis()->SetTitle("(cm)");
    if(histoRES->GetEntries()!=0) Ca4->SetLogy(); Ca4->SaveAs(labeltoSave.c_str()); Ca4->Clear();
  }
    
  folder = "DQMData/Muons/MuonSegEff/Residuals/EndCap/";
  
  command = "mkdir resEndCap"; system(command.c_str());
  
  meIdRES = folder + "GlobalResidualsClu1R2A"; histoRES = (TH1F*)theFile->Get(meIdRES.c_str());  histoRES->Draw(); labeltoSave = "resEndCap/ResidualsClu1R2A.png"; histoRES->GetXaxis()->SetTitle("(cm)");    if(histoRES->GetEntries()!=0)Ca4->SetLogy(); Ca4->SaveAs(labeltoSave.c_str()); Ca4->Clear();
  meIdRES = folder + "GlobalResidualsClu1R2B"; histoRES = (TH1F*)theFile->Get(meIdRES.c_str());  histoRES->Draw(); labeltoSave = "resEndCap/ResidualsClu1R2B.png"; histoRES->GetXaxis()->SetTitle("(cm)");    if(histoRES->GetEntries()!=0)Ca4->SetLogy(); Ca4->SaveAs(labeltoSave.c_str()); Ca4->Clear();
  meIdRES = folder + "GlobalResidualsClu1R2C"; histoRES = (TH1F*)theFile->Get(meIdRES.c_str());  histoRES->Draw(); labeltoSave = "resEndCap/ResidualsClu1R2C.png"; histoRES->GetXaxis()->SetTitle("(cm)");    if(histoRES->GetEntries()!=0)Ca4->SetLogy(); Ca4->SaveAs(labeltoSave.c_str()); Ca4->Clear();
  meIdRES = folder + "GlobalResidualsClu1R3A"; histoRES = (TH1F*)theFile->Get(meIdRES.c_str());  histoRES->Draw(); labeltoSave = "resEndCap/ResidualsClu1R3A.png"; histoRES->GetXaxis()->SetTitle("(cm)");    if(histoRES->GetEntries()!=0)Ca4->SetLogy(); Ca4->SaveAs(labeltoSave.c_str()); Ca4->Clear();
  meIdRES = folder + "GlobalResidualsClu1R3B"; histoRES = (TH1F*)theFile->Get(meIdRES.c_str());  histoRES->Draw(); labeltoSave = "resEndCap/ResidualsClu1R3B.png"; histoRES->GetXaxis()->SetTitle("(cm)");    if(histoRES->GetEntries()!=0)Ca4->SetLogy(); Ca4->SaveAs(labeltoSave.c_str()); Ca4->Clear();
  meIdRES = folder + "GlobalResidualsClu1R3C"; histoRES = (TH1F*)theFile->Get(meIdRES.c_str());  histoRES->Draw(); labeltoSave = "resEndCap/ResidualsClu1R3C.png"; histoRES->GetXaxis()->SetTitle("(cm)");    if(histoRES->GetEntries()!=0)Ca4->SetLogy(); Ca4->SaveAs(labeltoSave.c_str()); Ca4->Clear();
  meIdRES = folder + "GlobalResidualsClu2R2A"; histoRES = (TH1F*)theFile->Get(meIdRES.c_str());  histoRES->Draw(); labeltoSave = "resEndCap/ResidualsClu2R2A.png"; histoRES->GetXaxis()->SetTitle("(cm)");    if(histoRES->GetEntries()!=0)Ca4->SetLogy(); Ca4->SaveAs(labeltoSave.c_str()); Ca4->Clear();
  meIdRES = folder + "GlobalResidualsClu2R2B"; histoRES = (TH1F*)theFile->Get(meIdRES.c_str());  histoRES->Draw(); labeltoSave = "resEndCap/ResidualsClu2R2B.png"; histoRES->GetXaxis()->SetTitle("(cm)");    if(histoRES->GetEntries()!=0)Ca4->SetLogy(); Ca4->SaveAs(labeltoSave.c_str()); Ca4->Clear();
  meIdRES = folder + "GlobalResidualsClu2R2C"; histoRES = (TH1F*)theFile->Get(meIdRES.c_str());  histoRES->Draw(); labeltoSave = "resEndCap/ResidualsClu2R2C.png"; histoRES->GetXaxis()->SetTitle("(cm)");    if(histoRES->GetEntries()!=0)Ca4->SetLogy(); Ca4->SaveAs(labeltoSave.c_str()); Ca4->Clear();
  meIdRES = folder + "GlobalResidualsClu2R3A"; histoRES = (TH1F*)theFile->Get(meIdRES.c_str());  histoRES->Draw(); labeltoSave = "resEndCap/ResidualsClu2R3A.png"; histoRES->GetXaxis()->SetTitle("(cm)");    if(histoRES->GetEntries()!=0)Ca4->SetLogy(); Ca4->SaveAs(labeltoSave.c_str()); Ca4->Clear();
  meIdRES = folder + "GlobalResidualsClu2R3B"; histoRES = (TH1F*)theFile->Get(meIdRES.c_str());  histoRES->Draw(); labeltoSave = "resEndCap/ResidualsClu2R3B.png"; histoRES->GetXaxis()->SetTitle("(cm)");    if(histoRES->GetEntries()!=0)Ca4->SetLogy(); Ca4->SaveAs(labeltoSave.c_str()); Ca4->Clear();
  meIdRES = folder + "GlobalResidualsClu2R3C"; histoRES = (TH1F*)theFile->Get(meIdRES.c_str());  histoRES->Draw(); labeltoSave = "resEndCap/ResidualsClu2R3C.png"; histoRES->GetXaxis()->SetTitle("(cm)");    if(histoRES->GetEntries()!=0)Ca4->SetLogy(); Ca4->SaveAs(labeltoSave.c_str()); Ca4->Clear();
  meIdRES = folder + "GlobalResidualsClu3R2A"; histoRES = (TH1F*)theFile->Get(meIdRES.c_str());  histoRES->Draw(); labeltoSave = "resEndCap/ResidualsClu3R2A.png"; histoRES->GetXaxis()->SetTitle("(cm)");    if(histoRES->GetEntries()!=0)Ca4->SetLogy(); Ca4->SaveAs(labeltoSave.c_str()); Ca4->Clear();
  meIdRES = folder + "GlobalResidualsClu3R2B"; histoRES = (TH1F*)theFile->Get(meIdRES.c_str());  histoRES->Draw(); labeltoSave = "resEndCap/ResidualsClu3R2B.png"; histoRES->GetXaxis()->SetTitle("(cm)");    if(histoRES->GetEntries()!=0)Ca4->SetLogy(); Ca4->SaveAs(labeltoSave.c_str()); Ca4->Clear();
  meIdRES = folder + "GlobalResidualsClu3R2C"; histoRES = (TH1F*)theFile->Get(meIdRES.c_str());  histoRES->Draw(); labeltoSave = "resEndCap/ResidualsClu3R2C.png"; histoRES->GetXaxis()->SetTitle("(cm)");    if(histoRES->GetEntries()!=0)Ca4->SetLogy(); Ca4->SaveAs(labeltoSave.c_str()); Ca4->Clear();
  meIdRES = folder + "GlobalResidualsClu3R3A"; histoRES = (TH1F*)theFile->Get(meIdRES.c_str());  histoRES->Draw(); labeltoSave = "resEndCap/ResidualsClu3R3A.png"; histoRES->GetXaxis()->SetTitle("(cm)");    if(histoRES->GetEntries()!=0)Ca4->SetLogy(); Ca4->SaveAs(labeltoSave.c_str()); Ca4->Clear();
  meIdRES = folder + "GlobalResidualsClu3R3B"; histoRES = (TH1F*)theFile->Get(meIdRES.c_str());  histoRES->Draw(); labeltoSave = "resEndCap/ResidualsClu3R3B.png"; histoRES->GetXaxis()->SetTitle("(cm)");    if(histoRES->GetEntries()!=0)Ca4->SetLogy(); Ca4->SaveAs(labeltoSave.c_str()); Ca4->Clear();
  meIdRES = folder + "GlobalResidualsClu3R3C"; histoRES = (TH1F*)theFile->Get(meIdRES.c_str());  histoRES->Draw(); labeltoSave = "resEndCap/ResidualsClu3R3C.png"; histoRES->GetXaxis()->SetTitle("(cm)");    if(histoRES->GetEntries()!=0)Ca4->SetLogy(); Ca4->SaveAs(labeltoSave.c_str()); Ca4->Clear();

  Ca4->Close();
  
  //Setting Labels in Summary Label Barrel.
  
  std::stringstream binLabel;
  
  for(int i=1;i<=12;i++){
    binLabel.str("");
    binLabel<<"Sec "<<i;
    Wheelm2Summary->GetXaxis()->SetBinLabel(i,binLabel.str().c_str());
    Wheelm1Summary->GetXaxis()->SetBinLabel(i,binLabel.str().c_str());
    Wheel0Summary->GetXaxis()->SetBinLabel(i,binLabel.str().c_str());
    Wheel1Summary->GetXaxis()->SetBinLabel(i,binLabel.str().c_str());
    Wheel2Summary->GetXaxis()->SetBinLabel(i,binLabel.str().c_str());
  }
  
  std::map<int,std::string> rollNamesInter;

  rollNamesInter[1]="RB1in B";
  rollNamesInter[2]="RB1in F";
  rollNamesInter[3]="RB1out B";
  rollNamesInter[4]="RB1out F";
  rollNamesInter[5]="RB2in B";
  rollNamesInter[6]="RB2in M";
  rollNamesInter[7]="RB2in F";
  rollNamesInter[8]="RB2out B";
  rollNamesInter[9]="RB2out F";
  rollNamesInter[10]="RB3,- B";
  rollNamesInter[11]="RB3,- F";
  rollNamesInter[12]="RB3+ B";
  rollNamesInter[13]="RB3+ F";
  rollNamesInter[14]="RB4,- B";
  rollNamesInter[15]="RB4,- F";
  rollNamesInter[16]="RB4+ B";
  rollNamesInter[17]="RB4+ F";
  rollNamesInter[18]="RB4++ B";
  rollNamesInter[19]="RB4++ F";
  rollNamesInter[20]="RB4-- B";
  rollNamesInter[21]="RB4-- F";
  
  std::map<int,std::string> rollNamesExter;
  
  for(int i=1;i<=22;i++){
    rollNamesExter[i]=rollNamesInter[i];
    //if(debug) std::cout<<rollNamesInter[i]<<std::endl;
  }
  
  rollNamesExter[6]="RB2in F";
  rollNamesExter[7]="RB2out B";
  rollNamesExter[8]="RB2out M";
  
  for(int i=1;i<22;i++){
    Wheelm1Summary->GetYaxis()->SetBinLabel(i,rollNamesInter[i].c_str());
    Wheel0Summary->GetYaxis()->SetBinLabel(i,rollNamesInter[i].c_str());
    Wheel1Summary->GetYaxis()->SetBinLabel(i,rollNamesInter[i].c_str());
  }

  for(int i=1;i<22;i++){
    Wheelm2Summary->GetYaxis()->SetBinLabel(i,rollNamesExter[i].c_str());
    Wheel2Summary->GetYaxis()->SetBinLabel(i,rollNamesExter[i].c_str());
  }
  
  //Setting Labels in Summary Label Barrel.

  for(int i=1;i<=36;i++){
    binLabel.str("");
    binLabel<<i;
    Diskm3Summary->GetXaxis()->SetBinLabel(i,binLabel.str().c_str());
    Diskm2Summary->GetXaxis()->SetBinLabel(i,binLabel.str().c_str());
    Diskm1Summary->GetXaxis()->SetBinLabel(i,binLabel.str().c_str());
    Disk1Summary->GetXaxis()->SetBinLabel(i,binLabel.str().c_str());
    Disk2Summary->GetXaxis()->SetBinLabel(i,binLabel.str().c_str());
    Disk3Summary->GetXaxis()->SetBinLabel(i,binLabel.str().c_str());
    
    PositiveEndCapSummary->GetXaxis()->SetBinLabel(i,binLabel.str().c_str());
    NegativeEndCapSummary->GetXaxis()->SetBinLabel(i,binLabel.str().c_str());
  }

  for(int disk =1; disk <=3;disk++){
    for(int ri=2;ri<=3;ri++){
      binLabel.str("");
      binLabel<<"RE+"<<disk<<"/"<<ri;
      PositiveEndCapSummary->GetYaxis()->SetBinLabel((disk-1)*2+(ri-1),binLabel.str().c_str());
      binLabel.str("");
      binLabel<<"RE-"<<disk<<"/"<<ri;
      NegativeEndCapSummary->GetYaxis()->SetBinLabel((disk-1)*2+(ri-1),binLabel.str().c_str());
    }
  }

  for(int ri=2;ri<=3;ri++){
    for(int roll=1;roll<=3;roll++){
      binLabel.str("");
      if(roll==3) binLabel<<"Ring "<<ri<<" A";
      else if(roll==2) binLabel<<"Ring "<<ri<<" B";
      else if(roll==1) binLabel<<"Ring "<<ri<<" C";
      //if(debug) std::cout<<"Labeling EndCaps "<<binLabel.str()<<std::endl;
      Diskm3Summary->GetYaxis()->SetBinLabel((ri-2)*3+roll,binLabel.str().c_str());
      Diskm2Summary->GetYaxis()->SetBinLabel((ri-2)*3+roll,binLabel.str().c_str());
      Diskm1Summary->GetYaxis()->SetBinLabel((ri-2)*3+roll,binLabel.str().c_str());
      Disk1Summary->GetYaxis()->SetBinLabel((ri-2)*3+roll,binLabel.str().c_str());
      Disk2Summary->GetYaxis()->SetBinLabel((ri-2)*3+roll,binLabel.str().c_str());
      Disk3Summary->GetYaxis()->SetBinLabel((ri-2)*3+roll,binLabel.str().c_str());
    }
  }
  
  //exit(1);
  
  int indexWheel[5];
  for(int j=0;j<5;j++){
    indexWheel[j]=0;
  }
  
  int indexWheelf[5];
  for(int j=0;j<5;j++){
    indexWheelf[j]=0;
  }

  int indexDisk[6];
  for(int j=0;j<6;j++){
    indexDisk[j]=0;
  }
  
  int indexDiskf[6];
  for(int j=0;j<6;j++){
    indexDiskf[j]=0;
  }


  if(makehtml){

    command = "rm *.html" ; system(command.c_str());

    command = "cat htmltemplates/indexhead.html > indexDm3near.html"; system(command.c_str());
    command = "cat htmltemplates/indexhead.html > indexDm2near.html"; system(command.c_str());
    command = "cat htmltemplates/indexhead.html > indexDm1near.html"; system(command.c_str());

    command = "cat htmltemplates/indexhead.html > indexDm3far.html"; system(command.c_str());
    command = "cat htmltemplates/indexhead.html > indexDm2far.html"; system(command.c_str());
    command = "cat htmltemplates/indexhead.html > indexDm1far.html"; system(command.c_str());

    command = "cat htmltemplates/indexhead.html > indexD3near.html"; system(command.c_str());
    command = "cat htmltemplates/indexhead.html > indexD2near.html"; system(command.c_str());
    command = "cat htmltemplates/indexhead.html > indexD1near.html"; system(command.c_str());

    command = "cat htmltemplates/indexhead.html > indexD3far.html"; system(command.c_str());
    command = "cat htmltemplates/indexhead.html > indexD2far.html"; system(command.c_str());
    command = "cat htmltemplates/indexhead.html > indexD1far.html"; system(command.c_str());

    command = "cat htmltemplates/indexhead.html > indexWm2near.html"; system(command.c_str());
    command = "cat htmltemplates/indexhead.html > indexWm2far.html"; system(command.c_str());
    command = "cat htmltemplates/indexhead.html > indexWm1near.html"; system(command.c_str());
    command = "cat htmltemplates/indexhead.html > indexWm1far.html"; system(command.c_str());
    command = "cat htmltemplates/indexhead.html > indexW0near.html"; system(command.c_str());
    command = "cat htmltemplates/indexhead.html > indexW0far.html"; system(command.c_str());
    command = "cat htmltemplates/indexhead.html > indexW1near.html"; system(command.c_str());
    command = "cat htmltemplates/indexhead.html > indexW1far.html"; system(command.c_str());
    command = "cat htmltemplates/indexhead.html > indexW2near.html"; system(command.c_str());
    command = "cat htmltemplates/indexhead.html > indexW2far.html"; system(command.c_str());
    
  }
  
  //if(debug) std::cout<<"Before Rolls Loop"<<std::endl;
  

  
  theFileOut->cd();
  
  for (TrackingGeometry::DetContainer::const_iterator it=rpcGeo->dets().begin();it<rpcGeo->dets().end();it++){
    if( dynamic_cast< RPCChamber* >( *it ) != 0 ){
      RPCChamber* ch = dynamic_cast< RPCChamber* >( *it ); 
      std::vector< const RPCRoll*> roles = (ch->rolls());
      for(std::vector<const RPCRoll*>::const_iterator r = roles.begin();r != roles.end(); ++r){
	RPCDetId rpcId = (*r)->id();
	RPCGeomServ rpcsrv(rpcId);
	
	int sector = rpcId.sector();
	//int station = rpcId.station();

	int nstrips = int((*r)->nstrips());
	
	std::string name = rpcsrv.name();
	
	if(SkipProblematicChambers){
	  if(rpcId.region()!=0 && abs(rpcId.station())==3 && rpcId.ring()==2 && rpcId.roll()==3 ) continue; //skiping rolls with problems with the extrapolation methodin the endcap RE+/-3_R2_C
	  if(rpcId.region()!=0 && abs(rpcId.station())==2 && rpcId.ring()==3 && rpcId.roll()==1) continue; //skiping rolls with problems with the extrapolation methodin the endcap RE+/-2_R2_A
	}

	if(rpcId.region()==0 && barrel == false) continue;
	if(rpcId.region()!=0 && endcap == false ) continue;
		
	if(rpcId.region()==0 && barrel){  
	  
	  const RectangularStripTopology* top_= dynamic_cast<const RectangularStripTopology*> (&((*r)->topology()));
	  float stripl = top_->stripLength();
	  float stripw = top_->pitch();
	     
	  std::string detUnitLabel, meIdRPC, meIdRPC_2D, meIdDT, meIdDT_2D, meIdPRO, meIdPROY, meIdPROX,  signal_bxDistroId, bxDistroId, meIdRealRPC, meIdPRO_2D, meIdResidual, meIdResidual1, meIdResidual2, meIdResidual3, meIdResidualO, meIdCLS,meIdBXY,meIdINEF,meIdCEll_2D;
	  
	  RPCBookFolderStructure *  folderStr = new RPCBookFolderStructure(); //Anna
	  std::string folder = "DQMData/Muons/MuonSegEff/" +  folderStr->folderStructure(rpcId);

	  delete folderStr;

	  meIdRPC = folder +"/RPCDataOccupancy_"+ name;	
	  meIdDT =folder+"/ExpectedOccupancy_"+ name;

	  bxDistroId =folder+"/BXDistribution_"+ name;
	  signal_bxDistroId =folder+"/Signal_BXDistribution_"+ name;
	  meIdRealRPC =folder+"/RealDetectedOccupancy_"+ name;  

	  meIdPRO = "Profile_For_"+name;
	  meIdPROY = "Y_Profile_For_"+name;
	  meIdPROX = "X_Profile_For_"+name;
	  meIdPRO_2D = "Profile2D_For_"+name;
	  if(debug) std::cout<<" meIdPRO_2D "<<meIdPRO_2D<<std::endl;
	  meIdCEll_2D = "Efficiency_Cells_Distribution_For_"+name;
	  meIdResidual = folder+"/RPCResiduals_"+ name;
	  meIdResidual1 = folder+"/RPCResiduals_Clu1_"+ name;
	  meIdResidual2 = folder+"/RPCResiduals_Clu2_"+ name;
	  meIdResidual3 = folder+"/RPCResiduals_Clu3_"+ name;
	  meIdResidualO = folder+"/RPCResiduals_Other_"+ name;

	  meIdDT_2D = folder+"/ExpectedOccupancy2D_"+ name;
	  meIdRPC_2D = folder +"/RPCDataOccupancy2D_"+ name;	
	  meIdCLS = folder +"/CLSDistribution_"+ name;	
	  meIdBXY = folder +"/BXYDistribution_"+ name;
	  meIdINEF = folder +"/Inefficiency2D_"+ name;

	  histoRPC_2D= (TH2F*)theFile->Get(meIdRPC_2D.c_str());
	  histoDT_2D= (TH2F*)theFile->Get(meIdDT_2D.c_str());

	  for(int m=0;m<48;m++){
	    if(!(find(IntegralMuographyRawIdsVector[m].begin(),IntegralMuographyRawIdsVector[m].end(),rpcId.rawId())
		 ==IntegralMuographyRawIdsVector[m].end())){

	      if(!IntegralMuography[m]){
		assert(!IntegralMuographyExp[m]);
		assert(!IntegralMuographyObs[m]);
		
		std::cout<<"doing first booking for IntegralMuography[m="<<m<<"] with "<< name<<std::endl;
		
		IntegralMuography[m] = new TH2F(namesIntegralMuography[m].c_str(),namesIntegralMuography[m].c_str()
						,histoRPC_2D->GetNbinsX(),histoRPC_2D->GetXaxis()->GetXmin(),histoRPC_2D->GetXaxis()->GetXmax()
						,histoRPC_2D->GetNbinsY(),histoRPC_2D->GetYaxis()->GetXmin(),histoRPC_2D->GetYaxis()->GetXmax());  
		
		IntegralMuographyObs[m] = new TH2F((namesIntegralMuography[m]+"Obs").c_str(),(namesIntegralMuography[m]+"Obs").c_str()
						,histoRPC_2D->GetNbinsX(),histoRPC_2D->GetXaxis()->GetXmin(),histoRPC_2D->GetXaxis()->GetXmax()
						,histoRPC_2D->GetNbinsY(),histoRPC_2D->GetYaxis()->GetXmin(),histoRPC_2D->GetYaxis()->GetXmax());  

		IntegralMuographyExp[m] = new TH2F((namesIntegralMuography[m]+"Exp").c_str(),(namesIntegralMuography[m]+"Exp").c_str()
						,histoRPC_2D->GetNbinsX(),histoRPC_2D->GetXaxis()->GetXmin(),histoRPC_2D->GetXaxis()->GetXmax()
						,histoRPC_2D->GetNbinsY(),histoRPC_2D->GetYaxis()->GetXmin(),histoRPC_2D->GetYaxis()->GetXmax());  
		
		IntegralMuography[m]->Add(histoRPC_2D);
		IntegralMuographyExp[m]->Add(histoDT_2D);
	      }else{
		IntegralMuographyObs[m]->Add(histoRPC_2D);
		IntegralMuographyExp[m]->Add(histoDT_2D);
	      }
	    }
	  }
	  
	  histoResidual1= (TH1F*)theFile->Get(meIdResidual1.c_str());
	  histoResidual2= (TH1F*)theFile->Get(meIdResidual2.c_str());
	  histoResidual3= (TH1F*)theFile->Get(meIdResidual3.c_str());
	  histoResidualO= (TH1F*)theFile->Get(meIdResidualO.c_str());

	  if(!histoResidual1 || !histoResidual2 || !histoResidual3 || !histoResidualO) std::cout<<"Warning one of the histo residuals plot doesn't exist"<<std::endl;
	  
	  histoResidual = new TH1F (meIdResidual.c_str(),meIdResidual.c_str(),101,-20,20);
	  histoResidual->Add(histoResidual1);
	  histoResidual->Add(histoResidual2);
	  histoResidual->Add(histoResidual3);
	  histoResidual->Add(histoResidualO);

	  AllResidualsBarrel->Add(histoResidual);
	  
	  int layer = 0;
	  	    
	  if(rpcId.station()==1&&rpcId.layer()==1) layer = 1;
	  else if(rpcId.station()==1&&rpcId.layer()==2) layer = 2;
	  else if(rpcId.station()==2&&rpcId.layer()==1) layer = 3;
	  else if(rpcId.station()==2&&rpcId.layer()==2)  layer = 4;
	  else if(rpcId.station()==3) layer = 5;
	  else if(rpcId.station()==4) layer = 6;
	  
	  if(layer==1) residualRB1in->Add(histoResidual);
	  if(layer==2) residualRB1out->Add(histoResidual);
	  if(layer==3) residualRB2in->Add(histoResidual);
	  if(layer==4) residualRB2out->Add(histoResidual);
	  if(layer==5) residualRB3->Add(histoResidual);
	  if(layer==6) residualRB4->Add(histoResidual);

	  histoINEF= (TH2F*)theFile->Get(meIdINEF.c_str());
	  
	  const int n = 20;
	  
	  float x[n];
	  float y[n];
	  float ex[n];
	  float ey[n];
	  	  
	  histoCLS= (TH1F*)theFile->Get(meIdCLS.c_str());
	  histoBXY= (TH2F*)theFile->Get(meIdBXY.c_str());
	  
	  if(histoBXY && histoCLS){
	    if(debug) std::cout<<"the histograms exist"<<std::endl;
	    
	    float step = stripl/float(n);
	    for(int i=0;i<n;i++){
	      float mean = histoBXY->ProjectionX("_px",i,i+1)->GetMean();
	      float entries = histoBXY->ProjectionX("_px",i,i+1)->GetEntries();
	      float error = histoBXY->ProjectionX("_px",i,i+1)->GetRMS()/ sqrt(entries);
	      
	      x[i]=(i+1)*step;
	      ex[i]=step*0.5;
	      y[i]=mean;
	      ey[i]=error;
	    }
	    
	  }else{
	    if(debug) std::cout<<"WARNING!!! one of the two histograms (histoBXY or histoCLS) doesn't exist"<<std::endl;
	    if(debug) std::cout<<meIdBXY<<" "<<meIdCLS<<std::endl;
	  }
	  
	  histoRPC= (TH1F*)theFile->Get(meIdRPC.c_str());
          histoDT= (TH1F*)theFile->Get(meIdDT.c_str());
          BXDistribution = (TH1F*)theFile->Get(bxDistroId.c_str());
	  Signal_BXDistribution = (TH1F*)theFile->Get(signal_bxDistroId.c_str());
          histoRealRPC = (TH1F*)theFile->Get(meIdRealRPC.c_str());
	  
	  histoPRO= new TH1F (meIdPRO.c_str(),meIdPRO.c_str(),nstrips,0.5,nstrips+0.5);
	  histoPRO_2D= new TH2F (meIdPRO_2D.c_str(),meIdPRO_2D.c_str(),
				 2*(int)(0.6*nstrips*stripw),-0.6*nstrips*stripw,0.6*nstrips*stripw,
				 2*(int)(0.6*stripl),-0.6*stripl,0.6*stripl);

	  histoCellDistro = new TH1F (meIdCEll_2D.c_str(),meIdCEll_2D.c_str(),101,-0.5,100.5);

	  histoPROY = new TH1F (meIdPROY.c_str(),meIdPROY.c_str(),2*(int)(0.6*stripl),-0.6*stripl,0.6*stripl);
	  histoPROX = new TH1F (meIdPROX.c_str(),meIdPROX.c_str(),2*(int)(0.6*nstrips*stripw),-0.6*stripw*nstrips,0.6*stripw*nstrips);
	  
	  
	  //if(debug) std::cout <<folder<<"/"<<name<<std::endl;

	  //Cesare
          float effRoll =0;
          float errEffRoll = 0;
          float DT_Int = 0;
	  float RPCpoints = 0; 
          float MeanCLSRollBarrel = 0;

	  int NumberMasked=0;
	  int NumberWithOutPrediction=0;
	  double p = 0;
	  double o = 0;
	  float mybxhisto = 0;
	  float mybxerror = 0;
	  float signal_mybxhisto = 0;
	  float signal_mybxerror = 0;
	  float ef =0;
	  float er =0;
	  float efY =0;
	  float erY =0;
	  float efX =0;
	  float erX =0;
	  float ef2D =0;
	  float er2D =0;
	  float buffef = 0;
	  float buffer = 0;
	  float sumbuffef = 0;
	  float averageeff = 0.;
	  float averageerr = 0.;

	  float averageerrexp = 0.;	  
	  float averageerrocc = 0.;

	  float doublegapeff = 0;
	  float doublegaperr = 0;

	  float bufdoublegapeff = 0;
	  
	  float doublegaperrocc = 0;
	  float doublegaperrexp = 0;

	  float pinoeff = 0.;
	  float pinoerr = 0.;
	  
	  int NumberStripsPointed = 0;
	  
	  if(histoRPC_2D && histoDT_2D && histoResidual && barrel){ 
	    //if(debug) std::cout<<"Leidos los histogramas 2D!"<<std::endl;
	    for(int i=1;i<=2*(int)(0.6*nstrips*stripw);++i){
	      for(int j=1;j<=2*(int)(0.6*stripl);++j){
		ef2D=0.;
		er2D=0.;
		if(histoDT_2D->GetBinContent(i,j) != 0){
		  ef2D = histoRPC_2D->GetBinContent(i,j)/histoDT_2D->GetBinContent(i,j);
		  er2D = sqrt(ef2D*(1-ef2D)/histoDT_2D->GetBinContent(i,j));
		  histoCellDistro->Fill(ef2D*100);
		  histoPRO_2D->SetBinContent(i,j,ef2D*100.);
		  histoPRO_2D->SetBinError(i,j,er2D*100.);
		}
	      }//loop on the boxes
	    }
	    
	    TH1D * profileRPC_Y =  histoRPC_2D->ProjectionY(" ",int(0.6*nstrips*stripw)-int(nstrips*stripw*0.5),int(0.6*nstrips*stripw)+int(nstrips*stripw*0.5));
	    TH1D *  profileDT_Y =  histoDT_2D->ProjectionY(" ",int(0.6*nstrips*stripw)-int(nstrips*stripw*0.5),int(0.6*nstrips*stripw)+int(nstrips*stripw*0.5));
	    
	    for(int j=1;j<=2*(int)(0.6*stripl);++j){
	      efY=0.;
	      erY=0.;
	      if(profileDT_Y->GetBinContent(j) != 0){
		efY = profileRPC_Y->GetBinContent(j)/profileDT_Y->GetBinContent(j);
		erY = sqrt(efY*(1-efY)/profileDT_Y->GetBinContent(j));
	      }	
	      //if(debug) std::cout<<"Filling Y profile Y="<<j<<" efY="<<efY*100.<<" erY="<<erY*100.<<std::endl;
	      histoPROY->SetBinContent(j,efY);
	      histoPROY->SetBinError(j,erY);
	    }

	    TH1D * profileRPC_X =  histoRPC_2D->ProjectionX(" ",int(0.6*stripl)-int(stripl*0.5),int(0.6*stripl)+int(stripl*0.5));
	    TH1D *  profileDT_X =  histoDT_2D->ProjectionX(" ",int(0.6*stripl)-int(stripl*0.5),int(0.6*stripl)+int(stripl*0.5));
	    
	    for(int i=1;i<=2*(int)(0.6*nstrips*stripw);++i){
	      efX=0.;
	      erX=0.;
	      if(profileDT_X->GetBinContent(i) != 0){
		efX = profileRPC_X->GetBinContent(i)/profileDT_X->GetBinContent(i);
		erX = sqrt(efX*(1-efX)/profileDT_X->GetBinContent(i));
	      }	
	      //if(debug) std::cout<<"Filling X profile X="<<i<<" efX="<<efX*100.<<" erX="<<erX*100.<<std::endl;
	      histoPROX->SetBinContent(i,efX);
	      histoPROX->SetBinError(i,erX);
	    }

	    //ac[a hay que poner el fiducial cut en cm
	    int firstxbin = int(0.1*nstrips*stripw+fiducialcutx);
	    int lastxbin = int(1.1*nstrips*stripw-fiducialcutx);
	    int firstybin = int(0.1*stripl+fiducialcuty);
	    int lastybin = int(1.1*stripl-fiducialcuty);
	    
	    if(debug) std::cout<<" firstxbin "<<firstxbin<<" lastxbin "<<lastxbin;
	    if(debug) std::cout<<" firstybin "<<firstybin<<" lastybin "<<lastybin;
	      
	    float pinoobserved = histoRPC_2D->Integral(firstxbin,lastxbin,firstybin,lastybin);
	    float pinoexpected = histoDT_2D->Integral(firstxbin,lastxbin,firstybin,lastybin);

	    if(pinoexpected != 0){
	      pinoeff = (pinoobserved/pinoexpected)*100;
	      pinoerr = 100.*sqrt((pinoobserved/pinoexpected)*(1.-(pinoobserved/pinoexpected))/pinoexpected);
	    }

	    if(debug) std::cout<<"Filling txt file with roll and efficiency"<<std::endl;
	    RollYEff<<name<<" "<<pinoeff<<" "<<pinoerr<<" "<<pinoexpected<<" "<<histoCLS->GetMean()<<" ";
	    database<<name<<" "<<pinoeff<<" "<<pinoerr<<" "<<pinoexpected<<" "<<histoCLS->GetMean()<<" ";
	    
	    int nBins = histoCLS->GetSize();
	    for(int i=1; i<(nBins-1); i++){
	      double numCLS = histoCLS->Integral(i,(nBins-1));
	      double denCLS = histoCLS->Integral(1,(nBins-1));
	      double probCLS = 0;
	      if(denCLS != 0) probCLS = numCLS/denCLS;
	      else probCLS = -1;
	      RollYEff<<probCLS<<" ";
	    }

	    RollYEff<<std::endl;

	    float integralCLS = histoCLS->Integral()+histoCLS->GetBinContent(11);
	    for(int bin=1;bin<=11;bin++){
	      if(integralCLS!=0){
		double normCLS = histoCLS->GetBinContent(bin)/integralCLS;
		database<<normCLS<<" ";
	      }
	      else{
		database<<"-1 ";
	      }
	    }
	    
 	    for(int strip = 1;strip<histoRealRPC->GetSize()-1;strip++){
 	      database<<histoRealRPC->GetBinContent(strip)<<" ";
 	    }
	    
	    if(histoRealRPC->GetSize()<=92){
	      for(int extra = histoRealRPC->GetSize()-2;extra<90;extra++){
		database<<"-1 ";
	      }
	    }
	    
	    database<<std::endl;
	    
	    if(debug) std::cout<<" deleting profiles"<<std::endl;

	    delete profileDT_X;
	    delete profileDT_Y;
	    delete profileRPC_X;
	    delete profileRPC_Y;

	  }else{
	    if(debug) std::cout<<"Warning!!! Alguno de los  histogramas 2D no fue leido!"<<std::endl;
	  }

	  bool maskeffect[100];
	  for(int i=0;i<100;i++) maskeffect[i]=false;
	    
	  if(histoRPC && histoDT && BXDistribution && histoRealRPC && Signal_BXDistribution){
	    for(int i=1;i<=nstrips;++i){
	      if(histoRealRPC->GetBinContent(i)==0 || histoDT->GetBinContent(i)==0){
		if(debug) std::cout<<"1";
		if(i==1){
		  maskeffect[1]=true;
		  maskeffect[2]=true;
		  maskeffect[3]=true;
		}else if(i==2){
		  maskeffect[1]=true;
		  maskeffect[2]=true;
		  maskeffect[3]=true;
		  maskeffect[4]=true;
		}else if(i==nstrips){
		  maskeffect[nstrips-2]=true;
		  maskeffect[nstrips-1]=true;
		  maskeffect[nstrips]=true;
		}else if(i==nstrips-1){
		  maskeffect[nstrips-3]=true;
		  maskeffect[nstrips-2]=true;
		  maskeffect[nstrips-1]=true;
		  maskeffect[nstrips]=true;
		}else{
		  maskeffect[i-2]=true;
		  maskeffect[i-1]=true;
		  maskeffect[i]=true;
		  maskeffect[i+1]=true;
		  maskeffect[i+2]=true;
		}
	      }else{
		if(debug) std::cout<<"0";
	      }
	    }
	    
	    //Border Effect
	    //maskeffect[nstrips]=true;
	    //maskeffect[nstrips-1]=true;
	    //maskeffect[nstrips-2]=true;
	    //maskeffect[nstrips-3]=true;
	    //maskeffect[nstrips-4]=true;
	    //maskeffect[nstrips-5]=true;
	    
	    if(debug) std::cout<<std::endl;
	    
	    float withouteffect=0.;
	    
	    for(int i=1;i<=nstrips;i++){
	      if(maskeffect[i]==false){
		withouteffect++;
		if(debug) std::cout<<"0";
	      }else{
		if(debug) std::cout<<"1";
	      }
	    }

	    if(debug) std::cout<<std::endl;
	    
	    efftxt<<name<<"  "<<rpcId.rawId();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            DT_Int = float(histoDT->Integral());
	    RPCpoints = float(histoRPC->Integral());

            if(DT_Int != 0 ){
              if(!IsBadRoll(rpcId.rawId(),blacklist)){
            	effRoll = float(histoRPC->Integral())/float(histoDT->Integral());
                errEffRoll = (sqrt(effRoll*(1-effRoll)/DT_Int));
                EffBarrelRoll->Fill(100*effRoll);}
	    }
            
            float effRollNOBL = 100*(float(histoRPC->Integral())/float(histoDT->Integral()));
            
            HitDen->Fill(DT_Int);

            MeanCLSRollBarrel = histoCLS->GetMean();
            
            RollBeff<<rpcId.rawId()<<" "<<name<<" "<<effRollNOBL<<" "<<RPCpoints<<" "<<DT_Int<<" "<<MeanCLSRollBarrel<<std::endl;

            EffBarrelStrip<<rpcId.rawId()<<" "<<name<<" "; 
            for(int i=1;i<=nstrips;i++){
		 double occStrip = histoRPC->GetBinContent(i);
		 double expStrip = histoDT->GetBinContent(i);
		 double effStrip = 0;
		 if(expStrip != 0) effStrip = 100*occStrip/expStrip;
            	 EffBarrelStrip<<" "<<occStrip<<" "<<expStrip<<" "<<effStrip;
             }
            EffBarrelStrip<<std::endl;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	    if(debug) std::cout<<"Before the loop for the strips profiles"<<std::endl;

	    for(int i=1;i<=nstrips;++i){
	      if(histoRealRPC->GetBinContent(i)==0){
		NumberMasked++;
	      }

	      buffef=0.;
	      buffer=0.;
	      
	      if(histoDT->GetBinContent(i)!=0){
		buffef = float(histoRPC->GetBinContent(i))/float(histoDT->GetBinContent(i));
		buffer = sqrt(buffef*(1.-buffef)/float(histoDT->GetBinContent(i)));
		
		efftxt<<"  "<<buffef;
		
		sumbuffef = sumbuffef + buffef;
		averageerrocc = averageerrocc + histoRPC->GetBinContent(i);
		averageerrexp = averageerrexp + histoDT->GetBinContent(i);
		  
		NumberStripsPointed++;
		
		if(maskeffect[i]==false){
		  bufdoublegapeff=bufdoublegapeff+buffef;
		  doublegaperrocc = doublegaperrocc + histoRPC->GetBinContent(i);
		  doublegaperrexp = doublegaperrexp + histoDT->GetBinContent(i);
		}
	      }else{
		//if(debug) std::cout<<" NP";
		NumberWithOutPrediction++;
		efftxt<<"  "<<0.95;
	      }
	      
	      histoPRO->SetBinContent(i,buffef);
	      histoPRO->SetBinError(i,buffer);
	    }

	    for(int i=nstrips+1;i<=95;i++) efftxt<<"  "<<0.95; efftxt<<std::endl;
	    
	    assert(NumberWithOutPrediction+NumberStripsPointed == nstrips);

	    if(debug) std::cout<<"Checking assertion"<<std::endl;

	    doublegapeff=0.;
	    doublegaperr=0.;
	    
	    if(NumberStripsPointed!=0){
	      averageeff = (sumbuffef/float(NumberStripsPointed))*100.;
	      float efftmp = averageerrocc/averageerrexp;
	      averageerr = sqrt(efftmp*(1-efftmp)/averageerrexp)*100.;
	      
	      if(debug) std::cout<<"Filling Average Efficiency"<<std::endl;

	      if(withouteffect!=0){
		float efftmp = doublegaperrocc/doublegaperrexp;
		doublegapeff =efftmp*100;
		doublegaperr =sqrt(efftmp*(1.-efftmp)/doublegaperrexp)*100.;
	      }
	    }else{
	      if(debug) std::cout<<"This Roll Doesn't have any strip Pointed"<<std::endl;
	    }

	    if(debug) std::cout<<"Filling New histograms"<<std::endl;
	    
	    if(NumberStripsPointed!=0){
	      if(!IsBadRoll(rpcId.rawId(),blacklist)){
		DoubleGapBarrel->Fill(doublegapeff);
		CentralEffBarrel->Fill(pinoeff); //distribution with fiducial region
		EffBarrel->Fill(averageeff);
	      }else{
		DoubleGapBarrel_black->Fill(doublegapeff);
		CentralEffBarrel_black->Fill(pinoeff);
		EffBarrel_black->Fill(averageeff);
	      }

	    }

	    if(fabs(BXDistribution->GetMean())<0.5 && BXDistribution->GetRMS()<1.&& BXDistribution->GetMean()!=0) BXEffBarrel->Fill(pinoeff);
	    else badBXEffBarrel->Fill(pinoeff);
	      
	    if(debug) std::cout<<name<<" Eff="<<averageeff<<" DoubleGapEff="<<doublegapeff<<" Integral Eff="<<ef<<" CentralEff Eff"<<pinoeff<<std::endl;

	    
	    int Ring = rpcId.ring();
	    
	    if(sector==1||sector==2||sector==3||sector==10||sector==11||sector==12){
	      if(Ring==-2){ EffDistroWm2->Fill(averageeff);       DoubleGapDistroWm2->Fill(doublegapeff);
	      }else if(Ring==-1){ EffDistroWm1->Fill(averageeff); DoubleGapDistroWm1->Fill(doublegapeff);
	      }else if(Ring==0) { EffDistroW0->Fill(averageeff);  DoubleGapDistroW0->Fill(doublegapeff); 
	      }else if(Ring==1) { EffDistroW1->Fill(averageeff);  DoubleGapDistroW1->Fill(doublegapeff); 
	      }else if(Ring==2) { EffDistroW2->Fill(averageeff);  DoubleGapDistroW2->Fill(doublegapeff); 
	      }
	    }else{
	      if(Ring==-2){ EffDistroWm2far->Fill(averageeff);       DoubleGapDistroWm2far->Fill(doublegapeff);
	      }else if(Ring==-1){ EffDistroWm1far->Fill(averageeff); DoubleGapDistroWm1far->Fill(doublegapeff);
	      }else if(Ring==0) { EffDistroW0far->Fill(averageeff);  DoubleGapDistroW0far->Fill(doublegapeff); 
	      }else if(Ring==1) { EffDistroW1far->Fill(averageeff);  DoubleGapDistroW1far->Fill(doublegapeff); 
	      }else if(Ring==2) { EffDistroW2far->Fill(averageeff);  DoubleGapDistroW2far->Fill(doublegapeff); 
	      }
	    }
	    
	    if(prodimages || makehtml){
	      command = "mkdir " + name;
	      system(command.c_str());
	    }

	    if(debug) std::cout<<"Writing histograms"<<std::endl;

	    histoPRO->Write();
	    histoPROY->Write();
	    histoPROX->Write();
	    histoPRO_2D->Write();
	    histoCellDistro->Write();
	    histoCLS->Write();
	    histoBXY->Write();
	    BXDistribution->Write();
	    Signal_BXDistribution->Write();
	    
	    if(debug) std::cout<<"Producing images"<<std::endl;
	    if(prodimages){//BARREL
	      histoPRO->GetXaxis()->SetTitle("Strip");
	      histoPRO->GetYaxis()->SetTitle("Efficiency (%)");
	      histoPRO->GetYaxis()->SetRangeUser(0.,1.);
	      histoPRO->Draw();
	      std::string labeltoSave = name + "/Profile.png";
	      Ca0->SaveAs(labeltoSave.c_str());
	      Ca0->Clear();

	      histoRPC->GetXaxis()->SetTitle("Strip");
	      histoRPC->GetYaxis()->SetTitle("Occupancy Extrapolation");
	      histoRPC->Draw();
	      labeltoSave = name + "/RPCOccupancy.png";
	      Ca0->SaveAs(labeltoSave.c_str());
	      Ca0->Clear();

	      histoRealRPC->GetXaxis()->SetTitle("Strip");
	      histoRealRPC->GetYaxis()->SetTitle("RPC Occupancy");
	      histoRealRPC->Draw();
	      labeltoSave = name + "/DQMOccupancy.png";
	      Ca0->SaveAs(labeltoSave.c_str());
	      Ca0->Clear();
	      
	      histoDT->GetXaxis()->SetTitle("Strip");
	      histoDT->GetYaxis()->SetTitle("Expected Occupancy");
	      histoDT->Draw();
	      labeltoSave = name + "/DTOccupancy.png";
	      Ca0->SaveAs(labeltoSave.c_str());
	      Ca0->Clear();
	      
	      BXDistribution->GetXaxis()->SetTitle("BX");
	      BXDistribution->Draw();
	      Signal_BXDistribution->SetFillColor(2);
	      Signal_BXDistribution->Draw("same");
	      labeltoSave = name + "/BXDistribution.png";
	      Ca0->SaveAs(labeltoSave.c_str());
	      Ca0->Clear();

	      histoCLS->GetXaxis()->SetTitle("Cluster Size");
	      histoCLS->Draw();
	      labeltoSave = name + "/CLS.png";
	      Ca0->SaveAs(labeltoSave.c_str());
	      Ca0->Clear();
		
	      histoBXY->GetXaxis()->SetTitle("BX");
	      histoBXY->GetYaxis()->SetTitle("Distance to the Bottom of the Chamber (cm)");
	      
	      TGraphErrors * plot = new TGraphErrors(n,x,y,ex,ey);	
	      plot->SetMarkerColor(6);
	      plot->SetMarkerStyle(20);
	      plot->SetMarkerSize(0.5);
	      plot->GetXaxis()->SetTitle("Distance to the BOTTOM of the Chamber (cm)");
	      plot->GetYaxis()->SetTitle("Mean BX (bx Units)");	
	      plot->Draw("AP");
	      labeltoSave = name + "/BXY.png";
	      Ca0->SaveAs(labeltoSave.c_str());
	      Ca0->Clear();
	    
	      histoRPC_2D->GetXaxis()->SetTitle("cm");
	      histoRPC_2D->GetYaxis()->SetTitle("cm");
	      histoRPC_2D->Draw();
	      histoRPC_2D->SetDrawOption("COLZ");
	      labeltoSave = name + "/RPCOccupancy_2D.png";
	      Ca0->SaveAs(labeltoSave.c_str());
	      Ca0->Clear();
	      
	      histoINEF->GetXaxis()->SetTitle("cm");
	      histoINEF->GetYaxis()->SetTitle("cm");
	      histoINEF->Draw();
	      histoINEF->SetDrawOption("COLZ");
	      labeltoSave = name + "/INEF.png";
	      Ca0->SaveAs(labeltoSave.c_str());
	      Ca0->Clear();
	      
	      
	      histoPROY->SetTitle("Efficiency along Y");
	      histoPROY->GetXaxis()->SetTitle("cm");
	      histoPROY->GetYaxis()->SetRangeUser(0.,1.);
	      histoPROY->Draw();
	      labeltoSave = name + "/RPCOccupancy_2D_pfy.png";
	      Ca0->SaveAs(labeltoSave.c_str());
	      Ca0->Clear();
	      
	      histoPROX->SetTitle("Efficiency along X");
	      histoPROX->GetXaxis()->SetTitle("cm");
	      histoPROX->GetYaxis()->SetRangeUser(0.,1.);
	      histoPROX->Draw();
	      labeltoSave = name + "/RPCOccupancy_2D_pfx.png";
	      Ca0->SaveAs(labeltoSave.c_str());
	      Ca0->Clear();
	      
	      histoDT_2D->GetXaxis()->SetTitle("cm");
	      histoDT_2D->GetYaxis()->SetTitle("cm");
	      histoDT_2D->Draw();
	      histoDT_2D->SetDrawOption("COLZ");
	      labeltoSave = name + "/DTOccupancy_2D.png";
	      Ca0->SaveAs(labeltoSave.c_str());
	      Ca0->Clear();
	      
	      histoPRO_2D->GetXaxis()->SetTitle("cm");
	      histoPRO_2D->GetYaxis()->SetTitle("cm");
	      histoPRO_2D->Draw();
	      histoPRO_2D->SetDrawOption("COLZ");
	      labeltoSave = name + "/Profile_2D.png";
	      Ca0->SaveAs(labeltoSave.c_str());
	      Ca0->Clear();

	      histoCellDistro->GetXaxis()->SetTitle("Efficiency(%)");
	      histoCellDistro->Draw();
	      labeltoSave = name + "/CellDistro.png";
	      Ca0->SaveAs(labeltoSave.c_str());
	      Ca0->Clear();
	      
	      if(debug) std::cout<<"histoResidual "<<std::endl;
	      histoResidual1->GetXaxis()->SetTitle("cm");
	      //histoResidual->SetFillColor(1);  histoResidual->Draw();
	      if(histoResidual1->Integral()!=0) {histoResidual1->SetFillColor(1); histoResidual1->DrawNormalized();}
	      if(histoResidual2->Integral()!=0) {histoResidual2->SetFillColor(2); histoResidual2->DrawNormalized("same");}
	      if(histoResidual3->Integral()!=0) {histoResidual3->SetFillColor(3); histoResidual3->DrawNormalized("same");}
	      //histoResidualO->SetFillColor(5); histoResidualO->Draw("same");	      
	      labeltoSave = name + "/Residual.png";
	      Ca0->SaveAs(labeltoSave.c_str());
	      Ca0->Clear();
	    }
	    
	    delete histoPRO;
	    delete histoPROY;
	    delete histoPROX;
	    delete histoPRO_2D;
	    delete histoCellDistro;
	    delete histoBXY;
	    delete histoINEF;

	    int sector = rpcId.sector();
	    //Near Side

	    if(debug) std::cout<<"Before if = "<<makehtml<<std::endl;
	    if(makehtml){
	      command = "cp htmltemplates/indexLocal.html " + name + "/index.html"; system(command.c_str());
	      if(debug) std::cout<<"html for "<<rpcId<<std::endl;
	      
	      std::string color = "#0000FF";
	      if(averageeff<threshold) color = "#ff4500";
	      
	      std::string target;
	      
	      if(sector==1||sector==2||sector==3||sector==10||sector==11||sector==12){
		if(Ring==-2) target = "Wm2near.html";
		else if(Ring==-1) target = "Wm1near.html";
		else if(Ring==0) target = "W0near.html";
		else if(Ring==1) target = "W1near.html";
		else if(Ring==2) target = "W2near.html";
	      }else{
		if(Ring==-2) target = "Wm2far.html";
		else if(Ring==-1) target = "Wm1far.html";
		else if(Ring==0)  target = "W0far.html";
		else if(Ring==1) target = "W1far.html";
		else if(Ring==2) target = "W2far.html";
	      }
	      command = "sed -e \"s|roll|" + name + "|g\" htmltemplates/indextemplate.html >> index" + target; system(command.c_str());
	      command = "sed -e \"s|roll|" + name + "|g\" -e \"s|colore|" + color + "|g\" htmltemplates/indexline.html >> insert" + target; system(command.c_str());
	    }

	    mybxhisto = 50.+BXDistribution->GetMean()*10;
	    mybxerror = BXDistribution->GetRMS()*10;

	    signal_mybxhisto = 50.+Signal_BXDistribution->GetMean()*10;
	    signal_mybxerror = Signal_BXDistribution->GetRMS()*10;
	    
	    bxbarrel->Fill(BXDistribution->GetMean(),BXDistribution->GetRMS());
	    signal_bxbarrel->Fill(Signal_BXDistribution->GetMean(),Signal_BXDistribution->GetRMS());  
	  }else{
	    if(debug) std::cout<<"One of the histograms Doesn't exist for Barrel!!!"<<std::endl;
	    exit(1);
	  }
	  	  
	  p=histoDT->Integral();
	  o=histoRPC->Integral();


	  if(p==0) rollZeroPrediction<<name<<std::endl;
	  if(o==0&&p>10) rollZeroEff<<name<<std::endl;
	  
	  if(debug) std::cout<<"Integral P="<<p<<" Observed="<<o<<std::endl;
	  
	  if(p!=0){
	    ef = float(o)/float(p); 
	    er = sqrt(ef*(1.-ef)/float(p));
	  }
	  
	  ef=ef*100;
	  er=er*100;

	  //Filling azimutal Wheel Histograms

	  int wheel = rpcId.ring();
	  int sector = rpcId.sector();
	  int region = rpcId.region();
	  
	  if(region ==0){
	    int layer = 0;
	  	    
	    if(rpcId.station()==1&&rpcId.layer()==1) layer = 1;
	    else if(rpcId.station()==1&&rpcId.layer()==2) layer = 2;
	    else if(rpcId.station()==2&&rpcId.layer()==1) layer = 3;
	    else if(rpcId.station()==2&&rpcId.layer()==2)  layer = 4;
	    else if(rpcId.station()==3) layer = 5;
	    else if(rpcId.station()==4) layer = 6;

//Cesare
	    if(!IsBadRoll(rpcId.rawId(),blacklist)){
	      if(wheel==-2){ExsectorEffWm2->Fill(sector,p); OcsectorEffWm2->Fill(sector,o); ExpLayerWm2->Fill(layer, p); ObsLayerWm2->Fill(layer, o); sectorCLSWm2[sector]->Add(histoCLS);layerCLSWm2[layer]->Add(histoCLS);}
	      else if(wheel==-1){ExsectorEffWm1->Fill(sector,p); OcsectorEffWm1->Fill(sector,o); ExpLayerWm1->Fill(layer, p); ObsLayerWm1->Fill(layer, o); sectorCLSWm1[sector]->Add(histoCLS);layerCLSWm1[layer]->Add(histoCLS);}
	      else if(wheel==0){ExsectorEffW0->Fill(sector,p); OcsectorEffW0->Fill(sector,o); ExpLayerW0->Fill(layer, p); ObsLayerW0->Fill(layer, o); sectorCLSW0[sector]->Add(histoCLS);layerCLSW0[layer]->Add(histoCLS);}
	      else if(wheel==1){ExsectorEffW1->Fill(sector,p); OcsectorEffW1->Fill(sector,o); ExpLayerW1->Fill(layer, p); ObsLayerW1->Fill(layer, o); sectorCLSW1[sector]->Add(histoCLS);layerCLSW1[layer]->Add(histoCLS);}
	      else if(wheel==2){ExsectorEffW2->Fill(sector,p); OcsectorEffW2->Fill(sector,o); ExpLayerW2->Fill(layer, p); ObsLayerW2->Fill(layer, o); sectorCLSW2[sector]->Add(histoCLS);layerCLSW2[layer]->Add(histoCLS);}
	    }else{
	      if(debug) std::cout<<"blacklist skeeping roll "<<rpcId<<std::endl;
	    }
	  }

	  delete histoCLS;
	  
	  std::string camera = name.c_str();
	  float maskedratio = (float(NumberMasked)/float(nstrips))*100.;
	  float nopredictionsratio = (float(NumberWithOutPrediction)/float(nstrips))*100.;

	  alignment<<name<<"  "<<rpcId.rawId()<<" "<<histoResidual->GetMean()<<" "<<histoResidual->GetRMS()<<std::endl;

	  if(histoResidual->GetMean()!=0. && averageeff > 10. && nopredictionsratio < 70){ 
	    MeanResiduals->Fill(histoResidual->GetMean());  
	    RMSResiduals->Fill(histoResidual->GetRMS()); 
	    MeanResiduals11->Fill(histoResidual->GetMean()); 
	    RMSResiduals11->Fill(histoResidual->GetRMS());
	  }
	  
	  if(debug) std::cout<<"Super offsets in  "<<name<<" mean="<<histoResidual->GetMean()
			     <<" AverageEff"<<averageeff
			     <<" No Prediction Ratio"<<nopredictionsratio
			     <<std::endl;
	  
	  //Pigi Histos
	  
	  if(abs((*r)->id().ring())==2){
	    //if(debug) std::cout<<rollY(rpcsrv.shortname(),rollNamesExter)<<"--"<<rpcsrv.shortname()<<std::endl;
	    if((*r)->id().ring()==2) Wheel2Summary->SetBinContent((*r)->id().sector(),rollY(rpcsrv.shortname(),rollNamesExter),pinoeff);
	    else Wheelm2Summary->SetBinContent((*r)->id().sector(),rollY(rpcsrv.shortname(),rollNamesExter),pinoeff);
	    
	  }else{
	    //if(debug) std::cout<<rollY(rpcsrv.shortname(),rollNamesInter)<<"--"<<rpcsrv.shortname()<<std::endl; 
	    if((*r)->id().ring()==-1) Wheelm1Summary->SetBinContent((*r)->id().sector(),rollY(rpcsrv.shortname(),rollNamesInter),pinoeff);
	    else if((*r)->id().ring()==0) Wheel0Summary->SetBinContent((*r)->id().sector(),rollY(rpcsrv.shortname(),rollNamesInter),pinoeff);
	    else if((*r)->id().ring()==1) Wheel1Summary->SetBinContent((*r)->id().sector(),rollY(rpcsrv.shortname(),rollNamesInter),pinoeff);
	  }
	  	  
	  //Near Side

	  int Ring = rpcId.ring();
	  if((sector==1||sector==2||sector==3||sector==10||sector==11||sector==12)){
	    if(Ring==-2){
	      indexWheel[0]++;
	      EffGlobWm2->SetBinContent(indexWheel[0],ef);
	      EffGlobWm2->SetBinError(indexWheel[0],er);  
	      EffGlobWm2->GetXaxis()->SetBinLabel(indexWheel[0],camera.c_str());

	      BXGlobWm2->SetBinContent(indexWheel[0],mybxhisto);  
	      BXGlobWm2->SetBinError(indexWheel[0],mybxerror);  
	      BXGlobWm2->GetXaxis()->SetBinLabel(indexWheel[0],camera.c_str());
	      
	      Signal_BXGlobWm2->SetBinContent(indexWheel[0],signal_mybxhisto);  
	      Signal_BXGlobWm2->SetBinError(indexWheel[0],signal_mybxerror);  
	      Signal_BXGlobWm2->GetXaxis()->SetBinLabel(indexWheel[0],camera.c_str());
	      
	      MaskedGlobWm2->SetBinContent(indexWheel[0],maskedratio);  
	      MaskedGlobWm2->GetXaxis()->SetBinLabel(indexWheel[0],camera.c_str());

	      AverageEffWm2->SetBinContent(indexWheel[0],averageeff);
	      AverageEffWm2->SetBinError(indexWheel[0],averageerr);  
	      AverageEffWm2->GetXaxis()->SetBinLabel(indexWheel[0],camera.c_str());

	      DoubleGapWm2->SetBinContent(indexWheel[0],doublegapeff);
	      DoubleGapWm2->SetBinError(indexWheel[0],doublegaperr);  
	      DoubleGapWm2->GetXaxis()->SetBinLabel(indexWheel[0],camera.c_str());
	      
	      NoPredictionWm2->SetBinContent(indexWheel[0],nopredictionsratio);
              NoPredictionWm2->GetXaxis()->SetBinLabel(indexWheel[0],camera.c_str());
	    }else if(Ring==-1){
	      indexWheel[1]++;  
	      EffGlobWm1->SetBinContent(indexWheel[1],ef);  
	      EffGlobWm1->SetBinError(indexWheel[1],er);  
	      EffGlobWm1->GetXaxis()->SetBinLabel(indexWheel[1],camera.c_str());  
	      
	      BXGlobWm1->SetBinContent(indexWheel[1],mybxhisto);  
	      BXGlobWm1->SetBinError(indexWheel[1],mybxerror);  
	      BXGlobWm1->GetXaxis()->SetBinLabel(indexWheel[1],camera.c_str());

	      Signal_BXGlobWm1->SetBinContent(indexWheel[1],signal_mybxhisto);  
	      Signal_BXGlobWm1->SetBinError(indexWheel[1],signal_mybxerror);  
	      Signal_BXGlobWm1->GetXaxis()->SetBinLabel(indexWheel[1],camera.c_str());
	      
	      MaskedGlobWm1->SetBinContent(indexWheel[1],maskedratio);  
	      MaskedGlobWm1->GetXaxis()->SetBinLabel(indexWheel[1],camera.c_str());

	      AverageEffWm1->SetBinContent(indexWheel[1],averageeff);
	      AverageEffWm1->SetBinError(indexWheel[1],averageerr);  
	      AverageEffWm1->GetXaxis()->SetBinLabel(indexWheel[1],camera.c_str());
	      
	      DoubleGapWm1->SetBinContent(indexWheel[1],doublegapeff);
	      DoubleGapWm1->SetBinError(indexWheel[1],doublegaperr);  
	      DoubleGapWm1->GetXaxis()->SetBinLabel(indexWheel[1],camera.c_str());
	      
	      NoPredictionWm1->SetBinContent(indexWheel[1],nopredictionsratio);
              NoPredictionWm1->GetXaxis()->SetBinLabel(indexWheel[1],camera.c_str());

	    }else if(Ring==0){
	      indexWheel[2]++;  
	      EffGlobW0->SetBinContent(indexWheel[2],ef);  
	      EffGlobW0->SetBinError(indexWheel[2],er);  
	      EffGlobW0->GetXaxis()->SetBinLabel(indexWheel[2],camera.c_str());  
	      
	      BXGlobW0->SetBinContent(indexWheel[2],mybxhisto);  
	      BXGlobW0->SetBinError(indexWheel[2],mybxerror);  
	      BXGlobW0->GetXaxis()->SetBinLabel(indexWheel[2],camera.c_str());

	      Signal_BXGlobW0->SetBinContent(indexWheel[2],signal_mybxhisto);  
	      Signal_BXGlobW0->SetBinError(indexWheel[2],signal_mybxerror);  
	      Signal_BXGlobW0->GetXaxis()->SetBinLabel(indexWheel[2],camera.c_str());

	      MaskedGlobW0->SetBinContent(indexWheel[2],maskedratio);  
	      MaskedGlobW0->GetXaxis()->SetBinLabel(indexWheel[2],camera.c_str());
	      
	      AverageEffW0->SetBinContent(indexWheel[2],averageeff);
	      AverageEffW0->SetBinError(indexWheel[2],averageerr);  
	      AverageEffW0->GetXaxis()->SetBinLabel(indexWheel[2],camera.c_str());

	      DoubleGapW0->SetBinContent(indexWheel[2],doublegapeff);
	      DoubleGapW0->SetBinError(indexWheel[2],doublegaperr);  
	      DoubleGapW0->GetXaxis()->SetBinLabel(indexWheel[2],camera.c_str());
	      
	      NoPredictionW0->SetBinContent(indexWheel[2],nopredictionsratio);
              NoPredictionW0->GetXaxis()->SetBinLabel(indexWheel[2],camera.c_str());	      
	    }else if(Ring==1){
	      indexWheel[3]++;  
	      EffGlobW1->SetBinContent(indexWheel[3],ef);  
	      EffGlobW1->SetBinError(indexWheel[3],er);  
	      EffGlobW1->GetXaxis()->SetBinLabel(indexWheel[3],camera.c_str());  
	      
	      BXGlobW1->SetBinContent(indexWheel[3],mybxhisto);  
	      BXGlobW1->SetBinError(indexWheel[3],mybxerror);  
	      BXGlobW1->GetXaxis()->SetBinLabel(indexWheel[3],camera.c_str());

	      Signal_BXGlobW1->SetBinContent(indexWheel[3],signal_mybxhisto);  
	      Signal_BXGlobW1->SetBinError(indexWheel[3],signal_mybxerror);  
	      Signal_BXGlobW1->GetXaxis()->SetBinLabel(indexWheel[3],camera.c_str());
	      
	      MaskedGlobW1->SetBinContent(indexWheel[3],maskedratio);  
	      MaskedGlobW1->GetXaxis()->SetBinLabel(indexWheel[3],camera.c_str());

	      AverageEffW1->SetBinContent(indexWheel[3],averageeff);
	      AverageEffW1->SetBinError(indexWheel[3],averageerr);  
	      AverageEffW1->GetXaxis()->SetBinLabel(indexWheel[3],camera.c_str());

	      DoubleGapW1->SetBinContent(indexWheel[3],doublegapeff);
	      DoubleGapW1->SetBinError(indexWheel[3],doublegaperr);  
	      DoubleGapW1->GetXaxis()->SetBinLabel(indexWheel[3],camera.c_str());
	      
	      NoPredictionW1->SetBinContent(indexWheel[3],nopredictionsratio);
              NoPredictionW1->GetXaxis()->SetBinLabel(indexWheel[3],camera.c_str());	      
	    }else if(Ring==2){
	      indexWheel[4]++;
	      EffGlobW2->SetBinContent(indexWheel[4],ef);
	      EffGlobW2->SetBinError(indexWheel[4],er);
	      EffGlobW2->GetXaxis()->SetBinLabel(indexWheel[4],camera.c_str());

	      BXGlobW2->SetBinContent(indexWheel[4],mybxhisto);  
	      BXGlobW2->SetBinError(indexWheel[4],mybxerror);  
	      BXGlobW2->GetXaxis()->SetBinLabel(indexWheel[4],camera.c_str());
	      
	      Signal_BXGlobW2->SetBinContent(indexWheel[4],signal_mybxhisto);  
	      Signal_BXGlobW2->SetBinError(indexWheel[4],signal_mybxerror);  
	      Signal_BXGlobW2->GetXaxis()->SetBinLabel(indexWheel[4],camera.c_str());
	      
	      MaskedGlobW2->SetBinContent(indexWheel[4],maskedratio);  
	      MaskedGlobW2->GetXaxis()->SetBinLabel(indexWheel[4],camera.c_str());

	      AverageEffW2->SetBinContent(indexWheel[4],averageeff);
	      AverageEffW2->SetBinError(indexWheel[4],averageerr);  
	      AverageEffW2->GetXaxis()->SetBinLabel(indexWheel[4],camera.c_str());

	      DoubleGapW2->SetBinContent(indexWheel[4],doublegapeff);
	      DoubleGapW2->SetBinError(indexWheel[4],doublegaperr);  
	      DoubleGapW2->GetXaxis()->SetBinLabel(indexWheel[4],camera.c_str());
	      
	      NoPredictionW2->SetBinContent(indexWheel[4],nopredictionsratio);
              NoPredictionW2->GetXaxis()->SetBinLabel(indexWheel[4],camera.c_str());	      
	    }
	  }else{//Far Side 
	    if(Ring==-2){
	      indexWheelf[0]++;  
	      EffGlobWm2far->SetBinContent(indexWheelf[0],ef);  
	      EffGlobWm2far->SetBinError(indexWheelf[0],er);  
	      EffGlobWm2far->GetXaxis()->SetBinLabel(indexWheelf[0],camera.c_str());

	      BXGlobWm2far->SetBinContent(indexWheelf[0],mybxhisto);  
	      BXGlobWm2far->SetBinError(indexWheelf[0],mybxerror);  
	      BXGlobWm2far->GetXaxis()->SetBinLabel(indexWheelf[0],camera.c_str());
	      
	      Signal_BXGlobWm2far->SetBinContent(indexWheelf[0],signal_mybxhisto);  
	      Signal_BXGlobWm2far->SetBinError(indexWheelf[0],signal_mybxerror);  
	      Signal_BXGlobWm2far->GetXaxis()->SetBinLabel(indexWheelf[0],camera.c_str());
	      
	      MaskedGlobWm2far->SetBinContent(indexWheelf[0],maskedratio);
	      MaskedGlobWm2far->GetXaxis()->SetBinLabel(indexWheelf[0],camera.c_str());
	      
	      AverageEffWm2far->SetBinContent(indexWheelf[0],averageeff);
              AverageEffWm2far->SetBinError(indexWheelf[0],averageerr);
              AverageEffWm2far->GetXaxis()->SetBinLabel(indexWheelf[0],camera.c_str());
	      
	      DoubleGapWm2far->SetBinContent(indexWheelf[0],doublegapeff);
	      DoubleGapWm2far->SetBinError(indexWheelf[0],doublegaperr);  
	      DoubleGapWm2far->GetXaxis()->SetBinLabel(indexWheelf[0],camera.c_str());
	      
              NoPredictionWm2far->SetBinContent(indexWheelf[0],nopredictionsratio);
              NoPredictionWm2far->GetXaxis()->SetBinLabel(indexWheelf[0],camera.c_str());
	    }else if(Ring==-1){
	      indexWheelf[1]++;  
	      EffGlobWm1far->SetBinContent(indexWheelf[1],ef);  
	      EffGlobWm1far->SetBinError(indexWheelf[1],er);  
	      EffGlobWm1far->GetXaxis()->SetBinLabel(indexWheelf[1],camera.c_str());  
	      
	      BXGlobWm1far->SetBinContent(indexWheelf[1],mybxhisto);  
	      BXGlobWm1far->SetBinError(indexWheelf[1],mybxerror);  
	      BXGlobWm1far->GetXaxis()->SetBinLabel(indexWheelf[1],camera.c_str());
	      
	      Signal_BXGlobWm1far->SetBinContent(indexWheelf[1],signal_mybxhisto);  
	      Signal_BXGlobWm1far->SetBinError(indexWheelf[1],signal_mybxerror);  
	      Signal_BXGlobWm1far->GetXaxis()->SetBinLabel(indexWheelf[1],camera.c_str());
	      
	      MaskedGlobWm1far->SetBinContent(indexWheelf[1],maskedratio);
	      MaskedGlobWm1far->GetXaxis()->SetBinLabel(indexWheelf[1],camera.c_str());

	      AverageEffWm1far->SetBinContent(indexWheelf[1],averageeff);
              AverageEffWm1far->SetBinError(indexWheelf[1],averageerr);
              AverageEffWm1far->GetXaxis()->SetBinLabel(indexWheelf[1],camera.c_str());

	      DoubleGapWm1far->SetBinContent(indexWheelf[1],doublegapeff);
	      DoubleGapWm1far->SetBinError(indexWheelf[1],doublegaperr);  
	      DoubleGapWm1far->GetXaxis()->SetBinLabel(indexWheelf[1],camera.c_str());

              NoPredictionWm1far->SetBinContent(indexWheelf[1],nopredictionsratio);
              NoPredictionWm1far->GetXaxis()->SetBinLabel(indexWheelf[1],camera.c_str());

	    }else  if(Ring==0){
	      indexWheelf[2]++;  
	      EffGlobW0far->SetBinContent(indexWheelf[2],ef);  
	      EffGlobW0far->SetBinError(indexWheelf[2],er);  
	      EffGlobW0far->GetXaxis()->SetBinLabel(indexWheelf[2],camera.c_str());  
	      
	      BXGlobW0far->SetBinContent(indexWheelf[2],mybxhisto);  
	      BXGlobW0far->SetBinError(indexWheelf[2],mybxerror);  
	      BXGlobW0far->GetXaxis()->SetBinLabel(indexWheelf[2],camera.c_str());

	      Signal_BXGlobW0far->SetBinContent(indexWheelf[2],signal_mybxhisto);  
	      Signal_BXGlobW0far->SetBinError(indexWheelf[2],signal_mybxerror);  
	      Signal_BXGlobW0far->GetXaxis()->SetBinLabel(indexWheelf[2],camera.c_str());
	      
	      MaskedGlobW0far->SetBinContent(indexWheelf[2],maskedratio);
	      MaskedGlobW0far->GetXaxis()->SetBinLabel(indexWheelf[2],camera.c_str());

	      AverageEffW0far->SetBinContent(indexWheelf[2],averageeff);
              AverageEffW0far->SetBinError(indexWheelf[2],averageerr);
              AverageEffW0far->GetXaxis()->SetBinLabel(indexWheelf[2],camera.c_str());
	      
	      DoubleGapW0far->SetBinContent(indexWheelf[2],doublegapeff);
	      DoubleGapW0far->SetBinError(indexWheelf[2],doublegaperr);  
	      DoubleGapW0far->GetXaxis()->SetBinLabel(indexWheelf[2],camera.c_str());

              NoPredictionW0far->SetBinContent(indexWheelf[2],nopredictionsratio);
              NoPredictionW0far->GetXaxis()->SetBinLabel(indexWheelf[2],camera.c_str());
	    }else if(Ring==1){
	      indexWheelf[3]++;  
	      EffGlobW1far->SetBinContent(indexWheelf[3],ef);  
	      EffGlobW1far->SetBinError(indexWheelf[3],er);  
	      EffGlobW1far->GetXaxis()->SetBinLabel(indexWheelf[3],camera.c_str());  
	      
	      BXGlobW1far->SetBinContent(indexWheelf[3],mybxhisto);  
	      BXGlobW1far->SetBinError(indexWheelf[3],mybxerror);  
	      BXGlobW1far->GetXaxis()->SetBinLabel(indexWheelf[3],camera.c_str());

	      Signal_BXGlobW1far->SetBinContent(indexWheelf[3],signal_mybxhisto);  
	      Signal_BXGlobW1far->SetBinError(indexWheelf[3],signal_mybxerror);  
	      Signal_BXGlobW1far->GetXaxis()->SetBinLabel(indexWheelf[3],camera.c_str());

	      MaskedGlobW1far->SetBinContent(indexWheelf[3],maskedratio);
	      MaskedGlobW1far->GetXaxis()->SetBinLabel(indexWheelf[3],camera.c_str());
	      
	      AverageEffW1far->SetBinContent(indexWheelf[3],averageeff);
              AverageEffW1far->SetBinError(indexWheelf[3],averageerr);
              AverageEffW1far->GetXaxis()->SetBinLabel(indexWheelf[3],camera.c_str());
	      
	      DoubleGapW1far->SetBinContent(indexWheelf[3],doublegapeff);
	      DoubleGapW1far->SetBinError(indexWheelf[3],doublegaperr);  
	      DoubleGapW1far->GetXaxis()->SetBinLabel(indexWheelf[3],camera.c_str());
	      
              NoPredictionW1far->SetBinContent(indexWheelf[3],nopredictionsratio);
              NoPredictionW1far->GetXaxis()->SetBinLabel(indexWheelf[3],camera.c_str());

	    }else if(Ring==2){
	      indexWheelf[4]++;
	      EffGlobW2far->SetBinContent(indexWheelf[4],ef);
	      EffGlobW2far->SetBinError(indexWheelf[4],er);
	      EffGlobW2far->GetXaxis()->SetBinLabel(indexWheelf[4],camera.c_str());

	      BXGlobW2far->SetBinContent(indexWheelf[4],mybxhisto);  
	      BXGlobW2far->SetBinError(indexWheelf[4],mybxerror);  
	      BXGlobW2far->GetXaxis()->SetBinLabel(indexWheelf[4],camera.c_str());

	      Signal_BXGlobW2far->SetBinContent(indexWheelf[4],signal_mybxhisto);  
	      Signal_BXGlobW2far->SetBinError(indexWheelf[4],signal_mybxerror);  
	      Signal_BXGlobW2far->GetXaxis()->SetBinLabel(indexWheelf[4],camera.c_str());
	      
	      MaskedGlobW2far->SetBinContent(indexWheelf[4],maskedratio);
	      MaskedGlobW2far->GetXaxis()->SetBinLabel(indexWheelf[4],camera.c_str());

	      AverageEffW2far->SetBinContent(indexWheelf[4],averageeff);
              AverageEffW2far->SetBinError(indexWheelf[4],averageerr);
              AverageEffW2far->GetXaxis()->SetBinLabel(indexWheelf[4],camera.c_str());
	      
	      DoubleGapW2far->SetBinContent(indexWheelf[4],doublegapeff);
	      DoubleGapW2far->SetBinError(indexWheelf[4],doublegaperr);  
	      DoubleGapW2far->GetXaxis()->SetBinLabel(indexWheelf[4],camera.c_str());
	      
              NoPredictionW2far->SetBinContent(indexWheelf[4],nopredictionsratio);
              NoPredictionW2far->GetXaxis()->SetBinLabel(indexWheelf[4],camera.c_str());
	    }
	  }
	}else if(endcap&& rpcId.region()!=0){//ENDCAPs ACA COMIENZA EL ENDCAP
	  if(debug) std::cout<<"In the EndCap"<<std::endl;
	  
	  const TrapezoidalStripTopology* top_= dynamic_cast<const TrapezoidalStripTopology*> (&((*r)->topology()));
	  float stripl = top_->stripLength();
	  float stripw = top_->pitch();

	  int Disk=rpcId.station()*rpcId.region();
	  
	  std::string detUnitLabel, meIdRPC, meIdRPC_2D, meIdCSC, meIdCSC_2D, meIdPRO, meIdPROY, meIdPROX,  signal_bxDistroId, bxDistroId, meIdRealRPC, meIdPRO_2D, meIdResidual, meIdResidual1, meIdResidual2, meIdResidual3, meIdResidualO, meIdCLS,meIdBXY,meIdINEF,meIdCEll_2D;
		  
	  RPCBookFolderStructure *  folderStr = new RPCBookFolderStructure(); //Anna
	  std::string folder = "DQMData/Muons/MuonSegEff/" +  folderStr->folderStructure(rpcId);
	  
	  if(debug) std::cout<<"folder string "<<folder<<std::endl;
	  
	  delete folderStr;
	
	  meIdRPC = folder +"/RPCDataOccupancy_"+ name;	
	  meIdCSC =folder+"/ExpectedOccupancy_"+ name;

	  bxDistroId =folder+"/BXDistribution_"+ name;
	  signal_bxDistroId =folder+"/Signal_BXDistribution_"+ name;
	  meIdRealRPC =folder+"/RealDetectedOccupancy_"+ name;
	  
	  meIdPRO = "Profile_For_"+name;
	  meIdPROY = "Y_Profile_For_"+name;
	  meIdPROX = "X_Profile_For_"+name;
	  meIdPRO_2D = "Profile2D_For_"+name;
	  if(debug) std::cout<<" meIdPRO_2D "<<meIdPRO_2D<<std::endl;
	  meIdCEll_2D = "Efficiency_Cells_Distribution_For_"+name;
	  meIdResidual = folder+"/RPCResiduals_"+ name;
	  meIdResidual1 = folder+"/RPCResiduals_Clu1_"+ name;
	  meIdResidual2 = folder+"/RPCResiduals_Clu2_"+ name;
	  meIdResidual3 = folder+"/RPCResiduals_Clu3_"+ name;
	  meIdResidualO = folder+"/RPCResiduals_Other_"+ name;
	  
	  meIdCSC_2D = folder+"/ExpectedOccupancy2D_"+ name;
	  meIdRPC_2D = folder +"/RPCDataOccupancy2D_"+ name;
	  meIdCLS = folder +"/CLSDistribution_"+ name;	
	  meIdBXY = folder +"/BXYDistribution_"+ name;
	  meIdINEF = folder +"/Inefficiency2D_"+ name;

	  histoRPC_2D= (TH2F*)theFile->Get(meIdRPC_2D.c_str());
	  histoCSC_2D= (TH2F*)theFile->Get(meIdCSC_2D.c_str());

	  for(int m=0;m<48;m++){
	    if(!(find(IntegralMuographyRawIdsVector[m].begin(),IntegralMuographyRawIdsVector[m].end(),rpcId.rawId())
		 ==IntegralMuographyRawIdsVector[m].end())){

	      if(!IntegralMuography[m]){
		assert(!IntegralMuographyExp[m]);
		assert(!IntegralMuographyObs[m]);
		
		std::cout<<"doing first booking for IntegralMuography[m="<<m<<"] with "<< name<<std::endl;
		
		IntegralMuography[m] = new TH2F(namesIntegralMuography[m].c_str(),namesIntegralMuography[m].c_str()
						,histoRPC_2D->GetNbinsX(),histoRPC_2D->GetXaxis()->GetXmin(),histoRPC_2D->GetXaxis()->GetXmax()
						,histoRPC_2D->GetNbinsY(),histoRPC_2D->GetYaxis()->GetXmin(),histoRPC_2D->GetYaxis()->GetXmax());  
		
		IntegralMuographyObs[m] = new TH2F((namesIntegralMuography[m]+"Obs").c_str(),(namesIntegralMuography[m]+"Obs").c_str()
						,histoRPC_2D->GetNbinsX(),histoRPC_2D->GetXaxis()->GetXmin(),histoRPC_2D->GetXaxis()->GetXmax()
						,histoRPC_2D->GetNbinsY(),histoRPC_2D->GetYaxis()->GetXmin(),histoRPC_2D->GetYaxis()->GetXmax());  

		IntegralMuographyExp[m] = new TH2F((namesIntegralMuography[m]+"Exp").c_str(),(namesIntegralMuography[m]+"Exp").c_str()
						,histoRPC_2D->GetNbinsX(),histoRPC_2D->GetXaxis()->GetXmin(),histoRPC_2D->GetXaxis()->GetXmax()
						,histoRPC_2D->GetNbinsY(),histoRPC_2D->GetYaxis()->GetXmin(),histoRPC_2D->GetYaxis()->GetXmax());  
		
		IntegralMuography[m]->Add(histoRPC_2D);
		IntegralMuographyExp[m]->Add(histoCSC_2D);
	      }else{
		IntegralMuographyObs[m]->Add(histoRPC_2D);
		IntegralMuographyExp[m]->Add(histoCSC_2D);
	      }
	    }
	  }
	  
	  histoResidual1= (TH1F*)theFile->Get(meIdResidual1.c_str());
	  histoResidual2= (TH1F*)theFile->Get(meIdResidual2.c_str());
	  histoResidual3= (TH1F*)theFile->Get(meIdResidual3.c_str());
	  histoResidualO= (TH1F*)theFile->Get(meIdResidualO.c_str());

	  if(!histoResidual1 || !histoResidual2 || !histoResidual3 || !histoResidualO) std::cout<<"Warning one of the histo residuals plot doesn't exist"<<std::endl;

	  histoResidual= new TH1F (meIdResidual.c_str(),meIdResidual.c_str(),101,-20,20);
	  histoResidual->Add(histoResidual1);
	  histoResidual->Add(histoResidual2);
	  histoResidual->Add(histoResidual3);
	  histoResidual->Add(histoResidualO);

	  AllResidualsEndCap->Add(histoResidual);
 
	  histoINEF = (TH2F*)theFile->Get(meIdINEF.c_str());
	  
	  const int n = 20;
	  
	  float x[n];
	  float y[n];
	  float ex[n];
	  float ey[n];

	  histoCLS= (TH1F*)theFile->Get(meIdCLS.c_str());
	  histoBXY= (TH2F*)theFile->Get(meIdBXY.c_str());

	  
	  if(rpcId.region()!=0){//endcap
	    if(debug) std::cout<<"Filling CLS EndCaps "<<histoCLS->Integral()<<std::endl;
	    if(rpcId.ring()==2){
	      if(abs(rpcId.station())==1){
		if(rpcId.roll()==1) CLSStation1Ring2_A->Add(histoCLS);
		if(rpcId.roll()==2) CLSStation1Ring2_B->Add(histoCLS);
		if(rpcId.roll()==3) CLSStation1Ring2_C->Add(histoCLS);
	      }
	      if(abs(rpcId.station())==2 || abs(rpcId.station())==3){
		if(rpcId.roll()==1)CLSStation23Ring2_A->Add(histoCLS);
		if(rpcId.roll()==2)CLSStation23Ring2_B->Add(histoCLS);
		if(rpcId.roll()==3)CLSStation23Ring2_C->Add(histoCLS);
	      }
	    }
	    if(rpcId.ring()==3){
	      if(rpcId.roll()==1)CLSStation123Ring3_A->Add(histoCLS);
	      if(rpcId.roll()==2)CLSStation123Ring3_B->Add(histoCLS);
	      if(rpcId.roll()==3)CLSStation123Ring3_C->Add(histoCLS);
	    }
	  }
	  
	  
	  if(histoBXY && histoCLS){
	    if(debug) std::cout<<"the histograms exist"<<std::endl;
	      
	    float step = stripl/float(n);
	    for(int i=0;i<n;i++){
	      float mean = histoBXY->ProjectionX("_px",i,i+1)->GetMean();
	      float entries = histoBXY->ProjectionX("_px",i,i+1)->GetEntries();
	      float error = histoBXY->ProjectionX("_px",i,i+1)->GetRMS()/ sqrt(entries);
	      
	      x[i]=(i+1)*step;
	      ex[i]=step*0.5;
	      y[i]=mean;
	      ey[i]=error;
	    }
	    
	  }else{
	    if(debug) std::cout<<"WARNING!!! one of the two histograms (histoBXY or histoCLS) doesn't exist"<<std::endl;
	    if(debug) std::cout<<meIdBXY<<" "<<meIdCLS<<std::endl;
	  }
	  	  
	  histoRPC= (TH1F*)theFile->Get(meIdRPC.c_str()); if(!histoRPC) if(debug) std::cout<<meIdRPC<<"Doesn't exist"<<std::endl;
	  histoCSC= (TH1F*)theFile->Get(meIdCSC.c_str());if(!histoCSC)if(debug) std::cout<<meIdCSC<<"Doesn't exist"<<std::endl;
	  BXDistribution = (TH1F*)theFile->Get(bxDistroId.c_str());if(!BXDistribution)if(debug) std::cout<<BXDistribution<<"Doesn't exist"<<std::endl;
	  Signal_BXDistribution = (TH1F*)theFile->Get(signal_bxDistroId.c_str());
	  histoRealRPC = (TH1F*)theFile->Get(meIdRealRPC.c_str());if(!histoRealRPC)if(debug) std::cout<<meIdRealRPC<<"Doesn't exist"<<std::endl;
	  
	  histoPRO= new TH1F (meIdPRO.c_str(),meIdPRO.c_str(),nstrips,0.5,nstrips+0.5);
	  
	  if(debug) std::cout<<"meIdPRO_2D.c_str() = "<<meIdPRO_2D.c_str()<<std::endl;

	  histoPRO_2D= new TH2F (meIdPRO_2D.c_str(),meIdPRO_2D.c_str(),
				 2*(int)(0.6*nstrips*stripw),-0.6*nstrips*stripw,0.6*nstrips*stripw,
				 2*(int)(0.6*stripl),-0.6*stripl,0.6*stripl);

	  histoCellDistro = new TH1F (meIdCEll_2D.c_str(),meIdCEll_2D.c_str(),101,-0.5,100.5);
	  
	  histoPROY = new TH1F (meIdPROY.c_str(),meIdPROY.c_str(),2*(int)(0.6*stripl),-0.6*stripl,0.6*stripl);
	  histoPROX = new TH1F (meIdPROX.c_str(),meIdPROX.c_str(),2*(int)(0.6*nstrips*stripw),-0.6*stripw*nstrips,0.6*stripw*nstrips);


	  //if(debug) std::cout <<folder<<"/"<<name<<std::endl;

	  //Cesare
          float MeanCLSRollNOBL = 0;
	  
	  int NumberMasked=0;
	  int NumberWithOutPrediction=0;
	  double p = 0;
	  double o = 0;
	  float mybxhisto = 0.;
	  float mybxerror = 0.;
	  float signal_mybxhisto = 0.;
	  float signal_mybxerror = 0.;
	  float ef =0.;
	  float er =0.;
	  float efX =0;
	  float erX =0;
	  float efY =0;
	  float erY =0;
	  float ef2D =0.;
	  float er2D =0.;
	  float buffef = 0.;
	  float buffer = 0.;
	  float sumbuffef = 0.;
	  float averageeff = 0.;
	  float averageerr = 0.;

	  float averageerrexp = 0.;	  
	  float averageerrocc = 0.;

	  float doublegapeff = 0;
	  float doublegaperr = 0;

	  float bufdoublegapeff = 0;
	  	  
	  float doublegaperrocc = 0;
	  float doublegaperrexp = 0;
	 
 	  float pinoeff = 0.;
	  float pinoerr = 0.;
	  
	  float pinoobserved =0;
	  float pinoexpected =0;

	  int NumberStripsPointed = 0;
	  
	  if(debug) std::cout<<"Checking 2D Histograms for "<<name<<std::endl;

	  if(histoRPC_2D && histoCSC_2D && histoResidual && endcap){

	    //Residuals paper
	    if(rpcId.region()!=0){
	      if(abs(rpcId.station())==1){
		if(rpcId.ring()==2){
		  if(rpcId.roll()==1) residualStation1Ring2_A->Add(histoResidual);
		  if(rpcId.roll()==2) residualStation1Ring2_B->Add(histoResidual);
		  if(rpcId.roll()==3) residualStation1Ring2_C->Add(histoResidual);
		}
		if(rpcId.ring()==3){
		  if(rpcId.roll()==1) residualStation1Ring3_A->Add(histoResidual);
		  if(rpcId.roll()==2) residualStation1Ring3_B->Add(histoResidual);
		  if(rpcId.roll()==3) residualStation1Ring3_C->Add(histoResidual);
		}
	      }
	      if(abs(rpcId.station())==2 || abs(rpcId.station())==3){
		if(rpcId.ring()==2){
		  if(rpcId.roll()==1) residualStation23Ring2_A->Add(histoResidual);
		  if(rpcId.roll()==2) residualStation23Ring2_B->Add(histoResidual);
		  if(rpcId.roll()==3) residualStation23Ring2_C->Add(histoResidual);
		}
		if(rpcId.ring()==3){
		  if(rpcId.roll()==1) residualStation23Ring3_A->Add(histoResidual);
		  if(rpcId.roll()==2) residualStation23Ring3_B->Add(histoResidual);
		  if(rpcId.roll()==3) residualStation23Ring3_C->Add(histoResidual);
		}
	      }
	    }
	    
	    if(rpcId.region()==1){
	      if(rpcId.station()==1 && rpcId.ring()==2) residualDisk1Ring2->Add(histoResidual);
	      if(rpcId.station()==1 && rpcId.ring()==3) residualDisk1Ring3->Add(histoResidual);
	      if(rpcId.station()==2 && rpcId.ring()==2) residualDisk2Ring2->Add(histoResidual);
	      if(rpcId.station()==2 && rpcId.ring()==3) residualDisk2Ring3->Add(histoResidual);
	      if(rpcId.station()==3 && rpcId.ring()==2) residualDisk3Ring2->Add(histoResidual);
	      if(rpcId.station()==3 && rpcId.ring()==3) residualDisk3Ring3->Add(histoResidual);
	    }
	    if(rpcId.region()==-1){
	      if(rpcId.station()==1 && rpcId.ring()==2) residualDiskm1Ring2->Add(histoResidual);
	      if(rpcId.station()==1 && rpcId.ring()==3) residualDiskm1Ring3->Add(histoResidual);
	      if(rpcId.station()==2 && rpcId.ring()==2) residualDiskm2Ring2->Add(histoResidual);
	      if(rpcId.station()==2 && rpcId.ring()==3) residualDiskm2Ring3->Add(histoResidual);
	      if(rpcId.station()==3 && rpcId.ring()==2) residualDiskm3Ring2->Add(histoResidual);
	      if(rpcId.station()==3 && rpcId.ring()==3) residualDiskm3Ring3->Add(histoResidual);
	    }


	    for(int i=1;i<=2*(int)(0.6*nstrips*stripw);++i){
	      for(int j=1;j<=2*(int)(0.6*stripl);++j){
		ef2D=0.;
		er2D=0.; //ojo con esto en DQMOffline podria ser ef2D en lugar de er2D
		if(histoCSC_2D->GetBinContent(i,j) != 0){
		  ef2D = histoRPC_2D->GetBinContent(i,j)/histoCSC_2D->GetBinContent(i,j);
		  er2D = sqrt(ef2D*(1-ef2D)/histoCSC_2D->GetBinContent(i,j));
		  histoCellDistro->Fill(ef2D*100.);
		  histoPRO_2D->SetBinContent(i,j,ef2D*100.);
		  histoPRO_2D->SetBinError(i,j,er2D*100.);
		}
	      }//loop on the boxes
	    }

	    if(debug) std::cout<<"Getting Profiles"<<name<<std::endl;
	    
	    TH1D * profileRPC_Y =  histoRPC_2D->ProjectionY(" ",int(0.6*nstrips*stripw)-int(nstrips*stripw*0.5),int(0.6*nstrips*stripw)+int(nstrips*stripw*0.5));
	    TH1D * profileCSC_Y =  histoCSC_2D->ProjectionY(" ",int(0.6*nstrips*stripw)-int(nstrips*stripw*0.5),int(0.6*nstrips*stripw)+int(nstrips*stripw*0.5));
	    
	    for(int j=1;j<=2*(int)(0.6*stripl);++j){
	      efY=0.;
	      erY=0.;
	      if(profileCSC_Y->GetBinContent(j) != 0){
		efY = profileRPC_Y->GetBinContent(j)/profileCSC_Y->GetBinContent(j);
		erY = sqrt(efY*(1-efY)/profileCSC_Y->GetBinContent(j));
	      }	
	      //if(debug) std::cout<<"Filling Y profile Y="<<j<<" efY="<<efY*100.<<" erY="<<erY*100.<<std::endl;
	      histoPROY->SetBinContent(j,efY);
	      histoPROY->SetBinError(j,erY);
	    }
	    
	    TH1D * profileRPC_X =  histoRPC_2D->ProjectionX(" ",int(0.6*stripl)-int(stripl*0.5),int(0.6*stripl)+int(stripl*0.5));
	    TH1D * profileCSC_X =  histoCSC_2D->ProjectionX(" ",int(0.6*stripl)-int(stripl*0.5),int(0.6*stripl)+int(stripl*0.5));


	    for(int i=1;i<=2*(int)(0.6*nstrips*stripw);++i){
	      efX=0.;
	      erX=0.;
	      if(profileCSC_X->GetBinContent(i) != 0){
		efX = profileRPC_X->GetBinContent(i)/profileCSC_X->GetBinContent(i);
		erX = sqrt(efX*(1-efX)/profileCSC_X->GetBinContent(i));
	      }	
	      //if(debug) std::cout<<"Filling X profile X="<<i<<" efX="<<efX*100.<<" erX="<<erX*100.<<std::endl;
	      histoPROX->SetBinContent(i,efX);
	      histoPROX->SetBinError(i,erX);
	    }

	    int firstybin = int(0.1*stripl+fiducialcuty);
	    int lastybin = int(1.1*stripl-fiducialcuty);

	    float averageRollWidth = nstrips*stripw; //This is the roll with.
	    const BoundPlane & RPCSurface = (*r)->surface();
	    GlobalPoint CenterPointInGlobal = RPCSurface.toGlobal(top_->localPosition(0));
	    float radiusCenterRoll=top_->radius();
	    
	    if(debug) std::cout<<" Trapezoidal integral for "<<name<<std::endl;
	    if(debug) std::cout<<" Radius "<<radiusCenterRoll<<std::endl;
	    if(debug) std::cout<<" firstybin "<<firstybin<<" lastybin "<<lastybin<<std::endl;
	    
	    for(int j=firstybin;j<=lastybin;j++){
	      float y = j-stripl*0.6;
	      float localRollWidth = averageRollWidth*(1+y/radiusCenterRoll);
	      int firstxbin = int (0.6*averageRollWidth-localRollWidth*0.5+fiducialcutx);
	      int lastxbin  = int (0.6*averageRollWidth+localRollWidth*0.5-fiducialcutx);
	      if(debug) std::cout<<" firstxbin "<<firstxbin<<" lastxbin "<<lastxbin<<" localRollWidth "<<localRollWidth
				 <<" j "<<j<<" y "<<y<<" fiducial cut x "<<fiducialcutx<<std::endl;
	      for(int i=firstxbin;i<=lastxbin;i++){
		pinoobserved = pinoobserved + histoRPC_2D->GetBinContent(i,j);	
		pinoexpected = pinoexpected + histoCSC_2D->GetBinContent(i,j);
		float eff = -1;
		if(histoCSC_2D->GetBinContent(i,j)!= 0){
		  eff = (histoRPC_2D->GetBinContent(i,j)/histoCSC_2D->GetBinContent(i,j))*100.;
		}
	      }
	    }

//////////////////////////////////////////////////////////////////////////////////////////////////////////
	    //Cesare
            CSCDen->Fill(pinoexpected);

            float pinoeffNOBL = (pinoobserved/pinoexpected)*100.;
            MeanCLSRollNOBL = histoCLS->GetMean();
            RollEeff<<rpcId.rawId()<<" "<<name<<" "<<pinoeffNOBL<<" "<<pinoobserved<<" "<<pinoexpected<<" "<<MeanCLSRollNOBL<<std::endl;

            EffEndcapStrip<<rpcId.rawId()<<" "<<name<<" "; 
            for(int i=1;i<=nstrips;i++){
		 double occStrip = histoRPC->GetBinContent(i);
		 double expStrip = histoCSC->GetBinContent(i);
		 double effStrip = 0;
		 if(expStrip != 0) effStrip = 100*occStrip/expStrip;
            	 EffEndcapStrip<<" "<<occStrip<<" "<<expStrip<<" "<<effStrip;
             }
            EffEndcapStrip<<std::endl;

//////////////////////////////////////////////////////////////////////////////////////////////////////////
	    
	    if(pinoexpected != 0){
	      pinoeff = (pinoobserved/pinoexpected)*100.;
	      pinoerr = 100.*sqrt((pinoobserved/pinoexpected)*(1.-(pinoobserved/pinoexpected))/pinoexpected);
	    }	

	    if(debug) std::cout<<"Filling txt file with roll and efficiency"<<std::endl;
	    RollYEff<<name<<" "<<pinoeff<<" "<<pinoerr<<" "<<pinoexpected<<" "<<histoCLS->GetMean()<<" ";
	    database<<name<<" "<<pinoeff<<" "<<pinoerr<<" "<<pinoexpected<<" "<<histoCLS->GetMean()<<" ";

	    int nBins = histoCLS->GetSize();
	    for(int i=1; i<(nBins-1); i++){
	      double numCLS = histoCLS->Integral(i,(nBins-1));
	      double denCLS = histoCLS->Integral(1,(nBins-1));
	      double probCLS = 0;
	      if(denCLS != 0) probCLS = numCLS/denCLS;
	      else probCLS = -1;
	      //std::cout<<histoCLS->Integral(i,(nBins-1))<<" "<<probCLS<<std::endl;
	      RollYEff<<probCLS<<" ";
	    }
	    
	    RollYEff<<std::endl;
	    
	    float integralCLS = histoCLS->Integral()+histoCLS->GetBinContent(11);
	    for(int bin=1;bin<=11;bin++){
	      if(integralCLS!=0){
		double normCLS = histoCLS->GetBinContent(bin)/integralCLS;
		database<<normCLS<<" ";
	      }
	      else{
		database<<"-1 ";
	      }
	    }
	    
	    for(int strip = 1;strip<histoRealRPC->GetSize();strip++){
 	      database<<histoRealRPC->GetBinContent(strip)<<" ";
 	    }
 	    for(int extra = histoRealRPC->GetSize()-1;extra<90;extra++){
 	      database<<"-1 ";
	    }
	    
	    database<<std::endl;

	    if(debug) std::cout<<" deleting profiles"<<std::endl;

	    delete profileCSC_X;
	    delete profileCSC_Y;
	    delete profileRPC_X;
	    delete profileRPC_Y;
	  
	  }else{
	    if(debug) std::cout<<"Warning!!! Alguno de los  histogramas 2D no fue leido!"<<std::endl;
	  }

	  bool maskeffect[100];
	  for(int i=0;i<100;i++) maskeffect[i]=false;

	  if(histoRPC && histoCSC && BXDistribution && histoRealRPC && Signal_BXDistribution){
	    for(int i=1;i<=nstrips;++i){
	      if(histoRealRPC->GetBinContent(i)==0 || histoCSC->GetBinContent(i)==0){
		if(debug) std::cout<<"1";
		if(i==1){
		  maskeffect[1]=true;
		  maskeffect[2]=true;
		  maskeffect[3]=true;
		}else if(i==2){
		  maskeffect[1]=true;
		  maskeffect[2]=true;
		  maskeffect[3]=true;
		  maskeffect[4]=true;
		}else if(i==nstrips){
		  maskeffect[nstrips-2]=true;
		  maskeffect[nstrips-1]=true;
		  maskeffect[nstrips]=true;
		}else if(i==nstrips-1){
		  maskeffect[nstrips-3]=true;
		  maskeffect[nstrips-2]=true;
		  maskeffect[nstrips-1]=true;
		  maskeffect[nstrips]=true;
		}else{
		  maskeffect[i-2]=true;
		  maskeffect[i-1]=true;
		  maskeffect[i]=true;
		  maskeffect[i+1]=true;
		  maskeffect[i+2]=true;
		}
	      }else{
		if(debug) std::cout<<"0";
	      }
	    }
	    
	    //Border Effect
	    //maskeffect[nstrips]=true;
	    //maskeffect[nstrips-1]=true;
	    //maskeffect[nstrips-2]=true;
	    //maskeffect[nstrips-3]=true;
	    //maskeffect[nstrips-4]=true;
	    //maskeffect[nstrips-5]=true;
	    
	    if(debug) std::cout<<std::endl;
	    
	    float withouteffect=0.;
	    
	    for(int i=1;i<=nstrips;i++){
	      if(maskeffect[i]==false){
		withouteffect++;
		if(debug) std::cout<<"0";
	      }else{
		if(debug) std::cout<<"1";
	      }
	    }
	    
	    if(debug) std::cout<<std::endl;
	    
	    efftxt<<name<<"  "<<rpcId.rawId();

	    if(debug) std::cout<<"Before the loop for the strips profiles"<<std::endl;
	    
	    for(int i=1;i<=int((*r)->nstrips());++i){
	      if(histoRealRPC->GetBinContent(i)==0){
		NumberMasked++;
	      }
	      
	      buffef=0.;
	      buffer=0.;

	      if(histoCSC->GetBinContent(i)!=0){
		buffef = float(histoRPC->GetBinContent(i))/double(histoCSC->GetBinContent(i));
		buffer = sqrt(buffef*(1.-buffef)/double(histoCSC->GetBinContent(i)));
		
		efftxt<<"  "<<buffef;
		
		sumbuffef=sumbuffef+buffef;
		
		averageerrocc = averageerrocc + histoRPC->GetBinContent(i);
		averageerrexp = averageerrexp + histoCSC->GetBinContent(i);
		
		NumberStripsPointed++;

		if(maskeffect[i]==false){
		  bufdoublegapeff=bufdoublegapeff+buffef;
		  doublegaperrocc = doublegaperrocc + histoRPC->GetBinContent(i);
		  doublegaperrexp = doublegaperrexp + histoCSC->GetBinContent(i);
		}
	      }else{
		if(debug) std::cout<<" NP";
		NumberWithOutPrediction++;
		efftxt<<"  "<<0.95;
	      }

	      histoPRO->SetBinContent(i,buffef);
	      histoPRO->SetBinError(i,buffer);
	    }

	    for(int i=nstrips+1;i<=95;i++) efftxt<<"  "<<0.95; efftxt<<std::endl;
	    
	    assert(NumberWithOutPrediction+NumberStripsPointed == nstrips);

	    if(debug) std::cout<<"Checking assertion"<<std::endl;
	    
	    doublegapeff=0.;
	    doublegaperr=0.;
	    	    
	    if(NumberStripsPointed!=0){
	      averageeff = (sumbuffef/float(NumberStripsPointed))*100.;
	      float efftmp = averageerrocc/averageerrexp;
	      averageerr = sqrt(efftmp*(1-efftmp)/averageerrexp)*100.;
	      
	      if(debug) std::cout<<"Filling Average Efficiency"<<std::endl;
     
	      if(withouteffect!=0){
		float efftmp = doublegaperrocc/doublegaperrexp;
		doublegapeff=efftmp*100;
		doublegaperr = sqrt(efftmp*(1-efftmp)/doublegaperrexp)*100.;
	      }
	    }else{
	      if(debug) std::cout<<"This Roll Doesn't have any strip Pointed"<<std::endl;
	    }
	     
	    if(debug) std::cout<<"Filling txt file with roll and efficiency"<<std::endl;
	    
	    if(debug) std::cout<<"Filling New histograms"<<std::endl;
	    
	     if(NumberStripsPointed!=0){
	       if(!IsBadRoll(rpcId.rawId(),blacklist)){
		 DoubleGapEndCap->Fill(doublegapeff);
		 CentralEffEndCap->Fill(pinoeff);
		 EffEndCap->Fill(averageeff);
	       }else{
		 DoubleGapEndCap_black->Fill(doublegapeff);
		 CentralEffEndCap_black->Fill(pinoeff);
		 EffEndCap_black->Fill(averageeff);
	       }
	     }
	    if(fabs(BXDistribution->GetMean())<0.5 && BXDistribution->GetRMS()<1.&& BXDistribution->GetMean()!=0){
	      BXEffEndCap->Fill(pinoeff);
	      if(rpcId.region()==1) BXEffPositiveEndCap->Fill(pinoeff);
	      else if(rpcId.region() ==-1) BXEffNegativeEndCap->Fill(pinoeff);
	    }
	    else{
	      badBXEffEndCap->Fill(pinoeff);
	      if(rpcId.region()==1) badBXEffPositiveEndCap->Fill(pinoeff);
	      else if(rpcId.region() ==-1) badBXEffNegativeEndCap->Fill(pinoeff);
	    }

	    if(debug) std::cout<<name<<" Eff="<<averageeff<<" DoubleGapEff="<<doublegapeff<<" Integral Eff="<<ef<<" CentralEff Eff"<<pinoeff<<std::endl;

	    if(debug) std::cout<<"Filled New histograms"<<std::endl;
	  
	    if(sector==1||sector==2||sector==6){
	      if(Disk==-3){EffDistroDm3->Fill(pinoeff);
	      }else if(Disk==-2){EffDistroDm2->Fill(pinoeff);
	      }else if(Disk==-1){EffDistroDm1->Fill(pinoeff);
	      }else if(Disk==1){EffDistroD1->Fill(pinoeff);
	      }else if(Disk==2){EffDistroD2->Fill(pinoeff);
	      }else if(Disk==3){EffDistroD3->Fill(pinoeff);
	      }
	    }else{//Far Side 
	      if(Disk==-3){EffDistroDm3far->Fill(pinoeff);
	      }else if(Disk==-2){EffDistroDm2far->Fill(pinoeff);
	      }else if(Disk==-1){EffDistroDm1far->Fill(pinoeff);
	      }else if(Disk==1){EffDistroD1far->Fill(pinoeff);
	      }else if(Disk==2){EffDistroD2far->Fill(pinoeff);
	      }else if(Disk==3){EffDistroD3far->Fill(pinoeff);
	      }
	    }
	    
	    if(prodimages || makehtml){	       
	      command = "mkdir " + name;
	      system(command.c_str());
	    }

	    if(debug) std::cout<<"Writing histograms"<<std::endl;

	    histoPRO->Write();
	    histoPROY->Write(); //parece este Cuando se descomentan estos histogramas aparece el problema en la visualizacion con TBRowser 
	    histoPRO_2D->Write(); //ahora este
	    histoCLS->Write(); //parece este
	    histoCellDistro->Write();
	    histoBXY->Write();
	    BXDistribution->Write();
	    Signal_BXDistribution->Write();

	    if(debug) std::cout<<"Producing images"<<std::endl;
	    if(prodimages){//ENDCAP	
	      if(debug) std::cout<<"Creating Images for the endcap"<<std::endl;
	      histoPRO->GetXaxis()->SetTitle("Strip");
	      histoPRO->GetYaxis()->SetTitle("Efficiency (%)");
	      histoPRO->GetYaxis()->SetRangeUser(0.,1.);
	      histoPRO->Draw();
	      std::string labeltoSave = name + "/Profile.png";
	      Ca0->SaveAs(labeltoSave.c_str());
	      Ca0->Clear();
	      
	      histoRPC->GetXaxis()->SetTitle("Strip");
	      histoRPC->GetYaxis()->SetTitle("Occupancy Extrapolation");
	      histoRPC->Draw();
	      labeltoSave = name + "/RPCOccupancy.png";
	      Ca0->SaveAs(labeltoSave.c_str());
	      Ca0->Clear();
	      
	      histoRealRPC->GetXaxis()->SetTitle("Strip");
	      histoRealRPC->GetYaxis()->SetTitle("RPC Occupancy");
	      histoRealRPC->Draw();
	      labeltoSave = name + "/DQMOccupancy.png";
	      Ca0->SaveAs(labeltoSave.c_str());
	      Ca0->Clear();
	      
	      histoCSC->GetXaxis()->SetTitle("Strip");
	      histoCSC->GetYaxis()->SetTitle("Expected Occupancy");
	      histoCSC->Draw();
	      labeltoSave = name + "/DTOccupancy.png";
	      Ca0->SaveAs(labeltoSave.c_str());
	      Ca0->Clear();
	      
	      BXDistribution->GetXaxis()->SetTitle("BX");
	      BXDistribution->Draw();
	      Signal_BXDistribution->SetFillColor(2);
	      Signal_BXDistribution->Draw("same");
	      labeltoSave = name + "/BXDistribution.png";
	      Ca0->SaveAs(labeltoSave.c_str());
	      Ca0->Clear();
		
	      histoCLS->GetXaxis()->SetTitle("Cluster Size");
	      histoCLS->Draw();
	      labeltoSave = name + "/CLS.png";
	      Ca0->SaveAs(labeltoSave.c_str());
	      Ca0->Clear();
	      
	      histoBXY->GetXaxis()->SetTitle("BX");
	      histoBXY->GetYaxis()->SetTitle("Distance to the Bottom of the Chamber (cm)");
	      
	      TGraphErrors * plot = new TGraphErrors(n,x,y,ex,ey);	
	      plot->SetMarkerColor(6);
	      plot->SetMarkerStyle(20);
	      plot->SetMarkerSize(0.5);
	      plot->GetXaxis()->SetTitle("Distance to the BOTTOM of the Chamber (cm)");
	      plot->GetYaxis()->SetTitle("Mean BX (bx Units)");	
	      plot->Draw("AP");
	      labeltoSave = name + "/BXY.png";
	      Ca0->SaveAs(labeltoSave.c_str());
	      Ca0->Clear();
	    	      
	      histoRPC_2D->GetXaxis()->SetTitle("cm");
	      histoRPC_2D->GetYaxis()->SetTitle("cm");
	      histoRPC_2D->Draw();
	      histoRPC_2D->SetDrawOption("COLZ");
	      labeltoSave = name + "/RPCOccupancy_2D.png";
	      Ca0->SaveAs(labeltoSave.c_str());
	      Ca0->Clear();
	      
	      histoINEF->GetXaxis()->SetTitle("cm");
	      histoINEF->GetYaxis()->SetTitle("cm");
	      histoINEF->Draw();
	      histoINEF->SetDrawOption("COLZ");
	      labeltoSave = name + "/INEF.png";
	      Ca0->SaveAs(labeltoSave.c_str());
	      Ca0->Clear();
	      
	      histoPROY->SetTitle("Efficiency along Y");
	      histoPROY->GetXaxis()->SetTitle("cm");
	      histoPROY->GetYaxis()->SetRangeUser(0.,1.);
	      histoPROY->Draw();
	      labeltoSave = name + "/RPCOccupancy_2D_pfy.png";
	      Ca0->SaveAs(labeltoSave.c_str());
	      Ca0->Clear();
	      
	      histoPROX->SetTitle("Efficiency along X");
	      histoPROX->GetXaxis()->SetTitle("cm");
	      histoPROX->GetYaxis()->SetRangeUser(0.,1.);
	      histoPROX->Draw();
	      labeltoSave = name + "/RPCOccupancy_2D_pfx.png";
	      Ca0->SaveAs(labeltoSave.c_str());
	      Ca0->Clear();
	      
	      if(debug) std::cout<<"histoCSC_2D "<<std::endl;
	      histoCSC_2D->GetXaxis()->SetTitle("cm");
	      histoCSC_2D->GetYaxis()->SetTitle("cm");
	      histoCSC_2D->Draw();
	      histoCSC_2D->SetDrawOption("COLZ");
	      labeltoSave = name + "/DTOccupancy_2D.png";
	      Ca0->SaveAs(labeltoSave.c_str());
	      Ca0->Clear();
	      
	      if(debug) std::cout<<"histoPRO_2D "<<std::endl;
	      histoPRO_2D->GetXaxis()->SetTitle("cm");
	      histoPRO_2D->GetYaxis()->SetTitle("cm");
	      histoPRO_2D->Draw();
	      histoPRO_2D->SetDrawOption("COLZ");
	      labeltoSave = name + "/Profile_2D.png";
	      Ca0->SaveAs(labeltoSave.c_str());
	      Ca0->Clear();

	      histoCellDistro->GetXaxis()->SetTitle("Efficiency(%)");
	      histoCellDistro->Draw();
	      labeltoSave = name + "/CellDistro.png";
	      Ca0->SaveAs(labeltoSave.c_str());
	      Ca0->Clear();
	      
	      if(debug) std::cout<<"histoResidual "<<std::endl;
	      histoResidual1->GetXaxis()->SetTitle("cm");
	      //histoResidual->SetFillColor(1);  histoResidual->Draw();
	      if(histoResidual1->Integral()!=0) {histoResidual1->SetFillColor(1); histoResidual1->DrawNormalized();}
	      if(histoResidual2->Integral()!=0) {histoResidual2->SetFillColor(2); histoResidual2->DrawNormalized("same");}
	      if(histoResidual3->Integral()!=0) {histoResidual3->SetFillColor(3); histoResidual3->DrawNormalized("same");}
	      //histoResidualO->SetFillColor(5); histoResidualO->Draw("same");	      
	      labeltoSave = name + "/Residual.png";
	      Ca0->SaveAs(labeltoSave.c_str());
	      Ca0->Clear();
	    }
	    
	    delete histoPRO;
	    delete histoPROX;
	    delete histoPROY;
	    delete histoPRO_2D;
	    delete histoBXY;
	    delete histoINEF;

	    int sector = rpcId.sector();
	    //Near Side
	    
	    if(debug) std::cout<<"Before if = "<<makehtml<<std::endl;
	    if(makehtml){
	      command = "cp htmltemplates/indexLocal.html " + name + "/index.html"; system(command.c_str());
	      if(debug) std::cout<<"html for "<<rpcId<<std::endl;
	      
	      std::string color = "#0000FF";
	      if(pinoeff<threshold) color = "#ff4500";
	      
	      std::string target;
	      
	      if(sector==1||sector==2||sector==3){
		if(Disk==-3) target = "Dm3near.html";
		else if(Disk==-2) target = "Dm2near.html";
		else if(Disk==-1) target = "Dm1near.html";
		else if(Disk==1) target = "D1near.html";
		else if(Disk==2) target = "D2near.html";
		else if(Disk==3) target = "D3near.html";
	      }else{
		if(Disk==-3) target = "Dm3far.html";
		else if(Disk==-2) target = "Dm2far.html";
		else if(Disk==-1) target = "Dm1far.html";
		else if(Disk==1) target = "D1far.html";
		else if(Disk==2) target = "D2far.html";
		else if(Disk==3) target = "D3far.html";
	      }
	      command = "sed -e \"s|roll|" + name + "|g\" htmltemplates/indextemplate.html >> index" + target; system(command.c_str());
	      command = "sed -e \"s|roll|" + name + "|g\" -e \"s|colore|" + color + "|g\" htmltemplates/indexline.html >> insert" + target; system(command.c_str());
	    }
	    
	    mybxhisto = 50.+BXDistribution->GetMean()*10;
	    mybxerror = BXDistribution->GetRMS()*10;

	    signal_mybxhisto = 50.+Signal_BXDistribution->GetMean()*10;
	    signal_mybxerror = Signal_BXDistribution->GetRMS()*10;
	    
	    bxendcap->Fill(BXDistribution->GetMean(),BXDistribution->GetRMS());   
	    signal_bxendcap->Fill(Signal_BXDistribution->GetMean(),Signal_BXDistribution->GetRMS());   
	    
	  }else{
	    if(debug) std::cout<<"One of the histograms Doesn't exist!!!"<<std::endl;
	    exit(1);
	  }
	  
	  p=histoCSC->Integral();
	  o=histoRPC->Integral();

	  if(debug) std::cout<<"Integral P="<<p<<" Observed="<<o<<std::endl;

	  if(debug) std::cout<<"Disk="<<Disk<<std::endl;
	  
	  if(p!=0){
	    ef = float(o)/float(p); 
	    er = sqrt(ef*(1.-ef)/float(p));
	  }
	    
	  ef=ef*100;
	  er=er*100;

	  //Filling azimutal GregHistograms
	  
	  //ojo aca!

	  
	  if(!IsBadRoll(rpcId.rawId(),blacklist)){
	    if(rpcId.region()==1){
	      if(rpcId.station()==1 && rpcId.ring()==2){ ExGregD1R2->Fill(rpcsrv.segment(),pinoexpected);OcGregD1R2->Fill(rpcsrv.segment(),pinoobserved); if(debug) std::cout<<name<<"GREG Filling D1 R2 with o="<<o<<" p="<<p<<" ef="<<ef<<std::endl;}
	      if(rpcId.station()==1 && rpcId.ring()==3){ ExGregD1R3->Fill(rpcsrv.segment(),pinoexpected);OcGregD1R3->Fill(rpcsrv.segment(),pinoobserved); if(debug) std::cout<<name<<"GREG Filling D1 R3 with o="<<o<<" p="<<p<<" ef="<<ef<<std::endl;}
	      if(rpcId.station()==2 && rpcId.ring()==2){ ExGregD2R2->Fill(rpcsrv.segment(),pinoexpected);OcGregD2R2->Fill(rpcsrv.segment(),pinoobserved);}
	      if(rpcId.station()==2 && rpcId.ring()==3){ ExGregD2R3->Fill(rpcsrv.segment(),pinoexpected);OcGregD2R3->Fill(rpcsrv.segment(),pinoobserved);}
	      if(rpcId.station()==3 && rpcId.ring()==2){ ExGregD3R2->Fill(rpcsrv.segment(),pinoexpected);OcGregD3R2->Fill(rpcsrv.segment(),pinoobserved);}
	      if(rpcId.station()==3 && rpcId.ring()==3){ ExGregD3R3->Fill(rpcsrv.segment(),pinoexpected);OcGregD3R3->Fill(rpcsrv.segment(),pinoobserved);}
	    }else if(rpcId.region()==-1){
	      if(rpcId.station()==1 && rpcId.ring()==2){ ExGregDm1R2->Fill(rpcsrv.segment(),pinoexpected);OcGregDm1R2->Fill(rpcsrv.segment(),pinoobserved); if(debug) std::cout<<name<<"GREG Filling Dm1 R2 with o="<<o<<" p="<<p<<" ef="<<ef<<std::endl;}
	      if(rpcId.station()==1 && rpcId.ring()==3){ ExGregDm1R3->Fill(rpcsrv.segment(),pinoexpected);OcGregDm1R3->Fill(rpcsrv.segment(),pinoobserved); if(debug) std::cout<<name<<"GREG Filling Dm1 R3 with o="<<o<<" p="<<p<<" ef="<<ef<<std::endl;}
	      if(rpcId.station()==2 && rpcId.ring()==2){ ExGregDm2R2->Fill(rpcsrv.segment(),pinoexpected);OcGregDm2R2->Fill(rpcsrv.segment(),pinoobserved);}
	      if(rpcId.station()==2 && rpcId.ring()==3){ ExGregDm2R3->Fill(rpcsrv.segment(),pinoexpected);OcGregDm2R3->Fill(rpcsrv.segment(),pinoobserved);}
	      if(rpcId.station()==3 && rpcId.ring()==2){ ExGregDm3R2->Fill(rpcsrv.segment(),pinoexpected);OcGregDm3R2->Fill(rpcsrv.segment(),pinoobserved);}
	      if(rpcId.station()==3 && rpcId.ring()==3){ ExGregDm3R3->Fill(rpcsrv.segment(),pinoexpected);OcGregDm3R3->Fill(rpcsrv.segment(),pinoobserved);}
	    }
	  }else{
	    std::cout<<"blacklist skeeping roll "<<rpcId<<std::endl;
	  }

	  delete histoCLS;
	  
	  std::string camera = name.c_str();
	  float maskedratio = (float(NumberMasked)/float(nstrips))*100.;
	  float nopredictionsratio = (float(NumberWithOutPrediction)/float(nstrips))*100.;
	  
	  alignment<<name<<"  "<<rpcId.rawId()<<" "<<histoResidual->GetMean()<<" "<<histoResidual->GetRMS()<<std::endl;
	  
	  
	  if(histoResidual->GetMean()!=0. && pinoeff > 10. && nopredictionsratio < 70){
	    //MeanResiduals->Fill(histoResidual->GetMean());  
	    //RMSResiduals->Fill(histoResidual->GetRMS()); 
	    //MeanResiduals11->Fill(histoResidual->GetMean()); 
	    //RMSResiduals11->Fill(histoResidual->GetRMS());
	  }
	  
	  if(debug) std::cout<<"Super offsets in  "<<name<<" mean="<<histoResidual->GetMean()
			     <<" Pinoeff"<<pinoeff
			     <<" No Prediction Ratio"<<nopredictionsratio
			     <<std::endl;
	  
	  //Pigi Histos

	  int rollY = (*r)->id().roll();
	  if(rollY==1) rollY=3;
	  else if(rollY==3) rollY=1;

	  int Y=((*r)->id().ring()-2)*3+rollY;

	  if(debug) std::cout<<"Pigi "<<camera<<" "<<rpcsrv.shortname()<<" "<<(*r)->id()<<" ef="<<pinoeff<<" Y="<<Y<<std::endl;

	  if(Disk==-3) Diskm3Summary->SetBinContent(rpcsrv.segment(),Y,pinoeff);
	  else if(Disk==-2) Diskm2Summary->SetBinContent(rpcsrv.segment(),Y,pinoeff);
	  else if(Disk==-1) Diskm1Summary->SetBinContent(rpcsrv.segment(),Y,pinoeff);
	  else if(Disk==1) Disk1Summary->SetBinContent(rpcsrv.segment(),Y,pinoeff);
	  else if(Disk==2) Disk2Summary->SetBinContent(rpcsrv.segment(),Y,pinoeff);
	  else if(Disk==3) Disk3Summary->SetBinContent(rpcsrv.segment(),Y,pinoeff);

	  if(rpcId.region()==1){
	    PositiveEndCapSummary->SetBinContent(rpcsrv.segment(),(rpcId.station()-1)*2+rpcId.ring()-1,pinoeff);
	  }else if(rpcId.region()==-1){
	    NegativeEndCapSummary->SetBinContent(rpcsrv.segment(),(rpcId.station()-1)*2+rpcId.ring()-1,pinoeff);
	  }


 	  //Near Side

	  if(sector==1||sector==2||sector==6){
	    if(Disk==-3){
	      indexDisk[0]++;  
	      EffGlobDm3->SetBinContent(indexDisk[0],pinoeff);  
	      EffGlobDm3->SetBinError(indexDisk[0],pinoerr);  
	      EffGlobDm3->GetXaxis()->SetBinLabel(indexDisk[0],camera.c_str());
            
	      BXGlobDm3->SetBinContent(indexDisk[0],mybxhisto);  
	      BXGlobDm3->SetBinError(indexDisk[0],mybxerror);  
	      BXGlobDm3->GetXaxis()->SetBinLabel(indexDisk[0],camera.c_str());

	      Signal_BXGlobDm3->SetBinContent(indexDisk[0],signal_mybxhisto);  
	      Signal_BXGlobDm3->SetBinError(indexDisk[0],signal_mybxerror);  
	      Signal_BXGlobDm3->GetXaxis()->SetBinLabel(indexDisk[0],camera.c_str());
      
	      MaskedGlobDm3->SetBinContent(indexDisk[0],maskedratio);  
	      MaskedGlobDm3->GetXaxis()->SetBinLabel(indexDisk[0],camera.c_str());

	      AverageEffDm3->SetBinContent(indexDisk[0],averageeff);
	      AverageEffDm3->SetBinError(indexDisk[0],averageerr);  
	      AverageEffDm3->GetXaxis()->SetBinLabel(indexDisk[0],camera.c_str());
	      
	      if(debug) std::cout<<"Filling Problematic Histogram with"<<nopredictionsratio<<std::endl;
	      NoPredictionDm3->SetBinContent(indexDisk[0],nopredictionsratio);
              NoPredictionDm3->GetXaxis()->SetBinLabel(indexDisk[0],camera.c_str());
	    }else if(Disk==-2){
	      indexDisk[1]++;  
	      EffGlobDm2->SetBinContent(indexDisk[1],pinoeff);  
	      EffGlobDm2->SetBinError(indexDisk[1],pinoerr);  
	      EffGlobDm2->GetXaxis()->SetBinLabel(indexDisk[1],camera.c_str());

	      BXGlobDm2->SetBinContent(indexDisk[1],mybxhisto);  
	      BXGlobDm2->SetBinError(indexDisk[1],mybxerror);  
	      BXGlobDm2->GetXaxis()->SetBinLabel(indexDisk[1],camera.c_str());

	      Signal_BXGlobDm2->SetBinContent(indexDisk[1],signal_mybxhisto);  
	      Signal_BXGlobDm2->SetBinError(indexDisk[1],signal_mybxerror);  
	      Signal_BXGlobDm2->GetXaxis()->SetBinLabel(indexDisk[1],camera.c_str());
	      
	      MaskedGlobDm2->SetBinContent(indexDisk[1],maskedratio);  
	      MaskedGlobDm2->GetXaxis()->SetBinLabel(indexDisk[1],camera.c_str());

	      AverageEffDm2->SetBinContent(indexDisk[1],averageeff);
	      AverageEffDm2->SetBinError(indexDisk[1],averageerr);  
	      AverageEffDm2->GetXaxis()->SetBinLabel(indexDisk[1],camera.c_str());
	      
	      NoPredictionDm2->SetBinContent(indexDisk[1],nopredictionsratio);
              NoPredictionDm2->GetXaxis()->SetBinLabel(indexDisk[1],camera.c_str());
	    }else if(Disk==-1){
	      indexDisk[2]++;  
	      EffGlobDm1->SetBinContent(indexDisk[2],pinoeff);  
	      EffGlobDm1->SetBinError(indexDisk[2],pinoerr);  
	      EffGlobDm1->GetXaxis()->SetBinLabel(indexDisk[2],camera.c_str());  
	      
	      BXGlobDm1->SetBinContent(indexDisk[2],mybxhisto);  
	      BXGlobDm1->SetBinError(indexDisk[2],mybxerror);  
	      BXGlobDm1->GetXaxis()->SetBinLabel(indexDisk[2],camera.c_str());

	      Signal_BXGlobDm1->SetBinContent(indexDisk[2],signal_mybxhisto);  
	      Signal_BXGlobDm1->SetBinError(indexDisk[2],signal_mybxerror);  
	      Signal_BXGlobDm1->GetXaxis()->SetBinLabel(indexDisk[2],camera.c_str());
	      
	      MaskedGlobDm1->SetBinContent(indexDisk[2],maskedratio);  
	      MaskedGlobDm1->GetXaxis()->SetBinLabel(indexDisk[2],camera.c_str());

	      AverageEffDm1->SetBinContent(indexDisk[2],averageeff);
	      AverageEffDm1->SetBinError(indexDisk[2],averageerr);  
	      AverageEffDm1->GetXaxis()->SetBinLabel(indexDisk[2],camera.c_str());
	      
	      NoPredictionDm1->SetBinContent(indexDisk[2],nopredictionsratio);
              NoPredictionDm1->GetXaxis()->SetBinLabel(indexDisk[2],camera.c_str());

	    }else if(Disk==1){
	      indexDisk[3]++;  
	      EffGlobD1->SetBinContent(indexDisk[3],pinoeff);  
	      EffGlobD1->SetBinError(indexDisk[3],pinoerr);  
	      EffGlobD1->GetXaxis()->SetBinLabel(indexDisk[3],camera.c_str());  
	      
	      BXGlobD1->SetBinContent(indexDisk[3],mybxhisto);  
	      BXGlobD1->SetBinError(indexDisk[3],mybxerror);  
	      BXGlobD1->GetXaxis()->SetBinLabel(indexDisk[3],camera.c_str());

	      Signal_BXGlobD1->SetBinContent(indexDisk[3],signal_mybxhisto);  
	      Signal_BXGlobD1->SetBinError(indexDisk[3],signal_mybxerror);  
	      Signal_BXGlobD1->GetXaxis()->SetBinLabel(indexDisk[3],camera.c_str());
	      
	      MaskedGlobD1->SetBinContent(indexDisk[3],maskedratio);  
	      MaskedGlobD1->GetXaxis()->SetBinLabel(indexDisk[3],camera.c_str());

	      AverageEffD1->SetBinContent(indexDisk[3],averageeff);
	      AverageEffD1->SetBinError(indexDisk[3],averageerr);  
	      AverageEffD1->GetXaxis()->SetBinLabel(indexDisk[3],camera.c_str());
	      
	      NoPredictionD1->SetBinContent(indexDisk[3],nopredictionsratio);
              NoPredictionD1->GetXaxis()->SetBinLabel(indexDisk[3],camera.c_str());	      
	    }else if(Disk==2){
	      indexDisk[4]++;
	      EffGlobD2->SetBinContent(indexDisk[4],pinoeff);
	      EffGlobD2->SetBinError(indexDisk[4],pinoerr);
	      EffGlobD2->GetXaxis()->SetBinLabel(indexDisk[4],camera.c_str());

	      BXGlobD2->SetBinContent(indexDisk[4],mybxhisto);  
	      BXGlobD2->SetBinError(indexDisk[4],mybxerror);  
	      BXGlobD2->GetXaxis()->SetBinLabel(indexDisk[4],camera.c_str());

	      Signal_BXGlobD2->SetBinContent(indexDisk[4],signal_mybxhisto);  
	      Signal_BXGlobD2->SetBinError(indexDisk[4],signal_mybxerror);  
	      Signal_BXGlobD2->GetXaxis()->SetBinLabel(indexDisk[4],camera.c_str());

	      MaskedGlobD2->SetBinContent(indexDisk[4],maskedratio);  
	      MaskedGlobD2->GetXaxis()->SetBinLabel(indexDisk[4],camera.c_str());

	      AverageEffD2->SetBinContent(indexDisk[4],averageeff);
	      AverageEffD2->SetBinError(indexDisk[4],averageerr);  
	      AverageEffD2->GetXaxis()->SetBinLabel(indexDisk[4],camera.c_str());
	      
	      NoPredictionD2->SetBinContent(indexDisk[4],nopredictionsratio);
              NoPredictionD2->GetXaxis()->SetBinLabel(indexDisk[4],camera.c_str());	      
	    }else if(Disk==3){
	      indexDisk[5]++;
	      EffGlobD3->SetBinContent(indexDisk[5],pinoeff);
	      EffGlobD3->SetBinError(indexDisk[5],pinoerr);
	      EffGlobD3->GetXaxis()->SetBinLabel(indexDisk[5],camera.c_str());

	      BXGlobD3->SetBinContent(indexDisk[5],mybxhisto);  
	      BXGlobD3->SetBinError(indexDisk[5],mybxerror);  
	      BXGlobD3->GetXaxis()->SetBinLabel(indexDisk[5],camera.c_str());

	      Signal_BXGlobD3->SetBinContent(indexDisk[5],signal_mybxhisto);  
	      Signal_BXGlobD3->SetBinError(indexDisk[5],signal_mybxerror);  
	      Signal_BXGlobD3->GetXaxis()->SetBinLabel(indexDisk[5],camera.c_str());

	      MaskedGlobD3->SetBinContent(indexDisk[5],maskedratio);  
	      MaskedGlobD3->GetXaxis()->SetBinLabel(indexDisk[5],camera.c_str());

	      AverageEffD3->SetBinContent(indexDisk[5],averageeff);
	      AverageEffD3->SetBinError(indexDisk[5],averageerr);  
	      AverageEffD3->GetXaxis()->SetBinLabel(indexDisk[5],camera.c_str());
	      
	      NoPredictionD3->SetBinContent(indexDisk[5],nopredictionsratio);
              NoPredictionD3->GetXaxis()->SetBinLabel(indexDisk[5],camera.c_str());	      
	    }
	  }else{//Far Side 
	    
	    if(Disk==-3){
	      indexDiskf[0]++;  
	      EffGlobDm3far->SetBinContent(indexDiskf[0],pinoeff);  
	      EffGlobDm3far->SetBinError(indexDiskf[0],pinoerr);  
	      EffGlobDm3far->GetXaxis()->SetBinLabel(indexDiskf[0],camera.c_str());

	      BXGlobDm3far->SetBinContent(indexDiskf[0],mybxhisto);  
	      BXGlobDm3far->SetBinError(indexDiskf[0],mybxerror);  
	      BXGlobDm3far->GetXaxis()->SetBinLabel(indexDiskf[0],camera.c_str());
	      
	      Signal_BXGlobDm3far->SetBinContent(indexDiskf[0],signal_mybxhisto);  
	      Signal_BXGlobDm3far->SetBinError(indexDiskf[0],signal_mybxerror);  
	      Signal_BXGlobDm3far->GetXaxis()->SetBinLabel(indexDiskf[0],camera.c_str());
	      
	      MaskedGlobDm3far->SetBinContent(indexDiskf[0],maskedratio);
	      MaskedGlobDm3far->GetXaxis()->SetBinLabel(indexDiskf[0],camera.c_str());
	      
	      AverageEffDm3far->SetBinContent(indexDiskf[0],averageeff);
              AverageEffDm3far->SetBinError(indexDiskf[0],averageerr);
              AverageEffDm3far->GetXaxis()->SetBinLabel(indexDiskf[0],camera.c_str());

              NoPredictionDm3far->SetBinContent(indexDisk[0],nopredictionsratio);
              NoPredictionDm3far->GetXaxis()->SetBinLabel(indexDisk[0],camera.c_str());

	    }
	    else if(Disk==-2){
	      indexDiskf[1]++;  
	      EffGlobDm2far->SetBinContent(indexDiskf[1],pinoeff);  
	      EffGlobDm2far->SetBinError(indexDiskf[1],pinoerr);  
	      EffGlobDm2far->GetXaxis()->SetBinLabel(indexDiskf[1],camera.c_str());

	      BXGlobDm2far->SetBinContent(indexDiskf[1],mybxhisto);  
	      BXGlobDm2far->SetBinError(indexDiskf[1],mybxerror);  
	      BXGlobDm2far->GetXaxis()->SetBinLabel(indexDiskf[1],camera.c_str());

	      Signal_BXGlobDm2far->SetBinContent(indexDiskf[1],signal_mybxhisto);  
	      Signal_BXGlobDm2far->SetBinError(indexDiskf[1],signal_mybxerror);  
	      Signal_BXGlobDm2far->GetXaxis()->SetBinLabel(indexDiskf[1],camera.c_str());
	      
	      MaskedGlobDm2far->SetBinContent(indexDiskf[1],maskedratio);
	      MaskedGlobDm2far->GetXaxis()->SetBinLabel(indexDiskf[1],camera.c_str());
	      
	      AverageEffDm2far->SetBinContent(indexDiskf[1],averageeff);
              AverageEffDm2far->SetBinError(indexDiskf[1],averageerr);
              AverageEffDm2far->GetXaxis()->SetBinLabel(indexDiskf[1],camera.c_str());

              NoPredictionDm2far->SetBinContent(indexDisk[1],nopredictionsratio);
              NoPredictionDm2far->GetXaxis()->SetBinLabel(indexDisk[1],camera.c_str());

	    }else if(Disk==-1){
	      indexDiskf[2]++;  
	      EffGlobDm1far->SetBinContent(indexDiskf[2],pinoeff);  
	      EffGlobDm1far->SetBinError(indexDiskf[2],pinoerr);  
	      EffGlobDm1far->GetXaxis()->SetBinLabel(indexDiskf[2],camera.c_str());  
	      
	      BXGlobDm1far->SetBinContent(indexDiskf[2],mybxhisto);  
	      BXGlobDm1far->SetBinError(indexDiskf[2],mybxerror);  
	      BXGlobDm1far->GetXaxis()->SetBinLabel(indexDiskf[2],camera.c_str());

	      Signal_BXGlobDm1far->SetBinContent(indexDiskf[2],signal_mybxhisto);  
	      Signal_BXGlobDm1far->SetBinError(indexDiskf[2],signal_mybxerror);  
	      Signal_BXGlobDm1far->GetXaxis()->SetBinLabel(indexDiskf[2],camera.c_str());
	      
	      MaskedGlobDm1far->SetBinContent(indexDiskf[2],maskedratio);
	      MaskedGlobDm1far->GetXaxis()->SetBinLabel(indexDiskf[2],camera.c_str());

	      AverageEffDm1far->SetBinContent(indexDiskf[2],averageeff);
              AverageEffDm1far->SetBinError(indexDiskf[2],averageerr);
              AverageEffDm1far->GetXaxis()->SetBinLabel(indexDiskf[2],camera.c_str());

              NoPredictionDm1far->SetBinContent(indexDiskf[2],nopredictionsratio);
              NoPredictionDm1far->GetXaxis()->SetBinLabel(indexDiskf[2],camera.c_str());

	    }else if(Disk==1){
	      indexDiskf[3]++;  
	      EffGlobD1far->SetBinContent(indexDiskf[3],pinoeff);  
	      EffGlobD1far->SetBinError(indexDiskf[3],pinoerr);  
	      EffGlobD1far->GetXaxis()->SetBinLabel(indexDiskf[3],camera.c_str());  
	      
	      BXGlobD1far->SetBinContent(indexDiskf[3],mybxhisto);  
	      BXGlobD1far->SetBinError(indexDiskf[3],mybxerror);  
	      BXGlobD1far->GetXaxis()->SetBinLabel(indexDiskf[3],camera.c_str());

	      Signal_BXGlobD1far->SetBinContent(indexDiskf[3],signal_mybxhisto);  
	      Signal_BXGlobD1far->SetBinError(indexDiskf[3],signal_mybxerror);  
	      Signal_BXGlobD1far->GetXaxis()->SetBinLabel(indexDiskf[3],camera.c_str());
	      
	      MaskedGlobD1far->SetBinContent(indexDiskf[3],maskedratio);
	      MaskedGlobD1far->GetXaxis()->SetBinLabel(indexDiskf[3],camera.c_str());
	      
	      AverageEffD1far->SetBinContent(indexDiskf[3],averageeff);
              AverageEffD1far->SetBinError(indexDiskf[3],averageerr);
              AverageEffD1far->GetXaxis()->SetBinLabel(indexDiskf[3],camera.c_str());

              NoPredictionD1far->SetBinContent(indexDiskf[3],nopredictionsratio);
              NoPredictionD1far->GetXaxis()->SetBinLabel(indexDiskf[3],camera.c_str());

	    }else if(Disk==2){
	      indexDiskf[4]++;
	      EffGlobD2far->SetBinContent(indexDiskf[4],pinoeff);
	      EffGlobD2far->SetBinError(indexDiskf[4],pinoerr);
	      EffGlobD2far->GetXaxis()->SetBinLabel(indexDiskf[4],camera.c_str());

	      BXGlobD2far->SetBinContent(indexDiskf[4],mybxhisto);  
	      BXGlobD2far->SetBinError(indexDiskf[4],mybxerror);  
	      BXGlobD2far->GetXaxis()->SetBinLabel(indexDiskf[4],camera.c_str());
	      
	      Signal_BXGlobD2far->SetBinContent(indexDiskf[4],signal_mybxhisto);  
	      Signal_BXGlobD2far->SetBinError(indexDiskf[4],signal_mybxerror);  
	      Signal_BXGlobD2far->GetXaxis()->SetBinLabel(indexDiskf[4],camera.c_str());
	      
	      MaskedGlobD2far->SetBinContent(indexDiskf[4],maskedratio);
	      MaskedGlobD2far->GetXaxis()->SetBinLabel(indexDiskf[4],camera.c_str());

	      AverageEffD2far->SetBinContent(indexDiskf[4],averageeff);
              AverageEffD2far->SetBinError(indexDiskf[4],averageerr);
              AverageEffD2far->GetXaxis()->SetBinLabel(indexDiskf[4],camera.c_str());

              NoPredictionD2far->SetBinContent(indexDiskf[4],nopredictionsratio);
              NoPredictionD2far->GetXaxis()->SetBinLabel(indexDiskf[4],camera.c_str());
	    }else if(Disk==3){
	      indexDiskf[5]++;
	      EffGlobD3far->SetBinContent(indexDiskf[5],pinoeff);
	      EffGlobD3far->SetBinError(indexDiskf[5],pinoerr);
	      EffGlobD3far->GetXaxis()->SetBinLabel(indexDiskf[5],camera.c_str());

	      BXGlobD3far->SetBinContent(indexDiskf[5],mybxhisto);  
	      BXGlobD3far->SetBinError(indexDiskf[5],mybxerror);  
	      BXGlobD3far->GetXaxis()->SetBinLabel(indexDiskf[5],camera.c_str());

	      Signal_BXGlobD3far->SetBinContent(indexDiskf[5],signal_mybxhisto);  
	      Signal_BXGlobD3far->SetBinError(indexDiskf[5],signal_mybxerror);  
	      Signal_BXGlobD3far->GetXaxis()->SetBinLabel(indexDiskf[5],camera.c_str());
	      
	      MaskedGlobD3far->SetBinContent(indexDiskf[5],maskedratio);
	      MaskedGlobD3far->GetXaxis()->SetBinLabel(indexDiskf[5],camera.c_str());

	      AverageEffD3far->SetBinContent(indexDiskf[5],averageeff);
              AverageEffD3far->SetBinError(indexDiskf[5],averageerr);
              AverageEffD3far->GetXaxis()->SetBinLabel(indexDiskf[5],camera.c_str());

              NoPredictionD3far->SetBinContent(indexDiskf[5],nopredictionsratio);
              NoPredictionD3far->GetXaxis()->SetBinLabel(indexDiskf[5],camera.c_str());
	    }
	  }//Finishing EndCap
	}
      }
    }
  }

  if(debug) std::cout<<"Outside the loop of rolls"<<std::endl;
  
  if(makehtml){
    command = "cat htmltemplates/indextail.html >> indexDm3near.html"; system(command.c_str());
    command = "cat htmltemplates/indextail.html >> indexDm2near.html"; system(command.c_str());
    command = "cat htmltemplates/indextail.html >> indexDm1near.html"; system(command.c_str());

    command = "cat htmltemplates/indextail.html >> indexD1near.html"; system(command.c_str());
    command = "cat htmltemplates/indextail.html >> indexD2near.html"; system(command.c_str());
    command = "cat htmltemplates/indextail.html >> indexD3near.html"; system(command.c_str());

    command = "cat htmltemplates/indextail.html >> indexDm3far.html"; system(command.c_str());
    command = "cat htmltemplates/indextail.html >> indexDm2far.html"; system(command.c_str());
    command = "cat htmltemplates/indextail.html >> indexDm1far.html"; system(command.c_str());

    command = "cat htmltemplates/indextail.html >> indexD1far.html"; system(command.c_str());
    command = "cat htmltemplates/indextail.html >> indexD2far.html"; system(command.c_str());
    command = "cat htmltemplates/indextail.html >> indexD3far.html"; system(command.c_str());

    command = "cat htmltemplates/indextail.html >> indexWm2near.html"; system(command.c_str());
    command = "cat htmltemplates/indextail.html >> indexWm2far.html"; system(command.c_str());
    command = "cat htmltemplates/indextail.html >> indexWm1near.html"; system(command.c_str());
    command = "cat htmltemplates/indextail.html >> indexWm1far.html"; system(command.c_str());
    command = "cat htmltemplates/indextail.html >> indexW0near.html"; system(command.c_str());
    command = "cat htmltemplates/indextail.html >> indexW0far.html"; system(command.c_str());
    command = "cat htmltemplates/indextail.html >> indexW1near.html"; system(command.c_str());
    command = "cat htmltemplates/indextail.html >> indexW1far.html"; system(command.c_str());
    command = "cat htmltemplates/indextail.html >> indexW2near.html"; system(command.c_str());
    command = "cat htmltemplates/indextail.html >> indexW2far.html"; system(command.c_str());
  }

  TH1F * EfficiencyPerChamberNumber = new TH1F("EfficiencyPerChamberNumber","Efficiency per Chamber Number (All Rings and Disks)",36,0.5,36.5);

  float eff,N,err;
  int occCham = 0;
  int k =0; 

  for(k=1;k<=36;k++){
    float h = sin((k-1)*10*3.14159565/180.);
    
    float acumulatedexpected = 0;
    float acumulatedobserved = 0;
    
    err=0; eff=0; N=ExGregD1R2->GetBinContent(k);
    acumulatedexpected = N + acumulatedexpected;
    acumulatedobserved = acumulatedobserved + OcGregD1R2->GetBinContent(k);
    if(N!=0.){eff = OcGregD1R2->GetBinContent(k)/N; err=sqrt(eff*(1-eff)/N);}
    if(HasBadRoll(1,1,2,k,blacklist)){GregD1R2_black->SetBinContent(k,eff); GregD1R2_black->SetBinError(k,err);}
    else{GregD1R2->SetBinContent(k,eff); GregD1R2->SetBinError(k,err);}
    HeightVsEffR2->Fill(eff,h);
    //Cesare
    if(eff != 0) EndcapEffTot->Fill(100*eff);
    occCham = OcGregD1R2->GetBinContent(k);
    EffChamEndcap<<"D1R2"<<" "<<k<<" "<<eff<<" "<<err<<" "<<occCham<<" "<<N<<std::endl;
    
    err=0; eff=0; N=ExGregD1R3->GetBinContent(k);
    acumulatedexpected = N + acumulatedexpected;
    acumulatedobserved = acumulatedobserved + OcGregD1R3->GetBinContent(k);
    if(N!=0.){eff = OcGregD1R3->GetBinContent(k)/N;err=sqrt(eff*(1-eff)/N);}
    if(HasBadRoll(1,1,3,k,blacklist)){GregD1R3_black->SetBinContent(k,eff); GregD1R3_black->SetBinError(k,err);}
    else{GregD1R3->SetBinContent(k,eff); GregD1R3->SetBinError(k,err);}
    HeightVsEffR3->Fill(eff,h);
    //Cesare
    if(eff != 0) EndcapEffTot->Fill(100*eff);
    occCham = OcGregD1R3->GetBinContent(k);
    EffChamEndcap<<"D1R3"<<" "<<k<<" "<<eff<<" "<<err<<" "<<occCham<<" "<<N<<std::endl;
    
    err=0; eff=0; N=ExGregD2R2->GetBinContent(k);
    acumulatedexpected = N + acumulatedexpected;
    acumulatedobserved = acumulatedobserved + OcGregD2R2->GetBinContent(k);
    if(N!=0.){ eff = OcGregD2R2->GetBinContent(k)/N;err=sqrt(eff*(1-eff)/N);}
    if(HasBadRoll(1,2,2,k,blacklist)){GregD2R2_black->SetBinContent(k,eff); GregD2R2_black->SetBinError(k,err);}
    else{GregD2R2->SetBinContent(k,eff); GregD2R2->SetBinError(k,err);}
    HeightVsEffR2->Fill(eff,h);
    //Cesare
    if(eff != 0) EndcapEffTot->Fill(100*eff);
    occCham = OcGregD2R2->GetBinContent(k);
    EffChamEndcap<<"D2R2"<<" "<<k<<" "<<eff<<" "<<err<<" "<<occCham<<" "<<N<<std::endl;

    err=0; eff=0; N=ExGregD2R3->GetBinContent(k);
    acumulatedexpected = N + acumulatedexpected;
    acumulatedobserved = acumulatedobserved + OcGregD2R3->GetBinContent(k);
    if(N!=0.){ eff = OcGregD2R3->GetBinContent(k)/N;err=sqrt(eff*(1-eff)/N);}
    if(HasBadRoll(1,2,3,k,blacklist)){GregD2R3_black->SetBinContent(k,eff); GregD2R3_black->SetBinError(k,err);}
    else{GregD2R3->SetBinContent(k,eff); GregD2R3->SetBinError(k,err);}
    HeightVsEffR3->Fill(eff,h);
    //Cesare
    if(eff != 0) EndcapEffTot->Fill(100*eff);
    occCham = OcGregD2R3->GetBinContent(k);
    EffChamEndcap<<"D2R3"<<" "<<k<<" "<<eff<<" "<<err<<" "<<occCham<<" "<<N<<std::endl;

    err=0; eff=0; N=ExGregD3R2->GetBinContent(k);
    acumulatedexpected = N + acumulatedexpected;
    acumulatedobserved = acumulatedobserved + OcGregD3R2->GetBinContent(k);
    if(N!=0.){ eff = OcGregD3R2->GetBinContent(k)/N;err=sqrt(eff*(1-eff)/N);}
    if(HasBadRoll(1,3,2,k,blacklist)){GregD3R2_black->SetBinContent(k,eff); GregD3R2_black->SetBinError(k,err);}
    else{GregD3R2->SetBinContent(k,eff); GregD3R2->SetBinError(k,err);}
    HeightVsEffR2->Fill(eff,h);
    //Cesare
    if(eff != 0) EndcapEffTot->Fill(100*eff);
    occCham = OcGregD3R2->GetBinContent(k);
    EffChamEndcap<<"D3R2"<<" "<<k<<" "<<eff<<" "<<err<<" "<<occCham<<" "<<N<<std::endl;

    err=0; eff=0; N=ExGregD3R3->GetBinContent(k);
    acumulatedexpected = N + acumulatedexpected;
    acumulatedobserved = acumulatedobserved + OcGregD3R3->GetBinContent(k);
    if(N!=0.){ eff = OcGregD3R3->GetBinContent(k)/N;err=sqrt(eff*(1-eff)/N);}
    if(HasBadRoll(1,3,3,k,blacklist)){GregD3R3_black->SetBinContent(k,eff); GregD3R3_black->SetBinError(k,err);}
    else{GregD3R3->SetBinContent(k,eff); GregD3R3->SetBinError(k,err);}
    HeightVsEffR3->Fill(eff,h);
    //Cesare
    if(eff != 0) EndcapEffTot->Fill(100*eff);
    occCham = OcGregD3R3->GetBinContent(k);
    EffChamEndcap<<"D3R3"<<" "<<k<<" "<<eff<<" "<<err<<" "<<occCham<<" "<<N<<std::endl;

    //Negative EndCap

    err=0; eff=0; N=ExGregDm1R2->GetBinContent(k);
    acumulatedexpected = N + acumulatedexpected;
    acumulatedobserved = acumulatedobserved + OcGregDm1R2->GetBinContent(k);
    if(N!=0.){ eff = OcGregDm1R2->GetBinContent(k)/N; err=sqrt(eff*(1-eff)/N);}
    if(HasBadRoll(-1,1,2,k,blacklist)){GregDm1R2_black->SetBinContent(k,eff); GregDm1R2_black->SetBinError(k,err);}
    else{GregDm1R2->SetBinContent(k,eff); GregDm1R2->SetBinError(k,err);}
    HeightVsEffR2->Fill(eff,h);
    //Cesare
    if(eff != 0) EndcapEffTot->Fill(100*eff);
    occCham = OcGregDm1R2->GetBinContent(k);
    EffChamEndcap<<"Dm1R2"<<" "<<k<<" "<<eff<<" "<<err<<" "<<occCham<<" "<<N<<std::endl;
    
    err=0; eff=0; N=ExGregDm1R3->GetBinContent(k);
    acumulatedexpected = N + acumulatedexpected;
    acumulatedobserved = acumulatedobserved + OcGregDm1R3->GetBinContent(k);
    if(N!=0.){eff = OcGregDm1R3->GetBinContent(k)/N;err=sqrt(eff*(1-eff)/N);}
    if(HasBadRoll(-1,1,3,k,blacklist)){GregDm1R3_black->SetBinContent(k,eff); GregDm1R3_black->SetBinError(k,err);}
    else{GregDm1R3->SetBinContent(k,eff); GregDm1R3->SetBinError(k,err);}
    HeightVsEffR3->Fill(eff,h);
    //Cesare
    if(eff != 0) EndcapEffTot->Fill(100*eff);
    occCham = OcGregDm1R3->GetBinContent(k);
    EffChamEndcap<<"Dm1R3"<<" "<<k<<" "<<eff<<" "<<err<<" "<<occCham<<" "<<N<<std::endl;
    
    err=0; eff=0; N=ExGregDm2R2->GetBinContent(k);
    acumulatedexpected = N + acumulatedexpected;
    acumulatedobserved = acumulatedobserved + OcGregDm2R2->GetBinContent(k);
    if(N!=0.){ eff = OcGregDm2R2->GetBinContent(k)/N;err=sqrt(eff*(1-eff)/N);}
    if(HasBadRoll(-1,2,2,k,blacklist)){GregDm2R2_black->SetBinContent(k,eff); GregDm2R2_black->SetBinError(k,err);}
    else{GregDm2R2->SetBinContent(k,eff); GregDm2R2->SetBinError(k,err);}
    HeightVsEffR2->Fill(eff,h);
    //Cesare
    if(eff != 0) EndcapEffTot->Fill(100*eff);
    occCham = OcGregDm2R2->GetBinContent(k);
    EffChamEndcap<<"Dm2R2"<<" "<<k<<" "<<eff<<" "<<err<<" "<<occCham<<" "<<N<<std::endl;

    err=0; eff=0; N=ExGregDm2R3->GetBinContent(k);
    acumulatedexpected = N + acumulatedexpected;
    acumulatedobserved = acumulatedobserved + OcGregDm2R3->GetBinContent(k);
    if(N!=0.){ eff = OcGregDm2R3->GetBinContent(k)/N;err=sqrt(eff*(1-eff)/N);}
    if(HasBadRoll(-1,2,3,k,blacklist)){GregDm2R3_black->SetBinContent(k,eff); GregDm2R3_black->SetBinError(k,err);}
    else{GregDm2R3->SetBinContent(k,eff); GregDm2R3->SetBinError(k,err);}
    HeightVsEffR3->Fill(eff,h);
    //Cesare
    if(eff != 0) EndcapEffTot->Fill(100*eff);
    occCham = OcGregDm2R3->GetBinContent(k);
    EffChamEndcap<<"Dm2R3"<<" "<<k<<" "<<eff<<" "<<err<<" "<<occCham<<" "<<N<<std::endl;

    err=0; eff=0; N=ExGregDm3R2->GetBinContent(k);
    acumulatedexpected = N + acumulatedexpected;
    acumulatedobserved = acumulatedobserved + OcGregDm3R2->GetBinContent(k);
    if(N!=0.){ eff = OcGregDm3R2->GetBinContent(k)/N;err=sqrt(eff*(1-eff)/N);}
    if(HasBadRoll(-1,3,2,k,blacklist)){GregDm3R2_black->SetBinContent(k,eff); GregDm3R2_black->SetBinError(k,err);}
    else{GregDm3R2->SetBinContent(k,eff); GregDm3R2->SetBinError(k,err);}
    HeightVsEffR2->Fill(eff,h);
    //Cesare
    if(eff != 0) EndcapEffTot->Fill(100*eff);
    occCham = OcGregDm3R2->GetBinContent(k);
    EffChamEndcap<<"Dm3R2"<<" "<<k<<" "<<eff<<" "<<err<<" "<<occCham<<" "<<N<<std::endl;

    err=0; eff=0; N=ExGregDm3R3->GetBinContent(k);
    acumulatedexpected = N + acumulatedexpected;
    acumulatedobserved = acumulatedobserved + OcGregDm3R3->GetBinContent(k);
    if(N!=0.){ eff = OcGregDm3R3->GetBinContent(k)/N;err=sqrt(eff*(1-eff)/N);}
    if(HasBadRoll(-1,3,3,k,blacklist)){GregDm3R3_black->SetBinContent(k,eff); GregDm3R3_black->SetBinError(k,err);}
    else{GregDm3R3->SetBinContent(k,eff); GregDm3R3->SetBinError(k,err);}
    HeightVsEffR3->Fill(eff,h);
    //Cesare
    if(eff != 0) EndcapEffTot->Fill(100*eff);
    occCham = OcGregDm3R3->GetBinContent(k);
    EffChamEndcap<<"Dm3R3"<<" "<<k<<" "<<eff<<" "<<err<<" "<<occCham<<" "<<N<<std::endl;

    if(acumulatedexpected!=0){ 
      eff = acumulatedobserved/acumulatedexpected; err=sqrt(eff*(1-eff)/acumulatedexpected);
      EfficiencyPerChamberNumber->SetBinContent(k,eff); EfficiencyPerChamberNumber->SetBinError(k,err);
      std::cout<<acumulatedobserved<<" "<<acumulatedexpected<<" "<<eff<<" "<<err<<std::endl;
    }
  }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //Cesare

    TH1F * EfficiencyPerDisk = new TH1F("EfficiencyPerDisk","Efficiency per Disk",8,0.5,8.5);

    float exgDm3 = ExGregDm3R2->Integral()+ExGregDm3R3->Integral(); float obgDm3 = OcGregDm3R2->Integral()+OcGregDm3R3->Integral();
    float effDm3 = obgDm3/exgDm3; float errDm3 = sqrt(effDm3*(1-effDm3)/exgDm3);
    EfficiencyPerDisk->GetXaxis()->SetBinLabel(2,"Dm3");
    //EfficiencyPerDisk->SetBinContent(2,EffDm3->GetMean());
    EfficiencyPerDisk->SetBinContent(2,100*effDm3);
    EfficiencyPerDisk->SetBinError(2,100*errDm3);

    float exgDm2 = ExGregDm2R2->Integral()+ExGregDm2R3->Integral(); float obgDm2 = OcGregDm2R2->Integral()+OcGregDm2R3->Integral();
    float effDm2 = obgDm2/exgDm2; float errDm2 = sqrt(effDm2*(1-effDm2)/exgDm2);
    EfficiencyPerDisk->GetXaxis()->SetBinLabel(3,"Dm2");
    //EfficiencyPerDisk->SetBinContent(3,EffDm2->GetMean());
    EfficiencyPerDisk->SetBinContent(3,100*effDm2);
    EfficiencyPerDisk->SetBinError(3,100*errDm2);

    float exgDm1 = ExGregDm1R2->Integral()+ExGregDm1R3->Integral(); float obgDm1 = OcGregDm1R2->Integral()+OcGregDm1R3->Integral();
    float effDm1 = obgDm1/exgDm1; float errDm1 = sqrt(effDm1*(1-effDm1)/exgDm1);
    EfficiencyPerDisk->GetXaxis()->SetBinLabel(4,"Dm1");
    //EfficiencyPerDisk->SetBinContent(4,EffDm1->GetMean());
    EfficiencyPerDisk->SetBinContent(4,100*effDm1);
    EfficiencyPerDisk->SetBinError(4,100*errDm1);

    float exgD1 = ExGregD1R2->Integral()+ExGregD1R3->Integral(); float obgD1 = OcGregD1R2->Integral()+OcGregD1R3->Integral();
    float effD1 = obgD1/exgD1; float errD1 = sqrt(effD1*(1-effD1)/exgD1);
    EfficiencyPerDisk->GetXaxis()->SetBinLabel(5,"D1");
    //EfficiencyPerDisk->SetBinContent(5,EffD1->GetMean());
    EfficiencyPerDisk->SetBinContent(5,100*effD1);
    EfficiencyPerDisk->SetBinError(5,100*errD1);

    float exgD2 = ExGregD2R2->Integral()+ExGregD2R3->Integral(); float obgD2 = OcGregD2R2->Integral()+OcGregD2R3->Integral();
    float effD2 = obgD2/exgD2; float errD2 = sqrt(effD2*(1-effD2)/exgD2);
    EfficiencyPerDisk->GetXaxis()->SetBinLabel(6,"D2");
    //EfficiencyPerDisk->SetBinContent(6,EffD2->GetMean());
    EfficiencyPerDisk->SetBinContent(6,100*effD2);
    EfficiencyPerDisk->SetBinError(6,100*errD2);

    float exgD3 = ExGregD3R2->Integral()+ExGregD3R3->Integral(); float obgD3 = OcGregD3R2->Integral()+OcGregD3R3->Integral();
    float effD3 = obgD3/exgD3; float errD3 = sqrt(effD3*(1-effD3)/exgD3);
    EfficiencyPerDisk->GetXaxis()->SetBinLabel(7,"D3");
    //EfficiencyPerDisk->SetBinContent(7,EffD3->GetMean());
    EfficiencyPerDisk->SetBinContent(7,100*effD3);
    EfficiencyPerDisk->SetBinError(7,100*errD3);

    EfficiencyPerDisk->GetXaxis()->SetBinLabel(1,"Dm4");

    EfficiencyPerDisk->GetXaxis()->SetBinLabel(8,"D4");

    EfficiencyPerDisk->Write();

    Ca101->cd();
    EfficiencyPerDisk->Draw();
    Ca101->SaveAs("EfficiencyPerDisk.png");
    Ca101->Close();


    TH1F * EfficiencyPerStation = new TH1F("EfficiencyPerStation","Efficiency per Station",4,0.5,4.5);

    float exgS3 = ExGregDm3R2->Integral()+ExGregDm3R3->Integral()+ExGregD3R2->Integral()+ExGregD3R3->Integral(); 
    float obgS3 = OcGregDm3R2->Integral()+OcGregDm3R3->Integral()+OcGregD3R2->Integral()+OcGregD3R3->Integral();
    float effS3 = obgS3/exgS3; float errS3 = sqrt(effS3*(1-effS3)/exgS3);
    EfficiencyPerStation->GetXaxis()->SetBinLabel(3,"S3");
    EfficiencyPerStation->SetBinContent(3,100*effS3);
    EfficiencyPerStation->SetBinError(3,100*errS3);

    float exgS2 = ExGregDm2R2->Integral()+ExGregDm2R3->Integral()+ExGregD2R2->Integral()+ExGregD2R3->Integral(); 
    float obgS2 = OcGregDm2R2->Integral()+OcGregDm2R3->Integral()+OcGregD2R2->Integral()+OcGregD2R3->Integral();
    float effS2 = obgS2/exgS2; float errS2 = sqrt(effS2*(1-effS2)/exgS2);
    EfficiencyPerStation->GetXaxis()->SetBinLabel(2,"S2");
    EfficiencyPerStation->SetBinContent(2,100*effS2);
    EfficiencyPerStation->SetBinError(2,100*errS2);

    float exgS1 = ExGregDm1R2->Integral()+ExGregDm1R3->Integral()+ExGregD1R2->Integral()+ExGregD1R3->Integral(); 
    float obgS1 = OcGregDm1R2->Integral()+OcGregDm1R3->Integral()+OcGregD1R2->Integral()+OcGregD1R3->Integral();
    float effS1 = obgS1/exgS1; float errS1 = sqrt(effS1*(1-effS1)/exgS1);
    EfficiencyPerStation->GetXaxis()->SetBinLabel(1,"S1");
    EfficiencyPerStation->SetBinContent(1,100*effS1);
    EfficiencyPerStation->SetBinError(1,100*errS1);

    EfficiencyPerStation->GetXaxis()->SetBinLabel(4,"S4");

    EfficiencyPerStation->Write();

    TH1F * EfficiencyPerStationRing = new TH1F("EfficiencyPerStationRing","Efficiency per Station",8,0.5,8.5);

    float exgS3R2 = ExGregDm3R2->Integral()+ExGregD3R2->Integral(); 
    float obgS3R2 = OcGregDm3R2->Integral()+OcGregD3R2->Integral();
    float effS3R2 = obgS3R2/exgS3R2; float errS3R2 = sqrt(effS3R2*(1-effS3R2)/exgS3R2);
    EfficiencyPerStationRing->GetXaxis()->SetBinLabel(5,"S3R2");
    EfficiencyPerStationRing->SetBinContent(5,100*effS3R2);
    EfficiencyPerStationRing->SetBinError(5,100*errS3R2);

    float exgS3R3 = ExGregDm3R3->Integral()+ExGregD3R3->Integral(); 
    float obgS3R3 = OcGregDm3R3->Integral()+OcGregD3R3->Integral();
    float effS3R3 = obgS3R3/exgS3R3; float errS3R3 = sqrt(effS3R3*(1-effS3R3)/exgS3R3);
    EfficiencyPerStationRing->GetXaxis()->SetBinLabel(6,"S3R3");
    EfficiencyPerStationRing->SetBinContent(6,100*effS3R3);
    EfficiencyPerStationRing->SetBinError(6,100*errS3R3);

    float exgS2R2 = ExGregDm2R2->Integral()+ExGregD2R2->Integral(); 
    float obgS2R2 = OcGregDm2R2->Integral()+OcGregD2R2->Integral();
    float effS2R2 = obgS2R2/exgS2R2; float errS2R2 = sqrt(effS2R2*(1-effS2R2)/exgS2R2);
    EfficiencyPerStationRing->GetXaxis()->SetBinLabel(3,"S2R2");
    EfficiencyPerStationRing->SetBinContent(3,100*effS2R2);
    EfficiencyPerStationRing->SetBinError(3,100*errS2R2);

    float exgS2R3 = ExGregDm3R3->Integral()+ExGregD3R3->Integral(); 
    float obgS2R3 = OcGregDm3R3->Integral()+OcGregD3R3->Integral();
    float effS2R3 = obgS2R3/exgS2R3; float errS2R3 = sqrt(effS2R3*(1-effS2R3)/exgS2R3);
    EfficiencyPerStationRing->GetXaxis()->SetBinLabel(4,"S2R3");
    EfficiencyPerStationRing->SetBinContent(4,100*effS2R3);
    EfficiencyPerStationRing->SetBinError(4,100*errS2R3);

    float exgS1R2 = ExGregDm1R2->Integral()+ExGregD1R2->Integral(); 
    float obgS1R2 = OcGregDm1R2->Integral()+OcGregD1R2->Integral();
    float effS1R2 = obgS1R2/exgS1R2; float errS1R2 = sqrt(effS1R2*(1-effS1R2)/exgS1R2);
    EfficiencyPerStationRing->GetXaxis()->SetBinLabel(1,"S1R2");
    EfficiencyPerStationRing->SetBinContent(1,100*effS1R2);
    EfficiencyPerStationRing->SetBinError(1,100*errS1R2);

    float exgS1R3 = ExGregDm1R3->Integral()+ExGregD1R3->Integral(); 
    float obgS1R3 = OcGregDm1R3->Integral()+OcGregD1R3->Integral();
    float effS1R3 = obgS1R3/exgS1R3; float errS1R3 = sqrt(effS1R3*(1-effS1R3)/exgS1R3);
    EfficiencyPerStationRing->GetXaxis()->SetBinLabel(2,"S1R3");
    EfficiencyPerStationRing->SetBinContent(2,100*effS1R3);
    EfficiencyPerStationRing->SetBinError(2,100*errS1R3);

    EfficiencyPerStationRing->GetXaxis()->SetBinLabel(7,"S4R2");
    EfficiencyPerStationRing->GetXaxis()->SetBinLabel(8,"S4R3");

    EfficiencyPerStationRing->Write();


    float Exp[7];
    float Obs[7];
    float Eff[7];
    float Err[7];

    for(int k=1; k<=6; k++){
        Exp[k] = ExpLayerWm2->GetBinContent(k)+ExpLayerWm1->GetBinContent(k)+ExpLayerW0->GetBinContent(k)+ExpLayerW1->GetBinContent(k)+ExpLayerW2->GetBinContent(k);
        Obs[k] = ObsLayerWm2->GetBinContent(k)+ObsLayerWm1->GetBinContent(k)+ObsLayerW0->GetBinContent(k)+ObsLayerW1->GetBinContent(k)+ObsLayerW2->GetBinContent(k);
        if(Exp[k] != 0) {
          Eff[k] = Obs[k]/Exp[k];
          Err[k] = sqrt(Eff[k]*(1-Eff[k])/Exp[k]);
        }
	std::cout<<ExpLayerWm2->GetBinContent(k)<<" "<<ExpLayerWm1->GetBinContent(k)<<" "<<ExpLayerW0->GetBinContent(k)<<" "<<ExpLayerW1->GetBinContent(k)<<" "<<ExpLayerW2->GetBinContent(k)<<std::endl;
	std::cout<<Exp[k]<<" "<<Obs[k]<<" "<<Eff[k]<<std::endl;
    }
    
    EfficiencyPerLayer->GetXaxis()->SetBinLabel(1,"Layer 1");
    EfficiencyPerLayer->SetBinContent(1,100*Eff[1]);
    EfficiencyPerLayer->SetBinError(1,100*Err[1]);

    EfficiencyPerLayer->GetXaxis()->SetBinLabel(2,"Layer 2");
    EfficiencyPerLayer->SetBinContent(2,100*Eff[2]);
    EfficiencyPerLayer->SetBinError(2,100*Err[2]);

    EfficiencyPerLayer->GetXaxis()->SetBinLabel(3,"Layer 3");
    EfficiencyPerLayer->SetBinContent(3,100*Eff[3]);
    EfficiencyPerLayer->SetBinError(3,100*Err[3]);

    EfficiencyPerLayer->GetXaxis()->SetBinLabel(4,"Layer 4");
    EfficiencyPerLayer->SetBinContent(4,100*Eff[4]);
    EfficiencyPerLayer->SetBinError(4,100*Err[4]);

    EfficiencyPerLayer->GetXaxis()->SetBinLabel(5,"Layer 5");
    EfficiencyPerLayer->SetBinContent(5,100*Eff[5]);
    EfficiencyPerLayer->SetBinError(5,100*Err[5]);

    EfficiencyPerLayer->GetXaxis()->SetBinLabel(6,"Layer 6");
    EfficiencyPerLayer->SetBinContent(6,100*Eff[6]);
    EfficiencyPerLayer->SetBinError(6,100*Err[6]);

    EfficiencyPerLayer->SetMinimum(90);
    EfficiencyPerLayer->Write();

    Ca10->cd();
    EfficiencyPerLayer->Draw();
    Ca10->SaveAs("EfficiencyPerLayer.png");
    Ca10->Close();

    StatisticsPerLayer->GetXaxis()->SetBinLabel(1,"Layer 1");
    StatisticsPerLayer->SetBinContent(1,Exp[1]);

    StatisticsPerLayer->GetXaxis()->SetBinLabel(2,"Layer 2");
    StatisticsPerLayer->SetBinContent(2,Exp[2]);

    StatisticsPerLayer->GetXaxis()->SetBinLabel(3,"Layer 3");
    StatisticsPerLayer->SetBinContent(3,Exp[3]);

    StatisticsPerLayer->GetXaxis()->SetBinLabel(4,"Layer 4");
    StatisticsPerLayer->SetBinContent(4,Exp[4]);

    StatisticsPerLayer->GetXaxis()->SetBinLabel(5,"Layer 5");
    StatisticsPerLayer->SetBinContent(5,Exp[5]);

    StatisticsPerLayer->GetXaxis()->SetBinLabel(6,"Layer 6");
    StatisticsPerLayer->SetBinContent(6,Exp[6]);

    StatisticsPerLayer->Write();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  //Just one number for Negative EndCap

  float ExEndCapN = 
    ExGregDm3R3->Integral()+  
    ExGregDm2R3->Integral()+
    ExGregDm1R3->Integral()+
    ExGregDm3R2->Integral()+  
    ExGregDm2R2->Integral()+
    ExGregDm1R2->Integral();

  float OcEndCapN = 
    OcGregDm3R3->Integral()+  
    OcGregDm2R3->Integral()+
    OcGregDm1R3->Integral()+
    OcGregDm3R2->Integral()+  
    OcGregDm2R2->Integral()+
    OcGregDm1R2->Integral();

  float effEndCapN = 0;
  if(ExEndCapN!=0){
    effEndCapN = OcEndCapN/ExEndCapN; float errEndCapN = sqrt(effEndCapN*(1-effEndCapN)/ExEndCapN);
    RollYEff.precision(2);
    RollYEff<<"effEndCapN "<<effEndCapN<<" "<<errEndCapN<<std::endl;
  }

  //Just one number for Positive EndCap
  
  float ExEndCapP = 
    ExGregD3R3->Integral()+  
    ExGregD2R3->Integral()+
    ExGregD1R3->Integral()+
    ExGregD3R2->Integral()+  
    ExGregD2R2->Integral()+
    ExGregD1R2->Integral();

  float OcEndCapP = 
    OcGregD3R3->Integral()+  
    OcGregD2R3->Integral()+
    OcGregD1R3->Integral()+
    OcGregD3R2->Integral()+  
    OcGregD2R2->Integral()+
    OcGregD1R2->Integral();

  float effEndCapP = 0;
  if(ExEndCapP!=0){
    effEndCapP = OcEndCapP/ExEndCapP; float errEndCapP = sqrt(effEndCapP*(1-effEndCapP)/ExEndCapP);
    RollYEff<<"effEndCapP "<<effEndCapP<<" "<<errEndCapP<<std::endl;
  }
  
  std::cout<<"Doing Summary per disk"<<std::endl;
  
  TH1F * EfficiencyPerRing = new TH1F("EfficiencyPerRing","Efficiency per Ring in the whole endcap",12,0.5,12.5);
  TH1F * StatisticsPerRing = new TH1F("StatisticsPerRing","Statistics per Ring in the whole endcap",12,0.5,12.5);
  
  float exg = ExGregDm3R3->Integral(); float obg = OcGregDm3R3->Integral();
  eff = obg/exg; err = sqrt(eff*(1-eff)/exg);
  if(debug) std::cout<<"Ring -3 Ring 3 expected "<<exg<<" observed "<<obg<<" eff "<<eff<<"+/-"<<err<<std::endl;
  std::cout<<"Disk_-3_Ring_3 "<<eff<<" "<<err<<std::endl;
  
  EfficiencyPerRing->GetXaxis()->SetBinLabel(1,"RE-3/3");
  EfficiencyPerRing->SetBinContent(1,eff);
  EfficiencyPerRing->SetBinError(1,err);

  StatisticsPerRing->GetXaxis()->SetBinLabel(1,"RE-3/3");
  StatisticsPerRing->SetBinContent(1,exg);
  
  exg = ExGregDm3R2->Integral(); obg = OcGregDm3R2->Integral(); eff = obg/exg; err = sqrt(eff*(1-eff)/exg);
  std::cout<<"Disk -3 Ring 2 expected "<<exg<<" observed "<<obg<<" eff "<<eff<<"+/-"<<err<<std::endl;
  std::cout<<"Disk_-3_Ring_2 "<<eff<<" "<<err<<std::endl;
  EfficiencyPerRing->GetXaxis()->SetBinLabel(2,"RE-3/2");
  EfficiencyPerRing->SetBinContent(2,eff);
  EfficiencyPerRing->SetBinError(2,err);

  StatisticsPerRing->GetXaxis()->SetBinLabel(2,"RE-3/2");
  StatisticsPerRing->SetBinContent(2,exg);

  exg = ExGregDm2R3->Integral(); obg = OcGregDm2R3->Integral(); eff = obg/exg; err = sqrt(eff*(1-eff)/exg);
  std::cout<<"Disk -2 Ring 3 expected "<<exg<<" observed "<<obg<<" eff "<<eff<<"+/-"<<err<<std::endl;
  std::cout<<"Disk_-2_Ring_3 "<<eff<<" "<<err<<std::endl;
  EfficiencyPerRing->GetXaxis()->SetBinLabel(3,"RE-2/3");
  EfficiencyPerRing->SetBinContent(3,eff);
  EfficiencyPerRing->SetBinError(3,err);

  StatisticsPerRing->GetXaxis()->SetBinLabel(3,"RE-2/3");
  StatisticsPerRing->SetBinContent(3,exg);

  exg = ExGregDm2R2->Integral(); obg = OcGregDm2R2->Integral(); eff = obg/exg; err = sqrt(eff*(1-eff)/exg);
  std::cout<<"Disk -2 Ring 2 expected "<<exg<<" observed "<<obg<<" eff "<<eff<<"+/-"<<err<<std::endl;
  std::cout<<"Disk_-2_Ring_2 "<<eff<<" "<<err<<std::endl;
  EfficiencyPerRing->GetXaxis()->SetBinLabel(4,"RE-2/2");
  EfficiencyPerRing->SetBinContent(4,eff);
  EfficiencyPerRing->SetBinError(4,err);

  StatisticsPerRing->GetXaxis()->SetBinLabel(4,"RE-2/2");
  StatisticsPerRing->SetBinContent(4,exg);

  exg = ExGregDm1R3->Integral(); obg = OcGregDm1R3->Integral(); eff = obg/exg; err = sqrt(eff*(1-eff)/exg);
  std::cout<<"Disk -1 Ring 3 expected "<<exg<<" observed "<<obg<<" eff "<<eff<<"+/-"<<err<<std::endl;
  std::cout<<"Disk_-1_Ring_3 "<<eff<<" "<<err<<std::endl;
  EfficiencyPerRing->GetXaxis()->SetBinLabel(5,"RE-1/3");
  EfficiencyPerRing->SetBinContent(5,eff);
  EfficiencyPerRing->SetBinError(5,err);

  StatisticsPerRing->GetXaxis()->SetBinLabel(5,"RE-1/3");
  StatisticsPerRing->SetBinContent(5,exg);

  exg = ExGregDm1R2->Integral(); obg = OcGregDm1R2->Integral(); eff = obg/exg; err = sqrt(eff*(1-eff)/exg);
  std::cout<<"Disk -1 Ring 2 expected "<<exg<<" observed "<<obg<<" eff "<<eff<<"+/-"<<err<<std::endl;
  std::cout<<"Disk_-1_Ring_2 "<<eff<<" "<<err<<std::endl;
  EfficiencyPerRing->GetXaxis()->SetBinLabel(6,"RE-1/2");
  EfficiencyPerRing->SetBinContent(6,eff);
  EfficiencyPerRing->SetBinError(6,err);

  StatisticsPerRing->GetXaxis()->SetBinLabel(6,"RE-1/2");
  StatisticsPerRing->SetBinContent(6,exg);
    
  exg = ExGregD1R3->Integral(); obg = OcGregD1R3->Integral(); eff = obg/exg; err = sqrt(eff*(1-eff)/exg);
  std::cout<<"Disk 1 Ring 3 expected "<<exg<<" observed "<<obg<<" eff "<<eff<<"+/-"<<err<<std::endl;
  std::cout<<"Disk_1_Ring_3 "<<eff<<" "<<err<<std::endl;
  EfficiencyPerRing->GetXaxis()->SetBinLabel(7,"RE+1/3");
  EfficiencyPerRing->SetBinContent(7,eff);
  EfficiencyPerRing->SetBinError(7,err);

  StatisticsPerRing->GetXaxis()->SetBinLabel(7,"RE+1/3");
  StatisticsPerRing->SetBinContent(7,exg);

  exg = ExGregD1R2->Integral(); obg = OcGregD1R2->Integral(); eff = obg/exg; err = sqrt(eff*(1-eff)/exg);
  std::cout<<"Disk 1 Ring 2 expected "<<exg<<" observed "<<obg<<" eff "<<eff<<"+/-"<<err<<std::endl;
  std::cout<<"Disk_1_Ring_2 "<<eff<<" "<<err<<std::endl;
  EfficiencyPerRing->GetXaxis()->SetBinLabel(8,"RE+1/2");
  EfficiencyPerRing->SetBinContent(8,eff);
  EfficiencyPerRing->SetBinError(8,err);

  StatisticsPerRing->GetXaxis()->SetBinLabel(8,"RE+1/2");
  StatisticsPerRing->SetBinContent(8,exg);
  
  exg = ExGregD2R3->Integral(); obg = OcGregD2R3->Integral(); eff = obg/exg; err = sqrt(eff*(1-eff)/exg);
  std::cout<<"Disk 2 Ring 3 expected "<<exg<<" observed "<<obg<<" eff "<<eff<<"+/-"<<err<<std::endl;
  std::cout<<"Disk_2_Ring_3 "<<eff<<" "<<err<<std::endl;
  EfficiencyPerRing->GetXaxis()->SetBinLabel(9,"RE+2/3");
  EfficiencyPerRing->SetBinContent(9,eff);
  EfficiencyPerRing->SetBinError(9,err);

  StatisticsPerRing->GetXaxis()->SetBinLabel(9,"RE+2/3");
  StatisticsPerRing->SetBinContent(9,exg);

  exg = ExGregD2R2->Integral(); obg = OcGregD2R2->Integral(); eff = obg/exg; err = sqrt(eff*(1-eff)/exg);
  std::cout<<"Disk 2 Ring 2 expected "<<exg<<" observed "<<obg<<" eff "<<eff<<"+/-"<<err<<std::endl;
  std::cout<<"Disk_2_Ring_2 "<<eff<<" "<<err<<std::endl;
  EfficiencyPerRing->GetXaxis()->SetBinLabel(10,"RE+2/2");
  EfficiencyPerRing->SetBinContent(10,eff);
  EfficiencyPerRing->SetBinError(10,err);

  StatisticsPerRing->GetXaxis()->SetBinLabel(10,"RE+2/2");
  StatisticsPerRing->SetBinContent(10,exg);

  exg = ExGregD3R3->Integral(); obg = OcGregD3R3->Integral(); eff = obg/exg; err = sqrt(eff*(1-eff)/exg);
  std::cout<<"Disk 3 Ring 3 expected "<<exg<<" observed "<<obg<<" eff "<<eff<<"+/-"<<err<<std::endl;
  std::cout<<"Disk_3_Ring_3 "<<eff<<" "<<err<<std::endl;
  EfficiencyPerRing->GetXaxis()->SetBinLabel(11,"RE+3/3");
  EfficiencyPerRing->SetBinContent(11,eff);
  EfficiencyPerRing->SetBinError(11,err);

  StatisticsPerRing->GetXaxis()->SetBinLabel(11,"RE+3/3");
  StatisticsPerRing->SetBinContent(11,exg);

  exg = ExGregD3R2->Integral(); obg = OcGregD3R2->Integral(); eff = obg/exg; err = sqrt(eff*(1-eff)/exg);
  std::cout<<"Disk 3 Ring 2 expected "<<exg<<" observed "<<obg<<" eff "<<eff<<"+/-"<<err<<std::endl;
  std::cout<<"Disk_3_Ring_2 "<<eff<<" "<<err<<std::endl;
  EfficiencyPerRing->GetXaxis()->SetBinLabel(12,"RE+3/2");
  EfficiencyPerRing->SetBinContent(12,eff);
  EfficiencyPerRing->SetBinError(12,err);

  StatisticsPerRing->GetXaxis()->SetBinLabel(12,"RE+3/2");
  StatisticsPerRing->SetBinContent(12,exg);
  
  
  for(k=1;k<=12;k++){
    err=0; eff=0; N=ExsectorEffWm2->GetBinContent(k);
    if(N!=0.){ eff = OcsectorEffWm2->GetBinContent(k)/N;err=sqrt(eff*(1-eff)/N);}
    sectorEffWm2->SetBinContent(k,eff); sectorEffWm2->SetBinError(k,err);
    
    err=0; eff=0; N=ExsectorEffWm1->GetBinContent(k);
    if(N!=0.){ eff = OcsectorEffWm1->GetBinContent(k)/N;err=sqrt(eff*(1-eff)/N);}
    sectorEffWm1->SetBinContent(k,eff); sectorEffWm1->SetBinError(k,err);

    err=0; eff=0; N=ExsectorEffW0->GetBinContent(k);
    if(N!=0.){ eff = OcsectorEffW0->GetBinContent(k)/N;err=sqrt(eff*(1-eff)/N);}
    sectorEffW0->SetBinContent(k,eff); sectorEffW0->SetBinError(k,err);

    err=0; eff=0; N=ExsectorEffW1->GetBinContent(k);
    if(N!=0.){ eff = OcsectorEffW1->GetBinContent(k)/N;err=sqrt(eff*(1-eff)/N);}
    sectorEffW1->SetBinContent(k,eff); sectorEffW1->SetBinError(k,err);

    err=0; eff=0; N=ExsectorEffW2->GetBinContent(k);
    if(N!=0.){ eff = OcsectorEffW2->GetBinContent(k)/N;err=sqrt(eff*(1-eff)/N);}
    sectorEffW2->SetBinContent(k,eff); sectorEffW2->SetBinError(k,err);
  }

  float effWm2 = 0;
  float effWm1 = 0;
  float effW0 = 0;
  float effW1 = 0;
  float effW2 = 0;

  float errWm2 = 0;
  float errWm1 = 0;
  float errW0 = 0;
  float errW1 = 0;
  float errW2 = 0;

  if(ExsectorEffWm2->Integral()!=0) { effWm2= OcsectorEffWm2->Integral()/ExsectorEffWm2->Integral(); errWm2 = sqrt(effWm2*(1-effWm2)/ExsectorEffWm2->Integral());RollYEff<<" effwheelm2 "<<effWm2<<" "<<errWm2<<std::endl;}
  if(ExsectorEffWm1->Integral()!=0) { effWm1= OcsectorEffWm1->Integral()/ExsectorEffWm1->Integral(); errWm1 = sqrt(effWm1*(1-effWm1)/ExsectorEffWm1->Integral());RollYEff<<" effwheelm1 "<<effWm1<<" "<<errWm1<<std::endl;}
  if(ExsectorEffW0->Integral()!=0) {effW0  = OcsectorEffW0->Integral()/ExsectorEffW0->Integral();    errW0 = sqrt(effW0*(1-effW0)/ExsectorEffW0->Integral());    RollYEff<<" effwheel0 "<<effW0<<" "<<errW0<<std::endl;   }
  if(ExsectorEffW1->Integral()!=0) {effW1  = OcsectorEffW1->Integral()/ExsectorEffW1->Integral();    errW1 = sqrt(effW1*(1-effW1)/ExsectorEffW1->Integral());    RollYEff<<" effwheel1 "<<effW1<<" "<<errW1<<std::endl;   }
  if(ExsectorEffW2->Integral()!=0) {  effW2  = OcsectorEffW2->Integral()/ExsectorEffW2->Integral();  errW2 = sqrt(effW2*(1-effW2)/ExsectorEffW2->Integral());    RollYEff<<" effwheel2 "<<effW2<<" "<<errW2<<std::endl;   }
  
  float ExBarrel = 
    ExsectorEffWm2->Integral()+  
    ExsectorEffWm1->Integral()+
    ExsectorEffW0->Integral()+
    ExsectorEffW1->Integral()+
    ExsectorEffW2->Integral();

  float OcBarrel = 
    OcsectorEffWm2->Integral()+  
    OcsectorEffWm1->Integral()+
    OcsectorEffW0->Integral()+
    OcsectorEffW1->Integral()+
    OcsectorEffW2->Integral();

  float effBarrel = 0;
  if(ExBarrel!=0){
    effBarrel = OcBarrel/ExBarrel; float errBarrel = sqrt(effBarrel*(1-effBarrel)/ExBarrel);
    RollYEff.precision(2);
    RollYEff<<"effBarrel "<<effBarrel<<" "<<errBarrel<<std::endl;
  }
  
  for(int sector = 1;sector <=12 ; sector++){
    CLSWm2->SetBinContent(sector,sectorCLSWm2[sector]->GetMean());
    if(sectorCLSWm2[sector]->GetEntries()!=0) CLSWm2->SetBinError(sector,sectorCLSWm2[sector]->GetRMS()/sectorCLSWm2[sector]->GetEntries());

    CLSWm1->SetBinContent(sector,sectorCLSWm1[sector]->GetMean());
    if(sectorCLSWm1[sector]->GetEntries()!=0) CLSWm1->SetBinError(sector,sectorCLSWm1[sector]->GetRMS()/sectorCLSWm1[sector]->GetEntries());

    CLSW0->SetBinContent(sector,sectorCLSW0[sector]->GetMean());
    if(sectorCLSW0[sector]->GetEntries()!=0) CLSW0->SetBinError(sector,sectorCLSW0[sector]->GetRMS()/sectorCLSW0[sector]->GetEntries());

    CLSW1->SetBinContent(sector,sectorCLSW1[sector]->GetMean());
    if(sectorCLSW1[sector]->GetEntries()!=0) CLSW1->SetBinError(sector,sectorCLSW1[sector]->GetRMS()/sectorCLSW1[sector]->GetEntries());

    CLSW2->SetBinContent(sector,sectorCLSW2[sector]->GetMean());
    if(sectorCLSW2[sector]->GetEntries()!=0) CLSW2->SetBinError(sector,sectorCLSW2[sector]->GetRMS()/sectorCLSW2[sector]->GetEntries());
  }
  
  Ca5->Clear();
  
  bxbarrel->Draw();
  bxbarrel->GetYaxis()->SetTitle("RMS (bx Units)");
  bxbarrel->GetXaxis()->SetTitle("Mean (bx Units)");
  Ca5->SaveAs("bxbarrel.png");
  bxbarrel->Write();
  Ca5->Clear();

  signal_bxbarrel->Draw();
  signal_bxbarrel->GetYaxis()->SetTitle("RMS (bx Units)");
  signal_bxbarrel->GetXaxis()->SetTitle("Mean (bx Units)");
  Ca5->SaveAs("signal_bxbarrel.png");
  signal_bxbarrel->Write();
  Ca5->Clear();

  bxendcap->Draw();
  bxendcap->GetYaxis()->SetTitle("RMS (bx Units)");
  bxendcap->GetXaxis()->SetTitle("Mean (bx Units)");
  Ca5->SaveAs("bxendcap.png");
  bxendcap->Write();
  Ca5->Clear();
    
  signal_bxendcap->Draw();
  signal_bxendcap->GetYaxis()->SetTitle("RMS (bx Units)");
  signal_bxendcap->GetXaxis()->SetTitle("Mean (bx Units)");
  Ca5->SaveAs("signal_bxendcap.png");
  signal_bxendcap->Write();
  Ca5->Clear();
  
  if(barrel){
    EffGlobWm2->GetXaxis()->LabelsOption("v");
    if(debug) std::cout<<"Done the first Barrel"<<std::endl;
    EffGlobWm1->GetXaxis()->LabelsOption("v");
    EffGlobW0->GetXaxis()->LabelsOption("v");
    EffGlobW1->GetXaxis()->LabelsOption("v");
    EffGlobW2->GetXaxis()->LabelsOption("v");

    EffGlobWm2->GetXaxis()->SetLabelSize(0.03);
    EffGlobWm1->GetXaxis()->SetLabelSize(0.03);
    EffGlobW0->GetXaxis()->SetLabelSize(0.03);
    EffGlobW1->GetXaxis()->SetLabelSize(0.03);
    EffGlobW2->GetXaxis()->SetLabelSize(0.03);

    EffGlobWm2->GetYaxis()->SetRangeUser(0.,100.);
    EffGlobWm1->GetYaxis()->SetRangeUser(0.,100.);
    EffGlobW0->GetYaxis()->SetRangeUser(0.,100.);
    EffGlobW1->GetYaxis()->SetRangeUser(0.,100.);
    EffGlobW2->GetYaxis()->SetRangeUser(0.,100.);
  
    BXGlobWm2->GetXaxis()->LabelsOption("v");
    BXGlobWm1->GetXaxis()->LabelsOption("v");
    BXGlobW0->GetXaxis()->LabelsOption("v");
    BXGlobW1->GetXaxis()->LabelsOption("v");
    BXGlobW2->GetXaxis()->LabelsOption("v");

    MaskedGlobWm2->GetXaxis()->LabelsOption("v");
    MaskedGlobWm1->GetXaxis()->LabelsOption("v");
    MaskedGlobW0->GetXaxis()->LabelsOption("v");
    MaskedGlobW1->GetXaxis()->LabelsOption("v");
    MaskedGlobW2->GetXaxis()->LabelsOption("v");

    EffGlobWm2far->GetXaxis()->LabelsOption("v");
    EffGlobWm1far->GetXaxis()->LabelsOption("v");
    EffGlobW0far->GetXaxis()->LabelsOption("v");
    EffGlobW1far->GetXaxis()->LabelsOption("v");
    EffGlobW2far->GetXaxis()->LabelsOption("v");

    EffGlobWm2far->GetXaxis()->SetLabelSize(0.03);
    EffGlobWm1far->GetXaxis()->SetLabelSize(0.03);
    EffGlobW0far->GetXaxis()->SetLabelSize(0.03);
    EffGlobW1far->GetXaxis()->SetLabelSize(0.03);
    EffGlobW2far->GetXaxis()->SetLabelSize(0.03);

    EffGlobWm2far->GetYaxis()->SetRangeUser(0.,100.);
    EffGlobWm1far->GetYaxis()->SetRangeUser(0.,100.);
    EffGlobW0far->GetYaxis()->SetRangeUser(0.,100.);
    EffGlobW1far->GetYaxis()->SetRangeUser(0.,100.);
    EffGlobW2far->GetYaxis()->SetRangeUser(0.,100.);
  
    if(debug) std::cout<<"Done with Eff Glob"<<std::endl;

    BXGlobWm2far->GetXaxis()->LabelsOption("v");
    BXGlobWm1far->GetXaxis()->LabelsOption("v");
    BXGlobW0far->GetXaxis()->LabelsOption("v");
    BXGlobW1far->GetXaxis()->LabelsOption("v");
    BXGlobW2far->GetXaxis()->LabelsOption("v");

    MaskedGlobWm2far->GetXaxis()->LabelsOption("v");
    MaskedGlobWm1far->GetXaxis()->LabelsOption("v");
    MaskedGlobW0far->GetXaxis()->LabelsOption("v");
    MaskedGlobW1far->GetXaxis()->LabelsOption("v");
    MaskedGlobW2far->GetXaxis()->LabelsOption("v");

    if(debug) std::cout<<"Done with Average Masked"<<std::endl;

    AverageEffWm2->GetXaxis()->LabelsOption("v");
    AverageEffWm1->GetXaxis()->LabelsOption("v");
    AverageEffW0->GetXaxis()->LabelsOption("v");
    AverageEffW1->GetXaxis()->LabelsOption("v");
    AverageEffW2->GetXaxis()->LabelsOption("v");

    AverageEffWm2far->GetXaxis()->LabelsOption("v");
    AverageEffWm1far->GetXaxis()->LabelsOption("v");
    AverageEffW0far->GetXaxis()->LabelsOption("v");
    AverageEffW1far->GetXaxis()->LabelsOption("v");
    AverageEffW2far->GetXaxis()->LabelsOption("v");
    
    NoPredictionWm2->GetXaxis()->LabelsOption("v");
    NoPredictionWm1->GetXaxis()->LabelsOption("v");
    NoPredictionW0->GetXaxis()->LabelsOption("v");
    NoPredictionW1->GetXaxis()->LabelsOption("v");
    NoPredictionW2->GetXaxis()->LabelsOption("v");
    
    NoPredictionWm2far->GetXaxis()->LabelsOption("v"); if(debug) std::cout<<"Done with Wm2fa  "<<std::endl;
    NoPredictionWm1far->GetXaxis()->LabelsOption("v"); if(debug) std::cout<<"Done with Wm1fa  "<<std::endl;
    NoPredictionW0far->GetXaxis()->LabelsOption("v");  if(debug) std::cout<<"Done with W0far  "<<std::endl;
    NoPredictionW1far->GetXaxis()->LabelsOption("v");  if(debug) std::cout<<"Done with W1far  "<<std::endl;
    NoPredictionW2far->GetXaxis()->LabelsOption("v");  if(debug) std::cout<<"Done with W2far  "<<std::endl;
    
  }if(endcap){  
    if(debug) std::cout<<"Label Options Near"<<std::endl;
    NoPredictionDm3->GetXaxis()->LabelsOption("v");
    if(debug) std::cout<<"Label Options AverageEffDm3"<<std::endl;
    AverageEffDm3->GetXaxis()->LabelsOption("v");
    if(debug) std::cout<<"Label Options EffGlobDm3"<<std::endl;
    EffGlobDm3->GetXaxis()->LabelsOption("v");
    BXGlobDm3->GetXaxis()->LabelsOption("v");
    MaskedGlobDm3->GetXaxis()->LabelsOption("v");
          
    if(debug) std::cout<<"Label Options Far"<<std::endl;
    NoPredictionDm3far->GetXaxis()->LabelsOption("v");
    AverageEffDm3far->GetXaxis()->LabelsOption("v");
    EffGlobDm3far->GetXaxis()->LabelsOption("v");
    BXGlobDm3far->GetXaxis()->LabelsOption("v");
    MaskedGlobDm3far->GetXaxis()->LabelsOption("v");
    
    if(debug) std::cout<<"Label Size"<<std::endl;
    NoPredictionDm3->GetXaxis()->SetLabelSize(0.03);
    AverageEffDm3->GetXaxis()->SetLabelSize(0.03);
    EffGlobDm3->GetXaxis()->SetLabelSize(0.03);
    BXGlobDm3->GetXaxis()->SetLabelSize(0.03);
    MaskedGlobDm3->GetXaxis()->SetLabelSize(0.03);
    NoPredictionDm3far->GetXaxis()->SetLabelSize(0.03);
    AverageEffDm3far->GetXaxis()->SetLabelSize(0.03);
    EffGlobDm3far->GetXaxis()->SetLabelSize(0.03);
    BXGlobDm3far->GetXaxis()->SetLabelSize(0.03);
    MaskedGlobDm3far->GetXaxis()->SetLabelSize(0.03);
    
    if(debug) std::cout<<"Range User"<<std::endl;
    NoPredictionDm3->GetYaxis()->SetRangeUser(0.,100.);
    AverageEffDm3->GetYaxis()->SetRangeUser(0.,100.);
    EffGlobDm3->GetYaxis()->SetRangeUser(0.,100.);
    MaskedGlobDm3->GetYaxis()->SetRangeUser(0.,100.);
    NoPredictionDm3far->GetYaxis()->SetRangeUser(0.,100.);
    AverageEffDm3far->GetYaxis()->SetRangeUser(0.,100.);
    EffGlobDm3far->GetYaxis()->SetRangeUser(0.,100.);
    MaskedGlobDm3far->GetYaxis()->SetRangeUser(0.,100.);
    NoPredictionDm2->GetYaxis()->SetRangeUser(0.,100.);
    MaskedGlobDm3far->GetYaxis()->SetRangeUser(0.,100.);

    if(debug) std::cout<<"Done with Disk m 3"<<std::endl;

    NoPredictionDm2->GetXaxis()->LabelsOption("v");
    AverageEffDm2->GetXaxis()->LabelsOption("v");
    EffGlobDm2->GetXaxis()->LabelsOption("v");
    BXGlobDm2->GetXaxis()->LabelsOption("v");
    MaskedGlobDm2->GetXaxis()->LabelsOption("v");
    NoPredictionDm2far->GetXaxis()->LabelsOption("v");
    AverageEffDm2far->GetXaxis()->LabelsOption("v");
    EffGlobDm2far->GetXaxis()->LabelsOption("v");
    BXGlobDm2far->GetXaxis()->LabelsOption("v");
    MaskedGlobDm2far->GetXaxis()->LabelsOption("v");

    NoPredictionDm2->GetXaxis()->SetLabelSize(0.03);
    AverageEffDm2->GetXaxis()->SetLabelSize(0.03);
    EffGlobDm2->GetXaxis()->SetLabelSize(0.03);
    BXGlobDm2->GetXaxis()->SetLabelSize(0.03);
    MaskedGlobDm2->GetXaxis()->SetLabelSize(0.03);
    NoPredictionDm2far->GetXaxis()->SetLabelSize(0.03);
    AverageEffDm2far->GetXaxis()->SetLabelSize(0.03);
    EffGlobDm2far->GetXaxis()->SetLabelSize(0.03);
    BXGlobDm2far->GetXaxis()->SetLabelSize(0.03);
    MaskedGlobDm2far->GetXaxis()->SetLabelSize(0.03);

    NoPredictionDm2->GetYaxis()->SetRangeUser(0.,100.);
    AverageEffDm2->GetYaxis()->SetRangeUser(0.,100.);
    EffGlobDm2->GetYaxis()->SetRangeUser(0.,100.);
    MaskedGlobDm2->GetYaxis()->SetRangeUser(0.,100.);
    NoPredictionDm2far->GetYaxis()->SetRangeUser(0.,100.);
    AverageEffDm2far->GetYaxis()->SetRangeUser(0.,100.);
    EffGlobDm2far->GetYaxis()->SetRangeUser(0.,100.);
    MaskedGlobDm2far->GetYaxis()->SetRangeUser(0.,100.);

    if(debug) std::cout<<"Done with Disk m 2"<<std::endl;

    NoPredictionDm1->GetXaxis()->LabelsOption("v");
    AverageEffDm1->GetXaxis()->LabelsOption("v");
    EffGlobDm1->GetXaxis()->LabelsOption("v");
    BXGlobDm1->GetXaxis()->LabelsOption("v");
    MaskedGlobDm1->GetXaxis()->LabelsOption("v");
    NoPredictionDm1far->GetXaxis()->LabelsOption("v");
    AverageEffDm1far->GetXaxis()->LabelsOption("v");
    EffGlobDm1far->GetXaxis()->LabelsOption("v");
    BXGlobDm1far->GetXaxis()->LabelsOption("v");
    MaskedGlobDm1far->GetXaxis()->LabelsOption("v");
    NoPredictionD1->GetXaxis()->LabelsOption("v");

    NoPredictionDm1->GetXaxis()->SetLabelSize(0.03);
    AverageEffDm1->GetXaxis()->SetLabelSize(0.03);
    EffGlobDm1->GetXaxis()->SetLabelSize(0.03);
    BXGlobDm1->GetXaxis()->SetLabelSize(0.03);
    MaskedGlobDm1->GetXaxis()->SetLabelSize(0.03);
    NoPredictionDm1far->GetXaxis()->SetLabelSize(0.03);
    AverageEffDm1far->GetXaxis()->SetLabelSize(0.03);
    EffGlobDm1far->GetXaxis()->SetLabelSize(0.03);
    BXGlobDm1far->GetXaxis()->SetLabelSize(0.03);
    MaskedGlobDm1far->GetXaxis()->SetLabelSize(0.03);

    NoPredictionDm1->GetYaxis()->SetRangeUser(0.,100.);
    AverageEffDm1->GetYaxis()->SetRangeUser(0.,100.);
    EffGlobDm1->GetYaxis()->SetRangeUser(0.,100.);
    MaskedGlobDm1->GetYaxis()->SetRangeUser(0.,100.);
    NoPredictionDm1far->GetYaxis()->SetRangeUser(0.,100.);
    AverageEffDm1far->GetYaxis()->SetRangeUser(0.,100.);
    EffGlobDm1far->GetYaxis()->SetRangeUser(0.,100.);
    MaskedGlobDm1far->GetYaxis()->SetRangeUser(0.,100.);

    if(debug) std::cout<<"Done with Disk m1"<<std::endl;

    AverageEffD1->GetXaxis()->LabelsOption("v");
    EffGlobD1->GetXaxis()->LabelsOption("v");
    BXGlobD1->GetXaxis()->LabelsOption("v");
    MaskedGlobD1->GetXaxis()->LabelsOption("v");
    NoPredictionD1far->GetXaxis()->LabelsOption("v");
    AverageEffD1far->GetXaxis()->LabelsOption("v");
    EffGlobD1far->GetXaxis()->LabelsOption("v");
    BXGlobD1far->GetXaxis()->LabelsOption("v");
    MaskedGlobD1far->GetXaxis()->LabelsOption("v");

    NoPredictionD1->GetXaxis()->SetLabelSize(0.03);
    AverageEffD1->GetXaxis()->SetLabelSize(0.03);
    EffGlobD1->GetXaxis()->SetLabelSize(0.03);
    BXGlobD1->GetXaxis()->SetLabelSize(0.03);
    MaskedGlobD1->GetXaxis()->SetLabelSize(0.03);
    NoPredictionD1far->GetXaxis()->SetLabelSize(0.03);
    AverageEffD1far->GetXaxis()->SetLabelSize(0.03);
    EffGlobD1far->GetXaxis()->SetLabelSize(0.03);
    BXGlobD1far->GetXaxis()->SetLabelSize(0.03);
    MaskedGlobD1far->GetXaxis()->SetLabelSize(0.03);

    NoPredictionD1->GetYaxis()->SetRangeUser(0.,100.);
    AverageEffD1->GetYaxis()->SetRangeUser(0.,100.);
    EffGlobD1->GetYaxis()->SetRangeUser(0.,100.);
    MaskedGlobD1->GetYaxis()->SetRangeUser(0.,100.);
    NoPredictionD1far->GetYaxis()->SetRangeUser(0.,100.);
    AverageEffD1far->GetYaxis()->SetRangeUser(0.,100.);
    EffGlobD1far->GetYaxis()->SetRangeUser(0.,100.);
    MaskedGlobD1far->GetYaxis()->SetRangeUser(0.,100.);

    if(debug) std::cout<<"Done with Disk 1"<<std::endl;

    NoPredictionD2->GetXaxis()->LabelsOption("v");
    AverageEffD2->GetXaxis()->LabelsOption("v");
    EffGlobD2->GetXaxis()->LabelsOption("v");
    BXGlobD2->GetXaxis()->LabelsOption("v");
    MaskedGlobD2->GetXaxis()->LabelsOption("v");
    NoPredictionD2far->GetXaxis()->LabelsOption("v");
    AverageEffD2far->GetXaxis()->LabelsOption("v");
    EffGlobD2far->GetXaxis()->LabelsOption("v");
    BXGlobD2far->GetXaxis()->LabelsOption("v");
    MaskedGlobD2far->GetXaxis()->LabelsOption("v");

    NoPredictionD2->GetXaxis()->SetLabelSize(0.03);
    AverageEffD2->GetXaxis()->SetLabelSize(0.03);
    EffGlobD2->GetXaxis()->SetLabelSize(0.03);
    BXGlobD2->GetXaxis()->SetLabelSize(0.03);
    MaskedGlobD2->GetXaxis()->SetLabelSize(0.03);
    NoPredictionD2far->GetXaxis()->SetLabelSize(0.03);
    AverageEffD2far->GetXaxis()->SetLabelSize(0.03);
    EffGlobD2far->GetXaxis()->SetLabelSize(0.03);
    BXGlobD2far->GetXaxis()->SetLabelSize(0.03);
    MaskedGlobD2far->GetXaxis()->SetLabelSize(0.03);

    NoPredictionD2->GetYaxis()->SetRangeUser(0.,100.);
    AverageEffD2->GetYaxis()->SetRangeUser(0.,100.);
    EffGlobD2->GetYaxis()->SetRangeUser(0.,100.);
    MaskedGlobD2->GetYaxis()->SetRangeUser(0.,100.);
    NoPredictionD2far->GetYaxis()->SetRangeUser(0.,100.);
    AverageEffD2far->GetYaxis()->SetRangeUser(0.,100.);
    EffGlobD2far->GetYaxis()->SetRangeUser(0.,100.);
    MaskedGlobD2far->GetYaxis()->SetRangeUser(0.,100.);

    if(debug) std::cout<<"Done with Disk 2"<<std::endl;

    NoPredictionD3->GetXaxis()->LabelsOption("v");
    AverageEffD3->GetXaxis()->LabelsOption("v");
    EffGlobD3->GetXaxis()->LabelsOption("v");
    BXGlobD3->GetXaxis()->LabelsOption("v");
    MaskedGlobD3->GetXaxis()->LabelsOption("v");
    NoPredictionD3far->GetXaxis()->LabelsOption("v");
    AverageEffD3far->GetXaxis()->LabelsOption("v");
    EffGlobD3far->GetXaxis()->LabelsOption("v");
    BXGlobD3far->GetXaxis()->LabelsOption("v");
    MaskedGlobD3far->GetXaxis()->LabelsOption("v");
  
    NoPredictionD3->GetXaxis()->SetLabelSize(0.03);
    AverageEffD3->GetXaxis()->SetLabelSize(0.03);
    EffGlobD3->GetXaxis()->SetLabelSize(0.03);
    BXGlobD3->GetXaxis()->SetLabelSize(0.03);
    MaskedGlobD3->GetXaxis()->SetLabelSize(0.03);
    NoPredictionD3far->GetXaxis()->SetLabelSize(0.03);
    AverageEffD3far->GetXaxis()->SetLabelSize(0.03);
    EffGlobD3far->GetXaxis()->SetLabelSize(0.03);
    BXGlobD3far->GetXaxis()->SetLabelSize(0.03);
    MaskedGlobD3far->GetXaxis()->SetLabelSize(0.03);

    NoPredictionD3->GetYaxis()->SetRangeUser(0.,100.);
    AverageEffD3->GetYaxis()->SetRangeUser(0.,100.);
    EffGlobD3->GetYaxis()->SetRangeUser(0.,100.);
    MaskedGlobD3->GetYaxis()->SetRangeUser(0.,100.);
    NoPredictionD3far->GetYaxis()->SetRangeUser(0.,100.);
    AverageEffD3far->GetYaxis()->SetRangeUser(0.,100.);
    EffGlobD3far->GetYaxis()->SetRangeUser(0.,100.);
    MaskedGlobD3far->GetYaxis()->SetRangeUser(0.,100.);

    residualRB1in->Write();
    residualRB1out->Write();
    residualRB2in->Write();
    residualRB2out->Write();
    residualRB3->Write();
    residualRB4->Write();

    residualStation1Ring2_A->Write();
    residualStation1Ring2_B->Write();    
    residualStation1Ring2_C->Write();
    residualStation1Ring3_A->Write();
    residualStation1Ring3_B->Write();
    residualStation1Ring3_C->Write();
    
    residualStation23Ring2_A->Write();
    residualStation23Ring2_B->Write();
    residualStation23Ring2_C->Write();
    residualStation23Ring3_A->Write();
    residualStation23Ring3_B->Write();
    residualStation23Ring3_C->Write();

    residualStation123Ring3_A->Add(residualStation23Ring3_A);
    residualStation123Ring3_A->Add(residualStation1Ring3_A);
    
    residualStation123Ring3_B->Add(residualStation23Ring3_B);
    residualStation123Ring3_B->Add(residualStation1Ring3_B);

    residualStation123Ring3_C->Add(residualStation23Ring3_C);
    residualStation123Ring3_C->Add(residualStation1Ring3_C);
    
    residualStation123Ring3_A->Write();
    residualStation123Ring3_B->Write();
    residualStation123Ring3_C->Write();

    residualDiskm1Ring2->Write();
    residualDiskm1Ring3->Write();
    residualDiskm2Ring2->Write();
    residualDiskm2Ring3->Write();
    residualDiskm3Ring2->Write();
    residualDiskm3Ring3->Write();
  
    residualDisk1Ring2->Write(); 
    residualDisk1Ring3->Write(); 
    residualDisk2Ring2->Write(); 
    residualDisk2Ring3->Write(); 
    residualDisk3Ring2->Write(); 
    residualDisk3Ring3->Write(); 
    
  }


  if(debug) std::cout<<"Efficiency Images"<<std::endl;

  pave = new TPaveText(35,119,60,102);
  TText *t1=pave->AddText("BX Distribution (Right Axis ->)");
  t1->SetTextColor(9);
  TText *t2=pave->AddText("Average Efficiency (%)");
  t2->SetTextColor(8);
  TText *t3=pave->AddText("Integral Efficiency (%)");
  //black
  TText *t4=pave->AddText("Strips without Data (ratio)"); 
  t4->SetTextColor(2);
  TText *t5=pave->AddText("Strips Never Pointed for a Segment (ratio)");
  t5->SetTextColor(5);

  pave->SetFillColor(18);
  
  t1->SetTextSize(0.019);
  t2->SetTextSize(0.019);
  t3->SetTextSize(0.019);
  t4->SetTextSize(0.019);
  t5->SetTextSize(0.019);


  Ca2->SetBottomMargin(0.4);
  
  TGaxis * bxAxis = new TGaxis(104.,0.,104.,100.,-5,5,11,"+L");
  TGaxis * bxAxisFar = new TGaxis(108.,0.,108.,100.,-5,5,11,"+L");
  TGaxis * bxAxisEndCap = new TGaxis(112.,0.,112.,100.,-5,5,11,"+L");
  
  bxAxis->SetLabelColor(9);
  bxAxis->SetName("bxAxis");
  bxAxis->SetTitle("Mean BX (bx Units)");
  bxAxis->SetTitleColor(9);
  bxAxis->CenterTitle();
 
  bxAxisFar->SetLabelColor(9);
  bxAxisFar->SetName("bxAxis");
  bxAxisFar->SetTitle("Mean BX (bx Units)");
  bxAxisFar->SetTitleColor(9);
  bxAxisFar->CenterTitle();
 
  bxAxisEndCap->SetLabelColor(9);
  bxAxisEndCap->SetName("bxAxis");
  bxAxisEndCap->SetTitle("Mean BX (bx Units)");
  bxAxisEndCap->SetTitleColor(9);
  bxAxisEndCap->CenterTitle();
  
  gStyle->SetOptStat(stat);
  gStyle->SetPalette(1);
  
  //Negative EndCap
  
  command = "mkdir Sides" ; system(command.c_str());
  command = "mkdir Distro" ; system(command.c_str());
  command = "mkdir Pigi" ; system(command.c_str());
  command = "mkdir Greg" ; system(command.c_str());
  command = "mkdir CLS" ; system(command.c_str());
  
  //Producing Images

  const int n=50;

  float step = 180/float(n);

  float x[n];
  float y[n];
  float ex[n];
  float ey[n];
  
  if(debug) std::cout<<"Producing Scatter Plots"<<std::endl;

  Ca5->Clear();

  ScatterPlotAlphaCLSLa1= (TH2F*)theFile->Get("DQMData/Muons/MuonSegEff/Residuals/Investigation/ScatterPlotAlphaCLSLa1");
  if(ScatterPlotAlphaCLSLa1){
     for(int i=0;i<n;i++){
       float mean = ScatterPlotAlphaCLSLa1->ProjectionY("_py",i,i+1)->GetMean();
       float entries = ScatterPlotAlphaCLSLa1->ProjectionY("_py",i,i+1)->GetEntries();
       float error = ScatterPlotAlphaCLSLa1->ProjectionY("_py",i,i+1)->GetRMS()/ sqrt(entries);         
       x[i]=(i+1)*step;
       ex[i]=step*0.5;
       y[i]=mean;
       ey[i]=error;
       //std::cout<<"flag "<<i<<" "<<x[i]<<" "<<ex[i]<<" "<<y[i]<<" "<<ey[i]<<std::endl;
     }
  }   
  
  TGraphErrors * plot1 = new TGraphErrors(n,x,y,ex,ey);	
  plot1->SetMarkerColor(1);
  plot1->SetMarkerStyle(20);
  plot1->SetMarkerSize(0.5);
  plot1->GetXaxis()->SetTitle("Angle (degree)");
  plot1->GetYaxis()->SetTitle("Mean Cluster Size Layer 1");	
  plot1->Draw("AP");
  labeltoSave = "CLS/ClusterSizeVsAngleLa1.png";
  Ca5->SaveAs(labeltoSave.c_str());
  Ca5->Clear();
  

  ScatterPlotAlphaCLSLa2= (TH2F*)theFile->Get("DQMData/Muons/MuonSegEff/Residuals/Investigation/ScatterPlotAlphaCLSLa2");
  if(ScatterPlotAlphaCLSLa2){
     for(int i=0;i<n;i++){
       float mean = ScatterPlotAlphaCLSLa2->ProjectionY("_py",i,i+1)->GetMean();
       float entries = ScatterPlotAlphaCLSLa2->ProjectionY("_py",i,i+1)->GetEntries();
       float error;
       if(entries!=0 ) error = ScatterPlotAlphaCLSLa2->ProjectionY("_py",i,i+1)->GetRMS()/ sqrt(entries);   
       else error =0;
       x[i]=(i+1)*step;
       ex[i]=step*0.5;
       y[i]=mean;
       ey[i]=error;
     }
  }
   
  TGraphErrors * plot2 = new TGraphErrors(n,x,y,ex,ey);	
  plot2->SetMarkerColor(2);
  plot2->SetMarkerStyle(21);
  plot2->SetMarkerSize(0.5);
  plot2->GetXaxis()->SetTitle("Angle (degree)");
  plot2->GetYaxis()->SetTitle("Mean Cluster Size Layer 2");	
  plot2->Draw("AP");
  labeltoSave = "CLS/ClusterSizeVsAngleLa2.png";
  Ca5->SaveAs(labeltoSave.c_str());
  Ca5->Clear();
  
  
  ScatterPlotAlphaCLSLa3= (TH2F*)theFile->Get("DQMData/Muons/MuonSegEff/Residuals/Investigation/ScatterPlotAlphaCLSLa3");
  if(ScatterPlotAlphaCLSLa3){
     for(int i=0;i<n;i++){
       float mean = ScatterPlotAlphaCLSLa3->ProjectionY("_py",i,i+1)->GetMean();
       float entries = ScatterPlotAlphaCLSLa3->ProjectionY("_py",i,i+1)->GetEntries();
       float error;
       if(entries!=0) error = ScatterPlotAlphaCLSLa3->ProjectionY("_py",i,i+1)->GetRMS()/ sqrt(entries);   
       else error = 0;
       x[i]=(i+1)*step;
       ex[i]=step*0.5;
       y[i]=mean;
       ey[i]=error;
     }
  }
  
  TGraphErrors * plot3 = new TGraphErrors(n,x,y,ex,ey);	
  plot3->SetMarkerColor(3);
  plot3->SetMarkerStyle(22);
  plot3->SetMarkerSize(0.5);
  plot3->GetXaxis()->SetTitle("Angle (degree)");
  plot3->GetYaxis()->SetTitle("Mean Cluster Size Layer 3");	
  plot3->Draw("AP");
  labeltoSave = "CLS/ClusterSizeVsAngleLa3.png";
  Ca5->SaveAs(labeltoSave.c_str());
  Ca5->Clear();
  

  ScatterPlotAlphaCLSLa4= (TH2F*)theFile->Get("DQMData/Muons/MuonSegEff/Residuals/Investigation/ScatterPlotAlphaCLSLa4");
  if(ScatterPlotAlphaCLSLa4){
    for(int i=0;i<n;i++){
      float mean = ScatterPlotAlphaCLSLa4->ProjectionY("_py",i,i+1)->GetMean();
      float entries = ScatterPlotAlphaCLSLa4->ProjectionY("_py",i,i+1)->GetEntries();
      float error;
      if(entries!=0) error = ScatterPlotAlphaCLSLa4->ProjectionY("_py",i,i+1)->GetRMS()/ sqrt(entries);   
      else error = 0;
      x[i]=(i+1)*step;
      ex[i]=step*0.5;
      y[i]=mean;
      ey[i]=error;
    }
  }
  
  TGraphErrors * plot4 = new TGraphErrors(n,x,y,ex,ey);	
  plot4->SetMarkerColor(4);
  plot4->SetMarkerStyle(23);
  plot4->SetMarkerSize(0.5);
  plot4->GetXaxis()->SetTitle("Angle (degree)");
  plot4->GetYaxis()->SetTitle("Mean Cluster Size Layer 4");	
  plot4->Draw("AP");
  labeltoSave = "CLS/ClusterSizeVsAngleLa4.png";
  Ca5->SaveAs(labeltoSave.c_str());
  Ca5->Clear();
  

  ScatterPlotAlphaCLSLa5= (TH2F*)theFile->Get("DQMData/Muons/MuonSegEff/Residuals/Investigation/ScatterPlotAlphaCLSLa5");
  if(ScatterPlotAlphaCLSLa5){
    for(int i=0;i<n;i++){
      float mean = ScatterPlotAlphaCLSLa5->ProjectionY("_py",i,i+1)->GetMean();
      float entries = ScatterPlotAlphaCLSLa5->ProjectionY("_py",i,i+1)->GetEntries();
      float error;
      if(entries!=0) error = ScatterPlotAlphaCLSLa5->ProjectionY("_py",i,i+1)->GetRMS()/ sqrt(entries);   
      else error = 0;
      x[i]=(i+1)*step;
      ex[i]=step*0.5;
      y[i]=mean;
      ey[i]=error;
    }
  }
  
  TGraphErrors * plot5 = new TGraphErrors(n,x,y,ex,ey);	
  plot5->SetMarkerColor(5);
  plot5->SetMarkerStyle(25);
  plot5->SetMarkerSize(0.5);
  plot5->GetXaxis()->SetTitle("Angle (degree)");
  plot5->GetYaxis()->SetTitle("Mean Cluster Size Layer 5");	
  plot5->Draw("AP");
  labeltoSave = "CLS/ClusterSizeVsAngleLa5.png";
  Ca5->SaveAs(labeltoSave.c_str());
  Ca5->Clear();
    
  ScatterPlotAlphaCLSLa6= (TH2F*)theFile->Get("DQMData/Muons/MuonSegEff/Residuals/Investigation/ScatterPlotAlphaCLSLa6");
  if(ScatterPlotAlphaCLSLa6){
    for(int i=0;i<n;i++){
      float mean = ScatterPlotAlphaCLSLa6->ProjectionY("_py",i,i+1)->GetMean();
      float entries = ScatterPlotAlphaCLSLa6->ProjectionY("_py",i,i+1)->GetEntries();
      float error;
      if(entries!=0) error = ScatterPlotAlphaCLSLa6->ProjectionY("_py",i,i+1)->GetRMS()/ sqrt(entries);   
      else error = 0;
      x[i]=(i+1)*step;
      ex[i]=step*0.5;
      y[i]=mean;
      ey[i]=error;
    }
  }
  
  TGraphErrors * plot6 = new TGraphErrors(n,x,y,ex,ey);	
  plot6->SetMarkerColor(6);
  plot6->SetMarkerStyle(26);
  plot6->SetMarkerSize(0.5);
  plot6->GetXaxis()->SetTitle("Angle (degree)");
  plot6->GetYaxis()->SetTitle("Mean Cluster Size Layer 6");	
  plot6->Draw("AP");
  labeltoSave = "CLS/ClusterSizeVsAngleLa6.png";
  Ca5->SaveAs(labeltoSave.c_str());
  Ca5->Clear();
  
  Ca5a->Clear();
  
  plot1->SetName("gr1");
  plot2->SetName("gr2");
  plot3->SetName("gr3");
  plot4->SetName("gr4");
  plot5->SetName("gr5");
  plot6->SetName("gr6");

  plot1->Write();
  plot2->Write();
  plot3->Write();
  plot4->Write();
  plot5->Write();
  plot6->Write();

  Ca5a->SaveAs("CLS/LayersByAngle.png");

  //El otro angulo

  Ca5->Clear();

  ScatterPlotAlphaPCLSLa1= (TH2F*)theFile->Get("DQMData/Muons/MuonSegEff/Residuals/Investigation/ScatterPlotAlphaPCLSLa1");
  if(ScatterPlotAlphaPCLSLa1){
     for(int i=0;i<n;i++){
       float mean = ScatterPlotAlphaPCLSLa1->ProjectionY("_py",i,i+1)->GetMean();
       float entries = ScatterPlotAlphaPCLSLa1->ProjectionY("_py",i,i+1)->GetEntries();
       float error;
       if(entries!=0) error = ScatterPlotAlphaPCLSLa1->ProjectionY("_py",i,i+1)->GetRMS()/ sqrt(entries);         
       else error =0;
       x[i]=(i+1)*step;
       ex[i]=step*0.5;
       y[i]=mean;
       ey[i]=error;
     }
     
     TGraphErrors * plot = new TGraphErrors(n,x,y,ex,ey);	
     plot->SetMarkerColor(6);
     plot->SetMarkerStyle(27);
     plot->SetMarkerSize(0.5);
     plot->GetXaxis()->SetTitle("AngleParallel (degree)");
     plot->GetYaxis()->SetTitle("Mean Cluster Size Layer 1");	
     plot->Draw("AP");
     labeltoSave = "CLS/ClusterSizeVsAngleParallelLa1.png";
     Ca5->SaveAs(labeltoSave.c_str());
     Ca5->Clear();
  }

  
  ScatterPlotAlphaPCLSLa2= (TH2F*)theFile->Get("DQMData/Muons/MuonSegEff/Residuals/Investigation/ScatterPlotAlphaPCLSLa2");
  if(ScatterPlotAlphaPCLSLa2){
     for(int i=0;i<n;i++){
       float mean = ScatterPlotAlphaPCLSLa2->ProjectionY("_py",i,i+1)->GetMean();
       float entries = ScatterPlotAlphaPCLSLa2->ProjectionY("_py",i,i+1)->GetEntries();
       float error;
       if(entries!=0) error = ScatterPlotAlphaPCLSLa2->ProjectionY("_py",i,i+1)->GetRMS()/ sqrt(entries);   
       else error =0;
       x[i]=(i+1)*step;
       ex[i]=step*0.5;
       y[i]=mean;
       ey[i]=error;
     }
     
     TGraphErrors * plot = new TGraphErrors(n,x,y,ex,ey);	
     plot->SetMarkerColor(6);
     plot->SetMarkerStyle(20);
     plot->SetMarkerSize(0.5);
     plot->GetXaxis()->SetTitle("AngleParallel (degree)");
     plot->GetYaxis()->SetTitle("Mean Cluster Size Layer 2");	
     plot->Draw("AP");
     labeltoSave = "CLS/ClusterSizeVsAngleParallelLa2.png";
     Ca5->SaveAs(labeltoSave.c_str());
     Ca5->Clear();
  }
  
  ScatterPlotAlphaPCLSLa3= (TH2F*)theFile->Get("DQMData/Muons/MuonSegEff/Residuals/Investigation/ScatterPlotAlphaPCLSLa3");
  if(ScatterPlotAlphaPCLSLa3){
     for(int i=0;i<n;i++){
       float mean = ScatterPlotAlphaPCLSLa3->ProjectionY("_py",i,i+1)->GetMean();
       float entries = ScatterPlotAlphaPCLSLa3->ProjectionY("_py",i,i+1)->GetEntries();
       float error;
       if(entries!=0) error = ScatterPlotAlphaPCLSLa3->ProjectionY("_py",i,i+1)->GetRMS()/ sqrt(entries);   
       else error = 0;
       x[i]=(i+1)*step;
       ex[i]=step*0.5;
       y[i]=mean;
       ey[i]=error;
     }
     
     TGraphErrors * plot = new TGraphErrors(n,x,y,ex,ey);	
     plot->SetMarkerColor(6);
     plot->SetMarkerStyle(20);
     plot->SetMarkerSize(0.5);
     plot->GetXaxis()->SetTitle("AngleParallel (degree)");
     plot->GetYaxis()->SetTitle("Mean Cluster Size Layer 3");	
     plot->Draw("AP");
     labeltoSave = "CLS/ClusterSizeVsAngleParallelLa3.png";
     Ca5->SaveAs(labeltoSave.c_str());
     Ca5->Clear();
  }
  

  ScatterPlotAlphaPCLSLa4= (TH2F*)theFile->Get("DQMData/Muons/MuonSegEff/Residuals/Investigation/ScatterPlotAlphaPCLSLa4");
  if(ScatterPlotAlphaPCLSLa4){
    for(int i=0;i<n;i++){
      float mean = ScatterPlotAlphaPCLSLa4->ProjectionY("_py",i,i+1)->GetMean();
      float entries = ScatterPlotAlphaPCLSLa4->ProjectionY("_py",i,i+1)->GetEntries();
      float error;
      if(entries!=0) error = ScatterPlotAlphaPCLSLa4->ProjectionY("_py",i,i+1)->GetRMS()/ sqrt(entries);   
      else error =0;
      x[i]=(i+1)*step;
      ex[i]=step*0.5;
      y[i]=mean;
      ey[i]=error;
    }
    
    TGraphErrors * plot = new TGraphErrors(n,x,y,ex,ey);	
    plot->SetMarkerColor(6);
    plot->SetMarkerStyle(20);
    plot->SetMarkerSize(0.5);
    plot->GetXaxis()->SetTitle("AngleParallel (degree)");
    plot->GetYaxis()->SetTitle("Mean Cluster Size Layer 4");	
    plot->Draw("AP");
    labeltoSave = "CLS/ClusterSizeVsAngleParallelLa4.png";
    Ca5->SaveAs(labeltoSave.c_str());
    Ca5->Clear();
  }

  ScatterPlotAlphaPCLSLa5= (TH2F*)theFile->Get("DQMData/Muons/MuonSegEff/Residuals/Investigation/ScatterPlotAlphaPCLSLa5");
  if(ScatterPlotAlphaPCLSLa5){
    for(int i=0;i<n;i++){
      float mean = ScatterPlotAlphaPCLSLa5->ProjectionY("_py",i,i+1)->GetMean();
      float entries = ScatterPlotAlphaPCLSLa5->ProjectionY("_py",i,i+1)->GetEntries();
      float error;
      if(entries!=0) error = ScatterPlotAlphaPCLSLa5->ProjectionY("_py",i,i+1)->GetRMS()/ sqrt(entries);   
      else error =0;
      x[i]=(i+1)*step;
      ex[i]=step*0.5;
      y[i]=mean;
      ey[i]=error;
    }
    
    TGraphErrors * plot = new TGraphErrors(n,x,y,ex,ey);	
    plot->SetMarkerColor(6);
    plot->SetMarkerStyle(20);
    plot->SetMarkerSize(0.5);
    plot->GetXaxis()->SetTitle("AngleParallel (degree)");
    plot->GetYaxis()->SetTitle("Mean Cluster Size Layer 5");	
    plot->Draw("AP");
    labeltoSave = "CLS/ClusterSizeVsAngleParallelLa5.png";
    Ca5->SaveAs(labeltoSave.c_str());
    Ca5->Clear();
  }

  ScatterPlotAlphaPCLSLa6= (TH2F*)theFile->Get("DQMData/Muons/MuonSegEff/Residuals/Investigation/ScatterPlotAlphaPCLSLa6");
  if(ScatterPlotAlphaPCLSLa6){
    for(int i=0;i<n;i++){
      float mean = ScatterPlotAlphaPCLSLa6->ProjectionY("_py",i,i+1)->GetMean();
      float entries = ScatterPlotAlphaPCLSLa6->ProjectionY("_py",i,i+1)->GetEntries();
      float error;
      if(entries!=0) error = ScatterPlotAlphaPCLSLa6->ProjectionY("_py",i,i+1)->GetRMS()/ sqrt(entries);   
      else error = 0;
      x[i]=(i+1)*step;
      ex[i]=step*0.5;
      y[i]=mean;
      ey[i]=error;
    }
    
    TGraphErrors * plot = new TGraphErrors(n,x,y,ex,ey);	
    plot->SetMarkerColor(6);
    plot->SetMarkerStyle(20);
    plot->SetMarkerSize(0.5);
    plot->GetXaxis()->SetTitle("AngleParallel (degree)");
    plot->GetYaxis()->SetTitle("Mean Cluster Size Layer 6");	
    plot->Draw("AP");
    labeltoSave = "CLS/ClusterSizeVsAngleParallelLa6.png";
    Ca5->SaveAs(labeltoSave.c_str());
    Ca5->Clear();
  }
  
  if(debug) std::cout<<"Producing Dist Border Plots"<<std::endl;

  Ca8->Clear();

  DistBorderClu1La1=(TH1F*)theFile->Get("DQMData/Muons/MuonSegEff/Residuals/Investigation/DistBorderClu1La1");if(DistBorderClu1La1) { DistBorderClu1La1->GetYaxis()->SetTitle("Probability for impact point Cluster Size 1 Layer 1");DistBorderClu1La1->GetXaxis()->SetTitle("Impact Point (Strip Units)");if(DistBorderClu1La1->Integral()!=0) DistBorderClu1La1->Scale(1./DistBorderClu1La1->Integral());DistBorderClu1La1->Draw(); Ca8->SaveAs("CLS/DistBorderClu1La1.png");Ca8->Clear();} 
  DistBorderClu1La2=(TH1F*)theFile->Get("DQMData/Muons/MuonSegEff/Residuals/Investigation/DistBorderClu1La2");if(DistBorderClu1La2) { DistBorderClu1La2->GetYaxis()->SetTitle("Probability for impact point Cluster Size 1 Layer 2");DistBorderClu1La2->GetXaxis()->SetTitle("Impact Point (Strip Units)");if(DistBorderClu1La2->Integral()!=0) DistBorderClu1La2->Scale(1./DistBorderClu1La2->Integral());DistBorderClu1La2->Draw(); Ca8->SaveAs("CLS/DistBorderClu1La2.png");Ca8->Clear();} 
  DistBorderClu1La3=(TH1F*)theFile->Get("DQMData/Muons/MuonSegEff/Residuals/Investigation/DistBorderClu1La3");if(DistBorderClu1La3) { DistBorderClu1La3->GetYaxis()->SetTitle("Probability for impact point Cluster Size 1 Layer 3");DistBorderClu1La3->GetXaxis()->SetTitle("Impact Point (Strip Units)");if(DistBorderClu1La3->Integral()!=0) DistBorderClu1La3->Scale(1./DistBorderClu1La3->Integral());DistBorderClu1La3->Draw(); Ca8->SaveAs("CLS/DistBorderClu1La3.png");Ca8->Clear();} 
  DistBorderClu1La4=(TH1F*)theFile->Get("DQMData/Muons/MuonSegEff/Residuals/Investigation/DistBorderClu1La4");if(DistBorderClu1La4) { DistBorderClu1La4->GetYaxis()->SetTitle("Probability for impact point Cluster Size 1 Layer 4");DistBorderClu1La4->GetXaxis()->SetTitle("Impact Point (Strip Units)");if(DistBorderClu1La4->Integral()!=0) DistBorderClu1La4->Scale(1./DistBorderClu1La4->Integral());DistBorderClu1La4->Draw(); Ca8->SaveAs("CLS/DistBorderClu1La4.png");Ca8->Clear();} 
  DistBorderClu1La5=(TH1F*)theFile->Get("DQMData/Muons/MuonSegEff/Residuals/Investigation/DistBorderClu1La5");if(DistBorderClu1La5) { DistBorderClu1La5->GetYaxis()->SetTitle("Probability for impact point Cluster Size 1 Layer 5");DistBorderClu1La5->GetXaxis()->SetTitle("Impact Point (Strip Units)");if(DistBorderClu1La5->Integral()!=0) DistBorderClu1La5->Scale(1./DistBorderClu1La5->Integral());DistBorderClu1La5->Draw(); Ca8->SaveAs("CLS/DistBorderClu1La5.png");Ca8->Clear();} 
  DistBorderClu1La6=(TH1F*)theFile->Get("DQMData/Muons/MuonSegEff/Residuals/Investigation/DistBorderClu1La6");if(DistBorderClu1La6) { DistBorderClu1La6->GetYaxis()->SetTitle("Probability for impact point Cluster Size 1 Layer 6");DistBorderClu1La6->GetXaxis()->SetTitle("Impact Point (Strip Units)");if(DistBorderClu1La6->Integral()!=0) DistBorderClu1La6->Scale(1./DistBorderClu1La6->Integral());DistBorderClu1La6->Draw(); Ca8->SaveAs("CLS/DistBorderClu1La6.png");Ca8->Clear();} 

  if(debug) std::cout<<"Producing Dist Border Plots Clu2"<<std::endl;

  DistBorderClu2La1=(TH1F*)theFile->Get("DQMData/Muons/MuonSegEff/Residuals/Investigation/DistBorderClu2La1");if(DistBorderClu2La1) { DistBorderClu2La1->GetYaxis()->SetTitle("Probability for impact point Cluster Size 2 Layer 1");DistBorderClu2La1->GetXaxis()->SetTitle("Impact Point (Strip Units)");if(DistBorderClu2La1->Integral()!=0) DistBorderClu2La1->Scale(1./DistBorderClu2La1->Integral());DistBorderClu2La1->Draw(); Ca8->SaveAs("CLS/DistBorderClu2La1.png");Ca8->Clear();} 
  DistBorderClu2La2=(TH1F*)theFile->Get("DQMData/Muons/MuonSegEff/Residuals/Investigation/DistBorderClu2La2");if(DistBorderClu2La2) { DistBorderClu2La2->GetYaxis()->SetTitle("Probability for impact point Cluster Size 2 Layer 2");DistBorderClu2La2->GetXaxis()->SetTitle("Impact Point (Strip Units)");if(DistBorderClu2La2->Integral()!=0) DistBorderClu2La2->Scale(1./DistBorderClu2La2->Integral());DistBorderClu2La2->Draw(); Ca8->SaveAs("CLS/DistBorderClu2La2.png");Ca8->Clear();} 
  DistBorderClu2La3=(TH1F*)theFile->Get("DQMData/Muons/MuonSegEff/Residuals/Investigation/DistBorderClu2La3");if(DistBorderClu2La3) { DistBorderClu2La3->GetYaxis()->SetTitle("Probability for impact point Cluster Size 2 Layer 3");DistBorderClu2La3->GetXaxis()->SetTitle("Impact Point (Strip Units)");if(DistBorderClu2La3->Integral()!=0) DistBorderClu2La3->Scale(1./DistBorderClu2La3->Integral());DistBorderClu2La3->Draw(); Ca8->SaveAs("CLS/DistBorderClu2La3.png");Ca8->Clear();} 
  DistBorderClu2La4=(TH1F*)theFile->Get("DQMData/Muons/MuonSegEff/Residuals/Investigation/DistBorderClu2La4");if(DistBorderClu2La4) { DistBorderClu2La4->GetYaxis()->SetTitle("Probability for impact point Cluster Size 2 Layer 4");DistBorderClu2La4->GetXaxis()->SetTitle("Impact Point (Strip Units)");if(DistBorderClu2La4->Integral()!=0) DistBorderClu2La4->Scale(1./DistBorderClu2La4->Integral());DistBorderClu2La4->Draw(); Ca8->SaveAs("CLS/DistBorderClu2La4.png");Ca8->Clear();} 
  DistBorderClu2La5=(TH1F*)theFile->Get("DQMData/Muons/MuonSegEff/Residuals/Investigation/DistBorderClu2La5");if(DistBorderClu2La5) { DistBorderClu2La5->GetYaxis()->SetTitle("Probability for impact point Cluster Size 2 Layer 5");DistBorderClu2La5->GetXaxis()->SetTitle("Impact Point (Strip Units)");if(DistBorderClu2La5->Integral()!=0) DistBorderClu2La5->Scale(1./DistBorderClu2La5->Integral());DistBorderClu2La5->Draw(); Ca8->SaveAs("CLS/DistBorderClu2La5.png");Ca8->Clear();} 
  DistBorderClu2La6=(TH1F*)theFile->Get("DQMData/Muons/MuonSegEff/Residuals/Investigation/DistBorderClu2La6");if(DistBorderClu2La6) { DistBorderClu2La6->GetYaxis()->SetTitle("Probability for impact point Cluster Size 2 Layer 6");DistBorderClu2La6->GetXaxis()->SetTitle("Impact Point (Strip Units)");if(DistBorderClu2La6->Integral()!=0) DistBorderClu2La6->Scale(1./DistBorderClu2La6->Integral());DistBorderClu2La6->Draw(); Ca8->SaveAs("CLS/DistBorderClu2La6.png");Ca8->Clear();} 

  if(debug) std::cout<<"Producing Dist Border Plots Clu3"<<std::endl;

  DistBorderClu3La1=(TH1F*)theFile->Get("DQMData/Muons/MuonSegEff/Residuals/Investigation/DistBorderClu3La1");if(DistBorderClu3La1) { DistBorderClu3La1->GetYaxis()->SetTitle("Probability for impact point Cluster Size 3 Layer 1");DistBorderClu3La1->GetXaxis()->SetTitle("Impact Point (Strip Units)");if(DistBorderClu3La1->Integral()!=0) DistBorderClu3La1->Scale(1./DistBorderClu3La1->Integral());DistBorderClu3La1->Draw(); Ca8->SaveAs("CLS/DistBorderClu3La1.png");Ca8->Clear();} 
  DistBorderClu3La2=(TH1F*)theFile->Get("DQMData/Muons/MuonSegEff/Residuals/Investigation/DistBorderClu3La2");if(DistBorderClu3La2) { DistBorderClu3La2->GetYaxis()->SetTitle("Probability for impact point Cluster Size 3 Layer 2");DistBorderClu3La2->GetXaxis()->SetTitle("Impact Point (Strip Units)");if(DistBorderClu3La2->Integral()!=0) DistBorderClu3La2->Scale(1./DistBorderClu3La2->Integral());DistBorderClu3La2->Draw(); Ca8->SaveAs("CLS/DistBorderClu3La2.png");Ca8->Clear();} 
  DistBorderClu3La3=(TH1F*)theFile->Get("DQMData/Muons/MuonSegEff/Residuals/Investigation/DistBorderClu3La3");if(DistBorderClu3La3) { DistBorderClu3La3->GetYaxis()->SetTitle("Probability for impact point Cluster Size 3 Layer 3");DistBorderClu3La3->GetXaxis()->SetTitle("Impact Point (Strip Units)");if(DistBorderClu3La3->Integral()!=0) DistBorderClu3La3->Scale(1./DistBorderClu3La3->Integral());DistBorderClu3La3->Draw(); Ca8->SaveAs("CLS/DistBorderClu3La3.png");Ca8->Clear();} 
  DistBorderClu3La4=(TH1F*)theFile->Get("DQMData/Muons/MuonSegEff/Residuals/Investigation/DistBorderClu3La4");if(DistBorderClu3La4) { DistBorderClu3La4->GetYaxis()->SetTitle("Probability for impact point Cluster Size 3 Layer 4");DistBorderClu3La4->GetXaxis()->SetTitle("Impact Point (Strip Units)");if(DistBorderClu3La4->Integral()!=0) DistBorderClu3La4->Scale(1./DistBorderClu3La4->Integral());DistBorderClu3La4->Draw(); Ca8->SaveAs("CLS/DistBorderClu3La4.png");Ca8->Clear();} 
  DistBorderClu3La5=(TH1F*)theFile->Get("DQMData/Muons/MuonSegEff/Residuals/Investigation/DistBorderClu3La5");if(DistBorderClu3La5) { DistBorderClu3La5->GetYaxis()->SetTitle("Probability for impact point Cluster Size 3 Layer 5");DistBorderClu3La5->GetXaxis()->SetTitle("Impact Point (Strip Units)");if(DistBorderClu3La5->Integral()!=0) DistBorderClu3La5->Scale(1./DistBorderClu3La5->Integral());DistBorderClu3La5->Draw(); Ca8->SaveAs("CLS/DistBorderClu3La5.png");Ca8->Clear();} 
  DistBorderClu3La6=(TH1F*)theFile->Get("DQMData/Muons/MuonSegEff/Residuals/Investigation/DistBorderClu3La6");if(DistBorderClu3La6) { DistBorderClu3La6->GetYaxis()->SetTitle("Probability for impact point Cluster Size 3 Layer 6");DistBorderClu3La6->GetXaxis()->SetTitle("Impact Point (Strip Units)");if(DistBorderClu3La6->Integral()!=0) DistBorderClu3La6->Scale(1./DistBorderClu3La6->Integral());DistBorderClu3La6->Draw(); Ca8->SaveAs("CLS/DistBorderClu3La6.png");Ca8->Clear();} 
  
  Ca8->Close();
      
  if(debug) std::cout<<"Producing Residual Plots for Stations (Paper) in the End Cap"<<std::endl;

  Ca7->Clear();

  residualRB1in->Draw(); labeltoSave = "resBarrel/residualRB1in.png";  residualRB1in->GetXaxis()->SetTitle("(cm)");   if(  residualRB1in->GetEntries()!=0) Ca7->SetLogy(); Ca7->SaveAs(labeltoSave.c_str()); Ca7->Clear();
  residualRB1out->Draw(); labeltoSave = "resBarrel/residualRB1out.png"; residualRB1out->GetXaxis()->SetTitle("(cm)");   if(residualRB1out->GetEntries()!=0) Ca7->SetLogy(); Ca7->SaveAs(labeltoSave.c_str()); Ca7->Clear();
  residualRB2in->Draw(); labeltoSave = "resBarrel/residualRB2in.png";  residualRB2in->GetXaxis()->SetTitle("(cm)");   if(  residualRB2in->GetEntries()!=0) Ca7->SetLogy(); Ca7->SaveAs(labeltoSave.c_str()); Ca7->Clear();
  residualRB2out->Draw(); labeltoSave = "resBarrel/residualRB2out.png"; residualRB2out->GetXaxis()->SetTitle("(cm)");   if(residualRB2out->GetEntries()!=0) Ca7->SetLogy(); Ca7->SaveAs(labeltoSave.c_str()); Ca7->Clear();
  residualRB3->Draw(); labeltoSave = "resBarrel/residualRB3.png";       residualRB3->GetXaxis()->SetTitle("(cm)");   if(   residualRB3->GetEntries()!=0) Ca7->SetLogy(); Ca7->SaveAs(labeltoSave.c_str()); Ca7->Clear();
  residualRB4->Draw(); labeltoSave = "resBarrel/residualRB4.png";    residualRB4->GetXaxis()->SetTitle("(cm)");   if(      residualRB4->GetEntries()!=0) Ca7->SetLogy(); Ca7->SaveAs(labeltoSave.c_str()); Ca7->Clear();

  residualStation1Ring2_A->Draw(); labeltoSave = "resEndCap/residualStation1Ring2_A.png"; residualStation1Ring2_A->GetXaxis()->SetTitle("(cm)");   if(residualStation1Ring2_A->GetEntries()!=0) Ca7->SetLogy(); Ca7->SaveAs(labeltoSave.c_str()); Ca7->Clear();
  residualStation1Ring2_B->Draw(); labeltoSave = "resEndCap/residualStation1Ring2_B.png"; residualStation1Ring2_B->GetXaxis()->SetTitle("(cm)");   if(residualStation1Ring2_B->GetEntries()!=0) Ca7->SetLogy(); Ca7->SaveAs(labeltoSave.c_str()); Ca7->Clear();
  residualStation1Ring2_C->Draw(); labeltoSave = "resEndCap/residualStation1Ring2_C.png"; residualStation1Ring2_C->GetXaxis()->SetTitle("(cm)");   if(residualStation1Ring2_C->GetEntries()!=0) Ca7->SetLogy(); Ca7->SaveAs(labeltoSave.c_str()); Ca7->Clear();
  residualStation1Ring3_A->Draw(); labeltoSave = "resEndCap/residualStation1Ring3_A.png"; residualStation1Ring3_A->GetXaxis()->SetTitle("(cm)");   if(residualStation1Ring3_A->GetEntries()!=0) Ca7->SetLogy(); Ca7->SaveAs(labeltoSave.c_str()); Ca7->Clear();
  residualStation1Ring3_B->Draw(); labeltoSave = "resEndCap/residualStation1Ring3_B.png"; residualStation1Ring3_B->GetXaxis()->SetTitle("(cm)");   if(residualStation1Ring3_B->GetEntries()!=0) Ca7->SetLogy(); Ca7->SaveAs(labeltoSave.c_str()); Ca7->Clear();
  residualStation1Ring3_C->Draw(); labeltoSave = "resEndCap/residualStation1Ring3_C.png"; residualStation1Ring3_C->GetXaxis()->SetTitle("(cm)");   if(residualStation1Ring3_C->GetEntries()!=0) Ca7->SetLogy(); Ca7->SaveAs(labeltoSave.c_str()); Ca7->Clear();
  		       
  residualStation23Ring2_A->Draw();  labeltoSave = "resEndCap/residualStation23Ring2_A.png"; residualStation23Ring2_A->GetXaxis()->SetTitle("(cm)");   if(residualStation23Ring2_A->GetEntries()!=0) Ca7->SetLogy();   Ca7->SaveAs(labeltoSave.c_str()); Ca7->Clear();
  residualStation23Ring2_B->Draw();  labeltoSave = "resEndCap/residualStation23Ring2_B.png"; residualStation23Ring2_B->GetXaxis()->SetTitle("(cm)");   if(residualStation23Ring2_B->GetEntries()!=0) Ca7->SetLogy();   Ca7->SaveAs(labeltoSave.c_str()); Ca7->Clear();
  residualStation23Ring2_C->Draw();  labeltoSave = "resEndCap/residualStation23Ring2_C.png"; residualStation23Ring2_C->GetXaxis()->SetTitle("(cm)");   if(residualStation23Ring2_C->GetEntries()!=0) Ca7->SetLogy();   Ca7->SaveAs(labeltoSave.c_str()); Ca7->Clear();

  residualStation23Ring3_A->Draw();  labeltoSave = "resEndCap/residualStation23Ring3_A.png"; residualStation23Ring3_A->GetXaxis()->SetTitle("(cm)");   if(residualStation23Ring3_A->GetEntries()!=0) Ca7->SetLogy();   Ca7->SaveAs(labeltoSave.c_str()); Ca7->Clear();
  residualStation23Ring3_B->Draw();  labeltoSave = "resEndCap/residualStation23Ring3_B.png"; residualStation23Ring3_B->GetXaxis()->SetTitle("(cm)");   if(residualStation23Ring3_B->GetEntries()!=0) Ca7->SetLogy();   Ca7->SaveAs(labeltoSave.c_str()); Ca7->Clear();
  residualStation23Ring3_C->Draw();  labeltoSave = "resEndCap/residualStation23Ring3_C.png"; residualStation23Ring3_C->GetXaxis()->SetTitle("(cm)");   if(residualStation23Ring3_C->GetEntries()!=0) Ca7->SetLogy();   Ca7->SaveAs(labeltoSave.c_str()); Ca7->Clear();

  residualStation123Ring3_A->Draw();  labeltoSave = "resEndCap/residualStation123Ring3_A.png"; residualStation123Ring3_A->GetXaxis()->SetTitle("(cm)");   if(residualStation123Ring3_A->GetEntries()!=0) Ca7->SetLogy();   Ca7->SaveAs(labeltoSave.c_str()); Ca7->Clear();
  residualStation123Ring3_B->Draw();  labeltoSave = "resEndCap/residualStation123Ring3_B.png"; residualStation123Ring3_B->GetXaxis()->SetTitle("(cm)");   if(residualStation123Ring3_B->GetEntries()!=0) Ca7->SetLogy();   Ca7->SaveAs(labeltoSave.c_str()); Ca7->Clear();
  residualStation123Ring3_C->Draw();  labeltoSave = "resEndCap/residualStation123Ring3_C.png"; residualStation123Ring3_C->GetXaxis()->SetTitle("(cm)");   if(residualStation123Ring3_C->GetEntries()!=0) Ca7->SetLogy();   Ca7->SaveAs(labeltoSave.c_str()); Ca7->Clear();

  if(debug) std::cout<<"Producing Residual Plots for Disks in the End Cap"<<std::endl;

  residualDiskm1Ring2->Draw(); labeltoSave = "resEndCap/residualDiskm1Ring2.png"; residualDiskm1Ring2->GetXaxis()->SetTitle("(cm)");   if(residualDiskm1Ring2->GetEntries()!=0) Ca7->SetLogy(); Ca7->SaveAs(labeltoSave.c_str()); Ca7->Clear();
  residualDiskm1Ring3->Draw(); labeltoSave = "resEndCap/residualDiskm1Ring3.png"; residualDiskm1Ring3->GetXaxis()->SetTitle("(cm)");   if(residualDiskm1Ring3->GetEntries()!=0) Ca7->SetLogy(); Ca7->SaveAs(labeltoSave.c_str()); Ca7->Clear();
  residualDiskm2Ring2->Draw(); labeltoSave = "resEndCap/residualDiskm2Ring2.png"; residualDiskm2Ring2->GetXaxis()->SetTitle("(cm)");   if(residualDiskm2Ring2->GetEntries()!=0) Ca7->SetLogy(); Ca7->SaveAs(labeltoSave.c_str()); Ca7->Clear();
  residualDiskm2Ring3->Draw(); labeltoSave = "resEndCap/residualDiskm2Ring3.png"; residualDiskm2Ring3->GetXaxis()->SetTitle("(cm)");   if(residualDiskm2Ring3->GetEntries()!=0) Ca7->SetLogy(); Ca7->SaveAs(labeltoSave.c_str()); Ca7->Clear();
  residualDiskm3Ring2->Draw(); labeltoSave = "resEndCap/residualDiskm3Ring2.png"; residualDiskm3Ring2->GetXaxis()->SetTitle("(cm)");   if(residualDiskm3Ring2->GetEntries()!=0) Ca7->SetLogy(); Ca7->SaveAs(labeltoSave.c_str()); Ca7->Clear();
  residualDiskm3Ring3->Draw(); labeltoSave = "resEndCap/residualDiskm3Ring3.png"; residualDiskm3Ring3->GetXaxis()->SetTitle("(cm)");   if(residualDiskm3Ring3->GetEntries()!=0) Ca7->SetLogy(); Ca7->SaveAs(labeltoSave.c_str()); Ca7->Clear();

  if(debug) std::cout<<"Producing Residual Plots for Disks in the End Cap 2"<<std::endl;

  residualDisk1Ring2->Draw();  labeltoSave = "resEndCap/residualDisk1Ring2.png"; residualDisk1Ring2->GetXaxis()->SetTitle("(cm)");   if(residualDisk1Ring2->GetEntries()!=0) Ca7->SetLogy();   Ca7->SaveAs(labeltoSave.c_str()); Ca7->Clear();
  residualDisk1Ring3->Draw();  labeltoSave = "resEndCap/residualDisk1Ring3.png"; residualDisk1Ring3->GetXaxis()->SetTitle("(cm)");   if(residualDisk1Ring3->GetEntries()!=0) Ca7->SetLogy();   Ca7->SaveAs(labeltoSave.c_str()); Ca7->Clear();
  residualDisk2Ring2->Draw();  labeltoSave = "resEndCap/residualDisk2Ring2.png"; residualDisk2Ring2->GetXaxis()->SetTitle("(cm)");   if(residualDisk2Ring2->GetEntries()!=0) Ca7->SetLogy();   Ca7->SaveAs(labeltoSave.c_str()); Ca7->Clear();
  residualDisk2Ring3->Draw();  labeltoSave = "resEndCap/residualDisk2Ring3.png"; residualDisk2Ring3->GetXaxis()->SetTitle("(cm)");   if(residualDisk2Ring3->GetEntries()!=0) Ca7->SetLogy();   Ca7->SaveAs(labeltoSave.c_str()); Ca7->Clear();
  residualDisk3Ring2->Draw();  labeltoSave = "resEndCap/residualDisk3Ring2.png"; residualDisk3Ring2->GetXaxis()->SetTitle("(cm)");   if(residualDisk3Ring2->GetEntries()!=0) Ca7->SetLogy();   Ca7->SaveAs(labeltoSave.c_str()); Ca7->Clear();
  residualDisk3Ring3->Draw();  labeltoSave = "resEndCap/residualDisk3Ring3.png"; residualDisk3Ring3->GetXaxis()->SetTitle("(cm)");   if(residualDisk3Ring3->GetEntries()!=0) Ca7->SetLogy();   Ca7->SaveAs(labeltoSave.c_str()); Ca7->Clear();

  if(debug) std::cout<<"Closing Canvas and deleting"<<std::endl;

  Ca7->Close();
  
  delete residualRB1in;
  delete residualRB1out;
  delete residualRB2in;
  delete residualRB2out;
  delete residualRB3;
  delete residualRB4;   
     
  delete residualStation1Ring2_A;
  delete residualStation1Ring2_B;
  delete residualStation1Ring2_C;
  delete residualStation1Ring3_A;
  delete residualStation1Ring3_B;
  delete residualStation1Ring3_C;
  
  delete residualStation23Ring2_A;
  delete residualStation23Ring2_B;
  delete residualStation23Ring2_C;
  delete residualStation23Ring3_A;
  delete residualStation23Ring3_B;
  delete residualStation23Ring3_C;

  delete residualStation123Ring3_A;
  delete residualStation123Ring3_B;
  delete residualStation123Ring3_C;

  delete residualDiskm1Ring2;
  delete residualDiskm1Ring3;
  delete residualDiskm2Ring2;
  delete residualDiskm2Ring3;
  delete residualDiskm3Ring2;
  delete residualDiskm3Ring3;
  
  delete residualDisk1Ring2;
  delete residualDisk1Ring3;
  delete residualDisk2Ring2;
  delete residualDisk2Ring3;
  delete residualDisk3Ring2;
  delete residualDisk3Ring3;

  std::cout<<"Writting Inegral Muographies and deleting all."<<std::endl;
  
  for(int m =0; m<48;m++){
    if(debug) std::cout<<"m="<<m<<std::endl;
    if(!IntegralMuography[m] || !IntegralMuographyExp[m] || !IntegralMuographyObs[m]){
      for(int i=1;i<=IntegralMuography[m]->GetXaxis()->GetNbins();++i){
	for(int j=1;j<=IntegralMuography[m]->GetYaxis()->GetNbins();++j){
	  if(debug) std::cout<<"in (i,j) "<<i<<" "<<j<<std::endl;
	  float ef2D=0.;
	  float er2D=0.; //ojo con esto en DQMOffline podria ser ef2D en lugar de er2D
	  if(IntegralMuographyExp[m]->GetBinContent(i,j) != 0){
	    ef2D =IntegralMuographyObs[m]->GetBinContent(i,j)/IntegralMuographyExp[m]->GetBinContent(i,j);
	    er2D = sqrt(ef2D*(1-ef2D)/IntegralMuographyExp[m]->GetBinContent(i,j));
	    IntegralMuography[m]->SetBinContent(i,j,ef2D*100.);
	    IntegralMuography[m]->SetBinError(i,j,er2D*100.);
	  }
	}//loop on the boxes
      }
      IntegralMuography[m]->Write(); 
      IntegralMuographyObs[m]->Write(); 
      IntegralMuographyExp[m]->Write(); 
      
      delete IntegralMuography[m]; 
      delete IntegralMuographyObs[m]; 
      delete IntegralMuographyExp[m]; 
    }else{
      std::cout<<"Warning Integral Muography do not exist for m="<<m<<std::endl;
    }
  }

  std::stringstream nametosave;
  nametosave.str("");


  if(debug) std::cout<<"Doing Cluster Size Per Sector"<<std::endl;

  for(k=1;k<=12;k++){
    sectorCLSWm2[k]->Draw(); sectorCLSWm2[k]->GetXaxis()->SetTitle("Cluster Size");
    nametosave<<"CLS/Wm2sector"<<k<<".png"; Ca5->SaveAs(nametosave.str().c_str()); nametosave.str("");
    sectorCLSWm2[k]->Write();
    Ca5->Clear();
    sectorCLSWm1[k]->Draw(); sectorCLSWm1[k]->GetXaxis()->SetTitle("Cluster Size"); 
    nametosave<<"CLS/Wm1sector"<<k<<".png"; Ca5->SaveAs(nametosave.str().c_str()); nametosave.str("");
    sectorCLSWm1[k]->Write();
    Ca5->Clear(); 
    sectorCLSW0[k]->Draw(); sectorCLSW0[k]->GetXaxis()->SetTitle("Cluster Size"); 
    nametosave<<"CLS/W0sector"<<k<<".png"; Ca5->SaveAs(nametosave.str().c_str()); nametosave.str("");
    sectorCLSW0[k]->Write();
    Ca5->Clear(); 
    sectorCLSW1[k]->Draw(); sectorCLSW1[k]->GetXaxis()->SetTitle("Cluster Size"); 
    nametosave<<"CLS/W1sector"<<k<<".png"; Ca5->SaveAs(nametosave.str().c_str()); nametosave.str("");
    sectorCLSW1[k]->Write();
    Ca5->Clear(); 
    sectorCLSW2[k]->Draw(); sectorCLSW2[k]->GetXaxis()->SetTitle("Cluster Size"); 
    nametosave<<"CLS/W2sector"<<k<<".png"; Ca5->SaveAs(nametosave.str().c_str()); nametosave.str("");
    sectorCLSW2[k]->Write(); 
    Ca5->Clear(); 
  }
  
  nametosave.str("");

  if(debug) std::cout<<"Doing Cluster Size Per Layer"<<std::endl;

  for(k=1;k<=6;k++){
    layerCLSWm2[k]->Draw(); layerCLSWm2[k]->GetXaxis()->SetTitle("Cluster Size"); 
    nametosave<<"CLS/Wm2layer"<<k<<".png"; Ca5->SaveAs(nametosave.str().c_str()); nametosave.str("");
    layerCLSWm2[k]->Write(); 
    Ca5->Clear(); 

    layerCLSWm1[k]->Draw(); layerCLSWm1[k]->GetXaxis()->SetTitle("Cluster Size"); 
    nametosave<<"CLS/Wm1layer"<<k<<".png"; Ca5->SaveAs(nametosave.str().c_str()); nametosave.str("");
    layerCLSWm1[k]->Write();
    Ca5->Clear(); 

    layerCLSW0[k]->Draw(); layerCLSW0[k]->GetXaxis()->SetTitle("Cluster Size"); 
    nametosave<<"CLS/W0layer"<<k<<".png"; Ca5->SaveAs(nametosave.str().c_str()); nametosave.str("");
    layerCLSW0[k]->Write();
    Ca5->Clear(); 

    layerCLSW1[k]->Draw(); layerCLSW1[k]->GetXaxis()->SetTitle("Cluster Size"); 
    nametosave<<"CLS/W1layer"<<k<<".png"; Ca5->SaveAs(nametosave.str().c_str()); nametosave.str("");
    layerCLSW1[k]->Write();
    Ca5->Clear(); 

    layerCLSW2[k]->Draw(); layerCLSW2[k]->GetXaxis()->SetTitle("Cluster Size"); 
    nametosave<<"CLS/W2layer"<<k<<".png"; Ca5->SaveAs(nametosave.str().c_str()); nametosave.str("");
    layerCLSW2[k]->Write();
    Ca5->Clear(); 
    
    std::stringstream Id; Id.str(""); Id<<"ClusterSizeForLayer"<<k;
    std::stringstream title; title.str(""); title<<"Cluster Size For Layer "<<k;

    CLSlayer[k] = new TH1F (Id.str().c_str(),title.str().c_str(),10,0.5,10.5);
    CLSlayer[k]->Add(layerCLSWm2[k]);
    CLSlayer[k]->Add(layerCLSWm1[k]);
    CLSlayer[k]->Add(layerCLSW0[k]);
    CLSlayer[k]->Add(layerCLSW1[k]);
    CLSlayer[k]->Add(layerCLSW2[k]);

    CLSlayer[k]->Draw(); CLSlayer[k]->GetXaxis()->SetTitle(title.str().c_str()); 

    nametosave<<"CLS/ClusterSizeForLayer"<<k<<".png"; Ca5->SaveAs(nametosave.str().c_str()); nametosave.str("");
    CLSlayer[k]->Write();
    Ca5->Clear(); 
  }

  CLS = new TH1F ("CLS","Cluster Size for all the barrel",10,0.5,10.5);

  for(k=1;k<=6;k++) CLS->Add(CLSlayer[k]);

  if(debug) std::cout<<"Doing Cluster Size for each sector"<<std::endl;

  
  if(debug) std::cout<<"Cluster Size For the barrel"<<std::endl;
  
  Ca5->Clear();    
  CLS->Draw(); CLS->GetXaxis()->SetTitle("Cluster Size For The Barrel");
  Ca5->SaveAs("CLS/CLS.png"); CLS->Write(); 
  Ca5->Clear(); 
    

  if(debug) std::cout<<"Cluster Size For Wm2"<<std::endl;

  Ca6->Clear();

  CLSWm2->Draw();  CLSWm2->GetXaxis()->SetTitle("Sector"); CLSWm2->GetYaxis()->SetTitle("Cluster Size");
  Ca6->SaveAs("CLS/Wm2.png"); 
  CLSWm2->Write(); 
  Ca6->Clear(); 

  if(debug) std::cout<<"Cluster Size For Wm1"<<std::endl;

  CLSWm1->Draw(); CLSWm1->GetXaxis()->SetTitle("Sector"); CLSWm1->GetYaxis()->SetTitle("Cluster Size");
  Ca6->SaveAs("CLS/Wm1.png"); CLSWm1->Write();
  Ca6->Clear(); 
  
  if(debug) std::cout<<"Cluster Size For W0"<<std::endl;

  CLSW0->Draw(); CLSW0->GetXaxis()->SetTitle("Sector"); CLSW0->GetYaxis()->SetTitle("Cluster Size");
  Ca6->SaveAs("CLS/W0.png"); CLSW0->Write();
  Ca6->Clear(); 

  if(debug) std::cout<<"Cluster Size For W1"<<std::endl;

  CLSW1->Draw(); CLSW1->GetXaxis()->SetTitle("Sector"); CLSW1->GetYaxis()->SetTitle("Cluster Size");
  Ca6->SaveAs("CLS/W1.png"); CLSW1->Write();
  Ca6->Clear(); 

  if(debug) std::cout<<"Cluster Size For W2"<<std::endl;

  CLSW2->Draw(); CLSW2->GetXaxis()->SetTitle("Sector"); CLSW2->GetYaxis()->SetTitle("Cluster Size");
  Ca6->SaveAs("CLS/W2.png"); CLSW2->Write();
  Ca6->Clear(); 


  if(debug) std::cout<<"Cluster Size For the EndCap"<<std::endl;
 

  CLSStation1Ring2_A->Draw(); CLSStation1Ring2_A->GetXaxis()->SetTitle("CLSStation1Ring2_A");  CLSStation1Ring2_A->GetYaxis()->SetTitle("Cluster Size"); Ca6->SaveAs("CLS/CLSStation1Ring2_A.png");  CLSStation1Ring2_A->Write();  Ca6->Clear(); 
  CLSStation1Ring2_B->Draw(); CLSStation1Ring2_B->GetXaxis()->SetTitle("CLSStation1Ring2_B");  CLSStation1Ring2_B->GetYaxis()->SetTitle("Cluster Size"); Ca6->SaveAs("CLS/CLSStation1Ring2_B.png");  CLSStation1Ring2_B->Write();  Ca6->Clear(); 
  CLSStation1Ring2_C->Draw(); CLSStation1Ring2_C->GetXaxis()->SetTitle("CLSStation1Ring2_C");  CLSStation1Ring2_C->GetYaxis()->SetTitle("Cluster Size"); Ca6->SaveAs("CLS/CLSStation1Ring2_C.png");  CLSStation1Ring2_C->Write();  Ca6->Clear(); 

  CLSStation23Ring2_A->Draw(); CLSStation23Ring2_A->GetXaxis()->SetTitle("CLSStation23Ring2_A");  CLSStation23Ring2_A->GetYaxis()->SetTitle("Cluster Size"); Ca6->SaveAs("CLS/CLSStation23Ring2_A.png");  CLSStation23Ring2_A->Write();  Ca6->Clear(); 
  CLSStation23Ring2_B->Draw(); CLSStation23Ring2_B->GetXaxis()->SetTitle("CLSStation23Ring2_B");  CLSStation23Ring2_B->GetYaxis()->SetTitle("Cluster Size"); Ca6->SaveAs("CLS/CLSStation23Ring2_B.png");  CLSStation23Ring2_B->Write();  Ca6->Clear(); 
  CLSStation23Ring2_C->Draw(); CLSStation23Ring2_C->GetXaxis()->SetTitle("CLSStation23Ring2_C");  CLSStation23Ring2_C->GetYaxis()->SetTitle("Cluster Size"); Ca6->SaveAs("CLS/CLSStation23Ring2_C.png");  CLSStation23Ring2_C->Write();  Ca6->Clear(); 

  CLSStation123Ring3_A->Draw(); CLSStation123Ring3_A->GetXaxis()->SetTitle("CLSStation123Ring3_A");  CLSStation123Ring3_A->GetYaxis()->SetTitle("Cluster Size"); Ca6->SaveAs("CLS/CLSStation123Ring3_A.png");  CLSStation123Ring3_A->Write();  Ca6->Clear(); 
  CLSStation123Ring3_B->Draw(); CLSStation123Ring3_B->GetXaxis()->SetTitle("CLSStation123Ring3_B");  CLSStation123Ring3_B->GetYaxis()->SetTitle("Cluster Size"); Ca6->SaveAs("CLS/CLSStation123Ring3_B.png");  CLSStation123Ring3_B->Write();  Ca6->Clear(); 
  CLSStation123Ring3_C->Draw(); CLSStation123Ring3_C->GetXaxis()->SetTitle("CLSStation123Ring3_C");  CLSStation123Ring3_C->GetYaxis()->SetTitle("Cluster Size"); Ca6->SaveAs("CLS/CLSStation123Ring3_C.png");  CLSStation123Ring3_C->Write();  Ca6->Clear(); 

  Ca6->Close();

  if(debug) std::cout<<"Doing Greg Histograms Images"<<std::endl;
  
  Ca5->Clear(); 

  sectorEffWm2->Draw(); sectorEffWm2->GetXaxis()->SetTitle("Sector");  sectorEffWm2->GetYaxis()->SetRangeUser(0.,1.);
  Ca5->SaveAs("Greg/Wm2.png"); sectorEffWm2->Write();
  Ca5->Clear(); 

  sectorEffWm1->Draw(); sectorEffWm1->GetXaxis()->SetTitle("Sector");  sectorEffWm1->GetYaxis()->SetRangeUser(0.,1.);
  Ca5->SaveAs("Greg/Wm1.png"); sectorEffWm1->Write(); 
  Ca5->Clear(); 

  sectorEffW0->Draw(); sectorEffW0->GetXaxis()->SetTitle("Sector");  sectorEffW0->GetYaxis()->SetRangeUser(0.,1.);
  Ca5->SaveAs("Greg/W0.png"); sectorEffW0->Write();
  Ca5->Clear(); 

  sectorEffW1->Draw(); sectorEffW1->GetXaxis()->SetTitle("Sector");  sectorEffW1->GetYaxis()->SetRangeUser(0.,1.);
  Ca5->SaveAs("Greg/W1.png"); sectorEffW1->Write();
  Ca5->Clear(); 

  sectorEffW2->Draw(); sectorEffW2->GetXaxis()->SetTitle("Sector");  sectorEffW2->GetYaxis()->SetRangeUser(0.,1.);
  Ca5->SaveAs("Greg/W2.png"); sectorEffW2->Write();
  Ca5->Clear(); 

  HeightVsEffR3->Draw(); HeightVsEffR3->GetXaxis()->SetTitle("Efficiency");HeightVsEffR3->GetYaxis()->SetTitle("Height(R units)"); HeightVsEffR3->GetYaxis()->SetRangeUser(-1.,1.); HeightVsEffR3->GetXaxis()->SetRangeUser(0.,1.);
  HeightVsEffR3->SetMarkerColor(4);
  HeightVsEffR3->SetMarkerStyle(20);
  HeightVsEffR3->SetMarkerSize(0.7);
  
  Ca5->SaveAs("Greg/HeightVsEffR3.png");  HeightVsEffR3->Write();
  Ca5->Clear(); 

  if(debug) std::cout<<"Doing Summary Plot per wheel "<<std::endl;

  EfficiencyPerRing->Draw();
  EfficiencyPerRing->SetMaximum(1.);
  EfficiencyPerRing->SetMinimum(0.);
  //EfficiencyPerRing->GetXaxis()->LabelsOption("v");
  //Ca5->SetBottomMargin(0.3);
  Ca5->SaveAs("Greg/EfficiencyPerRing.png"); EfficiencyPerRing->Write();
  Ca5->Clear();

  StatisticsPerRing->Draw();
  StatisticsPerRing->Write();
  
  HeightVsEffR2->Draw(); HeightVsEffR2->GetXaxis()->SetTitle("Efficiency");HeightVsEffR2->GetYaxis()->SetTitle("Height(R units)"); HeightVsEffR2->GetYaxis()->SetRangeUser(-1.,1.); HeightVsEffR2->GetXaxis()->SetRangeUser(0.,1.);
  HeightVsEffR2->SetMarkerColor(4);
  HeightVsEffR2->SetMarkerStyle(20);
  HeightVsEffR2->SetMarkerSize(0.7);
  
  Ca5->SaveAs("Greg/HeightVsEffR2.png"); HeightVsEffR2->Write();
  Ca5->Clear(); 
  
  //Positive Endcap
  
  GregD1R2->Draw(); GregD1R2->GetXaxis()->SetTitle("Chamber"); GregD1R2->GetYaxis()->SetRangeUser(-0.01,1.01);
  GregD1R2_black->SetLineColor(kRed);
  GregD1R2_black->SetMarkerSize(1.0);
  GregD1R2_black->SetMarkerStyle(21);
  std::cout<<"drawing first black histogram"<<std::endl;
  GregD1R2_black->Draw("same");
  Ca5->SaveAs("Greg/D1R2.png");
  Ca5->Clear(); 
  
  GregD1R3->Draw(); GregD1R3->GetXaxis()->SetTitle("Chamber");  GregD1R3->GetYaxis()->SetRangeUser(-0.01,1.01);
  GregD1R3_black->SetLineColor(kRed);
  std::cout<<"drawing second black histogram"<<std::endl;
  GregD1R3_black->Draw("same");
  GregD1R3_black->SetMarkerSize(1.0);
  GregD1R3_black->SetMarkerStyle(21);
  Ca5->SaveAs("Greg/D1R3.png");
  Ca5->Clear(); 
  
  GregD2R2->Draw(); GregD2R2->GetXaxis()->SetTitle("Chamber");GregD2R2->GetYaxis()->SetRangeUser(-0.01,1.01);
  GregD2R2_black->SetLineColor(kRed);
  GregD2R2_black->Draw("same");
  GregD2R2_black->SetMarkerSize(1.0);
  GregD2R2_black->SetMarkerStyle(21);
  Ca5->SaveAs("Greg/D2R2.png");
  Ca5->Clear(); 
  
  GregD2R3->Draw(); GregD2R3->GetXaxis()->SetTitle("Chamber"); GregD2R3->GetYaxis()->SetRangeUser(-0.01,1.01);
  GregD2R3_black->SetLineColor(kRed);
  GregD2R3_black->Draw("same");
  GregD2R3_black->SetMarkerSize(1.0);
  GregD2R3_black->SetMarkerStyle(21);
  Ca5->SaveAs("Greg/D2R3.png");
  Ca5->Clear(); 
  
  GregD3R2->Draw(); GregD3R2->GetXaxis()->SetTitle("Chamber");GregD3R2->GetYaxis()->SetRangeUser(-0.01,1.01);
  GregD3R2_black->SetLineColor(kRed);
  std::cout<<"drawing fifth black histogram"<<std::endl;
  GregD3R2_black->Draw("same");
  GregD3R2_black->SetMarkerSize(1.0);
  GregD3R2_black->SetMarkerStyle(21);
  Ca5->SaveAs("Greg/D3R2.png");
  Ca5->Clear(); 
  
  GregD3R3->Draw(); GregD3R3->GetXaxis()->SetTitle("Chamber");GregD3R3->GetYaxis()->SetRangeUser(-0.01,1.01);
  GregD3R3_black->SetLineColor(kRed);
  std::cout<<"drawing sixth black histogram"<<std::endl;
  GregD3R3_black->Draw("same");
  GregD3R3_black->SetMarkerStyle(21);
  GregD3R3_black->SetMarkerSize(1.0);
  Ca5->SaveAs("Greg/D3R3.png");
  Ca5->Clear(); 

  //Negative Endcap

  GregDm1R2->Draw(); GregDm1R2->GetXaxis()->SetTitle("Chamber"); GregDm1R2->GetYaxis()->SetRangeUser(-0.01,1.01);
  GregDm1R2_black->SetLineColor(kRed);
  GregDm1R2_black->Draw("same");
  GregDm1R2_black->SetMarkerStyle(21);
  GregDm1R2_black->SetMarkerSize(1.0);
  std::cout<<"drawing second negative endcap black histogram"<<std::endl;
  Ca5->SaveAs("Greg/Dm1R2.png");
  Ca5->Clear(); 
  
  GregDm1R3->Draw(); GregDm1R3->GetXaxis()->SetTitle("Chamber");  GregDm1R3->GetYaxis()->SetRangeUser(-0.01,1.01);
  GregDm1R3_black->SetLineColor(kRed);
  GregDm1R3_black->Draw("same");
  GregDm1R3_black->SetMarkerStyle(21);
  GregDm1R3_black->SetMarkerSize(1.0);
  std::cout<<"drawing second negative endcap black histogram"<<std::endl;
  Ca5->SaveAs("Greg/Dm1R3.png");
  Ca5->Clear(); 
  
  GregDm2R2->Draw(); GregDm2R2->GetXaxis()->SetTitle("Chamber");GregDm2R2->GetYaxis()->SetRangeUser(-0.01,1.01);
  GregDm2R2_black->SetLineColor(kRed);
  GregDm2R2_black->Draw("same");
  GregDm2R2_black->SetMarkerStyle(21);
  GregDm2R2_black->SetMarkerSize(1.0);
  Ca5->SaveAs("Greg/Dm2R2.png");
  Ca5->Clear(); 
  
  GregDm2R3->Draw(); GregDm2R3->GetXaxis()->SetTitle("Chamber"); GregDm2R3->GetYaxis()->SetRangeUser(-0.01,1.01);
  GregDm2R3_black->SetLineColor(kRed);
  GregDm2R3_black->Draw("same");
  GregDm2R3_black->SetMarkerStyle(21);
  GregDm2R3_black->SetMarkerSize(1.0);
  Ca5->SaveAs("Greg/Dm2R3.png");
  Ca5->Clear();
  
  GregDm3R2->Draw(); GregDm3R2->GetXaxis()->SetTitle("Chamber");GregDm3R2->GetYaxis()->SetRangeUser(-0.01,1.01);
  GregDm3R2_black->SetLineColor(kRed);
  GregDm3R2_black->Draw("same");
  GregDm3R2_black->SetMarkerStyle(21);
  GregDm3R2_black->SetMarkerSize(1.0);
  Ca5->SaveAs("Greg/Dm3R2.png");
  Ca5->Clear(); 
  
  GregDm3R3->Draw(); GregDm3R3->GetXaxis()->SetTitle("Chamber");GregDm3R3->GetYaxis()->SetRangeUser(-0.01,1.01);
  GregDm3R3_black->SetLineColor(kRed);
  GregDm3R3_black->Draw("same");
  GregDm3R3_black->SetMarkerStyle(21);
  GregDm3R3_black->SetMarkerSize(1.0);
  Ca5->SaveAs("Greg/Dm3R3.png");
  Ca5->Clear(); 

  if(debug)std::cout<<"Deleting Greg histograms"<<std::endl;
  
  if(debug)std::cout<<"After Deleting Greg histograms"<<std::endl;

  EfficiencyPerChamberNumber->Draw(); EfficiencyPerChamberNumber->GetXaxis()->SetTitle("Chamber");EfficiencyPerChamberNumber->GetYaxis()->SetRangeUser(0.,1.);
  Ca5->SaveAs("Greg/EfficiencyPerChamberNumber.png"); EfficiencyPerChamberNumber->Write();
  Ca5->Clear(); 

  int colorPalette3[20];

  colorPalette3[0]= 632; // 0 red 
  colorPalette3[1]= 632; // 5 red
  colorPalette3[2]= 632; // 10 
  colorPalette3[3]= 632; // 15 
  colorPalette3[4]= 632; // 20 
  colorPalette3[5]= 632; // 25 
  colorPalette3[6]= 632; // 30 
  colorPalette3[7]= 632; // 35
  colorPalette3[8]= 632; // 40
  colorPalette3[9]= 632; // 45
  colorPalette3[10]= 632; // 50
  colorPalette3[11]= 632; // 55
  colorPalette3[12]= 632; // 60
  colorPalette3[13]= 632; // 65
  colorPalette3[14]= 807; // 70
  colorPalette3[15]= 807; // 75
  colorPalette3[16]= 400; // 80
  colorPalette3[17]= 400; // 85
  colorPalette3[18]= 400; // 90 
  colorPalette3[19]= 416; // 95 % green 
  
  //gStyle->SetPalette(20,colorPalette3);

  gStyle->SetPalette(1);

  Diskm3Summary->SetMinimum(0);  
  Diskm3Summary->SetMaximum(100);
  Diskm3Summary->Draw(); Diskm3Summary->GetXaxis()->SetTitle("Chamber");
  Diskm3Summary->SetDrawOption("COLZ");
  Ca5->SaveAs("Pigi/Diskm3Summary.png"); 
  Ca5->Clear();

  Diskm2Summary->SetMinimum(0); 
  Diskm2Summary->SetMaximum(100);
  Diskm2Summary->Draw(); Diskm2Summary->GetXaxis()->SetTitle("Chamber");
  Diskm2Summary->SetDrawOption("COLZ");
  Ca5->SaveAs("Pigi/Diskm2Summary.png");  
  Ca5->Clear();

  Diskm1Summary->SetMinimum(0); 
  Diskm1Summary->SetMaximum(100);
  Diskm1Summary->Draw(); Diskm1Summary->GetXaxis()->SetTitle("Chamber");
  Diskm1Summary->SetDrawOption("COLZ");
  Ca5->SaveAs("Pigi/Diskm1Summary.png"); 
  Ca5->Clear();
  
  Disk3Summary->SetMinimum(0); 
  Disk3Summary->SetMaximum(100);
  Disk3Summary->Draw(); Disk3Summary->GetXaxis()->SetTitle("Chamber");
  Disk3Summary->SetDrawOption("COLZ");
  Ca5->SaveAs("Pigi/Disk3Summary.png"); 
  Ca5->Clear();
  
  Disk2Summary->SetMinimum(0); 
  Disk2Summary->SetMaximum(100);
  Disk2Summary->Draw(); Disk2Summary->GetXaxis()->SetTitle("Chamber");
  Disk2Summary->SetDrawOption("COLZ");
  Ca5->SaveAs("Pigi/Disk2Summary.png"); 
  Ca5->Clear();
  
  Disk1Summary->SetMinimum(0); 
  Disk1Summary->SetMaximum(100);
  Disk1Summary->Draw(); Disk1Summary->GetXaxis()->SetTitle("Chamber");
  Disk1Summary->SetDrawOption("COLZ");
  Ca5->SaveAs("Pigi/Disk1Summary.png"); 
  Ca5->Clear();
  
  PositiveEndCapSummary->SetMinimum(0); 
  PositiveEndCapSummary->SetMaximum(100);
  PositiveEndCapSummary->Draw(); PositiveEndCapSummary->GetXaxis()->SetTitle("Chamber");
  PositiveEndCapSummary->SetDrawOption("COLZ");
  Ca5->SaveAs("Pigi/PositiveEndCapSummary.png"); 
  Ca5->Clear();

  NegativeEndCapSummary->SetMinimum(0); 
  NegativeEndCapSummary->SetMaximum(100);
  NegativeEndCapSummary->Draw(); NegativeEndCapSummary->GetXaxis()->SetTitle("Chamber");
  NegativeEndCapSummary->SetDrawOption("COLZ");
  Ca5->SaveAs("Pigi/NegativeEndCapSummary.png"); 
  Ca5->Clear();
  
  Wheelm2Summary->SetMinimum(0);  
  Wheelm2Summary->SetMaximum(100);
  Wheelm2Summary->Draw(); Wheelm2Summary->GetXaxis()->SetTitle("Sector");
  Wheelm2Summary->SetDrawOption("COLZ");
  Ca5->SaveAs("Pigi/Wheelm2Summary.png");  
  Ca5->Clear();
  
  Wheelm1Summary->SetMinimum(0);  
  Wheelm1Summary->SetMaximum(100);
  Wheelm1Summary->Draw(); Wheelm1Summary->GetXaxis()->SetTitle("Sector");
  Wheelm1Summary->SetDrawOption("COLZ");
  Ca5->SaveAs("Pigi/Wheelm1Summary.png");  
  Ca5->Clear();
  
  Wheel0Summary->SetMinimum(0);  
  Wheel0Summary->SetMaximum(100);
  Wheel0Summary->Draw(); Wheel0Summary->GetXaxis()->SetTitle("Sector");
  Wheel0Summary->SetDrawOption("COLZ");
  Ca5->SaveAs("Pigi/Wheel0Summary.png");  
  Ca5->Clear();
  
  Wheel1Summary->SetMinimum(0);  
  Wheel1Summary->SetMaximum(100);
  Wheel1Summary->Draw(); Wheel1Summary->GetXaxis()->SetTitle("Sector");
  Wheel1Summary->SetDrawOption("COLZ");
  Ca5->SaveAs("Pigi/Wheel1Summary.png");   
  Ca5->Clear();
  
  Wheel2Summary->SetMinimum(0);  
  Wheel2Summary->SetMaximum(100);
  Wheel2Summary->Draw(); Wheel2Summary->GetXaxis()->SetTitle("Sector");
  Wheel2Summary->SetDrawOption("COLZ");
  Ca5->SaveAs("Pigi/Wheel2Summary.png"); 
  Ca5->Clear();

  Ca5->Close();

  Ca9->Clear();
  MeanResiduals->Draw(); MeanResiduals->GetXaxis()->SetTitle("cm");
  Ca9->SaveAs("MeanResiduals.png");  
  Ca9->Clear();
  
  MeanResiduals11->Draw(); MeanResiduals11->GetXaxis()->SetTitle("cm");
  Ca9->SaveAs("MeanResiduals11.png");  
  Ca9->Clear();

  AllResidualsBarrel->Draw(); AllResidualsBarrel->GetXaxis()->SetTitle("cm");
  AllResidualsBarrel->Write();
  Ca9->SaveAs("AllResidualsBarrel.png"); 
  Ca9->Clear();

  AllResidualsEndCap->Draw(); AllResidualsEndCap->GetXaxis()->SetTitle("cm");
  AllResidualsEndCap->Write();
  Ca9->SaveAs("AllResidualsEndCap.png"); 
  Ca9->Clear();

  RMSResiduals->Draw(); RMSResiduals->GetXaxis()->SetTitle("cm");
  Ca9->SaveAs("RMSResiduals.png");  
  Ca9->Clear();
  
  RMSResiduals11->Draw(); RMSResiduals11->GetXaxis()->SetTitle("cm");
  Ca9->SaveAs("RMSResiduals11.png");
  Ca9->Clear();

  Ca9->Close();

  if(endcap){
    
    Ca2->Clear();
   
    EffGlobDm3->Draw();
    EffGlobDm3->GetYaxis()->SetTitle("%");
   
    BXGlobDm3->SetMarkerColor(9);
    BXGlobDm3->SetLineColor(9);
    BXGlobDm3->Draw("same");
   
    MaskedGlobDm3->SetMarkerColor(2);
    MaskedGlobDm3->SetLineColor(2);
    MaskedGlobDm3->Draw("same");
   
    AverageEffDm3->SetMarkerColor(8);
    AverageEffDm3->SetLineColor(8);
    AverageEffDm3->Draw("same");
   
    NoPredictionDm3->SetMarkerColor(5);
    NoPredictionDm3->SetLineColor(5);
    NoPredictionDm3->Draw("same");
   
    pave->Draw();

    bxAxisEndCap->Draw("same");
  
    Ca2->SaveAs("Sides/SegEff_Dm3near.png");
    Ca2->Clear();


    EffGlobDm2->Draw();
    EffGlobDm2->GetYaxis()->SetTitle("%");
  
    BXGlobDm2->SetMarkerColor(9);
    BXGlobDm2->SetLineColor(9);
    BXGlobDm2->Draw("same");

    MaskedGlobDm2->SetMarkerColor(2);
    MaskedGlobDm2->SetLineColor(2);
    MaskedGlobDm2->Draw("same");

    AverageEffDm2->SetMarkerColor(8);
    AverageEffDm2->SetLineColor(8);
    AverageEffDm2->Draw("same");

    NoPredictionDm2->SetMarkerColor(5);
    NoPredictionDm2->SetLineColor(5);
    NoPredictionDm2->Draw("same");

    pave->Draw();

    bxAxisEndCap->Draw("same");
  
    Ca2->SaveAs("Sides/SegEff_Dm2near.png");
    Ca2->Clear();

    EffGlobDm1->Draw();
    EffGlobDm1->GetYaxis()->SetTitle("%");
  
    BXGlobDm1->SetMarkerColor(9);
    BXGlobDm1->SetLineColor(9);
    BXGlobDm1->Draw("same");

    MaskedGlobDm1->SetMarkerColor(2);
    MaskedGlobDm1->SetLineColor(2);
    MaskedGlobDm1->Draw("same");

    AverageEffDm1->SetMarkerColor(8);
    AverageEffDm1->SetLineColor(8);
    AverageEffDm1->Draw("same");

    NoPredictionDm1->SetMarkerColor(5);
    NoPredictionDm1->SetLineColor(5);
    NoPredictionDm1->Draw("same");

    pave->Draw();

    bxAxisEndCap->Draw("same");
  
    Ca2->SaveAs("Sides/SegEff_Dm1near.png");
    Ca2->Clear();
  }
 
 //Barrel
 if(barrel){
   
   Ca2->Clear();

   EffGlobWm2->Draw();
   EffGlobWm2->GetYaxis()->SetTitle("%");
  
   BXGlobWm2->SetMarkerColor(9);
   BXGlobWm2->SetLineColor(9);
   BXGlobWm2->Draw("same");

   Signal_BXGlobWm2->SetMarkerColor(2);
   Signal_BXGlobWm2->SetLineColor(2);
   Signal_BXGlobWm2->Draw("same");
   
   MaskedGlobWm2->SetMarkerColor(2);
   MaskedGlobWm2->SetLineColor(2);
   MaskedGlobWm2->Draw("same");

   AverageEffWm2->SetMarkerColor(8);
   AverageEffWm2->SetLineColor(8);
   AverageEffWm2->Draw("same");

   DoubleGapWm2->SetMarkerColor(6);
   DoubleGapWm2->SetLineColor(6);
   DoubleGapWm2->Draw("same");

   NoPredictionWm2->SetMarkerColor(5);
   NoPredictionWm2->SetLineColor(5);
   NoPredictionWm2->Draw("same");

   pave->Draw();
 
   bxAxis->Draw("same");
  
   Ca2->SaveAs("Sides/SegEff_Wm2near.png");
   Ca2->Clear();

   EffGlobWm2far->Draw();
   EffGlobWm2far->GetYaxis()->SetTitle("%");
  
   BXGlobWm2far->SetMarkerColor(9);
   BXGlobWm2far->SetLineColor(9);
   BXGlobWm2far->Draw("same");

   Signal_BXGlobWm2far->SetMarkerColor(2);
   Signal_BXGlobWm2far->SetLineColor(2);
   Signal_BXGlobWm2far->Draw("same");
   
   MaskedGlobWm2far->SetMarkerColor(2);
   MaskedGlobWm2far->SetLineColor(2);
   MaskedGlobWm2far->Draw("same");

   AverageEffWm2far->SetMarkerColor(8);
   AverageEffWm2far->SetLineColor(8);
   AverageEffWm2far->Draw("same");

   DoubleGapWm2far->SetMarkerColor(6);
   DoubleGapWm2far->SetLineColor(6);
   DoubleGapWm2far->Draw("same");

   NoPredictionWm2far->SetMarkerColor(5);
   NoPredictionWm2far->SetLineColor(5);
   NoPredictionWm2far->Draw("same");

   pave->Draw();

   bxAxisFar->Draw("same");
  
   Ca2->SaveAs("Sides/SegEff_Wm2far.png");
   Ca2->Clear();

   EffGlobWm1->Draw();
   EffGlobWm1->GetYaxis()->SetTitle("%");
  
   BXGlobWm1->SetMarkerColor(9);
   BXGlobWm1->SetLineColor(9);
   BXGlobWm1->Draw("same");

   Signal_BXGlobWm1->SetMarkerColor(2);
   Signal_BXGlobWm1->SetLineColor(2);
   Signal_BXGlobWm1->Draw("same");

   MaskedGlobWm1->SetMarkerColor(2);
   MaskedGlobWm1->SetLineColor(2);
   MaskedGlobWm1->Draw("same");

   AverageEffWm1->SetMarkerColor(8);
   AverageEffWm1->SetLineColor(8);
   AverageEffWm1->Draw("same");

   DoubleGapWm1->SetMarkerColor(6);
   DoubleGapWm1->SetLineColor(6);
   DoubleGapWm1->Draw("same");
   
   NoPredictionWm1->SetMarkerColor(5);
   NoPredictionWm1->SetLineColor(5);
   NoPredictionWm1->Draw("same");
 
   pave->Draw();

   bxAxis->Draw("same");

   Ca2->SaveAs("Sides/SegEff_Wm1near.png");
   Ca2->Clear();

   EffGlobWm1far->Draw();
   EffGlobWm1far->GetYaxis()->SetTitle("%");
  
   BXGlobWm1far->SetMarkerColor(9);
   BXGlobWm1far->SetLineColor(9);
   BXGlobWm1far->Draw("same");

   Signal_BXGlobWm1far->SetMarkerColor(2);
   Signal_BXGlobWm1far->SetLineColor(2);
   Signal_BXGlobWm1far->Draw("same");
   
   MaskedGlobWm1far->SetMarkerColor(2);
   MaskedGlobWm1far->SetLineColor(2);
   MaskedGlobWm1far->Draw("same");

   AverageEffWm1far->SetMarkerColor(8);
   AverageEffWm1far->SetLineColor(8);
   AverageEffWm1far->Draw("same");

   DoubleGapWm1far->SetMarkerColor(6);
   DoubleGapWm1far->SetLineColor(6);
   DoubleGapWm1far->Draw("same");

   NoPredictionWm1far->SetMarkerColor(5);
   NoPredictionWm1far->SetLineColor(5);
   NoPredictionWm1far->Draw("same");
 
   pave->Draw();

   bxAxisFar->Draw("same");

   Ca2->SaveAs("Sides/SegEff_Wm1far.png");
   Ca2->Clear();

   EffGlobW0->Draw();
   EffGlobW0->GetYaxis()->SetTitle("%");
  
   BXGlobW0->SetMarkerColor(9);
   BXGlobW0->SetLineColor(9);
   BXGlobW0->Draw("same");

   Signal_BXGlobW0->SetMarkerColor(2);
   Signal_BXGlobW0->SetLineColor(2);
   Signal_BXGlobW0->Draw("same");

   MaskedGlobW0->SetMarkerColor(2);
   MaskedGlobW0->SetLineColor(2);
   MaskedGlobW0->Draw("same");

   AverageEffW0->SetMarkerColor(8);
   AverageEffW0->SetLineColor(8);
   AverageEffW0->Draw("same");

   DoubleGapW0->SetMarkerColor(6);
   DoubleGapW0->SetLineColor(6);
   DoubleGapW0->Draw("same");

   NoPredictionW0->SetMarkerColor(5);
   NoPredictionW0->SetLineColor(5);
   NoPredictionW0->Draw("same");
 
   pave->Draw();

   bxAxis->Draw("same");

   Ca2->SaveAs("Sides/SegEff_W0near.png");
   Ca2->Clear();

   EffGlobW0far->Draw();
   EffGlobW0far->GetYaxis()->SetTitle("%");
  
   BXGlobW0far->SetMarkerColor(9);
   BXGlobW0far->SetLineColor(9);
   BXGlobW0far->Draw("same");

   Signal_BXGlobW0far->SetMarkerColor(2);
   Signal_BXGlobW0far->SetLineColor(2);
   Signal_BXGlobW0far->Draw("same");

   MaskedGlobW0far->SetMarkerColor(2);
   MaskedGlobW0far->SetLineColor(2);
   MaskedGlobW0far->Draw("same");

   AverageEffW0far->SetMarkerColor(8);
   AverageEffW0far->SetLineColor(8);
   AverageEffW0far->Draw("same");

   DoubleGapW0far->SetMarkerColor(6);
   DoubleGapW0far->SetLineColor(6);
   DoubleGapW0far->Draw("same");

   NoPredictionW0far->SetMarkerColor(5);
   NoPredictionW0far->SetLineColor(5);
   NoPredictionW0far->Draw("same");
 
   pave->Draw();

   bxAxisFar->Draw("same");

   Ca2->SaveAs("Sides/SegEff_W0far.png");
   Ca2->Clear();

   EffGlobW1->Draw();
   EffGlobW1->GetYaxis()->SetTitle("%");
  
   BXGlobW1->SetMarkerColor(9);
   BXGlobW1->SetLineColor(9);
   BXGlobW1->Draw("same");

   Signal_BXGlobW1->SetMarkerColor(2);
   Signal_BXGlobW1->SetLineColor(2);
   Signal_BXGlobW1->Draw("same");

   MaskedGlobW1->SetMarkerColor(2);
   MaskedGlobW1->SetLineColor(2);
   MaskedGlobW1->Draw("same");

   AverageEffW1->SetMarkerColor(8);
   AverageEffW1->SetLineColor(8);
   AverageEffW1->Draw("same");

   DoubleGapW1->SetMarkerColor(6);
   DoubleGapW1->SetLineColor(6);
   DoubleGapW1->Draw("same");

   NoPredictionW1->SetMarkerColor(5);
   NoPredictionW1->SetLineColor(5);
   NoPredictionW1->Draw("same");

   pave->Draw();

   bxAxis->Draw("same");

   Ca2->SaveAs("Sides/SegEff_W1near.png");
   Ca2->Clear();

   EffGlobW1far->Draw();
   EffGlobW1far->GetYaxis()->SetTitle("%");
  
   BXGlobW1far->SetMarkerColor(9);
   BXGlobW1far->SetLineColor(9);
   BXGlobW1far->Draw("same");
   
   Signal_BXGlobW1far->SetMarkerColor(2);
   Signal_BXGlobW1far->SetLineColor(2);
   Signal_BXGlobW1far->Draw("same");

   MaskedGlobW1far->SetMarkerColor(2);
   MaskedGlobW1far->SetLineColor(2);
   MaskedGlobW1far->Draw("same");

   AverageEffW1far->SetMarkerColor(8);
   AverageEffW1far->SetLineColor(8);
   AverageEffW1far->Draw("same");

   DoubleGapW1far->SetMarkerColor(6);
   DoubleGapW1far->SetLineColor(6);
   DoubleGapW1far->Draw("same");

   NoPredictionW1far->SetMarkerColor(5);
   NoPredictionW1far->SetLineColor(5);
   NoPredictionW1far->Draw("same");

   pave->Draw();

   bxAxisFar->Draw("same");

   Ca2->SaveAs("Sides/SegEff_W1far.png");
   Ca2->Clear();

   EffGlobW2->Draw();
   EffGlobW2->GetYaxis()->SetTitle("%");
  
   BXGlobW2->SetMarkerColor(9);
   BXGlobW2->SetLineColor(9);
   BXGlobW2->Draw("same");

   Signal_BXGlobW2->SetMarkerColor(2);
   Signal_BXGlobW2->SetLineColor(2);
   Signal_BXGlobW2->Draw("same");

   MaskedGlobW2->SetMarkerColor(2);
   MaskedGlobW2->SetLineColor(2);
   MaskedGlobW2->Draw("same");

   AverageEffW2->SetMarkerColor(8);
   AverageEffW2->SetLineColor(8);
   AverageEffW2->Draw("same");

   DoubleGapW2->SetMarkerColor(6);
   DoubleGapW2->SetLineColor(6);
   DoubleGapW2->Draw("same");

   NoPredictionW2->SetMarkerColor(5);
   NoPredictionW2->SetLineColor(5);
   NoPredictionW2->Draw("same");

   pave->Draw();
  
   bxAxis->Draw("same");

   Ca2->SaveAs("Sides/SegEff_W2near.png");
   Ca2->Clear();
  
   EffGlobW2far->Draw();
   EffGlobW2far->GetYaxis()->SetTitle("%");
  
   BXGlobW2far->SetMarkerColor(9);
   BXGlobW2far->SetLineColor(9);
   BXGlobW2far->Draw("same");

   Signal_BXGlobW2far->SetMarkerColor(2);
   Signal_BXGlobW2far->SetLineColor(2);
   Signal_BXGlobW2far->Draw("same");

   MaskedGlobW2far->SetMarkerColor(2);
   MaskedGlobW2far->SetLineColor(2);
   MaskedGlobW2far->Draw("same");

   AverageEffW2far->SetMarkerColor(8);
   AverageEffW2far->SetLineColor(8);
   AverageEffW2far->Draw("same");
   
   DoubleGapW2far->SetMarkerColor(6);
   DoubleGapW2far->SetLineColor(6);
   DoubleGapW2far->Draw("same");

   NoPredictionW2far->SetMarkerColor(5);
   NoPredictionW2far->SetLineColor(5);
   NoPredictionW2far->Draw("same");

   pave->Draw();

   bxAxisFar->Draw("same");
  
   Ca2->SaveAs("Sides/SegEff_W2far.png");
   Ca2->Clear();
 }

 //Positive EndCap

 if(endcap){
   //POSITIVE
   EffGlobD1->Draw();
   EffGlobD1->GetYaxis()->SetTitle("%");
  
   BXGlobD1->SetMarkerColor(9);
   BXGlobD1->SetLineColor(9);
   BXGlobD1->Draw("same");

   Signal_BXGlobD1->SetMarkerColor(2);
   Signal_BXGlobD1->SetLineColor(2);
   Signal_BXGlobD1->Draw("same");

   MaskedGlobD1->SetMarkerColor(2);
   MaskedGlobD1->SetLineColor(2);
   MaskedGlobD1->Draw("same");

   AverageEffD1->SetMarkerColor(8);
   AverageEffD1->SetLineColor(8);
   AverageEffD1->Draw("same");

   NoPredictionD1->SetMarkerColor(5);
   NoPredictionD1->SetLineColor(5);
   NoPredictionD1->Draw("same");

   pave->Draw();

   bxAxisEndCap->Draw("same");
  
   Ca2->SaveAs("Sides/SegEff_D1near.png");
   Ca2->Clear();

   EffGlobD1far->Draw();
   EffGlobD1far->GetYaxis()->SetTitle("%");
  
   BXGlobD1far->SetMarkerColor(9);
   BXGlobD1far->SetLineColor(9);
   BXGlobD1far->Draw("same");

   Signal_BXGlobD1far->SetMarkerColor(2);
   Signal_BXGlobD1far->SetLineColor(2);
   Signal_BXGlobD1far->Draw("same");

   MaskedGlobD1far->SetMarkerColor(2);
   MaskedGlobD1far->SetLineColor(2);
   MaskedGlobD1far->Draw("same");

   AverageEffD1far->SetMarkerColor(8);
   AverageEffD1far->SetLineColor(8);
   AverageEffD1far->Draw("same");

   NoPredictionD1far->SetMarkerColor(5);
   NoPredictionD1far->SetLineColor(5);
   NoPredictionD1far->Draw("same");

   pave->Draw();

   bxAxisEndCap->Draw("same");
  
   Ca2->SaveAs("Sides/SegEff_D1far.png");
   Ca2->Clear();

   EffGlobD2->Draw();
   EffGlobD2->GetYaxis()->SetTitle("%");
  
   BXGlobD2->SetMarkerColor(9);
   BXGlobD2->SetLineColor(9);
   BXGlobD2->Draw("same");

   Signal_BXGlobD2->SetMarkerColor(2);
   Signal_BXGlobD2->SetLineColor(2);
   Signal_BXGlobD2->Draw("same");

   MaskedGlobD2->SetMarkerColor(2);
   MaskedGlobD2->SetLineColor(2);
   MaskedGlobD2->Draw("same");

   AverageEffD2->SetMarkerColor(8);
   AverageEffD2->SetLineColor(8);
   AverageEffD2->Draw("same");

   NoPredictionD2->SetMarkerColor(5);
   NoPredictionD2->SetLineColor(5);
   NoPredictionD2->Draw("same");

   pave->Draw();

   bxAxisEndCap->Draw("same");
  
   Ca2->SaveAs("Sides/SegEff_D2near.png");
   Ca2->Clear();

   EffGlobD2far->Draw();
   EffGlobD2far->GetYaxis()->SetTitle("%");
  
   BXGlobD2far->SetMarkerColor(9);
   BXGlobD2far->SetLineColor(9);
   BXGlobD2far->Draw("same");

   Signal_BXGlobD2far->SetMarkerColor(2);
   Signal_BXGlobD2far->SetLineColor(2);
   Signal_BXGlobD2far->Draw("same");

   MaskedGlobD2far->SetMarkerColor(2);
   MaskedGlobD2far->SetLineColor(2);
   MaskedGlobD2far->Draw("same");

   AverageEffD2far->SetMarkerColor(8);
   AverageEffD2far->SetLineColor(8);
   AverageEffD2far->Draw("same");

   NoPredictionD2far->SetMarkerColor(5);
   NoPredictionD2far->SetLineColor(5);
   NoPredictionD2far->Draw("same");

   pave->Draw();

   bxAxisEndCap->Draw("same");
  
   Ca2->SaveAs("Sides/SegEff_D2far.png");
   Ca2->Clear();

   EffGlobD3far->Draw();
   EffGlobD3far->GetYaxis()->SetTitle("%");
  
   BXGlobD3far->SetMarkerColor(9);
   BXGlobD3far->SetLineColor(9);
   BXGlobD3far->Draw("same");

   Signal_BXGlobD3far->SetMarkerColor(2);
   Signal_BXGlobD3far->SetLineColor(2);
   Signal_BXGlobD3far->Draw("same");

   MaskedGlobD3far->SetMarkerColor(2);
   MaskedGlobD3far->SetLineColor(2);
   MaskedGlobD3far->Draw("same");

   AverageEffD3far->SetMarkerColor(8);
   AverageEffD3far->SetLineColor(8);
   AverageEffD3far->Draw("same");

   NoPredictionD3far->SetMarkerColor(5);
   NoPredictionD3far->SetLineColor(5);
   NoPredictionD3far->Draw("same");

   pave->Draw();

   bxAxisEndCap->Draw("same");
  
   Ca2->SaveAs("Sides/SegEff_D3far.png");
   Ca2->Clear();

   EffGlobD3->Draw();
   EffGlobD3->GetYaxis()->SetTitle("%");
  
   BXGlobD3->SetMarkerColor(9);
   BXGlobD3->SetLineColor(9);
   BXGlobD3->Draw("same");

   Signal_BXGlobD3->SetMarkerColor(2);
   Signal_BXGlobD3->SetLineColor(2);
   Signal_BXGlobD3->Draw("same");

   MaskedGlobD3->SetMarkerColor(2);
   MaskedGlobD3->SetLineColor(2);
   MaskedGlobD3->Draw("same");

   AverageEffD3->SetMarkerColor(8);
   AverageEffD3->SetLineColor(8);
   AverageEffD3->Draw("same");

   NoPredictionD3->SetMarkerColor(5);
   NoPredictionD3->SetLineColor(5);
   NoPredictionD3->Draw("same");

   pave->Draw();

   bxAxisEndCap->Draw("same");
  
   Ca2->SaveAs("Sides/SegEff_D3near.png");
   Ca2->Clear();
   
   //NEGATIVE

   EffGlobDm1->Draw();
   EffGlobDm1->GetYaxis()->SetTitle("%");
  
   BXGlobDm1->SetMarkerColor(9);
   BXGlobDm1->SetLineColor(9);
   BXGlobDm1->Draw("same");

   Signal_BXGlobDm1->SetMarkerColor(2);
   Signal_BXGlobDm1->SetLineColor(2);
   Signal_BXGlobDm1->Draw("same");

   MaskedGlobDm1->SetMarkerColor(2);
   MaskedGlobDm1->SetLineColor(2);
   MaskedGlobDm1->Draw("same");

   AverageEffDm1->SetMarkerColor(8);
   AverageEffDm1->SetLineColor(8);
   AverageEffDm1->Draw("same");

   NoPredictionDm1->SetMarkerColor(5);
   NoPredictionDm1->SetLineColor(5);
   NoPredictionDm1->Draw("same");

   pave->Draw();

   bxAxisEndCap->Draw("same");
  
   Ca2->SaveAs("Sides/SegEff_Dm1near.png");
   Ca2->Clear();

   EffGlobDm1far->Draw();
   EffGlobDm1far->GetYaxis()->SetTitle("%");
  
   BXGlobDm1far->SetMarkerColor(9);
   BXGlobDm1far->SetLineColor(9);
   BXGlobDm1far->Draw("same");

   Signal_BXGlobDm1far->SetMarkerColor(2);
   Signal_BXGlobDm1far->SetLineColor(2);
   Signal_BXGlobDm1far->Draw("same");
   
   MaskedGlobDm1far->SetMarkerColor(2);
   MaskedGlobDm1far->SetLineColor(2);
   MaskedGlobDm1far->Draw("same");

   AverageEffDm1far->SetMarkerColor(8);
   AverageEffDm1far->SetLineColor(8);
   AverageEffDm1far->Draw("same");

   NoPredictionDm1far->SetMarkerColor(5);
   NoPredictionDm1far->SetLineColor(5);
   NoPredictionDm1far->Draw("same");

   pave->Draw();

   bxAxisEndCap->Draw("same");
  
   Ca2->SaveAs("Sides/SegEff_Dm1far.png");
   Ca2->Clear();

   EffGlobDm2->Draw();
   EffGlobDm2->GetYaxis()->SetTitle("%");
  
   BXGlobDm2->SetMarkerColor(9);
   BXGlobDm2->SetLineColor(9);
   BXGlobDm2->Draw("same");

   Signal_BXGlobDm2->SetMarkerColor(2);
   Signal_BXGlobDm2->SetLineColor(2);
   Signal_BXGlobDm2->Draw("same");


   MaskedGlobDm2->SetMarkerColor(2);
   MaskedGlobDm2->SetLineColor(2);
   MaskedGlobDm2->Draw("same");

   AverageEffDm2->SetMarkerColor(8);
   AverageEffDm2->SetLineColor(8);
   AverageEffDm2->Draw("same");

   NoPredictionDm2->SetMarkerColor(5);
   NoPredictionDm2->SetLineColor(5);
   NoPredictionDm2->Draw("same");

   pave->Draw();

   bxAxisEndCap->Draw("same");
  
   Ca2->SaveAs("Sides/SegEff_Dm2near.png");
   Ca2->Clear();

   EffGlobDm2far->Draw();
   EffGlobDm2far->GetYaxis()->SetTitle("%");
  
   BXGlobDm2far->SetMarkerColor(9);
   BXGlobDm2far->SetLineColor(9);
   BXGlobDm2far->Draw("same");

   Signal_BXGlobDm2far->SetMarkerColor(2);
   Signal_BXGlobDm2far->SetLineColor(2);
   Signal_BXGlobDm2far->Draw("same");

   MaskedGlobDm2far->SetMarkerColor(2);
   MaskedGlobDm2far->SetLineColor(2);
   MaskedGlobDm2far->Draw("same");

   AverageEffDm2far->SetMarkerColor(8);
   AverageEffDm2far->SetLineColor(8);
   AverageEffDm2far->Draw("same");

   NoPredictionDm2far->SetMarkerColor(5);
   NoPredictionDm2far->SetLineColor(5);
   NoPredictionDm2far->Draw("same");

   pave->Draw();

   bxAxisEndCap->Draw("same");
  
   Ca2->SaveAs("Sides/SegEff_Dm2far.png");
   Ca2->Clear();


   EffGlobDm3far->Draw();
   EffGlobDm3far->GetYaxis()->SetTitle("%");
  
   BXGlobDm3far->SetMarkerColor(9);
   BXGlobDm3far->SetLineColor(9);
   BXGlobDm3far->Draw("same");

   Signal_BXGlobDm3far->SetMarkerColor(2);
   Signal_BXGlobDm3far->SetLineColor(2);
   Signal_BXGlobDm3far->Draw("same");

   MaskedGlobDm3far->SetMarkerColor(2);
   MaskedGlobDm3far->SetLineColor(2);
   MaskedGlobDm3far->Draw("same");

   AverageEffDm3far->SetMarkerColor(8);
   AverageEffDm3far->SetLineColor(8);
   AverageEffDm3far->Draw("same");

   NoPredictionDm3far->SetMarkerColor(5);
   NoPredictionDm3far->SetLineColor(5);
   NoPredictionDm3far->Draw("same");

   pave->Draw();

   bxAxisEndCap->Draw("same");
  
   Ca2->SaveAs("Sides/SegEff_Dm3far.png");
   Ca2->Clear();

   EffGlobDm3->Draw();
   EffGlobDm3->GetYaxis()->SetTitle("%");
  
   BXGlobDm3->SetMarkerColor(9);
   BXGlobDm3->SetLineColor(9);
   BXGlobDm3->Draw("same");

   Signal_BXGlobDm3->SetMarkerColor(2);
   Signal_BXGlobDm3->SetLineColor(2);
   Signal_BXGlobDm3->Draw("same");
   
   MaskedGlobDm3->SetMarkerColor(2);
   MaskedGlobDm3->SetLineColor(2);
   MaskedGlobDm3->Draw("same");

   AverageEffDm3->SetMarkerColor(8);
   AverageEffDm3->SetLineColor(8);
   AverageEffDm3->Draw("same");

   NoPredictionDm3->SetMarkerColor(5);
   NoPredictionDm3->SetLineColor(5);
   NoPredictionDm3->Draw("same");

   pave->Draw();

   bxAxisEndCap->Draw("same");
  
   Ca2->SaveAs("Sides/SegEff_Dm3near.png");
   Ca2->Clear();

 }
 
 if(barrel){
  
   Ca1->Clear();
   EffBarrel_black->SetFillColor(4); EffBarrel->GetXaxis()->SetTitle("%"); EffBarrel->Draw(); EffBarrel_black->Draw("same"); Ca1->SaveAs("Distro/EffDistroBarrel.png");
   
   Ca1->Clear();
   DoubleGapBarrel_black->SetFillColor(4); DoubleGapBarrel->GetXaxis()->SetTitle("%"); DoubleGapBarrel->Draw();  DoubleGapBarrel_black->Draw("same"); Ca1->SaveAs("Distro/DoubleGapBarrel.png");
   
   Ca1->Clear();
   CentralEffBarrel_black->SetFillColor(4); CentralEffBarrel->GetXaxis()->SetTitle("%"); CentralEffBarrel->Draw(); CentralEffBarrel_black->Draw("same"); Ca1->SaveAs("Distro/CentralEffBarrel.png");

   Ca1->Clear();
   BXEffBarrel->GetXaxis()->SetTitle("%"); BXEffBarrel->Draw(); Ca1->SaveAs("Distro/BXEffDistroBarrel.png");
   badBXEffBarrel->GetXaxis()->SetTitle("%"); badBXEffBarrel->Draw(); Ca1->SaveAs("Distro/badBXEffDistroBarrel.png");
   
   EffDistroWm2->GetXaxis()->SetTitle("%"); EffDistroWm2->Draw(); Ca1->SaveAs("Distro/EffDistroWm2.png");
   EffDistroWm1->GetXaxis()->SetTitle("%"); EffDistroWm1->Draw(); Ca1->SaveAs("Distro/EffDistroWm1.png");
   EffDistroW0->GetXaxis()->SetTitle("%"); EffDistroW0->Draw(); Ca1->SaveAs("Distro/EffDistroW0.png"); 
   EffDistroW1->GetXaxis()->SetTitle("%"); EffDistroW1->Draw(); Ca1->SaveAs("Distro/EffDistroW1.png");
   EffDistroW2->GetXaxis()->SetTitle("%"); EffDistroW2->Draw(); Ca1->SaveAs("Distro/EffDistroW2.png"); 

   EffDistroWm2far->GetXaxis()->SetTitle("%"); EffDistroWm2far->Draw(); Ca1->SaveAs("Distro/EffDistroWm2far.png");
   EffDistroWm1far->GetXaxis()->SetTitle("%"); EffDistroWm1far->Draw(); Ca1->SaveAs("Distro/EffDistroWm1far.png");
   EffDistroW0far->GetXaxis()->SetTitle("%"); EffDistroW0far->Draw();  Ca1->SaveAs("Distro/EffDistroW0far.png");
   EffDistroW1far->GetXaxis()->SetTitle("%"); EffDistroW1far->Draw();  Ca1->SaveAs("Distro/EffDistroW1far.png");
   EffDistroW2far->GetXaxis()->SetTitle("%"); EffDistroW2far->Draw();  Ca1->SaveAs("Distro/EffDistroW2far.png");

  EffDistroWm2far->Write();
  EffDistroWm1far->Write();
  EffDistroW0far->Write(); 
  EffDistroW1far->Write(); 
  EffDistroW2far->Write(); 
  
  DoubleGapDistroWm2->GetXaxis()->SetTitle("%"); DoubleGapDistroWm2->Draw(); Ca1->SaveAs("Distro/DoubleGapDistroWm2.png");
  DoubleGapDistroWm1->GetXaxis()->SetTitle("%"); DoubleGapDistroWm1->Draw(); Ca1->SaveAs("Distro/DoubleGapDistroWm1.png");
  DoubleGapDistroW0->GetXaxis()->SetTitle("%");  DoubleGapDistroW0->Draw();  Ca1->SaveAs("Distro/DoubleGapDistroW0.png");
  DoubleGapDistroW1->GetXaxis()->SetTitle("%");  DoubleGapDistroW1->Draw();  Ca1->SaveAs("Distro/DoubleGapDistroW1.png");
  DoubleGapDistroW2->GetXaxis()->SetTitle("%");  DoubleGapDistroW2->Draw();  Ca1->SaveAs("Distro/DoubleGapDistroW2.png");

  DoubleGapDistroWm2->Write();
  DoubleGapDistroWm1->Write();
  DoubleGapDistroW0->Write(); 
  DoubleGapDistroW1->Write(); 
  DoubleGapDistroW2->Write(); 

  DoubleGapDistroWm2far->GetXaxis()->SetTitle("%"); DoubleGapDistroWm2far->Draw(); Ca1->SaveAs("Distro/DoubleGapDistroWm2far.png"); 
  DoubleGapDistroWm1far->GetXaxis()->SetTitle("%"); DoubleGapDistroWm1far->Draw(); Ca1->SaveAs("Distro/DoubleGapDistroWm1far.png"); 
  DoubleGapDistroW0far->GetXaxis()->SetTitle("%");  DoubleGapDistroW0far->Draw();  Ca1->SaveAs("Distro/DoubleGapDistroW0far.png"); 
  DoubleGapDistroW1far->GetXaxis()->SetTitle("%");  DoubleGapDistroW1far->Draw();  Ca1->SaveAs("Distro/DoubleGapDistroW1far.png"); 
  DoubleGapDistroW2far->GetXaxis()->SetTitle("%");  DoubleGapDistroW2far->Draw();  Ca1->SaveAs("Distro/DoubleGapDistroW2far.png"); 

  DoubleGapDistroWm2far->Write();
  DoubleGapDistroWm1far->Write();
  DoubleGapDistroW0far->Write(); 
  DoubleGapDistroW1far->Write(); 
  DoubleGapDistroW2far->Write(); 

 }

 if(endcap){
   
   EffEndCap_black->SetFillColor(4); EffEndCap->GetXaxis()->SetTitle("%"); EffEndCap->Draw(); EffEndCap_black->Draw("same"); Ca1->SaveAs("Distro/EffDistroEndCap.png");EffEndCap->Write();EffEndCap_black->Write();
   Ca1->Clear();
   DoubleGapEndCap_black->SetFillColor(4); DoubleGapEndCap->GetXaxis()->SetTitle("%"); DoubleGapEndCap->Draw(); DoubleGapEndCap_black->Draw("same"); Ca1->SaveAs("Distro/DoubleGapEndCap.png");DoubleGapEndCap->Write();DoubleGapEndCap_black->Write();
   
   Ca1->Clear();
   CentralEffEndCap_black->SetFillColor(4); CentralEffEndCap->GetXaxis()->SetTitle("%"); CentralEffEndCap->Draw(); CentralEffEndCap_black->Draw("same"); Ca1->SaveAs("Distro/CentralEffEndCap.png");
   

   Ca1->Clear();
   BXEffEndCap->GetXaxis()->SetTitle("%"); BXEffEndCap->Draw(); Ca1->SaveAs("Distro/BXEffDistroEndCap.png");BXEffEndCap->Write();
   badBXEffEndCap->GetXaxis()->SetTitle("%"); badBXEffEndCap->Draw(); Ca1->SaveAs("Distro/badBXEffDistroEndCap.png");badBXEffEndCap->Write();

   BXEffPositiveEndCap->GetXaxis()->SetTitle("%"); BXEffPositiveEndCap->Draw(); Ca1->SaveAs("Distro/BXEffPositiveDistroEndCap.png");BXEffPositiveEndCap->Write();
   BXEffNegativeEndCap->GetXaxis()->SetTitle("%"); BXEffNegativeEndCap->Draw(); Ca1->SaveAs("Distro/BXEffNegativeDistroEndCap.png");BXEffNegativeEndCap->Write();

   badBXEffPositiveEndCap->GetXaxis()->SetTitle("%"); badBXEffPositiveEndCap->Draw(); Ca1->SaveAs("Distro/badBXEffPositiveDistroEndCap.png");badBXEffPositiveEndCap->Write();
   badBXEffNegativeEndCap->GetXaxis()->SetTitle("%"); badBXEffNegativeEndCap->Draw(); Ca1->SaveAs("Distro/badBXEffNegativeDistroEndCap.png");badBXEffNegativeEndCap->Write();
     
   EffDistroDm3->GetXaxis()->SetTitle("%"); EffDistroDm3->Draw(); Ca1->SaveAs("Distro/EffDistroDm3.png");   
   EffDistroDm2->GetXaxis()->SetTitle("%"); EffDistroDm2->Draw(); Ca1->SaveAs("Distro/EffDistroDm2.png"); 
   EffDistroDm1->GetXaxis()->SetTitle("%"); EffDistroDm1->Draw(); Ca1->SaveAs("Distro/EffDistroDm1.png"); 
   EffDistroD1->GetXaxis()->SetTitle("%");  EffDistroD1->Draw();  Ca1->SaveAs("Distro/EffDistroD1.png"); 
   EffDistroD2->GetXaxis()->SetTitle("%");  EffDistroD2->Draw();  Ca1->SaveAs("Distro/EffDistroD2.png"); 
   EffDistroD3->GetXaxis()->SetTitle("%");  EffDistroD3->Draw();  Ca1->SaveAs("Distro/EffDistroD3.png"); 

   EffDistroDm3->Write();
   EffDistroDm2->Write();
   EffDistroDm1->Write();
   EffDistroD1->Write(); 
   EffDistroD2->Write(); 
   EffDistroD3->Write(); 
   
   EffDistroDm3far->GetXaxis()->SetTitle("%"); EffDistroDm3far->Draw(); Ca1->SaveAs("Distro/EffDistroDm3far.png");   
   EffDistroDm2far->GetXaxis()->SetTitle("%"); EffDistroDm2far->Draw(); Ca1->SaveAs("Distro/EffDistroDm2far.png"); 
   EffDistroDm1far->GetXaxis()->SetTitle("%"); EffDistroDm1far->Draw(); Ca1->SaveAs("Distro/EffDistroDm1far.png"); 
   EffDistroD1far->GetXaxis()->SetTitle("%");  EffDistroD1far->Draw();  Ca1->SaveAs("Distro/EffDistroD1far.png"); 
   EffDistroD2far->GetXaxis()->SetTitle("%");  EffDistroD2far->Draw();  Ca1->SaveAs("Distro/EffDistroD2far.png"); 
   EffDistroD3far->GetXaxis()->SetTitle("%");  EffDistroD3far->Draw();  Ca1->SaveAs("Distro/EffDistroD3far.png"); 
   
   EffDistroDm3far->Write();
   EffDistroDm2far->Write();
   EffDistroDm1far->Write();
   EffDistroD1far->Write(); 
   EffDistroD2far->Write(); 
   EffDistroD3far->Write(); 
 }

 Ca1->Close();


 if(barrel){
   Ca11->Clear();
   CentralEffBarrel_black->SetFillColor(4); CentralEffBarrel->GetXaxis()->SetTitle("%"); CentralEffBarrel->Draw(); CentralEffBarrel_black->Draw("same"); Ca11->SetLogy(); Ca11->SaveAs("Distro/CentralEffBarrelLog.png");
 }
 if(endcap){
   Ca11->Clear();
   CentralEffEndCap_black->SetFillColor(4); CentralEffEndCap->GetXaxis()->SetTitle("%"); CentralEffEndCap->Draw(); CentralEffEndCap_black->Draw("same");  Ca11->SetLogy();Ca11->SaveAs("Distro/CentralEffEndCapLog.png");
 }

 Ca11->Close();
 
 //Cesare
 EffBarrelRoll->Write();
 HitDen->Write();
 CSCDen->Write();
 EndcapEffTot->Write();

 ExpLayerWm2->Write();
 ExpLayerWm1->Write();
 ExpLayerW0->Write();
 ExpLayerW1->Write();
 ExpLayerW2->Write();

 ObsLayerWm2->Write();
 ObsLayerWm1->Write();
 ObsLayerW0->Write();
 ObsLayerW1->Write();
 ObsLayerW2->Write();
 //

 MeanResiduals11->Write();
 MeanResiduals->Write();
 RMSResiduals11->Write();
 RMSResiduals->Write();

 Wheelm2Summary->Write();
 Wheelm1Summary->Write();
 Wheel0Summary->Write();
 Wheel1Summary->Write();
 Wheel2Summary->Write();

 Diskm3Summary->Write();
 Diskm2Summary->Write();
 Diskm1Summary->Write();
 Disk1Summary->Write();
 Disk2Summary->Write();
 Disk3Summary->Write();

 PositiveEndCapSummary->Write();
 NegativeEndCapSummary->Write();

 DoubleGapDistroWm2->Write();
 DoubleGapDistroWm1->Write();
 DoubleGapDistroW0->Write();
 DoubleGapDistroW1->Write();
 DoubleGapDistroW2->Write();
 
 DoubleGapDistroWm2far->Write();
 DoubleGapDistroWm1far->Write();
 DoubleGapDistroW0far->Write();
 DoubleGapDistroW1far->Write();
 DoubleGapDistroW2far->Write();

 EffBarrel_black->Write();
 DoubleGapBarrel_black->Write();
 CentralEffBarrel_black->Write();

 EffBarrel->Write();
 DoubleGapBarrel->Write();
 CentralEffBarrel->Write();
 BXEffBarrel->Write();
 badBXEffBarrel->Write();

 EffDistroWm2->Write();
 EffDistroWm1->Write();
 EffDistroW0->Write();
 EffDistroW1->Write();
 EffDistroW2->Write();

 EffDistroWm2far->Write();
 EffDistroWm1far->Write();
 EffDistroW0far->Write();
 EffDistroW1far->Write();
 EffDistroW2far->Write();

 CentralEffEndCap->Write();
 CentralEffEndCap_black->Write();
 
 EffEndCap->Write();
 CentralEffEndCap->Write();
 DoubleGapEndCap->Write();
 badBXEffEndCap ->Write();

 GregD1R2->Write();
 GregD1R3->Write();
 GregD2R2->Write();
 GregD2R3->Write();
 GregD3R2->Write();
 GregD3R3->Write();
 GregDm1R2->Write();
 GregDm1R3->Write();
 GregDm2R2->Write();
 GregDm2R3->Write();
 GregDm3R2->Write();
 GregDm3R3->Write();


 GregD1R2_black->Write(); 
 GregD1R3_black->Write();
 GregD2R2_black->Write();
 GregD2R3_black->Write();
 GregD3R2_black->Write();
 GregD3R3_black->Write();
 GregDm1R2_black->Write();
 GregDm1R3_black->Write();
 GregDm2R2_black->Write();
 GregDm2R3_black->Write();
 GregDm3R2_black->Write();
 GregDm3R3_black->Write();

 OcGregD1R2->Write();
 OcGregD1R3->Write();
 OcGregD2R2->Write();
 OcGregD2R3->Write();
 OcGregD3R2->Write();
 OcGregD3R3->Write();

 ExGregD1R2->Write();
 ExGregD1R3->Write();
 ExGregD2R2->Write();
 ExGregD2R3->Write();
 ExGregD3R2->Write();
 ExGregD3R3->Write();

 OcGregDm1R2->Write();
 OcGregDm1R3->Write();
 OcGregDm2R2->Write();
 OcGregDm2R3->Write();
 OcGregDm3R2->Write();
 OcGregDm3R3->Write();

 ExGregDm1R2->Write();
 ExGregDm1R3->Write();
 ExGregDm2R2->Write();
 ExGregDm2R3->Write();
 ExGregDm3R2->Write();
 ExGregDm3R3->Write();

 EffDistroDm3->Write();  
 EffDistroDm2->Write();
 EffDistroDm1->Write();
 EffDistroD1->Write();
 EffDistroD2->Write();
 EffDistroD3->Write();

 EffDistroDm3far->Write();
 EffDistroDm2far->Write();
 EffDistroDm1far->Write();
 EffDistroD1far->Write();
 EffDistroD2far->Write();
 EffDistroD3far->Write();

 EffGlobWm2->Write();
 EffGlobWm1->Write();
 EffGlobW0->Write();
 EffGlobW1->Write();
 EffGlobW2->Write();

 EffGlobWm2far->Write();
 EffGlobWm1far->Write();
 EffGlobW0far->Write();
 EffGlobW1far->Write();
 EffGlobW2far->Write();

 BXGlobWm2->Write();
 BXGlobWm1->Write();
 BXGlobW0->Write();
 BXGlobW1->Write();
 BXGlobW2->Write();

 Signal_BXGlobWm2->Write();
 Signal_BXGlobWm1->Write();
 Signal_BXGlobW0->Write();
 Signal_BXGlobW1->Write();
 Signal_BXGlobW2->Write();

 BXGlobWm2far->Write();
 BXGlobWm1far->Write();
 BXGlobW0far->Write();
 BXGlobW1far->Write();
 BXGlobW2far->Write();

 Signal_BXGlobWm2far->Write();
 Signal_BXGlobWm1far->Write();
 Signal_BXGlobW0far->Write();
 Signal_BXGlobW1far->Write();
 Signal_BXGlobW2far->Write();

 MaskedGlobWm2->Write();
 MaskedGlobWm1->Write();
 MaskedGlobW0->Write();
 MaskedGlobW1->Write();
 MaskedGlobW2->Write();

 MaskedGlobWm2far->Write();
 MaskedGlobWm1far->Write();
 MaskedGlobW0far->Write();
 MaskedGlobW1far->Write();
 MaskedGlobW2far->Write();

 AverageEffWm2->Write();
 AverageEffWm1->Write();
 AverageEffW0->Write();
 AverageEffW1->Write();
 AverageEffW2->Write();

 AverageEffWm2far->Write();
 AverageEffWm1far->Write();
 AverageEffW0far->Write();
 AverageEffW1far->Write();
 AverageEffW2far->Write();

 NoPredictionWm2->Write();
 NoPredictionWm1->Write();
 NoPredictionW0->Write();
 NoPredictionW1->Write();
 NoPredictionW2->Write();

 NoPredictionWm2far->Write();
 NoPredictionWm1far->Write();
 NoPredictionW0far->Write();
 NoPredictionW1far->Write();
 NoPredictionW2far->Write();

 DoubleGapWm2far->Write();
 DoubleGapWm1far->Write();
 DoubleGapW0far->Write();
 DoubleGapW1far->Write();
 DoubleGapW2far->Write();

 DoubleGapWm2->Write();
 DoubleGapWm1->Write();
 DoubleGapW0->Write();
 DoubleGapW1->Write();
 DoubleGapW2->Write();

 NoPredictionDm3->Write();
 AverageEffDm3->Write();
 EffGlobDm3->Write();
 BXGlobDm3->Write();
 MaskedGlobDm3->Write();
 NoPredictionDm3far->Write();
 AverageEffDm3far->Write();
 EffGlobDm3far->Write();
 BXGlobDm3far->Write();
 MaskedGlobDm3far->Write();
 NoPredictionDm2->Write();
 AverageEffDm2->Write();
 EffGlobDm2->Write();
 BXGlobDm2->Write();
 MaskedGlobDm2->Write();
 NoPredictionDm2far->Write();
 AverageEffDm2far->Write();
 EffGlobDm2far->Write();
 BXGlobDm2far->Write();
 MaskedGlobDm2far->Write();
 NoPredictionDm1->Write();
 AverageEffDm1->Write();
 EffGlobDm1->Write();
 BXGlobDm1->Write();
 MaskedGlobDm1->Write();
 NoPredictionDm1far->Write();
 AverageEffDm1far->Write();
 EffGlobDm1far->Write();
 BXGlobDm1far->Write();
 MaskedGlobDm1far->Write();
 NoPredictionD1->Write();
 AverageEffD1->Write();
 EffGlobD1->Write();
 BXGlobD1->Write();
 MaskedGlobD1->Write();
 NoPredictionD1far->Write();
 AverageEffD1far->Write();
 EffGlobD1far->Write();
 BXGlobD1far->Write();
 MaskedGlobD1far->Write();
 NoPredictionD2->Write();
 AverageEffD2->Write();
 EffGlobD2->Write();
 BXGlobD2->Write();
 MaskedGlobD2->Write();
 NoPredictionD2far->Write();
 AverageEffD2far->Write();
 EffGlobD2far->Write();
 BXGlobD2far->Write();
 MaskedGlobD2far->Write();
 NoPredictionD3->Write();
 AverageEffD3->Write();
 EffGlobD3->Write();
 BXGlobD3->Write();
 MaskedGlobD3->Write();
 NoPredictionD3far->Write();
 AverageEffD3far->Write();
 EffGlobD3far->Write();
 BXGlobD3far->Write();
 MaskedGlobD3far->Write();

 Signal_BXGlobDm3->Write();
 Signal_BXGlobDm3far->Write();
 Signal_BXGlobDm2->Write();
 Signal_BXGlobDm2far->Write();
 Signal_BXGlobDm1->Write();
 Signal_BXGlobDm1far->Write();
 Signal_BXGlobD1->Write();
 Signal_BXGlobD1far->Write();
 Signal_BXGlobD2->Write();
 Signal_BXGlobD2far->Write();
 Signal_BXGlobD3->Write();
 Signal_BXGlobD3far->Write();

 Ca2->Close();

 theFileOut->Write();
 theFileOut->Close();
 theFile->Close();

 //Cesare
 RollBeff.close();
 RollEeff.close();
 EffBarrelStrip.close();
 EffEndcapStrip.close();
 EffChamEndcap.close();
 //

 efftxt.close();
 alignment.close();
 database.close();
 RollYEff.close();

 delete Ca0;
 delete Ca1;
 delete Ca11;
 delete Ca2;
 delete Ca3;
 delete Ca4;
 delete Ca5;
 delete Ca6;
 delete Ca7;
 delete Ca8;
 delete Ca9; 
 delete Ca10;
 delete Ca101;

 delete GregD1R2_black; 
 delete GregD1R3_black;
 delete GregD2R2_black;
 delete GregD2R3_black;
 delete GregD3R2_black;
 delete GregD3R3_black;
 delete GregDm1R2_black;
 delete GregDm1R3_black;
 delete GregDm2R2_black;
 delete GregDm2R3_black;
 delete GregDm3R2_black;
 delete GregDm3R3_black;
 
 delete GregD1R2; 
 delete GregD1R3;
 delete GregD2R2;
 delete GregD2R3;
 delete GregD3R2;
 delete GregD3R3;
 delete GregDm1R2;
 delete GregDm1R3;
 delete GregDm2R2;
 delete GregDm2R3;
 delete GregDm3R2;
 delete GregDm3R3;
} 

void 
RPCMonitorEfficiency::endRun(){
  
}

DEFINE_FWK_MODULE(RPCMonitorEfficiency);

