#ifndef SimTrackSimVertexAnalyzer_H
#define SimTrackSimVertexAnalyzer_H
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

class SimTrackSimVertexAnalyzer : public edm::EDAnalyzer{
   public:
  explicit SimTrackSimVertexAnalyzer( const edm::ParameterSet& );
  virtual ~SimTrackSimVertexAnalyzer() {};
  
  virtual void analyze( const edm::Event&, const edm::EventSetup& );
  virtual void beginJob( const edm::EventSetup& );
  virtual void endJob();
   private:
  std::string fOutputFileName;
  std::string SimTkLabel;
  std::string SimVtxLabel;

  TH1D* numbTk;
  TH1D* momentumX;
  TH1D* momentumY;
  TH1D* momentumZ;
  TH1D* momentumPERP;
  TH1D* trackID;
  TH1D* type;

  TH1D* numbVtx;
  TH1D* positionZ;
  TH1D* parentInd;
  
  TFile* histoPlot;

};

#endif
