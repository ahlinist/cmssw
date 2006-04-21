// -*- C++ -*-
// 
// H4SM10Config.h
// 
// last modified: $Date: 2004/11/23 17:03:54 $
// by:            $Author: nrl $
// 
#ifndef H4SM10CONFIG_H
#define H4SM10CONFIG_H

#include <string>
#include <vector>

#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4FEConfig.h"



class H4SM10Config : public TObject {
public:
  //! Default constructor --needed by ROOT
  H4SM10Config();
  
  //! Destructor
  virtual ~H4SM10Config();
  
  //! read itself from an ASCII file
  void                 readFrom2004AsciiFiles(std::string const& pedmul,
					      std::string const& stripparms);
  
  //! number of channels (3 for a MGPA. This is the default)
  int                  getNChannels() const { return nchannels_; }
  
  //! set the number of channels
  void                 setNChannels(int nchannels);
  
  //! get the operation mode (Barrel or EndCap)
  int                  getMode() const { return mode_; }
  
  //! set the operation mode
  void                 setMode(int mode);
  
  //! Get the i-th tower configuration
  H4FEConfig const*    getTowerConfig(int smTower) const;
  
  //! set the i-th tower configutation
  void                 setTowerConfig(H4FEConfig const&);
  
  //! copy
  void                 copy(H4SM10Config const&);

  //! copy operator
  H4SM10Config&        operator=(H4SM10Config const& c) { copy(c); return *this; }
  
  //! print method
  void                 print() const;
  
  //! clear
  void                 clear();
  
private:
  int runNumber_;                     ///
  int nchannels_;                     ///
  int mode_;                          ///
  std::vector<H4FEConfig*>  towers_;  ///
  
  friend class H4TPGConfigReader;
};




#endif

