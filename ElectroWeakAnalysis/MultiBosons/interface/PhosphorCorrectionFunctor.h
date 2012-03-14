/**
 * File: PhosphorCorrectionFunctor.h
 * Package: ElectroWeakAnalysis/MultiBosons
 * 
 * @class       PhosphorCorrectionFunctor
 * 
 * @brief       Corrects photon energy using PHOSPHOR fit results.
 * 
 * @details     Provides the @c operator(...) that takes some basic 
 *              photon quantities and returns the corrected photon transverse 
 *              energy. It uses the results of the PHOton energy Scale
 *              and PHOton energy Resolution in-situ measurement using
 *              radiative Z -> mu mu gamma decays (PHOSPHOR fit), 
 *              @see http://indico.cern.ch/getFile.py/access?contribId=1&resId=0&materialId=slides&confId=176399
 *
 *              This correction is being develped and primarily targeted
 *              for the V+gamma cross-section measurement in the context
 *              of the SMP MultiBosons Vgamma group using 2011 data. 
 *              However, other analyses using soft-to-intermediate photons 
 *              of typical transverse momenta in the range of 10-50 GeV
 *              may benefit from it too.
 *              
 *              It has multiple modes of operation depending on the nature 
 *              of the photons, one for the photons in simulation and 
 *              one for real data. The mode is specified in the 
 *              constructor by the embeded enum type
 *              @c PhosphorCorrector::Mode that has either
 *              the value @c kMonteCarlo or @c kRealData, respectively.
 * 
 *              The corrected energy aims at improving the scale for 
 *              both simulation and data. In addition, it also aims at
 *              smearing the simulation such that the resolution in
 *              simulation matches the resolution in data. 
 * 
 *              The photon properties provided as input depend on the mode.
 *              For the real data, they are:
 *                  @li @c pt - reconstructed transverse energy
 *                  @li @c eta - pseudorapidity
 *                  @li @c runNumber - the run number 
 *              For the simulation, instead of the run number a different
 *              parameter is required for the correlated resolution smearing,
 *              generator-level energy:
 *                  @li @c pt - reconstructed transverse energy
 *                  @li @c eta - pseudorapidity
 *                  @li @c genEnergy - the generator-level energy 
 *              The gen-level energy is the energy of a 
 *              matched generator-level photon if such a match is found,
 *              or any non-positive number if a matched generator-level
 *              photon is not found. No resolution smearing is performed
 *              in the latter case and only the scale correction is applied.
 * 
 *              The configuration is passed to the constructor as a C string.
 *              It is currently just a place-holder and is not used.
 * 
 *              This class is being develeped and maintained in the 
 *              ElectroWeakAnalysis/MultiBosons package of CMSSW but
 *              is intended to be independent of CMSSW such that
 *              it can be easily ported to analyses based and
 *              bare ROOT trees.
 * 
 * \version     0.1
 * \author      Jan Veverka, Caltech, veverka@caltech.edu
 * \date        14 March 2012
 */

#ifndef ElectroWeakAnalysis_MultiBosons_interface_PhosphorCorrector_h
#define ElectroWeakAnalysis_MultiBosons_interface_PhosphorCorrector_h

#include <string>
#include <vector>

namespace vgamma {
  
  class PhosphorCorrectionFunctor {
  public:
    /// Two modes of operation
    enum Mode {kMonteCarlo=0, kRealData, kNumModes};
    enum Category {kMonteCarloBarrelEt12to15=0  ,
                   kMonteCarloBarrelEt15to20    ,
                   kMonteCarloEndcapsEt12to15   ,
                   kMonteCarloEndcapsEt15to25   ,
                   kRealData2011ABarrelEt12to15 ,
                   kRealData2011ABarrelEt15to20 ,
                   kRealData2011AEndcapsEt12to15,
                   kRealData2011AEndcapsEt15to20,
                   kRealData2011BBarrelEt12to15 ,
                   kRealData2011BBarrelEt15to20 ,
                   kRealData2011BEndcapsEt12to15,
                   kRealData2011BEndcapsEt15to20,
                   kNumCategories};
    /// ctor
    PhosphorCorrectionFunctor(Mode mode, const char * configuration);
    
    /// Scale correction for photons in real data
    double operator()(double pt, double eta, int runNumber);

    /// Scale correction and correlated resolution smearing 
    /// for photons in simulation
    double operator()(double pt, double eta, double genEnergy);

    /// Data accessors
    const char * configuration() const {return configuration_.c_str();}

  private:
    /// Parses configuration and initializes data members.
    void init();
    
    /// Data members
    Mode mode_;
    std::string configuration_;
    std::vector<double> correctionFactors_;
        
  }; // PhosphorCorrectionFunctor
  
} // namespace vgamma
#endif
