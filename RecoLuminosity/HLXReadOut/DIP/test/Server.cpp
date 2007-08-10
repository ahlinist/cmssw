#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include "Dip.h"


class ErrHandler:public DipPublicationErrorHandler{
	void handleException(DipPublication* publication, DipException& ex){
		printf("Error because %s", ex.what());
	}
};

int main(const int argc, const char** argv){
	
	// Create the publications
	printf("a");
	DipPublication *pub[2];
	DipFactory *dip = Dip::create("TestPublisher");
	char itemName[2][20]={"dip/test/pub1", "dip/test/pub2"};
	ErrHandler errorHandler;

	printf("b");
	for (int i=0; i < 2; i++){
		pub[i] = dip->createDipPublication(itemName[i], &errorHandler);
	}
	
	printf("c");
	// set up vars who's values are to be sent
	DipData *data = dip->createDipData();
	DipInt intVal = 1;
	float floatVal = 0.1f;
	
	
	// send data via DIP
	while(true){
		try{
			data->insert(intVal, "field1");
			data->insert(floatVal, "field2");
			DipTimestamp time;
			printf("Pushing field1 = %d and field2 = %f\n", intVal,
			floatVal);
			pub[0]->send((DipInt)(intVal + 1), time);
			pub[1]->send(*data, time);
			intVal+=1;
			floatVal+=0.1f;
			// simulates server accessing a publications data
			// source 
	printf("blah");
#ifdef WIN32
			Sleep(10000L);
#else
			sleep(1);
#endif
		} catch (DipException e){
			printf("Failed to send data\n");
		}
	}

	// shutdown
	dip->destroyDipPublication(pub[0]);
	dip->destroyDipPublication(pub[1]);
	delete data;
//	delete dip;

	return 0;
}	
