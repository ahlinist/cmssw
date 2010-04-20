#include <vector>
#include <iostream>
#include <algorithm>
#include <boost/tokenizer.hpp>

// ****************************************************************
class TimingEvent
{
  public:
    float amplitude;
    float time;
    float chi2;
    float sigmaTime;
    float expectedPrecision;

    TimingEvent() :
      amplitude(-1),
      time(-1),
      chi2(-1),
      sigmaTime(-1),
      expectedPrecision(-1)
    {
    }
    
    TimingEvent(float amp,float t,float sigmaT, bool ee) : 
      amplitude(amp), 
      time(t), 
      chi2(-1),
      sigmaTime(sigmaT)
    {
      if(ee)
        expectedPrecision = 33/(amplitude/2.0);
      else
        expectedPrecision = 33/(amplitude/1.2);
    }

};

// ****************************************************************
class CrystalCalibration
{
  public:
    float mean;
    float sigma;
    float totalChi2;
    std::vector<TimingEvent> timingEvents;
    std::vector<TimingEvent>::iterator maxChi2Itr;

    CrystalCalibration() :
      mean(-1),
      sigma(-1),
      totalChi2(-1),
      useWeightedMean(true)
    {
    }
    
    CrystalCalibration(bool weightMean) :
      mean(-1),
      sigma(-1),
      totalChi2(-1),
      useWeightedMean(weightMean)
    {
    }
      
    CrystalCalibration(float m, float s, float tc, std::vector<TimingEvent> te) :
      mean(m),
      sigma(s),
      totalChi2(tc)
    {
      timingEvents = te;
    }

    CrystalCalibration(float m, float s, float tc, std::vector<TimingEvent> te, bool wm) :
      mean(m),
      sigma(s),
      totalChi2(tc),
      useWeightedMean(wm)
    {
      timingEvents = te;
    }

    bool insertEvent(float amp, float t, float sigmaT, bool ee)
    {
      if(sigmaT > 0) // throw away events with zero or negative errors
      {
        timingEvents.push_back(TimingEvent(amp,t,sigmaT,ee));
        updateChi2();
        return true;
      }
      else
        return false;
    }

    bool insertEvent(TimingEvent te)
    {
      if(te.sigmaTime > 0)
      {
        timingEvents.push_back(te);
        updateChi2();
        return true;
      }
      else
        return false;
    }

    int filterOutliers(float threshold = 0.5)
    {
      int numPointsErased = 0;
      while(timingEvents.size() > 4)
      {
        updateChi2();
        float oldMean = mean;
        // Erase largest chi2 event
        TimingEvent toRemove = *maxChi2Itr;
        timingEvents.erase(maxChi2Itr);
        //Calculate new mean/error
        updateChi2();
        //Compare to old mean and break if |(newMean-oldMean)| < newSigma
        //TODO: study acceptance threshold
        if(fabs(mean-oldMean) < threshold*sigma)
        {
          insertEvent(toRemove);
          break;
        }
        else
        {
          numPointsErased++;
        }
      }
      return numPointsErased;
    }

  private:
    bool useWeightedMean;

    void updateChi2() // update individual, total, maxChi2s
    {
      if(useWeightedMean)
        updateChi2Weighted();
      else
        updateChi2Unweighted();
    }

    void updateChi2Weighted()
    {
      updateMeanWeighted();
      float chi2 = 0;
      maxChi2Itr = timingEvents.begin();
      for(std::vector<TimingEvent>::iterator itr = timingEvents.begin();
          itr != timingEvents.end(); ++itr)
      {
        float singleChi = (itr->time-mean)/itr->sigmaTime;
        itr->chi2 = singleChi*singleChi;
        chi2+=singleChi*singleChi;
        if(itr->chi2 > maxChi2Itr->chi2)
          maxChi2Itr = itr;
      }
      totalChi2 = chi2;
    }

    void updateChi2Unweighted()
    {
      updateMeanUnweighted();
      float chi2 = 0;
      maxChi2Itr = timingEvents.begin();
      for(std::vector<TimingEvent>::iterator itr = timingEvents.begin();
          itr != timingEvents.end(); ++itr)
      {
        float singleChi = (itr->time-mean);
        itr->chi2 = singleChi*singleChi;
        chi2+=singleChi*singleChi;
        if(itr->chi2 > maxChi2Itr->chi2)
          maxChi2Itr = itr;
      }
      totalChi2 = chi2;
    }

    void updateMeanWeighted()
    {
      float meanTmp = 0;
      float sigmaTmp = 0;
      for(std::vector<TimingEvent>::const_iterator itr = timingEvents.begin();
          itr != timingEvents.end(); ++itr)
      {
        float sigmaT2 = itr->sigmaTime;
        sigmaT2*=sigmaT2;
        meanTmp+=(itr->time)/(sigmaT2);
        sigmaTmp+=1/(sigmaT2);
      }
      mean = meanTmp/sigmaTmp;
      sigma = sqrt(1/sigmaTmp);
    }

    void updateMeanUnweighted()
    {
      float meanTmp = 0;
      for(std::vector<TimingEvent>::const_iterator itr = timingEvents.begin();
          itr != timingEvents.end(); ++itr)
      {
        meanTmp+=itr->time;
      }
      mean = meanTmp/timingEvents.size();
      float sigmaTmp = 0;
      for(std::vector<TimingEvent>::const_iterator itr = timingEvents.begin();
          itr != timingEvents.end(); ++itr)
      {
        sigmaTmp+=(itr->time-mean)*(itr->time-mean);
      }
      sigma = sqrt(sigmaTmp/timingEvents.size());
    }
    
};


// ****************************************************************
// Data members (globals)

struct TTreeMembers {
  int numEBcrys_;
  int numEEcrys_;
  int cryHashesEB_[61200];
  int cryHashesEE_[14648];
  float cryTimesEB_[61200];
  float cryTimesEE_[14648];
  float cryUTimesEB_[61200];
  float cryUTimesEE_[14648];
  float cryTimeErrorsEB_[61200];
  float cryTimeErrorsEE_[14648];
  float cryAmpsEB_[61200];
  float cryAmpsEE_[14648];
  float cryETEB_[61200];
  float cryETEE_[14648];
  float e1Oe9EB_[61200];
  float kswisskEB_[61200];
  int numTriggers_;
  int numTechTriggers_;
  int triggers_[200];
  int techtriggers_[200];
  float absTime_;
  int lumiSection_;
  int bx_;
  int orbit_; 
  float correctionToSample5EB_;
  float correctionToSample5EEP_;
  float correctionToSample5EEM_;
} TTreeMembers_;


void setBranchAddresses(TTree* myTree, TTreeMembers& treeVars)
{
  myTree->SetBranchAddress("numberOfEBcrys",&treeVars.numEBcrys_);
  myTree->SetBranchAddress("numberOfEEcrys",&treeVars.numEEcrys_);
  myTree->SetBranchAddress("crystalHashedIndicesEB",treeVars.cryHashesEB_);
  myTree->SetBranchAddress("crystalHashedIndicesEE",treeVars.cryHashesEE_);
  myTree->SetBranchAddress("crystalTimesEB",treeVars.cryTimesEB_);
  myTree->SetBranchAddress("crystalTimesEE",treeVars.cryTimesEE_);
  myTree->SetBranchAddress("crystalUncalibTimesEB",treeVars.cryUTimesEB_);
  myTree->SetBranchAddress("crystalUncalibTimesEE",treeVars.cryUTimesEE_);
  myTree->SetBranchAddress("crystalTimeErrorsEB",treeVars.cryTimeErrorsEB_);
  myTree->SetBranchAddress("crystalTimeErrorsEE",treeVars.cryTimeErrorsEE_);
  myTree->SetBranchAddress("crystalAmplitudesEB",treeVars.cryAmpsEB_);
  myTree->SetBranchAddress("crystalAmplitudesEE",treeVars.cryAmpsEE_);
  myTree->SetBranchAddress("crystalETEB",treeVars.cryETEB_);
  myTree->SetBranchAddress("crystalETEE",treeVars.cryETEE_);
  myTree->SetBranchAddress("e1Oe9EB",treeVars.e1Oe9EB_);
  myTree->SetBranchAddress("kswisskEB",treeVars.kswisskEB_);
  myTree->SetBranchAddress("numTriggers",&treeVars.numTriggers_);
  myTree->SetBranchAddress("triggers",&(treeVars.triggers_));
  myTree->SetBranchAddress("numTechTriggers",&treeVars.numTechTriggers_);
  myTree->SetBranchAddress("techtriggers",treeVars.techtriggers_);
  myTree->SetBranchAddress("absTime",&treeVars.absTime_);
  myTree->SetBranchAddress("lumiSection",&treeVars.lumiSection_);
  myTree->SetBranchAddress("bx",&treeVars.bx_);
  myTree->SetBranchAddress("orbit",&treeVars.orbit_);
  myTree->SetBranchAddress("correctionToSampleEB",&treeVars.correctionToSample5EB_);
  myTree->SetBranchAddress("correctionToSampleEEM",&treeVars.correctionToSample5EEM_);
  myTree->SetBranchAddress("correctionToSampleEEP",&treeVars.correctionToSample5EEP_);
}


//
std::vector<std::string> split(std::string msg, std::string separator)
{
  boost::char_separator<char> sep(separator.c_str());
  boost::tokenizer<boost::char_separator<char> > tok(msg, sep );
  std::vector<std::string> token ;
  for ( boost::tokenizer<boost::char_separator<char> >::const_iterator i = tok.begin(); i != tok.end(); ++i ) {
    token.push_back(std::string(*i)) ;
  }
  return token ;
}


//
void genIncludeExcludeVectors(std::string optionString,
    std::vector<std::vector<double> >& includeVector,
    std::vector<std::vector<double> >& excludeVector)
{
  std::vector<std::string> rangeStringVector;
  std::vector<double> rangeIntVector;

  if(optionString != "-1"){
    std::vector<std::string> stringVector = split(optionString,",") ;

    for (uint i=0 ; i<stringVector.size() ; i++) {
      bool exclude = false;

      if(stringVector[i].at(0)=='x'){
        exclude = true;
        stringVector[i].erase(0,1);
      }
      rangeStringVector = split(stringVector[i],"-") ;

      rangeIntVector.clear();
      for(uint j=0; j<rangeStringVector.size();j++) {
        rangeIntVector.push_back(atof(rangeStringVector[j].c_str()));
      }
      if(exclude) excludeVector.push_back(rangeIntVector);
      else includeVector.push_back(rangeIntVector);

    }
  }
}


//
bool includeEvent(double eventParameter,
    std::vector<std::vector<double> > includeVector,
    std::vector<std::vector<double> > excludeVector)
{
  bool keepEvent = false;
  if(includeVector.size()==0) keepEvent = true;
  for(uint i=0; i!=includeVector.size();++i){
    if(includeVector[i].size()==1 && eventParameter==includeVector[i][0])
      keepEvent=true;
    else if(includeVector[i].size()==2 && (eventParameter>=includeVector[i][0] && eventParameter<=includeVector[i][1]))
      keepEvent=true;
  }
  if(!keepEvent) // if it's not in our include list, skip it
    return false;

  keepEvent = true;
  for(uint i=0; i!=excludeVector.size();++i){
    if(excludeVector[i].size()==1 && eventParameter==excludeVector[i][0])
      keepEvent=false;
    else if(excludeVector[i].size()==2 && (eventParameter>=excludeVector[i][0] && eventParameter<=excludeVector[i][1]))
      keepEvent=false;
  }

  return keepEvent; // if someone includes and excludes, exclusion will overrule

}


//
bool includeEvent(int* triggers,
    int numTriggers,
    std::vector<std::vector<double> > includeVector,
    std::vector<std::vector<double> > excludeVector)
{
  bool keepEvent = false;
  if(includeVector.size()==0) keepEvent = true;
  for (int ti = 0; ti < numTriggers; ++ti) {
    for(uint i=0; i!=includeVector.size();++i){
      if(includeVector[i].size()==1 && triggers[ti]==includeVector[i][0]) keepEvent=true;
      else if(includeVector[i].size()==2 && (triggers[ti]>=includeVector[i][0] && triggers[ti]<=includeVector[i][1])) keepEvent=true;
    }
  }
  if(!keepEvent)
    return false;

  keepEvent = true;
  for (int ti = 0; ti < numTriggers; ++ti) {
    for(uint i=0; i!=excludeVector.size();++i){
      if(excludeVector[i].size()==1 && triggers[ti]==excludeVector[i][0]) keepEvent=false;
      else if(excludeVector[i].size()==2 && (triggers[ti]>=excludeVector[i][0] && triggers[ti]<=excludeVector[i][1])) keepEvent=false;
    }
  }

  return keepEvent;
}
