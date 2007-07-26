// Flag Base container:
//    1. State
//    2. Comment
//
// Author : Samvel Khalatian (ksamdev at gmail dot com)
// Created: 05/31/07
// License: GPL

#ifndef FLAG_H
#define FLAG_H

#include <iosfwd>
#include <string>

#include "interface/Clonable.h"
#include "interface/Identifiable.h"

class Flag: public Clonable,
            public Identifiable {

  public:
    enum State { UNKNOWN = 0, ERROR = 1, OK = 2 };

    Flag(): eState_( UNKNOWN), oComment_( "") {}
    virtual ~Flag() {}

    virtual bool setState  ( const State &reSTATE);
    inline  void setComment( const std::string &roCOMMENT) {
      oComment_ = roCOMMENT;
    }

    inline State       getState  () const { return eState_; }
    inline std::string getComment() const { return oComment_; }

  protected:
    // Allow only children to copy Flags via clone method
    Flag( const Flag &roFLAG)
      : eState_  ( roFLAG.eState_),
        oComment_( roFLAG.oComment_) {}

    // Unfortunately all properties of Flag (that are going to be serialized)
    // have to be protected. Otherwise Serialization won't work :(.
    State       eState_;
    std::string oComment_;

  private:
    // Prevent copying Flags via assignment
    Flag &operator =( const Flag &rnFLAG);
};

std::ostream &operator <<( std::ostream &roOut,
                           const Flag &roFLAG);

#endif // FLAG_H
