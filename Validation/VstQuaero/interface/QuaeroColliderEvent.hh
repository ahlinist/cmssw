
#ifndef __QuaeroColliderEvent__
#define __QuaeroColliderEvent__

#include <vector>
#include <string>
#include <fstream>
#include "Validation/VstQuaero/interface/QuaeroItem.hh"


template<class T>
class QuaeroColliderEvent : public QuaeroItem<T>
{

template<class T1> friend std::istream &operator>>(std::istream&, QuaeroColliderEvent<T1> &);
template<class T1> friend std::ostream &operator<<(std::ostream&, const QuaeroColliderEvent<T1> &);

public:

  // Constructors
  QuaeroColliderEvent(std::string _eventType="", std::string _runNumber="0", double _wt=0, const std::vector<T>& _objects = std::vector<T>(0), double _rootS=0, double _zVtx=0, std::string _fullString="");
 
  // Accessors
  std::vector<T> getObjects() const;
  int numberOfObjects(double Emin=-1, double maxAbsEtaTheta=-1) const;
  int numberOfObjects(std::string objectType, double Emin=-1, double maxAbsEtaTheta=-1) const;
  std::string getEventType() const;
  double getRootS() const;
  double getZVtx() const;
  std::string getRunNumber() const;
  bool hadronMachine() const;
  bool leptonMachine() const;
  HepLorentzVector getPmiss() const;
  T* getThisObject(std::string objectType, int orderNumber=1);
  const T* getThisObject(std::string objectType, int orderNumber=1) const;
  bool approximatelyEqualTo(const QuaeroColliderEvent& rhs, double tol=0.1);

  // Modifiers
  void reVertex(double _zVtx);

  // Methods
  std::string print() const;
  bool operator==(const QuaeroColliderEvent & rhs) const;

private:
  double rootS;
  double zVtx;
  std::string fullString;
  void mergeObjectsOfThisType(std::string objectType);
};

#include "Validation/VstQuaero/interface/QuaeroColliderEvent.ii"

#endif
