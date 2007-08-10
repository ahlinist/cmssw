#ifndef ASCIISystemInterfaceParametersReader_HH_
#define ASCIISystemInterfaceParametersReader_HH_

#include <string>

#include "AbstractConfigFileParametersReader.hh"
#include "ICTypeDefs.hh"
#include "ASCIIReaderEngine.hh"

namespace HCAL_HLX
{

  class ConfigFileParameters;

  class ASCIIConfigFileParametersReader :
    public AbstractConfigFileParametersReader,
    private ASCIIReaderEngine
  {
  public:
    ASCIIConfigFileParametersReader(const std::string & aFileName);
    virtual ~ASCIIConfigFileParametersReader();
    
    virtual void Populate(ConfigFileParameters & aSysParams);

  private:
    ASCIIConfigFileParametersReader();
    ASCIIConfigFileParametersReader(const ASCIIConfigFileParametersReader &);
    ASCIIConfigFileParametersReader & operator=(const ASCIIConfigFileParametersReader &);

  protected:
  };

}//~namespace HCAL_HLX

#endif//~ASCIIConfigFileParametersReader_HH_

