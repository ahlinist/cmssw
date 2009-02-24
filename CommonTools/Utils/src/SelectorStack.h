#ifndef CommonTools_Utils_SelectorStack_h
#define CommonTools_Utils_SelectorStack_h
/* \class reco::parser::SelectorPtr
 *
 * Stack of selectors
 *
 * \author original version: Chris Jones, Cornell, 
 *         adapted to Reflex by Luca Lista, INFN
 *
 * \version $Revision: 1.1 $
 *
 */
#include <vector>
#include "CommonTools/Utils/src/SelectorPtr.h"

namespace reco {
  namespace parser {
    typedef std::vector<SelectorPtr> SelectorStack;
  }
}

#endif
