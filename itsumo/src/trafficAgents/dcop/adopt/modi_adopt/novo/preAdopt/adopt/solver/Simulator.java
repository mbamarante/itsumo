/*
 *    Simulator.java
 *    Author: Jay Modi
 *    Date: Oct 24  2002
 *
 *    Coordinates running a DCOP algorithm on a DCOP.
 */

package adopt.solver;

import adopt.common.*;
import adopt.problem.*;

import java.util.*;
import java.io.*;

public class Simulator implements MessageSender {

  /* A DCO Problem */
  public static Problem pbm;
  /* type of DCOP */
  String problemType;
  /* problem input file (must be of type 'problemType') */
  public static String inputFileName;
  /* name of algorithm */
  String algorithmName;
  /* tree type */
  int treeType;
  /* Sub factor */
  int sub;
  /* Div factor */
  int div;
  public static int dprog; 	
  /* name of a special "master" agent.
     used to collect problem solution.
     */
  String masterName;
  /* Data logger */
  Logger nLog;
  /* List of Adopt agents */
  Vector Agents;
  /* input message queues for each agent.  */
  Hashtable msgQs = null;
  /* total number of messages sent in the system */
  int numsent=0;
  /* total number of messages rcvd in the system */
  int numrcvd=0;
  /* total number of *counted* messages sent in the system.
   (some messages are not counted in the final stats, e.g., messages sent to
   the MASTER are not counted. */
  int msgCnt = 0;
  double msgLength = 0;


  long timeStart;
  long timeEnd;
  /* how often to print out stats (ms) */
  int pollTime = 5000;
  /* time elapsed after which to terminate program (sec)*/
  int timeMax = 87200; // 2 hrs

  boolean mutex = false;
  
  public static Context ValueVector;

  public Simulator (String args[]) {
    try{
      if (args.length < 5){
	System.out.println("Usage: Simulator " +
			   "<algorithm-name> <problem-type> <input-filename> <dp type > ");
	System.exit(0);
      }

      algorithmName = args[0];
      problemType = args[1];
      inputFileName = args[2];
      treeType = Integer.parseInt(args[3]);
      dprog = Integer.parseInt(args[4]);
      if(args.length > 5)
      {
	      sub = Integer.parseInt(args[5]);
	      div = Integer.parseInt(args[6]);
	  }

      masterName = "agent"+Master.masterID;

      /*Instrumentation Code*/
      PrintWriter pwtr = null;
      String fname = "Logs/Summary.txt";
      try{
	FileWriter ffile = new FileWriter(fname);
	pwtr = new PrintWriter(ffile);
      }catch(IOException e){
	System.out.println("Error opening "+fname+" for logging.");
	System.out.println("Exiting.");
	System.exit(0);
      }
      nLog = new Logger(pwtr);
      logAlgorithmName(nLog, algorithmName);
      /*Done Instrumentation Code*/


    } catch (Exception ex) {
      System.err.println(ex.getMessage());
    }

    /* init message queues */
    msgQs = new Hashtable(Collections.synchronizedMap(new Hashtable()));

  }

  public void run () {

      /* read Problem */
      if(problemType.equals("maxcsp-chain")){
	System.out.println("----------Problem: maxcsp-chain ----------");
	nLog.printToLogln("Problem MaxCSP (Random Chain)");
	pbm = new MaxCSPProblem(inputFileName, Problem.RANDOMCHAIN);
      }
      else if(problemType.equals("maxcsp-tree")){
	System.out.println("----------Problem: maxcsp-tree ----------");
	pbm = new MaxCSPProblem(inputFileName, Problem.TREE);
	nLog.printToLogln("Problem MaxCSP (Tree, depth = " + pbm.orderDepth() + ")");
	if(algorithmName.startsWith("sbb") ||
	   algorithmName.startsWith("sid")){
	  pbm.convertToChain();
	}
      }
      else if(problemType.equals("maxcop-tree")){
	System.out.println("----------Problem: maxcop-tree ----------");
	Problem.sub = sub;
	Problem.div = div;
	pbm = new MaxCOPProblem(inputFileName, Problem.TREE,treeType);
	nLog.printToLogln("Problem MaxCOP (Tree, depth = " + pbm.orderDepth() + ")");
      }
      else{
	System.out.println("Unknown problem: " + problemType);
	System.out.println("Exiting.");
	System.exit(0);
      }

//tree gen 
	if(dprog == 0 )
	{
	dp0 ntest = new dp0();   //Preprocessing steps   
	ntest.preprocess();
	}
	else if (dprog == 1)
	{
		
		dp1 ntest = new dp1();
		ntest.preprocess();
	}
	else if (dprog == 2) 
	{
		dp2 ntest = new dp2();
		ntest.preprocess();
	}
	else if(dprog == 4)
	{	
		double thresholdPercentage = 0.7;
		randomPreprocess(thresholdPercentage);
	
	}		
			
	
      /* copy problem input file to Logs directory */
      try{Runtime.getRuntime().exec("cp " + inputFileName + " Logs/");}catch(Exception e){}

      /* init list of agents */
      Agents = new Vector();
      /* init Master agent */
      AgentThread master = new AgentThread(Master.masterID, pbm, "master", this);
      Agents.add(master);
      addMsgReceiver(masterName);

      /* for each agent in the problem, create an AgentThread */
      for(int i=0;i<pbm.numAgents();i++){
	int agentID = ((Integer) pbm.agents.elementAt(i)).intValue();

	if(agentID == Master.masterID){
	  System.out.println(" Agent" + Master.masterID + " is a reserved agent ID!");
	  System.out.println(" Please choose another ID and restart. ");
	  System.exit(0);
	}

	AgentThread at = new AgentThread(agentID, pbm, algorithmName, this);
	Agents.add(at);
	addMsgReceiver("agent"+agentID);
      }

      Utility.Dprint("Simulator(): Begin Simulator run...");
      System.out.println("Begin algorithm execution...");

      timeStart = Utility.TimeMS();

      /* start each agent thread */
      for(int i=0;i<Agents.size();i++){
      	
	AgentThread at = (AgentThread) Agents.elementAt(i);
	ValueVector = new Context();
	at.start();

      }

      while(true){
	try {
	  Thread.sleep(pollTime);
	  /* check for termination */
	  boolean termFlag = true;
	  for(int i=0;i<Agents.size();i++){
	    AgentThread at = (AgentThread) Agents.elementAt(i);
	    /* check to see if some (non-master) agent does not want to terminate*/
 	    if(!at.algorithm.iWantToTerminate){
	      if(at.algorithm.localAgentID != Master.masterID){
		termFlag = false;
		break;
	      }
	    }
	  }
	  if(termFlag){
	    double t = (Utility.TimeMS() - (double)timeStart)/1000;
	    String s = "  All agents terminated";
	    System.out.println(s + Instrumentation.fillerString(s) + t) ;
	    s = "  Total msgs sent";
	    System.out.println(s + Instrumentation.fillerString(s)
			       + " " + msgCnt);
	    printSummaryAndTerminate(false);
	  }
	  else{
	    /* print execution status information */
	    double t = (Utility.TimeMS() - (double)timeStart)/1000;
	    String s = "  time elapsed(sec)";
	    System.out.println(s + Instrumentation.fillerString(s) + t) ;
	    s = "  total msgs communicated";
	    System.out.println(s + Instrumentation.fillerString(s) + numsent
			       + " (rcvd=" + numrcvd + ")") ;
	    if (t > timeMax){
	      s = "  Max Time Elapsed";
	      System.out.println(s + Instrumentation.fillerString(s) + t) ;
	      if(nLog != null){
		nLog.printToLogln("Error maxtime " + timeMax);
	      }
	      printSummaryAndTerminate(true);
	    }
	  }
	}catch ( Exception e ) { }
      }
  }

  /* errorCondition == true means terminating abnormally */
  public void printSummaryAndTerminate(boolean errorCondition){
    /* ending time is the latest ending time of any agent */
    timeEnd = 0;
    for(int i=0;i<Agents.size();i++){
      AgentThread at = (AgentThread) Agents.elementAt(i);
      if(at.algorithm.timeEnd > timeEnd){
	timeEnd = at.algorithm.timeEnd;
      }
    }

    double elapsed = ((double)timeEnd-timeStart)/1000;

    for(int i=0;i<Agents.size();i++){
      AgentThread at = (AgentThread) Agents.elementAt(i);
      if(at.algorithm.localAgentID == Master.masterID){
	/* inform master that all agents are dead */
	((Master)at.algorithm).agentsAlive = false;
	/* wait for master to finish up */
	while(!at.algorithm.iWantToTerminate){
	  System.out.println("Simulator waiting for Master to finish");
	  try { Thread.sleep(1000);
	  } catch ( Exception e ) { }
	}
	System.out.println("Done. ");

	/**************************/
	/* Check for any lost messages */

	while(mutex)
	  try {Thread.sleep(10);}
	catch ( Exception e ) { }
	mutex = true;
	/* count the number of messages left on message queues */
	int cnt=0;
	Message msg=null;
	for(int j=0;j<Agents.size();j++){
	  AgentThread at2 = (AgentThread) Agents.elementAt(j);
	  int id = at2.algorithm.localAgentID;
	  LinkedList q = (LinkedList) msgQs.get("agent"+id);
	  if(q != null){

	    try{msg = (Message) q.removeFirst();}
	    catch(NoSuchElementException ex){ msg = null;}
	    while(msg != null){
	      cnt++;
	      try{msg = (Message) q.removeFirst();}
	      catch(NoSuchElementException ex){ msg = null;}
	    }
	  }
	}
	if(numsent != numrcvd + cnt){
	  System.out.println("Simulator error....some messages may"
 			     + "have been lost or otherwise unaccounted for!");
	  String s = "  Error stats (sent,rcvd,undlvrd)";
	  System.out.println(s + Instrumentation.fillerString(s)
			     + " " + numsent
			     + " " + numrcvd
			     + " " + cnt);
	  errorCondition = true;
	  if(nLog != null){
	    nLog.printToLogln("Error msgLoss "
			     + " " + numsent
			     + "," + numrcvd
			     + "," + cnt);

	  }
	}

	/**************************/

	System.out.println("\n\n");
	System.out.println("Solution");
	System.out.println("--------------------");
	((Master)at.algorithm).computeQuality(0);
	((Master)at.algorithm).logSolution();

	/* Print summary Information */
	if(nLog != null){
	  if(errorCondition){
	    nLog.printToLogln("TOTALTIME=" + elapsed);
	  }
	  else{
	    nLog.printToLogln("TOTALTIME=" + elapsed);
	  }

	double avgCycle = 0;
	  for(int xxi=0;xxi<Agents.size();xxi++){
      		AgentThread ata = (AgentThread) Agents.elementAt(xxi);
		avgCycle += ata.algorithm.cycleCnt;
      	  }
	  avgCycle = avgCycle / Agents.size();
	  nLog.printToLogln("CYCLES=" + avgCycle);
	  
	  nLog.printToLogln("MSGS=" + msgCnt);

	double avgMsgLength = msgLength/msgCnt;	

	  nLog.printToLogln("AVGMSGSIZE=" + avgMsgLength);
	  nLog.printToLogln(" ");
	  nLog.printToLogln("FinalSolQlty " + ((Master)at.algorithm).finalQuality());
	  nLog.printToLogln("BestSolQlty " + ((Master)at.algorithm).bestQuality());

	}
	break;
      }
    }

    System.out.println("********* TotalTime (secs) " + elapsed
		       + " (pollTime " + pollTime/1000 + ") *********");
    System.out.println("********* Thanks for playing ************");
    System.exit(0);
  }


  /*Create an input msg queue agent 'aname'.
    */
  public void addMsgReceiver(String aname){
    LinkedList msgQ = new LinkedList(Collections.synchronizedList(new LinkedList()));
    msgQs.put(aname, msgQ);
  }

  /* some agent wants to send message 'm' to 'aname'.
    'cntFlag': count this message in the final stats?
    */
  public void sendMessage(Message m, String aname, boolean cntFlag){
    while(mutex)
      try {Thread.sleep(10);}
    catch ( Exception e ) { }
    mutex = true;
    LinkedList q = (LinkedList) msgQs.get(aname);
    if(q != null){
      q.addLast(m);
      numsent++;
      if(cntFlag) {
	msgCnt++;
	msgLength += m.message.length();
      }	
    }
    else{
      System.out.println("Simulator.sendMessage(): Unknown agent: " + aname);
    }
    mutex = false;
  }

  /* agent 'aname' wants a message */
  public Message getMessage(String aname){
    Message msg=null;
    while(mutex)
      try {Thread.sleep(10);}
    catch ( Exception e ) { }
    mutex = true;
    LinkedList q = (LinkedList) msgQs.get(aname);

    if(q != null){
      try{msg = (Message) q.removeFirst();}
      catch(NoSuchElementException ex){ msg = null;}
      if(msg != null){
	numrcvd++;
	mutex = false;
	return msg;
      }
    }
    else{
      System.out.println("Simulator.receiveMessage(): Unknown agent: " + aname);
    }
    mutex = false;
    return null;
  }


  public static void logAlgorithmName(Logger l, String algoName){

    /* Do some logging *************/
    if(algoName.equals("adopt")){
      Utility.Dprint("----------Algorithm: Adopt ----------", Utility.DEBUG);
      l.printToLogln("Algorithm Adopt");
    }
    else if(algoName.equals("master")){
      Utility.Dprint("----------Algorithm: Master ----------", Utility.DEBUG);
    }
    else if(algoName.equals("sbb")){
      Utility.Dprint("----------Algorithm: SBB ----------", Utility.DEBUG);
      l.printToLogln("Algorithm SBB");
    }
    else if(algoName.equals("sid")){
      Utility.Dprint("----------Algorithm: SID ----------", Utility.DEBUG);
      l.printToLogln("Algorithm SID");
    }
    /* test algorithm with message loss */
    else if(algoName.startsWith("adopt-loss")){
      int lr = new Integer(algoName.substring(algoName.lastIndexOf("s")+1)).intValue();

      Utility.Dprint("----------Algorithm: Adopt (msgLoss"+lr+") ----------",
		     Utility.DEBUG);
      l.printToLogln("Algorithm Adopt (msgLoss"+lr+")");
    }
    /* test algorithm with different solution quality error bounds */
    else if(algoName.startsWith("adopt-bound")){
      int lr = new Integer(algoName.substring(algoName.lastIndexOf("d")+1)).intValue();

      Utility.Dprint("----------Algorithm: Adopt (bound"+lr+") ----------",
		     Utility.DEBUG);
      l.printToLogln("Algorithm Adopt (bound"+lr+")");
    }
    else{
      l.printToLogln("Algorithm Unknown");
    }
    /* End logging *************/
  }

  public static void main (String argv[]) {
    Simulator sim = new Simulator(argv);
    sim.run();
  }
  public void randomPreprocess(double percent)
  {
	try {
     	System.out.println("Reading threshold file");
     	String infilename = "solutions/"+Simulator.inputFileName+"_3.thr";
        BufferedReader inputThreshFile = new BufferedReader(new FileReader(infilename));
        String line = null;
        StringTokenizer t;
        Variable x_i = null;
          //System.out.println("1");
        while ((line = inputThreshFile.readLine()) != null) {
        	
         // System.out.println("1");
          //if(line.startsWith("\n") )continue;
        	 t = new StringTokenizer(line);
		  if(t.hasMoreElements()) t.nextToken(); // get rid of first token
      			else continue;
	      if(line.startsWith("Var")){
	      	Integer varID = new Integer(t.nextToken());
	      	 x_i = pbm.getVariableFromID(varID.intValue());
	      	//System.out.println("Var = "+ varID.intValue()); 
	     	}
	      if(line.startsWith("Val")){
	      	Integer value = new Integer(t.nextToken());
			Integer threshold = new Integer(t.nextToken());
        	Value val = x_i.getValue(value.toString());
        	NominalValue nomVal = new NominalValue(val.toString());
        	
        	x_i.setInitThreshold(nomVal,(int)(threshold.intValue()*percent));
        	System.out.println("Setting Var "+x_i.varIDof()+" Val "+value.intValue()+"="+threshold.intValue()*percent);
        	}
        	                  
        }
        inputThreshFile.close();
    } catch (IOException e) {
    	System.out.println(e);
    }

	
}


}