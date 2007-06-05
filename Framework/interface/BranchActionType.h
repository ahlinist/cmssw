#ifndef Framework_BranchActionType_h
#define Framework_BranchActionType_h

/*----------------------------------------------------------------------
  
BranchActionType: BranchAction

$Id$
----------------------------------------------------------------------*/

namespace edm {
  enum BranchActionType {
    BranchActionEvent = 0,
    BranchActionBegin = 1,
    BranchActionEnd = 2,
    EndBranchActionType = 3
  };
}
#endif
