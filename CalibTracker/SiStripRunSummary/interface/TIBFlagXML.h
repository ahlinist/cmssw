// TIB Flag (serialization in XML stream)
// 
// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 06/01/07
// License: GPL

#ifndef TIB_FLAG_XML_H
#define TIB_FLAG_XML_H

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/export.hpp>

#include "CalibTracker/SiStripRunSummary/interface/FlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/ClassID.h"

class TIBFlagTxt;

class TIBFlagXML: public FlagXML {
  public:
    TIBFlagXML() {}
    TIBFlagXML( const TIBFlagTxt *poTIB_FLAGTXT);

    // Used in copying Tree
    inline virtual Clonable *clone() const {
      return new TIBFlagXML( *this);
    }

    virtual Clonable *cloneTxt() const;

  protected:
    // Used in serialization to get class ID
    inline virtual ClassIDBase::ID getID() const {
      return ClassID<TIBFlagXML>::get();
    }

  private:
    // Serialization
    friend class boost::serialization::access;

    template<class Archive>
      void serialize( Archive &roArchive, const unsigned int &rnVERSION) {
        roArchive & BOOST_SERIALIZATION_BASE_OBJECT_NVP( FlagXML);
      }
};

// Register class to enable serialization through pointer to base
BOOST_CLASS_EXPORT_GUID( TIBFlagXML, "TIBFlagXML")

#endif // TIB_FLAG_XML_H
