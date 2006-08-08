
#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "EventFilter/CSCRawToDigi/interface/CSCCFEBData.h"
#include "EventFilter/CSCRawToDigi/interface/CSCCFEBTimeSlice.h"
#include "EventFilter/CSCRawToDigi/interface/CSCBadCFEBTimeSlice.h"
#include "DataFormats/CSCDigi/interface/CSCStripDigi.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include <cassert>
#include <boost/cstdint.hpp>

CSCCFEBData::CSCCFEBData(unsigned number, unsigned short * buf) 
: theSize(0), boardNumber_(number), theNumberOfSamples(0) {
  // I may be grabbing too many words, but that's OK
  // parse for time slices
  unsigned pos = 0;
  // to be set later
  unsigned maxSamples = 8;
  theSliceStarts.reserve(8);
  while(theNumberOfSamples < maxSamples) {

    // first see if it's a bad slice
    CSCBadCFEBTimeSlice * badSlice
      = reinterpret_cast<CSCBadCFEBTimeSlice *>(buf+pos);
    if(badSlice->check()) {
      //show that a bad slice starts here
      theSliceStarts.push_back(std::pair<int, bool>(pos, false));
      pos += badSlice->sizeInWords();
    } else {
      // OK.  Maybe it's good.

      CSCCFEBTimeSlice * goodSlice 
       = reinterpret_cast<CSCCFEBTimeSlice *>(buf+pos);
      if(goodSlice->check()) {
        // show that a good slice starts here
        theSliceStarts.push_back(std::pair<int, bool>(pos, true));
        // it will just be an array of CSCCFEBTimeSlices, so we'll
        // grab the number of time slices from the first good one
        maxSamples =   goodSlice->sixteenSamples() ? 16 : 8;
        pos += goodSlice->sizeInWords();
      } else {
        edm::LogError ("CSCCFEBData") << "CORRUPT CFEB DATA slice " << theNumberOfSamples << std::hex 
				      << " " << *(buf+pos) << " " << *(buf+pos+1) << " " 
				      << *(buf+pos+2) << std::dec;
        return;
      }
    }
    ++theNumberOfSamples;
  }
  theSize = pos;
  memcpy(theData, buf, theSize*2);
}


CSCCFEBData::CSCCFEBData(unsigned number, bool sixteenSamples) 
: boardNumber_(number), theNumberOfSamples(sixteenSamples ? 16 : 8) {
  theSliceStarts.reserve(theNumberOfSamples);

  // fill the SCA controller words
  CSCCFEBSCAControllerWord scaWord;
  scaWord.ts_flag = sixteenSamples;

  // make a template slice to copy into theData buffer
  CSCCFEBTimeSlice slice;
  slice.setControllerWord(scaWord);

  for(unsigned i = 0; i < theNumberOfSamples; ++i) {
     unsigned short * pos = theData+i*100;
     memcpy(pos, &slice, 200);
     theSliceStarts.push_back(std::pair<int,bool>(i*100, true));
  }
  theSize = theNumberOfSamples*100;
}


const CSCCFEBTimeSlice * CSCCFEBData::timeSlice(unsigned i) const {
  const CSCCFEBTimeSlice * result;
  assert(i < theNumberOfSamples);
  std::pair<int,bool> start = theSliceStarts[i];
  // give a NULL pointer if this is a bad slice
  if(!start.second) {
    result = 0;
  } else {
    result = reinterpret_cast<const CSCCFEBTimeSlice *>(theData+start.first);
  }
  return result;
}


unsigned CSCCFEBData::adcCounts(unsigned layer, unsigned channel, unsigned timeBin) const {
  unsigned result = 0;
  const CSCCFEBTimeSlice * slice = timeSlice(timeBin);
  // zero is returned for bad slices
  if(slice) result = slice->timeSample(layer, channel)->adcCounts;
  return result;
}
unsigned CSCCFEBData::adcOverflow(unsigned layer, unsigned channel, unsigned timeBin) const {
  unsigned result = 0;
  const CSCCFEBTimeSlice * slice = timeSlice(timeBin);
  // zero is returned for bad slices
  if(slice) result = slice->timeSample(layer, channel)->adcOverflow;
  return result;
}
unsigned CSCCFEBData::controllerData(unsigned layer, unsigned channel, unsigned timeBin) const {
  unsigned result = 0;
  const CSCCFEBTimeSlice * slice = timeSlice(timeBin);
  // zero is returned for bad slices
  if(slice) result = slice->timeSample(layer, channel)->controllerData;
  return result;
}

unsigned CSCCFEBData::overlappedSampleFlag(unsigned layer, unsigned channel, unsigned timeBin) const {
  unsigned result = 0;
  const CSCCFEBTimeSlice * slice = timeSlice(timeBin);
  // zero is returned for bad slices
  if(slice) result = slice->timeSample(layer, channel)->overlappedSampleFlag;
  return result;
}
unsigned CSCCFEBData::errorstat(unsigned layer, unsigned channel, unsigned timeBin) const {
  unsigned result = 0;
  const CSCCFEBTimeSlice * slice = timeSlice(timeBin);
  // zero is returned for bad slices
  if(slice) result = slice->timeSample(layer, channel)->errorstat;
  return result;
}

std::vector<CSCStripDigi> CSCCFEBData::digis(unsigned idlayer) const {

  //  assert(layer>0 && layer <= 6);
  std::vector<CSCStripDigi> result;
  result.reserve(16);
  std::vector<int> sca(nTimeSamples());
  std::vector<uint16_t> overflow(nTimeSamples());
  std::vector<uint16_t> contrData(nTimeSamples());
  std::vector<uint16_t> overlap(nTimeSamples());
  std::vector<uint16_t> errorfl(nTimeSamples());

  bool me1a = (CSCDetId::station(idlayer)==1) && (CSCDetId::ring(idlayer)==4);
  unsigned layer = CSCDetId::layer(idlayer);

  for(unsigned ichannel = 1; ichannel <= 16; ++ichannel) {
    if (nTimeSamples()==0) {
      edm::LogError("CSCCFEBData") << "TimeSamples is Zero - CFEB Data Corrupt!";
      break;
    }
    for(unsigned itime = 0; itime < nTimeSamples(); ++itime) {
      sca[itime] = adcCounts(layer, ichannel, itime);
      overflow[itime] = adcOverflow(layer, ichannel, itime);
      contrData[itime] = controllerData(layer, ichannel, itime);
      overlap[itime] = overlappedSampleFlag(layer, ichannel, itime);
      errorfl[itime] = errorstat(layer, ichannel, itime);
    }
    if (sca.size()==0) {
      edm::LogError("CSCCFEBData") << "ADC counts are empty - CFEB Data Corrupt!";
      break;
    }
    int strip = ichannel + 16*boardNumber_;
    if ( me1a ) strip = strip%64; // reset 65-80 to 1-16
    CSCStripDigi digi(strip, sca, overflow, contrData, overlap, errorfl); 
    result.push_back(digi);
  }
  return result;
}


bool CSCCFEBData::check() const {
  bool result = true;
  for(unsigned i = 0; i < theNumberOfSamples; ++i) {
    const CSCCFEBTimeSlice * slice = timeSlice(i);
    if(slice==0 || !timeSlice(i)->check()) result = false;
  }
  return result;
}


std::ostream & operator<<(std::ostream & os, const CSCCFEBData & data) {
  os << "printing CFEB data sample by sample " << std::endl;
  for(unsigned ilayer = 1; ilayer <= 6; ++ilayer) {
    for(unsigned channel = 1; channel <= 16; ++channel) {
      unsigned strip = channel + data.boardNumber_*16;
      os << "Strip " << strip << " ";
      for(unsigned timeBin = 0; timeBin < data.nTimeSamples(); ++timeBin) {
        os << data.adcCounts(ilayer, channel, timeBin) << " " ;
      }
      os << std::endl;
    }
  }
  return os;
}

std::vector < std::vector<CSCStripDigi> > CSCCFEBData::stripDigis() const {
  std::vector < std::vector<CSCStripDigi> > result;
  for (int layer = 1; layer <= 6; ++layer) {
    result.push_back(digis(layer));
  }
  return result;
}

