/******************************
 Perform a cleanup of a Quaero submission, providing an error message to the requestor by email if the job crashes.
 Bruce Knuteson 2003
*******************************/


#include <iostream>
#include <fstream>
#include <string>
#include "Quaero/QuaeroIO.hh"
#include "Quaero/RCPParameters.hh"
using namespace std;

int main(int argc, char* argv[])
{
  if(argc!=4)
    {
      cout << "Usage: cleanup <localDir> <id> <requestor>" << endl;
      exit(0);
    }
  string localDir = argv[1];
  string id = argv[2];
  GlobalVariables::id = id;
  string requestor = argv[3];
  ifstream fdone((localDir+"/requests/justfinished/"+id).c_str());
  string doneQ="";
  system(("echo ' ' > "+localDir+"/requests/"+id+"/done").c_str());
  if((fdone)&&((fdone >> doneQ)&&(doneQ=="done!")))
    {
      fdone.close();
      system(("rm "+localDir+"/requests/justfinished/"+id+";").c_str());
      //system(("rm "+localDir+"/batchtmp/quaero_"+id+";").c_str());
    }
  else
    {
      string errormessage = "\n       ** Your job failed to properly execute **\n\nPlease resubmit your job after making the appropriate modifications.  If you continue to encounter this or other error messages, please contact the Quaero helpline at knuteson@fnal.gov.\n\nYour logfile is available at "+QuaeroIO::getFilename("resultURL")+"\n\n";
      system(("echo '"+errormessage+"' >> "+QuaeroIO::getFilename("answer")).c_str());
      // system(("(cat "+localDir+"/requests/justfinished/errormsg.txt "+localDir+"/requests/"+id+"/answer.txt) | cat > "+localDir+"/requests/"+id+"/answer.txt; cat "+localDir+"/requests/"+id+"/answer.txt "+localDir+"/requests/"+id+"/logfile.txt > "+localDir+"/requests/justfinished/tmp.txt; mail -oi 'knuteson@fnal.gov' < "+localDir+"/requests/justfinished/tmp.txt; mail -oi '"+requestor+"' < "+localDir+"/requests/justfinished/tmp.txt; rm "+localDir+"/requests/justfinished/tmp.txt ").c_str());
    }
  return(0);
}
