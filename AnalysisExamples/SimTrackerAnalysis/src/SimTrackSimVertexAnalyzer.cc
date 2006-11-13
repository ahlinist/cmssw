// -*- C++ -*-
//
// Package:    SimTrackerAnalyzer
// Class:      SimTrackSimVertexAnalyzer
// 
/*
 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
//


// system include files
#include <memory>

#include "AnalysisExamples/SimTrackerAnalysis/interface/SimTrackSimVertexAnalyzer.h"

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DetectorDescription/Core/interface/DDCompactView.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"

#include "SimDataFormats/Track/interface/SimTrack.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"
#include "SimDataFormats/Vertex/interface/SimVertex.h"
#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
 
#include "TFile.h"
#include "TH1.h"


SimTrackSimVertexAnalyzer::SimTrackSimVertexAnalyzer( const edm::ParameterSet& iConfig ):
  fOutputFileName(iConfig.getUntrackedParameter("HistOutFile",std::string("TestHiggsMass.root"))),
  SimTkLabel(iConfig.getUntrackedParameter("moduleLabelTk",std::string("g4SimHits"))),
  SimVtxLabel(iConfig.getUntrackedParameter("moduleLabelVtx",std::string("g4SimHits"))),
  numbTk(0),momentumX(0),momentumY(0),momentumZ(0),momentumPERP(0),trackID(0),type(0),
  numbVtx(0),positionZ(0),parentInd(0),histoPlot(0)
{

}

void
SimTrackSimVertexAnalyzer::beginJob(const edm::EventSetup& iSetup){

  histoPlot = new TFile(fOutputFileName.c_str(),"RECREATE");

  numbTk  = new TH1D(  "NumberOfTrack"  , "Number of Tracks" , 100,  0., 100. ) ;
  momentumX  = new TH1D(  "MomentumX"  , "Momentum X" , 100,  -2., 2. ) ;
  momentumY  = new TH1D(  "MomentumY"  , "Momentum Y" , 100,  -2., 2. ) ;
  momentumZ  = new TH1D(  "MomentumZ"  , "Momentum Z" , 100,  -40., 40. ) ;
  momentumPERP  = new TH1D(  "MomentumPerp"  , "Momentum Perp" , 100,  0., 5. ) ;
  trackID  = new TH1D(  "TrackID"  , "Track ID" , 300,  0., 600. ) ;
  type  = new TH1D(  "Type"  , "Hep PDT code" , 200,  -400., 400. ) ;

  numbVtx  = new TH1D(  "NumberOfVertex"  , "Number of Vertex" , 100,  0., 100. ) ;
  positionZ  = new TH1D(  "PositionZ"  , "Position Z" , 200,  -300., 300. ) ;
  parentInd  = new TH1D(  "ParentIndex"  , "Parent Index" , 100,  -10., 500. ) ;

  return;

}

//
// member functions
//

// ------------ method called to produce the data  ------------
void
SimTrackSimVertexAnalyzer::analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
   using namespace edm;

   std::vector<SimTrack> theSimTracks;
   std::vector<SimVertex> theSimVertexes;

   
   Handle<SimTrackContainer> SimTk;
   Handle<SimVertexContainer> SimVtx;

   iEvent.getByLabel(SimTkLabel,SimTk);
   iEvent.getByLabel(SimVtxLabel,SimVtx);

   theSimTracks.insert(theSimTracks.end(),SimTk->begin(),SimTk->end());
   theSimVertexes.insert(theSimVertexes.end(),SimVtx->begin(),SimVtx->end());

   numbTk->Fill(theSimTracks.size());
   for (std::vector<SimTrack>::iterator isimtk = theSimTracks.begin();
	isimtk != theSimTracks.end(); ++isimtk){
     momentumX->Fill(isimtk->momentum().x());
     momentumY->Fill(isimtk->momentum().y());
     momentumZ->Fill(isimtk->momentum().z());
     momentumPERP->Fill(isimtk->momentum().perp());
     trackID->Fill(double(isimtk->trackId()));
     type->Fill(isimtk->type());
   }

   numbVtx->Fill(theSimVertexes.size());
   for (std::vector<SimVertex>::iterator isimvtx = theSimVertexes.begin();
	isimvtx != theSimVertexes.end(); ++isimvtx){
     positionZ->Fill(isimvtx->position().z());
     parentInd->Fill(isimvtx->parentIndex());
   }

   return;

}


void
SimTrackSimVertexAnalyzer::endJob(){

  histoPlot->Write() ;
  histoPlot->Close() ;
  
  return ;
}
