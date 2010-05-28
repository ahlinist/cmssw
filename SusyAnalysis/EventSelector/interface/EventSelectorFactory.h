#ifndef SusyAnalysis_EventSelector_EventSelectorFactory_H
#define SusyAnalysis_EventSelector_EventSelectorFactory_H

/// Plugin factory for SUSY event selector modules.
///
/// Original author: W. Adam, 10/4/08
/// $Id: EventSelectorFactory.h,v 1.2 2008/05/23 15:48:21 fronga Exp $

#include "FWCore/PluginManager/interface/PluginFactory.h"
#include "SusyAnalysis/EventSelector/interface/SusyEventSelector.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

typedef edmplugin::PluginFactory<SusyEventSelector* (const edm::ParameterSet&)> EventSelectorFactory;

#endif
