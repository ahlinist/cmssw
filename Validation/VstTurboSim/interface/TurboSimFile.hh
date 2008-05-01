#ifndef __TurboSimFile_h
#define __TurboSimFile_h

#include<fstream>
#include<string>
#include "Validation/VstTurboSim/interface/TurboSimBuffer.hh"
#include "Validation/VstTurboSim/interface/TurboSimSaveableItem.hh"

///comments here

class TurboSimFile : public TurboSimBuffer {

public:

  // Constructors
  TurboSimFile( ) { }
  explicit TurboSimFile( const std::string &filename,
			 int mode = READ ) {
    if ( mode == READ ) 
      openFileForInput( filename );
    else if ( mode == WRITE )
      openFileForOutput( filename );
  }

  TurboSimFile( const std::string &inFilename,
		const std::string &outFilename ) {
    openFileForInput( inFilename );
    openFileForOutput( outFilename );
  }

  virtual ~TurboSimFile() {
    close();
  }

  void close() {
    fin.close();
    fout.close();
    clear();
  }

  //mrenna
  bool is_open() {
    if ( mode() == READ )
      return fin.is_open();
    else if ( mode() == WRITE )
      return fout.is_open();
    else
      return ( fout.is_open() &&
	       fin.is_open() );
  }

  bool openFileForInput( const std::string &filename ) {
    fin.close();
    fin.open( filename.c_str(), std::ios::in );
    if ( fin.is_open() ) 
      if ( setSource( &fin ).successfulRead() )
	return true;
    return false;
  }

  bool openFileForOutput( const std::string &filename ) {
    fout.close();
    fout.open( filename.c_str(), std::ios::out );
    if ( fout.is_open() )
      if ( setDest( &fout ).successfulWrite() )
	return true;
    return false;
  }

private:

  std::ifstream fin;
  std::ofstream fout;

};

#endif
