#include <string>
#include <TString.h>
#include <TRegexp.h>

#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "DataFormats/PatCandidates/interface/TriggerPath.h"

namespace vgamma {
  

  // ask if string p is contained in path
  bool 
    matchesPath(const std::string& p,
		const pat::TriggerPath& path)
  {
    return  TString(path.name()).Contains(TRegexp(p));
  } //matchesPath

  // prescale == 0 means return true if the path has a
  // exists and has fired with any prescale.
  bool 
    path(const std::string& p, 
	 const pat::TriggerEvent& te,
	 const unsigned& prescale = 1,
	 const bool& needsFire = true)
  {
    for(pat::TriggerPathCollection::const_iterator i = 
	  te.paths()->begin();
	i != te.paths()->end();
	++i) {
      if(matchesPath(p,*i) && 
	 (i->wasAccept() || !needsFire)&& 
	 (i->prescale() == prescale ||
	  prescale == 0) )
	return true;
    }
    return false;
  } //path

  // return the first index of path p with prescale  
  int
    pathIndex(const std::string& p, 
	      const pat::TriggerEvent& te,
	      const unsigned& prescale = 1)
  {
    for(pat::TriggerPathCollection::const_iterator i = 
	  te.paths()->begin();
	i != te.paths()->end();
	++i) {
      if(matchesPath(p,*i) && 
	 (i->prescale() == prescale ||
	  prescale == 0) )
	return i->index();
    }
    return -1;
  } //pathIndex

}//ns:vgamma
