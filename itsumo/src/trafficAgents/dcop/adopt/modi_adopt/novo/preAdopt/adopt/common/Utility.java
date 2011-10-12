/*
 *    Utility.java
 *    Author: Jay Modi
 *    Date: Aug 23 2000
 *
 *    Simple Class for holding misc utilities.
 *    
 */
package adopt.common;

import java.io.*;
import java.util.*;
import java.net.*;

public class Utility {
    
  public static int debug_level = 3;

  public static int DEBUG = 1;
  public static int ERROR_LEVEL = 0;
  public static int MSG_LEVEL = 5;
  public static int MSG_LEVEL2 = 6;
  public static int MSG_LEVEL3 = 8;
  public static int TRACE_FUNCTION_LEVEL = 10;
  public static int TRACE_FUNCTION_EXECUTION_LEVEL = 20;
  public static int TRACE_RIDICULOUS_LEVEL = 30;
  public static int DEFAULT_LEVEL = 8;

  public static double THRESHOLD = 0.1;
  public static long starttime =  System.currentTimeMillis();


  public static void init(int level){
    debug_level = level;
  }
  /* Return current time as a string for printing */
  public static String TimeString()
  {
    Date d = new Date(System.currentTimeMillis());
    return (d.toString());
    
  }


  public static String TimeStringMS()
  {
    long result =  System.currentTimeMillis() - starttime;
    return new Long(result).toString();
  }

  public static long TimeMS()
  {
    long result =  System.currentTimeMillis() - starttime;
    return result;
  }

  public static long TimeSec()
  {
    long result =  System.currentTimeMillis() - starttime;
    return result/1000;
  }


  public static void Dprint(String msg, int level)
  {
    if(level < debug_level)
      System.out.println(msg);
  }

  public static void Dprint(String msg) { Dprint(msg, DEFAULT_LEVEL); }

  /* print string without '\n' */
  public static void Dprintc(String msg, int level)
  {
    if(level < debug_level)
      System.out.print(msg);
  }

  public static void Dprintc(String msg){ Dprintc(msg, DEFAULT_LEVEL); }

  public static void main(String[] args){
    System.out.println(Utility.TimeStringMS());
  }

}
