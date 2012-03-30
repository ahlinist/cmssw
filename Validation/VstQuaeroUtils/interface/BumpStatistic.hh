/************************************************
The BumpStatistic class looks for mass bumps
Georgios Choudalakis, September 2006
************************************************/

#include <vector>
#include <string>
#include <limits>

class BumpStatistic
{
public:
  BumpStatistic();
  BumpStatistic(const std::string& _finalState, const std::string& _massVariableName, const std::vector<double>& _bkgpts, const std::vector<double>& _bkgwt, const std::vector<double>& _datapts, int _dataOrPseudoDataMode=1, int pseudoDataToProduce=-1); 
  void findBumps();
  double probability(double pvalmin = -1); //-1 will mean the pvalue of the most interesting bump found.
  std::vector<double> getData();
  std::pair<double, double> getLimits();
  double getPvalmin();
  std::string reportPvalOfAllBumps();
  std::string reportContentsOfAllBumps();
  void setBoundaries(double lower, double upper=std::numeric_limits<double>::max());

  //sensitivity section:
  std::string probeSensitivityAlongBackground(const double& targetSigma) ;
  std::string probeApproximateSensitivityAlongBackground(const double& targetSigma) ;
  int signalNeededForDiscovery(const double& targetSigma, const double& signalMass);
  int approximateSignalNeededForDiscovery(const double& targetSigma, const double& signalMass);

  
  //justData section:
  void findBumpsJustData();
  std::pair<double, double> getLimitsJustData();
  double getPvalminJustData();
  std::string reportPvalOfAllBumpsJustData();
  
private:
  bool debug;
  bool minimalDebug;
  bool debug_store; //in case we want to temporarily turn debug off and then know whether to turn it on or not.
  int dataOrPseudoDataMode;
  
  std::vector<double> bkgpts;
  std::vector<double> bkgwt;
  std::vector<double> datapts;

  double lowestMassToConsider; //default will be 0, but may change
  double highestMassToConsider;//default will be infinity.

  std::string finalState;
  std::string massVariable;

  double minBinSize;
  double sidebandsSize;
  double positionStep;
  double widthStep;
  bool useWidthOptimization;

  double minDataInWindow;
  double bumpynessConstantThreshold;
  bool weRequireObjectivelyGoodSidebands; // will be used when deciding quality criteria for bump candidates.
  double Neff; // probability = 1 - (1-pvalmin)^Neff

  struct BumpCandidate {
    BumpCandidate() { windowSize=0; }
    double windowCenter, windowStart, windowSize, windowEnd;
    double leftBandStart, rightBandEnd;
    double data, dataLeft, dataRight;
    double bkg, bkgLeft, bkgRight;
    double deltaBkg, deltaBkgLeft, deltaBkgRight;
    double dataSidebands;
    double bkgSidebands, deltaBkgSidebands;

    double pval;
    bool discrepantSidebands;
    bool windowDataMoreThanExtrapolatedSidebands;
    bool qualifies;
  };

public:  //need to make it public to be able to sort it with a custom sort criterion
  struct BumpCandidateJustData {
    double windowCenter, windowStart, windowSize, windowEnd;
    double leftBandStart, rightBandEnd;
    double dataLeft, dataRight;
    double leftBandSeparation, rightBandSeparation; //position that defines left and right half within a sideband.
    double dataLeftLeftHalf, dataLeftRightHalf; //data in left and right half of left sideband.
    double dataRightLeftHalf, dataRightRightHalf; //data in left and right half of right sideband.
    double averagePositionLeftLeft, averagePositionLeftRight, averagePositionRightLeft, averagePositionRightRight;
    double averagePositionLeft, averagePositionRight;
    double data;
    double dataExpected_oneLineInterpolation, deltaDataExpected_oneLineInterpolation,pvalue_oneLineInterpolation;
    double dataExpected_twoLineInterpolation, deltaDataExpected_twoLineInterpolation,pvalue_twoLineInterpolation;
    double dataExpected, deltaDataExpected, pval;
    bool qualifies;
  };

private:
  bool scannedForBumpsAlready;
  std::vector<BumpCandidate> allBumps;
  BumpCandidate mostInterestingBump;

  bool scannedForJustDataAlready;
  std::vector<BumpCandidateJustData> allBumpsJustData;
  BumpCandidateJustData mostInterestingBumpJustData;

  //save time by not reading the background 100s of times to produce 100s of pseudo-data sets.
  bool pseudoDataProductionHasScannedBkgAlready;
  double sum_bkg; 
  std::vector<double> bkgBins;
  std::vector<double> cumulativeBkgBins;
  double biggestBkgBinContent; 
  double smallestBkgPosition;
  double biggestBkgPosition;


  void sortBackgroundVectors() ;
  std::pair<double,double> fromWhereToWhereToScan(double smallestPosition, double biggestPosition);
  std::pair<double,double> fromHowToHowWide(const double& position);
  void optimizeWidth(BumpCandidate& bc, const std::vector<double>& _bkgpts, const std::vector<double>& _bkgwt);
  double countEntries(const std::vector<double>& points, const std::vector<double>& weights, const double& windowStart, const double& windowEnd);
  std::pair<double,double> countBkgAndDeltaBkg(const std::vector<double>& points, const std::vector<double>& weights, const double& windowStart, const double& windowEnd);
  double spikeWeightInRegion(const std::vector<double>& points, const std::vector<double>& weights, const double& windowStart, const double& windowEnd);
  void readWindowAndSidebands(const std::vector<double>& points, const double& leftBandStart, const double& windowStart, const double& windowEnd, const double& rightBandEnd, double& totalLeft, double& total, double& totalRight);
  void readWindowAndSidebands(const std::vector<double>& points, const std::vector<double>& weights, const double& leftBandStart, const double& windowStart, const double& windowEnd, const double& rightBandEnd, double& totalInWindow, double& deltaInWindow, double& totalLeft, double& deltaLeft, double& totalRight, double& deltaRight);
  void readWindowCentral(const std::vector<double>& points, const std::vector<double>& weights, const double& windowStart, const double& windowEnd, double& totalInWindow, double& deltaInWindow);
  double probQualityIsMet(BumpCandidate bc, int maxTrials = 1000);
  void checkQualityCriteria(BumpCandidate& bc, bool useAccuratePvalue=true);
  std::vector<BumpCandidate> getAllBumps(const std::vector<double>& _bkgpts, const std::vector<double>& _bkgwt, const std::vector<double>& _datapts, const double& lowLimit=-1, const double& highLimit=-1);
  BumpCandidate findMostInterestingBump(const std::vector<BumpCandidate>& allCandidates);
  bool isThereAnyMoreInterestingBumpQ(const std::vector<double>& _bkgpts, const std::vector<double>& _bkgwt, const std::vector<double>& _datapts, const double& targetPvalmin) ;
  std::vector<double> producePseudoDataTrialAndError(const std::vector<double>& bkgpts, const std::vector<double>& bkgwt, long int numOfData = -1);
  std::vector<double> producePseudoData(const std::vector<double>& bkgpts, const std::vector<double>& bkgwt, long int numOfData = -1);
  void   addPseudoSignal(std::vector<double>& _datapts, int numberOfPseudoSignal, double mean, double sigma=0);
  int signalNeededForDiscovery(const double& targetSigma, const double& signalMass, const std::vector<double>& dataBasis);
  double probabilityDirectEstimation(const double& pvalmin) ;
  double probabilityWithPseudoData(const double& targetPvalmin, double minutesAllowed=5.0) ;
  double massResolution(double mass);


  std::vector<BumpCandidateJustData> getAllBumpsJustData(const std::vector<double>& _datapts, const double& lowLimit=-1, const double& highLimit=-1);
  void readWindowJustData(const std::vector<double>& points, BumpCandidateJustData& bc);  
  void interpolateDataFromSidebands(BumpCandidateJustData& bc);  
  void checkQualityCriteriaJustData(BumpCandidateJustData& bc);
  std::vector<double> trimBumpsFromData(const std::vector<double>& _datapts, double pvalue);
  BumpCandidateJustData findMostInterestingBump(const std::vector<BumpCandidateJustData>& allCandidates);
};


namespace Bump {
 bool searchForBumpsHereOrNot(std::string variableName);
}
