// TIB Flag (serialization in Text/Binary stream)
// 
// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 06/01/07
// License: GPL

#ifndef TIB_FLAG_TXT_H
#define TIB_FLAG_TXT_H

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/export.hpp>

#include "CalibTracker/SiStripRunSummary/interface/FlagTxt.h"
#include "CalibTracker/SiStripRunSummary/interface/ClassID.h"

class TIBFlagXML;

class TIBFlagTxt: public FlagTxt {
  public:
    TIBFlagTxt() {}
    TIBFlagTxt( const TIBFlagXML *poTIB_FLAGXML);

    // Used in copying Tree
    inline virtual Clonable *clone() const {
      return new TIBFlagTxt( *this);
    }

    virtual Clonable *cloneXML() const;

  protected:
    // Used in serialization to get class ID
    inline virtual ClassIDBase::ID getID() const {
      return ClassID<TIBFlagTxt>::get();
    }

  private:
    // Serialization
    friend class boost::serialization::access;

    template<class Archive>
      void serialize( Archive &roArchive, const unsigned int &rnVERSION) {
        roArchive & boost::serialization::base_object<FlagTxt>( *this);
      }
};

// Register class to enable serialization through pointer to base
BOOST_CLASS_EXPORT_GUID( TIBFlagTxt, "TIBFlagTxt")

#endif // TIB_FLAG_TXT_H
