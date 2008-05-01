#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "Validation/VstQuaeroUtils/interface/QuaeroEvent.hh"
#include "Validation/VstQuaeroUtils/interface/QuaeroItemFile.hh"
using namespace std;


int main(int argc, char* argv[])
{
  if(argc==1)
    {
      cout << "Usage:  calculateVariable <files>" << endl;
      exit(1);
    }
  assert(argc>=1);
  QuaeroEvent e;
  double w=0;
  for(int i=1; i<argc; i++)
    {
      QuaeroItemFile<QuaeroEvent> ef(argv[i]); // open Quaero file
      while(ef.nextEvent(e)) // read next Quaero event e
	{
	  QuaeroRecoObject* ele = e.getThisObject("e+",1);
	  if(ele==NULL)
	    ele = e.getThisObject("e-",1);	  
	  if(ele!=NULL)
	    cout << e.getWeight() << " " 
		 << ele->getFourVector().perp() << " "
		 << endl;
	} // loop over all events in file
    } // loop over all files
  return(0);
}
