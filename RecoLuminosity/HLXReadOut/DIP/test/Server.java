/**
* This server generates 1000 publications that are updated continuously.
* It also changes the qulity and the timestamp to have all the possible situations.
* Finally, it embeds a DIP client that subscribes to 1 publications.
*/
import cern.dip.*;

public class Server {
	DipData sentData;
	int sentQuality;
	String sentQualityReason;
	boolean continueSending = false;


	class StrSubscriptionHandler implements DipSubscriptionListener
	{
		public void handleMessage(DipSubscription subscription, DipData message)
		{
			boolean allOK = true;
			if (message.extractDataQuality() != sentQuality){
				System.out.println("Bad quality - extpected " + sentQuality + " got " + message.extractDataQuality());
				allOK=false;
			}

			if (!message.extractQualityString().equals(sentQualityReason)){
				System.out.println("Bad quality string - expected '" + sentQualityReason + "' got '" + message.extractQualityString()+"'");
				allOK=false;
			}

			if (message.extractDataQuality() != DipQuality.DIP_QUALITY_UNINITIALIZED){
				System.out.println("Received Data:-");
				String [] tags = message.getTags();
				for (int i = 0; i < tags.length; i++){
					int tagType = message.getValueType(tags[i]);
					try{
					switch(tagType){
						case DipData.TYPE_BOOLEAN:		System.out.println(tags[i] + " " + message.extractBoolean(tags[i]));break;
						case DipData.TYPE_BOOLEAN_ARRAY:System.out.print(tags[i] + " " + "[");
						{
							boolean [] val = message.extractBooleanArray(tags[i]);
							for (int arrayInd = 0; arrayInd < val.length; arrayInd++){
								System.out.print(" " + val[arrayInd]);
							}
						}
							System.out.println("]");break;
						case DipData.TYPE_BYTE:			System.out.println(tags[i] + " " + message.extractByte(tags[i]));break;
						case DipData.TYPE_BYTE_ARRAY:	System.out.print(tags[i] + " " + "[");
						{
							byte [] val = message.extractByteArray(tags[i]);
							for (int arrayInd = 0; arrayInd < val.length; arrayInd++){
								System.out.print(" " + val[arrayInd]);
							}
						}
							System.out.println("]");break;
						case DipData.TYPE_SHORT:		System.out.println(tags[i] + " " + message.extractShort(tags[i]));break;
						case DipData.TYPE_SHORT_ARRAY:	System.out.print(tags[i] + " " + "[");
						{
							short [] val = message.extractShortArray(tags[i]);
							for (int arrayInd = 0; arrayInd < val.length; arrayInd++){
								System.out.print(" " + val[arrayInd]);
							}
						}
							System.out.println("]");break;
						case DipData.TYPE_INT :			System.out.println(tags[i] + " " + message.extractInt(tags[i]));break;
						case DipData.TYPE_INT_ARRAY:	System.out.print(tags[i] + " " + "[");
						{
							int [] val = message.extractIntArray(tags[i]);
							for (int arrayInd = 0; arrayInd < val.length; arrayInd++){
								System.out.print(" " + val[arrayInd]);
							}
						}
							System.out.println("]");break;
						case DipData.TYPE_LONG:			System.out.println(tags[i] + " " + message.extractLong(tags[i]));break;
						case DipData.TYPE_LONG_ARRAY:	System.out.print(tags[i] + " " + "[");
						{
							long [] val = message.extractLongArray(tags[i]);
							for (int arrayInd = 0; arrayInd < val.length; arrayInd++){
								System.out.print(" " + val[arrayInd]);
							}
						}
							System.out.println("]");break;
						case DipData.TYPE_FLOAT :		System.out.println(tags[i] + " " + message.extractFloat(tags[i]));break;
						case DipData.TYPE_FLOAT_ARRAY:	System.out.print(tags[i] + " " + "[");
						{
							float [] val = message.extractFloatArray(tags[i]);
							for (int arrayInd = 0; arrayInd < val.length; arrayInd++){
								System.out.print(" " + val[arrayInd]);
							}
						}
							System.out.println("]");break;
						case DipData.TYPE_DOUBLE:		System.out.println(tags[i] + " " + message.extractDouble(tags[i]));break;
						case DipData.TYPE_DOUBLE_ARRAY:	System.out.print(tags[i] + " " + "[");
						{
							double [] val = message.extractDoubleArray(tags[i]);
							for (int arrayInd = 0; arrayInd < val.length; arrayInd++){
								System.out.print(" " + val[arrayInd]);
							}
						}
							System.out.println("]");break;
						case DipData.TYPE_STRING:		System.out.println(tags[i] + " " + message.extractString(tags[i]));break;
						case DipData.TYPE_STRING_ARRAY:	System.out.print(tags[i] + " " + "[");
						{
							String [] val = message.extractStringArray(tags[i]);
							for (int arrayInd = 0; arrayInd < val.length; arrayInd++){
								System.out.print(" " + val[arrayInd]);
							}
						}
							System.out.println("]");break;
					}
					}catch(Exception e){
						System.out.println("Data extraction failed for " + tags[i] + " of type " + tagType + " because " + e.getMessage());
					}
				}
			}

			if (allOK){
				System.out.println("OK");
			}
			continueSending = true;
		}

		public void disconnected(DipSubscription subscription, String reason)
		{
			System.out.println(subscription.getTopicName()+" is "+reason);
		}
		public void connected(DipSubscription subscription)
		{
			System.out.println(subscription.getTopicName()+" connected");
		}

		public void handleException(DipSubscription subscription, Exception ex){
			System.out.println("Exception handler for " + subscription.getTopicName() + " called because " + ex.getMessage());
		}
	}



	void go() throws DipException{

		//Create the publications
		DipFactory dip = Dip.create("TestPingLinux");



		StrSubscriptionHandler strHandler = new StrSubscriptionHandler();
		DipSubscription subStr = dip.createDipSubscription("dip.test.Linux.item1",strHandler);
		 DipPublicationErrorHandler pubErrorHandler = new DipPublicationErrorHandler(){
			 public void handleException(DipPublication publication, DipException ex){
				 System.out.println("Error handler for " + publication.getTopicName() + " called because " + ex.getMessage());
			 }
		 };




			DipPublication [] pub = new DipPublication[1000];
			for (int i = 0; i < pub.length; i++){
				String name = "dip.test.Linux.item"+i;
				System.out.println("Making publication " + name);
				pub[i] = dip.createDipPublication(name, pubErrorHandler);
			}




			 // set up vars who's values are to be sent
			 sentData = dip.createDipData();
			 int intVal = 0;

		try{

			 // send data via DIP
			sentQuality = DipQuality.DIP_QUALITY_UNINITIALIZED;
			sentQualityReason = "";
			 while (true){

				if ((intVal % 10) == 0){
					if (intVal == 0){
						sentQuality = DipQuality.DIP_QUALITY_UNINITIALIZED;
					} else {
						sentQuality = DipQuality.DIP_QUALITY_BAD;
					}
					sentQualityReason = "";
					for (int i = 0; i < pub.length; i++){
						pub[i].setQualityBad();
					}
				} else if ((intVal % 10) == 1){
					if (intVal == 1){
						sentQuality = DipQuality.DIP_QUALITY_UNINITIALIZED;
					} else {
						sentQuality = DipQuality.DIP_QUALITY_BAD;
					}
					sentQualityReason = "Its.broken!";
					for (int i = 0; i < pub.length; i++){
						pub[i].setQualityBad(sentQualityReason);
					}
		 		}  else if ((intVal % 10) == 2){
					if (intVal == 2){
						sentQuality = DipQuality.DIP_QUALITY_UNINITIALIZED;
					} else {
						sentQuality = DipQuality.DIP_QUALITY_UNCERTAIN;
					}
					sentQualityReason = "";
					for (int i = 0; i < pub.length; i++){
						pub[i].setQualityUncertain();
					}

				} else if ((intVal % 10) == 3){
					if (intVal == 3 ){
						sentQuality = DipQuality.DIP_QUALITY_UNINITIALIZED;
					} else {
						sentQuality = DipQuality.DIP_QUALITY_UNCERTAIN;
					}
					sentQualityReason = "I.think.its.broken!";
					for (int i = 0; i < pub.length; i++){
						pub[i].setQualityUncertain(sentQualityReason);
					}

				}  else {
					sentQuality = DipQuality.DIP_QUALITY_GOOD;
					sentQualityReason = "";

					sentData.insert("string",Integer.toString(intVal));

					sentData.insert("bool",((intVal%2) == 1));
					 boolean [] ba = new boolean[2];
					 ba[0] = ((intVal%2) == 1);
					 ba[1] = ((intVal%2) == 0);
					 sentData.insert("boolArray",ba);
					 sentData.insert("byte",(byte)intVal);
					 {
						 byte[] val = new byte[11];
						 for (int i = 0; i < val.length; i++){
							 val[i] = (byte)(intVal+i);
						 }
						 sentData.insert("byteArray",val);
					 }

					 sentData.insert("string",Integer.toString(intVal));

					 sentData.insert("short",(short)intVal);
					 {
						 short[] val = new short[10];
						 for (int i = 0; i < val.length; i++){
							 val[i] = (short)(intVal+i);
						 }
						 sentData.insert("shortArray",val);
					 }

					 {
						 String[] val = new String[10];
						 for (int i = 0; i < val.length; i++){
							 val[i] = Integer.toString(intVal+i);
						 }
						 sentData.insert("stringArray",val);
					 }
					 sentData.insert("int",intVal);
					 {
						 int[] val = new int[10];
						 for (int i = 0; i < val.length; i++){
							 val[i] = (intVal+i);
						 }
						 sentData.insert("intArray",val);
					 }
					 sentData.insert("long",(long)intVal);
					 {
						 long[] val = new long[10];
						 for (int i = 0; i < val.length; i++){
							 val[i] = (intVal+i);
						 }
						 sentData.insert("longArray",val);
					 }
					 sentData.insert("float",(float)intVal);
					 {
						 float[] val = new float[14];
						 for (int i = 0; i < val.length; i++){
							 val[i] = (float)(intVal+i);
						 }
						 sentData.insert("floatArray",val);
					 }
					 sentData.insert("double",(double)intVal);
					 {
						 double[] val = new double[2];
						 for (int i = 0; i < val.length; i++){
							 val[i] = (double)(intVal+i);
						 }
						 sentData.insert("doubleArray",val);
					 }
					 sentData.insert("extraString","exra " + Integer.toString(intVal));
					 {
						 String[] val = new String[20];
						 for (int i = 0; i < val.length; i++){
							 val[i] = "extra " + Integer.toString(intVal+i);
						 }
						 sentData.insert("extraStringArray",val);
					 }
					for (int i = 0; i < pub.length; i++){
						pub[i].send(sentData, new DipTimestamp());
                        if ((i % 4) == 0){
                         	try{
								pub[0].send(intVal, new DipTimestamp());
                        	} catch (Throwable e){
								System.out.println(e.getMessage());
                        	}
                        }
					}
				}

				 intVal+=1;
				 //do{
					 try{
						 // simulates server accessing a publications data
						 // source here.
						 Thread.sleep(10);
					 }catch(InterruptedException e){
					 System.out.println("Send pause failed");
				 }
				 //}while (continueSending == false);
				continueSending = false;
			 }
		} catch (DipException e){
			System.out.println("Failed to send data because " + e.getMessage());
			e.printStackTrace();
		}

			 // shutdown
			 //dip.destroyDipPublication(pub[0]);
			 //dip.destroyDipPublication(pub[1]);
	}


	public static void main(String args[]){
		System.out.println("*********************************************");
		Server s = new Server();
		try{
		s.go();
		} catch(DipException e){
			e.printStackTrace();
		}
	}
}
