/*
 *    Algorithm.java
 *    Author: Jay Modi
 *    Date: Nov 12 2002
 *
 *    sub-class this for implementing your favorite algorithm.
 *
 */

package adopt.solver;

import adopt.problem.*;
import adopt.common.*;

import javax.swing.event.EventListenerList;
import java.util.*;
import java.io.*;

public abstract class Algorithm implements MessageSender {

  /* Problem: contains list of variables */
  Problem pbm;
  int localAgentID;

  /* use mSender to send/receive messages from other agents */
  MessageSender mSender;

  Logger nLog = null;

  boolean breakFlag;

  /* if true, messages get lost */
  public boolean msgLossFlag = false;
  /* lose every nth message */
  public int lossRate = -1;
  /* lose message with probablity n */
  public int lossRate2 = 0;
  int lossCounter = 0;

  /* approximation bound */
  public static int BOUND = 0;

  int MAXCYC = 100000;
  boolean iWantToTerminate = false;
  /* time of termination for this thread */
  long timeEnd=0;
  /* number of internal execution cycles */
  int cycleCnt=0;
  /* count of number of received messages */
  int numMsgsRcvd = 0;
  /* count time since last report of variable values to Master Thread*/
  long lastReportTime =0;
  /* how often to report variable values to Master Thread (ms)*/
  public static long reportInterval = 10000;

  public abstract void run();
  public abstract void handleOneMsg(Message m);

  public Algorithm(int agentID, Problem p, MessageSender mSndr){

    /* communication set up */
    mSender = mSndr;
    localAgentID = agentID;
    pbm = p;

    /* debugging set up */
    Properties properties = System.getProperties();
    String agentIDS = properties.getProperty("AdoptDebug");

    if (agentIDS != null) {
      int dl = new Integer(agentIDS).intValue();
      Utility.init(dl);
      if(dl > 20)
	breakFlag = true;
    }
    else
      breakFlag = false;
  }

  public void setLogger(Logger l){
    nLog = l;
  }


  /* Send message 'msg' to 'aname'
     'cntFlag': count this message in the final stats? */
  public void sendMessage(Message m, String aname, boolean cntFlag){
    mSender.sendMessage(m, aname, cntFlag);
    LogMsg_SEND(m, localAgentID);
  }

  public Message getMessage(String aname){
    Message result =  mSender.getMessage(aname);
    LogMsg_RCVD(result, localAgentID);
    return result;
  }

  /**
   * Method to log all messages sent.
   */
  public static void LogMsg_SEND(Message msg, int myid)
  {
    String msgstring =  msg.rawMessage();

    Utility.Dprint("", Utility.MSG_LEVEL3);
    Utility.Dprint(Utility.TimeStringMS() + " AGENT " + myid +
		   "-> SEND to AGENT " + msg.destAgent, Utility.MSG_LEVEL3);
    Utility.Dprint("    MSG: " + msgstring, Utility.MSG_LEVEL3);
    Utility.Dprint("", Utility.MSG_LEVEL3);
  }

  /**
   * Method to log all messages recieved.
   */
  public static void LogMsg_RCVD(Message msg, int myid)
  {
    String msgstring =  msg.rawMessage();
    Utility.Dprint("", Utility.MSG_LEVEL3);
    Utility.Dprint(Utility.TimeStringMS() +" AGENT " + myid + "-> RCVD from AGENT " + msg.sourceAgent,
		   Utility.MSG_LEVEL3);
    Utility.Dprint("    MSG: " + msgstring, Utility.MSG_LEVEL3);
    Utility.Dprint("", Utility.MSG_LEVEL3);
  }


  /* lose message with a given probablity */
  public boolean timeToLoseMsg2(){
    double d = Math.random();
    if ( 100*d < lossRate2)
      return true;
    else
      return false;
  }

  /* lose every n'th message */
  public boolean timeToLoseMsg(){

    if(msgLossFlag){
      if(lossRate == lossCounter){
	lossCounter = 0;
	return true;
      }
      else
	lossCounter++;
    }
    return false;
  }


  /* parse msg and return new Context */
  public Context parseContext(String msg){

    Utility.Dprint("Entering parseContext()", Utility.TRACE_RIDICULOUS_LEVEL);
    Utility.Dprint("  msg: " + msg, Utility.TRACE_RIDICULOUS_LEVEL);

    StringTokenizer stok = new StringTokenizer(msg);
    Context vw = new Context();

    while(stok.hasMoreTokens()){
      String token = stok.nextToken();
      Variable v = (Variable) pbm.getVariableFromUniqueVarName(token);
      token = stok.nextToken();
      Value val = v.getValue(token);
      vw.addVarToContext(v, val);
    }

    Utility.Dprint("  returning Context: " + vw.toString(),
		   Utility.TRACE_RIDICULOUS_LEVEL);

    Utility.Dprint("...return from parseContext()", Utility.TRACE_RIDICULOUS_LEVEL);
    return vw;
  }



  public static void waitForKey(boolean flag){
    if(!flag)
      return;

    char[] buf = new char[64];
    InputStreamReader rdr = new InputStreamReader(System.in);
    StringTokenizer stkn = null;

    try {
      System.out.print("Please hit enter to continue>>> ");
      rdr.read(buf, 0, 64);
    }
    catch(IOException e){
      System.out.println("io error");
      System.out.println(e);
      System.exit(1);
    }
  }


}




