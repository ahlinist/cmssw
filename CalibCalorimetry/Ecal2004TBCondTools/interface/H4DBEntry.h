//! H4DBEntry.h
//!
//! Interface class for classes which would like to be handled by
//! the H4DBManager class.
//!
//! last change : $Date: 2003/09/04 12:15:57 $
//! by          : $Author: koblitz $
//!
//!

#ifndef H4DBENTRY_H
#define H4DBENTRY_H

#ifdef HAVE_CONFIG_H
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/config.h"
#endif

#include <iostream>

#include "TObject.h"

#ifdef WITH_MYSQL
#include "TSQLServer.h"
#endif

class H4DBEntry: public TObject{
 public:
  enum IndexMatch {indexOnly, sloppy, exact};
  enum Subindices {NoSuchSubindex = -100};

  H4DBEntry();
  virtual ~H4DBEntry() {};
#ifdef WITH_MYSQL
  virtual int fillFromMySQL(TSQLServer *db, int index,
			    int subIndex =-1, IndexMatch match=exact) = 0;
#endif
  
  //! read itself from an ASCII stream. 
  //! (see H4DBManager::readAsciiFile())
  virtual int fillFromAsciiFile(istream& ifs) 
    { std::cout << "H4DBEntry::fillFromAsciiFile() not implemented!" 
                << std::endl;
      return 0;
     } 
  
  //! read itself from an ASCII stream. 
  //! (see H4DBManager::readAsciiFile())
  virtual int dumpToAsciiFile(ostream&) const 
    { std::cout << "H4DBEntry::dumpToAsciiFile() not implemented!" 
                << std::endl;
      return 0;
    } 
  
  virtual void print(Option_t *option="") const;

  /// Returns the current index for which this DB-Entry is valid
  /// (run, crystal...)
  virtual int getIndex() const = 0;

  /// Returns the current sub-index for which this DB-Entry is valid
  /// (sub-run, event...)
  virtual int getSubIndex() const = 0;

  /// Returns the minimum subindex for the major index with which this
  /// Object was tried to be read out of the database
  virtual int getMinSubIndex() const;

  /// Returns the maximum subindex for the major index with which this
  /// Object was tried to be read out of the database
  virtual int getMaxSubIndex() const;

  /// Was the object correctly initialized?
  int isValid() const;

  IndexMatch getMatchMethod() const { return matchMethod; }

 protected:
  bool valid; ///< true, if the object was properly initialized
  
  int  minSubIndex; //! The smallest subindex for a given major index
  int maxSubIndex; //! The larges subindex for a given major index
  IndexMatch matchMethod; //!< How indices are matched for the entry
 private:
  ClassDef(H4DBEntry,1) // base class for DB objects
};

// H4DBENTRY_H
#endif
