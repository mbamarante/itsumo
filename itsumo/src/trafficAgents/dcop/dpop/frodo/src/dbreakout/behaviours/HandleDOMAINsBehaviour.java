package frodo.dbreakout.behaviours;

import java.util.StringTokenizer;
import java.util.Vector;

import frodo.CSP.VariableId;
import frodo.communication.DistributedAgentInterface;
import frodo.communication.Message;
import frodo.communication.MessageQueue;
import frodo.communication.MessageQueueProcessor;
import frodo.dbreakout.CSPAgent;
import frodo.dbreakout.CSP.CSPVariableDBA;


public class HandleDOMAINsBehaviour extends MessageQueueProcessor {

	//---------------------------------------------------------------------------//
	public HandleDOMAINsBehaviour(DistributedAgentInterface a, MessageQueue mq) {
		super(a, mq);
	}
	//---------------------------------------------------------------------------//
	public Vector processMessage(Message msg) {
		//particular case of DBreakout agent
		CSPAgent myDBAgent = (CSPAgent) myAgent;
		String content = msg.getPayload();
		
		Vector updates = new Vector();
		myLogger.println("-------->incoming DOMAIN:" + content);
		if (!myDBAgent.getAllResponded()) {
			try {
				StringTokenizer st = new StringTokenizer(content);
				if (st.hasMoreTokens()) {
					String v1 = st.nextToken();
					if (v1.equalsIgnoreCase("DOMAIN")) {
						while (st.hasMoreTokens()) {
							v1 = st.nextToken("]").trim(); // that's the var id + value (sth like "1.2[0, 4, 5]")
							if (v1.equals(""))
								break;
							myLogger.println("&&&&&&&&&&&&&&&&Gonna break up domain>" + v1 + "<");
							StringTokenizer st1 = new StringTokenizer(v1);
							String tmp = st1.nextToken(".").trim();
							int agid = Integer.parseInt(tmp);
							tmp = st1.nextToken("[").substring(1);
							int varid = Integer.parseInt(tmp);
							VariableId vid = new VariableId(agid, varid);
							CSPVariableDBA curVar = new CSPVariableDBA(vid, null, null);
							Vector curDomain = new Vector();
							String v2 = st1.nextToken(",").substring(1);
							curDomain.add(new Integer(v2.trim()));
							while (st1.hasMoreElements()) {
								v2 = st1.nextToken(",");
								curDomain.add(new Integer(v2.trim()));
							}
							curVar.setDomain(curDomain);
							curVar.setValue(new Integer(0));
							updates.add(curVar);
						}
						if (updates.size() == 0)
							myLogger.println("\t\t$$$$$$$$$$$$ received NULL domain!!!!!!!!!!!!!");

						myDBAgent.addDOMAINs(updates);
					}
				}
			} catch (Exception nsee) {
				myLogger.println("\n\n\n GOT problematic DOMAIN: " + content);
				nsee.printStackTrace();
				myAgent.terminate ("GOT problematic DOMAIN: " + content);
			}
		} else {
			myLogger.println("GOT ALL DOMAINs... plus this: " + content);
		}
		return updates;
	}
	//---------------------------------------------------------------------------//
}
