/** \class CSCAnodeLCTAnalyzer
 *
 * Class for Monte Carlo studies of anode LCTs.  Returns a vector of up
 * to six (one per layer) CSCAnodeLayerInfo objects for a given ALCT.
 * They contain the list of wire digis used to build a given ALCT, and
 * the list of associated (closest) SimHits.
 *
 * \author Slava Valuev  26 May 2004.
 * Porting from ORCA by S. Valuev in September 2006.
 *
 * $Date: 2006/09/12 15:51:21 $
 * $Revision: 1.1 $
 *
 */

#ifndef CSCTriggerPrimitives_CSCAnodeLCTAnalyzer_H
#define CSCTriggerPrimitives_CSCAnodeLCTAnalyzer_H

#include <DataFormats/CSCDigi/interface/CSCWireDigiCollection.h>
#include <DataFormats/CSCDigi/interface/CSCALCTDigiCollection.h>
#include <SimDataFormats/TrackingHit/interface/PSimHitContainer.h>
#include <L1Trigger/CSCTriggerPrimitives/test/CSCLayerInfo.h>

typedef CSCLayerInfo<CSCWireDigi> CSCAnodeLayerInfo;

class CSCAnodeLCTAnalyzer
{
 public:
  /** Default constructor. */
  CSCAnodeLCTAnalyzer() {};

  /** Constructs vector of CSCAnodeLayerInfo objects for ALCT. */
  std::vector<CSCAnodeLayerInfo> getSimInfo(const CSCALCTDigi& alct,
       const CSCDetId& alctId, const CSCWireDigiCollection* wiredc,
       const edm::PSimHitContainer* allSimHits);

  /** Finds wiregroup, phi and eta of the nearest SimHit for comparison
      to the reconstructed values. */
  int nearestWG(const std::vector<CSCAnodeLayerInfo>& allLayerInfo,
		double& closestPhi, double& closestEta);

  /** Cache pointer to geometry for current event. */
  void setGeometry(const CSCGeometry* geom);

  /** Returns eta position of a given wiregroup. */
  double getWGEta(const CSCDetId& layerId, const int wiregroup);

  /** Turns on the debug flag for this class. */
  static void setDebug() {debug = true;}

  /** Turns off the debug flag for this class (default). */
  static void setNoDebug() {debug = false;}

 private:
  static bool debug;

  /* Cache geometry for current event. */
  const CSCGeometry* geom_;

  /* Find the list of WireDigis belonging to this ALCT. */
  std::vector<CSCAnodeLayerInfo> lctDigis(const CSCALCTDigi& alct,
       const CSCDetId& alctId, const CSCWireDigiCollection* wiredc);

  /* Find SimHits closest to each WireDigi on ALCT. */
  void digiSimHitAssociator(CSCAnodeLayerInfo& info,
			    const edm::PSimHitContainer* allSimHits);
};
#endif
