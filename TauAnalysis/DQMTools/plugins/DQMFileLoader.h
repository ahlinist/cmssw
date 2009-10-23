#ifndef TauAnalysis_DQMTools_DQMFileLoader_h
#define TauAnalysis_DQMTools_DQMFileLoader_h

/** \class DQMFileLoader
 *  
 *  Class to load DQM monitoring elements from ROOT files into DQMStore
 *
 *  $Date: 2009/08/17 16:07:21 $
 *  $Revision: 1.4 $
 *  \author Christian Veelken, UC Davis
 */

// framework & common header files
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DQMServices/Core/interface/DQMDefinitions.h"

#include <TH1.h>

#include <vector>
#include <string>

class DQMFileLoader : public edm::EDAnalyzer
{
  typedef std::vector<std::string> vstring;
  typedef std::set<std::string> sstring;

  struct cfgEntryFileSet
  {
    cfgEntryFileSet(const std::string&, const edm::ParameterSet&);
    void print() const;
    std::string name_;
    vstring inputFileNames_;
    double scaleFactor_;
    std::string dqmDirectory_store_;
    int cfgError_;
  };

 public:
  explicit DQMFileLoader(const edm::ParameterSet&);
  virtual ~DQMFileLoader();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();  

private:
  std::string inputFilePath_;
  std::map<std::string, cfgEntryFileSet> fileSets_;
  std::map<std::string, sstring> subDirectoryMap_;
  int cfgError_;
};

#endif


