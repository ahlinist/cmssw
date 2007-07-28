// DCS Flag (serialization in XML stream)
// 
// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/27/07
// License: GPL

#ifndef DCS_FLAG_XML_H
#define DCS_FLAG_XML_H

#include <iosfwd>

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/export.hpp>

#include "CalibTracker/SiStripRunSummary/interface/FlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/ClassIDBase.h"

class DCSFlagTxt;

class DCSFlagXML: public FlagXML {
  public:
    DCSFlagXML() {}
    DCSFlagXML( const DCSFlagTxt *poDCS_FLAGTXT);

  protected:
    // Used in copying Tree
    inline virtual Clonable *clone() const {
      return new DCSFlagXML( *this);
    }

    virtual Clonable *cloneTxt() const;

    // Used in serialization to get class ID
    virtual ClassIDBase::ID getID() const;

    // Enable children
    virtual int isChildValid( const FlagXML *poCHILD_CANDIDATE) const;

  private:
    // Serialization
    friend class boost::serialization::access;

    template<class Archive>
      void serialize( Archive &roArchive, const unsigned int &rnVERSION) {
        roArchive & BOOST_SERIALIZATION_BASE_OBJECT_NVP( FlagXML);
      }
};

std::ostream &
  operator <<( std::ostream &roOut, const DCSFlagXML &roFLAG);

BOOST_CLASS_EXPORT_GUID( DCSFlagXML, "DCSFlagXML")

#endif // DCS_FLAG_XML_H
