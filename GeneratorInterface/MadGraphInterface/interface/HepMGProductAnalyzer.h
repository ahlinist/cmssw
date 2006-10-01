#ifndef HepMGProductAnalyzer_h
#define HepMGProductAnalyzer_h
// -*- C++ -*-
//
// Package:    HepMGProductAnalyzer
// Class:      HepMGProductAnalyzer
// 
/**\class HepMGProductAnalyzer HepMGProductAnalyzer.cc IOMC/HepMGProductAnalyzer/src/HepMGProductAnalyzer.cc
 A modified version of the HepMCProducts, which is 
 used to print out the content of HepMC products.
 This version has an additional method to print
 a table of particles and their decay products.

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


class HepMGProductAnalyzer : public edm::EDAnalyzer {
   public:
      explicit HepMGProductAnalyzer(const edm::ParameterSet&);
      ~HepMGProductAnalyzer();

      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      void printTable(HepMC::GenEvent * event);
 private:
      std::string label_;
};
#endif
