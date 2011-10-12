/*
 * Logger.java
 *
 * Created on October 31, 2002, 1:46 PM
 */

package frodo.utils;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.PrintWriter;

/**
 *
 * @author  apetcu
 */
public class Logger {
	boolean logs;

	PrintWriter logger;

	boolean toSysOut;

	/** Creates a new instance of Logger */
	public Logger(String name) {
		this(name, true);
	}

	public Logger(String name, boolean logs) {
		try {
			logger = new PrintWriter(new FileOutputStream(name), true);
		} catch (IOException ioe) {
			System.out.println("Error while opening logfile.");
			ioe.printStackTrace();
		}
		this.logs = logs;
	}

	public void print(String s, int prio) {
		if (logs)
			logger.print(s);
		else if (prio > 1)
			logger.print(s);
	}

	public void print(String s) {
		this.print(s, 0);
	}

	/*
	 public void log(String s) {
	 logger.print(s);
	 }
	 public void logln(String s) {
	 }
	 */
	public void println(String message) {
		this.println(message, 0);
	}

	public void println(String s, int prio) {
		if (logs)
			if (toSysOut)
				System.out.println(s);
			else
				logger.println(s);
		else if (prio > 1)
			logger.println(s);
	}

	public void flush() {
		if (logs)
			logger.flush();
	}

	public void close() {
		logger.close();
	}

	/**
	 * @return Returns the logger.
	 * 
	 * @uml.property name="logger"
	 */
	public PrintWriter getLogger() {
		return logger;
	}

	/**
	 * @param logger The logger to set.
	 * 
	 * @uml.property name="logger"
	 */
	public void setLogger(PrintWriter logger) {
		this.logger = logger;
	}

	public boolean isToSysOut() {
		return toSysOut;
	}

	public void setToSysOut(boolean toSysOut) {
		this.toSysOut = toSysOut;
	}

}
