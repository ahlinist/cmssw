// Identifiable interface
//
// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 05/31/07
// License: GPL

#ifndef IDENTIFIABLE_H
#define IDENTIFIABLE_H

#include "interface/ClassIDBase.h"

// Interface for identifiable objects: objects that can return ID
class Identifiable {
  public:
    virtual ~Identifiable() {}

    virtual ClassIDBase::ID getID() const = 0;
};

#endif // IDENTIFIABLE_H
