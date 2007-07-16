// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/05/07
// License: GPL

#ifndef CALIBTRACKER_SISTRIPRUNSUMMARY_SISTRIPRUNSUMMARYREAD_H
#define CALIBTRACKER_SISTRIPRUNSUMMARY_SISTRIPRUNSUMMARYREAD_H

#include "FWCore/Framework/interface/EDAnalyzer.h"

// Forward declarations
#include "FWCore/Framework/interface/Frameworkfwd.h"

class SiStripRunSummaryRead: public edm::EDAnalyzer {
  public:
    // Constructor
    SiStripRunSummaryRead( const edm::ParameterSet &roCONFIG);
    virtual ~SiStripRunSummaryRead() {}

  protected:
    // Leave possibility of inheritance
    virtual void beginJob( const edm::EventSetup &roEVENT_SETUP);
    inline virtual void analyze ( const edm::Event      &roEVENT,
                                  const edm::EventSetup &roEVENT_SETUP) {}
};

#endif // CALIBTRACKER_SISTRIPRUNSUMMARY_SISTRIPRUNSUMMARYREAD_H
