#ifndef DQMEVF_PROCUTILS_H
#define DQMEVF_PROCUTILS_H

#include <string>

namespace dqmevf{
  namespace utils{
    void procStat(std::ostringstream *);
    void uptime(std::ostringstream *);
    void mDiv(std::ostringstream *out, std::string name);
    void cDiv(std::ostringstream *out, bool doendl = false);
    void mDiv(std::ostringstream *out, std::string name, std::string value);
    void mDiv(std::ostringstream *out, std::string name, unsigned int value);
	void mTd(std::ostringstream *out, std::string name, std::string id);
    void cTd(std::ostringstream *out, bool doendl = false);
    void mTd(std::ostringstream *out, std::string name, std::string value, std::string id);
    void mTd(std::ostringstream *out, std::string name, unsigned int value, std::string id);
  }
}
#endif
