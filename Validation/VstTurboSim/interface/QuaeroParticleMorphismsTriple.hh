
#ifndef __QuaeroParticleMorphismsTriple
#define __QuaeroParticleMorphismsTriple

#include <vector>
#include <string>
#include "Validation/VstMathUtils/interface/Math.hh"
#include "Validation/VstTurboSim/interface/MultiDimensionalBinaryForest.hh"
#include "Validation/VstQuaeroUtils/interface/QuaeroParticle.hh"

template<class T1, class T2>
class QuaeroParticleMorphismsTriple
{
  struct Node
  {
    std::vector<T1> key;
    std::vector<T2> particles;    
    std::string comment;
  };

public:
  QuaeroParticleMorphismsTriple();
  void Add(const std::vector<T1>& key, const std::vector<T2>& value, double zvtx, std::string comment="");
  std::vector<T2> findClosest(const std::vector<T1>& key, double zvtx, std::string& comment);
  void clear();

private:
  std::vector<std::string> getTreeName(const std::vector<T1>& particles);
  std::vector<double> getKey(const std::vector<T1>& particles, double zvtx);
  void shimmy(std::vector<T2>& particles, const std::vector<T1>& targetKeys, const std::vector<T1>& closestKeys, double zvtx);

  MultiDimensionalBinaryForest<std::vector<std::string>, double, Node> forest;

};

#include "Validation/VstTurboSim/interface/QuaeroParticleMorphismsTriple.ii"

#endif

