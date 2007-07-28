// Trigger Flag (serialization in XML stream)
// 
// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/27/07
// License: GPL

#ifndef TRIGGER_FLAG_XML_H
#define TRIGGER_FLAG_XML_H

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/export.hpp>

#include "CalibTracker/SiStripRunSummary/interface/FlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/ClassIDBase.h"

class TriggerFlagTxt;

class TriggerFlagXML: public FlagXML {
  public:
    TriggerFlagXML() {}
    TriggerFlagXML( const TriggerFlagTxt *poTRIGGER_FLAGTXT);

  protected:
    // Used in copying Tree
    inline virtual Clonable *clone() const {
      return new TriggerFlagXML( *this);
    }

    virtual Clonable *cloneTxt() const;

    // Used in serialization to get class ID
    virtual ClassIDBase::ID getID() const;

  private:
    // Serialization
    friend class boost::serialization::access;

    template<class Archive>
      void serialize( Archive &roArchive, const unsigned int &rnVERSION) {
        roArchive & BOOST_SERIALIZATION_BASE_OBJECT_NVP( FlagXML);
      }
};

// Register class to enable serialization through pointer to base
BOOST_CLASS_EXPORT_GUID( TriggerFlagXML, "TriggerFlagXML")

#endif // TRIGGER_FLAG_XML_H
