// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/25/07
// License: GPL

#ifndef SERIALIZE_BASE
#define SERIALIZE_BASE

#include <iosfwd>
#include <istream>

#include <boost/archive/archive_exception.hpp>

class FlagTxt;

// --[ BASE ARCHIVE MANAGER ]--------------------------------------------------

/** 
* @brief 
*   Generic Base for Manager classes that save/read any child FlagTxt class
*   in/from output/input stream in form of text, binary stream or even XML.
*/
class SerializeBase {
  public:
    SerializeBase();
    virtual ~SerializeBase();

    /** 
    * @brief
    *   Generic method for saving any FlagTxt anchestor in output stream
    *   that may be ostringstream, ofstream, etc. Type will be handled by
    *   boost::serialization library automatially.
    * 
    * @param roOut      Output stream where flag will be saved
    * @param roFLAGTXT  Flag reference to be saved
    * 
    * @return 
    *   true    On success
    *   false   Otherwise
    */
    bool write( std::ostream &roOut, const FlagTxt &roFLAGTXT) const;

    /** 
    * @brief 
    *   Generic method for reading FlagTxt from stream. Class T should be the
    *   Flags class to be read. In given case different Trees can be read
    *   with the same manager SerializeTxt, e.g.:
    *
    *     DQMFlagTxt *poFlag = oSerializeTxt.read<DQMFlagTxt>( oIn);
    *     TIBFlagTxt *poFlag = oSerializeTxt.read<TIBFlagTxt>( oIn);
    *
    *   of course it is assumed that stream contains given flag stored. Trying
    *   to read incorrect flag would lead to exception that is caught by
    *   manager and NULL pointer is returned.
    * 
    * @param roIn  Input stream containing saved Tree
    * 
    * @return 
    *   >0  Valud pointer to read Flags Tree on success. 
    *       [Note: Object will be deleted by SerializeTxt automatically in
    *              destructor (!!!) or next attempt to read flag. Do not 
    *              handle manually - copy it for further use]
    *   0   Otherwise
    */
    template<class T> T *read( std::istream &roIn);

    /** 
    * @brief  Get pointer to last read Flag if there is any available
    * 
    * @return 
    *   >0  Pointer to valid Flag
    *   0   If Flag was not read or failed to do so
    */
    inline FlagTxt *getFlag() { return poFlagTxt_; }

  private:
    /** 
    * @brief 
    *   Perform actual flag wtore operation. Method should be implemented by
    *   anchestor.
    * 
    * @param roOut      Output stream where flags tree will be saved
    * @param roFLAGTXT  Reference to flag to be stored
    */
    virtual void writeFlag( std::ostream &roOut, 
                            const FlagTxt &roFLAGTXT) const = 0;

    /** 
    * @brief 
    *   Perform actual flag read. Used by template read method to overcome
    *   compilation problems with include of boost/archive/text_i(o)archive.hpp
    *   from header [should be included only once from cpp]. Should be
    *   implemented by anchestors.
    * 
    * @param poFlagTxt  Pointer to FlagTxt anchestor to be saved in output
    *                   stream
    * @param roIn       Output stream where flag will be saved
    */
    virtual void readFlag ( FlagTxt *poFlagTxt, std::istream &roIn) = 0;

    FlagTxt *poFlagTxt_;
};

template<class T> T *SerializeBase::read( std::istream &roIn) {
  if( poFlagTxt_) {
    delete poFlagTxt_;
    poFlagTxt_ = 0;
  }

  // [Note: Object will be deleted by SerializeTxt (!)]
  T *poT = new T();

  // Check if this is actually child class of FlagTxt
  if( FlagTxt *poFlagTxt = dynamic_cast<FlagTxt *>( poT)) {

    // Attempt to read flag from tree
    try {
      readFlag( poFlagTxt, roIn);

      poFlagTxt_ = poFlagTxt;
    } catch( ...) { 
      // Failed: maybe stream contains other flag saved - not T?
      delete poT;
      poT = 0;
    }
  } else {
    // Not a child
    delete poT;
    poT = 0;
  }

  return poT;
}

#endif // SERIALIZE_BASE
