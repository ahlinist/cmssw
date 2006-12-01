#ifndef Framework_BranchActionType_h
#define Framework_BranchActionType_h

/*----------------------------------------------------------------------
  
BranchActionType: BranchAction

$Id$
----------------------------------------------------------------------*/

namespace edm {
  // Note: These enum values are used as subscripts for a fixed size array, so they must not change.
  enum BranchActionType {
    BranchActionEvent = 0,
    BranchActionBeginLumi = 1,
    BranchActionEndLumi = 2,
    BranchActionBeginRun = 3,
    BranchActionEndRun = 4,
    EndBranchActionType = 5
  };
}
#endif
