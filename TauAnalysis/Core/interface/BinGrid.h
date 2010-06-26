#ifndef TauAnalysis_Core_BinGrid_h  
#define TauAnalysis_Core_BinGrid_h

/** \class BinGrid
 *
 * Store number of bins and bin-boundaries
 * used for binning event passing selection
 * (in any number of dimensions)
 * 
 * \author Christian Veelken, UC Davis
 *         (inspired by code written for H1 by Paul Laycock, University of Liverpool)
 *
 * \version $Revision: 1.2 $
 *
 * $Id: BinGrid.h,v 1.2 2010/01/15 17:37:14 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <vector>
#include <string>

class BinGrid 
{
 public: 
  BinGrid();
  BinGrid(const edm::ParameterSet&);
  BinGrid(const BinGrid&);
  virtual ~BinGrid();
  
  unsigned dimensions() const { return numDimensions_; }
  unsigned numBins() const { return numBinsTotal_; }
  
  const std::vector<std::string>& objVarNames() const { return objVarNames_; }

  virtual int binNumber(const std::vector<double>&) const;

  virtual std::vector<double> binCenter(unsigned) const;
  virtual std::vector<double> binEdges(unsigned) const;
  virtual double binVolume(unsigned) const;

  virtual void print(std::ostream&) const;

  friend std::vector<std::string>& operator<<(std::vector<std::string>&, const BinGrid&);
  friend std::vector<std::string>& operator>>(std::vector<std::string>&, BinGrid&);

 protected:
//--- auxiliary functions
//    (for initializing numBinsPerDimension and dimValues
//     data-members for encoding/decoding of bin numbers)
  virtual void initialize();

 private:
//--- auxiliary functions
//    (for encoding/decoding of bin numbers)
  typedef std::vector<unsigned> vunsigned;
  unsigned encodeTotBin(const vunsigned&) const;
  vunsigned decodeTotBin(unsigned) const;
  unsigned getDimValue(unsigned) const;

  virtual std::vector<std::string> encodeStringRep() const;
  virtual void decodeStringRep(std::vector<std::string>&);

//--- data-members
  unsigned numDimensions_;

  typedef std::vector<std::string> vstring;
  vstring objVarNames_;

  // list of "left" bin-edges (similar to ROOT's TH1 histogram class) for each dimension
  // (xMin_i = binEdges[i][0],..binBoundary_j = binEdges[i][j + 1].., xMax_i = binEdges[i][numBinsPerDimension])
  typedef std::vector<double> vdouble;  
  std::vector<vdouble> binEdges_;

  // number of bins per dimension
  vunsigned numBinsPerDimension_; 
  unsigned numBinsTotal_;

  // auxiliary data-structure for encoding/decoding
  // of bin numbers
  vunsigned dimValues_;
};

std::vector<std::string>& operator<<(std::vector<std::string>&, const BinGrid&);
std::vector<std::string>& operator>>(std::vector<std::string>&, BinGrid&);

#endif  


