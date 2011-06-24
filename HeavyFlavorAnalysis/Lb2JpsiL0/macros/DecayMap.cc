#include "DecayMap.hh"
#include <map>
#include <string>
#include <stdexcept>
#include <iostream>
#include <fstream>

using std::cout;
using std::endl;

//==========================================================================
DecayMap::DecayMap() { };

//==========================================================================
DecayMap::pos_t DecayMap::getPos(std::string key)
{
    pos_t ret;
    map_t::iterator pos = myMap.find(key);
    if(pos!=myMap.end())
    {
        ret=pos->second;
    }
    else
    {
        ret=myMap.size();
        myMap[key]=ret;
    }
    return ret;
}

//==========================================================================
void DecayMap::printMap()
{
    createRevMap();
    cout << "map contains " << myMap.size() << " entries" << endl;
    cout << "revmap contains " << myRevMap.size() << " entries" << endl;
    for(revmap_t::const_iterator it = myRevMap.begin(); it!=myRevMap.end(); it++)
    {
        cout << it->first << " - " << it->second << endl;
    }
}

//==========================================================================
void DecayMap::createRevMap()
{
    myRevMap.empty();
    for(map_t::const_iterator it = myMap.begin(); it!=myMap.end(); it++)
    {
        myRevMap[it->second]=it->first;
    }
}

//==========================================================================
unsigned int DecayMap::readFile(std::string filename)
{
    std::ifstream infile(filename.c_str());
    if(!infile.good())
    {
        throw std::invalid_argument("File " + filename + " not found.");
    }
    std::string s;
    unsigned int counter(0);
    while (getline(infile,s))
    {
        getPos(s);
        counter++;
    }
    return counter;
}

