#ifndef __TurboSimMorphisms
#define __TurboSimMorphisms

#include <vector>
#include <string>
#include "Validation/VstQuaeroUtils/interface/QuaeroRecoObject.hh"
#include "Validation/VstQuaeroUtils/interface/QuaeroEvent.hh"
#include "Validation/VstTurboSim/interface/QuaeroPartonObject.hh"
#include "Validation/VstTurboSim/interface/QuaeroPartonEvent.hh"
#include "Validation/VstTurboSim/interface/QuaeroParticleMorphismsSingle.hh"
#include "Validation/VstTurboSim/interface/QuaeroParticleMorphismsDouble.hh"
#include "Validation/VstTurboSim/interface/QuaeroParticleMorphismsTriple.hh"

enum AngleMeasure { DELTA_R, DELTA_ANGLE, MANHATTAN_DISTANCE, KT_DURHAM, DEFAULT };

template<class T>
vector<string> getTag(vector<T> objects);

template <class T>
bool chargeConjugationRequiredQ(const vector<T>& objects);

template<class T>
void chargeConjugate(vector<T>& objects);

bool stickParticleObject(string objectTypeSansSign);

#include "Validation/VstTurboSim/interface/TurboSimMorphisms.ii"

#endif

