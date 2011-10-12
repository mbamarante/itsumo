/*
 * Created on Feb 21, 2005
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package frodo.MeetingScheduling.Hierarchical;

import java.io.FileWriter;
import java.util.Collection;
import java.util.Iterator;
import java.util.TreeMap;
import java.util.Vector;

import frodo.CSP.BinaryConstraint;
import frodo.CSP.CSPAgentInterface;
import frodo.CSP.CSPVariable;
import frodo.MeetingScheduling.Meeting;
import frodo.OCOP.OCOPAbstractNode;
import frodo.OCOP.OCOPAbstractProblem;
import frodo.OCOP.OCOPBasicProblem;


/**
 * @author apetcu
 */
public class Unit {
	//these are the agents in this unit
	TreeMap agents;

	//this is the boss of this unit
	CSPAgentInterface boss = null;

	//these are the children units
	TreeMap children = null;

	//the boss of this unit
	Integer id;

	//the parent of this unit
	Unit parent;

	//the internal meetings 
	Vector internalMeetings = new Vector();

	//the external meetings 
	Vector externalMeetings = new Vector();

	//all meetings
	Vector meetings = new Vector();

	//	---------------------------------------------------------------------------------//
	public Unit(Integer ids, Unit par) {
		agents = new TreeMap();
		children = new TreeMap();
		id = ids;
		parent = par;
	}

	//	---------------------------------------------------------------------------------//

	public void addAgent(CSPAgentInterface cspai) {
		agents.put(new Integer(cspai.getAgentId()), cspai);
	}

	public CSPAgentInterface getAgent(int agid) {
		return (CSPAgentInterface) agents.get(new Integer(agid));
	}

	/**
	 * @return Returns the boss.
	 */
	public CSPAgentInterface getBoss() {
		return boss;
	}

	/**
	 * @param boss The boss to set.
	 */
	public void setBoss(CSPAgentInterface boss) {
		this.boss = boss;
	}

	public boolean equals(Unit u) {
		if (u.hashCode() == hashCode())
			return true;
		else
			return false;
	}

	public int hashCode() {
		return id.intValue();
	}

	public void addChild(Unit chld) {
		children.put(chld.getId(), chld);
	}

	/**
	 * @return Returns the id.
	 */
	public Integer getId() {
		return id;
	}

	/**
	 * @param id The id to set.
	 */
	public void setId(Integer id) {
		this.id = id;
	}

	/**
	 * @return Returns the parent.
	 */
	public Unit getParent() {
		return parent;
	}

	/**
	 * @param parent The parent to set.
	 */
	public void setParent(Unit parent) {
		this.parent = parent;
	}

	/**
	 * @return Returns the children.
	 */
	public TreeMap getChildren() {
		return children;
	}

	//	---------------------------------------------------------------------------------//
	/**
	 * @param children The children to set.
	 */
	public void setChildren(TreeMap children) {
		this.children = children;
	}

	//---------------------------------------------------------------------------------//
	//---------------------------------------------------------------------------------//
	public void traverseDFS() {
		if (parent == null)
			System.out.println("Dept. " + id + " (boss=" + parent + ") has " + children.size() + " children.");
		else
			System.out.println("Dept. " + id + " (boss=" + parent.getId() + ") has " + children.size() + " children.");

		Iterator iter1 = agents.values().iterator();
		System.out.print("\t");
		while (iter1.hasNext()) {
			CSPAgentInterface cspai = (CSPAgentInterface) iter1.next();
			System.out.print(cspai.getAgentId() + " ");
		}
		System.out.println();

		Iterator iter = children.values().iterator();
		while (iter.hasNext()) {
			Unit child = (Unit) iter.next();
			child.traverseDFS();
		}
		//		unu.traverse();
	}

	//---------------------------------------------------------------------------------//
	//---------------------------------------------------------------------------------//
	public String toString() {
		StringBuffer sb;
		if (parent == null)
			sb = new StringBuffer("Dept. " + id + " (boss=" + "null)\n");
		else
			sb = new StringBuffer("Dept. " + id + " (boss=" + parent.getId() + ")\n");
		Iterator iter = children.values().iterator();
		while (iter.hasNext()) {
			Unit child = (Unit) iter.next();
			sb.append(child.toString());
		}
		return sb.toString();
	}

	//	---------------------------------------------------------------------------------//
	//---------------------------------------------------------------------------------//
	public String toStringAll(int level) {
		StringBuffer sb = new StringBuffer();
		for (int i = 0; i < level; i++)
			sb.append("\t");
		if (parent == null)
			sb.append("Dept. " + id + " (boss=" + "null): ");
		else
			sb.append("Dept. " + id + " (boss=" + parent.getId() + "): ");
		Iterator iter = agents.values().iterator();
		while (iter.hasNext()) {
			sb.append(((CSPAgentInterface) iter.next()).getAgentId() + " ");
		}
		sb.append("\n");
		iter = children.values().iterator();
		while (iter.hasNext()) {
			Unit child = (Unit) iter.next();
			sb.append(child.toStringAll(level + 1));
		}
		return sb.toString();
	}

	//	--------------------------------------------------------------------------------//
	public String toStringSerialize() {
		StringBuffer sb = new StringBuffer();
		sb.append(this.toStringAll(0));
		sb.append(toStringMeetings());
		return sb.toString();
	}

	//---------------------------------------------------------------------------------//
	public String toStringMeetings() {
		StringBuffer sb = new StringBuffer();
		for (int i = 0; i < meetings.size(); i++)
			sb.append(meetings.elementAt(i) + "\n");
		Iterator iter = children.values().iterator();
		while (iter.hasNext()) {
			Unit child = (Unit) iter.next();
			sb.append(child.toStringMeetings());
		}
		return sb.toString();
	}

	//	---------------------------------------------------------------------------------//

	/**
	 * @return Returns the agents.
	 */
	public Collection getAgents() {
		return agents.values();
	}

	/**
	 * @param agents The agents to set.
	 */
	public void setAgents(TreeMap agents) {
		this.agents = agents;
	}

	//---------------------------------------------------------------------------------//
	//---------------------------------------------------------------------------------//
	public OCOPAbstractProblem toOCOPProblem() {
		OCOPAbstractProblem prob = new OCOPBasicProblem();

		System.out.println("------------------------------------------");
		System.out.println("DFS iterative with root=" + getId());
		System.out.println("------------------------------------------");

		Vector stack = new Vector();
		//		boolean visited[] = new boolean[getNumberOfNodes()];
		int agents = 0;
		int vars = 0;
		stack.add(this);
		while (!stack.isEmpty()) {
			Unit curu = (Unit) stack.lastElement();
			System.out.print("--------------->>>> Stack: ");
			for (int i = 0; i < stack.size(); i++) {
				System.out.print(((Unit) (stack.elementAt(i))).getId() + " ");
			}
			System.out.println("");
			stack.remove(stack.size() - 1);
			System.out.println("Visiting dept " + curu.getId());
			Iterator iter = curu.getAgents().iterator();
			while (iter.hasNext()) {
				CSPAgentInterface cspai = (CSPAgentInterface) iter.next();
				OCOPAbstractNode node = (OCOPAbstractNode) cspai.getCSPNode();
				for (int i = 0; i < node.getNumberOfVariables(); i++) {
					CSPVariable oav = node.getVariable(i);
					if (oav.getNumberOfConstraints() > 0) {
						for (int j = 0; j < oav.getNumberOfConstraints(); j++) {
							BinaryConstraint constr = oav.getConstraint(j);
							if (constr.getSourceId().compareTo(constr.getDestinationId()) >= 0) {
								prob.addConstraint(constr);
							}
						}
						vars++;
						//						OCOPNode ndnew = new OCOPNode();

						prob.addVariable(oav);
					}
				}
				prob.addNode(node);
				agents++;
			}
			iter = curu.getChildren().values().iterator();
			while (iter.hasNext()) {
				Unit child = (Unit) iter.next();
				stack.add(child);
			}
		}
		//		prob.removeDisconnectedNodes();
		//		prob.removeDisconnectedVariables();
		//		prob.sortNodesById();
		//		prob.sortVariablesById();
		return prob;
	}

	//	---------------------------------------------------------------------------------//
	//	---------------------------------------------------------------------------------//
	public Vector getMeetings() {
		return meetings;
	}

	//	---------------------------------------------------------------------------------//

	public void setMeetings(Vector meetings) {
		this.meetings = meetings;
	}

	//	---------------------------------------------------------------------------------//

	public void addMeeting(Meeting meeting) {
		this.meetings.add(meeting);
	}

	//	---------------------------------------------------------------------------------//
	public boolean saveToTextFile(String fileName) {
		try {
			FileWriter fw = new FileWriter(fileName);
			fw.write(toStringSerialize());
			fw.close();
		} catch (Exception e) {
			System.out.println("Exception while saving problem to file:");
			e.printStackTrace();
			return false;
		}
		return true;
	}
	//	---------------------------------------------------------------------------------//
}