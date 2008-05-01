/**
 * Abstract base class for <code>TurboSim</code> classes that
 * have data that must be stored and read from an external source. 
 *
 * All TurboSim classes must implement: 
 * <pre>
 *       toString( )
 *       read( . . . )
 * </pre>
 * This is useful when interacting with files: all classes must
 * 'know' how to read and write themselves from and to external
 * storage. For instance, a <code>TurboSimParticle</code> can 
 * produce a string to save itself to a file, and also load itself 
 * from a file stream.
 *
 * @author David Friend, friend at mit dot edu
 * @version 2005.07.02
 *
 */

#ifndef __TurboSimSaveableItem_h
#define __TurboSimSaveableItem_h

#include <string>
#include <iostream>
#include "Validation/VstTurboSim/interface/TurboSimItem.hh"

class TurboSimSaveableItem : public TurboSimItem {
   
public:
   
   virtual ~TurboSimSaveableItem() {}
   
   virtual std::string toString() const = 0;
   virtual std::string toString( int ) const { return "TSSaveableItem: error1"; }
   virtual std::string toString( int, 
				 int ) const { return "TSSaveableItem: error2"; }
   virtual std::string toString( int, 
				 int,
				 int ) const { return "TSSaveableItem: error3"; }
   virtual std::string toString( int, 
				 int, 
				 int, 
				 int ) const { return "TSSaveableItem: error4"; }
   
   virtual bool read( std::istream& ) = 0;
   virtual bool read( std::istream&,
		      int ) { return false; }
   virtual bool read( std::istream&,
		      int,
		      int ) { return false; }
   virtual bool read( std::istream&,
		      int,
		      int, 
		      int ) { return false; }
   virtual bool read( std::istream&,
		      int,
		      int, 
		      int, 
		      int ) { return false; }
   
};

#endif
