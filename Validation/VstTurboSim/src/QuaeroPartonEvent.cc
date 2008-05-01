/*******************************************
Implementation of QuaeroPartonEvent
Bruce Knuteson Jan 2004
*******************************************/


#include "Validation/VstTurboSim/interface/QuaeroPartonEvent.hh"
using namespace std;


/*****  Constructor  *****/

QuaeroPartonEvent::QuaeroPartonEvent(std::string _eventType, 
		    std::string _runNumber, 
		    double _wt, 
		    const std::vector<QuaeroPartonObject>& _objects, 
				     string _collisionType,
		    double _rootS, 
		    double _zVtx, 
		    double _generatedSumPt,
		    std::string _fullString):
    QuaeroRecoEvent<QuaeroPartonObject>(_eventType, _runNumber, _wt, _objects, _collisionType, _rootS, _zVtx, _generatedSumPt, _fullString, true) 
  {
     smallestObjectPtHadronMachine = 5; // units are GeV
    smallestObjectEnergyLeptonMachine = 1; // units are GeV
  }


/*****  Stream operators  *****/

istream &operator>>(istream& fin, QuaeroPartonEvent & event)
{
  string eventType = "";
  if(!(fin >> eventType))
    return(fin);
  string runNumber = "0";
  fin >> runNumber;
  string wtString;
  fin >> wtString;
  double wt = event.parseSystematicallyAffectedNumber(wtString);
  double rootS, zVtx=0, generatedSumPt=0;
  string collisionType;
  fin >> collisionType;
  fin >> rootS;
  if((collisionType=="ppbar")||
     (collisionType=="pp")||
     (collisionType=="e+p")||
     (collisionType=="e-p"))	
    fin >> generatedSumPt;
  if((collisionType!="e+e-")) // hadron collider, large bunch length
    fin >> zVtx;

  QuaeroPartonObject object;
  vector<QuaeroPartonObject> objects;
  if(rootS>250)
    while(object.read(fin,"(m)-pt-eta-phi(deg)"))
      objects.push_back(object);
  else
    while(object.read(fin,"(m)-e-costheta-phi(rad)"))
      objects.push_back(object);

  event = QuaeroPartonEvent(eventType,runNumber,wt,objects,collisionType,rootS,zVtx,generatedSumPt);
  return(fin);
}


ostream &operator<<(ostream& fout, const QuaeroPartonEvent & event)
{
  fout << event.print();
  return(fout);
}


