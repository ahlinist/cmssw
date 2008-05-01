/******************************************************************************
  Usage:    reduceByN <N> <files...>  > <newFile>
  Purpose:  Reduce the number of events in a file by a factor of (integer) N, 
            keeping the total weight of the events in the file the same.  
            This is useful in speeding up any subsequent processing that uses 
            the new file, by a factor of N, with loss of statistical power,
            by a factor of roughly sqrt(N).
*******************************************************************************/


#include <cassert>
#include <fstream>
#include <string>
#include "Validation/VstMathUtils/interface/Math.hh"
#include "Validation/VstQuaeroUtils/interface/QuaeroItemFile.hh"
#include "Validation/VstQuaeroUtils/interface/QuaeroEvent.hh"
using namespace std;

int main(int argc, char* argv[])
{
  if(argc==1)
    {
      cout << "Usage:  reduceByN <N> <files...> " << endl;
      exit(1);
    }
  assert(argc>=2);
  int N = atoi(argv[1]);
  for(int i=2; i<argc; i++)
    {
      string tmpFile = Math::getTmpFilename();
      string fileName = argv[i];
      string catCommand = "cat";
      if(fileName.substr(fileName.length()-3)==".gz")
	catCommand = "zcat";
      system((catCommand+" "+fileName+" | awk 'NR % "+Math::ftoa(N)+" == 0' > "+tmpFile).c_str());
      QuaeroItemFile<QuaeroEvent> ef(tmpFile);
      QuaeroEvent e;
      while(ef.nextEvent(e))
	{
	  e.reWeight(e.getWeight()*N);
	  cout << e << endl;
	}
      Math::system("rm "+tmpFile);
    }  
return(0);
}
