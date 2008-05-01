/*******************
ChatterTab definitions
Bruce Knuteson 2003
*******************/


#include "Validation/VstQuaeroUtils/interface/ChatterTab.hh"
#include <string>
#include <cassert>
using namespace std;

/***  Constructor  ***/

ChatterTab::ChatterTab()
{
  indention = 0;
  return;
}

/***  Methods  ***/

string ChatterTab::tab(int n)
{
  string ans = "";
  for(int i=0; i<indention; i++)
    ans = ans + "  ";
  indention += n;
  if(indention<0)
    indention = 0; // can't go beyond the left edge of the screen
  return(ans);
}
