#ifndef ROOTDISTRIBUTOR_HH
#define ROOTDISTRIBUTOR_HH

/*

Authors: Adam Hunt and John Jones - Princeton University
date: 2007-10-05
email: ahunt@princeton.edu

*/

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"

// HLXCoreLibs headers
#include "ICTypeDefs.hh"
#include "LumiStructures.hh"
#include "AbstractDistributor.hh"
#include "ROOTMutex.hh"

namespace HCAL_HLX {

  class ROOTDistributor : public AbstractDistributor {
    
  public:    
    // Additional information not included in LUMI_SECTION
    TTree *m_tree;
    TFile *m_file;
    
    LUMI_SUMMARY *Summary;
    LUMI_BUNCH_CROSSING *BX;
    LUMI_THRESHOLD *Threshold;

    ET_SUM_SECTION    *EtSum;
    OCCUPANCY_SECTION *Occupancy;
    LHC_SECTION       *LHC;

    LUMI_SECTION_HST *LumiSectionHist;
    LEVEL1_HLT_TRIGGER *L1HLTrigger;
    TRIGGER_DEADTIME *TriggerDeadtime;

    ROOTDistributor(std::string filename = "LumiSchema",
		    std::string treename = "LumiTree");
    ~ROOTDistributor();

    // Called by SectionCollector
    bool ProcessSection(const LUMI_SECTION & lumiSection);

    // MBCD = Make Branch, Copy Data
    void MBCD(const ET_SUM_SECTION &in,    ET_SUM_SECTION **out,    int num, unsigned int compress);
    void MBCD(const OCCUPANCY_SECTION &in, OCCUPANCY_SECTION **out, int num, unsigned int compress);
    void MBCD(const LHC_SECTION &in,       LHC_SECTION **out,       int num, unsigned int compress);
    // TO DO: make a template for these three functions

  private:
    ROOTMutex mROOTMutex;
    std::string mBaseFileName;
    std::string mBaseTreeName;

  }; //~class ROOTDistributor

}

#endif // ROOTDISTRIBUTOR_HH
