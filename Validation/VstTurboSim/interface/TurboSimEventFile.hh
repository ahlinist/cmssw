#ifndef __TurboSimEventFile_h
#define __TurboSimEventFile_h


#include <string>
#include<iostream>

#include "Validation/VstTurboSim/interface/TurboSimFile.hh"
#include "Validation/VstTurboSim/interface/TurboSimEvent.hh"
#include "Validation/VstTurboSim/interface/TurboSimMultiFile.hh"

class TurboSimEventFile : private TurboSimMultiFile< int >  {

public:

   enum { 
      PARTON_FILE,
      RECO_FILE 
   };

   TurboSimEventFile( ) {
      if ( not TurboSimMultiFile< int >::addFile( PARTON_FILE, 
						  new TurboSimFile() ) )
	 std::cerr << "TurboSimEventFile constructor: cannot create "
	      << "parton file object.";
      if ( not TurboSimMultiFile< int >::addFile( RECO_FILE,
						  new TurboSimFile() ) )
	 std::cerr << "TurboSimEventFile constructor: cannot create "
	      << "reco file object.";
   }

   TurboSimEventFile( const std::string &partonFile,
		      const std::string &recoFile,
		      int mode ) {
      TurboSimEventFile();
      if ( mode & TurboSimBuffer::READ )
	 openFilesForInput( partonFile, recoFile );
      if ( mode & TurboSimBuffer::WRITE )
	 openFilesForOutput( partonFile, recoFile );
   }

   ~TurboSimEventFile() {
      TurboSimMultiFile< int >::clearFiles();
   }

   TurboSimEventFile &operator<<( const TurboSimEvent &event ) {
      if ( partonFile()->mode() & TurboSimBuffer::WRITE )
	 *partonFile() << event;
      if ( recoFile()->mode() & TurboSimBuffer::WRITE ) {
	 *recoFile() << event;
      }
      return *this;
   }

   TurboSimEventFile &operator<<( const std::string &str ) {
      if ( partonFile()->mode() & TurboSimBuffer::WRITE )
	 *partonFile() << str;
      if ( recoFile()->mode() & TurboSimBuffer::WRITE ) 
	 *recoFile() << str;
      return *this;
   }      

   TurboSimEventFile &operator>>( TurboSimEvent &event ) {
      if ( partonFile()->mode() & TurboSimBuffer::READ ) 
	 *partonFile() >> event;
      if ( recoFile()->mode() & TurboSimBuffer::READ ) 
	 *recoFile() >> event;
      return *this;
   }

   TurboSimEventFile &operator>>( std::string &str ) {
      if ( partonFile()->mode() & TurboSimBuffer::READ ) 
	 *partonFile() >> str;
      if ( recoFile()->mode() & TurboSimBuffer::READ ) 
	 *recoFile() >> str;
      return *this;
   }

   // Read and Write format modifiers
   void setReadOptions( bool partonPurality,
			bool recoPurality,
			int particleFormat = 
			TurboSimParticle::DEFAULT ) {
      if ( partonPurality )
	 partonFile()->setReadOptions( particleFormat,
				       TurboSimEvent::EVENT_DATA |
				       TurboSimEvent::PARTON_OBJS );
      else
	 partonFile()->setReadOptions( particleFormat,
				       TurboSimEvent::EVENT_DATA |
				       TurboSimEvent::PARTON_OBJ );

      if ( recoPurality )
	 recoFile()->setReadOptions( particleFormat,
				     TurboSimEvent::EVENT_DATA |
				     TurboSimEvent::RECO_OBJS );
      else
	 recoFile()->setReadOptions( particleFormat,
				     TurboSimEvent::EVENT_DATA |
				     TurboSimEvent::RECO_OBJ );
   }

   void setWriteOptions() {}

   // File Operations
   bool openFilesForInput( const std::string &pFile,
			   const std::string &rFile ) {
      if ( ( partonFile()->openFileForInput( pFile ) ) &&
	   ( recoFile()->openFileForInput( rFile ) ) ) {
      	 setReadOptions( true, true );
	 return true;
      }
      return false;
   }

   bool openFilesForOutput( const std::string &pFile,
			    const std::string &rFile ) {
      if ( partonFile()->openFileForOutput( pFile ) &&
	   recoFile()->openFileForOutput( rFile ) ) {
	 setWriteOptions();
	 return true;
      }
      return false;
   }

   bool is_open() const {
      return ( partonFile()->is_open() && 
	       recoFile()->is_open() );
   }

   TurboSimEventFile &close() {
      recoFile()->close();
      partonFile()->close();
   }

   // Error accessors

   bool successfulRead() const {
      return ( partonFile()->successfulRead() && 
	       recoFile()->successfulRead() );
   }

   bool successfulWrite() const {
      return ( partonFile()->successfulWrite() &&
	       recoFile()->successfulWrite() );
   }

   void clearErrors( ) {
      partonFile()->clearErrors();
      recoFile()->clearErrors();
      
   }

   operator bool() const {
      return ( *recoFile() && *partonFile() );
   }

private:

   TurboSimFile *partonFile() {
      return TurboSimMultiFile<int>::getFile( PARTON_FILE );
   }

   TurboSimFile *recoFile() {
      return TurboSimMultiFile<int>::getFile( RECO_FILE );
   }

   const TurboSimFile *partonFile() const {
      return TurboSimMultiFile<int>::getFile( PARTON_FILE );
   }

   const TurboSimFile *recoFile() const {
      return TurboSimMultiFile<int>::getFile( RECO_FILE );
   }

};

#endif
