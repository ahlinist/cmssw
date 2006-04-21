// -*- C++ -*-
// 
// H4FEConfig.h
// 
// utility class, to hold the configuration of the TPG
// functionalities of a FE board.
// 
// last modified: $Date: 2004/06/16 16:59:25 $
// by:            $Author: brunelie $
// 
#ifndef H4FECONFIG_H
#define H4FECONFIG_H


#include <string>
#include <vector>
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4DBEntry.h"
// #include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4FenixTcpConfig.h"
// #include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4FenixStripConfig.h"



class H4FEConfig : public H4DBEntry {
public:
  //! Fenix Operation Modes
  enum StripOperationMode {NoSuchOperationMode=-1,
			   BarrelMode=0, 
			   EndCapMode=1};
  
  //! Fenix Strip Amplitude filter output mode
  enum StripAmplitudeFilterMode {FullOutput=0,
				 NormalOutput=1};
  
  //! Default constructor
  H4FEConfig();
  
  //! Constructor
  H4FEConfig(int runNumber, int smTowerNumber, int mode, int nchannels);
  
  //! Destructor
  virtual ~H4FEConfig() { } 
  
#ifdef WITH_MYSQL
  virtual int   fillFromMySQL(TSQLServer* db, int index, int subIndex=-1, 
			      H4DBEntry::IndexMatch match=exact);
#endif
  
  //! read itself from an ASCII file. The format is the one
  //! defined and used by Jean Bourotte to dump the Fenix 
  //! configuration during the 2003 beam run. Please do 
  //! not use this function unless you know what you are doing.
  void          fillFromH42003AsciiFiles(std::string const& fname, int itower);
  
  //! read itself from an ASCII stream.
  virtual int   fillFromAsciiFile(istream&);
  
  //! write itself to an ASCII stream.
  virtual int   dumpToAsciiFile(ostream&) const;
  
  //! FE configurations are indexed by the run number
  virtual int   getIndex() const { return runNumber_; }
  
  //! SmTower number indexes the subRun
  virtual int   getSubIndex() const { return smTowerNumber_; }
  
  //! get the run number (primary index)
  int           getRunNumber() const { return runNumber_; }
  
  //! set the run number (primary index)
  void          setRunNumber(int run) { runNumber_=run; }
  
  //! get the Supermodule tower number (used as a secondary index)
  int           getSMTowerNumber() const { smTowerNumber_; };
  
  //! set the Supermodule tower number (used as a secondary index)
  void          setSMTowerNumber(int smTower) { smTowerNumber_=smTower; } 
  
  //! get the number of preamplifier channels: 4 if we use a FPPA, 
  //! 3 if we use a MGPA
  int           getNChannels() const { return nchannels_; }
  
  //! set the number of preamplifier channels
  void          setNChannels(int nchannels);
  
  //! operation mode (Barrel or EndCap)
  int           getOperationMode() const { return mode_; }
  
  //! set the operation mode 
  void          setOperationMode(int mode);
  
  //! tell whether the Fenix Strip amplitude filters
  //! are transparent or not
  int           getAFOutputMode() const { return afOutput_; }
  
  //! set the amplitude filter mode
  void          setAFOutputMode(int);
  
  //! enable / disable the i-th xtal in the istrip-th strip
  void          disableXtal(int istrip, int ixtal, bool disable);
  
  //! get the xtal-enabled bit
  bool          xtalEnabled(int istrip, int ixtal) const;
  
  //! set the calibration coefficient for channel number ichannel
  void          setCalibrationCoefficient(int strip, int xtal, int channel,
					  int mantissa, int exponent);
  
  //! set the calibration coefficient for channel number ichannel
  void          setCalibrationCoefficient(int strip, int xtal, int ichannel, 
					  double cal);
  
  //! get the pedestal values for channel number ichannel
  int           getPedestal(int strip, int xtal, int ichannel) const;
  
  //! set the pedestal values for channel number ichannel 
  void          setPedestal(int strip, int xtal, int ichannel, int ped);
  
  //! get the calibration coefficient mantissa (coded on nbits bits)
  int           getCCMantissa(int strip, int xtal, int channel) const;
  
  //! get the calibration coefficient exponent
  int           getCCExponent(int strip, int xtal, int channel) const;
  
  //! get the amplitude filter weights 
  void          getAmplitudeFilterWeights(int strip, std::vector<int>&) const;
  
  //! set the amplitude filter weights 
  void          setAmplitudeFilterWeights(int strip, std::vector<int> const&);
  
  //! get the veto bit threshold -- 12 bits value
  int           getFGVB5Threshold(int strip) const;
  
  //! set the veto bit threshhold (strip dependant) -- 12 bits value
  void          setFGVB5Threshold(int strip, int thresh);
  
  //! get the FGVB5 lookup table -- 32 boolean values
  unsigned int  getFGVB5LookupTable(int strip) const;
  
  //! set the FGVB5 lookup table -- 32 boolean values
  void          setFGVB5LookupTable(int strip, unsigned int lut);
  
  //! get the formatter Shift
  int           getFormatterShift(int strip) const;
  
  //! set the formatter Shift
  void          setFormatterShift(int strip, int shift);
  
  //! get the EE 32 bit lookup table
  unsigned int  getFGVBEELookupTable() const { return fgvbEELut_ ; }
  
  //! set the EE 32 bit lookup table
  void          setFGVBEELookupTable(unsigned int);
  
  //! get the EB 16 bit Lookup table 
  unsigned int  getFGVBEBLookupTable() const  { return fgvbEBLut_ ; }
  
  //! set the EB 16 bit Lookup table 
  void          setFGVBEBLookupTable(unsigned int);
  
  //! copy 
  void          copy(H4FEConfig const&);

  //! copy operator 
  H4FEConfig&   operator=(H4FEConfig const& c) { copy(c); return *this; }

  //! print method
  void          print() const;
  
  //! print the configuration of the i-th FENIX STRIP 
  void          printStripConfig(int i) const;
  
  //! print the configuration of the FENIX TCP
  void          printTcpConfig() const;

  //! return the FE mode name 
  static std::string modeName(int);

  //! return the amplitude filter mode name
  static std::string afModeName(int);

private:
  int    runNumber_;                          /// 
  int    smTowerNumber_;                      /// 
  int    nchannels_;                          /// 
  int    mode_;                               /// 
  int    afOutput_;                           /// 
  std::vector<int>           pedestals_;      /// 
  std::vector<int>           mantissa_;       /// 
  std::vector<int>           exponent_;       /// 
  std::vector<int>           afw_;            /// 
  std::vector<int>           fgvb5Thresh_;    /// 
  std::vector<unsigned int>  fgvb5Lut_;       /// 
  std::vector<unsigned int>  formatterShift_; /// 
  std::vector<unsigned int>  xtalMap_;        /// 
  
  unsigned int               fgvbEBLut_;      //!< 16 bits lut, used by the barrel
                                              //!< veto bit generator
  int                        el_, eh_;        //!< 8 bits energy thresholds, used by
                                              //!< the barrel veto bit generator.
  int                        tl_, th_;        //!< 7 bits energy fractions, used by
                                              //!< the barrel veto bit generator.
  unsigned int               fgvbEELut_;      //!< 32 bits lut, used by the end-cap
                                              //!< veto bit generator.
  
  ClassDef(H4FEConfig,1);
};


#endif





  //  std::vector<H4FenixStripConfig>  strip_;   /// 
  //  H4FenixTcpConfig                 tcp_;     /// 

  //! get the configuration of the i-th STRIP
  //  H4FenixStripConfig&          getStripConfig(int i);

  //! get the configuration of the i-th STRIP (const version)
  //  H4FenixStripConfig const&    getStripConfig(int i) const;

  //! get the configuration of the Fenix Tcp
  //  H4FenixTcpConfig&            getTcpConfig() { return tcp_; }

  //! get the configuration of the Fenix Tcp (const version)
  //  H4FenixTcpConfig   const&    getTcpConfig() const { return tcp_; }
