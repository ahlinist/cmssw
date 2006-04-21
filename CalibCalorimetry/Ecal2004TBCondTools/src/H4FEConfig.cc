// -*- C++ -*-
// 
// H4FEConfig.C
// 
/*!
  \class H4FEConfig
  \brief holds the configuration of one FE Board.
  
  last modified: $Date: 2005/01/05 15:56:46 $
  by:            $Author: nrl $
  
*/
#include <iomanip>
#include <fstream>
#include <sstream>
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4FEConfig.h"

#define NXTALS_IN_STRIP   5
#define NSTRIPS_IN_TOWER  5
#define NXTAL_IN_TOWER   25


H4FEConfig::H4FEConfig()
  : runNumber_(-1), smTowerNumber_(-1), nchannels_(4),
    mode_(0), afOutput_(0),
    fgvbEBLut_(0), el_(0), eh_(0), tl_(0), th_(0), fgvbEELut_(0)
{
  setNChannels(3);
}


H4FEConfig::H4FEConfig(int runNumber, int smTowerNumber, int mode, int nchannels)
  : runNumber_(runNumber), smTowerNumber_(smTowerNumber), nchannels_(nchannels),
    mode_(mode), afOutput_(0),
    fgvbEBLut_(0), el_(0), eh_(0), tl_(0), th_(0), fgvbEELut_(0)
{
  setNChannels(nchannels_);
}


#ifdef WITH_MYSQL
int H4FEConfig::fillFromMySQL(TSQLServer* db, int index, int subIndex, 
			      H4DBEntry::IndexMatch match)
{
  return 0;
}
#endif


/** This function reads the configuration of
    The TPG path, from the files provided by
    Jean Bourotte. The format is the following:
    
    # section 1: configuration of the linearizers
    <itower> <ixtal> <ichannel> <A> <B> <C>
    # pedestal value = <A> 
    # exponent       = <B>
    # mantissa       = <C>
    
    # section 2: configuration of the amplitude filters
    <itower> <istrip> <magic word>
    
    # section 3: tells you which xtal is on (if other xtals are masked)
    <itower> <istrip> <xtal index in strip>
    
    # section 4: configuration of the amplitude filters
    <itower> <istrip> <w0> <w1> <w2> <w3> <w4>
 */
void H4FEConfig::fillFromH42003AsciiFiles(std::string const& fname, int itower)
{
  std::cout << "*** H4FEConfig::fillFromH42003AsciiFiles(fname=" << fname 
	    << ")" << std::endl;
  int counter=-1;
  int A, B, C;
  int pedestal, exponent, mantissa, ww;
  int i, c, itow, istrip, ixtal, ichannel;
  std::vector<int> w;
  
  ifstream ifs(fname.c_str());
  if (!ifs.is_open()) {
    std::cout << "H4FEConfig::fillFromH42003AsciiFiles(fname="
	      << fname << ") ERROR, unable to open file." 
	      << std::endl;
    return;
  }
  
  char buff[1024];
  while (ifs.good()) {
    ifs.getline(buff, 1024);
    counter++;
    std::stringstream sstrm(buff);
    sstrm >> itow;
    if (itow != itower) continue;
    if (counter >= 460) continue;
    
    // now, where are we in this f*ck* file ? 
    if (counter < 400) {
      // 
      // we are reading the 1st section
      // <itower> <ixtal> <ichannel> <A> <B> <C>
      // where A, B and C are 3 8-bit words holding the 
      // configuration constants, encoded as follows:
      // A=[7...0]  B=[7...4] [3...0]  C=[7...0]
      //   ^^^^^^^    ^^^^^^^ ^^^^^^^    ^^^^^^^
      //  ped(7-0)      exp   ped(11-8)  mantissa
      // 
      sstrm >> ixtal >> ichannel 
		 >> std::hex >> A >> std::hex >> B >> std::hex >> C;
      istrip = (int)(ixtal/5);
      ixtal = ixtal%5;
      pedestal = ((B & 0x0F) << 8) + A;
      exponent = ((B & 0xF0) >> 4);
      mantissa = C;
      setPedestal(istrip, ixtal, ichannel, pedestal);
      setCalibrationCoefficient(istrip, ixtal, ichannel, mantissa, exponent);
      
    } else if (counter < 420) {
      // 
      // we are reading the 2d section
      // <itower> <istrip> <A> 
      // 
      // where A=[7-6]     [5...1]       [0]
      //         ^^^^^     ^^^^^^^       ^^^
      //     reserved   Enable channel  reserved
      sstrm >> istrip >> std::hex >> A;
      if( A == 0x80 )
	setAFOutputMode(1);
      
    } else if (counter < 440) {
      // 
      // we are reading section 3, which tells us 
      // which crystal is on and which is not
      // <itower> <istrip> <A>
      // 
      // where A=[6-7]         [5...1]          [0]
      //         ^^^^^         ^^^^^^^          ^^^
      //      reserved         xtal bitmap     reserved
      sstrm >> istrip >> std::hex >> A;
      // loop over the xtals to check which ones are disabled
      unsigned int mask=1;
      for (i = 0; i < 5; i++) {
	mask = (mask << 1);
	if (!(A & mask)) {
	  disableXtal(istrip, i, true);
	} else {
	  disableXtal(istrip, i, false);
	}
      }
    } else {
      // 
      // we are apparently reading the last section:
      // --configuration of the amplitude filters...
      // 
      sstrm >> istrip;
      w.clear();
      unsigned int aaa;
      for (i = 0; i < 5; i++) {
	sstrm >> std::hex >> aaa;
	ww = (int)(0x7F & aaa);
	w.push_back(ww);
      }
      setAmplitudeFilterWeights(istrip, w);
    }
  }
}


int H4FEConfig::fillFromAsciiFile(istream& is)
{
  return 0;
}


int H4FEConfig::dumpToAsciiFile(ostream& os) const
{
  return 0;
}


void H4FEConfig::setNChannels(int nchannels)
{
#ifdef H4_BOUNDS_CHECK
  if (nchannels != 3 && nchannels != 4) {
    std::cout << "H4TPGFEConfig::setNChannels(nchannels=" << nchannels
	      << ") invalig arg." << std::endl;
    return;
  }
#endif
  nchannels_=nchannels;
  
  // resize the arrays...
  pedestals_.resize(nchannels_*NXTAL_IN_TOWER);
  mantissa_.resize(nchannels_*NXTAL_IN_TOWER);
  exponent_.resize(nchannels_*NXTAL_IN_TOWER);
  afw_.resize(5*NSTRIPS_IN_TOWER);
  fgvb5Thresh_.resize(NSTRIPS_IN_TOWER);
  fgvb5Lut_.resize(NSTRIPS_IN_TOWER);
  formatterShift_.resize(NSTRIPS_IN_TOWER);
  xtalMap_.resize(NSTRIPS_IN_TOWER);
  
  int i;
  // by default, all the channels are enabled...
  for (i = 0; i < NSTRIPS_IN_TOWER; i++)
    xtalMap_[i] = 0xFF;
}


/** Set the hardware operation mode:
 *   mode=0: barrel (this is the default)
 *   mode=1: end-cap
 */
void H4FEConfig::setOperationMode(int mode)
{
#ifdef H4_BOUNDS_CHECK
  if (mode != 0 && mode != 1) {
    std::cout << "H4FEConfig::setOperationMode(mode=" << mode
	      << ") invalig arg." << std::endl;
    return;
  }
#endif
  mode_ = mode;
}


/** set the amplitude filter operation mode.
 *  afmode=0: partial output (this is the default)
 *  afmode-1: full output
 */
void H4FEConfig::setAFOutputMode(int afmode)
{
#ifdef H4_BOUNDS_CHECK
  if (afmode != 0 && afmode != 1) {
    std::cout << "H4FEConfig::setAFOutputMode(afmode=" << afmode
	      << ") invalig arg." << std::endl;
    return;
  }
#endif
  afOutput_ = afmode;
}


/*!
  Disable the specified xtal. It is possible indeed to disable 
  a given xtal (Strip Mode) or strip (Tcp Mode)
 */
void H4FEConfig::disableXtal(int istrip, int ixtal, bool disable)
{
#ifdef H4_BOUNDS_CHECK
  if (istrip < 0 || istrip >= 5 || 
      ixtal < 0  || ixtal >= 5) {
    std::cout << "H4FEConfig::disableXtal(istrip=" << istrip
	      << ", ixtal=" << ixtal 
	      << ") args out of range" << std::endl;
    return;
  }
#endif
  unsigned int mask = (1 << (ixtal+1));
  if (disable) {
    mask = (mask^0xFF);
    xtalMap_[istrip] = (xtalMap_[istrip] & mask);
  } else {
    xtalMap_[istrip] = (xtalMap_[istrip] | mask);
  }
}


bool H4FEConfig::xtalEnabled(int istrip, int ixtal) const
{
#ifdef H4_BOUNDS_CHECK
  if (istrip < 0 || istrip >= 5 || 
      ixtal < 0  || ixtal >= 5) {
    std::cout << "H4FEConfig::xtalEnabled(istrip=" << istrip
	      << ", ixtal=" << ixtal 
	      << ") args out of range" << std::endl;
    return false;
  }
#endif
  unsigned int mask = 1 << (ixtal+1);
  return (bool)(xtalMap_[istrip] & mask);
}



void H4FEConfig::setCalibrationCoefficient(int strip, int xtal, int ichannel,
					   int mantissa, int exponent)
{
#ifdef H4_BOUNDS_CHECK
  if (strip < 0    || strip >= 5 || 
      xtal < 0     || xtal >= 5  || 
      ichannel < 0 || ichannel >= nchannels_) {
    std::cout << "H4FEConfig::setCalibrationCoefficient(strip=" << strip
	      << ", xtal=" << xtal << ", channel=" << ichannel 
	      << ") args out of range" << std::endl;
    return;
  }
#endif
  mantissa_[strip*NXTALS_IN_STRIP*nchannels_+xtal*nchannels_+ichannel]=mantissa;
  exponent_[strip*NXTALS_IN_STRIP*nchannels_+xtal*nchannels_+ichannel]=exponent;
}


void H4FEConfig::setCalibrationCoefficient(int strip, int xtal, int ichannel, double cal)
{
#ifdef H4_BOUNDS_CHECK
  if (strip < 0    || strip >= 5 || 
      xtal < 0     || xtal >= 5  || 
      ichannel < 0 || ichannel >= nchannels_) {
    std::cout << "H4FEConfig::setCalibrationCoefficient(strip=" << strip
	      << ", xtal=" << xtal << ", channel=" << ichannel 
	      << ") args out of range" << std::endl;
    return;
  }
#endif
  int exp, m;
  double mantissa;
  mantissa = std::frexp(cal,&exp);
  m = (int)(mantissa * pow(2., exp));
  setCalibrationCoefficient(strip, xtal, ichannel, m, exp);
}


int H4FEConfig::getPedestal(int strip, int xtal, int ichannel) const
{
#ifdef H4_BOUNDS_CHECK
  if (strip < 0    || strip >= 5 || 
      xtal < 0     || xtal >= 5  || 
      ichannel < 0 || ichannel >= nchannels_) {
    std::cout << "H4FEConfig::getPedestal(strip=" << strip
	      << ", xtal=" << xtal << ", channel=" << ichannel 
	      << ") args out of range" << std::endl;
    return 0;
  }
#endif
  return pedestals_[strip*NXTALS_IN_STRIP*nchannels_+xtal*nchannels_+ichannel];
}


void H4FEConfig::setPedestal(int strip, int xtal, int ichannel, int ped)
{
#ifdef H4_BOUNDS_CHECK
  if (strip < 0    || strip >= 5 || 
      xtal < 0     || xtal >= 5  || 
      ichannel < 0 || ichannel >= nchannels_) {
    std::cout << "H4FEConfig::setPedestal(strip=" << strip
	      << ", xtal=" << xtal << ", channel=" << ichannel 
	      << ") args out of range" << std::endl;
    return;
  }
#endif
  pedestals_[strip*NXTALS_IN_STRIP*nchannels_+xtal*nchannels_+ichannel] = ped;
}


int H4FEConfig::getCCMantissa(int strip, int xtal, int ichannel) const
{
#ifdef H4_BOUNDS_CHECK
  if (strip < 0    || strip >= 5 || 
      xtal < 0     || xtal >= 5  || 
      ichannel < 0 || ichannel >= nchannels_) {
    std::cout << "H4FEConfig::getCCMantissa(strip=" << strip
	      << ", xtal=" << xtal << ", channel=" << ichannel 
	      << ") args out of range" << std::endl;
    return 0;
  }
#endif
  return mantissa_[strip*NXTALS_IN_STRIP*nchannels_+xtal*nchannels_+ichannel];
}


int H4FEConfig::getCCExponent(int strip, int xtal, int ichannel) const
{
#ifdef H4_BOUNDS_CHECK
  if (strip < 0    || strip >= 5 || 
      xtal < 0     || xtal >= 5  || 
      ichannel < 0 || ichannel >= nchannels_) {
    std::cout << "H4FEConfig::getCCExponent(strip=" << strip
	      << ", xtal=" << xtal << ", channel=" << ichannel 
	      << ") args out of range" << std::endl;
    return 0;
  }
#endif
  return exponent_[strip*NXTALS_IN_STRIP*nchannels_+xtal*nchannels_+ichannel];
}


void H4FEConfig::getAmplitudeFilterWeights(int strip, std::vector<int>& w) const
{
#ifdef H4_BOUNDS_CHECK
  if (strip < 0    || strip >= 5) {
    std::cout << "H4FEConfig::getAmplitudeFilterWeights(strip=" << strip
	      << ") args out of range" << std::endl;
    return;
  }
#endif
  w.clear();
  for(int i = 0; i < 5; i++) w.push_back( afw_[strip*5 + i] );
}


void H4FEConfig::setAmplitudeFilterWeights(int strip, std::vector<int> const& w)
{
#ifdef H4_BOUNDS_CHECK
  if (strip < 0    || strip >= 5) {
    std::cout << "H4FEConfig::setAmplitudeFilterWeights(strip=" << strip
	      << ") args out of range" << std::endl;
    return;
  }
#endif
  for(int i = 0; i < 5; i++)
    afw_[strip*5 + i] = w[i];
  //  cout << "H4FEConfig::setAmplitudeFilterWeights(strip=" << strip << ")" << endl;
  //  for(i=0;i<5;i++)
  //    cout << " ---> afw_[" << i << "]=" << afw_[strip*5+i] << endl;
}


int H4FEConfig::getFGVB5Threshold(int strip) const
{
#ifdef H4_BOUNDS_CHECK
  if (strip < 0 || strip >= 5) {
    std::cout << "H4FEConfig::getFGVB5Threshold(strip=" << strip
	      << ") args out of range" << std::endl;
    return 0;
  }
#endif
  return fgvb5Thresh_[strip];
}


void H4FEConfig::setFGVB5Threshold(int strip, int thresh)
{
#ifdef H4_BOUNDS_CHECK
  if (strip < 0 || strip >= 5) {
    std::cout << "H4FEConfig::getFGVB5Threshold(strip=" << strip
	      << ") args out of range" << std::endl;
    return;
  }
#endif
  fgvb5Thresh_[strip] = thresh;
}


unsigned int H4FEConfig::getFGVB5LookupTable(int strip) const
{
#ifdef H4_BOUNDS_CHECK
  if (strip < 0 || strip >= 5) {
    std::cout << "H4FEConfig::getFGVB5LookupTable(strip=" << strip
	      << ") args out of range" << std::endl;
    return 0;
  }
#endif
  return fgvb5Lut_[strip];
}


void H4FEConfig::setFGVB5LookupTable(int strip, unsigned int lut)
{
#ifdef H4_BOUNDS_CHECK
  if (strip < 0 || strip >= 5) {
    std::cout << "H4FEConfig::setFGVB5LookupTable(strip=" << strip
	      << ") args out of range" << std::endl;
    return;
  }
  // FIXME: I should also test the Lut value 
#endif
  fgvb5Lut_[strip] = lut;
}


int H4FEConfig::getFormatterShift(int strip) const
{
#ifdef H4_BOUNDS_CHECK
  if (strip < 0 || strip >= 5) {
    std::cout << "H4FEConfig::getFormatterShift(strip=" << strip
	      << ") args out of range" << std::endl;
    return 0;
  }
#endif
  return formatterShift_[strip];
}


void H4FEConfig::setFormatterShift(int strip, int shift)
{
#ifdef H4_BOUNDS_CHECK
  if (strip < 0 || strip >= 5) {
    std::cout << "H4FEConfig::setFormatterShift(strip=" << strip
	      << ") args out of range" << std::endl;
    return;
  }
#endif
  formatterShift_[strip] = shift;
}


void H4FEConfig::setFGVBEELookupTable(unsigned int lut)
{
#ifdef H4_BOUNDS_CHECK
  // FIXME: should check the value of the lut...
#endif
  fgvbEELut_ = lut;
}


void H4FEConfig::setFGVBEBLookupTable(unsigned int lut)
{
#ifdef H4_BOUNDS_CHECK
  // FIXME: should check the value of the lut...
#endif
  fgvbEBLut_ = lut;
}


void H4FEConfig::copy(H4FEConfig const& conf)
{
  runNumber_     = conf.runNumber_;
  smTowerNumber_ = conf.smTowerNumber_;
  nchannels_     = conf.nchannels_;
  mode_          = conf.mode_;
  afOutput_      = conf.afOutput_;
  
  pedestals_.clear();
  std::copy(conf.pedestals_.begin(), conf.pedestals_.end(), 
	    std::back_inserter(pedestals_));
  mantissa_.clear();
  std::copy(conf.mantissa_.begin(), conf.mantissa_.end(), 
	    std::back_inserter(mantissa_));
  exponent_.clear();
  std::copy(conf.exponent_.begin(), conf.exponent_.end(), 
	    std::back_inserter(exponent_));
  afw_.clear();
  std::copy(conf.afw_.begin(), conf.afw_.end(), 
	    std::back_inserter(afw_));
  fgvb5Thresh_.clear();
  std::copy(conf.fgvb5Thresh_.begin(), conf.fgvb5Thresh_.end(), 
	    std::back_inserter(fgvb5Thresh_));
  fgvb5Lut_.clear();
  std::copy(conf.fgvb5Lut_.begin(), conf.fgvb5Lut_.end(), 
	    std::back_inserter(fgvb5Lut_));
  formatterShift_.clear();
  std::copy(conf.formatterShift_.begin(), conf.formatterShift_.end(), 
	    std::back_inserter(formatterShift_));
  xtalMap_.clear();
  std::copy(conf.xtalMap_.begin(), conf.xtalMap_.end(), 
	    std::back_inserter(xtalMap_));
  
  
  fgvbEBLut_ = conf.fgvbEBLut_;
  el_        = conf.el_;
  eh_        = conf.eh_;
  tl_        = conf.tl_;
  th_        = conf.th_;
  fgvbEELut_ = conf.fgvbEELut_;
}



void H4FEConfig::print() const
{
  std::cout << "H4FEConfig::print(): " << std::dec << std::endl;
  std::cout << " (*) runNumber:    " << std::setw(10) << runNumber_
	    << "    "
	    << " (*) smTowerNumber:" << std::setw(10) << smTowerNumber_
	    << std::endl;
  std::cout << " (*) nchannels:    " << std::setw(10) << nchannels_
	    << "    "
	    << " (*) mode:         " << std::setw(10) << modeName(mode_)
	    << std::endl;
  std::cout << " (*) afOutput:     " << std::setw(10) << afModeName(afOutput_)
	    << std::endl;
  
  // Now, dumps the internal configuration ... 
  for(int i = 0; i < 5; i++)
    printStripConfig(i);
  
  printTcpConfig();
}


void H4FEConfig::printStripConfig(int i) const
{
  std::cout << std::dec << " STRIP[" << i << "]:" << std::endl;
  
  for (int x = 0; x < NXTALS_IN_STRIP; x++) {
    std::cout << " (*) LIN[" << std::setw(2) << x 
	      << " | en=" << xtalEnabled(i,x)
	      << "]:";
    for (int c = 0; c < nchannels_; c++) {
      if (c > 0) std::cout << "                    ";
      std::cout << " p=" << std::dec << getPedestal(i,x,c) 
		<< " ("  << std::hex << getPedestal(i,x,c) << ") ";
      std::cout << " m=" << std::dec << getCCMantissa(i,x,c)
		<< " ("  << std::hex << getCCMantissa(i,x,c) << ") ";
      std::cout << " e=" << std::dec << getCCExponent(i,x,c)
		<< " ("  << std::hex << getCCExponent(i,x,c) << ")"
		<< std::endl;
    }
  }
  std::vector<int> w;
  getAmplitudeFilterWeights(i,w);
  std::cout << " (*) AF: w0=" << w[0] << " w1=" << w[1]
	    << " w2=" << w[2] << " w3=" << w[3] << " w4=" << w[4]
	    << std::endl;
  std::cout << " (*) XTALMAP[" << i 
	    << "]: " << std::hex << xtalMap_[i] 
	    << std::endl;
  
  std::cout << " (*) VETO BIT:  " << std::endl;
  std::cout << " (*) FORMATTER: " << std::endl;
  std::cout << std::dec;
}


void H4FEConfig::printTcpConfig() const
{
}


std::string H4FEConfig::modeName(int mode)
{
  switch (mode) {
  case 0:
    return "barrel";
    break;
  case 1:
    return "end-cap";
    break;
  }
  return "unknown";
}


std::string H4FEConfig::afModeName(int mode)
{
  switch (mode) {
  case 0:
    return "default";
    break;
  case 1:
    return "transparent";
    break;
  }
  return "unknown";
}





//      if( A != 0x1F )
// loop over the xtals in the strip
//	for(ixtal=0;ixtal<NXTALS_IN_STRIP;ixtal++);
//	  if( ixtal != (A-1) )
//	    for(c=0;c<nchannels_;c++) {
//	      cout << " istrip=" << istrip << ", ixtal=" << ixtal
//		   << ", ichannel=" << c 
//		   << ", A=" << A
//		   << endl;
//	      setCalibrationCoefficient(istrip, ixtal, c, 0);
//	      setPedestal(istrip, ixtal, c, 0);
