#ifndef BeamSpotProducer_BeamSpotAnalyzer_h
#define BeamSpotProducer_BeamSpotAnalyzer_h

/**_________________________________________________________________
   class:   BeamSpotAnalyzer.h
   package: RecoVertex/BeamSpotProducer
   


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: BeamSpotAnalyzer.h,v 1.9 2009/12/18 20:45:07 wmtan Exp $

________________________________________________________________**/


// C++ standard
#include <string>
// CMS
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "RecoVertex/BeamSpotProducer/interface/BeamFitter.h"


class BeamSpotAnalyzer : public edm::EDAnalyzer {
 public:
  explicit BeamSpotAnalyzer(const edm::ParameterSet&);
  ~BeamSpotAnalyzer();

 private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  virtual void beginLuminosityBlock(const edm::LuminosityBlock& lumiSeg, 
									const edm::EventSetup& context) ;
  virtual void endLuminosityBlock(const edm::LuminosityBlock& lumiSeg, 
								  const edm::EventSetup& c);

  int    ftotalevents;
  int fitNLumi_;
  int resetFitNLumi_;
  int countEvt_;       //counter
  int countLumi_;      //counter
  int ftmprun0, ftmprun;
  int ftmplumi0, ftmplumi;
  
  bool write2DB_;
  bool runbeamwidthfit_;
  bool runallfitters_;
  double inputBeamWidth_;

  BeamFitter * theBeamFitter;
};

#endif
