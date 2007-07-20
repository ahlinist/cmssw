#ifndef ECALPROFILEBUILDER_H
#define ECALPROFILEBUILDER_H

//Author: David Wardrope - Imperial College London
//01/03/2007

// system include files
#include <memory>

// user include files
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "TBDataFormats/EcalTBObjects/interface/EcalTBTDCRecInfo.h"
#include "TBDataFormats/EcalTBObjects/interface/EcalTBEventHeader.h"
#include "TBDataFormats/EcalTBObjects/interface/EcalTBHodoscopeRecInfo.h"

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>

#include "TStyle.h"
#include "TFile.h"
#include "TF1.h"
#include "TProfile.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TLine.h"
class TFile;
class TProfile;
class TH2F;

class EcalProfileBuilder : public edm::EDAnalyzer {
 public:

  explicit EcalProfileBuilder(edm::ParameterSet const& pSet);
  ~EcalProfileBuilder();


  virtual void analyze(const edm::Event& evt, const edm::EventSetup& evtSetup);
  virtual void beginJob(edm::EventSetup const&);
  virtual void endJob();

 private:

  std::string digiProducer_;
  std::string tdcRecInfoCollection_;
  std::string tdcRecInfoProducer_;
/*  std::string hodoRecInfoCollection_;
  std::string hodoRecInfoProducer_;*/
  std::string eventHeaderCollection_;
  std::string eventHeaderProducer_;

  std::string  Shapefile_;
  unsigned int verbosity_;
  unsigned int SuperModule_;
  bool         debug_;
  bool         scanmode_;
  int          xtalchosen_;
  int          nentry_;
  int          maxIdRef_;
  int          evt_cnt_;

  EBDetId      xtalInBeam_;

  std::map<int, std::vector<double> > Samples ;
  std::vector<int>    xtalIncluded_;

  std::vector<double> weights_ ;
//  std::map<int, std::vector<double> > TDCbinNorm_ ;
  std::map<int, std::vector<double> > Corrections_ ;
  std::map<int, std::vector<double> > Profiles_ ;
  std::map<int, std::vector<double> > ProfileErrors_ ;
  std::map<int, std::vector<double> >::iterator Xtal_ ;

  std::map<int, TProfile*> p_Shape ;
 // std::map<int, TH2F*> ProfRes;
  std::map<int, TProfile*>::iterator p_TProfIter ;

};
#endif
