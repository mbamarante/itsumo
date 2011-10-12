/*
 * Created on Mar 9, 2006 by apetcu
 * Project Public
 * Package frodo.utils
 */
package frodo.utils;

import java.io.FileWriter;

public class FileUtilities {

	public FileUtilities() {
		super();
		// TODO Auto-generated constructor stub
	}

	public static void writeStringToFile(String tow, String resultFile, boolean append) {
		try {
			FileWriter fw = new FileWriter(resultFile, append);
			fw.write(tow);
			fw.close();
		} catch (Exception e) {
			System.out.println("Exception while saving String=[" + tow + "] to file:");
			e.printStackTrace();
			System.exit(1);
		}
	}

	public static void writeStringToFile(String tow, String resultFile) {
		writeStringToFile(tow, resultFile, false);
	}

	public static void appendStringToFile(String tow, String resultFile) {
		writeStringToFile(tow, resultFile, true);
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub

	}

}
