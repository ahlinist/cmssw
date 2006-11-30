#ifndef HepMCProduct_HepMCProduct_h
#define HepMCProduct_HepMCProduct_h

/** \class HepMCProduct
 *
 *  \author Joanna Weng, Filip Moortgat
 */

#include "CLHEP/HepMC/ReadHepMC.h"
#include "CLHEP/HepMC/GenEvent.h"

#include "DataFormats/Common/interface/Ref.h"

class CLHEP::Hep3Vector ;

namespace edm {

   

  class HepMCProduct {
  public:
    HepMCProduct() : evt_( 0 ), isVtxGenApplied_ ( false ) { };
    explicit HepMCProduct( HepMC::GenEvent * evt );
    virtual ~HepMCProduct();
    
    void addHepMCData( HepMC::GenEvent  *evt);
    void applyVtxGen( CLHEP::Hep3Vector* vtxShift ) const ; 
    
    const HepMC::GenEvent& getHepMCData() const ;
 
    const HepMC::GenEvent * GetEvent() const {
      return evt_;
    }
    
    bool isVtxGenApplied() const { return isVtxGenApplied_ ; }

    HepMCProduct(HepMCProduct const& x);
    void swap(HepMCProduct & other);
    HepMCProduct& operator=(HepMCProduct const& other);

  private:
    HepMC::GenEvent * evt_;
    mutable bool      isVtxGenApplied_ ;
  };

  //This allows edm::Refs to work with HepMCProduct
  namespace refhelper {
      
    template<> 
    struct FindTrait<edm::HepMCProduct, HepMC::GenParticle> {
      struct Find : public std::binary_function<edm::HepMCProduct const&, int, HepMC::GenParticle const*> {
	typedef Find self;
	self::result_type operator()(self::first_argument_type iContainer,
				     self::second_argument_type iBarCode) {
	   return iContainer.getHepMCData().barcode_to_particle(iBarCode);
	}
      };
      typedef Find value;
    };
      
    template<> 
    struct FindTrait<edm::HepMCProduct, HepMC::GenVertex> {
      struct Find : public std::binary_function<edm::HepMCProduct const&, int, HepMC::GenVertex const*> {
	typedef Find self;
	self::result_type operator()(self::first_argument_type iContainer,
				     self::second_argument_type iBarCode) {
	   return iContainer.getHepMCData().barcode_to_vertex(iBarCode);
	}
      };
      typedef Find value;
    };
      
  }
   
}
#endif
