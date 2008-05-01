#ifndef __TurboSimBuffer_h
#define __TurboSimBuffer_h

#include <istream>
#include <ostream>
#include <vector>
#include "Validation/VstTurboSim/interface/TurboSimSaveableItem.hh"

class TurboSimBuffer {

public:

  enum {
    NOT_OPEN = 0,
    READ = 1,
    WRITE = 2 
  };
   
  virtual ~TurboSimBuffer() {
    if ( _dest )
      delete _dest;
    if ( _source )
      delete _source;
  }

  // Store into buffer
  virtual TurboSimBuffer &operator<<( const TurboSimSaveableItem &data ) {
    clearErrors();
    if ( not ( _mode & WRITE ) ) {
      _successfulWrite = false;
      return *this;
    }
    if ( _writeOptions.empty() )
      _successfulWrite = ( *_dest << data.toString() );
    else 
      switch ( _writeOptions.size() ) {
      case 1:
	_successfulWrite = ( *_dest << data.toString( _writeOptions[0] ) );
	break;
      case 2:
	_successfulWrite = ( *_dest << data.toString( _writeOptions[0],
						      _writeOptions[1] ) );
	break;
      case 3:
	_successfulWrite = ( *_dest << data.toString( _writeOptions[0],
						      _writeOptions[1],
						      _writeOptions[2] ) );
	break;
      case 4:
	_successfulWrite = ( *_dest << data.toString( _writeOptions[0],
						      _writeOptions[1],
						      _writeOptions[2],
						      _writeOptions[3] ) );
	break;
      }
    
    return *this;
  }

  virtual TurboSimBuffer &operator<<( const std::string &data ) {
    clearErrors();
    if ( not ( _mode & WRITE ) ) {
      _successfulWrite = false;
      return *this;
    }
    _successfulWrite = ( *_dest << data );
    return *this;
  }

  // Read from buffer
  virtual TurboSimBuffer &operator>>( TurboSimSaveableItem &item ) { 
    clearErrors();
    if ( not ( _mode & READ ) ) {
      _successfulRead = false;
      return *this;
    }
    if ( _readOptions.empty() )
      _successfulRead = ( item.read( *_source ) );
    else 
      switch ( _readOptions.size() ) {
      case 1:
	_successfulRead = ( item.read( *_source, 
				       _readOptions[0] ) );
	break;
      case 2:
	_successfulRead = ( item.read( *_source, 
				       _readOptions[0],
				       _readOptions[1] ) );
	break;
      case 3:
	_successfulRead = ( item.read( *_source, 
				       _readOptions[0],
				       _readOptions[1],
				       _readOptions[2] ) );
	break;
      case 4:
	_successfulRead = ( item.read( *_source,
				       _readOptions[0],
				       _readOptions[1],
				       _readOptions[2],
				       _readOptions[3] ) );
	break;
      }
    
    return *this;
  }
  
  virtual TurboSimBuffer &operator>>( std::string &str ) { 
    clearErrors();
    if ( not ( _mode & READ ) ) {
      _successfulRead = false;
      return *this;
    }
    _successfulRead = ( *_source >> str );
    return *this;
  }

  
  
  // Error Accessors
  void clearErrors() {
    _successfulRead = _successfulWrite = true;
  }
  
  bool successfulRead() const {
    return _successfulRead;
  }

  bool successfulWrite() const {
    return _successfulWrite;
  }

  operator bool() const {
    return _successfulWrite && _successfulRead;
  }


  // Other Accessors
  int mode() const {
    return _mode;
  }

  // Read and Write format modifiers
  void setReadOptions( int opt1 = - 1,
		       int opt2 = -1,
		       int opt3 = -1,
		       int opt4 = -1 ) {
    _readOptions.clear();
    if ( opt1 >= 0 )
      _readOptions.push_back( opt1 );
    if ( opt2 >= 0 )
      _readOptions.push_back( opt2 );
    if ( opt3 >= 0 )
      _readOptions.push_back( opt3 );
    if ( opt4 >= 0 )
      _readOptions.push_back( opt4 );
  }
  
  void setWriteOptions( int opt1 = - 1,
			int opt2 = -1,
			int opt3 = -1,
			int opt4 = -1 ) {
    _writeOptions.clear();
    if ( opt1 >= 0 )
      _writeOptions.push_back( opt1 );
    if ( opt2 >= 0 )
      _writeOptions.push_back( opt2 );
    if ( opt3 >= 0 )
      _writeOptions.push_back( opt3 );
    if ( opt4 >= 0 )
      _writeOptions.push_back( opt4 );
  }
  
 
protected:

  // Constructors
  TurboSimBuffer( ) 
    : _source( 0 ),
      _dest( 0 ),
      _mode( NOT_OPEN ),
      _successfulRead( true ), 
      _successfulWrite( true ) { ;} 
  
  TurboSimBuffer( std::istream *source,
		  std::ostream *dest ) 
    : _mode( NOT_OPEN ),
      _successfulRead( true ),
      _successfulWrite( true ) {
    setDest( dest );
    setSource( source );
  } 


  TurboSimBuffer &setSource( std::istream *source ) {
    if ( not source ) {
      if ( _mode & READ )
	_mode ^= READ;
      _successfulRead = false;
      _source = NULL;
    }
    else {
      if ( not ( _mode & READ ) )
	_mode ^= READ;
      _successfulRead = true;
      _source = source;
    }
    return *this;
  }
  
  TurboSimBuffer &setDest( std::ostream *dest ) {
    if ( not dest ) {
      if ( _mode & WRITE ) 
	_mode ^= WRITE;
      _successfulWrite = false;
      _dest = NULL;
    }
    else {
      if ( not ( _mode & WRITE ) ) 
	_mode ^= WRITE;
      _successfulWrite = true;
      _dest = dest;
    }
    return *this;
  }

  void clear() {
    _source = NULL;
    _dest = NULL;
    _mode = NOT_OPEN;
    clearErrors();
  }



private:
  
  std::istream *_source;
  std::ostream *_dest;
  int _mode;
  bool _successfulRead, _successfulWrite;
  std::vector< int > _readOptions;
  std::vector< int > _writeOptions;

};

#endif
