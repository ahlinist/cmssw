#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include "Dip.h"
#include <iostream>

using namespace std;

class ErrHandler:public DipPublicationErrorHandler{
	void handleException(DipPublication* publication, DipException& ex){
		printf("Error because %s", ex.what());
	}
};

int main(const int argc, const char** argv){

	//Check whether a specific DNS shall be used.
	if (argc == 2) {
		cout<<"The DIP DNS running on "<<argv[1]<<" will be used to published data."<<endl<<endl;
	} else {
		cout<<"You could pass on the command line the name of the DNS to use if different from default."<<endl<<endl;
	}

	// Create the publications
	DipPublication *pub[2];
	DipFactory *dip = Dip::create("TestPublisher");
	//The following call forces the selection for the DNS of your choice rather than the default (hardcoded in DIP),
	// or the one you have specified via the DIPNS environment variable.
	if (argc == 2)
		dip->setDNSNode(argv[1]);
	char itemName[2][11]={"test.pub1", "test.pub2"};
	ErrHandler errorHandler;

	for (int i=0; i < 2; i++){
		pub[i] = dip->createDipPublication(itemName[i], &errorHandler);
	}

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
			pub[0]->send((DipInt)(intVal + 1), time);
			pub[1]->send(*data, time);
			intVal+=1;
			floatVal+=0.1f;
			// simulates server accessing a publications data
			// source
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
