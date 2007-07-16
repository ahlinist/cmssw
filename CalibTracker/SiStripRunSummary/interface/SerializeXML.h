// Flag Store Manager (store in external file/stream). Serialize in XML 
// format.
// 
// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 06/01/07
// License: GPL

#ifndef SERIALIZE_XML
#define SERIALIZE_XML

class DQMFlagXML;

class SerializeXML {
  public:
    SerializeXML() { poFlagXML_ = 0; }
    ~SerializeXML();

    bool write( const char *pcFILE, DQMFlagXML &roDQMFlagXML);
    bool read ( const char *pcFILE); 

    DQMFlagXML *getFlag() { return poFlagXML_; }

  private:
    DQMFlagXML *poFlagXML_;
};

#endif
