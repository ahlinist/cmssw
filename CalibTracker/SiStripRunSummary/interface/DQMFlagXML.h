// DQM Flag (serialization in XML stream)
// 
// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 06/01/07
// License: GPL

#ifndef DQM_FLAG_XML_H
#define DQM_FLAG_XML_H

#include <iosfwd>

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/export.hpp>

#include "interface/FlagXML.h"
#include "interface/ClassIDBase.h"

class DQMFlagTxt;

class DQMFlagXML: public FlagXML {
  public:
    DQMFlagXML() {}
    DQMFlagXML( const DQMFlagTxt *poDQM_FLAGTXT);

  protected:
    // Used in copying Tree
    inline virtual Clonable *clone() const {
      return new DQMFlagXML( *this);
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
  operator <<( std::ostream &roOut, const DQMFlagXML &roFLAG);

BOOST_CLASS_EXPORT_GUID( DQMFlagXML, "DQMFlagXML")

#endif // DQM_FLAG_XML_H
