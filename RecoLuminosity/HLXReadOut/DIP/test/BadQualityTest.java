/**
* Written by Beharrell
*
* This server creates a publication with an unititialized quality.
* There is no client here.
*/

import cern.dip.*;

public class BadQualityTest {
    DipData sentData;
    int sentQuality;
    String sentQualityReason;
    boolean continueSending = false;




    void go() throws DipException {
        DipFactory dip = Dip.create("TestPing");
        DipPublicationErrorHandler pubErrorHandler = new DipPublicationErrorHandler() {
            public void handleException(DipPublication publication, DipException ex) {
                System.out.println("Error handler for " + publication.getTopicName() + " called because " + ex.getMessage());
            }
        };
        DipPublication[] pub = new DipPublication[1];
        for (int i = 0; i < pub.length; i++) {
            String name = "dip.test.item" + i;
            System.out.println("Making publication " + name);
            pub[i] = dip.createDipPublication(name, pubErrorHandler);
        }
        // set up vars who's values are to be sent
        sentData = dip.createDipData();
        int intVal = 0;
        try {
            while (true) {
                // send data via DIP
                sentQuality = DipQuality.DIP_QUALITY_UNINITIALIZED;
                sentQualityReason = "Its.broken!";
                for (int i = 0; i < pub.length; i++) {
                    sentQualityReason = "Its.broken! " + (intVal%2);
                    pub[i].setQualityBad(sentQualityReason);
                    System.out.println("Sent quality reason " + sentQualityReason);
                    Thread.sleep(1000);
                }
                intVal++;
            }
        } catch (Exception e) {
            System.out.println("Failed to send data because " + e.getMessage());
            e.printStackTrace();
        }
        // shutdown
        dip.destroyDipPublication(pub[0]);
        dip.destroyDipPublication(pub[1]);
    }

    public static void main(String args[]) {
        System.out.println("*********************************************");
        BadQualityTest s = new BadQualityTest();
        try {
            s.go();
        } catch (DipException e) {
            e.printStackTrace();
        }
    }
}
