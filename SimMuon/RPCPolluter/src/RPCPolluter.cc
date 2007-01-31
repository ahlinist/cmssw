
#include "../interface/RPCPolluter.h"


// RPCPolluter


// Constructor of RPCPolluter

RPCPolluter::RPCPolluter(const RPCRoll* roll)
{

  int i;

  int strip;

  double time_hit;

  std::vector<int>::iterator pos_strip;

  std::vector<double>::iterator pos_time;


  strip_vector.reserve(100);
  time_vector.reserve(100);

  double area = 0.0;

  RPCDetId rpcId = roll->id();

  rate = 5.0;

  nbxing = 8;

  double gate = 25.0;

  int nstrips = roll->nstrips();

  if ( rpcId.region() == 0 )
    {
      const RectangularStripTopology* top_ = dynamic_cast<const
      RectangularStripTopology*>(&(roll->topology()));
      float xmin = (top_->localPosition(0.)).x();
      float xmax = (top_->localPosition((float)roll->nstrips())).x();
      float striplength = (top_->stripLength());
      area = striplength*(xmax-xmin);
    }

  double ave = rate*nbxing*gate*area*1.0e-9;

  N_hits = RandPoisson::shoot(ave);

  pos_strip = strip_vector.begin();
  pos_time = time_vector.begin();

  for ( i = 0; i < N_hits; i++ )
    {
      strip = RandFlat::shootInt(nstrips);
      strip_vector.insert(pos_strip,strip);
      ++pos_strip;
      time_hit = RandFlat::shoot((nbxing*gate));
      time_vector.insert(pos_time,time_hit);
      ++pos_time;
    }


}  // End Constructor of RPCPolluter





// Destructor of RPCPolluter

RPCPolluter::~RPCPolluter()
{


}  // End Destructor of RPCPolluter

