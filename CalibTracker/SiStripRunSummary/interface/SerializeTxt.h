// Flag Store Manager (store in external file/stream). Serialize in TXT/BIN
// format.
// 
// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 06/01/07
// License: GPL

#ifndef SERIALIZE_TXT
#define SERIALIZE_TXT

#include <string>

class DQMFlagTxt;

class SerializeTxt {
  public:
    SerializeTxt() { poFlagTxt_ = 0; }
    ~SerializeTxt();

    bool write   ( const char *pcFILE, const DQMFlagTxt &roDQM_FLAGTXT);
    bool writeStr( std::string &roStringOut, const DQMFlagTxt &roDQM_FLAGTXT);

    bool read   ( const char *pcFILE);
    bool readStr( const std::string &roStringIn);

    DQMFlagTxt *getFlag() { return poFlagTxt_; }

  private:
    DQMFlagTxt *poFlagTxt_;
};

class SerializeBin {
  public:
    SerializeBin() { poFlagTxt_ = 0; }
    ~SerializeBin();

    bool write   ( const char *pcFILE, const DQMFlagTxt &roDQM_FLAGTXT);
    bool writeStr( std::string &roStringOut, const DQMFlagTxt &roDQM_FLAGTXT);

    bool read   ( const char *pcFILE);
    bool readStr( const std::string &roStringIn);

    DQMFlagTxt *getFlag() { return poFlagTxt_; }

  private:
    DQMFlagTxt *poFlagTxt_;
};

#endif // SERIALIZE_TXT
