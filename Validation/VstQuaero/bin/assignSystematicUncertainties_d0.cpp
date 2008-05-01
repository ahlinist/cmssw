/*********************************************
 assignSystematicUncertainties_d0

 Usage:  assignSystematicUncertainties_d0 <files>

 Bruce Knuteson  August 2004
*********************************************/


#include <unistd.h>
#include "QuaeroUtils/QuaeroRecoObject.hh"
#include "QuaeroUtils/QuaeroEvent.hh"
#include "QuaeroUtils/QuaeroItemFile.hh"
using namespace std;

/* print a vector of systematic errors in my 
   {errorCode/magnitude,errorCode/magnitude,...} notation */
string printError(const vector<string>& errors)
{
  if(errors.empty())
    return("");
  string ans = "{";
  for(int i=0; i<errors.size(); i++)
    ans += errors[i] + (i==errors.size()-1 ? "" : ",");
  ans += "}";
  return(ans);
}


int main(int argc, char* argv[])
{
  if(argc<2)
    {
      cout << "Usage:  assignSystematicUncertainties_d0 <files>" << endl;
      exit(1);
    }
  assert(argc>=2);
  for(int ifile=1; ifile<argc; ifile++)
    {
      string filename = argv[ifile];
      QuaeroItemFile<QuaeroEvent> ef(filename);
      string tmpFilename = "/tmp/tmp_assignSystematicUncertainties_d0_"+Math::ftoa(getpid());
      ofstream fout(tmpFilename.c_str());
      QuaeroEvent e;
      vector<string> errors;
      while(ef.nextEvent(e))
	{
	  if(e.getEventType()=="data")
	    {
	      fout << e << endl;
	      continue;
	    }
	  
	  // Event type
	  fout << e.getEventType() << "  ";
	  
	  // Run Number
	  fout << e.getRunNumber() << "  ";
	  
	  // Weight
	  fout << e.getWeight();
	  errors = vector<string>(0);
	  /* 0801    m       Tevatron Run I luminosity (D0)
	     5% overall luminosity error
	  */
	  errors.push_back("0801/"+Math::ftoa(e.getWeight()*0.05));	  
	  if(e.getEventType()=="sig") // simulated using TurboSim@D0
	    errors.push_back("0881/"+Math::ftoa(e.getWeight()*0.10)); // 10% weight uncertainty	  

	  fout << printError(errors) << " ";

	  // sqrt(s)
	  fout << e.getCollisionType() << "  ";
	  fout << e.getRootS() << "  ";

	  fout << e.getGeneratedSumPt() << "  ";
	  fout << e.getZVtx() << "  ";

	  // objects
	  vector<QuaeroRecoObject> o = e.getObjects();
	  for(int i=0; i<o.size(); i++)
	    {
	      
	      // object type
	      string objectType = o[i].getObjectType();
	      errors = vector<string>(0);
	      fout << objectType;
	      fout << printError(errors) << " ";
	      
	      // mass
	      if((objectType=="j")||(objectType=="b")||(objectType=="uncl"))
		fout << Math::ftoa(Math::toleranceRound(o[i].getFourVector().m(),0.001))+" ";
	      
	      // energy
	      fout << Math::ftoa(Math::toleranceRound(o[i].getFourVector().perp(),0.001));
	      errors = vector<string>(0);

	      /*
		0811    m       D0 EM energy scale
		0812    m       D0 hadronic energy scale
	      */
	      if((o[i].getObjectTypeSansSign()=="e")||
		 (o[i].getObjectType()=="ph"))
		errors.push_back("0811/"+Math::ftoa(Math::toleranceRound(o[i].getFourVector().perp()*0.01,0.001)));
	      if((o[i].getObjectType()=="j")||
		 (o[i].getObjectType()=="b")||
		 (o[i].getObjectType()=="uncl"))
		errors.push_back("0812/"+Math::ftoa(Math::toleranceRound(o[i].getFourVector().perp()*0.03,0.001)));
	      fout << printError(errors) << " ";

	      // Polar angle (costheta, eta)
	      double polar = Math::theta2eta(o[i].getFourVector().theta());
	      fout << Math::ftoa(Math::toleranceRound(polar,0.0001)) << " ";

	      // Azimuthal angle (phi(rad), phi(deg))
	      double azimuthal = o[i].getFourVector().phi()*180/M_PI;
	      fout << Math::ftoa(Math::toleranceRound(azimuthal,0.0001));
	      fout << " ";
	    }
	  fout << ";" << endl;
	}
      fout.close();
      system(("mv "+tmpFilename+" "+filename).c_str());
    }
  return(0);
}


