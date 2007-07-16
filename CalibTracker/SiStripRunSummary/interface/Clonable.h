// Clonable interface
//
// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 05/31/07
// License: GPL

#ifndef CLONABLE_H
#define CLONABLE_H

// Interface for clonable objects
class Clonable {
  public:
    virtual ~Clonable() {}

    virtual Clonable *clone() const = 0;

    inline virtual Clonable *cloneTxt() const { return clone(); }
    inline virtual Clonable *cloneXML() const { return clone(); }
};

#endif // CLONABLE_H
