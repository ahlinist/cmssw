
#ifndef __QuaeroParticleMorphismsDouble
#define __QuaeroParticleMorphismsDouble

#include <vector>
#include <string>
#include "Validation/VstMathUtils/interface/Math.hh"
#include "Validation/VstTurboSim/interface/MultiDimensionalBinaryForest.hh"
#include "Validation/VstQuaeroUtils/interface/QuaeroParticle.hh"

template<class T1, class T2>
class QuaeroParticleMorphismsDouble
{
  struct Node
  {
    T1 key1, key2;
    std::vector<T2> particles;
    std::string comment;
  };

public:
   QuaeroParticleMorphismsDouble();
   ~QuaeroParticleMorphismsDouble() {
     //cout << "MorphismsDouble destructor called. " << endl;
      clear();
     //cout << "MorphismsDouble destructor finished. " << endl;
   }

  void Add(const T1& key1, const T1& key2, const vector<T2>& value, double zvtx, std::string comment="");
  vector<T2> findClosest(const T1& key1, const T1& key2, double zvtx, std::string& comment);
  void clear();

private:
  std::vector<std::string> getTreeName(const T1& particle1, const T2& particle2);
  std::vector<double> getKey(const T1& particle1, const T1& particle2, double zvtx);
  void shimmy(vector<T2>& particles, const T1& targetKey1, const T1& targetKey2, const T1& closestKey1, const T1& closestKey2, double zvtx);

  MultiDimensionalBinaryForest<std::vector<std::string>, double, Node> forest;

};

#include "Validation/VstTurboSim/interface/QuaeroParticleMorphismsDouble.ii"

#endif

