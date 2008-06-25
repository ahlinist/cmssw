#ifndef DIP_H_INCLUDED
#define DIP_H_INCLUDED


#include "Options.h"
#include "DipException.h"
#include "StdTypes.h"
#include "DipSubscriptionListener.h"
#include "DipPublicationErrorHandler.h"
#include "DipFactory.h"

/**
* This is used to set up the DIP infrastructure.
* Singleton
*/
class DipDllExp Dip {
private:
	/**
	* DO not allow copying
	*/
	Dip(const Dip& other);
	
	/**
	* DO not allow copying
	*/
	Dip& operator=(const Dip& other);	

protected:
	// single instance per process
	static DipFactory* theFactory;

public:
	/**
	* Create a DipFactory Object - Dip can act as a publisher and subscriber
	* @param name - name of the publisher (will not appear in a publication name) - (call retains ownership of memory)
	* @returns ptr to factory - NOT OWNED BY CALLER
	* @throw DipInternalError - if unable to set up the DIP infra structure / unexpected exception
	*/
	static DipFactory* create(const char *name) throw (DipInternalError);


	/**
	* Create a DipFactory Object - Dip will act as a subscriber
	* @returns ptr to factory - NOT OWNED BY CALLER
	* @throw DipInternalError - if unable to set up the DIP infra structure / unexpected exception
	*/
	static DipFactory* create() throw (DipInternalError);


	/**
	* Shut DIP down - not DIP objects may be used after this
	* @throw DipInternalError unexpected exception
	*/
	static void shutdown() throw (DipInternalError);
};

#endif //DIP_H_INCLUDED
