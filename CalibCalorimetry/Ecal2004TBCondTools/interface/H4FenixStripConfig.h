// -*- C++ -*-
// 
// H4FenixStripConfig.h
// 
//   last modified: $Date: 2003/08/28 11:24:02 $
//   by:            $Author: koblitz $
//
#ifndef H4FENIXSTRIPCONFIG_H
#define H4FENIXSTRIPCONFIG_H

#include <TObject.h>
#include <iostream>
#include <vector>


class H4FenixStripConfig : public TObject {
public:
  //! Default constructor --needed by ROOT
  H4FenixStripConfig();
  
  //! Destructor
  ~H4FenixStripConfig();
  
  //! number of channels (4 for a FPPA and 3 for a MGPA)
  int          getNChannels() const { return nchannels_ ; } 
  
  //! set the number of channels (4 for a FPPA and 3 for a MGPA)
  //! and clear the data structure
  void         setNChannels(int nchannels);
  
  //! get the mode of operation (Barrel or EndCap ?)
  int          getMode() const { return mode_ ; }
  
  //! set the operation mode 
  void         setMode(int mode);
  
  //! Calibration coefficient for ith FENIX, in nth Channel
  double       getCalibrationCoefficient(int ilin, int ichannel) const;
  
  //! set the calibration coefficient for channel number ichannel
  void         setCalibrationCoefficient(int ilin, int ichannel, double cal);
  
  //! get the pedestal values for channel number ichannel
  int          getPedestal(int ilin, int ichannel) const;
  
  //! set the pedestal values for channel number ichannel 
  void         setPedestal(int ilin, int ichannel, int ped);
  
  //! get the calibration coefficient mantissa (coded on nbits bits)
  int          getCCMantissa(int ilin, int channel) const;
  
  //! get the calibration coefficient exponent
  int          getCCExponent(int ilin, int channel) const;
  
  //! get the amplitude filter weights 
  void         getAmplitudeFilterWeights(std::vector<int>&) const;
  
  //! set the amplitude filter weights 
  void         setAmplitudeFilterWeights(std::vector<int> const&);
  
  //! get the veto bit threshold -- 12 bits value
  int          getFGVB5Threshold() const { return fgvb5Thresh_; } 
  
  //! set the veto bit threshhold (strip dependant) -- 12 bits value
  void         setFGVB5Threshold(int thresh) { fgvb5Thresh_=thresh; } 
  
  //! get the FGVB5 lookup table -- 32 boolean values
  unsigned int getFGVB5LookupTable() const { return fgvb5Lut_; } 
  
  //! set the FGVB5 lookup table -- 32 boolean values
  void         setFGVB5LookupTable(unsigned int lut) { fgvb5Lut_=lut; } 
  
  //! get the formatter Shift
  int          getFormatterShift() const { return formatterShift_; }
  
  //! set the formatter Shift
  void         setFormatterShift(int shift) { formatterShift_=shift; }
  
  //! default configuration
  void         setDefaultFPPAConfig();
  
  //! copy 
  void         copy(H4FenixStripConfig const&);
  
  //! copy operator
  H4FenixStripConfig& operator=(H4FenixStripConfig const& fc) 
                              { copy(fc); return *this; } 
  
private:
  int               nchannels_;
  int               mode_;
  std::vector<int>  pedestals_;
  std::vector<int>  mantissa_;
  std::vector<int>  exponent_;
  int               afw_[5];         //!< amplitude filter weights
  int               fgvb5Thresh_;    //!< threshold, used by the strip fine
                                     //!< grain veto bit generator
  unsigned int      fgvb5Lut_;       //!< 32 bit lookup table, used by
                                     //!< the fgvb generator
  unsigned int formatterShift_;      //!< 3 bits value of the shift to
                                     //!< be applied to the strip energy.
  
  ClassDef(H4FenixStripConfig,1);
};


#endif


