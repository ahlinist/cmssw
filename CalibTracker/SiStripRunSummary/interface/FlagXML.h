// Flag base made for serialization in XML streams
//
// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 06/01/06
// License: GPL

#ifndef FLAG_XML_H
#define FLAG_XML_H

#include <list>
#include <map>

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/list.hpp>

#include "CalibTracker/SiStripRunSummary/interface/ClassIDBase.h"
#include "CalibTracker/SiStripRunSummary/interface/ClassID.h"
#include "CalibTracker/SiStripRunSummary/interface/Flag.h"

class FlagTxt;

class FlagXML: public Flag {
  public:
    FlagXML(): Flag(), poParentFlag_( 0) {}
    virtual ~FlagXML();

    template<class T> FlagXML *createChild();
    template<class T> FlagXML *getChild   () const;

    virtual bool setState( const State &reSTATE);

  protected:
    FlagXML( const FlagXML &roFLAGXML);
    FlagXML( const FlagTxt &roFLAGTXT);

    inline virtual int isChildValid( const FlagXML *) const {
      // No children are allowed by default
      return 0;
    }

  private:
    friend class FlagTxt;

    // prevent copy assignment
    FlagXML &operator =( const FlagXML &roFLAGXML);

    void updateState( const State &reCHILD_STATE);

    // XML Serialization
    friend class boost::serialization::access;

    template<class Archive>
      void save( Archive &roArchive, const unsigned int &rnVersion) const;

    template<class Archive>
      void load( Archive &roArchive, const unsigned int &rnVersion);

    BOOST_SERIALIZATION_SPLIT_MEMBER()

    typedef ClassIDBase::ID FlagID; 
    typedef std::map<FlagID,FlagXML *> MChildFlags;

    FlagXML     *poParentFlag_;
    MChildFlags oMChildFlags_;
};

template<class T>
  FlagXML *FlagXML::createChild() {
    FlagXML *poResult = 0;
    
    try {
      FlagXML *poFlag = new T();

      int nChildFlags = isChildValid( poFlag);
      if( 1 == nChildFlags) {
        // Get Class ID
        FlagID nID = ClassID<T>::get();

        // Check if Flag already exists
        std::pair<MChildFlags::const_iterator, MChildFlags::const_iterator>
          oFoundChildren = oMChildFlags_.equal_range( nID);

        // We assume only one Flag is allowed in case any value other then
        // 0 is returned
        if( 1 > std::distance( oFoundChildren.first,
                               oFoundChildren.second)) {

          poFlag->poParentFlag_ = this;

          // explicitly set state in order to update the parent state
          poFlag->setState( UNKNOWN);
          oMChildFlags_[nID] = poFlag;
          poResult = poFlag;
        } else {
          // No more child flags are allowed
          delete poFlag;
        }
      } else {
        // No children allowed
        delete poFlag;
      }
    } catch(...) {
      // Ups, trying to create child of Flag that is not inherited from
      // given class :[
    }

    return poResult;
  }

template<class T>
  FlagXML *FlagXML::getChild() const {
    // Get Class ID
    FlagID nID = ClassID<T>::get();

    // Find appropriate child
    std::pair<MChildFlags::const_iterator, MChildFlags::const_iterator>
      oFoundChildren = oMChildFlags_.equal_range( nID);

    // Return found child or invalid pointer in case child can not be found
    FlagXML *poChild = 0;
    if( 1 == std::distance( oFoundChildren.first, oFoundChildren.second)) {
      // One child was found: copy pointer
      poChild = oFoundChildren.first->second;
    }

    return poChild;
  }

template<class Archive>
  void FlagXML::save( Archive &roArchive, const unsigned int &rnVersion) const {
    roArchive & BOOST_SERIALIZATION_NVP( eState_);
    roArchive & BOOST_SERIALIZATION_NVP( oComment_);
    roArchive & BOOST_SERIALIZATION_NVP( poParentFlag_);

    // Copy Child Flags into list
    std::list<FlagXML *> oLChildren;
    for( MChildFlags::const_iterator oIter = oMChildFlags_.begin();
         oIter != oMChildFlags_.end();
         ++oIter) {

      oLChildren.push_back( oIter->second);
    }

    // Save Child Flags
    roArchive & BOOST_SERIALIZATION_NVP( oLChildren);
  }

template<class Archive>
  void FlagXML::load( Archive &roArchive, const unsigned int &rnVersion) {
    roArchive & BOOST_SERIALIZATION_NVP( eState_);
    roArchive & BOOST_SERIALIZATION_NVP( oComment_);
    roArchive & BOOST_SERIALIZATION_NVP( poParentFlag_);

    // Read Child Flags
    std::list<FlagXML *> oLChildren;
    roArchive & BOOST_SERIALIZATION_NVP( oLChildren);

    // Put all Read children to local map
    for( std::list<FlagXML *>::const_iterator oIter = oLChildren.begin();
         oIter != oLChildren.end();
         ++oIter) {

      // Get Class ID
      oMChildFlags_[( *oIter)->getID()] = *oIter;
    }
  }

#endif
