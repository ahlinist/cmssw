#include <unistd.h>
#include "QuaeroUtils/QuaeroRecoObject.hh"
#include "QuaeroUtils/QuaeroEvent.hh"
#include "QuaeroUtils/QuaeroItemFile.hh"
using namespace std;


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
      cout << "Usage:  assignSystematicUncertainties_cms <fileName>" << endl;
      exit(1);
    }
  assert(argc>=2);
  for(int ifile=1; ifile<argc; ifile++)
    {
  string filename = argv[ifile];
  QuaeroItemFile<QuaeroEvent> ef(filename);
  string tmpFilename = "/tmp/tmp_assignSystematicUncertainties_cms_"+Math::ftoa(getpid());
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

      /*
      if((e.getEventType().substr(0,4)=="pyth")||
	 (e.getEventType()=="sig"))
	errors.push_back("0501/"+Math::ftoa(e.getWeight()*0.05));
      */

      fout << printError(errors) << " ";

      // sqrt(s)
      fout << e.getCollisionType() << "  ";
      fout << e.getRootS() << "  ";
      fout << e.getGeneratedSumPt() << "  ";
      string machineType = "lepton";
      if(e.getRootS()>250)
	machineType = "hadron";

      // z vertex
      if(machineType=="hadron")
	fout << e.getZVtx() << "  ";

      // objects
      vector<QuaeroRecoObject> o = e.getObjects();
      for(int i=0; i<o.size(); i++)
	{

	  // object type
	  string objectType = o[i].getObjectType();
	  fout << objectType;
	  errors = vector<string>(0);

	  /*
	  if(o[i].getObjectTypeSansSign()=="e")
	    errors.push_back("0521/j/"+Math::ftoa(Math::toleranceRound(drand48()*0.01,0.0001)));
	  */
	  
	  fout << printError(errors) << " ";

	  // mass
	  if((objectType=="j")||(objectType=="b")||(objectType=="uncl"))
	    fout << Math::ftoa(Math::toleranceRound(o[i].getFourVector().m(),0.001))+" ";

	  // Energy (E, pT)
	  double e = 0;
	  if(machineType=="hadron")
	    e = o[i].getFourVector().perp();
	  if(machineType=="lepton")
	    e = o[i].getFourVector().e();
	  fout << Math::ftoa(Math::toleranceRound(e,0.001));
	  errors = vector<string>(0);
	  /*
	  if((o[i].getObjectTypeSansSign()=="e")||
	     (o[i].getObjectType()=="ph"))
	    errors.push_back("0511/"+Math::ftoa(Math::toleranceRound(e*0.01,0.001)));
	  if((objectType=="j")||(objectType=="b")||(objectType=="uncl"))
	    errors.push_back("0512/"+Math::ftoa(Math::toleranceRound(e*0.03,0.001)));
	  */
	  fout << printError(errors) << " ";

	  // Polar angle (costheta, eta)
	  double polar = 0.;
	  if(machineType=="hadron")
	    polar = Math::theta2eta(o[i].getFourVector().theta());
	  if(machineType=="lepton")
	    polar = cos(o[i].getFourVector().theta());	  
	  fout << Math::ftoa(Math::toleranceRound(polar,0.001)) << " ";

	  // Azimuthal angle (phi(rad), phi(deg))
	  double azimuthal = 0.;
	  if(machineType=="hadron")
	    azimuthal = fmod((o[i].getFourVector().phi()*180./M_PI) ,360.);
	  if(machineType=="lepton")
	    azimuthal = fmod((o[i].getFourVector().phi()),2*M_PI);
	  fout << Math::ftoa(Math::toleranceRound(azimuthal,0.01)) << " ";

	  fout << " ";
	}
      fout << ";" << endl;
    }
  fout.close();
  system(("mv "+tmpFilename+" "+filename).c_str());
}
  return(0);
}




