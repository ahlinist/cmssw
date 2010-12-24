#ifndef TauAnalysis_Core_genericAnalyzerAuxFunctions_h
#define TauAnalysis_Core_genericAnalyzerAuxFunctions_h

namespace GenericAnalyzer_namespace 
{
  // define data-structures used to pass information which cuts passed/failed
  // and about event weights to FilterStatisticsTable and event-dump plugins
  //
  // NOTE: need to use std::vector<std::pair<std::string, bool> >
  //       (and not std::map<std::string, bool>), 
  //       in order not to loose information about the order in which the cuts are applied !!
  //

  typedef std::pair<std::string, bool> string_bool_pair;
  struct filterResults_type : std::vector<string_bool_pair>
  {
    void insert(const std::string& name, bool value)
    {
      this->push_back(string_bool_pair(name, value));
    }

    filterResults_type::const_iterator find(const std::string& name) const
    {
      for ( filterResults_type::const_iterator it = this->begin();
            it != this->end(); ++it ) {
        if ( it->first == name ) return it;
      }
      return this->end(); // no entry with given name found
    } 
  }; 

  typedef std::pair<std::string, double> string_double_pair;
  struct eventWeights_type : std::vector<string_double_pair>
  {
    void insert(const std::string& name, double value)
    {
      this->push_back(string_double_pair(name, value));
    }

    eventWeights_type::const_iterator find(const std::string& name) const
    {
      for ( eventWeights_type::const_iterator it = this->begin();
            it != this->end(); ++it ) {
        if ( it->first == name ) return it;
      }
      return this->end(); // no entry with given name found
    } 
  }; 
}

#endif
