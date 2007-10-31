#include "ROOTDistributor.hh"
#include "ICUtilityToolbox.hh"
#include <iomanip>

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

  ROOTDistributor::ROOTDistributor(std::string baseFileName,
				   std::string baseTreeName) {
    mBaseFileName = baseFileName;
    mBaseTreeName = baseTreeName;
    
    Header  = new LUMI_SECTION_HEADER;
    Summary = new LUMI_SUMMARY;
    BX      = new LUMI_BUNCH_CROSSING;
    
    EtSum     = new ET_SUM_SECTION[HCAL_HLX_MAX_HLXS];
    Occupancy = new OCCUPANCY_SECTION[HCAL_HLX_MAX_HLXS];
    LHC       = new LHC_SECTION[HCAL_HLX_MAX_HLXS];

    Threshold       = new LUMI_THRESHOLD;
    LumiSectionHist = new LUMI_SECTION_HST;
    L1HLTrigger     = new LEVEL1_HLT_TRIGGER;
    TriggerDeadtime = new TRIGGER_DEADTIME;
  }
  
  ROOTDistributor::~ROOTDistributor(){   
    delete Header;
    delete Summary;
    delete BX;

    delete [] EtSum;
    delete [] Occupancy;
    delete [] LHC;

    delete Threshold;
    //    delete LumiSection;
    delete LumiSectionHist;
    delete L1HLTrigger;
    delete TriggerDeadtime;
  }
  
  bool ROOTDistributor::ProcessSection(const LUMI_SECTION& localSection){
    //cout << "Begin " << __PRETTY_FUNCTION__ << endl;

    int i;

    ET_SUM_SECTION    *EtSumPtr[HCAL_HLX_MAX_HLXS];
    OCCUPANCY_SECTION *OccupancyPtr[HCAL_HLX_MAX_HLXS];
    LHC_SECTION       *LHCPtr[HCAL_HLX_MAX_HLXS];

    std::ostringstream outputString;
    outputString << "data/" << "LS__"
		 << setfill('0') << setw(6)  << localSection.hdr.runNumber 
		 << "_" << setfill('0') << setw(6) << localSection.hdr.sectionNumber << ".root";

    string fileName = outputString.str();
    cout << "Output file is " << fileName << endl;    

    mROOTMutex.Lock();
    m_file          = new TFile(fileName.c_str(), "RECREATE");  
    m_file->cd();    
    m_tree          = new TTree(mBaseTreeName.c_str(), "Lumi Section",1);

    m_tree->Bronch("Threshold.",        "HCAL_HLX::LUMI_THRESHOLD",      &Threshold, sizeof(Threshold)/compress);
    m_tree->Bronch("Lumi_Section_Hist.","HCAL_HLX::LUMI_SECTION_HST",    &LumiSectionHist, sizeof(LumiSectionHist)/compress);  
    m_tree->Bronch("Level1_HLTrigger.", "HCAL_HLX::LEVEL1_HLT_TRIGGER",  &L1HLTrigger, sizeof(L1HLTrigger)/compress);
    m_tree->Bronch("Trigger_Deadtime.", "HCAL_HLX::TRIGGER_DEADTIME",    &TriggerDeadtime, sizeof(TriggerDeadtime)/compress);
        
    m_tree->Bronch("Header.",        "HCAL_HLX::LUMI_SECTION_HEADER", &Header, sizeof(Header)/compress);
    m_tree->Bronch("Summary.",       "HCAL_HLX::LUMI_SUMMARY",        &Summary, sizeof(Summary)/compress);
    m_tree->Bronch("BunchCrossing.", "HCAL_HLX::LUMI_BUNCH_CROSSING", &BX, sizeof(BX)/compress);
    
    for(i = 0; i < HCAL_HLX_MAX_HLXS; i++){
      EtSumPtr[i] = &EtSum[i];
      MBCD(localSection.etSum[i], &EtSumPtr[i], i, 100);
      OccupancyPtr[i] = &Occupancy[i];
      MBCD(localSection.occupancy[i], &OccupancyPtr[i], i, 100);
      LHCPtr[i] = &LHC[i];
      MBCD(localSection.lhc[i], &LHCPtr[i], i, 100);
    }
    
    Threshold->Threshold1Set1 = 51;
    Threshold->Threshold2Set1 = 52;
    Threshold->Threshold1Set2 = 53;
    Threshold->Threshold2Set2 = 54;
    Threshold->ET             = 55;
    
    LumiSectionHist->IsDataTaking      = true;
    LumiSectionHist->BeginOrbitNumber  = 61;
    LumiSectionHist->EndOrbitNumber    = 62;
    LumiSectionHist->RunNumber         = 63;
    LumiSectionHist->LumiSectionNumber = 64;
    LumiSectionHist->FillNumber        = 65;
    LumiSectionHist->SecStopTime       = 66;
    LumiSectionHist->SecStartTime      = 67;
    
    L1HLTrigger->TriggerValue          = 71;
    L1HLTrigger->TriggerBitNumber      = 72;
    
    TriggerDeadtime->TriggerDeadtime   = 81;
    
    memcpy(Header,  &localSection.hdr,               sizeof (localSection.hdr));
    memcpy(Summary, &localSection.lumiSummary,       sizeof(localSection.lumiSummary));
    memcpy(BX,      &localSection.lumiBunchCrossing, sizeof(localSection.lumiBunchCrossing));
    
    m_tree->Fill();
    m_file->Write();
    m_file->Close();

    //delete m_tree; // NO!!! root does this when you delete m_file
    delete m_file;
    
    mROOTMutex.Unlock();
    
    return true;
    //m_tree->Print();
    //cout << "End " << __PRETTY_FUNCTION__ << endl;
  }

  // MBCD = Make Branch, Copy Data
  void ROOTDistributor::MBCD(const ET_SUM_SECTION &in, ET_SUM_SECTION **out, int num, unsigned int compress = 100){
#ifdef DEBUG
    cout << "In " << __PRETTY_FUNCTION__ << endl;
#endif
  
    // TODO: Generalize the following line for ET_SUM, OCCUPANCY, and LHC. 
    std::string branchName = "ET_SUM";
    //  Size of class?  Sorry, sizeof(ET_SUM_SECTION) == sizeof(LHC_SECTION).

    std::string className = "HCAL_HLX::" +branchName+ "_SECTION";

    std::ostringstream numString;
    numString << std::dec << num;
    branchName = branchName + ((num / 10 == 0)? "0" : "") + numString.str() + "."; 
    m_tree->Bronch(branchName.c_str() , className.c_str(), out, sizeof(in)/compress); 
    memcpy(*out, &in, sizeof(in));    
  }

  void ROOTDistributor::MBCD(const OCCUPANCY_SECTION &in, OCCUPANCY_SECTION **out, int num, unsigned int compress = 100){
#ifdef DEBUG
    cout << "In " << __PRETTY_FUNCTION__ << endl;
#endif
  
    // TODO: Generalize the following line.
    std::string branchName = "OCCUPANCY";
    //  Size of class?  Sorry, sizeof(ET_SUM_SECTION) == sizeof(LHC_SECTION).

    std::string className = "HCAL_HLX::" + branchName + "_SECTION";
    std::ostringstream numString;

    numString << std::dec << num;
    branchName = branchName + ((num / 10 == 0)? "0" : "") + numString.str() + "."; 
    m_tree->Bronch(branchName.c_str() , className.c_str(), out, sizeof(in)/compress); 
    memcpy(*out, &in, sizeof(in));  
  }

  void ROOTDistributor::MBCD(const LHC_SECTION &in, LHC_SECTION **out, int num,unsigned int compress = 100){
#ifdef DEBUG
    cout << "In " << __PRETTY_FUNCTION__ << endl;
#endif
  
    // TODO: Generalize the following line.
    std::string branchName = "LHC";
    //  Size of class?  Sorry, sizeof(ET_SUM_SECTION) == sizeof(LHC_SECTION).

    std::string className = "HCAL_HLX::" + branchName+ "_SECTION";
    std::ostringstream numString;

    numString << std::dec << num;
    branchName = branchName + ((num / 10 == 0)? "0" : "") + numString.str() + "."; 
    m_tree->Bronch(branchName.c_str() , className.c_str(), out, sizeof(in)/compress); 
    memcpy(*out, &in, sizeof(in));  
  }
} // namespace HCAL_HLX

