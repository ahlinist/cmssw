/******************************************
Implementation of QuaeroRecoObject class, defining the characteristics of reconstructed objects
Reconstructed objects are electrons, muons, taus, photons, jets, b-tagged jets, and unclustered energy
Bruce Knuteson 2003
******************************************/


#include "Validation/VstQuaeroUtils/interface/QuaeroRecoObject.hh"
#include "Validation/VstMathUtils/interface/Math.hh"
#include "Validation/VstQuaeroUtils/interface/ChatterTab.hh"
#include <cmath>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

/*****  Constructor  *****/

QuaeroRecoObject::QuaeroRecoObject(std::string _objectType, const CLHEP::HepLorentzVector & p, double _idQuality): QuaeroParticle(_objectType, p), idQuality(_idQuality)
{
}


/*****    Methods    *****/

// convert Particle Data Group code into Quaero's particle name

std::string QuaeroRecoObjectType::pdgCodeToParticleName(int particleCode)
{
  string objectType = "null";
  string sign = ( particleCode > 0 ? "+" : "-" );
  particleCode = (int)fabs((double)particleCode);
  if((particleCode<=4)||(particleCode==21))
    objectType = "j";
  if(particleCode==5)
    objectType = "b";
  if(particleCode==6)
    objectType = "t";
  if((particleCode<=6)||(particleCode==21))
    sign="";
  if(particleCode==11)
    {
      objectType = "e";
      sign = (sign=="+" ? "-" : "+");
    }
  if(particleCode==13)
    {
      objectType = "mu";
      sign = (sign=="+" ? "-" : "+");
    }
  if(particleCode==15)
    {
      objectType = "tau";
      sign = (sign=="+" ? "-" : "+");
    }
  if((particleCode==12)||
     (particleCode==14)||
     (particleCode==16))
    {
      objectType="v";
      sign="";
    }
  if(particleCode==22)
    {
      objectType = "ph";
      sign = "";
    }
  if(particleCode==23)
    {
      objectType = "Z";
      sign = "";
    }
  if(particleCode==24)
    objectType = "W";
  if(particleCode==111)
    {
      objectType = "pi";
      sign = "0";
    }
  if(particleCode==113)
    {
      objectType = "rho";
      sign = "0";
    }
  if(particleCode==211)
    objectType = "pi";
  if(particleCode==213)
    objectType = "rho";
  if(objectType=="null")
    objectType = Math::ftoa(particleCode);
  else
    objectType += sign;
  return(objectType);
}

// convert Particle Data Group particle code to Quaero particle name

string QuaeroRecoObject::particleCodeToObjectType(int particleCode)
{
  string objectType = "null";
  if(abs(particleCode)<=4)
    objectType = "j";
  if(abs(particleCode)==5)
    objectType = "b";
  if((particleCode)==-11)
    objectType = "e-";
  if((particleCode)==11)
    objectType = "e+";
  if((particleCode)==-13)
    objectType = "mu-";
  if((particleCode)==13)
    objectType = "mu+";
  if((particleCode)==-15)
    objectType = "tau-";
  if((particleCode)==15)
    objectType = "tau+";
  if((abs(particleCode)==12)||
     (abs(particleCode)==14)||
     (abs(particleCode)==16))
    objectType = "nu";
  if(particleCode==21)
    objectType = "j";
  if(particleCode==22)
    objectType = "ph";
  assert(objectType!="null");
  return(objectType);
}

// Generate a list of all possible Quaero Reco Object names

vector<string> QuaeroRecoObjectType::listOfPossibleFinalStateObjects()
{
  vector<string> ans;
  ans.push_back("e+");
  ans.push_back("e-");
  ans.push_back("mu+");
  ans.push_back("mu-");
  ans.push_back("tau+");
  ans.push_back("tau-");
  ans.push_back("ph"); // photon
  ans.push_back("j"); // jet
  ans.push_back("b"); // b-tagged jet
  ans.push_back("kill"); // kill event
  ans.push_back("jb"); // parton-level jet
  ans.push_back("jf"); // parton-level jet
  ans.push_back("q"); // parton-level quark
  ans.push_back("q+"); // parton-level quark, charge > 0
  ans.push_back("q-"); // parton-level quark, charge < 0
  ans.push_back("quds"); // parton-level u, d, or s quark
  ans.push_back("quds+"); // parton-level u, d, or s quark, charge > 0
  ans.push_back("quds-"); // parton-level u, d, or s quark, charge < 0
  ans.push_back("qc"); // parton-level c
  ans.push_back("qb"); // parton-level b
  ans.push_back("qe"); // parton-level electron
  ans.push_back("g"); // parton-level gluon
  ans.push_back("jtau+"); // parton-level jet faking a reconstructed tau
  ans.push_back("jtau-"); // parton-level jet faking a reconstructed tau
  ans.push_back("w-");
  ans.push_back("w+");
  ans.push_back("z");
  ans.push_back("weirdO_emu");
  ans.push_back("weirdO_multiMu");
  ans.push_back("uncl"); // unclustered energy
  return(ans);
}

// Return the mass of an object type

double QuaeroRecoObjectType::mass(string objecttype)
{
  if((objecttype=="e")||(objecttype=="e+")||(objecttype=="e-")||
     (objecttype=="mu")||(objecttype=="mu+")||(objecttype=="mu-")||
     (objecttype=="ph")||
     (objecttype=="weirdO_emu")||(objecttype=="weirdO_multiMu"))
    return(0.); // these objects are (nearly) massless

  if(objecttype=="kill")
    return(0.);

  if((objecttype=="j"))
    return(0.); // jet should be nearly massless

  if((objecttype=="pmiss")|| 
     (objecttype=="uncl"))
    return(0.); // mass could be anything

  if((objecttype=="tau")||(objecttype=="tau+")||(objecttype=="tau-"))
    return(1.78); // mass of the tau, in units of GeV

  if((objecttype=="b")||
     (objecttype=="jb"))
    return(4.2); // mass of the b quark, in units of GeV

  if((objecttype=="jf")||
     (objecttype=="q")||
     (objecttype=="q+")||
     (objecttype=="q-")||
     (objecttype=="quds")||
     (objecttype=="quds+")||
     (objecttype=="quds-")||
     (objecttype=="qc")||
     (objecttype=="qb")||
     (objecttype=="qe")||
     (objecttype=="g"))
    return(0); // massless

  if((objecttype=="jtau+")||
     (objecttype=="jtau-"))
    return(0); // massless

  if(objecttype=="w")
    return(80.4); // mass of the W boson, in units of GeV
  if(objecttype=="z")
    return(91.2); // mass of the Z boson, in units of GeV

  cout << "do not understand object of type " << objecttype << endl;
  assert(false);
  return(0.);
}


// return the mass of a vector of QuaeroRecoObjects

double QuaeroRecoObject::mass(vector<QuaeroRecoObject> objects)
{
  CLHEP::HepLorentzVector p = CLHEP::HepLorentzVector();  
  for(size_t i=0; i<objects.size(); i++)
    p = p + objects[i].getFourVector();
  return(p.m());
}


// if reading a number that has a systematic error associate with it, drop the systematic error.
// Such a number is in the form 52.4{<systematicError>}; we drop everything within the {}.

double QuaeroRecoObject::parseSystematicallyAffectedNumber(const string& blah)
{
  for(size_t i=0; i<blah.length(); i++)
    if(blah[i]=='{')
      return(atof(blah.substr(0,i).c_str()));
  return(atof(blah.c_str()));
}


/* If reading an object type that has a systematic error associated with it,
   keep the object type if the magnitude is < 0.5, and change the object type
   if the magnitude is > 0.5.                                                  */

pair<string,double> QuaeroRecoObject::parseSystematicallyAffectedIdentity(const string& _blah)
{

  /*  E.g., e+{0533/ph/0.292} should be e+
            e+{0533/ph/0.723} should be ph    
	      ^    ^  ^     ^  
     Marker#  1    2  3     4                 */

  if((_blah.find("{")==string::npos)&&
     (_blah.find("$")==string::npos)&&
     (_blah.find("(")==string::npos))   // if blah contains neither { nor $ nor (
    return(pair<string,double>(_blah,0));                      // then no special processing is required

  string blah = _blah;
  bool on = true;
  string ans = "";
  for(size_t i=0; i<blah.length(); i++)
    if(blah.substr(i,1)=="$")
      on = (!on);
    else
      if(on) 
	ans += blah.substr(i,1);

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

  vector<int> marker;
  for(size_t i=0; i<blah.length(); i++)
    {
      if( ( (marker.size()==3) && (blah.substr(i,1)==",") ) ||
	  ( (marker.size()==3) && (blah.substr(i,1)=="}") ) ||
	  ( (marker.size()==2) && (blah.substr(i,1)=="/") ) ||
	  ( (marker.size()==1) && (blah.substr(i,1)=="/") ) ||
	  ( (marker.size()==0) && (blah.substr(i,1)=="{") ) )
	marker.push_back(i);
    }
  if(marker.empty())
    return(pair<string,double>(ans,_idQuality));
  if(marker.size()<4)
    assert(false);
  double r = atof(blah.substr(marker[2]+1,marker[3]-marker[2]-1).c_str());
  assert((r>=0)&&(r<=1));
  if(r<0.5)
    ans = blah.substr(0,marker[0]);
  else
    ans = blah.substr(marker[1]+1,marker[2]-marker[1]-1);
  
  return(pair<string,double>(ans,_idQuality));
}

// Read a QuaeroRecoObject in from an input stream.
// format is a string that determines the input format 

bool QuaeroRecoObject::read(istream& fin, const string& format)
{
  string _objectType, objectType;
  double idQuality=0;
  if(!(fin >> _objectType))
    return(false);
  if((_objectType==";")||(!fin))
    return(false);
  else
    {
      objectType = _objectType = (parseSystematicallyAffectedIdentity(_objectType)).first;
      bool objectTypeUnderstood =
	(
	 // Standard objects
	 (objectType=="j")||
	 (objectType=="jb")||
	 (objectType=="jf")||
	 (objectType=="q")||
	 (objectType=="q+")||
	 (objectType=="q-")||
	 (objectType=="quds")||
	 (objectType=="quds+")||
	 (objectType=="quds-")||
	 (objectType=="qc")||
	 (objectType=="qb")||
	 (objectType=="qe")||
	 (objectType=="g")||
	 (objectType=="jtau+")||
	 (objectType=="jtau-")||
	 (objectType=="uncl")||
	 (objectType=="ph")||
	 (objectType=="b")||
	 (objectType=="e+")||
	 (objectType=="e-")||
	 (objectType=="mu+")||
	 (objectType=="mu-")||
	 (objectType=="tau+")||
	 (objectType=="tau-")||
	 (objectType=="weirdO_emu")||
	 (objectType=="weirdO_multiMu")||
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

      // Convert jb object to b object
      if(objectType=="jb")
	objectType = "j";

      // Convert jtau object to tau object
      if(objectType=="jtau+")
	objectType = "j";
      if(objectType=="jtau-")
	objectType = "j";
      // Convert qe object to quds object
      if(objectType=="qe")
	objectType = "quds";

      if(objectType=="q")
	objectType = "quds";
      if(objectType=="q+")
	objectType = "quds+";
      if(objectType=="q-")
	objectType = "quds-";

      string s="";
      double m=0, e=0, px, py, pz;
      if((_objectType=="j")||(_objectType=="b")||(_objectType=="jb")||(_objectType=="uncl")
	 ||(_objectType=="w+")||(_objectType=="w-")||(_objectType=="z"))
	{
	  fin >> s; m = parseSystematicallyAffectedNumber(s);
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
	  double costheta, phi;
	  fin >> s; e = parseSystematicallyAffectedNumber(s);
	  fin >> s; costheta = parseSystematicallyAffectedNumber(s);
	  fin >> s; phi = parseSystematicallyAffectedNumber(s);
	  double p = e; // sqrt(e*e-m*m); 
	  pz = p*costheta;
	  double pt = sqrt(p*p-pz*pz);
	  px = pt * cos(phi);
	  py = pt * sin(phi);
	  formatUnderstood = true;
	}
      if(format=="(m)-pt-eta-phi(deg)")
	{
	  double eta = 0, theta = M_PI/2;
	  double pt, phi;
	  fin >> s; pt = parseSystematicallyAffectedNumber(s);
	  if(pt>0.001)
	    {
	      fin >> s; eta = parseSystematicallyAffectedNumber(s);
	      theta = Math::eta2theta(eta);
	    }
	  else 
	    fin >> s;
	  // if(objectType=="uncl") { m=0; eta=0; }
	  fin >> s; phi=  parseSystematicallyAffectedNumber(s);
	  phi *= M_PI/180.;
	  if(pt<0.001) pt = 0.001;
	  double p = pt/sin(theta);
	  e = sqrt(p*p+m*m);
	  px = pt*cos(phi);
	  py = pt*sin(phi);
	  pz = p*cos(theta);
	  formatUnderstood = true;
	}
      if(format=="p-px-py-pz")
	{
	  fin >> e;
	  fin >> px;
	  fin >> py;
	  fin >> pz;
	  formatUnderstood = true;
	}
      assert(formatUnderstood);

      if((_objectType=="nu_e")||(_objectType=="nu_mu")||(_objectType=="nu_tau"))
	{
	  objectType="uncl";
	  e = px = py = pz = 0;
	}
      if((objectType=="weirdO_emu")||(objectType=="weirdO_multiMu"))
	objectType="kill";

      *this = QuaeroRecoObject(objectType, CLHEP::HepLorentzVector(e, CLHEP::Hep3Vector(px, py, pz)), idQuality);
    }
  return(true);
} 

// return the QuaeroRecoObject as a string suitable for printing.
// Use format "format", with numbers rounded to "tol".

string QuaeroRecoObject::print(string format, double tol) const
{
  string ans = "";
  ans = ans + componentType;
  if(idQuality!=0)
    ans += "("+Math::ftoa(idQuality)+")";
  ans += " ";
  bool formatUnderstood = false;
  if(format=="(m)-e-costheta-phi(rad)")
    {
      if((componentType=="j")||(componentType=="b")||(componentType=="jb")||(componentType=="uncl"))
	ans += Math::ftoa(Math::toleranceRound(fourVector.m(),tol))+" ";
      ans += 
	Math::ftoa(Math::toleranceRound(fourVector.e(),tol))+" "+
	Math::ftoa(Math::toleranceRound(fourVector.cosTheta(),tol))+" "+
	Math::ftoa(Math::toleranceRound(fourVector.phi(),tol))+"  "; 
      formatUnderstood = true;
    }
  if(format=="(m)-pt-eta-phi(deg)")
    {
      if((componentType=="j")||(componentType=="b")||(componentType=="jb")||(componentType=="uncl")||
	 (componentType=="w+")||(componentType=="w-")||(componentType=="z"))
	ans += Math::ftoa(Math::toleranceRound(fourVector.m(),tol))+" ";
      double phi_deg = fmod((fourVector.phi()*180./M_PI) ,360.) ;      
      ans += 
	Math::ftoa(Math::toleranceRound(fourVector.perp(),tol))+" "+
	(fourVector.perp()==0 ? "0" : Math::ftoa(Math::toleranceRound(Math::theta2eta(fourVector.theta()),tol)))+" "+
	Math::ftoa(Math::toleranceRound(phi_deg,tol*10))+"  "; 
      formatUnderstood = true;
    }
  assert(formatUnderstood);
  return(ans);
}

// This defines when two QuaeroRecoObjects are equivalent

bool QuaeroRecoObject::operator==(const QuaeroRecoObject & rhs) const
{
  return((QuaeroParticle)(*this)==(QuaeroParticle)rhs);
}

// This routine takes two HepLorentzVectors, the second of which is a neutrino (pz not specified).
// The pz of o2 is calculated by forcing mass(o1+o2) to be as close to the input "Mass" as possible.

void QuaeroRecoObject::ChiSqdConstrainNeutrino(const CLHEP::HepLorentzVector & o1, CLHEP::HepLorentzVector & o2, double Mass)
{
  // o2 is pmiss; o1 is the sum of the other objects
  double obj1e = o1.e();
  double obj2pt = o2.perp();
  double obj12pt = (o1+o2).perp();
  double obj1pz = o1.pz();

  // The following solutions for the pz of the neutrino are taken from Mathematica
  double obj2pz_1 =  (pow(Mass,2)*obj1pz + 
		      pow(obj12pt,2)*obj1pz - pow(obj1e,2)*obj1pz + 
		      pow(obj1pz,3) - obj1pz*pow(obj2pt,2) - 
		      obj1e*sqrt(pow(Mass,4) + 
				 2*pow(Mass,2)*pow(obj12pt,2) + 
				 pow(obj12pt,4) - 
				 2*pow(Mass,2)*pow(obj1e,2) - 
				 2*pow(obj12pt,2)*pow(obj1e,2) + 
				 pow(obj1e,4) + 
				 2*pow(Mass,2)*pow(obj1pz,2) + 
				 2*pow(obj12pt,2)*pow(obj1pz,2) - 
				 2*pow(obj1e,2)*pow(obj1pz,2) + 
				 pow(obj1pz,4) - 
				 2*pow(Mass,2)*pow(obj2pt,2) - 
				 2*pow(obj12pt,2)*pow(obj2pt,2) - 
				 2*pow(obj1e,2)*pow(obj2pt,2) + 
				 2*pow(obj1pz,2)*pow(obj2pt,2) + 
				 pow(obj2pt,4)))/
    (2.*(pow(obj1e,2) - pow(obj1pz,2)));
  double obj2pz_2 =  (pow(Mass,2)*obj1pz + 
		      pow(obj12pt,2)*obj1pz - pow(obj1e,2)*obj1pz + 
		      pow(obj1pz,3) - obj1pz*pow(obj2pt,2) + 
		      obj1e*sqrt(pow(Mass,4) + 
				 2*pow(Mass,2)*pow(obj12pt,2) + 
				 pow(obj12pt,4) - 
				 2*pow(Mass,2)*pow(obj1e,2) - 
				 2*pow(obj12pt,2)*pow(obj1e,2) + 
				 pow(obj1e,4) + 
				 2*pow(Mass,2)*pow(obj1pz,2) + 
				 2*pow(obj12pt,2)*pow(obj1pz,2) - 
				 2*pow(obj1e,2)*pow(obj1pz,2) + 
				 pow(obj1pz,4) - 
				 2*pow(Mass,2)*pow(obj2pt,2) - 
				 2*pow(obj12pt,2)*pow(obj2pt,2) - 
				 2*pow(obj1e,2)*pow(obj2pt,2) + 
				 2*pow(obj1pz,2)*pow(obj2pt,2) + 
				 pow(obj2pt,4)))/
    (2.*(pow(obj1e,2) - pow(obj1pz,2)));
  double obj2pz;
  if(abs(obj2pz_1)<abs(obj2pz_2))
    obj2pz = obj2pz_1;
  else
    obj2pz = obj2pz_2;
  bool foundAnImaginarySolution=false;
  if(Math::isNaNQ(obj2pz))
    foundAnImaginarySolution=true;
  if(foundAnImaginarySolution)
    if(o1.perp()>0.)
      obj2pz = o1.pz()*(o2.perp()/o1.perp());
    else
      obj2pz = 0.;
  double massbeforefitting = (o1+o2).m();
  CLHEP::Hep3Vector pmiss3v = CLHEP::Hep3Vector(o2.px(), o2.py(), obj2pz);
  o2 = CLHEP::HepLorentzVector(pmiss3v.mag(),pmiss3v);
  double massafterfitting = (o1+o2).m();
  assert(
	 Math::MPEquality(massafterfitting,Mass,0.01)||
	 (foundAnImaginarySolution&&(massbeforefitting>Mass))||
	 (Math::MPEquality(o1.e(),0.)&&Math::MPEquality(massafterfitting,0.,1.e-2))
	 );
  return;
}

/* return detector eta
   Inputs:
     name = name of experiment
     objectType = "e", "mu", etc.
     eta = event eta, measured from the position of the primary vertex
     zv = position of the primary vertex in the event */

double QuaeroRecoObject::getDetectorEta(string name, string objectType, double eta, double zv)
{

   if((name=="l3")||(name=="aleph")||(name=="all")||(name=="cms"))
    return(eta); // collision occurs at zvtx = 0 to very good approximation

  assert((name=="cdf")||
	 (name=="d0"));

  if((objectType=="uncl"))
    return(0.);

  if((objectType=="mu+")||
     (objectType=="mu-")||
     (objectType=="mu")||
     (objectType=="weirdO_multiMu")||
     (objectType=="weirdO_emu"))
    return(eta); // for muons, detEta = eta

  double rcc=0, zcc_max=0, zec=0;
  if(name=="cdf")
    {
      // these are CDF numbers
      if((objectType=="e")||(objectType=="e+")||(objectType=="e-")||
	 (objectType=="ph")||(objectType=="kill"))	{
	rcc=184.15; // radius of central shower max
	zcc_max=239.6; // how far in positive z does CES reach?
	zec=185.4; // z of plug shower max
	double rpes=138; //radius of PES

	double thetaDetector=0;
	double detEta=-666;
	
	double theta = 2.*atan(exp(-eta));
	//does it hit the PES?
	double h_atZpes;
	if (cos(theta)>0)//hits right
	  h_atZpes=tan(theta)*(zec-zv);
	else
	  h_atZpes=tan(theta)*(-zec-zv);
	assert(h_atZpes >= 0);
	
	if ( h_atZpes > rpes ) {
	  //hits ceiling
	  double zWhenHittingCeiling = zv + rcc/tan(theta);
	  thetaDetector=atan(rcc/zWhenHittingCeiling);
	}
	else {
	  //hits plug
	  thetaDetector = atan(h_atZpes/zec); //positive
	  if ( cos(theta) < 0 ) //it has to hit left
	    thetaDetector = M_PI - thetaDetector;
	}
	if (thetaDetector < 0) thetaDetector=M_PI + thetaDetector;
	detEta=-log(tan(thetaDetector/2));

	return(detEta);
      }
      else if((objectType=="tau")||(objectType=="tau+")||(objectType=="tau-")||
	      (objectType=="jtau+")||(objectType=="jtau-")||
	      (objectType=="j")||(objectType=="jf")||
	      (objectType=="q")||(objectType=="q+")||(objectType=="q-")||
	      (objectType=="quds")||(objectType=="quds+")||(objectType=="quds-")||
	      (objectType=="qc")||(objectType=="qb")||(objectType=="qe")||
	      (objectType=="g")||(objectType=="jb")||(objectType=="b"))
	{
	  rcc=185; // radius of central shower center of gravity (guess)
	  zcc_max=rcc; // eta = 1.0 AT r=185 cm 
	  zec=185; // z of plug shower center of gravity (guess)
	}
      else
	{
	  cout << "Error!  Object type " << objectType << " not understood." << endl;
	  assert(false);
	}
    }
  else if(name=="d0")
    {
      // these are D0 numbers
       rcc=91.6; // EM3
       zcc_max=135.85; // ETA = 1.2 AT R=90 CM 
       zec=178.9; // EM3
    }

  double theta = 2.*atan(exp(-eta));
  double z = zv + rcc/tan(theta);
  if(fabs(z)<zcc_max) // in CC
    z = z/rcc;
  else // in EC
    {
      if(cos(theta)>0)
        z = 1./(1. - zv/zec);
      else
        z = 1./(1. + zv/zec);
      z = z/tan(theta);
    }
  double ans = log(z+sqrt(pow(z,2.)+1.)); // detector eta
  return(ans);
}

// return event eta
// Inputs similar to those for getDetectorEta above, except that detectorEta (deta) is input,
// and event eta is returned.

double QuaeroRecoObject::getEventEta(string name, string objectType, double deta, double zv)
{

  if(((name=="cdf")||(name=="d0"))&&
     (objectType=="uncl"))
    return(0.);

  /* This is a very inefficient way to invert the function getDetectorEta. */

  double etaLo=-10.;
  double etaHi=10.;
  double eta=(etaHi+etaLo)/2.;
  double deltaEta=fabs(etaHi-etaLo);
  double tol=.0001;

  // using the fact that getDetectorEta(eta) is monotonic in eta, we perform a binary search for the root of the equation getDetectorEta(eta) == deta 

  while(deltaEta>tol)
    {
      double x=getDetectorEta(name, objectType, eta, zv);
      if(x<deta)
	etaLo=eta;
      else
	etaHi=eta;
      eta=(etaHi+etaLo)/2.;
      deltaEta=fabs(etaHi-etaLo);
    }
  return(eta);

}

// Set a HepLorentzVector by setting its mass, pt, eta, and phi

CLHEP::HepLorentzVector QuaeroRecoObject::setLorentzVectorMPtEtaPhi(double m, double pt, double eta, double phi)
{
  if(fabs(eta)>10)
    return(CLHEP::HepLorentzVector(CLHEP::Hep3Vector(pt*cos(phi),pt*sin(phi),0),sqrt(pt*pt+m*m)));
  double theta = Math::eta2theta(eta);
  double p = pt/sin(theta);
  double e = sqrt(p*p+m*m);
  double px = pt*cos(phi);
  double py = pt*sin(phi);
  double pz = p*cos(theta);
  return(CLHEP::HepLorentzVector(CLHEP::Hep3Vector(px,py,pz),e));
}

