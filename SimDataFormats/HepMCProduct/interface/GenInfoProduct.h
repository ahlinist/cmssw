#ifndef GenInfoProduct_h
#define GenInfoProduct_h

/** \class GenInfoProduct
 *
 *  \author Filip Moortgat
 */


namespace edm {

   

  class GenInfoProduct {
  public:
    GenInfoProduct() : cs_( 0 ) { }
    explicit GenInfoProduct( double cross_section );
    virtual ~GenInfoProduct() { }
    
    const double cross_section() const {return cs_ ; }
 
    void set_cross_section(double cross_section) { cs_ = cross_section ; }

    GenInfoProduct(GenInfoProduct const& x);

  private:
    double cs_;

  };

   
}
#endif
