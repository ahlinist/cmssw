import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

import cern.dip.*;


class ValidatorErrHandler implements DipPublicationErrorHandler{
	public void handleException(DipPublication dp, DipException de){
		System.out.println("Publication source "+dp.getTopicName() +" has error " + de.getMessage());
	}
}

/**
 * @author mdutour
 * This class is used to verify the behavior of the timestamp fix provided in the DIP API Manager.
 * It generates updates on demand waiting for the User to type something on the command line.
 */
public class Validator {

	public static int noPublications;

	public static void main(String args[]){

		// Create the publications

		String temp = System.getProperty("noPubs");
		if (null == temp) {
			System.out.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
			System.out.println("You can specify XXX DIP publications by typing: java -DnoPubs=XXX Server");
			System.out.println("Nothing specified, default is 100.");
			System.out.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
			noPublications = 100;
		} else {
			noPublications = (new Integer(temp)).intValue();
			System.out.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
			System.out.println("Understood, let `s go for " + noPublications + " DIP publications...");
			System.out.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
		}
		DipPublication pub[] = new DipPublication[noPublications];
		DipFactory dip = Dip.create("TestPublisher");
		//String itemName[]= new String[noPublications];
		try{
			for (int i=0; i < noPublications; i++){
				   String itemName = "dip/test/publication_" + i;
				   pub[i] = dip.createDipPublication(itemName, new ValidatorErrHandler());
			}
		} catch (DipException de){
				de.printStackTrace();
				System.exit(1);
		}
		// set up vars who's values are to be sent
		int intVal = 0;
		float floatVal = 0.0f;
		String stringValue = intVal + "as a String.";

		// send data via DIP
		while (true){
			try{
				//Waiting for the user to type something and press return.
		          System.out.println("Waiting for you to type something.\n");
			      BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
			      //  read the username from the command-line; need to use try/catch with the
			      //  readLine() method
			      //try {
			      br.readLine();
			      //} catch (IOException ioe) {
			      //   System.out.println("IO error trying to read your name!");
			      //   System.exit(1);
			      //}
				for (int i=0; i < noPublications; i++){
					DipData data = dip.createDipData();
					data.insert("field1",intVal);
					data.insert("field2",floatVal);
					data.insert("field3",stringValue);
					pub[i].send(data, new DipTimestamp());
					intVal+=1;
					floatVal+=1.0;
					stringValue = intVal + "as a String.";
				}
				System.out.println("OK, just sent " + noPublications + " values... ");


				if (intVal >= 1000000) {
					System.out.println("Keep values below 1000000 for readability...");
					intVal = 0;
					floatVal = 0.0f;
				}
			} catch (DipException e){
				System.out.println("Failed to send DIP data.");
			} catch (IOException e){
				System.out.println("Failed to send DIP data.");
			}
		}

		// shutdown
		//dip.destroyDipPublication(pub[0]);
		//dip.destroyDipPublication(pub[1]);
	}
}
