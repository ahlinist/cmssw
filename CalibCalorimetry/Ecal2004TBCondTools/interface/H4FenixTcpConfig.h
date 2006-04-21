// -*- C++ -*-
// 
// H4FenixTcpConfig.h
// 
// 
//   last modified: $Date: 2003/08/28 08:31:25 $
//   by:            $Author: nrl $
// 
#ifndef H4FENIXTCPCONFIG_H
#define H4FENIXTCPCONFIG_H

#include <vector>

#include <TObject.h>


class H4FenixTcpConfig : public TObject {
public:
  //! Default constructor
  H4FenixTcpConfig();
  
  //! Default destructor
  ~H4FenixTcpConfig();
  
  //! get the mode of operation (Barrel or EndCap ?)
  int          getMode() const { return mode_ ; }
  
  //! set the operation mode 
  void         setMode(int mode);
  
  //! get the EE 32 bit lookup table
  unsigned int getFGVBEELookupTable() const { return fgvbEELut_ ; } 
  
  //! set the EE 32 bit lookup table
  void         setFGVBEELookupTable(unsigned int);
  
  //! get the EB 16 bit Lookup table 
  unsigned int getFGVBEBLookupTable() const  { return fgvbEBLut_ ; }
  
  //! set the EB 16 bit Lookup table 
  void         setFGVBEBLookupTable(unsigned int);
  
  //! reset default configuration
  void         setDefaultConfig();
  
  //! copy 
  void         copy(H4FenixTcpConfig const&);
  
  //! copy operator
  H4FenixTcpConfig& operator=(H4FenixTcpConfig const& fc) { copy(fc); return *this; } 

 private:
  
  int mode_;
  
  // configuration of the one FENIX chip operated in TCP mode
  unsigned int         fgvbEBLut_;      //!< 16 bits lut, used by the barrel
                                        //!< veto but generator.
  int                  el_, eh_;        //!< 8 bits energy thresholds, used by
                                        //!< the barrel veto bit generator.
  int                  tl_, th_;        //!< 7 bits energy fractions, used by
                                        //!< the barrel veto bit generator.
  unsigned int         fgvbEELut_;      //!< 32 bits lut, used by the end-cap
                                        //!< veto bit generator.
  
  ClassDef(H4FenixTcpConfig,1);
};

#endif

