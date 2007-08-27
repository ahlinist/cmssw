

#if !defined(TriggerLineInfo_H)
 #define TriggerLineInfo_H

 #include <list>
 #include <iostream>
 #include <vector>
 #include <string>
 #include <map>
 #include <iomanip>
 #include <cstdlib>
 #include "TGraphErrors.h"
 #include "TF1.h" 
 #include "TSystem.h"
 #include "TLegend.h"
 #include "TPaveText.h"
 #include "TGaxis.h"
 #include "TCanvas.h"
 #include "TStyle.h"
 #include "TH2.h"  
 #include "ResidualStat.h"


 class TriggerLineInfo 
 { 




   typedef std::vector<std::string> SVector;
   typedef std::vector<Int_t> IVector;
   typedef std::vector<Double_t> DVector;
   typedef std::list<Double_t> DList;
   typedef std::list<Int_t> IList;

  

 public:
   //////////////////////
   // Public Constants //
   //////////////////////
   
      
   // Type of Data File
   enum modeType  
     {
       TLIc_RootFile,
       TLIc_DataBase,
       TLIc_TXMonRoot
     }; //'c' is for constant
   
   // Trigger Level Type
   enum LevelType 
     {
       TLIc_L1, 
       TLIc_L2, 
       TLIc_L3,
       TLIc_Ln,
       TLIc_NoLevel
     }; //TXMon is set up for 3 + n trigger levels

   enum NotFittingType       
     {
       TLIc_name,       // NOT fit because of name
       TLIc_HiLo,       // NOT fit because not enough high/low points
       TLIc_GoodPoints  // NOT fit because not enough good points
     };
   
   // Used in fit type
   enum UsedFitType
     {
       TLIc_Used,    // Used in the fit      
       TLIc_Five,    // Not used in fit: factor of 5 above/below average
       TLIc_BadRun,  // Run marked as bad
       TLIc_Excluded // explicity Excluded for this Trigger line
     };
   

   enum { TLIc_NumberParams = 10 };       // p0, p1, p2, p3 (Intentionally set higher than official fit)
   enum { TLIc_MaxEntries = 9000 };       // Set this greater than total number of Runs             
   enum { TLIc_LineWidth = 2 };           // width of lines on plots     
   enum { TLIc_NoTime = -1 };             // using time or not 
   enum { TLIc_NumberColors = 14 };       // number of colors used in graphs
   enum { TLIc_NumberStyles = 5 };        // number of styles used in graphs
   enum { TLIc_BadPointsColor = 1 };      // color of bad points
   enum { TLIc_BadPointsStyle = 23 };     // style of bad points 
   enum { TLIc_MaxNumberTrigger = 384 };  // maximum number of Triggers, matches TXMon's max
   
   
   /////////////
   // friends //
   /////////////
   // tells particle data how to print itself out
     friend std::ostream& operator<< (std::ostream& o_stream, const TriggerLineInfo &rhs);



   //////////////////////////
   //            _         //
   // |\/|      |_         //
   // |  |EMBER | UNCTIONS //
   //                      //
   //////////////////////////
   
   /////////////////////////////////
   // Constructors and Destructor //              
   /////////////////////////////////
   TriggerLineInfo();     
   TriggerLineInfo(Int_t RunNumber,
		   Double_t x, 
		   Double_t y,
		   Double_t ex,
		   Double_t ey,
		   const std::string& table,
		   UsedFitType used = TLIc_Used,
		   Int_t time = TLIc_NoTime);


   ////////////////
   // One Liners //
   ////////////////

   //////////////////////
   // Access Functions //
   //////////////////////
   Int_t index() const      { return TLI_index; }
   Double_t x()  const      { return TLI_x; }
   Double_t ex() const      { return TLI_ex; }
   Double_t y()  const      { return TLI_y; }
   Int_t RunNumber()     const { return TLI_RunNumber; }
   Int_t time()          const { return TLI_time; }
   Int_t group()         const { return TLI_group; }
   std::string table() const { return TLI_table; }
   static modeType mode() { return sTLI_mode; }
   static Int_t minRunfit() { return sTLI_minRunfit; }
   static Int_t maxRunfit() { return sTLI_maxRunfit; }
   static Double_t xHi() { return sTLI_xHi; }
   static Double_t xLo() { return sTLI_xLo; }
   static Double_t yHi() { return sTLI_yHi; }
   static Double_t yLo() { return sTLI_yLo; }
   static Int_t NumberHi()           { return sTLI_NumberHi; }             
   static Int_t NumberLo()           { return sTLI_NumberLo; }
   static Int_t NumberOfGoodPoints() { return sTLI_NumberOfGoodPoints; }
   static Int_t NumberOfEntries()    { return sTLI_Vector.size(); }
   static std::string VersionTag()    { return sTLI_VersionTag; }
   static std::string basename()      { return sTLI_basename; }
   static std::string Triggername()   { return sTLI_Triggername; }
   static std::string noVersionname() { return sTLI_noVersionname; }
   static Double_t p0()       { return sTLI_p0; }
   static Double_t p1()       { return sTLI_p1; }
   static Double_t p2()       { return sTLI_p2; }
   static Double_t p3()       { return sTLI_p3; }
   static Double_t pn()       { return sTLI_pn; }  ///the nth paramters
   static Double_t fitError() { return sTLI_fitError; }
   static Int_t NumberRunNumbers()   { return sTLI_RunNumbers.size(); }
   static Int_t RunNumber(Int_t index) { return sTLI_RunNumbers[index]; }
   static Int_t NumberL1Triggers()   { return sTLI_L1TriggerMap.size(); }
   static Int_t NumberL2Triggers()   { return sTLI_L2TriggerMap.size(); }
   static Int_t NumberLnTriggers()   { return sTLI_L3TriggerMap.size(); }
   static Double_t sq(Double_t value)  { return (value * value); }

   Int_t    sumN(Int_t RunNumber)  { return sTLI_ResidualStatMap[RunNumber].sumN(); }
   Double_t sumS(Int_t RunNumber)  { return sTLI_ResidualStatMap[RunNumber].sumS(); }
   Double_t sumS2(Int_t RunNumber) { return sTLI_ResidualStatMap[RunNumber].sumS2(); }
   Int_t    sumBad(Int_t RunNumber){ return sTLI_ResidualStatMap[RunNumber].sumBad(); }


   ///////////////////////
   // Setting Functions //
   /////////////////////// 
   void setIndex(Int_t index) { TLI_index = index; }
   void setX (Double_t x)     { TLI_x = x; }                       
   void setEx(Double_t ex)    { TLI_ex = ex; }
   void setY (Double_t y)     { TLI_y = y; }
   void setEy(Double_t ey)    { TLI_ey = ey; }
   void setRunNumber(Int_t RunNumber) { TLI_RunNumber = RunNumber; }
   void setTime(Int_t time)           { TLI_time = time; }
   void setGroup(Int_t group)         { TLI_group = group; }
   void setUsedInFit(UsedFitType usedInFit) { TLI_usedInFit = usedInFit; }
   void setTable(std::string table)         { TLI_table = table; }
   static void setFitParams (Double_t p0 = 0.,
			     Double_t p1 = 0.,
			     Double_t p2 = 0.,
			     Double_t p3 = 0.,
			     Double_t Error = 1.)  // Sets all four fit parameters. Add 'Double_t pn = 0
     { sTLI_p0 = p0;
       sTLI_p1 = p1;
       sTLI_p2 = p2;
       sTLI_p3 = p3;
       sTLI_fitError = Error; } //add 'stTLI_pn = p0
   static void setFitParamErrors (Double_t e0,
				  Double_t e1,
				  Double_t e2,
				  Double_t e3) //add 'Double_t en'
     { sTLI_e0 = e0;
       sTLI_e1 = e1;
       sTLI_e2 = e2;
       sTLI_e3 = e3; } //add 'sTLI_en = en'
   static void setFitError(Double_t Error)   { sTLI_fitError = Error; }
   static void setMinRun(Int_t minRun) { sTLI_minRun = minRun; }    // set min and max Runs
   static void setMaxRun(Int_t maxRun) { sTLI_maxRun = maxRun; }    // set min and max Runs
   static void setMinRunfit(Int_t minRunfit) { sTLI_minRunfit = minRunfit; }  // minimum for fitting   
   static void setMaxRunfit(Int_t maxRunfit) { sTLI_maxRunfit = maxRunfit; }  // maximum for fitting
   static void setXHi(Double_t xHi) { sTLI_xHi = xHi; }       // set points
   static void setXLo(Double_t xLo) { sTLI_xLo = xLo; }
   static void setYHi(Double_t yHi) { sTLI_yHi = yHi; }        
   static void setYLo(Double_t yLo) { sTLI_yLo = yLo; }
   static void setNumberHi(Int_t NumberHi) { sTLI_NumberHi = NumberHi; }
   static void setNumberLo(Int_t NumberLo) { sTLI_NumberLo = NumberLo; }
   static void setNumberOfGoodPoints(Int_t NumberOfGoodPoints)
     { sTLI_NumberOfGoodPoints = NumberOfGoodPoints; }
   static void setLineIsFit(bool fit)    { sTLI_lineIsFit = fit; }
   static void setVersionTag(std::string VersionTag)           { sTLI_VersionTag = VersionTag; }
   static void setNoVersionname(std::string noVersionname)     { sTLI_noVersionname = noVersionname; }
   static void setDontFit (unsigned int dontFit)             { sTLI_dontFit = dontFit; }
   static void setRunNumberFilename (std::string filename)     { sTLI_RunNumberFilename = filename; }
 

   //////////////////////////////
   // Regular Member Functions //
   //////////////////////////////
   
   void reset();                     // reset all internal variables 
   Double_t residual() const;        // return the residual   
   Double_t residualError() const;   // return the residual Error   
   Double_t fit() const;             // what the fit says at this x value
   bool isUsedInFit() const;         
   std::string usedInFitString() const;      // returns a string describing how this point is used in fit   
   Double_t ey(bool useAve = true) const;    // returns either average y Error or actual y Error
 

   /////////////////////////////
   // Static Member Functions //
   /////////////////////////////

     // call before anything else to set up, except to change name 
     static void setMode(modeType mode);    
     
     // Initializes the Run Numbers
     static void initialize();     
     
     // set all of the physics tables for the Run Numbers        
     static void setPhysicsTables();     
     
     // make the legend
     static void makeLegend();                
     
     // Processes Triggers matching Triggername
     static void ProcessSpecificTriggers(std::string Triggername); 
     
     // Processes database Triggers  
     static void ProcessAllTriggers();                             
     
     // returns filename for Triggers
     static std::string getFilenameForTrigger(Int_t level, Int_t RunNumber); 
     
     //load all points from level 1 Trigger
     static void loadPoints(Int_t RunNumber, std::string filename);   

     // Runs all the routines to graph Trigger 
     static void ProcessTrigger();            
 
     // check to see if this Trigger should be graphed
     static bool checkBeforeFit();        

     // makes graphs and fits   
     static void makeGraphs();            
     // finds the original group Number for data base mode 
     
     static Int_t findGroup(Int_t RunNumber); 
 
     // get the physics table for a Run
     static std::string getTable (Int_t RunNumber);  
     
     //checks Triggername for bad fitting names   
     static bool checkTriggername();  
     
     // reset all static entries
     static void resetStaticEntries();  
     
     // add an entry, default ey => ey = 0.01 * y
     static void addMergedEntry
       (Int_t RunNumber,
	Double_t x,
	Double_t y,
	std::string table = "",
	Double_t ey = -1,
	Double_t ex = 0.,
	Int_t time = TLIc_NoTime);   

     static void addEntry 
       (Int_t RunNumber,
	Double_t x,
	Double_t y,
	std::string table = "",
	Double_t ey = -1,
	Double_t ex = 0.,
	Int_t time = TLIc_NoTime);
 
    // get arrays for a TGraphWithErrors 
     static Int_t getArrays
       (Double_t *xArray,
	Double_t *exArray,
	Double_t *yArray,
	Double_t *eyArray,
	bool returnAllPoints = false);  

     // gets arrays for a TGraph for plotting vs Run Number
     static Int_t getRunArrays 
       (Double_t *xArray,
	Double_t *exArray,
	Double_t *yArray,
	Double_t *eyArray ); 

      //Merge Low luminosity points
     static void MergePoints(Int_t xMin, Int_t xMax);   


     // calculate averages, which lines shouldn't be used, etc.
     static void Calculate();
     
     // return the fit value at a given x
     static Double_t fit(Double_t x);
     
     // base filename
     static std::string baseFilename();
     
     // returns all sorts of information about the fit: error and bad run list
     static void CalculateFitInfo (Double_t &Error, IList &badRunList);
     
     // return vector of legend names
     static SVector legendnames();
     
     // write out fit information
     static void writeFitInfo();
     
     // split a line into parts
     static void Split (SVector &list, std::string line);
     
     // get a single list of integers from a file
     static void getSingleListFromFile (IVector &iList, std::string filename);
     static void getSingleListFromFile (SVector &finalList,std::string filename);
     
     // get a Double_t list of integers from a file
     static void getDoubleListFromFile (IVector &lowerList,IVector &upperList,std::string filename);
     
     //get a list of an Int_t and then three Doubles from a file
     static void getFourListsFromFile 
       (IVector &timeList,
	DVector &crossList,
	DVector &lumiList,
	DVector &ErrorList,
	std::string filename);
     
     // get a list of strings and a Double list of integers from a file   
     static void getThreeListsFromFile 
       (SVector &nameList,
	IVector &lowerList,
	IVector &upperList, 
	std::string filename);    
     
     //get a a list of Int_ts and 2 lists of Doubles 
     static void getThreeListsFromFile 
       (IVector &timeList,
	DVector &crossList,
	DVector &lumiList,
	std::string filename);      
  
     static void setTriggername(std::string name);
     
     static bool getOrigPoint
       (Int_t index,
	Int_t &RunNumber,
	Double_t &x,
	Double_t &y,
	Double_t &ex,
	Double_t &ey,
	Int_t &group,
	Double_t &residual,
	Double_t &residualError);  

     static bool getMergedPoint 
       (Int_t index, 
	Int_t &RunNumber,
	Double_t &x,
	Double_t &y,
	Double_t &ex, 
	Double_t &ey,
	Int_t &group,
	Double_t &residual,
	Double_t &residualError);

     static bool getPoint 
       (Int_t index,
	Int_t &RunNumber, 
	Double_t &x,
	Double_t &y,
	Double_t &ex,
	Double_t &ey, 
	Int_t &group,
	Double_t &residual, 
	Double_t &residualError);

     static bool getPoint 
       (Int_t index,
	Int_t &RunNumber, 
	Double_t &x,
	Double_t &y,
	Double_t &ex,
	Double_t &ey, 
	Int_t &group,
	Double_t &residual, 
	Double_t &residualError,
	Int_t &time);
     
     // Is this a L1, L2, L3 or Ln Trigger?
     static LevelType Level();   
     
     //makes webpage for the stats of the residual
     static void MakeStatisticsWebsite ();   
     
     //deletes a vector of pointers
     static void deletePointers (std::vector< TGraphErrors* > &list);
     static void deletePointers (std::vector< TF1* > &list);
     
     //graphs those plots that don't have fits
     static void noFitPlots ();
     
     //deletes sTLI_legendList and sTLI_leg
     static void memoryCleanUp ();
     
     // returns which level based on the Trigger name
     static LevelType decodeLevelType (std::string name);
     
     // returns name without Version Number modifies 'Version' to contain Version Number
     static std::string StripVersionInfo (std::string name, Int_t &Version);
     
     // Gets the Position of the Trigger name, returns -1 if the Trigger name is NOT found.
     static Int_t getPositionOfname 
       (LevelType level,
	std::string name,
	std::string TXMonRootFile,
	Int_t &Version);
     
     // extracts Run Number from file name (returns -1 if none)
     static Int_t getRunNumberFromname (std::string name);
     
     // Processes one Trigger using TXMon root files and name
     static void ProcessOneTXMonRootTrigger
       (std::string Triggername,
	std::string basename = "", 
	bool fitBadnames = false);
     
     // sets the Run list vector also sets the Run Numbers vector
     static void setRunList (const SVector &Vector);

     // returns an ordered unique list of Run Numbers from a list of filenames
     static IVector getOrderedListFromFilenames(const SVector &Vector);  

     // set a non-default filename 
     static void setNonDefaultFilename(const std::string &name);  
     
     // return the list of all Triggers in a particular root file
     static SVector allTriggerLinesInRootFile (const std::string &filename);   

     // tells class to use the official fit and define 4+1 parameters add 'Double_t pn'
     static void useOfficialFit
       (Double_t p0,
	Double_t p1,
	Double_t p2,
	Double_t p3,
	Double_t fitError); 
     
     
     

 protected:
   /////////////////////////
   // Protected Constants //
   /////////////////////////
   
   ///////////////////////////
   // Protected Member Data //
   ///////////////////////////

   
 private:
   ///////////////////////
   // Private Constants //
   ///////////////////////
   
   /////////////////////////
   // Private Member Data //
   /////////////////////////
   
   Double_t TLI_x;
   Double_t TLI_ex;
   Double_t TLI_y;
   Double_t TLI_ey;
   Int_t TLI_RunNumber;
   Int_t TLI_time;
   Int_t TLI_group;
   Int_t TLI_index; 
   UsedFitType TLI_usedInFit;
   std::string TLI_table;
   
   static modeType sTLI_mode;
   static std::vector<TriggerLineInfo> sTLI_Vector;
   static std::vector<TriggerLineInfo> sTLI_MergedVector;
   static std::vector<TriggerLineInfo> sTLI_OrigVector;
   
   static Double_t sTLI_p0;
   static Double_t sTLI_p1;
   static Double_t sTLI_p2;
   static Double_t sTLI_p3;
   static Double_t sTLI_pn;

   static Double_t sTLI_fitError;
   static Double_t sTLI_e0;
   static Double_t sTLI_e1;
   static Double_t sTLI_e2;
   static Double_t sTLI_e3;
   static Double_t sTLI_en;

   static Double_t sTLI_origMaxy1;
   static Double_t sTLI_origMiny1;

   static Double_t sTLI_orig_p0;
   static Double_t sTLI_orig_p1;
   static Double_t sTLI_orig_p2;
   static Double_t sTLI_orig_p3;
   static Double_t sTLI_orig_pn;

   static Double_t sTLI_orig_fitError;
   static Double_t sTLI_orig_e0;
   static Double_t sTLI_orig_e1;
   static Double_t sTLI_orig_e2;
   static Double_t sTLI_orig_e3;
   static Double_t sTLI_orig_en;

   static Double_t sTLI_minX;
   static Double_t sTLI_maxX;
   static Int_t sTLI_minRun;
   static Int_t sTLI_maxRun;
   static Int_t sTLI_NumberRunRanges;
   static Int_t sTLI_minRunfit;
   static Int_t sTLI_maxRunfit;
   static IVector sTLI_RunNumbers;
   static IVector sTLI_RunRanges;

   static SVector sTLI_Excludenames;
   static IVector sTLI_lowerExclude;
   static IVector sTLI_upperExclude;

   static Double_t sTLI_xHi;
   static Double_t sTLI_xLo;
   static Double_t sTLI_yHi;
   static Double_t sTLI_yLo;
   static Double_t sTLI_yErrorSum;
   static Double_t sTLI_yErrorAvg;

   static Int_t sTLI_NumberHi;
   static Int_t sTLI_NumberLo;
   static Int_t sTLI_NumberOfGoodPoints;

   static std::string sTLI_VersionTag;
   static std::string sTLI_Triggername;
   static std::string sTLI_basename;
   static std::string sTLI_noVersionname;
   static std::string sTLI_RunNumberFilename;

   static IVector sTLI_singleBadList;
   static IVector sTLI_lowerDoubleBadList;
   static IVector sTLI_upperDoubleBadList;
   static bool sTLI_lineIsFit;
   static bool sTLI_fitOriginalPoints;
   static bool sTLI_plotOriginalPoints;

   static unsigned int sTLI_dontFit;
   static std::map< Int_t, ResidualStat > sTLI_ResidualStatMap;
   static std::map< std::string, IVector > sTLI_L1TriggerMap;
   static std::map< std::string, IVector > sTLI_L2TriggerMap;
   static std::map< std::string, IVector > sTLI_L3TriggerMap;
   static std::map< std::string, IVector > sTLI_LnTriggerMap;

   static std::vector< TGraphErrors* > sTLI_pointList, sTLI_legendList;
   static std::vector< TF1* > sTLI_functionList;
   static std::map< Int_t, std::string > sTLI_physicsTables; 
   static Int_t sTLI_Index;
   static TLegend* sTLI_leg;
   static const Int_t sTLI_colorArray[TLIc_NumberColors];
   static const Int_t sTLI_styleArray[TLIc_NumberStyles];
   static bool sTLI_useOtherFilename;
   static std::string sTLI_otherFilename;
   static bool sTLI_rejectZeroOnly;

   // XMon Root fit stuff
   static SVector sTLI_RunListVector;
   static bool sTLI_useOfficialFit;

   static Double_t sTLI_off_p0;
   static Double_t sTLI_off_p1;
   static Double_t sTLI_off_p2;
   static Double_t sTLI_off_p3;
   static Double_t sTLI_off_pn;
   static Double_t sTLI_off_fitError;
   
   // drawing options
   static bool sTLI_useGrid;
   
   
   // private static constants
   static const std::string TLIc_BadSingleFile;
   static const std::string TLIc_BadDoubleFile;
   static const std::string TLIc_RunRangeFile;
   static const std::string TLIc_nameExclude;

 };
 

 

#endif // TriggerLineInfo_H
