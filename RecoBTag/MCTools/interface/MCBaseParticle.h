#ifndef SimDataFormats_BTauMC_MCBaseParticle_h
#define SimDataFormats_BTauMC_MCBaseParticle_h

// base class for BTag MonteCarlo particles

#include <string>
#include <vector>

#include "DataFormats/Math/interface/LorentzVector.h"
#include "RecoBTag/MCTools/interface/MCParticleInfo.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"

class MCBaseParticle {

public:
  MCBaseParticle(){}

  MCBaseParticle( const HepMC::GenParticle* , const HepMC::GenEvent *);

  /// default destructor
  virtual ~MCBaseParticle();

  /**
   * fill particle info - this also decodes all particle related information and
   * sets the corresponding variables
   */
  virtual void setPDGid( int info ) { 
    particleInfo_.setCode( info ); 
  }

  /**
   * fill kinematical info
   */
  virtual void fourVector ( const HepLorentzVector & fv) {
    fourVector_ = fv;
  }

  /**
   * prints base info
   */
  virtual void print() const;

  // access to data members
  virtual const MCParticleInfo & particleInfo ()     const { return particleInfo_; }
  virtual const math::XYZTLorentzVector  & fourVector () const { return fourVector_; }
  virtual const HepMC::GenParticle * particle ()   const { return hepParticle; }

protected:
  const HepMC::GenEvent *    hepEvent;
  const HepMC::GenParticle * hepParticle;
  MCParticleInfo particleInfo_;
  math::XYZTLorentzVector  fourVector_;

  math::XYZTLorentzVector lorentzVect(const CLHEP::HepLorentzVector & p4) const 
  	{return  math::XYZTLorentzVector( p4.x(), p4.y(), p4.z(), p4.t() );}

};

#endif // SimDataFormats_BTauMC_MCBaseParticle_h
