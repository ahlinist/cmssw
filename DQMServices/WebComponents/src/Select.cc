#include "DQMServices/WebComponents/interface/Select.h"


void Select::printHTML(xgi::Output *out)
{
  *out << cgicc::select().set("name", name) << std::endl;
  std::list<std::string>::iterator it;
  for (it = options.begin(); it != options.end(); it++)
    { 
      *out << cgicc::option() << *it << cgicc::option() << std::endl;
    }
  *out << cgicc::select() << std::endl;
}
