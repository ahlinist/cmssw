/*******************
A QuaeroSysEvent is an event with reconstructed final state objects
that can change under systematic shifts.
Bruce Knuteson   Feb 2004
*******************/                                                                                            
#include "Validation/VstQuaero/interface/QuaeroSysEvent.hh"
#include "Validation/VstMathUtils/interface/Math.hh"
#include "Validation/VstQuaero/interface/SystematicShift.hh"
#include "Validation/VstQuaero/interface/RCPParameters.hh"
#include <cmath>
using namespace std;

QuaeroRecoEvent<QuaeroRecoObject> QuaeroSysEvent::returnQuaeroEventWithoutSystematics() const
{
  vector<QuaeroRecoObject> o1;
  for(size_t i=0; i<components.size(); i++)
    o1.push_back(components[i]);
  QuaeroRecoEvent<QuaeroRecoObject> e(getEventType(), getRunNumber(), getWeight(), o1, 
				      getCollisionType(), getRootS(), getZVtx());
  return(e);
}

double QuaeroSysEvent::parseSystematicallyAffectedNumber(std::string blah)
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


                                                                                                                
                                                                                                                
                                                                                                                
