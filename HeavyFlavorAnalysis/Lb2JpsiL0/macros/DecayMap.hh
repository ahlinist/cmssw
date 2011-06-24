#ifndef DECAYMAP_H_GUARD
#define DECAYMAP_H_GUARD

#include <map>
#include <string>

/*! Class for handling maps of decay chains

  It consists of two maps to handle requests in both ways.
  */
class DecayMap
{
public:
    typedef unsigned int pos_t;
    typedef std::map<std::string,pos_t> map_t;
    typedef std::map<pos_t,std::string> revmap_t;

    DecayMap();
    pos_t getPos(std::string key); // returns the key, adds entry if not existing
    void printMap(); // prints the map to cout
    unsigned int readFile(std::string filename); // reads a file into a map

private:
    map_t myMap; // main map
    revmap_t myRevMap; // reverse map, created upon need
    void createRevMap();
};

#endif

