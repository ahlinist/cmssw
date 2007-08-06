// Author : Samvel Khalatian (ksamdev at gmail dot com)
// Created: 05/31/07
// License: GPL

#ifndef FLAG_H
#define FLAG_H

#include <iosfwd>
#include <string>

#include "CalibTracker/SiStripRunSummary/interface/Clonable.h"
#include "CalibTracker/SiStripRunSummary/interface/Identifiable.h"

/** 
* @brief 
*   Flag Base container that holds such values as:
*     1. Current Flag state
*     2. Flag comment
*/
class Flag: public Clonable,
            public Identifiable 
{
  public:
    /** 
    * @brief 
    *   Available Flags states
    */
    enum State { UNKNOWN = 0, 
                 ERROR   = 1, 
                 OK      = 2 };

    Flag(): eState_( UNKNOWN), oComment_( "") {}
    virtual ~Flag() {}

    /** 
    * @brief 
    *   Leave chance to override setState method to add option of updating 
    *   parent value based on child change. Should be implemented by children
    *   that hold containers of sub-flags.
    * 
    * @param reSTATE  New State of flag to be set
    * 
    * @return 
    *   true   If flag state changed [it will change only in case new value
    *          differs from current one]
    *   false  Otherwise
    */
    virtual bool setState  ( const State &reSTATE);

    /** 
    * @brief 
    *   Set flag comment
    * 
    * @param roCOMMENT  New comment to be set [any previous comment will be
    *                   replaced]
    */
    inline  void setComment( const std::string &roCOMMENT) {
      oComment_ = roCOMMENT;
    }

    /** 
    * @brief  
    *   Retrieve Flags current state.
    * 
    * @return 
    *   Flag::State
    */
    inline State       getState  () const { return eState_; }

    /** 
    * @brief 
    *   Retrieve Flags comment
    * 
    * @return 
    *   std::string  Representing comment
    */
    inline std::string getComment() const { return oComment_; }

  protected:
    /** 
    * @brief 
    *   Allow only children to copy Flags. Method is needed for base 
    *   construction.
    * 
    * @param roFLAG  Flag to be copied.
    */
    Flag( const Flag &roFLAG)
      : eState_  ( roFLAG.eState_),
        oComment_( roFLAG.oComment_) {}

    /**
    * @brief
    *   Unfortunately all properties of Flag (that are going to be serialized)
    *   have to be protected. Otherwise Serialization won't work :(.
    */
    State       eState_;
    std::string oComment_;

  private:
    // Prevent copying Flags via assignment
    /** 
    * @brief 
    *   Prevent copying Flags via assignment
    * 
    * @param rnFLAG  Flag to be copied
    * 
    * @return 
    *   Reference to current flag
    */
    Flag &operator =( const Flag &rnFLAG);
};

/** 
* @brief 
*   Make flag displayable. Helper function will define output format that will
*   be used by all Child Flags.
* 
* @param roOut   Output stream where flag should be written
* @param roFLAG  Flag to be written
* 
* @return 
*   Reference to output stream.
*/
std::ostream &operator <<( std::ostream &roOut,
                           const Flag &roFLAG);

#endif // FLAG_H
