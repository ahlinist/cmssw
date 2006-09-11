#ifndef HepMGProductAnalyzer_h
#define HepMGProductAnalyzer_h
// -*- C++ -*-
//
// Package:    HepMGProductAnalyzer
// Class:      HepMGProductAnalyzer
// 
/**\class HepMGProductAnalyzer HepMGProductAnalyzer.cc IOMC/HepMGProductAnalyzer/src/HepMGProductAnalyzer.cc
 A modified version of the HepMCProducts, which is 
 used to print out content of HepMCProducts.
 This version has an additional method to print
 a table of basic 4-vector quantities and produce
 a simple ntuple and plots.

*/
//
// Original Author:  Filip Moortgat
// Additions made: Maria Hansen
//


// system include files
#include <memory>
#include <string>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "TNtuple.h"

class TFile;
class TH1D;

class HepMGProductAnalyzer : public edm::EDAnalyzer {
   public:
      explicit HepMGProductAnalyzer(const edm::ParameterSet&);
      ~HepMGProductAnalyzer();

      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      void printTable(HepMC::GenEvent * event);
   private:
 std::string label_;
 TFile* fOutputFile;
 TH1D* pdgPlot;
 TH1D* etaPlot;
 TH1D* etPlot;
 TH1D* phiPlot;
 TNtuple *nt;
};
#endif
