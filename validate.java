import java.io.*;
import java.util.regex.*;
import java.util.ArrayList;
import java.util.HashMap;

public class validate {
    public static void main(String args[]) throws Exception {
        String dump = "";
        String line;
        validate val = new validate();
        File file = new File("memorydump.dmp"); 
        BufferedReader br = new BufferedReader(new FileReader(file)); 

        while ((line = br.readLine()) != null) {
            dump = dump + line;
        }
        //System.out.println(dump); 

        val.Decrypt(dump);
    }

    public void Decrypt(String dump) {
        Regex re = new Regex();

    }

    public void printRecord() {
        
    }
}