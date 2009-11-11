#ifndef DQM_HCALMONITORTASKS_HCALHOTCELLMONITOR_H
#define DQM_HCALMONITORTASKS_HCALHOTCELLMONITOR_H

#include "DQM/HcalMonitorTasks/interface/HcalBaseMonitor.h"
//#include "CalibFormats/HcalObjects/interface/HcalCalibrationWidths.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "CondFormats/HcalObjects/interface/HcalChannelStatus.h"
#include "CondFormats/HcalObjects/interface/HcalChannelQuality.h"

#include <cmath>
#include <iostream>
#include <fstream>

/** \class HcalHotCellMonitor
  *
  * $Date: 2009/11/10 14:10:56 $
  * $Revision: 1.35 $
  * \author J. Temple - Univ. of Maryland
  */

struct hotNeighborParams{
  int DeltaIphi;
  int DeltaIeta;
  int DeltaDepth;
  double minCellEnergy; // cells below this threshold can never be considered "hot" by this algorithm
  double minNeighborEnergy; //neighbors must have some amount of energy to be counted
  double maxEnergy; //  a cell above this energy will always be considered hot
  double HotEnergyFrac; // a cell will be considered hot if neighbor energy/ cell energy is less than this value
};

class HcalHotCellMonitor: public HcalBaseMonitor {

 public:
  HcalHotCellMonitor();

  ~HcalHotCellMonitor();

  void setup(const edm::ParameterSet& ps, DQMStore* dbe);
  void beginRun();
  void setupNeighborParams(const edm::ParameterSet& ps, hotNeighborParams& N, std::string type);
  void done();
  void clearME(); // overrides base class function
  void reset();

  void createMaps(const HcalDbService& cond);
  
  void processEvent(const HBHERecHitCollection& hbHits,
                    const HORecHitCollection& hoHits,
                    const HFRecHitCollection& hfHits,
		    const HcalDbService& cond,
		    int   CalibType
                    );

  void processEvent_rechitenergy( const HBHERecHitCollection& hbheHits,
                                  const HORecHitCollection& hoHits,
                                  const HFRecHitCollection& hfHits);

  void processEvent_rechitneighbors( const HBHERecHitCollection& hbheHits,
                                     const HORecHitCollection& hoHits,
                                     const HFRecHitCollection& hfHits);
  void beginLuminosityBlock(int lb);
  void endLuminosityBlock();
  void periodicReset();

 private:
  void fillNevents_neighbor();
  void fillNevents_energy();
  void fillNevents_persistentenergy();
  
  void fillNevents_problemCells();
  void zeroCounters();

  bool hotmon_makeDiagnostics_;
  int hotmon_minEvents_; // minimum # of events in a lumi block before persistent test will be checked
  // Booleans to control which of the three hot cell checking routines are used
  bool hotmon_test_neighbor_;
  bool hotmon_test_energy_;
  bool hotmon_test_persistent_;

  double energyThreshold_, HBenergyThreshold_, HEenergyThreshold_, HOenergyThreshold_, HFenergyThreshold_, ZDCenergyThreshold_;
  double persistentThreshold_, HBpersistentThreshold_, HEpersistentThreshold_, HOpersistentThreshold_, HFpersistentThreshold_, ZDCpersistentThreshold_;

  double HFfwdScale_;

  double hotmon_minErrorFlag_; // minimum error rate needed to dump out bad bin info 
  
  double nsigma_;
  double HBnsigma_, HEnsigma_, HOnsigma_, HFnsigma_, ZDCnsigma_;
  EtaPhiHists   AboveNeighborsHotCellsByDepth;
  EtaPhiHists   AboveEnergyThresholdCellsByDepth;
  EtaPhiHists   AbovePersistentThresholdCellsByDepth; 

  // map of pedestals from database (in ADC)
  /*
  std::map<HcalDetId, float> pedestals_;
  std::map<HcalDetId, float> widths_;
  std::map<HcalDetId, float> pedestal_thresholds_;
  */
  std::map<HcalDetId, double> rechitEnergies_;

  double SiPMscale_;
  int aboveneighbors[85][72][4];
  int aboveenergy[85][72][4]; // when rechit is above threshold energy
  int abovepersistent[85][72][4]; // when rechit is consistently above some threshold
  int rechit_occupancy_sum[85][72][4];
  float rechit_energy_sum[85][72][4];
  
  // counters for diagnostic plots
  int diagADC_HB[300];
  int diagADC_HE[300];
  int diagADC_HO[300];
  int diagADC_HF[300];
  int diagADC_ZDC[300];


  // Diagnostic plots
  MonitorElement* d_HBrechitenergy;
  MonitorElement* d_HErechitenergy;
  MonitorElement* d_HOrechitenergy;
  MonitorElement* d_HFrechitenergy;
  MonitorElement* d_ZDCrechitenergy;
 
  MonitorElement* d_HBenergyVsNeighbor;
  MonitorElement* d_HEenergyVsNeighbor;
  MonitorElement* d_HOenergyVsNeighbor;
  MonitorElement* d_HFenergyVsNeighbor;
  MonitorElement* d_ZDCenergyVsNeighbor;

  EtaPhiHists  d_avgrechitenergymap;
  EtaPhiHists  d_avgrechitoccupancymap;

  hotNeighborParams defaultNeighborParams_, HBNeighborParams_, HENeighborParams_, HONeighborParams_, HFNeighborParams_, ZDCNeighborParams_;
};

#endif
