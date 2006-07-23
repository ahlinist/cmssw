/**
 * \file EcalGroupIdBuilder.cc 
 * Determination of crystal group Ids to be used by the 
 * EcalWeightsBuilder to generate weights for SuperModules
 *
 * $Date: 2006/07/19$
 * $Revision: 1.1 $
 * Alexandre Zabi - Imperial College
*/

#include "CalibCalorimetry/EcalWeightsTools/interface/EcalGroupIdBuilder.h"

#include <iostream>
#include <fstream>
#include <vector>

//TO BE DONE:
//check the file: continuity, missing lines, timing to zero, 1700 channels total
//print a report
//mapping to ieta and iphi
//2D plot du mapping des group Id
//tell what is done for particular channels -> if more than 10 channels outside -> new group Id?
//granularity of the group Ids
//plot de timing -> plot de timing en log
//add options of having 1groupid/channel

//This code will produce a file to assign each crystal of the supermodule to
//a group id number according its signal timing.

EcalGroupIdBuilder::EcalGroupIdBuilder(edm::ParameterSet const& pSet)
{
  //verbosity
  verbosity_        = pSet.getUntrackedParameter("verbosity", 1U);

  //Prints out 
  debug_            = pSet.getUntrackedParameter<bool>("debug", false);

  //INPUT FILE: this file contain the timing of all SuperModule channels as 
  // measured with the laser system.
  inputFile_        = pSet.getUntrackedParameter<std::string>("inputFile", "SM03.txt");
 
  //NUMBER OF GROUP IDs 
  // number of group Ids that has to be created and with which granularity in ns
  nGroups_          = pSet.getUntrackedParameter<unsigned int>("nGroups", 3);
  granularity_      = pSet.getUntrackedParameter<double>("granularity", 1.0);

  //SPECIFY WHICH SUPERMODULE
  SuperModule_      = pSet.getUntrackedParameter<unsigned int>("SuperModule", 0);

  //INIT VARIABLES
  // should be set if problem found when reading timing profile
  problem_          = false; 

}//CONSTRUCTOR

EcalGroupIdBuilder::~EcalGroupIdBuilder()
{

}//DESTRUCTOR

void EcalGroupIdBuilder::analyze(const edm::Event& evt, const edm::EventSetup& evtSetup) 
{
  //CREATING OUTPUT FILE
  char* grpid_file = new char[20];
  std::sprintf (grpid_file,"GroupId_SM%02u.out",SuperModule_);
  std::cout << "CREATING OUTPUT GROUPID FILE=" << grpid_file << std::endl;
  std::ofstream groupid_out(grpid_file);

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
      abort();
    }//file check
  
  if(ind != 1700) 
    {std::cout << "ERROR: EcalGroupIdBuilder: Missing crystal timing=" << abs(ind - 1700)
	       << std::endl;
      problem_ = true;
    }//check numbers      
  
  //IF PROBLEMS ABORTING:
  if(problem_){
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
  groupid_out << nGroups_ << " " << mean_timing << std::endl;

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

  //OUTPUT GROUP ID FILE
  for(int unsigned i=0; i < timing.size(); ++i){
    if(timing[i] < (mean_timing - timing_interval))
      {
	groupID0.push_back(i+1);
	groupid_out << i+1 << " " << i/20 << " " << i%20 << " " << 0 << std::endl;
      }//group 0
    if((timing[i] >= (mean_timing - timing_interval)) && (timing[i] <= (mean_timing + timing_interval)))
      {
	groupID1.push_back(i+1);
	groupid_out << i+1 << " " << i/20 << " " << i%20 << " " << 1 << std::endl;
      }//group 1
    if(timing[i] > (mean_timing + timing_interval))
      {
	groupID2.push_back(i+1);
	groupid_out << i+1 << " " << i/20 << " " << i%20 << " " << 2 << std::endl;
      }//group 2
  }//loop on crystals
    
  std::cout << "SUMMARY" << std::endl; 
  std::cout << " NG0=" << groupID0.size() << " NG1=" << groupID1.size() << " NG2=" << groupID2.size() << std::endl; 

  timing_in.close();
  delete [] grpid_file;

}//ANALYZE
