#include "../interface/CSCTT_MyStub.h"
#include <iostream>



CSCTT_MyStub::CSCTT_MyStub(double ax, double ay,double az,double aeta,double aphi, int alayNo, bool ainDz, bool ainDphi)
{
x=ax; y=ay; z=az; r=sqrt(ax*ax+ay*ay); phi=aphi; eta=aeta; layer=alayNo;
cot=r/z;
inDz = ainDz; 
inDphi = ainDphi; 
}                                         

//float CSCTT_MyStub::Cot() {return(r/z)}


