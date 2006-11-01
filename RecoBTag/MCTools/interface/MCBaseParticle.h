#ifndef SimDataFormats_BTauMC_MCBaseParticle_h
#define SimDataFormats_BTauMC_MCBaseParticle_h

// base class for BTauMC MonteCarlo particles

//  Author:             Christian.Weiser@cern.ch
//  Ported to CMSSW by: Andrea.Bocci@cern.ch
//  Last Update:        12/07/2006

#include <string>
#include <vector>

#include "DataFormats/Common/interface/Ref.h"

#include "DataFormats/Math/interface/LorentzVector.h"
#include "RecoBTag/MCTools/interface/MCParticleInfo.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"

typedef edm::RefProd<edm::HepMCProduct>                 HepEventRef;
typedef edm::Ref<edm::HepMCProduct, HepMC::GenParticle> HepParticleRef;

class MCBaseParticle {

public:
  MCBaseParticle(){}

  MCBaseParticle( const HepMC::GenParticle* , const HepMC::GenEvent *);

  /// default destructor
  virtual ~MCBaseParticle();

  // fill particle info - this also decodes all particle related information and sets the corresponding variables
  virtual void setMCInfo( int info ) { 
    m_LundCode.setCode( info ); 
  }

  // fill kinematical info
  virtual void setFourVector ( const HepLorentzVector & fv) {
    m_FourVector = fv;
  }

  // print
  virtual void print() const;

  // access to data members
  virtual const MCParticleInfo & getMCInfo ()     const { return m_LundCode; }
  virtual const HepLorentzVector  & getFourVector () const { return m_FourVector; }
  virtual const HepMC::GenParticle * getParticle ()   const { return m_HepParticle; }

protected:
  const HepMC::GenEvent *    m_HepEvent;
  const HepMC::GenParticle * m_HepParticle;
  MCParticleInfo m_LundCode;
  HepLorentzVector  m_FourVector;

};

#endif // SimDataFormats_BTauMC_MCBaseParticle_h
