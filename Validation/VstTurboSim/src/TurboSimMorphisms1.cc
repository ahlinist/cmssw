#include <vector>
#include <string>
#include <cfloat>
#include <numeric>
#include <getopt.h>
#include "Validation/VstQuaeroUtils/interface/QuaeroEvent.hh"
#include "Validation/VstQuaeroUtils/interface/QuaeroItemFile.hh"
#include "Validation/VstTurboSim/interface/TurboSimMorphisms.hh"
#include "Validation/VstTurboSim/interface/TurboSimMorphisms1.hh"
#include "Validation/VstTurboSim/interface/QuaeroPartonEvent.hh"
#include "CLHEP/Vector/ThreeVector.h"

using namespace std;
 

double TurboSimMorphisms1::deltaAngle(const CLHEP::HepLorentzVector& v1, 
				      const CLHEP::HepLorentzVector& v2,
				      AngleMeasure angleMeasureMethod) {
  
 
  if (angleMeasureMethod == DEFAULT)
    angleMeasureMethod = angleMeasureFat;
  
  double theta1 = v1.theta();
  double phi1 = v1.phi();
  double theta2 = v2.theta();
  double phi2 = v2.phi();

  double ans, angle, dotProduct;

  switch ( angleMeasureMethod ) {
    
    // deltaR
    case DELTA_R:
      return Math::deltaR( phi1, Math::theta2eta( theta1 ), phi2, Math::theta2eta( theta2 ) );
      break;

    // deltaAngle
    case DELTA_ANGLE: 
      dotProduct = 
	sin( theta1 ) * cos( phi1 ) * sin( theta2 ) * cos( phi2 ) +
	sin( theta1 ) * sin( phi1 ) * sin( theta2 ) * sin( phi2 ) +
	cos( theta1 ) * cos( theta2 );
      dotProduct = min( 0.999, dotProduct ); // handle 1 + numerical error
      angle = fabs( acos( dotProduct ) );
      return angle;
      break;

    // manhattanDistance
    case MANHATTAN_DISTANCE:
      return Math::addInQuadrature( Math::deltaphi( phi1, phi2 ), theta1 - theta2 );
      break;

    // Energy difference
    case KT_DURHAM:
      ans = pow ( ( v1 + v2 ).m() / 200, 2. );
      if ( ( v1.e() < 0.1 ) || ( v2.e() < 0.1 ) )
	ans = 99;
      return ans;
      break;
    
    default:
      assert(false);
      return 0;
  }
}

struct SimpleObject {
  short objectType;
  float e, px, py, pz;
};

short stringObjectType2Short( string objectType )
{
  vector<string> objectList = QuaeroRecoObjectType::listOfPossibleFinalStateObjects();
  short ans = find(objectList.begin(), objectList.end(), objectType) - objectList.begin();
  assert(ans < (short) objectList.size());
  return ans;
}

string shortObjectType2String(short objectType)
{
  vector<string> objectList = QuaeroRecoObjectType::listOfPossibleFinalStateObjects();
  assert(objectType < (short) objectList.size());
  return(objectList[objectType]);
}
  

template<class T>
T makeQuaeroObjectFromSimpleObject(const SimpleObject& simpleObject)
{
  T quaeroObject = T(shortObjectType2String(simpleObject.objectType),CLHEP::HepLorentzVector(simpleObject.e,CLHEP::Hep3Vector(simpleObject.px, simpleObject.py, simpleObject.pz)));
  return quaeroObject;
}

template<class T>
SimpleObject makeSimpleObjectFromQuaeroObject(const T& quaeroObject)
{
  SimpleObject simpleObject;
  simpleObject.objectType = stringObjectType2Short(quaeroObject.getObjectType());
  simpleObject.e = quaeroObject.getFourVector().e();
  simpleObject.px = quaeroObject.getFourVector().px();
  simpleObject.py = quaeroObject.getFourVector().py();
  simpleObject.pz = quaeroObject.getFourVector().pz();
  return simpleObject;
}

template<class T>
vector<T> makeQuaeroObjectsFromSimpleObjects(const vector<SimpleObject>& simpleObjects)
{
  vector<T> ans;
  for(size_t i=0; i<simpleObjects.size(); i++)
    ans.push_back(makeQuaeroObjectFromSimpleObject<T>(simpleObjects[i]));
  return(ans);
}

template<class T>
vector<SimpleObject> makeSimpleObjectsFromQuaeroObjects(const vector<T>& quaeroObjects)
{
  vector<SimpleObject> ans;
  for(size_t i=0; i<quaeroObjects.size(); i++)
    ans.push_back(makeSimpleObjectFromQuaeroObject<T>(quaeroObjects[i]));
  return(ans);
}

bool satisfiesDebuggingPartonObjectCut(const vector<QuaeroPartonObject>& _partonObjects)
{
  return(true);

  /*
  bool passes = true;

  if(_partonObjects.size()<=2)
    passes = false;

  for(size_t i=0; i<_partonObjects.size(); i++)
    {
      string objectType = _partonObjects[i].getObjectTypeSansSign();
      if((objectType=="e")||
	 (objectType=="mu")||
	 (objectType=="tau"))
	passes = false;
    }
  return(passes);
  */
}


TurboSimMorphisms1::TurboSimMorphisms1()
{
  objectPrintFormat = "";


  angleMeasureStick = DELTA_ANGLE;
  
  //angleMeasureFat
  char* angMeasure = getenv( "angleMeasureFat" );
  std::string str1 = "manhattanDistance";
  std::string str2 = "deltaR";
  std::string str3 = "deltaAngle";
  std::string str4 = "kT-Durham";
  if ( str1.compare( angMeasure) == 0 )
    angleMeasureFat = MANHATTAN_DISTANCE;
  else if ( str2.compare( angMeasure ) == 0 )
    angleMeasureFat = DELTA_R;
  else if ( str3.compare( angMeasure ) == 0 )
    angleMeasureFat = DELTA_ANGLE;
  else if ( str4.compare( angMeasure ) == 0 )
    angleMeasureFat = DELTA_R;
  else 
    angleMeasureFat = KT_DURHAM;

  clusterDistanceStick = ( getenv( "clusterDistanceStick" ) == NULL ? 
			   (angleMeasureStick == KT_DURHAM ? 0.001 : 0.25) : 
			   atof(getenv("clusterDistanceStick")) );
  clusterDistanceFat = ( getenv("clusterDistanceFat")==NULL ? 
			 (angleMeasureFat == KT_DURHAM ? 0.002 : 0.5) :  
			 atof(getenv("clusterDistanceFat")) );
  swallowDistance = ( getenv("swallowDistance")==NULL ? 
		      (angleMeasureFat == KT_DURHAM ? 0.001 : 0.2) : 
		      atof(getenv("swallowDistance")) );
  mergeMass = ( getenv("mergeMass")==NULL ? 15 : atof(getenv("mergeMass")) );
  mergeEnergy = ( getenv("mergeEnergy")==NULL ? 10 : atof(getenv("mergeEnergy")) );
  ignoreEnergy = ( getenv("ignoreEnergy")==NULL ? 7.5 : atof(getenv("ignoreEnergy")) );
  maximumNumberOfMorphismTableRowsAbleToBeHeldInMemoryAtOneTime = 500000;
  maxPartonObjectsOnOneLine = 2; 

}


/*****  Create the TurboSim Morphisms Table  *****/

void TurboSimMorphisms1::create(const std::string & partonFileName, 
				const std::string & recoFileName, 
				const std::string & targetTurboSimMorphisms1FileName) {
  
  ofstream fMorphismsFile(targetTurboSimMorphisms1FileName.c_str());
  objectPrintFormat = "(m)-e-costheta-phi(rad)"; // lepton collider

  map<vector<string>,vector<vector<SimpleObject> > > partonObjectsColumn;
  map<vector<string>,vector<vector<SimpleObject> > > recoObjectsColumn;
  map<vector<string>,vector<double> > zvtx;
  map<vector<string>,vector<string> > identifier;
  cout << "reading events  "; system("date");
  int counter = 0;
  QuaeroItemFile<QuaeroPartonEvent> partonEventFile(partonFileName);
  QuaeroItemFile<QuaeroEvent> recoEventFile(recoFileName);  
  QuaeroPartonEvent partonEvent;
  QuaeroEvent recoEvent;
  vector<vector<QuaeroPartonObject> > partonObjectsList;
  vector<vector<QuaeroRecoObject> > recoObjectsList;
  int nrows = 0;
  map<vector<string>,int> numberOfRowsInThisTree;
  map<vector<string>,int> numberOfRowsAlreadyKnockedOutOfThisTree;
  vector<string> largestTree;
  numberOfRowsInThisTree[largestTree]=0;
  numberOfRowsAlreadyKnockedOutOfThisTree[largestTree]=0;
  while(partonEventFile.nextEvent(partonEvent))
    {
      recoEventFile.nextEvent(recoEvent);

      // cout << "parton = " << partonEvent << endl;
      // cout << "reco = " << recoEvent << endl;

      if((partonEvent.getEventType()!=recoEvent.getEventType()) ||
	 (partonEvent.getRunNumber()!=recoEvent.getRunNumber()))
	{
	  cout << "Error!  Mismatch between " << counter << "th event in " << partonFileName << " and " << recoFileName << endl;
	  cout << "  partonEvent:  " << partonEvent << endl;
	  cout << "    recoEvent:  " << recoEvent << endl;
	  cout << "Skipping." << endl; continue;
	  cout << "Aborting." << endl; exit(1);
	}
      partonObjectsList.clear();
      recoObjectsList.clear();
      if(counter==0)
	if(partonEvent.hadronMachine())
	  objectPrintFormat = "(m)-pt-eta-phi(deg)"; // hadron collider
      counter++;
      if(counter%10000==0) {cout << counter << " " << flush;}
      if((partonEvent.getObjects().size()==0)) {
         if(recoEvent.getObjects().size()==0)
            continue;
         else
         {
            cout << endl << "Warning!  This event has no parton-level objects:" << endl;
            cout << partonEvent << endl;
            cout << "But does have reconstructed-level objects:" << endl;
            cout << recoEvent << endl;
         }
      }
      clusterObjects(partonEvent.getObjects(), recoEvent.getObjects(), partonObjectsList, recoObjectsList);
      assert(partonObjectsList.size()==recoObjectsList.size());
      
      for(size_t i=0; i<partonObjectsList.size(); i++)
	{
	  if(chargeConjugationRequiredQ(partonObjectsList[i]))
	    {
	      chargeConjugate(partonObjectsList[i]);
	      chargeConjugate(recoObjectsList[i]);
	    }
	  vector<string> tag = getTag(partonObjectsList[i]);
	  if(tag==vector<string>(1,"uncl"))
	    continue;
	  if(numberOfRowsInThisTree.find(tag)==numberOfRowsInThisTree.end())
	    numberOfRowsInThisTree[tag] = numberOfRowsAlreadyKnockedOutOfThisTree[tag] = 0;
	  if((tag!=largestTree) ||
	     (nrows<maximumNumberOfMorphismTableRowsAbleToBeHeldInMemoryAtOneTime) ||
	     (numberOfRowsInThisTree[tag]==0) ||
	     (drand48() < 1.*numberOfRowsInThisTree[tag]/(numberOfRowsInThisTree[tag] + numberOfRowsAlreadyKnockedOutOfThisTree[tag]))
	     )
	    {
	      partonObjectsColumn[tag].push_back(makeSimpleObjectsFromQuaeroObjects<QuaeroPartonObject>(partonObjectsList[i]));
	      recoObjectsColumn[tag].push_back(makeSimpleObjectsFromQuaeroObjects<QuaeroRecoObject>(recoObjectsList[i]));
	      zvtx[tag].push_back(partonEvent.getZVtx());
	      identifier[tag].push_back(partonEvent.getEventType()+" "+partonEvent.getRunNumber());
	      nrows++;
	      numberOfRowsInThisTree[tag]++;
	      if(numberOfRowsInThisTree[tag]>numberOfRowsInThisTree[largestTree])
		largestTree = tag;
	    }
	  if(nrows>=maximumNumberOfMorphismTableRowsAbleToBeHeldInMemoryAtOneTime)
	    {
	      int r = (int)(drand48()*numberOfRowsInThisTree[largestTree]);
	      partonObjectsColumn[largestTree][r] = partonObjectsColumn[largestTree].back();
	      partonObjectsColumn[largestTree].pop_back();
	      recoObjectsColumn[largestTree][r] = recoObjectsColumn[largestTree].back();
	      recoObjectsColumn[largestTree].pop_back();
	      zvtx[largestTree][r] = zvtx[largestTree].back();
	      zvtx[largestTree].pop_back();
	      identifier[largestTree][r] = identifier[largestTree].back();
	      identifier[largestTree].pop_back();
	      numberOfRowsAlreadyKnockedOutOfThisTree[largestTree]++;
	      numberOfRowsInThisTree[largestTree]--;
	      nrows--;
	    }
	}
    }
  
  if(getenv("nodoubletree") == NULL)
    {
      counter = 0;
      cout << "handling doublets -- building tree "; system("date");
      
     // Now take care of the doublets
      // ==============================
      QuaeroParticleMorphismsSingle<QuaeroRecoObject,QuaeroPartonObject> quaeroParticleMorphismsSingle1;
      QuaeroParticleMorphismsSingle<QuaeroPartonObject,QuaeroRecoObject> quaeroParticleMorphismsSingle2;
      for(map<vector<string>, vector<vector<SimpleObject> > >::iterator i1 = partonObjectsColumn.begin();
	  i1 != partonObjectsColumn.end(); i1++)
	{
	  vector<string> tag = i1->first;
	  for(size_t i2=0; i2<partonObjectsColumn[tag].size(); i2++)
	    {
	      counter++;
	      if(counter%10000==0)
		cout << counter << " " << flush;
	      if((partonObjectsColumn[tag][i2].size()==1)&&
		 ((recoObjectsColumn[tag][i2].size()==1)||
		  ((recoObjectsColumn[tag][i2].size()==2)&&
		   (makeQuaeroObjectFromSimpleObject<QuaeroRecoObject>(recoObjectsColumn[tag][i2][1]).getObjectType()=="uncl"))))
		{
		  // cout << "yes1" << endl;
		  if((makeQuaeroObjectFromSimpleObject<QuaeroPartonObject>(partonObjectsColumn[tag][i2][0]).getObjectType()
		      ==makeQuaeroObjectFromSimpleObject<QuaeroRecoObject>(recoObjectsColumn[tag][i2][0]).getObjectType()))
		    {
		      // cout << "yes2: " << makeQuaeroObjectFromSimpleObject<QuaeroPartonObject>(partonObjectsColumn[tag][i2][0]).getObjectType() << " = " << makeQuaeroObjectFromSimpleObject<QuaeroRecoObject>(recoObjectsColumn[tag][i2][0]).getObjectType() << endl;
		      quaeroParticleMorphismsSingle1.Add(makeQuaeroObjectFromSimpleObject<QuaeroRecoObject>(recoObjectsColumn[tag][i2][0]), vector<QuaeroPartonObject>(1,makeQuaeroObjectFromSimpleObject<QuaeroPartonObject>(partonObjectsColumn[tag][i2][0])), zvtx[tag][i2]);
		    }
		}
	      if(((partonObjectsColumn[tag][i2].size()==1)&&(makeQuaeroObjectFromSimpleObject<QuaeroPartonObject>(partonObjectsColumn[tag][i2][0]).getObjectType()!="uncl")))
		{
		  //cout << partonObjectsColumn[i][0].print("(m)-pt-eta-phi(deg)") << endl;
		  quaeroParticleMorphismsSingle2.Add(makeQuaeroObjectFromSimpleObject<QuaeroPartonObject>(partonObjectsColumn[tag][i2][0]), 
						     makeQuaeroObjectsFromSimpleObjects<QuaeroRecoObject>(recoObjectsColumn[tag][i2]), 
						     zvtx[tag][i2]);
		}
	    }
	}
      counter = 0;
      cout << "handling doublets -- using tree "; system("date");
      string identifier="";
      for(map<vector<string>, vector<vector<SimpleObject> > >::iterator i1 = partonObjectsColumn.begin();
	  i1 != partonObjectsColumn.end(); i1++)
	{
	  vector<string> tag = i1->first;
	  for(size_t i2=0; i2<partonObjectsColumn[tag].size(); i2++)
	    {
	      counter++;
	      if(counter%10000==0)
		cout << counter << " " << flush;
	      if((partonObjectsColumn[tag][i2].size()==2)||(partonObjectsColumn[tag][i2].size()==3))
		{
		  for(size_t j=0; j<recoObjectsColumn[tag][i2].size(); j++)
		    {		 
		      QuaeroRecoObject o1 = makeQuaeroObjectFromSimpleObject<QuaeroRecoObject>(recoObjectsColumn[tag][i2][j]);
		      bool cc = chargeConjugationRequiredQ(vector<QuaeroRecoObject>(1,o1));
		      if(cc)
			o1.chargeConjugate();
		      vector<QuaeroPartonObject> link = quaeroParticleMorphismsSingle1.findClosest(o1, zvtx[tag][i2], identifier);
		      if(o1.getObjectType()=="kill")
			link = vector<QuaeroPartonObject>(1,QuaeroPartonObject("kill",o1.getFourVector()));
		      if(link.size()!=1)
			{
			  cout << "Link size = " << link.size() << endl;
			  cout << o1.print("(m)-pt-eta-phi(deg)") << " appears as a reconstructed object, but no parton-level object of type " << o1.getObjectType() << " appears." << endl;
			  cout << "Please include events with parton-level objects of type " << o1.getObjectType() << "." << endl;
			  cout << "Aborting." << endl;
			  exit(1);
			}
		      assert(link.size()==1);
		      QuaeroRecoObject o2(link[0].getObjectType(), link[0].getFourVector());
		      if(cc)
			o2.chargeConjugate();
		      recoObjectsColumn[tag][i2][j] = makeSimpleObjectFromQuaeroObject<QuaeroRecoObject>(o2);
		    }
		  if(recoObjectsColumn[tag][i2].empty())
		    {
		      for(size_t j=0; j<partonObjectsColumn[tag][i2].size(); j++)
			{
			  vector<QuaeroRecoObject> link = quaeroParticleMorphismsSingle2.findClosest(makeQuaeroObjectFromSimpleObject<QuaeroPartonObject>(partonObjectsColumn[tag][i2][j]), zvtx[tag][i2], identifier);
			  if((link.empty()) || ((link.size()==1) && (link[0].getObjectType()=="uncl")))
			    recoObjectsColumn[tag][i2].push_back(partonObjectsColumn[tag][i2][j]);
			}
		    }
		}
	    }
	}
    }
  
  counter = 0;
  cout << "writing to file" << endl;
  double tol = 0.001;
  // Now write to file
  fMorphismsFile << objectPrintFormat << endl << endl;
  for(map<vector<string>, vector<vector<SimpleObject> > >::iterator i1 = partonObjectsColumn.begin();
      i1 != partonObjectsColumn.end(); i1++)
    {
      vector<string> tag = i1->first;
      for(size_t i2=0; i2<partonObjectsColumn[tag].size(); i2++)
	{
	  counter++;
	  if(counter%10000==0)
	    cout << counter << " " << flush;
	  if(!((partonObjectsColumn[tag][i2].size()==1)&&
	       (makeQuaeroObjectFromSimpleObject<QuaeroPartonObject>(partonObjectsColumn[tag][i2][0]).getObjectType()=="uncl")))
	    {
	      fMorphismsFile << "*  " << flush;
	      fMorphismsFile << identifier[tag][i2] << "  " << flush;
	      fMorphismsFile << zvtx[tag][i2] << flush;
	      for(size_t k=0; k<partonObjectsColumn[tag][i2].size(); k++)
		fMorphismsFile << "  " << makeQuaeroObjectFromSimpleObject<QuaeroPartonObject>(partonObjectsColumn[tag][i2][k]).print(objectPrintFormat,tol) << flush;
	      fMorphismsFile << " ;   -> ";
	      for(size_t k=0; k<recoObjectsColumn[tag][i2].size(); k++)
		fMorphismsFile << "  " << makeQuaeroObjectFromSimpleObject<QuaeroRecoObject>(recoObjectsColumn[tag][i2][k]).print(objectPrintFormat,tol) << flush;
	      fMorphismsFile << " ; " << endl;
	    }
	}
    }
  fMorphismsFile.close();
  return;
}

/*****  Read the TurboSim Morphisms file and build the multivariate binary tree  *****/

void TurboSimMorphisms1::readFromFile(const string & filename)
{
  ifstream fin(filename.c_str());
  string marker;
  double zvtx;
  string eventType, runNumber, identifier;
  int k = 0;
  cout << "reading file... " << flush;
  bool skipFakes = false;
  if(getenv("skipFakes")!=NULL)
    {
      cout << "(skipping fakes) " << flush;
      skipFakes = true;
    }
  int maximumNumberOfMorphismTableLinesToUse = ((int)(1e8));
  if(getenv("maximumNumberOfMorphismTableLinesToUse")!=NULL)
    maximumNumberOfMorphismTableLinesToUse = atoi(getenv("maximumNumberOfMorphismTableLinesToUse"));

  if(fin)
    {

      // Read the object format:  lepton or hadron machine
      fin >> objectPrintFormat;
      while(fin >> marker)
	{
	  k++;
	  if(k%100000==0)
	    cout << k << " " << flush;
	  if(k>maximumNumberOfMorphismTableLinesToUse)
	    break;
	  assert(marker=="*");
	  fin >> eventType >> runNumber;
	  identifier = eventType+"-"+runNumber;
	  fin >> zvtx; // vertext position

	  QuaeroPartonObject partonObject;
	  vector<QuaeroPartonObject> partonObjects;
	  QuaeroRecoObject fsObject;
	  vector<QuaeroRecoObject> fsObjects;

	  bool parton_NoLeptons = true;
	  while(partonObject.read(fin,objectPrintFormat))
	    {
	      if(partonObject.getObjectType()!="uncl")
		partonObjects.push_back(partonObject);
	      if((partonObject.getObjectType()=="j")||
		 (partonObject.getObjectType()=="b")||
		 (partonObject.getObjectType()=="uncl")
		 )
		;
	      else
		parton_NoLeptons = false;
	    }
	  
	  fin >> marker;
	  assert(marker=="->");
	  
	  bool fs_ContainsLepton = false;
	  while(fsObject.read(fin,objectPrintFormat))
	    {
	      fsObjects.push_back(fsObject);
	      if((fsObject.getObjectTypeSansSign()=="e")||
		 (fsObject.getObjectTypeSansSign()=="mu")||
		 (fsObject.getObjectTypeSansSign()=="tau")||
		 (fsObject.getObjectTypeSansSign()=="ph")
		 )
		fs_ContainsLepton = true;
	    }

	  if(skipFakes)
	    if(parton_NoLeptons && fs_ContainsLepton)
	      continue; // skip fakes

	  assert((partonObjects.size()<=3));
	  if(partonObjects.size()==1) {
         if(partonObjects[0].getFourVector().perp()<0.002) {
	      continue;
         } else {
            turboSimMorphismsForestSingle.Add(partonObjects[0], fsObjects, zvtx, identifier);
         }
      }
	  if(partonObjects.size()==2)
	    turboSimMorphismsForestDouble.Add(partonObjects[0], partonObjects[1], fsObjects, zvtx, identifier);
	  if(partonObjects.size()==3)
	    turboSimMorphismsForestTriple.Add(partonObjects, fsObjects, zvtx, identifier);

	}
    }
  else
    {
      cout << "TurboSim morphisms file '"+filename+"' not found." << endl;
      cout << "Aborting." << endl;
      assert(false);
    }
  fin.close();
  return;
}


QuaeroEvent TurboSimMorphisms1::morph(const QuaeroPartonEvent& partonEvent)
{
  vector<QuaeroPartonObject> partonObjects = partonEvent.getObjects();
  vector<vector<QuaeroPartonObject> > partonObjectsList;
  clusterObjects(partonObjects, partonObjectsList);

  /*
  for(size_t i=0; i<partonObjectsList.size(); i++)
    {
      for(size_t j=0; j<partonObjectsList[i].size(); j++)
	cout << partonObjectsList[i][j].print("(m)-pt-eta-phi(deg)") << " , " << flush;
      cout << endl;
    }
  */

  vector<QuaeroRecoObject> recoObjects;
  string comment;
  if(getenv("debug")!=NULL)
    cout << partonEvent.getEventType() << "  " << partonEvent.getRunNumber() << endl;
  for(size_t i=0; i<partonObjectsList.size(); i++)
    {
      vector<QuaeroRecoObject> a = morph(partonObjectsList[i], partonEvent.getZVtx(), comment);
      if(getenv("debug")!=NULL)
	{
	  cout << "  ";
	  for(size_t j=0; j<partonObjectsList[i].size(); j++)
	    cout << partonObjectsList[i][j].print("(m)-pt-eta-phi(deg)") << "  ";
	  cout << "TO  ";
	  for(size_t j=0; j<a.size(); j++)
	    cout << a[j].print("(m)-pt-eta-phi(deg)") << "  ";
	  cout << "FROM  " << comment << endl;
	}
      recoObjects.insert(recoObjects.end(), a.begin(), a.end());
    }

  QuaeroEvent quaeroEvent(partonEvent.getEventType(), partonEvent.getRunNumber(), partonEvent.getWeight(), 
			  recoObjects, partonEvent.getCollisionType(), partonEvent.getRootS(), partonEvent.getZVtx(), partonEvent.getGeneratedSumPt());
  return(quaeroEvent);
}


//==================================
// Private
//==================================

vector<QuaeroRecoObject> TurboSimMorphisms1::morph(const vector<QuaeroPartonObject>& _partonObjects, 
						   double zvtx, 
						   string & comment)
{
  assert(_partonObjects.size()<=3);
  vector<QuaeroRecoObject> result;
  comment="";
  string comment1="";

  vector<QuaeroPartonObject> partonObjects = _partonObjects;
  bool cc = chargeConjugationRequiredQ(partonObjects);
  if(cc)
    chargeConjugate(partonObjects);

  if(partonObjects.size()==1)
    {
      result = turboSimMorphismsForestSingle.findClosest(partonObjects[0], zvtx, comment);
    }
  if(partonObjects.size()==2)
    {
      vector<QuaeroRecoObject> result1 = turboSimMorphismsForestDouble.findClosest(partonObjects[0], partonObjects[1], zvtx, comment);
      for(size_t i=0; i<result1.size(); i++)
	{
	  vector<QuaeroRecoObject> a = vector<QuaeroRecoObject>(1,result1[i]);
	  if(getenv("nodoubletree")==NULL)
	    a = turboSimMorphismsForestSingle.findClosest(QuaeroPartonObject(result1[i].getObjectType(),result1[i].getFourVector()), zvtx, comment1);
	  comment += "_"+comment1;
	  if(result.empty())
	    result = a;
	  else
	    result.insert(result.end(), a.begin(), a.end());
	}
    }
  if(partonObjects.size()==3)
    {
      vector<QuaeroRecoObject> result1 = turboSimMorphismsForestTriple.findClosest(partonObjects, zvtx, comment);
      for(size_t i=0; i<result1.size(); i++)
	{
	  vector<QuaeroRecoObject> a = vector<QuaeroRecoObject>(1,result1[i]);
	  if(getenv("nodoubletree")==NULL)
	    a = turboSimMorphismsForestSingle.findClosest(QuaeroPartonObject(result1[i].getObjectType(),result1[i].getFourVector()), zvtx, comment1);
	  comment += "_"+comment1;
	  if(result.empty())
	    result = a;
	  else
	    result.insert(result.end(), a.begin(), a.end());
	}
    }
  if(partonObjects.size()>=4)
    cout << "Multiparticle gathering -- giving up." << endl;
  if(cc) 
    chargeConjugate(result);
  return(result);
}

bool stickParticleObject(string objectTypeSansSign)
{
  bool ans = 
    ( (objectTypeSansSign=="e") ||
      (objectTypeSansSign=="mu") ||
      (objectTypeSansSign=="tau") ||
      (objectTypeSansSign=="ph") );
  return(ans);
}


void TurboSimMorphisms1::clusterObjects(const vector<QuaeroPartonObject>& _partonObjects, vector<vector<QuaeroPartonObject> >& partonObjectsList)
{  

  partonObjectsList = vector<vector<QuaeroPartonObject> >(0);
  vector<QuaeroPartonObject> partonObjects = _partonObjects;
  for(size_t i=0; i<partonObjects.size(); i++)
    if((partonObjects[i].getFourVector().perp()<3)||(partonObjects[i].getObjectType()=="uncl"))
      {
	partonObjects.erase(partonObjects.begin()+i);
	i--;
      }

  for(size_t i=0; i<partonObjects.size(); i++)
    for(size_t j=i+1; j<partonObjects.size(); j++)
      {
	string q = "";
	if( ((partonObjects[i].getObjectTypeSansSign()=="e")&&
	     (partonObjects[j].getObjectTypeSansSign()=="ph")) )
	  q = partonObjects[i].getObjectType();
	if( ((partonObjects[i].getObjectTypeSansSign()=="ph")&&
	     (partonObjects[j].getObjectTypeSansSign()=="e")) )
	  q = partonObjects[j].getObjectType();
	if( ((partonObjects[i].getObjectTypeSansSign()=="j")&&
	     (partonObjects[j].getObjectTypeSansSign()=="ph")) )
	  q = partonObjects[i].getObjectType();
	if( ((partonObjects[i].getObjectTypeSansSign()=="ph")&&
	     (partonObjects[j].getObjectTypeSansSign()=="j")) )
	  q = partonObjects[j].getObjectType();
	if((q!=""))
	  {
	    if( ( (q=="j")&&(objectPrintFormat=="(m)-pt-eta-phi(deg)")&& // hadron collider
		  (deltaAngle(partonObjects[i].getFourVector(),partonObjects[j].getFourVector(),DELTA_R) < 0.5)) ||
		( (q.substr(0,1)=="e") &&
		  (deltaAngle(partonObjects[i].getFourVector(),partonObjects[j].getFourVector(),DELTA_ANGLE) < 0.2) ) )
	      {
		partonObjects[i] = QuaeroPartonObject(q,partonObjects[i].getFourVector()+partonObjects[j].getFourVector());
		partonObjects.erase(partonObjects.begin()+j);
		j--;
	      }
	  }
      }
  
  CLHEP::HepLorentzVector smallOffset = CLHEP::HepLorentzVector(0.007*sqrt(7.),CLHEP::Hep3Vector(0.007*sqrt(1.),0.007*sqrt(2.),0.007*sqrt(3.))); // ensure that find(...) below doesn't fail because of numerical roundoff
  for(size_t i=0; i<partonObjects.size(); i++)
    partonObjects[i] = QuaeroPartonObject(partonObjects[i].getObjectType(),partonObjects[i].getFourVector()+smallOffset);
  for(size_t i=0; i<partonObjects.size(); i++)
    partonObjectsList.push_back(vector<QuaeroPartonObject>(1,partonObjects[i]));

  // Collect particle matches in one  
  // line if parton objects are close
  // ===============================
  for(size_t i=0; i<partonObjects.size(); i++)
    for(size_t j=i+1; j<partonObjects.size(); j++)
      {
	bool closeTogether = false;
	if(stickParticleObject(partonObjects[i].getObjectTypeSansSign())&&
	   stickParticleObject(partonObjects[j].getObjectTypeSansSign()))
	  closeTogether = (deltaAngle(partonObjects[i].getFourVector(),partonObjects[j].getFourVector(),angleMeasureStick) < clusterDistanceStick);
	else
	  closeTogether = (deltaAngle(partonObjects[i].getFourVector(),partonObjects[j].getFourVector(),angleMeasureFat) < clusterDistanceFat);
	   
      if(closeTogether)
	{
	  int ki = -1, kj = -1;
	  for(size_t k=0; k<partonObjectsList.size(); k++)
	    {
	      for(size_t l=0; l<partonObjectsList[k].size(); l++)
		{
		  // if(find(partonObjectsList[k].begin(), partonObjectsList[k].end(), partonObjects[i]) != partonObjectsList[k].end())
		  if((partonObjects[i].getObjectType()==partonObjectsList[k][l].getObjectType()) &&
		     Math::MPEquality(partonObjects[i].getFourVector().e(),partonObjectsList[k][l].getFourVector().e(),0.01)&&		  
		     Math::MPEquality(partonObjects[i].getFourVector().cosTheta(),partonObjectsList[k][l].getFourVector().cosTheta(),0.01)&&
		     Math::MPEquality(Math::deltaphi(partonObjects[i].getFourVector().phi(),partonObjectsList[k][l].getFourVector().phi()),0,0.01))
		    ki = k;
		  // if(find(partonObjectsList[k].begin(), partonObjectsList[k].end(), partonObjects[j]) != partonObjectsList[k].end())
		  if((partonObjects[j].getObjectType()==partonObjectsList[k][l].getObjectType())&&
		     Math::MPEquality(partonObjects[j].getFourVector().e(),partonObjectsList[k][l].getFourVector().e(),0.01)&&
		     Math::MPEquality(partonObjects[j].getFourVector().cosTheta(),partonObjectsList[k][l].getFourVector().cosTheta(),0.01)&&
		     Math::MPEquality(Math::deltaphi(partonObjects[j].getFourVector().phi(),partonObjectsList[k][l].getFourVector().phi()),0,0.01))
		    kj = k;
		}
	    }
	  /*
	  if((ki<0)||(kj<0))
	    {
	      cout << "oops  " << ki << "," << kj << " " << partonObjectsList.size() << " i=" << i << " j=" << j << " " << partonObjects[i].print(objectPrintFormat) << " " << partonObjects[j].print(objectPrintFormat) << endl;
	      for(int k=0; k<partonObjectsList.size(); k++)
		{
		  cout << "(k=" << k << "): ";
		  for(int l=0; l<partonObjectsList[k].size(); l++)
		    cout << partonObjectsList[k][l].print(objectPrintFormat) << " , ";
		  cout << endl;
		}
	    }
	  */
	  if((ki!=kj)&&((ki>=0)&&(kj>=0)))
	    {
	      partonObjectsList[ki].insert(partonObjectsList[ki].end(), partonObjectsList[kj].begin(), partonObjectsList[kj].end());
	      partonObjectsList.erase(partonObjectsList.begin()+kj, partonObjectsList.begin()+kj+1);
	    }
	}
      }
  if(getenv("noclosemerge") == NULL)
    {
  
      // Merge particles into one if partonObjects 
      // are extremely close (according to the rules below)
      // ==================================================
      for(size_t n=0; n<partonObjectsList.size(); n++)
	{
	  if(partonObjectsList[n].size()>1)
	    {
	      vector<CLHEP::HepLorentzVector> mergedJets = vector<CLHEP::HepLorentzVector>(partonObjectsList[n].size(),0);
	      //	      int jetMerged, jetIndex, jetSwallowerIndex;
	      //double jetPt;
	      for(size_t i=0; i<partonObjectsList[n].size(); i++)
		{
		  if((partonObjectsList[n][i].getObjectType() == "j") || (partonObjectsList[n][i].getObjectType() == "b"))
		    mergedJets[i] += partonObjectsList[n][i].getFourVector();
		}
	      for(size_t i=0; i<partonObjectsList[n].size(); i++)
		for(size_t j=i+1; j<partonObjectsList[n].size(); j++)
		  if(((partonObjectsList[n][i].getObjectType()=="j")||(partonObjectsList[n][i].getObjectType()=="b"))&&
		     ((partonObjectsList[n][j].getObjectType()=="j")||(partonObjectsList[n][j].getObjectType()=="b"))&&
		     (deltaAngle(partonObjectsList[n][i].getFourVector(),partonObjectsList[n][j].getFourVector()) < swallowDistance))		    {
		      CLHEP::HepLorentzVector newFourVector =  partonObjectsList[n][i].getFourVector() + partonObjectsList[n][j].getFourVector();
		      string newType = "j";
		      if((partonObjectsList[n][i].getObjectType()=="b")||(partonObjectsList[n][j].getObjectType()=="b"))
			newType = "b";
		      partonObjectsList[n][i] = QuaeroPartonObject(newType, newFourVector); 
		      partonObjectsList[n].erase(partonObjectsList[n].begin()+j);
		      j--;
		    }
	    }	
	}
    }


  if(getenv("noradmerge") == NULL)
    {
      // Merge radiated particles with pt less than 10 GeV 
      // into nearest particle (according to the rules below)
      // ====================================================
      for(size_t n=0; n<partonObjectsList.size(); n++)
	{
	  if(partonObjectsList[n].size()>1)
	    {
	      for(size_t i=0; i<partonObjectsList[n].size(); i++)
		{
		  double e = 0;
		  if(objectPrintFormat=="(m)-e-costheta-phi(rad)") // lepton collider
		    e = partonObjectsList[n][i].getFourVector().e();
		  else
		    e = partonObjectsList[n][i].getFourVector().perp();
		  if(((partonObjectsList[n][i].getObjectType()=="j")||
		      (partonObjectsList[n][i].getObjectType()=="ph"))
		     &&(e < mergeEnergy))
		  {
		    //double minDistance = mergeDistance, distance;
		    double minMass = mergeMass, mass;
		    int closestObject = -1;
		    // Jets, photons can be merged into nearest j or b
		    // ---------------------------------------
		    for(size_t j=0; j<partonObjectsList[n].size(); j++)
		      if((j != i) &&
			 ((partonObjectsList[n][j].getObjectType()=="j") ||
			  (partonObjectsList[n][j].getObjectType()=="ph") ||
			  (partonObjectsList[n][j].getObjectType()=="b")))
			{
			  /*
			    distance = (Math::deltaR(partonObjectsList[n][i].getFourVector().phi(), 
						   Math::theta2eta(partonObjectsList[n][i].getFourVector().theta()), 
						   partonObjectsList[n][j].getFourVector().phi(),  
						   Math::theta2eta(partonObjectsList[n][j].getFourVector().theta())));
			  if(distance<minDistance)
			    {
			      minDistance = distance;
			      closestObject = j;
			    }
			  */
			  mass = (partonObjectsList[n][i].getFourVector()+partonObjectsList[n][j].getFourVector()).m();
			  if(mass<minMass)
			    {
			      minMass = mass;
			      closestObject = j;
			    }
			}
		    if((closestObject>=0)) 
		      {
			CLHEP::HepLorentzVector newFourVector =  partonObjectsList[n][i].getFourVector() + partonObjectsList[n][closestObject].getFourVector();
			partonObjectsList[n][closestObject] = QuaeroPartonObject(partonObjectsList[n][closestObject].getObjectType(), newFourVector); 
			partonObjectsList[n].erase(partonObjectsList[n].begin()+i, partonObjectsList[n].begin()+i+1);
			i--;
		      }
		  }
		}
	    }
	} // End loop over partonObjectsList.size()
    }
  
  // Put very low pt objects into "uncl"
  // ===================================
  CLHEP::HepLorentzVector unclFourVector;
  for(size_t i=0; i<partonObjectsList.size(); i++)
    {
      double sum = 0;
      for(size_t j=0; j<partonObjectsList[i].size(); j++)
	{
	  if(objectPrintFormat=="(m)-e-costheta-phi(rad)") // lepton collider
	    sum += partonObjectsList[i][j].getFourVector().e();
	  else
	    sum += partonObjectsList[i][j].getFourVector().perp();
	}
      if(sum < ignoreEnergy)
	{
	  for(size_t j=0; j<partonObjectsList[i].size(); j++)
	    unclFourVector +=  partonObjectsList[i][j].getFourVector();
	  partonObjectsList.erase(partonObjectsList.begin()+i);
	  i--;
	}
    }
  QuaeroPartonObject uncl = QuaeroPartonObject("uncl", unclFourVector);
  partonObjectsList.insert(partonObjectsList.end(), vector<QuaeroPartonObject>(1,uncl)); 

  if(getenv("nopairmerge") == NULL)
    {

      // Combine objects until we have only two objects 
      // on the same line in partonEvents
      // =================================
      for(size_t n=0; n<partonObjectsList.size(); n++)
	{
	  
	  if(true)
	    {	      
	      if(partonObjectsList[n].size()>2)
		{
		  double distance = FLT_MAX;
		  int qj=-1, qk=-1;
		  for(size_t j=0; j<partonObjectsList[n].size(); j++)
		    for(size_t k=j+1; k<partonObjectsList[n].size(); k++)
		      {
			double d = deltaAngle(partonObjectsList[n][j].getFourVector(),partonObjectsList[n][k].getFourVector());
			if(d<distance)
			  {
			    distance = d;
			    qj = j; qk = k;
			  }
		      }
		  partonObjectsList.push_back(partonObjectsList[n]);
		  partonObjectsList[partonObjectsList.size()-1].erase(partonObjectsList[partonObjectsList.size()-1].begin()+qk);
		  partonObjectsList[partonObjectsList.size()-1].erase(partonObjectsList[partonObjectsList.size()-1].begin()+qj);
		  for(int j=partonObjectsList[n].size()-1; j>=0; j--)
		    if((j!=qj)&&(j!=qk))
		      partonObjectsList[n].erase(partonObjectsList[n].begin()+j);
		}
	      

	    }

	  // comment out the below
	  else
	    {
	      if(partonObjectsList[n].size()>1)
		{
		  while(partonObjectsList[n].size()>3)       
		    {
		      // First find lowest pt j or b in the line
		      // -------------------------------------------
		      double minPt = FLT_MAX;
		      bool noIndex = true;
		      size_t minIndex = 0;
		      for(size_t i=0; i<partonObjectsList[n].size(); i++)
			{
			  double e=0;
			  if(objectPrintFormat=="(m)-e-costheta-phi(rad)") // lepton collider
			    e = partonObjectsList[n][i].getFourVector().e();
			  else
			    e = partonObjectsList[n][i].getFourVector().perp();		    
			  if(((partonObjectsList[n][i].getObjectType()=="j")||(partonObjectsList[n][i].getObjectType()=="b"))&&
			     (e < minPt))
			    { 
			      minIndex = i;
			      noIndex = false;
			      minPt = e;
			    }
			}
		      if(noIndex)
			break;
		      
		      // Find j or b closest to minPt j or b
		      // ------------------------------------------------------------------
		      double minDistance = FLT_MAX, distance;
		      int closestObject = -1;
		      for(size_t i=0; i<partonObjectsList[n].size(); i++)
			if((i != minIndex) &&
			   ((partonObjectsList[n][i].getObjectType()=="j")||(partonObjectsList[n][i].getObjectType()=="b")))
			  {
			    distance = (deltaAngle(partonObjectsList[n][i].getFourVector(),partonObjectsList[n][minIndex].getFourVector()));
			    if(distance<minDistance)
			      {
				minDistance = distance;
				closestObject = i;
			      }
			  }
		      if(closestObject>=0) 
			{
			  CLHEP::HepLorentzVector newFourVector =  partonObjectsList[n][minIndex].getFourVector() + partonObjectsList[n][closestObject].getFourVector();
			  string newType = "j";
			  if((partonObjectsList[n][minIndex].getObjectType()=="b")||(partonObjectsList[n][closestObject].getObjectType()=="b"))
			    newType = "b";
			  partonObjectsList[n][closestObject] = QuaeroPartonObject(newType, newFourVector); 
			  partonObjectsList[n].erase(partonObjectsList[n].begin()+minIndex, partonObjectsList[n].begin()+minIndex+1);
			}
		      else
			{
			  break;
			}
		    }
		  if(partonObjectsList[n].size()>2)
		    {
		      double minMass = FLT_MAX;
		      int i1=-1, i2=-1;
		      for(size_t i=0; i<partonObjectsList[n].size(); i++)
			for(size_t j=i+1; j<partonObjectsList[n].size(); j++)
			  {
			    double m = (partonObjectsList[n][i].getFourVector()+partonObjectsList[n][j].getFourVector()).m();
			    if(m<minMass)
			      {
				minMass = m;
				i1 = i;
				i2 = j;
			      }
			  }
		      assert((i1>=0)&&(i2>=0));
		      partonObjectsList.push_back(partonObjectsList[n]);
		      partonObjectsList[n] = vector<QuaeroPartonObject>(1,partonObjectsList.back()[i1]);
		      partonObjectsList[n].push_back(partonObjectsList.back()[i2]);
		      partonObjectsList.back().erase(partonObjectsList.back().begin()+i2);
		      partonObjectsList.back().erase(partonObjectsList.back().begin()+i1);
		    }
		  /*
		    if(partonObjectsList[n].size()>3)
		    {
		    double minPt = FLT_MAX;
		    int minIndex = -1;
		    for(size_t i=0; i<partonObjectsList[n].size(); i++)
		    if((partonObjectsList[n][i].getFourVector().perp() < minPt))
		    { 
		    minIndex = i;
		    minPt = partonObjectsList[n][i].getFourVector().perp();
		    }
		    assert(minIndex>=0);
		    partonObjectsList[n].erase(partonObjectsList[n].begin()+minIndex);
		    }
		  */
		  
		} // End while loop to get down to two objects per line
	    }
	} // End if more than one object in line
    } // End n loop over size of partonObjectsList  
  
  // order parton objects in each cluster according to decreasing pt
  for(size_t i=0; i<partonObjectsList.size(); i++)
    for(size_t j=0; j<partonObjectsList[i].size(); j++)
      for(size_t k=j+1; k<partonObjectsList[i].size(); k++)
	if(partonObjectsList[i][j].getFourVector().perp() < partonObjectsList[i][k].getFourVector().perp())
	  swap(partonObjectsList[i][j],partonObjectsList[i][k]);
    
  return;
}

class UnclLikelihood: public Math::FunctionObject
{

public:
  UnclLikelihood(const vector<string>& _objectTypes, const vector<double>& _lhs_p, const vector<double>& _rhs_p)
  {
    n = _lhs_p.size();
    assert(n==_rhs_p.size());
    assert(n==_objectTypes.size());
    lhs_p = _lhs_p;
    rhs_p = _rhs_p;
    objectTypes = _objectTypes;
  }

  virtual ~UnclLikelihood() {}

  double operator()(const vector<double>& _x)
  {
    double ans = 0;
    double deltaE = 1.2; // units are GeV
    double fudgeE = 4.0; // units are GeV
    vector<double> x = _x;
    assert(x.size()==n);
    for(size_t i=0; i<n; i++)
      {
	double p = 2.;
	if(((objectTypes[i]=="btau")&&(lhs_p[i]>rhs_p[i]+x[i])))
	  p = 1.; // bOrTauWithEscapedNeutrino

	ans += pow(
		   fabs(x[i] + rhs_p[i] - lhs_p[i])/
		   Math::addInQuadrature(
					 sqrt((fabs(lhs_p[i])+fabs(rhs_p[i]))/2)
					 ,deltaE),
		   p); // pt_x
      }
    ans += pow( fabs(accumulate(x.begin(), x.end(), 0.))/
		fudgeE, 
		2.);
    ans = sqrt(ans);
    return(ans);
  }

private:
  size_t n;
  vector<double> lhs_p, rhs_p;
  vector<string> objectTypes;
};



void TurboSimMorphisms1::clusterObjects(const vector<QuaeroPartonObject>& partonObjects, const vector<QuaeroRecoObject>& recoObjects, 
				       vector<vector<QuaeroPartonObject> >& partonObjectsList, vector<vector<QuaeroRecoObject> >& recoObjectsList)
{

  partonObjectsList = vector<vector<QuaeroPartonObject> >(0);
  clusterObjects(partonObjects, partonObjectsList);
  recoObjectsList = vector<vector<QuaeroRecoObject> >(partonObjectsList.size());


  // Every reconstructed object is associated with the closest parton object
  //  (every effect has a cause)
  // ----------------------------
  for(size_t i=0; i<recoObjects.size(); i++)
    {
      if((recoObjects[i].getObjectType()!="uncl")&&
	 (recoObjects[i].getObjectType()!="jf")&&
	 (recoObjects[i].getObjectTypeSansSign()!="jtau"))
	{
	  double minDistance = FLT_MAX, distance;
	  int closestPartonObjects = -1;
	  for(size_t k=0; k<partonObjectsList.size(); k++)
	    for(size_t j=0; j<partonObjectsList[k].size(); j++)
	      {
		distance = deltaAngle(recoObjects[i].getFourVector(),partonObjectsList[k][j].getFourVector(),DELTA_ANGLE);  
		if(distance<minDistance)
		  {
		    minDistance = distance;
		    closestPartonObjects = k;
		  }
	      }
	  if(closestPartonObjects>=0)
	    recoObjectsList[closestPartonObjects].push_back(recoObjects[i]);
	  else
	    {
	      cout << "bad" << endl;
	      recoObjectsList.back().push_back(recoObjects[i]); // associate with "uncl"
	    }
	}
      
      // Unclustered energy is treated separately and associated with uncl 
      // ----------------------------------------------------------------
      else if(recoObjects[i].getObjectType()=="uncl")
	{
	  int unclPos = -1;
	  for(size_t j=0; j<partonObjectsList.size(); j++)
	    {
	      assert(partonObjectsList[j].size()>=1);
	      if(partonObjectsList[j][0].getObjectType()=="uncl")
		unclPos = j;
	    }
	  if(unclPos<0)
	    cout << partonObjects.size() << endl;
	  assert(unclPos>=0);
	  recoObjectsList[unclPos].push_back(recoObjects[i]);
	}      
    }
  
  for(size_t i=0; i<partonObjectsList.size(); i++)
    for(size_t j=0; j<partonObjectsList[i].size(); j++)
      {
      if(((partonObjectsList[i][j].getObjectType()=="b")||
	  (partonObjectsList[i][j].getObjectType()=="j"))&&
	  ( ((objectPrintFormat=="(m)-e-costheta-phi(rad)")&& // lepton collider
	     (partonObjectsList[i][j].getFourVector().e()>10)&&
	     (fabs(cos(partonObjectsList[i][j].getFourVector().theta()))<0.9)) ||
	    ((objectPrintFormat=="(m)-pt-eta-phi(deg)")&& // lepton collider
	     (partonObjectsList[i][j].getFourVector().perp()>15)&&
	     (fabs(Math::theta2eta(partonObjectsList[i][j].getFourVector().theta()))<2.0))
	    ) &&
	 (recoObjectsList[i].empty()))
	{
	  partonObjectsList.clear();
	  recoObjectsList.clear();
	  break;
	}
      }
  for(size_t i=0; i<partonObjectsList.size(); i++)
    {
      double sumEparton = 0;
      for(size_t j=0; j<partonObjectsList[i].size(); j++)
	if(((partonObjectsList[i][j].getObjectType()=="b")||
	    (partonObjectsList[i][j].getObjectType()=="j")))
	  sumEparton += partonObjectsList[i][j].getFourVector().e();
	else
	  {
	    sumEparton = 0;
	    break;
	  }
      double sumEreco = 0;
      for(size_t j=0; j<recoObjectsList[i].size(); j++)
	if(((recoObjectsList[i][j].getObjectType()=="b")||
	    (recoObjectsList[i][j].getObjectType()=="j")))
	  sumEreco += recoObjectsList[i][j].getFourVector().e();
	else
	  {
	    sumEreco = 0;
	    break;
	  }
      if((sumEparton>10)&&(sumEreco>10)&&
	 (fabs(sumEparton-sumEreco)>10)&&
	 (fabs(sumEparton-sumEreco)>2.00*sqrt((sumEparton+sumEreco)/2)))
	{
	  partonObjectsList.clear();
	  recoObjectsList.clear();
	}
    }

  // Figure out where the unclustered energy goes, in a modestly sophisticated manner 
  // --------------------------------------------------------------------------------
  bool sophisticatedUnclusteredTreatment = false;
  if(sophisticatedUnclusteredTreatment)
    {
      vector<string> objectTypes;
      vector<double> lhs_px, lhs_py, rhs_px, rhs_py, x, y;
      for(size_t i=0; i<partonObjectsList.size(); i++)
	{
	  CLHEP::HepLorentzVector lhs, rhs, lhs_mu, rhs_mu, lhs_uncl, lhs_btau;
	  for(size_t j=0; j<partonObjectsList[i].size(); j++)
	    {
	      lhs += partonObjectsList[i][j].getFourVector();
	      if((partonObjectsList[i][j].getObjectType()=="b")||
		 (partonObjectsList[i][j].getObjectTypeSansSign()=="tau"))
		lhs_btau += partonObjectsList[i][j].getFourVector();
	      if(partonObjectsList[i][j].getObjectType()=="uncl")
		lhs_uncl += partonObjectsList[i][j].getFourVector();
	      if(partonObjectsList[i][j].getObjectTypeSansSign()=="mu")
		lhs_mu += partonObjectsList[i][j].getFourVector();
	    }
	  if(lhs_uncl.perp()>=lhs.perp()/2)
	    objectTypes.push_back("uncl");
	  else if(lhs_btau.perp()>lhs.perp()/2)
	    objectTypes.push_back("btau");
	  else if(lhs_mu.perp()>lhs.perp()/2)
	    objectTypes.push_back("mu");
	  else
	    objectTypes.push_back("j");
	  for(size_t j=0; j<recoObjectsList[i].size(); j++)
	    {
	      rhs += recoObjectsList[i][j].getFourVector();
	      if(recoObjectsList[i][j].getObjectTypeSansSign()=="mu")	    
		rhs_mu +=  recoObjectsList[i][j].getFourVector();
	    }
	  if((lhs_mu.perp()>1)&&(rhs_mu.perp()<1))
	    {
	      lhs = rhs = CLHEP::HepLorentzVector(0,0,0,0);
	    }
	  lhs_px.push_back(lhs.px());
	  lhs_py.push_back(lhs.py());
	  rhs_px.push_back(rhs.px());
	  rhs_py.push_back(rhs.py());
	  x.push_back(lhs_px.back()-rhs_px.back()+0.5);
	  y.push_back(lhs_py.back()-rhs_py.back()+0.5);
	}
      if(partonObjectsList.size()>=1)
	{
	  UnclLikelihood unclLikelihood_x(objectTypes, lhs_px, rhs_px);
	  UnclLikelihood unclLikelihood_y(objectTypes, lhs_py, rhs_py);
	  double chiSqd_x = Math::minimize(x,&unclLikelihood_x);
	  double chiSqd_y = Math::minimize(y,&unclLikelihood_y);
	  for(size_t i=0; i<partonObjectsList.size(); i++)
	    {
	      CLHEP::HepLorentzVector v = CLHEP::HepLorentzVector(Math::addInQuadrature(x[i],y[i]),CLHEP::Hep3Vector(x[i],y[i],0));
	      recoObjectsList[i].push_back(QuaeroRecoObject("uncl",v));
	    }
	  double chiSqd = Math::addInQuadrature(chiSqd_x,chiSqd_y);
	  if(chiSqd>3) // not a decent fit
	    {
	      cout << Math::nice(chiSqd) << " " << flush;
	      for(size_t j=0; j<partonObjectsList.size(); j++)
		{
		  for(size_t k=0; k<partonObjectsList[j].size(); k++)
		    cout << "  " << partonObjectsList[j][k].print("(m)-pt-eta-phi(deg)",0.1) << flush;
		  cout << " ;   -> ";
		  for(size_t k=0; k<recoObjectsList[j].size(); k++)
		    cout << "  " << recoObjectsList[j][k].print("(m)-pt-eta-phi(deg)",0.1) << flush;
		  cout << " ; " << endl;
		}
	    }
	}
    }

  return;
}



