// Dinamic issue of Classes IDs. Each class has unique ID that represent
// integer value.
//
// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 05/25/07
// License: GPL

#ifndef CLASS_ID_H
#define CLASS_ID_H

#include "CalibTracker/SiStripRunSummary/interface/ClassIDBase.h"

/** 
* @brief 
*   Manager that issues ID to any class [registration]. Implemented as a 
*   template in order to achieve desired effect: once Manager is called to
*   issue ID for some class compiler will generate appropriate class.
*/
template<class T>
  class ClassID: public ClassIDBase {
    public:
      /** 
      * @brief  Get Class ID [all classes (same ones) will have the same ID]
      *         [Note: issued ID is runtime dependent - first to call 
      *                registration will first to get id. For example map
      *                of objects where keys are IDs can not be saved and
      *                later read with assumption that classes stored will
      *                have the same IDs except situations when classes are
      *                registered in exactly the same sequence]
      * 
      * @return ClassIDBase::ID  that represent ID
      */
      static ID get();

    private:
      // Registered class ID
      static ID nID_;
  };

// Initialize static variable
template<class T>
  ClassIDBase::ID ClassID<T>::nID_ = ClassID<T>::nINIT_CLASS_ID_;

template<class T>
  int ClassID<T>::get() {
    // Check if class was registered
    if( nINIT_CLASS_ID_ == nID_) {
      // Class is not Registered: register it 
      nID_ = ++nLastClassID_;
    }

    return nID_;
  }

#endif // CLASS_ID_H
