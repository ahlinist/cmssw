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

/* Convert an error in theta (in radians) to an error in eta */
double deltaTheta2deltaEta(double thetaRad, double deltaThetaRad)
{
  double deltaEta = fabs(Math::theta2eta(thetaRad+deltaThetaRad)-Math::theta2eta(thetaRad-deltaThetaRad))/2;
  return(deltaEta);
}


/* Angular systematic error on the polar angle of EM clusters */
double etaSystematicErrorOnEMClusters(double theta)
{
  double polar = theta*180/M_PI;
  assert(polar>=0);
  assert(polar<=180);
  double deltaThetaRad = 0;
  if(polar < 120.)
    deltaThetaRad = 0.003;
  else if(polar < 135.)
    deltaThetaRad = 0.002;
  else
    deltaThetaRad = 0.001;
  double deltaEta = deltaTheta2deltaEta(theta,deltaThetaRad);
  return(deltaEta);
}


int main(int argc, char* argv[])
{
  if((argc!=4)||(((string)argv[2])!="-parton"))
    {
      cout << "Usage:  assignSystematicUncertainties_h1 <recoFile> -parton <partonFile>" << endl;
      exit(1);
    }
  assert(argc==4);

  string recoFilename = argv[1];
  string partonFilename = argv[3];
  QuaeroItemFile<QuaeroEvent> efReco(recoFilename);
  QuaeroItemFile<QuaeroEvent> efParton(partonFilename);
  string tmpFilename = "/tmp/tmp_assignSystematicUncertainties_h1_"+Math::ftoa(getpid());
  ofstream fout(tmpFilename.c_str());
  QuaeroEvent e, eParton;
  vector<string> errors;
  efParton.nextEvent(eParton);
  while(efReco.nextEvent(e))
    {
      if(e.getEventType()=="data")
	{
	  fout << e << endl;
	  continue;
	}
      while((eParton.getEventType()!=e.getEventType())||
	    (eParton.getRunNumber()!=e.getRunNumber()))
	if(!efParton.nextEvent(eParton))
	  break;
      assert((eParton.getEventType()==e.getEventType())&&
	     (eParton.getRunNumber()==e.getRunNumber()));
      
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
	 0531    m       Track identification efficiency
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
      
      double q = 0;
      for(int i=0; i<e.numberOfObjects("e+"); i++)
	q += trackIdentificationEfficiencyUncertainty(e.getThisObject("e+",i+1)->getFourVector().theta());
      for(int i=0; i<e.numberOfObjects("e-"); i++)
	q += trackIdentificationEfficiencyUncertainty(e.getThisObject("e-",i+1)->getFourVector().theta());
      for(int i=0; i<e.numberOfObjects("ph"); i++)
	q += trackIdentificationEfficiencyUncertainty(e.getThisObject("ph",i+1)->getFourVector().theta());
      if(q>0)
	errors.push_back("0531/"+Math::ftoa(Math::sigFigRound(e.getWeight()*q,3)));
      
      if(e.numberOfObjects("mu")>=1)
	errors.push_back("0532/"+Math::ftoa(Math::sigFigRound(e.getWeight()*0.05,3)));
      if(e.getEventType()=="sig") // simulated using TurboSim@H1
	errors.push_back("0581/"+Math::ftoa(e.getWeight()*0.10)); // 10% weight uncertainty	  
      
      fout << printError(errors) << " ";
      
      // sqrt(s)
      fout << e.getCollisionType() << "  ";
      fout << e.getRootS() << "  ";
      
      // generatedSumPt
      fout << e.getGeneratedSumPt() << "  ";

      // z vertex
      fout << e.getZVtx() << "  ";
      
      // objects
      vector<QuaeroRecoObject> o = e.getObjects();
      for(int i=0; i<o.size(); i++)
	{
	  
	  // object type
	  string objectType = o[i].getObjectType();
	  errors = vector<string>(0);

	  /* The misId rate for a parton-level e+ to be identified as a reco-level ph is 0.001.
	     The uncertainty on this rate is +- 50%.
	     In order to handle this, 50% of reconstructed-level photons that come from a parton-level e+ 
	     via TurboSim are here called e+ at reco-level.
	     Then systematic source 0533, a random number pulled from the unit interval, allows
	     p(e+->"ph") to range between 0.0005 and 0.0015.  */

	  // One out of every 1000 electrons may be called a photon
	  if((objectType=="e+")&&(drand48()<0.001))
	    errors.push_back("0533/ph/"+Math::ftoa(Math::toleranceRound(drand48(),0.001)));

	  // Half of all reconstructed-level photons arising from parton-level electrons 
	  // are called electrons.
	  if((objectType=="ph"))
	    {
	      string whatThisPhotonComesFrom = "";
	      double minDistance=99,distance;
	      vector<QuaeroRecoObject> oParton = eParton.getObjects();
	      for(int j=0; j<oParton.size(); j++)
		{
		  distance = Math::deltaR(o[i].getFourVector().phi(), Math::theta2eta(o[i].getFourVector().theta()), 
					  oParton[j].getFourVector().phi(), Math::theta2eta(oParton[j].getFourVector().theta()));
		  if(distance<minDistance)
		    {
		      minDistance=distance;
		      whatThisPhotonComesFrom = oParton[j].getObjectType();
		    }
		}
	      if((whatThisPhotonComesFrom=="e+")&&(drand48()<0.50))
		objectType="e+";
	    }
	  fout << objectType << printError(errors) << " ";
	  
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
	      errors.push_back("0001/"+Math::ftoa(Math::toleranceRound(deltaTheta2deltaEta(o[i].getFourVector().theta(),0.005),0.001)));
	    else
	      errors.push_back("0001/"+Math::ftoa(Math::toleranceRound(deltaTheta2deltaEta(o[i].getFourVector().theta(),0.010),0.001)));

	  /* The angular uncertainty of collimated electromagnetic clusters varies 
	     theta dependent between 1 and 3 mrad */

	  if((o[i].getObjectTypeSansSign()=="e")||
	     (o[i].getObjectType()=="ph"))
	    errors.push_back("0001/"+Math::ftoa(Math::toleranceRound(etaSystematicErrorOnEMClusters(o[i].getFourVector().theta()),0.001)));
	  /* The uncertainty on the muon polar angle determination is 3 mrad */

	  if((o[i].getObjectTypeSansSign()=="mu"))
	    errors.push_back("0001/"+Math::ftoa(Math::toleranceRound(deltaTheta2deltaEta(o[i].getFourVector().theta(),0.003),0.001)));
	  fout << printError(errors) << " ";
	  
	  // Azimuthal angle (phi(rad), phi(deg))
	  double azimuthal = fmod((o[i].getFourVector().phi()*180./M_PI) ,360.);
	  fout << Math::ftoa(Math::toleranceRound(azimuthal,0.01));
	  errors = vector<string>(0);
	  errors.push_back("0001/"+Math::ftoa(Math::toleranceRound(0.001*180/M_PI,0.001)));
	  fout << printError(errors) << " ";
	  fout << " ";
	}
      fout << ";" << endl;
    }
  fout.close();
  system(("mv "+tmpFilename+" "+recoFilename).c_str());
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
0531    m       Track identification efficiency
0532    m       Muon identification uncertainty
0533    i       H1 electrons faking photons

*/

