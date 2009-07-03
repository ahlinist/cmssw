// $Id: Stopped.cc,v 1.3 2009/07/02 13:46:38 dshpakov Exp $

#include "EventFilter/StorageManager/interface/Notifier.h"
#include "EventFilter/StorageManager/interface/StateMachine.h"

#include <iostream>

#include "xcept/tools.h"

using namespace std;
using namespace stor;

Stopped::Stopped( my_context c ): my_base(c)
{
  safeEntryAction( outermost_context().getNotifier() );
}

void Stopped::do_entryActionWork()
{
  TransitionRecord tr( stateName(), true );
  outermost_context().updateHistory( tr );
  outermost_context().setExternallyVisibleState( "Ready" );
  outermost_context().getNotifier()->reportNewState( "Ready" );
}

Stopped::~Stopped()
{
  safeExitAction( outermost_context().getNotifier() );
}

void Stopped::do_exitActionWork()
{
  TransitionRecord tr( stateName(), false );
  outermost_context().updateHistory( tr );
}

string Stopped::do_stateName() const
{
  return string( "Stopped" );
}



/// emacs configuration
/// Local Variables: -
/// mode: c++ -
/// c-basic-offset: 2 -
/// indent-tabs-mode: nil -
/// End: -
