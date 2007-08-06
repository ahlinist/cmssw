// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/27/07
// License: GPL

#ifndef GETFLAGTREE_H
#define GETFLAGTREE_H

#include "CalibTracker/SiStripRunSummary/interface/GlobalFlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/GlobalFlagTxt.h"

/**
* @brief
*   Construct Tree of flags: TXT format. Used for testing in main_xxx.cc
*/
GlobalFlagTxt getTXTFlagTree();

/**
* @brief
*   Construct Tree of flags: XML format. Used for testing in main_xxx.cc
*/
GlobalFlagXML getXMLFlagTree();

#endif // GETFLAGTREE_H
