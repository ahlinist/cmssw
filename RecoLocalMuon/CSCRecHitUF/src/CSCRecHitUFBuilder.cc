// This is CSCRecHitUFBuilder.cc


#include <RecoLocalMuon/CSCRecHitUF/src/CSCRecHitUFBuilder.h>
/*
  #include <RecoLocalMuon/CSCRecHitUF/src/CSCHitFromStripOnly.h>
  #include <RecoLocalMuon/CSCRecHitUF/src/CSCHitFromWireOnly.h>
  #include <RecoLocalMuon/CSCRecHitUF/src/CSCWireSegments.h>
  #include <RecoLocalMuon/CSCRecHitUF/src/CSCStripSegments.h>
  #include <RecoLocalMuon/CSCRecHitUF/src/CSCMake2DRecHit.h>
  #include <RecoLocalMuon/CSCRecHitUF/interface/CSCWireHitCollection.h>
  #include <RecoLocalMuon/CSCRecHitUF/interface/CSCStripHitCollection.h>
  #include <RecoLocalMuon/CSCRecHitUF/interface/CSCRangeMapForRecHit.h>
*/

#include <DataFormats/MuonDetId/interface/CSCDetId.h>
#include <DataFormats/CSCDigi/interface/CSCStripDigi.h>
#include <DataFormats/CSCDigi/interface/CSCWireDigi.h>

#include <Geometry/CSCGeometry/interface/CSCChamberSpecs.h>
#include <Geometry/CSCGeometry/interface/CSCLayer.h>
#include <Geometry/CSCGeometry/interface/CSCGeometry.h>

#include <CondFormats/CSCObjects/interface/CSCGains.h>
#include <CondFormats/DataRecord/interface/CSCGainsRcd.h>
#include <CondFormats/CSCObjects/interface/CSCcrosstalk.h>
#include <CondFormats/DataRecord/interface/CSCcrosstalkRcd.h>
#include <CondFormats/CSCObjects/interface/CSCNoiseMatrix.h>
#include <CondFormats/DataRecord/interface/CSCNoiseMatrixRcd.h>

#include <FWCore/Utilities/interface/Exception.h>
#include <FWCore/MessageLogger/interface/MessageLogger.h> 

#include <iostream>




// UF algo parts: start

#include "Utilities/Timing/interface/TimingReport.h"

#define PARAM slope, constant, refLayer, quickCoord, quickCoordLayer, quickCoordY, quickCoordStrip, quickCoordKeyWG, quickCoordKeyWGSegment, quickCoordCscType
#define RESID slope, constant, refLayer, angle, sigmaVec, fitCoordVec, residualsVec, stripPartNumVec, wgSegmentNumVec, quickCoord, quickCoordLayer, quickCoordY, quickCoordStrip, quickCoordKeyWG, quickCoordKeyWGSegment, quickCoordCscType
#define LINFIT quickCoord, quickCoordLayer, quickCoordY, quickCoordStrip, quickCoordKeyWG, quickCoordKeyWGSegment, quickCoordQc, quickCoordQl, quickCoordQr, quickCoordCscType

// UF algo parts: end





/* Constructor
 *
 */
CSCRecHitUFBuilder::CSCRecHitUFBuilder( const edm::ParameterSet& ps ) {//: geom_(0) {
  
  // Receives ParameterSet percolated down from EDProducer	


  useCalib               = ps.getUntrackedParameter<bool>("CSCUseCalibrations");  
  isData                 = ps.getUntrackedParameter<bool>("CSCIsRunningOnData"); 
  debug                  = ps.getUntrackedParameter<bool>("CSCDebug");
  stripWireDeltaT        = ps.getUntrackedParameter<int>("CSCstripWireDeltaTime");
  useCleanStripCollection= ps.getUntrackedParameter<bool>("CSCuseCleanStripCollection");
  useCleanWireCollection = ps.getUntrackedParameter<bool>("CSCuseCleanWireCollection");
  makePseudo2DHits       = ps.getUntrackedParameter<bool>("CSCproduce1DHits");


  
  /*
  HitsFromStripOnly_     = new CSCHitFromStripOnly( ps ); 
  HitsFromWireOnly_      = new CSCHitFromWireOnly( ps );  
  HitsFromWireSegments_  = new CSCWireSegments( ps );
  HitsFromStripSegments_ = new CSCStripSegments( ps );
  Make2DHits_            = new CSCMake2DRecHit( ps );
  */



  //UF algo: start
  initializations(ps);
  //UF algo: end


}

/* Destructor
 *
 */
CSCRecHitUFBuilder::~CSCRecHitUFBuilder() {

  /*
  delete HitsFromStripOnly_;
  delete HitsFromWireOnly_;
  delete HitsFromWireSegments_;
  delete HitsFromStripSegments_;
  delete Make2DHits_;   
  */

  //UF algo: start
  //TimingReport::current()->dump(std::cout);
  //UF algo: end


}










//////////////////////////////////////////////////////////////////////////////////////////////////////////
// UF algo: start
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void CSCRecHitUFBuilder::printWhereIm(std::string s)
{
  if (debugPrint) 
    std::cout << "[CSCRecHitUFBuilder]: " << s << std::endl;
}




void CSCRecHitUFBuilder::initializations(const edm::ParameterSet& ps)
{
 
  printWhereIm("initializations");

  debugPrint = ps.getUntrackedParameter<int>("debugPrint");
  doQuickCoordinate = ps.getUntrackedParameter<int>("doQuickCoordinate");
  doTrackSegmentLinearFit = ps.getUntrackedParameter<int>("doTrackSegmentLinearFit");
  doUseTabPedestals = ps.getUntrackedParameter<int>("doUseTabPedestals");

  selectHalfStripsOnly = ps.getUntrackedParameter<int>("selectHalfStripsOnly");
  selectDiStripsOnly = ps.getUntrackedParameter<int>("selectDiStripsOnly");
  useOnlyIfOneMLCTperCSC = ps.getUntrackedParameter<int>("useOnlyIfOneMLCTperCSC");
  ifTakeWG1_48_inME11 = ps.getUntrackedParameter<int>("ifTakeWG1_48_inME11");
  takeOnlyTopOfME11 = ps.getUntrackedParameter<int>("takeOnlyTopOfME11");
  useBorders = ps.getUntrackedParameter<int>("useBorders");

  doPruning = ps.getUntrackedParameter<int>("doPruning");
  minHitsInSegment = ps.getUntrackedParameter<int>("minHitsInSegment");
  Chi2threshold = ps.getUntrackedParameter<double>("Chi2threshold");
  linFitChi2threshold = ps.getUntrackedParameter<double>("linFitChi2threshold");
  linFitAngleCut = ps.getUntrackedParameter<double>("linFitAngleCut");
  pedestalSigmaDyn = ps.getUntrackedParameter<double>("pedestalSigmaDyn");
  cutLandauThreshold = ps.getUntrackedParameter<double>("cutLandauThreshold");

  stripTunnelSize = ps.getUntrackedParameter<int>("stripTunnelSize");
  nSigmasAbovePedToBeSignal = ps.getUntrackedParameter<int>("nSigmasAbovePedToBeSignal");
  useRing = ps.getUntrackedParameter<int>("useRing");
  useStation1 = ps.getUntrackedParameter<int>("useStation1");
  useStation2 = ps.getUntrackedParameter<int>("useStation2");






  if (1 || debugPrint)
    std::cout
      << "\nINITIALIZATION/CONFIGURATION PARAMETERS:"
      << "\ndebugPrint " << debugPrint
      << "\ndoQuickCoordinate " << doQuickCoordinate
      << "\ndoTrackSegmentLinearFit " << doTrackSegmentLinearFit
      << "\ndoUseTabPedestals " << doUseTabPedestals

      << "\nselectHalfStripsOnly " << selectHalfStripsOnly
      << "\nselectDiStripsOnly " << selectDiStripsOnly
      << "\nuseOnlyIfOneMLCTperCSC " << useOnlyIfOneMLCTperCSC
      << "\nifTakeWG1_48_inME11 " << ifTakeWG1_48_inME11
      << "\ntakeOnlyTopOfME11 " << takeOnlyTopOfME11
      << "\nuseBorders " << useBorders

      << "\ndoPruning " << doPruning
      << "\nminHitsInSegment " << minHitsInSegment
      << "\nChi2threshold " << Chi2threshold
      << "\nlinFitChi2threshold " << linFitChi2threshold
      << "\nlinFitAngleCut " << linFitAngleCut
      << "\npedestalSigmaDyn " << pedestalSigmaDyn
      << "\ncutLandauThreshold " << cutLandauThreshold

      << "\nstripTunnelSize " << stripTunnelSize
      << "\nnSigmasAbovePedToBeSignal " << nSigmasAbovePedToBeSignal
      << "\nuseRing " << useRing
      << "\nuseStation1 " << useStation1
      << "\nuseStation2 " << useStation2


      << "\n";



  eventNumber = 0;



  if (doUseTabPedestals)
    {
      for (int i=0; i<nEnd; i++)
	{
	  for (int j=0; j<nStat; j++)
	    {
	      for (int k=0; k<nRing; k++)
		{
		  for (int l=0; l<nCSC; l++)
		    {
		      for (int m=0; m<nLayer; m++)
			{
			  for (int n=0; n<nStripsTotal; n++)
			    {
			      tab_pedestal[i][j][k][l][m][n] = 0.0;
			      n_tab_pedestal[i][j][k][l][m][n] = 0;
			    }
			}
		    }
		}
	    }
	}
    }//if (doUseTabPedestals)





}//void CSCRecHitUFBuilder::initializations(const edm::ParameterSet& ps)






int CSCRecHitUFBuilder::whatCscType(CSCDetId myDetId)
{

  printWhereIm("whatCscType");  

  //int endcap = myDetId.endcap();
  int station = myDetId.station();
  int ring = myDetId.ring();
  //int chamber = myDetId.chamber();

  if (station == 1 && ring == 1)
    return 11;
  if (station == 1 && ring == 2)
    return 12;
  if (station == 1 && ring == 3)
    return 13;
  if (station == 2 && ring == 1)
    return 21;
  if ( (station == 2 || station == 3 || station == 4) && ring == 2)
    return 234;
  if (station == 3 && ring == 1)
    return 31;
  if (station == 4 && ring == 1)
    return 41;

  return 0;
}








int CSCRecHitUFBuilder::isCscON(CSCDetId myDetId)
{

  printWhereIm("isCscON");

  if (!myDetId.null())
    {
      //int endcap = myDetId.endcap();
      int station = myDetId.station();
      int ring = myDetId.ring();
      //int chamber = myDetId.chamber();

      // check#1:
      if (ring == useRing
	  && (station == useStation1 
	      || station == useStation2) 
	  )
	return 1;
      // check#1: end


      // ALL CSCs check:
      if (useRing == -1 && useStation1 == -1 && useStation2 == -1)
	return 1;
      
    }
  return 0; 
}










/*
  In the current version we take all ALCT (0 and 1) doesn't matter if
  they are ghosts, low quality...
*/
void CSCRecHitUFBuilder::findValidALCTs(const CSCALCTDigiCollection* alcts, 
				       std::vector<int>& keyWG0,
				       std::vector<int>& keyWG1,
				       std::vector<CSCDetId>& keyWGDetId0,
				       std::vector<CSCDetId>& keyWGDetId1
				       )
{


  printWhereIm("findValidALCTs");

  for (CSCALCTDigiCollection::DigiRangeIterator j=alcts->begin(); j!=alcts->end(); j++) 
    { 
      std::vector<CSCALCTDigi>::const_iterator digiItr = (*j).second.first;
      std::vector<CSCALCTDigi>::const_iterator last = (*j).second.second;
      if (debugPrint)
	std::cout << "this is CSC id:" << (*j).first << std::endl;

      CSCDetId oldDetId;
      CSCDetId myDetId = (*j).first;


      int tmpFlag1 = 0, flagID = 0;
      for (int id=0; id<int(activeCSC.size()); id++)
	{
	  if (myDetId == activeCSC[id])
	    {
	      tmpFlag1 = 1;
	      flagID = id;
	    }
	}
      if (tmpFlag1)
	activeCSCstat[flagID]++;
      else
	{
	  activeCSC.push_back(myDetId);	
	  activeCSCstat.push_back(1);
	}
	

      for( ; digiItr != last; ++digiItr) 
	{
	  if (debugPrint)
	    digiItr->print();

	  if (0)
	    std::cout 
	      << " ifTakeWG1_48_inME11 " << ifTakeWG1_48_inME11
	      << " digiItr->getKeyWG() " << digiItr->getKeyWG()
	      << std::endl;

	  int isItME11 = 0;
	  if (whatCscType(myDetId) == 11)
	    isItME11 = 1;

	  if ( digiItr->isValid() && isCscON(myDetId) 
	       && ( !isItME11
		    ||
		    (isItME11 && ifTakeWG1_48_inME11)
		    ||
		    (isItME11 && !ifTakeWG1_48_inME11 && (digiItr->getKeyWG() != 0 && digiItr->getKeyWG() != 47))
		    )
	       )
	    {	      
	      if (oldDetId != myDetId) 
		{

		  int keyW = digiItr->getKeyWG();

		  if (whatCscType(myDetId) == 13 
		      || whatCscType(myDetId) == 31)
		    keyW = keyW - 16;

		  if (isKeyWGgood(keyW, whatCscType(myDetId)))
		    {
		      keyWG0.push_back(keyW);
		      keyWGDetId0.push_back(myDetId);
		      oldDetId = myDetId;
		    }
		}
	      else
		{
		  int keyW = digiItr->getKeyWG();
		  if (whatCscType(myDetId) == 13 
		      || whatCscType(myDetId) == 31)
		    keyW = keyW - 16;

		  if (isKeyWGgood(keyW, whatCscType(myDetId)))
		    {
		      keyWG1.push_back(keyW);
		      keyWGDetId1.push_back(myDetId);
		    }
		}
	    } // if ( digiItr->isValid() && isCscON((*j).first) )
	} // for( ; digiItr != last; ++digiItr) 
    }// for (CSCALCTDigiCollection::DigiRangeIterator j=alcts->begin(); j!=alcts->end(); j++)

  
}//void findValidALCTs...




/*
  In the current version we take all CLCT (0 and 1) doesn't matter if
  they are ghosts, low quality, Di- or Half-strip patterns...
*/
void CSCRecHitUFBuilder::findValidCLCTs(const CSCCLCTDigiCollection* clcts, 
				       std::vector<int>& keyHS0,
				       std::vector<int>& keyHS1,
				       std::vector<int>& halfOrDi0,
				       std::vector<int>& halfOrDi1,
				       std::vector<CSCDetId>& keyHSDetId0,
				       std::vector<CSCDetId>& keyHSDetId1
				       )
{


  printWhereIm("findValidCLCTs");

  for (CSCCLCTDigiCollection::DigiRangeIterator j=clcts->begin(); j!=clcts->end(); j++) 
    { 
      std::vector<CSCCLCTDigi>::const_iterator digiItr = (*j).second.first;
      std::vector<CSCCLCTDigi>::const_iterator last = (*j).second.second;
      if (debugPrint)	
	std::cout << "this is CSC id:" << (*j).first << std::endl;

      CSCDetId oldDetId;
      CSCDetId myDetId = (*j).first;

      for( ; digiItr != last; ++digiItr) 
	{
	  if (debugPrint)
	    digiItr->print();
	  if ( digiItr->isValid() && isCscON(myDetId) )
	    {	      
	      if (oldDetId != myDetId) 
		{
		  if ( (digiItr->getStripType() == 1 && selectHalfStripsOnly) 
		       || (digiItr->getStripType() == 0 && selectDiStripsOnly)
		       || (!selectHalfStripsOnly && !selectDiStripsOnly)
		       )
		    {
		      if (useBorders || (!useBorders && digiItr->getKeyStrip() > 2 && digiItr->getKeyStrip() < 78) )
			{
			  keyHS0.push_back(digiItr->getKeyStrip());
			  halfOrDi0.push_back(digiItr->getStripType());
			  keyHSDetId0.push_back(myDetId);
			  oldDetId = myDetId;
			}
		    }
		}
	      else
		{
		  if ( (digiItr->getStripType() == 1 && selectHalfStripsOnly) 
		       || (digiItr->getStripType() == 0 && selectDiStripsOnly)
		       || (!selectHalfStripsOnly && !selectDiStripsOnly)
		       )
		    {
		      if (useBorders || (!useBorders && digiItr->getKeyStrip() > 2 && digiItr->getKeyStrip() < 78) )
			{
			  keyHS1.push_back(digiItr->getKeyStrip());
			  halfOrDi1.push_back(digiItr->getStripType());
			  keyHSDetId1.push_back(myDetId);
			}
		    }
		}
	    } // if ( digiItr->isValid() && isCscON((*j).first) )
	} // for( ; digiItr != last; ++digiItr)
    } // for (CSCCLCTDigiCollection::DigiRangeIterator j=clcts->begin(); j!=clcts->end(); j++)

} //void findValidCLCTs...









/*
  Matching for the moment all found ALCTs to all found CLCTs if they
  happen to be in the same CSC... 

  call them MLCT (ALCT and matched CLCT)

*/
void CSCRecHitUFBuilder::matchALCTandCLCT(std::vector<int>& keyWG0,
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
					 )
{

  printWhereIm("matchALCTandCLCT");

  std::vector<int> keyWGtmp;
  std::vector<int> keyHStmp;
  std::vector<int> halfOrDitmp;
  std::vector<CSCDetId> keyDetIdtmp1;
  std::vector<CSCDetId> keyDetIdtmp2;

  for (int i=0; i<int(keyWG0.size()); i++)
    {
      keyWGtmp.push_back(keyWG0[i]);      
      keyDetIdtmp1.push_back(keyWGDetId0[i]);
    }
  for (int i=0; i<int(keyWG1.size()); i++)
    {
      keyWGtmp.push_back(keyWG1[i]);      
      keyDetIdtmp1.push_back(keyWGDetId1[i]);
    }
  for (int i=0; i<int(keyHS0.size()); i++)
    {
      keyHStmp.push_back(keyHS0[i]);      
      halfOrDitmp.push_back(halfOrDi0[i]);
      keyDetIdtmp2.push_back(keyHSDetId0[i]);
    }
  for (int i=0; i<int(keyHS1.size()); i++)
    {
      keyHStmp.push_back(keyHS1[i]);      
      halfOrDitmp.push_back(halfOrDi1[i]);
      keyDetIdtmp2.push_back(keyHSDetId1[i]);
    }

  for (int i=0; i<int(keyWGtmp.size()); i++)
    {
      for (int j=0; j<int(keyHStmp.size()); j++)
	{
	  if (keyDetIdtmp1[i] == keyDetIdtmp2[j])
	    {
	      keyWG.push_back(keyWGtmp[i]);
	      keyHS.push_back(keyHStmp[j]);
	      halfOrDi.push_back(halfOrDitmp[j]);
	      keyDetId.push_back(keyDetIdtmp1[i]);
	    }
	}
    }


  if (useOnlyIfOneMLCTperCSC)
    //if (int(keyWG.size()) == 1)
    {

      keyWGtmp = keyWG;
      keyHStmp = keyHS;
      halfOrDitmp = halfOrDi;
      keyDetIdtmp1 = keyDetId;

      keyWG.clear();
      keyHS.clear();
      halfOrDi.clear();
      keyDetId.clear();

      std::vector<CSCDetId> tmpId;

      for (int i=0; i<int(keyWGtmp.size()); i++)
	{
	  int takeIt = 1;

	  for (int j=0; j<int(tmpId.size()); j++)
	    {
	      if (tmpId[j] == keyDetIdtmp1[i])
		takeIt = 0;
	    }//for (int j=0; j<int(tmpId.size()); j++)

	  if (takeIt)
	    {
	      tmpId.push_back(keyDetIdtmp1[i]);

	      keyWG.push_back(keyWGtmp[i]);
	      keyHS.push_back(keyHStmp[i]);
	      halfOrDi.push_back(halfOrDitmp[i]);
	      keyDetId.push_back(keyDetIdtmp1[i]);
	    } //if (takeIt) 
	} // for (int i=0; i<int(keyWGtmp.size()); i++)
    } // if (useOnlyIfOneMLCTperCSC)

}//void CSCRecHitUFBuilder::matchALCTandCLCT(





int CSCRecHitUFBuilder::sameDetId(const CSCDetId id1, const CSCDetId id2)
{

  printWhereIm("sameDetId");

  if (id1.endcap() == id2.endcap()
      && id1.station() == id2.station()
      && id1.ring() == id2.ring()
      && id1.chamber() == id2.chamber()
      )
    return 1;

  return 0;  
}













void CSCRecHitUFBuilder::update_tab_Pedestals(const CSCStripDigiCollection* strips)
{
  CSCDetId myDetId;
  for (CSCStripDigiCollection::DigiRangeIterator j=strips->begin(); j!=strips->end(); j++) 
    {
      myDetId = (*j).first;
      std::vector<CSCStripDigi>::const_iterator digiItr = (*j).second.first;
      std::vector<CSCStripDigi>::const_iterator last = (*j).second.second;

      int endcap  = myDetId.endcap();
      int station = myDetId.station();
      int ring    = myDetId.ring();
      int chamber = myDetId.chamber();
      int layer   = myDetId.layer();      

      for( ; digiItr != last; ++digiItr) 
	{
	  int strip = digiItr->getStrip();
	  std::vector<int> countsADC = digiItr->getADCCounts();
	  n_tab_pedestal[endcap][station][ring][chamber][layer][strip]++;
	  tab_pedestal[endcap][station][ring][chamber][layer][strip] += countsADC[0];
	}//for( ; digiItr != last; ++digiItr)
    }//for (CSCStripDigiCollection::DigiRangeIterator j=strips->begin(); j!=strips->end(); j++)

}



double CSCRecHitUFBuilder::tabPedestal(const int endcap, 
				      const int station, 
				      const int ring, 
				      const int chamber, 
				      const int layer, 
				      const int strip)
{
  if (n_tab_pedestal[endcap][station][ring][chamber][layer][strip]>2)
    return 
      tab_pedestal[endcap][station][ring][chamber][layer][strip]
      /double(n_tab_pedestal[endcap][station][ring][chamber][layer][strip]);

  return -1;
}











void CSCRecHitUFBuilder::quickCoordinate(const CSCStripDigiCollection* strips,
					std::vector<int>& keyWG_,
					std::vector<int>& keyHS_,
					std::vector<int>& halfOrDi,
					std::vector<CSCDetId>& keyDetId
					)
{
  

  printWhereIm("quickCoordinate");

  if (doUseTabPedestals)
    {
      update_tab_Pedestals(strips);
    }

  //int doPr = 0;
  //int debugPr = 0;

  double pedestalMeanDyn, pedestalMeanDynLeft, pedestalMeanDynRight;

  CSCDetId myDetId;
  
  std::vector<double> quickCoord;
  std::vector<double> quickCoordLayer;
  std::vector<double> quickCoordY;
  std::vector<double> quickCoordStrip;
  std::vector<int> quickCoordKeyWG;
  std::vector<int> quickCoordKeyWGSegment;
  std::vector<double> quickCoordQc;
  std::vector<double> quickCoordQl;
  std::vector<double> quickCoordQr;
  std::vector<int> quickCoordCscType;
  
  quickCoordR.clear();
  quickCoordQ1.clear();
  quickCoordQ.clear();

  CSCDetId oldDetId;
  int keyHSold = -1;
  


  if (debugPrint)
    {
      std::cout 
	<< "\n\n------------------------------------------------------\n";
      for (int i=0; i<int(keyWG_.size()); i++)
	{
	  std::cout 
	    << eventNumber
	    << " keyWG_ " << keyWG_[i]
	    << " keyHS_ " << keyHS_[i]
	    << " halfOrDi " << halfOrDi[i]
	    << " keyDetId " << keyDetId[i]
	    << std::endl;
	}
    }

  // DEFINE MAX STRIP & CALC. LANDAU/QuickCoord INPUT:
  for (int i=0; i<int(keyDetId.size()); i++)	    
    {

      {
	if (int(quickCoord.size()) >= minHitsInSegment && doTrackSegmentLinearFit)
	  trackSegmentLinearFit5of6(LINFIT);
		  
	oldDetId = myDetId;
	keyHSold = keyHS_[i];

	quickCoord.clear();
	quickCoordLayer.clear();
	quickCoordY.clear();
	quickCoordStrip.clear();
	quickCoordKeyWG.clear();
	quickCoordKeyWGSegment.clear();
	quickCoordQc.clear();
	quickCoordQl.clear();
	quickCoordQr.clear();
	quickCoordCscType.clear();

	quickCoordR.clear();
	quickCoordQ1.clear();
	quickCoordQ.clear();

      }



      for (CSCStripDigiCollection::DigiRangeIterator j=strips->begin(); j!=strips->end(); j++) 
	{
	  myDetId = (*j).first;
	  if (isCscON(myDetId))
	    {	  


	      if (sameDetId(keyDetId[i], myDetId))
		{

		  
		  int keyWG = keyWG_[i];
		  int keyS = keyHS_[i];
		  if (debugPrint)
		    std::cout << "* keyWG " << keyWG << " keyS " << keyS << std::endl;

		  std::vector<CSCStripDigi>::const_iterator digiItr = (*j).second.first;
		  std::vector<CSCStripDigi>::const_iterator digiItr2 = (*j).second.first;
		  std::vector<CSCStripDigi>::const_iterator last = (*j).second.second;
		  int layerG = (*j).first.layer();	  

		  std::vector<int> stripMax, timeMax;
		  int stripMaxFin = -10, timeMaxFin = -10;
		  double leftCharge = 0, centerCharge = 0, rightCharge = 0;


		  /*
		    firstStrip and lastStrip

		    this is to use to avoid reported strips array
		    borders... for the time being want maxStrip to be
		    not the most right or most left strip... (or even
		    worse: as not always second CFEB is reported when
		    signal on the border...)
		  */
		  int firstStrip = -10;
		  int lastStrip = -10;
		  for( ; digiItr != last; ++digiItr) 
		    {
		      if (firstStrip == -10)
			firstStrip = digiItr->getStrip();
		      lastStrip = digiItr->getStrip();
		      
		      if (debugPrint)
			{
			  std::vector<int> countsADC = digiItr->getADCCounts();
			  std::cout << eventNumber
				    << " strip " << digiItr->getStrip() << " countsADC Ref ";
			  for (int k=0; k<int(countsADC.size()); k++)
			    std::cout << " " << countsADC[k];
			  std::cout << std::endl;
			}

		    }

		  digiItr = (*j).second.first;
		  digiItr2 = digiItr;
		  for( ; digiItr != last; ++digiItr) 
		    {      		      
		      int strip = digiItr->getStrip();

		      //no 0 strips, *strips collection numbers strips from 1 to 80
		      // as it is now the following doesn't take into consideration first/last reported strip...
		      if ( strip > firstStrip && strip < lastStrip
			   && ( whatCscType(myDetId) != 11
				|| ( whatCscType(myDetId)==11 && 
				     ( ( strip <= 64 && takeOnlyTopOfME11 )
				       || !takeOnlyTopOfME11 ) 
				     ) 
				)
			   && ( strip > (double(keyS)/2.0-stripTunnelSize) && strip < (double(keyS)/2.0+stripTunnelSize) )
			   )
			{
			  
			  digiItr2 = digiItr;
			  std::vector<int> countsADC = digiItr2->getADCCounts();
			  digiItr2 = digiItr;
			  std::vector<int> countsADCLeft = (--digiItr2)->getADCCounts();
			  digiItr2 = digiItr;
			  std::vector<int> countsADCRight = (++digiItr2)->getADCCounts();

			  int stripL = (--digiItr2)->getStrip(); digiItr2 = digiItr;
			  int stripR = (++digiItr2)->getStrip(); digiItr2 = digiItr;

			  if (0 && debugPrint)
			    {
			      std::cout << "countsADC ";
			      for (int k=0; k<int(countsADC.size()); k++)
				std::cout << " " << countsADC[k];
			      std::cout << std::endl; 
			      std::cout << "countsADCLeft ";
			      for (int k=0; k<int(countsADCLeft.size()); k++)
				std::cout << " " << countsADCLeft[k];
			      std::cout << std::endl; 
			      std::cout << "countsADCRight ";
			      for (int k=0; k<int(countsADCRight.size()); k++)
				std::cout << " " << countsADCRight[k];
			      std::cout << std::endl; 
			    }

			  // Dynamic pedestals
			  pedestalMeanDyn = (countsADC[0] + countsADC[1])/2.0;
			  pedestalMeanDynLeft = (countsADCLeft[0] + countsADCLeft[1])/2.0;
			  pedestalMeanDynRight = (countsADCRight[0] + countsADCRight[1])/2.0;

			  if (doUseTabPedestals)
			    {
			      double pp = tabPedestal(myDetId.endcap(), 
						      myDetId.station(), 
						      myDetId.ring(), 
						      myDetId.chamber(), 
						      myDetId.layer(), 
						      strip);
			      if (pp>0)
				pedestalMeanDyn = pp;

			      pp = tabPedestal(myDetId.endcap(), 
					       myDetId.station(), 
					       myDetId.ring(), 
					       myDetId.chamber(), 
					       myDetId.layer(), 
					       stripL);
			      if (pp>0)
				pedestalMeanDynLeft = pp;

			      pp = tabPedestal(myDetId.endcap(), 
					       myDetId.station(), 
					       myDetId.ring(), 
					       myDetId.chamber(), 
					       myDetId.layer(), 
					       stripR);
			      if (pp>0)
				pedestalMeanDynRight = pp;
			    }//if (doUseTabPedestals)


			  for (int i=1; i<(int(countsADC.size()) - 1); i++)
			    {
			      if (debugPrint)
				std::cout 
				  << strip 
				  << " " << i 
				  << " # " << double(countsADC[i]-pedestalMeanDyn)/double(pedestalSigmaDyn)
				  << " $ " << double(countsADC[i]-pedestalMeanDyn)
				  << " $ " << double(countsADC[i+1]-pedestalMeanDyn)
				  << " $ " << double(countsADC[i-1]-pedestalMeanDyn)
				  << " & " << double(countsADC[i] + countsADC[i-1] + countsADC[i+1]
						     - 3.0*pedestalMeanDyn)
				  << " & " << double(countsADCLeft[i] + countsADCLeft[i-1] + countsADCLeft[i+1]
						     - 3.0*pedestalMeanDynLeft)
				  << " & " << double(countsADCRight[i] + countsADCRight[i-1] + countsADCRight[i+1]
						     - 3.0*pedestalMeanDynRight)
				  << "\n";

			      if (double(countsADC[i]-pedestalMeanDyn)/double(pedestalSigmaDyn)>nSigmasAbovePedToBeSignal

				  && double(countsADC[i]-pedestalMeanDyn) > double(countsADC[i+1]-pedestalMeanDyn)
				  && double(countsADC[i]-pedestalMeanDyn) > double(countsADC[i-1]-pedestalMeanDyn)

				  && double(countsADC[i] + countsADC[i-1] + countsADC[i+1]
					    - 3.0*pedestalMeanDyn)  
				  > double(countsADCLeft[i] + countsADCLeft[i-1] + countsADCLeft[i+1]
					   - 3.0*pedestalMeanDynLeft)

				  && double(countsADC[i] + countsADC[i-1] + countsADC[i+1]
					    - 3.0*pedestalMeanDyn)  
				  >= double(countsADCRight[i] + countsADCRight[i-1] + countsADCRight[i+1]
					    - 3.0*pedestalMeanDynRight)
				  )
				{				  
				  stripMax.push_back(strip);
				  timeMax.push_back(i);
				}				
			    }//for (int i=1; i<int(countsADC.size()-1); i++)
			}//if (strip > firstStrip && strip < lastStrip)
		    }//for( ; digiItr != last; ++digiItr)







		  double minDist = 1000;
		  stripMaxFin = -1;
		  timeMaxFin = -1;
		  centerCharge = 0;
		  leftCharge = 0;
		  rightCharge = 0;
		  for (int ii=0; ii<int(stripMax.size()); ii++)
		    {
		      if (fabs(stripMax[ii] - double(keyS)/2.0) < minDist)
			{
			  stripMaxFin = stripMax[ii];
			  timeMaxFin = timeMax[ii];
			  minDist = fabs(stripMax[ii] - double(keyS)/2.0);
			}
		      if (debugPrint)
			std::cout << eventNumber
				  << " Max Strip #" << ii 
				  << " strip " << stripMax[ii] 
				  << " stripMaxFin " << stripMaxFin
				  << " keyS " << keyS
				  << " diff " << (stripMax[ii] - double(keyS)/2.0)
				  << " minDist " << minDist
				  << std::endl;
		    }


		  digiItr = (*j).second.first;
		  for( ; digiItr != last; ++digiItr) 
		    { 
		      int strip = digiItr->getStrip();
		      if (strip == stripMaxFin 
			  || strip == stripMaxFin+1
			  || strip == stripMaxFin-1
			  )
			{
			  std::vector<int> countsADC = digiItr->getADCCounts();	      		      
			  int strip = digiItr->getStrip();

			  // Dynamic pedestal
			  pedestalMeanDyn = (countsADC[0] + countsADC[1])/2.0;
			  if (doUseTabPedestals)
			    {
			      double pp = tabPedestal(myDetId.endcap(), 
						      myDetId.station(), 
						      myDetId.ring(), 
						      myDetId.chamber(), 
						      myDetId.layer(), 
						      strip);
			      if (pp>0)
				pedestalMeanDyn = pp;
			    } //if (doUseTabPedestals)

			  if ( strip == (stripMaxFin - 1) )
			    {
			      leftCharge = 
				countsADC[timeMaxFin] 
				+ countsADC[timeMaxFin-1]
				+ countsADC[timeMaxFin+1]
				- 3.0*pedestalMeanDyn;      
			    }
			  if ( strip == (stripMaxFin + 1) )
			    {
			      rightCharge = 
				countsADC[timeMaxFin] 
				+ countsADC[timeMaxFin-1]
				+ countsADC[timeMaxFin+1]
				- 3.0*pedestalMeanDyn;
			    }
			  if ( strip == stripMaxFin)
			    {
			      centerCharge = 
				countsADC[timeMaxFin] 
				+ countsADC[timeMaxFin-1]
				+ countsADC[timeMaxFin+1]
				- 3.0*pedestalMeanDyn;
			    }
			    
			} //if (strip == stripMaxFin...
		    }//for( ; digiItr != last; ++digiItr)

	      


		  

		  int wgSegmentNum = defineWGSegmentNum(keyWG, 
							whatCscType(myDetId), 
							nWGSegments);




		  if (stripMaxFin >= 0 && centerCharge > 0)
		    {

		      if (debugPrint)
			std::cout << eventNumber
				  << " keyS " << keyS
				  << " keyWG " << keyWG
				  << " wgSegmentNum " << wgSegmentNum
				  << " layerG " << layerG
				  << " stripMaxFin " << stripMaxFin
				  << " centerCharge " << centerCharge
				  << " leftCharge " << leftCharge
				  << " rightCharge " << rightCharge
				  << " timeMaxFin " << timeMaxFin
				  << std::endl;
			  


		      double min;
		      if (rightCharge <= leftCharge)
			min = rightCharge;
		      else 
			min = leftCharge;


		      if ( (centerCharge + rightCharge + leftCharge) > cutLandauThreshold )
			{

			  double r = 0.5*(rightCharge-leftCharge)/(centerCharge-min);

			  double b = 1.25;
			  double a = 0.27/pow(stripWidth(keyWG,whatCscType(myDetId))/10.0,2.7);
			  if (whatCscType(myDetId) == 11)
			    a = 0.11/pow(stripWidth_ME11(keyWG,keyS)/10.0,2.9);
			  double q = r + r*(0.5-fabs(r))/(a+b*fabs(r));

			  if (debugPrint)
			    std::cout << eventNumber
				      << " r " << r
				      << " q " << q;

			  double q1 = q;

			  q = q + QuickCoordCorr(q, keyWG, whatCscType(myDetId));

			  if (debugPrint)
			    std::cout
			      << " q' " << q
			      << "\n";





			  quickCoordR.push_back(r);
			  quickCoordQ1.push_back(q1);
			  quickCoordQ.push_back(q);			  
			  quickCoord.push_back(q);
			  quickCoordLayer.push_back(layerG);
			  for (int ll=0; ll<6; ll++)
			    {
			      if (ll == layerG-1)
				quickCoordY.push_back(-2.5 + 1.0*ll);
			    }

			  quickCoordQc.push_back(centerCharge);
			  quickCoordQl.push_back(leftCharge);
			  quickCoordQr.push_back(rightCharge);
			  quickCoordCscType.push_back(whatCscType(myDetId));

			  quickCoordStrip.push_back(stripMaxFin);
			  quickCoordKeyWG.push_back(keyWG);
			  quickCoordKeyWGSegment.push_back(wgSegmentNum);
			  
			    
			} //if ( (centerCharge + rightCharge + leftCharge) > cutLandauThreshold )

		    } //if (stripMaxFin >= 0 && centerCharge > 0)

		} //if (sameDetId(keyDetId[i], myDetId))

	    }//if (isCscON(myDetId))

	}//for (CSCStripDigiCollection::DigiRangeIterator

    } //for (int i=0; i<int(keyDetId.size()); i++)	  
	  

  if (int(quickCoord.size()) >= minHitsInSegment && doTrackSegmentLinearFit)
    trackSegmentLinearFit5of6(LINFIT);

}//void quickCoordinate...








int CSCRecHitUFBuilder::isKeyWGgood(const int keyWG, const int id)
{

  printWhereIm("isKeyWGgood");

  if (!useBorders)
    return 1;

  int OK = 0;
  int n = 0;

  if (id == 11)
    return 1;
  if (id == 12 || id == 13 || id == 21 || id == 31 || id == 41)
    n=3;
  if (id == 234)
    n=5;
  
  const int nWGSegmentsMax = n; 
  int wgSegmentMin[nWGSegmentsMax];
  int wgSegmentMax[nWGSegmentsMax];
  
  if (id == 234)
    {
      wgSegmentMin[0] = 1;
      wgSegmentMin[1] = 17;
      wgSegmentMin[2] = 29;
      wgSegmentMin[3] = 41;
      wgSegmentMin[4] = 53;

      wgSegmentMax[0] = 14;
      wgSegmentMax[1] = 26;
      wgSegmentMax[2] = 38;
      wgSegmentMax[3] = 50;
      wgSegmentMax[4] = 62;
    }
  if (id == 12)
    {
      wgSegmentMin[0] = 1;
      wgSegmentMin[1] = 25;
      wgSegmentMin[2] = 49;

      wgSegmentMax[0] = 22;
      wgSegmentMax[1] = 46;
      wgSegmentMax[2] = 62;
    }
  if (id == 13)
    {
      wgSegmentMin[0] = 1;
      wgSegmentMin[1] = 13;
      wgSegmentMin[2] = 23;

      wgSegmentMax[0] = 10;
      wgSegmentMax[1] = 20;
      wgSegmentMax[2] = 30;
    }
  if (id == 21)
    {
      wgSegmentMin[0] = 1;
      wgSegmentMin[1] = 45;
      wgSegmentMin[2] = 81;

      wgSegmentMax[0] = 42;
      wgSegmentMax[1] = 78;
      wgSegmentMax[2] = 110;
    }
  if (id == 31 || id == 41)
    {
      wgSegmentMin[0] = 1;
      wgSegmentMin[1] = 33;
      wgSegmentMin[2] = 65;

      wgSegmentMax[0] = 30;
      wgSegmentMax[1] = 62;
      wgSegmentMax[2] = 94;
    }


  for (int ii=0; ii<nWGSegmentsMax; ii++)
    {
      if (keyWG >= wgSegmentMin[ii] && keyWG <= wgSegmentMax[ii])
	OK = 1;
    }

  return OK;
	      	
}














int CSCRecHitUFBuilder::defineWGSegmentNum(const int keyWG, const int id, int& nWGSegments)
{

  printWhereIm("defineWGSegmentNum");

  int wgSegmentNum = -1;
  int n = 0;

  if (id == 11)
    return 0;
  if (id == 12 || id == 13 || id == 21 || id == 31 || id == 41)
    n=3;
  if (id == 234)
    n=5;
  
  const int nWGSegmentsMax = n; 
  int wgSegmentMin[nWGSegmentsMax];
  int wgSegmentMax[nWGSegmentsMax];
  
  if (id == 234)
    {
      wgSegmentMin[0] = 0;
      wgSegmentMin[1] = 16;
      wgSegmentMin[2] = 28;
      wgSegmentMin[3] = 40;
      wgSegmentMin[4] = 52;

      wgSegmentMax[0] = 15;
      wgSegmentMax[1] = 27;
      wgSegmentMax[2] = 39;
      wgSegmentMax[3] = 51;
      wgSegmentMax[4] = 63;
    }
  if (id == 12)
    {
      wgSegmentMin[0] = 0;
      wgSegmentMin[1] = 24;
      wgSegmentMin[2] = 48;

      wgSegmentMax[0] = 23;
      wgSegmentMax[1] = 47;
      wgSegmentMax[2] = 63;
    }
  if (id == 13)
    {
      wgSegmentMin[0] = 0;
      wgSegmentMin[1] = 12;
      wgSegmentMin[2] = 22;

      wgSegmentMax[0] = 11;
      wgSegmentMax[1] = 21;
      wgSegmentMax[2] = 31;
    }
  if (id == 21)
    {
      wgSegmentMin[0] = 0;
      wgSegmentMin[1] = 44;
      wgSegmentMin[2] = 80;

      wgSegmentMax[0] = 43;
      wgSegmentMax[1] = 79;
      wgSegmentMax[2] = 111;
    }
  if (id == 31 || id == 41)
    {
      wgSegmentMin[0] = 0;
      wgSegmentMin[1] = 32;
      wgSegmentMin[2] = 64;

      wgSegmentMax[0] = 31;
      wgSegmentMax[1] = 63;
      wgSegmentMax[2] = 95;
    }


  for (int ii=0; ii<nWGSegmentsMax; ii++)
    {
      if (keyWG >= wgSegmentMin[ii] && keyWG <= wgSegmentMax[ii])
	wgSegmentNum = ii;
    }
  if (keyWG > wgSegmentMax[nWGSegmentsMax-1])
    {
      //std::cout << "large KeyWG = " << keyWG << std::endl;
      wgSegmentNum = nWGSegmentsMax-1;
    }
  if (keyWG < wgSegmentMin[0])
    {
      //std::cout << "small KeyWG = " << keyWG << std::endl;
      wgSegmentNum = 0;
    }

  return wgSegmentNum;
	      	
}






double CSCRecHitUFBuilder::stripWidth(const int wg, const int id) //in mm
{

  printWhereIm("stripWidth");

  if (id == 234)
    return stripWidth_23_2(wg);
  if (id == 12)
    return stripWidth_1_2(wg);
  if (id == 13)
    return stripWidth_1_3(wg);
  if (id == 21)
    return stripWidth_2_1(wg);
  if (id == 31)
    return stripWidth_3_1(wg);
  if (id == 41)
    return stripWidth_4_1(wg);

  return 10000.00;

}




double CSCRecHitUFBuilder::stripWidth_ME11(int wg, int strip) //in mm
{

  printWhereIm("stripWidth_ME11");

  //const int doPr = 0;
  //int debugPr = 0;
  
  const float HS_Phi_ME1_1_top    = (10.84*3.14159/180.0)/64.0/2.0;  // Angular size of half strips (HS), radians
  const float HS_Phi_ME1_1_bottom = (10.84*3.14159/180.0)/48.0/2.0;  // Angular size of half strips (HS), radians
  const float Phi_ME1_1_top = HS_Phi_ME1_1_top*2.0;
  const float Phi_ME1_1_bottom = HS_Phi_ME1_1_bottom*2.0;
  const int nS_ME1_1_top    = 64; // Number of Strips in chamber ME1/1 (top part)
  const int nS_ME1_1_bottom = 48; // Number of Strips in chamber ME1/1 (bottom part)
  // const float DPhi_ME1_1 = 5.25*3.14159/180.0;
  // Only first 64 CLCT chanels are belong to the top part of chamber
  // Also we do not take into account 1st and last (48th) wire group because of their "triangle" structure
  double w = 0.000000000001;

  strip = strip + 1;
  wg = wg + 1;
  if (debugPrint)
    std::cout << "key wg " << wg << "  key strip: " << strip << std::endl;  
  
  double localPhi_mu = -1000, localY_mu = -1000;

  //top
  if (wg >= 1 && wg <= 48 && strip > 0 && strip <= nS_ME1_1_top*2)
    {
      localPhi_mu = HS_Phi_ME1_1_top*((float)(strip - nS_ME1_1_top) - 0.5);
      localY_mu   = (1060.0 + 75.7 + 31.4422*(float)(wg - 2))
	*(1.0 + sin(localPhi_mu)*sin(29.0*3.14159/180.0)/sin(61.0*3.14159/180.0 - localPhi_mu));
      w = fabs(localY_mu*Phi_ME1_1_top/cos(localPhi_mu));
    }

  // bottom
  if (wg >= 1 && wg <= 48 && strip > nS_ME1_1_top*2 && strip <= (nS_ME1_1_top*2 + nS_ME1_1_bottom*2) )
    {
      strip = strip - nS_ME1_1_top*2;
      localPhi_mu = HS_Phi_ME1_1_bottom*((float)(strip - nS_ME1_1_bottom) - 0.5);
      localY_mu   = (1060.0 + 75.7 + 31.4422*(float)(wg - 2))
	*(1.0 + sin(localPhi_mu)*sin(29.0*3.14159/180.0)/sin(61.0*3.14159/180.0 - localPhi_mu));
      w = fabs(localY_mu*Phi_ME1_1_bottom/cos(localPhi_mu));
    }
    

  if (debugPrint)
    std::cout 
      << "localPhi_mu " << localPhi_mu
      << "\tlocalY_mu " << localY_mu
      << "\tstripWidth " << w
      << std::endl;      


  if (w < 0.01)
    std::cout << "WARNING: strip width not defined properly... wg: " << wg << "\n";

  return w;
}






double CSCRecHitUFBuilder::stripWidth_4_1(const int wg) //in mm
{

  printWhereIm("stripWidth_4_1");

  double w = 0.000000000001;
  const int nWG = 96;
  double width[nWG] //in mm
    = {
      8.73, 8.80, 8.87, 8.94, 9.01, 9.09, 9.16, 9.23, 9.31, 9.38, 
      9.45, 9.52, 9.60, 9.67, 9.74, 9.81, 9.89, 9.96, 10.03, 10.10, 
      10.18, 10.25, 10.32, 10.39, 10.47, 10.54, 10.61, 10.68, 10.76, 10.83, 
      10.90, 10.95, 11.08, 11.12, 11.19, 11.27, 11.34, 11.41, 11.48, 11.56, 
      11.63, 11.70, 11.77, 11.85, 11.92, 11.99, 12.06, 12.14, 12.21, 12.28, 
      12.35, 12.43, 12.50, 12.57, 12.64, 12.72, 12.79, 12.86, 12.93, 13.01, 
      13.08, 13.15, 13.23, 13.27, 13.40, 13.44, 13.52, 13.59, 13.66, 13.73, 
      13.81, 13.88, 13.95, 14.02, 14.10, 14.17, 14.24, 14.31, 14.39, 14.46, 
      14.53, 14.60, 14.68, 14.75, 14.82, 14.90, 14.97, 15.04, 15.11, 15.19, 
      15.26, 15.33, 15.40, 15.48, 15.55, 15.62
    };
  
  for (int i=0; i<nWG; i++)
    {
      if (wg >= 0 && wg == i)
	w = width[i];
    }

  if (w < 0.01)
    std::cout << "WARNING: strip width not defined properly... wg: " << wg << "\n";

  return w;
}






double CSCRecHitUFBuilder::stripWidth_3_1(const int wg) //in mm
{

  printWhereIm("stripWidth_3_1");

  double w = 0.000000000001;
  const int nWG = 96;
  double width[nWG] //in mm
    = {
      7.80, 7.86, 7.93, 8.01, 8.08, 8.15, 8.22, 8.30, 8.37, 8.44, 
      8.51, 8.59, 8.66, 8.73, 8.81, 8.88, 8.95, 9.02, 9.10, 9.17, 
      9.24, 9.31, 9.39, 9.46, 9.53, 9.60, 9.68, 9.75, 9.82, 9.89, 
      9.97, 10.01, 10.15, 10.21, 10.29, 10.38, 10.47, 10.55, 10.64, 10.73, 
      10.82, 10.90, 10.99, 11.08, 11.16, 11.25, 11.34, 11.43, 11.51, 11.60, 
      11.69, 11.77, 11.86, 11.95, 12.04, 12.12, 12.21, 12.30, 12.38, 12.47, 
      12.56, 12.65, 12.73, 12.79, 12.94, 12.99, 13.08, 13.17, 13.26, 13.34, 
      13.43, 13.52, 13.60, 13.69, 13.78, 13.87, 13.95, 14.04, 14.13, 14.21, 
      14.30, 14.39, 14.47, 14.56, 14.65, 14.74, 14.82, 14.91, 15.00, 15.08, 
      15.17, 15.26, 15.35, 15.43, 15.52, 15.61
    };
  
  for (int i=0; i<nWG; i++)
    {
      if (wg >= 0 && wg == i)
	w = width[i];
    }

  if (w < 0.01)
    std::cout << "WARNING: strip width not defined properly... wg: " << wg << "\n";

  return w;
}






double CSCRecHitUFBuilder::stripWidth_2_1(const int wg) //in mm
{

  printWhereIm("stripWidth_2_1");

  double w = 0.000000000001;
  const int nWG = 112;
  double width[nWG] //in mm
    = {
      6.87, 6.93, 7.00, 7.08, 7.15, 7.22, 7.30, 7.37, 7.44, 7.51, 
      7.59, 7.66, 7.73, 7.80, 7.88, 7.95, 8.02, 8.09, 8.17, 8.24, 
      8.31, 8.38, 8.46, 8.53, 8.60, 8.67, 8.75, 8.82, 8.89, 8.96, 
      9.04, 9.11, 9.18, 9.26, 9.33, 9.40, 9.47, 9.55, 9.62, 9.69, 
      9.76, 9.84, 9.91, 9.95, 10.08, 10.13, 10.20, 10.27, 10.34, 10.42, 
      10.49, 10.56, 10.63, 10.71, 10.78, 10.85, 10.92, 11.00, 11.07, 11.14, 
      11.22, 11.29, 11.36, 11.43, 11.51, 11.60, 11.69, 11.77, 11.86, 11.95, 
      12.04, 12.12, 12.21, 12.30, 12.38, 12.47, 12.56, 12.65, 12.73, 12.79, 
      12.94, 12.99, 13.08, 13.17, 13.26, 13.34, 13.43, 13.52, 13.60, 13.69, 
      13.78, 13.86, 13.95, 14.04, 14.13, 14.21, 14.30, 14.39, 14.47, 14.56, 
      14.65, 14.74, 14.82, 14.91, 15.00, 15.08, 15.17, 15.26, 15.35, 15.43, 
      15.52, 15.61
    };
  
  for (int i=0; i<nWG; i++)
    {
      if (wg >= 0 && wg == i)
	w = width[i];
    }

  if (w < 0.01)
    std::cout << "WARNING: strip width not defined properly... wg: " << wg << "\n";

  return w;
}






double CSCRecHitUFBuilder::stripWidth_1_2(const int wg) //in mm
{

  printWhereIm("stripWidth_1_2");

  double w = 0.000000000001;
  const int nWG = 64;
  double width[nWG] //in mm
    = {
      6.57, 6.62, 6.67, 6.72, 6.78, 6.83, 6.88, 6.93, 6.98, 7.03, 
      7.08, 7.14, 7.19, 7.24, 7.29, 7.34, 7.40, 7.46, 7.51, 7.57, 
      7.63, 7.69, 7.75, 7.79, 7.88, 7.99, 8.04, 8.10, 8.16, 8.22, 
      8.28, 8.34, 8.41, 8.47, 8.54, 8.61, 8.67, 8.74, 8.81, 8.87, 
      8.94, 9.00, 9.07, 9.14, 9.20, 9.27, 9.32, 9.42, 9.42, 9.48, 
      9.55, 9.63, 9.70, 9.77, 9.85, 9.92, 10.00, 10.08, 10.16, 10.24, 
      10.32, 10.40, 10.48, 10.57      
    };
  
  for (int i=0; i<nWG; i++)
    {
      if (wg >= 0 && wg == i)
	w = width[i];
    }

  if (w < 0.01)
    std::cout << "WARNING: strip width not defined properly... wg: " << wg << "\n";

  return w;
}






double CSCRecHitUFBuilder::stripWidth_1_3(const int wg) //in mm
{

  printWhereIm("stripWidth_1_3");

  double w = 0.000000000001;
  const int nWG = 32;
  double width[nWG] //in mm
    = {
      11.07, 11.18, 11.29, 11.40, 11.50, 11.61, 11.72, 11.83, 11.94, 12.05, 
      12.16, 12.25, 12.39, 12.48, 12.59, 12.70, 12.81, 12.92, 13.03, 13.14, 
      13.25, 13.34, 13.48, 13.57, 13.69, 13.80, 13.92, 14.03, 14.15, 14.26, 
      14.38, 14.50
    };
  
  for (int i=0; i<nWG; i++)
    {
      if (wg >= 0 && wg == i)
	w = width[i];
    }

  if (w < 0.01)
    std::cout << "WARNING: strip width not defined properly... wg: " << wg << "\n";

  return w;
}






double CSCRecHitUFBuilder::stripWidth_23_2(const int wg) //in mm
{

  printWhereIm("stripWidth_23_2");

  double w = 0.000000000001;
  const int nWG = 64;
  double width[nWG] //in mm
    = {
      8.53, 8.64, 8.76, 8.88, 8.99, 9.11, 9.23, 9.35, 9.46, 9.58, 
      9.70, 9.82, 9.93, 10.05, 10.17, 10.27, 10.42, 10.52, 10.64, 10.76, 
      10.88, 10.99, 11.11, 11.23, 11.35, 11.47, 11.58, 11.69, 11.83, 11.94, 
      12.05, 12.17, 12.29, 12.41, 12.52, 12.64, 12.76, 12.88, 13.00, 13.10, 
      13.25, 13.35, 13.47, 13.58, 13.70, 13.82, 13.94, 14.05, 14.17, 14.29, 
      14.41, 14.51, 14.66, 14.76, 14.88, 15.00, 15.11, 15.23, 15.35, 15.47, 
      15.58, 15.70, 15.82, 15.93
    };
  
  for (int i=0; i<nWG; i++)
    {
      //if (wg == 0)
      //std::cout << "WARNING: there is WG# = 0...\n";
      if (wg >= 0 && wg == i)
	w = width[i];
    }

  if (w < 0.01)
    std::cout << "WARNING: strip width not defined properly... wg: " << wg << "\n";

  return w;
}








double CSCRecHitUFBuilder::QuickCoordCorr(double x, double wg, const int id)
{

  printWhereIm("QuickCoordCorr");

  double corr2 = 1.0;
  double corr  = 1.0*corr2;
  if (x < 0)
    {
      corr = -corr;
      x = fabs(x);
    }
    
  const double c0 =  0.002538073;
  const double c1 =  0.070332773;
  const double c2 = -0.072870846;
  const double b1 = -11.0;
  const double b2 = -6.5;

  return corr*(c0 + c1*exp(b1*x) + c2*exp(b2*x));

}








int CSCRecHitUFBuilder::defineStripPart(double x)
{


  printWhereIm("defineStripPart");

  int stripPartNum = -10;		      
  for (int ii=0; ii<nStripDiv; ii++)
    {
      if ( x >= stripParts[ii] && x < stripParts[ii+1])
	stripPartNum = ii;
    }
  if (x >= stripParts[nStripDiv])
    stripPartNum = nStripDiv-1;
  if (x < stripParts[0])
    stripPartNum = 0;

  return stripPartNum;
}








double CSCRecHitUFBuilder::stripShift(int layer, const int id)
{

  printWhereIm("stripShift");

  double corr;
  if (id == 11)
    corr = 0.0;
  else
    {
      if (layer == 2 || layer == 4 || layer == 6)      
	corr = -0.25;
      else
	corr =  0.25;
    }

  return corr;
}







void CSCRecHitUFBuilder::defineLinFitParam5of6(double &slope, 
					      double &constant,
					      const int refLayer,
					      const std::vector<double> quickCoord,//
					      const std::vector<double> quickCoordLayer,//
					      const std::vector<double> quickCoordY,//
					      const std::vector<double> quickCoordStrip,
					      const std::vector<int> quickCoordKeyWG,//
					      const std::vector<int> quickCoordKeyWGSegment,
					      const std::vector<int> quickCoordCscType
					      )
{


  printWhereIm("defineLinFitParam5of6");

  int j = refLayer;
  double N=0, X=0, Y=0, XX=0, XY=0;

  for (int i=0; i<int(quickCoord.size()); i++)
    {
      if ( (i != j && refLayer >= 0) || (refLayer < 0) )
	{
	  double fabsR = fabs(quickCoord[i]);		      		      

	  int wgSegmentNum = quickCoordKeyWGSegment[i];		      
	  int stripPartNum = defineStripPart(fabsR);

	  double sigma = tabulatedLinFitSigma(stripPartNum, wgSegmentNum, quickCoordCscType[i]);
	  double sigma2 = sigma*sigma;		      

	  double corr = stripShift(int(quickCoordLayer[i]), quickCoordCscType[i]);

	  double quickCoordX = quickCoordStrip[i] + corr + quickCoord[i];

	  N += 1.0/sigma2;
	  Y += quickCoordX/sigma2;
	  X += quickCoordY[i]/sigma2;
	  XX += quickCoordY[i]*quickCoordY[i]/sigma2;
	  XY += quickCoordX*quickCoordY[i]/sigma2;

	} // if (i != j)
    } // for (int i=0; i<int(quickCoord.size()); i++)

  slope = (N*XY - Y*X)/(N*XX-X*X);
  constant = (Y-slope*X)/N;
}













double CSCRecHitUFBuilder::defineResid5of6(const double slope, 
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
					  )
{

  printWhereIm("defineResid5of6");

  //const int doPr = 0;


  double chi2 = 0;
  int chi2counter = 0;
  const int nParamForChi2 = 2;

  int minLayer = 100, maxLayer = -100;
  double minX = 0, maxX = 0;

  double chi2_6planes = 0;

  for (int i=0; i<int(quickCoord.size()); i++)
    {
      
      double fabsR = fabs(quickCoord[i]);
	  
      int wgSegmentNum = quickCoordKeyWGSegment[i];
      int stripPartNum = defineStripPart(fabsR);
	  
      double sigma = tabulatedLinFitSigma(stripPartNum, wgSegmentNum, quickCoordCscType[i]);
      double sigma2 = sigma*sigma;		      
	  
      double corr = stripShift(int(quickCoordLayer[i]), quickCoordCscType[i]);
	  
      double fitCoord = (slope*quickCoordY[i] + constant);
      double quickCoordX = (quickCoordStrip[i] + corr + quickCoord[i]);
      double quickCoordResid = quickCoordX - fitCoord;

      if (refLayer < 0)
	{
	  chi2_6planes += quickCoordResid*quickCoordResid/sigma2;
	  chi2counter++;	  
	}
      else
	{
	  if (i != refLayer)
	    {
	      chi2 += quickCoordResid*quickCoordResid/sigma2;
	      chi2counter++;
	    }
	  else
	    {
	      fitCoordVec.push_back(fitCoord);
	      residualsVec.push_back(quickCoordResid);
	      sigmaVec.push_back(sigma);
	      stripPartNumVec.push_back(stripPartNum);
	      wgSegmentNumVec.push_back(wgSegmentNum);
	    }
	}//if else (refLayer < 0)

      if (i < minLayer)
	{
	  minLayer = i;//refLayer;
	  minX = fitCoord;
	}
      if (i > maxLayer)
	{
	  maxLayer = i;//refLayer;
	  maxX = fitCoord;
	}      

    }//for (int i=0; i<int(quickCoord.size()); i++)


  //angle: start
  double deltaX = (maxX-minX)*stripWidth(quickCoordKeyWG[0], quickCoordCscType[0]);
  double detectorThickness = 128.625;//mm

  if (quickCoordCscType[0] == 11)
    {
      detectorThickness = 110.000;//mm
      deltaX = (maxX-minX)*stripWidth_ME11(quickCoordKeyWG[0],int(quickCoordStrip[0]));
    }

  angle = atan(deltaX/detectorThickness);
  if (debugPrint)
    std::cout 
      << maxX << " "
      << minX << " "
      << deltaX << " " 
      << angle << "\n";
  //angle: end


  // normalize chi2 to the number of deegrees of freedom: nHitsForResolStudy points - 2 parameter
  if (refLayer < 0)
    {
      double t = chi2_6planes/double(chi2counter - nParamForChi2);
      chi2 = t;//Chi2all_6planes;
    }
  else
    {
      chi2 = chi2/double(chi2counter - nParamForChi2);
    }


  return chi2;
}












//aaa
void CSCRecHitUFBuilder::trackSegmentLinearFit5of6(std::vector<double> quickCoord,
						  std::vector<double> quickCoordLayer,
						  std::vector<double> quickCoordY,
						  std::vector<double> quickCoordStrip,
						  std::vector<int> quickCoordKeyWG,
						  std::vector<int> quickCoordKeyWGSegment,
						  std::vector<double> quickCoordQc, 
						  std::vector<double> quickCoordQl, 
						  std::vector<double> quickCoordQr,
						  std::vector<int> quickCoordCscType
						  ) 
{


  printWhereIm("trackSegmentLinearFit5of6");

  double angle=0;
  double chi2 = 0;
  std::vector<double> chi2v;
  std::vector<double> angleVec;
  std::vector<double> slopeVec;
  std::vector<double> constantVec;

  std::vector<double> sigmaVec;
  std::vector<double> fitCoordVec;
  std::vector<double> residualsVec;
  std::vector<int> stripPartNumVec;
  std::vector<int> wgSegmentNumVec;

  //const int doPr = 0;

  double slope_6planes=0, constant_6planes=0;

  if (debugPrint)
    {
      std::cout << "int(quickCoord.size()): " << int(quickCoord.size()) << std::endl;
      for (int i=0; i<int(quickCoord.size()); i++)
	std::cout 
	  << " q " << quickCoord[i]
	  << " Layer " << quickCoordLayer[i]
	  << " Y " << quickCoordY[i]
	  << " Strip " << quickCoordStrip[i]
	  << " KeyWG " << quickCoordKeyWG[i]
	  << " KeyWGSegment " << quickCoordKeyWGSegment[i]
	  << " Qc " << quickCoordQc[i]
	  << " Ql " << quickCoordQl[i]
	  << " Qr " << quickCoordQr[i]
	  << " CscType " <<  quickCoordCscType[i]
	  << std::endl;
    }

	  
  if (int(quickCoord.size()) > 6)
    std::cout << "WARNING: int(quickCoord.size()) > 6: " << int(quickCoord.size()) << std::endl;


  if (int(quickCoord.size()) >= minHitsInSegment 
      && int(quickCoord.size()) >= 2 
      && int(quickCoord.size()) <= 6 
      )
    {
	  
      double minChi2_N_1 = 10000000000.0;
      int minChi2_i = -1;
      double chi2N = 10000000000.0;

      int maxChi2_i = -1;
      double maxChi2_N_1 = 0;

      double Chi2ofL3 = 1000000000.0;
      int firstRound = 1;

      do
	{
	  double slope, constant;
	  int refLayer = -1;
	  defineLinFitParam5of6(PARAM);
	  chi2N = defineResid5of6(RESID);
	  if (firstRound)
	    {
	      Chi2all_6planes = chi2N;
	      firstRound = 0;
	    }
	      
	  if ( int(quickCoord.size()) > 2 )
	    {
	      for (int j=0; j<int(quickCoord.size()); j++)
		{
		  refLayer = j;

		  defineLinFitParam5of6(PARAM);
		  slopeVec.push_back(slope);
		  constantVec.push_back(constant);		

		  chi2 = defineResid5of6(RESID);
		  chi2v.push_back(chi2);

		  if (minChi2_N_1 > chi2)
		    {
		      minChi2_N_1 = chi2;		      
		      minChi2_i = refLayer;
		    }
		  if (maxChi2_N_1 < chi2)
		    {
		      maxChi2_N_1 = chi2;		      
		      maxChi2_i = refLayer;
		    }
		  angleVec.push_back(angle);

		  if (refLayer == 2)
		    {
		      Chi2ofL3 = chi2;
		    }

		}//for (int j=0; j<int(quickCoord.size()); j++)


	      if ( doPruning 
		   && (int(quickCoord.size())-1) >= minHitsInSegment 
		   && chi2N > Chi2threshold
		   )
		{
		  std::vector<double> quickCoord_ = quickCoord;
		  std::vector<double> quickCoordLayer_ = quickCoordLayer;
		  std::vector<double> quickCoordY_ = quickCoordY;
		  std::vector<double> quickCoordStrip_ = quickCoordStrip;
		  std::vector<int> quickCoordKeyWG_ = quickCoordKeyWG;
		  std::vector<int> quickCoordKeyWGSegment_ = quickCoordKeyWGSegment;
		  std::vector<double> quickCoordQc_ = quickCoordQc; 
		  std::vector<double> quickCoordQl_ = quickCoordQl; 
		  std::vector<double> quickCoordQr_ = quickCoordQr;
		  std::vector<int> quickCoordCscType_ = quickCoordCscType;
		  chi2v.clear();
		  angleVec.clear();
		  slopeVec.clear();
		  constantVec.clear();
		  sigmaVec.clear();
		  fitCoordVec.clear();
		  residualsVec.clear();
		  stripPartNumVec.clear();
		  wgSegmentNumVec.clear();

		  quickCoord.clear();
		  quickCoordLayer.clear();
		  quickCoordY.clear();
		  quickCoordStrip.clear();
		  quickCoordKeyWG.clear();
		  quickCoordKeyWGSegment.clear();
		  quickCoordQc.clear();
		  quickCoordQl.clear();
		  quickCoordQr.clear();
		  quickCoordCscType.clear();

		  for (int j=0; j<int(quickCoord_.size()); j++)
		    {
		      if (j != maxChi2_i)
			{
			  quickCoord.push_back(quickCoord_[j]);
			  quickCoordLayer.push_back(quickCoordLayer_[j]);
			  quickCoordY.push_back(quickCoordY_[j]);
			  quickCoordStrip.push_back(quickCoordStrip_[j]);
			  quickCoordKeyWG.push_back(quickCoordKeyWG_[j]);
			  quickCoordKeyWGSegment.push_back(quickCoordKeyWGSegment_[j]);
			  quickCoordQc.push_back(quickCoordQc_[j]);
			  quickCoordQl.push_back(quickCoordQl_[j]);
			  quickCoordQr.push_back(quickCoordQr_[j]);
			  quickCoordCscType.push_back(quickCoordCscType_[j]);
			}
		    }//for (int j=0; j<int(quickCoord.size()); j++)
		}//if (doPruning...
	    }//if ( int(quickCoord.size()) > 2 )
	} while ( (int(quickCoord.size())-1) >= minHitsInSegment 
		  && doPruning
		  && chi2N > Chi2threshold
		  );


      double angle_6planes = 100000;
      Chi2_min_5planes = minChi2_N_1;
      {// N of N planes: start

	double slope, constant;
	int refLayer = -1;
	defineLinFitParam5of6(PARAM);

	slope_6planes = slope;
	constant_6planes = constant;
	
	Chi2all_6planes = defineResid5of6(RESID);
	angle_6planes = angle;


	if (1 || debugPrint && Chi2_min_5planes > 10)
	  {
	    for (int ii=0; ii<int(quickCoord.size()); ii++)
	      {
		std::cout
		  << eventNumber
		  << " nHits: " << int(quickCoord.size())
		  << " Chi2all_Nplanes: " << Chi2all_6planes
		  << " Chi2_min_(N-1)planes: " << Chi2_min_5planes
		  << " CscT: " << quickCoordCscType[ii]
		  << " Layer: " << quickCoordLayer[ii]
		  << " strip: " << quickCoordStrip[ii]
		  << " resid: " << residualsVec[ii]
		  << " Q: " << quickCoordQc[ii]+quickCoordQr[ii]+quickCoordQl[ii]
		  << std::endl;
	      }//for (int ii=0; ii<int(quickCoord.size()); ii++)
	  }//if (Chi2_min_5planes > 10)

      }// N of N planes: end



      if (Chi2ofL3 < linFitChi2threshold && fabs(angle_6planes) < linFitAngleCut)
	{
	  for (int j=0; j<int(quickCoord.size()); j++)
	    {

	      /*
		const int indexG = cscIndexFromType(quickCoordCscType[j]);

		double quickCoordResid = residualsVec[j];
		double sigma = sigmaVec[j];
		int stripPartNum = stripPartNumVec[j];
		int wgSegmentNum = wgSegmentNumVec[j];

		double ccc;
		double cc;
		cc = modf((slope_6planes*(0.0) + constant_6planes), &ccc) - 0.5;	      
	      */      


	    } // for (int j=0; j<int(quickCoord.size()); j++)

	} // if (Chi2all_6planes < linFitChi2threshold && fabs(angle_6planes) < linFitAngleCut)

    } // if (int(quickCoord.size()) >= minHitsInSegment)

}//void CSCRecHitUFBuilder::trackSegmentLinearFit5of6(...











double CSCRecHitUFBuilder::tabulatedLinFitSigma(int sp, int hv, const int id)
{

  printWhereIm("tabulatedLinFitSigma");

  // id = (11, 12,) 13, 31, 41 need to be updated

  if (id == 234)
    return tabulatedLinFitSigma_23_2(sp, hv);
  if (id == 12 || id == 13)
    return tabulatedLinFitSigma_1_2(sp, hv);
  if (id == 21 || id == 31 || id == 41)
    return tabulatedLinFitSigma_2_1(sp, hv);
  if (id == 11)
    return tabulatedLinFitSigma_1_1(sp, hv);

  return 0.05;

}




double CSCRecHitUFBuilder::tabulatedLinFitSigma_1_1(int sp, int hv)
{
  // STATISTICS LIMITED... 

  printWhereIm("tabulatedLinFitSigma_1_1");

  const int nSP = 5;
  const int nHV = 1;
  double s[nSP][nHV] =
    {
      {0.02466},
      {0.02067},
      {0.03006},
      {0.04188},
      {0.02119}
    };
  return s[sp][hv];
}





double CSCRecHitUFBuilder::tabulatedLinFitSigma_2_1(int sp, int hv)
{
  // TAKEN FROM 234_2 FOR THE MOMENT

  printWhereIm("tabulatedLinFitSigma_2_1");

  const int nSP = 5;
  const int nHV = 3;
  double s[nSP][nHV] =
    {
      {0.05189, 0.05584, 0.08133},
      {0.04654, 0.04886, 0.06273},
      {0.04129, 0.03765, 0.04138},
      {0.03686, 0.02963, 0.03143},
      {0.03072, 0.02698, 0.02695}
    };
  return s[sp][hv];
}





double CSCRecHitUFBuilder::tabulatedLinFitSigma_1_2(int sp, int hv)
{
  // STATISTICS LIMITED...

  printWhereIm("tabulatedLinFitSigma_1_2");

  const int nSP = 5;
  const int nHV = 3;
  double s[nSP][nHV] =
    {
      {0.05113, 0.03831,  0.04139}, 
      {0.04585, 0.03473,  0.03769}, 
      {0.03973, 0.03577,  0.03944}, 
      {0.04207, 0.03289,  0.03069}, 
      {0.03789, 0.03117,  0.02408}
    };
  return s[sp][hv];
}





double CSCRecHitUFBuilder::tabulatedLinFitSigma_23_2(int sp, int hv)
{


  printWhereIm("tabulatedLinFitSigma_23_2");

  const int nSP = 5;
  const int nHV = 5;
  // dynamic-pedestals, HALF, cut on angle, Chi2, ... see last run on 21 Mar 07
  // backup.03-22-2007_10-22-16.run4188.ALLev.resol-study-L3_chi2cut-angle_cut-one_MLCT-HALF.OK
  // sp0_hv0_l2, sp0_hv1_l2, sp0_hv2_l2, ...
  // sp1_hv0_l2, ...
  double s[nSP][nHV] =
    {
      {0.05189, 0.05439,  0.05584, 0.06601, 0.08133},
      {0.04654, 0.04585,  0.04886, 0.05413, 0.06273},
      {0.04129, 0.03967,  0.03765, 0.03995, 0.04138},
      {0.03686, 0.03281,  0.02963, 0.02992, 0.03143},
      {0.03072, 0.02735,  0.02698, 0.02486, 0.02695}
    };



  /*
  // (di-strips):
  double sd[nSP][nHV] =
  {
  {0.05644, 0.06011, 0.06508, 0.06657, 0.07165},
  {0.06457, 0.06168, 0.06606, 0.06667, 0.06998},
  {0.06578, 0.05725, 0.05111, 0.05795, 0.06004},
  {0.05760, 0.04939, 0.05518, 0.05295, 0.04911},
  {0.05019, 0.05242, 0.04567, 0.05471, 0.05017}
  };



  if (selectDiStripsOnly)
  {
  for (int i=0; i<nSP; i++)
  {
  for (int j=0; j<nHV; j++)
  {
  s[i][j] = sd[i][j];
  }
  }
  }
  */

  return s[sp][hv];
}






//////////////////////////////////////////////////////////////////////////////////////////////////////////
// UF algo: end
//////////////////////////////////////////////////////////////////////////////////////////////////////////










/* Member function build
 *
 */
void CSCRecHitUFBuilder::build( const CSCStripDigiCollection* strips, 
			       const CSCWireDigiCollection* wires,
			       const CSCALCTDigiCollection* alcts,
			       const CSCCLCTDigiCollection* clcts,
			       CSCRecHit2DCollection& oc )
{
  

  /*
    UF algo: start
  */
  {

    printWhereIm("build");

    //TimeMe t1("NewAlgoTiming", false);

    if (debugPrint) std::cout << "\n-------------- Event Number: " << eventNumber << " --------------\n";
    eventNumber++;
    
    int goodEvent = 0;
    for (CSCALCTDigiCollection::DigiRangeIterator j=alcts->begin(); j!=alcts->end(); j++) 
      { 
	CSCDetId myDetId = (*j).first;

	if (isCscON(myDetId))
	  {
	    goodEvent = 1;
	  }
      }
    for (CSCCLCTDigiCollection::DigiRangeIterator j=clcts->begin(); j!=clcts->end(); j++) 
      { 
	CSCDetId myDetId = (*j).first;
	if (isCscON(myDetId))
	  {
	    goodEvent = 1;
	  }
      }
  


    if (( 1 || debugPrint) && int(eventNumber)%10000 == 0)
      std::cout << "Passing event #" << eventNumber << std::endl;


    
    if (debugPrint) std::cout << "\n-------------- Found ALCTs --------------\n";
    std::vector<int> keyWG0;
    std::vector<int> keyWG1;
    std::vector<CSCDetId> keyWGDetId0;
    std::vector<CSCDetId> keyWGDetId1;
    findValidALCTs(alcts, keyWG0, keyWG1, keyWGDetId0, keyWGDetId1);
    if (debugPrint)
      {
	for (int i=0; i<int(keyWG0.size()); i++)
	  std::cout << "DetId: " << keyWGDetId0[i] << "  keyWG0: " << keyWG0[i] << std::endl;
	for (int i=0; i<int(keyWG1.size()); i++)
	  std::cout << "DetId: " << keyWGDetId1[i] << "  keyWG1: " << keyWG1[i] << std::endl;
      }


    if (debugPrint) std::cout << "\n-------------- Found CLCTs --------------\n";
    std::vector<int> keyHS0;
    std::vector<int> keyHS1;
    std::vector<int> halfOrDi0;
    std::vector<int> halfOrDi1;
    std::vector<CSCDetId> keyHSDetId0;
    std::vector<CSCDetId> keyHSDetId1;
    findValidCLCTs(clcts, keyHS0, keyHS1, halfOrDi0, halfOrDi1, keyHSDetId0, keyHSDetId1);
    if (debugPrint)
      {
	for (int i=0; i<int(keyHS0.size()); i++)
	  std::cout << "DetId: " << keyHSDetId0[i] << "  keyHS0: " << keyHS0[i] << "  halfOrDi0: " << halfOrDi0[i] << std::endl;
	for (int i=0; i<int(keyHS1.size()); i++)
	  std::cout << "DetId: " << keyHSDetId1[i] << "  keyHS1: " << keyHS1[i] << "  halfOrDi1: " << halfOrDi1[i] << std::endl;
      }

  
    if (debugPrint) std::cout << "\n-------------- Matched ALCTs and CLCTs --------------\n";
    std::vector<int> keyWG;
    std::vector<int> keyHS;
    std::vector<int> halfOrDi;
    std::vector<CSCDetId> keyDetId;
    matchALCTandCLCT(keyWG0, keyWG1, 
		     keyWGDetId0, keyWGDetId1,
		     keyHS0, keyHS1, 
		     halfOrDi0, halfOrDi1, 
		     keyHSDetId0, keyHSDetId1,
		     keyWG, keyHS, halfOrDi, keyDetId);
    if (debugPrint)
      {
	for (int i=0; i<int(keyHS.size()); i++)
	  std::cout << "DetId: " << keyDetId[i] 
		    << "  keyWG: " << keyWG[i] 
		    << "  keyHS: " << keyHS[i]
		    << "  halfOrDi: " << halfOrDi[i]
		    << std::endl;
      }


    if (debugPrint) std::cout << "\n-------------- Calculated Coordinates --------------\n";
    if ( int(keyWG.size()) > 0 )
      quickCoordinate(strips,keyWG,keyHS,halfOrDi,keyDetId);


    

  }
  /*
    UF algo: end 
  */



  






  /*
    Old algo part starts here...
  */
  if (0) {
    /*

    TimeMe t2("OldAlgoTiming", false);



    if ( useCalib ) {
    // Pass gain constants to strip hit reconstruction package
    HitsFromStripOnly_->setCalibration( gains_ );
    // Pass X-talks and noise matrix to 2-D hit builder (these are ultimately implemented in Gatti fit)
    Make2DHits_->setCalibration( gains_, xtalk_, noise_ );
    }

  
    // Make collection of wire only hits !  
    CSCWireHitCollection woc;
  
    for ( CSCWireDigiCollection::DigiRangeIterator it = wiredc->begin(); it != wiredc->end(); ++it ){
    const CSCDetId& id = (*it).first;
    const CSCLayer* layer = getLayer( id );
    const CSCWireDigiCollection::Range rwired = wiredc->get( id );
    
    // Skip if no wire digis in this layer
    if ( rwired.second == rwired.first ) continue;
    
    std::vector<CSCWireHit> rhv = HitsFromWireOnly_->runWire( id, layer, rwired );
    
    // Add the wire hits to master collection
    woc.put( id, rhv.begin(), rhv.end() );
    }

    // Clean up the wire hit collection by trying to build wire segments
    CSCWireHitCollection clean_woc;  
    if ( useCleanWireCollection ) {
    // First pass geometry
    HitsFromWireSegments_->setGeometry( geom_ );
    // Now try building segments
    clean_woc = HitsFromWireSegments_->cleanWireHits(woc);
    } else {
    clean_woc = woc;
    }

  
    // Make collection of strip only hits
  
    CSCStripHitCollection soc;  
    for ( CSCStripDigiCollection::DigiRangeIterator it = stripdc->begin(); it != stripdc->end(); ++it ){
    const CSCDetId& id = (*it).first;
    const CSCLayer* layer = getLayer( id );
    const CSCStripDigiCollection::Range& rstripd = (*it).second;
    
    // Skip if no strip digis in this layer
    if ( rstripd.second == rstripd.first ) continue;
    
    std::vector<CSCStripHit> rhv = HitsFromStripOnly_->runStrip( id, layer, rstripd );
    
    // Add the strip hits to master collection
    soc.put( id, rhv.begin(), rhv.end() );
    }

    // Now clean up the strip hit collection by trying to build strip segments  
    CSCStripHitCollection clean_soc;
    if ( useCleanStripCollection ) {
    // First pass geometry
    HitsFromStripSegments_->setGeometry( geom_ );
    clean_soc = HitsFromStripSegments_->cleanStripHits(soc);
    } else {
    clean_soc = soc;
    }


    // Sort clean hit collections by layer    

    if (debug) std::cout << "[CSCRecHitUFBuilder]  sorting hits by layer" << std::endl;
    std::vector<CSCDetId> stripLayer;
    std::vector<CSCDetId>::const_iterator sIt;
    std::vector<CSCDetId> wireLayer;
    std::vector<CSCDetId>::const_iterator wIt;
 
    // Sort strip hits  
    for ( CSCStripHitCollection::const_iterator it = clean_soc.begin(); it != clean_soc.end(); it++ ) {
    bool insert = true;
    for ( sIt=stripLayer.begin(); sIt != stripLayer.end(); ++sIt ) {
    if (((*it).cscDetId().chamber() == (*sIt).chamber()) &&
    ((*it).cscDetId().station() == (*sIt).station()) &&
    ((*it).cscDetId().ring()    == (*sIt).ring())    &&
    ((*it).cscDetId().endcap()  == (*sIt).endcap())  &&
    ((*it).cscDetId().layer()   == (*sIt).layer()))
    insert = false;
    }
    if ( insert ) stripLayer.push_back((*it).cscDetId());
    }
    if (debug) std::cout << "[CSCRecHitUFBuilder] Done sorting strip hits" << std::endl;
    if (debug) std::cout << "# of layers with strip hits: " << stripLayer.size() << std::endl;

    // Sort wire hits
    for ( CSCWireHitCollection::const_iterator it = clean_woc.begin(); it != clean_woc.end(); it++ ) {
    bool insert = true;
    for ( wIt=wireLayer.begin(); wIt != wireLayer.end(); ++wIt ) {
    if (((*it).cscDetId().chamber() == (*wIt).chamber()) &&
    ((*it).cscDetId().station() == (*wIt).station()) &&
    ((*it).cscDetId().ring()    == (*wIt).ring())    &&
    ((*it).cscDetId().endcap()  == (*wIt).endcap())  &&
    ((*it).cscDetId().layer()   == (*wIt).layer()))
    insert = false;
    }
    if ( insert ) wireLayer.push_back((*it).cscDetId());
    }
    if (debug) std::cout << "[CSCRecHitUFBuilder] Done sorting wire hits" << std::endl;
    if (debug) std::cout << "# of layers with wire hits: " << wireLayer.size() << std::endl;


    // Now create 2-D hits by looking at superposition of strip and wire hit in a layer
    //
    // N.B.  I've sorted the hits from layer 1-6 always, so can test if there are "holes", 
    // that is layers without hits for a given chamber.

    // Vector to store rechit within layer
    std::vector<CSCRecHit2D> hitsInLayer;

    if (debug) std::cout << "[CSCRecHitUFBuilder] Now trying to build 2D hits" << std::endl;

    int layer_idx     = 0;
    int hits_in_layer = 0;
    CSCDetId old_id; 

    // Now loop over each layer containing strip hits
    for ( sIt=stripLayer.begin(); sIt != stripLayer.end(); ++sIt ) {

    bool foundMatch = false;
    hitsInLayer.clear();
    hits_in_layer = 0;
   
    std::vector<CSCStripHit> cscStripHit;
    
    CSCRangeMapForRecHit acc;
    CSCStripHitCollection::range range = clean_soc.get(acc.cscDetLayer(*sIt));

    // Create vector of strip hits for this layer    
    for ( CSCStripHitCollection::const_iterator clean_soc = range.first; clean_soc != range.second; clean_soc++)
    cscStripHit.push_back(*clean_soc);

    const CSCDetId& sDetId = (*sIt);
    const CSCLayer* layer  = getLayer( sDetId );

    // This is used to test for gaps in layers and needs to be initialized here 
    if ( layer_idx == 0 ) {
    old_id = sDetId;
    }

    // This is the id I'll compare the wire digis against because of the ME1a confusion in data
    // i.e. ME1a == ME11 == ME1b for wire in data
    CSCDetId compId;

    // For ME11, real data wire digis are labelled as belonging to ME1b, 
    // so that's where ME1a must look
    if ((      isData          ) && 
    (sDetId.station() == 1 ) && 
    (sDetId.ring()    == 4 )) {
    int sendcap  = sDetId.endcap();
    int schamber = sDetId.chamber();
    int slayer   = sDetId.layer();
    CSCDetId testId( sendcap, 1, 1, schamber, slayer );
    compId = testId;
    } else {
    compId = sDetId;
    }

    // Test if layer gap in strip hit collection --> if so, build pseudo 2-D hit from wire hit
    // N.B. as is, cannot accomodate for 2 successive gaps in layers for strip hit 
    // e.g. if no strip hit in layer 1 and 2, can only try retrieving wire hit in layer 1.

    if ((          makePseudo2DHits          ) &&
    (sDetId.layer()   == old_id.layer()+2) &&
    (sDetId.endcap()  == old_id.endcap() ) &&  
    (sDetId.station() == old_id.station()) &&
    (sDetId.ring()    == old_id.ring()   ) &&
    (sDetId.chamber() == old_id.chamber())) {

    if ( debug ) std::cout << " Found gap in layer for strip hit collection " << std::endl;
    CSCDetId wDetId_modified;

    // Then loop over each layer containing wire hits and try
    // to see if have wire hits in gap
    for ( wIt=wireLayer.begin(); wIt != wireLayer.end(); ++wIt ) {
        
    const CSCDetId wDetId = (*wIt);
      
    if ((wDetId.endcap()  == compId.endcap() ) &&
    (wDetId.station() == compId.station()) &&
    (wDetId.ring()    == compId.ring()   ) &&
    (wDetId.chamber() == compId.chamber()) &&
    (wDetId.layer()   == compId.layer()  )) {

    // Create vector of wire hits for this layer
    std::vector<CSCWireHit> cscWireHit;   
    CSCRangeMapForRecHit acc2;
    CSCWireHitCollection::range range = clean_woc.get(acc2.cscDetLayer(*wIt));
        
    for ( CSCWireHitCollection::const_iterator clean_woc = range.first; clean_woc != range.second; clean_woc++)
    cscWireHit.push_back(*clean_woc);

    if ( debug ) std::cout << "# wire hits in layer: "<< cscWireHit.size() << std::endl;

    // Because of ME-1a problem, have to do the following trick:
    int wendcap  = sDetId.endcap();
    int wstation = sDetId.station();
    int wring    = sDetId.ring();
    int wchamber = sDetId.chamber();
    int wlayer   = wDetId.layer();   
    CSCDetId testId( wendcap, wstation, wring, wchamber, wlayer );
    wDetId_modified = testId;

    const CSCLayer* w_layer = getLayer( wDetId );
  
    // Build pseudo 2D hit for all wire hits within this layer
    for ( unsigned j = 0; j != cscWireHit.size(); j++ ) {
    const CSCWireHit w_hit = cscWireHit[j];
    CSCRecHit2D rechit = Make2DHits_->hitFromWireOnly( testId, w_layer, w_hit);
    bool isInFiducial = Make2DHits_->isHitInFiducial( w_layer, rechit );
    if ( isInFiducial ) {
    hitsInLayer.push_back( rechit );
    hits_in_layer++;
    }
    }
    }
    if ( hits_in_layer > 0 ) {
    oc.put( wDetId_modified, hitsInLayer.begin(), hitsInLayer.end() );
    hitsInLayer.clear();
    if ( debug ) std::cout << "# of rechit/ layer = " << hits_in_layer << std::endl;
    hits_in_layer = 0;
    old_id = wDetId_modified;
    }      
    }
    }

    // Now try to build proper 2D rechit from overlap of strip/wire hits

    for ( wIt=wireLayer.begin(); wIt != wireLayer.end(); ++wIt ) {
        
    const CSCDetId& wDetId  = (*wIt);
        
    // Again, because of ME1a, use the compId to make a comparison between strip and wire hit CSCDetId
    if ((wDetId.endcap()  == compId.endcap() ) &&
    (wDetId.station() == compId.station()) &&
    (wDetId.ring()    == compId.ring()   ) &&
    (wDetId.chamber() == compId.chamber()) &&
    (wDetId.layer()   == compId.layer()  )) {
          
    // Create vector of wire hits for this layer
    std::vector<CSCWireHit> cscWireHit;
       
    CSCRangeMapForRecHit acc2;
    CSCWireHitCollection::range range = clean_woc.get(acc2.cscDetLayer(*wIt));
      
    for ( CSCWireHitCollection::const_iterator clean_woc = range.first; clean_woc != range.second; clean_woc++)
    cscWireHit.push_back(*clean_woc);

    // Build 2D hit for all possible strip-wire pairs 
    // overlapping within this layer
    if (debug) std::cout << "# strip hits in layer: " << cscStripHit.size() << "  " 
    << "# wire hits in layer: "  << cscWireHit.size()  << std::endl;

    for (unsigned i = 0; i != cscStripHit.size(); i++ ) {
    const CSCStripHit s_hit = cscStripHit[i];
    for (unsigned j = 0; j != cscWireHit.size(); j++ ) {
    const CSCWireHit w_hit = cscWireHit[j];
    CSCRecHit2D rechit = Make2DHits_->hitFromStripAndWire(sDetId, layer, w_hit, s_hit);
    bool isInFiducial = Make2DHits_->isHitInFiducial( layer, rechit );
    if ( isInFiducial ) {
    foundMatch = true;  
    hitsInLayer.push_back( rechit );
    hits_in_layer++;
    }
    }
    }
    }
    }

    if ( hits_in_layer < 1 && makePseudo2DHits ) { 
    // If missing wire hit in a layer, form pseudo 2-D hit from strip hit
    if ( debug ) std::cout << " Found gap in layer for wire hit collection " << std::endl;      
    if ( debug ) std::cout << "# strip hits in layer: "<< cscStripHit.size() << std::endl;

    for (unsigned i = 0; i != cscStripHit.size(); i++ ) {
    const CSCStripHit s_hit = cscStripHit[i];
    CSCRecHit2D rechit = Make2DHits_->hitFromStripOnly(sDetId, layer, s_hit);
    hitsInLayer.push_back( rechit );
    hits_in_layer++;
    }
    }

    // output vector of 2D rechits to collection
    if (hits_in_layer > 0) {
    oc.put( sDetId, hitsInLayer.begin(), hitsInLayer.end() );
    hitsInLayer.clear();
    }
    if ( debug ) std::cout << "# hits in layer: "<< hits_in_layer << std::endl;
    hits_in_layer = 0;
    layer_idx++;
    old_id = sDetId;
    }
    */
  } // if (0) // old algo ends...


}


/* getLayer
 *
 */
/*
  const CSCLayer* CSCRecHitUFBuilder::getLayer( const CSCDetId& detId )  {
  if ( !geom_ ) throw cms::Exception("MissingGeometry") << "[CSCRecHitUFBuilder::getLayer] Missing geometry" << std::endl;
  return geom_->layer(detId);
  }
*/
