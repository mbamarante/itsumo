/*
 *    AgentThread.java
 *    Author: Jay Modi
 *    Date: Nov 12  2002

 *    One Adopt agent thread.
 */

package adopt.solver;

import adopt.common.*;
import adopt.problem.*;

import javax.swing.event.EventListenerList;
import java.util.*;
import java.io.*;

public class AgentThread extends Thread {

  /* DCOP Problem Name */
  Problem pbm;
  /* Algorithm Name */
  Algorithm algorithm;
  /* used to send messages to other agents */
  MessageSender mSender;
  /* for logging output */
  Logger nLog = null;

  /* Create agent thread with name 'agentName'. */
  public AgentThread (int agentID, Problem p, String algorithmName, 
		      MessageSender ms) {

    pbm = p; 
    mSender = ms;

    if(algorithmName.equals("adopt")){
      algorithm = new Intradopt(agentID, pbm, mSender);
    }
    else if(algorithmName.equals("sbb")){
      algorithm = new IntraSBB(agentID, pbm, mSender);
    }

    else if(algorithmName.startsWith("adopt-loss")){
      int lr = new Integer(algorithmName.substring(algorithmName.lastIndexOf("s")+1)).intValue();
      algorithm = new Intradopt(agentID, pbm, mSender);
      algorithm.msgLossFlag = true;
      algorithm.lossRate = lr;
      algorithm.lossRate2 = lr;
    }
    else if(algorithmName.startsWith("adopt-bound")){
      int lr = new Integer(algorithmName.substring(algorithmName.lastIndexOf("d")+1)).intValue();
      algorithm = new Intradopt(agentID, pbm, mSender);
      algorithm.BOUND = lr;
    }
    else if(algorithmName.startsWith("master")){
      setPriority(Thread.MIN_PRIORITY);
      algorithm = new Master(agentID, pbm, mSender);
    }
//     else if(algorithmName.startsWith("testcomm")){
//       algorithm = new TestComm(agentID, pbm, mSender);
//     }
    else{
      System.out.println("Unknown algorithm: " + algorithmName);
      System.exit(0);
    }


    /*Start Logging Setup*/
    PrintWriter pwtr = null;
    String fname = "Logs/agent"+agentID+".log";
    try{
      FileWriter ffile = new FileWriter(fname); 
      pwtr = new PrintWriter(ffile);   
    }catch(IOException e){
      System.out.println("Error in opening "+fname+" for logging");
      System.out.println("Exiting.");
      System.exit(1);
    }
    nLog = new Logger(pwtr);
    algorithm.setLogger(nLog);
    /*End Logging Setup*/

  }

  public void start () {
    super.start();
  }
  
  /** 
      Main loop
  **/
  public void run () { 
    algorithm.run(); 
  }
}
