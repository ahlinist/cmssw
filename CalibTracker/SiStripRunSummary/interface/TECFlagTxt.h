// TEC Flag (serialization in Text/Binary stream)
// 
// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/27/07
// License: GPL

#ifndef TEC_FLAG_TXT_H
#define TEC_FLAG_TXT_H

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/export.hpp>

#include "CalibTracker/SiStripRunSummary/interface/FlagTxt.h"
#include "CalibTracker/SiStripRunSummary/interface/ClassIDBase.h"

class TECFlagXML;

class TECFlagTxt: public FlagTxt {
  public:
    TECFlagTxt() {}
    TECFlagTxt( const TECFlagXML *poTEC_FLAGXML);

  protected:
    // Used in copying Tree
    inline virtual Clonable *clone() const {
      return new TECFlagTxt( *this);
    }

    virtual Clonable *cloneXML() const;

    // Used in serialization to get class ID
    virtual ClassIDBase::ID getID() const;

  private:
    // Serialization
    friend class boost::serialization::access;

    template<class Archive>
      void serialize( Archive &roArchive, const unsigned int &rnVERSION) {
        roArchive & boost::serialization::base_object<FlagTxt>( *this);
      }
};

// Register class to enable serialization through pointer to base
BOOST_CLASS_EXPORT_GUID( TECFlagTxt, "TECFlagTxt")

#endif // TEC_FLAG_TXT_H
