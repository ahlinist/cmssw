// -*- C++ -*-
//
// Package:    TtSemiIdeogramProducer
// Class:      TtSemiIdeogramProducer
// 
/**\class TtSemiIdeogramProducer TtSemiIdeogramProducer.cc AnalysisDataFormats/TopObjectsProducers/src/TtSemiIdeogramProducer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Jan Heyninck
//         Created:  Tue Apr  10 12:01:49 CEST 2007
// $Id: TtSemiIdeogramProducer.h,v 1.1 2007/05/02 15:10:44 lowette Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "AnalysisDataFormats/TopObjects/interface/TtSemiMassSolution.h"
#include "TopQuarkAnalysis/TopMassMeasurement/interface/MtopUncertaintyCalc.h"
#include "TopQuarkAnalysis/TopMassMeasurement/interface/TtSemiIdeogramScan.h"
#include <vector>

using namespace std;

//
// class decleration
//

class TtSemiIdeogramProducer : public edm::EDProducer {
   public:
      explicit TtSemiIdeogramProducer(const edm::ParameterSet&);
      ~TtSemiIdeogramProducer();

      virtual void produce(edm::Event&, const edm::EventSetup&);
      
   private:
      string solChoice_;
      double hadWProbChi2Cut_;
      vector<double> mtvals_;
      MtopUncertaintyCalc *myDMtopCalc;
      TtSemiIdeogramScan  *myIdeogramScan;
};
