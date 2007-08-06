// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 06/01/06
// License: GPL

#ifndef FLAG_TXT_H
#define FLAG_TXT_H

#include <list>
#include <map>

#include <boost/serialization/string.hpp>
#include <boost/serialization/list.hpp>

#include "CalibTracker/SiStripRunSummary/interface/ClassIDBase.h"
#include "CalibTracker/SiStripRunSummary/interface/ClassID.h"
#include "CalibTracker/SiStripRunSummary/interface/Flag.h"

// Declare class to save compilation time: used for conversion of flags
// XML -> Txt
class FlagXML;

/** 
* @brief 
*   Flags base that:
*     1. Knows how to serialize Flags Tree in text/binary formats.
*     2. Holds children and responsible for their creation and access.
*     3. Tracks Children change state in order to update state of parent.
*        [aka recursive set State].
*/
class FlagTxt: public Flag {
  public:
    typedef std::list<FlagTxt *> LChildren;

    FlagTxt(): Flag() { poParentFlag_ = 0; }
    virtual ~FlagTxt();

    /** 
    * @brief 
    *   Method that is responsible for Child Flags creation. Its tasks are:
    *     1. Check if Child Flag of given type is allowed
    *     2. Create Child Flag in case (1.) is true
    *     3. Set newly created child flags parent.
    * 
    * @return 
    *   >0  Pointer to newly created flag
    *   0   Invalid pointer in case child flag of given class is not allowed.
    */
    template<class T> FlagTxt *createChild();

    /** 
    * @brief 
    *   Search for a specific child flag of given class if there is any
    *   available among children. Flag should be first created with 
    *   createChild() method.
    * 
    * @return 
    *   >0  Pointer to child flag if there is any available.
    *   0   Invalid pointer in case child flag does not exist.
    */
    template<class T> FlagTxt *getChild   () const;

    /** 
    * @brief 
    *   Search for children that are instances of the same class. Flags should
    *   be first created with createChild() method. Method also can be used as
    *   a getChild but might be inconvenient since it adds std::list overhead
    *
    *   [CAUTION: Variable will be overwritten (!) Anything contained there
    *             will be removed]
    * 
    * @return 
    *   number of children available
    */
    template<class T> int getChildren( LChildren &roLChildren) const;

    /** 
    * @brief 
    *   Override base setState method to update parents state.
    * 
    * @param reSTATE  State of flag to be set.
    * 
    * @return 
    *   true   In case flag's state was changed
    *   false  Otherwise
    */
    virtual bool setState( const State &reSTATE);

  protected:
    /** 
    * @brief 
    *   Construct flag from another. Used by cloneXXX().
    * 
    * @param roFLAGTXT  Flag to be copied.
    */
    FlagTxt( const FlagTxt &roFLAGTXT);

    /** 
    * @brief 
    *   Construct flag from another. Used by cloneXXX().
    * 
    * @param roFLAGXML  Flag to be copied.
    */
    FlagTxt( const FlagXML &roFLAGXML);

    /** 
    * @brief 
    *   Specify policy of current flag: can it have any children? Should be
    *   overriden by successors. Use:
    *
    *     dynamic_cast<const AllowedSubFlagClass *>( poCHILD_CANDIDATE)
    *
    *   expression to check if child is allowed.
    * 
    * @param poCHILD_CANDIDATE  Pointer to child candidate object that should
    *                           be checked for validity.
    * 
    * @return 
    *   Number of allowed children:
    *     -1  No limit
    *      0  No children allowed
    *      1  One child only
    *     ...
    *      N  N children allowed (current number of children may be from zero
    *         up to given number)
    */
    inline virtual 
      int isChildValid( const FlagTxt * /* poCHILD_CANDIDATE */) const {
        
      // No children are allowed by default
      return 0;
    }

  private:
    /** 
    * @brief 
    *   Grant access of FlagXML to children. Used in conversion: Txt -> XML.
    */
    friend class FlagXML;

    typedef ClassIDBase::ID FlagID; 
    typedef std::multimap<FlagID,FlagTxt *> MChildFlags;

    /** 
    * @brief 
    *   prevent copy assignment
    * 
    * @param roFLAGTXT  Flag to be copied
    * 
    * @return 
    *   Reference to current flag.
    */
    FlagTxt &operator =( const FlagTxt &roFLAGTXT);

    /** 
    * @brief 
    *   Update parents state. Given method specifies logic: how 
    *   successors affect ancestors.
    * 
    * @param reCHILD_STATE  New state (changed) of Child flag
    */
    void updateState( const State &reCHILD_STATE);

    /** 
    * @brief 
    *   Add child flag into the container of subflags
    * 
    * @param poSubFlag  Flag to be added 
    */
    void addChild( FlagTxt *poSubFlag);

    /**
    * @brief
    *                                                   --[ BEGIN ]--
    *   Serialization section: Text format
    */
    friend class boost::serialization::access;

    template<class Archive>
      void save( Archive &roArchive, const unsigned int &rnVersion) const;

    template<class Archive>
      void load( Archive &roArchive, const unsigned int &rnVersion);

    BOOST_SERIALIZATION_SPLIT_MEMBER()
    /**
    * @brief
    *                                                   --[ END ]--
    */

    FlagTxt     *poParentFlag_;
    MChildFlags  oMChildFlags_;
};

// --[ TEMPLATES IMPLEMENTATION ]----------------------------------------------
//                                                  --[ PUBLIC ]--
/**
* @brief
*   Create child flag of desired class
*/
template<class T>
  FlagTxt *FlagTxt::createChild() {
    FlagTxt *poResult = 0;
    
    try {
      // Create object of requested class: may throw exception
      FlagTxt *poFlag = new T();

      // Get number of allowed children
      int nChildFlags = isChildValid( poFlag);

      FlagID nID = ClassID<T>::get();

      switch( nChildFlags) {
        case 0:
          // No child flags allowed
          delete poFlag;
          break;
        case -1:
          // No limit on child flags
          {
            // Parent will be set by addChild(...)
            addChild( poFlag);

            // Update parent state: child may be put into ERROR state by 
            // default (this is specified in Flag class). Unfortunately
            // parent won't be updated
            updateState( poFlag->getState());

            poResult = poFlag;
          }
          break;
        default:
          // All other cases
          {
            if( 0 < nChildFlags) {
              // Check if Flag already exists
              std::pair<MChildFlags::const_iterator, 
                        MChildFlags::const_iterator> oFoundChildren = 
                oMChildFlags_.equal_range( nID);

              if( nChildFlags > std::distance( oFoundChildren.first,
                                               oFoundChildren.second)) {

                // Parent will be set by addChild(...)
                addChild( poFlag);

                // Update parent state: child may be put into ERROR state by 
                // default (this is specified in Flag class). Unfortunately
                // parent won't be updated
                updateState( poFlag->getState());

                poResult = poFlag;
              } else {
                // No more child tags are allowed
                delete poFlag;
              }
            } else {
              // Unsupported value of child flags returned
              delete poFlag;
            }// End check if nChildFlags is positive
          }
          break;
      } // End switch( nChildFlags)
    } catch(...) {
      // Ups, trying to create child of Flag that is not inherited from
      // given class :[
    }

    return poResult;
  }

/**
* @brief
*   Extract child flag of desired class
*/
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
      // One child was found: copy pointer.
      // [ToDo: add support for several allowed children]
      poChild = oFoundChildren.first->second;
    }

    return poChild;
  }

/**
* @brief
*   Extract child flags of desired class
*/
template<class T>
  int FlagTxt::getChildren( LChildren &roLChildren) const {
    // Get Class ID
    FlagID nID = ClassID<T>::get();

    // Find appropriate children
    std::pair<MChildFlags::const_iterator, MChildFlags::const_iterator>
      oFoundChildren = oMChildFlags_.equal_range( nID);

    // Return found child or invalid pointer in case child can not be found
    // Return list of found children or empty one if children can not be
    // found
    roLChildren.clear();
    if( 0 < std::distance( oFoundChildren.first, oFoundChildren.second)) {
      // A number of children was found: copy them into a list and return it
      while( oFoundChildren.first != oFoundChildren.second) {
        roLChildren.push_back( ( oFoundChildren.first++)->second);
      }
    } // End check if any children are found

    return roLChildren.size();
  }

//                                                  --[ PRIVATE ]--
/**
* @brief
*                                                   --[ BEGIN ]--
*   Serialization section: Text format
*/
template<class Archive>
  void FlagTxt::save( Archive &roArchive, const unsigned int &rnVersion) const {
    roArchive & eState_;
    roArchive & oComment_;

    // Copy Child Flags into list: we can not save map since IDs of classes
    // are runtime dependent. Solution: save pure list of child flags. And
    // Recreate map on reading the list from archive.
    LChildren oLChildren;
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

    // Read Child Flags: regular list will be extracted.
    LChildren oLChildren;
    roArchive & oLChildren;

    // Reconstruct map of child flags since IDs of classes are runtime
    // dependent.
    for( LChildren::const_iterator oIter = oLChildren.begin();
         oIter != oLChildren.end();
         ++oIter) {

      // Parent will be set by addChild(...)
      addChild( *oIter);
    }
  }
/**
* @brief
*                                                   --[ END ]--
*/

#endif // FLAG_TXT_H
