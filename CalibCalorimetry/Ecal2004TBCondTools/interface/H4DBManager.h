// -*- C++ -*- 
// 
// H4DBManager.h
//
// Manager class for Conversion of MySQL H4 Beamline info to
// a RootDB and also for read-access to this RootDB.
//
// last change : $Date: 2003/08/26 06:15:41 $
// by          : $Author: nrl $
//

#ifndef H4DBMANAGER_H
#define H4DBMANAGER_H

#ifdef HAVE_CONFIG_H
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/config.h"
#endif

#include "TObject.h"
#include "TFile.h"
#include "TTree.h"
#include "TClass.h"

#ifdef WITH_MYSQL
#include "TSQLServer.h"
#endif

#include <string>
#include <list>

using namespace std;

#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4DBEntry.h"

class H4DBManager: public TObject{
 public:
  //! Default Constructor needed by Root
  H4DBManager();

  //! Constructor with information on RootDB-file and/or the MySQL-Database.
  H4DBManager(bool write,
	      const char *rootdb="h4rootdb.root",
	      const char *server="", // e.g. mysql://pcroot.cern.ch:3456/test
	      const char *userName="",
	      const char *passwd="");

  //! Destructor: Closes file and possible DB-Access
  virtual ~H4DBManager();

  //! Connect to MySQL-database
  bool connect(const char *server, const char *userName,
	       const char *passwd);

  //! Open Root-DB file
  bool openDB(bool write=false,
	      const char *rootdb="h4rootdb.root");
  
  //! Open the ASCII file specified in argument.
  //! Try to interpret its contents as a collection of objects of the same class,
  //! deriving from H4DBEntry. Read these objects into a TTree managed by H4DBManager.
  //! Updates the internal list of managed entries.
  bool readAsciiFile(string const& filename);
  
  //! Update an Object
  int update(int index, int subIndex= H4DBEntry::NoSuchSubindex,
	     H4DBEntry *object=0);

  //! Get an Object for a certain index
  H4DBEntry *getEntry(const char *entryName, int index,
		      int subIndex=H4DBEntry::NoSuchSubindex);

  /// Writes out all managed entries to the rootDB
  bool writeToFile();

 private:

  //! Structure to store some information belonging to each managed H4DBEntry:
  struct H4DBManagedEntry {
    TTree* tree;           //!< trees of different objects on file
    H4DBEntry* object;     //!< objects of different classes in memory
    int index;             //!< current index for the different objects
    int subIndex;          //!< current subIndex for the different objects
  };

  //! Get a managed object if alreay in the list
  H4DBManagedEntry* getFromList(const TClass *dbEntryClass) const;

  //! Creates new DB Entry in read and write mode
  H4DBEntry* createDBEntry(const TClass *dbEntryType, int index, int subIndex);

  //! A helper function to convert a class name string ito a TClass pointer
  TClass* getDBEntryClassByName(const char *className) const;

  TFile *rootDBFile;      //!< Name of root-File
#ifdef WITH_MYSQL
  TSQLServer *myServer;   //!< The SQL server object if in SQL mode
#endif
  bool writeMode;         //!< Whether we are writing out to the root-file

  list<H4DBManagedEntry *> managedEntries; //! list of managed objects

  //! For convinience a MEIterator type is defined which walks along
  //! the list of H4DBManagedEntry pointers
  typedef list<H4DBManagedEntry *>::const_iterator MEIterator;

  ClassDef(H4DBManager,0) //!< The Class is not persistent
};


// Do not forget to define the global variables so that rootcint
// produces the necessary stub
R__EXTERN const char *gH4RootDBVersion;

#endif // #ifndef H4DBMANAGER_H
