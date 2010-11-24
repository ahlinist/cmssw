#ifndef TauAnalysis_FittingTools_DQMEffXsecCalculator_h  
#define TauAnalysis_FittingTools_DQMEffXsecCalculator_h

/** \class DQMEffXsecCalculator
 *
 * Auxiliary class to compute "effective" cross-section
 * for a combination of Monte Carlo samples
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.2 $
 *
 * $Id: DQMEffXsecCalculator.h,v 1.2 2010/11/17 19:23:29 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"

#include <vector>
#include <string>

class DQMEffXsecCalculator : public edm::EDAnalyzer
{
 public:
  
  explicit DQMEffXsecCalculator(const edm::ParameterSet&);
  ~DQMEffXsecCalculator();
  
 private:

  void beginJob() {}
  void analyze(const edm::Event&, const edm::EventSetup&) {}	
  void endJob();

//--- configuration parameters
  double dataIntLumi_;

  std::string moduleLabel_;

  struct channelEntryType
  { 
    channelEntryType(const std::string& name, const edm::ParameterSet& cfg, double dataIntLumi)
      : name_(name),
	dataIntLumi_(dataIntLumi)
    {
      std::string dqmDirectory = cfg.getParameter<std::string>("dqmDirectory");

      edm::ParameterSet cfgEfficiency = cfg.getParameter<edm::ParameterSet>("efficiency");
      std::string meNameNumerator = cfgEfficiency.getParameter<std::string>("numerator");
      meNameNumerator_ = dqmDirectoryName(dqmDirectory).append(meNameNumerator);
      std::string meNameDenominator = cfgEfficiency.getParameter<std::string>("denominator");
      meNameDenominator_ = dqmDirectoryName(dqmDirectory).append(meNameDenominator);

      std::string meNameNumEventsPassed = cfg.getParameter<std::string>("numEventsPassed");
      meNameNumEventsPassed_ = dqmDirectoryName(dqmDirectory).append(meNameNumEventsPassed);
    }
    ~channelEntryType() {}
    double getEffXsec(DQMStore& dqmStore) const
    {
      bool error = false;
      double numerator = getValue(dqmStore, meNameNumerator_, error);
      double denominator = getValue(dqmStore, meNameDenominator_, error);

      double numEventsPassed = getValue(dqmStore, meNameNumEventsPassed_, error);

      if ( error ) {
	edm::LogError ("getEffXsec") 
	  << " Failed to access MonitorElements --> returning dummy value !!";
	return -1.;
      }

      if ( numerator > denominator ) {
	edm::LogWarning ("getEffXsec") 
	  << " Value of numerator exceeds denominator !!";
	return 1.;
      }
      
      double efficiency = ( denominator != 0. ) ? (numerator/denominator) : 0.5;

      double effXsec = numEventsPassed/(dataIntLumi_*efficiency);

      //std::cout << "<getEffXsec>:" << std::endl;
      //std::cout << " name = " << name_ << std::endl;
      //std::cout << " numerator = " << numerator << std::endl;
      //std::cout << " denominator = " << denominator << std::endl;
      //std::cout << " efficiency = " << efficiency << std::endl;
      //std::cout << "--> effXsec = " << effXsec << std::endl;
      
      return effXsec;
    }
    std::string name_;	
    double dataIntLumi_;
    std::string meNameNumerator_;	
    std::string meNameDenominator_;
    std::string meNameNumEventsPassed_;
  };

  std::vector<channelEntryType*> channels_;
};

#endif  


