// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/27/07
// License: GPL

#ifndef GETFLAGTREE_H
#define GETFLAGTREE_H

#include "CalibTracker/SiStripRunSummary/interface/GlobalFlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/GlobalFlagTxt.h"

GlobalFlagTxt getTXTFlagTree();
GlobalFlagXML getXMLFlagTree();

#endif // GETFLAGTREE_H
