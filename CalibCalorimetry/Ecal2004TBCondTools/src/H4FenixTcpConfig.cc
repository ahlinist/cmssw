// -*- C++ -*-
// 
// H4FenixTcpConfig.C
// 
/**
   \class H4FenixTcpConfig
   \brief hold the configuration of one Fenix Tcp Chip.
   
   last modified: $Date: 2003/08/28 08:34:29 $
   by:            $Author: nrl $
   
 */

#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4FenixTcpConfig.h"


ClassImp(H4FenixTcpConfig);


H4FenixTcpConfig::H4FenixTcpConfig()
  : mode_(0), fgvbEBLut_(0xC480),
    el_(32), eh_(64), tl_(64), th_(96),
    fgvbEELut_(0x0101115E)
{
  setDefaultConfig();
}


H4FenixTcpConfig::~H4FenixTcpConfig()
{
}


void H4FenixTcpConfig::setMode(int mode)
{
#ifdef H4_BOUNDS_CHECK
  if(mode!=3 && mode!=4) {
    std::cout << "H4FenixTcpConfig::setMode(mode=" << mode
	      << ") ERROR arg out of range" << std::endl;
    return;
  }
#endif
  mode_ = mode;
}


void H4FenixTcpConfig::setFGVBEELookupTable(unsigned int lut)
{
#ifdef H4_BOUNDS_CHECK
  if(lut>=MAX_32) {
    std::cout << "H4FenixTcpConfig::setFGVBEELookupTable(lut=" << lut
	      << ") arg out of range" << std::endl;
    return;
  }
#endif
  fgvbEELut_=lut;
}


void H4FenixTcpConfig::setFGVBEBLookupTable(unsigned int lut)
{
#ifdef H4_BOUNDS_CHECK
  if(lut>=MAX_16) {
    std::cout << "H4FenixTcpConfig::setFGVBEBLookupTable(lut=" << lut
	      << ") arg out of range" << std::endl;
    return;
  }
#endif
  fgvbEBLut_=lut;
}


void H4FenixTcpConfig::setDefaultConfig()
{
  mode_=0; // Barrel
  fgvbEBLut_=0xC480;
  el_=32;
  eh_=64;
  tl_=64;
  th_=96;
  fgvbEELut_=0x0101115E;
}


void H4FenixTcpConfig::copy(H4FenixTcpConfig const& cfg)
{
  mode_=cfg.mode_;
  fgvbEBLut_=cfg.fgvbEBLut_;
  el_=cfg.el_;
  eh_=cfg.eh_;
  tl_=cfg.tl_;
  th_=cfg.th_;
  fgvbEELut_=cfg.fgvbEELut_;
}

