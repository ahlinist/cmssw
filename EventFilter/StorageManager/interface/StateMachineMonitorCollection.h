// $Id: StateMachineMonitorCollection.h,v 1.2 2009/06/10 08:15:24 dshpakov Exp $

#ifndef StorageManager_StateMachineMonitorCollection_h
#define StorageManager_StateMachineMonitorCollection_h

#include <ostream>
#include <string>

#include "xdata/String.h"

#include "EventFilter/StorageManager/interface/MonitorCollection.h"
#include "EventFilter/StorageManager/interface/TransitionRecord.h"


namespace stor {

  /**
   * A collection of monitored quantities related to the state machine
   *
   * $Author: dshpakov $
   * $Revision: 1.2 $
   * $Date: 2009/06/10 08:15:24 $
   */
  
  class StateMachineMonitorCollection : public MonitorCollection
  {

  public:

    StateMachineMonitorCollection();

    /**
     * Add the TransitionRecord to the state machine history
     */
    void updateHistory(const TransitionRecord&);

    /**
     * Copy the state machine history in the given History vector
     */
    typedef std::vector<TransitionRecord> History;
    void getHistory(History&) const;

    /**
     * Dump the state machine history into the stream
     */
    void dumpHistory(std::ostream&) const;

    /**
     * Set the externally visible state name
     */
    void setExternallyVisibleState(const std::string&);

    /**
     * Retrieve the externally visible state name
     */
    const std::string& externallyVisibleState() const;

    /**
     * Retrieve the current internal state name
     */
    const std::string& innerStateName() const;


  private:

    //Prevent copying of the StateMachineMonitorCollection
    StateMachineMonitorCollection(StateMachineMonitorCollection const&);
    StateMachineMonitorCollection& operator=(StateMachineMonitorCollection const&);

    virtual void do_calculateStatistics();
    virtual void do_reset();
    virtual void do_appendInfoSpaceItems(InfoSpaceItems&);
    virtual void do_updateInfoSpaceItems();

    History _history;
    std::string _externallyVisibleState;
    mutable boost::mutex _stateMutex;

    xdata::String _stateName;

  };
  
} // namespace stor

#endif // StorageManager_StateMachineMonitorCollection_h 


/// emacs configuration
/// Local Variables: -
/// mode: c++ -
/// c-basic-offset: 2 -
/// indent-tabs-mode: nil -
/// End: -
