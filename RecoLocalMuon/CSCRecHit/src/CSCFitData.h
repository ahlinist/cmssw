#ifndef CSCRecHit_CSCFitData_h
#define CSCRecHit_CSCFitData_h

/** \class CSCFitData
 * Hold data for fitting while RecHit building.
 */

class CSCFitData
{
 public:
  /* The default ctor initializes all elements of thePulseHeightMap for
   * which explicit digis do not exist. Thus y must be 0. and dy
   * should be typical 'fluctuation' size, perhaps 5-10?
   * Use sentinel value for x and bx. 
   */
  //@@ Value for dy should be tuned/settable in input
  CSCFitData() : x_(-99.), y_( 0. ), dy_( 8. ), bx_(-99) {};
  CSCFitData(float x, float y, float dy, int bx = 0) 
   : x_(x), y_(y), dy_(dy), bx_(bx) {};

  float x()  const {return x_;}
  float y()  const {return y_;}
  float dy() const {return dy_;}
  int   bx() const {return bx_;}
  bool operator<( const CSCFitData & data ) const { return y_ < data.y_; }
  void operator+=( float addend ) {y_ += addend;}

private:
  float x_;
  float y_;
  float dy_;
  int   bx_;
};

#endif

