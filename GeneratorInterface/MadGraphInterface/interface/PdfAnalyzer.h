// -*- C++ -*-
// Package:    PdfAnalyzer
// Class:      PdfAnalyzer
/**\class PdfAnalyzer PdfAnalyzer.cc GeneratorInterface/MadGraphInterface/src/PdfAnalyzer.cc
 Description: get pdf info for every event
 Implementation:
     <Notes on implementation>
*/
// Original Author: Carsten Hof
//         Created:  Wed Jan 23 12:56:49 CET 2008
// $Id: PdfAnalyzer.h,v 1.2 2008/02/07 13:29:11 dkcira Exp $
#include <memory>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"

class PdfAnalyzer : public edm::EDAnalyzer {
   public:
      explicit PdfAnalyzer(const edm::ParameterSet&);
      ~PdfAnalyzer();
   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
};
