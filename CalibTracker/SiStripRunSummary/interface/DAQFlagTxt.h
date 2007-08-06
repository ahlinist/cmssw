// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/27/07
// License: GPL

#ifndef DAQ_FLAG_TXT_H
#define DAQ_FLAG_TXT_H

#include <iosfwd>

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/version.hpp>

#include "CalibTracker/SiStripRunSummary/interface/FlagTxt.h"
#include "CalibTracker/SiStripRunSummary/interface/ClassIDBase.h"

/**
* @brief
*   The same flag should also be implemented for XML format. We need 
*   to declare partner that is used for conversion: DAQFlagTxt <-> DAQFlagXML 
*/
class DAQFlagXML;

/** 
* @brief 
*   Txt format flag. Tasks:
*     1. Define cloning
*     2. Implement identification (get class ID)
*     3. Specify allowed children
*     4. Serialization part:
*           a) (de)serialization through base class
*           b) register class in boost/serialization
*     5. flag output in ostream (print, display).
*/
class DAQFlagTxt: public FlagTxt {
  public:
    DAQFlagTxt() {}

    /** 
    * @brief 
    *   Construct Txt format flag from XML formatted one
    * 
    * @param poDAQ_FLAGXML  XML format flag to be used as source
    */
    DAQFlagTxt( const DAQFlagXML *poDAQ_FLAGXML);

  protected:
    /** 
    * @brief 
    *   Clone method is used in copying Tree. Should be implemented by each
    *   flag individually. Flag should be created on a heap store.
    * 
    * @return 
    *   Pointer to newly created flag.
    */
    inline virtual Clonable *clone() const {
      return new DAQFlagTxt( *this);
    }

    /** 
    * @brief 
    *   Convert Txt format flag to XML. XML flag should be created on a heap
    *   store and constructed out of Txt formatted one.
    * 
    * @return 
    *   Pointer to newly created flag.
    */
    virtual Clonable *cloneXML() const;

    /** 
    * @brief 
    *   Used in (de)serialization process to get class ID.
    * 
    * @return 
    *   ID of current class.
    */
    ClassIDBase::ID getID() const;

    /** 
    * @brief 
    *   Override current method in case flag may have any children.
    * 
    * @param poCHILD_CANDIDATE  Pointer to flag to be checked if it is allowed.
    * 
    * @return 
    *   Number of children allowed (refer to base class for details)
    */
    virtual int isChildValid( const FlagTxt *poCHILD_CANDIDATE) const;

  private:
    /**
    * @brief
    *                                                   --[ BEGIN ]--
    *   Serialization section: Text format
    */
    friend class boost::serialization::access;

    template<class Archive>
      void serialize( Archive &roArchive, const unsigned int &rnVERSION) {
        roArchive & boost::serialization::base_object<FlagTxt>( *this);
      }
    /**
    * @brief
    *                                                   --[ END ]--
    */
};

/** 
* @brief 
*   Make flag printable. Flag output format is defined by:
*
*     std::ostream &operator <<( std::ostream &roOut, const Flag &roFLAG);
*
* @param roOut   Output stream where flag should be written
* @param roFLAG  Flag to be written
* 
* @return 
*   Reference to output stream.
*/
std::ostream &
  operator <<( std::ostream &roOut, const DAQFlagTxt &roFLAG);

/**
* @brief
*   Register class in boost/serialization otherwise flag can not be saved/read.
*   [IMPORTANT: Label (the second argument) should be unique (!). This is
*               pretty much the same as class ID]
*/
BOOST_CLASS_EXPORT_GUID( DAQFlagTxt, "DAQFlagTxt")

/**
* @brief
*   Register current class version. Given number will be used in serialization
*   process in should be changed once core changes in order to maintain any
*   saved tree before. For example, Flag class will be extended with additional
*   property, let's say: author. Any tree that was created before won't have 
*   given value. Thus attempting to read it would lead to runtime error.
*   Versioning was introduced to solve current issue.
*/
BOOST_CLASS_VERSION( DAQFlagTxt, 0)

#endif // DAQ_FLAG_TXT_H
