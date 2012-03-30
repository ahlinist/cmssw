

#include "Validation/VstMathUtils/interface/Math.hh"
#include "Validation/VstQuaero/interface/QuaeroSysObject.hh"
#include "Validation/VstQuaero/interface/SystematicShift.hh"
#include "Validation/VstQuaero/interface/RCPParameters.hh"
#include <cmath>
using namespace std;


double QuaeroSysObject::parseSystematicallyAffectedNumber(const string& blah)
{
  double nominalValue = 0.;
  int marker = 0;
  vector<int> errorNumber;
  vector<double> errorMagnitude;
  string readType = "nominal";
  for(size_t i=0; i<blah.length(); i++)
    {
      if(blah.substr(i,1)=="{")
	{
	  if(readType!="nominal")
	    {
	      cout << (string)"Error in reading systematic error (a) in value '"+blah+"'." << endl;
	      exit(1);
	    }
	  nominalValue = atof(blah.substr(0,i).c_str());
	  if(blah.substr(0,i)=="nan")
	    {
	      cout << (string)"Error in reading systematic error (a) in value '"+blah+"'." << endl;
	      exit(1);
	    }
	  marker = i+1;
	  readType="errorNumber";
	}
      if(blah.substr(i,1)=="/")
	{
	  if(readType!="errorNumber")
	    {
	      cout << (string)"Error in reading systematic error (b) in value '"+blah+"'." << endl;
	      exit(1);
	    }
	  errorNumber.push_back(atoi(blah.substr(marker,i-marker).c_str()));
	  marker = i+1;
	  readType="errorMagnitude";
	}
      if(blah.substr(i,1)==",")
	{
	  if(readType!="errorMagnitude")
	    {
	      cout << (string)"Error in reading systematic error (c) in value '"+blah+"'." << endl;
	      exit(1);
	    }
	  errorMagnitude.push_back(atof(blah.substr(marker,i-marker).c_str()));
	  marker = i+1;
	  readType="errorNumber";
	}
      if(blah.substr(i,1)=="}")
	{
	  if(readType!="errorMagnitude")
	    {
	      cout << (string)"Error in reading systematic error (d) in value '"+blah+"'." << endl;
	      exit(1);
	    }
	  errorMagnitude.push_back(atof(blah.substr(marker,i-marker).c_str()));
	  if(i!=blah.length()-1)
	    {
	      cout << (string)"Error in reading systematic error (e) in value '"+blah+"'." << endl;
	      exit(1);
	    }
	}
      if((i+1==blah.length())&&
	 (readType=="nominal"))
	return(atof(blah.c_str()));
    }
  assert(errorNumber.size()==errorMagnitude.size());
  double ans = GlobalVariables::systematicShift.modifyValue(nominalValue,errorNumber,errorMagnitude);
  return(ans);
}


pair<string,double> QuaeroSysObject::parseSystematicallyAffectedIdentity(const string& _blah)
{
  string nominalValue = "";
  int marker = 0;
  vector<int> errorNumber;
  vector<string> newID;
  vector<double> chance;
  string readType = "nominal";

  string blah = _blah;
  double _idQuality = 0;
  for(size_t i=0; i<blah.length(); i++)
    if(blah.substr(i,1)=="(")
      {
	for(size_t j=i+1; j<blah.length(); j++)
	  if(blah.substr(j,1)==")")
	    {
	      _idQuality = atof(blah.substr(i+1,j-i-1).c_str());
	      blah = blah.substr(0,i) + ( j == blah.length()-1 ? "" : blah.substr(j+1));
	      break;
	    }
      }

  for(size_t i=0; i<blah.length(); i++)
    {
      if(blah.substr(i,1)=="{")
	{
	  if(readType!="nominal")
	    {
	      cout << (string)"Error in reading systematic error (f) in value '"+blah+"'." << endl;
	      exit(1);
	    }
	  nominalValue = blah.substr(0,i);
	  marker = i+1;
	  readType="errorNumber";
	}
      if(blah.substr(i,1)=="/")
	{
	  if((readType!="errorNumber")&&(readType!="newID"))
	    {
	      cout << (string)"Error in reading systematic error (g) in value '"+blah+"' at position " << i << "." << endl;
	      exit(1);
	    }
	  if(readType=="errorNumber")
	    {
	      errorNumber.push_back(atoi(blah.substr(marker,i-marker).c_str()));
	      marker = i+1;
	      readType="newID";
	    }
	  else if(readType=="newID")
	    {
	      newID.push_back(blah.substr(marker,i-marker));
	      marker = i+1;
	      readType="chance";
	    }
	}
      if(blah.substr(i,1)==",")
	{
	  if(readType!="chance")
	    {
	      cout << (string)"Error in reading systematic error (h) in value '"+blah+"'." << endl;
	      exit(1);
	    }
	  chance.push_back(atof(blah.substr(marker,i-marker).c_str()));
	  marker = i+1;
	  readType="errorNumber";
	}
      if(blah.substr(i,1)=="}")
	{
	  if(readType!="chance")
	    {
	      cout << (string)"Error in reading systematic error (i) in value '"+blah+"'." << endl;
	      exit(1);
	    }
	  chance.push_back(atof(blah.substr(marker,i-marker).c_str()));
	  if(i!=blah.length()-1)
	    {
	      cout << (string)"Error in reading systematic error (j) in value '"+blah+"'." << endl;
	      exit(1);
	    }
	}
      if((i+1==blah.length())&&
	 (readType=="nominal"))
	return(pair<string,double>(blah,_idQuality));
    }
  assert(errorNumber.size()==newID.size());
  assert(errorNumber.size()==chance.size());
  string ans = GlobalVariables::systematicShift.modifyValue(nominalValue,errorNumber,newID,chance);
  return(pair<string,double>(ans,_idQuality));
}



bool QuaeroSysObject::read(istream& fin, const string& format)
{
  string _objectType, objectType;
  double idQuality=0;
  if(!(fin >> _objectType))
    return(false);
  if((_objectType==";")||(!fin))
    return(false);
  else
    {
      pair<string,double> objectTypeAndIdQuality = parseSystematicallyAffectedIdentity(_objectType);
      objectType = objectTypeAndIdQuality.first;
      idQuality = objectTypeAndIdQuality.second;
      bool objectTypeUnderstood =
	(
	 // Standard objects
	 (objectType=="j")||
	 (objectType=="jf")||
	 (objectType=="uncl")||
	 (objectType=="ph")||
	 (objectType=="b")||
	 (objectType=="e+")||
	 (objectType=="e-")||
	 (objectType=="mu+")||
	 (objectType=="mu-")||
	 (objectType=="tau+")||
	 (objectType=="tau-")||
	 // Special objects
	 (objectType=="nu_e")||
	 (objectType=="nu_mu")||
	 (objectType=="nu_tau")||
	 (objectType=="kill")||
	 (objectType=="w+")||
	 (objectType=="w-")||
	 (objectType=="z")||
	 // RHIC objects
	 (objectType=="p+")||
	 (objectType=="p-")||
	 (objectType=="pi+")||
	 (objectType=="pi-")||
	 (objectType=="K+")||
	 (objectType=="K-")
	 );
      if(!objectTypeUnderstood)
	{
	  cerr << "Object type not understood:  " << objectType << endl;
	  assert(false);
	}
      string s="";
      double m=0, e=0, px, py, pz;
      if((_objectType=="j")||(_objectType=="b")||(_objectType=="uncl"))
	// ||(_objectType=="w+")||(_objectType=="w-")||(_objectType=="z"))
	{
	  fin >> s;  
	  m = parseSystematicallyAffectedNumber(s);
	}
      else
	{
	  if((_objectType=="tau+")||(_objectType=="tau-"))
	    m = 1.78;
	  else
	    m = 0.;
	}
      bool formatUnderstood = false;
      if(format=="(m)-e-costheta-phi(rad)")
	{
	  fin >> s;  e = parseSystematicallyAffectedNumber(s);
	  fin >> s;  double costheta = parseSystematicallyAffectedNumber(s);
	  fin >> s;  double phi = parseSystematicallyAffectedNumber(s);
	  double p = e; // sqrt(e*e-m*m); 
	  pz = p*costheta;
	  double pt = sqrt(p*p-pz*pz);
	  px = pt * cos(phi);
	  py = pt * sin(phi);
	  formatUnderstood = true;
	}
      if(format=="(m)-pt-eta-phi(deg)")
	{
	  if(objectType=="uncl")
	    m=0;
	  fin >> s;  double pt = parseSystematicallyAffectedNumber(s);
	  fin >> s;  double eta = 0; 
	  if((pt>0.001)&&(objectType!="uncl"))
	    eta = parseSystematicallyAffectedNumber(s);
	  fin >> s;  double phi = parseSystematicallyAffectedNumber(s)*M_PI/180.;
	  if(pt<0.001) pt = 0.001;
	  double theta = Math::eta2theta(eta);
	  double p = pt/sin(theta);
	  e = sqrt(p*p+m*m);
	  px = pt*cos(phi);
	  py = pt*sin(phi);
	  pz = p*cos(theta);
	  formatUnderstood = true;
	}
      if(format=="p-px-py-pz")
	{
	  fin >> s;   e = parseSystematicallyAffectedNumber(s);
	  fin >> px;  px = parseSystematicallyAffectedNumber(s);
	  fin >> py;  py = parseSystematicallyAffectedNumber(s);
	  fin >> pz;  pz = parseSystematicallyAffectedNumber(s);
	  formatUnderstood = true;
	}
      assert(formatUnderstood);

      if((_objectType=="nu_e")||(_objectType=="nu_mu")||(_objectType=="nu_tau"))
	{
	  objectType=="uncl";
	  e = px = py = pz = 0;
	}
      *this = QuaeroSysObject(objectType, CLHEP::HepLorentzVector(e, CLHEP::Hep3Vector(px, py, pz)), idQuality);
    }
  return(true);
} 
