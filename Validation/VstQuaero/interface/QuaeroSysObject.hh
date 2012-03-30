
#ifndef __QuaeroSysObject__
#define __QuaeroSysObject__

#include "Validation/VstQuaeroUtils/interface/QuaeroRecoObject.hh"


class QuaeroSysObject: public QuaeroRecoObject
{
public:
  QuaeroSysObject(std::string objectType = "", const CLHEP::HepLorentzVector& p = CLHEP::HepLorentzVector(), double idQuality = 0): QuaeroRecoObject(objectType, p, idQuality) {};
  /// Read this object from an input stream, in specified format
  virtual bool read(std::istream& is, const std::string& format);

private:
  virtual double parseSystematicallyAffectedNumber(const std::string& blah);
  virtual std::pair<std::string,double> parseSystematicallyAffectedIdentity(const std::string& blah);
};


#endif
