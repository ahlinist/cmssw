#ifndef AbstractConfigFileParametersReader_HH_
#define AbstractConfigFileParametersReader_HH_

namespace HCAL_HLX
{
  class ConfigFileParameters;

  class AbstractConfigFileParametersReader {

  public:
    AbstractConfigFileParametersReader(){}
    virtual ~AbstractConfigFileParametersReader(){}
    
    virtual void Populate(ConfigFileParameters & aConfigParams)=0;

  }; //~class AbstractConfigFileParametersReader

} //~namespace HCAL_HLX

#endif //~AbstractConfigFileParametersReader_HH_
