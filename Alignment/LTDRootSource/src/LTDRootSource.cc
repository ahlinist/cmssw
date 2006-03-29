#include "OpticalAlignment/LTDRootSource/interface/LTDRootSource.h"
//#include "OpticalAlignment/LTDRootSource/interface/OptAlignCSCFileReader.h"

#include "CondFormats/OptAlignObjects/interface/OpticalAlignments.h"
#include "CondFormats/OptAlignObjects/interface/OAQuality.h"
#include "FWCore/Framework/interface/ExternalInputSource.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/InputSourceMacros.h"

#include "TFile.h"
#include "Htypes.h"
#include "TStringLong.h"
#include "TTree.h" 
class TBasket;
class TLeaf;
class TBrowser;
class TDirectory;
//class TFile;
class TClonesArray;

//#include <string>
//#include <vector>
//#include <algorithm>
//#include <fstream>
#include <iostream>
//#include <sstream>
//#include <stdexcept>

LTDRootSource::LTDRootSource ( edm::ParameterSet const& ps
			       , edm::InputSourceDescription const& desc) 
  : edm::ExternalInputSource::ExternalInputSource(ps, desc)
{
  // ... read any configuration parameters specific to your input source
  // ... do any other initialization specific to your input source.
  // note: no argument in the call to produces
  // the standard module label is assumed
  produces<OpticalAlignments>();
}
  
LTDRootSource::~LTDRootSource () { }


bool LTDRootSource::produce ( edm::Event& e ) {

  // filenames come from super class.

  // confirm the file(s) exists and is/are readable.
  std::vector<std::string> fnames(edm::ExternalInputSource::fileNames());
  std::auto_ptr<OpticalAlignments> result(new OpticalAlignments);
  OpticalAlignParam temp;
  OpticalAlignInfo oainfo;
  //Variables
  int edmsID;
  int surveyCode;
  int line;
  int plate;
  int object;
  int side;
  float x;
  float y;
  float z;
  float sx;
  float sy;
  float sz;
  Long64_t measDate;
  Long64_t measTime;
  Long64_t timestampD;
  Long64_t timestampT;
  for ( std::vector<std::string>::const_iterator it = fnames.begin();
	it != fnames.end(); it++ ) {

    TFile *measfile = new TFile(it->c_str(),"READ");
    if(measfile->IsZombie()){
      std::cerr<<"Could not open file!" <<std::endl;
      exit(0);
    }
	
    TTree *tree = (TTree*)measfile->Get("T");
		
    tree->SetBranchAddress("EDMS_ID",&edmsID);
    tree->SetBranchAddress("SurveyCode",&surveyCode);
    tree->SetBranchAddress("Line",&line);
    tree->SetBranchAddress("Plate",&plate);
    tree->SetBranchAddress("Object",&object);
    tree->SetBranchAddress("Side",&side);
    tree->SetBranchAddress("X",&x);
    tree->SetBranchAddress("Y",&y);
    tree->SetBranchAddress("Z",&z);
    tree->SetBranchAddress("sX",&sx);
    tree->SetBranchAddress("sY",&sy);
    tree->SetBranchAddress("sZ",&sz);
    tree->SetBranchAddress("MeasDate",&measDate);
    tree->SetBranchAddress("MeasTime",&measTime);
    tree->SetBranchAddress("TimestampD",&timestampD);
    tree->SetBranchAddress("TimestampT",&timestampT);
	
	
    Int_t nentries = tree->GetEntries();
    std::cout << "number of entries: " << nentries << std::endl;
    for(int i=0; i<nentries; i++){
      tree->GetEntry(i);
      temp.value_  = x;
      temp.error_  = 0.0;
      temp.qual_ = oa_unknown;
      oainfo.x_ = temp;
      temp.value_  = y;
      temp.error_  = 0.0;

      oainfo.y_ = temp;
      temp.value_  = z;
      temp.error_  = 0.0;

      oainfo.z_ = temp;

      temp.value_ = 0.0;
      temp.error_ = 0.0;

      oainfo.angx_ = temp;
      oainfo.angy_ = temp;
      oainfo.angz_ = temp;

      temp.value_ = edmsID;
      oainfo.extraEntries_.push_back(temp);
      temp.value_ = surveyCode;
      oainfo.extraEntries_.push_back(temp);
      temp.value_ = line;
      oainfo.extraEntries_.push_back(temp);
      temp.value_ = plate;
      oainfo.extraEntries_.push_back(temp);
      temp.value_ = object;
      oainfo.extraEntries_.push_back(temp);
      temp.value_ = side;
      oainfo.extraEntries_.push_back(temp);
      result->opticalAlignments_.push_back(oainfo);
      oainfo.extraEntries_.clear();

    }	
    // 		std::cout<<i <<std::endl;
    // 		std::cout<<"EDMS_ID: "<<edmsID<<std::endl;
    // 		std::cout<<"SurveyCode: "<<surveyCode<<std::endl;
    // 		std::cout<<"Line: "<<line<<std::endl;
    // 		std::cout<<"Plate: "<<plate<<std::endl;
    // 		std::cout<<"Object: "<<object<<std::endl;
    // 		std::cout<<"Side: "<<side<<std::endl;
    //		std::cout<<"X: "<<x<<std::endl;
    // 		std::cout<<"Y: "<<y<<std::endl;
    // 		std::cout<<"Z: "<<z<<std::endl;
    // 		std::cout<<"MeasDate: "<<measDate<<std::endl;
    // 		std::cout<<"MeasTime: "<<measTime<<std::endl;
    // 		std::cout<<"TimestampD: "<<timestampD<<std::endl;
    // 	        std::cout<<"TimestampT: "<<timestampT<<std::endl;
    // 		std::cout<<" "<<std::endl;
    // 	}


  } // end for each file name.
  e.put(result);
  return true;
}

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_INPUT_SOURCE(LTDRootSource)
