import java.io.*;
import java.util.regex.*;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Collections;

public class validate {
    private static ArrayList<String> card_no = new ArrayList<String>();
    private static ArrayList<String> names = new ArrayList<String>();
    private static ArrayList<String> misc = new ArrayList<String>();

    public static void main(String args[]) throws Exception {
        String dump = "";
        String line;
        validate val = new validate();
        File file = new File("memorydump.dmp"); 
        BufferedReader br = new BufferedReader(new FileReader(file)); 

        while ((line = br.readLine()) != null) {
            dump = dump + line;
        }
        val.Decrypt(dump);

        System.out.println("There is " + card_no.size() + " track I record in the memory data");
        for(int i = 0; i < card_no.size(); i++)
        {
            System.out.println("<Information of the " + (i + 1) + " entry>");
            System.out.println("Cardholder's Name: " + names.get(i));
            System.out.println("Card Number: " + card_no.get(i));
            System.out.println("Expiration Date: " + misc.get(i).substring(2, 4) + "/20" +misc.get(i).substring(0, 2) );
            System.out.println("CVC Number: " + misc.get(i).substring(4));
        }
    }

    public void Decrypt(String dump) {
        Pattern card_no_re = Pattern.compile("(?<=%B)\\d{16}");
        Pattern name_re = Pattern.compile("(?<=\\^)[\\D]+?(?=\\^)");
        Pattern misc_re = Pattern.compile("(?<=\\=)\\d{7}");

        Matcher match = card_no_re.matcher(dump);

        while(match.find())
        {
           String data = match.group();
        //    System.out.println(data.charAt(0));
           if(data.charAt(0) != '1' && !card_no.contains(data)){
               card_no.add(data);
           }
        }

        match = name_re.matcher(dump);

        while(match.find())
        {
            String data = match.group();
            if(!names.contains(data)){
                names.add(data);
            }
        }

        match = misc_re.matcher(dump);

        while(match.find())
        {
            String data = match.group();
            if(!misc.contains(data)){
                misc.add(data);
            }
        }
        Collections.reverse(misc);

    }

    public void printRecord() {
        
    }
}