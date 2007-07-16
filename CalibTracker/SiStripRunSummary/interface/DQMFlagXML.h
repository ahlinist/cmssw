// DQM Flag (serialization in XML stream)
// 
// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 06/01/07
// License: GPL

#ifndef DQM_FLAG_XML_H
#define DQM_FLAG_XML_H

#include <ostream>

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/export.hpp>

#include "CalibTracker/SiStripRunSummary/interface/FlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/ClassID.h"
#include "CalibTracker/SiStripRunSummary/interface/TIBFlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/TOBFlagXML.h"

class DQMFlagTxt;

class DQMFlagXML: public FlagXML {
  public:
    DQMFlagXML() {}
    DQMFlagXML( const DQMFlagTxt *poDQM_FLAGTXT);

    // Used in copying Tree
    inline virtual Clonable *clone() const {
      return new DQMFlagXML( *this);
    }

    virtual Clonable *cloneTxt() const;

  protected:
    // Used in serialization to get class ID
    inline virtual ClassIDBase::ID getID() const {
      return ClassID<DQMFlagXML>::get();
    }

    // Enable children
    inline virtual int isChildValid( const FlagXML *poCHILD_CANDIDATE) const {
      return dynamic_cast<const TIBFlagXML *>( poCHILD_CANDIDATE) ||
             dynamic_cast<const TOBFlagXML *>( poCHILD_CANDIDATE) ?
             1 :
             0;
    }

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
