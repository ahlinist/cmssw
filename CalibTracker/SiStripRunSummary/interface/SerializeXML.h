// Flag Store Manager (store in external file/stream). Serialize in XML 
// format.
// 
// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 06/01/07
// License: GPL

#ifndef SERIALIZE_XML
#define SERIALIZE_XML

#include <iosfwd>

class FlagXML;

// --[ XML ARCHIVE MANAGER ]---------------------------------------------------

/** 
* @brief 
*   Generic Manager class that saves/reads any FlagXML anchestor in/from
*   output/input stream in XML format.
*/
class SerializeXML {
  public:
    SerializeXML();
    ~SerializeXML();

    /** 
    * @brief
    *   Generic method for saving any FlagXML anchestor in output stream
    *   that may be ostringstream, ofstream, etc. Type will be handled by
    *   boost::serialization library automatially.
    * 
    * @param roOut      Output stream where flag will be saved
    * @param roFLAGXML  Flag reference to be saved
    * 
    * @return 
    *   true    On success
    *   false   Otherwise
    */
    bool write( std::ostream &roOut, const FlagXML &roFLAGXML) const;

    /** 
    * @brief 
    *   Generic method for reading FlagXML from stream. Class T should be the
    *   Flags class to be read. In given case different Trees can be read
    *   with the same manager SerializeXML, e.g.:
    *
    *     DQMFlagXML *poFlag = oSerializeXML.read<DQMFlagXML>( oIn);
    *     TIBFlagXML *poFlag = oSerializeXML.read<TIBFlagXML>( oIn);
    *
    *   of course it is assumed that stream contains given flag stored. Trying
    *   to read incorrect flag would lead to exception that is caught by
    *   manager and NULL pointer is returned.
    * 
    * @param roIn  Input stream containing saved Tree
    * 
    * @return 
    *   >0  Valud pointer to read Flags Tree on success. 
    *       [Note: Object will be deleted by SerializeXML automatically in
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
    inline FlagXML *getFlag() { return poFlagXML_; }

  private:
    /** 
    * @brief 
    *   Perform actual flag read. Used by template read method to overcome
    *   compilation problems with include of boost/archive/xml_i(o)archive.hpp
    *   from header [should be included only once from cpp].
    * 
    * @param poFlagXML  Pointer to FlagXML anchestor to be saved in output
    *                   stream
    * @param roIn       Output stream where flag will be saved
    */
    void readFlag( FlagXML *poFlagXML, std::istream &roIn);

    FlagXML *poFlagXML_;
};

template<class T> T *SerializeXML::read( std::istream &roIn) {
  if( poFlagXML_) {
    delete poFlagXML_;
    poFlagXML_ = 0;
  }

  // [Note: Object will be deleted by SerializeXML (!)]
  T *poT = new T();

  // Check if this is actually child class of FlagXML
  if( FlagXML *poFlagXML = dynamic_cast<FlagXML *>( poT)) {

    // Attempt to read flag from tree
    try {
      readFlag( poFlagXML, roIn);

      poFlagXML_ = poFlagXML;
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

#endif // SERIALIZE_XML
