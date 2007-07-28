// Global Flag (serialization in XML stream)
// 
// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/27/07
// License: GPL

#ifndef GLOBAL_FLAG_XML_H
#define GLOBAL_FLAG_XML_H

#include <iosfwd>

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/export.hpp>

#include "CalibTracker/SiStripRunSummary/interface/FlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/ClassIDBase.h"

class GlobalFlagTxt;

class GlobalFlagXML: public FlagXML {
  public:
    GlobalFlagXML() {}
    GlobalFlagXML( const GlobalFlagTxt *poGLOBAL_FLAGTXT);

  protected:
    // Used in copying Tree
    inline virtual Clonable *clone() const {
      return new GlobalFlagXML( *this);
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
  operator <<( std::ostream &roOut, const GlobalFlagXML &roFLAG);

BOOST_CLASS_EXPORT_GUID( GlobalFlagXML, "GlobalFlagXML")

#endif // GLOBAL_FLAG_XML_H
