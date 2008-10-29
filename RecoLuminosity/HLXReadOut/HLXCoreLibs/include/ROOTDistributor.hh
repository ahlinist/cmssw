#ifndef ROOTDISTRIBUTOR_HH
#define ROOTDISTRIBUTOR_HH

/*

Authors: Adam Hunt and John Jones - Princeton University
date: 2007-10-05
email: ahunt@princeton.edu

*/

#include "RecoLuminosity/ROOTSchema/interface/ROOTFileBase.h" //JW
//#include "ROOTSchema/interface/ROOTFileBase.h" //JW

// HLXCoreLibs headers

#include "AbstractDistributor.hh"

#ifndef ROOTDICTCOMPILE
#include "ROOTMutex.hh"
#endif

namespace HCAL_HLX {

  class ROOTDistributor : public ROOTFileBase, public AbstractDistributor {
    
  public:    

    ROOTDistributor();
    ~ROOTDistributor();

    // Called by SectionCollector
    bool ProcessSection(const LUMI_SECTION & lumiSection);

  private:
#ifndef ROOTDICTCOMPILE
    ROOTMutex mROOTMutex;
#endif

  }; //~class ROOTDistributor

}

#endif // ROOTDISTRIBUTOR_HH
