
/***********************
This class implements a simple way to keep track of tabs for logfile output.
The function of this class is purely asthetic, and not integral to the Quaero algorithm.
Bruce Knuteson  2003
***********************/

#include <string>

#ifndef __ChatterTab__
#define __ChatterTab__

class ChatterTab
{
public:

  // Constructor
  ChatterTab();

  /// Tab over the current tabbing distance, then change the current tabbing distance an amount n
  std::string tab(int n=0);

private:
  int indention; ///< current tabbing distance
};

#endif











