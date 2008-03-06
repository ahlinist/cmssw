#ifndef DQM_SiStripCommissioningClients_CommissioningHistograms_H
#define DQM_SiStripCommissioningClients_CommissioningHistograms_H

#define USING_NEW_COLLATE_METHODS

#include "DataFormats/SiStripCommon/interface/SiStripConstants.h"
#include "DataFormats/SiStripCommon/interface/SiStripHistoTitle.h"
#include "DataFormats/SiStripCommon/interface/SiStripEnumsAndStrings.h"
#include "DQM/SiStripCommissioningSummary/interface/CommissioningSummaryFactory.h"
#include "DQM/SiStripCommon/interface/ExtractTObject.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#ifndef USING_NEW_COLLATE_METHODS
#include "DQMServices/Core/interface/CollateMonitorElement.h"
#endif
#include "DQMServices/Core/interface/DQMOldReceiver.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include <boost/cstdint.hpp>
#include "TProfile.h"
#include "TH1.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

class MonitorElement;
class CommissioningAnalysis;

class CommissioningHistograms {

 public:

  // ---------- con(de)structors ----------
  
  CommissioningHistograms( DQMOldReceiver* const,
			   const sistrip::RunType& );
  
  // DEPRECACTE
  CommissioningHistograms( DQMStore* const,
			   const sistrip::RunType& );

  // MAKE PRIVATE
  CommissioningHistograms(); // private constructor

  virtual ~CommissioningHistograms();

  // ---------- histogram container class ----------
  
  class Histo {
  public:
#ifdef USING_NEW_COLLATE_METHODS
    Histo( const std::string& title, 
	   MonitorElement* const me,
	   MonitorElement* const cme ) 
      : title_(title), me_(me), cme_(cme) {;}
#else
    Histo( const std::string& title, 
	   MonitorElement* const me,
	   CollateMonitorElement* const cme ) 
      : title_(title), me_(me), cme_(cme) {;}
#endif
    Histo() : title_(""), me_(0), cme_(0) {;}
    void print( std::stringstream& ) const;
    std::string title_;
    MonitorElement* me_;
#ifdef USING_NEW_COLLATE_METHODS
    MonitorElement* cme_;
#else
    CollateMonitorElement* cme_;
#endif
  };
  
  // ---------- typedefs ----------
  
  typedef std::map<uint32_t,CommissioningAnalysis*> Analyses;

  typedef Analyses::iterator Analysis;
  
  typedef SummaryPlotFactory<CommissioningAnalysis*> Factory; 

  typedef std::vector<Histo*> Histos;
  
  typedef std::map<uint32_t,Histos> HistosMap;
  
  typedef std::map<uint32_t,uint32_t> FedToFecMap;

  // ---------- histogram "actions" ----------
  
  static uint32_t runNumber( DQMStore* const,
			     const std::vector<std::string>& );
  
  static sistrip::RunType runType( DQMStore* const,
				   const std::vector<std::string>& );
  
  /** Extracts custom information from list of MonitorElements. */
  static void copyCustomInformation( DQMStore* const,
				     const std::vector<std::string>& );
  
  /** Retrieves list of histograms in form of strings. */
  static void getContents( DQMStore* const,
			   std::vector<std::string>& );
  
  void extractHistograms( const std::vector<std::string>& );

  // DEPRECATE
  void createCollations( const std::vector<std::string>& );

  virtual void histoAnalysis( bool debug );

  virtual void printAnalyses();

  virtual void printSummary();
  
  virtual void createSummaryHisto( const sistrip::Monitorable&, 
				   const sistrip::Presentation&, 
				   const std::string& top_level_dir,
				   const sistrip::Granularity& );
  
  void remove( std::string pattern = "" ); 
  
  void save( std::string& filename,
	     uint32_t run_number = 0 ); 

  // ---------- protected methods ----------
  
 protected:

  inline const sistrip::RunType& task() const;
  
  inline DQMOldReceiver* const mui() const;
  
  inline DQMStore* const bei() const;
  
  inline Analyses& data();
  
  inline Factory* const factory();
  
  inline const HistosMap& histos() const;
  
  inline const FedToFecMap& mapping() const;
  
  TH1* histogram( const sistrip::Monitorable&, 
		  const sistrip::Presentation&, 
		  const sistrip::View&,
		  const std::string& directory,
		  const uint32_t& xbins,
		  const float& xlow = 1. * sistrip::invalid_,
		  const float& xhigh = 1. * sistrip::invalid_ );
  
  void printHistosMap();

  void clearHistosMap();

  // ---------- private member data ----------

 protected:

  std::auto_ptr<Factory> factory_;
  
 private:
  
  sistrip::RunType task_;
  
  DQMOldReceiver* mui_;
  
  DQMStore* bei_;
  
  Analyses data_;
  
  HistosMap histos_;
  
  FedToFecMap mapping_;
  
};

// ---------- inline methods ----------

const sistrip::RunType& CommissioningHistograms::task() const { return task_; }
DQMOldReceiver* const CommissioningHistograms::mui() const { return mui_; }
DQMStore* const CommissioningHistograms::bei() const { return bei_; }
CommissioningHistograms::Analyses& CommissioningHistograms::data() { return data_; }
CommissioningHistograms::Factory* const CommissioningHistograms::factory() { return factory_.get(); }
const CommissioningHistograms::HistosMap& CommissioningHistograms::histos() const { return histos_; }
const CommissioningHistograms::FedToFecMap& CommissioningHistograms::mapping() const { return mapping_; }

#endif // DQM_SiStripCommissioningClients_CommissioningHistograms_H
