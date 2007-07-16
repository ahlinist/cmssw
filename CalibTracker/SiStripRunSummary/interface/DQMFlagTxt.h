// DQM Flag (serialization in Text/Binary stream)
// 
// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 06/01/07
// License: GPL

#ifndef DQM_FLAG_TXT_H
#define DQM_FLAG_TXT_H

#include <ostream>

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/export.hpp>

#include "CalibTracker/SiStripRunSummary/interface/FlagTxt.h"
#include "CalibTracker/SiStripRunSummary/interface/ClassID.h"
#include "CalibTracker/SiStripRunSummary/interface/TIBFlagTxt.h"
#include "CalibTracker/SiStripRunSummary/interface/TOBFlagTxt.h"

class DQMFlagXML;

class DQMFlagTxt: public FlagTxt {
  public:
    DQMFlagTxt() {}
    DQMFlagTxt( const DQMFlagXML *poDQM_FLAGXML);

    // Used in copying Tree
    inline virtual Clonable *clone() const {
      return new DQMFlagTxt( *this);
    }

    virtual Clonable *cloneXML() const;

  protected:
    // Used in serialization to get class ID
    inline virtual ClassIDBase::ID getID() const {
      return ClassID<DQMFlagTxt>::get();
    }

    // Enable children
    inline virtual int isChildValid( const FlagTxt *poCHILD_CANDIDATE) const {
      return dynamic_cast<const TIBFlagTxt *>( poCHILD_CANDIDATE) ||
             dynamic_cast<const TOBFlagTxt *>( poCHILD_CANDIDATE) ?
             1 :
             0;
    }

  private:
    // Serialization
    friend class boost::serialization::access;

    template<class Archive>
      void serialize( Archive &roArchive, const unsigned int &rnVERSION) {
        roArchive & boost::serialization::base_object<FlagTxt>( *this);
      }
};

std::ostream &
  operator <<( std::ostream &roOut, const DQMFlagTxt &roFLAG);

BOOST_CLASS_EXPORT_GUID( DQMFlagTxt, "DQMFlagTxt")

#endif // DQM_FLAG_TXT_H
