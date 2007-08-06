// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 05/31/07
// License: GPL

#ifndef IDENTIFIABLE_H
#define IDENTIFIABLE_H

#include "CalibTracker/SiStripRunSummary/interface/ClassIDBase.h"

// Interface for identifiable objects: objects that can return ID
/** 
* @brief 
*   Interface for all objects that should be identifiable, aka have a function
*   that returns it's ID. Application example:
*
*     std::map<ID, Flag *> oFlags;
*     // Save Flags in file
*     ...
*     // Read Flags from file
*     // restore oFlags in memory. Problem: since operations are performed with
*     // pointers it is unknow what flag is pointed at currently and it's ID
*     // can not be obtained. Solution: use Identifiable interface. Then each
*     // Child flag can automatically return it's ID.
*/
class Identifiable {
  public:
    virtual ~Identifiable() {}

    /** 
    * @brief 
    *   Method should be overriden by children: each child flag knows how to 
    *   issue ID.
    * 
    * @return 
    *   ID of Child Flag.
    */
    virtual ClassIDBase::ID getID() const = 0;
};

#endif // IDENTIFIABLE_H
