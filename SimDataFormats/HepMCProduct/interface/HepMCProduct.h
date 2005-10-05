#ifndef HepMCProduct_HepMCProduct_h
#define HepMCProduct_HepMCProduct_h

/** \class HepMCProduct
 *
 *  \author Joanna Weng, Filip Moortgat
 */

#include "CLHEP/HepMC/ReadHepMC.h"
#include "CLHEP/HepMC/GenEvent.h"


namespace edm {

  class HepMCProduct {
  public:
    HepMCProduct() : evt_( 0 ) { };
    explicit HepMCProduct( HepMC::GenEvent * evt );
     virtual ~HepMCProduct();
     void addHepMCData( HepMC::GenEvent  *evt);
    const HepMC::GenEvent& getHepMCData() const ;
 
    const HepMC::GenEvent * GetEvent() const {
      return evt_;
    }
    private:
     HepMC::GenEvent * evt_;
  };

}
#endif
