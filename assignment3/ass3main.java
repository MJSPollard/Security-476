import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.math.BigInteger;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.text.DecimalFormat;
import java.util.Arrays;

public class ass3main {
	public static String[] passwords = { "6f047ccaa1ed3e8e05cde1c7ebc7d958", "275a5602cd91a468a0e10c226a03a39c",
			"b4ba93170358df216e8648734ac2d539", "dc1c6ca00763a1821c5af993e0b6f60a", "8cd9f1b962128bd3d3ede2f5f101f4fc",
			"554532464e066aba23aee72b95f18ba2" };
	final static long startTime = System.nanoTime();

	// Method to handle converting a password string into MD5 encryption.
	public static String convertToMD5(String input) {
		try {
			MessageDigest m = MessageDigest.getInstance("MD5");
			byte[] digestMessage = m.digest(input.getBytes());
			BigInteger arrayNum = new BigInteger(1, digestMessage);
			String hashedPassword = arrayNum.toString(16);
			while (hashedPassword.length() < 32) {
				hashedPassword = "0" + hashedPassword;
			}
			return hashedPassword;
		} catch (NoSuchAlgorithmException e) {
			throw new RuntimeException(e);
		}
	}

	// Method to print password matches.
	public static void printOutcomes(String hashedPassword, String crackedPassword, double time, int count) {
		System.out.println("The password for hash value " + hashedPassword + " is " + crackedPassword + ",");
		System.out.println("This program took: " + new DecimalFormat("#.##").format(time)
				+ " seconds to recover the password. At count: " + count);
		System.out.println("------------------------------------------------------------------------------");
	}

	// Driver
	public static void main(String args[]) throws NoSuchAlgorithmException {
		System.out.println("Running program... this could take up to 30 minutes.");
		System.out.println("Parsing through 1.5 billion passwords from crackstations password dictionary");
		System.out.println(
				"Can be found at: https://crackstation.net/crackstation-wordlist-password-cracking-dictionary.htm");
		System.out.println("------------------------------------------------------------------------------");
		BufferedReader reader;
		int count = 0;
		try {
			// Checks empty string
			if (Arrays.asList(passwords).contains(convertToMD5(""))) {
				System.out.println("First password is empty string, but probably not gonna happen.");
				count++;
			}

			reader = new BufferedReader(new FileReader("/Users/johnson/Desktop/crackstation.txt"));
			// Read in first line of string.
			String attemptCrack = reader.readLine();
			int crackCount = 0;
			// Loop through every password in crackstation.txt
			while (attemptCrack != null) {
				// Converting current password to MD5 encryption.
				String hashAttempt = convertToMD5(attemptCrack);
				// Comparing this to our array of hashed passwords.
				if (Arrays.asList(passwords).contains(hashAttempt)) {
					long duration = System.nanoTime() - startTime;
					double time = ((double) duration / 1000000000);
					printOutcomes(hashAttempt, attemptCrack, time, count);
					crackCount++;
				}
				// Break when all passwords are accounted for.
				if (crackCount == 6) {
					System.out.println("All passwords cracked.");
					break;
				}
				attemptCrack = reader.readLine();
				count++;
			}
			reader.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
