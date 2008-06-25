/**
 * Simple demo client that subscribes to 2 data items
 * dip/test/publication_1 
 * dip/test/publication_1 
 * Data from these two publications is diplayed on the console as it arrives.
 * The demo prints connect/disconnect messages as these events occur.
 * */

import cern.dip.*;
import java.util.Date;

public class Client {

	DipSubscription sub[] = new DipSubscription[2];

	/**
	 * Start the client
	 * */
	public static void main(String args[]){
		new Client();

		while(true){
			try{
				Thread.sleep(10000);
			} catch (InterruptedException ie){}
		}
	}


	/**
	 * set up the subscriptions to test.item1 and test.item2
	 * */
	public Client(){
		Date theDate = new Date();	
		DipFactory dip = Dip.create("Client" + theDate);
		String itemName[]= new String[]{"dip/test/publication_1", "dip/test/publication_2"};
		GeneralDataListener handler = new GeneralDataListener();
		try{
		for (int i=0; i < 2; i++){
		   sub[i] = dip.createDipSubscription(itemName[i], handler);
		}
		} catch (DipException de){
			de.printStackTrace();
			System.exit(1);
		}
	}

	/**
	 * handler for connect/disconnect/data reception events
	 * Nested class
	 * */
	class GeneralDataListener implements DipSubscriptionListener{

		/**
		 * handle changes to subscribed to publications
		 * Simply prints the contents of the received data.
		 * @param subscription - the subsciption to the publications thats changed.
		 * @param message - object containing publication data
		 * */
		public void handleMessage(DipSubscription subscription, DipData message){
			if (message.extractDataQuality() == DipQuality.DIP_QUALITY_UNINITIALIZED){
				return;
			}

			if (subscription == sub[0]){
				try{
					System.out.println("New values for dip/test/publication_1 are " + message.extractInt("field1") + " and " + message.extractFloat("field2") + " and " + message.extractString("field3"));
				} catch(TypeMismatch e){
					System.out.println("dip/test/publication_1 data is of unexpected type");
				} catch(BadParameter e){
					System.out.println("dip/test/publication_1 has bad fields");
				}

			} else if (subscription == sub[1]){
				try{
					System.out.println("New values for dip/test/publication_2 are " + message.extractInt("field1") + " and " + message.extractFloat("field2") + " and " + message.extractString("field3"));
				} catch(TypeMismatch e){
					System.out.println("dip/test/publication_2 data is of unexpected type");
				} catch(BadParameter e){
					System.out.println("dip/test/publication_2 has bad fields");
				}
			}
		}


		/**
		 * called when a publication subscribed to is available.
		 * @param arg0 - the subsctiption who's publication is available.
		 * */
		public void connected(DipSubscription arg0) {
			System.out.println("Publication source "+arg0.getTopicName() +" available");
		}


		/**
		* called when a publication subscribed to is unavailable.
		* @param arg0 - the subsctiption who's publication is unavailable.
		* @param arg1 - string providing more information about why the publication is unavailable.
		* */
		public void disconnected(DipSubscription arg0, String arg1) {
			System.out.println("Publication source "+arg0.getTopicName() +" unavailable");
		}


		public void handleException(DipSubscription ds, Exception de){
			System.out.println("Publication source "+ds.getTopicName() +" has error " + de.getMessage());
		}
	}
}
