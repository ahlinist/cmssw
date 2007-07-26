// DQM Flag (serialization in Text/Binary stream)
// 
// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 06/01/07
// License: GPL

#ifndef DQM_FLAG_TXT_H
#define DQM_FLAG_TXT_H

#include <iosfwd>

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/export.hpp>

#include "interface/FlagTxt.h"
#include "interface/ClassIDBase.h"

class DQMFlagXML;

class DQMFlagTxt: public FlagTxt {
  public:
    DQMFlagTxt() {}
    DQMFlagTxt( const DQMFlagXML *poDQM_FLAGXML);

  protected:
    // Used in copying Tree
    inline virtual Clonable *clone() const {
      return new DQMFlagTxt( *this);
    }

    virtual Clonable *cloneXML() const;

    // Used in serialization to get class ID
    ClassIDBase::ID getID() const;

    // Enable children
    virtual int isChildValid( const FlagTxt *poCHILD_CANDIDATE) const;

  private:
    // Serialization
    friend class boost::serialization::access;

    template<class Archive>
      void serialize( Archive &roArchive, const unsigned int &rnVERSION) {
        roArchive & boost::serialization::base_object<FlagTxt>( *this);
      }
};

std::ostream &
  operator <<( std::ostream &roOut, const DQMFlagTxt &roFLAG);

BOOST_CLASS_EXPORT_GUID( DQMFlagTxt, "DQMFlagTxt")

#endif // DQM_FLAG_TXT_H
