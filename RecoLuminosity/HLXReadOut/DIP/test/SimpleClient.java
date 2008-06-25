/*
 * Created on Aug 18, 2004
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */

import cern.dip.*;

/**
 * @author beharrel
 *
 * To change the template for this generated type comment go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
class SubListener implements DipSubscriptionListener{

	/* (non-Javadoc)
	 * @see cern.dip.DipSubscriptionListener#connected(cern.dip.DipSubscription)
	 */
	public void connected(DipSubscription subscription) {
		System.out.println("The SimpleClient is connected.");

	}

	/* (non-Javadoc)
	 * @see cern.dip.DipSubscriptionListener#disconnected(cern.dip.DipSubscription, java.lang.String)
	 */
	public void disconnected(DipSubscription subscription, String reason) {
		System.out.println("The SimpleClient is disconnected.");

	}

	/* (non-Javadoc)
	 * @see cern.dip.DipSubscriptionListener#handleException(cern.dip.DipSubscription, java.lang.Exception)
	 */
	public void handleException(DipSubscription subscription, Exception ex) {
		System.out.println("An exception was received through handleException.");

	}

	/* (non-Javadoc)
	 * @see cern.dip.DipSubscriptionListener#handleMessage(cern.dip.DipSubscription, cern.dip.DipData)
	 */
	public void handleMessage(DipSubscription subscription, DipData message) {
		System.out.println("Received a massage through handleMessage.");

	}

}



public class SimpleClient {

	public static void main(String[] args) {
		DipFactory df = Dip.create("SimpleClient");
		
		try{
			DipSubscription sub = df.createDipSubscription("dip/test/item1", new SubListener());
			sub.requestUpdate();
			df.destroyDipSubscription(sub);
		} catch (DipException de){
			de.printStackTrace();
		}

		while (true) {
			try {
			Thread.sleep(10);
			} catch (InterruptedException e) {
				System.out.println("Main thread interupted.");
			}
		}
		
	}
}
