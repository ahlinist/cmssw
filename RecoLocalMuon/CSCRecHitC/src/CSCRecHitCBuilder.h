#ifndef CSCRecHitC_CSCRecHitCBuilder_h 
#define CSCRecHitC_CSCRecHitCBuilder_h 


/** \class CSCRecHitCBuilder 
 *
 * Algorithm to build 2-D RecHit from wire and strip digis
 * in endcap muon CSCs by implementing a 'build' function 
 * required by CSCRecHitCProducer.
 *
 * The builder goes through many stages before building 2-D hits:
 * 1) It finds wire clusters and form wire hits which it stores in CSCWireHit.
 * 2) From these wire hits, it builds pseudo-wire segments to clean up
 *    the wire hit collection from noisy hits.  Only the hits falling on
 *    the segment or far away from existing segments are retained.
 * 1) It then finds strip cluster and hits which it stores in CSCStripHit.
 * 2) Similary to the wire hits, segments are build using the strip hits.
 *    Because of the trapezoidal geometry of the strips, all strip hits falling
 *    close to the pseudo-strip segments are retained.
 *
 * \author Dominique Fortin - UCR
 *
 */

#include <DataFormats/CSCRecHit/interface/CSCRecHit2DCollection.h>
#include <DataFormats/CSCDigi/interface/CSCStripDigiCollection.h>
#include <DataFormats/CSCDigi/interface/CSCWireDigiCollection.h>

#include <FWCore/ParameterSet/interface/ParameterSet.h>


// UF algo parts: start

#include "DataFormats/CSCDigi/interface/CSCALCTDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCCLCTDigiCollection.h"
#include <DataFormats/MuonDetId/interface/CSCDetId.h>

/*
#include <TROOT.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1I.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TString.h>

#include "TLine.h"
#include "TBox.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TStyle.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TLatex.h"
*/

#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>

//#include "Utilities/Timing/interface/TimingReport.h"

// UF algo parts: end






// UF algo parts: start

const double pi = 3.14159265;
const int nEnd = 2;
const int nStat = 4;
const int nRing = 3;
const int nCSC = 50;
const int nLayer = 6;
const int nStripsTotal = 80;

const int nWGSegmentsMax = 5;
//int wgSegmentMin[nWGSegmentsMax];
//int wgSegmentMax[nWGSegmentsMax];

const int nStripDiv = 5;
const double stripParts[nStripDiv+1] = {0., 0.1, 0.2, 0.3, 0.4, 0.5};
// UF algo parts: end




/*
class CSCLayer;
class CSCGeometry;
class CSCGains;
class CSCcrosstalk;
class CSCNoiseMatrix;
class CSCDetId;
class CSCHitFromStripOnly;
class CSCHitFromWireOnly;
class CSCWireSegments;
class CSCStripSegments;
class CSCMake2DRecHit;
*/




class CSCRecHitCBuilder
{
 public:
  
  /** Configure the algorithm via ctor.
   * Receives ParameterSet percolated down from EDProducer
   * which owns this Builder.
   */
  explicit CSCRecHitCBuilder( const edm::ParameterSet& ps);  
  ~CSCRecHitCBuilder();
  
  /*
  void build( const CSCStripDigiCollection* stripds, const CSCWireDigiCollection* wireds,
	      CSCRecHit2DCollection& oc );
  
  
  // Cache pointer to geometry and calibration constants so they can be
  // redistributed further downstream
   
  void setGeometry   ( const CSCGeometry* geom ) {geom_ = geom;}
  void setCalibration( const CSCGains* gains,
                       const CSCcrosstalk* xtalk,
                       const CSCNoiseMatrix* noise ) {
    gains_ = gains;
    xtalk_ = xtalk;
    noise_ = noise;
  }

  const CSCLayer* getLayer( const CSCDetId& detId );
  */

  // UF algo: start
  void build( const CSCStripDigiCollection* strips, 
	      const CSCWireDigiCollection* wires,
	      const CSCALCTDigiCollection* alcts,
	      const CSCCLCTDigiCollection* clcts,
	      CSCRecHit2DCollection& oc );

  void printWhereIm(std::string s);

  void initializations(const edm::ParameterSet& conf);
  void quickCoordinate(const CSCStripDigiCollection* strips,
		       std::vector<int>& keyWG,
		       std::vector<int>& keyHS,
		       std::vector<int>& halfOrDi,
		       std::vector<CSCDetId>& keyDetId
		       );
  void findValidALCTs(const CSCALCTDigiCollection* alcts, 
		      std::vector<int>& keyWG0,
		      std::vector<int>& keyWG1,
		      std::vector<CSCDetId>& keyWGDetId0,
		      std::vector<CSCDetId>& keyWGDetId1
		      );
  void findValidCLCTs(const CSCCLCTDigiCollection* clcts, 
		      std::vector<int>& keyHS0,
		      std::vector<int>& keyHS1,
		      std::vector<int>& halfOrDi0,
		      std::vector<int>& halfOrDi1,
		      std::vector<CSCDetId>& keyHSDetId0,
		      std::vector<CSCDetId>& keyHSDetId1
		      );

  void matchALCTandCLCT(std::vector<int>& keyWG0,
			std::vector<int>& keyWG1,
			std::vector<CSCDetId>& keyWGDetId0,
			std::vector<CSCDetId>& keyWGDetId1,
			std::vector<int>& keyHS0,
			std::vector<int>& keyHS1,
			std::vector<int>& halfOrDi0,
			std::vector<int>& halfOrDi1,
			std::vector<CSCDetId>& keyHSDetId0,
			std::vector<CSCDetId>& keyHSDetId1,
			std::vector<int>& keyWG,
			std::vector<int>& keyHS,
			std::vector<int>& halfOrDi,
			std::vector<CSCDetId>& keyDetId
			);
  void trackSegmentLinearFit5of6(std::vector<double> quickCoord,
				 std::vector<double> quickCoordLayer,
				 std::vector<double> quickCoordY,
				 std::vector<double> quickCoordStrip,
				 std::vector<int> quickCoordKeyWG,
				 std::vector<int> quickCoordKeyWGSegment,
				 std::vector<double> quickCoordQc, 
				 std::vector<double> quickCoordQl, 
				 std::vector<double> quickCoordQr,
				 std::vector<int> quickCoordCscType
				 );



  int isCscON(CSCDetId myDetId);
  int whatCscType(CSCDetId myDetId);
  int sameDetId(const CSCDetId id1, const CSCDetId id2);
  int cscIndexFromType(int id);
  int defineWGSegmentNum(const int wg, const int id, int& nWGSegments);
  //in mm:
  double stripWidth(const int wg, const int id);
  double stripWidth_ME11(int wg, int strip);
  double stripWidth_1_2(const int wg);
  double stripWidth_1_3(const int wg);
  double stripWidth_2_1(const int wg);
  double stripWidth_3_1(const int wg);
  double stripWidth_4_1(const int wg);
  double stripWidth_23_2(const int wg);

  double QuickCoordCorr(double x, double wg, const int id);

  int defineStripPart(double x);
  double stripShift(int layer, const int id);

  void defineLinFitParam5of6(double &slope, 
			     double &constant,
			     const int refLayer,
			     const std::vector<double> quickCoord,//
			     const std::vector<double> quickCoordLayer,//
			     const std::vector<double> quickCoordY,//
			     const std::vector<double> quickCoordStrip,
			     const std::vector<int> quickCoordKeyWG,//
			     const std::vector<int> quickCoordKeyWGSegment,//
			     const std::vector<int> quickCoordCscType
			     );
  double defineResid5of6(const double slope, 
			 const double constant,
			 const int refLayer,		   
			 double& angle,
			 std::vector<double>& sigmaVec,//
			 std::vector<double>& fitCoordVec,//
			 std::vector<double>& residualsVec,//
			 std::vector<int>& stripPartNumVec,
			 std::vector<int>& wgSegmentNumVec,
			 const std::vector<double> quickCoord,//
			 const std::vector<double> quickCoordLayer,//
			 const std::vector<double> quickCoordY,//
			 const std::vector<double> quickCoordStrip,
			 const std::vector<int> quickCoordKeyWG,//
			 const std::vector<int> quickCoordKeyWGSegment,//
			 const std::vector<int> quickCoordCscType
			 );
  double tabulatedLinFitSigma(int sp, int hv, const int id);
  double tabulatedLinFitSigma_23_2(int sp, int hv);
  double tabulatedLinFitSigma_1_2(int sp, int hv);
  double tabulatedLinFitSigma_2_1(int sp, int hv);
  double tabulatedLinFitSigma_1_1(int sp, int hv);

  void update_tab_Pedestals(const CSCStripDigiCollection* strips);
  double tabPedestal(const int endcap, 
		     const int station, 
		     const int ring, 
		     const int chamber, 
		     const int layer, 
		     const int strip);

  int isKeyWGgood(const int keyWG, const int id);

  std::vector<double> quickCoordR, quickCoordQ1, quickCoordQ;

  int eventNumber;
 
  std::vector<CSCDetId> activeCSC;
  std::vector<int> activeCSCstat;

  // UF algo: end



 private:

  // UF algo: start

  int debugPrint;
  int doPruning, minHitsInSegment;
  double Chi2threshold;
  double cutLandauThreshold;
  int doQuickCoordinate;
  int doTrackSegmentLinearFit;
  int selectHalfStripsOnly;
  int selectDiStripsOnly;
  int useOnlyIfOneMLCTperCSC;
  int ifTakeWG1_48_inME11;
  int takeOnlyTopOfME11;
  double quickCoordDelQ;
  double linFitAngleCut;
  double linFitChi2threshold;
  double pedestalSigmaDyn;
  int stripTunnelSize;
  int nSigmasAbovePedToBeSignal;
  int useRing;
  int useStation1;
  int useStation2;
  int doUseTabPedestals;
  int useBorders;

  double tab_pedestal[nEnd][nStat][nRing][nCSC][nLayer][nStripsTotal];
  int n_tab_pedestal[nEnd][nStat][nRing][nCSC][nLayer][nStripsTotal];

  int nWGSegments;
  double Chi2all_6planes, Chi2_min_5planes;
  // UF algo: end





  bool isData;
  bool useCalib;
  bool debug;  
  int stripWireDeltaT;
  bool useCleanStripCollection;
  bool useCleanWireCollection;
  bool makePseudo2DHits;

  /*
  CSCHitFromStripOnly*   HitsFromStripOnly_;
  CSCHitFromWireOnly*    HitsFromWireOnly_;
  CSCWireSegments*       HitsFromWireSegments_;  
  CSCStripSegments*      HitsFromStripSegments_;  
  CSCMake2DRecHit*       Make2DHits_;

  
  //Cache geometry and calibrations for current event
   
  const CSCGeometry* geom_;
  const CSCGains* gains_;
  const CSCcrosstalk* xtalk_;
  const CSCNoiseMatrix* noise_;
  */




};

#endif
