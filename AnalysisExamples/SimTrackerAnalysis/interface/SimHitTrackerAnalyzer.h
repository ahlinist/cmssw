#ifndef SimHitTrackerAnalyzer_H
#define SimHitTrackerAnalyzer_H
// 
//
// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

// forward declarations
class TFile;
class TH1D;

class SimHitTrackerAnalyzer : public edm::EDAnalyzer{
   public:
  explicit SimHitTrackerAnalyzer( const edm::ParameterSet& );
  virtual ~SimHitTrackerAnalyzer() {};
  
  virtual void analyze( const edm::Event&, const edm::EventSetup& );
  virtual void beginJob( const edm::EventSetup& );
  virtual void endJob();
   private:


  std::string fOutputFileName;

  TH1D* entryX_exitXTK;
  TH1D* entryY_exitYTK;
  TH1D* entryZ_exitZTK;
  TH1D* energyLossTK;

  TH1D* entryX_exitXPXB;
  TH1D* entryY_exitYPXB;
  TH1D* entryZ_exitZPXB;
  TH1D* energyLossPXB;

  TH1D* entryX_exitXPXF;
  TH1D* entryY_exitYPXF;
  TH1D* entryZ_exitZPXF;
  TH1D* energyLossPXF;

  TH1D* entryX_exitXTIB;
  TH1D* entryY_exitYTIB;
  TH1D* entryZ_exitZTIB;
  TH1D* energyLossTIB;

  TH1D* entryX_exitXTID;
  TH1D* entryY_exitYTID;
  TH1D* entryZ_exitZTID;
  TH1D* energyLossTID;

  TH1D* entryX_exitXTOB;
  TH1D* entryY_exitYTOB;
  TH1D* entryZ_exitZTOB;
  TH1D* energyLossTOB;

  TH1D* entryX_exitXTEC;
  TH1D* entryY_exitYTEC;
  TH1D* entryZ_exitZTEC;
  TH1D* energyLossTEC;

  TH1D* entryX_exitXPXB1;
  TH1D* entryY_exitYPXB1;
  TH1D* entryZ_exitZPXB1;
  TH1D* energyLossPXB1;
  TH1D* entryX_exitXPXB2;
  TH1D* entryY_exitYPXB2;
  TH1D* entryZ_exitZPXB2;
  TH1D* energyLossPXB2;
  TH1D* entryX_exitXPXB3;
  TH1D* entryY_exitYPXB3;
  TH1D* entryZ_exitZPXB3;
  TH1D* energyLossPXB3;
			   
  TH1D* entryX_exitXPXF1;
  TH1D* entryY_exitYPXF1;
  TH1D* entryZ_exitZPXF1;
  TH1D* energyLossPXF1;
  TH1D* entryX_exitXPXF2;
  TH1D* entryY_exitYPXF2;
  TH1D* entryZ_exitZPXF2;
  TH1D* energyLossPXF2;

  TH1D* entryX_exitXTIB1;
  TH1D* entryY_exitYTIB1;
  TH1D* entryZ_exitZTIB1;
  TH1D* energyLossTIB1;
  TH1D* entryX_exitXTIB2;
  TH1D* entryY_exitYTIB2;
  TH1D* entryZ_exitZTIB2;
  TH1D* energyLossTIB2;
  TH1D* entryX_exitXTIB3;
  TH1D* entryY_exitYTIB3;
  TH1D* entryZ_exitZTIB3;
  TH1D* energyLossTIB3;
  TH1D* entryX_exitXTIB4;
  TH1D* entryY_exitYTIB4;
  TH1D* entryZ_exitZTIB4;
  TH1D* energyLossTIB4;

  TH1D* entryX_exitXTID1;
  TH1D* entryY_exitYTID1;
  TH1D* entryZ_exitZTID1;
  TH1D* energyLossTID1;
  TH1D* entryX_exitXTID2;
  TH1D* entryY_exitYTID2;
  TH1D* entryZ_exitZTID2;
  TH1D* energyLossTID2;
  TH1D* entryX_exitXTID3;
  TH1D* entryY_exitYTID3;
  TH1D* entryZ_exitZTID3;
  TH1D* energyLossTID3;

  TH1D* entryX_exitXTOB1;
  TH1D* entryY_exitYTOB1;
  TH1D* entryZ_exitZTOB1;
  TH1D* energyLossTOB1;
  TH1D* entryX_exitXTOB2;
  TH1D* entryY_exitYTOB2;
  TH1D* entryZ_exitZTOB2;
  TH1D* energyLossTOB2;
  TH1D* entryX_exitXTOB3;
  TH1D* entryY_exitYTOB3;
  TH1D* entryZ_exitZTOB3;
  TH1D* energyLossTOB3;
  TH1D* entryX_exitXTOB4;
  TH1D* entryY_exitYTOB4;
  TH1D* entryZ_exitZTOB4;
  TH1D* energyLossTOB4;
  TH1D* entryX_exitXTOB5;
  TH1D* entryY_exitYTOB5;
  TH1D* entryZ_exitZTOB5;
  TH1D* energyLossTOB5;
  TH1D* entryX_exitXTOB6;
  TH1D* entryY_exitYTOB6;
  TH1D* entryZ_exitZTOB6;
  TH1D* energyLossTOB6;

  TH1D* entryX_exitXTEC1;
  TH1D* entryY_exitYTEC1;
  TH1D* entryZ_exitZTEC1;
  TH1D* energyLossTEC1;
  TH1D* entryX_exitXTEC2;
  TH1D* entryY_exitYTEC2;
  TH1D* entryZ_exitZTEC2;
  TH1D* energyLossTEC2;
  TH1D* entryX_exitXTEC3;
  TH1D* entryY_exitYTEC3;
  TH1D* entryZ_exitZTEC3;
  TH1D* energyLossTEC3;
  TH1D* entryX_exitXTEC4;
  TH1D* entryY_exitYTEC4;
  TH1D* entryZ_exitZTEC4;
  TH1D* energyLossTEC4;
  TH1D* entryX_exitXTEC5;
  TH1D* entryY_exitYTEC5;
  TH1D* entryZ_exitZTEC5;
  TH1D* energyLossTEC5;
  TH1D* entryX_exitXTEC6;
  TH1D* entryY_exitYTEC6;
  TH1D* entryZ_exitZTEC6;
  TH1D* energyLossTEC6;
  TH1D* entryX_exitXTEC7;
  TH1D* entryY_exitYTEC7;
  TH1D* entryZ_exitZTEC7;
  TH1D* energyLossTEC7;
  TH1D* entryX_exitXTEC8;
  TH1D* entryY_exitYTEC8;
  TH1D* entryZ_exitZTEC8;
  TH1D* energyLossTEC8;
  TH1D* entryX_exitXTEC9;
  TH1D* entryY_exitYTEC9;
  TH1D* entryZ_exitZTEC9;
  TH1D* energyLossTEC9;
  TH1D* numbHitsForDet;
  
  TFile* histoPlot;
};

#endif
