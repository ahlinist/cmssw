#ifndef Mapping_h
#define Mapping_h
/** \class  Mapping
 *   Channel Mapping for LB unpacking 
 *
 *  \author Raffaello Trentadue & Alessandro Grelli
 */
#include <string>
#include <map>

struct chamstrip{
   int region; 
   int ringwheel; 
   int sector;
   int subsector;
   int station;
   int layer;
   int roll;
   int strip;
  
};

class Mapping{
 public:
  Mapping();
  Mapping(int wheel, int sector);
  ~Mapping();
  
  int stripind(std::string lbname, int channel);

 private:
  int w;
  int s;
  std::map<std::string, std::map<int,int> > maps;
};

#endif
