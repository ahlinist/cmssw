// -*- C++ -*-
//
// Package:    SimHitTrackerAnalyzer
// Class:      SimHitTrackerAnalyzer
// 
/**\class SimHitTrackerAnalyzer SimHitTrackerAnalyzer.cc test/SimHitTrackerAnalyzer/src/SimHitTrackerAnalyzer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Tommaso Boccali
//         Created:  Tue Jul 26 08:47:57 CEST 2005
// $Id: SimHitTrackerAnalyzer.cc,v 1.9 2006/08/02 08:13:37 llista Exp $
//
//


// system include files
#include <memory>

// user include files
#include "AnalysisExamples/SimTrackerAnalysis/interface/SimHitTrackerAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"

#include "DataFormats/DetId/interface/DetId.h"

#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"

#include "DataFormats/SiStripDetId/interface/TIBDetId.h"
#include "DataFormats/SiStripDetId/interface/TIDDetId.h"
#include "DataFormats/SiStripDetId/interface/TOBDetId.h"
#include "DataFormats/SiStripDetId/interface/TECDetId.h"

#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/StripGeomDetUnit.h"

#include "TFile.h"
#include "TH1.h"



SimHitTrackerAnalyzer::SimHitTrackerAnalyzer( const edm::ParameterSet& iConfig ):
  fOutputFileName(iConfig.getUntrackedParameter("HistOutFile",std::string("TestHiggsMass.root"))),
  entryX_exitXTK(0),entryY_exitYTK(0),entryZ_exitZTK(0),energyLossTK(0),
  entryX_exitXPXB(0),entryY_exitYPXB(0),entryZ_exitZPXB(0),energyLossPXB(0),
  entryX_exitXPXF(0),entryY_exitYPXF(0),entryZ_exitZPXF(0),energyLossPXF(0),
  entryX_exitXTIB(0),entryY_exitYTIB(0),entryZ_exitZTIB(0),energyLossTIB(0),
  entryX_exitXTID(0),entryY_exitYTID(0),entryZ_exitZTID(0),energyLossTID(0),
  entryX_exitXTOB(0),entryY_exitYTOB(0),entryZ_exitZTOB(0),energyLossTOB(0),
  entryX_exitXTEC(0),entryY_exitYTEC(0),entryZ_exitZTEC(0),energyLossTEC(0),
  entryX_exitXPXB1(0),entryY_exitYPXB1(0),entryZ_exitZPXB1(0),energyLossPXB1(0),entryX_exitXPXB2(0), entryY_exitYPXB2(0),
  entryZ_exitZPXB2(0),energyLossPXB2(0),entryX_exitXPXB3(0),entryY_exitYPXB3(0),entryZ_exitZPXB3(0),energyLossPXB3(0),
  entryX_exitXPXF1(0),entryY_exitYPXF1(0),entryZ_exitZPXF1(0),energyLossPXF1(0),entryX_exitXPXF2(0),entryY_exitYPXF2(0),
  entryZ_exitZPXF2(0),energyLossPXF2(0),entryX_exitXTIB1(0),entryY_exitYTIB1(0),entryZ_exitZTIB1(0),energyLossTIB1(0),
  entryX_exitXTIB2(0),entryY_exitYTIB2(0),entryZ_exitZTIB2(0),energyLossTIB2(0),entryX_exitXTIB3(0),entryY_exitYTIB3(0),
  entryZ_exitZTIB3(0),energyLossTIB3(0),entryX_exitXTIB4(0),entryY_exitYTIB4(0),entryZ_exitZTIB4(0),energyLossTIB4(0),
  entryX_exitXTID1(0),entryY_exitYTID1(0),entryZ_exitZTID1(0),energyLossTID1(0),entryX_exitXTID2(0),entryY_exitYTID2(0),
  entryZ_exitZTID2(0),energyLossTID2(0),entryX_exitXTID3(0),entryY_exitYTID3(0),entryZ_exitZTID3(0),energyLossTID3(0),
  entryX_exitXTOB1(0),entryY_exitYTOB1(0),entryZ_exitZTOB1(0),energyLossTOB1(0),entryX_exitXTOB2(0),entryY_exitYTOB2(0),
  entryZ_exitZTOB2(0),energyLossTOB2(0),entryX_exitXTOB3(0),entryY_exitYTOB3(0),entryZ_exitZTOB3(0),energyLossTOB3(0),
  entryX_exitXTOB4(0),entryY_exitYTOB4(0),entryZ_exitZTOB4(0),energyLossTOB4(0),entryX_exitXTOB5(0),entryY_exitYTOB5(0),
  entryZ_exitZTOB5(0),energyLossTOB5(0),entryX_exitXTOB6(0),entryY_exitYTOB6(0),entryZ_exitZTOB6(0),energyLossTOB6(0),
  entryX_exitXTEC1(0),entryY_exitYTEC1(0),entryZ_exitZTEC1(0),energyLossTEC1(0),entryX_exitXTEC2(0),entryY_exitYTEC2(0),
  entryZ_exitZTEC2(0),energyLossTEC2(0),entryX_exitXTEC3(0),entryY_exitYTEC3(0),entryZ_exitZTEC3(0),energyLossTEC3(0),
  entryX_exitXTEC4(0),entryY_exitYTEC4(0),entryZ_exitZTEC4(0),energyLossTEC4(0),entryX_exitXTEC5(0),entryY_exitYTEC5(0),
  entryZ_exitZTEC5(0),energyLossTEC5(0),entryX_exitXTEC6(0),entryY_exitYTEC6(0),entryZ_exitZTEC6(0),energyLossTEC6(0),
  entryX_exitXTEC7(0),entryY_exitYTEC7(0),entryZ_exitZTEC7(0),energyLossTEC7(0),entryX_exitXTEC8(0),entryY_exitYTEC8(0),
  entryZ_exitZTEC8(0),energyLossTEC8(0),entryX_exitXTEC9(0),entryY_exitYTEC9(0),entryZ_exitZTEC9(0),energyLossTEC9(0),
  numbHitsForDet(0),histoPlot(0)
{
   //now do what ever initialization is needed
}

void SimHitTrackerAnalyzer::beginJob(const edm::EventSetup&){

  histoPlot = new TFile(fOutputFileName.c_str(),"RECREATE");

  entryX_exitXTK  = new TH1D(  "HistEntryExitXTK"  , "EntryX - ExitX" , 100,  -0.03, 0.03 ) ;
  entryY_exitYTK  = new TH1D(  "HistEntryExitYTK"  , "EntryY - ExitY" , 100,  -0.03, 0.03 ) ;
  entryZ_exitZTK  = new TH1D(  "HistEntryExitZTK"  , "EntryZ - ExitZ" , 100,  -0.03, 0.03 ) ;
  energyLossTK    = new TH1D(  "HistEnergyLossTK"  , "EnergyLoss" , 100,  0., 0.001 ) ;


  //PXB histo		   
  entryX_exitXPXB  = new TH1D(  "HistEntryExitXPXB"  , "EntryX - ExitX" , 100,  -0.03, 0.03 ) ;
  entryY_exitYPXB  = new TH1D(  "HistEntryExitYPXB"  , "EntryY - ExitY" , 100,  -0.03, 0.03 ) ;
  entryZ_exitZPXB  = new TH1D(  "HistEntryExitZPXB"  , "EntryZ - ExitZ" , 100,  -0.03, 0.03 ) ;
  energyLossPXB    = new TH1D(  "HistEnergyLossPXB"  , "EnergyLoss" , 100,  0., 0.001 ) ;

  entryX_exitXPXB1  = new TH1D(  "HistEntryExitXPXB1"  , "EntryX - ExitX" , 100,  -0.03, 0.03 ) ;
  entryY_exitYPXB1  = new TH1D(  "HistEntryExitYPXB1"  , "EntryY - ExitY" , 100,  -0.03, 0.03 ) ;
  entryZ_exitZPXB1  = new TH1D(  "HistEntryExitZPXB1"  , "EntryZ - ExitZ" , 100,  -0.03, 0.03 ) ;
  energyLossPXB1    = new TH1D(  "HistEnergyLossPXB1"  , "EnergyLoss" , 100,  0., 0.001 ) ;
  entryX_exitXPXB2  = new TH1D(  "HistEntryExitXPXB2"  , "EntryX - ExitX" , 100,  -0.03, 0.03 ) ;
  entryY_exitYPXB2  = new TH1D(  "HistEntryExitYPXB2"  , "EntryY - ExitY" , 100,  -0.03, 0.03 ) ;
  entryZ_exitZPXB2  = new TH1D(  "HistEntryExitZPXB2"  , "EntryZ - ExitZ" , 100,  -0.03, 0.03 ) ;
  energyLossPXB2    = new TH1D(  "HistEnergyLossPXB2"  , "EnergyLoss" , 100,  0., 0.001 ) ;
  entryX_exitXPXB3  = new TH1D(  "HistEntryExitXPXB3"  , "EntryX - ExitX" , 100,  -0.03, 0.03 ) ;
  entryY_exitYPXB3  = new TH1D(  "HistEntryExitYPXB3"  , "EntryY - ExitY" , 100,  -0.03, 0.03 ) ;
  entryZ_exitZPXB3  = new TH1D(  "HistEntryExitZPXB3"  , "EntryZ - ExitZ" , 100,  -0.03, 0.03 ) ;
  energyLossPXB3    = new TH1D(  "HistEnergyLossPXB3"  , "EnergyLoss" , 100,  0., 0.001 ) ;
			   
  //PXF histo		   
  entryX_exitXPXF  = new TH1D(  "HistEntryExitXPXF"  , "EntryX - ExitX" , 100,  -0.03, 0.03 ) ;
  entryY_exitYPXF  = new TH1D(  "HistEntryExitYPXF"  , "EntryY - ExitY" , 100,  -0.03, 0.03 ) ;
  entryZ_exitZPXF  = new TH1D(  "HistEntryExitZPXF"  , "EntryZ - ExitZ" , 100,  -0.03, 0.03 ) ;
  energyLossPXF    = new TH1D(  "HistEnergyLossPXF"  , "EnergyLoss" , 100,  0., 0.001 ) ;

  entryX_exitXPXF1  = new TH1D(  "HistEntryExitXPXF1"  , "EntryX - ExitX" , 100,  -0.03, 0.03 ) ;
  entryY_exitYPXF1  = new TH1D(  "HistEntryExitYPXF1"  , "EntryY - ExitY" , 100,  -0.03, 0.03 ) ;
  entryZ_exitZPXF1  = new TH1D(  "HistEntryExitZPXF1"  , "EntryZ - ExitZ" , 100,  -0.03, 0.03 ) ;
  energyLossPXF1    = new TH1D(  "HistEnergyLossPXF1"  , "EnergyLoss" , 100,  0., 0.001 ) ;
  entryX_exitXPXF2  = new TH1D(  "HistEntryExitXPXF2"  , "EntryX - ExitX" , 100,  -0.03, 0.03 ) ;
  entryY_exitYPXF2  = new TH1D(  "HistEntryExitYPXF2"  , "EntryY - ExitY" , 100,  -0.03, 0.03 ) ;
  entryZ_exitZPXF2  = new TH1D(  "HistEntryExitZPXF2"  , "EntryZ - ExitZ" , 100,  -0.03, 0.03 ) ;
  energyLossPXF2    = new TH1D(  "HistEnergyLossPXF2"  , "EnergyLoss" , 100,  0., 0.001 ) ;
			   
  //TIB histo		   
  entryX_exitXTIB  = new TH1D(  "HistEntryExitXTIB"  , "EntryX - ExitX" , 100,  -0.03, 0.03 ) ;
  entryY_exitYTIB  = new TH1D(  "HistEntryExitYTIB"  , "EntryY - ExitY" , 100,  -0.03, 0.03 ) ;
  entryZ_exitZTIB  = new TH1D(  "HistEntryExitZTIB"  , "EntryZ - ExitZ" , 100,  -0.03, 0.03 ) ;
  energyLossTIB    = new TH1D(  "HistEnergyLossTIB"  , "EnergyLoss" , 100,  0., 0.001 ) ;

  entryX_exitXTIB1  = new TH1D(  "HistEntryExitXTIB1"  , "EntryX - ExitX" , 100,  -0.03, 0.03 ) ;
  entryY_exitYTIB1  = new TH1D(  "HistEntryExitYTIB1"  , "EntryY - ExitY" , 100,  -0.03, 0.03 ) ;
  entryZ_exitZTIB1  = new TH1D(  "HistEntryExitZTIB1"  , "EntryZ - ExitZ" , 100,  -0.03, 0.03 ) ;
  energyLossTIB1    = new TH1D(  "HistEnergyLossTIB1"  , "EnergyLoss" , 100,  0., 0.001 ) ;
  entryX_exitXTIB2  = new TH1D(  "HistEntryExitXTIB2"  , "EntryX - ExitX" , 100,  -0.03, 0.03 ) ;
  entryY_exitYTIB2  = new TH1D(  "HistEntryExitYTIB2"  , "EntryY - ExitY" , 100,  -0.03, 0.03 ) ;
  entryZ_exitZTIB2  = new TH1D(  "HistEntryExitZTIB2"  , "EntryZ - ExitZ" , 100,  -0.03, 0.03 ) ;
  energyLossTIB2    = new TH1D(  "HistEnergyLossTIB2"  , "EnergyLoss" , 100,  0., 0.001 ) ;
  entryX_exitXTIB3  = new TH1D(  "HistEntryExitXTIB3"  , "EntryX - ExitX" , 100,  -0.03, 0.03 ) ;
  entryY_exitYTIB3  = new TH1D(  "HistEntryExitYTIB3"  , "EntryY - ExitY" , 100,  -0.03, 0.03 ) ;
  entryZ_exitZTIB3  = new TH1D(  "HistEntryExitZTIB3"  , "EntryZ - ExitZ" , 100,  -0.03, 0.03 ) ;
  energyLossTIB3    = new TH1D(  "HistEnergyLossTIB3"  , "EnergyLoss" , 100,  0., 0.001 ) ;
  entryX_exitXTIB4  = new TH1D(  "HistEntryExitXTIB4"  , "EntryX - ExitX" , 100,  -0.03, 0.03 ) ;
  entryY_exitYTIB4  = new TH1D(  "HistEntryExitYTIB4"  , "EntryY - ExitY" , 100,  -0.03, 0.03 ) ;
  entryZ_exitZTIB4  = new TH1D(  "HistEntryExitZTIB4"  , "EntryZ - ExitZ" , 100,  -0.03, 0.03 ) ;
  energyLossTIB4    = new TH1D(  "HistEnergyLossTIB4"  , "EnergyLoss" , 100,  0., 0.001 ) ;
			   
  //TID histo		   
  entryX_exitXTID  = new TH1D(  "HistEntryExitXTID"  , "EntryX - ExitX" , 100,  -0.03, 0.03 ) ;
  entryY_exitYTID  = new TH1D(  "HistEntryExitYTID"  , "EntryY - ExitY" , 100,  -0.03, 0.03 ) ;
  entryZ_exitZTID  = new TH1D(  "HistEntryExitZTID"  , "EntryZ - ExitZ" , 100,  -0.03, 0.03 ) ;
  energyLossTID    = new TH1D(  "HistEnergyLossTID"  , "EnergyLoss" , 100,  0., 0.001 ) ;

  entryX_exitXTID1  = new TH1D(  "HistEntryExitXTID1"  , "EntryX - ExitX" , 100,  -0.03, 0.03 ) ;
  entryY_exitYTID1  = new TH1D(  "HistEntryExitYTID1"  , "EntryY - ExitY" , 100,  -0.03, 0.03 ) ;
  entryZ_exitZTID1  = new TH1D(  "HistEntryExitZTID1"  , "EntryZ - ExitZ" , 100,  -0.03, 0.03 ) ;
  energyLossTID1    = new TH1D(  "HistEnergyLossTID1"  , "EnergyLoss" , 100,  0., 0.001 ) ;
  entryX_exitXTID2  = new TH1D(  "HistEntryExitXTID2"  , "EntryX - ExitX" , 100,  -0.03, 0.03 ) ;
  entryY_exitYTID2  = new TH1D(  "HistEntryExitYTID2"  , "EntryY - ExitY" , 100,  -0.03, 0.03 ) ;
  entryZ_exitZTID2  = new TH1D(  "HistEntryExitZTID2"  , "EntryZ - ExitZ" , 100,  -0.03, 0.03 ) ;
  energyLossTID2    = new TH1D(  "HistEnergyLossTID2"  , "EnergyLoss" , 100,  0., 0.001 ) ;
  entryX_exitXTID3  = new TH1D(  "HistEntryExitXTID3"  , "EntryX - ExitX" , 100,  -0.03, 0.03 ) ;
  entryY_exitYTID3  = new TH1D(  "HistEntryExitYTID3"  , "EntryY - ExitY" , 100,  -0.03, 0.03 ) ;
  entryZ_exitZTID3  = new TH1D(  "HistEntryExitZTID3"  , "EntryZ - ExitZ" , 100,  -0.03, 0.03 ) ;
  energyLossTID3    = new TH1D(  "HistEnergyLossTID3"  , "EnergyLoss" , 100,  0., 0.001 ) ;
			   
  //TOB histo		   
  entryX_exitXTOB  = new TH1D(  "HistEntryExitXTOB"  , "EntryX - ExitX" , 100,  -0.03, 0.03 ) ;
  entryY_exitYTOB  = new TH1D(  "HistEntryExitYTOB"  , "EntryY - ExitY" , 100,  -0.03, 0.03 ) ;
  entryZ_exitZTOB  = new TH1D(  "HistEntryExitZTOB"  , "EntryZ - ExitZ" , 100,  -0.03, 0.03 ) ;
  energyLossTOB    = new TH1D(  "HistEnergyLossTOB"  , "EnergyLoss" , 100,  0., 0.001 ) ;

  entryX_exitXTOB1  = new TH1D(  "HistEntryExitXTOB1"  , "EntryX - ExitX" , 100,  -0.03, 0.03 ) ;
  entryY_exitYTOB1  = new TH1D(  "HistEntryExitYTOB1"  , "EntryY - ExitY" , 100,  -0.03, 0.03 ) ;
  entryZ_exitZTOB1  = new TH1D(  "HistEntryExitZTOB1"  , "EntryZ - ExitZ" , 100,  -0.03, 0.03 ) ;
  energyLossTOB1    = new TH1D(  "HistEnergyLossTOB1"  , "EnergyLoss" , 100,  0., 0.001 ) ;
  entryX_exitXTOB2  = new TH1D(  "HistEntryExitXTOB2"  , "EntryX - ExitX" , 100,  -0.03, 0.03 ) ;
  entryY_exitYTOB2  = new TH1D(  "HistEntryExitYTOB2"  , "EntryY - ExitY" , 100,  -0.03, 0.03 ) ;
  entryZ_exitZTOB2  = new TH1D(  "HistEntryExitZTOB2"  , "EntryZ - ExitZ" , 100,  -0.03, 0.03 ) ;
  energyLossTOB2    = new TH1D(  "HistEnergyLossTOB2"  , "EnergyLoss" , 100,  0., 0.001 ) ;
  entryX_exitXTOB3  = new TH1D(  "HistEntryExitXTOB3"  , "EntryX - ExitX" , 100,  -0.03, 0.03 ) ;
  entryY_exitYTOB3  = new TH1D(  "HistEntryExitYTOB3"  , "EntryY - ExitY" , 100,  -0.03, 0.03 ) ;
  entryZ_exitZTOB3  = new TH1D(  "HistEntryExitZTOB3"  , "EntryZ - ExitZ" , 100,  -0.03, 0.03 ) ;
  energyLossTOB3    = new TH1D(  "HistEnergyLossTOB3"  , "EnergyLoss" , 100,  0., 0.001 ) ;
  entryX_exitXTOB4  = new TH1D(  "HistEntryExitXTOB4"  , "EntryX - ExitX" , 100,  -0.03, 0.03 ) ;
  entryY_exitYTOB4  = new TH1D(  "HistEntryExitYTOB4"  , "EntryY - ExitY" , 100,  -0.03, 0.03 ) ;
  entryZ_exitZTOB4  = new TH1D(  "HistEntryExitZTOB4"  , "EntryZ - ExitZ" , 100,  -0.03, 0.03 ) ;
  energyLossTOB4    = new TH1D(  "HistEnergyLossTOB4"  , "EnergyLoss" , 100,  0., 0.001 ) ;
  entryX_exitXTOB5  = new TH1D(  "HistEntryExitXTOB5"  , "EntryX - ExitX" , 100,  -0.03, 0.03 ) ;
  entryY_exitYTOB5  = new TH1D(  "HistEntryExitYTOB5"  , "EntryY - ExitY" , 100,  -0.03, 0.03 ) ;
  entryZ_exitZTOB5  = new TH1D(  "HistEntryExitZTOB5"  , "EntryZ - ExitZ" , 100,  -0.03, 0.03 ) ;
  energyLossTOB5    = new TH1D(  "HistEnergyLossTOB5"  , "EnergyLoss" , 100,  0., 0.001 ) ;
  entryX_exitXTOB6  = new TH1D(  "HistEntryExitXTOB6"  , "EntryX - ExitX" , 100,  -0.03, 0.03 ) ;
  entryY_exitYTOB6  = new TH1D(  "HistEntryExitYTOB6"  , "EntryY - ExitY" , 100,  -0.03, 0.03 ) ;
  entryZ_exitZTOB6  = new TH1D(  "HistEntryExitZTOB6"  , "EntryZ - ExitZ" , 100,  -0.03, 0.03 ) ;
  energyLossTOB6    = new TH1D(  "HistEnergyLossTOB6"  , "EnergyLoss" , 100,  0., 0.001 ) ;
			   
  //TEC histo		   
  entryX_exitXTEC  = new TH1D(  "HistEntryExitXTEC"  , "EntryX - ExitX" , 100,  -0.03, 0.03 ) ;
  entryY_exitYTEC  = new TH1D(  "HistEntryExitYTEC"  , "EntryY - ExitY" , 100,  -0.03, 0.03 ) ;
  entryZ_exitZTEC  = new TH1D(  "HistEntryExitZTEC"  , "EntryZ - ExitZ" , 100,  -0.03, 0.03 ) ;
  energyLossTEC    = new TH1D(  "HistEnergyLossTEC"  , "EnergyLoss" , 100,  0., 0.001 ) ;

  entryX_exitXTEC1  = new TH1D(  "HistEntryExitXTEC1"  , "EntryX - ExitX" , 100,  -0.03, 0.03 ) ;
  entryY_exitYTEC1  = new TH1D(  "HistEntryExitYTEC1"  , "EntryY - ExitY" , 100,  -0.03, 0.03 ) ;
  entryZ_exitZTEC1  = new TH1D(  "HistEntryExitZTEC1"  , "EntryZ - ExitZ" , 100,  -0.03, 0.03 ) ;
  energyLossTEC1    = new TH1D(  "HistEnergyLossTEC1"  , "EnergyLoss" , 100,  0., 0.001 ) ;
  entryX_exitXTEC2  = new TH1D(  "HistEntryExitXTEC2"  , "EntryX - ExitX" , 100,  -0.03, 0.03 ) ;
  entryY_exitYTEC2  = new TH1D(  "HistEntryExitYTEC2"  , "EntryY - ExitY" , 100,  -0.03, 0.03 ) ;
  entryZ_exitZTEC2  = new TH1D(  "HistEntryExitZTEC2"  , "EntryZ - ExitZ" , 100,  -0.03, 0.03 ) ;
  energyLossTEC2    = new TH1D(  "HistEnergyLossTEC2"  , "EnergyLoss" , 100,  0., 0.001 ) ;
  entryX_exitXTEC3  = new TH1D(  "HistEntryExitXTEC3"  , "EntryX - ExitX" , 100,  -0.03, 0.03 ) ;
  entryY_exitYTEC3  = new TH1D(  "HistEntryExitYTEC3"  , "EntryY - ExitY" , 100,  -0.03, 0.03 ) ;
  entryZ_exitZTEC3  = new TH1D(  "HistEntryExitZTEC3"  , "EntryZ - ExitZ" , 100,  -0.03, 0.03 ) ;
  energyLossTEC3    = new TH1D(  "HistEnergyLossTEC3"  , "EnergyLoss" , 100,  0., 0.001 ) ;
  entryX_exitXTEC4  = new TH1D(  "HistEntryExitXTEC4"  , "EntryX - ExitX" , 100,  -0.03, 0.03 ) ;
  entryY_exitYTEC4  = new TH1D(  "HistEntryExitYTEC4"  , "EntryY - ExitY" , 100,  -0.03, 0.03 ) ;
  entryZ_exitZTEC4  = new TH1D(  "HistEntryExitZTEC4"  , "EntryZ - ExitZ" , 100,  -0.03, 0.03 ) ;
  energyLossTEC4    = new TH1D(  "HistEnergyLossTEC4"  , "EnergyLoss" , 100,  0., 0.001 ) ;
  entryX_exitXTEC5  = new TH1D(  "HistEntryExitXTEC5"  , "EntryX - ExitX" , 100,  -0.03, 0.03 ) ;
  entryY_exitYTEC5  = new TH1D(  "HistEntryExitYTEC5"  , "EntryY - ExitY" , 100,  -0.03, 0.03 ) ;
  entryZ_exitZTEC5  = new TH1D(  "HistEntryExitZTEC5"  , "EntryZ - ExitZ" , 100,  -0.03, 0.03 ) ;
  energyLossTEC5    = new TH1D(  "HistEnergyLossTEC5"  , "EnergyLoss" , 100,  0., 0.001 ) ;
  entryX_exitXTEC6  = new TH1D(  "HistEntryExitXTEC6"  , "EntryX - ExitX" , 100,  -0.03, 0.03 ) ;
  entryY_exitYTEC6  = new TH1D(  "HistEntryExitYTEC6"  , "EntryY - ExitY" , 100,  -0.03, 0.03 ) ;
  entryZ_exitZTEC6  = new TH1D(  "HistEntryExitZTEC6"  , "EntryZ - ExitZ" , 100,  -0.03, 0.03 ) ;
  energyLossTEC6    = new TH1D(  "HistEnergyLossTEC6"  , "EnergyLoss" , 100,  0., 0.001 ) ;
  entryX_exitXTEC7  = new TH1D(  "HistEntryExitXTEC7"  , "EntryX - ExitX" , 100,  -0.03, 0.03 ) ;
  entryY_exitYTEC7  = new TH1D(  "HistEntryExitYTEC7"  , "EntryY - ExitY" , 100,  -0.03, 0.03 ) ;
  entryZ_exitZTEC7  = new TH1D(  "HistEntryExitZTEC7"  , "EntryZ - ExitZ" , 100,  -0.03, 0.03 ) ;
  energyLossTEC7    = new TH1D(  "HistEnergyLossTEC7"  , "EnergyLoss" , 100,  0., 0.001 ) ;
  entryX_exitXTEC8  = new TH1D(  "HistEntryExitXTEC8"  , "EntryX - ExitX" , 100,  -0.03, 0.03 ) ;
  entryY_exitYTEC8  = new TH1D(  "HistEntryExitYTEC8"  , "EntryY - ExitY" , 100,  -0.03, 0.03 ) ;
  entryZ_exitZTEC8  = new TH1D(  "HistEntryExitZTEC8"  , "EntryZ - ExitZ" , 100,  -0.03, 0.03 ) ;
  energyLossTEC8    = new TH1D(  "HistEnergyLossTEC8"  , "EnergyLoss" , 100,  0., 0.001 ) ;
  entryX_exitXTEC9  = new TH1D(  "HistEntryExitXTEC9"  , "EntryX - ExitX" , 100,  -0.03, 0.03 ) ;
  entryY_exitYTEC9  = new TH1D(  "HistEntryExitYTEC9"  , "EntryY - ExitY" , 100,  -0.03, 0.03 ) ;
  entryZ_exitZTEC9  = new TH1D(  "HistEntryExitZTEC9"  , "EntryZ - ExitZ" , 100,  -0.03, 0.03 ) ;
  energyLossTEC9    = new TH1D(  "HistEnergyLossTEC9"  , "EnergyLoss" , 100,  0., 0.001 ) ;

  numbHitsForDet    = new TH1D(  "NumbHitsInDet"  , "Hits per Det" , 20,  0., 20. ) ;  

  return;

}

//
// member functions
//

// ------------ method called to produce the data  ------------
void SimHitTrackerAnalyzer::analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup ){

   using namespace edm;

   std::vector<PSimHit> theTrackerHits;
   // take the Geometry
   edm::ESHandle<TrackerGeometry> pDD;
   iSetup.get<TrackerDigiGeometryRecord>().get( pDD );     

   //   Handle<HepMCProduct> MCEvt;
   Handle<PSimHitContainer> PixelBarrelHitsLowTof;
   Handle<PSimHitContainer> PixelBarrelHitsHighTof;
   Handle<PSimHitContainer> PixelEndcapHitsLowTof;
   Handle<PSimHitContainer> PixelEndcapHitsHighTof;
   Handle<PSimHitContainer> TIBHitsLowTof;
   Handle<PSimHitContainer> TIBHitsHighTof;
   Handle<PSimHitContainer> TIDHitsLowTof;
   Handle<PSimHitContainer> TIDHitsHighTof;
   Handle<PSimHitContainer> TOBHitsLowTof;
   Handle<PSimHitContainer> TOBHitsHighTof;
   Handle<PSimHitContainer> TECHitsLowTof;
   Handle<PSimHitContainer> TECHitsHighTof;

   iEvent.getByLabel("g4SimHits","TrackerHitsPixelBarrelLowTof", PixelBarrelHitsLowTof);
   iEvent.getByLabel("g4SimHits","TrackerHitsPixelBarrelHighTof", PixelBarrelHitsHighTof);
   iEvent.getByLabel("g4SimHits","TrackerHitsPixelEndcapLowTof", PixelEndcapHitsLowTof);
   iEvent.getByLabel("g4SimHits","TrackerHitsPixelEndcapHighTof", PixelEndcapHitsHighTof);
   iEvent.getByLabel("g4SimHits","TrackerHitsTIBLowTof", TIBHitsLowTof);
   iEvent.getByLabel("g4SimHits","TrackerHitsTIBHighTof", TIBHitsHighTof);
   iEvent.getByLabel("g4SimHits","TrackerHitsTIDLowTof", TIDHitsLowTof);
   iEvent.getByLabel("g4SimHits","TrackerHitsTIDHighTof", TIDHitsHighTof);
   iEvent.getByLabel("g4SimHits","TrackerHitsTOBLowTof", TOBHitsLowTof);
   iEvent.getByLabel("g4SimHits","TrackerHitsTOBHighTof", TOBHitsHighTof);
   iEvent.getByLabel("g4SimHits","TrackerHitsTECLowTof", TECHitsLowTof);
   iEvent.getByLabel("g4SimHits","TrackerHitsTECHighTof", TECHitsHighTof);

   theTrackerHits.insert(theTrackerHits.end(), PixelBarrelHitsLowTof->begin(), PixelBarrelHitsLowTof->end()); 
   theTrackerHits.insert(theTrackerHits.end(), PixelBarrelHitsHighTof->begin(), PixelBarrelHitsHighTof->end());
   theTrackerHits.insert(theTrackerHits.end(), PixelEndcapHitsLowTof->begin(), PixelEndcapHitsLowTof->end()); 
   theTrackerHits.insert(theTrackerHits.end(), PixelEndcapHitsHighTof->begin(), PixelEndcapHitsHighTof->end());
   theTrackerHits.insert(theTrackerHits.end(), TIBHitsLowTof->begin(), TIBHitsLowTof->end()); 
   theTrackerHits.insert(theTrackerHits.end(), TIBHitsHighTof->begin(), TIBHitsHighTof->end());
   theTrackerHits.insert(theTrackerHits.end(), TIDHitsLowTof->begin(), TIDHitsLowTof->end()); 
   theTrackerHits.insert(theTrackerHits.end(), TIDHitsHighTof->begin(), TIDHitsHighTof->end());
   theTrackerHits.insert(theTrackerHits.end(), TOBHitsLowTof->begin(), TOBHitsLowTof->end()); 
   theTrackerHits.insert(theTrackerHits.end(), TOBHitsHighTof->begin(), TOBHitsHighTof->end());
   theTrackerHits.insert(theTrackerHits.end(), TECHitsLowTof->begin(), TECHitsLowTof->end()); 
   theTrackerHits.insert(theTrackerHits.end(), TECHitsHighTof->begin(), TECHitsHighTof->end());

   std::map<unsigned int, unsigned int,std::less<unsigned int> > SimHitMap;

   for (std::vector<PSimHit>::iterator isim = theTrackerHits.begin();
	isim != theTrackerHits.end(); ++isim){
     DetId tkId(isim->detUnitId());

     SimHitMap[(*isim).detUnitId()]++;

     float entryX = (isim->entryPoint()).x();
     float entryY = (isim->entryPoint()).y();
     float entryZ = (isim->entryPoint()).z();
     
     float exitX = (isim->exitPoint()).x();
     float exitY = (isim->exitPoint()).y();
     float exitZ = (isim->exitPoint()).z();
     
     float eloss = isim->energyLoss();
     
     entryX_exitXTK->Fill(entryX-exitX);
     entryY_exitYTK->Fill(entryY-exitY);
     entryZ_exitZTK->Fill(entryZ-exitZ);
     energyLossTK->Fill(eloss);

     if(tkId.subdetId()==1){
       PXBDetId pxbId(tkId);
       entryX_exitXPXB->Fill(entryX-exitX);
       entryY_exitYPXB->Fill(entryY-exitY);
       entryZ_exitZPXB->Fill(entryZ-exitZ);
       energyLossPXB->Fill(eloss);
       if(pxbId.layer()==1){
	 entryX_exitXPXB1->Fill(entryX-exitX);
	 entryY_exitYPXB1->Fill(entryY-exitY);
	 entryZ_exitZPXB1->Fill(entryZ-exitZ);
	 energyLossPXB1->Fill(eloss);
       }else if(pxbId.layer()==2){
	 entryX_exitXPXB2->Fill(entryX-exitX);
	 entryY_exitYPXB2->Fill(entryY-exitY);
	 entryZ_exitZPXB2->Fill(entryZ-exitZ);
	 energyLossPXB2->Fill(eloss);
       }else if(pxbId.layer()==3){
	 entryX_exitXPXB3->Fill(entryX-exitX);
	 entryY_exitYPXB3->Fill(entryY-exitY);
	 entryZ_exitZPXB3->Fill(entryZ-exitZ);
	 energyLossPXB3->Fill(eloss);
       }
     }else if(tkId.subdetId()==2){
       PXFDetId pxfId(tkId);
       entryX_exitXPXF->Fill(entryX-exitX);
       entryY_exitYPXF->Fill(entryY-exitY);
       entryZ_exitZPXF->Fill(entryZ-exitZ);
       energyLossPXF->Fill(eloss);
       if(pxfId.disk()==1){
	 entryX_exitXPXF1->Fill(entryX-exitX);
	 entryY_exitYPXF1->Fill(entryY-exitY);
	 entryZ_exitZPXF1->Fill(entryZ-exitZ);
	 energyLossPXF1->Fill(eloss);
       }else if(pxfId.disk()==2){
	 entryX_exitXPXF2->Fill(entryX-exitX);
	 entryY_exitYPXF2->Fill(entryY-exitY);
	 entryZ_exitZPXF2->Fill(entryZ-exitZ);
	 energyLossPXF2->Fill(eloss);
       }
     }else if(tkId.subdetId()==3){
       TIBDetId tibId(tkId);
       entryX_exitXTIB->Fill(entryX-exitX);
       entryY_exitYTIB->Fill(entryY-exitY);
       entryZ_exitZTIB->Fill(entryZ-exitZ);
       energyLossTIB->Fill(eloss);
       if(tibId.layer()==1){
	 entryX_exitXTIB1->Fill(entryX-exitX);
	 entryY_exitYTIB1->Fill(entryY-exitY);
	 entryZ_exitZTIB1->Fill(entryZ-exitZ);
	 energyLossTIB1->Fill(eloss);
       }else if(tibId.layer()==2){
	 entryX_exitXTIB2->Fill(entryX-exitX);
	 entryY_exitYTIB2->Fill(entryY-exitY);
	 entryZ_exitZTIB2->Fill(entryZ-exitZ);
	 energyLossTIB2->Fill(eloss);
       }else if(tibId.layer()==3){
	 entryX_exitXTIB3->Fill(entryX-exitX);
	 entryY_exitYTIB3->Fill(entryY-exitY);
	 entryZ_exitZTIB3->Fill(entryZ-exitZ);
	 energyLossTIB3->Fill(eloss);
       }else if(tibId.layer()==4){
	 entryX_exitXTIB4->Fill(entryX-exitX);
	 entryY_exitYTIB4->Fill(entryY-exitY);
	 entryZ_exitZTIB4->Fill(entryZ-exitZ);
	 energyLossTIB3->Fill(eloss);
       }
     }else if(tkId.subdetId()==4){
       TIDDetId tidId(tkId);
       entryX_exitXTID->Fill(entryX-exitX);
       entryY_exitYTID->Fill(entryY-exitY);
       entryZ_exitZTID->Fill(entryZ-exitZ);
       energyLossTID->Fill(eloss);
       if(tidId.wheel()==1){
	 entryX_exitXTID1->Fill(entryX-exitX);
	 entryY_exitYTID1->Fill(entryY-exitY);
	 entryZ_exitZTID1->Fill(entryZ-exitZ);
	 energyLossTID1->Fill(eloss);
       }else if(tidId.wheel()==2){
	 entryX_exitXTID2->Fill(entryX-exitX);
	 entryY_exitYTID2->Fill(entryY-exitY);
	 entryZ_exitZTID2->Fill(entryZ-exitZ);
	 energyLossTID2->Fill(eloss);
       }else if(tidId.wheel()==3){
	 entryX_exitXTID3->Fill(entryX-exitX);
	 entryY_exitYTID3->Fill(entryY-exitY);
	 entryZ_exitZTID3->Fill(entryZ-exitZ);
	 energyLossTID3->Fill(eloss);
       }
     }else if(tkId.subdetId()==5){
       TOBDetId tobId(tkId);
       entryX_exitXTOB->Fill(entryX-exitX);
       entryY_exitYTOB->Fill(entryY-exitY);
       entryZ_exitZTOB->Fill(entryZ-exitZ);
       energyLossTOB->Fill(eloss);
       if(tobId.layer()==1){
	 entryX_exitXTOB1->Fill(entryX-exitX);
	 entryY_exitYTOB1->Fill(entryY-exitY);
	 entryZ_exitZTOB1->Fill(entryZ-exitZ);
	 energyLossTOB1->Fill(eloss);
       }else if(tobId.layer()==2){
	 entryX_exitXTOB2->Fill(entryX-exitX);
	 entryY_exitYTOB2->Fill(entryY-exitY);
	 entryZ_exitZTOB2->Fill(entryZ-exitZ);
	 energyLossTOB2->Fill(eloss);
       }else if(tobId.layer()==3){
	 entryX_exitXTOB3->Fill(entryX-exitX);
	 entryY_exitYTOB3->Fill(entryY-exitY);
	 entryZ_exitZTOB3->Fill(entryZ-exitZ);
	 energyLossTOB3->Fill(eloss);
       }else if(tobId.layer()==4){
	 entryX_exitXTOB4->Fill(entryX-exitX);
	 entryY_exitYTOB4->Fill(entryY-exitY);
	 entryZ_exitZTOB4->Fill(entryZ-exitZ);
	 energyLossTOB4->Fill(eloss);
       }else if(tobId.layer()==5){
	 entryX_exitXTOB5->Fill(entryX-exitX);
	 entryY_exitYTOB5->Fill(entryY-exitY);
	 entryZ_exitZTOB5->Fill(entryZ-exitZ);
	 energyLossTOB5->Fill(eloss);
       }else if(tobId.layer()==6){
	 entryX_exitXTOB6->Fill(entryX-exitX);
	 entryY_exitYTOB6->Fill(entryY-exitY);
	 entryZ_exitZTOB6->Fill(entryZ-exitZ);
	 energyLossTOB6->Fill(eloss);
       }
     }else if(tkId.subdetId()==6){
       TECDetId tecId(tkId);
       entryX_exitXTEC->Fill(entryX-exitX);
       entryY_exitYTEC->Fill(entryY-exitY);
       entryZ_exitZTEC->Fill(entryZ-exitZ);
       energyLossTEC->Fill(eloss);
       if(tecId.wheel()==1){
	 entryX_exitXTEC1->Fill(entryX-exitX);
	 entryY_exitYTEC1->Fill(entryY-exitY);
	 entryZ_exitZTEC1->Fill(entryZ-exitZ);
	 energyLossTEC1->Fill(eloss);
       }else if(tecId.wheel()==2){
	 entryX_exitXTEC2->Fill(entryX-exitX);
	 entryY_exitYTEC2->Fill(entryY-exitY);
	 entryZ_exitZTEC2->Fill(entryZ-exitZ);
	 energyLossTEC2->Fill(eloss);
       }else if(tecId.wheel()==3){
	 entryX_exitXTEC3->Fill(entryX-exitX);
	 entryY_exitYTEC3->Fill(entryY-exitY);
	 entryZ_exitZTEC3->Fill(entryZ-exitZ);
	 energyLossTEC3->Fill(eloss);
       }else if(tecId.wheel()==4){
	 entryX_exitXTEC4->Fill(entryX-exitX);
	 entryY_exitYTEC4->Fill(entryY-exitY);
	 entryZ_exitZTEC4->Fill(entryZ-exitZ);
	 energyLossTEC4->Fill(eloss);
       }else if(tecId.wheel()==5){
	 entryX_exitXTEC5->Fill(entryX-exitX);
	 entryY_exitYTEC5->Fill(entryY-exitY);
	 entryZ_exitZTEC5->Fill(entryZ-exitZ);
	 energyLossTEC5->Fill(eloss);
       }else if(tecId.wheel()==6){
	 entryX_exitXTEC6->Fill(entryX-exitX);
	 entryY_exitYTEC6->Fill(entryY-exitY);
	 entryZ_exitZTEC6->Fill(entryZ-exitZ);
	 energyLossTEC6->Fill(eloss);
       }else if(tecId.wheel()==7){
	 entryX_exitXTEC7->Fill(entryX-exitX);
	 entryY_exitYTEC7->Fill(entryY-exitY);
	 entryZ_exitZTEC7->Fill(entryZ-exitZ);
	 energyLossTEC7->Fill(eloss);
       }else if(tecId.wheel()==8){
	 entryX_exitXTEC8->Fill(entryX-exitX);
	 entryY_exitYTEC8->Fill(entryY-exitY);
	 entryZ_exitZTEC8->Fill(entryZ-exitZ);
	 energyLossTEC8->Fill(eloss);
       }else if(tecId.wheel()==9){
	 entryX_exitXTEC9->Fill(entryX-exitX);
	 entryY_exitYTEC9->Fill(entryY-exitY);
	 entryZ_exitZTEC9->Fill(entryZ-exitZ);
	 energyLossTEC9->Fill(eloss);
       }
     }

   }

   for(std::vector<GeomDetUnit*>::const_iterator it = pDD->detUnits().begin(); it != pDD->detUnits().end(); it++){
     unsigned int moduleId = (*it)->geographicalId().rawId();  
     if (SimHitMap.find(moduleId) != SimHitMap.end()){
       numbHitsForDet->Fill((SimHitMap.find(moduleId))->second); 
     }
   }	

   return;

}


void SimHitTrackerAnalyzer::endJob(){

  histoPlot->Write() ;
  histoPlot->Close() ;
  
  return ;
}
