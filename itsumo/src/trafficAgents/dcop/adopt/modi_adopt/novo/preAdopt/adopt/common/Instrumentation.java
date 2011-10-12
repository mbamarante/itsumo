/*
 *    Instrumentation.java
 *    Author: Jay Modi
 *    Date: Nov 07 2002
 *    Simple Class for doing timing instrumentation.
 *    
 */
package adopt.common;

import java.io.*;
import java.util.*;
import java.net.*;

public class Instrumentation {

  long start=0;
  long end=0;
  boolean print = false;
  int lineLen = 50;
  long total=0;
  int numAdds=0;

  public Instrumentation(){

  }

  public static String fillerString(String s){
    int diff = 50 - s.length();
    StringBuffer sb = new StringBuffer();
    for(int i=0;i<diff;i++){
      sb.append("-");
    }
    return sb.toString();
  }


  public void start(String s, long t){
    start = t;
    if(print)
      System.out.println(s + fillerString(s) + t) ;
  }

  public void end(String s, long t){
   end = t;
   if(print)
      System.out.println(s + fillerString(s) + t) ;
  }

  public void timeElapsed(String s){
    double elap = ((double)end - start)/1000;

    if(print)
      System.out.println(s + fillerString(s) + elap + " secs") ;
  }
  
  public void add(){
    double elap = ((double)end - start);
    total += elap;
    numAdds++;
  }

  public void timeAverage(String s){
    double avg = ((double)total)/(numAdds*1000);

    if(print)
      System.out.println(s + fillerString(s) + avg + " secs") ;
  }


  public static void instPrint(String s){
    System.out.println(s) ;
  }

  public static void main(String[] args){
    Instrumentation itn = new Instrumentation();
    itn.start("hello", Utility.TimeMS());
    itn.end("hi", Utility.TimeMS());
    itn.timeElapsed("    time elapsed: ");
  }

}
