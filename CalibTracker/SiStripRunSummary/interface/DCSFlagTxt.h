// DCS Flag (serialization in Text/Binary stream)
// 
// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/27/07
// License: GPL

#ifndef DCS_FLAG_TXT_H
#define DCS_FLAG_TXT_H

#include <iosfwd>

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/export.hpp>

#include "CalibTracker/SiStripRunSummary/interface/FlagTxt.h"
#include "CalibTracker/SiStripRunSummary/interface/ClassIDBase.h"

class DCSFlagXML;

class DCSFlagTxt: public FlagTxt {
  public:
    DCSFlagTxt() {}
    DCSFlagTxt( const DCSFlagXML *poDCS_FLAGXML);

  protected:
    // Used in copying Tree
    inline virtual Clonable *clone() const {
      return new DCSFlagTxt( *this);
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
  operator <<( std::ostream &roOut, const DCSFlagTxt &roFLAG);

BOOST_CLASS_EXPORT_GUID( DCSFlagTxt, "DCSFlagTxt")

#endif // DCS_FLAG_TXT_H
