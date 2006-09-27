/**
 * \file EcalGroupIdBuilder.cc 
 * Determination of crystal group Ids to be used by the 
 * EcalWeightsBuilder to generate weights for SuperModules
 *
 * $Date: 2006/07/24 15:28:47 $
 * $Revision: 1.2 $
 * Alexandre Zabi - Imperial College
 *
 * Modif-Alex Zabi 11/08/06
 * Adding the Data Base standard header information
 * Adding the option to create a default GroupId file 
 * in which all crystals are assigned to the same group Id.
 *
 * Modif Alex Zabi 18/08/06
 * GroupID assignment in case of special weights being produced.
 * Crystals with known shape and timing are assigned to 
 * a specific groupID number = 10xxxx (xxxx going from 1 to 1700) 
*/

//header file
#include "CalibCalorimetry/EcalWeightsTools/interface/EcalGroupIdBuilder.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "TFile.h"

//This code will produce a file to assign each crystal of the supermodule to
//a group id number according its signal timing.

EcalGroupIdBuilder::EcalGroupIdBuilder(edm::ParameterSet const& pSet)
{
  //verbosity
  verbosity_        = pSet.getUntrackedParameter("verbosity", 1U);

  //Prints out 
  debug_            = pSet.getUntrackedParameter<bool>("debug", false);

  //Default group ids: if selected, all the crystals are in the 
  //same group id. 
  default_          = pSet.getUntrackedParameter<bool>("default", false);

  //DATA BASE HEADER INFO:
  //Standard or Special groupid file:
  //- standard: when all the channels are assigned to a group id number
  //            according to their timing.
  //- special: For specific crystals, special weights are produced and 
  //           particular group ids are chosen for them: 10XXXX where XXXX 
  //           is the crystal number
  grouptype_        = pSet.getUntrackedParameter<std::string>("grouptype", "standard");

  //This group id file corresponds to a weights file for laser or electron
  //data reconstruction. 
  datatype_         = pSet.getUntrackedParameter<std::string>("datatype", "electron");

  //File version number
  version_          = pSet.getUntrackedParameter<std::string>("version", "V0");

  //INPUT FILE: this file contain the timing of all SuperModule channels as 
  // measured with the laser system.
  inputFile_        = pSet.getUntrackedParameter<std::string>("inputFile", "SM03.txt");
  //In case default group ids are generated, no file need to be specified.
  if(default_) inputFile_   = " ";

  //NUMBER OF GROUP IDs 
  // number of group Ids that has to be created and with which granularity in ns
  nGroups_          = pSet.getUntrackedParameter<unsigned int>("nGroups", 3);
  granularity_      = pSet.getUntrackedParameter<double>("granularity", 1.0);

  //SPECIFY WHICH SUPERMODULE
  SuperModule_      = pSet.getUntrackedParameter<unsigned int>("SuperModule", 0);

  //INIT VARIABLES
  // should be set if problem found when reading timing profile
  problem_          = false; 

  //OUTPUT ROOT FILE
  // Plots of the timing distribution and map the group Id
  rootFile_         = pSet.getUntrackedParameter<std::string>("rootFile", "GroupId.root");
  
  //HISTOS
  TIMING_     = new TH1F("TIMING","TIMING",1000,0,10);
  GROUPIDMAP_ = new TH2F("GROUPIDMAP","GROUPIDMAP",85,0,85,20,0,20);

  //GETTING THE XTAL NUMBERS FOR SPECIAL GROUPIDs
  if(grouptype_ == "special")
    {
      std::cout << "LOOKING FOR CRYSTALS ASSIGNED TO SPECIAL GROUPIDS " << std::endl;
      char* profile_file = new char[20];
      std::sprintf (profile_file,"PROFILES/profile_SM%02u.txt",SuperModule_);
      std::cout << "LOADING FILE= " << profile_file << std::endl;
      std::ifstream profilein(profile_file);
      if(profilein.is_open())
	{
	  int nXtal = 0;
	  profilein >> nXtal;
	  std::cout << "THERE ARE " << nXtal << " AVAILABLE XTALs" << std::endl;

	  for(int ite=0; ite < nXtal; ++ite)
	    {
	      int xtalin;
	      profilein >> xtalin;
	      XtalSpecial_.push_back(xtalin);
	      for(int nb=0; nb < 250; ++nb){
		int    bini  = 0;
		double val   = 0.0;
		double error = 0.0;
		profilein >> bini >> val >> error;
	      }//bin
	    }//profile
	  
	  for(unsigned int y=0; y<XtalSpecial_.size(); ++y)
	    std::cout << XtalSpecial_[y] << " ";
	  std::cout << std::endl;
	}	  
      else {
	std::cout << "ERROR: EcalGroupIdBuilder: could not open profile file " << profile_file
		  << "Aborting!" << std::endl;
	if(!default_) abort();
      }//check file
      profilein.close();
    }//special

}//CONSTRUCTOR

EcalGroupIdBuilder::~EcalGroupIdBuilder()
{
  TFile f(rootFile_.c_str(),"RECREATE");
  TIMING_->Write();
  GROUPIDMAP_->Write();
  f.Close();
}//DESTRUCTOR

void EcalGroupIdBuilder::analyze(const edm::Event& evt, const edm::EventSetup& evtSetup) 
{
  //CREATING OUTPUT FILE
  char* grpid_file = new char[20];
  std::sprintf (grpid_file,"GroupId_SM%02u_%s.out",SuperModule_,version_.c_str());
  std::cout << "CREATING OUTPUT GROUPID FILE=" << grpid_file << std::endl;
  std::ofstream groupid_out(grpid_file);

  //DEFAULT GROUPIDs
  if(default_) 
    {
      std::cout << std::endl;
      std::cout << "WARNING: CREATING DEFAULT GROUPIDs" << std::endl;
      std::cout << std::endl;
    }//default

  //WRITING STANDARD DATA BASE HEADER TO THE FILE
  groupid_out << SuperModule_ << std::endl;
  groupid_out << "notused"    << std::endl;
  groupid_out << grouptype_   << std::endl;
  groupid_out << datatype_    << std::endl;
  groupid_out << version_     << std::endl;
  groupid_out << "notused"    << std::endl;
	      
  //DETERMINING THE GROUP-ID ACCORDING TO THE SIGNAL TIMING ///////////////////////////////////////////////////////
  std::cout << "LOADING FILE=" << inputFile_.c_str() << std::endl;
  std::ifstream timing_in(inputFile_.c_str());

  std::vector<double> timing;
  double mean_timing = 0.0;
  int    ind         = 0;
  //READING TIMING FILE
  if(timing_in.is_open())
    {
      while(!timing_in.eof())
	{
	  unsigned int SM; int xtal; double time = 0.0;
	  timing_in >> SM >> xtal >> time;
	  
	  if(SM != SuperModule_) 
	    {std::cout << "ERROR: EcalGroupIdBuilder: wrong super module file SM=" << SM 
		       << " instead of chosen SM=" << SuperModule_ << std::endl;
	      problem_ = true;
	    }//check SuperModule
	  
	  if(ind+1 != xtal)
	    {std::cout << "ERROR: EcalGroupIdBuilder: Missing crystal in file   =" << ind+1 
		       << std::endl;
	      problem_ = true;
	    }//check xtal
	  
	  timing.push_back(time);
	  mean_timing += time;

	  ind++;
	}//loop timing file
     }
  else
    {
      std::cout << "ERROR: EcalGroupIdBuilder: could not open timing file " << inputFile_
		<< "Aborting!" << std::endl;
      if(!default_) abort();
    }//file check
  
  if(ind != 1700) 
    {std::cout << "ERROR: EcalGroupIdBuilder: Missing crystal timing=" << abs(ind - 1700)
	       << std::endl;
      problem_ = true;
    }//check numbers      
  
  //IF PROBLEMS ABORTING:
  if(problem_ && !default_){
    std::cout << "ABORT: EcalGroupIdBuilder: problem in timing file" 
	      << std::endl;
    abort();
  }//problems
  mean_timing/=double(timing.size());

  //VERIFYING RMS
  double rms_timing  = 0.0;
  for(int unsigned i=0; i < timing.size(); ++i)
    rms_timing += (timing[i]-mean_timing)*(timing[i]-mean_timing);    
  rms_timing = sqrt(rms_timing/double(timing.size()));

  std::cout << "MEAN=" << mean_timing << " RMS=" << rms_timing << std::endl;

  double new_mean_timing = 0.0;
  std::vector<int> xtal_problem;
  for(int unsigned i=0; i < timing.size(); ++i)
    if((timing[i] > (mean_timing + 5*rms_timing)) || (timing[i] < (mean_timing - 5*rms_timing))){
      if(debug_) std::cout << "INFO: EcalGroupIdBuilder: crystal with timing oustide of +/-5 RMS=" << i+1 << " " 
			   << timing[i] << std::endl;
      xtal_problem.push_back(i);
    } else new_mean_timing += timing[i];
  new_mean_timing /=(double(timing.size() - xtal_problem.size()));
  mean_timing = new_mean_timing;

  //saving mean
  if(default_) groupid_out << 1 << " " << 5.5 << std::endl;
  else groupid_out << nGroups_ + XtalSpecial_.size() << " " << mean_timing << std::endl;

  double min_timing = 999.9;
  double max_timing = 0.0;  
  rms_timing = 0.0;
  for(int unsigned i=0; i < timing.size(); ++i)
    { 
      bool discard_channel = false;
      for(int unsigned j=0; j < xtal_problem.size(); ++j){
	int unsigned xtalp = xtal_problem[j];
	if(i == xtalp) discard_channel = true;
      }//loop xtal_problem

      if(!discard_channel) {
	rms_timing += (timing[i]-mean_timing)*(timing[i]-mean_timing);
	if(timing[i] < min_timing) min_timing = timing[i];
	if(timing[i] > max_timing) max_timing = timing[i];

	TIMING_->Fill(timing[i]);
      }//kept channels
    }//loop channels

  rms_timing = sqrt(rms_timing/double(timing.size() - xtal_problem.size()));
  int nGroupIds = int((max_timing - min_timing)/(2*granularity_/25.0)) + 1;
  
  if(debug_){
    std::cout << "NEW RMS after removal of channel not within +/- 5 RMS" << std::endl;
    std::cout << "MEAN=" << mean_timing << " RMS=" << rms_timing << std::endl;
    std::cout << "MEAN(ns)=" << mean_timing*25.0 << " RMS(ns)=" << rms_timing*25.0 << std::endl;
    std::cout << std::endl;
    std::cout << "MIN=" << min_timing << " MAX=" << max_timing << std::endl;
    std::cout << "Number of GroupIDs possible=" << nGroupIds << std::endl;
    std::cout << std::endl;
  }//debug

  //CREATING THE GROUP IDs
  double timing_interval = granularity_/25.0;
  
  if(debug_){
    std::cout << "Interval chosen for groupIds= +/-" << timing_interval << " = +/-" << granularity_ << " ns" << std::endl;
    for(int unsigned i=0; i < timing.size(); ++i){
      if((timing[i] < (mean_timing - 3*timing_interval)) || (timing[i] > (mean_timing + 3*timing_interval)))
	std::cout << "INFO: EcalGroupIdBuilder: channel outside group intervals =" << i+1 << " " 
		  << timing[i] << std::endl;
    }//loop channels
  }//debug

  std::vector<int> groupID0;
  std::vector<int> groupID1;
  std::vector<int> groupID2;

  if(default_)
    for(int unsigned i=0; i < 1700; ++i){
      groupid_out << i+1 << " " << i/20 << " " << i%20 << " " << 0 << std::endl;
    }//loop crystals
  else{

    //OUTPUT GROUP ID FILE
    for(int unsigned i=0; i < timing.size(); ++i){

      if(grouptype_ == "special") {
	bool found = false;
	for(unsigned int ispe=0; ispe < XtalSpecial_.size(); ++ispe)
	  if((i+1) == XtalSpecial_[ispe]) {
	    std::cout << "SPECIAL GROUPID for XTAL=" << XtalSpecial_[ispe] << std::endl;
	    groupid_out << i+1 << " " << i/20 << " " << i%20 << " " << 10000+i+1 << std::endl;
	    found = true;
	  }//found
	if(found) continue;
      }//special

      if(timing[i] < (mean_timing - timing_interval))
	{
	  groupID0.push_back(i+1);
	  groupid_out << i+1 << " " << i/20 << " " << i%20 << " " << 0 << std::endl;
	  GROUPIDMAP_->Fill(i/20,i%20,0);
	}//group 0
      if((timing[i] >= (mean_timing - timing_interval)) && (timing[i] <= (mean_timing + timing_interval)))
	{
	  groupID1.push_back(i+1);
	  groupid_out << i+1 << " " << i/20 << " " << i%20 << " " << 1 << std::endl;
	  GROUPIDMAP_->Fill(i/20,i%20,1);
	}//group 1
      if(timing[i] > (mean_timing + timing_interval))
	{
	  groupID2.push_back(i+1);
	  groupid_out << i+1 << " " << i/20 << " " << i%20 << " " << 2 << std::endl;
	  GROUPIDMAP_->Fill(i/20,i%20,2);
	}//group 2
    }//loop on crystals
  
  }//groupid assignment
  
  std::cout << "SUMMARY" << std::endl; 
  std::cout << " NG0=" << groupID0.size() << " NG1=" << groupID1.size() << " NG2=" << groupID2.size() << std::endl; 

  timing_in.close();
  delete [] grpid_file;

}//ANALYZE
