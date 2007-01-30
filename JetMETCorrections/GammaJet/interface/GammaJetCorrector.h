#ifndef GammaJetCorrector_h
#define GammaJetCorrector_h

#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include <map>
#include <string>

namespace edm {
  class ParameterSet;
}

namespace {
class ParametrizationGammaJet;
}

///
/// jet energy corrections from GammaJet calibration
///

class GammaJetCorrector : public JetCorrector
{
public:  

  GammaJetCorrector(const edm::ParameterSet& fParameters);
                         
  virtual ~GammaJetCorrector();
  
  /// apply correction using Jet information only
  virtual double  correction (const LorentzVector& fJet) const;
  
  /// if correction needs event information
  virtual bool eventRequired () const {return false;}
   
private:
  
  typedef std::map<double,ParametrizationGammaJet *> ParametersMap;
  void setParameters(std::string );
  ParametersMap parametrization;
  
};

#endif
