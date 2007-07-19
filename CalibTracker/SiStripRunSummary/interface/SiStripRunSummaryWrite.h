// Author : Samvel Khalatian (samvel at fnal dot gov) 
// Created: 06/29/07 
// Licence: GPL 

#ifndef CALIBTRACKER_SISTRIPRUNSUMMARY_SISTRIPRUNSUMMARYWRITE_H
#define CALIBTRACKER_SISTRIPRUNSUMMARY_SISTRIPRUNSUMMARYWRITE_H

#include <string>

// #include "FWCore/Framework/interface/EDAnalyzer.h"
#include "CommonTools/ConditionDBWriter/interface/ConditionDBWriter.h"
#include "CondFormats/SiStripObjects/interface/SiStripRunSummary.h"

// Save Compile time by forwarding declarations
#include "FWCore/Framework/interface/Frameworkfwd.h"

// class SiStripRunSummaryWrite: public edm::EDAnalyzer {
class SiStripRunSummaryWrite: public ConditionDBWriter<SiStripRunSummary> {
  public:
    SiStripRunSummaryWrite( const edm::ParameterSet &roCONFIG);
    virtual ~SiStripRunSummaryWrite() {}

  protected:
    // Leave possibility of inheritance
    void algoAnalyze ( const edm::Event      &roEVENT,
		   const edm::EventSetup &roEVENT_SETUP) {}

    void algoEndRun ( const edm::Event      &roEVENT,
		      const edm::EventSetup &roEVENT_SETUP) ;
  
    void algoEndJob(){};

  private:
    virtual SiStripRunSummary *getNewObject();

    // Prevent object copying
    SiStripRunSummaryWrite( const SiStripRunSummaryWrite &);
    SiStripRunSummaryWrite &operator =( const SiStripRunSummaryWrite &);

    std::string oFileXMLIn_;
    std::string oFlagTreeTxt_;
};

#endif // CALIBTRACKER_SISTRIPRUNSUMMARY_SISTRIPRUNSUMMARYWRITE_H
