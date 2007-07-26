// Flag Store Manager (store in external file/stream). Serialize in TXT/BIN
// format.
// 
// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 06/01/07
// License: GPL

#ifndef SERIALIZE_TXT_BIN
#define SERIALIZE_TXT_BIN

#include <iosfwd>

#include "interface/SerializeBase.h"

class FlagTxt;

// --[ TEXT ARCHIVE MANAGER ]--------------------------------------------------

/** 
* @brief 
*   Generic Manager class that saves/reads any FlagTxt anchestor in/from
*   output/input stream in form of Text Characters Sequence.
*/
class SerializeTxt: public SerializeBase {
  public:
    SerializeTxt(): SerializeBase() {}

  private:
    virtual void writeFlag( std::ostream &roOut,
                            const FlagTxt &roFLAGTXT) const;

    virtual void readFlag ( FlagTxt *poFlagTxt, std::istream &roIn);
};

// --[ BINARY ARCHIVE MANAGER ]------------------------------------------------

/** 
* @brief 
*   Generic Manager class that saves/reads any FlagTxt anchestor in/from
*   output/input stream in form of Binary Characters Sequence.
*/
class SerializeBin: public SerializeBase {
  public:
    SerializeBin(): SerializeBase() {}

  private:
    virtual void writeFlag( std::ostream &roOut,
                            const FlagTxt &roFLAGTXT) const;

    virtual void readFlag ( FlagTxt *poFlagTxt, std::istream &roIn);
};

#endif // SERIALIZE_TXT_BIN
