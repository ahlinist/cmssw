// -*- C++ -*-
// 
// File H4DBManager.C
//
/*! \class H4DBManager
 * \brief Manager class for Conversion of MySQL H4 Beamline info to
 *  a RootDB and also for read-access to this RootDB.
 *
 * $Date: 2004/12/17 10:03:32 $
 * $Author: brunelie $
 *
 * The class H4DBManager is used to manage objects deriving from
 * H4DBEntry which contain run-dependent information for the H4
 * beam-tests. This information can either be read directly from the
 * MySQL database or a root database. In addition, the H4DBManager can be
 * used to write out objects into a new root database. After the
 * initialization of H4DBManager the source of the data is invisible to
 * the user and a unified interface is presented.
 * 
 * The manager is started by creating an instance of it:
 * \code
 *   H4DBManager(bool write = false,
 *	      const char *rootdb="h4rootdb.root",
 *	      const char *server="",
 *	      const char *userName="",
 *	      const char *passwd="");
 * \endcode
 * Basically three modes of operation are forseen:
 *  -# Read from rootdb:
 *   set write to false and give the name of the respective root-db file.
 *  -# Read from the MySQL-database: Set write to false, do not
 *    give andy filename ("") but set the parameters for the MySQL
 *    database correctly. The server thereby is of the format
 *    <tt> mysql://pcroot.cern.ch:3456/test </tt>
 *    (open db test on pcroot using port
 *    3456). The default port of MySQL is 3306.
 *  -# Copy mode. Open file for writing (write=true) and give MySQL
 *    parameters. Every entry of the db you open will automatically be
 *    copied to the root-db file.
 *
 * Example:
 * \code
 *  // Open root-file for read access
 *  H4DBManager myManager(false,"myrootdb.root");
 *  
 *  // Loop over 4 runs 49000 to 49103
 *  for (int i= 49000; i<49104; i++){
 *  // Get an entry for the respective run of the type H4Run
 *    H4DBEntry *myRunEntry=myManager.getEntry("H4Run", i);
 *  // Print info on it
 *    myRunEntry->print();
 *  }
 * \endcode
 * It is important to note that the object myRunEntry is owned by
 * myManager. In fact, the same object is returned all the time after
 * updating for the respective run. If NULL is returned by
 * getEntry(), then either no proper entry of an ambiguous entry was
 * found for the run (the run contained sub-runs). If you are
 * interested in the subrun 3 of run 49999 then you could say for
 * example <tt>H4DBEntry *myRunEntry=myManager.getEntry("H4Run", 49999,3);
 * </tt>
 * to get this sub-run.
 *
 * To get information on how many subruns exist, you may use
 * /code int n=myManager.update(49999,H4DBEntry::NoSuchSubindex,myRunEntry);
 * /endcode
 * If a single entry exists, 1 is returned, otherwise the number of
 * sub-runs or 0 if no entry was found for this run.
 *
 * If you would like to create your own copy of the database, you
 * should look into h4rootdb.cpp .
*/

#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4DBManager.h"
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4DBEntry.h"
#include "TROOT.h"
#include "TClassTable.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>

ClassImp(H4DBManager) ;

const char *gH4RootDBVersion=VERSION;

H4DBManager::H4DBManager(){
  writeMode=false;
  rootDBFile=0;
#ifdef WITH_MYSQL
  myServer = 0;
#endif  
}


H4DBManager::H4DBManager(bool write,
	      const char *rootdb, const char *server,
	      const char *userName, const char *passwd)
{
  writeMode=write;
  rootDBFile = 0;
#ifdef WITH_MYSQL
  myServer = 0;
#endif  

  // Connect to SQL
  if (strlen(server)) {
#if WITH_MYSQL
    if(!connect(server,userName,passwd)) {
      std::cout << "H4DBManager::H4DBManager: Could not open MySQL-Database"
		<< std::endl << "Aborting!" << std::endl;
      abort();
    }
#else
    std::cout << "H4DBManager::H4DBManager: Write mode requires MySQL"
	      << std::endl << "You need to relink this program!" << std::endl
	      << "Aborting!" << std::endl;
    abort();
#endif
  }

  // Open the rootdb file for input or output only if
  // Filename is not given
  if(rootdb){
    if (!openDB(writeMode, rootdb)){
      std::cout << "H4DBManager::H4DBManager: Aborting!" << std::endl;
      abort();
    }    
  }    
}


H4DBManager::~H4DBManager()
{
  if(writeMode) writeToFile();

  if(rootDBFile){
    rootDBFile->Close();
    delete rootDBFile;
    rootDBFile=0;
  }
#if WITH_MYSQL
  if(myServer){
    myServer->Close();
    delete myServer;
    myServer=0;
  }
#endif
}

 
bool H4DBManager::connect( const char *server, const char *userName,
			   const char *passwd)
{
#ifdef DEBUG
  std::cout << "H4DBManager::connect: Connecting to server " << server
	    << " as " << userName << " using passwd " << passwd << std::endl;
#endif

#if WITH_MYSQL
  myServer=TSQLServer::Connect(server, userName, passwd);
  if(!myServer){
    std::cout << "H4DBManager::connect: Could not open connection to server "
	      << server << "!" << std::endl;
    return false;
  }
  return true;
#else
  std::cout << "H4DBManager::connect: MySQL functionality not available!" << std::endl
	    << "Could not open server connection!" << std::endl;
  return false;
#endif
}


bool H4DBManager::openDB(bool write, const char *rootdb)
{
  TDirectory* currentDir = gDirectory;
  
  // --nrl 2003/08/27 writeMode must be set
  writeMode = write;
  
  if(write){
    rootDBFile=new TFile(rootdb,"RECREATE");
  } else {
    rootDBFile=new TFile(rootdb,"READ");
  }
  if (!rootDBFile){
    std::cout << "H4DBManager::openDB: Could not open file "
	      << rootdb << std::endl;
    gDirectory = currentDir; // to go back to the old directory!
    return false;
  }
  gDirectory = currentDir; // to go back to the old directory!
  return true;  
}


/** 
   The format of the file must follow the following rules:
     -# the class name must appear on the first line, preceded with the keyword "class"
     -# each line contains the values of the class members,
        as expected by the method H4DBEntry::fillFromAsciiFile(ifstream& ifs) 
     -# one object per line
     -# blank lines are not allowed
     -# comment lines must begin with '#'

   For example, if we define the class H4Example below:
   \code
   class H4Example : public H4DBEntry {
   public:
     H4Example();
     ~H4Example();
     ...
     virtual int fillFromAsciiFile(ifstream*);
     ...
   private:
     int a;    //!< first member 
     double b; //!< another member
   };
   \endcode
   
   A valid ASCII file could look like:
   
   \verbatim
   class H4Example
   1242 132432.432
   # This is a comment... 
   5743 4382.354
   5933 9323.354
   \endverbatim
*/
bool H4DBManager::readAsciiFile(string const& filename)
{
  ifstream ifs(filename.c_str());
  if( !ifs.is_open() ) {
    std::cout << "H4DBManager::readAsciiFile ERROR unable to open file="
	      << filename << std::endl;
    return false;
  }
  
  char buff[8192];
  string inputstr, className;
  ifs.getline(buff, 8192);
  
  // class name ? 
  std::stringstream sstrm(buff);
  sstrm >> inputstr;
  if( inputstr != "class") {
    std::cout << "H4DBManager::readAsciiFile ERROR with file format!  " << std::endl
	      << "first line should contain the word 'class' followed " << std::endl
	      << "by the name of the class of the objects stored in the file" << std::endl;
    return false;
  }
  else 
    sstrm >> className;
  
  // check that this class is known to us
  TClass* dbEntryType = getDBEntryClassByName(className.c_str());
  if(!dbEntryType) {
    std::cout << "H4DBManager::readAsciiFile ERROR, class=" << className
	      << " is unknown. Giving up." << std::endl;
    return false;
  }
  
  // maybe the class is already registered with managedEntries
  H4DBEntry* entry=0;
  H4DBManagedEntry* me = getFromList(dbEntryType);
  
  // if not, make sure that the class derives from H4DBEntry and create a new entry
  if(!me) {
    if(!dbEntryType->InheritsFrom(H4DBEntry::Class())) {
      std::cout << "H4DBManager::readAsciiFile: class=" << className
		<< " does not seem to derive from H4DBEntry."
		<< " Giving up." << std::endl;
      return false;
    }
    entry = createDBEntry(dbEntryType, 0, 0);
    if(!entry) {
      std::cout << "H4DBManager::readAsciiFile: ERROR creating new Entry for class="
		<< className << ". Giving up." << std::endl;
      return false;
    }
    me = getFromList(dbEntryType);
  }
  
  // loop over all the entries and update the tree
  while( ifs.good() ) {
    entry->fillFromAsciiFile(ifs);
    me->index = entry->getIndex();
    me->subIndex = entry->getSubIndex();
    me->tree->Fill();
  }
  
  ifs.close();
}


int H4DBManager::update(int index, int subIndex, H4DBEntry *object)
{  
  // Returns 1 in case everything fine. -1 if no entry found and the
  // number of entries if more than one entry was found.

#ifdef DEBUG
  std::cout << "H4DBManager::update: index " << index << " subIndex "
	    << subIndex << std::endl;
#endif

  MEIterator I = managedEntries.begin();
  while (I != managedEntries.end()){
    H4DBEntry *obj = (*I)->object;

    // if object given, just treat this one
    if(object && obj != object) { I++; continue;}
#ifdef WITH_MYSQL
    if(myServer){
      int nSubIndices;

      if(writeMode)
	nSubIndices=obj->fillFromMySQL(myServer, index, subIndex);
      else
	nSubIndices=obj->fillFromMySQL(myServer, index,
				       subIndex, H4DBEntry::sloppy);
      if(nSubIndices != 1) return nSubIndices;
      (*I)->index=obj->getIndex();
      (*I)->subIndex=obj->getSubIndex();
    }else{
#endif
      int entry;
      switch(obj->getMatchMethod()){
      case H4DBEntry::exact:
	if (gROOT->GetVersionInt() < 40008)
	  entry = (*I)->tree->GetEntryNumberWithIndex(index, subIndex);
	else
	  entry = (*I)->tree->GetEntryNumberWithIndex(subIndex, index);
	if (entry < 0 && 
	    (std::string(obj->GetName()).insert(0,"Is").find("H4ShapeParam")
	     == 2 ||
	     std::string(obj->GetName()).insert(0,"Is").find("H4TdcRange") 
	     == 2 || 
	     std::string(obj->GetName()).insert(0,"Is").find("H4PnLinearity") 
	     == 2)) { 
	  // Necessary modification for H4ShapeParam
	  if (gROOT->GetVersionInt() < 40008) {
	    double* indexValues = (*I)->tree->GetIndexValues();
	    entry = (*I)->tree->GetEntryNumberWithBestIndex(index, index);
	    int newIndex = (int)indexValues[entry];
	    entry = (*I)->tree->GetEntryNumberWithIndex(newIndex, subIndex);
	  } else {
	    entry = (*I)->tree->GetEntryNumberWithBestIndex(subIndex, index);
	  }
	}
	break;
      case H4DBEntry::sloppy:
	if (gROOT->GetVersionInt() < 40008)
	  entry = (*I)->tree->GetEntryNumberWithBestIndex(index, subIndex);
	else
	  entry = (*I)->tree->GetEntryNumberWithBestIndex(subIndex, index);
	break;
      case H4DBEntry::indexOnly:
#ifdef DEBUG
	std::cout << "Checking only index !" << std::endl;
#endif
	entry = (*I)->tree->GetEntryNumberWithBestIndex(index, 0);
	break;
      default:
	std::cout << "Something very wrong has happend: bailing out!" << std::endl;
	abort();
      }
      if (entry < 0) {
#ifdef DEBUG
	std::cout << "H4DBManager::update: No valid index found for type "
		  << obj->GetName() << " at index " << index << "!" << std::endl;
#endif
	return -1;
#ifdef WITH_MYSQL
      } 
#endif

#ifdef DEBUG
      std::cout << "I will take entry: " << entry << std::endl;
#endif
      if((*I)->tree->GetEntry(entry)){
#ifdef DEBUG
	obj->Print();
#endif
      } else {
	std::cout << "H4DBManager::update: No valid index found for type "
		  << obj->GetName() << " in index " << index << "!" 
		  << std::endl;
	return -1;
      }
    }
    if(writeMode) (*I)->tree->Fill();
    I++;
  }
  return 1;
}

H4DBEntry *H4DBManager::getEntry(const char *entryName, int index,
				 int subIndex)
{
  //! Returns a pointer to the managed instance of type 'H4DBEntry'
  //! Entries later can be updated by calling H4DBManager::update(int index)
  //! If necessary an instance is created, but you must not delete it.

#ifdef DEBUG
  std::cout << "Trying to get " << entryName << " object for index " 
	    << index << "." << std::endl;
#endif

  // Is the entry already in our list?

  TClass *dbEntryType = getDBEntryClassByName(entryName);

  H4DBManagedEntry* me = getFromList(dbEntryType);
  if(me){
    update(index, subIndex, me->object); // ???????????
    return me->object;
  }

  if (!dbEntryType || !dbEntryType->InheritsFrom(H4DBEntry::Class())) {
    std::cout << "H4DBManager::getEntry: " << "Unknown object " << entryName 
	      << std::endl << "Returning NULL!" << std::endl;
    return NULL;
  }

  // Ok, let's create a new object:
  H4DBEntry *entry=createDBEntry(dbEntryType, index, subIndex);
  if (!entry) return entry;
  if(update(index,subIndex,entry) == -1) return NULL; 
  return entry;
}

H4DBEntry* H4DBManager::createDBEntry(const TClass *dbEntryType, int index,
				      int subIndex)
{
  //! Private method to create a new db entry managed by the H4DBManager
  //! class. Called by get Entry.

#ifdef DEBUG
  std::cout << "createDBEntry " << dbEntryType->GetName() << " object for index " 
	    << index << "." << std::endl;
#endif

  H4DBManagedEntry *me = new H4DBManagedEntry;
  me->object = static_cast<H4DBEntry *>
    (const_cast<TClass*>(dbEntryType)->New() );

  if(writeMode){                // Write mode
    managedEntries.push_back(me);

    // create tree and branch on the rootDB file
    TDirectory* currentDir = gDirectory;
    rootDBFile->cd(); // to create the tree in the file...
    me->tree = new TTree(dbEntryType->GetName(), dbEntryType->GetName());
    me->tree->Branch(dbEntryType->GetName(), dbEntryType->GetName(), 
		     &me->object, 128000, 0);
  
    me->tree->Branch("Index",&me->index,"Index/I",64000);
    me->tree->Branch("SubIndex",&me->subIndex,"SubIndex/I",64000);

#ifdef DEBUG
    std::cout << "Added " << dbEntryType->GetName() << " in RootDB." << std::endl;
#endif

    currentDir->cd(); // go back to old directory
 
    return me->object;
  }

  //                               Read mode
  if(!rootDBFile){  // Reading from DB, no file 
    H4DBEntry* result = me->object;
    managedEntries.push_back(me);
    return result;
  }

  // Reading from file
  me->tree = static_cast<TTree*>(rootDBFile->Get(dbEntryType->GetName()));
  if (gROOT->GetVersionInt() >= 40008) // Root version > 4.00.08
    if (std::string(dbEntryType->GetName()).insert(0,"Is").
	find("H4ShapeParam") == 2 ||
	std::string(dbEntryType->GetName()).insert(0,"Is").
	find("H4TdcRange") == 2 ||
	std::string(dbEntryType->GetName()).insert(0,"Is").
	find("H4PnLinearity") == 2 ||
	std::string(dbEntryType->GetName()).insert(0,"Is").
	find("H4Gains") == 2)
      me->tree->BuildIndex("SubIndex","Index");
    else
      me->tree->BuildIndex("Index","SubIndex");
  if (me->tree) {
#ifdef DEBUG
    std::cout << "Found a tree with Object-Name!" << std::endl;
#endif
    TBranch *bb = me->tree->GetBranch(dbEntryType->GetName());
    if (bb) {
#ifdef DEBUG
      std::cout << "Found a branch with Object-Name!" << std::endl;
#endif
      bb->SetAddress(&me->object);
      me->tree->GetBranch("Index")->SetAddress(&me->index);
      me->tree->GetBranch("SubIndex")->SetAddress(&me->subIndex);
    } else {
      std::cout << "H4DBManager::createDBEntry: No branch found for class "
		<< dbEntryType->GetName() << std::endl;
    }
  } else {
    std::cout << "H4DBManager::createDBEntry: No branch found for class "
	      << dbEntryType->GetName() << std::endl;
  }

  H4DBEntry* result = me->object;
  managedEntries.push_back(me);

  return result;
}

TClass* H4DBManager::getDBEntryClassByName(const char *className) const
{
  // returning a pointer to the class 'className' if inherits from H4DBEntry

  TClass* c = gROOT->GetClass(className);
  if (!c || !c->InheritsFrom(H4DBEntry::Class())) {
    std::cout << "H4DBManager::getDBEntryClassByName: Class "
	      <<  className << " cannot be managed!" << std::endl;
  }
  return c;
}


bool H4DBManager::writeToFile()
{
  //! This routine writes out all managed database entries to the attached
  //! root file if in write mode

  // #ifdef DEBUG
  std::cout << "H4DBManager::writeToFile: Creating index and writing out file!"
	    << std::endl;
  // #endif

  if(!writeMode){
    std::cout << "H4DBManager::WriteToFile: Not in write mode!" << std::endl;
    return false;
  }

  for (MEIterator I = managedEntries.begin(); I != managedEntries.end(); I++){
    H4DBManagedEntry *me=*I;
    me->tree->BuildIndex("Index","SubIndex");
  }
  rootDBFile->Write();

  return kTRUE;
}


H4DBManager::H4DBManagedEntry* H4DBManager::getFromList(
				   const TClass *dbEntryClass) const
{
  //! looks for an object of type 'dbentryClass' in list of managed objects
  //! if it is already in ==> return its H4DBManagedEntry

  MEIterator I = managedEntries.begin();
  while (I != managedEntries.end()){
    H4DBEntry *obj = (*I)->object;
    if (obj->IsA() == dbEntryClass) return (*I);
    I++;
  }
  return NULL;
}
    
