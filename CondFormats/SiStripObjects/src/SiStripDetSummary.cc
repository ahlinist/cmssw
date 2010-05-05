#include "CondFormats/SiStripObjects/interface/SiStripDetSummary.h"

void SiStripDetSummary::add(const DetId & detid, const float & value)
{
  int layer = 0;
  int stereo = 0;
  int detNum = 0;

  // Using the operator[] if the element does not exist it is created with the default value. That is 0 for integral types.
  switch (detid.subdetId()) {
  case StripSubdetector::TIB:
    {
      TIBDetId theTIBDetId(detid.rawId());
      layer = theTIBDetId.layer();
      stereo = theTIBDetId.stereo();
      detNum = 1000;
      break;
    }
  case StripSubdetector::TOB:
    {
      TOBDetId theTOBDetId(detid.rawId());
      layer = theTOBDetId.layer();
      stereo = theTOBDetId.stereo();
      detNum = 2000;
      break;
    }
  case StripSubdetector::TEC:
    {
      TECDetId theTECDetId(detid.rawId());
      // is this module in TEC+ or TEC-?
      layer = theTECDetId.wheel();
      stereo = theTECDetId.stereo();
      detNum = 3000;
      break;
    }
  case StripSubdetector::TID:
    {
      TIDDetId theTIDDetId(detid.rawId());
      // is this module in TID+ or TID-?
      layer = theTIDDetId.wheel();
      stereo = theTIDDetId.stereo();
      detNum = 4000;
      break;
    }
  }
  detNum += layer*10 + stereo;
  // string name( detector + boost::lexical_cast<string>(layer) + boost::lexical_cast<string>(stereo) );
  valueMap_[detNum].mean += value;
  valueMap_[detNum].rms += value*value;
  valueMap_[detNum].count += 1;
}

void SiStripDetSummary::print(std::stringstream & ss, const bool mean) const
{
  // Compute the mean for each detector and for each layer.
  std::map<unsigned int, Values>::const_iterator valueMapIt = valueMap_.begin();

  ss << "subDet" << std::setw(15) << "layer" << std::setw(16) << "mono/stereo" << std::setw(20);
  if( mean ) ss << "mean +- rms" << std::endl;
  else ss << "count" << std::endl;

  std::string detector;
  std::string oldDetector;

  for( ; valueMapIt != valueMap_.end(); ++valueMapIt ) {
    int count = valueMapIt->second.count;
    double mean = 0.;
    double rms = 0.;
    if( computeMean_ && count != 0 ) {
      mean = (valueMapIt->second.mean)/count;
      rms = (valueMapIt->second.rms)/count - mean*mean;
      if (rms <= 0)
	rms = 0;
      else
	rms = sqrt(rms);
    }
    // Detector type
    switch ((valueMapIt->first)/1000) {
    case 1:
      detector = "TIB ";
      break;
    case 2:
      detector = "TOB ";
      break;
    case 3:
      detector = "TEC ";
      break;
    case 4:
      detector = "TID ";
      break;
    }
    if( detector != oldDetector ) {
      ss << std::endl << detector;
      oldDetector = detector;
    }
    else ss << "    ";
    // Layer number
    int layer = (valueMapIt->first)/10 - (valueMapIt->first)/1000*100;
    int stereo = valueMapIt->first - layer*10 -(valueMapIt->first)/1000*1000;

    ss << std::setw(15) << layer << std::setw(13) << stereo << std::setw(18);
    if( computeMean_ ) ss << mean << " +- " << rms << std::endl;
    else ss << count << std::endl;
  }
}
