/**************************
QuaeroItemFile provides a convenient way to sequentially read through a file containing many QuaeroItems.
Bruce Knuteson 2003
**************************/

#include <vector>
#include <string>
#include <fstream>

#ifndef __QuaeroItemFile__
#define __QuaeroItemFile__


template<class T>
class QuaeroItemFile
{
public:

  /// Constructor, attached to a specified file
  QuaeroItemFile<T>(std::string _filename);

  /// Constructor, attached to a specified input stream
  QuaeroItemFile<T>(std::istream& _inStream);

  /// Destructor
  ~QuaeroItemFile<T>();

  /// Return the next event in the file
  /// Return "true" if successful, "false" if no events remain to read
  bool nextEvent(T& event);

  /// Read in all events at once, returning a vector of all events
  /// If opt=="leaveOut", exclude all QuaeroItems with itemType in specialEventTypes
  /// If opt=="onlyInclude", include only those QuaeroItems with itemType in specialEventTypes
  std::vector<T> loadEvents(std::vector<std::string> specialEventTypes=std::vector<std::string>(0), std::string opt="leaveOut");

private:
  std::string filename; ///< the file this QuaeroItemFile is attached to
  std::istream* inStream; ///< the input stream this QuaeroItemFile is attached to
  QuaeroItemFile<T>* daughterQuaeroItemFile;

  void setUpIStream();

};

/// Normalize the file <sigFilename> so that the total weight equals <desiredWeight>
/// keeping relative weights within the file the same.
void normalizeWeight(std::string sigFilename, double desiredWeight);

#include "Validation/VstQuaeroUtils/interface/QuaeroItemFile.ii"

#endif
