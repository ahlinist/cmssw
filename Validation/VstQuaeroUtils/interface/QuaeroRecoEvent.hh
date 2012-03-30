/****************************
QuaeroRecoEvent represents the high-level reconstructed debris from a high energy collision.
Bruce Knuteson 2003
****************************/

#ifndef __QuaeroRecoEvent__
#define __QuaeroRecoEvent__

#include <vector>
#include <string>
#include <fstream>
#include "Validation/VstQuaeroUtils/interface/QuaeroItem.hh"

template<class T>
class QuaeroRecoEvent : public QuaeroItem<T>
{
  /// Support streaming output
  template<class T1> friend std::istream &operator>>(std::istream&, QuaeroRecoEvent<T1> &);
  /// Support streaming input
  template<class T1> friend std::ostream &operator<<(std::ostream&, const QuaeroRecoEvent<T1> &);

public:
  
  /*****  Constructor  *****/

  /// Input arguments map one-to-one to private members
  QuaeroRecoEvent(std::string _eventType="", 
		  std::string _runNumber="0", 
		  double _wt=0, 
		  const std::vector<T>& _objects = std::vector<T>(0), 
                  std::string _collisionType="",
		  double _rootS=0, 
		  double _zVtx=0, 
		  double _generatedSumPt=0,
		  std::string _fullString="",
		  bool keepAllObjects = false); // bktemp -- "true" for Vista@CdfRunII
 
  /*****  Accessors  *****/

  std::string getFullString() { return(fullString); }

  /// Return all reconstructed objects in the event
  std::vector<T> getObjects() const;

  /// Return the number of objects with energy exceeding Emin and more central than maxAbsEtaTheta
  int numberOfObjects(double Emin=-1, double maxAbsEtaTheta=-1) const;

  /// Return the number of objects of type objectType with energy exceeding Emin and more central than maxAbsEtaTheta
  /// Specifying a lepton without the charge returns the number of that lepton independent of charge;
  /// thus numberOfObjects("e") == numberOfObjects("e+") + numberOfObjects("e-")
  int numberOfObjects(std::string objectType, double Emin=-1, double maxAbsEtaTheta=-1) const;

  /// Return the event type
  std::string getEventType() const;

  /// Return the center of mass energy of the event
  /// This is the collision energy of the machine (e.g., 1960 for Tevatron Run II), and _not_ the parton-parton collision energy
  /// Units are GeV
  std::string getCollisionType() const;
  double getRootS() const;

  /// Return the z position of the primary vertex
  /// Units are centimeters
  double getZVtx() const;

  /// Return the run number of the event
  /// Usually "runNumber" here is in the form runNumber.eventNumber
  std::string getRunNumber() const;

  /// Return the generated summed transverse momentum
  double getGeneratedSumPt() const;

  /// Did this event occur in a hadron machine?
  bool hadronMachine() const;

  /// Or did this event occur in a lepton machine?
  bool leptonMachine() const;

  /// At which collider did this event occur?
  std::string collider() const;

  /// Return the <orderNumber>^th object of type <objectType>
  T* getThisObject(std::string objectType, int orderNumber=1);

  /// Return the <orderNumber>^th object of type <objectType>
  const T* getThisObject(std::string objectType, int orderNumber=1) const;

  /*****  Methods  *****/

  /// Check for approximate equality to another QuaeroRecoEvent
  bool approximatelyEqualTo(const QuaeroRecoEvent& rhs, double tol=0.1);

  /// Check for equality to another QuaeroRecoEvent
  bool operator==(const QuaeroRecoEvent & rhs) const;

  /// Calculate sumPt of all objects in event
  double sumPt() const; 

  /// Return the missing energy in the event
  CLHEP::HepLorentzVector getPmiss() const;

  /// Collapse this object to a string
  std::string print() const;
  
  /*****  Modifiers  *****/
  
  /// Change the position of the primary vertex
  void reVertex(double _zVtx); 
  
  /// Change the objects in the event
  void setObjects(const std::vector<T>& _objects = std::vector<T>(0), bool keepAllObjects = false);


protected:
  
  /*****  Methods  *****/

  /// Parse a string that contains a number and (possibly) systematic errors that affect that number
  /// If affected by systematic errors, impose the current systematic shifts and obtain a number
  virtual double parseSystematicallyAffectedNumber(const std::string& wtString) const;

  /// Merge all of objects of specified type
  void mergeObjectsOfThisType(std::string objectType);

  /*****  Static members (in spirit) *****/

  /// Smallest transverse momentum an object can have at a hadron machine and still be worth counting
  double smallestObjectPtHadronMachine; // units are GeV
  
  /// Smallest energy an object can have at a lepton machine and still be worth counting
  double smallestObjectEnergyLeptonMachine; // units are GeV

  /*****  Non-Static members  *****/

  std::string collisionType; ///< colliding particles (e+e-, e+p, e-p, ppbar, pp, auau, etc.)
  double rootS; ///< center of mass energy of collision; units are GeV
  double zVtx; ///< z position of primary vertex; units are cm
  double generatedSumPt; ///< summed transverse momentum of particles at generator level
  std::string fullString; ///< a full string specifying the event, not usually used
};

#include "Validation/VstQuaeroUtils/interface/QuaeroRecoEvent.ii"

#endif
