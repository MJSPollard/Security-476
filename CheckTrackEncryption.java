import java.io.*;
import java.util.regex.*;
import java.util.ArrayList;

// run with javac CheckTrackEncryption.java && java CheckTrackEncryption
public class CheckTrackEncryption {
    /**
     * Read in the File and call other functions.
     */
    public static void main(String args[]) throws Exception {
        String memDump = "";
        String line;
        CheckTrackEncryption cte = new CheckTrackEncryption();
        File file = new File("memorydump.dmp"); //assuming this file is in the $PWD
        BufferedReader br = new BufferedReader(new FileReader(file));

        // read file into string
        while ((line = br.readLine()) != null) {
            memDump = memDump + line;
        }
        cte.printTrackRecords(cte.searchForTrackData(memDump));
    }


    /**
     * Extract Track I formatted data if there is any.
     * @param memDump, the PoS RAM data.
     * @return The ArrayList of Track 1 data if there is any.
     */
    public ArrayList searchForTrackData(String memDump) {
        // The below url shows format of track I data.
        // http://sagan.gae.ucm.es/~padilla/extrawork/magexam1.html

        ArrayList<String> Track_I_List = new ArrayList<String>();

        // Track I pattern format, could fail if cardholders name contains numbers
        Pattern isTrack1 = Pattern.compile("%B\\d{16}(\\^[^0-9]+?\\/[^0-9]+?\\^[0-9]+\\?;)");

        // Search for name in line data.
        Matcher match = isTrack1.matcher(memDump);
        while(match.find()) {
            String data = match.group();
            Track_I_List.add(data);
        }
        return Track_I_List;
    }
    /**
     * Print out the track I data appropriately.
     * @param records, the list of track I data.
     */
    public void printTrackRecords(ArrayList records) {
        String name = "";
        String cardNum = "";
        String exp = "";
        String cvc = "";
        boolean nameFlag = false;

        System.out.println("\nThere is " + records.size() + " track I record(s) in the memory data\n");
        for(int i = 0; i < records.size(); i++) { //loop through all track I records
            String trackRecord = (String) records.get(i);
            System.out.println("<Information for record number: " + (i + 1) + ">");
            for(int j = 2; j < trackRecord.length(); j++) { // loop through track string
                if (j < 18) { // parse card num
                    if(((j - 1) % 4) == 0) {
                        cardNum = cardNum + trackRecord.charAt(j) + " ";
                    } else {
                        cardNum = cardNum + trackRecord.charAt(j);
                    }
                } else if (!nameFlag) { // parse cardholder name
                    if(trackRecord.charAt(j) == '^' && j != 18) {
                        nameFlag = true;
                    } else if (j > 18) {
                        if(trackRecord.charAt(j) != '/') {
                            name = name + trackRecord.charAt(j);
                        }
                    }
                } else { //parse expiration date and cvc
                    if(exp.length() < 4) {
                        exp = exp + trackRecord.charAt(j);
                    } else if (cvc.length() < 3) {
                        cvc = cvc + trackRecord.charAt(j);
                    } else {
                        break;
                    }
                }
            }
            System.out.println("Cardholder's Name: " + name);
            System.out.println("Card Number: " + cardNum);
            System.out.println("Expiration Date: " + exp.substring(2, 4) + "/20" + exp.substring(0, 2));
            System.out.println("CVC Number: " + cvc + "\n");
            name = "";
            cardNum = "";
            exp = "";
            cvc = "";
            nameFlag = false;
        }
    }
}