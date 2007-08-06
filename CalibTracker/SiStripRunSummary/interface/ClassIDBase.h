// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 05/31/07
// License: GPL

#ifndef CLASS_ID_BASE_H
#define CLASS_ID_BASE_H

/** 
* @brief 
*   Every ClassID has "static" base that holds ID of last registered class
*/
class ClassIDBase {
  public:
    /** 
    * @brief 
    *   Used by Identifiable child as a return type.
    */
    typedef int ID;

  private:
    template<class T> friend class ClassID;

    static const ID nINIT_CLASS_ID_ = -1;
    static       ID nLastClassID_;
};

#endif // CLASS_ID_BASE_H
