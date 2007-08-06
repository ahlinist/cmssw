// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 05/31/07
// License: GPL

#ifndef CLONABLE_H
#define CLONABLE_H

/** 
* @brief
*   Interface for all clonable flags. It is used in:
*     1. Copying flags
*     2. Conversions FlagTxt <-> FlagXML
*/
class Clonable {
  public:
    virtual ~Clonable() {}

  protected:
    /** 
    * @brief 
    *   Method should be redefined by child Flag (each flag known how to clone 
    *   itself). It is used in copying flags. The idea is that objects should
    *   created in heap memory
    * 
    * @return
    *   Newly created object.
    */
    virtual Clonable *clone() const = 0;

    /** 
    * @brief 
    *   Method is used in coversion XML -> Txt. It is by default grounded to
    *   clone method. Should be redefined by child flag to perform conversion
    *   NewFlagXML -> NewFlagTxt.
    * 
    * @return 
    *   Newly created object
    */
    inline virtual Clonable *cloneTxt() const { return clone(); }
    
    /** 
    * @brief 
    *   Method is used in coversion Txt -> XML. It is by default grounded to
    *   clone method. Should be redefined by child flag to perform conversion
    *   NewFlagTxt -> NewFlagXML.
    * 
    * @return 
    *   Newly created object
    */
    inline virtual Clonable *cloneXML() const { return clone(); }
};

#endif // CLONABLE_H
