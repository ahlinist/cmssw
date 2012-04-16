#include "TauAnalysis/Skimming/plugins/RunLumiSectionEventNumberFilter.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <TPRegexp.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <TString.h>

#include <iostream>
#include <fstream>

const int noMatchRequired = -1;

RunLumiSectionEventNumberFilter::RunLumiSectionEventNumberFilter(const edm::ParameterSet& cfg)
  : cfgError_(0)
{
  //std::cout << "<RunLumiSectionEventNumberFilter::RunLumiSectionEventNumberFilter>:" << std::endl;

  runLumiSectionEventNumberFileName_ = cfg.getParameter<std::string>("runLumiSectionEventNumberFileName");

  separator_ = cfg.exists("separator") ? 
    cfg.getParameter<std::string>("separator") : "[[:space:]]+";
  //std::cout << " separator = '" << separator_ << "'" << std::endl;

  numEventsProcessed_ = 0;
  numEventsSelected_ = 0;
}

RunLumiSectionEventNumberFilter::~RunLumiSectionEventNumberFilter()
{
  std::string matchRemark = ( numEventsSelected_ == numEventsToBeSelected_ ) ? "matches" : "does NOT match";
  edm::LogInfo ("~RunLumiSectionEventNumberFilter") 
    << " Number of Events processed = " << numEventsProcessed_ << std::endl
    << " Number of Events selected = " << numEventsSelected_ << ","
    << " " << matchRemark << " Number of Events to be selected = " << numEventsToBeSelected_ << ".";
 
//--- check for events specified by run + event number in ASCII file
//    and not found in EDM input .root file
  int numRunLumiSectionEventNumbersUnmatched = 0;
  for ( std::map<edm::RunNumber_t, matchedLumiSectionEventNumberMap>::const_iterator run = matchedRunLumiSectionEventNumbers_.begin();
	run != matchedRunLumiSectionEventNumbers_.end(); ++run ) {
    for ( matchedLumiSectionEventNumberMap::const_iterator lumiSection = run->second.begin();
	  lumiSection != run->second.end(); ++lumiSection ) {
      for ( matchedEventNumbersMap::const_iterator event = lumiSection->second.begin();
	    event != lumiSection->second.end(); ++event ) {
	if ( event->second < 1 ) {
	  if ( numRunLumiSectionEventNumbersUnmatched == 0 ) std::cout << "Events not found in PoolInputSource:" << std::endl;
	  std::cout << " Run# = " << run->first << ", Luminosity Section# " << lumiSection->first << ","
		    << " Event# " << event->first << std::endl;
	  ++numRunLumiSectionEventNumbersUnmatched;
	}
      }
    }
  }

  if ( numRunLumiSectionEventNumbersUnmatched > 0 ) 
    std::cout << "--> Number of unmatched Events = " << numRunLumiSectionEventNumbersUnmatched << std::endl;

//--- check for events specified by run + event number in ASCII file
//    and found more than once in EDM input .root file
  int numRunLumiSectionEventNumbersAmbiguousMatch = 0;
  for ( std::map<edm::RunNumber_t, matchedLumiSectionEventNumberMap>::const_iterator run = matchedRunLumiSectionEventNumbers_.begin();
	run != matchedRunLumiSectionEventNumbers_.end(); ++run ) {
    for ( matchedLumiSectionEventNumberMap::const_iterator lumiSection = run->second.begin();
	  lumiSection != run->second.end(); ++lumiSection ) {
      for ( matchedEventNumbersMap::const_iterator event = lumiSection->second.begin();
	    event != lumiSection->second.end(); ++event ) {
	if ( event->second > 1 ) {
	  if ( numRunLumiSectionEventNumbersAmbiguousMatch == 0 ) 
	    std::cout << "Events found in PoolInputSource more than once:" << std::endl;
	  std::cout << " Run# = " << run->first << ", Luminosity Section# " << lumiSection->first << ","
		    << " Event# " << event->first << std::endl;
	  ++numRunLumiSectionEventNumbersAmbiguousMatch;
	}
      }
    }
  }
  
  if ( numRunLumiSectionEventNumbersAmbiguousMatch > 0 ) 
    std::cout << "--> Number of ambiguously matched Events = " << numRunLumiSectionEventNumbersAmbiguousMatch << std::endl;
}

void RunLumiSectionEventNumberFilter::beginJob()
{
  if ( runLumiSectionEventNumberFileName_ != "" ) {
    readRunLumiSectionEventNumberFile();
  } else {
    edm::LogError ("RunLumiSectionEventNumberFilter") 
      << " Configuration Parameter runLumiSectionEventNumberFileName = " << runLumiSectionEventNumberFileName_ 
      << " invalid --> no Events will be selected !!";
    cfgError_ = 1;
  }
}

void RunLumiSectionEventNumberFilter::readRunLumiSectionEventNumberFile()
{
//--- read run + luminosity section + event number pairs from ASCII file

  //std::cout << "<RunLumiSectionEventNumberFilter::readRunLumiSectionEventNumberFile>:" << std::endl;

  std::string regexpParser_threeColumnLine_string = std::string("[[:digit:]]+");
  regexpParser_threeColumnLine_string.append(separator_).append("[[:digit:]]+").append(separator_).append("[[:digit:]]+\\s*");
  //std::cout << " regexpParser_threeColumnLine_string = " << regexpParser_threeColumnLine_string << std::endl;
  TPRegexp regexpParser_threeColumnLine(regexpParser_threeColumnLine_string.data());
  std::string regexpParser_threeColumnNumber_string = std::string("([[:digit:]]+)");
  regexpParser_threeColumnNumber_string.append(separator_).append("([[:digit:]]+)").append(separator_).append("([[:digit:]]+)\\s*");
  //std::cout << " regexpParser_threeColumnNumber_string = " << regexpParser_threeColumnNumber_string << std::endl;
  TPRegexp regexpParser_threeColumnNumber(regexpParser_threeColumnNumber_string.data());

  ifstream runLumiSectionEventNumberFile(runLumiSectionEventNumberFileName_.data());
  int iLine = 0;
  numEventsToBeSelected_ = 0;
  while ( !(runLumiSectionEventNumberFile.eof() || runLumiSectionEventNumberFile.bad()) ) {
    std::string line;
    getline(runLumiSectionEventNumberFile, line);
    ++iLine;

//--- skip empty lines
    if ( line == "" ) continue;

    //std::cout << " line = '" << line << "'" << std::endl;

    bool parseError = false;

    TString line_tstring = line.data();
//--- check if line matches three column format;
//    in which case require four matches (first match refers to entire line)
//    and match individually run, event and luminosity section numbers
    if ( regexpParser_threeColumnLine.Match(line_tstring) == 1 ) {
      TObjArray* subStrings = regexpParser_threeColumnNumber.MatchS(line_tstring);
      if ( subStrings->GetEntries() == 4 ) {
	//std::cout << " runNumber_string = " << ((TObjString*)subStrings->At(1))->GetString() << std::endl;
	edm::RunNumber_t runNumber = ((TObjString*)subStrings->At(1))->GetString().Atoll();
	//std::cout << " lumiSectionNumber_string = " << ((TObjString*)subStrings->At(2))->GetString() << std::endl;
	edm::LuminosityBlockNumber_t lumiSectionNumber = ((TObjString*)subStrings->At(2))->GetString().Atoll();
	//std::cout << " eventNumber_string = " << ((TObjString*)subStrings->At(3))->GetString() << std::endl;
	edm::EventNumber_t eventNumber = ((TObjString*)subStrings->At(3))->GetString().Atoll();

	std::cout << "--> adding Run# = " << runNumber << ", Luminosity Section# " << lumiSectionNumber << ","
		  << " Event# " << eventNumber << std::endl;

	runLumiSectionEventNumbers_[runNumber][lumiSectionNumber].insert(eventNumber);
	matchedRunLumiSectionEventNumbers_[runNumber][lumiSectionNumber][eventNumber] = 0;
	++numEventsToBeSelected_;
      } else {
	parseError = true;
      }
      
      delete subStrings;
    } else {
      parseError = true;
    }

    if ( parseError ) {
      edm::LogError ("readRunLumiSectionEventNumberFile") 
	<< " Error in parsing Line " << iLine << " = '" << line << "'"
	<< " of File = " << runLumiSectionEventNumberFileName_ << " !!"; 
      cfgError_ = 1;
    }
  }

  if ( numEventsToBeSelected_ == 0 ) {
    edm::LogError ("readRunLumiSectionEventNumberFile") 
      << " Failed to read any run + luminosity-section + event Number Triplets from File = " << runLumiSectionEventNumberFileName_ 
      << " --> no Events will be selected !!";
    cfgError_ = 1;
  }
}

bool RunLumiSectionEventNumberFilter::filter(edm::Event& evt, const edm::EventSetup& es)
{
//--- check that configuration parameters contain no errors
  if ( cfgError_ ) {
    edm::LogError ("filter") << " Error in Configuration ParameterSet --> skipping !!";
    return false;
  }

//--- retrieve run and event numbers from the event
  edm::RunNumber_t runNumber = evt.id().run();
  edm::LuminosityBlockNumber_t lumiSectionNumber = evt.luminosityBlock();
  edm::EventNumber_t eventNumber = evt.id().event();

//--- check if run number matches any of the runs containing events to be selected
  bool isSelected = false;
  if ( runLumiSectionEventNumbers_.find(runNumber) != runLumiSectionEventNumbers_.end() ) {
    const lumiSectionEventNumberMap& lumiSectionEventNumbers = runLumiSectionEventNumbers_.find(runNumber)->second;

    if ( lumiSectionEventNumbers.find(lumiSectionNumber) != lumiSectionEventNumbers.end() ) {
      const eventNumberSet& eventNumbers = lumiSectionEventNumbers.find(lumiSectionNumber)->second;

      if ( eventNumbers.find(eventNumber) != eventNumbers.end() ) isSelected = true;
    }
  }

  ++numEventsProcessed_;
  if ( isSelected ) {
    edm::LuminosityBlockNumber_t lumiSectionNumber = evt.luminosityBlock();
    edm::LogInfo ("filter") 
      << "copying Run# " << runNumber << ", Luminosity Section# " << lumiSectionNumber << ","
      << " Event# " << eventNumber << ".";
    ++matchedRunLumiSectionEventNumbers_[runNumber][lumiSectionNumber][eventNumber];
    ++numEventsSelected_;
    return true;
  } else {
    return false;
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(RunLumiSectionEventNumberFilter);

