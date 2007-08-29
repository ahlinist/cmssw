#ifndef ROOTDISTRIBUTOR_HH
#define ROOTDISTRIBUTOR_HH

//include <TROOT.h>
// Joy of ROOT
#include "TFile.h"
#include "TTree.h"

// HLXCoreLibs stuff
#include "ICTypeDefs.hh"
#include "LumiStructures.hh"
#include "AbstractDistributor.hh"

namespace HCAL_HLX {

  class ROOTDistributor : public AbstractDistributor {
    
  public:    
    // Additional information not included in LUMI_SECTION
    TTree *m_tree;
    TFile *m_file;
    
    LUMI_SUMMARY *Summary;
    LUMI_BUNCH_CROSSING *BX;
    LUMI_THRESHOLD *Threshold;
    LUMI_SECTION* LumiSection;
    LUMI_SECTION_HST *LumiSectionHist;
    LEVEL1_HLT_TRIGGER *L1HLTrigger;
    TRIGGER_DEADTIME *TriggerDeadtime;
    
    ROOTDistributor(const int & runNumber = 0,
                    std::string filename = "LumiSchema",
		    std::string treename = "LumiTree");
    ~ROOTDistributor();

    // Called by SectionCollector
    void ProcessSection(const LUMI_SECTION & lumiSection);
    //void FillTree(const HCAL_HLX::LUMI_SECTION&);

  }; //~class ROOTDistributor

}

#endif // ROOTDISTRIBUTOR_HH
