// Flag base made for serialization in Text/Binary streams
//
// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 06/01/06
// License: GPL

#ifndef FLAG_TXT_H
#define FLAG_TXT_H

#include <list>
#include <map>

#include <boost/serialization/string.hpp>
#include <boost/serialization/list.hpp>

#include "interface/ClassIDBase.h"
#include "interface/ClassID.h"
#include "interface/Flag.h"

class FlagXML;

class FlagTxt: public Flag {
  public:
    FlagTxt(): Flag(), poParentFlag_( 0) {}
    virtual ~FlagTxt();

    template<class T> FlagTxt *createChild();
    template<class T> FlagTxt *getChild   () const;

    virtual bool setState( const State &reSTATE);

  protected:
    FlagTxt( const FlagTxt &roFLAGTXT);
    FlagTxt( const FlagXML &roFLAGXML);

    inline virtual int isChildValid( const FlagTxt *) const {
      // No children are allowed by default
      return 0;
    }

  private:
    friend class FlagXML;

    // prevent copy assignment
    FlagTxt &operator =( const FlagTxt &roFLAGTXT);

    void updateState( const State &reCHILD_STATE);

    // Text Serialization
    friend class boost::serialization::access;

    template<class Archive>
      void save( Archive &roArchive, const unsigned int &rnVersion) const;

    template<class Archive>
      void load( Archive &roArchive, const unsigned int &rnVersion);

    BOOST_SERIALIZATION_SPLIT_MEMBER()

    typedef ClassIDBase::ID FlagID; 
    typedef std::map<FlagID,FlagTxt *> MChildFlags;

    FlagTxt    *poParentFlag_;
    MChildFlags oMChildFlags_;
};

template<class T>
  FlagTxt *FlagTxt::createChild() {
    FlagTxt *poResult = 0;
    
    try {
      FlagTxt *poFlag = new T();

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
  FlagTxt *FlagTxt::getChild() const {
    // Get Class ID
    FlagID nID = ClassID<T>::get();

    // Find appropriate child
    std::pair<MChildFlags::const_iterator, MChildFlags::const_iterator>
      oFoundChildren = oMChildFlags_.equal_range( nID);

    // Return found child or invalid pointer in case child can not be found
    FlagTxt *poChild = 0;
    if( 1 == std::distance( oFoundChildren.first, oFoundChildren.second)) {
      // One child was found: copy pointer
      poChild = oFoundChildren.first->second;
    }

    return poChild;
  }

template<class Archive>
  void FlagTxt::save( Archive &roArchive, const unsigned int &rnVersion) const {
    roArchive & eState_;
    roArchive & oComment_;
    roArchive & poParentFlag_;

    // Copy Child Flags into list
    std::list<FlagTxt *> oLChildren;
    for( MChildFlags::const_iterator oIter = oMChildFlags_.begin();
         oIter != oMChildFlags_.end();
         ++oIter) {

      oLChildren.push_back( oIter->second);
    }

    // Save Child Flags
    roArchive & oLChildren;
  }

template<class Archive>
  void FlagTxt::load( Archive &roArchive, const unsigned int &rnVersion) {
    roArchive & eState_;
    roArchive & oComment_;
    roArchive & poParentFlag_;

    // Read Child Flags
    std::list<FlagTxt *> oLChildren;
    roArchive & oLChildren;

    // Put all Read children to local map
    for( std::list<FlagTxt *>::const_iterator oIter = oLChildren.begin();
         oIter != oLChildren.end();
         ++oIter) {

      // Get Class ID
      oMChildFlags_[( *oIter)->getID()] = *oIter;
    }
  }

#endif // FLAG_TXT_H
