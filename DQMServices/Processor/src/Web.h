#include <string>
#include <iostream>
#include <sstream>

namespace dqmevf {

class Web {
  std::string JCLogURL_;
	public:
  void setJCLogURL(std::string prochost, std::string procpid);
  void importStaticJS(std::ostringstream *, std::string);
  void importStaticHTML(std::ostringstream *, std::string);

};

}
