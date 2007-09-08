#include "ROOTDistributor.hh"
//#include "IntToString.hh"
#include "ICUtilityToolbox.hh"

using namespace std;

//
// constructor and destructor
//

namespace HCAL_HLX {

  ROOTDistributor::ROOTDistributor(std::string baseFileName,
				   std::string baseTreeName) {
    mBaseFileName = baseFileName;
    mBaseTreeName = baseTreeName;

    Summary         = new LUMI_SUMMARY;
    BX              = new LUMI_BUNCH_CROSSING;
    Threshold       = new LUMI_THRESHOLD;
    LumiSection     = new LUMI_SECTION;
    LumiSectionHist = new LUMI_SECTION_HST;
    L1HLTrigger     = new LEVEL1_HLT_TRIGGER;
    TriggerDeadtime = new TRIGGER_DEADTIME;
  }
  
  ROOTDistributor::~ROOTDistributor(){   
    delete Summary;
    delete BX;
    delete Threshold;
    delete LumiSection;
    delete LumiSectionHist;
    delete L1HLTrigger;
    delete TriggerDeadtime;
  }
  
  bool ROOTDistributor::ProcessSection(const HCAL_HLX::LUMI_SECTION& localSection){
    //cout << "Begin " << __PRETTY_FUNCTION__ << endl;

    unsigned int i, j;

    //Summary
    Summary->DeadtimeNormalization = 50;
    Summary->DeadtimeNormalization = 50;
    Summary->Normalization         = 50;
    Summary->InstantLumi           = 50;
    Summary->InstantLumiErr        = 50;
    Summary->InstantLumiQlty       = 50;
    Summary->InstantETLumi         = 50;
    Summary->InstantETLumiErr      = 50;
    Summary->InstantETLumiQlty     = 50;
    for(i=0; i<2; i++){
      Summary->InstantOccLumi[i]     = 50;
      Summary->InstantOccLumiErr[i]  = 50;
      Summary->InstantOccLumiQlty[i] = 50;
    }

    //Threshold
    Threshold->Threshold1Set1      = 300;
    Threshold->Threshold2Set1      =  10;
    Threshold->Threshold1Set2      = 300;
    Threshold->Threshold2Set2      =  10;
    Threshold->ET                  =  60;
    
    // Bunch Crossing
    for(i=0; i<3564 ; i++){
      BX->ETLumi[i]     = 3;
      BX->ETLumiErr[i]  = 3;
      BX->ETLumiQlty[i] = 3;
      for(j=0; j< 2; j++){
	BX->OccLumi[j][i]    = 3;
	BX->OccLumiErr[j][i] = 3;
	BX->OccLumiQlty[j][i]= 3;
      }
    }
    
    // Section Header
    // TODO: memcpy?!
    //    for(i=0; i < 36; i ++){
    // for(j =0; j <3564; j ++){
    //	LumiSection->etSum[i].data[j] = localSection.etSum[i].data[j] ;
    // }
      
    //}

    std::ostringstream outputString;
    outputString << "data/" << mBaseFileName << "_" 
		 << dec << localSection.hdr.runNumber 
		 << "_" << localSection.hdr.sectionNumber << ".root";

    string fileName = outputString.str();
    cout << "Output file is " << fileName << endl;    

    mROOTMutex.Lock();
    m_file          = new TFile(fileName.c_str(), "RECREATE");  
    m_file->cd();    
    m_tree          = new TTree(mBaseTreeName.c_str(), "Lumi Section",1);
    m_tree->Bronch("Summary.",          "HCAL_HLX::LUMI_SUMMARY",       &Summary, sizeof(HCAL_HLX::LUMI_SUMMARY));
    m_tree->Bronch("BunchCrossing.",    "HCAL_HLX::LUMI_BUNCH_CROSSING",&BX);
    m_tree->Bronch("Threshold.",        "HCAL_HLX::LUMI_THRESHOLD",     &Threshold);
    m_tree->Bronch("LumiSection.",      "HCAL_HLX::LUMI_SECTION",       &LumiSection, sizeof(HCAL_HLX::LUMI_SECTION));
    m_tree->Bronch("Lumi_Section_Hist.","HCAL_HLX::LUMI_SECTION_HST",   &LumiSectionHist);  
    m_tree->Bronch("Level1_HLTrigger.", "HCAL_HLX::LEVEL1_HLT_TRIGGER", &L1HLTrigger);
    m_tree->Bronch("Trigger_Deadtime.", "HCAL_HLX::TRIGGER_DEADTIME",   &TriggerDeadtime);

    memcpy(LumiSection, &localSection, sizeof(localSection));    
    
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

}
