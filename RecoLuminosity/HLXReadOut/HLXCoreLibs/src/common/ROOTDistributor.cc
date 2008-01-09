#include "ROOTDistributor.hh"
#include "ICUtilityToolbox.hh"

/*

Authors: Adam Hunt and John Jones - Princeton University
Date: 2007-10-05
email: ahunt@princeton.edu
*/


using namespace std;

//
// constructor and destructor
//

namespace HCAL_HLX {

  ROOTDistributor::ROOTDistributor(){}
  
  ROOTDistributor::~ROOTDistributor(){}
  
  bool ROOTDistributor::ProcessSection(const LUMI_SECTION& localSection){
#ifdef DEBUG
  std::cout << "End " << __PRETTY_FUNCTION__ << std::endl;
#endif

    CreateFileName(localSection);

    mROOTMutex.Lock();

    CreateTree(localSection);
    FillTree(localSection);
    CloseTree();

    mROOTMutex.Unlock();
    
#ifdef DEBUG
    //m_tree->Print();
    std::cout << "End " << __PRETTY_FUNCTION__ << std::endl;
#endif

    return true;
  }

} // namespace HCAL_HLX

