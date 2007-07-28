// TID Flag (serialization in XML stream)
// 
// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/27/07
// License: GPL

#ifndef TID_FLAG_XML_H
#define TID_FLAG_XML_H

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/export.hpp>

#include "CalibTracker/SiStripRunSummary/interface/FlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/ClassIDBase.h"

class TIDFlagTxt;

class TIDFlagXML: public FlagXML {
  public:
    TIDFlagXML() {}
    TIDFlagXML( const TIDFlagTxt *poTID_FLAGTXT);

  protected:
    // Used in copying Tree
    inline virtual Clonable *clone() const {
      return new TIDFlagXML( *this);
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
BOOST_CLASS_EXPORT_GUID( TIDFlagXML, "TIDFlagXML")

#endif // TID_FLAG_XML_H
