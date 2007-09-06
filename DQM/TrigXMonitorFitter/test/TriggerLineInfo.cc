/*
  Module name: TriggerLineInfo.cc
  Author: Charles Plager (CDF)
  Modified by: Nagesh Kulkarni
  Date Modified: 2007/09/25
  Modified by: Richard Ruiz (CMS)
  Date Modified: (On going)
  Modifications: 
   Made fitter capable of handling n trigger levels
   and n fit paramters  
*/


// -*- C++ -*-
#include <iostream>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <assert.h>
#include <math.h>
#include <map>
#include <algorithm>
#include <cstdlib>              
#include "TriggerLineInfo.h"
#include "ResidualStat.h"
#include "TFile.h"
#include "TTree.h"
#include "dout.h"

using namespace std;             



TriggerLineInfo::IVector TriggerLineInfo::sTLI_RunNumbers;          
vector <TriggerLineInfo> TriggerLineInfo::sTLI_Vector;
vector <TriggerLineInfo> TriggerLineInfo::sTLI_MergedVector;
vector <TriggerLineInfo> TriggerLineInfo::sTLI_OrigVector;
TriggerLineInfo::modeType TriggerLineInfo::sTLI_mode = TriggerLineInfo::TLI_kRootFile;

Double_t TriggerLineInfo::sTLI_minX;
Double_t TriggerLineInfo::sTLI_maxX;
Double_t TriggerLineInfo::sTLI_p0;
Double_t TriggerLineInfo::sTLI_p1;
Double_t TriggerLineInfo::sTLI_p2;
Double_t TriggerLineInfo::sTLI_p3;
// Double_t TriggerLineInfo::sTLI_pn;
Double_t TriggerLineInfo::sTLI_e0;
Double_t TriggerLineInfo::sTLI_e1;
Double_t TriggerLineInfo::sTLI_e2;
Double_t TriggerLineInfo::sTLI_e3;
// Double_t TriggerLineInfo::sTLI_en;
Double_t TriggerLineInfo::sTLI_fitError;

Double_t TriggerLineInfo::sTLI_origMaxy1;        
Double_t TriggerLineInfo::sTLI_origMiny1;
Double_t TriggerLineInfo::sTLI_orig_p0;
Double_t TriggerLineInfo::sTLI_orig_p1;
Double_t TriggerLineInfo::sTLI_orig_p2;
Double_t TriggerLineInfo::sTLI_orig_p3;
// Double_t TriggerLineInfo::sTLI_orig_pn;
Double_t TriggerLineInfo::sTLI_orig_e0;
Double_t TriggerLineInfo::sTLI_orig_e1;
Double_t TriggerLineInfo::sTLI_orig_e2;
Double_t TriggerLineInfo::sTLI_orig_e3;
// Double_t TriggerLineInfo::sTLI_orig_en;
Double_t TriggerLineInfo::sTLI_orig_fitError;

Double_t TriggerLineInfo::sTLI_off_p0;
Double_t TriggerLineInfo::sTLI_off_p1;
Double_t TriggerLineInfo::sTLI_off_p2;
Double_t TriggerLineInfo::sTLI_off_p3;
// Double_t TriggerLineInfo::sTLI_off_pn;
Double_t TriggerLineInfo::sTLI_off_fitError;

Int_t TriggerLineInfo::sTLI_minRun = 0;
Int_t TriggerLineInfo::sTLI_maxRun = 9999999;
Int_t TriggerLineInfo::sTLI_NumberRunRanges;
TriggerLineInfo::IVector TriggerLineInfo::sTLI_RunRanges;
TriggerLineInfo::SVector TriggerLineInfo::sTLI_Excludenames;
TriggerLineInfo::IVector TriggerLineInfo::sTLI_lowerExclude;
TriggerLineInfo::IVector TriggerLineInfo::sTLI_upperExclude;         
Int_t TriggerLineInfo::sTLI_minRunfit;
Int_t TriggerLineInfo::sTLI_maxRunfit;

Double_t TriggerLineInfo::sTLI_xHi;
Double_t TriggerLineInfo::sTLI_xLo;
Double_t TriggerLineInfo::sTLI_yHi;
Double_t TriggerLineInfo::sTLI_yLo;
Double_t TriggerLineInfo::sTLI_yErrorSum;
Double_t TriggerLineInfo::sTLI_yErrorAvg;

Int_t TriggerLineInfo::sTLI_NumberHi;
Int_t TriggerLineInfo::sTLI_NumberLo;
Int_t TriggerLineInfo::sTLI_NumberOfGoodPoints;
bool TriggerLineInfo::sTLI_lineIsFit = true;
bool TriggerLineInfo::sTLI_fitOriginalPoints  = true;
bool TriggerLineInfo::sTLI_plotOriginalPoints = true;

TriggerLineInfo::IVector TriggerLineInfo::sTLI_singleBadList;     
TriggerLineInfo::IVector TriggerLineInfo::sTLI_upperDoubleBadList;
TriggerLineInfo::IVector TriggerLineInfo::sTLI_lowerDoubleBadList;
unsigned int TriggerLineInfo::sTLI_dontFit;

map <Int_t, ResidualStat> TriggerLineInfo::sTLI_ResidualStatMap;
map <string, TriggerLineInfo::IVector> TriggerLineInfo::sTLI_L1TriggerMap;
map <string, TriggerLineInfo::IVector> TriggerLineInfo::sTLI_L2TriggerMap;
map <string, TriggerLineInfo::IVector> TriggerLineInfo::sTLI_L3TriggerMap;    
map <string, TriggerLineInfo::IVector> TriggerLineInfo::sTLI_LnTriggerMap;  /////////Nth Trigger

vector <TGraphErrors*> TriggerLineInfo::sTLI_pointList;
vector <TGraphErrors*> TriggerLineInfo::sTLI_legendList;
vector <TF1*> TriggerLineInfo::sTLI_functionList;
map <Int_t, string> TriggerLineInfo::sTLI_physicsTables;

Int_t TriggerLineInfo::sTLI_Index;
TLegend* TriggerLineInfo::sTLI_leg;
TriggerLineInfo::SVector TriggerLineInfo::sTLI_RunListVector;

bool TriggerLineInfo::sTLI_useOtherFilename = false;
bool TriggerLineInfo::sTLI_useOfficialFit = false;
bool TriggerLineInfo::sTLI_rejectZeroOnly = false;
string TriggerLineInfo::sTLI_otherFilename;
bool TriggerLineInfo::sTLI_useGrid = true;


#ifndef __CINT_T__  // Because CINT_T is a little goofy        
// Numbers for colors found in Root Users Guide 4.08 page 151
const Int_t TriggerLineInfo::sTLI_colorArray[TriggerLineInfo::TLI_kNumberColors] = 
  {   2,   3,   4,   5,   6,   7,  28,
      102, 103, 104, 105, 106, 107, 128};
// Numbers for styles found in Root Users Guide 4.08 page 125
const Int_t TriggerLineInfo::sTLI_styleArray[TriggerLineInfo::TLI_kNumberStyles] = 
  { 20, 21, 22, 23, 29 };
string TriggerLineInfo::sTLI_Triggername = "Triggername";
string TriggerLineInfo::sTLI_basename = "Triggername";
string TriggerLineInfo::sTLI_noVersionname = "Triggername";
string TriggerLineInfo::sTLI_VersionTag = "0";
string TriggerLineInfo::sTLI_RunNumberFilename = "RunNumbers.txt";

// Filenames with Run Numbers to avoid
const string TriggerLineInfo::TLI_kBadSingleFile = "BadSingle.txt";
const string TriggerLineInfo::TLI_kBadDoubleFile = "BadDouble.txt";
const string TriggerLineInfo::TLI_kRunRangeFile  = "RunRanges.txt";
const string TriggerLineInfo::TLI_knameExclude   = "NameExclude.txt";

#endif


//
// Constructor
TriggerLineInfo::TriggerLineInfo()
{
  reset();
}

//
// TriggerLineInfo()
TriggerLineInfo::TriggerLineInfo(Int_t RunNumber,
				 Double_t x,
				 Double_t y,
				 Double_t ex,
				 Double_t ey,
				 const std::string& table,
				 UsedFitType used,
				 Int_t time)
{
  setX (x); 
  setY (y);
  setEx (ex); 
  setEy (ey);
  setRunNumber (RunNumber);
  setTable (table);
  setUsedInFit (used);
  setGroup(-1);
  setTime(time);
}


//
// ey (bool)
Double_t TriggerLineInfo::ey (bool useAve) const
{ 
  if (useAve)
    {
      return sTLI_yErrorAvg;
    } 
  else
    {
      return TLI_ey; 
    }
}


// 
// reset 
void TriggerLineInfo::reset()
{
  setX (0.);
  setY (0.);
  setEx (0.);
  setEy (0.);
  setRunNumber (0);
  setTable ("table");
  setUsedInFit (TLI_kUsed);
  setGroup (-1);
  setTime (TLI_kNoTime);
}


//
// residual 
Double_t TriggerLineInfo::residual() const
{
  if ( (0 == sTLI_fitError) || (0 == fit()) )
    {
      return 0.; // don't bother dividing by 0
    }
  return ( ( y() - fit() ) / (sTLI_fitError * fit()) );
}


//
// residual error
Double_t TriggerLineInfo::residualError() const
{
  if ( (0 == sTLI_fitError) || (0 == fit()) )
    {
      return 0.; // don't bother dividing by 0      
    }
  return ey() /  (sTLI_fitError * fit());
}


//
// fit
Double_t TriggerLineInfo::fit() const
{
  return fit( x() );
}


//
// is used in fit (bool)
bool TriggerLineInfo::isUsedInFit() const
{
  return ( TLI_usedInFit == TLI_kUsed );
}


//
// used in fit (string)
string TriggerLineInfo::usedInFitString() const
{
  switch (TLI_usedInFit)
    {
    case TLI_kUsed:      return "Used";
    case TLI_kFive:      return "**Five**";
    case TLI_kBadRun:    return "**Bad Run**";
    case TLI_kExcluded:  return "**Excluded**";

    default:             return "**Unknown**";
    }
}




/////////////////////////////
// Static Member Functions //
/////////////////////////////


//
// set Triggername
void TriggerLineInfo::setTriggername(std::string name)
{ 
  
  sTLI_useOtherFilename = false;
  sTLI_Triggername = name; 
  // Let's get the tag too

  cout << "Triggername = " << sTLI_Triggername << endl;
  unsigned int colPos = sTLI_Triggername.rfind(":");   

  if (string::npos == colPos)
    {
      // We don't seem to have any ':'s, do we have any _v12 s
      colPos = sTLI_Triggername.rfind("_v");

      if (string::npos == colPos)
	{
	  // This should never happen
	  setVersionTag ("0");
	  setNoVersionname(sTLI_Triggername);
	} 
      else
	{
	  // This probably means L1 or L2
	  setVersionTag( sTLI_Triggername.substr(colPos + 2) );
	  setNoVersionname ( sTLI_Triggername.substr(0, colPos) );
	}
    }
 
  else
    {
      setVersionTag( sTLI_Triggername.substr(colPos + 1) );
      setNoVersionname ( sTLI_Triggername.substr(0, colPos) );
    }

  sTLI_basename = sTLI_noVersionname;
  sTLI_basename += "_v" + sTLI_VersionTag;  // remove '&' from names (bad in unix)


  while (string::npos != sTLI_basename.find("&"))
    {
      sTLI_basename.replace(sTLI_basename.find("&"), 1, "and");
    }
}


//
// get MergedPoint (withouth time)
bool TriggerLineInfo::getMergedPoint(Int_t index, 
				     Int_t &RunNumber,
				     Double_t &x,
				     Double_t &y,
				     Double_t &ex, 
				     Double_t &ey,
				     Int_t &group,
				     Double_t &residual,
				     Double_t &residualError)
{
  Int_t NumberOfEntries = sTLI_MergedVector.size();
  if ((index < 0) || (index >= NumberOfEntries))
    {
      x             = 0.;
      ex            = 0.;
      y             = 0.;
      ey            = 0.;
      group         = 0;
      RunNumber     = 0;
      residual      = 0.;
      residualError = 0.;

      cout << "TriggerLineInfo::getMergedPoint: index out of range" << endl;
      return false;
    }


  x             = sTLI_MergedVector[index].x();
  ex            = sTLI_MergedVector[index].ex();
  y             = sTLI_MergedVector[index].y();
  ey            = sTLI_MergedVector[index].ey();
  group         = sTLI_MergedVector[index].group();
  RunNumber     = sTLI_MergedVector[index].RunNumber();
  residual      = sTLI_MergedVector[index].residual();
  residualError = sTLI_MergedVector[index].residualError();

  return sTLI_MergedVector[index].isUsedInFit();
} // getPoint without time (for root file)


//
// Get Origin Point
bool TriggerLineInfo::getOrigPoint(Int_t index, 
				     Int_t &RunNumber,
				     Double_t &x, 
				     Double_t &y,
				     Double_t &ex,
				     Double_t &ey, 
				     Int_t &group,
				     Double_t &residual,
				     Double_t &residualError)
{
  Int_t NumberOfEntries = sTLI_OrigVector.size();
  if ((index < 0) || (index >= NumberOfEntries))
    {
      x             = 0.;
      ex            = 0.;
      y             = 0.;
      ey            = 0.;
      group         = 0;
      RunNumber     = 0;
      residual      = 0.;
      residualError = 0.;
      cout << "TriggerLineInfo::getOrigPoint: index out of range" << endl;

      return false;
    }
  
  x             = sTLI_OrigVector[index].x();
  ex            = sTLI_OrigVector[index].ex();
  y             = sTLI_OrigVector[index].y();
  ey            = sTLI_OrigVector[index].ey();
  group         = sTLI_OrigVector[index].group();
  RunNumber     = sTLI_OrigVector[index].RunNumber();
  residual      = sTLI_OrigVector[index].residual();
  residualError = sTLI_OrigVector[index].residualError();
  
  return sTLI_OrigVector[index].isUsedInFit();
} // getPoint without time (for root file)


//
// Get Point
bool TriggerLineInfo::getPoint(Int_t index, 
			       Int_t &RunNumber,
			       Double_t &x, 
			       Double_t &y,
			       Double_t &ex, 
			       Double_t &ey,
			       Int_t &group,
			       Double_t &residual,
			       Double_t &residualError)
{
  Int_t NumberOfEntries = sTLI_Vector.size();
  if ((index < 0) || (index >= NumberOfEntries))
    {
      x             = 0.;
      ex            = 0.;
      y             = 0.;
      ey            = 0.;
      group         = 0;
      RunNumber     = 0;
      residual      = 0.;
      residualError = 0.;
      cout << "TriggerLineInfo::getPoint: index out of range" << endl;

      return false;
    }
  
  x             = sTLI_Vector[index].x();
  ex            = sTLI_Vector[index].ex();
  y             = sTLI_Vector[index].y();
  ey            = sTLI_Vector[index].ey();
  group         = sTLI_Vector[index].group();
  RunNumber     = sTLI_Vector[index].RunNumber();
  residual      = sTLI_Vector[index].residual();
  residualError = sTLI_Vector[index].residualError();

  return sTLI_Vector[index].isUsedInFit();
} // getPoint without time (for root file)


//
// Get Point with (time)
bool TriggerLineInfo::getPoint (Int_t index,
				Int_t &RunNumber, 
				Double_t &x, 
				Double_t &y,
				Double_t &ex,
				Double_t &ey,
				Int_t &group, 
				Double_t &residual,
				Double_t &residualError,
				Int_t &time)
{
  if ( !(getPoint (index,
		   RunNumber,
		   x, 
		   y,
		   ex,
		   ey,
		   group,
		   residual,
		   residualError)) )
    {
      time = 0;
      return false;
    }
  
  time = sTLI_Vector[index].time();
  return sTLI_Vector[index].isUsedInFit();
} //getPoint with time added


//
// Calculate Fit Info
void TriggerLineInfo::CalculateFitInfo (Double_t &Error, 
					IList &badRunList)
{
  DList absPercentDeviation;
  badRunList.clear();
  
  Int_t NumberOfEntries = sTLI_Vector.size();
  for (Int_t loop = 0; loop < NumberOfEntries; ++loop)
    {
      Double_t pred   = sTLI_Vector[loop].fit();
      Double_t actual = sTLI_Vector[loop].y();
      Double_t percent;
      
      if (pred > 0)
	{
	  percent = fabs ((pred - actual) / pred);
	}
      else
	{
	  percent = 10; // something really big
	}
      // Only keep track of the good points

      if ( sTLI_Vector[loop].isUsedInFit() )
	{
	  absPercentDeviation.push_back(percent);
	}
    } // for loop


  // we ought to have >= 4 points at this point, but let's check just in case
  Int_t size = absPercentDeviation.size();
  if (size < 4)  // this is bad
    {      
      cout << "Too few in absPercentDeviation: " << size << endl;
      Error = 1.;
      return;
    }

  absPercentDeviation.sort();
  Int_t index_95 = (Int_t) (size * 0.95);
  Double_t value_95 = 0, value_100 = 0;
  Int_t count = 1;

  for (list <Double_t>::const_iterator iter = absPercentDeviation.begin();
       iter != absPercentDeviation.end(); ++iter)
    {     
      if (count == index_95)
	{
	  value_95  = *iter;
	}
      if (count == size)
	{
	  value_100  = *iter;
	}
      ++count;
    }

  if (!value_100)
    {
      cout << "got nothing" << endl;
    }
  // Error is value_95 added in quadrature with 0.10 (0.01 = 0.10 ^ 2)
  Error = sqrt( value_95 * value_95) + 0.1; 

  if (Error < 0.2)
    {
      Error = 0.2;
    }
  if (Error > 1)
    {
      cout << "fixing Error: " << Error <<  endl;
      Error = 1;
    }

  cout << "Error " << Error << endl;
  setFitError(Error);   
  for (Int_t loop = 0; loop < NumberOfEntries; ++loop)
    {
      if ( !sTLI_Vector[loop].isUsedInFit() )
	{
	  continue;
	}
      Double_t pred   = sTLI_Vector[loop].fit();
      Double_t actual = sTLI_Vector[loop].y();
      Double_t percent;

      if (pred > 0)
	{
	  percent = fabs ((pred - actual) / pred);
	}
      else
	{
	  cout << "prediction is less than 0: " << pred << endl;
	  percent = 10; // something really big
	}
      if (percent > Error)
	{
	  // this is a bad Run
	  badRunList.push_back( sTLI_Vector[loop].RunNumber() );
	}
    } // for loop 
  // all done

  return;
}


// Line
// fit ()
Double_t TriggerLineInfo::fit(Double_t x)
{
  if (0 == x)
    {
      // don't bother dividing by 0
      return 0.;
    }
  ////// This is the official fit
  return sTLI_p0 + (sTLI_p1 / x) + sTLI_p2 * x + sTLI_p3 * x * x;  
}


//
// reset static entries
void TriggerLineInfo::resetStaticEntries()
{
  sTLI_MergedVector.clear();
  sTLI_Vector.clear();
  sTLI_OrigVector.clear();
  sTLI_lineIsFit = true;
  sTLI_dontFit = 0;
  sTLI_p0 = sTLI_orig_p0 = 0;
  sTLI_p1 = sTLI_orig_p1 = 0;
  sTLI_p2 = sTLI_orig_p2 = 0;
  sTLI_p3 = sTLI_orig_p3 = 0;
  //  sTLI_pn = sTLI_orig_pn = 0;
  sTLI_e0 = sTLI_orig_e0 = 0;
  sTLI_e1 = sTLI_orig_e1 = 0;
  sTLI_e2 = sTLI_orig_e2 = 0;
  sTLI_e3 = sTLI_orig_e3 = 0;
  //  sTLI_en = sTLI_orig_en = 0;
  sTLI_fitError = sTLI_orig_fitError = 1;
  sTLI_minX = 1000; // something ridiculously big
  sTLI_maxX = 0;
  sTLI_minRunfit = 100000;
  sTLI_maxRunfit = 10000000;
  sTLI_xHi = 0;
  sTLI_xLo = 0;
  sTLI_yHi = 0;
  sTLI_yLo = 0;
  sTLI_yErrorSum = sTLI_yErrorAvg = 0.;
  sTLI_dontFit = 0;
  sTLI_yErrorAvg = 0;
  sTLI_NumberOfGoodPoints = 0;
  sTLI_NumberHi = sTLI_NumberLo = 0;
  sTLI_Index = 0;
}


//
//
void TriggerLineInfo::addMergedEntry(Int_t RunNumber,
				     Double_t x,
				     Double_t y, 
				     string table, 
				     Double_t ey,
				     Double_t ex, 
				     Int_t time)
{
  if ( (RunNumber < sTLI_minRun) || (RunNumber > sTLI_maxRun) )
    {
      return;
    }

  if (ey < 0)
    {
      ey = y * 0.05;
    }
  //dout << "time " << time << endl;

  if (TLI_kNoTime == time)
    {
      sTLI_MergedVector.push_back ( TriggerLineInfo (RunNumber, 
						     x,
						     y,
						     ex,
						     ey,
						     table) );
      sTLI_yErrorSum += ey;
      sTLI_yErrorAvg = ( sTLI_yErrorSum / sTLI_MergedVector.size() );
    } //time isn't entered
  else 
    {
      sTLI_MergedVector.push_back ( TriggerLineInfo (RunNumber,
						     x, 
						     y, 
						     ex,
						     ey,
						     table,
						     TLI_kUsed,
						     time) );
      sTLI_yErrorSum += ey;
      sTLI_yErrorAvg = ( sTLI_yErrorSum / sTLI_MergedVector.size() );
    } //if time < 0 (if time is a variable)
} // addMergedEntry


//
// add entry
void TriggerLineInfo::addEntry(Int_t RunNumber,
			       Double_t x,
			       Double_t y,
			       string table,
			       Double_t ey,
			       Double_t ex, 
			       Int_t time)
{
  if ( (RunNumber < sTLI_minRun) || (RunNumber > sTLI_maxRun) )
    {
      // don't add points that are outside the range
      //cout << "Run Number" << RunNumber << " not in range" << endl;
      return;
    }

  if (ey < 0) 
    {
      ey = y * 0.05;
    }
  //dout << "time " << time << endl;

  if (TLI_kNoTime == time)
    {
      sTLI_Vector.push_back ( TriggerLineInfo (RunNumber,
					       x,
					       y,
					       ex,
					       ey,
					       table) );
      sTLI_yErrorSum += ey;
      sTLI_yErrorAvg = (sTLI_yErrorSum / sTLI_Vector.size() );
    } //time isn't entered
  else
    {
      sTLI_Vector.push_back ( TriggerLineInfo (RunNumber,
					       x, 
					       y,
					       ex,
					       ey,
					       table,
					       TLI_kUsed,
					       time) );
      sTLI_yErrorSum += ey;
      sTLI_yErrorAvg = sTLI_yErrorSum / sTLI_Vector.size();
    } //if time < 0 (if time is a variable)
} // addEntry


//
// Merged points
void TriggerLineInfo::MergePoints(Int_t xMin, Int_t xMax)
{
  Double_t xSum = 0;
  Double_t ySum = 0;
  Double_t x_low_Avg;
  Double_t y_low_Avg;
  Int_t NumberPoints = 0;
  Int_t NumberOfEntries = sTLI_Vector.size();
  
  for (Int_t loop = 0; loop < NumberOfEntries;loop++ )
    {
      
      if( (sTLI_Vector[loop].x() >= xMin) && (sTLI_Vector[loop].x() <= xMax) )                      
	{
          if(sTLI_Vector[loop].isUsedInFit())
	    {
	      xSum+= sTLI_Vector[loop].x();
	      ySum+= sTLI_Vector[loop].y();
	      NumberPoints ++;
	    }
        }
      
    }//loop

      
  if(NumberPoints > 0)
    {
      x_low_Avg = xSum/ NumberPoints;
      y_low_Avg = ySum/ NumberPoints;

      /* cout << "Range: " << xMin << " - " << xMax << endl;
	 cout << "NumberPoints = " << NumberPoints << endl;
	 cout << "x_low_Ave = " << x_low_Ave << endl; 
      */

      float Error =  y_low_Avg * 0.01;
      
      addMergedEntry (sTLI_Vector[xMin].RunNumber(),
		      x_low_Avg,
		      y_low_Avg,
		      "",
		      Error,
		      0,
		      sTLI_Vector[xMin].time());
    } 
}


//
// Calculate
void TriggerLineInfo::Calculate()
{
  // Don't bother if we have no entries
  
  Int_t NumberOfEntries = sTLI_Vector.size();
  if (NumberOfEntries == 0)
    {
      return;
    }
  
  IVector lowerList = sTLI_lowerDoubleBadList;
  IVector upperList = sTLI_upperDoubleBadList;
  Int_t NumberDoubleBadRun = sTLI_upperDoubleBadList.size();
  // find which Trigger exclusions should be used
  Int_t ExcludedSize = sTLI_Excludenames.size();
  
  for (Int_t index = 0; index < ExcludedSize; ++index)
    {
      if ( string::npos != sTLI_Triggername.find(sTLI_Excludenames[index]) )
	{
	  // we got a hit
	  lowerList.push_back( sTLI_lowerExclude[index] );
	  upperList.push_back( sTLI_upperExclude[index] );
	} // found
    } // for index
  
  // get the average Y value
  float ySum = 0, yAvg;
  float xSum = 0, xAvg;
  Int_t NumberUsed = 0;

  for (Int_t loop = 0; loop < NumberOfEntries; ++loop)
    {
      if (0 == sTLI_Vector[loop].y())
	{
	  // don't bother using this point
	  continue;
	}
      
      /* Comment by Nagesh:
       1. Exclude the points if the cross section is zero.
       2. Exclude the point if it is way below the 4th point and the difference between 
          the first point and fourth point is more than 20%. Do this only if Run Number changes
       3. Set their status as "Excluded" so that these points appear as black on the plot
      */ 
      
      Double_t y1,y2,y3,y4,thresh;
      thresh = 30.00;
      Double_t thisRunNumber = sTLI_Vector[loop].RunNumber();
 
      if( (loop >0) )
        if(sTLI_Vector[loop-1].y()==0.00)
	  {
            sTLI_Vector[loop].setUsedInFit(TLI_kExcluded);
            continue;
	  }           
      y1 = sTLI_Vector[loop].y();

      if( (loop < NumberOfEntries) && 
	  (sTLI_Vector[loop + 1].RunNumber() == thisRunNumber) )
	{
	  y2 = sTLI_Vector[loop + 1].y();
	}
      else
	{
	  y2 = sTLI_Vector[loop - 1].y();
	}
      
      if( (loop < NumberOfEntries-1) && 
	  (sTLI_Vector[loop + 2].RunNumber() == thisRunNumber) )
	{
	  y3 = sTLI_Vector[loop + 2].y();
	}
      else
	{
	  y3 = sTLI_Vector[loop - 2].y();
	}
      
      if( (loop < NumberOfEntries-2) && 
	  (sTLI_Vector[loop + 3].RunNumber() == thisRunNumber) )
	{
	  y4 = sTLI_Vector[loop + 3].y();
	}
      else
	{
	  y4 = sTLI_Vector[loop - 3].y();
	}


           
      Double_t percent1 = fabs( ((y2 - y1) * 100) / y1);
      Double_t percent2 = fabs( ((y3 - y1) * 100) / y1);
      Double_t percent3 = fabs( ((y4 - y1) * 100) / y1);
      
      cout << "loop = " << loop << endl;
      cout << "Run = "  << sTLI_Vector[loop].RunNumber() << endl;
      cout << "Percent1 = " << percent1 << endl;
      cout << "Percent2 = " << percent2 << endl;
      cout << "Percent3 = " << percent3 << endl;
      

      if( (percent1 > thresh) & (percent2 > thresh) & (percent3 > thresh) )
	{ 
	  sTLI_Vector[loop].setUsedInFit(TLI_kExcluded);
	  cout << loop << " Excluded!" << endl;
	  continue;
	}
   
      
      ++NumberUsed;
      
      ySum += sTLI_Vector[loop].y();
      xSum += sTLI_Vector[loop].x();
      // get Run range here
      if ( sTLI_Vector[loop].RunNumber() > sTLI_maxRun )
	{
	  sTLI_maxRun = sTLI_Vector[loop].RunNumber();
	}
      if ( sTLI_Vector[loop].RunNumber() < sTLI_minRun )
	{
	  sTLI_minRun = sTLI_Vector[loop].RunNumber();
	}
    } // for loop

  if (0 == NumberUsed)
    {
      NumberUsed = 1;
    }

  yAvg = ( ySum / NumberUsed );
  xAvg = ( xSum / NumberUsed );
  float maxY = ( yAvg * 5.0 );  // * 5
  float minY = ( yAvg * 0.2 ); // / 5

  // check to see if we have a gap Trigger
  if( string::npos != sTLI_Triggername.find("GAP") )
    {
      minY /= 5; 
    }

  float yErrorSum  = 0;
  Int_t NumberOfGood = 0;
  float xSumLo = 0;
  float xSumHi = 0;
  float ySumLo = 0;
  float ySumHi = 0;
  Int_t NumberHi = 0;
  Int_t NumberLo = 0;

  for (Int_t loop = 0; loop < NumberOfEntries; ++loop)
    {
      float y = sTLI_Vector[loop].y();
      Int_t RunNumber = sTLI_Vector[loop].RunNumber();
      // is this a bad Run Number
      Int_t Numberbad = sTLI_singleBadList.size();
      bool badPoint = false;
      // check single Runs
      for (Int_t inner = 0; inner < Numberbad; ++inner)
	{
	  if ( RunNumber == sTLI_singleBadList[inner] )
	    {
	      // don't fit this puppy
	      sTLI_Vector[loop].setUsedInFit(TLI_kBadRun);
	      //sTLI_Vector[loop].setGroup(-1);
	      badPoint = true;
	      break;
	    }
	}

      // check specific Run ranges
      Int_t size = lowerList.size();
      for (Int_t index = 0; index < size; ++index)
	{
	  if ( (lowerList[index] <= RunNumber) &&  (upperList[index] >= RunNumber) )
	    {
	      // Exclude this point too
	      if (index < NumberDoubleBadRun)
		{
		  // This is from one of the bad Run ranges
		  sTLI_Vector[loop].setUsedInFit(TLI_kBadRun);
		}
	      else
		{
		  // this must be for this Trigger line specifically
		  sTLI_Vector[loop].setUsedInFit(TLI_kExcluded);
		}
	      //sTLI_Vector[loop].setGroup(-1);
	      badPoint = true;
	      break;
	    }
	} // for index;
      if (! badPoint && ! sTLI_useOfficialFit)
	{
	  if (sTLI_rejectZeroOnly)
	    {
	      if (0. == y)
		{
		  sTLI_Vector[loop].setUsedInFit(TLI_kFive);
		  badPoint = true;
		}
	    }
	  else
	    {
	      if ( ( y > maxY ) || ( y < minY ))
		{
		  // don't fit this puppy
		  sTLI_Vector[loop].setUsedInFit(TLI_kFive);
		  badPoint = true;
		  //sTLI_Vector[loop].setGroup(-1);
		} 
	    }
	} // if not badPoint
      
      if (TLI_kRootFile == sTLI_mode)
	{
	  for (Int_t groupIndex = 0; groupIndex < sTLI_NumberRunRanges; ++groupIndex)
	    {
	      if ((RunNumber >= sTLI_RunRanges[groupIndex]) &&
		  (RunNumber < sTLI_RunRanges[groupIndex + 1]))
		{
		  sTLI_Vector[loop].setGroup(groupIndex);
		  break;
		}
	    } // for groupIndex;
	}
      else
	if ((TLI_kTXMonRoot == sTLI_mode) || (TLI_kDataBase == sTLI_mode))
	  {
	    sTLI_Vector[loop].setGroup( findGroup(RunNumber) );
	  } // if in root file mode or database mode
      // If any of the above failed, don't bother continuing
      if (badPoint)
	{
	  continue;
	} // if badPoint don't go on
      // Looks like we are going to use this point
      // sum Real value of Error

      yErrorSum +=  sTLI_Vector[loop].ey(false);
      ++NumberOfGood;
      // find min and max X here
      if (sTLI_Vector[loop].x() > sTLI_maxX)
	{
	  sTLI_maxX = sTLI_Vector[loop].x();
	}
      if (sTLI_Vector[loop].x() < sTLI_minX)
	{
	  sTLI_minX = sTLI_Vector[loop].x();
	}

      if (sTLI_Vector[loop].x() > xAvg)
	{
	  xSumHi += sTLI_Vector[loop].x();
	  ySumHi += sTLI_Vector[loop].y();
	  ++NumberHi;
	}
      else
	{
	  xSumLo += sTLI_Vector[loop].x();
	  ySumLo += sTLI_Vector[loop].y();
	  ++NumberLo;
	}
    } // for loop
  
  sTLI_yErrorAvg = ( yErrorSum / NumberOfGood );
  sTLI_NumberOfGoodPoints = NumberOfGood;
  // o.k.  We've split the data up in to two ranges, Hi and Lo.  We are going
  // to use these points to seed our fits.  But before we start, let's make
  // sure that we have at least two point per group

  if ( (NumberHi < 2) || (NumberLo < 2) )
    {
      setXHi (0);
      setXLo (0);
      setYHi (0);
      setYLo (0);
      setNumberHi (0);
      setNumberLo (0);
    }
  else
    {
      setXHi (xSumHi / NumberHi);
      setXLo (xSumLo / NumberLo);
      setYHi (ySumHi / NumberHi);
      setYLo (ySumLo / NumberLo);
      setNumberHi (NumberHi);
      setNumberLo (NumberLo);
    } // not enough high or low points
}


//
// Get Arrays
Int_t TriggerLineInfo::getArrays (Double_t *xArray,
				  Double_t *exArray,
				  Double_t *yArray,
				  Double_t *eyArray,
				  bool returnAllPoints)
{
  Int_t index = 0;
  Int_t NumberOfEntries = sTLI_Vector.size();
  for (Int_t loop = 0; loop < NumberOfEntries; ++loop)
    {
      if (returnAllPoints || sTLI_Vector[loop].isUsedInFit())
	{
	  xArray[index] = sTLI_Vector[loop].x();
	  yArray[index] = sTLI_Vector[loop].y();
	  //  exArray[index] = sTLI_Vector[loop].ex();
	  //  eyArray[index] = sTLI_Vector[loop].ey();
	  exArray[index] = 0.;
	  eyArray[index] = 0.;
	  //cout << "x " << xArray[index] << " +- " << exArray[index]
	  //     << " y " << yArray[index] << " +- " << eyArray[index]
	  //     << endl;
	  ++index;
	}
    } // for loop
  return index;
}


//
// get run arrays
Int_t TriggerLineInfo::getRunArrays (Double_t *xArray,
				     Double_t *exArray,
				     Double_t *yArray,
				     Double_t *eyArray )
{
  Int_t index = 0;
  Int_t NumberOfEntries = sTLI_Vector.size();
  for (Int_t loop = 0; loop < NumberOfEntries; ++loop)
    {
      if (TLI_kRootFile == sTLI_mode)
	{
	  xArray[index] = sTLI_Vector[loop].RunNumber();
	}
      else
	if (TLI_kDataBase == sTLI_mode)
	  {
	    //xArray[index] = sTLI_Vector[loop].time();
	    xArray[index] = ++sTLI_Index + 2 * sTLI_Vector[loop].group();
	  }
	else
	  if (TLI_kTXMonRoot == sTLI_mode)
	    {
	      xArray[index] = sTLI_Vector[loop].time();
	    }
      // sets the x-axis for the residual graph
      exArray[index] = 0;
      yArray[index] = sTLI_Vector[loop].residual();
      eyArray[index] = 0; // don't bother giving the Error here
      ++index;
    } // for loop
  return index;
}


//
// legend names
TriggerLineInfo::SVector TriggerLineInfo::legendnames()
{
  std::vector< std::string > retval;
  if (TLI_kRootFile == sTLI_mode)
    {
      for (Int_t loop = 0; loop < sTLI_NumberRunRanges; ++loop)
	{
	  char cstring[100];
	  if ( (sTLI_minRun == sTLI_RunRanges[loop]) && 
	       (sTLI_maxRun == sTLI_RunRanges[loop + 1]) )
	    {
	      // should only happen when we have only 1 category
	      sprintf (cstring, "Used in fit");
	      retval.push_back( cstring );
	    }
	  else
	    if (sTLI_minRun == sTLI_RunRanges[loop])
	      {
		// smallest category
		sprintf (cstring, "Runs less than %d", sTLI_RunRanges[loop + 1]);
		retval.push_back( cstring );
	      } // if min Run
	    else
	      if (sTLI_maxRun == sTLI_RunRanges[loop + 1])
		{
		  sprintf (cstring, "Runs greater than %d", sTLI_RunRanges[loop]);
		  retval.push_back( cstring );
		} // if max Run
	      else
		{
		  sprintf (cstring, "Runs between %d and %d", 
			   sTLI_RunRanges[loop], sTLI_RunRanges[loop + 1]);
		  retval.push_back( cstring );
		} // normal situation
	}
    }
  
  else
    if ( (TLI_kTXMonRoot == sTLI_mode) || (TLI_kDataBase == sTLI_mode) )
      {
	for (Int_t RunIndex = 0; RunIndex < NumberRunNumbers(); ++RunIndex)
	  {
	    char cstring[100];
	    sprintf(cstring, "Run Number %d", RunNumber(RunIndex));
	    retval.push_back( cstring );
	  } //for each Run Number
      } // if from root file or database
  return retval;
}


//
// base filename
string TriggerLineInfo::baseFilename()
{
  if (sTLI_useOtherFilename)
    {
      return sTLI_otherFilename;
    }
 
  LevelType type = Level();
  string filename;


  if ( (TLI_kLeveL1TriggerSwitch == 1) && (type == TLI_kL1) )
    {
      return "L1/" + sTLI_basename;		
    }
  else
    if ( (TLI_kLeveL2TriggerSwitch == 1) && (type == TLI_kL2) )
    {
      return "L2/" + sTLI_basename;      
    }
    else
      if ( (TLI_kLeveL3TriggerSwitch == 1) && (type == TLI_kL3) )
	{
	  return "L3/" + sTLI_basename;
	}      
      else  
	if ( (TLI_kLeveLnTriggerSwitch == 1) && (type == TLI_kLn) )
	  {      
	    return "Ln/" + sTLI_basename;
	  }  
	else
	  {
	    return "NotUsed/" + sTLI_basename;
	  }


}


//
//
TriggerLineInfo::LevelType TriggerLineInfo::Level()
{
  string part = sTLI_Triggername.substr(0, 2);

  if (part == "L1") 
    {
      return TLI_kL1;
    } 
  else
    if (part == "L2") 
      {
	return TLI_kL2;
      }
    else 
      if (part == "L3")
      {
	return TLI_kL3;
      }
      else
	if (part == "Ln")
	  {
	    return TLI_kLn;
	  }

  /// Currently, all L3 triggers do not have prefixes! so we need this:

	else
	  {
	    return TLI_kL3; 
	  }

}


//
// set mode
void TriggerLineInfo::setMode(modeType mode)
{
  sTLI_mode = mode;
  // If we're not getting the info only one point per Run,
  //don't bother checking to see if they are too high or too low. Reject zero points only.
  if (TLI_kRootFile != sTLI_mode)
    {
      sTLI_rejectZeroOnly = true;
    }
  resetStaticEntries();
  initialize();
}


//
// initialize 
void TriggerLineInfo::initialize()
{
  atexit(TriggerLineInfo::memoryCleanUp);
  // get single list of bad Runs
  getSingleListFromFile (sTLI_singleBadList, TLI_kBadSingleFile); // vector ,string
  getDoubleListFromFile (sTLI_lowerDoubleBadList, sTLI_upperDoubleBadList, TLI_kBadDoubleFile);
  getThreeListsFromFile (sTLI_Excludenames, sTLI_lowerExclude, sTLI_upperExclude, TLI_knameExclude);

  if (TLI_kRootFile == mode())
    {
      // get Run ranges from file
      getSingleListFromFile (sTLI_RunRanges, TLI_kRunRangeFile);
      sTLI_NumberRunRanges = sTLI_RunRanges.size() - 1;
      if (sTLI_NumberRunRanges < 1)
	{
	  sTLI_RunRanges.clear();
	  sTLI_RunRanges.push_back(100000);
	  sTLI_RunRanges.push_back(9999999);
	  sTLI_NumberRunRanges = 1;
	}
      setMinRun(sTLI_RunRanges[0]);
      setMaxRun(sTLI_RunRanges[sTLI_NumberRunRanges]);
    }
  else
    if ( (mode() == TLI_kDataBase) || (mode() == TLI_kTXMonRoot) )
      {
	if (mode() == TLI_kDataBase)
	  {
	    // this was already done if we're in XMonRoot mode
	    getSingleListFromFile(sTLI_RunNumbers, sTLI_RunNumberFilename);
	  }
	sort( sTLI_RunNumbers.begin(), sTLI_RunNumbers.end() );
	if (0 == sTLI_RunNumbers.size())
	  {
	    cerr << "no Run Numbers.... exiting" << endl;
	    exit(1);
	  } // if no Run Numbers, exit
      
	//remove all repeat Run Numbers from Vector
	IVector  tempVector = sTLI_RunNumbers;
	sTLI_RunNumbers.clear();
	Int_t last = -1;

	for (unsigned int loop = 0; loop < tempVector.size(); ++loop)
	  {
	    if (last != tempVector[loop])
	      {
		sTLI_RunNumbers.push_back( tempVector[loop] );
	      }
	    last = tempVector[loop];
	  }
	
	setMinRun(sTLI_RunNumbers[0]);
	setMaxRun(sTLI_RunNumbers[sTLI_RunNumbers.size() - 1]);
	sTLI_NumberRunRanges = sTLI_RunNumbers.size();
	setPhysicsTables();
      
	if (mode() == TLI_kTXMonRoot)
	  {
	    // I don't need to do the stuff below for XMonRoot mode
	    makeLegend();
	    return;
	  }
	
	for (unsigned int i = 0; i < sTLI_RunNumbers.size(); ++i)
	  {
	    std::vector< std::string > Triggers;
	    Int_t RunNumber = sTLI_RunNumbers[i];
	    Int_t RunStart = RunNumber / 1000;
	    stringstream ss1, ss2, ss3, ssn;



	    // get Triggers from L1
	    ss1 << "data/" << RunStart << "/" << RunNumber << "/L1Triggers.txt";
	    getSingleListFromFile(Triggers, ss1.str());
	    for (unsigned int i = 0; i < Triggers.size(); i++)
	      {
		sTLI_L1TriggerMap[Triggers[i]].push_back(RunNumber);
	      } //add Run Numbers to map of L1 Triggers
	    Triggers.clear();

	    

	    // get Tirggers from L2
	    ss2 << "data/" << RunStart << "/" << RunNumber << "/L2Triggers.txt";
	    getSingleListFromFile(Triggers, ss2.str());
	    for (unsigned int i = 0; i < Triggers.size(); i++)
	      {
		sTLI_L2TriggerMap[Triggers[i]].push_back(RunNumber);
	      } //add Run Numbers to map of L2 Triggers 
	    Triggers.clear();
	    


	    // get Triggers from L3
	    ss3 << "data/" << RunStart << "/" << RunNumber << "/L3Triggers.txt";
	    getSingleListFromFile(Triggers, ss3.str());
	    for (unsigned int i = 0; i < Triggers.size(); i++)
	      {
		sTLI_L3TriggerMap[Triggers[i]].push_back(RunNumber);
	      } //add Run Numbers to map of L3 Triggers 
	    Triggers.clear();
	    


	    // get Triggers from Ln
	    ssn << "data/" << RunStart << "/" << RunNumber << "/LnTriggers.txt";
	    getSingleListFromFile(Triggers, ssn.str());
	    for (unsigned int i = 0; i < Triggers.size(); i++)
	      {
		sTLI_LnTriggerMap[Triggers[i]].push_back(RunNumber);
	      } //add Run Numbers to map of Ln Triggers
	    Triggers.clear();
	    
	  } //for each Run Number   
      } // if info from root or database
  
  
  makeLegend();  // look about 15 lines below for makeLegend()
} 


// Line
// set physics tables
void TriggerLineInfo::setPhysicsTables ()
{
  for (unsigned int i = 0; i < sTLI_RunNumbers.size(); ++i)
    {
      sTLI_physicsTables[sTLI_RunNumbers[i]] = getTable(sTLI_RunNumbers[i]);
    } // set a physics table for each Run Number
} 


// 
// make legend
void TriggerLineInfo::makeLegend ()
{
  vector <string> legendnamesVector;
  Int_t NumberLegendEntries;
  legendnamesVector = legendnames();
  NumberLegendEntries = legendnamesVector.size();
  Double_t zerod = 0.;
  // sTLI_leg = new TLegend(0.67,0.13,0.96,0.29); // Original
  sTLI_leg = new TLegend(0.88,0.15,0.997,0.80);  ////(left,bottom,right,top)
  sTLI_leg->SetFillStyle(4000); // Transparent.
  
  for (Int_t loop = NumberLegendEntries - 1; loop >= 0; --loop)
    {
      TGraphErrors *point = new TGraphErrors(1, &zerod, &zerod, &zerod, &zerod);
      point->SetMarkerColor(sTLI_colorArray[loop % TLI_kNumberColors]);
      point->SetMarkerStyle(sTLI_styleArray[loop % TLI_kNumberStyles]);
      sTLI_leg->AddEntry(point, legendnamesVector[loop].c_str(), "p");
      sTLI_legendList.push_back( point );
    } // for each entry in legend 

  TGraphErrors *point =  new TGraphErrors(1, &zerod, &zerod, &zerod, &zerod);
  point->SetMarkerColor( TLI_kBadPointsColor );
  point->SetMarkerStyle( TLI_kBadPointsStyle );
  sTLI_leg->AddEntry(point, "Not used in the fit", "p");
  sTLI_legendList.push_back( point );
} // makeLegend()


// Line
// Process Specific Triggers
void TriggerLineInfo::ProcessSpecificTriggers(string Triggername)
{
  map < string, IVector >::const_iterator mItr;


  for (mItr = sTLI_L1TriggerMap.begin(); mItr != sTLI_L1TriggerMap.end(); mItr++ )
    {
      if ( (mItr->first).find(Triggername) != string::npos)
	{
	  TriggerLineInfo::resetStaticEntries();
	  cout << "trying to set name to " << mItr->first << endl;
	  setTriggername(mItr->first);
	  if (checkTriggername())
	    {
	      for (unsigned int i = 0; i < mItr->second.size(); ++i)
		{
		  string filename = getFilenameForTrigger(1, mItr->second[i]);
		  loadPoints(mItr->second[i], filename); 
		  // mItr->second is the IVector  of RunNumbers
		} //for each Run Number in Trigger's list
	      ProcessTrigger();
	    } // if Triggername is OK 
	} // if Trigger's name matches Triggername
    } // for each L1 Trigger that matches Triggername


  for (mItr = sTLI_L2TriggerMap.begin(); mItr != sTLI_L2TriggerMap.end(); mItr++ )
    {
      if ( (mItr->first).find(Triggername) != string::npos)
	{
	  TriggerLineInfo::resetStaticEntries();
	  cout << "trying to set name to " << mItr->first << endl;
	  setTriggername(mItr->first);
	  if (checkTriggername())
	    {
	      for (unsigned int i = 0; i < mItr->second.size(); ++i)
		{
		  string filename = getFilenameForTrigger(2, mItr->second[i]);
		  loadPoints(mItr->second[i], filename); 
		  // mItr->second is the IVector  of RunNumbers
		} //for each Run Number in Trigger's list
	      ProcessTrigger();
	    } // if Triggername is OK 
	} // if Trigger's name matches Triggername
    } // for each L2 Trigger that matches Triggername


  for (mItr = sTLI_L3TriggerMap.begin(); mItr != sTLI_L3TriggerMap.end(); mItr++ )
    {
      if ( (mItr->first).find(Triggername) != string::npos)
	{
	  TriggerLineInfo::resetStaticEntries();
	  cout << "trying to set name to " << mItr->first << endl;
	  setTriggername(mItr->first);
	  if (checkTriggername())
	    {
	      for (unsigned int i = 0; i < mItr->second.size(); ++i)
		{
		  string filename = getFilenameForTrigger(3, mItr->second[i]);  
		  loadPoints(mItr->second[i], filename); 
		  // mItr->second is the IVector  of RunNumbers
		} //for each Run Number in Trigger's list
	      ProcessTrigger();
	    } // if Triggername is OK 
	} // if Trigger's name matches Triggername
    } // for each L3 Trigger that matches Triggername

  //  for an nth trigger!
  //for (mItr = sTLI_LnTriggerMap.begin(); mItr != sTLI_LnTiggerMap.end(); mItr++ )      
  //{
  //  if ( (mItr->first).find(Triggername) != string::npos)
  //  {
  //    TriggerLineInfo::resetStaticEntries();
  //    cout << "trying to set name to " << mItr->first << endl;
  //    setTriggername(mItr->first);
  //    if (checkTriggername())
  //     {
  //       for (unsigned int i = 0; i < mItr->second.size(); ++i)
  //	    {
  //	     string filename = getFilenameForTrigger(nth, mItr->second[i]);
  //	     loadPoints(mItr->second[i], filename); 
  //	     // mItr->second is the IVector  of RunNumbers
  //	    } //for each Run Number in Trigger's list
  //      ProcessTrigger();
  //     } // if Triggername is OK 
  //   } // if Trigger's name matches Triggername
  //}

} // ProcessOneTrigger(string Triggername)


// Line
// Process All Triggers
void TriggerLineInfo::ProcessAllTriggers()
{
  map < string, IVector  >::const_iterator mItr;


  for (mItr = sTLI_L1TriggerMap.begin(); mItr != sTLI_L1TriggerMap.end(); mItr++ )
    {
      TriggerLineInfo::resetStaticEntries();
      setTriggername(mItr->first);
      if (checkTriggername())
	{
	  for (unsigned int i = 0; i < mItr->second.size(); ++i)
	    {
	      string filename = getFilenameForTrigger(1, mItr->second[i]);
	      loadPoints(mItr->second[i], filename); 
	      // mItr->second is the std::vector< Int_t >  of RunNumbers
	    } //for each Run Number in Trigger's list
	  ProcessTrigger();
	} // if Triggername is OK 
    } // for each L1 Trigger


  for (mItr = sTLI_L2TriggerMap.begin(); mItr != sTLI_L2TriggerMap.end(); mItr++ )
    {
      TriggerLineInfo::resetStaticEntries();
      setTriggername(mItr->first);
      if (checkTriggername())
	{
	  for (unsigned int i = 0; i < mItr->second.size(); ++i)
	    {
	      string filename = getFilenameForTrigger(2, mItr->second[i]);
	      loadPoints(mItr->second[i], filename); 
	      // mItr->second is the std::vector< Int_t >  of RunNumbers
	    } //for each Run Number in Trigger's list
	  ProcessTrigger();
	} // if Triggername is OK 
    } // for each L2 Trigger

  
  for (mItr = sTLI_L3TriggerMap.begin(); mItr != sTLI_L3TriggerMap.end(); mItr++ )
    {
      TriggerLineInfo::resetStaticEntries();
      setTriggername(mItr->first);
      if (checkTriggername())
	{
	  for (unsigned int i = 0; i < mItr->second.size(); ++i)
	    {
	      string filename = getFilenameForTrigger(3, mItr->second[i]);
		loadPoints(mItr->second[i], filename); 
		// mItr->second is the std::vector< Int_t >  of RunNumbers
	    } //for each Run Number in Trigger's list
	  ProcessTrigger();
	} // if Triggername is OK 
    } 

  //  for (mItr = sTLI_LnTriggerMap.begin(); mItr != sTLI_LnTriggerMap.end(); mItr++ )
  //    {
  //      TriggerLineInfo::resetStaticEntries();
  //      setTriggername(mItr->first);
  //      if (checkTriggername())
  //	{
  //	  for (unsigned int i = 0; i < mItr->second.size(); ++i)
  //	    {
  //	      string filename = getFilenameForTrigger(nth, mItr->second[i]); 
  //		loadPoints(mItr->second[i], filename); 
  //		// mItr->second is the std::vector< Int_t >  of RunNumbers
  //	    } //for each Run Number in Trigger's list
  //	  ProcessTrigger();
  //	} // if Triggername is OK 
  //    } 

}


//
// 
string TriggerLineInfo::getFilenameForTrigger (Int_t Level, Int_t RunNumber)
{
  Int_t RunStart = RunNumber / 1000;
  stringstream ss;
  ss << "data/" << RunStart << "/" << RunNumber << "/L" << Level << "/" 
     << Triggername(); 
  return ss.str();
} //getFilenameForL1(Int_t RuNumber)


//
// load points
void TriggerLineInfo::loadPoints(Int_t RunNumber, string filename)
{
  IVector timeList;
  DVector crossList;
  DVector lumiList;
  DVector ErrorList;
  getFourListsFromFile(timeList, crossList, lumiList, ErrorList, filename);

  for (unsigned int index = 0; index < timeList.size(); ++index)
    {
      addEntry(RunNumber,
	       lumiList[index],
	       crossList[index],
	       getTable(RunNumber),
	       ErrorList[index],
	       0,
	       timeList[index]);
    } // for each point add to list of points
}    


// 
// Process Trigger
void TriggerLineInfo::ProcessTrigger()
{
  Calculate();
  if (!checkBeforeFit()) 
    {
      noFitPlots();
      return;
    } // if doesn't pass test, continue to next Trigger
  
  makeGraphs();

  if( (sTLI_fitOriginalPoints == true) && (sTLI_plotOriginalPoints == true) )
    {
      sTLI_orig_p0 = sTLI_p0;
      sTLI_orig_p1 = sTLI_p1;
      sTLI_orig_p2 = sTLI_p2;
      sTLI_orig_p3 = sTLI_p3;
      //      sTLI_orig_pn = sTLI_pn;

      sTLI_orig_e0 = sTLI_e0;
      sTLI_orig_e1 = sTLI_e1;
      sTLI_orig_e2 = sTLI_e2;
      sTLI_orig_e3 = sTLI_e3;
      //      sTLi_orig_en = sTLI_en;

      sTLI_orig_fitError = sTLI_fitError;
      sTLI_OrigVector = sTLI_Vector;
    }
  
  // Clean up Memory for this fit.
  deletePointers (sTLI_pointList);
  deletePointers (sTLI_functionList);
  // write out file for this fit
  
  writeFitInfo();
} // ProcessTrigger


// Line
// check before fit 
bool TriggerLineInfo::checkBeforeFit ()
{
  // If we are using the official fits and there are ANY points,
  // that's good enough
  if (sTLI_useOfficialFit && sTLI_Vector.size())
    {
      return true;
    }
  
  // now would be a good time to see if it is worth going on
  Int_t NumberHi = TriggerLineInfo::NumberHi();
  Int_t NumberLo = TriggerLineInfo::NumberLo();
  
  if ((NumberHi < 2) || (NumberLo < 2))
    {
      sTLI_dontFit |= 1 << TLI_kHiLo;
    } //if too few points high or low
  if (NumberOfGoodPoints() < 6)
    {
      sTLI_dontFit |= 1 << TLI_kGoodPoints;
    } // if not enough points to fit
   
  if (sTLI_dontFit)
    {
      // don't even bother
      return false;
    } //if any reason not to fit, don't

  return true;
} // checkBeforeFit ()


// Line 1489
// makeGraphs()
void TriggerLineInfo::makeGraphs()
{
  if (sTLI_useOfficialFit)
    {
      cout << "Using official fit" << endl;
    }
  else
    {
      cout << "I'm going to fit this puppy." << endl;
    }
  Double_t xArray[TLI_kMaxEntries];
  Double_t exArray[TLI_kMaxEntries];
  Double_t yArray[TLI_kMaxEntries];
  Double_t eyArray[TLI_kMaxEntries];
  
  float xHi = TriggerLineInfo::xHi();
  float xLo = TriggerLineInfo::xLo();
  float yHi = TriggerLineInfo::yHi();
  float yLo = TriggerLineInfo::yLo();
  Int_t NumberHi = TriggerLineInfo::NumberHi();
  Int_t NumberLo = TriggerLineInfo::NumberLo();
  TCanvas *canvas = new TCanvas("Canvas", "canvas", 1); ///////

  if (sTLI_useGrid) canvas->SetGrid();
  
  Int_t Number = getArrays(xArray, exArray, yArray, eyArray);
  
  
  float real_minx = 0., real_maxx = 0.;
  for (Int_t loop = 0; loop < Number; ++loop)
    {
      // for the first entry, just grab the values
      if (0 == loop)
	{
	  real_minx = real_maxx = xArray[0];
	  continue;
	}
      
      if (xArray[loop] > real_maxx)
	{
	  real_maxx = xArray[loop];
	}
      
      if (xArray[loop] < real_minx)
	{
	  real_minx = xArray[loop];
	}
    }

  //dout << "minx " << real_minx << " maxx " << real_maxx << endl;
  TGraphErrors *gr = new TGraphErrors(Number, xArray, yArray, exArray, eyArray);
  sTLI_pointList.push_back( gr );
  float minx = gr->GetXaxis()->GetXmin();
  float maxx = gr->GetXaxis()->GetXmax();
  Double_t maxprob = -1., bestchidf = 9999999.;
  const Int_t TLI_kNumberFits = 5;
  Int_t imax = 0;

  // If we are going to use the official fit, don't bother Running the fitting part
  for (Int_t loop = 0; loop < TLI_kNumberFits && !sTLI_useOfficialFit; ++loop)
    {
      TF1 *func =  new TF1("fun1", "[0] + [1] / x + [2] * x + [3] * x * x", minx, maxx);
      
      sTLI_functionList.push_back(func);
      cout << endl << "Fitting trial " << loop << endl;
      
      if (0 == loop)
	{
	  // the flat fit
	  Double_t p0_seed = ( (NumberLo * yLo + NumberHi * yHi) / (NumberLo + NumberHi) );
	  func->SetParameters(p0_seed, 0., 0.); 
	  func->FixParameter(1, 0.); // turn off 1/L piece
	  func->FixParameter(2, 0.); // turn off L piece
	  func->FixParameter(3, 0.); // turn off L^2 piece
	}
      else
	if (1 == loop) 
	  {
	    // the constant  + 1/L fit
	    Double_t p1_seed = (yHi - yLo) * xHi * xLo / (xLo - xHi);
	    Double_t p0_seed = yLo - p1_seed / xLo;
	    func->SetParameters(p0_seed, p1_seed, 0., 0.); 
	    func->FixParameter(2, 0.); // turn off L piece
	    func->FixParameter(3, 0.); // turn off L^2 piece
	  }
	else 
	  if (2 == loop) 
	    {
	      // linear fit
	      Double_t p2_seed = (yHi - yLo) / (xHi - xLo);
	      Double_t p0_seed = yLo - p2_seed * xLo;
	      func->SetParameters(p0_seed, 0., p2_seed, 0.); 
	      func->FixParameter(1, 0.); // turn off 1/L piece
	      func->FixParameter(3, 0.); // turn off L^2 piece
	    } 
	  else
	    if (3 == loop) 
	      {
		// linear fit + 1/L fit
		Double_t p2_seed_a = (yHi - yLo) / (xHi - xLo);
		Double_t p0_seed_a = yLo - p2_seed_a * xLo;
		Double_t p1_seed_b = (yHi - yLo) * xHi * xLo / (xLo - xHi);
		Double_t p0_seed_b = yLo - p1_seed_b / xLo;
		Double_t p0_seed = (p0_seed_a + p0_seed_b) / 2;
		Double_t p1_seed = p1_seed_b / 2;
		Double_t p2_seed = p2_seed_a / 2;
		func->FixParameter(3, 0.); // turn off L^2 piece
		func->SetParameters(p0_seed, p1_seed, p2_seed, 0.); 
	      }
	    else
	      if (4 == loop) 
		{
		  Double_t p2_seed_a = (yHi - yLo) / (xHi - xLo);
		  Double_t p0_seed_a = yLo - p2_seed_a * xLo;
		  Double_t p1_seed_b = (yHi - yLo) * xHi * xLo / (xLo - xHi);
		  Double_t p0_seed_b = yLo - p1_seed_b / xLo;
		  Double_t p0_seed = (p0_seed_a + p0_seed_b) / 2;
		  Double_t p1_seed = p1_seed_b / 2;
		  Double_t p2_seed = p2_seed_a / 2;
		  func->SetParameters(p0_seed, p1_seed, p2_seed, 0.); 
		} 
	      else
		{
		  cout << "Danger.  We should never be here." << endl;

		  return;
		}
      
      gr->Fit("fun1", "+NQ");
      Double_t chi2 = func->GetChisquare();
      Double_t prob = func->GetProb();
      Double_t ndf = func->GetNDF();
      
      cout << "fit " << loop + 1 << ": chi2 " << chi2 << " prob " << prob << endl;

      for (Int_t inner = 0; inner < TLI_kNumberParams; ++inner)
	{      
	  Double_t params = sTLI_functionList[loop]->GetParameter(inner);
	  Double_t Errors = sTLI_functionList[loop]->GetParError(inner);
	  cout << inner << ") " <<  params << " +- " << Errors << endl;
	}

      // get P1 and make sure it is not negative
      Double_t P1 = func->GetParameter(1);
      Double_t P3 = func->GetParameter(3);
      if ( (P1 < 0) && (0 == P3) )
	{
	  // We don't want to allow for these fits
	  // Make sure they are not the best ones
	  cout << "Tanking fit " << loop << endl;
	  prob = -999999999;
	}
      
      Double_t chidf = chi2/ndf;
      cout << loop << ") " << "prob " << prob << " maxprob " << maxprob << " chidf " 
	   << chidf << " bestchidf " << bestchidf << endl;
      // We want to take the fit with the best (highest) probability.  If
      // two fits have the same probability (usually 0), we want the
      // best (lowest) chi^2 / df

      if ( (prob > maxprob) || ((prob == maxprob) && (chidf < bestchidf)) )
	{   
	  maxprob = prob;        
	  bestchidf = chidf;   
	  imax = loop;        
	  cout << "choosing " << imax << endl;
	} // if ( prob > maxprob )      
    } // for loop
  

  /////////////////////////////
  // /////////////////////// //
  // // Make Pretty Plots // //
  // /////////////////////// //
  /////////////////////////////

    float params[TLI_kNumberParams];
    Double_t Error;

    if (sTLI_useOfficialFit)
      {
	//////////////////
	// Official Fit //
	//////////////////
	params[0] = sTLI_off_p0;
	params[1] = sTLI_off_p1;
	params[2] = sTLI_off_p2;
	params[3] = sTLI_off_p3;
     
	Error = sTLI_off_fitError;
	setFitError (sTLI_off_fitError);
	cout << "Error " << sTLI_fitError << endl;
	setFitParams(params[0], params[1], params[2], params[3], Error);
      } // Official fit 
    else 
      {
	/////////////////
	// Current Fit //
	/////////////////
	float chi2 = sTLI_functionList[imax]->GetChisquare();
	float ndf = sTLI_functionList[imax]->GetNDF();
	cout <<"Chi**2 of " << chi2 << " for " << ndf << " degrees of Freedom" << endl;
	float Errors[TLI_kNumberParams];
	cout << "params: " << endl;

	for (Int_t loop = 0; loop < TLI_kNumberParams; ++loop)
	  {      
	    params[loop] = sTLI_functionList[imax]->GetParameter(loop);
	    Errors[loop] = sTLI_functionList[imax]->GetParError(loop);
	    cout << loop << ") " <<  params[loop] << " +- " << Errors[loop] << endl;
	  }

	setFitParams(params[0], params[1], params[2], params[3], Error);
	setFitParamErrors(Errors[0], Errors[1], Errors[2], Errors[3]);
	cout << endl;

	Error = 0.2;     ////////////// Error = 20% (see about 15 lines below)
	list <Int_t> badRunList;
	CalculateFitInfo (Error, badRunList);
	cout << "Error " << Error << endl;
      } // current fit



    TF1 *upper = new TF1("fun1", "[0] + [1] / x + [2] * x + [3] * x * x", minx, maxx);
    TF1 *lower = new TF1("fun1", "[0] + [1] / x + [2] * x + [3] * x * x", minx, maxx);
    TF1 *main  = new TF1("fun1", "[0] + [1] / x + [2] * x + [3] * x * x", minx, maxx);

    sTLI_functionList.push_back(upper);
    sTLI_functionList.push_back(lower);
    sTLI_functionList.push_back(main);
    //////// Upper factor = 120% of fit cross section value
    float upperfactor = 1 + Error;
    //////// Lower factor = 80%  of fit cross section value   
    float lowerfactor = 1 - Error;
     
    for (Int_t loop = 0; loop < TLI_kNumberParams; ++loop)
      {      
	upper->SetParameter(loop, upperfactor * params[loop]);
	lower->SetParameter(loop, lowerfactor * params[loop]);
	main->SetParameter(loop, params[loop]);
	//  cout << "main  " << loop << " " << main->GetParameter(loop) << endl;
	//  cout << "lower " << loop << " " << lower->GetParameter(loop) << endl;
      }


    upper->SetLineWidth(TLI_kLineWidth + 1); //Width =2+1=3
    lower->SetLineWidth(TLI_kLineWidth + 1); //Width =2+1=3
    main->SetLineWidth(TLI_kLineWidth);      //Width =2
    upper->SetLineColor(41);
    lower->SetLineColor(41);
    main->SetLineColor(1);
    // make sure everything fits
    Double_t miny1 = gr->GetYaxis()->GetXmin();
    Double_t maxy1 = gr->GetYaxis()->GetXmax();
   
    
    //dout << "minx " << minx << " maxx " << maxx << endl;
    float uppermax = upper->GetMaximum(real_minx, real_maxx);
    float lowermin = lower->GetMinimum(real_minx, real_maxx);
    //dout << "miny " << miny1 << " maxy " << maxy1 << endl;
    //dout << "uppermax " << uppermax << " lowermin " << lowermin << endl;
    if (uppermax > maxy1)
      {
	maxy1 = uppermax;
      }
    if (lowermin < miny1)
      {
	miny1 = lowermin;
      }
    if (miny1 < 0)
      {
	miny1 = 0;
      }
    //dout << "miny " << miny1 << " maxy " << maxy1 << endl;
    //dout << "uppermax " << uppermax << " lowermin " << lowermin << endl;
    // increase size on bottom by 5%
    Double_t newMin = maxy1 - 1.05 * ( maxy1 - miny1); 
    // increase size on top by 5%
    Double_t newMax = maxy1 + 0.05 * ( maxy1 - miny1); 
    //dout << "newMin " << newMin << " newMax " << newMax << endl;
    
    if(sTLI_fitOriginalPoints && sTLI_plotOriginalPoints)
      {
	// remember newMin and newMax and use them while plotting orig points on merged fits
	// For the case of plotting merged points on merged fit, no need to do anything,
	// just use miny1, maxy1 as they are.
	sTLI_origMiny1 = newMin;
	sTLI_origMaxy1 = newMax;
      }
    if(!sTLI_fitOriginalPoints && sTLI_plotOriginalPoints)
      {
	newMin = sTLI_origMiny1;
	newMax = sTLI_origMaxy1;
      }
    
    gr->SetMaximum(newMax);
    gr->SetMinimum(newMin);



    // start drawing
    string name = Triggername() + " Cross Section vs. Avg. Inst. Lumi";
    gr->SetTitle(name.c_str());

    char name2[50] ;
    sprintf(name2,"Avg. Inst. Lumi (E%i cm-2 s-1)",(int)TLI_kLumiExponent);

    gr->GetXaxis()->SetTitle (name2);
    gr->GetYaxis()->SetTitle ("Cross Section (nb)");
    gr->Draw("AP");
    upper->Draw("same");
    lower->Draw("same");
    main->Draw("same");
    Int_t NumberPoints = 0;
    
    if(sTLI_plotOriginalPoints && sTLI_fitOriginalPoints)
      NumberPoints = sTLI_Vector.size();

    if( (!sTLI_plotOriginalPoints)&&(!sTLI_fitOriginalPoints) )
      NumberPoints = sTLI_MergedVector.size();

    if( (sTLI_plotOriginalPoints)&&(!sTLI_fitOriginalPoints) )
      NumberPoints = sTLI_OrigVector.size(); 




    bool drawLegend = true;
    
    for (Int_t loop = 0; loop < NumberPoints; ++loop)
      {
	Double_t x, y, ex, ey, residual, residualError;
	Int_t group, RunNumber;
	//Nagesh
	bool isUsedInFit = getPoint (loop, 
				     RunNumber, 
				     x, 
				     y, 
				     ex, 
				     ey,
				     group,
				     residual,
				     residualError);
        if( sTLI_plotOriginalPoints && sTLI_fitOriginalPoints )
	  {
	    isUsedInFit = getPoint (loop,
				    RunNumber,
				    x, 
				    y,
				    ex,
				    ey,
				    group,
				    residual,
				    residualError);
            // This will create .eps file
	    drawLegend = true;
	  }

	if( (!sTLI_plotOriginalPoints)&&(!sTLI_fitOriginalPoints) )
	  {
	    isUsedInFit = getMergedPoint (loop, RunNumber, x, y, ex, ey, group, residual, residualError);
	    // This will create _Merged1.eps file
            drawLegend = false;            
	  }
	
         if( (sTLI_plotOriginalPoints)&(!sTLI_fitOriginalPoints) )
	   {
	     isUsedInFit = getOrigPoint (loop, RunNumber, x, y, ex, ey, group, residual, residualError);
	     //This will create _Merged2.eps file
	     drawLegend = true;
	   }

	 Double_t zero = 0.;
	 TGraphErrors *gg = new TGraphErrors(1, &x, &y, &ex, &zero);
	 sTLI_pointList.push_back( gg );
	 if (isUsedInFit)
	   {
	     gg->SetMarkerColor(sTLI_colorArray[ (group + TLI_kNumberColors) % 
						 TLI_kNumberColors]);
	     gg->SetMarkerStyle(sTLI_styleArray[ (group + TLI_kNumberStyles) % 
						 TLI_kNumberStyles]);
	   }
	 else
	   {
	     gg->SetMarkerColor( TLI_kBadPointsColor );
	     gg->SetMarkerStyle( TLI_kBadPointsStyle );
	   } // if bad point or not
	 gg->Draw("P"); // Draw single point.
      } // for loop
    
    if(drawLegend)
      sTLI_leg->Draw();




    
    string filename = baseFilename() + ".eps";
    if( (sTLI_fitOriginalPoints == false) && (sTLI_plotOriginalPoints == false) )
    {
      filename = baseFilename() + "_WeightedAvg.eps"; //Merged1
    }
    if( (sTLI_fitOriginalPoints == false) && (sTLI_plotOriginalPoints == true) )
      { 
	filename = baseFilename() + "_WeightedAvgReplot.eps"; ///Merged2
      }
    
    canvas->Print(filename.c_str());
    delete canvas;
    canvas = new TCanvas("Canvas", "canvas");
    if (sTLI_useGrid)
      canvas->SetGrid();
    Number = getRunArrays(xArray, exArray, yArray, eyArray );
    TGraphErrors *gr2 = new TGraphErrors(Number, xArray, yArray, exArray, eyArray);
    sTLI_pointList.push_back( gr2 );

    if (sTLI_mode == TLI_kRootFile)
      {
	name = Triggername() + " Residual vs. Run Number";
      }
    else
      if (sTLI_mode == TLI_kDataBase)
	{
	  name = Triggername() + " Residual vs. Chronological Order";
	} // title based on mode

    gr2->SetTitle(name.c_str());
    Double_t miny = gr2->GetYaxis()->GetXmin();
    Double_t maxy = gr2->GetYaxis()->GetXmax();
    if (miny > -3.6) 
      {
	gr2->SetMinimum(-3.6);
      }
    if (maxy < 3) 
      {
	gr2->SetMaximum(3.);
      } 
    if ((maxy > 3) ||  (miny < -3))
      {
	gr2->SetMinimum(-7.);
	gr2->SetMaximum(11.);
      }
    if (TLI_kRootFile == sTLI_mode)
      {
	gr2->GetXaxis()->SetTitle ("Run Number");
      }
    else 
      {
	gr2->GetXaxis()->SetTitle ("Chronological Order");
      } //label axis based on rootfile or database mode
    gr2->GetYaxis()->SetTitle ("Residual");
    gr2->Draw("AP");
    // Place in lower right corner.
    sTLI_Index = 0;


    for (Int_t loop = 0; loop < NumberOfEntries(); ++loop)
      {
	Double_t x, y, ex, ey, residual, residualError;
	Int_t group, RunNumber, time;
	TGraphErrors *gg;
	Double_t RunDouble_T;
	Double_t indexDouble_T;
	bool isUsedInFit = false;

	if (TLI_kRootFile == sTLI_mode)
	  {
	    isUsedInFit = getPoint (loop,
				    RunNumber,
				    x,
				    y,
				    ex,
				    ey,
				    group,
				    residual,
				    residualError);
	    sTLI_ResidualStatMap[RunNumber].Add (residual);//sums up resid. over Run
	    RunDouble_T = RunNumber;
	    residualError = 0;
	    gg = new TGraphErrors(1, &RunDouble_T, &residual, &ex, &residualError);
	  }
	else
	  if ((TLI_kTXMonRoot == sTLI_mode) || (TLI_kDataBase == sTLI_mode))
	    {
	      isUsedInFit = getPoint (loop,
					RunNumber,
					x,
					y,
					ex,
					ey, 
					group,
					residual,
					residualError,
					time);
	      sTLI_ResidualStatMap[RunNumber].
		Add (residual);//sums up resid. over Run
	      indexDouble_T = time;
	      //indexDouble_T = ++sTLI_Index + 2 * group;
	      residualError = 0;
	      gg = new TGraphErrors(1, &indexDouble_T, &residual, 0, &residualError);
	    } //if rootFile or DataBase mode
	sTLI_pointList.push_back( gg );

	if (isUsedInFit)
	  {
	    gg->SetMarkerColor(sTLI_colorArray[ (group + TLI_kNumberColors) % 
						TLI_kNumberColors]);
	    gg->SetMarkerStyle(sTLI_styleArray[ (group + TLI_kNumberStyles) % 
						TLI_kNumberStyles]);
	  }
	else
	  {
	    gg->SetMarkerColor( TLI_kBadPointsColor );
	    gg->SetMarkerStyle( TLI_kBadPointsStyle );     
	  } // if bad point or not
	gg->Draw("P"); // Draw single point.
      } // for loop

    sTLI_leg->Draw();
    minx = gr2->GetXaxis()->GetXmin();
    maxx = gr2->GetXaxis()->GetXmax();
    // draw guide lines
    TLine zero (minx, 0., maxx, 0.);
    zero.SetLineColor(46);
    TLine plus (minx, 1., maxx, 1.);
    plus.SetLineColor(41);
    TLine minus (minx, -1., maxx, -1.);
    minus.SetLineColor(41);

    if (sTLI_useGrid) 
      {
	// consider making these lines dashed
	zero.SetLineWidth (2 * TLI_kLineWidth);
	plus.SetLineWidth (2 * TLI_kLineWidth);
	minus.SetLineWidth(2 * TLI_kLineWidth);
      }
    else 
      {
	// these are just fine as is.
	zero.SetLineWidth (TLI_kLineWidth);
	plus.SetLineWidth (TLI_kLineWidth);
	minus.SetLineWidth(TLI_kLineWidth);
      }
    
    zero.Draw();
    plus.Draw();
    minus.Draw();
    filename = baseFilename() + "_residual.eps";
    canvas->Print(filename.c_str());
    delete canvas;
    // // If we used an official fit this time, we don't
    // // want to use the same fit for the next time, so turn it off
    // // (if it was already off, no harm, no foul)
    // sTLI_useOfficialFit = false;
} //makeGraphs()


//
// find group
Int_t TriggerLineInfo::findGroup(Int_t RunNumber) 
{
  for (unsigned int RunIndex = 0; RunIndex < sTLI_RunNumbers.size(); ++RunIndex)
    {
      if (RunNumber == sTLI_RunNumbers[RunIndex])
	{
	  return RunIndex;
	} // if found Run Number
    } // for all the Run Numbers
  return 0;
} 


//
// get Table
string TriggerLineInfo::getTable(Int_t RunNumber)
{
  if ( sTLI_physicsTables.end() != sTLI_physicsTables.find(RunNumber) )
    {
      return sTLI_physicsTables[RunNumber];
    } // if the physics table is already known
  
  Int_t RunStart = RunNumber / 1000;
  stringstream ss;
  ss << "data/" << RunStart << "/" << RunNumber << "/" << "physicsTable.txt";
  string filename = ss.str();

  ifstream source(filename.c_str(), ios::in);

  if (! source)
    {
      cerr << "file " << filename << "could not be opened" << endl;
      return "physics table unknown";
    }
  string line;
  getline (source, line);
  return line;
}   


// 
// Check Triggername
bool TriggerLineInfo::checkTriggername()
{
  if (TLI_kDataBase == sTLI_mode) 
    {
      return true;
    } // database mode checks and corrects for bad name earlier
  
  //cout << "Trigger: "<<sTLI_Triggername <<endl;
  // check for names of things we don't want to fit
  if ( (string::npos != Triggername().find("_PS0")) ||
       (string::npos != Triggername().find("_DPS")) ||
       (string::npos != Triggername().find("_RL"))  ||
       (string::npos != Triggername().find("_FPS")) ||
       (string::npos != Triggername().find("_UPS")) ||
       (string::npos != Triggername().find("COT_SPIKE")) ||
       (string::npos != Triggername().find("ERROROR")) )
    {
      cout << "not fitting " << Triggername() << " because you told me not to." << endl;
      sTLI_dontFit |= 1 << TLI_kname;
      return false;

    } // if Triggername matches bad Triggers
  return true;
} 


// 
// Split
void TriggerLineInfo::Split (SVector &retval, string line)
{
  retval.clear();
  line += " "; // get last word of line
  unsigned int last = string::npos;
  unsigned int current = line.find(" ");
  
  while (current != string::npos)
    {
      unsigned int pos;
      if (last != string::npos)
	{
	  pos = last + 1;
	}
      else
	{
	  pos = 0;
	}
      string part = line.substr( pos, current - last - 1);
      // don't bother adding 0 length strings

      if (part.length()) 
	{
	  retval.push_back(part);
	}
      last = current;
      current = line.find(" ", current + 1);
    } 
}


//
// get Single List from File
void TriggerLineInfo::getSingleListFromFile (IVector &iList, string filename)
{
  iList.clear();
  ifstream source(filename.c_str(), ios::in);
  if (! source)
    {
      cerr << "file " << filename << "could not be opened" << endl;
      return;
    }
  string line;
  while (getline (source, line))
    {
      std::vector< std::string > sList;
      Split (sList, line);
      if (sList.size() < 1)
	{
	  continue;
	}
      // make sure this line isn't commented out
      string firstChar = sList[0].substr(0, 1);

      if ("#" == firstChar)
	{
	  continue;
	}      
      Int_t RunNumber = atoi( sList[0].c_str() );
      if (RunNumber > 0)
	{
	  iList.push_back( RunNumber );
	} // positive RunNumber
    } // while getline   
}


// 
// get Single List From File
void TriggerLineInfo::getSingleListFromFile (std::vector< std::string > &finalList,
					     string filename)
{
  finalList.clear();
  ifstream source(filename.c_str(), ios::in);
  if (! source)
    {
      cerr << "file " << filename << "could not be opened" << endl;
      return;
    }
  
  string line;
  
  while (getline (source, line))
    {
      std::vector< std::string > sList;
      Split (sList, line);
      
      if (sList.size() < 1)
	{
	  continue;
	}
      // make sure this line isn't commented out
      string firstChar = sList[0].substr(0, 1);
      if ("#" == firstChar)
	{
	  continue;
	}      
      finalList.push_back( sList[0] );
    } // while getline   
}


//
// get Double_T List From File
void TriggerLineInfo::getDoubleListFromFile (IVector  &lowerList,
					     IVector  &upperList,
					     string filename)
{
  lowerList.clear();
  upperList.clear();
  ifstream source(filename.c_str(), ios::in);
  if (! source)
    {
      cerr << "file " << filename << "could not be opened" << endl;
      return;
    }

  string line;
  while (getline (source, line))
    {
      std::vector< std::string > sList;
      Split (sList, line);
      if (sList.size() < 1)
	{
	  continue;
	}

      // make sure this line isn't commented out
      string firstChar = sList[0].substr(0, 1);
      if ("#" == firstChar)
	{
	  continue;
	}      
      Int_t lower, upper = 9999999;
      lower = atoi( sList[0].c_str() );
      if (sList.size() >= 2) 
	{
	  upper = atoi( sList[1].c_str() );
	}
      if (lower > 0)
	{
	  lowerList.push_back( lower );
	  upperList.push_back( upper );
	} // positive RunNumber
    } // while getline   
}


// Line
// get Three lists from file
void TriggerLineInfo::getThreeListsFromFile (IVector  &timeList,
					     DVector &crossList,
					     DVector &lumiList,
					     string filename)
{
  timeList.clear();
  crossList.clear();
  lumiList.clear();
  ifstream source(filename.c_str(), ios::in);
  
  if (! source)
    {
      cerr << "file " << filename << "could not be opened" << endl;
      return;
    }
  string line;

  while (getline (source, line))
    {
      std::vector< std::string > sList;
      Split (sList, line);
      
      if (sList.size() < 2)
	{
	  continue;
	}

      // make sure this line isn't commented out
      string firstChar = sList[0].substr(0, 1);
      if ("#" == firstChar)
	{
	  continue;
	}   
   
      Int_t time;
      Double_t cross, lumi;
      time  = atoi( sList[0].c_str() );
      cross = atof( sList[1].c_str() );
      lumi  = atof( sList[2].c_str() );
      timeList.push_back ( time );
      crossList.push_back( cross );
      lumiList.push_back( lumi );
    } // while getline   
} 


// 
// get three lists from file
void TriggerLineInfo::getThreeListsFromFile (SVector &nameList, 
					     IVector &lowerList, 
					     IVector  &upperList,
					     string filename)
{
  nameList.clear();
  lowerList.clear();
  upperList.clear();
  ifstream source(filename.c_str(), ios::in);
  if (! source)
    {
      cerr << "file " << filename << "could not be opened" << endl;
      return;
    }
  string line;

  while (getline (source, line))
    {
      std::vector< std::string > sList;
      Split (sList, line);
      if (sList.size() < 2)
	{
	  continue;
	}
      // make sure this line isn't commented out
      string firstChar = sList[0].substr(0, 1);
      if ("#" == firstChar)
	{
	  continue;
	}      
      Int_t lower, upper = 9999999;
      lower = atoi( sList[1].c_str() );
      if (sList.size() >= 3) 
	{
	  upper = atoi( sList[2].c_str() );
	}
      if (lower > 0)
	{
	  nameList.push_back ( sList[0] );
	  lowerList.push_back( lower );
	  upperList.push_back( upper );
	} // positive RunNumber
    } // while getline   
}


//
// get four lists from file
void TriggerLineInfo::getFourListsFromFile (IVector  &timeList,
					    DVector &crossList,
					    DVector &lumiList,
					    DVector &ErrorList,
					    string filename)
{
  timeList.clear();
  crossList.clear();
  lumiList.clear();
  ErrorList.clear();
  ifstream source(filename.c_str(), ios::in);
  
  if (! source)
    {
      cerr << "file " << filename << "could not be opened" << endl;
      return;
    }
  
  string line;
  while (getline (source, line))
    {
      std::vector< std::string > sList;
      Split (sList, line);
      if (sList.size() < 4)
	{
	  continue;
	}
      // make sure this line isn't commented out
      string firstChar = sList[0].substr(0, 1);
      if ("#" == firstChar)
	{
	  continue;
	}   
   
      Int_t time;
      Double_t cross, lumi, Error;
      time  = atoi( sList[0].c_str() );
      cross = atof( sList[1].c_str() );
      lumi  = atof( sList[2].c_str() );
      Error = atof( sList[3].c_str() );
      timeList.push_back ( time );
      crossList.push_back( cross );
      lumiList.push_back( lumi );
      ErrorList.push_back( Error );
    } // while getline   
} // getFourListsFromFile Int_t, float, float, float


// Line
// write fit info
void TriggerLineInfo::writeFitInfo()
{
  string filename = baseFilename() + ".html";
  ofstream target (filename.c_str(), ios::out);

  if (! target)
    {
      cerr << "file " << filename << "could not be opened" << endl;
      return;
    }
  cout << "writing " << filename << endl;
  target << "<html><title>" << Triggername() << "</title>"
	 <<endl
	 <<endl
	 <<"<!-- Start Common -->"
	 <<endl
	 <<"<h1><center>" << Triggername() << "</center></h1>" << endl;

  if (sTLI_lineIsFit)
    {
      // line is successfully fit
      target << "<table>" << endl
             << "<tr><td><a href=\"" << basename() << ".eps\">"
             << "<img src=\"" << basename() << ".gif\"></a></td>" << endl;
      if (sTLI_useOfficialFit)
	{
         // Official fit
	  target << "<td><table>" << endl;	  
	  target << "<tr><td colspan=2><b>Official Fit Used</b></td></tr>" << endl
		 << "<tr><td><b>p0</b></td><td>" << sTLI_orig_p0 << "</td></tr>" << endl
		 << "<tr><td><b>p1</b></td><td>" << sTLI_orig_p1 << "</td></tr>" << endl
		 << "<tr><td><b>p2</b></td><td>" << sTLI_orig_p2 << "</td></tr>" << endl
		 << "<tr><td><b>p3</b></td><td>" << sTLI_orig_p3 << "</td></tr>" << endl;
	  //     << "<tr><td><b>pn</b></td><td>" << sTLI_orig_pn << "</td></tr>" << endl;

	  target << "<tr><td><b>Fit Error</b></td>" 
		 << "<td>" << sTLI_orig_fitError << "</td></tr>"<< endl
		 << "<tr><td><a href=\"" << basename() 
		 << ".html\">html</a></td></tr>" << endl
		 << "</table>" << endl;
	}
      else
	{
	  // Current fit
	  target <<"<td><table>" << endl;
	  target <<"<tr><td><b>p0</b></td><td>"   <<sTLI_orig_p0 
		 <<"</td><td> &plusmn;</td><td> " <<sTLI_orig_e0 << "</td></tr>" << endl
		 <<"<tr><td><b>p1</b></td><td>"   <<sTLI_orig_p1 
		 <<"</td><td> &plusmn;</td><td> " <<sTLI_orig_e1 << "</td></tr>" << endl
		 <<"<tr><td><b>p2</b></td><td>"   <<sTLI_orig_p2 
		 <<"</td><td> &plusmn;</td><td> " <<sTLI_orig_e2 << "</td></tr>" << endl
		 <<"<tr><td><b>p3</b></td><td>"   <<sTLI_orig_p3 
		 <<"</td><td> &plusmn;</td><td> " <<sTLI_orig_e3 << "</td></tr>" << endl;
	  //     <<"<tr><td><b>pn</b></td><td>"   <<sTLI_orig_pn 
	  //     <<"</td><td> &plusmn;</td><td> " <<sTLI_orig_en << "</td></tr>" << endl

         target << "<tr><td><b>Fit Error</b></td>" << "<td colspan=3 align=center>"
		<< sTLI_orig_fitError << "</td></tr>"<< endl
                << "<tr><td><a href=\"" << basename() << ".html\">html</a></td></tr>" << endl
                << "</table>" << endl;
	}

      target << "</td>" << endl;
      target << "<td><a href=\"" << basename() << "_WeightedAvg2.eps\">" << "<img src=\""
	     << basename() << "_WeightedAvg2.gif\"></a></td>";      
      target << "<td><a href=\"" << basename() << "_WeightedAvg1.eps\">" << "<img src=\""
	     << basename() << "_WeightedAvg1.gif\"></a></td>" << endl;

      if (sTLI_useOfficialFit)
	{
	  // Official fit
	  target << "<td><table>" << endl;
	  target << "<tr><td colspan=2><b>Official Fit Used</b></td></tr>" << endl
		 << "<tr><td><b>p0</b></td><td>" << sTLI_p0 << "</td></tr>" << endl
		 << "<tr><td><b>p1</b></td><td>" << sTLI_p1 << "</td></tr>" << endl
		 << "<tr><td><b>p2</b></td><td>" << sTLI_p2 << "</td></tr>" << endl
		 << "<tr><td><b>p3</b></td><td>" << sTLI_p3 << "</td></tr>" << endl;
	  //     << "<tr><td><b>pn</b></td><td>" << sTLI_pn << "</td></tr>" << endl

	  target << "<tr><td><b>Fit Error</b></td>"
		 << "<td>" << sTLI_fitError << "</td></tr>"<< endl
		 << "<tr><td><a href=\"" << basename() << ".html\">html</a></td></tr>" << endl
		 << "</table>" << endl;
	}
      else
	{
	  // Current fit
	  target <<"<td><table>" << endl;
	  target <<"<tr><td><b>p0</b></td><td>"   <<sTLI_p0
		 <<"</td><td> &plusmn;</td><td> " <<sTLI_e0 << "</td></tr>" << endl
		 <<"<tr><td><b>p1</b></td><td>"   <<sTLI_p1
		 <<"</td><td> &plusmn;</td><td> " <<sTLI_e1 << "</td></tr>" << endl
		 <<"<tr><td><b>p2</b></td><td>"   <<sTLI_p2
		 <<"</td><td> &plusmn;</td><td> " <<sTLI_e2 << "</td></tr>" << endl
		 <<"<tr><td><b>p3</b></td><td>"   <<sTLI_p3
		 <<"</td><td> &plusmn;</td><td> " <<sTLI_e3 << "</td></tr>" << endl;
	  //     <<"<tr><td><b>pn</b></td><td>"   <<sTLI_pn
	  //     <<"</td><td> &plusmn;</td><td> " <<sTLI_en << "</td></tr>" << endl

	  target << "<tr><td><b>Fit Error</b></td>"
		 << "<td colspan=3 align=center>" << sTLI_fitError << "</td></tr>"<< endl
		 << "<tr><td><a href=\"" << basename() << ".html\">html</a></td></tr>" << endl
		 << "</table>" << endl;
	}
      
      
      target << "<td><a href=\"" << basename() << "_residual.eps\">" << "<img src=\""
	     << basename() << "_residual.gif\"></a></td>" << endl;
      
      target << "</tr></table>" << endl;           
    }
  
  else
    {
      // line is NOT fit
      if (NumberOfEntries())
	{
	  target << "<table>" << endl
		 << "<tr><td><a href=\"" << basename() << ".eps\">" << "<img src=\""
		 << basename() << ".gif\"></a></td>" << endl;
	  target << "<td>" << "  <table><tr><td>This line NOT fit:</td></tr>" << endl;

	  if (sTLI_dontFit & (1 << TLI_kname))
	    {
	      target << "  <tr><td>Bad name</td></tr>" << endl;
	    }
	  if (sTLI_dontFit & (1 << TLI_kGoodPoints))
	    {
	      target << "  <tr><td>Not Enough Good Points</td></tr>" << endl;
	    }
	  else
	    if (sTLI_dontFit & (1 << TLI_kHiLo))
	      {
		target << "  <tr><td>Not Enough High/Low points</td></tr>" << endl;
	      }
	  
	  target  << "<tr><td><a href=\"" << basename() << ".html\">html</a></td></tr>" <<endl 
		  << "  </table>" << endl << "</tr></table>" << endl;
	}

      else
	{
	  target << "No points at all" << endl;
	}
    }
  
  target << "<!-- End Common -->" << endl << endl;


  target << "<pre>" << endl;
  Int_t NumberOfEntries = sTLI_OrigVector.size();
  Int_t NumberOfEntries_merged = sTLI_Vector.size();// this is actually merged Vector now
  
  if (NumberOfEntries)
    {
      // print out header
      //         33.3317     3.07986    179103     0.23           PHYSICS_2_01_v-4,424            Used      
      target << "     Lumi       txsec      Run #   Residual                 Table            Used in Fit               Avg lumi (10<sup>" 
	     << TLI_kLumiExponent << "</sup>)      Avg txsec    " << endl;
    }
  
  Int_t loop1 = 0;  
  for (Int_t loop = 0; loop < NumberOfEntries; ++loop)
    {
      target << sTLI_OrigVector[loop] ; // Lum  txsec  Run # Residual table Used in Fit
      
      if(loop1 < NumberOfEntries_merged)
	{
          target << "                     "<< sTLI_Vector[loop1].x() <<"                 " << sTLI_Vector[loop1].y() << endl;
          loop1++; // Avg lumi Avg txsec
	}
      else
        target << endl;
      
    } // for loop
  target << "</pre>" << endl;



  // Put in the SQL info 
  target << "<!-- Start SQL" << setprecision(-1) << endl;
  // used by L1 and L2
  string name = "TRIGGER_NAME";
  string tag = "TRIGGER_TAG";
  string firstname = "TRIGGER_NAME";
  if (TLI_kL3 == Level())
    {
      // used by L3
      name = "PATH_NAME";
      tag = "PATH_TAG";
      firstname = "TRIGGER_PATH";
    }

  target << "UPDATE " << firstname << "S SET CROSS_SECTION_NB = "    << p0()       
	 << " WHERE " << name << " = '" << noVersionname() 
	 << "' AND " << tag << "=" << VersionTag() << ";" << endl;
  target << "UPDATE " << firstname << "S SET CROSS_SECTION_RATE = "  << p1()      
	 << " WHERE " << name << " = '" << noVersionname() 
	 << "' AND " << tag << "=" << VersionTag() << ";" << endl;
  target << "UPDATE " << firstname << "S SET CROSS_SECTION_COMB = "  << p2()      
	 << " WHERE " << name << " = '" << noVersionname() 
	 << "' AND " << tag << "=" << VersionTag() << ";" << endl;
  target << "UPDATE " << firstname << "S SET CROSS_SECTION_QUAD = "  << p3()       
	 << " WHERE " << name << " = '" << noVersionname() 
	 << "' AND " << tag << "=" << VersionTag() << ";" << endl;
  target << "UPDATE " << firstname << "S SET CROSS_SECTION_RANGE = " << fitError() 
	 << " WHERE " << name << " = '" << noVersionname() 
	 << "' AND " << tag << "=" << VersionTag() << ";" << endl;
  target << "End SQL -->" << endl;
  target.close();
}


//
//
void TriggerLineInfo::MakeStatisticsWebsite ()
{
  ofstream file("stats.html", ios::out);
  cout <<"writing stats.html ..."<< endl;
  file << "<html><title> TXMON Stats </title>" << endl
       << "<h1><center> TXMON Stats </center></h1>" << endl;
  
  map < Int_t, ResidualStat > ::iterator itr;
  map < Double_t, Int_t > R_SMap;//residal
  map < Double_t, Int_t > R_RMSMap;//RMS of residual
  map < Int_t, string > GoodBadMap; //keeps track of good/bad Runs
  map < Int_t, Int_t > R_BadMap; //map of variable sumBad

  for (itr = sTLI_ResidualStatMap.begin(); 
       itr != sTLI_ResidualStatMap.end(); itr++)
    {
      Double_t mean;
      Double_t RMS;
      Double_t SDOM;

      RMS  = itr->second.CalculateRMS();
      mean = itr->second.CalculateMean();
      SDOM = itr->second.CalculateSDOM();

      R_SMap[ mean ] = itr->first;
      R_RMSMap[ RMS ] = itr->first;
      R_BadMap [ itr->second.sumBad() ] = itr->first;
      GoodBadMap[ itr->first ] = "&nbsp;";  
       
      for (unsigned int i = 0; i < sTLI_singleBadList.size(); i++)
	if (sTLI_singleBadList[i] == itr->first ) 
	  GoodBadMap[ itr->first ] = "bad";
    }//create reverse map and map of bad Runs

  file << "<h2><center> Highest Ten Mean Residuals </h2></center>" << endl
       << "<table border = 3 cellpadding = 2> " <<endl
       << "<tr align = center><th> Number </th><th> Run Number </th>" 
       << "<th> Mean of Residuals </th>"
       << "<th> Good / Bad Run </th></tr>" << endl;

  map < Double_t, Int_t >::reverse_iterator rItr;
  rItr = R_SMap.rbegin();

  for (Int_t i = 0 ; i < 10 && rItr != R_SMap.rend(); i++ ,rItr++)
    {
      file << setprecision(3) << setw(8) << setiosflags(ios::fixed)
           << "<tr align = center><td>  " << i + 1 << "  </td>"
           << "<td>  " << rItr->second << "  </td>"
           << "<td>  " << rItr->first << "  </td>"
           << "<td>  " << GoodBadMap[ rItr->second ] << "  </td>"
           << "</tr>" << endl;
   }//prints highest ten residuals

  file << "</table>" << endl;
  
  file << "<h2><center> Highest Ten RMS of Residuals </h2></center>" << endl
       << "<table border = 3 cellpadding = 2>" << endl
       << "<tr align = center><th> Number </th><th> Run Number </th>"  
       << "<th> RMS of Residual </th> "
       << "<th> Good / Bad Run </th></tr>" << endl;
  rItr = R_RMSMap.rbegin();

  for (Int_t i = 0; i < 10 && rItr != R_RMSMap.rend(); i++ ,rItr++)
    {
      file << "<tr align = center><td>  " << i + 1 << "  </td>"
           << "<td>  " << rItr->second << "  </td>"
           << "<td>  " << rItr->first << "  </td>"
           << "<td>  " << GoodBadMap[ rItr->second ] << "  </td>"
           << "</tr>" << endl;
    }//prints highest ten RMS of residuals
  file << "</table>" << endl;
   
  file << "<h2><center> Most Bad Triggers </h2></center>" << endl
       << "<table border = 3 cellpadding = 2> " <<endl
       << "<tr align = center><th> Number </th><th> Run Number </th>" 
       << "<th> # bad Triggers </th>"
       << "<th> Good / Bad Run </th></tr>" << endl;

  map < Int_t, Int_t >::reverse_iterator rItrI;
  rItrI = R_BadMap.rbegin();
  
  for (Int_t i = 0 ; i < 10 && rItrI != R_BadMap.rend(); i++ ,rItrI++)
    {
      file << "<tr align = center><td>  " << i + 1 << "  </td>"
           << "<td>  " << rItrI->second << "  </td>"
           << "<td>  " << rItrI->first << "  </td>"
           << "<td>  " << GoodBadMap[ rItrI->second ] << "  </td>"
           << "</tr>" << endl;
    }//prints most Triggers over 1 residual

   file << "</table><br><br>" << endl;

   file << "<center><h2> All the data </h2></center>" << endl
        << "<table border = 3 cellpadding = 2>" << endl
        << "<tr align = center><th> Run Numbers </th>"
        << "<th> Mean of Residuals </th>"
        << "<th> RMS Residual </th>" 
        << "<th> Bad Triggers </th>" 
        << "<th> Good / Bad Run </th></tr>"
        << endl;
     
   for (itr = sTLI_ResidualStatMap.begin(); 
        itr != sTLI_ResidualStatMap.end(); itr++)
     {
       file << "<tr align = center><td>  " << itr->first << "  </td>"
	    << "<td>  " << itr->second.CalculateMean() << "  </td>"
	    << "<td>  " << itr->second.CalculateRMS() << "  </td>" 
	    << "<td>  " << itr->second.sumBad() << " </td>"
	    << "<td>  " << GoodBadMap[ itr->first ] << "  </td></tr>" <<endl;
       
     }//for sTLI_ResidualStatMap
   file << "</table>" <<endl;
   
   file << "</html>" <<endl;
   file.close();
}


// 
// delete pointers
void TriggerLineInfo::deletePointers (vector< TGraphErrors* > &list)
{
  Int_t size = list.size();
  for (Int_t loop = 0; loop < size; ++loop)
    {
      if (list[loop])
	{
	  delete list[loop];
	}
    }
  list.clear();
}


// 
// delete pointers
void TriggerLineInfo::deletePointers (vector< TF1* > &list)
{
  Int_t size = list.size();
  for (Int_t loop = 0; loop < size; ++loop)
    {
      if (list[loop])
	{
	  delete list[loop];
	}
    }
  list.clear();
}


// 
// no fit points
void TriggerLineInfo::noFitPlots()
{
  TriggerLineInfo::setLineIsFit(false); // tells rest to treat as non-fit
  vector< TGraphErrors* > pointList;
  // If there are NO points, don't bother doing anything
  if (0 == TriggerLineInfo::NumberOfEntries())
    {
      cout << "this Trigger had no entries..." << endl;
      return;
    }
  // o.k.  For whatever reason, we are not fitting the points that
  // we have.  Let's plot them and get on to real work
  TCanvas *canvas = new TCanvas("Canvas", "canvas");
  if (sTLI_useGrid) canvas->SetGrid();
  Double_t xArray[TLI_kMaxEntries];
  Double_t exArray[TLI_kMaxEntries];
  Double_t yArray[TLI_kMaxEntries];
  Double_t eyArray[TLI_kMaxEntries];
  Int_t Number = TriggerLineInfo::getArrays(xArray, exArray, yArray, eyArray,true);

  if (0 == Number)
    {
      // this is at least a little strange
      cout << "no points.  Aborting" << endl;
      return;
    }

  cout << "making a no-Fit plot." << endl;
  TGraphErrors *gr = new TGraphErrors(Number, xArray, yArray, exArray, eyArray);
  string name = TriggerLineInfo::Triggername() + " Cross Section vs. Inst. Lum";
  gr->SetTitle(name.c_str());
  gr->GetXaxis()->SetTitle ("Ave. Inst. Luminosity (E 30 cm-2 s-1)");
  gr->GetYaxis()->SetTitle ("Cross Section (nb)");
  gr->Draw("AP");   
  
  for (Int_t loop = 0; loop < TriggerLineInfo::NumberOfEntries(); ++loop)
    {
      Double_t x, y, ex, ey, residual, residualError;
      Int_t group, RunNumber;
      bool isUsedInFit;
      isUsedInFit = getPoint (loop, RunNumber, x, y, ex, ey, group,
                              residual, residualError);
      TGraphErrors *gg = new TGraphErrors(1, &x, &y, &ex, &ey);
      pointList.push_back( gg );
      if (isUsedInFit)
	{
	  gg->SetMarkerColor(sTLI_colorArray[ (group + TLI_kNumberColors) %
					      TLI_kNumberColors]);
	  gg->SetMarkerStyle(sTLI_styleArray[ (group + TLI_kNumberStyles) %
					      TLI_kNumberStyles]);
	}
      else
	{
	  gg->SetMarkerColor( TLI_kBadPointsColor );
	  gg->SetMarkerStyle( TLI_kBadPointsStyle );
	} // if bad point or not
      gg->Draw("P"); // Draw single point.
    } // for loop
  
  sTLI_leg->Draw();
  string filename = baseFilename() + ".eps";
  canvas->Print(filename.c_str());
  writeFitInfo();
  delete canvas;
  delete gr;
  deletePointers (pointList);
}


//
// memory clean up
void TriggerLineInfo::memoryCleanUp ()
{
  cout << "Pointers being erased" << endl;
  deletePointers (sTLI_legendList);
  delete sTLI_leg;
}


// 
// decode level type
TriggerLineInfo::LevelType TriggerLineInfo::decodeLevelType (string name)
{
  string prefix = name.substr(0, 2);
  if ("L1" == prefix)
    {
      return TLI_kL1;
    }
  else
    if ("L2" == prefix)
      {
	return TLI_kL2;
      }
    else
      if (prefix == "L3")
	{
	  return TLI_kL3;
	}
      else
	//  {
	//   return TLI_kLn;
	//  }
	{
	  return TLI_kL3;
	}
}


// 
// Strip Version Info
string TriggerLineInfo::StripVersionInfo (string name, Int_t &Version)
{
  Version = 0;
  unsigned int pos = name.find("_v");
  if (string::npos == pos)
    {
      // no Version here, return the string as is and the Version as 0
      return name;
    }
  string retval = name.substr(0, pos);
  // skip the '_v' and get the Int_teger after
  string VersionString = name.substr(pos + 2);
  Version = atoi (VersionString.c_str());
  return retval;
}


// Line 
// get position of name
Int_t TriggerLineInfo::getPositionOfname (TriggerLineInfo::LevelType Level, 
					  string name, 
					  string XMonRootFile,
					  Int_t &Version)
{
  Version = -1;
  string treename;
  if (TLI_kL1 == Level)
    {
      treename = "l1names";
    }
  else
    if (TLI_kL2 == Level)
      {
	treename = "l2names";
      }
    else
      if (TLI_kL3 == Level)
	{
	  treename = "l3names";
	} 
  //  else
  //	if (TLI_kLn == Level)
  //	  {
  //	    treename == "lnnames";
  //	  }
      else
	{
	  cerr <<"TriggerLineInfo::getPositionOfname called with invalid Level" <<endl;
	  return -1;
	}

  TFile file (XMonRootFile.c_str());
  if (! file.IsOpen())
    {
      cerr << "TriggerLineInfo::getPositionOfname: Couldn't open " << XMonRootFile 
	   << ".  Aborting" << endl;
      return -1;
    }

  TTree *tree = (TTree*) file.Get(treename.c_str());
  if (! tree)
    {
      cerr << "TriggerLineInfo::getPositionOfname: Couldn't find " 
	   << treename << " in " << XMonRootFile << ".  Aborting" << endl;
      return -1;
    }

  Int_t NumberOfEntries = (Int_t) tree->GetEntries();
  Int_t Number;
  char tempname[300];
  tree->SetBranchAddress ("index", &Number);
  tree->SetBranchAddress ("name", tempname);

  for (Int_t loop = 0; loop < NumberOfEntries; ++loop)
    {
      tree->GetEntry(loop);
      Int_t tempVersion;
      string noVersion = StripVersionInfo (tempname, tempVersion);
      // is this the right name?

      if (noVersion == name)
	{
	  // We got a match
	  Version = tempVersion;
	  return loop;
	} // if match
    } // for loop
  // If we're here, it wasn't found
  return -1;
}


// 
// get run number from name
Int_t TriggerLineInfo::getRunNumberFromname (std::string name)
{
  unsigned int lastslash = name.find_last_of("/");
  unsigned int start = name.find_first_of("01234567890", lastslash);
  unsigned int end = name.find_last_of("01234567890");
  if (string::npos == start)
    {
      // no Run Number here
      return -1;
    }
  if (end < start) 
    {
      // this should never happen
      return -1;
    }
  return atoi (name.substr(start, end - start + 1).c_str());
}


// 
// Process one trigger
void TriggerLineInfo::ProcessOneTXMonRootTrigger(string Triggername, 
						 string basename,
						 bool fitBadnames)
{
  if (! Triggername.size())
    {
      cerr << "TriggerLineInfo::ProcessOneTXMonRootTrigger called with no " 
	   << "match name.  Aborting..." << endl;
      return;
    }
  LevelType Level = decodeLevelType (Triggername);
  if (! sTLI_RunListVector.size())
    {
      cerr << "TriggerLineInfo::ProcessOneTXMonRootTrigger called with " 
	   << "no Runlist.  Aborting..." << endl;
      return;
    }
  
  string firstfile = sTLI_RunListVector[0];
  Int_t Version;
  string Novers = StripVersionInfo (Triggername, Version);
  Int_t Position = getPositionOfname (Level, Novers, firstfile, Version);
  if (Position < 0 || Version < 0)
    {
      // this is not going to be successful
      cerr << "TriggerLineInfo::ProcessOneTXMonRootTrigger: can not " 
	   << "find specified Trigger in first root file " << firstfile << "." << endl;
      return;
    }

  resetStaticEntries();
  char longline[100];
  sprintf (longline, "%s_v%d", Novers.c_str(), Version);
  setTriggername (longline);
  if (! fitBadnames)
    {
      checkTriggername();
    }
  if (basename.size())
    {
      setNonDefaultFilename(basename);
    }

  // set tree name    // xsec will be changed to txsec
  TString treename;  
  if (TLI_kL1 == Level)
    {
      treename = "l1xsec";
    }
  else
    if (TLI_kL2 == Level)
      {
	treename = "l2xsec";
      }
    else
      if (TLI_kL3 == Level)
	{
	  treename = "l3xsec";
	}
  //  else
  //	if (TLI_kLn == Level)
  //	  {
  //	    treename = "lnxsec";
  //	  }
      else
	{
	  cerr << "Shoudln't be here" << __FILE__ << " " << __LINE__ << endl;
	  return;
	}
  
  // let's get our points, baby...
  unsigned int NumberFiles = sTLI_RunListVector.size();
  // We want to start with the oldest file first.
  // The files are loaded in newest first, so....
  // we loop over the files backwards
  
  Int_t time = 0;
  Int_t fileCount = -1; // we want it start at 0
  
  for (Int_t loop = (Int_t) NumberFiles - 1; loop >= 0; --loop)
    {
      ++fileCount;
      Int_t thisVersion;
      Position = getPositionOfname (Level, Novers, sTLI_RunListVector[loop], thisVersion);
      if ((thisVersion != Version) || (Position < 0))
	{
	  // we don't want to use old Versions
	  cout << "Skipping " << sTLI_RunListVector[loop] << " due to Version mismatch." << endl;
	  continue;
	} // is there a Version mismatch?

      TFile file (sTLI_RunListVector[loop].c_str());
      if ( ! file.IsOpen())
	{
	  cout << "Couldn't open " << sTLI_RunListVector[loop] << ". Continuing..." << endl;
	  continue;
	}
      
      TTree *tree = (TTree*) file.Get(treename);
      if (! tree)
	{
	  cout << "Couldn't get " << treename << " from " << sTLI_RunListVector[loop]
	       << ". Continuing..." << endl;
	  continue;
	} // if not tree
      // get the Run Number

      Int_t RunNumber = getRunNumberFromname (sTLI_RunListVector[loop]);
      Int_t NumberOfEntries = (Int_t) tree->GetEntries();
      float xsec[400];
      float lumi;
      tree->SetBranchAddress ("lum", &lumi);
      tree->SetBranchAddress ("xsec", xsec);
      
      // if we're still here, we're golden   
      for (Int_t inner = 0; inner < NumberOfEntries; ++inner)
	{
	  tree->GetEntry(inner);
	  float Error =  xsec[Position] * 0.01;
	  /*
	    Comment by Nagesh:
	    We don't want to fit the Runs for which there are less than 3 pionts
	    
	  */
	  
	  if(NumberOfEntries >4)
	    {
	      
	      addEntry (RunNumber, lumi, xsec[Position], "", Error, 0, ++time + 3 * fileCount ); 
	    } 
	  else
	    {
	      ++time;
	    }
	} // for inner
    } // for loop
  
  
  /*
    if(sTLI_fitOriginalPoint == true & sTLI_plotOriginalPoints== true)
    --use sTLI_Vector to fit and plot
    use getPoint() function
    
    if(sTLI_fitOrignalPoints== false & sTLI_plotOrignalPoints == false)
    i.e fit Merged points (by copying merged points Int_to sTLI_Vector) and plot Merged points
    -- use sTLI_MergedVector to plot and fit both.
    use getMergedPoint() function
    
    if(sTLI_fitOrignalPoints == false & sTLI_plotOrignalPoints = true)
    i.e. fit Merged points but plot orignal points
    -- use sTLI_MergedVector for fitting but sTLI_Vector for plotting.
    use getOriginalPoint() function
    
  */
  
  //first Process original Vector and plot original points
  sTLI_fitOriginalPoints = true;
  sTLI_plotOriginalPoints = true;
  
  //Also in the ProcessTrigger() function, remembers some 
  // quantities (after calculate() function is called) 
  // which will be used later (while Processing
  // Trigger again for merged points.
  
  ProcessTrigger();
   
  
  //now create new Vector by merging points of original Vector
  
  sTLI_fitOriginalPoints = false;
  sTLI_plotOriginalPoints = false;
  
  
  Int_t binsize = 10;
  for(Int_t i = 0; i< 300; i+= binsize)
    {
      MergePoints(i,i+ binsize);
    }
  
  //copy this new merged Vector Int_to the original Vector                          
  sTLI_Vector.clear();
  sTLI_Vector = sTLI_MergedVector;
  
  //Process Trigger : fit merged points, plot merged points. 
  
  ProcessTrigger();
  
  // now Process Trigger: fit merged points but plot original points
  
  sTLI_fitOriginalPoints = false;
  sTLI_plotOriginalPoints = true;
  
  ProcessTrigger();
  
  //set flags to original for next Trigger  
  
  sTLI_fitOriginalPoints = true;
  sTLI_plotOriginalPoints = true;
    
}


//
// set run list
void TriggerLineInfo::setRunList (const std::vector< std::string > &Vector) 
{
  sTLI_RunListVector = Vector;
  sTLI_RunNumbers = getOrderedListFromFilenames (sTLI_RunListVector);
}


//
//  get ordered list from filenames
TriggerLineInfo::IVector TriggerLineInfo::
getOrderedListFromFilenames(const TriggerLineInfo::SVector &Vector)
{
  IVector retval;
  unsigned int entries = Vector.size();
  for (unsigned loop = 0; loop < entries; ++loop)
    {
      Int_t Run = getRunNumberFromname ( Vector[loop] );
      if (Run > 0)
	{
	  retval.push_back( Run );
	}
    } // for loop
  return retval;
}


//
//
void TriggerLineInfo::setNonDefaultFilename (const string &name)
{
  sTLI_otherFilename = name;
  sTLI_useOtherFilename = true;
  unsigned int pos = name.find_last_of("/");
  if (string::npos != pos)
    {
      sTLI_basename = name.substr(pos + 1);
    }
  else
    {
      sTLI_basename = name;
    }   
}


// 
// all Triggers In Root File
TriggerLineInfo::SVector TriggerLineInfo::allTriggerLinesInRootFile (const std::string &filename)
{
  std::vector< std::string > retval;         ///////////3024
  TFile file (filename.c_str());
  if (! file.IsOpen())
    {
      cerr << "TriggerLineInfo::alltriigerLinesInRootFile: Couldn't open " << filename 
	   << ".  Aborting" << endl;
      return retval;
    }
  
  for (Int_t Level = 1; Level <= 3; ++Level) // 3 -> n for n triggers
    {
      TString treename;
      if (Level == 1)
	{
	  treename = "l1names";
	}
      else
	if (Level == 2)
	  {
	    treename = "l2names";
	  }
	else
	  if (Level == 3)
	    {
	      treename = "l3names";
	    }
      //	  else
      //	    if (Level == nth)
      //	      {
      //		treename = "lnnames";
      //	      }
      
      TTree *tree = (TTree*) file.Get(treename);
      if (! tree)
	{
	  cerr << "TriggerLineInfo:::allTriggerLinesInRootFile Couldn't find "
	       << treename << " in " << filename << ".  Aborting" << endl;
	  continue;
	}
      Int_t NumberOfEntries = (Int_t) tree->GetEntries();
      char tempname[300];
      tree->SetBranchAddress ("name", tempname);
      for (Int_t loop = 0; loop < NumberOfEntries; ++loop)
	{
	  tree->GetEntry(loop);
	  retval.push_back( tempname );
	} // for loop
    } // for Level
  return retval;
}


//
//
void TriggerLineInfo::useOfficialFit(Double_t p0,
				     Double_t p1,
				     Double_t p2,
				     Double_t p3,
				     Double_t fitError)

{
  sTLI_useOfficialFit = true;
  sTLI_off_p0 = p0;
  sTLI_off_p1 = p1;
  sTLI_off_p2 = p2;
  sTLI_off_p3 = p3;
  // sTLI_off_pn = pn
  sTLI_off_fitError = fitError;
  cout << "In TriggerLineInfo::useOfficialFit" << endl;
}



/////////////
// Friends //
/////////////
ostream& operator<< (ostream& o_stream, const TriggerLineInfo &rhs)
{
  float residual = ((Int_t) (100 * rhs.residual())) / 100.;
  o_stream << setw(9) << rhs.x() << setw(12) << rhs.y()
	   << setw(10)  << rhs.RunNumber() 
	   << setprecision(3) << setw(11) << residual
	   << "  "
	   << setw(32) << rhs.table() << "   "
	   << setw(10) << rhs.usedInFitString();
  return o_stream;
} 
