// -*- C++ -*-
// 
// H4SM10Config.C
// 
// last modified: $Date: 2004/11/23 17:03:54 $
// by:            $Author: nrl $
// 
#include <iostream>
#include <fstream>
#include <sstream>

#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4FEConfig.h"
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4SM10Config.h"


using namespace std;



H4SM10Config::H4SM10Config()
  : runNumber_(-1), nchannels_(3), mode_(0) // barrel
{
  int i;
  for(i=0;i<68;i++)
    towers_.push_back(new H4FEConfig(runNumber_, i+1, mode_, nchannels_));
}



H4SM10Config::~H4SM10Config()
{
  int i;
  for(i=0;i<68;i++)
    delete towers_[i];
}


void H4SM10Config::readFrom2004AsciiFiles(string const& pedmultfile,
					  string const& stripparms)
{
  // 1. the file containing the fenix strip linearizer configurations
  ifstream ifs(pedmultfile.c_str());
  if( !ifs.is_open() ) {
    cerr << "[H4SM10Config::readFrom2004AsciiFiles()] ERROR opening "
	 << pedmultfile << endl;
    return;
  }
  
  int i;
  char buffer[1024];
  unsigned int itower, istrip, ixtal, ichannel;
  unsigned int A, B, C;
  while( ifs.good() ) {
    ifs.getline(buffer, 1024);
    stringstream sstrm(buffer);
    sstrm >> hex >> itower >> istrip >> ixtal >> ichannel;
    sstrm >> A >> B >> C;
    if(itower<1 || itower>68) {
      cerr << "[H4SM10Config::readFrom2004AsciiFiles()] ERROR itower= "
	   << itower << endl;
      return;
    }
    if(istrip<1 || istrip>5) {
      cerr << "[H4SM10Config::readFrom2004AsciiFiles()] ERROR istrip= "
	   << istrip << endl;
      return;
    }
    if(ixtal<1 || ixtal>5) {
      cerr << "[H4SM10Config::readFrom2004AsciiFiles()] ERROR ixtal= "
	   << ixtal << endl;
      return;      
    }
    if(ichannel<1 || ichannel>3) {
      cerr << "[H4SM10Config::readFrom2004AsciiFiles()] ERROR ichannel= "
	   << ichannel << endl;
      return;
    }
    
    unsigned int pedestal = ((B & 0x0F) << 8) + A;
    unsigned int exponent = ((B & 0xF0) >> 4);
    unsigned int mantissa = C;
    towers_[itower-1]->setPedestal(istrip-1, ixtal-1, ichannel-1, pedestal);
    towers_[itower-1]->setCalibrationCoefficient(istrip-1, ixtal-1, ichannel-1, mantissa, exponent);
  }
  ifs.close();
  
  // 2. the second file, containing the fenix strip configurations
  unsigned int mgpa_mode, pipe_delay, frame_length;
  unsigned int channel_enabled, peak_finder;
  vector<int> filter_weights;
  
  ifstream ifs2(stripparms.c_str());
  if( !ifs2.is_open() ) {
    cerr << "[H4SM10Config::readFrom2004AsciiFiles()] ERROR opening config file "
	 << stripparms << endl;
    return;
  }
  ifs2.getline(buffer, 1024);
  ifs2.getline(buffer, 1024);
  while( ifs2.good() ) {
    ifs2.getline(buffer, 1024);
    stringstream sstrm(buffer);
    sstrm >> hex >> itower >> istrip >> mgpa_mode >> pipe_delay;
    sstrm >> hex >> frame_length >> channel_enabled >> peak_finder;
    for(i=0;i<5;i++) {
      unsigned int w;
      sstrm >> hex >> w;
      filter_weights.push_back(w);
    }
    
    // crystals enabled / disabled
    unsigned int mask = 1;
    for(i=0;i<5;i++) {
      mask = (mask<<1);
      if( !(channel_enabled & mask) )
	towers_[itower-1]->disableXtal(istrip-1, i, true);
      else
	towers_[itower-1]->disableXtal(istrip-1, i, false);
    }
    
    // peak finder ON/OFF
    if( peak_finder & 0x80 ) 
      towers_[itower-1]->setAFOutputMode(1);
    else
      towers_[itower-1]->setAFOutputMode(0);
    
    towers_[itower-1]->setAmplitudeFilterWeights(istrip-1, filter_weights);
  }
  
}


void H4SM10Config::setNChannels(int nchannels)
{
#ifdef H4_BOUNDS_CHECK
  if(nchannels!=3 && nchannels!=4) {
    std::cout << "H4SM10Config::setNChannels(nchannels=" << nchannels
	      << ") invalid arg." << std::endl;
    return;
  }
#endif
  nchannels_ = nchannels;
}


void H4SM10Config::setMode(int mode)
{
  mode_ = mode;
}



H4FEConfig const* H4SM10Config::getTowerConfig(int smTower) const
{
#ifdef H4_BOUNDS_CHECK
  if(smTower<1 || smTower>68) {
    std::cout << "H4SM10Config::getTowerConfig(smTower=" << smTower
	      << ") invalid arg." << std::endl;
    return 0;
  }
#endif
  return towers_[smTower];
}


void H4SM10Config::setTowerConfig(H4FEConfig const& tc) 
{
  int smtn = tc.getSMTowerNumber();
#ifdef H4_BOUNDS_CHECK
  if(smtn<1 || smtn>68) {
    std::cout << "H4SM10Config::setTowerConfig(): tc.getSMTowerNumber()=" << smtn
	      << " -- invalid arg." << std::endl;
    return;
  }
  if(tc.getRunNumber() != runNumber_) {
    std::cout << "H4SM10Config::setTowerConfig(): runNumber=" << tc.getRunNumber()
	      << " != " << runNumber_ << " -- invalid arg." << std::endl;
    return;
  }
#endif
  towers_[smtn-1]->copy(tc);
}


void H4SM10Config::copy(H4SM10Config const& c)
{
  clear();
  int i;
  for(i=1;i<69;i++) 
    setTowerConfig(*(c.getTowerConfig(i)));
}


void H4SM10Config::print() const
{
  int i;
  for(i=0;i<68;i++)
    towers_[i]->print();
}


void H4SM10Config::clear()
{
  int i, sz = towers_.size();
  for(i=0;i<sz;i++) delete towers_[i];
  //  towers_[i].clear();
}
