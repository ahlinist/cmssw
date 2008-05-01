/*********************************************
 assignSystematicUncertainties_aleph

 Usage:  assignSystematicUncertainties_aleph <files>

 Bruce Knuteson  August 2004
*********************************************/


#include <unistd.h>
#include "QuaeroUtils/QuaeroRecoObject.hh"
#include "QuaeroUtils/PartitionRule.hh"
#include "QuaeroUtils/QuaeroEvent.hh"
#include "QuaeroUtils/QuaeroItemFile.hh"
using namespace std;

/* Print a vector of systematic errors in my 
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
      cout << "Usage:  assignSystematicUncertainties_aleph <files>" << endl;
      exit(1);
    }
  assert(argc>=2);

  for(int ifile=1; ifile<argc; ifile++)
    {
      string filename = argv[ifile];
      QuaeroItemFile<QuaeroEvent> ef(filename);
      string tmpFilename = "/tmp/tmp_assignSystematicUncertainties_aleph_"+Math::ftoa(getpid());
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
	  /* 0101    m       LEP Run II luminosity (Aleph)
	     0.1% overall luminosity error
	  */
	  errors.push_back("0101/"+Math::ftoa(e.getWeight()*0.001));	  
	  if(e.getEventType()=="sig") // simulated using TurboSim@Aleph
	    errors.push_back("0181/"+Math::ftoa(e.getWeight()*0.10)); // 10% weight uncertainty	  

	  fout << printError(errors) << " ";

	  // sqrt(s)
	  fout << e.getCollisionType() << "  ";
	  fout << e.getRootS() << "  ";

	  // objects
	  vector<QuaeroRecoObject> o = e.getObjects();
	  for(int i=0; i<o.size(); i++)
	    {
	      
	      // object type
	      string objectType = o[i].getObjectType();
	      errors = vector<string>(0);
	      if(o[i].getObjectTypeSansSign()=="e")
		{
		  string charge = o[i].getObjectType().substr(1);
		  if(drand48() < 0.0001)  // bktemp drand48() < 36./1389
		    {
		      errors.push_back("0131/tau"+charge+"/"+Math::ftoa(drand48()));
		    }
		}
	      fout << objectType;
	      fout << printError(errors) << " ";
	      
	      // mass
	      if((objectType=="j")||(objectType=="b")||(objectType=="uncl"))
		fout << Math::ftoa(Math::toleranceRound(o[i].getFourVector().m(),0.001))+" ";
	      
	      // energy
	      fout << Math::ftoa(Math::toleranceRound(o[i].getFourVector().e(),0.001));
	      errors = vector<string>(0);

	      /*
		0111    m       Aleph EM energy scale
		0115    m       Aleph hadronic energy scale
		0116    m       Aleph muon (track) momentum
	      */
	      if((o[i].getObjectTypeSansSign()=="e")||
		 (o[i].getObjectType()=="ph"))
		errors.push_back("0111/"+Math::ftoa(Math::toleranceRound(o[i].getFourVector().e()*0.001,0.001)));
	      if((o[i].getObjectType()=="j")||
		 (o[i].getObjectType()=="b")||
		 (o[i].getObjectType()=="uncl"))
		errors.push_back("0115/"+Math::ftoa(Math::toleranceRound(o[i].getFourVector().e()*0.002,0.001)));
	      if((o[i].getObjectTypeSansSign()=="mu"))
		errors.push_back("0116/"+Math::ftoa(Math::toleranceRound(o[i].getFourVector().e()*0.001,0.001)));
	      fout << printError(errors) << " ";

	      // Polar angle (costheta, eta)
	      double polar = cos(o[i].getFourVector().theta());
	      fout << Math::ftoa(Math::toleranceRound(polar,0.0001)) << " ";

	      // Azimuthal angle (phi(rad), phi(deg))
	      double azimuthal = o[i].getFourVector().phi();
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

/*

0101    m       LEP Run II luminosity (Aleph)
0111    m       Aleph EM energy scale
0115    m       Aleph hadronic energy scale
0116    m       Aleph muon momentum scale

*/

