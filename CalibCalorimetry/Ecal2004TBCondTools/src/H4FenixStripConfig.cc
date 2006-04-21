// -*- C++ -*-
// 
// H4FenixStripConfig.C
// 
/**
  \class H4FenixStripConfig 
  \brief hold the configuration of one single Fenix Strip Chip.

  last modified: $Date: 2003/09/24 09:12:54 $
  by:            $Author: nrl $

*/
#include <cmath>
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4FenixStripConfig.h"

using namespace std;

ClassImp(H4FenixStripConfig);


H4FenixStripConfig::H4FenixStripConfig()
  : nchannels_(0), mode_(0),  // FIXME: Barrel
    fgvb5Thresh_(0), fgvb5Lut_(0), formatterShift_(0)
{
  setNChannels(4);
  setDefaultFPPAConfig();
}


H4FenixStripConfig::~H4FenixStripConfig()
{
}


/*!
  Set the number of channels for each of the five linearizer. 
  The number of channels must be equal to the number 
  of channels implemented by the preamplifier chips mounted on the 
  VFE. 
  
  \param nchannels: number of channels
  
 */
void  H4FenixStripConfig::setNChannels(int nchannels)
{
#ifdef H4_BOUNDS_CHECK
  if(nchannels!=3 && nchannels!=4) {
    std::cout << "H4FenixStripConfig::setNChannels(nchannels=" 
	      << nchannels << ") ERROR, arg out of range!"
	      << std::endl;
    return;
  }
#endif
  nchannels_ = nchannels;
  // size: 5 linearizers \times nchannels_ channels 
  // ordering: ped(lin_i, channel_j) = pedestals_[5*lin_i+channel_j]
  pedestals_.resize(5*nchannels_);
  mantissa_.resize(5*nchannels_);
  exponent_.resize(5*nchannels_);
}


/*!
  Set the operation mode. Can be 0 (barrel), or 1 (end-cap). 
  
  \param mode: operation mode (0|1)
 */
void H4FenixStripConfig::setMode(int mode)
{
#ifdef H4_BOUNDS_CHECK
  if(mode!=H4TPGFEConfig::Barrel && mode!=H4TPGFEConfig::EndCap) {
    std::cout << "H4FenixStripConfig::setMode(mode=" 
	      << mode << ") ERROR, arg out of range!"
	      << std::endl;
    return;
  }
#endif
  mode_ = mode;
}


double  H4FenixStripConfig::getCalibrationCoefficient(int ilin, int ichannel) const
{
#ifdef H4_BOUNDS_CHECK
  if(ichannel<0 || ichannel>=nchannels_ || 
     ilin<0     || ilin>=5) {
    std::cout << "H4FenixStripConfig::getCalibrationCoefficient(ilin=" << ilin
	      << ", ichannel=" << ichannel
	      << ") ERROR, args out of range" << std::endl;
    return 0;
  }
#endif
  return mantissa_[5*ilin+ichannel] * pow(2.,-exponent_[5*ilin+ichannel]);
}


void H4FenixStripConfig::setCalibrationCoefficient(int ilin, int ichannel, double cal)
{
#ifdef H4_BOUNDS_CHECK
  if(ichannel<0 || ichannel>=nchannels_ || 
     ilin<0     || ilin>=5) {
    std::cout << "H4FenixStripConfig::setCalibrationCoefficient(ichannel=" << ichannel
	      << ") ERROR, args out of range" << std::endl;
    return;
  }
#endif
  std::cout << "H4FenixStripConfig::setCalibrationCoefficient: not implemented yet!" 
	    << std::endl;
}


int  H4FenixStripConfig::getPedestal(int ilin, int ichannel) const
{
#ifdef H4_BOUNDS_CHECK
  if(ichannel<0 || ichannel>=nchannels_ || 
     ilin<0     || ilin>=5) {
    std::cout << "H4FenixStripConfig::getPedestal(ilin=" << ilin
	      << ", ichannel=" << ichannel
	      << ") ERROR, args out of range" << std::endl;
    return 0;
  }
#endif
  return pedestals_[5*ilin+ichannel];
}


void H4FenixStripConfig::setPedestal(int ilin, int ichannel, int ped)
{
#ifdef H4_BOUNDS_CHECK
  if(ichannel<0 || ichannel>=nchannels_ || 
     ilin<0     || ilin>=5) {
    std::cout << "H4FenixStripConfig::setPedestal(ilin=" << ilin
	      << ", ichannel=" << ichannel
	      << ") ERROR, args out of range" << std::endl;
    return;
  }
#endif
  pedestals_[5*ilin+ichannel] = ped;
}


int H4FenixStripConfig::getCCMantissa(int ilin, int ichannel) const
{
#ifdef H4_BOUNDS_CHECK
  if(ichannel<0 || ichannel>=nchannels_ || 
     ilin<0     || ilin>=5) {
    std::cout << "H4FenixStripConfig::getCCMantissa(ilin=" << ilin
	      << ", ichannel=" << ichannel
	      << ") ERROR, args out of range" << std::endl;
    return 0;
  }
#endif
  return mantissa_[5*ilin+ichannel];
}


int H4FenixStripConfig::getCCExponent(int ilin, int ichannel) const
{
#ifdef H4_BOUNDS_CHECK
  if(ichannel<0 || ichannel>=nchannels_ || 
     ilin<0     || ilin>=5) {
    std::cout << "H4FenixStripConfig::getCCExponent(ilin=" << ilin
	      << ", ichannel=" << ichannel
	      << ") ERROR, args out of range" << std::endl;
    return 0;
  }
#endif
  return exponent_[5*ilin+ichannel];
}


void H4FenixStripConfig::getAmplitudeFilterWeights(std::vector<int>& w) const
{
  w.clear();
  int i;
  for(i=0;i<5;i++) 
    w.push_back(afw_[i]);
}


void H4FenixStripConfig::setAmplitudeFilterWeights(std::vector<int> const& w)
{
  if( w.size() != 5 ) {
    std::cout << "H4FenixStripConfig::setAmplitudeFilterWeights ERROR: "
	      << "incorrect size=" << w.size() << std::endl;
    return;
  }
  int i;
  for(i=0;i<5;i++)
    afw_[i]=w[i];
}


void H4FenixStripConfig::setDefaultFPPAConfig()
{
  nchannels_=4;
  mode_=0; // FIXME: Barrel
  
  // linearizers:
  int i, c;
  for(i=0;i<5;i++) {
    // channel 0
    pedestals_[5*i+0]=0;
    mantissa_[5*i+0]=0x001;
    exponent_[5*i+0]=0;
    // channel 1
    pedestals_[5*i+1]=0;
    mantissa_[5*i+1]=0x234;
    exponent_[5*i+1]=6;
    // channel 2
    pedestals_[5*i+2]=0;
    mantissa_[5*i+2]=0x211;
    exponent_[5*i+2]=5;
    // channel 3
    pedestals_[5*i+3]=0;
    mantissa_[5*i+3]=0x132;
    exponent_[5*i+3]=2;
  }
  
  // Strip Fine Grain Veto Bit
  fgvb5Thresh_=100; // FIXME: check this value!
  fgvb5Lut_=0x34404000;
  
  // Amplitude filter
  afw_[0]=0x09; 
  afw_[1]=0x1B;
  afw_[2]=0x28;
  afw_[3]=0x71;
  afw_[4]=0x41;
  
  // formatter
  formatterShift_=2;
}



void H4FenixStripConfig::copy(H4FenixStripConfig const& cf)
{
  nchannels_ = cf.nchannels_;
  mode_ = cf.mode_;
  pedestals_.clear();
  mantissa_.clear();
  exponent_.clear();
  std::copy(cf.pedestals_.begin(), cf.pedestals_.end(), 
	    std::back_inserter(pedestals_));
  std::copy(cf.mantissa_.begin(), cf.mantissa_.end(), 
	    std::back_inserter(mantissa_));
  std::copy(cf.exponent_.begin(), cf.exponent_.end(), 
	    std::back_inserter(exponent_));
  memcpy(afw_, cf.afw_, 5*sizeof(int));
  fgvb5Thresh_ = cf.fgvb5Thresh_;
  fgvb5Lut_ = cf.fgvb5Lut_;
  formatterShift_ = cf.formatterShift_;
}


