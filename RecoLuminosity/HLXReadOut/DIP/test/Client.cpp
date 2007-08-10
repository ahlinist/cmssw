#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include "Dip.h"
#include "DipSubscription.h"
#include <string>

using namespace std;

/**
 * Simple demo client that subscribes to 2 data items
 * test.item1 - int
 * test.test2 - structure with fields field1(int) and field2(float)
 * Data from these two publications is diplayed on the console as it arrives.
 * The demo prints connect/disconnect messages as these events occur 
 * */
class Client {	
	private: 
		// hold reference to subscription objects
		DipSubscription *sub[2];

		// DIP object
		DipFactory *dip;
	
	public:
		
		/**
		* set up the subscriptions to test.item1 and test.item2
		* */
		Client(){
			DipTimestamp aTimeStamp;
			long someRandomness = aTimeStamp.getAsMillis();
			string clientName = "Client";
			clientName += someRandomness;
			dip = Dip::create(clientName.c_str());
			char itemName[][20]={"dip/test/pub1", "dip/test/pub2"};
			GeneralDataListener *handler = new GeneralDataListener(this);

			for (int i=0; i < 2; i++){
				sub[i] = dip->createDipSubscription(itemName[i], handler);
			}
		}
	
		
		~Client(){
			dip->destroyDipSubscription(sub[0]);
			dip->destroyDipSubscription(sub[1]);
			//delete dip;
		}

		/**
		* handler for connect/disconnect/data reception events
		* Nested class
		* */
		class GeneralDataListener:public DipSubscriptionListener{
		private:

			// allow us to access subscription objects
			Client * client;
		
		public:
			GeneralDataListener(Client *c):client(c){};

			/**
			* handle changes to subscribed to publications
			* Simply prints the contents of the received data.
			* @param subscription - the subsciption to the publications thats changed.
			* @param message - object containing publication data
			* */
			void handleMessage(DipSubscription *subscription, DipData &message){
				if (message.extractDataQuality() == DIP_QUALITY_UNINITIALIZED){
					return;
				}

				if (subscription == client->sub[0]){
					try{
						printf("New value for test.item1 is %d\n", message.extractInt());
					} catch(DipException e){
						printf("dip/test/item1 data is of unexpected type\n");
					}
				} else if (subscription == client->sub[1]){
					try{
						int iv = message.extractInt("field1");
						float fv = message.extractFloat("field2");
						printf("New values for dip/test/item2 are %d and %f\n",iv,fv);
					} catch(DipException e){
						printf("test.item2 has problem %s\n", e.what());
					} 
				}
			}
	
		
			/**
			 * called when a publication subscribed to is available.
			 * @param arg0 - the subsctiption who's publication is available.
			* */
			void connected(DipSubscription *arg0) {
				printf("Publication source %s available\n", arg0->getTopicName());
			}

		
			/**
			* called when a publication subscribed to is unavailable.
			* @param arg0 - the subsctiption who's publication is unavailable.
			* @param arg1 - string providing more information about why the publication is unavailable.
			* */
			void disconnected(DipSubscription *arg0, char *arg1) {
				printf("Publication source %s unavailable\n", arg0->getTopicName());
			}

			void handleException(DipSubscription* subscription, DipException& ex){
				printf("Subs %s has error %s\n", subscription->getTopicName(), ex.what());
			}

		};
		friend class GeneralDataListener;
};


/**
* Start the client 
* */
int main(const int carg, char ** varg){
	Client *client = new Client();
		
	while(true){
#ifdef WIN32
			Sleep(10000L);
#else
			sleep(1);
#endif
	}

	return(0);
}
