// Trigger Flag (serialization in Text/Binary stream)
// 
// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/27/07
// License: GPL

#ifndef TRIGGER_FLAG_TXT_H
#define TRIGGER_FLAG_TXT_H

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/export.hpp>

#include "CalibTracker/SiStripRunSummary/interface/FlagTxt.h"
#include "CalibTracker/SiStripRunSummary/interface/ClassIDBase.h"

class TriggerFlagXML;

class TriggerFlagTxt: public FlagTxt {
  public:
    TriggerFlagTxt() {}
    TriggerFlagTxt( const TriggerFlagXML *poTRIGGER_FLAGXML);

  protected:
    // Used in copying Tree
    inline virtual Clonable *clone() const {
      return new TriggerFlagTxt( *this);
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
BOOST_CLASS_EXPORT_GUID( TriggerFlagTxt, "TriggerFlagTxt")

#endif // TRIGGER_FLAG_TXT_H
