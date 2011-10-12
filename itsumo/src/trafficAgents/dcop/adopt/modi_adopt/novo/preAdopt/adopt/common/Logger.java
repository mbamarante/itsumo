/*
 *    Logger.java
 *    Author: Jay Modi
 *    Date: Oct  21 2001
 *
 *    Simple Class for holding logging utilities.
 *    
 */


package adopt.common;

import java.io.*;
import java.util.*;

public class Logger{
  
  PrintWriter nLog = null;

  public Logger(PrintWriter l){
    nLog = l;
  }
  
  public void printToLog(String s){
    nLog.print(s);
    nLog.flush();
    Utility.Dprintc(s, Utility.TRACE_RIDICULOUS_LEVEL);
  }
  
  public void printToLogln(String s){
    nLog.println(s);
    nLog.flush();
    Utility.Dprint(s, Utility.TRACE_RIDICULOUS_LEVEL);
  }
}
