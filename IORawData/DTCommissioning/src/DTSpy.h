#ifndef DaqSource_DTSpy_h
#define DaqSource_DTSpy_h

/*
 *  DTSpy.h
 *  
 *
 *  Created by Sandro Ventura on 7/28/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */


#include "DTSpyHelper.h"


class DTSpy : public DTCtcp {
  protected:
   
	char * spybuf;
	 
  public:
	   
	DTSpy();
	DTSpy(char *hostaddr,int port);
	~DTSpy();
	
	int getNextBuffer();

    int getBuffSize();
	int getRunNo();
	char * getEventPointer();
	
};


#endif

