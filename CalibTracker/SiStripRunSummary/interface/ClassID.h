// Dinamic issue of Classes IDs. Each class has unique ID that represent
// integer value.
//
// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 05/25/07
// License: GPL

#ifndef CLASS_ID_H
#define CLASS_ID_H

#include "CalibTracker/SiStripRunSummary/interface/ClassIDBase.h"

template<class T>
  class ClassID: public ClassIDBase {
    public:
      /** 
      * @brief  Get Class ID [all classes will have the same ID]
      * 
      * @return  int that represent ID
      */
      static ID get();

    private:
      static ID nID_;
  };

// Initialize static variable
template<class T>
  ClassIDBase::ID ClassID<T>::nID_ = ClassID<T>::nINIT_CLASS_ID_;

template<class T>
  int ClassID<T>::get() {
    // Check if class was registered
    if( nINIT_CLASS_ID_ == nID_) {
      // Class is not Registered: Register it 
      nID_ = ++nLastClassID_;
    }

    return nID_;
  }

#endif // CLASS_ID_H
