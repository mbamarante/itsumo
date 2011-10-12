/*
 * Created on Feb 21, 2005
 *
 */
package frodo.MeetingScheduling.SocialChoice;

import java.util.Iterator;
import java.util.Random;
import java.util.Vector;

import frodo.COP.OCOPProblemFactory;
import frodo.COP.StabilityConstraint;
import frodo.COP.ValuedConstraint;
import frodo.CSP.CSPAgentInterface;
import frodo.CSP.CSPVariable;
import frodo.CSP.VariableId;
import frodo.MeetingScheduling.Meeting;
import frodo.MeetingScheduling.Hierarchical.Unit;
import frodo.OCOP.OCOPAbstractAgent;
import frodo.OCOP.OCOPAbstractNode;
import frodo.OCOP.OCOPAbstractProblem;
import frodo.OCOP.OCOPBasicNode;
import frodo.OCOP.DPOP.DPOPAgent;
import frodo.OCOP.communication.Environment;
import frodo.OCOP.graph.DFSMaster;
import frodo.OCOP.graph.DFSStatistics;
import frodo.utils.HelperUtilities;


/**
 * @author apetcu
 */
public class Generator {
	Environment environment;

	Random randomGenerator;

	int meetings;

	/**
	 * @param environment
	 */
	public Generator(Environment environment) {
		//		super();
		this.environment = environment;
		randomGenerator = new Random();
	}

	//	-----------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------//
	/*
	 public String generateRandomValuations(int values, int maxval) {
	 StringBuffer sb = new StringBuffer();
	 for (int i = 0; i < values; i++) {
	 for (int j = 0; j < values; j++) {
	 int val = randomGenerator.nextInt(maxval);
	 sb.append(" (" + i + "," + j + " -> " + val + ")");
	 }
	 }
	 return sb.toString();
	 }
	 */
	//-----------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------//
	public Unit generateOrg(int nodes, int dept_size, int branching) {
		Vector fifo = new Vector();

		Unit root = new Unit(new Integer(0), null);
		Unit cur = root;
		Unit parent = root;
		fifo.add(root);
		for (int i = 0; i < nodes; i++) {
			//			System.out.println("New dept. " + newUnit.getId());
			//			System.out.println("Adding agent " + i);

			if (i % dept_size == 0 && i > 0) {
				if (parent.getChildren().size() == branching) {
					//that means we need to change the parent, this one is full
					fifo.removeElementAt(0);//remove the old one
					parent = (Unit) fifo.elementAt(0);//take a new parent from the fifo
				}
				Unit newUnit = new Unit(new Integer(cur.getId().intValue() + 1), parent);
				//				System.out.println("New dept. " + newUnit.getId() + " parent=" + parent.getId());
				fifo.add(newUnit);
				cur = newUnit;
				parent.addChild(cur);
			}

			OCOPAbstractAgent ag = new DPOPAgent(environment, i);
			OCOPAbstractNode nd = new OCOPBasicNode(ag, ag.getAgentId(), ag.getLogger());
			ag.setNode(nd);
			cur.addAgent(ag);
			//			System.out.println("Adding agent " + i + " with node=" + ag.getCSPNode());
		}
		return root;
	}

	//-----------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------//
	public Unit generateSchedulingProblem(Unit org, int intM, int intSize, int extM, int extS) {
		System.out.println("Generating " + intM + " meetings in group " + org.getId());
		//			System.out.println("Remaining " + meetings + " meetings");
		Vector units = new Vector();
		units.add(org);
		generateMeetingsInGroups(units, intM, intSize);

		Iterator iter = org.getChildren().values().iterator();
		int howmchildren = org.getChildren().size();
		while (iter.hasNext()) {
			Unit child = (Unit) iter.next();
			units.removeAllElements();
			units.add(org);
			units.add(child);
			int externalM = extM / howmchildren;
			if (externalM == 0) {
				externalM++;
			}
			System.out.println("Generating " + externalM + " meetings between groups " + org.getId() + " and " + child.getId());
			generateMeetingsInGroups(units, externalM, extS);
			generateSchedulingProblem(child, intM, intSize, extM, extS);
		}

		//		OCOPProblem prob = new OCOPProblem();
		//		return prob;
		return org;
	}

	//-----------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------//
	public void meetingToCOP(Vector agents) {
		CSPVariable previous = null;
		OCOPAbstractNode prevN = null;
		for (int i = 0; i < agents.size(); i++) {
			CSPAgentInterface cspai = (CSPAgentInterface) agents.elementAt(i);
			OCOPAbstractNode nd = (OCOPAbstractNode) cspai.getCSPNode();
			//					System.out.println(which + " ->Agent " + cspai.getAgentId() + " has node=" + nd);

			//build the ID of the new variable= agent ID, local id=nr of variables (count from 0)
			VariableId vid = new VariableId(cspai.getAgentId(), nd.getNumberOfVariables());
			//build the domain of the new variable
			Vector dom = new Vector();
			for (int pp = 0; pp < 8; pp++)
				dom.add(new Integer(pp));
			//build the new variable, and add it to the node
			CSPVariable curvar = new CSPVariable(vid, dom);
			nd.addVariable(curvar);

			//I am adding a unary constraint to this variable, to show the preference of the agent
			ValuedConstraint unaryC = new ValuedConstraint(curvar);
			for (int j = 0; j < curvar.getDomainSize(); j++) {
				unaryC.setValuation(curvar.getValue(j), randomGenerator.nextInt(curvar.getDomainSize()));
			}
			unaryC.setOwnerAgent(cspai);
			unaryC.setOwnerAgentId(nd.getId());
			curvar.addConstraint(unaryC);
			nd.addConstraint(unaryC);

			//I have to add mutex constraints between this last var and all other vars in this node
			for (int j = 0; j < nd.getNumberOfVariables(); j++) {
				CSPVariable tmpv = nd.getVariable(j);
				//				ValuedConstraint diff = new ValuedConstraint(tmpv, curvar);
				ValuedConstraint diff = new ValuedConstraint(tmpv, curvar);
				diff.setRandomValuations(randomGenerator, 10);
				diff.setOwnerAgentId(nd.getId());
				diff.setOwnerAgent(cspai);
				tmpv.addConstraint(diff);
				nd.addConstraint(diff);
				ValuedConstraint diff2 = new ValuedConstraint(curvar, tmpv);
				diff2.setOwnerAgentId(nd.getId());
				diff2.setOwnerAgent(cspai);
				diff2.setValuations(HelperUtilities.intArrayTranspose(diff.getValuations()));
				curvar.addConstraint(diff2);
			}

			//add an equality constraint between this var and the last one
			if (previous != null) {
				//if this is not the first variable, then add eq constraint
				ValuedConstraint bc = new ValuedConstraint(previous, curvar);
				bc.setRandomValuations(randomGenerator, 10);
				previous.addConstraint(bc);
				prevN.addConstraint(bc);
				ValuedConstraint bc2 = new ValuedConstraint(curvar, previous);
				bc2.setValuations(HelperUtilities.intArrayTranspose(bc.getValuations()));
				curvar.addConstraint(bc2);
				nd.addConstraint(bc2);
			}
			prevN = nd;
			previous = curvar;
		}
	}

	//-----------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------//
	/*
	 public void meetingToCOPCSP(Vector agents) {
	 CSPVariable previous = null;
	 ODPOPNode prevN = null;
	 for (int i = 0; i < agents.size(); i++) {
	 CSPAgentInterface cspai = (CSPAgentInterface) agents.elementAt(i);
	 ODPOPNode nd = (ODPOPNode) cspai.getCSPNode();
	 //					System.out.println(which + " ->Agent " + cspai.getAgentId() + " has node=" + nd);

	 //build the ID of the new variable= agent ID, local id=nr of variables (count from 0)
	 VariableId vid = new VariableId(cspai.getAgentId(), nd.getNumberOfVariables());
	 //build the domain of the new variable
	 Vector dom = new Vector();
	 for (int pp = 0; pp < 8; pp++)
	 dom.add(new Integer(pp));
	 //build the new variable, and add it to the node
	 CSPVariable curvar = new CSPVariable(vid, dom);
	 nd.addVariable(curvar);

	 //I am adding a unary constraint to this variable, to show the preference of the agent
	 ValuedConstraint unaryC = new ValuedConstraint(curvar);
	 for (int j = 0; j < curvar.getDomainSize(); j++) {
	 unaryC.setValuation(curvar.getValue(j), randomGenerator.nextInt(curvar.getDomainSize()));
	 }
	 unaryC.setOwnerAgent(cspai);
	 unaryC.setOwnerAgentId(nd.getId());
	 curvar.addConstraint(unaryC);
	 nd.addConstraint(unaryC);

	 //I have to add mutex constraints between this last var and all other vars in this node
	 for (int j = 0; j < nd.getNumberOfVariables(); j++) {
	 CSPVariable tmpv = nd.getVariable(j);
	 //				ValuedConstraint diff = new ValuedConstraint(tmpv, curvar);
	 InequalityConstraint diff = new InequalityConstraint(tmpv, curvar);
	 diff.setOwnerAgentId(nd.getId());
	 diff.setOwnerAgent(cspai);
	 tmpv.addConstraint(diff);
	 nd.addConstraint(diff);
	 diff = new InequalityConstraint(curvar, tmpv);
	 diff.setOwnerAgentId(nd.getId());
	 diff.setOwnerAgent(cspai);
	 curvar.addConstraint(diff);
	 }

	 //add an equality constraint between this var and the last one
	 if (previous != null) {
	 //if this is not the first variable, then add eq constraint
	 EqualityConstraint bc = new EqualityConstraint(previous, curvar);
	 previous.addConstraint(bc);
	 prevN.addConstraint(bc);
	 bc = new EqualityConstraint(curvar, previous);
	 curvar.addConstraint(bc);
	 nd.addConstraint(bc);
	 }
	 prevN = nd;
	 previous = curvar;
	 }
	 }
	 */
	//-----------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------//
	public void generateMeetingsInGroups(Vector grps, int mtgs, int meetingSize) {
		//		System.out.print("Generating " + mtgs + " meetings in groups ");
		Vector agents = new Vector();
		for (int i = 0; i < grps.size(); i++) {
			Unit curu = (Unit) grps.elementAt(i);
			agents.addAll(curu.getAgents());
			//			System.out.print(curu.getId() + " ");
		}
		//		System.out.println();
		Vector meetingAgents = new Vector();//this is going to hold all agents involved in a meeting
		for (int i = 0; i < mtgs; i++) {
			meetings++;

			boolean meeting[] = new boolean[agents.size()];
			meetingAgents.removeAllElements();

			//we need to make sure that there is at least one participant from each dept
			int offset = 0;//this is to compute the starting index of each department in the array "meeting"
			for (int j = 0; j < grps.size(); j++) {
				Unit curu = (Unit) grps.elementAt(j);
				int which = randomGenerator.nextInt(curu.getAgents().size());
				meeting[which + offset] = true;
				offset = offset + curu.getAgents().size();
				meetingAgents.add(curu.getAgents().toArray()[which]);
				//			System.out.print(curu.getId() + " ");
			}

			//a random no of participants
			int howmP = randomGenerator.nextInt(meetingSize) - grps.size();
			if (howmP < 2)
				howmP = 2;
			while (howmP > 0) {
				int which = randomGenerator.nextInt(agents.size());
				if (!meeting[which]) {
					meeting[which] = true;
					meetingAgents.add(agents.elementAt(which));
					howmP--;
				}
			}
			System.out.print("Meeting " + i + "->>>>>>");
			for (int j = 0; j < meetingAgents.size(); j++) {
				System.out.print(((CSPAgentInterface) meetingAgents.elementAt(j)).getAgentId() + " ");
			}
			System.out.println("");
			meetingToCOP(meetingAgents);
			int agentsvector[] = new int[meetingAgents.size()];
			int valuationsvector[] = new int[meetingAgents.size()];
			for (int j = 0; j < meetingAgents.size(); j++) {
				agentsvector[j] = ((CSPAgentInterface) meetingAgents.elementAt(j)).getAgentId();
			}
			((Unit) grps.elementAt(0)).addMeeting(new Meeting(agentsvector, valuationsvector));
		}
	}

	//-----------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------//
	public OCOPAbstractProblem introduceChanges(OCOPAbstractProblem srcp, int percentChange) {
		//this method takes a OCOPProblem (single var per node already), and changes percentChange unary constraints out of it
		boolean changed[] = new boolean[srcp.getNumberOfVariables()];
		int howMany = srcp.getNumberOfVariables() * percentChange / 100;
		System.out.println("Will change " + howMany + " unary constraints out of " + srcp.getNumberOfVariables());
		int i = 0;
		while (i < howMany) {
			boolean found = false;
			while (!found) {
				int which = randomGenerator.nextInt(srcp.getNumberOfVariables());
				if (changed[which] == false) {
					found = true;
					i++;
					CSPVariable var = srcp.getVariable(which);
					OCOPAbstractNode nd = srcp.getNodeById(var.getId().getAgentID());
					ValuedConstraint unary = (ValuedConstraint) nd.getUnaryConstraints().elementAt(0);
					System.out.println("Will change unary constraint on " + var.getId() + " from\n" + unary.toString());
					//I am changing the unary constraint on this variable, to simulate the change of preferences of the agent
					for (int j = 0; j < var.getDomainSize(); j++) {
						unary.setValuation(var.getValue(j), randomGenerator.nextInt(var.getDomainSize()));
					}
					System.out.println("Changed unary constraint on " + var.getId() + " to\n" + unary.toString());
				}
			}
		}
		return srcp;
	}

	//-----------------------------------------------------------------------------------------//
	public OCOPAbstractProblem addStabilityConstraints(OCOPAbstractProblem problem) {
		//this method takes a OCOPProblem (single var per node already), and adds stability constraints to each of its nodes.
		System.out.println("Will generate " + problem.getNumberOfNodes() + " stability constraints.");
		for (int i = 0; i < problem.getNumberOfNodes(); i++) {
			CSPVariable var = problem.getVariable(i);
			OCOPAbstractNode node = problem.getNodeById(var.getId().getAgentID());
			StabilityConstraint sc = new StabilityConstraint(var);
			//			sc.setRandomValuations(randomGenerator,var.getDomainSize());
			sc.setValuations(StabilityConstraint.randomValuations(var.getDomainSize(), var.getDomainSize()));
			node.addConstraint(sc);
			var.addConstraint(sc);
			problem.addConstraint(sc);
			System.out.println("Generated stability constraint on " + var.getId() + " to\n" + sc.toString());
		}
		return problem;
	}

	//-----------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------//
	public static void main(String[] args) {
		if (args.length < 7) {
			System.out
					.println("Usage: Generator file agents(total) agents/department branching(depts) meetings_internal meetingsize_internal meetings_external meetingsize_external percent_changes_start percent_changes_steps ");
		} else {
			String file = args[0];
			int agents = Integer.parseInt(args[1]);
			int agentsperdept = Integer.parseInt(args[2]);
			int branching = Integer.parseInt(args[3]);
			int meetings_internal = Integer.parseInt(args[4]);
			int meetingsize_internal = Integer.parseInt(args[5]);
			int meetings_external = Integer.parseInt(args[6]);
			int meetingsize_external = Integer.parseInt(args[7]);

			Generator gen = new Generator(null);
			boolean done = false;
			OCOPAbstractProblem problem1 = null;
			while (!done) {
				Unit myOrg = gen.generateOrg(agents, agentsperdept, branching);
				System.out.println("Generated problem:\n" + myOrg.toStringAll(0));
				gen.meetings = 0;
				gen.generateSchedulingProblem(myOrg, meetings_internal, meetingsize_internal, meetings_external, meetingsize_external);
				OCOPAbstractProblem problem = myOrg.toOCOPProblem();
				problem.saveToTextFile(file);
				problem1 = OCOPProblemFactory.toSingleVariablePerNode(problem);
				if (!problem1.isConnected()) {
					System.out.println("Disconnected problem!!!!");
					//					System.exit(1);
				} else {
					System.out.println("MEETINGS:\n" + myOrg.toStringSerialize());
					myOrg.saveToTextFile(file + ".mtg");
					//					gen.addStabilityConstraints(problem1);
					problem1.saveToTextFile(file + ".1");
					problem1 = OCOPProblemFactory.loadFromTextFile(file + ".1",new OCOPBasicNode(null,0,null));
					DFSMaster.getMinimalWidthDFS_MCN(problem1);
					DFSStatistics statistix  = DFSMaster.getStatisticsOfDFS(problem1);
					System.out.println("#agents\tmeetings\tvariables\tconstraints\tDPOP");
					System.out.println(agents + "\t" + gen.meetings + "\t" + problem1.getNumberOfVariables() + "\t" + problem1.getNumberOfConstraints() + "\t" + statistix.getWidth());

					done = true;
				}
				Vector clusters = DFSMaster.getClusters(problem1);
				System.out.println("\n---------------\n");
				System.out.println(clusters.size() + " clusters.");
			}
		}
	} //-----------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------//
}