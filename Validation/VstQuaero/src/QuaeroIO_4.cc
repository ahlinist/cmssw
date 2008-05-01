
/***********************************
Implementation of QuaeroIO namespace
Bruce Knuteson 2003
***********************************/


#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <numeric>
#include "Validation/VstMathUtils/interface/Math.hh"
#include "Validation/VstQuaero/interface/QuaeroIO.hh"
#include "Validation/VstQuaeroUtils/interface/generate.hh"
#include "Validation/VstQuaero/interface/QuaeroSysObject.hh"
#include "Validation/VstQuaero/interface/RCPParameters.hh"
using namespace std;


/*****  Midgame:  File manipulations  *****/


void QuaeroIO::makeTmpDirectory(int iSystematicShift)
{
  // make the temporary directory
  string tmpDir = getFilename("tmpDir");
  if(iSystematicShift==-4)
    {
      system(("rm -r "+tmpDir+" "+GlobalVariables::silencerString).c_str());
      system(("mkdir -p "+tmpDir).c_str());
      system(("mkdir "+tmpDir+"/relevantBkg/").c_str());
      for(map<string,vector<string> >::iterator iColliderRun=RCPParameters::colliderExperiments.begin();
	  iColliderRun!=RCPParameters::colliderExperiments.end(); iColliderRun++)
	{
	  string colliderRun = iColliderRun->first;
	  system(("mkdir "+tmpDir+"/relevantBkg/"+colliderRun+"/").c_str());     
	  system(("mkdir "+tmpDir+"/discriminator/"+colliderRun+"/").c_str());     
	  for(int i=0; i<iColliderRun->second.size(); i++)
	    {
	      string experiment = iColliderRun->second[i];
	      system(("mkdir "+tmpDir+"/relevantBkg/"+colliderRun+"/"+experiment+"/").c_str());
	      system(("mkdir "+tmpDir+"/discriminator/"+colliderRun+"/"+experiment+"/").c_str());
	    }
	}
    }
  else
    {
      string tmpSysDir = tmpDir + Math::ftoa(iSystematicShift)+"/";
      system(("mkdir "+tmpSysDir).c_str());
      for(map<string,vector<string> >::iterator iColliderRun=RCPParameters::colliderExperiments.begin();
	  iColliderRun!=RCPParameters::colliderExperiments.end(); iColliderRun++)
	{
	  string colliderRun = iColliderRun->first;
	  string tmpSysColliderDir = tmpSysDir+colliderRun+"/";
	  system(("mkdir "+tmpSysColliderDir).c_str());
	  for(int i=0; i<iColliderRun->second.size(); i++)
	    {
	      string experiment = iColliderRun->second[i];
	      string tmpSysColliderExperimentDir = tmpSysColliderDir+experiment+"/";
	      system(("mkdir "+tmpSysColliderExperimentDir).c_str());
	      system(("mkdir "+tmpSysColliderExperimentDir+"/sig/").c_str());
	      system(("mkdir "+tmpSysColliderExperimentDir+"/bkg/").c_str());
	    }
	}
    }
  return;
}

void QuaeroIO::removeTmpDirectory(int iSystematicShift)
{
  if(iSystematicShift>=-1)
    {
      system(("rm -rf "+getFilename("tmpDir")+"/"+Math::ftoa(iSystematicShift)+"/ "+GlobalVariables::silencerString).c_str());
      system(("rm -rf "+getFilename("tmpDir")+"/"+Math::ftoa(iSystematicShift)+"/ "+GlobalVariables::silencerString).c_str()); // do this twice
    }
  else
    {
      system(("rm -rf "+getFilename("tmpDir")+"/ "+GlobalVariables::silencerString).c_str());
      system(("rm -rf "+getFilename("tmpDir")+"/ "+GlobalVariables::silencerString).c_str()); // do this twice
    }
  return;
}


/*****  Endgame:  Finishing routines  *****/

void QuaeroIO::produceEmailResult(double logLikelihood, double err)
{
  string tmpFile1 = getFilename("tmp");
  ofstream fans(tmpFile1.c_str());

  fans << endl << "Result\n" << endl << "  log10[ p(data | your hypothesis) / p(data | standard model) ] = " << Math::nice(logLikelihood,+1) << endl;
  if(Math::nice(logLikelihood,+1)==0)
    {
      fans << endl << "Logfile\n" << endl 
	   << "  The logfile of this request is available at " << endl 
	   << getFilename("resultURL")+"." << endl 
	   << "  Questions concerning the details of execution should be sent to knuteson@mit.edu." << endl << endl;
    }
  else
    {
      cout << endl 
	   << "      d(Answer)/d(GraduateStudent) = " << Math::nice(err,+1) << endl;
      cout << "        This derivative can be reduced by increasing Quaero's target analysis time" << endl << endl;
      fans << endl << "Plots\n" << endl 
	   << "  Plots of the variables used are available for viewing at " << endl 
	   << getFilename("resultURL")+"." << endl 
	   << "  The red curve is the prediction of the reference model; the green curve is your hypothesis; the black dots are data." << endl << endl;
    }

  fans.close();
    
  system(("cat "+tmpFile1+" >> "+getFilename("answer")).c_str()); 
  system(("rm "+tmpFile1).c_str());

  ofstream findexHtml(getFilename("index.html").c_str());
  findexHtml << "<html><body><center>" << endl;
  findexHtml << "<h1>" << RCPParameters::requestorInfo[2] << "<br>" << endl;
  findexHtml << RCPParameters::modelDescription << "<br>" << endl;
  findexHtml << "logL = " << Math::nice(logLikelihood,+1) << "<br>" << endl;
  findexHtml << "<h3>" << endl;
  findexHtml << "<a href=\"answer.txt\">email</a><br>" << endl;
  if(Math::nice(logLikelihood,+1)!=0)
    findexHtml << "<a href=\"plots.ps.gz\">plots</a><br>" << endl;
  findexHtml << "<a href=\"generatorfiles/\">generator</a><br>" << endl;
  findexHtml << "<a href=\"logfile.txt\">logfile</a><br>" << endl;
  if(Math::nice(logLikelihood,+1)!=0)
    findexHtml << "<a href=\".plots.ps.gz\">.plots</a><br>" << endl;
  findexHtml << "</body></html>" << endl;
  findexHtml.close();

  /*
  if(RCPParameters::debugLevel>0)
    cout << "Sending email with results . . . " << endl;
  system(("mail -f 'Quaero <knuteson@fnal.gov>' -oi '"+RCPParameters::requestorInfo[2]+"' < "+getFilename("answer")).c_str());
  system(("mail -f 'Quaero <knuteson@fnal.gov>' -oi 'knuteson@mit.edu' < "+getFilename("answer")).c_str());
  */

  return;
}      

void QuaeroIO::produceEmailError(string errormessage)
{
  //errormessage = "Subject: Quaero Request #"+GlobalVariables::id+"\n\n"
  errormessage = "\n\n       ** Your job failed to properly execute **\n\n"+errormessage+"\n\nPlease resubmit your job after making the appropriate modifications.  If you continue to encounter this or other error messages, please contact the Quaero helpline at knuteson@fnal.gov.\n\nYour logfile is available at "+QuaeroIO::getFilename("resultURL")+"\n\n";
  system(("echo '"+errormessage+"' >> "+getFilename("answer")).c_str());
  //system(("mail -f 'Quaero <knuteson@fnal.gov>' -oi '"+RCPParameters::requestorInfo[2]+",knuteson@fnal.gov' < "+getFilename("error")).c_str());
  return;
}

void QuaeroIO::informOurParentProcessThatWeHaveFinishedSuccessfully()
{
  //system("sleep 60"); // give lsf a chance to finish writing logfiles and so forth before cleaning the tmp directory
  if(RCPParameters::debugLevel<=2)
    removeTmpDirectory();
  ofstream fdone1((GlobalVariables::localDir+"requests/justfinished/"+GlobalVariables::id).c_str());
  fdone1 << "done!" << endl;
  fdone1.close();
  ofstream fdone2((GlobalVariables::localDir+"requests/"+GlobalVariables::id+"/done").c_str());
  fdone2 << "done!" << endl;
  fdone2.close();
  cout << "done!" << string(70,' ') << flush;  system("date");
  return;
}

