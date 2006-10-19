#include "RecoBTag/Analysis/src/Tools.h"

#include <iostream>

#include "TROOT.h"
#include "TSystem.h"

using namespace std ;

//
//
// TOOLS
//
//


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double HistoBinWidth ( TH1F * theHisto , int iBin ) {
  int nBins = theHisto->GetSize() ; // includes underflow/overflow
  // return 0.0 , if invalid bin
  if ( iBin < 0 || iBin >= nBins ) return 0.0 ;
  // return first binwidth, if underflow bin
  if ( iBin == 0 ) return theHisto->GetBinWidth ( 1 ) ;
  // return last real binwidth, if overflow bin
  if ( iBin == nBins - 1 ) return theHisto->GetBinWidth ( nBins - 2 ) ;
  // return binwidth from histo, if within range
  return theHisto->GetBinWidth ( iBin ) ;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double IntegrateHistogram ( TH1F * theHisto ) {
  // include underflow and overflow: assign binwidth of first/last bin to them!!
  // integral = sum ( entry_i * binwidth_i )
  //
  double histoIntegral = 0.0 ;
  int    nBins = theHisto->GetSize() ;
  //
  // loop over bins:
  // bin 0       : underflow
  // bin nBins-1 : overflow
  for ( int iBin = 0 ; iBin < nBins ; iBin++ ) {
    double binWidth = HistoBinWidth ( theHisto , iBin )  ;
    histoIntegral += (*theHisto)[iBin] * binWidth ;
  }
  //
  return histoIntegral ;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void HistoToNormalizedArrays ( TH1F * theHisto , TArrayF & theNormalizedArray , TArrayF & theLeftOfBinArray , TArrayF & theBinWidthArray ) {

  int nBins = theHisto->GetSize() ;

  // check that all arrays/histo have the same size
  if  ( nBins == theNormalizedArray.GetSize()   &&
	nBins == theLeftOfBinArray .GetSize()   &&
	nBins == theBinWidthArray  .GetSize()       ) {

    double histoIntegral = IntegrateHistogram ( theHisto ) ;

    for ( int iBin = 0 ; iBin < nBins ; iBin++ ) {
      theNormalizedArray[iBin] = (*theHisto)[iBin] / histoIntegral  ;
      theLeftOfBinArray [iBin] = theHisto->GetBinLowEdge(iBin) ;
      theBinWidthArray  [iBin] = HistoBinWidth ( theHisto , iBin )  ;
    }

  }
  else {
    cout << "============>>>>>>>>>>>>>>>>" << endl
	 << "============>>>>>>>>>>>>>>>>" << endl
	 << "============>>>>>>>>>>>>>>>>" << endl
	 << "============>>>>>>>>>>>>>>>>" << endl
	 << "============>>>>>>>>>>>>>>>> HistoToNormalizedArrays failed: not equal sizes of all arrays!!" << endl
	 << "============>>>>>>>>>>>>>>>>" << endl
	 << "============>>>>>>>>>>>>>>>>" << endl
	 << "============>>>>>>>>>>>>>>>>" << endl
	 << "============>>>>>>>>>>>>>>>>" << endl ;
  }

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double IntegrateArray ( const TArrayF & theArray , const TArrayF & theBinWidth ) {

  double arrayIntegral = 0.0 ;
  int    nBins = theArray.GetSize() ;
  //
  for ( int iBin = 0 ; iBin < nBins ; iBin++ ) {
    arrayIntegral += theArray[iBin] * theBinWidth[iBin] ;
  }
  //
  return arrayIntegral ;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PrintCanvasHistos ( TCanvas * canvas , TString psFile , TString epsFile , TString gifFile ) {
  //
  //
  // to create gif in 'batch mode' (non-interactive) see
  // http://root.cern.ch/cgi-bin/print_hit_bold.pl/root/roottalk/roottalk00/0402.html?gifbatch#first_hit
  //
  // ROOT 4 can do it!!??
  //
  // if string = "" don't print to corresponding file
  //
  if ( psFile  != "" ) canvas->Print ( psFile.Data() ) ;
  if ( epsFile != "" ) canvas->Print ( epsFile.Data() , "eps" ) ;
  // if in batch: use a converter tool
  TString rootVersion ( gROOT->GetVersion() ) ;
  bool rootCanGif = rootVersion.BeginsWith ("4") || rootVersion.BeginsWith ("5") ;
  if ( gifFile != "" ) {
    if ( !(gROOT->IsBatch()) || rootCanGif )  { // to find out if running in batch mode
      cout << "--> Print directly gif!" << endl ;
      canvas->Print ( gifFile.Data() , "gif" ) ;
    }
    else {
      if ( epsFile != "" ) {   // eps file must have been created before
	cout << "--> Print gif via scripts!" << endl ;
	TString executeString1 = "pstopnm -ppm -xborder 0 -yborder 0 -portrait " + epsFile ;
	gSystem->Exec(executeString1) ;
	TString ppmFile = epsFile + "001.ppm" ;
	TString executeString2 = "ppmtogif " + ppmFile + " > " + gifFile ;
	gSystem->Exec(executeString2) ;
	TString executeString3 = "rm " + ppmFile  ;
	gSystem->Exec(executeString3) ; // delete the intermediate file
      }
    }
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TObjArray getHistArray ( TFile * histoFile , TString baseName ) {
  //
  // return the TObjArray built from the basename
  //
  //
  TObjArray histos (3) ;  // reserve 3
  //
  TString nameB    ( baseName ) ;
  TString nameC    ( baseName ) ;
  TString nameDUSG ( baseName ) ;
  //
  nameB    += "B"    ;
  nameC    += "C"    ;
  nameDUSG += "DUSG" ;
  // Data() converts TString to a char*
  histos.Add ( (TH1F*)histoFile->Get( nameB   .Data() ) ) ;
  histos.Add ( (TH1F*)histoFile->Get( nameC   .Data() ) ) ;
  histos.Add ( (TH1F*)histoFile->Get( nameDUSG.Data() ) ) ;
  //
  return histos ;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TString flavour ( int flav ) {
  // THIS ONE IS VERY SLOW BECAUSE IT USES TSTRING!!
  // ==> AVOID TO USE!!
  TString FlavString = "" ;

  if ( flav == 1  ) FlavString = "d";
  if ( flav == 2  ) FlavString = "u";
  if ( flav == 3  ) FlavString = "s";
  if ( flav == 4  ) FlavString = "c";
  if ( flav == 5  ) FlavString = "b";
  if ( flav == 21 ) FlavString = "g";

  return FlavString ;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool flavourIsD ( const int & flav ) { return flav == 1  ; }
bool flavourIsU ( const int & flav ) { return flav == 2  ; }
bool flavourIsS ( const int & flav ) { return flav == 3  ; }
bool flavourIsC ( const int & flav ) { return flav == 4  ; }
bool flavourIsB ( const int & flav ) { return flav == 5  ; }
bool flavourIsG ( const int & flav ) { return flav == 21 ; }

bool flavourIsDUS  ( const int & flav ) { return ( flavourIsD(flav) || flavourIsU(flav) || flavourIsS(flav) ) ; }
bool flavourIsDUSG ( const int & flav ) { return ( flavourIsDUS(flav) || flavourIsG(flav) ) ; }

bool flavourIsNI  ( const int & flav ) { return !( flavourIsD(flav) || flavourIsU(flav) || flavourIsS(flav) ||
						 flavourIsC(flav) || flavourIsB(flav) || flavourIsG(flav)    ) ; }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int  checkCreateDirectory ( TString directory ) {
  cout << "====>>>> ToolsC:checkCreateDirectory() : " << endl ;
  int exists = gSystem->Exec ( "ls -d " + directory ) ;
  // create it if it doesn't exist
  if ( exists != 0 ) {
    cout << "====>>>> ToolsC:checkCreateDirectory() : The directory does not exist : " << directory << endl ;
    cout << "====>>>> ToolsC:checkCreateDirectory() : I'll try to create it" << endl ;
    int create = gSystem->Exec ( "mkdir " + directory ) ;
    if ( create != 0 ) {
      cout << "====>>>> ToolsC:checkCreateDirectory() : Creation of directory failed : " << directory << endl
	   << "====>>>> ToolsC:checkCreateDirectory() : Please check your write permissions!" << endl ;
    }
    else {
      cout << "====>>>> ToolsC:checkCreateDirectory() : Creation of directory successful!" << endl ;
      // check again if it exists now
      cout << "====>>>> ToolsC:checkCreateDirectory() : " << endl ;
      exists = gSystem->Exec ( "ls -d " + directory ) ;
      if ( exists != 0 ) cout << "ToolsC:checkCreateDirectory() : However, it still doesn't exist!?" << endl ;
    }
  }
  cout << endl ;
  return exists ;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int findBinClosestYValue ( TH1F * histo , float yVal , float yLow , float yHigh ) {
  //
  // Find the bin in a 1-dim. histogram which has its y-value closest to
  // the given value yVal where the value yVal has to be in the range yLow < yVal < yHigh.
  // If it is outside this range the corresponding bin number is returned as negative value.
  // Currently, there is no protection if there are many bins with the same value!
  // Teh user has to take care to interpret the output correctly.
  //

  // init
  int   nBins = histo->GetNbinsX() - 2 ; // -2 because we don't include under/overflow alos in this loop
  int   iBinClosestInit = 0    ;
  // init start value properly: must avoid that the real one is not filled
  float yClosestInit ;
  //
  float maxInHisto = histo->GetMaximum() ;
  float minInHisto = histo->GetMinimum() ;
  //
  // if yVal is smaller than max -> take any value well above the maximum
  if ( yVal <= maxInHisto ) {
    yClosestInit = maxInHisto + 1 ; }
  else {
    // if yVal is greater than max value -> take a value < minimum
    yClosestInit = minInHisto - 1.0 ;
  }

  int   iBinClosest = iBinClosestInit ;
  float yClosest    = yClosestInit ;

  // loop over bins of histogram
  for ( int iBin = 1 ; iBin <= nBins ; iBin++ ) {
    float yBin = histo->GetBinContent(iBin) ;
    if ( fabs(yBin-yVal) < fabs(yClosest-yVal) ) {
      yClosest = yBin  ;
      iBinClosest = iBin ;
    }
  }

  // check if in interval
  if ( yClosest < yLow  || yClosest > yHigh ) {
    iBinClosest *= -1 ;
  }

  // check that not the initialization bin (would mean that init value was the closest)
  if ( iBinClosest == iBinClosestInit ) {
    cout << "====>>>> ToolsC=>findBinClosestYValue() : WARNING: returned bin is the initialization bin!!" << endl ;
  }

  return iBinClosest ;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////








