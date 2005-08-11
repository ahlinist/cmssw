#ifndef DATAFORMATS_SISTRIPCLUSTERCOLLECTION_H
#define DATAFORMATS_SISTRIPCLUSTERCOLLECTION_H

#include "DataFormats/SiStripCluster/interface/SiStripCluster.h"
#include <vector>
#include <map>
#include <utility>

class SiStripClusterCollection {

 public:

  typedef std::vector<SiStripCluster>::const_iterator ContainerIterator;
  typedef std::pair<ContainerIterator, ContainerIterator> Range;
  typedef std::pair<unsigned int, unsigned int> IndexRange;
  typedef std::map<unsigned int, IndexRange> Registry;
  typedef std::map<unsigned int, IndexRange>::const_iterator RegistryIterator;

  SiStripClusterCollection() {}
  
  void put(Range input, unsigned int detID);
  const Range get(unsigned int detID) const;
  const std::vector<unsigned int> detIDs() const;
  
 private:
  mutable std::vector<SiStripCluster> container_;
  mutable Registry map_;

};

#endif // 


