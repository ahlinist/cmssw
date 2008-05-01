/*********************************************
 assignSystematicUncertainties_h1

 Usage:  assignSystematicUncertainties_h1 <files>

 Bruce Knuteson  May 2004
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


/* Uncertainty in track identification, as a function of polar angle */
double trackIdentificationEfficiencyUncertainty(double theta)
{
  double th = theta*180/M_PI;
  double ans = 0;
  if(th<37)
    ans = 0.07;
  else
    ans = 0.02;
  return(ans);
}

/* Z position of center of gravity of EM shower in EM LAr calorimeter */ 
double calculateZpositionInLArCalorimeter(double zVertexPosition, double objectEta)
{
  double theta = Math::eta2theta(objectEta);
  double result=1000;
  if(abs(objectEta)<4)
    result = 125./tan(theta)+zVertexPosition;
  return(result);
}

/* Angular systematic error on the polar angle of EM clusters */
double angularSystematicErrorOnEMClusters(double theta)
{
  double polar = theta*180/M_PI;
  assert(polar>=0);
  assert(polar<=180);
  if(polar < 120.)
    return(0.003);
  else if(polar < 135.)
    return(0.002);
  else
    return(0.001);
  assert(false);
  return(0);
}


int main(int argc, char* argv[])
{
  if(argc<2)
    {
      cout << "Usage:  assignSystematicUncertainties_h1 <files>" << endl;
      exit(1);
    }
  assert(argc>=2);
  for(int ifile=1; ifile<argc; ifile++)
    {
      string filename = argv[ifile];
      QuaeroItemFile<QuaeroEvent> ef(filename);
      string tmpFilename = "/tmp/tmp_assignSystematicUncertainties_h1_"+Math::ftoa(getpid());
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
	  /* 0501    m       HERA Run I luminosity (H1)
	     1.5% overall luminosity error
	     0521    m       Jet trigger efficiency
	     0522    m       Muon trigger efficiency
	     0531    i       Track identification efficiency
	     0532    m       Muon identification uncertainty */
	  errors.push_back("0501/"+Math::ftoa(e.getWeight()*0.015));	  
	  if((e.numberOfObjects("e")>=1)||
	     (e.numberOfObjects("ph")>=1))
	    ;
	  else 
	    if(e.numberOfObjects("j")>=1)
	      errors.push_back("0521/"+Math::ftoa(e.getWeight()*0.03));
	    else
	      errors.push_back("0522/"+Math::ftoa(e.getWeight()*0.05));
	  if((e.numberOfObjects("e")+e.numberOfObjects("ph"))>=1)
	    {
	      double q = 0;
	      for(int i=0; i<e.numberOfObjects("e+"); i++)
		q += trackIdentificationEfficiencyUncertainty(e.getThisObject("e+",i+1)->getFourVector().theta());
	      for(int i=0; i<e.numberOfObjects("e-"); i++)
		q += trackIdentificationEfficiencyUncertainty(e.getThisObject("e-",i+1)->getFourVector().theta());
	      for(int i=0; i<e.numberOfObjects("ph"); i++)
		q += trackIdentificationEfficiencyUncertainty(e.getThisObject("ph",i+1)->getFourVector().theta());
	      errors.push_back("0531/"+Math::ftoa(Math::toleranceRound(e.getWeight()*q,0.001)));
	    }
	  if(e.numberOfObjects("mu")>=1)
	    errors.push_back("0532/"+Math::ftoa(Math::toleranceRound(e.getWeight()*0.05)));

	  fout << printError(errors) << " ";

	  // sqrt(s)
	  fout << e.getRootS() << "  ";

	  // z vertex
	  fout << e.getZVtx() << "  ";
	  
	  // objects
	  vector<QuaeroRecoObject> o = e.getObjects();
	  for(int i=0; i<o.size(); i++)
	    {
	      
	      // object type
	      string objectType = o[i].getObjectType();
	      fout << objectType << " ";
	      
	      // mass
	      if((objectType=="j")||(objectType=="b")||(objectType=="uncl"))
		fout << Math::ftoa(Math::toleranceRound(o[i].getFourVector().m(),0.001))+" ";
	      
	      // pT
	      fout << Math::ftoa(Math::toleranceRound(o[i].getFourVector().perp(),0.001));
	      errors = vector<string>(0);
	      if((o[i].getObjectTypeSansSign()=="e")||
		 (o[i].getObjectType()=="ph")) // EM object
		{
		  double z = calculateZpositionInLArCalorimeter(e.getZVtx(), Math::theta2eta(o[i].getFourVector().theta()));
		  /*
		    0511    m       H1 EM energy scale (z < -145cm)
		    0512    m       H1 EM energy scale (-145 < z < 20cm)
		    0513    m       H1 EM energy scale (20 < z < 100cm)
		    0514    m       H1 EM energy scale (100cm < z)
		  */
		  if(z<=-152.5)
		    errors.push_back("0511/"+Math::ftoa(Math::toleranceRound(o[i].getFourVector().perp()*0.01,0.001)));	      
		  else if(z<=20.0)
		    errors.push_back("0512/"+Math::ftoa(Math::toleranceRound(o[i].getFourVector().perp()*0.007,0.001)));	      
		  else if(z<=95.0)
		    errors.push_back("0513/"+Math::ftoa(Math::toleranceRound(o[i].getFourVector().perp()*0.015,0.001)));	      
		  else
		    errors.push_back("0514/"+Math::ftoa(Math::toleranceRound(o[i].getFourVector().perp()*0.03,0.001)));	      
		}

	      /*
		0515    m       H1 hadronic energy scale
		0516    m       H1 muon energy scale
	      */
	      if((o[i].getObjectType()=="j")||
		 (o[i].getObjectType()=="b")||
		 (o[i].getObjectType()=="uncl"))
		errors.push_back("0515/"+Math::ftoa(Math::toleranceRound(o[i].getFourVector().perp()*0.02,0.001)));
	      if((o[i].getObjectTypeSansSign()=="mu"))
		errors.push_back("0516/"+Math::ftoa(Math::toleranceRound(o[i].getFourVector().perp()*0.05,0.001)));
	      fout << printError(errors) << " ";

	      // Polar angle (costheta, eta)
	      double polar = Math::theta2eta(o[i].getFourVector().theta());
	      fout << Math::ftoa(Math::toleranceRound(polar,0.001));
	      errors = vector<string>(0);
	      /* The uncertainty on the jet polar angle is 5mrad for theta < 30 degrees,
		 and 10 mrad for theta > 30 degrees.
		 Error code 0001 is instantiated as a (different) Gaussian 
		 distributed random number every time it appears */ 
	      if(o[i].getObjectTypeSansSign()=="j")
		if(o[i].getFourVector().theta()*180/M_PI<30)
		  errors.push_back("0001/0.005");
		else
		  errors.push_back("0001/0.010");
	      /* The angular uncertainty of collimated electromagnetic clusters varies 
		 theta dependent between 1 and 3 mrad */
	      if((o[i].getObjectTypeSansSign()=="e")||
		 (o[i].getObjectType()=="ph"))
		errors.push_back("0001/"+Math::ftoa(Math::toleranceRound(angularSystematicErrorOnEMClusters(o[i].getFourVector().theta()))));
	      /* The uncertainty on the muon polar angle determination is 3 mrad */
	      if((o[i].getObjectTypeSansSign()=="mu"))
		errors.push_back("0001/0.003");
	      fout << printError(errors) << " ";

	      // Azimuthal angle (phi(rad), phi(deg))
	      double azimuthal = fmod((o[i].getFourVector().phi()*180./M_PI) ,360.);
	      fout << Math::ftoa(Math::toleranceRound(azimuthal,0.01));
	      errors = vector<string>(0);
	      errors.push_back("0001/0.001");
	      fout << printError(errors) << " ";
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

0501    m       HERA Run I luminosity (H1)
0511    m       H1 EM energy scale (z < -145cm)
0512    m       H1 EM energy scale (-145 < z < 20cm)
0513    m       H1 EM energy scale (20 < z < 100cm)
0514    m       H1 EM energy scale (100cm < z)
0515    m       H1 hadronic energy scale
0516    m       H1 muon energy scale
0521    m       Jet trigger efficiency
0522    m       Muon trigger efficiency
0531    i       Track identification efficiency
0532    m       Muon identification uncertainty

*/

