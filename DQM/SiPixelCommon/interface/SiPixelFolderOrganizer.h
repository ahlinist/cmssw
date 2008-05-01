#ifndef SiPixelCommon_SiPixelFolderOrganizer_h
#define SiPixelCommon_SiPixelFolderOrganizer_h
// -*- C++ -*-
//
// Package:     SiPixelCommon
// Class  :     SiPixelFolderOrganizer
// 
/**\class SiPixelFolderOrganizer SiPixelFolderOrganizer.h DQM/SiPixelCommon/interface/SiPixelFolderOrganizer.h
   
Description: <Organizes the folders for the monitoring elements of the Pixel detector. Its methods return strings with names of folders to be created and used.>

Usage:
<usage>

*/
//
// Original Author:  chiochia
//         Created:  Thu Jan 26 23:49:46 CET 2006
// $Id: SiPixelFolderOrganizer.h,v 1.3 2008/03/01 20:19:44 lat Exp $
#include "DQMServices/Core/interface/DQMStore.h"
#include <boost/cstdint.hpp>
#include <string>

class SiPixelFolderOrganizer {
  
 public:

  /// Constructor
  SiPixelFolderOrganizer();

  /// Destructor
  virtual ~SiPixelFolderOrganizer();
  
  /// Set folder name for a module or plaquette
  bool setModuleFolder(const uint32_t& rawdetid=0);
  void getModuleFolder(const uint32_t& rawdetid, std::string& path);

  /// Set folder name for a FED (used in the case of errors without detId)
  bool setFedFolder(const uint32_t FedId);
  
 private:

  std::string rootFolder;
  std::string slash;
  DQMStore* dbe_;
};
#endif
