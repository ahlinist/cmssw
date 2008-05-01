/************************************
Main routine for the Quaero algorithm
Bruce Knuteson 2003
************************************/

#include <cmath>
#include "Validation/VstQuaero/interface/QuaeroIO.hh"  // IO routines
#include "Validation/VstQuaero/interface/RCPParameters.hh"  // Run Control Parameters
#include "Validation/VstQuaero/interface/quaero.hh"  // Algorithmic routines
using namespace std;

int main(int argc, char* argv[])
{
  srand48(0);  // set random number seed

  string errorMessage="";
  try 
    {


      assert(getenv("QUAERO_CC_DIR")!=NULL);
      assert(getenv("QUAERO_EDITION")!=NULL);
      assert(getenv("QUAERO_HTML_URL")!=NULL);

      QuaeroIO::parseOptions(argc, argv); // parse command line options
      QuaeroIO::parseRequestFile(); // parse the text file that contains the Quaero request
      
      double logLikelihood = 0, err=0;
      logLikelihood = computeAnswer(err); // compute the result, a single number -- all of the work done here
      QuaeroIO::produceEmailResult(logLikelihood,err); // send email result
    }
  catch(string errorMessage)
    {
      QuaeroIO::produceEmailError(errorMessage);
    }
  QuaeroIO::informOurParentProcessThatWeHaveFinishedSuccessfully(); // done!

  return(0);
}
